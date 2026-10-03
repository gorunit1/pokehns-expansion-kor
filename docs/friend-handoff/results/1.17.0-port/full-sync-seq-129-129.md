# full-sync 실제 port 결과 — seq 129 (#9674 매직미러·매직코트·가로채기 리팩터, + seq 274 #10386 선반영)

완료: seq 129 unit `U-magicbounce-9674`를 이식했다. 같은 unit의 후속 수정 seq 274 #10386을 바로 다음 커밋으로 선반영했다. 두 커밋 뒤 전체 테스트와 기록까지 마쳤다. 커밋 리뷰(병렬 3개)에서 나온 회귀 2건(반사자의 대상 상태)은 HnS 수정 커밋 `587f4e7cdc`로 고쳤다(아래 "리뷰 후 HnS 수정"). 다음 seq는 **130**이다(선진행으로 이미 적용, 실제로 이식할 다음 행은 seq 132). 선진행으로 넣은 seq 130·131·133·134·137·143·148·149·151·156·157·158·160·165와 이번에 선반영한 **seq 274**는 닿으면 "이미 적용"으로 처리한다.

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), 메시지 출력 기록: [`BATTLE_MESSAGE_OUTPUT_CHANGES.md`](../../../localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md)
시작 HEAD: `eb9c747212`(작업 트리 clean, 코드는 `06c6bac8c2`과 같음). 작업 컴퓨터: 데스크탑(`/usr/bin` 툴체인).

## 요약

| seq | PR | 판정 | 커밋 | ROM 변화 | 비고 |
|---|---|---|---|---:|---|
| 129 | #9674 | 적용(HnS 적응) | `49415e3007` | +848 B | 매직미러·매직코트 반사를 move end `MOVEEND_BOUNCED_MOVE`로, 가로채기를 캔슬러 `CANCELER_SNATCH`로 옮겼다. 습기·비비드바디 계열은 공용 `BattleScript_PokemonCannotUseMove`, 다크홀 판정은 `CancelerMoveFailure`로 옮겼다. HnS 적응 4곳: 캔슬러 순서 1.17.0형, 빈 비트 이름 `unused5`, 한글 이름 토큰 2개(본문 불변), 가로채기 연출 `gEffectBattler` HnS 1줄 |
| — | HnS 수정(리뷰) | 반사자 대상 상태 유지 | `587f4e7cdc` | +16 B | 튕긴 기술 뒤 반사자의 `targetsDone` 초기화, `moveTarget`을 교체 전에 기록(1.17.0 #9859 형태). 아래 "리뷰 후 HnS 수정" |
| 274 | #10386 | 적용(선반영, **seq 274 도달 시 이미 적용**) | `887c8e9ef8` | +64 B | 반사된 기술이 반사자 기준으로 move end를 `MOVEEND_CLEAR_BITS` 직전까지 돈 뒤 원래 사용자로 돌아간다(반사자 목스프레이 등). #9674 단독 중간 상태(반사자 move end 후반 생략)를 남기지 않으려고 같은 unit으로 바로 이었다. #9730·#9859 전 문맥에 맞췄다 |

- 빌드(최종, 작업 트리 = `587f4e7cdc`): 종료 코드 0, **ROM 32,716,884 B(97.50%, +928 B) / EWRAM 248,936 B(94.96%, 0) / IWRAM 25,516 B(77.87%, 0)**. `pokehns.gba` SHA1 `f453524c011ec4ed23dc7ff3e1225d98c9752c05`. (#10386 뒤 리뷰 전: 32,716,868 B, SHA1 `b5be6485…`)
- 새 경고 0(두 빌드 모두).
- 한글이 든 소스 줄 변경: `battle_message.c` 2줄의 이름 토큰만(`STRINGID_PKMNMOVEBOUNCEDABILITY` ATK→DEF, `STRINGID_PKMNSNATCHEDMOVE` DEF→ATK). 본문·조사·바이트 길이 불변. #10386 커밋은 비 ASCII 줄 변경 0.
- 세이브: 영향 없음. 바뀐 구조체는 배틀 중 힙에 잡히는 `struct BattleStruct`뿐이다.
- 지정 테스트: #9674 4파일 PASS 15 / TOTAL 44(사전 분석 C 예측과 같음). #10386 뒤 `throat_spray.c` 10/10 PASS, `parting_shot.c` PASS 4 / TOTAL 17(되살린 2개는 알려진 FAIL).
- 전체 테스트: PASSED 2,340 / FAILED 2,260 / KNOWN_FAILING 10 / TOTAL 5,261(seq 128 대비 PASS +5, TOTAL +8). 사라진 PASS 2건은 #9674가 주석 처리하고 #10386이 이름을 바꿔 되살린 막말내뱉기 반사 테스트다(알려진 FAIL). 그 밖의 PASS↔FAIL 변화와 새 INVALID 0. 새 기준 목록 [`test-baseline-seq129.txt`](test-baseline-seq129.txt).
- `BATTLE_MESSAGE_OUTPUT_CHANGES.md`: 새 행 10개(기술·필드 4, 특성·도구·도주 6)와 검증 상태 문단 1개.

## 공통 사항

- 툴체인: 데스크탑 기본 PATH `arm-none-eabi-gcc` 13.2.1(`/usr/bin`). PATH 접두어 없이 빌드했다.
- 빌드 명령: `GITHUB_ACTION=1 make hns -j8 > build/port.log 2>&1`. 커밋 1 빌드 로그는 커밋 2 빌드가 같은 파일에 덮어썼다. 커밋 1 수치는 덮어쓰기 전에 기록했다.
- 이식 전 기준(데스크탑, `06c6bac8c2` 코드): ROM 32,715,956 B / EWRAM 248,936 B / IWRAM 25,516 B, SHA1 `4b3b96bf8f924f4da6f1bb464ca8e48ebc789cec`.
- 경고 비교: `LC_ALL=C grep -a 'warning:' build/port.log | sed -E 's/:[0-9]+:[0-9]+: /: /' | sort -u | comm -13 warn-base.txt -`. 기준은 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-ahead-130-167/warn-base.txt`(파일: 메시지 고유 42개).
- 사전 분석: 읽기 전용 분석 4개가 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-129/`에 patch와 스크래치 검증을 남겼다.
  - A(`part-A.md`/`.patch`, 3파일): 판정·캔슬러·move end(`battle_move_resolution.c`, `battle_move_resolution.h`, `battle.h`). 선택 patch `tmp-A/opt-snatch-effectbattler.patch`(가로채기 연출 1줄)
  - B(`part-B.md`/`.patch`, 6파일): 스크립트 명령·배틀 스크립트(`battle_script_commands.c/.h`, `battle_scripts_1.s`, `battle_scripts.h`, `battle_script.inc`, `battle_move_effects.h`)
  - C(`part-C.md`/`.patch`, 5파일): 한글 토큰(`battle_message.c`), 테스트 4파일, 출력 변화 초안, A·B 밖 사용처 전수 조사(고칠 파일 0)
  - D(`part-D.md`/`.patch`, 4파일): #10386(seq 274) 선반영 분석
  - 적용: `git apply --check` 뒤 A → B → C → opt 순서로 쌓고(커밋 1), 그 위에 D(커밋 2). 충돌 없음. A·B·C·opt를 쌓은 뒤와 D를 얹은 뒤 `git diff --check` 통과. patch md5: A `f7f0af05…`, B `2cd33ffd…`, C `5013ae03…`, D `a694aa1e…`(사전 분석 문서에 적힌 값과 같음), opt `79477593…`.

## 동기화 단위: seq 129 #9674 `U-magicbounce-9674` Magic Bounce / Magic Coat/ Snatch refactor

- 현재 판정: 적용(HnS 적응)
- 커밋: `49415e3007`
- upstream 근거: `c532ceac79`(14파일 +492/−227). HnS 커밋도 같은 14파일, +488/−222(차이는 제외한 공백 hunk, 캔슬러 순서·`unused5`·연출 1줄 적응).
- 수정 파일(14): `src/battle_move_resolution.c`, `include/constants/battle_move_resolution.h`, `include/battle.h`, `src/battle_script_commands.c`, `include/battle_script_commands.h`, `data/battle_scripts_1.s`, `include/battle_scripts.h`, `asm/macros/battle_script.inc`, `src/data/battle_move_effects.h`, `src/battle_message.c`, `test/battle/ability/magic_bounce.c`, `test/battle/move_effect/{magic_coat,parting_shot,snatch}.c`
- 내용:
  - 반사: `TryMagicBounce()`·`TryMagicCoat()`가 배틀러별 `magicBouncePending`·`magicCoatPending` 비트만 세운다. 원래 기술이 모든 대상을 처리한 뒤 새 `MoveEndBouncedMove()`(`MOVEEND_BOUNCED_MOVE`)가 Save → 공격자·대상 교체 → `CANCELER_SET_TARGETS`부터 다시 실행한다. 상대 필드 기술은 `SortBattlersByRawSpeed()`로 가장 빠른 한 마리만 반사한다. `setmagiccoattarget`·`BS_SetMagicCoatTarget`과 `Cmd_attackcanceler`의 반사 분기는 지웠다.
  - 가로채기: `Cmd_attackcanceler`의 루프를 새 `CancelerSnatch()`로 옮겼다. Save 1회, 대상 교체, `CANCELER_VOLATILE_BLOCKED`부터 가로챈 쪽으로 다시 판정한다. `MoveEndUpdateLastMoves()`에서 `RestoreAttacker()`·`RestoreTarget()`(새 공개 함수)로 되돌린다. `snatchsetbattlers`는 빈 명령으로 남는다(opcode 표 불변).
  - 습기·비비드바디·여왕의위엄·테일아머: 공용 `BattleScript_PokemonCannotUseMove`(`BattleScript_DazzlingProtected` 개명, `BattleScript_DampStopsExplosion` 삭제). 팝업은 `gBattlerAbility`.
  - 다크홀: `BattleScript_EffectDarkVoid`를 지우고 `CancelerMoveFailure()`의 `EFFECT_DARK_VOID`로 판정한다(튕긴 다크홀은 건너뜀).
  - `BattleStruct`: `magicBounceActive`·`magicCoatActive`·`moveBouncer`·`attackerBeforeBounce`를 지우고 `magic*Pending:6`, `bouncedMoveIsUsed`를 u32 비트필드로 옮겼다.
- **HnS 적응:**
  - 캔슬러 enum·함수 표 순서: HnS는 #9942(seq 25)를 선이식해 `CANCELER_INTERRUPTIBLE_MOVES`가 `PRIORITY_BLOCK` 바로 뒤에 있었다. upstream merge `76da6ac3d8`이 정한 1.17.0 순서 `PRIORITY_BLOCK → EXPLODING_DAMP → INTERRUPTIBLE_MOVES → PROTEAN → CHARGING → SNATCH → EXPLOSION`을 썼다. 미래예지만 다루는 `INTERRUPTIBLE_MOVES`는 Damp 금지 기술이 아니어서 동작이 같다. HnS 메가솔 판정이 든 `CanTwoTurnMoveFireThisTurn`은 내용 그대로 upstream 자리로 옮겼다.
  - `include/battle.h`: upstream은 비게 된 `bouncedMoveIsUsed:1` 비트를 `unused2:1`로 바꾸지만 HnS `BattleStruct`에는 #10047(선이식)의 `unused2`가 이미 있다. `u8 unused5:1; // HnS: …`로 이름만 바꿨다. seq 166 #9784가 이 비트를 `redCardActivated`로 다시 쓰므로 그때 hunk의 `-u8 unused2:1;`를 이 줄에 맞춘다.
  - `src/battle_message.c`: 한글 2줄의 이름 토큰만 바꿨다(아래 "한글 토큰 증명"). 공백 정리 hunk 2개(`gCureStatusStringIds`·`gPartyCureStatusStringIds`)는 HnS에 이미 공백이 없어 제외했다.
  - `src/battle_script_commands.c`: `Cmd_curestatuswithmove`·`BS_CureStatus`의 줄 끝 공백 hunk 3개는 HnS `GetCuredStatusMessage()` 구현이라 해당 줄이 없어 제외했다(기능 무관). `include/battle_script_commands.h`는 HnS `CanFireMoveThawTarget` 시그니처 때문에 문맥만 맞췄다.
  - **가로채기 연출 HnS 보호 1줄**(메인 결정): `CancelerSnatch()`의 `snatchedMoveIsUsed = TRUE;` 다음에 `gEffectBattler = gBattlerAttacker; // HnS: B_ANIM_SNATCH_MOVE reads gEffectBattler as the robbed battler (old snatchsetbattlers set it)`. 이식 전 `Cmd_snatchsetbattlers`가 넣던 값(원래 사용자)이고, `gBattleAnimGeneral_SnatchMove`의 `AnimTask_SetAnimAttackerAndTargetForEffectTgt`(`src/battle_anim_utility_funcs.c`)가 이 값을 연출 대상으로 읽는다. upstream은 `snatchsetbattlers`를 비우면서 이 설정도 없앴고 **upstream 1.17.0 `CancelerSnatch`에도 같은 잠재 버그가 남아 있다**(지난 효과의 배틀러가 남아 파트너용 연출이 나올 수 있음). 메시지·판정·테스트 결과에는 영향이 없다(사전 분석 D 전체 테스트: opt 유무 결과 같음).
  - `include/battle_scripts.h`의 정의 없는 `BattleScript_DampStopsExplosion` extern은 upstream 1.17.0처럼 남겼다(참조 0).
- 제외한 hunk: 줄 끝 공백 정리 5개(위). 그 밖에는 없다.
- 보존 확인: HnS 고유 줄(관통드릴 `MoveEndProtectLikeEffect`, 집단구타 챌린지 `GetMaxPartySize`, 참기 `// HnS:`, `#include "challenge_menu.h"`, 메가솔)은 hunk 밖이거나 위치만 옮겼다. 구형 필드 4개의 `src`·`include` 잔존 참조 0. 사용 config `B_SNATCH`·`B_DARK_VOID_FAIL`은 `GEN_LATEST` 그대로.
- 빌드(`49415e3007` 작업 트리): 종료 코드 0, **ROM 32,716,804 B(+848 B) / EWRAM 248,936 B(0) / IWRAM 25,516 B(0)**, SHA1 `f9b8e6bc4fa4287a8bbdef503fab970214db1e4e`. 경고 21줄이 모두 기준 안(새 경고 0). 사전 분석 D의 스크래치 ROM(ABC+opt 32,716,804 B)과 같다.
- 지정 테스트(아래 "지정 테스트" 표): 4파일 PASS 15 / TOTAL 44.
- 남은 위험:
  - **중간 상태(seq 181 #9730 전까지):** 가로채기 재진입이 `CANCELER_SNATCH - 1`(= `CANCELER_CHARGING`)이라, 가로챈 쪽에 대해서는 `CancelerMoveFailure`(HP 가득일 때 회복 실패 등)를 다시 보지 않고 원래 사용자 기준 판정이 적용된다. #9730이 `CANCELER_SET_TARGETS - 1`로 바꾼다.
  - 막말내뱉기 반사 중간 상태(seq 181까지, 아래 #10386 절).
  - 가로챈 멀리짖기 assert(아래 "친구에게 물을 것" 1).
  - **커밋 리뷰 A가 찾은 것(upstream 로직, 1.17.0도 같은 코드):**
    - 탈출버튼·탈출팩으로 들어온 포켓몬이 같은 턴에 반사하거나 가로채면, 저장한 공격자가 복원되지 않아 `make hns`에서 그 배틀 끝까지 기술마다 assert 화면이 뜬다(아래 "친구에게 물을 것" 2).
    - **중간 상태(seq 181 #9730 전까지):** 가로챈 쪽은 `CANCELER_SET_TARGETS`를 건너뛰므로 대상 판정이 엉뚱한 포켓몬에 적용된다(원래 사용자의 전기엔진·방음 등 팝업, 사이코필드 문구, 더블에서 다른 포켓몬의 방어·공중 상태 문구). 출력 변화 문서에 행을 넣었다.
  - pending 비트는 처리될 때만 지워진다(upstream 같음). `MOVEEND_BOUNCED_MOVE`를 건너뛰는 범위 지정(`moveendto MOVEEND_NEXT_TARGET` 등)으로 끝나는 스크립트는 모두 TargetFailure 전에 끝나거나 반사할 수 없는 기술이다(사전 분석 A 위험 3, B 적용 뒤 목록이 upstream과 같음).
  - `CancelerSnatch`의 else 분기(대상이 자기 자신이 아닌 가로챈 기술)는 `{B_SCR}`를 설정하지 않는다. HnS config(`B_UPDATED_MOVE_FLAGS`·`B_UPDATED_MOVE_DATA` = `GEN_LATEST`)에서는 도달하지 않는다. config를 낮추면 다시 검토한다.
- 실기 확인: 필요(아래 "실기 확인 항목").

## 동기화 단위: seq 274 #10386 `U-magicbounce-9674` Fixes bounced moves not activating some effects (선반영)

- 현재 판정: 적용(선반영). **seq 274에 닿으면 "이미 적용(seq 129 unit에서 선반영, `887c8e9ef8`)"으로 건너뛴다.**
- 커밋: `887c8e9ef8`
- upstream 근거: `9621cae24a`(4파일 +80/−38). HnS 커밋도 같은 4파일, +80/−38.
- 선반영 이유: #9674 단독 상태에서는 반사된 기술의 move end 후반(HP 임계 도구, 다중 타격, 해동, 반동, 생명의구슬, 탈출버튼·레드카드·탈출팩, 하양허브·흉내허브, 목스프레이·과사열매·허탕보험, 교체 처리)이 원래 공격자 기준으로 한 번만 돈다(반사자 목스프레이 미발동, 원래 사용자 목스프레이가 대신 발동 등). #10386이 이를 1.17.0 구조로 되돌리므로 같은 unit으로 바로 이었다.
- 수정 파일(4): `include/battle.h`, `src/battle_move_resolution.c`, `test/battle/hold_effect/throat_spray.c`, `test/battle/move_effect/parting_shot.c`
- 내용: `MoveEndBouncedMove()`는 반사 중이면 Restore 없이 다음 단계로 넘어간다. 반사 직전에 `savedMoveResultFlags`를 저장한다. `MoveEndClearBits()` 앞머리에서 반사 중이면 Restore, `moveResultFlags` 복원, `MOVEEND_BOUNCED_MOVE`로 되돌아간다(다음 반사자나 원래 사용자). `IsTargetingBothFoes()`가 변화 기술에서도 사용자 파트너를 "영향 없음"으로 표시한다.
- **HnS 적응(손으로 맞춘 hunk 4개, 논리는 upstream과 같음):**
  - `battle.h`: `u32 savedMoveResultFlags[MAX_BATTLERS_COUNT]; // for Bounced moves`를 `moveResultFlags` 바로 뒤(1.17.0과 같은 자리)에 넣었다. HnS에는 다음 줄에 아직 쓰지 않는 `noResultString`(#9730이 삭제)이 있다.
  - `IsTargetingBothFoes`: 조건만 지우고 값은 HnS 현재 `MOVE_RESULT_NO_EFFECT`로 두었다. upstream의 `MOVE_RESULT_DOESNT_AFFECT_FOE`는 #9730(seq 181)이 바꾼 값이다.
  - `MoveEndBouncedMove`·`MoveEndClearBits`: 시그니처 `(void)`, 대상 배틀러는 `gBattlerAttacker`(교체 뒤 반사자) 그대로. upstream의 `cv->` 형태는 #9859(seq 466) 몫이다.
  - `throat_spray.c`: 새 테스트를 HnS 마지막 테스트("Sheer Force") 뒤에 붙였다(upstream 문맥인 "just switched in" 테스트는 #9784(seq 166)가 넣는다). `parting_shot.c`는 upstream 그대로다.
- 커밋 메시지는 사전 분석 D 9절 초안에 선반영 1줄을 더했다.
- 빌드(`887c8e9ef8` 작업 트리): 종료 코드 0, **ROM 32,716,868 B(+64 B) / EWRAM 248,936 B(0) / IWRAM 25,516 B(0)**, SHA1 `b5be6485b965ddd7e29ec63923c02c2c6cb3c5ad`. 경고 21줄 모두 기준 안(새 경고 0). 사전 분석 D 스크래치 예측(+64 B, EWRAM·IWRAM 0)과 같다.
- 지정 테스트: `throat_spray.c` PASS 10 / TOTAL 10(새 테스트 "activates on user and bouncer …" PASS). `parting_shot.c` PASS 4 / FAIL 13 / TOTAL 17. 되살린 2개 "Magic Coat/Magic Bounce bounces it and switches the target out and original user doesn't switch out"가 `TURN 1 incomplete`로 FAIL하고 나머지 15개는 #9674 커밋 결과와 같다. `magic_bounce.c`·`magic_coat.c`·`snatch.c`는 테스트별 결과가 #9674 커밋과 같다.
- **알려진 FAIL(seq 181에서 풀림):** 되살린 막말내뱉기 테스트 2개. HnS에서는 막말내뱉기 교체가 아직 `BattleScript_EffectPartingShotSwitch`의 `moveendall` 뒤 `BattleScript_MoveSwitchPursuitEnd`라서, `moveendall` 안의 `MoveEndClearBits`가 원래 사용자로 Restore한 뒤 원래 사용자를 교체한다. 교체를 move end(`MoveEndHitEscape`)로 옮기는 #9730(seq 181) 이식 뒤 PASS가 확인 항목이다(메인 결정: upstream대로 넣음).
- 남은 위험:
  - 막말내뱉기 반사 중간 상태가 seq 181까지 남는다(출력 변화 행 기록). HnS 트레이너 파티에 막말내뱉기는 없다.
  - 반사된 기술의 move end 후반이 반사자 기준으로 한 번 더 돈다(이식 전 HnS·1.17.0 구조로 돌아감). 전체 테스트에서 기존 테스트의 PASS↔FAIL 변화는 0이다. 영문 `MESSAGE` 때문에 FAIL인 테스트의 내부 순서는 테스트로 다 볼 수 없다.
  - `bouncedMoveIsUsed` 유지 구간이 반사자의 `MOVEEND_CLEAR_BITS`까지 길어진다. `moveendall` 없이 끝나는 반사 가능한 기술 경로는 없다(사전 분석 D 위험 5).
  - 가로챈 멀리짖기 assert는 그대로다(#10386은 `bouncedMoveIsUsed` 경로만 바꿈, 사전 분석 D 실측 D8).
- 실기 확인: 필요(아래 "실기 확인 항목" 6·7).

## 리뷰 후 HnS 수정: 반사자의 대상 상태 (`587f4e7cdc`)

커밋 리뷰 D가 #9674/#10386 뒤 생긴 회귀 2건을 찾았다. 둘 다 이식 전 HnS에서는 정상이던 동작이다. 리뷰어가 스크래치에서 만든 수정안을 메인이 넣었다.

| 문제 | 원인 | 수정 |
|---|---|---|
| 매직미러·매직코트로 튕긴 포켓몬의 **다음 행동**에서 대상 판정(방어·무효·면역)이 빠진다. 예: 튕긴 다음 턴 반사자의 몸통박치기가 내 방어를 뚫음, 같은 턴 반사자의 전기자석파가 땅 타입을 마비시킴, 더블에서 반사자의 범위기가 한 대상을 빠뜨림 | `MoveEndClearBits()`의 반사 조기 반환이 반사자의 `targetsDone`을 지우지 않는다. 초기화는 일반 경로에만 있고, 그때 `gBattlerAttacker`는 이미 원래 사용자다. `ShouldSkipFailureCheckOnBattler()`가 그 대상을 건너뛴다 | 조기 반환 분기에서 `RestoreAttacker()` **앞에** 반사자의 `targetsDone`을 지운다(`// HnS:` 2줄). upstream 1.17.0도 원인이 같고 증상만 다르다(반사자의 다음 공격 피해 0). upstream master에도 아직 수정이 없다 |
| 더블배틀에서 단일 대상 변화기를 튕긴 반사자의 **같은 턴 행동**이 원래 사용자에게 간다 | `MoveEndBouncedMove()`가 공격자·대상을 바꾼 **뒤** `moveTarget[gBattlerAttacker] = gBattlerTarget`을 써서 반사자의 선택 대상을 덮어쓴다(`HandleAction_UseMove`가 읽음) | 같은 줄을 교체 **전**으로 옮겼다(`// HnS:` 주석). upstream 1.17.0(#9859)의 `moveTarget[cv->battlerAtk] = cv->battlerDef`와 같은 의미다 |

- 검증
  - 빌드: 종료 코드 0, ROM +16 B, 새 경고 0.
  - 리뷰어 임시 테스트 16개를 저장소에 잠시 두고 돌려 모두 PASS했다(커밋하지 않음, 돌린 뒤 지움).
    - 싱글: 매직미러·매직코트 뒤 다음 턴 방어, 같은 턴 땅 타입 전기자석파, 가로채기 뒤 방어·땅 타입
    - 더블: 범위기 대상, 같은 반사자의 두 번 반사, 반사자 둘, 반사자의 선택 대상 유지(단일·범위)
    - 수정 전 커밋에서는 같은 테스트가 FAIL했다(리뷰 D 실측).
  - 전체 테스트(`587f4e7cdc`, `build/port-check-post129fix.log`): PASSED 2,340 / FAILED 2,260 / TOTAL 5,261. 추출 목록 5,192줄이 `test-baseline-seq129.txt`와 **바이트 단위로 같다**. INVALID 21개 같음, Killed 0.
- 1.17.0과의 관계: `moveTarget`은 1.17.0 형태가 됐다. `targetsDone`은 반사자 기준 HnS 형태다. 1.17.0 형태(`cv->battlerAtk`)로 바꾸면 같은 반사자가 한 턴에 두 번 반사하는 경우(리뷰 D R3)가 FAIL한다.
- 사전 분석 D 3.2의 D10("두 대상 모두 맞음")은 HP 바만 봐서 이 문제를 잡지 못했다(범위 피해는 첫 패스에서 모든 대상의 HP를 깎음).

## 지정 테스트

`GITHUB_ACTION=1 make check BUILD=hns -j8 TESTS="test/battle/<파일>.c"`. 로그 `build/port-check-129-*.log`(#9674 커밋), `build/port-check-274-*.log`(#10386 커밋). 기준은 [`test-baseline-seq128.txt`](test-baseline-seq128.txt).

| 파일 | seq 128 | #9674 뒤 | #10386 뒤 | 바뀐 것 |
|---|---|---|---|---|
| `ability/magic_bounce.c` | PASS 2 / 9 | PASS 3 / 12 | 같음 | 새 3개: "bounces back status moves before Magic Coat" PASS, "activates on all opposing mons"·"will trigger after all valid targets have been targetted" FAIL(MESSAGE) |
| `move_effect/magic_coat.c` | PASS 2 / 5 | PASS 3 / 6 | 같음 | 새 "activates on the fastest opposing mon for hazard setting moves (raw speed)" PASS |
| `move_effect/parting_shot.c` | PASS 6 / 17 | PASS 4 / 15 | PASS 4 / 17 | #9674가 "Magic Coat/Magic Bounce bounces it and switches the target out" PASS 2개를 주석 처리, #10386이 이름을 바꿔 되살림(알려진 FAIL 2) |
| `move_effect/snatch.c` | PASS 1 / 8(TO_DO 2) | PASS 5 / 11 | 같음 | 새 PASS 4개(Disable·Imprison·Heal Block·Throat Chop), "can steal healing moves" TO_DO 삭제, "does not steal moves that cannot be snatched" TO_DO → FAIL(MESSAGE) |
| `hold_effect/throat_spray.c` | PASS 9 / 9 | — | PASS 10 / 10 | 새 "activates on user and bouncer if at least one target if affected by sound move" PASS |

- #9674 4파일 합계: PASS 11 → 15, TOTAL 39 → 44. 사전 분석 C 3.2 표와 테스트별로 같다. FAIL 29건의 사유는 모두 `Unmatched MESSAGE`다.
- #10386 뒤 non-MESSAGE FAIL은 되살린 막말내뱉기 2개(`TURN 1 incomplete`)뿐이다.

## 한글 토큰 증명 요약 (사전 분석 C 2절)

문장은 `printstring` 시점의 `gBattlerAttacker`(ATK)·`gBattlerTarget`(DEF)·`gBattleScripting.battler`(SCR)로 이름을 채운다. `{B_ATK_NAME_WITH_PREFIX}`(FD 0F)와 `{B_DEF_NAME_WITH_PREFIX}`(FD 10)는 둘 다 2바이트라 인코딩 길이·창 너비가 같다. 조사(`{B_TXT_EUNNEUN}` 등)는 바로 앞에 펼친 이름으로 고르므로 같은 포켓몬이면 같다.

| 문장 | 이식 전 출력 시점 | 이식 후 출력 시점 | 토큰 | 화면 이름 |
|---|---|---|---|---|
| `STRINGID_PKMNMOVEBOUNCEDABILITY` `{토큰}의\n{기술}{을를} 되받아쳤다!` | `Cmd_attackcanceler`가 부른 `BattleScript_MagicBounce`에서 `setmagiccoattarget` **전**. ATK = 원래 사용자 | `MoveEndBouncedMove`가 `BattleScriptCall` 직후 공격자·대상을 바꾼 뒤. ATK = 반사자, DEF = 원래 사용자 | ATK → **DEF** | 원래 사용자 → 원래 사용자(같음). 토큰을 그대로 두면 반사자로 바뀐다(사전 분석 C 실측 T1: A+B만 넣은 사본 FAIL) |
| `STRINGID_PKMNSNATCHEDMOVE` `{토큰}{은는} {B_SCR}의\n기술을 가로챘다!` | `snatchsetbattlers` 뒤 ATK = DEF = 가로챈 쪽, SCR = 원래 사용자 | `CancelerSnatch` 자기 대상 분기: ATK = DEF = 가로챈 쪽, SCR = 원래 사용자 | DEF → **ATK** | 같음. HnS에서 가로챌 수 있는 기술은 모두 `TARGET_USER`이거나 멀리짖기(첫 대상이 자신)라 늘 이 분기를 탄다. upstream과 같게 맞춰 else 분기 주어와 뒤 PR 문맥도 맞음 |
| `STRINGID_PKMNMOVEBOUNCED`(매직코트) | `setmagiccoattarget` 뒤 ATK = 매직코트 사용자 | ATK = 매직코트 사용자 | 바꾸지 않음 | 같음 |
| `STRINGID_POKEMONCANNOTUSEMOVE`(습기·비비드바디 계열) | ATK = 사용자, 팝업 `AbilityPopUpScripting` | ATK = 사용자, 팝업 `AbilityPopUp`(`gBattlerAbility` = 보유자) | 바꾸지 않음 | 같음 |

- 사전 분석 C가 스크래치에서 한글 `MESSAGE` 임시 테스트(T1 매직미러, T2 매직코트, T3 가로채기)로 이식 전·후 모두 PASS를 확인했다. 임시 테스트는 저장소에 넣지 않았다.
- 튕긴·가로챈 기술 안에서 출력되는 다른 문장은 이전에도 이후에도 DEF가 원래 사용자다. 이전 `setmagiccoattarget`의 SCR 설정에 기대는 HnS 전용 `{B_SCR}` 문장은 없다.

## 세이브·메모리

- 바뀐 구조체는 `struct BattleStruct` 하나다. `gBattleStruct`는 EWRAM 포인터이고 본체는 `AllocateBattleResources()`의 `AllocZeroed`로 힙에 잡힌다. 세이브 블록·녹화 배틀·링크 코드는 `gBattleStruct`를 쓰지 않는다. **일반 세이브 영향 없음.**
- `sizeof(struct BattleStruct)`(사전 분석 사본 컴파일): 996 B → #9674 뒤 996 B → #10386 뒤 1,012 B(+16, `savedMoveResultFlags`). 힙 사용만 16 B 늘고 정적 EWRAM은 두 빌드 모두 0 B 변화다.
- 배틀 스크립트 opcode 표는 바뀌지 않았다(`setmagiccoattarget`은 `callnative`, `snatchsetbattlers` opcode는 빈 명령으로 남음).

## 출력 변화

`BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 새 행 10개를 넣었고, 커밋 리뷰 뒤 2행(가로챈 쪽 대상 판정 중간 상태, 탈출버튼·탈출팩 뒤 반사·가로채기 assert)을 더해 12개다. 모든 행에서 문자열 ID·한글 본문 변화는 없고(이름 토큰 2개는 표시 같음), 순서·발동 조건·주체만 바뀐다. 새 한글 문장은 없다. seq 128 → #10386 뒤 기준으로 적었다.

- 「기술·필드 상태 효과」 4행: 다크라이가 아닌 다크홀(사용 문구·PP 소모), 가로챈 쪽의 회복봉인·지옥찌르기 재판정, **막말내뱉기 반사 중간 상태(seq 129~180)**, **더블 가로챈 멀리짖기 assert**.
- 「특성·도구·도주」 6행: 더블 범위기 한쪽 반사 순서, 상대 둘 다 반사(두 번 반사, 순서는 seq 166 #9784 뒤 실제 스피드 순), 상대 필드 기술 반사자(실제 스피드 최고), 변환자재·리베로 + 습기(습기 먼저), 더블 소리 변화 범위기의 반사자 목스프레이, 소리 변화 범위기가 둘 다 막힐 때 사용자 목스프레이 미발동(#10386).
- 넣지 않은 행: 가로채기 연출 조건부 행(HnS 1줄로 이전 연출 유지), #10386이 되돌리는 #9674 중간 상태 중 목스프레이 행. #9674 단독에서만 생기던 "싱글에서 튕긴 소리 기술에 원래 사용자 목스프레이 발동"도 #10386 커밋으로 해소되어 행이 없다.
- 문서 정정: 사전 분석 A 4절·C 4.1이 반사 순서를 실제 스피드 순으로 바꾸는 PR을 #9957(seq 193)로 적었으나 실제로는 **#9784(seq 166)**의 `MoveEndBouncedMove` hunk다. 출력 변화 행에는 #9784로 적었다. #9957은 정렬 함수의 남은 정의만 지운다.

### upstream대로 둔 동작 변화 (배틀 메시지 아님)

- **분함의발구르기·열불내기 위력 2배 조건이 넓어진다(#10386).** 더블배틀에서 상대 둘을 노리는 변화 기술(달콤한향기·울음소리 등)을 상대 한쪽이 매직미러·매직코트로 튕기고 다른 쪽은 영향을 받았을 때, 같은 사용자가 다음 턴에 쓰는 분함의발구르기·열불내기 위력이 2배가 된다(사전 분석 D 실측 D15: 피해 32 → 63). `ShouldSetStompingTantrumTimer()`가 사용자 파트너(이제 `NO_EFFECT`)와 반사자(`FAILED`, 복원된 결과)를 "영향 없음"으로 세어 `numSpreadTargets`와 같아지기 때문이다. upstream 1.17.0도 같은 계산이다(코드 대조, 1.17.0 실측은 안 함). 반사 없이 한쪽이 공중날기·구멍파기로 피해서 `FAILED`가 될 때도 같을 수 있다(코드 추정, 미확인). 방음은 `MISSED`라 세지 않는다. "일부 대상에게 통했는데도 실패로 취급"하는 upstream 쪽 결함으로 볼 수도 있으나 D4 원칙대로 upstream과 같게 두었다.

## 전체 테스트

- 명령: `GITHUB_ACTION=1 make check BUILD=hns -j6 > build/port-check-post129.log 2>&1`(데스크탑, 작업 트리 = `887c8e9ef8`). `make` 종료 코드 2는 실패 테스트가 있을 때의 정상 종료다. `Killed`·러너 크래시 0.
- 결과: PASSED **2,340** / FAILED 2,260 / KNOWN_FAILING 10 / ASSUMPTIONS_FAILED 38 / TO_DO 607 / EXPECT_FAILING 6 / TOTAL **5,261**. seq 128(PASS 2,335 / FAILED 2,255 / TO_DO 609 / TOTAL 5,253) 대비 PASS +5, FAILED +5, TO_DO −2, TOTAL +8. 사전 분석 D의 스크래치 예측(PASS 2,340 / TOTAL 5,261, INVALID 21 같음)과 같다.
- 목록 비교: `PORT_INSTRUCTIONS.md`의 `LC_ALL=C`·`grep -a` 추출(5,192줄: PASS 2,337 / FAIL 2,235 / KNOWN_FAILING 10 / TO_DO 605 / EXPECTED_FAIL 5)을 [`test-baseline-seq128.txt`](test-baseline-seq128.txt)(5,184줄)와 비교했다.
  - **사라진 PASS 2건:** `Parting Shot: Magic Bounce bounces it and switches the target out`, `Parting Shot: Magic Coat bounces it and switches the target out`. #9674가 주석 처리하고 #10386이 이름에 "… and original user doesn't switch out"을 붙여 되살렸다. 되살린 2개는 `TURN 1 incomplete`로 FAIL한다(알려진 FAIL, seq 181 #9730에서 풀림 예상). 사전 분석 C·D 예측과 같다.
  - **새 이름 11줄:** PASS 7(Magic Bounce "before Magic Coat", Magic Coat "raw speed", Snatch Disable·Imprison·Heal Block·Throat Chop, Throat Spray "user and bouncer"), FAIL 4(Magic Bounce "activates on all opposing mons"·"will trigger after all valid targets have been targetted"는 `Unmatched MESSAGE`, 되살린 Parting Shot 2개는 `TURN 1 incomplete`). 되살린 Parting Shot 2개는 이름이 바뀐 기존 테스트다.
  - **사라진 이름 3줄:** 위 Parting Shot 2개, `Snatch can steal healing moves`(TO_DO, upstream 삭제).
  - **상태가 바뀐 기존 테스트 1개:** `Snatch does not steal moves that cannot be snatched` TO_DO → FAIL(`Unmatched MESSAGE`, upstream이 TO_DO를 실제 테스트로 바꿈).
  - 그 밖에 PASS↔FAIL로 바뀐 테스트는 0이다. 매직코트·매직미러·가로채기·습기·비비드바디·변환자재·다크홀·목스프레이·탈출팩을 함께 쓰는 Damp·Dazzling·Protean·Libero·Dark Void·Sleep Clause·Dancer·Pressure·Prankster·Eject Pack 등도 기준과 같다.
- INVALID(추출 목록 밖): 이름 21개가 seq 128 로그(`build/port-check-post128.log`)와 같다. 새 assert 0(가로챈 멀리짖기 경로는 upstream 테스트에 없음).
- 새 기준 목록: [`test-baseline-seq129.txt`](test-baseline-seq129.txt)(5,192줄). 리뷰 후 HnS 수정(`587f4e7cdc`) 뒤 전체 테스트도 이 목록과 바이트 단위로 같다(위 "리뷰 후 HnS 수정").

## 커밋 리뷰 (병렬 3개, 읽기 전용)

리뷰 지시: 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-129/REVIEW.md`. 리뷰어는 커밋 기준 코드를 upstream·1.17.0·사전 분석·메인 결정과 대조하고, 스크래치 사본에서 임시 테스트로 이식 전·후·1.17.0을 실측했다.

| 리뷰 | 대상 | 판정 | 내용 |
|---|---|---|---|
| A | `49415e3007` 판정 영역 | 경미 | 3파일이 upstream `c532ceac79`와 같고 HnS 차이는 이식 전후 같음(새 차이는 메가솔 함수 위치 이동, 가로채기 연출 1줄). 캔슬러 순서 1.17.0형, `unused5`, `// HnS:` 보존 확인. 문서에 없던 upstream 동작 2건을 실측으로 찾음: 탈출버튼·탈출팩 뒤 반사·가로채기 assert("친구에게 물을 것" 2), 가로챈 쪽 대상 판정 중간 상태(출력 변화 행). 둘 다 문서에 반영 |
| B | `49415e3007` 스크립트·메시지·테스트 | 문제 없음 | upstream hunk 대조(차이는 공백 hunk 5개 제외·문맥 1·한글 2줄), 지운 심볼 잔존 0, opcode 표 upstream과 같음, 튕긴 기술 89개의 `attackcanceler` 앞 명령 재실행 안전. 한글 토큰을 임시 한글 테스트 6개로 이식 전·`49415e3007`·`887c8e9ef8`에서 모두 PASS 확인. 이전 토큰을 넣으면 매직미러 3개가 FAIL(변경 필요 입증). 테스트 4파일 upstream과 바이트 동일 |
| D | `887c8e9ef8`, 결과 문서 | 수정 필요 2 → 반영 | 반사자의 `targetsDone` 미초기화, 반사자 `moveTarget` 덮어쓰기(위 "리뷰 후 HnS 수정", `587f4e7cdc`). 손으로 맞춘 4곳은 문제 없음(HnS 반사자 기준 `targetsDone`이 1.17.0보다 나음). 결과 문서 사실(빌드·테스트·출력 변화 6행 코드 확인·후속 메모 번호)은 맞음. D10 결론 보충과 seq 466 메모 정정을 반영 |

리뷰 B의 참고 2건: `Cmd_attackcanceler`의 `!IsStatChangeMove` 조건은 #9730(seq 181) 몫, `BattleScript_DampStopsExplosion` extern 잔존은 upstream 1.17.0과 같음(조치 없음).

## 친구에게 물을 것

1. **더블배틀에서 가로챈 멀리짖기 → assert 화면(upstream대로 둠).**
   - 무슨 일: 더블배틀에서 상대가 쓴 멀리짖기(`TARGET_USER_AND_ALLY`)를 가로채면, 두 번째 대상(파트너)으로 move end를 다시 돌 때 `MoveEndUpdateLastMoves()`가 `RestoreAttacker()`를 한 번 더 불러 저장 스택이 비어 `assertf("No savedBattlerAttackers")`가 실패한다. `make hns`(RELEASE=0) 빌드에서는 재개 가능한 assert 화면이 뜬다. `make release`는 복구 코드(Restore 생략)로 진행한다.
   - 이식 전: 같은 상황에서 능력이 하나도 오르지 않는 결함이 있었지만 화면은 뜨지 않았다(사전 분석 C 실측 T11c `6666`).
   - 이 결함은 upstream c532·1.17.0·master에 모두 있다(#10386 뒤에도 같음, 사전 분석 D 실측 D8).
   - 발생 조건은 좁다. 가로채기를 쓰는 쪽과 멀리짖기를 쓰는 상대가 같은 더블배틀에 있어야 한다. HnS 트레이너 파티 명시 기술에는 둘 다 없지만, 기술을 적지 않은 트레이너 포켓몬은 레벨업 기술로 멀리짖기를 쓸 수 있다.
   - 선택지: (a) 그대로 둔다(지금). (b) HnS 보호 수정. 단순 가드(저장 스택이 비어 있으면 Restore 생략)는 두 번째 대상이 원래 사용자의 파트너가 되어 올바른 동작이 아니다. 가로챈 쪽과 그 파트너가 오르게 하려면 설계가 더 필요하다. 수정한다면 seq 127처럼 친구 승인 뒤 별도 커밋으로 한다.

2. **탈출버튼·탈출팩으로 들어온 포켓몬의 반사·가로채기 → 그 배틀 끝까지 assert 화면(upstream대로 둠, 커밋 리뷰 A).**
   - 무슨 일: 탈출버튼·탈출팩으로 교체돼 들어온 포켓몬이 같은 턴에 매직미러로 반사하거나, 이전 포켓몬에게서 남은 매직코트·가로채기 상태로 반사·가로채면 문제가 생긴다.
     - 공격자를 저장한 뒤 `Cmd_attackcanceler`의 `usedEjectItem` 분기가 기술을 버리므로 복원이 일어나지 않는다.
     - 그래서 `make hns`(RELEASE=0)에서는 그 배틀이 끝날 때까지 이후 모든 기술의 move end에서 `ValidateBattlers` assert 화면이 다시 뜬다.
     - 반사 대기 비트가 둘이면 남은 비트가 다음 기술로 넘어갈 수 있다(코드로만 확인).
   - 이식 전: 기술만 사라지고 assert는 없었다(리뷰 A 실측 R3·R3b·R4: 이식 전 PASS, 이식 뒤 INVALID). upstream 1.17.0도 같은 코드다.
   - 덧붙여, `gProtectStructs`의 `bounceMove`·`stealMove`가 교체 때 지워지지 않아 새로 들어온 포켓몬이 이전 포켓몬의 매직코트·가로채기를 물려받는다. 이것은 이전부터 있던 동작이다.
   - 선택지
     - (a) 그대로 둔다(지금).
     - (b) HnS 보호 수정. 예: `usedEjectItem` 조기 반환을 `!bouncedMoveIsUsed && !snatchedMoveIsUsed`일 때만 한다(1줄). 이 경우 교체돼 들어온 매직미러 포켓몬이 그대로 반사한다. 물려받은 매직코트·가로채기도 그대로 발동하므로, 교체 때 두 상태를 지우는 수정도 함께 검토하는 편이 좋다.
   - 멀리짖기(1)보다 화면이 반복해서 뜨므로 고칠 가치가 더 크다고 본다. 고친다면 seq 127처럼 친구 승인 뒤 별도 커밋으로 한다.

## 실기 확인 항목 (친구용)

이식 전 ROM(`06c6bac8c2` 코드, SHA1 `4b3b96bf…`)과 이식 후 ROM(`587f4e7cdc`, SHA1 `f453524c…`, 데스크탑 툴체인)을 같은 `.sav`로 비교한다. 문장 주체(이름)·조사, 팝업 주체, 애니메이션 방향, 순서를 본다.

1. **매직미러:** OBC 뒤 나츠메(`TRAINER_SABRINA_POSTOBC_HNS`, 에브이)·추(`TRAINER_WILL_POSTOBC_HNS`, 네이티오)에게 독가루·전기자석파·스텔스록 → 팝업 뒤 `[내 포켓몬]의 [기술]을(를) 되받아쳤다!`(원래 사용자 이름)와 반사된 효과.
2. **매직코트:** 상대 변화기를 튕김 → `[매직코트 사용자]은(는) [기술]을(를) 되받아쳤다!`
3. **가로채기:** 가로채기 → 상대 칼춤 → `[가로챈 쪽]은(는) 상대 [원래 사용자]의 기술을 가로챘다!`. 가로채기 연출에서 가로챈 포켓몬이 원래 사용자 쪽으로 다녀오는지(HnS 1줄로 유지한 이전 연출).
4. **습기:** 습기 포켓몬 앞에서 대폭발·자폭 → 습기 팝업 → `…은(는) 대폭발을 쓸 수 없다!` → 다음 행동 정상 진행.
5. (가능하면) **더블배틀 째려보기를 한쪽 매직미러에:** 튕기지 않은 상대 하락 문구가 먼저, 그 뒤 팝업·되받아침 문장·사용자 편 하락.
6. **매직미러에 막말내뱉기(#10386 뒤 중간 상태):** 지금은 반사자가 아니라 **내 포켓몬(원래 사용자)이 교체**되고 반사자는 남는 것이 예상 동작이다. 반사자가 교체되는 원래 동작은 seq 181 #9730 뒤에 돌아온다. 교체할 포켓몬이 없으면 능력 하락만 일어나는지도 본다.
7. **반사자 목스프레이:** 목스프레이를 지닌 매직미러 포켓몬에게 울음소리 → 팝업 → 반사 → **반사자의 목스프레이 발동**(특공↑).
8. (가능하면) 다크라이가 아닌 포켓몬이 손가락흔들기 등으로 다크홀 → `X는 다크홀을 썼다!` → `하지만 X는 사용할 수 없었다!`
9. **반사자의 다음 행동(리뷰 후 HnS 수정 확인):** 매직미러 포켓몬에게 변화기를 튕기게 한 뒤, 다음 턴에 내가 방어 → 반사자의 공격이 막히는지. 같은 턴 반사자가 느릴 때 땅 타입에게 전기자석파 → `효과가 없는 것 같다`. 더블에서 반사자가 다른 상대를 노렸을 때 그 대상에게 가는지.

## 후속 행 메모

- **seq 274 #10386:** 이미 적용(`887c8e9ef8`). 닿으면 재이식하지 않고 결과 문서에 "이미 적용(seq 129 unit 선반영)"으로 적는다.
- **seq 166 #9784(Eject Items):**
  - `include/battle.h`: upstream hunk의 `-u8 unused2:1;`를 HnS `u8 unused5:1; // HnS: …` 줄에 맞춰 `redCardActivated`로 바꾼다.
  - `MoveEndBouncedMove`의 `battlers[]`·`SortBattlersByRawSpeed` → `gBattlersByRawSpeed` hunk는 #10386 뒤 줄 수가 1 줄어 offset만 생긴다(반사 순서가 실제 스피드 순으로 바뀌는 시점, 출력 변화 "상대 둘 다 반사" 행).
  - `test/battle/hold_effect/throat_spray.c` 끝 hunk는 #10386 테스트 때문에 충돌한다. "just switched in" 테스트를 #10386 테스트 **앞**, "Sheer Force" 뒤에 넣는다(upstream 순서).
- **seq 181 #9730(Stat Change Refactor):**
  - `IsTargetingBothFoes`: 조건은 이미 없다. 남은 한 줄의 값만 `MOVE_RESULT_DOESNT_AFFECT_FOE`로 바꾼다.
  - `include/battle.h`: `noResultString`만 지운다(위에 `savedMoveResultFlags`가 있음).
  - 가로채기 재진입 `CANCELER_SNATCH - 1` → `CANCELER_SET_TARGETS - 1`, PP 차감 제외에 `snatchedMoveIsUsed` 추가(위 "중간 상태" 해소).
  - 가로챈 쪽 대상 판정 중간 상태(커밋 리뷰 A 실측: 가로챈 충전 → 상대 전기엔진, 가로챈 치유방울 → 상대 방음)가 풀리는지 확인하고 출력 변화 행을 고치거나 지운다.
  - 막말내뱉기 교체가 move end로 옮겨지면 되살린 parting_shot 테스트 2개 PASS를 확인한다. 그때 `BATTLE_MESSAGE_OUTPUT_CHANGES.md`의 "막말내뱉기를 튕김 (seq 129~180 중간 상태)" 행을 고치거나 지운다.
- **seq 193 #9957(Guard Dog):** `SortBattlersByRawSpeed` 함수의 남은 정의를 지운다(D hunk와 겹치지 않음).
- **seq 466 #9859(CalcValue to MoveEnd):** `MoveEndBouncedMove`·`MoveEndClearBits`는 #10386과 리뷰 후 HnS 수정(`587f4e7cdc`) 뒤 형태라 upstream hunk가 기대하는 문맥이 없다. `cv` 치환을 손으로 한다. **`targetsDone`은 HnS 반사자 형태를 유지**한다(1.17.0의 `cv->battlerAtk` 형태면 같은 반사자의 두 번 반사, 리뷰 D R3이 FAIL). **`moveTarget`은 이미 1.17.0 형태**(교체 전 기록)라 그대로 둔다. `MoveEndClearBits` 조기 반환의 `targetsDone` 초기화 2줄(`// HnS:`)도 유지한다.
- **seq 479 #10330(TargetFailure):** 단계형 `TARGET_FAILURE_BOUNCE`, 반사 대상의 `gLastLandedMoves`·`gLastHitByType` 초기화. 이번 `CancelerTargetFailure`의 반사 판정 구간이 문맥이다.
- 가로챈 멀리짖기 assert, 탈출버튼·탈출팩 뒤 반사·가로채기 assert를 HnS 보호 수정으로 고치기로 하면 별도 커밋으로 하고, 출력 변화 행을 고친다.
- 사전 분석 문서(`chunk-129/part-A.md` 4절, `part-C.md` 4.1)의 "#9957(193)" 표기는 "#9784(166)"가 맞다.
