// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * hash.h - the hash table                                      *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.11                        *
 *                                                              *
 ****************************************************************/

#ifndef _HASH_SIMPLE_H
#define _HASH_SIMPLE_H

#include "pool.h"

static const unsigned long POOL_BLOCK_SIZE=(1<<16);

/*===============================================================
 *
 * Hash table
 *
 *==============================================================*/

template <typename K, typename V>
class CHashMap {
protected:
   //===============================================================
   // Hash table entry
   //===============================================================
   class CEntry {
   public:
      K m_key;
      V m_value;
      CEntry *m_next;

   public:
      CEntry() : m_key(), m_value(), m_next(0) {}
      explicit CEntry(const K &key) : m_key(key), m_value(), m_next(0) {}
      CEntry(const K &key, const V &value) : m_key(key), m_value(value), m_next(0){}
   };

public:
   //===============================================================
   // Hash table iterator class
   //===============================================================
   class iterator {
   private:
      unsigned long int m_nBucket;
      CHashMap<K, V> *m_parent;
      CEntry *m_entry;

   private:
      void validate() {
         // when the next item is at the end of the bucket, move on
         assert(m_nBucket < m_parent->m_nTableSize);
         while (m_entry == 0) { 
            if (m_nBucket == m_parent->m_nTableSize-1) 
               return; 
            else { 
               m_entry = m_parent->m_buckets[++m_nBucket]; 
               continue; 
            } 
         } 
      }

   public:
      iterator() {}
      iterator(CHashMap<K, V> *parent, int bucket, CEntry *entry) : m_nBucket(bucket), m_parent(parent), m_entry(entry) {
         validate();
      }
      iterator(const iterator &it) { m_parent = it.m_parent; m_nBucket = it.m_nBucket; m_entry = it.m_entry; }
      bool operator != (const iterator &it) const { return !((*this)==it);}
      bool operator == (const iterator &it) const { return m_parent == it.m_parent && m_nBucket == it.m_nBucket && m_entry == it.m_entry; }
      // move to next places
      void operator ++ () { 
         assert(m_entry != 0);
         m_entry=m_entry->m_next ;  
         validate();
      }
      bool valid() const { if (m_nBucket < 0 || m_nBucket > m_parent->m_nTableSize-1 || m_entry == 0) return false; return true; }

      const K &first() { return m_entry->m_key; }
      V &second() { return m_entry->m_value; }
   }; 

   //===============================================================

protected:
   const unsigned long int m_nTableSize;
   CEntry **const m_buckets;

public:
   explicit CHashMap(unsigned long TABLE_SIZE) : m_nTableSize(TABLE_SIZE), m_buckets(new CEntry*[m_nTableSize]) { 
      memset(m_buckets, 0, m_nTableSize*sizeof(CEntry*));
   }
   virtual ~CHashMap() { 
      clear();
      delete [] m_buckets;
   }

protected:
   CEntry *&getEntry(const K &key) { return m_buckets[hash(key)%m_nTableSize]; }
   CEntry * const &getEntry(const K &key) const { return m_buckets[hash(key)%m_nTableSize]; }

   static CMemoryPool<CEntry> &getPool() { thread_local static CMemoryPool<CEntry> pool(POOL_BLOCK_SIZE); return pool; }
   static CEntry* &getFreeMemory() { thread_local static CEntry* c_free = 0; return c_free; }

   CEntry *allocate() {
      CEntry* &c_freed = getFreeMemory();
      if (c_freed) {
         CEntry *retval = c_freed;
         c_freed = c_freed->m_next;
         retval->m_next = 0;
         return retval;
      }
      else {
         return getPool().allocate();
      }
   }

public:
   V &operator[] (const K &key) { 
      CEntry* entry = getEntry(key); 
      if (entry==0) {
         entry = getEntry(key) = allocate(); 
         entry->m_key = key;
         return entry->m_value;
      }
      while (true) {
         assert(entry);
         if (entry->m_key==key)
            return entry->m_value;
         else {
            if (entry->m_next==0)
               break;
            else
               entry = entry->m_next;
         }
      }
      assert(entry);
      entry->m_next = allocate();
      entry->m_next->m_key = key;   
      return entry->m_next->m_value;
   }
   const V &operator [] (const K &key) const { 
      const CEntry*entry=getEntry(key); 
      while (entry) {
         if (entry->m_key == key)
            return entry->m_value;
         else
            entry = entry->m_next;
      }
      THROW("const[]: Cannot find key in hashmap.");
   }
   void insert (const K &key, const V &val) { (*this)[key] = val; }
   const V &find (const K &key, const V &val) const { 
      const CEntry*entry=getEntry(key); 
      while (entry) {
         if (entry->m_key == key)
            return entry->m_value;
         else
            entry = entry->m_next;
      }
      return val;
   }
   bool findorinsert (const K &key, const V &val, V &retval) { 
      CEntry*entry=getEntry(key); 
      if (entry == 0) { 
         retval = val; 
         entry= getEntry(key) =allocate(); 
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
       entry->m_next = allocate();
       entry->m_next->m_key = key;
       entry->m_next->m_value = val;
       retval = val;
       return true;
   }
   bool element (const K &key) const { 
      CEntry*entry=getEntry(key); 
      while (entry) {
         if (entry->m_key == key)
            return true;
         else
            entry = entry->m_next;
      }
      return false;
   }
   void clear() {
      thread_local static V value;
      CEntry * tail = 0;
      for (unsigned i = 0; i < m_nTableSize; ++i) {
         if (m_buckets[i]) {
            tail = m_buckets[i];
            while (tail->m_next) {
               tail->m_value = value;
               tail = tail->m_next;
            }
            tail->m_value = value;
            CEntry* &c_freed = getFreeMemory();
            tail->m_next = c_freed;
            c_freed = m_buckets[i];
            m_buckets[i]=0;
         }
      }
   }

public:
   iterator begin() { 
      return iterator(this, 0, m_buckets[0]); 
   }
   iterator end() { 
      return iterator(this, m_nTableSize-1, 0); 
   }

  void debugUsage(void) const {
    unsigned int nitems = 0, nbuckets_used = 0, ncollisions = 0;
    for (unsigned long int b = 0; b != m_nTableSize; ++b) {
      if (m_buckets[b] == nullptr)
        continue;
      ++nbuckets_used;
      for (const CEntry *node = m_buckets[b]; node != nullptr; node = node->m_next) {
        ++nitems;
        if (node != m_buckets[b])
          ++ncollisions;
      }
    }
    std::cout << "[debugUsage] nbuckets=" << m_nTableSize << " nbuckets_used=" << nbuckets_used << " nitems=" << nitems << " ncollisions=" << ncollisions;
    std::cout << " bucket usage %age=" << (100 * nbuckets_used/static_cast<float>(m_nTableSize));
    const float collision_percentage = (nitems == 0) ? 0.0f : 100 * ncollisions/static_cast<float>(nitems);
    if (nitems != 0) {
      std::cout << " collision %age=" << collision_percentage;
      if (collision_percentage > 30) {
        std::cout << std::endl << "{" << std::endl;
        for (unsigned long int b = 0; b != m_nTableSize; ++b) {
          if (m_buckets[b] == nullptr)
            continue;
          std::cout << "  " << b << ")";
          for (const CEntry *node = m_buckets[b]; node != nullptr; node = node->m_next) {
            std::cout << " [" << node->m_key << "(" << hash(node->m_key) << ", " << (hash(node->m_key) % m_nTableSize) << "): " << &node->m_value << "]";
          }
          std::cout << std::endl;
        }
        std::cout << "}";
      }
    }
    std::cout << std::endl;
  }

public:
#ifdef DEBUG 
   void trace() { 
      std::cout << "tracing size:amount" << std::endl;
      std::map<unsigned, unsigned> statistic;
      for (unsigned i=0; i<m_nTableSize; ++i) {
         unsigned size = 0;
         CEntry* entry = m_buckets[i];
         while (entry) {
            ++size;
            entry=entry->m_next;
         }
         ++statistic[size];
      }
      std::map<unsigned, unsigned>::iterator it;
      for (it=statistic.begin(); it!=statistic.end(); ++it)
         if (it->second != 0)
            std::cout << it->first << ':' << it->second << " (" << float(it->second)/m_nTableSize << ")" << std::endl;
      std::cout << "done" << std::endl;
   }
#endif

//public:
//   static void freePoolMemory() { // call after all instances clean!
//      getPool().reset();
//      getFreeMemory() = 0;
//   }

};


#endif
