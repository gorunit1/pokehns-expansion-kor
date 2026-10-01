# review-R2 — 배틀 스크립트 (seq 127 #9655 + #9856 흡수)

- 리뷰어: R2(읽기 전용). 기준 HEAD `05319fd9b7`, 대상은 작업 트리 `git diff`.
- 범위: `data/battle_scripts_1.s`, `data/battle_scripts_2.s`, `include/battle_scripts.h`, `include/constants/battle_script_commands.h`. 참고로 연결된 C 경로(`battle_hold_effects.c`, `battle_script_commands.c`, `battle_util.c`, `battle_end_turn.c`, `pokemon.c`)는 읽기만 했다.
- 수정·빌드·테스트는 하지 않았다. "확인" = 코드를 직접 읽었거나 아래 시뮬레이션으로 검증함. "추정" = 코드 흐름으로 판단했지만 실행으로 확인하지 않음. "미확인" = 확인하지 못함.
- 작업 트리의 4개 파일 diff는 `part-B.patch`와 `index` 줄만 빼고 같다(확인: `diff` 결과 동일).

## 요약

| 등급 | 건수 |
|---|---|
| 수정 필요 | 1 (범위 밖 파일 `pokemon.c`, 낮은 위험) |
| 경미 | 4 |
| 확인만 | 12 |

담당 4개 파일 안에서는 수정이 필요한 결함을 찾지 못했다. 점검 항목 1~7은 모두 통과했다. 아래의 "수정 필요" 1건은 가방 상태회복(점검 6)의 대기 포켓몬 경로를 따라가다 발견한 C 쪽 범위 밖 읽기다.

---

## 수정 필요

### N1. `HealStatusConditions()`가 `battler == MAX_BATTLERS_COUNT`일 때 `gBattlerPartyIndexes[4]`를 읽는다 (범위 밖 읽기, 범위 밖 파일)

- 위치: `src/pokemon.c:6556` (이번 이식으로 추가된 줄)
  ```c
  PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, battler, gBattlerPartyIndexes[battler]);
  ```
  `include/battle.h:973` `extern u16 gBattlerPartyIndexes[MAX_BATTLERS_COUNT];`
- `battler`가 `MAX_BATTLERS_COUNT`인 호출자(확인):
  - `BS_ItemCureStatus` (`battle_script_commands.c:12370`): 싸우지 않는 대기 포켓몬에게 가방 상태회복 도구를 쓰면 `targetBattler = MAX_BATTLERS_COUNT`. 이 경로가 `battle_scripts_2.s`의 `printfromtable gPartyCureStatusStringIds`로 이어진다.
  - `PokemonUseItemEffects` (`pokemon.c:6105` `battler = MAX_BATTLERS_COUNT`, `:6191-6199`): 필드에서 해독제·만병통치제 등을 쓸 때마다 이 함수를 지난다.
- 실패 시나리오: 필드나 배틀에서 대기 포켓몬에게 해독제 사용 → `gBattlerPartyIndexes[4]`(배열 밖, 인접 전역)를 읽는다. 읽은 값은 `gBattleTextBuff1[3]`에만 들어가고, 배틀에서는 바로 뒤 `PREPARE_SPECIES_BUFFER`(`battle_script_commands.c:12383`)가 덮어쓰며, 필드에서는 쓰이지 않는다. 그래서 **보이는 출력 변화나 크래시는 없을 것으로 추정한다.** 다만 브리프 기준의 "범위 밖 읽기"에 해당한다.
- upstream `32fcd64868`과 `expansion/1.17.0` 모두 같은 코드다(확인). HEAD에는 없던 줄이다.
- 제안: 동작을 바꾸지 않는 한 줄 보호를 넣는다.
  ```c
  if (battler < MAX_BATTLERS_COUNT) // HnS: avoid gBattlerPartyIndexes[MAX_BATTLERS_COUNT] (party/overworld callers)
      PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, battler, gBattlerPartyIndexes[battler]);
  ```
  upstream과 같게 두기로 하면 "경미, upstream 동일"로 낮춰 기록한다. 판단은 메인(또는 C 담당 리뷰어)에게 맡긴다.

---

## 경미

### M1. 만병통치제류를 헤롱헤롱만 걸린 전투 포켓몬에게 쓰면 "혼란이 풀렸다"가 나온다 (upstream 동일 버그)

- 위치: `src/battle_util.c:10283-10290` `ItemHealMonVolatile()` (C, 파트 C). 출력은 `data/battle_scripts_2.s` `BattleScript_CureStatus_Battler`의 `printfromtable gCureStatusStringIds`
  ```c
  if (effect[3] & ITEM3_STATUS_ALL)
  {
      statusChanged = (gBattleMons[battler].volatiles.infatuation || ...confusion...);
      ...
      gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_CURED_CONFUSION;   // 무조건 혼란
  }
  ```
- 실패 시나리오(추정, 코드 흐름으로 판단): 상대의 헤롱헤롱으로 헤롱헤롱 상태이고 상태이상과 혼란이 없는 전투 포켓몬에게 만병통치제(또는 회복약 등 `ITEM3_STATUS_ALL` 도구)를 쓴다. `HealStatusConditions()`는 상태이상이 없어 MULTISTRING을 덮지 않는다 → `STRINGID_SCRCUREDCONFUSION` "`{이름}의\n혼란이 풀렸다!`"가 출력된다. HEAD에서는 "`{종족}은(는)\n건강해졌다!`"였다. 혼란과 헤롱헤롱이 함께 있으면 혼란 문장만 나오고, 상태이상과 혼란이 함께 있으면 상태이상 문장만 나온다.
- upstream `32fcd64868`·`1.17.0`이 같다(확인). D4 15번에 포함되지만, `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 작업 트리 행은 "헤롱헤롱 `STRINGID_PKMNGOTOVERITSINFATUATION`"이라고 적는다. 실제로 그 문장이 나오는 도구는 빨강비드로(`ITEM0_INFATUATION`, `src/data/pokemon/item_effects.h:167`)뿐이다.
- 제안: (a) 출력 변경 문서에 "만병통치제류는 휘발 상태를 항상 혼란 문장으로 표시(upstream 동일)"라고 적는다. 또는 (b) HnS 수정으로 헤롱헤롱만 풀렸을 때 `B_MSG_CURED_INFATUATION`을 고르게 한다. (b)는 HnS 정책을 새로 정하는 일이라 친구의 결정이 필요하다. 실기 확인 후보.

### M2. 정의 없는 `extern BattleScript_DefogClearHazards`가 남아 있다

- 위치: `include/battle_scripts.h:296`. 정의(`data/battle_scripts_1.s`)와 C 참조는 모두 없다(`git grep` 1건 = 이 선언뿐).
- 영향: 없음. 누군가 참조하면 링크 오류로 바로 드러난다. upstream도 같은 줄을 남겨 두었다(1.17.0까지).
- 제안: 그대로 두거나, upstream이 지우는 시점에 맞춘다. 기록만 한다.

### M3. HnS `BattleScriptPush`의 오버플로 검사가 실제 스택 크기와 맞지 않는다(기존 문제). 멘탈허브는 upstream보다 호출을 1단계 더 쓴다

- 위치: `src/battle_util.c:1298` `assertf(... size < UINT8_MAX ...)`, `include/battle.h:275` `const u8 *ptr[8];`. upstream 1.17.0은 `ARRAY_COUNT(ptr)`로 검사하고 넘치면 바로 돌아간다.
- 이번 변경: `BattleScript_MentalHerbCureRet`/`...Fling`이 `call BattleScript_MentalHerbCureMessages`로 한 단계를 더 쓴다. 추정 최대 깊이는 Fling 경로 3(SetMoveEffect의 `BattleScriptPush` → `BattleScriptCall` → `call`), Ret 경로 2~3이다. 8칸 안이므로 지금은 안전하다고 추정한다.
- 제안: 별건으로 기록한다(검사식을 `ARRAY_COUNT`로 바꾸는 것은 이 unit의 범위가 아니다).

### M4. (범위 밖 참고) 주석·문서 표기

- `include/constants/battle_string_ids.h:1270` 주석 "`gBreakScreensStringIds/enum BreakScreensStringID are not added`"가 오해를 부를 수 있다. 같은 파일 `:874`에 HnS의 같은 이름 `enum BreakScreensStringID`(값형 1,2,4)가 있으므로 "upstream의 인덱스형 enum은 넣지 않았다"로 읽히게 고치면 좋다(파트 A 파일).
- `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 작업 트리의 멘탈허브 D6-b 행은 "(`// HnS:`)"라고 적지만, 스크립트 주석은 `@ HnS:`(`battle_scripts_1.s:7280`, `:7292`)다.
- 둘 다 동작에는 영향이 없다.

---

## 확인만

### C1. 함정 1 — 멘탈허브 비트마스크 (Ret·Fling)

- `CMP_BITMASK` = 6(`include/constants/battle_script_commands.h:350`). 비교 구현: `Cmd_jumpifbyte`/`jumpifhalfword`/`jumpifword`의 `case CMP_BITMASK: if (*ptr & (1 << value)) jump;`(`battle_script_commands.c:4811`, `:4855`, `:4899`). 값은 "비트 번호"로 해석한다(확인).
- C 쪽 비트: `TryMentalHerb()`(`battle_hold_effects.c:428`)가 시작할 때 `MULTISTRING = 0`으로 두고, 효과마다 `|= 1 << B_MSG_MENTALHERBCURE_*`(`:434`, `:443`, `:451`, `:458`, `:466`, `:473`)를 더한다. enum은 INFATUATION 0 · TORMENT 1 · DISABLE 2 · HEALBLOCK 3 · ENCORE 4 · TAUNT 5(`battle_string_ids.h:1149-1157`)다. 스크립트도 같은 enum 이름을 `jumpifbyte CMP_BITMASK`의 value로 쓴다. 비트 번호가 일치한다(확인).
- `Fling`/`Ret` 분기: `battle_hold_effects.c:481-484`(`timing == IsOnFlingActivation`). Ret과 Fling 모두 `call BattleScript_MentalHerbCureMessages`(`battle_scripts_1.s:7242`, `:7250`)를 쓴다. `printfromtable gMentalHerbCureStringIds`는 남아 있지 않다. 그래서 비트마스크 값으로 표를 읽어 범위 밖으로 나가는 경로가 없다.
- **시뮬레이션(확인)**: 스크립트 원문을 파싱해 Ret·Fling × 마스크 1~63 전부를 실행했다(scratchpad `sim.py`). 결과는 126가지 모두 다음과 같았다.
  - 켜진 비트의 문장이 헤롱헤롱 → 트집 → 사슬묶기 → 회복봉인 → 앙코르 → 도발 순서로 한 번씩 나온다. 빠지거나 중복된 문장은 없다.
  - 모든 분기가 `...RetFinish`의 `return`으로 호출자(Ret/Fling 본문)에 돌아간다. 이후 Ret은 `updatestatusicon` → `removeitem` → `return`, Fling은 `updatestatusicon` → `return` 순서로 진행한다.
  - 공격자 저장 스택이 0으로 끝나고 공격자 값이 원래대로 복원된다.
  - 런타임 설정 `CONFIG_B_MENTAL_HERB < GEN_5`이면 헤롱헤롱 문장 뒤 바로 Finish로 간다(upstream과 같다). C의 컴파일 시점 `B_MENTAL_HERB`와 스크립트의 런타임 설정이 어긋날 수 있는 것은 테스트 설정 변경 때뿐이고, upstream도 같다(`config_changes.h:181` "TODO: use in tests").

### C2. D6b — 사슬묶기·앙코르 문장의 이름

- `BattleScript_MentalHerbCuresDisable`(`:7279`)·`...Encore`(`:7291`): `saveattacker` → `copybyte gBattlerAttacker, sBATTLER` → `printstring` → `waitmessage` → `restoreattacker` → `goto` 다음 검사. 분기 없이 같은 블록에서 짝이 맞는다.
- `sBATTLER`가 보유자인지(확인):
  - `ItemBattleEffects()` 끝 `gBattleScripting.battler = gPotentialItemEffectBattler = itemBattler;`(`battle_hold_effects.c:1192`).
  - Ret 경로 호출자: `MoveEndItemEffectsTarget`(`battle_move_resolution.c:2504`, itemBattler = 대상), `...Attacker1`(`:2517`, itemBattler = 공격자). 멘탈허브의 발동 시점 플래그는 `onTargetAfterHit`·`onAttackerAfterHit`·`onFling`뿐이다(`src/data/hold_effects.h:138-143`). `IsForceTriggerItemActivation`에는 걸리지 않는다.
  - Fling 경로: `SetMoveEffect` FLING → `ItemBattleEffects(effectBattler, 0, holdEffect, IsOnFlingActivation)`(`battle_script_commands.c:3338`). 이후 `SetMoveEffect`·`Cmd_setadditionaleffects`(`:3918-3960`)는 `gBattleScripting.battler`를 바꾸지 않는다.
  - 따라서 두 경로 모두 문장 출력 시점에 `sBATTLER`는 허브 보유자다. 같은 블록의 `playanimation`/`updatestatusicon`/`removeitem BS_SCRIPTING`도 이 값에 기대고 있다.
- 복원 뒤 흐름: Fling 경로는 `return` 뒤 `BattleScript_RemoveItem`(`:561`, `removeitem BS_ATTACKER`)으로 간다. `restoreattacker`가 그보다 먼저 실행되므로 던진 쪽의 도구가 정상적으로 지워진다. 공격자가 보유자인 경우(`...Attacker1`)는 같은 값을 복사하므로 아무 변화가 없다.
- 저장 스택: `savedBattlerAttacker[5]`(`include/battle.h:639`). 한 번에 1칸만 쓰고 곧바로 비운다. 멘탈허브가 발동하는 이동 종료·Fling 효과 단계에서 이미 저장이 걸려 있을 수 있는 곳은 레드카드(`battle_move_resolution.c:3363`) 정도이고, 그 스크립트가 직접 복원한다. 5칸을 넘을 경로는 찾지 못했다(추정).
- `printstring`의 부작용: `PrepareStringBattle(id, gBattlerAttacker)`(`battle_util.c:1180`)에서 이 두 ID는 특별 처리 대상이 아니다. 컨트롤러만 보유자 쪽으로 바뀌며, 이는 기존 HnS `BattleScript_ScriptingAbilityStatRaise`(`:6875`)·`CheekPouchActivates`와 같은 패턴이다.
- 턴 종료: `BattleScript_DisabledNoMore`(`:4846`)·`EncoredNoMore`(`:4865`) 스크립트, 문자열(`battle_message.c:315`, `:318`, `{B_ATK_NAME_WITH_PREFIX}`), C 핸들러(`battle_end_turn.c:757-758`, `:788-789`)가 모두 diff에 없다. 턴 종료 루프의 `battler = gBattlerAttacker = ...`(`:1536`)도 그대로이므로 출력이 바뀌지 않는다(확인).
- 참고: upstream은 #7857(`eee546df06`, #9655 이전)부터 이 두 영문 문자열에 `{B_SCR_NAME_WITH_PREFIX}`를 쓴다. HnS 한글은 `{B_ATK}`를 유지한다(D6b 결정). 나중에 한글 토큰을 `{B_SCR}`로 바꾸게 되면 D6b의 6줄은 필요 없어진다. 턴 종료 핸들러는 이미 `gBattleScripting.battler = battler`를 설정한다.

### C3. 함정 2 — BreakScreens

- `BattleScript_BreakScreens`/`BreakScreensMessages`(`:3778-3810`)는 diff에 없다. `CMP_COMMON_BITS`와 값형 `B_MSG_BREAK_*`(1,2,4, `battle_string_ids.h:874-879`)를 그대로 쓴다.
- 값을 만드는 C 코드도 값형 OR이다: `battle_script_commands.c:3718-3722`, `battle_util.c:9233-9249`(`TryRemoveScreens`).
- `CMP_BITMASK`는 `git grep` 기준 멘탈허브 6줄에서만 쓴다. 섞인 곳은 없다(확인).

### C4. 함정 5 — HnS 전용 스크립트 보존

HEAD와 비교해 바뀌지 않은 것(확인): `HealerActivates`(`:6311`, `STRINGID_HEALERCURE`), `DampPreventsAftermath`(`:5696`, 문구 없음), `RanAwayUsingMonAbility`(`:4377`, `copybyte`+팝업+`PKMNFLEDUSING`), `LumBerryCureStatusRet`(`:7201`, 아이템 팝업+반복), `BerryCureStatusRet`(`:7193`, 아이템 팝업), `BreakScreens`, `SolarPowerActivates`(`:6304`, 문구 없음). `RockyHelmetActivates`(`:7021`)는 `call BattleScript_ItemPopUp_ScriptingNoFlush`를 유지하고 도구 애니메이션 5줄만 지웠다(D4 14번). `PoisonHealActivates`(`:5742`)는 계속 문구가 없고 `B_ANIM_SIMPLE_HEAL` 2줄만 추가했다(D4 13번, upstream과 줄 단위로 같음).

### C5. #9856 / 함정 6 — `BattleScript_HurtAttacker`

- `:7008-7014`: `printfromtable gHurtByStringIds`를 한 번만 출력한다. 중복 `printstring STRINGID_AFTERMATHDMG`는 없다. upstream `c1eaced09e`·1.17.0과 줄 단위로 같다(HP 매크로 인자 차이는 HnS 기존 그대로).
- 호출 경로는 3개 스크립트이고, C 호출자 4곳 모두 MULTISTRING을 설정한다(확인).
  - `RoughSkinActivates` ← `battle_util.c:4069` `= B_MSG_HURT`
  - `RockyHelmetActivates` ← `battle_hold_effects.c:258` `= B_MSG_HURT_BY_ITEM`
  - `JabocaRowapBerryActivates` ← `:355`, `:379` `= B_MSG_HURT_BY_ITEM`
- 설정 이후 `HurtAttacker`에 이르기까지 지나는 `AbilityPopUp`·`ItemPopUp_ScriptingNoFlush`·`jumpifability`·`playanimation`은 MULTISTRING을 건드리지 않는다. `gHurtByStringIds`(`battle_message.c:1510-1514`)는 0·1만 있고, 두 한글 문장(`:388`, `:618`)은 바이트가 같다. 보이는 출력은 바뀌지 않는다.

### C6. 토큰과 배틀러 설정 (점검 6)

| 스크립트 | 문자열 / 토큰 | 출력 시점 배틀러 | 판정 |
|---|---|---|---|
| `RemoveHazards`(`:5114`) | `gRemoveHazardsStringIds` → `*DISAPPEAREDFROMTEAM` `{B_ATK_TEAM2}` | 고속스핀 `Cmd_rapidspinfree`(`:9631`, 공격자=사용자), 안개제거 `DefogClearHazards`(`:7278`, 기존 HnS 공격자 처리 그대로). MULTISTRING = `hazardType`(1~5, 표가 모두 채움) | 확인. HEAD의 Spin/Defog 표와 값이 같다. 강철(`PKMNBLEWAWAYSHARPSTEEL` ↔ `SHARPSTEELDISAPPEAREDFROMTEAM`)도 바이트가 같다(`battle_message.c:827-828`). YOU 분기는 없다 |
| `HealBlockEndTurn`(`:5988`) | `HEALBLOCKEDNOMORE` `{B_SCR}` | `battle_end_turn.c:838` `gBattleScripting.battler = battler` | 확인 |
| `IntimidatePrevented`(`:6388`) | `PKMNATKNOTLOWERED` `{B_SCR}` | 스크립트 첫 줄 `copybyte sBATTLER, gBattlerTarget`(위협을 받은 포켓몬) | 확인 |
| `SoundproofProtected`(`:6735`) | `SCR_ITDOESNTAFFECT` `{B_SCR}` | `CanAbilityBlockMove`… `battle_util.c:2474` `gBattleScripting.battler = gBattlerAbility = ctx->battlerDef` | 확인 |
| `MonMadeMoveUseless`(`:6689`) | 같음 | 같은 함수(`:2474`) | 확인. 스크립트 안의 다른 사용처 `BattleScript_VoltAbsorbHeal`(`:2805`)은 참조가 없는 죽은 라벨이다 |
| `HealBellSoundproof`(`:3074`) | `SCR_ITDOESNTAFFECT` `{B_SCR}` | `Cmd_healpartystatus` `:9300` `gBattleScripting.battler = partner` | 확인. 동료 문장에 동료 이름이 나온다. ATTACKER 비트 분기는 Gen6~7 설정에서만 생기고 그때는 동료 이름이 나오지만, HnS `B_HEAL_BELL_SOUNDPROOF = GEN_LATEST`(`include/config/battle.h:144`)라 도달할 수 없다. upstream도 같다 |
| `SturdyPreventsOHKO`(`:6654`) | `ITDOESNTAFFECT` `{B_DEF}` | `gBattlerTarget` = 옹골참 보유자(`battle_script_commands.c:1208-1212`) | 확인. `PrepareStringBattle`이 `TryInitializeTrainerSlideEnemyMonUnaffected`를 부르게 되지만 HnS에는 해당 슬라이드 데이터가 없어 보이는 변화가 없다(`git grep` 0) |
| `AbilityHpHeal`(`:6265`) | 문구 없음, `B_ANIM_SIMPLE_HEAL`(`battle_anim.h:597`, 존재 확인) | `BS_ATTACKER` = 젖은접시(`battle_util.c:3710`) / 볼주머니(`saveattacker`+`copybyte`) | 확인 |
| `PoisonHealActivates`(`:5742`) | 문구 없음 | `battle_end_turn.c` 루프 공격자 | 확인 |
| 가방 `ItemCureStatus`(`scripts_2:90`) | `gPartyCureStatusStringIds` → `PARTYCURED*` `{B_BUFF1}` | `BS_ItemCureStatus` `:12383` `PREPARE_SPECIES_BUFFER` (종족명) | 확인. 대기 포켓몬은 종족명, HEAD와 같다. MULTISTRING은 `HealStatusConditions`가 0~5로 설정한다(대기 경로에서는 상태이상만 바뀐다). N1 참고 |
| 가방 `CureStatus_Battler`(`scripts_2:99`) | `gCureStatusStringIds` → `SCRCURED*` 등 `{B_SCR}` | `:12390` `gBattleScripting.battler = targetBattler`(복식 동료 포함) | 확인. 전투 중 포켓몬은 별명이 나온다(D4 15번). M1 참고 |
| 상태회복 표 6곳(TakeHeart·JungleHealing·PsychoShift·Refresh·ShedSkin·AbilityCuredStatus) | `gCureStatusStringIds` `{B_SCR}` | `Cmd_curestatuswithmove` `:9847`, `BS_CureStatus` `:14776`, 탈피 `battle_util.c:3744`, 특성 회복 `:9135` | 확인. `GetCuredStatusMessage()`의 PROBLEM 반환도 표 안에 있다(`battle_message.c:1493-1494`) |

### C7. upstream hunk 누락·제외 판정 (점검 7)

- upstream #9655의 해당 4파일 hunk 41개(scripts_1 35, scripts_2 1, battle_scripts.h 4, commands.h 1)가 모두 part-B 표에 있다. "그대로" 판정은 작업 트리와 줄 단위로 대조해 같았다(`AbilityHpHeal`의 ` \t` → tab 정리만 다름).
- 제외 6건의 근거는 타당하다(확인).
  - #6 BreakScreens: HnS에 같은 기능이 있다(C3). 넣으면 이름과 의미가 충돌한다.
  - #8 RanAway: D3. HnS는 이미 팝업과 `copybyte`를 가지고 있다.
  - #11 DampPreventsAftermath: HnS 출력 정책(HEAD 출력 문서 48행)상 문구가 없다.
  - #17 SolarPower: HnS에서 이미 지운 상태라 결과가 같다.
  - #34·h#3 `BerryCureStatusAndConfusionRet`: HnS 리샘 반복(`BS_TryPrintNextLumBerryCureStatus` `:12430-12434` 혼란 비트 포함)이 같은 일을 한다. C 참조가 0건이다.
  - #18 Healer 부분 제외: HEAD 출력 문서 57행의 `HEALERCURE` 정책이다.
- 지운 라벨과 표의 잔여 참조(`git grep -w`, src·include·data·test·asm, 확인):
  - `BattleScript_SpinHazardsAway`·`FlinchPrevention`·`RockyHelmetActivatesDmg`, `gSpin/gDefogHazardsStringIds`, `gStatusCureStringIds`, `B_MSG_CURED_FREEEZE`는 0건이다.
  - `BattleScript_DefogClearHazards`는 extern 1건만 남았다(M2).
  - 스크립트에서 쓰지 않게 된 STRINGID(`PKMNSXBLOCKSY`·`PKMNPROTECTEDBY`·`PKMNPREVENTSUSAGE`·`PKMNSXMADEYUSELESS`·`PKMNSXPREVENTSFLINCHING`·`*ABILITYSTAT*` 등)는 enum과 표에 남아 있다. 이들은 `test/text.c`와 `src/battle_arena.c:420-433`(배틀아레나 감점)에서만 참조한다. 아레나 감점이 이 상황들에서 더는 일어나지 않지만 upstream도 같다. HnS에서 배틀아레나를 쓰는지는 미확인이다.
- 출력 판단 보충(확인): `PrepareStringBattle`(`battle_util.c:1189-1199`, `:1233`)의 청개구리 문장 교체와 오기·승기 훅이 이제 `ATTACKERSSTATROSE`/`DEFENDERSSTATFELL`/`DEFENDERSSTATROSE`에 걸린다. 깨어진갑옷·불굴의마음 보유자는 같은 시점에 청개구리·오기·승기를 함께 가질 수 없으므로 출력 변화가 없다(upstream 동일).

### C8. 기타 정적 점검

- 라벨 중복 정의는 없다(기존 `.if`/`.else`의 `BattleScript_LocalBattleLostEnd` 제외).
- 새 라벨(`...CureMessages`, `...RetX` 7개, `...CuresX` 6개, `RemoveHazards`, `HealBlockEndTurn`)은 각각 한 번만 정의된다. `BattleScript_RemoveHazardsRet:`(`:5117`)는 YOU 분기를 뺀 뒤 참조가 없는 로컬 라벨이다. upstream 문맥을 맞추려고 둔 것으로 보이며 무해하다.
- 한글 줄 변경은 0건이다. 이 4개 파일에는 비ASCII 문자가 추가되지 않았다.

## 실기 확인 후보(이 리뷰에서 추가·강조)

1. 헤롱헤롱만 걸린 전투 포켓몬에게 만병통치제를 사용한다(M1). "혼란이 풀렸다"가 나오는지 본다.
2. 멘탈허브로 상대가 쓴 사슬묶기·앙코르를 풀고, 내던지기(멘탈허브)를 맞힌다. 두 경우 모두 보유자 이름이 나오는지, 내던지기 쪽의 도구가 사라지는지 본다(C2).
3. 대기 포켓몬에게 해독제를 쓴다(종족명 문장). 필드에서 해독제를 쓴다(N1 보호를 넣은 경우 회귀 확인).
