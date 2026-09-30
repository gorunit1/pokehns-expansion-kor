# full-sync 선진행 결과 — 구간 1 (seq 130~167 중 15행)

진행 중: 마지막 완료 seq 158, 다음 seq 160

**순서표와 다르게 진행한 구간이다.** 순서표([`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv))의 다음 행 seq 127 #9655(배틀 메시지 리팩터)가 D1~D7 결정 대기라서, #9655와 무관한 뒤쪽 행을 앞당겨 이식한다. 선정 기준과 전체 분류는 [`ahead-of-9655/README.md`](ahead-of-9655/README.md)(`ahead_candidates.tsv`)에 있다. 이 구간은 그 "선진행" 114행 가운데 앞쪽 15행(seq 130, 131, 133, 134, 137, 143, 148, 149, 151, 156, 157, 158, 160, 165, 167)이다.

- 이 구간에서 건너뛴 seq 127~129·132·135·136·138.5·139~142·144~147·150·152~155·159·161~164·166은 보류(#9655 의존·#9655 파일·unit/deps 선행 제외·줄 겹침) 또는 제외(이미 적용: seq 138·163) 행이다. 원래 자리에서 진행한다.
- **seq 167 #9819도 이번 구간에서 적용하지 않았다(보류, 중간 회귀 회피: #10548 직전 적용).** 이유는 아래 seq 167 항목.

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), [`CLAUDE_FULL_SYNC_PORT_PROMPT.md`](../../CLAUDE_FULL_SYNC_PORT_PROMPT.md), 메시지 출력 기록: [`BATTLE_MESSAGE_OUTPUT_CHANGES.md`](../../../localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md)
시작 HEAD: `e629de8bdf` (작업 트리 clean. `673240f6ae`(seq 126 코드) 뒤로는 docs만 바뀜)

## 요약

| seq | PR | 판정 | 커밋 | ROM 변화 | 비고 |
|---|---|---|---|---:|---|
| 130 | #9575 | 적용(HnS 적응) | `0f60183f1f` | −192 B | AI 예측 처리 통합. `battle_ai_main.c` 수동 맞춤(HnS `battlerMovesScored` 줄·무조건 디버그 타이머 유지). **예측 AI 트레이너 25명의 AI 동작이 1.17.0과 같아짐** |
| 131 | #9462 | 부분 적용(잔여분) | `99388d6158` | +32 B | 게임 기능(`AI_REVERSE_BATTLER_LOGIC_ORDER_CHANCE`·RNG·뒤집기 로직)은 HnS에 이미 있음. `GetConfig` 2곳·`AI_CONFIG_DEFINITIONS` 2항목·테스트 4파일만 이식(`(reversed)` Choiced 테스트 값 `ABILITY_GLUTTONY`). 게임 동작 불변 |
| 133 | #9006 | 적용(HnS 적응) | `fc205e1a40` | −640 B (EWRAM −4 B) | 기술 떠올리기를 공용 `LearnMove`로. HnS chooseboxmon·요약 START/R/L 유지, 검은먹시티 NPC `Special_HasMoveToRelearn`/`VAR_RESULT`, **#10223 1줄 선반영**, **가르침 교체 최대 PP 유지(`// HnS:`)**, **새 문자열 2개 한글 초안(미결)** |
| 134 | #9903 | 부분 적용(HnS 적응) | `cf71e21e56` | 0 B | relearner hunk만 `HandleMoveRelearnerInput`(#9006)으로 옮겨 넣음. 이름 바꾸기 hunk 제외(HnS 요약 화면에 분기 없음). config로 꺼진 경로라 동작 불변(코드 바이트 동일, assert 줄 번호 문자열만 이동) |
| 137 | #9713 | 적용(HnS 적응, **B안**) | `d057cee5c2` | −1,296 B | 디버그 사운드 메뉴 `FindSong`/`sSongNames`. **곡 이름 저장 안 함(`SE_`/`MUS_` 접두어만, Korean patch 화면 유지)**. HnS GBS 전환 유지, `FIRST_PHONEME_SONG`은 `DP_MUSIC_END + 1`(값 746 불변), DP 음악 11곡·`SE_FASTER_JOY_HEAL` 목록 추가. 이름 `{0}`(EOS 없음) EWRAM 덮어쓰기 잠재 버그 해소 |
| 143 | #9721 | 적용 | `f3893a4cb4` | 0 B | `Makefile` 1줄(learnables JSON order-only 의존 삭제). **seq 137 빌드와 `pokehns.gba` SHA1 동일** |
| 148 | #9690 | 적용(문맥 수동) | `aeab20beac` | +16 B | 파운드 변환 식 현대화(`DECAGRAMS_IN_POUND` 453592, u64 식). `pokedex.h` 문맥(`IS_HNS`)만 다름. **옵션 "단위계 = 야드파운드법"일 때만** 도감 무게 일부 +0.1 lb(기본 미터법 화면 불변) |
| 149 | #9461 | 적용(HnS 적응) | `9f6c5b5c58` | +128 B | 맵 팝업 층 번호(`MapHeader.floorNumber`, mapjson). HnS 적응 3곳: 피라미드 조건 유지, **`FONT_NARROW` 유지**, `CELADON DEPT.` 특례 `!IS_HNS`. HnS 맵 `floor_number` 0개라 팝업 문구·맵 헤더 바이트 불변. 새 테스트 `Map names fit in popup` PASS |
| 151 | #9755 | 적용 | `ce1fc01da9` | −16 B | AI `IsDamageMoveUnusable`의 `HasWeatherEffect()` 이중 검사 제거(upstream 그대로). `ctx->weather`가 이미 날씨 무효를 반영해 사실상 동작 동일 |
| 156 | #9774 | 적용 | `adb22cd5f0` | 0 B | Fallarbor 떠올리기 NPC 판정 `VAR_0x8004, 0` → `VAR_RESULT, FALSE`(upstream 그대로). HnS 실사용 검은먹시티 NPC의 같은 수정은 seq 133에 포함. Fallarbor는 HnS에서 도달 불가 |
| 157 | #8628 | 적용 | `5381abba16` | 0 B | `setmetatileinrange` 매크로(`callnative`, 새 opcode 없음)와 `NativeFunc_SetMetatileInRange`. 쓰는 스크립트가 없어 함수는 gc로 빠짐. 공백 1줄 정리 |
| 158 | #9765 | 적용 | `2cef59f506` | −5,360 B (EWRAM −4 B) | 도감 분포 지도 템플릿 제거, BG 3 상수. 쓰이지 않던 affine 그래픽·BG 번호 힙 할당 삭제. HnS `pokedex_area_screen.c` 고유 변경 보존. 동작 동일 |

## 공통 사항

- 툴체인: 이 컴퓨터의 기본 PATH `arm-none-eabi-gcc` 13.2.1(`/usr/bin`). PATH 접두어 없이 빌드했다.
- 빌드 명령: `GITHUB_ACTION=1 make hns -j8 > build/port.log 2>&1`
- 이식 전 기준: HEAD `e629de8bdf` 빌드(= `673240f6ae` 빌드, 작업 트리의 `pokehns.gba` SHA1 `197afe076fa94dccc2579157efc696d93ca3a63b`) ROM 32,722,852 B / EWRAM 248,944 B / IWRAM 25,516 B.
- 경고 비교: 기준 경고 목록(`build/port-base-full.log`에서 만든 "파일: 메시지" 고유 42개, 사본 `/home/hjm0725/hns-sync-work/chunk-ahead-130-167/warn-base.txt`)과 매 빌드 경고를 같은 형식으로 비교해 목록에 없는 것을 "새 경고"로 셌다.
  `LC_ALL=C grep -a 'warning:' build/port.log | LC_ALL=C sed -E 's/:[0-9]+:[0-9]+: /: /' | LC_ALL=C sort -u | comm -13 warn-base.txt -`
- 테스트 명령: 파일별 `GITHUB_ACTION=1 make check BUILD=hns -j6 TESTS="<파일>"`. 파일마다 따로 돌려 `PORT_INSTRUCTIONS`의 `LC_ALL=C`·`grep -a` 추출로 목록을 만들고, 같은 테스트 이름의 [`test-baseline-seq126.txt`](test-baseline-seq126.txt) 줄과 비교했다. 전체 테스트는 메인이 구간 끝에 돌린다.
- 사전 분석: 읽기 전용 분석 에이전트가 PR별 이식 계획·적응 patch·검증 도구를 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-ahead-130-167/`에 만들었다(`seq<SEQ>-<PR>.md`/`.patch`, `tmp-<seq>/`). 메인이 patch 14개(+#9819)를 스크래치 사본에서 순서대로 쌓아 모두 적용되고 `git diff --check`도 통과함을 확인했다(`stackcheck/`). 적용 직전에 patch를 `git apply --check`로 다시 확인했다. patch에는 파일 모드 줄이 없다.

## 동기화 단위: seq 130 #9575 `U-9575` Unify AI prediction handling

- 현재 판정: 적용(HnS 적응)
- 커밋: `0f60183f1f`
- upstream 근거: `c06999df7d`(5파일 +50/−65, 테스트 변경 없음). deps #9596(seq 113, `f6ef307f76`) 적용됨.
- 수정 파일(5): `include/battle_ai_main.h`, `include/battle_ai_util.h`, `src/battle_ai_main.c`, `src/battle_ai_util.c`, `src/battle_main.c`
- 적용 방법: 사전 분석 patch(`seq130-9575.patch`)를 `git apply`했다. 충돌 없음. `battle_ai_main.c`의 두 hunk는 사전 분석이 HnS 문맥에 맞춰 손으로 만든 것이다(upstream hunk는 HnS 문맥과 달라 그대로 적용되지 않음).
- 내용(upstream):
  - `CanAiPredictMove` 삭제, `ComputeBattlerDecisions` → `ComputeAiBattlerDecisions`. AI 배틀러 판정(`isAiBattler`)을 `HandleTurnActionSelectionState` 호출부로 옮겨 AI 배틀러일 때만 부른다.
  - 턴 시작 `SetAiLogicDataForTurn`: 플레이어 쪽 배틀러마다(플래그 무관) `BattleAI_SetupAIData` + `SetupAIPredictionData(battler)`. 교체 예측(`PREDICT_SWITCH`)과 기술 예측(`PREDICT_MOVE`: `BattleAI_ChooseMoveIndex` → `predictedMove`)을 여기서 한다.
  - `predictingSwitch`/`predictingMove`는 행동을 고르는 AI 배틀러에게 해당 플래그가 있을 때만 굴린다. `IsBattlerPredictedToSwitch`·`GetIncomingMove*`의 플래그 검사 제거.
  - 첫 턴 `SetShellSideArmCategory`·`SetAiLogicDataForTurn`을 `AssignUsableGimmicks()` 바로 뒤로 옮김(AI 코드는 사이의 `gQueuedStatBoosts`·`gBattleCommunication`·`eventState`를 읽지 않음).
- HnS 적응(모두 기존 차이, 이번 PR이 만든 차이 아님):
  - `ComputeAiBattlerDecisions` 끝의 `gAiLogicData->battlerMovesScored |= 1u << battler;` 유지(HnS 원본부터 있던 #9448 형태, 1.17.0에도 같은 자리에 있음).
  - `AIDebugTimerStart/End`에 `if (DEBUG_AI_DELAY_TIMER)`가 없는 HnS 형태(#9585 `d50afab833`) 유지.
  - `SetAiLogicDataForTurn`의 `turnOrder` 두 줄은 HnS에 없다(seq 113에서 제외, 1.17.0에도 없음).
  - g6 plan의 "HnS 난이도 설정 분기"는 이 함수들에 없다(`battle_ai_main.c`의 `IS_HNS`/`HnS:` 0건).
- 제외한 hunk: 없음.
- **upstream대로 둔 변화(AI 동작, 1.17.0과 같아짐):**
  - `AI_FLAG_PREDICTION`을 쓰는 HnS 트레이너 **25명**(`src/data/trainers_hns.party`의 `AI: … Prediction`): `STEVEN_HNS`, `FINLEY_HNS`·`MUALANI_HNS`(더블배틀), `*_POSTOBC_HNS` 22명(관장 16, 사천왕 4, 목호, 레드).
    - 교체 예측 대상이 "AI 배틀러의 맞은편 한 명"에서 "플레이어 쪽 전원"으로 바뀐다(더블배틀에서 차이).
    - 기술 예측이 턴 시작 때 `BattleAI_ChooseMoveIndex`로 한 번 계산된다. 이전의 행동 선택 때 플레이어 배틀러 점수 재계산이 없어져 AI 계산량이 줄었다.
  - 예측 플래그 없는 AI 배틀: `RNG_AI_PREDICT_SWITCH` 굴림이 사라지고, 더블배틀에서는 턴 시작마다 플레이어 배틀러의 `BattleAI_SetupAIData`(`SetRandomTarget`)가 새로 불린다. 난수 소비 순서만 바뀌고 판단 결과는 같다.
  - 인게임 파트너 더블배틀: 플레이어 배틀러의 `battlerMovesScored` 비트가 켜지지 않아 파트너 AI의 `ShouldAvoidProtectingAgainstPartnerMove`가 플레이어 기술을 보지 않는다(1.17.0과 같음).
  - 배틀 메시지 출력 변화는 없다(`BATTLE_MESSAGE_OUTPUT_CHANGES.md` 변경 없음).
- 검증:
  - `git diff --check` 통과. `ComputeBattlerDecisions`·`CanAiPredictMove` 남은 곳 0(`src`·`include`·`test`). 한글 줄·config·세이브 구조체 변경 0.
  - 빌드(`build/port.log`): 종료 코드 0, **ROM 32,722,660 B(97.52%, −192 B) / EWRAM 248,944 B(0) / IWRAM 25,516 B(0)**. 사전 분석 추정 약 −236 B(정렬 전). `pokehns.gba` SHA1 `d3c855dd3ccac508a230b67172f6f3b84db7f070`. 새 경고 0.
- 테스트(지정 9파일: `test/battle/ai/` 아래 `ai_flag_predict_move.c`, `ai_flag_predict_switch.c`, `ai.c`, `ai_check_viability.c`, `check_bad_move.c`, `gimmick_z_move.c`, `ai_switching.c`, `ai_doubles.c`, `ai_multi.c`) → 336줄(PASS 286)이 seq 126 기준 목록의 같은 줄과 **모두 같다**. 예측 테스트 `ai_flag_predict_move.c` PASS 3, `ai_flag_predict_switch.c` PASS 11, `ai.c`의 "AI thinking time doesn't explode" 6건 PASS 유지. 이 PR은 `test/**`를 바꾸지 않는다.
- 남은 위험:
  - 낮음: 녹화 배틀(더블) 재생 중 난수 소비가 바뀌어 이식 전 녹화가 어긋날 수 있다(#8943 A안의 기존 녹화 무효화 허용 범위).
  - 낮음: `SetupAIPredictionData`는 플레이어 쪽 배틀러의 생존 여부를 보지 않는다(1.17.0과 같음).
- 실기 확인: 필요(아래 "실기 확인 항목" 1).

## 동기화 단위: seq 131 #9462 `U-aiconfig-9460` Config to randomize the order AI mons compute logic in double battles

- 현재 판정: 부분 적용(잔여분). 예전 판정표 `all_prs_master.tsv`의 "이미 적용(기능 동등)"은 게임 코드만 본 판정이었다. g4 plan의 "잔여분 이식"이 맞다.
- 커밋: `99388d6158`
- upstream 근거: `6c40826d14`(9파일 +217/−5, 부모가 #9575 `c06999df7d`). deps #9460(seq 88, `d81b37f15b`) 적용됨.
- 수정 파일(7): `include/constants/config_changes.h`, `src/battle_ai_switch.c`, `src/battle_main.c`, `test/battle/ai/ai_choice.c`, `test/battle/ai/ai_double_ace.c`, `test/battle/ai/ai_switching.c`, `test/battle/move_effect/first_turn_only.c`
- 적용 방법: 사전 분석 patch(`seq131-9462.patch`)를 `git apply`했다. 충돌 없음.
- 이미 있던 것(제외한 hunk):
  - `include/config/ai.h`의 `AI_REVERSE_BATTLER_LOGIC_ORDER_CHANCE 50`(같은 값·자리, HnS 주석 유지)
  - `include/random.h`의 `RNG_AI_REVERSE_BATTLER_LOGIC_ORDER`(upstream과 같은 자리라 enum 값 같음)
  - `battle_main.c`의 판단 순서 뒤집기 로직(HnS에 1.17.0형 `gAiLogicData->reverseBattlerLogicOrder`로 이미 있음). upstream의 지역 변수·`battlerIndex` 루프는 넣지 않았다.
- 내용(잔여분):
  - `battle_main.c` 뒤집기 확률과 `battle_ai_switch.c` `GetSwitchChance`의 `SHOULD_SWITCH_ALL_MOVES_BAD_PERCENTAGE`를 `GetConfig(...)`로 읽는다.
  - `AI_CONFIG_DEFINITIONS`에 `AI_REVERSE_BATTLER_LOGIC_ORDER_CHANCE`(`reverseBattlerLogicChance`, `(u32, 100)`)·`SHOULD_SWITCH_ALL_MOVES_BAD_PERCENTAGE`(`switchAllBadMovesChance`, `(u32, 100)`)를 `AI_ROLL_ATTACKING` 앞에 넣었다(1.17.0과 같은 열 맞춤). 없으면 `WITH_CONFIG(AI_REVERSE_…)` 테스트가 컴파일되지 않는다.
  - 테스트: 기존 더블 AI 테스트에 `WITH_CONFIG(AI_REVERSE_BATTLER_LOGIC_ORDER_CHANCE, 0)`, `(reversed)` 변형과 "AI can switch out both mons in either order" 추가.
- HnS 적응:
  - `ai_choice.c`의 새 `(reversed)` 테스트 `defendingAbility = SPECIES_ZIGZAGOON` → **`ABILITY_GLUTTONY`**. upstream이 원래 테스트의 버그를 복사한 것이고, 원래 테스트는 #9719 `4a7bebe2b2`가 고쳐 HnS에 이미 있다(1.17.0도 둘 다 `ABILITY_GLUTTONY`). HnS는 #9507(seq 91)로 종이 `enum Species`라 upstream 값 그대로면 `-Werror=enum-conversion`으로 테스트 빌드가 깨진다.
  - `ai_switching.c` 마지막 hunk: HnS 파일 끝이 upstream과 달라(#9124 이식 순서 차이) 새 테스트 2개를 파일 끝에 그대로 붙였다(1.17.0에서도 "HP changes on switchin" 뒤쪽).
  - `first_turn_only.c`: #9655 이전형 파일(`ABILITY_POPUP` 줄 있음)이지만 #9462 hunk는 48행 뒤라 겹치지 않는다. 나중에 #9655의 이 파일 hunk도 그대로 들어간다(사전 분석 `git apply --check` 확인).
- 검증:
  - `git diff --check` 통과. 한글 줄 변경 0. config 기본값 변경 0(HnS 50/100 유지). 비테스트 빌드의 `GetConfig`는 `sConfigChanges` 값(50, 100)을 돌려준다(7비트 필드). 게임 동작 불변.
  - 빌드(`build/port.log`): 종료 코드 0, **ROM 32,722,692 B(97.52%, +32 B) / EWRAM 248,944 B(0) / IWRAM 25,516 B(0)**. 사전 분석 추정 약 +66 B(정렬 전). `pokehns.gba` SHA1 `2dd0353e224599d9a04f8f4bbc0565b8776e4137`. 새 경고 0(배틀 헤더 변경으로 경고 21줄이 나왔지만 모두 기준 목록의 기존 경고).
- 테스트(지정 4파일: `test/battle/ai/ai_choice.c`, `test/battle/ai/ai_double_ace.c`, `test/battle/ai/ai_switching.c`, `test/battle/move_effect/first_turn_only.c`) → 160줄(PASS 137). 이름이 seq 126 기준에 있는 줄은 아래를 빼고 **모두 같다.** 사라진 PASS 0.
  - **FAIL → PASS 3건**(STATUS의 "AI 더블 테스트 3건"): `Choiced Pokémon won't switch out if they can still affect one opposing Pokémon in doubles`(기준 `… 1/2 (1/?): FAIL`), `AI_FLAG_DOUBLE_ACE_POKEMON: Ace mons won't be switched in even if they are the best candidates`(기준 FAIL), `AI can switch out both mons on the same turn in double battles`(기준 `… (1/?): FAIL`). `WITH_CONFIG(…, 0)`으로 판단 순서가 고정된 결과다. 실패 꼬리가 사라져 이름 줄이 바뀐다.
  - 새 줄 PASS 5: `Choiced … doubles (reversed)`, `AI_FLAG_DOUBLE_ACE_POKEMON: … (reversed)`, `AI can switch out both mons on the same turn in double battles (reversed)`, `AI can switch out both mons in either order`, `AI will Fake Out either opponent if one has a slower Fake Out (reversed)`.
  - 새 줄 FAIL 3: `AI will not try to switch for the same Pokémon for 2 spots in a double battle (all bad moves, reversed) 1/2 (1/?)`, `… (Wonder Guard, reversed) (1/?)`, `AI will not try to switch for the same pokemon for 2 spots in a 2v1 battle (all bad moves, reversed) 1/2`. 사유는 `Unmatched MESSAGE`(영문 `withdrew …`/`sent out …` 기대값)와 그로 인한 `PASSES_RANDOMLY`의 `observed 0.0`이다. reversed가 아닌 원래 3개도 기준에서 같은 사유로 FAIL이다(알려진 한계).
- 남은 위험: 없음(게임 동작 불변).
- 실기 확인: 불필요.

## 동기화 단위: seq 133 #9006 `U-relearner-9006` Move relearner refactor

- 현재 판정: 적용(HnS 적응)
- 커밋: `fc205e1a40`
- upstream 근거: `780805f169`(19파일). `git log --all --grep='#9006'` 결과 0건, HEAD에 `Special_HasMoveToRelearn`·`HandleMoveRelearnerInput` 없음 → 미적용이었다.
- 수정 파일(19): `asm/macros/event.inc`, `data/maps/BlackthornCity_House3_hns/scripts.inc`, `data/maps/FallarborTown_MoveRelearnersHouse/scripts.inc`, `data/scripts/move_relearner.inc`, `data/specials.inc`, `include/chooseboxmon.h`, `include/constants/move_relearner.h`, `include/menu_specialized.h`, `include/move_relearner.h`, `include/strings.h`, `src/chooseboxmon.c`, `src/field_specials.c`, `src/list_menu.c`, `src/menu_specialized.c`, `src/move_relearner.c`, `src/party_menu.c`, `src/pokemon_summary_screen.c`, `src/scrcmd.c`, `src/strings.c`
- 적용 방법: 사전 분석 patch(`seq133-9006.patch`, 19파일 +508/−959, 76 hunk)를 `git apply`했다. 충돌 없음. `src/move_relearner.c`, `data/scripts/move_relearner.inc`, `include/move_relearner.h`, `include/constants/move_relearner.h`는 적용 뒤 upstream `780805f169`와 **바이트 동일**하다(HnS가 relearner 본체를 고친 적이 없어 이식 전에도 upstream 부모와 같았다).
- 내용(upstream):
  - relearner 자체 상태 기계(`MENU_STATE_*`)를 없애고 기술 가르침 공용 `LearnMove()`(`chooseboxmon.c`)를 쓴다. `LearnMove`에 확인 단계 `PROMPT_BEFORE_LEARNING_1/2`와 `recoverPP`(data[3]) 추가, `CanMonLearnMove(boxmon, move)`·`CanMonLearnSpecialVarMove`.
  - 결과를 `VAR_0x8004` 대신 `gSpecialVar_Result`로 돌려준다. special `HasMovesToRelearn` → `Special_HasMoveToRelearn`(같은 자리, special 번호 불변).
  - 요약 화면 relearner 입력을 `HandleMoveRelearnerInput`으로 분리, `UpdateMoveRelearnerState`/`UpdateRelearnPrompt`. `getmoverelearnerstate`·`istmrelearneractive`(`callnative` 매크로, 스크립트 명령 번호 무관)와 C 함수 삭제. `RELEARN_MODE_BOX_PSS_PAGE_*` 삭제.
  - `RedrawListMenu`가 커서 콜백을 부른다(`list_menu.c`). `InitMoveRelearnerWindows(bool32)`.
  - 떠올리기 화면의 배우기·잊기 메시지가 relearner 전용 문자열에서 공용 문자열(`gText_PkmnNeedsToReplaceMove`, `gText_WhichMoveToForget`, `gText_StopLearningMove2`, `gText_12PoofForgotMove`, `gText_PkmnLearnedMove4`, `gText_MoveNotLearned`)로 바뀐다. 모두 HnS에서 이미 한글이다. 176px 창(`RELEARNERWIN_MSG`)에 가장 넓은 기술명·닉네임 6글자로도 최대 155px라 들어간다(사전 분석 `tmp-133/width.py`).
- HnS 적응:
  - `chooseboxmon.c`: HnS 고유 코드(너즐록 `IsBoxMonExcluded`, `SELECT_PC_MON_PLA_TUTOR`/`CanMonLearnPLAMove`/`sPLATutorLearnsets`, `IsMatchingSpecies`의 `SPECIES_NONE`) 보존. `VALIDATE_BEFORE_LEARNING`은 upstream `switch (CanMonLearnMove(boxmon, move))` 대신 HnS `switch (IsBoxMonExcluded(boxmon))` 유지(이 상태는 가르침에서만 쓰고 relearner는 `PROMPT_BEFORE_LEARNING_1`에서 시작).
  - `pokemon_summary_screen.c`: HnS 능력치 페이지 START/R/L(능력치/개체값/노력치 전환) 유지. `HandleMoveRelearnerInput` 호출 분기는 HnS START 분기 **앞**에 `ShouldShowMoveRelearner() && IS_MOVE_PAGE(...)` 조건으로 넣었다(`// HnS:` 주석). `P_SUMMARY_SCREEN_MOVE_RELEARNER FALSE`라 분기 전체가 컴파일에서 빠진다(`pokehns.map`에 `HandleMoveRelearnerInput` 0건). `ChangePage`의 무조건 `ShowUtilityPrompt(SUMMARY_MODE_NORMAL)` 유지.
  - `data/maps/BlackthornCity_House3_hns/scripts.inc`(HnS 전용, 실사용 relearner 경로): `special Special_HasMoveToRelearn`, `goto_if_eq VAR_RESULT, FALSE, …ChooseMon`(이전 `VAR_0x8004, 0`). #9006 뒤 `VAR_0x8004`는 선택 파티 번호/`PC_MON_CHOSEN`으로 남으므로 고치지 않으면 하트비늘 판정이 틀린다. Fallarbor 쪽 같은 수정(#9774)은 seq 156.
  - **`PROMPT_BEFORE_LEARNING_2` "아니오"의 `gSpecialVar_Result = FALSE;`(#10223 `42e299ed0f` 1줄 선반영, 주석 없이 upstream과 같은 줄).** #9006만 넣으면 확인에 "아니오"를 골라도 직전 `Special_HasMoveToRelearn`의 `VAR_RESULT = TRUE`가 남아, 검은먹시티 NPC가 기술을 배운 것으로 보고 **하트비늘을 가져간다**(upstream 1.16.x 버그, 1.17.0에서 #10223으로 고쳐짐). 커밋 메시지에도 적었다.
  - `field_specials.c` `CanTeachMoveBoxMon`의 `tRecoverPp = TRUE`(upstream)와 `data[3]` 정의 추가.
- **upstream과 다르게 둔 곳:**
  - **`field_specials.c` `Task_ReturnToFieldWhileLearningMove`의 `tRecoverPp = TRUE`(`// HnS:`)**: 가르침에서 기술을 교체하면 새 기술이 최대 PP(현재 HnS 동작 유지). upstream은 `CanTeachMoveBoxMon`에서만 TRUE로 두는데, 교체는 항상 요약 화면을 거치고 돌아올 때 `FieldCB_ContinueLearningMove`가 새 task(data 0)를 만들어 `recoverPP`가 0이 된다. 그러면 PP가 min(잊은 기술의 남은 PP, 새 기술 최대 PP)가 된다(upstream 1.17.0·1.17.1 버그). 영향 경로: 필드 기술 가르침(`data/scripts/move_tutors.inc`, `UlaulaIsle_hns`, `BattleFrontier_Lounge7_hns`, `NewSinjoh_HotSprings_hns` PLA). upstream 1.17.0과 똑같이 하려면 이 한 줄을 빼면 된다.
  - `VALIDATE_BEFORE_LEARNING`의 `IsBoxMonExcluded`, 요약 화면 START/R/L과 relearner 호출 분기 위치(위 "HnS 적응").
- 제외한 hunk:
  - `pokemon_summary_screen.c` `Task_HandleInput`의 A 버튼 hunk(`ShouldShowRename` 이름 바꾸기 분기): HnS A 버튼은 다시 작성됐고 이름 바꾸기 분기가 없다(`P_SUMMARY_SCREEN_RENAME FALSE`).
  - 같은 파일 `ClearPageWindowTilemaps` 능력치 페이지 IV/EV 프롬프트 hunk: HnS에 해당 줄·`ShouldShowIvEvPrompt`가 없다.
- 문자열: 새 문자열 2개는 한글 초안으로 넣었다(아래 "한글 문구 미결"). 기존 한글 줄 변경·삭제 0. 더 이상 참조되지 않는 relearner 전용 한글 6개(`gText_MoveRelearnerAndPoof` 등)는 upstream처럼 정의를 남겼다(gc-sections가 ROM에서 뺀다). 요약 화면 영문 프롬프트 `sRelearnTexts`는 숨은 창에만 그려지고 tilemap은 `ShouldShowMoveRelearner()`(HnS FALSE)일 때만 올라가 화면에 나오지 않는다.
- 세이브: 영향 없음. `gMoveRelearnerState`·`gRelearnMode`는 세이브 밖 EWRAM이고 special·스크립트 명령 번호는 그대로다.
- 검증:
  - `git diff --check` 통과. 비 ASCII `+`/`−` 줄은 새 한글 문자열 2줄뿐. `HasMovesToRelearn`·`getmoverelearnerstate`·`istmrelearneractive` 남은 곳 0(`MoveRelearnerRunTextPrinters` 선언은 upstream `780805f169`에도 남아 있음).
  - 빌드(`build/port.log`): 종료 코드 0, **ROM 32,722,052 B(97.52%, −640 B) / EWRAM 248,940 B(94.96%, −4 B) / IWRAM 25,516 B(0)**. 사전 분석 추정 약 −0.8 KB, EWRAM −4 B(`sMoveRelearnerMenuState` 6 B → `sMoveRelearnerScrollState` 4 B). 스크립트의 special 이름과 `data/specials.inc` 일치는 링크 성공으로 확인했다. `pokehns.gba` SHA1 `bcb11875cef2d6048c0098af0fc5756dbe7ff99e`. 새 경고 0(경고 151줄 모두 기준 목록의 기존 경고).
- 테스트(지정 2파일: `test/text.c`, `test/pokemon.c`) → 63줄(PASS 37)이 seq 126 기준 목록의 같은 줄과 **모두 같다**. `Move names fit on Move Relearner Screen`은 FAIL(94/94) 유지(한글 이름 폭, 기존). `test/pokemon.c`의 `Pokémon level up learnsets fit … 1436/1573: INVALID`(disabled species)는 표준 목록 밖이고 seq 126 전체 실행(`build/port-check-post126.log`)에도 같게 있다. upstream #9006은 `test/**`를 바꾸지 않는다.
- 남은 위험:
  - 중간: 떠올리기 흐름 변화(upstream 설계). "배우게 하겠습니까?" 확인이 `LearnMove`로 옮겨졌고, 교체 때 팡파레가 없으며, 배운 직후 팡파레가 울리는 동안 화면이 닫힌다. 스크립트 모드는 `tRecoverPp = TRUE`라 교체 기술 최대 PP로 이전과 같다.
  - 낮음: `RedrawListMenu`가 커서 콜백을 한 번 더 부른다. HnS 사용처(옵션·챌린지 메뉴, 커트 볼 상점)의 콜백은 멱등이다.
  - 낮음: 요약 화면을 열 때마다 숨은 창 버퍼(BG0 타일 800~821)에 영문 프롬프트를 그린다(tilemap은 올리지 않음).
- 실기 확인: 필요(아래 "실기 확인 항목" 2).

## 동기화 단위: seq 134 #9903 `U-relearner-9006` Fix move relearner from summary screen from pc

- 현재 판정: 부분 적용(HnS 적응)
- 커밋: `cf71e21e56`
- upstream 근거: `83c6b89760`(`src/pokemon_summary_screen.c` +2, 2 hunk). deps #9006(seq 133) 적용 뒤.
- 수정 파일(1): `src/pokemon_summary_screen.c`
- 적용 방법: 사전 분석 patch(`seq134-9903.patch`, 1 hunk +1)를 `git apply`했다. 충돌 없음.
- 내용: 요약 화면에서 PC 박스 포켓몬으로 relearner에 들어갈 때 `gSpecialVar_MonBoxId = StorageGetCurrentBox();`를 넣는다(다른 박스의 포켓몬을 가리키던 버그). upstream hunk 2(START → relearner, #9006 이전 구조)를 #9006이 만든 `HandleMoveRelearnerInput`의 `isBoxMon` 블록, `gSpecialVar_MonBoxPos = …` 다음 줄로 옮겼다. 1.17.0 최종 코드와 같은 위치다(upstream은 master→upcoming 병합 `19bfea3974`에서 이 줄이 빠졌다가 #10445가 다시 넣음).
- 제외한 hunk: hunk 1(A 버튼 → 정보 페이지 → `ShouldShowRename()` 분기). HnS `Task_HandleInput` A 버튼에는 이름 바꾸기 분기가 없다(`P_SUMMARY_SCREEN_RENAME FALSE`). 넣을 곳이 없다.
- 검증:
  - `git diff --check` 통과. 한글·문자열·세이브 변경 0.
  - 빌드(`build/port.log`): 종료 코드 0, **ROM 32,722,052 B(0) / EWRAM 248,940 B(0) / IWRAM 25,516 B(0)**. `pokehns.gba` SHA1 `c737f75c5d82585baee94dc48c373a1124c0c7a5`. 새 경고 0.
  - SHA1이 seq 133과 다른 이유: 요약 화면 파일에 한 줄이 늘어 이 파일의 assert/`errorf` 위치 문자열(`src/pokemon_summary_screen.c:NNNN`) 13개의 줄 번호가 1씩 바뀌었다. 커밋 전후 파일을 같은 옵션으로 어셈블리까지 컴파일해 비교하니 차이는 이 위치 문자열뿐이었다(코드 동일). `HandleMoveRelearnerInput`은 `ShouldShowMoveRelearner()`(`P_SUMMARY_SCREEN_MOVE_RELEARNER FALSE`) 분기에서만 불려 컴파일에서 빠진다(ELF 심볼 0건).
- 테스트: 지정 없음(upstream 테스트 변경 없음, config로 꺼진 경로라 자동 테스트 대상 없음). 전체 테스트는 메인이 구간 끝에 돌린다.
- 남은 위험: 없음(HnS config에서 닿지 않는 경로).
- 실기 확인: 불필요.

## 동기화 단위: seq 137 #9713 `U-9713` Support non-contiguous SE/MUS IDs in debug menu

- 현재 판정: 적용(HnS 적응, **메인 결정 B안**)
- 커밋: `d057cee5c2`
- upstream 근거: `48a165c403`(`include/constants/songs.h`, `src/debug.c`). `git log --grep='#9713'` 없음, `FindSong`/`sSongNames` 없음 → 미적용이었다. deps 없음. 후행 #9927(seq 351)이 이 PR을 deps로 둔다.
- 수정 파일(2): `include/constants/songs.h`, `src/debug.c`
- 적용 방법: 사전 분석의 B안 patch(`tmp-137/seq137-9713-optB-no-names.patch`)를 `git apply`했다. 충돌 없음. A안 patch(`seq137-9713.patch`)와는 이름 표 생성부(D7)만 다르다.
- 내용(upstream):
  - `songs.h`의 `END_SE`·`START_MUS`·`END_MUS` 삭제.
  - `debug.c`: `enum SongType`/`enum FindSongMode`, 비정적 `FindSong()`, 위·아래 입력에 `sPowersOfTen[tDigit]`번 `FindSong`을 부르는 `Debug_HandleInput_SongId()`. `sBGMNames`/`sSENames` 두 표를 곡 ID로 바로 인덱싱하는 `sSongNames[]` 하나로 합치고, 사운드 메뉴 시작값을 `FindSong(…, SONG_FIRST_GE, MUS_DUMMY)`로 정한 뒤 초기 표시를 task 설정 뒤로 옮김. `FindSong`은 이름 문자열 접두어(`SE_`/`MUS_`)로 SE와 음악을 가린다.
- **B안(메인 결정):** 곡 이름을 저장하지 않는다. `sSongNames[songId]`가 곡 종류별 접두어 문자열 `sSongNamePrefix_SE`(`_("SE_")`)/`sSongNamePrefix_MUS`(`_("MUS_")`)를 가리킨다(`// HnS:` 주석). `FindSong`은 upstream 코드 그대로 동작하고, 화면의 이름 자리에는 `SE_`/`MUS_`만 나온다. 근거: Korean patch(`361f1e4a77`)가 곡 이름을 비운 현재 HnS 화면 유지, ROM 약 −1.1 KB(A안은 약 +12.5 KB). 이후 upstream(#9927 등)이 기대하는 `sSongNames`/`FindSong` 이름·구조는 A안과 같다.
  - **대안 A안(이름 복원, `seq137-9713.patch`):** upstream대로 `[songId] = COMPOUND_STRING(#songId)`. 화면에 `MUS_HG_NEW_BARK` 같은 상수 이름(영문, HnS charmap에서 `_`는 밑줄 기호 `F9 09`)이 나오고 ROM 약 +12.5 KB. 디버그 메뉴 문구는 원래 전부 영문이라 번역 문제는 없다. 바꾸려면 `src/debug.c`의 이름 표 생성부만 A안 patch의 D7로 바꾼다.
  - 두 안 모두 사운드 메뉴 EWRAM 덮어쓰기 잠재 버그를 없앤다: 이식 전 HnS 이름은 `{0}`(1바이트 `0x00`, EOS `0xFF` 없음)이라 `StringCopyPadded(gStringVar1, name, CHAR_SPACE, 35)`가 다음 `0xFF`까지(첫 SE 이름부터 5,410 B, 첫 BGM 이름부터 2,798 B) 0x100 B짜리 `gStringVar1` 뒤(`gTextFlags`·`gFonts`)를 덮어썼다(사전 분석의 코드·ROM 분석, 실기 미확인).
- HnS 적응(A·B 공통):
  - HnS GBS 사운드 테스트 변경(`f9afc19dd1`: `Debug_Sound_Redraw_SE/MUS`, SELECT로 GBS 전환 `Debug_Sound_ToggleGBS`, `m4aSongNumStart/Stop`의 GBS 인자, `sDebugText_Sound_*_Gbs` 문구) 유지. 초기 표시는 GBS 문구 선택을 유지한 채 task 설정 뒤로 옮겼고, 이름 조회는 HnS `Debug_Sound_Redraw_*`에서 `sSongNames[tInput]`으로 바꿨다.
  - `songs.h`: HnS는 `FIRST_PHONEME_SONG (END_MUS + 1)`이 `END_MUS`를 쓰므로 `(DP_MUSIC_END + 1)`로 바꿨다(`END_MUS`가 `DP_MUSIC_END`로 정의돼 있어 값 746 같음). PH_*와 `SE_FASTER_JOY_HEAL`(797) 번호 불변.
  - `SOUND_LIST_BGM` 끝에 DP 음악 11곡(`MUS_DP_AZURE_FLUTE`~`MUS_DP_STARK_MOUNTAIN`, 735~745), `SOUND_LIST_SE` 끝에 `SE_FASTER_JOY_HEAL`(797)을 넣었다. 이식 전에는 숫자 입력으로 735~745에 갈 수 있었으므로 도달 범위를 유지하고, 797은 이번에 처음 디버그에서 들을 수 있다. 매크로 안에는 주석을 넣지 않았다(줄 이음 문제), 설명은 커밋 메시지에 적었다.
- 제외한 hunk: 없음(upstream `seName` 지역 변수 재배치는 HnS 구조에 해당 없음).
- 검증:
  - `git diff --check` 통과. 한글 줄 변경 0(`src/debug.c`에 한글 없음). `END_SE`·`START_MUS`·`END_MUS` 남은 곳 0(docs 제외).
  - 빌드(`build/port.log`): 종료 코드 0, **ROM 32,720,756 B(97.51%, −1,296 B) / EWRAM 248,940 B(0) / IWRAM 25,516 B(0)**. 사전 분석 추정 B안 약 −1,108 B(정렬 전). `pokehns.gba` SHA1 `a9db6b6c32941fd8581bb571c9728ebd4ca6522e`. 새 경고 0(`songs.h` 변경으로 경고 154줄이 나왔지만 모두 기준 목록의 기존 경고).
  - `arm-none-eabi-nm -S`: `sSongNames` 크기 `0xc78`(798칸), `FindSong` 있음. ROM의 `sSongNames`를 읽으면 SE 접두어 270칸(1~269 연속 + 797), MUS 접두어 396칸(350~745 연속), 나머지는 NULL이다. 따라서 SE/음악 선택 순서는 이식 전과 같고, SE 269 다음이 797이다. PH_*(746~796)는 지금처럼 메뉴에서 빠진다.
- 테스트: 없음(이 PR은 `test/**`를 바꾸지 않고, 관련 테스트도 없다). 빌드 검증만 했다.
- 남은 위험:
  - 낮음(디버그 전용): 이름 자리에 곡 이름 대신 `SE_`/`MUS_`만 나온다(이식 전에는 빈 칸). 곡은 번호로 구분한다.
  - 낮음: 자릿수 1000 단위에서 위/아래 한 번에 `FindSong` 최대 1,000번(upstream과 같음).
- 실기 확인: 필요(아래 "실기 확인 항목" 3, 디버그 메뉴).

## 동기화 단위: seq 143 #9721 `U-9721` Learnset Helper: Remove unnecessary order-only dependencies

- 현재 판정: 적용(그대로)
- 커밋: `f3893a4cb4`
- upstream 근거: `1d854c4cb4`(`Makefile` 1줄). `git log --grep='#9721'` 없음, `Makefile`에 order-only 의존이 그대로 있었다.
- 수정 파일(1): `Makefile`
- 적용 방법: 사전 분석 patch(`seq143-9721.patch`)를 `git apply`했다(오프셋 +32, 문맥 동일). 충돌 없음. HnS `-ffunction-sections` CFLAGS 줄(`7e7c38ab10`)과 떨어진 위치다.
- 내용: `$(ALL_LEARNABLES_JSON):  | $(wildcard $(LEARNSET_HELPERS_DATA_DIR)/*.json)` → `$(ALL_LEARNABLES_JSON):`. 규칙 본문(`make_learnables.py`)은 그대로. `src/data/pokemon/all_learnables.json`은 git 추적 파일이라 이전·이후 모두 재생성하지 않고, 파일이 없을 때는 둘 다 같은 명령을 실행한다(사전 분석 장난감 Makefile 확인).
- HnS 적응: 없음. `make hns`도 같은 규칙을 쓴다(`BUILD=hns` 분기 없음).
- 제외한 hunk: 없음.
- 검증:
  - `git diff --check` 통과.
  - 빌드(`build/port.log`): 종료 코드 0, **ROM 32,720,756 B(0) / EWRAM 248,940 B(0) / IWRAM 25,516 B(0)**. 재링크만 일어났고 `make_learnables.py` 실행 줄 0, 경고 0줄(새 경고 0).
  - **`pokehns.gba` SHA1 `a9db6b6c32941fd8581bb571c9728ebd4ca6522e` — seq 137 빌드(`d057cee5c2`, 스크래치 사본 `apply/pokehns-137.gba`)와 같다(메인 결정대로 확인).**
- 테스트: 없음(테스트 추가·변경 없음).
- 남은 위험: 없음.
- 실기 확인: 불필요(ROM 바이트 동일).

## 동기화 단위: seq 148 #9690 `U-9690` fix: Modernize the conversion formula for imperial weights

- 현재 판정: 적용(한 줄 문맥 수동)
- 커밋: `aeab20beac`
- upstream 근거: `0cd398953c`(2파일 각 1줄). `git log --grep='#9690'` 없음, 옛 값·옛 식 그대로였다.
- 수정 파일(2): `include/constants/pokedex.h`, `src/pokedex.c`
- 적용 방법: 사전 분석 patch(`seq148-9690.patch`)를 `git apply`했다. 충돌 없음. `pokedex.h` hunk는 upstream 문맥 `REGIONAL_DEX_COUNT (IS_FRLG ? …)`가 HnS에서 `(IS_HNS ? JOHTO_DEX_COUNT : IS_FRLG ? …)`라 사전 분석이 문맥만 HnS에 맞췄다. 바뀐 줄은 upstream과 같다.
- 내용: `DECAGRAMS_IN_POUND` 4536 → 453592, `ConvertMonWeightToImperialString`의 `lbs = (weight * 100000) / DECAGRAMS_IN_POUND` → `lbs = (u32)(((u64)weight * 10000000) / DECAGRAMS_IN_POUND)`(두 줄을 함께 바꿈. 사용처는 이 한 곳).
- HnS 적응·화면 영향:
  - HnS 도감은 config `UNITS`가 아니라 플레이어 옵션 `gSaveBlock3Ptr->challengeSettings.unitSystem`(옵션 "단위계", 새 게임 기본 0 = 미터법)을 쓴다. `ConvertMonWeightToString`이 `unitSystem == 1`일 때만 이 함수를 부른다(기본 도감·HGSS 도감 공통).
  - **기본(미터법) 화면은 변화 없음.** 야드파운드법에서는 HnS 종 무게 547종류 중 22개가 **+0.1 lb** 달라진다(사전 분석 계산. 예: 32.5 kg 폴리곤2·다꼬리 71.6 → 71.7, 레쿠쟈 455.2 → 455.3, 펄기아 740.7 → 740.8, 디아루가 1505.7 → 1505.8). 표시 형식·바이트 길이·한글 문자열은 그대로다.
  - 나무열매 태그(자체 인치 계산)와 크기 기록(키만 사용)은 영향 없음.
- 제외한 hunk: 없음.
- 검증:
  - `git diff --check` 통과. 한글 줄·세이브 변경 0.
  - 빌드(`build/port.log`): 종료 코드 0, **ROM 32,720,772 B(97.52%, +16 B) / EWRAM 248,940 B(0) / IWRAM 25,516 B(0)**. 사전 분석 추정 +16 B. `pokehns.gba` SHA1 `1507a194434b846323ebe18ec1ff36ffbbb66843`. 새 경고 0(`pokedex.h` 변경으로 경고 163줄, 모두 기준 목록의 기존 경고).
  - 64비트 나눗셈 `__udivdi3`는 이미 ROM에 있던 libgcc 함수를 쓴다(ELF 심볼 1개, 새 libgcc 코드 없음). 오버플로: 새 식은 u64 캐스트로 `9999 × 10^7`까지 안전하다.
- 테스트: 없음(테스트 추가·변경 없음, `test/`에 `ConvertMonWeight` 사용 0건). 빌드 검증만.
- 남은 위험: 낮음(야드파운드법 표시 +0.1 lb, upstream 1.17.0과 같음).
- 실기 확인: 필요(아래 "실기 확인 항목" 4).

## 동기화 단위: seq 149 #9461 `U-mapheader-9461` Show floor number in map popup

- 현재 판정: 적용(HnS 적응)
- 커밋: `9f6c5b5c58`
- upstream 근거: `85128bdeb5`(6파일 +103/−14). `git log --all --grep='#9461'` 없음, `GetPopUpMapName`·`floorNumber`·`FLOOR_ROOFTOP` 없음 → 미적용이었다. 이 unit의 첫 행이다.
- 수정 파일(6): `include/constants/map_types.h`, `include/global.fieldmap.h`, `include/map_name_popup.h`, `src/map_name_popup.c`, `test/text.c`, `tools/mapjson/mapjson.cpp`
- 적용 방법: 사전 분석 patch(`seq149-9461.patch`)를 `git apply`했다. 충돌 없음. `include/global.fieldmap.h`의 `MapHeader`와 `tools/mapjson/mapjson.cpp`는 upstream과 같은 모양이 됐다(뒤 행 #7975·#9080·#10167·#10176·#10159의 전제 유지).
- 내용(upstream):
  - `struct MapHeader`의 `filler_18[2]` → `s8 floorNumber` + `u8 filler_19`(크기 0x1C·오프셋 불변). `mapjson`이 모든 버전에서 `floor_number`를 `.byte`로 출력(없으면 0).
  - `map_name_popup.c`: `MapNamePopupAppendFloorNum`(" B1F"/" 1F"/" " + `gText_Rooftop`), `IsCeladonDeptStore`, 공개 `GetPopUpMapName`. `FLOOR_ROOFTOP 127`, `MAP_POPUP_STRING_BUFFER_LENGTH 27`, `MAP_POPUP_PREFIX_BUFFER_LENGTH 6`.
  - 새 테스트 `test/text.c` "Map names fit in popup"(`FONT_NARROWER`, 80px, `showMapName` 맵 전부).
- HnS 적응(메인 결정대로 3곳):
  - **피라미드 조건 유지:** `ShowMapNamePopUpWindow`의 HnS 조건(`LAYOUT_BATTLE_FRONTIER_BATTLE_PYRAMID_TOP || …_PYRAMID_TOP_HNS`)을 그대로 두고, `&(mapDisplayHeader[6])` 3곳 → `[MAP_POPUP_PREFIX_BUFFER_LENGTH]`, `GetMapName(…)` → `GetPopUpMapName(withoutPrefixPtr, &gMapHeader)`.
  - **`FONT_NARROW` 유지(upstream `GetFontIdToFit(…, FONT_NORMAL, -1, 80)` hunk 제외, `// HnS:` 주석 2줄):** HnS 한글 글리프는 `FONT_NORMAL`·`FONT_NARROW` 8px, `FONT_NARROWER` 11px라 "좁아지는" 대체가 한글에서 오히려 넓어진다. upstream대로면 모든 팝업이 `FONT_NORMAL`이 되어 숫자·라틴 글자 폭이 바뀐다(예: "29번 도로" 37 → 39px). g6 plan 6절 결정.
  - **`CELADON DEPT.` 특례 `!IS_HNS`(g6 plan 7절):** `GetPopUpMapName`에서 `if (!IS_HNS && IsCeladonDeptStore(mapHeader))`(`// HnS:` 주석). HnS 백화점 맵은 `*_HNS` 레이아웃·`show_map_name` FALSE라 원래도 닿지 않지만 영문 이름을 HnS 빌드에서 뺐다. `#if`로 막으면 미사용 static 함수 경고가 나서 `if` 조건으로 처리했다. 빌드 결과 ROM에 `CELADON DEPT` 0건, `IsCeladonDeptStore`·`MapNamePopupAppendFloorNum` 심볼 없음(`GetPopUpMapName`에 인라인·제거).
- 제외한 hunk: `map_name_popup.c`의 글꼴 hunk(위).
- 화면·데이터 영향:
  - `make hns`에 들어가는 맵 560개(전부 `*_hns`) 중 `floor_number`가 있는 맵은 0개다. `floorNumber`가 모두 0이라 `GetPopUpMapName` 결과는 이식 전 `GetMapName`과 같다. **팝업 문구 변화 없음.**
  - 층 표기 함수는 upstream 그대로(영문 `B`/`F` 조합, `gText_Rooftop`은 HnS에서 이미 `"옥상"`). 지금은 닿지 않는다.
  - 맵 헤더: 생성 파일 `header.inc`는 빌드 규칙이 `$(MAPJSON)`에 의존하지 않아 이번 빌드에서 재생성되지 않았다. 새로 빌드된 `tools/mapjson/mapjson`으로 HnS 맵 560개 헤더를 스크래치(`apply/mj149/`)에 다시 만들어, 기존 `.2byte 0`을 `.byte 0`×2로 바꿔 비교하니 **560개 모두 같다**. 즉 새 도구로 다시 만들어도 맵 헤더 ROM 바이트는 같다. 커밋할 생성 파일 없음.
  - 세이브: `MapHeader`는 ROM 데이터, `gMapHeader` 크기 0x1C 불변. SaveBlock 영향 없음.
- 검증:
  - `git diff --check` 통과. 한글이 든 소스 줄 변경 0(주석 영문).
  - 빌드(`build/port.log`): 종료 코드 0, **ROM 32,720,900 B(97.52%, +128 B) / EWRAM 248,940 B(0) / IWRAM 25,516 B(0)**. 사전 분석 추정 약 +124 B(`GetPopUpMapName` 0x7c). `pokehns.gba` SHA1 `bd3dc3e5e8d08d1576e1b67fcb5b14fdf8fea43f`. 새 경고 0(`global.fieldmap.h` 변경으로 경고 163줄, 모두 기준 목록의 기존 경고).
- 테스트(`test/text.c`) → 38줄(PASS 12). 기존 37줄은 seq 126 기준과 **모두 같고**, 새 줄 **`Map names fit in popup: PASS`**(`showMapName` 맵 139개, 최장 "사파리존 게이트" `FONT_NARROWER` 80px = 한도 80px).
- 남은 위험:
  - **floor_number 도입 시 층 표기 한글화:** 나중에 HnS 맵에 `floor_number`를 넣으면 upstream 영문 `B1F`/`1F` 형식이 팝업에 나온다. HnS 기존 층 표기(`gText_1F` "1층", `gText_B1F` "지하1층", `gText_Rooftop` "옥상")와 맞추려면 `MapNamePopupAppendFloorNum`에 HnS 분기가 필요하다(사전 분석 4-1 B안 코드). 한글 안 최장 "블루시티동굴 지하1층"은 20바이트·`FONT_NARROW` 80px로 표시 한도에 딱 맞는다. 별도 결정.
  - **`FONT_NARROWER` 80px 테스트 경계:** 새 테스트는 한글 11px 글꼴로 재므로 현재 최장 이름 "사파리존 게이트"가 **정확히 80px**다. 팝업 이름을 한 글자라도 늘리면(또는 floor_number로 층을 붙이면: 한글 안 9개 맵, 영문 안도 "블루시티동굴 B1F/B2F" 81px) 테스트가 실패한다. 실제 팝업은 `FONT_NARROW`(한글 8px)로 그려 여유가 있다.
  - 낮음: `test/text.c` 새 테스트의 `s8 mapGroup/mapNum`은 HnS 그룹 106개·그룹당 최대 123맵이라 범위 안이다(127 초과 시 오버플로).
- 실기 확인: 필요(아래 "실기 확인 항목" 5).

## 동기화 단위: seq 151 #9755 `U-9755` Remove reundant weather check in IsDamageMoveUnusable

- 현재 판정: 적용(그대로)
- 커밋: `ce1fc01da9`
- upstream 근거: `e6fb64d2c2`(`src/battle_ai_util.c` +4/−7). 부모 #9717(seq 150, 보류)과 파일·줄이 겹치지 않는다. `git log --grep='#9755'` 없음 → 미적용이었다.
- 수정 파일(1): `src/battle_ai_util.c`
- 적용 방법: 사전 분석 patch(`seq151-9755.patch`)를 `git apply`했다. 충돌 없음. 결과가 upstream·1.17.0과 같다.
- 내용: `IsDamageMoveUnusable`에서 원시 날씨(끝의대지·시작의바다) 검사를 감싼 `if (HasWeatherEffect())` 블록을 없앤다. `ctx->weather`는 모두 `AI_GetWeather()`/`AI_GetSwitchinWeather()`에서 오고, 두 함수는 `!AI_WeatherHasEffect()`이면 `B_WEATHER_NONE`을 돌려준다(턴 시작 `HasWeatherEffect()` 스냅숏).
- 동작: 사실상 같다. 달라지는 경우는 `AI_FLAG_NEGATE_UNAWARE` AI(HnS 트레이너 데이터 0건, 디버그로만 켤 수 있음)와, 같은 턴 안에 날씨부정·에어록이 새로 나온 뒤 원시 날씨 아래에서 AI가 다시 계산하는 드문 경우뿐이다(upstream 동작과 같음).
- HnS 적응: 없음. 제외한 hunk: 없음.
- 검증:
  - `git diff --check` 통과. 한글·config·세이브 변경 0.
  - 빌드(`build/port.log`): 종료 코드 0, **ROM 32,720,884 B(97.52%, −16 B) / EWRAM 248,940 B(0) / IWRAM 25,516 B(0)**. 사전 분석 추정 −4 B(정렬 전). `pokehns.gba` SHA1 `66559267420cdc9f58b5a73ba76c4b1d9983f794`. 경고 0줄(새 경고 0).
- 테스트(지정 3파일: `test/battle/ai/ai_switching.c`(원시 그란돈), `test/battle/ai/ai.c`·`test/battle/ai/ai_doubles.c`(날씨부정)) → 259줄(PASS 219)이 seq 130·131 적용 뒤 결과와 **모두 같다**(사라진 PASS 0). upstream 테스트 변경 없음.
- 남은 위험: 없음.
- 실기 확인: 불필요.

## 동기화 단위: seq 156 #9774 `U-relearner-9006` Properly cancel move relearner NPC after refusing to learn a move

- 현재 판정: 적용(그대로)
- 커밋: `adb22cd5f0`
- upstream 근거: `2b9bffd8ce`(`data/maps/FallarborTown_MoveRelearnersHouse/scripts.inc` 1줄). deps #9006(seq 133). `git log --all --grep='#9774'` 없음 → 미적용이었다.
- 수정 파일(1): `data/maps/FallarborTown_MoveRelearnersHouse/scripts.inc`
- 적용 방법: 사전 분석 patch(`seq156-9774.patch`)를 `git apply`했다. 충돌 없음(seq 133이 바꾼 31행은 이 hunk 문맥 밖).
- 내용: #9006 뒤 `TeachMoveRelearnerMove`는 결과를 `VAR_RESULT`로 돌려주므로 `goto_if_eq VAR_0x8004, 0, …ChooseMon` → `goto_if_eq VAR_RESULT, FALSE, …ChooseMon`.
- HnS 적응: 같은 판정을 쓰는 HnS 실사용 맵 `data/maps/BlackthornCity_House3_hns/scripts.inc`는 **seq 133 커밋 `fc205e1a40`에서 이미 고쳤다**(#9006과 같이 넣지 않으면 seq 133~155 사이에 하트비늘 처리가 깨지므로). "배우게 하겠습니까?" 거절 시 제대로 취소되는 것은 seq 133의 #10223 1줄(`gSpecialVar_Result = FALSE`)이 있어야 완성된다. `data/maps/TwoIsland_House_Frlg/scripts.inc`(`VAR_0x8004, 0`)는 upstream 1.17.0에도 그대로라 upstream대로 둔다(FRLG 맵).
- 제외한 hunk: 없음.
- 검증:
  - `git diff --check` 통과.
  - 빌드(`build/port.log`): 종료 코드 0, **ROM 32,720,884 B(0) / EWRAM 248,940 B(0) / IWRAM 25,516 B(0)**. `pokehns.gba` SHA1 `b530755d8b5ee6a9ff49393be23c37605d2d0532`. 경고 0줄(새 경고 0). SHA1이 바뀐 것은 Fallarbor 스크립트가 HnS 맵 목록에는 없지만 `data/event_scripts.s`(218행)로 ROM에 어셈블되기 때문이다(`goto_if_eq` 변수 인자 0x8004 → 0x800D, 크기 동일). HnS 맵에서 `MAP_FALLARBOR_TOWN`으로 가는 워프는 0건이라 도달하지 않는다.
- 테스트: 없음(스크립트 1줄, upstream 테스트 변경 없음). 빌드(스크립트 어셈블)로 확인.
- 남은 위험: 없음.
- 실기 확인: 불필요(도달 불가 맵). 검은먹시티 NPC는 "실기 확인 항목" 2.

## 동기화 단위: seq 157 #8628 `U-8628` Add setmetatileinrange Script Command

- 현재 판정: 적용(그대로, 공백 1줄 정리)
- 커밋: `5381abba16`
- upstream 근거: `6045e2a3c9`(`asm/macros/event.inc`, `src/scrcmd.c`). `git log --grep='#8628'` 없음 → 미적용이었다.
- 수정 파일(2): `asm/macros/event.inc`, `src/scrcmd.c`
- 적용 방법: 사전 분석 patch(`seq157-8628.patch`)를 `git apply`했다. 충돌 없음(seq 133 #9006의 두 파일 삭제 hunk는 더 뒤쪽이라 겹치지 않음).
- 내용: `setmetatile` 매크로 뒤에 `setmetatileinrange xmin, ymin, xmax, ymax, metatileId, collision=FALSE, elevation=0xFF` 매크로(`callnative NativeFunc_SetMetatileInRange` + 인자 바이트), `ScrCmd_setmetatile` 뒤에 사각형 범위 메타타일 설정 함수 `NativeFunc_SetMetatileInRange`.
- 스크립트 명령 번호·세이브: **새 opcode를 추가하지 않는다**(기존 `SCR_OP_CALLNATIVE` + 함수 주소). `data/script_cmd_table.inc`(HnS `0x00~0xf6`, 끝의 HnS 추가분 포함) 불변. 기존 스크립트 바이트코드·opcode 번호 불변, HnS 맵은 이 매크로를 쓰지 않는다. 세이브 영향 없음.
- HnS 적응: upstream `u32 temp;` 다음 빈 줄의 줄끝 공백 4칸을 지웠다(`git diff --check` 통과용). 그 밖에 없음.
- 제외한 hunk: 없음.
- 검증:
  - `git diff --check` 통과. 한글·config 변경 0.
  - 빌드(`build/port.log`): 종료 코드 0, **ROM 32,720,884 B(0) / EWRAM 248,940 B(0) / IWRAM 25,516 B(0)**. `pokehns.gba` SHA1 `b234b82e58c16022cafd2fe7ff094fa24be40f2b`. 경고 0줄(새 경고 0). `pokehns.map`의 Discarded input sections에 `.text.NativeFunc_SetMetatileInRange`(0xbc)가 있다(참조 없음 → gc).
  - SHA1이 seq 156과 다른 이유(진단): 커밋 전 상태를 patch 역적용으로 다시 빌드해(`build/port-diag.log`, SHA1 `b530755d…` = seq 156 결과와 같음) ELF 심볼을 비교했다. 주소가 바뀐 심볼은 링커가 만드는 ARM/Thumb 전환 veneer 38개(`__*_from_thumb`/`__*_from_arm`, 0x08248660~ 같은 영역 안)의 **순서**뿐이고, 그 밖의 모든 심볼 주소는 같다(`CSWTCH.168` → `CSWTCH.171` 이름만 바뀜). ROM 차이 1,758 B는 이 veneer를 부르는 호출 오프셋이다. 전역 심볼이 하나 늘어 링커의 veneer 배치 순서가 바뀐 것이고 동작은 같다. 진단 뒤 patch를 다시 적용했다(작업 트리 = 커밋 내용).
- 테스트(`test/script.c`, callnative 효과 분석 경로) → `Script_HasNoEffect control flow`·`Script_HasNoEffect variables` PASS 2, seq 126 기준과 같다. 이 PR은 테스트를 바꾸지 않는다.
- 남은 위험: 없음. 좌표가 `u8`이라 `xmin + MAP_OFFSET`이 255를 넘으면 잘린다(upstream과 같음, 쓰는 스크립트 없음).
- 실기 확인: 불필요(쓰는 스크립트 없음).

## 동기화 단위: seq 158 #9765 `U-9765` Remove template abstraction for pokedex area map

- 현재 판정: 적용(그대로)
- 커밋: `2cef59f506`
- upstream 근거: `63d96455ca`(3파일 +13/−57). `git log --all --grep='#9765'` 없음 → 미적용이었다.
- 수정 파일(3): `include/pokedex_area_region_map.h`, `src/pokedex_area_region_map.c`, `src/pokedex_area_screen.c`
- 적용 방법: 사전 분석 patch(`seq158-9765.patch`)를 `git apply`했다. 충돌 없음. `src/pokedex_area_region_map.c`·`include/pokedex_area_region_map.h`는 적용 뒤 upstream `63d96455ca`와 **바이트 동일**(이식 전에도 upstream 부모와 같았다).
- 내용: `PokedexAreaMapTemplate` 구조체를 없애고 BG 번호를 `#define POKEDEX_AREA_MAP_BG 3`으로 고정. 쓰이지 않던 affine 분기(mode≠0)와 그 그래픽 `sPokedexAreaMapAffine_Gfx`·`_Tilemap` INCBIN, BG 번호 EWRAM 포인터 `sPokedexAreaMapBgNum`과 `Alloc`/`FreePokedexAreaMapBgNum`, `offset` 0이라 아무 일도 안 하던 `AddValToTilemapBuffer` 호출 삭제. `pokedex_area_screen.c`의 `sPokedexAreaMapTemplate`(bg 3, offset 0, mode 0) 삭제, 호출 2곳 `LoadPokedexAreaMapGfx()`, `FreePokedexAreaMapBgNum()` 호출 삭제.
- 동작: 같다. 이전에도 템플릿 값으로 항상 mode 0 분기만 탔고, 이후 같은 호출을 BG 3 상수로 한다. 기본 도감·HGSS 도감 모두 같은 `DisplayPokedexAreaScreen`을 거친다. 조토·관동 지도 선택(`GetRegionMapType`/`gRegionMapInfos`)은 건드리지 않았다.
- HnS 적응: 없음. HnS `pokedex_area_screen.c` 고유 변경(`MAP_GROUP_*_HNS`, 조토 표시 시 관동 MAPSEC 제외, `MapHasSpecies(…, headerSectionId, …)`, 낮/밤 전환, `GetActiveRegionMapEntries()` 좌표)은 hunk와 떨어져 있어 그대로다.
- 제외한 hunk: 없음. `graphics/pokedex/region_map_affine.*`와 `graphics_file_rules.mk` 규칙은 upstream도 남겼다(참조 없음, ROM에 안 들어감).
- 검증:
  - `git diff --check` 통과. 한글·config·세이브 변경 0.
  - 빌드(`build/port.log`): 종료 코드 0, **ROM 32,715,524 B(97.50%, −5,360 B) / EWRAM 248,936 B(94.96%, −4 B) / IWRAM 25,516 B(0)**. 사전 분석 추정 약 −5.29 KB(affine gfx 4,368 B + tilemap 608 B + 함수), EWRAM −4 B(`sPokedexAreaMapBgNum`). `pokehns.gba` SHA1 `f5cdbb4354fd0f1c6178c6987db0e67add92684a`. 새 경고 0(경고 2줄 모두 기존).
- 테스트: 없음(이 PR은 테스트를 바꾸지 않고, 도감 분포 화면 테스트도 없다). 전체 테스트는 메인이 구간 끝에 돌린다.
- 남은 위험: 낮음(코드 경로 동일, 힙 4 B 할당·해제가 없어짐).
- 실기 확인: 필요(아래 "실기 확인 항목" 6).

## 한글 문구 미결

**공식 문구 확인 전 임시 번역**이다. 확정되면 `src/strings.c`의 해당 줄만 바꾼다(seq 133 #9006).

| ID | 초안(`src/strings.c`) | 영문 원문(upstream #9006) | HnS에서 화면에 나오지 않는 이유 |
|---|---|---|---|
| `gText_MoveRelearnerTeachMoveConfirmUseTm` | `{STR_VAR_2}{K_EULREUL} 배우게 하겠습니까?\n{STR_VAR_3} 1개가 없어집니다` | `Teach {STR_VAR_2}?\nThis will consume one {STR_VAR_3}.` | `ShouldConsumeTmItem`(TM 떠올리기 모드이면서 비스크립트 모드)에서만 쓴다. HnS는 `P_TM_MOVES_RELEARNER FALSE`이고 요약·파티 relearner config도 FALSE다 |
| `gText_MoveRelearnerStop` | `{STR_VAR_1}에게 새 기술을\n배우게 하는 것을 그만두겠습니까?` | `Stop trying to learn new\nmoves for {STR_VAR_1}?` | 비스크립트 모드(요약·파티 relearner)의 목록 취소에서만 쓴다. HnS는 `P_SUMMARY_SCREEN_MOVE_RELEARNER`·`P_PARTY_MOVE_RELEARNER` FALSE. 검은먹시티 NPC(스크립트 모드)는 기존 `gText_MoveRelearnerGiveUp`을 쓴다 |

- 근거 표현: `gText_MoveRelearnerTeachMoveConfirm`(`{STR_VAR_2}{K_EULREUL}\n배우게 하겠습니까?`), `gText_MoveRelearnerGiveUp`(`…에게 기술을\n배우게 하는 것을 포기하겠습니까?`), 챌린지 메뉴의 `기술머신은 한 번\n사용하면 없어집니다`(`challenge_menu.c`). 종결 마침표는 HnS 확인 문구 관례(마침표 없음)를 따랐다. upstream 줄 끝의 `;;`는 하나로 줄였다.
- 인코딩·폭: 44 B·50 B(종단 포함). 176px 창에서 최대 172px(가장 넓은 기술명 88px 기준)·127px로 들어간다(사전 분석 계산).

## 실기 확인 항목 (친구용)

이식 전 ROM(`e629de8bdf`, SHA1 `197afe07…`)과 이식 후 ROM을 같은 세이브로 비교한다.

1. **예측 AI 트레이너(#9575):** 예측 AI 트레이너(예: 재대전 관장 1명, 더블배틀 `FINLEY_HNS` 또는 `MUALANI_HNS`, `STEVEN_HNS`)와 싸워 AI의 교체·기술 선택이 멈추거나 이상하지 않은지, 턴 시작 지연이 늘지 않았는지 본다. AI 판단이 1.17.0과 같아지는 변화라 이식 전과 다른 선택을 할 수 있다.
2. **기술 떠올리기·가르침(#9006):**
   - 검은먹시티 기술 떠올리기 NPC(`BlackthornCity_House3_hns`), 파티 포켓몬과 PC 박스 포켓몬 각각:
     - 기술 3개 이하인 포켓몬이 배우기 → 하트비늘 1개 감소
     - 기술 4개인 포켓몬의 교체(요약 화면 선택, PC 포켓몬 포함) → 새 기술 최대 PP
     - "배우게 하겠습니까?"에서 아니오 → 목록으로 돌아가고 **하트비늘 유지**(#10223 1줄)
     - 목록 취소 → 포기 → 포켓몬 선택으로 돌아가고 하트비늘 유지
     - 교체 거절 → "결국 배우지 않았다" → 목록
     - 메시지 줄바꿈·창 넘침(공용 문자열로 바뀜), 팡파레가 끊기는지
   - 기술 가르침 NPC(`data/scripts/move_tutors.inc` 사용 NPC, `UlaulaIsle_hns`, `BattleFrontier_Lounge7_hns`, `NewSinjoh_HotSprings_hns` PLA 가르침)에서 PP가 적은 기술을 교체하면 새 기술이 **최대 PP**인지(`// HnS:` 줄).
   - 요약 화면: 능력치 페이지 START/R/L 전환, 기술 페이지 오른쪽 위에 잔상·영문이 없는지, 박스 요약, 전투 중 요약.
   - 목록 다시 그리기(`RedrawListMenu`): 옵션·챌린지 메뉴에서 값 변경 시 설명·하이라이트, 챌린지 메뉴 맨 아래로 이동, 커트 볼 상점 구매 뒤 목록 복귀(아이콘·열매 수).
3. **디버그 사운드 메뉴(#9713, B안):** 디버그 메뉴(R+START) → Sound → SFX/Music.
   - 첫 화면 SE `0001`, 음악 `0350`, 이름 자리에 `SE_`/`MUS_`가 나오는지
   - 위/아래·좌우 자릿수 이동, 음악 734 → 735(DP 곡) ~ 745, SE 269 다음 797(`SE_FASTER_JOY_HEAL`) 재생
   - SELECT GBS 전환 뒤 문구 On/Off 갱신과 재생
   - 메뉴를 나간 뒤 다른 창의 글자가 깨지지 않는지(이식 전 이름 복사 오버런 해소 확인)
4. **도감 무게 파운드 표시(#9690):** 옵션 "단위계"를 "야드파운드법"으로 바꾼 뒤 도감(기본·HGSS 화면)에서 폴리곤2 또는 다꼬리 무게가 `71.7 lbs.`(이식 전 71.6), 피카츄(6.0 kg)는 `13.2 lbs.`(변화 없음)인지 본다. "미터법"으로 되돌리면 kg 표시가 이식 전과 같아야 한다.
5. **맵 이름 팝업(#9461):** 팝업 모양·글꼴이 이식 전과 같은지. 숫자가 든 도로("29번 도로"), 긴 이름("사파리존 게이트", "블루시티동굴", "남쪽의 외딴섬"), 다층 던전(모다피의 탑·연결동굴 등: **층 표시가 나오지 않는 것이 정상**), 배틀프런티어·배틀 피라미드 팝업.
6. **도감 분포 화면(#9765):** HGSS 도감 → 분포 화면(조토 지도, 관동 방문 뒤 관동/조토 지도), 위·아래로 낮/밤 전환 반복, 분포 화면 ↔ 울음소리/크기 화면 전환 뒤 복귀, 도감 종료. 지도·서식지 표시가 이식 전과 같아야 한다.

## 후속 행 메모

- **seq 390 #10223:** `chooseboxmon.c` `PROMPT_BEFORE_LEARNING_2` "아니오"의 `gSpecialVar_Result = FALSE;` hunk는 **이미 적용**(seq 133 커밋 `fc205e1a40`에 선반영, upstream `42e299ed0f`와 같은 줄). 나머지 hunk만 이식한다.
- **seq 144 #7573:** `CanMonLearnMove` → `ChooseBoxMon_CanMonLearnMove`로 이름을 바꿀 때 HnS `VALIDATE_BEFORE_LEARNING`의 `switch (IsBoxMonExcluded(boxmon))`와 PLA 가르침 코드는 유지한다.
- **#9006 이후 relearner 행:** `field_specials.c` `Task_ReturnToFieldWhileLearningMove`의 `tRecoverPp = TRUE`(`// HnS:`)는 upstream에 없는 줄이다. 후속 PR이 이 주변을 고치면 유지 여부를 본다. seq 268 #10368(`UIEndTask`의 비스크립트 TRUE 경로)은 HnS 스크립트 모드에서 닿지 않는다.
- **seq 298 #10445:** `HandleMoveRelearnerInput`의 `gSpecialVar_MonBoxId = StorageGetCurrentBox();` 줄(박스 번호 줄)은 **이미 적용**(seq 134 커밋 `cf71e21e56`). 함수 앞의 중복 `gSpecialVar_MonBoxPos = sMonSummaryScreen->curMonIndex;` 삭제만 남는다.
- **`U-mapheader-9461` 뒤 행(seq 358 #7975, 371 #9080, 374 #10167, 377 #10176, 462 #10159):** `global.fieldmap.h`의 `MapHeader`와 `tools/mapjson/mapjson.cpp`는 upstream #9461과 같은 모양이다. `map_name_popup.c`의 HnS 차이 3곳(피라미드 조건, `FONT_NARROW`, `!IS_HNS` 백화점 가드)은 유지한다.
- **HnS 맵에 `floor_number`를 넣을 때(별도 결정):** 층 표기 한글화(`MapNamePopupAppendFloorNum` HnS 분기)와 `Map names fit in popup`(`FONT_NARROWER` 80px) 테스트 한계를 같이 정한다. 생성 `header.inc`는 도구가 바뀌어도 자동 재생성되지 않으므로 `floor_number`를 넣은 맵은 `map.json` 수정으로 재생성된다.
