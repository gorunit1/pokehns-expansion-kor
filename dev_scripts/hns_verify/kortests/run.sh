#!/usr/bin/env bash
# hnsfix-1007b kortests-final (APPLY2.md): copy of chunk-181/E-kortests (unchanged there) for the 2nd HnS commits
#   + tmp-X2/kortests-expect-x2.diff applied (decision 6: 1-stage stat messages have one space; 16 files)
#   + tmp-X1/kortests/zz_hnsx1_kor.c (HNSX1 01-21, X1 + X1-3b) converted with the same rule
#     (MESSAGE "[이가]  (올라갔다|떨어졌다)!" -> one space, 17 lines)
#   + run.sh from tmp-X1/kortests (zz_hnsx1_*.c and the HNSX1 count).
#   Expected statuses after X1-7/X1-3/X1-3b/X1-5/X2/X3/X4: runs/expected-final-summary.txt (see apply/kortests-final.md).
#   Original header follows.
# seq 181 #9730 area E scratch tool (never committed). Korean battle-output regression set:
#   set328/  the 328-test integrated set copied unchanged from chunk-171-174/tmp-171/kortests (22 files)
#   zz_hns9730_k*.c  new stat-change Korean output tests (prefix "HNS9730"), all PASS on chunk-181/base
#
# usage: [ALLOW_REPO=1] run.sh <tree> <label> [jobs] [filter]
#   <tree>   scratch copy made by chunk-132/D-tests/mkcopy.sh (… --from chunk-181/base [--patch part-A.patch …]),
#            optionally with tools/trace.patch applied (git apply -p1 tools/trace.patch inside the copy) to get
#            runs/<label>-trace.txt; or the real repo with ALLOW_REPO=1 (files are copied in, run, deleted again).
#   <label>  outputs: runs/<label>.log, runs/<label>-summary.txt, runs/<label>-trace.txt (trace only if patched)
#   [jobs]   make -j (default 8; do not go higher, other areas build at the same time)
#   [filter] TESTS= prefix (default "HNS" = everything; "HNS9730" = only the new stat-change tests)
#
# Sets (all test names start with "HNS"; no repo test does):
#   HNS9680 HNS9714 HNSFIX1 HNSFIXOBS HNSREV HNS9717 HNS9168 HNSPOPUP HNSSW HNS9717P HNS9784 HNS9799  (set328, 328 tests)
#   HNS9730  zz_hns9730_k1_moves.c(80) k2_abilities.c(69) k3_items.c(25) k4_interact.c(57) k5_totem.c(4) = 235
#            stat-change messages / pop-ups / animations (this unit), generated from the base trace by
#            tools/gen_scene.py (probe sources: chunk-181/tmp-E/probe/)
#
# Results (see chunk-181/part-E.md 12, expected-changes.tsv):
#   runs/base-*          chunk-181/base copy + tools/trace.patch      513/563 (set328 278/328 = post1007b, HNS9730 235/235)
#   runs/base-notrace-*  chunk-181/base copy without trace.patch      same list
#   runs/post-v2c-*      base + part-A..F (12:22-12:44) + final part-E 453/563 (HNS9730 180/235)
#   runs/post-v2c-plusR-* post-v2c + F-201/F-266/F-395 + upstream #10074  452/563 (HNS9730 182/235)
#   runs/post-ABCDEF-v1-* early merge (220 tests, part-E with upstream {B_EFF} in BECAMENIMBLE: K1-50 wrong name)
#
# Compare pre and post:
#   diff runs/<pre>-summary.txt runs/<post>-summary.txt
#   python3 tools/tracecmp.py runs/<pre>-trace.txt runs/<post>-trace.txt
#   python3 tools/tracediff.py runs/<pre>-trace.txt runs/<post>-trace.txt 'HNS9730 K4-06'   # unified diff of one test
#   tools/showtrace.sh runs/<label>-trace.txt "K1-03"     # one test's decoded events
set -uo pipefail
here="$(cd "$(dirname "$0")" && pwd)"
tree="$(realpath "$1")"
label="$2"
jobs="${3:-8}"
filter="${4:-HNS}"
files=( "$here"/set328/*.c "$here"/zz_hns9730_k*.c "$here"/zz_hnsx1_*.c )  # hnsfix-1007b X1: + zz_hnsx1_kor.c
in_repo=0
case "$tree" in
    /home/hjm0725/pokehns-expansion-kor|/home/hjm0725/pokehns-expansion-kor/*)
        if [ "${ALLOW_REPO:-0}" != 1 ]; then
            echo "refusing to run inside the real repo (set ALLOW_REPO=1 to copy, run and delete the test files)" >&2; exit 2
        fi
        if grep -q -- '-TRACE' "$tree/test/test_runner_battle.c"; then
            echo "the real repo must not carry trace.patch" >&2; exit 2
        fi
        in_repo=1;;
esac
mkdir -p "$here/runs"
rm -f "$tree"/test/battle/zz_hns*.c
for f in "${files[@]}"; do cp "$f" "$tree/test/battle/"; done
log="$here/runs/$label.log"
(cd "$tree" && PATH=/opt/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin:$PATH make check BUILD=hns -j"$jobs" TESTS="$filter") > "$log" 2>&1
rc=$?
if [ "$in_repo" = 1 ]; then
    for f in "${files[@]}"; do rm -f "$tree/test/battle/$(basename "$f")"; done
    echo "removed the scratch test files from $tree/test/battle (git status should be clean again)"
fi
echo "make check exit $rc"
LC_ALL=C sed 's/\x1b\[[0-9;]*m//g' "$log" | LC_ALL=C command grep -a -E '^\[[0-9]+\] HNS|^- Tests|^  - test/' \
    | LC_ALL=C sed -E 's/^\[[0-9]+\] //' | LC_ALL=C sort > "$here/runs/$label-summary.txt"
command grep -a -E '^- Tests' "$here/runs/$label-summary.txt" || true
for p in HNS9680 HNS9714 HNSFIX1 HNSFIXOBS HNSREV HNS9717 HNS9168 HNSPOPUP HNSSW HNS9717P HNS9784 HNS9799 HNS9730 HNSX1; do
    printf '%-9s PASS %3d / total %3d\n' "$p" \
        "$(command grep -a -c "^$p .*: PASS$" "$here/runs/$label-summary.txt")" \
        "$(command grep -a -c -E "^$p .*: [A-Z_]+$" "$here/runs/$label-summary.txt")"
done
command grep -a -E '^HNS.*: [A-Z_]+$' "$here/runs/$label-summary.txt" | command grep -a -v ': PASS$' || true
if command grep -q -- '-TRACE' "$tree/test/test_runner_battle.c"; then
    python3 "$here/tools/trace_decode.py" "$tree" "$log" > "$here/runs/$label-trace.txt"
    echo "trace: $here/runs/$label-trace.txt ($(command grep -c '^## ' "$here/runs/$label-trace.txt") test blocks)"
fi
