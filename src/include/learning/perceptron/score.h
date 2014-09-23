// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * score.h - the definitions for the score for averaged ptrn.   *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _SCORE_H
#define _SCORE_H

/*===============================================================
 *
 * CScore - score definition
 *
 *==============================================================*/

template <typename SCORE_TYPE>
class CScore {
public:
  enum SCORE_AVERAGE { eNonAverage=0 , eAverage=1 } ;

protected:
  SCORE_TYPE current;
  SCORE_TYPE total;
  int lastupdate; // the round number for the last update action

public:
  CScore() : current(0), total(0), lastupdate(0) { }
  CScore(const CScore &s1) : current(s1.current), total(s1.total), lastupdate(s1.lastupdate) { }
  ~CScore() {};

  inline void reset() { current = 0; total = 0; lastupdate = 0; }
  inline bool empty() const { return current==0 && total==0 && lastupdate==0; }
  inline bool zero() const { return current==0 && total==0; }
  inline void operator ++ (int) { current++; }
  inline void operator -- (int) { current--; }

  inline SCORE_TYPE &operator [] (const int &n) { return (n == eNonAverage) ? current : total; }
  inline const SCORE_TYPE &operator [] (const int &n) const { return (n == eNonAverage) ? current : total; }

  inline const SCORE_TYPE score(const int &n=eNonAverage) const { return (n == eNonAverage) ? current : total; }

  inline void
  combineAdd(const CScore &other) {
    current += other.current;
    total += other.total;
  }

  inline void
  combineDiv(const unsigned int n) {
    current /= n;
    total /= n;
  }

  void
  scaleCurrent(const SCORE_TYPE &scale, const int &round=0) {
    assert(round >= lastupdate);
    if (round > lastupdate) {
      updateAverage(round);
      lastupdate=round;
    }
    total -= current;
    current *= scale;
    total += current;
  }

  inline void
  updateAverage(const int &round=0) {
    if (round > lastupdate)
      total += current*(round-lastupdate);
    else if (round < lastupdate)
      std::cout << "Round is: "<<round<<"<"<<lastupdate<<std::endl;
  }

  inline void
  updateCurrent(const SCORE_TYPE &added, const int &round=0) {
    assert(round >= lastupdate);
    if (round > lastupdate) {
      updateAverage(round);
      lastupdate=round;
    }
    current += added;
    total += added;
  }


  std::istream &
  deserialise(std::istream &is) {
    const uint32_t nitems = mp::read_array_size(is);
    assert(nitems == 2); (void)nitems;
    mp::read(is, current);
    mp::read(is, total);
    return is;
  }

  std::ostream &
  serialise(std::ostream &os) const {
    mp::write_array_size(os, 2);
    mp::write(os, current);
    mp::write(os, total);
    return os;
  }
};

#endif
