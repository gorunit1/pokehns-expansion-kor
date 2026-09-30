#!/usr/bin/env bash
# berry_ids.sh <repo> <out.tsv>
#   HnS 저장소 헤더를 호스트 gcc로 컴파일해 "아이템 -> 열매 번호" 표와 경계 상수를 뽑는다.
#   - 이식 전(include/constants/berries.h 없음): ITEM_TO_BERRY + berry.c의 clamp 규칙을 흉내
#   - 이식 후(include/constants/berries.h 있음): item.h의 실제 static inline
#     ItemIdToBerryType()/BerryTypeToItemId()를 호출
#   저장소는 읽기만 한다. 산출물은 <out.tsv> 하나.
#   diff 기대: CONST/ITEM/BERRY 줄 0 차이. BERRY_INVALID 줄은 의도된 차이(문서 참조).
set -euo pipefail
REPO=$(cd "$1" && pwd)
OUT=$2
WORK=$(mktemp -d "${TMPDIR:-/tmp}/berry_ids.XXXXXX")
trap 'rm -rf "$WORK"' EXIT

# 1) items.h의 FIRST_BERRY_INDEX ~ LAST_BERRY_INDEX 블록에서 열매 아이템 상수 이름을 순서대로 수집
python3 - "$REPO/include/constants/items.h" > "$WORK/names.inc" <<'PY'
import re, sys
src = open(sys.argv[1], encoding='utf-8').read()
start = src.index('FIRST_BERRY_INDEX =')
end = src.index('LAST_BERRY_INDEX', start)
names = re.findall(r'\b(ITEM_[A-Z0-9_]+)\s*=', src[start:end])
for n in names:
    print('    { %s, "%s" },' % (n, n))
PY

cat > "$WORK/berry_ids.c" <<'EOF'
#include "global.h"
#include "item.h"
#include "constants/berry.h"
#include <stdio.h>

#if __has_include("constants/berries.h")
#define POST 1
#else
#define POST 0
#endif

struct E { int id; const char *name; };
static const struct E sItems[] = {
#include "names.inc"
};
#define N (int)(sizeof(sItems) / sizeof(sItems[0]))

static const char *NameOf(int item)
{
    for (int i = 0; i < N; i++)
        if (sItems[i].id == item)
            return sItems[i].name;
    return item == 0 ? "ITEM_NONE" : "?";
}

#if POST
static int ItemToBerry(int item) { return (int)ItemIdToBerryType((enum Item)item); }
static int BerryToItem(int b)    { return (int)BerryTypeToItemId((enum BerryId)b); }
static const int sNumBerries = NUM_BERRIES;
static const int sEReader = BERRY_ID_ENGIMA_E_READER;
static const int sEnigma = BERRY_ID_ENIGMA;
#else
// HnS berry.c(이식 전) ItemIdToBerryType / BerryTypeToItemId 와 같은 규칙
static int ItemToBerry(int item)
{
    unsigned short b = item - FIRST_BERRY_INDEX;
    if (b > LAST_BERRY_INDEX - FIRST_BERRY_INDEX)
        return ITEM_TO_BERRY(FIRST_BERRY_INDEX);
    return ITEM_TO_BERRY(item);
}
static int BerryToItem(int berry)
{
    unsigned short item = berry - 1;
    if (item > LAST_BERRY_INDEX - FIRST_BERRY_INDEX)
        return FIRST_BERRY_INDEX;
    return berry + FIRST_BERRY_INDEX - 1;
}
static const int sNumBerries = ITEM_TO_BERRY(LAST_BERRY_INDEX);
static const int sEReader = ITEM_TO_BERRY(ITEM_ENIGMA_BERRY_E_READER);
static const int sEnigma = ITEM_TO_BERRY(ITEM_ENIGMA_BERRY);
#endif

int main(void)
{
    struct BerryTree t = {0};
    int bits = 0;
    t.berry = 0x7F; while ((t.berry >> bits) & 1) bits++;  // host에서 berry 비트 폭 확인(참고용)

    printf("#mode\t%s\n", POST ? "post" : "pre");
    printf("CONST\tBERRY_NONE\t%d\n", BERRY_NONE);
    printf("CONST\tFIRST_BERRY_INDEX\t%d\n", (int)FIRST_BERRY_INDEX);
    printf("CONST\tLAST_BERRY_INDEX\t%d\n", (int)LAST_BERRY_INDEX);
    printf("CONST\tNUM_BERRIES(=last valid berry no)\t%d\n", sNumBerries);
    printf("CONST\tENIGMA_BERRY no\t%d\n", sEnigma);
    printf("CONST\tENIGMA_BERRY_E_READER no\t%d\n", sEReader);
    printf("CONST\tBERRY_TREES_COUNT\t%d\n", BERRY_TREES_COUNT);
    printf("CONST\tBerryTree.berry bits(>=)\t%d\n", bits);
    printf("CONST\tberry item count(items.h block)\t%d\n", N);

    // 아이템 -> 열매 번호 (저장값 berryTrees[].berry 로 쓰이는 번호)
    for (int i = 0; i < N; i++)
        printf("ITEM\t%d\t%s\t%d\n", sItems[i].id, sItems[i].name, ItemToBerry(sItems[i].id));

    // 저장값 v(0..127, 7비트) -> 아이템 (기존 세이브 값을 새 코드가 해석한 결과)
    for (int v = 0; v < 128; v++)
    {
        int item = BerryToItem(v);
        if (v >= 1 && v <= sNumBerries)
            printf("BERRY\t%d\t%d\t%s\n", v, item, NameOf(item));
        else
            printf("BERRY_INVALID\t%d\t%d\t%s\n", v, item, NameOf(item));
    }

#if POST
    // 역함수 일관성: 모든 아이템 ID에 대해 ItemIdToBerryType != 0 인 것은 정확히 열매 블록뿐이어야 한다
    int mapped = 0, bad = 0;
    for (int item = 0; item < ITEMS_COUNT; item++)
    {
        int b = ItemToBerry(item);
        if (b == 0) continue;
        mapped++;
        if (BerryToItem(b) != item || item < FIRST_BERRY_INDEX || item > LAST_BERRY_INDEX)
        {
            bad++;
            printf("CHECK_FAIL\titem %d -> berry %d -> item %d\n", item, b, BerryToItem(b));
        }
    }
    fprintf(stderr, "[post] items mapped to a berry: %d, inconsistencies: %d\n", mapped, bad);
#endif
    return 0;
}
EOF

gcc -std=gnu17 -w -iquote "$REPO/include" -I "$WORK" -DMODERN=1 -DTESTING=0 -DPOKEMON_HNS \
    -o "$WORK/berry_ids" "$WORK/berry_ids.c"
"$WORK/berry_ids" > "$OUT"
echo "wrote $OUT ($(grep -c '^ITEM' "$OUT") ITEM rows, mode=$(head -1 "$OUT" | cut -f2))" >&2
