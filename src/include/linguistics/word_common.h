// Copyright (C) University of Oxford 2010
// word manipulation methods

inline const std::string &
wordToString(const CWord &w) {
   return w.str();
}

inline CWord
stringToWord(const std::string &s) { // this method is unecessary, for word can be assigned by a std::string
   return CWord(s);
}

// bigram word definition
typedef CBigram<CWord> CTwoWords;


// io methods
inline std::istream &
operator >>(std::istream &is, CWord &w) {
  w.load(mp::read_str(is));
  return is;
}

inline std::ostream &
operator <<(std::ostream &os, const CWord &w) {
  mp::write_str(os, w.str());
  return os ;
}
