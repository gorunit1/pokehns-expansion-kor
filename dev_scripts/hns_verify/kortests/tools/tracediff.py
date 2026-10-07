#!/usr/bin/env python3
# seq 181 area E scratch: unified diff of one or more tests between two decoded traces.
# usage: tracediff.py <a-trace> <b-trace> <substring of test name> [...]
import sys, re, difflib
def load(p):
    blocks = {}; cur = None
    for line in open(p, encoding='utf-8'):
        if line.startswith('## '):
            cur = re.sub(r' \d+/\d+$', '', re.sub(r': [A-Z_]+\s*$', '', line[3:].rstrip('\n'))); blocks.setdefault(cur, [])
        elif cur is not None:
            l = line.rstrip('\n')
            if l.startswith(('HP ', 'E b=', 'HP   ', 'STATUS', 'X b=', '-- run')):
                continue
            blocks[cur].append(l)
    return blocks
a = load(sys.argv[1]); b = load(sys.argv[2])
for key in sys.argv[3:]:
    for n in [n for n in a if key in n]:
        print('#### ' + n)
        for l in difflib.unified_diff(a[n], b.get(n, []), lineterm='', n=2):
            if l.startswith(('+++', '---')):
                continue
            print(l)
