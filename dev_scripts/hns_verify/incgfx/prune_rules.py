#!/usr/bin/env python3
# usage: [PRUNE_BASE=<dir> PRUNE_THEIRS=<dir>] prune_rules.py <tree> <out-dir> [rules files...]
# Keep only the graphics rules (and variables) of <tree> that are still needed after the INCGFX migration.
# With PRUNE_BASE/PRUNE_THEIRS, a rule (or variable) that is unchanged from the upstream base and that
# upstream kept is kept as well, so that the shared part matches upstream.
import os, re, subprocess, sys

tree = sys.argv[1]; out = os.path.abspath(sys.argv[2])
files = sys.argv[3:] or ['graphics_file_rules.mk', 'spritesheet_rules.mk']
BASE = os.environ.get('PRUNE_BASE'); THEIRS = os.environ.get('PRUNE_THEIRS')
os.chdir(tree)

def tracked(pattern_re):
    fs = subprocess.run(['git', 'ls-files'], capture_output=True, text=True).stdout.split('\n')
    return [f for f in fs if f and re.search(pattern_re, f) and os.path.isfile(f)]

refs = set()
for f in tracked(r'^(src|include|test)/.*\.(c|h)$'):
    s = open(f, encoding='utf-8', errors='replace').read()
    for m in re.finditer(r'INCBIN(?:_[A-Z0-9]+)?\s*\(', s):
        depth = 1; i = m.end(); buf = ''
        while i < len(s) and depth:
            c = s[i]
            if c == '(':
                depth += 1
            elif c == ')':
                depth -= 1
            buf += c; i += 1
        refs.update(re.findall(r'"([^"]+)"', buf))
    for m in re.finditer(r'INCGFX(?:_[A-Z0-9]+)?\s*\(\s*"([^"]+)"', s):
        refs.add(m.group(1))
for f in tracked(r'\.(s|inc|S)$'):
    s = open(f, encoding='utf-8', errors='replace').read()
    refs.update(re.findall(r'\.incbin\s+"([^"]+)"', s))

def logical_lines(text):
    lines = text.split('\n'); res = []; cur = None; start = 0
    for i, l in enumerate(lines):
        if cur is None:
            cur = l; start = i
        else:
            cur += '\n' + l
        if l.endswith('\\'):
            continue
        res.append((start, i, cur)); cur = None
    if cur is not None:
        res.append((start, len(lines) - 1, cur))
    return res

VAR_RE = re.compile(r'^(\w+)\s*(:=|=|\?=)\s*(.*?)\s*$')
variables = {}
def expand(s, depth=0):
    if depth > 10:
        return s
    return re.sub(r'\$[({](\w+)[)}]', lambda m: expand(variables[m.group(1)], depth + 1) if m.group(1) in variables else m.group(0), s)

def expand_list(s):
    def subst(m):
        name, pat, repl = m.group(1), m.group(2), m.group(3)
        res = []
        for v in expand(variables.get(name, '')).split():
            if '%' in pat:
                pre, suf = pat.split('%', 1)
                if v.startswith(pre) and v.endswith(suf):
                    res.append(repl.replace('%', v[len(pre):len(v) - len(suf)] if suf else v[len(pre):]))
                    continue
            res.append(v)
        return ' '.join(res)
    s = re.sub(r'\$[({](\w+):([^=]*)=([^)}]*)[)}]', subst, s)
    return expand(s)

def flat(l):
    return l.replace('\\\n', ' ')

mk_order = ['config.mk', 'make_tools.mk', 'graphics_file_rules.mk', 'map_data_rules.mk', 'spritesheet_rules.mk',
            'json_data_rules.mk', 'audio_rules.mk', 'trainer_rules.mk']
mkfiles = ['Makefile'] + [f for f in mk_order if os.path.exists(f)] + \
          sorted(f for f in os.listdir('.') if f.endswith('.mk') and f not in mk_order)
texts = {f: open(f).read() for f in mkfiles}

def load_vars(f):
    for (_, _, l) in logical_lines(texts[f]):
        m = VAR_RE.match(flat(l))
        if m and not l.startswith('\t'):
            variables[m.group(1)] = m.group(3)

GFX_EXT = re.compile(r'\.(4bpp|8bpp|1bpp|2bpp|gbapal|bin|smol|lz|rl|latfont|hwjpnfont|fwjpnfont|fwkfont|smolTM|fastSmol)\b')

class Block:
    def __init__(s, f, text, kind, targets=(), prereqs=(), var=None):
        s.f, s.text, s.kind, s.targets, s.prereqs, s.var = f, text, kind, list(targets), list(prereqs), var
        s.keep = None

def split_colons(h):
    parts = []; depth = 0; cur = ''
    for ch in h:
        if ch in '({':
            depth += 1
        elif ch in ')}':
            depth -= 1
        if ch == ':' and depth == 0:
            parts.append(cur); cur = ''
        else:
            cur += ch
    parts.append(cur)
    return parts

blocks = []
load_vars('Makefile')
for f in mkfiles[1:]:
    if f not in files:
        load_vars(f)
        for (_, _, l) in logical_lines(texts[f]):
            for tok in expand_list(flat(l)).split():
                if GFX_EXT.search(tok) and '%' not in tok:
                    refs.add(tok)
        continue
    ll = logical_lines(texts[f]); i = 0
    while i < len(ll):
        _, _, l = ll[i]; fl = flat(l)
        if fl.strip() == '':
            blocks.append(Block(f, l, 'blank')); i += 1; continue
        if fl.lstrip().startswith('#'):
            blocks.append(Block(f, l, 'comment')); i += 1; continue
        m = VAR_RE.match(fl)
        if m and not fl.startswith('\t'):
            variables[m.group(1)] = m.group(3)
            blocks.append(Block(f, l, 'var', var=m.group(1))); i += 1; continue
        if fl.startswith('\t'):
            blocks.append(Block(f, l, 'other')); i += 1; continue
        parts = split_colons(fl.split(';', 1)[0])
        tgt = expand_list(parts[0]).split()
        pre = []
        if len(parts) >= 3:
            tpat = expand(parts[1].strip()); ppat = parts[2].strip()
            for t in tgt:
                tp, ts = tpat.split('%', 1)
                stem = t[len(tp):len(t) - len(ts)] if ts else t[len(tp):]
                pre += [p.replace('%', stem) for p in expand_list(ppat).split()]
        elif len(parts) == 2:
            pre = expand_list(parts[1]).split()
        j = i + 1; body = l
        while j < len(ll) and ll[j][2].startswith('\t'):
            body += '\n' + ll[j][2]; j += 1
        blocks.append(Block(f, body, 'rule', tgt, pre)); i = j

SUFFIXES = ('.smolTM', '.fastSmol', '.smol', '.lz', '.rl')
needed = set()
def add_needed(n):
    while n not in needed:
        needed.add(n)
        for sfx in SUFFIXES:
            if n.endswith(sfx):
                n = n[:-len(sfx)]
                break
        else:
            break
for r in refs:
    add_needed(r)

def is_pattern(t):
    return '%' in t
def pat_stem(p, t):
    pre, suf = p.split('%', 1)
    if t.startswith(pre) and t.endswith(suf) and len(t) >= len(pre) + len(suf):
        return t[len(pre):len(t) - len(suf)] if suf else t[len(pre):]
    return None

rules = [bl for bl in blocks if bl.kind == 'rule']
def propagate():
    changed = True
    while changed:
        changed = False
        for bl in rules:
            stems = []
            hit = False
            for t in bl.targets:
                if is_pattern(t):
                    for n in list(needed):
                        st = pat_stem(t, n)
                        if st is not None:
                            hit = True; stems.append(st)
                elif t in needed:
                    hit = True
            if not hit:
                continue
            before = len(needed)
            if not bl.keep:
                bl.keep = True; changed = True
            for p in bl.prereqs:
                if is_pattern(p):
                    for st in stems:
                        add_needed(p.replace('%', st))
                else:
                    add_needed(p)
            if len(needed) != before:
                changed = True
propagate()
need_only = [bl for bl in rules if bl.keep]

forced = []
if BASE and THEIRS:
    for f in files:
        bt = open(os.path.join(BASE, f)).read(); tt = open(os.path.join(THEIRS, f)).read()
        for bl in rules:
            if bl.f == f and not bl.keep and bl.text in bt and bl.text in tt:
                bl.keep = True; forced.append(bl)
    for bl in forced:
        for p in bl.prereqs:
            if not is_pattern(p):
                add_needed(p)
    propagate()

def var_used(bl):
    # used by a kept rule after this definition in the same file, or by a kept rule of a later rules
    # file that does not define the variable itself
    pat = re.compile(r'\$[({]' + re.escape(bl.var) + r'[):}]')
    idx = blocks.index(bl); seen_file = bl.f; later_files = files[files.index(bl.f) + 1:]
    for x in blocks[idx + 1:]:
        if x.f == seen_file:
            if x.kind == 'var' and x.var == bl.var:
                return False
            if x.kind == 'rule' and x.keep and pat.search(x.text):
                return True
    for lf in later_files:
        if any(x.f == lf and x.kind == 'var' and x.var == bl.var for x in blocks):
            break
        if any(x.f == lf and x.kind == 'rule' and x.keep and pat.search(x.text) for x in blocks):
            return True
    return False
for bl in blocks:
    if bl.kind == 'var':
        refd = var_used(bl)
        same_up = bool(BASE and THEIRS and bl.text in open(os.path.join(BASE, bl.f)).read() and bl.text in open(os.path.join(THEIRS, bl.f)).read())
        bl.keep = refd or same_up
    elif bl.kind == 'rule' and bl.keep is None:
        bl.keep = False
    elif bl.kind in ('blank', 'comment', 'other'):
        bl.keep = True

os.makedirs(out, exist_ok=True)
report = open(os.path.join(out, 'report.txt'), 'w')
for f in files:
    fb = [bl for bl in blocks if bl.f == f]
    res = []; prev_dropped = False
    for bl in fb:
        if bl.kind == 'blank' and prev_dropped:
            continue
        if bl.keep:
            res.append(bl); prev_dropped = False
        else:
            prev_dropped = True
    res2 = []
    for k, bl in enumerate(res):
        if bl.kind == 'comment' and bl.text.strip().startswith('###'):
            nxt = [x for x in res[k + 1:] if x.kind != 'blank']
            if not nxt or (nxt[0].kind == 'comment' and nxt[0].text.strip().startswith('###')):
                continue
        res2.append(bl)
    txt = '\n'.join(bl.text for bl in res2)
    txt = re.sub(r'\n{3,}', '\n\n', txt).lstrip('\n')
    if not txt.endswith('\n'):
        txt += '\n'
    open(os.path.join(out, f), 'w').write(txt)
    nk = sum(1 for bl in fb if bl.kind == 'rule' and bl.keep); nd = sum(1 for bl in fb if bl.kind == 'rule' and not bl.keep)
    nn = sum(1 for bl in need_only if bl.f == f); nf = sum(1 for bl in forced if bl.f == f)
    print(f, 'rules kept', nk, '(needed', nn, '+ upstream-kept', nf, ') dropped', nd)
    for bl in fb:
        if bl.kind == 'rule':
            report.write('%s\t%s\t%s\n' % (f, 'KEEP-NEED' if bl in need_only else ('KEEP-UP' if bl in forced else 'DROP'), ' '.join(bl.targets)))
