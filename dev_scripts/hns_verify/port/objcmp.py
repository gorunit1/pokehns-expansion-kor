#!/usr/bin/env python3
# usage: objcmp.py <old build dir> <new build dir> [start]   (e.g. <clone>/build/hns build/hns; start = skip objects up to this path)
# Per object file, compare each function's disassembly with relocations shown by symbol (addresses stripped),
# and each data section's bytes + relocations. Prints changed functions/sections per object (8 processes).
# Use when a port should change only some functions (seq 518 #10555): every listed function should be one the
# upstream diff touches, or explained (assert line numbers, a renamed table). Takes about an hour on build/hns.
import sys, os, re, subprocess, collections, hashlib
A, B = sys.argv[1], sys.argv[2]
def funcs(o):
    out = subprocess.run(['arm-none-eabi-objdump', '-d', '-r', '--no-show-raw-insn', o], capture_output=True, text=True).stdout
    res = collections.OrderedDict(); cur = None
    for l in out.split('\n'):
        m = re.match(r'^[0-9a-f]+ <(.+)>:$', l)
        if m: cur = m.group(1); res[cur] = []; continue
        if cur is None or not l.strip(): continue
        l = re.sub(r'^\s*[0-9a-f]+:\s*', '', l)
        l = re.sub(r'\b[0-9a-f]+ <', '<', l)          # branch target address
        l = re.sub(r'; \(.*\)$', '', l)
        l = re.sub(r'@ 0x[0-9a-f]+', '', l)
        res[cur].append(l.strip())
    return res
def data(o):
    out = subprocess.run(['arm-none-eabi-objdump', '-h', o], capture_output=True, text=True).stdout
    secs = [m.group(1) for m in re.finditer(r'^\s*\d+\s+(\S+)\s+([0-9a-f]+)', out, re.M) if int(m.group(2), 16) and not m.group(1).startswith(('.text', '.ARM', '.comment', '.debug'))]
    res = {}
    for s in secs:
        b = subprocess.run(['arm-none-eabi-objcopy', '-O', 'binary', '--only-section=' + s, o, '/dev/stdout'], capture_output=True).stdout
        r = subprocess.run(['arm-none-eabi-objdump', '-r', '-j', s, o], capture_output=True, text=True).stdout
        r = '\n'.join(x for x in r.split('\n') if re.match(r'^[0-9a-f]{8} ', x))
        res[s] = hashlib.md5(b + r.encode()).hexdigest()
    return res
def one(a):
        b = os.path.join(B, os.path.relpath(a, A))
        if not os.path.exists(b): return 'MISSING ' + b
        if open(a, 'rb').read() == open(b, 'rb').read(): return None
        fa, fb = funcs(a), funcs(b)
        ch = [n for n in fa if n in fb and fa[n] != fb[n]]
        on = [n for n in fa if n not in fb]; nn = [n for n in fb if n not in fa]
        da, db = data(a), data(b)
        dch = [s for s in set(da) | set(db) if da.get(s) != db.get(s)]
        if ch or on or nn or dch:
            return ' '.join([os.path.relpath(a, A), '| funcs:', ' '.join(ch), '| only old:', ' '.join(on), '| only new:', ' '.join(nn), '| data:', ' '.join(sorted(dch))])
        return None

import multiprocessing
objs = sorted(os.path.join(r, f) for r, _, fs in os.walk(A) for f in fs if f.endswith('.o'))
start = sys.argv[3] if len(sys.argv) > 3 else ''
objs = [o for o in objs if os.path.relpath(o, A) > start]
with multiprocessing.Pool(8) as p:
    for r in p.imap(one, objs):
        if r: print(r, flush=True)
