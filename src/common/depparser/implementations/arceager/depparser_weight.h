// Copyright (C) University of Oxford 2010
/***************************************************************
 *
 * weight.h - the dependency parser weights 
 *
 * Yue Zhang, 2007.8 - 2008.1
 *
 ***************************************************************/

#ifndef _DEPPARSER_WEIGHTS_H
#define _DEPPARSER_WEIGHTS_H


//#include "english/pos/penn.h"
//#include "english/tags.h"

//#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS 
//#define BOOST_MPL_LIMIT_LIST_SIZE 30 
//#define BOOST_MPL_LIMIT_VECTOR_SIZE 30
//#include "boost/variant.hpp"
#include "depparser_weight_base.h"
#include <vector>
#include <unordered_map>

namespace TARGET_LANGUAGE {

namespace depparser {

const static unsigned DEP_TABLE_SIZE = 1000121;  // This should be a prime number.

//
// TYPE DEFINITIONS
//
//typedef boost::variant<CWord, CTag> FeatureVariant;
typedef boost::variant<int, CWord, CTag, CTaggedWord<CTag, TAG_SEPARATOR>, 
    CSetOfTags<CDependencyLabel>, CTwoWords, CTwoTaggedWords,
    CTuple2<CWord, CTag>, CTuple2<CWord, int>, CTuple2<CTag, int>,
    CTuple3<CWord, CTag, CTag>, CTuple3<CWord, CWord, CTag>, CTuple3<CWord, CWord, int>,
    CTuple3<CTag, CTag, int>, CTuple2<CWord, CSetOfTags<CDependencyLabel> >,
    CTuple2<CTag, CSetOfTags<CDependencyLabel> >, CTagSet<CTag, 2>, CTagSet<CTag, 3>,
    CLemma, CCoNLLCPOS> FeatureVariant;
typedef CPackedScoreMap<FeatureVariant, SCORE_TYPE, action::MAX> WeightMap;
/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

const auto feat_to_str = std::unordered_map<Feature, std:string>({
    { m_mapSTw, "StackWord" },
    { m_mapSTt, "StackTag" },
    { m_mapSTwt, "StackWordTag" },
    { m_mapN0w, "NextWord" },
    { m_mapN0t, "NextTag" },
    { m_mapN0wt, "NextWordTag" },
    { m_mapN1w, "Next+1Word" },
    { m_mapN1t, "Next+1Tag" },
    { m_mapN1wt, "Next+1WordTag" },
    { m_mapN2w, "Next+2Word" },
    { m_mapN2t, "Next+2Tag" },
    { m_mapN2wt, "Next+2WordTag" },
    { m_mapSTHw, "StackHeadWord" },
    { m_mapSTHt, "StackHeadTag" },
    { m_mapSTi, "StackLabel" },
    { m_mapSTHHw, "StackHeadHeadWord" },
    { m_mapSTHHt, "StackHeadHeadTag" },
    { m_mapSTHi, "StackLabel" },
    { m_mapSTLDw, "StackLDWord" },
    { m_mapSTLDt, "StackLDTag" },
    { m_mapSTLDi, "StackLDLabel" },
    { m_mapSTRDw, "StackRDWord" },
    { m_mapSTRDt, "StackRDTag" },
    { m_mapSTRDi, "StackRDLabel" },
    { m_mapN0LDw, "NextLDWord" },
    { m_mapN0LDt, "NextLDTag" },
    { m_mapN0LDi, "NextLDLabel" },
    { m_mapSTL2Dw, "StackL2DWord" },
    { m_mapSTL2Dt, "StackL2DTag" },
    { m_mapSTL2Di, "StackL2DLabel" },
    { m_mapSTR2Dw, "StackR2DWord" },
    { m_mapSTR2Dt, "StackR2DTag" },
    { m_mapSTR2Di, "StackR2DLabel" },
    { m_mapN0L2Dw, "NextL2DWord" },
    { m_mapN0L2Dt, "NextL2DTag" },
    { m_mapN0L2Di, "NextL2DLabel" },
    { m_mapHTw, "HeadStackWord" },
    { m_mapHTt, "HeadStackTag" },
    { m_mapHTwt, "HeadStackWordTag" },
    { m_mapSTwtN0wt, "StackWordTagNextWordTag" },
    { m_mapSTwtN0w, "StackWordTagNextWord" },
    { m_mapSTwN0wt, "StackWordNextWordTag" },
    { m_mapSTtN0wt, "StackTagNextWordTag" },
    { m_mapSTwtN0t, "StackWordTagNextTag" },
    { m_mapSTwN0w, "StackWordNextWord" },
    { m_mapSTtN0t, "StackTagNextTag" },
    { m_mapN0tN1t, "NextTagNext+1Tag" },
    { m_mapN0tN1tN2t, "NextTagTrigram" },
    { m_mapSTtN0tN1t, "StackTagNextTagNext+1Tag" },
    { m_mapSTtN0tN0LDt, "StackTagNextTagNextLDTag" },
    { m_mapN0tN0LDtN0L2Dt, "StackTagNextTagNextLDTagNextTagNextL2DTag" },
    { m_mapSTHtSTtN0t, "StackHeadTagStackTagNextTag" },
    { m_mapHTtHT2tN0t, "HeadStackTagHeadStack2TagNextTag" },
    { m_mapSTHHtSTHtSTt, "StackHeadHeadTagStackHeadTagStackTag" },
    { m_mapSTtSTLDtN0t, "StackTagStackLDTagNextTag" },
    { m_mapSTtSTLDtSTL2Dt, "StackTagStackLDTagStackL2DTag" },
    { m_mapSTtSTRDtN0t, "StackTagStackRDTagNextTag" },
    { m_mapSTtSTRDtSTR2Dt, "StackTagStackRDTagStackR2DTag" },
    { m_mapSTwd, "StackWordDist" },
    { m_mapSTtd, "StackTagDist" },
    { m_mapN0wd, "NextWordDist" },
    { m_mapN0td, "NextTagDist" },
    { m_mapSTwN0wd, "StackWordNextWordDist" },
    { m_mapSTtN0td, "StackTagNextTagDist" },
    { m_mapSTwra, "StackWordRightArity" },
    { m_mapSTtra, "StackTagRightArity" },
    { m_mapSTwla, "StackWordLeftArity" },
    { m_mapSTtla, "StackTagLeftArity" },
    { m_mapN0wla, "NextWordRightArity" },
    { m_mapN0tla, "NextTagRightArity" },
    { m_mapSTwrp, "StackWordRightSetoftags" },
    { m_mapSTtrp, "StackTagRightSetoftags" },
    { m_mapSTwlp, "StackWordLeftSetoftags" },
    { m_mapSTtlp, "StackTagLeftSetoftags" },
    { m_mapN0wlp, "Next0WordLeftSetoftags" },
    { m_mapN0tlp, "Next0TagLeftSetoftags" },
    { m_mapSTl, "StackLemma" },
    { m_mapSTc, "StackCPOS" },
    { m_mapSTf, "StackFeats" },
    { m_mapN0l, "NextLemma" },
    { m_mapN0c, "NextCPOS" },
    { m_mapN0f, "NextFeats" },
    { m_mapN1l, "Next+1Lemma" },
    { m_mapN1c, "Next+1CPOS" },
    { m_mapN1f, "Next+1Feats" }
};

class CWeight : public CWeightBase {

public:
   WeightMap weight_map;

public:

   CWeight(const std::string &sPath, bool bTrain) : CWeightBase(sPath, bTrain) , 
                                                    weight_map("StackWord", DEP_TABLE_SIZE)
   { loadScores(); }

   virtual ~CWeight() {
//      CPackedScore<SCORE_TYPE, action::MAX>::freePoolMemory();
   }

   // MEHTODS
   virtual void loadScores();
   virtual void saveScores();
   void computeAverageFeatureWeights(int round);
   SCORE_TYPE dotProduct(const CWeight &w);
 
    void debugUsage(void) const {
/*std::cout << "m_mapSTw "; m_mapSTw.debugUsage();
std::cout << "m_mapSTt "; m_mapSTt.debugUsage();
std::cout << "m_mapSTwt "; m_mapSTwt.debugUsage();
std::cout << "m_mapN0w "; m_mapN0w.debugUsage();
std::cout << "m_mapN0t "; m_mapN0t.debugUsage();
std::cout << "m_mapN0wt "; m_mapN0wt.debugUsage();
std::cout << "m_mapN1w "; m_mapN1w.debugUsage();
std::cout << "m_mapN1t "; m_mapN1t.debugUsage();
std::cout << "m_mapN1wt "; m_mapN1wt.debugUsage();
std::cout << "m_mapN2w "; m_mapN2w.debugUsage();
std::cout << "m_mapN2t "; m_mapN2t.debugUsage();
std::cout << "m_mapN2wt "; m_mapN2wt.debugUsage();
std::cout << "m_mapSTHw "; m_mapSTHw.debugUsage();
std::cout << "m_mapSTHt "; m_mapSTHt.debugUsage();
std::cout << "m_mapSTi "; m_mapSTi.debugUsage();
std::cout << "m_mapSTHHw "; m_mapSTHHw.debugUsage();
std::cout << "m_mapSTHHt "; m_mapSTHHt.debugUsage();
std::cout << "m_mapSTHi "; m_mapSTHi.debugUsage();
std::cout << "m_mapSTLDw "; m_mapSTLDw.debugUsage();
std::cout << "m_mapSTLDt "; m_mapSTLDt.debugUsage();
std::cout << "m_mapSTLDi "; m_mapSTLDi.debugUsage();
std::cout << "m_mapSTRDw "; m_mapSTRDw.debugUsage();
std::cout << "m_mapSTRDt "; m_mapSTRDt.debugUsage();
std::cout << "m_mapSTRDi "; m_mapSTRDi.debugUsage();
std::cout << "m_mapN0LDw "; m_mapN0LDw.debugUsage();
std::cout << "m_mapN0LDt "; m_mapN0LDt.debugUsage();
std::cout << "m_mapN0LDi "; m_mapN0LDi.debugUsage();
std::cout << "m_mapSTL2Dw "; m_mapSTL2Dw.debugUsage();
std::cout << "m_mapSTL2Dt "; m_mapSTL2Dt.debugUsage();
std::cout << "m_mapSTL2Di "; m_mapSTL2Di.debugUsage();
std::cout << "m_mapSTR2Dw "; m_mapSTR2Dw.debugUsage();
std::cout << "m_mapSTR2Dt "; m_mapSTR2Dt.debugUsage();
std::cout << "m_mapSTR2Di "; m_mapSTR2Di.debugUsage();
std::cout << "m_mapN0L2Dw "; m_mapN0L2Dw.debugUsage();
std::cout << "m_mapN0L2Dt "; m_mapN0L2Dt.debugUsage();
std::cout << "m_mapN0L2Di "; m_mapN0L2Di.debugUsage();
std::cout << "m_mapHTw "; m_mapHTw.debugUsage();
std::cout << "m_mapHTt "; m_mapHTt.debugUsage();
std::cout << "m_mapHTwt "; m_mapHTwt.debugUsage();
std::cout << "m_mapSTwtN0wt "; m_mapSTwtN0wt.debugUsage();
std::cout << "m_mapSTwtN0w "; m_mapSTwtN0w.debugUsage();
std::cout << "m_mapSTwN0wt "; m_mapSTwN0wt.debugUsage();
std::cout << "m_mapSTtN0wt "; m_mapSTtN0wt.debugUsage();
std::cout << "m_mapSTwtN0t "; m_mapSTwtN0t.debugUsage();
std::cout << "m_mapSTwN0w "; m_mapSTwN0w.debugUsage();
std::cout << "m_mapSTtN0t "; m_mapSTtN0t.debugUsage();
std::cout << "m_mapN0tN1t "; m_mapN0tN1t.debugUsage();
std::cout << "m_mapN0tN1tN2t "; m_mapN0tN1tN2t.debugUsage();
std::cout << "m_mapSTtN0tN1t "; m_mapSTtN0tN1t.debugUsage();
std::cout << "m_mapSTtN0tN0LDt "; m_mapSTtN0tN0LDt.debugUsage();
std::cout << "m_mapN0tN0LDtN0L2Dt "; m_mapN0tN0LDtN0L2Dt.debugUsage();
std::cout << "m_mapSTHtSTtN0t "; m_mapSTHtSTtN0t.debugUsage();
std::cout << "m_mapHTtHT2tN0t "; m_mapHTtHT2tN0t.debugUsage();
std::cout << "m_mapSTHHtSTHtSTt "; m_mapSTHHtSTHtSTt.debugUsage();
std::cout << "m_mapSTtSTLDtN0t "; m_mapSTtSTLDtN0t.debugUsage();
std::cout << "m_mapSTtSTLDtSTL2Dt "; m_mapSTtSTLDtSTL2Dt.debugUsage();
std::cout << "m_mapSTtSTRDtN0t "; m_mapSTtSTRDtN0t.debugUsage();
std::cout << "m_mapSTtSTRDtSTR2Dt "; m_mapSTtSTRDtSTR2Dt.debugUsage();
std::cout << "m_mapSTwd "; m_mapSTwd.debugUsage();
std::cout << "m_mapSTtd "; m_mapSTtd.debugUsage();
std::cout << "m_mapN0wd "; m_mapN0wd.debugUsage();
std::cout << "m_mapN0td "; m_mapN0td.debugUsage();
std::cout << "m_mapSTwN0wd "; m_mapSTwN0wd.debugUsage();
std::cout << "m_mapSTtN0td "; m_mapSTtN0td.debugUsage();
std::cout << "m_mapSTwra "; m_mapSTwra.debugUsage();
std::cout << "m_mapSTtra "; m_mapSTtra.debugUsage();
std::cout << "m_mapSTwla "; m_mapSTwla.debugUsage();
std::cout << "m_mapSTtla "; m_mapSTtla.debugUsage();
std::cout << "m_mapN0wla "; m_mapN0wla.debugUsage();
std::cout << "m_mapN0tla "; m_mapN0tla.debugUsage();
std::cout << "m_mapSTwrp "; m_mapSTwrp.debugUsage();
std::cout << "m_mapSTtrp "; m_mapSTtrp.debugUsage();
std::cout << "m_mapSTwlp "; m_mapSTwlp.debugUsage();
std::cout << "m_mapSTtlp "; m_mapSTtlp.debugUsage();
std::cout << "m_mapN0wlp "; m_mapN0wlp.debugUsage();
std::cout << "m_mapN0tlp "; m_mapN0tlp.debugUsage();
std::cout << "m_mapSTl "; m_mapSTl.debugUsage();
std::cout << "m_mapSTc "; m_mapSTc.debugUsage();
std::cout << "m_mapSTf "; m_mapSTf.debugUsage();
std::cout << "m_mapN0l "; m_mapN0l.debugUsage();
std::cout << "m_mapN0c "; m_mapN0c.debugUsage();
std::cout << "m_mapN0f "; m_mapN0f.debugUsage();
std::cout << "m_mapN1l "; m_mapN1l.debugUsage();
std::cout << "m_mapN1c "; m_mapN1c.debugUsage();
std::cout << "m_mapN1f "; m_mapN1f.debugUsage();
*/    }
};

}
}

#endif
