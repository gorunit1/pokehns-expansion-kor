# full-sync 묶음 57 — seq 490 (#10593 `SetMoveEffect`를 함수 표로, 새 파일 `battle_set_effect.c`)

- 요청: 사용자 "ㄱㄱ 490 시작해 혼자해도 되는거라면"(2026-10-28).
- upstream: 읽기 전용 사본, #10593 `1242c88a10`(9파일, +1716/−1444, XL). 단위 `U-setmoveeffect-10593`.
- 혼자 진행한 근거: 동작을 바꾸지 않는 리팩터(새 문장 0). 기존 함수 안의 HnS 줄을 새 파일로 옮기는 일이며, 이식 전 코드와 같은 출력인지 한글 테스트·전체 테스트·한글 회귀 세트로 확인.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 490 | #10593 | `98b25dbc` | 거대한 `SetMoveEffect` switch를 효과별 처리 함수 표로(`src/battle_set_effect.c`), `struct SetEffect`는 `include/battle_set_effect.h`, `enum MoveEffect`는 `include/constants/battle_set_effect.h`로, HnS 줄 6종 옮김, 한글 회귀 테스트 | 32,962,740 |

이식 전(`e7cb895f`, 게임 코드 = `1f406399`) 32,961,428 B → **32,962,740 B(+1,312)** — 함수 표·효과별 함수.

## 내용과 HnS 판단

- **적용 방법:** 파일별 3-way 병합(`git merge-file`). 충돌 2곳은 옮겨지는 영역 안에 HnS 줄이 있어서 생긴 것 — 지워지는 쪽(upstream 형태)으로 정리하고 HnS 줄은 새 파일에 다시 넣음.
  - 옮기기 전 함수를 upstream 이전 형태와 HnS 형태로 뽑아 비교해 HnS 줄을 모두 찾음(`SetMoveEffect` 1,138 ↔ 1,143줄, `SetMoveEffectHelper` 같음). `enum MoveEffect`·`struct SetEffect` 정의는 HnS가 바꾼 줄이 없어 병합이 깨끗함(HnS 고유 효과 없음 — 함수 표에 빠지는 효과 없음).
- **새 파일로 옮긴 HnS 줄:**
  1. 파트너 판정 `BATTLE_PARTNER` → `GetPartnerBattler` 4곳(능력 변화 파트너, 불꽃튀기기, 페인트의 와이드가드 등, 드래곤옐 급소 단계).
  2. 반동 턴 생략 HnS 챌린지 옵션 `gSaveblock3.challengeSettings.genOneRecharge`(새 파일에 `load_save.h` include 1줄, `// HnS:`).
  3. 수면 클로즈 해제 `TryDeactivateSleepClause(effectBattler, …)`(HnS 인자는 배틀러).
  4. 내던지기 열매 vs 인분·방진고글 주석 1줄.
  5. 깨뜨리다·사이코팽 등의 방벽 해제 문장 `B_MSG_BREAK_*` 비트 + `gBattleStruct->removedScreens[side]` + 타이머 0(HnS 방벽 문장).
  - 불태우기 vs 점착은 HnS가 조건을 한 줄로 묶어 둔 형태였는데 upstream 새 형태(점착이면 먼저 끝냄)와 동작이 같아 upstream 형태로.
- **`include/battle_util.h`:** upstream은 `struct SetEffect` 블록만 지움. HnS는 그 자리에 `SleepClauseBlock`·`EjectPackTiming`·`WeatherFailure` 열거형을 둬서(이전 이식 때 옮김) 열거형은 유지.
- 병합 뒤 "upstream 변경 줄 ↔ HnS 변경 줄" 대조: 남는 차이는 위 HnS 줄과 include 1줄뿐.
- 세이브: 배틀 코드만. `include/move.h`에 include 1줄 → `BoxMonRestorePP` assert 줄 번호 상수만 바뀜.

## 검증

- **빌드:** `make hns -j8` 종료 0(첫 빌드는 새 파일의 `gSaveblock3` 선언 없음 → `load_save.h` include), 전체 재빌드(`build/hns` 지우고) 로그로 새 컴파일 경고 0(`warncheck.sh`).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `98b25dbc`): `test-baseline-seq489.txt` 대비 **새 PASS 3(한글 3), 사라진 PASS 0**, 다른 줄 같음, INVALID 24(이전 실행과 같은 목록). 새 기준 [`test-baseline-seq490.txt`](test-baseline-seq490.txt)(**다음 비교 기준**).
- **새 한글 회귀 `test/hns_10593_set_effect.c` 3개:** 깨뜨리다가 리플렉터·빛의장막을 각각 문장으로(`상대의 리플렉터가 없어졌다!`·`상대의 빛의장막이 없어졌다!`), 더블 불꽃튀기기의 파트너 피해(`분출하는 불꽃이 상대 마자에게 명중했다!`), 불태우기 vs 점착(열매 그대로 / 해감액이면 `상대 꼴깍몬의 오랭열매는 녹여 버렸다!`): **3/3 PASS**.
  - 이식 전 코드(클론 `e7cb895f`, `ZZ old` 사본, 커밋 안 함·지움): **3/3 PASS**(출력 같음 — 의도대로).
- **세이브 정적 비교**(이전 = 클론 `e7cb895f` 빌드, ROM SHA1 `be0cfe0d…` = 묶음 56 기록과 같음): **PASS(FAIL 0, WARN 2)** — `BoxMonRestorePP` assert 줄 번호 상수 245 → 246(`move.h` include 1줄, 검토 묶음 1건). 세이브 블록 크기·섹터 배치·EWRAM/IWRAM 같음.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s490 8`): **555/652, `판정: PASS (기대와 같음)`, 요약 바이트 같음**(`HNSSCREEN` 15/15 — 방벽 해제 문장 포함).
- **ROM**(커밋 `98b25dbc`): SHA1 `ebed1c440bc77cee64cd107cab4eac291847d0c6`, 사용 32,962,740 B(파일 33,554,432 B), 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 하지 않음(친구).** 후보(기대: 이식 전과 같은 화면) — 추가 효과가 있는 기술 전반(상태이상·풀죽음·능력 변화·도구 빼앗기·불태우기·깨뜨리다·불꽃튀기기), 반동 턴 생략 챌린지 옵션을 켠 상태의 파괴광선.

## 다음

- seq 491 #10630(압류·회복봉인 중복 플래그 정리). 목표 1.17.1.
