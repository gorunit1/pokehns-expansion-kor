#!/usr/bin/env python3
"""HnS 재확인 114 보조 검사: 이벤트 야생 더블배틀은 모두 포획 금지(FLAG_NO_WILD_CATCHING)여야 한다.

훔친 야생 도구 대기 목록(HNS_STOLEN_WILD_ITEMS_MAX = 8)의 정상 플레이 상한 근거 중 하나.
야생 더블에서는 야생 2마리의 도구 + 파티 6개 = 8개까지 훔칠 수 있다. 볼을 던질 수 있으면 볼 줍기(실패한 볼을
주움, 배틀당 한 번)로 1개가 더 생겨 9개가 될 수 있으므로, 더블 이벤트는 볼을 던질 수 없어야 한다.
(무작위 야생 더블은 설정으로 꺼져 있음 — test/hns_stolen_item_supply.c의 (data) 테스트가 확인.)

data/ 아래 *.inc에서 두 번째 종을 지정한 setwildbattle / setwildbossbattle을 찾아, 다음 dowildbattle 전까지
setflag FLAG_NO_WILD_CATCHING이 있는지 확인한다. 저장소 루트에서 실행: python3 dev_scripts/hns_verify/check_wild_doubles.py
종료 코드 0 = 통과, 1 = 포획 금지가 없는 더블 이벤트 있음(목록 출력), 2 = 더블 이벤트를 하나도 못 찾음(검사 자체 이상).
"""
import pathlib
import re
import sys

SETWILD = re.compile(r'^\s*(setwildbattle|setwildbossbattle)\b(.*)$')
DOWILD = re.compile(r'^\s*dowildbattle\b')
NOCATCH = re.compile(r'^\s*setflag\s+FLAG_NO_WILD_CATCHING\b')


def is_double(cmd, args):
    if 'species2' in args:
        return not re.search(r'species2\s*=\s*SPECIES_NONE\b', args)
    if cmd == 'setwildbattle':
        # 위치 인자: species, level, item, species2, level2, item2
        parts = [p.strip() for p in args.split(',')]
        return len(parts) >= 4 and parts[3].startswith('SPECIES_') and parts[3] != 'SPECIES_NONE'
    return False


def main():
    root = pathlib.Path('data')
    found, bad = [], []
    for path in sorted(root.rglob('*.inc')):
        lines = path.read_text(encoding='utf-8', errors='replace').splitlines()
        for i, line in enumerate(lines):
            m = SETWILD.match(line)
            if not m or not is_double(m.group(1), m.group(2)):
                continue
            where = f'{path}:{i + 1}'
            found.append(where)
            ok = False
            for nxt in lines[i + 1:]:
                if DOWILD.match(nxt):
                    break
                if NOCATCH.match(nxt):
                    ok = True
            if not ok:
                bad.append(where + ': ' + line.strip())
    print(f'scripted wild doubles: {len(found)}')
    for where in found:
        print('  ' + where)
    if not found:
        print('FAIL: no scripted wild double found (check the scanner)')
        return 2
    if bad:
        print('FAIL: scripted wild double without FLAG_NO_WILD_CATCHING before dowildbattle:')
        for b in bad:
            print('  ' + b)
        return 1
    print('PASS: every scripted wild double sets FLAG_NO_WILD_CATCHING')
    return 0


if __name__ == '__main__':
    sys.exit(main())
