# full-sync 묶음 76 — seq 510 (#10155 테스트 러너 `:L` 꼼수 대체)

- 요청: 사용자 "ㄱㄱ 510 시작해 혼자해도 되는거라면"(2026-10-29).
- upstream: 읽기 전용 사본, #10155 `d128aefe21`(1.16.2, 5파일 +232/−257). 단위 `U-runnerinfra-10155`(다음 #9807 = seq 511, #10390 = seq 512).
- 혼자 진행한 근거: 테스트 러너·hydra 도구만(게임 코드 0). 판정 기준 = ROM 바이트 같음 + 테스트 목록·한글 회귀가 새 출력 형식에서도 같음.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 510 | #10155 | `139c9931` | 실패·건너뜀 메시지의 `":L%s:%d:"` 접두어를 없애고(hydra가 줄 번호를 따로 받음) `tools/mgba-rom-test-hydra/main.c` 파서 개편 | 32,980,692(같음) |

이식 전(`0e19318a`, 게임 코드 = `9ecda50d`) SHA1 `e71242c8…` → 이식 후 **SHA1 `e71242c8…` 같음(바이트 같음)**.

## 내용과 HnS 판단

- `include/test/test.h`·`include/test/battle.h`·`test/test_runner.c`·`test/test_runner_battle.c`는 충돌 0.
- **HnS 줄 1개:** HnS가 더한 `ITEM_POPUP` 장면 검사 메시지(`test_runner_battle.c`)만 `":L"`이 남아 → 옆 `ABILITY_POPUP`과 같이 접두어 지움. 그 밖에 HnS 테스트·한글 회귀 세트·검증 도구에 `":L` 쓰는 곳 없음(검색 0).
- **hydra 충돌 1곳:** HnS는 요약 줄을 `FAILED :` → `FAILED:`로 고쳐 둠(HnS 도구가 이 줄을 읽음) — upstream 변수 이름 변경과 겹침 → upstream 줄 + HnS 띄어쓰기. 1.17.1도 `FAILED:`라 결과는 1.17.1과 같음.
- 실패 테스트 하나(`(DYNAMAX) Dynamaxed Pokemon are immune to Encore`)를 돌려 새 형식 출력이 `test/battle/move_effect/encore.c:138` 꼴로 나오는 것 확인.
- 세이브: 영향 없음.

## 검증

- **빌드:** `make hns -j8` 종료 0, SHA1 `e71242c8…` = 이식 전과 같음. 저장소 전체 재빌드(`build/hns` 지우고): 종료 0, SHA1 `e71242c8…`(같음), 새 컴파일 경고 0(`warncheck.sh`).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `139c9931`): 목록이 `test-baseline-seq508.txt`와 **바이트 같음**(PASS 3,010, 새 PASS 0, 사라진 PASS 0) — `testlist.sh`는 새 출력도 그대로 읽음.
- **한글 테스트:** 새로 쓰지 않음(러너만).
- **세이브 정적 비교**(이전 = 클론 `0e19318a` 빌드): **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s510 8`): **555/652, `판정: PASS (기대와 같음)`, 상태 줄 차이 0**. 요약 파일의 hydra 실패 목록(앞 50줄, 판정에 안 씀) 줄 형식이 "`…:<실패 줄>: <메시지> - <이름>`" → "`…:<테스트 줄>: <이름>: <실패 줄>: <메시지>`"로 바뀌어 바이트로는 달라서, 기대 요약 `kortests/expected/summary.txt`를 이 실행 요약으로 갱신하고 `summary.meta`에 이유를 적음(그 밖 줄은 바이트 같음, 갱신 뒤 비교 `byte-identical: yes`).
- **ROM**(커밋 `139c9931`): SHA1 `e71242c84ea01e72a8e58228244957fab6af1372`(묶음 75와 같은 파일), 사용 32,980,692 B, 파일 33,554,432 B, 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 새 후보 없음**(ROM 바이트 같음).

## 다음

- seq 511 #9807(배틀 테스트 실패 제안), 512 #10390(알려진 실패 표시). 목표 1.17.1.
