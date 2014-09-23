// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * tagset.h - the definitions for general tag set.              *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2009.8                         *
 *                                                              *
 ****************************************************************/

#ifndef _TAGSET_H
#define _TAGSET_H

/*===============================================================
 *
 * definitions 
 *
 *==============================================================*/

template<typename CTag, unsigned long SIZE>
class CTagSet {
protected:
   unsigned long m_nHash;

public:
   CTagSet() : m_nHash(0) { }
   CTagSet(const unsigned long hash) : m_nHash(hash) { assert((hash >> (CTag::SIZE * SIZE)) == 0); }
   virtual ~CTagSet() {}

private:
   void operator += (const CTag &i) { 
      m_nHash = (m_nHash<<CTag::SIZE) | i.code();
   }
   unsigned long getTag(const unsigned long &tc) const {
      return tc & ((1<<(CTag::SIZE-1))-1);
   }

public:
   inline unsigned long hash() const { return m_nHash; }
   virtual bool operator == (const CTagSet &set) const { 
      return m_nHash == set.m_nHash; 
   }
   virtual bool operator != (const CTagSet &set) const { 
      return ! ((*this) == set); 
   }
   virtual bool operator < (const CTagSet &set) const { 
      return m_nHash < set.m_nHash; 
   }
   void operator = (const CTagSet &set) { 
      m_nHash = set.m_nHash; 
   }
   const std::string str() const { 
      std::string retval = "";
      unsigned long hs = m_nHash;
      unsigned long tc;
      for (unsigned long i = 0; i < SIZE; ++i) {
         if (!retval.empty())
            retval = " " + retval;
         tc = hs & ((1 << CTag::SIZE) - 1);
         retval = CTag(tc).str() + retval;
         hs >>= CTag::SIZE;
      }
      return retval;
   }
   void load(const std::string &s) {
      clear();
      std::istringstream iss(s);
      std::string t;
      iss >> t;
      for (unsigned long i=0; i<SIZE; ++i) {
         assert(iss.good());
         *this += CTag(t).code();
         iss >> t;
      }
   }
   void load(const unsigned long &i) { m_nHash = i; }
   void clear() { m_nHash = 0; }

   template <typename T0, unsigned long T1>
   friend std::istream &operator >>(std::istream &, CTagSet<T0, T1> &c);
   template <typename T0, unsigned long T1>
   friend std::ostream &operator <<(std::ostream &, const CTagSet<T0, T1> &c);
};

//===============================================================

template<typename CTag>
inline unsigned long encodeTags(const CTag &tag1, const CTag &tag2) {
   return (tag1.code()<<CTag::SIZE) | tag2.code();
}

template<typename CTag>
inline unsigned long encodeTags(const CTag &tag1, const CTag &tag2, const CTag &tag3) {
   return (tag1.code()<<(CTag::SIZE*2)) | (tag2.code()<<CTag::SIZE) | tag3.code() ;
}

template<typename CTag>
inline unsigned long encodeTags(const CTag &tag1, const CTag &tag2, const CTag &tag3, const CTag &tag4) {
   assert( CTag::SIZE*4<sizeof(unsigned long)*8 ); 
   return (tag1.code()<<CTag::SIZE*3) | (tag2.code()<<CTag::SIZE*2) |
          (tag3.code()<<CTag::SIZE) | tag4.code() ;
}


//===============================================================
template<typename CTag, unsigned long SIZE>
inline std::istream &
operator >>(std::istream &is, CTagSet<CTag, SIZE> &c) {
  c.m_nHash = mp::read_uint(is);
  return is;
}

template<typename CTag, unsigned long SIZE>
inline std::ostream &
operator <<(std::ostream &os, const CTagSet<CTag, SIZE> &c) {
  mp::write_uint(os, c.m_nHash);
  return os;
}


//=============================================================================
//
// a set of tags -- satisfy the size of CTag but static. use csetofint dynamic
//
//=============================================================================
template <bool A_GREATER, typename T, T A, T B>
struct _MAX {
  static constexpr const T value = A;
};

template <typename T, T A, T B>
struct _MAX<false, T, A, B> {
  static constexpr const T value = B;
};

template <typename T, T A, T B>
struct MAX : public _MAX<(A > B), T, A, B> { };


template<typename CTag>
class CSetOfTags {
protected:
   typedef unsigned char CBasicType;
   static constexpr const unsigned long BASIC_SIZE = sizeof(CBasicType)*8;
   CBasicType m_code[MAX<size_t, CTag::MAX_COUNT/BASIC_SIZE + 1, 8>::value];

public:
   CSetOfTags() {
      memset(m_code, 0, sizeof(m_code));
//      for (unsigned i=0; i<sizeof(m_code)/sizeof(CBasicType); ++i)
//         ASSERT(m_code[i]==0, "Initialize");
   }
   static unsigned long size() { return CTag::MAX_COUNT; }

public:
   void add(const CTag &tag) { 
      const unsigned long &bit = tag.code();
      m_code[bit/BASIC_SIZE] |= (static_cast<CBasicType>(1)<<(bit%BASIC_SIZE)); 
   }
   void remove(const CTag &tag) { 
      const unsigned long &bit = tag.code();
      m_code[bit/BASIC_SIZE] &= ~(static_cast<CBasicType>(1)<<(bit%BASIC_SIZE)); 
   }
   void clear() { memset(m_code, 0, sizeof(m_code)); }

public:
   bool contains(const CTag &tag) const { 
      const unsigned long &bit = tag.code();
      return m_code[bit/BASIC_SIZE] & (static_cast<CBasicType>(1)<<(bit%BASIC_SIZE)); 
   }

public:
//   const unsigned long long &code() const { return m_code; }
   unsigned long long hash() const {
     return *reinterpret_cast<const unsigned long long *>(m_code);
   }

public:
   bool operator == (const CSetOfTags &s) const { return memcmp(m_code, s.m_code, sizeof(m_code))==0; }

   template <typename T0>
   friend std::istream &operator >>(std::istream &, CSetOfTags<T0> &s);
   template <typename T0>
   friend std::ostream &operator <<(std::ostream &, const CSetOfTags<T0> &s);
};


//==============================================================================
template<typename CTag>
inline std::istream &
operator >>(std::istream &is, CSetOfTags<CTag> &c) {
  const uint32_t nitems = mp::read_array_size(is);
  assert(nitems == sizeof(c.m_code)); (void)nitems;
  for (uint32_t i = 0; i != nitems; ++i)
    c.m_code[i] = static_cast<typename CSetOfTags<CTag>::CBasicType>(mp::read_uint8(is));
  return is;
}

template<typename CTag>
inline std::ostream &
operator <<(std::ostream &os, const CSetOfTags<CTag> &c) {
  mp::write_array_size(os, sizeof(c.m_code));
  for (uint32_t i = 0; i != sizeof(c.m_code); ++i)
    mp::write_uint8(os, c.m_code[i]);
  return os;
}

#endif
