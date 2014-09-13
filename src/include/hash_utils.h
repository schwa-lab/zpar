// Copyright (C) University of Oxford 2010
#ifndef _HASH_FUNCTION_UTIL_H
#define _HASH_FUNCTION_UTIL_H

#include "definitions.h"

enum class Feature { m_mapSTw, m_mapSTt, m_mapSTwt, m_mapN0w, m_mapN0t, m_mapN0wt, m_mapN1w, m_mapN1t, m_mapN1wt, m_mapN2w, m_mapN2t, m_mapN2wt, m_mapSTHw, m_mapSTHt, m_mapSTi, m_mapSTHHw, m_mapSTHHt, m_mapSTHi, m_mapSTLDw, m_mapSTLDt, m_mapSTLDi, m_mapSTRDw, m_mapSTRDt, m_mapSTRDi, m_mapN0LDw, m_mapN0LDt, m_mapN0LDi, m_mapSTL2Dw, m_mapSTL2Dt, m_mapSTL2Di, m_mapSTR2Dw, m_mapSTR2Dt, m_mapSTR2Di, m_mapN0L2Dw, m_mapN0L2Dt, m_mapN0L2Di, m_mapHTw, m_mapHTt, m_mapHTwt, m_mapSTwtN0wt, m_mapSTwtN0w, m_mapSTwN0wt, m_mapSTtN0wt, m_mapSTwtN0t, m_mapSTwN0w, m_mapSTtN0t, m_mapN0tN1t, m_mapN0tN1tN2t, m_mapSTtN0tN1t, m_mapSTtN0tN0LDt, m_mapN0tN0LDtN0L2Dt, m_mapSTHtSTtN0t, m_mapHTtHT2tN0t, m_mapSTHHtSTHtSTt, m_mapSTtSTLDtN0t, m_mapSTtSTLDtSTL2Dt, m_mapSTtSTRDtN0t, m_mapSTtSTRDtSTR2Dt, m_mapSTwd, m_mapSTtd, m_mapN0wd, m_mapN0td, m_mapSTwN0wd, m_mapSTtN0td, m_mapSTwra, m_mapSTtra, m_mapSTwla, m_mapSTtla, m_mapN0wla, m_mapN0tla, m_mapSTwrp, m_mapSTtrp, m_mapSTwlp, m_mapSTtlp, m_mapN0wlp, m_mapN0tlp, m_mapSTl, m_mapSTc, m_mapSTf, m_mapN0l, m_mapN0c, m_mapN0f, m_mapN1l, m_mapN1c, m_mapN1f };

// The idea here is to provide a global hash function for basic types
// but for all types defined in my code they are give a hash() member
// so that the inline hash functions are always there.

inline unsigned long hash(const unsigned int &i) {return i;}
inline unsigned long hash(const int &i) {return i;} //return i>0?i:-i;}

inline const unsigned long &hash(const unsigned long &i) {return i;}
inline unsigned long hash(const long &i) {return i;} //return i>0?i:-i;}

inline unsigned long hash(const long long &i) {return i;} //return i>0?i:-i;}
inline unsigned long hash(const unsigned long long &i) {return i;}

inline unsigned long hash(const std::string &s) {unsigned long retval=0; for (unsigned long i=0; i<s.size(); ++i) retval=retval*33+(static_cast<unsigned char>(s[i])&127); return retval;}

template<typename T>
inline unsigned long hash(const T& t) { return t.hash(); }

template<>
inline unsigned long hash(const Feature& t) { return std::hash<int>()(static_cast<int>(t)); }

template<typename T1, typename T2>
inline unsigned long hash(const std::pair<T1, T2> &o) { return hash(o.first)*33 + hash(o.second) ; }

#endif
