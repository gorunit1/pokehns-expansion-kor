#!/usr/bin/env python3
# seq166 #9784 scratch (copied from chunk-150-155/tmp-150/kortests): compare two decoded traces block by block (block = one test result).
# usage: tracecmp.py <a-trace> <b-trace>   -> prints per-test: SAME / DIFF (n lines) ; test name keyed without the final ": RESULT"
import sys, re, difflib
def load(p):
    blocks = {}; order = []; cur = None
    for line in open(p, encoding='utf-8'):
        if line.startswith('## '):
            name = re.sub(r': [A-Z_]+\s*$', '', line[3:].rstrip('\n'))
            cur = name; blocks[cur] = []; order.append(cur)
        elif cur is not None:
            blocks[cur].append(line.rstrip('\n'))
    return blocks, order
a, oa = load(sys.argv[1]); b, ob = load(sys.argv[2])
same = diff = 0
for n in oa:
    if n not in b:
        print('MISSING-IN-B', n); continue
    if a[n] == b[n]:
        same += 1
    else:
        d = [l for l in difflib.unified_diff(a[n], b[n], lineterm='', n=0) if l[:1] in '+-' and not l.startswith(('+++', '---'))]
        diff += 1
        print('DIFF %3d  %s' % (len(d), n))
print('same %d, diff %d' % (same, diff))
