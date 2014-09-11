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
#include "hash_small.h"
#include "score.h"

/*===============================================================
 *
 * CPackedScore - packed score definition
 *
 *==============================================================*/

#include "score_packed_list.h"
//#include "score_packed_hash.h"
//#include "score_packed_array.h"

/*===============================================================
 *
 * CPackedScoreType - packed score type definition
 *
 *==============================================================*/

template <typename SCORE_TYPE, unsigned PACKED_SIZE>
class CPackedScoreType {
protected:
   SCORE_TYPE scores[PACKED_SIZE];
public:
   void reset() {
      memset(scores, 0, sizeof(SCORE_TYPE)*PACKED_SIZE);
//      for (unsigned index=0; index<PACKED_SIZE; ++index)
//         scores[index]=0;
   }
   bool empty() const {
      for (unsigned index=0; index<PACKED_SIZE; ++index)
         if (scores[index]!=0) return false;
      return true;
   }
public:
   SCORE_TYPE &operator [](const unsigned &index) {
      assert(index<PACKED_SIZE);
      return scores[index];
   }
   const SCORE_TYPE &operator [](const unsigned &index) const {
      assert(index<PACKED_SIZE);
      return scores[index];
   }
   void operator +=(const CPackedScoreType &i) {
      for (unsigned index=0; index<PACKED_SIZE; ++index)
         scores[index]+=i.scores[index];
   }
};

template <typename SCORE_TYPE, unsigned PACKED_SIZE>
std::istream & operator >> (std::istream &is, CPackedScoreType<SCORE_TYPE, PACKED_SIZE> &score) {
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
std::ostream & operator << (std::ostream &os, CPackedScoreType<SCORE_TYPE, PACKED_SIZE> &score) {
   assert(PACKED_SIZE>0);
   os << " [ ";
   os << score[0];
   for (unsigned index=1; index<PACKED_SIZE; ++index)
      os << " , " << score[index];
   os << " ] ";
   return os;
}

/*===============================================================
 *
 * CPackedScoreMap - map to packed score definition
 *
 *==============================================================*/

template <typename K, typename SCORE_TYPE, unsigned PACKED_SIZE>
class CPackedScoreMap : public CHashMap< K , CPackedScore<SCORE_TYPE, PACKED_SIZE> > {
protected:
   const CPackedScore<SCORE_TYPE, PACKED_SIZE> m_zero ;

#ifdef NO_NEG_FEATURE
protected:
   const CPackedScoreMap *m_positive;
#endif

public:
   const std::string name ;
   unsigned count ;

   CPackedScoreMap(std::string input_name, int TABLE_SIZE) : CHashMap<K,CPackedScore<SCORE_TYPE, PACKED_SIZE> >(TABLE_SIZE) , name(input_name) , count(0)
#ifdef NO_NEG_FEATURE
, m_positive(this)
#endif
   {
      assert(m_zero.empty());
   }

#ifdef NO_NEG_FEATURE
   inline void setPositiveFeature(const CPackedScoreMap &positive) {
      m_positive = &positive;
   }

   inline void addPositiveFeature(const K &key, const unsigned &index) {
      (*this)[key][index];
   }
#endif // define features

   inline void getScore( CPackedScoreType<SCORE_TYPE, PACKED_SIZE>&o, const K &key , const int &which ) {
      this->find( key , m_zero ).add( o , which );
   }

   inline void updateScore( const K &key , const unsigned &index , const SCORE_TYPE &amount , const int &round ) {
#ifdef NO_NEG_FEATURE
      if (m_positive->element(key) && (*m_positive)[key].element(index))
#endif // update can only happen with defined features
      (*this)[ key ].updateCurrent( index , amount , round );
   }

   inline void getOrUpdateScore( CPackedScoreType<SCORE_TYPE, PACKED_SIZE> &out , const K &key , const unsigned &index , const int &which , const SCORE_TYPE &amount=0 , const int &round=0 ) {
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
         updateScore( key , index , amount , round ) ;
      }
   }

   void computeAverage(unsigned long int round) {
      typedef typename CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >::iterator iterator;
      const iterator end = this->end();
      for (iterator it = this->begin(); it != end; ++it) {
         it.second().updateAverage(round) ;
      }
   }

//   void clearScores() {
//      typename CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >::iterator it = this->begin();
//      while (it != this->end()) {
//         it.second().reset() ;
//         ++ it;
//      }
//   }

   void clear() {
      // first clear each ste of packed scores so that the memory will be freed
      typedef typename CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >::iterator iterator;
      const iterator end = this->end();
      for (iterator it = this->begin(); it != end; ++it) {
         it.second().clear() ;
      }
      // second clear hash
      CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >::clear();
   }

   void addCurrent(CPackedScoreMap &mp, const int &round) {
      typedef typename CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >::iterator iterator;
      const iterator end = this->end();
      for (iterator it = this->begin(); it != end; ++it) {
         (*this)[it.first()].addCurrent(it.second(), round);
      }
   }
   void subtractCurrent(CPackedScoreMap &mp, const int &round) {
      typedef typename CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >::iterator iterator;
      const iterator end = this->end();
      for (iterator it = this->begin(); it != end; ++it) {
         (*this)[it.first()].subtractCurrent(it.second(), round);
      }
   }
   void scaleCurrent(const SCORE_TYPE &scale, const int &round) {
      typedef typename CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >::iterator iterator;
      const iterator end = this->end();
      for (iterator it = this->begin(); it != end; ++it) {
         it.second().scaleCurrent(scale, round);
      }
   }
   SCORE_TYPE squareNorm() {
      typedef typename CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >::iterator iterator;
      const iterator end = this->end();
      SCORE_TYPE retval = 0;
      for (iterator it = this->begin(); it != end; ++it) {
         retval += it.second().squareNorm();
      }
      return retval;
   }

   SCORE_TYPE dotProduct(CPackedScoreMap &mp) {
      SCORE_TYPE retval = 0;
      typedef typename CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >::iterator iterator;
      const iterator end = this->end();
      for (iterator it = this->begin(); it != end; ++it) {
         retval += it.second().dotProduct(mp.find(it.first(), m_zero));
      }
      return retval;
   }

   void addWeighted(const double mu, const CPackedScoreMap &other) {
      typedef typename CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >::const_iterator iterator;
      const iterator end = other.end();
      for (iterator it = other.begin(); it != end; ++it) {
         (*this)[it.first()].addWeighted(mu, it.second());
      }
   }

#ifdef DEBUG
   void trace() {
      std::cout << name << ": ";
      CHashMap< K , CPackedScore<SCORE_TYPE, PACKED_SIZE> >::trace();
   }
#endif
};

//===============================================================

template<typename K, typename SCORE_TYPE, unsigned PACKED_SIZE>
inline
std::istream & operator >> (std::istream &is, CPackedScoreMap<K, SCORE_TYPE, PACKED_SIZE> &score_map) {
   if (!is) return is ;
   std::string s ;
   getline(is, s) ;
   // match name
   const unsigned &size = score_map.name.size();
   if ( s.substr(0, size)!=score_map.name ) THROW("hashmap_score_packed.h: the expected score map " << score_map.name << " is not matched.");
   is >> static_cast< CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> > &>(score_map) ;
   return is ;
}

template<typename K, typename SCORE_TYPE, unsigned PACKED_SIZE>
inline
std::ostream & operator << (std::ostream &os, CPackedScoreMap<K, SCORE_TYPE, PACKED_SIZE> &score_map) {
   assert(os);
   if (score_map.count)
      os << score_map.name << ' ' << score_map.count << std::endl ;
   else
      os << score_map.name << std::endl ;

   typedef typename CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >::iterator iterator;
   const iterator end = score_map.end();
   for (iterator it = score_map.begin(); it != end; ++it) {
#ifndef NO_NEG_FEATURE
      if ( !it.second().empty() ) 
#endif // do not write zero scores if allow negative scores
         os << it.first() << "\t:\t" << it.second() << std::endl ;
   }
   os << std::endl ;
   return os ;
}

#endif
