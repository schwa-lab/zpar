/****************************************************************
 *                                                              *
 * score_packed_array.h - the array implementation of packed score. *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _SCORE_PACKED_ARRAY_H
#define _SCORE_PACKED_ARRAY_H


template <typename SCORE_TYPE, unsigned _PACKED_SIZE>
class CPackedScoreArray {
public:
  using PoolItemType = void;
  using ScoreType = SCORE_TYPE;
  static constexpr const size_t PACKED_SIZE = _PACKED_SIZE;
  static constexpr const size_t POOL_BLOCK_SIZE = 0;

private:
  CScore<ScoreType> scores[PACKED_SIZE];

public:
  CPackedScoreArray() { }

  void
  add(CPackedScoreType<ScoreType, PACKED_SIZE> &o, const int &which) const {
    for (size_t index = 0; index != PACKED_SIZE; ++index)
      o[index] += scores[index].score(which);
  }

  inline void clear() { }

  void
  combineAdd(const CPackedScoreArray &other) {
    for (size_t i = 0; i != PACKED_SIZE; ++i)
      scores[i].combineAdd(other.scores[i]);
  }

  void
  combineDiv(const unsigned int n) {
    for (size_t i = 0; i != PACKED_SIZE; ++i)
      scores[i].combineDiv(n);
  }

  bool
  empty() const {
    for (size_t index = 0; index != PACKED_SIZE; ++index)
      if (!scores[index].zero())
        return false;
    return true;
  }

  inline const ScoreType
  score(const unsigned int index, const int &n) const {
    return scores[index].score(n);
  }

  void
  updateAverage(const int &round) {
    for (unsigned index = 0; index != PACKED_SIZE; ++index)
      scores[index].updateAverage(round);
  }

  inline void
  updateCurrent(const unsigned int index, const ScoreType &added, const int &round) {
    scores[index].updateCurrent(added, round);
  }

  CScore<ScoreType> &operator [](const unsigned int index) { return scores[index]; }
  const CScore<ScoreType> &operator [](const unsigned int index) const { return scores[index]; }

  std::istream &
  deserialise(std::istream &is) {
    assert(PACKED_SIZE > 0);
    const uint32_t nitems = mp::read_array_size(is);
    for (unsigned i = 0; i != nitems; ++i)
      (*this)[i].deserialise(is);
    return is;
  }

  std::ostream &
  serialise(std::ostream &os) const {
    assert(PACKED_SIZE > 0);
    mp::write_array_size(os, PACKED_SIZE);
    for (unsigned i = 0; i != PACKED_SIZE; ++i)
      (*this)[i].serialise(os);
    return os;
  }
};


#endif
