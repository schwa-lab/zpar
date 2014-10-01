// Copyright (C) University of Oxford 2010
#ifndef _HASH_FUNCTION_UTIL_H
#define _HASH_FUNCTION_UTIL_H

#include "definitions.h"

// The idea here is to provide a global hash function for basic types
// but for all types defined in my code they are give a hash() member
// so that the inline hash functions are always there.

inline unsigned long hash(const unsigned int i) {return i;}
inline unsigned long hash(const int i) {return i;} //return i>0?i:-i;}

inline unsigned long hash(const unsigned long i) {return i;}
inline unsigned long hash(const long i) {return i;} //return i>0?i:-i;}

inline unsigned long hash(const long long i) {return i;} //return i>0?i:-i;}
inline unsigned long hash(const unsigned long long i) {return i;}

inline unsigned long
hash(const std::string &s) { return std::hash<std::string>()(s); }

template<typename T>
inline unsigned long hash(const T& t) { return t.hash(); }

template<typename T1, typename T2>
inline unsigned long hash(const std::pair<T1, T2> &o) { return hash(o.first)*33 + hash(o.second) ; }

template<typename T1, typename T2>
inline unsigned long hash(const std::tuple<T1, T2> &o) { return hash(std::get<0>(o))*31 + hash(std::get<1>(o)) ; }

template<typename T1, typename T2, typename T3>
inline unsigned long hash(const std::tuple<T1, T2, T3> &o) { return hash(std::get<0>(o))*31 + hash(std::get<1>(o))*29 + hash(std::get<2>(o)) ; }

template<typename T1, typename T2, typename T3, typename T4>
inline unsigned long hash(const std::tuple<T1, T2, T3, T4> &o) { return hash(std::get<0>(o))*31 + hash(std::get<1>(o))*29 + hash(std::get<2>(o))*23 + hash(std::get<3>(o)) ; }


template <typename T>
struct Hasher {
  using result_type = uint64_t;

  inline result_type operator ()(const T &obj) { return hash(obj); }
};

#endif
