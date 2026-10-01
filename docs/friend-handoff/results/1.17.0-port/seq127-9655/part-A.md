# seq 127 #9655 사전 분석 — 파트 A(문자열)

- 담당: `src/battle_message.c`, `include/constants/battle_string_ids.h`
- 기준: 스크래치 트리 첫 커밋(HnS `fcf855d4e8`). 작업 시점 메인 HEAD는 `05319fd9b7`로 움직였지만 두 파일 blob이 같다(`594d321e…`, `36cf7f15…`).
- 산출물
  - `part-A.patch` (2파일, +112/−56). `git -C /home/jinmo/pokehns-expansion-kor apply --check part-A.patch` **통과**. `git diff --check` 통과.
  - `part-A-stringids.tsv` (impact 112행의 최종 처리)
- 검증 범위: 코드·데이터 대조와 apply --check만 했다. **빌드·테스트는 하지 않았다**(BRIEF 규칙). 이 패치만 단독으로 넣으면 빌드가 깨진다. 3절 계약의 파트 B·C 변경이 같이 들어가야 한다.

## 1. upstream hunk 표 (39개)

판정 집계: 그대로 11, 수정해서 9, 제외 19. dry-run 실패 27개(battle_message.c 24, 헤더 3)를 모두 수동으로 판정했다.

### include/constants/battle_string_ids.h (9개, dry-run 실패 #2 #4 #6)

| # | 내용 | 판정 | 이유 |
|---|---|---|---|
| 1 | `STRINGID_ATTACKMISSED` 삭제 | 그대로 | D1 C안 |
| 2 | `STRINGID_PKMNAURORAVEIL` 추가 | 수정해서 | HnS는 같은 자리에 HnS 전용 `PKMNRAISEDDEFSPDEF`가 있다. 그 뒤, `PKMNCOVEREDBYVEIL` 앞에 넣었다 |
| 3 | `PASTELVEILENTERS` → `PKMNHEALEDPOISON` | 그대로 | 이름만 바뀐다 |
| 4 | enum 끝에 새 ID 16개 | 수정해서 | `ZENMODEENDED` 뒤, HnS/Champions 블록(`WILDPKMNDROPPEDITEM`…) 앞에 12개(SCRCURED×5, PARTYCURED×6, PKMNATKNOTLOWERED)를 넣었다. 1.17.0과 같은 위치다. `*WOREOFF` 3개는 HnS에 이미 있어서(:336-338) 넣지 않았다. `STICKYWEBDISAPPEAREDFROMYOU`도 넣지 않았다. 둘 다 `// HnS:` 주석으로 남겼다 |
| 5 | `B_MSG_WEATHER_END_EXTREMELY_HARSH_SUNLIGHT`·`_HEAVY_RAIN` 추가 | 그대로 | STRONG_WINDS 6→8, COUNT 7→9 |
| 6 | `B_MSG_SET_AURORA_VEIL` 추가 | 제외 | HnS에 이미 있다(:854) |
| 7 | `enum CureStatusBerryEffectStringID` → `CureStatusEffectStringID`, FREEEZE→FREEZE, CONFUSION·INFATUATION·TAUNT 추가 | 수정해서 | HnS 끝에 있던 `B_MSG_CURED_CONFUSION`을 upstream 위치로 옮겼다(fuzz로 적용하면 CONFUSION이 중복된다). PROBLEM/NORMALIZED는 upstream처럼 끝에 남겼다 |
| 8 | 멘탈허브 enum을 비트 인덱스 순서로 재배치 | 그대로 | INFATUATION0 TORMENT1 DISABLE2 HEALBLOCK3 ENCORE4 TAUNT5 |
| 9 | `enum HurtByStringID`, `enum BreakScreensStringID` 추가 | 수정해서 | HurtBy만 넣었다. BreakScreens는 HnS 값형(`1 << n`, :857-863) enum이 이미 있다. 함정 2에 따라 인덱스형을 추가하지 않고 `// HnS:` 주석을 남겼다 |

### src/battle_message.c (30개, dry-run 실패 #1~#21 #26 #28 #30)

| # | 내용 | 판정 | 이유 |
|---|---|---|---|
| 1 | `gText_StatSharply` ` sharply`, `gText_DefendersStatRose` 어순 | 제외 | 한국어 어순 `크게 `·`{B_BUFF2}올라갔다` 유지(g6 plan) |
| 2 | `gText_drastically` | 제외 | `매우 크게 ` 유지 |
| 3 | `ATTACKMISSED` 문자열 삭제 | 그대로 | D1. 한글 줄 삭제 |
| 4 | `PKMNAURORAVEIL` 문자열 | 수정해서 | 한글은 `PKMNRAISEDDEFSPDEF` 본문을 복제했다 |
| 5 | `PKMNSTAYEDAWAKEUSING` 영문 | 제외 | D4: 재번역 금지 |
| 6 | GOTFREE·SHEDLEECHSEED·BLEWAWAYSPIKES·PKMNATTACK·NATUREPOWERTURNEDINTO 영문·주석 | 제외 | 〃 (NATUREPOWER 이름 추가는 선택 개선이라 하지 않았다) |
| 7 | ANCHOREDITSELF~SCRIPTINGSTATROSE 14줄 영문·주석 | 제외 | 〃, ATTACKERS/SCRIPTINGSTATROSE의 영어 접미 이동도 따르지 않음 |
| 8 | `ENEMYABOUTTOSWITCHPKMN` `\p` 제거 | 제외 | HnS 두 쪽 구성 유지 |
| 9 | `PKMNSITEMCUREDPROBLEM` 주석 | 제외 | 주석만 |
| 10 | RESTOREDHPALITTLE2·PREVENTSYLOSS·INFATUATEDY·MADEYINEFFECTIVE | 제외 | D4, D2 A안(MADEYINEFFECTIVE 한글 유지) |
| 11 | ITEMSCANTBEUSEDNOW·USINGITEMSTATOFPKMNROSE/FELL | 제외 | D4 |
| 12 | SXTOOKATTACK·SITEMNORMALIZEDSTATUS·MADEITINEFFECTIVE | 제외 | D4, HnS 스위트베일 문장 유지(#10149 동등) |
| 13 | TARGETABILITYSTATRAISE~SYMBIOSISITEMPASS 7줄 | 제외 | D4. POISONHEALHPUP·SOLARPOWERHPDROP·ICEBODYHPGAIN은 HnS 영문 잔존 줄이지만 주석만 바뀌어 제외 |
| 14 | CURSEDBODYDISABLED~IMPOSTERTRANSFORM 5줄 | 제외 | D4 |
| 15 | `HEALERCURE` 영문 | 제외 | HnS 사용자 요청 행 유지 |
| 16 | `PKMNSWILLPERISHIN3TURNS` perish→faint | 제외 | BRIEF: 별건, 지금 고치지 않음 |
| 17 | ATKGOTOVERINFATUATION·HEALBLOCKEDNOMORE 영문 | 제외 | D4 |
| 18 | `PASTELVEILENTERS` → `PKMNHEALEDPOISON` | 수정해서 | ID만 바꿨다. 한글 본문은 그대로다 |
| 19 | ITEMCUREDSPECIESSTATUS 주석·ITEMRESTOREDSPECIESPP 영문 | 제외 | D4(BUFF2 기술명 추가는 선택 개선이라 하지 않았다) |
| 20 | TERASTALLIZEDINTO 주석·COMMANDERACTIVATES 영문 | 제외 | D6-c 별도 과제 |
| 21 | 표 끝 새 문자열 16개 | 수정해서 | 12개를 `new_sentences.tsv` 제안 한글로 넣었다. WOREOFF 3개와 STICKYWEB YOU는 넣지 않았다 |
| 22 | `gMentalHerbCureStringIds` 재배치, TAUNT→PKMNSHOOKOFFTHETAUNT | 그대로 | 멘탈허브 도발 출력은 화면 바이트가 같다 |
| 23 | 파스텔베일 표, `gMissStringIds[B_MSG_MISSED]`→PKMNAVOIDEDATTACK | 그대로 | D1 |
| 24 | `gAbilityWeatherChangeStringId` → STARTEDTORAIN 등 | 그대로 | 다섯 쌍 모두 한글 문자열·바이트가 같다 |
| 25 | `gWeatherEndsStringIds` 원시 날씨 2항목 | 그대로 | D4 #16 |
| 26 | `gReflectLightScreenSafeguardStringIds[B_MSG_SET_AURORA_VEIL]=PKMNAURORAVEIL` | 제외 | HnS `PKMNRAISEDDEFSPDEF` 매핑을 유지했다(사용자 요청 행). 같은 줄에 `// HnS:` 주석만 달았다 |
| 27 | `gKOFailedStringIds[B_MSG_KO_MISS]`→PKMNEVADEDATTACK | 그대로 | HnS·1.17.0 모두 쓰지 않는 표다 |
| 28 | `CureStatusBerryEffectStringID` FREEEZE→FREEZE, PROBLEM/NORMALIZED 삭제 | 수정해서 | FREEZE 이름만 바꿨다. PROBLEM/NORMALIZED는 함정 3에 따라 유지했다(`// HnS:`) |
| 29 | `gDefogHazardsStringIds` → `gRemoveHazardsStringIds` | 그대로 | |
| 30 | `gSpinHazardsStringIds` 삭제, `gCureStatusStringIds`·`gPartyCureStatusStringIds`·`gHurtByStringIds`·`gBreakScreensStringIds` 추가 | 수정해서 | gSpin 삭제는 그대로다(강철 `PKMNBLEWAWAYSHARPSTEEL`↔`SHARPSTEELDISAPPEAREDFROMTEAM` 바이트 동일). HnS `gStatusCureStringIds`(:1254)를 지우고 upstream 이름 `gCureStatusStringIds`로 upstream 위치에 다시 만들었다. PROBLEM/NORMALIZED 대비값 2줄을 더했다(`// HnS:`). gPartyCure·gHurtBy는 그대로다. gBreakScreens는 제외했다(함정 2) |

추가(HnS, upstream hunk 아님): D7 `STRINGID_PKMNWOKEUPINUPROAR` `{B_ATK_NAME_WITH_PREFIX}` → `{B_EFF_NAME_WITH_PREFIX}`.

## 2. HnS 보존·적응 (`// HnS:` 위치)

- `battle_string_ids.h`
  - `enum StringID`의 `STRINGID_PKMNATKNOTLOWERED` 뒤 주석 2줄: WOREOFF 3개가 이미 있다는 것, STICKYWEB YOU를 넣지 않았다는 것.
  - 파일 끝, `HurtByStringID` 뒤 주석 2줄: gBreakScreens와 인덱스형 enum을 넣지 않았다는 것. HnS 값형 `B_MSG_BREAK_*`와 `CMP_COMMON_BITS` 유지.
- `battle_message.c`
  - `[STRINGID_PKMNAURORAVEIL]` 줄 끝: PKMNRAISEDDEFSPDEF와 같은 본문.
  - `gReflectLightScreenSafeguardStringIds[B_MSG_SET_AURORA_VEIL]` 줄 끝: 사용자 요청 매핑 유지.
  - `CureStatusBerryEffectStringID`: PROBLEM/NORMALIZED 유지.
  - `gCureStatusStringIds`: PROBLEM/NORMALIZED 대비값(`STRINGID_PURIFYTARGETSTATUSNORMAL`). `GetCuredStatusMessage()`가 아직 PROBLEM(9)을 반환할 수 있다. 이 항목이 없으면 표(9칸) 밖을 읽는다.
- 주석 없이 보존한 것
  - 한글 문장 전부(D4)
  - `gText_StatSharply`·`gText_drastically`·`gText_DefendersStatRose`
  - `gPurifyStatusCureStringIds`(FREEZE 이름만 교체)
  - `gFlashFireStringIds`(D2), HEALERCURE, PKMNSXMADEITINEFFECTIVE
  - HnS WOREOFF 위치·본문(D6-a 미수정)

## 3. 다른 파트와의 계약

### 3-1. 이 패치가 제공하는 심볼

**STRINGID**
- 추가: `STRINGID_PKMNAURORAVEIL`, `STRINGID_PKMNHEALEDPOISON`(PASTELVEILENTERS 대체), `STRINGID_SCRCUREDPARALYSIS/POISON/BURN/SLEEP/CONFUSION`, `STRINGID_PARTYCUREDPARALYSIS/POISON/BURN/SLEEP/FREEZE/FROSTBITE`, `STRINGID_PKMNATKNOTLOWERED`
- 삭제: `STRINGID_ATTACKMISSED`, `STRINGID_PASTELVEILENTERS`
- **없음**: `STRINGID_STICKYWEBDISAPPEAREDFROMYOU`
- 기존 유지: `STRINGID_REFLECTWOREOFF/LIGHTSCREENWOREOFF/AURORAVEILWOREOFF`(HnS)

**enum 값**
- `B_MSG_WEATHER_END_*`: RAIN0 SUN1 SANDSTORM2 HAIL3 SNOW4 FOG5 **EXTREMELY_HARSH_SUNLIGHT6 HEAVY_RAIN7** STRONG_WINDS8 COUNT9
- `enum CureStatusEffectStringID`: PARALYSIS0 POISON1 BURN2 **FREEZE3** FROSTBITE4 SLEEP5 **CONFUSION6 INFATUATION7 TAUNT8** PROBLEM9 NORMALIZED_STATUS10
  - **`B_MSG_CURED_FREEEZE`는 더 이상 없다.**
- `MentalHerbCureStringID`(비트 번호, `1 << 값`): INFATUATION0 TORMENT1 DISABLE2 HEALBLOCK3 ENCORE4 TAUNT5
- `HurtByStringID`: `B_MSG_HURT`=0, `B_MSG_HURT_BY_ITEM`=1
- 그대로 둔 것
  - `B_MSG_BREAK_REFLECT/LIGHT_SCREEN/AURORA_VEIL` = 1, 2, 4(값형)
  - `B_MSG_SET_AURORA_VEIL`
  - `B_MSG_MISSED/PROTECTED/AVOIDED_ATK`

**표(asm에서 `printfromtable`로 쓰는 이름)**
- `gCureStatusStringIds`(새 표, **HnS `gStatusCureStringIds`를 대체**)
  - 모든 값이 `{B_SCR}`다.
  - FREEZE→PKMNWASDEFROSTED, FROSTBITE→PKMNFROSTBITEHEALED, INFATUATION→PKMNGOTOVERITSINFATUATION, TAUNT→PKMNSHOOKOFFTHETAUNT
  - PROBLEM/NORMALIZED→PURIFYTARGETSTATUSNORMAL
- `gPartyCureStatusStringIds`(새 표): 6상태는 `{B_BUFF1}` 주어, CONFUSION/INFATUATION/TAUNT는 upstream 값
- `gHurtByStringIds`(새 표): HURT→AFTERMATHDMG, HURT_BY_ITEM→PKMNHURTSWITH(두 한글 바이트 동일)
- `gRemoveHazardsStringIds`: `gDefogHazardsStringIds` 이름 변경. **`gSpinHazardsStringIds` 삭제**
- `gMentalHerbCureStringIds`: 비트 인덱스 순서로 재배치. **비트마스크 값으로 `printfromtable` 하면 안 된다**
- `CureStatusBerryEffectStringID`: CONFUSION·PROBLEM·NORMALIZED 포함
- `gPurifyStatusCureStringIds`: HnS 유지
- `gMissStringIds[B_MSG_MISSED]`=PKMNAVOIDEDATTACK
- `gWeatherEndsStringIds`(9칸), `gAbilityWeatherChangeStringId`(upstream ID)
- `gReflectLightScreenSafeguardStringIds`: AURORA_VEIL은 HnS 매핑 유지
- `gBreakScreensStringIds`: **넣지 않음**
- `include/battle_message.h`에 extern을 추가하지 않았다(upstream과 같이 asm에서만 참조). C에서 이 표들을 쓰려면 그 파트가 extern을 추가한다.

### 3-2. 파트 B(스크립트)에 가정하는 것 — 빠지면 링크·빌드 실패

1. `printfromtable gStatusCureStringIds`를 모두 `printfromtable gCureStatusStringIds`로 바꾼다(HEAD `battle_scripts_1.s` :391 :761 :1280 :3885 :6317 :7136).
   - 출력 문장의 이름이 `{B_SCR}`이므로 각 경로에서 `gBattleScripting.battler`가 회복 대상이어야 한다. C 쪽은 이미 그렇다: `BS_CureStatus`, `Cmd_curestatuswithmove`, 탈피 `battle_util.c:3744`, `TryImmunityAbilityHealStatus :9122`.
2. `BattleScript_SpinHazardsAway`(:5114)·`BattleScript_DefogClearHazards`(:5119)를 upstream `BattleScript_RemoveHazards`로 합친다(`printfromtable gRemoveHazardsStringIds`).
   - STICKYWEB YOU를 넣지 않으므로 upstream 스크립트의 앞 4줄(`jumpifbyte CMP_NOT_EQUAL … HAZARDS_STICKY_WEB …`, `jumpifside …`, `printstring STRINGID_STICKYWEBDISAPPEAREDFROMYOU`, `goto …Ret`)과 `…Cont`/`…Ret` 라벨을 빼고 다음 세 줄만 둔다: `printfromtable gRemoveHazardsStringIds` / `waitmessage B_WAIT_TIME_LONG` / `return`
   - 양 진영 모두 `STICKYWEBDISAPPEAREDFROMTEAM`이 나와서 지금 HnS 출력과 같다.
   - extern 이름 변경(`include/battle_scripts.h`)과 C 호출 2곳(`battle_script_commands.c:7268`, `:9621`)도 함께 바꾼다.
3. `printstring STRINGID_PASTELVEILENTERS`(:5941)를 `STRINGID_PKMNHEALEDPOISON`으로 바꾼다.
4. `battle_scripts_2.s` :94는 `printfromtable gPartyCureStatusStringIds`, :101은 `printfromtable gCureStatusStringIds`로 바꾼다(upstream). 파트 C가 MULTISTRING을 설정해야 한다.
5. 멘탈허브 `BattleScript_MentalHerbCureRet`와 HnS 전용 `BattleScript_MentalHerbCureFling`
   - 둘 다 `jumpifbyte CMP_BITMASK, cMULTISTRING_CHOOSER, B_MSG_MENTALHERBCURE_*` 분기와 효과별 `printstring`으로 바꾼다(함정 1).
   - 쓰는 문자열: ATKGOTOVERINFATUATION / TORMENTEDNOMORE / PKMNMOVEDISABLEDNOMORE / HEALBLOCKEDNOMORE / PKMNENCOREENDED / PKMNSHOOKOFFTHETAUNT
   - D6-b: DISABLE·ENCORE 두 문장은 `{B_ATK_NAME_WITH_PREFIX}`다. 분기 안에서 공격자를 스크립트 배틀러로 잠시 바꾼다(예: `saveattacker` / `copybyte gBattlerAttacker, sBATTLER` / `restoreattacker`). 문장은 그대로 둔다.
   - `CMP_BITMASK`(=6) 정의는 `include/constants/battle_script_commands.h`(파트 B 파일)에 들어가야 한다.
6. `BattleScript_IntimidatePrevented`에서 `printstring STRINGID_PKMNATKNOTLOWERED`
7. `BattleScript_HurtAttacker`에서 `printfromtable gHurtByStringIds`. #9856의 중복 `printstring`은 넣지 않는다.
8. `BattleScript_BreakScreens*`는 HnS 그대로 둔다(`CMP_COMMON_BITS` + 값형 `B_MSG_BREAK_*`). `CMP_BITMASK`를 `B_MSG_BREAK_*`에 쓰지 않는다.
9. `BattleScript_HealerActivates`의 `printstring STRINGID_HEALERCURE`, 도주 특성 `PKMNFLEDUSING`(D3), DampPreventsAftermath 무문구는 그대로 둔다.

### 3-3. 파트 C(C 코드)에 가정하는 것 — 빠지면 컴파일 실패

1. FREEEZE 오타: 이 파트가 맡은 곳은 헤더 1곳과 표 3곳(`gCureStatusStringIds`, `gPurifyStatusCureStringIds`, `CureStatusBerryEffectStringID`)이다. 파트 C는 코드 3곳을 `B_MSG_CURED_FREEZE`로 바꾼다.
   - `battle_script_commands.c:12405`
   - `battle_util.c:9679` `GetCuredStatusMessage`
   - `battle_hold_effects.c:720`
   - upstream이 새로 넣는 곳(`pokemon.c` HealStatusConditions, curestatuswithmove, ItemHealMonVolatile 등)도 FREEZE 철자다.
2. `Cmd_resultmessage`(`battle_script_commands.c:2051`): `stringId = STRINGID_PKMNAVOIDEDATTACK; // HnS:` (D1). upstream의 PKMNEVADEDATTACK이 아니다.
   - AccuracyCheck의 `MISS_TYPE = STRINGID_PKMNEVADEDATTACK`(upstream)는 `B_MSG_PROTECTED`와 비교하는 데만 쓰여 무해하다. 파트 C가 판단한다.
3. `GetCuredStatusMessage()`는 PROBLEM 반환을 유지해도 된다(표가 받는다).
4. `TryMentalHerb`(HnS :422)
   - `MULTISTRING=0`으로 초기화하고 `|= 1 << B_MSG_MENTALHERBCURE_*`로 설정한다. Fling 경로도 같다.
   - 단일 값을 대입하면 새 enum 순서에서 잘못된 문장이 나온다.
5. `B_MSG_HURT_BY_ITEM`(울퉁불퉁멧·자보·애터 열매)와 `B_MSG_HURT`(까칠한피부·철가시·니들가드 경로)를 설정한다.
6. `pokemon.c` HealStatusConditions·`BS_ItemCureStatus`가 B_MSG_CURED_*를 설정한다. 설정하지 않으면 남아 있던 값으로 표를 읽는다.
7. 원시 날씨를 지울 때 `B_MSG_WEATHER_END_HEAVY_RAIN` / `_EXTREMELY_HARSH_SUNLIGHT`를 설정한다(D4 #16).
8. 방벽 깨기 `SetMoveEffect`의 upstream `|= 1 << 0/1/2` hunk는 제외하고 HnS `|= B_MSG_BREAK_*`를 유지한다. `B_MSG_SET_AURORA_VEIL` 중복 hunk도 제외한다.
9. D7: 코드 변경 없음. 세 경로 모두 `gEffectBattler`가 깬 포켓몬이다.
   - 행동 전: `battle_move_resolution.c:120`
   - 턴 종료: `battle_end_turn.c:1238-1246`
   - 배틀팰리스: `battle_util2.c:140`
   - 판단 근거: seq 125 결과 문서 :243

### 3-4. 파트 D·메인

- `BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 D7 1행을 새로 추가하고 5절의 D4 항목을 적는다.
- `test/text.c` "Battle strings fit on the battle message window"를 통합 빌드 뒤 돌려 새 문자열 12개를 확인한다.

## 4. 한글이 든 줄 변경 (전부)

| ID | 전 | 후 | 바이트(EOS 제외)·줄 바이트 | 점검 |
|---|---|---|---|---|
| ATTACKMISSED | `그러나 {B_ATK_NAME_WITH_PREFIX}의\n공격은 빗나갔다!` | (삭제) | — | D1 |
| PKMNAURORAVEIL | (없음) | `{B_ATK_PREFIX2} {B_CURRENT_MOVE}{B_TXT_EU}로\n물리공격과 특수공격에 강해졌다!` | 41 [9,31] | PKMNRAISEDDEFSPDEF와 **바이트 동일** |
| PKMNWOKEUPINUPROAR | `{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n소란스러워서 눈을 떴다!` | `{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n소란스러워서 눈을 떴다!` | 28 [4,23], 길이 같음 | D7. 토큰 FD 0F→FD 11. 조사 근거는 아래 |
| PASTELVEILENTERS→PKMNHEALEDPOISON | ID `PASTELVEILENTERS` | ID `PKMNHEALEDPOISON`, 본문 `{B_DEF_NAME_WITH_PREFIX}의 독은 말끔하게 해독됐다!` 그대로 | 28 [28] | 본문 바이트 동일 |
| SCRCUREDPARALYSIS | (없음) | `{B_SCR_NAME_WITH_PREFIX}의\n몸저림이 풀렸다!` | 21 [4,16] | PURIFYTARGETPARALYSISCURED와 바이트 동일(D5) |
| SCRCUREDPOISON | (없음) | `{B_SCR_NAME_WITH_PREFIX}의 독은\n말끔하게 해독됐다!` | 28 [9,18] | PURIFYTARGETPOISONCURED와 바이트 동일 |
| SCRCUREDBURN | (없음) | `{B_SCR_NAME_WITH_PREFIX}의\n화상이 나았다!` | 19 [4,14] | PURIFYTARGETBURNCURED와 바이트 동일 |
| SCRCUREDSLEEP | (없음) | `{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 눈을 떴다!` | 15 [15] | PURIFYTARGETSLEEPCURED와 바이트 동일 |
| SCRCUREDCONFUSION | (없음) | `{B_SCR_NAME_WITH_PREFIX}의\n혼란이 풀렸다!` | 19 [4,14] | PKMNHEALEDCONFUSION과 길이·줄 같음, 토큰만 SCR |
| PARTYCUREDPARALYSIS | (없음) | `{B_BUFF1}의\n몸저림이 풀렸다!` | 21 [4,16] | PKMNPARALYSISCURED 본문, 주어만 BUFF1(D5) |
| PARTYCUREDPOISON | (없음) | `{B_BUFF1}의 독은\n말끔하게 해독됐다!` | 28 [9,18] | PKMNPOISONCURED 본문 |
| PARTYCUREDBURN | (없음) | `{B_BUFF1}의\n화상이 나았다!` | 19 [4,14] | PKMNBURNCURED 본문 |
| PARTYCUREDSLEEP | (없음) | `{B_BUFF1}{B_TXT_EUNNEUN} 눈을 떴다!` | 15 [15] | PKMNWOKEUP 본문 |
| PARTYCUREDFREEZE | (없음) | `{B_BUFF1}의\n얼음이 녹았다!` | 19 [4,14] | PKMNWASDEFROSTED 본문 |
| PARTYCUREDFROSTBITE | (없음) | `{B_BUFF1}의\n동상이 나았다!` | 19 [4,14] | PKMNFROSTBITEHEALED 본문 |
| PKMNATKNOTLOWERED | (없음) | `{B_SCR_NAME_WITH_PREFIX}의\n공격은 떨어지지 않는다!` | 28 [4,23] | PKMNSXPREVENTSYLOSS(26 B [4,21])에 BUFF1=`공격`을 넣은 화면과 같다. 소스는 `{B_BUFF1}{B_TXT_EUNNEUN}`(4 B) 대신 `공격은`(6 B) |

- 바이트는 저장소 `charmap.txt` 기준으로 직접 인코딩해 셌다. 한글 2 B, `{B_…}` 2 B, `\n` 1 B, 공백·`!` 1 B.
- **창 폭**
  - 모든 새 문장은 기존 문장과 줄 구성(줄바꿈 위치)이 같다.
  - PARTYCURED*의 주어 `{B_BUFF1}`은 `BS_ItemCureStatus`가 넣는 종족명이다. 접두어(`상대 `/`야생 `)가 없어서 `{B_SCR_NAME_WITH_PREFIX}`보다 짧거나 같다.
  - PKMNATKNOTLOWERED 둘째 줄은 PREVENTSYLOSS에 가장 긴 능력치명(`특수공격` 등)이 들어간 경우보다 짧다.
- **조사(D7 근거)**
  - `BattleStringExpandPlaceholders`(`battle_message.c:3234-3252`)는 `B_TXT_EUNNEUN` 등을 만나면 바로 앞까지 dst에 써 넣은 출력의 마지막 2바이트로 받침을 판정한다: `jong = GetJongCode((dst[dstID-2] << 8) | dst[dstID-1])`.
  - `GetJongCode`(`src/korean.c:32`)는 한글 2바이트 글자와 숫자 1바이트를 처리한다.
  - 판정이 토큰 종류와 무관하게 직전에 펼친 이름의 실제 끝 글자를 보므로, EFF로 바꿔도 `은/는`이 깬 포켓몬 이름에 맞게 붙는다. 한글 본문과 조사 토큰은 그대로다.
  - 새 PARTY/SCR 문장의 `{B_BUFF1}{B_TXT_EUNNEUN}`도 같은 원리로 맞는다. 기존 `ITEMCUREDSPECIESSTATUS`가 같은 형태를 이미 쓴다.

## 5. 출력 변화(이 파트의 표·문장이 관여하는 것)

| 상황 | 이전 → 이후 | 구분 |
|---|---|---|
| 일반 공격 빗나감 | `그러나 {공격자}의\n공격은 빗나갔다!` → `{대상}에게는\n맞지 않았다!` | D4 표 #1(D1 C안). 이 파트의 `gMissStringIds`와 파트 C의 resultmessage가 함께 바꾼다 |
| 회복봉인 턴 종료 | BUFFERENDS → HEALBLOCKEDNOMORE | #2. 문장은 이미 있다. 경로는 파트 B·C |
| 멘탈허브 | 한 문장 → 풀린 효과마다 한 문장 | #3. 도발 문장은 BUFFERENDS(BUFF1=도발)→PKMNSHOOKOFFTHETAUNT로 화면 바이트가 같다 |
| 위협 방지 | `…은(는)\n{특성}의 효과로 능력이 떨어지지 않는다!` → `…의\n공격은 떨어지지 않는다!` | #9. 새 문장 PKMNATKNOTLOWERED. 경로는 파트 B |
| 가방 상태회복 도구 | `{종족}은(는)\n건강해졌다!` → `{종족}의\n몸저림이 풀렸다!` 등 상태별 문장 | #15. gPartyCure/gCureStatus 표, 경로는 B·C |
| 테라폼제로가 원시 날씨를 지움 | `햇살이 약해졌다!`/`비가 그쳤다!` → `햇살이 원래대로 되돌아왔다!`/`강한 비가 그쳤다!` | #16. gWeatherEnds 표, 설정은 C |
| 상태 회복 이름·동상 | 이름 `{B_ATK}`→`{B_SCR}`. 동상 `…의\n상태이상이 나았다!`→`…의\n동상이 나았다!`. 특성(마이페이스 등) 혼란 회복 이름도 교정 | #17. gCureStatusStringIds |
| 턴 종료 소란 기상 | 소란 사용자 이름 → 실제로 깬 포켓몬 이름(`…은(는)\n소란스러워서 눈을 떴다!`) | **새로 생긴 것(D7, D4 17건 밖)**. 출력 변경 문서에 1행 추가 필요 |
| 날씨 특성 시작·파스텔베일·고속스핀 강철·접촉 피해·오로라베일 | 변화 없음 | ID 교체지만 한글 바이트가 같거나 HnS 매핑을 유지했다 |

## 6. 위험·미해결·실기 후보

- **질문 1(D5 범위)**
  - 친구 답장 D5는 "도구에 의한 회복은 따로 `마비가 풀렸다!`"라고 했다.
  - `PARTYCUREDPARALYSIS`는 가방 도구(마비치료제 등) 회복 문장이다. 현재 패치는 `new_sentences.tsv`와 지시대로 `몸저림이 풀렸다!`를 썼다.
  - 이 구분이 지닌 열매(PKMNSITEMCUREDPARALYSIS)만 가리키는지, 가방 도구도 포함하는지 확인이 필요하다. 바꾼다면 기존 열매 본문의 `마비가 풀렸다!` 조합이다(새 번역 아님).
- **질문 2(대비값)**
  - `gCureStatusStringIds`의 PROBLEM/NORMALIZED 대비값으로 `PURIFYTARGETSTATUSNORMAL`(`{B_SCR}의\n상태이상이 나았다!`)을 골랐다. HnS는 이전에 같은 본문의 `{B_ATK}`판(PKMNSTATUSNORMAL)을 썼다.
  - 정상 경로에서는 나오지 않는다(`STATUS1_ANY`가 6상태로 모두 분기된다). 표 안에 대비값을 두는 것이 목적이다.
- 위험
  - 파트 B·C의 이름 변경(3-2·3-3)과 같이 넣지 않으면 컴파일·링크가 실패한다(gStatusCureStringIds, gSpin/DefogHazards, PASTELVEILENTERS, ATTACKMISSED, FREEEZE).
  - 의미상 함정: 멘탈허브 비트마스크를 C와 스크립트 양쪽에 맞추지 않으면 문장이 잘못 나오거나 표 밖을 읽는다.
  - STRINGID 숫자 값이 대부분 바뀐다(ATTACKMISSED −1, AURORAVEIL +1, 끝 12개 +12). 세이브에는 저장되지 않는다. 같은 ROM끼리만 통신하므로 영향은 없다.
  - 쓰이지 않게 되는 문자열: PKMNAURORAVEIL(HnS 매핑 유지 때문), PKMNPARALYSIS/POISON/BURNCURED, PKMNSTATUSNORMAL, ITEMCUREDSPECIESSTATUS, PKMNBLEWAWAYSHARPSTEEL. upstream처럼 남겼다(ROM 수백 B).
  - `gKOFailedStringIds`는 upstream대로 EVADED다. HnS와 1.17.0 모두 이 표를 쓰지 않는다.
- 빌드 확인 메모(실행하지 않음)
  - `enum CureStatusBerryEffectStringID` 타입명을 쓰는 C 코드는 HEAD에 없다(git grep).
  - asm `enum` 파서는 enum 안의 `//` 주석을 이미 처리한다(기존 `STRINGID_TABLE_START, // …`).
- 실기 확인 후보
  - 일반 빗나감 문장
  - 가방 상태회복 도구 5상태
  - 정글힐 아군 회복 이름
  - 마이페이스·둔감 혼란 회복 이름
  - 테라폼제로 원시 날씨 해제
  - 소란 턴 종료 기상 이름(D7)
  - 멘탈허브 다중 해제와 도발 문장
  - 위협 방지(클리어바디 등) 문장
