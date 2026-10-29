# full-sync 묶음 77 — seq 511 (#9807 배틀 테스트 실패 제안)

- 요청: 사용자 "ㄱㄱ 511 시작해 혼자해도 되는거라면"(2026-10-29).
- upstream: 읽기 전용 사본, #9807 `e656b9b824`(1.16.2, 4파일 +295/−40). 단위 `U-runnerinfra-10155`(seq 510 다음, 다음 #10390 = seq 512).
- 혼자 진행한 근거: 테스트 러너만(게임 코드 0). 판정 기준 = ROM 바이트 같음 + 테스트 목록·한글 회귀 상태 같음.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 511 | #9807 | `ee7fb48e` | 장면 검사(`MESSAGE`·`HP_BAR`·`ANIMATION`·`ABILITY_POPUP` 등)가 안 맞으면 기대한 값과 가장 가까운 실제 이벤트를 알려 주는 출력, `TEST_RESULT_FLAKY`, `queuedEventsFailIndex` | 32,980,692(같음) |

이식 전(`fa537389`, 게임 코드 = `139c9931`) SHA1 `e71242c8…` → 이식 후 **SHA1 `e71242c8…` 같음(바이트 같음)**.

## 내용과 HnS 판단

- `include/test/battle.h`·`include/test_result.h`·`test/test_runner.c`는 충돌 0.
- **`test_runner_battle.c` 충돌 1곳:** upstream이 `TearDownBattle`을 파일 위쪽으로 옮겼고(본문 그대로), HnS는 그 함수에 `DeactivateAllTextPrinters()`(테스트 사이 텍스트 프린터 정리, 주석 포함)를 더해 둔 상태 → 옮겨 간 함수에 HnS 줄을 다시 넣고 옛 자리는 upstream처럼 지움. 함수 본문이 이식 전 HnS와 같음(비교 확인).
- HnS가 더한 장면 이벤트 `ITEM_POPUP`은 제안 대상 목록에 없어(upstream도 `CATCH_CHANCE`·`EFFECTIVENESS`는 없음) 실패 때 제안 없이 기존처럼 실패만 알림.
- 실패 테스트 하나로 새 출력 확인: `Unmatched MESSAGE("Wobbuffet used Max Strike!")`처럼 기대 문장이 붙음.
- 세이브: 영향 없음.

## 검증

- **빌드:** `make hns -j8` 종료 0, SHA1 `e71242c8…` = 이식 전과 같음. 저장소 전체 재빌드(`build/hns` 지우고): FULLBUILD_RESULT
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `ee7fb48e`): CHECK_RESULT
- **한글 테스트:** 새로 쓰지 않음(러너만).
- **세이브 정적 비교**(이전 = 클론 `fa537389` 빌드): SAVE_RESULT
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s511 8`): KOR_RESULT
- **ROM**(커밋 `ee7fb48e`): SHA1 `e71242c84ea01e72a8e58228244957fab6af1372`(묶음 75·76과 같은 파일), 사용 32,980,692 B, 파일 33,554,432 B, 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 새 후보 없음**(ROM 바이트 같음).

## 다음

- seq 512 #10390(알려진 실패 `and 0 more` 표시·`assertf %c/%C`). 목표 1.17.1.
