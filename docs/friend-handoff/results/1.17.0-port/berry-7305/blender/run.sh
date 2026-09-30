#!/usr/bin/env bash
# run.sh <repo> <out.tsv>
#   Berry Blender NPC berry choice table: for every berry the player puts in, which berries the
#   NPC opponents use (1-3 opponents, Blend Master flag hidden or not; E-Reader berry per lowest flavor).
#   Cuts NUM_NPC_BERRIES, struct BlenderBerry, sOpponentBerrySets, sBerryMasterBerries and
#   SetOpponentsBerryData() out of <repo>/src/berry_blender.c verbatim and compiles them with host gcc
#   against <repo>/include. The repo is only read.
#   Expected: identical to blender_pre.tsv (pre-#7305 behaviour, kept by the HnS fix in SetOpponentsBerryData).
#   HnS build: FLAG_HIDE_LILYCOVE_CONTEST_HALL_BLEND_MASTER is 0, so FlagGet() is FALSE -> "flagHidden=0" rows apply.
set -euo pipefail
HERE=$(cd "$(dirname "$0")" && pwd)
REPO=$(cd "$1" && pwd)
OUT=$(realpath -m "$2")
WORK=$(mktemp -d "${TMPDIR:-/tmp}/blender7305.XXXXXX")
trap 'rm -rf "$WORK"' EXIT
python3 "$HERE/extract.py" "$REPO/src/berry_blender.c" > "$WORK/extracted.inc"
python3 - "$REPO/include/constants/items.h" > "$WORK/names.inc" <<'PY'
import re, sys
src = open(sys.argv[1], encoding='utf-8').read()
start = src.index('FIRST_BERRY_INDEX =')
end = src.index('LAST_BERRY_INDEX', start)
for n in re.findall(r'\b(ITEM_[A-Z0-9_]+)\s*=', src[start:end]):
    print('    { %s, "%s" },' % (n, n))
PY
gcc -std=gnu17 -w -include string.h -iquote "$REPO/include" -I "$WORK" -DMODERN=1 -DTESTING=0 \
    -o "$WORK/harness" "$HERE/harness.c"
"$WORK/harness" > "$OUT"
echo "wrote $OUT ($(wc -l < "$OUT") rows)" >&2
