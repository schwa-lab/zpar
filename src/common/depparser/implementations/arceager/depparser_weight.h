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

#include "depparser_weight_base.h"

#define iterate_templates(left, right) \
  left(m_mapSTw)right\
  left(m_mapSTt)right\
  left(m_mapSTwt)right\
  left(m_mapN0w)right\
  left(m_mapN0t)right\
  left(m_mapN0wt)right\
  left(m_mapN1w)right\
  left(m_mapN1t)right\
  left(m_mapN1wt)right\
  left(m_mapN2w)right\
  left(m_mapN2t)right\
  left(m_mapN2wt)right\
  left(m_mapSTHw)right\
  left(m_mapSTHt)right\
  left(m_mapSTi)right\
  left(m_mapSTHHw)right\
  left(m_mapSTHHt)right\
  left(m_mapSTHi)right\
  left(m_mapSTLDw)right\
  left(m_mapSTLDt)right\
  left(m_mapSTLDi)right\
  left(m_mapSTRDw)right\
  left(m_mapSTRDt)right\
  left(m_mapSTRDi)right\
  left(m_mapN0LDw)right\
  left(m_mapN0LDt)right\
  left(m_mapN0LDi)right\
  left(m_mapSTL2Dw)right\
  left(m_mapSTL2Dt)right\
  left(m_mapSTL2Di)right\
  left(m_mapSTR2Dw)right\
  left(m_mapSTR2Dt)right\
  left(m_mapSTR2Di)right\
  left(m_mapN0L2Dw)right\
  left(m_mapN0L2Dt)right\
  left(m_mapN0L2Di)right\
  left(m_mapHTw)right\
  left(m_mapHTt)right\
  left(m_mapHTwt)right\
\
  left(m_mapSTwtN0wt)right\
  left(m_mapSTwtN0w)right\
  left(m_mapSTwN0wt)right\
  left(m_mapSTtN0wt)right\
  left(m_mapSTwtN0t)right\
  left(m_mapSTwN0w)right\
  left(m_mapSTtN0t)right\
\
  left(m_mapN0tN1t)right\
  left(m_mapN0tN1tN2t)right\
  left(m_mapSTtN0tN1t)right\
  left(m_mapSTtN0tN0LDt)right\
  left(m_mapN0tN0LDtN0L2Dt)right\
  left(m_mapSTHtSTtN0t)right\
  left(m_mapHTtHT2tN0t)right\
  left(m_mapSTHHtSTHtSTt)right\
  left(m_mapSTtSTLDtN0t)right\
  left(m_mapSTtSTLDtSTL2Dt)right\
  left(m_mapSTtSTRDtN0t)right\
  left(m_mapSTtSTRDtSTR2Dt)right\
\
  left(m_mapSTwd)right\
  left(m_mapSTtd)right\
  left(m_mapN0wd)right\
  left(m_mapN0td)right\
  left(m_mapSTwN0wd)right\
  left(m_mapSTtN0td)right\
\
  left(m_mapSTwra)right\
  left(m_mapSTtra)right\
  left(m_mapSTwla)right\
  left(m_mapSTtla)right\
  left(m_mapN0wla)right\
  left(m_mapN0tla)right\
\
  left(m_mapSTwrp)right\
  left(m_mapSTtrp)right\
  left(m_mapSTwlp)right\
  left(m_mapSTtlp)right\
  left(m_mapN0wlp)right\
  left(m_mapN0tlp)right\
\
  left(m_mapSTl)right\
  left(m_mapSTc)right\
  left(m_mapSTf)right\
\
  left(m_mapN0l)right\
  left(m_mapN0c)right\
  left(m_mapN0f)right\
\
  left(m_mapN1l)right\
  left(m_mapN1c)right\
  left(m_mapN1f)right

namespace TARGET_LANGUAGE {
namespace depparser {

/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/
template <typename SCORE_TYPE>
class CWeight : public CWeightBase {
public:
  static constexpr const unsigned DEP_TABLE_SIZE = 15139;  // This should be a prime number.

  using DenseFeatureMap = CPackedScoreMap<unsigned long, CPackedScoreArray<SCORE_TYPE, action::MAX>>;
  using SparseFeatureMap = CPackedScoreMap<unsigned long, CPackedScoreList<SCORE_TYPE, action::MAX>>;

  SparseFeatureMap m_mapSTw;
  DenseFeatureMap m_mapSTt;
  SparseFeatureMap m_mapSTwt;

  SparseFeatureMap m_mapN0w;
  DenseFeatureMap m_mapN0t;
  SparseFeatureMap m_mapN0wt;

  SparseFeatureMap m_mapN1w;
  DenseFeatureMap m_mapN1t;
  SparseFeatureMap m_mapN1wt;

  SparseFeatureMap m_mapN2w;
  DenseFeatureMap m_mapN2t;
  SparseFeatureMap m_mapN2wt;

  SparseFeatureMap m_mapSTHw;
  SparseFeatureMap m_mapSTHt;
  SparseFeatureMap m_mapSTi;

  SparseFeatureMap m_mapSTHHw;
  SparseFeatureMap m_mapSTHHt;
  SparseFeatureMap m_mapSTHi;

  SparseFeatureMap m_mapSTLDw;
  DenseFeatureMap m_mapSTLDt;
  DenseFeatureMap m_mapSTLDi;

  SparseFeatureMap m_mapSTRDw;
  DenseFeatureMap m_mapSTRDt;
  DenseFeatureMap m_mapSTRDi;

  SparseFeatureMap m_mapN0LDw;
  DenseFeatureMap m_mapN0LDt;
  DenseFeatureMap m_mapN0LDi;

  SparseFeatureMap m_mapSTL2Dw;
  DenseFeatureMap m_mapSTL2Dt;
  DenseFeatureMap m_mapSTL2Di;

  SparseFeatureMap m_mapSTR2Dw;
  DenseFeatureMap m_mapSTR2Dt;
  DenseFeatureMap m_mapSTR2Di;

  SparseFeatureMap m_mapN0L2Dw;
  SparseFeatureMap m_mapN0L2Dt;
  SparseFeatureMap m_mapN0L2Di;

  SparseFeatureMap m_mapHTw;
  SparseFeatureMap m_mapHTt;
  SparseFeatureMap m_mapHTwt;

  SparseFeatureMap m_mapSTwtN0wt;
  SparseFeatureMap m_mapSTwtN0w;
  SparseFeatureMap m_mapSTwN0wt;
  SparseFeatureMap m_mapSTtN0wt;
  SparseFeatureMap m_mapSTwtN0t;
  SparseFeatureMap m_mapSTwN0w;
  SparseFeatureMap m_mapSTtN0t;

  SparseFeatureMap m_mapN0tN1t;
  SparseFeatureMap m_mapN0tN1tN2t;
  SparseFeatureMap m_mapSTtN0tN1t;
  SparseFeatureMap m_mapSTtN0tN0LDt;
  SparseFeatureMap m_mapN0tN0LDtN0L2Dt;
  SparseFeatureMap m_mapSTHtSTtN0t;
  SparseFeatureMap m_mapHTtHT2tN0t;
  SparseFeatureMap m_mapSTHHtSTHtSTt;
  SparseFeatureMap m_mapSTtSTLDtN0t;
  SparseFeatureMap m_mapSTtSTLDtSTL2Dt;
  SparseFeatureMap m_mapSTtSTRDtN0t;
  SparseFeatureMap m_mapSTtSTRDtSTR2Dt;

  SparseFeatureMap m_mapSTwd;
  SparseFeatureMap m_mapSTtd;
  SparseFeatureMap m_mapN0wd;
  DenseFeatureMap m_mapN0td;
  SparseFeatureMap m_mapSTwN0wd;
  SparseFeatureMap m_mapSTtN0td;

  SparseFeatureMap m_mapSTwra;
  DenseFeatureMap m_mapSTtra;
  SparseFeatureMap m_mapSTwla;
  DenseFeatureMap m_mapSTtla;
  SparseFeatureMap m_mapN0wla;
  DenseFeatureMap m_mapN0tla;

  SparseFeatureMap m_mapSTwrp;
  SparseFeatureMap m_mapSTtrp;
  SparseFeatureMap m_mapSTwlp;
  SparseFeatureMap m_mapSTtlp;
  SparseFeatureMap m_mapN0wlp;
  SparseFeatureMap m_mapN0tlp;

  SparseFeatureMap m_mapSTl;
  SparseFeatureMap m_mapSTc;
  SparseFeatureMap m_mapSTf;

  SparseFeatureMap m_mapN0l;
  SparseFeatureMap m_mapN0c;
  SparseFeatureMap m_mapN0f;

  SparseFeatureMap m_mapN1l;
  SparseFeatureMap m_mapN1c;
  SparseFeatureMap m_mapN1f;

public:
  CWeight(const std::string &sInputPath, bool bTrain) : CWeight(sInputPath, sInputPath, bTrain) { }
  CWeight(const std::string &sInputPath, const std::string &sOutputPath, bool bTrain);
  virtual ~CWeight() { }

  virtual void loadScores() override;
  virtual void saveScores() override;

  void combineAdd(const CWeight &other);
  void combineDiv(unsigned int n);
  void computeAverageFeatureWeights(int round);
  void debugUsage(void) const;
};


template <typename SCORE_TYPE>
CWeight<SCORE_TYPE>::CWeight(const std::string &sInputPath, const std::string &sOutputPath, bool bTrain) :
    CWeightBase(sInputPath, sOutputPath, bTrain),
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
  {
  loadScores();
}


template <typename SCORE_TYPE>
void
CWeight<SCORE_TYPE>::loadScores() {
  std::ifstream in(m_sInputPath, std::ios_base::binary);
  if (!in.is_open()) {
    //std::cout << "No scores loaded." << std::endl;
    return;
  }

#ifdef LABELED
  // Read in each of the dependency labels.
  CDependencyLabel label;
  const uint32_t nlabels = mp::read_array_size(in);
  for (uint32_t i = 0; i != nlabels; ++i)
    in >> label;
#endif

  // Read in each of the feature tables.
  iterate_templates( , .deserialise(in); );

  // Read in whether or not rules were used.
  setRules(mp::read_bool(in));

  in.close() ;
}


template <typename SCORE_TYPE>
void
CWeight<SCORE_TYPE>::saveScores() {
  std::ofstream out(m_sOutputPath, std::ios_base::binary);

#ifdef LABELED
  // Write each of the dependency labels.
  mp::write_array_size(out, CDependencyLabel::COUNT - CDependencyLabel::FIRST);
  for (unsigned label = CDependencyLabel::FIRST; label != CDependencyLabel::COUNT; ++label)
    out << CDependencyLabel(label);
#endif

  // Write out each of the feature tables.
  iterate_templates( , .serialise(out); )

  // Write out whether or not rules were used.
  mp::write_bool(out, m_bRules);

  out.close();
}


template <typename SCORE_TYPE>
void
CWeight<SCORE_TYPE>::combineAdd(const CWeight<SCORE_TYPE> &other) {
  m_mapSTw.combineAdd(other.m_mapSTw);
  m_mapSTt.combineAdd(other.m_mapSTt);
  m_mapSTwt.combineAdd(other.m_mapSTwt);
  m_mapN0w.combineAdd(other.m_mapN0w);
  m_mapN0t.combineAdd(other.m_mapN0t);
  m_mapN0wt.combineAdd(other.m_mapN0wt);
  m_mapN1w.combineAdd(other.m_mapN1w);
  m_mapN1t.combineAdd(other.m_mapN1t);
  m_mapN1wt.combineAdd(other.m_mapN1wt);
  m_mapN2w.combineAdd(other.m_mapN2w);
  m_mapN2t.combineAdd(other.m_mapN2t);
  m_mapN2wt.combineAdd(other.m_mapN2wt);
  m_mapSTHw.combineAdd(other.m_mapSTHw);
  m_mapSTHt.combineAdd(other.m_mapSTHt);
  m_mapSTi.combineAdd(other.m_mapSTi);
  m_mapSTHHw.combineAdd(other.m_mapSTHHw);
  m_mapSTHHt.combineAdd(other.m_mapSTHHt);
  m_mapSTHi.combineAdd(other.m_mapSTHi);
  m_mapSTLDw.combineAdd(other.m_mapSTLDw);
  m_mapSTLDt.combineAdd(other.m_mapSTLDt);
  m_mapSTLDi.combineAdd(other.m_mapSTLDi);
  m_mapSTRDw.combineAdd(other.m_mapSTRDw);
  m_mapSTRDt.combineAdd(other.m_mapSTRDt);
  m_mapSTRDi.combineAdd(other.m_mapSTRDi);
  m_mapN0LDw.combineAdd(other.m_mapN0LDw);
  m_mapN0LDt.combineAdd(other.m_mapN0LDt);
  m_mapN0LDi.combineAdd(other.m_mapN0LDi);
  m_mapSTL2Dw.combineAdd(other.m_mapSTL2Dw);
  m_mapSTL2Dt.combineAdd(other.m_mapSTL2Dt);
  m_mapSTL2Di.combineAdd(other.m_mapSTL2Di);
  m_mapSTR2Dw.combineAdd(other.m_mapSTR2Dw);
  m_mapSTR2Dt.combineAdd(other.m_mapSTR2Dt);
  m_mapSTR2Di.combineAdd(other.m_mapSTR2Di);
  m_mapN0L2Dw.combineAdd(other.m_mapN0L2Dw);
  m_mapN0L2Dt.combineAdd(other.m_mapN0L2Dt);
  m_mapN0L2Di.combineAdd(other.m_mapN0L2Di);
  m_mapHTw.combineAdd(other.m_mapHTw);
  m_mapHTt.combineAdd(other.m_mapHTt);
  m_mapHTwt.combineAdd(other.m_mapHTwt);
  m_mapSTwtN0wt.combineAdd(other.m_mapSTwtN0wt);
  m_mapSTwtN0w.combineAdd(other.m_mapSTwtN0w);
  m_mapSTwN0wt.combineAdd(other.m_mapSTwN0wt);
  m_mapSTtN0wt.combineAdd(other.m_mapSTtN0wt);
  m_mapSTwtN0t.combineAdd(other.m_mapSTwtN0t);
  m_mapSTwN0w.combineAdd(other.m_mapSTwN0w);
  m_mapSTtN0t.combineAdd(other.m_mapSTtN0t);
  m_mapN0tN1t.combineAdd(other.m_mapN0tN1t);
  m_mapN0tN1tN2t.combineAdd(other.m_mapN0tN1tN2t);
  m_mapSTtN0tN1t.combineAdd(other.m_mapSTtN0tN1t);
  m_mapSTtN0tN0LDt.combineAdd(other.m_mapSTtN0tN0LDt);
  m_mapN0tN0LDtN0L2Dt.combineAdd(other.m_mapN0tN0LDtN0L2Dt);
  m_mapSTHtSTtN0t.combineAdd(other.m_mapSTHtSTtN0t);
  m_mapHTtHT2tN0t.combineAdd(other.m_mapHTtHT2tN0t);
  m_mapSTHHtSTHtSTt.combineAdd(other.m_mapSTHHtSTHtSTt);
  m_mapSTtSTLDtN0t.combineAdd(other.m_mapSTtSTLDtN0t);
  m_mapSTtSTLDtSTL2Dt.combineAdd(other.m_mapSTtSTLDtSTL2Dt);
  m_mapSTtSTRDtN0t.combineAdd(other.m_mapSTtSTRDtN0t);
  m_mapSTtSTRDtSTR2Dt.combineAdd(other.m_mapSTtSTRDtSTR2Dt);
  m_mapSTwd.combineAdd(other.m_mapSTwd);
  m_mapSTtd.combineAdd(other.m_mapSTtd);
  m_mapN0wd.combineAdd(other.m_mapN0wd);
  m_mapN0td.combineAdd(other.m_mapN0td);
  m_mapSTwN0wd.combineAdd(other.m_mapSTwN0wd);
  m_mapSTtN0td.combineAdd(other.m_mapSTtN0td);
  m_mapSTwra.combineAdd(other.m_mapSTwra);
  m_mapSTtra.combineAdd(other.m_mapSTtra);
  m_mapSTwla.combineAdd(other.m_mapSTwla);
  m_mapSTtla.combineAdd(other.m_mapSTtla);
  m_mapN0wla.combineAdd(other.m_mapN0wla);
  m_mapN0tla.combineAdd(other.m_mapN0tla);
  m_mapSTwrp.combineAdd(other.m_mapSTwrp);
  m_mapSTtrp.combineAdd(other.m_mapSTtrp);
  m_mapSTwlp.combineAdd(other.m_mapSTwlp);
  m_mapSTtlp.combineAdd(other.m_mapSTtlp);
  m_mapN0wlp.combineAdd(other.m_mapN0wlp);
  m_mapN0tlp.combineAdd(other.m_mapN0tlp);
  m_mapSTl.combineAdd(other.m_mapSTl);
  m_mapSTc.combineAdd(other.m_mapSTc);
  m_mapSTf.combineAdd(other.m_mapSTf);
  m_mapN0l.combineAdd(other.m_mapN0l);
  m_mapN0c.combineAdd(other.m_mapN0c);
  m_mapN0f.combineAdd(other.m_mapN0f);
  m_mapN1l.combineAdd(other.m_mapN1l);
  m_mapN1c.combineAdd(other.m_mapN1c);
  m_mapN1f.combineAdd(other.m_mapN1f);
}


template <typename SCORE_TYPE>
void
CWeight<SCORE_TYPE>::combineDiv(const unsigned int n) {
   iterate_templates(,.combineDiv(n);) ;
}


template <typename SCORE_TYPE>
void
CWeight<SCORE_TYPE>::computeAverageFeatureWeights(int round) {
   //std::cout<<"Computing averaged (total) feature vector..." << std::endl;
   iterate_templates(,.computeAverage(round);) ;
}

}  // namespace depparser
}  // namespace TARGET_LANGUAGE

#endif
