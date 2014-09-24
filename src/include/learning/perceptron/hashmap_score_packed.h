/****************************************************************
 *                                                              *
 * hashmap_score_packed.h - the definitions for hash map with   *
 *                          packed score v.                     *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _HASHMAP_SCORE_PACKED_H
#define _HASHMAP_SCORE_PACKED_H

#include "hash.h"
#include "pool.h"
#include "score.h"

/*===============================================================
 *
 * CPackedScoreType - packed score type definition
 *
 *==============================================================*/
template <typename SCORE_TYPE, unsigned PACKED_SIZE>
class CPackedScoreType {
private:
  SCORE_TYPE scores[PACKED_SIZE];

public:
  bool
  empty() const {
    for (size_t index = 0; index != PACKED_SIZE; ++index)
      if (scores[index] != 0)
        return false;
    return true;
  }

  inline void
  reset() {
    std::memset(scores, 0, sizeof(SCORE_TYPE)*PACKED_SIZE);
  }

  inline SCORE_TYPE &operator [](const unsigned int index) { return scores[index]; }
  inline const SCORE_TYPE &operator [](const unsigned int index) const { return scores[index]; }

  inline CPackedScoreType &
  operator +=(const CPackedScoreType &i) {
    for (size_t index = 0; index != PACKED_SIZE; ++index)
      scores[index] += i.scores[index];
    return *this;
  }
};


template <typename SCORE_TYPE, unsigned PACKED_SIZE>
inline std::istream &
operator >>(std::istream &is, CPackedScoreType<SCORE_TYPE, PACKED_SIZE> &score) {
   assert(PACKED_SIZE>0);
   std::string s;
   is >> s;
   ASSERT(s=="[", "hashmap_score_packed.h: not well formatted CPackedScore");
   is >> score[0];
   for (unsigned index=1; index<PACKED_SIZE; ++index) {
      is >> s;
      ASSERT(s==",", "hashmap_score_packed.h: not well formatted CPackedScore");
      is >> score[index];
   }
   is >> s;
   ASSERT(s=="]", "hashmap_score_packed.h: not well formatted CPackedScore");
   return is;
}

template <typename SCORE_TYPE, unsigned PACKED_SIZE>
inline std::ostream &
operator <<(std::ostream &os, const CPackedScoreType<SCORE_TYPE, PACKED_SIZE> &score) {
   assert(PACKED_SIZE>0);
   os << " [ ";
   os << score[0];
   for (unsigned index=1; index<PACKED_SIZE; ++index)
      os << " , " << score[index];
   os << " ] ";
   return os;
}


#include "score_packed_array.h"
//#include "score_packed_hash.h"
#include "score_packed_list.h"


/*===============================================================
 *
 * CPackedScoreMap - map to packed score definition
 *
 *==============================================================*/
template <typename T, bool IS_INTEGRAL_OR_FLOAT>
struct __MessagePackIO {
  static inline std::istream &
  read(std::istream &is, T &key) {
    return key.deserialise(is);
  }

  template <typename CEntry>
  static inline std::istream &
  read(std::istream &is, T &key, CMemoryPool<CEntry> &pool, CEntry **free) {
    return key.deserialise(is, pool, free);
  }

  static inline std::ostream &
  write(std::ostream &os, const T &key) {
    return key.serialise(os);
  }
};

template <typename T>
struct __MessagePackIO<T, true> {
  static inline std::istream &
  read(std::istream &is, T &key) {
    mp::read(is, key);
    return is;
  }

  static inline std::ostream &
  write(std::ostream &os, const T &key) {
    mp::write(os, key);
    return os;
  }
};

template <typename T>
struct _MessagePackIO : public __MessagePackIO<T, std::is_integral<T>::value || std::is_floating_point<T>::value> { };


template <typename PoolItemType, typename CPackedScore>
struct __CPackedScoreMapPoolMixin {
protected:
  using ScoreType = typename CPackedScore::ScoreType;

  CMemoryPool<PoolItemType> _pool;
  PoolItemType *_free;

  __CPackedScoreMapPoolMixin(void) : _pool(1024 * CPackedScore::POOL_BLOCK_SIZE), _free(nullptr) { }

  inline void _itemAddCurrent(CPackedScore &item, CPackedScore &other, const int round) { item.addCurrent(other, round, _pool, &_free); }
  inline void _itemClear(CPackedScore &item) { item.clear(&_free); }
  inline void _itemCombineAdd(CPackedScore &item, const CPackedScore &other) { item.combineAdd(other, _pool, &_free); }
  inline void _itemUpdateCurrent(CPackedScore &item, const unsigned int index, const ScoreType added, const int round) { item.updateCurrent(index, added, round, _pool, &_free); }

  inline std::istream &_itemDeserialise(std::istream &is, CPackedScore &item) { return _MessagePackIO<CPackedScore>::read(is, item, _pool, &_free); }
};

template <typename CPackedScore>
struct __CPackedScoreMapPoolMixin<void, CPackedScore> {
protected:
  using ScoreType = typename CPackedScore::ScoreType;

  __CPackedScoreMapPoolMixin(void) { }

  inline void _itemAddCurrent(CPackedScore &item, CPackedScore &s, const int round) { item.addCurrent(s, round); }
  inline void _itemClear(CPackedScore &item) { item.clear(); }
  inline void _itemCombineAdd(CPackedScore &item, const CPackedScore &other) { item.combineAdd(other); }
  inline void _itemUpdateCurrent(CPackedScore &item, const unsigned int index, const ScoreType added, const int round) { item.updateCurrent(index, added, round); }

  inline std::istream &_itemDeserialise(std::istream &is, CPackedScore &item) { return _MessagePackIO<CPackedScore>::read(is, item); }
};

template <typename CPackedScore>
struct _CPackedScoreMapPoolMixin : public __CPackedScoreMapPoolMixin<typename CPackedScore::PoolItemType, CPackedScore> { };


template <typename K, typename CPackedScore>
class CPackedScoreMap : public CHashMap<K , CPackedScore>, protected _CPackedScoreMapPoolMixin<CPackedScore> {
public:
  using ScoreType = typename CPackedScore::ScoreType;
  static constexpr const size_t PACKED_SIZE = CPackedScore::PACKED_SIZE;

protected:
  const CPackedScore m_zero;
#ifdef NO_NEG_FEATURE
  const CPackedScoreMap *m_positive;
#endif

public:
  const std::string name ;
  unsigned count ;

  CPackedScoreMap(std::string input_name, int TABLE_SIZE) :
      CHashMap<K, CPackedScore>(TABLE_SIZE),
      m_zero(),
      name(input_name),
      count(0)
#ifdef NO_NEG_FEATURE
     , m_positive(this)
#endif
      {
    assert(m_zero.empty());
  }
  virtual ~CPackedScoreMap(void) { }


  void
  addCurrent(CPackedScoreMap &mp, const int round) {
    const auto end = this->end();
    for (auto it = this->begin(); it != end; ++it)
      this->_itemAddCurrent((*this)[it.first()], it.second(), round);
  }

#ifdef NO_NEG_FEATURE
  inline void
  addPositiveFeature(const K &key, const unsigned index) {
    (*this)[key][index];
  }
#endif // define features

  void
  clear() override {
    const auto end = this->end();
    for (auto it = this->begin(); it != end; ++it)
      this->_itemClear(it.second());
    CHashMap<K, CPackedScore>::clear();
  }

  void
  combineAdd(const CPackedScoreMap &other) {
    const auto end = other.end();
    for (auto it = other.begin(); it != end; ++it)
      this->_itemCombineAdd((*this)[it.first()], it.second());
  }

  void
  combineDiv(const unsigned int n) {
    const auto end = this->end();
    for (auto it = this->begin(); it != end; ++it)
      it.second().combineDiv(n);
  }

  void
  computeAverage(unsigned long int round) {
    const auto end = this->end();
    for (auto it = this->begin(); it != end; ++it)
      it.second().updateAverage(round) ;
  }

  ScoreType
  dotProduct(CPackedScoreMap &mp) {
    ScoreType retval = 0;
    const auto end = this->end();
    for (auto it = this->begin(); it != end; ++it)
      retval += it.second().dotProduct(mp.find(it.first(), m_zero));
    return retval;
  }

  template <typename T>
  inline void
  getOrUpdateScore(CPackedScoreType<ScoreType, PACKED_SIZE> &out, const T &key_ , const unsigned index, const ScoreAverage which, const ScoreType amount=0, const int round=0) {
    const unsigned long key = hash(key_);
#ifdef NO_NEG_FEATURE
    if ( round == -1 ) {
      addPositiveFeature( key, index );
      return;
    }
#endif
    if ( amount == 0 ) {
      this->find(key, m_zero).add(out, which) ;
    }
    else {
      assert( round > 0 );
      updateScore(key, index, amount, round) ;
    }
  }

  inline void
  getScore(CPackedScoreType<ScoreType, PACKED_SIZE> &o, const K &key, const ScoreAverage which) {
    this->find(key, m_zero).add(o , which);
  }

  void
  scaleCurrent(const ScoreType scale, const int round) {
    const auto end = this->end();
    for (auto it = this->begin(); it != end; ++it)
      it.second().scaleCurrent(scale, round);
  }

#ifdef NO_NEG_FEATURE
  inline void
  setPositiveFeature(const CPackedScoreMap &positive) {
    m_positive = &positive;
  }
#endif // define features

  ScoreType
  squareNorm() {
    const auto end = this->end();
    ScoreType retval = 0;
    for (auto it = this->begin(); it != end; ++it)
      retval += it.second().squareNorm();
    return retval;
  }

  void
  subtractCurrent(CPackedScoreMap &mp, const int round) {
    const auto end = this->end();
    for (auto it = this->begin(); it != end; ++it)
      this->itemSubtractCurrent((*this)[it.first()], it.second(), round);
  }

  inline void
  updateScore(const K &key, const unsigned index, const ScoreType amount, const int round) {
#ifdef NO_NEG_FEATURE
    if (m_positive->element(key) && (*m_positive)[key].element(index))
#endif // update can only happen with defined features
      this->_itemUpdateCurrent((*this)[ key ], index, amount, round);
  }


  std::istream &
  deserialise(std::istream &is) {
    if (!is)
      return is ;

    // Match name.
    const std::string name = mp::read_str(is);
    if (name != this->name)
      THROW("hashmap_score_packed.h: the expected score map " << this->name << " is not matched.");

    // Read in the size of the map.
    K key;
    const uint32_t npairs = mp::read_map_size(is);
    for (uint32_t i = 0; i != npairs; ++i) {
      _MessagePackIO<K>::read(is, key);
      this->_itemDeserialise(is, (*this)[key]);
    }

    return is;
  }

  std::ostream &
  serialise(std::ostream &os) const {
    assert(os);

    // Write out the name of the table.
    mp::write_str(os, this->name);

    // Count how many items are in the hashtable, and write that out.
    uint32_t npairs = 0;
    const auto end = this->end();
    for (auto it = this->begin(); it != end; ++it) {
#ifndef NO_NEG_FEATURE
      // Do not write zero scores if allow negative scores.
      if (it.second().empty()) continue;
#endif
        ++npairs;
    }
    mp::write_map_size(os, npairs);

    // Output each key/value pair.
    for (auto it = this->begin(); it != end; ++it) {
#ifndef NO_NEG_FEATURE
      // Do not write zero scores if allow negative scores.
      if (it.second().empty()) continue;
#endif
        _MessagePackIO<K>::write(os, it.first());
        _MessagePackIO<CPackedScore>::write(os, it.second());
    }

    return os ;
  }
};

#endif
