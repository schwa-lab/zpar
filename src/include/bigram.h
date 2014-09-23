// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * bigram.h - the definitions for bigrams                       *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _BIGRAM_H
#define _BIGRAM_H

/*===============================================================
 *
 * Bigram
 *
 *==============================================================*/
template<class CUnigram>
class CBigram {
protected:
   unsigned long m_nHash;
   CUnigram m_unigram1;
   CUnigram m_unigram2;

public:
   CBigram() : m_nHash(0) { }
   CBigram(const CUnigram &u1, const CUnigram &u2) : m_unigram1(u1), m_unigram2(u2) { computehash(); }
   CBigram(const CBigram &o) : m_nHash(o.m_nHash), m_unigram1(o.m_unigram1), m_unigram2(o.m_unigram2) { }
   CBigram(const CBigram &&o) : m_nHash(o.m_nHash), m_unigram1(o.m_unigram1), m_unigram2(o.m_unigram2) { }

   inline unsigned long hash() const { return m_nHash; }
   const CUnigram &first() const { return m_unigram1; };
   const CUnigram &second() const { return m_unigram2; }

   inline bool operator ==(const CBigram &w) const { return m_nHash == w.m_nHash && m_unigram1 == w.m_unigram1 && m_unigram2 == w.m_unigram2; }
   inline bool operator <(const CBigram &w) const { return m_unigram1 < w.m_unigram1 || (m_unigram1 == w.m_unigram1 && m_unigram2 < w.m_unigram2); }

   void
   assign(const CUnigram &u1, const CUnigram &u2) {
      m_unigram1 = u1;
      m_unigram2 = u2;
      computehash();
   }

   // assignment
   CBigram &
   operator =(const CBigram &o) {
      m_nHash = o.m_nHash;
      m_unigram1 = o.m_unigram1;
      m_unigram2 = o.m_unigram2;
      return *this;
   }

protected:
   inline void computehash() { m_nHash = m_unigram1.hash()*31 + m_unigram2.hash(); }
};

//===============================================================
template<class CUnigram>
inline unsigned long int hash(const CBigram<CUnigram> &w) { return w.hash(); }

//===============================================================

template <class CUnigram>
inline std::istream &
operator >>(std::istream &is, CBigram<CUnigram> &bigram) {
  const uint32_t nitems = mp::read_array_size(is);
  assert(nitems == 2); (void)nitems;
  CUnigram first, second;
  is >> first >> second;
  bigram.assign(first, second);
  return is;
}

template <class CUnigram>
inline std::ostream &
operator <<(std::ostream &os, const CBigram<CUnigram> &bigram) {
  mp::write_array_size(os, 2);
  os << bigram.first();
  os << bigram.second();
  return os;
}

#endif
