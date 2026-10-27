# full-sync 묶음 53 — seq 485 (#10488 승부굳히기 위력 2배 판정 정리)

- 요청: 사용자 "ㄱㄱ 485 시작해 혼자해도 되는거라면"(2026-10-27).
- upstream: 읽기 전용 사본, #10488 `34438b93ae`(8파일, +586/−72). 같은 단위(`U-10488`)에 다른 seq 없음.
- 혼자 진행한 근거: 새 문장 0, 한글 문장 변화 0. 바뀌는 것은 승부굳히기(Assurance) 위력이 2배가 되는 조건뿐이고 upstream 형태 그대로(HnS 보정·적응 0). HnS 트레이너 중 승부굳히기를 쓰는 포켓몬은 사천왕 카렌 재대전(`TRAINER_KAREN_2_HNS`)의 페르시온 Lv68 하나.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 485 | #10488 | `312792d9` | `datahpupdate`에 인자(`ASSURANCE_DOUBLE`/`ASSURANCE_IGNORE`, 57곳), 스스로 깎은 HP는 2배 조건에서 뺌, 아이스페이스가 막은 공격은 2배 조건, `TurnValuesCleanUp` 인자 이름, upstream 테스트 21개, 한글 회귀 테스트 | 32,961,380 |

이식 전(`9a6cb2a9`, 게임 코드 = `5fca546a`) 32,961,284 B → **32,961,380 B(+96)** — `datahpupdate`마다 1바이트, 판정 코드.

## 내용과 HnS 판단

- **적용 방법:** 파일별 3-way 병합(`git merge-file`). 충돌 1곳 — `BattleScript_IceBodyHeal`: HnS는 Champions 형태로 회복 문장(`STRINGID_ICEBODYHPGAIN`)을 뺐고 upstream은 같은 자리의 `datahpupdate`에 인자를 붙임 → HnS 형태(문장 없음) + 인자 `ASSURANCE_DOUBLE`. 병합 뒤 "upstream 변경 줄 ↔ HnS 변경 줄" 대조: 남는 차이는 `recoverbasedonsunlight` 한 줄의 끝 공백뿐(HnS에 원래 없음). 인자 없는 `datahpupdate`는 0(HnS 고유 스크립트에 `datahpupdate` 없음 — 모두 upstream과 같은 자리).
- **바뀐 판정(upstream 그대로):**
  - `datahpupdate … ASSURANCE_IGNORE`(2배 조건 아님): 대타출동·꼬리자르기, 아픔나누기(양쪽), 혼란 자해, HP를 깎는 기술(`BattleScript_PlayMoveAnimAndChangeHP` — 배북 등). 이전엔 HP가 줄면 무조건 2배.
  - 아이스페이스가 공격을 막아 대미지 0이어도 2배(`TryMoveDamageUpdate`).
  - 나머지 HP 감소(반동·생명의구슬·울퉁불퉁멧·까칠한피부·씨뿌리기·날씨·상태이상 등)는 `ASSURANCE_DOUBLE` — 이전과 같음. 회복 쪽 `ASSURANCE_DOUBLE`은 HP가 줄 때만 표시하므로 영향 없음.
- **화면 변화:** 문장·순서 변화 0 — 대미지 크기만(위 조건). `BATTLE_MESSAGE_OUTPUT_CHANGES.md`는 메시지 출력 문서라 행을 더하지 않음.
- 세이브: 배틀 스크립트 바이트·배틀 코드만, 세이브 구조 변화 없음.

## 검증

- **빌드:** `make hns -j8` 종료 0, 전체 재빌드(`build/hns` 지우고) 로그로 새 컴파일 경고 0(`warncheck.sh`).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `312792d9`): `test-baseline-seq394.txt` 대비 **새 PASS 26(upstream 20 + 한글 6), 사라진 PASS 0**. 없어진 줄 5개는 upstream이 바꾼 테스트 — 이름이 바뀐 생명의구슬 테스트 1개(이전 이름은 영문 문장 검사라 HnS에서 FAIL, 새 이름 PASS)와 지워진 TO_DO 4개(반동·점프 기술 빗나감·혼란·울퉁불퉁멧 → 실제 테스트로 바뀌어 PASS). INVALID 24(이전 실행과 같은 목록). 새 기준 [`test-baseline-seq485.txt`](test-baseline-seq485.txt)(**다음 비교 기준**).
- **새 한글 회귀 `test/hns_10488_assurance.c` 6개:** 배북·아픔나누기·대타출동·혼란 자해 뒤 2배 아님, 생명의구슬 반동 뒤 2배(이전과 같음), 더블에서 아이스페이스가 막은 뒤 2배 — 각 장면의 한글 문장(`상대 마자는 승부굳히기를 썼다!`, `마자용은 체력을 깎아서 풀 파워로 만들었다!`, `서로의 체력을 나누어 가졌다!`, `마자용의 대타가 나타났다`, `영문도 모른 채 자신을 공격했다!`)과 대미지 비교: **6/6 PASS**.
  - 이식 전 코드(클론 `9a6cb2a9`, `ZZ old` 사본, 커밋 안 함·지움): **1/6** — 생명의구슬만 PASS(바뀌지 않는 조건), 나머지 5개 FAIL = 의도한 변화.
  - 처음 쓴 아픔나누기 테스트(upstream처럼 양쪽 HP 같음)는 HP가 안 바뀌어 이식 전에도 PASS → HP를 800/200으로 바꿔 실제로 깎이게 함.
- **세이브 정적 비교**(이전 = 클론 `9a6cb2a9` 빌드, ROM SHA1 `677e531d…` = 묶음 52 기록과 같음): **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s485 8`): **555/652, `판정: PASS (기대와 같음)`, 요약 바이트 같음**.
- **ROM**(커밋 `312792d9`): SHA1 `7996553f14ccb6c14ad5097aac3e9ced0c06aaad`, 사용 32,961,380 B(파일 33,554,432 B), 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 하지 않음(친구).** 후보 — ① 카렌 재대전 페르시온의 승부굳히기(내 포켓몬이 대타출동·배북을 쓴 턴, 반동기를 쓴 턴), ② 내 포켓몬의 승부굳히기(상대가 대타출동을 쓴 턴 등). 빙큐보는 HnS 트레이너·야생 데이터에 없어 아이스페이스 경우는 일반 진행에 없음. 문장 변화는 없음.

## 다음

- seq 486 #10566(HP 바·결과 문장 정리, 재확인 97 정리). 목표 1.17.1.
