/****************************************************************
 *                                          *
 * score_packed_hash.h - the definitions for packed score v.   *
 *                                          *
 * Author: Yue Zhang                              *
 *                                          *
 ****************************************************************/

#ifndef _SCORE_PACKED_LIST_H
#define _SCORE_PACKED_LIST_H

#include "hash.h"
#include "linkedlist.h"
#include "score.h"


template <typename SCORE_TYPE, unsigned _PACKED_SIZE>
class CPackedScoreList {
public:
  using ListType = CLinkedList<unsigned int, CScore<SCORE_TYPE>>;
  using PoolItemType = typename ListType::PoolItemType;
  using ScoreType = SCORE_TYPE;
  static constexpr const size_t PACKED_SIZE = _PACKED_SIZE;
  static constexpr const size_t POOL_BLOCK_SIZE = ListType::POOL_BLOCK_SIZE;

private:
  ListType scores;

public:
  CPackedScoreList() { }

  void
  add(CPackedScoreType<ScoreType, PACKED_SIZE> &o, const ScoreAverage which) const {
    const auto end = scores.end();
    for (auto it = scores.begin(); it != end; ++it)
      o[it.first()] += it.second().score(which);
  }

  void
  addCurrent(CPackedScoreList &s, const int round, CMemoryPool<PoolItemType> &pool, PoolItemType **free) {
    const auto end = scores.end();
    for (auto it = scores.begin(); it != end; ++it)
      scores.get(it.first(), pool, free).updateCurrent(it.second().score(), round);
  }

  inline void
  clear(PoolItemType **free) {
    scores.clear(free);
  }

  void
  combineAdd(const CPackedScoreList &other, CMemoryPool<PoolItemType> &pool, PoolItemType **free) {
    const auto end = other.scores.end();
    for (auto it = other.scores.begin(); it != end; ++it)
      scores.get(it.first(), pool, free).combineAdd(it.second());
  }

  void
  combineDiv(const unsigned int n) {
    const auto end = scores.end();
    for (auto it = scores.begin(); it != end; ++it)
      it.second().combineDiv(n);
  }

  ScoreType
  dotProduct(const CPackedScoreList &s) {
    const CScore<ScoreType> sc_zero;
    ScoreType retval = 0;
    if (s.empty())
      return retval;
    const auto end = scores.end();
    for (auto it = scores.begin(); it != end; ++it)
      retval += it.second().score() * s.scores.find(it.first(), sc_zero).score();
    return retval;
  }

  inline bool
  element(const unsigned int index) const {
    return scores.element(index);
  }

  bool
  empty() const {
    const auto end = scores.end();
    for (auto it = scores.begin(); it != end; ++it)
      if (!it.second().zero())
        return false;
    return true;
  }

  void
  scaleCurrent(ScoreType scale, const int round) {
    const auto end = scores.end();
    for (auto it = scores.begin(); it != end; ++it)
      it.second().scaleCurrent(scale, round);
  }

  inline const ScoreType
  score(const unsigned index, const int n) const {
    return scores.find(index, CScore<ScoreType>()).score(n);
  }

  ScoreType
  squareNorm() {
    const auto end = scores.end();
    ScoreType retval = 0;
    for (auto it = scores.begin(); it != end; ++it)
      retval += it.second().score() * it.second().score();
    return retval;
  }

  void
  subtractCurrent(CPackedScoreList &s, const int round, CMemoryPool<PoolItemType> &pool, PoolItemType **free) {
    const auto end = scores.end();
    for (auto it = scores.begin(); it != end; ++it)
      scores.get(it.first(), pool, free).updateCurrent(-it.second().score(), round);
  }

  void
  updateAverage(const int round) {
    const auto end = scores.end();
    for (auto it = scores.begin(); it != end; ++it)
      it.second().updateAverage(round);
  }

  inline void
  updateCurrent(const unsigned int index, const ScoreType added, const int round, CMemoryPool<PoolItemType> &pool, PoolItemType **free) {
    scores.get(index, pool, free).updateCurrent(added, round);
  }

  std::istream &
  deserialise(std::istream &is, CMemoryPool<PoolItemType> &pool, PoolItemType **free) {
    const uint32_t nitems = mp::read_map_size(is);
    for (uint32_t n = 0; n != nitems; ++n) {
      uint32_t key = mp::read_uint(is);
      scores.get(key, pool, free).deserialise(is);
    }
    return is;
  }

  std::ostream &
  serialise(std::ostream &os) const {
    uint32_t nitems = 0;
    const auto end = scores.end();
    for (auto it = scores.begin(); it != end; ++it) {
#ifndef NO_NEG_FEATURE
      if (it.second().zero()) continue;
#endif
        ++nitems;
    }

    mp::write_map_size(os, nitems);
    for (auto it = scores.begin(); it != end; ++it) {
#ifndef NO_NEG_FEATURE
      if (it.second().zero()) continue;
#endif
        mp::write_uint(os, it.first());
        it.second().serialise(os);
    }

    return os;
  }
};

#endif
