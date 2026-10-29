#!/usr/bin/env python3
# usage: relocmp.py <old.elf> <old.gba> <new.elf> <new.gba>
#   For a port that should only reorder ROM contents (same SHA1 not expected, same symbols and sizes): checks that
#   every ROM symbol keeps its size and type, and that each byte difference inside a symbol is a Thumb BL or a
#   4-byte pointer whose old and new targets are the same symbol + offset. Prints moved symbols and the ones it
#   cannot explain (linker veneers `*_from_thumb`/`*_from_arm` and ARM-mode callers of them show up there; they only
#   move). Used for seq 505 #10184 (tileset data order).
import bisect, struct, subprocess, sys


def load(elf, gba):
    rom = open(gba, 'rb').read()
    out = subprocess.run(['arm-none-eabi-nm', '-S', '-n', elf], capture_output=True, text=True).stdout
    syms, starts = {}, []
    for line in out.split('\n'):
        p = line.split()
        if len(p) != 4:
            continue
        addr, size = int(p[0], 16), int(p[1], 16)
        if 0x08000000 <= addr < 0x0A000000:
            syms[p[3]] = (addr, size, p[2], rom[addr - 0x08000000:addr - 0x08000000 + size])
            starts.append((addr, size, p[3]))
    starts.sort()
    return syms, starts


def name_of(starts, addr):
    i = bisect.bisect_right(starts, (addr, 1 << 40, '')) - 1
    while i >= 0:
        a, size, n = starts[i]
        if a <= addr < a + max(size, 1):
            return (n, addr - a)
        if a < addr - 0x10000:
            break
        i -= 1
    return None


def bl_target(base, i, h1, h2):
    off = ((h1 & 0x7ff) << 12) | ((h2 & 0x7ff) << 1)
    if off & 0x400000:
        off -= 0x800000
    return base + i + 4 + off


A, As = load(sys.argv[1], sys.argv[2])
B, Bs = load(sys.argv[3], sys.argv[4])
missing = sorted(set(A) ^ set(B))
resized = [n for n in A if n in B and (A[n][1], A[n][2]) != (B[n][1], B[n][2])]
moved = [n for n in A if n in B and A[n][0] != B[n][0]]
bad = []
for n, (a, size, t, x) in A.items():
    if n not in B or B[n][3] == x or (size, t) != B[n][1:3]:
        continue
    b, y = B[n][0], B[n][3]
    covered, ok = set(), True
    if t in 'tT':
        for i in range(0, len(x) - 3, 2):
            h1, h2 = struct.unpack('<HH', x[i:i + 4])
            g1, g2 = struct.unpack('<HH', y[i:i + 4])
            if (h1 >> 11) == 0x1e and (h2 >> 11) == 0x1f and (g1 >> 11) == 0x1e and (g2 >> 11) == 0x1f and (h1, h2) != (g1, g2):
                if name_of(As, bl_target(a, i, h1, h2)) != name_of(Bs, bl_target(b, i, g1, g2)):
                    ok = False
                covered.update(range(i, i + 4))
    for i in range(0, len(x) - 1, 2):
        if not ok:
            break
        if x[i:i + 2] == y[i:i + 2] or i in covered:
            continue
        j = i - (i % 4)
        wo, wn = struct.unpack('<I', x[j:j + 4])[0], struct.unpack('<I', y[j:j + 4])[0]
        no, nn = name_of(As, wo & ~1), name_of(Bs, wn & ~1)
        if no is not None and no == nn:
            covered.update(range(j, j + 4))
            continue
        ok = False
    if not ok:
        bad.append(n)
print('symbols', len(A), 'missing/new', len(missing), 'resized', len(resized), 'moved', len(moved),
      'content differs', sum(1 for n in A if n in B and A[n][3] != B[n][3]), 'unexplained', len(bad))
for n in missing[:20]:
    print('  missing/new', n)
for n in resized[:20]:
    print('  resized', n)
for n in bad:
    print('  unexplained', n)
