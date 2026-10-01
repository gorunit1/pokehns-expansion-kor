# R3 리뷰 — C 코드 (seq 127 #9655 + #10064)

- 리뷰어: R3(읽기 전용). 기준 HEAD `05319fd9b7`, 작업 트리 `git diff`의 5파일(`src/battle_script_commands.c`, `src/battle_util.c`, `src/battle_hold_effects.c`, `src/battle_end_turn.c`, `src/pokemon.c`, +109/−62).
- 비교: upstream `32fcd64868`(#9655), `3ed1ce5570`(#10064), `expansion/1.17.0`, `master`. 참고 `part-C.md`.
- 파일 수정·빌드·테스트를 하지 않았다. 아래 "확인"은 코드를 읽어 확인한 것이고, 확인하지 못한 것은 "미확인"으로 적었다. 줄 번호는 현재 작업 트리 기준이다(`bsc` = `src/battle_script_commands.c`).

## 요약

| 등급 | 건수 | 항목 |
|---|---|---|
| 수정 필요 | 1 | R3-1: `HealStatusConditions()`에서 `gBattlerPartyIndexes[MAX_BATTLERS_COUNT]`를 범위 밖으로 읽음. 규칙상 수정 필요로 분류했지만 출력과 동작에는 영향이 없음 |
| 경미 | 2 | R3-2: 멘탈허브가 효과 없이도 MULTISTRING을 0으로 만듦. R3-3: 열매 표 PROBLEM 칸에 도달할 수 없고, 그 문장의 `{B_BUFF1}`을 채우던 코드가 사라짐 |
| 확인만 | 9 | R3-4~R3-12 |
| 범위 밖(이번 diff와 무관한 기존 문제) | 2 | X-1: 가방 도구로 대기 포켓몬의 잠듦을 고치면 `gBattleMons[4]`에 씀. X-2: 소란 기상 상태가 잘못된 배틀러에 전송됨 |

### hunk 완전성과 HnS 표식
- upstream의 C hunk 32개와 #10064 1개를 대상으로, 변경 줄(+/−)을 다중집합으로 비교했다(공백 정규화). 작업 트리에 없는 upstream 줄은 part-C가 "제외"한 6개 hunk(`TryCureAnyStatus`, `SetMoveEffect` AURORA_VEIL, BREAK_SCREEN, `Cmd_curestatuswithmove`, `BS_CureStatus`, `BS_SetAuroraVeil`)와 D1의 1줄(`STRINGID_PKMNEVADEDATTACK`)뿐이다. 작업 트리에만 있는 줄은 `// HnS:` 3곳, `FinalizeCapture` 저장·복원 2줄, FREEEZE→FREEZE 3곳, HnS 중복 대입 3줄 삭제(`HEAL_MON_STATUS` 1줄, `TryImmunityAbilityHealStatus` 2줄)뿐이다. **빠진 upstream hunk는 없다.**
- 5파일 모두에서 `HnS|IS_HNS|IS_FRLG|tx_` 줄 수가 HEAD보다 줄지 않았다(bsc 22→24, util 9→10, 나머지 3파일은 같음). diff의 삭제 줄에는 HnS 표식과 `GetConfig`가 없다. 변경된 함수의 HnS 분기(#9929 삼항식, 멘탈허브 Fling 분기, 리샘 루프, 값형 `B_MSG_BREAK_*`, #9616 방음 줄, `B_MENTAL_HERB`·`B_ROUGH_SKIN_DMG`·`B_OBLIVIOUS_TAUNT` 조건)가 모두 남아 있다.
- 5파일에서 `git diff --check`를 통과했다.

---

## 수정 필요

### R3-1 `HealStatusConditions()`의 `gBattlerPartyIndexes[MAX_BATTLERS_COUNT]` 범위 밖 읽기
- 위치: `src/pokemon.c:6556`
  ```c
  PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, battler, gBattlerPartyIndexes[battler]);
  ```
  배열은 `u16 gBattlerPartyIndexes[MAX_BATTLERS_COUNT]`이다(`include/battle.h:973`, `src/battle_main.c:161`).
- `battler == MAX_BATTLERS_COUNT`(4)인 호출 경로(확인)
  - 필드: `PokemonUseItemEffects()`에서 `battler = MAX_BATTLERS_COUNT`(pokemon.c:6105)로 정한 뒤 다시 대입하지 않고, 6191~6199에서 상태 회복 5종을 처리할 때 그대로 넘긴다. 파티 메뉴 도구 사용(`ExecuteTableBasedItemEffect`)과 포켓몬 피리(item_use.c:1714)가 이 경로를 쓴다.
  - 전투: 대기 포켓몬에게 가방 도구를 쓰면 `BS_ItemCureStatus()`가 `targetBattler = MAX_BATTLERS_COUNT`(bsc:12352)인 채로 bsc:12370을 호출한다.
- 실패 시나리오: 필드에서 마비된 포켓몬에게 "마비치료제"를 쓰면 배열 끝 뒤의 2바이트를 읽어 `gBattleTextBuff1[3]`에 넣는다. 어떤 변수를 읽는지는 링커 배치에 달려 있어 확인하지 못했다(선언 순서상 `gBattlerPositions` 앞부분으로 추정).
- 영향(확인): 읽기만 한다. 쓰는 곳은 `gBattleTextBuff1`(정상 버퍼)와 MULTISTRING뿐이고, `gBattleMons` 쓰기는 기존 `gMain.inBattle && battler != MAX_BATTLERS_COUNT` 가드가 막는다. 이 닉네임 버퍼는 어느 경로에서도 출력되지 않는다.
  - 필드에서는 쓰지 않는다.
  - `BS_ItemCureStatus`에서는 bsc:12383의 `PREPARE_SPECIES_BUFFER`가 덮어쓴다.
  - `FinalizeCapture`에서는 이후 포획 문장에 `{B_BUFF1}`이 없다.
  - 따라서 **출력과 동작에는 영향이 없다.** upstream `32fcd64868`, 1.17.0(pokemon.c:3905), master가 모두 같은 코드이고, part-C Q4가 이미 기록했다.
- 등급 이유: REVIEW_BRIEF의 등급 기준이 "범위 밖 읽기"를 수정 필요로 정한다. upstream과 같게 두기로 결정하면 기록만 남기면 된다.
- 제안(출력 불변, 1줄 가드):
  ```c
      // HnS: battler is MAX_BATTLERS_COUNT for field use and bench mons; do not read gBattlerPartyIndexes out of bounds
      if (battler < MAX_BATTLERS_COUNT)
          PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, battler, gBattlerPartyIndexes[battler]);
  ```

---

## 경미

### R3-2 `TryMentalHerb()`가 효과가 없어도 MULTISTRING을 0으로 만듦
- 위치: `src/battle_hold_effects.c:427` `gBattleCommunication[MULTISTRING_CHOOSER] = 0;`(upstream과 같음)
- HEAD는 효과가 있을 때만 대입했다. 이제는 멘탈허브를 가진 포켓몬이 타격 뒤 move end 단계(대상은 battle_move_resolution.c:2504, 공격자는 :2517)를 지날 때마다 MULTISTRING이 0이 된다.
- 확인한 것
  - 두 move end 단계는 각각 한 보유자만 보고, `||` 단락 평가로 첫 효과가 나면 스크립트를 실행한 뒤 다음 단계로 넘어간다. 따라서 한 보유자의 비트마스크가 스크립트 실행 전에 다른 호출로 지워지지 않는다.
  - Fling(bsc:3338)도 한 번만 호출한다.
  - C에서 MULTISTRING을 읽는 곳(battle_anim_throw.c:2651, bsc:8160, 11379/11397/11411)은 move end 이후의 묵은 값에 의존하지 않는다.
- 실패 시나리오를 찾지 못했다. 기록만 권장한다.

### R3-3 `CureStatusBerryEffectStringID`의 PROBLEM/NORMALIZED 칸은 도달할 수 없고, 그 문장의 `{B_BUFF1}`을 채우던 코드가 사라짐
- 위치: `src/battle_message.c:1401~1403`(파트 A 파일이지만 C와의 계약 문제)
- C에서 `B_MSG_CURED_PROBLEM`을 만드는 곳은 `GetCuredStatusMessage()`(battle_util.c:9697) 하나다. 그 값은 `Cmd_curestatuswithmove`(bsc:9846)와 `BS_CureStatus`(bsc:14775)를 거쳐 `gCureStatusStringIds`(battle_scripts_1.s:391/761/1280/3885)와 `gPurifyStatusCureStringIds`(:851)로만 간다.
- 열매 표를 출력하는 `BattleScript_BerryCureStatusRet`·`BattleScript_LumBerryCureStatusRet`의 호출자(hold_effects:679/694/709/734/749/787)는 모두 상태별 값이나 리샘 루프 값만 넣는다. `B_MSG_NORMALIZED_STATUS`를 만드는 곳은 없다.
- 함정 3의 근거인 "GetCuredStatusMessage()가 씀"은 정확히는 `gCureStatusStringIds`(1493~1494)와 `gPurifyStatusCureStringIds`(1276~1277)에 해당한다. 두 표 모두 PROBLEM/NORMALIZED 칸이 있어 범위 밖 읽기는 없다(아래 R3-7).
- 열매 표에 남긴 `STRINGID_PKMNSITEMCUREDPROBLEM`(battle_message.c:476)은 `…로\n{B_BUFF1}상태가 나았다!`이다. 이번 diff로 BUFF1에 상태 이름을 넣던 `StringCopy(gBattleTextBuff1, gStatusConditionString_*Jpn)`이 C 전체에서 0건이 되었다(HEAD는 battle_util.c 11건, hold_effects 1건).
- 지금은 도달할 수 없으므로 출력 영향은 없다. 나중에 PROBLEM을 열매 표로 보내는 코드가 생기면 묵은 BUFF1이 출력된다. 기록을 권장한다.

---

## 확인만

### R3-4 D1과 빗나감 관련 경로
- 확인: bsc:2051 `stringId = STRINGID_PKMNAVOIDEDATTACK; // HnS: … (D1)`. upstream 1.17.0도 `Cmd_resultmessage`에서 `STRINGID_PKMNAVOIDEDATTACK`를 쓴다(1.17.0 bsc:1322). 따라서 1.17.0 최종형과 같아 이후 이식에서 충돌하지 않는다.
- #9929 분기 bsc:1219~1221 `failInstr == BattleScript_ButItFailed ? BattleScript_TargetAvoidsAttackEnd : failInstr`가 그대로이다. `AccuracyCheck` 안의 변경은 1170의 1줄뿐이다.
- `MISS_TYPE`
  - 값이 0(`B_MSG_MISSED`)에서 96(현재 헤더의 `STRINGID_PKMNEVADEDATTACK`, u8에 들어감)으로 바뀐다.
  - MISS_TYPE을 읽는 3곳은 모두 `B_MSG_PROTECTED`(1)와 비교한다: 메멘토 C(bsc:9807 `!=`), 메멘토 스크립트(battle_scripts_1.s:3626 `CMP_EQUAL`), 배틀 아레나 판정(battle_arena.c:392 `!=`).
  - MISS_TYPE에 1을 쓰는 곳은 HEAD와 작업 트리 모두 없다(쓰는 곳은 bsc:1170과 battle_util.c:455의 0 리셋뿐). 따라서 **판정은 바뀌지 않는다.** `gBattleCommunication[6]`을 숫자로 직접 가리키는 곳도 없다.
- 일격기: `gKOFailedStringIds`(`B_MSG_KO_MISS`)는 C와 스크립트 어디서도 참조하지 않아 출력과 무관하다. 파트 A는 이 칸을 `PKMNEVADEDATTACK`로 두었고 1.17.0은 `PKMNAVOIDEDATTACK`이지만, 쓰이지 않으니 영향이 없다. 레벨 차이 실패와 옹골참 분기는 그대로이다.
- 방어: battle_move_resolution.c:1835 `B_MSG_PROTECTED` → `gMissStringIds[PROTECTED]`로 바뀌지 않았다.

### R3-5 CMP_BITMASK
- bsc:4811/4855/4899 세 함수의 새 case가 upstream과 글자 단위로 같다(`*ptr & (1 << value)`). 기존 case(EQUAL~NO_COMMON_BITS) 줄은 바뀌지 않았다. 정의는 `include/constants/battle_script_commands.h:350`의 `6`이다.
- 사용처는 멘탈허브 6줄(battle_scripts_1.s:7256~7266)뿐이고 값은 0~5다. HnS의 `B_MSG_BREAK_*`는 `CMP_COMMON_BITS` 그대로이다(:3786~3792, 함정 2).

### R3-6 멘탈허브
- hold_effects:434/443/451/458/466/473에서 `|= 1 << B_MSG_MENTALHERBCURE_*`로 비트를 쌓는다. 검사 순서(헤롱헤롱→트집→사슬묶기→회복봉인→앙코르→도발)는 upstream과 같다.
- enum(battle_string_ids.h:1151~1156)은 0~5로, upstream `32fcd64868`과 순서가 같다. 스크립트 분기도 같은 enum 이름을 쓰므로 비트 번호가 일치한다. 최댓값 0x3F는 u8(`gBattleCommunication` u8[8], battle.h:1017)에 충분히 들어간다.
- Fling 경로: bsc:3338 → `ItemBattleEffects(…, IsOnFlingActivation)` → 같은 `TryMentalHerb()` → hold_effects:482 `BattleScript_MentalHerbCureFling`. 같은 값을 만든다. Ret과 Fling은 `BattleScript_MentalHerbCureMessages`를 공유한다.
- `gBattleScripting.battler = itemBattler`(hold_effects:1192)가 스크립트 실행 전에 설정된다. 따라서 D6-b의 `copybyte gBattlerAttacker, sBATTLER`가 허브 보유자를 가리킨다.

### R3-7 함정 3
- `FREEEZE`는 src·include·data·test에서 0건이다(docs에만 남음). C 쪽 3곳(hold_effects:723, bsc:12418, battle_util.c:9692)과 새 줄(3738, 9087)이 모두 `B_MSG_CURED_FREEZE`이다.
- `GetCuredStatusMessage()`의 반환값이 쓰이는 표는 `gCureStatusStringIds`와 `gPurifyStatusCureStringIds` 둘이고, 둘 다 PROBLEM/NORMALIZED 칸이 있다. 실제로 PROBLEM이 나오는 경로도 없다. 호출 전에 모든 경로가 상태를 확인한다.
  - 리프레시·브레이브차지: `shouldHeal`
  - 정글힐·정화: `jumpifstatus`
  - 사이코시프트: `BS_TryPsychoShift`가 공격자의 status1을 지우지 않음
  - 힐러: 표 대신 `printstring`
- `gPartyCureStatusStringIds`(0~8)는 `HealStatusConditions()`가 주는 0~5만 받는다(대기 포켓몬 경로는 `ItemHealMonVolatile`을 호출하지 않음). 따라서 범위 안이다.

### R3-8 함정 4와 D7
`{B_SCR}` 문장을 출력하는 경로마다 `gBattleScripting.battler`가 스크립트 실행 전에 실제 회복 대상으로 설정되는지 확인했다.

| 출력 | 경로 | 설정 위치 |
|---|---|---|
| `gCureStatusStringIds` | 리프레시·브레이브차지 | bsc:9847 `= gBattlerAttacker` |
| `gCureStatusStringIds`·`gPurifyStatusCureStringIds` | 정글힐·사이코시프트·정화 | bsc:14776 `= battler` |
| `gCureStatusStringIds` | 탈피·촉촉한몸(`ShedSkinActivates`) | battle_util.c:3744 `= battler` |
| `gCureStatusStringIds` | 면역·파스텔베일·유연·불면·의기양양·수의베일·수포·열교환·마그마의무장·마이페이스(`AbilityCuredStatus`) | battle_util.c:9135(`BattleScriptCall` 뒤지만 스크립트 실행 전) |
| `PKMNGOTOVERITSINFATUATION`·`PKMNSHOOKOFFTHETAUNT` | 둔감 전용 스크립트 | battle_util.c:9135 |
| `gCureStatusStringIds` | 가방, 필드의 포켓몬(`CureStatus_Battler`) | bsc:12390 `= targetBattler` |
| `HEALBLOCKEDNOMORE` | 회복봉인 턴 종료 | battle_end_turn.c:838 `= battler` |
| 헤롱헤롱·트집·회복봉인·도발(멘탈허브) | Ret·Fling | hold_effects:1192 |

- D7 `STRINGID_PKMNWOKEUPINUPROAR`(`{B_EFF_NAME_WITH_PREFIX}`)를 출력하는 경로는 3개이고, 모두 `gEffectBattler`가 깬 포켓몬이다. `B_UPROAR = GEN_4`라 세 경로 모두 활성이다.
  - 행동 전 기상: battle_move_resolution.c:120 `gEffectBattler = cv->battlerAtk`
  - 배틀팰리스: battle_util2.c:140 `gEffectBattler = battler`
  - 턴 종료: battle_end_turn.c:1231~1245. 루프 변수 `gEffectBattler`가 깬 포켓몬을 가리키고, 찾으면 `break`한다.

### R3-9 #10064 (battle_util.c:5550)
- upstream `3ed1ce5570`과 같다. `RUN_SCRIPT` 호출자는 `Cmd_trynonvolatilestatus`(bsc:8258) 하나다.
- 특성이 아닌 차단 스크립트의 문장은 모두 `{B_DEF}`이거나 `resultmessage` 경로다. 따라서 **출력은 바뀌지 않는다.**
  - `{B_DEF}`: AlreadyParalyzed/Burned/Asleep/Poisoned, CantMakeAsleep(`gUproarAwakeStringIds` 2개), SleepClauseBlocked, Electric/MistyTerrainPrevents, SafeguardProtected
  - `resultmessage`: NotAffected, ButItFailed
- 특성 경로(Flower/Sweet Veil이 `battlerDef`를 아군으로 바꾸는 경우 포함)는 HEAD에서도 같은 값을 설정했다.

### R3-10 `FinalizeCapture` 힐볼 MULTISTRING 저장·복원(HnS 추가) — 필요하고 타당함
- 위치: bsc:10771~10774. 코드로 추적한 흐름은 다음과 같다.
  1. bsc:10763: 파티가 가득 차면 `MULTISTRING = 0`.
  2. 힐볼이면 `HealStatusConditions(caughtMon, STATUS1_ANY, gBattlerTarget)`. 잠든 포켓몬이면 `MULTISTRING = B_MSG_CURED_SLEEP`(=5, battle_string_ids.h:1063)이 되는데, 이 값은 `B_MSG_SWAPPED_INTO_PARTY`(=5, :1000)와 같다.
  3. `BattleScript_SuccessBallThrow`(battle_scripts_2.s:192~215)의 getexp·도감·별명 단계는 MULTISTRING을 쓰지 않는다. `Cmd_getexp` 4277~4612, `BattleScript_LevelUp`, bsc:11423~11640을 확인했다. `setbyte gBattleCommunication, 0`은 MULTIUSE_STATE를 바꾼다.
  4. `Cmd_givecaughtmon`(bsc:11267): 파티가 가득 차 있고 `B_CATCH_SWAP_INTO_PARTY`가 GEN_LATEST이며 파티 제한 챌린지가 아니면 ASK_ADD_TO_PARTY로 가고, 이 단계는 MULTISTRING을 바꾸지 않는다. 플레이어가 아니오·B·교체 취소를 고르거나 `CopyMonToPC`가 실패하면 GIVE_AND_SHOW_MSG로 간다.
  5. bsc:11378~11379 `GiveCapturedMonToPlayer(...) != MON_GIVEN_TO_PARTY && MULTISTRING != B_MSG_SWAPPED_INTO_PARTY`: 값이 5라서 박스 전송 문구 선택과 STR_VAR1/2 채우기, 박스 가득 참·빌 PC 분기를 건너뛴다. 그 결과 `gCaughtMonStringIds[5]` = `STRINGID_PKMNSENTTOPCAFTERCATCH`("{STR_VAR_2} … BOX {STR_VAR_1}")가 묵은 STR_VAR 값으로 출력된다.
- 실패 시나리오(이 수정이 없을 때): 파티 6마리 상태에서 야생 포켓몬을 재우고 힐볼로 잡는다. "지닌 포켓몬에 넣겠습니까?"에서 아니오를 고르면 잡은 포켓몬은 박스로 가지만, "교체해서 PC로 보낸 포켓몬" 문구가 이전 STR_VAR 값으로 나온다.
- 다른 상태값 0~4는 5가 아니어서 PC 분기가 값을 덮어쓰므로 문제는 잠듦뿐이다. 파티가 덜 찼거나 교체 기능이 꺼진 경우에는 CHECK_PARTY_SIZE가 `B_MSG_NO_MESSAGE_SKIP`을 직접 넣으므로 안전하다.
- 저장·복원은 HEAD의 동작(MULTISTRING 0/1 유지)과 같아 부작용이 없다. `HealStatusConditions()`가 덮어쓰는 `gBattleTextBuff1`도 무해하다. 이후 포획 문장(GOTCHAPKMNCAUGHTPLAYER, PKMNDATAADDEDTODEX, GIVENICKNAMECAPTURED, SENDCAUGHTMONPARTYORBOX, `gCaughtMonStringIds` 4개)에 `{B_BUFF1}`이 없다.
- **upstream도 같은 문제가 있다.** 1.17.0의 `FinalizeCapture`(bsc:7803~7815, 저장·복원 없음), `HealStatusConditions`(pokemon.c:3901~3920), `givecaughtmon`(:8262, :8371~8372), enum 값(SLEEP 5, SWAPPED 5)이 모두 같다. master(bsc:7822~7828)도 같다. upstream 보고 후보다. 실제 게임 동작은 확인하지 못했다(실기 확인 후보).

### R3-11 `TryImmunityAbilityHealStatus`의 `// HnS:`와 중복 2줄 삭제
- 삭제한 HEAD 줄은 `IMMUNITY_STATUS_CLEARED`의 `GetCuredStatusMessage(...)`와 `IMMUNITY_CONFUSION_CLEARED`의 `B_MSG_CURED_CONFUSION`이다. 첫 switch(upstream)가 같은 값을 먼저 넣으므로 중복이다.
  - 상태 특성은 각자 한 상태만 검사하고, status1에는 비휘발 상태가 하나뿐이다. 따라서 `GetCuredStatusMessage()`와 같은 값이 나온다. 맹독 카운터만 남은 상태는 실제로 생기지 않는다.
  - 마그마의무장의 얼음/동상 분리는 `STATUS1_ICY_ANY`와 범위가 같다.
- 둔감(헤롱헤롱·도발)은 HnS 전용 스크립트를 유지했다(battle_util.c:9122 `// HnS:`). upstream 첫 switch가 넣는 `B_MSG_CURED_INFATUATION/TAUNT`는 HnS 스크립트가 쓰지 않는 값이지만 해가 없다.
  - 전용 스크립트의 문장 ID는 `gCureStatusStringIds[INFATUATION/TAUNT]`와 같다. 따라서 upstream `BattleScript_AbilityCuredStatus`와 출력이 같다. 차이는 `updatestatusicon`뿐인데, 휘발 상태라 무관하다.
- `HEAL_MON_STATUS`(탈피·촉촉한몸)의 `GetCuredStatusMessage()` 1줄 삭제도 같은 이유로 동등하다. `STATUS1_ANY`의 6종이 모두 if 체인에 있다(battle.h:163).

### R3-12 제외한 6개 hunk와 나머지 hunk
- 제외 6개는 모두 타당하다.
  - `TryCureAnyStatus`: HnS 리샘 반복 출력(hold_effects:770~790, bsc:12395~12440, 출력 정책 문서 HEAD 39행)을 유지했다. 루프 값은 모두 열매 표 범위 안이다(CONFUSION 칸 있음).
  - `SetMoveEffect` AURORA_VEIL(bsc:3541), `BS_SetAuroraVeil`(:14932/14934): 이미 `B_MSG_SET_AURORA_VEIL`이다.
  - BREAK_SCREEN(bsc:3716~3722): HnS 값형 1/2/4와 `CMP_COMMON_BITS`를 쓴다(함정 2). `gSideStatuses` 기준이라 upstream의 `gSideTimers` 기준과 결과가 같다.
  - `Cmd_curestatuswithmove`·`BS_CureStatus`: HnS의 `GetCuredStatusMessage()`와 battler 설정이 이미 있다. 판정 순서가 upstream(마비>독>화상>잠듦>얼음>동상)과 다르지만, 비휘발 상태가 하나뿐이라 결과가 같다.
- `SetNonVolatileStatus`의 BY_ABILITY 삭제는 출력을 바꾸지 않는다.
  - `TRIGGER_ON_ABILITY` 경로는 `BattleScript_AbilityStatusEffect`(잠듦·독·마비·화상·맹독)와 싱크로(독·맹독·마비·화상, bsc:2394~2397)뿐이고, 둘 다 얼음을 걸지 않는다.
  - 따라서 STATUSED와 BY_ABILITY 값이 다른 유일한 칸인 `gGotFrozenStringIds`에 도달하지 않는다. `gAttractUsedStringIds`는 참조가 0건이다.
- 정신력(D4 #7): bsc:2640 `gBattlescriptCurrInstr = battleScript;`가 upstream과 같다. `BattleScript_FlinchPrevention`의 C 참조는 0건이다.
- `RemoveAllWeather`: 원시 날씨 검사가 일반 날씨보다 앞에 있다. `B_WEATHER_RAIN`이 `RAIN_PRIMAL`을 포함하므로(battle.h:457) 이 순서가 맞다. 새 enum 값은 모두 이름으로만 쓰인다(`sBattleWeatherInfo` 포함).
- 울퉁불퉁멧·자보열매·애터열매·까칠한피부/철가시: MULTISTRING 설정이 `BattleScriptCall`보다 앞에 있다. 그 사이 스크립트(아이템 팝업, `tryactivateabilityshield`)는 MULTISTRING을 바꾸지 않는다. `BattleScript_HurtAttacker` 호출자 3개가 모두 MULTISTRING을 설정한다.
- 출력 불변: `BS_ItemRestorePP`의 BUFF2와 사령탑의 BUFF1은 해당 HnS 문장에 토큰이 없다. `HandleEndTurnHealBlock`도 확인했다.

---

## 범위 밖 관찰(이번 diff가 만들지 않음, 별건 권장, 건수에 넣지 않음)

### X-1 `BS_ItemCureStatus`: 대기 포켓몬의 잠듦을 고치면 `gBattleMons[MAX_BATTLERS_COUNT]`에 씀
- bsc:12372~12374 `if (GetItemStatus1Mask(gLastUsedItem) & STATUS1_SLEEP) gBattleMons[targetBattler].volatiles.nightmare = FALSE;`. 대기 포켓몬이면 `targetBattler = MAX_BATTLERS_COUNT`(12352)라서 배열 끝 뒤의 비트를 쓴다. 무엇을 덮어쓰는지는 확인하지 못했다(battle_main.c:168 다음 선언은 `gBattlerSpriteIds`).
- 시나리오: 전투 중 가방의 잠깨는약이나 만병통치제를 잠든 대기 포켓몬에게 쓴다.
- HEAD에도 있는 문제다(upstream #8339 `68a974af86`에서 들어옴). 1.17.0(bsc:9383~9384)과 master도 같다. 제안: `targetBattler != MAX_BATTLERS_COUNT` 조건을 추가한다.

### X-2 소란 턴 종료 기상의 상태 전송 대상
- battle_end_turn.c:1246 `BtlController_EmitSetMonData(gEffectBattler, …, &gBattleMons[gBattlerAttacker].status1)`. part-C Q2와 같은 문제다. 1.17.0(:1298)도 같다. D7 문장과는 무관하다.

## 미확인
- 빌드·테스트·실제 게임 확인은 하지 않았다(지시). R3-10은 코드 추적만으로 판단했다.
- `gBattlerPartyIndexes[4]`가 실제로 읽는 EWRAM 변수(링커 배치).
- HnS 한국어판 `gText_PkmnSentToPCAfterCatch` 본문의 위치(`data/text/pc_transfer.inc`는 영문). R3-10의 결론에는 영향이 없다.
