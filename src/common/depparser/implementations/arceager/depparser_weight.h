#ifndef _DEPPARSER_WEIGHTS_H
#define _DEPPARSER_WEIGHTS_H

#include "depparser_weight_base.h"


namespace TARGET_LANGUAGE {
namespace depparser {


template <size_t INDEX_BITS=24, Label NLABELS=action::MAX>
class FeatureHashtableDO {
public:
  static constexpr const Label PADDED_NLABELS = _MAKE_ALIGNED_32<SCORE_TYPE, NLABELS>::VALUE;

  static_assert((PADDED_NLABELS * sizeof(SCORE_TYPE)) % 32 == 0, "PADDED_NLABELS must be divisible by 32");
  static_assert(INDEX_BITS > 0, "INDEX_BITS must be non-zero");
  static_assert(INDEX_BITS <= 64, "INDEX_BITS cannot exceed 64 since the hash function is 64-bit.");

  static constexpr const size_t TABLE_INDEX_BITS = INDEX_BITS;  //!< Number of bits from the hash to use as the index into the table.
  static constexpr const uint64_t TABLE_INDEX_MASK = (static_cast<uint64_t>(1) << TABLE_INDEX_BITS) - static_cast<uint64_t>(1);
  static constexpr const size_t TABLE_SIZE = static_cast<size_t>(1) << TABLE_INDEX_BITS;

  static constexpr const size_t DEFAULT_CHAIN_POOL_BLOCK_SIZE = 8192 * 4080;  // ~32MB
  static constexpr const size_t DEFAULT_DATA_POOL_BLOCK_SIZE = 16384 * 2120;  // ~32MB
  static constexpr const uint64_t TYPE_ID_HASH_MIXER = 982451653ULL;

private:
  class Chain {
  private:
    uint64_t _hash;
    SCORE_TYPE *_current;
    SCORE_TYPE *_total;
    unsigned int *_last_updated;
    Chain *_next;

  public:
    Chain(const uint64_t hash, schwa::Pool &current_pool, schwa::Pool &total_pool, schwa::Pool &updated_pool) :
        _hash(hash),
        _current(current_pool.alloc<SCORE_TYPE *>(PADDED_NLABELS*sizeof(SCORE_TYPE))),
        _total(total_pool.alloc<SCORE_TYPE *>(PADDED_NLABELS*sizeof(SCORE_TYPE))),
        _last_updated(updated_pool.alloc<unsigned int *>(NLABELS*sizeof(unsigned int))),
        _next(nullptr)
      {
      std::memset(_current, 0, PADDED_NLABELS*sizeof(SCORE_TYPE));
      std::memset(_total, 0, PADDED_NLABELS*sizeof(SCORE_TYPE));
      std::memset(_last_updated, 0, NLABELS*sizeof(unsigned int));
    }
    ~Chain(void) { }

    inline uint64_t hash(void) const { return _hash; }
    inline Chain *next(void) const { return _next; }
    inline size_t size(size_t count=0) const { return _next == nullptr ? count + 1 : _next->size(count + 1); }

    inline void set_next(Chain *next) { _next = next; }

    inline void
    for_each_label_current(CPackedScoreType<SCORE_TYPE, action::MAX> &out) const {
      using aligned_score_type = SCORE_TYPE __attribute__((aligned(32)));
      const aligned_score_type *const __restrict__ src = _current;
      aligned_score_type *const __restrict__ dst = &out[0];
      for (Label l = 0; l != PADDED_NLABELS; ++l)
        dst[l] += src[l];
    }

    inline void
    for_each_label_total(CPackedScoreType<SCORE_TYPE, action::MAX> &out) const {
      using aligned_score_type = SCORE_TYPE __attribute__((aligned(32)));
      const aligned_score_type *const __restrict__ src = _total;
      aligned_score_type *const __restrict__ dst = &out[0];
      for (Label l = 0; l != PADDED_NLABELS; ++l)
        dst[l] += src[l];
    }

    void
    update_average(const unsigned int iteration) {
      for (Label l = 0; l != NLABELS; ++l)
        if (iteration > _last_updated[l])
          _total[l] += _current[l]*(iteration - _last_updated[l]);
    }

    void
    update_current(const Label label, const SCORE_TYPE added, const unsigned int iteration) {
      if (iteration > _last_updated[label]) {
        _total[label] += _current[label]*(iteration - _last_updated[label]);
        _last_updated[label] = iteration;
      }
      _current[label] += added;
      _total[label] += added;
    }

    void
    deserialise(std::istream &in) {
      uint32_t nitems = mp::read_array_size(in);
      assert(nitems == 4); (void)nitems;
      _hash = mp::read_uint(in);

      nitems = mp::read_map_size(in);
      for (uint32_t i = 0; i != nitems; ++i) {
        const Label l = mp::read_uint(in);
        _current[l] = mp::read_double(in);
      }

      nitems = mp::read_map_size(in);
      for (uint32_t i = 0; i != nitems; ++i) {
        const Label l = mp::read_uint(in);
        _total[l] = mp::read_double(in);
      }
    }

    void
    serialise(std::ostream &out) const {
      uint32_t non_zero;
      mp::write_array_size(out, 4);
      mp::write_uint(out, _hash);

      non_zero = 0;
      for (Label l = 0; l != NLABELS; ++l)
        if (_current[l] != 0)
          ++non_zero;
      mp::write_map_size(out, non_zero);
      for (Label l = 0; l != NLABELS; ++l) {
        if (_current[l] != 0) {
          mp::write_uint(out, l);
          mp::write_double(out, _current[l]);
        }
      }

      non_zero = 0;
      for (Label l = 0; l != NLABELS; ++l)
        if (_total[l] != 0)
          ++non_zero;
      mp::write_map_size(out, non_zero);
      for (Label l = 0; l != NLABELS; ++l) {
        if (_total[l] != 0) {
          mp::write_uint(out, l);
          mp::write_double(out, _total[l]);
        }
      }
    }

  private:
    SCHWA_DISALLOW_COPY_AND_ASSIGN(Chain);
  };


private:
  template <typename CP, typename HASHER>
  inline uint64_t
  _hash(const FeatureType &type, const CP &cp, const HASHER &hasher) const {
    uint64_t h = TYPE_ID_HASH_MIXER * static_cast<uint64_t>(type.id());
    return (h << 16) + (h << 6) + hash(cp);
  }

  template <typename CP, typename HASHER>
  inline void
  _update_current(const FeatureType &type, const CP &cp, const HASHER &hasher, const Label label, const SCORE_TYPE amount, const unsigned int iteration) {
    const uint64_t hash = _hash(type, cp, hasher);
    const size_t index = hash & TABLE_INDEX_MASK;

    Chain *prev = nullptr, *chain;
    for (chain = _table[index]; chain != nullptr; chain = chain->next()) {
      if (chain->hash() == hash) {
        chain->update_current(label, amount, iteration);
        return;
      }
      prev = chain;
    }

    // Create the new chain item and insert it appropriately.
    ++_size;
    chain = _chain_pool.alloc<Chain *>(sizeof(Chain));
    new (chain) Chain(hash, _current_pool, _total_pool, _updated_pool);
    if (prev == nullptr)
      _table[index] = chain;
    else
      prev->set_next(chain);

    chain->update_current(label, amount, iteration);
  }

  template <typename CP, typename HASHER>
  inline void
  _for_each_label_current(const FeatureType &type, const CP &cp, const HASHER &hasher, CPackedScoreType<SCORE_TYPE, action::MAX> &out) const {
    const uint64_t hash = _hash(type, cp, hasher);
    const size_t index = hash & TABLE_INDEX_MASK;

    for (const Chain *chain = _table[index]; chain != nullptr; chain = chain->next()) {
      if (chain->hash() == hash) {
        chain->for_each_label_current(out);
        return;
      }
    }
  }

  template <typename CP, typename HASHER>
  inline void
  _for_each_label_total(const FeatureType &type, const CP &cp, const HASHER &hasher, CPackedScoreType<SCORE_TYPE, action::MAX> &out) const {
    const uint64_t hash = _hash(type, cp, hasher);
    const size_t index = hash & TABLE_INDEX_MASK;

    for (const Chain *chain = _table[index]; chain != nullptr; chain = chain->next()) {
      if (chain->hash() == hash) {
        chain->for_each_label_total(out);
        return;
      }
    }
  }

private:
  schwa::Pool _chain_pool;
  schwa::Pool _current_pool;
  schwa::Pool _total_pool;
  schwa::Pool _updated_pool;
  Chain **const _table;
  size_t _size;

public:
  explicit FeatureHashtableDO(size_t chain_pool_block_size=DEFAULT_CHAIN_POOL_BLOCK_SIZE, size_t data_pool_block_size=DEFAULT_DATA_POOL_BLOCK_SIZE) :
      _chain_pool(chain_pool_block_size),
      _current_pool(data_pool_block_size),
      _total_pool(data_pool_block_size),
      _updated_pool(data_pool_block_size),
      _table(new Chain *[TABLE_SIZE]),
      _size(0)
    {
    std::memset(_table, 0, TABLE_SIZE*sizeof(Chain *));
  }
  ~FeatureHashtableDO(void) {
    delete [] _table;
  }

  // Capacity
  bool empty(void) const { return _size == 0; }
  size_t size(void) const { return _size; }

  // Hash policy
  float load_factor(void) const noexcept { return _size / static_cast<float>(TABLE_SIZE); }
  uint16_t nlabels(void) const { return NLABELS; }

  void
  update_average(const unsigned int iteration) {
    for (size_t index = 0; index != TABLE_SIZE; ++index)
      for (Chain *chain = _table[index]; chain != nullptr; chain = chain->next())
        chain->update_average(iteration);
  }

  template <typename CP, typename HASHER=Hasher<CP>>
  void
  update_current(const FeatureType &type, const CP &contextual_predicate, const Label label, const SCORE_TYPE amount, const unsigned int iteration, const HASHER &hasher=HASHER()) {
    static_assert(sizeof(typename HASHER::result_type) == 8, "64-bit hash function required");
    _update_current(type, contextual_predicate, hasher, label, amount, iteration);
  }

  template <typename CP, typename HASHER=Hasher<CP>>
  void
  for_each_label_current(const FeatureType &type, const CP &contextual_predicate, CPackedScoreType<SCORE_TYPE, action::MAX> &out, const HASHER &hasher=HASHER()) const {
    static_assert(sizeof(typename HASHER::result_type) == 8, "64-bit hash function required");
    _for_each_label_current(type, contextual_predicate, hasher, out);
  }

  template <typename CP, typename HASHER=Hasher<CP>>
  void
  for_each_label_total(const FeatureType &type, const CP &contextual_predicate, CPackedScoreType<SCORE_TYPE, action::MAX> &out, const HASHER &hasher=HASHER()) const {
    static_assert(sizeof(typename HASHER::result_type) == 8, "64-bit hash function required");
    _for_each_label_total(type, contextual_predicate, hasher, out);
  }


  void
  deserialise(std::istream &in) {
    if (!empty())
      throw std::logic_error("Cannot deserialise a non-empty table");

    const uint16_t nlabels = mp::read_uint16(in);
    if (nlabels != NLABELS)
      throw std::runtime_error("nlabels on the table != nlabels on the model");

    const uint32_t nkeys = mp::read_map_size(in);
    for (size_t n = 0; n != nkeys; ++n) {
      const size_t index = mp::read_uint(in);
      const uint32_t nitems = mp::read_array_size(in);
      Chain *prev = nullptr;
      for (uint32_t i = 0; i != nitems; ++i) {
        ++_size;
        Chain *chain = _chain_pool.alloc<Chain *>(sizeof(Chain));
        new (chain) Chain(0, _current_pool, _total_pool, _updated_pool);
        chain->deserialise(in);
        if (prev == nullptr)
          _table[index] = chain;
        else
          prev->set_next(chain);
        prev = chain;
      }
    }
  }

  void
  serialise(std::ostream &out) const {
    mp::write_uint16(out, NLABELS);

    uint32_t nkeys = 0;
    for (size_t index = 0; index != TABLE_SIZE; ++index)
      if (_table[index] != nullptr)
        ++nkeys;
    mp::write_map_size(out, nkeys);

    for (size_t index = 0; index != TABLE_SIZE; ++index) {
      const Chain *chain = _table[index];
      if (chain == nullptr)
        continue;
      mp::write_uint(out, index);
      const uint32_t nitems = chain->size();
      mp::write_array_size(out, nitems);
      for (uint32_t i = 0; i != nitems; ++i) {
        chain->serialise(out);
        chain = chain->next();
      }
    }
  }

  std::ostream &
  stats(std::ostream &out) const {
    std::map<uint32_t, uint32_t> chain_lengths;
    for (size_t index = 0; index != TABLE_SIZE; ++index) {
      const Chain *chain = _table[index];
      if (chain != nullptr)
        chain_lengths[chain->size()] += 1;
    }
    for (const auto &pair : chain_lengths)
      out << pair.first << " => " << pair.second << std::endl;
    return out;
  }

private:
  SCHWA_DISALLOW_COPY_AND_ASSIGN(FeatureHashtableDO);
};


class CWeight : public CWeightBase {
public:
  static const FeatureType STw;
  static const FeatureType STt;
  static const FeatureType STwt;
  static const FeatureType N0w;
  static const FeatureType N0t;
  static const FeatureType N0wt;
  static const FeatureType N1w;
  static const FeatureType N1t;
  static const FeatureType N1wt;
  static const FeatureType N2w;
  static const FeatureType N2t;
  static const FeatureType N2wt;
  static const FeatureType STHw;
  static const FeatureType STHt;
  static const FeatureType STi;
  static const FeatureType STHHw;
  static const FeatureType STHHt;
  static const FeatureType STHi;
  static const FeatureType STLDw;
  static const FeatureType STLDt;
  static const FeatureType STLDi;
  static const FeatureType STRDw;
  static const FeatureType STRDt;
  static const FeatureType STRDi;
  static const FeatureType N0LDw;
  static const FeatureType N0LDt;
  static const FeatureType N0LDi;
  static const FeatureType STL2Dw;
  static const FeatureType STL2Dt;
  static const FeatureType STL2Di;
  static const FeatureType STR2Dw;
  static const FeatureType STR2Dt;
  static const FeatureType STR2Di;
  static const FeatureType N0L2Dw;
  static const FeatureType N0L2Dt;
  static const FeatureType N0L2Di;
  static const FeatureType HTw;
  static const FeatureType HTt;
  static const FeatureType HTwt;
  static const FeatureType STwtN0wt;
  static const FeatureType STwtN0w;
  static const FeatureType STwN0wt;
  static const FeatureType STtN0wt;
  static const FeatureType STwtN0t;
  static const FeatureType STwN0w;
  static const FeatureType STtN0t;
  static const FeatureType N0tN1t;
  static const FeatureType N0tN1tN2t;
  static const FeatureType STtN0tN1t;
  static const FeatureType STtN0tN0LDt;
  static const FeatureType N0tN0LDtN0L2Dt;
  static const FeatureType STHtSTtN0t;
  static const FeatureType HTtHT2tN0t;
  static const FeatureType STHHtSTHtSTt;
  static const FeatureType STtSTLDtN0t;
  static const FeatureType STtSTLDtSTL2Dt;
  static const FeatureType STtSTRDtN0t;
  static const FeatureType STtSTRDtSTR2Dt;
  static const FeatureType STwd;
  static const FeatureType STtd;
  static const FeatureType N0wd;
  static const FeatureType N0td;
  static const FeatureType STwN0wd;
  static const FeatureType STtN0td;
  static const FeatureType STwra;
  static const FeatureType STtra;
  static const FeatureType STwla;
  static const FeatureType STtla;
  static const FeatureType N0wla;
  static const FeatureType N0tla;
  static const FeatureType STwrp;
  static const FeatureType STtrp;
  static const FeatureType STwlp;
  static const FeatureType STtlp;
  static const FeatureType N0wlp;
  static const FeatureType N0tlp;
  static const FeatureType STl;
  static const FeatureType STc;
  static const FeatureType STf;
  static const FeatureType N0l;
  static const FeatureType N0c;
  static const FeatureType N0f;
  static const FeatureType N1l;
  static const FeatureType N1c;
  static const FeatureType N1f;

protected:
  FeatureHashtableDO<> _weights;

public:
  CWeight(const std::string &sInputPath, bool bTrain) : CWeight(sInputPath, sInputPath, bTrain) { }
  CWeight(const std::string &sInputPath, const std::string &sOutputPath, bool bTrain);
  virtual ~CWeight(void);

  void loadScores(void) override;
  void saveScores(void) override;

  void addWeighted(double mu, const CWeight &other);
  void computeAverageFeatureWeights(unsigned int iteration);
  void debugUsage(void) const;

  template <typename CP>
  void getScore(const FeatureType &type, const CP &cp, CPackedScoreType<SCORE_TYPE, action::MAX> &out, ScoreAverage sa);

  template <typename CP>
  void updateScore(const FeatureType &type, const CP &cp, unsigned action, SCORE_TYPE amount, unsigned int iteration);
};


template <typename CP>
inline void
CWeight::getScore(const FeatureType &type, const CP &cp, CPackedScoreType<SCORE_TYPE, action::MAX> &out, const ScoreAverage sa) {
  if (sa == SCORE_NON_AVERAGE)
    _weights.for_each_label_current(type, cp, out);
  else
    _weights.for_each_label_total(type, cp, out);
}


template <typename CP>
inline void
CWeight::updateScore(const FeatureType &type, const CP &cp, const unsigned action, const SCORE_TYPE amount, const unsigned int iteration) {
  const Label label = action;
  _weights.update_current(type, cp, label, amount, iteration);
}

}  // namespace depparser
}  // namespace TARGET_LANGUAGE

#endif
