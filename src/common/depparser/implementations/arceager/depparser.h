// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * depparser.h - the definitions for the dependency parser.     *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.8                         *
 *                                                              *
 ****************************************************************/

#ifndef _DEPPARSER_IMPL_H
#define _DEPPARSER_IMPL_H

#include "depparser_base.h"

/*===============================================================
 *
 * Global items
 *
 *==============================================================*/

namespace TARGET_LANGUAGE {

namespace depparser {

#include "state.h"

} // namespace depparser
}

#include "depparser_weight.h"

namespace TARGET_LANGUAGE {

/*===============================================================
 *
 * CDepParser - the dependency parser for English
 *
 *==============================================================*/

class CDepParser : public CDepParserBase {
private:
   CAgendaBeam<depparser::CStateItem> *m_Agenda;
   CAgendaSimple<depparser::action::CScoredAction> *m_Beam;

   // caches for input
   std::vector<CTaggedWord<CTag, TAG_SEPARATOR>> m_lCache;
#ifdef LABELED
   std::vector<CDependencyLabel> m_lCacheLabel;
#endif
   std::vector<CLemma> m_lCacheCoNLLLemma; // conll
   std::vector<CGenericTag> m_lCacheCoNLLCPOS; // conll
   std::vector<std::vector<CGenericTag>> m_lCacheCoNLLFeats; // conll

   CStringTokenizer m_wordTokenizer;
   CStringTokenizer m_lemmaTokenizer;
   CStringTokenizer m_conllcposTokenizer;
   CStringTokenizer m_conllfeatsTokenizer;

   CTwoTaggedWords st_word_tag_n0_word_tag ;
   CTwoWords st_word_n0_word ;

   std::tuple<CWord, int> word_int;
   std::tuple<CTag, int> tag_int;
   std::tuple<CWord, CTag, CTag> word_tag_tag;
   std::tuple<CWord, CWord, CTag> word_word_tag;
   std::tuple<CWord, CWord, int> word_word_int;
   std::tuple<CTag, CTag, int> tag_tag_int;
   std::tuple<CWord, CSetOfTags<CDependencyLabel>> word_tagset;
   std::tuple<CTag, CSetOfTags<CDependencyLabel>> tag_tagset;

   int m_nTrainingRound;
   unsigned int m_nTotalErrors;
   //bool m_bScoreModified;
   int m_nScoreIndex;

public:
   CDepParser(const std::string &sInputPath, bool bTrain, bool bCoNLL=false);
   CDepParser(depparser::CWeight<depparser::SCORE_TYPE> *weights, bool bCoNLL=false);
   ~CDepParser();

public:
   void parse( const CTwoStringVector &sentence , CDependencyParse *retval , int nBest=1 , depparser::SCORE_TYPE *scores=0 ) ;
   void train( const CDependencyParse &correct , int round ) ;
   void extract_features( const CDependencyParse &input ) ;

   void parse_conll( const CCoNLLInput &sentence , CCoNLLOutput *retval , int nBest=1, depparser::SCORE_TYPE *scores=0 ) ;
   void train_conll( const CCoNLLOutput &correct , int round ) ;
   void extract_features_conll( const CCoNLLOutput &input ) ;

   void finishtraining();
   depparser::SCORE_TYPE getGlobalScore(const CDependencyParse &parsed);
   void updateScores(const CDependencyParse &parse, const CDependencyParse &correct, int round=0);

   inline unsigned int getTotalTrainingErrors(void) const { return m_nTotalErrors; }

private:
   enum SCORE_UPDATE {eAdd=0, eSubtract};

   template<typename CCoNLLInputOrOutput>
   void initCoNLLCache( const CCoNLLInputOrOutput &sentence ) ;

   void work( const bool bTrain, const CTwoStringVector &sentence , CDependencyParse *retval, const CDependencyParse &correct, int nBest, depparser::SCORE_TYPE *scores ) ;

   void getOrUpdateStackScore( const depparser::CStateItem *item, CPackedScoreType<depparser::SCORE_TYPE, depparser::action::MAX> &retval, const unsigned &action, depparser::SCORE_TYPE amount=0, int round=0 );

   // update the built-in weight std::vector for this feature object specifically
   void updateScoresForStates( const depparser::CStateItem *outout , const depparser::CStateItem *correct , depparser::SCORE_TYPE amount_add , depparser::SCORE_TYPE amount_subtract ) ;
   void updateScoreForState( const depparser::CStateItem &from, const depparser::CStateItem *outout , const depparser::SCORE_TYPE &amount ) ;


   // helper method
   void reduce( const depparser::CStateItem *item, const CPackedScoreType<depparser::SCORE_TYPE, depparser::action::MAX> &scores ) ;
   void shift( const depparser::CStateItem *item, const CPackedScoreType<depparser::SCORE_TYPE, depparser::action::MAX> &scores) ;
   void arcleft( const depparser::CStateItem *item, const CPackedScoreType<depparser::SCORE_TYPE, depparser::action::MAX> &scores) ;
   void arcright( const depparser::CStateItem *item, const CPackedScoreType<depparser::SCORE_TYPE, depparser::action::MAX> &scores) ;
   void poproot( const depparser::CStateItem *item, const CPackedScoreType<depparser::SCORE_TYPE, depparser::action::MAX> &scores) ;
};

} // namespace TARGET_LANGUAGE

#endif
