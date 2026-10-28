#!/bin/bash
# usage: run.sh <old-tree> <new-tree> <work-dir>
#   Compares data/battle_anim_scripts.s of two trees (e.g. a pre-port clone and the repo) per animation label after
#   inlining subroutine calls, using each tree's own macros and assembler pipeline. Used for animation refactors where
#   the ROM changes size (seq 497 #9924). Tools must be built in both trees (`make hns` once).
set -e
here="$(cd "$(dirname "$0")" && pwd)"
mkdir -p "$3"
python3 "$here/flatten.py" "$1/data/battle_anim_scripts.s" "$3/old.s"
python3 "$here/flatten.py" "$2/data/battle_anim_scripts.s" "$3/new.s"
bash "$here/dump.sh" "$1" "$3/old.s" "$3/old"
bash "$here/dump.sh" "$2" "$3/new.s" "$3/new"
python3 "$here/cmp.py" "$3/"
