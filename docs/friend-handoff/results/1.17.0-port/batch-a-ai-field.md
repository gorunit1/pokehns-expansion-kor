> 이 문서의 커밋 해시는 작업용 worktree 기준이다. 메인 브랜치 해시는 [결과 보고서](../pokeemerald-expansion-1.17.0-update-report.md)의 "적용 단위 기록" 표를 본다.

# 묶음 A — AI·필드·일반 이식 결과 (worktree `port/a-ai-field`, 시작점 `7c4dd0c096`)

빌드 명령: `make hns -j6 > build/port.log 2>&1` (worktree `<worktree>`). 기준(보고서 1절): ROM 33,326,484 B / EWRAM 94.99% / IWRAM 78.37%.

## 적용 단위: #10342 Fix reversed AI battlerAtk and battlerDef usage (스탯 외 hunk)

- 판정: 부분 적용
- upstream 근거: PR #10342, 커밋 `983e7bfaf3`
- 커밋: `bc2903faf8`
- 수정 파일: `src/battle_ai_util.c`, `src/battle_ai_main.c`
- 적용 내용:
  - `AI_GetDamage` `AI_DEFENDING` 분기가 방어 측(`battlerDef`)의 RISKY/CONSERVATIVE 플래그를 보도록 수정
  - `IncreaseStatUpScoreInternal` 앵콜 PP 검사 뒤 세미콜론 제거
  - `DoesSideHaveDamagingHazards` `default: return FALSE` → `break` (모든 해저드 검사)
  - `RecordMovesBasedOnStab` `GetMovePower(playerMove != 0)` → `GetMovePower(playerMove) != 0`
  - `AI_CalcDamage` `EFFECT_HIT_ENEMY_HEAL_ALLY` 조기 반환 시 `aiCalcInProgress = FALSE`
- 제외 hunk: 스탯 변화 관련(`AI_CanAnyStatChange`, `AI_CanStatChangeBePrevented`, `AI_GetAdjustedStatStage`, `GetFoeStatChangeScore`, `GetSelfStatChangeScore`, `GetAllyStatChangeScore`, `AI_CheckBadMove`의 `AI_CanAnyStatChange` 인자) — HnS에 해당 함수가 없음. `AI_SWITCHIN_DEFENDING`/`AI_SHOULD_SETUP_DEFENDING` 분기 — HnS에 #9568이 없어 해당 문맥 없음.
- 보존한 HNS·한글화 차이: 문자열·메시지 경로 변경 없음
- 충돌 여부와 상세: 없음
- 검증:
  - 코드 대조: HnS 코드에 5개 버그 모두 존재 확인 후 수정
  - `git diff --check`: 통과
  - `make hns -j6`: 성공(종료 0, 전체 빌드 1분 34초). ROM 33,326,548 B (99.32%), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10412 Reset move data between switch-in calculations

- 판정: 적용
- upstream 근거: PR #10412, 커밋 `3f383c0b84`
- 커밋: `e5a39d6b14`
- 수정 파일: `src/battle_ai_main.c` (`CalcBattlerAiMovesData`)
- 보존한 HNS·한글화 차이: HnS `AiCalcValues` 구조 유지. upstream의 지역 변수 `effectiveness` 대신 `aiCalc.typeEffectiveness`(직전에 0으로 초기화)를 사용
- 충돌 여부와 상세: 없음
- 검증:
  - 코드 대조: `IsMoveUnusable`로 `continue`하기 전에 4개 필드를 초기화하도록 upstream과 같은 위치에 추가
  - `git diff --check`: 통과
  - `make hns -j6`: 성공(종료 0). ROM 33,326,564 B (99.32%), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10302 Fix AI partner seeing all moves bad on dead adjacent opponent battler

- 판정: 적용
- upstream 근거: PR #10302, 커밋 `fca2d4a35f`
- 커밋: `bdfd13fe9f`
- 수정 파일: `src/battle_ai_switch.c` (`ShouldSwitchIfAllMovesBad`)
- 보존한 HNS·한글화 차이: HnS `BattleContext`는 `abilityDef`/`holdEffectDef`를 별도 필드로 캐시하므로 upstream(`battlerDef`만 복원)과 달리 세 필드를 함께 복원
- 충돌 여부와 상세: 없음
- 검증:
  - 코드 대조: 파트너 검사 뒤 `ctx.battlerDef`가 복원되지 않는 버그 확인 후 수정
  - `git diff --check`: 통과
  - `make hns -j6`: 성공(종료 0). ROM 33,326,596 B (99.32%), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10425 Fix AI running Focus Punch checks on Present and Fixed HP dmg moves

- 판정: 적용
- upstream 근거: PR #10425, 커밋 `613899a370`
- 커밋: `2f6cf4e90d`
- 수정 파일: `src/battle_ai_main.c` (`AI_CheckBadMove`)
- 보존한 HNS·한글화 차이: 해당 없음(AI 점수만). 테스트 hunk 미이식
- 충돌 여부와 상세: 없음
- 검증:
  - 코드 대조: HnS의 fallthrough가 upstream 수정 전과 동일함을 확인. 소스 hunk를 그대로 적용
  - `git diff --check`: 통과
  - `make hns -j6`: 성공(종료 0). ROM 33,326,564 B (99.32%), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10409 Fix AI target filtering and debug score highlighting

- 판정: 적용
- upstream 근거: PR #10409, 커밋 `b88200a4bf`
- 커밋: `7345201ae4`
- 수정 파일: `src/battle_ai_main.c` (`ShouldConsiderMoveForBattler`), `src/battle_debug.c` (`PutMovesPointsText`)
- 보존한 HNS·한글화 차이: `battle_debug.c`는 기능 수정(`chosenTarget == j` → `== battlerDef`, 3곳)만 반영하고 변수명 정리(i/j → moveIndex/battler)는 제외. 디버그 영문 문자열 불변
- 충돌 여부와 상세: 없음
- 검증:
  - 코드 대조: HnS 코드가 upstream 수정 전과 동일
  - `git diff --check`: 통과
  - `make hns -j6`: 성공(종료 0). ROM 33,326,596 B (99.32%), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10411 fix(ai): avoid rewarding Levitate ally immunity

- 판정: 적용
- upstream 근거: PR #10411, 커밋 `8b11097c4f` (1.17.0의 `ABILITY_EELEVATE` 배치 참고)
- 커밋: `ee4e42c910`
- 수정 파일: `src/battle_ai_main.c` (`ShouldTriggerPartnerAbility` 추가, `AI_DoubleBattle` 파트너 특성 분기), `src/battle_util.c` (`AbsorbedByFlashFire`)
- 보존한 HNS·한글화 차이:
  - HnS 전용 Champions 특성 `ABILITY_EELEVATE`(천정부지)는 1.17.0과 같이 부유와 함께 새 분기(가산 없음)로 이동
  - `AbsorbedByFlashFire`는 HnS의 `struct BattleContext *`(upstream은 `DamageContext`)로 인자를 바꾸고, `runScript`일 때만 `flashFireBoosted`를 설정. `MULTISTRING_CHOOSER`(`B_MSG_FLASH_FIRE_BOOST`/`NO_BOOST`)와 `BattleScript_FlashFireBoost` 반환은 그대로라 배틀 메시지 선택 불변
  - `runScript = TRUE`는 실제 기술 처리(`CancelerTargetFailure`)에서만 설정됨을 확인. AI(`AI_CanMoveBeBlockedByTarget`)·타입 상성 계산 경로에서 타오르는불꽃 상태가 켜지던 문제만 제거
  - 테스트 hunk 미이식
- 충돌 여부와 상세: `battle_util.c`는 고위험 파일이나 변경은 상태 플래그 가드뿐이고 메시지·스크립트 경로를 건드리지 않음
- 검증:
  - 코드 대조: HnS 파트너 특성 분기가 upstream 수정 전과 동일(+EELEVATE)
  - `git diff --check`: 통과
  - `make hns -j6`: 성공(종료 0). ROM 33,326,900 B (99.32%), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10006 Fix AI partner incorrectly having flags set to Battler1 instead of Battler2

- 판정: 적용
- upstream 근거: PR #10006, 커밋 `5c6bcbf5be`
- 커밋: `50867ebb87`
- 수정 파일: `src/battle_ai_main.c` (`BattleAI_SetupFlags`)
- 보존한 HNS·한글화 차이: 해당 없음. 테스트 hunk(`ai_multi.c`) 미이식
- 충돌 여부와 상세: 없음
- 검증:
  - 코드 대조: 1줄 동일 적용
  - `git diff --check`: 통과
  - `make hns -j6`: 성공(종료 0). ROM 33,326,900 B (99.32%, 변화 없음), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #9985 Remove Defiant and Competitive from Partner Ability check in Doubles

- 판정: 적용
- upstream 근거: PR #9985, 커밋 `e632ab55ca`
- 커밋: `4ba9e18670`
- 수정 파일: `src/battle_ai_main.c` (`AI_DoubleBattle`)
- 보존한 HNS·한글화 차이: HnS 문맥(`IsStatLoweringEffect(effect)`, upstream은 `IsStatLoweringMove(move)`)은 유지하고 `case` 레이블 2줄만 삭제
- 충돌 여부와 상세: 없음
- 검증:
  - 코드 대조: 수동 적응(문맥 차이만)
  - `git diff --check`: 통과
  - `make hns -j6`: 성공(종료 0). ROM 33,326,692 B (99.32%), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10191 Fix event mon stat calculate without ivs

- 판정: 적용
- upstream 근거: PR #10191, 커밋 `70dbb6151a`
- 커밋: `5df45529d7`
- 수정 파일: `src/pokemon.c` (`CreateEventMon`, `CreateEnemyEventMon`)
- 보존한 HNS·한글화 차이: HnS `CreateEnemyEventMon`의 randomizer 분기·싱크로 성격/성별 `GetMonPersonality`·`SetNuzlockeChecks`는 그대로. 대상 파티는 HnS `gEnemyParty[0]`(upstream `gParties[B_TRAINER_OPPONENT_A][0]`) — 삭제만 하므로 치환 불필요
- 충돌 여부와 상세: 없음. HnS `CreateMon`/`CreateMonWithIVs` 구현이 upstream과 동일함을 확인
- 검증:
  - 코드 대조: 수정 전에는 `CalculateMonStats`가 개체값 0 상태에서 호출되고 이후 재계산이 없었음
  - `git diff --check`: 통과
  - `make hns -j6`: 성공(종료 0). ROM 33,326,692 B (99.32%, 변화 없음), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시(정적 조우 능력치 실기 확인 권장)
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10242 Fix checking item id instead of hold effect for hold_effect_repel

- 판정: 적용
- upstream 근거: PR #10242, 커밋 `379122dfc3`
- 커밋: `e2d48d8b1a`
- 수정 파일: `src/wild_encounter.c` (`ApplyCleanseTagEncounterRateMod`, `#include "item.h"`)
- 보존한 HNS·한글화 차이: HnS `gPlayerParty[0]`(upstream `gParties[B_TRAINER_PLAYER][0]`)로 맞춤. HnS에서 `HOLD_EFFECT_REPEL` 아이템은 순결의부적·깨끗한향로 2개
- 충돌 여부와 상세: 없음
- 검증:
  - 코드 대조: 수동 적응(파티 배열명만)
  - `git diff --check`: 통과
  - `make hns -j6`: 성공(종료 0). ROM 33,326,708 B (99.32%), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10247 fix(spots): add arguments to incgfx and check for isEgg

- 판정: 부분 적용
- upstream 근거: PR #10247, 커밋 `07f10612ab`
- 커밋: `5f3058a3db`
- 수정 파일: `src/decompress.c` (`LoadSpecialPokePicIsEgg`)
- 보존한 HNS·한글화 차이: HnS는 반점 일반화(#9594) 이전 구조(`species == SPECIES_SPINDA` + `DrawSpindaSpots`)이므로 그 조건에 `&& !isEgg`만 추가
- 제외 hunk: `src/pokemon_spots.c` INCGFX 인자 변경 — HnS에 해당 파일/INCGFX 없음
- 충돌 여부와 상세: 없음
- 검증:
  - 코드 대조: 알 그래픽에도 얼룩 반점이 그려지는 경로 확인
  - `git diff --check`: 통과
  - `make hns -j6`: 성공(종료 0). ROM 33,326,724 B (99.32%), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10551 Fix follower crashing when interupting spin movement

- 판정: 적용
- upstream 근거: PR #10551, 커밋 `84a6947095`
- 커밋: `1746ed266b`
- 수정 파일: `src/field_player_avatar.c` (`ForcedMovement_None`, 전방 선언 1줄)
- 보존한 HNS·한글화 차이: 해당 없음
- 충돌 여부와 상세: 없음. HnS에 `PlayerSetCopyableMovement`·`COPY_MOVE_NONE` 존재 확인
- 검증:
  - 코드 대조: upstream과 동일 적용
  - `git diff --check`: 통과
  - `make hns -j6`: 성공(종료 0). ROM 33,326,708 B (99.32%), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시(회전 타일 위에서 동반 포켓몬과 함께 이동 중단 실기 확인 권장)
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10329 fix(trainer-see): fix trainer type SEE_ALL_DIRECTIONS not facing player

- 판정: 적용
- upstream 근거: PR #10329, 커밋 `d87e4e2a71`
- 커밋: `92844b787b`
- 수정 파일: `src/trainer_see.c` (`TRSEE_TURN_TO_FACE_PLAYER` 단계·`TrainerTurnToFacePlayer` 추가, `TrainerMoveToPlayer` 정리)
- 보존한 HNS·한글화 차이: HnS `trainer_see.c`는 upstream 수정 전 파일과 INCBIN/INCGFX 줄·주석 1곳만 다름. 해당 hunk 5개를 `git apply`로 그대로 적용(파일 모드 100755 유지)
- 충돌 여부와 상세: 없음. 변장·매몰 트레이너가 `TRSEE_MOVE_TO_PLAYER`로 직접 가는 경로는 upstream과 같음
- 검증:
  - 코드 대조: `git apply --check` 통과
  - `git diff --check`: 통과
  - `make hns -j6`: 성공(종료 0). ROM 33,326,788 B (99.32%), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10529 Fix movement type playing between trainer move and player face

- 판정: 적용
- upstream 근거: PR #10529, 커밋 `4dc3dc6588` (선행 #10329)
- 커밋: `34b18c3cd5`
- 수정 파일: `src/trainer_see.c` (`TrainerMoveToPlayer`, `PlayerFaceApproachingTrainer`)
- 보존한 HNS·한글화 차이: 해당 없음. upstream hunk 2개를 `git apply`로 그대로 적용. 적용 후 두 함수는 upstream 1.17.0·현재 upstream master(#10827 이후)와 동일
- 충돌 여부와 상세: 없음
- 검증:
  - 코드 대조: `git apply --check` 통과
  - `git diff --check`: 통과
  - `make hns -j6`: 성공(종료 0). ROM 33,326,756 B (99.32%), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 남은 문제 및 사용자 결정 필요 사항:
  - **upstream 동작 그대로 들어온 부작용(참고):** 바로 옆 칸(거리 1, `tTrainerRange == 0`)에서 발견한 트레이너는 #10329 이후 `TrainerTurnToFacePlayer` → `TRSEE_PLAYER_FACE`로 바로 가서 `TrainerMoveToPlayer`를 거치지 않는다. #10529가 이동 타입 고정(`SetTrainerMovementType` + 템플릿 덮어쓰기)을 `TrainerMoveToPlayer`로 옮겼기 때문에, 인접 트레이너는 더 이상 템플릿 이동 타입이 "그 방향 바라보기"로 바뀌지 않는다. 전투 후 맵을 다시 읽으면 원래 이동 타입(두리번거리기·회전 등)으로 돌아갈 수 있다. 이식 전 HnS와 바닐라는 인접 트레이너에도 적용했다. upstream 1.17.0과 최신 master도 같은 구조다. 유지하려면 `TrainerTurnToFacePlayer`의 `!task->tTrainerRange` 분기에 같은 3줄을 넣으면 된다(미적용, 결정 필요). 실기 확인 권장.

## 적용 단위: #10015 Fix incorrect action handler being set after rearranging moves in Emerald tutorial battle

- 판정: 적용
- upstream 근거: PR #10015, 커밋 `1729e440e9`
- 커밋: `3fadb3c31b`
- 수정 파일: `src/battle_controller_player.c` (`HandleMoveSwitching` 2곳)
- 보존한 HNS·한글화 차이: 해당 없음
- 충돌 여부와 상세: 없음. **upstream과의 차이:** upstream은 기술 순서 교체 확정 쪽 1곳(HnS :1091)만 고쳤고, B/SELECT로 교체를 취소하는 쪽(HnS :1111)은 1.17.0에서도 같은 버그가 남아 있다. g5 보고서가 두 줄을 모두 대상으로 적었으므로 같은 `IS_FRLG &&` 조건을 두 곳에 적용했다. 취소 쪽을 upstream과 똑같이 두려면 :1111 한 줄만 되돌리면 된다
- 검증:
  - 코드 대조: HnS는 비FRLG(`IS_FRLG 0`) 빌드. 첫 전투(`BATTLE_TYPE_FIRST_BATTLE`)에서 기술 교체 후 핸들러가 FRLG 튜토리얼용으로 바뀌던 경로 확인
  - `git diff --check`: 통과
  - `make hns -j6`: 성공(종료 0). ROM 33,326,724 B (99.32%), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 남은 문제 및 사용자 결정 필요 사항: 취소 경로까지 넓힌 부분(위 설명) 확인

## 적용 단위: #9963 Fixes double battle rematches being single battles

- 판정: 적용
- upstream 근거: PR #9963, 커밋 `9b86a8709b`
- 커밋: `46c226ad6f`
- 수정 파일: `src/battle_setup.c` (`BattleSetup_StartRematchBattle`)
- 보존한 HNS·한글화 차이: HnS 리매치 스크립트(`data/scripts/trainer_battle.inc`)·리매치 테이블 불변. 일반 트레이너전(`:1639`)과 같은 `GetTrainerBattleType` 판정을 사용. upstream의 공백만 있는 줄은 빈 줄로 바꿈
- 충돌 여부와 상세: 없음
- 검증:
  - 코드 대조: `EventScript_TryDoDoubleRematchBattle`이 `BattleSetup_StartRematchBattle`을 부르는데 더블 플래그가 없던 경로 확인
  - `git diff --check`: 통과
  - `make hns -j6`: 성공(종료 0). ROM 33,326,884 B (99.32%), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10241 Fix off by one error in randomweightedIndex

- 판정: 적용
- upstream 근거: PR #10241, 커밋 `44ef98a6b9`
- 커밋: `703282acfd`
- 수정 파일: `src/random.c` (`RandomWeightedIndex`)
- 보존한 HNS·한글화 차이: 해당 없음. HnS 호출처는 동반 포켓몬 감정 선택(`event_object_movement.c:2887`) 1곳
- 충돌 여부와 상세: 없음
- 검증:
  - 코드 대조: 1줄 동일 적용
  - `git diff --check`: 통과
  - `make hns -j6`: 성공(종료 0). ROM 33,326,884 B (99.32%, 변화 없음), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #9990 Various Follower Sprite Fixes from issue #5135

- 판정: 적용
- upstream 근거: PR #9990, 커밋 `cfb06654ef`
- 커밋: `08d2efebed`
- 수정 파일: `graphics/pokemon/{carracosta,cinccino,garbodor,gothita,gothitelle,gothorita,jellicent,krokorok,maractus,minccino,scolipede,scrafty,scraggy,tirtouga,vanillish,vanillite,zoroark,zorua}/overworld*.png|pal` 24개
- 보존한 HNS·한글화 차이: 적용 전 HnS 24개 파일의 blob 해시가 upstream 수정 전 이미지와 모두 같음을 확인(HnS 고유 수정 없음). PR의 바이너리 diff를 `git apply`로 적용. PNG 크기(192×32 / 256×32)는 그대로이고 저장 비트 깊이만 4bit→8bit 색인(값 16 미만)으로 바뀜. gbagfx가 정상 변환함
- 충돌 여부와 상세: 없음
- 검증:
  - 코드 대조: blob 해시·PNG 크기 비교
  - `git diff --check`: 통과
  - `make hns -j6`: 성공(종료 0, 대상 `overworld.4bpp`·`.gbapal` 재생성 확인). ROM 33,326,516 B (99.32%, −368 B, 압축 결과 차이), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시(동반 포켓몬 화면 확인 권장)
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #9608 Removes DecompressTrainerBackPic fixing multibattles with 5-frame partner back sprites

- 판정: 적용
- upstream 근거: PR #9608, 커밋 `2f56d57f31`
- 커밋: `cb82d098e2`
- 수정 파일: `include/battle_gfx_sfx_util.h`, `src/battle_gfx_sfx_util.c`, `src/battle_controllers.c`(2곳), `src/reshow_battle_screen.c`(사파리·포획 튜토리얼 2곳)
- 보존한 HNS·한글화 차이: 헤더는 HnS 선언 문맥(`UseGen4BattleUI` 등)이 달라 프로토타입 1줄만 직접 삭제. src 3파일은 줄 오프셋만 있고 내용이 같아 `git apply`로 적용
- 충돌 여부와 상세: 없음. HnS `SetMultiuseSpriteTemplateToTrainerBack`(`pokemon.c:4304`)이 upstream과 같이 `gTrainerBacksprites[].backPic`을 직접 쓰는 것을 확인. `spritesGfx`를 읽는 나머지 경로(기절 연출·기술 애니 BG 복사·진화/교환 등)는 포켓몬 그림용이라 영향 없음
- 검증:
  - 코드 대조: 위 내용
  - `git diff --check`: 통과
  - `make hns -j6`: 성공(종료 0). ROM 33,326,500 B (99.32%, −16 B), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시(전투 시작 시 플레이어 뒷모습, 사파리 존, 파트너 멀티배틀 뒷모습 실기 확인 권장)
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 요약 (port/a-ai-field)

- 커밋 19개(`bc2903faf8`..`cb82d098e2`), 모두 `make hns -j6` 성공 후 커밋. push 안 함.
- 판정: 적용 17, 부분 적용 2(#10342 스탯 hunk 제외, #10247 INCGFX/반점 일반화 제외), 보류 0.
- 최종 빌드(`cb82d098e2`): ROM 33,326,500 B / 32 MB (99.32%, 기준 대비 +16 B), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%). `pokehns.gba` SHA1 `87888bf108e2b1feb04b2359a0f1a16a398b2b41`.
- 한글 문자열·배틀 메시지 출력·HnS 전용 맵/스크립트/데이터·config 기본값 변경 없음. upstream `test/**` 미이식.
- 확인 필요:
  1. #10529 — 인접(거리 1) 트레이너는 이동 타입 고정이 빠진다(upstream 1.17.0/master와 같음). 유지할지, `TrainerTurnToFacePlayer`에 3줄을 보완할지 결정 필요.
  2. #10015 — upstream이 고치지 않은 B/SELECT 취소 경로(HnS :1111)에도 같은 조건을 넣었다.
- 실기 검증은 전 항목 미실시.
