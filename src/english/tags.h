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


//===============================================================

inline unsigned long hash(const CTag &tag) { return tag.code();}

//===============================================================

inline std::istream & operator >> (std::istream &is, CTag &tag) {
   std::string s;
   is >> s;
   tag.load(s);
   return is;
}

inline std::ostream & operator << (std::ostream &os, const CTag &tag) {
   os << tag.str() ;
   return os;
}

//===============================================================
} // namespace english

#include "pos/penn_morph.h"

#endif

