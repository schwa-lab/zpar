/****************************************************************
 *                                                              *
 * score_packed_array.h - the array implementation of packed score. *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _SCORE_PACKED_ARRAY_H
#define _SCORE_PACKED_ARRAY_H

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
   CScore<SCORE_TYPE> scores[PACKED_SIZE];

public:
   const SCORE_TYPE score(const unsigned &index, const int &n) const {
      return scores[index].score(n);
   }
   void updateCurrent(const unsigned &index, const SCORE_TYPE &added, const int &round) {
      scores[index].updateCurrent(added, round);
   }
   void updateAverage(const int &round) {
      for (unsigned index=0; index<PACKED_SIZE; ++index)
         scores[index].updateAverage(round);
   }
   void reset() {
      for (unsigned index=0; index<PACKED_SIZE; ++index)
         scores[index].reset();
   }
   bool empty() const {
      for (unsigned index=0; index<PACKED_SIZE; ++index)
         if (!scores[index].zero()) return false;
      return true;
   }
   inline void clear() { }
   void add(CPackedScoreType<SCORE_TYPE, PACKED_SIZE> &o, const int &which) const {
      for (unsigned index=0; index<PACKED_SIZE; ++index)
         o[index] += scores[index].score(which);
   }
public:
   CScore<SCORE_TYPE> & operator [](const unsigned &index) { return scores[index]; }
   const CScore<SCORE_TYPE> & operator [](const unsigned &index) const { return scores[index]; }
};


template <typename SCORE_TYPE, unsigned PACKED_SIZE>
inline std::istream &
operator >>(std::istream &is, CPackedScore<SCORE_TYPE, PACKED_SIZE> &score) {
  assert(PACKED_SIZE > 0);
  const uint32_t nitems = mp::read_array_size(is);
  for (unsigned i = 0; i != nitems; ++i)
    is >> score[i];
  return is;
}


template <typename SCORE_TYPE, unsigned PACKED_SIZE>
inline std::ostream &
operator <<(std::ostream &os, const CPackedScore<SCORE_TYPE, PACKED_SIZE> &score) {
  assert(PACKED_SIZE > 0);
  mp::write_array_size(os, PACKED_SIZE);
  for (unsigned i = 0; i != PACKED_SIZE; ++i)
    os << score[i];
  return os;
}


#endif
