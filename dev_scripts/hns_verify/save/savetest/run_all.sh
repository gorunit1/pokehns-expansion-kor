#!/usr/bin/env bash
# usage: [ALLOW_REPO=1] save/savetest/run_all.sh <tree> <label> [jobs]
#   이식 전 ROM이 만든 세이브 2개(baseline/pre-make-flash.bin: 고정 상태, baseline/pre-newgame-flash.bin: NewGameInitData
#   + 파티·박스)를 <tree> 빌드의 실제 LoadGameSave로 읽고, 이식 전 기준(baseline/pre-load-make/, pre-load-newgame/)과 비교한다.
#   <tree>: mkcopy.sh 사본(기본 용도) 또는 ALLOW_REPO=1일 때 실제 저장소(테스트 파일을 넣고 실행한 뒤 지운다).
#   [jobs]: make -j(기본 $HNS_JOBS 또는 nproc).
#   결과: $HNS_VERIFY_OUT/savetest/<label>-make/, <label>-newgame/ (+ .stdout)
#   기대: 두 이미지 모두 "[PASS] LOAD 테스트가 읽은 이미지 = …"와 "판정: PASS", make 이미지에서 "녹화 기록 유효: 1 → 0".
#   종료 코드 0 = 둘 다 PASS.
set -uo pipefail
here="$(cd "$(dirname "$0")" && pwd)"
. "$here/../../common.sh"
[ $# -ge 2 ] || { sed -n '2,9p' "$0"; exit 2; }
tree="$1"; label="$2"; jobs="${3:-${HNS_JOBS:-$(nproc)}}"
rc=0
runs="$HNS_VERIFY_OUT/savetest"; mkdir -p "$runs"
for img in make newgame; do
    python3 -B "$here/savetest.py" run "$tree" "$label-$img" --image "$here/baseline/pre-$img-flash.bin" \
        --image-name "pre-$img.sav" --jobs "$jobs" > "$runs/$label-$img.stdout" 2>&1 || rc=1
    command grep -a -E "HNS8943|Tests|^LOAD image=|LOAD 테스트|^removed|^ERROR|^Traceback" "$runs/$label-$img.stdout" | sed 's/^/  /'
    if [ ! -f "$runs/$label-$img/summary.txt" ]; then
        echo "== $label-$img: run failed before the test ran:"; tail -n 3 "$runs/$label-$img.stdout" | sed 's/^/  /'
        rc=1; continue
    fi
    echo "== $label-$img vs 이식 전 기준 pre-$img"
    python3 -B "$here/savetest.py" compare "$here/baseline/pre-load-$img" "$label-$img" || rc=1
done
echo "run_all: $([ $rc = 0 ] && echo PASS || echo FAIL)  (logs: $runs/$label-*.stdout)"
exit $rc
