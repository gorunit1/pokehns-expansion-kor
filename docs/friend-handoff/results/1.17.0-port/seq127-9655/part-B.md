# part-B (배틀 스크립트) — seq 127 #9655 + #9856 사전 분석·이식안

- 기준: HnS HEAD `fcf855d4e8`, upstream #9655 `32fcd64868`, #9856 `c1eaced09e`.
- 담당 파일: `data/battle_scripts_1.s`, `data/battle_scripts_2.s`, `include/battle_scripts.h`, `include/constants/battle_script_commands.h`
- 결과: `part-B.patch`(4파일, +97/−53, hunk 35개). `git diff --check` 통과. `git -C /home/jinmo/pokehns-expansion-kor apply --check part-B.patch` 통과.
- 빌드·테스트는 돌리지 않았다(BRIEF 규칙). 아래 판정은 코드 대조만으로 한 것이다.
- 이 patch에는 한글이 든 줄이 없다(비ASCII 0줄).

## 1. upstream hunk 표 (41개)

집계: 그대로 23 / 수정해서 12(1개는 일부 제외) / 제외 6. dry-run 실패 14개는 모두 수정하거나 제외했다. `battle_scripts_1.s` #34는 `patch`에서는 fuzz 2로 붙지만 `git apply`에서는 거부되어 실질적으로는 실패 15개다.

### data/battle_scripts_1.s (35)

| # | upstream 위치 / 스크립트 | 내용 | dry | 판정 | 이유 |
|---|---|---|---|---|---|
| 1 | 386 `EffectTakeHeart` | `PKMNSTATUSNORMAL` → `printfromtable gCureStatusStringIds` | FAIL | 수정해서 | HnS는 이미 `printfromtable gStatusCureStringIds`. 표 이름만 upstream으로 바꿈 |
| 2 | 756 `JungleHealingCureStatus` | 같음 | FAIL | 수정해서 | 같음 |
| 3 | 1273 `EffectPsychoShiftCanWork` | 같음 | FAIL | 수정해서 | 같음 |
| 4 | 2595 `EffectRest` | `printfromtable gRestUsedStringIds` → `printstring STRINGID_PKMNSLEPTHEALTHY` | ok | 그대로 | HnS 표의 두 칸이 모두 `PKMNSLEPTHEALTHY`라 출력이 같음 |
| 5 | 3030 `HealBellSoundproof` | `PKMNSXBLOCKSY` ×2 → `SCR_ITDOESNTAFFECT` | ok | 그대로 | D4 5번. `healpartystatus`가 `gBattleScripting.battler = partner` 설정(`battle_script_commands.c:9290`) |
| 6 | 3718 `BreakScreens` | 비트마스크(`CMP_BITMASK`) 순차 출력 + TO-DO 주석 | FAIL | **제외** | HnS `BattleScript_BreakScreens`/`BreakScreensMessages`(:3778~)가 이미 같은 기능을 `CMP_COMMON_BITS` + 값형 `B_MSG_BREAK_*`(1,2,4)로 구현함. 함정 2 |
| 7 | 3797 `EffectRefresh` | 1번과 같음 | FAIL | 수정해서 | 표 이름만 바꿈 |
| 8 | 4278 `RanAwayUsingMonAbility` | 팝업 + `GOTAWAYSAFELY` | FAIL | **제외** | D3. HnS `copybyte`·팝업·`PKMNFLEDUSING`(`무사히 도망쳤다\p`) 유지 |
| 9 | 4969 `SpinHazardsAway`/`DefogClearHazards` → `RemoveHazards` | 통합 + 플레이어 측 끈적끈적네트 `STICKYWEBDISAPPEAREDFROMYOU` 분기 | ok | 수정해서 | YOU 분기를 뺌(`@ HnS:` 주석). 5절 참고 |
| 10 | 5296 `SturdiedMsg` | `ENDUREDSTURDY` → `PKMNENDUREDHIT` | ok | 그대로 | 한글 본문 동일 |
| 11 | 5557 `DampPreventsAftermath` | `PKMNSABILITYPREVENTSABILITY` → `SCR_ITDOESNTAFFECT` | FAIL | **제외** | HnS는 이 스크립트에서 문구 자체를 지움(출력 변경 문서, 함정 5) |
| 12 | 5602 `PoisonHealActivates` | 문구 삭제 + `B_ANIM_SIMPLE_HEAL` | FAIL | 수정해서 | 문구는 HnS에서 이미 지움. 애니메이션 2줄만 추가(D4 13번) |
| 13 | 5674 `TryActivateSteadFast` | `ATTACKERABILITYSTATRAISE` → `ATTACKERSSTATROSE` | ok | 그대로 | 한글 본문 동일 |
| 14 | 5797 `TargetPoisonHealed` | `PASTELVEILENTERS` → `PKMNHEALEDPOISON` | ok | 그대로 | ID 이름만 바뀜(파트 A가 이름 변경, 본문 동일) |
| 15 | 5838 `HealBlockEndTurn` 신규 | `HEALBLOCKEDNOMORE` 출력 | ok | 그대로 | D4 2번. 파트 C가 턴 종료에서 연결 |
| 16 | 6082 `AbilityHpHeal` | 문구 삭제 + `B_ANIM_SIMPLE_HEAL` | ok | 수정해서 | upstream 줄 앞의 `space+tab`을 tab으로 정리(`git diff --check` 통과용). 그 밖에는 그대로. D4 12번 |
| 17 | 6123 `SolarPowerActivates` | 문구 삭제 | FAIL | **제외** | HnS에서 이미 지움(같은 결과) |
| 18 | 6132 `HealerActivates` + `ShedSkinActivates` | 둘 다 `printfromtable gCureStatusStringIds` | FAIL | 수정해서(일부 제외) | Healer는 제외(HnS `STRINGID_HEALERCURE` 유지, 함정 5). ShedSkin은 표 이름만 바꿈 |
| 19 | 6208 `IntimidatePrevented` | `PKMNPREVENTSSTATLOSSWITH` → `PKMNATKNOTLOWERED` | ok | 그대로 | D4 9번. 새 ID는 파트 A |
| 20 | 6464 `TookAttack` | 특성 팝업 추가 | ok | 그대로 | D4 8번. HnS `CancelerTookAttack`(`battle_move_resolution.c:1757`)에는 팝업이 없어 중복 없음 |
| 21 | 6471 `SturdyPreventsOHKO` / `DampStopsExplosion` | `PKMNPROTECTEDBY` → `ITDOESNTAFFECT`, `PKMNPREVENTSUSAGE` → `POKEMONCANNOTUSEMOVE` | ok | 그대로 | 옹골참은 D4 6번. 습기 폭발 방지는 한글 본문 동일 |
| 22 | 6506 `MonMadeMoveUseless` | `PKMNSXMADEYUSELESS` → `SCR_ITDOESNTAFFECT` | ok | 그대로 | 한글 본문 동일, 토큰만 `B_DEF`→`B_SCR`. `battle_util.c:2474`가 `gBattleScripting.battler = battlerDef` 설정. 스크립트 내 사용처(:2807 `VoltAbsorbHeal`)는 참조 없는 죽은 라벨 |
| 23 | 6541 `FlinchPrevention` 삭제 | 정신력 풀죽음 방지 스크립트 삭제 | ok | 그대로 | D4 7번. **파트 C가 `SetMoveEffect` 참조를 지워야 링크됨** |
| 24 | 6559 `SoundproofProtected` | `PKMNSXBLOCKSY` → `SCR_ITDOESNTAFFECT` | ok | 그대로 | D4 4번. `battle_util.c:2474`가 스크립팅 배틀러 설정 |
| 25 | 6688 `BattlerAbilityStatRaiseOnSwitchIn` | `SCRIPTINGABILITYSTATRAISE` → `SCRIPTINGSTATROSE` | ok | 그대로 | D4 10번(불굴의검·불굴의방패·바람타기·면영) |
| 26 | 6699 `ScriptingAbilityStatRaise` | `ATTACKERABILITYSTATRAISE` → `ATTACKERSSTATROSE` | ok | 그대로 | 한글 본문 동일 |
| 27 | 6717 `WeakArmorDefPrintString` | `TARGETABILITYSTATLOWER` → `DEFENDERSSTATFELL` | ok | 그대로 | D4 11번 |
| 28 | 6735 `WeakArmorSpeedPrintString` | `TARGETABILITYSTATRAISE` → `DEFENDERSSTATROSE` | ok | 그대로 | 한글 본문 동일 |
| 29 | 6754 `AttackerAbilityStatRaise` | `ATTACKERABILITYSTATRAISE` → `ATTACKERSSTATROSE` | ok | 그대로 | 한글 본문 동일 |
| 30 | 6826 `HurtAttacker` | `printfromtable gHurtByStringIds` + 중복 `printstring STRINGID_AFTERMATHDMG` | ok | 수정해서 | 함정 6/#9856. 중복 줄을 빼서 upstream #9856 뒤 형태와 같게 함 |
| 31 | 6837 `RockyHelmetActivates` | 도구 애니메이션 삭제 | FAIL | 수정해서 | HnS `call BattleScript_ItemPopUp_ScriptingNoFlush` 줄은 유지하고 애니메이션 5줄만 삭제. D4 14번 |
| 32 | 6850 `SpikyShieldEffect` | `PKMNHURTSWITH` → `AFTERMATHDMG` | ok | 그대로 | 한글 본문 동일 |
| 33 | 6949 `AbilityCuredStatus` | `printfromtable gCureStatusStringIds` | FAIL | 수정해서 | 표 이름만 바꿈 |
| 34 | 7019 `BerryCureStatusAndConfusionRet` 신규 | 상태+혼란 동시 회복 열매 스크립트 | fuzz2(git apply 거부) | **제외** | HnS 리샘 루프(`LumBerryCureStatusRet` + `trynextlumberrycurestatus`) 유지. upstream `TryCureAnyStatus` 재작성은 파트 C에서 제외 대상이라 호출자 없음. 넣으면 HnS 아이템 팝업도 빠짐 |
| 35 | 7044 `MentalHerbCureRet` | 표 출력 → 효과별 `CMP_BITMASK` 분기 | FAIL | 수정해서 | 2·3절. Ret/Fling이 같은 비트마스크 서브루틴을 씀 + D6b |

### data/battle_scripts_2.s (1)

| # | 위치 | 내용 | dry | 판정 | 이유 |
|---|---|---|---|---|---|
| 1 | 91 `ItemCureStatus` / `CureStatus_Battler` | `ITEMCUREDSPECIESSTATUS` → `printfromtable gPartyCureStatusStringIds` / `gCureStatusStringIds` | ok | 그대로 | D4 15번. 표와 MULTISTRING은 파트 A·C |

### include/battle_scripts.h (4)

| # | 내용 | dry | 판정 | 이유 |
|---|---|---|---|---|
| 1 | `SpinHazardsAway` → `RemoveHazards` 선언 | ok | 그대로 | `DefogClearHazards` 선언(:296)은 upstream도 남겨 둠(1.17.0까지 유지). 정의가 없어도 참조가 없으면 무해 |
| 2 | `FlinchPrevention` 선언 삭제 | ok | 그대로 | |
| 3 | `BerryCureStatusAndConfusionRet` 선언 추가 | FAIL | **제외** | #34 제외와 같은 이유 |
| 4 | `HealBlockEndTurn` 선언 추가 | ok | 그대로 | |

### include/constants/battle_script_commands.h (1)

| # | 내용 | dry | 판정 |
|---|---|---|---|
| 1 | `#define CMP_BITMASK 6` | ok | 그대로. 실제 비교 구현(`*ptr & (1 << value)`)은 파트 C |

### #9856 (`9856.patch`)

- 흡수됨. #30을 넣을 때 중복 `printstring STRINGID_AFTERMATHDMG`를 처음부터 넣지 않았다. 결과는 upstream `c1eaced09e`의 `BattleScript_HurtAttacker`와 줄 단위로 같다. 확인: 결과 트리에서 `git apply --check -R 9856.patch`가 통과한다(이미 적용 상태).
- seq 174에 닿으면 `이미 적용(#9655 unit에서 흡수)`으로 처리한다.
- 출력: HnS 한글 `STRINGID_AFTERMATHDMG` = `STRINGID_PKMNHURTSWITH` = `{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n상처를 입었다!`(바이트 동일)이다. 그래서 MULTISTRING이 0이든 1이든 지금의 단일 출력과 같다. 단, MULTISTRING이 표 범위(0·1) 안에 있어야 한다(4절 계약 C-6).

## 2. HnS 보존·적응 (`@ HnS:` 주석 위치는 patch 적용 뒤 줄 번호)

| 위치 | 내용 |
|---|---|
| `battle_scripts_1.s:5115` `BattleScript_RemoveHazards` | `@ HnS:` YOU 분기 없음. 모든 장판이 `gRemoveHazardsStringIds`(`*DISAPPEAREDFROMTEAM`) 출력 |
| `:7247` `BattleScript_MentalHerbCureFling` | `@ HnS:` Fling 경로 유지(아이템이 이미 없어서 `removeitem` 없음). Ret과 같은 비트마스크 서브루틴 `BattleScript_MentalHerbCureMessages`를 `call` |
| `:7280` `BattleScript_MentalHerbCuresDisable` | `@ HnS:` D6b. `saveattacker` / `copybyte gBattlerAttacker, sBATTLER` / 출력 / `restoreattacker` |
| `:7292` `BattleScript_MentalHerbCuresEncore` | `@ HnS:` D6b. 위와 같음 |
| 주석 없음(보존) | `BreakScreens`·`BreakScreensMessages`(`CMP_COMMON_BITS`), `RanAwayUsingMonAbility`, `DampPreventsAftermath`, `HealerActivates`(`HEALERCURE`), `SolarPowerActivates`, `PoisonHealActivates` 무문구, `LumBerryCureStatusRet`, `BerryCureStatusRet`·`RockyHelmetActivates`의 ItemPopUp 줄, `gPurifyStatusCureStringIds`를 쓰는 `EffectPurify`. 모두 그대로 둠 |

멘탈허브 구조(함정 1):
- `BattleScript_MentalHerbCureRet::` = 애니메이션 → `call BattleScript_MentalHerbCureMessages` → `updatestatusicon` → `removeitem` → `return`
- `BattleScript_MentalHerbCureFling::` = 애니메이션 → `call ...Messages` → `updatestatusicon` → `return`
- `BattleScript_MentalHerbCureMessages:`(로컬)부터 upstream 라벨(`...RetInfatuation`~`...RetFinish`, `...CuresX`)과 순서를 그대로 쓴다. `...RetFinish`는 `return`만 한다. HnS `BreakScreensMessages`와 같은 방식이다. `printfromtable gMentalHerbCureStringIds`는 두 곳 모두에서 없앴다. 그래서 비트마스크 값으로 표를 읽는 경로가 남지 않는다.
- 출력 순서: 헤롱헤롱 → 트집 → 사슬묶기 → 회복봉인 → 앙코르 → 도발. 효과마다 한 문장(D4 3번). 헤롱헤롱 뒤 `jumpifgenconfiglowerthan CONFIG_B_MENTAL_HERB, GEN_5`(태그 `CONFIG_B_MENTAL_HERB`는 `include/constants/config_changes.h:181`에서 생성됨)는 upstream 그대로다.

D6b 근거:
- 명령 존재: `saveattacker`/`restoreattacker` 매크로(`asm/macros/battle_script.inc:1217-1223` → `BS_SaveAttacker`/`BS_RestoreAttacker`, `battle_script_commands.c:11970/11977`). 저장 스택은 `savedBattlerAttacker[5]`(`include/battle.h:639`). `copybyte`는 `:1629`.
- 같은 패턴을 이미 쓰는 HnS 스크립트: `BattleScript_ScriptingAbilityStatRaise`(`saveattacker` → `copybyte gBattlerAttacker, sBATTLER` → `printstring` → `waitmessage` → `restoreattacker`), `BattleScript_CheekPouchActivates`
- `sBATTLER` = 멘탈허브 보유자: `ItemBattleEffects()`가 `gBattleScripting.battler = itemBattler`를 설정한다(`battle_hold_effects.c:1189`). Fling 경로도 `ItemBattleEffects(effectBattler, 0, holdEffect, IsOnFlingActivation)`(`battle_script_commands.c:3344`)를 거쳐 같은 줄을 지난다. 그래서 두 경로 모두 `{B_ATK}`가 실제로 풀린 포켓몬을 가리킨다. 출력 직후 공격자를 복원하므로 Fling 기술 스크립트의 나머지 흐름은 바뀌지 않는다.
- 턴 종료 경로와의 관계: `BattleScript_DisabledNoMore`·`BattleScript_EncoredNoMore`(:4846/:4865)는 문자열 ID만 공유하고 스크립트는 따로 있다. 이 patch는 두 스크립트를 건드리지 않는다. 턴 종료 루프가 `battler = gBattlerAttacker = ...`(`battle_end_turn.c:1537`)로 대상을 공격자로 두므로 출력은 지금과 같다.
- 문자열(`PKMNMOVEDISABLEDNOMORE`·`PKMNENCOREENDED`의 `{B_ATK_NAME_WITH_PREFIX}`)은 바꾸지 않았다. 파트 A도 바꾸면 안 된다.

## 3. 결정 범위 준수 확인

- 함정 1: Ret과 Fling 모두 비트마스크로 처리했다(위).
- 함정 2: `BreakScreens`는 그대로 `CMP_COMMON_BITS`를 쓴다. `CMP_BITMASK`는 멘탈허브에만 쓴다. 방벽 스크립트를 중복 추가하지 않았다.
- 함정 5: HnS 전용 스크립트 7종을 보존했다(2절).
- 함정 6/#9856: `HurtAttacker`는 출력 1회다.
- D3: `RanAwayUsingMonAbility`를 바꾸지 않았다. D6a는 `BreakScreens`와 진영 토큰을 바꾸지 않았고, D6c(사령탑)도 건드리지 않았다. 해당 hunk가 없다.
- 끈적끈적네트 YOU: 넣지 않았다. HnS는 지금도 `gSpinHazardsStringIds`/`gDefogHazardsStringIds`가 모든 장판에 `*DISAPPEAREDFROMTEAM`을 출력한다. patch 뒤에는 `BattleScript_RemoveHazards`가 `gRemoveHazardsStringIds`를 무조건 출력하므로 결과가 같다(파트 A의 표가 HnS Defog 표 값과 같다는 조건. 강철은 `PKMNBLEWAWAYSHARPSTEEL`과 `SHARPSTEELDISAPPEAREDFROMTEAM`의 본문 `{B_ATK_TEAM2} 주변의\n강철이 사라졌다!`가 바이트 동일). `STRINGID_STICKYWEBDISAPPEAREDFROMYOU`는 이 파트에서 참조하지 않는다.

## 4. 다른 파트와의 계약

### 이 patch가 정의·제공하는 것
- 상수: `CMP_BITMASK` = 6(`include/constants/battle_script_commands.h`)
- 전역 스크립트 라벨: `BattleScript_RemoveHazards`(신규, 선언 있음), `BattleScript_HealBlockEndTurn`(신규, 선언 있음). `BattleScript_MentalHerbCureRet`·`BattleScript_MentalHerbCureFling`·`BattleScript_MentalHerbCureEnd2`는 이름과 진입 조건이 그대로다.
- 삭제한 라벨: `BattleScript_SpinHazardsAway`, `BattleScript_DefogClearHazards`, `BattleScript_FlinchPrevention`(선언도 삭제. `DefogClearHazards` 선언만 upstream처럼 남김). 로컬 `BattleScript_RockyHelmetActivatesDmg`
- 추가하지 않은 라벨: `BattleScript_BerryCureStatusAndConfusionRet`

### 파트 A(`battle_message.c`, `battle_string_ids.h`)에 기대하는 것
- A-1 표 `gCureStatusStringIds`: HnS `gStatusCureStringIds`의 이름을 바꾼 것(값은 SCRCURED* 등 upstream·REPORT안). `GetCuredStatusMessage()`가 `B_MSG_CURED_PROBLEM`을 돌려줄 수 있으므로(`battle_util.c:9684`) **PROBLEM·NORMALIZED 칸도 채워야** 범위 밖 읽기가 없다. 스크립트 6곳(TakeHeart·JungleHealing·PsychoShift·Refresh·ShedSkin·AbilityCuredStatus)과 `battle_scripts_2.s` `CureStatus_Battler`가 이 표를 쓴다. `gStatusCureStringIds` 이름은 이 파트에서 더 쓰지 않는다.
- A-2 표 `gPartyCureStatusStringIds`(신규, PARTYCURED*, `new_sentences.tsv` 본문)
- A-3 표 `gRemoveHazardsStringIds`(신규 = HnS `gDefogHazardsStringIds` 값). `gSpinHazardsStringIds`·`gDefogHazardsStringIds`는 더 이상 스크립트에서 쓰지 않는다.
- A-4 표 `gHurtByStringIds`(신규): `[B_MSG_HURT] = STRINGID_AFTERMATHDMG`, `[B_MSG_HURT_BY_ITEM] = STRINGID_PKMNHURTSWITH`, enum `HurtByStringID`
- A-5 새 ID: `STRINGID_PKMNHEALEDPOISON`(= `PASTELVEILENTERS` 이름 변경, 본문 동일), `STRINGID_PKMNATKNOTLOWERED`(`{B_SCR_NAME_WITH_PREFIX}의\n공격은 떨어지지 않는다!`)
- A-6 그대로 있어야 하는 ID(이 patch가 참조): `PKMNSLEPTHEALTHY`, `SCR_ITDOESNTAFFECT`, `PKMNENDUREDHIT`, `ATTACKERSSTATROSE`, `SCRIPTINGSTATROSE`, `DEFENDERSSTATFELL`, `DEFENDERSSTATROSE`, `ITDOESNTAFFECT`, `POKEMONCANNOTUSEMOVE`, `AFTERMATHDMG`, `HEALBLOCKEDNOMORE`, `ATKGOTOVERINFATUATION`, `TORMENTEDNOMORE`, `PKMNMOVEDISABLEDNOMORE`, `PKMNENCOREENDED`(이 둘은 `{B_ATK}` 유지 — D6b), `PKMNSHOOKOFFTHETAUNT`, HnS 유지분 `PKMNFLEDUSING`, `HEALERCURE`, `REFLECTWOREOFF`/`LIGHTSCREENWOREOFF`/`AURORAVEILWOREOFF`, `PKMNSXTOOKATTACK`
- A-7 enum: `B_MSG_MENTALHERBCURE_{INFATUATION,TORMENT,DISABLE,HEALBLOCK,ENCORE,TAUNT}`. 이름만 쓰므로 순서는 자유지만 값이 0~7이어야 한다(u8 비트마스크). HnS `B_MSG_BREAK_*`(값형 1,2,4)는 유지하고, upstream `enum BreakScreensStringID`·`gBreakScreensStringIds`는 넣지 않는다(이름 충돌·의미 충돌).
- A-8 유지: `CureStatusBerryEffectStringID`(PROBLEM/NORMALIZED 포함), `gPurifyStatusCureStringIds`. `gMentalHerbCureStringIds`·`gRestUsedStringIds`는 이제 스크립트에서 쓰지 않는다(upstream도 표는 남김. 지워도 이 파트는 무관).
- A-9 넣지 않음: `STRINGID_STICKYWEBDISAPPEAREDFROMYOU`(이 파트는 참조하지 않음)

### 파트 C(C 코드)에 기대하는 것
- C-1 (필수, 조용한 실패 위험) `Cmd_jumpifbyte`(+`jumpifhalfword`/`jumpifword`)에 `case CMP_BITMASK: if (*ptr & (1 << value)) jump;`를 넣는다. 빠지면 빌드는 되지만 멘탈허브 문구가 하나도 나오지 않는다.
- C-2 (필수) `TryMentalHerb()`: 시작할 때 `MULTISTRING = 0`, 효과마다 `|= 1 << B_MSG_MENTALHERBCURE_*`. HnS의 `timing == IsOnFlingActivation`에 따른 Fling/Ret 분기는 유지한다. 값형으로 남으면 헤롱헤롱(0)이 문구 없이 끝나는 등 오동작한다.
- C-3 (필수, 링크) `SetMoveEffect` 정신력 분기에서 `BattleScript_FlinchPrevention` 참조를 지운다(`battle_script_commands.c:2646`).
- C-4 (필수, 링크) `BattleScript_DefogClearHazards`(`:7268`)와 `BattleScript_SpinHazardsAway`(`:9621`)를 `BattleScript_RemoveHazards`로 바꾼다. MULTISTRING = `hazardType`은 그대로 둔다.
- C-5 `HandleEndTurnHealBlock`: `BattleScript_BufferEndTurn` + `PREPARE_MOVE_BUFFER(HEAL_BLOCK)` 대신 `BattleScript_HealBlockEndTurn`을 쓴다(`gBattleScripting.battler = battler` 유지). 하지 않으면 새 스크립트가 쓰이지 않을 뿐 출력은 지금과 같다.
- C-6 `gHurtByStringIds`의 MULTISTRING: 까칠한피부·철가시(`BattleScript_RoughSkinActivates` 호출 전, `battle_util.c:4069`)에 `B_MSG_HURT`, 울퉁불퉁멧·자보열매·애터열매(`battle_hold_effects.c:258/354/377`)에 `B_MSG_HURT_BY_ITEM`. `HurtAttacker`를 부르는 곳은 이 3개 스크립트뿐이다.
- C-7 `{B_SCR}` 표의 스크립팅 배틀러: HnS가 이미 설정한다. `Cmd_curestatuswithmove`(`:9837`), `BS_CureStatus`(`:14761`), 탈피(`battle_util.c:3744`), 특성 회복, `BS_ItemCureStatus`(`:12377`). 유지하면 된다.
- C-8 아이템 상태회복 MULTISTRING(`HealStatusConditions`/`ItemHealMonVolatile`/`pokemon.c` 경로)이 `gPartyCureStatusStringIds`/`gCureStatusStringIds` 범위 안의 값으로 설정되어야 한다.
- C-9 `BattleScript_BerryCureStatusAndConfusionRet`을 참조하지 않는다(`TryCureAnyStatus` upstream 재작성 제외, HnS 리샘 루프 유지).
- C-10 D1(`resultmessage`)·D7 등 C/문자열 쪽 결정은 이 파트와 무관하다.

## 5. 한글이 든 줄 변경

- 없음. 이 patch는 한글 문자열과 토큰을 바꾸지 않는다. 바이트 길이·줄바꿈·창 너비에 영향이 없다.
- 출력 문장이 바뀌는 곳은 "어떤 기존 ID를 고르는가"가 바뀌는 경우뿐이다(6절). 새 ID 두 개의 본문은 파트 A가 확인한다.

## 6. 출력 변화 (스크립트 기여분)

| D4 | 상황 | 이전(HnS) | 이후 | 이 파트 변경 | 다른 파트 조건 |
|---|---|---|---|---|---|
| 2 | 회복봉인 턴 종료 | `BUFFERENDS` `…은(는)\n회복봉인의 효과가 풀렸다!` | `HEALBLOCKEDNOMORE` `…의\n회복봉인 효과가 사라졌다!` | `HealBlockEndTurn` 추가 | C-5 |
| 3 | 멘탈허브 | 마지막에 판정된 효과 한 문장(사슬묶기>회복봉인>트집>앙코르>도발>헤롱헤롱 순으로 덮어씀). 사슬묶기·앙코르 문장은 `{B_ATK}`(상대 이름이 나올 수 있음) | 풀린 효과마다 한 문장: 헤롱헤롱→트집→사슬묶기→회복봉인→앙코르→도발. 사슬묶기·앙코르도 보유자 이름(D6b). 도발은 `BUFFERENDS`(도발) → `PKMNSHOOKOFFTHETAUNT`이지만 보이는 문장은 같음 | 35번 | C-1, C-2 |
| 4 | 방음·방탄 | 팝업 + `PKMNSXBLOCKSY` `…은(는) {특성} 때문에\n{기술}을(를) 받지 않는다!` | 팝업 + `SCR_ITDOESNTAFFECT` `…에게는\n효과가 없는 것 같다...` | 24번 | — |
| 5 | 치유방울(방음 동료) | 위 `PKMNSXBLOCKSY`(팝업 없음) | `SCR_ITDOESNTAFFECT`(팝업 없음) | 5번 | — |
| 6 | 옹골참 일격기 방지 | 팝업 + `그러나 …에게는\n실패하고 말았다!` | 팝업 + `…에게는\n효과가 없는 것 같다...` | 21번 | — |
| 7 | 정신력 풀죽음 방지 | 팝업 + `PKMNSXPREVENTSFLINCHING` | 팝업·문구 없음 | 23번(스크립트 삭제) | C-3 |
| 8 | 피뢰침·마중물 끌어들임 | `PKMNSXTOOKATTACK`만 | 특성 팝업 → 같은 문장 | 20번 | — |
| 9 | 위협 방지 특성 | 팝업 + `…은(는)\n{특성}의 효과로 능력이 떨어지지 않는다!` | 팝업 + `…의\n공격은 떨어지지 않는다!` | 19번 | A-5 |
| 10 | 불굴의검·불굴의방패·바람타기·면영 | 팝업 + `…은(는)\n{특성}(으)로 {능력}이(가) {크게 }올라갔다!` | 팝업 + `…의\n{능력}이(가) {크게 }올라갔다!` | 25번 | — |
| 11 | 깨어진갑옷(방어 하락 성공) | `…의\n방어는 더 떨어지지 않는다!`(`TARGETABILITYSTATLOWER`, 오역 경로) | `…의\n방어가 떨어졌다!` | 27번 | — |
| 12 | 젖은접시·건조피부(비)·볼주머니 | 팝업 + `…의\n체력이 회복되었다.` | 팝업 + 회복 애니메이션(문구 없음) | 16번 | — |
| 13 | 포이즌힐 | 팝업 + 상태 애니메이션 + HP 바 | 위에 회복 애니메이션 추가(문구는 계속 없음) | 12번 | — |
| 14 | 울퉁불퉁멧 | 아이템 팝업 + 도구 애니메이션 + 문장 | 아이템 팝업 + 문장(도구 애니메이션 없음) | 31번 | C-6 |
| 15 | 가방 상태회복 도구 | `{종족}은(는)\n건강해졌다!` | 상태별 문장(PARTYCURED*/SCRCURED* 등) | scripts_2 #1 | A-1, A-2, C-8 |
| 16 | 테라폼제로 원시 날씨 | — | — | 스크립트 변경 없음 | 파트 A·C |
| 17 | 상태 회복 이름·동상 | `{B_ATK}` 기반 표 | `{B_SCR}` 기반 `gCureStatusStringIds` | 표 이름 변경 6곳 | A-1 |
| 1·18 | 빗나감·도주 | — | — | 도주(8번)는 제외해 HnS 유지. 빗나감은 C | D1(C), D3 |

보이는 출력이 바뀌지 않는 교체(ID만 바뀌고 한글 바이트 동일): 잠자기(4), 옹골참 버팀(10), 불굴의마음(13), 파스텔베일 해독(14), 습기 폭발 방지(21), 무효화 특성 최대치(22, `B_DEF`→`B_SCR` 같은 대상), 특성 능력 상승 2곳(26·29), 깨어진갑옷 스피드(28), 접촉 피해(30), 니들가드(32), 장판 제거(9).

새로 생긴 변화(D4 밖): 없다. D6b는 승인된 결정이고, 멘탈허브가 사슬묶기·앙코르를 풀 때 이름이 보유자로 고정된다. 지금은 공격자=보유자가 아닐 때(상대 기술 뒤 발동, Fling 맞음) 상대 이름이 나올 수 있다.

## 7. 정적 점검

- 새·변경 라벨 정의 1회씩: `...CureMessages`, `...CuresX` 6개, `...RetX` 7개, `RemoveHazards`, `HealBlockEndTurn`. 지운 라벨(`SpinHazardsAway`, `DefogClearHazards`, `FlinchPrevention`, `RockyHelmetActivatesDmg`, 표 `gMentalHerbCureStringIds`/`gRestUsedStringIds`/`gSpin`/`gDefog`/`gStatusCureStringIds`)을 참조하는 스크립트는 없다. 남은 참조는 C 3곳(C-3, C-4)과 stale extern(`DefogClearHazards`, upstream 동일)이다.
- `return`/`end` 짝: Ret·Fling·RemoveHazards·RockyHelmet·TookAttack는 `BattleScriptCall`로 들어와 `return`으로 나간다. HealBlockEndTurn·PoisonHeal은 `BattleScriptExecute`로 들어와 `end2`로 나간다. `...CureMessages`의 모든 분기는 `...RetFinish`의 `return`으로 끝난다. `saveattacker`/`restoreattacker`는 분기 없이 같은 블록에서 짝이 맞는다.
- 호출 깊이: 멘탈허브는 upstream보다 1단계 깊다(`call ...Messages`). Fling 경로는 대략 이동 스크립트 → SetMoveEffect push → BattleScriptCall → call로 4단계이고, 스택 8칸(`include/battle.h:275`) 안이다. HnS `BreakScreensMessages`와 같은 방식이다. 공격자 저장 스택은 1단계만 더 쓴다(최대 5).
- 공백: upstream `AbilityHpHeal`의 ` \t`는 tab으로 바꿨다. 나중에 upstream hunk가 이 줄을 문맥으로 쓰면 fuzz가 필요할 수 있다.

## 8. 위험·질문·실기 확인 후보

위험:
1. C-1·C-2가 빠지면 빌드는 통과하지만 멘탈허브 문구가 사라지거나 틀린 효과가 출력된다. 파트 C 결과와 함께 확인해야 한다.
2. A-1에서 PROBLEM 칸이 없으면 `GetCuredStatusMessage()`의 기본값 경로에서 표 범위 밖을 읽는다.
3. C-6이 빠지면 `HurtAttacker`가 남은 MULTISTRING 값으로 표를 읽는다. 지금 한글은 두 ID가 같아서 0·1이면 무해하지만 그 밖의 값이면 깨진다.
4. 테스트: 멘탈허브·방음·위협·정신력·옹골참·깨어진갑옷·젖은접시 테스트의 영문 MESSAGE 기대값과 다르다(알려진 한계). `steadfast.c` PASS→FAIL 가능성은 REPORT 7번과 같다.

질문(결정 범위 밖, 고치지 않음):
- Q1. 멘탈허브 Fling 경로의 출력 순서·이름은 Ret과 같게 만들었다. upstream에는 Fling 전용 스크립트가 없으므로(HnS 고유) 별도 확인이 필요한지 정해야 한다.
- Q2. `BattleScript_BerryCureStatusAndConfusionRet`를 미사용으로라도 넣을지 정해야 한다(REPORT는 둘 다 허용). 이 안은 넣지 않는다. 이후 upstream hunk가 이 라벨을 문맥으로 쓰면 그때 판단한다.

실기 확인 후보:
- 멘탈허브: 도발+앙코르+사슬묶기를 동시에 풀 때 문장 3개가 순서대로 나오는지, 사슬묶기·앙코르 문장에 보유자 이름이 나오는지. Fling으로 맞힌 경우도 확인.
- 턴 종료 사슬묶기·앙코르 해제 문장이 그대로인지(회귀 확인)
- 방음(소리 기술)·방탄, 치유방울+방음 동료, 옹골참+일격기, 정신력+풀죽음, 피뢰침 끌어들임 팝업, 위협+클리어바디류
- 불굴의검 등장, 깨어진갑옷, 젖은접시(비), 포이즌힐 애니메이션, 울퉁불퉁멧(공격자 기절 포함), 회복봉인 턴 종료
- 가방 해독제·만병통치제(배틀 중 교체 대기 포켓몬 포함), 고속스핀·안개제거 끈적끈적네트(플레이어 측) 문장이 TEAM 문장으로 나오는지
