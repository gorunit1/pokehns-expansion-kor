#!/usr/bin/env python3
"""berry_rom_tables.sh 에서 호출. 인자: types.o elf berry_ids.tsv  -> stdout TSV"""
import re
import sys

sys.path.insert(0, __file__.rsplit('/', 1)[0])
from dwarf_layout import Dwarf  # noqa: E402
from elfsym import Elf  # noqa: E402

types_o, elf_path, ids_path = sys.argv[1:4]
dw = Dwarf(types_o)
elf = Elf(elf_path)

items = []  # (item_id, name, berry_no)
for line in open(ids_path):
    p = line.rstrip('\n').split('\t')
    if p[0] == 'ITEM':
        items.append((int(p[1]), p[2], int(p[3])))

berry_size, berry_members = dw.members('Berry')
post = any(m[0].startswith('info.') for m in berry_members)
print('#mode\t%s\tsizeof(struct Berry)=%d' % ('post' if post else 'pre', berry_size))


def norm(name):
    # sPicTable_X / gPicTable_X 처럼 정적->전역 이름 변경만 흡수
    return re.sub(r'(^|\|)[sg](?=[A-Z])', r'\1', name)


def field_values(blob, members, strip_prefix=''):
    v = int.from_bytes(blob, 'little')
    out = []
    for path, off, bit, bits, kind in members:
        if path.startswith('[') or path.endswith(']') and kind.startswith('array'):
            out.append((path[len(strip_prefix):], blob[off:off + bits // 8].hex()))
            continue
        if '[]' in path:
            continue
        val = (v >> bit) & ((1 << bits) - 1)
        out.append((path[len(strip_prefix):], val, kind))
    return out


def read_str(ptr, limit=400):
    if ptr == 0:
        return 'NULL'
    b = elf.read_addr(ptr, 1)
    s = bytearray()
    a = ptr
    while len(s) < limit:
        c = elf.read_addr(a, 1)[0]
        s.append(c)
        a += 1
        if c == 0xFF:  # EOS
            break
    return s.hex()


def sym_size_at(ptr):
    for sy in elf._addr.get(ptr & ~1, []):
        if sy['size']:
            return sy['size']
    return 0


def frames(ptr):
    """SpriteFrameImage 배열(심볼 크기만큼)을 (data 심볼, size) 목록으로"""
    fsize, fmem = dw.members('SpriteFrameImage')
    n = sym_size_at(ptr) // fsize if fsize else 0
    res = []
    for i in range(n):
        blob = elf.read_addr(ptr + i * fsize, fsize)
        v = int.from_bytes(blob, 'little')
        parts = []
        for path, off, bit, bits, kind in fmem:
            val = (v >> bit) & ((1 << bits) - 1)
            parts.append(norm(elf.names_at(val)) if kind == 'ptr' else '%s=%d' % (path, val))
        res.append('/'.join(parts))
    return ';'.join(res) if res else 'UNSIZED:' + norm(elf.names_at(ptr))


def emit(item, field, value):
    print('%s\t%s\t%s' % (item, field, value))


def table_entry(sym, index, esize):
    s = elf.sym(sym)
    if (index + 1) * esize > s['size'] or index < 0:
        return None
    return elf.read_sym(sym, esize, index * esize)


def dump_info(item, blob, members, prefix):
    for path, off, bit, bits, kind in members:
        if not path.startswith(prefix) or '[]' in path:
            continue
        name = path[len(prefix):]
        if kind.startswith('array'):
            emit(item, name, blob[off:off + bits // 8].hex())
            continue
        val = (int.from_bytes(blob, 'little') >> bit) & ((1 << bits) - 1)
        if kind == 'ptr':
            if name.startswith('description'):
                emit(item, name, read_str(val))
            else:
                emit(item, name, norm(elf.names_at(val)))
        else:
            emit(item, name, val)


def tree_tables(item, pic_ptr, pal_ptr):
    emit(item, 'treePicTable', norm(elf.names_at(pic_ptr)))
    emit(item, 'treePicTable.frames', frames(pic_ptr))
    emit(item, 'treePaletteSlotTable', norm(elf.names_at(pal_ptr)))
    n = sym_size_at(pal_ptr) or 7
    emit(item, 'treePaletteSlotTable.bytes', elf.read_addr(pal_ptr, n).hex())


for item_id, item, b in items:
    if post:
        blob = table_entry('gBerries', b, berry_size)
        if blob is None:
            emit(item, 'gBerries', 'OOB')
            continue
        dump_info(item, blob, berry_members, 'info.')
        v = int.from_bytes(blob, 'little')
        get = {m[0]: m for m in berry_members}

        def f(name):
            path, off, bit, bits, kind = get[name]
            return (v >> bit) & ((1 << bits) - 1)
        emit(item, 'naturalGift.type', f('naturalGiftType'))
        emit(item, 'naturalGift.power', f('naturalGiftPower'))
        emit(item, 'crush.difficulty', f('berryCrushDifficulty'))
        emit(item, 'crush.powder', f('berryCrushPowder'))
        emit(item, 'berryPic', norm(elf.names_at(f('berryPic'))))
        emit(item, 'berryPal', norm(elf.names_at(f('berryPal'))))
        tree_tables(item, f('berryTreePicTable'), f('berryTreePaletteSlotTable'))
    else:
        blob = table_entry('gBerries', b - 1, berry_size)
        dump_info(item, blob, berry_members, '')
        tsz, tmem = dw.members('TypePower')
        tp = table_entry('gNaturalGiftTable', b, tsz)
        if tp is None:
            emit(item, 'naturalGift.type', 'OOB')
            emit(item, 'naturalGift.power', 'OOB')
        else:
            tv = {m[0]: (int.from_bytes(tp, 'little') >> m[2]) & ((1 << m[3]) - 1) for m in tmem}
            emit(item, 'naturalGift.type', tv['type'])
            emit(item, 'naturalGift.power', tv['power'])
        csz, cmem = dw.members('BerryCrushBerryData')
        cb = table_entry('gBerryCrush_BerryData', b - 1, csz)
        cv = {m[0]: (int.from_bytes(cb, 'little') >> m[2]) & ((1 << m[3]) - 1) for m in cmem}
        emit(item, 'crush.difficulty', cv['difficulty'])
        emit(item, 'crush.powder', cv['powder'])
        pb = table_entry('sBerryPicTable', b - 1, 8)
        emit(item, 'berryPic', norm(elf.names_at(int.from_bytes(pb[0:4], 'little'))))
        emit(item, 'berryPal', norm(elf.names_at(int.from_bytes(pb[4:8], 'little'))))
        pic = int.from_bytes(table_entry('gBerryTreePicTablePointers', b - 1, 4), 'little')
        pal = int.from_bytes(table_entry('gBerryTreePaletteSlotTablePointers', b - 1, 4), 'little')
        tree_tables(item, pic, pal)
