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
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include <schwa/msgpack.h>

namespace mp = ::schwa::msgpack;

/* stream operators for tuple */
template <typename T1, typename T2>
inline std::ostream &
operator <<(std::ostream &os, const std::tuple<T1, T2> &tuple) {
  mp::write_array_size(os, 2);
  os << std::get<0>(tuple);
  os << std::get<1>(tuple);
  return os;
}

template <typename T1, typename T2, typename T3>
inline std::ostream &
operator <<(std::ostream &os, const std::tuple<T1, T2, T3> &tuple) {
  mp::write_array_size(os, 3);
  os << std::get<0>(tuple);
  os << std::get<1>(tuple);
  os << std::get<2>(tuple);
  return os;
}

template <typename T1, typename T2>
inline std::istream &
operator >>(std::istream &is, std::tuple<T1, T2> &tuple) {
  const uint32_t nitems = mp::read_array_size(is);
  assert(nitems == 2); (void)nitems;
  is >> std::get<0>(tuple);
  is >> std::get<1>(tuple);
  return is;
}

template <typename T1, typename T2, typename T3>
inline std::istream &
operator >>(std::istream &is, std::tuple<T1, T2, T3> &tuple) {
  const uint32_t nitems = mp::read_array_size(is);
  assert(nitems == 3); (void)nitems;
  is >> std::get<0>(tuple);
  is >> std::get<1>(tuple);
  is >> std::get<2>(tuple);
  return is;
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

