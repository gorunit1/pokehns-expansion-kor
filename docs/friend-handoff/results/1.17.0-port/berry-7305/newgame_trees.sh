#!/usr/bin/env bash
# newgame_trees.sh <repo> <out.tsv> [elf]
#   data/scripts/new_game.inc 의 EventScript_ResetAllBerries(새 게임 초기 나무)를
#   (1) 저장소 소스로 스크래치에서 직접 어셈블해 바이트를 뽑고,
#   (2) [elf] 가 주어지면 빌드된 ELF(pokehns.elf)의 같은 심볼 바이트와도 대조한다.
#   출력: idx, treeId, BERRY_TREE_* 이름, 열매 번호(저장값), 소스의 열매 토큰(정규화), stage
#   diff 기대: 0 줄 (이식 전후 (treeId, 열매 번호, stage, 열매 이름) 동일).
#   저장소는 읽기만 한다(어셈블러는 저장소 루트에서 실행하지만 출력은 스크래치).
set -euo pipefail
HERE=$(cd "$(dirname "$0")" && pwd)
REPO=$(cd "$1" && pwd)
OUT=$(realpath -m "$2")
ELF=${3:-}
[ -n "$ELF" ] && ELF=$(realpath "$ELF")
TC=${TC:-/opt/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin}
WORK=$(mktemp -d "${TMPDIR:-/tmp}/berry_newgame.XXXXXX")
trap 'rm -rf "$WORK"' EXIT

# event_scripts.s 의 머리(#include 들과 .include 매크로)만 가져와 new_game.inc 만 어셈블
awk '/^\t\.section script_data/ {exit} {print}' "$REPO/data/event_scripts.s" > "$WORK/stub.s"
printf '\t.section script_data, "aw", %%progbits\n\t.include "data/scripts/new_game.inc"\n' >> "$WORK/stub.s"

cd "$REPO"
"$REPO/tools/preproc/preproc" -s "$WORK/stub.s" charmap.txt \
  | "$TC/arm-none-eabi-cpp" -iquote include -Wno-trigraphs -DMODERN=1 -DTESTING=0 -DPOKEMON_HNS -std=gnu17 -I include - \
  | "$REPO/tools/preproc/preproc" -ie "$WORK/stub.s" charmap.txt \
  | "$TC/arm-none-eabi-as" -mcpu=arm7tdmi -march=armv4t -meabi=5 --defsym MODERN=1 --defsym POKEMON_HNS=1 \
      -o "$WORK/stub.o"

# 소스 쪽: 전처리 후 실제로 남는 setberrytree 줄(#if IS_HNS 반영)을 순서대로
"$TC/arm-none-eabi-cpp" -P -iquote include -DMODERN=1 -DTESTING=0 -DPOKEMON_HNS -std=gnu17 -I include \
    -include include/constants/global.h data/scripts/new_game.inc 2>/dev/null \
  | awk '/EventScript_ResetAllBerries::/{if(!d)f=1;next} f&&/::/{f=0;d=1} f&&/setberrytree/{print}' > "$WORK/src_lines.txt"
# (awk reads all input instead of 'exit': an early exit made cpp fail with EPIPE under pipefail at random)

python3 - "$WORK/stub.o" "$REPO/include/constants/berry.h" "$WORK/src_lines.txt" "$OUT" "$ELF" "$HERE" <<'PY'
import re, sys
sys.path.insert(0, sys.argv[6])
from elfsym import Elf
obj, berry_h, src_lines, out, elf = sys.argv[1:6]

def decode(blob):
    rows, i = [], 0
    while i < len(blob):
        op = blob[i]
        if op == 0x03:  # SCR_OP_RETURN
            return rows, i + 1
        if op != 0x8A:  # SCR_OP_SETBERRYTREE
            raise SystemExit('unexpected opcode 0x%02x at +%d' % (op, i))
        rows.append((blob[i + 1], blob[i + 2], blob[i + 3]))
        i += 4
    raise SystemExit('no return found')

o = Elf(obj)
blob = o.read_sym('EventScript_ResetAllBerries', 4 * 256 + 1)
rows, used = decode(blob)

tree_names = {}
for m in re.finditer(r'#define\s+(BERRY_TREE_\w+)\s+(\d+)', open(berry_h).read()):
    tree_names.setdefault(int(m.group(2)), m.group(1))

toks = []
for line in open(src_lines):
    args = [a.strip() for a in line.split('setberrytree', 1)[1].split(',')]
    b = args[1]
    m = re.match(r'ITEM_TO_BERRY\(\s*ITEM_(\w+)_BERRY\s*\)$', b) or re.match(r'BERRY_ID_(\w+)$', b)
    toks.append(m.group(1) if m else 'RAW:' + b)
if len(toks) != len(rows):
    raise SystemExit('source lines %d != assembled rows %d' % (len(toks), len(rows)))

with open(out, 'w') as f:
    f.write('#idx\ttreeId\ttreeConst\tberryNo\tberryToken\tstage\n')
    for i, ((t, b, s), tok) in enumerate(zip(rows, toks)):
        f.write('%d\t%d\t%s\t%d\t%s\t%d\n' % (i, t, tree_names.get(t, '?'), b, tok, s))

msg = 'assembled %d setberrytree rows (%d bytes)' % (len(rows), used)
if elf:
    e = Elf(elf)
    eb = e.read_sym('EventScript_ResetAllBerries', used)
    msg += '; ELF bytes %s' % ('MATCH' if eb == blob[:used] else 'MISMATCH (rebuild? stale ELF?)')
    if eb != blob[:used]:
        print(msg, file=sys.stderr); sys.exit(2)
print(msg, file=sys.stderr)
PY
