#!/usr/bin/env bash
# berry_rom_tables.sh <repo> <elf> <berry_ids.tsv> <out.tsv>
#   빌드된 ELF(make hns 결과 pokehns.elf)에서 열매 번호로 인덱싱되는 ROM 표를 아이템 기준으로 풀어 적는다.
#   이식 전: gBerries[b-1], gNaturalGiftTable[b], gBerryCrush_BerryData[b-1], sBerryPicTable[b-1],
#            gBerryTreePicTablePointers[b-1], gBerryTreePaletteSlotTablePointers[b-1]
#   이식 후: gBerries[b].info.*, .naturalGift*, .berryCrush*, .berryPic/.berryPal,
#            .berryTreePicTable, .berryTreePaletteSlotTable
#   (b = <berry_ids.tsv>의 ITEM 줄이 주는 그 빌드의 열매 번호)
#   구조체 레이아웃은 <repo>/include 로 -g 컴파일한 타입 TU의 DWARF에서 읽는다(ELF와 같은 트리여야 함).
#   diff 기대: ITEM_ENIGMA_BERRY_E_READER 의 naturalGift.* 2줄만 다름(이식 전 OOB 읽기 -> 이식 후 0).
set -euo pipefail
HERE=$(cd "$(dirname "$0")" && pwd)
REPO=$(cd "$1" && pwd)
ELF=$(realpath "$2")
IDS=$(realpath "$3")
OUT=$(realpath -m "$4")
TC=${TC:-/opt/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin}
WORK=$(mktemp -d "${TMPDIR:-/tmp}/berry_rom.XXXXXX")
trap 'rm -rf "$WORK"' EXIT

cat > "$WORK/types.c" <<'EOF'
#include "global.h"
#include "sprite.h"
#include "berry.h"
struct Berry gT_Berry;
struct SpriteFrameImage gT_Frame;
#if __has_include("constants/berries.h")
struct BerryInfo gT_BerryInfo;
#else
#include "battle_util.h"
struct TypePower gT_TypePower;
struct BerryCrushBerryData gT_Crush;
struct TilesPal { const u32 *tiles; const u16 *pal; } gT_TilesPal; // item_menu_icons.c 의 static 구조체와 동일
#endif
EOF
"$TC/arm-none-eabi-gcc" -mthumb -mthumb-interwork -O0 -g -mabi=apcs-gnu -mtune=arm7tdmi -march=armv4t \
    -std=gnu17 -w -iquote "$REPO/include" -DMODERN=1 -DTESTING=0 -DPOKEMON_HNS -c -o "$WORK/types.o" "$WORK/types.c"

python3 "$HERE/berry_rom_tables.py" "$WORK/types.o" "$ELF" "$IDS" > "$OUT"
echo "wrote $OUT ($(grep -vc '^#' "$OUT") rows, $(head -1 "$OUT"))" >&2
