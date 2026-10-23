# full-sync 묶음 32 — seq 396~399

- 요청: 사용자 "ㄱㄱ 혼자 되는지 보자" → 사전 분석(재확인 목록 대조 — 해당 항목 없음, 전부 혼자 가능) → "ㄱㄱ, 혼자 할 수 있는 건 다 끝내고 보고 메시지는 마지막에 한 번에"(2026-10-23).
- upstream: 읽기 전용 사본 `/home/user/pokeemerald-expansion-upstream`에서 `git format-patch`로 읽었다.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 396 | #10258 | `babcb780` | (게임 코드는 HnS에 이미 있음) 테스트만 | 32,963,732 |
| 397 | #9969 | `f6d6e037` | 디버그 메뉴 랜더마이저 시험 2항목 | 32,967,012 |
| 398 | #10274 | `dcf82156` | 애니 테스트 수정(`gLoadFail`을 `TESTING` 조건으로) | 32,967,012 |
| 399 | #9265 | `2c45bf17` | 테스트만 | 32,967,012 |

이식 전(`a512bb9b`) 32,963,732 B → **32,967,012 B(+3,280, 디버그 메뉴)**, EWRAM 250,408·IWRAM 25,516 같음, SHA1 **`b0910991d37c0f5ea5f16864798a7048403a037d`**.

## PR별 내용과 HnS 판단

- **396 #10258:** AI가 반동기로 자기 포켓몬을 쓰러뜨리지 않게 하는 판단. 게임 코드는 2026-09-15 선별 이식분으로 이미 있어(패치의 `IsRecoilDamageEffect`는 두 번째 정의가 됨) 테스트 1개만(PASS). 테스트 줄 끝 공백 1개 삭제.
- **397 #9969:** 디버그 메뉴 Utilities에 "Test Species Randomizer"·"Test Item Randomizer"(옵션 번호·필터 인자를 고르면 결과 24개, A 다시·B 끝). HnS는 `DEBUG_OVERWORLD_MENU TRUE`라 R+START 디버그 메뉴에 보인다. HnS 목록의 목호·실버 멀티 항목 유지.
  - **새 영어 텍스트(정책 5 — 디버그, 영어 그대로):** `Test Species Randomizer`, `Test Item Randomizer`, `Arg {STR_VAR_1}: {STR_VAR_2}`, `Options ID: {STR_VAR_1}` 및 결과 창 문구.
- **398 #10274:** `gLoadFail` 설정·초기화가 `T_SHOULD_RUN_MOVE_ANIM` 대신 `TESTING` 조건(선언은 이미 `TESTING` 안). 애니 테스트 재작성. 게임 빌드 무관, 무거운 애니 테스트는 여전히 꺼짐. `sprite.c`는 HnS 반환값(`TAG_NONE`) 문맥이라 손 적용.
- **399 #9265:** 테스트 3개 추가. 충전 1개 PASS, 필드 2개는 영어 `MESSAGE`라 FAIL(같은 파일의 기존 테스트와 같은 이유).
- 게임 동작 변화 0, 새 배틀 문장 0, 세이브 무관.

## 검증

- **빌드:** 커밋마다 `make hns -j8` 종료 0, 새 경고 0(`warncheck.sh`).
- **세이브 정적 비교**(이식 전 사실 = 세션 임시 `b33/save-pre`, 코드 `a512bb9b`를 클론에서 빌드 — ROM 32,963,732 B로 같음): **PASS(FAIL 0, WARN 0)**.
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 `2c45bf17`): 러너 PASSED 2,759 / TOTAL 5,735(KNOWN_FAILING 25), 목록 — `test-baseline-seq393.txt` 대비 **사라진 PASS 0**, 새 PASS 2(반동 AI, 충전), 새 FAIL 2(필드 테스트 영어 문장). 새 기준 [`test-baseline-seq399.txt`](test-baseline-seq399.txt)(**다음 비교 기준**).
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-b32 8`, 코드 `2c45bf17`): **558/652, 기대와 바이트 같음, `판정: PASS (기대와 같음)`**(`HNSB2021` 30/30 포함).
- **실기: 하지 않음(친구).**

## 실기(mGBA) 후보 — 친구

1. (원하면) R+START 디버그 메뉴 → Utilities → Test Species/Item Randomizer 화면.

## 재확인·뒤 PR 메모

- 새 재확인 항목 없음. 순서표 396~399 기록.
- 다음은 seq 400 #9440(재확인 51).
