# full-sync 묶음 43 — seq 476 (#10220 공격 전 효과·대미지 계산을 공격 취소 단계로)

- 요청: 사용자 "ㄱㄱ 476 시작해 혼자해도 되는거라면"(2026-10-25).
- upstream: 읽기 전용 사본, `git diff a0efba995f^1 a0efba995f`(15파일, 게임 코드만 — 테스트 변경 없음). kor=N.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 476 | #10220 | `84d0b6f8` | `damagecalc`·`setpreattackadditionaleffect` 명령 삭제 → 취소 단계 `CANCELER_PRE_ATTACK_MOVE_EFFECT`·`CANCELER_DAMAGE_CALC`, 꿈먹기·트랩셸은 일반 공격 스크립트, 프레젠트 자체 타격 순서, 미래예지 착탄의 `damagecalc` 삭제, `IsAsleepOrComatose` | 32,963,636 |
| (재확인 32) | — | `0fed230b` | `CancelerInterruptibleMoves`의 `switch`를 upstream처럼 `cv->moveEffect`로(HnS `DoAttackCanceler`가 이미 채움) | 32,963,588 |

이식 전(`63f94554`, 코드 = `5cd8e493`) 32,963,812 B → **32,963,588 B(−224)**.

## 내용과 HnS 판단

- `git apply`는 15파일 중 대부분 문맥이 달라 손으로. HnS 스크립트·C 코드에서 지워지는 명령·라벨(`damagecalc`, `setpreattackadditionaleffect`, `jumpifshelltrap`, `BattleScript_HitFromAccCheck`·`HitFromDamageCalc`·`EffectHit_RetFromAccCheck`·`Hit_RetFromAtkAnimation`, `BattleScript_EffectDreamEater`·`EffectShellTrap`)를 쓰는 HnS 고유 코드는 없음(`grep`).
- **대미지 계산:** 새 `CancelerDamageCalc`가 대상마다 계산(주얼 문장도 여기서). HnS는 #10616(seq 325)의 `baseMove`를 문맥에 넣음 — 지운 `Cmd_damagecalc`에 있던 줄, upstream 1.17.1에도 있음(Z기술·다이맥스 위력 기준).
- **공격 전 효과:** `CancelerPreAttackMoveEffect` — 내던지기·폴터가이스트 문장 등. 명중 판정 뒤 아무도 맞지 않으면 `CANCELER_RESULT_END`로 취소 단계를 끝냄.
- **꿈먹기:** 깨어 있는 대상은 대상 실패 단계에서 "효과가 없는 것 같다"(타입 무효와 같은 갈래, 대상 이름).
- **트랩셸:** 맞지 않았으면 취소 단계에서 실패 → `BattleScript_ShellTrapFailed`(같은 문장).
- **잠자기(Rest):** upstream은 `IsAsleepOrComatose(cv->battlerDef, …)`로 바꾸는데 사용자 대상 기술이라 같은 배틀러 — HnS는 이전처럼 사용자 기준을 유지(주석).
- **명령 번호:** HnS는 `UNUSED_39`를 미리 두고 37·38 자리를 비워 뒀다(seq 427 주석) → 이번에 37·38을 그 자리에.
- 재확인 32의 "`CancelerInterruptibleMoves`의 `GetMoveEffect(cv->move)` — #10220 때 맞춤": HnS `DoAttackCanceler`가 이미 `cv.moveEffect`를 채우므로 `cv->moveEffect`로 맞춤(후속 커밋).

## 검증

- **빌드:** `make hns -j8` 종료 0, 새 경고 0. ROM 32,963,812 → 32,963,636 B(−176, `84d0b6f8`).
- **전체 테스트**(`84d0b6f8` 직전 작업 트리, 새 테스트 넣기 전): 목록이 `test-baseline-seq475.txt`와 **바이트 같음**(새 PASS 0, 사라진 PASS 0).
- **새 한글 회귀 `test/hns_10220_canceler.c` 9개** — 꿈먹기(깬 대상·잠든 대상), 트랩셸(불발·발동), 프레젠트 대미지, 주얼 문장 순서, 내던지기·폴터가이스트 사전 문장, 전체기 대상별 대미지: **9/9 PASS**. 이식 전 코드(클론 `63f94554`)에서도 **9/9 PASS** → 화면 출력 같음(순수 리팩터).
- **전체 테스트**(코드 `84d0b6f8` + 새 테스트): `test-baseline-seq475.txt` 대비 새 PASS 9(새 한글 테스트)뿐, 사라진 PASS 0, 다른 줄 0. 새 기준 [`test-baseline-seq476.txt`](test-baseline-seq476.txt)(**다음 비교 기준**).
- **재확인 32 후속(`0fed230b`):** 빌드 새 경고 0, `Future Sight` 7/16·`Pledge` 27/42·`Shell Trap` 3/12(영어 문장 FAIL 포함, 전체 실행과 같은 결과), `HnS 10220` 9/9·`HnS 9` 25/25 PASS.
- **세이브 정적 비교**(이식 전 = 코드 `63f94554`, 클론 빌드): **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`kortests/run.sh <저장소> kor-b56 8`, 코드 `84d0b6f8`): **558/652, `판정: PASS (기대와 같음)`**.
- **ROM**(코드 `0fed230b`): SHA1 `e8d53c3cdde54ac1b87d2e70a06f22e55534a4c7`, 사용 32,963,588 B(파일 33,554,432 B), 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 하지 않음(친구).** 화면 변화가 없는 묶음이라 실기 후보 없음(원하면 꿈먹기·트랩셸·내던지기 정도).

## 다음

- seq 477 #10311(Remove end2, kor=Y — 재확인·결과 문서 132 메모: HnS `End2` 라벨 이름 정리).
