#!/usr/bin/env bash
# layout.sh <repo> <out.txt>
#   HnS 빌드와 같은 ARM 플래그(+ -g)로 세이브 구조체만 담은 TU를 스크래치에서 컴파일하고
#   DWARF에서 SaveBlock1/2/3 및 열매 관련 세이브 구조체의 평탄화 레이아웃을 뽑는다.
#   저장소는 읽기만 한다(-iquote <repo>/include). 산출물은 <out.txt> 하나.
#   diff 기대: 0 줄 (이식 전후 세이브 레이아웃 동일).
set -euo pipefail
HERE=$(cd "$(dirname "$0")" && pwd)
REPO=$(cd "$1" && pwd)
OUT=$2
TC=${TC:-/opt/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin}
WORK=$(mktemp -d "${TMPDIR:-/tmp}/berry_layout.XXXXXX")
trap 'rm -rf "$WORK"' EXIT

cat > "$WORK/save_layout.c" <<'EOF'
#include "global.h"
// 세이브에 들어가는 구조체(열매 번호·아이템을 담을 수 있는 것 위주) + 세이브 블록 전체
struct SaveBlock1 gL_SaveBlock1;
struct SaveBlock2 gL_SaveBlock2;
struct SaveBlock3 gL_SaveBlock3;
struct BerryTree gL_BerryTree;
struct EnigmaBerry gL_EnigmaBerry;            // FREE_ENIGMA_BERRY==TRUE 이면 SaveBlock1에는 없음
struct BattleEnigmaBerry gL_BattleEnigmaBerry;
struct WonderNewsMetadata gL_WonderNewsMetadata; // FREE_MYSTERY_GIFT==TRUE 이면 SaveBlock1에는 없음
struct BerryCrush gL_BerryCrush;
struct BerryPickingResults gL_BerryPickingResults;
struct Pokeblock gL_Pokeblock;
struct Bag gL_Bag;
struct ItemSlot gL_ItemSlot;
EOF

"$TC/arm-none-eabi-gcc" -mthumb -mthumb-interwork -O0 -g -mabi=apcs-gnu -mtune=arm7tdmi -march=armv4t \
    -std=gnu17 -w -iquote "$REPO/include" -DMODERN=1 -DTESTING=0 -DPOKEMON_HNS \
    -c -o "$WORK/save_layout.o" "$WORK/save_layout.c"

python3 "$HERE/dwarf_layout.py" "$WORK/save_layout.o" \
    SaveBlock1 SaveBlock2 SaveBlock3 BerryTree EnigmaBerry BattleEnigmaBerry WonderNewsMetadata \
    BerryCrush BerryPickingResults Pokeblock Bag ItemSlot > "$OUT"
echo "wrote $OUT ($(wc -l < "$OUT") lines)" >&2
grep '^STRUCT' "$OUT" >&2
