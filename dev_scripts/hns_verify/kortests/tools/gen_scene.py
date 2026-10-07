#!/usr/bin/env python3
# seq 181 #9730 area E scratch tool (never committed).
# Fill the empty SCENE blocks of probe tests with the events recorded on the pre-port base copy.
#
# usage: gen_scene.py <tree> <decoded-trace.txt> <probe.c> <out.c>
#   <tree>   HnS copy with include/constants/{abilities,moves}.h (numeric ids in the trace are resolved there)
#   <trace>  output of trace_decode.py for a run of the probe file(s) (all tests PASS with empty SCENE)
#   <probe>  test file whose SCENE blocks are "    } SCENE {\n    }\n"
#   <out>    written test file: every recorded MESSAGE (except control-code / blank ones), ABILITY_POPUP,
#            ANIMATION(ANIM_TYPE_MOVE ...) and the stat / held-item / berry / snatch / syrup GENERAL animations,
#            in recorded order, without the default Celebrate turns and the battle intro, at most the last 30
#            (MAX_QUEUED_EVENTS). Item pop-ups (TR:I) are not visible to SCENE before upstream #10321; they stay
#            in the trace and are compared with tools/tracecmp.py.
import re
import sys

tree, trace_path, probe_path, out_path = sys.argv[1:5]

GENERAL_KEEP = {
    'B_ANIM_STATS_CHANGE', 'B_ANIM_HELD_ITEM_EFFECT', 'B_ANIM_HELD_ITEM_BERRY',
    'B_ANIM_SNATCH_MOVE', 'B_ANIM_SYRUP_BOMB_SPEED_DROP', 'B_ANIM_TOTEM_FLARE',
}


def load_enum(path, prefix):
    table = {}
    names = {}
    value = -1
    for line in open(path, encoding='utf-8'):
        m = re.match(r'^\s*([A-Z][A-Z0-9_]*)\s*(?:=\s*([^,/]+?))?\s*,', line)
        if not m:
            continue
        name, rhs = m.group(1), m.group(2)
        if rhs is None:
            value += 1
        else:
            rhs = rhs.strip()
            if re.fullmatch(r'\d+', rhs):
                value = int(rhs)
            elif rhs in table:
                value = table[rhs]
            else:
                value += 1
        table[name] = value
        if name.startswith(prefix):
            names.setdefault(value, name)
    return names


abilities = load_enum(tree + '/include/constants/abilities.h', 'ABILITY_')
moves = load_enum(tree + '/include/constants/moves.h', 'MOVE_')

blocks = {}
cur = None
for line in open(trace_path, encoding='utf-8'):
    line = line.rstrip('\n')
    if line.startswith('## '):
        m = re.match(r'^## (.*): ([A-Z_]+)$', line)
        cur = m.group(1)
        blocks[cur] = {'result': m.group(2), 'events': []}
        continue
    if cur is not None:
        blocks[cur]['events'].append(line)


def battler_name(b, doubles):
    b = int(b)
    if doubles:
        return ['playerLeft', 'opponentLeft', 'playerRight', 'opponentRight'][b]
    return ['player', 'opponent'][b]


MAX_EVENTS = 30  # include/test/battle.h MAX_QUEUED_EVENTS


def scene_lines(events, doubles):
    out = []
    leading = True
    for ev in events:
        # noise: the default Celebrate turns, and the battle intro before the first real event
        if ev.startswith('MSG  ') and ('축하를 썼다!' in ev or ev.startswith('MSG  축하합니다,')):
            continue
        if ev.startswith('ANIM MOVE MOVE_CELEBRATE'):
            continue
        if leading and ev.startswith('MSG  ') and ('내보냈다!' in ev or ev.startswith('MSG  가랏!') or '{' in ev or ev[5:].strip() == ''):
            continue
        if not ev.startswith('-- run'):
            leading = False
        if ev.startswith('-- run'):
            continue
        if ev.startswith('MSG  '):
            text = ev[5:]
            if '{' in text or text.strip() == '':
                continue
            text = text.rstrip('¶')
            text = text.replace('⏎', ' ').replace('⇡', ' ').replace('¶', ' ')
            assert '"' not in text and '\\' not in text, text
            out.append('        MESSAGE("%s");' % text)
        elif ev.startswith('POPUP '):
            m = re.match(r'POPUP b=(\d+) (\S+)', ev)
            ab = m.group(2)
            if ab.isdigit():
                ab = abilities[int(ab)]
            out.append('        ABILITY_POPUP(%s, %s);' % (battler_name(m.group(1), doubles), ab))
        elif ev.startswith('ANIM '):
            m = re.match(r'ANIM (\w+) (\S+) atk=(\d+) tgt=(\d+)', ev)
            kind, anim, atk = m.group(1), m.group(2), m.group(3)
            if kind == 'MOVE':
                if anim.isdigit():
                    anim = moves[int(anim)]
                out.append('        ANIMATION(ANIM_TYPE_MOVE, %s, %s);' % (anim, battler_name(atk, doubles)))
            elif kind == 'GENERAL' and anim in GENERAL_KEEP:
                out.append('        ANIMATION(ANIM_TYPE_GENERAL, %s, %s);' % (anim, battler_name(atk, doubles)))
    if len(out) > MAX_EVENTS:  # keep the last events (the turn under test); earlier ones are setup
        out = ['        // (setup events before this point omitted: MAX_QUEUED_EVENTS = 30)'] + out[-MAX_EVENTS:]
    return out


src = open(probe_path, encoding='utf-8').read()
pat = re.compile(r'((SINGLE|DOUBLE)_BATTLE_TEST\("([^"]+)"\)\n\{.*?\n    \} SCENE \{\n)(    \}(?: THEN \{)?\n)', re.S)
missing = []


def fill(m):
    name = m.group(3)
    doubles = m.group(2) == 'DOUBLE'
    if name not in blocks:
        missing.append(name)
        return m.group(0)
    assert blocks[name]['result'] == 'PASS', (name, blocks[name]['result'])
    lines = scene_lines(blocks[name]['events'], doubles)
    return m.group(1) + ''.join(l + '\n' for l in lines) + m.group(4)


out = pat.sub(fill, src)
if missing:
    sys.exit('no trace block for: ' + ', '.join(missing))
open(out_path, 'w', encoding='utf-8').write(out)
print('%s: %d tests filled' % (out_path, len(pat.findall(src))))
