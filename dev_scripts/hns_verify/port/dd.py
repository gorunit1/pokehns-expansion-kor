#!/usr/bin/env python3
# usage: C=<upstream commit> [UPSTREAM=<dir>] dd.py      (run in the HnS repo before committing)
# Diff of diffs: compares the lines the upstream commit adds/removes with the lines `git diff` adds/removes in
# the HnS working tree for the same files, and prints what differs (HnS deltas, missed upstream lines).
# New files must be added with `git add -N` first.
import collections, os, subprocess
up = os.environ.get('UPSTREAM', os.path.join(os.path.dirname(os.getcwd()), 'pokeemerald-expansion-upstream'))
C = os.environ['C']
files = subprocess.run(['git', '-C', up, 'show', '--name-only', '--format=', C], capture_output=True, text=True).stdout.split()
def ch(diff):
    a = collections.Counter(); r = collections.Counter()
    for l in diff.split('\n'):
        if l.startswith('+++') or l.startswith('---'): continue
        if l.startswith('+'): a[l[1:].strip()] += 1
        elif l.startswith('-'): r[l[1:].strip()] += 1
    return a, r
for f in files:
    ud = subprocess.run(['git', '-C', up, 'show', C, '--', f], capture_output=True, text=True).stdout
    hf = 'test/battle/ai/values_moves_over_splash.c' if f.endswith('can_use_all_moves.c') else f
    hd = subprocess.run(['git', 'diff', '--', hf], capture_output=True, text=True).stdout
    ua, ur = ch(ud); ha, hr = ch(hd)
    groups = [('UP+ not in HnS', ua - ha), ('UP- not in HnS', ur - hr), ('HnS+ extra', ha - ua), ('HnS- extra', hr - ur)]
    if any(v for _, v in groups):
        print('=====', f)
        for k, v in groups:
            for line in v:
                if line: print(' ', k, '|', line[:150])
