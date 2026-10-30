#!/usr/bin/env bash
# usage: verify_seq.sh <seq> <pre-port commit> [work-dir]
#   Full verification of one full-sync seq after its code commit (run from the HnS repo root):
#   pre-port clone build (<work-dir>/w), full rebuild + warncheck, make check + testlist (baseline seq556),
#   save static compare against the pre-port clone, Korean regression (kortests, expected 555/652).
#   Prints the results; logs go to <work-dir>. Takes about 40 minutes on 4 cores.
set -u
seq="$1"; pre="$2"; W="${3:-${TMPDIR:-/tmp}/hns-verify}"
R="$(pwd)"; V=dev_scripts/hns_verify
BASE=docs/friend-handoff/results/1.17.0-port/test-baseline-f1030b.txt
export HNS_WORK="$W/hnswork"; mkdir -p "$W"
[ -d "$W/w/.git" ] || git clone -q "$R" "$W/w"
( cd "$W/w" && git fetch -q "$R" pokehns-expansion-kor && git checkout -q --detach "$pre" && git log --oneline -1 \
  && make hns -j8 > "$W/bw$seq.log" 2>&1; echo "pre build rc=$?"; sha1sum "$W/w/pokehns.gba" )
rm -rf build/hns
make hns -j8 > "$W/fb$seq.log" 2>&1; echo "fullbuild rc=$?"; sha1sum pokehns.gba
$V/warncheck.sh "$W/fb$seq.log"; echo "warncheck rc=$?"
GITHUB_ACTION=1 make check BUILD=hns -j8 > "$W/check$seq.log" 2>&1; echo "check rc=$? (2 is normal)"
$V/testlist.sh "$W/check$seq.log" "$BASE"; echo "testlist rc=$?"
rm -rf "$W/save-pre" "$W/save-run"
python3 $V/save/save_compat.py collect --src "$W/w" --elf "$W/w/pokehns.elf" --label "pre$seq" --out "$W/save-pre" > "$W/save$seq.log" 2>&1
python3 $V/save/save_compat.py run --pre "$W/save-pre" --out "$W/save-run" >> "$W/save$seq.log" 2>&1; echo "save rc=$?"
tail -3 "$W/save$seq.log"
ALLOW_REPO=1 $V/kortests/run.sh "$R" "kor-s$seq" 8 > "$W/kor$seq.log" 2>&1; echo "kortests rc=$?"
tail -4 "$W/kor$seq.log"
grep -a "Tests PASSED\|Tests TOTAL" "$W/kor$seq.log" | tail -2
python3 -c "import sys; d=open('pokehns.gba','rb').read(); print('ROM used', format(len(d.rstrip(b'\xff')), ','), 'B')"
git status --short
echo ALLDONE
