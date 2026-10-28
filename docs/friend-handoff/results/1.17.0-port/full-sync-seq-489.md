# full-sync 묶음 56 — seq 489 (#10600 기술 도중 특성이 바뀐 배짱의 고스트 판정)

- 요청: 사용자 "ㄱㄱ 489 시작해 혼자해도 되는거라면"(2026-10-28).
- upstream: 읽기 전용 사본, #10600 `32320410eb`(6파일, +35/−13). 같은 단위(`U-10600`)에 다른 seq 없음.
- 혼자 진행한 근거: 새 문장 0, 한글 문장 변화 0, upstream 그대로(HnS 보정 0). 바뀌는 장면은 배짱 다단 기술이 미라 고스트를 칠 때뿐(HnS 트레이너 데이터에 미라·테라쉘 포켓몬 없음).

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 489 | #10600 | `1f406399` | 대상 실패 판정에서 계산한 상성 배율을 대상마다 저장(`gSpecialStatuses[].storedTypeEffectiveness`)해 대미지 계산이 다시 씀(`useStoredTypeEffectiveness`), 테라쉘 배율 처리를 `MulByTypeEffectiveness`로, `distortedTypeMatchups` 삭제, `overworldWeatherPresent` 비트 위치 이동, upstream 테스트 2개(KNOWN_FAILING 해제·TO_DO 구현), 한글 회귀 테스트 | 32,961,428 |

이식 전(`74fc1c60`, 게임 코드 = `c03e1b92`) 32,961,204 B → **32,961,428 B(+224)**.

## 내용과 HnS 판단

- **적용 방법:** 파일별 3-way 병합(`git merge-file`). 충돌 1곳 — `include/battle.h` `struct BattleStruct` 끝: upstream은 `overworldWeatherPresent`를 u32 비트필드(`padding5`)로 옮기고, HnS는 그 아래에 고유 `removedScreens[NUM_BATTLE_SIDES]`(방벽 해제 문장용)가 있음 → 옮긴 필드 + HnS 줄 유지. 병합 뒤 "upstream 변경 줄 ↔ HnS 변경 줄" 대조: 남는 차이 없음(HnS 줄은 충돌 밖 문맥).
- **저장된 상성 배율의 재사용 경로 확인:** 저장은 `CancelerTargetFailure`의 상성 단계에서 대상마다, 사용은 `CancelerDamageCalc` 한 곳. 그 단계를 건너뛰는 대상은 대미지 단계도 건너뜀(`targetsDone`·`MOVE_RESULT_INVALID_TARGET`). 1.17.1·master에서도 이 필드를 바꾼 뒤 커밋 없음. 전체 테스트(미래예지·다단·스마트 대상·부모의사랑 포함)에서 사라진 PASS 0.
- **바뀌는 출력(`BATTLE_MESSAGE_OUTPUT_CHANGES.md` 1행):** 배짱 노말 다단 기술(더블어택 등)이 미라 고스트를 칠 때 첫 타격 뒤 특성이 미라로 바뀌어도 두 번째 타격이 들어감(이전: 두 번째 타격 없음). 새 문장 0.
- 세이브: `gSpecialStatuses`(배틀 중 EWRAM) +16 B만, 세이브 구조 변화 없음.

## 검증

- **빌드:** `make hns -j8` 종료 0, 전체 재빌드(`build/hns` 지우고) 로그로 새 컴파일 경고 0(`warncheck.sh`).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `1f406399`): `test-baseline-seq488.txt` 대비 **새 PASS 4(한글 2 + upstream 2 — `Scrappy will bypass … replaced mid attack` KNOWN_FAILING → PASS, `Strength Sap … Contrary and is at +6 Atk` TO_DO → PASS), 사라진 PASS 0**, INVALID 24(이전 실행과 같은 목록). 새 기준 [`test-baseline-seq489.txt`](test-baseline-seq489.txt)(**다음 비교 기준**).
- **새 한글 회귀 `test/hns_10600_scrappy.c` 2개:** 배짱 캥카 더블어택 vs 미라 데스니칸(`캥카는 더블어택을 썼다!` → HP 바 → 미라 팝업 → `캥카는 미라가 되었다!` → HP 바 → `2번 맞았다!`), 더블 지진의 대상별 상성 문장(`상대 피카츄에게 효과가 굉장했다!`·`상대 치코리타에게 효과가 별로인 듯하다.`): **2/2 PASS**.
  - 이식 전 코드(클론 `74fc1c60`, `ZZ old` 사본, 커밋 안 함·지움): **1/2** — 배짱 테스트 FAIL(두 번째 HP 바 없음 = 의도한 변화), 지진 PASS(출력 같음).
- **세이브 정적 비교**(이전 = 클론 `74fc1c60` 빌드, ROM SHA1 `f80fa97f…` = 묶음 55 기록과 같음): **PASS(FAIL 0, WARN 1)** — WARN은 `gSpecialStatuses` 304 → 320 B(대상마다 저장 배율 u16, 배틀 중 변수). 세이브 블록 크기·섹터 배치 같음.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s489 8`): **555/652, `판정: PASS (기대와 같음)`, 요약 바이트 같음**.
- **ROM**(커밋 `1f406399`): SHA1 `be0cfe0dde486bd0b56b884e6ef8a925d44b924f`, 사용 32,961,428 B(파일 33,554,432 B), 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 하지 않음(친구).** 후보 — 일반 배틀의 상성 문장(효과가 굉장했다·별로인 듯하다·없는 것 같다)과 대미지가 이식 전과 같은지(단일·더블 전체기·다단 기술). 기대: 같은 화면.

## 다음

- seq 490 #10593(`SetMoveEffect`를 함수 표로). 목표 1.17.1.
