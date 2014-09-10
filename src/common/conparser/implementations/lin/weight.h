// Copyright (C) University of Oxford 2010
/***************************************************************
 *
 * weight.h - the constituent parser weights 
 *
 * Yue Zhang, 2007.8 - 2008.1
 *
 ***************************************************************/

#ifndef _CONPARSER_WEIGHTS_H
#define _CONPARSER_WEIGHTS_H

#include "weight_base.h"

//#define TABLE_SIZE 122651
const static unsigned TABLE_SIZE = 1<<17;

#define iterate_templates(left, right) \
   left(m_mapS0c)right\
   left(m_mapS0w)right\
   left(m_mapS0tc)right\
   left(m_mapS0wc)right\
   left(m_mapS0k)right\
   left(m_mapS0n)right\
\
   left(m_mapS1c)right\
   left(m_mapS1w)right\
   left(m_mapS1tc)right\
   left(m_mapS1wc)right\
   left(m_mapS1k)right\
   left(m_mapS1n)right\
\
   left(m_mapS2tc)right\
   left(m_mapS2wc)right\
   left(m_mapS2k)right\
   left(m_mapS2n)right\
\
   left(m_mapS3tc)right\
   left(m_mapS3wc)right\
   left(m_mapS3k)right\
   left(m_mapS3n)right\
\
   left(m_mapN0w)right\
   left(m_mapN0t)right\
   left(m_mapN0wt)right\
   left(m_mapN0k)right\
   left(m_mapN0n)right\
\
   left(m_mapN1w)right\
   left(m_mapN1t)right\
   left(m_mapN1wt)right\
   left(m_mapN1k)right\
   left(m_mapN1n)right\
\
   left(m_mapN2w)right\
   left(m_mapN2t)right\
   left(m_mapN2wt)right\
   left(m_mapN2k)right\
   left(m_mapN2n)right\
\
   left(m_mapN3w)right\
   left(m_mapN3t)right\
   left(m_mapN3wt)right\
   left(m_mapN3k)right\
   left(m_mapN3n)right\
\
   left(m_mapS0Lwc)right\
   left(m_mapS0Ltc)right\
   left(m_mapS0Rwc)right\
   left(m_mapS0Rtc)right\
   left(m_mapS0Uwc)right\
   left(m_mapS0Utc)right\
   left(m_mapS1Lwc)right\
   left(m_mapS1Ltc)right\
   left(m_mapS1Rwc)right\
   left(m_mapS1Rtc)right\
   left(m_mapS1Uwc)right\
   left(m_mapS1Utc)right\
\
   left(m_mapS0wcS1wc)right\
   left(m_mapS0wS1c)right\
   left(m_mapS0cS1w)right\
   left(m_mapS0cS1c)right\
   left(m_mapBetweenTags)right\
\
   left(m_mapS0wN0w)right\
   left(m_mapS0cN0w)right\
   left(m_mapS0wN0t)right\
   left(m_mapS0cN0t)right\
   left(m_mapS0cmN0tm)right\
\
   left(m_mapS1wN0w)right\
   left(m_mapS1cN0w)right\
   left(m_mapS1wN0t)right\
   left(m_mapS1cN0t)right\
\
   left(m_mapN0wN1w)right\
   left(m_mapN0tN1w)right\
   left(m_mapN0wN1t)right\
   left(m_mapN0tN1t)right\
\
   left(m_mapS0wS1cN0t)right\
   left(m_mapS0cS1wN0t)right\
   left(m_mapS0cS1cN0w)right\
   left(m_mapS0cS1cN0t)right\
   left(m_mapS0tS1tN0t)right\
   left(m_mapS0jS1jN0t)right\
\
   left(m_mapS0wN0tN1t)right\
   left(m_mapS0cN0wN1t)right\
   left(m_mapS0cN0tN1w)right\
   left(m_mapS0cN0tN1t)right\
   left(m_mapS0tN0tN1t)right\
   left(m_mapS0jN0tN1t)right\
\
   left(m_mapS0wS1cS2c)right\
   left(m_mapS0cS1wS2c)right\
   left(m_mapS0cS1cS2w)right\
   left(m_mapS0cS1cS2c)right\
   left(m_mapS0tS1tS2t)right\
\
   left(m_mapS0cS0LcN0t)right\
   left(m_mapS0wS0LcN0t)right\
   left(m_mapS0cS0LcN0w)right\
   left(m_mapS0cS0RcN0t)right\
   left(m_mapS0cS0RjN0t)right\
   left(m_mapS0wS0RcN0t)right\
   left(m_mapS0cS0RcN0w)right\
   left(m_mapS0cS0UcN0t)right\
   left(m_mapS0wS0UcN0t)right\
   left(m_mapS0cS0UcN0w)right\
\
   left(m_mapS0cS0LcS1c)right\
   left(m_mapS0cS0LjS1j)right\
   left(m_mapS0wS0LcS1c)right\
   left(m_mapS0cS0LcS1w)right\
   left(m_mapS0cS0RcS1c)right\
   left(m_mapS0wS0RcS1c)right\
   left(m_mapS0cS0RcS1w)right\
   left(m_mapS0cS0UcS1c)right\
   left(m_mapS0wS0UcS1c)right\
   left(m_mapS0cS0UcS1w)right\
\
   left(m_mapS0cS1cS1Lc)right\
   left(m_mapS0wS1cS1Lc)right\
   left(m_mapS0cS1wS1Lc)right\
   left(m_mapS0cS1cS1Rc)right\
   left(m_mapS0jS1cS1Rj)right\
   left(m_mapS0wS1cS1Rc)right\
   left(m_mapS0cS1wS1Rc)right\
   left(m_mapS0cS1cS1Uc)right\
   left(m_mapS0wS1cS1Uc)right\
   left(m_mapS0cS1wS1Uc)right


namespace TARGET_LANGUAGE {

namespace conparser {

//#include "conparser_macros.h"

//
// TYPE DEFINITIONS
//
typedef CScoreMapType<CWord, SCORE_TYPE, CAction::MAX> CWordMap;
typedef CScoreMapType<CTag, SCORE_TYPE, CAction::MAX> CTagMap;
typedef CScoreMapType<CTaggedWord<CTag, TAG_SEPARATOR>, SCORE_TYPE, CAction::MAX> CTaggedWordMap;
typedef CScoreMapType<std::pair<CWord, unsigned long>,  SCORE_TYPE, CAction::MAX> CWordIntMap;
typedef CScoreMapType<int, SCORE_TYPE, CAction::MAX> CIntMap;
typedef CScoreMapType<std::pair<unsigned long, unsigned long>, SCORE_TYPE, CAction::MAX> CTwoIntMap;
typedef CScoreMapType<CTwoWords, SCORE_TYPE, CAction::MAX> CTwoWordsMap;
typedef CScoreMapType<std::pair<CTwoWords, CActionType>, SCORE_TYPE, CAction::MAX> CTwoWordsActionTypeMap;
typedef CScoreMapType<CTuple3<CTwoWords, CTag, CActionType>, SCORE_TYPE, CAction::MAX> CTwoWordsTagActionTypeMap;
typedef CScoreMapType<CTuple2<CTwoWords, CCFGSet>, SCORE_TYPE, CAction::MAX> CTwoWordsCFGSetMap;
typedef CScoreMapType<CTwoTaggedWords, SCORE_TYPE, CAction::MAX> CTwoTaggedWordsMap;
typedef CScoreMapType<std::pair<CTwoTaggedWords, CActionType>, SCORE_TYPE, CAction::MAX> CTwoTaggedWordsActionTypeMap;
typedef CScoreMapType<CCFGSet, SCORE_TYPE, CAction::MAX> CCFGSetMap;
typedef CScoreMapType<std::pair<CCFGSet, unsigned long>, SCORE_TYPE, CAction::MAX> CCFGSetIntMap;
typedef CScoreMapType<CTuple2<CWord, CCFGSet>, SCORE_TYPE, CAction::MAX> CWordCFGSetMap;
typedef CScoreMapType<CTuple3<CWord, CCFGSet, CActionType>, SCORE_TYPE, CAction::MAX> CWordCFGSetActionTypeMap;
typedef CScoreMapType<std::pair<CWord, CCFGSet>, SCORE_TYPE, CAction::MAX> CWordCFGRuleMap;
typedef CScoreMapType<std::pair<CTwoWords, CCFGSet>, SCORE_TYPE, CAction::MAX> CTwoWordsCFGRuleMap;
typedef CScoreMapType<std::pair<CCFGSet, unsigned long>, SCORE_TYPE, CAction::MAX> CCFGSetIntMap;
typedef CScoreMapType<std::pair<CCFGSet, CActionType>, SCORE_TYPE, CAction::MAX> CCFGSetActionTypeMap;
typedef CScoreMapType<CTuple2<CWord, CConstituent>, SCORE_TYPE, CAction::MAX>
CWordConstituentMap;
typedef CScoreMapType<CTuple2<CTag, CConstituent>, SCORE_TYPE, CAction::MAX> CTagConstituentMap;
typedef CScoreMapType<CTuple3<CTag, CTag, unsigned long>, SCORE_TYPE, CAction::MAX> CWordTagIntMap;
typedef CScoreMapType<CConstituent, SCORE_TYPE, CAction::MAX> CConstituentMap;

typedef CHashMap<CWord, unsigned long> CWordToIntMap;

/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

class CWeight : public CWeightBase {

public:

   // S0
   CConstituentMap m_mapS0c;
   CTaggedWordMap m_mapS0w;
   CTagConstituentMap m_mapS0tc;
   CWordConstituentMap m_mapS0wc;
   CIntMap m_mapS0k;
   CIntMap m_mapS0n;

   // S1
   CConstituentMap m_mapS1c;
   CTaggedWordMap m_mapS1w;
   CTagConstituentMap m_mapS1tc;
   CWordConstituentMap m_mapS1wc;
   CIntMap m_mapS1k;
   CIntMap m_mapS1n;

   // S2
   CTagConstituentMap m_mapS2tc;
   CWordConstituentMap m_mapS2wc;
   CIntMap m_mapS2k;
   CIntMap m_mapS2n;

   // S3
   CTagConstituentMap m_mapS3tc;
   CWordConstituentMap m_mapS3wc;
   CIntMap m_mapS3k;
   CIntMap m_mapS3n;

   // N0
   CWordMap m_mapN0w;
   CTagMap m_mapN0t;
   CTaggedWordMap m_mapN0wt;
   CIntMap m_mapN0k;
   CIntMap m_mapN0n;

   // N1
   CWordMap m_mapN1w;
   CTagMap m_mapN1t;
   CTaggedWordMap m_mapN1wt;
   CIntMap m_mapN1k;
   CIntMap m_mapN1n;

   // N2
   CWordMap m_mapN2w;
   CTagMap m_mapN2t;
   CTaggedWordMap m_mapN2wt;
   CIntMap m_mapN2k;
   CIntMap m_mapN2n;

   // N3
   CWordMap m_mapN3w;
   CTagMap m_mapN3t;
   CTaggedWordMap m_mapN3wt;
   CIntMap m_mapN3k;
   CIntMap m_mapN3n;

   // S0L
   CWordConstituentMap m_mapS0Lwc;
   CTagConstituentMap m_mapS0Ltc;

   // S0R
   CWordConstituentMap m_mapS0Rwc;
   CTagConstituentMap m_mapS0Rtc;

   // S0U
   CWordConstituentMap m_mapS0Uwc;
   CTagConstituentMap m_mapS0Utc;

   // S1L
   CWordConstituentMap m_mapS1Lwc;
   CTagConstituentMap m_mapS1Ltc;

   // S1R
   CWordConstituentMap m_mapS1Rwc;
   CTagConstituentMap m_mapS1Rtc;

   // S1U
   CWordConstituentMap m_mapS1Uwc;
   CTagConstituentMap m_mapS1Utc;

   // S0 S1
   CTwoWordsCFGSetMap m_mapS0wcS1wc; // the original 
   CWordCFGSetMap m_mapS0wS1c;
   CWordCFGSetMap m_mapS0cS1w;
   CCFGSetMap m_mapS0cS1c;

   CCFGSetMap m_mapBetweenTags;

   // S0 N0
   CTwoWordsCFGSetMap m_mapS0wN0w;
   CWordCFGSetMap m_mapS0cN0w;
   CWordCFGSetMap m_mapS0wN0t;
   CCFGSetMap m_mapS0cN0t;
   CTwoIntMap m_mapS0cmN0tm;

   // S1 N0
   CTwoWordsCFGSetMap m_mapS1wN0w;
   CWordCFGSetMap m_mapS1cN0w;
   CWordCFGSetMap m_mapS1wN0t;
   CCFGSetMap m_mapS1cN0t;

   // N0 N1
   CTwoWordsCFGSetMap m_mapN0wN1w;
   CWordCFGSetMap m_mapN0tN1w;
   CWordCFGSetMap m_mapN0wN1t;
   CCFGSetMap m_mapN0tN1t;

   // S0 S1N0
   CWordCFGSetMap m_mapS0wS1cN0t;
   CWordCFGSetMap m_mapS0cS1wN0t;
   CWordCFGSetMap m_mapS0cS1cN0w;
   CCFGSetMap m_mapS0cS1cN0t;
   CCFGSetMap m_mapS0tS1tN0t;
   CCFGSetMap m_mapS0jS1jN0t;

   // S0 N0N1
   CWordCFGSetMap m_mapS0wN0tN1t;
   CWordCFGSetMap m_mapS0cN0wN1t;
   CWordCFGSetMap m_mapS0cN0tN1w;
   CCFGSetMap m_mapS0cN0tN1t;
   CCFGSetMap m_mapS0tN0tN1t;
   CCFGSetMap m_mapS0jN0tN1t;

   // S0 S1S2
   CWordCFGSetMap m_mapS0wS1cS2c;
   CWordCFGSetMap m_mapS0cS1wS2c;
   CWordCFGSetMap m_mapS0cS1cS2w;
   CCFGSetMap m_mapS0cS1cS2c;
   CCFGSetMap m_mapS0tS1tS2t;

   // S0 S0LRUN0
   CCFGSetMap m_mapS0cS0RcN0t;
   CCFGSetMap m_mapS0cS0RjN0t;
   CWordCFGSetMap m_mapS0cS0RcN0w;

   CCFGSetMap m_mapS0cS0LcN0t;
   CWordCFGSetMap m_mapS0wS0LcN0t;
   CWordCFGSetMap m_mapS0cS0LcN0w;
   CWordCFGSetMap m_mapS0wS0RcN0t;
   CCFGSetMap m_mapS0cS0UcN0t;
   CWordCFGSetMap m_mapS0wS0UcN0t;
   CWordCFGSetMap m_mapS0cS0UcN0w;

   // S0 S0LRUS1
   CCFGSetMap m_mapS0cS0LcS1c;
   CCFGSetMap m_mapS0cS0LjS1j;
   CWordCFGSetMap m_mapS0cS0LcS1w;

   CWordCFGSetMap m_mapS0wS0LcS1c;
   CCFGSetMap m_mapS0cS0RcS1c;
   CWordCFGSetMap m_mapS0wS0RcS1c;
   CWordCFGSetMap m_mapS0cS0RcS1w;
   CCFGSetMap m_mapS0cS0UcS1c;
   CWordCFGSetMap m_mapS0wS0UcS1c;
   CWordCFGSetMap m_mapS0cS0UcS1w;

   // S0 S1LRUS1
   CCFGSetMap m_mapS0cS1cS1Rc;
   CCFGSetMap m_mapS0jS1cS1Rj;
   CWordCFGSetMap m_mapS0wS1cS1Rc;

   CCFGSetMap m_mapS0cS1cS1Lc;
   CWordCFGSetMap m_mapS0wS1cS1Lc;
   CWordCFGSetMap m_mapS0cS1wS1Lc;
   CWordCFGSetMap m_mapS0cS1wS1Rc;
   CCFGSetMap m_mapS0cS1cS1Uc;
   CWordCFGSetMap m_mapS0wS1cS1Uc;
   CWordCFGSetMap m_mapS0cS1wS1Uc;

   // Rule dictionary
   unsigned long m_nMaxWordFrequency;
   CWordToIntMap m_mapWordFrequency;

public:

   CWeight(bool bTrain) : CWeightBase(bTrain) ,

                          m_nMaxWordFrequency(0) ,
                          m_mapWordFrequency(TABLE_SIZE),

                          m_mapS0c("Stack0Constituent", TABLE_SIZE),
                          m_mapS0w("Stack0Word", TABLE_SIZE),
                          m_mapS0tc("Stack0Tag", TABLE_SIZE),
                          m_mapS0wc("Stack0WordConstituent", TABLE_SIZE),
                          m_mapS0k("Stack0Kluster", TABLE_SIZE),
                          m_mapS0n("Stack0Kluster", TABLE_SIZE),

                          m_mapS1c("Stack1Constituent", TABLE_SIZE),
                          m_mapS1w("Stack1Word", TABLE_SIZE),
                          m_mapS1tc("Stack1Tag", TABLE_SIZE),
                          m_mapS1wc("Stack1WordConstituent", TABLE_SIZE),
                          m_mapS1k("Stack1Kluster", TABLE_SIZE),
                          m_mapS1n("Stack1Kluster", TABLE_SIZE),

                          m_mapS2tc("Stack2Tag", TABLE_SIZE),
                          m_mapS2wc("Stack2WordConstituent", TABLE_SIZE),
                          m_mapS2k("Stack2Kluster", TABLE_SIZE),
                          m_mapS2n("Stack2Kluster", TABLE_SIZE),

                          m_mapS3tc("Stack3Tag", TABLE_SIZE),
                          m_mapS3wc("Stack3WordConstituent", TABLE_SIZE),
                          m_mapS3k("Stack3Kluster", TABLE_SIZE),
                          m_mapS3n("Stack3Kluster", TABLE_SIZE),

                          m_mapN0w("Next0Word", TABLE_SIZE),
                          m_mapN0t("Next0Tag", TABLE_SIZE),
                          m_mapN0wt("Next0WordTag", TABLE_SIZE),
                          m_mapN0k("Next0Klustor", TABLE_SIZE),
                          m_mapN0n("Next0Klustor", TABLE_SIZE),
                                            
                          m_mapN1w("Next1Word", TABLE_SIZE),
                          m_mapN1t("Next1Tag", TABLE_SIZE),
                          m_mapN1wt("Next1WordTag", TABLE_SIZE),
                          m_mapN1k("Next1Klustor", TABLE_SIZE),
                          m_mapN1n("Next1Klustor", TABLE_SIZE),

                          m_mapN2w("Next2Word", TABLE_SIZE),
                          m_mapN2t("Next2Tag", TABLE_SIZE),
                          m_mapN2wt("Next2WordTag", TABLE_SIZE),
                          m_mapN2k("Next2Klustor", TABLE_SIZE),
                          m_mapN2n("Next2Klustor", TABLE_SIZE),

                          m_mapN3w("Next3Word", TABLE_SIZE),
                          m_mapN3t("Next3Tag", TABLE_SIZE),
                          m_mapN3wt("Next3WordTag", TABLE_SIZE),
                          m_mapN3k("Next3Klustor", TABLE_SIZE),
                          m_mapN3n("Next3Klustor", TABLE_SIZE),

                          m_mapS0Lwc("Stack0LeftWord", TABLE_SIZE),
                          m_mapS0Ltc("Stack0LeftConstituent", TABLE_SIZE), 

                          m_mapS0Rwc("Stack0RightWord", TABLE_SIZE),
                          m_mapS0Rtc("Stack0RightConstituent", TABLE_SIZE), 

                          m_mapS0Uwc("Stack0UniWord", TABLE_SIZE),
                          m_mapS0Utc("Stack0UniConstituent", TABLE_SIZE), 

                          m_mapS1Lwc("Stack1LeftWord", TABLE_SIZE),
                          m_mapS1Ltc("Stack1LeftConstituent", TABLE_SIZE), 

                          m_mapS1Rwc("Stack1RightWord", TABLE_SIZE),
                          m_mapS1Rtc("Stack1RightConstituent", TABLE_SIZE), 

                          m_mapS1Uwc("Stack1UniWord", TABLE_SIZE),
                          m_mapS1Utc("Stack1UniConstituent", TABLE_SIZE), 

                          m_mapS0wcS1wc("Stack0WordStack1Word", TABLE_SIZE),
                          m_mapS0wS1c("Stack0WordStack1Constituent", TABLE_SIZE),
                          m_mapS0cS1w("Stack0ConstituentStack1Word", TABLE_SIZE),
                          m_mapS0cS1c("Stack0ConstituentStack1Constituent",
TABLE_SIZE),
                          m_mapBetweenTags("BetweenTags", TABLE_SIZE),
                                            
                          m_mapS0wN0w("Stack0WordNext0Word", TABLE_SIZE),
                          m_mapS0wN0t("Stack0WordNext0Tag", TABLE_SIZE),
                          m_mapS0cN0w("Stack0ConstituentNext0Word", TABLE_SIZE),
                          m_mapS0cN0t("Stack0ConstituentNext0Tag", TABLE_SIZE),
                          m_mapS0cmN0tm("Stack0ConstituentRhythmNext0TagRhythm", TABLE_SIZE),

                          m_mapS1wN0w("Stack1WordNext0Word", TABLE_SIZE),
                          m_mapS1wN0t("Stack1WordNext0Tag", TABLE_SIZE),
                          m_mapS1cN0w("Stack1ConstituentNext0Word", TABLE_SIZE),
                          m_mapS1cN0t("Stack1ConstituentNext0Tag", TABLE_SIZE),

                          m_mapN0wN1w("Next0WordNext1Word", TABLE_SIZE),
                          m_mapN0tN1w("Next0TagNext1Word", TABLE_SIZE),
                          m_mapN0wN1t("Next0WordNext1Tag", TABLE_SIZE),
                          m_mapN0tN1t("Next0TagNext1WordTag", TABLE_SIZE),

                          m_mapS0wS1cN0t("Stack0WordStack1ConstituentNext0Tag", TABLE_SIZE),
                          m_mapS0cS1wN0t("Stack0ConstituentStack1WordNext0Tag", TABLE_SIZE),
                          m_mapS0cS1cN0w("Stack0ConstituentStack1ConstituentNext0Word", TABLE_SIZE),
                          m_mapS0cS1cN0t("Stack0ConstituentStack1ConstituentNext0Tag", TABLE_SIZE),
                          m_mapS0tS1tN0t("Stack0TagStack1TagNext0Tag", TABLE_SIZE),
                          m_mapS0jS1jN0t("Stack0TagOrConstituentStack1TagOrConstituentNext0Tag", TABLE_SIZE),

                          m_mapS0wN0tN1t("Stack0WordNext0TagNext", TABLE_SIZE),
                          m_mapS0cN0wN1t("Stack0ConstituentNext0WordNext1Tag", TABLE_SIZE),
                          m_mapS0cN0tN1w("Stack0ConstituentNext0TagNext1Word", TABLE_SIZE),
                          m_mapS0cN0tN1t("Stack0ConstituentNext0TagNext1Tag", TABLE_SIZE),
                          m_mapS0tN0tN1t("Stack0TagNext0TagNext1Tag", TABLE_SIZE),
                          m_mapS0jN0tN1t("Stack0TagOrConstituentNext0TagNext1Tag", TABLE_SIZE),

                          m_mapS0wS1cS2c("Stack0WordStack1ConstituentStack2Constituent", TABLE_SIZE),
                          m_mapS0cS1wS2c("Stack0ConstituentStack1WordStack2Constituent", TABLE_SIZE),
                          m_mapS0cS1cS2w("Stack0ConstituentStack1ConstituentStack2Word", TABLE_SIZE),
                          m_mapS0cS1cS2c("Stack0ConstituentStack1ConstituentStack2Constituent", TABLE_SIZE),
                          m_mapS0tS1tS2t("Stack0TagStack1TagStack2Tag", TABLE_SIZE),

                          m_mapS0cS0LcN0t("Stack0ConstituentStack0LConstituentNex0tTag", TABLE_SIZE),
                          m_mapS0wS0LcN0t("Stack0WordStack0LConstituentNex0tTag", TABLE_SIZE),
                          m_mapS0cS0LcN0w("Stack0ConstituentStack0LConstituentNex0tWord", TABLE_SIZE),
                          m_mapS0cS0RcN0t("Stack0ConstituentStack0RConstituentNex0tTag", TABLE_SIZE),
                          m_mapS0cS0RjN0t("Stack0ConstituentStack0RTagOrConstituentNex0tTag", TABLE_SIZE),
                          m_mapS0wS0RcN0t("Stack0WordStack0RConstituentNex0tTag", TABLE_SIZE),
                          m_mapS0cS0RcN0w("Stack0ConstituentStack0RConstituentNex0tWord", TABLE_SIZE),
                          m_mapS0cS0UcN0t("Stack0ConstituentStack0UConstituentNex0tTag", TABLE_SIZE),
                          m_mapS0wS0UcN0t("Stack0WordStack0UConstituentNex0tTag", TABLE_SIZE),
                          m_mapS0cS0UcN0w("Stack0ConstituentStack0UConstituentNex0tWord", TABLE_SIZE),

                          m_mapS0cS0LcS1c("Stack0ConstituentStack0LConstituentStack1Constituent", TABLE_SIZE),
                          m_mapS0cS0LjS1j("Stack0ConstituentStack0LTagOrConstituentStack1TagOrConstituent", TABLE_SIZE),
                          m_mapS0wS0LcS1c("Stack0WordStack0LConstituentStack1Constituent", TABLE_SIZE),
                          m_mapS0cS0LcS1w("Stack0ConstituentStack0LConstituentStack1Word", TABLE_SIZE),
                          m_mapS0cS0RcS1c("Stack0ConstituentStack0RConstituentStack1Constituent", TABLE_SIZE),
                          m_mapS0wS0RcS1c("Stack0WordStack0RConstituentStack1Constituent", TABLE_SIZE),
                          m_mapS0cS0RcS1w("Stack0ConstituentStack0RConstituentStack1Word", TABLE_SIZE),
                          m_mapS0cS0UcS1c("Stack0ConstituentStack0UConstituentStack1Constituent", TABLE_SIZE),
                          m_mapS0wS0UcS1c("Stack0WordStack0UConstituentStack1Constituent", TABLE_SIZE),
                          m_mapS0cS0UcS1w("Stack0ConstituentStack0UConstituentStack1Word", TABLE_SIZE),

                          m_mapS0cS1cS1Lc("Stack0ConstituentStack1ConstituentStack1LConstituent", TABLE_SIZE),
                          m_mapS0wS1cS1Lc("Stack0WordStack1ConstituentStack1LConstituent", TABLE_SIZE),
                          m_mapS0cS1wS1Lc("Stack0ConstituentStack1WordStack1LConstituent", TABLE_SIZE),
                          m_mapS0cS1cS1Rc("Stack0ConstituentStack1ConstituentStack1RConstituent", TABLE_SIZE),
                          m_mapS0jS1cS1Rj("Stack0TagOrConstituentStack1ConstituentStack1RTagOrConstituent", TABLE_SIZE),
                          m_mapS0wS1cS1Rc("Stack0WordStack1ConstituentStack1RConstituent", TABLE_SIZE),
                          m_mapS0cS1wS1Rc("Stack0ConstituentStack1WordStack1RConstituent", TABLE_SIZE),
                          m_mapS0cS1cS1Uc("Stack0ConstituentStack1ConstituentStack1UConstituent", TABLE_SIZE),
                          m_mapS0wS1cS1Uc("Stack0WordStack1ConstituentStack1UConstituent", TABLE_SIZE),
                          m_mapS0cS1wS1Uc("Stack0ConstituentStack1WordStack1UConstituent", TABLE_SIZE)
   { }
   ~CWeight() { }

   // MEHTODS
   virtual void loadScores(std::ifstream &is);
   virtual void saveScores(std::ofstream &os);
   void computeAverageFeatureWeights(int round);
   SCORE_TYPE dotProduct(const CWeight &w);
 
};

}
}

#endif
