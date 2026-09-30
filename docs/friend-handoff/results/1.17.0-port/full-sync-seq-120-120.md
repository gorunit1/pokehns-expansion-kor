# full-sync 실제 port 결과 — seq 120

완료: seq 120 이식·전체 테스트·기록 완료(다음 구간은 seq 121 #9594부터). 결과 커밋에 기준 목록 [`test-baseline-seq120.txt`](test-baseline-seq120.txt)를 넣었다. 메인의 커밋 리뷰는 이 문서 밖에서 진행한다.

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), [`CLAUDE_FULL_SYNC_PORT_PROMPT.md`](../../CLAUDE_FULL_SYNC_PORT_PROMPT.md), 메시지 출력 기록: [`BATTLE_MESSAGE_OUTPUT_CHANGES.md`](../../../localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md)
시작 HEAD: `17aa03192f` (작업 트리 clean. `de9b581a28`(seq 119 코드) 뒤로는 docs만 바뀜)

## seq 120 요약

| seq | PR | 판정 | 커밋 | ROM 변화 | 비고 |
|---|---|---|---|---:|---|
| 120 | #9657 | 적용(HnS 적응) | `aa175914e9` | +96 B | 캔슬러 `BattleCalcValues *cv`화, `BattleContext`→`DamageContext`(146곳), `CANCELER_RESULT_BREAK/PAUSE` 이름 변경, `UpdateStallMons` 지연 방식. 메가솔라·잠자기 불면/의기양양 문구·관통드릴·참기 `animTurn`·챌린지 집단구타 등 HnS 분기 cv형으로 유지. 프리폴 무게 판정이 대상 특성·도구를 반영(출력 변화 문서 1행) |

- 빌드(`aa175914e9`): 종료 코드 0, **ROM 32,719,060 B(97.51%, +96 B) / EWRAM 248,940 B(94.96%, 0) / IWRAM 25,516 B(77.87%, 0)**. `pokehns.gba` SHA1 `2ef9b346a463da48dc2a7f41f12d58ca4a619b53`. 사전 분석 추정(비 LTO)은 `battle_move_resolution.o` +472 B, `battle_util.o` −391 B였다.
- 새 경고 0. 헤더 변경으로 152개 파일이 다시 컴파일되어 경고 21줄이 나왔지만, 모두 수정하지 않은 6개 파일(`match_call.c` 10, `battle_frontier_exchange_corner.h` 7, `battle_gfx_sfx_util.c`·`nuzlocke.c`·`pokemon_summary_screen.c`·`pokenav_match_call_data.c` 각 1)의 기존 미사용 경고다. 수정한 14개 파일의 경고는 0이다.
- 한글이 든 소스 줄 변경: 0(비 ASCII `+`/`-` 줄 0).
- `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 추가 행: 1행("기술·필드 상태 효과", 프리폴 무게 판정).
- 전체 테스트: 표준 목록이 `test-baseline-seq119.txt`와 **바이트 동일**. 사라진 PASS 0(아래 "전체 테스트").

## 공통 사항

- 툴체인: 이 컴퓨터의 기본 PATH `arm-none-eabi-gcc` 13.2.1(`/usr/bin`). PATH 접두어 없이 빌드했다.
- 빌드 명령: `GITHUB_ACTION=1 make hns -j8 > build/port.log 2>&1`
- 테스트 명령: 파일별 `GITHUB_ACTION=1 make check BUILD=hns -j6 TESTS="<파일>"`, 전체 `GITHUB_ACTION=1 make check BUILD=hns -j6`(로그 `build/port-check-post120.log`, 4분 1초, 종료 코드 2 = 실패 테스트 있음)
- 목록 추출: `PORT_INSTRUCTIONS`의 `LC_ALL=C`·`grep -a` 표준 추출과, `ASSUMPTION_FAIL|INVALID|TIMEOUT|CRASH`를 더한 확장 추출.

## 사전 분석 (읽기 전용 분석 에이전트 4개)

이식 전에 분석 에이전트 4개가 저장소를 수정하지 않고 영역별 계획과 HEAD 기준 적응 patch를 만들었다. 산출물은 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-120/`에 있다.

| 문서 | 영역 | 요지 |
|---|---|---|
| `part-A.md`·`.patch`(md5 `7abe8e07…`) | `battle_move_resolution.c` 앞부분(파일 머리 ~ `CancelerBide`) | upstream hunk 29개: 그대로 25 / 수정해서 4 / 제외 0. `CancelerSetTargets`가 `cv->battlerDef`에 쓰고 SUCCESS를 돌려줌(재진입 제거). HnS 줄 5개 유지 |
| `part-B.md`·`.patch`(md5 `190fbe0e…`) | 같은 파일 뒷부분(`CancelerMoveFailure` ~ 끝) | upstream hunk 28개: 그대로 22 / 수정해서 6 / 제외 0, hunk 밖 HnS 전용 변환 3곳. 메가솔라판 `CanTwoTurnMoveFireThisTurn` 유지, 프리폴 무게 동작 변화(R1) |
| `part-C.md`·`.patch`(md5 `9f747460…`), `tmp-C/battlecontext-sites.tsv` | `battle_util.c`, 헤더 4개, `battle_script_commands.c`, terastal, tv, controller | upstream hunk 56개: 그대로 47 / 수정해서 8 / 제외 1, HnS 전용 hunk 4개. 관통드릴 유지, `SpecialStatus` 마지막 여유 비트 사용 |
| `part-D.md`·`.patch`(md5 `4136f933…`) | AI 4개 파일 | upstream hunk 17개: 그대로 11 / 수정해서 6 / 제외 0, HnS 추가 1(`partnerCtx`). 타입 이름만 18곳 |

## 동기화 단위: seq 120 #9657 `U-calcvalues-9657` BattleCalcValues usage and rename BattleContext back to DamageContext

- 현재 판정: 적용(HnS 적응)
- 커밋: `aa175914e9`
- upstream 근거: `1f45edb04c`(14파일 +640/−610), 부모 `9fbfb39797`. 선행 #9116·#9176·#9417·#9249·#9610·#9532·#9494는 모두 앞 구간에서 적용됐다.
- 수정 파일(14): `include/battle.h`, `include/battle_ai_util.h`, `include/battle_terastal.h`, `include/battle_util.h`, `include/constants/battle_move_resolution.h`, `src/battle_ai_main.c`, `src/battle_ai_switch.c`, `src/battle_ai_util.c`, `src/battle_controller_player.c`, `src/battle_move_resolution.c`, `src/battle_script_commands.c`, `src/battle_terastal.c`, `src/battle_tv.c`, `src/battle_util.c`(+663/−634)
- 적용 방법:
  - 사전 분석 patch 4개를 A→B→C→D 순서로 `git apply --check` 뒤 `git apply`했다. 네 개 모두 충돌 없이 들어갔다. 부분 적용하면 빌드가 깨지므로 한 커밋으로 넣었다.
  - patch의 `index … 100644` 줄에서 모드만 지운 사본을 썼다. `battle_move_resolution.c`·`battle_util.c` 등 8개 파일이 저장소에서 100755라서 그대로 적용하면 모드가 100644로 바뀐다. 커밋에 모드 변경은 없다.
  - `battle_move_resolution.c` 확인: "upstream 부모 대 HEAD" 차이에 ctx→cv 기계 치환을 한 것과 "upstream 자식 대 결과" 차이가 줄 단위로 같다(각 416줄). HnS 고유 차이가 빠지거나 새로 생기지 않았다. `CancelerTookAttack`·`CanMoveBeBlockedByTargetHelper`·`CancelerTargetFailure`·`DoAttackCanceler`·`CancelerSetTargets`·`HandleMoveTargetRedirection`·bounce 3종·`UpdateStallMons`는 upstream 자식과 글자까지 같다.

### 영역별 요약

- **A (`battle_move_resolution.c` 파일 머리 ~ `CancelerBide`)**
  - 캔슬러 인자 `struct BattleContext *ctx` → `struct BattleCalcValues *cv`, `ctx->abilityAtk` → `cv->abilities[cv->battlerAtk]`, `ctx->chosenMove` → `gChosenMove`, BREAK → `CANCELER_RESULT_RUN_SCRIPT_AND_INCREMENT`
  - 자기 공격·혼란 자해 피해의 지역 변수는 `struct DamageContext dmgCtx`
  - `CancelerSetTargets`는 대상을 `cv->battlerDef`에 쓰고 `gBattlerTarget = cv->battlerDef` 뒤 SUCCESS를 돌려준다. 예전의 BREAK → 재진입 → ctx 재생성이 없어진다. `HandleMoveTargetRedirection(cv, moveTarget)`, `WasOriginalTargetAlly(atk, def, moveTarget)`
  - 파일 머리에 bounce 3종 cv형 선언과 `static void UpdateStallMons(void);`
- **B (`CancelerMoveFailure` ~ 파일 끝)**
  - 나머지 캔슬러 cv화, PAUSE → `CANCELER_RESULT_RUN_SCRIPT`, `sMoveSuccessOrderCancelers` 함수 포인터 형식 변경
  - `DoAttackCanceler`가 `cv`를 지정 초기화하고 모든 배틀러의 특성·도구를 채운다. 루프는 `!= CANCELER_RESULT_RUN_SCRIPT`
  - 새 `CanMoveBeBlockedByTargetHelper`. `CancelerTargetFailure`가 막힘·상성 0일 때 `gSpecialStatuses[cv->battlerDef].updateStallMons = TRUE`(2곳)
  - 파일 끝에 static `UpdateStallMons(void)`. `MoveEndUpdateLastMoves`의 호출은 그대로
- **C (`battle_util.c`, 헤더, `battle_script_commands.c`, terastal, tv, controller)**
  - `struct BattleContext` → `struct DamageContext`(필드·배치 그대로), 원형·정의 이름 변경
  - `bool32 IsBattlerProtected(struct BattleCalcValues *cv)`
  - `CANCELER_RESULT_*` 열거자 이름 변경(값 1·2 그대로)
  - `SpecialStatus.updateStallMons:1`, `battle_util.c`/`.h`의 공개 `UpdateStallMons` 삭제
- **D (AI 4파일):** 타입 이름만 18곳(upstream 17 + HnS `partnerCtx` 1). **AI 오브젝트 역어셈블 sha1이 이식 전후 같다**(`battle_ai_main.o` `d7508e13…`, `battle_ai_switch.o` `5aeed09d…`, `battle_ai_util.o` `89263b4a…`, `/usr/bin/arm-none-eabi-objdump -d -r`, 사전 분석 참고값과도 같음).

### HnS 적응·보존

- **A:** HnS 줄 5개를 cv형으로 유지했다.
  - `#include "challenge_menu.h"`
  - #10093 `gBattlescriptCurrInstr = GetMoveBattleScript(gCalledMove);`
  - #9999 `gLastMoves[cv->battlerAtk] = gChosenMove;`
  - 4인자 `SetTypeBeforeUsingMove(cv->move, cv->battlerAtk, cv->abilities[…], cv->holdEffects[…])`
  - 참기 방출 `gBattleScripting.animTurn = 1; // HnS: …`
- **B:**
  - 메가솔라: HnS판 `CanTwoTurnMoveFireThisTurn(struct BattleCalcValues *cv, bool32 *showAbilityPopUp)`를 `CancelerExplosion` 앞 한 곳에 둔다(upstream판 1인자 함수는 넣지 않음). 만능우산 반영 실제 날씨면 바로 발사, 메가솔라 날씨로만 조건이 맞으면 `BattleScript_MegaSolActivatesTwoTurnMove` 뒤 `RUN_SCRIPT_AND_INCREMENT`(옛 BREAK와 같은 값)
  - 잠자기: 불면·의기양양은 `BattleScript_StayedAwakeUsingAbility`, 정화의소금은 `BattleScript_InsomniaProtects`로 가는 두 분기와 `gLastUsedAbility`·`gBattlerAbility`·`RecordAbilityBattle`을 유지(upstream처럼 합치지 않음)
  - HnS 전용 `CancelerInterruptibleMoves`(#9942 선반영)와 표 항목, 프리폴 두 번째 턴 `skyDropTarget` 처리, 파워풀허브 분기의 `semiInvulnerable = STATE_NONE`
  - 부자유친(`twoTurnEffect`·`EFFECT_OHKO`), 로디드다이스(스킬링크 제외), 집단구타 챌린지 파티 크기(`GetMaxPartySize()`)
- **C:**
  - 관통드릴: `IsBattlerProtected(cv)`에서 `(cv->abilities[cv->battlerAtk] == ABILITY_UNSEEN_FIST || cv->abilities[cv->battlerAtk] == ABILITY_PIERCING_DRILL) && IsMoveMakingContact(…)`로 유지(upstream #9740과 같은 줄)
  - upstream hunk가 없는 HnS 사용처 6곳 이름 변경: `AbsorbedByFlashFire` 원형·정의, `IsCriticalHit` 원형, `GetDamageCalcAbility`, `IsDamageCalcAbilityOnField`, `GetDefenderPartnerAbilitiesModifier`(HnS 2인자형)
  - `SpecialStatus`: #10047 뒤 여유가 `padding:1` 하나라서 이를 지우고 `mindBlownRecoil` 뒤에 `updateStallMons:1`을 넣었다(1.17.0 순서, `poisonPuppeteer` 비트 6→7, 원시 비트 접근 없음). 구조체 8 B 그대로라 EWRAM 0
  - 본문 그대로 남은 HnS 분기: 메가솔라 `GetAttackerWeather`(`// HnS:` 2곳), `GetAdjustedDamage`의 `tx_Mode_Sturdy`, 효과 단계 결과 플래그(`UpdateMoveResultFlags`)
- **D:** 1.17.0형 시그니처(`CalcDynamicMoveDamage(…, struct SimulatedDamage *)`, `CanPalafinZeroSafelyUseHitEscape`, `AI_CalcDamage(struct AiCalcValues *, …)`)를 upstream 부모형으로 되돌리지 않았다.
- `// HnS` 주석 보존: 14개 파일의 `HnS` 포함 줄 수가 전후 같고, `+`/`-` 줄에 `HnS`가 0이다. 지시로 확인한 줄:
  - 구 시트러스 `battle_ai_switch.c:1692`, HP 클램프 `:2971`
  - 집단구타 합계 `battle_ai_util.c:777`, 아군 `.maximum` `:2623`
  - 참기 `animTurn` `battle_move_resolution.c:1085`
  - 원시 날씨 해제 `data/battle_scripts_1.s:6221`(`@ HnS:`, 이번 커밋이 건드리지 않음)
- 제외한 hunk: `battle_util.c` `DoFutureSightAttackDamageCalcVars` 1개(HnS에 함수가 없음. `DoFutureSightAttackDamageCalc`가 1.17형으로 이미 통합). 문맥만 달라 손으로 옮긴 hunk는 위 사전 분석 표대로다.

### 적용 뒤 확인 (모두 충족)

| 조건 | 결과 |
|---|---|
| `git grep -w BattleContext -- ':!docs'` | 0 |
| `git grep 'CANCELER_RESULT_BREAK\|CANCELER_RESULT_PAUSE'` | docs 1줄(`batch-a-battle.md` 역사 기록)뿐, 코드 0 |
| `IsBattlerProtected(cv)`의 관통드릴 | `battle_util.c:5920` 유지 |
| `UpdateStallMons` | `battle_move_resolution.c` 선언 27·호출 2705·static 정의 4453, 3줄 |
| `updateStallMons` | `battle.h:108` + 설정 2곳(`battle_move_resolution.c:1849`·`1882`) + 판독 1곳(`4475`) |
| HnS 줄 3개(A) | `GetMoveBattleScript(gCalledMove)` 229, `gLastMoves[cv->…]` 975, `animTurn = 1; // HnS` 1085 |
| 메가솔라 | `CanTwoTurnMoveFireThisTurn(cv, &showAbilityPopUp)` 정의 1510(`CancelerExplosion` 1533 앞) 하나 |
| `ctx` 이름 | `battle_move_resolution.c`에는 지역 `struct DamageContext ctx` 두 곳과 `dmgCtx`만 남음 |
| AI 4파일 `struct DamageContext` | 1 / 2 / 5 / 10(합 18), +18/−18 |
| `git diff --check`, 파일 모드 | 통과, 모드 변경 0 |

### 동작 변화

- **프리폴 무게(출력 변화, upstream 1.17.0과 같아짐):** HEAD는 `HandleSkyDropResult`에서 `GetBattlerWeight(ctx->battlerDef, ctx->abilityDef, ctx->holdEffectDef)`를 불렀다. 그런데 `abilityDef`·`holdEffectDef`는 `CANCELER_CHARGING`(64)보다 뒤의 `CancelerTargetFailure`(67)에서야 채워져 이 시점에 항상 0이었다. 그래서 대상의 헤비메탈·라이트메탈·가벼운돌이 무시됐다(HnS의 #10145 선반영 때 생긴 차이). 이제 `DoAttackCanceler`가 진입 때 채운 `cv->abilities[]`·`cv->holdEffects[]`를 넘긴다. 임시 테스트로 전후를 확인했다(아래). 문자열 ID·본문은 그대로이고 `BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 1행을 추가했다.
- **PP 끌기 기록(AI만, 메시지 무관, upstream 동작):** `gAiBattleData->playerStallMons` 증가가 MoveEnd 재계산에서 `CancelerTargetFailure`의 실제 막힘·상성 0 갈래로 옮겨졌다. 공격자가 캔슬러 앞단(마비·풀죽음 등)에서 멈춘 턴은 더 세지 않고, 더블의 `TARGET_SMART`(드래곤애로)는 세지 않는다. `ai_pp_stall_prevention.c` PASS 유지.
- **대상 지정(보이는 변화 없음):** `CancelerSetTargets` 재진입 제거, 싱글에서 `CancelerTargetFailure`가 배틀러 0~3을 차례로 봄, `gLastLandedMoves`·`gLastHitByType` 초기화 대상이 실제로 피한 배틀러로 바뀜(더블 비스마트 AI 교체 판단에만 영향), `TARGET_FOES_AND_ALLY` 전원 기절 때 대상이 범위 밖 값 대신 이전 대상 유지. 모두 upstream 동작이다.
- 그 밖의 메시지·스크립트 호출 순서는 같다(`RUN_SCRIPT_AND_INCREMENT`/`RUN_SCRIPT`는 옛 BREAK/PAUSE와 같은 값).

### 테스트

- **지정 파일 전후 비교:** part-A·B·C·D 문서가 지정한 파일 160개(배틀 134 + `test/battle/ai/` 26. part-C의 `gimmick/z_move.c`는 없어서 `gimmick/zmove.c`로 대신함)를 파일별로 돌렸다.
  - 이식 후 1,690줄(PASS 852)이 `test-baseline-seq119.txt`의 같은 줄과 모두 같다. 사라진 PASS 0, 새 PASS 0, 상태 변화 0.
  - 실패 사유도 비교했다. 이식 전 코드(작업 트리를 스크래치에 복사하고 이 커밋의 diff를 되돌린 사본, 14개 파일이 HEAD와 바이트 동일)에서 같은 160개 파일을 돌렸다. 상태 목록(확장, 1,698줄)과 실패 사유 목록(660줄, 파일·줄 번호 포함)이 이식 후와 **완전히 같다**. 실패 사유는 `Unmatched MESSAGE` 579줄, 그리고 그 밖의 81줄이다. 81줄은 `PASSES_RANDOMLY`의 `observed 0.0`, MESSAGE가 맞지 않아 `NOT` 구간이 끝까지 이어진 `Matched ANIMATION`, ASSUME 실패 등이며 모두 이식 전과 같은 줄이다.
- **`ai_pp_stall_prevention.c`:** "AI_FLAG_PP_STALL_PREVENTION: AI will stop using moves that has hit into immunities due to switches sometimes" **PASS 유지**(이식 전후 모두).
- **AI 핵심:** `check_bad_move.c` 보이지않는주먹 방어 2건, `ai_switching.c` 전부 0 대미지·흡수 특성 교체와 팔라피나 9건, `ai.c` 반감 열매·Bolt Beak·확정 급소, `ai_doubles.c` 집단구타 등 part-D가 적은 PASS가 모두 유지됐다.

### 전체 테스트

| 항목 | 이식 전(seq 119) | 이식 후(seq 120) |
|---|---|---|
| 러너 요약 | PASSED 2,321 / FAILED 2,243 / KNOWN_FAILING 10 / ASSUMPTIONS_FAILED 38 / TO_DO 611 / EXPECT_FAILING 6 / TOTAL 5,229 | 같음 |
| 표준 목록 | 5,160줄(PASS 2,318 / FAIL 2,218 / TO_DO 609 / KNOWN_FAILING 10 / EXPECTED_FAIL 5) | 5,160줄, **`test-baseline-seq119.txt`와 바이트 동일** |
| 확장 목록 | 5,219줄(ASSUMPTION_FAIL 37 / INVALID 21 / CRASH 1 포함) | 5,219줄, 같은 분포 |

- **사라진 PASS 0.** 설명이 필요한 줄이 없다.
- 새 기준 목록: [`test-baseline-seq120.txt`](test-baseline-seq120.txt)(표준 추출 5,160줄, seq 119 목록과 같은 내용).

### 임시 테스트 (관통드릴·프리폴 무게, 저장소에 남기지 않음)

part-C 6절의 관통드릴 테스트에 대조 2건과 프리폴 무게 확인(part-B의 선택 확인 1)을 더해 `test/battle/hns_tmp_seq120.c`로 만들어 돌린 뒤 **삭제했다**. 남은 `build/hns-test`의 오브젝트와 `pokehns-test.elf`도 지우고 다시 링크했다. 다시 링크한 ELF에 `HnS temp` 문자열은 0개다. 삭제 뒤 `git status --short --untracked-files=all`에는 이식한 14개 파일만 나왔다. 이식 전 결과는 위의 되돌린 스크래치 사본에서 같은 파일을 돌려 얻었다.

| 테스트 | 이식 전 | 이식 후 |
|---|---|---|
| 관통드릴(`Ability(ABILITY_PIERCING_DRILL)` 마자용) 할퀴기(접촉)가 방어를 뚫음: 방어 애니메이션 → 할퀴기 애니메이션 → HP 바 | PASS | PASS |
| 관통드릴 물대포(비접촉)는 방어에 막힘(`NOT ANIMATION`) | PASS | PASS |
| 관통드릴 없는(텔레파시) 할퀴기는 방어에 막힘 | PASS | PASS |
| 프리폴: 부유 동탁군(187 kg) → 들어 올림 | PASS | PASS |
| 프리폴: 헤비메탈 동탁군(374 kg) → 실패 | **FAIL**(들어 올림) | PASS |
| 프리폴: 클리어바디 레지스틸(205 kg) → 실패 | PASS | PASS |
| 프리폴: 라이트메탈 레지스틸(102.5 kg) → 들어 올림 | **FAIL**(실패) | PASS |
| 프리폴: 가벼운돌 레지스틸(102.5 kg) → 들어 올림 | **FAIL**(실패) | PASS |

이식 후에는 프리폴 다섯 경우를 `PARAMETRIZE` 1개 테스트(5/5 PASS)로, 이식 전에는 러너가 첫 실패 매개변수에서 멈추므로 다섯 개의 개별 테스트로 돌렸다.

- 남은 위험:
  - 낮음: 랜덤 대상 absent 검사 `gAbsentBattlerFlags & (1u << cv->battlerAtk)`는 upstream 오기(사실상 죽은 분기, `SetRandomTarget`이 이미 처리)다. #10459(seq 447) 문맥 때문에 고치지 않았다.
  - 낮음: 캔슬러가 진입 때 만든 특성·도구 스냅샷을 쓴다. 스크립트를 실행하면 다시 들어오며 새로 만들기 때문에 값의 시점은 이전과 같다. `gPotentialItemEffectBattler`가 공격자 대신 마지막 배틀러로 남지만, 이 값은 e-Reader 나무열매 이름에만 쓰인다.
  - 메가솔라는 자동 테스트가 없다(HnS에 메가솔라 테스트 없음). 실기로 확인한다.
- 실기 확인: 필요(아래).

## 실기 확인 항목 (친구용)

이식 전 ROM(`de9b581a28`, SHA1 `d439ac3b…`)과 이식 후 ROM(`aa175914e9`, SHA1 `2ef9b346…`)을 비교한다.

1. **프리폴 무게 경계(변경점):**
   - 헤비메탈 동탁군에게 프리폴 → `상대 동탁군은(는)\n너무 무거워서 들 수 없다!`(이전에는 들어 올림).
   - 라이트메탈 레지스틸, 가벼운돌을 지닌 레지스틸 → 들어 올림(이전에는 같은 "너무 무거워서" 문구).
   - 특성 없는 200 kg 이상(예: 메타그로스) → 이전처럼 실패. 공격자가 틀깨기면 헤비메탈 동탁군도 들어 올리는지.
2. **메가솔라:** 메가진화한 메가니움이 맑음이 아닐 때 솔라빔·솔라블레이드를 쓰면 특성 팝업 뒤 충전 없이 바로 공격하는지, 실제 쾌청(만능우산 없음)에서는 팝업 없이 바로 공격하는지. 파워풀허브를 가진 다른 포켓몬의 솔라빔(허브 소모 문구)과 충전 턴(`…은(는) 빛을 흡수했다!`)도 이전과 같은지.
3. **잠자기의 불면·의기양양 문구:** 불면·의기양양 보유자가 HP가 줄어든 상태에서 잠자기 → 특성 팝업 뒤 `STRINGID_PKMNSTAYEDAWAKEUSING`, 정화의소금 보유자 → 특성 팝업 뒤 `STRINGID_PKMNCANNOTSLEEP`. 한글 본문은 둘 다 `…은(는)\n잠들지 않는다!`이고, 팝업 특성 이름이 맞는지 본다. 이미 잠듦·HP 가득·절대안깸 보유자의 실패 문구도 그대로인지.
4. **관통드릴:** 메가진화한 몰드류의 접촉 기술이 상대 방어·킹실드·니들가드·토치카 등을 뚫고 대미지를 주는지, 이때 니들가드 반동·킹실드 공격 하락 같은 방어 부가 효과가 없는지. 비접촉 기술은 막히는지(`…은(는) 공격으로부터\n몸을 지켰다!`). 다이맥스의 다이월은 뚫지 못하는지. 보이지않는주먹(우라오스)도 같은지.
5. **PP 끌기 AI(`AI_FLAG_PP_STALL_PREVENTION` 트레이너):** 싱글에서 AI의 전기 기술을 땅 타입·축전 포켓몬으로 몇 번 받아낸 뒤 AI가 그 기술을 덜 쓰는지. 더블에서 이전과 크게 다르지 않은지(드래곤애로는 이제 세지 않음).

## 후속 행 메모

- **seq 129 #9674(매직코트·매직미러·가로챈다):** 파일 머리 bounce 3종 선언과 `CanBattlerBounceBackMove`/`TryMagicBounce`/`TryMagicCoat`가 이번에 upstream 자식과 같은 cv형이 됐다. upstream 문맥대로 들어갈 가능성이 높다. `magicBounceActive`·`moveBouncer` 구형 필드는 아직 남아 있다.
- **seq 153 #9710(`GetDefenderPartnerAbilitiesModifier`):** HnS판은 `(const struct DamageContext *ctx, enum BattlerId battlerPartnerDef)` 2인자형이다. 1인자형으로 바꿀 때 이번 이름 변경 결과를 문맥으로 본다.
- **seq 173 #9847(`SwitchAiContext`):** `ShouldSwitchIfAllMovesBad` 등의 `struct DamageContext ctx` 줄이 문맥이다.
- **seq 383 #10145:** upstream `6a3eb28ff4`의 `battle_move_resolution.c` hunk(`GetBattlerWeight(gBattlerTarget)` → `GetBattlerWeight(cv->battlerDef, cv->abilities[cv->battlerDef], cv->holdEffects[cv->battlerDef])`)는 이번 커밋의 줄과 같다. 그 hunk는 "이미 적용"으로 본다.
- **seq 447 #10459:** `CancelerSetTargets`의 `gAbsentBattlerFlags & (1u << cv->battlerAtk)` 줄을 `!IsBattlerAlive(cv->battlerAtk)`로 바꾼다(이번에 upstream대로 둔 문맥).
- **seq 466 #9859(MoveEnd cv):** `UpdateStallMons(cv)` 형태로 바뀐다. static 정의와 `MoveEndUpdateLastMoves` 호출이 upstream 자식과 같으므로 문맥이 맞는다. MoveEnd의 HnS 분기(관통드릴 `MoveEndProtectLikeEffect` 등)는 그때 옮긴다.
- **seq 475.5 #10151(Champions 설정):** `CancelerParalyzed`·`CancelerAsleepOrFrozen`이 cv형이 됐다. 정책 B(GEN_9 유지)대로 옮긴다.
- **seq 479 #10330(`TargetFailure` 재구성):** `CancelerTargetFailure`가 upstream 자식과 글자까지 같다. 이때 `updateStallMons` 설정 2곳의 위치를 다시 확인한다.
- **메가솔라 `CanTwoTurnMoveFireThisTurn`:** 1.17.0에서 `cv->moveEffect`·`GetTwoTurnMoveWeather`형으로 다시 바뀐다. 그 PR에서 HnS판(만능우산 반영·팝업 판정)을 합친다.
- **seq 470 #9939·seq 476 #10220(명중 판정·공격 전 효과 캔슬러):** 새 캔슬러는 cv형으로 들어온다. `AccuracyCheck`의 #9929 분기, #9610 내던지기 분기, HnS Champions 동작은 seq 108~119 메모대로 유지한다.
