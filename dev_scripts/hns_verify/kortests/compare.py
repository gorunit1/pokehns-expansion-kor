#!/usr/bin/env python3
"""한글 회귀 요약 두 개를 비교한다(옛 hnsfix-1007b/kortests-final/tools/cmp_expected.py 일반화).

usage: compare.py <run-summary> [expected-summary]   (기본 기대: kortests/expected/summary.txt)

판정은 테스트별 상태 줄("HNS… : PASS/FAIL/INVALID …")과 "- Tests …" 합계 줄로 한다(-j와 무관).
"  - test/<파일>:<줄>: …" 줄은 hydra가 실패 앞 50개만 runner 순서대로 적는 것이라 -j에 따라 달라져 판정에 쓰지 않고,
파일 전체가 바이트 같은지만 따로 알린다(기대 요약은 -j8로 만들었다).
종료 코드 0 = 상태·합계 같음, 1 = 다름.
"""
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
STATUS = re.compile(r'^(HNS.*): ([A-Z_]+)$')


def load(p):
    st, totals = {}, []
    with open(p, encoding='utf-8', errors='surrogateescape') as f:
        for line in f:
            line = line.rstrip('\n')
            m = STATUS.match(line)
            if m:
                st[m.group(1)] = m.group(2)
            elif line.startswith('- Tests'):
                totals.append(re.sub(r'\s+', ' ', line.split('Add TESTS')[0]).strip())
    return st, sorted(totals)


def main():
    if len(sys.argv) < 2:
        sys.stderr.write(__doc__)
        return 2
    run = sys.argv[1]
    exp = sys.argv[2] if len(sys.argv) > 2 else os.path.join(HERE, 'expected', 'summary.txt')
    r, rt = load(run)
    e, et = load(exp)
    same_bytes = open(run, 'rb').read() == open(exp, 'rb').read()
    print('run      PASS %d / %d' % (sum(v == 'PASS' for v in r.values()), len(r)))
    print('expected PASS %d / %d' % (sum(v == 'PASS' for v in e.values()), len(e)))
    sets = []
    for k in sorted(set(r) | set(e)):
        s = k.split(' ')[0]
        if s not in sets:
            sets.append(s)
    for s in sets:
        rk = [k for k in r if k.split(' ')[0] == s]
        ek = [k for k in e if k.split(' ')[0] == s]
        print('  %-9s run %3d/%3d  expected %3d/%3d' % (s, sum(r[k] == 'PASS' for k in rk), len(rk),
                                                     sum(e[k] == 'PASS' for k in ek), len(ek)))
    diff = [k for k in sorted(set(r) | set(e)) if r.get(k) != e.get(k)]
    print('status lines that differ from expected: %d' % len(diff))
    for k in diff:
        print('  DIFF %s: run %s / expected %s' % (k, r.get(k, '(missing)'), e.get(k, '(missing)')))
    if rt != et:
        print('totals differ: run %s / expected %s' % (rt, et))
    print('whole summary file byte-identical to expected: %s' % ('yes' if same_bytes else
          'no (if the status lines are the same, only the hydra "  - test/" failure list differs, e.g. other -j)'))
    ok = not diff and rt == et
    print('판정: %s' % ('PASS (기대와 같음)' if ok else 'FAIL (기대와 다름)'))
    return 0 if ok else 1


if __name__ == '__main__':
    sys.exit(main())
