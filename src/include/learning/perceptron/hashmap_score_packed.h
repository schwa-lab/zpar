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

//#include "score_packed_list.h"
//#include "score_packed_hash.h"
#include "score_packed_array.h"
#include "hash_utils.h" // Feature enum is there for now
#include "hash_simple.h"
#include "english/tags.h"
#include "english/pos/penn.h"
#include <utility>

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
class CPackedScoreMap : public CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> > {

protected:
   const CPackedScore<SCORE_TYPE, PACKED_SIZE> m_zero ;

#ifdef NO_NEG_FEATURE
protected:
   const CPackedScoreMap *m_positive;
#endif

public:
   const std::string name ;
   bool initialized ;
   unsigned count ;

public:
   CPackedScoreMap(std::string input_name, int TABLE_SIZE, bool bInitMap=true) : CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >(TABLE_SIZE, bInitMap) , m_zero(), name(input_name) , initialized(bInitMap) , count(0)
#ifdef NO_NEG_FEATURE
, m_positive(this)
#endif
   {
      assert(m_zero.empty());
   }

public:
   virtual inline void init() {
      initialized = true;
      CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >::init();
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

   inline void getOrUpdateScore( Feature f, CPackedScoreType<SCORE_TYPE, PACKED_SIZE> &out , const K &key , const unsigned &index , const int &which , const SCORE_TYPE &amount=0 , const int &round=0 ) {
      const auto new_key = std::make_pair( f, key );
#ifdef NO_NEG_FEATURE
      if ( round == -1 ) {
         addPositiveFeature( new_key , index );
         return;
      }
#endif
      if ( amount == 0 ) {
         this->find( new_key , m_zero ).add( out, which ) ;
      }
      else {
         assert( round > 0 );
         updateScore( new_key , index , amount , round ) ;
      }
   }

   void computeAverage(unsigned long int round) {
      count = 0;
      //typename CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >::iterator 
      auto it = this->begin();
      while (it != this->end()) {
          ++ it;
         ++ count;
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
      typename CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >::iterator it = this->begin();
      while (it != this->end()) {
         it.second().clear() ;
         ++ it;
      }
      // second clear hash
      CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >::clear();
   }

public:
   void addCurrent(CPackedScoreMap &mp, const int &round) {
      typename CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >::iterator it = mp.begin();
      while (it != mp.end()) {
         (*this)[it.first()].addCurrent(it.second(), round);
         ++ it;
      }
   }
   void subtractCurrent(CPackedScoreMap &mp, const int &round) {
      typename CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >::iterator it = mp.begin();
      while (it != mp.end()) {
         (*this)[it.first()].subtractCurrent(it.second(), round);
         ++ it;
      }
   }
   void scaleCurrent(const SCORE_TYPE &scale, const int &round) {
      typename CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >::iterator it = this->begin();
      while (it != this->end()) {
         it.second().scaleCurrent(scale, round);
         ++it;
      }
   }
   SCORE_TYPE squareNorm() {
      SCORE_TYPE retval = 0;
      typename CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >::iterator it = this->begin();
      while (it != this->end()) {
         retval += it.second().squareNorm();
         ++it;
      }
      return retval;
   }

   SCORE_TYPE dotProduct(CPackedScoreMap &mp) {
      SCORE_TYPE retval = 0;
      typename CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >::iterator it = this->begin();
      while (it != this->end()) {
         retval += it.second().dotProduct(mp.find(it.first(), m_zero));
         ++ it;
      }
      return retval;
   }


#ifdef DEBUG
public:
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
   if ( !score_map.initialized ) {
      // match size
      if ( s.size()>size ) {
         unsigned table_size = 0;
         std::istringstream buffer(s.substr(size));
         buffer >> table_size;
         if (table_size) {
            unsigned hash_size = 1;
            while (hash_size<table_size) hash_size <<= 1;
            hash_size <<= 1;
            score_map.resize(hash_size);
         }
      }
      score_map.init();
   }
   is >> static_cast< CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> > &>(score_map) ;
   return is ;
}

template<typename K, typename SCORE_TYPE, unsigned PACKED_SIZE>
inline
std::ostream & operator << (std::ostream &os, CPackedScoreMap<K, SCORE_TYPE, PACKED_SIZE> &score_map) {
   assert(os);
   if (score_map.count) {
      os << score_map.name << ' ' << score_map.count << std::endl ;
      std::cout << score_map.name << ' ' << score_map.count << std::endl ;
   } else {
      os << score_map.name << std::endl ;
   }

   auto it = score_map.begin() ;
   while ( it != score_map.end() ) {
#ifndef NO_NEG_FEATURE
      if ( !it.second().empty() ) 
#endif // do not write zero scores if allow negative 
         //os << it.first().second << "\t:\t" << it.second() << std::endl ;
         os << it.first() << "\t:\t" << it.second() << std::endl ;
         //os << it.second() << std::endl ;
      ++ it;
   }
   os << std::endl ;
   return os ;
}

#endif
