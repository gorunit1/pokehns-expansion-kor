#!/usr/bin/env python3
# usage: scriptcmp.py <old.o> <new.o> [section]   (default section: script_data, e.g. build/hns/data/event_scripts.o)
# Compare a script object label by label, with section-relative pointers (REL addends into the same section)
# replaced by "label+offset" so pure layout moves do not count as differences. Prints labels only in one file
# and labels whose bytes/pointers differ, with the first differing tokens (ints = bytes, tuples = pointers).
# Used for seq 516 #9986 (implicit waitstate after specials, pret#2200).
import sys, subprocess, re, bisect, struct, collections
SEC = sys.argv[3] if len(sys.argv) > 3 else 'script_data'
def load(o):
    data = subprocess.run(['arm-none-eabi-objcopy','-O','binary','--only-section='+SEC,o,'/dev/stdout'],capture_output=True).stdout
    labs = []
    for l in subprocess.run(['arm-none-eabi-objdump','-t',o],capture_output=True,text=True).stdout.split('\n'):
        m = re.match(r'^([0-9a-f]{8}) (.{7}) (\S+)\s+[0-9a-f]+ (\S+)$', l)
        if m and m.group(3) == SEC and m.group(4) != SEC and not m.group(4).startswith('$'):
            labs.append((int(m.group(1),16), m.group(4)))
    labs.sort()
    offs = [a for a,_ in labs]
    rel = {}
    cur = None
    for l in subprocess.run(['arm-none-eabi-objdump','-r',o],capture_output=True,text=True).stdout.split('\n'):
        m = re.match(r'RELOCATION RECORDS FOR \[(?:\.rel)?([^\]]+)\]', l)
        if m: cur = m.group(1); continue
        m = re.match(r'^([0-9a-f]{8}) (\S+)\s+(\S+)', l)
        if m and cur == SEC: rel[int(m.group(1),16)] = (m.group(2), m.group(3))
    def name_at(a):
        i = bisect.bisect_right(offs, a) - 1
        if i < 0: return '?+%d' % a
        # prefer the first label at that offset
        j = i
        while j > 0 and offs[j-1] == offs[i]: j -= 1
        return '%s+%d' % (labs[j][1], a - offs[i])
    res = collections.OrderedDict()
    for i,(a,n) in enumerate(labs):
        e = len(data)
        for k in range(i+1, len(labs)):
            if labs[k][0] > a: e = labs[k][0]; break
        toks = []; p = a
        while p < e:
            if p in rel and p + 4 <= len(data):
                t, s = rel[p]; add = struct.unpack_from('<I', data, p)[0]
                toks.append(('R', t, name_at(add) if s == SEC else '%s+%d' % (s, add))); p += 4
            else:
                toks.append(data[p]); p += 1
        res[n] = toks
    return res
a = load(sys.argv[1]); b = load(sys.argv[2])
print('labels', len(a), '->', len(b))
print('only old:', ' '.join(sorted(set(a) - set(b))))
print('only new:', ' '.join(sorted(set(b) - set(a))))
d = [n for n in a if n in b and a[n] != b[n]]
print('differ:', len(d))
for n in d:
    print(' ', n, len(a[n]), '->', len(b[n]))
    x, y = a[n], b[n]
    i = 0
    while i < min(len(x), len(y)) and x[i] == y[i]: i += 1
    print('     first diff at token', i, ':', x[i:i+8], '|', y[i:i+8])
