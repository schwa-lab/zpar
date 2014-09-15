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

#include "depparser_weight_base.h"
#include <vector>
#include <map>
#include "hash_utils.h"

namespace TARGET_LANGUAGE {

namespace depparser {

const static unsigned DEP_TABLE_SIZE = 1000121;  // This should be a prime number.

//
// TYPE DEFINITIONS
//
typedef boost::variant<CWord> FeatureVariant;
/*
typedef boost::variant<int, CWord, CTag, CTaggedWord<CTag, TAG_SEPARATOR>, 
    CSetOfTags<CDependencyLabel>, CTwoWords, CTwoTaggedWords,
    CTuple2<CWord, CTag>, CTuple2<CWord, int>, CTuple2<CTag, int>,
    CTuple3<CWord, CTag, CTag>, CTuple3<CWord, CWord, CTag>, CTuple3<CWord, CWord, int>,
    CTuple3<CTag, CTag, int>, CTuple2<CWord, CSetOfTags<CDependencyLabel> >,
    CTuple2<CTag, CSetOfTags<CDependencyLabel> >, CTagSet<CTag, 2>, CTagSet<CTag, 3>,
    CLemma, CCoNLLCPOS> FeatureVariant;
*/
typedef CPackedScoreMap<FeatureVariant, SCORE_TYPE, action::MAX> WeightMap;
/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

auto feat_to_str = std::map<Feature, std::string>({
    { Feature::m_mapSTw, "StackWord" } /*,
    { Feature::m_mapSTt, "StackTag" },
    { Feature::m_mapSTwt, "StackWordTag" },
    { Feature::m_mapN0w, "NextWord" },
    { Feature::m_mapN0t, "NextTag" },
    { Feature::m_mapN0wt, "NextWordTag" },
    { Feature::m_mapN1w, "Next+1Word" },
    { Feature::m_mapN1t, "Next+1Tag" },
    { Feature::m_mapN1wt, "Next+1WordTag" },
    { Feature::m_mapN2w, "Next+2Word" },
    { Feature::m_mapN2t, "Next+2Tag" },
    { Feature::m_mapN2wt, "Next+2WordTag" },
    { Feature::m_mapSTHw, "StackHeadWord" },
    { Feature::m_mapSTHt, "StackHeadTag" },
    { Feature::m_mapSTi, "StackLabel" },
    { Feature::m_mapSTHHw, "StackHeadHeadWord" },
    { Feature::m_mapSTHHt, "StackHeadHeadTag" },
    { Feature::m_mapSTHi, "StackLabel" },
    { Feature::m_mapSTLDw, "StackLDWord" },
    { Feature::m_mapSTLDt, "StackLDTag" },
    { Feature::m_mapSTLDi, "StackLDLabel" },
    { Feature::m_mapSTRDw, "StackRDWord" },
    { Feature::m_mapSTRDt, "StackRDTag" },
    { Feature::m_mapSTRDi, "StackRDLabel" },
    { Feature::m_mapN0LDw, "NextLDWord" },
    { Feature::m_mapN0LDt, "NextLDTag" },
    { Feature::m_mapN0LDi, "NextLDLabel" },
    { Feature::m_mapSTL2Dw, "StackL2DWord" },
    { Feature::m_mapSTL2Dt, "StackL2DTag" },
    { Feature::m_mapSTL2Di, "StackL2DLabel" },
    { Feature::m_mapSTR2Dw, "StackR2DWord" },
    { Feature::m_mapSTR2Dt, "StackR2DTag" },
    { Feature::m_mapSTR2Di, "StackR2DLabel" },
    { Feature::m_mapN0L2Dw, "NextL2DWord" },
    { Feature::m_mapN0L2Dt, "NextL2DTag" },
    { Feature::m_mapN0L2Di, "NextL2DLabel" },
    { Feature::m_mapHTw, "HeadStackWord" },
    { Feature::m_mapHTt, "HeadStackTag" },
    { Feature::m_mapHTwt, "HeadStackWordTag" },
    { Feature::m_mapSTwtN0wt, "StackWordTagNextWordTag" },
    { Feature::m_mapSTwtN0w, "StackWordTagNextWord" },
    { Feature::m_mapSTwN0wt, "StackWordNextWordTag" },
    { Feature::m_mapSTtN0wt, "StackTagNextWordTag" },
    { Feature::m_mapSTwtN0t, "StackWordTagNextTag" },
    { Feature::m_mapSTwN0w, "StackWordNextWord" },
    { Feature::m_mapSTtN0t, "StackTagNextTag" },
    { Feature::m_mapN0tN1t, "NextTagNext+1Tag" },
    { Feature::m_mapN0tN1tN2t, "NextTagTrigram" },
    { Feature::m_mapSTtN0tN1t, "StackTagNextTagNext+1Tag" },
    { Feature::m_mapSTtN0tN0LDt, "StackTagNextTagNextLDTag" },
    { Feature::m_mapN0tN0LDtN0L2Dt, "StackTagNextTagNextLDTagNextTagNextL2DTag" },
    { Feature::m_mapSTHtSTtN0t, "StackHeadTagStackTagNextTag" },
    { Feature::m_mapHTtHT2tN0t, "HeadStackTagHeadStack2TagNextTag" },
    { Feature::m_mapSTHHtSTHtSTt, "StackHeadHeadTagStackHeadTagStackTag" },
    { Feature::m_mapSTtSTLDtN0t, "StackTagStackLDTagNextTag" },
    { Feature::m_mapSTtSTLDtSTL2Dt, "StackTagStackLDTagStackL2DTag" },
    { Feature::m_mapSTtSTRDtN0t, "StackTagStackRDTagNextTag" },
    { Feature::m_mapSTtSTRDtSTR2Dt, "StackTagStackRDTagStackR2DTag" },
    { Feature::m_mapSTwd, "StackWordDist" },
    { Feature::m_mapSTtd, "StackTagDist" },
    { Feature::m_mapN0wd, "NextWordDist" },
    { Feature::m_mapN0td, "NextTagDist" },
    { Feature::m_mapSTwN0wd, "StackWordNextWordDist" },
    { Feature::m_mapSTtN0td, "StackTagNextTagDist" },
    { Feature::m_mapSTwra, "StackWordRightArity" },
    { Feature::m_mapSTtra, "StackTagRightArity" },
    { Feature::m_mapSTwla, "StackWordLeftArity" },
    { Feature::m_mapSTtla, "StackTagLeftArity" },
    { Feature::m_mapN0wla, "NextWordRightArity" },
    { Feature::m_mapN0tla, "NextTagRightArity" },
    { Feature::m_mapSTwrp, "StackWordRightSetoftags" },
    { Feature::m_mapSTtrp, "StackTagRightSetoftags" },
    { Feature::m_mapSTwlp, "StackWordLeftSetoftags" },
    { Feature::m_mapSTtlp, "StackTagLeftSetoftags" },
    { Feature::m_mapN0wlp, "Next0WordLeftSetoftags" },
    { Feature::m_mapN0tlp, "Next0TagLeftSetoftags" },
    { Feature::m_mapSTl, "StackLemma" },
    { Feature::m_mapSTc, "StackCPOS" },
    { Feature::m_mapSTf, "StackFeats" },
    { Feature::m_mapN0l, "NextLemma" },
    { Feature::m_mapN0c, "NextCPOS" },
    { Feature::m_mapN0f, "NextFeats" },
    { Feature::m_mapN1l, "Next+1Lemma" },
    { Feature::m_mapN1c, "Next+1CPOS" },
    { Feature::m_mapN1f, "Next+1Feats" }*/
});

class CWeight : public CWeightBase {

public:
   WeightMap weight_map;

public:

   CWeight(const std::string &sPath, bool bTrain) : CWeightBase(sPath, bTrain) , 
                                                    weight_map("Feature", DEP_TABLE_SIZE)
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
/*std::cout << "Feature::m_mapSTw "; m_mapSTw.debugUsage();
std::cout << "Feature::m_mapSTt "; m_mapSTt.debugUsage();
std::cout << "Feature::m_mapSTwt "; m_mapSTwt.debugUsage();
std::cout << "Feature::m_mapN0w "; m_mapN0w.debugUsage();
std::cout << "Feature::m_mapN0t "; m_mapN0t.debugUsage();
std::cout << "Feature::m_mapN0wt "; m_mapN0wt.debugUsage();
std::cout << "Feature::m_mapN1w "; m_mapN1w.debugUsage();
std::cout << "Feature::m_mapN1t "; m_mapN1t.debugUsage();
std::cout << "Feature::m_mapN1wt "; m_mapN1wt.debugUsage();
std::cout << "Feature::m_mapN2w "; m_mapN2w.debugUsage();
std::cout << "Feature::m_mapN2t "; m_mapN2t.debugUsage();
std::cout << "Feature::m_mapN2wt "; m_mapN2wt.debugUsage();
std::cout << "Feature::m_mapSTHw "; m_mapSTHw.debugUsage();
std::cout << "Feature::m_mapSTHt "; m_mapSTHt.debugUsage();
std::cout << "Feature::m_mapSTi "; m_mapSTi.debugUsage();
std::cout << "Feature::m_mapSTHHw "; m_mapSTHHw.debugUsage();
std::cout << "Feature::m_mapSTHHt "; m_mapSTHHt.debugUsage();
std::cout << "Feature::m_mapSTHi "; m_mapSTHi.debugUsage();
std::cout << "Feature::m_mapSTLDw "; m_mapSTLDw.debugUsage();
std::cout << "Feature::m_mapSTLDt "; m_mapSTLDt.debugUsage();
std::cout << "Feature::m_mapSTLDi "; m_mapSTLDi.debugUsage();
std::cout << "Feature::m_mapSTRDw "; m_mapSTRDw.debugUsage();
std::cout << "Feature::m_mapSTRDt "; m_mapSTRDt.debugUsage();
std::cout << "Feature::m_mapSTRDi "; m_mapSTRDi.debugUsage();
std::cout << "Feature::m_mapN0LDw "; m_mapN0LDw.debugUsage();
std::cout << "Feature::m_mapN0LDt "; m_mapN0LDt.debugUsage();
std::cout << "Feature::m_mapN0LDi "; m_mapN0LDi.debugUsage();
std::cout << "Feature::m_mapSTL2Dw "; m_mapSTL2Dw.debugUsage();
std::cout << "Feature::m_mapSTL2Dt "; m_mapSTL2Dt.debugUsage();
std::cout << "Feature::m_mapSTL2Di "; m_mapSTL2Di.debugUsage();
std::cout << "Feature::m_mapSTR2Dw "; m_mapSTR2Dw.debugUsage();
std::cout << "Feature::m_mapSTR2Dt "; m_mapSTR2Dt.debugUsage();
std::cout << "Feature::m_mapSTR2Di "; m_mapSTR2Di.debugUsage();
std::cout << "Feature::m_mapN0L2Dw "; m_mapN0L2Dw.debugUsage();
std::cout << "Feature::m_mapN0L2Dt "; m_mapN0L2Dt.debugUsage();
std::cout << "Feature::m_mapN0L2Di "; m_mapN0L2Di.debugUsage();
std::cout << "Feature::m_mapHTw "; m_mapHTw.debugUsage();
std::cout << "Feature::m_mapHTt "; m_mapHTt.debugUsage();
std::cout << "Feature::m_mapHTwt "; m_mapHTwt.debugUsage();
std::cout << "Feature::m_mapSTwtN0wt "; m_mapSTwtN0wt.debugUsage();
std::cout << "Feature::m_mapSTwtN0w "; m_mapSTwtN0w.debugUsage();
std::cout << "Feature::m_mapSTwN0wt "; m_mapSTwN0wt.debugUsage();
std::cout << "Feature::m_mapSTtN0wt "; m_mapSTtN0wt.debugUsage();
std::cout << "Feature::m_mapSTwtN0t "; m_mapSTwtN0t.debugUsage();
std::cout << "Feature::m_mapSTwN0w "; m_mapSTwN0w.debugUsage();
std::cout << "Feature::m_mapSTtN0t "; m_mapSTtN0t.debugUsage();
std::cout << "Feature::m_mapN0tN1t "; m_mapN0tN1t.debugUsage();
std::cout << "Feature::m_mapN0tN1tN2t "; m_mapN0tN1tN2t.debugUsage();
std::cout << "Feature::m_mapSTtN0tN1t "; m_mapSTtN0tN1t.debugUsage();
std::cout << "Feature::m_mapSTtN0tN0LDt "; m_mapSTtN0tN0LDt.debugUsage();
std::cout << "Feature::m_mapN0tN0LDtN0L2Dt "; m_mapN0tN0LDtN0L2Dt.debugUsage();
std::cout << "Feature::m_mapSTHtSTtN0t "; m_mapSTHtSTtN0t.debugUsage();
std::cout << "Feature::m_mapHTtHT2tN0t "; m_mapHTtHT2tN0t.debugUsage();
std::cout << "Feature::m_mapSTHHtSTHtSTt "; m_mapSTHHtSTHtSTt.debugUsage();
std::cout << "Feature::m_mapSTtSTLDtN0t "; m_mapSTtSTLDtN0t.debugUsage();
std::cout << "Feature::m_mapSTtSTLDtSTL2Dt "; m_mapSTtSTLDtSTL2Dt.debugUsage();
std::cout << "Feature::m_mapSTtSTRDtN0t "; m_mapSTtSTRDtN0t.debugUsage();
std::cout << "Feature::m_mapSTtSTRDtSTR2Dt "; m_mapSTtSTRDtSTR2Dt.debugUsage();
std::cout << "Feature::m_mapSTwd "; m_mapSTwd.debugUsage();
std::cout << "Feature::m_mapSTtd "; m_mapSTtd.debugUsage();
std::cout << "Feature::m_mapN0wd "; m_mapN0wd.debugUsage();
std::cout << "Feature::m_mapN0td "; m_mapN0td.debugUsage();
std::cout << "Feature::m_mapSTwN0wd "; m_mapSTwN0wd.debugUsage();
std::cout << "Feature::m_mapSTtN0td "; m_mapSTtN0td.debugUsage();
std::cout << "Feature::m_mapSTwra "; m_mapSTwra.debugUsage();
std::cout << "Feature::m_mapSTtra "; m_mapSTtra.debugUsage();
std::cout << "Feature::m_mapSTwla "; m_mapSTwla.debugUsage();
std::cout << "Feature::m_mapSTtla "; m_mapSTtla.debugUsage();
std::cout << "Feature::m_mapN0wla "; m_mapN0wla.debugUsage();
std::cout << "Feature::m_mapN0tla "; m_mapN0tla.debugUsage();
std::cout << "Feature::m_mapSTwrp "; m_mapSTwrp.debugUsage();
std::cout << "Feature::m_mapSTtrp "; m_mapSTtrp.debugUsage();
std::cout << "Feature::m_mapSTwlp "; m_mapSTwlp.debugUsage();
std::cout << "Feature::m_mapSTtlp "; m_mapSTtlp.debugUsage();
std::cout << "Feature::m_mapN0wlp "; m_mapN0wlp.debugUsage();
std::cout << "Feature::m_mapN0tlp "; m_mapN0tlp.debugUsage();
std::cout << "Feature::m_mapSTl "; m_mapSTl.debugUsage();
std::cout << "Feature::m_mapSTc "; m_mapSTc.debugUsage();
std::cout << "Feature::m_mapSTf "; m_mapSTf.debugUsage();
std::cout << "Feature::m_mapN0l "; m_mapN0l.debugUsage();
std::cout << "Feature::m_mapN0c "; m_mapN0c.debugUsage();
std::cout << "Feature::m_mapN0f "; m_mapN0f.debugUsage();
std::cout << "Feature::m_mapN1l "; m_mapN1l.debugUsage();
std::cout << "Feature::m_mapN1c "; m_mapN1c.debugUsage();
std::cout << "Feature::m_mapN1f "; m_mapN1f.debugUsage();
*/    }
};

}
}

#endif
