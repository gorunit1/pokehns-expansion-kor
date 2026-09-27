# g6_overworld_refactors — 1.17.0 전체 엔진 동기화 1단계 재판정·이식 계획

- 기준: 스냅샷 `e95679c545`(묶음 A 35건, `-ffunction-sections` 적용, #10529 후속 포함), upstream `expansion/1.17.0`(`e8bd1cd7b0`), merge-base `3efb836f72`.
- 대상: `g6_overworld_refactors.tsv` 112건(REFACTORS 40건 전부 + 1.15.x~1.17.0 Overworld 72건). 결과 행은 `g6_overworld_refactors_plan.tsv`(112행, 12열).
- 방법(읽기 전용):
  1. PR별 upstream diff를 추출하고 파일 단위 `git apply --check --include`로 실패 파일을 찾았다. 실패 원인은 "HnS 수정 파일(merge-base 대비 numstat)"과 "upstream 중간 커밋(merge-base~PR 부모 사이)"으로 나눠 기록했다.
  2. 체인 PR(팔레트 페이드, MapHeader)은 scratchpad 사본에 `patch`를 순차로 적용해 실제로 연속 적용되는지 확인했다(저장소는 건드리지 않음).
  3. 의존 관계는 pending 594건 전체를 upstream `order`로 정렬하고, 각 PR이 처음 추가한 식별자 중 HnS에 없는 것을 그 PR의 "신규 심볼"로 잡았다. 뒤 PR의 문맥·삭제 줄(사전 이미지)이나 추가 줄에 그 심볼이 나오면 후행으로 기록했다. 시그니처만 바꾸는 PR(#9859, #9510, #9335)은 패턴 검색으로 보완했다.
  4. 한글 겹침은 `BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 나온 STRINGID·스크립트·함수와 HnS가 추가한 STRINGID·배틀 스크립트 라벨(123개)을 "HnS 사용자 지정 심볼"로 잡았다. 그리고 각 PR의 삭제·문맥 줄에 나오는지 셌다.
- 빌드·테스트·실기 검증은 하지 않았다.

## 1. 판정별 개수 (112)

| 판정 | 개수 | PR |
| --- | ---: | --- |
| 이식 | 104 | 나머지 전부(REFACTORS 38건 포함) |
| 동등 | 7 | #9269, #10163, #10195, #10230, #10313, #10417, #10430 |
| 외부결정 | 1 | #9920 |
| 무관 | 0 | 문서·CI·마이그레이션 전용 PR이 이 그룹에는 없다 |

어제 판정에서 달라진 점은 다음과 같다.

- 어제 "HNS와 무관" 18건은 모두 `이식`이다. FRLG 전용, config로 꺼진 기능, 리팩터 회귀 수정 등으로 이유는 제각각이지만, 모두 게임 코드이거나 빌드에 영향을 준다.
- 어제 "충돌" 15건 중 14건은 `이식`(적응 방법 기재)이다. #9920만 `외부결정`이다.
- 어제 "이미 적용" #9878은 `이식`(잔여분)으로 바꿨다. `src/daycare.c`의 알 기술 전수 로직이 1.17.0과 다르고, 테스트 1209줄이 빠져 있다.
- 어제 "부분 적용" #9850, #10089, #10121은 잔여분만 `이식`한다.
- 크기 분포: S 58 / M 26 / L 19 / XL 9.
- kind 분포: refactor 52 / fix 34 / feature 24 / gfx 1 / test 1(#9751).
- `korean_touch=Y`는 18건이다: #9176 #9446 #9249 #9610 #9494 #9655 #9674 #9680 #9006 #9461 #9717 #9730 #9918 #9518 #10311 #10362 #10426 #10521.
- `save_impact=Y`는 5건이다: #7305 #9920 #9877 #9927 #10383.

## 2. 이 그룹의 unit과 의존 사슬

여러 PR을 묶는 unit은 아래와 같다. 나머지는 단독 unit(`U-<pr>`)이다.

| unit | 구성(upstream 순) | 필수 선행 |
| --- | --- | --- |
| U-fade-9407 | #9407 → #9549 → #9707 → #10573 | 없음. scratch 사본에서 4개 순차 적용을 확인했다 |
| U-relearner-9006 | #9006 → #9693, #9774, #9903, #10445, #10117(괄호 수정) → #10223 | 없음. #7573은 #9006·#8943 뒤 |
| U-mapheader-9461 | #9461 → #7975 → #9080 → #10159 → #10167 → #10176 | 없음. `tools/mapjson/mapjson.cpp`의 HnS `hns` 분기는 수동으로 반영 |
| U-owe-8434 | #8434 → #9910 → #9968 → #10020 → #10066 → #9966 → #10076 → #10096 | #7305(문맥), #9879·#9537(g5) |
| U-dailyseed-9920 | #9920(외부결정) → #9877 → #10383, #9955, #10012 | #10050(g5) |
| U-randommon-9896 | #9896 → #9970 → #10320, #10408, #9969 | #8434, #8678(g5), #9879(g5), #9927(#9969) |
| U-outbreak-9927 | #9927 → #10089 | #8434, #9920, #9890·#9713·#10051(g5), #8943(g3) |
| U-trainerbattle-8678 | (#8678 g5) → #10233, #10339, #10462 | #8678 |
| U-coins-9335 | #9335 → #10407 | 없음 |
| U-fieldmove-9819 | #9819 → #10548 | 없음 |
| U-pokedex-9518 | #9518 → #10172 | #9881(g5), #10116(g4) |
| U-namebox-9905 | #9905 → #10540 | #9881(문맥) |
| U-9557 | #9557 → #10080 | 없음 |
| U-anim-8497 | #8497 (+ g3 #9595) | #9142·#9473(g3) |
| U-12v12-8943 | (#8943 g3) → #9751(test) | #8943 |

대형 배틀 리팩터 unit은 3절의 권장 순서를 따른다.

## 3. 전체 리팩터 의존 그래프 (REFACTORS 40건 + 필수 5건)

- "바꾸는 API·심볼"은 신규 심볼과 삭제 심볼 중 대표만 적었다.
- "다른 그룹 후행"은 그 API를 문맥으로 가지거나 새로 쓰는 pending PR이다. 그룹 번호는 새 입력 파일 기준이다.
- 자동 추출은 심볼 1개 이상이면 기록하므로 일부 약한 의존이 섞여 있다. #9859와 #9510은 시그니처 패턴 검색 결과다.

| order | 리팩터 | 바꾸는 API·심볼 | 선행(upstream PR) | 다른 그룹 후행 PR |
| ---: | --- | --- | --- | --- |
| ~0 | #9881 INCGFX(g5) | `INCGFX_U8/U16/U32`, preproc·scaninc의 INCGFX, `graphics_file_rules.mk` 삭제(HnS 한글 폰트 규칙 81줄 포함) | 없음(merge-base 바로 다음 커밋) | g3 #10129 #10225 / g4 #9945 #9974 #10116 #10141 #10206 #10208 #10252 #10270 #10346 #10414 #10561 / g5 #9986 #10036 #10070 #10156 #10184 #10334 #10335 #10555 / g6 #9147 #9518 #10281 |
| 108 | #9176 Fling | `MOVE_EFFECT_FLING`, `FLUNG_ITEM_*`, `BattleScript_RemoveItem`, `CanFling(atk, ability)`. 삭제 `BS_TryFlingHoldEffect` | — | g2 #10309 #10325 / g3 #9514 #9939 #10593 / g5 #9466 #10181 |
| 122 | #9446 Synchronize | `trysynchronize`, `SynchronizeState`, `GetMoveEffectFromStatus`. 삭제 `movevaluescleanup`·`setmultihit`·`decrementmultihit`(UNUSED_31/32), `MOVEEND_SYNCHRONIZE_*`, `ABILITYEFFECT_(ATK_)SYNCHRONIZE` | #9176, #9417(g3) | g1 #9532 #10226 / g2 #10488 / g3 #9916 #9928 #10471 #10593 / g5 #10070 #10181 |
| 130 | #9249 Sky Drop/난동 | `VOLATILE_SKY_DROP_TARGET`, `MOVEEND_RAMPAGE`·`CONFUSION_AFTER_SKY_DROP`, `B_RAMPAGE_CONFUSION`, `CancelMultiTurnMoves(b)`, `CanBeConfused(a,e)`, `FaintClearSetData(void)`. 삭제 `skyDropTargets`, `SkyDropState` | #9446, #9358(g4) | g1 #9532 #9784 #9988 #10231 #10288 #10295 / g2 #10317 #10354 #10648 / g3 #8893 #9657 #9928 #10151 #10471 #10630 |
| 133 | #8497 loadspritegfx 제거 | `TryLoadSpriteAssets`, 애니 스프라이트 태그 표, `unloadspritepal`. 삭제 `loadspritegfx` | #9142, #9473(g3) | g3 #9595 #10589(+이후 신규 애니 전부) / g5 #10656 #10721 |
| 136 | #9510 날씨 판정 | `GetWeather()` 공개, `IsBattlerWeatherAffected(holdEffect, weather, flags)` | — | g1 #10079 / g2 #10193 #10542 / g3 #9657 #9735 #10145 #10630 / g4 #9710 |
| 158 | #7305 gBerries | `gBerries`, `enum BerryId`/`BERRY_ID_*`, `FOREACH_BERRY`, `setberrytree(tree, berryId, stage)`, `giverandomberry`. 삭제 `ITEM_TO_BERRY`, `gNaturalGiftTable` | — | g3 #9735 / g5 #10181 / g6 #8434 |
| 164 | #9610 Fling·폴터가이스트 메시지 | `BattleScript_FlingMessage`/`PoltergeistMessage`, `MOVE_EFFECT_ITEM_MESSAGE` | #9176 | g3 #9939 #10593 |
| 165 | #9269 특수 트레이너 ID | (HnS 동등 구현 보유) | — | — |
| 168 | #9532 Bide(g1) | `BattleScript_SetUpBide`, `CancelerBide`. 삭제 `setbide`·`copybidedmg`(UNUSED_33) | #9446, #9249 | g3 #9657 #9916 #10057 / g6 #9730 #10220 |
| 169 | #9494 대기열 교체(MoveEnd) | `enum QueuedSwitch`, `MOVEEND_SPRAY_LEPPA_BLUNDER`·`SEND_OUT_REPLACEMENTS`, `BattleScript_QueuedSwitch*`, `redCardSwitched` | #9176, #9417, #9249 | g1 #9784 #9864 #9988 #10161 #10231 #10288 / g2 #10217 #10600 #10682 / g3 #8943 #9008 #9786 #9832 #9916 #10338 / g4 #9709 #9757 |
| 204 | #9655 배틀 메시지 | `gCureStatusStringIds`, `gPartyCureStatusStringIds`, `gRemoveHazardsStringIds`, `gHurtByStringIds`, `B_MSG_WEATHER_END_*`, `BattleScript_RemoveHazards`, 멘탈허브 스크립트, 신규 STRINGID 15개. 삭제 `gSpinHazardsStringIds`, `STRINGID_ATTACKMISSED` | #9514(g3) | g1 #9856 #10226 / g2 #10268 #10488 #10608 / g3 #9008 #9168 #9714 #9777 #9916 #10151 #10471 #10630 |
| 205 | #9475 트레이너 사진 | `enum TrainerPicID` 통합 표. 삭제 `gTrainerBacksprites`, `TRAINER_BACK_PIC_*` | — | g3 #8943 / g5 #9788 #10179 |
| 206 | #9674 매직코트·매직미러·가로챈다 | `magicCoatPending`/`magicBouncePending`, `RestoreAttacker/Target`, `CANCELER_SNATCH`, `MOVEEND_BOUNCED_MOVE` | #9176, #9657(g3), #9655 | g1 #9784 #9957 / g2 #10386 #10571 |
| 207 | #9575 AI 예측 통합 | `ComputeAiBattlerDecisions`. 삭제 `CanAiPredictMove` | #9596(g4) | (g6 #9847만) |
| 211 | #9680 턴 종료 BattleScriptCall | `endturnevents`, `ENDTURN_FAINTED_MON_ACTIONS`·`ARENA_TURN_END`, `*Ret` 스크립트. 삭제 턴 종료 `*End2`, `BattleScript_ToxicOrb/FlameOrb`, `RainDishActivates`, `WhiteHerbEnd2` | #9249, #9610, #9494, #9616(g3), #9655 | g3 #9777 #9939 #10471 |
| 212 | #9006 relearner | chooseboxmon 기반 relearner, `Special_HasMoveToRelearn`, `HandleMoveRelearnerInput`. 삭제 `HasMovesToRelearn` | — | g3 #8943 / g4 #10368 / g5 #10051 #10179 |
| 216 | #8943 12v12(g3) | `gParties[MAX_BATTLE_TRAINERS][PARTY_SIZE]`, `gPartiesCount`, `AreMultiPartiesFullTeams`, `GetBattlerTrainerFromParty`. 삭제 `gPlayerParty/gEnemyParty` 실체(매크로화) | #9494, #9657, #9475, #9006(문맥), #9116·#9451(g4) | g1 10건 / g2 13건 / g3 7건 / g4 10건 / g5 16건 (합 56건, g6은 15건) |
| 222 | #8930 지역 도감 자동화 | `FOREACH_SPECIES_IN_HOENN/KANTO_DEX_ORDER`. 삭제 수동 `HOENN_DEX_*` enum | — | g5 #9761 #9780 |
| 232 | #9717 대기열 교체(EndTurn) | `ENDTURN_SEND_OUT_REPLACEMENTS_1~5`, `IsBattlerPresent`, `BattleScript_EjectItemActivates` | #9494, #9680 | g1 #9784 #10161 #10169 / g2 #10433 / g3 #9777 #10630 |
| 254 | #9751 테스트 러너 | `Test_GetBattlerTrainer`, `Test_BattlersShareParty` | #8943 | g1 #10039 / g5 #10051 #10659 |
| 259 | #9847 SwitchAiContext | `struct SwitchAiContext`, `ShouldSwitchIfLoses1v1`. 삭제 `FindMonWithFlagsAndSuperEffective` | #8472 #9124 #9451 #9460 #9462 #9551 #9568(g4), #9575, #9657, #8943 | g2 #10212 #10542 #10616 / g3 #10256 / g4 #9857 #10277 #10626 / g5 #9865 |
| 261 | #9859 MoveEnd CalcValue | MoveEnd 핸들러가 `(struct BattleCalcValues *cv)`를 받도록 시그니처 변경 | #9176 #9417 #9249 #9494 #9657 #9674 #9784(g1) #9408(g3) | g2 #10217 #10309 #10386 #10431 #10443 #10682 / g3 #9777 |
| 270 | #9730 능력치 변화 | `SetStatChange`, `trybattlerstatchange`, `MOVE_EFFECT_STAT_PLUS/MINUS`, `EFFECT_STAT_CHANGE*`, `STRINGID_STATROSE/STATFELL/STATWASMAXEDOUT` 등 신규 234개. 삭제 `statbuffchange`·`setstatchanger`, `MOVE_EFFECT_*_MINUS_1`, `STRINGID_ATTACKERSSTATROSE` 외 9개 | 23건(#9176 #9446 #9249 #9494 #9610 #9655 #9674 #9680 #9717 #8943 #9532 #9514 #9657 #9168 #9786 #9417 #9858 #9349 #9107 #9116 #9451 #9529 #9568) | g1 16 / g2 17 / g3 23 / g4 7 / g5 9 (합 72건. 목록은 5절) |
| 280 | #9918 맹세 기술 | `PledgeCombo`/`pledgeState`, `CANCELER_PLEDGE_ATTACK`, `MOVE_EFFECT_SEA_OF_FIRE/RAINBOW/SWAMP`. 삭제 `setpledge` | #9176 #9446 #9494 #9514 #9657 #9674 #9730 | g2 #10217 #10309 #10542 / g3 #10384 #10593 / g5 #10162 #10181 |
| 281 | #9939 명중 판정 캔슬러(g3) | `CANCELER_ACCURACY_CHECK`, `BattleScript_BattlerAvoidedAttack`, `STRINGID_BATTLERAVOIDEDATTACK`. 삭제 `accuracycheck` 명령, `gMissStringIds`, `STRINGID_PKMNEVADEDATTACK/AVOIDEDATTACK` | #9176 #9358 #9514 #9610 #9657 #9680 #9730 #9858 | g1 #9983 #10144 / g2 #10443 #10467 #10542 #10566 #10588 #10606 #10687 / g3 #9777 #10576 #10595 / g5 #10555 |
| 610 | #9927 대량발생 | `mass_outbreak.c/h`, `MassOutbreakIndex`, `checkhasactiveoutbreak`. TVShow 필드 의미 변경 | #8434, #9920, #9890·#9713·#10051(g5), #8943 | g5 #10113 #10142 #10178 |
| 620 | #9211 트레이너 슬라이드 | `TRAINER_SLIDE_*` 26종(ATTACKER/DEFENDER/SELF/OPPONENT 명명), `ShouldRunTrainerSlide*` | #9680, #9717, #8943 | g2 #10624 / g3 #9777 |
| 624 | #9518 도감 통합 | 공통 도감 코드, `*_HGSS` 훅, `NationalPokedexNumToSpeciesForm`. 삭제 `CB2_OpenPokedexPlusHGSS`, `gText_Dex*` | #9881(g5), #10116(g4) | g5 #10335 |
| 627 | #8678 동적 trainerbattle(g5) | `EventSnippet_*`, `PUSH_IF_SET`, `trainerbattle` 매크로 인자 재정의. 삭제 `TRAINER_BATTLE_*` 타입 | #8434 | g5 #10168 #10178 #10179 #10293 / g6 #9970 #10233 #10339 #10462 |
| 629 | #9878 알 재작업 | 부모 상속 config, `TransferEggMovesFromBoxmonToBoxmon` | #8943, #10051, #9518, #9460·#9462(g4) | g5 #10181 |
| 639 | #10163 혼란 열매 | `HOLD_EFFECT_CONFUSE_FLAVOR`(HnS 동등) | #9124(g4) | — |
| 647 | #9147 문 크기 | `DoorSize`, `sDoorSizeInfo` | #9881 | — |
| 667 | #10220 공격 전 효과·대미지 캔슬러 | `CancelerPreAttackMoveEffect`, `CancelerDamageCalc`, `CANCELER_RESULT_END`, `IsAsleepOrComatose`. 삭제 `damagecalc`·`setpreattackadditionaleffect`(UNUSED_37/38) | 18건(#9249 #9610 #9532 #9657 #9674 #9717 #9730 #9918 #9939 #9211 #9786 #9916 #9983 #10057 #10186 #9008 #9858 #9977) | g2 #10443 #10566 / g3 #10595 / g5 #10404 |
| 676 | #9440 트레이너 파티 생성 | `trainer_util.c`, `TrainerGenerator`, `GenerateMonFromTrainerMon` | #8943, #10051·#9562(g5), #10059(g1) | g2 #10463 / g5 #10523 |
| 680 | #9970 포켓몬 생성 정리 | `PokemonTemplate`, `CreateMonFromTemplate`, `Resolve*` | #9896, #8434, #8678, #9879·#10051(g5), #9135(g4) | (g6만) |
| 682 | #10299 무작위 추가 효과 | `MOVE_EFFECT_RANDOM_FROM_LIST`. 삭제 `MOVE_EFFECT_TRI_ATTACK/DIRE_CLAW` | #9730, #9918, #9514·#10151(g3) | g3 #10593 |
| 688 | #10311 end2 제거 | `end2` 삭제(UNUSED_39), `*SlideMsgEnd` | #9674 #9680 #10220 #9008 #9777 #10151 #10057 #9514 | g3 #10595 / g5 #10404 |
| 695 | #10300 동적 분류·선물 | `dynamicMoveCategory`, `BattleScript_HealTarget`, `DAMAGE_CATEGORY_NONE`. 삭제 `BattleScript_PresentHealTarget` | #9176 #9657 #9918 #10220 #9784 #9983 #10151 | g2 #10488 / g3 #10416 / g4 #8647 #10277 |
| 701 | #10330 TargetFailure | `enum TargetFailure`, `IsTargetUnaffectedByMoveEffect` | #9730 #10220 #9939 #9211 #9657 #9674 #9358 #10144 | g2 #10467 #10600 |
| 706 | #10326 지형 | `battleTerrain`/`gBattleTerrainInfo`, `B_TERRAIN_*`, `jumpifterrain`, `EFFECT_TERRAIN`. 삭제 `STATUS_FIELD_*_TERRAIN` | 15건(#9730 #9680 #9446 #9124 #9847 #10220 #10311 #9777 #9928 #10151 #10057 #9514 #9168 #9657 #10155) | g2 #10440 / g3 #10454 #10593 #10595 #10630 / g4 #10453 #10610 / g5 #10404 |
| 708 | #10362 애니·HP 갱신 캔슬러 | `CANCELER_MOVE_ANIMATION/HIT_ANIMATION/HEALTH_BAR_UPDATE`. `healthbarupdate`·`datahpupdate` 인자 제거 | 16건(#10220 #9730 #9610 #9655 #9777 #9939 #10300 #10330 #9494 #9176 #9657 #9168 #9858 #7360 #10374 #9677) | g2 #10431 / g3 #10595 |
| 716 | #10426 흡수·SetMoveEffect | `MOVE_EFFECT_ABSORB`, `SetMoveEffect` 새 시그니처(UNUSED_40). 삭제 `seteffectsecondary` | #9176 #9680 #9717 #9730 #9918 #10220 #10299 #10311 #9514 #10151 | g2 #10488 / g3 #10471 #10593 #10595 |
| 700 | #9335 코인·머니 명령 | 스크립트 명령 인자 제거(바이트코드 변경), `ShowCoinsWindow` 좌표 +1 | — | 없음(g6 #10407, g5 #10061 마이그레이션 문서) |
| 744 | #10548 비전기술 해금 | `FieldMoveUnlock` 표, `unlockType`, `hideIfLocked` | #9819 | — |
| 750 | #10121 디버그 메뉴 | `DebugSelection*` 공통 구조 | #9927 #9969 #9896 #8434 #9970 #10408 #9890 #9879 #8943 | — |

### 3-1. 권장 이식 순서 (upstream `order` 기준)

opcode 빈 번호(UNUSED_31/32 #9446, 33 #9532, 34 #9730, 35 #9916, 36 #10057, 37/38 #10220, 39 #10311, 40 #10426)를 upstream과 맞추려면 배틀 체인은 반드시 order 순으로 적용한다. 괄호 안 PR은 다른 그룹 소속으로, 같은 순서 안에서 먼저 넣어야 하는 것이다.

**0단계 — 빌드 기반**
1. (#9881 g5, INCGFX): merge-base 바로 다음 커밋이다.
   - HnS `graphics_file_rules.mk`의 한글 폰트 규칙(275~322행, 81줄)을 INCGFX나 잔존 규칙 파일로 보존해야 한다.
   - `spritesheet_rules.mk`(HnS +5474)는 HnS OW 그림 규칙을 전부 옮겨야 한다.
   - 이후 그래픽 PR 20여 건(g3·g4·g5, g6 #9147 #9518 #10281과 #9905 문맥)의 적용성이 여기서 결정된다.
2. (#9142 → #9473 g3) → #8497 → (#9595 g3): 애니메이션 기반.

**1단계 — 1.16 배틀 기반(order 102~211)**

3. (#9116 #9358 g4) → #9176 → (#9417 g3) → #9446 → #9249 → #9510 → (#9514 g3) → (#9568 g4) → #9610 → (#9532 g1) → #9494 → (#9657 g3) → (#9616 g3) → #9655 → #9674 → (#9596 g4) → #9575 → #9680

**2단계 — 파티·기준선(order 205~281)**

4. #9475 → #9006 → (#8943 g3)
   - #9475·#9006은 #8943의 문맥 선행이다(`TRAINER_PIC_*`, `HasMoveToRelearn`).
5. #9717 → #9751
6. (#8472 #9124 #9451 #9460 #9462 #9551 g4) → #9847
7. (#9784 g1, #9408 g3) → #9859
8. (#9168 #9786 g3) → #9730 → #9918 → (#9939 g3)
   - #9730 뒤에 #9928·#10057·#9916(g3) 후속 정리를 이어서 넣는다.

**3단계 — 1.17 캔슬러 체인(order 620~716)**

9. #9211 → #10220 → #10299 → #10311 → #10300 → #10330 → #10326 → #10362 → #10426
   - 사이에 #9008 #9777 #10151(g3), #9983 #10144 #10186(g1)이 선행으로 끼어든다.

**필드 트랙** — 배틀 체인과 독립이다. 각 트랙 안에서는 order 순으로 진행한다.

- #7305(158) → #8434(262, OWE) → OWE 후속 7건
- #9006 relearner 체인(3절 unit)
- #8930(222) → #9518(624, #9881 뒤) → #10172
- #9461(231) → #7975 → #9080 → #10159 → #10167 → #10176
- #9920(275, 외부결정) → #9877 → #10383 / #9955 / #10012
- #9896(285) → (#8678 g5, 627) → #9970(680) → #10320 / #10408 / #9969
- #9927(610): #8434·#9920·(#9890 #9713 #10051 g5)·#8943 뒤
- (#8678 g5) → #10233 / #10339 / #10462
  - #10233의 `event.inc` 오타 1줄은 먼저 넣어도 된다.
- #9335(700) → #10407
- #9819(251) → #10548(744)
- #9147(647): #9881 뒤
- #9440(676): #8943·#10051·#10059 뒤
- 마지막: #10121(750). 디버그 메뉴는 모든 선행 뒤에 둔다.

## 4. 필수 11건 평가: HnS 적응 난이도와 한글 메시지 겹침

- 지표 정의:
  - 실패 파일: 파일 단위 `--check` 실패 수 / 그중 HnS가 수정한 파일 수.
  - 한글 겹침: HnS 사용자 지정 심볼 등장 수, `battle_message.c`·`battle_scripts_1.s` 변경 줄 수, 신규·삭제 STRINGID 수, 테스트의 영문 `MESSAGE(...)` 줄 수.

| PR | 규모 | 실패 파일 | 다른 그룹 후행 | 적응 난이도 | 한글 메시지 겹침 |
| --- | --- | --- | --- | --- | --- |
| #9730 Stat Change | 176파일 +5762/-6809 | 34 / 21 | 72(+g6 7) | **최상**: 선행 23건. `battle_scripts_1.s` 2462줄. HnS 능력치 문구·오로라베일·특성/도구 상승 보완·하얀허브/미러허브 팝업이 모두 이 경로에 있다 | **상**: 사용자 지정 심볼 15개(IceBodyHeal·PoisonHeal·SolarPower·EffectPurify·HealerActivates·StickyBarbTransfer·STAYEDAWAKEUSING 등). `battle_message.c` 53줄. STRINGID 삭제 10/신규 5개(STATROSE/FELL/MAXEDOUT/ITDOESNTAFFECTSCR은 기존 한글 본문 이전, FLOWERVEILPROTECTEDTARGET만 새 문장). 테스트 MESSAGE 64줄 |
| #8943 12v12(g3) | 168파일 +3709/-3192 | 54 / 44 | 56(+g6 15) | **최상**: `gPlayerParty/gEnemyParty`→`gParties`가 HnS 최다 수정 파일(pokemon.c +3000, daycare.c +945, scrcmd.c +876, debug.c +616, storage +495, roamer +490, battle_setup +471 …)을 전부 관통한다. EWRAM 약 +1.2KB(현재 여유 약 13KB) | **중하**: 링크·파트너 "내보냈다" 문구 4개가 `{B_OPPONENT_MON1_NAME}`→`{B_BUFF1}`로 바뀐다(한글 본문 유지, 토큰만). 상태 문구는 문맥으로만 6곳 등장 |
| #9655 Battle Messages | 149파일 +2066/-986 | 12 / 9 | 13(+g6 4) | **상**: 파일 충돌은 적다. 대신 HnS 최신화와 같은 영역(상태 회복·장판·방벽·멘탈허브·생명의구슬)을 upstream이 다른 형태로 짠다. 방벽 순차 해제·장판별 DISAPPEAREDFROMTEAM은 HnS가 이미 같은 방향으로 구현했다 | **최상**: 사용자 지정 심볼 30개. `battle_message.c` 224줄 + `battle_scripts_1.s` 175줄. 신규 STRINGID 15개(공식 한글 확인 필요). 테스트 MESSAGE 1316줄(최대). HnS가 고치지 않은 경로의 upstream 문구 선택 변경(날씨 특성 시작, 파스텔베일, 빗나감)도 출력 변화로 기록해야 한다 |
| #9939 명중 캔슬러(g3) | 16파일 +299/-231 | 11 / 11 | 13(+g6 4) | **상**: `accuracycheck`가 캔슬러로 이동하므로 HnS 전 기술 스크립트의 accuracycheck 줄에 영향이 있다. #9730 뒤에만 가능 | **중**: `battle_message.c` 15줄, `battle_scripts_1.s` 119줄. `gMissStringIds`가 삭제되고 STRINGID_PKMNEVADEDATTACK/AVOIDEDATTACK이 BATTLERAVOIDEDATTACK으로 통합된다(새 한글 문장 1개). 보고서 6절의 #10144·#9777 상성 메시지 충돌과 한 묶음 |
| #9494 대기열 교체 | 23파일 +336/-145 | 8 / 6 | 17(+g6 6) | **중**: 교체 절차 재배치. 탈출버튼·레드카드 HnS 아이템 팝업 줄만 보존하면 된다 | **하**: 사용자 지정 심볼 0개. `battle_scripts_1.s` 68줄(교체 순서). 문구 불변, 교체 화면 타이밍만 변한다. 테스트 MESSAGE 2줄 |
| #9859 CalcValue MoveEnd | 2파일 ±455 | 1 / 1 | 시그니처 후행 7(g2 6, g3 1) | **중**: `battle_move_resolution.c` 기계적 치환. 단 선행 8건(#9784 등)이 먼저 있어야 문맥이 맞는다 | **하**: `StealTargetItem` 문맥 1곳뿐. 문자열 변경 없음 |
| #9446 Synchronize | 11파일 +183/-180 | 5 / 3 | 9(+g6 4) | **중하**: opcode 3개 삭제와 스크립트 1곳 추가 | **중**: 싱크로·상태 치료 열매 발동 시점이 앞당겨진다(출력 순서 변화, upstream 유래). HnS `B_MSG_STATUSED_BY_ABILITY` 매핑과 리샘열매 상태별 반복 출력은 문구 그대로 유지 |
| #9532 Bide(g1) | 9파일 +230/-93 | 4 / 3 | 3(+g6 2) | **하**: opcode 2개 삭제와 캔슬러 1단계. opcode 번호(UNUSED_33)는 #9446 뒤에 맞춰야 한다 | **하**: 사용자 지정 심볼 0개. `battle_scripts_1.s` 24줄. 테스트 MESSAGE 10줄 |
| #9881 INCGFX(g5) | 141파일 +24574/-29772 | 36 / 36 | 21(+g6 3) | **상**: HnS 그래픽 규칙(`spritesheet_rules.mk` +5474, `tilesets/graphics.h` +2218, `option_menu.c` +1561, `naming_screen.c` +762, `text.c` +392 …)을 INCGFX 표기로 전환해야 한다. `migrate_incgfx.py`로 기계 변환이 가능하다 | **중(폰트)**: 배틀 문자열 겹침은 0이다. 대신 `graphics_file_rules.mk` 삭제로 HnS 한글 폰트 생성 규칙(font0/1/2/7/8_korean.latfont)이 사라지므로 반드시 따로 보존해야 한다 |
| #9335 코인·머니 명령 | 7파일 +84/-97 | 1 / 1 | 0(g6 #10407) | **하**: 마이그레이션 스크립트로 HnS `_hns` 스크립트 약 110곳을 변환한다. 코인 창 +1 이동은 `showcoinsbox` 좌표 -1로 상쇄 | **없음**: 문자열 변경 없음(스크립트 바이트코드만 변경) |
| #8678 동적 trainerbattle(g5) | 28파일 +737/-291 | 7 / 2 | 4(+g6 5) | **중**: 대부분 그대로 적용된다. 실패는 `battle_setup.c`(HnS +471, IS_HNS 조토 재대결 표 `gRematchTable`)와 `battle_pyramid.c`(+60)이다. HnS 맵은 `trainerbattle_*` 래퍼만 쓰므로(single 336·no_intro 127·rematch 40·double 14회) 스크립트 수정은 없다 | **없음**: 문자열 변경 없음. 트레이너 접근·재대결 흐름만 변경 |

요약:

- 한글 메시지 겹침은 #9655 > #9730 > #9939 > #9446 순으로 크다.
- 적응 난이도는 #9730 ≈ #8943 > #9655 ≈ #9881 ≈ #9939 > #9494 ≈ #9859 ≈ #8678 > #9446 > #9532 ≈ #9335 순이다.

## 5. #9730 후행 PR 목록 (심볼 문맥 기준, 다른 그룹)

- g1(16): #9957 #9972 #9983 #9988 #10032 #10034 #10074 #10077 #10079 #10088 #10091 #10185 #10216 #10263 #10272 #10295
- g2(17): #10268 #10350 #10354 #10431 #10488 #10542 #10556 #10566 #10569 #10588 #10600 #10631 #10661 #10671 #10687 #10690 #10707
- g3(23): #8893 #9008 #9777 #9916 #9928 #9939 #9989 #10057 #10145 #10151 #10256 #10257 #10265 #10416 #10428 #10454 #10459 #10471 #10576 #10593 #10595 #10630 #10666
- g4(7): #8647 #9857 #10124 #10277 #10285 #10381 #10464
- g5(9): #10058 #10070 #10127 #10162 #10178 #10179 #10210 #10274 #10404

## 6. 외부결정 상세

### #9920 Basic daily seed

- 파일·함수:
  - `include/global.h` `struct SaveBlock1`의 `/*0x9C2*/`.
  - `src/clock.c` `UpdateDailySeed`.
  - `Crc32B`를 `random.c`로 이동.
- HnS 현재 동작: upstream이 `dailySeed`를 두는 자리(`unused_9C2[6]` 중 4바이트)를 HnS는 `u32 saveVersionMagic`(`include/global.h:1228`, HnS 세이브 버전 시스템)으로 이미 쓴다. 남은 공간은 `unused_9C6[2]`뿐이다.
- 선택지:
  - **A(권장)**: `u32 dailySeed`를 `SaveBlock3` 끝(`include/global.h:360` `registeredItemHold` 뒤)에 추가한다.
    - `SaveBlock3`는 fakeRTC·challengeSettings 등 수십 바이트라 1624바이트 한도까지 여유가 크다(`src/save.c:82` assert).
    - 기존 세이브는 0으로 읽혀 첫 날짜 변경 때 갱신되므로 호환된다.
    - 세이브 버전은 올리지 않는다.
    - 후행 #9877·#9955·#10012·#10050(g5)·#9927은 이 위치를 읽도록 적응한다.
  - **B**: upstream 위치를 그대로 쓴다. 이 경우 `saveVersionMagic`을 옮기고 세이브 버전 판별과 마이그레이션을 새로 만들어야 한다. 기존 세이브 판별 로직이 바뀌는 위험이 있다.
  - **C**: 이식하지 않는다. 동적 날씨·복권/환상의섬 일일 seed·일일 이벤트 디버그·대량발생 일부가 함께 빠진다.

## 7. 한글 메시지와 겹치는 항목의 적응 방법

1. **토큰만 바뀌는 문자열**
   - 대상: #9610 FLUNG·POLTERGEIST, #9674 SNATCHED·BOUNCED, #9918 바다불/습지, #10426 흡수·씨뿌리기·해감액, #8943 링크/파트너 내보내기.
   - 한글 본문·조사 토큰·줄바꿈은 그대로 둔다. 엔진이 주체를 담는 변수를 바꾸기 때문에 `{B_ATK_*}/{B_DEF_*}`↔`{B_EFF_*}/{B_SCR_*}` 대상 토큰만 맞춘다.
   - 토큰을 유지하려면 기존 변수도 같은 값을 갖도록 엔진 쪽에서 설정하는 방법도 있다. g3의 #9514도 같은 원칙으로 처리했다.
2. **STRINGID 통합·삭제**
   - #9730: ATTACKERS/DEFENDERS/SCRIPTINGSTATROSE/FELL → STATROSE/STATFELL.
   - #9939: EVADED/AVOIDED → BATTLERAVOIDEDATTACK.
   - 삭제되는 ID의 HnS 한글 본문을 새 ID로 옮기고 주체 토큰만 교체한다. 한국어 어순용 `gText_StatSharply "크게 "`·`drastically`의 앞 배치는 upstream의 영어 접미 이동(#9655)을 따르지 않는다.
3. **HnS 사용자 지정 출력과 같은 영역을 upstream이 재구성하는 경우**
   - 대상: #9655, #9680, #10362, #9730.
   - upstream의 표·enum·스크립트 이름은 도입한다. 값·본문은 HnS의 현재 출력(`BATTLE_MESSAGE_OUTPUT_CHANGES.md` 표의 STRINGID와 스크립트 순서)으로 매핑한다.
   - upstream이 삭제하는 스크립트라도 HnS가 쓰면 return형으로만 바꿔 유지한다. 예: #9680의 `BattleScript_ToxicOrb/FlameOrb`, IceBody 무문구.
4. **HnS가 고치지 않은 경로의 upstream 문구 선택 변경**
   - 대상: #9655 날씨 특성 시작·파스텔베일·멘탈허브 도발, #9446 싱크로 시점, #9249 난동 혼란 시점, #9494 교체 화면 시점.
   - 엔진 동작으로 따라간다. 기존 한글 STRINGID를 재사용하고, 변화는 `BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 "upstream 유래"로 기록한다.
5. **새 문장이 필요한 것** — 공식 한국어 근거가 없으면 코드는 이식하고 문구는 미결 목록에 둔다.
   - #9655 신규 STRINGID 15개
   - #9730 `FLOWERVEILPROTECTEDTARGET`
   - #9939 `BATTLERAVOIDEDATTACK`
   - #9006 `gText_MoveRelearnerStop`·`TeachMoveConfirmUseTm`
   - #9461 셀라돈 백화점 특례: 영문 `CELADON DEPT.` 노출 금지
6. **표시·폰트**
   - #9461 맵 팝업은 HnS `FONT_NARROW`를 유지한다.
   - #10521 설명 폰트 자동 축소는 HnS `text.c`의 한글 좁은 글리프 폭(8~9px)이 있으므로 동작한다. 단 기존 한글 설명이 창 안에 맞는지 먼저 확인한다.
   - #9881은 한글 폰트 생성 규칙을 보존한다.

## 8. ROM·세이브 위험

- 기준: ROM 32,734,772 B(97.56%, 여유 약 820 KB), EWRAM 249,000 B(여유 약 13 KB), IWRAM 25,636 B. `-ffunction-sections` 적용 뒤 수치다.
- ROM 증가:
  - **#9211 약 +170 KB**: 트레이너 슬라이드 3차원 포인터 표(`src/trainer_slide.c:59`)가 커진다. 표를 희소 목록으로 바꾸면 현재 200 KB까지 절감할 수 있다.
  - #8434 OWE 약 +10 KB(추정, gc-sections 뒤 실측 필요)
  - #9927 약 +6 KB
  - #9896 약 +4 KB
  - #9425 약 +3.8 KB
  - #9080 약 +3 KB(#10159에서 회수)
  - 디버그 #9969·#10121 수 KB
- ROM 감소: #9518 약 -10 KB, #8497·#9730 각 약 -8 KB, #9765 -5 KB, #8930·#10096·#10159 각 약 -3 KB.
- 순증: 표 적응 없이 약 +160 KB, 희소 표 적응 시 약 -200 KB.
- EWRAM: #8943 약 +1.2 KB(`gParties[4][6]`), #8497 +32 B, #9425 +4 B, #8434 +1 B.
- 세이브 위험:
  - **#7305**: upstream `FOREACH_BERRY` 순서는 기존 `ITEM_TO_BERRY` 번호와 36~65번이 다르다(CHILAN·ROSELI 위치). HnS에서는 아이템 순서를 유지해야 기존 게임 내 세이브의 `berryTrees[].berry`가 보존된다.
  - **#9920**: 6절 참조.
  - **#9877/#10383**: HnS `WEATHER_LEAVES=23`과 저장되는 날씨 번호를 지켜야 한다. `WEATHER_DYNAMIC=24`로 두고, #10383 enum은 명시값으로 기존 20~23을 유지한다.
  - **#9927**: `TVShow.massOutbreak`의 `unused2`·`daysLeft` 의미가 바뀐다. 크기는 같다. 진행 중인 대량발생이 있는 기존 세이브는 실기로 확인해야 한다.
- 스크립트 바이트코드 변경:
  - #9335·#10407(머니/코인 명령)
  - #8678(trainerbattle 매크로)
  - #10311(end2)
  - 모두 세이브에는 영향이 없다. 하지만 스크립트 변환이 하나라도 빠지면 스크립트 읽기가 어긋난다.

## 9. 불확실 항목

1. **#9878**: 알 기술 전수 규칙이 다르다. 어느 쪽을 따를지 확인이 필요하다.
   - HnS(`src/daycare.c:183`): Gen9에서도 같은 종이면 미러허브 없이 전수된다. `include/config/pokemon.h:33` 주석과 일치한다.
   - upstream 1.17.0(`daycare.c:189`): Gen9에서 항상 미러허브가 필요하다.
   - 이 판정에서는 HnS 쪽 유지를 전제로 잔여분 `이식`으로 뒀다.
2. **#9819**:
   - `data/tilesets/secondary/indigo_plateau_hns`에 바위오르기 메타타일(`MB_ROCK_CLIMB`=239)이 있다. HnS `field_control_avatar.c:635`는 config와 무관하게 상호작용을 호출한다.
   - upstream대로 `IsFieldMoveUnlocked(FIELD_MOVE_ROCK_CLIMB)`를 붙이면 `OW_ROCK_CLIMB_FIELD_MOVE FALSE`인 HnS에서 이 상호작용이 막힌다.
   - 확인하지 않은 것: 실제 맵 배치, 그리고 `checkfieldmove FIELD_MOVE_ROCK_CLIMB`가 어떻게 조립되는지.
3. **#10117**: upstream 1.17.0 그대로면 요약 화면 콘테스트 페이지가 항상 숨겨진다. 괄호 수정으로 이식하면 1.17.0 "실제 동작"과 달라진다(버그 수정 쪽을 택함).
4. **#7305**: 나무열매 번호 순서 유지안이 upstream 테스트(BERRY_ID 순서 가정)나 후행 #10181과 충돌하지 않는지는 코드로만 추정했다.
5. **자동 의존 추출의 한계**:
   - 심볼 1개 일치도 후행으로 기록했으므로 약한 문맥 의존이 섞여 있다. 예: #9730 후행 72건 중 일부.
   - 시그니처만 바꾸는 PR은 패턴 검색으로 보완했지만 누락이 있을 수 있다.
   - #8943의 선행으로 적힌 #9475·#9006은 문맥 의존이다.
6. **ROM 수치**:
   - #9211은 링커 맵 비례 추정이다.
   - #8434·#9927·#9896은 소스 규모 추정이다. gc-sections 뒤 실측이 필요하다.
7. **#9461 셀라돈 백화점 특례**: `LAYOUT_CELADON_CITY_DEPARTMENT_STORE_*` 중 HnS에 있는 것은 ELEVATOR뿐으로 보인다. HnS 백화점 맵이 이 특례에 걸리는지 확인하지 않았다.
8. **#10557**: `text.c` `RunTextPrinters`의 RENDER_FINISH 콜백 추가가 HnS 한글 입력·특수 화면 프린터에 영향이 없는지는 실기 확인이 필요하다.
9. **scratchpad 공유 주의**: 병렬 세션이 같은 scratchpad를 쓰고 있어(다른 그룹이 `addrow.py`를 덮어씀) 이 그룹의 분석 파일은 `scratchpad/g6w/`로 분리했다. 산출물(`W/`)에는 영향이 없었다.
