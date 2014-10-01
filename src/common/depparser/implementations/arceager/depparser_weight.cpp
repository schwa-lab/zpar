#include "depparser_weight.h"


namespace TARGET_LANGUAGE {
namespace depparser {

const FeatureType CWeight::STw(0);
const FeatureType CWeight::STt(1);
const FeatureType CWeight::STwt(2);
const FeatureType CWeight::N0w(3);
const FeatureType CWeight::N0t(4);
const FeatureType CWeight::N0wt(5);
const FeatureType CWeight::N1w(6);
const FeatureType CWeight::N1t(7);
const FeatureType CWeight::N1wt(8);
const FeatureType CWeight::N2w(9);
const FeatureType CWeight::N2t(10);
const FeatureType CWeight::N2wt(11);
const FeatureType CWeight::STHw(12);
const FeatureType CWeight::STHt(13);
const FeatureType CWeight::STi(14);
const FeatureType CWeight::STHHw(15);
const FeatureType CWeight::STHHt(16);
const FeatureType CWeight::STHi(17);
const FeatureType CWeight::STLDw(18);
const FeatureType CWeight::STLDt(19);
const FeatureType CWeight::STLDi(20);
const FeatureType CWeight::STRDw(21);
const FeatureType CWeight::STRDt(22);
const FeatureType CWeight::STRDi(23);
const FeatureType CWeight::N0LDw(24);
const FeatureType CWeight::N0LDt(25);
const FeatureType CWeight::N0LDi(26);
const FeatureType CWeight::STL2Dw(27);
const FeatureType CWeight::STL2Dt(28);
const FeatureType CWeight::STL2Di(29);
const FeatureType CWeight::STR2Dw(30);
const FeatureType CWeight::STR2Dt(31);
const FeatureType CWeight::STR2Di(32);
const FeatureType CWeight::N0L2Dw(33);
const FeatureType CWeight::N0L2Dt(34);
const FeatureType CWeight::N0L2Di(35);
const FeatureType CWeight::HTw(36);
const FeatureType CWeight::HTt(37);
const FeatureType CWeight::HTwt(38);
const FeatureType CWeight::STwtN0wt(39);
const FeatureType CWeight::STwtN0w(40);
const FeatureType CWeight::STwN0wt(41);
const FeatureType CWeight::STtN0wt(42);
const FeatureType CWeight::STwtN0t(43);
const FeatureType CWeight::STwN0w(44);
const FeatureType CWeight::STtN0t(45);
const FeatureType CWeight::N0tN1t(46);
const FeatureType CWeight::N0tN1tN2t(47);
const FeatureType CWeight::STtN0tN1t(48);
const FeatureType CWeight::STtN0tN0LDt(49);
const FeatureType CWeight::N0tN0LDtN0L2Dt(50);
const FeatureType CWeight::STHtSTtN0t(51);
const FeatureType CWeight::HTtHT2tN0t(52);
const FeatureType CWeight::STHHtSTHtSTt(53);
const FeatureType CWeight::STtSTLDtN0t(54);
const FeatureType CWeight::STtSTLDtSTL2Dt(55);
const FeatureType CWeight::STtSTRDtN0t(56);
const FeatureType CWeight::STtSTRDtSTR2Dt(57);
const FeatureType CWeight::STwd(58);
const FeatureType CWeight::STtd(59);
const FeatureType CWeight::N0wd(60);
const FeatureType CWeight::N0td(61);
const FeatureType CWeight::STwN0wd(62);
const FeatureType CWeight::STtN0td(63);
const FeatureType CWeight::STwra(64);
const FeatureType CWeight::STtra(65);
const FeatureType CWeight::STwla(66);
const FeatureType CWeight::STtla(67);
const FeatureType CWeight::N0wla(68);
const FeatureType CWeight::N0tla(69);
const FeatureType CWeight::STwrp(70);
const FeatureType CWeight::STtrp(71);
const FeatureType CWeight::STwlp(72);
const FeatureType CWeight::STtlp(73);
const FeatureType CWeight::N0wlp(74);
const FeatureType CWeight::N0tlp(75);
const FeatureType CWeight::STl(76);
const FeatureType CWeight::STc(77);
const FeatureType CWeight::STf(78);
const FeatureType CWeight::N0l(79);
const FeatureType CWeight::N0c(80);
const FeatureType CWeight::N0f(81);
const FeatureType CWeight::N1l(82);
const FeatureType CWeight::N1c(83);
const FeatureType CWeight::N1f(84);


CWeight::CWeight(const std::string &sInputPath, const std::string &sOutputPath, bool bTrain) :
    CWeightBase(sInputPath, sOutputPath, bTrain)
  {
  loadScores();
}

CWeight::~CWeight(void) { }


void
CWeight::loadScores(void) {
  std::ifstream f(m_sInputPath, std::ios_base::binary);
  if (!f.is_open()) {
    LOG(DEBUG) << "[CWeight::loadScores] Could not open '" << m_sInputPath << "' for reading. Bailing." << std::endl;
    return;
  }

  if (!_weights.empty())
    throw std::runtime_error("Cannot loadScores for a non-empty weights table");
  _weights.deserialise(f);
}


void
CWeight::saveScores(void) {
  std::ofstream f(m_sOutputPath, std::ios_base::binary);
  if (!f.is_open()) {
    LOG(ERROR) << "[CWeight::saveScores] Could not open '" << m_sOutputPath << "' for reading. Bailing." << std::endl;
    return;
  }

  _weights.serialise(f);
  LOG(INFO) << "[CWeight::saveScores] size=" << _weights.size() << " load_factor=" << _weights.load_factor() << std::endl;
  _weights.stats(std::cout);
}


void
CWeight::addWeighted(const double mu, const CWeight &other) {
  LOG(INFO) << "[CWeight::addWeighted] TODO" << std::endl;
}


void
CWeight::computeAverageFeatureWeights(const unsigned int iteration) {
  //for (auto &entry : _weights)
    //entry.updateAverage(iteration);
  _weights.update_average(iteration);
}


void
CWeight::debugUsage(void) const {
  LOG(INFO) << "[CWeight::debugUsage] size=" << _weights.size() << " load_factor=" << _weights.load_factor() << std::endl;
}

}  // namespace depparser
}  // namespace TARGET_LANGUAGE
