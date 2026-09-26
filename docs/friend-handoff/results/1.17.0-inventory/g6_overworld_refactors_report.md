# g6_overworld_refactors — 1.17.0 1단계 인벤토리 보고서

- 대상: `g6_overworld_refactors.tsv`의 118개 PR. 구성은 1.15.2~1.15.3 Overworld Fixed 6개, 1.16.0 REFACTORS 22개, 1.16.x Overworld 46개, 1.17.0 REFACTORS 18개, 1.17.0 Overworld 26개다.
- 기준: HnS 스냅샷 `<HnS 작업트리>/` @ `0d89762071`, upstream `expansion/1.17.0`. merge-base는 `3efb836f72`(master 1.15.2 개발 계열)이다.
- 방법은 다음과 같다. 스냅샷과 upstream 저장소 모두 읽기만 했다.
  1. **hunk 단위 판정.** 스냅샷 파일 사본을 scratchpad에 두고 `patch --dry-run`을 정방향과 역방향으로 각각 돌렸다. 각 hunk는 F(정방향만 적용 = 미적용), R(역방향만 적용 = 이미 적용), X(둘 다 실패), ?(둘 다 적용) 중 하나로 분류했다.
  2. **신규 심볼 대조.** 각 PR이 새로 만든 식별자(merge-base에 없는 것)가 HnS에 있는지 확인했다.
  3. **선행 PR 자동 추출.** 1.16/1.17 upcoming PR 609개 전체를 대상으로, 각 PR의 사전 이미지(컨텍스트와 삭제 줄)에 **먼저 머지된 다른 PR이 만든 심볼**이 있으면 그 PR을 선행 PR로 기록했다. HnS에 없는 심볼만 셌다. 그다음 실제 코드와 대조했다.
- HnS 고유 사항 중 판정에 영향을 준 것:
  - `IS_HNS=1`, `IS_FRLG=0`.
  - 설정: `OW_FOLLOWERS_ENABLED TRUE`, `OW_ENABLE_DNS TRUE`, `FNPC_ENABLE_NPC_FOLLOWERS FALSE`, `OW_ROCK_CLIMB/DEFOG_FIELD_MOVE FALSE`, `P_*RELEARNER FALSE`, `POKEDEX_PLUS_HGSS TRUE`.
  - 파티 배열은 `gPlayerParty`/`gEnemyParty`다. #8943의 `gParties[B_TRAINER_*]`는 없다.
  - 1.16.0 upcoming 리팩터는 원작업자가 선별 이식한 것 외에는 **하나도 없다**.

## 요약

### 판정별 개수 (118)

| 판정 | 개수 | PR |
| --- | ---: | --- |
| 이미 적용 | 8 | #9269, #9878, #10163, #10195(다른 형태), #10230, #10313, #10417(다른 형태), #10430 |
| 부분 적용 | 3 | #9850, #10089, #10121 |
| 미적용·안전 이식 가능 | 12 | #9883, #9925, #9930, #8628, #9765, #9762, #9819, #9407(묶음), #10111, #10329, #10410, #10551 |
| 미적용·수동 적응 가능 | 16 | #9176, #8497, #9510, #9575, #9425, #9896, #8434, #10191, #10247, #10242, #10322, #10382, #10343, #7975, #10117, #10452 |
| 미적용·선행 필요 | 46 | 아래 표 참조. 대부분 배틀 리팩터 체인, #8434(OWE), #8678(g5 동적 trainerbattle), #9006(relearner), #9920(daily seed), MapHeader 체인에 걸린다 |
| 충돌 | 15 | #7305, #9655, #9475, #9680, #9006, #8930, #9730, #9461, #9920, #9927, #9518, #9147, #10548, #9335, #10521 |
| HNS와 무관 | 18 | #9905, #9903, #9991, #9557, #9549, #9707, #9774, #10020, #10100, #10281, #10445, #10550, #10557, #10540, #10652, #10080, #10176, #10320 |

### 이식 추천 상위 (작고 HnS 경로에 실제로 영향)

1. **#10191** — 정적 조우 포켓몬의 능력치가 개체값 반영 전에 계산되는 버그다.
   - HnS `src/pokemon.c:3634` `CreateEventMon`, `:3690-3692`에서 `CalculateMonStats` 뒤에 `SetBoxMonIVs`를 호출한다.
   - HnS의 `Route19_Cave_hns` 등에서 이 경로를 쓴다.
   - `gParties[B_TRAINER_OPPONENT_A]`를 `gEnemyParty`로 바꿔서 옮긴다.
2. **#9925** — 반사 팔레트 태그 충돌이다.
   - OW 포켓몬 팔레트 태그가 `species+0x4000(+0x2000 이로치)`여서 반사 오프셋 0x2000과 겹친다(`src/field_effect_helpers.c:21`).
   - HnS는 동반 포켓몬이 켜져 있다. 깨끗이 적용된다.
3. **#9883** — Trainer Hill 안에서 동반 포켓몬에게 말을 걸면 트레이너 스크립트가 선택되는 문제다(`src/field_control_avatar.c:442`). HnS는 Trainer Hill을 이식했고 동반 포켓몬도 켜져 있다. 깨끗이 적용된다.
4. **#10343** — 시간대(ToD) 블렌드가 매 프레임 재설정되는 문제다.
   - `UpdateTimeOfDay(bool32)` 시그니처가 바뀐다. 호출부는 `overworld.c:1904`, `field_weather.c:794`, `rtc.c:333`이다.
   - HnS는 DNS를 사용한다. 헤더 컨텍스트만 수동으로 맞추면 된다.
5. **#10322** — 날씨 컬러맵이 `BLEND_IMMUNE` 스프라이트 팔레트까지 물들이는 문제다. `money.c`의 include 컨텍스트만 수동으로 맞추면 된다.
6. **#10242** — 순결의부적 조우율 판정이 아이템 ID(`ITEM_CLEANSE_TAG`)로 되어 있어 정결의향로 같은 `HOLD_EFFECT_REPEL` 아이템이 무시된다(`src/wild_encounter.c:1334`, 1줄).
7. **#10247** — 얼룩 무늬(Spinda) 알에 반점이 그려지는 문제다.
   - HnS `src/decompress.c:1175`에 `&& !isEgg`만 추가하면 된다.
   - INCGFX와 반점 일반화 부분은 HnS와 무관하다.
8. **#10551** — 회전 이동이 끊길 때 동반 포켓몬이 크래시 나는 문제다. 1줄이며 깨끗이 적용된다.
9. **#10329 + #10529** — 접근 트레이너의 이동 타입 복원 시점을 수정한다(`src/trainer_see.c`).
   - #10329는 깨끗이 적용된다.
   - #10529는 모든 접근 트레이너에 해당하는 일반 수정이지만 #10329가 먼저 있어야 한다.
10. 저비용 정리 항목:
    - **#9765**: 도감 서식지 맵의 affine 템플릿을 제거한다. ROM이 약 5KB 줄어든다.
    - **#10111**: NULL goto/call에 assertf를 넣는다.
    - **#10233의 `asm/macros/event.inc:818` 오타 1줄**: `trainerBattle_flags` 때문에 playMusicB 플래그가 설정되지 않는다. 이 1줄은 #8678 없이도 옮길 수 있다.

### 충돌 목록 (상세는 아래 절)

- **배틀 메시지 최신화와 겹침.** HnS가 사용자 요청으로 바꾼 출력 경로를 upstream이 다른 형태로 다시 짠다.
  - #9655 Battle Messages: 방벽 제거 부분은 HnS가 같은 심볼로 이미 구현했다. 나머지는 충돌한다.
  - #9680 End Turn: 맹독구슬·화염구슬, 아이스바디, 하얀허브 스크립트를 제거하거나 재작성한다.
  - #9730 Stat Change Refactor: 176개 파일이며 1.16~1.17 PR 약 50개의 선행 조건이다.
- **HnS 고유 데이터·기능:**
  - #7305 나무열매 트리(`new_game.inc` IS_HNS 블록)
  - #9475 트레이너 사진 enum(`_HNS` 137개)
  - #8930·#9518 도감(조토 도감, HGSS 도감 수정)
  - #9147 문(HNS 문 43개)
  - #10548 비전기술 배지 해금(IS_HNS 매핑)
  - #9927 대량발생(HnS `tv.c` 수정, 세이브 구조)
- **세이브 구조:** #9920 daily seed. HnS는 `SaveBlock1`의 `unused_9C2` 자리를 `saveVersionMagic`으로 이미 사용한다.
- **한글 표시:**
  - #9461 맵 팝업 층수 표기와 폰트 자동 선택. 영문 "CELADON DEPT."/Rooftop과 B1F 형식이 들어간다.
  - #10521 기술·특성 설명 폰트 자동 축소. HnS Gen4 기술 정보 창에 영향이 있다.
  - #9006 relearner: 한글 신규 문자열 2개가 필요하고 HnS 맵 스크립트의 special 이름이 바뀐다.
- **스크립트 매크로:** #9335 코인·머니 명령 시그니처 변경. HnS `_hns` 스크립트에서 약 90곳을 마이그레이션해야 하고, 코인 창 좌표가 +1 이동한다.

### 용량 주의

| PR | 영향 | 근거 |
| --- | --- | --- |
| **#9211** Additional trainer slides | **큼, 약 +170KB ROM** | HnS `trainer_slide.o` .rodata가 이미 0x30dd8(200,152B)이다. `TRAINER_SLIDE_COUNT`가 14에서 26으로 늘면 `[DIFFICULTY][트레이너+파트너][COUNT]` 포인터 표가 비례해서 커진다. HnS는 슬라이드 문구를 하나도 정의하지 않으므로 이득이 없다. **ROM 여유 223KB를 거의 다 쓴다.** 참고로 현재 표 자체가 200KB를 쓰고 있어 별도 최적화 후보다. |
| #8434 Overworld Encounters | 큼(추정 15KB 이상 코드·데이터) | `wild_encounter_ow.c` 70KB 소스와 `event_object_movement.c` +595줄은 `WE_OW_ENCOUNTERS FALSE`여도 컴파일된다. EWRAM +1B. |
| #9927 Mass outbreak | 중간 | 신규 `mass_outbreak.c`, `debug.c` +700줄, 세이브블록 변경 |
| #9896 Random Mon Generation | 중간 | 신규 `random_mon_generation.c` 581줄 |
| #7305 gBerries / #9475 trainer pic / #9147 door | 중간 | 데이터 표 재구성(+795~+2956줄) |
| #9425 조건부 상점 | 작음(약 +3.8KB) | `struct ItemInfo`에 함수 포인터가 추가된다(아이템 약 950개 × 4B). EWRAM +4B. |
| #9080 night music | 작음(맵 헤더당 +2B, 약 1500개 맵) | #10159에서 다시 압축된다 |
| #9765 | **감소(약 -5KB)** | `pokedex_area_region_map.o` .rodata 0x1370 중 affine 그래픽 제거 |
| #8497 | 감소 추정 | `loadspritegfx` 약 2400줄 삭제. EWRAM +32B(태그 표) |

## 판정 표

범례: 고위험 = 지시서의 배틀 고위험 파일 포함 여부(TSV sens). 선행 PR 옆 `gN`은 인벤토리 그룹이다.

### 1.15.2~1.15.3 Overworld Fixed

| PR | 제목 | 판정 | 근거 | 고위험 | 용량 | 의존·비고 |
| --- | --- | --- | --- | --- | --- | --- |
| #9883 | Follower check before trainer hill | 미적용·안전 이식 가능 | HnS `field_control_avatar.c:442`가 아직 `InTrainerHill()`을 먼저 검사하고, 동반 포켓몬 분기는 `event_object_movement.c:3977`에 있다. 2 hunk 모두 F다. HnS는 Trainer Hill을 이식했고 동반 포켓몬이 켜져 있다. | - | 없음 | 추천 |
| #9905 | namebox flicker | HNS와 무관 | HnS는 speaker name을 쓰지 않는다(`SP_NAME_` NONE/MOM/PLAYER만 있고 맵 사용 0건). hunk는 F지만 `match_call.c`는 IS_HNS로 수정된 파일이다. | - | 작음 | #10540의 선행 |
| #9903 | relearner from PC summary | HNS와 무관 | HnS `pokemon_summary_screen.c` `Task_HandleInput`(1928~)은 재작성되어 있어 rename 경로도, START relearner 경로도 없다(START는 능력치 표시). | - | 없음 | #10445와 같은 부류 |
| #9925 | reflection palette collisions | 미적용·안전 이식 가능 | `field_effect_helpers.c:21`의 오프셋 0x2000이 OW 포켓몬 태그(`event_object_movement.c:2332`, +0x4000/+0x2000)와 충돌한다. `NUM_SPECIES`가 1573으로 0x800보다 작으므로 STATIC_ASSERT를 통과한다. | - | 없음 | 추천 |
| #9930 | FRLG player sprite fishing after surf | 미적용·안전 이식 가능 | 일반 수정이다(`LoadSheetGraphicsInfo`의 프레임 크기 비교, `event_object_movement.c:1957`). 두 번째 hunk는 `!OW_GFX_COMPRESS` 조건이라 HnS에서는 효과가 없다. | - | 없음 | 가치 낮음 |
| #9991 | On-step scripts after FNPC door task | HNS와 무관 | `FNPC_ENABLE_NPC_FOLLOWERS FALSE`이고 HnS 데이터에 setfollowernpc 사용이 없다. 적용해도 걸음마다 태스크 검색 1회가 늘 뿐이다. | - | 없음 | |

### 1.16.0 REFACTORS

| PR | 제목 | 판정 | 근거 | 고위험 | 용량 | 의존·비고 |
| --- | --- | --- | --- | --- | --- | --- |
| #9176 | Fling Refactor | 미적용·수동 적응 가능 | `MOVE_EFFECT_FLING`/`FLUNG_ITEM_*`이 없다. HnS에는 `BS_TryFlingHoldEffect`(`battle_script_commands.c:13501`)와 `CanFling(battler)`(`battle_util.h:299`)가 남아 있다. hunk는 모두 F이고, R 3개는 공백 정리뿐이다. | Y | 작음 | **선행 기반**: #9494, #9610, #9674, #9859, #9918, #10300, #10362, #10426, #9514(g3), #10309·#10325(g1), #10593(g3), #9466·#10181(g5). 맹독·화염구슬을 던질 때의 메시지는 HnS 구슬 메시지 변경과 대조가 필요하다. |
| #9446 | synchronize / cure berry timing | 미적용·선행 필요 | 신규 심볼 16개가 HnS에 전혀 없다. opcode 3개를 제거하고(UNUSED_31/32로 번호 보존) `trysynchronize`를 `BattleScript_UpdateEffectStatusIconRet`에 넣는다. | Y | 작음 | 선행: #9176, #9417(g3). 후행: #9249, #9730, #9918, #9928(g3). 싱크로 팝업 순서와 HnS `B_MSG_STATUSED_BY_ABILITY` 매핑(`battle_message.c:1205~`)을 대조해야 한다. |
| #9249 | Sky Drop / rampage confusion | 미적용·선행 필요 | `skyDropTargets`를 volatile로 옮기고 `CancelMultiTurnMoves`/`CanBeConfused`/`FaintClearSetData` 시그니처를 바꾼다. config `B_RAMPAGE_CANCELLING`이 `B_RAMPAGE_CONFUSION`으로 이름이 바뀐다. | Y | 작음 | 선행: #9446, #9358(g4). **후행 16개**: #9494, #9680, #9730, #9859, #8893·#9657·#9928·#10151·#10318(g3), #9784·#10288·#10295(g1), #10180·#10317·#10354·#10648(g2) |
| #8497 | Remove loadspritegfx | 미적용·수동 적응 가능(대형·기계적) | 1104 hunk F, X 8개. X는 모두 `battle_anim_scripts.s`에서 HnS가 수정한 10줄(#10324 파티클 팔레트 등) 부근이다. `migration_scripts/1.16/remove_loadspritegfx.py`로 재생성할 수 있다. | Y | 감소 추정 | 1.16 이후 upstream 신규 기술 애니메이션은 모두 `loadspritegfx` 없이 작성되어 있으므로 애니메이션 이식에 필요하다(#9172, #9595 g3). |
| #9510 | IsBattlerWeatherAffected | 미적용·수동 적응 가능 | `GetWeather()`를 추가하고 `IsBattlerWeatherAffected(holdEffect, weather, flags)`로 시그니처를 바꾼다. HnS에는 `GetAttackerWeather`(Champions/AI 이식)가 이미 있어 컨텍스트가 X다. | Y | 없음 | 소규모. 후행 의존은 적다. |
| #7305 | gBerries refactor | **충돌** | `new_game.inc:114` `#if IS_HNS` 조토·관동 나무열매 트리가 `setberrytree ..., ITEM_TO_BERRY(...)` 형식을 쓰는데, 이 PR이 매크로 인자를 berryId로 바꾼다. HnS `berry.c`는 merge-base 대비 +254줄 수정되어 있다. | Y | 중간 | 세이브 호환성(나무열매 인덱스) 확인이 필요하다. 후행: #10181(g5), #8434 |
| #9610 | Poltergeist/Fling pre-attack | 미적용·선행 필요 | `BattleScript_FlingMessage`/`PoltergeistMessage`를 추가하고 `battle_message.c` 4줄(한글 문자열 경로)을 바꾼다. | Y | 작음 | 선행: #9176. 후행: #9730, #10220, #10593(g3) |
| #9269 | Rework special trainer IDs | **이미 적용** | HnS `f9dfdaffbb`("Port upstream commit d32845cbee")에 들어 있다. `constants/trainers.h`는 1.17.0(#9964 후속 포함)과 동일하다. R 10개, X 2개는 후속 표기 차이다. | Y | 없음 | |
| #9494 | Queued switches (MoveEnd) | 미적용·선행 필요 | `QueuedSwitch`와 `MOVEEND_SPRAY_LEPPA_BLUNDER`/`SEND_OUT_REPLACEMENTS`를 추가한다. 신규 24개가 모두 없다. | Y | 작음 | 선행: #9176, #9249, #9417(g3). **후행 14개**: #9680, #9717, #9730, #9859, #9757(g4), #9784·#10161·#10231(g1), #9786·#9916·#10338(g3), #10217·#10600·#10682(g2). HnS 레드카드·탈출버튼 아이템 팝업(Champions 이식)과 대조가 필요하다. |
| #9655 | Refactor Battle Messages | **충돌** (일부 선반영) | 신규 68개 중 16개(방벽 제거 `B_MSG_BREAK_*`, `BattleScript_BreakScreens*`, `STRINGID_*WOREOFF`)는 HnS가 이미 같은 이름으로 구현했다(`battle_scripts_1.s:3797`). 나머지인 상태 회복·장판 제거·멘탈허브·`gHurtByStringIds`는 HnS 최신화와 겹친다. | Y | 작음 | 후행: #9674, #9680, #9730, #9714·#9777·#10151·#10471(g3). 상세는 충돌 절 |
| #9475 | Refactor trainer pic info | **충돌** | `enum TrainerPicID`와 트레이너 그래픽 표를 전면 개편한다. HnS `constants/trainers.h`에는 `_HNS` 항목이 137개 있다(Gold/목호 등). 신규 187개가 모두 없다. | Y | 중간 | 후행: #8943(g3) 일부 |
| #9674 | Magic Bounce/Coat/Snatch | 미적용·선행 필요 | `magicCoatPending`/`magicBouncePending`, `RestoreAttacker`를 추가한다. X 19개. | Y | 작음 | 선행: #9176, #9655, #9657(g3). 후행: #9859, #9918, #10220, #10311, #9784(g1), #10386(g2) |
| #9575 | Unify AI prediction | 미적용·수동 적응 가능 | `ComputeBattlerDecisions`를 `ComputeAiBattlerDecisions`로 바꾸고 `CanAiPredictMove`를 제거한다. HnS AI 선별 이식으로 X 3개가 생긴다. | Y | 없음 | 선행: #9596(g4) 컨텍스트 |
| #9680 | End Turn → BattleScriptCall | **충돌** | `BattleScript_ToxicOrb/FlameOrb`(HnS `battle_scripts_1.s:6020/6028`, 구슬 팝업·메시지 분리), `RainDishActivates`(:6261)가 `AbilityHpHeal`로, `IceBodyHeal`(:4537, 팝업 전용), `WhiteHerbEnd2`(:7263)를 제거하거나 재작성한다. | Y | 작음 | 선행: #9249, #9494, #9610, #9616(g3), #9655. 후행: #9211, #9717, #9730, #10311, #9777(g3) |
| #9006 | Move relearner refactor | **충돌** | 한글 `strings.c:1239~` 컨텍스트에 신규 문자열 2개(`gText_MoveRelearnerStop`, `...UseTm`)를 추가해야 한다. `HasMovesToRelearn`이 `Special_HasMoveToRelearn`으로 바뀌어 HnS `BlackthornCity_House3_hns/scripts.inc:135`에 영향을 준다. `pokemon_summary_screen.c`는 HnS가 재작성했다. | - | 감소(-500줄) | 후행: #7573, #9693, #9774, #10117, #10223 |
| #8930 | Automate regional dex orders | **충돌** | HnS `constants/pokedex.h`는 조토 도감(`enum JohtoDexOrder`, :1679)이 추가되어 +881줄이다. 이 PR은 자동 생성과 마이그레이션으로 표를 다시 만든다. | - | 감소 | |
| #9717 | Queued switches (EndTurn) | 미적용·선행 필요 | `IsBattlerPresent`, `ENDTURN_SEND_OUT_REPLACEMENTS_*`, `BattleScript_EjectItemActivates` | Y | 작음 | 선행: #9494, #9680. 후행: #9211, #10426, #10433(g2) |
| #9751 | AreMultiPartiesFullTeams tests | 미적용·선행 필요 | 대부분 테스트다. `AreMultiPartiesFullTeams`/`B_MULTI_HALF_TEAMS`에 의존한다. | Y | 없음 | 선행: #8943(g3). 후행: #10039(g1) |
| #9847 | ShouldSwitch → SwitchContext | 미적용·선행 필요 | `struct SwitchAiContext`를 도입한다. X 27개. | Y | 감소 | 선행: #8472·#9124·#9451·#9460·#9462·#9551·#9568(g4 AI), #8943. 후행: #9857·#10277·#10626(g4) |
| #9859 | CalcValue struct to MoveEnd | 미적용·선행 필요 | `battle_move_resolution.c` 455줄 교체. X 67개. | Y | 없음 | 선행: #9176, #9249, #9417, #9494, #9657, #9674, #9784(g1) |
| #9730 | Stat Change Refactor | **충돌** (+선행 필요) | 176개 파일 +5762/-6809. `SetStatChange`, `MOVE_EFFECT_STAT_MINUS` 등 신규 269개 중 HnS에 있는 것은 3개뿐이다. HnS의 능력치 상승·하락 메시지 최신화, 하얀허브·미러허브 팝업(Champions 이식)과 겹친다. | Y | 감소 추정 | 선행: #9176, #9249, #9446, #9494, #9610, #9655, #9680, #8943, #9168(g3) 외. **후행 약 50개**(g1 12개, g2 11개, g3 16개, g4 3개, g5 3개, g6 6개). 하위 그룹은 이 PR을 사실상의 1.16 배틀 기준선으로 봐야 한다. |
| #9918 | Customizeable Pledge Moves | 미적용·선행 필요 | `pledgeState`/`PledgeCombo`를 도입하고 `setpledge` 등을 제거한다. HnS는 #8647(g4)을 적응 이식해 컨텍스트가 다르다. | Y | 작음 | 선행: #9176, #9446, #9494, #9674, #9730. 후행: #10220, #10300, #10426, #10217·#10542(g2), #10309(g1), #10384·#10593(g3), #10162·#10181(g5) |

### 1.16.0 Overworld

| PR | 제목 | 판정 | 근거 | 고위험 | 용량 | 의존·비고 |
| --- | --- | --- | --- | --- | --- | --- |
| #9557 | Improved FRLG Map Previews | HNS와 무관 | HnS `map_preview_screen.c`는 `#if IS_FRLG`(18~602)이고, hns 빌드는 `IS_FRLG=0`이다. 문서 이미지 34개도 없다. | - | 없음(HnS) | #10080·#7573의 선행 |
| #9425 | conditional shop items | 미적용·수동 적응 가능(선택 기능) | HnS `shop.c`가 Kurt·BP 상점과 나무열매 아이콘으로 크게 수정되어 `CB2_InitBuyMenu`/`BuyMenuFreeMemory` hunk가 X다. `data/text/mart_clerk.inc`(한글)를 `data/scripts`로 옮긴다. | - | 작음(+약 3.8KB) | 쓰지 않으면 이득이 없어 비권장 |
| #7573 | tryspecialevo rework | 미적용·선행 필요 | HnS에서 `tryspecialevo` 사용이 0건이다. `chooseboxmon.c` X 5개. | - | 작음 | 선행: #9006, #8943, #9557 컨텍스트 |
| #9461 | floor number in map popup | **충돌** | 팝업 폰트를 `FONT_NARROW`에서 `GetFontIdToFit(FONT_NORMAL)`로 바꾼다(HnS `map_name_popup.c:771` 한글 팝업). 영문 "CELADON DEPT."와 `gText_Rooftop`, `B1F/1F` 표기가 들어가고 `MapHeader.filler_18`을 `floorNumber`로 쓴다. HnS mapjson은 `hns` 분기(`tools/mapjson/mapjson.cpp:177`)가 따로 있다. | - | 작음 | MapHeader 체인: #9461 → #7975 → #9080 → #10159 → #10167 |
| #8628 | setmetatileinrange | 미적용·안전 이식 가능 | callnative 스크립트 명령의 순수 추가다. 필요한 매크로(`SWAP`, `MAPGRID_ELEVATION_SHIFT`)가 HnS에 있다. | - | 작음 | 쓸 때만 의미가 있다 |
| #9920 | Basic daily seed | **충돌** | HnS `include/global.h:1228`에서 `unused_9C2`를 이미 `saveVersionMagic`(HnS 세이브 버전 시스템)으로 쓴다. `dailySeed`의 위치를 새로 정해야 한다. | - | 없음 | 후행: #9877, #9955, #10012, #9927, #10050(g5) |
| #9877 | Dynamic Weather | 미적용·선행 필요 | HnS `constants/weather.h`의 `WEATHER_LEAVES=23`이 `WEATHER_DYNAMIC=23`과 번호가 겹친다. `dailySeed`가 필요하다. | - | 작음 | 선행: #9920. 후행: #10383 |
| #9896 | Random Mon Generation | 미적용·수동 적응 가능(선택 기능) | 신규 파일 위주이며 `script_pokemon_util.c` X가 있다. HnS 자체 랜더마이저(tertu 이식)와는 별개 기능이다. | - | 중간 | 후행: #9970, #9969, #10121 일부, #10127(g5) |
| #9765 | pokedex area map template 제거 | 미적용·안전 이식 가능 | hunk 모두 F. 미사용 affine 그래픽을 제거한다. 호출부 3곳이 모두 `pokedex_area_screen.c`에 있다. | - | **감소 약 5KB** | 추천(ROM) |
| #9762 | mail duplicate code | 미적용·안전 이식 가능 | HnS `GiveMailToMonByItemId`(`mail_data.c:74-75`)가 이미 MAIL/HELD_ITEM을 설정하므로 중복 제거가 안전하다. | - | 감소 | |
| #9693 | Remove relearner from party menu | 미적용·선행 필요 | HnS `P_PARTY_MOVE_RELEARNER FALSE`. `party_menu.c`에 HnS 수정(선택 재정렬 등)이 있어 X 4개다. | - | 감소 | 선행: #9006 |
| #9819 | rock climb/defog always defined | 미적용·안전 이식 가능 | HnS 두 config가 FALSE다. 적용 후 바위오르기 타일 상호작용은 config로 막힌다. HnS에서 바위오르기는 기술머신(`Route49_hns`)이라 필드 이동 타일 영향이 없는 것으로 판단했다. | - | 없음 | #10548의 선행 |
| #9850 | ow_abilities cleanup/tests | **부분 적용** | #9878 이식 때 `include/ow_abilities.h`와 `DoesLeadingMonHaveAbilityEffect`가 이미 들어왔다. HnS는 `ow_synchronize.h`와 자체 `IsSynchronizeActive`(`b9d05d5cbc` synchronize setting)를 유지한다. 남은 것은 헤더 통합과 테스트다. | - | 없음 | HnS 싱크로 설정은 보존해야 한다 |
| #8434 | Overworld Encounters | 미적용·수동 적응 가능(선택 기능·비권장) | 대형 신규 기능이다. `WE_OW_ENCOUNTERS FALSE`가 기본이지만 코드는 컴파일된다. `event_object_movement.c`(HnS +457줄 수정)에 X가 있다. | Y | **큼** | 후행(OWE 전용): #9910, #9968, #10066, #10076, #10020, #9966, #10096, #9927, #9955, #9970, #10121, #8678·#9879·#10051(g5) 일부 |
| #9407 | Fade BG and sprites simultaneously | 미적용·안전 이식 가능(묶음) | HnS `palette.c`는 merge-base 이후 수정되지 않았고 4 hunk 모두 F다. #10573이 이 동작을 `FadeSelectedPals` 한정 opt-in으로 되돌리므로 **#9407+#9549+#9707+#10573을 한 번에** 옮기거나 모두 보류한다. | - | 작음 | 가치 낮음 |
| #9549 | blend-immune sprite fade | HNS와 무관 | #9407 회귀 수정이다(`copyPalettes` u16→u32). HnS 교대 페이드 코드에는 해당 결함이 없다. | - | 없음 | #9407과 묶음 |
| #9707 | reset objPaletteToggle | HNS와 무관 | #9407 이후 회귀 수정이다. HnS 교대 페이드에서는 toggle이 0으로 끝난다(`palette.c` 330~). 적용해도 무해하다. | - | 없음 | #9407과 묶음 |
| #9774 | relearner NPC cancel | HNS와 무관 | #9006 회귀 수정이다. HnS `move_relearner.c:602/636`은 `VAR_0x8004`를 TRUE/FALSE로 설정하고, HnS Blackthorn 스크립트도 그 값을 쓴다. | - | 없음 | #9006을 이식하면 `BlackthornCity_House3_hns:144`도 같이 고쳐야 한다 |
| #9910 | GetAvailableObjectEventId by value | 미적용·선행 필요 | `wild_encounter_ow.*`가 없다. | - | 없음 | 선행: #8434 |
| #9968 | OWE despawn | 미적용·선행 필요 | OWE 전용 | - | 없음 | 선행: #8434, #9879(g5) |
| #10020 | DetermineFollowerNPCDirection check | HNS와 무관 | #8434 회귀 수정이다. HnS `follower_npc.c:1366`은 구현이 달라 같은 좌표에서 `DIR_NONE`을 이미 반환한다. | - | 없음 | |
| #10066 | Generated OWEs garbage data | 미적용·선행 필요 | `wild_encounter_ow.c`만 수정한다. | - | 없음 | 선행: #8434 |

### 1.16.1~1.16.4 Overworld

| PR | 제목 | 판정 | 근거 | 고위험 | 용량 | 의존·비고 |
| --- | --- | --- | --- | --- | --- | --- |
| #10111 | assertf for NULL goto/call | 미적용·안전 이식 가능 | HnS `script.c:44/122`, `scrcmd.c:98`에 `gNullScriptPtr`이 그대로 있다. assertf 인프라도 있다. | - | 없음 | 추천(저비용) |
| #10100 | colosseum warps frlg | HNS와 무관 | `cable_club_frlg.inc`, FRLG 전용 | - | 없음 | |
| #10191 | event mon stats without IVs | 미적용·수동 적응 가능 | HnS `pokemon.c:3634-3641` `CreateEventMon`은 `CalculateMonStats` 뒤에 `SetBoxMonIVs`(:3691)를 부르고 재계산하지 않는다. 버그가 그대로 있다. `gParties`를 `gEnemyParty`로 바꿔야 한다. | - | 없음 | **추천 1순위** |
| #10195 | icon tags eggs/female | **이미 적용(다른 형태)** | HnS `28d1a1c0bf`의 `GetMonIconListKey()`(`pokemon_storage_system.c:5277`)가 이로치 아닌 암컷·알 비트를 키로 쓰고, 생성과 해제 양쪽에 적용한다. | - | 없음 | |
| #10247 | spots incgfx + isEgg | 미적용·수동 적응 가능 | 반점 일반화(#9594 g4)와 INCGFX는 HnS와 무관하다. 핵심은 `decompress.c:1175` 알 검사 누락이며 1줄 적응으로 끝난다. | - | 없음 | 추천 |
| #10242 | hold_effect_repel | 미적용·수동 적응 가능 | `wild_encounter.c:1334`가 `ITEM_CLEANSE_TAG`를 직접 비교한다. `gPlayerParty`로 적응하면 된다. | - | 없음 | 추천 |
| #10281 | FRLG maniac spritesheet | HNS와 무관 | FRLG OW, INCGFX 표기 | - | 없음 | |
| #10230 | GetRandomDifferentSpeciesSeenByPlayer | **이미 적용** | HnS `2d4fe60efd`(#10230) | - | 없음 | |
| #10313 | flicker box→party | **이미 적용** | HnS `pokemon_storage_system.c:4849` `CreatePartyMonSprite`, :10288 호출 | - | 없음 | |
| #10322 | weather blend BLEND_IMMUNE | 미적용·수동 적응 가능 | `field_weather.c` 6 hunk F. `money.c`는 include 컨텍스트만 다르다(X). | - | 없음 | 추천(DNS·날씨) |
| #10329 | SEE_ALL_DIRECTIONS facing | 미적용·안전 이식 가능 | `trainer_see.c` 5 hunk F. HnS 맵은 SEE_ALL 트레이너를 쓰지 않지만 #10529의 선행이다. | - | 없음 | #10529와 묶음 |
| #10382 | debug time scripts followers | 미적용·수동 적응 가능 | 디버그 전용이다. HnS `debug.inc` 첫 hunk가 X다. | - | 없음 | 가치 낮음 |
| #10343 | tod blend every frame | 미적용·수동 적응 가능 | HnS에 `UpdateTimeOfDay(void)`와 호출부 3곳(`overworld.c:1904`, `field_weather.c:794`, `rtc.c:333`)이 있다. `overworld.h`는 컨텍스트만 X다. | - | 없음 | 추천(DNS) |
| #10417 | chooseboxmon blending | **이미 적용(다른 형태)** | HnS `pokemon_storage_system.c:2105`가 MOVE_ITEMS/SELECT_MON/MOVE_MONS/DEPOSIT/WITHDRAW 전부에 블렌드를 설정한다. | - | 없음 | |
| #10410 | assert trainer script needs obj | 미적용·안전 이식 가능 | `battle_setup.c:1528` `SetTrainerFacingDirection`에 assertf를 추가한다. F. | - | 없음 | |
| #10445 | Box ID var relearner from summary | HNS와 무관 | HnS 요약 화면에는 `HandleMoveRelearnerInput`이 없다. | - | 없음 | |
| #10550 | signmsg/normalmsg | HNS와 무관 | HnS에서 signmsg를 쓰는 곳은 FRLG 맵과 `move_tutors_frlg.inc`뿐이다. 적용해도 무해하다. | - | 없음 | |
| #10551 | follower crash spin | 미적용·안전 이식 가능 | `field_player_avatar.c` `ForcedMovement_None` 1줄. F. | - | 없음 | 추천 |
| #10573 | Rework simultaneous fade | 미적용·선행 필요 | `palette.c` X 1개 | - | 작음 | 선행: #9407(묶음) |
| #10557 | bard text speed | HNS와 무관 | 모빌 음유시인 전용이다. `text.c` `RunTextPrinters`의 공통 콜백 변경은 한글 텍스트 경로라 필요가 없으면 적용하지 않는 것을 권한다. | - | 없음 | |
| #10540 | non-existing speaker box | HNS와 무관 | #9905 후속(`PrepareNamebox`)이며 네임박스를 쓰지 않는다. | - | 없음 | |
| #10529 | movement type between move and face | 미적용·선행 필요 | 접근 트레이너 전반에 해당하는 일반 수정이다. | - | 없음 | 선행: #10329 |
| #10652 | talking to buried npc | HNS와 무관 | HnS 맵에서 `MOVEMENT_TYPE_BURIED` 사용이 0건이다. hunk는 F이며 적용해도 무해하다. | - | 없음 | |

### 1.17.0 REFACTORS

| PR | 제목 | 판정 | 근거 | 고위험 | 용량 | 의존·비고 |
| --- | --- | --- | --- | --- | --- | --- |
| #9927 | Mass outbreak | **충돌** (+선행) | HnS는 바닐라 TV 대량발생(`tv.c:1578/1699`)을 쓰고 `tv.c`에 HnS 수정(:869 `LAYOUT_VERSION_HNS`, :1519 프런티어 마트)이 있다. `global.tv.h`/세이브블록도 바뀐다. | - | 중간 | 선행: #8434, #9920, #9890·#9713(g5), #8943. 후행: #10089, #10121, #9969, #10012, #10178(g5) |
| #9211 | Additional trainer slides | 미적용·선행 필요 (**비권장**) | HnS는 슬라이드 문구가 0개다. `battle_script_commands.c` +699줄. | Y | **큼(약 +170KB)** | 선행: #9494, #9680, #9717. 후행: #10220, #10330, #10624(g2) |
| #9518 | Consolidation of Pokédex Files | **충돌** | HnS `pokedex_plus_hgss.c`는 merge-base 대비 +446줄 수정되어 있다(도감 버그 수정 `0b1b6bfb09`, `148bed2b2d`, `8615258d76`, SV 스키핑 이식). 이 PR은 -7814줄 통합이다. X 11개. | - | 감소 | 후행: #10172, #10335(g5) |
| #9878 | Rework egg creation/hatching | **이미 적용** | 원작업자가 이식했다(SESSION_LOG 2026-09-15). 신규 67개 중 60개가 있고 R 32개다. X는 `gPlayerParty`와 HnS 부화 요구사항에 맞춘 적응분이다. | - | - | |
| #10163 | Consolidate confuse flavor | **이미 적용** | `HOLD_EFFECT_CONFUSE_FLAVOR`(`hold_effects.h:16`, `battle_ai_switch.c:1868`). 남은 것은 주석 1줄뿐이다. | Y | - | |
| #9147 | Door Size Refactor | **충돌** | `field_door.c` +795줄 재구성. HnS 문 항목이 43개 있다(`35e28cec3b` 마호가니 문, `a65d895f20` 용의굴 사당). X 8개. | - | 중간 | |
| #10220 | pre-attack/damage calc to canceler | 미적용·선행 필요 | SESSION_LOG 2026-09-15: "최신 `CancelerPreAttackMoveEffect` 구조는 … 기계적으로 덮어쓰지 않았다". 신규 22개가 모두 없다. | Y | 작음 | 선행: #9610, #9674, #9717, #9730, #9211, #9249, #9657·#9008(g3), #9532(g1). 후행: #10311, #10326, #10330, #10362, #10426, #10404(g5), #10595(g3) |
| #9440 | FillPartnerParty unify | 미적용·선행 필요 | `trainer_util.c`/`TrainerGenerator`를 신설한다. HnS 파티 생성(`trainers_hns.party`, 랜더마이저) 컨텍스트 때문에 X 18개다. | Y | 작음 | 선행: #8943, #10051(g5), #10059(g1). 후행: #10463(g2), #10523(g5) |
| #9970 | More mon generation cleanup | 미적용·선행 필요 | `PokemonTemplate`/`CreateMonFromTemplate` | - | 중간 | 선행: #9896, #8434, #8678. 후행: #10320, #10408, #10121 |
| #10299 | MOVE_EFFECT_RANDOM_FROM_LIST | 미적용·선행 필요 | Tri Attack과 Dire Claw 효과를 통합한다. | Y | 없음 | 선행: #9730, #9918, #9514(g3). 후행: #10426, #10593(g3) |
| #10311 | Remove end2 | 미적용·선행 필요 | `B_SCR_OP_END2`를 `UNUSED_39`로 바꾸고 `*End2` 스크립트 이름을 변경한다. HnS 스크립트의 end2 사용처를 전부 바꿔야 한다. | Y | 없음 | 선행: #9680, #9674, #10220, #9008·#9777·#10151(g3) |
| #10300 | dynamic move category / Present | 미적용·선행 필요 | `BattleScript_PresentHealTarget`을 `BattleScript_HealTarget`으로 바꾼다. HnS는 이 라벨을 5곳에서 쓴다(`battle_scripts_1.s:2166/2649/3345/3352`, `battle_script_commands.c:9365`, 메가솔 연결 포함, SESSION_LOG 2026-09-15). | Y | 작음 | 선행: #9176, #9918, #10220, #9657·#10151(g3). 후행: #10362, #8647·#10277(g4), #10488(g2) |
| #10330 | TargetFailure accuracy | 미적용·선행 필요 | `enum TargetFailure` | - | 작음 | 선행: #9730, #10220, #9939(g3), #9211 |
| #10326 | Battle Terrain Refactor | 미적용·선행 필요 | 필드 타이머 u16→u8, `jumpifterrain` opcode | Y | 감소 | 선행: #9730, #9680, #9446, #9124(g4), #9777(g3). 후행: #10440(g2), #10454·#10593(g3) |
| #10362 | moveanim/hitanim/hpupdate to Canceler | 미적용·선행 필요 | HnS에 `MOVE_RESULT_*_EFFECTIVE` 4개(Champions 배율 이식)만 있고 나머지 39개는 없다. 피격 애니메이션과 HP 갱신 순서가 HnS 아이템 팝업·생명의구슬 메시지 순서와 겹칠 수 있다. | Y | 작음 | 선행: #10220, #9730, #9610, #9655, #9777(g3). 후행: #10431(g2) |
| #10426 | absorb / SetMoveEffect signature | 미적용·선행 필요 | SESSION_LOG: 전면 리팩터를 이식하지 않았다. `MOVE_EFFECT_ABSORB` 등 신규 26개가 모두 없다. **지시서가 "#10426 흡수 리팩터링 반영"이라고 적은 것과 다르다.** 동작 일부만 반영되었을 가능성이 있다. | Y | 작음 | 선행: #9176, #9717, #9730, #9918, #10299, #9680, #9514(g3). 후행: #10593·#10595(g3) |
| #10548 | Minor field move fixes | **충돌** | `FieldMoveInfo`를 `unlockType`/`arg` 방식으로 바꾼다. HnS `field_move.c:21-80`은 `IS_HNS` 분기로 배지 매핑을 따로 둔다(바위깨기=배지1 등). `party_menu.c` X. | - | 없음 | 선행: #9819 |
| #10121 | Another debug menu refactor | **부분 적용** | 원작업자가 기능 단위로 이식했다(SESSION_LOG 2026-09-16). 공통 `DebugSelection`/`DebugAction_Selection_StepUpdate`는 이식하지 않았다(STATUS.md:1293). | - | 중간(디버그) | 남은 부분의 선행: #9927, #9969, #9896, #8434 |

### 1.17.0 Overworld

| PR | 제목 | 판정 | 근거 | 고위험 | 용량 | 의존·비고 |
| --- | --- | --- | --- | --- | --- | --- |
| #7975 | writeSpecialVarIsEffect map flag | 미적용·수동 적응 가능 | HnS에 `BattleFrontier_BattleDome*_hns` 맵이 따로 있으므로 `_hns` 돔 맵 json과 PreBattleRoom 스크립트에도 반영해야 한다. mapjson은 `hns` 분기에 추가한다(X). | - | 없음 | MapHeader 체인 |
| #10012 | debug "Redo daily Events" | 미적용·선행 필요 | 디버그 | - | 없음 | 선행: #9920, #9927 |
| #10080 | disable map name in preview | HNS와 무관 | FRLG 맵 프리뷰 | - | 없음 | 선행: #9557 |
| #9955 | daily seed for lottery/mirage | 미적용·선행 필요 | 에메랄드 복권과 환상의섬 | - | 없음 | 선행: #9920 |
| #9080 | night music | 미적용·선행 필요 | `START_MUS/END_MUS` 범위를 HnS 음악 ID(`songs.h` X)에 맞춰야 한다. 선택 기능이다. | - | 작음 | 선행: #9461, #7975. 후행: #10159, #10167, #10176 |
| #10117 | hide contest data config | 미적용·수동 적응 가능(선택) | **upstream 1.17.0 버그**: `PSS_PAGE_COUNT - (C_HIDE_CONTEST_DATA) ? 2 : 1`은 연산자 우선순위 때문에 항상 2가 되어, config가 FALSE여도 콘테스트 페이지가 숨겨진다. 이식하려면 괄호를 고쳐야 한다. | - | 없음 | relearner 부분의 선행: #9006 |
| #10223 | relearner selection in scene | 미적용·선행 필요 | X 17개 | - | 작음 | 선행: #9006, #10117 |
| #9969 | debug randomizer actions | 미적용·선행 필요 | 디버그 | - | 작음 | 선행: #9896, #9927 |
| #10452 | Pokémon icons in dynmultichoice | 미적용·수동 적응 가능(선택) | `constants/script_menu.h`, `pokemon_icon.c`(#10121 이식분) 컨텍스트가 X다. | - | 작음 | |
| #10430 | keep candy use menu open | **이미 적용** | SESSION_LOG 2026-09-15 Items 이식. R 2개. | - | - | |
| #10096 | overworld_ascending_frames in tables | 미적용·선행 필요 | 표 매크로 정리다. OWE 반짝임과 Kanto OW 이름(#9537 g5)에 의존한다. | - | 없음 | 선행: #8434, #9537(g5) |
| #9966 | vanilla ow mons var species | 미적용·선행 필요 | `wild_encounter_ow.c`만 수정 | - | 감소 | 선행: #8434 |
| #10076 | GetDirectionToFace | 미적용·선행 필요 | `DetermineObjectEventDirectionFromObject`는 #8434가 도입했다. | - | 없음 | 선행: #8434 |
| #10159 | Optimize map header structure | 미적용·선행 필요 | merge 커밋 `7b7972ea6d`다. `cave`를 비트필드로 옮기고 `requires_flash`를 플래그 매크로로 옮긴다. HnS mapjson `hns` 분기 수정이 필요하다. | - | 감소(헤더당 2B) | 선행: #9461, #7975, #9080 |
| #10167 | NightSong → night_music | 미적용·선행 필요 | mapjson 키 이름 변경 | - | 없음 | 선행: #9080 |
| #10172 | pokedex_common.h | 미적용·선행 필요 | | - | 없음 | 선행: #9518 |
| #9335 | Coins and money script commands | **충돌** | `hidecoinsbox`/`updatecoinsbox`/`hidemoneybox`/`updatemoneybox`의 인자를 제거하고 `checkmoney` 등의 disable 인자를 없앤다. HnS 스크립트에서 `updatecoinsbox N, N` 17회, `hidecoinsbox N, N` 8회, `checkmoney N, N` 7회, `updatemoneybox N` 9회 등이 쓰인다. `ShowCoinsWindow`는 x+1, y+1로 좌표가 바뀐다(HnS `coins.c:27`). | - | 없음 | 마이그레이션 스크립트가 있다. 후행: #10407 |
| #10383 | overworld weather enum | 미적용·선행 필요 | HnS `WEATHER_LEAVES=23`을 보존해야 한다. | - | 없음 | 선행: #9877 |
| #10521 | FontIdToFit: move/ability desc | **충돌** | 요약 화면(`PrintMonAbilityDescription` :3781 등), relearner, 배틀 기술 설명(`battle_controller_player.c:1751`, HnS Gen4 기술 정보 창·L/R 전환)에 폰트 자동 축소를 적용한다. hunk는 F다. | Y(controller) | 없음 | 한글 폰트 대체 규칙을 확인하기 전에는 보류한다 |
| #10089 | debug trainer/flags submenu | **부분 적용** | HnS `debug.c:743-744`는 이미 서브메뉴 배열을 넘긴다. 남은 것은 `tStrain data[6]→[7]`(:5203)과 outbreak enum 쉼표다. | - | 없음 | 선행(나머지): #9927 |
| #10176 | Remove clock-based music transition | HNS와 무관 | #9080 회귀 수정이다. HnS에는 해당 줄이 없다(rev=ok). | - | 없음 | |
| #10233 | dynamic trainer script regression | 미적용·선행 필요 (일부 독립) | `battle_setup.c` hunk는 #8678(g5)의 `EventSnippet` 구조를 전제로 한다. 다만 `asm/macros/event.inc:818`의 `trainerBattle_flags` 오타는 HnS에도 있어 1줄을 독립적으로 옮길 수 있다. | - | 없음 | 선행: #8678(g5) |
| #10320 | givemon not setting pp | HNS와 무관 | #9970 회귀 수정이다. HnS에는 `CreateMonFromTemplate`이 없다. | - | 없음 | |
| #10339 | trainerbattle flag check | 미적용·선행 필요 | `trainerbattle` 매크로에 `skipFlagCheck` 인자를 추가한다. HnS 매크로는 #8678 이전 형태다(`event.inc:812`, `type` 인자). | Y | 없음 | 선행: #8678(g5) |
| #10407 | updatemoneybox one-byte | 미적용·선행 필요 | | - | 없음 | 선행: #9335 |
| #10408 | ignore total EV check | 미적용·선행 필요 | `PokemonTemplate.ignoreTotalEvCheck` | - | 없음 | 선행: #9970, #9955(config 컨텍스트) |
| #10462 | facing player snippet param | 미적용·선행 필요 | `EventSnippet_*`/`PUSH_IF_SET` 구조가 HnS에 없다. | - | 없음 | 선행: #8678(g5) |

## REFACTORS 선행 관계 요약 (다른 그룹 참조용)

아래 "후행"은 1.16/1.17 upcoming PR 609개에서 자동으로 추출했다. 해당 PR의 컨텍스트나 삭제 줄에 이 리팩터가 만든 심볼이 2개 이상 등장하는 경우다.

| 리팩터 | 바꾸는 파일·API | 이 리팩터를 전제로 하는 다른 그룹 PR |
| --- | --- | --- |
| #9730 Stat Change | `battle_script_commands.c`/`battle_util.c`/`battle_scripts_1.s` 전반. `SetStatChange`, `MOVE_EFFECT_STAT_PLUS/MINUS`, `GetStatStage`, `Cmd_trybattlerstatchange` 등 | g1: #9957, #9972, #9983, #9988, #10032, #10074, #10077, #10088, #10091, #10185 / g2: #10350, #10354, #10488, #10542, #10569, #10600, #10631, #10687, #10707 / g3: #8893, #9008, #9777, #9928, #9939, #9989, #10057, #10145, #10151, #10416, #10454, #10459, #10471, #10593, #10666 / g4: #9857, #10285, #10342, #10464 / g5: #10058, #10127, #10179 |
| #9249 Sky Drop/rampage | `CancelMultiTurnMoves(battler)`, `CanBeConfused(atk, eff)`, `FaintClearSetData`→void, volatile Sky Drop, `B_RAMPAGE_CONFUSION` | g1: #9784, #10288, #10295 / g2: #10180, #10317, #10354, #10648 / g3: #8893, #9657, #9928, #10151, #10318 |
| #9494 Queued switches | `enum QueuedSwitch`, `MOVEEND_SEND_OUT_REPLACEMENTS`, `redCardSwitched`, `sprayLeppaBlunder` | g1: #9784, #10161, #10231 / g2: #10217, #10600, #10682 / g3: #9786, #9916, #10338 / g4: #9757 |
| #9176 Fling | `MOVE_EFFECT_FLING`, `FLUNG_ITEM_*`, `CanFling(atk, ability)` | g1: #10309, #10325 / g3: #9514, #10593 / g5: #9466, #10181 |
| #9918 Pledge | `PledgeCombo`/`pledgeState`, `setpledge` 제거 | g1: #10309 / g2: #10217, #10542 / g3: #10384, #10593 / g5: #10162, #10181 |
| #9655 Battle Messages | `gCureStatusStringIds`, `gPartyCureStatusStringIds`, `gHurtByStringIds`, `gRemoveHazardsStringIds`, 멘탈허브 스크립트 | g3: #9714, #9777, #10151, #10471 |
| #10220 Canceler | `CancelerPreAttackMoveEffect`, `CancelerDamageCalc`, `CANCELER_RESULT_*` | g3: #10595 / g5: #10404 |
| #9446 Synchronize | `trysynchronize`, `SynchronizeState`, opcode 3개 제거 | g3: #9928, #10593 / g5: #10181 |
| #9680 End Turn | `endturnevents`, `*End2` 스크립트 제거 | g3: #9777 |
| #9674 Magic Bounce | `magicCoatPending`/`magicBouncePending` | g1: #9784 / g2: #10386 |
| #9610 | `BattleScript_FlingMessage`/`PoltergeistMessage` | g3: #10593 |
| #9717 | `IsBattlerPresent`, `ENDTURN_SEND_OUT_REPLACEMENTS_*` | g2: #10433 |
| #10299 / #10426 | `MOVE_EFFECT_RANDOM_FROM_LIST` / `SetMoveEffect` 시그니처, `MOVE_EFFECT_ABSORB` | g3: #10593, #10595 |
| #10300 | `BattleScript_HealTarget`, `dynamicMoveCategory` | g2: #10488 / g4: #8647(HnS가 이미 적응 이식), #10277 |
| #10326 Terrain | 필드 타이머 u8, `jumpifterrain` | g2: #10440 / g3: #10454, #10593 |
| #10330 / #10362 | `enum TargetFailure` / 캔슬러 애니메이션·HP 갱신 | g2: #10467 / g2: #10431 |
| #9847 SwitchContext | `struct SwitchAiContext` | g4: #9857, #10277, #10626 |
| #9440 | `trainer_util.c`, `TrainerGenerator` | g2: #10463 / g5: #10523 |
| #8497 | `loadspritegfx` 제거, `TryLoadSpriteAssets` | g3: #9172, #9595 (그리고 1.16 이후 모든 신규 애니메이션) |
| #9475 / #9518 / #7305 | 트레이너 사진 enum / 도감 통합 / 나무열매 ID | g3: #8943 / g5: #10335 / g5: #10181 |
| #8434 OWE (Overworld) | `wild_encounter_ow.*`, OW 이동 함수 | g5: #8678, #9879, #9965, #10051, #10127, #10179, #10323 |

그룹 밖의 공통 선행 PR로, 이 그룹 판정에 반복해서 등장한 것:

- **#8943** 12v12 (`gParties[B_TRAINER_*]`, g3). 1.16 이후 거의 모든 파티 관련 hunk의 표기 차이 원인이다. HnS는 `gPlayerParty`/`gEnemyParty`를 쓴다.
- **#10051** `B_TRAINER_*` 이름 변경(g5)
- **#8678** Dynamic trainerbattle Scripts(g5)
- **#9657** `DamageContext`/`CANCELER_RESULT_*`(g3)
- **#9417** Dancer 정리(g3)
- **#9939** 명중 판정 캔슬러 통합(g3)
- **#9537** Kanto OW 이름(g5)
- **#9594** 반점 일반화(g4)
- **#9890** 디버그 메뉴 일반화(g5)

## 충돌 상세

### #9655 Refactor Battle Messages (고위험)

- **upstream 목적:**
  - 상태 회복 메시지를 `gCureStatusStringIds`(자신·상대·파티별: SCRCURED/PARTYCURED*)로 통합한다.
  - 장판 제거는 `BattleScript_RemoveHazards`와 `gRemoveHazardsStringIds`로, 방벽 해제는 `gBreakScreensStringIds`로 정리한다.
  - 멘탈허브 효과별 스크립트와 `gHurtByStringIds`(`B_MSG_HURT_BY_ITEM`)를 추가한다.
  - `battle_message.c` 224줄, `battle_scripts_1.s` 175줄이 바뀐다.
- **HnS 현재 동작과 보존 이유:**
  - 사용자 요청으로 같은 영역을 별도로 최신화했다(`BATTLE_MESSAGE_OUTPUT_CHANGES.md`).
  - 방벽 순차 해제 부분은 HnS가 이미 upstream과 같은 이름으로 넣었다: `BattleScript_BreakScreens*` `battle_scripts_1.s:3797~`, `B_MSG_BREAK_*`, `STRINGID_REFLECTWOREOFF` 등.
  - 상태 회복은 HnS 자체 `gStatusCureStringIds`(`battle_message.c:1250`)와 `BattleScript_AbilityCuredStatus`(`battle_scripts_1.s:7132`)로 상태별 문구를 낸다.
  - 장판 제거는 각 장판의 `*DISAPPEAREDFROMTEAM`을 쓴다. 생명의구슬은 `BattleScript_LifeOrbActivates`(:7333)에서 `STRINGID_LOSTSOMEOFITSHP`를 쓴다. 멘탈허브는 `BattleScript_MentalHerbCure*`(:7244~)를 쓴다.
  - upstream 구조로 덮으면 이 출력들이 되돌아간다.
- **선택지:**
  1. 보류(권장). #9730과 #9680 등 후행 리팩터를 한꺼번에 결정할 때까지 둔다.
  2. upstream 테이블 이름과 enum만 도입하고, 값은 HnS 문자열 ID로 매핑한다. 컨텍스트 적응 비용이 크다.
  3. 후행 PR(#9777 등)을 이식할 때 필요한 심볼만 HnS 쪽 별칭으로 추가한다.

### #9680 End Turn events → BattleScriptCall (고위험)

- **upstream 목적:** 턴 종료 효과를 `end2` 대신 `BattleScriptCall`/return으로 처리한다(`endturnevents`). `BattleScript_ToxicOrb/FlameOrb`, `*End2` 변형, `RainDishActivates`를 `AbilityHpHeal`로 통합하는 등 스크립트를 삭제하거나 합친다.
- **HnS 현재 동작:**
  - 맹독구슬·화염구슬은 아이템 팝업 뒤 `STRINGID_PKMNPOISONEDBY`/`BURNEDBY`를 직접 출력한다(`battle_scripts_1.s:6020/6028`).
  - 아이스바디·포이즌힐·솔라파워는 팝업과 HP 애니메이션만 낸다(:4537 등).
  - 하얀허브에는 아이템 팝업이 있다(:7263 `BattleScript_WhiteHerbEnd2`).
- **보존 이유:** 사용자가 결정한 출력 순서다. 스크립트를 삭제하거나 통합하면 팝업과 문구가 바뀐다.
- **선택지:**
  1. 보류.
  2. #9680 구조를 도입하되 HnS 스크립트 본문을 return형으로 수동 변환한다. 영향 범위는 12개 파일, 약 330줄이다.
  3. 후행(#9717, #9211, #10311)도 함께 보류한다.

### #9730 Stat Change Refactor (고위험, 선행 핵심)

- **upstream 목적:** 능력치 변화 처리 전체를 `SetStatChange`/`trybattlerstatchange` 기반으로 재작성한다. 176개 파일, +5762/-6809줄이다.
- **HnS 현재 동작:**
  - 능력치 상승·하락 문구의 공백·조사·글리프 수정(SESSION_LOG 2026-09-21)
  - 오로라베일 성공을 방어·특방 동시 상승 문구로 연결
  - 특성·도구 능력치 상승 메시지 보완(2026-09-23)
  - 미러허브·하얀허브 팝업(Champions 이식)
  - 이 로직들이 현재 `battle_script_commands.c`/`battle_scripts_1.s`에 흩어져 있다.
- **보존 이유:** 위 출력들이 모두 이 코드 경로에 들어 있다.
- **영향:** 1.16~1.17의 배틀 수정 약 50개가 이 PR의 심볼을 컨텍스트로 가진다. 이식하지 않으면 해당 PR들은 모두 "HnS 표기로 수동 재작성"해야 한다.
- **선택지:**
  1. 1.17.0 배틀 계열 전체를 "#9730 이전 구조" 기준으로 기능 단위 수동 적응한다. 현재 원작업자 방식이다.
  2. #9730 체인(#9176→#9446→#9249→#9494→#9655→#9680→#9730)을 별도 브랜치에서 통째로 이식한 뒤, HnS 메시지 최신화를 다시 적용한다. 비용과 회귀 위험이 매우 크다.

### #7305 gBerries refactor

- **upstream 목적:** 나무열매 인덱스를 아이템 ID와 분리한다(`BERRY_ID_*`, `setberrytree treeId, berryId, stage`). `gNaturalGiftTable`을 없애고 나무 그래픽 표를 재구성한다.
- **HnS 현재 동작:**
  - `data/scripts/new_game.inc:114` `#if IS_HNS` 블록에서 조토·관동 나무열매 트리 수십 그루를 `ITEM_TO_BERRY(ITEM_*_BERRY)`로 심는다.
  - `src/berry.c`는 merge-base 대비 +254줄이다. `berry_tag_screen.c`에도 HnS 수정이 있다.
- **선택지:**
  1. 보류.
  2. 이식한다면 HnS 트리 목록을 `BERRY_ID_*`로 일괄 변환한다. 세이브 안의 나무 인덱스가 같은지 먼저 확인한다(번호가 바뀌면 세이브 호환성이 깨진다).

### #9475 Refactor trainer pic info

- **upstream 목적:** 트레이너 앞·뒤 사진 정보(`TrainerPicID` 표, 팔레트, 애니메이션)를 하나의 구조로 통합한다.
- **HnS 현재 동작:**
  - `include/constants/trainers.h`에 HnS 전용 `_HNS` 항목 137개가 있다.
  - Gold/목호 사진과 팔레트는 SESSION_LOG 2026-09-16에 회귀 복구한 자산이다.
- **보존 이유:** 자산 교체와 회귀 위험이 이미 한 번 발생했다.
- **선택지:**
  1. 보류(권장).
  2. HnS 항목을 모두 새 표 형식으로 옮긴다. 수작업량이 크고 그래픽 회귀 검증이 필수다.

### #9006 Move relearner refactor

- **upstream 목적:** 기술 떠올리기를 `chooseboxmon` 기반으로 재구성하고 요약 화면 연계를 정리한다.
- **HnS 현재 동작:**
  - `BlackthornCity_House3_hns/scripts.inc:135`가 `special HasMovesToRelearn`을 쓴다. upstream은 이 special의 이름을 바꾼다.
  - `strings.c:1239~`의 relearner 문구는 한글이다. upstream은 `gText_MoveRelearnerStop`, `gText_MoveRelearnerTeachMoveConfirmUseTm` 2개를 새로 추가하는데, 공식 한국어 원문이 필요하다.
  - HnS 요약 화면은 START relearner가 없도록 재작성되어 있다.
- **선택지:**
  1. 보류. relearner 체인(#9693, #9774, #10117, #10223, #10445, #9903)도 함께 보류한다.
  2. 이식한다면 HnS 맵 스크립트 special 이름과 #9774 방식의 `VAR_RESULT` 판정을 함께 고치고, 신규 문구 2개를 미결 번역 항목으로 등록한다.

### #8930 Automate regional Pokedex orders / #9518 Consolidation of Pokédex Files

- **upstream 목적:**
  - #8930: 지역 도감 순서를 자동 생성하고 마이그레이션한다.
  - #9518: `pokedex_plus_hgss.c`와 `pokedex.c`의 공통부를 통합한다(-7814줄).
- **HnS 현재 동작:**
  - `constants/pokedex.h`에 `enum JohtoDexOrder`(:1679)를 포함해 +881줄이 추가되어 있다.
  - `pokedex_plus_hgss.c`는 HnS 버그 수정 여러 건과 SV 스키핑 이식으로 +446줄이다.
- **선택지:**
  1. 보류(권장).
  2. #9518 이후의 도감 관련 upstream 수정(#10172 등)은 HnS 파일에 기능 단위로 수동 적응한다.

### #9147 Door Size Refactor

- **upstream 목적:** 문 애니메이션 표를 크기별 일반 구조로 재작성한다.
- **HnS 현재 동작:** `src/field_door.c`에 HnS 문 43종이 있고, 마호가니와 용의굴 사당 개별 수정이 들어 있다.
- **선택지:**
  1. 보류.
  2. HnS 문 표를 새 구조로 옮긴다(도어 그래픽 전수 확인 필요).

### #9461 Show floor number in map popup

- **upstream 목적:**
  - `MapHeader.floorNumber`로 팝업에 층수("B1F", "2F", Rooftop)를 붙인다.
  - 팝업 폰트를 폭에 맞게 자동으로 고른다.
  - 셀라돈 백화점을 "CELADON DEPT."로 특례 처리한다.
- **HnS 현재 동작:**
  - 한글 지역명 팝업(`map_name_popup.c:713` `ShowMapNamePopUpWindow`)을 `FONT_NARROW`로 출력한다.
  - HnS 테마 12종을 쓴다.
  - 층수 표기는 HGSS식 한글 표기 여부가 정해지지 않았다.
- **선택지:**
  1. 보류.
  2. `floorNumber` 필드와 mapjson만 도입하고, 표기 문자열과 폰트 변경은 한글 기준으로 따로 결정한다. HGSS 공식 표기 근거가 필요하다.

### #9920 Basic daily seed

- **upstream 목적:** `SaveBlock1.unused_9C2[6]` 중 4바이트를 `u32 dailySeed`로 쓰고 날마다 갱신한다. `Crc32B`를 `random.c`로 옮긴다.
- **HnS 현재 동작:** `include/global.h:1228` `/*0x9C2*/ u32 saveVersionMagic;`와 `unused_9C6[2]`. 해당 자리를 HnS 세이브 버전 시스템(`58c2e88db2`)이 이미 쓰고 있다.
- **선택지:**
  1. `dailySeed`를 다른 여유 공간(SaveBlock3 등)에 배치한다. 세이브 버전을 올릴지 결정해야 한다.
  2. 보류. 이 경우 후행 #9877, #9955, #10012와 #9927 일부도 함께 보류된다.

### #9927 Mass outbreak

- **upstream 목적:** 대량발생을 TV 쇼에서 분리해 `mass_outbreak.c`로 새로 구성한다. OWE 연동과 디버그 메뉴를 포함한다.
- **HnS 현재 동작:**
  - 바닐라 TV 대량발생을 쓴다(`tv.c:1578`, `wild_encounter.c:687~`).
  - `tv.c`에 HnS 수정이 있다: 프런티어 TV, DAD 제거, `LAYOUT_VERSION_HNS`.
  - 호수·포켓몬센터 대사에 대량발생이 언급된다.
- **선택지:**
  1. 보류.
  2. 이식한다면 HnS 발생 종 목록과 조토식 발생 연출(라디오·전화 여부)을 먼저 정한다. 세이브블록 변경도 검토한다.

### #10548 Minor field move fixes

- **upstream 목적:** 비전기술 해금을 `unlockType`(배지 번호 `arg`)과 `hideIfLocked`로 데이터화하고, 파티 메뉴 표시를 정리한다.
- **HnS 현재 동작:** `src/field_move.c:21-80`의 각 `IsFieldMoveUnlocked_*`에 `if (IS_HNS)` 분기로 HGSS 배지를 매핑한다(플래시·바위깨기=배지1, 괴력=3, 파도타기=4, 공중날기=5, 다이빙=7, 폭포오르기=8).
- **선택지:**
  1. 보류.
  2. 이식한다면 HnS 매핑을 `arg` 값으로 옮겨 IS_HNS 전용 표를 만든다. #9819가 먼저 필요하다.

### #9335 Coins and money script commands update

- **upstream 목적:** 쓰이지 않는 인자(`disable`, x/y)를 제거하고, 코인 창 좌표를 머니박스와 같은 기준(+1)으로 맞춘다.
- **HnS 현재 동작:** `_hns` 맵 스크립트가 옛 시그니처를 쓴다. `hidemoneybox` 47회, `showmoneybox` 24회, `updatecoinsbox N, N` 17회, `hidecoinsbox N, N` 8회, `checkmoney N, N` 7회, `updatemoneybox N` 9회 등이다(게임코너·슬롯·상점).
- **보존 이유:** 인자를 바꾸면 스크립트를 일괄 수정해야 하고, 코인 창 위치가 1타일 이동한다(HnS UI 배치 변경).
- **선택지:**
  1. 보류.
  2. `migration_scripts/1.17/update_moneybox_and_coinbox_commands.py`로 `_hns` 스크립트를 변환한다. 코인 창 좌표는 HnS 호출부에서 -1로 보정하거나 새 위치를 승인받는다. #10407은 이후에 옮긴다.

### #10521 FontIdToFit: move and ability descriptions

- **upstream 목적:** 설명문이 창 폭을 넘으면 더 좁은 폰트로 자동 전환한다.
- **HnS 현재 동작:**
  - 기술 설명과 특성 설명을 한글 폰트로 고정 출력한다.
  - 배틀 기술 정보 창은 HnS Gen4 전용 창이며 L/R 전환(SESSION_LOG 2026-09-16~19)이 있다(`battle_controller_player.c:1751`).
- **보존 이유:** 좁은 폰트(`sNarrowerFontIds`)에 한글 글리프가 동일하게 있는지 확인되지 않았다. 폭 계산이 바뀌면 줄바꿈과 잘림이 바뀔 수 있다.
- **선택지:**
  1. 보류.
  2. 한글 폰트의 narrow 대체표를 확인한 뒤, 요약 화면에만 부분 도입하고 실기에서 검증한다.

## 불확실 항목과 확인 한계

- **#10426**: 지시서는 "#10426 흡수 리팩터링 반영"이라고 적었지만, SESSION_LOG 2026-09-15와 코드(`MOVE_EFFECT_ABSORB` 등 신규 26개가 모두 없음)는 구조를 이식하지 않았다고 보여준다. 흡수 계열 동작 일부만 개별 반영되었는지는 배틀 그룹이 동작 단위로 대조해야 한다.
- **#9446**: 싱크로 팝업과 상태 메시지 순서가 HnS `B_MSG_STATUSED_BY_ABILITY` 변경과 실제로 충돌하는지 실기로 확인하지 않았다.
- **#7305**: `BERRY_ID_*` 번호가 기존 `ITEM_TO_BERRY` 값과 같아 세이브 호환이 유지되는지는 확인하지 않았다(판정에 영향 있음).
- **#9819**: HnS 맵에 `MB_ROCK_CLIMB` 메타타일이 있는지는 바이너리 속성까지 전수 확인하지 않았다(바위오르기가 기술머신으로 쓰이는 것만 확인).
- **#9925, #9883**: HnS에서 반사 타일이나 Trainer Hill 동반 포켓몬 상황을 실제로 재현하지 않았다. 코드로만 판정했다.
- **용량 수치**: #9211(+약 170KB)은 링커 맵(`pokehns.map`의 `trainer_slide.o` .rodata)에서 비례 추정한 값이다. #8434와 #9896 등은 소스 규모로 추정했으며, 실제 수치는 빌드로 확인해야 한다.
- 선행 관계 자동 추출은 심볼 이름 기반이라, 일반 단어 때문에 오탐이 일부 있을 수 있다(예: `fprintf`, `activations`). 표에는 코드로 확인한 선행만 남겼다.
