// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * weight.cpp - the weight definition method                    *
 *                                                              *
 * Part of the dependency parser of english                     *
 *                                                              *
 * Yue Zhang                                                    *
 * 2007.8                                                       *
 *                                                              *
 ****************************************************************/

#include "depparser_weight.h"

using namespace TARGET_LANGUAGE;
using namespace TARGET_LANGUAGE::depparser;

/*---------------------------------------------------------------
 *
 * loadScores - load scores from the file specified at constructor
 *              currently this uses database, but it can be modified
 *         
 * Affects: m_bScoreModified, clearing it. 
 *
 *--------------------------------------------------------------*/

void TARGET_LANGUAGE::depparser::CWeight::loadScores() {
   clock_t time_start = clock();
   std::cout<<"Loading scores..."; std::cout.flush();
   std::ifstream file ; 
   std::string s;
   file.open(m_sRecordPath.c_str()) ;

   if (!file.is_open()) {
      std::cout << " empty." << std::endl; return;
   }

#ifdef LABELED
   getline(file, s);
   ASSERT(s=="Dependency labels:", "Dependency labels not found in model file") ;
   getline(file, s);
   std::istringstream iss(s);
   CDependencyLabel label;
   while(iss >> label);
   getline(file, s);
   ASSERT(s=="", "No empty line after the dependency labels") ;
#endif
   iterate_templates(file >>,;);

   getline(file, s);
   if (s=="Rules=1") {
      setRules(true);
   }
   else {
      ASSERT(s=="Rules=0", "Rules flag not found from the model file");
      setRules(false);
   }

   file.close() ;
   std::cout << " done. (" << double(clock()-time_start)/CLOCKS_PER_SEC << "s)" << std::endl;
}

/*---------------------------------------------------------------
 *
 * saveScores - save scores back to database
 *
 * This method is called by the destructor is m_bScoreModified
 * is true.
   std::cout<<"done."<<std::endl;
 *
 *--------------------------------------------------------------*/

void TARGET_LANGUAGE::depparser::CWeight::saveScores() {
   std::cout<<"Saving scores..."; std::cout.flush();
   std::ofstream file ;
   file.open(m_sRecordPath.c_str()) ;

#ifdef LABELED
   file << "Dependency labels:" << std::endl;
   for (unsigned label=CDependencyLabel::FIRST; label<CDependencyLabel::COUNT; ++label)
      file << CDependencyLabel(label) << ' ';
   file << std::endl << std::endl;
#endif
   iterate_templates(file<<,;)
#ifdef DEBUG
   iterate_templates(,.trace(););
#endif
   if (m_bRules) file << "Rules=1" << std::endl;
   else file << "Rules=0" << std::endl;

   file.close();
   std::cout<<" done."<<std::endl;
}

/*--------------------------------------------------------------
 *
 * computeAverageFeatureWeights - compute average feature weights
 *
 *-------------------------------------------------------------*/

void TARGET_LANGUAGE::depparser::CWeight::computeAverageFeatureWeights(int round) {
   std::cout<<"Computing averaged (total) feature vector..."; std::cout.flush();
   iterate_templates(,.computeAverage(round);) ;

   std::cout<<"done."<<std::endl;
}


void TARGET_LANGUAGE::depparser::CWeight::debugUsage(void) const {
  std::cout << "m_mapSTw "; m_mapSTw.debugUsage();
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
}
