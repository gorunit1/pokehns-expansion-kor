#!/usr/bin/env bash
# verify.sh <repo> <elf> <outdir> [baseline_dir]
#   #7305 이식 후 세이브 호환 검증 일괄 실행. baseline_dir(기본: 이 스크립트 옆 pre/)과 비교한다.
#   (a) 아이템->열매 번호 표          : BERRY_INVALID 줄 제외 diff 0 이어야 PASS
#   (b) 세이브 구조체 레이아웃(DWARF) : diff 0 이어야 PASS
#   (c) 새 게임 초기 나무 118그루     : diff 0 이어야 PASS (+ ELF 바이트와 소스 어셈블 결과 일치)
#   (d) 열매 번호 인덱스 ROM 표 덤프  : ITEM_ENIGMA_BERRY_E_READER naturalGift.* 외 diff 0 이어야 PASS
#   저장소는 읽기만 한다. <elf> 는 반드시 같은 트리에서 `make hns -j8` 로 만든 pokehns.elf.
set -uo pipefail
HERE=$(cd "$(dirname "$0")" && pwd)
REPO=$1
ELF=$2
OUT=$3
BASE=${4:-$HERE/pre}
mkdir -p "$OUT"
fail=0
res() { printf '%-4s %s\n' "$1" "$2"; [ "$1" = PASS ] || fail=1; }

"$HERE/berry_ids.sh" "$REPO" "$OUT/berry_ids.tsv" 2>"$OUT/berry_ids.log" || res FAIL "(a) berry_ids.sh 실행 실패 ($OUT/berry_ids.log)"
"$HERE/layout.sh" "$REPO" "$OUT/save_layout.txt" 2>"$OUT/layout.log" || res FAIL "(b) layout.sh 실행 실패"
"$HERE/newgame_trees.sh" "$REPO" "$OUT/newgame_trees.tsv" "$ELF" 2>"$OUT/newgame.log" || res FAIL "(c) newgame_trees.sh 실행 실패/ELF 불일치 ($OUT/newgame.log)"
"$HERE/berry_rom_tables.sh" "$REPO" "$ELF" "$OUT/berry_ids.tsv" "$OUT/berry_rom_tables.tsv" 2>"$OUT/rom.log" || res FAIL "(d) berry_rom_tables.sh 실행 실패 ($OUT/rom.log)"

filt_ids() { grep -v -e '^#' -e '^BERRY_INVALID' "$1"; }
if diff <(filt_ids "$BASE/berry_ids.tsv") <(filt_ids "$OUT/berry_ids.tsv") > "$OUT/a_ids.diff"; then
    res PASS "(a) 아이템->열매 번호/경계 상수 동일 ($(grep -c '^ITEM' "$OUT/berry_ids.tsv") 종)"
else
    res FAIL "(a) 열매 번호 표 차이: $OUT/a_ids.diff"
fi
grep '^CHECK_FAIL' "$OUT/berry_ids.tsv" >/dev/null && res FAIL "(a) ItemIdToBerryType/BerryTypeToItemId 역함수 불일치 (CHECK_FAIL 줄)"
diff <(grep '^BERRY_INVALID' "$BASE/berry_ids.tsv") <(grep '^BERRY_INVALID' "$OUT/berry_ids.tsv") > "$OUT/a_invalid.diff" \
    && echo "     (참고) 무효 저장값(0, 69~127) 해석도 동일" \
    || echo "     (참고) 무효 저장값(0, 69~127) 해석 차이 $(grep -c '^>' "$OUT/a_invalid.diff")줄 — 예상된 차이(이전: 버치열매(CHERI)로 clamp, 이후: ITEM_NONE). README 참고"

if diff "$BASE/save_layout.txt" "$OUT/save_layout.txt" > "$OUT/b_layout.diff"; then
    res PASS "(b) 세이브 레이아웃 동일 ($(grep '^STRUCT SaveBlock1' "$OUT/save_layout.txt"))"
else
    res FAIL "(b) 세이브 레이아웃 차이: $OUT/b_layout.diff"
fi

if diff "$BASE/newgame_trees.tsv" "$OUT/newgame_trees.tsv" > "$OUT/c_newgame.diff"; then
    res PASS "(c) 새 게임 초기 나무 동일 ($(grep -vc '^#' "$OUT/newgame_trees.tsv")그루; $(cat "$OUT/newgame.log"))"
else
    res FAIL "(c) 새 게임 초기 나무 차이: $OUT/c_newgame.diff"
fi

diff <(tail -n +2 "$BASE/berry_rom_tables.tsv") <(tail -n +2 "$OUT/berry_rom_tables.tsv") > "$OUT/d_rom.diff"
unexpected=$(grep '^[<>]' "$OUT/d_rom.diff" | grep -v -P '^[<>] ITEM_ENIGMA_BERRY_E_READER\tnaturalGift\.(type|power)\t' | wc -l)
if [ "$unexpected" -eq 0 ]; then
    res PASS "(d) ROM 표(열매 데이터·자연의은혜·크래시·열매 그림·나무 그림/팔레트) 아이템 기준 동일 (허용 차이 $(grep -c '^>' "$OUT/d_rom.diff")줄: E-Reader 자연의은혜)"
else
    res FAIL "(d) ROM 표 차이 ${unexpected}줄: $OUT/d_rom.diff"
fi
exit $fail
