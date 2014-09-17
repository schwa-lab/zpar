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

  inline SCORE_TYPE
  score(const ScoreAverage n) const {
    return n == eNonAverage ? _current : _total;
  }

  inline void
  updateAverage(const unsigned int iteration) {
    if (iteration > _last_updated)
      _total += _current*(iteration - _last_updated);
  }

  void
  updateCurrent(const SCORE_TYPE added, const unsigned int iteration) {
    if (iteration > _last_updated) {
      updateAverage(iteration);
      _last_updated = iteration;
    }
    _current += added;
    _total += added;
  }

  // ==============================================================================================
  // FeatureHashtableEntryBase requirements.
  // ==============================================================================================
  inline void
  clear(void) {
    std::memset(this, 0, sizeof(Entry));
  }

  inline void
  deserialise(std::istream &in) {
    const uint32_t nitems = mp::read_array_size(in);
    assert(nitems == 4); (void)nitems;
    _value = mp::read_uint64(in);
    mp::read(in, _current);
    mp::read(in, _total);
    _last_updated = mp::read_uint(in);
  }

  inline void
  serialise(std::ostream &out) const {
    mp::write_array_size(out, 4);
    mp::write_uint64(out, _value);
    mp::write(out, _current);
    mp::write(out, _total);
    mp::write_uint(out, _last_updated);
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
  void computeAverageFeatureWeights(unsigned int iteration);
  void debugUsage(void) const;

  template <typename CP>
  void getOrUpdateScore(const schwa::FeatureType &type, const CP &cp, CPackedScoreType<SCORE_TYPE, action::MAX> &out, action::StackAction action, ScoreAverage sa, SCORE_TYPE amount, unsigned int iteration);
};


template <typename CP>
inline void
CWeight::getOrUpdateScore(const schwa::FeatureType &type, const CP &cp, CPackedScoreType<SCORE_TYPE, action::MAX> &out, const action::StackAction action, const ScoreAverage sa, const SCORE_TYPE amount, const unsigned int iteration) {
  const schwa::Label label = action;

  if (amount == 0) {
    const auto &fn = [&](const schwa::Label l, const decltype(_weights)::mapped_type &entry) {
      out[l] += entry.score(sa);
    };
    _weights.for_each_label(type, cp, 0, action::MAX, fn);
  }
  else
    _weights.get(type, cp, label).updateCurrent(amount, iteration);
}

}  // namespace depparser
}  // namespace TARGET_LANGUAGE

#endif
