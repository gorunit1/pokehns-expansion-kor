# g2_battle_fixed_b 인벤토리 보고서 (Battle General / Fixed, 1.16.2~1.17.0, 85 PR)

- 대상: `inv/g2_battle_fixed_b.tsv`의 85개 PR (1.16.2: 3, 1.16.3: 18, 1.16.4: 38, 1.17.0: 26)
- 기준: HNS 스냅샷 `<HnS 작업트리>/` (HEAD `0d89762071`), upstream `<upstream clone>/`
- 방법: PR마다 upstream 커밋 diff(`git diff <c>^1 <c>`, #10556은 머지 커밋 `d636217e0a` 기준)를 파일 단위로 `git apply --check`/`-R --check`/`-C1 --check` 하고, 추가·삭제 라인이 HNS에 있는지 검사했다. fail이 나온 PR은 HNS 해당 함수를 직접 열어 비교했다. 거대 PR(#10542, #10488, #10616, #10657)은 핵심 hunk만 확인했다(표본 확인). 빌드와 테스트는 하지 않았다(읽기 전용 조사).
- 스냅샷과 HNS 원본은 수정하지 않았다(`--check`만 사용).

## 0. 요약

### 판정별 개수

| 판정 | 개수 | PR |
| --- | --- | --- |
| 이미 적용 | 5 | #10477(동등), #10587, #10692, #10193, #10212 |
| 부분 적용 | 0 | (부분 적용 요소는 충돌 항목 #10354, #10268에 포함) |
| 미적용·안전 이식 가능 | 31 | #10344, #10357, #10317, #10369, #10366, #10389, #10406, #10298, #10448, #10476, #10433, #10514, #10539, #10547, #10543, #10554, #10483, #10622, #10636, #10645, #10654, #10653, #10661, #10675, #10683, #10682, #10180, #10434, #10475, #10467, #10690 |
| 미적용·선행 필요 | 20 | #10350, #10386, #10388, #10415, #10441, #10536, #10569, #10568, #10577, #10606, #10631, #10648, #10674, #10687, #10711, #10431, #10463, #10488, #10566, #10600 |
| 충돌 | 4 | #10354, #10217, #10268, #10443 |
| HNS와 무관 | 25 | #10432, #10436, #10556, #10542, #10572, #10571, #10608, #10616, #10639, #10647, #10657, #10671, #10662, #10707, #10286, #10294, #10365, #10440, #10466, #10588, #10586, #10624, #10618, #10621, #10714 |

코디네이터 기준과의 대응: HNS는 upstream master(1.15.2 개발 중) 계열이고 upcoming 전용 리팩터가 전혀 없다. 따라서 표의 "미적용·안전 이식 가능(수동)"은 코디네이터 기준의 **"수동 적응 가능"**(표기·구조 차이만 있음)이다. "선행 필요"는 리팩터가 먼저 있어야 하는 경우, "HNS와 무관"에는 리팩터가 만든 회귀를 고친 PR이 포함된다.

"미적용·안전 이식 가능" 31건 중 `git apply`가 그대로 되는 것은 #10357, #10366(C1), #10369, #10406, #10476, #10539, #10543, #10554, #10622, #10683뿐이다. 나머지는 HNS가 1.16.0 리팩터(아래 §3) 이전 구조(`struct BattleContext *ctx`, `gBattlerTarget` 기반 moveend 등)라서 같은 논리를 손으로 옮겨야 한다. 표에는 "수동"으로 적었다.

### 이식 추천 상위 항목 (작고, 실제 버그이며, 문자열과 무관한 것)

1. **#10406 Intrepid/Dauntless**: HNS는 `GEN_LATEST = GEN_CHAMPIONS (GEN_9+1)`(include/config/general.h:71-74)이다. 그래서 `battle_util.c:3497,3511`의 `GetConfig(...) == GEN_9`가 항상 거짓이 되어, 불굴의검·불굴의방패가 **나올 때마다 발동한다.** `==`를 `>=`로 두 곳만 바꾸면 된다.
2. **#10514 Rage Fist 카운터 오버플로**: `PartyState.timesGotHit:5`(include/battle.h:525)는 32번 맞으면 0으로 돌아간다. 비트 폭만 바꾸면 되고 구조체 크기는 같다.
3. **#10476 Court Change**: `BS_CourtChangeSwapSideStatuses`에 `numHazards` 교환이 없다. apply가 그대로 된다.
4. **#10543 Regenerator/Natural Cure 파티 슬롯**: `Cmd_switchoutabilities`(battle_script_commands.c:10286,10298)에 같은 버그가 있다. apply가 그대로 된다.
5. **#10554 Hunger Switch 팝업 잔류**: 스크립트 한 줄이며 apply가 그대로 된다.
6. **#10622 Flying Press 두 번째 타입 누수**: battle_util.c:8576. apply가 그대로 된다.
7. **#10344 Future Sight가 반응형 도구·특성을 발동**: 조건 5줄을 수동으로 추가한다.
8. **#10475 절대영도 명중**: HNS `GetTotalAccuracy`(battle_util.c:10706)가 얼음 타입이 아닌 사용자의 명중을 ×1.1로 **올리고** 있다. 플래그 이름 변경과 ×0.9 수정이 필요하다.
9. **#10675 Solar Beam/Electro Shot + 만능우산**: `CanTwoTurnMoveFireThisTurn`(battle_move_resolution.c:1482)가 우산을 무시한다. 수동으로 고친다.
10. **#10180 메가진화 뒤 하얀허브·Opportunist·Mirror Herb·Eject Pack 처리**: HNS는 메가진화를 쓰므로 체감 효과가 있다. 메가 스크립트 끝에 명령 하나를 넣고 `BS_EffectsAfterFormChange`를 HNS API로 옮긴다. Sky Drop hunk는 제외한다.

그 밖에 작고 안전한 것: #10357·#10366(특성 플래그 데이터), #10369, #10539, #10547, #10389, #10682, #10653, #10654, #10636/#10645(Acupressure 대상).

### 충돌 목록 (사용자 결정 필요, §2 상세)

- **#10354** Thousand Arrows 양쪽 접지: `MoveEndMoveBlock` 전면 재작성과 한글 문자열 4개의 `{B_DEF_NAME…}`→`{B_EFF_NAME…}` 변경이 필요하다.
- **#10217** Pickpocket(원래 공격자 교체 후): 한글 `STRINGID_PKMNSTOLEITEM`의 `{B_DEF_NAME_WITH_PREFIX}`→`{B_BUFF2}` 변경, `StealTargetItem` 시그니처 변경(HNS의 끈적끈적바늘 출력 변경과 같은 함수), 선행 #9494·#8943이 필요하다.
- **#10268** Champions 배틀 메시지 후속 수정: HNS가 따로 이식한 Champions 아이템 팝업 영역(열매·Custap·Micle·Jaboca·Rough Skin 스크립트)을 다시 손댄다. `COULDNTFULLYPROTECT`는 이미 HNS가 `{B_DEF_…}`다.
- **#10443** Beak Blast·Focus Punch·Shell Trap: 한글 문자열 3개의 `{B_ATK_NAME…}`→`{B_SCR_NAME…}` 변경과 새 Champions config `B_MOVE_EFFECTS_BEFORE_MOVES`가 필요하다.

### 용량 주의

- 큰 증가는 없다. 모두 수 바이트~수백 바이트 수준이다.
- **#10542**(BATTLE_PARTNER/OPPOSITE 매크로 제거, 53파일 +813/−797): 매크로 XOR를 함수 호출로 바꾸므로 ROM이 늘고 속도가 약간 떨어질 수 있다(중간). 기능 변화가 없어 이식을 권장하지 않는다.
- **#10488**(Assurance): `datahpupdate` 인자 추가로 스크립트가 약 60바이트 늘어난다. 선행 필요.
- **#10467**: 애니메이션 스크립트와 스프라이트 템플릿이 추가된다(작음).
- EWRAM: #10386은 `gBattleStruct`에 `u32[4]`(+16B, 힙 할당)를 더한다. #10463은 오히려 비트필드를 줄인다. #10556의 미사용 `UseStatIncreaseItem` 제거는 ROM을 조금 줄인다.

### 핵심 관찰: HNS의 구조적 위치

HNS의 merge-base `3efb836f72`는 **1.15.x master 선상**에 있고, **1.16.0 upcoming 사이클 전체가 HNS에 없다.** 1.16.2~1.16.4의 Fixed 항목은 모두 1.16.0 리팩터 위에서 작성된 수정이라, 대부분 fwd·rev가 둘 다 fail이다. 선행 리팩터 목록은 §3에 정리했다. 이 리팩터가 만들어 낸 회귀를 고치는 PR(#10440, #10466, #10294, #10714, #10586, #10647, #10618, #10588)은 HNS에 버그 자체가 없으므로 "무관"으로 판정했다.

---

## 1. 전체 판정표

고위험파일: Y(…)는 지시서의 고위험 파일(battle_message.c, battle_scripts_1/2.s, battle_util.c, battle_script_commands.c, battle_hold_effects.c, battle_end_turn.c, include/battle*.h, battle_string_ids.h, battle_script.inc)을 게임 코드에서 건드린다는 뜻이다. T는 고위험 파일이 테스트(test/battle)뿐, N은 해당 없음이다.

| PR | 제목 | 판정 | 근거(파일·함수) | 고위험파일 | 용량 영향 | 의존·비고 |
| --- | --- | --- | --- | --- | --- | --- |
| #10344 (1.16.2) | Future Sight가 반응형 도구·특성 발동 | 미적용·안전 이식 가능(수동) | HNS `TryJabocaBerry/TryRowapBerry`(battle_hold_effects.c ~348/370), `TryRedCard/TryEjectButton`(battle_move_resolution.c:3324/3347), 저주받은바디(battle_util.c:3963)에 Future Sight 제외 조건이 없다. HNS에는 `cv`가 없으므로 `GetMoveEffect(gCurrentMove) != EFFECT_FUTURE_SIGHT`로 추가한다 | Y(hold_effects, util) | 없음 | 컨텍스트 불일치로 수동 |
| #10354 (1.16.2) | Thousand Arrows가 두 대상 모두 접지 안 됨 | **충돌** | HNS `MoveEndMoveBlock`(battle_move_resolution.c:3048)은 `gBattlerTarget` 하나만 처리하므로 버그가 있다. 수정하려면 함수 전체를 대상 루프로 바꾸고 한글 문자열 4개의 지정 코드를 바꿔야 한다 | Y(scripts_1, message) | 작음 | §2-1. #9859 구조 차이 |
| #10357 (1.16.2) | Commander 억제·덮어쓰기 플래그 | 미적용·안전 이식 가능 | abilities.h:2161-2162의 `cantBeSuppressed/cantBeOverwritten` 두 줄만 지운다(apply가 그대로 됨) | T | 없음 | 데이터만 |
| #10350 (1.16.3) | Mirror Armor 수정 | 미적용·선행 필요 | 본체가 `battle_stat_change.c`의 `IsMirrorArmorReflected`인데 HNS에는 이 파일이 없다(구형 처리는 battle_script_commands.c:7932). config는 HNS `generational_changes.h`에 추가해야 한다 | Y(util.h, util.c) | 작음 | #9730 Stat Change Refactor, #9529 |
| #10317 (1.16.3) | Commander와 멀티·교체·기믹 예외 | 미적용·안전 이식 가능(수동) | HNS `BS_TryAllySwitch`(12734), Commander(battle_util.c:4811), `TrySwitchInEjectPack`(10279), `MoveEndEjectPack`에 Commander 조건이 없다. HNS에는 선행 Commander 조건(`commanderSpecies` 검사)도 없어 upstream보다 조금 더 넣어야 한다 | Y(script_commands, util) | 없음 | HNS Commander 코드가 더 구형(`gChosenActionByBattler[partner]`) |
| #10369 (1.16.3) | 공용 그림자 애니메이션이 단일 대상기에서 모든 상대에게 표시 | 미적용·안전 이식 가능 | `AnimTask_DestinyBondWhiteShadow` 한 hunk, apply가 그대로 된다 | N | 없음 | |
| #10366 (1.16.3) | 진홍빛고동·하드론엔진 특성 플래그 | 미적용·안전 이식 가능 | abilities.h:2239-2251 플래그 삭제(한글 이름 때문에 C1 컨텍스트) + AI `BenefitsFromSun` 한 줄 | T | 없음 | 데이터 |
| #10386 (1.16.3) | 반사된 기술이 일부 효과를 발동하지 않음 | 미적용·선행 필요 | HNS에는 `MOVEEND_BOUNCED_MOVE`/`MoveEndBouncedMove`가 없다(구형 Magic Bounce 처리) | Y(battle.h) | 작음(EWRAM 힙 +16B) | #9674 Magic Bounce/Coat/Snatch refactor |
| #10389 (1.16.3) | 보이지않는주먹과 미라 상호작용 | 미적용·안전 이식 가능(수동) | HNS `MoveEndProtectLikeEffect`(2124)와 `IsBattlerProtected`(battle_util.c:5991, `ctx` 사용)가 같은 로직이다. `gSpecialStatuses` 패딩 비트(padding2)를 쓰면 된다 | Y(battle.h, util) | 없음 | |
| #10388 (1.16.3) | 아로마테라피가 초식에 막히지 않음 | 미적용·선행 필요 | 데이터(moves_info)는 apply가 되지만, HNS는 `TARGET_USER_AND_ALLY`를 Howl에만 쓰고 `IsAnyTargetAffected`(battle_util.c:10946)에 USER_AND_ALLY 분기가 없다. 단독 적용하면 대상 처리가 틀어질 수 있다 | Y(scripts_1, 주석만) | 없음 | #9898, #10175 |
| #10406 (1.16.3) | Intrepid/Dauntless 미래 대비 | **미적용·안전 이식 가능(최우선)** | HNS `GEN_LATEST=GEN_CHAMPIONS`라 battle_util.c:3497,3511의 `== GEN_9`가 거짓이다. 실제로 1회 제한이 동작하지 않는다 | Y(util) | 없음 | apply가 그대로 됨 |
| #10415 (1.16.3) | 2인 트레이너 전의 환영 파티 범위 | 미적용·선행 필요(**단독 적용 금지**) | upstream은 트레이너별 `gParties`(12v12)라 범위 제한을 지우는 것이 맞다. HNS는 `gEnemyParty` 0-2/3-5 공유 구조(battle.h:1141 `GetBattlerMon`)라, 지우면 두 번째 트레이너의 환영이 A 트레이너 파티를 고르게 된다 | Y(util) | 없음 | #8943 12v12, #9885 |
| #10432 (1.16.3) | FRLG 노인 문구 | HNS와 무관 | 포획 튜토리얼(노인·민진)은 FRLG/에메랄드 기본 맵에서만 쓰인다. 이식하면 한글 `STRINGID_WALLYUSEDITEM`("민진은…", battle_message.c:435)을 지우고 영문 리터럴 "The old man"/"WALLY"를 넣게 되므로 **이식하지 않는 것을 권장한다** | Y(scripts_2, string_ids, message) | 없음 | 한글화 충돌 요소 있음 |
| #10298 (1.16.3) | 변신 포켓몬 보상·포획률 config | 미적용·안전 이식 가능(수동) | HNS `Cmd_getexp`(4166)는 기절 종을 그대로 쓴다. config 2개는 HNS `generational_changes.h`에 추가하고, `gParties[...]`→`gPlayerParty`로 바꾸고, `GetBattleMonCatchRate`를 추가한다 | Y(script_commands) | 작음 | #10441과 한 묶음 |
| #10436 (1.16.3) | FRLG 첫 전투 내보내기 문구 | HNS와 무관 | `STRINGID_INTROSENDOUT` 분기(battle_message.c:2537)는 노인 컨트롤러일 때만 차이가 난다. HNS에서는 튜토리얼 전투를 쓰지 않는다. 코드는 apply가 되고 무해하다 | Y(message, 로직만) | 없음 | 그룹 1이 충돌로 판정한 #9799와 **같은 `BufferStringBattle` `STRINGID_INTROSENDOUT` 분기**(battle_message.c:2537, HNS 챌린지 문구 인접)를 건드린다. #9799가 보류되는 동안 이것도 보류한다 |
| #10441 (1.16.3) | B_TRANSFORM_BATTLE_REWARDS 적용 세대 | 미적용·선행 필요 | #10298이 추가하는 config·코드를 고치는 PR이다 | Y(script_commands) | 없음 | #10298과 함께 이식 |
| #10448 (1.16.3) | Struggle 반동 반올림 | 미적용·안전 이식 가능(수동) | HNS `SetMoveEffect` `MOVE_EFFECT_RECOIL_HP_25`(battle_script_commands.c:2868, `gEffectBattler` 사용)에 반올림 두 줄을 넣는다 | Y(script_commands) | 없음 | |
| #10476 (1.16.3) | Court Change 장판 개수 교환 | 미적용·안전 이식 가능 | `BS_CourtChangeSwapSideStatuses`에 `numHazards` SWAP 추가, apply가 그대로 된다 | Y(script_commands) | 없음 | |
| #10433 (1.16.3) | Opportunist·Mirror Herb가 턴 종료에 발동 안 함 | 미적용·안전 이식 가능(수동, 중간 규모) | HNS end turn(battle_end_turn.c:1302 `IsWhiteHerbEndTurnActivation`)과 `gQueuedStatBoosts`(battle_main.c:5331 초기화) 구조가 같다. ENDTURN 단계 2개와 핸들러를 추가하고 hold effect 플래그를 합친다 | Y(hold_effects.h, end_turn, hold_effects) | 작음 | HNS `activateOpportunist:2` 구형 플래그와의 상호작용 검증 필요 |
| #10477 (1.16.3) | Misty Terrain 접지 검사 | 이미 적용(동등) | HNS battle_util.c:6729는 이미 `ctx->abilityDef`(방어 측 특성)를 쓴다 | Y(util) | 없음 | |
| #10514 (1.16.3) | Rage Fist 피격 카운터 오버플로 | 미적용·안전 이식 가능 | include/battle.h:525 `timesGotHit:5`를 `:8`로, padding을 `8`에서 `5`로 바꾼다(구조체 크기 동일) | Y(battle.h) | 없음 | |
| #10536 (1.16.4) | 여러 경험치 버그 | 미적용·선행 필요 | `givenExpMons[2]`/`GetBattlerTrainer` 인덱스와 파트너 파티 분리(12v12)가 전제다. HNS는 파트너 몬이 `gPlayerParty` 3-5에 있어 `INGAME_PARTNER && expMonId>=3` 검사를 지우면 틀어진다 | Y(battle.h, script_commands, util) | 없음 | #8943. 레벨업 배틀러 선택 부분만 따로 떼어 옮기는 방법은 검토할 수 있다 |
| #10539 (1.16.4) | 그림자가 한 프레임 SPECIES_NONE 크기 | 미적용·안전 이식 가능 | apply가 그대로 된다. `gBattleMons` 초기화를 `FreeResetData_ReturnToOvOrDoEvolutions`로 옮긴다. 이후 경로(진화·너즐록)에서 `gBattleMons`를 쓰지 않음을 확인했다 | N | 없음 | |
| #10547 (1.16.4) | CanTargetBattler에 생존 검사 | 미적용·안전 이식 가능 | battle_util.c `CanTargetBattler`(9710) apply가 그대로 되고, player controller는 C1이다. 야생 더블 대상 선택 루프(battle_controller_opponent.c:516)에도 안전하다 | Y(util) | 없음 | |
| #10543 (1.16.4) | Regenerator/Natural Cure 슬롯 오류 | 미적용·안전 이식 가능 | battle_script_commands.c:10286,10298 `gBattleStruct->battlerPartyIndexes`를 `gBattlerPartyIndexes`로 바꾼다. apply가 그대로 된다 | Y(script_commands) | 없음 | |
| #10556 (1.16.4) | X 아이템 수정 | HNS와 무관 | 이중 능력치 상승 버그는 #9730 이후 `item_use.c`의 `SetStatChange`에서 생긴 것인데 HNS(item_use.c:1318)에는 없다. `UseStatIncreaseItem`(pokemon.c:6681)은 HNS에서도 호출하는 곳이 없어 지우면 ROM이 조금 줄어든다(선택) | Y(script_commands) | 없음(삭제 시 감소) | 머지 커밋 d636217e0a |
| #10554 (1.16.4) | Hunger Switch 팝업 잔류 | 미적용·안전 이식 가능 | `BattleScript_BattlerFormChangeNoPopup`(scripts_1:5557)에 `sethword sABILITY_OVERWRITE, 0` 한 줄. 메시지와 무관하다 | Y(scripts_1) | 없음 | |
| #10542 (1.16.4) | BATTLE_PARTNER/OPPOSITE 매크로 제거 | HNS와 무관(순수 리팩터, 이식 비권장) | HNS는 `BATTLE_PARTNER(`를 632곳에서 쓴다. 기능 변화 없이 매크로를 함수로 바꾸는 작업이다(표본 확인) | Y(다수) | **중간(ROM 증가 가능)** | 앞으로 upstream 수정을 옮길 때 이름만 바꿔 읽으면 된다 |
| #10483 (1.16.4) | Knock Off 뒤 Symbiosis 발동 | 미적용·안전 이식 가능(수동, 부분) | 핵심은 `TrySymbiosis`(battle_util.c:10358)의 `itemLost[...].stolen` 조건 삭제다. `TryRestoreHeldItems`(9470) 변경은 #10577이 대부분 되돌리므로 두 PR을 합친 결과만 반영한다. `B_TRAINER_PLAYER`는 HNS에서 `B_SIDE_PLAYER`/`B_TRAINER_0`에 해당한다 | Y(script_commands, util) | 없음 | #10577과 함께. itemLost 인덱스 의미 확인 필요 |
| #10569 (1.16.4) | Own Tempo가 막은 Swagger/No Retreat 뒤 멈춤 | 미적용·선행 필요 | `trymovestatchanges` 명령과 `battle_stat_change.c`가 HNS에 없다 | Y(scripts_1, scripts.h) | 없음 | #9730/#9928 |
| #10568 (1.16.4) | 파티 패배 함수 단순화 | 미적용·선행 필요 | `gParties[B_TRAINER_PARTNER]` 등 12v12 전제다. HNS 고유 `battle_setup.c`(`CB2_EndTrainerBattle`, 너즐록·화이트아웃)와도 겹친다 | Y(script_commands) | 없음(감소) | #8943. HNS battle_setup 보존 주의 |
| #10572 (1.16.4) | 멀티 배틀에서 플레이어 측 트레이너 슬라이드 | HNS와 무관 | HNS `sTrainerSlides`가 비어 있다(trainer_slide.c:59). 논리는 `B_TRAINER_0` 명칭으로 옮길 수 있다 | N | 없음 | 슬라이드를 도입하면 재검토 |
| #10571 (1.16.4) | 트레이너 슬라이드와 폼 변경 | HNS와 무관 | 슬라이드 데이터가 없다. 메가 스크립트(`trytrainerslidemegaevolutionmsg`, scripts_1:5496)와 `ActivateMegaEvolution` 흐름을 바꾸므로 이득 없이 위험만 있다 | Y(다수) | 없음 | #10624가 다시 대체 |
| #10577 (1.16.4) | 도구 복원 회귀 수정 | 미적용·선행 필요 | #10483이 바꾼 `TryRestoreHeldItems`를 되돌리는 PR이다 | Y(util) | 없음 | #10483과 한 묶음 |
| #10587 (1.16.4) | TARGET_USER_AND_ALLY 깜빡임 | 이미 적용 | rev가 된다. HNS에 `HideShownTargets`가 없고 `HideAllTargets()`를 쓴다(battle_controller_player.c:602-642) | N | 없음 | |
| #10608 (1.16.4) | 멀티 배틀 수면 클로즈 | HNS와 무관 | 트레이너별 파티 인덱스 충돌은 12v12에서만 생긴다. HNS는 같은 편 파티 인덱스 0-5가 겹치지 않는다 | Y(다수) | 없음 | #8943 전용 |
| #10606 (1.16.4) | Dragon Darts가 같은 대상을 여러 번 검사 | 미적용·선행 필요 | `CancelerAccuracyCheck`가 HNS에 없다(명중은 `Cmd_accuracycheck`) | T | 없음 | #9939. 그룹 1의 #10315(Dragon Darts 적중 후 메시지, 충돌)와 같은 기술 영역이므로 함께 다룬다 |
| #10616 (1.16.4) | Z기술·다이맥스 대미지 계산 | HNS와 무관 | HNS는 다이맥스 플래그가 `B_FLAG_DYNAMAX_BATTLE 0`이고, Z크리스탈·Z파워링 입수 경로와 트레이너 데이터가 없다. `DamageContext`(#9657) 전제다(표본 확인) | Y(다수) | 작음 | |
| #10631 (1.16.4) | 능력치 변화기 명중 | 미적용·선행 필요 | `StatChangeAccuracy`는 #9730 구조다 | T | 없음 | #9730 |
| #10622 (1.16.4) | Flying Press가 두 번째 타입을 유지 | 미적용·안전 이식 가능 | battle_util.c:8576 `EFFECT_TWO_TYPED_MOVE` 뒤에 `moveType`을 복원한다. apply가 그대로 된다 | Y(util) | 없음 | |
| #10636 (1.16.4) | Acupressure가 상대를 대상으로 삼음(대상 결정) | 미적용·안전 이식 가능(수동) | HNS `CancelerSetTargets`(battle_move_resolution.c:869, `gBattlerTarget` 사용)에 `TARGET_USER_OR_ALLY` 아군 기절 분기가 없다 | T | 없음 | #10645와 함께 |
| #10639 (1.16.4) | 저주받은바디가 다이맥스기에 발동 | HNS와 무관 | 다이맥스 미사용. 조건 한 줄이라 넣어도 무해하다 | Y(util) | 없음 | |
| #10647 (1.16.4) | Future Sight 뒤 배틀몬 미복원 | HNS와 무관 | HNS `DoFutureSightAttackDamageCalc`(battle_util.c:8054)에는 `savedBattleMons` 백업·복원 구조 자체가 없다 | Y(util) | 없음 | |
| #10648 (1.16.4) | 원래 대상 기절 시 Sky Drop이 교대 몬을 맞힘 | 미적용·선행 필요 | HNS는 구형 `gBattleStruct->skyDropTargets`(battle_move_resolution.c:1526~) 구조이고 `volatiles.skyDropTarget`이 없다 | Y(script_commands) | 없음 | #9249 Sky Drop refactor. 구형 구현에 같은 버그가 있는지는 따로 검증해야 함 |
| #10645 (1.16.4) | Acupressure가 상대를 대상으로 삼음(선택 UI) | 미적용·안전 이식 가능(수동, 부분) | HNS player controller(728)가 `CountAliveMonsInBattle<=1`일 때 `GetDefaultMoveTarget`로 **상대를 고르는** 버그가 있다. `SetFinalChosenTarget`은 HNS에 없고, 상대·파트너 컨트롤러는 USER_OR_ALLY를 자기 자신으로 처리하므로 제외한다 | Y(controllers.h, util) | 없음(감소) | |
| #10654 (1.16.4) | 대상을 기절시킨 기술을 Copycat이 복사 못 함 | 미적용·안전 이식 가능(수동) | HNS `MoveEndUpdateLastMoves`(2691~, `gCurrentMove/gBattlerTarget`)가 같은 구조다. `gLastUsedMove` 갱신을 생존 조건 밖으로 옮긴다 | T | 없음 | |
| #10653 (1.16.4) | 전체기에서 Symbiosis 도구가 반감열매로 소모 | 미적용·안전 이식 가능(수동) | HNS `MoveEndSymbiosis`(2484)에 `berryReduced = FALSE` 한 줄 | T | 없음 | |
| #10657 (1.16.4) | 선택 단계에서 Z기술이 제한에 막힘 | HNS와 무관 | Z기술·다이맥스 전용(표본 확인) | Y(util) | 없음 | |
| #10661 (1.16.4) | Metronome으로 나온 전체기가 파트너를 맞힘 | 미적용·안전 이식 가능(수동) | HNS `CancelerPPDeduction`(battle_move_resolution.c:1010)에도 `ClearDamageCalcResults()`가 있다. `Cmd_setcalledmove`(8797)로 옮긴다 | Y(script_commands) | 없음 | 더블 배틀 실전 검증 필요 |
| #10674 (1.16.4) | gSelectedOrderFromParty 범위 밖 읽기 | 미적용·선행 필요 | #10568이 새로 만든 `WillPlayerWhiteOutIfPartnerWinsAlone` 안의 수정이다 | Y(script_commands) | 없음 | #10568 |
| #10671 (1.16.4) | Red Card로 Z기술 재사용 | HNS와 무관 | Z기술 미사용 | Y(script_commands) | 없음 | |
| #10662 (1.16.4) | 멀티 배틀 파티 슬롯 충돌(환영) | HNS와 무관 | HNS `&party[gBattlerPartyIndexes[partner]]`(battle_util.c:9159)는 HNS `GetBattlerMon`과 같은 결과다 | Y(util) | 없음 | #8943 전용 |
| #10675 (1.16.4) | Solar Beam/Solar Blade/Electro Shot이 충전 생략 | 미적용·안전 이식 가능(수동) | HNS `CanTwoTurnMoveFireThisTurn`(1482)은 `weather & moveWeather`만 보고 만능우산을 무시한다. HNS `IsBattlerWeatherAffected(battler, flags)` 시그니처에 맞춘다 | T | 없음 | |
| #10683 (1.16.4) | 배틀본드 뒤 지우개닌자 능력치 재계산 | 미적용·안전 이식 가능(실효 없음) | apply가 그대로 된다. `GetConfig(B_BATTLE_BOND) < GEN_9` 경로만 바뀌는데 HNS는 최신 세대 설정이라 쓰이지 않는다 | Y(util) | 작음 | 우선순위 낮음 |
| #10687 (1.16.4) | 다중 대상 능력치기 빗나감 문구 | 미적용·선행 필요 | `StatChangeTryChange`는 #9730 구조다 | T | 없음 | #9730 |
| #10682 (1.16.4) | 전체기에 맞은 뒤 Shell Trap이 늦게 발동 | 미적용·안전 이식 가능(수동) | HNS `MoveEndShellTrap`(3268)과 `ChangeOrderTargetAfterAttacker(void)`(battle_util.c:11043)가 같은 구조다. 호출부 4곳의 인자를 맞춘다 | Y(util.h, script_commands, util) | 없음 | |
| #10692 (1.16.4) | 기술 선택 중 배틀러가 안 보임 | 이미 적용 | HNS `ShouldHideBattler`(battle_main.c:2924, 주석 포함)와 `HideAllTargets`가 이미 있다 | Y(battle_main.h) | 없음 | |
| #10707 (1.16.4) | 거다이 산미 추가 효과 | HNS와 무관 | 거다이맥스 미사용. `MOVE_EFFECT_STAT_MINUS` 명칭은 #9730 쪽이다 | N | 없음 | |
| #10711 (1.16.4) | 팔로워 NPC와 6마리 멀티 배틀 | 미적용·선행 필요 | `AreMultiPartiesFullTeams`로 6마리 멀티를 하려면 12v12가 필요하다 | Y(util) | 없음 | #8943 |
| #10180 (1.17.0) | 메가진화 뒤 효과 발동 | 미적용·안전 이식 가능(수동) | HNS 메가 스크립트(scripts_1:5494-5507, 원시회귀, 테라)는 `switchinabilities` 뒤에서 끝난다. `effectsafterformchange` 매크로와 `BS_EffectsAfterFormChange`를 옮긴다. HNS에는 `gBattlersByRawSpeed`와 `cv`가 없으므로 `SortBattlersBySpeed`로 바꾼다. `IsBattlerInvolvedInSkyDrop` hunk는 제외한다 | Y(inc, scripts_1, util.h, script_commands, util) | 작음 | Sky Drop 부분은 #9249. 메시지 출력 변경 없음(명령 추가만) |
| #10193 (1.17.0) | 명중 계산 함수 개선 | 이미 적용 | SESSION_LOG 2026-09-15(Items·Battle AI 이식)에 기록돼 있다. `GetTotalAccuracy(struct BattleCalcValues*)` 등이 있다(battle_util.h:405-408) | Y | 없음 | |
| #10217 (1.17.0) | 원래 공격자가 필드에 없을 때 Pickpocket | **충돌** | 한글 `STRINGID_PKMNSTOLEITEM`(battle_message.c:325)의 지정 코드를 바꿔야 한다. `StealTargetItem`(2264) 시그니처를 바꾸고, HNS에 없는 `redCardSwitched`·`gParties`를 전제한다 | Y(다수) | 작음 | §2-2. #9494, #8943 |
| #10212 (1.17.0) | AI용 함수의 gCurrentMove 하드코딩 | 이미 적용 | SESSION_LOG 2026-09-15 기록. rev가 되는 파일이 다수이고 추가 라인 대부분이 존재한다 | Y | 없음 | |
| #10268 (1.17.0) | Champions 배틀 메시지 후속 수정 | **충돌(일부 이미 반영)** | `COULDNTFULLYPROTECT`는 HNS에서 이미 `{B_DEF_…}`다(battle_message.c:801). 나머지는 HNS가 따로 이식한 아이템 팝업 영역(Custap 7686, Micle 7695, Jaboca 7707, BerryFocusEnergy 7432, Rough Skin 7015)의 팝업·대기 순서를 바꾼다 | Y(inc, scripts_1, interface.h, message, script_commands) | 작음 | §2-3 |
| #10286 (1.17.0) | 초반 라이벌전 뒤 스크립트 미진행 | HNS와 무관 | `trainerbattle_earlyrival`(asm/macros/event.inc:884)은 HNS에서 다른 형식(TRAINER_BATTLE_EARLY_RIVAL)이고 FRLG Route22에서만 쓴다 | N | 없음 | |
| #10294 (1.17.0) | 아이템 회복 애니메이션 대상 오류 | HNS와 무관 | 버그를 만든 upstream 스크립트 구조(`…_AnimContinue`, SIMPLE_HEAL on ATTACKER)가 HNS에 없다. HNS `BattleScript_ItemHealHP_RemoveItem`(7282)은 BS_SCRIPTING만 쓴다 | Y(scripts_1) | 없음 | |
| #10365 (1.17.0) | 분류 아이콘 off-by-one | HNS와 무관(**적용 금지**) | apply는 되지만 HNS `enum DamageCategory`(include/constants/pokemon.h:239)에는 `DAMAGE_CATEGORY_NONE`이 없다. `NULL`을 넣으면 요약 화면 아이콘이 한 칸씩 밀린다 | N | 없음 | |
| #10431 (1.17.0) | 반감열매 발동 순서 | 미적용·선행 필요 | HNS는 `TryActivateWeaknessBerry`(battle_script_commands.c:1483) 구형 경로를 쓴다. `CancelerPreAnimActivations`·`MOVEEND_RESIST_BERRY_MESSAGE`가 없다. HNS `BattleScript_BerryReduceDmg`(7227)의 Champions 팝업과도 겹친다 | Y(scripts_1, battle.h, scripts.h, script_commands) | 작음 | #10362(canceler 애니·HP 갱신), #10268 |
| #10434 (1.17.0) | 실패한 접촉기가 방어기 접촉 효과를 발동 | 미적용·안전 이식 가능(수동) | HNS `MoveEndProtectLikeEffect`(2108)에 `unableToUseMove`·`IsBattlerUnaffectedByMove` 조기 반환이 없고, `IsBattlerProtected`(battle_util.c:6015)의 Burning Bulwark에 상태기 예외가 없다 | Y(util) | 없음 | |
| #10440 (1.17.0) | 원시 날씨에서 Solar Beam/Electro Shot 문제 | HNS와 무관 | 1.16의 enum 날씨 비교 도입에서 생긴 회귀다. HNS는 `weather & moveWeather` 비트 플래그 방식이고 `B_WEATHER_SUN`이 PRIMAL을 포함해 정상이다 | Y(battle.h, util) | 없음 | |
| #10466 (1.17.0) | 모든 날씨에서 2턴 기술이 즉시 발동 | HNS와 무관 | #10440의 후속 회귀 수정이라 HNS에는 해당 코드가 없다 | Y(util.h, end_turn, util) | 없음 | |
| #10463 (1.17.0) | ShouldTrainerBattlerUseGimmick 재작성 | 미적용·선행 필요 | `trainer_util.c/h`가 HNS에 없다. HNS는 `opponentMonCanTera` 비트필드(battle_gimmick.c:81)다. HNS에서는 Red(TRAINER_RED_POSTOBC_HNS)가 테라를 쓴다 | Y(battle.h) | EWRAM 소폭 감소 | #9440 |
| #10443 (1.17.0) | Beak Blast·Focus Punch·Shell Trap 상호작용 | **충돌** | 한글 문자열 3개(battle_message.c:357,770,795)를 `{B_ATK_…}`에서 `{B_SCR_…}`로 바꿔야 한다. Champions config 추가, `TryDoMoveEffectsBeforeMoves`(battle_main.c:5411) 재작성, 앵콜 스크립트 추가 | Y(inc, scripts_1, main.h, scripts.h, message, script_commands) | 작음 | §2-4. Beak Blast 접촉 조건만 따로 떼면 안전 이식 가능(HNS 2185) |
| #10475 (1.17.0) | 절대영도 타입 검사·플래그 이름 | 미적용·안전 이식 가능 | HNS `GetTotalAccuracy`(battle_util.c:10706)가 `×110/100`로 명중을 올리는 버그가 있다. move.h, moves_info, AI는 apply가 되고 battle_util만 수동이다 | Y(util) | 없음 | |
| #10467 (1.17.0) | Protect 애니메이션 추가와 버그 수정 | 미적용·안전 이식 가능(수동, 일부) | Wide Guard 판정(battle_util.c:6005 `IsSpreadMove`는 싱글에서 FALSE)이 수정 대상이다. 새 방어 애니메이션은 HNS에 `B_ANIM_HELD_ITEM_BERRY`가 없어(NUM_B_ANIMS_GENERAL 63) 번호를 맞춰야 한다. `BattleScript_TargetProtected`에 해당하는 HNS 라벨도 다르다 | Y(scripts_1, anim_scripts.h, util) | 작음 | 애니메이션은 시각 변화라 선택 사항 |
| #10488 (1.17.0) | Assurance 불일치 | 미적용·선행 필요 | HNS `datahpupdate`는 `battler, updateState` 2인자 구형(battle_script.inc:51)이고 스크립트 59곳에서 쓴다. upstream은 #10362 이후 1인자를 전제로 인자를 추가한다(표본 확인) | Y(inc, scripts_1/2, script_commands) | 작음(+스크립트 바이트) | #10362. 고위험 스크립트 대량 수정이라 비권장 |
| #10566 (1.17.0) | 없는 배틀러에 대한 HP바·결과 메시지 | 미적용·선행 필요 | `CancelerAccuracyCheck`, `ShouldSkipBattlerForDamage`, `ShouldSkipStatChangeOnBattler` 등 1.16 canceler·stat 구조가 전제다 | Y(util) | 작음 | #9939, #10220, #10362, #9730 |
| #10588 (1.17.0) | "Simplify IsAnyTargetAffected" 되돌리기 | HNS와 무관 | #10576을 되돌리는 PR이다. HNS에는 #10576이 없어 합산 효과가 0이다 | Y(util) | 없음 | |
| #10586 (1.17.0) | 배경 로드 전 지형 미초기화 | HNS와 무관(**적용 금지**) | apply는 되지만 HNS 지형은 `gFieldStatuses` 비트(battle_bg.c:1429)이고 `gFieldTimers.terrain`이 없다. `gFieldStatuses = 0`(battle_util2.c:60)을 지우면 컴파일이 실패하거나 회귀한다 | N | 없음 | |
| #10600 (1.17.0) | 특성이 바뀐 뒤 배짱이 고스트를 관통 못 함 | 미적용·선행 필요 | `CancelerTargetFailure`·`CancelerDamageCalc`의 저장된 상성 구조는 #10220이 전제다. HNS에는 #10220이 의도적으로 빠져 있다(SESSION_LOG) | Y(battle.h, util.h, util) | 없음 | #10220 |
| #10624 (1.17.0) | 기믹 슬라이드 수정 | HNS와 무관 | 슬라이드 데이터가 없고 #10571이 전제다 | Y(scripts_1, gimmick.h, script_commands, util) | 없음(감소) | |
| #10618 (1.17.0) | 가방 열기 메모리 누수 | HNS와 무관(버그 없음) | HNS `OpenBagAndChooseItem`(battle_controller_player.c:1599)은 `CB2_BagMenuFromBattle`을 한 번만 부른다 | N | 없음 | |
| #10621 (1.17.0) | 보이면 안 되는 HP바 요소 표시 | HNS와 무관 | 디버그 `hpNumbersNoBars`와 `B_HP_PERCENTAGE_DISPLAY`(HNS FALSE) 경로만 바뀐다. HNS 전용 64×32 체력바 영역이다 | N | 없음 | HP%를 켤 때 재검토 |
| #10690 (1.17.0) | 기술 대상 선택과 판정 분리 | 미적용·안전 이식 가능(수동) | HNS `GetBattlerMoveTargetType`(battle_util.c:9697)이 Expanding Force 지형 판정을 선택 UI에도 쓴다. 호출부(controller 7곳, gfx_sfx_util 2곳)를 선택용 함수로 바꾼다 | Y(util.h, util) | 없음 | |
| #10714 (1.17.0) | 기술 애니메이션 회귀 | HNS와 무관 | `CancelerMoveAnimation`(#10362)이 만든 회귀라 HNS에는 없다 | N | 없음 | |

---

## 2. 충돌 항목 상세

### 2-1. #10354 Thousand Arrows가 두 대상 모두 접지되지 않음 (upstream 3e3b79d916)

- **upstream 목적**: `MoveEndMoveBlock`을 `eventState.moveEndBattler` 루프로 바꿔 Knock Off·Thief·Smack Down/Thousand Arrows·Fell Stinger 등을 대상마다 처리한다. 처리 대상을 `gEffectBattler`로 두므로 해당 문자열의 대상 지정 코드도 `{B_EFF_…}`로 바꾼다. `BattleScript_KnockedOff`의 애니메이션 대상은 `BS_EFFECT_BATTLER`로, `BattleScript_AbilityPreventsPhasingOutRet`는 `BattleScript_AbilityPopUp`으로 바꾼다.
- **HNS 현재 동작**: `src/battle_move_resolution.c:3048 MoveEndMoveBlock()`은 `gBattlerTarget` 하나만 처리한다(3170 `EFFECT_SMACK_DOWN`). 그래서 더블에서 Thousand Arrows가 한쪽만 떨어뜨린다(버그 존재).
- **겹치는 한글 문자열** (`src/battle_message.c`):
  - 366 `STRINGID_PKMNANCHOREDITSELF` = `{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 뿌리를 뻗어서…`
  - 368 `STRINGID_PKMNKNOCKEDOFF` = `…{B_DEF_NAME_WITH_PREFIX}의\n{B_LAST_ITEM}…`
  - 386 `STRINGID_PKMNANCHORSITSELFWITH` = `{B_DEF_NAME_WITH_PREFIX}…{B_DEF_ABILITY} 때문에…`
  - 586 `STRINGID_FELLSTRAIGHTDOWN` = `{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n떨어뜨리기를 당해서…`
  - 847/849 불바다·습지 문구는 이미 `{B_ATK_TEAM2}`라 upstream 결과와 같다(부분 일치).
- **보존 이유**: 지시서 규칙상 한글 문장과 `{B_...}` 지정 코드는 HNS가 우선이다. 루프로 바꾼 뒤에도 `gBattlerTarget`을 처리 대상과 같게 맞추지 않으면, `{B_DEF_…}`인 한글 문자열이 **잘못된 포켓몬 이름을 출력한다.** 386번은 `{B_DEF_ABILITY}`도 쓰므로 `gBattlerAbility` 기준 코드와의 정합도 확인해야 한다.
- **스크립트 위치**: `data/battle_scripts_1.s:5407 BattleScript_KnockedOff`, `:6692 BattleScript_AbilityPreventsPhasingOutRet`.
- **선택지**:
  1. 이식하지 않고 HNS 동작을 유지한다(Thousand Arrows 한쪽만 접지).
  2. 루프로 바꾸되, 각 효과에서 `gBattlerTarget = battlerDef`로 설정해 한글 `{B_DEF_…}`를 그대로 쓴다. 문자열은 바꾸지 않는다. 한글 변경은 없지만 upstream과 다르게 가므로 테스트가 필요하다.
  3. upstream대로 한글 문자열 4개의 지정 코드를 `{B_EFF_…}`로 바꾼다. 조사 토큰(`{B_TXT_EUNNEUN}` 등)이 EFF 이름에도 제대로 붙는지 확인해야 하고, 사용자 승인이 필요하다.
  - 어느 쪽이든 #10483(`itemLost` 인덱스), #10217(`StealTargetItem` 시그니처)과 같은 함수를 건드리므로 순서를 조율해야 한다.

### 2-2. #10217 원래 공격자가 필드에 없을 때 Pickpocket (upstream 52d3bf9f5b)

- **upstream 목적**: Red Card 등으로 공격자가 교체된 뒤에도 Pickpocket이 원래 공격자의 도구를 빼앗게 한다. `redCardSwitched`를 `originalBattlerPartyId`로 바꾸고, `StealTargetItem(…, itemOverride)`에 인자를 추가하고, 문자열 주체를 `gBattleTextBuff2`로 옮긴다. `BattleScript_PickpocketPrevented`는 따로 노출한다.
- **HNS 현재 동작**:
  - `src/battle_message.c:325 STRINGID_PKMNSTOLEITEM` = `{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_DEF_NAME_WITH_PREFIX}{B_TXT_EU}로부터\n{B_LAST_ITEM}{B_TXT_EULREUL} 빼앗았다!`. upstream은 이 자리를 `{B_BUFF2}`로 바꾼다.
  - `src/battle_script_commands.c:2264 StealTargetItem(battlerStealer, itemBattler)`: HNS의 "끈적끈적바늘 즉시 전이"(BATTLE_MESSAGE_OUTPUT_CHANGES.md, `BattleScript_StickyBarbTransfer` 연출 제거)가 이 함수를 쓴다.
  - `src/battle_move_resolution.c:3508 MoveEndPickpocket()`, `data/battle_scripts_1.s:7817 BattleScript_Pickpocket`.
  - HNS에는 `battlerState.redCardSwitched`(#9494)가 없고 `gParties[GetBattlerTrainer()]`(12v12)도 없다.
- **보존 이유**: 한글 문장의 지정 코드와 조사 토큰(`{B_TXT_EU}로부터`)을 `{B_BUFF2}`로 바꾸면 조사 처리가 버퍼 문자열에서도 되는지 확인해야 한다. 끈적끈적바늘 출력 변경 경로도 바뀐다.
- **선택지**:
  1. 보류한다(권장). 선행 #9494, #8943이 없어 어차피 그대로 옮길 수 없다.
  2. 원래 공격자가 필드에 있는 일반 경우만 고친다: 끈적끈적한몸이면 `BattleScript_PickpocketPrevented`로 분기하고, `DoesSubstituteBlockMove`·`IsBattlerAlive` 조건을 정리한다. 문자열은 바꾸지 않는다.
  3. 선행 리팩터를 들인 뒤 `{B_BUFF2}` 전환을 사용자가 승인하면 적용한다.

### 2-3. #10268 Champions 배틀 메시지 후속 수정 (upstream 94780b0c86)

- **upstream 목적**: Champions 메시지 PR(#9777)의 후속이다. `waitabilitypopup` 명령을 추가해 특성·아이템 팝업이 사라질 때까지 기다리고, Custap·Micle·Jaboca/Rowap·Focus Energy 열매에 아이템 팝업을 추가하고, Rough Skin에 `flushtextbox`와 대기를 넣고 `BattleScript_HurtAttackerNoMsg`를 쓰고, `COULDNTFULLYPROTECT` 주체를 `{B_DEF_…}`로 바꾼다.
- **HNS 현재 동작**:
  - HNS는 #9777 계열 "아이템 발동(Champions 이식)"을 자체 방식으로 이식했다(BATTLE_MESSAGE_OUTPUT_CHANGES.md "아이템 팝업 helper 연결"). 그런데 `BattleScript_CustapBerryActivation`(scripts_1:7686), `BattleScript_MicleBerryActivate`(7695), `BattleScript_JabocaRowapBerryActivates`(7707), `BattleScript_BerryFocusEnergy`(7432)에는 팝업이 없다. HNS가 의도적으로 뺀 것인지는 문서만으로 판단할 수 없다.
  - `BattleScript_RoughSkinActivates`(7015)는 메시지가 있는 `BattleScript_HurtAttacker`를 쓴다. upstream은 NoMsg 경로다. 메시지 출력이 달라진다.
  - `STRINGID_COULDNTFULLYPROTECT`(battle_message.c:801)는 이미 `{B_DEF_NAME_WITH_PREFIX}`다(이미 반영).
  - `waitabilitypopup`/`BS_WaitAbilityPopup`은 HNS에 없다.
- **보존 이유**: 지시서가 보존을 요구하는 "특성 팝업 순서", "아이템 발동 팝업"의 현재 HNS 동작을 직접 바꾼다.
- **선택지**:
  1. 보류한다.
  2. 부작용이 가장 적은 `waitabilitypopup` 인프라(명령과 `IsAnyAbilityPopUpActive` 공개)만 추가하고, 스크립트 삽입은 항목별로 사용자 승인을 받는다.
  3. 열매 4종 팝업 추가만 따로 승인받는다. Rough Skin 메시지 제거는 출력 변경이므로 별도 결정이 필요하다.

### 2-4. #10443 Beak Blast·Focus Punch·Shell Trap 상호작용 (upstream 5b83f3ed5f)

- **upstream 목적**: 앵콜로 끌려 나온 경우에도 충전 연출·효과가 나오게 한다(Champions 규칙 `B_MOVE_EFFECTS_BEFORE_MOVES`). 부리캐논 화상은 접촉기에만 걸리도록 `MoveEndBeakBlast`로 분리한다. 연출 주체를 `BS_SCRIPTING`으로 바꾸고 문자열 주체도 `{B_SCR_…}`로 바꾼다.
- **HNS 현재 동작**:
  - `src/battle_message.c:357 STRINGID_PKMNTIGHTENINGFOCUS`, `:770 STRINGID_HEATUPBEAK`, `:795 STRINGID_PREPARESHELLTRAP`은 모두 `{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}…`이다.
  - `src/battle_main.c:5411 TryDoMoveEffectsBeforeMoves()`, `data/battle_scripts_1.s:484/508/5485` SetUp 스크립트.
  - `src/battle_move_resolution.c:2185`: 부리캐논 화상에 접촉 조건이 없다(비접촉기에도 화상이 걸리는 버그).
- **보존 이유**: 한글 문자열의 `{B_...}` 지정 코드 변경은 HNS 우선 규칙에 걸린다. 스크립트를 `BS_SCRIPTING` 기준으로 바꾸면 문자열도 `{B_SCR_…}`로 바꿔야 올바른 이름이 나온다.
- **선택지**:
  1. **부리캐논 접촉 조건만** 따로 이식한다. 문자열 영향이 없으므로 안전하다.
  2. 앵콜 연동까지 이식하되 `gBattlerAttacker`를 해당 배틀러로 유지해 한글 `{B_ATK_…}`를 그대로 쓰는 절충안을 만든다. 테스트가 필요하다.
  3. upstream대로 한글 문자열 3개를 `{B_SCR_…}`로 바꾼다. 사용자 승인이 필요하다.

### (참고) #10432 FRLG 노인 문구

HNS와 무관으로 판정했다. 다만 이식하면 한글 `STRINGID_WALLYUSEDITEM`(battle_message.c:435 "민진은…")을 지우고 영문 `COMPOUND_STRING("The old man")`/`("WALLY")`를 넣게 되므로 이식 대상에서 뺄 것을 권장한다.

---

## 3. 선행 리팩터 목록 (HNS에 없는 1.16.0 계열, 이 그룹의 "선행 필요" 원인)

| upstream PR | 내용 | 이것이 막는 이 그룹 PR |
| --- | --- | --- |
| #8943 / #9885 | 12v12, 트레이너별 `gParties[]`, 구 파티 전역 변수 폐지 | #10415, #10536, #10568(→#10674), #10711, (#10608·#10662는 HNS와 무관) |
| #9529 | `generational_changes.h`를 `config_changes.h`로 이름 변경 | config를 추가하는 PR 전부(#10350, #10298, #10443). HNS 파일에 수동 추가로 대체 가능 |
| #9657 | BattleCalcValues 도입, BattleContext를 DamageContext로 이름 변경 | #10616 등 `ctx`/`cv` 명칭 차이 전반 |
| #9674 | Magic Bounce/Coat/Snatch 리팩터 | #10386 |
| #9730 / #9928 | Stat Change Refactor(`battle_stat_change.c`, `trymovestatchanges`) | #10350, #10569, #10631, #10687 |
| #9859 | MoveEnd에 CalcValue 구조체 | #10354와 moveend 계열 대부분(수동 이식 필요의 원인) |
| #9898 / #10175 | IsAnyTargetAffected USER_AND_ALLY, 아군 대상 실패 | #10388 |
| #9939 | 명중 판정을 canceler로 통합 | #10606, #10566 |
| #9249 | Sky Drop·광분 혼란 리팩터 | #10648, #10180(Sky Drop hunk) |
| #9494 | Move End 교체 큐(`redCardSwitched`) | #10217 |
| #9440 | `trainer_util.c` 통합 | #10463 |
| #10220 | 선공격 효과·대미지 계산을 canceler로 이동(HNS가 의도적으로 제외) | #10600, #10566 |
| #10362 | 애니·사운드·HP 갱신을 canceler로 이동, `datahpupdate` 1인자화 | #10431, #10488, #10566 (#10714는 이 PR의 회귀) |

## 3-1. 그룹 1 충돌 PR과의 STRINGID 교차 확인

그룹 1이 충돌로 판정한 PR이 건드리는 문자열·함수와 이 그룹 PR을 대조했다.

| 그룹 1 PR | 건드리는 STRINGID·함수 | 이 그룹에서 겹치는 PR |
| --- | --- | --- |
| #10144 | `STRINGID_ITDOESNTAFFECTTWOFOES` | 없음 |
| #10149 | `STRINGID_PKMNSXMADEITINEFFECTIVE`, `STRINGID_PKMNSXMADEYINEFFECTIVE` | 없음(이 그룹의 문자열 PR은 다른 ID를 쓴다) |
| #9799 | `BufferStringBattle`의 `STRINGID_INTROMSG/INTROSENDOUT/RETURNMON/SWITCHINMON`과 `sText_*` 다수 | **#10436**(같은 `STRINGID_INTROSENDOUT` 분기). #10436은 무관 판정이지만 #9799와 함께 보류해야 한다 |
| #10315 | 문자열 없음(Dragon Darts 적중 후 메시지 순서, battle_move_resolution.c·battle_script_commands.c) | **#10606** Dragon Darts(선행 필요). 같은 기술 경로라 함께 다룬다 |

이 그룹이 건드리는 STRINGID 전체: #10354(`PKMNANCHOREDITSELF`, `PKMNKNOCKEDOFF`, `PKMNANCHORSITSELFWITH`, `FELLSTRAIGHTDOWN`, `THESEAOFFIREDISAPPEARED`, `THESWAMPDISAPPEARED`), #10432(`WALLYUSEDITEM` 삭제), #10217(`PKMNSTOLEITEM`), #10268(`COULDNTFULLYPROTECT`), #10443(`PKMNTIGHTENINGFOCUS`, `HEATUPBEAK`, `PREPARESHELLTRAP`). 그룹 1 충돌 PR과 겹치는 ID는 없다.

## 4. 불확실·추가 검증 필요

- #10433: HNS의 구형 `activateOpportunist:2`·`eatMirrorHerb` 플래그가 턴 종료 단계에서 설정·소비되는 시점과 `TurnValuesCleanUp`의 초기화 순서가 upstream과 달라, 실제 전투 확인이 필요하다.
- #10483/#10577: 두 PR을 합친 `TryRestoreHeldItems` 조건에서 `B_RESTORE_HELD_BATTLE_ITEMS >= GEN_9 ||`가 빠진다. HNS 설정에서 "소모한 도구 복원" 동작이 달라질 수 있다. 확실히 안전한 것은 `TrySymbiosis` 조건 제거뿐이다.
- #10648: HNS 구형 Sky Drop(`skyDropTargets`)에 같은 버그가 있는지는 확인하지 못했다.
- #10661: HNS `CancelerSetTargets`→`CancelerPPDeduction` 순서에서도 `ClearDamageCalcResults()`가 대상 플래그를 지우는지는 논리상 같다고 보지만, 더블 배틀 실측이 필요하다.
- #10388: HNS에 `TARGET_USER_AND_ALLY` 처리가 부족해 선행 필요로 판정했다. 선행 PR(#9898, #10175)을 좁게 옮기면 데이터 변경만으로 해결될 수도 있다.
- 거대 PR(#10542, #10488, #10616, #10657)은 표본 hunk만 확인했다.
- 모든 판정은 코드 대조만 거쳤고, 빌드·자동 테스트·실제 게임 확인은 하지 않았다.
