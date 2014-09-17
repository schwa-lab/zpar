#include <stdexcept>

#include <schwa/io/logging.h>

#include "depparser_weight.h"


namespace TARGET_LANGUAGE {
namespace depparser {

const schwa::FeatureType CWeight::STw(0);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::STt(1, schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STwt(2);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::N0w(3);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::N0t(4, schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::N0wt(5);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::N1w(6);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::N1t(7, schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::N1wt(8);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::N2w(9);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::N2t(10, schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::N2wt(11);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::STHw(12);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::STHt(13, schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STi(14, schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STHHw(15);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STHHt(16);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STHi(17);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STLDw(18);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::STLDt(19);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STLDi(20);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STRDw(21);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::STRDt(22);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STRDi(23);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::N0LDw(24);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::N0LDt(25);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::N0LDi(26);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STL2Dw(27);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::STL2Dt(28);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STL2Di(29);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STR2Dw(30);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::STR2Dt(31);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STR2Di(32);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::N0L2Dw(33);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::N0L2Dt(34);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::N0L2Di(35);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::HTw(36);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::HTt(37);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::HTwt(38);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STwtN0wt(39);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::STwtN0w(40);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::STwN0wt(41);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::STtN0wt(42);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::STwtN0t(43);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::STwN0w(44);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::STtN0t(45);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::N0tN1t(46);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::N0tN1tN2t(47);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STtN0tN1t(48);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STtN0tN0LDt(49);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::N0tN0LDtN0L2Dt(50);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STHtSTtN0t(51);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::HTtHT2tN0t(52);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STHHtSTHtSTt(53);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STtSTLDtN0t(54);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STtSTLDtSTL2Dt(55);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STtSTRDtN0t(56);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STtSTRDtSTR2Dt(57);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STwd(58);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::STtd(59);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::N0wd(60);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::N0td(61);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STwN0wd(62);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::STtN0td(63);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STwra(64);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STtra(65);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STwla(66);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STtla(67);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::N0wla(68);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::N0tla(69);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STwrp(70);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::STtrp(71);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STwlp(72);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::STtlp(73);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::N0wlp(74);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::N0tlp(75);  // schwa::FeatureStorage::SPARSE);
const schwa::FeatureType CWeight::STl(76);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STc(77);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::STf(78);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::N0l(79);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::N0c(80);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::N0f(81);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::N1l(82);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::N1c(83);  // schwa::FeatureStorage::DENSE);
const schwa::FeatureType CWeight::N1f(84);  // schwa::FeatureStorage::DENSE);


CWeight::CWeight(const std::string &sInputPath, const std::string &sOutputPath, bool bTrain) : CWeightBase(sInputPath, sOutputPath, bTrain) {
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
    _weights.clear();
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
}


void
CWeight::addWeighted(const double mu, const CWeight &other) {
  LOG(INFO) << "[CWeight::addWeighted] TODO" << std::endl;
}


void
CWeight::computeAverageFeatureWeights(const unsigned int iteration) {
  for (auto &entry : _weights)
    entry.updateAverage(iteration);
}


void
CWeight::debugUsage(void) const {
  LOG(INFO) << "[CWeight::debugUsage] size=" << _weights.size() << " load_factor=" << _weights.load_factor() << std::endl;
}

}  // namespace depparser
}  // namespace TARGET_LANGUAGE
