#!/usr/bin/env python3
"""ELF32 little-endian 최소 파서: 섹션, 심볼표, 주소/심볼 기준 바이트 읽기.
링크된 실행 파일(pokehns.elf)과 재배치 오브젝트(.o) 모두 지원한다."""
import bisect
import struct


class Elf:
    def __init__(self, path):
        self.path = path
        self.data = open(path, 'rb').read()
        d = self.data
        assert d[:4] == b'\x7fELF' and d[4] == 1 and d[5] == 1, 'ELF32 LE only'
        self.e_type = struct.unpack_from('<H', d, 16)[0]  # 1=REL, 2=EXEC
        shoff = struct.unpack_from('<I', d, 32)[0]
        shentsize, shnum, shstrndx = struct.unpack_from('<HHH', d, 46)
        self.sections = []
        for i in range(shnum):
            name, typ, flags, addr, off, size, link, info, align, entsize = \
                struct.unpack_from('<IIIIIIIIII', d, shoff + i * shentsize)
            self.sections.append(dict(name_off=name, type=typ, flags=flags, addr=addr, off=off, size=size,
                                      link=link, entsize=entsize, idx=i))
        shstr = self.sections[shstrndx]
        for s in self.sections:
            s['name'] = self._cstr(shstr['off'] + s['name_off'])
        self.syms = []
        for s in self.sections:
            if s['type'] != 2:  # SHT_SYMTAB
                continue
            strtab = self.sections[s['link']]
            for j in range(s['size'] // 16):
                nm, val, sz, info, other, shndx = struct.unpack_from('<IIIBBH', d, s['off'] + j * 16)
                name = self._cstr(strtab['off'] + nm)
                if not name:
                    continue
                self.syms.append(dict(name=name, value=val, size=sz, type=info & 0xF, bind=info >> 4, shndx=shndx))
        self.by_name = {}
        for sy in self.syms:
            # 전역 심볼 우선
            if sy['name'] not in self.by_name or (sy['bind'] == 1 and self.by_name[sy['name']]['bind'] != 1):
                self.by_name[sy['name']] = sy
        # 주소 -> 이름 (실행 파일에서 포인터 해석용). 데이터/함수 심볼만, 매핑 심볼($d 등) 제외
        self._addr = {}
        for sy in self.syms:
            if sy['shndx'] == 0 or sy['name'].startswith('$') or sy['type'] in (3, 4):  # SECTION, FILE
                continue
            self._addr.setdefault(sy['value'], []).append(sy)
        self._sorted_addrs = sorted(self._addr)

    def _cstr(self, off):
        end = self.data.index(b'\0', off)
        return self.data[off:end].decode('latin-1')

    def read_addr(self, addr, n):
        """실행 파일: 가상주소 addr 에서 n 바이트 (PROGBITS 섹션에서만)"""
        for s in self.sections:
            if s['type'] == 1 and s['addr'] <= addr and addr + n <= s['addr'] + s['size'] and s['addr'] != 0:
                o = s['off'] + (addr - s['addr'])
                return self.data[o:o + n]
        raise KeyError('address 0x%08x (+%d) not in a PROGBITS section' % (addr, n))

    def sym(self, name):
        return self.by_name[name]

    def read_sym(self, name, n=None, offset=0):
        sy = self.sym(name)
        if n is None:
            n = sy['size'] - offset
        if self.e_type == 1:  # REL: value = 섹션 내 오프셋
            s = self.sections[sy['shndx']]
            o = s['off'] + sy['value'] + offset
            return self.data[o:o + n]
        return self.read_addr(sy['value'] + offset, n)

    def names_at(self, addr):
        """addr 에 정확히 놓인 심볼 이름들(정렬). 없으면 가장 가까운 앞 심볼+오프셋."""
        if addr == 0:
            return 'NULL'
        a = addr & ~1  # thumb 비트 제거
        if a in self._addr:
            names = sorted({s['name'] for s in self._addr[a]})
            return '|'.join(names)
        i = bisect.bisect_right(self._sorted_addrs, a) - 1
        if i >= 0:
            base = self._sorted_addrs[i]
            s = sorted(self._addr[base], key=lambda x: x['name'])[0]
            if s['size'] and a < base + s['size']:
                return '%s+0x%x' % (s['name'], a - base)
        return '0x%08x' % addr
