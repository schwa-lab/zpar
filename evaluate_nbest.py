#!/usr/bin/env python3
import argparse
import re
import string

from collections import defaultdict
from operator import itemgetter

# malt tab indices
WORD_INDEX = 0
POS_INDEX = 1
HEAD_INDEX = 2
LABEL_INDEX = 3

RE_PUNCT_POS = re.compile(r'^(HYPH|NFP|[' + re.escape(string.punctuation) + ']+)$')


def next_sent(file):
  out = []
  for line in file:
    line = line.strip()
    if line:
      out.append(line.split("\t"))
    elif out:
      yield out
      out = []
  if out:
    yield out


def main(args):
  test_iter = next_sent(args.test_file)
  gold_iter = next_sent(args.gold_file)

  gold_sent = None
  test_words = []
  gold_words = []
  correct_uarcs = 0
  total_uarcs = 0
  incorrect_uarcs = 0
  skipped_uarcs = 0

  correct_larcs = 0
  total_larcs = 0
  incorrect_larcs = 0
  skipped_larcs = 0
  correct_label = defaultdict(int)
  incorrect_label = defaultdict(int)
  skipped_label = defaultdict(int)

  parse_index = 0
  total_sentences = 0
  total_ucorrect_sentences = 0
  total_lcorrect_sentences = 0
  total_parses = 0
  total_unique_parses = 0

  oracle_uranks = []
  oracle_lranks = []
  current_best_uparse_index = 0
  current_best_lparse_index = 0

  current_correct_uarcs = 0
  current_total_uarcs = 0
  current_incorrect_uarcs = 0
  current_skipped_uarcs = 0
  current_correct_larcs = 0
  current_total_larcs = 0
  current_incorrect_larcs = 0
  current_skipped_larcs = 0
  current_correct_label = defaultdict(int)
  current_incorrect_label = defaultdict(int)
  current_skipped_label = defaultdict(int)
  local_parses = 0
  current_deps = set()

  for test_sent in test_iter:
    local_correct_uarcs = 0
    local_total_uarcs = 0
    local_incorrect_uarcs = 0
    local_skipped_uarcs = 0
    local_correct_larcs = 0
    local_total_larcs = 0
    local_incorrect_larcs = 0
    local_skipped_larcs = 0
    local_parses += 1
    local_correct_label = defaultdict(int)
    local_incorrect_label = defaultdict(int)
    local_skipped_label = defaultdict(int)
    dep_list = []
    test_words = [w[WORD_INDEX] for w in test_sent]

    if gold_sent is None or test_words != gold_words:
      gold_sent = next(gold_iter)
      gold_words = [w[WORD_INDEX] for w in gold_sent]
      parse_index = 0
      total_sentences += 1
      if current_total_uarcs > 0:
        total_uarcs += current_total_uarcs
        correct_uarcs += current_correct_uarcs
        skipped_uarcs += current_skipped_uarcs
        incorrect_uarcs += current_incorrect_uarcs
        oracle_uranks.append(current_best_uparse_index)
        if current_incorrect_uarcs == 0:
          total_ucorrect_sentences += 1

      if current_total_larcs > 0:
        total_larcs += current_total_larcs
        correct_larcs += current_correct_larcs
        skipped_larcs += current_skipped_larcs
        incorrect_larcs += current_incorrect_larcs
        for key, val in current_correct_label.items():
          correct_label[key] += val
        for key, val in current_incorrect_label.items():
          incorrect_label[key] += val
        for key, val in current_skipped_label.items():
          skipped_label[key] += val
        oracle_lranks.append(current_best_lparse_index)
        if current_incorrect_larcs == 0:
          total_lcorrect_sentences += 1

      current_correct_uarcs = 0
      current_total_uarcs = 0
      current_incorrect_uarcs = 0
      current_skipped_uarcs = 0
      current_correct_larcs = 0
      current_total_larcs = 0
      current_incorrect_larcs = 0
      current_skipped_larcs = 0
      current_best_uparse_index = 0
      current_best_lparse_index = 0
      current_correct_label = defaultdict(int)
      current_incorrect_label = defaultdict(int)
      current_skipped_label = defaultdict(int)
      total_parses += local_parses
      total_unique_parses += len(current_deps)
      local_parses = 0
      current_deps = set()

    parse_index += 1
    if not args.oracle and parse_index > 1:
      continue

    if test_words != gold_words:
      raise Exception("Words do not match")

    for t, g in zip(test_sent, gold_sent):
      local_total_uarcs += 1
      local_total_larcs += 1
      if not args.eval_punct and RE_PUNCT_POS.match(t[POS_INDEX]):
        # skip punct
        local_skipped_uarcs += 1
        local_skipped_larcs += 1
        local_skipped_label[t[LABEL_INDEX]] += 1
      else:
        dep_list.append(tuple(t))
        if t[HEAD_INDEX] == g[HEAD_INDEX]:
          local_correct_uarcs += 1
          if t[LABEL_INDEX] == g[LABEL_INDEX]:
            local_correct_larcs += 1
            local_correct_label[t[LABEL_INDEX]] += 1
          else:
            local_incorrect_larcs += 1
            local_incorrect_label[(t[LABEL_INDEX], g[LABEL_INDEX])] += 1
        else:
          local_incorrect_uarcs += 1
          local_incorrect_larcs += 1
          local_incorrect_label[(t[LABEL_INDEX], g[LABEL_INDEX])] += 1

    if local_correct_uarcs > current_correct_uarcs:
      current_correct_uarcs = local_correct_uarcs
      current_incorrect_uarcs = local_incorrect_uarcs
      current_skipped_uarcs = local_skipped_uarcs
      current_total_uarcs = local_total_uarcs
      current_best_uparse_index = parse_index

    if local_correct_larcs > current_correct_larcs:
      current_correct_larcs = local_correct_larcs
      current_incorrect_larcs = local_incorrect_larcs
      current_skipped_larcs = local_skipped_larcs
      current_correct_label = local_correct_label
      current_incorrect_label = local_incorrect_label
      current_skipped_label = local_skipped_label
      current_total_larcs = local_total_larcs
      current_best_lparse_index = parse_index

    deps = frozenset(dep_list)
    if deps not in current_deps:
      current_deps.add(deps)

  # count up final sentence
  total_uarcs += current_total_uarcs
  correct_uarcs += current_correct_uarcs
  skipped_uarcs += current_skipped_uarcs
  incorrect_uarcs += current_incorrect_uarcs
  oracle_uranks.append(current_best_uparse_index)

  total_larcs += current_total_larcs
  correct_larcs += current_correct_larcs
  skipped_larcs += current_skipped_larcs
  incorrect_larcs += current_incorrect_larcs
  for key, val in current_correct_label.items():
    correct_label[key] += val
  for key, val in current_incorrect_label.items():
    incorrect_label[key] += val
  for key, val in current_skipped_label.items():
    skipped_label[key] += val
  oracle_lranks.append(current_best_lparse_index)

  total_parses += local_parses
  total_unique_parses += len(current_deps)

  evaluated_uarcs = total_uarcs - skipped_uarcs
  evaluated_larcs = total_larcs - skipped_larcs
  print("UAS: {:.2f} ({} / {})".format(correct_uarcs / float(evaluated_uarcs) * 100, correct_uarcs, evaluated_uarcs))
  print("LAS: {:.2f} ({} / {})".format(correct_larcs / float(evaluated_larcs) * 100, correct_larcs, evaluated_larcs))
  print("Unlabelled sentence accuracy {:.2f} ({} / {})".format(total_ucorrect_sentences / float(total_sentences) * 100, total_ucorrect_sentences, total_sentences))
  print("Labelled sentence accuracy {:.2f} ({} / {})".format(total_lcorrect_sentences / float(total_sentences) * 100, total_lcorrect_sentences, total_sentences))
  if args.oracle:
    print("Average oracle unlabeled parse rank: {:.2f} (Min: {}, Max: {})".format(sum(oracle_uranks) / float(len(oracle_uranks)), min(oracle_uranks), max(oracle_uranks)))
    print("Average oracle labeled parse rank: {:.2f} (Min: {}, Max: {})".format(sum(oracle_lranks) / float(len(oracle_lranks)), min(oracle_lranks), max(oracle_lranks)))
  print("Parses per sentence: {:.2f} ({} / {})".format(total_parses / float(total_sentences), total_parses, total_sentences))
  print("Unique parses per sentence: {:.2f} ({} / {})".format(total_unique_parses / float(total_sentences), total_unique_parses, total_sentences))

  if args.label_errors:
    print("\nLabel confusion: test -> gold")
    for (test_label, gold_label), val in sorted(incorrect_label.items(), reverse=True, key=itemgetter(1)):
      print("{} -> {}: {}".format(test_label, gold_label, val))


if __name__ == "__main__":
  ap = argparse.ArgumentParser(description="Runs an evaluation over nbest dependency parse output")
  ap.add_argument("test_file", type=open, help="test nbest file to evaluate")
  ap.add_argument("gold_file", type=open, help="gold file to evaluate against")
  ap.add_argument("--eval-punct", action="store_true", default=False, help="Evaluate punctuation")
  ap.add_argument("--oracle", action="store_true", default=False, help="Perform oracle evaluation of nbest output (by default evaluates only the first parse)")
  ap.add_argument("--conll", action="store_true", default=False, help="Use CoNLL format (uses zpar tab-output by default)")
  ap.add_argument("--label-errors", action="store_true", default=False, help="Print an error summary by label")

  args = ap.parse_args()
  if args.conll:
    WORD_INDEX = 1
    POS_INDEX = 3
    HEAD_INDEX = 6
    LABEL_INDEX = 7
  main(args)
