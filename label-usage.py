#!/usr/bin/env python3
import collections
import sys


def process_labels(tokens, counts, search_label):
  for pos, head, label in tokens:
    if label == search_label:
      dep_tag = pos
      head_tag = tokens[head][0]
      counts[(head_tag, dep_tag)] += 1


def main(search_label):
  counts = collections.Counter()
  tokens = []
  for line in sys.stdin:
    line = line.strip()
    if not line:
      process_labels(tokens, counts, search_label)
      tokens = []
    else:
      parts = line.split('\t')
      if len(parts) == 4:
        word, pos, head, label = parts
        head = int(head)
      elif len(parts) == 10:
        _, word, _, _, pos, _, head, label, _, _ = parts
        head = int(head) - 1
      else:
        raise Exception('Unknown number of column')
      tokens.append((pos, head, label))
  process_labels(tokens, counts, search_label)

  rows = [('head', 'dep', 'freq')]
  for k, v in counts.most_common():
    rows.append((k[0], k[1], str(v)))

  widths = [max(len(row[c]) for row in rows) for c in range(3)]
  for row in rows:
    out = []
    out.append(row[0].ljust(widths[0]))
    out.append(row[1].ljust(widths[1]))
    out.append(row[2].rjust(widths[2]))
    print('  '.join(out))


if __name__ == '__main__':
  if len(sys.argv) != 2:
    print('Usage: {0} label'.format(sys.argv[0]))
    sys.exit(1)

  main(sys.argv[1])
