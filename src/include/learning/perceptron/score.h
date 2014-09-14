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

template<typename SCORE_TYPE>
class CScore;

template<typename SCORE_TYPE>
std::istream & operator >> (std::istream &is, CScore<SCORE_TYPE> &score) {
   char c ;
   ASSERT(is >> score[0], "The first element of CScore cannot be read."); 
   ASSERT((is >> c) && c=='/', "The separator CScore cannot be read");
   ASSERT(is >> score[1], "The second element of CScore cannot be read");
   return is ;
}

template<typename SCORE_TYPE>
std::ostream & operator << (std::ostream &os, const CScore<SCORE_TYPE> &score) {
   return os << score[0] << " / " << score[1] ;
}

/*===============================================================
 *
 * CScore - score definition
 *
 *==============================================================*/
enum ScoreAverage {
  eNonAverage = 0,
  eAverage = 1,
};


template<typename SCORE_TYPE>
class CScore {
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

   inline SCORE_TYPE &operator [] (const ScoreAverage n) { return (n == eNonAverage) ? current : total; }
   inline const SCORE_TYPE &operator [] (const ScoreAverage n) const { return (n == eNonAverage) ? current : total; }

   inline const SCORE_TYPE score(const ScoreAverage n=eNonAverage) const { return (n == eNonAverage) ? current : total; }

   void updateAverage(const int &round=0) {
     if (round > lastupdate)
       total += current*(round-lastupdate);
     else if (round < lastupdate)
       std::cout << "Round is: "<<round<<"<"<<lastupdate<<std::endl;
   }

   void updateCurrent(const SCORE_TYPE &added, const int &round=0) {
     assert(round >= lastupdate);
     if (round > lastupdate) {
       updateAverage(round);
       lastupdate=round;
     }
     current += added;
     total += added;
   }

   void scaleCurrent(const SCORE_TYPE &scale, const int &round=0) {
     assert(round >= lastupdate);
     if (round > lastupdate) {
       updateAverage(round);
       lastupdate=round;
     }
     total -= current;
     current *= scale;
     total += current;
   }

   void addWeighted(const double mu, const CScore &other) {
      current += static_cast<SCORE_TYPE>(mu * other.current);
      total += static_cast<SCORE_TYPE>(mu * other.total);
   }
   //void updateCurrent(SCORE_TYPE added, int round=0) {if (round>=lastupdate){updateAverage(round);total+=added;lastupdate=round;}current+=added; }
};

#endif
