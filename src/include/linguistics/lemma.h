// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * lemma.h - the definitions for lemma                          *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _LEMMA_H
#define _LEMMA_H

#include "tokenizer.h"

/*===============================================================
 *
 * definitions abstd::cout lemma 
 *
 *==============================================================*/

class CLemma {
protected:
  unsigned long m_nEmpty;
  unsigned long m_nHash;

public:
  CLemma() : m_nEmpty(0), m_nHash(0) { }
  CLemma(const std::string &s, CStringTokenizer &tokenizer) : m_nEmpty(tokenizer.lookup("")), m_nHash(tokenizer.lookup(s)) { }
  CLemma(const CLemma &w) : m_nEmpty(w.m_nEmpty), m_nHash(w.m_nHash) { }
  CLemma(const CLemma &&w) : m_nEmpty(w.m_nEmpty), m_nHash(w.m_nHash) { }

public:
  inline unsigned long code() const { return m_nHash; }
  inline bool empty() const { return m_nHash == m_nEmpty; }
  inline unsigned long hash() const { return m_nHash; }
  inline const std::string &str(CStringTokenizer &tokenizer) const { return tokenizer.key(m_nHash); }

  inline bool operator ==(const CLemma &w) const { return m_nHash == w.m_nHash; }
  inline bool operator !=(const CLemma &w) const { return m_nHash != w.m_nHash; }
  inline bool operator <(const CLemma &w) const { return m_nHash < w.m_nHash; }

  inline CLemma &
  operator =(const CLemma &w) {
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


#if 0
//===============================================================
inline std::istream &
operator >>(std::istream &is, CLemma &l) {
  l.load(mp::read_str(is));
  return is ;
}

inline std::ostream &
operator <<(std::ostream &os, const CLemma &l) {
  mp::write_str(os, l.str());
  return os ;
}
#endif

#endif
