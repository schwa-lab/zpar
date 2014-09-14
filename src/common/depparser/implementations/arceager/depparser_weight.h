#ifndef _DEPPARSER_WEIGHTS_H
#define _DEPPARSER_WEIGHTS_H

#include <schwa/hashtable.h>

#include "depparser_weight_base.h"


namespace TARGET_LANGUAGE {
namespace depparser {

template <typename SCORE_TYPE>
class Entry : public schwa::FeatureHashtableEntryBase {
private:
  SCORE_TYPE _current;
  SCORE_TYPE _total;
  unsigned int _last_updated;

public:
  Entry(void) : schwa::FeatureHashtableEntryBase(), _current(0), _total(0), _last_updated(0) { }
  ~Entry(void) { }

  inline SCORE_TYPE &operator [](const ScoreAverage n) { return (n == eNonAverage) ? _current : _total; }
  inline const SCORE_TYPE &operator [](const ScoreAverage n) const { return (n == eNonAverage) ? _current : _total; }

  inline const SCORE_TYPE score(const ScoreAverage n=eNonAverage) const { return (n == eNonAverage) ? _current : _total; }

  inline void
  updateAverage(const unsigned int round) {
    if (round > _last_updated)
      _total += _current*(round - _last_updated);
  }

private:
  SCHWA_DISALLOW_COPY_AND_ASSIGN(Entry);
};


class CWeight : public CWeightBase {
public:
  static const schwa::FeatureType STw;
  static const schwa::FeatureType STt;
  static const schwa::FeatureType STwt;
  static const schwa::FeatureType N0w;
  static const schwa::FeatureType N0t;
  static const schwa::FeatureType N0wt;
  static const schwa::FeatureType N1w;
  static const schwa::FeatureType N1t;
  static const schwa::FeatureType N1wt;
  static const schwa::FeatureType N2w;
  static const schwa::FeatureType N2t;
  static const schwa::FeatureType N2wt;
  static const schwa::FeatureType STHw;
  static const schwa::FeatureType STHt;
  static const schwa::FeatureType STi;
  static const schwa::FeatureType STHHw;
  static const schwa::FeatureType STHHt;
  static const schwa::FeatureType STHi;
  static const schwa::FeatureType STLDw;
  static const schwa::FeatureType STLDt;
  static const schwa::FeatureType STLDi;
  static const schwa::FeatureType STRDw;
  static const schwa::FeatureType STRDt;
  static const schwa::FeatureType STRDi;
  static const schwa::FeatureType N0LDw;
  static const schwa::FeatureType N0LDt;
  static const schwa::FeatureType N0LDi;
  static const schwa::FeatureType STL2Dw;
  static const schwa::FeatureType STL2Dt;
  static const schwa::FeatureType STL2Di;
  static const schwa::FeatureType STR2Dw;
  static const schwa::FeatureType STR2Dt;
  static const schwa::FeatureType STR2Di;
  static const schwa::FeatureType N0L2Dw;
  static const schwa::FeatureType N0L2Dt;
  static const schwa::FeatureType N0L2Di;
  static const schwa::FeatureType HTw;
  static const schwa::FeatureType HTt;
  static const schwa::FeatureType HTwt;
  static const schwa::FeatureType STwtN0wt;
  static const schwa::FeatureType STwtN0w;
  static const schwa::FeatureType STwN0wt;
  static const schwa::FeatureType STtN0wt;
  static const schwa::FeatureType STwtN0t;
  static const schwa::FeatureType STwN0w;
  static const schwa::FeatureType STtN0t;
  static const schwa::FeatureType N0tN1t;
  static const schwa::FeatureType N0tN1tN2t;
  static const schwa::FeatureType STtN0tN1t;
  static const schwa::FeatureType STtN0tN0LDt;
  static const schwa::FeatureType N0tN0LDtN0L2Dt;
  static const schwa::FeatureType STHtSTtN0t;
  static const schwa::FeatureType HTtHT2tN0t;
  static const schwa::FeatureType STHHtSTHtSTt;
  static const schwa::FeatureType STtSTLDtN0t;
  static const schwa::FeatureType STtSTLDtSTL2Dt;
  static const schwa::FeatureType STtSTRDtN0t;
  static const schwa::FeatureType STtSTRDtSTR2Dt;
  static const schwa::FeatureType STwd;
  static const schwa::FeatureType STtd;
  static const schwa::FeatureType N0wd;
  static const schwa::FeatureType N0td;
  static const schwa::FeatureType STwN0wd;
  static const schwa::FeatureType STtN0td;
  static const schwa::FeatureType STwra;
  static const schwa::FeatureType STtra;
  static const schwa::FeatureType STwla;
  static const schwa::FeatureType STtla;
  static const schwa::FeatureType N0wla;
  static const schwa::FeatureType N0tla;
  static const schwa::FeatureType STwrp;
  static const schwa::FeatureType STtrp;
  static const schwa::FeatureType STwlp;
  static const schwa::FeatureType STtlp;
  static const schwa::FeatureType N0wlp;
  static const schwa::FeatureType N0tlp;
  static const schwa::FeatureType STl;
  static const schwa::FeatureType STc;
  static const schwa::FeatureType STf;
  static const schwa::FeatureType N0l;
  static const schwa::FeatureType N0c;
  static const schwa::FeatureType N0f;
  static const schwa::FeatureType N1l;
  static const schwa::FeatureType N1c;
  static const schwa::FeatureType N1f;

protected:
  schwa::FeatureHashtable<Entry<SCORE_TYPE>> _weights;

public:
  CWeight(const std::string &sInputPath, bool bTrain) : CWeight(sInputPath, sInputPath, bTrain) { }
  CWeight(const std::string &sInputPath, const std::string &sOutputPath, bool bTrain);
  virtual ~CWeight();

  virtual void loadScores(void) override;
  virtual void saveScores(void) override;

  void addWeighted(double mu, const CWeight &other);
  void computeAverageFeatureWeights(unsigned int round);
  void debugUsage(void) const;

  template <typename CP>
  void getOrUpdateScore(const schwa::FeatureType &type, const CP &cp, CPackedScoreType<SCORE_TYPE, action::MAX> &out, action::StackAction action, ScoreAverage sa, SCORE_TYPE amount, unsigned int round);
};


template <typename CP>
inline void
CWeight::getOrUpdateScore(const schwa::FeatureType &type, const CP &cp, CPackedScoreType<SCORE_TYPE, action::MAX> &out, const action::StackAction action, const ScoreAverage sa, const SCORE_TYPE amount, const unsigned int round) {
  const schwa::Label label = action;

  // Don't create an entry if the update amount is zero.
  if (amount == 0 && _weights.find(type, cp, label) == _weights.end())
    return;
}


#if 0
const static unsigned DEP_TABLE_SIZE = 1000121;  // This should be a prime number.


//
// TYPE DEFINITIONS
//
typedef CPackedScoreMap<CWord, SCORE_TYPE, action::MAX> CWordMap;
typedef CPackedScoreMap<CTag, SCORE_TYPE, action::MAX> CTagMap;
typedef CPackedScoreMap<int, SCORE_TYPE, action::MAX> CIntMap;
typedef CPackedScoreMap<CTagSet<CTag, 2>,  SCORE_TYPE, action::MAX> CTagSet2Map;
typedef CPackedScoreMap<CTagSet<CTag, 3>, SCORE_TYPE, action::MAX> CTagSet3Map;
typedef CPackedScoreMap<std::tuple<CWord, CTag, CTag>, SCORE_TYPE, action::MAX> CWordTagTagMap;
typedef CPackedScoreMap<std::tuple<CWord, CWord, CTag>, SCORE_TYPE, action::MAX> CWordWordTagMap;
typedef CPackedScoreMap<CTaggedWord<CTag, TAG_SEPARATOR>, SCORE_TYPE, action::MAX> CTaggedWordMap;
typedef CPackedScoreMap<CTwoWords, SCORE_TYPE, action::MAX> CTwoWordsMap;
typedef CPackedScoreMap<CTwoTaggedWords, SCORE_TYPE, action::MAX> CTwoTaggedWordsMap;
typedef CPackedScoreMap<std::tuple<CWord, int>, SCORE_TYPE, action::MAX> CWordIntMap;
typedef CPackedScoreMap<std::tuple<CTag, int>, SCORE_TYPE, action::MAX> CTagIntMap;
typedef CPackedScoreMap<std::tuple<CTag, CTag, int>, SCORE_TYPE, action::MAX> CTagTagIntMap;
typedef CPackedScoreMap<std::tuple<CWord, CWord, int>, SCORE_TYPE, action::MAX> CWordWordIntMap;
typedef CPackedScoreMap<std::tuple<CWord, CSetOfTags<CDependencyLabel>>, SCORE_TYPE, action::MAX> CWordSetOfLabelsMap;
typedef CPackedScoreMap<std::tuple<CTag, CSetOfTags<CDependencyLabel>>, SCORE_TYPE, action::MAX> CTagSetOfLabelsMap;
typedef CPackedScoreMap<CLemma, SCORE_TYPE, action::MAX> CLemmaMap;
typedef CPackedScoreMap<CCoNLLCPOS, SCORE_TYPE, action::MAX> CCoNLLCPOSMap;
typedef CPackedScoreMap<CCoNLLFeats, SCORE_TYPE, action::MAX> CCoNLLFeatsMap;

/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/
class CWeight : public CWeightBase {
public:
   CWordMap m_mapSTw;
   CTagMap m_mapSTt;
   CTaggedWordMap m_mapSTwt;

   CWordMap m_mapN0w;
   CTagMap m_mapN0t;
   CTaggedWordMap m_mapN0wt;

   CWordMap m_mapN1w;
   CTagMap m_mapN1t;
   CTaggedWordMap m_mapN1wt;

   CWordMap m_mapN2w;
   CTagMap m_mapN2t;
   CTaggedWordMap m_mapN2wt;

   CWordMap m_mapSTHw;
   CTagMap m_mapSTHt;
   CIntMap m_mapSTi;

   CWordMap m_mapSTHHw;
   CTagMap m_mapSTHHt;
   CIntMap m_mapSTHi;

   CWordMap m_mapSTLDw;
   CTagMap m_mapSTLDt;
   CIntMap m_mapSTLDi;

   CWordMap m_mapSTRDw;
   CTagMap m_mapSTRDt;
   CIntMap m_mapSTRDi;

   CWordMap m_mapN0LDw;
   CTagMap m_mapN0LDt;
   CIntMap m_mapN0LDi;

   CWordMap m_mapSTL2Dw;
   CTagMap m_mapSTL2Dt;
   CIntMap m_mapSTL2Di;

   CWordMap m_mapSTR2Dw;
   CTagMap m_mapSTR2Dt;
   CIntMap m_mapSTR2Di;

   CWordMap m_mapN0L2Dw;
   CTagMap m_mapN0L2Dt;
   CIntMap m_mapN0L2Di;

   CWordMap m_mapHTw;
   CTagMap m_mapHTt;
   CTaggedWordMap m_mapHTwt;

   CTwoTaggedWordsMap m_mapSTwtN0wt;
   CWordWordTagMap m_mapSTwtN0w;
   CWordWordTagMap m_mapSTwN0wt;
   CWordTagTagMap m_mapSTtN0wt;
   CWordTagTagMap m_mapSTwtN0t;
   CTwoWordsMap m_mapSTwN0w;
   CTagSet2Map m_mapSTtN0t;

   CTagSet2Map m_mapN0tN1t;
   CTagSet3Map m_mapN0tN1tN2t;
   CTagSet3Map m_mapSTtN0tN1t;
   CTagSet3Map m_mapSTtN0tN0LDt;
   CTagSet3Map m_mapN0tN0LDtN0L2Dt;
   CTagSet3Map m_mapSTHtSTtN0t;
   CTagSet3Map m_mapHTtHT2tN0t;
   CTagSet3Map m_mapSTHHtSTHtSTt;
   CTagSet3Map m_mapSTtSTLDtN0t;
   CTagSet3Map m_mapSTtSTLDtSTL2Dt;
   CTagSet3Map m_mapSTtSTRDtN0t;
   CTagSet3Map m_mapSTtSTRDtSTR2Dt;

   CWordIntMap m_mapSTwd;
   CTagIntMap m_mapSTtd;
   CWordIntMap m_mapN0wd;
   CTagIntMap m_mapN0td;
   CWordWordIntMap m_mapSTwN0wd;
   CTagTagIntMap m_mapSTtN0td;

   CWordIntMap m_mapSTwra;
   CTagIntMap m_mapSTtra;
   CWordIntMap m_mapSTwla;
   CTagIntMap m_mapSTtla;
   CWordIntMap m_mapN0wla;
   CTagIntMap m_mapN0tla;

   CWordSetOfLabelsMap m_mapSTwrp;
   CTagSetOfLabelsMap m_mapSTtrp;
   CWordSetOfLabelsMap m_mapSTwlp;
   CTagSetOfLabelsMap m_mapSTtlp;
   CWordSetOfLabelsMap m_mapN0wlp;
   CTagSetOfLabelsMap m_mapN0tlp;

   CLemmaMap m_mapSTl;
   CCoNLLCPOSMap m_mapSTc;
   CCoNLLFeatsMap m_mapSTf;

   CLemmaMap m_mapN0l;
   CCoNLLCPOSMap m_mapN0c;
   CCoNLLFeatsMap m_mapN0f;

   CLemmaMap m_mapN1l;
   CCoNLLCPOSMap m_mapN1c;
   CCoNLLFeatsMap m_mapN1f;

public:
   CWeight(const std::string &sInputPath, bool bTrain) : CWeight(sInputPath, sInputPath, bTrain) { }
   CWeight(const std::string &sInputPath, const std::string &sOutputPath, bool bTrain) : CWeightBase(sInputPath, sOutputPath, bTrain) ,

                                               m_mapSTw("StackWord", DEP_TABLE_SIZE),
                                               m_mapSTt("StackTag", DEP_TABLE_SIZE),
                                               m_mapSTwt("StackWordTag", DEP_TABLE_SIZE),

                                               m_mapN0w("NextWord", DEP_TABLE_SIZE),
                                               m_mapN0t("NextTag", DEP_TABLE_SIZE),
                                               m_mapN0wt("NextWordTag", DEP_TABLE_SIZE),

                                               m_mapN1w("Next+1Word", DEP_TABLE_SIZE),
                                               m_mapN1t("Next+1Tag", DEP_TABLE_SIZE),
                                               m_mapN1wt("Next+1WordTag", DEP_TABLE_SIZE),

                                               m_mapN2w("Next+2Word", DEP_TABLE_SIZE),
                                               m_mapN2t("Next+2Tag", DEP_TABLE_SIZE),
                                               m_mapN2wt("Next+2WordTag", DEP_TABLE_SIZE),

                                               m_mapSTHw("StackHeadWord", DEP_TABLE_SIZE),
                                               m_mapSTHt("StackHeadTag", DEP_TABLE_SIZE),
                                               m_mapSTi("StackLabel", DEP_TABLE_SIZE),

                                               m_mapSTHHw("StackHeadHeadWord", DEP_TABLE_SIZE),
                                               m_mapSTHHt("StackHeadHeadTag", DEP_TABLE_SIZE),
                                               m_mapSTHi("StackLabel", DEP_TABLE_SIZE),

                                               m_mapSTLDw("StackLDWord", DEP_TABLE_SIZE),
                                               m_mapSTLDt("StackLDTag", DEP_TABLE_SIZE),
                                               m_mapSTLDi("StackLDLabel", DEP_TABLE_SIZE),

                                               m_mapSTRDw("StackRDWord", DEP_TABLE_SIZE),
                                               m_mapSTRDt("StackRDTag", DEP_TABLE_SIZE),
                                               m_mapSTRDi("StackRDLabel", DEP_TABLE_SIZE),

                                               m_mapN0LDw("NextLDWord", DEP_TABLE_SIZE),
                                               m_mapN0LDt("NextLDTag", DEP_TABLE_SIZE),
                                               m_mapN0LDi("NextLDLabel", DEP_TABLE_SIZE),

                                               m_mapSTL2Dw("StackL2DWord", DEP_TABLE_SIZE),
                                               m_mapSTL2Dt("StackL2DTag", DEP_TABLE_SIZE),
                                               m_mapSTL2Di("StackL2DLabel", DEP_TABLE_SIZE),

                                               m_mapSTR2Dw("StackR2DWord", DEP_TABLE_SIZE),
                                               m_mapSTR2Dt("StackR2DTag", DEP_TABLE_SIZE),
                                               m_mapSTR2Di("StackR2DLabel", DEP_TABLE_SIZE),

                                               m_mapN0L2Dw("NextL2DWord", DEP_TABLE_SIZE),
                                               m_mapN0L2Dt("NextL2DTag", DEP_TABLE_SIZE),
                                               m_mapN0L2Di("NextL2DLabel", DEP_TABLE_SIZE),

                                               m_mapHTw("HeadStackWord", DEP_TABLE_SIZE),
                                               m_mapHTt("HeadStackTag", DEP_TABLE_SIZE),
                                               m_mapHTwt("HeadStackWordTag", DEP_TABLE_SIZE),

                                               m_mapSTwtN0wt("StackWordTagNextWordTag", DEP_TABLE_SIZE),
                                               m_mapSTwtN0w("StackWordTagNextWord", DEP_TABLE_SIZE),
                                               m_mapSTwN0wt("StackWordNextWordTag", DEP_TABLE_SIZE),
                                               m_mapSTtN0wt("StackTagNextWordTag", DEP_TABLE_SIZE),
                                               m_mapSTwtN0t("StackWordTagNextTag", DEP_TABLE_SIZE),
                                               m_mapSTwN0w("StackWordNextWord", DEP_TABLE_SIZE),
                                               m_mapSTtN0t("StackTagNextTag", DEP_TABLE_SIZE),

                                               m_mapN0tN1t("NextTagNext+1Tag", DEP_TABLE_SIZE),
                                               m_mapN0tN1tN2t("NextTagTrigram", DEP_TABLE_SIZE),
                                               m_mapSTtN0tN1t("StackTagNextTagNext+1Tag", DEP_TABLE_SIZE),
                                               m_mapSTtN0tN0LDt("StackTagNextTagNextLDTag", DEP_TABLE_SIZE),
                                               m_mapN0tN0LDtN0L2Dt("StackTagNextTagNextLDTagNextTagNextL2DTag", DEP_TABLE_SIZE),
                                               m_mapSTHtSTtN0t("StackHeadTagStackTagNextTag", DEP_TABLE_SIZE),
                                               m_mapHTtHT2tN0t("HeadStackTagHeadStack2TagNextTag", DEP_TABLE_SIZE),
                                               m_mapSTHHtSTHtSTt("StackHeadHeadTagStackHeadTagStackTag", DEP_TABLE_SIZE),
                                               m_mapSTtSTLDtN0t("StackTagStackLDTagNextTag", DEP_TABLE_SIZE),
                                               m_mapSTtSTLDtSTL2Dt("StackTagStackLDTagStackL2DTag", DEP_TABLE_SIZE),
                                               m_mapSTtSTRDtN0t("StackTagStackRDTagNextTag", DEP_TABLE_SIZE),
                                               m_mapSTtSTRDtSTR2Dt("StackTagStackRDTagStackR2DTag", DEP_TABLE_SIZE),

                                               m_mapSTwd("StackWordDist", DEP_TABLE_SIZE),
                                               m_mapSTtd("StackTagDist", DEP_TABLE_SIZE),
                                               m_mapN0wd("NextWordDist", DEP_TABLE_SIZE),
                                               m_mapN0td("NextTagDist", DEP_TABLE_SIZE),
                                               m_mapSTwN0wd("StackWordNextWordDist", DEP_TABLE_SIZE),
                                               m_mapSTtN0td("StackTagNextTagDist", DEP_TABLE_SIZE),

                                               m_mapSTwra("StackWordRightArity", DEP_TABLE_SIZE),
                                               m_mapSTtra("StackTagRightArity", DEP_TABLE_SIZE),
                                               m_mapSTwla("StackWordLeftArity", DEP_TABLE_SIZE),
                                               m_mapSTtla("StackTagLeftArity", DEP_TABLE_SIZE),
                                               m_mapN0wla("NextWordRightArity", DEP_TABLE_SIZE),
                                               m_mapN0tla("NextTagRightArity", DEP_TABLE_SIZE),

                                               m_mapSTwrp("StackWordRightSetoftags", DEP_TABLE_SIZE),
                                               m_mapSTtrp("StackTagRightSetoftags", DEP_TABLE_SIZE),
                                               m_mapSTwlp("StackWordLeftSetoftags", DEP_TABLE_SIZE),
                                               m_mapSTtlp("StackTagLeftSetoftags", DEP_TABLE_SIZE),
                                               m_mapN0wlp("Next0WordLeftSetoftags", DEP_TABLE_SIZE),
                                               m_mapN0tlp("Next0TagLeftSetoftags", DEP_TABLE_SIZE),

                                               m_mapSTl("StackLemma", DEP_TABLE_SIZE),
                                               m_mapSTc("StackCPOS", DEP_TABLE_SIZE),
                                               m_mapSTf("StackFeats", DEP_TABLE_SIZE),

                                               m_mapN0l("NextLemma", DEP_TABLE_SIZE),
                                               m_mapN0c("NextCPOS", DEP_TABLE_SIZE),
                                               m_mapN0f("NextFeats", DEP_TABLE_SIZE),

                                               m_mapN1l("Next+1Lemma", DEP_TABLE_SIZE),
                                               m_mapN1c("Next+1CPOS", DEP_TABLE_SIZE),
                                               m_mapN1f("Next+1Feats", DEP_TABLE_SIZE)
   { loadScores(); }
   virtual ~CWeight() { }

   virtual void loadScores();
   virtual void saveScores();

   void addWeighted(double mu, const CWeight &other);
   void computeAverageFeatureWeights(int round);
   void debugUsage(void) const;
};
#endif

}
}

#endif
