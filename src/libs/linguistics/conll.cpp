#include "linguistics/conll.h"

void readCoNLLFeats(std::vector<CGenericTag> &outout, CStringTokenizer &tokenizer, const std::string &input) {
   std::istringstream iss(input);
   outout.clear();
   std::string line;
   getline(iss, line, '|');
   if (line=="_")
      return;
   while (iss && !line.empty()) {
      outout.push_back(CGenericTag(line, tokenizer));
      getline(iss, line, '|');
   }
}

