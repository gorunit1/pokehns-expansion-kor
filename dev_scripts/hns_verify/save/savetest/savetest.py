#!/usr/bin/env python3
"""#8943 일반 세이브 왕복 확인 (테스트 러너 = mgba-rom-test 안의 실제 저장·불러오기 코드).
(옛 chunk-1385/verify/savetest/savetest.py. 경로·가드·이미지 확인만 바뀜)

  run <tree> <label> [--image X.sav|X-flash.bin] [--image-name NAME] [--jobs N]
      <tree>의 test/에 zz_hns8943_savecompat.c(+ --image가 있으면 zz_hns8943_savimage.h)를 넣고
      `make check BUILD=hns TESTS="HNS8943 SAVE"`를 돌려 결과를 $HNS_VERIFY_OUT/savetest/<label>/ 에 남긴다.
        <tree>: mkcopy.sh 사본(.git 없음). 실제 체크아웃(.git 있음)은 ALLOW_REPO=1일 때만. 어느 쪽이든 넣은 .c/.h와
                그 오브젝트를 실행 뒤(중간에 끊겨도) 지운다.
        매번 테스트 오브젝트(build/hns-test/test/zz_hns8943_savecompat.o/.d)를 지워 다시 컴파일하고, LOAD 테스트가
        출력한 "LOAD image=<이름>@<sha1 12자리>"가 이번 --image와 같은지 확인한다(다르면 FAIL — 옛 도구에서
        이미지를 바꾼 뒤 테스트가 다시 빌드되지 않아 앞 이미지를 읽은 적이 있다).
        make.sav  : MAKE 테스트가 이 빌드의 TrySavingData(SAVE_NORMAL)로 쓴 플래시 128KB(+ 이 빌드 형식의 녹화 기록 섹터 31)
        load.txt  : LOAD 테스트가 --image 세이브를 이 빌드의 LoadGameSave로 읽은 결과(파티·박스 원시 바이트와
                    GetMonData 전 필드, 플레이어 정보, 세이브 블록 해시, 녹화 기록 유효 여부, 다시 저장한 섹터 해시)
        make.log  : make check 전체 출력
  compare <labelA|dir> <labelB|dir>
      make.sav: 섹터별 바이트 비교(섹터 31 녹화 기록은 A안이라 따로 표시)
        (기준 폴더 baseline/pre-load-*/에는 .sav가 없고 baseline/pre-make-flash.bin·pre-newgame-flash.bin을 읽는다:
         .gitignore의 *.sa* 때문에 이름을 바꿨다. 내용은 이식 전 make.sav·newgame.sav와 바이트 같다.
         newgame은 baseline/expect-newgame-flash.bin이 있으면 그것을 읽는다 — 새 게임 값이 의도해서 바뀐 뒤의 기준, README 5절)
      load.txt: 줄 비교(RECORDED_BATTLE_VALID 줄은 A안 기대값으로 따로 판정)
  reparse <dir>
      <dir>/make.log를 다시 읽어 load.txt 등을 다시 만든다(출력 형식을 고친 뒤 옛 결과에 적용)
  image <in.sav> <out.h>
      .sav(128KB, mGBA 실기 세이브도 됨) → 테스트가 읽는 C 헤더(0xFF가 아닌 섹터만)
"""
import argparse
import hashlib
import os
import re
import shutil
import signal
import subprocess
import sys

sys.dont_write_bytecode = True   # 저장소 안에 __pycache__를 남기지 않는다
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(os.path.dirname(HERE)))
import hnsverify_paths  # noqa: E402
RUNS = os.path.join(hnsverify_paths.OUT, 'savetest')   # 기본 $HNS_VERIFY_OUT/savetest
TEST_C = 'zz_hns8943_savecompat.c'
TEST_H = 'zz_hns8943_savimage.h'
SECTOR = 4096
NSEC = 32
SAV_SIZE = 0x20000


def image_tag(sav_path, name=None):
    """LOAD 테스트가 출력하는 이미지 이름: <이름>@<sha1 앞 12자리>(이미지가 바뀌었는지 확인용)."""
    return '%s@%s' % (name or os.path.basename(sav_path), hashlib.sha1(open(sav_path, 'rb').read()).hexdigest()[:12])


def make_image_header(sav_path, out_h, name=None):
    data = open(sav_path, 'rb').read()
    if len(data) < NSEC * SECTOR:
        raise SystemExit('%s: %d바이트 — 128KB(.sav) 이상이어야 한다' % (sav_path, len(data)))
    secs = [s for s in range(NSEC) if data[s * SECTOR:(s + 1) * SECTOR] != b'\xff' * SECTOR]
    with open(out_h, 'w') as f:
        f.write('// 자동 생성: savetest.py image %s\n#define HNS8943_SAVIMAGE 1\n' % os.path.basename(sav_path))
        f.write('#define HNS8943_SAVIMAGE_NAME "%s"\n' % image_tag(sav_path, name))
        f.write('static const u8 sSavImageSectors[] = {%s};\n' % ', '.join(map(str, secs)))
        f.write('static const u8 sSavImageData[%d][%d] = {\n' % (len(secs), SECTOR))
        for s in secs:
            blob = data[s * SECTOR:(s + 1) * SECTOR]
            f.write('  { // sector %d\n' % s)
            for o in range(0, SECTOR, 32):
                f.write('    ' + ','.join('0x%02x' % b for b in blob[o:o + 32]) + ',\n')
            f.write('  },\n')
        f.write('};\n')
    return secs


ANSI = re.compile(r'\x1b\[[0-9;]*m')


def parse_log(log):
    sav = bytearray(b'\xff' * SAV_SIZE)
    savn = bytearray(b'\xff' * SAV_SIZE)
    nsav = nsavn = 0
    dump, info, results = [], [], []
    cur = '?'
    by_test = {}
    for line in open(log, encoding='utf-8', errors='replace'):
        line = ANSI.sub('', line.rstrip('\n'))
        m = re.match(r'^\[\d+\] (.*?): ', line)
        if m:
            cur = m.group(1)   # hydra는 결과 줄 뒤에 그 테스트의 출력을 붙인다(프로세스 순서는 매번 다를 수 있음)
        m = re.match(r'^SAV ([0-9a-f]{2}) ([0-9a-f]{8}) ([0-9a-f]+)$', line)
        if m:
            s, o, hx = int(m.group(1), 16), int(m.group(2), 16), bytes.fromhex(m.group(3))
            sav[s * SECTOR + o: s * SECTOR + o + len(hx)] = hx
            nsav += 1
            continue
        m = re.match(r'^SAVN ([0-9a-f]{2}) ([0-9a-f]{8}) ([0-9a-f]+)$', line)
        if m:
            s, o, hx = int(m.group(1), 16), int(m.group(2), 16), bytes.fromhex(m.group(3))
            savn[s * SECTOR + o: s * SECTOR + o + len(hx)] = hx
            nsavn += 1
            continue
        if line.startswith('DUMP '):
            by_test.setdefault(cur, []).append(line)
        elif line.startswith(('MAKE ', 'LOAD ', 'NEWGAME ')):
            info.append(line)
        m = re.match(r'^\[\d+\] (HNS8943 SAVE \w+): (\S+)', line)
        if m:
            results.append('%s: %s' % (m.group(1), m.group(2)))
        if line.startswith('- Tests'):
            results.append(line)
    for t in sorted(by_test):
        dump += by_test[t]
    info.sort()
    results.sort()
    return bytes(sav), nsav, bytes(savn), nsavn, dump, info, results


def _sigterm(_signum, _frame):
    raise SystemExit(143)


def cmd_run(a):
    tree = os.path.realpath(a.tree)
    if not os.path.isfile(os.path.join(tree, 'test', 'test_runner.c')):
        raise SystemExit('HnS 트리가 아니다: %s' % tree)
    in_repo = hnsverify_paths.is_checkout(tree)
    if in_repo and os.environ.get('ALLOW_REPO') != '1':
        raise SystemExit('%s 는 실제 체크아웃(.git)이다. ALLOW_REPO=1이면 테스트 파일을 넣고 실행한 뒤 지운다' % tree)
    test_c = os.path.join(tree, 'test', TEST_C)
    h = os.path.join(tree, 'test', TEST_H)
    objs = [os.path.join(tree, 'build', 'hns-test', 'test', TEST_C[:-2] + ext) for ext in ('.o', '.d')]
    if in_repo:
        for rel in ('test/' + TEST_C, 'test/' + TEST_H):
            if subprocess.run(['git', '-C', tree, 'ls-files', '--error-unmatch', rel],
                              capture_output=True).returncode == 0:
                raise SystemExit('%s 가 %s 에서 추적 파일이다 — 덮어쓰거나 지우지 않는다' % (rel, tree))
        if os.path.exists(test_c) and open(test_c, 'rb').read() != open(os.path.join(HERE, TEST_C), 'rb').read():
            raise SystemExit('%s 가 이미 있고 이 도구의 파일과 다르다 — 먼저 치운다' % test_c)
    out = hnsverify_paths.check_out_dir(os.path.join(RUNS, a.label))
    if os.path.exists(out):
        shutil.rmtree(out)
    os.makedirs(out)
    want = None
    old_term = signal.signal(signal.SIGTERM, _sigterm)
    try:
        shutil.copy(os.path.join(HERE, TEST_C), test_c)
        if a.image:
            secs = make_image_header(a.image, h, a.image_name or os.path.basename(a.image))
            want = image_tag(a.image, a.image_name)
            shutil.copy(a.image, os.path.join(out, 'input.sav'))
            print('image %s: sectors %s, tag %s' % (a.image, secs, want))
        elif os.path.exists(h):
            os.remove(h)
        for o in objs:          # 이미지 헤더는 make 의존성에 안 잡힐 수 있어 매번 다시 컴파일한다
            if os.path.exists(o):
                os.remove(o)
        log = os.path.join(out, 'make.log')
        with open(log, 'w') as f:
            r = subprocess.run(['make', 'check', 'BUILD=hns', '-j%d' % a.jobs, 'TESTS=HNS8943 SAVE'], cwd=tree,
                               stdout=f, stderr=subprocess.STDOUT)
    finally:   # 사본·실제 저장소 모두 넣은 파일과 오브젝트를 지운다(사본에 남으면 kortests의 TESTS=HNS 등에 섞인다)
        for p in [test_c, h] + objs:
            if os.path.exists(p):
                os.remove(p)
        print('removed %s, %s and their objects from %s%s' % (TEST_C, TEST_H, tree,
              ' (git status should be clean again)' if in_repo else ''))
        signal.signal(signal.SIGTERM, old_term)
    sav, nsav, savn, nsavn, dump, info, results = parse_log(log)
    if nsav:
        open(os.path.join(out, 'make.sav'), 'wb').write(sav)
    if nsavn:
        open(os.path.join(out, 'newgame.sav'), 'wb').write(savn)
    open(os.path.join(out, 'load.txt'), 'w').write('\n'.join(dump) + ('\n' if dump else ''))
    open(os.path.join(out, 'summary.txt'), 'w').write('\n'.join(['tree ' + tree, 'make_exit %d' % r.returncode]
                                                             + info + results) + '\n')
    sys.stdout.write(open(os.path.join(out, 'summary.txt')).read())
    print('SAV lines %d, SAVN lines %d, DUMP lines %d → %s' % (nsav, nsavn, len(dump), out))
    bad = r.returncode != 0
    loads = [l.split()[1][len('image='):] for l in info if l.startswith('LOAD image=')]
    if want is not None:
        if loads != [want]:
            print('[FAIL] LOAD 테스트가 읽은 이미지 %s ≠ 이번 이미지 %s (테스트가 다시 빌드되지 않았다?)' % (loads or '없음', want))
            bad = True
        else:
            print('[PASS] LOAD 테스트가 읽은 이미지 = %s' % want)
    elif loads:
        print('[FAIL] --image 없이 돌렸는데 LOAD 테스트가 이미지 %s 를 읽었다' % loads)
        bad = True
    return 1 if bad else 0


def flash_file(d, kind):
    """<d>/<kind>.sav, 없으면 기준 폴더(baseline/pre-load-*)용 파일: newgame은 <d>/../expect-newgame-flash.bin
    (새 게임 초기화 값이 의도해서 바뀐 뒤의 NEWGAME 출력, README 5절)이 있으면 그것, 그 밖은 <d>/../pre-<kind>-flash.bin.
    pre-<kind>-flash.bin은 LOAD 테스트 입력(이식 전 ROM이 만든 이미지)이라 바꾸지 않는다."""
    base = os.path.dirname(os.path.abspath(d))
    cands = [os.path.join(d, kind + '.sav')]
    if kind == 'newgame':
        cands.append(os.path.join(base, 'expect-newgame-flash.bin'))
    cands.append(os.path.join(base, 'pre-%s-flash.bin' % kind))
    for p in cands:
        if os.path.exists(p):
            return p
    return os.path.join(d, kind + '.sav')


def cmd_compare(a):
    A = a.a if os.path.isdir(a.a) else os.path.join(RUNS, a.a)
    B = a.b if os.path.isdir(a.b) else os.path.join(RUNS, a.b)
    bad = 0
    pa, pb = flash_file(A, 'make'), flash_file(B, 'make')
    if os.path.exists(pa) and os.path.exists(pb):
        da, db = open(pa, 'rb').read(), open(pb, 'rb').read()
        diff = [s for s in range(NSEC) if da[s * SECTOR:(s + 1) * SECTOR] != db[s * SECTOR:(s + 1) * SECTOR]]
        normal = [s for s in diff if s != 31]
        print('[%s] make.sav 일반 세이브 섹터(0~30) 바이트 %s' % ('PASS' if not normal else 'FAIL',
              '동일' if not normal else '차이: 섹터 %s' % normal))
        bad += bool(normal)
        print('[INFO] make.sav 섹터 31(녹화 기록, A안) %s' % ('동일' if 31 not in diff else '다름(형식이 바뀌면 기대)'))
    else:
        print('[INFO] make.sav 한쪽 없음 — 건너뜀')
    pa, pb = flash_file(A, 'newgame'), flash_file(B, 'newgame')
    if os.path.exists(pa) and os.path.exists(pb):
        da, db = open(pa, 'rb').read(), open(pb, 'rb').read()
        diff = [s for s in range(NSEC) if da[s * SECTOR:(s + 1) * SECTOR] != db[s * SECTOR:(s + 1) * SECTOR]]
        print('[%s] newgame.sav(새 게임 + 파티·박스) 섹터 바이트 %s (기준 %s)' % ('PASS' if not diff else 'FAIL',
              '동일' if not diff else '차이: 섹터 %s' % diff, os.path.basename(pa)))
        bad += bool(diff)
    else:
        print('[INFO] newgame.sav 한쪽 없음 — 건너뜀')
    la = open(os.path.join(A, 'load.txt')).read().splitlines() if os.path.exists(os.path.join(A, 'load.txt')) else []
    lb = open(os.path.join(B, 'load.txt')).read().splitlines() if os.path.exists(os.path.join(B, 'load.txt')) else []
    if la and lb:
        ra = [l for l in la if 'RECORDED_BATTLE_VALID' in l]
        rb = [l for l in lb if 'RECORDED_BATTLE_VALID' in l]

        def resave(lines):
            hs = []
            for l in lines:
                if l.startswith('DUMP RESAVE_SECTOR_FNV@'):
                    hs += l.split()[2:]
            return hs
        ha, hb = resave(la), resave(lb)
        keep = lambda l: 'RECORDED_BATTLE_VALID' not in l and not l.startswith('DUMP RESAVE_SECTOR_FNV@')
        fa = [l for l in la if keep(l)] + ['DUMP RESAVE_SECTOR_FNV[0..30] ' + ' '.join(ha[:31])]
        fb = [l for l in lb if keep(l)] + ['DUMP RESAVE_SECTOR_FNV[0..30] ' + ' '.join(hb[:31])]
        print('[INFO] 다시 저장한 섹터 31(녹화 기록, 입력 이미지 그대로) 해시 %s' % ('동일' if ha[31:] == hb[31:] else '다름(입력 이미지의 녹화 기록 형식이 다르면 기대)'))
        if fa == fb:
            print('[PASS] load.txt 불러오기 결과 %d줄 동일 (파티 %d·박스 몬 %d·다시 저장 섹터 해시 포함)'
                  % (len(fa), sum(1 for l in fa if l.startswith('DUMP PRAW')),
                     sum(1 for l in fa if l.startswith('DUMP BRAW'))))
        else:
            import difflib
            d = list(difflib.unified_diff(fa, fb, a.a, a.b, lineterm='', n=0))
            print('[FAIL] load.txt 차이 %d줄:' % sum(1 for x in d if x[:1] in '+-' and x[:3] not in ('+++', '---')))
            for x in d[:60]:
                print('    ' + x[:220])
            bad += 1
        print('[INFO] 녹화 기록 유효: %s → %s' % (ra[0].split()[2] if ra else '?', rb[0].split()[2] if rb else '?'))
    else:
        print('[INFO] load.txt 한쪽 없음 — 건너뜀')
    print('판정: %s' % ('FAIL' if bad else 'PASS'))
    return 1 if bad else 0


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest='cmd', required=True)
    p = sub.add_parser('run')
    p.add_argument('tree')
    p.add_argument('label')
    p.add_argument('--image')
    p.add_argument('--image-name')
    p.add_argument('--jobs', type=int, default=hnsverify_paths.default_jobs(), help='기본 $HNS_JOBS 또는 nproc')
    p = sub.add_parser('compare')
    p.add_argument('a')
    p.add_argument('b')
    p = sub.add_parser('reparse')
    p.add_argument('dir')
    p = sub.add_parser('image')
    p.add_argument('sav')
    p.add_argument('out')
    a = ap.parse_args()
    if a.cmd == 'run':
        sys.exit(cmd_run(a))
    if a.cmd == 'compare':
        sys.exit(cmd_compare(a))
    if a.cmd == 'reparse':
        sav, nsav, savn, nsavn, dump, info, results = parse_log(os.path.join(a.dir, 'make.log'))
        open(os.path.join(a.dir, 'load.txt'), 'w').write('\n'.join(dump) + ('\n' if dump else ''))
        print('%s: DUMP %d줄' % (a.dir, len(dump)))
    if a.cmd == 'image':
        print(make_image_header(a.sav, a.out))


if __name__ == '__main__':
    main()
