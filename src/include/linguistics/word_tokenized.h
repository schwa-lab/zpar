// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * tokenizedword.h - the definitions for tokenized words        *
 *                                                              *
 * A faster alternative to word.h                               *
 *                                                              *
 * Pay special attention to unseen words.                       *
 * They are all allocated as UNKNOWN in the map,                *
 * so this shouldn't be used if necessary to distinguish them.  *
 * Using constructor and the method setString has two effects.  *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.5                         *
 *                                                              *
 ****************************************************************/

#ifndef _WORD_H
#define _WORD_H

//#include "hash.h"
#include "tokenizer.h"
#include "bigram.h"

/*===============================================================
 *
 * the tokenizer
 *
 *==============================================================*/

class CStringTokenizer : public CTokenizer<std::string, 65537> {
public: 
  CStringTokenizer() : CTokenizer<std::string, 65537>(1/*reserve for the other unknown */) {lookup("");} 
};

//const unsigned long g_tokenForUnknownString = 0;
//const unsigned long g_tokenForEmptyString = 1;

/*===============================================================
 *
 * definitions about word 
 *
 *==============================================================*/

class CWord {
protected:
   unsigned long m_nHash;

protected:
   // static method assigns tokenizer as global dictionary of words
   CStringTokenizer &getTokenizer() const { thread_local static CStringTokenizer tokenizer; return tokenizer;}

public:
   enum {UNKNOWN=0, EMPTY=1};

public:
   CWord() : m_nHash(EMPTY) { }
   explicit CWord(const std::string &s, bool bModify=true) : m_nHash(bModify ?  getTokenizer().lookup(s) : getTokenizer().find(s, UNKNOWN)) { }
   explicit CWord(const unsigned long &n) : m_nHash(n) { }
   CWord(const CWord &w) : m_nHash(w.m_nHash) { }
//   CWord(const CWord *w) : m_nHash(w->m_nHash) { }
   ~CWord() {}

public:
   inline unsigned long hash() const { return m_nHash; }
   inline unsigned long code() const { return m_nHash; }
   inline bool operator == (const CWord &w) const { return m_nHash == w.m_nHash; }
   inline bool operator != (const CWord &w) const { return m_nHash != w.m_nHash; }
   inline bool operator < (const CWord &w) const { return m_nHash < w.m_nHash; }
//   void operator = (const std::string &s) { m_nHash = getTokenizer().lookup(s); }
   inline void operator = (const CWord &w) { m_nHash =  w.m_nHash; }

   inline void copy(const CWord &w) { m_nHash = w.m_nHash; }
   inline void setString(const std::string &s) { m_nHash = getTokenizer().find(s, UNKNOWN); }
   inline void load/*=*/ (const std::string &s) { m_nHash = getTokenizer().lookup(s); }
   //addedzms
   inline void load/*=*/ (const unsigned long &n) { m_nHash = n; }
   // do not use str() for unknown words!!

   inline const std::string &str() const { return getTokenizer().key(m_nHash); }
   inline bool empty() { return m_nHash==EMPTY; }
   inline bool unknown() { return m_nHash==UNKNOWN; }
   inline void clear() { m_nHash=EMPTY; }

   inline xxhash::XXH_errorcode
   xxhash64(void *state) const {
      return xxhash::XXH64_update(state, &m_nHash, sizeof(decltype(m_nHash)));
   }
};

//===============================================================

#include "word_common.h"

inline unsigned long hash(const CWord &w) {return w.hash();}

#endif

