// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * tags.h - the definitions for english tags                    *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _ENGLISH_TAGS_H
#define _ENGLISH_TAGS_H

#include "pos/penn.h"

namespace english {

const char TAG_SEPARATOR = '/';

} // namespace english

//===============================================================

inline unsigned long hash(const english::CTag &tag) { return tag.code();}

//===============================================================
inline std::istream &
operator >>(std::istream &is, english::CTag &tag) {
  tag.load(mp::read_str(is));
  return is;
}

inline std::ostream &
operator <<(std::ostream &os, const english::CTag &tag) {
  mp::write_str(os, tag.str());
  return os;
}

//===============================================================

#include "pos/penn_morph.h"

#endif
