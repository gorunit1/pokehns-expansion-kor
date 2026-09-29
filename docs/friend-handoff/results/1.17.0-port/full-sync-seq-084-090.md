# full-sync 실제 port 결과 — seq 84~90

완료: seq 84~90 이식·전체 테스트·기록 완료(다음 구간은 seq 91 #9507부터). 아래 "seq 84~90 요약" 참고.

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

## 동기화 단위: seq 86 #9510 `U-9510` Minor IsBattlerWeatherAffected refactor

- 현재 판정: 적용(HnS 적응)
- 커밋: `7a6cd5b51b`
- upstream 근거: `60eeba987f`
- 해결한 의존성: 없음. #9735(seq 146, 메가솔)의 본체는 HnS에 먼저 들어와 있다(`GetAttackerWeather`).
- 수정 파일: `include/battle_util.h`, `src/battle_util.c`, `src/battle_script_commands.c`, `src/battle_move_resolution.c`, `src/battle_main.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - **`GetWeather()` 공개:** `battle_script_commands.c`의 static `GetWeather`를 지우고 `battle_util.c`에 공개 함수로 둔다. HnS에는 본문이 같은 `GetBattleWeatherForEffects()`(HnS 이름, #9735 선반영 때 `GetWeather`가 static이라 따로 만든 것)가 있어, 이것을 `GetWeather`로 바꾸고 호출 4곳(`DoesMoveMissTarget` 2, `CanTwoTurnMoveFireThisTurn`, `GetDynamicMoveType`)을 맞췄다. 1.17.0도 같은 자리에서 `GetWeather()`를 쓴다.
  - **새 시그니처 `IsBattlerWeatherAffected(holdEffect, weather, flags)`:** 본문은 #9510 그대로. HnS 이전 판은 `GetAttackerWeather(보유 도구, 특성, 날씨) & flags`라 메가솔 보유자를 "햇살"로 봤다. 새 함수에는 특성 인자가 없어서 호출부마다 메가솔 영향 여부를 확인했다.
    - 특성이 정해진 경로(수확·아이스바디·건조피부·젖은접시·촉촉한몸·선파워·리프가드·기상예보/플라워기프트/아이스페이스, 플라워기프트 공격/방어·아군): 대상의 특성이 그 특성이라 메가솔일 수 없다 → upstream 형태 그대로. 날씨·우산 판정은 옛 식과 같다(날씨 ≠ 없음이면 `weather == gBattleWeather`).
    - **메가솔이 걸릴 수 있는 3곳은 HnS 동작 유지:** `CanSetNonVolatileStatus` 얼음 상태 면역(맑음), `CalcDefenseStat` 모래바람 바위 특방·설경 얼음 방어 보정. `GetAttackerWeather(…) & flag` 형태로 옛 판정을 그대로 두고 `// HnS:` 주석을 달았다(방어 보정은 #9510 의도대로 `ctx->holdEffectDef/abilityDef/weather` 사용, 실전 계산에서는 `CalculateMoveDamage`가 채운 값이라 옛 값과 같다). upstream #9510을 그대로 쓰면 메가솔 방어측이 얼음 상태에 걸리게 되는 등 동작이 바뀐다. 이 두 보정은 seq 146 #9735에서 공격측 `GetAttackerWeather` 기준으로 바뀔 예정이다(group plan).
    - 대미지 계산의 선파워·플라워기프트(자신)·아군 플라워기프트(방어)는 #9510대로 `ctx->holdEffectAtk`·`ctx->weather`를 쓴다. 실전 계산은 같은 값이고, AI 계산은 AI가 아는 도구·예상 날씨를 쓰게 된다(upstream 의도).
    - 기상예보·플라워기프트·아이스페이스 폼 체인지 조건의 `gBattleWeather == NONE || !HasWeatherEffect()`는 upstream처럼 `weather == B_WEATHER_NONE`(`weather = GetWeather()`)으로 합쳤다(같은 값).
  - upstream hunk 중 HnS가 이미 나중 형태(`GetAttackerWeather`)를 쓰는 곳은 해당 없음: 2턴 기술 `CanTwoTurnMoveFireThisTurn`, `BS_JumpIfWeatherAffected`, 명중 `CanMoveSkipAccuracyCalc`·`GetTotalAccuracy`, 솔라빔 위력.
  - 기존 HnS `GetAttackerWeather(holdEffect, ability, weather)`는 인자 순서가 이미 1.17.0과 같아 바꾸지 않았다.
- 저장·ROM·그래픽 영향: ROM +224 B(32,712,852 B). 세이브 무관.
- 코드 비교(#9466 직후 오브젝트 기준): 바뀐 함수는 날씨 판정 호출부(`AbilityBattleEffects`, `CanSetNonVolatileStatus`, `DoMoveDamageCalcVars`, `IsLeafGuardProtected`·`IsAbilityStatusProtected`·`BS_JumpIfAbilityPreventsRest`, `BS_JumpIfWeatherAffected`·`Cmd_damagecalc`·`Cmd_recoverbasedonsunlight`(static `GetWeather` 인라인 → 외부 호출), `CancelerCharging`·`GetDynamicMoveType`(호출 이름만))와, 줄 수 변화에 따른 assert `__LINE__` 상수·명령 배치만 다른 함수들(`Cmd_call`·`BS_SaveAttacker` 등 19개, `Cmd_getexp`·`Cmd_attackanimation`·`Cmd_endselectionscript`는 같은 호출 집합에서 레지스터·상수 배치만 다름).
- 검증:
  - `git diff --check`: 통과. 비ASCII 줄 변경 0.
  - `make hns -j8`: 종료 코드 0, ROM 32,712,852 B / EWRAM 248,924 B / IWRAM 25,516 B, 새 경고 없음.
  - 자동 테스트: 전체 실행(16분 24초, 병렬 부하로 느림). 목록 5,128행이 이식 전(#9466 직후 = `test-baseline-seq083.txt`)과 **바이트 동일**. 그중 날씨 관련 24파일(`test/battle/weather/*.c` + 기상예보·플라워기프트·아이스페이스·수확·아이스바디·건조피부·젖은접시·촉촉한몸·선파워·리프가드·엽록소·쓱쓱 등 특성, 만능우산) 테스트 145건: 전후 모두 PASS 83 / FAIL 61 / TO_DO 1.
  - 실기 확인: 권장(선택). 맑음·비에서 만능우산 보유자의 선파워·건조피부·젖은접시, 수확, 체리꼬 폼 체인지, 에어록/날씨부정 상태의 기상예보 복귀.
- 남은 위험: 낮음. AI 대미지 계산에서 선파워·플라워기프트 보정이 AI가 아는 도구(만능우산 미확인 시)를 기준으로 바뀐다(upstream 의도).

## 동기화 단위: seq 87 #9135 `U-9135` Allow other species to have Shedinja HP handling

- 현재 판정: 적용(HnS 적응: config 한 줄만)
- 커밋: `fd549f70bc`
- upstream 근거: `f851f3b8bf`
- 해결한 의존성: 없음. #9507(seq 91) 전이라 시그니처는 upstream 이 커밋과 같은 `bool32 HasShedinjaHPHandling(u32 species)`.
- 수정 파일: `include/config/pokemon.h`, `include/pokemon.h`, `src/pokemon.c`, `src/battle_dome.c`, `src/battle_dynamax.c`, `src/party_menu.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - `include/config/pokemon.h`: `P_SHOW_DYNAMIC_TYPES` 뒤에 `P_BASE_HP_1_SHEDINJA_HANDLING FALSE` 한 줄만 추가. HnS 설정값(`P_EGG_CYCLE_LENGTH GEN_3`, `P_SHOW_TERA_TYPE GEN_8`, `P_EGG_SHINY_ROLL_ON_PICKUP` 등)은 그대로. 새 줄은 ASCII(`Shedinja's`).
  - `src/pokemon.c`: `CalculateMonStats`의 비교 1곳 교체, 함수는 HnS 파일 끝(`GetPaldeaCatchProgress` 뒤)에 추가. `battle_dome.c` 1곳, `battle_dynamax.c` 3곳, `party_menu.c` 2곳은 그대로 적용(7곳, group plan과 같음).
  - 남은 `SPECIES_SHEDINJA` 비교(`battle_main.c` `TryCorrectShedinjaLanguage`, `evolution_scene.c` 껍질몬 생성, `trade.c` 이름 보정, `frontier_util.c` 데이터)는 1.17.0에서도 그대로인 비-HP 용도라 대상 아님.
- 저장·ROM·그래픽 영향: 기본값 FALSE라 동작 불변(`species == SPECIES_SHEDINJA`와 같음). ROM 크기 32,712,852 B(변화 0, 비교 명령이 같은 길이의 호출로 바뀜). 세이브 무관.
- 코드 비교: 바뀐 함수 8개(`CalculateMonStats`, 새 `HasShedinjaHPHandling`, `CalcDomeMonStats`, `ApplyDynamaxHPMultiplier`·`GetNonDynamaxHP`·`GetNonDynamaxMaxHP`, `ItemEffectToMonEv`·`ItemUseCB_Medicine`) 모두 비교식 → 함수 호출 교체뿐.
- 검증:
  - `git diff --check`: 통과. 비ASCII 줄 변경 0.
  - `make hns -j8`: 종료 코드 0, ROM 32,712,852 B / EWRAM 248,924 B / IWRAM 25,516 B, 새 경고 없음.
  - 자동 테스트: `test/battle/gimmick/dynamax.c` 81건(PASS 11 / FAIL 67 / 기타 3) — 이식 전 목록과 테스트별로 같음. upstream 테스트 hunk 없음.
  - 실기 확인: 불필요.
- 남은 위험: 없음.

## 동기화 단위: seq 88 #9460 `U-aiconfig-9460` Add AI internal config support

- 현재 판정: 적용(HnS 적응: 파일명 유지, 수동 삽입)
- 커밋: `d81b37f15b`
- upstream 근거: `62c4ac5f5a`
- 해결한 의존성: 없음. 같은 unit의 #9462(seq 131, `AI_REVERSE_BATTLER_LOGIC_ORDER_CHANCE` 등 실제 항목과 `WITH_CONFIG` 테스트)는 group plan상 "함께 넣는다"가 아니라 순서대로 뒤에서 넣으므로 이번에 넣지 않았다.
- 수정 파일: `include/constants/generational_changes.h`, `include/generational_changes.h`, `src/generational_changes.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - HnS 파일명 `generational_changes.*` 유지(#9460은 #9529 개명 전 커밋이라 upstream도 이 경로).
  - `include/constants/generational_changes.h`: HnS `POKEMON_CONFIG_DEFINITIONS` 끝(알 상속 5항목 `BALL/MOVE/NATURE/ABILITY_INHERITANCE`·`EGG_MOVE_TRANSFER` 뒤) 빈 줄 다음에 빈 `#define AI_CONFIG_DEFINITIONS(F) \`를, `enum ConfigTag`에 `AI_CONFIG_DEFINITIONS(UNPACK_CONFIG_ENUMS)`를 넣었다. 나머지 두 파일은 `git apply` 그대로(`struct GenChanges` → `ConfigChanges`, `config/ai.h` include, 게터·세터·클램퍼에 AI 매크로, 인자 이름 `_genConfig` → `_config`).
  - `GenChanges`·`_genConfig`를 쓰는 다른 곳 없음(`src`·`include`·`test`·`tools` 검색).
- 저장·ROM·그래픽 영향: **ROM 바이트 동일**(`cmp`). 헤더 변경으로 오브젝트 160개가 다시 컴파일됐고 결과가 같으므로 `sConfigChanges` 구조·`GetConfigInternal`(0x84 B) 불변. 세이브 무관.
- 검증:
  - `git diff --check`: 통과. 비ASCII 줄 변경 0.
  - `make hns -j8`: 종료 코드 0, ROM 32,712,852 B / EWRAM 248,924 B / IWRAM 25,516 B, 새 경고 없음.
  - 자동 테스트: upstream 테스트 hunk 없음. AI 더블 테스트 3건이 든 `ai_double_ace.c`(4건)·`ai_choice.c`(10건)·`ai_switching.c`(118건)를 돌려 이식 전 목록과 같음을 확인. **알려진 AI 3건은 그대로 FAIL**(`AI_FLAG_DOUBLE_ACE_POKEMON: Ace mons won't…`, `Choiced Pokémon won't switch out… 1/2 (1/?)`, `AI can switch out both mons… (1/?)`) — #9460은 빈 인프라뿐이고, 테스트의 `WITH_CONFIG(AI_REVERSE_BATTLER_LOGIC_ORDER_CHANCE, 0)`과 설정 항목은 seq 131 #9462에서 들어온다.
  - 실기 확인: 불필요(ROM 동일).
- 남은 위험: 없음.

## 동기화 단위: seq 89 #9514 `U-9514` SetMoveEffect cleanup

- 현재 판정: 적용(HnS 적응, **한글 문자열 토큰 교체 22건 / upstream이 바꾼 3건은 의도적으로 유지**)
- 커밋: `f0c3349daf`
- upstream 근거: `3bbcc63258`
- 해결한 의존성: 선행 #9176·#9446·#9249 모두 앞 구간에서 이식 완료. 후속 수정 #10064(seq 206, "이미 ~ 상태" 문구의 배틀러 수정)는 group plan상 #9655 unit이라 이번에 넣지 않았다(아래 "ALREADY 3건" 참고).
- 수정 파일(13): `data/battle_scripts_1.s`, `include/battle_scripts.h`, `include/constants/battle_string_ids.h`, `src/battle_message.c`, `src/battle_move_resolution.c`, `src/battle_script_commands.c`, `src/data/moves_info.h`, 테스트 6개(`test/battle/gimmick/dynamax.c`, `move_effect/heal_bell.c`, `move_effect_secondary/aromatherapy.c`·`light_screen.c`·`reflect.c`, `sleep_clause.c`)
- 적용 방법: upstream diff를 스크래치 사본에 `patch -F0`로 적용(문맥 불일치 hunk만 거부) → 거부된 hunk 6개와 한글 문자열 15 hunk를 손으로 옮김 → 결과를 저장소에 복사. 적용 뒤 HnS `SetMoveEffect`를 upstream #9514 직후 `SetMoveEffect`와 함수 단위로 비교해, 남은 차이가 HnS 고유 분기뿐임을 확인했다(Gen1 반동 챌린지 `genOneRecharge`, Core Enforcer `isFirstTurn != 2`(1.17.0과 같은 형태로 `effectBattler`), Fling #9951 순서, 오로라베일 `B_MSG_SET_AURORA_VEIL`, 깨뜨리다 계열 방벽 마스크, Spectral Thief 루프 밖 호출). `IgnoreTargetingForMoveEffect`·`DoesSubstituteBlockMoveEffectOnTarget`은 upstream과 같아졌다.
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - **방벽:** `BattleScript_MoveEffectReflect`·`LightScreen`·`AuroraVeil`을 upstream대로 `BattleScript_MoveEffectScreens`(`saveattacker` / `copybyte gBattlerAttacker, gEffectBattler` / `printfromtable gReflectLightScreenSafeguardStringIds` / `restoreattacker`)로 합쳤다. 오로라베일 C 분기의 HnS 선택값 `B_MSG_SET_AURORA_VEIL`(→ `STRINGID_PKMNRAISEDDEFSPDEF`)을 유지했다(upstream은 `B_MSG_SET_SAFEGUARD`). 리플렉터·빛의장막은 `TrySetReflect`·`TrySetLightScreen`이 정하는 단일/복수 선택값 그대로. 해당 기술 14개가 `.self = TRUE`가 되어 `gEffectBattler` = 사용자이므로 `copybyte`는 값이 같다. `BattleScript_BreakScreens` 순차 출력(리플렉터 → 빛의장막 → 오로라베일)과 `MOVE_EFFECT_BREAK_SCREEN`의 방벽 마스크 저장은 손대지 않았다.
  - **Feint·Hyperspace Fury:** HnS `BattleScript_HyperspaceFuryRemoveProtect`를 upstream처럼 없애고 `BattleScript_MoveEffectFeint`가 새 표 `gBrokeProtectionStringIds`(`B_MSG_FEINT` → `STRINGID_FELLFORFEINT`, `B_MSG_HYPERSPACE_FURY` → `STRINGID_BROKETHROUGHPROTECTION`)를 출력한다. 기술별 출력 ID는 이전과 같다.
  - `B_MSG_NO_MESSSAGE_SKIP` 오타 수정(값 불변), 스크립트 이름 정리(`StealthRockActivates` → `MoveEffectStealthRock` 등 11개)는 그대로.
- **한글 문자열 토큰 대응표(22건, 본문 바이트 동일):**

| STRINGID | 옛 토큰 → 새 토큰 | 한글 문장(새) |
|---|---|---|
| PKMNSQUEEZEDBYBIND | DEF→EFF, ATK→SCR | `{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_SCR_NAME_WITH_PREFIX}에게 조이기를 당했다!` |
| PKMNTRAPPEDINVORTEX | DEF→EFF | `{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n소용돌이 속에 갇혔다!` |
| PKMNWRAPPEDBY | DEF→EFF, ATK→SCR | `{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_SCR_NAME_WITH_PREFIX}에게 휘감겼다!` |
| PKMNCLAMPED | DEF→EFF, ATK→SCR | `{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_SCR_NAME_WITH_PREFIX}의 껍질에 꼈다!` |
| PKMNCAUSEDUPROAR | ATK→EFF | `{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 소란피기 시작했다!` |
| PKMNTRAPPEDBYSANDTOMB | DEF→EFF | `{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n모래지옥에 붙잡혔다!` |
| TRAPPEDBYSWIRLINGMAGMA | DEF→EFF | `{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n불꽃의 소용돌이에 갇혔다!` |
| INFESTATION | ATK2→SCR2, DEF→EFF | `{B_SCR_NAME_WITH_PREFIX2}{B_TXT_EUNNEUN}\n{B_EFF_NAME_WITH_PREFIX}에게 엉겨 붙었다!` |
| BURSTINGFLAMESHIT | SCR2→EFF2 | `분출하는 불꽃이\n{B_EFF_NAME_WITH_PREFIX2}에게 명중했다!` |
| FELLFORFEINT | DEF→EFF | `{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n페인트에 걸렸다!` |
| ATTACKERLOSTFIRETYPE | ATK→EFF | `{B_EFF_NAME_WITH_PREFIX}의 불꽃은 다 타 버렸다!` |
| BROKETHROUGHPROTECTION | DEF2→EFF2 | `{B_EFF_NAME_WITH_PREFIX2}의\n방어를 깨뜨렸다!` |
| PKMNINSNAPTRAP | DEF→EFF | `{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n집게덫에 붙잡혔다!` |
| ATTACKERLOSTELECTRICTYPE | ATK→EFF | `{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n전기를 다 써 버렸다!` |
| THUNDERCAGETRAPPED | ATK→SCR, DEF2→EFF2 | `{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_EFF_NAME_WITH_PREFIX2}{B_TXT_EULREUL} 번개우리로 가뒀다!` |
| TARGETISBEINGSALTCURED | DEF→EFF | `{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n소금에 절여졌다!` |
| TARGETCOVEREDINSTICKYCANDYSYRUP | DEF→EFF | `{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n물엿범벅이 되었다!` |
| TEAMTRAPPEDWITHVINES | DEF_TEAM1→EFF_TEAM1 | `{B_EFF_TEAM1} 포켓몬은\n채찍의 맹타에 휩싸였다!` |
| TEAMCAUGHTINVORTEX | DEF_TEAM1→EFF_TEAM1 | `{B_EFF_TEAM1} 포켓몬은\n거친 물살에 휩싸였다!` |
| TEAMSURROUNDEDBYFIRE | DEF_TEAM1→EFF_TEAM1 | `{B_EFF_TEAM1} 포켓몬은\n불꽃에 휩싸였다!` |
| TEAMSURROUNDEDBYROCKS | DEF_TEAM1→EFF_TEAM1 | `{B_EFF_TEAM1} 포켓몬은\n바위에 둘러싸였다!` |
| ATTACKERLOSTITSTYPE | ATK→EFF | `{B_EFF_NAME_WITH_PREFIX}의\n타입이 원래대로 되돌아왔다!` |

  - 옛 문장은 표의 새 토큰을 옛 토큰으로 되돌린 것과 같다(`PREFIX`/`PREFIX2` 구분은 HnS 문장 그대로 유지, 예: INFESTATION 첫 토큰은 `B_ATK_NAME_WITH_PREFIX2` → `B_SCR_NAME_WITH_PREFIX2`). upstream의 토큰 대응(대상 DEF→EFF, 주체 ATK→SCR, 자기 효과 ATK→EFF, 불꽃튀기기 SCR→EFF)을 HnS 문장의 같은 자리 토큰에 그대로 적용했다.
  - **검증 (1) 본문:** 네 종류 이름 토큰을 같은 자리표시로 바꾼 뒤 옛/새 줄의 UTF-8 바이트가 22건 모두 같다. **(2) 조사·줄 제어:** `{B_TXT_EUNNEUN}`·`{B_TXT_EULREUL}` 목록과 `\n` 개수 22건 모두 같다. **(3) 토큰 정의:** `charmap.txt` `B_EFF_NAME_WITH_PREFIX = FD 11`, `B_SCR_NAME_WITH_PREFIX = FD 13`, `B_EFF_NAME_WITH_PREFIX2 = FD 47`, `B_SCR_NAME_WITH_PREFIX2 = FD 48`, `B_EFF_TEAM1 = FD 4D`. `battle_message.c` 확장부는 네 배틀러 모두 같은 `HANDLE_NICKNAME_STRING_CASE(배틀러)`·`HANDLE_NICKNAME_STRING_LOWERCASE(배틀러)` 매크로(상대/야생 접두어 `sText_FoePkmnPrefix`·`sText_WildPkmnPrefix`·소문자판 동일)와 같은 `sText_Your1`/`sText_Opposing1` 분기를 쓴다. 조사 토큰은 직전에 출력된 글자 기준이라 토큰 종류와 무관하다. 이 토큰 코드를 따로 해석하는 곳은 확장부 외에 없다(`src`·`include`·`test`·`tools` 검색).
  - **ROM 확인:** 이식 전후 ROM의 `gBattleStringsTable` 726개 항목을 모두 읽어 비교했다. 달라진 것은 위 22건뿐이고, 22건 모두 길이가 같으며 `FD` 다음 토큰 코드 1바이트만 다르다(FD 10→11, 0F→13, 0F→11, 45→48, 46→47, 48→47, 41→4D). 스크립트 `p84/strcheck.py`.
- **배틀러 일치 확인(출력 경로별):** `SetMoveEffect` 머리에서 `gBattleScripting.battler = battlerAtk`, `gEffectBattler = effectBattler`(HnS도 같음).
  - 조이기 계열 9건(`gWrappedStringIds`): `MOVE_EFFECT_WRAP`를 가진 기술 10개가 모두 `.self` 없음 → `Cmd_setadditionaleffects`가 `SetMoveEffect(gBattlerAttacker, gBattlerTarget, …)`로 부른다. `BattleScriptPush` 직후 `BattleScript_MoveEffectWrap`의 첫 명령이 `printfromtable`이라 사이에 바뀌는 곳이 없다. 이 스크립트를 부르는 다른 곳 없음 → EFF = 옛 DEF, SCR = 옛 ATK.
  - Uproar(`.self`), Burn Up·Double Shock(`MOVE_EFFECT_REMOVE_ARG_TYPE`, `.self` 2개): `effectBattler = gBattlerAttacker` → EFF = 옛 ATK. 각 스크립트(`BattleScript_MoveEffectUproar`, `BattleScript_Remove*Type`)의 호출처는 해당 `SetMoveEffect` 분기 하나뿐.
  - Feint 문구 2건(`MOVE_EFFECT_FEINT` 기술 5개 모두 `.self` 없음), Salt Cure, Syrup Bomb, G-Max 4종(Vine Lash·Wildfire·Cannonade·Volcalith): 모두 대상 효과 → EFF = 옛 DEF / EFF_TEAM1 = 옛 DEF_TEAM1. 스크립트 호출처는 각 분기 하나뿐.
  - Flame Burst: C에서 `gBattleScripting.battler = partnerTarget` → `gEffectBattler = partnerTarget`, 스크립트 `BS_SCRIPTING` → `BS_EFFECT_BATTLER`, 문자열 SCR2 → EFF2를 함께 바꿔 가리키는 배틀러(대상의 파트너)가 같다.
  - `savetarget; copybyte gBattlerTarget, gEffectBattler`가 새로 붙은 스크립트(Psychic Noise, Sappy Seed, Core Enforcer, Eerie Spell, Spectral Thief, G-Max Snooze, G-Max Depletion, 거다이 측면 상태이상 루프 7종)는 모두 대상 효과라 `gEffectBattler == gBattlerTarget`. 스펙트럴시프는 공격 전 효과 루프에서 부르지만, 대상이 아닌 배틀러는 캔슬러에서 `targetsDone`이 켜져 `GetPossibleNextTarget`이 실제 대상만 돌려준다. 아군 루프 3종(G-Max Chi Strike·Finale·Replenish)은 `.self`가 되어 `sBATTLER = gEffectBattler = 사용자`(옛 `gBattlerAttacker`와 같음). 루프 안 명령(`statbuffchange` 증가 경로, `BS_HealOneSixth`, `BS_TryRecycleBerry`, HP 갱신)은 `gBattleScripting.battler`를 바꾸지 않는다. 이 스크립트들의 문자열은 바뀌지 않았다.
  - **배틀러 불일치 경로: 0건**(바꾼 22건 기준).
- **ALREADY 3건 (upstream과 다르게 유지, 판단 사항):** upstream #9514는 `STRINGID_PKMNALREADYASLEEP`·`PKMNALREADYPOISONED`·`PKMNISALREADYPARALYZED`의 `{B_DEF_NAME_WITH_PREFIX}`를 `{B_SCR_NAME_WITH_PREFIX}`로 바꿨다. 이 문구는 상태 기술의 `trynonvolatilestatus`(`CanSetNonVolatileStatus(…, RUN_SCRIPT)`)가 `BattleScript_AlreadyAsleep`·`AlreadyPoisoned`·`AlreadyParalyzed`로 보낼 때만 나오는데, 이 경로는 `gBattleScripting.battler`를 대상으로 설정하지 않는다(`IsNonVolatileStatusBlocked`가 특성 방어일 때만 설정, HnS·upstream #9514 모두 같음). 그래서 SCR로 바꾸면 "이미 잠들어 있다" 문장에 대상이 아닌 배틀러(보통 공격자) 이름이 나온다. upstream도 이것을 #10064(seq 206, `3ed1ce5570` "Fix already-status messages using the wrong battler", 테스트 `NOT MESSAGE("Wobbuffet is already paralyzed!")`)로 고쳤다. g1 plan의 #10064 항목도 "HnS 한글 문장({B_DEF_NAME_WITH_PREFIX} 사용)은 바꾸지 않는다"고 적고 있다. 따라서 HnS 세 문장은 `B_DEF` 그대로 두었다(출력 불변). #10064를 이식할 때는 엔진 1줄만 넣고 문장은 그대로 두면 된다.
  - `STRINGID_RESETSTARGETSSTATLEVELS`: HnS 문장("모든 상태가\n원래대로 되돌아왔다!")에 이름 토큰이 없어 바꿀 것이 없다.
- **upstream 동작 수정이 함께 들어온 것(메시지 문구 불변, 조건·상태만):**
  - 방벽·중력·아로마테라피·팀 회복·열매 재생·팀 능력치 상승·급소 랭크 부가 효과를 가진 14개 기술(Glitzy Glow·Baddy Bad·Sparkly Swirl, Max Knuckle·Max Ooze·Max Airstream·Max Quake·Max Steelspike, G-Max Chi Strike·Resonance·Replenish·Gravitas·Sweetness·Finale)에 `.self = TRUE`. 효과 대상이 사용자로 계산된다. HnS 기술 데이터에서 이 효과를 가진 기술은 이 14개뿐이다. 부작용으로 AI 점수의 "대상 효과" 분기에 있던 `MOVE_EFFECT_RAISE_TEAM_*`·`GRAVITY`·`AURORA_VEIL` 가산이 이 기술들에 더는 적용되지 않는다(upstream #9514 직후와 1.17.0 모두 같은 상태).
  - G-Max Gold Rush(`MOVE_EFFECT_CONFUSE_PAY_DAY_SIDE`): 트레이너 배틀에서 **플레이어 편이 쓸 때만** 돈이 늘고 "돈이 주위에 흩어졌다!"가 나온다(이전에는 상대가 써도 나옴).
  - G-Max Snooze(`MOVE_EFFECT_YAWN_FOE`): `CanBeSlept`의 공격자 인자가 대상 자신 → 실제 공격자. 수면 판정에서 공격자 인자를 쓰는 곳은 수면 클로즈 아군 예외(`CanBeSlept`가 먼저 클로즈를 검사해 도달 전 반환)와 신비의부적(아군이어도 막음, 공격자 특성은 `ABILITY_NONE`으로 전달)뿐이라 결과는 같다.
  - Jaw Lock(`MOVE_EFFECT_TRAP_BOTH`): 둘 다 도망 불가가 아닐 때만 상태를 건다(메시지 조건은 이전과 같음).
  - Sappy Seed(`BattleScript_MoveEffectLeechSeed`) 스크립트 끝 `goto BattleScript_MoveEnd` → `return`, G-Max Depletion(`BattleScript_MoveEffectSpite`) 실패 분기 `BattleScript_MoveEnd` → 복귀: 스크립트 스택에 복귀 주소가 남던 문제 수정(출력 순서 같음).
  - G-Max Befuddle·Smite·Gold Rush·Cuddle 루프(`ConfuseSide`·`InfatuateSide`)의 애니메이션 대상이 루프의 현재 대상으로(이전에는 항상 첫 효과 대상).
  - Fling 아이템을 `gLastUsedItem` 대신 사용자의 현재 도구에서 읽음, 미러아머 판정을 효과 대상 기준으로.
- 저장·ROM·그래픽 영향: ROM −224 B(32,712,628 B). 세이브 무관.
- 검증:
  - `git diff --check`: 통과. 비ASCII 줄 변경: `src/battle_message.c` 22쌍(44줄)뿐, 위 대응표와 같음.
  - `make hns -j8`: 종료 코드 0, ROM 32,712,628 B / EWRAM 248,924 B / IWRAM 25,516 B, 새 경고 없음.
  - 자동 테스트: L 크기라 전체 실행(11분 47초). 목록 5,128행이 이식 전(#9510 직후 전체 실행 = `test-baseline-seq083.txt`)과 **바이트 동일**, CRASH/INVALID 줄도 같음. #9514가 건드린 테스트 6파일과 관련 파일(`test/battle/move_effect_secondary/*.c` 전체, `move_effect/` 방벽·오로라베일·중력·씨뿌리기·코어퍼니셔·내던지기·원한·소란·수면·마비·독, `gimmick/dynamax.c`, `sleep_clause.c`) 52파일 319건: 전후 모두 PASS 100 / FAIL 196 / KNOWN_FAILING 1 / TO_DO 22. upstream 테스트 hunk(`ASSUME(MoveHasAdditionalEffect…)` → `MoveHasAdditionalEffectSelf`, 14줄)는 그대로 이식했다(영문 `MESSAGE` 기대값 변경은 이 PR에 없음).
  - 실기 확인: 권장. 조이기 계열 10개(Bind·Wrap·Fire Spin·Clamp·Whirlpool·Sand Tomb·Magma Storm·Infestation·Snap Trap·Thunder Cage) 문장의 두 이름과 조사(싱글·더블, 상대/야생 접두어), Uproar·Burn Up·Double Shock, Feint·Hyperspace Fury 방어 해제 문구, Flame Burst 파트너 이름(더블), Salt Cure·Syrup Bomb, Glitzy Glow·Baddy Bad(방벽 문구)·G-Max Resonance(오로라베일 성공 문구 `STRINGID_PKMNRAISEDDEFSPDEF`), 깨뜨리다·사이코팽의 방벽 순차 해제 문구, 수면·독·마비 상태 기술을 이미 그 상태인 상대에게 썼을 때 대상 이름(변경 없음 확인).
- 남은 위험: 중간. 스크립트·`SetMoveEffect` 전반의 변수 정리라 위 경로 외 드문 조합(다이맥스 기술, 더블배틀 부가 효과)은 실기로 확인하는 것이 좋다. `BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 G-Max Gold Rush 1행을 추가했다.

## 동기화 단위: seq 90 #9539 `U-9539` Some bool cleanup

- 현재 판정: 적용(그대로)
- 커밋: `7b578d7bb2`
- upstream 근거: `439b38b990`
- 해결한 의존성: 없음
- 수정 파일: `src/easy_chat.c`(`EasyChatIsNationalPokedexEnabled`·`IsEasyChatIndexAndGroupUnlocked`·`IsRestrictedWordSpecies` 반환형 → `bool32`, 선언 3줄·정의 3줄)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: `git apply` 그대로. 문자열 무관.
- 저장·ROM·그래픽 영향: ROM −16 B(32,712,612 B). 세이브 무관.
- 코드 비교: `easy_chat.o` 함수 58개 중 2개만 다름 — `IsEasyChatIndexAndGroupUnlocked`(`bool8` 절단 `lsls/lsrs #24` 제거, `enabled`를 `ldrb` 대신 `ldr`로 읽음)와 `InitEasyChatScreen`(인라인된 `EasyChatIsNationalPokedexEnabled`의 절단 제거). `EasyChatWordInfo.enabled`는 `int`이고 데이터 값은 `TRUE` 1,002개·`FALSE` 6개뿐, `IsNationalPokedexEnabled()`는 `bool32` TRUE/FALSE라 결과가 같다.
- 검증:
  - `git diff --check`: 통과. 비ASCII 줄 변경 0.
  - `make hns -j8`: 종료 코드 0, ROM 32,712,612 B / EWRAM 248,924 B / IWRAM 25,516 B, 새 경고 없음.
  - 자동 테스트: 해당 테스트 없음. 구간 끝 전체 실행에 포함.
  - 실기 확인: 불필요.
- 남은 위험: 없음.

## seq 84~90 요약

| seq | PR | 판정 | 커밋 | ROM 변화 |
|---:|---|---|---|---:|
| 84 | #9376 | 적용(hunk 단위 수동) | `6e050d6e70` | 0 B |
| 85 | #9466 | 적용(hunk 단위 수동) | `a4c46a4a8f` | +80 B |
| 86 | #9510 | 적용(HnS 적응) | `7a6cd5b51b` | +224 B |
| 87 | #9135 | 적용(config 한 줄) | `fd549f70bc` | 0 B |
| 88 | #9460 | 적용(파일명 유지, ROM 바이트 동일) | `d81b37f15b` | 0 B |
| 89 | #9514 | 적용(HnS 적응, 한글 토큰 교체 22건) | `f0c3349daf` | −224 B |
| 90 | #9539 | 적용(그대로) | `7b578d7bb2` | −16 B |

- 구간 밖 행은 넣지 않았다.
- 마지막 빌드(`e1b1914846` 소스, 메인이 `rm -f pokehns.elf pokehns.gba` 뒤 `make hns -j8`로 재링크): 종료 코드 0, ROM 32,712,612 B(97.49%, 구간 시작 대비 +64 B), EWRAM 248,924 B(94.96%, 변화 0), IWRAM 25,516 B(77.87%, 변화 0). SHA1 `9dcd5c1578cbf4a7b117e3681eb030478bb4742b`.
- 한글이 든 소스 줄: `src/battle_message.c` 22쌍뿐이다(#9514). 메인이 따로 확인한 결과, `{B_DEF_*}/{B_ATK_*}` → `{B_EFF_*}/{B_SCR_*}` 토큰 자리를 같은 자리표시로 바꾸면 옛 줄과 새 줄이 22건 모두 바이트 동일하다.

### 전체 테스트 (구간 끝)

- 에이전트의 구간 끝 전체 실행은 사용자 지시(인터넷 연결 문제)로 중단됐다. 메인이 재개 뒤 `make check BUILD=hns -j8`를 다시 돌렸다.
- 결과: PASS 2,298 / FAIL 2,229 / KNOWN_FAILING 8 / ASSUMPTIONS_FAILED 38 / TO_DO 618 / EXPECT_FAILING 6 / TOTAL 5,197. assertion·illegal opcode·Killed 0.
- 테스트별 목록(5,128행)이 [`test-baseline-seq083.txt`](test-baseline-seq083.txt)와 **바이트 동일**하다. 회귀 0. 새 기준 목록: [`test-baseline-seq090.txt`](test-baseline-seq090.txt)(내용은 seq083과 같다).
- AI 더블 테스트 3건(`AI_FLAG_DOUBLE_ACE_POKEMON: Ace mons…`, `Choiced Pokémon won't switch out…`, `AI can switch out both mons on the same turn…`)은 #9460 뒤에도 FAIL이다. #9460은 빈 AI config 틀만 추가하므로 예상대로다. seq 131 #9462 이식 뒤 다시 확인한다.
- 목록 추출 주의: "~ fit on ~" 계열 23개 테스트는 실패 내용의 한글 인코딩 바이트가 테스트 이름 줄에 붙어 출력된다. UTF-8 로케일에서 `grep`·`sed`를 쓰면 이 줄이 빠지므로 `LC_ALL=C`와 `grep -a`로 추출해야 기준 목록과 비교할 수 있다.

### 실기 확인 필요 (mGBA)

1. #9514: 조이기 계열 10개(조이기·김밥말이·회오리불꽃·껍질끼우기·바다회오리·모래지옥·마그마스톰·엉겨붙기·집게덫·썬더프리즌) 문장의 두 이름과 조사(싱글·더블, 상대/야생 접두어), 소란피기·불사르기·전광쌍격, 페인트·이차원러시 방어 해제 문구, 불꽃튀기기 파트너 이름(더블), 소금절이·시럽봄, 방벽을 치는 기술의 방벽 문구, 오로라베일 성공 문구.
2. #9514: 거다이골드러시는 플레이어 편이 쓸 때만 "돈이 주위에 흩어졌다!"가 나온다(upstream 동작 변경, `BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 기록).
3. #9510(선택): 맑음·비에서 만능우산 보유자의 선파워·건조피부·젖은접시, 수확, 체리꼬 폼 체인지, 에어록·날씨부정 상태의 기분파(캐스퐁) 복귀.

### 다음 구간 담당 참고

- 다음 시작: seq 91 #9507 `U-species-enum-9507` Add Species enum (XL, 단독 구간).
- #10064(seq 206)를 이식할 때 HnS의 "이미 잠들어/독/마비" 세 문장은 `{B_DEF_NAME_WITH_PREFIX}`를 그대로 두고 엔진 1줄만 넣는다(#9514 항목 "ALREADY 3건").
