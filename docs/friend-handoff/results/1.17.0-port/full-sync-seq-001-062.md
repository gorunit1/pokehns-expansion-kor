# full-sync 실제 port 결과 — seq 1~62

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`CLAUDE_FULL_SYNC_PORT_PROMPT.md`](../../CLAUDE_FULL_SYNC_PORT_PROMPT.md)
시작 HEAD: `88d72d436e`

## 동기화 단위: seq 1 `U-testrunner-fix`

- 현재 판정: 적용
- upstream 근거: #9892 커밋 `35a45557e1`의 첫 hunk와 동일
- 해결한 의존성: 없음
- 수정 파일: `test/test_runner.c` (`fake_rtc.h` include를 `global.h` 뒤로 이동)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 해당 없음(테스트 러너 전용, 게임 ROM 불변)
- 저장·ROM·그래픽 영향: 없음
- 검증:
  - 코드 대조: HnS `16ca5376eb`가 `fake_rtc.h`에 `gSaveBlock3Ptr`를 쓰는 inline 함수를 넣어, `global.h`보다 먼저 include되면 컴파일되지 않았다.
  - `git diff --check`: 통과
  - `make check BUILD=hns -j8`: 테스트 ELF 컴파일·링크 성공. 실행 도중 프로세스가 종료되어(데스크탑 WSL 재시작) 전체 결과는 없다.
  - 부분 결과: PASS 243, FAIL 3,641, KNOWN_FAILING 9, ASSUMPTION/SKIP 36
    - FAIL 중 3,381건이 같은 원인이다: `src/malloc.c:120` `AGB_ASSERT(block->allocated == TRUE)`와 `Illegal opcode: 0000efff`. 이미 해제된 블록을 해제하는 것으로 보이며, 영문 `MESSAGE` 불일치와는 다른 문제다.
    - 84건에서 `src/text.c:2696`(upstream `AllocateTextPrinter`)의 56바이트 누수 경고가 함께 나왔다.
    - HnS가 upstream 대비 추가한 `Free` 호출 중 배틀 경로에 있는 것은 없다(`challenge_menu`, `credits_hns`, `naming_screen`, `tv` 등만 해당).
  - 실제 ROM: 해당 없음
- 다음 단위 또는 외부 결정 필요 사항: 테스트 힙 assertion 원인을 별도로 조사한다. 해결 전까지 port 검증은 `make hns` 빌드 기준으로 한다.

## seq 2~62 공통 사항

- 시작 HEAD: `733543267f`. 빌드 명령은 `GITHUB_ACTION=1 make hns -j6`(다른 worktree와 CPU 공유로 `-j6`).
- 기준 빌드(`733543267f`, 전체 재빌드): 종료 코드 0, ROM 32,735,268 B(97.56%), EWRAM 249,112 B(95.03%), IWRAM 25,636 B(78.23%), 경고 164건(기존 `-Woverride-init`·미사용 함수/변수 계열).
- 경고 비교: 매 빌드의 경고 목록(파일·메시지)을 기준 목록과 비교해 새 경고만 확인했다.
- 테스트 모드 컴파일: 테스트 파일을 바꾼 항목은 `make TEST=1 BUILD=hns build/hns-test/<파일>.o`로 해당 테스트 오브젝트만 컴파일해 문법·심볼 오류가 없는지 확인했다(링크·실행 안 함). seq 3~23의 테스트 12개(`innards_out`, `beads_of_ruin`, `vessel_of_ruin`, `aroma_veil`, `paralysis`, `event_object_movement`, `disguise`, `air_balloon`, `eject_pack`, `fling`, `iron_barbs`, `rocky_helmet`)는 seq 24 뒤에 한꺼번에 컴파일 성공을 확인했다.
- 자동 테스트: 전 항목 **미실행(테스트 힙 문제 조사 중)**. seq 1에서 확인한 `src/malloc.c:120` double-free assertion으로 배틀 테스트 대부분이 실패하므로 실행하지 않았다. upstream `test/**` 변경은 그대로 이식했고 영문 `MESSAGE` 기대값은 원문대로 두었다.

## 동기화 단위: seq 2 #9870 `U-9870` Improve error handling of smol compressor

- 현재 판정: 적용
- 커밋: `d037d758c8`
- upstream 근거: `e855ea1d40`
- 해결한 의존성: 없음
- 수정 파일: `tools/compresSmol/compressAlgo.cpp`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 해당 없음(그대로 적용, `git apply` 무충돌)
- 저장·ROM·그래픽 영향: 없음. 새 도구로 저장소의 기존 압축 산출물 7,812개(`.smol` 7,180·`.fastSmol` 147·`.smolTM` 485)를 다시 압축해 모두 바이트 동일함을 확인했다.
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,735,268 B / EWRAM 249,112 B / IWRAM 25,636 B(변화 없음), 새 경고 없음
  - 자동 테스트: 해당 테스트 없음
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 3 #9858 `U-9858` Fix Innards Out multihit damage basis and Core Enforcer suppression on fainted targets

- 현재 판정: 적용
- 커밋: `5470147fab`
- upstream 근거: `8e8c5c318b`
- 해결한 의존성: 없음(#9532 Bide 리팩터보다 먼저 이식)
- 수정 파일: `include/battle.h`, `src/battle_script_commands.c`, `src/battle_util.c`, `test/battle/ability/innards_out.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: `include/battle.h`는 HnS의 `u32 moveResultFlags[]`(Champions) 문맥 때문에 `innardsOutHpLost[]` 1줄만 `moveDamage[]` 뒤에 수동 삽입. 나머지 hunk는 그대로. 메시지 경로 변화 없음(`BattleScript_AftermathDmg` 재사용).
- 저장·ROM·그래픽 영향: `gBattleStruct`(heap) +8 B. 세이브 무관.
- 검증:
  - 코드 대조: HnS `MoveDamageDataHpUpdate`·`ClearDamageCalcResults` 호출 위치가 upstream과 같음을 확인
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,735,316 B(+48) / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). `innards_out.c`는 MESSAGE 의존 없음
  - 실기 확인: 선택(연속기로 내용물쏟기 보유자를 쓰러뜨릴 때 반격 피해량)
- 남은 위험: 낮음

## 동기화 단위: seq 4 #9855 `U-9855` Move animation fixes

- 현재 판정: 적용
- 커밋: `04262b2086`
- upstream 근거: `7b7a66dc13`
- 해결한 의존성: 없음
- 수정 파일: `src/battle_anim.c`(`Task_InitUpdateMonBg`의 중복 monbg `assertf`), `src/battle_anim_electric.c`(`CreateSprite` 5곳 → `CreateSpriteUnchecked`)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. HnS 전용 애니 스크립트가 새 assert에 걸리는지 정적 검사했다: `data/battle_anim_scripts.s`(monbg 사용 파일은 이것뿐)에서 `clearmonbg` 없이 `monbg`가 다시 나오는 경로(호출 서브루틴 포함)와 `clearmonbg` 직후 대기 없이 `monbg`가 오는 경로 모두 0건(upstream 같은 시점 파일도 0건).
- 저장·ROM·그래픽 영향: ROM +112 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,735,428 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 해당 테스트 없음
  - 실기 확인: 권장. HnS는 비릴리스 빌드라 assert 실패 시 재개 가능한 크래시 화면이 뜬다. 조건 분기(`jumpif*`) 안의 monbg는 정적 검사로 완전히 확인되지 않았으므로, `CreateSpriteUnchecked`로 바뀐 애니(Charge·Flash Cannon·Steel Beam·Volt Tackle·Fairy Lock·Collision Course·Shock Wave)와 HnS 추가 기술 애니를 실제로 재생해 크래시 화면이 없는지 본다.
- 남은 위험: 낮음(정적 검사 범위 밖의 분기 경로)

## 동기화 단위: seq 5 #9883 `U-9883` Move follower check before trainer hill check whene selecting script

- 현재 판정: 적용
- 커밋: `c7660cf1ee`
- upstream 근거: `84e02096fc`
- 해결한 의존성: 없음
- 수정 파일: `src/event_object_movement.c`, `src/field_control_avatar.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. 동반 포켓몬 스크립트 선택을 `GetInteractedObjectEventScript`로 옮기고 Trainer Hill 검사보다 먼저 둔다. 다른 `GetObjectEventScriptPointerByObjectEventId` 호출처(링크 플레이어·트레이너 시선·NPC follower)는 upstream과 같다.
- 저장·ROM·그래픽 영향: ROM +16 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,735,444 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 해당 테스트 없음
  - 실기 확인: 필요. Trainer Hill 안에서 동반 포켓몬에게 말 걸기, 일반 맵 동반 포켓몬 말 걸기.
- 남은 위험: 낮음

## 동기화 단위: seq 6 #9880 `U-ruin-9880` Fixes Ruin abilities / gastro acid interaction

- 현재 판정: 적용
- 커밋: `0da2294cbc`
- upstream 근거: `cfb4f864e9`
- 해결한 의존성: 없음(같은 unit의 #9901은 seq 11)
- 수정 파일: `data/battle_scripts_1.s`, `include/battle_util.h`, `src/battle_script_commands.c`, `src/battle_util.c`, `test/battle/ability/beads_of_ruin.c`, `test/battle/ability/vessel_of_ruin.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. `BattleScript_NeutralizingGasExits`는 루프 변수만 `gBattlerTarget`/`gEffectBattler`로 바뀌고 `STRINGID_NEUTRALIZINGGASOVER` 출력은 같다.
- 저장·ROM·그래픽 영향: ROM +192 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,735,636 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). 새 테스트 중 1줄은 영문 특성 MESSAGE 의존(알려진 한계)
  - 실기 확인: 선택(위액·화학변화가스와 재앙 특성)
- 남은 위험: 낮음

## 동기화 단위: seq 7 #9888 `U-9888` Fix compiling with -O0

- 현재 판정: 적용
- 커밋: `d15b8fb8a1`
- upstream 근거: `1cf36d1d82`
- 수정 파일: `src/battle_controllers.c`(테스트 전용 분기 `#if TESTING`), `src/ow_abilities.c`(함수 포인터 표 `static const ... *const`)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용
- 저장·ROM·그래픽 영향: ROM 변화 없음
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,735,636 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 해당 없음
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 8 #9882 `U-safari-9882` Safari fixes

- 현재 판정: 부분 적용(스크립트 hunk는 이미 적용)
- 커밋: `0e47928bba`
- upstream 근거: `57f3f4a0b6`
- 해결한 의존성: 없음(같은 줄을 #9912가 seq 13에서 다시 고침)
- 수정 파일: `src/battle_interface.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - `data/scripts/safari_zone.inc`의 `#ifdef IS_FRLG` → `#if IS_FRLG` 4곳은 HnS에 이미 있다(2·10·26행).
  - `UpdateLeftNoOfBallsTextOnHealthbox`는 HnS에 Gen4 UI 분기가 추가돼 문맥이 달라, 비-Gen4 분기의 지우기 폭 31 → 39만 수동 적용. Gen4 UI 분기와 한글 `gText_SafariBallLeft`는 그대로.
- 저장·ROM·그래픽 영향: ROM 변화 없음(상수만 변경)
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,735,636 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 해당 없음
  - 실기 확인: seq 13(#9912) 뒤에 사파리 배틀(비-Gen4 UI)에서 남은 볼 수 표시가 겹치지 않는지 확인
- 남은 위험: 낮음

## 동기화 단위: seq 9 #9872 `U-9872` fix(debug_menu): fix flicker when used when map pop-up active

- 현재 판정: 적용
- 커밋: `33dc2a9374`
- upstream 근거: `fab0777918`
- 수정 파일: `src/debug.c`(`Debug_ShowMenu`에 `CopyWindowToVram` 1줄)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용(디버그 메뉴 전용)
- 저장·ROM·그래픽 영향: ROM +16 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,735,652 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 해당 없음
  - 실기 확인: 불필요(디버그 메뉴)
- 남은 위험: 없음

## 동기화 단위: seq 10 #9905 `U-namebox-9905` fix (namebox): back 2 back namebox no longer flickers

- 현재 판정: 적용(1 hunk는 HnS에 이미 있음)
- 커밋: `5c30632c8c`
- upstream 근거: `74ba82ba85`
- 해결한 의존성: 없음. 후속 #10540(같은 unit)이 `TrySpawnAndShowNamebox`의 speaker NULL + 창 없음 경로를 고친다(아직 미이식, 이 경로에서 `DrawNamebox(WINDOW_NONE)` 가능성은 upstream 중간 상태와 같다).
- 수정 파일: `include/field_name_box.h`, `src/field_message_box.c`, `src/field_name_box.c`, `src/match_call.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - `src/field_name_box.c`: HnS 변경 없이 #9881 전 `INCBIN_U32` 문맥만 달라 문맥을 줄여 그대로 적용.
  - `src/match_call.c` `MatchCall_PrintIntro`: HnS `#if IS_HNS`의 `gSpeakerName = GetTrainerNameFromId(...)` 뒤에 `IsSpeakerBuffered(gStringVar4)` 가드를 넣었다. HnS 매치콜은 트레이너 이름이 설정돼 있으므로 이름 상자가 계속 표시된다.
  - `MatchCall_EndCall`의 `DestroyNamebox()` hunk는 HnS `4eccb85f8b`에 이미 있다.
- 저장·ROM·그래픽 영향: ROM +256 B. 세이브 무관.
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,735,908 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 해당 없음
  - 실기 확인: 필요. HnS 매치콜(포켓기어 전화) 이름 상자 표시·종료 후 표지판에 이름 상자가 남지 않는지, 연속 대사 이름 상자 깜빡임.
- 남은 위험: 낮음

## 동기화 단위: seq 11 #9901 `U-ruin-9880` Fix Ruin abilities respecting each other even if deactivated

- 현재 판정: 적용
- 커밋: `756d605a86`
- upstream 근거: `4c41f277db`
- 해결한 의존성: #9880(seq 6)
- 수정 파일: `src/battle_util.c`, `test/battle/ability/beads_of_ruin.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용(재앙 판정 4줄을 `ctx->abilityAtk/abilityDef` 비교로). 메시지 변화 없음.
- 저장·ROM·그래픽 영향: ROM −80 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,735,828 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). 추가 테스트는 MESSAGE 의존 없음
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 12 #9913 `U-9913` Cleanup of Dexnav checks

- 현재 판정: 적용
- 커밋: `44b9b01490`
- upstream 근거: `82be34e925`
- 수정 파일: `src/dexnav.c`(`sDexNavSearchDataPtr` NULL 검사 3곳, 루프 변수 정리)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. HnS `DEXNAV_ENABLED`는 FALSE라 현재 동작 변화 없음.
- 저장·ROM·그래픽 영향: ROM +64 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,735,892 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 해당 없음
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 13 #9912 `U-safari-9882` Fix font shadow not being cleared in safari healthbox

- 현재 판정: 적용
- 커밋: `44439cbdef`
- upstream 근거: `3425ea97f0`
- 해결한 의존성: #9882(seq 8)
- 수정 파일: `src/battle_interface.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: #9882와 같은 줄(비-Gen4 분기) 39 → 40. Gen4 UI 분기·한글 문구 그대로.
- 저장·ROM·그래픽 영향: ROM 변화 없음
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,735,892 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 해당 없음
  - 실기 확인: 필요(비-Gen4 UI 사파리 배틀에서 남은 볼 수가 줄어들 때 이전 숫자·그림자가 남지 않는지). Gen4 UI는 챌린지 설정 `newBattleUI`로 켜지므로 이 옵션을 끈 상태에서 확인.
- 남은 위험: 낮음

## 동기화 단위: seq 14 #9921 `U-9921` Fix Aroma Veil not blocking Destiny Knot infatuation

- 현재 판정: 적용
- 커밋: `75add72fc5`
- upstream 근거: `19c842d97d`
- 수정 파일: `src/battle_script_commands.c`(`Cmd_tryinfatuating` 앞머리 아로마베일 검사), `test/battle/ability/aroma_veil.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. 기존 `BattleScript_AromaVeilProtectsRet`(특성 팝업 + 한글 `STRINGID_AROMAVEILPROTECTED`)을 재사용하고 새 문자열은 없다. 운명의실 반사 헤롱헤롱이 아로마베일에 막히면 팝업·아로마베일 문구 뒤 기존 실패 경로(`BattleScript_TryDestinyKnotTargetFailed`의 `STRINGID_BUTITFAILED`)가 출력된다. upstream 동작이며 기존 ID라 `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 기록 대상이 아니다(group plan 판정).
- 저장·ROM·그래픽 영향: ROM +64 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,735,956 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). 새 테스트 1줄 영문 MESSAGE 의존(알려진 한계)
  - 실기 확인: 선택(운명의실 + 아로마베일 헤롱헤롱 반사 시 메시지 순서)
- 남은 위험: 낮음

## 동기화 단위: seq 15 #9917 `U-9917` Fix incorrect ability battle history recording

- 현재 판정: 적용
- 커밋: `a0f15740e3`
- upstream 근거: `7377e88396`
- 수정 파일: `src/battle_script_commands.c`(`Cmd_tryswapabilities`, `BS_TryEntrainment`), `src/battle_util.c`(`CanMoveSkipAccuracyCalc`)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: `battle_script_commands.c`는 그대로. `CanMoveSkipAccuracyCalc`는 HnS가 이미 `struct BattleCalcValues *cv` 판(1.17.0 최종형과 같은 시그니처)이라 `abilityBattler` 변수와 대입 3줄·`RecordAbilityBattle(abilityBattler, ability)`를 수동 삽입. AI 전투 기록만 바뀌고 메시지 변화 없음.
- 저장·ROM·그래픽 영향: ROM +64 B
- 검증:
  - 코드 대조: upstream master 판(`abilityBattler` 사용)과 같은 형태
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,020 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 해당 테스트 없음
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 16 #9925 `U-9925` Prevent reflection tag palette collisions

- 현재 판정: 적용(HnS 적응 1곳)
- 커밋: `940489468e`
- upstream 근거: `44b6958d8c`
- 수정 파일: `src/field_effect_helpers.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - group plan의 사전 확인 항목(0x3000~0x3010 사용 여부)을 검사한 결과 **충돌이 있었다.** HnS 전용 서핑 포켓몬 팔레트 태그가 `PAL_TAG_SQUIRTLE_SURF = 0x3001`부터 139개(0x3001~0x308B, `src/data/object_events/surfable/surfable_pokemon_templates.h`)를 쓴다. upstream 값이면 raw 반사 태그(0x3000~0x300F)와 높은 다리 반사 태그(0x3010)가 서핑 포켓몬 팔레트와 겹친다.
  - 그래서 `PAL_RAW_REFLECTION_OFFSET`만 0x3100으로 두었다(raw 0x3100~0x310F, 다리 0x3110). 소스 전체 숫자 태그 정의를 검사해 0x3100~0x31FF와 0x1900~0x19FF(0x11xx 오브젝트 태그 + 0x800)에 다른 태그가 없음을 확인했다. 나머지(`PAL_TAG_REFLECTION_OFFSET 0x0800`, `NUM_SPECIES` STATIC_ASSERT, `IndexOfSpritePaletteTag` 0xFF 비교)는 upstream 그대로.
  - 이전 HnS 값(raw 0x4000~, 다리 0x4010)은 동반 포켓몬 팔레트 태그(종+0x4000)와 겹쳤으므로 이 수정으로 해소된다.
- 저장·ROM·그래픽 영향: ROM 변화 없음. 세이브 무관.
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,020 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음(`NUM_SPECIES <= 0x800` STATIC_ASSERT 통과)
  - 자동 테스트: 해당 없음
  - 실기 확인: 필요. 물가에서 동반 포켓몬·NPC·플레이어 반사 색, 서핑 포켓몬(HnS 서핑 탈것)으로 서핑 중 높은 다리 위 NPC 반사, 이로치 동반 포켓몬 반사.
- 남은 위험: 낮음(0x3100 선택은 HnS 적응값이다. 이후 upstream이 이 값을 다시 바꾸면 서핑 태그 범위와 다시 대조해야 한다)

## 동기화 단위: seq 17 #9929 `U-9929` Correct status move miss messaging

- 현재 판정: 적용
- 커밋: `2be66d7011`
- upstream 근거: `21e41d55dd`
- 수정 파일: `data/battle_scripts_1.s`, `include/battle_scripts.h`, `src/battle_script_commands.c`, `test/battle/status1/paralysis.c`, `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. 새 `BattleScript_TargetAvoidsAttackEnd`는 기존 한글 `gMissStringIds[B_MSG_AVOIDED_ATK]` = `STRINGID_PKMNAVOIDEDATTACK`("…에게는 맞지 않았다!")을 출력하므로 새 문자열이 없다. **출력 변화:** `accuracycheck BattleScript_ButItFailed`를 쓰는 변화기(현재 13곳)가 빗나가면 "그러나 실패하고 말았다!" 대신 "…에게는 맞지 않았다!"가 나온다(공식 게임과 같음). group plan 지시대로 `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 기술·필드 상태 효과 표에 1행 추가했다. 이후 #9939(명중 판정 canceler 통합)가 이 분기를 옮긴다.
- 저장·ROM·그래픽 영향: ROM +64 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,084 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). `paralysis.c` 새 테스트는 영문 MESSAGE 의존(알려진 한계)
  - 실기 확인: 필요(전기자석파·최면술 등 명중 판정 변화기가 빗나갈 때 "맞지 않았다" 문구)
- 남은 위험: 낮음

## 동기화 단위: seq 18 #9930 `U-9930` Fix FRLG player sprite glitch when fishing after surfing

- 현재 판정: 적용
- 커밋: `0916127143`
- upstream 근거: `ba783f681c`
- 수정 파일: `src/event_object_movement.c`(`LoadSheetGraphicsInfo`·`ObjectEventSetGraphics` 크기 비교를 `images->size`로), `test/event_object_movement.c`(신규)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. 두 번째 hunk는 `!OW_GFX_COMPRESS` 조건이라 HnS 기본 설정에서는 효과가 제한적.
- 저장·ROM·그래픽 영향: ROM −16 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,068 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). 새 테스트는 MESSAGE 의존 없음
  - 실기 확인: 권장(서핑 후 낚시, 자전거·서핑 전환 시 플레이어 스프라이트)
- 남은 위험: 낮음

## 동기화 단위: seq 19 #9943 `U-airballoon-9943` Fix Air Balloon not popping when damaging moves hit a Substitute

- 현재 판정: 적용
- 커밋: `6699ae7d3c`
- upstream 근거: `3fc40c8d12`
- 수정 파일: `src/battle_hold_effects.c`(`TryAirBalloon` `EXCLUDING_SUBSTITUTES` → `INCLUDING_SUBSTITUTES`), `test/battle/ability/disguise.c`, `test/battle/hold_effect/air_balloon.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. 기존 `BattleScript_AirBalloonMsgPop`·한글 `STRINGID_AIRBALLOONPOP` 사용. upstream 테스트의 공백만 있는 줄 1개는 `git diff --check` 통과를 위해 비웠다.
- 저장·ROM·그래픽 영향: ROM +16 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,084 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). `air_balloon.c` MESSAGE 의존(알려진 한계)
  - 실기 확인: 선택(대타출동에 공격이 맞을 때 풍선 터짐)
- 남은 위험: 없음

## 동기화 단위: seq 20 #9946 `U-9946` Fix Eject Pack switch when the holder is trapped

- 현재 판정: 적용
- 커밋: `e9f14f03bd`
- upstream 근거: `5de54b9757`
- 수정 파일: `data/battle_scripts_1.s`(`BattleScript_EjectPackActivates`의 `jumpifcantswitch`에 `SWITCH_IGNORE_ESCAPE_PREVENTION`), `test/battle/hold_effect/eject_pack.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. 문자열 변화 없음.
- 저장·ROM·그래픽 영향: ROM 변화 없음
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,084 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). `eject_pack.c` MESSAGE 의존(알려진 한계)
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 21 #9951 `U-9951` Fix Fling Berry effects not being blocked by Covert Cloak and Shield Dust

- 현재 판정: 적용
- 커밋: `b2f9c31e1c`
- upstream 근거: `9f6313b51b`
- 수정 파일: `src/battle_script_commands.c`(`BS_TryFlingHoldEffect` 두 분기 순서 교체), `test/battle/move_effect/fling.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. 인분·비밀망토 대상에게 열매를 던지면 기존 `BattleScript_FlingBlockedByShieldDust`(한글 `STRINGID_ITEMWASUSEDUP`)로 간다. 새 문자열 없음. 기존 ID를 쓰는 upstream 판정 순서 수정이라 `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 기록 대상이 아니다(group plan). #9176(seq 59)이 이 함수를 다시 리팩터한다.
- 저장·ROM·그래픽 영향: ROM 변화 없음
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,084 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). `fling.c` 새 테스트 MESSAGE 의존 없음
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 22 #9952 `U-airballoon-9943` Fix Air Balloon not popping when holder faints from a damaging hit

- 현재 판정: 적용
- 커밋: `b479540f00`
- upstream 근거: `cbeac29501`
- 해결한 의존성: #9943(seq 19)
- 수정 파일: `src/battle_hold_effects.c`(`ItemBattleEffects` 기절 배틀러 예외 목록에 `HOLD_EFFECT_AIR_BALLOON`), `test/battle/hold_effect/air_balloon.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. 기절 시에도 기존 `STRINGID_AIRBALLOONPOP`(한글)이 나온다. HnS `BattleScript_AirBalloonMsgPop`은 upstream과 같은 구조(아이템 팝업 없음).
- 저장·ROM·그래픽 영향: ROM 변화 없음(정렬 범위 내)
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,084 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). MESSAGE 의존(알려진 한계)
  - 실기 확인: 선택(풍선 보유자가 공격으로 기절할 때 풍선 터짐 문구)
- 남은 위험: 없음

## 동기화 단위: seq 23 #9937 `U-9937` Fix stale hit state after multihit moves miss mid-sequence

- 현재 판정: 적용
- 커밋: `17a7b84993`
- upstream 근거: `99f230f3e5`
- 수정 파일: `src/battle_script_commands.c`(`Cmd_resultmessage` 연속기 중간 빗나감 분기에 초기화 2줄), `test/battle/ability/iron_barbs.c`, `test/battle/hold_effect/rocky_helmet.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. HnS Champions 상성 메시지 분기와 다른 위치라 메시지 변화 없음.
- 저장·ROM·그래픽 영향: ROM +48 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,132 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). MESSAGE 의존(알려진 한계)
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 24 #7360 `U-7360` Add EFFECTIVENESS_SE for tests

- 현재 판정: 적용(HnS 적응)
- 커밋: `5c67712698`
- upstream 근거: `ba6a063abf`, 최종형 upstream master `battle_move_resolution.c` `UpdateEffectivenessResultFlagsForDoubleSpreadMoves`(#10362 이후)
- 수정 파일: `src/battle_script_commands.c`, `include/test/battle.h`, `include/test_runner.h`, `test/test_runner_battle.c`, `test/battle/spread_moves.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작(group plan approach 그대로):
  - 효과음 경로(`Cmd_effectivenesssound`)의 `UpdateEffectivenessResultFlagsForDoubleSpreadMoves`를 1.17.0 최종형 논리로 교체했다: 무효 대상·대미지 무효화(탈 등) 대상 제외, 유효 대상 플래그를 OR, 1배 이상 대상이 있으면 NVE 제거, `MOVE_RESULT_HIGH_EFFECTIVENESS`가 하나라도 있으면 SE. HnS(Champions) `HIGH/LOW_EFFECTIVENESS` 매크로를 쓴다.
  - `Cmd_attackanimation`은 #10362(canceler) 이식 전까지 기존 "최선 결과" 선택을 유지하도록 옛 함수를 `GetBestResultFlagsForDoubleSpreadMoveAnimation`으로 이름만 바꿔 남겼다(최종형은 모든 대상 무효 시 0을 돌려 애니 재생 판정이 달라지기 때문).
  - 테스트 훅 `TestRunner_Battle_RecordEffectivenessSound`를 HnS 효과음 분기 5곳(EXTREMELY/MOSTLY 케이스 포함)에 `#if TESTING`으로 넣었다. 러너·매크로·테스트는 그대로(upstream `test_runner_battle.c` 끝 빈 줄 1개는 `git diff --check` 때문에 제거).
  - 메시지 선택은 불변. **효과음 변화:** 더블 범위기가 보통+별로 대상이면 NVE 효과음 대신 보통 효과음.
- 저장·ROM·그래픽 영향: ROM +256 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,388 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 테스트 모드 컴파일: `make TEST=1 BUILD=hns`로 `test_runner_battle.o`, `spread_moves.o`, `src/battle_script_commands.o`만 컴파일 성공(링크·실행은 안 함)
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). `spread_moves.c` 기존 테스트는 영문 MESSAGE 의존(알려진 한계)
  - 실기 확인: 선택(더블배틀 범위기: 효과 굉장+보통, 별로+보통 조합 효과음)
- 남은 위험: 낮음. #10362 이식 때 애니 판정 helper를 제거하고 최종형으로 합친다.

## 동기화 단위: seq 25 #9942 `U-futuresight-9942` Future Sight fixes and tests (+ 같은 unit의 #10647)

- 현재 판정: 적용(HnS 적응). 같은 unit의 후속 회귀 수정 #10647(순서표 seq 329)도 함께 적용했다.
- 커밋: `c10979491d`(#9942), `7afa522502`(#10647)
- upstream 근거: `1de47539af`(#9942), `fb786a3d64`(#10647)
- 해결한 의존성: 없음. #10161(seq 228, 1.16.2 후속)의 전제.
- 수정 파일: `include/constants/battle_move_resolution.h`, `src/battle_move_resolution.c`(`CANCELER_INTERRUPTIBLE_MOVES`), `src/battle_util.c`, `test/battle/move_effect/future_sight.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - `battle_util.c`는 HnS `CalculateMoveDamage`의 OHKO 성공 플래그 블록(`STRINGID_ONEHITKO`용)이 인접 문맥이라 수동 이식했다. `DoFutureSightAttackDamageCalcVars` 삭제, `AllocSaveBattleMons` + `PokemonToBattleMon` 기반 `DoFutureSightAttackDamageCalc`로 교체, 상성·급소 계산을 `DoMoveDamageCalc` 안으로 이동. OHKO 블록은 그대로 보존.
  - #9942 판은 상성 0 조기 반환에서 `gBattleMons` 백업을 복원·해제하지 않는다(누수 + 파티 몬 데이터가 배틀러에 남음). g2 plan이 "#9942 이식 때 같은 단위에서 넣는다"고 정한 #10647을 바로 뒤 커밋으로 넣었다. upstream은 탭·후행 공백을 썼으나 HnS 들여쓰기(공백)로 옮겼다.
  - 메시지 경로 변화 없음.
- 저장·ROM·그래픽 영향: ROM −560 B(#9942), +16 B(#10647). 미래예지 발동 시 `BattlePokemon` 4개 크기 임시 heap 할당.
- 검증:
  - 코드 대조: `CalculateMoveDamage` 호출처(`CalculateAndSetMoveDamage`, AI, battle_tv, 반동 계산 2곳)가 upstream과 같은 전제(호출 뒤 `ctx->isCrit`/상성 재사용 없음)임을 확인
  - `git diff --check`: 통과
  - `make hns -j6`: #9942 종료 코드 0, ROM 32,735,828 B / EWRAM 249,112 B / IWRAM 25,636 B. #10647 종료 코드 0, ROM 32,735,844 B / EWRAM 249,112 B / IWRAM 25,636 B. 새 경고 없음
  - 테스트 모드 컴파일: `future_sight.o` 성공
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). `future_sight.c` MESSAGE 의존 없음
  - 실기 확인: 필요. 미래예지 시전자를 교체한 뒤 공격이 들어갈 때 대미지·교체된 배틀러 표시, 상성 무효 대상(악 타입)에게 들어갈 때 이후 배틀러 능력치가 흐트러지지 않는지.
- 남은 위험: 중간. heap 할당이 늘어나므로 테스트 힙 문제 조사 결과와 함께 본다. seq 329(#10647) 담당은 이 커밋을 "이미 적용"으로 처리하면 된다.

## 동기화 단위: seq 26 #9954 `U-9954` Fix Guard Dog activating when Intimidate is blocked or cannot lower Attack

- 현재 판정: 적용
- 커밋: `a2955ee8f8`
- upstream 근거: `8bb8984c7e`
- 수정 파일: `src/battle_script_commands.c`(`BS_JumpIfIntimidateAbilityPrevented`의 `ABILITY_GUARD_DOG` 분기), `test/battle/ability/guard_dog.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용(흰안개·공격 최저 단계·플라워베일 선행 검사). 문자열 변화 없음. 1.15.3 판 임시 수정이며 #9730 이식 때 #9957(`battle_stat_change.c` `IsIntimidateBlocked`) 판으로 대체된다.
- 저장·ROM·그래픽 영향: ROM +160 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,004 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 테스트 모드 컴파일: `guard_dog.o` 성공
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). MESSAGE 의존(알려진 한계)
  - 실기 확인: 선택
- 남은 위험: 없음

## 동기화 단위: seq 27 #9977 `U-9977` Fix failing Dream Eater test when target is behind Substitute (Gen 5+)

- 현재 판정: 적용
- 커밋: `2f56496c05`
- upstream 근거: `68a5890c85`
- 수정 파일: `data/battle_scripts_1.s`(`BattleScript_EffectDreamEater` 컴파일 타임 `.if` → `jumpifgenconfiglowerthan CONFIG_B_DREAM_EATER_SUBSTITUTE`), `include/constants/generational_changes.h`, `test/battle/move_effect/dream_eater.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 스크립트·테스트는 그대로. `generational_changes.h`는 HnS 목록(`B_RAGE_FIST` 등) 문맥 때문에 `B_DREAM_EATER_LIQUID_OOZE` 다음에 1줄 수동 삽입. HnS `B_DREAM_EATER_SUBSTITUTE`는 `GEN_LATEST`라 실행 결과는 전후 같다. 실패 시 출력은 기존 `BattleScript_DoesntAffectTargetAtkString`(한글 문구·순서 불변). 설정 표는 ROM 상수라 세이브 무관.
- 저장·ROM·그래픽 영향: ROM +32 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,036 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 테스트 모드 컴파일: `dream_eater.o` 성공
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). 기존 테스트 영문 MESSAGE 의존(알려진 한계)
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 28 #9976 `U-9976` Fix Red Card activating when attacker faints

- 현재 판정: 적용
- 커밋: `ee08800374`
- upstream 근거: `2a3bf97bed`
- 수정 파일: `src/battle_move_resolution.c`(`TryRedCard`에 공격자 생존 검사), `test/battle/hold_effect/red_card.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. HnS 레드카드 아이템 팝업 스크립트는 그대로. #9784(탈출 아이템 재편)보다 먼저 이식.
- 저장·ROM·그래픽 영향: ROM +64 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,100 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 테스트 모드 컴파일: `red_card.o` 성공
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). MESSAGE 의존(알려진 한계)
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 29 #9991 `U-9991` Bug fix: On-step scripts now only happen once the FNPC out of door task is complete

- 현재 판정: 적용
- 커밋: `c2cfd1050a`
- upstream 근거: `82dc0eb576`
- 수정 파일: `include/field_control_avatar.h`, `include/follower_npc.h`, `src/field_control_avatar.c`, `src/follower_npc.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. `GetPlayerPosition`/`TryStartStepBasedScript`/`Task_FollowerNPCOutOfDoor` 공개, 문 밖으로 나오는 NPC 동행자 태스크 중에는 발판 스크립트를 미루고 태스크 끝에 한 번 실행. HnS `FNPC_ENABLE_NPC_FOLLOWERS`는 FALSE라 실제 발동 경로는 없다(걸음마다 `FindTaskIdByFunc` 1회만 추가).
- 저장·ROM·그래픽 영향: ROM +288 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,388 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 해당 테스트 없음
  - 실기 확인: 불필요(기능 꺼짐). 걸음 발판 이벤트(좌표 스크립트)가 평소대로 동작하는지만 일반 플레이에서 확인
- 남은 위험: 없음

## 동기화 단위: seq 30 #10000 `U-10000` Add test for Curse + Baton Pass interaction

- 현재 판정: 적용(테스트 전용)
- 커밋: `ad1947c25c`
- upstream 근거: `3bf7c9f74c`
- 수정 파일: `test/battle/move_effect/curse.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. 영문 `SEND_IN_MESSAGE("Wynaut")` 기대값은 원문 유지.
- 저장·ROM·그래픽 영향: 게임 ROM 불변(테스트 전용이라 빌드 생략)
- 검증:
  - `git diff --check`: 통과
  - 테스트 모드 컴파일: `curse.o` 성공
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). 영문 SEND_IN_MESSAGE 때문에 한글 HnS에서는 실패 예상(알려진 한계), HP_BAR 검사는 언어 무관
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 31 #9973 `U-fillsprite-9973` Fix `FillSpriteRect` wrongly triggering an assert

- 현재 판정: 적용
- 커밋: `c24b95141d`
- upstream 근거: `290703355a`
- 해결한 의존성: 없음. 같은 unit의 #9813(seq 165)·#10349(seq 273)·#10392(seq 275)는 각 순서에서 이식한다.
- 수정 파일: `src/sprite.c`(`FillSpriteRect` X축 스프라이트 전환 위치를 채우기 전으로 이동)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. 끝 위치가 스프라이트 경계와 맞으면 다음 스프라이트(`nextX`)로 넘어가려다 assert가 나던 문제를 고친다.
- 저장·ROM·그래픽 영향: ROM +16 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,404 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 해당 테스트 없음
  - 실기 확인: 권장. `FillSpriteRectColor` 사용처(체력바·HP 숫자·사파리 볼 수, `src/battle_interface.c`)를 싱글/더블, Gen4 UI 켬/끔으로 확인.
- 남은 위험: 낮음

## 동기화 단위: seq 32 #10002 `U-10002` Fix Toxic Debris checking Toxic Spikes cap on the wrong side

- 현재 판정: 적용
- 커밋: `28c44999f1`
- upstream 근거: `21c2420299`
- 수정 파일: `src/battle_util.c`(`ABILITY_TOXIC_DEBRIS` 독압정 한도 검사를 `BATTLE_OPPOSITE(gBattlerTarget)` 쪽으로), `test/battle/ability/toxic_debris.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 로직은 그대로. HnS에는 바로 뒤에 `ABILITY_SPICY_SPRAY` 케이스(HnS 추가분)가 있어 `git apply`가 새 블록의 닫는 중괄호를 그 케이스 뒤에 붙였다. 닫는 중괄호를 독성잔해 케이스 `break` 바로 뒤로 옮겨 upstream 구조와 맞췄다. #10542가 이후 `BATTLE_OPPOSITE`를 없앨 때 함께 치환한다.
- 저장·ROM·그래픽 영향: ROM +16 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,420 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 테스트 모드 컴파일: `toxic_debris.o` 성공
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). MESSAGE 의존(알려진 한계)
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 33 #10004 `U-10004` Fix reverse-cry off-by-one if Porygon disabled

- 현재 판정: 적용
- 커밋: `4177b10846`
- upstream 근거: `a6dea8e6aa`
- 수정 파일: `sound/cry_tables.inc`(`gCryTable_Reverse`의 `.if P_FAMILY_PORYGON`를 `cry_reverse Cry_Porygon` 아래로)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. 2026-09-16 64MiB 철회 때 메가 울음소리와 함께 되돌린 기록은 32MiB 결정의 부수 롤백이며 이 PR 거부가 아니다(group plan). HnS `P_FAMILY_PORYGON`=TRUE라 출력 불변.
- 저장·ROM·그래픽 영향: 없음. 빌드 결과 `pokehns.gba`가 적용 전과 바이트 동일.
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,420 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음, ROM 바이트 동일
  - 자동 테스트: 해당 없음
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 34 #9995 `U-9995` Prevent error in title screen cinematic if Kyogre/Groudon are disabled

- 현재 판정: 적용
- 커밋: `5f4625a97c`
- upstream 근거: `6a26e421da`
- 수정 파일: `src/intro.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: include 문맥(HnS `intro_hns.h`)만 달라 문맥을 줄여 적용. `#include "pokemon.h"`는 HnS include 묶음 뒤, 그라돈·가이오가 울음소리를 `IsSpeciesEnabled`로 감쌌다. HnS 기본 인트로는 `SetUpCopyrightScreenHns` 경로라 현재 출력 불변.
- 저장·ROM·그래픽 영향: ROM 변화 없음(정렬 범위 내)
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,420 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 해당 없음
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 35 #10007 `U-10007` Fix Infiltrator bypassing ally Safeguard for confusion effects

- 현재 판정: 적용
- 커밋: `f9452927e3`
- upstream 근거: `28645b21b0`
- 수정 파일: `asm/macros/battle_script.inc`(`jumpifsafeguard`에 `jumpiftargetally 2f`), `test/battle/ability/infiltrator.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. 아군 대상이면 침투 무시 분기를 건너뛰어 아군의 신비의부적이 적용된다. 문자열 변화 없음. 매크로 사용처(혼란 계열 스크립트) 전체에 영향.
- 저장·ROM·그래픽 영향: ROM +48 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,468 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 테스트 모드 컴파일: `infiltrator.o` 성공
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). MESSAGE 의존 없음
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 36 #10017 `U-10017` Set higher HP in Gem Damage calculation test fixing gen5 config result

- 현재 판정: 적용(테스트 전용)
- 커밋: `15175164b5`
- upstream 근거: `74347e1c40`
- 수정 파일: `test/battle/damage_formula.c`(보석 대미지 테스트 `KNOWN_FAILING` 제거, 상대 HP 999)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. MESSAGE 의존 없음. HnS 보석 배율 분기(Champions `#if`)에 따라 기대값이 달라질 수 있어 테스트 러너 복구 후 실행 확인 필요.
- 저장·ROM·그래픽 영향: 게임 ROM 불변(테스트 전용이라 빌드 생략)
- 검증:
  - `git diff --check`: 통과
  - 테스트 모드 컴파일: `damage_formula.o` 성공
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중)
  - 실기 확인: 불필요
- 남은 위험: 테스트 실행 시 HnS 보석 배율 때문에 실패할 수 있다(로직 확인 필요 항목으로 남김).

## 동기화 단위: seq 37 #10021 `U-10021` Fix Electrify status move test expectations

- 현재 판정: 적용(테스트 전용)
- 커밋: `1031e1f6f8`
- upstream 근거: `148ab2d1b5`
- 수정 파일: `test/battle/move_effect/electrify.c`(`KNOWN_FAILING` 제거, 고지전기 대상을 쥬피썬더 축전으로)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. 영문 `MESSAGE("The opposing Jolteon restored HP using its Volt Absorb!")`는 원문 유지.
- 저장·ROM·그래픽 영향: 게임 ROM 불변(테스트 전용이라 빌드 생략)
- 검증:
  - `git diff --check`: 통과
  - 테스트 모드 컴파일: `electrify.o` 성공
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). 영문 MESSAGE로 실패 예상(알려진 한계), ABILITY_POPUP·HP_BAR는 언어 무관
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 38 #10011 `U-10011` Align outdated move descriptions with current mechanics

- 현재 판정: 적용
- 커밋: `24eb5d730d`
- upstream 근거: `4a9c709d7e`
- 수정 파일: `src/data/moves_info.h`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 같은 항목의 `.name`이 한글이라 문맥이 달라 수동 적용. 킹실드·페이탈클로·암석액스·비검천중파의 영문 `.description`만 upstream 문구로 바꾸고 킹실드는 `B_KINGS_SHIELD_LOWER_ATK` 분기를 넣었다(HnS 값 `GEN_LATEST` → "Evades damage, reducing / Attack if struck."). 한글 `.name` 줄은 그대로. HnS 기술 설명 899개는 아직 영문이며 `GetMoveDescription`이 그대로 쓴다.
- 저장·ROM·그래픽 영향: ROM −16 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,452 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 해당 없음
  - 실기 확인: 불필요(영문 설명 문구). 기술 설명 한글화 때 새 문구 기준으로 번역.
- 남은 위험: 없음

## 동기화 단위: seq 39 #10028 `U-10028` Fix King’s Rock flinch chance with Serene Grace and rainbow

- 현재 판정: 적용
- 커밋: `6cae4092c4`
- upstream 근거: `6cc7f6638f`
- 수정 파일: `src/battle_hold_effects.c`(`TryKingsRock` 하늘의은총/무지개 2배를 OR 한 번으로), `test/battle/hold_effect/flinch.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. 두 조건이 겹칠 때 4배가 되던 문제 수정. 메시지 변화 없음.
- 저장·ROM·그래픽 영향: ROM −16 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,436 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 테스트 모드 컴파일: `flinch.o` 성공
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). MESSAGE 의존(알려진 한계)
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 40 #10003 `U-10003` Fix incorrect Loaded Dice priority over Skill Link for Population Bomb

- 현재 판정: 적용
- 커밋: `8e8278db4d`
- upstream 근거: `0670ab8dc1`
- 수정 파일: `src/battle_move_resolution.c`(`CancelerMultihitMoves` 쥐들의 행진 분기에 스킬링크 제외), `test/battle/move_effect/population_bomb.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. HnS canceler ctx가 `abilityAtk`·`holdEffectAtk`를 이미 채운다(`battle_move_resolution.c` 2091~2092행). 메시지 변화 없음.
- 저장·ROM·그래픽 영향: ROM +16 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,452 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 테스트 모드 컴파일: `population_bomb.o` 성공
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). MESSAGE 의존(알려진 한계)
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 41 #10035 `U-10035` Fix Strength Sap activation on miss

- 현재 판정: 적용
- 커밋: `3c9c5ab6be`
- upstream 근거: `c70fcd906d`
- 수정 파일: `src/battle_move_resolution.c`(`MoveEndAbsorb` `EFFECT_STRENGTH_SAP` 조건에 `!IsBattlerUnaffectedByMove(gBattlerTarget)`)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. 메시지 변화 없음(빗나갔을 때 회복 스크립트가 돌지 않을 뿐).
- 저장·ROM·그래픽 영향: ROM +32 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,484 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 해당 테스트 없음
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 42 #9949 `U-mapjson-9949` mapjson assume undefined layout and maps match the compiled version

- 현재 판정: 부분 적용(HnS 적응)
- 커밋: `643874efb5`
- upstream 근거: `fcc372d85b`(mapjson 10줄 + map.json 470개 `region` 추가 + migration 스크립트)
- 해결한 의존성: 없음(같은 unit의 #10198은 이후 순서)
- 수정 파일: `tools/mapjson/mapjson.cpp`
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - HnS mapjson은 `region` 대신 `game_version`/`layout_version`으로 게이팅한다(`84700339f5`, `7cbe3a021e`). upstream hunk는 그대로 쓸 수 없어 의도("값이 없으면 컴파일 버전으로 간주")만 옮겼다.
  - `get_default_game_version()`: firered 빌드는 `frlg`, emerald·hns 빌드는 지금처럼 `emerald`. hns 빌드는 명시적 `game_version: hns` 데이터만 포함하므로 HnS 맵 집합이 그대로다(group plan approach).
  - `layout_matches_version`: `game_version`이 없고 `layout_version`이 emerald/frlg로 명시된 레이아웃은 그 버전을 따른다. HnS `layouts.json`에는 `game_version` 없이 `layout_version: emerald`인 레이아웃이 441개 있어, 이 처리가 없으면 firered 빌드에 에메랄드 레이아웃이 섞인다.
  - `generate_layout_headers_text`의 `layout_version` 기본값도 같은 함수로.
  - 제외: map.json 470개의 `"region"` 줄 추가와 `migration_scripts/add_region_hoenn_attribute_to_hoenn_maps.py`. HnS mapjson이 `region`을 읽지 않아 결과에 영향이 없다(group plan "선택" 항목).
- 저장·ROM·그래픽 영향: 없음
- 검증:
  - 새·옛 mapjson을 따로 빌드해 emerald·firered·hns 세 버전으로 `groups`·`layouts`를 스크래치 디렉터리에 생성·비교: 세 버전 모두 옛 도구와 바이트 동일. hns 결과는 저장소의 `data/maps/{groups,headers,events,connections}.inc`(경로 접두어만 정규화), `include/constants/map_groups.h`, `data/layouts/layouts.inc`·`layouts_table.inc`, `include/constants/layouts.h`, `src/data/heal_locations.json`과 동일.
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,484 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음, `pokehns.gba` 바이트 동일
  - 자동 테스트: 해당 없음
  - 실기 확인: 불필요
- 남은 위험: 없음. 이후 새 맵·레이아웃을 추가할 때 hns 빌드에 넣으려면 지금처럼 `game_version: hns`를 명시해야 한다.

## 동기화 단위: seq 43 #10047 `U-10047` Fix Poison Puppeteer target tracking

- 현재 판정: 적용
- 커밋: `764e6894c5`
- upstream 근거: `dd2b6e41d7`
- 수정 파일: `include/battle.h`, `src/battle_move_resolution.c`, `src/battle_script_commands.c`, `src/battle_util.c`, `test/battle/ability/poison_puppeteer.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. `BattleStruct.poisonPuppeteerConfusion` 전역 플래그를 `SpecialStatus.poisonPuppeteer`(대상별, 패딩 1비트 사용)로 옮기고 혼란 부여 시 `gBattleScripting.battler`/`gEffectBattler`를 명시한다. 출력은 기존 `BattleScript_AbilityStatusEffect`(특성 팝업 + 한글 혼란 문구) 그대로. HnS에 이 플래그의 다른 사용처는 없었다.
- 저장·ROM·그래픽 영향: ROM +16 B. `SpecialStatus`·`BattleStruct` 크기 불변(패딩 사용), EWRAM 불변.
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,500 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 테스트 모드 컴파일: `poison_puppeteer.o` 성공
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). MESSAGE 의존 없음
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 44 #10040 `U-10040` Fix Teraform activations timing

- 현재 판정: 적용(HnS 적응)
- 커밋: `3e5a4b8677`
- upstream 근거: `69aa5bb05a`, 1.17.0 `BattleScript_TeraFormChange` 순서(switchinabilities → abilityonformchange → effectsafterformchange)
- 수정 파일: `asm/macros/battle_script.inc`, `data/battle_scripts_1.s`, `include/battle_util.h`, `src/battle_script_commands.c`, `src/battle_util.c`, `test/battle/ability/teraform_zero.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - `ABILITYEFFECT_ON_FORM_CHANGE`·`BS_AbilityOnFormChange`·`abilityonformchange` 매크로 추가. HnS `BattleScript_TeraFormChange`에는 이미 `effectsafterformchange`(#10180 선행 이식)가 있어 문맥이 달랐다. `switchinabilities` 다음, `effectsafterformchange` 앞에 넣어 1.17.0 순서와 맞췄다. 매크로와 명령 함수는 `switchinabilities`/`BS_SwitchinAbilities` 바로 뒤에 두었다.
  - `ON_SWITCHIN`의 `TERAFORM_ZERO`(종 검사) 분기 삭제는 그대로.
  - `UpdateEffectivenessResultFlagsForDoubleSpreadMoves`의 공백 전용 hunk는 HnS 함수 형태가 달라(#7360 적응) 버렸다.
  - 문구 변화 없음(테라폼제로 발동 스크립트 기존 것).
- 저장·ROM·그래픽 영향: ROM +128 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,628 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 테스트 모드 컴파일: `teraform_zero.o` 성공
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). MESSAGE 의존(알려진 한계)
  - 실기 확인: 선택(테라파고스 테라스탈 시 날씨·필드 제거 타이밍)
- 남은 위험: 낮음

## 동기화 단위: seq 45 #10042 `U-10042` Fix Syrup Bomb end-turn stat drop source for Defiant and Competitive

- 현재 판정: 적용
- 커밋: `7616748be6`
- upstream 근거: `d927117c80`
- 수정 파일: `data/battle_scripts_1.s`(`BattleScript_SyrupBombEndTurn` BS_ATTACKER → BS_TARGET), `src/battle_end_turn.c`(사탕폭탄 턴 종료 시 대상·공격자 설정, 문어굳히기에 생존 검사), `test/battle/ability/competitive.c`, `test/battle/ability/defiant.c`, `test/battle/move_effect/octolock.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. 턴 종료 속도 하락 문구가 `STRINGID_ATTACKERSSTATFELL` → `STRINGID_DEFENDERSSTATFELL`로 바뀌지만 HnS 한글 두 문구는 이름 토큰(`B_ATK_NAME_WITH_PREFIX`/`B_DEF_NAME_WITH_PREFIX`)만 다른 같은 형태이고 가리키는 포켓몬도 같아 화면 문구는 동일하다. HnS 오기·승기는 `STRINGID_DEFENDERSSTATFELL` 조건(`battle_util.c` 1271행)으로 발동하므로 이 수정으로 사탕폭탄 사용자 기준으로 올바르게 발동한다. 새 문자열 없음.
- 저장·ROM·그래픽 영향: ROM +80 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,708 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 테스트 모드 컴파일: `competitive.o`, `defiant.o`, `octolock.o` 성공
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). MESSAGE 의존 없음
  - 실기 확인: 선택(사탕폭탄 턴 종료 하락 + 오기/승기)
- 남은 위험: 없음

## 동기화 단위: seq 46 #10038 `U-10038` Fix Symbiosis triggering after Eject items consumed

- 현재 판정: 적용
- 커밋: `9989c9c959`
- upstream 근거: `979b1496a4`
- 수정 파일: `src/battle_util.c`(`TrySymbiosis`의 탈출버튼/탈출팩 검사를 `GetItemHoldEffect(itemId)`로), `test/battle/ability/symbiosis.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. 메시지 변화 없음. #10325(seq 이후)가 `TrySymbiosis` 시그니처를 바꾸므로 먼저 이식.
- 저장·ROM·그래픽 영향: ROM 변화 없음
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,708 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 테스트 모드 컴파일: `symbiosis.o` 성공
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). MESSAGE 의존(알려진 한계)
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 47 #10030 `U-10030` Fix Illusion reveal on fainting without attack damage

- 현재 판정: 적용
- 커밋: `eeb826879d`
- upstream 근거: `356ad4f4f3`
- 수정 파일: `src/battle_util.c`(`TryClearIllusion` 조기 반환을 `!IsBattlerTurnDamaged(battler, EXCLUDING_SUBSTITUTES)`로), `test/battle/ability/illusion.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. 호출처 3곳(`battle_switch_in.c`, `battle_script_commands.c`, `battle_move_resolution.c`)이 upstream과 같다. 일루전 해제 문구는 기존 것.
- 저장·ROM·그래픽 영향: ROM −32 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,676 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 테스트 모드 컴파일: `illusion.o` 성공
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). MESSAGE 의존(알려진 한계)
  - 실기 확인: 선택(조로아크 일루전: 공격 대미지 없이 기절할 때 정체 공개 여부)
- 남은 위험: 없음

## 동기화 단위: seq 48 #9974 `U-gen7anim-sprites` Updated Gen VII animations

- 현재 판정: 부분 적용(대부분 HnS에 이미 있음, 잔여분 적용)
- 커밋: `0ec96c2d77`
- upstream 근거: `db7df7a37f`. 바꾼 pal/back 19개 blob은 `expansion/1.17.0`과 같다.
- 해결한 의존성: 없음. 같은 unit의 #10141(seq 227)·#10208(seq 235)·#10270(seq 283)·#10414(seq 302)는 각 순서에서 이식.
- 파일별 대조 결과(HnS blob vs upstream 부모/적용 후):
  - 이미 적용: `anim_front.png` 14개(HnS `1821fd6749`), `src/data/graphics/pokemon.h`의 앞모습 경로(`anim_front`, #9881 전 INCBIN 표기), `gen_7_families.h` 16 hunk 중 12개(미니어 2개는 줄끝 공백만 다른 동등).
  - 미적용이던 것: `shared_front_pic_anims.h`(upstream 부모와 같은 blob), `gen_7_families.h`의 형사구스(+토템)·따라큐 탈(+토템) `frontAnimFrames` 자리표시자 4곳, 냐오불·시마사리·메테노·메테노 코어의 back/팔레트 19개, 참조 없는 옛 `front.png` 14개.
- 수정 파일: `src/data/pokemon/species_info/shared_front_pic_anims.h`(`sAnims_Gumshoos`, `sAnims_MimikyuDisguised` 추가), `src/data/pokemon/species_info/gen_7_families.h`(자리표시자 4곳 교체), `graphics/pokemon/{litten,mareanie,minior}/{back.png,normal.pal,shiny.pal}`, `graphics/pokemon/minior/core/{back.png,shiny.pal,<7색>/normal.pal}`, 옛 `front.png` 14개 삭제
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 새 `anim_front.png`와 옛 팔레트를 섞어 쓰던 색 버그(냐오불·시마사리 일부 픽셀 검정, 메테노·코어 색 뒤섞임, 뒷모습 재인덱싱 불일치)를 upstream 팔레트·뒷모습으로 복구했다. 해당 그래픽 파일에는 HnS 수정 이력이 없다. `.pal`은 `.gitattributes`(`eol=crlf`)에 맞춰 작업 트리도 CRLF로 두었다. 종 데이터의 한글 이름 등은 건드리지 않았다.
- 저장·ROM·그래픽 영향: ROM +256 B. 그래픽 변화(위 4종 색·뒷모습, 형사구스·따라큐 탈 앞모습 2프레임 애니).
- 검증:
  - 적용 후 19개 파일의 `git hash-object`가 upstream blob과 일치
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,932 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 해당 없음
  - 실기 확인: 필요. 냐오불·시마사리·메테노(유성/코어 7색)·메테노 코어 이로치의 앞/뒷모습 색, 형사구스·따라큐 앞모습 애니(요약·전투 등장).
- 남은 위험: 낮음(그래픽 실기 확인 전)

## 동기화 단위: seq 49 #10029 `U-10029` Fix end-turn form changes for fainted battlers

- 현재 판정: 적용
- 커밋: `68125dc902`
- upstream 근거: `75b648cfdf`
- 수정 파일: `src/battle_end_turn.c`(`HandleEndTurnFormChange` 앞머리 생존 검사), `test/battle/ability/power_construct.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. 메시지 변화 없음.
- 저장·ROM·그래픽 영향: ROM +32 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,964 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 테스트 모드 컴파일: `power_construct.o` 성공
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). MESSAGE 의존(알려진 한계)
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 50 #10016 `U-10016` Align wind move flags with current mechanics

- 현재 판정: 적용
- 커밋: `72fa9a02c2`
- upstream 근거: `5d8a69544c`
- 수정 파일: `src/data/moves_info.h`(`MOVE_SANDSTORM`의 `.windMove = TRUE` 삭제)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. **동작 변화:** 바람타기·풍력발전이 모래바람에 더는 발동하지 않는다(공식 동작). 메시지 경로 변화 없음.
- 저장·ROM·그래픽 영향: ROM 변화 없음(비트필드)
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,964 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 해당 테스트 없음
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 51 #10071 `U-10071` BoxPokemon: Less error-prone "static asserts"

- 현재 판정: 적용
- 커밋: `c4af4a17f9`
- upstream 근거: `7ded6d12ae`
- 수정 파일: `src/pokemon.c`(STATIC_ASSERT 묶음 → `UNUSED static const struct BoxPokemon sBoxPokemonConstantsFit` 초기화 + `MAX_LEVEL` assert)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. HnS 빌드는 `-Werror`라 비트필드 초과(`-Woverflow`)는 빌드 실패로 잡힌다. HnS 종·도구·기술·볼·언어 등 상수는 모두 들어맞았다(경고 0). 세이브 구조체 정의는 바꾸지 않는다.
- 저장·ROM·그래픽 영향: ROM 변화 없음(UNUSED 상수, 링크 시 제거)
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,964 B / EWRAM 249,112 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 해당 없음
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 52 #9207 `U-ram-9231` Remove unnecessary EWRAM allocation for cable car animation

- 현재 판정: 적용
- 커밋: `a927fae439`
- upstream 근거: `40d07454af`
- 해결한 의존성: 없음(같은 unit의 #9231은 seq 61)
- 수정 파일: `src/cable_car.c`(EWRAM 정적 변수 6개 → 지역 변수)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. 값은 매 호출마다 `sCableCar` 상태에서 다시 계산되므로 동작 불변.
- 저장·ROM·그래픽 영향: EWRAM −8 B(249,112 → 249,104), ROM −64 B. 세이브 무관.
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,736,900 B / EWRAM 249,104 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 해당 없음
  - 실기 확인: 권장(케이블카 오르내림 배경 지면 그리기)
- 남은 위험: 없음

## 동기화 단위: seq 53 #9360 `U-9360` AI considers Defense‑boosting moves before using Body Press

- 현재 판정: 적용
- 커밋: `b9086828f4`
- upstream 근거: `ed3078e0a9`
- 수정 파일: `src/battle_ai_util.c`(`IncreaseStatUpScoreInternal` STAT_DEF/STAT_SPDEF에 바디프레스 가산, 원더룸 반영), `test/battle/ai/ai_check_viability.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. HnS의 천진 2배 처리·앵콜 검사는 같은 함수의 다른 부분이라 건드리지 않았다. **AI 동작 변화:** 바디프레스를 가진 AI가 방어 랭크업을 더 선호(개선).
- 저장·ROM·그래픽 영향: ROM +192 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,737,092 B / EWRAM 249,104 B / IWRAM 25,636 B, 새 경고 없음
  - 테스트 모드 컴파일: `ai_check_viability.o` 성공
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). MESSAGE 의존 없음(점수·EXPECT_MOVE)
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 54 #9359 `U-9359` AI uses Beat Up on allies to power up Rage Fist in doubles

- 현재 판정: 부분 적용(게임 코드 이미 적용, 테스트만 이식)
- 커밋: `10e5c5c477`
- upstream 근거: `b2cfcfdcc4`
- 수정 파일: `test/battle/ai/ai_doubles.c`(Rage Fist Beat Up 테스트 2개)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 게임 코드(`GetUsableMoveIndexWithEffect`, `CanMoveIndexHitAnyOpponent`, `ShouldBeatUpForJustified`, `ShouldBeatUpForRageFist`, `AI_DoubleBattle` EFFECT_BEAT_UP 호출)는 HnS `src/battle_ai_util.c` 2634~2695행·`src/battle_ai_main.c` 3939행에 이미 있다(#10124가 다듬은 1.17.0 최종형, `ShouldBeatUpForRageFist`에 `battlerAtk` 인자 포함). 테스트 hunk만 그대로 적용.
- 저장·ROM·그래픽 영향: 게임 ROM 불변(테스트 전용이라 빌드 생략)
- 검증:
  - `git diff --check`: 통과
  - 테스트 모드 컴파일: `ai_doubles.o` 성공
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). EXPECT_MOVE만 사용, MESSAGE 의존 없음
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 55 #9358 `U-9358` AI avoids bad moves when a faster foe is predicted to go semi-invulnerable

- 현재 판정: 적용
- 커밋: `abfa669e98`
- upstream 근거: `3278f4b1d4`
- 수정 파일: `include/battle_util.h`, `src/battle_ai_main.c`, `src/battle_ai_util.c`, `src/battle_move_resolution.c`, `src/battle_util.c`, `test/battle/ai/check_bad_move.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. `BreaksThroughSemiInvulnerablity`를 `CanBreakThroughSemiInvulnerablity`/`BreaksThroughSemiInvulnerableState`로 나누고(HnS 호출처 4곳 모두 upstream과 같은 위치, 남은 옛 이름 0건) `AI_CheckBadMove`에 "빠른 상대가 공중날기류 예정" 감점을 추가. `CancelerTargetFailure`는 함수 이름만 바뀌고 `B_MSG_AVOIDED_ATK` 선택·메시지 경로 불변. upstream의 공백만 있는 줄 1개는 `git diff --check` 때문에 비웠다.
- 저장·ROM·그래픽 영향: ROM +2,256 B. 이전 커밋 소스로 오브젝트를 따로 컴파일해 비교한 결과 `AI_CheckBadMove`가 16,952 → 18,960 B(+2,008 B, 새 판정 블록과 `AI_IsSlower` 등 인라인 전개), `GetMoveTwoTurnAttackStatus` 116 B가 별도 심볼로 생겼다. 계획 추정(+0.2 KB)보다 크지만 ROM 여유(약 800 KB) 안이다.
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,739,348 B(97.57%) / EWRAM 249,104 B / IWRAM 25,636 B, 새 경고 없음
  - 테스트 모드 컴파일: `check_bad_move.o` 성공
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). MESSAGE 의존 없음
  - 실기 확인: 불필요
- 남은 위험: 없음. 후속 #9857(`predictedMoveSpeedCheck` → `predictedMove`)과 `GetMoveTwoTurnAttackStatus` 개명 PR이 이 블록을 다시 만진다.

## 동기화 단위: seq 56 #9349 `U-9349` Improve Rest sleep AI logic with cleanup and tests

- 현재 판정: 적용
- 커밋: `b2e083e797`
- upstream 근거: `b7ebb3af49`
- 수정 파일: `include/battle_ai_util.h`, `src/battle_ai_main.c`, `src/battle_ai_util.c`, `test/battle/ai/ai_check_viability.c`, `test/battle/ai/can_use_all_moves.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. `HasUsableWhileAsleepMove` → `HasMoveUsableWhileAsleep`(사용 불가 기술 제외, 호출처 4곳 + `IsBattlerIncapacitated`), 잠자기 점수 3단계 분리, 잠꼬대·코골기를 `AI_IsBattlerAsleepOrComatose`로. 옛 이름 사용처 0건. **AI 동작 변화:** 절대안깸도 잠꼬대 가산, 잠자기 점수 세분화(개선).
- 저장·ROM·그래픽 영향: ROM +32 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,739,380 B / EWRAM 249,104 B / IWRAM 25,636 B, 새 경고 없음
  - 테스트 모드 컴파일: `ai_check_viability.o`, `can_use_all_moves.o` 성공
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). MESSAGE 의존 없음
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 57 #9116 `U-9116` Adds bridge between battle engine and ai calcs

- 현재 판정: 적용(include 줄 수동 적응)
- 커밋: `ca2e53bdb2`
- upstream 근거: `9b3157ab1f`, 최종 include 형태는 `expansion/1.17.0` 대조
- 수정 파일: `include/battle_ai_record.h`·`src/battle_ai_record.c`(신규), `include/battle_ai_main.h`, `include/battle_ai_util.h`, `include/battle_util.h`, `src/battle_ai_main.c`, `src/battle_ai_util.c`, `src/battle_util.c`, `src/battle_main.c`, `src/battle_move_resolution.c`, `src/battle_script_commands.c`, `src/battle_message.c`, `src/battle_end_turn.c`, `src/battle_hold_effects.c`, `src/battle_z_move.c`, `src/battle_debug.c`, 배틀 컨트롤러 8개
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - `Record*`/`Clear*History` 8개를 `battle_ai_record.c/.h`로 이동(HnS 본문이 upstream과 같아 제거 hunk가 그대로 적용됨), `IsNaturalEnemy`를 `battle_util.c`로 이동, `BattleAI_SetupAIData`를 `BattleAI_SetupItems` 앞으로 이동(1.17.0 위치).
  - HnS가 `challenge_menu.h`를 끼워 둔 `battle_main.c`·`battle_move_resolution.c`는 include 줄만 수동 교체(`challenge_menu.h` 유지).
  - `battle_util.c`는 upstream처럼 `battle_ai_main.h`를 빼고 `battle_ai_record.h`를 넣되, #9942 이식으로 쓰는 `AllocSaveBattleMons` 때문에 `battle_ai_util.h`는 남겼다(1.17.0 최종형과 같은 include 조합).
  - `battle_message.c`는 include 1줄만 바뀌고 문자열·메시지 로직 불변.
  - 같은 PR에 들어 있는 탈출팩 조건 1줄(`CountUsablePartyMons(battlerDef) > 0` → `CanBattlerSwitch(battlerDef)`)도 이식했다. 교체 불가(구속 등) 상태면 탈출팩이 발동하지 않는다. 1.17.0에서는 #9784 `TryEjectPack`이 다시 쓴다.
- 저장·ROM·그래픽 영향: ROM 변화 없음(32,739,380 B 그대로). 새 오브젝트 `battle_ai_record.o`가 링크됨을 map 파일로 확인.
- 검증:
  - 옮긴 함수 사용처(`src`·`test`) 모두 `battle_ai_record.h` 또는 `battle.h`→`battle_util.h` 경로로 선언을 받는다(-Werror로 암시적 선언 없음 확인).
  - `git diff --check`: 통과(새 파일도 후행 공백 없음)
  - `make hns -j6`: 종료 코드 0, ROM 32,739,380 B / EWRAM 249,104 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 해당 테스트 파일 없음
  - 실기 확인: 선택(탈출팩: 교체할 포켓몬은 있으나 도망칠 수 없는 상태에서 미발동)
- 남은 위험: 낮음

## 동기화 단위: seq 58 #9063 `U-9063` Made moves' animation's AbsorbEffects more consistent

- 현재 판정: 적용
- 커밋: `da2c827f0e`
- upstream 근거: `22e55ce62b`
- 수정 파일: `data/battle_anim_scripts.s`(드레인펀치 → GigaDrain 흡수, 드레인키스 → MegaDrain 흡수, 흡혈 → `B_UPDATED_MOVE_DATA >= GEN_7`이면 GigaDrain 흡수)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. #8497/#9172 애니 체인보다 먼저. monbg/clearmonbg 변화 없음(#9855 assert와 무관).
- 저장·ROM·그래픽 영향: ROM 크기 변화 없음(호출 대상 포인터만 바뀜). 애니메이션 연출 변화.
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,739,380 B / EWRAM 249,104 B / IWRAM 25,636 B, 새 경고 없음
  - 자동 테스트: 해당 없음
  - 실기 확인: 선택(드레인펀치·드레인키스·흡혈 애니)
- 남은 위험: 없음

## 동기화 단위: seq 59 #9176 `U-fling-9176` Fling Refactor

- 현재 판정: 적용(HnS 적응)
- 커밋: `2e93d86ed1`
- upstream 근거: `563f5fef7c`, 1.17.0 `HandleSetEffectFling`(판정 순서 대조)
- 해결한 의존성: 없음. 배틀 리팩터 체인의 첫 기반(후행 #9446·#9494·#9610·#9674·#9859·#9730 등).
- 수정 파일: `asm/macros/battle_script.inc`(`tryflingholdeffect` 삭제), `data/battle_scripts_1.s`, `include/battle.h`(`flingItem:14`·`flungItem:2`), `include/battle_scripts.h`, `include/battle_util.h`, `include/constants/battle.h`(`MOVE_EFFECT_FLING`), `include/constants/battle_script_commands.h`(`enum FlungItem`), `src/battle_ai_main.c`, `src/battle_move_resolution.c`, `src/battle_script_commands.c`, `src/battle_util.c`, `src/data/moves_info.h`(내던지기 `additionalEffects`), `test/battle/move_effect/fling.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - `BattleScript_EffectFling`을 공용 `BattleScript_HitFromDamageCalc`로 보내고, 던진 도구 효과를 `SetMoveEffect`의 `MOVE_EFFECT_FLING` 추가 효과로 옮겼다. `BS_TryFlingHoldEffect`는 삭제.
  - 이미 이식한 #9951(seq 21)을 살리려고 새 `MOVE_EFFECT_FLING` 분기에서 인분·비밀망토 검사를 나무열매 검사보다 **먼저** 두었다(1.17.0 `HandleSetEffectFling`과 같은 순서). upstream #9176 당시 코드는 열매 검사가 먼저였다.
  - `TrySymbiosis`: 이미 이식한 #10038의 `GetItemHoldEffect(itemId)` 형태를 유지하고 `EFFECT_FLING` 제외 줄만 삭제(1.17.0 최종형과 같음).
  - `include/battle_scripts.h`는 #9929의 `BattleScript_TargetAvoidsAttackEnd` 줄 때문에 문맥이 달라 `BattleScript_RemoveItem` 선언을 수동으로 넣었다.
  - `battle_script_commands.c` 공백 전용 hunk 3개는 HnS에 이미 반영돼 있었다.
  - 한글 `STRINGID_PKMNFLUNG`·`STRINGID_ITEMWASUSEDUP`, HnS 아이템 팝업이 붙은 `BattleScript_WhiteHerbFling` 경로는 그대로다. 문구 ID 변화 없이 스크립트 흐름만 바뀐다(도구 제거 시점이 공격 애니 전 → 효과 처리 뒤).
- 저장·ROM·그래픽 영향: ROM +80 B. `gBattleStruct`(heap) 안에서 `flingItem`이 비트필드로 옮겨짐. 세이브 무관.
- 검증:
  - HnS에서 옛 `tryflingholdeffect`/`BS_TryFlingHoldEffect`/`CanFling(1인자)` 사용처가 남지 않았음을 확인
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0(헤더 변경으로 전체 재빌드), ROM 32,739,460 B / EWRAM 249,104 B / IWRAM 25,636 B, 경고 164건(기준과 같음, 새 경고 없음)
  - 테스트 모드 컴파일: `fling.o` 성공
  - 자동 테스트: 미실행(테스트 힙 문제 조사 중). `fling.c` 새 줄 MESSAGE 의존 없음
  - 실기 확인: **필요.** 내던지기 기본(대미지·"던졌다" 문구·도구 소모), 불꽃구슬·맹독구슬·전기구슬·독바늘·왕의징표석·하양허브·멘탈허브 던지기, 나무열매 던지기(상대가 먹음), 인분/비밀망토 대상(열매 포함) "다 써버렸다" 문구, 빗나감·실패 시 도구 소모, 공생.
- 남은 위험: 중간. 배틀 스크립트 흐름 변경이라 자동 테스트 복구 후 `fling.c` 전체 실행이 필요하다.

## 동기화 단위: seq 60 #8816 `U-8816` Cleaned up Match Call

- 현재 판정: 적용(HnS 블록 구조만 맞춤)
- 커밋: `89f3493d0e`
- upstream 근거: `5f36a55bf4`(비기능 정리)
- 수정 파일: `src/pokenav_match_call_data.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - HnS 파일에는 `#if IS_HNS`(HnS 매치콜: 바오바 등, 조토 체육관 플래그) 블록과 `#else // Emerald` 블록이 둘 다 있어 `git apply`가 되지 않는다. 텍스트·위치 표(`match_call_text_data_t`, `MatchCallLocationOverride`)를 헤더 안 compound literal로 옮기는 변환을 스크립트로 두 블록에 적용했다.
  - 스크립트 검증: upstream 부모 파일에 돌린 결과가 upstream #8816 결과와 바이트 동일. HnS 파일의 Emerald 블록 변환 결과도 upstream #8816과 같다(후행 공백 무시).
  - HnS 블록 내용·플래그 보존: 옛 파일의 표 이름을 본문으로 풀어 비교한 결과 헤더 43개(두 블록 합계)가 모두 같다. upstream이 남긴 미사용 `sMomTextScripts` 표도 같은 모양으로 남겼다(컴파일러가 제거). Wally 위치 표의 `0xFFFF`는 upstream처럼 같은 값인 `ALWAYS_AVAILABLE`로 표기.
  - Pokegear/Pokenav 한글 문자열·그래픽 무관.
- 저장·ROM·그래픽 영향: ROM 크기 동일(32,739,460 B). 바이트 차이 872 B는 모두 이 오브젝트의 rodata 구간(0x08D803C8~0x08D80AA7) 안의 표 배치·포인터 변화다.
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,739,460 B / EWRAM 249,104 B / IWRAM 25,636 B, 새 경고 없음(기존 `MatchCall_GetMapSec_Birch` 미사용 경고만)
  - 자동 테스트: 해당 없음
  - 실기 확인: 권장(포켓기어 전화부 목록·매치콜 대화, 월리 위치 표시)
- 남은 위험: 낮음

## 동기화 단위: seq 61 #9231 `U-ram-9231` Random EWRAM and IWRAM savings

- 현재 판정: 적용(HnS 적응 1곳)
- 커밋: `3b8f30411d`
- upstream 근거: `65e2f409d2`
- 해결한 의존성: 같은 unit의 #9207(seq 52)
- 수정 파일: `src/link_rfu_2.c`, `src/save.c`, `src/tv.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - `save.c`(미사용 `gSaveUnusedVar`·`gSaveUnusedVar2` 삭제)와 `tv.c`의 비밀기지 방문·레코드 믹싱 임시 변수 지역화는 그대로 적용.
  - `link_rfu_2.c`: HnS가 `gRfuAPIBuffer`/`gRfu`를 `EWRAM_DATA`로 둔 문맥 차이만 있어 문맥을 줄여 적용(`struct RfuDebug` 축소, `unkFlag` 대입 제거).
  - **upstream과 다르게 둔 곳:** `sTVSecretBaseSecretsRandomValues[3]`는 static EWRAM으로 유지했다. `DoTVShowSecretBaseSecrets`는 호출마다 상태 하나만 처리하고 다음 호출에서 앞서 뽑은 값([0]·[1])으로 중복을 피한다. upstream #9231은 이를 함수 지역 배열(`= {}`)로 바꿔 다음 상태에서는 항상 0을 읽게 되며, 1.17.0에도 그대로 남아 있다. 현재 동작 보존을 위해 이 3바이트만 지역화하지 않았다.
  - 세이브 레이아웃 무관(COMMON/EWRAM 변수만).
- 저장·ROM·그래픽 영향: EWRAM −212 B(249,104 → 248,892), IWRAM −120 B(25,636 → 25,516), ROM −144 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0, ROM 32,739,316 B / EWRAM 248,892 B(94.94%) / IWRAM 25,516 B(77.87%), 새 경고 없음
  - 자동 테스트: 해당 없음
  - 실기 확인: 선택(TV 비밀기지 방문·비밀기지 비밀 방송, 레코드 믹싱 — HnS에서 쓰는 경우만)
- 남은 위험: 낮음. `tv.c` 해당 함수는 upstream과 달라 이후 upstream hunk가 이 줄을 만지면 수동 대조가 필요하다.

## 동기화 단위: seq 62 #9173 `U-9173` Separate Perfect IV Logic Into New Function

- 현재 판정: 적용(문서 hunk는 이미 있음)
- 커밋: `670f578e53`
- upstream 근거: `35a2ee34bb`
- 수정 파일: `include/pokemon.h`, `src/pokemon.c`, `src/dexnav.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - `SetBoxMonIVs` 끝의 완벽 개체값 블록을 `SetBoxMonPerfectIVs()`로 분리. HnS 챌린지 `MaxPartyIVs`(1: 전부 31, 2: 30~31) 조기 반환 분기는 앞에 그대로 둔다.
  - `CreateDexNavWildMon`의 `iv[3]` 루프를 `SetBoxMonPerfectIVs(&mon->box, min(3, potential))`로 교체. HnS 사파리 포켓블록 숨겨진 특성 보존 분기는 그대로.
  - `docs/tutorials/mon_generation.md` 문장은 HnS 문서(#9531 반영판)에 이미 있다.
  - **동작 차이:** DexNav 완벽 개체값 선택이 do-while 중복 제거 → 목록에서 빼는 방식이라 `Random()` 호출 수가 달라진다(분포는 같은 균등 무작위). HnS `DEXNAV_ENABLED`는 FALSE.
- 저장·ROM·그래픽 영향: ROM −96 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j6`: 종료 코드 0(헤더 변경으로 전체 재빌드), ROM 32,739,220 B / EWRAM 248,892 B / IWRAM 25,516 B, 경고 164건(기준과 같음, 새 경고 없음)
  - 자동 테스트: 해당 테스트 없음
  - 실기 확인: 선택(전설 포켓몬 등 `perfectIVCount` 종의 개체값, 챌린지 MaxPartyIVs 설정)
- 남은 위험: 없음

## seq 2~62 요약

- 처리 범위: seq 2~62(61개 PR) + 같은 unit 회귀 수정 #10647(순서표 seq 329) 1개. 커밋 62개, 시작 `733543267f` → 끝 `670f578e53`.
- 판정(61개): 적용 57(그중 HnS 적응이 큰 것: #9905, #9925, #7360, #9942(+#10647), #10040, #9116, #9176, #8816, #9231), 부분 적용 4(#9882 스크립트 hunk 기존 적용, #9949 mapjson 의도만, #9974 잔여분만, #9359 테스트만), 이미 적용·skip 0. 중단 없음.
- 마지막 빌드(`670f578e53`, 헤더 변경으로 사실상 전체 재빌드): 종료 코드 0, **ROM 32,739,220 B(97.57%) / EWRAM 248,892 B(94.94%) / IWRAM 25,516 B(77.87%)**, 경고 164건(기준과 같은 목록, 새 경고 0).
- 기준 대비: ROM +3,952 B, EWRAM −220 B, IWRAM −120 B.
- 자동 테스트: 전부 미실행(테스트 힙 문제 조사 중). 테스트 파일을 바꾼 PR은 해당 오브젝트만 테스트 모드로 컴파일해 성공을 확인했다.

### upstream과 일부러 다르게 둔 곳 (이후 port 담당 참고)

| seq | PR | 내용 |
|---|---|---|
| 16 | #9925 | `PAL_RAW_REFLECTION_OFFSET` 0x3000 → **0x3100**. HnS 서핑 포켓몬 팔레트 태그(0x3001~0x308B)와 충돌 회피 |
| 24 | #7360 | 애니 재생 판정은 옛 "최선 결과" 함수(`GetBestResultFlagsForDoubleSpreadMoveAnimation`)를 #10362까지 유지. 효과음 판정만 1.17.0 최종형 |
| 25 | #9942 | 같은 unit의 #10647(seq 329)을 바로 뒤 커밋 `7afa522502`로 적용. **seq 329 담당은 "이미 적용"으로 처리** |
| 42 | #9949 | HnS mapjson(`game_version` 게이팅)에 맞춰 의도만 이식. map.json `region` 470줄·migration 스크립트 제외 |
| 59 | #9176 | 인분·비밀망토 검사를 나무열매보다 먼저(#9951 유지, 1.17.0 순서) |
| 61 | #9231 | `sTVSecretBaseSecretsRandomValues`는 static 유지(upstream 지역화는 상태 간 값이 사라지는 회귀, 1.17.0에도 남음) |

### 출력 문구 변화 (기록 완료)

- #9929: 명중 판정 변화기 빗나감 "그러나 실패하고 말았다!" → "…에게는 맞지 않았다!". `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 1행 추가(group plan 지시).
- #9921·#9951·#10042: 기존 ID 재사용 또는 같은 형태 문구라 기록 대상 아님(항목별 설명 참고).

### 실기 확인 필요 (mGBA)

1. #9855: 전기 계열 애니(Charge·Flash Cannon·Steel Beam·Volt Tackle·Fairy Lock·Collision Course·Shock Wave)와 HnS 추가 기술 애니 재생 중 assert 크래시 화면이 없는지
2. #9883: Trainer Hill 안·일반 맵에서 동반 포켓몬 말 걸기
3. #9905: 포켓기어 매치콜 이름 상자 표시, 통화 종료 뒤 표지판에 이름 상자가 남지 않는지, 연속 대사 이름 상자 깜빡임
4. #9882·#9912·#9973: 비-Gen4 UI(챌린지 `newBattleUI` 끔) 사파리 배틀 남은 볼 수 갱신, 싱글/더블·Gen4 UI 켬/끔 체력바·HP 숫자
5. #9925: 물가 반사(동반 포켓몬·NPC·플레이어·이로치), HnS 서핑 탈것으로 서핑 중 높은 다리 위 NPC 반사 색
6. #9929: 명중 판정 변화기가 빗나갈 때 "맞지 않았다" 문구
7. #9942/#10647: 미래예지 시전자를 교체한 뒤 공격, 악 타입(상성 0) 대상일 때 이후 배틀러 상태가 흐트러지지 않는지
8. #9974: 냐오불·시마사리·메테노(유성/코어 7색)·메테노 코어 이로치 앞/뒷모습 색, 형사구스·따라큐 앞모습 애니
9. #9176: 내던지기 전반(대미지·문구·도구 소모, 구슬·허브·열매 던지기, 인분/비밀망토 대상, 빗나감, 공생)
10. #8816: 포켓기어 전화부 목록·매치콜 대화·월리 위치 표시
11. #9207: 케이블카 오르내림 배경
12. 선택: #9858 내용물쏟기, #9880 재앙 특성+위액, #10030 일루전, #10040 테라폼제로, #10042 사탕폭탄+오기/승기, #9116 탈출팩, #9063 흡수 애니, #9231 TV 비밀기지 방송

### 남은 위험

- 배틀 스크립트 흐름 변경(#9176 Fling, #9942 미래예지)은 자동 테스트 복구 뒤 `fling.c`·`future_sight.c`를 반드시 실행해야 한다.
- #9358로 `AI_CheckBadMove`가 약 2 KB 커졌다(인라인 전개). ROM 여유 안이지만 누적 증가를 계속 기록한다.
- #9905의 speaker NULL 경로는 후속 #10540 이식 전까지 upstream 중간 상태와 같다.
