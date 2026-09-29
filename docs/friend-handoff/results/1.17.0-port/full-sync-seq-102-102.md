# full-sync 실제 port 결과 — seq 102

완료: seq 102 #9172 이식·바이너리 검증·기록 완료(다음 구간은 seq 103 #9568부터).

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), [`CLAUDE_FULL_SYNC_PORT_PROMPT.md`](../../CLAUDE_FULL_SYNC_PORT_PROMPT.md), group plan [`g3_battle_changes_moves_abilities_items_plan.md`](../1.17.0-sync-plan/g3_battle_changes_moves_abilities_items_plan.md)(g3 #9172 행)
시작 HEAD: `e84eb9f4d2` (작업 트리 clean)

## seq 102 공통 사항

- 빌드 명령: `GITHUB_ACTION=1 make hns -j8 > build/port.log 2>&1`. 이 컴퓨터(데스크톱 WSL)에는 `/opt/arm-gnu-toolchain-13.2.Rel1…`이 없어 `/usr/bin/arm-none-eabi-*`(GNU as 2.42)를 썼다.
- 기준 빌드(`e84eb9f4d2`, 증분 빌드): 종료 코드 0, **ROM 32,714,868 B(97.50%), EWRAM 248,924 B(94.96%), IWRAM 25,516 B(77.87%)**. `pokehns.gba` SHA-1 `3613568d3329886ca25b4f54148bf2c5d267e275`, `build/hns/data/battle_anim_scripts.o` SHA-1 `42a878fb195f9b93ccb3665a755b76682138b32d`. 두 파일과 `pokehns.elf`를 스크래치에 복사해 두고 비교에 썼다.
- 테스트 기준: [`test-baseline-seq101.txt`](test-baseline-seq101.txt)의 같은 이름 줄.
- 스크래치(저장소 밖, `hns-sync-work/seq102/`):
  - `convert.py`: 변환 스크립트(아래 규칙). `python3 convert.py IN OUT`.
  - `analyze.py`: upstream 전후 파일에서 (이전 → 이후) 16진수 토큰 쌍을 모으는 분석 도구. `pairs.json`이 결과(318쌍).
  - `upstream_before.s`/`upstream_after.s`/`upstream_repro.s`: upstream `48bf615a67^`/`48bf615a67` 파일과 재현 결과.
  - `hns_before.s`/`hns_after.s`/`hns_final.s`: HnS 변환 전, 스크립트 결과, 커밋한 파일.
  - `pokehns_before.gba`·`pokehns_before.elf`·`bas_before.o`·`bas_test_before.o`: 변환 전 빌드 산출물.
  - `p<PR>.diff`/`p9924.rej`: 후속 행 적용 시험(아래 "후속 행 메모").

## 동기화 단위: seq 102 #9172 `U-9172` Converts move animation hex numbers to decimal

- 현재 판정: 적용(upstream 규칙으로 HnS 파일 전체를 다시 변환)
- 커밋: `b72fd0c63d`
- upstream 근거: `48bf615a67`(부모 `ddc169b8fe`), `data/battle_anim_scripts.s` 한 파일 4,399줄(git diff 기준. 줄 대 줄로는 4,380줄이 바뀌고 줄 수는 그대로다).
- 해결한 의존성: #9142(`125e893903`), #8497(`f59f50ca17`), #9063(`da2c827f0e`)이 이미 들어가 있다. 이 셋 덕분에 HnS 파일이 upstream 부모 파일과 18줄(HnS 고유 변경)만 다르다.
- 수정 파일(1): `data/battle_anim_scripts.s`

### 변환 규칙 (upstream diff에서 도출)

upstream 전후 파일은 줄 수가 같다(35,604줄). 바뀐 4,380줄 가운데 4,375줄은 16진수 토큰 자리만 바뀌었다. 토큰 17,269개, 서로 다른 (이전 → 이후) 쌍 318개를 모두 분류했다. 변환 뒤 파일에는 16진수가 하나도 남지 않는다(주석 포함).

| 규칙 | 내용 | upstream 건수 |
|---|---|---|
| R1 | 식별자·숫자에 붙지 않은 `0x…` 토큰을 10진수로 바꾼다. 대소문자(`0xffDF`·`0xFFFF`)와 앞자리 0(`0x0000`·`0x0a`·`0x04E0`)은 가리지 않는다. `0X`와 16비트를 넘는 값은 없다. | 토큰 17,269 |
| R1-부호 | 값이 `0x8000 < v ≤ 0xFFFF`이면 16비트 부호값 `v - 0x10000`으로 쓴다. 인자 자리나 매크로와 상관없이 값으로만 정한다. `0x8000` 자체와 그보다 작은 값은 양수로 둔다(`0x8000` → `32768`, `0x8003` → `-32765`, `0xF000` → `-4096`, `0xFFF` → `4095`, `0xff` → `255`). | 음수 2,326 |
| R1-주석 | `@` 주석 안과 `@`로 시작하는 주석 줄의 16진수도 같은 규칙으로 바꾼다. 앞의 `-` 부호는 그대로 둔다(`@-0x1000` → `@-4096`, `@-0x2aa` → `@-682`, `(0x50 (time), -0x400 …)` → `(80 (time), -1024 …)`, `@setblends 0x80C` → `@setblends 2060`). | — |
| R2 | 16진수 바로 뒤에 `@`가 붙어 있으면 사이에 탭을 넣는다(`scenery=0x101@thunder flash` → `scenery=257\t@thunder flash`). | 4줄 |
| R3 | `setarg A B`처럼 두 인자 사이에 콤마가 없으면 콤마를 넣는다(`setarg 0x7 0xffff` → `setarg 7, -1`). `jumpargeq 0x7 0x1 X`, `loopsewithpan …, SOUND_PAN_ATTACKER 0x7 0x12`처럼 콤마가 빠진 다른 매크로 인자는 숫자만 바꾸고 콤마는 넣지 않는다. | 15줄 |
| X1 | 규칙 밖 수동 예외 1건. `gGuardianOfAlolaDirtGeyserSpriteTemplate` 줄의 꼬리 주석 `\t@ -4, -0x10`을 지웠다. 같은 주석이 붙은 BloomDoom·GigavoltHavoc 줄은 R1대로 `@ -4, -16`으로 남았다. | 1줄 |

- 16진수로 남긴 경우: 없다. 비트마스크·팔레트 플래그 자리의 값(예: `0x100D`, `0x0120`, `0x1902`)과 주석도 모두 10진수가 됐다. 레이블·식별자 안의 `0x`는 원래 없다.
- 부호 기준에 대한 근거: upstream에서 `0x8000`~`0xEFFF` 범위의 값은 `0x8000`(8건, 주석 처리된 SparkElectricityFlashing 줄 포함)과 `0x8003`(82건)뿐이다. 그래서 문턱이 `0x8000`과 `0x8003` 사이라는 것만 확정된다. 스크립트는 `v > 0x8000`으로 두었다. HnS 파일에도 `0x8001`·`0x8002`가 없어 결과에는 차이가 없다.
- 핵심 코드(`convert.py`):
  ```python
  HEX = re.compile(r'(?<![\w.])0[xX]([0-9a-fA-F]+)(?![\w.])')
  def dec(m):
      v = int(m.group(1), 16)
      if 0x8000 < v <= 0xFFFF:
          v -= 0x10000
      return str(v)
  # 줄마다: X1 → R2(hex 뒤 '@' 앞에 탭) → R3(setarg 콤마) → R1(HEX.sub(dec, line))
  ```

### upstream 재현

- `convert.py upstream_before.s`(= `48bf615a67^`) 결과가 `48bf615a67` 파일과 **바이트 동일**하다(SHA-1 `388ba2c3373d3fb100aee74296a9a6c12c8d19e8`). 바뀐 줄 4,380, 토큰 17,269, 음수 2,326, R2 4줄, R3 15줄, X1 1줄.

### HnS 적용

- HnS 파일(35,602줄)을 upstream 부모와 비교하면 차이는 16진수가 없는 18줄뿐이다(`call HealingEffect` 4곳 삭제, `end` 1줄 추가, 메가·알파·오메가 스톤 `AnimTask_BlendParticle` 3줄 추가). **HnS 고유 줄에는 16진수가 없어 규칙이 애매한 줄이 없었다.**
- 스크립트 결과: 바뀐 줄 **4,380**(git diff 4,399/4,399), 토큰 17,269, 음수 2,326, R2 4, R3 15, X1 1줄. upstream과 건수가 모두 같다. 변환 뒤 HnS 파일과 upstream `48bf615a67` 파일의 차이는 변환 전과 같은 18줄뿐이다.
- X1(GuardianOfAlola 주석 삭제)도 HnS에 똑같이 적용했다. 같은 줄이 있고, upstream 후속 PR의 문맥을 맞추려는 것이다.
- **upstream과 다르게 둔 곳 1줄:** `gBattleAnimMove_KowtowCleave`의 `createsprite gLeerSpriteTemplate, ANIM_TARGET, 2, 24, -12` 줄(16111행). 원래 줄 앞에 공백+탭(` \t`)이 있었다. 변환으로 이 줄이 바뀌면서 `git diff --check`가 "space before tab in indent"로 걸려, 앞 공백만 지웠다. upstream은 1.17.0 이후에도 이 공백을 그대로 두지만, 후속 행 #9676·#9924·#10374·#10401·#10421 가운데 이 줄을 건드리는 것은 없다. 같은 모양의 다른 Leer 줄 2개(17619·17660행)는 16진수가 없어 바뀌지 않았고 그대로 두었다.
- 한글 문자열·STRINGID·조사: 해당 없음. 비ASCII 변경 줄 0.

### 바이너리 동일성 검증

- `git diff --check`: 통과(위 Leer 줄 공백 정리 뒤).
- 변환 뒤 `GITHUB_ACTION=1 make hns -j8`: 종료 코드 0, 경고 0줄(로그 10줄, `battle_anim_scripts.s`만 다시 조립하고 링크). **ROM 32,714,868 B / EWRAM 248,924 B / IWRAM 25,516 B(모두 변화 없음).**
- **`pokehns.gba` SHA-1: 변환 전 `3613568d3329886ca25b4f54148bf2c5d267e275` → 변환 뒤 `3613568d3329886ca25b4f54148bf2c5d267e275`(동일).**
- **`build/hns/data/battle_anim_scripts.o` SHA-1: 전후 모두 `42a878fb195f9b93ccb3665a755b76682138b32d`.** `.o` 파일 전체가 같으므로 `script_data` 섹션(0x3b9c6 B) 바이트·재배치·기호표가 모두 같다.
- 테스트 빌드(`-DTESTING=1`) 오브젝트: `build/hns-test/data/battle_anim_scripts.o`가 변환 전 파일을 같은 명령(`preproc | cpp -DTESTING=1 | preproc -ie | as`)으로 조립한 결과와 SHA-1이 같다(`42a878fb…`, 릴리스 빌드 오브젝트와도 같다).
- 조건 조립: 이 파일의 `.if`는 `B_UPDATED_MOVE_DATA` 분기 2개뿐이고, 양쪽 가지 4줄 모두 16진수가 없어 변환 대상이 아니었다. 매크로 안의 `.if`는 인자 값에 따른 것이라 설정과 무관하다. 따라서 다른 `B_UPDATED_MOVE_DATA` 설정에서도 결과가 같다.

### 테스트

- `make check BUILD=hns -j6 TESTS="test/battle/move_animations/all_anims.c"`(변환 뒤): 6개 중 PASS 3 / FAIL 3.
  - 추출 목록 5줄이 `test-baseline-seq101.txt`의 같은 이름 줄과 상태까지 같다: `Move Animations work 1 222/222: FAIL`, `… 2 221/221: FAIL`(둘 다 기존 `Task_FreeAbilityPopUpGfx: task not freed`, seq 83 기록의 HnS 도구 팝업 태스크), `… 3: PASS`, `… 4: PASS`, `Tera Blast animations work: PASS`.
  - `Z-Moves animations work 17/37: INVALID`(`Cannot turn … into a Z-Move`)는 추출 정규식 밖 상태로, seq 83부터 기록된 HnS 페어리 설정 차이다(seq 92~101 문서 기록과 같음).
- 오브젝트와 ROM이 바이트 동일하므로 전체 실행은 하지 않았다.

### 남은 위험·실기 확인

- 남은 위험: 없음(ROM·오브젝트 바이트 동일).
- 실기 확인: 불필요.

## 후속 행 메모 (#9677·#9676·#9924·#10374·#10401·#10421·#10027)

변환 뒤 HnS 파일 사본에 upstream 후속 커밋의 `data/battle_anim_scripts.s` diff를 port 순서대로 `git apply --reject`로 적용해 보았다(스크래치 `applytest/`, 저장소는 건드리지 않음). #9677 `813e1c66c0`은 병합 커밋이라 첫 부모 기준 diff(`813e1c66c0^1..813e1c66c0`)를 썼다.

| 순서 | PR(seq) | upstream | 이 파일 hunk | 결과 |
|---|---|---|---|---|
| 1 | #10027(356) | `ea8dfcd695` | 2 | 그대로 적용 |
| 2 | #10421(425) | `4e87b5a291` | 1 | 그대로 적용(CorrosiveGas 템플릿 교체) |
| 3 | #9677(495) | `813e1c66c0`(병합) | 76 | 그대로 적용 |
| 4 | #9676(496) | `057a4095bd` | 24 | 그대로 적용 |
| 5 | #9924(497) | `1a35ede7d5` | 73 | **7개 거부**: FusionBolt, Charge, VoltTackle, IcePunch, Haze, GigavoltHavoc, StokedSparksurferFinish |
| 6 | #10374(498) | `58b806c6eb` | 127 | 그대로 적용(5의 거부분 제외 상태에서) |
| 7 | #10401(499) | `ab462dea6e` | 5 | 그대로 적용 |

- #9924의 거부 7개는 #9172와 무관하다. upstream에서 #9676과 #9924 사이에 들어온 pret 병합 #9986(`3ea7f0d71b`, **seq 516**)이 가져온 pret#2290 애니 매크로(`76799d84eb` "Added electric puff macro", `edc58c881f` "Added fist and foot templates")가 문맥 전제다. 시험 삼아 `76799d84eb`의 이 파일 diff를 #9924 앞에 넣으면 거부가 5개로 줄었다. `edc58c881f`는 #9676과 겹쳐 그대로는 17개가 거부된다. #9924(seq 497)를 옮길 때 #9986(seq 516)의 배틀 애니 매크로 부분을 먼저 가져올지, 거부 hunk를 수동으로 옮길지 정해야 한다.
- #10401은 group plan대로 체인 뒤라서 upstream 형태로 들어간다.
- 이후 이 파일을 건드리는 행도 이번과 같은 방식으로 바이너리를 비교할 수 있다. 기능 변화가 없는 행이면 `battle_anim_scripts.o` SHA-1이 같아야 한다.
