// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * taggedword.h - the definitions for tagged general words      *
 *                                                              *
 * A tagged word is a word with a tag. Since the definition of  *
 * CWord can be implemented in different ways, a proper file    *
 * that defines CWord must be included before using this file.  *
 * This file presumes that CWord has already been defined.      *
 * Similarly, CTag must have been defined before including this *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _TAGGED_WORD_H
#define _TAGGED_WORD_H

/*===============================================================
 *
 * definitions abstd::cout tagged word 
 *
 *==============================================================*/

// the word class, which includes hash
template <typename CTag, char sTagSep>
class CTaggedWord {
public:
  CWord word;
  CTag tag;

  CTaggedWord() {}
  CTaggedWord(const std::string &s, CStringTokenizer &word_tokenizer, const std::string &t) : word(s, word_tokenizer), tag(t) { }
  CTaggedWord(const CWord &w, const CTag &t) : word(w), tag(t) { }
  CTaggedWord(const CTaggedWord &w) : word(w.word), tag(w.tag) { }
  ~CTaggedWord() {}

  // the ordering of words are defined:
  // when the hash are not equal, order is defined by hash
  // when the hash are equal, order is defined by char-value
  inline bool operator < (const CTaggedWord &w) const { return word == w.word ? tag < w.tag : word < w.word ; }
  inline bool operator == (const CTaggedWord &w) const { return word == w.word && tag == w.tag ; }
  inline bool operator != (const CTaggedWord &w) const { return !(*this == w); }

  inline bool empty() const { return word.empty() && tag.empty(); }
  inline unsigned long hash() const { return word.hash() + 31*tag.hash(); }

  inline void clear() { word.clear(); tag.clear(); }

  inline void
  load(const CWord &word, const CTag &tt=CTag::NONE) {
    this->word = (word) ;
    tag = tt ;
  }

  // assign value
  inline CTaggedWord &
  operator =(const CTaggedWord &tw) {
    word = tw.word;
    tag = tw.tag;
    return *this;
  }

  template <typename T0, char T1>
  friend std::istream &operator >>(std::istream &, CTaggedWord<T0, T1> &tw);
  template <typename T0, char T1>
  friend std::ostream &operator <<(std::ostream &, const CTaggedWord<T0, T1> &tw);
};

//===============================================================

template <typename CTag, char sTagSep>
inline std::istream &
operator >>(std::istream &is, CTaggedWord<CTag, sTagSep> &tw) {
  const uint32_t nitems = mp::read_array_size(is);
  assert(nitems == 2); (void)nitems;
  is >> tw.word;
  is >> tw.tag;
  return is;
}

template <typename CTag, char sTagSep>
inline std::ostream &
operator <<(std::ostream &os, const CTaggedWord<CTag, sTagSep> &tw) {
  mp::write_array_size(os, 2);
  os << tw.word;
  os << tw.tag;
  return os;
}


#endif

