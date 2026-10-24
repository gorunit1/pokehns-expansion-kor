# full-sync 묶음 39 — seq 466~468

- 요청: 사용자 "혼자 할 수 있는 거라면 다 끝내고 보낼 보고 메시지는 마지막에 한 번에"(2026-10-23 밤, 사용자 취침 중 자율 진행)의 연속.
- upstream: 읽기 전용 사본 `/home/user/pokeemerald-expansion-upstream`, `git diff <커밋>^1 <커밋>`.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 466 | #9859 | `ec79adbc` | MoveEnd 처리기에 `BattleCalcValues`(공격자·대상·기술·기술 효과·배틀러별 특성·도구) 전달, 공격 취소 단계도 `cv.moveEffect` 채움 | 32,963,460 |
| 467 | #9841 | `ff86566b` | 블런더폴리시·용대미사일 테스트 KNOWN_FAILING 해제(테스트만) | 32,963,460 |
| 468 | #9898 | `4d9f0306` | `IsAnyTargetAffected`(전체·자신과 아군 대상), 편승(Opportunist) 원래 스피드 순, `GetDynamicMoveType` 타입 배열 | 32,963,556 |

이식 전(`1859ff22`) 32,963,828 B → **32,963,556 B(−272)**, EWRAM·IWRAM 같음.

## PR별 내용과 HnS 판단

### 466 #9859 — MoveEnd `cv`

- upstream 101 hunk 중 47개가 HnS 줄과 달라 그대로 안 붙는다. upstream 전·후 파일을 함수 이름으로 짝지어, **HnS 줄이 upstream 옛 줄과 같은 곳만** 같은 새 줄로 바꿨다(371줄). HnS에만 있거나 이미 더 뒤 upstream 형태인 줄은 전역 변수를 그대로 읽는다 — 처리기가 시작할 때 `cv`와 전역은 같은 값이다.
- 처리기가 대상을 바꾸고 같은 호출 안에서 계속 가는 곳은 `cv`도 같이 바꿨다: 용대미사일 파트너 전환(upstream과 같음), HnS 능력 변화 기술의 다음 대상 `CONTINUE` 경로(`MoveEndNextTarget`, HnS — upstream 1.17.0은 여기서 `cv`가 옛 대상으로 남는다).
- `MoveEndBeakBlast`(묶음 37에서 HnS 형태로 쓴 것)는 1.17.0 형태로. 매직코트·매직미러 반사 때 `targetsDone` 초기화가 원래 공격자 쪽이 됨(1.17.0과 같음).
- 결과: **전체 테스트 목록이 seq465 기준과 바이트 같음**(코드 `ec79adbc`), 세이브 PASS(WARN 0), 한글 회귀 558/652 기대와 같음.

### 467 #9841 — 테스트

- `Blunder Policy activates for Dragon Darts if one target misses…`가 KNOWN_FAILING → PASS.

### 468 #9898

- `IsAnyTargetAffected`: 전체 대상 기술은 모든 배틀러, 자신과 아군 대상 기술은 자기 편만 본다(Throat Spray 등 "기술이 누군가에게 효과" 판정). 편승(Opportunist)은 원래 스피드 순.
- HnS `GetDynamicMoveType`은 시그니처(특성·도구 인자)와 HnS 리베레이션댄스 갈래를 유지하고 `type1~3`만 배열로 — 결과 같음(테라·날개쉬기는 같은 검사 앞에서 처리).
- 테스트(Revelation Dance, Opportunist, Throat Spray, Coaching, Aromatic Mist) 결과 같음.

## 검증

- **빌드:** 커밋마다 `make hns -j8` 종료 0, 새 경고 0.
- **466 단독**(코드 `ec79adbc`): 전체 테스트 목록이 `test-baseline-seq465.txt`와 바이트 같음, 세이브 정적 PASS(FAIL 0·WARN 0, 이식 전 사실 `1859ff22` 클론 빌드), 한글 회귀 558/652 기대와 같음.
- **묶음 끝**(코드 `4d9f0306`): 러너 PASSED 2,788 / TOTAL 5,764(KNOWN_FAILING 12) — seq465 기준과 다른 줄은 467의 블런더폴리시 1줄(KNOWN_FAILING → PASS)뿐, 새 기준 [`test-baseline-seq468.txt`](test-baseline-seq468.txt)(**다음 비교 기준**). 세이브 정적 PASS(FAIL 0·WARN 0). 한글 회귀 상태 줄 차이 0·558/652 `판정: PASS (기대와 같음)`.
- ROM 32,963,556 B, EWRAM 250,404 B·IWRAM 25,516 B, SHA1 **`a4348114a6338002d11825467f6ca113091bf1a1`**.
- **실기: 하지 않음(친구).**

## 실기(mGBA) 후보 — 친구

- 동작 변화가 없는 리팩터라 별도 항목 없음. 평소 배틀(연속기·전체기·방어·도구 발동)에서 이상한 점이 있으면 알려 주면 된다.

## 다음

- seq 469 #9916(배틀 구조체·스크립트 정리 — 노려보기 문장 토큰 `B_ATK`/`B_DEF` → `B_EFF`/`B_SCR`, 정책 74 대상, 6파일 충돌). 이어서 470 #9939(XL, 명중 판정을 취소 단계로).
