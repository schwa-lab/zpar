// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * pool.h - the pool                                            *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _MEMORY_POOL_H
#define _MEMORY_POOL_H

/*===============================================================
 *
 * Memory pool entry
 *
 *==============================================================*/
template <typename T>
class CMemoryPoolEntry {
public:
   T* blocks;
   CMemoryPoolEntry *prev;
   const unsigned long blocksize;

public:
   explicit CMemoryPoolEntry(const unsigned long bs) : blocks(new T[bs]), prev(0), blocksize(bs) { }
   ~CMemoryPoolEntry() {
      delete [] blocks;
   }
};

/*===============================================================
 *
 * Memory pool
 *
 *==============================================================*/
template <typename T>
class CMemoryPool {
protected:
   CMemoryPoolEntry<T> *current;
   unsigned long nItem;
   unsigned long blocksize;

public:
   explicit CMemoryPool(const unsigned long bs): current(0), nItem(0), blocksize(bs) { }
   ~CMemoryPool() {
      reset();
   }

protected:
   void newblock() {
      CMemoryPoolEntry<T> *iter = current;
      current = new CMemoryPoolEntry<T>(blocksize);
      current->prev = iter;
      nItem = 0;
      blocksize<<=1;
   }

public:
   T* allocate() {
      if (current==0 || nItem==current->blocksize) { // if start or block full
         newblock();
      }
      return &(current->blocks[nItem++]) ;
   }
   void reset() {
      CMemoryPoolEntry<T> *iter = current;
      while (iter) {
         CMemoryPoolEntry<T> *prev = iter->prev;
         delete iter;
         iter = prev;
      }
      current=0;
   }
};

#endif
