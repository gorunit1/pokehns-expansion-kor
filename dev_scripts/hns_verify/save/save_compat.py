#!/usr/bin/env python3
"""seq 138.5 #8943(12v12) 이식 전후 '일반 게임 세이브 호환' 기계 확인 도구 (영역 D).

A안 확정 결정: RecordedBattleSave(녹화 배틀 기록)만 바뀌어도 되고, 일반 세이브(SaveBlock1/2/3·PokemonStorage)는
바이트 배치까지 이식 전과 같아야 한다. 이 도구는 이식 전/후 각각에서 아래 '사실'을 모아(collect) 비교(compare)한다.

  (1) 구조체 레이아웃   HnS 헤더를 Makefile과 같은 ARM 플래그(+ -g)로 컴파일한 작은 TU의 DWARF에서
                        SaveBlock1/2/3·PokemonStorage·Pokemon·BoxPokemon·SaveSector·ASLR 래퍼 등을 평탄화(모든 잎 필드의
                        비트 오프셋·비트 폭)하고, 세이브 상수(SECTOR_*, PARTY_SIZE, TOTAL_BOXES_COUNT …)를 뽑는다.
                        RecordedBattleSave는 A안 허용 대상이라 '정보'로만 보이고, 섹터 한도·checksum 위치만 검사한다.
                        .c 안에 정의된 특수 섹터 구조체(명예의 전당 HallofFameMon/Team)는 본문 텍스트를 비교한다.
  (2) 섹터 배치         링크된 ELF의 sSaveSlotLayout[14](섹터별 offset/size)와 sGFRomHeader(외부 도구용 세이브 오프셋·크기)
  (3) 세이브 경로 코드  save.o·load_save.o 전체 함수 + 파티/박스 세이브 접근 함수(GetSavedPlayerPartyMon 등)를 objdump로
                        역어셈블해 주소·분기 대상·리터럴 풀을 심볼로 바꾼 '정규화 코드'를 비교한다. gParties[0]/[1],
                        gPartiesCount[0]/[1]은 이식 전 이름 gPlayerParty/gEnemyParty/…Count로 바꿔(별칭) 비교한다.
                        → 같으면 "이식 뒤에도 같은 RAM 바이트를 같은 세이브 자리로 복사한다"는 기계 증명이 된다.
  (4) RAM 정적 변수     ELF 심볼표(OBJECT)와 .map(입력 섹션→오브젝트 파일)로 EWRAM/IWRAM 변수별 크기·합계를 비교한다.

부속 명령(dev_scripts/hns_verify/README.md 참고)
  collect   한쪽 사실만 모은다. 이식 전(묶음 직전 깨끗한 HEAD를 make hns -j8 한 뒤) 같은 기계에서 만든다.
            기계어·RAM 사실은 툴체인에 따라 달라질 수 있어 다른 기계의 사실 폴더를 기준으로 쓰지 않는다.
  run       --pre <collect 폴더>(필수)와 이식 후 사실(--post-src 작업 트리 헤더 + --post-elf/--post-map)을 비교한다.
  compare   사실 디렉터리 둘을 비교한다.
  (옛 기본 run의 verify/pre·base/ 자동 생성, selftest는 저장소판에 없다 — 2단계.)

저장소는 읽기만 한다(헤더를 -iquote로 읽고 git archive/rev-parse/status만 쓴다). 산출물은 --out
(run 기본: $HNS_VERIFY_OUT/save/run-<시각>)에만 쓴다. 경로 규칙은 ../hnsverify_paths.py(HNS_REPO 등).
필요: arm-none-eabi-gcc/objdump/readelf(PATH 또는 $DEVKITARM/bin), python3. 종료 코드 0=PASS(정보·주의만 있음),
1=FAIL, 2=도구 오류.
"""
import argparse
import bisect
import datetime
import difflib
import hashlib
import io
import os
import re
import shutil
import struct
import subprocess
import sys
import tarfile

sys.dont_write_bytecode = True   # 저장소 안에 __pycache__를 남기지 않는다
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
sys.path.insert(0, os.path.dirname(HERE))
import hnsverify_paths  # noqa: E402  (DEVKITARM → PATH도 여기서)
import dwarf_layout  # noqa: E402
REPO = hnsverify_paths.REPO                       # 기본 HNS_REPO = 이 도구가 든 저장소
TMP = os.path.join(hnsverify_paths.OUT, 'save')   # 기본 $HNS_VERIFY_OUT/save

GCC = os.environ.get('ARM_GCC', 'arm-none-eabi-gcc')
OBJDUMP = os.environ.get('ARM_OBJDUMP', 'arm-none-eabi-objdump')
# Makefile(HnS) CPPFLAGS/CFLAGS 중 레이아웃에 영향을 주는 것 (+ -g). Makefile:168, :179
CPPFLAGS = ['-Wno-trigraphs', '-DMODERN=1', '-DTESTING=0', '-DPOKEMON_HNS', '-std=gnu17']
CFLAGS = ['-mthumb', '-mthumb-interwork', '-O2', '-mabi=apcs-gnu', '-mtune=arm7tdmi', '-march=armv4t', '-g']

FACTS_VERSION = 1

# ---------------------------------------------------------------------------------------------
# (1) 구조체·상수
# ---------------------------------------------------------------------------------------------
# 일반 세이브: 하나라도 다르면 FAIL
STRICT_STRUCTS = [
    'SaveBlock1', 'SaveBlock2', 'SaveBlock3', 'PokemonStorage',          # 세이브 이미지 전체(평탄화)
    'SaveBlock1ASLR', 'SaveBlock2ASLR', 'PokemonStorageASLR',            # EWRAM 래퍼(ASLR 이동 범위)
    'Pokemon', 'BoxPokemon', 'SaveSector', 'TrainerHillChallenge',       # 파티/박스 원소, 섹터, 특수 섹터 30
    'HallofFameMon', 'HallofFameTeam',                                   # 특수 섹터 28·29(명예의 전당)
    # 아래는 위 SaveBlock 평탄화에 이미 들어 있다. 차이를 읽기 쉽게 따로 한 번 더 보인다.
    'BattleFrontier', 'ChallengeSettings', 'LinkBattleRecords', 'PlayersApprentice',
]
# A안 허용: 바뀌어도 되지만 섹터 한도와 checksum 위치를 검사한다
ALLOWED_STRUCTS = ['RecordedBattleSave']

PROBE_HEADERS = ['global.h', 'pokemon.h', 'pokemon_storage_system.h', 'recorded_battle.h', 'save.h',
                 'load_save.h', 'trainer_hill.h', 'hall_of_fame.h', 'constants/battle.h']

# (이름, 식, 엄격?) — 엄격이면 값이 달라지면 FAIL. 매크로가 없으면 MISSING으로 기록한다.
CONSTS = [
    ('SECTOR_DATA_SIZE', 'SECTOR_DATA_SIZE', True),
    ('SAVE_BLOCK_3_CHUNK_SIZE', 'SAVE_BLOCK_3_CHUNK_SIZE', True),
    ('SECTOR_FOOTER_SIZE', 'SECTOR_FOOTER_SIZE', True),
    ('SECTOR_SIZE', 'SECTOR_SIZE', True),
    ('SECTOR_SIGNATURE', 'SECTOR_SIGNATURE', True),
    ('SECTOR_SIGNATURE_OFFSET', 'SECTOR_SIGNATURE_OFFSET', True),
    ('SECTOR_COUNTER_OFFSET', 'SECTOR_COUNTER_OFFSET', True),
    ('SPECIAL_SECTOR_SENTINEL', 'SPECIAL_SECTOR_SENTINEL', True),
    ('SECTOR_ID_SAVEBLOCK2', 'SECTOR_ID_SAVEBLOCK2', True),
    ('SECTOR_ID_SAVEBLOCK1_START', 'SECTOR_ID_SAVEBLOCK1_START', True),
    ('SECTOR_ID_SAVEBLOCK1_END', 'SECTOR_ID_SAVEBLOCK1_END', True),
    ('SECTOR_ID_PKMN_STORAGE_START', 'SECTOR_ID_PKMN_STORAGE_START', True),
    ('SECTOR_ID_PKMN_STORAGE_END', 'SECTOR_ID_PKMN_STORAGE_END', True),
    ('NUM_SECTORS_PER_SLOT', 'NUM_SECTORS_PER_SLOT', True),
    ('SECTOR_ID_HOF_1', 'SECTOR_ID_HOF_1', True),
    ('SECTOR_ID_HOF_2', 'SECTOR_ID_HOF_2', True),
    ('SECTOR_ID_TRAINER_HILL', 'SECTOR_ID_TRAINER_HILL', True),
    ('SECTOR_ID_RECORDED_BATTLE', 'SECTOR_ID_RECORDED_BATTLE', True),
    ('SECTORS_COUNT', 'SECTORS_COUNT', True),
    ('SAVE_VERSION_MAGIC', 'SAVE_VERSION_MAGIC', True),
    ('SAVEBLOCK_MOVE_RANGE', 'SAVEBLOCK_MOVE_RANGE', True),
    ('PARTY_SIZE', 'PARTY_SIZE', True),
    ('TOTAL_BOXES_COUNT', 'TOTAL_BOXES_COUNT', True),
    ('IN_BOX_COUNT', 'IN_BOX_COUNT', True),
    ('BOX_NAME_LENGTH', 'BOX_NAME_LENGTH', True),
    ('PLAYER_NAME_LENGTH', 'PLAYER_NAME_LENGTH', True),
    ('POKEMON_NAME_LENGTH', 'POKEMON_NAME_LENGTH', True),
    ('MAX_MON_MOVES', 'MAX_MON_MOVES', True),
    ('sizeof_Pokemon_x_PARTY', 'sizeof(struct Pokemon) * PARTY_SIZE', True),
    # 정보(바뀌어도 FAIL 아님)
    ('BATTLER_RECORD_SIZE', 'BATTLER_RECORD_SIZE', False),
    ('MAX_LINK_PLAYERS', 'MAX_LINK_PLAYERS', False),
]
ENUM_CONSTS = [('NUM_STATS', 'NUM_STATS', True),                     # enum이라 #ifdef 불가
               ('MAX_BATTLERS_COUNT', 'MAX_BATTLERS_COUNT', False),
               ('MAX_BATTLE_TRAINERS', 'MAX_BATTLE_TRAINERS', False)]

# .c 파일 안에 정의돼 헤더 TU로는 못 보는 세이브 구조체 — 본문 텍스트 비교(현재 없음: 명예의 전당은 hall_of_fame.h)
SRC_STRUCTS = []
# 세이브 경로 소스 조각(텍스트, 정보용): save.c 섹터 표
SRC_SNIPPETS = [('src/save.c', r'#define SAVEBLOCK_CHUNK\(structure, chunkNum\).*?^};'),
                ('src/hall_of_fame.c', r'^#define HALL_OF_FAME_MAX_TEAMS .*?$')]


def die(msg):
    sys.stderr.write('ERROR: %s\n' % msg)
    sys.exit(2)


def sh(cmd, **kw):
    return subprocess.run(cmd, check=True, capture_output=True, text=True, errors='replace', **kw).stdout


def sha1_file(p):
    h = hashlib.sha1()
    with open(p, 'rb') as f:
        for b in iter(lambda: f.read(1 << 20), b''):
            h.update(b)
    return h.hexdigest()


# ---------------------------------------------------------------------------------------------
# 소스 트리 준비: 작업 트리 그대로 또는 git rev를 스크래치에 풀기(+생성 헤더 복사)
# ---------------------------------------------------------------------------------------------
GENERATED_INCLUDES_CACHE = None


def generated_includes(repo):
    """저장소에서 git이 무시하는 생성 헤더(include/constants/map_groups.h 등) 목록."""
    global GENERATED_INCLUDES_CACHE
    if GENERATED_INCLUDES_CACHE is None:
        out = sh(['git', '-C', repo, 'ls-files', '--others', '--ignored', '--exclude-standard', 'include'])
        GENERATED_INCLUDES_CACHE = [l for l in out.splitlines() if l.endswith('.h')]
    return GENERATED_INCLUDES_CACHE


def materialize_rev(repo, rev, dest):
    """git archive <rev> include src/hall_of_fame.c src/save.c 를 dest에 풀고, 생성 헤더는 저장소 작업 트리에서 복사."""
    if os.path.exists(dest):
        shutil.rmtree(dest)
    os.makedirs(dest)
    blob = subprocess.run(['git', '-C', repo, 'archive', '--format=tar', rev, 'include',
                           'src/hall_of_fame.c', 'src/save.c'],
                          check=True, capture_output=True).stdout
    with tarfile.open(fileobj=io.BytesIO(blob)) as t:
        t.extractall(dest)
    for g in generated_includes(repo):
        dst = os.path.join(dest, g)
        if not os.path.exists(dst):
            os.makedirs(os.path.dirname(dst), exist_ok=True)
            shutil.copy2(os.path.join(repo, g), dst)
    with open(os.path.join(dest, 'MATERIALIZED_FROM'), 'w') as f:
        f.write('%s %s\n' % (repo, sh(['git', '-C', repo, 'rev-parse', rev]).strip()))
    return dest


# ---------------------------------------------------------------------------------------------
# (1) 헤더 TU 컴파일 → DWARF 평탄화 + 상수
# ---------------------------------------------------------------------------------------------
def probe_source():
    lines = ['/* save_compat.py probe TU (자동 생성) */']
    for h in PROBE_HEADERS:
        lines.append('#include "%s"' % h)
    for s in STRICT_STRUCTS + ALLOWED_STRUCTS:
        lines.append('struct %s gL_%s;' % (s, s))
    lines.append('#define SCX_MISSING 0xDEADBEEFu')
    lines.append('__attribute__((used)) const unsigned int gSaveCompatConsts[] = {')
    for name, expr, _strict in CONSTS:
        macro = expr.split()[0] if not expr.startswith('sizeof') else None
        if macro:
            lines.append('#ifdef %s\n    (unsigned int)(%s),\n#else\n    SCX_MISSING,\n#endif' % (macro, expr))
        else:
            lines.append('    (unsigned int)(%s),' % expr)
    for name, expr, _strict in ENUM_CONSTS:
        lines.append('    (unsigned int)(%s),' % expr)
    lines.append('};')
    return '\n'.join(lines) + '\n'


def read_rel_symbol(obj, name):
    """재배치 오브젝트(.o)에서 심볼 name의 바이트(섹션 데이터)."""
    d = open(obj, 'rb').read()
    shoff = struct.unpack_from('<I', d, 32)[0]
    shentsize, shnum, shstrndx = struct.unpack_from('<HHH', d, 46)
    secs = [struct.unpack_from('<IIIIIIIIII', d, shoff + i * shentsize) for i in range(shnum)]
    for (nm, typ, fl, addr, off, size, link, info, al, es) in secs:
        if typ != 2:
            continue
        stroff = secs[link][4]
        for j in range(size // 16):
            st_name, val, ssz, sinfo, oth, shndx = struct.unpack_from('<IIIBBH', d, off + j * 16)
            e = d.index(b'\0', stroff + st_name)
            if d[stroff + st_name:e].decode('latin1') == name:
                return d[secs[shndx][4] + val: secs[shndx][4] + val + ssz]
    die('%s: 심볼 %s 없음' % (obj, name))


def collect_layout(src, work):
    os.makedirs(work, exist_ok=True)
    c = os.path.join(work, 'save_compat_probe.c')
    o = os.path.join(work, 'save_compat_probe.o')
    with open(c, 'w') as f:
        f.write(probe_source())
    cmd = [GCC, '-c', '-w', '-iquote', os.path.join(src, 'include')] + CPPFLAGS + CFLAGS + ['-o', o, c]
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode != 0:
        die('헤더 TU 컴파일 실패 (%s):\n%s' % (' '.join(cmd), r.stderr[-4000:]))
    dw = dwarf_layout.Dwarf(o)
    out = []
    for s in STRICT_STRUCTS + ALLOWED_STRUCTS:
        size, mem = dw.members(s)
        if mem is None:
            out.append('STRUCT\t%s\tMISSING' % s)
            continue
        out.append('STRUCT\t%s\tsize=%d' % (s, size))
        for p, off, bit, bits, kind in mem:
            out.append('F\t%s\t%s\t%d\t%d\t%d\t%s' % (s, p, off, bit, bits, kind))
    blob = read_rel_symbol(o, 'gSaveCompatConsts')
    vals = struct.unpack('<%dI' % (len(blob) // 4), blob)
    for (name, expr, strict), v in zip(CONSTS + ENUM_CONSTS, vals):
        out.append('CONST\t%s\t%s\t%s' % (name, 'MISSING' if v == 0xDEADBEEF else str(v), 'strict' if strict else 'info'))
    # .c 안 구조체·조각(텍스트)
    for rel, sname in SRC_STRUCTS:
        p = os.path.join(src, rel)
        txt = open(p, encoding='utf-8', errors='replace').read() if os.path.exists(p) else ''
        m = re.search(r'struct\s+%s\s*\{.*?\};' % re.escape(sname), txt, re.S)
        body = norm_c(m.group(0)) if m else 'MISSING'
        out.append('SRCSTRUCT\t%s\t%s\t%s' % (rel, sname, body))
    for rel, pat in SRC_SNIPPETS:
        p = os.path.join(src, rel)
        txt = open(p, encoding='utf-8', errors='replace').read() if os.path.exists(p) else ''
        m = re.search(pat, txt, re.S | re.M)
        out.append('SRCSNIP\t%s\t%s' % (rel, norm_c(m.group(0)) if m else 'MISSING'))
    return out


def norm_c(s):
    s = re.sub(r'/\*.*?\*/', ' ', s, flags=re.S)
    s = re.sub(r'//[^\n]*', ' ', s)
    return re.sub(r'\s+', ' ', s).strip()


# ---------------------------------------------------------------------------------------------
# ELF / map
# ---------------------------------------------------------------------------------------------
class Elf:
    def __init__(self, path):
        self.path = path
        d = open(path, 'rb').read()
        self.data = d
        if d[:4] != b'\x7fELF' or d[4] != 1 or d[5] != 1:
            die('%s: ELF32 LE 아님' % path)
        shoff = struct.unpack_from('<I', d, 32)[0]
        shentsize, shnum, shstrndx = struct.unpack_from('<HHH', d, 46)
        secs = []
        for i in range(shnum):
            nm, typ, fl, addr, off, size, link, info, al, es = struct.unpack_from('<IIIIIIIIII', d, shoff + i * shentsize)
            secs.append(dict(nm=nm, type=typ, flags=fl, addr=addr, off=off, size=size, link=link))
        for s in secs:
            s['name'] = self._cstr(secs[shstrndx]['off'] + s['nm'])
        self.secs = secs
        self.rng = sorted((s['addr'], s['addr'] + s['size'], s['off']) for s in secs
                          if s['type'] == 1 and (s['flags'] & 2) and s['size'])
        self.rng_starts = [r[0] for r in self.rng]
        self.syms = []          # (addr, size, name, type, bind)
        self.by_name = {}
        for s in secs:
            if s['type'] != 2:
                continue
            strt = secs[s['link']]
            blob = d[s['off']:s['off'] + s['size']]
            for (st_name, val, size, info, _o, shndx) in struct.iter_unpack('<IIIBBH', blob):
                typ = info & 0xf
                if shndx == 0 or typ not in (1, 2):  # OBJECT, FUNC
                    continue
                name = self._cstr(strt['off'] + st_name)
                if not name or name.startswith('$'):
                    continue
                bind = info >> 4
                self.syms.append((val & ~1 if typ == 2 else val, size, name, typ, bind))
                prev = self.by_name.get(name)
                if prev is None or (bind == 1 and prev[4] != 1):
                    self.by_name[name] = (val & ~1 if typ == 2 else val, size, name, typ, bind)
                elif bind != 1 and prev[4] != 1:
                    self.by_name[name] = 'AMBIGUOUS'
        self.syms.sort()
        self.sym_starts = [x[0] for x in self.syms]

    def _cstr(self, off):
        e = self.data.index(b'\0', off)
        return self.data[off:e].decode('latin1')

    def sym(self, name):
        v = self.by_name.get(name)
        return None if v in (None, 'AMBIGUOUS') else v

    def read(self, addr, n):
        i = bisect.bisect_right(self.rng_starts, addr) - 1
        if i < 0:
            return None
        a, e, off = self.rng[i]
        if addr + n > e:
            return None
        return self.data[off + addr - a: off + addr - a + n]

    def symbolize_ex(self, a):
        """a → (이름, 오프셋, 타입 1=OBJECT 2=FUNC) 또는 None. 같은 주소면 전역 우선."""
        i = bisect.bisect_right(self.sym_starts, a) - 1
        exact = []
        j = i
        while j >= 0 and self.syms[j][0] == a:
            exact.append(self.syms[j])
            j -= 1
        if exact:
            exact.sort(key=lambda s: (s[4] != 1, s[2]))
            return exact[0][2], 0, exact[0][3]
        j, steps = i, 0
        while j >= 0 and steps < 256:
            s = self.syms[j]
            if s[0] <= a < s[0] + s[1]:
                return s[2], a - s[0], s[3]
            j -= 1
            steps += 1
        return None

    def symbolize(self, a):
        """a → 'name' / 'name+0xN' (OBJECT·FUNC 심볼 시작이거나 그 안이면), 아니면 None."""
        r = self.symbolize_ex(a)
        if r is None:
            return None
        return r[0] if r[1] == 0 else '%s+0x%x' % (r[0], r[1])


SEC_RE = re.compile(r'^ (\S+)\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)\s+(\S+)\s*$')
SEC_NAME_ONLY_RE = re.compile(r'^ (\S+)\s*$')
SEC_CONT_RE = re.compile(r'^\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)\s+(\S+)\s*$')
OUT_SEC_RE = re.compile(r'^(\.\S+|\S+)\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)(?:\s+load address.*)?$')


def parse_map(path):
    """입력 섹션 목록 [(addr, size, secname, file)] 과 출력 섹션 {name: (addr, size)}."""
    ins, outs = [], {}
    pend = None
    with open(path, encoding='latin1') as f:
        for line in f:
            line = line.rstrip('\n')
            m = OUT_SEC_RE.match(line)
            if m and not line.startswith(' '):
                outs[m.group(1)] = (int(m.group(2), 16), int(m.group(3), 16))
                pend = None
                continue
            m = SEC_RE.match(line)
            if m and not m.group(1).startswith('0x') and m.group(1) != '*fill*':
                ins.append((int(m.group(2), 16), int(m.group(3), 16), m.group(1), m.group(4)))
                pend = None
                continue
            m = SEC_NAME_ONLY_RE.match(line)
            if m and not m.group(1).startswith('0x') and not m.group(1).startswith('*'):
                pend = m.group(1)
                continue
            if pend:
                m = SEC_CONT_RE.match(line)
                if m:
                    ins.append((int(m.group(1), 16), int(m.group(2), 16), pend, m.group(3)))
                pend = None
    ins.sort()
    return ins, outs


EWRAM = (0x02000000, 0x02040000)
IWRAM = (0x03000000, 0x03008000)


def in_ram(a):
    return EWRAM[0] <= a < EWRAM[1] or IWRAM[0] <= a < IWRAM[1]


# ---------------------------------------------------------------------------------------------
# (2) 섹터 배치 / GF 헤더
# ---------------------------------------------------------------------------------------------
GF_FIELDS = [('version', 'u32'), ('language', 'u32'), ('gameName', 'b32')] + \
    [(n, 'ptr') for n in ('monFrontPics', 'monBackPics', 'monNormalPalettes', 'monShinyPalettes', 'monIcons',
                          'monIconPaletteIds', 'monIconPalettes', 'monSpeciesNames', 'moveNames', 'decorations')] + \
    [(n, 'u32') for n in ('flagsOffset', 'varsOffset', 'pokedexOffset', 'seen1Offset', 'seen2Offset', 'pokedexVar',
                          'pokedexFlag', 'mysteryEventFlag', 'pokedexCount')] + \
    [(n, 'u8') for n in ('playerNameLength', 'trainerNameLength', 'pokemonNameLength1', 'pokemonNameLength2')] + \
    [('unk%d' % i, 'u8') for i in range(5, 18)] + \
    [(n, 'u32') for n in ('saveBlock2Size', 'saveBlock1Size', 'partyCountOffset', 'partyOffset', 'warpFlagsOffset',
                          'trainerIdOffset', 'playerNameOffset', 'playerGenderOffset', 'frontierStatusOffset',
                          'frontierStatusOffset2', 'externalEventFlagsOffset', 'externalEventDataOffset', 'unk18')] + \
    [(n, 'ptr') for n in ('speciesInfo', 'abilityNames', 'abilityDescriptions', 'items', 'moves', 'ballGfx',
                          'ballPalettes')] + \
    [(n, 'u32') for n in ('gcnLinkFlagsOffset', 'gameClearFlag', 'ribbonFlag')] + \
    [(n, 'u8') for n in ('bagCountItems', 'bagCountKeyItems', 'bagCountPokeballs', 'bagCountTMHMs',
                         'bagCountBerries', 'pcItemsCount')] + \
    [(n, 'u32') for n in ('pcItemsOffset', 'giftRibbonsOffset', 'enigmaBerryOffset', 'enigmaBerrySize')] + \
    [('moveDescriptions', 'ptr'), ('unk20', 'u32')]


def collect_sector(elf):
    out = []
    s = elf.sym('sSaveSlotLayout')
    if s is None:
        out.append('SLOT\tMISSING')
    else:
        b = elf.read(s[0], s[1])
        n = s[1] // 4
        out.append('SLOT\tcount=%d' % n)
        for i in range(n):
            off, size = struct.unpack_from('<HH', b, i * 4)
            out.append('SLOT\t%d\toffset=%d\tsize=%d' % (i, off, size))
    s = elf.sym('sGFRomHeader')
    if s is None:
        out.append('GF\tMISSING')
    else:
        b = elf.read(s[0], s[1])
        off = 0
        out.append('GF\tsize=%d' % s[1])
        for name, typ in GF_FIELDS:
            al = 1 if typ in ('u8', 'b32') else 4
            off = (off + al - 1) // al * al
            if typ == 'u32':
                out.append('GF\t%s\t%d' % (name, struct.unpack_from('<I', b, off)[0]))
                off += 4
            elif typ == 'u8':
                out.append('GF\t%s\t%d' % (name, b[off]))
                off += 1
            elif typ == 'b32':
                out.append('GF\t%s\t%s' % (name, b[off:off + 32].rstrip(b'\0').decode('latin1')))
                off += 32
            else:
                v = struct.unpack_from('<I', b, off)[0]
                out.append('GFPTR\t%s\t%s' % (name, 'NULL' if v == 0 else (elf.symbolize(v & ~1) or '0x%08x' % v)))
                off += 4
        out.append('GF\tparsed_size=%d' % ((off + 3) // 4 * 4))
    return out


# ---------------------------------------------------------------------------------------------
# (3) 세이브 경로 코드 (정규화 역어셈블)
# ---------------------------------------------------------------------------------------------
STRICT_CODE_OBJS = ['src/save.o', 'src/load_save.o']
# 세이브 블록 안의 파티·박스에 직접 닿는 함수(엄격)
STRICT_CODE_FUNCS = ['GetSavedPlayerPartyMon', 'GetSavedPlayerPartyCount', 'SavePlayerPartyMon',
                     'ZeroPlayerPartyMons', 'GetBoxMonDataAt', 'SetBoxMonDataAt', 'GetBoxedMonPtr', 'CopyBoxMonAt',
                     'ZeroBoxMonAt', 'SetBoxMonAt', 'GetBoxNamePtr', 'StorageGetCurrentBox',
                     'ResetPokemonStorageSystem']
# 저장된 바이트의 '해석'(복호화·체크섬·필드 접근; 정적 함수 EncryptBoxMon 등은 이 안에 인라인됨) — 다르면 사람이 확인(주의)
REVIEW_CODE_FUNCS = ['GetMonData2', 'GetMonData3', 'GetBoxMonData2', 'GetBoxMonData3', 'SetMonData', 'SetBoxMonData',
                     'ZeroBoxMonData', 'ZeroMonData', 'BoxMonToMon', 'CalculatePlayerPartyCount', 'CalculateMonStats',
                     'CopyMon', 'CopyMonToPC', 'GetLevelFromBoxMonExp', 'GetBoxMonGender', 'BoxMonRestorePP',
                     # SaveBlock1.playerParty를 직접 쓰는 특수 전투 경로(battle_special.c)
                     'HandleSpecialTrainerBattleEnd', 'DoSpecialTrainerBattle']
# A안: 녹화 배틀 저장/재생(정보)
INFO_CODE_OBJS = ['src/recorded_battle.o']

LINE_ADDR_RE = re.compile(r'^\s*([0-9a-f]+):\s*(.*)$')
TARGET_RE = re.compile(r'\b[0-9a-f]{7,8} <([^>]+)>')
PCREL_RE = re.compile(r'@ \(([0-9a-f]+) <([^>]+)>\)')
WORD_RE = re.compile(r'^\.word\s+0x([0-9a-f]{8})(.*)$')


def func_table(elf, ins):
    """{name: (addr, size, objfile)} — map의 .text.<name> 입력 섹션으로 파일을 정한다."""
    funcs = {}
    for a, sz, sec, fil in ins:
        if sec.startswith('.text.') and 0x08000000 <= a < 0x0a000000:
            funcs.setdefault(sec[len('.text.'):], []).append((a, sz, fil))
    return funcs


def disasm(elf_path, elf, addr, size):
    out = sh([OBJDUMP, '-d', '--no-show-raw-insn', '--start-address=0x%x' % addr,
              '--stop-address=0x%x' % (addr + size), elf_path])
    lines = []
    started = False
    for line in out.splitlines():
        if re.match(r'^[0-9a-f]{8} <', line):
            started = True
            continue
        if not started:
            continue
        m = LINE_ADDR_RE.match(line)
        if not m:
            continue
        body = m.group(2).strip()
        lit = LDR_PC_RE.search(body)
        if lit and body.startswith('ldr'):
            # PC 상대 리터럴 읽기: 오프셋 대신 읽히는 값(심볼)으로 적는다 → 코드가 몇 줄 늘어도 이 줄은 그대로
            b = elf.read(int(lit.group(1), 16), 4)
            val = word_name(elf, struct.unpack('<I', b)[0]) if b else '?'
            body = LDR_PC_RE.sub('=' + val, body)
        body = PCREL_RE.sub(lambda mm: '@ (<%s>)' % mm.group(2), body)
        body = TARGET_RE.sub(lambda mm: '<%s>' % mm.group(1), body)
        w = WORD_RE.match(body)
        if w:
            body = '.word %s' % word_name(elf, int(w.group(1), 16))
        body = re.sub(r'\s+', ' ', body)
        lines.append(body)
    return lines


LDR_PC_RE = re.compile(r'\[pc, #\d+\]\s*@ \(([0-9a-f]+) <[^>]+>\)')


def word_name(elf, v):
    """리터럴 풀 워드 → 심볼 이름(+오프셋, |thumb) / 내용 표기 / 숫자."""
    if not (0x02000000 <= v < 0x0a000000):
        return '0x%08x' % v
    thumb = bool(v & 1) and v >= 0x08000000
    r = elf.symbolize_ex(v & ~1 if thumb else v)
    if r is not None and r[2] == 2 and r[1] != 0:
        # 함수 '중간'을 가리키는 값은 포인터가 아니라 숫자 상수(예: SECTOR_SIGNATURE 0x08012025)
        return '0x%08x' % v
    if r is not None:
        name = r[0] if r[1] == 0 else '%s+0x%x' % (r[0], r[1])
        return name + ('|thumb' if thumb else '')
    return rom_anon(elf, v) if v >= 0x08000000 else 'RAM?'


def rom_anon(elf, v):
    """심볼 없는 ROM 주소(switch 점프 표, 이름 없는 문자열 등)를 주소 대신 내용으로 표기한다.
    점프 표: 연속 워드가 같은 함수 안 코드 주소이면 'JT[함수+off,…]'. 그 밖: 0xFF(EOS)까지(최대 64바이트) 내용 해시."""
    words = []
    fn = None
    for k in range(64):
        b = elf.read(v + 4 * k, 4)
        if b is None:
            break
        w = struct.unpack('<I', b)[0]
        nm = elf.symbolize(w & ~1) if 0x08000000 <= w < 0x0a000000 else None
        base = nm.split('+')[0] if nm else None
        if base is None or (fn is not None and base != fn):
            break
        fn = base
        words.append(nm)
    if len(words) >= 2:
        return 'JT[%s]' % ','.join(words)
    blob = elf.read(v, 64) or b''
    e = blob.find(b'\xff')
    if e >= 0:
        blob = blob[:e + 1]
    return 'ROM?<%s>' % hashlib.sha1(blob).hexdigest()[:10]


def collect_code(elf_path, elf, ins, outdir):
    funcs = func_table(elf, ins)
    os.makedirs(outdir, exist_ok=True)
    wanted = []  # (class, name)
    for name, lst in sorted(funcs.items()):
        for a, sz, fil in lst:
            if fil in STRICT_CODE_OBJS:
                wanted.append(('strict', name))
            elif fil in INFO_CODE_OBJS:
                wanted.append(('info', name))
    for n in STRICT_CODE_FUNCS:
        wanted.append(('strict', n))
    for n in REVIEW_CODE_FUNCS:
        wanted.append(('review', n))
    seen = set()
    index = []
    jobs = {}
    from concurrent.futures import ThreadPoolExecutor
    pool = ThreadPoolExecutor(max_workers=min(16, os.cpu_count() or 4))
    for cls, name in wanted:
        lst = funcs.get(name)
        if lst and len(lst) == 1 and name not in jobs:
            jobs[name] = pool.submit(disasm, elf_path, elf, lst[0][0], lst[0][1])
    for cls, name in wanted:
        if name in seen:
            continue
        seen.add(name)
        lst = funcs.get(name)
        if not lst:
            index.append('CODE\t%s\t%s\tABSENT' % (cls, name))
            continue
        if len(lst) > 1:
            index.append('CODE\t%s\t%s\tMULTIPLE\t%s' % (cls, name, ','.join(x[2] for x in lst)))
            continue
        a, sz, fil = lst[0]
        lines = jobs[name].result()
        with open(os.path.join(outdir, name + '.s'), 'w') as f:
            f.write('\n'.join(lines) + '\n')
        index.append('CODE\t%s\t%s\t%s\tsize=%d\tinsns=%d' % (cls, name, fil, sz, len(lines)))
    pool.shutdown()
    return index


# ---------------------------------------------------------------------------------------------
# (4) RAM 정적 변수
# ---------------------------------------------------------------------------------------------
def collect_ram(elf, ins, outs):
    ram_ins = [x for x in ins if in_ram(x[0]) and x[1] > 0]
    starts = [x[0] for x in ram_ins]
    rows = []
    for a, sz, name, typ, bind in elf.syms:
        if typ != 1 or not in_ram(a):
            continue
        i = bisect.bisect_right(starts, a) - 1
        fil = '?'
        if i >= 0 and a < ram_ins[i][0] + ram_ins[i][1]:
            fil = ram_ins[i][3]
        region = 'EWRAM' if a < 0x03000000 else 'IWRAM'
        rows.append('RAM\t%s\t%s\t%s\t%s\t%d' % (region, fil, name, 'G' if bind == 1 else 'L', sz))
    rows.sort()
    files = {}
    for a, sz, sec, fil in ram_ins:
        region = 'EWRAM' if a < 0x03000000 else 'IWRAM'
        files[(region, fil)] = files.get((region, fil), 0) + sz
    for (region, fil), sz in sorted(files.items()):
        rows.append('RAMFILE\t%s\t%s\t%d' % (region, fil, sz))
    for name in ('.ewram', '.ewram.sbss', '.iwram', '.iwram.bss'):
        if name in outs:
            rows.append('RAMSEC\t%s\taddr=0x%08x\tsize=%d' % (name, outs[name][0], outs[name][1]))
    ew_end = max([outs[n][0] + outs[n][1] for n in ('.ewram', '.ewram.sbss') if n in outs] or [0])
    iw_end = max([outs[n][0] + outs[n][1] for n in ('.iwram', '.iwram.bss') if n in outs] or [0])
    rows.append('RAMEND\tEWRAM\tend=0x%08x\tused=%d\tfree=%d' % (ew_end, ew_end - EWRAM[0], EWRAM[1] - ew_end))
    rows.append('RAMEND\tIWRAM\tend=0x%08x\tused=%d\tfree_before_0x03007F00=%d' % (iw_end, iw_end - IWRAM[0],
                                                                                    0x03007F00 - iw_end))
    return rows


# ---------------------------------------------------------------------------------------------
# collect
# ---------------------------------------------------------------------------------------------
def collect(src, elf_path, map_path, out, label):
    if os.path.exists(out):
        shutil.rmtree(out)
    os.makedirs(out)
    work = os.path.join(out, 'work')
    lay = collect_layout(src, work)
    with open(os.path.join(out, 'layout.txt'), 'w') as f:
        f.write('\n'.join(lay) + '\n')
    meta = ['label\t%s' % label, 'facts_version\t%d' % FACTS_VERSION, 'src\t%s' % os.path.abspath(src)]
    if os.path.exists(os.path.join(src, 'MATERIALIZED_FROM')):
        meta.append('src_materialized_from\t%s' % open(os.path.join(src, 'MATERIALIZED_FROM')).read().strip())
    elif os.path.isdir(os.path.join(src, '.git')):
        meta.append('src_head\t%s' % sh(['git', '-C', src, 'rev-parse', 'HEAD']).strip())
        st = sh(['git', '-C', src, 'status', '--porcelain', '--untracked-files=no']).strip()
        meta.append('src_dirty_tracked_files\t%d' % (len(st.splitlines()) if st else 0))
    if elf_path:
        elf = Elf(elf_path)
        ins, outs = parse_map(map_path)
        meta += ['elf\t%s' % os.path.abspath(elf_path), 'elf_sha1\t%s' % sha1_file(elf_path),
                 'map\t%s' % os.path.abspath(map_path)]
        gba = os.path.splitext(elf_path)[0] + '.gba'
        if os.path.exists(gba):
            meta.append('gba_sha1\t%s\tsize=%d' % (sha1_file(gba), os.path.getsize(gba)))
        with open(os.path.join(out, 'sector.txt'), 'w') as f:
            f.write('\n'.join(collect_sector(elf)) + '\n')
        idx = collect_code(elf_path, elf, ins, os.path.join(out, 'code'))
        with open(os.path.join(out, 'code.txt'), 'w') as f:
            f.write('\n'.join(idx) + '\n')
        with open(os.path.join(out, 'ram.txt'), 'w') as f:
            f.write('\n'.join(collect_ram(elf, ins, outs)) + '\n')
    meta.append('collected\t%s' % datetime.datetime.now().isoformat(timespec='seconds'))
    with open(os.path.join(out, 'meta.txt'), 'w') as f:
        f.write('\n'.join(meta) + '\n')
    shutil.rmtree(work, ignore_errors=True)
    return out


# ---------------------------------------------------------------------------------------------
# compare
# ---------------------------------------------------------------------------------------------
class Report:
    def __init__(self):
        self.lines = []
        self.fail = 0
        self.warn = 0

    def res(self, kind, msg):
        self.lines.append('[%s] %s' % (kind, msg))
        if kind == 'FAIL':
            self.fail += 1
        elif kind == 'WARN':
            self.warn += 1

    def add(self, s=''):
        self.lines.append(s)

    def text(self):
        return '\n'.join(self.lines) + '\n'


def read_lines(p):
    return open(p).read().splitlines() if os.path.exists(p) else None


def parse_layout(lines):
    structs, fields, consts, srcs = {}, {}, {}, {}
    for l in lines:
        p = l.split('\t')
        if p[0] == 'STRUCT':
            structs[p[1]] = p[2]
            fields.setdefault(p[1], [])
        elif p[0] == 'F':
            fields.setdefault(p[1], []).append((int(p[4]), int(p[5]), p[2], p[6]))
        elif p[0] == 'CONST':
            consts[p[1]] = (p[2], p[3])
        elif p[0] in ('SRCSTRUCT', 'SRCSNIP'):
            srcs['\t'.join(p[:-1])] = p[-1]
    return structs, fields, consts, srcs


def cmp_struct(rep, name, pre_size, post_size, pre_f, post_f, strict, detail_dir):
    """FAIL(엄격)/INFO(허용): 크기 변화, 같은 이름 필드의 위치·폭 변화(순서 바꿈 포함), 잎 구간 변화.
    WARN: 바이트 구간은 같고 필드 이름·잎 타입 표기만 다름(예: 이름 변경, u16 → enum Item)."""
    if pre_size == post_size and pre_f == post_f:
        rep.res('PASS', '%-22s %s, 잎 필드 %d개 동일' % (name, pre_size, len(pre_f)))
        return True
    path = os.path.join(detail_dir, 'struct_%s.diff' % name)
    a = ['%7d %5d %s %s' % x for x in pre_f]
    b = ['%7d %5d %s %s' % x for x in post_f]
    with open(path, 'w') as f:
        f.write('# 열: 비트오프셋 비트폭 경로 잎종류 (이식 전 → 이식 후)\n')
        f.writelines(x + '\n' for x in difflib.unified_diff(a, b, 'pre', 'post', lineterm='', n=2))
    nd = sum(1 for x in difflib.unified_diff(a, b, lineterm='', n=0) if x[:1] in '+-' and x[:3] not in ('+++', '---'))
    pa = {p: (bo, w) for bo, w, p, _k in pre_f}
    pb = {p: (bo, w) for bo, w, p, _k in post_f}
    moved = sorted(p for p in set(pa) & set(pb) if pa[p] != pb[p])
    same_iv = [(bo, w) for bo, w, _p, _k in pre_f] == [(bo, w) for bo, w, _p, _k in post_f]
    if pre_size == post_size and same_iv and not moved:
        rep.res('WARN' if strict else 'INFO', '%-22s %s, 바이트 배치 동일, 이름/잎 타입 표기만 차이 %d줄(이름 변경·타입 별칭) → %s'
                % (name, post_size, nd, path))
        return True
    first = ''
    if moved:
        p0 = moved[0]
        first = ' 예: %s 비트 %d/%d → %d/%d' % (p0, pa[p0][0], pa[p0][1], pb[p0][0], pb[p0][1])
    rep.res('FAIL' if strict else 'INFO', '%-22s %s → %s, 배치 차이 %d줄, 위치·폭이 바뀐 같은 이름 필드 %d개%s → %s'
            % (name, pre_size, post_size, nd, len(moved), first, path))
    return False


KEY_FIELDS = [('SaveBlock1', 'playerPartyCount'), ('SaveBlock1', 'playerParty[6]'), ('SaveBlock2', 'playerName[8]'),
              ('SaveBlock2', 'playerTrainerId[4]'), ('SaveBlock2', 'frontier.challengeStatus'),
              ('PokemonStorage', 'currentBox'), ('PokemonStorage', 'boxes[14][30]'),
              ('PokemonStorage', 'boxNames[14][17]'), ('PokemonStorage', 'boxWallpapers[14]')]


def key_fields_line(fields):
    out = []
    for s, p in KEY_FIELDS:
        hit = [x for x in fields.get(s, []) if x[2] == p]
        out.append('%s.%s@%s' % (s, p, hit[0][0] // 8 if hit else '?'))
    return ', '.join(out)


def size_of(s):
    m = re.match(r'size=(\d+)', s or '')
    return int(m.group(1)) if m else None


def compare(pre, post, out):
    os.makedirs(out, exist_ok=True)
    rep = Report()
    mp = dict(l.split('\t', 1) for l in read_lines(os.path.join(pre, 'meta.txt')) or [] if '\t' in l)
    mq = dict(l.split('\t', 1) for l in read_lines(os.path.join(post, 'meta.txt')) or [] if '\t' in l)
    rep.add('# #8943 세이브·메모리 호환 비교 (save_compat.py)')
    rep.add('이식 전: %s  [%s] src=%s elf_sha1=%s' % (pre, mp.get('label'), mp.get('src_materialized_from') or mp.get('src'),
                                                  mp.get('elf_sha1', '-')[:12]))
    rep.add('이식 후: %s  [%s] src=%s elf_sha1=%s' % (post, mq.get('label'), mq.get('src_materialized_from') or mq.get('src'),
                                                  mq.get('elf_sha1', '-')[:12]))
    if mq.get('src_dirty_tracked_files'):
        rep.add('        (이식 후 src 추적 파일 변경 %s개 — 작업 트리 헤더 기준)' % mq['src_dirty_tracked_files'])
    rep.add('')

    # ---- (1) 레이아웃
    rep.add('## (1) 세이브 구조체 레이아웃 (헤더 TU DWARF)')
    sp, fp, cp, srcp = parse_layout(read_lines(os.path.join(pre, 'layout.txt')))
    sq, fq, cq, srcq = parse_layout(read_lines(os.path.join(post, 'layout.txt')))
    for name in STRICT_STRUCTS:
        if sp.get(name) == 'MISSING' and sq.get(name) == 'MISSING':
            rep.res('INFO', '%-22s 양쪽 모두 없음(설정상 비활성)' % name)
            continue
        cmp_struct(rep, name, sp.get(name), sq.get(name), fp.get(name, []), fq.get(name, []), True, out)
    rep.add('    핵심 오프셋(전): ' + key_fields_line(fp))
    rep.add('    핵심 오프셋(후): ' + key_fields_line(fq))
    rep.add('')
    rep.add('## (1b) A안 허용: RecordedBattleSave')
    for name in ALLOWED_STRUCTS:
        cmp_struct(rep, name, sp.get(name), sq.get(name), fp.get(name, []), fq.get(name, []), False, out)
        s_old, s_new = size_of(sp.get(name)), size_of(sq.get(name))
        lim = cq.get('SECTOR_COUNTER_OFFSET', ('MISSING',))[0]
        if s_new is not None and lim.isdigit():
            ok = s_new <= int(lim)
            rep.res('PASS' if ok else 'FAIL', '%s 크기 %d ≤ SECTOR_COUNTER_OFFSET %s (recorded_battle.c STATIC_ASSERT 조건)'
                    % (name, s_new, lim))
        ck = [x for x in fq.get(name, []) if x[2] == 'checksum']
        if ck and s_new is not None:
            ok = ck[0][0] == (s_new - 4) * 8 and ck[0][1] == 32
            rep.res('PASS' if ok else 'FAIL', '%s.checksum 위치 = 끝 4바이트(off %d, 크기 %d) — CalcByteArraySum(save, sizeof-4) 검사 대상'
                    % (name, ck[0][0] // 8, s_new))
        if s_old is not None and s_new is not None and s_old != s_new:
            if s_new - 4 >= s_old:
                why = ('새 checksum 자리(off %d)는 옛 기록 뒤 0으로 채운 영역이라 저장값 0, 앞 %d바이트 합은 옛 기록이 '
                       '비어 있지 않으면 0이 아니므로 불일치 → 무효' % (s_new - 4, s_new - 4))
            else:
                why = ('새 checksum 자리(off %d)는 옛 기록 내부 바이트다. 옛 기록이 우연히 그 자리에 앞 %d바이트 합을 '
                       '담을 확률만큼(≈2^-32)만 유효로 보인다' % (s_new - 4, s_new - 4))
            rep.res('INFO', '옛 녹화 기록 무효 처리 근거: 크기 %d → %d. %s' % (s_old, s_new, why))
        elif s_old == s_new and fp.get(name) != fq.get(name):
            rep.res('WARN', '%s 크기가 같고 배치만 다르다 → 옛 기록이 checksum을 통과해 새 형식으로 잘못 읽힐 수 있다. '
                    '확인 필요' % name)
    rep.add('')
    rep.add('## (1c) 세이브 상수')
    bad = []
    for name in [c[0] for c in CONSTS + ENUM_CONSTS]:
        a, b = cp.get(name, ('?', '?')), cq.get(name, ('?', '?'))
        if a[1] == 'strict' and a[0] == 'MISSING' and b[0] == 'MISSING':
            continue
        if a[0] != b[0]:
            if a[1] == 'strict':
                bad.append(name)
                rep.res('FAIL', '%s %s → %s' % (name, a[0], b[0]))
            else:
                rep.res('INFO', '%s %s → %s (정보)' % (name, a[0], b[0]))
    if not bad:
        rep.res('PASS', '엄격 상수 %d개 동일 (SECTOR_*, PARTY_SIZE=%s, TOTAL_BOXES_COUNT=%s, IN_BOX_COUNT=%s …)'
                % (sum(1 for c in CONSTS + ENUM_CONSTS if c[2]), cq.get('PARTY_SIZE', ('?',))[0],
                   cq.get('TOTAL_BOXES_COUNT', ('?',))[0], cq.get('IN_BOX_COUNT', ('?',))[0]))
    for k in sorted(set(srcp) | set(srcq)):
        if srcp.get(k) == srcq.get(k):
            rep.res('PASS', '%s 본문 동일 (%s…)' % (k.replace('\t', ' '), (srcq.get(k) or '')[:40]))
        else:
            rep.res('FAIL', '%s 본문 차이\n    전: %s\n    후: %s' % (k.replace('\t', ' '), srcp.get(k), srcq.get(k)))
    rep.add('')

    # ---- (2) 섹터 배치
    rep.add('## (2) 세이브 섹터 배치 (ELF sSaveSlotLayout, sGFRomHeader)')
    a = read_lines(os.path.join(pre, 'sector.txt'))
    b = read_lines(os.path.join(post, 'sector.txt'))
    if a is None or b is None:
        rep.res('INFO', 'ELF 없음 — 건너뜀')
    else:
        sa = [l for l in a if l.startswith('SLOT')]
        sb = [l for l in b if l.startswith('SLOT')]
        if sa == sb and 'MISSING' not in ''.join(sa):
            slots = ', '.join('%s:%s/%s' % (p.split('\t')[1], p.split('\t')[2][7:], p.split('\t')[3][5:])
                              for p in sb[1:])
            rep.res('PASS', 'sSaveSlotLayout %d칸 동일 [섹터ID:offset/size] %s' % (len(sb) - 1, slots))
        else:
            rep.res('FAIL', 'sSaveSlotLayout 차이:\n' + '\n'.join('    ' + x for x in difflib.unified_diff(sa, sb, 'pre', 'post', lineterm='', n=0)))
        ga = [l for l in a if l.startswith('GF\t')]
        gb = [l for l in b if l.startswith('GF\t')]
        if ga == gb:
            d = dict(l.split('\t')[1:3] for l in gb if l.count('\t') == 2)
            rep.res('PASS', 'sGFRomHeader 오프셋·크기 %d개 동일 (saveBlock1Size=%s saveBlock2Size=%s partyCountOffset=%s partyOffset=%s)'
                    % (len(gb), d.get('saveBlock1Size'), d.get('saveBlock2Size'), d.get('partyCountOffset'), d.get('partyOffset')))
        else:
            rep.res('FAIL', 'sGFRomHeader 차이:\n' + '\n'.join('    ' + x for x in difflib.unified_diff(ga, gb, 'pre', 'post', lineterm='', n=0)))
        pa = [l for l in a if l.startswith('GFPTR')]
        pb = [l for l in b if l.startswith('GFPTR')]
        if pa != pb:
            rep.res('INFO', 'sGFRomHeader 포인터 대상 이름 차이(주소 무관, 참고):\n' + '\n'.join('    ' + x for x in difflib.unified_diff(pa, pb, 'pre', 'post', lineterm='', n=0)))
    rep.add('')

    # ---- (3) 코드
    rep.add('## (3) 세이브 경로 코드 (정규화 역어셈블; gParties[0/1]·gPartiesCount[0/1] → 이식 전 이름 별칭)')
    ia = read_lines(os.path.join(pre, 'code.txt'))
    ib = read_lines(os.path.join(post, 'code.txt'))
    if ia is None or ib is None:
        rep.res('INFO', 'ELF 없음 — 건너뜀')
    else:
        pok = size_of('size=' + cq.get('sizeof_Pokemon_x_PARTY', ('0',))[0])
        ma = {l.split('\t')[2]: l.split('\t') for l in ia}
        mb = {l.split('\t')[2]: l.split('\t') for l in ib}
        counts = {}
        for name in sorted(set(ma) | set(mb), key=lambda n: ((ma.get(n) or mb.get(n))[1], n)):
            ra, rb = ma.get(name), mb.get(name)
            cls = (ra or rb)[1]
            ka = 'ABSENT' if ra is None else ra[3]
            kb = 'ABSENT' if rb is None else rb[3]
            sev = {'strict': 'FAIL', 'review': 'WARN', 'info': 'INFO'}[cls]
            if ka == 'ABSENT' and kb == 'ABSENT':
                continue
            if ka in ('ABSENT', 'MULTIPLE') or kb in ('ABSENT', 'MULTIPLE'):
                rep.res(sev if cls != 'info' else 'INFO', '%s %s: 전 %s / 후 %s' % (cls, name, ka, kb))
                counts.setdefault(cls, [0, 0])[1] += 1
                continue
            la = open(os.path.join(pre, 'code', name + '.s')).read().splitlines()
            lb = [alias_line(x, pok) for x in open(os.path.join(post, 'code', name + '.s')).read().splitlines()]
            la = [alias_line(x, pok) for x in la]
            if la == lb:
                counts.setdefault(cls, [0, 0])[0] += 1
                continue
            counts.setdefault(cls, [0, 0])[1] += 1
            path = os.path.join(out, 'code_%s.diff' % name)
            with open(path, 'w') as f:
                f.writelines(x + '\n' for x in difflib.unified_diff(la, lb, 'pre/' + name, 'post/' + name, lineterm='', n=3))
            nd = sum(1 for x in difflib.unified_diff(la, lb, lineterm='', n=0) if x[:1] in '+-' and x[:3] not in ('+++', '---'))
            rep.res(sev, '%s %s (%s): 정규화 코드 차이 %d줄 → %s' % (cls, name, rb[3], nd, path))
        for cls in ('strict', 'review', 'info'):
            if cls in counts:
                same, diff = counts[cls]
                kind = {'strict': 'PASS' if diff == 0 else 'FAIL', 'review': 'PASS' if diff == 0 else 'WARN',
                        'info': 'INFO'}[cls]
                desc = {'strict': '엄격(save.o·load_save.o 전체 + 세이브 파티/박스 접근 함수)',
                        'review': '검토(GetMonData·암호화·체크섬 등 저장 바이트 해석)',
                        'info': '정보(recorded_battle.o, A안)'}[cls]
                rep.res(kind, '%s: 동일 %d / 차이·누락 %d' % (desc, same, diff))
    rep.add('')

    # ---- (4) RAM
    rep.add('## (4) EWRAM·IWRAM 정적 변수 (ELF 심볼 + map)')
    ra = read_lines(os.path.join(pre, 'ram.txt'))
    rb = read_lines(os.path.join(post, 'ram.txt'))
    if ra is None or rb is None:
        rep.res('INFO', 'ELF 없음 — 건너뜀')
    else:
        ram_compare(rep, ra, rb, out)
    rep.add('')
    verdict = 'FAIL' if rep.fail else 'PASS'
    rep.add('판정: %s (FAIL %d, WARN %d)' % (verdict, rep.fail, rep.warn))
    txt = rep.text()
    with open(os.path.join(out, 'report.txt'), 'w') as f:
        f.write(txt)
    return rep, txt


def alias_line(line, party_bytes):
    """이식 후 이름 → 이식 전 이름. gParties+k: k<P → gPlayerParty+k, P≤k<2P → gEnemyParty+(k-P)."""
    def rp(m):
        base, off = m.group(1), int(m.group(2), 16) if m.group(2) else 0
        if base == 'gParties' and party_bytes:
            if off < party_bytes:
                return 'gPlayerParty' + ('+0x%x' % off if off else '')
            if off < 2 * party_bytes:
                return 'gEnemyParty' + ('+0x%x' % (off - party_bytes) if off - party_bytes else '')
            return m.group(0)
        if base == 'gPartiesCount':
            return {0: 'gPlayerPartyCount', 1: 'gEnemyPartyCount'}.get(off, m.group(0))
        return m.group(0)
    return re.sub(r'\b(gParties|gPartiesCount)(?:\+0x([0-9a-f]+))?\b', rp, line)


EXPECTED_RAM = {
    # #8943 upstream 70340c1135이 바꾸는 정적 변수(이름만): 바뀌어도 '예상'으로 분류
    'gPlayerParty', 'gEnemyParty', 'gPlayerPartyCount', 'gEnemyPartyCount', 'gParties', 'gPartiesCount',
    'sSavedPlayerParty', 'sSavedOpponentParty', 'sSavedParties',          # recorded_battle.c
    'gMultiPartnerParty', 'sMultiPartnerPartyBuffer',                      # battle_main.c
    'sBattleRecords',                  # recorded_battle.c [MAX_BATTLERS_COUNT][BATTLER_RECORD_SIZE] 664 → 388
}


def ram_compare(rep, ra, rb, out):
    def parse(lines):
        syms, files, secs, ends = {}, {}, {}, {}
        for l in lines:
            p = l.split('\t')
            if p[0] == 'RAM':
                key = (p[1], p[2], p[3])
                syms.setdefault(key, []).append(int(p[5]))
            elif p[0] == 'RAMFILE':
                files[(p[1], p[2])] = int(p[3])
            elif p[0] == 'RAMSEC':
                secs[p[1]] = int(p[3].split('=')[1])
            elif p[0] == 'RAMEND':
                ends[p[1]] = dict(x.split('=') for x in p[2:])
        return syms, files, secs, ends
    sa, fa, xa, ea = parse(ra)
    sb, fb, xb, eb = parse(rb)
    for region in ('EWRAM', 'IWRAM'):
        ua, ub = int(ea[region]['used']), int(eb[region]['used'])
        free_k = [k for k in eb[region] if k.startswith('free')][0]
        rep.res('INFO', '%s 사용 %d → %d B (%+d), 남은 공간 %s B' % (region, ua, ub, ub - ua, eb[region][free_k]))
    rows = []
    for key in sorted(set(sa) | set(sb)):
        a, b = sum(sa.get(key, [])), sum(sb.get(key, []))
        if sorted(sa.get(key, [])) != sorted(sb.get(key, [])):
            rows.append((key, a, b))
    # 이름만 같은 쪽으로 파일 이동한 것은 한 줄로 합친다
    exp, unexp = [], []
    for (region, fil, name), a, b in rows:
        (exp if name in EXPECTED_RAM else unexp).append('%s\t%s\t%s\t%d\t%d\t%+d' % (region, fil, name, a, b, b - a))
    path = os.path.join(out, 'ram_symbols.diff.tsv')
    with open(path, 'w') as f:
        f.write('# 구역\t파일\t심볼\t전(B)\t후(B)\t차이  — 예상(#8943 upstream 변수)\n')
        f.writelines(x + '\n' for x in exp)
        f.write('# 그 밖(확인 필요)\n')
        f.writelines(x + '\n' for x in unexp)
    se = sum(int(x.split('\t')[5]) for x in exp)
    rep.res('INFO', '예상된 변수 변화 %d건 합계 %+d B (gParties·gPartiesCount·sSavedParties·gMultiPartnerParty 등) → %s'
            % (len(exp), se, path))
    for x in exp:
        rep.add('        ' + x.replace('\t', '  '))
    if unexp:
        su = sum(int(x.split('\t')[5]) for x in unexp)
        rep.res('WARN', '그 밖 변수 변화 %d건 합계 %+d B — 세이브와 무관한지 확인 (%s 아래쪽)' % (len(unexp), su, path))
        for x in unexp[:40]:
            rep.add('        ' + x.replace('\t', '  '))
        if len(unexp) > 40:
            rep.add('        … (%d건 더)' % (len(unexp) - 40))
    else:
        rep.res('PASS', '그 밖 RAM 변수 크기·위치 파일 변화 없음')
    # 세이브 블록 실체(EWRAM)
    for nm in ('gSaveblock1', 'gSaveblock2', 'gSaveblock3', 'gPokemonStorage'):
        ka = [v for k, v in sa.items() if k[2] == nm]
        kb = [v for k, v in sb.items() if k[2] == nm]
        if ka == kb and ka:
            rep.res('PASS', '%s 크기 %s B 동일' % (nm, ka[0][0]))
        else:
            rep.res('FAIL', '%s 크기 %s → %s' % (nm, ka, kb))


# ---------------------------------------------------------------------------------------------
# 명령
# ---------------------------------------------------------------------------------------------
def default_out(tag):
    return os.path.join(TMP, '%s-%s' % (tag, datetime.datetime.now().strftime('%Y%m%d-%H%M%S')))


def cmd_collect(args):
    hnsverify_paths.check_out_dir(args.out)
    src = args.src
    if args.rev:
        src = materialize_rev(REPO, args.rev, os.path.join(args.out + '-src'))
    collect(src, args.elf, args.map or (os.path.splitext(args.elf)[0] + '.map' if args.elf else None), args.out,
            args.label or args.out)
    print('wrote', args.out)


def cmd_compare(args):
    rep, txt = compare(args.pre, args.post, args.out)
    sys.stdout.write(txt)
    print('report:', os.path.join(args.out, 'report.txt'))
    return 1 if rep.fail else 0


def norm_report(txt):
    """판정 줄만 남기고 경로·해시를 지운다(기대 결과 비교용)."""
    out = []
    for l in txt.splitlines():
        if l.startswith('[') or l.startswith('        EWRAM') or l.startswith('        IWRAM') or l.startswith('판정'):
            l = re.sub(r'→ \S+/(struct_|code_|ram_)', r'→ \1', l)
            l = re.sub(r'\(\S*/ram_symbols\.diff\.tsv', '(ram_symbols.diff.tsv', l)
            out.append(l)
    return out


def check_expect(txt, expect_path):
    if not expect_path or not os.path.exists(expect_path):
        return
    a = norm_report(open(expect_path).read())
    b = norm_report(txt)
    if a == b:
        print('기대 결과(%s)와 판정 줄 %d개가 모두 같다.' % (expect_path, len(b)))
    else:
        print('기대 결과(%s)와 판정 줄이 다르다(아래 -기대 +이번). 다른 줄만 사람이 읽으면 된다:' % expect_path)
        for x in difflib.unified_diff(a, b, 'expected', 'this-run', lineterm='', n=0):
            if not x.startswith(('---', '+++', '@@')):
                print('    ' + x)


def cmd_run(args):
    pre = args.pre
    if not os.path.exists(os.path.join(pre, 'meta.txt')):
        die('%s 에 meta.txt가 없다 — 먼저 collect로 이식 전 사실을 만든다' % pre)
    out = hnsverify_paths.check_out_dir(args.out or default_out('run'))
    post = os.path.join(out, 'post-facts')
    elf = args.post_elf
    mp = args.post_map or os.path.splitext(elf)[0] + '.map'
    for p in (elf, mp):
        if not os.path.exists(p):
            die('%s 없음 (make hns -j8 뒤에 돌린다)' % p)
    newest, newest_f = 0, None
    for sub in ('include', 'src'):
        for root, _d, files in os.walk(os.path.join(args.post_src, sub)):
            for fn in files:
                if fn.endswith(('.h', '.c', '.inc', '.party')):
                    m = os.path.getmtime(os.path.join(root, fn))
                    if m > newest:
                        newest, newest_f = m, os.path.join(root, fn)
    if newest > os.path.getmtime(elf):
        print('주의: %s 가 ELF보다 새롭다 → make hns -j8 뒤에 다시 돌린다(지금 결과는 옛 ELF 기준).' % newest_f)
    collect(args.post_src, elf, mp, post, 'post: %s 헤더 + %s' % (args.post_src, elf))
    rep, txt = compare(pre, post, out)
    sys.stdout.write(txt)
    print('report:', os.path.join(out, 'report.txt'))
    check_expect(txt, args.expect)
    return 1 if rep.fail else 0


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest='cmd', required=True)
    p = sub.add_parser('run', help='이식 후 검사(메인용)')
    p.add_argument('--post-src', default=REPO, help='이식 후 헤더 트리(기본: 저장소 작업 트리)')
    p.add_argument('--post-elf', default=os.path.join(REPO, 'pokehns.elf'))
    p.add_argument('--post-map', default=None)
    p.add_argument('--pre', required=True, help='이식 전 사실 디렉터리(같은 기계에서 collect로 만든 것)')
    p.add_argument('--expect', default=None,
                   help='기대 보고서(선택). 판정 줄만 비교해 같은지 알려 준다')
    p.add_argument('--out', default=None, help='기본: $HNS_VERIFY_OUT/save/run-<시각>')
    p = sub.add_parser('collect')
    p.add_argument('--src', default=REPO)
    p.add_argument('--rev', default=None, help='작업 트리 대신 git rev 헤더를 풀어 쓴다')
    p.add_argument('--elf', default=None)
    p.add_argument('--map', default=None)
    p.add_argument('--label', default=None)
    p.add_argument('--out', required=True)
    p = sub.add_parser('compare')
    p.add_argument('pre')
    p.add_argument('post')
    p.add_argument('--out', required=True)
    args = ap.parse_args()
    if args.cmd == 'run':
        sys.exit(cmd_run(args))
    if args.cmd == 'collect':
        cmd_collect(args)
    if args.cmd == 'compare':
        hnsverify_paths.check_out_dir(args.out)
        sys.exit(cmd_compare(args))


if __name__ == '__main__':
    main()
