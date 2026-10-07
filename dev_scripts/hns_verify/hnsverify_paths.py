"""HnS 검증 도구 공통 경로 규칙(파이썬판, common.sh와 같은 규칙). 빌드·테스트 입력이 아니다.

환경 변수: HNS_REPO, HNS_WORK, HNS_VERIFY_OUT, HNS_JOBS, DEVKITARM, ALLOW_REPO (README.md 참고).
"""
import glob
import os
import shutil
import subprocess
import sys

sys.dont_write_bytecode = True   # 저장소 안에 __pycache__를 남기지 않는다

VERIFY_DIR = os.path.dirname(os.path.abspath(__file__))


def _git_toplevel(path):
    try:
        r = subprocess.run(['git', '-C', path, 'rev-parse', '--show-toplevel'],
                           capture_output=True, text=True, check=False)
    except OSError:
        return None
    return r.stdout.strip() if r.returncode == 0 and r.stdout.strip() else None


def _repo_default():
    return _git_toplevel(VERIFY_DIR) or os.path.dirname(os.path.dirname(VERIFY_DIR))


REPO = os.path.realpath(os.environ.get('HNS_REPO') or _repo_default())
WORK = os.environ.get('HNS_WORK') or os.path.join(os.path.expanduser('~'), 'hns-sync-work')
OUT = os.path.realpath(os.environ.get('HNS_VERIFY_OUT') or os.path.join(WORK, 'verify-runs'))


def default_jobs():
    v = os.environ.get('HNS_JOBS')
    if v:
        return int(v)
    return os.cpu_count() or 4


def setup_toolchain_path():
    """DEVKITARM이 있으면 $DEVKITARM/bin을 PATH 앞에(Makefile 규칙). 없고 PATH에 arm-none-eabi-gcc도 없으면
    /opt/arm-gnu-toolchain-*/bin 하나를 찾아 붙인다. subprocess로 부르는 arm-none-eabi-* 모두에 적용된다."""
    path = os.environ.get('PATH', '')
    dk = os.environ.get('DEVKITARM')
    if dk and os.path.isdir(os.path.join(dk, 'bin')):
        os.environ['PATH'] = os.path.join(dk, 'bin') + os.pathsep + path
    elif not shutil.which('arm-none-eabi-gcc'):
        for d in sorted(glob.glob('/opt/arm-gnu-toolchain-*/bin')):
            if os.access(os.path.join(d, 'arm-none-eabi-gcc'), os.X_OK):
                os.environ['PATH'] = d + os.pathsep + path
                break


def is_checkout(tree):
    """<tree>/.git(디렉터리 또는 worktree 파일)이 있으면 실제 체크아웃이다. mkcopy.sh 사본에는 .git이 없다."""
    return os.path.exists(os.path.join(tree, '.git'))


def check_out_dir(path):
    """출력 폴더가 git 작업 트리 안(build/ 밖)이면 거부한다(git status를 더럽히지 않게)."""
    p = os.path.realpath(path)
    q = p
    while not os.path.isdir(q) and q != os.path.dirname(q):
        q = os.path.dirname(q)
    top = _git_toplevel(q)
    if top:
        top = os.path.realpath(top)
        if not (p + os.sep).startswith(os.path.join(top, 'build') + os.sep):
            sys.stderr.write('ERROR: 출력 폴더 %s 가 git 작업 트리 %s 안에 있다(밖이나 build/ 아래를 쓴다)\n' % (p, top))
            sys.exit(2)
    return p


setup_toolchain_path()
