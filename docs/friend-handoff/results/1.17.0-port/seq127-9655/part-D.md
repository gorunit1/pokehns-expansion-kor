# part-D — seq 127 #9655 (+#10064) 테스트·출력 변경 문서 사전 분석

- 기준: 메인 HEAD `fcf855d4e8`. 스크래치 트리 `part-D/tree/`에서만 편집했다. make·빌드·테스트는 돌리지 않았다.
- 산출물
  - `part-D.patch` = `git -C part-D/tree diff` (143파일: 테스트 142 + `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`). `git -C /home/jinmo/pokehns-expansion-kor apply --check part-D.patch` 통과, `git diff --check` 깨끗함.
  - `part-D-tests.tsv` = 바뀌는 테스트 418행의 상태 예측(열: 파일, 테스트 이름(전/후), 기준 상태, 예상 상태, 이유). 기준은 `test-baseline-seq167-ahead1.txt`(= `chunk-127/pre127.txt`, 바이트 동일).
- `break_screens.c`는 바뀌지 않아 patch의 테스트 파일은 142개다(담당 143개 중).

## 1. upstream hunk 표

### 집계

| 출처 | hunk | 그대로 | 수정해서 | 제외 |
|---|---|---|---|---|
| #9655 테스트(138파일) | 408 | 391 | 4 | 13 |
| #10064 테스트(5파일) | 7 | 7 | 0 | 0 |

- `git apply`(fuzz 없음) 기준 실패는 17 hunk(5파일)다. `patch` dry-run의 16 hunk(4파일)에 `iron_barbs.c` 1 hunk가 더해졌다(`patch`는 fuzz로 붙였을 것).
- 그대로 들어간 hunk는 파일별로 "HnS HEAD↔upstream 9655 부모" 차이 줄 수와 "적용 후 트리↔upstream 9655" 차이 줄 수가 같다. 오프셋 적용 위치가 어긋난 곳은 없다.
- #10064 hunk는 #9655 테스트를 적용한 뒤에 그대로 맞았다.

### 그대로 / 수정 / 제외가 아닌 hunk

| 파일 | hunk | 내용 | 판정 | 이유 |
|---|---|---|---|---|
| `ability/iron_barbs.c` | #1 | TO_DO를 새 테스트 "Rough Skin and Iron Barbs cause the attacker to take damage when using a contact move"로 바꿈 | 수정해서 | HnS에 #9937(`17a7b84993`) 테스트가 이미 들어가 문맥이 다르다. 새 테스트를 #9937 테스트 뒤에 넣었고, **TO_DO 줄은 남겼다**. upstream 병합 `76da6ac3d8`과 1.17.0 최종형이 이 모양이다. |
| `ability/poison_touch.c` | #2, #4 | `was poisoned by Grimer's Poison Touch!` → `was poisoned!` (양성 1줄 + NONE_OF 1줄씩) | 수정해서 | #9715(`ef4220be66`)가 NONE_OF 안의 MESSAGE 줄을 이미 지웠다. 남은 양성 줄만 바꿨다. 1.17.0 최종형과 같다. |
| `move_effect/octolock.c` | #5 | "Octolock triggers Defiant…"의 `sharply rose` → `rose sharply` | 제외 | HnS #10042 이식(`7616748be6`)이 이 테스트를 지웠다(defiant.c·competitive.c로 옮김). 옮겨진 테스트는 MESSAGE를 검사하지 않는다. |
| `move_effect/raging_bull.c` | #1 | 방벽 ASSUME 3줄과 방벽 테스트 5개 삭제(break_screens.c로 통합) | 수정해서 | HnS는 이 5개를 이미 방벽별 문구로 고쳐 두었다(`1821fd6749`). 삭제 의미대로 같은 범위를 지웠다. 결과는 upstream 9655 파일과 바이트가 같다. |
| `move_effect_secondary/break_screens.c` | #1~#12 | 레이징불 추가, 방벽별 `wore off` 메시지, 순서 테스트 추가 | 제외 | HnS가 같은 내용을 이미 반영했다(`1821fd6749`). upstream 9655 결과와는 표기만 다르다: ASSUME 1줄, 변수명 `breakingMove`, 빈 줄. 테스트 이름은 같다. |

### 그대로 들어간 hunk(파일(개수))

ability/anger_point(3), big_pecks(2), bulletproof(1), clear_body(5), color_change(7), commander(2), competitive(10), contrary(5), costar(2), cursed_body(7), cute_charm(2), dancer(3), dauntless_shield(4), defiant(11), disguise(2), download(5), drizzle(1), dry_skin(7), earth_eater(3), effect_spore(5), embody_aspect(2), flame_body(2), galvanize(2), grim_neigh(2), gulp_missile(1), healer(2), hydration(1), hyper_cutter(5), inner_focus(2), intimidate(6), intrepid_sword(5), keen_eye(1), magic_bounce(6), mirror_armor(1), moody(1), moxie(3), oblivious(3), opportunist(3), own_tempo(5), pastel_veil(1), poison_heal(2), poison_point(3), poison_touch(#1·#3), prankster(2), purifying_salt(1), rain_dish(1), rattled(2), scrappy(2), shed_skin(1), soundproof(1), speed_boost(1), static(2), steadfast(1), steam_engine(1), sticky_hold(1), sturdy(2), symbiosis(4), tangling_hair(2), teraform_zero(1), volt_absorb(6), water_absorb(5), water_compaction(1), weak_armor(5), wind_rider(2), zero_to_hero(1); end_turn_effects(4); gimmick/dynamax(3); hold_effect/attack_up(3), berserk_gene(9), cure_status(4), defense_up(3), jaboca_berry(1), kee_berry(3), maranga_berry(3), mental_herb(1), random_stat_up(1), red_card(2), rocky_helmet(1), rowap_berry(1), safety_goggles(1), sp_attack_up(3), sp_defense_up(3), speed_up(4), terrain_seed(6), white_herb(2); item_effect/cure_status(18), heal_and_cure_status(8); move_effect/ally_switch(1), attack_up_2(1), attack_up_user_ally(1), belly_drum(2), corrosive_gas(1), defense_up_2(1), defense_up_3(1), defog(3), echoed_voice(1), evasion_up_2(1), fillet_away(2), first_turn_only(1), fling(2), grudge(1), hit_switch_target(2), knock_off(2), light_screen(1), octolock(#1~#4), parting_shot(2), rage(1), rapid_spin(6), recoil_if_miss(3), refresh(1), roar(1), roost(1), sleep_talk(2), snatch(4), special_attack_up_3(1), spicy_extract(6), stomping_tantrum(1), stuff_cheeks(2), take_heart(3), teatime(11), telekinesis(1), tidy_up(1), trick(1); move_effect_secondary/bug_bite(1), psychic_noise(2), syrup_bomb(4); move_effects_combined/axe_kick(1), hurricane(3), mind_blown(1), relic_song(1); move_flags/always_hits_in_hail_snow(1), always_hits_in_rain(1), explosion(1); sleep_clause(21); `test/text.c`(1, 주석만).
#10064: ai/check_bad_move(1), move_effect/toxic(1), status1/burn(1), status1/paralysis(3), status1/sleep(1).

- 영문 `MESSAGE` 기대값은 모두 upstream 원문 그대로 두었다(D1·D3·치유의마음처럼 HnS가 upstream과 다르게 두는 경로도 마찬가지).
- 컴파일 위험 점검(빌드는 안 함): 추가된 테스트 줄의 식별자 272개가 모두 HnS `include/src/test`에 있다. 새로 나온 것은 테스트 이름 속 단어 `Gateau`뿐이다. `GetMoveNonVolatileStatus`, `B_ANIM_SIMPLE_HEAL`, `EFFECT_NON_VOLATILE_STATUS`도 이미 있다. #9655 코드가 지우는 식별자 가운데 테스트가 쓰는 것은 `STRINGID_BUFFERENDS`·`PKMNSXCUREDITSYPROBLEM`·`SCRIPTINGABILITYSTATRAISE`(`test/text.c`)인데, upstream 헤더에도 남아 있어 문제없다. 헤더에서 실제로 지워지는 `STRINGID_ATTACKMISSED`·`STRINGID_PASTELVEILENTERS`는 테스트가 쓰지 않는다.

## 2. HnS 보존·적응

- 테스트 파일에는 `// HnS:` 주석을 달지 않았다. 적응은 위 표의 5파일뿐이다.
- 문서: 기존 3행의 변경 지점을 고쳤고 새 행 20개와 검증 상태 단락 1개를 추가했다(5절).

## 3. 다른 파트와의 계약

이 patch는 게임 심볼을 정의하지 않는다. 문서 초안은 아래 심볼과 동작을 **가정**한다. 최종 ID와 라벨은 메인이 A·B·C 결과에 맞춰 고친다.

| 가정 | 담당 | 문서 행 |
|---|---|---|
| `STRINGID_ATTACKMISSED` 삭제, `Cmd_resultmessage` 빗나감 = `STRINGID_PKMNAVOIDEDATTACK`(`// HnS:` 1줄), `gMissStringIds[B_MSG_MISSED]` = `STRINGID_PKMNAVOIDEDATTACK` | A·C | D1 |
| `BattleScript_HealBlockEndTurn` + `STRINGID_HEALBLOCKEDNOMORE`, `HandleEndTurnHealBlock` 교체(도발·전자부유는 `BUFFERENDS` 유지) | B·C | #2 |
| `gCureStatusStringIds`(이름 변경, 값 `STRINGID_SCRCURED{PARALYSIS,POISON,BURN,SLEEP,CONFUSION}`·`PKMNWASDEFROSTED`·`PKMNFROSTBITEHEALED`·`PKMNGOTOVERITSINFATUATION`·`PKMNSHOOKOFFTHETAUNT`). 본문은 `new_sentences.tsv`. `GetCuredStatusMessage()` 유지. `HEALERCURE` 유지 | A·B·C | #17, 기존 상태별 회복 행 2개 |
| `gPartyCureStatusStringIds` + `STRINGID_PARTYCURED*` 6개(`{B_BUFF1}` = 종족명, `BS_ItemCureStatus`의 `PREPARE_SPECIES_BUFFER`가 마지막에 덮어씀) | A·B·C | #15 |
| `STRINGID_PKMNWOKEUPINUPROAR` 토큰 `{B_ATK_NAME_WITH_PREFIX}` → `{B_EFF_NAME_WITH_PREFIX}` | A | D7 |
| 멘탈허브 비트마스크: `TryMentalHerb` OR 비트, `BattleScript_MentalHerbCureRet`·HnS `BattleScript_MentalHerbCureFling` 모두 `CMP_BITMASK` 순차 출력, `gMentalHerbCureStringIds[TAUNT]` = `PKMNSHOOKOFFTHETAUNT` | B·C | #3 |
| D6-b: 두 멘탈허브 스크립트의 사슬묶기·앙코르 분기에서 `saveattacker`/`copybyte gBattlerAttacker, sBATTLER`/`restoreattacker`(`// HnS:`) | B | D6-b |
| `SoundproofProtected`·`HealBellSoundproof` → `STRINGID_SCR_ITDOESNTAFFECT`(두 경로 모두 `gBattleScripting.battler` = 방음 포켓몬이어야 함) | B·C | #4, #5 |
| `SturdyPreventsOHKO` → `STRINGID_ITDOESNTAFFECT`, `BattleScript_FlinchPrevention` 삭제, `TookAttack` 팝업, `IntimidatePrevented` → 새 `STRINGID_PKMNATKNOTLOWERED`, `BattlerAbilityStatRaiseOnSwitchIn` → `SCRIPTINGSTATROSE`, `WeakArmorDefPrintString` → `DEFENDERSSTATFELL` | A·B·C | #6~#11 |
| `AbilityHpHeal`·`PoisonHealActivates`에 `B_ANIM_SIMPLE_HEAL` | B | #12, #13 |
| `RockyHelmetActivates`의 `playanimation` 삭제, HnS `ItemPopUp_ScriptingNoFlush` 유지, `HurtAttacker` = `printfromtable gHurtByStringIds`(#9856 중복 줄 없음) | B·C | #14 |
| `RemoveAllWeather` 원시 날씨 분기 + `B_MSG_WEATHER_END_EXTREMELY_HARSH_SUNLIGHT`·`HEAVY_RAIN` 표 값 | A·C | #16 |
| `BattleScript_RemoveHazards`·`gRemoveHazardsStringIds`, **`STRINGID_STICKYWEBDISAPPEAREDFROMYOU` 분기 미이식** | A·B | 기존 장판 행 |

### 다른 파트에 필요한 것(메모)

- B: upstream `BattleScript_AbilityHpHeal` hunk의 `playanimation` 줄 앞에 공백 1개+탭이 섞여 있다. 탭으로 맞추면 된다. `dry_skin`/`rain_dish` 테스트 PASS 전환이 이 hunk에 달려 있다.
- B: 끈적끈적네트 YOU 분기를 넣으면 문서의 장판 행 문구를 바꿔야 한다. 넣지 않으면 `CMP_NOT_EQUAL … HAZARDS_STICKY_WEB` 분기 없이 `printfromtable gRemoveHazardsStringIds`만 남기면 된다.
- C: upstream `HealStatusConditions()`의 `PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, battler, gBattlerPartyIndexes[battler])`는 대기 포켓몬(`battler = MAX_BATTLERS_COUNT`)일 때 배열 밖을 1회 읽는다. 1.17.0에도 같고, 곧 종족명 버퍼로 덮여서 출력 영향은 없다(기록만).
- C: upstream `AccuracyCheck`의 `MISS_TYPE = STRINGID_PKMNEVADEDATTACK`은 B_MSG 값이 아니라 문자열 ID다. HnS에서 `MISS_TYPE`는 `B_MSG_PROTECTED`와 비교할 때만 쓰여(`battle_arena.c:392`, `battle_script_commands.c:9797`) 해는 없다. 그대로 넣을지는 C가 판단한다.
- A: `test/text.c` "Battle strings fit…"는 새 `{B_BUFF1}` ID(PARTYCURED*)의 버퍼를 준비하지 않는다(upstream도 같음). 이미 FAIL이므로 실패 목록 내용만 늘 수 있다.

## 4. 한글이 든 줄 변경

- 게임 문자열(`src/`·`data/`)은 이 patch에서 바뀌지 않는다. 테스트 추가·변경 줄에 한글은 없다(영문 MESSAGE만).
- 문서(`BATTLE_MESSAGE_OUTPUT_CHANGES.md`)의 기존 행 3개를 고쳤다.
  - 장판별 `DISAPPEAREDFROMTEAM` 행: 변경 지점 끝에 "#9655 뒤 `gRemoveHazardsStringIds`·`BattleScript_RemoveHazards`로 통합, YOU 분기 미이식, 출력 같음"을 덧붙였다.
  - 상태별 회복 행(브레이브차지…): 현재 출력의 ID를 `STRINGID_SCRCURED*`로 바꾸고 이전 ID를 괄호에 남겼다. 변경 지점의 표 이름을 `gCureStatusStringIds`(이전 이름 `gStatusCureStringIds`)로 바꿨다.
  - 특성에 의한 상태 회복 행: 표 이름만 같은 방식으로 바꿨다.
- 문서 인용 한글은 모두 HEAD `src/battle_message.c`의 원문(토큰 포함)이거나 `new_sentences.tsv`의 재사용 본문이다. 새 번역은 없다. 이름은 HnS 공식명을 썼다: 불요의검, 초상투영, 제로포밍, 치료방울, 내던지기, 소란피기, 불굴의마음. REPORT의 `불굴의검`·`면영`·`테라폼제로`는 HnS 이름이 아니다.

## 5. 출력 변화 → 문서 행 대응

| REPORT # / 결정 | 문서 위치 | 처리 |
|---|---|---|
| 1 일반 빗나감(D1 C안) | 기술·필드 새 행 | 새 행. #9929 행(변화기 빗나감)은 상황이 달라 고치지 않고 "같은 문장으로 통일"이라고만 적었다. |
| 2 회복봉인 턴 종료 | 기술·필드 새 행 | 새 행 |
| 3 멘탈허브 순차 출력 | 특성·도구 새 행 | 새 행 |
| D6-b 멘탈허브 사슬묶기·앙코르 이름 | 특성·도구 새 행 | 새 행 |
| 4 방음·방탄 / 5 치료방울 / 6 옹골참 / 7 정신력 / 8 피뢰침·마중물 / 9 위협 방지 / 10 불요의검 등 / 11 깨어진갑옷 / 12 젖은접시 등 / 14 울퉁불퉁멧 / 16 제로포밍 | 특성·도구 새 행 11개 | 새 행 |
| 13 포이즌힐 애니메이션 | 특성·도구 새 행 | 기존 포이즌힐 행(HnS 무문구)은 "이전 → 현재"가 그대로 맞아 두고, #9655 차이만 새 행으로 적었다. |
| 15 가방 상태회복 도구 | 상태이상 새 행 | 새 행 |
| 17 상태 회복 이름·동상 | 상태이상 새 행 + 기존 2행 ID·표 이름 갱신 | 기존 행은 "현재 출력"이 소스와 맞아야 해서 ID만 고쳤다. 관찰되는 변화(이름 대상, 동상 문장)는 새 행에 적었다. |
| D7 소란 기상 이름 | 상태이상 새 행 | 새 행. "제외한 변경"의 토큰 교체 규칙과 달리 표시되는 포켓몬이 바뀌므로 기록한다고 행 안에 밝혔다. |
| 18 도주 특성(D3), D2 점착·타오르는불꽃, D6-a·c | 검증 상태 단락 | 변화 없음을 단락으로만 적었다. 기존 도주·스위트베일·치유의마음·습기·리샘열매·방벽 순차 해제 행은 HnS 동작 유지라 고치지 않았다. |
| (변화 없음 확인) 잠자기 `printstring`, 특성 상태이상 `B_MSG_STATUSED`, 오로라베일 `PKMNAURORAVEIL`, `PKMNHEALEDPOISON` 이름 변경, 날씨 특성 시작, 불굴의마음·`ATTACKERABILITYSTATRAISE`→`ATTACKERSSTATROSE`, `ENDUREDSTURDY`→`PKMNENDUREDHIT`, `PKMNPREVENTSUSAGE`→`POKEMONCANNOTUSEMOVE`, `PKMNSXMADEYUSELESS`→`SCR_ITDOESNTAFFECT`, 가시방패 `PKMNHURTSWITH`→`AFTERMATHDMG` | 행 없음 | HEAD 한글 바이트가 같아 행을 추가하지 않았다(직접 대조함). |

## 6. 테스트 상태 예측 요약(`part-D-tests.tsv`)

- **(a) PASS → FAIL: 1건.** `steadfast.c` "Steadfast boosts Speed when the user attempts to move but is flinched"에 양성 `MESSAGE("Lucario's Speed rose!")`이 추가된다. REPORT 예상과 같다.
  - 그 밖에 PASS 21건은 NONE_OF/NOT 안의 영문만 바뀌어 PASS를 유지한다.
  - 패치 밖 테스트의 엔진 영향도 찾아봤다(울퉁불퉁멧 `HELD_ITEM_EFFECT`, 정신력·피뢰침 팝업, `B_ANIM_SIMPLE_HEAL`). 해당 PASS 테스트는 없었다.
- **(b) 이름 변경·신규·삭제**
  - 이름 변경·분할(모두 FAIL→FAIL):
    - Shed Skin 1개 → 3개(Gen 3/4/5)
    - Jaboca·Rowap "dies" → "faints"
    - Ice Heal 1개 → 2개
    - "Old Gateu" → "Old Gateau"
  - 삭제(레이징불 5개 → `break_screens.c` 기존 테스트와 짝): 이 가운데 **PASS 3개(immune/Protected/misses)가 사라진 것처럼 보이지만** 짝 테스트가 PASS를 유지한다.
  - 이동: #10064의 AI 테스트 4개가 같은 이름으로 `check_bad_move.c`로 옮겨지며 PASS를 유지한다.
  - TO_DO 2개 삭제: mental_herb, light_screen.
  - 새 테스트 24개: FAIL 22(영문 MESSAGE, 멘탈허브 7·Shed Skin 3·#10064 2 포함), PASS 2("Light Screen reduces special damage", "Light Screen applies for 5 turns").
- **(c) FAIL → PASS 후보: 5건**
  - 엔진 무관 3건: "Dry Skin causes 1/8th Max HP damage in Sun", "Poison Heal heals from (Toxic) Poison damage", "Poison Heal heals from Toxic Poison damage are constant". HnS가 이미 무문구라 양성 MESSAGE가 빠지면 통과한다.
  - 조건부 2건: "Dry Skin heals 1/8th Max HP in Rain", "Rain Dish recovers 1/16th of Max HP in Rain". 파트 B가 `AbilityHpHeal` 회복 애니메이션 hunk를 넣어야 한다.
  - `rocky_helmet.c` Disguise 테스트는 기준 FAIL 원인을 알 수 없어 FAIL 유지로 적었다.
- 기준 목록에 없는 테스트 7개(가정 실패·조건부 컴파일로 보임)는 `-`로 두었다. 나머지 FAIL 336(KNOWN_FAILING 1 별도)은 영문 MESSAGE 때문에 FAIL을 유지한다.

## 7. 위험·질문

1. **iron_barbs TO_DO:** upstream 병합·1.17.0을 따라 남겼다. 9655 hunk처럼 지울지 메인이 정한다(지우면 TO_DO 1건 감소).
2. **raging_bull 중복 삭제:** upstream 의도대로 지웠다. 결과 비교에서 PASS 3건이 사라진 것처럼 보이므로 결과 문서에 짝을 적어야 한다. HnS 테스트를 남기려면 이 hunk만 빼면 된다.
3. **break_screens:** HnS 선반영이라 제외했다. upstream과 남은 차이는 표기 3곳뿐이다. 맞출지는 선택(이름·결과 영향 없음).
4. 문서 "범위"는 바꾸지 않았다. 기존 upstream 이식 행과 같은 방식으로 "(upstream #9655 이식)"을 붙였다.
5. `rocky_helmet` Disguise 테스트와 Mimikyu Rough Skin 테스트의 기준 FAIL 원인은 로그에 상세가 없어 확인하지 못했다.

## 8. 실기 확인 후보(mGBA, 초안)

공통 준비: 필드 디버그 메뉴(R+START)로 포켓몬·도구를 주고, 전투 중 디버그 메뉴(SELECT)로 특성·상태·도구를 바꿀 수 있다. 각 항목에서 "문장·이름·조사·줄바꿈·창 넘침"을 함께 본다.

| # | 장면 | 재현법 | 기대 |
|---|---|---|---|
| 1 | 일반 빗나감(D1) | 모래뿌리기 등으로 명중을 낮추거나 디버그로 명중 랭크 -6, 공격 기술 사용 | `{상대}에게는\n맞지 않았다!` (변화기 빗나감과 같은 문장) |
| 2 | 멘탈허브 여러 효과 동시 해제 + D6-b(내던지기 경로) | 더블배틀에서 상대가 아군 A에게 도발·트집·앙코르(가능하면 사슬묶기·회복봉인)를 걸게 한 뒤, 아군 B가 멘탈허브를 지닌 채 A에게 내던지기 | 트집 → 사슬묶기 → 회복봉인 → 앙코르 → 도발 순으로 한 문장씩. 사슬묶기·앙코르 문장 이름이 **A**(B나 상대가 아님) |
| 3 | 멘탈허브 지닌 쪽 발동 + D6-b | 멘탈허브를 지닌 포켓몬에게 상대가 앙코르 또는 사슬묶기 | `{지닌 포켓몬}의\n앙코르 상태가 풀렸다!` / `…사슬묶기가 풀렸다!`. 상대 이름이 아님 |
| 4 | 방음·방탄, 치료방울 | 방음 포켓몬에게 소리 기술, 방탄 포켓몬에게 탄 기술. 더블에서 동료가 방음일 때 치료방울 | 팝업 + `…에게는\n효과가 없는 것 같다...`. 치료방울은 팝업 없이 같은 문장 |
| 5 | 위협 방지 | 상대 위협 등장 때 정신력·둔감·마이페이스·배짱 포켓몬 | 팝업 + `…의\n공격은 떨어지지 않는다!` |
| 6 | 불요의검·불굴의방패·초상투영(바람타기는 순풍 중 교체 등장) | 디버그로 해당 포켓몬을 주고 선두로 등장 | 팝업 + `…의\n공격이 올라갔다!` 류(특성명 없음) |
| 7 | 깨어진갑옷 | 깨어진갑옷 포켓몬이 물리 공격을 맞음 | `…의\n방어가 떨어졌다!` → 스피드 상승 문장 |
| 8 | 젖은접시·건조피부(비)·볼주머니 | 비바라기 뒤 턴 종료, 볼주머니는 열매를 먹을 때 | 팝업 → 회복 애니메이션 → HP 증가, 문장 없음 |
| 9 | 가방 상태회복 도구 | 전투 중 싸우는 포켓몬과 대기 포켓몬에게 각각 해독제·마비치료제·잠깨는약 등 사용 | 싸우는 포켓몬: `{이름}의\n몸저림이 풀렸다!` 등. 대기 포켓몬: `{종족명}의 …` 상태별 문장 |
| 10 | 정글힐 아군 | 더블에서 동료를 독·마비 상태로 둔 뒤 정글힐 | 회복 문장 이름이 **동료** |
| 11 | 소란 턴 끝 기상(D7) | 상대가 더 빠르고 잠든 상태(수면가루 등)일 때 이쪽이 소란피기 첫 턴 | 턴 끝 `{깬 포켓몬}은(는)\n소란스러워서 눈을 떴다!`. 소란 사용자 이름이 아님 |
| 12 | 회복봉인 턴 종료 | 회복봉인을 받고 5턴 경과 | `…의\n회복봉인 효과가 사라졌다!` |
| 13 | 깨뜨리다·배리어프리 진영(D6-a, 이번엔 안 고침) | 상대 리플렉터를 깨뜨리다로 깸, 배리어프리 등장 | `{B_ATK_PREFIX1}` 진영 표기가 실제 방벽 편과 맞는지. 틀리면 D6-a 별도 과제 |
| 14 | 제로포밍 | 디버그로 끝의대지·시작의바다 날씨를 만든 뒤 테라파고스 테라스탈 | `햇살이 원래대로 되돌아왔다!` / `강한 비가 그쳤다!` |
| 15 | 회귀 확인(변화 없어야 함) | 옹골참 일격기, 정신력 속이기(팝업·문구 없음), 피뢰침 끌어들이기(팝업 추가), 울퉁불퉁멧(도구 애니 없음), 포이즌힐(회복 애니 추가), 치유의마음·도주 특성·리샘열매·습기-유폭 | 문서의 각 행대로 |
