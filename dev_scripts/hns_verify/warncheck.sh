#!/usr/bin/env bash
# usage: warncheck.sh <build-log> [warn-base]
#   빌드 로그(make hns -j8 > log 2>&1)의 컴파일 경고 중 기준(expected/warn-base.txt, "파일: warning: 메시지" 고유 목록)에
#   없는 것만 보인다. 줄·열 번호는 지우고 비교한다(이식으로 줄이 밀려도 같은 경고로 본다).
#   허용 도구 경고 `libpng warning: bKGD: invalid index`(그래픽 도구)는 목록 밖에서 따로 허용한다.
#   증분 빌드 로그에는 다시 컴파일한 파일의 경고만 나온다 — 묶음 검증은 전체 빌드 로그로 한다.
#   종료 코드 0 = 새 경고 없음, 1 = 새 경고 있음.
set -uo pipefail
. "$(cd "$(dirname "$0")" && pwd)/common.sh"
[ $# -ge 1 ] || { sed -n '2,7p' "$0"; exit 2; }
log="$1"
base="${2:-$HNS_VERIFY_DIR/expected/warn-base.txt}"
[ -f "$log" ] || hv_die "no log $log"
total="$(LC_ALL=C command grep -a -c 'warning:' "$log")"
new="$(LC_ALL=C command grep -a 'warning:' "$log" | LC_ALL=C command grep -a -v 'libpng warning: bKGD: invalid index' \
       | LC_ALL=C sed -E 's/:[0-9]+:[0-9]+: /: /' | LC_ALL=C sort -u | LC_ALL=C comm -13 "$base" -)"
echo "warning lines in log: $total, base: $(wc -l < "$base") unique"
if [ -n "$new" ]; then
    echo "NEW warnings ($(printf '%s\n' "$new" | wc -l)):"
    printf '%s\n' "$new"
    exit 1
fi
echo "new warnings: 0"
