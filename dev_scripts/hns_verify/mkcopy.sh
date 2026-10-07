#!/usr/bin/env bash
# HNS9784 seq166 scratch tool (copied from chunk-150-155/tmp-150/tools) (never committed).
# Make a scratch copy of the HnS repo working tree for `make check BUILD=hns`, optionally apply patches.
#
# usage: mkcopy.sh <dest-dir> [--from <repo-or-copy>] [--patch <file.patch>]... [--trace]
#   --from   source tree (default: /home/hjm0725/pokehns-expansion-kor). Its tracked files must be clean.
#            A previous scratch copy (e.g. tmp-D/wb) also works and is faster (already built objects).
#   --patch  apply with `git apply -p1` inside the copy (repeatable, in the given order: A, B, C ...)
#   --trace  also apply tools/trace.patch (prints every recorded battle event; see trace_decode.py)
#
# The copy excludes .git, build/hns, build/emerald and old logs; it keeps tools/, generated graphics and
# build/hns-test so only changed objects rebuild. `.histignore` is created because there is no .git.
set -euo pipefail
here="$(cd "$(dirname "$0")" && pwd)"
dest="$1"; shift
from=/home/hjm0725/pokehns-expansion-kor
patches=()
trace=0
while [ $# -gt 0 ]; do
    case "$1" in
        --from) from="$2"; shift 2;;
        --patch) patches+=("$(realpath "$2")"); shift 2;;
        --trace) trace=1; shift;;
        *) echo "unknown arg $1" >&2; exit 2;;
    esac
done
if [ -d "$from/.git" ]; then
    if [ -n "$(git -C "$from" status --porcelain --untracked-files=no)" ]; then
        echo "source $from has uncommitted tracked changes; refusing" >&2; exit 1
    fi
    echo "source HEAD: $(git -C "$from" rev-parse HEAD)"
fi
mkdir -p "$dest"
rsync -a --delete --exclude=/.git --exclude=/build/hns --exclude=/build/emerald \
      --exclude=/build/localization-logs --exclude='/build/port-*.log' \
      --exclude='/test/battle/zz_*' "$from"/ "$dest"/
touch "$dest/.histignore"
for p in "${patches[@]}"; do
    echo "apply $p"
    (cd "$dest" && git apply -p1 --whitespace=nowarn "$p")
done
if [ "$trace" = 1 ]; then
    echo "apply trace.patch"
    (cd "$dest" && git apply -p1 "$here/trace.patch")
fi
echo "copy ready: $dest"
