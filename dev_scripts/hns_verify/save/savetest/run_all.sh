#!/usr/bin/env bash
# run_all.sh <scratch-tree> <label> [jobs]
#   이식 전 ROM이 만든 세이브 2개(baseline/pre-make.sav: 고정 상태, baseline/pre-newgame.sav: NewGameInitData + 파티·박스)를
#   <scratch-tree> 빌드의 실제 LoadGameSave로 읽고, 이식 전 기준(baseline/pre-load-make/, pre-load-newgame/)과 비교한다.
#   <scratch-tree>는 mkcopy_worktree.sh(또는 chunk-132 mkcopy.sh)로 만든 사본(저장소 자체는 거부). 결과: chunk-1385/tmp-D/savetest-runs/<label>-make, <label>-newgame
set -uo pipefail
here="$(cd "$(dirname "$0")" && pwd)"
tree="$1"; label="$2"; jobs="${3:-16}"
rc=0
runs="$(cd "$here/../.." && pwd)/tmp-D/savetest-runs"; mkdir -p "$runs"
for img in make newgame; do
    python3 "$here/savetest.py" run "$tree" "$label-$img" --image "$here/baseline/pre-$img.sav" --image-name "pre-$img.sav" --jobs "$jobs" > "$runs/$label-$img.stdout" 2>&1 || rc=1
    grep -E "HNS8943|Tests" "$runs/$label-$img.stdout" | sed 's/^/  /'
    echo "== $label-$img vs 이식 전 기준 pre-$img"
    python3 "$here/savetest.py" compare "$here/baseline/pre-load-$img" "$label-$img" || rc=1
done
exit $rc
