#!/usr/bin/env bash
# 저장소 작업 트리의 스크래치 사본을 만든다(`make check BUILD=hns`용). 원본 저장소는 읽기만 한다.
# (옛 chunk-166-170/tmp-166/tools/mkcopy.sh + chunk-1385/verify/savetest/mkcopy_worktree.sh)
#
# usage: mkcopy.sh <dest-dir> [--from <tree>] [--allow-dirty] [--patch <file.patch>]... [--trace]
#   --from         원본 트리(기본: $HNS_REPO = 이 도구가 든 저장소). 이전 사본도 된다(이미 빌드한 오브젝트가 있어 빠르다).
#   --allow-dirty  원본의 추적 파일 변경(커밋 전 patch 적용 상태 등)을 그대로 복사한다. 없으면 변경이 있을 때 거부.
#   --patch        사본 안에서 `git apply -p1`(여러 번, 준 순서대로)
#   --trace        kortests/tools/trace.patch도 적용(기록되는 모든 배틀 이벤트를 출력; kortests/run.sh가 해독)
#
# 사본에서는 .git, build/hns, build/emerald, 이식 로그, test/battle/zz_*, test/zz_*를 뺀다(사본에 남은 zz_*도 지움). tools/, 생성 그래픽,
# build/hns-test는 가져온다(바뀐 오브젝트만 다시 빌드). .git이 없으므로 kortests/run.sh·savetest.py가 사본으로 판정한다.
# 사본에 .histignore를 만든다(.git 없는 트리에서 check_history.sh가 빌드를 멈추지 않게).
# <dest-dir>는 어떤 git 작업 트리 안에도 있으면 안 된다(실제 저장소에 trace.patch가 들어가는 것을 막는다).
set -euo pipefail
. "$(cd "$(dirname "$0")" && pwd)/common.sh"
[ $# -ge 1 ] || { sed -n '2,14p' "$0"; exit 2; }
dest="$(realpath -m "$1")"; shift
from="$HNS_REPO"
patches=()
trace=0
dirty=0
while [ $# -gt 0 ]; do
    case "$1" in
        --from) from="$(realpath "$2")"; shift 2;;
        --allow-dirty) dirty=1; shift;;
        --patch) patches+=("$(realpath "$2")"); shift 2;;
        --trace) trace=1; shift;;
        *) hv_die "unknown arg $1";;
    esac
done
case "$dest/" in
    "$from"/*|"$HNS_REPO"/*) hv_die "dest $dest must be outside the source tree and the repo";;
esac
p="$dest"; while [ ! -d "$p" ] && [ "$p" != / ]; do p="$(dirname "$p")"; done
if top="$(git -C "$p" rev-parse --show-toplevel 2>/dev/null)"; then
    hv_die "dest $dest is inside the git work tree $top"
fi
[ -e "$dest/.git" ] && hv_die "dest $dest has .git (a real checkout?)"
if hv_is_checkout "$from"; then
    n="$(git -C "$from" status --porcelain --untracked-files=no | wc -l)"
    echo "source $from HEAD: $(git -C "$from" rev-parse HEAD), tracked changes: $n"
    if [ "$n" != 0 ] && [ "$dirty" != 1 ]; then
        hv_die "source $from has uncommitted tracked changes (use --allow-dirty to copy them)"
    fi
fi
# test/battle/zz_*, test/zz_*(한글 회귀·세이브 왕복 테스트를 넣는 자리)는 원본에서 가져오지 않고, 다시 동기화할 때 사본에
# 남아 있던 것도 지운다(-s = 보내는 쪽에만 적용하는 제외 규칙이라 --delete가 받는 쪽 파일을 지운다).
excl=( --exclude=/.git --exclude=/build/hns --exclude=/build/emerald
       --exclude=/build/localization-logs --exclude='/build/port-*.log'
       --filter='-s /test/battle/zz_*' --filter='-s /test/zz_*' )
case "$HNS_VERIFY_OUT/" in
    "$from"/*) excl+=( --exclude="/${HNS_VERIFY_OUT#"$from"/}" );;   # 출력 폴더가 원본 build/ 아래면 빼기
esac
mkdir -p "$dest"
rsync -a --delete "${excl[@]}" "$from"/ "$dest"/
touch "$dest/.histignore"
for f in "${patches[@]}"; do
    echo "apply $f"
    (cd "$dest" && git apply -p1 --whitespace=nowarn "$f")
done
if [ "$trace" = 1 ]; then
    echo "apply trace.patch"
    (cd "$dest" && git apply -p1 "$HNS_VERIFY_DIR/kortests/tools/trace.patch")
fi
echo "copy ready: $dest"
