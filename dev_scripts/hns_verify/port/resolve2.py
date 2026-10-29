#!/usr/bin/env python3
# usage: resolve2.py <file> <choice>...   (one choice per conflict, in order; non-diff3 markers)
#   choice: ours | theirs | both | empty | @<path>(replacement text file)
# Resolves the conflict blocks that `git merge-file` left in <file>.
import sys
p = sys.argv[1]; choices = sys.argv[2:]
src = open(p).read().split('\n'); out = []; i = 0; n = 0
while i < len(src):
    l = src[i]
    if l.startswith('<<<<<<< '):
        ours = []; theirs = []; i += 1
        while not src[i].startswith('======='):
            ours.append(src[i]); i += 1
        i += 1
        while not src[i].startswith('>>>>>>> '):
            theirs.append(src[i]); i += 1
        i += 1
        c = choices[n]; n += 1
        if c == 'ours': out += ours
        elif c == 'theirs': out += theirs
        elif c == 'both': out += ours + theirs
        elif c == 'empty': pass
        elif c.startswith('@'): out += open(c[1:]).read().rstrip('\n').split('\n')
        else: raise SystemExit('bad choice ' + c)
        continue
    out.append(l); i += 1
assert n == len(choices), (n, len(choices))
open(p, 'w').write('\n'.join(out))
print(p, 'resolved', n)
