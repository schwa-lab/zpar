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

#include "tokenizer.h"
#include "bigram.h"


class CWord {
private:
  unsigned long m_nEmpty;
  unsigned long m_nHash;

public:
  CWord() : m_nEmpty(0), m_nHash(0) { }
  CWord(const std::string &s, CStringTokenizer &tokenizer) : m_nEmpty(tokenizer.lookup("")), m_nHash(tokenizer.lookup(s)) { }
  CWord(const CWord &w) : m_nEmpty(w.m_nEmpty), m_nHash(w.m_nHash) { }
  CWord(const CWord &&w) : m_nEmpty(w.m_nEmpty), m_nHash(w.m_nHash) { }

  inline unsigned long code() const { return m_nHash; }
  inline bool empty() const { return m_nHash == m_nEmpty; }
  inline unsigned long hash() const { return m_nHash; }
  inline const std::string &str(const CStringTokenizer &tokenizer) const { return tokenizer.key(m_nHash); }

  inline bool operator ==(const CWord &w) const { return m_nHash == w.m_nHash; }
  inline bool operator !=(const CWord &w) const { return m_nHash != w.m_nHash; }
  inline bool operator <(const CWord &w) const { return m_nHash < w.m_nHash; }

  inline CWord &
  operator =(const CWord &w) {
    m_nEmpty = w.m_nEmpty;
    m_nHash = w.m_nHash;
    return *this;
  }

  inline void clear() { m_nHash = m_nEmpty; }
  inline void
  load(const std::string &s, CStringTokenizer &tokenizer) {
    m_nEmpty = tokenizer.lookup("");
    m_nHash = tokenizer.lookup(s);
  }
};

#include "word_common.h"

#endif
