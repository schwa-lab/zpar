// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * tokenizer.h - the tokenizer.                                 *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.5                         *
 *                                                              *
 ****************************************************************/

#ifndef _TOKENIZER_H
#define _TOKENIZER_H

#include "hash.h"

//static const unsigned TOKENIZER_SIZE = 65537 ;

/*===============================================================
 *
 * Tokenizer
 *
 *==============================================================*/

template <typename K>
class CTokenizer {
protected:
  std::hash<K> _hasher;
  std::unordered_map<unsigned long, std::string> _tokens;

public:
  CTokenizer() { }

  inline unsigned long
  find(const K &key, const unsigned long default_val) const {
    const auto &it = _tokens.find(_hasher(key));
    return it == _tokens.end() ? default_val : it->first;
  }

  inline const K &
  key(const unsigned long hash) const {
    const auto &it = _tokens.find(hash);
    if (it == _tokens.end()) {
      std::ostringstream ss;
      ss << "Unknown key 0x" << std::hex << hash << " passed to CTokenizer.key()";
      throw std::runtime_error(ss.str());
    }
    return it->second;
  }

  unsigned long
  lookup(const K &key) {
    const unsigned long hash = _hasher(key);
    const auto &it = _tokens.find(hash);
    if (it == _tokens.end())
      _tokens.emplace(hash, key);
    return hash;
  }
};


using CStringTokenizer = CTokenizer<std::string>;

#endif
