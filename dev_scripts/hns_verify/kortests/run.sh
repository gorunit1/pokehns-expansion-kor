#!/usr/bin/env bash
# HnS 한글 배틀 출력 통합 회귀(607개). 테스트 .c는 sets/에 있고 test/ 밖이라 평소 빌드·make check·CI에 들어가지 않는다.
# 실행 때만 <tree>/test/battle/에 같은 파일 이름으로 복사해 `make check BUILD=hns TESTS=HNS`를 돌린다.
#
# usage: [ALLOW_REPO=1] kortests/run.sh <tree> <label> [jobs] [filter]
#   <tree>    mkcopy.sh 사본(.git 없음, trace.patch 적용 가능) 또는 ALLOW_REPO=1일 때 실제 저장소
#             (어느 쪽이든 복사 → 실행 → 복사한 .c와 그 오브젝트를 지운다. 중간에 끊겨도 trap으로 지운다)
#   <label>   출력 이름: $HNS_VERIFY_OUT/kortests/<label>.log, <label>-summary.txt, (trace 사본이면) <label>-trace.txt
#   [jobs]    make -j(기본 $HNS_JOBS 또는 8). 기대 요약의 "  - test/…" 줄(hydra 실패 목록 앞 50개)은 -j에 따라
#             순서·선택이 달라진다 → 기대 요약과 바이트 비교하려면 8. 상태 비교(compare.py)는 -j와 무관하다.
#   [filter]  TESTS= 접두어(기본 "HNS" = 전부, 예 "HNS9730" = 그 세트만. 전부일 때만 기대 요약과 비교한다)
#
# 세트(모든 테스트 이름이 "HNS"로 시작, 저장소 테스트에는 없음): HNS9680 HNS9714 HNSFIX1 HNSFIXOBS HNSREV HNS9717
#   HNS9168 HNSPOPUP HNSSW HNS9717P HNS9784 HNS9799(옛 328개, 22파일) + HNS9730(235) + HNSX1(21) + HNS9918(23)
#
# 끝에 kortests/compare.py로 expected/summary.txt(또는 $HNS_KOR_EXPECT)와 비교한다.
#   종료 코드: 0 = 기대와 같음, 1 = 다름, 2 = 도구·빌드 오류(요약 없음)
# 비교·trace 보조: tools/tracecmp.py, tools/tracediff.py, tools/showtrace.sh(사용법은 각 파일 머리, README)
set -uo pipefail
here="$(cd "$(dirname "$0")" && pwd)"
. "$here/../common.sh"
[ $# -ge 2 ] || { sed -n '2,18p' "$0"; exit 2; }
tree="$(realpath "$1")" || hv_die "no tree $1"
label="$2"
jobs="${3:-${HNS_JOBS:-8}}"
filter="${4:-HNS}"
[ -f "$tree/test/test_runner_battle.c" ] || hv_die "$tree is not an HnS tree"
out="$HNS_VERIFY_OUT/kortests"
mkdir -p "$out"
files=( "$here"/sets/*.c )
[ "${#files[@]}" -gt 0 ] && [ -f "${files[0]}" ] || hv_die "no test files in $here/sets"
objdir="$tree/build/hns-test/test/battle"

in_repo=0
if hv_is_checkout "$tree"; then
    [ "${ALLOW_REPO:-0}" = 1 ] || hv_die "$tree is a real checkout (.git); set ALLOW_REPO=1 to copy, run and delete the test files"
    if command grep -q -- '-TRACE' "$tree/test/test_runner_battle.c"; then
        hv_die "the real repo must not carry trace.patch (test/test_runner_battle.c has a -TRACE marker)"
    fi
    for f in "${files[@]}"; do
        d="$tree/test/battle/$(basename "$f")"
        if [ -e "$d" ] && ! cmp -s "$f" "$d"; then
            hv_die "$d already exists and differs from $f (not ours?); move it away first"
        fi
        if git -C "$tree" ls-files --error-unmatch "test/battle/$(basename "$f")" > /dev/null 2>&1; then
            hv_die "test/battle/$(basename "$f") is tracked in $tree; refusing to overwrite/delete it"
        fi
    done
    in_repo=1
fi

cleaned=0
cleanup() {   # 사본·실제 저장소 모두: 넣은 .c와 그 오브젝트를 지운다(다음 전체 make check 목록에 섞이지 않게)
    if [ "$cleaned" = 0 ]; then
        for f in "${files[@]}"; do
            b="$(basename "$f" .c)"
            rm -f "$tree/test/battle/$b.c" "$objdir/$b.o" "$objdir/$b.d"
        done
        echo "removed the copied test files (and their objects) from $tree$([ "$in_repo" = 1 ] && echo ' (git status should be clean again)')"
        cleaned=1
    fi
}
trap cleanup EXIT
trap 'exit 130' INT TERM

if [ "$in_repo" = 0 ]; then
    rm -f "$tree"/test/battle/zz_hns*.c
fi
stray="$(ls "$tree"/test/zz_*.c "$tree"/test/battle/zz_*.c 2>/dev/null || true)"
if [ -n "$stray" ]; then
    echo "WARNING: other scratch tests are in $tree/test (tests named HNS… among them are counted too):"; printf '  %s\n' $stray
fi
for f in "${files[@]}"; do cp "$f" "$tree/test/battle/"; done
log="$out/$label.log"
echo "tree $tree ($([ "$in_repo" = 1 ] && echo "real checkout, HEAD $(git -C "$tree" rev-parse --short HEAD)" || echo copy)), jobs $jobs, filter '$filter'"
(cd "$tree" && make check BUILD=hns -j"$jobs" TESTS="$filter") > "$log" 2>&1
rc=$?
cleanup
echo "make check exit $rc (2 is normal: some tests are expected to fail)  log: $log"
summary="$out/$label-summary.txt"
LC_ALL=C sed 's/\x1b\[[0-9;]*m//g' "$log" | LC_ALL=C command grep -a -E '^\[[0-9]+\] HNS|^- Tests|^  - test/' \
    | LC_ALL=C sed -E 's/^\[[0-9]+\] //' | LC_ALL=C sort > "$summary"
if ! command grep -a -q -E '^- Tests TOTAL' "$summary"; then
    echo "no test summary in the log (build error?) — see $log" >&2
    tail -n 20 "$log" >&2
    exit 2
fi
echo "summary: $summary"
command grep -a -E '^- Tests' "$summary" || true
# 세트별 PASS 수(이름 첫 단어 = 세트)
LC_ALL=C command grep -a -E '^HNS.*: [A-Z_]+$' "$summary" | LC_ALL=C awk '
    { set = $1; total[set]++; if ($NF == "PASS") pass[set]++; if (!(set in seen)) { seen[set] = 1; order[++n] = set } }
    END { for (i = 1; i <= n; i++) printf "%-9s PASS %3d / total %3d\n", order[i], pass[order[i]], total[order[i]] }'
if command grep -q -- '-TRACE' "$tree/test/test_runner_battle.c"; then
    python3 -B "$here/tools/trace_decode.py" "$tree" "$log" > "$out/$label-trace.txt"
    echo "trace: $out/$label-trace.txt ($(command grep -c '^## ' "$out/$label-trace.txt") test blocks)"
fi
expect="${HNS_KOR_EXPECT:-$here/expected/summary.txt}"
if [ "$filter" = HNS ] && [ -f "$expect" ]; then
    echo "== compare with $expect"
    python3 -B "$here/compare.py" "$summary" "$expect"
    exit $?
fi
echo "(filter '$filter': not compared with the expected summary; non-PASS list follows)"
command grep -a -E '^HNS.*: [A-Z_]+$' "$summary" | command grep -a -v ': PASS$' || true
exit 0
