// Copyright (C) University of Oxford 2010
#ifndef _DEPPARSER_BASE_INCLUDE
#define _DEPPARSER_BASE_INCLUDE
#define SIMPLE_HASH
#include "definitions.h"
#include "utils.h"
#include "pos/penn.h"
#include "tags.h"
#include "linguistics/word_tokenized.h"
#include "linguistics/taggedword.h"
#include "linguistics/dependency.h"
#ifdef LABELED
#include "linguistics/dependencylabel.h"
#endif
#ifdef LABELED
   typedef CLabeledDependencyTree CDependencyParse;
#else
   typedef CDependencyTree CDependencyParse;
#endif
#include "dep.h"
namespace TARGET_LANGUAGE { 
namespace depparser { 
#include "depparser_macros.h" 
}
}

#include "linguistics/conll.h"
#include "depparser_impl_inc.h"
#include "linguistics/tagset.h"
#include "options.h"

#include "supertag.h"

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS 
#define BOOST_MPL_LIMIT_LIST_SIZE 3
#define BOOST_MPL_LIMIT_VECTOR_SIZE 3
#include <boost/variant.hpp>

#endif
