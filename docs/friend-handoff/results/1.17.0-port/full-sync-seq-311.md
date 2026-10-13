# full-sync 묶음 18 — seq 311 #10542 (단독)

- PR: #10542 Remove BATTLE_PARTNER and BATTLE_OPPOSITE macros(upstream `a242cd18ac`, 53파일 +813/−797, XL 리팩터). 시작 HEAD `f5802f4d`(코드 = `6d08adc6`). 작업: 클라우드 세션, 2026-10-13.
- 방식: 사본(세션 임시 `b18/w`)에 upstream 패치를 `--reject`로 넣고 거부 hunk·HnS 전용 사용처를 에이전트가 손으로 맞춤 → 메인이 HnS 전용 치환을 검토(아이템 대상·경험치·레벨업 연출·강제 교체·기사회생의기원 등 파트너 판정이 모두 더블/멀티 조건 안인지) → 실제 저장소에 적용·검증. 결정이 필요한 것 없음(모두 upstream 규칙, HnS 보정 유지).
- 실기(mGBA) 미확인(친구가 함).

## 커밋

| seq | PR | 커밋 | 판정·HnS 적응 | ROM(B) |
|---|---|---|---|---:|
| 311 | #10542 Remove BATTLE_PARTNER and BATTLE_OPPOSITE macros | `1b1fa5fe` | 적용(손 맞춤). 바뀐 파일 53개 = upstream과 같은 목록. 거부 hunk 42개(12파일)는 모두 HnS가 바꾼 근처 줄 때문(내용은 이름 치환 — 진영 XOR 1줄만 upstream 식). HnS 줄 유지: `BattlersShareParty` 가드(#8943), 맹세 가드 `b857f809e0`(재확인 32), `AnimateMonAfterKnockout`의 `IsBattlerPresent`(21b), HnS 기본 교체 블록 `cc8fa0418a`(8q), `GetDamageCalcAbility`·`AI_CalcMoveEffectScore` 형태, 스마트 타깃 `AccuracyCheck`, 다중 타격 `gBattlerTarget` 형태. HnS 전용 매크로 사용처 38줄도 upstream 규칙으로 치환(아래 3절). 낱말 단위 grep 0 | 32,783,684 |

- ROM 32,786,196 → **32,783,684 B(−2,512)** — 계획 추정(+2~4 KB)과 반대. inline이던 `GetBattlerAtPosition`·`GetPartnerBattler`·`GetOppositeBattler`가 일반 함수가 되어 호출부마다 루프가 빠졌다(upstream도 같음, 호출 비용만 약간). EWRAM 250,408 B·IWRAM 25,516 B 그대로.

## 1. upstream 새 API(`src/battle_util.c` 82~123행, `include/battle.h` 선언)

| 함수 | 정의 |
|---|---|
| `GetBattlerAtPosition(pos)` | `gBattlersCount`까지 찾아 그 위치의 배틀러, 없으면 `gBattlersCount` (inline → 일반 함수) |
| `GetPartnerPosition(pos)` / `GetOppositePosition(pos)` | `pos ^ BIT_FLANK` / `pos ^ BIT_SIDE` (옛 매크로와 같음, 위치용) |
| `GetPartnerBattler(b)` / `GetOppositeBattler(b)` | 위치 기준으로 찾음 (inline → 일반 함수) |
| `GetBattlerLeftFoe(b)` / `GetBattlerRightFoe(b)` | `GetBattlerAtPosition(GetBattlerSide(b) ^ BIT_SIDE)` / 그 동반 |

옛 `GetOpposingSideBattler(b)` = `GetBattlerAtPosition(BATTLE_OPPOSITE(GetBattlerSide(b)))`는 `GetBattlerLeftFoe(b)`와 같은 식이다(upstream도 이렇게 바꿈).

**옛 매크로와 다른 점:** 옛 `BATTLE_PARTNER(id)`는 ID의 XOR이라 싱글에서 0→2, 1→3이었다. 새 함수는 싱글에서 둘 다 `gBattlersCount`(=2)를 돌려준다. 비링크 배틀은 위치 = ID라 더블·멀티에서는 결과가 같다. 링크 배틀(위치가 ID와 다를 수 있음)에서는 위치 기준으로 바뀐다(upstream 의도, 계획 tsv에 기록된 대로).

## 2. 거부 hunk 42개(12파일)와 처리

모든 거부 hunk의 −/+ 줄은 매크로 이름 바꾸기뿐이었다(스크립트로 확인: 남은 차이는 `HandleMoveTargetRedirection`의 진영 XOR 1줄뿐이고 upstream 그대로 적용). 거부 이유는 HnS가 근처 줄을 바꾼 것(문맥 불일치)이다. 그래서 HnS 줄을 그대로 두고 매크로 호출만 upstream 새 이름으로 바꿨다.

| 파일 | hunk | 거부 이유(HnS 쪽) / 처리 |
|---|---|---|
| battle_util.c | 7: `CalcMoveBasePowerAfterModifiers`, `CalcAttackStat`×2, `CalcDefenseStat`, `SetIllusionMon`, `TryRemoveScreens`, `GetTotalAccuracy` | HnS `GetDamageCalcAbility(ctx, …)`, `GetBattlerMon(…)`, `cv->abilities[…]` 유지, 매크로만 치환 |
| battle_script_commands.c | 10: `Cmd_resultmessage`, `Cmd_openpartyscreen`, `Cmd_forcerandomswitch`×3, `BS_ItemRestoreHP`×2, `BS_ItemCureStatus`, `BS_ItemRestorePP`, `BS_TryRevivalBlessing` | HnS `BattlersShareParty` 가드(#8943 대응), HnS `GetItemTargetBattler`/`GetItemTargetPartyOwner` 도우미가 upstream 줄을 대신함 → 도우미 안의 매크로를 치환. upstream `CancelerAccuracyCheck` 스마트 타깃 hunk는 HnS에서 `AccuracyCheck`(이 파일 1144·1148행)에 있어 그쪽을 치환 |
| battle_ai_util.c | 5: `IsDamageMoveUnusable`, `AI_CalcDamage`, `CanIndexMoveFaintTarget`, `GetMovesArray`, `AnyPartyMemberStatused` | HnS 변수 배치·`IsAiFlagPresent(AI_FLAG_MOVE_OMNISCIENCE)` 유지 |
| battle_move_resolution.c | 4: `CancelerPledgeAttack`, `HandleMoveTargetRedirection`, `CancelerAccuracyCheck`, `MoveEndMultihitMove` | 맹세 가드 `b857f809e0` 유지(재확인 32), 진영은 upstream대로 `(GetBattlerSide(…) ^ BIT_SIDE)`. 다중 타격 2타 대상은 HnS `gBattlerTarget` 형태 유지(upstream의 `cv->battlerDef =` 넣지 않음) |
| battle_controller_player.c | 4: `HandleInputChooseMove`, `Task_GiveExpToMon`, `Task_LaunchLvlUpAnim`, `Task_UpdateLvlInHealthbox` | HnS `BattlersShareParty` 가드 유지. `GetOpposingSideBattler` → `GetBattlerLeftFoe` |
| battle_ai_main.c | 3: `ChooseMoveOrAction_Doubles`, `AI_CheckBadMove`, `AI_CalcMoveEffectScore` | HnS `<=`, `IsMoldBreakerTypeAbility(…, MOVE_NONE)` 유지 |
| battle_main.c | 3: `SpriteCB_WildMon`, `SpriteCB_WildMonShowHealthbox`, `HandleTurnActionSelectionState` | HnS `gAiLogicData->reverseBattlerLogicOrder`(upstream은 지역변수) 유지 |
| battle_gfx_sfx_util.c | 2: `HandleLowHpMusicChange`, `BattleStopLowHpSound` | 문맥 차이, 치환만 |
| battle_controllers.c | 1: `AnimateMonAfterKnockout` | HnS `IsBattlerPresent` 소프트락 가드 유지(재확인 21b) |
| battle_end_turn.c | 1: `IsFutureSightAttackerInParty` | HnS `BattlersShareParty` 가드 유지 |
| battle_gimmick.c | 1: `SetGimmickAsActivated` | 문맥 차이, 치환만 |
| battle_ai_switch.c | 1: `ShouldStayInToUseMove` | HnS 팔라핀 가드 문맥, 치환만. 같은 파일 HnS B안 기본 교체 블록(`cc8fa0418a`, 재확인 8q)의 `BATTLE_PARTNER` 1곳도 치환(블록 유지) |

재확인 8r(`IsSwitchinValid`의 `BATTLE_PARTNER`)은 이미 `GetPartnerBattler`를 쓰고 있어 바꿀 것이 없었다.

## 3. 치환한 곳 전체(78줄, 매크로 사용 82개)

출처 "upstream 줄(rej)" = upstream 패치에 있던 줄인데 문맥 때문에 거부된 것(40줄). "HnS" = HnS에만 있거나 HnS가 바꾼 줄(38줄). "새 줄=upstream master" = 1.17.0 이후 upstream master에 같은 줄이 그대로 있음(앞당겨 넣은 upstream 기능: #10124 매운스프레이, #8647 라운드·맹세 AI, #10236 드래곤애로우 AI 등). 줄 번호는 사본 기준(치환으로 줄 수는 바뀌지 않음).

| 위치(이식 뒤 줄) | 함수 | 출처 | 옛 → 새 | 인자 | 동작 변화 / 근거 |
|---|---|---|---|---|---|
| battle_util.c:6764 | `CalcMoveBasePowerAfterModifiers` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `IsBattlerAlive` 가드. 싱글에서 옛 값 2·3, 새 값 2 모두 `>= gBattlersCount`라 FALSE |
| battle_util.c:6766 | `CalcMoveBasePowerAfterModifiers` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 위 `IsBattlerAlive` 안. HnS `GetDamageCalcAbility(ctx, …)` 유지 |
| battle_util.c:7018 | `CalcAttackStat` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `IsBattlerAlive` 가드(플러스) |
| battle_util.c:7020 | `CalcAttackStat` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 가드 안. HnS `GetDamageCalcAbility` 유지 |
| battle_util.c:7027 | `CalcAttackStat` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `IsBattlerAlive` 가드(마이너스) |
| battle_util.c:7029 | `CalcAttackStat` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 가드 안. HnS `GetDamageCalcAbility` 유지 |
| battle_util.c:7138 | `CalcAttackStat` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `IsBattlerAlive` 가드 |
| battle_util.c:7140 | `CalcAttackStat` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 가드 안. HnS `GetDamageCalcAbility` 유지 |
| battle_util.c:7143 | `CalcAttackStat` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 가드 안(플라워기프트) |
| battle_util.c:7325 | `CalcDefenseStat` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `IsBattlerAlive` 가드 |
| battle_util.c:7327 | `CalcDefenseStat` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 가드 안. HnS `GetDamageCalcAbility` 유지 |
| battle_util.c:7330 | `CalcDefenseStat` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 가드 안 |
| battle_util.c:7331 | `CalcDefenseStat` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 가드 안 |
| battle_util.c:9019 | `SetIllusionMon` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `IsBattlerAlive` 가드(일루전) |
| battle_util.c:9020 | `SetIllusionMon` | HnS · 새 줄=upstream master | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 가드 안. HnS `GetBattlerMon` 형태 = upstream master와 같은 줄 |
| battle_util.c:9221 | `TryRemoveScreens` | upstream 줄(rej) | `BATTLE_OPPOSITE` → `GetOppositeBattler` | 배틀러 | 비링크 없음 — 맞은편은 싱글·더블 모두 존재. 결과는 진영만 씀 |
| battle_util.c:10585 | `GetTotalAccuracy` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 쓰는 곳(승리의별)이 `IsBattlerAlive(atkAlly)` 가드 |
| battle_main.c:2773 | `SpriteCB_WildMon` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `WILD_DOUBLE_BATTLE` 안 |
| battle_main.c:2806 | `SpriteCB_WildMonShowHealthbox` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `WILD_DOUBLE_BATTLE` 안 |
| battle_main.c:4328 | `HandleTurnActionSelectionState` | HnS · 새 줄=upstream master | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `reverseBattlerLogicOrder`는 `IsDoubleBattle()`일 때만 TRUE. 위치 순열이라 모든 배틀러를 한 번씩 돈다. upstream master와 같은 줄 |
| battle_controllers.c:3066 | `AnimateMonAfterKnockout` | upstream 줄(rej) | `BATTLE_OPPOSITE` → `GetOppositeBattler` | 배틀러 | 비링크 없음. HnS `IsBattlerPresent` 가드 유지(재확인 21b) |
| battle_controllers.c:3067 | `AnimateMonAfterKnockout` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `BATTLE_TYPE_DOUBLE` 가드가 먼저 |
| battle_gfx_sfx_util.c:1159 | `HandleLowHpMusicChange` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `IsDoubleBattle()` 가드 |
| battle_gfx_sfx_util.c:1173 | `BattleStopLowHpSound` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `IsDoubleBattle()` 가드 |
| battle_move_resolution.c:658 | `CancelerPledgeAttack` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 맹세 대기는 더블에서만. HnS 맹세 가드 `b857f809e0` 유지(재확인 32) |
| battle_move_resolution.c:839 | `HandleMoveTargetRedirection` | upstream 줄(rej) | `BATTLE_OPPOSITE(GetBattlerSide(x))` → `(GetBattlerSide(x) ^ BIT_SIDE)` | 진영 | 없음 — upstream과 같게 `(GetBattlerSide(…) ^ BIT_SIDE)`(진영 XOR, 함수 아님) |
| battle_move_resolution.c:3182 | `MoveEndMultihitMove` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 싱글은 `moveResultFlags[2]`를 읽지만(배열 4칸) 다음 `CanTargetPartner`(`IsDoubleBattle`)가 FALSE. HnS는 `gBattlerTarget` 형태(upstream은 `cv->battlerDef`) 유지 |
| battle_move_resolution.c:3184 | `MoveEndMultihitMove` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `CanTargetPartner` 뒤 |
| battle_move_resolution.c:3185 | `MoveEndMultihitMove` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 같은 조건 안. HnS 형태 유지(`cv->battlerDef =` 추가 안 함) |
| battle_ai_util.c:623 | `IsDamageMoveUnusable` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 싱글 이론상 — 옛 `abilities[def^2]`, 새 `abilities[2]`. 싱글에서 2·3칸은 둘 다 채워지지 않는 칸(0=ABILITY_NONE). upstream도 같은 치환(else 가지) |
| battle_ai_util.c:764 | `CalcDynamicMoveDamage` | HnS · 새 줄=upstream master | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `IsDoubleBattle()` 가드. upstream master와 같은 줄 |
| battle_ai_util.c:947 | `AI_CalcDamage` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 싱글 이론상 — `ctx` 2칸에 복사만(사용처는 동반 가드). upstream과 같은 줄 |
| battle_ai_util.c:948 | `AI_CalcDamage` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 위와 같음 |
| battle_ai_util.c:2572 | `CanIndexMoveFaintTarget` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `IsDoubleBattle()` 가드 |
| battle_ai_util.c:2584 | `GetMovesArray` | HnS · 새 줄=upstream master | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 싱글 이론상 — 배틀러 1을 물을 때 옛 `IsAiBattlerAware(3)`, 새 `(2)`. 앞 항 `IsAiBattlerAware(1)`이 AI면 TRUE라 차이 없음. upstream master와 같은 줄 |
| battle_ai_util.c:2678 | `ShouldTriggerSpicySprayForBurn` | HnS · 새 줄=upstream master | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `!HasPartner(battlerAtk)`가 먼저 걸러냄. upstream master와 같은 줄 |
| battle_ai_util.c:3928 | `AnyPartyMemberStatused` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 더블 가지 안 |
| battle_ai_util.c:3931 | `AnyPartyMemberStatused` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 더블 가지 안 |
| battle_ai_util.c:4514 | `ShouldUseFusionMove` | HnS · 새 줄=upstream master | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `IsBattlerAlive(partner)` 가드. upstream master와 같은 줄 |
| battle_ai_util.c:4528 | `ShouldUseRound` | HnS · 새 줄=upstream master | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `IsBattlerAlive(partner)` 가드. upstream master와 같은 줄 |
| battle_ai_util.c:4540 | `ShouldUsePledgeMove` | HnS · 새 줄=upstream master | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 더블 동반 행동 판단에서만 의미. upstream master와 같은 줄 |
| battle_gimmick.c:110 | `SetGimmickAsActivated` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `IsDoubleBattle()` 가드 |
| battle_end_turn.c:237 | `IsFutureSightAttackerInParty` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `IsDoubleBattle()` 가드. HnS `BattlersShareParty` 유지 |
| battle_end_turn.c:240 | `IsFutureSightAttackerInParty` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 같은 가드 안 |
| battle_ai_main.c:470 | `SetAllyMove` | HnS · 새 줄=upstream master | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `IsBattlerAlive` 가드. upstream master와 같은 줄 |
| battle_ai_main.c:1077 | `ChooseMoveOrAction_Doubles` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 더블 전용 함수. HnS `<=` 유지 |
| battle_ai_main.c:2306 | `AI_CheckBadMove` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `DoesPartnerHaveSameMoveEffect`가 `HasPartner` 검사 |
| battle_ai_main.c:4334 | `AI_CalcMoveEffectScore` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 싱글 이론상 — 옛 `[def^2]`, 새 `[2]`(채워지지 않는 칸). upstream과 같은 치환. HnS 3번째 인자 `MOVE_NONE` 유지 |
| battle_ai_main.c:4335 | `AI_CalcMoveEffectScore` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 위와 같음 |
| battle_controller_player.c:711 | `HandleInputChooseMove` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `TARGET_ALLY` 기술은 더블에서만 선택 |
| battle_controller_player.c:713 | `HandleInputChooseMove` | upstream 줄(rej) | `GetOpposingSideBattler` → `GetBattlerLeftFoe` | 배틀러 | 없음 — `GetOpposingSideBattler`의 정의 = `GetBattlerLeftFoe`의 정의(같은 식). upstream과 같음 |
| battle_controller_player.c:719 | `HandleInputChooseMove` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 더블 블록 안 + `IsBattlerAlive` |
| battle_controller_player.c:1439 | `Task_GiveExpToMon` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `IsDoubleBattle()` 가드. HnS `BattlersShareParty` 유지 |
| battle_controller_player.c:1541 | `Task_LaunchLvlUpAnim` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `IsDoubleBattle()` 가드. 다음 줄 `battler ^= BIT_FLANK`는 upstream도 그대로 |
| battle_controller_player.c:1542 | `Task_LaunchLvlUpAnim` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 같은 가드 |
| battle_controller_player.c:1557 | `Task_UpdateLvlInHealthbox` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `IsDoubleBattle()` 가드 |
| battle_controller_player.c:1558 | `Task_UpdateLvlInHealthbox` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 같은 가드 |
| battle_controller_player.c:1559 | `Task_UpdateLvlInHealthbox` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 같은 가드 |
| battle_ai_switch.c:1526 | `ShouldSwitch` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `IsDoubleBattle()` 가드. HnS B안 기본 교체 블록(`cc8fa0418a`, 재확인 8q) 유지 |
| battle_ai_switch.c:1583 | `ShouldStayInToUseMove` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `IsDoubleBattle()` 가드 |
| battle_script_commands.c:1144 | `AccuracyCheck` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `CanTargetPartner`(더블) 뒤. HnS `AccuracyCheck` 스마트 타깃(upstream은 `CancelerAccuracyCheck`) |
| battle_script_commands.c:1148 | `AccuracyCheck` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 같은 조건 안 |
| battle_script_commands.c:2049 | `Cmd_resultmessage` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `…TWOFOES`(더블 전체기) |
| battle_script_commands.c:2068 | `Cmd_resultmessage` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `…TWOFOES` |
| battle_script_commands.c:2115 | `Cmd_resultmessage` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `ShouldPrintTwoFoesMessage`(더블 전체기) |
| battle_script_commands.c:5603 | `Cmd_openpartyscreen` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 싱글 이론상 — 옛 `monToSwitchIntoId[b^2]`, 새 `[2]`. 싱글에서 2·3칸은 같은 초기값(0, 채우는 루프는 `gBattlersCount`까지)이라 값 같음. upstream과 같은 줄 |
| battle_script_commands.c:7632 | `Cmd_forcerandomswitch` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 더블·멀티 가지. HnS `BattlersShareParty` 유지 |
| battle_script_commands.c:7633 | `Cmd_forcerandomswitch` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 같은 가지 |
| battle_script_commands.c:7642 | `Cmd_forcerandomswitch` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 더블·멀티 가지 |
| battle_script_commands.c:9901 | `Cmd_pursuitdoubles` | HnS | `GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(x)))` → `GetBattlerAtPosition(GetPartnerPosition(GetBattlerPosition(x)))` | 위치 | 없음 — 원래 위치 기준 식. upstream 같은 파일 관례대로 `GetBattlerAtPosition(GetPartnerPosition(GetBattlerPosition(…)))`(미사용 명령) |
| battle_script_commands.c:11602 | `GetItemTargetPartyOwner` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 비링크 멀티 전용(위치=ID). HnS 멀티 아이템 대상 |
| battle_script_commands.c:11614 | `GetItemTargetBattler` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `IsDoubleBattle()` 가드 |
| battle_script_commands.c:11615 | `GetItemTargetBattler` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 같은 가드 |
| battle_script_commands.c:11616 | `GetItemTargetBattler` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 같은 가드 |
| battle_script_commands.c:11682 | `BS_ItemRestoreHP` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 다음 줄 `IsDoubleBattle()` 가드 |
| battle_script_commands.c:12444 | `BS_TryRevivalBlessing` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — `IsDoubleBattle()` 가드(부활의기원). HnS `BattlersShareParty` 유지 |
| battle_script_commands.c:12445 | `BS_TryRevivalBlessing` | HnS | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 같은 가드 |
| battle_script_commands.c:12447 | `BS_TryRevivalBlessing` | upstream 줄(rej) | `BATTLE_PARTNER` → `GetPartnerBattler` | 배틀러 | 없음 — 같은 가드 |

인자 분류: 배틀러 76줄, 진영 1줄(`battle_move_resolution.c:839`), 위치 1줄(`battle_script_commands.c:9901`, 미사용 `Cmd_pursuitdoubles` — upstream의 같은 파일 관례대로 `GetBattlerAtPosition(GetPartnerPosition(GetBattlerPosition(…)))`). HnS 코드에서 위치를 매크로에 직접 넣은 다른 곳은 없었다.

### 3.1 그대로 적용된 upstream hunk 중 동작에 관계있는 것(참고)

- `pokemon.c` `CalculatePartyCountOfSide`: `CalculatePartyCount(BATTLE_PARTNER(battler))`(배틀러 ID를 트레이너 번호로 넘김) → `CalculatePartyCount(GetBattlerTrainer(GetPartnerBattler(battler)))`. `B_TRAINER_*` 순서(0 플레이어, 1 상대A, 2 동반, 3 상대B)가 배틀러 ID와 맞아 비링크에서는 같은 값.
- `pokemon.c` 3772: `opposing = GetBattlerLeftFoe(battlerId)`(옛 값은 진영 번호 = 위치) — 비링크에서 같음. upstream 그대로.
- `battle_message.c` 3400~3406: 링크 멀티 이름 버퍼의 `BATTLE_PARTNER`/`LEFT_FOE`/`RIGHT_FOE` → upstream 그대로(코드 줄만, 한글 문자열 테이블 변경 없음).
- `test/test_runner_battle.c`: upstream대로 `^ BIT_SIDE`/`^ BIT_FLANK` 직접 XOR.

## 4. 위험·판단

1. **싱글에서 동반 칸 2·3을 가드 없이 읽는 곳(동작 변화 이론상만):** `battle_ai_util.c:623`(`IsDamageMoveUnusable` 피뢰침·마중물 판정), `:947~948`(`AI_CalcDamage` ctx 복사), `:2584`(`GetMovesArray`), `battle_ai_main.c:4334~4335`, `battle_script_commands.c:5603`(`Cmd_openpartyscreen`이 컨트롤러에 넘기는 값). 옛 값은 `[b^2]`(2 또는 3), 새 값은 `[2]`. 싱글에서 2·3칸은 둘 다 쓰이지 않고 같은 방식으로 초기화되는 칸이라(전투 구조체 0 할당, 턴마다 채우는 루프는 `gBattlersCount`까지) 값이 같다. 모두 upstream이 같은 치환을 한 줄(또는 upstream master와 같은 줄)이다. 테스트에서 차이 없음.
2. **링크·녹화 배틀:** 위치 ≠ ID일 수 있어 동반·맞은편 계산이 위치 기준으로 바뀐다. upstream 의도이고 HnS 전용 줄도 같은 규칙을 따른다. 링크 실기는 확인 못 함.
3. **HnS 동반 배틀(#8943 별도 파티):** `BattlersShareParty` 가드는 모두 유지. 비링크라 위치 = ID여서 결과 같음.
4. **ROM:** 계획 추정(+2~4KB)과 달리 −2,512 B. inline이던 `GetBattlerAtPosition` 루프·`GetPartnerBattler`가 함수 호출이 되어 줄어든 것으로 본다. 호출 비용으로 약간 느려질 수 있음(upstream도 같음).
5. 남은 `BATTLE_PARTNER` 문자열은 `FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_*`, `FNPC_DATA_BATTLE_PARTNER`, `GUARD_CONSTANTS_BATTLE_PARTNERS_H` 등 다른 이름의 일부뿐(낱말 단위 grep 0). 주석 속 사용 0.

결정이 필요한 것: 없음(전부 upstream 규칙대로, HnS 보정 유지).

## 5. 검증

- **사본과 실제 저장소가 같음:** 실제 저장소에 이식안을 넣은 뒤 `src`·`include`·`test`·`data`·`asm`이 사본과 차이 0, `make hns -j8` ROM SHA1 `7897b54b49013c6746ac641f451bad37674b1208`이 사본 빌드와 같다. 그래서 사본의 전체 테스트 결과를 그대로 쓴다.
- **빌드:** 종료 0, 새 경고 0(`warncheck.sh`, 클라우드 libpng `iCCP` 줄 제외).
- **전체 테스트**(사본, `GITHUB_ACTION=1 make check BUILD=hns -j3`): PASS 2,659 / TOTAL 5,582, `testlist.sh` 목록이 `test-baseline-hnsfix1013.txt`와 **바이트 같음**(새 PASS 0, 사라진 PASS 0) — 기준 목록은 그대로 `test-baseline-hnsfix1013.txt`.
- **한글 회귀 607개**(실제 저장소, `ALLOW_REPO=1 dev_scripts/hns_verify/kortests/run.sh <저장소> kor-seq311 8`): 517/607, 기대 요약과 바이트 같음.
- **세이브 정적 비교**(`pre-301` 기준): PASS(FAIL 0, WARN 0) — 세이브 경로 코드 엄격 54·검토 18 같음, `recorded_battle.o` 37 같음, RAM 변수 변화 0.
- **링크·녹화 배틀:** 위치 ≠ ID일 수 있는 링크에서는 동반·맞은편 계산이 위치 기준으로 바뀐다(upstream 의도). 테스트·실기로는 확인 못 함 — HnS 일반 진행 밖.

## 재확인·뒤 PR 메모

- 8q(`cc8fa0418a` HnS 기본 교체 블록): `BATTLE_PARTNER` 1곳 → `GetPartnerBattler`, 블록 유지.
- 8r(`IsSwitchinValid`): 이미 `GetPartnerBattler` — 변경 없음.
- 32(맹세 가드 `b857f809e0`): `CancelerPledgeAttack`에서 가드 유지, 매크로만 치환.
- 뒤 PR이 새 함수 이름(`GetPartnerBattler`·`GetBattlerLeftFoe` 등)을 쓰므로 이제 그대로 들어간다. HnS에 새로 쓰는 코드도 매크로 대신 이 함수를 쓴다.
- 치환한 곳 전체 표(파일:줄·출처·인자 종류·근거)는 아래 3절. 원본 TSV는 [`full-sync-seq-311-sites.tsv`](full-sync-seq-311-sites.tsv).
