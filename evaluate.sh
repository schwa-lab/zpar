#!/bin/bash
set -e

DATA=$(dirname ${0})/data
if [[ ${#} -gt 0 ]]; then
  DATA=${1}
fi

NITERATIONS=10
MODEL=${DATA}/train.model
TRAIN_GOLD=${DATA}/wsj_02-21.mxpost.sd
TEST_GOLD=${DATA}/wsj_22.ontonotes.mxpost.sd
TEST_RUN=${DATA}/wsj_22.ontonotes.mxpost.slash

TEST_OUTPUT=${DATA}/out

rm -f ${MODEL}*
./dist/english.depparser/train ${TRAIN_GOLD} ${MODEL} ${NITERATIONS}
mv ${MODEL}.0 ${MODEL}
./dist/english.depparser/depparser ${TEST_RUN} ${TEST_OUTPUT} ${MODEL}
python3 evaluate_nbest.py ${TEST_OUTPUT} ${TEST_GOLD}
