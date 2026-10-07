# HnS 검증 도구 공통 설정(bash에서 source). 빌드·테스트 입력이 아니다.
#
# 환경 변수(모두 선택, 없으면 기본값):
#   HNS_REPO        대상 저장소. 기본: 이 폴더가 든 git 체크아웃 최상위(.git이 없으면 이 폴더의 ../..)
#   HNS_WORK        스크래치 작업 폴더. 기본: $HOME/hns-sync-work
#   HNS_VERIFY_OUT  도구 출력 폴더. 기본: $HNS_WORK/verify-runs (저장소 안이면 build/ 아래만 허용)
#   HNS_JOBS        make -j 기본값(각 도구 인자로도 줄 수 있다)
#   DEVKITARM       있으면 $DEVKITARM/bin을 PATH 앞에 붙인다(Makefile과 같은 규칙).
#                   없고 PATH에 arm-none-eabi-gcc도 없으면 /opt/arm-gnu-toolchain-*/bin 하나를 찾아 붙인다.
#   ALLOW_REPO=1    실제 체크아웃(.git이 있는 트리)에서 테스트 파일을 복사·실행·삭제하도록 허용

HNS_VERIFY_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if [ -z "${HNS_REPO:-}" ]; then
    HNS_REPO="$(git -C "$HNS_VERIFY_DIR" rev-parse --show-toplevel 2>/dev/null || true)"
    [ -n "$HNS_REPO" ] || HNS_REPO="$(cd "$HNS_VERIFY_DIR/../.." && pwd)"
fi
HNS_REPO="$(realpath -m "$HNS_REPO")"
HNS_WORK="${HNS_WORK:-$HOME/hns-sync-work}"
HNS_VERIFY_OUT="$(realpath -m "${HNS_VERIFY_OUT:-$HNS_WORK/verify-runs}")"
export HNS_REPO HNS_WORK HNS_VERIFY_OUT

if [ -n "${DEVKITARM:-}" ] && [ -d "$DEVKITARM/bin" ]; then
    PATH="$DEVKITARM/bin:$PATH"
elif ! command -v arm-none-eabi-gcc > /dev/null 2>&1; then
    for _d in /opt/arm-gnu-toolchain-*/bin; do
        if [ -x "$_d/arm-none-eabi-gcc" ]; then PATH="$_d:$PATH"; break; fi
    done
    unset _d
fi
export PATH

hv_die() { echo "ERROR: $*" >&2; exit 2; }

# 실제 체크아웃 판정: <tree>/.git(디렉터리 또는 worktree 파일)이 있으면 실제 저장소다.
# mkcopy.sh 사본에는 .git이 없다.
hv_is_checkout() { [ -e "$1/.git" ]; }

# 출력 폴더가 저장소 작업 트리 안(build/ 밖)이면 git status를 더럽히므로 거부한다.
hv_check_out_dir() {
    local out top
    out="$(realpath -m "$1")"
    top="$(git -C "$(dirname "$out")" rev-parse --show-toplevel 2>/dev/null || true)"
    if [ -z "$top" ]; then
        # 아직 없는 폴더라면 가장 가까운 기존 상위 폴더로 판정
        local p="$out"
        while [ ! -d "$p" ] && [ "$p" != / ]; do p="$(dirname "$p")"; done
        top="$(git -C "$p" rev-parse --show-toplevel 2>/dev/null || true)"
    fi
    if [ -n "$top" ]; then
        case "$out/" in
            "$top"/build/*) ;;
            *) hv_die "output folder $out is inside the git work tree $top (use a folder outside it, or under build/)";;
        esac
    fi
}

hv_check_out_dir "$HNS_VERIFY_OUT"
