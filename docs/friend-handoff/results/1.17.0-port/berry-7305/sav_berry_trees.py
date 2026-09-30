#!/usr/bin/env python3
"""기존 세이브(.sav, 128KB 플래시)의 SaveBlock1.berryTrees[] 를 풀어 열매 번호 -> 아이템으로 해석한다.

사용: sav_berry_trees.py <save.sav> <save_layout.txt> <berry_ids.tsv> [--all]
  save_layout.txt : layout.sh 산출물(해당 빌드의 SaveBlock1/BerryTree 레이아웃)
  berry_ids.tsv   : berry_ids.sh 산출물(해당 빌드의 열매 번호 -> 아이템)
이식 전 표 두 개로 한 번, 이식 후 표 두 개로 한 번 돌려 출력이 같으면
"기존 세이브의 나무를 새 코드가 같은 열매로 읽는다"는 데이터 수준 확인이 된다.
(기본은 stage != 0 인 나무만 출력. --all 이면 128그루 전부)
"""
import struct
import sys

SECTOR_SIZE = 4096
SECTOR_DATA_SIZE = 3968
FOOTER_OFF = SECTOR_DATA_SIZE + 116  # saveBlock3Chunk 뒤
SIGNATURE = 0x08012025
SB1_START, SB1_END, PER_SLOT = 1, 4, 14

sav_path, layout_path, ids_path = sys.argv[1:4]
show_all = '--all' in sys.argv
data = open(sav_path, 'rb').read()
if len(data) < 0x20000:
    raise SystemExit('save too small: %d bytes' % len(data))

# 레이아웃
sb1_size = None
tree_off = None
tree_fields = {}
for line in open(layout_path):
    if line.startswith('STRUCT SaveBlock1 size='):
        sb1_size = int(line.split('=')[1])
    p = line.rstrip('\n').split('\t')
    if len(p) == 6 and p[0] == 'SaveBlock1' and p[1].startswith('berryTrees['):
        if p[1].startswith('berryTrees[') and p[1].endswith(']') and '.' not in p[1]:
            tree_off = int(p[2])
            tree_count = int(p[1][len('berryTrees['):-1])
    if len(p) == 6 and p[0] == 'BerryTree':
        tree_fields[p[1]] = (int(p[3]), int(p[4]))  # bit offset in struct, bit size
if sb1_size is None or tree_off is None:
    raise SystemExit('layout file missing SaveBlock1/berryTrees')

# 열매 번호 -> 아이템
berry2item = {}
for line in open(ids_path):
    p = line.rstrip('\n').split('\t')
    if p[0] in ('BERRY', 'BERRY_INVALID'):
        berry2item[int(p[1])] = (p[0], p[3])

# 슬롯 선택(가장 큰 counter, 14개 섹터 모두 서명 정상)
best = None
for slot in range(2):
    secs = {}
    counter = -1
    for i in range(PER_SLOT):
        base = (slot * PER_SLOT + i) * SECTOR_SIZE
        sid, chk, sig, cnt = struct.unpack_from('<HHII', data, base + FOOTER_OFF)
        if sig != SIGNATURE:
            continue
        secs[sid] = base
        counter = max(counter, cnt)
    if len(secs) == PER_SLOT and (best is None or counter > best[0]):
        best = (counter, slot, secs)
if best is None:
    raise SystemExit('no valid save slot')
counter, slot, secs = best
sb1 = b''.join(data[secs[i]:secs[i] + SECTOR_DATA_SIZE] for i in range(SB1_START, SB1_END + 1))[:sb1_size]

print('#save\t%s\tslot=%d\tcounter=%d' % (sav_path, slot, counter))
print('#treeId\tberryNo\tstage\titem(via berry_ids)\tclass')
tsize = 8
for t in range(tree_count):
    raw = int.from_bytes(sb1[tree_off + t * tsize: tree_off + (t + 1) * tsize], 'little')
    get = lambda f: (raw >> tree_fields[f][0]) & ((1 << tree_fields[f][1]) - 1)
    berry, stage = get('berry'), get('stage')
    if stage == 0 and not show_all:
        continue
    cls, item = berry2item.get(berry, ('?', '?'))
    print('%d\t%d\t%d\t%s\t%s' % (t, berry, stage, item, cls))
