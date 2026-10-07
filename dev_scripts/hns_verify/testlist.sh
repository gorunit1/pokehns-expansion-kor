#!/usr/bin/env bash
# usage: testlist.sh <check-log> [baseline-list] [--out <list.txt>] [--tree <tree>]
#   전체 테스트 로그(GITHUB_ACTION=1 make check BUILD=hns -j8 > log 2>&1)를 비교용 목록("이름: 상태", 정렬·고유)으로 바꾼다.
#   docs/friend-handoff/results/1.17.0-port/PORT_INSTRUCTIONS.md "테스트" 절의 LC_ALL=C + grep -a 명령과 같다
#   (한글 바이트가 붙는 "~ fit on ~" 줄을 놓치지 않게).
#   [baseline-list]  기준 목록(예: docs/friend-handoff/results/1.17.0-port/test-baseline-<직전>.txt). 주면 기준에서 PASS였는데
#                    지금 PASS가 아닌 것(사라진 PASS = 회귀 후보)과 새 PASS 수를 보인다.
#   --out            목록 저장 위치(기본: $HNS_VERIFY_OUT/testlist/<로그 이름>.txt)
#   --tree           남은 한글 회귀·세이브 테스트 파일(test/battle/zz_*, test/zz_*)과 trace 표식을 확인할 트리(기본 $HNS_REPO)
#   종료 코드 0 = 사라진 PASS 없음(또는 기준 없음), 1 = 사라진 PASS 있음.
set -uo pipefail
. "$(cd "$(dirname "$0")" && pwd)/common.sh"
log=""; base=""; out=""; tree="$HNS_REPO"
while [ $# -gt 0 ]; do
    case "$1" in
        --out) out="$2"; shift 2;;
        --tree) tree="$2"; shift 2;;
        -h|--help) sed -n '2,10p' "$0"; exit 0;;
        *) if [ -z "$log" ]; then log="$1"; elif [ -z "$base" ]; then base="$1"; else hv_die "extra arg $1"; fi; shift;;
    esac
done
[ -n "$log" ] || { sed -n '2,10p' "$0"; exit 2; }
[ -f "$log" ] || hv_die "no log $log"
if [ -z "$out" ]; then
    out="$HNS_VERIFY_OUT/testlist/$(basename "$log" .log).txt"
else
    hv_check_out_dir "$out"
fi
mkdir -p "$(dirname "$out")"

LC_ALL=C sed 's/\x1b\[[0-9;]*m//g' "$log" \
    | LC_ALL=C command grep -a -E '^\[[0-9]+\] .*: (PASS|FAIL|KNOWN_FAILING|TO_DO|EXPECTED_FAIL)$' \
    | LC_ALL=C sed -E 's/^\[[0-9]+\] //' | LC_ALL=C sort -u > "$out"
echo "list: $out ($(wc -l < "$out") lines)"
LC_ALL=C sed 's/\x1b\[[0-9;]*m//g' "$log" | LC_ALL=C command grep -a -E '^- Tests' || echo "(no '- Tests' totals in the log)"
LC_ALL=C awk -F': ' '{ c[$NF]++ } END { for (k in c) printf "  %s %d\n", k, c[k] }' "$out" | LC_ALL=C sort
echo "  INVALID $(LC_ALL=C sed 's/\x1b\[[0-9;]*m//g' "$log" | LC_ALL=C command grep -a -c -E '^\[[0-9]+\] .*: INVALID')"

if [ -d "$tree/test" ]; then
    left="$(ls -d "$tree"/test/battle/zz_* "$tree"/test/zz_* 2>/dev/null || true)"
    if [ -n "$left" ] && hv_is_checkout "$tree"; then
        echo "WARNING: scratch test files are in the real checkout $tree (they are in this log's test list; do not git add them):"
        printf '  %s\n' $left
    fi
    if command grep -q -- '-TRACE' "$tree/test/test_runner_battle.c" 2>/dev/null && hv_is_checkout "$tree"; then
        echo "WARNING: $tree/test/test_runner_battle.c carries trace.patch (-TRACE marker); do not commit it"
    fi
fi

rc=0
if [ -n "$base" ]; then
    [ -f "$base" ] || hv_die "no baseline $base"
    lost="$(LC_ALL=C diff <(LC_ALL=C command grep -a ': PASS$' "$base") <(LC_ALL=C command grep -a ': PASS$' "$out") \
            | LC_ALL=C command grep -a '^<' || true)"
    newp="$(LC_ALL=C diff <(LC_ALL=C command grep -a ': PASS$' "$base") <(LC_ALL=C command grep -a ': PASS$' "$out") \
            | LC_ALL=C command grep -a -c '^>' || true)"
    if [ "$(cmp -s "$base" "$out" && echo same)" = same ]; then
        echo "list is byte-identical to $base"
    fi
    echo "vs $base: new PASS $newp, lost PASS $( [ -n "$lost" ] && printf '%s\n' "$lost" | wc -l || echo 0)"
    if [ -n "$lost" ]; then
        printf '%s\n' "$lost"
        echo "(a renamed test shows up as one lost + one new PASS; check names before calling it a regression)"
        rc=1
    fi
fi
exit $rc
