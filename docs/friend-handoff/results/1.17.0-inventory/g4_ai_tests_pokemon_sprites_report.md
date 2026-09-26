# g4_ai_tests_pokemon_sprites 인벤토리 보고서 (Battle AI · Test Runner · Pokémon/Sprites, 101 PR)

- 대상: HnS 스냅샷 `<HnS 작업트리>/`(`0d89762071`, 한글화 저장소 HEAD와 동일). upstream은 `expansion/1.17.0`이다. 읽기 전용으로 조사했고, 스냅샷에서는 `git apply --check`/`-R --check`만 실행했다.
- **판정 기준(코디네이터 정정 반영)**
  - HnS는 upstream **master(1.15.2 개발) 계열**이며 upcoming 전용 리팩터가 없다(#9730 Stat Change, #9655, `DamageContext` 개편, INCGFX, #9006, #10170 등).
  - 예외로 SESSION_LOG 2026-09-15~16에 기록된 1.17.0 수동 이식분이 들어가 있다.
  - 1.16.x/1.17.0 변경은 다음처럼 나눴다.
    - upcoming 리팩터가 만든 회귀를 고친 것 → `HNS와 무관`
    - 표기·문맥만 다른 것 → `미적용·수동 적응 가능`
    - 리팩터가 먼저 있어야 하는 것 → `미적용·선행 필요`
  - `미적용·안전 이식 가능`은 소스 hunk가 그대로(또는 오프셋만 달리) 적용되고 의존 심볼이 모두 있는 경우다.
- **방법**
  - AI: 커밋마다 소스/테스트 hunk를 나눠 `git apply --check`를 돌리고, +/− 줄이 HnS에 있는지 정규화해 대조했다.
  - 이식 기록(SESSION_LOG 1923~1957행, STATUS 1493·1499행)과 대조하고, 실패 hunk와 의존 심볼(시그니처 포함)을 직접 확인했다.
  - Pokémon/Sprites: 그래픽 blob(부모·커밋·스냅샷)과 PNG PLTE·인덱스를 비교하고, compresSmol로 용량을 쟀다.
  - Test Runner: 게임 코드 hunk만 따로 코드 대조했다.
- **HnS의 `make check` 사용 여부**
  - HnS는 `make check`를 실질적으로 쓰지 않는다. `test/test_runner.c`의 `fake_rtc.h` 포함 순서 오류(`struct SaveBlock3` 미정의)로 실행 전에 중단되고, 원작업자도 개별 테스트 오브젝트 컴파일만 해 보았다(STATUS 279행, SESSION_LOG 254행).
  - CI(`build.yml`)는 upstream 상속본으로, `master/upcoming`만 트리거하고 에메랄드 기본 빌드를 쓴다.
  - 그래서 테스트 전용 PR은 `HNS와 무관`으로 판정하고, 러너 복구 때 참고할 것은 비고에 적었다.

## 요약

### 판정별 개수 (총 101)

| 판정 | 계 | Battle AI (54) | Test Runner (18) | Pokémon/Sprites (29) |
|---|---|---|---|---|
| 이미 적용 | 25 | 20 — #9359, #9462, #10139, #9448, #10464(해당분), #10626, #10669, #10700, #10124, #10243, #10258, #10236, #8647, #10046, #10427, #10277, #10453, #10461, #10610, #10688 | 0 | 5 — #9945, #10250, #10327, #10603, #10601 |
| 부분 적용 | 7 | 1 — #9710 | 0 | 6 — #9974, #10208, #10270, #10346, #10414, #10397 |
| 미적용·안전 이식 가능 | 16 | 9 — #10006, #9360, #9358, #9349, #9107, #9587, #9755, #10425, #10409 | 1 — #7360(게임 코드분) | 6 — #10071, #10004, #9173, #9135, #9690, #10535 |
| 미적용·수동 적응 가능 | 23 | 17 — #8664, #9451, #9460, #9124, #9548, #9551, #8472, #9116, #9709, #9757, #9857, #9985, #10302, #10342, #10399, #10412, #10411 | 0 | 6 — #9594, #9507, #9558, #10141, #10252, #10206 |
| 미적용·선행 필요 | 5 | 3 — #9568(#9460), #9630(일부, #9568), #10424(#10289) | 1 — #10282(#10151 미이식분) | 1 — #9796(#9594) |
| 충돌 | 0 | PR 단위 전면 충돌은 없다(hunk 단위 주의·사용자 결정 5건은 아래) | | |
| HNS와 무관 | 25 | 4 — #9779, #10381(테스트), #9596(#9548 회귀), #10285(#9730 회귀) | 16 — #10214(#10170 회귀) 외 테스트·툴 전용 15 | 5 — #10116·#10561(INCGFX), #10368(#9006 회귀), #10387(툴), #9987(사용자 결정으로 미사용) |

**문서 기록과 다른 점**
- #10124(Spicy Spray)는 SESSION_LOG 목록에 없지만 코드에는 반영돼 있다.
- SESSION_LOG의 "#10561 이식"은 실제로는 #10346의 아이콘 부분을 INCBIN으로 연결한 것이다.
- "애니메이션 테이블 6개 자리표시자"는 퇴행을 동반했다(아래 참조).

### 이식 추천 상위 항목(우선순위 순)

1. **#10342 비스탯 hunk.** HnS에 같은 AI 버그 5종이 그대로 있다.
   - `AI_GetDamage` 방어 문맥이 공격자 RISKY/CONSERVATIVE 플래그를 본다(`battle_ai_util.c:103-108`).
   - `...PP1 + monIndex) > 0);` 뒤의 세미콜론 때문에 Encore 검사가 무력화된다(`:5202`).
   - `DoesSideHaveDamagingHazards`가 첫 해저드만 검사한다(`battle_ai_main.c:6973~`).
   - `GetMovePower(playerMove != 0)` 괄호 오타(`:679`).
   - `EFFECT_HIT_ENEMY_HEAL_ALLY` 조기 반환 시 `aiCalcInProgress`를 해제하지 않는다(`battle_ai_util.c:925`).
2. **#10412**: `CalcBattlerAiMovesData`(`battle_ai_main.c:763-770`)가 사용 불가 기술 슬롯을 초기화하지 않는다. 그래서 교체 후보를 연속 계산할 때 이전 후보의 대미지·상성이 남는다.
3. **#10302**: `ShouldSwitchIfAllMovesBad` 더블 루프(`battle_ai_switch.c:496-528`)가 `ctx.battlerDef/abilityDef/holdEffectDef`를 복원하지 않는다.
4. **Pokémon 잔여분 A — 색 버그와 애니 퇴행 복구.** 1.17.0 수동 이식이 남긴 결함이다.
   - 새 `anim_front.png`만 들어오고 `normal/shiny.pal`·`back.png`가 옛판이다. 미니어(유성·코어 7색), 실바디 18타입, 타입:널은 색이 크게 틀어지고, 메가뮤츠Y·냐오불·시마사리·꼬마돌/페르시온 알로라는 일부 픽셀이 검정이 된다(ROM 약 +0.2KB).
   - `shared_front_pic_anims.h`를 1.17.0판으로 갱신하고, 자리표시자 10곳(형사구스·따라큐·마기아나·레트라/텅구리 알로라·라란티스, 토템 포함)을 교체해야 한다. 라란티스는 원래 2프레임이었는데 1프레임으로, 투구뿌논은 옛 1회 동작으로 퇴행했다.
   - 대상 PR은 #9974·#10270·#10414 잔여분이다.
5. **#10425**: `EFFECT_PRESENT/FIXED_HP_DAMAGE`가 `EFFECT_FOCUS_PUNCH` 검사로 fallthrough한다(`battle_ai_main.c:1839-1841`). 소스 hunk는 fwd ok다.
6. **#10409**: 아군 대상 필터에 `TARGET_OPPONENT/RANDOM`을 추가하고 디버그 점수 강조를 고친다(fwd ok).
7. **#10411**: 부유·천정부지 아군 흡수 가산을 고친다. 엔진 쪽 `AbsorbedByFlashFire`(`battle_util.c:2544`)가 AI 계산 중에도 `flashFireBoosted`를 켜는 문제에 `runScript` 가드를 추가한다(메시지 선택 불변).
8. **#10006, #9985**: 1~2줄 AI 정정. 파트너 플래그 슬롯, 아군 능력 하락 시 Defiant/Competitive 오인.
9. **#7360 게임 코드분**: 더블 범위기 효과음 판정(`battle_script_commands.c:1418`). 효과음 경로(1920행)에만 적용할 것을 권장한다.
10. **#9690**: 파운드 체중 변환. HnS가 `UNITS_IMPERIAL`이라 실제로 쓰이며, `pokedex.h`·`pokedex.c` 두 줄을 함께 바꿔야 한다. 같은 급으로 **#9710의 Friend Guard 혼란 자해 예외 1줄**도 권장한다.

그다음 후보:
- 작은 AI 개선: #9360, #9358, #9349, #9107, #9587, #9755, #9757, #8664.
- 스위칭 AI 묶음(9124→9551→8472→9451 순서): 효과는 AI_FLAG_SMART_SWITCHING 트레이너(HnS `Smart Trainer` 27명)에 한정된다.
- #9548: 1.17.0 최종형 Payback·Bolt Beak·Analytic AI 계산.
- 애니메이션 추가: #10141, #10252, #10206.

### 충돌·사용자 결정 목록(모두 hunk 단위, PR 전면 충돌 없음)

| ID | PR | 내용 | 권장 |
|---|---|---|---|
| P-A | #10270 | 대여르 메가 특성 hunk가 #10250의 Defiant×3을 Battle Armor/None/Defiant로 되돌린다(upstream 병합 실수로 추정). HnS는 Defiant×3이다. | 이 hunk만 제외 |
| P-B | #10414 | 텅구리 알로라 `normal.pal`이 HnS 커스텀 팔레트(`993b7bdf50`)이고 upstream 팔레트 수정과 겹친다. | HnS pal 유지, shiny.pal·back.png·애니 표만 이식(화면 확인) |
| A-1 | #9710 | `DamageContext.abilities[4]` 배열화는 upcoming 구조 개편이 선행돼야 한다. HnS `BattleContext`와 1.17.0 AI 이식분이 서로 맞물려 있다. | 배열화는 보류, Friend Guard 1줄만 이식 |
| A-2 | #10461(이식분) | HnS는 아군 대상 기술 기준이 `<= AI_SCORE_DEFAULT`이고(`battle_ai_main.c:1031`), upstream은 `<`다. 의도 여부가 기록에 없다. | 사용자 확인 |
| A-3 | #9568 | 이식하면 AI 공격 롤 기본값이 median에서 **max**로 바뀐다(난이도 영향). | 이식 시 `AI_ROLL_ATTACKING`을 median으로 둘지 결정 |
| (보존) | #10214 | 불바다·습지 한글 문자열(`battle_message.c:847-849`)이 이미 upstream 목표 토큰이다. | 적용하지 않음 |

### 용량 주의

- **큰 항목은 없다(여유 약 223KB 대비).**
- **#9987**: 고음질 메가 울음소리 WAV 26개. `P_MODIFIED_MEGA_CRIES FALSE`라 현재는 링크되지 않지만, 설정을 켜면 수백 KB 규모다. 사용자 결정(32MiB 유지)으로 적용하지 않는다.
- 애니메이션 추가: #10141 +약 3.96KB, #10252 +1.23KB, #10206 +0.44KB, #10346 잔여 약 1KB, 팔레트·back 잔여 약 +0.2KB.
- AI 코드: #9124는 2~3KB로 추정되고, 나머지 AI PR은 각각 1KB 미만이다.
- EWRAM/힙: #9709는 `BattlePokemon`에 1B를 추가한다(gBattleMons 등 +수~16B). #9568은 `SimulatedDamage.random`을 추가한다(AiLogicData 힙 +128B).
- #9507(Species enum)은 용량 변화가 없지만 148개 파일을 건드리므로 보류를 권장한다.

## 판정 표

각 표의 제목 앞 괄호는 upstream 릴리스와 changelog 분류다.

### Battle AI (54건)

| PR | 제목 | 판정 | 근거(파일·함수) | 고위험파일 | 용량 영향 | 의존·비고 |
|---|---|---|---|---|---|---|
| #10006 | (1.15.3 Fixed) Fix AI partner flags set to Battler1 instead of Battler2 | 미적용·안전 이식 가능 | `BattleAI_SetupFlags()`(`src/battle_ai_main.c:309-311`)가 아직 `GetAiFlags(gPartnerTrainerId, B_BATTLER_1)`이다. 1줄 수정이다. 슬롯 인자는 녹화 배틀의 `GetAiScriptsInRecordedBattle(battler)`에만 쓰여 영향 범위가 좁다. | 없음 | 없음 | fwd ok. 테스트 hunk(`ai_multi.c`)는 무관. |
| #9360 | (1.16.0 Added) AI considers Defense-boosting moves before Body Press | 미적용·안전 이식 가능 | `IncreaseStatUpScoreInternal()`(`src/battle_ai_util.c:5162~`)의 STAT_DEF/SPDEF 분기에 바디프레스 가산이 없다. `shouldSetUp`, `gFieldStatuses`, `EFFECT_BODY_PRESS`는 모두 있다. | 없음 | 작음 | fwd ok. |
| #9359 | (1.16.0 Added) AI uses Beat Up on allies for Rage Fist | 이미 적용(후속 형태) | `ShouldBeatUpForJustified/RageFist`, `GetUsableMoveIndexWithEffect`, `CanMoveIndexHitAnyOpponent`(`src/battle_ai_util.c:2631-2691`)와 호출부(`battle_ai_main.c:3903-3904`)가 있다. #10124가 다듬은 1.17.0 형태다. | 없음 | 없음 | #10124 이식분에 포함됐다. HnS는 `IsSubstituteProtected` 대신 `DoesSubstituteBlockMove`를 쓴다. |
| #9358 | (1.16.0 Added) AI avoids bad moves vs faster foe going semi-invulnerable | 미적용·안전 이식 가능 | `AI_CheckBadMove`(`battle_ai_main.c:1264`)에 "상대가 먼저 공중날기 등으로 사라질 예정" 검사가 없다. 엔진 쪽은 `BreaksThroughSemiInvulnerablity`를 `CanBreakThrough...`와 `BreaksThroughSemiInvulnerableState`로 나누는 이름 변경뿐이다(`battle_util.c:10828`, `battle_move_resolution.c:1779`, `battle_ai_util.c:58`, `include/battle_util.h:410`). 호출처 5곳이 upstream과 같다. | include/battle_util.h, src/battle_util.c(이름 변경만, 메시지 불변) | 작음 | fwd ok. #9857을 나중에 적용하면 `predictedMoveSpeedCheck` 이름이 바뀐다. |
| #9349 | (1.16.0 Added) Improve Rest sleep AI logic | 미적용·안전 이식 가능 | `HasUsableWhileAsleepMove`(`battle_ai_util.c:3120`, 호출 2곳)를 개명하고 `IsMoveUnusable` 검사를 추가한다. Rest 점수도 3단계로 나눈다. `AI_IsBattlerAsleepOrComatose`와 `IsMoveUnusable`은 이미 있다. | 없음 | 작음 | fwd ok. |
| #8664 | (1.16.0 Added) Smarter Doubles Fake Out AI | 미적용·수동 적응 가능 | `IsFlinchGuaranteed` 가산(`battle_ai_main.c:4497`)과 `EFFECT_FIRST_TURN_ONLY`(`:5132-5135`)가 구형 그대로다. `RNG_AI_FAKE_OUT_SAVE_ALLY`는 `include/random.h:256`에 이미 있다(그래서 fwd fail). | 없음 | 작음(~1KB) | random.h hunk는 빼고 `FAKE_OUT_SAVE_ALLY_CHANCE`(config/ai.h)만 추가한다. 필요한 `CanAIFaintTarget`, `AI_WhoStrikesFirst`, `chosenMoveIndex`는 있다. |
| #9451 | (1.16.0 Added) Both AI opponents in doubles can switch same turn | 미적용·수동 적응 가능 | `IsPartyMonPlannedToBeSwitchedInByPartner`가 없다. `FindMonWithMoveOfEffectiveness`(`battle_ai_switch.c:446`)와 `GetNextMonInParty`(`:2499`)도 구형이다. 실패 hunk는 `GetBestMonIntegrated`(`:2136`) 하나다. HnS의 `InitializeSwitchinCandidate(battler, monIndex, mon)` 3인자 문맥 차이일 뿐이다. | 없음 | 작음 | 수동 문맥 보정 1곳. |
| #9460 | (1.16.0 Added) Add AI internal config support | 미적용·수동 적응 가능(인프라) | `AI_CONFIG_DEFINITIONS`가 없고 구조체 이름도 아직 `struct GenChanges`다(`include/generational_changes.h:11`). HnS가 알 상속 설정 5개를 추가했기 때문에(`include/constants/generational_changes.h:235-239`) 문맥이 어긋나 fail한다. | 없음 | 없음(빈 매크로) | 단독으로는 효과가 없다. #9568·#9462의 `GetConfig(AI_...)` 선행이다. upstream은 #9529에서 `config_changes.h`로 개명했는데 HnS에는 없으므로 `generational_changes.h`에 맞춰 넣는다. |
| #9124 | (1.16.0 Added) Switch AI sees stat/volatile/status/HP changes on switchin | 미적용·수동 적응 가능(대형) | `InitializeSwitchinCandidate`가 아직 `SetBattlerFieldStatusForSwitchin`만 호출한다. `SetBattlerStatStagesForSwitchin`·`GetSwitchinSingleUseItemHealing`·`IsSwitchinTSpikesAffected`·`GetDownloadStat`이 없고, `AI_GetSwitchinWeather`에 Orichalcum Pulse도 없다. `battle_util.c:3269` 다운로드 hunk는 스탯 계산을 함수로 빼낼 뿐이며 메시지와 스크립트는 그대로다. | src/battle_util.c(리팩터, 메시지 불변) | 작음~중간(2~3KB 추정) | HnS의 `InitializeSwitchinCandidate`에는 Champions Supreme Overlord 어댑트가 있으니 보존한다. 혼란 열매는 #10163 이식으로 `HOLD_EFFECT_CONFUSE_FLAVOR`(`battle_ai_switch.c:1868`)이므로 upstream의 5개 case를 그대로 쓰지 않는다. |
| #9548 | (1.16.0 Added) AI calcs for Bolt Beak, Payback, Analytic | 미적용·수동 적응 가능(1.17.0 최종형 권장) | `CalcMoveBasePower`의 `EFFECT_PAYBACK/BOLT_BEAK`(`battle_util.c:6571-6579`)와 Analytic에 `ctx->aiCalc` 분기가 없다. AI는 실전 행동 여부(`HasBattlerActedThisTurn`)로 판단한다. `AI_SetBattlerTurnOrder`(`battle_ai_util.c:4387`)는 #8647 이식으로 이미 공개돼 있다. | src/battle_util.c(대미지 계산, 메시지 없음) | 작음 | 9548→9596→10453 원형을 차례로 옮기지 말고, 1.17.0의 `Ai_AttackerMovesAfterTarget(battlerAtk, battlerDef)`/`Ai_AttackerMovesLast` 형태(upstream `battle_util.c:1234-1250`)를 직접 넣는다. BattleContext 필드 추가도 필요 없다. |
| #9568 | (1.16.0 Added) Add AI contextual damage roll configs | 미적용·선행 필요 | `DMG_ROLL_DEFAULT`(`include/battle_ai_util.h:15`)와 `SimulatedDamage{min,median,max}`(`include/battle.h:200`)가 구형이다. `RNG_AI_DMG_ROLL_RANDOM`만 `random.h:257`에 있다. | include/battle.h | 작음(AiLogicData +128B 힙) | 선행: #9460(AI config 인프라). **동작 변화**가 있다. 기본 공격 롤이 median에서 max로 바뀌어(`AI_ROLL_ATTACKING=AI_ROLL_MAX`) AI가 더 공격적으로 판단한다. HnS 난이도 의도와 맞는지 확인이 필요하다. |
| #9551 | (1.16.0 Added) Intimidate cycling logic for AI switching | 미적용·수동 적응 가능 | `SHOULD_SWITCH_INTIMIDATE`, `ShouldSwitchIfIntimidateBenefit`이 없다. `DoesIntimidateRaiseStats`(`battle_ai_util.c:6208`)는 있다. fail 원인은 #9124 선언부 문맥이다. | 없음 | 작음 | Zero to Hero hunk는 빼야 한다. #10626 이식본(`battle_ai_switch.c:995-1005`)이 이미 대체했다. RNG 태그 `RNG_AI_SWITCH_INTIMIDATE`와 config 2개를 추가한다. 영향은 AI_FLAG_SMART_SWITCHING 트레이너에 한정된다. |
| #9462 | (1.16.0 Added) Randomize AI mon logic order in doubles | 이미 적용(기능 동등) | `HandleTurnActionSelectionState`(`battle_main.c:4374-4379`)에 `gAiLogicData->reverseBattlerLogicOrder`가 있다(#8647 후속 형태). `AI_REVERSE_BATTLER_LOGIC_ORDER_CHANCE 50`(`config/ai.h:68`)과 RNG 태그도 있다. | 없음 | 없음 | `GetConfig(...)` 래핑과 `SHOULD_SWITCH_ALL_MOVES_BAD_PERCENTAGE` config화만 빠졌다(#9460 필요, 런타임 동일). |
| #8472 | (1.16.0 Added) Wish Passing in ShouldSwitch | 미적용·수동 적응 가능 | `ShouldSwitchIfWishPassing`, `SHOULD_SWITCH_WISH_PASSING`이 없다. 필요한 `GetWishHealAmountForBattler`(`battle_ai_switch.c:97`)와 `gBattleStruct->wish[].counter`는 있다. | 없음 | 작음 | 선언부 문맥이 #9124/#9551 뒤라 fail한다. 순서는 9124→9551→8472를 권장한다. |
| #9116 | (1.16.0 Changed) Bridge between battle engine and AI calcs | 미적용·수동 적응 가능(순수 리팩터, 우선순위 낮음) | `battle_ai_record.c/.h`가 없다. Record 함수 이동과 include 교체만 한다. 게임 동작은 없다. 끼어 있는 Eject Pack `CanBattlerSwitch` 1줄(`battle_move_resolution.c:3673`)은 1.17.0에서 다시 재작성됐으므로 해당 그룹에서 판정한다. | src/battle_message.c·battle_script_commands.c·battle_util.c·battle_end_turn.c·battle_hold_effects.c(include 줄만) | 없음 | 이득이 작고 고위험 파일 include를 건드린다. 후속 PR 문맥 정합용으로만 고려한다. |
| #9107 | (1.16.0 Changed) AI scores Order Up boosts under Commander | 미적용·안전 이식 가능 | `AI_CalcAdditionalEffectScore`에 `MOVE_EFFECT_ORDER_UP` case가 없다. `commanderSpecies`와 효과 상수는 있다. | 없음 | 작음 | fwd ok. |
| #9587 | (1.16.0 Changed) Use precalculated speedStats in AI speed comparison | 미적용·안전 이식 가능 | `AI_WhoStrikesFirst`(`battle_ai_util.c:1479~`)가 매번 `GetBattlerTotalSpeedStat`을 호출한다. `aiData->speedStats`는 `SetBattlerAiData`(`battle_ai_main.c:706`)에서 이미 계산된다. | 없음 | 없음 | fwd ok. 사고 시간이 줄어든다. |
| #9630 | (1.16.0 Changed) Damage roll speed wizardry | 미적용·선행 필요(일부) | roll 함수들(`battle_ai_util.c:~720-742`)을 `noinline ARM_FUNC`로 바꾼다. 실패 hunk는 `RandomRollDmg`뿐이다(#9568이 추가하는 함수). | 없음 | 없음(ARM 코드, ROM 상주) | Lowest/Highest/DmgRoll 3개는 단독으로 적용할 수 있다. `ARM_FUNC`는 IWRAM 배치가 아니다(`include/gba/defines.h:19`). |
| #9709 | (1.16.0 Changed) Improve AI calc speed with affection hearts | 미적용·수동 적응 가능(우선순위 낮음) | `GetBattlerAffectionHearts`(`battle_util.c:1779`)가 매 호출 `GetMonData`를 한다. `CalcFinalDmg` 버티기 판정(`:8280`)에서 무조건 호출된다. `B_AFFECTION_MECHANICS FALSE`라 효과는 속도 개선뿐이다. | include/pokemon.h(BattlePokemon), src/battle_util.c | 작음(gBattleMons 등 EWRAM +수~16B) | BattlePokemon 구조체가 커진다(링크·녹화 전송 포함). upstream의 `notOnField` 조건은 다른 PR 소관이다. |
| #9755 | (1.16.0 Changed) Remove redundant weather check in IsDamageMoveUnusable | 미적용·안전 이식 가능 | `IsDamageMoveUnusable`의 `HasWeatherEffect()` 이중 검사가 남아 있다. `ctx->weather`는 이미 `AI_GetWeather()`(`battle_ai_util.c:1937`, 날씨 무효 반영)에서 온다. | 없음 | 없음 | fwd ok. |
| #9757 | (1.16.0 Changed) Remove redundant SetBattlerAiData in GetMostSuitableMonToSwitchInto | 미적용·수동 적응 가능 | `GetBestMonIntegrated`의 복원 직후 `SetBattlerAiData(battler, ...)`(`battle_ai_switch.c:2354`)가 남아 있다. `FreeRestoreAiLogicData`가 전체를 복원하므로 중복이다. | 없음 | 없음 | config/ai.h 변경은 테스트 프레임 상한이라 무관하다. |
| #9779 | (1.16.0 Changed) Isolate AI thinking time tests | HNS와 무관(테스트 전용) | `AI_FRAME_CEILING_*` 상수를 config에서 테스트로 옮긴다. 게임 코드에는 참조가 없다. HnS에는 `ai_thinking_time.c`가 없다. | test/battle/** | 없음 | 러너 복구 시 참고. |
| #9857 | (1.16.0 Changed) Incoming and predicted move function cleanup | 미적용·수동 적응 가능(대규모 치환, 동작 변화) | `GetIncomingMoveSpeedCheck`(`battle_ai_util.c:213`, 호출 24곳)가 구형이다. 예측 기술에서 위력 0·첫턴전용 기술을 거르는 필터와 `lastUsedMove` 폴백이 사라지는 동작 변화가 있다. | 없음 | 없음 | HnS는 `CanAiPredictMove(battler)` 조건을 추가해 두었으므로 보존한다. #9358 뒤에 적용한다. 이득 대비 작업량이 크다. |
| #9596 | (1.16.0 Fixed) Fix frame counter in multi battles | HNS와 무관(#9548 도입분의 성능 회귀 수정) | turnOrder를 AiLogicData에 캐시하는 구조다. 1.17.0에서는 #10453이 이를 다시 호출 시 계산으로 되돌렸다. 고유하게 남는 부분은 `holdEffectParams` 제거(`include/battle.h:213`, `battle_ai_main.c:702`; Focus Band 판정은 `GetBattlerHoldEffectParam` 직접 호출)뿐이다. | include/battle.h, src/battle_util.c | 없음(힙 −4B) | #9548을 1.17.0 최종형으로 넣으면 turnOrder hunk는 불필요하다. `holdEffectParams` 제거는 선택 사항이다(동작 동일). |
| #9710 | (1.16.0 Fixed) Use ctx in GetDefenderPartnerAbilitiesModifier | 부분 적용 | 목적(파트너 특성을 가상 컨텍스트로 조회)은 HnS `GetDamageCalcAbility(ctx, battler)`(`battle_util.c:6153`, #10145 어댑트)로 달성됐다. `DamageContext.abilities[4]/holdEffects[4]` 배열화 리팩터는 미적용이다. Friend Guard가 혼란 자해를 줄이지 않게 하는 조건(`ctx->battlerAtk != ctx->battlerDef`)도 없다(`:7745-7759`). | src/battle_util.c, battle_script_commands.c, battle_move_resolution.c(배열화 시) | 없음 | 배열화는 upcoming의 `BattleContext`→`DamageContext` 개편이 먼저 있어야 한다(선행 필요). HnS는 master 계열 `BattleContext`이며, 1.17.0 AI 이식분도 이 구조에 맞춰져 있다(주의 절 참조). Friend Guard 1줄은 수동으로 적응시켜 단독 이식할 것을 권장한다. |
| #9985 | (1.16.0 Fixed) Remove Defiant/Competitive from partner ability check | 미적용·수동 적응 가능 | `AI_DoubleBattle` 파트너 특성 분기(`battle_ai_main.c:3818-3820`)에 `ABILITY_DEFIANT/COMPETITIVE`가 남아 있다. 아군의 능력 하락으로는 발동하지 않으므로 잘못된 가산이다. | 없음 | 없음 | HnS 문맥은 `IsStatLoweringEffect(effect)`라 수동으로 두 줄 삭제한다. |
| #10139 | (1.16.1 Fixed) Prevent AI illegally targeting itself in doubles | 이미 적용(다른 형태) | `ChooseMoveOrAction_Doubles`(`battle_ai_main.c:989`)에 `gBattleMons[battlerDef].hp == 0 \|\| battlerAtk == battlerDef`가 있다. | 없음 | 없음 | #10046·#10461 이식분에 포함됐다. |
| #10302 | (1.16.2 Fixed) Fix AI partner all-moves-bad on dead adjacent foe | 미적용·수동 적응 가능 | `ShouldSwitchIfAllMovesBad`(`battle_ai_switch.c:496-528`) 더블 루프에서 `ctx.battlerDef`를 파트너로 바꾼 뒤 복원하지 않는다. 다음 기술부터 잘못된 대상으로 판정한다. | 없음 | 없음 | HnS 구조체에 맞춰 `battlerDef`·`abilityDef`·`holdEffectDef` 3개를 복원한다. **추천(버그 수정)**. |
| #10285 | (1.16.2 Fixed) Fix move effect AI stat change check | HNS와 무관(#9730 Stat Change Refactor 경로의 회귀 수정) | `MOVE_EFFECT_STAT_PLUS/MINUS`, `AI_GetAdjustedStatStage`, `GetAllyStatChangeScore`가 HnS에 없다. HnS는 구형 `MOVE_EFFECT_ATK_PLUS_1` 체계를 쓴다(`battle_ai_main.c:6052-6065`). | 없음 | 없음 | HnS 구형 경로(master 계열)에는 이 버그가 없다. #9730/#10057을 이식할 때만 함께 가져온다. |
| #10342 | (1.16.2 Fixed) Fix reversed AI battlerAtk/battlerDef usage | 미적용·수동 적응 가능(비스탯 hunk) / 스탯 hunk는 HNS와 무관 | HnS에 같은 버그들이 남아 있다. ① `AI_GetDamage` 방어 문맥이 공격자 플래그로 RISKY/CONSERVATIVE를 판정한다(`battle_ai_util.c:103-108`). ② `...MON_DATA_PP1 + monIndex) > 0);` 세미콜론 버그(`:5202`). ③ `DoesSideHaveDamagingHazards` default `return FALSE`로 첫 해저드만 본다(`battle_ai_main.c:6973~6985`). ④ `GetMovePower(playerMove != 0)` 괄호 오타(`:679`). ⑤ `EFFECT_HIT_ENEMY_HEAL_ALLY` 조기 반환 시 `aiCalcInProgress`를 해제하지 않는다(`battle_ai_util.c:925`). | 없음 | 없음 | 스탯 관련 hunk(`AI_CanStatChangeBePrevented`, `AI_GetAdjustedStatStage` 등)는 upcoming #9730/#10057 경로의 수정이라 HnS와 무관하다. 비스탯 hunk 5종만 먼저 적용한다. HnS의 SWITCHIN/SHOULD_SETUP 문맥은 #9568이 없어 해당 없다. **추천**. |
| #10381 | (1.16.3 Changed) Can Use All Moves → Values Moves Over Splash | HNS와 무관(테스트 전용) | `test/battle/ai/can_use_all_moves.c` 개명·재작성뿐이다. | test/battle/** | 없음 | 러너 복구 시 참고. |
| #10399 | (1.16.3 Fixed) Fix invalid switch-in if ace flag present | 미적용·수동 적응 가능(영향 낮음) | `AI_TrySwitchOrUseItem`(`battle_ai_main.c:505-526`)에서 Ace만 남으면 `monToSwitchId`가 `firstId-1`(음수)이 되어 잘못 교체할 수 있다. | 없음 | 없음 | HnS는 `GetAIPartyIndexes`·`IsPartyMonOnFieldOrChosenToSwitch` 3인자 구조라 수동 어댑트가 필요하다. `trainers_hns.party`에 Ace 플래그 트레이너가 없어 현재는 도달하기 어렵다. controller hunk는 HnS `gEnemyParty` 구형과 동등하다. |
| #10412 | (1.16.3 Fixed) Reset move data between switch-in calcs | 미적용·수동 적응 가능 | `CalcBattlerAiMovesData`(`battle_ai_main.c:763-770`)가 `IsMoveUnusable`로 `continue`하기 전에 슬롯을 초기화하지 않는다. 연속 교체 후보 계산 때 이전 후보의 대미지·상성이 남는다. | 없음 | 없음 | HnS `AiCalcValues`(#10453 어댑트) 형태에 맞춰 4줄을 추가한다. **추천(버그 수정)**. |
| #10411 | (1.16.3 Fixed) Avoid rewarding Levitate ally immunity | 미적용·수동 적응 가능 | `AI_DoubleBattle` 파트너 특성 분기(`battle_ai_main.c:3641-3650`, 불꽃·초식 등은 `~3740-3760`)에 `ShouldTriggerPartnerAbility`가 없다. 부유(와 Champions 이식분 `ABILITY_EELEVATE`)를 흙먹기처럼 가산한다. 엔진 쪽 `AbsorbedByFlashFire`(`battle_util.c:2544-2551`)는 `runScript`와 무관하게 `flashFireBoosted = TRUE`를 설정한다. | src/battle_util.c(상태 플래그 가드만, 메시지 선택 불변) | 작음 | HnS는 `AbsorbedByFlashFire(battlerDef)` 시그니처이므로 ctx 전달형으로 바꾸거나 호출부에서 가드한다. Champions 특성 `ABILITY_EELEVATE`(천정부지)도 1.17.0처럼 부유와 같은 새 분기로 옮긴다(upstream `battle_ai_main.c:3504-3505`). |
| #10424 | (1.16.3 Fixed) Fix AI Binding Band check for Wrap damage | 미적용·선행 필요 | `GetTrapDamage`(`battle_ai_util.c:3509-3518`)가 `holdEffects[wrappedBy]`를 본다. 대체 필드 `volatiles.wrappedBindingBand`가 HnS에 없다. | 없음 | 없음 | 선행: #10289(Binding Band 무효화 수정). #10427 이식으로 `ShouldTrap`이 `wrapped`를 임시로 TRUE로 두므로, 현재 HnS는 오래된 `wrappedBy`를 읽을 수 있다. |
| #10425 | (1.16.3 Fixed) Fix AI Focus Punch checks on Present/Fixed HP moves | 미적용·안전 이식 가능 | `AI_CheckBadMove`의 `case EFFECT_PRESENT/FIXED_HP_DAMAGE`(`battle_ai_main.c:1839~`)가 `EFFECT_FOCUS_PUNCH` 분기로 떨어진다(fallthrough). | 없음 | 없음 | 소스 hunk는 fwd ok다(전체 fail은 테스트 문맥 때문). 3줄. |
| #10409 | (1.16.3 Fixed) Fix AI target filtering and debug score highlighting | 미적용·안전 이식 가능 | `ShouldConsiderMoveForBattler`(`battle_ai_main.c:1072-1083`)가 아군에게 `TARGET_OPPONENT/RANDOM` 기술 점수를 매긴다. 디버그 화면 `PutMovesPointsText`(`battle_debug.c:731`)는 선택 대상 강조 인덱스가 틀리다. | 없음 | 없음 | fwd ok. battle_debug.c 영문 UI 문자열은 그대로다. |
| #9448 | (1.16.4 Fixed) Prevent Protect AI from blocking ally beneficial-hit setups | 이미 적용 | `battlerMovesScored`(`include/battle.h:241`, `battle_ai_main.c:422`, `battle_ai_util.c:2311`)와 util 추가 30줄이 모두 있다. | 없음 | 없음 | 문서 기록과 일치한다. 패딩 비트 수만 다르다. |
| #10464 | (1.16.4 Fixed) Fix AI scoring for stat-changing move effects | 이미 적용(해당분) / 나머지 HNS와 무관 | `GetConfig(B_STURDY)`(`battle_ai_util.c:3690`)만 반영됐다. 스탯 점수 hunk는 구형 경로라 해당 없다. | 없음 | 없음 | SESSION_LOG 1952행 기록과 일치한다. 누락된 `break;`·스탯 루프 수정은 upcoming #9730 경로 전용이라 HnS에는 해당 없다. |
| #10626 | (1.16.4 Fixed) Fix Palafin-Hero repeatedly switching | 이미 적용 | `CanPalafinZeroSafelyUseHitEscape`(`battle_ai_switch.c:883`)와 Zero to Hero 분기(`:995-1005`, `:1438-1440`)가 있다. | 없음 | 없음 | `switchContext` 대신 battler 인자 구조로 어댑트됐다. |
| #10669 | (1.16.4 Fixed) Fix Mind Reader/Lock-On spam in doubles | 이미 적용 | `EFFECT_LOCK_ON` 검사가 `battlerWithSureHit != 0`이다(`battle_ai_main.c:2408`). 확정 명중 대상 가산도 있다. | 없음 | 없음 | — |
| #10700 | (1.16.4 Fixed) Fix AI not calcing crit at high crit stage | 이미 적용 | 추가 3줄이 모두 있다(battle_util.h/battle_ai_util.c/battle_util.c). | — | 없음 | — |
| #10124 | (1.17.0 Added) AI triggers ally's Spicy Spray when burn beneficial | 이미 적용 | 38줄 중 35줄이 있다. 없는 3줄은 `IsSubstituteProtected`를 HnS의 동등 함수 `DoesSubstituteBlockMove`로 바꾼 것이다. | 없음 | 없음 | 문서 목록에는 없지만 코드상 반영됐다(Champions 특성 이식과 함께 들어온 것으로 보임). |
| #10243 | (1.17.0 Added) Modular AI Omniscience Flags | 이미 적용 | 플래그 3개(`include/constants/battle_ai.h`)와 분기가 있다. 파티 능력 기록은 HnS 구조(`gAiPartyData->mons[B_SIDE_PLAYER]`, `battle_ai_main.c:620`)로 어댑트됐다. | 없음 | 없음 | `docs/tutorials/ai_flags.md`만 미반영(무관). |
| #10258 | (1.17.0 Added) AI avoids self-KOs with recoil | 이미 적용 | 반동 판정(`battle_ai_main.c:4169-4187`)이 있다. `BattleSideHasTwoTrainers`는 HnS 배틀 타입 플래그로 대체했다. | 없음 | 없음 | SESSION_LOG 1953행의 호환 결정과 일치한다. |
| #10236 | (1.17.0 Added) Dragon Darts AI | 이미 적용 | `dragonDartsHitsBothTarget`(`include/battle.h:242`, `battle_ai_util.c:790`, `:1541`)이 있다. | 없음 | 없음 | 헬퍼 함수를 인라인한 형태다. |
| #8647 | (1.17.0 Added) Round / Pledge combo attack AI | 이미 적용 | `AI_SetBattlerTurnOrder`, `WillPartnerActBeforeOrAfter`(`battle_ai_util.c:4387-4670`), `reverseBattlerLogicOrder`가 있다. `battle_util.c`의 미세 최적화(allMovesMask, attackerWeather 캐시, targetCount 캐시)는 동작이 같아서 넣지 않았다. | 없음 | 없음 | `Ai_AttackerMovesAfterTarget` 교체 hunk는 #9548이 없어 해당 없다. |
| #10046 | (1.17.0 Changed) Doubles AI skip scoring on itself | 이미 적용(다른 형태) | `battle_ai_main.c:989`의 `battlerAtk == battlerDef` 스킵. | 없음 | 없음 | #10139와 같은 줄이다. |
| #10427 | (1.17.0 Changed) AI considers Wrap residual damage | 이미 적용 | `ShouldTrap`의 `shouldTrap`/wrap 시뮬레이션(`battle_ai_util.c:3980-3997`)과 `BattlerWillFaintFromSecondaryDamage` 변경이 있다. | 없음 | 없음 | #10424 비고 참조(오래된 `wrappedBy`). |
| #10277 | (1.17.0 Changed) AI thinking time and accuracy improvements | 이미 적용 | `SetTypeBeforeUsingMove(move, battler, ability, holdEffect)`와 `GetDynamicMoveType(..., ability, holdEffect, ...)`의 새 시그니처(`include/battle_main.h:111-112`)와 AI 호출부 7곳이 있다. | src/battle_util.c·battle_script_commands.c·battle_end_turn.c(호출 인자만) | 없음 | 휴리스틱상 "미존재" 줄은 공백·줄바꿈 차이다. |
| #10453 | (1.17.0 Changed) Tidy up AI_CalcDamage signature with a struct | 이미 적용(HnS 구조) | `struct AiCalcValues`(`battle_ai_main.c:756-761`)를 쓴다. HnS는 `terrain` 대신 `fieldStatuses`를 유지한다. | 없음 | 없음 | battle_util.c Payback 턴순서 hunk는 #9548이 없어 해당 없다. |
| #10461 | (1.17.0 Changed) Minor AI score calculation clean up | 이미 적용 | 61줄 중 58줄이 있다. `ChooseMoveOrAction`이 `struct ChosenAction`을 반환하는 형태다. | 없음 | 없음 | **HnS 차이**: 아군 대상 기준이 `<= AI_SCORE_DEFAULT`다(`battle_ai_main.c:1031`, upstream은 `<`). 의도한 변경인지 확인이 필요하다(불확실 항목). |
| #10610 | (1.17.0 Changed) Minor battle_util.c fixes | 이미 적용(HnS 대응) | Stomping Tantrum `!gAiLogicData->switchInCalc`(`battle_util.c:6699`)와 지진 Grassy 판정 `ctx->fieldStatuses`(`:6704`)가 있다. | src/battle_util.c | 없음 | upstream `ctx->terrain` 대신 HnS 필드 표현을 쓴다. |
| #10688 | (1.17.0 Fixed) Fix Fusion Move AI turn-order checks | 이미 적용 | 추가 9줄이 모두 있다(`battle_ai_util.c` WillPartnerActBeforeOrAfter 계열). | 없음 | 없음 | — |

### Test Runner (18건)

| PR | 제목 | 판정 | 근거(파일·함수) | 고위험파일 | 용량 영향 | 의존·비고 |
|---|---|---|---|---|---|---|
| #7360 | (1.15.3 Changed) Add EFFECTIVENESS_SE for tests | 미적용·안전 이식 가능(게임 코드 부분만) | **게임 코드**: `UpdateEffectivenessResultFlagsForDoubleSpreadMoves()`(`src/battle_script_commands.c:1418`)가 여전히 구 알고리즘이다. 더블 범위기가 한 대상에 보통, 다른 대상에 "효과가 별로"로 들어가면 NVE 효과음이 난다. upstream은 "1배 이상 대상이 하나라도 있으면 NVE 제거, SE가 하나라도 있으면 SE"로 고쳤다(효과음만 바뀌고 메시지와는 무관하다). **테스트 부분**: `QueueEffectivenessSound`·`EFFECTIVENESS_SE` 러너 인프라와 spread_moves.c 테스트로, HnS와 무관하다. | src/battle_script_commands.c, include/test/battle.h, test/battle/spread_moves.c | 작음(수십 바이트) | 파일 fwd fail. HnS는 #9777 이식으로 `MOVE_RESULT_HIGH/LOW_EFFECTIVENESS` 매크로를 쓰므로 수동 어댑트해야 한다. 1.17.0 최종형은 #10362(`battle_move_resolution.c` `CancelerEffectivenessSound`, 초기값 0에서 OR)다. 같은 함수를 `Cmd_attackanimation`(1553행)도 호출하므로 새 로직은 효과음 경로(1920행)에만 적용할 것을 권장한다(불확실 항목 1). `#if TESTING` 훅을 넣으려면 `include/test_runner.h` 스텁(fwd ok)도 필요하다. 러너 복구 시 참고. |
| #10000 | (1.15.3 Changed) Add test for Curse + Baton Pass interaction | HNS와 무관(테스트 전용) | `test/battle/move_effect/curse.c`만 변경(TO_DO를 실제 테스트로 전환). 게임 코드 없음. | test/battle/** | 없음 | fwd ok. `SEND_IN_MESSAGE("Wynaut")` 등 영문 기대값이 있다. 러너 복구 시 참고. |
| #10017 | (1.15.3 Changed) Set higher HP in Gem Damage calculation test | HNS와 무관(테스트 전용) | `test/battle/damage_formula.c`의 KNOWN_FAILING 제거와 상대 HP 999 설정. | test/battle/** | 없음 | fwd ok. |
| #9977 | (1.15.3 Fixed) Fix failing Dream Eater test behind Substitute (Gen 5+) | HNS와 무관(게임 동작 불변, 테스트용 런타임 config화) | **게임 코드**: `BattleScript_EffectDreamEater`(`data/battle_scripts_1.s:2396-2403`)의 컴파일 타임 `.if B_DREAM_EATER_SUBSTITUTE < GEN_5`를 `jumpifgenconfiglowerthan CONFIG_B_DREAM_EATER_SUBSTITUTE`로 바꾸고, `generational_changes.h`에 항목 1개를 추가한다. HnS는 `B_DREAM_EATER_SUBSTITUTE = GEN_LATEST`(`include/config/battle.h:157`)라서 두 형태의 실행 결과가 같다(대타출동 검사 생략). **테스트 부분**: `WITH_CONFIG`로 Gen4/Gen5+를 모두 검사하고, 기대값을 `SUB_HIT`로 정정한다. | data/battle_scripts_1.s, test/battle/** | 작음(스크립트 수 바이트와 ROM 상수 구조체 비트필드 1개) | 스크립트 hunk는 fwd ok다. generational_changes.h는 HnS의 `B_RAGE_FIST` 행 때문에 문맥이 달라 한 줄을 수동으로 넣어야 한다. 한글 문자열·메시지 경로는 건드리지 않는다. 러너 복구 시 dream_eater 테스트와 함께 선택 이식해도 안전하다. |
| #10021 | (1.15.3 Fixed) Fix Electrify status move test expectations | HNS와 무관(테스트 전용) | `test/battle/move_effect/electrify.c` 기대값만 수정. | test/battle/** | 없음 | fwd ok. 영문 Volt Absorb 메시지를 기대한다. |
| #9805 | (1.16.0 Added) Adds Ghost battle tests | HNS와 무관(FRLG 전용 + 테스트 전용) | **게임 코드**: `STRINGID_GHOSTWASMAROWAK`의 `"\p\n"`을 `"\p"`로 바꾼다(`src/battle_message.c:898`). HnS 쪽은 미번역 영문에 `//frlg` 주석이 붙어 있어 fwd fail이다. SESSION_LOG "HNS 유령 배틀 도달 가능성 재확인"(438-442행)대로 포켓몬타워 유령 배틀은 HnS에서 도달할 수 없다. **테스트 부분**: `GHOST_BATTLE_TEST`/`BATTLE_TEST_GHOST` 러너 지원과 `test/battle/ghost.c`(HnS에 없음). | src/battle_message.c, include/test/battle.h, test/battle/** | 없음(1바이트) | 이식 불필요. 나중에 FRLG 콘텐츠를 연결한다면 `//frlg` 주석은 유지하고 `\n` 한 글자만 제거하면 된다. #10537의 선행. |
| #9642 | (1.16.2 Added) Add test support for inventory management in battle | HNS와 무관(테스트 전용) | **게임 코드**: `RecordedPlayerHandleChooseItem()`(`src/battle_controller_recorded_player.c:376`)에 `if (TESTING)` 블록을 추가한다. 비테스트 빌드에서는 상수 FALSE라 사라지고, 게임 동작은 바뀌지 않는다. **테스트 부분**: `GIVE_PLAYER_ITEM`, 테스트 인벤토리. | include/test/battle.h | 없음 | recorded_player hunk는 fwd ok, test_test_runner.c는 fail. 러너 복구 시 참고(#10537, #10321의 선행). |
| #10155 | (1.16.2 Changed) Test Runner: Replace :L hack | HNS와 무관(테스트·툴 전용) | `test/test_runner*.c`, `include/test/test.h`, `tools/mgba-rom-test-hydra/main.c`를 개편한다(테스트 이름·줄 번호 출력 방식). 게임 코드 없음. | include/test/battle.h | 없음 | test_runner.c와 test_runner_battle.c는 fwd fail이다. 선행 러너 커밋(#9892 등)이 없기 때문이다. 러너 복구 시 참고(hydra 재빌드 필요). |
| #9807 | (1.16.2 Changed) Battle Test: Failure Suggestions | HNS와 무관(테스트 전용) | 실패 시 "Did you mean" 제안 출력(`test/test_runner_battle.c`, `include/test_result.h`). | include/test/battle.h | 없음 | fwd fail(선행 러너 커밋 누락). 러너 복구 시 참고(개발 편의). |
| #10358 | (1.16.2 Fixed) Fix incorrect Mummy and Doodle ability tests | HNS와 무관(테스트 전용) | `test/battle/ability/mummy.c`, `test/battle/move_effect/doodle.c` 기대값 수정. | test/battle/** | 없음 | fwd ok. |
| #10379 | (1.16.3 Fixed) Fix inverse battle matchup parametrization | HNS와 무관(테스트 전용) | `test/battle/inverse_battle.c` 매개변수화만 수정. | test/battle/** | 없음 | fwd ok. |
| #10390 | (1.16.3 Fixed) Fix Known Fails Passing `and 0 more` + additional print modes | HNS와 무관(러너 출력·개발 도구) | **게임 코드(assert 인프라)**: `src/assertf.c`에 `%c`/`%C` 서식을 추가하고 `Puts`/`PutS` 조건 순서를 바꾼다. HnS의 `Puts`/`PutS`(148·158행)에는 #9892(`17d3887310` `%.*s` 지원)의 `n` 인자가 없어 hunk가 적용되지 않는다. HnS 코드에는 `%c`/`%C`를 쓰는 assertf가 없어 게임 동작 영향은 없다. **테스트 부분**: `MgbaVPrintf_`의 `%c/%C/%U`, hydra의 "and 0 more" 수정. | 없음 | 없음(assertf 이식 시 수십 바이트) | 전 파일 fwd fail. assertf.c 부분은 **#9892 선행**이 필요하다. 러너 복구 시 참고. |
| #10537 | (1.16.4 Fixed) Move ResetTestInventory to fix flaky error | HNS와 무관(테스트 전용) | `test/test_runner_battle.c`의 `ResetTestInventory` 호출 위치 이동, ghost.c·test_test_runner.c 수정. | test/battle/** | 없음 | 선행: #9642(ResetTestInventory, GIVE_PLAYER_ITEM), #9805(ghost.c). 러너 복구 시 참고. |
| #10321 | (1.17.0 Added) feat(test-runner): add support for item popups | HNS와 무관(테스트 지원, 게임 동작 불변) | **게임 코드**: `CreateItemPopUp()`(`src/battle_interface.c:2955`) 첫머리에 `if (gTestRunnerEnabled)` 기록 훅을 추가한다. HnS에서는 `gTestRunnerEnabled`가 `const FALSE`(`src/test_runner_stub.c:5`)라 동작이 바뀌지 않는다. HnS의 `CreateItemPopUp`은 팔레트 로드부(`GetAbilityPopUpSpritePal`)가 달라 fwd fail이다. **테스트 부분**: `ITEM_POPUP()` 매크로와 큐 이벤트. | include/test/battle.h | 없음(몇 바이트) | 선행: #7360(`QUEUED_EFFECTIVENESS_EVENT` 문맥), #9642. HnS는 Champions 이식으로 아이템 팝업 출력 경로를 대거 추가했다(BATTLE_MESSAGE_OUTPUT_CHANGES "아이템 발동"). 러너를 복구하면 그 회귀 검증에 특히 유용하다. |
| #10133 | (1.17.0 Fixed) Remove explicit targets from opponent in Trainer Slide tests | HNS와 무관(테스트 전용) | `test/battle/trainer_slides.c`만 변경. | test/battle/** | 없음 | fwd fail. upstream #9211(Additional trainer slides), #10046 등 테스트 문맥이 빠져 있다. |
| #10214 | (1.17.0 Fixed) Fix Starting Status messages not printing at start of battle | HNS와 무관(upcoming #10170이 만든 회귀 수정; HnS에는 해당 버그 없음) | **게임 코드**: 아래 네 부분으로 나뉜다. **테스트 부분**: 새 파일 `test/battle/starting_status/general.c`(영문 기대값)로, HnS와 무관하다. | src/battle_util.c, src/battle_message.c, src/battle_end_turn.c, include/constants/battle.h, test/battle/** | 없음(현재 이식 불필요) | 단독 이식하지 말 것. upstream #10170을 이식하면 이 PR의 battle_util.c 재구성도 반드시 함께 가져와야 한다. 한글 문자열과는 충돌하지 않는다(유지). |
| #10282 | (1.17.0 Fixed) Fix failing tests using GEN_CHAMPIONS | 미적용·선행 필요(#10151 미이식분) | **게임 코드**: 아래 네 부분으로 나뉜다. **테스트 부분**: 33개 테스트 파일로, HnS와 무관하다. HnS가 고친 `encore.c`와 문맥이 겹친다. | include/battle_util.h, src/battle_util.c, src/battle_script_commands.c, src/battle_move_resolution.c, src/battle_ai_main.c, src/data/moves_info.h, test/battle/** | 작음 | 선행: #10151의 선택 제한 설정 6종, `MOVE_LIMITATION_UNUSABLE`, `B_PARALYSIS_CHANCE`. 이 부분은 HnS의 #10151 수동 이식(SESSION_LOG 2026-09-15)에서 빠졌으므로 Battle Mechanics 그룹 판정과 교차 확인이 필요하다. 테스트 hunk는 `WITH_RNG(RNG_PARALYSIS, …)`를 쓰므로, 테스트를 가져오려면 마비 판정의 `RandomWeighted` 전환이 먼저 필요하다. |
| #10665 | (1.17.0 Fixed) upcoming: Clear callbacks between tests (커밋 제목 "Initialize battle type in Trainer Party Pool test") | HNS와 무관(테스트 전용) | `test/test_runner.c`에 `ResetGlobalVariables()`(콜백 재초기화, `gBattleTypeFlags = 0`)를 추가한다. 게임 코드 없음. | 없음 | 없음 | fwd ok. 테스트 간 상태 누수를 막는 러너 수정이다. 러너 복구 시 참고. |

### Pokémon/Sprites (29건)

| PR | 제목 | 판정 | 근거(파일·함수) | 고위험파일 | 용량 영향 | 의존·비고 |
|---|---|---|---|---|---|---|
| #9945 | (1.15.3 Changed) Garchomp Mega Z Sprites | 이미 적용 | `graphics/pokemon/garchomp/mega_z/{front,back}.png·normal/shiny.pal`의 blob이 #9945 blob과 같다. `src/data/graphics/pokemon.h:15342-15345`는 INCBIN 표기로 연결돼 있고, `gen_4_families.h`의 `[SPECIES_GARCHOMP_MEGA_Z]`(종명 `한카리아스`)는 필드 순서만 달라졌을 뿐 값은 #9945와 같다. | gen_4 species_info(한글 종명 블록) | 없음 | 후속 #10346이 이 스프라이트를 다시 바꿨으나 미적용(아래) |
| #9974 | (1.15.3 Changed) Updated Gen VII animations | 부분 적용 | 14종 `anim_front.png`(NEW)와 종 데이터 프레임은 들어갔다. 미적용: ① `shared_front_pic_anims.h`의 `sAnims_Gumshoos`·`sAnims_MimikyuDisguised`(HnS는 `sAnims_SingleFramePlaceHolder`로 대체, 4곳) ② litten·mareanie·minior(+core 7색) `back.png`·`normal/shiny.pal`이 OLD. **새 anim_front의 인덱스와 옛 팔레트가 어긋난다**: 미니어(유성·코어 전부 색 뒤섞임), 냐오불 idx15 10px, 시마사리 idx13-14 4px가 검정으로 나온다. | species_info gen_7, shared_front_pic_anims.h | 잔여 이식 시 없음(back.png 약 +0.2KB) | 미니어 back.png는 픽셀도 재인덱싱돼 있어 pal과 back을 반드시 함께 옮겨야 한다. 옛 `front.png` 14개는 남아 있지만 참조가 없어 ROM에 들어가지 않는다 |
| #10071 | (1.15.3 Changed) BoxPokemon: Less error-prone "static asserts" | 미적용·안전 이식 가능 | `git apply --check` 통과. `src/pokemon.c`의 STATIC_ASSERT 묶음을 `UNUSED static const struct BoxPokemon sBoxPokemonConstantsFit`로 바꾼다. HnS 값(NUM_LANGUAGES 7, NUM_VERSIONS 15, GENDER_COUNT 2, metLocation은 min(…,0xFD))은 모두 비트필드 안에 들어가고, 필요한 매크로도 모두 있다. | src/pokemon.c | 없음(컴파일 시 검사만) | 1.15.3 |
| #10004 | (1.15.3 Fixed) Fix reverse-cry off-by-one if Porygon disabled | 미적용·안전 이식 가능 | `sound/cry_tables.inc`의 `gCryTable_Reverse` 쪽 `.if P_FAMILY_PORYGON` 위치만 바꾼다(fwd ok). HnS는 P_FAMILY_PORYGON이 TRUE라 출력 결과는 같다. | sound/cry_tables.inc | 없음 | SESSION_LOG 2026-09-16의 "Porygon/PCM 변경 되돌림"이 이 PR로 보인다. 효과가 없으니 선택 사항 |
| #9594 | (1.16.0 Added) refactor(graphics): make spinda spots generic | 미적용·수동 적응 가능 | HnS는 옛 `DrawSpindaSpots`(`src/pokemon.c:7561`, `src/decompress.c:1175-1179`)를 그대로 쓴다. 새 `src/pokemon_spots.c`·`include/pokemon_spots.h`는 시그니처에 `enum Species`만 쓰므로 `u16`로 바꾸면 옮겨진다. 실패 원인은 HnS의 `pokemon.h`·`pokemon.c` 주변 문맥(Johto 도감 등)이다. | src/pokemon.c, src/decompress.c(HnS 806c6f2af4 수정 파일) | 작음(코드 수백 B 이내) | 기능 변화 없는 리팩터라 실익이 낮다. 옮길 경우 #9796과 #10247(isEgg 검사, 이 그룹 밖)을 같이 가져가야 한다. 알록반 점 2의 기준 y가 25→27로 2px 바뀐다(불확실 절) |
| #9173 | (1.16.0 Changed) Separate Perfect IV Logic Into New Function | 미적용·안전 이식 가능 | `SetBoxMonPerfectIVs()` 분리 + DexNav `CreateDexNavWildMon`의 중복 로직 제거. 실패 원인은 HnS 고유 코드와 인접한 문맥이다(`src/pokemon.c:3231` SetBoxMonIVs의 챌린지 MaxPartyIVs 분기, `src/dexnav.c:1206` 사파리 포켓블록 특성 분기). 의미 충돌은 없다. | src/pokemon.c, src/dexnav.c(HnS 수정) | 없음 | hunk 수동 배치 필요. HnS 두 분기는 그대로 보존 |
| #9135 | (1.16.0 Changed) Allow other species to have Shedinja HP handling | 미적용·안전 이식 가능 | `HasShedinjaHPHandling()` 추가, 새 설정 `P_BASE_HP_1_SHEDINJA_HANDLING` 기본 FALSE → 동작 불변. `battle_dome.c`·`battle_dynamax.c`·`party_menu.c`·`pokemon.h`는 그대로 적용된다. `include/config/pokemon.h`와 `pokemon.c` 파일 끝부분은 HnS 문맥 차이로 수동 배치가 필요하다. | include/config/pokemon.h(사용자 설정), src/battle_dome.c, src/battle_dynamax.c | 없음 | config 파일은 한 줄만 추가하고 기존 HnS 설정값은 건드리지 않는다 |
| #9507 | (1.16.0 Changed) Add Species enum | 미적용·수동 적응 가능(대규모, 표본 확인) | HnS `include/constants/species.h`는 merge-base와 같아 HnS 전용 SPECIES_* 추가가 **없다**(종 enum화와 HnS 전용 종의 겹침 없음). 148개 파일 중 108개는 그대로 적용되고 40개는 실패한다. 실패 표본 5개(`battle_util.c` CalcBeatUpPower의 BST 균등화, `pokedex.h` Johto, `roamer.c` locationTableId, `daycare.c`, `battle.h` 주석)는 모두 HnS 고유 코드 문맥 때문이며 다른 upcoming PR이 원인은 아니었다. 변경은 타입 표기뿐이다(packed enum이라 구조체 크기 불변, preproc가 asm용 enum을 지원). | include/battle*.h, battle_util.c, battle_script_commands.c, battle_main.c, battle_move_resolution.c, dexnav.c, pokedex_plus_hgss.c, daycare.c, debug.c 등 | 없음 | 실익이 낮고 수정 범위가 커서 **보류 권장**. 이후 PR의 `enum Species`는 `u16/u32`로 바꿔 옮기면 되므로 선행 필수는 아니다 |
| #9558 | (1.16.0 Changed) Add constants nudging users where to put custom species | 미적용·수동 적응 가능 | #9507 enum 안에 `SPECIES_CUSTOM_START/END`를 추가한다. HnS의 `#define` 체계에서는 같은 값의 define으로 표현할 수 있다(SPECIES_EGG 값 불변). | include/constants/species.h | 없음 | HnS에 커스텀 종이 없어 실익이 없다. 생략 권장 |
| #9690 | (1.16.0 Fixed) Modernize imperial weight conversion formula | 미적용·안전 이식 가능 | HnS `UNITS = UNITS_IMPERIAL`(`include/config/general.h:85`)이라 실제로 쓰이는 경로다. `src/pokedex.c:4430` ConvertMonWeightToImperialString은 그대로 적용된다(오프셋 19). `include/constants/pokedex.h:2475` DECAGRAMS_IN_POUND 한 줄은 HnS Johto 문맥 때문에 수동 반영이 필요하다. 이 상수를 쓰는 곳은 pokedex.c뿐이다. | src/pokedex.c | 없음(`__udivdi3` 이미 링크) | 두 줄을 반드시 함께 바꿔야 한다(한쪽만 바꾸면 100배 오차) |
| #9796 | (1.16.0 Fixed) Add Spot Coord Limits | 미적용·선행 필요 | #9594가 새로 만드는 `src/pokemon_spots.c` DrawPokemonSpots의 경계 검사다. HnS 옛 `DrawSpindaSpots`에는 해당 코드 경로가 없다. | - | 없음 | 선행 #9594. #9594를 안 옮기면 HNS와 무관 |
| #10116 | (1.16.1 Changed) Fix missing incgfx for Mega Garchomp Z | HNS와 무관 | INCGFX 빌드 체계(pret#2283 이관, upcoming)에서 생긴 누락을 고친 PR이다. HnS에는 INCGFX 매크로 자체가 없고, `INCBIN_U32(".../front.4bpp.smol")`를 일반 규칙으로 생성한다. | - | 없음 | #9945 후속 |
| #10250 | (1.16.2 Changed) Add new data from Champions Regulation M-B | 이미 적용 | `ABILITY_EELEVATE(313)`·`ABILITY_FIRE_MANE(316)`에 한글명 `천정부지`·`불꽃의갈기`가 들어가 있고, 메가 특성 11개 hunk도 모두 NEW다(라이츄X/Y, 찌르호크, 펜드라, 곤율거니, 저리더프, 화염레오, 칼라마네로, 거북손데스, 드래캄, 대여르 메가). | abilities.h(한글 특성명), species_info | 없음 | species hunk 문맥에 영문 speciesName이 있어 HnS(한글)에서는 기계 적용이 안 되지만, 값은 동일하게 들어가 있다 |
| #10327 | (1.16.2 Changed) Fix Ursaluna Bloodmoon and Pikachu forms overworld sprites | 이미 적용 | HnS 커밋 `ffcedaf955`(Fix Ursaluna overworld sprite)로 반영됐다. overworld.png·pal 3개 NEW, `[SPECIES_URSALUNA_BLOODMOON]` OVERWORLD hunk NEW, `-R --check` ok. | - | 없음 | 제목의 Pikachu 부분은 이 커밋에 파일 변경이 없다 |
| #10141 | (1.16.2 Added) More Gen VII animation data added | 미적용·수동 적응 가능 | 13종(bounsweet·bruxish·comfey·cosmog·dhelmise·fomantis·komala·morelull·palossand·sandygast·shiinotic·steenee·tsareena)의 `anim_front.png`가 없고(ABSENT) 종 데이터도 OLD다. `gen_7_families.h`와 PNG는 그대로 적용되고, `graphics/pokemon.h` 13줄만 INCGFX→INCBIN 표기 변환이 필요하다. HnS가 이 종들의 그래픽을 수정한 이력은 없다. anim ID 53종은 모두 HnS `pokemon_animation.h`에 있다. | species_info gen_7, graphics/pokemon.h | 작음(+약 3.96KB) | #10208의 sandygast PNG 수정(−36B)과 같이 옮기는 게 좋다 |
| #10208 | (1.16.2 Added) More Gen VII Animation Data | 부분 적용 | 14종 anim_front NEW, pokemon.h·종 데이터 NEW(춤추새 4폼은 주석 한 줄만 남음). 미적용은 `sandygast/anim_front.png` 수정 1건인데, 이 파일은 #10141이 만든다. | - | 없음 | 잔여분은 선행 #10141 |
| #10270 | (1.16.3 Changed) Even More Gen VII Animation Data + Gen VIII Clean Up | 부분 적용 (충돌 hunk 1) | Gen VII 19종 anim_front·종 데이터는 NEW다. 미적용: ① `sAnims_Magearna`(마기아나 2폼이 자리표시자) ② silvally(18타입 pal + back 재인덱싱)·type_null pal·back이 OLD여서 **실바디·타입:널 앞모습 색이 어긋난다** ③ Gen VIII 정리(파라꼬·아머까오·깨물부기·탄동·탄차곤·석탄산·과사삭벌레·애프룡·단지래플·태우지네·다태우지네·찌르성게의 frontAnim, appletun anim_front.png) ④ 대여르 메가 특성 hunk(충돌 절 A). | species_info gen_7/gen_8, shared_front_pic_anims.h | 없음(약 +0.05KB) | Gen VIII hunk 11개는 그대로 적용되고, 대여르 메가 hunk만 한글 종명 문맥 때문에 실패한다. 이 hunk는 제외해야 한다 |
| #10346 | (1.16.3 Changed) PokeCommunity sprites batch (June 2026) | 부분 적용 | 적용된 것: 메가찌르호크 아이콘 PNG(NEW)와 `.iconSprite = gMonIcon_StaraptorMega`(SESSION_LOG의 "#10561 이식"). 미적용: 메가한카리아스Z front/back/pal과 `enemyMonElevation = 8`·`SHADOW(0,18,L)`, 메가모단단게·메가냐오닉스·메가펜드라 아이콘, 자시안·자마젠타(+왕) 아이콘과 `iconPalIndex 2→0`. 해당 경로에 HnS 수정 이력은 없다. | species_info gen_4/gen_8 | 없음(아이콘 고정 1KB, Garchomp −60B) | 자시안·자마젠타 아이콘 PNG와 iconPalIndex는 반드시 같이 옮긴다. gen_8 hunk는 그대로 적용되고 gen_4는 수동이다 |
| #10414 | (1.16.3 Changed) Alolan and Mega Form Animation Frames | 부분 적용 (충돌 1) | 32개 anim_front·종 데이터 NEW. 미적용: ① `sAnims_RaticateAlola`·`sAnims_MarowakAlola`·`sAnims_Lurantis`(+토템) 자리표시자. **라란티스는 이식 전 HnS에 2프레임 ANIM_FRAMES가 있었는데 1프레임으로 퇴행했다** ② `sAnims_Vikavolt` 표 갱신이 빠져서, 이식 후 참조를 바꾼 **투구뿌논이 옛 1회 동작으로 퇴행했다** ③ 꼬마돌 알로라·페르시온 알로라·메가뮤츠Y pal·back이 OLD라 새 프레임 색이 검정으로 나온다(메가뮤츠Y idx15 84px) ④ marowak-알로라는 HnS 커스텀 팔레트(충돌 절 B). | species_info gen_1/gen_7, shared_front_pic_anims.h, marowak/alola(HnS 커스텀) | 없음 | `shared_front_pic_anims.h`는 HnS=merge-base이고, 1.17.0까지 이 파일을 바꾼 PR은 #9974·#10270·#10414 셋뿐이다. 1.17.0판으로 갱신한 뒤 자리표시자 10곳만 교체하면 된다 |
| #10368 | (1.16.3 Fixed) Fix duplicate CANCEL entry in move relearner list | HNS와 무관 | upcoming #9006(Move relearner refactor)의 `UIEndTask`/`RedrawListMenu` 경로에서 생긴 버그다. HnS `src/move_relearner.c`는 merge-base 그대로이고, 복귀 때마다 `CB2_InitLearnMoveReturnFromSelectMove`가 구조체를 AllocZeroed한 뒤 목록을 다시 만든다. 그래서 중복 CANCEL이 생기지 않는다. | src/move_relearner.c | 없음 | #9006을 들이면 함께 필요 |
| #10387 | (1.16.3 Fixed) Remove text encoding error in za.json | HNS와 무관 | `tools/learnset_helpers/porymoves_files/za.json`의 키 문자열만 고친다(fwd ok). `all_learnables.json`은 order-only 의존이라 재생성되지 않는다. HnS는 all_learnables.json을 직접 편집하고 있으므로(3038dcd2e2 등) 재생성하면 안 된다. | tools(도구 데이터) | 없음 | 적용해도 무해하다. 단 `make clean-teachables` 금지 |
| #10397 | (1.16.3 Fixed) Fix ANIM_RAPID_H_HOPS freezing when opponent is wild shiny | 부분 적용 | 멈춤의 원인은 `battle_controllers.c:3132`의 `animEnded && x2 == 0` 대기다. HnS 자체 커밋 `877d3aea01`(Fix shiny rat)이 `Anim_RapidHorizontalHops` 외 10여 개 애니에 `x2 = 0`을 넣어 이미 해결했다. 미적용은 `y2 = 0`(외형만)과 테스트 러너 `forceMoveAnim` 훅(`battle_anim_throw.c:2284`, `pokemon_animation.c:512`), `test/battle/front_anim.c`다. | src/battle_anim_throw.c, test/battle/** | 없음 | 잔여분은 선택 사항이다. HnS 쪽 수정이 범위가 더 넓으므로 보존한다 |
| #10535 | (1.16.3 Fixed) update mega magearna pokedex entry | 미적용·안전 이식 가능 | `gen_7_families.h`의 `[SPECIES_MAGEARNA_MEGA]` 설명을 바꾼다(fwd ok). HnS 도감 설명은 전 세대 1,315개 모두 영문이다(한글 설명 0건). 종명 `마기아나`·분류 `인조`는 hunk 밖이라 한글 텍스트를 덮지 않는다. | species_info gen_7 | 없음 | 영문 → 영문 교체다 |
| #10561 | (1.16.4 Changed) Converted Mega Staraptor's icon to INCGFX_U8 | HNS와 무관(목적 충족) | #10346이 넣은 `INCBIN_U8(".../icon.4bpp")`를 INCGFX 체계용으로 바꾼 표기 수정이다. HnS는 INCGFX가 없고 `pokemon.h:14481` INCBIN_U8과 일반 `%.4bpp` 규칙으로 같은 결과를 낸다. | - | 없음 | SESSION_LOG의 "#10561 이식"은 실제로는 #10346의 아이콘 부분을 INCBIN 경로로 연결한 것이다 |
| #10603 | (1.16.4 Changed) Correct the ZA Mega body colors | 이미 적용 | 8개 파일, 18개 메가 `bodyColor` hunk가 모두 NEW다. | - | 없음 | SESSION_LOG 기록과 일치 |
| #10601 | (1.16.4 Fixed) Fix Pokedex Plus evolution text misalignment | 이미 적용 | `pokedex_plus_hgss.c:6373` HandleTargetSpeciesPrintText에 `numLines` 인자가 있고, `:6383`의 y 계산, `:6661`·`:7041`의 `arrowSpriteDist[*depth_i]`가 모두 반영돼 있다(`u16` 표기). | src/pokedex_plus_hgss.c(한글 도감 UI) | 없음 | - |
| #9987 | (1.17.0 Changed) Better quality Legends Z-A cries | HNS와 무관(의도적 미적용) | 메가 WAV 26개가 모두 OLD다. `sound/direct_sound_data.inc`의 메가 울음소리는 `.if P_MODIFIED_MEGA_CRIES == TRUE`로 감싸여 있고 HnS는 FALSE(`CRY_MODE_HIGH_PITCH`)라 빌드에 링크되지 않는다. SESSION_LOG 2026-09-16의 사용자 결정(32MiB)으로 되돌린 이력도 있다. | sound/(사용자 결정) | 현재 설정에서는 없음. 설정을 켜면 큼 | 적용하지 않는다 |
| #10252 | (1.17.0 Changed) Scorbunny line front anim | 미적용·수동 적응 가능 | 염버니·래비풋·에이스번의 anim_front가 없고(ABSENT) 종 데이터도 OLD다. `gen_8_families.h`와 PNG는 그대로 적용되고, pokemon.h 3줄만 INCGFX→INCBIN 변환이 필요하다. HnS 수정 이력은 없다. | species_info gen_8 | 작음(+약 1.23KB) | - |
| #10206 | (1.17.0 Added) Add second front frames to Tadbulb and Bellibolt | 미적용·수동 적응 가능 | 빈나두·찌리배리의 anim_front가 없고 종 데이터도 OLD다. `gen_9_families.h`와 PNG는 그대로 적용되고, pokemon.h 2줄만 표기 변환이 필요하다. | species_info gen_9 | 작음(+약 0.44KB) | - |

## 충돌·주의 항목 상세

PR 단위 전면 충돌(한글 문자열·{B_...} 코드·배틀 메시지 출력 최신화와 같은 hunk가 겹쳐 사용자 결정 없이는 진행할 수 없는 항목)은 이 그룹에 없다. 다만 다음 hunk 단위 항목은 이식 전에 사용자 결정이나 주의가 필요하다.

### Battle AI

#### C-AI-1. #9710 `DamageContext` 배열화 리팩터 — 선행 필요(upcoming 구조 개편), 메시지와 무관

- **upstream 목적**: 대미지 계산 컨텍스트가 공격자·방어자 두 명분만 들고 있던 특성·도구를 `abilities[MAX_BATTLERS_COUNT]`/`holdEffects[MAX_BATTLERS_COUNT]` 배열로 바꾼다. 파트너 특성(Friend Guard 등)도 AI 가상값으로 조회하게 하려는 것이다. 같은 PR에서 Friend Guard가 혼란 자해 대미지를 줄이지 않도록 했다(`ctx->battlerAtk != ctx->battlerDef`).
- **HnS 현재 동작**:
  - HnS는 upstream master(1.15.2 개발) 계열이다. merge-base `3efb836f72`의 `struct BattleContext`를 그대로 쓰며, upcoming의 `DamageContext` 개명·개편은 없다. `include/battle_util.h:107` `struct BattleContext`는 `abilityAtk/abilityDef/holdEffectAtk/holdEffectDef`와 `fieldStatuses`를 유지한다.
  - 파트너 특성은 `GetDamageCalcAbility(ctx, battler)`(`src/battle_util.c:6153-6162`)가 `ctx->aiCalc`일 때 `gAiLogicData->abilities[]`를 읽어 가상값을 쓴다. 따라서 PR의 핵심 목적은 이미 달성됐다.
  - `GetDefenderPartnerAbilitiesModifier`(`:7745-7759`)에는 혼란 자해 예외가 없다.
- **보존 이유**: 2026-09-15의 1.17.0 AI 이식(#10145, #10453, #10610, #10277 등)이 전부 이 HnS 구조체에 맞춰 어댑트돼 있다. 배열화하려면 `battle_util.c`(약 220줄), `battle_script_commands.c`, `battle_move_resolution.c`, `battle_ai_*.c`를 대량 치환해야 한다. 이는 지시서가 금지한 "고위험 파일 대규모 자동 치환"에 해당한다.
- **선택지**:
  - (a) 배열화는 이식하지 않고 Friend Guard 혼란 자해 예외 1줄만 넣는다. **권장**. 영향은 더블에서 혼란 자해 대미지가 25% 늘어나는 정확도 수정이다.
  - (b) 배틀 그룹의 `BattleContext`→`DamageContext` 개명·개편 PR과 한 단위로 묶어 나중에 일괄 검토한다.
  - (c) 전부 보류한다.

#### C-AI-2. #10461 이식본과 upstream의 아군 대상 점수 기준 차이(동작 차이, 사용자 확인 필요)

- **upstream 1.17.0**: `if (battlerDef == GetPartnerBattler(battlerAtk) && bestMovePointsForTarget[battlerDef] < AI_SCORE_DEFAULT)` → 점수가 정확히 100(기본값)인 아군 대상 기술은 허용한다.
- **HnS**: `src/battle_ai_main.c:1030-1031`이 `<= AI_SCORE_DEFAULT`이고 주석도 "unless it scores above the default 100 points"로 바뀌었다. 기본 점수 그대로인 아군 대상 기술(예: 가산이 없는 도우미류)을 AI가 고르지 않는다.
- 이식 과정의 의도적 수정인지 실수인지 문서에 기록이 없다. 되돌릴지 보존할지는 사용자 결정 사항이다. 이번 조사에서는 수정하지 않는다.

#### C-AI-3. #9568 기본 AI 대미지 롤 변경(난이도 영향, 사용자 결정 필요)

- **upstream**: 컨텍스트별 롤 config를 도입했다. 기본값은 `AI_ROLL_ATTACKING = AI_ROLL_MAX`, `AI_ROLL_SHOULD_SETUP_DEFENDING = AI_ROLL_MAX`, `AI_ROLL_ATTACKING_PARTNER = AI_ROLL_MAX`다. 즉 AI가 자신의 공격 대미지를 최대 롤로 가정한다.
- **HnS**: 모든 컨텍스트가 median(`DMG_ROLL_DEFAULT`)이다. 이식하면 AI가 "확정 1타"를 더 낙관적으로 판단해 공격 기술 선택 빈도가 달라진다.
- **선택지**: (a) 이식하되 HnS 기존 체감을 유지하도록 `AI_ROLL_ATTACKING`을 `AI_ROLL_MEDIAN`으로 둔다. (b) upstream 기본값을 따른다. (c) 보류한다. 선행 조건은 #9460이다.

#### 참고(충돌 아님): #10427 이식 후 남은 잠재 문제

- HnS `ShouldTrap`은 #10427 이식으로 `gBattleMons[battlerDef].volatiles.wrapped`를 잠시 TRUE로 두고 `BattlerWillFaintFromSecondaryDamage`를 호출한다.
- 이 경로에서 `GetTrapDamage`(`src/battle_ai_util.c:3515`)가 `holdEffects[volatiles.wrappedBy]`를 읽는다. 실제로 묶이지 않은 대상이면 `wrappedBy`가 이전 값이라 조임밴드 판정이 틀릴 수 있다.
- upstream은 #10289(`wrappedBindingBand` 휘발 플래그)와 #10424로 이 문제를 해결했다. 두 PR을 함께 이식하는 것을 권장한다(#10289는 배틀 그룹).

### Pokémon/Sprites


#### A. #10270 — 대여르 메가(`[SPECIES_FALINKS_MEGA]`) 특성 hunk
- upstream 목적: #10270은 애니메이션 PR인데, `src/data/pokemon/species_info/gen_8_families.h` upstream 5286행 부근에서 `.abilities`를 `{ ABILITY_DEFIANT ×3 }` → `{ ABILITY_BATTLE_ARMOR, ABILITY_NONE, ABILITY_DEFIANT }`로 바꾼다. 이 값은 바로 전 PR #10250(Champions Regulation M-B)이 넣은 Defiant ×3을 되돌린 것이다. 병합 중 실수로 되돌린 것으로 보인다. 1.17.0 태그에는 되돌린 값이 남아 있다.
- HnS 현재: `gen_8_families.h` `[SPECIES_FALINKS_MEGA]`(종명 `대여르`)가 `{ ABILITY_DEFIANT, ABILITY_DEFIANT, ABILITY_DEFIANT }`다(#10250 이식값, 1821fd6749). HnS에 들어간 Champions 데이터(#10058/#10250/#10257)와 일관된다.
- 기계 적용: 이 hunk만 문맥의 `.speciesName = _("Falinks")`가 HnS 한글명과 달라 실패한다(나머지 Gen VIII hunk 11개는 성공).
- 선택지:
  - (1) HnS 값 유지, 이 hunk 제외(권장): Champions 데이터와 일관된다.
  - (2) 1.17.0 값으로 맞추기: 대여르 메가의 특성 구성이 바뀌어 실전 밸런스에 영향이 있다.
  - (3) upstream 이후 커밋이나 이슈에서 의도를 확인한 뒤 결정한다.

#### B. #10414 — 텅구리 알로라 앞모습 팔레트(`graphics/pokemon/marowak/alola/`)
- upstream 목적:
  - 알로라 텅구리에 2프레임 `anim_front.png`를 추가한다(HnS 이미 NEW, `pokemon.h:4257`이 참조).
  - `normal.pal`(idx10 `32 24 48`→`88 42 58`, idx12 `80 72 136`→`94 84 160`), `shiny.pal`(3색), `back.png`(45px 재인덱싱)도 함께 고친다.
- HnS 현재:
  - HnS 개발 커밋 `993b7bdf50`("Isles merged", 2026-07-02)이 `normal.pal` idx11·12·14를 보라 계열(`65 46 68`/`92 68 97`/`65 46 68`)로 바꾼 **HnS 커스텀 팔레트**를 쓴다.
  - 같은 커밋의 `front.png`는 픽셀이 upstream과 같고 PLTE만 다르다. 현재는 `anim_front.png`(upstream)와 HnS 커스텀 `normal.pal` 조합으로 표시된다.
  - `front.png`는 참조되지 않는다. `shiny.pal`·`back.png`는 upstream 옛판이다.
- 보존 이유: 그래픽은 HnS 우선이고, 1.17.0 병합 사고(HnS/Soulgold 그래픽 덮어씀)의 재발도 막아야 한다.
- 선택지:
  - (1) `normal.pal`은 HnS 유지, `shiny.pal`·`back.png`·애니 표(`sAnims_MarowakAlola`)만 이식한다(back.png의 새 45px가 HnS idx10·12 색으로 보이는지 화면 확인 필요).
  - (2) back.png도 보류하고 애니 표만 이식한다.
  - (3) upstream 팔레트를 채택하고 HnS 커스텀 색을 재적용한다(비권장, 사용자 결정 필요).

(PR 단위 충돌은 없다. #9507은 HnS 고유 코드 40개 파일과 문맥이 겹치지만 타입 표기만 다르므로 "수동 적응 가능·보류 권장"으로 분류했다.)


### Test Runner


**전면 충돌(HnS 고유 변경·한글화·배틀 메시지 최신화와 같은 hunk가 겹쳐 사용자 결정이 필요한 항목)은 없다.** 다만 다음 항목은 겹치는 부분이 있으니 이식할 때 주의해야 한다.

#### 2-1. #10214 `src/battle_message.c:847-849`(불바다·습지 문자열) — 충돌 아님, 보존

- upstream 목적: #9918에서 `{B_EFF_TEAM2}`로 바뀐 불바다·습지 문자열을 `{B_ATK_TEAM2}`/`{B_DEF_TEAM2}`로 되돌리고, 턴 종료 코드가 `gBattlerAttacker`를 설정하게 한다.
- HnS 현재 동작: 한글 문자열이 이미 upstream의 목표 토큰을 쓴다. 턴 종료에서는 무지개 블록이 `gBattlerAttacker`를 먼저 설정하므로 출력되는 편이 올바르다.
- 보존 이유: 한글 본문·줄바꿈·`{B_...}` 토큰은 HnS가 우선이다. upstream 영문 줄을 가져오면 한글 번역이 영문으로 바뀐다.
- 선택지: (a) 아무것도 적용하지 않는다(권장). (b) #10170을 이식하게 되면 `battle_util.c` 재구성만 기능 단위로 함께 반영하고, `battle_message.c` hunk는 건너뛴다.

#### 2-2. #10282 테스트 부분과 HnS 테스트·메시지 최신화 — 러너 복구 때만 해당

- 겹치는 파일: HnS가 +86행을 추가한 `test/battle/move_effect/encore.c`(fwd fail).
- upstream이 기대값을 바꾼 `refresh.c`, `aromatherapy.c`, `heal_bell.c`, `life_orb.c`, `sleep.c`, `freeze.c` 등은 HnS가 바꾼 출력(BATTLE_MESSAGE_OUTPUT_CHANGES: 리프레시·아로마테라피 계열의 상태별 치료 문구, 생명의구슬 팝업과 `STRINGID_LOSTSOMEOFITSHP`)과 다르다.
- 선택지: 러너를 복구할 때 테스트 기대값을 HnS 동작·한글 문구 기준으로 다시 쓴다. upstream 테스트를 그대로 덮어쓰지 않는다.


## 부분 적용 잔여분(Pokémon/Sprites, 이식 단위)


1. **팔레트·back 불일치 해소(색 버그 수정, ROM 약 +0.2KB)**
   - #9974: `graphics/pokemon/litten/{back.png,normal.pal,shiny.pal}`, `mareanie/{back.png,normal.pal,shiny.pal}`, `minior/{back.png,normal.pal,shiny.pal}`, `minior/core/{back.png,shiny.pal,blue|green|indigo|orange|red|violet|yellow/normal.pal}`
   - #10270: `silvally/{back.png,normal.pal,shiny.pal}` + 17개 타입 폴더의 `normal/shiny.pal`, `type_null/{back.png,normal.pal,shiny.pal}`
   - #10414: `geodude/alola`, `persian/alola`, `mewtwo/mega_y`의 `{back.png,normal.pal,shiny.pal}`
   - marowak/alola는 충돌 절 B를 따른다.
   - 미니어·실바디 back.png는 픽셀이 재인덱싱돼 있어(각 1537/1321/2208px 변화) pal과 반드시 함께 옮겨야 한다.
2. **애니 표 복구(퇴행 수정)**
   - `src/data/pokemon/species_info/shared_front_pic_anims.h`를 1.17.0판으로 갱신한다(HnS 파일 = merge-base이고, 변경분은 이 세 PR뿐이라 순수 추가와 Vikavolt 수정만 들어간다).
   - 다음 10곳의 `sAnims_SingleFramePlaceHolder`를 해당 표로 바꾼다: 형사구스(+토템)→Gumshoos, 따라큐(+토템)→MimikyuDisguised, 마기아나(+오리지널)→Magearna, 레트라 알로라(+토템)→RaticateAlola, 텅구리 알로라(+토템)→MarowakAlola, 라란티스(+토템)→Lurantis.
3. **#10270 Gen VIII 정리**: gen_8 hunk 11개와 `appletun/anim_front.png`를 옮긴다. 대여르 메가 특성 hunk는 제외한다.
4. **#10346 잔여**: 아이콘 7개, 자시안·자마젠타 `iconPalIndex = 0` 4곳, 메가한카리아스Z 스프라이트·pal·elevation·shadow.
5. **#10208 잔여**: sandygast PNG는 #10141과 함께 옮긴다.


## Test Runner 게임 코드 세부

> 코디네이터 정정에 따라 #10214는 'HNS와 무관'으로 재분류했다. #10170(Starting Status for Weather)은 upcoming 전용이고 HnS에는 없다. 아래 세부의 '선행 필요' 표현은 '#10170을 이식할 때 동반 필요'로 읽는다.

#### #10214 게임 코드 세부

1. **`src/battle_message.c`는 이미 동등하다.**
   - upstream은 #9918(Customizeable Pledge Moves)이 바꾼 `{B_EFF_TEAM2}`를 `{B_ATK_TEAM2}`/`{B_DEF_TEAM2}`로 되돌렸다.
   - HnS는 #9918이 없고, 한글 문자열 847-849행이 이미 `{B_ATK_TEAM2} 주변의\n불바다가 사라졌다!`, `{B_DEF_TEAM2} 주변에\n습지초원이 펼쳐졌다!`, `{B_ATK_TEAM2} 주변의\n습지초원이 사라졌다!`다. 수정할 것이 없다.
2. **`src/battle_end_turn.c`는 불필요하다.**
   - 이 hunk는 불바다 소멸 전에 `gBattlerAttacker = GetBattlerSideForMessage(side)`를 넣는다.
   - HnS에서는 같은 편 처리 중 바로 앞 블록인 `SECOND_EVENT_BLOCK_RAINBOW`(1036-1037행)가 이 값을 무조건 설정한다. 블록 순서는 `include/constants/battle_end_turn.h:81-83`에 있다. 따라서 불바다 문구의 `{B_ATK_TEAM2}`는 현재도 올바른 편을 가리킨다.
3. **`src/battle_util.c`는 불필요하다.**
   - 이 hunk는 #10170(Starting Status for Weather)이 `TryFieldEffects` 끝의 `if (effect) … BattleScript_OverworldStatusStarts` 푸시를 지워 생긴 회귀를 고친다.
   - HnS에는 #10170이 없다(`STARTING_STATUS_WEATHER_*` 없음). 그리고 `TryFieldEffects` 끝부분(2958-2964행)이 그대로 남아 있어 방·순풍·무지개·불바다·습지 시작 메시지가 정상 출력된다.
4. **나머지 두 hunk는 효과가 없다.**
   - 안개 `moveStartMessage`의 `B_MSG_STARTED_FOG` 교체(205-213행): HnS에서는 이 필드를 읽는 코드가 없다.
   - `include/constants/battle.h` 변경: 주석뿐이다.

#### #10282 게임 코드 세부

1. **선택 제한 hunk는 HnS에 선행 코드가 없다.**
   - 대상: `CheckMoveLimitations` 인자를 u16에서 u32로 확장, AI `moveLimitations`(`battle_ai_main.c:705`)와 잠꼬대(`battle_move_resolution.c:4221`)의 `~MOVE_LIMITATION_UNUSABLE`.
   - 이 hunk들은 #10151이 추가한 `MOVE_LIMITATION_UNUSABLE`(1<<15)와 `PLACEHOLDER`(1<<16)를 전제로 한다. HnS `include/battle_util.h:8-25`에는 UNUSABLE이 없고 PLACEHOLDER가 1<<15다. 즉 u16으로도 문제가 없다.
   - HnS에는 `B_BELCH/STUFF_CHEEKS/SPIT_UP/LAST_RESORT_SELECTABLE`, `B_FIRST_TURN_MOVE`, `B_MOVES_THAT_REMOVE_TYPE` 선택 제한도 없다(`IsBelchPreventingMove` 1388-1394행 등).
2. **마비 hunk(`CancelerParalyzed`, `battle_move_resolution.c:435`)는 확률이 같다.**
   - HnS는 `!RandomPercentage(RNG_PARALYSIS, 75)`이고, `B_PARALYSIS_CHANCE`(Champions 87.5%)가 없다.
   - `RandomWeighted(…, 3, 1)`로 바꿔도 25% 확률은 같다. 테스트 RNG 지정용 변경이다.
3. **수면 턴 hunk(`battle_script_commands.c:2358`)는 동작이 같다.**
   - `B_SLEEP_TURNS`를 `GetConfig()`로 바꾼다. HnS에 `CONFIG_B_SLEEP_TURNS`가 있어 fwd ok이고, `GEN_LATEST` 설정에서 동작은 같다(테스트용).
4. **Z기술 선택 우회(`TrySetCantSelectMoveBattleScript`, `battle_util.c:1397` 이하)는 지금 HnS에서 도달하지 않는다.**
   - Z기술을 "선택"한 상태(`IsGimmickSelected`)에서도 앵콜·사슬묶기·트집·도발·지옥찌르기·봉인·중력·회복봉인·트림 제한을 우회하는 로직 수정이다.
   - HnS 맵·트레이너 데이터에는 Z링과 Z크리스탈 배포가 없다(`data/`, `src/data/trainers*.party` 검색 결과 0건). 정상 플레이에서 도달할 수 없다.
5. **프리즈드라이 설명(`moves_info.h:14849`)은 해당 없다.**
   - HnS에는 `B_UPDATED_MOVE_DATA` 설명 분기가 없다(설명은 영문 단일본).


### 테스트 러너 복구 시 참고 순서


1. #9892(특히 `35a45557e1`의 include 순서, `17d3887310`의 `%.*s`)로 `fake_rtc.h` 컴파일 오류를 해소한다.
2. 러너 인프라를 upstream 순서대로 가져온다: #7360(테스트 부분) → #9805(GHOST 지원, 선택) → #9642 → #10155 → #9807 → #10321 → #10390 → #10537 → #10665. 사이의 러너 커밋(#10127, #10051, #10326, #10542, #10619, #10659 등)도 필요하다.
3. 영문 `MESSAGE()` 기대값을 한글 문구로 바꿀지, 메시지 검사를 뺄지 정책을 정한다(0절 참고). HnS 고유 회귀 테스트(teleport.c 등)의 부정 검사도 한글 문자열 기준으로 고친다.

## 불확실 항목

### Battle AI

1. #10461 이식본의 `<= AI_SCORE_DEFAULT`(upstream `<`)가 의도한 변경인지 기록이 없다(A-2).
2. #9124·#9551·#8472·#9451 스위칭 AI 묶음은 HnS `InitializeSwitchinCandidate`의 Champions Supreme Overlord 어댑트, `HOLD_EFFECT_CONFUSE_FLAVOR`(#10163)와 겹친다. 문맥 병합 후 빌드로 확인해야 한다. 의존 심볼과 시그니처(IsMistyTerrainAffected 등)는 HnS에 모두 있음을 확인했다.
3. #9857은 예측 기술 필터 제거라는 동작 변화가 있다. HnS가 추가한 `CanAiPredictMove(battler)` 조건과 어떻게 합칠지 결정이 필요하다.
4. #10424·#10427: 이식된 #10427의 Wrap 시뮬레이션이 `GetTrapDamage`에서 오래된 `wrappedBy`를 읽을 수 있다. #10289(배틀 그룹, HnS `HandleEndTurnWrap`에도 같은 조임밴드 무효화 버그 존재)와 #10424를 함께 이식할지는 배틀 그룹 판정과 교차 확인해야 한다.
5. #9709·#9568은 구조체 크기를 늘린다(`BattlePokemon`, `SimulatedDamage`). EWRAM 95% 상황이라 이식 후 링크 맵으로 실측해야 한다.
6. AI 판정은 코드 대조와 apply 검사에 근거한다. 빌드와 실전(mGBA) 검증은 하지 않았다.

### Pokémon/Sprites


1. 팔레트 불일치 색 버그(미니어·실바디·타입:널·메가뮤츠Y 등)는 PNG의 PLTE·인덱스 사용량 분석으로 확인했다. 실제 mGBA 화면으로는 검증하지 않았다.
2. 대여르 메가 특성의 올바른 값: #10250(Defiant ×3)과 #10270 이후 1.17.0(Battle Armor/None/Defiant) 중 무엇이 맞는지 upstream 의도를 확인하지 못했다.
3. 텅구리 알로라: HnS 커스텀 팔레트와 새 back.png를 조합했을 때의 결과는 화면 확인이 필요하다.
4. #9594: 새 구현에서 알록반 점 2의 기준 y가 25→27로 바뀐다. 기존 개체의 무늬가 2px 달라지는데, upstream이 의도한 것인지 확인하지 못했다.
5. #9507: 표본 확인만 했다. 실패 40개 파일 중 5개 hunk만 원인을 봤고(모두 HnS 고유 문맥), 나머지 35개 파일에 다른 upcoming 구조 의존이 숨어 있을 가능성은 배제하지 못했다.
6. #10004: SESSION_LOG의 "Porygon/PCM 변경 되돌림"이 이 PR인지는 추정이다. HnS 설정에서는 효과가 없다.
7. 용량 수치는 추출 PNG를 compresSmol로 쟀고 정렬 패딩은 뺐다. 실제 링크 증가량은 수십 바이트 다를 수 있다.
8. #10535 등 도감 설명: HnS 설명은 현재 전부 영문이다. 나중에 한글화할 계획이면 영문 교체의 실익이 없다.
9. #10387: za.json 수정은 all_learnables.json을 재생성할 때만 영향을 준다. HnS에서 재생성 절차를 쓰는지는 확인하지 않았다(재생성하면 HnS 커스텀 학습표가 덮이는 위험이 있다).

### Test Runner


1. **#7360 효과음 로직과 `Cmd_attackanimation`의 공유**
   - HnS에서 `UpdateEffectivenessResultFlagsForDoubleSpreadMoves()`는 효과음(1920행)과 공격 애니메이션 재생 판정(1553행, `!(moveResultFlags & MOVE_RESULT_NO_EFFECT)`)에 함께 쓰인다.
   - #7360 원형(`ret = gBattlerTarget 플래그`에서 시작)이나 1.17.0 최종형(0에서 OR)을 그대로 넣으면, 첫 대상이 빗나갔거나 모든 대상이 무효일 때 애니메이션 재생 여부가 지금과 달라질 수 있다. 1.17.0은 애니메이션 판정이 Canceler로 분리돼 있어 이 공유가 없다.
   - 효과음 경로 전용으로 분리해 이식하고, 더블 범위기 실기 확인(보통+별로, 빗나감+명중)을 권장한다.
2. **#10282의 선행 조건 판정**
   - HnS의 #10151 수동 이식에서 Champions 선택 제한 6종과 `B_PARALYSIS_CHANCE`가 "의도적 보류"인지 "누락"인지는 문서에 없다. SESSION_LOG 2026-09-15 목록에는 수면·하품, Rage Fist, Make It Rain, 교체 카운터, Supreme Overlord, Howl만 적혀 있다.
   - Battle Mechanics 그룹 판정과 교차 확인이 필요하다. `GEN_LATEST = GEN_CHAMPIONS`인 HnS에서는 upstream 대비 마비 확률(75% 대 87.5%)과 기술 선택 가능성이 다르다.
3. **#10214 무지개 블록의 `gBattlerAttacker` 무조건 대입**
   - HnS 1037행은 무지개가 없어도 턴마다 `gBattlerAttacker`를 편 대표 배틀러로 덮어쓴다(upstream은 #10214에서 if 안으로 옮겼다). 이후 턴 종료 이벤트가 이 값에 의존하는 경우는 코드상 찾지 못했다.
   - 다만 전체 턴 종료 경로를 전수 검증하지는 않았다. 이 덮어쓰기 덕분에 불바다 문구가 올바르므로, 옮길 경우 불바다 블록에도 대입을 추가해야 한다(#10214 형태).
4. **#10665 제목 불일치**
   - 입력 제목은 "Clear callbacks between tests"이고, 커밋 `d999ec8db6` 제목은 "Initialize battle type in Trainer Party Pool test (#10665)"다. 내용(콜백 재초기화와 `gBattleTypeFlags` 초기화)은 두 제목에 모두 들어맞는다. 같은 PR로 판단했다.
5. **HnS CI 실제 동작 여부**
   - `origin`(gorunit1/pokehns-expansion-kor)에서 GitHub Actions가 켜져 있는지는 로컬에서 확인할 수 없다. 켜져 있다면 `master` push나 PR에서 `make check`가 fake_rtc 문제로 실패할 것으로 예상된다(실행하지 않음).


## 작업 파일

- 하위 결과(원본): `g4work/ai_result.md`, `g4work/ai_conflicts.md`, `g4work/tests_result.md`, `g4work/pokemon_result.md`(같은 inv 디렉터리)
- 보조 스크립트: `g4work/hunkcheck.py`(+/− 줄 존재 대조), `g4work/applychk.sh`(소스 경로 한정 apply --check), `g4work/merge2.py`
