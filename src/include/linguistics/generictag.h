// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * generictag.h - the definitions for generic tags with str;    *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _GENERIC_TAG_H
#define _GENERIC_TAG_H

#include "tokenizer.h"

/*===============================================================
 *
 * definitions about tag
 *
 * Each tag is associated with a tagset.
 * This is different from the concept of POS tags, which are
 * assocaited with a universal set. When a program runs, the
 * set of POS tag for each language is fixed.
 * For a generic tag, de to its generic purpose, there can
 * be differnt sets of tags coexistance. An example is CoNLL CTag/CFeats.
 *
 *==============================================================*/

class CGenericTag {
private:
  unsigned long m_nEmpty;
  unsigned long m_nHash;

public:
  CGenericTag() : m_nEmpty(0), m_nHash(0) { }
  CGenericTag(const std::string &s, CStringTokenizer &tokenizer) : m_nEmpty(tokenizer.lookup("")), m_nHash(tokenizer.lookup(s)) { }
  CGenericTag(const CGenericTag &t) : m_nEmpty(t.m_nEmpty), m_nHash(t.m_nHash) { }
  CGenericTag(const CGenericTag &&t) : m_nEmpty(t.m_nEmpty), m_nHash(t.m_nHash) { }

  inline CGenericTag &
  operator =(const CGenericTag &w) {
    m_nEmpty = w.m_nEmpty;
    m_nHash = w.m_nHash;
    return *this;
  }

  inline unsigned long code() const { return m_nHash; }
  inline unsigned long hash() const { return m_nHash; }
  inline const std::string &str(CStringTokenizer &tokenizer) const { return tokenizer.key(m_nHash); }

  inline bool operator ==(const CGenericTag &w) const { return m_nHash == w.m_nHash; }
  inline bool operator !=(const CGenericTag &w) const { return m_nHash != w.m_nHash; }
  inline bool operator <(const CGenericTag &w) const { return m_nHash < w.m_nHash; }
  inline bool operator >(const CGenericTag &w) const { return m_nHash > w.m_nHash; }
  inline bool operator <=(const CGenericTag &w) const { return m_nHash <= w.m_nHash; }
  inline bool operator >=(const CGenericTag &w) const { return m_nHash >= w.m_nHash; }

  inline void clear() { m_nHash = m_nEmpty; }
  inline void
  load(const std::string &s, CStringTokenizer &tokenizer) {
    m_nEmpty = tokenizer.lookup("");
    m_nHash = tokenizer.lookup(s);
  }
};

#endif
