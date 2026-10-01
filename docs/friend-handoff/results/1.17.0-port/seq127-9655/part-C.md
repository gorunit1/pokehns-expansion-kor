# seq 127 #9655 (+#10064) 사전 분석 — 파트 C (C 코드)

- 기준: 메인 저장소 HEAD `fcf855d4e8`(분석 중 HEAD가 `05319fd9b7`로 올라갔으나 이 커밋은 기록 문서만 바꿈. 담당 5파일·헤더 4개 변화 없음).
- 산출물: `part-C.patch`(31 hunk, 5파일 +109/−62), 이 문서. 스크래치 트리 `part-C/tree/`.
- 검증: `git diff --check` 통과, `git -C /home/jinmo/pokehns-expansion-kor apply --check part-C.patch` 통과. **빌드는 하지 않았다**(지시). 이 patch는 파트 A·B의 심볼(아래 3절)이 함께 들어가야 컴파일된다.
- 헤더(`include/battle.h`, `battle_util.h`, `battle_hold_effects.h`, `constants/battle.h`)는 바꿀 필요가 없었다.

## 1. upstream hunk 표

판정: 그대로 24 / 수정해서 2 / 제외 6 (#9655 C hunk 32개). #10064 1개 그대로. HnS 추가 3곳(1-7).

### 1-1. `src/battle_end_turn.c`
| # | 위치(HEAD) | 내용 | 판정 | 이유 |
|---|---|---|---|---|
| 1 | `HandleEndTurnHealBlock` :836 | `BattleScript_BufferEndTurn`+BUFF1(회복봉인) → `BattleScript_HealBlockEndTurn` | 그대로 | D4 #2. `gBattleScripting.battler = battler`가 이미 있어 `{B_SCR}`(HEALBLOCKEDNOMORE) 정상 |

### 1-2. `src/battle_hold_effects.c`
| # | 위치 | 내용 | 판정 | 이유 |
|---|---|---|---|---|
| 1 | `TryRockyHelmet` :258 | `MULTISTRING = B_MSG_HURT_BY_ITEM` | 그대로 | `BattleScript_HurtAttacker`가 `gHurtByStringIds` 사용(파트 B) |
| 2 | `TryJabocaBerry` :354 | 〃 | 그대로 | 〃 |
| 3 | `TryRowapBerry` :377 | 〃 | 그대로 | 〃 |
| 4 | `TryMentalHerb` :422 | 대입형 → 비트마스크(`|= 1 << B_MSG_MENTALHERBCURE_*`), 검사 순서 헤롱헤롱→트집→사슬묶기→회복봉인→앙코르→도발, LoveJpn BUFF1 삭제 | 그대로(fuzz 1) | HnS 시그니처 `(battler, timing)`와 `IsOnFlingActivation`이면 `BattleScript_MentalHerbCureFling` 호출 분기 유지 확인 |
| 5 | `TryCureFreezeOrFrostbite` :720 | `B_MSG_CURED_FREEEZE`→`FREEZE` | 그대로 | 오타 교정(함정 3) |
| 6 | `TryCureAnyStatus` :765 | 상태별 MULTISTRING + `BattleScript_BerryCureStatusAndConfusionRet` | **제외** | HnS 리샘열매 반복 출력(`lumBerryCureStatusMask`·`BattleScript_LumBerryCureStatusRet`) 유지. 출력 변경 문서 행 보존 |

### 1-3. `src/battle_script_commands.c`
| # | 위치 | 내용 | 판정 | 이유 |
|---|---|---|---|---|
| 1 | `AccuracyCheck` :1170 | `MISS_TYPE = B_MSG_MISSED` → `STRINGID_PKMNEVADEDATTACK` | 그대로(fuzz 1) | MISS_TYPE은 `B_MSG_PROTECTED`(1)와 비교만 함(출력 무관, 값 95~96). HnS #9929 삼항식(`BattleScript_ButItFailed`→`BattleScript_TargetAvoidsAttackEnd`) 그대로 |
| 2 | `Cmd_resultmessage` :2051 | `STRINGID_ATTACKMISSED` → upstream `PKMNEVADEDATTACK` | **수정해서** | D1: `STRINGID_PKMNAVOIDEDATTACK` + `// HnS:` 주석 |
| 3 | `SetNonVolatileStatus` :2473 | `B_MSG_STATUSED_BY_ABILITY` 선택 삭제 | 그대로 | HnS 표의 BY_ABILITY 값이 STATUSED와 같음(얼음만 다르나 특성 얼음 경로 없음) → 출력 불변 |
| 4 | `SetMoveEffect` FLINCH :2638 | 정신력: `BattleScript_FlinchPrevention` 호출 삭제 | 그대로 | D4 #7. C에서 더는 참조 안 함 |
| 5 | `SetMoveEffect` AURORA_VEIL :3547 | `B_MSG_SET_SAFEGUARD`→`B_MSG_SET_AURORA_VEIL` | **제외** | HnS에 이미 있음 |
| 6 | `SetMoveEffect` BREAK_SCREEN :3718 | `gSideTimers` 기준 `|= 1 << n` | **제외** | HnS 값형 `B_MSG_BREAK_*`(1,2,4) + `CMP_COMMON_BITS` 유지(함정 2) |
| 7~9 | `Cmd_jumpifbyte/halfword/word` :4780/4820/4860 | `case CMP_BITMASK: if (*ptr & (1 << value))` | 그대로 | 멘탈허브 스크립트용 |
| 10 | `RemoveAllWeather` :7189 | 원시 비·원시 햇살 → `B_MSG_WEATHER_END_HEAVY_RAIN`/`_EXTREMELY_HARSH_SUNLIGHT` | 그대로 | D4 #16(테라폼제로) |
| 11 | `DefogClearHazards` :7268 | `BattleScript_DefogClearHazards`→`BattleScript_RemoveHazards` | 그대로 | 계약(파트 B) |
| 12 | `Cmd_rapidspinfree` :9594(호출 :9621) | `BattleScript_SpinHazardsAway`→`BattleScript_RemoveHazards` | 그대로 | 〃 |
| 13 | `Cmd_curestatuswithmove` :9824 | 상태별 MULTISTRING + `gBattleScripting.battler` | **제외** | HnS 동등: `GetCuredStatusMessage()` + `gBattleScripting.battler = gBattlerAttacker` 이미 있음 |
| 14~15 | `BS_ItemRestorePP` :12448/:12498 | `enum Move moveId`, `PREPARE_MOVE_BUFFER(gBattleTextBuff2, moveId)` | 그대로 | HnS `ITEMRESTOREDSPECIESPP` 한글은 BUFF2 미사용 → 출력 불변(무해) |
| 16 | `BS_CureStatus` :14755 | 상태별 MULTISTRING + battler | **제외** | HnS 동등(위 13과 같음) |
| 17 | `BS_SetAuroraVeil` :14917 | `B_MSG_SET_AURORA_VEIL` | **제외** | HnS에 이미 있음 |

### 1-4. `src/battle_util.c`
| # | 위치 | 내용 | 판정 | 이유 |
|---|---|---|---|---|
| 1 | `AbilityBattleEffects` HEAL_MON_STATUS :3725 (탈피·촉촉한몸) | `StringCopy(BUFF1, *Jpn)` → 상태별 MULTISTRING | **수정해서** | upstream 줄 적용 + 바로 위 HnS `GetCuredStatusMessage()` 1줄 삭제(같은 값 중복 대입). BUFF1은 `gStatusCureStringIds` 문장이 안 써서 죽은 코드였음 |
| 2 | ROUGH_SKIN :4069 | `MULTISTRING = B_MSG_HURT` | 그대로 | `gHurtByStringIds` |
| 3 | COMMANDER :4744 | `PREPARE_MON_NICK_BUFFER(BUFF1, partner)` | 그대로 | HnS `COMMANDERACTIVATES` 한글은 BUFF1 미사용 → 무해. D6-c 건드리지 않음 |
| 4~6 | `TryImmunityAbilityHealStatus` :9035 | StringCopy → MULTISTRING(독·혼란·마비·잠듦·화상·얼음/동상 분리·헤롱헤롱·도발) | 그대로 | 아래 1-7의 HnS 중복 2줄 삭제와 같이 감 |
| 7 | `ItemHealMonVolatile` :10266 | `MULTISTRING = B_MSG_CURED_CONFUSION/INFATUATION` | 그대로 | 가방 도구 혼란·헤롱헤롱 회복(D4 #15) |

### 1-5. `src/pokemon.c`
| # | 위치 | 내용 | 판정 | 이유 |
|---|---|---|---|---|
| 1 | `HealStatusConditions` :6552 | `PREPARE_MON_NICK_BUFFER(BUFF1, battler, …)` + 상태별 MULTISTRING | 그대로 | upstream 빈 줄의 trailing space만 제거(`git diff --check`). D4 #15 |

### 1-6. #10064 `src/battle_util.c`
| # | 위치 | 내용 | 판정 | 이유 |
|---|---|---|---|---|
| 1 | `IsNonVolatileStatusBlocked` :5539(변경 :5548) | `gBattleScripting.battler = battlerDef`를 특성 여부와 무관하게 설정 | 그대로 | 이 경로 스크립트(Already*·Safeguard·Misty/ElectricTerrain·SleepClause)의 HnS 한글은 모두 `{B_DEF}` → 출력 불변 |

### 1-7. HnS 추가·적응(upstream hunk 밖)
| 위치 | 내용 | 이유 |
|---|---|---|
| `BS_TryPrintNextLumBerryCureStatus` :12415 | `B_MSG_CURED_FREEEZE`→`FREEZE` | 함정 3, HnS 전용 리샘 루프 |
| `GetCuredStatusMessage` :9692 | 〃 | 함정 3. 함수와 `B_MSG_CURED_PROBLEM` 반환은 유지 |
| `TryImmunityAbilityHealStatus` 두 번째 switch :9100 | HnS `GetCuredStatusMessage()`·`B_MSG_CURED_CONFUSION` 대입 2줄 삭제, 헤롱헤롱·도발 전용 스크립트 유지 + `// HnS:` 주석 | 첫 switch(upstream)가 같은 값을 넣음. 출력 동일 |
| `FinalizeCapture` 힐볼 :10758 | `HealStatusConditions()` 앞뒤로 MULTISTRING 저장·복원 + `// HnS:` | **#9655 회귀 방지.** 아래 6절 Q1 |

## 2. HnS 보존·`// HnS:` 주석

- 새 `// HnS:` 3곳: `Cmd_resultmessage`(D1), `TryImmunityAbilityHealStatus`(전용 스크립트), `FinalizeCapture`(힐볼).
- 변경 함수 안의 기존 HnS 표식(`// HnS`, `IS_HNS`, `IS_FRLG`) 전후 비교: 삭제 0, 추가 3(위). HnS config(`GetConfig(B_*)`, `B_MENTAL_HERB`, `B_UPROAR` 등) 줄 변경 없음.
- 보존 확인: #9929 `AccuracyCheck` 삼항식, 멘탈허브 Fling 분기, 리샘 루프(`TryCureAnyStatus`·`BS_TryPrintNextLumBerryCureStatus`), 방벽 값형 비트(`B_MSG_BREAK_*`, :3720~3728), `GetCuredStatusMessage()`(curestatuswithmove·BS_CureStatus에서 계속 사용), 특성 회복의 헤롱헤롱·도발 전용 스크립트, `HandleEndTurnThirdEventBlock`의 #9616 방음 HnS 줄.

## 3. 다른 파트와의 계약

### 이 patch가 C에서 쓰는(다른 파트가 정의해야 하는) 심볼
파트 A (`include/constants/battle_string_ids.h`, `src/battle_message.c`)
- `B_MSG_CURED_FREEZE`(FREEEZE 이름 교체). **FREEEZE는 헤더 1·표 3(`gStatusCureStringIds`→`gCureStatusStringIds`, `gPurifyStatusCureStringIds`, `CureStatusBerryEffectStringID`)에서 파트 A가, 코드 3(`TryCureFreezeOrFrostbite`, `BS_TryPrintNextLumBerryCureStatus`, `GetCuredStatusMessage`)은 이 patch가 바꾼다. 둘을 같은 커밋에 넣어야 한다.**
- `B_MSG_CURED_CONFUSION`, `B_MSG_CURED_INFATUATION`, `B_MSG_CURED_TAUNT`(새로 필요: INFATUATION·TAUNT), `B_MSG_CURED_PROBLEM`, `B_MSG_NORMALIZED_STATUS` 유지.
- 표 범위: C가 넣는 값 — `GetCuredStatusMessage()`는 상태가 0이면 `B_MSG_CURED_PROBLEM` 반환 → **`gCureStatusStringIds`(구 gStatusCureStringIds)·`gPurifyStatusCureStringIds`에 PROBLEM/NORMALIZED 항목 유지 필요**(upstream `gCureStatusStringIds`에는 없음). `CureStatusBerryEffectStringID`는 PROBLEM/NORMALIZED/CONFUSION 유지. `gCureStatusStringIds`에는 CONFUSION·INFATUATION·TAUNT 항목 필요(`ItemHealMonVolatile`·Oblivious). `gPartyCureStatusStringIds`는 C가 0~5(마비·독·화상·얼음·동상·잠듦)만 넣는다.
- `enum HurtByStringID { B_MSG_HURT, B_MSG_HURT_BY_ITEM }` + `gHurtByStringIds`.
- `B_MSG_WEATHER_END_EXTREMELY_HARSH_SUNLIGHT`, `B_MSG_WEATHER_END_HEAVY_RAIN` + `gWeatherEndsStringIds` 항목(`[B_MSG_WEATHER_END_COUNT]` 크기).
- `MentalHerbCureStringID` 재배열: INFATUATION=0, TORMENT=1, DISABLE=2, HEALBLOCK=3, ENCORE=4, TAUNT=5(비트 번호, MULTISTRING u8이라 <8).
- `STRINGID_ATTACKMISSED` 삭제 가능(C 사용처 0). `STRINGID_PKMNAVOIDEDATTACK`·`STRINGID_PKMNEVADEDATTACK`는 유지. `gMissStringIds[B_MSG_MISSED]`·`gKOFailedStringIds[B_MSG_KO_MISS]`(미사용)는 파트 A 몫.
- 그대로 쓰는 기존 값: `B_MSG_SET_AURORA_VEIL`, `B_MSG_BREAK_*`(값형), `B_MSG_STATUSED`.

파트 B (`data/battle_scripts_1.s`, `battle_scripts_2.s`, `include/battle_scripts.h`, `include/constants/battle_script_commands.h`)
- `#define CMP_BITMASK 6`.
- `BattleScript_RemoveHazards`(정의 + extern): `gBattleCommunication[MULTISTRING_CHOOSER] = hazardType` 기준 `printfromtable`(구 Spin/Defog 두 경로 공용). C는 `BattleScript_SpinHazardsAway`·`BattleScript_DefogClearHazards`를 더 참조하지 않는다. 끈적끈적네트 YOU 분기는 넣지 않는 것을 전제(안개제거 경로는 `gBattlerAttacker = 진영 번호`라 `jumpifside BS_ATTACKER` 판정이 upstream과 다르게 동작할 수 있음).
- `BattleScript_HealBlockEndTurn`(정의 + extern, `printstring STRINGID_HEALBLOCKEDNOMORE` … `end2`).
- `BattleScript_HurtAttacker`: `printfromtable gHurtByStringIds` 1회(#9856 중복 `printstring` 없음). 호출자 MULTISTRING은 C가 설정(까칠한피부·철가시 `B_MSG_HURT`, 울퉁불퉁멧·자보/애터열매 `B_MSG_HURT_BY_ITEM`).
- 멘탈허브 `BattleScript_MentalHerbCureRet`·**HnS `BattleScript_MentalHerbCureFling` 둘 다** `jumpifbyte CMP_BITMASK, cMULTISTRING_CHOOSER, B_MSG_MENTALHERBCURE_*` 분기(함정 1). C는 Ret/Fling 이름 그대로 호출.
- `BattleScript_FlinchPrevention`: C 참조 없음(삭제 가능). `BattleScript_BerryCureStatusAndConfusionRet`: C가 호출하지 않음(추가 불필요).
- C가 계속 호출하는 HnS 스크립트(보존): `BattleScript_LumBerryCureStatusRet`, `BattleScript_BerryCureStatusRet`, `BattleScript_BerryCureConfusionRet`, `BattleScript_BattlerGotOverItsInfatuation`, `BattleScript_BattlerShookOffTaunt`, `BattleScript_AbilityCuredStatus`, `BattleScript_ShedSkinActivates`, `BattleScript_BreakScreens`, `BattleScript_MoveEffectScreens`, `BattleScript_BufferEndTurn`(도발·전자부유 턴 종료), `BattleScript_MonWokeUpInUproar`.
- 가방 상태회복(`battle_scripts_2.s`): `BattleScript_ItemCureStatus` → `printfromtable gPartyCureStatusStringIds`, `BattleScript_CureStatus_Battler` → `gCureStatusStringIds`. C 보장: MULTISTRING(`HealStatusConditions`·`ItemHealMonVolatile`), 필드 대상이면 `gBattleScripting.battler = targetBattler`, BUFF1 = 종족명(`BS_ItemCureStatus`의 `PREPARE_SPECIES_BUFFER`가 `HealStatusConditions`의 닉네임 버퍼를 덮어씀).
- 위협 방지 `BattleScript_IntimidatePrevented`의 `copybyte sBATTLER, gBattlerTarget` 유지(`PKMNATKNOTLOWERED` `{B_SCR}`).

### D6-b(멘탈허브 사슬묶기·앙코르 이름) 분담 — **C 지원 불필요**
- `ItemBattleEffects()`가 효과 발생 시 `gBattleScripting.battler = itemBattler`(허브 보유자)를 넣고(:1189), 스크립트는 그 뒤 실행된다. Fling도 같다.
- 파트 B: Ret·Fling의 사슬묶기(`PKMNMOVEDISABLEDNOMORE`)·앙코르(`PKMNENCOREENDED`, 둘 다 HnS 한글 `{B_ATK}`) 출력 앞뒤로 `saveattacker` / `copybyte gBattlerAttacker, sBATTLER` / … / `restoreattacker`. 문장 불변.
- 나머지 넷(헤롱헤롱·트집·회복봉인·도발)은 HnS 한글이 이미 `{B_SCR}`.

## 4. 한글이 든 줄 변경
- 없음(이 patch의 비ASCII 줄 0). 새 문자열 없음.

## 5. 출력 변화(C 기여분)
| D4 # | 상황 | 이전 → 이후 | C 변경 |
|---|---|---|---|
| 1 (D1) | 일반 공격 빗나감(`resultmessage`) | `그러나 {공격자}의\n공격은 빗나갔다!` → `{대상}에게는\n맞지 않았다!`(PKMNAVOIDEDATTACK) | resultmessage 1줄 |
| 2 | 회복봉인 턴 종료 | `…은(는)\n회복봉인의 효과가 풀렸다!` → `…의\n회복봉인 효과가 사라졌다!` | end_turn |
| 3 | 멘탈허브 | 마지막 판정 효과 1문장 → 풀린 효과마다 순서대로 | TryMentalHerb 비트마스크(+파트 B) |
| 7 | 정신력 풀죽음 방지 | 팝업+`…은(는) {특성} 때문에\n풀이 죽지 않는다!` → 팝업·문구 없음 | SetMoveEffect FLINCH |
| 14 | 울퉁불퉁멧 | 문장 동일(MULTISTRING만 설정) | hold_effects(애니메이션 삭제는 파트 B) |
| 15 | 가방 상태회복 | `{종족}은(는)\n건강해졌다!` → 필드: `gCureStatusStringIds`(닉네임), 파티: `gPartyCureStatusStringIds`(종족명) 상태별 | pokemon.c, ItemHealMonVolatile |
| 16 | 테라폼제로가 원시 날씨 제거 | `햇살이 약해졌다!`/`비가 그쳤다!` → `햇살이 원래대로 되돌아왔다!`/`강한 비가 그쳤다!` | RemoveAllWeather |
| 17 | 특성·기술 상태 회복 이름 | 표 교체는 파트 A. C는 모든 경로에서 `gBattleScripting.battler` 설정 확인(아래) | 없음(동등) |
- 출력 불변 확인: SetNonVolatileStatus(BY_ABILITY), BS_ItemRestorePP(BUFF2), 사령탑 BUFF1, #10064, MISS_TYPE 값, 특성 회복 MULTISTRING 위치 이동, FREEEZE 이름.
- 새로 생긴 출력 변화: 없음(힐볼 수정은 기존 출력 유지용).

### 함정 4 / `{B_SCR}` 경로의 `gBattleScripting.battler` (HEAD 코드 확인)
1. `Cmd_curestatuswithmove`(리프레시·브레이브차지) :9837 `= gBattlerAttacker` ✓
2. `BS_CureStatus`(정글힐·사이코시프트·정화·치유의마음) :14761 `= battler` ✓
3. HEAL_MON_STATUS(탈피·촉촉한몸) :3744 `= battler` ✓
4. `TryImmunityAbilityHealStatus`(면역·유연·불면 등·둔감) :9122 `= battler` ✓
5. `BS_ItemCureStatus`(가방, 필드 대상) :12377 `= targetBattler` ✓
- 그 밖: 방음·방탄·MonMadeMoveUseless(`CanAbilityBlockMove` :2474), 치유방울 방음(`Cmd_healpartystatus` :9290 `= partner`, HnS config GEN_LATEST라 공격자 분기 미사용), 회복봉인 턴 종료(:838), 멘탈허브(:1189), 위협 방지(스크립트 copybyte). upstream hunk에 추가할 설정 줄 없음.

### D7 확인 — `STRINGID_PKMNWOKEUPINUPROAR`를 `{B_EFF_…}`로 바꿔도 되는가
- 행동 전 기상 `CancelerAsleepOrFrozen`(`battle_move_resolution.c:120`) `gEffectBattler = cv->battlerAtk` ✓
- 배틀팰리스 `BattlePalace_TryEscapeStatus`(`battle_util2.c:140`) `gEffectBattler = battler` ✓
- 턴 종료 `HandleEndTurnThirdEventBlock`(`battle_end_turn.c:1229~1246`) 루프 변수 `gEffectBattler`가 깬 포켓몬, 찾으면 `break` ✓ → `BattleScript_MonWokeUpInUproar`
- 결론: C 수정 불필요. `B_UPROAR = GEN_4`, #9616 HnS 방음 줄 그대로.

## 6. 위험·질문·실기 후보

질문
- **Q1(patch에 포함, 확인 요청)** upstream `HealStatusConditions()`가 MULTISTRING을 바꾸면서 힐볼 포획(`FinalizeCapture`)에서 잠든 포켓몬을 잡으면 `B_MSG_CURED_SLEEP`(5) = `B_MSG_SWAPPED_INTO_PARTY`(5)가 남는다. 파티가 가득 찬 상태(HnS `B_CATCH_SWAP_INTO_PARTY` GEN_LATEST)에서 "교체 안 함/취소"를 고르면 `givecaughtmon`이 박스 전송 문구 설정을 건너뛰고 `gText_PkmnSentToPCAfterCatch`를 묵은 STR_VAR로 출력한다(upstream 1.17.0·master 동일). HnS 출력 보존을 위해 저장·복원 2줄을 넣었다. 원치 않으면 `FinalizeCapture` hunk만 빼면 된다.
- Q2(범위 밖, 고치지 않음) 턴 종료 소란 기상 `BtlController_EmitSetMonData(gEffectBattler, …, &gBattleMons[gBattlerAttacker].status1)` — 깬 포켓몬 파티 데이터에 공격자 상태를 써 넣는다(upstream 1.17.0 동일, 기존 문제). 별건으로 둘지.
- Q3(upstream 동작, 고치지 않음) 필드의 포켓몬에 만병통치제(ITEM3_STATUS_ALL)를 썼고 헤롱헤롱만 있었으면 `ItemHealMonVolatile`이 CONFUSION을 골라 `…의\n혼란이 풀렸다!`가 나온다(1.17.0 동일). D4 #15 범위로 받아들일지.
- Q4(upstream 그대로 둠) `HealStatusConditions()`의 `PREPARE_MON_NICK_BUFFER`는 전투에서는 곧바로 종족 버퍼로 덮여 죽은 코드이고, 필드 사용(`battler = MAX_BATTLERS_COUNT`)에서는 `gBattlerPartyIndexes[4]` 범위 밖 읽기(읽기만, 무해). upstream 그대로 유지했다.
- 참고: `TryMentalHerb()`가 효과가 없어도 매 타격 뒤 MULTISTRING을 0으로 만든다(upstream 동일). 이 값에 의존하는 뒤 단계는 찾지 못했다.

위험
- 컴파일은 파트 A(enum·표)·B(스크립트 라벨·CMP_BITMASK)와 함께여야 한다. 특히 FREEEZE 이름은 A와 C가 동시에 바뀌어야 한다.
- 파트 A가 `gCureStatusStringIds`를 upstream 표 그대로(PROBLEM 없음) 넣으면 `GetCuredStatusMessage()` 폴백에서 범위 밖 읽기 위험(현재 경로는 상태가 항상 있어 실제로는 안 나옴).

실기 확인 후보
- 일반 공격 빗나감 문구(D1), 회복봉인 턴 종료 문구, 멘탈허브 다중 해제(헤롱헤롱+사슬묶기+앙코르, 이름=허브 보유자), 정신력+속이기(팝업 없음), 테라폼제로 vs 끝의대지·시작의바다, 가방 상태회복(필드/파티, 혼란), 울퉁불퉁멧·까칠한피부 문구 1회, 소란 턴 종료 기상 이름(D7), **잠든 포켓몬을 힐볼로 잡고 파티 가득 → 박스로 보내기 문구(Q1)**.
