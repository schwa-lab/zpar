#!/bin/bash

DATA_DIR=data
MODEL_DIR=data/modelsU

TEST_GOLD=~dom/repos/zpar/data/wsj_22.ontonotes.mxpost.sd

MODEL_NAME=wsj_02-21_plus_400ks.mxpost.sd_10_1
MODEL=deplabeler.model
TEST_OUTPUT=${MODEL_DIR}/${MODEL_NAME}.out
DEPTEST_OUTPUT=${TEST_OUTPUT}.deps

for i in $(seq 1 10); do
    echo "./dist/english.deplabeler/deplabeler ${TEST_OUTPUT}.$i ${DEPTEST_OUTPUT}.$i ${MODEL}"
    ./dist/english.deplabeler/deplabeler ${TEST_OUTPUT}.$i ${DEPTEST_OUTPUT}.$i ${MODEL}
    echo "python3 evaluate_nbest.py ${DEPTEST_OUTPUT}.$i ${TEST_GOLD} > ${MODEL_DIR}/${MODEL_NAME}.depresults.$i"
    python3 evaluate_nbest.py ${DEPTEST_OUTPUT}.$i ${TEST_GOLD} > ${MODEL_DIR}/${MODEL_NAME}.depresults.$i
    cat ${MODEL_DIR}/${MODEL_NAME}.depresults.$i
done
