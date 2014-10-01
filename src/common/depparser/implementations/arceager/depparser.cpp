// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * depparser.cpp - the action based parser implementation       *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.12                        *
 *                                                              *
 ****************************************************************/

#include "depparser.h"
#include "depparser_weight.h"

using namespace TARGET_LANGUAGE;
using namespace TARGET_LANGUAGE::depparser;

const CTaggedWord<CTag, TAG_SEPARATOR> g_emptyTaggedWord;

#define cast_weights static_cast<CWeight *>(m_weights)
#define _conll_or_empty(x) (x == "_" ? "" : x)

/*===============================================================
 *
 * CDepParser - the depparser for TARGET_LANGUAGE 
 *
 *==============================================================*/

CDepParser::CDepParser(const std::string &sInputPath, bool bTrain, bool bCoNLL) :
    CDepParserBase(bTrain, bCoNLL),
    m_eScoreAverage(bTrain ? SCORE_NON_AVERAGE : SCORE_AVERAGE)
  {
  m_Agenda = new CAgendaBeam<depparser::CStateItem>(AGENDA_SIZE);
  m_Beam = new CAgendaSimple<depparser::action::CScoredAction>(AGENDA_SIZE);
  m_weights = new depparser::CWeight(sInputPath, sInputPath, bTrain);
  m_nTrainingRound = 0;
  m_nTotalErrors = 0;
}

CDepParser::CDepParser(depparser::CWeight *weights, bool bCoNLL) :
    CDepParserBase(true, bCoNLL),
    m_eScoreAverage(SCORE_NON_AVERAGE)
  {
  m_Agenda = new CAgendaBeam<depparser::CStateItem>(AGENDA_SIZE);
  m_Beam = new CAgendaSimple<depparser::action::CScoredAction>(AGENDA_SIZE);
  m_weights = weights;
  m_nTrainingRound = 0;
  m_nTotalErrors = 0;
}

CDepParser::~CDepParser() {
  delete m_Agenda;
  delete m_Beam;
  delete m_weights;
}


void
CDepParser::getStackScore(const CStateItem *item, CPackedScoreType<SCORE_TYPE, action::MAX> &retval) {
   const int &st_index = item->stackempty() ? -1 : item->stacktop(); // stack top
   const int &sth_index = st_index == -1 ? -1 : item->head(st_index); // stack top head
   const int &sthh_index = sth_index == -1 ? -1 : item->head(sth_index); // stack top head
   const int &stld_index = st_index == -1 ? -1 : item->leftdep(st_index); // leftmost dep of stack
   const int &strd_index = st_index == -1 ? -1 : item->rightdep(st_index); // rightmost dep st
   const int &stl2d_index = stld_index == -1 ? -1 : item->sibling(stld_index); // left 2ndmost dep of stack
   const int &str2d_index = strd_index == -1 ? -1 : item->sibling(strd_index); // right 2ndmost dep st
   const int &n0_index = item->size()==m_lCache.size() ? -1 : item->size(); // next
   assert(n0_index<static_cast<int>(m_lCache.size())); // the next index shouldn't exceed sentence
   const int &n0ld_index = n0_index==-1 ? -1 : item->leftdep(n0_index); // leftmost dep of next
   const int &n0l2d_index = n0ld_index==-1 ? -1 : item->sibling(n0ld_index); // leftmost dep of next
   //const int &ht_index = item->headstackempty() ? -1 : item->headstacktop(); // headstack
   //const int &ht2_index = item->headstacksize()<2 ? -1 : item->headstackitem(item->headstacksize()-2); // headstack 2nd
   const int n1_index = (n0_index != -1 && n0_index+1<m_lCache.size()) ? n0_index+1 : -1 ;
   const int n2_index = (n0_index != -1 && n0_index+2<m_lCache.size()) ? n0_index+2 : -1 ;
   //const int n3_index = (n0_index != -1 && n0_index+3<m_lCache.size()) ? n0_index+3 : -1 ;

   const CTaggedWord<CTag, TAG_SEPARATOR> &st_word_tag = st_index==-1 ? g_emptyTaggedWord : m_lCache[st_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &sth_word_tag = sth_index==-1 ? g_emptyTaggedWord : m_lCache[sth_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &sthh_word_tag = sthh_index==-1 ? g_emptyTaggedWord : m_lCache[sthh_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &stld_word_tag = stld_index==-1 ? g_emptyTaggedWord : m_lCache[stld_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &strd_word_tag = strd_index==-1 ? g_emptyTaggedWord : m_lCache[strd_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &stl2d_word_tag = stl2d_index==-1 ? g_emptyTaggedWord : m_lCache[stl2d_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &str2d_word_tag = str2d_index==-1 ? g_emptyTaggedWord : m_lCache[str2d_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &n0_word_tag = n0_index==-1 ? g_emptyTaggedWord : m_lCache[n0_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &n0ld_word_tag = n0ld_index==-1 ? g_emptyTaggedWord : m_lCache[n0ld_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &n0l2d_word_tag = n0l2d_index==-1 ? g_emptyTaggedWord : m_lCache[n0l2d_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &n1_word_tag = n1_index==-1 ? g_emptyTaggedWord : m_lCache[n1_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &n2_word_tag = n2_index==-1 ? g_emptyTaggedWord : m_lCache[n2_index];
   //const CTaggedWord<CTag, TAG_SEPARATOR> &ht_word_tag = ht_index==-1 ? g_emptyTaggedWord : m_lCache[ht_index];
   //const CTaggedWord<CTag, TAG_SEPARATOR> &ht2_word_tag = ht2_index==-1 ? g_emptyTaggedWord : m_lCache[ht2_index];

   const CWord &st_word = st_word_tag.word;
   const CWord &sth_word = sth_word_tag.word;
   const CWord &sthh_word = sthh_word_tag.word;
   const CWord &stld_word = stld_word_tag.word;
   const CWord &strd_word = strd_word_tag.word;
   const CWord &stl2d_word = stl2d_word_tag.word;
   const CWord &str2d_word = str2d_word_tag.word;
   const CWord &n0_word = n0_word_tag.word;
   const CWord &n0ld_word = n0ld_word_tag.word;
   const CWord &n0l2d_word = n0l2d_word_tag.word;
   const CWord &n1_word = n1_word_tag.word;
   const CWord &n2_word = n2_word_tag.word;
   //const CWord &ht_word = ht_word_tag.word;
   //const CWord &ht2_word = ht2_word_tag.word;

   const CTag &st_tag = st_word_tag.tag;
   const CTag &sth_tag = sth_word_tag.tag;
   const CTag &sthh_tag = sthh_word_tag.tag;
   const CTag &stld_tag = stld_word_tag.tag;
   const CTag &strd_tag = strd_word_tag.tag;
   const CTag &stl2d_tag = stl2d_word_tag.tag;
   const CTag &str2d_tag = str2d_word_tag.tag;
   const CTag &n0_tag = n0_word_tag.tag;
   const CTag &n0ld_tag = n0ld_word_tag.tag;
   const CTag &n0l2d_tag = n0l2d_word_tag.tag;
   const CTag &n1_tag = n1_word_tag.tag;
   const CTag &n2_tag = n2_word_tag.tag;
   //const CTag &ht_tag = ht_word_tag.tag;
   //const CTag &ht2_tag = ht2_word_tag.tag;

   const int &st_label = st_index==-1 ? CDependencyLabel::NONE : item->label(st_index);
   const int &sth_label = sth_index==-1 ? CDependencyLabel::NONE : item->label(sth_index);
   const int &stld_label = stld_index==-1 ? CDependencyLabel::NONE : item->label(stld_index);
   const int &strd_label = strd_index==-1 ? CDependencyLabel::NONE : item->label(strd_index);
   const int &stl2d_label = stl2d_index==-1 ? CDependencyLabel::NONE : item->label(stl2d_index);
   const int &str2d_label = str2d_index==-1 ? CDependencyLabel::NONE : item->label(strd_index);
   const int &n0ld_label = n0ld_index==-1 ? CDependencyLabel::NONE : item->label(n0ld_index);
   const int &n0l2d_label = n0l2d_index==-1 ? CDependencyLabel::NONE : item->label(n0l2d_index);

   const int st_n0_dist = encodeLinkDistance(st_index, n0_index);

   const int st_rarity = st_index==-1?0:item->rightarity(st_index);
   const int st_larity = st_index==-1?0:item->leftarity(st_index);
   const int n0_larity = n0_index==-1?0:item->leftarity(n0_index);

   const CSetOfTags<CDependencyLabel> &st_rtagset = st_index==-1?CSetOfTags<CDependencyLabel>():item->righttagset(st_index);
   const CSetOfTags<CDependencyLabel> &st_ltagset = st_index==-1?CSetOfTags<CDependencyLabel>():item->lefttagset(st_index);
   const CSetOfTags<CDependencyLabel> &n0_ltagset = n0_index==-1?CSetOfTags<CDependencyLabel>():item->lefttagset(n0_index);

   st_word_tag_n0_word_tag.assign( st_word_tag, n0_word_tag );
   st_word_n0_word.assign( st_word, n0_word );

   // single
   if (st_index != -1) {
      cast_weights->getScore(depparser::CWeight::STw, st_word, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::STt, st_tag, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::STwt, st_word_tag, retval, m_eScoreAverage);
   }

   if (n0_index != -1) {
      cast_weights->getScore(depparser::CWeight::N0w, n0_word, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::N0t, n0_tag, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::N0wt, n0_word_tag, retval, m_eScoreAverage);
   }

   if (n1_index != -1) {
      cast_weights->getScore(depparser::CWeight::N1w, n1_word, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::N1t, n1_tag, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::N1wt, n1_word_tag, retval, m_eScoreAverage);
   }

   if (n2_index != -1) {
      cast_weights->getScore(depparser::CWeight::N2w, n2_word, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::N2t, n2_tag, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::N2wt, n2_word_tag, retval, m_eScoreAverage);
   }

   if (sth_index != -1) {
      cast_weights->getScore(depparser::CWeight::STHw, sth_word, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::STHt, sth_tag, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::STi, st_label, retval, m_eScoreAverage);
   }

   if (sthh_index != -1) {
      cast_weights->getScore(depparser::CWeight::STHHw, sthh_word, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::STHHt, sthh_tag, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::STHi, sth_label, retval, m_eScoreAverage);
   }

   if (stld_index != -1) {
      cast_weights->getScore(depparser::CWeight::STLDw, stld_word, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::STLDt, stld_tag, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::STLDi, stld_label, retval, m_eScoreAverage);
   }

   if (strd_index != -1) {
      cast_weights->getScore(depparser::CWeight::STRDw, strd_word, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::STRDt, strd_tag, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::STRDi, strd_label, retval, m_eScoreAverage);
   }

   if (n0ld_index != -1) {
      cast_weights->getScore(depparser::CWeight::N0LDw, n0ld_word, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::N0LDt, n0ld_tag, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::N0LDi, n0ld_label, retval, m_eScoreAverage);
   }

   if (stl2d_index != -1) {
      cast_weights->getScore(depparser::CWeight::STL2Dw, stl2d_word, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::STL2Dt, stl2d_tag, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::STL2Di, stl2d_label, retval, m_eScoreAverage);
   }

   if (str2d_index != -1) {
      cast_weights->getScore(depparser::CWeight::STR2Dw, str2d_word, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::STR2Dt, str2d_tag, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::STR2Di, str2d_label, retval, m_eScoreAverage);
   }

   if (n0l2d_index != -1) {
      cast_weights->getScore(depparser::CWeight::N0L2Dw, n0l2d_word, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::N0L2Dt, n0l2d_tag, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::N0L2Di, n0l2d_label, retval, m_eScoreAverage);
   }

   // s0 and n0
   if (st_index != -1) {
      cast_weights->getScore(depparser::CWeight::STwtN0wt, st_word_tag_n0_word_tag, retval, m_eScoreAverage);
      std::get<0>(word_word_tag) = st_word;
      std::get<1>(word_word_tag) = n0_word;
      std::get<2>(word_word_tag) = st_tag;
      cast_weights->getScore(depparser::CWeight::STwtN0w, word_word_tag, retval, m_eScoreAverage);
      std::get<0>(word_word_tag) = st_word;
      std::get<1>(word_word_tag) = n0_word;
      std::get<2>(word_word_tag) = n0_tag;
      cast_weights->getScore(depparser::CWeight::STwN0wt, word_word_tag, retval, m_eScoreAverage);
      std::get<0>(word_tag_tag) = st_word;
      std::get<1>(word_tag_tag) = st_tag;
      std::get<2>(word_tag_tag) = n0_tag;
      cast_weights->getScore(depparser::CWeight::STwtN0t, word_tag_tag, retval, m_eScoreAverage);
      std::get<0>(word_tag_tag) = n0_word;
      std::get<1>(word_tag_tag) = st_tag;
      std::get<2>(word_tag_tag) = n0_tag;
      cast_weights->getScore(depparser::CWeight::STtN0wt, word_tag_tag, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::STwN0w, st_word_n0_word, retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::STtN0t, CTagSet<CTag, 2>(encodeTags(st_tag,n0_tag)), retval, m_eScoreAverage);
   }

   if (st_index != -1 && n0_index != -1) {
      cast_weights->getScore(depparser::CWeight::N0tN1t, CTagSet<CTag, 2>(encodeTags(n0_tag,n1_tag)), retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::N0tN1tN2t, CTagSet<CTag, 3>(encodeTags(n0_tag,n1_tag,n2_tag)), retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::STtN0tN1t, CTagSet<CTag, 3>(encodeTags(st_tag,n0_tag,n1_tag)), retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::STtN0tN0LDt, CTagSet<CTag, 3>(encodeTags(st_tag,n0_tag,n0ld_tag)), retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::N0tN0LDtN0L2Dt, CTagSet<CTag, 3>(encodeTags(n0_tag,n0ld_tag,n0l2d_tag)), retval, m_eScoreAverage);
   }
   if (st_index!=-1) {
      cast_weights->getScore(depparser::CWeight::STHtSTtN0t, CTagSet<CTag, 3>(encodeTags(sth_tag,st_tag,n0_tag)), retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::STHHtSTHtSTt, CTagSet<CTag, 3>(encodeTags(sthh_tag, sth_tag,st_tag)), retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::STtSTLDtN0t, CTagSet<CTag, 3>(encodeTags(st_tag,stld_tag,n0_tag)), retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::STtSTLDtSTL2Dt, CTagSet<CTag, 3>(encodeTags(st_tag,stld_tag,stl2d_tag)), retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::STtSTRDtN0t, CTagSet<CTag, 3>(encodeTags(st_tag,strd_tag,n0_tag)), retval, m_eScoreAverage);
      cast_weights->getScore(depparser::CWeight::STtSTRDtSTR2Dt, CTagSet<CTag, 3>(encodeTags(st_tag,strd_tag,str2d_tag)), retval, m_eScoreAverage);
   }

   // distance
   if (st_index!=-1 && n0_index!=-1) {
      std::get<0>(word_int) = st_word;
      std::get<1>(word_int) = st_n0_dist;
      cast_weights->getScore(depparser::CWeight::STwd, word_int, retval, m_eScoreAverage);
      std::get<0>(tag_int) = st_tag;
      std::get<1>(tag_int) = st_n0_dist;
      cast_weights->getScore(depparser::CWeight::STtd, tag_int, retval, m_eScoreAverage);
      std::get<0>(word_int) = n0_word;
      std::get<1>(word_int) = st_n0_dist;
      cast_weights->getScore(depparser::CWeight::N0wd, word_int, retval, m_eScoreAverage);
      std::get<0>(tag_int) = n0_tag;
      std::get<1>(tag_int) = st_n0_dist;
      cast_weights->getScore(depparser::CWeight::N0td, tag_int, retval, m_eScoreAverage);
      std::get<0>(word_word_int) = st_word;
      std::get<1>(word_word_int) = n0_word;
      std::get<2>(word_word_int) = st_n0_dist;
      cast_weights->getScore(depparser::CWeight::STwN0wd, word_word_int, retval, m_eScoreAverage);
      std::get<0>(tag_tag_int) = st_tag;
      std::get<1>(tag_tag_int) = n0_tag;
      std::get<2>(tag_tag_int) = st_n0_dist;
      cast_weights->getScore(depparser::CWeight::STtN0td, tag_tag_int, retval, m_eScoreAverage);
   }

   // st arity
   if (st_index != -1) {
      std::get<0>(word_int) = st_word;
      std::get<1>(word_int) = st_rarity;
      cast_weights->getScore(depparser::CWeight::STwra, word_int, retval, m_eScoreAverage);
      std::get<0>(tag_int) = st_tag;
      std::get<1>(tag_int) = st_rarity;
      cast_weights->getScore(depparser::CWeight::STtra, tag_int, retval, m_eScoreAverage);
      std::get<0>(word_int) = st_word;
      std::get<1>(word_int) = st_larity;
      cast_weights->getScore(depparser::CWeight::STwla, word_int, retval, m_eScoreAverage);
      std::get<0>(tag_int) = st_tag;
      std::get<1>(tag_int) = st_larity;
      cast_weights->getScore(depparser::CWeight::STtla, tag_int, retval, m_eScoreAverage);
   }

   // n0 arity
   if (n0_index!=-1) {
      std::get<0>(word_int) = n0_word;
      std::get<1>(word_int) = n0_larity;
      cast_weights->getScore(depparser::CWeight::N0wla, word_int, retval, m_eScoreAverage);
      std::get<0>(tag_int) = n0_tag;
      std::get<1>(tag_int) = n0_larity;
      cast_weights->getScore(depparser::CWeight::N0tla, tag_int, retval, m_eScoreAverage);
   }

   // st labelset
   if (st_index != -1){
      std::get<0>(word_tagset) = st_word;
      std::get<1>(word_tagset) = st_rtagset;
      cast_weights->getScore(depparser::CWeight::STwrp, word_tagset, retval, m_eScoreAverage);
      std::get<0>(tag_tagset) = st_tag;
      std::get<1>(tag_tagset) = st_rtagset;
      cast_weights->getScore(depparser::CWeight::STtrp, tag_tagset, retval, m_eScoreAverage);

      std::get<0>(word_tagset) = st_word;
      std::get<1>(word_tagset) = st_ltagset;
      cast_weights->getScore(depparser::CWeight::STwlp, word_tagset, retval, m_eScoreAverage);
      std::get<0>(tag_tagset) = st_tag;
      std::get<1>(tag_tagset) = st_ltagset;
      cast_weights->getScore(depparser::CWeight::STtlp, tag_tagset, retval, m_eScoreAverage);
   }

   // n0 labelset
   if (n0_index != -1){
      std::get<0>(word_tagset) = n0_word;
      std::get<1>(word_tagset) = n0_ltagset;
      cast_weights->getScore(depparser::CWeight::N0wlp, word_tagset, retval, m_eScoreAverage);
      std::get<0>(tag_tagset) = n0_tag;
      std::get<1>(tag_tagset) = n0_ltagset;
      cast_weights->getScore(depparser::CWeight::N0tlp, tag_tagset, retval, m_eScoreAverage);
   }

   if (m_bCoNLL) {
      if (st_index!=-1) {
         if (!m_lCacheCoNLLLemma[st_index].empty()) cast_weights->getScore(depparser::CWeight::STl, m_lCacheCoNLLLemma[st_index], retval, m_eScoreAverage);
         if (m_lCacheCoNLLCPOS[st_index] != CGenericTag()) cast_weights->getScore(depparser::CWeight::STc, m_lCacheCoNLLCPOS[st_index], retval, m_eScoreAverage);
         for (unsigned i=0; i<m_lCacheCoNLLFeats[st_index].size(); ++i)
            cast_weights->getScore(depparser::CWeight::STf, m_lCacheCoNLLFeats[st_index][i], retval, m_eScoreAverage);
      } // if (st_index!=-1)

      if (n0_index!=-1) {
         if (!m_lCacheCoNLLLemma[n0_index].empty()) cast_weights->getScore(depparser::CWeight::N0l, m_lCacheCoNLLLemma[n0_index], retval, m_eScoreAverage);
         if (m_lCacheCoNLLCPOS[n0_index] != CGenericTag()) cast_weights->getScore(depparser::CWeight::N0c, m_lCacheCoNLLCPOS[n0_index], retval, m_eScoreAverage);
         for (unsigned i=0; i<m_lCacheCoNLLFeats[n0_index].size(); ++i)
            cast_weights->getScore(depparser::CWeight::N0f, m_lCacheCoNLLFeats[n0_index][i], retval, m_eScoreAverage);
      } // if (n0_index!=-1)

      if (n1_index!=-1) {
         if (!m_lCacheCoNLLLemma[n1_index].empty()) cast_weights->getScore(depparser::CWeight::N1l, m_lCacheCoNLLLemma[n1_index], retval, m_eScoreAverage);
         if (m_lCacheCoNLLCPOS[n1_index] != CGenericTag()) cast_weights->getScore(depparser::CWeight::N1c, m_lCacheCoNLLCPOS[n1_index], retval, m_eScoreAverage);
         for (unsigned i=0; i<m_lCacheCoNLLFeats[n1_index].size(); ++i)
            cast_weights->getScore(depparser::CWeight::N1f, m_lCacheCoNLLFeats[n1_index][i], retval, m_eScoreAverage);
      } // if (n1_index!=-1)
   }
}


void
CDepParser::updateStackScore(const CStateItem *item, const unsigned &action, const SCORE_TYPE amount, const int round) {
   const int &st_index = item->stackempty() ? -1 : item->stacktop(); // stack top
   const int &sth_index = st_index == -1 ? -1 : item->head(st_index); // stack top head
   const int &sthh_index = sth_index == -1 ? -1 : item->head(sth_index); // stack top head
   const int &stld_index = st_index == -1 ? -1 : item->leftdep(st_index); // leftmost dep of stack
   const int &strd_index = st_index == -1 ? -1 : item->rightdep(st_index); // rightmost dep st
   const int &stl2d_index = stld_index == -1 ? -1 : item->sibling(stld_index); // left 2ndmost dep of stack
   const int &str2d_index = strd_index == -1 ? -1 : item->sibling(strd_index); // right 2ndmost dep st
   const int &n0_index = item->size()==m_lCache.size() ? -1 : item->size(); // next
   assert(n0_index<static_cast<int>(m_lCache.size())); // the next index shouldn't exceed sentence
   const int &n0ld_index = n0_index==-1 ? -1 : item->leftdep(n0_index); // leftmost dep of next
   const int &n0l2d_index = n0ld_index==-1 ? -1 : item->sibling(n0ld_index); // leftmost dep of next
   //const int &ht_index = item->headstackempty() ? -1 : item->headstacktop(); // headstack
   //const int &ht2_index = item->headstacksize()<2 ? -1 : item->headstackitem(item->headstacksize()-2); // headstack 2nd
   const int n1_index = (n0_index != -1 && n0_index+1<m_lCache.size()) ? n0_index+1 : -1 ;
   const int n2_index = (n0_index != -1 && n0_index+2<m_lCache.size()) ? n0_index+2 : -1 ;
   //const int n3_index = (n0_index != -1 && n0_index+3<m_lCache.size()) ? n0_index+3 : -1 ;

   const CTaggedWord<CTag, TAG_SEPARATOR> &st_word_tag = st_index==-1 ? g_emptyTaggedWord : m_lCache[st_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &sth_word_tag = sth_index==-1 ? g_emptyTaggedWord : m_lCache[sth_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &sthh_word_tag = sthh_index==-1 ? g_emptyTaggedWord : m_lCache[sthh_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &stld_word_tag = stld_index==-1 ? g_emptyTaggedWord : m_lCache[stld_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &strd_word_tag = strd_index==-1 ? g_emptyTaggedWord : m_lCache[strd_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &stl2d_word_tag = stl2d_index==-1 ? g_emptyTaggedWord : m_lCache[stl2d_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &str2d_word_tag = str2d_index==-1 ? g_emptyTaggedWord : m_lCache[str2d_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &n0_word_tag = n0_index==-1 ? g_emptyTaggedWord : m_lCache[n0_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &n0ld_word_tag = n0ld_index==-1 ? g_emptyTaggedWord : m_lCache[n0ld_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &n0l2d_word_tag = n0l2d_index==-1 ? g_emptyTaggedWord : m_lCache[n0l2d_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &n1_word_tag = n1_index==-1 ? g_emptyTaggedWord : m_lCache[n1_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &n2_word_tag = n2_index==-1 ? g_emptyTaggedWord : m_lCache[n2_index];
   //const CTaggedWord<CTag, TAG_SEPARATOR> &ht_word_tag = ht_index==-1 ? g_emptyTaggedWord : m_lCache[ht_index];
   //const CTaggedWord<CTag, TAG_SEPARATOR> &ht2_word_tag = ht2_index==-1 ? g_emptyTaggedWord : m_lCache[ht2_index];

   const CWord &st_word = st_word_tag.word;
   const CWord &sth_word = sth_word_tag.word;
   const CWord &sthh_word = sthh_word_tag.word;
   const CWord &stld_word = stld_word_tag.word;
   const CWord &strd_word = strd_word_tag.word;
   const CWord &stl2d_word = stl2d_word_tag.word;
   const CWord &str2d_word = str2d_word_tag.word;
   const CWord &n0_word = n0_word_tag.word;
   const CWord &n0ld_word = n0ld_word_tag.word;
   const CWord &n0l2d_word = n0l2d_word_tag.word;
   const CWord &n1_word = n1_word_tag.word;
   const CWord &n2_word = n2_word_tag.word;
   //const CWord &ht_word = ht_word_tag.word;
   //const CWord &ht2_word = ht2_word_tag.word;

   const CTag &st_tag = st_word_tag.tag;
   const CTag &sth_tag = sth_word_tag.tag;
   const CTag &sthh_tag = sthh_word_tag.tag;
   const CTag &stld_tag = stld_word_tag.tag;
   const CTag &strd_tag = strd_word_tag.tag;
   const CTag &stl2d_tag = stl2d_word_tag.tag;
   const CTag &str2d_tag = str2d_word_tag.tag;
   const CTag &n0_tag = n0_word_tag.tag;
   const CTag &n0ld_tag = n0ld_word_tag.tag;
   const CTag &n0l2d_tag = n0l2d_word_tag.tag;
   const CTag &n1_tag = n1_word_tag.tag;
   const CTag &n2_tag = n2_word_tag.tag;
   //const CTag &ht_tag = ht_word_tag.tag;
   //const CTag &ht2_tag = ht2_word_tag.tag;

   const int &st_label = st_index==-1 ? CDependencyLabel::NONE : item->label(st_index);
   const int &sth_label = sth_index==-1 ? CDependencyLabel::NONE : item->label(sth_index);
   const int &stld_label = stld_index==-1 ? CDependencyLabel::NONE : item->label(stld_index);
   const int &strd_label = strd_index==-1 ? CDependencyLabel::NONE : item->label(strd_index);
   const int &stl2d_label = stl2d_index==-1 ? CDependencyLabel::NONE : item->label(stl2d_index);
   const int &str2d_label = str2d_index==-1 ? CDependencyLabel::NONE : item->label(strd_index);
   const int &n0ld_label = n0ld_index==-1 ? CDependencyLabel::NONE : item->label(n0ld_index);
   const int &n0l2d_label = n0l2d_index==-1 ? CDependencyLabel::NONE : item->label(n0l2d_index);

   const int st_n0_dist = encodeLinkDistance(st_index, n0_index);

   const int st_rarity = st_index==-1?0:item->rightarity(st_index);
   const int st_larity = st_index==-1?0:item->leftarity(st_index);
   const int n0_larity = n0_index==-1?0:item->leftarity(n0_index);

   const CSetOfTags<CDependencyLabel> &st_rtagset = st_index==-1?CSetOfTags<CDependencyLabel>():item->righttagset(st_index);
   const CSetOfTags<CDependencyLabel> &st_ltagset = st_index==-1?CSetOfTags<CDependencyLabel>():item->lefttagset(st_index);
   const CSetOfTags<CDependencyLabel> &n0_ltagset = n0_index==-1?CSetOfTags<CDependencyLabel>():item->lefttagset(n0_index);

   st_word_tag_n0_word_tag.assign( st_word_tag, n0_word_tag );
   st_word_n0_word.assign( st_word, n0_word );

   // single
   if (st_index != -1) {
      cast_weights->updateScore(depparser::CWeight::STw, st_word, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::STt, st_tag, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::STwt, st_word_tag, action, amount, round);
   }

   if (n0_index != -1) {
      cast_weights->updateScore(depparser::CWeight::N0w, n0_word, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::N0t, n0_tag, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::N0wt, n0_word_tag, action, amount, round);
   }

   if (n1_index != -1) {
      cast_weights->updateScore(depparser::CWeight::N1w, n1_word, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::N1t, n1_tag, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::N1wt, n1_word_tag, action, amount, round);
   }

   if (n2_index != -1) {
      cast_weights->updateScore(depparser::CWeight::N2w, n2_word, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::N2t, n2_tag, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::N2wt, n2_word_tag, action, amount, round);
   }

   if (sth_index != -1) {
      cast_weights->updateScore(depparser::CWeight::STHw, sth_word, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::STHt, sth_tag, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::STi, st_label, action, amount, round);
   }

   if (sthh_index != -1) {
      cast_weights->updateScore(depparser::CWeight::STHHw, sthh_word, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::STHHt, sthh_tag, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::STHi, sth_label, action, amount, round);
   }

   if (stld_index != -1) {
      cast_weights->updateScore(depparser::CWeight::STLDw, stld_word, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::STLDt, stld_tag, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::STLDi, stld_label, action, amount, round);
   }

   if (strd_index != -1) {
      cast_weights->updateScore(depparser::CWeight::STRDw, strd_word, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::STRDt, strd_tag, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::STRDi, strd_label, action, amount, round);
   }

   if (n0ld_index != -1) {
      cast_weights->updateScore(depparser::CWeight::N0LDw, n0ld_word, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::N0LDt, n0ld_tag, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::N0LDi, n0ld_label, action, amount, round);
   }

   if (stl2d_index != -1) {
      cast_weights->updateScore(depparser::CWeight::STL2Dw, stl2d_word, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::STL2Dt, stl2d_tag, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::STL2Di, stl2d_label, action, amount, round);
   }

   if (str2d_index != -1) {
      cast_weights->updateScore(depparser::CWeight::STR2Dw, str2d_word, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::STR2Dt, str2d_tag, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::STR2Di, str2d_label, action, amount, round);
   }

   if (n0l2d_index != -1) {
      cast_weights->updateScore(depparser::CWeight::N0L2Dw, n0l2d_word, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::N0L2Dt, n0l2d_tag, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::N0L2Di, n0l2d_label, action, amount, round);
   }

   // s0 and n0
   if (st_index != -1) {
      cast_weights->updateScore(depparser::CWeight::STwtN0wt, st_word_tag_n0_word_tag, action, amount, round);
      std::get<0>(word_word_tag) = st_word;
      std::get<1>(word_word_tag) = n0_word;
      std::get<2>(word_word_tag) = st_tag;
      cast_weights->updateScore(depparser::CWeight::STwtN0w, word_word_tag, action, amount, round);
      std::get<0>(word_word_tag) = st_word;
      std::get<1>(word_word_tag) = n0_word;
      std::get<2>(word_word_tag) = n0_tag;
      cast_weights->updateScore(depparser::CWeight::STwN0wt, word_word_tag, action, amount, round);
      std::get<0>(word_tag_tag) = st_word;
      std::get<1>(word_tag_tag) = st_tag;
      std::get<2>(word_tag_tag) = n0_tag;
      cast_weights->updateScore(depparser::CWeight::STwtN0t, word_tag_tag, action, amount, round);
      std::get<0>(word_tag_tag) = n0_word;
      std::get<1>(word_tag_tag) = st_tag;
      std::get<2>(word_tag_tag) = n0_tag;
      cast_weights->updateScore(depparser::CWeight::STtN0wt, word_tag_tag, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::STwN0w, st_word_n0_word, action, amount, round);
      cast_weights->updateScore(depparser::CWeight::STtN0t, CTagSet<CTag, 2>(encodeTags(st_tag,n0_tag)), action, amount, round);
   }

   if (st_index != -1 && n0_index != -1) {
      cast_weights->updateScore(depparser::CWeight::N0tN1t, CTagSet<CTag, 2>(encodeTags(n0_tag,n1_tag)), action, amount, round);
      cast_weights->updateScore(depparser::CWeight::N0tN1tN2t, CTagSet<CTag, 3>(encodeTags(n0_tag,n1_tag,n2_tag)), action, amount, round);
      cast_weights->updateScore(depparser::CWeight::STtN0tN1t, CTagSet<CTag, 3>(encodeTags(st_tag,n0_tag,n1_tag)), action, amount, round);
      cast_weights->updateScore(depparser::CWeight::STtN0tN0LDt, CTagSet<CTag, 3>(encodeTags(st_tag,n0_tag,n0ld_tag)), action, amount, round);
      cast_weights->updateScore(depparser::CWeight::N0tN0LDtN0L2Dt, CTagSet<CTag, 3>(encodeTags(n0_tag,n0ld_tag,n0l2d_tag)), action, amount, round);
   }
   if (st_index!=-1) {
      cast_weights->updateScore(depparser::CWeight::STHtSTtN0t, CTagSet<CTag, 3>(encodeTags(sth_tag,st_tag,n0_tag)), action, amount, round);
      cast_weights->updateScore(depparser::CWeight::STHHtSTHtSTt, CTagSet<CTag, 3>(encodeTags(sthh_tag, sth_tag,st_tag)), action, amount, round);
      cast_weights->updateScore(depparser::CWeight::STtSTLDtN0t, CTagSet<CTag, 3>(encodeTags(st_tag,stld_tag,n0_tag)), action, amount, round);
      cast_weights->updateScore(depparser::CWeight::STtSTLDtSTL2Dt, CTagSet<CTag, 3>(encodeTags(st_tag,stld_tag,stl2d_tag)), action, amount, round);
      cast_weights->updateScore(depparser::CWeight::STtSTRDtN0t, CTagSet<CTag, 3>(encodeTags(st_tag,strd_tag,n0_tag)), action, amount, round);
      cast_weights->updateScore(depparser::CWeight::STtSTRDtSTR2Dt, CTagSet<CTag, 3>(encodeTags(st_tag,strd_tag,str2d_tag)), action, amount, round);
   }

   // distance
   if (st_index!=-1 && n0_index!=-1) {
      std::get<0>(word_int) = st_word;
      std::get<1>(word_int) = st_n0_dist;
      cast_weights->updateScore(depparser::CWeight::STwd, word_int, action, amount, round);
      std::get<0>(tag_int) = st_tag;
      std::get<1>(tag_int) = st_n0_dist;
      cast_weights->updateScore(depparser::CWeight::STtd, tag_int, action, amount, round);
      std::get<0>(word_int) = n0_word;
      std::get<1>(word_int) = st_n0_dist;
      cast_weights->updateScore(depparser::CWeight::N0wd, word_int, action, amount, round);
      std::get<0>(tag_int) = n0_tag;
      std::get<1>(tag_int) = st_n0_dist;
      cast_weights->updateScore(depparser::CWeight::N0td, tag_int, action, amount, round);
      std::get<0>(word_word_int) = st_word;
      std::get<1>(word_word_int) = n0_word;
      std::get<2>(word_word_int) = st_n0_dist;
      cast_weights->updateScore(depparser::CWeight::STwN0wd, word_word_int, action, amount, round);
      std::get<0>(tag_tag_int) = st_tag;
      std::get<1>(tag_tag_int) = n0_tag;
      std::get<2>(tag_tag_int) = st_n0_dist;
      cast_weights->updateScore(depparser::CWeight::STtN0td, tag_tag_int, action, amount, round);
   }

   // st arity
   if (st_index != -1) {
      std::get<0>(word_int) = st_word;
      std::get<1>(word_int) = st_rarity;
      cast_weights->updateScore(depparser::CWeight::STwra, word_int, action, amount, round);
      std::get<0>(tag_int) = st_tag;
      std::get<1>(tag_int) = st_rarity;
      cast_weights->updateScore(depparser::CWeight::STtra, tag_int, action, amount, round);
      std::get<0>(word_int) = st_word;
      std::get<1>(word_int) = st_larity;
      cast_weights->updateScore(depparser::CWeight::STwla, word_int, action, amount, round);
      std::get<0>(tag_int) = st_tag;
      std::get<1>(tag_int) = st_larity;
      cast_weights->updateScore(depparser::CWeight::STtla, tag_int, action, amount, round);
   }

   // n0 arity
   if (n0_index!=-1) {
      std::get<0>(word_int) = n0_word;
      std::get<1>(word_int) = n0_larity;
      cast_weights->updateScore(depparser::CWeight::N0wla, word_int, action, amount, round);
      std::get<0>(tag_int) = n0_tag;
      std::get<1>(tag_int) = n0_larity;
      cast_weights->updateScore(depparser::CWeight::N0tla, tag_int, action, amount, round);
   }

   // st labelset
   if (st_index != -1){
      std::get<0>(word_tagset) = st_word;
      std::get<1>(word_tagset) = st_rtagset;
      cast_weights->updateScore(depparser::CWeight::STwrp, word_tagset, action, amount, round);
      std::get<0>(tag_tagset) = st_tag;
      std::get<1>(tag_tagset) = st_rtagset;
      cast_weights->updateScore(depparser::CWeight::STtrp, tag_tagset, action, amount, round);

      std::get<0>(word_tagset) = st_word;
      std::get<1>(word_tagset) = st_ltagset;
      cast_weights->updateScore(depparser::CWeight::STwlp, word_tagset, action, amount, round);
      std::get<0>(tag_tagset) = st_tag;
      std::get<1>(tag_tagset) = st_ltagset;
      cast_weights->updateScore(depparser::CWeight::STtlp, tag_tagset, action, amount, round);
   }

   // n0 labelset
   if (n0_index != -1){
      std::get<0>(word_tagset) = n0_word;
      std::get<1>(word_tagset) = n0_ltagset;
      cast_weights->updateScore(depparser::CWeight::N0wlp, word_tagset, action, amount, round);
      std::get<0>(tag_tagset) = n0_tag;
      std::get<1>(tag_tagset) = n0_ltagset;
      cast_weights->updateScore(depparser::CWeight::N0tlp, tag_tagset, action, amount, round);
   }

   if (m_bCoNLL) {
      if (st_index!=-1) {
         if (!m_lCacheCoNLLLemma[st_index].empty()) cast_weights->updateScore(depparser::CWeight::STl, m_lCacheCoNLLLemma[st_index], action, amount, round);
         if (m_lCacheCoNLLCPOS[st_index] != CGenericTag()) cast_weights->updateScore(depparser::CWeight::STc, m_lCacheCoNLLCPOS[st_index], action, amount, round);
         for (unsigned i=0; i<m_lCacheCoNLLFeats[st_index].size(); ++i)
            cast_weights->updateScore(depparser::CWeight::STf, m_lCacheCoNLLFeats[st_index][i], action, amount, round);
      } // if (st_index!=-1)

      if (n0_index!=-1) {
         if (!m_lCacheCoNLLLemma[n0_index].empty()) cast_weights->updateScore(depparser::CWeight::N0l, m_lCacheCoNLLLemma[n0_index], action, amount, round);
         if (m_lCacheCoNLLCPOS[n0_index] != CGenericTag()) cast_weights->updateScore(depparser::CWeight::N0c, m_lCacheCoNLLCPOS[n0_index], action, amount, round);
         for (unsigned i=0; i<m_lCacheCoNLLFeats[n0_index].size(); ++i)
            cast_weights->updateScore(depparser::CWeight::N0f, m_lCacheCoNLLFeats[n0_index][i], action, amount, round);
      } // if (n0_index!=-1)

      if (n1_index!=-1) {
         if (!m_lCacheCoNLLLemma[n1_index].empty()) cast_weights->updateScore(depparser::CWeight::N1l, m_lCacheCoNLLLemma[n1_index], action, amount, round);
         if (m_lCacheCoNLLCPOS[n1_index] != CGenericTag()) cast_weights->updateScore(depparser::CWeight::N1c, m_lCacheCoNLLCPOS[n1_index], action, amount, round);
         for (unsigned i=0; i<m_lCacheCoNLLFeats[n1_index].size(); ++i)
            cast_weights->updateScore(depparser::CWeight::N1f, m_lCacheCoNLLFeats[n1_index][i], action, amount, round);
      } // if (n1_index!=-1)
   }
}

/*---------------------------------------------------------------
 *
 * getGlobalScore - get the score of a parse tree
 *
 * Inputs: parse graph
 *
 *---------------------------------------------------------------*/

SCORE_TYPE CDepParser::getGlobalScore(const CDependencyParse &parsed) {
   THROW("depparser.cpp: getGlobalScore unsupported");
}

/*---------------------------------------------------------------
 *
 * updateScores - update the score std::vector 
 *
 * This method is different from updateScoreVector in that
 * 1. It is for external call
 * 2. The tagging sequences for parsed and correct may differ
 *
 * Inputs: the parsed and the correct example
 *
 *---------------------------------------------------------------*/

void CDepParser::updateScores(const CDependencyParse & parsed , const CDependencyParse & correct , int round ) {
   THROW("depparser.cpp: updateScores unsupported");
}

/*---------------------------------------------------------------
 *
 * updateScoreForState - update a single positive or negative outout
 *
 *--------------------------------------------------------------*/

inline void CDepParser::updateScoreForState( const CStateItem &from, const CStateItem *outout , const SCORE_TYPE &amount ) {
   CStateItem item(&m_lCache);
   item = from;
   while ( item != *outout ) {
      const unsigned action = item.FollowMove( outout );
      updateStackScore(&item, action, amount, m_nTrainingRound);
#ifdef LABELED
//      if ( action::getLabel(action) )
//         updateStackScore(&item, action::getUnlabeledAction(action), amount, m_nTrainingRound);
#endif
      item.Move( action );
   }
}

/*---------------------------------------------------------------
 *
 * updateScoresForStates - update scores for states
 *
 *--------------------------------------------------------------*/

void CDepParser::updateScoresForStates( const CStateItem *outout , const CStateItem *correct , SCORE_TYPE amount_add, SCORE_TYPE amount_subtract ) {

   // do not update those steps where they are correct
   CStateItem item(&m_lCache);
   item.clear();
   while ( item != *outout ) {
      const unsigned action = item.FollowMove( outout );
      const unsigned correct_action = item.FollowMove( correct );
      if ( action == correct_action )
         item.Move( action );
      else break;
   }

   // for the necessary information for the correct and outout parsetree
   updateScoreForState( item, correct , amount_add ) ;
   updateScoreForState( item, outout , amount_subtract ) ;

   m_nTotalErrors++;
}

/*---------------------------------------------------------------
 *
 * reduce - helper function
 *
 *--------------------------------------------------------------*/

inline void CDepParser::reduce( const CStateItem *item, const CPackedScoreType<SCORE_TYPE, action::MAX> &scores ) {
   action::CScoredAction scoredaction;
   // update stack score
   scoredaction.action = action::REDUCE;
   scoredaction.score = item->score + scores[scoredaction.action]; 
   m_Beam->insertItem(&scoredaction);
}

/*---------------------------------------------------------------
 *
 * arcleft - helping function
 *
 *--------------------------------------------------------------*/

inline void CDepParser::arcleft( const CStateItem *item, const CPackedScoreType<SCORE_TYPE, action::MAX> &scores ) {
   action::CScoredAction scoredaction;
   unsigned label;
#ifdef LABELED
   for (label=CDependencyLabel::FIRST; label<CDependencyLabel::COUNT; ++label) {
      if ( !m_weights->rules() || canAssignLabel(m_lCache, item->size(), item->stacktop(), label) ) {
         scoredaction.action = action::encodeAction(action::ARC_LEFT, label);
         scoredaction.score = item->score + scores[scoredaction.action];
                               //+scores[action::ARC_LEFT];
         m_Beam->insertItem(&scoredaction);
      }
   }
#else
   scoredaction.action = action::ARC_LEFT;
   scoredaction.score = item->score + scores[scoredaction.action];
      m_Beam->insertItem(&scoredaction);
#endif
}

/*---------------------------------------------------------------
 *
 * arcright - helping function
 *
 *--------------------------------------------------------------*/

inline void CDepParser::arcright( const CStateItem *item, const CPackedScoreType<SCORE_TYPE, action::MAX> &scores ) {
   action::CScoredAction scoredaction;
   unsigned label;
#ifdef LABELED
   for (label=CDependencyLabel::FIRST; label<CDependencyLabel::COUNT; ++label) {
      if ( !m_weights->rules() || canAssignLabel(m_lCache, item->stacktop(), item->size(), label) ) {
         scoredaction.action = action::encodeAction(action::ARC_RIGHT, label);
         scoredaction.score = item->score + scores[scoredaction.action];
                              //+scores[action::ARC_RIGHT];
         m_Beam->insertItem(&scoredaction);
      }
   }
#else
   scoredaction.action = action::ARC_RIGHT;
   scoredaction.score = item->score + scores[scoredaction.action];
   m_Beam->insertItem(&scoredaction);
#endif
}

/*---------------------------------------------------------------
 *
 * shift - help function
 *
 *--------------------------------------------------------------*/

inline void CDepParser::shift( const CStateItem *item, const CPackedScoreType<SCORE_TYPE, action::MAX> &scores ) {
   action::CScoredAction scoredaction;
   // update stack score
   scoredaction.action = action::SHIFT;
   scoredaction.score = item->score + scores[scoredaction.action];
   m_Beam->insertItem(&scoredaction);
}

/*---------------------------------------------------------------
 *
 * poproot - help function
 *
 *--------------------------------------------------------------*/

inline void CDepParser::poproot( const CStateItem *item, const CPackedScoreType<SCORE_TYPE, action::MAX> &scores ) {
   action::CScoredAction scoredaction;
   // update stack score
   scoredaction.action = action::POP_ROOT;
   scoredaction.score = item->score + scores[scoredaction.action];
   m_Beam->insertItem(&scoredaction);
}
   

/*---------------------------------------------------------------
 *
 * work - the working process shared by training and parsing
 *
 * Returns: makes a new instance of CDependencyParse 
 *
 *--------------------------------------------------------------*/

void CDepParser::work( const bool bTrain , const CTwoStringVector &sentence , CDependencyParse *retval , const CDependencyParse &correct , int nBest , SCORE_TYPE *scores ) {

#ifdef DEBUG
   clock_t total_start_time = clock();
#endif
   int index;
   const int length = sentence.size() ; 

   const CStateItem *pGenerator = 0;
   CStateItem pCandidate(&m_lCache) ;

   // used only for training
   bool bCorrect = false ;  // used in learning for early update
   bool bContradictsRules = false;
   CStateItem correctState(&m_lCache) ;
   CPackedScoreType<SCORE_TYPE, action::MAX> packed_scores;

   ASSERT(length<MAX_SENTENCE_SIZE, "The size of the sentence is larger than the system configuration.");

   TRACE("Initialising the decoding process...") ;
   // initialise word cache
   bContradictsRules = false;
   m_lCache.clear();
   for ( index=0; index<length; ++index ) {
      m_lCache.push_back( CTaggedWord<CTag, TAG_SEPARATOR>(sentence[index].first, m_wordTokenizer, sentence[index].second) );
      // filter std::cout training examples with rules
      if (bTrain && m_weights->rules()) {
         // the root
         if ( correct[index].head == DEPENDENCY_LINK_NO_HEAD && canBeRoot(m_lCache[index].tag.code())==false) {
            TRACE("Rule contradiction: " << m_lCache[index].tag.code() << " can be root.");
            bContradictsRules = true;
         }
         // head left
         if ( correct[index].head < index && hasLeftHead(m_lCache[index].tag.code())==false) {
            TRACE("Rule contradiction: " << m_lCache[index].tag.code() << " has left head.");
            bContradictsRules = true;
         }
         // head right
         if ( correct[index].head > index && hasRightHead(m_lCache[index].tag.code())==false) {
            TRACE("Rule contradiction: " << m_lCache[index].tag.code() << " has right head.");
            bContradictsRules = true;
         }
      }
   }

   // initialise agenda
   m_Agenda->clear();
   pCandidate.clear();                          // restore state using clean
   m_Agenda->pushCandidate(&pCandidate);           // and push it back
   m_Agenda->nextRound();                       // as the generator item
   if (bTrain) correctState.clear();

   // verifying supertags
   if (m_supertags) {
      ASSERT(m_supertags->getSentenceSize()==length, "Sentence size does not match supertags size");
   }

#ifdef LABELED
   //unsigned long label;
   m_lCacheLabel.clear();
   if (bTrain) {
      for (index=0; index<length; ++index) {
         m_lCacheLabel.push_back(CDependencyLabel(correct[index].label));
         if (m_weights->rules() && !canAssignLabel(m_lCache, correct[index].head, index, m_lCacheLabel[index])) {
            TRACE("Rule contradiction: " << correct[index].label << " on link head " << m_lCache[correct[index].head].tag.code() << " dep " << m_lCache[index].tag.code());
            bContradictsRules = true;
         }
      }
   }
#endif

   // skip the training example if contradicts
   if (bTrain && m_weights->rules() && bContradictsRules) {
      std::cout << "Skipping training example because it contradicts rules..." <<std::endl;
      return;
   }

   TRACE("Decoding started"); 
   // loop with the next word to process in the sentence
   for (index=0; index<length*2; ++index) {
      
      if (bTrain) bCorrect = false ; 

      // none can this find with pruning ???
      if (m_Agenda->generatorSize() == 0) {
         WARNING("parsing failed"); 
         return;
      }

      pGenerator = m_Agenda->generatorStart();
      // iterate generators
      for (int j=0; j<m_Agenda->generatorSize(); ++j) {

         // for the state items that already contain all words
         m_Beam->clear();
         packed_scores.reset();
         getStackScore(pGenerator, packed_scores);
         if ( pGenerator->size() == length ) {
            assert( pGenerator->stacksize() != 0 );
            if ( pGenerator->stacksize()>1 ) {
#ifdef FRAGMENTED_TREE
               if (pGenerator->head(pGenerator->stacktop()) == DEPENDENCY_LINK_NO_HEAD)
                  poproot(pGenerator, packed_scores);
               else
#endif
               reduce(pGenerator, packed_scores) ;
            }
            else {
               poproot(pGenerator, packed_scores); 
            }
         }
         // for the state items that still need more words
         else {  
            if ( !pGenerator->afterreduce() ) { // there are many ways when there are many arcrighted items on the stack and the root need arcleft. force this.               
               if ( 
#ifndef FRAGMENTED_TREE
                    ( pGenerator->size() < length-1 || pGenerator->stackempty() ) && // keep only one global root
#endif
                    ( pGenerator->stackempty() || m_supertags == 0 || m_supertags->canShift( pGenerator->size() ) ) && // supertags
                    ( pGenerator->stackempty() || !m_weights->rules() || canBeRoot( m_lCache[pGenerator->size()].tag.code() ) || hasRightHead(m_lCache[pGenerator->size()].tag.code()) ) // rules
                  ) {
                  shift(pGenerator, packed_scores) ;
               }
            }
            if ( !pGenerator->stackempty() ) {
               if ( 
#ifndef FRAGMENTED_TREE
                    ( pGenerator->size() < length-1 || pGenerator->headstacksize() == 1 ) && // one root
#endif
                    ( m_supertags == 0 || m_supertags->canArcRight(pGenerator->stacktop(), pGenerator->size()) ) && // supertags conform to this action
                    ( !m_weights->rules() || hasLeftHead(m_lCache[pGenerator->size()].tag.code()) ) // rules
                  ) { 
                  arcright(pGenerator, packed_scores) ;
               }
            }
            if ( (!m_bCoNLL && !pGenerator->stackempty()) ||
                 (m_bCoNLL && pGenerator->stacksize()>1) // make sure that for conll the first item is not popped
               ) {
               if ( pGenerator->head( pGenerator->stacktop() ) != DEPENDENCY_LINK_NO_HEAD ) {
                  reduce(pGenerator, packed_scores) ;
               }
               else {
                  if ( (m_supertags == 0 || m_supertags->canArcLeft(pGenerator->size(), pGenerator->stacktop())) && // supertags
                       (!m_weights->rules() || hasRightHead(m_lCache[pGenerator->stacktop()].tag.code())) // rules
                     ) {
                     arcleft(pGenerator, packed_scores) ;
                  }
               }
            }
         }

         // insert item
         for (unsigned i=0; i<m_Beam->size(); ++i) {
            pCandidate = *pGenerator;
            pCandidate.score = m_Beam->item(i)->score;
            pCandidate.Move( m_Beam->item(i)->action );
            m_Agenda->pushCandidate(&pCandidate);
         }

         if (bTrain && *pGenerator == correctState) {
            bCorrect = true ;
         }
         pGenerator = m_Agenda->generatorNext() ;

      }
      // when we are doing training, we need to consider the standard move and update
      if (bTrain) {
#ifdef EARLY_UPDATE
         if (!bCorrect) {
            TRACE("Error at the "<<correctState.size()<<"th word; total is "<<correct.size())
            updateScoresForStates(m_Agenda->bestGenerator(), &correctState, 1, -1) ; 
#ifndef LOCAL_LEARNING
            return ;
#else
            m_Agenda->clearCandidates();
            m_Agenda->pushCandidate(&correctState);
#endif
         }
#endif

         if (bCorrect) {
#ifdef LABELED
            correctState.StandardMoveStep(correct, m_lCacheLabel);
#else
            correctState.StandardMoveStep(correct);
#endif
         }
#ifdef LOCAL_LEARNING
         ++m_nTrainingRound; // each training round is one transition-action
#endif
      } 
      
      m_Agenda->nextRound(); // move round
   }

   if (bTrain) {
      correctState.StandardFinish(); // pop the root that is left
      // then make sure that the correct item is stack top finally
      if ( *(m_Agenda->bestGenerator()) != correctState ) {
         TRACE("The best item is not the correct one")
         updateScoresForStates(m_Agenda->bestGenerator(), &correctState, 1, -1) ; 
         return ;
      }
   } 

   TRACE("Outputing sentence");
   m_Agenda->sortGenerators();
   for (int i=0; i<std::min(m_Agenda->generatorSize(), nBest); ++i) {
      pGenerator = m_Agenda->generator(i) ; 
      if (pGenerator) {
         pGenerator->GenerateTree( sentence , retval[i] ) ; 
         if (scores) scores[i] = pGenerator->score;
      }
   }
   TRACE("Done, the highest score is: " << m_Agenda->bestGenerator()->score ) ;
   TRACE("The total time spent: " << double(clock() - total_start_time)/CLOCKS_PER_SEC) ;
}

/*---------------------------------------------------------------
 *
 * parse - do dependency parsing to a sentence
 *
 * Returns: makes a new instance of CDependencyParse 
 *
 *--------------------------------------------------------------*/

void CDepParser::parse( const CTwoStringVector &sentence , CDependencyParse *retval , int nBest , SCORE_TYPE *scores ) {

   CDependencyParse empty ;

   assert( !m_bCoNLL );

   for (int i=0; i<nBest; ++i) {
      // clear the outout sentences
      retval[i].clear();
      if (scores) scores[i] = 0; //pGenerator->score;
   }

   work(false, sentence, retval, empty, nBest, scores ) ;

}

/*---------------------------------------------------------------
 *
 * train - train the models with an example
 *
 *---------------------------------------------------------------*/

void CDepParser::train( const CDependencyParse &correct , int round ) {

   CTwoStringVector sentence ;
   CDependencyParse outout ; 

   assert( !m_bCoNLL );
#ifndef FRAGMENTED_TREE
   assert( IsProjectiveDependencyTree(correct) ) ;
#endif
   UnparseSentence( &correct, &sentence ) ;

   // The following code does update for each processing stage
#ifndef LOCAL_LEARNING
   ++m_nTrainingRound;
   ASSERT(m_nTrainingRound == round, "Training round error") ;
#endif
   //std::cout << "round=" << round << std::endl;
   //if (round > 1)
      //cast_weights->debugUsage();

   work( true , sentence , &outout , correct , 1 , 0 ) ; 

}

/*---------------------------------------------------------------
 *
 * extract_features - extract features from an example (counts recorded to parser model as weights)
 *
 *---------------------------------------------------------------*/

void CDepParser::extract_features(const CDependencyParse &input) {

   CStateItem item(&m_lCache);
   CStateItem tmp(&m_lCache);
   unsigned action;

   // word and pos
   m_lCache.clear();
#ifdef LABELED
   m_lCacheLabel.clear();
#endif
   for (int i=0; i<input.size(); ++i) {
      m_lCache.push_back(CTaggedWord<CTag, TAG_SEPARATOR>(input[i].word, m_wordTokenizer, input[i].tag));
#ifdef LABELED
   m_lCacheLabel.push_back(CDependencyLabel(input[i].label));
#endif
   }

   // make standard item
   item.clear();
   for (int i=0; i<input.size() * 2; ++i) {
#ifdef LABELED
   item.StandardMoveStep(input, m_lCacheLabel);
#else
   item.StandardMoveStep(input);
#endif
   }

   // extract feature now with another step less efficient yet easier here
   tmp.clear();
   while (tmp != item) {
      action = tmp.FollowMove(&item );
      updateStackScore(&tmp, action, 1, 0);
      tmp.Move(action);
   }
}

/*---------------------------------------------------------------
 *
 * initCoNLLCache
 *
 *---------------------------------------------------------------*/

template<typename CCoNLLInputOrOutput>
void CDepParser::initCoNLLCache( const CCoNLLInputOrOutput &sentence ) {
   m_lCacheCoNLLLemma.resize(sentence.size());
   m_lCacheCoNLLCPOS.resize(sentence.size());
   m_lCacheCoNLLFeats.resize(sentence.size());
   for (unsigned i=0; i<sentence.size(); ++i) {
      m_lCacheCoNLLLemma[i].load(_conll_or_empty(sentence.at(i).lemma), m_lemmaTokenizer);
      m_lCacheCoNLLCPOS[i].load(_conll_or_empty(sentence.at(i).ctag), m_conllcposTokenizer);
      m_lCacheCoNLLFeats[i].clear();
      if (sentence.at(i).feats != "_")
         readCoNLLFeats(m_lCacheCoNLLFeats[i], m_conllfeatsTokenizer, sentence.at(i).feats);
   }
}

/*---------------------------------------------------------------
 *
 * parse_conll - do dependency parsing to a sentence
 *
 * Returns: makes a new instance of CDependencyParse 
 *
 *--------------------------------------------------------------*/

void CDepParser::parse_conll( const CCoNLLInput &sentence , CCoNLLOutput *retval , int nBest, SCORE_TYPE *scores ) {

   CDependencyParse empty ;
   CTwoStringVector input ;
   CDependencyParse outout[AGENDA_SIZE] ;

   assert( m_bCoNLL ) ;

   initCoNLLCache(sentence);

   sentence.toTwoStringVector(input);

   for (int i=0; i<nBest; ++i) {
      // clear the outout sentences
      retval[i].clear();
      outout[i].clear();
      if (scores) scores[i] = 0; //pGenerator->score;
   }

   work(false, input, outout, empty, nBest, scores ) ;

   for (int i=0; i<std::min(nBest, m_Agenda->generatorSize()); ++i) {
      // now make the conll format stype outout
      retval[i].fromCoNLLInput(sentence);
      retval[i].copyDependencyHeads(outout[i]);
   }

}

/*---------------------------------------------------------------
 *
 * train_conll - train the models with an example
 *
 *---------------------------------------------------------------*/

void CDepParser::train_conll( const CCoNLLOutput &correct , int round ) {

   CTwoStringVector sentence ;
   CDependencyParse outout ; 
   CDependencyParse reference ;

   assert( m_bCoNLL ) ;
   assert( IsProjectiveDependencyTree(correct) ) ;

   initCoNLLCache(correct);

   correct.toDependencyTree( reference );
   UnparseSentence( &reference, &sentence ) ;

   // The following code does update for each processing stage
   m_nTrainingRound = round ;
   work( true , sentence , &outout , reference , 1 , 0 ) ; 

}

/*---------------------------------------------------------------
 *
 * extract_features_conll - extract features from an example
 *
 *---------------------------------------------------------------*/

void CDepParser::extract_features_conll( const CCoNLLOutput &input) {

   CDependencyParse dep;
   assert( m_bCoNLL );
   initCoNLLCache( input );
   input.toDependencyTree( dep );
   extract_features(dep);

}


void CDepParser::finishtraining() {
  cast_weights->computeAverageFeatureWeights(m_nTrainingRound);
  cast_weights->saveScores();
}
