# full-sync 실제 port 결과 — seq 84~90

진행 중: 마지막 완료 seq 85, 다음 seq 86

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), [`CLAUDE_FULL_SYNC_PORT_PROMPT.md`](../../CLAUDE_FULL_SYNC_PORT_PROMPT.md)
시작 HEAD: `79af946ddf`

## seq 84~90 공통 사항

- 빌드 명령: `PATH=/opt/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin:$PATH GITHUB_ACTION=1 make hns -j8 > build/port.log 2>&1` (노트북 WSL, 8코어).
- 기준 빌드(`79af946ddf`, `rm -rf build/hns` 뒤 전체 재빌드, 약 40초): 종료 코드 0, ROM 32,712,548 B(97.49%), EWRAM 248,924 B(94.96%), IWRAM 25,516 B(77.87%). 경고 줄 166개, "파일: 메시지"(줄 번호 제거) 고유 목록 44개. ROM SHA-1 `09927f92473a555ef521349aee72e8367125452d`(직전 구간 최종 ROM과 같음).
- 경고 비교: 매 빌드의 경고를 같은 형식으로 만들어 기준 목록과 비교하고 새 경고만 확인했다.
- 테스트 기준: [`test-baseline-seq083.txt`](test-baseline-seq083.txt)(PASS 2,298 / FAIL 2,229 / TOTAL 5,197). PR마다 관련 테스트 파일을 돌려 기준 목록과 테스트 이름별로 비교했다. 알려진 예외: AI 더블 테스트 3건(`AI_REVERSE_BATTLER_LOGIC_ORDER_CHANCE 50`), `Move Animations work 1`·`2`(HnS 도구 팝업 태스크).
- 한글 포함 소스 줄: 커밋마다 `git show <커밋> | grep -aP '^[-+](?![-+]).*[^\x00-\x7F]'`로 확인했다.
- 코드 비교 도구(스크래치 `p84/`, 재생성 가능): `objcmp.py`는 기준 빌드 오브젝트(`obj-base/`)와 새 오브젝트를 `objdump -d -r`로 함수별 명령열(주소 제거, 재배치 기호 포함)로 바꿔 비교한다. `fndiff.sh`는 함수 하나의 명령 차이를 보여 준다. 비기능 PR은 이 방법으로 "바뀐 함수 목록 = 설명 가능한 것뿐"인지 확인했다(ROM 전체는 함수 크기 변화로 주소가 밀려 바이트 비교가 의미 없다).

## 동기화 단위: seq 84 #9376 `U-cleanup-9376` Clean up boolean comparisons

- 현재 판정: 적용(hunk 단위 수동 적용)
- 커밋: `6e050d6e70`
- upstream 근거: `f2c4aa4b99`
- 해결한 의존성: 없음. 같은 unit의 #9466(seq 85)은 바로 다음 행이라 순서대로 따로 커밋한다.
- 수정 파일(22): `include/librfu.h`, `src/battle_controller_player.c`, `src/battle_main.c`, `src/battle_script_commands.c`, `src/battle_tower.c`, `src/battle_util.c`, `src/dodrio_berry_picking.c`, `src/follower_npc.c`, `src/librfu_intr.c`, `src/librfu_stwi.c`, `src/link.c`, `src/link_rfu_2.c`, `src/menu_helpers.c`, `src/mystery_event_menu.c`, `src/mystery_gift_menu.c`, `src/party_menu.c`, `src/pokemon.c`, `src/sound.c`, `src/text.c`, `src/trainer_card.c`, `src/union_room.c`, `test/battle/hold_effect/berserk_gene.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - 18파일은 `git apply`로 그대로 들어갔다(파일 모드 100755 유지). 문맥이 달라 실패한 4파일은 같은 표현이 남아 있는 줄만 손으로 바꿨다.
    - `battle_controller_player.c` `SetLinkBattleEndCallbacks`: HnS GBS 인자(`m4aSongNumStop(SE_LOW_HEALTH, FlagGet(FLAG_SYS_GBS_ENABLED))`)는 그대로, `gReceivedRemoteLinkPlayers == 0` → `!…`만.
    - `battle_util.c` `TryRunFromBattle`: HnS 도전 설정 분기(`tx_Challenges_LessEscapes`, `Random() & 512`)는 그대로 두고, 함수 안의 `effect++` 11개(그중 HnS 분기 1개 포함)를 `effect = TRUE`, `if (effect != 0)` → `if (effect)`. 분기가 서로 배타적이라 `effect`는 원래 0/1이다. 도주 메시지 경로(`HandleAction_Run` 이후)는 건드리지 않았다.
    - `sound.c` `PlaySE`: HnS GBS 분기(`ClearPlayerForGBSSoundEffect`)는 그대로, `== 0` → `== MUSIC_DISABLE_OFF`와 `#include "overworld.h"`(값 0, `include/overworld.h` enum). HnS의 로컬 `extern u8 gDisableMapMusicChangeOnMapLoad;`는 upstream처럼 남겼다(같은 형 중복 선언).
    - `text.c`: `DrawDownArrow` `drawArrow == 0` → `!drawArrow`, `DecompressGlyph_Small` `isJapanese == 1` → `isJapanese`. HnS 한글 글리프 경로(`glyphId >= 0x3700` → `gFont0KoreanGlyphs`)는 그 앞에 그대로 있다.
  - `!= 1`/`== 1` → 진릿값 치환의 전제 확인: `gReceivedRemoteLinkPlayers`(`bool8`)에 쓰는 곳 10곳이 모두 0/1/`TRUE`/`FALSE`. `gSTWIStatus->sending`(`vu8` → `vbool8`, `typedef vu8 vbool8`) 쓰기 8곳 모두 0/1. `TextPrinter.japanese`는 `u8 :1` 비트필드. `IsOverworldLinkActive()`는 `TRUE`/`FALSE`만 반환.
- 저장·ROM·그래픽 영향: ROM 크기 불변(32,712,548 B). 세이브 무관.
- **비기능 확인(오브젝트 코드 비교):** 바뀐 C 파일 21개의 오브젝트 함수 2,074개 중 2,063개가 명령열까지 같다. 다른 11개는 모두 `cmp rX, #1; beq` → `cmp rX, #0; bne` 형태의 비교 상수·분기 방향 변화와 그에 따른 블록 배치 변화뿐이다.
  - `battle_main.o` `CB2_AskRecordBattle`·`CB2_EndLinkBattle`(`AskRecordBattle`·`EndLinkBattleInSteps` 인라인, `gReceivedRemoteLinkPlayers != 1`), `librfu_stwi.o` `STWI_init`·`STWI_poll_CommandEnd`(`sending == 1`), `link_rfu_2.o` `LinkManagerCB_Child`·`_Parent`·`_UnionRoom`, `menu_helpers.o` `MenuHelpers_IsLinkActive`·`MenuHelpers_ShouldWaitForLinkRecv`(인라인), `trainer_card.o` `CreateTrainerCardTrainerPic`(`== 1`), `text.o` `RenderText`(`DecompressGlyph_Small` 인라인, `isJapanese == 1`, 블록 순서만 바뀜).
  - 위 전제(값이 0/1뿐)로 모두 동작이 같다. `battle_util.o`(`TryRunFromBattle` 포함)·`battle_script_commands.o`·`party_menu.o`·`pokemon.o`·`sound.o` 등 나머지 파일은 함수 전부 명령열 동일.
- 검증:
  - `git diff --check`: 통과. 비ASCII 줄 변경 0.
  - `make hns -j8`: 종료 코드 0, ROM 32,712,548 B / EWRAM 248,924 B / IWRAM 25,516 B, 새 경고 없음.
  - 자동 테스트: `test/battle/hold_effect/berserk_gene.c` 13건 — PASS 4 / FAIL 9, 기준 목록과 테스트별로 같음(FAIL은 영문 `MESSAGE`).
  - 실기 확인: 불필요(동작 동등). 선택: 통신(유니온룸·무선) 기능은 원래 실기 확인 범위 밖.
- 남은 위험: 없음.

## 동기화 단위: seq 85 #9466 `U-cleanup-9376` Fix enum usage

- 현재 판정: 적용(hunk 단위 수동 적용)
- 커밋: `a4c46a4a8f`
- upstream 근거: `56ee6f0f19`
- 해결한 의존성: #9376(seq 84) 뒤. 같은 unit 두 행을 순서대로 따로 커밋했다.
- 수정 파일(55): 게임 16개(`src/battle_ai_main.c`, `battle_ai_util.c`, `battle_dome.c`, `battle_hold_effects.c`, `battle_main.c`, `battle_move_resolution.c`, `battle_script_commands.c`, `battle_util.c`, `daycare.c`, `evolution_scene.c`, `item.c`, `item_use.c`, `mail_data.c`, `party_menu.c`, `pokemon.c`, `trainer_pools.c`), 테스트 39개(`test/battle/**` 38개 + `test/text.c`, 모두 지역 변수 형·`0` → `*_NONE` 표기)
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - 49파일은 `git apply`로 그대로, 문맥이 달라 실패한 6파일은 같은 표현이 남은 줄만 손으로 바꿨다.
    - `battle_ai_main.c`: HnS 함수는 `struct ChosenAction ChooseMoveOrAction_Singles`(구조 다름)지만 `gAiLogicData->partnerMove = 0;` 줄이 같아 `MOVE_NONE`으로.
    - `battle_script_commands.c`: `SetMoveEffect` 소각·벌레먹음 `item = 0` → `ITEM_NONE`, 내던지기 `u32 item` → `enum Item`, 맥스 기술 측면 능력치 하락 `enum Stat statId = 0` → 초기값 없음(HnS switch 3갈래 모두 대입), `BS_JumpIfAbilityCantBeReactivated`·`BS_TryActivateSoulheart`·`BS_PlayMoveAnimation`·`BS_TryPsychoShift`·`BS_JumpIfAbilityPreventsRest`·`BS_CutOneThirdHpAndRaiseStats`. `BS_SwitchinAbilities`·`BS_TryActivateReceiver`는 HnS가 이미 `enum Ability`(나중 upstream 형태)라 해당 없음.
    - `battle_util.c`: `PrepareStringBattle` 2줄, `GetHighestStatId`·`GetParadoxHighestStatId` 루프 변수, `CalcDefenseStat` 쿼크차지. 파일 끝 빈 줄 삭제 hunk는 HnS에 빈 줄이 이미 없어 해당 없음.
    - `daycare.c`: HnS `InheritIVs`·`GiveMoveIfItem` 계열은 이미 `enum Item`(나중 upstream 알 재작업 형태)이라 `AlterEggSpeciesWithIncenseItem` 한 줄만.
    - `pokemon.c`: `DoesMonMeetAdditionalConditions`·`GetEvolutionTargetSpecies` 5곳(`partnerSpecies, partnerHeldItem` 두 줄로 나눔).
    - `test/battle/ability/infiltrator.c`: HnS 테스트 구성이 upstream 부모와 달라(아군 흰안개·신비의부적 테스트가 `ability` 변수 없음) `u32 ability` 5곳만 1.17.0과 같은 표기로.
  - group plan의 "HnS 전용 배틀 함수에도 같은 규칙" 문구는 upstream hunk가 닿는 함수 안에서만 적용했다. 저장소 전체 일괄 치환은 하지 않았다(범위 밖 변경 방지).
  - `evolution_scene.c` `CreateShedinja`: upstream은 `MON_DATA_POKEBALL`에 도구 번호 대신 `GetItemSecondaryId(ball)`를 넣는다. HnS에서 `ITEM_POKE_BALL = 1`, 그 `secondaryId = BALL_POKE = 1`이라 저장되는 값이 같다(`P_SHEDINJA_BALL = GEN_LATEST`).
- 저장·ROM·그래픽 영향: ROM +80 B(32,712,628 B). 세이브 무관(저장값 동일).
- **비기능 확인(오브젝트 코드 비교, #9376 직후 오브젝트 기준):** 바뀐 C 파일 16개의 함수 2,117개 중 2,079개 동일, 38개 다름. 모두 설명된다.
  - HnS의 `enum Item`·`Ability`·`Move`·`Stat`은 `__attribute__((packed))`라 `u32` → enum 변경이 16/8비트 절단·확장 명령을 넣거나 뺀다. 값 범위(도구·특성 < 65,536, 능력치 번호 < 8) 안이라 결과가 같다: `pokemon.o` `DoesMonMeetAdditionalConditions`, `battle_util.o` `GetHighestStatId`·`GetParadoxHighestStatId`·`GetParadoxBoostedStatId`(인라인)·`DoMoveDamageCalcVars`·`AbilityBattleEffects`(인라인 블록 배치), `battle_script_commands.o` `SetMoveEffect`·`BS_JumpIfAbilityPreventsRest`·`ChangeStatBuffs`(명령열은 정규화 후 동일, 분기 주소만).
  - `pokemon.o`의 나머지 26개(`GetSpeciesBaseHP` 등): `pokemon.c` 한 줄이 두 줄로 나뉘어 뒤쪽 assert의 `__LINE__` 상수가 1씩 커진 것뿐(`0x24b5` → `0x24b6` 등).
  - `battle_util.o` `CanFling`: 컴파일러 생성 switch 표 이름(`CSWTCH.1346` → `.1348`)만.
  - `evolution_scene.o` `Task_EvolutionScene`: 위 `GetItemSecondaryId(ITEM_POKE_BALL)` 호출 추가(결과 1).
- 검증:
  - `git diff --check`: 통과. 비ASCII 줄 변경 0.
  - `make hns -j8`: 종료 코드 0, ROM 32,712,628 B / EWRAM 248,924 B / IWRAM 25,516 B, 새 경고 없음.
  - 자동 테스트: L 크기라 전체 실행(`make check BUILD=hns -j8`, 9분 2초). PASS 2,298 / FAIL 2,229 / KNOWN_FAILING 8 / TO_DO 618 / EXPECT_FAILING 6 / TOTAL 5,197. 목록(5,128행)이 `test-baseline-seq083.txt`와 **바이트 동일**.
  - 실기 확인: 불필요(동작 동등).
- 남은 위험: 없음.
