#!/usr/bin/env python3
import os
import subprocess
import tempfile


def read_gold(file):
  sents = []
  sent = []
  for line in file:
    line = line.strip()
    if not line:
      sents.append(sent)
      sent = []
    else:
      parts = line.split('\t')
      assert 8 <= len(parts) <= 10  # (id, form, lemma, cpos, pos, feats, head1, deprel, phead, pdeprel)
      sent.append(parts)
  if sent:
    sents.append(sent)
  return sents


def read_test(file):
  sents = []
  sent = []
  for line in file:
    line = line.strip()
    if not line:
      sents.append(sent)
      sent = []
    else:
      parts = line.split('\t')
      assert len(parts) == 4  # (form, pos, head0, deprel)
      sent.append(parts)
  if sent:
    sents.append(sent)
  return sents


def write_sents(path, sents):
  with open(path, 'w') as f:
    for sent in sents:
      for parts in sent:
        print('\t'.join(parts), file=f)
      print(file=f)


def main(cmd_args):
  # Read in the gold and parsed test files.
  gsents = read_gold(cmd_args.gold_file)
  tsents = read_test(cmd_args.test_file)

  new_gsents = []
  new_tsents = []

  assert len(gsents) == len(tsents)
  for gsent, tsent in zip(gsents, tsents):
    assert len(gsent) == len(tsent)

    new_gsent = []
    new_tsent = []
    for gparts, tparts in zip(gsent, tsent):
      # Convert the gold part into the form required by the CoNLL07 eval script.
      new_gparts = gparts[:8] + ['_', '_']
      assert len(new_gparts) == 10
      new_gsent.append(new_gparts)

      # Convert the test part into the form required by the CoNLL07 eval script.
      new_tparts = list(new_gparts)
      assert new_tparts[1] == tparts[0]
      new_tparts[6] = str(int(tparts[2]) + 1)
      new_tparts[7] = tparts[3]
      assert len(new_tparts) == 10
      new_tsent.append(new_tparts)

    assert len(new_gsent) == len(new_tsent)
    new_gsents.append(new_gsent)
    new_tsents.append(new_tsent)

  # Output the reformatted gold and test files.
  _, new_gpath = tempfile.mkstemp()
  _, new_tpath = tempfile.mkstemp()
  write_sents(new_gpath, new_gsents)
  write_sents(new_tpath, new_tsents)

  # Call the CoNLL07 eval script.
  args = ['perl', 'eval07.pl', '-g', new_gpath, '-s', new_tpath]
  if not cmd_args.eval_punct:
    args.append('-p')
  subprocess.check_call(args)

  # Remove the temp files.
  os.remove(new_gpath)
  os.remove(new_tpath)


if __name__ == '__main__':
  import argparse
  ap = argparse.ArgumentParser(description='Runs an evaluation over nbest dependency parse output')
  ap.add_argument('gold_file', type=open, help='gold file to evaluate against')
  ap.add_argument('test_file', type=open, help='test nbest file to evaluate')
  ap.add_argument('--eval-punct', action='store_true', default=False, help='Evaluate punctuation')

  main(ap.parse_args())
