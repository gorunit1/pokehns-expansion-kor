# review-R4 — seq 127 #9655 출력 변경 문서·테스트 리뷰

- 리뷰어: R4(읽기 전용). 대상은 `git diff`(HEAD `05319fd9b7`) 가운데 `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`와 `test/**`이다.
- 수정·빌드·테스트는 하지 않았다. 확인은 `git diff`/`git show`/파일 읽기, upstream `32fcd64868`(#9655)·`76da6ac3d8`(master merge)·`3ed1ce5570`(#10064)·`expansion/1.17.0` 대조로 했다.
- 먼저 확인한 사실
  - 작업 트리의 `test/**` + 문서 diff는 `part-D.patch`와 파일 143개 모두 내용이 같다(index 줄 제외). **메인이 문서 초안을 최종 코드에 맞춰 고치지 않은 상태**다.
  - 문서에 새로 들어가거나 고쳐진 행의 한글 인용 54개를 HEAD·작업 트리 `src/battle_message.c`와 바이트 단위로 대조했다. 모두 일치한다(`…`로 시작하는 렌더링 예시 5개와 용어 `몸저림`, `무사히 도망쳤다` 부분 문자열은 제외).
  - 문서가 가정한 STRINGID·표·스크립트 라벨·함수는 실제 코드에 모두 있다: `STRINGID_PKMNAVOIDEDATTACK`(resultmessage, `// HnS:`), `BattleScript_HealBlockEndTurn`, `gRemoveHazardsStringIds`/`BattleScript_RemoveHazards`, `gCureStatusStringIds`/`gPartyCureStatusStringIds`, `STRINGID_SCRCURED*`/`PARTYCURED*`/`PKMNATKNOTLOWERED`, `gHurtByStringIds`, `B_MSG_WEATHER_END_EXTREMELY_HARSH_SUNLIGHT`/`HEAVY_RAIN`, `CMP_BITMASK`, D6-b의 `saveattacker`/`copybyte gBattlerAttacker, sBATTLER`/`restoreattacker`, D7 토큰.
  - 표에 쓴 config 값(`B_UPDATED_INTIMIDATE`·`B_MENTAL_HERB` = `GEN_LATEST`, `B_UPROAR` = `GEN_4`, `B_USE_FROSTBITE` = `FALSE`, `B_HEAL_BELL_SOUNDPROOF` = `GEN_LATEST`)이 `include/config/battle.h`와 맞다.

## 집계

| 등급 | 건수 |
|---|---|
| 수정 필요 | 1 |
| 경미 | 11 |
| 확인만 | 9 |

---

## 수정 필요

### N1. 가방 도구 행: 만병통치제·회복약으로 헤롱헤롱만 풀면 실제로는 혼란 문장이 나온다

- 등급: **수정 필요**(문서의 "현재 출력"이 실제와 다름. 엔진 동작 자체는 upstream 1.17.0과 같은 결함)
- 위치: `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md:52` "현재 출력"의 `헤롱헤롱 STRINGID_PKMNGOTOVERITSINFATUATION`
- 근거
  - `src/battle_util.c:10283-10290` `ItemHealMonVolatile()`의 첫 분기(`effect[3] & ITEM3_STATUS_ALL`)는 헤롱헤롱도 풀지만 선택값을 무조건 혼란으로 둔다.
    ```c
    if (effect[3] & ITEM3_STATUS_ALL)
    {
        statusChanged = (gBattleMons[battler].volatiles.infatuation || ...confusionTurns > 0 || ...);
        gBattleMons[battler].volatiles.infatuation = 0;
        ...
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_CURED_CONFUSION;
    }
    ```
  - 만병통치제(`gItemEffect_FullHeal`)와 회복약(`gItemEffect_FullRestore`)은 `[3] = ITEM3_STATUS_ALL`이다. 헤롱헤롱 선택값(`B_MSG_CURED_INFATUATION`)은 빨강비드로(`ITEM0_INFATUATION`)만 탄다.
  - `src/item_use.c:1397-1406`의 `SelectedMonHasVolatile()` 때문에 헤롱헤롱만 걸린 싸우는 포켓몬에게도 두 도구를 쓸 수 있다.
  - 상태이상과 혼란·헤롱헤롱이 같이 있으면 `HealStatusConditions()`(`src/pokemon.c:6553-`)가 선택값을 상태이상으로 덮어써서 상태이상 문장 하나만 나온다. 혼란·헤롱헤롱 해제는 문장 없이 처리된다.
- 실패 시나리오
  - 헤롱헤롱만 걸린 싸우는 포켓몬(예: 피카츄)에게 만병통치제 사용 → `피카츄의\n혼란이 풀렸다!`(`STRINGID_SCRCUREDCONFUSION`). 이전(HEAD)은 `피카츄은(는)\n건강해졌다!`.
  - 마비 + 혼란인 싸우는 포켓몬에게 만병통치제 사용 → `…의\n몸저림이 풀렸다!`만 나오고 혼란도 풀린다.
- 제안
  - 문서: 52행 "현재 출력"을 실제대로 고친다. 예: "헤롱헤롱 `STRINGID_PKMNGOTOVERITSINFATUATION`(빨강비드로만). 만병통치제·회복약 등 모든 상태를 고치는 도구는 헤롱헤롱만 풀어도 혼란 문장(`STRINGID_SCRCUREDCONFUSION`)이 나오고, 상태이상이 함께 있으면 상태이상 문장 하나만 나온다(upstream 1.17.0과 같음)". 상황 칸에 `회복약`과 비드로도 적는다(회복약은 `BattleScript_ItemHealAndCureStatus`(`data/battle_scripts_2.s:105`)가 같은 `itemcurestatus` 경로를 탄다).
  - 엔진: upstream 1.17.0의 `ItemHealMonVolatile()`도 같다(`expansion/1.17.0:src/battle_util.c:10197-10222`). HnS만 고치면 새 HnS 차이가 되므로 이번에는 기록만 하고, 고칠지는 친구에게 묻는 것을 권한다. 실기 확인 후보에도 넣는다.

---

## 경미

### M1. D1 행의 상황 설명이 공격 기술로 좁혀져 있다

- 위치: 문서 31행 "공격 기술이 명중 판정으로 빗나감(`resultmessage`)"
- 근거: `data/battle_scripts_1.s`의 `accuracycheck BattleScript_MoveMissedPause`가 41곳이다. 부식성가스·문어굳히기·타르샷·힘흡수·막말내뱉기·사이코시프트·독실·물붓기 같은 변화기도 이 경로로 `resultmessage`에 가고, HEAD에서는 `STRINGID_ATTACKMISSED`가 나왔다. #9929 행(24행)은 `accuracycheck BattleScript_ButItFailed` 기술만 다룬다.
- 시나리오: 사이코시프트가 빗나감 → HEAD `그러나 {사용자}의\n공격은 빗나갔다!` → 현재 `{대상}에게는\n맞지 않았다!`. 문서만 읽으면 변화기는 바뀌지 않은 것처럼 보인다.
- 제안: 상황 칸을 "명중 판정으로 빗나가 `resultmessage`로 가는 모든 기술(공격 기술과 `accuracycheck BattleScript_MoveMissedPause`를 쓰는 변화기)"로 고친다.

### M2. 상태 회복 이름 행에 초승달의기도가 빠졌다

- 위치: 문서 51행 상황 칸 "브레이브차지·리프레시·정글힐·사이코시프트, …"
- 근거: `MOVE_LUNAR_BLESSING`은 `EFFECT_JUNGLE_HEALING`이고 `BattleScript_EffectJungleHealing`의 `curestatus BS_TARGET` + `printfromtable gCureStatusStringIds`(`data/battle_scripts_1.s:761`)를 같이 쓴다. 40행(기존 행)에는 초승달의기도가 들어 있다.
- 제안: 51행 상황 칸에 `초승달의기도`를 추가한다.

### M3. 특성 이름이 HnS 공식명과 다르다(고친 행 안)

- 위치: 문서 40행과 51행의 `촉촉한몸`, 49행의 `자기 페이스`, `천진`
- 근거: `src/data/abilities.h` HnS 이름은 `ABILITY_HYDRATION` = `촉촉바디`, `ABILITY_OWN_TEMPO` = `마이페이스`, `ABILITY_OBLIVIOUS` = `둔감`이다. `천진`은 `ABILITY_UNAWARE`이고 상태를 고치지 않는다. 40·49행의 오기는 HEAD부터 있었지만 이번 diff가 두 행을 고쳤고, 51행은 새로 쓴 행이다.
- 제안: `촉촉한몸` → `촉촉바디`(40·51행), `자기 페이스` → `마이페이스`, `천진` → `둔감`(49행).

### M4. "HnS `GetCuredStatusMessage()` 선택은 유지"가 특성 경로에는 맞지 않는다

- 위치: 문서 51행 변경 지점
- 근거: HEAD에서는 `GetCuredStatusMessage()`를 4곳이 불렀다(`battle_script_commands.c` 2곳, `battle_util.c:3726` 탈피·촉촉바디, `battle_util.c:9101` `TryImmunityAbilityHealStatus`). 작업 트리에서는 기술 경로 2곳(`Cmd_curestatuswithmove`, `BS_CureStatus`)만 남았다. 특성 경로는 upstream처럼 상태별 `B_MSG_CURED_*`를 직접 넣는다(`battle_util.c:3726-3740`, `9044-9107`). 출력은 같다.
- 제안: "기술 경로(`BS_CureStatus`·`Cmd_curestatuswithmove`)는 HnS `GetCuredStatusMessage()` 선택을 유지, 특성 경로(탈피·촉촉바디·면역 등)는 upstream처럼 상태별 선택값을 직접 설정"으로 고친다.

### M5. #9655 뒤 낡은 "변경 지점" 2행(출력은 그대로)

- 위치: 문서 38행(특성으로 독·화상·마비·수면 발생), 43행(잠자기 성공)
- 근거
  - `SetNonVolatileStatus()`가 이제 항상 `B_MSG_STATUSED`를 고른다(`src/battle_script_commands.c:2473`). `B_MSG_STATUSED_BY_ABILITY` 매핑은 표에 남아 있지만 고르는 코드가 없다.
  - `BattleScript_EffectRest`가 `printfromtable gRestUsedStringIds` 대신 `printstring STRINGID_PKMNSLEPTHEALTHY`를 쓴다(`data/battle_scripts_1.s:2640`).
  - part-D 5절은 두 곳을 "변화 없음"으로 확인했지만 행의 변경 지점은 고치지 않았다.
- 제안: 각 행 변경 지점 끝에 "#9655 이식 뒤에는 `SetNonVolatileStatus()`가 항상 `B_MSG_STATUSED`를 고름(같은 ID)" / "#9655 이식 뒤에는 스크립트가 `STRINGID_PKMNSLEPTHEALTHY`를 직접 출력"을 덧붙인다.

### M6. 장판 행의 "같은 장판별 ID"는 고속스핀의 강철가시에서 정확하지 않다

- 위치: 문서 20행 변경 지점
- 근거: HEAD `gSpinHazardsStringIds[HAZARDS_STEELSURGE]` = `STRINGID_PKMNBLEWAWAYSHARPSTEEL`이었고, 합친 표는 `STRINGID_SHARPSTEELDISAPPEAREDFROMTEAM`(`src/battle_message.c:1472`)이다. 두 한글은 `{B_ATK_TEAM2} 주변의\n강철이 사라졌다!`로 같아 출력은 같다.
- 제안: "(고속스핀·킬러스핀의 강철가시는 `PKMNBLEWAWAYSHARPSTEEL` → `SHARPSTEELDISAPPEAREDFROMTEAM`, 한글 같음)"을 덧붙인다.

### M7. D6-b 행의 주석 표기·스크립트 위치

- 위치: 문서 74행 "(`// HnS:`)", "멘탈허브 스크립트(`BattleScript_MentalHerbCureRet`·`BattleScript_MentalHerbCureFling`)의 사슬묶기·앙코르 분기"
- 근거: 실제 주석은 어셈블리 주석 `@ HnS:`(`data/battle_scripts_1.s:7280`, `7292`)다. 분기는 두 스크립트가 함께 `call`하는 `BattleScript_MentalHerbCureMessages`(`7254`)의 `BattleScript_MentalHerbCuresDisable`/`…CuresEncore`에 있다. 73행도 같은 공용 서브루틴 구조다.
- 제안: "`BattleScript_MentalHerbCureRet`·`…Fling`이 함께 부르는 `BattleScript_MentalHerbCureMessages`의 사슬묶기·앙코르 분기(`@ HnS:`)"로 고친다.

### M8. 가방 도구 행: 대기 포켓몬 이름이 종족명이라는 점과 동상이 빠졌다

- 위치: 문서 52행 "현재 출력"
- 근거: `BS_ItemCureStatus()`가 `HealStatusConditions()`의 `PREPARE_MON_NICK_BUFFER` 뒤에 `PREPARE_SPECIES_BUFFER(gBattleTextBuff1, …)`로 덮어쓴다(`src/battle_script_commands.c:12383`, upstream 같음). 싸우는 포켓몬은 `{B_SCR_NAME_WITH_PREFIX}`라 별명이 나오고, 대기 포켓몬은 종족명이 나온다. 싸우는 포켓몬의 동상은 `STRINGID_PKMNFROSTBITEHEALED`다.
- 제안: "대기 중인 포켓몬(`{B_BUFF1}` = 종족명, 별명 아님)", 싸우는 포켓몬 목록에 "동상 `STRINGID_PKMNFROSTBITEHEALED`"를 추가한다.

### M9. 상태 회복 표의 PROBLEM·NORMALIZED 대체값이 문서에 없다

- 위치: 문서 51행
- 근거: `gCureStatusStringIds[B_MSG_CURED_PROBLEM/NORMALIZED_STATUS]` = `STRINGID_PURIFYTARGETSTATUSNORMAL`(`{B_SCR_NAME_WITH_PREFIX}의\n상태이상이 나았다!`, `src/battle_message.c:1493-1494`)이다. HEAD는 `STRINGID_PKMNSTATUSNORMAL`(`{B_ATK…}`)이었다. 기술 경로는 `STATUS1_ANY`/`STATUS1_CAN_MOVE` 안에서만 고치므로 지금은 닿지 않는 값으로 본다(추정).
- 제안: 변경 지점에 "HnS가 남긴 PROBLEM·NORMALIZED는 `STRINGID_PURIFYTARGETSTATUSNORMAL`로 대체(현재 경로에서는 나오지 않음)"를 한 줄 적는다.

### M10. `part-D-tests.tsv`의 "기준 목록에 없음(-)" 7행 중 4행은 실제로 기준에 FAIL로 있다

- 위치: `part-D-tests.tsv`의 `-` 행
- 근거: 기준 `pre127.txt`(바이너리 바이트가 섞여 `grep -a` 필요)에 다음처럼 있다. 이름 뒤 매개변수 접미사와 소스의 `\%` 이스케이프 때문에 대조가 빗나간 것으로 보인다.
  - `Keen Eye, Gen9+ Illuminate & Minds Eye prevent accuracy stage reduction from moves 1/3 (1/?): FAIL`
  - `Dynamax: G-Max Replenish recycles allies' berries 50% of the time (2/2): FAIL`
  - `Dynamax: G-Max Replenish recycles allies' berries 50% of the time, even if it faints the foe (2/2): FAIL`
  - `Spicy Extract bypasses accuracy checks (1/?): FAIL`
  - 정말 없는 것은 3개다: `Competitive doesn't activate if the pokemon lowers it's own stats`, `Defiant doesn't activate if the Pokémon lowers it's own stats`, `Roost suppresses the user's not-yet-aquired Flying-type this turn`.
- 제안: 결과 대조 때 위 4개는 FAIL→FAIL로 본다. 양성 영문 MESSAGE가 남아 있어 FAIL 유지가 예상이다.

### M11. 쓰이지 않는 `gKOFailedStringIds[B_MSG_KO_MISS]`가 D1 방향과 다르다(R4 범위 밖, 기록용)

- 위치: `src/battle_message.c:1283` `[B_MSG_KO_MISS] = STRINGID_PKMNEVADEDATTACK`
- 근거: 이 표는 어디서도 읽지 않아 지금 출력 영향은 없다(`grep` 결과 정의만 있음). upstream 1.17.0 최종형은 같은 칸이 `STRINGID_PKMNAVOIDEDATTACK`이다. 나중에 이 표를 쓰는 PR이 오면 D1(모든 빗나감 `…맞지 않았다!`)과 어긋날 수 있다.
- 제안: A/C 리뷰 담당에 넘긴다. 바꾸려면 1.17.0과 같은 `STRINGID_PKMNAVOIDEDATTACK`으로 두면 된다.

---

## 확인만

### C1. 테스트 patch 판단(수정 4·제외 13)이 맞다

- `raging_bull.c`: 작업 트리 = upstream `32fcd64868` 파일과 바이트가 같다.
- `iron_barbs.c`: 작업 트리 = upstream merge `76da6ac3d8` 파일과 바이트가 같다(TO_DO 유지, 새 테스트는 #9937 테스트 뒤). 1.17.0과의 차이(`enum Species`, `4 times`, `MESSAGE("Wobbuffet was hurt!")` 삭제)는 #10127·#9777 같은 뒤 PR의 몫이다.
- `poison_touch.c`: 바꾼 양성 MESSAGE 4줄이 1.17.0과 같다. 9655판에 있는 NONE_OF 안 MESSAGE 2줄이 없는 것은 HnS #9715 선반영 결과와 같다.
- `octolock.c` #5 제외: HnS #10042 이식이 이 테스트를 지웠고 1.17.0에도 없다.
- `break_screens.c` 12 hunk 제외: 9655판과의 차이는 ASSUME 1줄, 빈 줄, 변수명 `breakingMove`뿐이다.
- 나머지 133파일은 "HEAD↔9655 부모" 차이 줄 수와 "작업 트리↔9655" 차이 줄 수가 같다(직접 계산. 다른 것은 위 두 파일뿐).
- #10064 5파일(`ai/check_bad_move.c`, `move_effect/toxic.c`, `status1/burn.c`, `status1/paralysis.c`, `status1/sleep.c`)은 upstream `3ed1ce5570` 파일과 바이트가 같다. 옮긴 AI 테스트의 ASSUME(`EFFECT_NON_VOLATILE_STATUS`, `MOVE_EFFECT_PARALYSIS/BURN/TOXIC/SLEEP`)은 HnS `moves_info.h`와 맞다. 빠진 `WITH_CONFIG(B_PARALYZE_ELECTRIC, GEN_6)`은 HnS가 `GEN_LATEST`라 결과가 같다.
- 테스트 diff에 한글 줄은 0개다. 영문 `MESSAGE` 기대값을 한글로 바꾼 곳은 없다.

### C2. PASS→FAIL 1건(`steadfast.c`)과 FAIL→PASS 5건 예측은 코드로 설명된다

- 테스트별 양성 MESSAGE(NONE_OF/NOT 밖)를 HEAD와 작업 트리에서 직접 세었다. 1개 이상에서 0개로 바뀐 것은 정확히 dry_skin 2, poison_heal 2, rain_dish 1이다. 0개에서 1개로 바뀐 것은 steadfast 1이다.
- Steadfast: `BattleScript_TryActivateSteadFast`가 한글 `STRINGID_ATTACKERSSTATROSE`를 출력하므로 새 영문 `MESSAGE("Lucario's Speed rose!")`와 맞지 않는다. FAIL이 맞다.
- "PASS(조건)" 2건(`Dry Skin heals 1/8th Max HP in Rain`, `Rain Dish recovers 1/16th of Max HP in Rain`)의 조건은 작업 트리에서 충족된다. `BattleScript_AbilityHpHeal`에 `playanimation BS_ATTACKER, B_ANIM_SIMPLE_HEAL`이 있다(`data/battle_scripts_1.s:6265-6271`).
- 포이즌힐 2건은 HnS가 이미 문구가 없고 새 회복 애니메이션은 기대값과 부딪히지 않는다.

### C3. `Rocky Helmet damages attacker even if damage is blocked by Disguise`는 FAIL→PASS일 수 있다(미확인)

- 양성 MESSAGE가 0개이고, 이번 diff는 `ANIMATION(…, B_ANIM_HELD_ITEM_EFFECT, player)` 기대만 지운다. 엔진도 같은 애니메이션을 지웠다. 기준 FAIL 원인이 이 애니메이션이었다면 PASS로 바뀐다. 원인이 `IsBattlerTurnDamaged(EXCLUDING_SUBSTITUTES)`(탈 때문에 피해 0)라면 FAIL이 유지된다. tsv는 FAIL 유지로 적었다. 결과로 확인한다.

### C4. 문서 범위

- 새 행은 모두 코드·스크립트 변경으로 같은 상황의 문자열·팝업·표시 이름·애니메이션이 달라진 경우다. 포이즌힐·울퉁불퉁멧(애니메이션만), D6-b·D7·#17(이름만)은 범위 문장 "다른 문자열을 선택하거나 팝업만 표시"보다 넓다. 하지만 결정 D4("17건 전부 기록")·D6-b·D7을 따른 것이라 맞다. 원하면 "검증 상태" 단락에 "애니메이션·표시 이름 변화도 결정에 따라 기록"을 한 줄 덧붙인다.
- 바이트가 같아 행을 넣지 않은 교체가 실제로 같은지 확인했다: `ATTACKERABILITYSTATRAISE`↔`ATTACKERSSTATROSE`(불굴의마음 등), `TARGETABILITYSTATRAISE`↔`gText_DefendersStatRose`, `ENDUREDSTURDY`↔`PKMNENDUREDHIT`, `PKMNPREVENTSUSAGE`↔`POKEMONCANNOTUSEMOVE`, `PKMNHURTSWITH`↔`AFTERMATHDMG`, `PASTELVEILENTERS`→`PKMNHEALEDPOISON`, 날씨 특성 시작 5개.
- `PKMNSXMADEYUSELESS`(`{B_DEF}`)→`SCR_ITDOESNTAFFECT`(`{B_SCR}`)는 `MonMadeMoveUseless`의 유일한 실사용 경로(`battle_util.c:2474`)가 `gBattleScripting.battler = ctx->battlerDef`를 넣어서 이름도 같다.

### C5. D1·D7·D6-b·회복봉인·제로포밍·멘탈허브 순차 행의 설명이 코드와 맞다

- D7: 턴 종료 경로는 `gEffectBattler`를 깬 포켓몬으로 돌린다(`battle_end_turn.c:1231-1245`). 행동 전(`battle_move_resolution.c:120`)과 배틀팰리스(`battle_util2.c:140`)는 `gEffectBattler = 공격자`로 둔다. 문서 설명대로다.
- D6-b: 턴 종료 사슬묶기·앙코르 해제는 `gBattlerAttacker = battler`(`battle_end_turn.c:1536`)라 이전과 같다는 설명이 맞다.
- 회복봉인: 도발(`HandleEndTurnTaunt`)·전자부유는 계속 `BattleScript_BufferEndTurn`이다.
- 제로포밍: `RemoveAllWeather()`를 부르는 곳은 `BS_RemoveWeather`(`BattleScript_ActivateTeraformZero`) 하나다.
- 멘탈허브: HEAD 판정 순서(헤롱헤롱→도발→앙코르→트집→회복봉인→사슬묶기)와 현재 출력 순서가 문서와 같다.

### C6. 힐볼 포획: HEAD 대비 출력 변화 없음. 이 문서에 행을 넣지 않는 것이 맞다

- `FinalizeCapture()`가 `HealStatusConditions()` 앞뒤로 `MULTISTRING_CHOOSER`를 보존한다(`src/battle_script_commands.c:10771-10774`, `// HnS:`). upstream 1.17.0에는 이 보존이 없어 상태이상 포켓몬을 힐볼로 잡으면 파티 가득 참 분기가 흔들린다. HnS만의 차이이므로 seq 127 결과 문서에는 적는다.
- 덮어쓰이는 `gBattleTextBuff1`을 포획 뒤 문구(`GOTCHAPKMNCAUGHTPLAYER`·`PKMNDATAADDEDTODEX`·`GIVENICKNAMECAPTURED`·`gCaughtMonStringIds`)가 쓰지 않는 것도 확인했다.

### C7. #10064 엔진 1줄은 HnS 화면 출력을 바꾸지 않는다

- `IsNonVolatileStatusBlocked()`가 고르는 비특성 스크립트의 문장(`PKMNISALREADYPARALYZED` 등 이미 상태 문장, `MISTYTERRAINPREVENTS`, `PKMNUSEDSAFEGUARD`, `ITDOESNTAFFECT`)이 HnS에서는 모두 `{B_DEF…}`다. 그래서 문서에 행을 넣지 않은 것이 맞다.

### C8. 사령탑 `PREPARE_MON_NICK_BUFFER` 추가는 HnS 출력에 영향이 없다(D6-c 미착수 유지)

- HnS `STRINGID_COMMANDERACTIVATES`는 `{B_BUFF1}`이 아니라 `{B_DEF_NAME_WITH_PREFIX2}`를 쓴다.

### C9. `test/text.c` "Battle strings fit on the battle message window"

- 기준은 이미 FAIL이다(`(124/124) … : FAIL`). STRINGID가 순증 12개라 접미사 숫자가 달라질 수 있으므로 이름 앞부분으로 대조한다.

---

## 메인 대조용 테스트 이름(정확한 문자열)

- 결과 파일에서 매개변수 테스트의 FAIL 줄은 이름 뒤에 ` 1/2`, ` 1/3 (2/2)` 같은 접미사가 붙는다. 앞부분 일치로 대조한다.

### PASS→FAIL 예상(1)
- `Steadfast boosts Speed when the user attempts to move but is flinched`

### FAIL→PASS 예상(5)
- `Dry Skin causes 1/8th Max HP damage in Sun`
- `Dry Skin heals 1/8th Max HP in Rain`
- `Poison Heal heals from (Toxic) Poison damage`
- `Poison Heal heals from Toxic Poison damage are constant`
- `Rain Dish recovers 1/16th of Max HP in Rain`

### 미확인(FAIL 유지 예상, PASS로 바뀌어도 설명 가능)
- `Rocky Helmet damages attacker even if damage is blocked by Disguise`

### 새 테스트 PASS 예상(2)
- `Light Screen reduces special damage`
- `Light Screen applies for 5 turns`

### 이동, PASS 유지 예상(4, `test/battle/ai/check_bad_move.c`)
- `AI avoids Thunder Wave when it can not paralyse target`
- `AI avoids Will-o-Wisp when it can not burn target`
- `AI avoids hypnosis when it can not put target to sleep`
- `AI avoids toxic when it can not poison target`

### 사라지는 PASS 3과 PASS를 유지해야 할 짝(`break_screens.c`)
- `Raging Bull doesn't remove Light Screen, Reflect and Aurora Veil if it misses` ↔ `Brick Break, Psychic Fangs, and Raging Bull don't remove Light Screen, Reflect and Aurora Veil if it misses`
- `Raging Bull doesn't remove Light Screen, Reflect and Aurora Veil if the target Protected` ↔ `Brick Break, Psychic Fangs, and Raging Bull don't remove Light Screen, Reflect and Aurora Veil if the target Protected`
- `Raging Bull doesn't remove Light Screen, Reflect and Aurora Veil if the target is immune` ↔ `Brick Break, Psychic Fangs, and Raging Bull don't remove Light Screen, Reflect and Aurora Veil if the target is immune`

### 사라지는 FAIL 2(짝은 FAIL 유지)
- `Raging Bull can remove Light Screen, Reflect and Aurora Veil on users side` ↔ `Brick Break, Psychic Fangs, and Raging Bull can remove Light Screen, Reflect and Aurora Veil on users side`
- `Raging Bull removes Light Screen, Reflect and Aurora Veil from the target's side of the field` ↔ `Brick Break, Psychic Fangs, and Raging Bull remove Light Screen, Reflect and Aurora Veil from the target's side of the field`

### 이름 변경(옛 이름 → 새 이름, 모두 FAIL→FAIL)
- `Shed Skin triggers 33% (Gen3, Gen5+) or 30% (Gen 4) of the time` → `Shed Skin triggers 33% of the time (Gen 3)` / `Shed Skin triggers 30% of the time (Gen 4)` / `Shed Skin triggers 33% of the time (Gen 5)`
- `Jaboca Berry is triggered even if berry user dies` → `Jaboca Berry is triggered even if berry user faints`
- `Rowap Berry is triggered even if berry user dies` → `Rowap Berry is triggered even if berry user faints`
- `Ice Heal heals a battler from being frozen or frostbite` → `Ice Heal heals a battler from being frozen` / `Ice Heal heals a battler from frostbite`
- `Old Gateu heals a battler from any primary status` → `Old Gateau heals a battler from any primary status`

### TO_DO 감소 2(유지 1)
- 삭제: `TODO: Write Mental Herb (Hold Effect) test titles`, `TODO: Write Light Screen (Move Effect) test titles`
- 유지: `TODO: Write Iron Barbs (Ability) test titles`

### 새 테스트 FAIL 예상(22, 영문 양성 MESSAGE)
- `Rough Skin and Iron Barbs cause the attacker to take damage when using a contact move`
- `Oblivious cures infatuation and Taunt`
- `Shed Skin triggers 30% of the time (Gen 4)`, `Shed Skin triggers 33% of the time (Gen 3)`, `Shed Skin triggers 33% of the time (Gen 5)`
- `Lum Berry properly cures a battler affected by only confusion`
- `Jaboca Berry is triggered even if berry user faints`, `Rowap Berry is triggered even if berry user faints`
- `Mental Herb cures Disable volatile status (Gen 5+)`, `Mental Herb cures Encore volatile status (Gen 5+)`, `Mental Herb cures Heal Block volatile status (Gen 5+)`, `Mental Herb cures Taunt volatile status (Gen 5+)`, `Mental Herb cures Torment volatile status (Gen 5+)`, `Mental Herb cures infatuation`, `Mental Herb cures volatile statuses in the following order - Infatuation, Torment, Disable, Heal Block, Encore, Taunt`
- `Ice Heal heals a battler from being frozen`, `Ice Heal heals a battler from frostbite`, `Old Gateau heals a battler from any primary status`
- `Light Screen fails if already active`
- `Rapid Spin and Mortal Spin remove Leech Seed`
- `Thunder Wave prints already paralyzed message with the right target`, `Thunder Wave prints failure when the target already has a different non-volatile status`

### tsv가 "-"로 적었지만 기준에 FAIL로 있는 것(FAIL→FAIL 예상, M10)
- `Keen Eye, Gen9+ Illuminate & Minds Eye prevent accuracy stage reduction from moves`
- `Dynamax: G-Max Replenish recycles allies' berries 50% of the time`
- `Dynamax: G-Max Replenish recycles allies' berries 50% of the time, even if it faints the foe`
- `Spicy Extract bypasses accuracy checks`
