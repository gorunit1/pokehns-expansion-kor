#!/usr/bin/env python3
# HNS9717 seq150 scratch tool (never committed).
# Decode the "TR:" event lines printed by trace.patch into a readable, diffable trace.
#
# usage: trace_decode.py <repo-copy-root> <runner-log> [> out.txt]
#   <repo-copy-root>: a tree that has charmap.txt and include/constants/*.h (any HnS copy)
#   <runner-log>:     stdout of `make check BUILD=hns TESTS=...` on a tree with trace.patch applied
#
# Output: one block per test result line, events in recorded order:
#   MSG  <decoded Korean text, \n shown as ⏎>
#   POPUP b=<battler> <ABILITY_...>
#   ITEM  b=<battler> <ITEM_...>          (item pop-up, CreateItemPopUp)
#   ANIM <TYPE> <ID name> atk=<b> tgt=<b>
#   HP   b=<battler> old->new
#   STATUS b=<battler> <status1>
#   ...
import re
import sys

root = sys.argv[1]
log = sys.argv[2]


def load_charmap(path):
    multi = {}
    single = {}
    for line in open(path, encoding='utf-8'):
        m = re.match(r"^'((?:\\.|[^'\\])+)'\s*=\s*((?:[0-9A-Fa-f]{2}\s*)+)", line)
        if not m:
            continue
        ch = m.group(1)
        bs = tuple(int(x, 16) for x in m.group(2).split())
        if ch == '\\n':
            ch = '⏎'
        elif ch == '\\l':
            ch = '⇡'
        elif ch == '\\p':
            ch = '¶'
        if len(bs) == 1:
            b = bs[0]
            # prefer ASCII / Latin over the Japanese table that shares low bytes
            if b not in single or (ch.isascii() and not single[b].isascii()):
                single[b] = ch
        else:
            multi.setdefault(bs, ch)
    return single, multi


def load_enum(path, prefix):
    names = {}
    for line in open(path, encoding='utf-8'):
        m = re.match(r'^\s*(' + prefix + r'[A-Z0-9_]+)\s*=\s*(\d+)\s*,', line)
        if m:
            names.setdefault(int(m.group(2)), m.group(1))
    return names


def load_defines(path, prefix, lo_line_pat=None):
    names = {}
    for line in open(path, encoding='utf-8'):
        m = re.match(r'^#define\s+(' + prefix + r'[A-Z0-9_]+)\s+(\d+)\b', line)
        if m:
            names.setdefault(int(m.group(2)), m.group(1))
    return names


single, multi = load_charmap(root + '/charmap.txt')
abilities = load_enum(root + '/include/constants/abilities.h', 'ABILITY_')
moves = load_enum(root + '/include/constants/moves.h', 'MOVE_')
items = load_enum(root + '/include/constants/items.h', 'ITEM_')

# general / status / special animation ids
gen, sta, spe = {}, {}, {}
section = None
for line in open(root + '/include/constants/battle_anim.h', encoding='utf-8'):
    if 'sBattleAnims_General' in line:
        section = gen
    elif 'sBattleAnims_Special' in line or 'special animations' in line.lower():
        section = spe
    elif 'sBattleAnims_StatusConditions' in line:
        section = sta
    m = re.match(r'^#define\s+(B_ANIM_[A-Z0-9_]+)\s+(\d+)\b', line)
    if m and section is not None and not m.group(1).startswith('B_ANIM_ARG'):
        section.setdefault(int(m.group(2)), m.group(1))
ANIM_TYPES = {0: 'GENERAL', 1: 'MOVE', 2: 'STATUS', 3: 'SPECIAL'}


def decode(hexstr):
    data = bytes.fromhex(hexstr)
    out = []
    i = 0
    while i < len(data):
        for n in (3, 2):
            key = tuple(data[i:i + n])
            if len(key) == n and key in multi:
                out.append(multi[key])
                i += n
                break
        else:
            b = data[i]
            out.append(single.get(b, '{%02X}' % b))
            i += 1
    return ''.join(out)


ansi = re.compile(r'\x1b\[[0-9;]*[A-Za-z]')
result_re = re.compile(r'^\[\d+\] (.*?): (PASS|FAIL|KNOWN_FAILING|TO_DO|ASSUMPTIONS_FAILED|INVALID|ERROR|CRASH|TIMEOUT|.*?)\s*$')
blocks = {}
order = []
cur = ['## (before first result line)']
blocks[cur[0]] = cur


def emit(text):
    cur.append(text)


def print(text):  # noqa: A001 - collect instead of printing directly
    emit(text)


pending = None
for raw in open(log, encoding='utf-8', errors='replace'):
    line = ansi.sub('', raw.rstrip('\n'))
    if line.startswith('TR:'):
        tag, _, rest = line[3:].partition(' ')
        if tag == 'M+':
            if pending is not None:
                pending += rest
            continue
        if pending is not None:
            print('MSG  ' + decode(pending))
            pending = None
        if tag == 'M':
            pending = rest
        elif tag == 'A':
            m = re.match(r'b=(\d+) ab=(\d+)', rest)
            print('POPUP b=%s %s' % (m.group(1), abilities.get(int(m.group(2)), m.group(2))))
        elif tag == 'I':
            m = re.match(r'b=(\d+) item=(\d+)', rest)
            print('ITEM  b=%s %s' % (m.group(1), items.get(int(m.group(2)), m.group(2))))
        elif tag == 'N':
            m = re.match(r't=(\d+) id=(\d+) atk=(\d+) tgt=(\d+)', rest)
            t, i = int(m.group(1)), int(m.group(2))
            table = {0: gen, 1: moves, 2: sta, 3: spe}.get(t, {})
            print('ANIM %s %s atk=%s tgt=%s' % (ANIM_TYPES.get(t, t), table.get(i, i), m.group(3), m.group(4)))
        elif tag == 'H':
            print('HP   ' + rest)
        elif tag == 'S':
            print('STATUS ' + rest)
        elif tag == 'RUN':
            print('-- run ' + rest)
        else:
            print(tag + ' ' + rest)
        continue
    if pending is not None:
        print('MSG  ' + decode(pending))
        pending = None
    m = result_re.match(line)
    if m:
        key = '## %s: %s' % (m.group(1), m.group(2))
        cur = [key]
        blocks[key] = cur
if pending is not None:
    print('MSG  ' + decode(pending))

import builtins
for key in sorted(blocks):
    lines = blocks[key]
    if key.startswith('## (before') and len(lines) == 1:
        continue
    for l in lines:
        builtins.print(l)
