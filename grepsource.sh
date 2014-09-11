#!/bin/bash

cd $(dirname ${0})
find src -type f \( -name '*.h' -or -name '*.cpp' \) \
  | egrep -v '^src/(chinese|generic)/' \
  | egrep -v '^src/common/(conparser|deplabeler|tagger)/' \
  | egrep -v '^src/common/depparser/implementations/(acl11|cad|covington|eisner|emnlp08|meta|punct|uppsala)/' \
  | xargs egrep --color ${@}
