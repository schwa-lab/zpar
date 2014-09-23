// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * stanford.h - the Stanford Dependency style dependency labels *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _DEPENDENCY_L_STANFORD
#define _DEPENDENCY_L_STANFORD

#include "tags.h"

namespace english {

   /*
    * missing:
    * npadvmod
    * discourse
    * agent
    * goeswith
    * root
    * vmod
    *
    */
const std::string STANFORD_DEP_STRINGS[] = {
   "-none-",
   "root", /* used to be null */
   "cc", 
   "number", 
   "ccomp", 
   "possessive", 
   "prt", 
   "num", 
   "nsubjpass", 
   "csubj", 
   "measure", 
   "conj", 
   "amod", 
   "pred", 
   "nn", 
   "neg", 
   "csubjpass", 
   "mark", 
   "auxpass", 
   "infmod", 
   "rel", 
   "advcl", 
   "aux", 
   "prep", 
   "parataxis", 
   "nsubj", 
   "rcmod", 
   "advmod", 
   "npadvmod",
   "punct", 
   "quantmod", 
   "tmod", 
   "acomp", 
   "complm", 
   "pcomp", 
   "poss", 
   "xcomp", 
   "cop", 
   "attr", 
   "partmod", 
   "dep", 
   "appos", 
   "det", 
   "dobj", 
   "purpcl", 
   "abbrev", 
   "pobj", 
   "iobj", 
   "expl", 
   "predet", 
   "preconj"
};

enum STANFORD_DEP_LABELS {
   STANFORD_DEP_NONE=0,
   STANFORD_DEP_ROOT,
   STANFORD_DEP_CC, 
   STANFORD_DEP_NUMBER, 
   STANFORD_DEP_CCOMP, 
   STANFORD_DEP_POSSESSIVE, 
   STANFORD_DEP_PRT, 
   STANFORD_DEP_NUM, 
   STANFORD_DEP_NSUBJPASS, 
   STANFORD_DEP_CSUBJ, 
   STANFORD_DEP_MEASURE, 
   STANFORD_DEP_CONJ, 
   STANFORD_DEP_AMOD, 
   STANFORD_DEP_PRED, 
   STANFORD_DEP_NN, 
   STANFORD_DEP_NEG, 
   STANFORD_DEP_CSUBJPASS, 
   STANFORD_DEP_MARK, 
   STANFORD_DEP_AUXPASS, 
   STANFORD_DEP_INFMOD, 
   STANFORD_DEP_REL, 
   STANFORD_DEP_ADVCL, 
   STANFORD_DEP_AUX, 
   STANFORD_DEP_PREP, 
   STANFORD_DEP_PARATAXIS, 
   STANFORD_DEP_NSUBJ, 
   STANFORD_DEP_RCMOD, 
   STANFORD_DEP_ADVMOD, 
   STANFORD_DEP_NPADVMOD, 
   STANFORD_DEP_PUNCT, 
   STANFORD_DEP_QUANTMOD, 
   STANFORD_DEP_TMOD, 
   STANFORD_DEP_ACOMP, 
   STANFORD_DEP_COMPLM, 
   STANFORD_DEP_PCOMP, 
   STANFORD_DEP_POSS, 
   STANFORD_DEP_XCOMP, 
   STANFORD_DEP_COP, 
   STANFORD_DEP_ATTR, 
   STANFORD_DEP_PARTMOD, 
   STANFORD_DEP_DEP, 
   STANFORD_DEP_APPOS, 
   STANFORD_DEP_DET, 
   STANFORD_DEP_DOBJ, 
   STANFORD_DEP_PURPCL, 
   STANFORD_DEP_ABBREV, 
   STANFORD_DEP_POBJ, 
   STANFORD_DEP_IOBJ, 
   STANFORD_DEP_EXPL, 
   STANFORD_DEP_PREDET, 
   STANFORD_DEP_PRECONJ,
   STANFORD_DEP_COUNT 
};

const unsigned long STANFORD_DEP_COUNT_BITS = 6;

/*==============================================================
 *
 * dependency lab
 *
 *==============================================================*/

class CDependencyLabel {
public:
   enum {NONE=0};
   enum {ROOT=1};
   enum {FIRST=1};
   enum {COUNT=STANFORD_DEP_COUNT};
   enum {MAX_COUNT=COUNT};
   enum {SIZE=STANFORD_DEP_COUNT_BITS};

protected:
   unsigned long m_code;

public:
   CDependencyLabel() : m_code(NONE) {}
   CDependencyLabel(const unsigned long &code) : m_code(code) { }
   explicit CDependencyLabel(const std::string &str) { load(str); }
   ~CDependencyLabel() {}

public:
   inline const unsigned long &code() const { return m_code; }
   inline unsigned long hash() const { return m_code; }
   inline const std::string &str() const { return STANFORD_DEP_STRINGS[m_code]; }

   inline bool operator == (const CDependencyLabel &l) const { return m_code == l.m_code; }
   inline bool operator != (const CDependencyLabel &l) const { return m_code != l.m_code; }
   inline bool operator < (const CDependencyLabel &l) const { return m_code < l.m_code; }
   inline bool operator > (const CDependencyLabel &l) const { return m_code > l.m_code; }
   inline bool operator <= (const CDependencyLabel &l) const { return m_code <= l.m_code; }
   inline bool operator >= (const CDependencyLabel &l) const { return m_code >= l.m_code; }

   inline void load(const unsigned long &u) { m_code = u; }
   void load(const std::string &str) { 
      m_code = STANFORD_DEP_NONE;
      for (int i=FIRST; i<COUNT; ++i) {
         if (STANFORD_DEP_STRINGS[i] == str) {
            m_code = i;
            return;
         }
      }
   }
};


inline std::istream &
operator >>(std::istream &is, CDependencyLabel &label) {
  label.load(mp::read_str(is));
  return is;
}


inline std::ostream &
operator <<(std::ostream &os, const CDependencyLabel &label) {
  mp::write_str(os, label.str());
  return os;
}

}

#endif
