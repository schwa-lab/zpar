// Copyright (C) University of Oxford 2010

/****************************************************************
 *                                                              *
 * definitions.h - the general definitions                      *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/*===============================================================
 *
 * The things that should be included 
 *  
 *==============================================================*/

#include <cstdlib>                              // define NULL
#include <utility>                              // define std::pair
#include <cassert>                              // assert
#include <ctime>                                // clock(); CLK_TCK
#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <set>
#include <stack>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <algorithm>
#include <cmath>
#include <tuple>

#include <schwa/hashtable.h>

namespace mp = ::schwa::msgpack;
namespace xxhash = ::schwa::third_party::xxhash;

//using namespace std;

/* stream operators for tuple */

template <typename T1, typename T2>
std::ostream &operator<<(std::ostream &os, const std::tuple<T1, T2> &tuple) {
  os << std::get<0>(tuple) << " , " << std::get<1>(tuple);
  return os;
}

template <typename T1, typename T2, typename T3>
std::ostream &operator<<(std::ostream &os, const std::tuple<T1, T2, T3> &tuple) {
  os << std::get<0>(tuple) << " , " << std::get<1>(tuple) << " , " << std::get<2>(tuple);
  return os;
}

template <typename T1, typename T2>
std::istream &operator>>(std::istream &is, std::tuple<T1, T2> &tuple) {
  char c;
  T1 tmp1;
  is >> tmp1;
  std::get<0>(tuple) = tmp1;
  is >> c;
  assert(c == ',');
  T2 tmp2;
  is >> tmp2;
  std::get<1>(tuple) = tmp2;
  return is;
}

template <typename T1, typename T2, typename T3>
std::istream &operator>>(std::istream &is, std::tuple<T1, T2, T3> &tuple) {
  char c;
  T1 tmp1;
  is >> tmp1;
  std::get<0>(tuple) = tmp1;
  is >> c;
  assert(c == ',');
  T2 tmp2;
  is >> tmp2;
  std::get<1>(tuple) = tmp2;
  is >> c;
  assert(c == ',');
  T3 tmp3;
  is >> tmp3;
  std::get<2>(tuple) = tmp3;
  return is;
}

namespace schwa {

  template <typename T0, typename T1>
  struct Hasher64<std::tuple<T0, T1>> : public Hasher64Base<std::tuple<T0, T1>> {
    using typename Hasher64Base<std::tuple<T0, T1>>::argument_type;
    using typename Hasher64Base<std::tuple<T0, T1>>::result_type;

    inline xxhash::XXH_errorcode
    operator ()(const argument_type &obj, void *state) const {
      Hasher64<T0>()(std::get<0>(obj), state);
      Hasher64<T1>()(std::get<1>(obj), state);
      return xxhash::XXH_OK;
    }
  };

  template <typename T0, typename T1, typename T2>
  struct Hasher64<std::tuple<T0, T1, T2>> : public Hasher64Base<std::tuple<T0, T1, T2>> {
    using typename Hasher64Base<std::tuple<T0, T1, T2>>::argument_type;
    using typename Hasher64Base<std::tuple<T0, T1, T2>>::result_type;

    inline xxhash::XXH_errorcode
    operator ()(const argument_type &obj, void *state) const {
      Hasher64<T0>()(std::get<0>(obj), state);
      Hasher64<T1>()(std::get<1>(obj), state);
      Hasher64<T2>()(std::get<2>(obj), state);
      return xxhash::XXH_OK;
    }
  };

}

/*===============================================================
 *
 * debugging utilities
 *  
 *==============================================================*/

// throw
#define THROW(x) { std::ostringstream __tmp_os; __tmp_os << x; throw(__tmp_os.str()); }

// assert
#define ASSERT(x,y) { if (!(x)) THROW(y); }

// error report tool
#define REPORT(x) { std::cerr << std::endl << "In " << __FILE__ << ", line " << __LINE__ << ": " << std::endl << x << std::endl; std::cerr.flush(); }
#define WARNING(x) { std::cerr << "Warning:" << " " << x << std::endl; std::cerr.flush(); }

// trace tool
#ifdef DEBUG 
#define TRACE(x) { std::cout << x << std::endl; std::cout.flush(); }
#define TRACE_WORD(x) { std::cout << x; std::cout.flush(); }
#define TRACE_IF(x, y) { if (x) { std::cout << y << std::endl; std::cout.flush(); } }
#else
#define TRACE(x) 
#define TRACE_WORD(x)
#define TRACE_IF(x, y)
#endif

// conditional compiling
#ifdef DEBUG
#define WHEN_DEBUGGING(x) x
#else
#define WHEN_DEBUGGING(x) 
#endif

/*===============================================================
 *
 * definitions abstd::cout sentence structures 
 *  
 *==============================================================*/

#include "linguistics/sentence_string.h"

#endif

