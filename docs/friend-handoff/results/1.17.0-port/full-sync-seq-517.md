# full-sync 묶음 82 — seq 517 (#10061 암묵 waitstate 정리 스크립트)

- 요청: 사용자 "ㄱㄱ 517 시작해 혼자해도 되는거라면"(2026-10-29).
- upstream: 읽기 전용 사본, #10061 병합 커밋 `af94108be5`(1.15.3, 첫 부모 대비 30파일 +67/−72). 단위 `U-waitstate-9986`(seq 516 다음, 이 단위 끝).
- 혼자 진행한 근거: 스크립트 정리. 판정 기준 = 이벤트 스크립트가 seq 516 빌드와 라벨 단위로 같고 ROM SHA1이 같음(충족).

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 517 | #10061 | `6c4ca257` | upstream 30파일(충돌 0) + 정리 스크립트를 HnS 전체에 돌림 + HnS 전용 special 14개 `waitstate=1` | 32,980,836(같음) |

이식 전(`47fbedf4`, 게임 코드 = `376139af`) SHA1 `35adc84a…` → 이식 후 **SHA1 `35adc84a…` 같음(바이트 같음)**.

## upstream 내용

- `callnative`에 `waitstate=` 인자(1이면 뒤에 암묵 대기), `def_special` 인자 순서를 `requests_effects`, `waitstate` 순으로(HnS 줄은 모두 이름 붙은 인자라 영향 없음).
- preproc: 레이블 뒤 위치 출력(어셈블러 경고가 매크로를 부른 스크립트 줄을 가리키게).
- `migration_scripts/1.15/delete_implicit_waitstates.py`: 빌드 경고 "explicit waitstate follows implicit waitstate"를 모아 그 줄의 명시 `waitstate`를 지움.
- FRLG 스크립트의 중복 `waitstate` 정리(HnS FRLG 파일은 대부분 upstream과 같아 그대로 들어감).

## HnS 작업

- **정리 스크립트(`hns` 빌드):** seq 516에서 남긴 HnS 명시 `waitstate` 125개 중 124개 삭제(HnS 맵 스크립트·공용 스크립트 60여 파일).
- **남은 1개 손으로:** `data/maps/SaffronCity_FightingDojo_hns/scripts.inc`의 `special ChoosePartyMon` 바로 뒤 `waitstate`. 경고가 가리키는 줄이 실제보다 2줄 아래(주석 줄)라 스크립트가 "waitstate 없는 줄"로 보고 건너뜀. 같은 빌드의 다른 HnS 경고(`setvar` 안내)도 1줄씩 어긋나 있음 — 기록만(스크립트는 `waitstate`가 적힌 줄만 지우므로 엉뚱한 줄을 지우지 않고, 아래 바이트 비교로 확인).
- **HnS 전용 special 14개 `waitstate=1`:** HnS가 더한 special 73개 중, 스크립트의 모든 호출이 바로 뒤에 `waitstate`를 다는 것만 — BP 상점 7개(`CreateBPVitaminShop`·`CreateBPHoldItemShop`·`CreateBPHoldItemShop2`·`CreateBPDecorShop1`·`CreateBPDecorShop2`·`CreateBPPokeBallShop`·`CreateBPPowerShop`), `CreateKurtBallShop`, `DoSlidingPuzzle`(8곳), `NameRival`, `Special_BuenaPasswordShowMultichoice`, `Special_MomOpenDepositInput`, `Special_MomOpenWithdrawInput`, `Special_ViewVoltorbFlip`. 표시 후 정리 스크립트를 다시 돌려 그 뒤 명시 `waitstate` 29개 삭제(BP 상점 7×2·커트 1·퍼즐 8·라이벌 2·나머지 4×1; 비FRLG 삭제 합계 154 = 124 + 1 + 29와 맞음). 앞으로 HnS 스크립트에서 이 special 뒤 대기를 빠뜨려도 자동으로 붙음.
- 신오 유적 아르세우스 방 `callnative` 3곳(seq 516)은 그대로(대기 없음이 맞음).
- **어셈블러 `waitstate` 경고 125 → 0.**
- 세이브: 영향 없음.

## 검증

- **스크립트 바이트 비교**(`port/scriptcmp.py`, 이전 = 클론 `47fbedf4` 빌드): 라벨 30,864개 차이 0, 한쪽에만 있는 라벨 0.
- **빌드:** `make hns -j8` 종료 0, SHA1 `35adc84a…` = 이식 전과 같음. 저장소 전체 재빌드(`build/hns` 지우고): 종료 0, SHA1 같음, 새 컴파일 경고 0(`warncheck.sh`), `waitstate` 어셈블러 경고 0.
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `6c4ca257`): 목록이 `test-baseline-seq511.txt`와 **바이트 같음**(PASS 3,010).
- **한글 테스트:** 새로 쓰지 않음(ROM 같음).
- **세이브 정적 비교**(이전 = 클론 `47fbedf4` 빌드): **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s517 8`): **555/652, `판정: PASS (기대와 같음)`, 요약 바이트 같음**.
- **ROM**(커밋 `6c4ca257`): SHA1 `35adc84a4085c2007a2abaeb30ab370ae47398dd`(묶음 81과 같은 파일), 사용 32,980,836 B, 파일 33,554,432 B, 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 새 후보 없음**(ROM 같음). 묶음 81 실기 3항목은 그대로 유효.

## 다음

- seq 518 #10555(pret 병합 2026-07-30, XL — 선행 #10323(seq 271)·#9986·#9881 확인부터). 목표 1.17.1.
