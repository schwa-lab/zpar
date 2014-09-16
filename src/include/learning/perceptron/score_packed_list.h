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

/*===============================================================
 *
 * CPackedScore - packed score definition
 *
 *==============================================================*/

template <typename SCORE_TYPE, unsigned PACKED_SIZE>
class CPackedScoreType;

template <typename SCORE_TYPE, unsigned PACKED_SIZE>
class CPackedScore {
protected:
  CLinkedList< unsigned, CScore<SCORE_TYPE> > scores;

public:
  CPackedScore() { }

public:
  inline const SCORE_TYPE score(const unsigned &index, const int &n) const {
    return scores.find(index, CScore<SCORE_TYPE>()).score(n);
  }

  inline void updateCurrent(const unsigned &index, const SCORE_TYPE &added, const int &round) {
    scores[index].updateCurrent(added, round);
  }

  void updateAverage(const int &round) {
    typedef typename CLinkedList< unsigned, CScore<SCORE_TYPE> >::iterator iterator;
    const iterator end = scores.end();
    for (iterator it = scores.begin(); it != end; ++it) {
      it.second().updateAverage(round);
    }
  }

//  void reset() {
//    typename CLinkedList< unsigned, CScore<SCORE_TYPE> >::iterator it;
//    it = scores.begin();
//    while (it != scores.end()) {
//      it.second().reset();
//      ++it;
//    }
//  }

  inline void clear() {
    scores.clear();
  }

  bool empty() const {
    typedef typename CLinkedList< unsigned, CScore<SCORE_TYPE> >::const_iterator iterator;
    const iterator end = scores.end();
    for (iterator it = scores.begin(); it != end; ++it) {
      if (!it.second().zero())
        return false;
    }
    return true;
  }

  void add(CPackedScoreType<SCORE_TYPE, PACKED_SIZE> &o, const int &which) const {
    typedef typename CLinkedList< unsigned, CScore<SCORE_TYPE> >::const_iterator iterator;
    const iterator end = scores.end();
    for (iterator it = scores.begin(); it != end; ++it) {
      o[it.first()]+=it.second().score(which);
    }
  }

  void trace() const {
    typedef typename CLinkedList< unsigned, CScore<SCORE_TYPE> >::const_iterator iterator;
    const iterator end = scores.end();
    for (iterator it = scores.begin(); it != end; ++it) {
      (std::cout) << it.first();
      std::cout << ' ';
      (std::cout) << (it.second());
      std::cout << std::endl;
    }
  }

  inline bool element(const unsigned &index) const {
    return scores.element(index);
  }

  void addCurrent(CPackedScore &s, const int &round) {
    typedef typename CLinkedList< unsigned, CScore<SCORE_TYPE> >::iterator iterator;
    const iterator end = scores.end();
    for (iterator it = scores.begin(); it != end; ++it) {
      scores[it.first()].updateCurrent(it.second().score(), round);
    }
  }

  void addWeighted(const float mu, const CPackedScore &other) {
    typedef typename CLinkedList< unsigned, CScore<SCORE_TYPE> >::const_iterator iterator;
    const iterator end = other.scores.end();
    for (iterator it = other.scores.begin(); it != end; ++it) {
      scores[it.first()].addWeighted(mu, it.second());
    }
  }

  void combineAdd(const CPackedScore &other) {
    typedef typename CLinkedList< unsigned, CScore<SCORE_TYPE> >::const_iterator iterator;
    const iterator end = other.scores.end();
    for (iterator it = other.scores.begin(); it != end; ++it) {
      scores[it.first()].combineAdd(it.second());
    }
  }

  void combineDiv(const unsigned int n) {
    typedef typename CLinkedList< unsigned, CScore<SCORE_TYPE> >::iterator iterator;
    const iterator end = scores.end();
    for (iterator it = scores.begin(); it != end; ++it) {
      it.second().combineDiv(n);
    }
  }

  void subtractCurrent(CPackedScore &s, const int &round) {
    typedef typename CLinkedList< unsigned, CScore<SCORE_TYPE> >::iterator iterator;
    const iterator end = scores.end();
    for (iterator it = scores.begin(); it != end; ++it) {
      scores[it.first()].updateCurrent(-it.second().score(), round);
    }
  }

  SCORE_TYPE dotProduct(const CPackedScore &s) {
    typedef typename CLinkedList< unsigned, CScore<SCORE_TYPE> >::iterator iterator;
    const CScore<SCORE_TYPE> sc_zero;
    SCORE_TYPE retval = 0;
    if (s.empty())
      return retval;
    const iterator end = scores.end();
    for (iterator it = scores.begin(); it != end; ++it) {
      retval += it.second().score() * s.scores.find(it.first(), sc_zero).score();
    }
    return retval;
  }

  SCORE_TYPE squareNorm() {
    typedef typename CLinkedList< unsigned, CScore<SCORE_TYPE> >::iterator iterator;
    const iterator end = scores.end();
    SCORE_TYPE retval = 0;
    for (iterator it = scores.begin(); it != end; ++it) {
      retval += it.second().score() * it.second().score();
    }
    return retval;
  }

  void scaleCurrent(SCORE_TYPE scale, const int &round) {
    typedef typename CLinkedList< unsigned, CScore<SCORE_TYPE> >::iterator iterator;
    const iterator end = scores.end();
    for (iterator it = scores.begin(); it != end; ++it) {
      it.second().scaleCurrent(scale, round);
    }
  }

  inline CScore<SCORE_TYPE> & operator [](const unsigned &index) { return scores[index]; }
  inline const CScore<SCORE_TYPE> & find(const unsigned &index) const { return scores.find(index,CScore<SCORE_TYPE>()); }

  template <typename T0, unsigned T1>
  friend std::istream &operator >>(std::istream &is, CPackedScore<T0, T1> &score);
  template <typename T0, unsigned T1>
  friend std::ostream &operator <<(std::ostream &os, const CPackedScore<T0, T1> &score);

  static inline void freePoolMemory() { // call after all instances clean!
    CLinkedList< unsigned, CScore<SCORE_TYPE> >::freePoolMemory();
  }
};


template <typename SCORE_TYPE, unsigned PACKED_SIZE>
inline std::istream &
operator >>(std::istream &is, CPackedScore<SCORE_TYPE, PACKED_SIZE> &score) {
  const uint32_t nitems = mp::read_map_size(is);
  for (uint32_t n = 0; n != nitems; ++n) {
    uint32_t key = mp::read_uint(is);
    is >> score[key];
  }
  return is;
}


template <typename SCORE_TYPE, unsigned PACKED_SIZE>
inline std::ostream &
operator <<(std::ostream &os, const CPackedScore<SCORE_TYPE, PACKED_SIZE> &score) {
  typedef typename CLinkedList< unsigned, CScore<SCORE_TYPE> >::const_iterator iterator;
  const iterator end = score.scores.end();

  uint32_t nitems = 0;
  for (iterator it = score.scores.begin(); it != end; ++it) {
#ifndef NO_NEG_FEATURE
    if (!it.second().zero()) {
#endif
      ++nitems;
#ifndef NO_NEG_FEATURE
    }
#endif
  }

  mp::write_map_size(os, nitems);
  for (iterator it = score.scores.begin(); it != end; ++it) {
#ifndef NO_NEG_FEATURE
    if (!it.second().zero()) {
#endif
      mp::write_uint(os, it.first());
      os << it.second();
#ifndef NO_NEG_FEATURE
    }
#endif
  }

  return os;
}

#endif
