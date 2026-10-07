#!/usr/bin/env python3
"""readelf --debug-dump=info 출력으로 구조체 레이아웃을 뽑는 작은 DWARF 파서.

사용:
  dwarf_layout.py <obj.o> STRUCT [STRUCT...]      -> 평탄화한 레이아웃을 stdout으로
  (모듈로 import 해서 Dwarf(obj).members('Berry') 식으로도 쓴다)

출력 줄 형식(탭 구분):
  STRUCT <name> size=<bytes>
  <name>\t<path>\t<byte_off>\t<bit_off_in_struct>\t<bit_size>\t<leaf_kind>
leaf_kind 는 typedef 이름을 풀어 u8/s16/enum2/ptr 같은 '크기·부호' 수준으로 정규화한다.
(구조체 타입 이름 변경 Berry2->EnigmaBerryInfo, u16->enum Item 같은 이름 변화는 차이로 잡지 않고
 오프셋·비트폭·크기 변화만 잡기 위함)
"""
import os
import re
import subprocess
import sys

READELF = os.environ.get(  # (복사본: berry-7305/dwarf_layout.py, READELF 기본값만 PATH의 arm-none-eabi-readelf로 바꿈)
    'READELF',
    'arm-none-eabi-readelf')

DIE_RE = re.compile(r'^\s*<(\d+)><([0-9a-f]+)>: Abbrev Number: (\d+)(?: \((DW_TAG_\w+)\))?')
ATTR_RE = re.compile(r'^\s*<[0-9a-f]+>\s+(DW_AT_\w+)\s*:\s?(.*)$')


class Die:
    __slots__ = ('off', 'tag', 'attrs', 'children', 'parent')

    def __init__(self, off, tag, parent):
        self.off = off
        self.tag = tag
        self.attrs = {}
        self.children = []
        self.parent = parent


def _val(raw):
    raw = raw.strip()
    # readelf -W 는 "(strp) (offset: 0x..): name", "(data1) 4", "(ref4) <0x..>" 처럼 form 이름을 붙인다
    m = re.match(r'^\((?:strp|line_strp|strx\d?|string|data\d+|sdata|udata|ref\d+|ref_udata|flag_present|'
                 r'implicit_const|sec_offset|exprloc|block\d?)\)\s*(.*)$', raw)
    if m:
        raw = m.group(1)
    m = re.match(r'^\((?:indirect (?:line )?string, )?offset: 0x[0-9a-f]+\):\s*(.*)$', raw)
    if m:
        return m.group(1)
    m = re.match(r'^<0x([0-9a-f]+)>(?:,.*)?$', raw)
    if m:
        return ('ref', int(m.group(1), 16))
    m = re.match(r'^(-?\d+)$', raw)
    if m:
        return int(m.group(1))
    m = re.match(r'^0x([0-9a-f]+)$', raw)
    if m:
        return int(m.group(1), 16)
    m = re.match(r'^\d+ byte block: .*\(DW_OP_plus_uconst: (\d+)\)', raw)
    if m:
        return int(m.group(1))
    return raw


class Dwarf:
    def __init__(self, obj):
        out = subprocess.run([READELF, '--debug-dump=info', '-W', obj],
                             check=True, capture_output=True, text=True, errors='replace').stdout
        self.dies = {}
        self.by_name = {}
        stack = []
        cur = None
        for line in out.splitlines():
            m = DIE_RE.match(line)
            if m:
                depth, off, abbrev, tag = int(m.group(1)), int(m.group(2), 16), int(m.group(3)), m.group(4)
                while stack and stack[-1][0] >= depth:
                    stack.pop()
                if abbrev == 0:
                    cur = None
                    continue
                parent = stack[-1][1] if stack else None
                cur = Die(off, tag, parent)
                self.dies[off] = cur
                if parent is not None:
                    parent.children.append(cur)
                stack.append((depth, cur))
                continue
            m = ATTR_RE.match(line)
            if m and cur is not None:
                cur.attrs[m.group(1)] = _val(m.group(2))
        for d in self.dies.values():
            if d.tag in ('DW_TAG_structure_type', 'DW_TAG_union_type') and 'DW_AT_name' in d.attrs \
                    and 'DW_AT_declaration' not in d.attrs and d.children:
                self.by_name.setdefault(d.attrs['DW_AT_name'], d)

    def ref(self, die, attr='DW_AT_type'):
        v = die.attrs.get(attr)
        if isinstance(v, tuple) and v[0] == 'ref':
            return self.dies.get(v[1])
        return None

    def strip(self, t):
        while t is not None and t.tag in ('DW_TAG_typedef', 'DW_TAG_const_type', 'DW_TAG_volatile_type',
                                          'DW_TAG_atomic_type', 'DW_TAG_restrict_type'):
            t = self.ref(t)
        return t

    def size(self, t):
        t = self.strip(t)
        if t is None:
            return 0
        if 'DW_AT_byte_size' in t.attrs:
            return t.attrs['DW_AT_byte_size']
        if t.tag == 'DW_TAG_pointer_type':
            return 4
        if t.tag == 'DW_TAG_array_type':
            n = 1
            for c in t.children:
                if c.tag == 'DW_TAG_subrange_type':
                    n *= self.count(c)
            return n * self.size(self.ref(t))
        return 0

    @staticmethod
    def count(sub):
        if 'DW_AT_count' in sub.attrs:
            return sub.attrs['DW_AT_count']
        if 'DW_AT_upper_bound' in sub.attrs and isinstance(sub.attrs['DW_AT_upper_bound'], int):
            return sub.attrs['DW_AT_upper_bound'] + 1
        return 0

    def leaf_kind(self, t):
        t = self.strip(t)
        if t is None:
            return 'void'
        if t.tag == 'DW_TAG_base_type':
            enc = str(t.attrs.get('DW_AT_encoding', ''))
            sz = t.attrs.get('DW_AT_byte_size', 0)
            signed = 's' if 'signed' in enc and 'unsigned' not in enc else 'u'
            if 'boolean' in enc:
                signed = 'b'
            if 'float' in enc:
                signed = 'f'
            return '%s%d' % (signed, sz * 8)
        if t.tag == 'DW_TAG_enumeration_type':
            return 'enum%d' % t.attrs.get('DW_AT_byte_size', 0)
        if t.tag == 'DW_TAG_pointer_type':
            return 'ptr'
        if t.tag == 'DW_TAG_subroutine_type':
            return 'func'
        return t.tag.replace('DW_TAG_', '')

    def flatten(self, t, prefix, base, out):
        """(path, byte_off, bit_off_in_struct, bit_size, kind) 리스트"""
        t = self.strip(t)
        if t is None:
            return
        if t.tag in ('DW_TAG_structure_type', 'DW_TAG_union_type'):
            for m in t.children:
                if m.tag != 'DW_TAG_member':
                    continue
                name = m.attrs.get('DW_AT_name', '<anon>')
                path = (prefix + '.' + name) if prefix else name
                mt = self.ref(m)
                if 'DW_AT_data_bit_offset' in m.attrs:
                    bit = base * 8 + m.attrs['DW_AT_data_bit_offset']
                    out.append((path, bit // 8, bit, m.attrs.get('DW_AT_bit_size', 0), self.leaf_kind(mt)))
                    continue
                off = m.attrs.get('DW_AT_data_member_location', 0)
                if not isinstance(off, int):
                    off = 0
                if 'DW_AT_bit_size' in m.attrs:  # DWARF<=4 style
                    bit = (base + off) * 8 + m.attrs.get('DW_AT_bit_offset', 0)
                    out.append((path, bit // 8, bit, m.attrs['DW_AT_bit_size'], self.leaf_kind(mt)))
                    continue
                self.flatten(mt, path, base + off, out)
        elif t.tag == 'DW_TAG_array_type':
            dims = [self.count(c) for c in t.children if c.tag == 'DW_TAG_subrange_type']
            et = self.ref(t)
            esz = self.size(et)
            out.append((prefix + ''.join('[%d]' % d for d in dims), base, base * 8, self.size(t) * 8,
                        'array of %s x%d' % (self.leaf_kind(et) if self.strip(et).tag not in
                                              ('DW_TAG_structure_type', 'DW_TAG_union_type', 'DW_TAG_array_type')
                                              else 'struct%d' % esz, esz)))
            st = self.strip(et)
            if st is not None and st.tag in ('DW_TAG_structure_type', 'DW_TAG_union_type', 'DW_TAG_array_type'):
                self.flatten(et, prefix + '[]', base, out)
        else:
            out.append((prefix, base, base * 8, self.size(t) * 8, self.leaf_kind(t)))

    def members(self, struct_name):
        d = self.by_name.get(struct_name)
        if d is None:
            return None, None
        out = []
        self.flatten(d, '', 0, out)
        return d.attrs.get('DW_AT_byte_size', 0), out


def main():
    obj = sys.argv[1]
    dw = Dwarf(obj)
    for name in sys.argv[2:]:
        size, mem = dw.members(name)
        if mem is None:
            print('STRUCT %s MISSING' % name)
            continue
        print('STRUCT %s size=%d' % (name, size))
        for p, off, bit, bits, kind in mem:
            print('%s\t%s\t%d\t%d\t%d\t%s' % (name, p, off, bit, bits, kind))


if __name__ == '__main__':
    main()
