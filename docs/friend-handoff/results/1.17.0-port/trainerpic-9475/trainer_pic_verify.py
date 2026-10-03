#!/usr/bin/env python3
"""seq 128 #9475 (Refactor/trainer pic info) 이식 전후 트레이너 그림 데이터 대조 도구.

이식 전 ELF(옛 구조: gTrainerSprites / gTrainerBacksprites / Trainer.trainerBackPic)와
이식 후 ELF(새 구조: gTrainerPicInfo[] -> TrainerFrontPicInfo / TrainerBackPicInfo)에서
"트레이너가 실제로 쓰는 그림 데이터"를 내용(바이트 해시, 좌표, 애니메이션 명령 열)으로 뽑아 비교한다.
심볼 주소·팔레트 태그·enum 숫자 값은 비교하지 않는다(이식으로 바뀌는 것이 정상이므로).

부속 명령
  run        옛 덤프(캐시) + 새 덤프 생성 후 비교. 메인이 적용 뒤 실행하는 명령.
  dump       ELF 하나를 덤프(--layout old|new).
  compare    덤프 두 개 비교.
  make-map   옛 enum 이름 -> 새 enum 이름 대응표(pic_name_map.tsv) 생성.
  selftest   도구 자체 검증(같은 ELF 두 번, 변조 덤프 검출, 새 구조 합성 ELF).

표준 라이브러리 + arm-none-eabi-gcc/objcopy 만 쓴다. 저장소에는 아무것도 쓰지 않는다
(헤더는 읽기만 하고, 컴파일 산출물은 --work 디렉터리에 쓴다).
"""
import argparse
import bisect
import hashlib
import os
import re
import shutil
import struct
import subprocess
import sys
import tarfile
import io

CHUNK = '/home/hjm0725/hns-sync-work/chunk-128'
REPO = '/home/hjm0725/pokehns-expansion-kor'
UPSTREAM = '/home/hjm0725/pokeemerald-expansion-upstream'
UPSTREAM_COMMIT = 'becfa70a97'
VERIFY = os.path.join(CHUNK, 'verify')
WORK = os.path.join(CHUNK, 'tmp-D', 'work')
BASE_ELF = os.path.join(CHUNK, 'base', 'pokehns.elf')
BASE_HEAD = os.path.join(CHUNK, 'base', 'HEAD.txt')
MAP_FILE = os.path.join(VERIFY, 'pic_name_map.tsv')
PLAYER_FILE = os.path.join(VERIFY, 'player_cases.tsv')
OLD_DUMP = os.path.join(VERIFY, 'old_dump.tsv')

GCC = 'arm-none-eabi-gcc'
OBJCOPY = 'arm-none-eabi-objcopy'
# Makefile(HnS) 의 CPPFLAGS / CFLAGS 중 레이아웃에 영향을 주는 것.
CPPFLAGS = ['-Wno-trigraphs', '-DMODERN=1', '-DTESTING=0', '-DPOKEMON_HNS', '-std=gnu17']
CFLAGS = ['-mthumb', '-mthumb-interwork', '-O2', '-mabi=apcs-gnu', '-mtune=arm7tdmi', '-march=armv4t']

DUMP_VERSION = 1


def die(msg):
    sys.stderr.write('ERROR: %s\n' % msg)
    sys.exit(2)


def sha(b):
    return hashlib.sha1(b).hexdigest()[:16]


# ----------------------------------------------------------------------------
# ELF reader (ELF32 little endian, executable)
# ----------------------------------------------------------------------------
class Elf:
    def __init__(self, path):
        self.path = path
        with open(path, 'rb') as f:
            d = f.read()
        self.data = d
        if d[:4] != b'\x7fELF' or d[4] != 1 or d[5] != 1:
            die('%s: ELF32 LE 가 아님' % path)
        (_t, _m, _v, _entry, _phoff, shoff, _fl, _ehs, _phes, _phn,
         shentsize, shnum, shstrndx) = struct.unpack_from('<HHIIIIIHHHHHH', d, 16)
        secs = []
        for i in range(shnum):
            (nm, typ, flags, addr, off, size, link, info, _al, _es) = struct.unpack_from(
                '<IIIIIIIIII', d, shoff + i * shentsize)
            secs.append(dict(nm=nm, type=typ, flags=flags, addr=addr, off=off, size=size, link=link))
        shs = secs[shstrndx]
        for s in secs:
            s['name'] = self._cstr(shs['off'] + s['nm'])
        self.secs = secs
        rng = [(s['addr'], s['addr'] + s['size'], s['off']) for s in secs
               if s['type'] == 1 and (s['flags'] & 2) and s['size']]
        rng.sort()
        self.rng = rng
        self.rng_starts = [r[0] for r in rng]
        self.glob = {}      # name -> (addr, size)
        self.loc = {}       # name -> [(addr,size)]
        objs = []
        for s in secs:
            if s['type'] != 2:
                continue
            strt = secs[s['link']]
            blob = d[s['off']:s['off'] + s['size']]
            for (st_name, val, size, info, _o, shndx) in struct.iter_unpack('<IIIBBH', blob):
                if (info & 0xf) != 1 or shndx == 0:   # STT_OBJECT, defined
                    continue
                name = self._cstr(strt['off'] + st_name)
                if (info >> 4) == 1:
                    self.glob[name] = (val, size)
                else:
                    self.loc.setdefault(name, []).append((val, size))
                if size:
                    objs.append((val, size, name))
        objs.sort()
        self.objs = objs
        self.obj_starts = [o[0] for o in objs]

    def _cstr(self, off):
        e = self.data.index(b'\0', off)
        return self.data[off:e].decode('latin1')

    def sym(self, name, required=True):
        if name in self.glob:
            return self.glob[name]
        if name in self.loc and len(self.loc[name]) == 1:
            return self.loc[name][0]
        if required:
            die('%s: 심볼 %s 없음' % (self.path, name))
        return None

    def read(self, addr, n):
        i = bisect.bisect_right(self.rng_starts, addr) - 1
        if i < 0:
            return None
        a, e, off = self.rng[i]
        if addr + n > e:
            return None
        return self.data[off + addr - a: off + addr - a + n]

    def u8(self, a):
        return self.read(a, 1)[0]

    def u16(self, a):
        return struct.unpack('<H', self.read(a, 2))[0]

    def s16(self, a):
        return struct.unpack('<h', self.read(a, 2))[0]

    def u32(self, a):
        return struct.unpack('<I', self.read(a, 4))[0]

    def uint(self, a, size):
        return int.from_bytes(self.read(a, size), 'little')

    def containing(self, addr):
        """addr 를 포함하는 OBJECT 심볼 (start, size, name). 시작 주소 일치를 우선한다."""
        i = bisect.bisect_right(self.obj_starts, addr) - 1
        exact = None
        best = None
        j = i
        while j >= 0 and i - j < 256:
            st, sz, nm = self.objs[j]
            if st <= addr < st + sz:
                if st == addr:
                    if exact is None or sz > exact[1]:
                        exact = (st, sz, nm)
                elif best is None:
                    best = (st, sz, nm)
            if st < addr and exact is not None:
                break
            j -= 1
        return exact or best


# ----------------------------------------------------------------------------
# value canonicalisation
# ----------------------------------------------------------------------------
def blob_id(elf, ptr):
    """포인터가 가리키는 데이터: 포함 심볼의 끝까지를 해시(압축 그림은 압축 바이트 그대로)."""
    if ptr == 0:
        return 'NULL'
    s = elf.containing(ptr)
    if s is None:
        return 'NOSYM'
    st, sz, _nm = s
    n = st + sz - ptr
    b = elf.read(ptr, n)
    if b is None:
        return 'BADPTR'
    return '%d:%s%s' % (n, sha(b), '' if st == ptr else '+mid')


def blob_sym(elf, ptr):
    if ptr == 0:
        return '-'
    s = elf.containing(ptr)
    return s[2] if s else '?'


def anim_seq(elf, p):
    if p == 0:
        return 'NULL'
    out = []
    for k in range(64):
        b = elf.read(p + 4 * k, 4)
        if b is None:
            out.append('BADPTR')
            break
        w = struct.unpack('<I', b)[0]
        t = w & 0xffff
        if t == 0xffff:
            out.append('E')
            break
        if t == 0xfffe:
            out.append('J%d' % ((w >> 16) & 0x3f))
            break
        if t == 0xfffd:
            out.append('L%d' % ((w >> 16) & 0x3f))
            continue
        out.append('F%d:%d%s%s' % (t, (w >> 16) & 0x3f, 'h' if (w >> 22) & 1 else '', 'v' if (w >> 23) & 1 else ''))
    else:
        out.append('...')
    return ','.join(out)


def anim_table(elf, ptr):
    if ptr == 0:
        return 'NULL'
    s = elf.containing(ptr)
    n = (s[0] + s[1] - ptr) // 4 if s else 8
    n = max(0, min(n, 32))
    raw = elf.read(ptr, 4 * n)
    if raw is None:
        return 'BADPTR'
    ents = struct.unpack('<%dI' % n, raw)
    return '%d[%s]' % (n, '|'.join(anim_seq(elf, p) for p in ents))


def fmt_front(elf, img, pal, anim, mx, my, rot, usz):
    return 'img=%s pal=%s anim=%s mug=%d,%d rot=%d usz=%d' % (
        blob_id(elf, img), blob_id(elf, pal), anim_table(elf, anim), mx, my, rot, usz)


def fmt_back(elf, csize, cyoff, img, fsz, rel, pal, anim):
    return 'coords=%d,%d img=%s fsz=%d rel=%d pal=%s anim=%s' % (
        csize, cyoff, blob_id(elf, img), fsz, rel, blob_id(elf, pal), anim_table(elf, anim))


# ----------------------------------------------------------------------------
# source trees, preprocessing and compile-time probes
# ----------------------------------------------------------------------------
def git(repo, *args, binary=False):
    r = subprocess.run(['git', '-C', repo] + list(args), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if r.returncode != 0:
        die('git %s: %s' % (' '.join(args), r.stderr.decode(errors='replace')))
    return r.stdout if binary else r.stdout.decode('utf-8', errors='replace')


def materialize_rev(repo, rev, work):
    """git archive <rev> include -> work/src-<rev>/, 생성(ignored) 헤더는 작업 트리에서 보충."""
    full = git(repo, 'rev-parse', rev).strip()
    dest = os.path.join(work, 'src-' + full[:10])
    mark = os.path.join(dest, '.complete')
    if os.path.exists(mark):
        return dest
    if os.path.exists(dest):
        shutil.rmtree(dest)
    os.makedirs(dest)
    tar = git(repo, 'archive', full, 'include', binary=True)
    with tarfile.open(fileobj=io.BytesIO(tar)) as t:
        t.extractall(dest)
    gen = git(repo, 'ls-files', '-o', '-i', '--exclude-standard', 'include').split()
    for f in gen:
        dst = os.path.join(dest, f)
        if not os.path.exists(dst):
            os.makedirs(os.path.dirname(dst), exist_ok=True)
            shutil.copy2(os.path.join(repo, f), dst)
    open(mark, 'w').write(full + '\n')
    return dest


PROBE_INCLUDES = ['global.h', 'data.h', 'constants/trainers.h', 'constants/opponents.h',
                  'constants/battle_partner.h', 'constants/difficulty.h']


def _flags(src):
    return ['-iquote', os.path.join(src, 'include')] + CPPFLAGS


def preprocess(src, work, tag, macros=False):
    """macros=True 면 -dD(정의 위치를 줄 표시로 남김) 출력."""
    os.makedirs(work, exist_ok=True)
    c = os.path.join(work, 'pp_%s.c' % tag)
    with open(c, 'w') as f:
        for h in PROBE_INCLUDES:
            f.write('#include "%s"\n' % h)
    cmd = [GCC, '-E'] + (['-dD'] if macros else []) + _flags(src) + CFLAGS + [c]
    r = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if r.returncode != 0:
        die('preprocess failed (%s):\n%s' % (src, r.stderr.decode(errors='replace')[-3000:]))
    return r.stdout.decode('latin1')


def probe(src, work, tag, exprs):
    """exprs: [(key, C expr)] -> {key: int}. ARM 컴파일러로 평가(offsetof/sizeof/enum 값)."""
    os.makedirs(work, exist_ok=True)
    c = os.path.join(work, 'probe_%s.c' % tag)
    o = c[:-2] + '.o'
    b = c[:-2] + '.bin'
    with open(c, 'w') as f:
        for h in PROBE_INCLUDES:
            f.write('#include "%s"\n' % h)
        f.write('#include <stddef.h>\n')
        f.write('const int probe_vals[] __attribute__((section(".probe"))) = {\n')
        for k, e in exprs:
            f.write('    (int)(%s), /* %s */\n' % (e, k))
        f.write('};\n')
    cmd = [GCC, '-c'] + _flags(src) + CFLAGS + ['-o', o, c]
    r = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if r.returncode != 0:
        die('probe compile failed (%s, %s):\n%s' % (src, tag, r.stderr.decode(errors='replace')[-3000:]))
    subprocess.run([OBJCOPY, '-O', 'binary', '--only-section=.probe', o, b], check=True)
    raw = open(b, 'rb').read()
    vals = struct.unpack('<%di' % (len(raw) // 4), raw)
    if len(vals) != len(exprs):
        die('probe size mismatch')
    return dict(zip([k for k, _ in exprs], vals))


def parse_enum_names(text, enum_name=None, member_prefix=None):
    """전처리된 코드에서 enum 본문의 멤버 이름 목록(선언 순서)."""
    if enum_name:
        m = re.search(r'enum\s+(?:__attribute__\s*\(\([^)]*\)\)\s*)?' + enum_name + r'\s*\{(.*?)\}', text, re.S)
    else:
        m = None
        for mm in re.finditer(r'enum\s*(?:__attribute__\s*\(\([^)]*\)\)\s*)?\{(.*?)\}', text, re.S):
            if re.search(r'\b' + member_prefix, mm.group(1)):
                m = mm
                break
    if not m:
        return []
    names = []
    for part in m.group(1).split(','):
        part = part.strip()
        if not part:
            continue
        nm = re.match(r'([A-Za-z_]\w*)', part)
        if nm:
            names.append(nm.group(1))
    return names


class Layout:
    """한쪽(옛/새) 소스 트리에서 구조체 오프셋·상수·enum 값을 뽑는다."""

    def __init__(self, src, work, kind):
        self.src = src
        self.kind = kind
        tag = kind
        text = preprocess(src, work, tag + '_dD', macros=True)
        pic_names = parse_enum_names(text, 'TrainerPicID')
        if not pic_names:
            die('%s: enum TrainerPicID 를 찾지 못함' % src)
        diff_names = parse_enum_names(text, 'DifficultyLevel')
        fac_names = parse_enum_names(text, None, 'FACILITY_CLASS_HIKER')
        common = [
            ('T_SIZE', 'sizeof(struct Trainer)'),
            ('T_PIC', 'offsetof(struct Trainer, trainerPic)'),
            ('T_PIC_SZ', 'sizeof(((struct Trainer *)0)->trainerPic)'),
            ('T_PARTY', 'offsetof(struct Trainer, partySize)'),
            ('T_PARTYPTR', 'offsetof(struct Trainer, party)'),
            ('DIFFICULTY_COUNT', 'DIFFICULTY_COUNT'),
            ('TRAINERS_COUNT', 'TRAINERS_COUNT'),
            ('PARTNER_COUNT', 'PARTNER_COUNT'),
            ('TRAINER_PIC_COUNT', 'TRAINER_PIC_COUNT'),
            ('TRAINER_PIC_SIZE', 'TRAINER_PIC_SIZE'),
            ('ANIMCMD_SIZE', 'sizeof(union AnimCmd)'),
        ]
        if kind == 'old':
            spec = common + [
                ('T_BACKPIC', 'offsetof(struct Trainer, trainerBackPic)'),
                ('T_BACKPIC_SZ', 'sizeof(((struct Trainer *)0)->trainerBackPic)'),
                ('TRAINER_PIC_FRONT_COUNT', 'TRAINER_PIC_FRONT_COUNT'),
                ('TS_SIZE', 'sizeof(struct TrainerSprite)'),
                ('TS_YOFF', 'offsetof(struct TrainerSprite, y_offset)'),
                ('TS_IMG', 'offsetof(struct TrainerSprite, frontPic.data)'),
                ('TS_USZ', 'offsetof(struct TrainerSprite, frontPic.size)'),
                ('TS_PAL', 'offsetof(struct TrainerSprite, palette.data)'),
                ('TS_ANIM', 'offsetof(struct TrainerSprite, animation)'),
                ('TS_MUGX', 'offsetof(struct TrainerSprite, mugshotCoords.x)'),
                ('TS_MUGY', 'offsetof(struct TrainerSprite, mugshotCoords.y)'),
                ('TS_ROT', 'offsetof(struct TrainerSprite, mugshotRotation)'),
                ('TB_SIZE', 'sizeof(struct TrainerBacksprite)'),
                ('TB_CSIZE', 'offsetof(struct TrainerBacksprite, coordinates.size)'),
                ('TB_CYOFF', 'offsetof(struct TrainerBacksprite, coordinates.y_offset)'),
                ('TB_IMG', 'offsetof(struct TrainerBacksprite, backPic.data)'),
                ('TB_FSZ', 'offsetof(struct TrainerBacksprite, backPic.size)'),
                ('TB_REL', 'offsetof(struct TrainerBacksprite, backPic.relativeFrames)'),
                ('TB_PAL', 'offsetof(struct TrainerBacksprite, palette.data)'),
                ('TB_ANIM', 'offsetof(struct TrainerBacksprite, animation)'),
            ]
        else:
            spec = common + [
                ('PI_SIZE', 'sizeof(struct TrainerPicInfo)'),
                ('PI_FRONT', 'offsetof(struct TrainerPicInfo, frontPic)'),
                ('PI_BACK', 'offsetof(struct TrainerPicInfo, backPic)'),
                ('FI_IMG', 'offsetof(struct TrainerFrontPicInfo, imageData)'),
                ('FI_PAL', 'offsetof(struct TrainerFrontPicInfo, paletteData)'),
                ('FI_ANIM', 'offsetof(struct TrainerFrontPicInfo, animation)'),
                ('FI_MUGX', 'offsetof(struct TrainerFrontPicInfo, mugshotCoords.x)'),
                ('FI_MUGY', 'offsetof(struct TrainerFrontPicInfo, mugshotCoords.y)'),
                ('FI_ROT', 'offsetof(struct TrainerFrontPicInfo, mugshotRotation)'),
                ('BI_CSIZE', 'offsetof(struct TrainerBackPicInfo, coordinates.size)'),
                ('BI_CYOFF', 'offsetof(struct TrainerBackPicInfo, coordinates.y_offset)'),
                ('BI_IMG', 'offsetof(struct TrainerBackPicInfo, image.data)'),
                ('BI_FSZ', 'offsetof(struct TrainerBackPicInfo, image.size)'),
                ('BI_REL', 'offsetof(struct TrainerBackPicInfo, image.relativeFrames)'),
                ('BI_PAL', 'offsetof(struct TrainerBackPicInfo, paletteData)'),
                ('BI_ANIM', 'offsetof(struct TrainerBackPicInfo, animation)'),
            ]
        self.L = probe(src, work, tag + '_layout', spec)
        enum_spec = [('E:' + n, n) for n in pic_names]
        enum_spec += [('F:' + n, n) for n in fac_names]
        enum_spec += [('D:' + n, n) for n in diff_names]
        ev = probe(src, work, tag + '_enum', enum_spec)
        self.pic_values = [(n, ev['E:' + n]) for n in pic_names]
        self.val2pic = {}
        for n, v in self.pic_values:
            if n.endswith('_COUNT'):
                continue
            self.val2pic.setdefault(v, n)
        self.fac_names = {}
        for n in fac_names:
            self.fac_names.setdefault(ev['F:' + n], n)
        self.diff_names = {}
        for n in diff_names:
            if not n.endswith('_COUNT'):
                self.diff_names.setdefault(ev['D:' + n], n.replace('DIFFICULTY_', ''))
        self.trainer_names = {}
        self.partner_names = {}
        cur = ''
        hns_named = set()
        for line in text.splitlines():
            mm = re.match(r'# \d+ "([^"]*)"', line)
            if mm:
                cur = mm.group(1)
                continue
            mm = re.match(r'#define (TRAINER_\w+|PARTNER_\w+)\s+(0x[0-9A-Fa-f]+|\d+)\s*$', line)
            if not mm:
                continue
            v = int(mm.group(2), 0)
            if mm.group(1).startswith('TRAINER_') and cur.endswith('constants/opponents_hns.h'):
                self.trainer_names[v] = mm.group(1) if v not in hns_named else self.trainer_names[v]
                hns_named.add(v)
            elif mm.group(1).startswith('TRAINER_') and cur.endswith('constants/opponents.h'):
                if v not in hns_named:
                    self.trainer_names.setdefault(v, mm.group(1))
            elif mm.group(1).startswith('PARTNER_') and cur.endswith('constants/battle_partner.h'):
                self.partner_names.setdefault(v, mm.group(1))

    def pic_name(self, v):
        return self.val2pic.get(v, '#%d' % v)


# ----------------------------------------------------------------------------
# dump
# ----------------------------------------------------------------------------
class Dumper:
    def __init__(self, elf, lay):
        self.e = elf
        self.lay = lay
        self.L = lay.L
        self.kind = lay.kind
        self.notes = []
        L = self.L
        if self.kind == 'old':
            self.ts = elf.sym('gTrainerSprites')
            self.tb = elf.sym('gTrainerBacksprites')
            if self.ts[1] != L['TRAINER_PIC_FRONT_COUNT'] * L['TS_SIZE']:
                self.notes.append('gTrainerSprites size %d != FRONT_COUNT*sizeof %d' % (
                    self.ts[1], L['TRAINER_PIC_FRONT_COUNT'] * L['TS_SIZE']))
            if self.tb[1] != L['TRAINER_PIC_COUNT'] * L['TB_SIZE']:
                self.notes.append('gTrainerBacksprites size %d != PIC_COUNT*sizeof %d' % (
                    self.tb[1], L['TRAINER_PIC_COUNT'] * L['TB_SIZE']))
        else:
            self.pi = elf.sym('gTrainerPicInfo')
            if self.pi[1] != L['TRAINER_PIC_COUNT'] * L['PI_SIZE']:
                self.notes.append('gTrainerPicInfo size %d != PIC_COUNT*sizeof %d' % (
                    self.pi[1], L['TRAINER_PIC_COUNT'] * L['PI_SIZE']))

    # front / back by pic value -------------------------------------------------
    def front(self, v):
        e, L = self.e, self.L
        if self.kind == 'old':
            if not 0 <= v < L['TRAINER_PIC_FRONT_COUNT']:
                return 'OUT_OF_RANGE'
            b = self.ts[0] + v * L['TS_SIZE']
            return fmt_front(e, e.u32(b + L['TS_IMG']), e.u32(b + L['TS_PAL']), e.u32(b + L['TS_ANIM']),
                             e.s16(b + L['TS_MUGX']), e.s16(b + L['TS_MUGY']), e.s16(b + L['TS_ROT']),
                             e.u16(b + L['TS_USZ']))
        if not 0 <= v < L['TRAINER_PIC_COUNT']:
            return 'OUT_OF_RANGE'
        p = e.u32(self.pi[0] + v * L['PI_SIZE'] + L['PI_FRONT'])
        if p == 0:
            return 'NULL'
        return fmt_front(e, e.u32(p + L['FI_IMG']), e.u32(p + L['FI_PAL']), e.u32(p + L['FI_ANIM']),
                         e.s16(p + L['FI_MUGX']), e.s16(p + L['FI_MUGY']), e.s16(p + L['FI_ROT']),
                         L['TRAINER_PIC_SIZE'])

    def back(self, v):
        e, L = self.e, self.L
        if not 0 <= v < L['TRAINER_PIC_COUNT']:
            return 'OUT_OF_RANGE'
        if self.kind == 'old':
            b = self.tb[0] + v * L['TB_SIZE']
            pfx = 'TB'
        else:
            b = e.u32(self.pi[0] + v * L['PI_SIZE'] + L['PI_BACK'])
            if b == 0:
                return 'NULL'
            pfx = 'BI'
        g = lambda k: L[pfx + '_' + k]
        return fmt_back(e, e.u8(b + g('CSIZE')), e.u8(b + g('CYOFF')), e.u32(b + g('IMG')),
                        e.u16(b + g('FSZ')), e.u8(b + g('REL')), e.u32(b + g('PAL')), e.u32(b + g('ANIM')))

    def front_syms(self, v):
        """사람이 읽는 참고용(비교하지 않음): 그림/팔레트 심볼 이름."""
        e, L = self.e, self.L
        if self.kind == 'old':
            if not 0 <= v < L['TRAINER_PIC_FRONT_COUNT']:
                return ''
            b = self.ts[0] + v * L['TS_SIZE']
            return '%s,%s' % (blob_sym(e, e.u32(b + L['TS_IMG'])), blob_sym(e, e.u32(b + L['TS_PAL'])))
        if not 0 <= v < L['TRAINER_PIC_COUNT']:
            return ''
        p = e.u32(self.pi[0] + v * L['PI_SIZE'] + L['PI_FRONT'])
        if p == 0:
            return '-'
        return '%s,%s' % (blob_sym(e, e.u32(p + L['FI_IMG'])), blob_sym(e, e.u32(p + L['FI_PAL'])))

    def back_syms(self, v):
        e, L = self.e, self.L
        if not 0 <= v < L['TRAINER_PIC_COUNT']:
            return ''
        if self.kind == 'old':
            b = self.tb[0] + v * L['TB_SIZE']
            return '%s,%s' % (blob_sym(e, e.u32(b + L['TB_IMG'])), blob_sym(e, e.u32(b + L['TB_PAL'])))
        b = e.u32(self.pi[0] + v * L['PI_SIZE'] + L['PI_BACK'])
        if b == 0:
            return '-'
        return '%s,%s' % (blob_sym(e, e.u32(b + L['BI_IMG'])), blob_sym(e, e.u32(b + L['BI_PAL'])))

    # tables -------------------------------------------------------------------
    def trainer_table(self, symname, kind, count, with_back, names):
        e, L, lay = self.e, self.L, self.lay
        s = e.sym(symname, required=False)
        if s is None:
            self.notes.append('%s 없음(건너뜀)' % symname)
            return []
        nd = L['DIFFICULTY_COUNT']
        if count is None:
            count = s[1] // (nd * L['T_SIZE'])
        if s[1] != nd * count * L['T_SIZE']:
            self.notes.append('%s size %d != %d*%d*%d' % (symname, s[1], nd, count, L['T_SIZE']))
        rows = []
        for d in range(nd):
            dn = lay.diff_names.get(d, str(d))
            for i in range(count):
                b = s[0] + (d * count + i) * L['T_SIZE']
                pic = e.uint(b + L['T_PIC'], L['T_PIC_SZ'])
                party = e.u8(b + L['T_PARTY'])
                used = 1 if e.u32(b + L['T_PARTYPTR']) else 0
                key = '%s/%04d' % (dn, i)
                ctx = 'used=%d party=%d id=%s' % (used, party, names.get(i, '-'))
                rows.append((kind, key, 'FRONT', lay.pic_name(pic), self.front(pic),
                             ctx + ' syms=' + self.front_syms(pic)))
                if with_back:
                    if self.kind == 'old':
                        bp = e.uint(b + L['T_BACKPIC'], L['T_BACKPIC_SZ'])
                    else:
                        bp = pic
                    rows.append((kind, key, 'BACK', lay.pic_name(bp), self.back(bp),
                                 ctx + ' syms=' + self.back_syms(bp)))
        return rows

    def dump(self):
        e, L, lay = self.e, self.L, self.lay
        rows = []
        for n, v in lay.pic_values:
            rows.append(('ENUM', n, '-', n, str(v), ''))
        # (5) 그림 표 자체
        if self.kind == 'old':
            fc = L['TRAINER_PIC_FRONT_COUNT']
            for v in range(fc):
                rows.append(('PIC', lay.pic_name(v), 'FRONT', lay.pic_name(v), self.front(v),
                             'syms=' + self.front_syms(v)))
            for v in range(fc, L['TRAINER_PIC_COUNT']):
                rows.append(('PIC', lay.pic_name(v), 'BACK', lay.pic_name(v), self.back(v),
                             'syms=' + self.back_syms(v)))
        else:
            for v in range(L['TRAINER_PIC_COUNT']):
                n = lay.pic_name(v)
                f = self.front(v)
                if f != 'NULL':
                    rows.append(('PIC', n, 'FRONT', n, f, 'syms=' + self.front_syms(v)))
                bk = self.back(v)
                if bk != 'NULL':
                    rows.append(('PIC', n, 'BACK', n, bk, 'syms=' + self.back_syms(v)))
        # (1) gTrainers, sDebugTrainers / (1)(2) gBattlePartners
        rows += self.trainer_table('gTrainers', 'TRAINER', L['TRAINERS_COUNT'], False, lay.trainer_names)
        rows += self.trainer_table('gBattlePartners', 'PARTNER', L['PARTNER_COUNT'], True, lay.partner_names)
        rows += self.trainer_table('sDebugTrainers', 'DEBUGTR', None, False, {})
        # (4) facility class -> pic
        s = e.sym('gFacilityClassToPicIndex')
        for i in range(s[1] // 2):
            pic = e.u16(s[0] + 2 * i)
            rows.append(('FACILITY', '%03d' % i, 'FRONT', lay.pic_name(pic), self.front(pic),
                         'class=%s syms=%s' % (lay.fac_names.get(i, '-'), self.front_syms(pic))))
        return rows


def elf_digest(path):
    h = hashlib.sha1()
    with open(path, 'rb') as f:
        for chunk in iter(lambda: f.read(1 << 20), b''):
            h.update(chunk)
    return h.hexdigest()


def write_dump(path, rows, meta):
    with open(path, 'w') as f:
        f.write('# trainer_pic_verify dump v%d\n' % DUMP_VERSION)
        for k, v in meta:
            f.write('# %s: %s\n' % (k, v))
        f.write('#kind\tkey\trole\tpic\tvalue\tctx\n')
        for r in rows:
            f.write('\t'.join(r) + '\n')


def read_dump(path):
    rows = []
    meta = {}
    for line in open(path):
        line = line.rstrip('\n')
        if line.startswith('#'):
            m = re.match(r'# (\S+): (.*)$', line)
            if m:
                meta[m.group(1)] = m.group(2)
            continue
        if not line:
            continue
        p = line.split('\t')
        while len(p) < 6:
            p.append('')
        rows.append(tuple(p[:6]))
    return rows, meta


def do_dump(elf_path, layout_kind, src, work, out, src_desc):
    lay = Layout(src, work, layout_kind)
    elf = Elf(elf_path)
    d = Dumper(elf, lay)
    rows = d.dump()
    L = lay.L
    meta = [('layout', layout_kind), ('elf', elf_path), ('elf_sha1', elf_digest(elf_path)),
            ('src', src_desc),
            ('layout_values', ' '.join('%s=%d' % (k, L[k]) for k in sorted(L)))]
    for n in d.notes:
        meta.append(('note', n))
    write_dump(out, rows, meta)
    return rows


# ----------------------------------------------------------------------------
# name map (rule 2) -------------------------------------------------------------
# ----------------------------------------------------------------------------
HNS_BACK_MERGE = {
    'TRAINER_PIC_BACK_GOLD_HNS': 'TRAINER_PIC_GOLD_HNS',
    'TRAINER_PIC_BACK_KRIS_HNS': 'TRAINER_PIC_KRIS_HNS',
    'TRAINER_PIC_BACK_SILVER_HNS': 'TRAINER_PIC_SILVER_HNS',
    'TRAINER_PIC_BACK_LANCE_HNS': 'TRAINER_PIC_CHAMPION_LANCE_HNS',
}


def raw_enum_names(text):
    m = re.search(r'enum\s+__attribute__\(\(packed\)\)\s+TrainerPicID\s*\{(.*?)\};', text, re.S)
    body = re.sub(r'//[^\n]*', '', m.group(1))
    body = re.sub(r'/\*.*?\*/', '', body, flags=re.S)
    names = []
    for part in body.split(','):
        nm = re.match(r'\s*([A-Za-z_]\w*)', part)
        if nm and not nm.group(1).endswith('_COUNT'):
            names.append(nm.group(1))
    return names


def name_rule(old):
    n = old.replace('TRAINER_PIC_FRONT_', 'TRAINER_PIC_').replace('TRAINER_PIC_BACK_', 'TRAINER_PIC_')
    return n.replace('TRAINER_PIC_RUBY_SAPPHIRE_', 'TRAINER_PIC_RS_')


def build_map(repo, old_rev, upstream, commit):
    up_old_enum = raw_enum_names(git(upstream, 'show', commit + '^:include/constants/trainers.h'))
    up_new_enum = raw_enum_names(git(upstream, 'show', commit + ':include/constants/trainers.h'))
    hns_enum = raw_enum_names(git(repo, 'show', old_rev + ':include/constants/trainers.h'))
    og = git(upstream, 'show', commit + '^:src/data/graphics/trainers.h')
    ng = git(upstream, 'show', commit + ':src/data/graphics/trainers.h')
    old_front = {m.group(2): m.group(1) for m in re.finditer(r'TRAINER_SPRITE\((TRAINER_PIC_FRONT_\w+),\s*(\w+)', og)}
    old_back = {m.group(2): m.group(1) for m in re.finditer(r'TRAINER_BACK_SPRITE\((TRAINER_PIC_BACK_\w+),\s*\w+,\s*(\w+)', og)}
    new_front, new_back = {}, {}
    cur = None
    for line in ng.splitlines():
        m = re.match(r'\s*\[(TRAINER_PIC_\w+)\]\s*=', line)
        if m:
            cur = m.group(1)
        m = re.search(r'\.frontPic\s*=\s*TRAINER_FRONT_PIC\((\w+)', line)
        if m:
            new_front[m.group(1)] = cur
        m = re.search(r'\.backPic\s*=\s*TRAINER_BACK_PIC\(\s*\w+\s*,\s*(\w+)', line)
        if m:
            new_back[m.group(1)] = cur
    up_map = {}
    problems = []
    for sym, on in old_front.items():
        if sym not in new_front:
            problems.append('upstream front %s(%s) 새 표에 없음' % (on, sym))
            continue
        up_map[on] = (new_front[sym], 'upstream-gfx(%s)' % sym)
    for sym, on in old_back.items():
        if sym not in new_back:
            problems.append('upstream back %s(%s) 새 표에 없음' % (on, sym))
            continue
        up_map[on] = (new_back[sym], 'upstream-gfx(%s)' % sym)
    for on in up_old_enum:
        if on not in up_map:
            problems.append('upstream old %s 대응 없음' % on)
        elif name_rule(on) != up_map[on][0]:
            problems.append('upstream %s: 그림 심볼 대응 %s, 이름 규칙 %s 불일치' % (on, up_map[on][0], name_rule(on)))
    for on, (nn, _b) in up_map.items():
        if nn not in up_new_enum:
            problems.append('upstream new %s 가 새 enum 에 없음' % nn)
    rows = []
    for on in hns_enum:
        kind = 'back' if on.startswith('TRAINER_PIC_BACK_') else 'front'
        if on in up_map:
            nn, basis = up_map[on]
        elif on in HNS_BACK_MERGE:
            nn, basis = HNS_BACK_MERGE[on], 'hns-rule2-back-merge'
        elif on.startswith('TRAINER_PIC_FRONT_') and on.endswith('_HNS'):
            nn, basis = name_rule(on), 'hns-rule2'
        else:
            nn, basis = '', 'UNMAPPED'
            problems.append('HnS %s 대응 규칙 없음' % on)
        rows.append((on, kind, nn, basis))
    # 새 enum 예상 순서(공통 결정 3): upstream 새 순서 + HnS 앞모습 상대 순서
    expected = list(up_new_enum)
    for on, kind, nn, basis in rows:
        if basis.startswith('hns') and nn not in expected:
            expected.append(nn)
    return rows, problems, expected


def write_map(path, rows, problems, expected, old_rev):
    with open(path, 'w') as f:
        f.write('# 옛 enum 이름 -> 새 enum 이름 (ANALYZE 공통 결정 2)\n')
        f.write('# 생성: trainer_pic_verify.py make-map, HnS 옛 헤더 %s, upstream %s\n' % (old_rev, UPSTREAM_COMMIT))
        for p in problems:
            f.write('# PROBLEM: %s\n' % p)
        f.write('# expected_new_order: %s\n' % ' '.join(expected))
        f.write('#old_name\tkind\tnew_name\tbasis\n')
        for r in rows:
            f.write('\t'.join(r) + '\n')


def read_map(path):
    m = {}
    expected = []
    for line in open(path):
        if line.startswith('# expected_new_order:'):
            expected = line.split(':', 1)[1].split()
        if line.startswith('#') or not line.strip():
            continue
        p = line.rstrip('\n').split('\t')
        if p[2]:
            m[p[0]] = p[2]
    return m, expected


# ----------------------------------------------------------------------------
# player cases (3)
# ----------------------------------------------------------------------------
DEFAULT_PLAYER_CASES = [
    # case, old_id(이식 전 코드가 고르는 ID), new_id(이식 뒤 코드가 고르는 ID, 빈칸=pic_name_map 대응),
    # expect(same=데이터 같아야 함 / changed=의도한 변화, 차이 수에 넣지 않음), 근거
    ('PLAYER_MALE', 'TRAINER_PIC_BACK_GOLD_HNS', 'TRAINER_PIC_GOLD_HNS', 'same',
     'old TRAINER_BACK_PIC_PLAYER_MALE(IS_HNS): player.c:1899, safari.c:323, reshow_battle_screen.c:285/334 / new GetPlayerTrainerPic(MALE, GAME_VERSION) (part-A 0절)'),
    ('PLAYER_FEMALE', 'TRAINER_PIC_BACK_KRIS_HNS', 'TRAINER_PIC_KRIS_HNS', 'same',
     'old TRAINER_BACK_PIC_PLAYER_FEMALE(IS_HNS) / new GetPlayerTrainerPic(FEMALE, GAME_VERSION)'),
    ('PARTNER_SLOT_PLAYER_MALE', 'TRAINER_PIC_BACK_GOLD_HNS', 'TRAINER_PIC_GOLD_HNS', 'same',
     'old gender + TRAINER_BACK_PIC_PLAYER_MALE: player_partner.c:207, recorded_partner.c:203'),
    ('PARTNER_SLOT_PLAYER_FEMALE', 'TRAINER_PIC_BACK_KRIS_HNS', 'TRAINER_PIC_KRIS_HNS', 'same', '위와 같음(gender=1)'),
    ('LINK_ANYVER_MALE', 'TRAINER_PIC_BACK_GOLD_HNS', 'TRAINER_PIC_GOLD_HNS', 'same',
     'old LinkPlayerGetTrainerPicId #if IS_HNS gender + BACK_GOLD_HNS (버전 무시): player.c:1877, link_partner.c:150/163 / new GetPlayerTrainerPic(gender, version) HnS 분기'),
    ('LINK_ANYVER_FEMALE', 'TRAINER_PIC_BACK_KRIS_HNS', 'TRAINER_PIC_KRIS_HNS', 'same', '위와 같음(gender=1)'),
    ('RECORDED_LINK_DRAW_MALE', 'TRAINER_PIC_BACK_BRENDAN', 'TRAINER_PIC_GOLD_HNS', 'changed',
     'old gender + TRAINER_PIC_BACK_BRENDAN: recorded_player.c. 2026-10-04 친구 결정으로 Gold/Kris(upstream 1.17.0과 같은 GetPlayerTrainerPic, d78de7fdb5) → 의도한 변화'),
    ('RECORDED_LINK_DRAW_FEMALE', 'TRAINER_PIC_BACK_MAY', 'TRAINER_PIC_KRIS_HNS', 'changed',
     '위와 같음(gender=1). 2026-10-04 친구 결정으로 Kris'),
    ('RECORDED_LINK_INTRO_PAL_MALE', 'TRAINER_PIC_BACK_GOLD_HNS', 'TRAINER_PIC_GOLD_HNS', 'same',
     'old TRAINER_BACK_PIC_PLAYER_MALE 팔레트: recorded_player.c:405 (이식 전 그림=BRENDAN, 팔레트=GOLD 불일치)'),
    ('RECORDED_LINK_INTRO_PAL_FEMALE', 'TRAINER_PIC_BACK_KRIS_HNS', 'TRAINER_PIC_KRIS_HNS', 'same', '위와 같음(gender=1)'),
    ('MULTIBATTLE_TEST_PLAYER', 'TRAINER_PIC_BACK_GOLD_HNS', 'TRAINER_PIC_GOLD_HNS', 'same',
     'old TRAINER_BACK_PIC_PLAYER_MALE: player.c:1915, recorded_player.c:280 (테스트 전용, part-A 결정 M2)'),
    ('MULTIBATTLE_TEST_PARTNER', 'TRAINER_PIC_BACK_STEVEN', 'TRAINER_PIC_STEVEN', 'same', 'player_partner.c:225 (테스트 전용)'),
    ('CATCH_TUTORIAL', 'TRAINER_PIC_BACK_WALLY', 'TRAINER_PIC_WALLY', 'same',
     'CATCH_TUTORIAL_TRAINER_PIC_BACK(!IS_FRLG): reshow_battle_screen.c:27/287/345, battle_controller_wally.c:285/369'),
    ('FIRST_BATTLE_MALE_FRLG', 'TRAINER_PIC_BACK_RED', '', 'same',
     'oak_old_man.c:680/686 (IS_FRLG 전용 컨트롤러, HnS 미실행 — ID 데이터만 확인)'),
    ('FIRST_BATTLE_FEMALE_FRLG', 'TRAINER_PIC_BACK_LEAF', '', 'same', '위와 같음'),
    ('CATCH_TUTORIAL_FRLG', 'TRAINER_PIC_BACK_OLD_MAN', '', 'same', 'CATCH_TUTORIAL_TRAINER_PIC_BACK(IS_FRLG) (HnS 미실행)'),
]


def write_player_cases(path):
    with open(path, 'w') as f:
        f.write('# (3) 플레이어/고정 뒷모습 경우: 이식 전 코드가 고르던 old_id 의 뒷모습 데이터와 이식 뒤 코드가 고르는 new_id 의 뒷모습 데이터를 비교한다.\n')
        f.write('# new_id 가 비면 pic_name_map 대응을 쓴다. 메인은 A 의 GetPlayerTrainerPic(HnS 분기)과 B 호출부를 읽고 new_id 가 실제 반환값인지 확인한다.\n')
        f.write('# expect=changed 는 메인이 의도한 변화로 확정한 경우만 쓴다(차이로 세지 않고 EXPECTED 로 보고).\n')
        f.write('#case\told_id\tnew_id\texpect\twhere\n')
        for r in DEFAULT_PLAYER_CASES:
            f.write('\t'.join(r) + '\n')


def read_player_cases(path):
    out = []
    for line in open(path):
        if line.startswith('#') or not line.strip():
            continue
        p = line.rstrip('\n').split('\t')
        while len(p) < 5:
            p.append('')
        out.append(tuple(p[:5]))
    return out


# ----------------------------------------------------------------------------
# compare
# ----------------------------------------------------------------------------
GENDER_PAIRS = [('TRAINER_PIC_BRENDAN', 'TRAINER_PIC_MAY'), ('TRAINER_PIC_RED', 'TRAINER_PIC_LEAF'),
                ('TRAINER_PIC_RS_BRENDAN', 'TRAINER_PIC_RS_MAY'), ('TRAINER_PIC_GOLD_HNS', 'TRAINER_PIC_KRIS_HNS')]


def scan_gender_arith(src, enum_vals):
    """새 소스에서 'x + TRAINER_PIC_Y' / 'TRAINER_PIC_Y + x' 산술을 찾아, Y+1 이 무엇인지 보고."""
    hits = []
    v2n = {}
    for n, v in enum_vals.items():
        if not n.endswith('_COUNT'):
            v2n.setdefault(v, n)
    skip = ('TRAINER_PIC_COUNT', 'TRAINER_PIC_SIZE', 'TRAINER_PIC_WIDTH', 'TRAINER_PIC_HEIGHT')
    pat = re.compile(r'(\w+)\s*\+\s*(TRAINER_PIC_\w+)|(TRAINER_PIC_\w+)\s*\+\s*(\w+)')
    for root in ('src', 'include'):
        base = os.path.join(src, root)
        for dp, _dn, fns in os.walk(base):
            for fn in fns:
                if not fn.endswith(('.c', '.h')):
                    continue
                p = os.path.join(dp, fn)
                try:
                    text = open(p, encoding='utf-8', errors='replace').read()
                except OSError:
                    continue
                if 'TRAINER_PIC_' not in text:
                    continue
                for ln, line in enumerate(text.splitlines(), 1):
                    if 'TRAINER_PIC_' not in line or line.lstrip().startswith(('//', '*')):
                        continue
                    for m in pat.finditer(line):
                        name = m.group(2) or m.group(3)
                        if name in skip or name.startswith('TRAINER_PIC_FRONT_') or name.startswith('TRAINER_PIC_BACK_'):
                            continue
                        v = enum_vals.get(name)
                        nxt = v2n.get(v + 1, '?') if v is not None else '?'
                        hits.append((os.path.relpath(p, src), ln, name, nxt, line.strip()))
    return hits


def classify_gender_hits(hits, NE):
    """'x + TRAINER_PIC_<남자>' 에서 짝(여자)이 남자+1 이 아니면 BAD."""
    out = []
    for f, ln, name, nxt, text in hits:
        bad = any(name == m_ and (f_ not in NE or m_ not in NE or NE[f_] != NE[m_] + 1) for m_, f_ in GENDER_PAIRS)
        out.append(('BAD' if bad else 'CHECK', f, ln, name, nxt, text))
    return out


OLD_NAME_RE = re.compile(r'TRAINER_PIC_FRONT_|TRAINER_PIC_BACK_|TRAINER_BACK_PIC_PLAYER_|gTrainerSprites|'
                         r'gTrainerBacksprites|trainerBackPic|struct TrainerSprite\b|struct TrainerBacksprite\b|'
                         r'^\s*Back Pic:', re.M)


def scan_old_names(src):
    hits = []
    for root in ('src', 'include', 'test', 'tools/trainerproc'):
        base = os.path.join(src, root)
        for dp, _dn, fns in os.walk(base):
            for fn in fns:
                if not fn.endswith(('.c', '.h', '.party', '.inc', '.s')):
                    continue
                p = os.path.join(dp, fn)
                rel = os.path.relpath(p, src)
                # 생성 파일(.gitignore)은 빌드 때 다시 만들어지므로 건너뛴다.
                if rel in ('src/data/trainers.h', 'src/data/trainers_frlg.h', 'src/data/trainers_hns.h',
                           'src/data/battle_partners.h', 'src/data/debug_trainers.h',
                           'test/battle/trainer_control.h', 'test/battle/partner_control.h'):
                    continue
                try:
                    text = open(p, encoding='utf-8', errors='replace').read()
                except OSError:
                    continue
                if not OLD_NAME_RE.search(text):
                    continue
                for ln, line in enumerate(text.splitlines(), 1):
                    if OLD_NAME_RE.search(line):
                        hits.append((rel, ln, line.strip()))
    return hits


def compare(old_rows, new_rows, namemap, player_cases, identity=False, src=None, out=sys.stdout):
    def M(n):
        if identity:
            return n
        return namemap.get(n, '?UNMAPPED:' + n)

    O = {}
    N = {}
    for r in old_rows:
        O[(r[0], r[1], r[2])] = r
    for r in new_rows:
        N[(r[0], r[1], r[2])] = r
    lines = []
    total = 0
    info = 0
    P = lines.append

    def section(title, kinds_roles):
        nonlocal total, info
        cnt = ok = unused = 0
        diffs = []
        keys = sorted({k for k in list(O) + list(N) if (k[0], k[2]) in kinds_roles})
        for k in keys:
            o, n = O.get(k), N.get(k)
            cnt += 1
            if o is None or n is None:
                diffs.append('  MISSING %s %s %s: old=%s new=%s' % (k[0], k[1], k[2], 'yes' if o else 'NO', 'yes' if n else 'NO'))
                continue
            data_eq = o[4] == n[4]
            name_eq = M(o[3]) == n[3]
            if data_eq and name_eq:
                ok += 1
                continue
            # party 포인터가 NULL 인 슬롯은 게임이 쓰지 않는다(GetTrainerDifficultyLevel 등은 NORMAL 로 대체).
            if 'used=0' in o[5] and 'used=0' in n[5]:
                unused += 1
                continue
            what = []
            if not data_eq:
                what.append('DATA')
            if not name_eq:
                what.append('NAME(map(old)=%s)' % M(o[3]))
            diffs.append('  DIFF[%s] %s %s %s  ctx: %s\n      old: %s %s\n      new: %s %s' % (
                '+'.join(what), k[0], k[1], k[2], o[5].split(' syms=')[0], o[3], o[4], n[3], n[4]))
        total += len(diffs)
        info += unused
        P('%s: 비교 %d, 같음 %d, 차이 %d, 미사용 슬롯(party=NULL 양쪽) 차이 %d' % (title, cnt, ok, len(diffs), unused))
        lines.extend(diffs)

    P('== 트레이너 그림 대조 ==')
    section('[1] gTrainers 앞모습', {('TRAINER', 'FRONT')})
    section('[1] sDebugTrainers 앞모습', {('DEBUGTR', 'FRONT')})
    section('[1] gBattlePartners 앞모습', {('PARTNER', 'FRONT')})
    section('[2] gBattlePartners 뒷모습(옛 trainerBackPic <-> 새 trainerPic.backPic)', {('PARTNER', 'BACK')})

    # (3) player
    OP = {(r[1], r[2]): r for r in old_rows if r[0] == 'PIC'}
    NP = {(r[1], r[2]): r for r in new_rows if r[0] == 'PIC'}
    pd = []
    expected_changes = []
    for case, oid, nid, expect, where in player_cases:
        tgt = M(oid) if (identity or not nid) else nid
        o = OP.get((oid, 'BACK'))
        n = NP.get((tgt, 'BACK'))
        if o is None or n is None:
            pd.append('  MISSING %s: old %s(%s) new %s(%s)' % (case, oid, 'yes' if o else 'NO', tgt, 'yes' if n else 'NO'))
        elif o[4] != n[4]:
            msg = '%s: old %s -> new %s\n      old: %s\n      new: %s' % (case, oid, tgt, o[4], n[4])
            if expect == 'changed':
                expected_changes.append('  EXPECTED ' + msg)
            else:
                pd.append('  DIFF ' + msg)
        elif expect == 'changed':
            pd.append('  DIFF %s: expect=changed 인데 old %s 와 new %s 데이터가 같음' % (case, oid, tgt))
    total += len(pd)
    P('[3] 플레이어·고정 뒷모습 경우: 비교 %d, 차이 %d, 의도한 변화(expect=changed) %d' % (
        len(player_cases), len(pd), len(expected_changes)))
    lines.extend(pd)
    lines.extend(expected_changes)

    section('[4] gFacilityClassToPicIndex 경유 앞모습', {('FACILITY', 'FRONT')})

    # (5) pic table via name map
    hit = set()
    d5 = []
    cnt5 = 0
    for (on, role), o in sorted(OP.items()):
        cnt5 += 1
        tgt = M(on)
        n = NP.get((tgt, role))
        if n is None:
            d5.append('  MISSING %s %s -> %s %s (새 덤프에 없음)' % (on, role, tgt, role))
            continue
        hit.add((tgt, role))
        if o[4] != n[4]:
            d5.append('  DIFF %s %s -> %s\n      old: %s  [%s]\n      new: %s  [%s]' % (
                on, role, tgt, o[4], o[5], n[4], n[5]))
    total += len(d5)
    newonly = sorted(k for k in NP if k not in hit)
    P('[5] 옛 그림 ID -> 새 그림 ID(규칙 2) 전체: 비교 %d, 차이 %d, 새 쪽에만 있는 항목 %d' % (cnt5, len(d5), len(newonly)))
    lines.extend(d5)
    for k in newonly:
        P('  INFO new-only %s %s  %s' % (k[0], k[1], NP[k][5]))

    # (6) enum facts / gender arithmetic
    NE = {r[1]: int(r[4]) for r in new_rows if r[0] == 'ENUM'}
    P('[6] 성별 짝 인접성(gender + 남자 ID 산술이 안전한지):')
    for m_, f_ in GENDER_PAIRS:
        if identity:
            break
        if m_ in NE and f_ in NE:
            P('  %s=%d %s=%d %s' % (m_, NE[m_], f_, NE[f_], '인접' if NE[f_] == NE[m_] + 1 else '인접 아님 (gender + %s 금지)' % m_))
        else:
            P('  %s / %s: 새 enum 에 없음' % (m_, f_))
    if src and not identity:
        hits = scan_gender_arith(src, NE)
        bad = 0
        for mark, f, ln, name, nxt, text in classify_gender_hits(hits, NE):
            if mark == 'BAD':
                bad += 1
            P('  %s %s:%d  %s (+1 = %s)  | %s' % (mark, f, ln, name, nxt, text[:120]))
        total += bad
        P('  새 소스 TRAINER_PIC_* 덧셈 %d곳, 그중 HnS 짝처럼 이웃하지 않는 남자 ID 에 더한 곳(BAD) %d곳' % (len(hits), bad))
        left = scan_old_names(src)
        P('[7] 새 소스에 남은 옛 이름(주석·비활성 #if 포함, 차이 수에는 넣지 않음): %d곳' % len(left))
        for f, ln, text in left[:60]:
            P('  CHECK %s:%d  | %s' % (f, ln, text[:140]))
        if len(left) > 60:
            P('  ... %d곳 더' % (len(left) - 60))
    P('')
    P('RESULT: 차이 %d%s (미사용 슬롯 정보 %d)' % (total, ' (OK)' if total == 0 else ' (NG)', info))
    text = '\n'.join(lines) + '\n'
    out.write(text)
    return total, text


# ----------------------------------------------------------------------------
# synthetic new-layout ELF (self test)
# ----------------------------------------------------------------------------
def simulate_new_headers(old_src, dest, expected_order):
    """옛 헤더 사본에서 data.h / constants/trainers.h 만 upstream 새 구조처럼 바꾼다(자체 검증 전용)."""
    if os.path.exists(dest):
        shutil.rmtree(dest)
    shutil.copytree(old_src, dest)
    dh = os.path.join(dest, 'include', 'data.h')
    t = open(dh).read()
    new_structs = '''struct TrainerFrontPicInfo
{
    const u32 *imageData;
    const u16 *paletteData;
    const union AnimCmd *const *const animation;
    const struct Coords16 mugshotCoords;
    s16 mugshotRotation;
};

struct TrainerBackPicInfo
{
    const struct MonCoords coordinates;
    const struct SpriteFrameImage image;
    const u16 *paletteData;
    const union AnimCmd *const *const animation;
};

struct TrainerPicInfo
{
    const struct TrainerFrontPicInfo *frontPic;
    const struct TrainerBackPicInfo *backPic;
};
'''
    t2 = re.sub(r'struct TrainerSprite\n\{.*?\};\n\nstruct TrainerBacksprite\n\{.*?\};\n', new_structs, t, flags=re.S)
    t2 = t2.replace('    enum TrainerPicID trainerBackPic;\n', '')
    t2 = re.sub(r'static inline const u8 GetTrainerBackPicFromId\(u16 trainerId\)\n\{\n[^}]*\}\n\n', '', t2)
    t2 = t2.replace('extern const struct TrainerSprite gTrainerSprites[];\n',
                    'extern const struct TrainerPicInfo gTrainerPicInfo[TRAINER_PIC_COUNT];\n')
    t2 = t2.replace('extern const struct TrainerBacksprite gTrainerBacksprites[];\n', '')
    if t2 == t or 'TrainerSprite' in t2:
        die('data.h 변환 실패(자체 검증)')
    open(dh, 'w').write(t2)
    th = os.path.join(dest, 'include', 'constants', 'trainers.h')
    t = open(th).read()
    body = 'enum __attribute__((packed)) TrainerPicID\n{\n' + ''.join('    %s,\n' % n for n in expected_order) + \
        '    TRAINER_PIC_COUNT,\n};\n'
    t2 = re.sub(r'enum __attribute__\(\(packed\)\) TrainerPicID\n\{.*?\n\};\n', body, t, flags=re.S)
    t2 = re.sub(r'#define TRAINER_BACK_PIC_PLAYER_(MALE|FEMALE) [^\n]*\n', '', t2)
    open(th, 'w').write(t2)


def build_synth_elf(old_elf_path, old_lay, new_src, work, namemap, out_elf, mutate=None):
    """옛 ELF 의 그림 데이터를 새 구조(gTrainerPicInfo)로 옮겨 담은 작은 ELF 를 만든다."""
    e = Elf(old_elf_path)
    L = old_lay.L
    ts = e.sym('gTrainerSprites')
    tb = e.sym('gTrainerBacksprites')
    blobs = {}      # old addr -> c name
    src = ['#include "global.h"', '#include "data.h"', '#include "constants/trainers.h"', '']

    def cbytes(b):
        return ','.join(str(x) for x in b)

    def blob(ptr, ctype):
        if ptr == 0:
            return 'NULL'
        if ptr in blobs:
            return '(%s)%s' % (ctype, blobs[ptr])
        s = e.containing(ptr)
        n = s[0] + s[1] - ptr
        nm = 'synth_blob_%d' % len(blobs)
        data = bytearray(e.read(ptr, n))
        if mutate and mutate[0] == 'blob' and mutate[1] == ptr:
            data[0] ^= 0xff
        src.append('const u8 %s[] __attribute__((aligned(4))) = {%s};' % (nm, cbytes(data)))
        blobs[ptr] = nm
        return '(%s)%s' % (ctype, nm)

    seqs = {}
    tabs = {}

    def seq(p):
        if p in seqs:
            return seqs[p]
        words = []
        for k in range(64):
            w = e.u32(p + 4 * k)
            words.append(w)
            t = w & 0xffff
            if t in (0xffff, 0xfffe):
                break
        nm = 'synth_seq_%d' % len(seqs)
        src.append('const u32 %s[] __attribute__((aligned(4))) = {%s};' % (nm, ','.join('0x%x' % w for w in words)))
        seqs[p] = nm
        return nm

    def table(p):
        if p == 0:
            return 'NULL'
        if p in tabs:
            return tabs[p]
        s = e.containing(p)
        n = (s[0] + s[1] - p) // 4
        ents = struct.unpack('<%dI' % n, e.read(p, 4 * n))
        names = [seq(x) if x else 'NULL' for x in ents]
        nm = 'synth_tab_%d' % len(tabs)
        src.append('const void *const %s[] = {%s};' % (nm, ','.join(names)))
        tabs[p] = '(const union AnimCmd *const *)%s' % nm
        return tabs[p]

    entries = {}   # new name -> {'front': cname, 'back': cname}
    for v in range(L['TRAINER_PIC_COUNT']):
        on = old_lay.pic_name(v)
        nn = namemap[on]
        if v < L['TRAINER_PIC_FRONT_COUNT']:
            b = ts[0] + v * L['TS_SIZE']
            img = blob(e.u32(b + L['TS_IMG']), 'const u32 *')
            pal = blob(e.u32(b + L['TS_PAL']), 'const u16 *')
            an = table(e.u32(b + L['TS_ANIM']))
            mx, my, rot = e.s16(b + L['TS_MUGX']), e.s16(b + L['TS_MUGY']), e.s16(b + L['TS_ROT'])
            if mutate and mutate[0] == 'mug' and mutate[1] == on:
                mx += 1
            cn = 'synth_front_%d' % v
            src.append('const struct TrainerFrontPicInfo %s = {.imageData=%s, .paletteData=%s, .animation=%s, '
                       '.mugshotCoords={%d,%d}, .mugshotRotation=%d};' % (cn, img, pal, an, mx, my, rot))
            entries.setdefault(nn, {})['front'] = cn
        else:
            b = tb[0] + v * L['TB_SIZE']
            img = blob(e.u32(b + L['TB_IMG']), 'const void *')
            pal = blob(e.u32(b + L['TB_PAL']), 'const u16 *')
            an = table(e.u32(b + L['TB_ANIM']))
            cn = 'synth_back_%d' % v
            src.append('const struct TrainerBackPicInfo %s = {.coordinates={.size=%d,.y_offset=%d}, '
                       '.image={.data=%s,.size=%d,.relativeFrames=%d}, .paletteData=%s, .animation=%s};' % (
                           cn, e.u8(b + L['TB_CSIZE']), e.u8(b + L['TB_CYOFF']), img, e.u16(b + L['TB_FSZ']),
                           e.u8(b + L['TB_REL']), pal, an))
            entries.setdefault(nn, {})['back'] = cn
    src.append('const struct TrainerPicInfo gTrainerPicInfo[TRAINER_PIC_COUNT] = {')
    for nn, d in entries.items():
        src.append('  [%s] = {.frontPic=%s, .backPic=%s},' % (
            nn, '&' + d['front'] if 'front' in d else 'NULL', '&' + d['back'] if 'back' in d else 'NULL'))
    src.append('};')

    src.append('const u32 synth_dummy_party[4] = {0};')

    def trainers(sym, count):
        s = e.sym(sym, required=False)
        if s is None:
            return
        nd = L['DIFFICULTY_COUNT']
        if count is None:
            count = s[1] // (nd * L['T_SIZE'])
        if sym == 'sDebugTrainers':
            src.append('const struct Trainer sDebugTrainers[DIFFICULTY_COUNT][%d] = {' % count)
        else:
            src.append('const struct Trainer %s[DIFFICULTY_COUNT][%s] = {' % (
                sym, 'TRAINERS_COUNT' if sym == 'gTrainers' else 'PARTNER_COUNT'))
        for d in range(nd):
            for i in range(count):
                b = s[0] + (d * count + i) * L['T_SIZE']
                pic = e.uint(b + L['T_PIC'], L['T_PIC_SZ'])
                party = e.u8(b + L['T_PARTY'])
                used = e.u32(b + L['T_PARTYPTR']) != 0
                if not used and pic == 0:
                    continue
                nn = namemap[old_lay.pic_name(pic)]
                if mutate and mutate[0] == 'trainerpic' and mutate[1] == (sym, d, i):
                    nn = mutate[2]
                src.append('  [%d][%d] = {.trainerPic=%s, .partySize=%d, .party=%s},' % (
                    d, i, nn, party, '(const struct TrainerMon *)synth_dummy_party' if used else 'NULL'))
        src.append('};')

    trainers('gTrainers', L['TRAINERS_COUNT'])
    trainers('gBattlePartners', L['PARTNER_COUNT'])
    trainers('sDebugTrainers', None)
    s = e.sym('gFacilityClassToPicIndex')
    vals = [namemap[old_lay.pic_name(e.u16(s[0] + 2 * i))] for i in range(s[1] // 2)]
    src.append('const u16 gFacilityClassToPicIndex[] = {%s};' % ','.join(vals))
    src.append('void synth_entry(void) {}')
    c = os.path.join(work, 'synth_new.c')
    o = os.path.join(work, 'synth_new.o')
    open(c, 'w').write('\n'.join(src) + '\n')
    cmd = [GCC, '-c'] + _flags(new_src) + CFLAGS + ['-Wno-int-conversion', '-o', o, c]
    r = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if r.returncode != 0:
        die('synth compile failed:\n' + r.stderr.decode(errors='replace')[-3000:])
    cmd = [GCC, '-nostdlib', '-mthumb', '-mabi=apcs-gnu', '-march=armv4t', '-Wl,-Ttext=0x08000000',
           '-Wl,-e,synth_entry', '-Wl,--no-gc-sections', '-o', out_elf, o]
    r = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if r.returncode != 0:
        die('synth link failed:\n' + r.stderr.decode(errors='replace')[-3000:])
    return out_elf


# ----------------------------------------------------------------------------
# commands
# ----------------------------------------------------------------------------
def parse_gfx_old(text):
    """옛 src/data/graphics/trainers.h: {ID: ('front'|'back', args tuple)} + {symbol: incbin path}."""
    ent = {}
    for m in re.finditer(r'^\s*TRAINER_SPRITE\((TRAINER_PIC_\w+),\s*([^)]*)\)', text, re.M):
        a = [x.strip() for x in m.group(2).split(',')]
        a += ['0', '0', '0x200'][len(a) - 2:] if len(a) < 5 else []
        ent[(m.group(1), 'FRONT')] = tuple(a)
    for m in re.finditer(r'^\s*TRAINER_BACK_SPRITE\((TRAINER_PIC_\w+),\s*([^)]*)\)', text, re.M):
        ent[(m.group(1), 'BACK')] = tuple(x.strip() for x in m.group(2).split(','))
    return ent


def parse_gfx_new(text):
    ent = {}
    cur = None
    for line in text.splitlines():
        m = re.match(r'\s*\[(TRAINER_PIC_\w+)\]\s*=', line)
        if m:
            cur = m.group(1)
        m = re.search(r'\.frontPic\s*=\s*TRAINER_FRONT_PIC\(([^)]*)\)', line)
        if m:
            a = [x.strip() for x in m.group(1).split(',')]
            a += ['0', '0', '0x200'][len(a) - 2:] if len(a) < 5 else []
            ent[(cur, 'FRONT')] = tuple(a)
        m = re.search(r'\.backPic\s*=\s*TRAINER_BACK_PIC\(([^)]*)\)', line)
        if m:
            ent[(cur, 'BACK')] = tuple(x.strip() for x in m.group(1).split(','))
    return ent


def parse_incbin(text):
    return {m.group(1): m.group(2) for m in re.finditer(r'(\w+)\[\]\s*=\s*INCBIN_U\d+\("([^"]+)"\)', text)}


def parse_lookup(text):
    m = re.search(r'gFacilityClassToPicIndex\[\]\s*=\s*\{(.*?)\};', text, re.S)
    return dict(re.findall(r'\[(FACILITY_CLASS_\w+)\]\s*=\s*(TRAINER_PIC_\w+)', m.group(1))) if m else {}


def cmd_src_check(args):
    """빌드 전 소스 대조(참고용): 옛 그림 표/시설 표의 인자(심볼·좌표·애니메이션)가 새 표의 대응 ID 에 그대로 있는지."""
    ensure_map(args)
    namemap, _ = read_map(args.map)
    og = git(args.repo, 'show', args.old_rev + ':src/data/graphics/trainers.h')
    ng = open(os.path.join(args.new_src, 'src/data/graphics/trainers.h')).read()
    oe, ne = parse_gfx_old(og), parse_gfx_new(ng)
    oi, ni = parse_incbin(og), parse_incbin(ng)
    bad = 0
    for (oid, role), oargs in sorted(oe.items()):
        nid = namemap.get(oid)
        nargs = ne.get((nid, role))
        if nargs is None:
            print('MISSING %s %s -> %s' % (oid, role, nid))
            bad += 1
            continue
        # 옛 FRONT 인자: pic, pal, x, y, rot / 옛 BACK 인자: yoff, sprite, pal, anim (새 쪽도 같은 순서)
        if [x.replace(' ', '') for x in oargs] != [x.replace(' ', '') for x in nargs]:
            print('ARGS %s %s -> %s\n   old %s\n   new %s' % (oid, role, nid, oargs, nargs))
            bad += 1
        for sym in oargs:
            if sym in oi and ni.get(sym) != oi[sym]:
                print('INCBIN %s: old %s new %s' % (sym, oi[sym], ni.get(sym)))
                bad += 1
    hit = {(namemap.get(o), r) for (o, r) in oe}
    for k in sorted(ne):
        if k not in hit:
            print('INFO new-only %s %s %s' % (k[0], k[1], ne[k]))
    ol = parse_lookup(git(args.repo, 'show', args.old_rev + ':src/data/pokemon/trainer_class_lookups.h'))
    nl = parse_lookup(open(os.path.join(args.new_src, 'src/data/pokemon/trainer_class_lookups.h')).read())
    for fc, op in sorted(ol.items()):
        if nl.get(fc) != namemap.get(op):
            print('LOOKUP %s: old %s -> expected %s, new %s' % (fc, op, namemap.get(op), nl.get(fc)))
            bad += 1
    if set(nl) - set(ol):
        print('LOOKUP new-only classes: %s' % sorted(set(nl) - set(ol)))
    print('src-check: 그림 표 %d 항목, 시설 표 %d 항목, 문제 %d' % (len(oe), len(ol), bad))
    sys.exit(0 if bad == 0 else 1)


def old_rev_default():
    try:
        return open(BASE_HEAD).read().strip()
    except OSError:
        return 'HEAD'


def ensure_map(args):
    if not os.path.exists(args.map):
        rows, problems, expected = build_map(args.repo, args.old_rev, UPSTREAM, UPSTREAM_COMMIT)
        write_map(args.map, rows, problems, expected, args.old_rev)
    if not os.path.exists(args.player):
        write_player_cases(args.player)


def cmd_make_map(args):
    rows, problems, expected = build_map(args.repo, args.old_rev, UPSTREAM, UPSTREAM_COMMIT)
    write_map(args.map, rows, problems, expected, args.old_rev)
    if not os.path.exists(args.player):
        write_player_cases(args.player)
    print('map: %s (%d 항목, 문제 %d)' % (args.map, len(rows), len(problems)))
    for p in problems:
        print('  PROBLEM', p)


def cmd_dump(args):
    if args.src:
        src, desc = args.src, 'dir:' + args.src
    else:
        src, desc = materialize_rev(args.repo, args.rev, args.work), 'git:' + git(args.repo, 'rev-parse', args.rev).strip()
    rows = do_dump(os.path.abspath(args.elf), args.layout, src, args.work, args.out, desc)
    print('dump: %s (%d rows)' % (args.out, len(rows)))


def cmd_compare(args):
    ensure_map(args)
    old_rows, _ = read_dump(args.old)
    new_rows, _ = read_dump(args.new)
    namemap, _ = read_map(args.map)
    total, _ = compare(old_rows, new_rows, namemap, read_player_cases(args.player), identity=args.identity,
                       src=args.new_src)
    sys.exit(0 if total == 0 else 1)


def cmd_run(args):
    ensure_map(args)
    os.makedirs(args.work, exist_ok=True)
    old_src = materialize_rev(args.repo, args.old_rev, args.work)
    full = git(args.repo, 'rev-parse', args.old_rev).strip()
    regen = True
    if os.path.exists(args.old_dump):
        _r, meta = read_dump(args.old_dump)
        if meta.get('elf_sha1') == elf_digest(args.old_elf) and meta.get('src') == 'git:' + full:
            regen = False
    if regen:
        do_dump(args.old_elf, 'old', old_src, args.work, args.old_dump, 'git:' + full)
    new_dump = args.new_dump
    do_dump(args.new_elf, 'new', args.new_src, args.work, new_dump, 'dir:' + args.new_src)
    old_rows, _ = read_dump(args.old_dump)
    new_rows, _ = read_dump(new_dump)
    namemap, _ = read_map(args.map)
    total, text = compare(old_rows, new_rows, namemap, read_player_cases(args.player), src=args.new_src)
    with open(args.report, 'w') as f:
        f.write(text)
    sys.stderr.write('report: %s\nold dump: %s\nnew dump: %s\n' % (args.report, args.old_dump, new_dump))
    sys.exit(0 if total == 0 else 1)


def cmd_selftest(args):
    ensure_map(args)
    work = args.work
    st = os.path.join(work, 'selftest')
    os.makedirs(st, exist_ok=True)
    full = git(args.repo, 'rev-parse', args.old_rev).strip()
    old_src = materialize_rev(args.repo, args.old_rev, work)
    results = []

    def check(name, cond, detail=''):
        results.append((name, cond))
        print('%s %s %s' % ('PASS' if cond else 'FAIL', name, detail))

    # T1: 같은 ELF 를 두 번 덤프(작업 폴더도 따로) -> 바이트 동일, 비교 차이 0
    a = os.path.join(st, 'old_A.tsv')
    b = os.path.join(st, 'old_B.tsv')
    do_dump(args.old_elf, 'old', old_src, os.path.join(st, 'wA'), a, 'git:' + full)
    do_dump(args.old_elf, 'old', old_src, os.path.join(st, 'wB'), b, 'git:' + full)
    check('T1a old dump x2 byte-identical', open(a, 'rb').read() == open(b, 'rb').read())
    ra, _ = read_dump(a)
    rb, _ = read_dump(b)
    cases = read_player_cases(args.player)
    sink = io.StringIO()
    tot, _ = compare(ra, rb, {}, cases, identity=True, out=sink)
    check('T1b old vs old (identity map) diff 0', tot == 0, '(diff=%d)' % tot)

    # T2: 변조 덤프 검출
    def mutate(rows, pred, fn):
        out, done = [], False
        for r in rows:
            if not done and pred(r):
                done = True
                nr = fn(r)
                if nr is not None:
                    out.append(nr)
                continue
            out.append(r)
        if not done:
            die('mutation target not found')
        return out

    def flip_hash(v, field):
        return re.sub(field + r'=(\d+):([0-9a-f])', lambda m: '%s=%s:%s' % (field, m.group(1), 'f' if m.group(2) != 'f' else '0'), v, count=1)

    muts = [
        ('T2a gTrainers 한 명 앞모습 그림 해시 변조 -> [1] 1건',
         lambda r: r[0] == 'TRAINER' and 'used=0' not in r[5],
         lambda r: r[:4] + (flip_hash(r[4], 'img'),) + r[5:], 1, '[1] gTrainers'),
        ('T2b 파트너 뒷모습 애니메이션 변조 -> [2] 1건',
         lambda r: r[0] == 'PARTNER' and r[2] == 'BACK' and 'LANCE' in r[3],
         lambda r: r[:4] + (r[4].replace('F0:24', 'F0:25', 1),) + r[5:], 1, '[2]'),
        ('T2c BACK_GOLD_HNS 팔레트 변조 -> [5] 1건 + [3] GOLD 를 쓰는 경우 수만큼',
         lambda r: r[0] == 'PIC' and r[1] == 'TRAINER_PIC_BACK_GOLD_HNS',
         lambda r: r[:4] + (flip_hash(r[4], 'pal'),) + r[5:],
         1 + sum(1 for c in cases if c[1] == 'TRAINER_PIC_BACK_GOLD_HNS'), 'EXACT'),
        ('T2d 시설 표 한 칸 이름만 변경(데이터 같음) -> [4] 1건',
         lambda r: r[0] == 'FACILITY' and r[1] == '000',
         lambda r: (r[0], r[1], r[2], 'TRAINER_PIC_FRONT_SOMETHING_ELSE', r[4], r[5]), 1, '[4]'),
        ('T2e gTrainers 행 삭제 -> MISSING 1건',
         lambda r: r[0] == 'TRAINER' and 'used=0' not in r[5],
         lambda r: None, 1, '[1] gTrainers'),
        ('T2f 머그샷 좌표 변조(FRONT_STEVEN) -> [5] 1건',
         lambda r: r[0] == 'PIC' and r[1] == 'TRAINER_PIC_FRONT_STEVEN',
         lambda r: r[:4] + (r[4].replace('mug=0,7', 'mug=1,7'),) + r[5:], 1, None),
    ]
    for name, pred, fn, expect_min, sect in muts:
        rb2 = mutate(rb, pred, fn)
        sink = io.StringIO()
        tot, text = compare(ra, rb2, {}, cases, identity=True, out=sink)
        ok = tot >= expect_min and tot > 0
        if sect == 'EXACT':
            ok = tot == expect_min
        elif sect:
            line = [l for l in text.splitlines() if l.startswith(sect)][0]
            ok = ok and (' 차이 1,' in line or ' 차이 1 ' in line or line.rstrip().endswith('차이 1'))
        check(name, ok, '(total=%d)' % tot)

    # T3: 새 구조 합성 ELF(옛 데이터를 gTrainerPicInfo 형식으로) -> 규칙 2 대응으로 차이 0, 변조 검출
    if not args.no_synth:
        namemap, expected = read_map(args.map)
        sim = os.path.join(st, 'sim-new-src')
        simulate_new_headers(old_src, sim, expected)
        old_lay = Layout(old_src, os.path.join(st, 'wA'), 'old')
        selfe = os.path.join(st, 'synth_new.elf')
        build_synth_elf(args.old_elf, old_lay, sim, st, namemap, selfe)
        nd = os.path.join(st, 'synth_new_dump.tsv')
        do_dump(selfe, 'new', sim, os.path.join(st, 'wN'), nd, 'dir:' + sim)
        rn, _ = read_dump(nd)
        sink = io.StringIO()
        tot, text = compare(ra, rn, namemap, cases, src=None, out=sink)
        open(os.path.join(st, 'synth_report.txt'), 'w').write(text)
        unused = re.search(r'미사용 슬롯 정보 (\d+)', text).group(1)
        check('T3a synth new-layout ELF vs old dump (규칙 2 대응) diff 0', tot == 0,
              '(diff=%d, 미사용 슬롯 %s, report %s)' % (tot, unused, os.path.join(st, 'synth_report.txt')))
        # T3b: 합성 ELF 에서 KRIS_HNS 뒷모습 그림 바이트 하나를 바꾸면 [3][5] 에서 잡혀야 한다
        e = Elf(args.old_elf)
        L = old_lay.L
        kris = dict(old_lay.pic_values)['TRAINER_PIC_BACK_KRIS_HNS']
        ptr = e.u32(e.sym('gTrainerBacksprites')[0] + kris * L['TB_SIZE'] + L['TB_IMG'])
        selfe2 = os.path.join(st, 'synth_new_mut.elf')
        build_synth_elf(args.old_elf, old_lay, sim, st, namemap, selfe2, mutate=('blob', ptr))
        nd2 = os.path.join(st, 'synth_new_mut_dump.tsv')
        do_dump(selfe2, 'new', sim, os.path.join(st, 'wN'), nd2, 'dir:' + sim)
        rn2, _ = read_dump(nd2)
        tot2, text2 = compare(ra, rn2, namemap, cases, out=io.StringIO())
        check('T3b synth ELF KRIS_HNS 뒷모습 1바이트 변조 검출', tot2 > 0 and 'DIFF TRAINER_PIC_BACK_KRIS_HNS BACK' in text2,
              '(total=%d)' % tot2)
        # T3c: 합성 ELF 에서 gTrainers 한 명의 trainerPic 을 다른 그림으로 바꾸면 [1] 에서 DATA+NAME
        tgt = None
        for r in ra:
            if r[0] == 'TRAINER' and r[3].endswith('_HNS') and r[3] != 'TRAINER_PIC_FRONT_KRIS_HNS' and 'used=0' not in r[5]:
                tgt = r
                break
        if tgt:
            dname, idx = tgt[1].split('/')
            dval = [k for k, v in old_lay.diff_names.items() if v == dname][0]
            selfe3 = os.path.join(st, 'synth_new_mut3.elf')
            build_synth_elf(args.old_elf, old_lay, sim, st, namemap, selfe3,
                            mutate=('trainerpic', ('gTrainers', dval, int(idx)), 'TRAINER_PIC_KRIS_HNS'))
            nd3 = os.path.join(st, 'synth_new_mut3_dump.tsv')
            do_dump(selfe3, 'new', sim, os.path.join(st, 'wN'), nd3, 'dir:' + sim)
            rn3, _ = read_dump(nd3)
            tot3, text3 = compare(ra, rn3, namemap, cases, out=io.StringIO())
            check('T3c synth ELF 트레이너 그림 ID 교체 검출(DATA+NAME)', tot3 == 1 and 'DIFF[DATA+NAME' in text3,
                  '(total=%d, %s %s -> TRAINER_PIC_KRIS_HNS)' % (tot3, tgt[1], tgt[3]))
        else:
            check('T3c target trainer found', False)
        # T4: expect=changed 처리 — RECORDED_LINK_DRAW 를 GOLD/KRIS 로 바꾸고 changed 로 두면 차이 0, EXPECTED 2
        cases2 = [(c[0], c[1], {'RECORDED_LINK_DRAW_MALE': 'TRAINER_PIC_GOLD_HNS',
                                 'RECORDED_LINK_DRAW_FEMALE': 'TRAINER_PIC_KRIS_HNS'}.get(c[0], c[2]),
                   'changed' if c[0].startswith('RECORDED_LINK_DRAW') else c[3], c[4]) for c in cases]
        tot4, text4 = compare(ra, rn, namemap, cases2, out=io.StringIO())
        check('T4a player expect=changed -> 차이 0, EXPECTED 2', tot4 == 0 and text4.count('  EXPECTED ') == 2,
              '(total=%d)' % tot4)
        cases3 = [(c[0], c[1], c[2], 'changed' if c[0] == 'PLAYER_MALE' else c[3], c[4]) for c in cases]
        tot5, _ = compare(ra, rn, namemap, cases3, out=io.StringIO())
        check('T4b expect=changed 인데 데이터가 같으면 차이 1', tot5 == 1, '(total=%d)' % tot5)
        # T5: 소스 산술 검사 — gender + GOLD_HNS 는 BAD, gender + BRENDAN 은 CHECK(인접), 옛 이름 잔존 검출
        ft = os.path.join(st, 'scan-tree')
        if os.path.exists(ft):
            shutil.rmtree(ft)
        os.makedirs(os.path.join(ft, 'src'))
        open(os.path.join(ft, 'src', 'x.c'), 'w').write(
            'u32 a = gender + TRAINER_PIC_GOLD_HNS;\nu32 b = TRAINER_PIC_BRENDAN + gender;\n'
            'u32 c = TRAINER_PIC_COUNT + id;\nu32 d = TRAINER_PIC_BACK_GOLD_HNS;\n')
        NE = {r[1]: int(r[4]) for r in rn if r[0] == 'ENUM'}
        cl = classify_gender_hits(scan_gender_arith(ft, NE), NE)
        marks = sorted((m, n) for m, _f, _l, n, _x, _t in cl)
        check('T5a 산술 검사 BAD/CHECK 분류', marks == [('BAD', 'TRAINER_PIC_GOLD_HNS'), ('CHECK', 'TRAINER_PIC_BRENDAN')],
              str(marks))
        check('T5b 옛 이름 잔존 검출', len(scan_old_names(ft)) == 1)
    nfail = sum(1 for _, c in results if not c)
    print('SELFTEST: %d/%d PASS' % (len(results) - nfail, len(results)))
    sys.exit(0 if nfail == 0 else 1)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--repo', default=REPO)
    ap.add_argument('--old-rev', default=old_rev_default(), help='옛 헤더 리비전(기본: base/HEAD.txt)')
    ap.add_argument('--work', default=WORK, help='컴파일·임시 파일 위치(저장소 밖)')
    ap.add_argument('--map', default=MAP_FILE)
    ap.add_argument('--player', default=PLAYER_FILE)
    sp = ap.add_subparsers(dest='cmd', required=True)

    p = sp.add_parser('run', help='옛/새 덤프 + 비교 (메인용)')
    p.add_argument('--old-elf', default=BASE_ELF)
    p.add_argument('--old-dump', default=OLD_DUMP)
    p.add_argument('--new-elf', default=os.path.join(REPO, 'pokehns.elf'))
    p.add_argument('--new-src', default=REPO, help='새 헤더가 있는 트리(include/), 기본: 저장소 작업 트리')
    p.add_argument('--new-dump', default=os.path.join(VERIFY, 'new_dump.tsv'))
    p.add_argument('--report', default=os.path.join(VERIFY, 'report.txt'))
    p.set_defaults(func=cmd_run)

    p = sp.add_parser('dump')
    p.add_argument('--elf', required=True)
    p.add_argument('--layout', choices=['old', 'new'], required=True)
    p.add_argument('--src', help='include/ 를 가진 디렉터리(없으면 --rev 를 git archive)')
    p.add_argument('--rev', default=old_rev_default())
    p.add_argument('-o', '--out', required=True)
    p.set_defaults(func=cmd_dump)

    p = sp.add_parser('compare')
    p.add_argument('old')
    p.add_argument('new')
    p.add_argument('--identity', action='store_true', help='이름 대응 없이 같은 이름끼리(옛 vs 옛 자체 검증)')
    p.add_argument('--new-src', default=None, help='주면 새 소스의 TRAINER_PIC 덧셈 산술을 검사')
    p.set_defaults(func=cmd_compare)

    p = sp.add_parser('src-check', help='빌드 전 소스 대조(참고용)')
    p.add_argument('--new-src', default=REPO)
    p.set_defaults(func=cmd_src_check)

    p = sp.add_parser('make-map')
    p.set_defaults(func=cmd_make_map)

    p = sp.add_parser('selftest')
    p.add_argument('--old-elf', default=BASE_ELF)
    p.add_argument('--no-synth', action='store_true')
    p.set_defaults(func=cmd_selftest)

    args = ap.parse_args()
    args.func(args)


if __name__ == '__main__':
    main()
