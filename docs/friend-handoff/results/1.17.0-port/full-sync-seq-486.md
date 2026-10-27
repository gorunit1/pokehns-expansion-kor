# full-sync 묶음 54 — seq 486 (#10566 자리에 없는 배틀러의 HP 바·결과 문장, 재확인 97 정리)

- 요청: 사용자 "ㄱㄱ 486 시작해 혼자해도 되는거라면"(2026-10-27).
- upstream: 읽기 전용 사본, #10566 `718de8d57d`(4파일, +159/−16). 단위 `U-canceler-10362`(앞 seq 482·483·479는 적용됨).
- 혼자 진행한 근거: 새 문장 0, 한글 문장 변화 0. HnS 화면 출력은 이식 전과 같음(아래 한글 테스트가 이식 전 코드에서도 PASS). 재확인 97은 "#10566이 들어오면 HnS 보정 줄을 지우고 `HnS 10330`이 그대로 PASS인지 확인"으로 미리 정해 둔 정리 — 새 결정 없음.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 486 | #10566 | `2ae2ad83` | `MOVE_RESULT_NOT_PRESENT`(1 << 19)·`MOVE_RESULT_INVALID_TARGET`, 기술 시작 때 쓰러진 대상은 실패 검사·대미지·능력 변화·`IsAnyTargetAffected`에서 빠짐, 내던지기(Fling) 대상 없음 처리를 공통 실패 경로로, 명중 판정에서 대상이 없으면 `BattleScript_MoveEnd`로, 재확인 97 HnS 보정 줄 삭제, upstream 테스트 5개, 한글 회귀 테스트 | 32,961,492 |

이식 전(`00775c1f`, 게임 코드 = `312792d9`) 32,961,380 B → **32,961,492 B(+112)**.

## 내용과 HnS 판단

- **적용 방법:** 파일별 3-way 병합(`git merge-file`). 충돌 1곳 — `include/constants/battle.h` `MOVE_RESULT_…` 묶음 정의 순서(HnS는 HIGH/LOW를 앞에 둠) → upstream 순서로(정의 값 같음). 병합 뒤 "upstream 변경 줄 ↔ HnS 변경 줄" 대조: 남는 차이는 그 순서와 아래 재확인 97 삭제뿐. `moveResultFlags`는 `u32`라 1 << 19 들어감.
- **재확인 97 해결:** seq 479(#10330) 때 넣은 HnS 보정 — `CancelerTargetFailure` 첫 블록에서 쓰러진 대상을 `MOVE_RESULT_FAILED`로 두고 넘김 — 을 지움. upstream `ShouldCheckTargetMoveFailure`의 생존 검사 + `CancelerSetTargets`의 `MOVE_RESULT_NOT_PRESENT`가 같은 일을 함.
  - 지운 뒤 `HnS 10330` 6개 PASS(재확인 97이 적은 확인 항목 — 쓰러진 대상이 `targetsDone`이 되어 `FAILED` 플래그가 없어도 결과 문장·HP 바 없음).
  - 확인용으로 upstream 생존 검사 2줄도 잠시 빼면(= 보정도 검사도 없는 1.17.0 #10330 상태) `HnS 10330` 3개·`HnS 10566` 2개 FAIL → 테스트가 그 회귀를 잡음(되돌린 것은 복구, 커밋 안 함).
- **upstream 새 테스트 5개:** 3개 PASS(특성 흡수·매직미러·보이지않는주먹), 2개 FAIL — `MESSAGE("Whimsicott used Cotton Spore!")`·`MESSAGE("Wobbuffet avoided the attack!")` 영문 문장 검사라 HnS에서 FAIL(같은 장면을 한글 테스트로 옮겨 PASS). PASS인 3개 중 보이지않는주먹은 영문 `NOT MESSAGE`라 HnS에서 늘 통과 → 한글로 다시 검사.
- **화면 변화:** 없음(새 문장 0, 이식 전 코드에서도 한글 테스트 PASS — HnS는 재확인 97 보정과 다른 코드가 이미 막고 있었음). `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 행 없음.
- 세이브: 배틀 코드만, 세이브 구조 변화 없음.

## 검증

- **빌드:** `make hns -j8` 종료 0, 전체 재빌드(`build/hns` 지우고) 로그로 새 컴파일 경고 0(`warncheck.sh`).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `2ae2ad83`): `test-baseline-seq485.txt` 대비 **새 줄 8개(한글 3 PASS, upstream 3 PASS·2 FAIL — 위), 사라진 PASS 0**, 다른 줄 같음, INVALID 24(이전 실행과 같은 목록). 새 기준 [`test-baseline-seq486.txt`](test-baseline-seq486.txt)(**다음 비교 기준**).
- **새 한글 회귀 `test/hns_10566_spread.c` 3개:** 추억의선물로 먼저 쓰러진 상대에게 지진·목화포자 결과 문장·HP 바·능력 변화·사이코필드 문장 없음, 먼저 쓰러진 상대를 빗나간 지진이 이름 부르지 않음(`마자용에게는 맞지 않았다!` → `상대 마자에게는 맞지 않았다!`), 보이지않는주먹 세차게휘두르기가 먼저 쓰러진 상대를 부르지 않음: **3/3 PASS**.
  - 이식 전 코드(클론 `00775c1f`, `ZZ old` 사본, 커밋 안 함·지움): **3/3 PASS**(출력 같음 — 의도대로). upstream 새 테스트 5개도 이식 전 코드에서 같은 상태(3 PASS·2 FAIL).
- **세이브 정적 비교**(이전 = 클론 `00775c1f` 빌드, ROM SHA1 `7996553f…` = 묶음 53 기록과 같음): **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s486 8`): **555/652, `판정: PASS (기대와 같음)`, 요약 바이트 같음**.
- **ROM**(커밋 `2ae2ad83`): SHA1 `58bd67e9a903f4128a2f5b82f31fc65e57254e6f`, 사용 32,961,492 B(파일 33,554,432 B), 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 하지 않음(친구).** 후보 — 더블에서 한 턴에 상대 하나가 먼저 쓰러진 뒤 같은 턴 지진·파도타기·하이퍼보이스 등 전체기, 내던지기를 쓰기 전에 대상이 쓰러진 경우(실패하고 도구는 소모). 기대: 이식 전과 같은 화면.

## 다음

- seq 488 #10595(Substitute 검사를 기술 처리로 옮김). 목표 1.17.1.
