#include <stdexcept>

#include <schwa/io/logging.h>

#include "depparser_weight.h"


namespace TARGET_LANGUAGE {
namespace depparser {

const schwa::FeatureType CWeight::STw(0);
const schwa::FeatureType CWeight::STt(1);
const schwa::FeatureType CWeight::STwt(2);
const schwa::FeatureType CWeight::N0w(3);
const schwa::FeatureType CWeight::N0t(4);
const schwa::FeatureType CWeight::N0wt(5);
const schwa::FeatureType CWeight::N1w(6);
const schwa::FeatureType CWeight::N1t(7);
const schwa::FeatureType CWeight::N1wt(8);
const schwa::FeatureType CWeight::N2w(9);
const schwa::FeatureType CWeight::N2t(10);
const schwa::FeatureType CWeight::N2wt(11);
const schwa::FeatureType CWeight::STHw(12);
const schwa::FeatureType CWeight::STHt(13);
const schwa::FeatureType CWeight::STi(14);
const schwa::FeatureType CWeight::STHHw(15);
const schwa::FeatureType CWeight::STHHt(16);
const schwa::FeatureType CWeight::STHi(17);
const schwa::FeatureType CWeight::STLDw(18);
const schwa::FeatureType CWeight::STLDt(19);
const schwa::FeatureType CWeight::STLDi(20);
const schwa::FeatureType CWeight::STRDw(21);
const schwa::FeatureType CWeight::STRDt(22);
const schwa::FeatureType CWeight::STRDi(23);
const schwa::FeatureType CWeight::N0LDw(24);
const schwa::FeatureType CWeight::N0LDt(25);
const schwa::FeatureType CWeight::N0LDi(26);
const schwa::FeatureType CWeight::STL2Dw(27);
const schwa::FeatureType CWeight::STL2Dt(28);
const schwa::FeatureType CWeight::STL2Di(29);
const schwa::FeatureType CWeight::STR2Dw(30);
const schwa::FeatureType CWeight::STR2Dt(31);
const schwa::FeatureType CWeight::STR2Di(32);
const schwa::FeatureType CWeight::N0L2Dw(33);
const schwa::FeatureType CWeight::N0L2Dt(34);
const schwa::FeatureType CWeight::N0L2Di(35);
const schwa::FeatureType CWeight::HTw(36);
const schwa::FeatureType CWeight::HTt(37);
const schwa::FeatureType CWeight::HTwt(38);
const schwa::FeatureType CWeight::STwtN0wt(39);
const schwa::FeatureType CWeight::STwtN0w(40);
const schwa::FeatureType CWeight::STwN0wt(41);
const schwa::FeatureType CWeight::STtN0wt(42);
const schwa::FeatureType CWeight::STwtN0t(43);
const schwa::FeatureType CWeight::STwN0w(44);
const schwa::FeatureType CWeight::STtN0t(45);
const schwa::FeatureType CWeight::N0tN1t(46);
const schwa::FeatureType CWeight::N0tN1tN2t(47);
const schwa::FeatureType CWeight::STtN0tN1t(48);
const schwa::FeatureType CWeight::STtN0tN0LDt(49);
const schwa::FeatureType CWeight::N0tN0LDtN0L2Dt(50);
const schwa::FeatureType CWeight::STHtSTtN0t(51);
const schwa::FeatureType CWeight::HTtHT2tN0t(52);
const schwa::FeatureType CWeight::STHHtSTHtSTt(53);
const schwa::FeatureType CWeight::STtSTLDtN0t(54);
const schwa::FeatureType CWeight::STtSTLDtSTL2Dt(55);
const schwa::FeatureType CWeight::STtSTRDtN0t(56);
const schwa::FeatureType CWeight::STtSTRDtSTR2Dt(57);
const schwa::FeatureType CWeight::STwd(58);
const schwa::FeatureType CWeight::STtd(59);
const schwa::FeatureType CWeight::N0wd(60);
const schwa::FeatureType CWeight::N0td(61);
const schwa::FeatureType CWeight::STwN0wd(62);
const schwa::FeatureType CWeight::STtN0td(63);
const schwa::FeatureType CWeight::STwra(64);
const schwa::FeatureType CWeight::STtra(65);
const schwa::FeatureType CWeight::STwla(66);
const schwa::FeatureType CWeight::STtla(67);
const schwa::FeatureType CWeight::N0wla(68);
const schwa::FeatureType CWeight::N0tla(69);
const schwa::FeatureType CWeight::STwrp(70);
const schwa::FeatureType CWeight::STtrp(71);
const schwa::FeatureType CWeight::STwlp(72);
const schwa::FeatureType CWeight::STtlp(73);
const schwa::FeatureType CWeight::N0wlp(74);
const schwa::FeatureType CWeight::N0tlp(75);
const schwa::FeatureType CWeight::STl(76);
const schwa::FeatureType CWeight::STc(77);
const schwa::FeatureType CWeight::STf(78);
const schwa::FeatureType CWeight::N0l(79);
const schwa::FeatureType CWeight::N0c(80);
const schwa::FeatureType CWeight::N0f(81);
const schwa::FeatureType CWeight::N1l(82);
const schwa::FeatureType CWeight::N1c(83);
const schwa::FeatureType CWeight::N1f(84);


CWeight::CWeight(const std::string &sInputPath, const std::string &sOutputPath, bool bTrain) : CWeightBase(sInputPath, sOutputPath, bTrain) { }

CWeight::~CWeight(void) { }


void
CWeight::loadScores(void) {
  LOG(INFO) << "[CWeight::loadScores] TODO" << std::endl;
}


void
CWeight::saveScores(void) {
  LOG(INFO) << "[CWeight::loadScores] TODO" << std::endl;
}


void
CWeight::addWeighted(const double mu, const CWeight &other) {
  LOG(INFO) << "[CWeight::addWeighted] TODO" << std::endl;
}


void
CWeight::computeAverageFeatureWeights(const unsigned int round) {
  LOG(INFO) << "[CWeight::computeAverage] TODO" << std::endl;
}


void
CWeight::debugUsage(void) const {
  LOG(INFO) << "[CWeight::debugUsage] TODO" << std::endl;
}


}  // namespace depparser
}  // namespace TARGET_LANGUAGE
