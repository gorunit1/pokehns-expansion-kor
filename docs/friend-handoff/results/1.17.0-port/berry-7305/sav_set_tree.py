#!/usr/bin/env python3
"""실기 확인용: 세이브(.sav)의 최신 슬롯에서 berryTrees[treeId] 를 원하는 열매 번호/단계로 바꾼 사본을 만든다.

사용: sav_set_tree.py <in.sav> <out.sav> <save_layout.txt> TREE=BERRY:STAGE[:YIELD] ...
  예) sav_set_tree.py hns.sav hns_occa.sav pre/save_layout.txt 90=37:5:3 91=36:5:3 92=53:5:3
      (90~127 은 HnS 조토·관동 나무 ID. 37=OCCA, 36=CHILAN, 53=ROSELI, 52=BABIRI, 54=LIECHI, 65=ROWAP, 61=ENIGMA)
  <in.sav> 는 건드리지 않는다. 섹터 checksum 을 다시 계산한다(SaveBlock3 청크는 checksum 대상 아님).
  이 사본을 이식 전 ROM / 이식 후 ROM 에 각각 넣어 같은 나무 그림·열매 이름·수확 아이템이 나오는지 본다.
"""
import struct
import sys

SECTOR_SIZE, DATA = 4096, 3968
FOOTER = DATA + 116
SIG = 0x08012025

src, dst, layout = sys.argv[1:4]
specs = sys.argv[4:]
sav = bytearray(open(src, 'rb').read())

sb1_size = tree_off = None
fields = {}
for line in open(layout):
    if line.startswith('STRUCT SaveBlock1 size='):
        sb1_size = int(line.split('=')[1])
    p = line.rstrip('\n').split('\t')
    if len(p) == 6 and p[0] == 'SaveBlock1' and p[1].startswith('berryTrees[') and '.' not in p[1]:
        tree_off = int(p[2])
    if len(p) == 6 and p[0] == 'BerryTree':
        fields[p[1]] = (int(p[3]), int(p[4]))

best = None
for slot in range(2):
    secs, counter = {}, -1
    for i in range(14):
        base = (slot * 14 + i) * SECTOR_SIZE
        sid, chk, sig, cnt = struct.unpack_from('<HHII', sav, base + FOOTER)
        if sig == SIG:
            secs[sid] = base
            counter = max(counter, cnt)
    if len(secs) == 14 and (best is None or counter > best[0]):
        best = (counter, slot, secs)
if best is None:
    raise SystemExit('no valid slot')
_, slot, secs = best


def sb1_loc(off):
    chunk = off // DATA
    return 1 + chunk, off % DATA


touched = set()
for spec in specs:
    t, rest = spec.split('=')
    vals = [int(x) for x in rest.split(':')]
    t = int(t)
    base_off = tree_off + t * 8
    sid, o = sb1_loc(base_off)
    if o + 8 > DATA:
        raise SystemExit('tree %d straddles a sector boundary; not supported' % t)
    pos = secs[sid] + o
    raw = int.from_bytes(sav[pos:pos + 8], 'little')

    def put(name, v):
        global raw
        bit, bits = fields[name]
        raw = (raw & ~(((1 << bits) - 1) << bit)) | ((v & ((1 << bits) - 1)) << bit)
    put('berry', vals[0])
    put('stage', vals[1])
    if len(vals) > 2:
        put('berryYield', vals[2])
    put('stopGrowth', 0)
    sav[pos:pos + 8] = raw.to_bytes(8, 'little')
    touched.add(sid)
    print('tree %d -> berry %d stage %d%s (sector id %d)' % (t, vals[0], vals[1],
          (' yield %d' % vals[2]) if len(vals) > 2 else '', sid))

for sid in touched:
    base = secs[sid]
    size = min(sb1_size - (sid - 1) * DATA, DATA)
    s = sum(struct.unpack_from('<%dI' % (size // 4), sav, base)) & 0xFFFFFFFF
    chk = ((s >> 16) + s) & 0xFFFF
    struct.pack_into('<H', sav, base + FOOTER + 2, chk)
open(dst, 'wb').write(sav)
print('wrote %s (slot %d)' % (dst, slot))
