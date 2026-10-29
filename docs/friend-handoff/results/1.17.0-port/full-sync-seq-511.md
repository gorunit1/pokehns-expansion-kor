# full-sync 묶음 77 — seq 511 (#9807 배틀 테스트 실패 제안) + seq 513 (#10537 앞당김)

- 요청: 사용자 "ㄱㄱ 511 시작해 혼자해도 되는거라면"(2026-10-29).
- upstream: 읽기 전용 사본, #9807 `e656b9b824`(1.16.2, 4파일 +295/−40). 단위 `U-runnerinfra-10155`(seq 510 다음, 다음 #10390 = seq 512).
- 혼자 진행한 근거: 테스트 러너만(게임 코드 0). 판정 기준 = ROM 바이트 같음 + 테스트 목록·한글 회귀 상태 같음.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 511 | #9807 | `ee7fb48e` | 장면 검사(`MESSAGE`·`HP_BAR`·`ANIMATION`·`ABILITY_POPUP` 등)가 안 맞으면 로그를 켜고 한 번 더 돌려 기대값·가까운 실제 이벤트를 알려 주는 출력, `TEST_RESULT_FLAKY`, `queuedEventsFailIndex` | 32,980,692(같음) |
| 513(앞당김) | #10537 | `e4cd4538` | `ResetTestInventory`를 `StartBattle` 안으로(재실행 때 가방 다시 채움), `ghost.c` `GIVE_PLAYER_ITEM`, 러너 테스트 1개 | 같음 |

이식 전(`fa537389`, 게임 코드 = `139c9931`) SHA1 `e71242c8…` → 이식 후(`e4cd4538`) **SHA1 `e71242c8…` 같음(바이트 같음)**.

## 내용과 HnS 판단

- `include/test/battle.h`·`include/test_result.h`·`test/test_runner.c`는 충돌 0.
- **`test_runner_battle.c` 충돌 1곳:** upstream이 `TearDownBattle`을 파일 위쪽으로 옮겼고(본문 그대로), HnS는 그 함수에 `DeactivateAllTextPrinters()`(테스트 사이 텍스트 프린터 정리, 주석 포함)를 더해 둔 상태 → 옮겨 간 함수에 HnS 줄을 다시 넣고 옛 자리는 upstream처럼 지움. 함수 본문이 이식 전 HnS와 같음(비교 확인).
- HnS가 더한 장면 이벤트 `ITEM_POPUP`은 제안 대상 목록에 없어(upstream도 `CATCH_CHANCE`·`EFFECTIVENESS`는 없음) 실패 때 제안 없이 기존처럼 실패만 알림.
- 실패 테스트 하나로 새 출력 확인: `Unmatched MESSAGE("Wobbuffet used Max Strike!")`처럼 기대 문장이 붙음.
- **#10537 앞당김(seq 513):** 511만 넣은 첫 전체 검증에서 원래 FAIL이던 upstream 테스트 42개(해독제·각성제 등 도구 사용 테스트 — HnS에서는 영문 MESSAGE라 FAIL)가 FLAKY로 바뀜. 재실행 때 첫 실행에서 쓴 도구가 가방에 없어서("Tried to used an item not present in bag") 다른 곳에서 실패한 것 → upstream이 바로 이걸 고친 #10537(seq 513, 선행 #9642·#9805·#9807 모두 들어 있음)을 앞당김. 3파일 충돌 0, 1.17.1과 같은 모양. 해독제 테스트가 다시 FAIL(`Unmatched MESSAGE("Wobbuffet was cured of its poisoning!")`)로 돌아오는 것 확인.
- **남은 FLAKY 2개(재확인 113, upstream 한계):** 테스트 본문의 `SetStartingStatus`(시작 날씨·불바다)가 첫 실행에서 소모돼 재실행이 달라짐 — upstream "Teraform Zero fails if overworld weather is present (Gen9)"와 한글 회귀 HNS9918 K23. 둘 다 원래 FAIL이고 PASS 수는 그대로.
- **HnS 도구:** `dev_scripts/hns_verify/testlist.sh`가 FLAKY 상태 줄을 빼먹어(첫 검증 목록 5,921 → 5,879줄) 정규식에 FLAKY 추가.
- 세이브: 영향 없음.

## 검증

- **빌드:** `make hns -j8` 종료 0, SHA1 `e71242c8…` = 이식 전과 같음. 저장소 전체 재빌드(`build/hns` 지우고): 종료 0, SHA1 `e71242c8…`(같음), 새 컴파일 경고 0(`warncheck.sh`).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `e4cd4538`): 기준 `test-baseline-seq508.txt` 대비 새 PASS 0, 사라진 PASS 0(PASS 3,010). 차이 2줄: 러너 새 테스트 "Failing MESSAGE check doesn't cause FLAKY tests when player uses an item" `EXPECTED_FAIL`, "Teraform Zero … (Gen9)" FAIL → FLAKY(위). 새 기준 **`test-baseline-seq511.txt`**(`verify_seq.sh`도 이 기준으로).
- **한글 테스트:** 새로 쓰지 않음(러너만).
- **세이브 정적 비교**(이전 = 클론 `fa537389` 빌드): **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s511b 8`): **555/652**, 상태 줄 차이 1 = HNS9918 K23 FAIL → FLAKY(위). 실패 목록 줄도 기대 문장이 붙는 새 형식 → 기대 요약 `kortests/expected/summary.txt`를 이 실행으로 갱신, `summary.meta`에 이유(갱신 뒤 비교 PASS·바이트 같음).
- **ROM**(커밋 `e4cd4538`): SHA1 `e71242c84ea01e72a8e58228244957fab6af1372`(묶음 75·76과 같은 파일), 사용 32,980,692 B, 파일 33,554,432 B, 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 새 후보 없음**(ROM 바이트 같음).

## 다음

- seq 512 #10390(알려진 실패 `and 0 more` 표시·`assertf %c/%C`), 그다음 514(513은 이번에 앞당김). 목표 1.17.1.
