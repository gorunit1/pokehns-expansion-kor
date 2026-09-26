# g1_battle_fixed_a — Battle General / Fixed (1.15.2 ~ 1.16.2) 인벤토리 판정

- 대상: 85개 PR (1.15.2 2, 1.15.3 26, 1.16.0 23, 1.16.1 6, 1.16.2 28)
- 기준: HnS 스냅샷 `<HnS 작업트리>/` (HEAD `0d89762071`), upstream `<upstream clone>/`
- 방법: PR별 upstream 커밋을 파일 단위로 `git apply --check` / `-R --check` 한 뒤(테스트 파일 분리), 실패하거나 의미 확인이 필요한 경우 HnS 함수 본문과 upstream diff를 직접 대조했다. 모든 조사는 읽기 전용이다.

## 0. 선행 확인: HnS는 upstream **master(1.15.x) 계열**이다

- `3efb836f72`(#9875)는 `expansion/1.15.2`·`1.15.3`의 조상이다. 반면 upcoming 전용 커밋인 #9532 Bide Refactor(03-24)와 #9730 Stat Change Refactor, #9494 queued switches, #8943 12v12, #9655 Refactor Battle Messages, #9939 Integrate accuracy check into canceler, #9446 Synchronize refactor, #9859 MoveEnd CalcValue는 **HnS와 1.15.3 어디에도 없다**(`git merge-base --is-ancestor`로 확인).
- 작업 지시서의 "upcoming 도중"이라는 설명과 달리, HnS는 1.15.2 개발 중인 master 브랜치를 기반으로 한다.
- 그래서 1.15.2/1.15.3 수정(28건)은 master에 들어간 커밋이라 HnS에 거의 그대로 적용된다(25건이 소스 기준 clean).
- 반대로 1.16.x 수정(57건)은 upcoming의 리팩터 결과물(`battle_stat_change.c`, `struct BattleCalcValues *cv`를 쓰는 MoveEnd/Canceler, `gParties[]`, `notOnField`/`queuedSwitch`)을 전제로 하는 경우가 많다. 이런 PR은 ① 리팩터가 만든 회귀를 고친 것이라 HnS와 무관하거나, ② 표기만 달라서(`cv->` 대신 `gBattlerAttacker`/`ctx->`) 수동으로 옮기면 되거나, ③ 실제 선행 리팩터가 필요한 경우로 나뉜다.

## 1. 요약

| 판정 | 개수 | PR |
| --- | ---: | --- |
| 이미 적용 | 7 | 9940, 9999(둘 다 HnS 원작자 cherry-pick), 9975, 9982, 10072, 10079, 10105 |
| 부분 적용 | 1 | 10132 |
| 미적용·안전 이식 가능 | 44 | 1.15.x 25건(9858, 9880, 9901, 9921, 9917, 9929, 9943, 9946, 9951, 9952, 9937, 9942, 9954, 9976, 10002, 10007, 10028, 10003, 10035, 10047, 10040, 10042, 10038, 10030, 10029) + 1.16.x 수동 적응 19건(10093, 10169, 10186, 10207, 10213, 10227, 10228, 10229, 10231, 10219, 10263, 10272, 10289, 10288, 10278, 10307, 10295, 10309, 10325) |
| 미적용·선행 필요 | 9 | 9532, 9784, 9957, 9983, 9988, 10091, 10161, 10185, 10226 |
| 충돌 | 4 | 9799, 10144, 10149, 10315 |
| HNS와 무관 | 20 | 9729, 9811, 9843, 9856, 9864, 9972, 10024, 10032, 10034, 10039, 10059, 10064, 10074, 10077, 10088, 10102, 10175, 10216, 10287, 10306 |

### 이식 추천 상위 (HnS 실제 버그, 작은 변경)

1. **#10093** 명령을 듣지 않을 때 무작위 기술이 원래 선택한 기술의 스크립트로 실행됨 — `CancelerObedience`의 `DISOBEYS_RANDOM_MOVE`에 `gBattlescriptCurrInstr = GetMoveBattleScript(gCalledMove);` 1줄 추가.
2. **#10213** 공중날기·뛰어오르기 중인 대상을 떨어뜨리기로 맞혀도 `multipleTurns`가 풀리지 않음, 프리폴 예외 없음 — `MoveEndMoveBlock`의 `EFFECT_SMACK_DOWN`.
3. **#10228** 동적으로 불꽃 타입이 된 기술(햇빛 속 웨더볼 등)이 얼음을 녹이지 못함 — `CanFireMoveThawTarget(move, moveType)`.
4. **#10207** 나이트메어 특성 팝업이 닫히지 않고 남음 — `BattleScript_BadDreamsIncrement`에 `setbyte sFIXED_ABILITY_POPUP, FALSE` 1줄.
5. **#10132 잔여분** 난기류에서 웨더볼 위력이 2배가 됨 — `CalcMoveBasePower`의 `EFFECT_WEATHER_BALL` 마스크를 `(B_WEATHER_ANY & ~B_WEATHER_STRONG_WINDS)`로 변경(1줄). 회복기 부분은 이미 반영됨.
6. **#9937** 연속기가 중간에 빗나간 뒤 이전 타격 상태가 남음(`Cmd_resultmessage` 2줄). clean apply.
7. **#10169** 포이즌힐·매직가드 상태에서 맹독 카운터가 오르지 않음 — `HandleEndTurnPoison`(HnS는 `IsBattlerAlive`를 쓰므로 수동 적응). 포이즌힐 텍스트를 뺀 HnS 스크립트는 건드리지 않는다.
8. **#9954** 위협이 막히거나 공격을 더 내릴 수 없을 때 번견이 발동함(`BS_JumpIfIntimidateAbilityPrevented`). clean apply. upcoming 쪽 #9957 대신 이 master 버전을 쓴다.
9. **#9880 → #9901** 재앙 특성 4종과 위액·화학변화가스 상호작용. clean apply. 고위험 파일(`battle_scripts_1.s`의 `BattleScript_NeutralizingGasExits`)을 건드리지만 문자열 변경은 없다.
10. **#10263** 더블배틀 AI가 필드 대상 기술(트릭룸 등)을 쓸 때, 파트너가 없으면 실패함 — `OpponentHandleChooseMove`의 대상 결정을 `SetFinalChosenTarget`로 공통화. 수동 적응 필요.

그 밖에 1.15.3 master 수정 25건은 거의 모두 clean apply되므로 **PR 순서대로 하나씩 이식하기 좋은 묶음**이다. 단 #9929는 메시지 출력을 바꾸고(아래 참고), #9858은 `battle.h` 1줄을 수동으로 넣어야 하며, #9917은 `cv->` 표기에 맞게 적응해야 한다.

### 충돌 목록 (사용자 결정 필요)

- **#9799** 교체·등장 메시지 선택 로직 재작성과 영어 문장 변경 — HnS의 한글 `sText_*`·`BufferStringBattle`와 충돌하고, 12v12 API(`BattleSideHasTwoTrainers`, `BattlersShareParty`)도 없다.
- **#10144** 상성 메시지 순서와 `STRINGID_ITDOESNTAFFECTTWOFOES` 삭제 — HnS의 Champions 상성 메시지(`Cmd_resultmessage`)와 한글 문자열을 제거·변경한다.
- **#10149** `STRINGID_PKMNSXMADEITINEFFECTIVE` 문장·용도 변경 — HnS는 이 ID를 스위트베일 수면 방지 문장("잠들지 않는다!")으로 재정의해 두었다.
- **#10315** 드래곤애로우의 타격 후 메시지 — `Cmd_resultmessage`의 상성 문장 선택과 "N번 맞았다!" 출력 조건을 바꾼다(HnS Champions 분기와 겹침).
- (잠재 충돌, 선행 필요) #9983은 연속기가 빗나갈 때 새 메시지 경로를 추가하고, #10226은 사이코시프트 스크립트의 치료 문구 인접 구역을 수정한다.

### 용량 주의

- 이 그룹에서 ROM 용량을 **크게** 늘리는 PR은 없다. 모두 수~수백 바이트 수준의 로직 수정이고, 테스트 파일은 ROM에 들어가지 않는다.
- EWRAM/heap 증가(미미)
  - #9858: `gBattleStruct`에 `u16 innardsOutHpLost[4]`(+8B, heap)
  - #10186: `u8 faintCounter[MAX_BATTLE_TRAINERS]`(+4B, heap)
  - #9784(선행 필요): `EWRAM_DATA gBattlersByRawSpeed[4]`(+4B, 정적 EWRAM)
  - #10289: volatile 비트 1개. `VOLATILE_UNUSED` 패딩을 재사용하면 구조체 크기 증가를 피할 수 있다.
  - #10047·#10231: `SpecialStatus` 패딩 비트(`padding2:2`)를 각각 1개씩 쓴다. **두 PR을 모두 넣으면 패딩 2비트가 모두 소진된다.** 이후 비트를 더 추가하면 구조체가 1바이트 늘어나고, 배열이라 4바이트가 증가한다.
- 상대적으로 큰 코드 변경: #9942(퓨처사이드 계산 재작성, 코드 순감 가능), #9532·#9988·#10161(선행 필요 대형).

## 2. 판정표

범례: 고위험파일 Y = 지시서의 고위험 배틀 파일(`battle_message.c`, `battle_scripts_1.s`, `battle_util.c`, `battle_script_commands.c`, `battle_hold_effects.c`, `battle_end_turn.c`, `include/battle*.h`, `battle_script.inc`, `test/battle/**`)을 포함한다는 뜻이다. "수동"은 hunk context나 `cv->`/`ctx->` 표기 차이 때문에 손으로 옮겨야 한다는 뜻이다.

| PR | 제목 | 판정 | 근거(파일·함수) | 고위험파일 | 용량 영향 | 의존·비고 |
| --- | --- | --- | --- | --- | --- | --- |
| 9858 | Innards Out 연속기 피해 기준·Core Enforcer | 미적용·안전 이식 가능 | `battle_script_commands.c`·`battle_util.c`는 clean. `battle.h`는 HnS의 `moveResultFlags`가 `u32`(Champions)라 context만 다르다(1줄 수동). | Y | 작음(+8B heap) | #10278(Core Enforcer TARGET_BOTH)과 함께 적용 권장 |
| 9880 | 재앙 특성 / 위액 상호작용 | 미적용·안전 이식 가능 | 소스 전 파일 clean. `RemoveRuinAbilityFlags` 신설, `BattleScript_NeutralizingGasExits`는 루프 변수(`gBattlerAttacker`→`gBattlerTarget`/`gEffectBattler`)만 바꾸고 문자열은 그대로. | Y | 작음 | — |
| 9901 | 재앙 특성끼리 비활성 무시 | 미적용·안전 이식 가능 | `CalcAttackStat`/`CalcDefenseStat` clean. 테스트만 #9880 이후 순서에 의존. | Y | 없음 | #9880 다음에 적용 |
| 9921 | 아로마베일이 운명의실 헤롱헤롱을 막지 못함 | 미적용·안전 이식 가능 | `Cmd_tryinfatuating` clean, `BattleScript_AromaVeilProtectsRet`이 HnS에 있음. | Y | 작음 | 기존 아로마베일 문구 재사용 |
| 9917 | 특성 전투기록 오기록 | 미적용·안전 이식 가능 | `battle_script_commands.c` clean. `CanMoveSkipAccuracyCalc`는 HnS가 `cv->abilities[]`를 써서 수동 적응(3줄). | Y | 없음 | 수동 |
| 9929 | 변화기 빗나감 메시지 교정 | 미적용·안전 이식 가능 | clean. `AccuracyCheck`의 `failInstr==BattleScript_ButItFailed`를 `BattleScript_TargetAvoidsAttackEnd`로 바꾼다. | Y | 작음 | **메시지 출력 변경**: 실패 문구 → `STRINGID_PKMNAVOIDEDATTACK`("…에게는 맞지 않았다!"). 이식 시 `BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 기록 권장 |
| 9943 | 대타출동을 맞아도 풍선이 터지지 않음 | 미적용·안전 이식 가능 | `TryAirBalloon` clean(`INCLUDING_SUBSTITUTES`). | Y | 없음 | — |
| 9946 | 도망갈 수 없는 상태에서 탈출팩 교체 | 미적용·안전 이식 가능 | `BattleScript_EjectPackActivates` clean. `SWITCH_IGNORE_ESCAPE_PREVENTION`이 HnS에 있음. | Y | 없음 | — |
| 9951 | 내던지기 나무열매를 인분·비밀망토가 막지 못함 | 미적용·안전 이식 가능 | `BS_TryFlingHoldEffect` 분기 순서만 바꿈, clean. | Y | 없음 | — |
| 9952 | 기절 피해를 받아도 풍선이 터지지 않음 | 미적용·안전 이식 가능 | `ItemBattleEffects` clean. 테스트만 #9943 이후 순서에 의존. | Y | 없음 | #9943 다음에 적용 |
| 9937 | 연속기 중간 빗나감 후 타격 상태 잔존 | 미적용·안전 이식 가능 | `Cmd_resultmessage`에 `moveDamage=0`, `damagedByAttack=FALSE` 2줄. clean. | Y | 없음 | 문구 변화 없음 |
| 9942 | 퓨처사이드 수정·테스트 | 미적용·안전 이식 가능 | clean. `CANCELER_INTERRUPTIBLE_MOVES` 추가, 파티에 있는 시전자의 능력치로 `DoMoveDamageCalc`를 재사용. `AllocSaveBattleMons`·`PokemonToBattleMon`이 HnS에 있음. HnS의 OHKO 플래그 블록과는 context가 겹치지 않음. | Y | 작음(순감 가능) | #10161의 전제 |
| 9940 | 앵콜 대상의 마지막 기술 보존(풀죽음) | 이미 적용 | HnS 커밋 `6d3c7690c0`(원작자 cherry-pick). 소스 reverse-apply 성립. | Y | — | — |
| 9954 | 위협이 막힐 때 번견 발동 | 미적용·안전 이식 가능 | `BS_JumpIfIntimidateAbilityPrevented` clean. `IsFlowerVeilProtected`·`CompareStat`이 HnS에 있음. | Y | 작음 | #9957(upcoming 판)을 대체 |
| 9976 | 공격자가 기절해도 레드카드 발동 | 미적용·안전 이식 가능 | `TryRedCard`에 `IsBattlerAlive(battlerAtk)` 추가, clean. HnS 레드카드 팝업은 스크립트 쪽이라 영향 없음. | Y(테스트) | 없음 | — |
| 9975 | 만능우산 + 웨더볼 | 이미 적용 | HnS의 `CalcMoveBasePower`·`CheckDynamicMoveType`이 이미 `GetAttackerWeather()`(우산 시 해·비 제거)를 쓴다(메가솔 이식 때 들어옴). | Y | — | — |
| 10002 | 독치장이 반대편 독압정 한도를 확인 | 미적용·안전 이식 가능 | `AbilityBattleEffects` `ABILITY_TOXIC_DEBRIS` clean. | Y | 없음 | — |
| 10007 | 침투가 아군 신비의부적(혼란)을 무시 | 미적용·안전 이식 가능 | `jumpifsafeguard` 매크로에 `jumpiftargetally` 추가, clean. 매크로가 HnS에 있음. | Y | 없음 | — |
| 10028 | 왕의징표석 + 하늘의은총/무지개 중복 | 미적용·안전 이식 가능 | `TryKingsRock` clean. | Y | 없음 | — |
| 10003 | 스킬링크보다 속임수주사위를 우선한 오류(Population Bomb) | 미적용·안전 이식 가능 | `CancelerMultihitMoves` clean(`ctx->holdEffectAtk`/`abilityAtk`가 HnS ctx에 있음). | Y(테스트) | 없음 | — |
| 9999 | gLastMoves 설정 위치 수정 | 이미 적용 | HnS 커밋 `e9a9613f5c`. `battle_message.c`의 `STRINGID_PKMNREDUCEDPP`는 BUFF1/BUFF2로 한글화되어 있음. | Y | — | — |
| 10035 | 흡수 실패 시 드레인주먹(Strength Sap) 발동 | 미적용·안전 이식 가능 | `MoveEndAbsorb` 1줄 clean. | Y(테스트 없음) | 없음 | #10034(upcoming 판)을 대체 |
| 10047 | 독조종 대상 추적 | 미적용·안전 이식 가능 | 전 파일 clean. `SpecialStatus.padding2`에서 1비트 사용. | Y | 없음 | #10231과 패딩 공유(주의) |
| 10040 | 테라폼제로 발동 시점 | 미적용·안전 이식 가능 | `battle_script_commands.c`는 공백 정리 hunk만 실패(HnS의 `UpdateEffectivenessResultFlagsForDoubleSpreadMoves` 구조가 다름) → 이 hunk는 제외. | Y | 작음 | 저우선(테라 사용 여부) |
| 10042 | 사탕폭탄 턴 종료 하락 주체(오기·승기) | 미적용·안전 이식 가능 | `BattleScript_SyrupBombEndTurn`·`HandleEndTurnSyrupBomb` clean. `stickySyrupedBy`가 HnS에 있음. | Y | 없음 | — |
| 10038 | 탈출 아이템 소모 후 공생 발동 | 미적용·안전 이식 가능 | `TrySymbiosis` clean. | Y | 없음 | — |
| 10030 | 공격 없이 기절할 때 일루전 해제 | 미적용·안전 이식 가능 | `TryClearIllusion` clean. | Y | 없음 | — |
| 10029 | 기절한 배틀러의 턴 종료 폼체인지 | 미적용·안전 이식 가능 | `HandleEndTurnFormChange` clean. | Y | 없음 | — |
| 9532 | Bide 리팩터 | 미적용·선행 필요 | 참기를 `EffectHit` 경로로 통합하고 `Cmd_setbide`/`copybidedmg` opcode를 삭제. HnS는 opcode 열거 끝이 `UNUSED_30`(upstream은 선행 삭제로 `UNUSED_31`까지)이고, `BattleScript_BideAttack`에 Champions 효과 플래그(EXTREMELY/MOSTLY)가 들어 있다. | Y | 작음(순감) | 수동 재구성 필요. 고정피해기에 HnS 추가 상성 플래그가 붙지 않는지 확인 필요 |
| 9729 | 야생 더블에 1마리만 나옴 | HNS와 무관 | 원인은 #8943 12v12(`gParties[B_TRAINER_3]`). HnS는 `gEnemyParty[1]`을 사용. | N | — | — |
| 9811 | GetBattlerPartyState에 trainer 사용 | HNS와 무관 | 12v12 전용. HnS는 `partyState[side]` 구조라 정상. | Y | — | — |
| 9784 | 탈출 아이템·거울허브·하얀허브 조정 | 미적용·선행 필요 | `HasAnyBattlerQueuedSwitch`, `queuedSwitch`, `redCardActivated`를 쓰며 `MOVEEND_*` 열거를 재편. | Y | 작음(+4B EWRAM) | #9494·#9717(queued switch) 선행. HnS 아이템 팝업(하얀허브·레드카드·탈출버튼)과 인접 |
| 9799 | 교체 메시지 테스트 + 교체 AI 버그 | 충돌 | 3절 참고 | Y | 작음 | #8943·#9655 선행 |
| 9856 | 접촉 피해 메시지 중복 출력 | HNS와 무관 | HnS의 `BattleScript_HurtAttacker`는 `STRINGID_PKMNHURTSWITH` 한 번만 출력(원인 #9655 없음). | Y | — | — |
| 9864 | 파티 화면 후 이전 몬 HP박스 표시 | HNS와 무관 | `battlerState.notOnField`(#9494 도입)가 HnS에 없음. patch는 적용되지만 컴파일 불가. | N | — | #9494 이식 시 동반 |
| 9843 | 배틀 중 요약화면 파티 순서 | HNS와 무관 | HnS `CB2_ShowPokemonSummaryScreen`은 이미 무조건 `UpdatePartyToBattleOrder()`를 호출(원인은 12v12 개편). | N | — | — |
| 9957 | 번견 수정(upcoming) | 미적용·선행 필요 | `battle_stat_change.c`(#9730), `gBattlersByRawSpeed`(#9784) 전제. 등장 특성의 원시 스피드 순서 변경도 포함. | Y | 작음 | 핵심은 #9954가 대체 |
| 9972 | 지오컨트롤이 2턴 기술이 아님 | HNS와 무관 | HnS `[EFFECT_GEOMANCY]`에 이미 `.twoTurnEffect = TRUE`. 원인은 #9730 통합. | N | — | — |
| 9982 | 헤롱헤롱 메시지 오류 | 이미 적용 | HnS `STRINGID_PKMNSXINFATUATEDY`가 이미 `{B_ATK_NAME_WITH_PREFIX}` 사용. | Y | — | 한글 문자열 유지 |
| 9983 | 연속기 결과 메시지 | 미적용·선행 필요 | `CancelerAccuracyCheck`(#9939)와 `DamageContext` 전제. 새 `BattleScript_BattlerAvoidedMultiHit`가 `STRINGID_BATTLERAVOIDEDATTACK` 뒤 `HITXTIMES`를 출력. | Y | 작음 | 메시지 출력 변경(잠재 충돌). master 대응은 #9937 |
| 9988 | Dancer 순서 config + upcoming 버그 묶음 | 미적용·선행 필요 | Dancer는 `gBattlersByRawSpeed`(#9784), `B_DANCER_ORDER` 필요. 트레이스+특성가드, 원한·길동무 순서, 페인트의 `consecutiveMoveUses`, 탈출버튼/팩의 프리폴 예외는 부분적으로 수동 이식 가능(HnS `MoveEndFaintBlock`은 같은 구조에 cv만 없음). | Y | 작음 | 표본 확인. #10024가 후속 |
| 10024 | 재획득한 트레이스 미발동 | HNS와 무관 | #9988이 만든 `traceActivated` 플래그의 후속 수정. HnS에는 플래그가 없음. | Y | — | #9988 이식 시 필수 동반 |
| 10032 | 뻐기기 + 마이페이스 소프트락 | HNS와 무관 | `battle_stat_change.c`(#9730) 전용 회귀. HnS는 스크립트 기반(`BattleScript_OwnTempoPrevents`). | N | — | — |
| 10034 | 드레인주먹 빗나감·능력 버퍼 | HNS와 무관 | 빗나감 부분은 #10035(master)로 대체. 버퍼 부분은 #9730 회귀. | N | — | #10035 적용 |
| 10064 | 이미 걸린 상태 메시지의 배틀러 오류 | HNS와 무관 | upstream은 #9655 이후 문자열을 `{B_SCR_…}`로 바꿨지만 HnS 한글 문자열은 `{B_DEF_…}`를 유지하고 `B_SCR`를 쓰는 "이미" 문구가 없음. 적용해도 무해(clean). | Y | 없음 | 선택 |
| 10059 | 12v12 파트너 파티 슬롯 | HNS와 무관 | `gPartiesCount`, `PARTY_LAYOUT_MULTI_FULL_PARTNER`(12v12) 전용. | N | — | — |
| 10039 | 멀티배틀 NoAliveMonsForPlayer | HNS와 무관 | 12v12판 함수 수정. HnS `NoAliveMonsForPlayer`는 `gPlayerParty` 기반 구버전(`B_MULTI_BATTLE_WHITEOUT` 매크로 사용). | Y | — | HnS 멀티배틀(목호)은 기존 로직 유지 |
| 10072 | 퓨처사이드 시전자에게 스파이시스프레이 | 이미 적용 | HnS `ABILITY_SPICY_SPRAY` 분기에 이미 `!IsFutureSightAttackerInParty` 있음. | Y | — | — |
| 10074 | 능력 변화기 실패 메시지 중복 | HNS와 무관 | `StatChangeSubstitute`(#9730) 전용. | N | — | — |
| 10077 | 최저 단계에서 오기·승기·아드레날린오브 발동 | HNS와 무관 | HnS는 `PrepareStringBattle`에서 `STRINGID_DEFENDERSSTATFELL`일 때만 발동하고, 최저 단계면 "더 이상 떨어지지 않는다" 문구라 발동하지 않음. 원인은 #9730. | Y | — | — |
| 10079 | 메가솔 날씨·관통드릴 AI | 이미 적용 | HnS에 `AI_CanContactBypassProtect`(수정판)와 `GetTotalAccuracy`/`CanMoveSkipAccuracyCalc`의 `attackerWeather`가 있음. Growth 함수는 HnS에 없음. | Y | — | Battle AI 이식 때 반영 |
| 10088 | 능력 변화 큐의 잘못된 값 | HNS와 무관 | `ClearBothStatChangeQueues`·`SetStatChange`는 #9730 전용. HnS `AbsorbedByStatIncreaseAbility`는 구 방식. | Y | — | — |
| 10091 | 더블에서 지압 타임아웃 | 미적용·선행 필요 | 대부분 `StatChange*`(#9730). `IsTargetingSelfOrAlly` hunk(대상 외 배틀러 처리)는 HnS에도 같은 함수가 있어 별도 검토 필요. | N(테스트 Y) | 작음 | **불확실**: HnS에서 재현 여부 미확인 |
| 10093 | 명령 불복 무작위 기술 | 미적용·안전 이식 가능 | HnS `CancelerObedience` `DISOBEYS_RANDOM_MOVE`는 `BattleScriptCall` 후 원래 기술 스크립트로 복귀(버그 존재). 1줄 수동. | N | 없음 | 추천 |
| 10102 | 링크 싱글·더블 파티 오류 | HNS와 무관 | 12v12판 `GetBattlerTrainer` 수정. HnS는 위치 기반 구버전. | N | — | — |
| 10105 | 링크배틀 P2 최대 HP 0 표시 | 이미 적용 | HnS `Controller_WaitForHealthBar`가 이미 party mon의 `MON_DATA_MAX_HP` 사용(주석 포함). | N | — | — |
| 10149 | 범용 "무효" 메시지 오용 | 충돌 | 3절 참고 | Y | 없음 | — |
| 10132 | 난기류 + 웨더볼·회복기 | 부분 적용 | `Cmd_recoverbasedonsunlight`의 `healingWeather`는 이미 있음. `CalcMoveBasePower` `EFFECT_WEATHER_BALL`은 여전히 `& B_WEATHER_ANY`(난기류 포함) → 1줄 남음. | Y | 없음 | 추천 |
| 10144 | 상성 메시지 순서·사이코필드/매직미러 | 충돌 | 3절 참고 | Y | 작음 | canceler 구조(#9939 등) 선행도 필요 |
| 10161 | 파티 내 퓨처사이드 시전자 문제 | 미적용·선행 필요 | `attackerInParty` 비트, `BattleScript_DoFutureAttackResult`를 `moveendall`로, `StatStages`에서 `changedStatsBattlerId`/`statLowered` 제거(#9730). HnS는 `changedStatsBattlerId`를 오기/승기 판정에 쓴다. 아이템(조개껍질방울·생명의구슬) 함수도 수정. | Y | 작음 | #9942·#9730·#9494 선행. 표본 확인 |
| 10169 | 포이즌힐 시 맹독 카운터 미증가 | 미적용·안전 이식 가능 | HnS `HandleEndTurnPoison`에 같은 버그. 수동(`IsBattlerAlive` 유지). Floette 영원의꽃 폼 테이블은 이미 적용(reverse 성립). `EmergencyExitCanBeTriggered` 시그니처 변경은 선택. | Y | 없음 | 추천 |
| 10186 | 최고지휘관·성묘 기절 카운터 | 미적용·안전 이식 가능 | `faintCounter[trainer]`와 `SetValuesOnFaint` 신설. HnS는 `GetBattlerSideFaintCounter` 사용. 수동. | Y | 작음(+4B heap) | 멀티배틀에서만 차이 |
| 10207 | 나이트메어 팝업 잔존 | 미적용·안전 이식 가능 | HnS `BattleScript_BadDreamsIncrement`(end2 구조)에 1줄 수동. | Y | 없음 | 추천 |
| 10213 | 떨어뜨리기 해제값 | 미적용·안전 이식 가능 | HnS `MoveEndMoveBlock` 구버전에 버그 존재(비행 중 `multipleTurns` 미해제, 프리폴 예외 없음). 수동. `pokemon.c` hunk는 공백만. | N | 없음 | 추천 |
| 10216 | 저주 잘못된 애니메이션 | HNS와 무관 | HnS `BattleScript_EffectCurse`는 구 구조로 `sB_ANIM_TURN`을 경로별로 설정. `Cmd_cursetarget`도 구버전. | Y | — | — |
| 10227 | 불바다 피해 면역(불꽃 타입·매직가드) | 미적용·안전 이식 가능 | `HandleEndTurnFirstEventBlock` clean. `side` 변수가 HnS에 있음. | Y | 없음 | — |
| 10228 | 동적 불꽃 타입 기술의 해동 | 미적용·안전 이식 가능 | `CanFireMoveThawTarget(move, moveType)`. `battle_move_resolution.c`는 `gCurrentMove`로 수동 적응. AI hunk는 clean. | N | 없음 | 추천 |
| 10229 | 일격기가 돌격(Glaive Rush) 명중 보정을 무시 | 미적용·안전 이식 가능 | `DoesOHKOMoveMissTarget` clean(HnS도 cv 판). | Y | 없음 | — |
| 10226 | 사이코시프트 상태 전이 | 미적용·선행 필요 | `TrySynchronizeActivation`/`trysynchronize`(#9446)가 HnS에 없음. 스크립트 hunk는 HnS가 치료 문구를 바꾼 `BattleScript_EffectPsychoShift`(`gStatusCureStringIds`) 바로 앞에 삽입된다. AI 2 hunk(`AI_CheckBadMove`, `GetSwitchinStatusDamage` 괄호 버그)는 독립적이고 안전. | Y | 작음 | AI hunk만 선이식 가능 |
| 10231 | 조개껍질방울 회복 후 위기회피 누락 | 미적용·안전 이식 가능 | `shellBellEmergencyExit` 비트. `TryShellBell`·`EmergencyExitCanBeTriggered` 수동(HnS는 1인자 시그니처, `queuedSwitch` 줄 없음). | Y | 없음 | #10047과 패딩 공유 |
| 10219 | 배틀팩토리 스타일 오표기 | 미적용·안전 이식 가능 | `GetMoveBattleStyle` clean(HnS 프런티어 이식됨). | N | 없음 | — |
| 10185 | 거울허브 오발동 | 미적용·선행 필요 | `battle_stat_change.c`(#9730)의 플래그 기반. HnS 구 `Cmd_statbuffchange`(8024~8045행)에도 같은 버그(다른 거울허브 복사에 재반응, 최대 단계 무시)가 있지만 구조가 다르다. | Y | 작음 | 구조로 재구현 가능하나 고위험 |
| 10263 | 파트너 부재 시 필드 기술 실패(AI 대상) | 미적용·안전 이식 가능 | `SetFinalChosenTarget` 신설로 opponent/partner의 대상 결정을 통합. context(공백·include) 차이로 수동. | N | 작음 | 추천(더블) |
| 10272 | 부활 시 따라큐·빙큐보 폼 복귀 | 미적용·안전 이식 가능 | `form_change_tables.h` clean(`FORM_CHANGE_FAINT` 3줄 삭제). | N | 없음 | — |
| 10287 | 토템 오라 메시지 대상 | HNS와 무관 | HnS는 토템 부스트 때 `gBattlerTarget = battler`로 설정해 `{B_DEF_…}`가 올바름. upstream은 흐름을 attacker 기준으로 바꾼 뒤 고친 것. | Y | — | HnS 한글 문자열 유지 |
| 10289 | 조임밴드 무효화 후 속박 피해 | 미적용·안전 이식 가능 | `VOLATILE_WRAPPED_BINDING_BAND` 추가. `SetWrapTurns`는 clean, `HandleEndTurnWrap`은 수동 1줄. | Y | 없음(비트) | `VOLATILE_UNUSED` 재사용 권장 |
| 10288 | 따라가때리기가 교체 중인 적의 탈출버튼 발동 | 미적용·안전 이식 가능 | `TryEjectButton`/`TryEjectPack`/`EmergencyExitCanBeTriggered`에 `!IsPursuitTargetSet()`. HnS 함수는 queued-switch 줄이 없어 수동. | Y | 없음 | — |
| 10278 | 불태우기 아이템 처리·코어퍼니셔 대상 | 미적용·안전 이식 가능 | `MOVE_EFFECT_INCINERATE`의 `GetBattlerHoldEffect`를 `GetItemHoldEffect(item)`로(1줄 수동). `moves_info.h` Core Enforcer `TARGET_BOTH`는 clean. | Y | 없음 | #9858과 함께 |
| 10175 | 아군 대상 실패 판정 | HNS와 무관 | canceler의 `EFFECT_CAPTIVATE`/`STAT_CHANGE_ON_STATUS`(#9730 통합). HnS 베놈트랩은 별도 `EFFECT_VENOM_DRENCH`. | N | — | — |
| 10306 | 다이맥스 기술의 섬뜩한주문 PP 감소 | HNS와 무관 | 다이맥스 비활성(`B_FLAG_DYNAMAX_BATTLE 0`, HnS 트레이너 데이터에 다이맥스 없음). 적용해도 무해. | Y | — | 선택 |
| 10307 | 멘탈허브가 타이머를 지우지 않음 | 미적용·안전 이식 가능 | `TryMentalHerb`에 `tormentTimer=0`, `healBlockTimer=0` 2줄 수동. HnS는 구 단일-선택 메시지 구조이므로 upstream의 비트마스크 부분은 가져오지 않는다. | Y | 없음 | — |
| 10295 | 사령탑 동시 교체·어써러셔 교체 | 미적용·안전 이식 가능 | `battle_scripts_1.s`·`battle_main.c`·`BS_JumpIfCommanderActive`는 clean. `jumpifcommanderactive` 매크로 인자 추가(HnS 사용처 1곳, 2566행), `battle_util.c`/`battle_move_resolution.c`는 수동. | Y | 작음 | #10231·#10288 다음 |
| 10309 | hitanim 뒤 moveanim 재생 | 미적용·안전 이식 가능 | `MoveEndClearBits`에 `animTurn`/`animTargetsHit` 초기화 2줄 수동. HnS는 `HandleAction_ActionFinished`에서만 초기화. | N | 없음 | 재현 조건 미확인(표본) |
| 10315 | 드래곤애로우 타격 후 메시지 | 충돌 | 3절 참고 | Y | 없음 | — |
| 10325 | 볼주머니 후 공생 미발동 | 미적용·안전 이식 가능 | HnS `Cmd_removeitem`의 `TryCheekPouch && TrySymbiosis`가 같은 버그. `TrySymbiosis(…, nextInstr)` 시그니처 변경을 수동 적응. 팝업을 `BattleScript_AbilityPopUpScripting`으로 교체(같은 배틀러라 순서 변화 없음). | Y | 작음 | — |

## 3. 충돌 상세

### 3.1 #9799 — Adds tests for switch battle messages and switch AI bugfix (`972bffb89f`)

- **upstream 목적**: 링크·녹화·인게임 파트너·2대1 배틀에서 교체와 첫 등장 메시지를 고른다. `sText_TwoLinkTrainersIntroSendOutPkmn` 등을 신설하고, `sText_LinkPartnerSentOutPkmn*GoPkmn`을 "Go, X!"로 바꾸며, `BufferStringBattle`의 `STRINGID_INTROSENDOUT`/`RETURNMON`/`SWITCHINMON` 분기를 재작성한다. AI 쪽은 파트너와 같은 몬을 교체 대상으로 고르지 않도록 `BattlersShareParty` 조건을 추가하고, 스페셜 트레이너의 AI 플래그를 막는다.
- **HnS 현재 동작**: `src/battle_message.c:99-120`의 `sText_*` 교체 문장이 모두 한글화되어 있고(예: 109, 110, 119행), `BufferStringBattle`(2430행~)에 `TESTING` 분기(2470, 2785행)를 포함한 구 분기 구조가 있다. `BattleSideHasTwoTrainers`와 `BattlersShareParty`는 HnS에 없다(12v12 #8943 API).
- **보존 이유**: 한글 문장·줄바꿈·`{B_TXT_…}` 조사 토큰이 들어간 문자열을 새 ID로 대체하거나 삭제하게 된다. 메시지 선택 분기는 HnS 멀티배틀(목호 등)의 출력과 직결된다.
- **선택지**
  - (a) 이식하지 않는다(권장). HnS 멀티배틀 문구에 실제 오류가 있을 때만 해당 분기를 별도로 고친다.
  - (b) AI hunk(`battle_ai_main.c` 스페셜 트레이너 플래그, `battle_ai_switch.c` 파트너 교체 중복)만 HnS 구조(`gEnemyParty` 공유 여부)에 맞게 재작성한다.
  - (c) 12v12 선행 이식 후 전체를 검토한다(비권장, 대규모).

### 3.2 #10144 — Fix effectiveness message order and Psychic Terrain/Magic Bounce interaction (`9e4eb2648c`)

- **upstream 목적**: 효과 없음 판정을 canceler(`CancelerTargetFailure`) 단계로 옮겨 "효과가 없다" 메시지 순서를 맞춘다. 그 과정에서 `STRINGID_ITDOESNTAFFECTTWOFOES`를 삭제하고, `Cmd_resultmessage`의 `MOVE_RESULT_DOESNT_AFFECT_FOE` 더블 분기를 단순화한다. 사이코필드 방어를 매직미러보다 먼저 판정하도록 `CanMoveBeBlockedByTarget`에서 분리한다.
- **HnS 현재 동작**
  - `src/battle_script_commands.c:2093-2115` `Cmd_resultmessage`에 TWOFOES 분기가 있고, 2018~2060행에는 Champions 상성 메시지(`EXTREMELYEFFECTIVE*`, `MOSTLYINEFFECTIVE*`)가 붙어 있다.
  - `src/battle_message.c:884`에 `STRINGID_ITDOESNTAFFECTTWOFOES` 한글 문장("…와 …에게는 효과가 없는 것 같다…")이 있고, `include/constants/battle_string_ids.h:693`, `src/battle_tv.c:187`에서도 사용한다.
  - `src/battle_util.c:2384` `CanMoveBeBlockedByTarget`는 구 순서이고, `src/battle_move_resolution.c:1741` `CancelerTargetFailure`는 `BattleContext` 판이다.
- **보존 이유**: 한글 문자열 ID를 삭제하게 되고, Champions 상성 메시지 최신화(BATTLE_MESSAGE_OUTPUT_CHANGES의 "0.5배 미만·2배 초과", "…TWOFOES")와 같은 함수를 수정한다.
- **선택지**
  - (a) 보류한다(권장).
  - (b) 사이코필드/매직미러 순서 버그만 떼어 HnS `CancelerTargetFailure`에 옮긴다(`CanPsychicTerrainProtectTarget`를 protect/bounce 판정 전으로). 메시지 경로는 건드리지 않는다.
  - (c) 전체 이식 시 `ITDOESNTAFFECTTWOFOES`를 유지하고 canceler 경로에서 TWOFOES 문구를 새로 구성한다(대규모, 사용자 결정 필요).

### 3.3 #10149 — Fix generic ability ineffective battle message misuse (`41467c049e`)

- **upstream 목적**: `STRINGID_PKMNSXMADEITINEFFECTIVE` 문장이 (#9655 이후) "item cannot be removed!"로 잘못 바뀐 것을 "X made it ineffective!"로 되돌린다. 타오르는불꽃의 "이미 강화됨"(`gFlashFireStringIds[B_MSG_FLASH_FIRE_NO_BOOST]`)은 `MADEYINEFFECTIVE` 대신 이 ID를 쓰게 한다.
- **HnS 현재 동작**
  - `src/battle_message.c:522`에서 `STRINGID_PKMNSXMADEITINEFFECTIVE`를 "`{B_SCR_NAME}{B_TXT_EUNNEUN}\n{B_SCR_ABILITY} 때문에 잠들지 않는다!`"로 **스위트베일 수면 방지 전용**으로 재정의했다. 호출처는 `src/battle_end_turn.c:898`, `src/battle_util.c:5534`, `src/battle_script_commands.c:15068`의 스위트베일 경로와 `data/battle_scripts_1.s:2262, 3858`이다.
  - `battle_message.c:489`의 `MADEYINEFFECTIVE`는 "…에게는 효과가 없는 것 같다..."이고, 1410행에서 타오르는불꽃 no-boost가 이것을 쓴다.
- **보존 이유**: upstream 테이블 변경을 그대로 가져오면 타오르는불꽃에서 "잠들지 않는다!"가 출력된다. HnS 쪽 문장·용도가 우선이다.
- **선택지**
  - (a) 이식하지 않는다(권장). HnS는 이미 의미에 맞는 문구를 낸다.
  - (b) upstream의 범용 "무효로 했다" 문구가 필요하면 새 ID를 추가한다(공식 한글 원문 확인 후).
- **참고(HnS 자체 잠재 문제)**: `data/battle_scripts_1.s:3466`의 텔레포트 `BATTLE_RUN_FAILURE` 경로도 `BattleScript_PrintAbilityMadeIneffective`를 거쳐 이 ID(수면 문장)를 출력할 수 있다. Gen8+ 야생 텔레포트는 HnS에서 이 판정을 건너뛰지만, 다른 조건에서 도달할 수 있는지는 확인하지 않았다.

### 3.4 #10315 — Fixes Dragon Darts after hit messages (`4362234013`)

- **upstream 목적**: 드래곤애로우(`TARGET_SMART`)가 "N번 맞았다!"를 출력하지 않고, 타격마다 상성 메시지를 출력하게 한다(`MoveEndMultihitMove`, `Cmd_resultmessage`).
- **HnS 현재 동작**
  - `src/battle_script_commands.c:2024`, `2045`의 `else if (!gMultiHitCounter)` 분기가 Champions의 EXTREMELY/MOSTLY 상성 분기(2018~2060행) 안에 섞여 있다.
  - `src/battle_move_resolution.c:2854` `MoveEndMultihitMove`(2874, 2906행)가 `BattleScript_MultiHitPrintStrings`를 호출한다. `STRINGID_HITXTIMES`는 "`{B_BUFF1}번 맞았다!`"(battle_message.c:219)이다.
- **보존 이유**: 결과 메시지 선택 로직과 연속기 횟수 문구의 출력 조건을 바꾸는 배틀 메시지 출력 변경이다. HnS Champions 상성 메시지 분기와 같은 줄을 수정해야 한다.
- **선택지**
  - (a) 보류한다.
  - (b) 사용자가 동의하면 HnS 분기 4곳(EXTREMELY/SUPER/NOTVERY/MOSTLY)에 `|| target == TARGET_SMART`를 똑같이 넣고, `MoveEndMultihitMove` 2곳에서 SMART일 때 `MultiHitPrintStrings`를 생략한다. 이식하면 BATTLE_MESSAGE_OUTPUT_CHANGES에 기록한다.

### 3.5 잠재 충돌(선행 필요 항목 안에 포함)

- **#9983**: 연속기가 도중에 빗나가면 `BattleScript_BattlerAvoidedMultiHit`가 `STRINGID_BATTLERAVOIDEDATTACK`과 `HITXTIMES`를 출력하는 새 메시지 경로를 만든다. #9939 선행이 필요하고, 이식하면 메시지 출력이 바뀐다. HnS에는 master판 #9937만 넣는 것을 권장한다.
- **#10226**: `BattleScript_EffectPsychoShift`(data/battle_scripts_1.s:1278-1298)의 `curestatus` 바로 앞에 `waitstate`/`trysynchronize`를 삽입한다. HnS는 이 구간의 치료 문구를 `gStatusCureStringIds`(상태별 문구)로 바꿔 두었다. 문구 자체를 바꾸지는 않지만 고위험 구역이다.
- **#9784**: 탈출버튼·레드카드·하얀허브의 MoveEnd 로직을 재편한다. HnS는 이 아이템들에 Champions 아이템 팝업을 연결해 두었으므로, queued-switch 선행 이식 시 팝업 호출 위치를 재확인해야 한다.

## 4. 불확실·추가 확인 필요

- **#10091**: `IsTargetingSelfOrAlly` 변경분이 HnS에서도 지압 더블 문제를 일으키는지 재현하지 않았다. 나머지 부분은 #9730 전용이다.
- **#10309**: HnS에서 같은 애니메이션 순서 오류가 재현되는지 확인하지 않았다. 원인 경로(댄서·Instruct·매직미러 등 한 action 안의 2회 실행)를 추정했다.
- **#9988**: 18파일 묶음이라 표본만 확인했다(Dancer, faint block, Trace, 탈출 아이템, 페인트). 독립 hunk를 개별 이식할지는 따로 판단해야 한다.
- **#9532·#10161**: 대형이라 핵심 hunk만 표본 확인했다.
- **#10040**: HnS에서 테라스탈을 실제로 쓰는지 확인하지 않았다(트레이너 데이터에 `Tera Type`만 2건 존재). 저우선.
- 1.15.x clean-apply 항목은 context와 심볼 존재만 확인했고 빌드·테스트는 하지 않았다. 순차 이식할 때 각 PR마다 `make hns -j8`로 검증해야 한다.
