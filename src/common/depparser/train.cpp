// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                          *
 * train.cpp - the training of the general dependency parser.  *
 *                                          *
 * Author: Yue Zhang                              *
 *                                          *
 * Computing Laboratory, Oxford. 2007.8                 *
 *                                          *
 ****************************************************************/
#include <algorithm>

#include "definitions.h"
#include "depparser.h"
#include "reader.h"
#include "writer.h"

using namespace TARGET_LANGUAGE;

static void
read_input_file(const std::string &path, std::vector<CDependencyParse *> &sentences) {
  std::ifstream is(path.c_str());
  assert(is.is_open());

  CDependencyParse *sent = new CDependencyParse();
  while (is >> *sent) {
    sentences.push_back(sent);
    sent = new CDependencyParse();
  }
  delete sent;
}


/*===============================================================
 *
 * auto_train - train by the parser itself, black-box training
 *
 *===============================================================*/
void auto_train(const std::string &sInputFile, const std::string &sFeatureFile, const bool &bRules, const bool &bExtract, const std::string &sMetaPath, const unsigned int training_iterations) {
  // Read in the input.
  std::cout << "Reading in the training data..." << std::flush;
  std::vector<CDependencyParse *> sentences;
  read_input_file(sInputFile, sentences);
  std::cout << " found " << sentences.size() << " sentences." << std::endl;

  // Run each iteration of the perceptron learning.
  for (unsigned int iteration = 0; iteration != training_iterations; ++iteration) {
    // Construct the parser.
    CDepParser parser(sFeatureFile, true, false);
    parser.setRules(bRules);
#ifdef SUPPORT_META_FEATURE_DEFINITION
    if (!sMetaPath.empty() )
      parser.loadMeta(sMetaPath);
#endif

    // Shuffle the sentence order between each iteration.
    std::random_shuffle(sentences.begin(), sentences.end());

    // Train on each sentence.
    std::cout << "Training iteration " << iteration << " has started..." << std::endl ; std::cout.flush();
    for (auto n = 0; n != sentences.size(); ++n) {
      CDependencyParse &sent = *sentences[n];
      TRACE("Sentence " << n);

      if (bExtract) {
#ifdef SUPPORT_FEATURE_EXTRACTION
        parser.extract_features(sent);
#else
        ASSERT(false, "Internal error: feature extract not allowed but option set.");
#endif
      }
      else {
        parser.train(sent, n + 1);
      }
    }

    // Tell the parser that this training round has finished.
    parser.finishtraining();
    std::cout << "Done. " << std::endl;
  }
}

/*===============================================================
 *
 * main
 *
 *==============================================================*/

int main(int argc, char* argv[]) {

  try {
    COptions options(argc, argv);
    CConfigurations configurations;
    configurations.defineConfiguration("r", "", "use rules", "");
#ifdef SUPPORT_FEATURE_EXTRACTION
    configurations.defineConfiguration("f", "", "extract features only: weights will be counts", "");
#endif
#ifdef SUPPORT_META_FEATURE_DEFINITION
    configurations.defineConfiguration("t", "path", "meta feature types", "");
#endif
    if (options.args.size() != 4) {
      std::cout << "\nUsage: " << argv[0] << " training_data model num_iterations" << std::endl ;
      std::cout << configurations.message() << std::endl;
      return 1;
    } 
    configurations.loadConfigurations(options.opts);
  
    int training_iterations;
    if (!fromString(training_iterations, options.args[3])) {
      std::cerr << "Error: the number of training iterations must be an integer." << std::endl;
      return 1;
    }
  
    bool bRules = configurations.getConfiguration("r").empty() ? false : true;
    bool bExtract = false;
#ifdef SUPPORT_FEATURE_EXTRACTION
    bExtract = configurations.getConfiguration("f").empty() ? false : true;
#endif
  std::string sMetaPath;
#ifdef SUPPORT_META_FEATURE_DEFINITION
    sMetaPath = configurations.getConfiguration("t");
#endif

    std::cout << "Training started" << std::endl;
    int time_start = clock();
    auto_train(options.args[1], options.args[2], bRules, bExtract, sMetaPath, training_iterations);
    std::cout << "Training has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << std::endl;
  
    return 0;
  } catch (const std::string &e) {
    std::cerr << std::endl << "Error: " << e << std::endl;
    return 1;
  }

}

