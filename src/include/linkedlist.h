// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * linkedlist.h - the linked list                               *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

#include "pool.h"

template <typename K, typename V>
class CLinkedList {
public:
  class CEntry {
  public:
    K m_key;
    V m_value;
    CEntry *m_next;

  public:
    CEntry() : m_next(0) { }
    explicit CEntry(const K &key) : m_key(key),  m_next(0) { }
    CEntry(const K &key, const V &value) : m_key(key), m_value(value), m_next(0) { }
  };


  class iterator {
  private:
    CEntry *m_entry;

  public:
    explicit iterator(CEntry *entry) : m_entry(entry) { }
    iterator(const iterator &o) : m_entry(o.m_entry) { }
    iterator(const iterator &&o) : m_entry(o.m_entry) { }

    inline bool operator ==(const iterator &o) const { return m_entry == o.m_entry; }
    inline bool operator !=(const iterator &o) const { return m_entry != o.m_entry; }
    inline void operator ++() { m_entry = m_entry == nullptr ? nullptr : m_entry->m_next; }
    inline const K &first() { return m_entry->m_key; }
    inline V &second() { return m_entry->m_value; }
  };


  class const_iterator {
  private:
    const CEntry *m_entry;

  public:
    explicit const_iterator(const CEntry *entry) : m_entry(entry) { }
    const_iterator(const const_iterator &o) : m_entry(o.m_entry) { }
    const_iterator(const const_iterator &&o) : m_entry(o.m_entry) { }

    inline bool operator ==(const const_iterator &o) const { return m_entry == o.m_entry; }
    inline bool operator !=(const const_iterator &o) const { return m_entry != o.m_entry; }
    inline void operator ++() { m_entry = m_entry == nullptr ? nullptr : m_entry->m_next; }
    inline const K &first() { return m_entry->m_key; }
    inline const V &second() { return m_entry->m_value; }
  };

public:
  using PoolItemType = CEntry;
  static constexpr const size_t POOL_BLOCK_SIZE = 8 * sizeof(CEntry);

private:
  CEntry *m_buckets;

public:
  CLinkedList() : m_buckets(0) { }
  ~CLinkedList() { }

  CEntry *
  allocate(CMemoryPool<CEntry> &pool, CEntry **free) {
    if (*free) {
      CEntry *retval = *free;
      *free = (*free)->m_next;
      retval->m_next = 0;
      return retval;
    }
    else
      return pool.allocate();
  }

  V &
  get(const K &key, CMemoryPool<CEntry> &pool, CEntry **free) {
    CEntry *entry = m_buckets;
    if (entry==0) {
      entry = m_buckets = allocate(pool, free);
      entry->m_key = key;
      return entry->m_value;
    }
    while (true) {
      if (entry->m_key==key)
        return entry->m_value;
      else {
        if (entry->m_next==0)
          break;
        else
          entry = entry->m_next;
      }
    }
    entry->m_next = allocate(pool, free);
    entry->m_next->m_key = key;
    return entry->m_next->m_value;
  }

  inline void
  insert (const K &key, const V &val) {
    (*this)[key] = val;
  }

  const V &
  find(const K &key, const V &val) const {
    const CEntry *entry = m_buckets;
    while (entry) {
      if (entry->m_key == key)
        return entry->m_value;
      else
        entry = entry->m_next;
    }
    return val;
  }

  bool
  findorinsert(const K &key, const V &val, V &retval, CMemoryPool<CEntry> &pool, CEntry **free) {
    CEntry *entry = m_buckets;
    if (entry == 0) {
      retval = val;
      entry = m_buckets = allocate(pool, free);
      entry->m_key = key;
      entry->m_value = val;
      return true;
    }
    while (true) {
      assert (entry);
      if (entry->m_key == key) {
        retval = entry->m_value;
        return false;
      }
      else if (entry->m_next==0)
        break;
      else
        entry = entry->m_next;
    }
    assert(entry);
    entry->m_next = allocate(pool, free);
    entry->m_next->m_key = key;
    entry->m_next->m_value = val;
    retval = val;
    return true;
  }

  bool
  element(const K &key) const {
    CEntry*entry=m_buckets;
    while (entry) {
      if (entry->m_key == key)
        return true;
      else
        entry = entry->m_next;
    }
    return false;
  }

  void
  clear(CEntry **free) {
    static const V empty = V();
    if (!m_buckets)
      return;
    CEntry *tail = m_buckets;
    while (tail->m_next) {
      tail->m_value = empty;
      tail = tail->m_next;
    }
    tail->m_value = empty;
    tail->m_next = *free;
    *free = m_buckets;
    m_buckets = 0;
  }

  inline iterator begin() { return iterator(m_buckets); }
  inline iterator end() { return iterator(nullptr); }
  inline const_iterator begin() const { return const_iterator(m_buckets); }
  inline const_iterator end() const { return const_iterator(nullptr); }
  inline const_iterator cbegin() const { return const_iterator(m_buckets); }
  inline const_iterator cend() const { return const_iterator(nullptr); }

  inline bool empty() const { return m_buckets == nullptr; }
};


template <typename K, typename V>
inline std::istream &
operator >>(std::istream &is, CLinkedList<K, V> &score_map) {
   if (!is) return is ;
   std::string s ;
   K key;
//   static V value;
//   assert(score_map.empty());
   is >> s;
   ASSERT(s=="{"||s=="{}", "The small hashmap does not start with {");
   if (s=="{}")
      return is;
   while (true) {
      is >> key;
      is >> s;
      ASSERT(s==":", "The small hashmap does not have : after key: "<<key);
//      is >> value;
//      score_map[key] = value;
      is >> score_map[key];
      is >> s;
      ASSERT(s==","||s=="}", "The small hashmap does not have a , or } after value: "<<score_map[key]);
      if (s=="}")
         return is;
   }
   THROW("hashmap_small.h: the program should not have reached here.");
   return is ;
}


template <typename K, typename V>
inline std::ostream &
operator <<(std::ostream &os, const CLinkedList<K, V> &score_map) {
   os << "{";
   typename CLinkedList<K, V>::const_iterator it = score_map.begin();
   if (it==score_map.end()) {
      os << "}"; // empty {}
      return os;
   }
   else
      os << " "; // non-empty { a , b , c }
   while (it!=score_map.end()) {
      if (it!=score_map.begin())
         os << " , ";
      os << it.first() << " : " << it.second();
      ++it;
   }
   os << " }";
   return os;
}

#endif
