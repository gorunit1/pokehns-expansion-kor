# HnS 테스트 실패 원인 조사 보고서 (fix/test-heap)

- worktree: `<worktree>/` (브랜치 `fix/test-heap`, 시작점 `733543267f`)
- 수정 커밋
  - `7df90335e4` Free pending text printers when a battle test stops (본 과제, 힙 오염)
  - `a04eae0499` Turn off the One Type Challenge in test save blocks (포획 테스트 무한 크래시 → `make check` OOM)
- 게임 ROM 영향: **없음**. 두 커밋 모두 `test/test_runner_battle.c`, `test/test_runner.c`만 바꿨다. `test/`는 `TEST=1` 빌드에만 링크되므로 `make hns` ROM은 그대로다.
- push·merge 하지 않음. 임시 디버그 코드와 임시 테스트 파일은 모두 지웠다(`git status` 깨끗함).
- 상태: **완료** (2026-09-29 02:37 기준)

## 요약

| 항목 | 수정 전 (`check-hns-20260929-013823.log`) | 수정 후 (`a04eae0499`) |
|---|---|---|
| 실행 결과 | 도중 `Killed`(OOM) | 끝까지 실행 |
| PASSED | 243 (중단 시점) | **2,232** |
| FAILED(FAIL+INVALID) | 3,641 (중단 시점) | **2,197** |
| KNOWN_FAILING / TO_DO / EXPECT_FAILING | - | 10 / 627 / 6 |
| ASSUMPTIONS_FAILED | - | 38 |
| TOTAL | - | 5,110 |
| `malloc.c:120` 단언 실패 | 3,381건 | **0** |
| `text.c:2696 ... bytes not freed` | 다수 | **0** |
| `Illegal opcode` | 다수 | **0** |

남은 실패 2,197건 중 약 95%(2,080건)가 영문 `MESSAGE` 불일치다.

## 1. 원인 (본 과제: `malloc.c:120 block->allocated == TRUE`)

HnS가 `src/battle_message.c`의 `gText_EmptyString3`를 upstream `_("")`에서 `_(" ")`(공백 1칸)로 바꿨다. 이 변경은 `1821fd6749 Upload current HNS worktree`에 들어 있다.

1. 매 턴 시작(`FIRST_TURN_EVENTS_END`, 턴 종료 처리 끝)에 `BattlePutTextOnWindow(gText_EmptyString3, B_WIN_MSG)`가 텍스트 프린터를 힙에 할당한다(`AllocateTextPrinter`, `src/text.c:2696`).
   - upstream `""`는 첫 `RunTextPrinters`에서 곧바로 EOS를 만나 끝나고 해제된다.
   - HnS `" "`는 공백을 먼저 찍고, autoScroll 지연(테스트에서 3프레임) 뒤에야 EOS를 처리한다.
2. 테스트 러너는 마지막 `TURN` 직후 녹화 재생을 끝내고 `CB2_QuitRecordedBattle`로 간다. 이 콜백은 `RunTextPrinters`를 부르지 않으므로 공백 프린터가 `active=1, isInUse=1`로 남는다.
   - 임시 덤프로 확인한 값: `type=0(window) win=0(B_WIN_MSG) font=1 cur=<gText_EmptyString3+0x1>`
3. 통과한 테스트에서는 이 프린터가 누수로 잡힌다: `src/text.c:2696: 56 bytes not freed` → FAIL.
4. 리스트 머리 `sFirstTextPrinter`는 `EWRAM_DATA` static이다. 러너는 다음 테스트에서 `InitHeap`으로 힙만 초기화하므로, 이 포인터는 초기화된 힙 안의 옛 주소를 계속 가리킨다.
5. 다음 배틀 테스트에서 `CB2_InitBattle` → `BattleInitBgsAndWindows` → `DeactivateAllTextPrinters` → `FreeFinishedTextPrinters`가 이 포인터를 `Free`한다. 그러면 `malloc.c:120` 단언이 실패하고(`Illegal opcode: 0000efff`는 단언의 `.hword 0xEFFF`) 힙이 깨진 채 진행된다. 같은 러너 프로세스의 이후 테스트가 연쇄적으로 실패했다.
6. 영문 `MESSAGE` 불일치로 배틀 도중 중단된 테스트도 출력 중인 프린터를 남겨 같은 연쇄를 일으켰다.

검증:
- `Hail doesn't do damage when weather is negated`를 단독 실행하면 단언 없이 누수만으로 FAIL이다. Hail 테스트를 한 프로세스(`-j1`)에서 연달아 돌리면 두 번째 테스트부터 단언이 실패한다.
- 임시로 `gText_EmptyString3`를 `""`로 되돌리면 Hail 5건이 PASS로 바뀌었다. 확인 후 복구했다.

### 실제 게임 영향: 없음 (테스트 환경 문제로 분류)

- 게임도 `CB2_InitBattle`과 맵 로드에서 `MoveSaveBlocks_ResetHeap`으로 힙을 초기화한다. 따라서 그 순간 프린터가 남아 있다면 게임에서도 위험하다.
- 그러나 게임에서는 턴 시작 공백 프린터 뒤에 항상 여러 프레임이 흐른다. 행동 선택 입력을 기다리거나, 대기하는 메시지를 출력하거나, 페이드가 진행된다. 이 동안 `BattleMainCB2`의 `RunTextPrinters`가 공백 프린터를 끝내고 해제한다(지연은 최대 8프레임, 느린 텍스트 속도 기준).
- 공백 프린터 직후 곧바로 배틀을 끝내는 경로는 테스트 러너의 강제 종료뿐이다.
- 그래서 `gText_EmptyString3`(HnS 게임 데이터)는 바꾸지 않았다.

## 2. 수정 내용

### 2-1. `7df90335e4` 텍스트 프린터 정리 (`test/test_runner_battle.c`)

- `TearDownBattle()` 앞부분에서 `DeactivateAllTextPrinters()`를 호출한다. 트라이얼 사이, 테스트 종료 teardown, 중단된 배틀에서 남은 프린터를 힙이 유효할 때 해제한다.
- `CB2_BattleTest_NextParameter()`의 `TestRunner_CheckMemory()` 앞에서도 호출한다. 파라미터 테스트는 teardown 전에 메모리 검사를 하기 때문이다.
- `#include "text.h"`를 추가했다.

### 2-2. `a04eae0499` 포획 테스트 무한 크래시 (`test/test_runner.c`)

힙 수정 뒤 전체 실행에서 `Capture: ball data is properly set in captured pokemon`이 끝나지 않았다.
- mgba가 `Illegal opcode: e710b710`을 끝없이 출력했다(20초에 약 6만 줄).
- hydra가 이 출력을 테스트별로 메모리에 쌓아 RSS가 약 20MB/s씩 늘었다. 원래 로그 끝의 `make: *** [Makefile:386: check] Killed`도 같은 OOM으로 보인다.
- 테스트 타임아웃은 에뮬레이션 시간 60초 기준인데, 로그 출력 때문에 에뮬레이션이 거의 진행되지 않아 타임아웃이 사실상 발동하지 않았다.

원인(임시 추적 코드로 단계별 확인):
1. 러너의 `ClearSaveBlocks`가 SaveBlock3를 0으로 채운다. HnS에서 `tx_Challenges_OneTypeChallenge == 0`은 "꺼짐"이 아니라 "TYPE_NONE 한 타입 챌린지 켜짐"이다. 꺼짐 값은 `ONE_TYPE_OFF = 31`(`src/challenge_menu.c`)이다.
2. 그래서 포획한 몬이 `DoesSpeciesPassOneTypeChallenge`를 통과하지 못해 `CopyMonToPC`로 간다.
3. 테스트에서는 PC 박스가 초기화되지 않아 박스 이름에 EOS가 없다(0x00뿐).
4. `Cmd_givecaughtmon`의 `StringCopy(gStringVar1, GetBoxNamePtr(...))`가 `gStringVar1`(0x0203C314)에서 EWRAM 끝을 넘어, 미러링된 EWRAM 시작 부분까지 덮어쓴다. 이 과정에서 `gBattleTypeFlags`(0x020000AC)와 `gBattlersCount`(0x020000B0)가 0이 된다.
   - 확인값: 복사 직전 `n=2 flags=16777220`, 직후 `n=0 flags=0`
5. `gBattlersCount == 0`이면 `RunTurnActionsFunctions`가 `setbyte gBattleOutcome` 전에 턴을 끝낸다(`HandleEndTurn_ContinueBattle`). 이후 전투원이 없는 턴이 이어지다가 쓰레기 코드로 점프한다.

게임 영향: 없다. 실제 게임은 새 게임 챌린지 메뉴(`oak_speech_hns` → `CB2_InitChallengeMenu` → `Task_ConfirmSaveYes`)가 One Type 값을 항상 기록하고, PC 박스 이름도 초기화된다.

수정: `ClearSaveBlocks()`에서 `ClearSav3()` 뒤에 `tx_Challenges_OneTypeChallenge`를 31(`ONE_TYPE_OFF`)로 설정한다. 이 상수는 `challenge_menu.c` 내부 `#define`이다. 메인 저장소의 이식 작업과 충돌하지 않도록 게임 소스는 건드리지 않고, 러너에 `TEST_ONE_TYPE_OFF`를 두어 주석으로 연결했다.

## 3. 단일 테스트 실행 방법

`TESTS` 값은 링크 뒤 `gTestRunnerArgv`에 패치된다. `make check`는 매번 다시 링크하고 패치하므로 값만 바꿔 다시 실행하면 된다.

```sh
cd <worktree>/
# 테스트 이름 접두어 일치
make check BUILD=hns -j4 TESTS="Hail deals 1/16"
# 파일 단위(.c로 끝나면 파일 이름 정확 일치 모드)
make check BUILD=hns -j4 TESTS="test/battle/weather/hail.c"
# 이름 중간 일치(맨 앞에 * 하나)
make check BUILD=hns -j4 TESTS="*Sandstorm"
```

- 러너 프로세스 수는 `MAKEFLAGS`의 `-jN`을 따른다(최대 32). 같은 프로세스에서 연속 실행해야 재현되는 문제(이번 힙 오염 같은 경우)는 `-j1`로 확인한다.
- 실패 사유(`Unmatched MESSAGE` 등)는 hydra 요약에 러너당 50건까지만 나온다. 전체 사유가 필요하면 mgba 원시 출력을 저장한다.
  1. 먼저 `pokehns-test.elf`를 복사한다.
  2. `tools/patchelf/patchelf <복사본> gTestRunnerArgv "<필터>\0" gTestRunnerHeadless '\x01' gTestRunnerSkipIsFail '\x00' gTestRunnerN '\x04' gTestRunnerI '\x0i'`로 패치한다.
  3. worktree 루트에서 `stdbuf -oL tools/mgba/mgba-rom-test -l15 -ClogLevel.gba.dma=16 -Rr0 <복사본>`을 실행한다.
  4. 출력의 `GBA Debug: :N`(이름), `:L`(파일:줄: 사유), `:P/:F/...`(결과)를 파싱한다.

## 4. 전체 결과와 실패 분류

실행: `make check BUILD=hns -j4` (HEAD `a04eae0499`). 같은 ELF를 원시 출력 러너(4 프로세스)로도 돌렸고, 분류는 그 출력으로 했다. 두 실행의 수치는 완전히 같다.

```
- Tests FAILED :         2197
- Tests KNOWN_FAILING:   10
- Tests TO_DO:           627
- Tests EXPECT_FAILING:  6      (EXPECTED_FAIL 5 + 의도된 "Tests resume after CRASH" 1)
- Tests PASSED:          2232
- Tests TOTAL:           5110
  (ASSUMPTIONS_FAILED 38 별도)
```

### 실패 2,197건 분류

| 분류 | 건수 | 내용 |
|---|---:|---|
| **영문 `MESSAGE` 불일치** | **2,080** | |
| └ `Unmatched MESSAGE` | 1,951 | 테스트의 영문 문자열과 한글 배틀 메시지가 다르다. 폴더별: move_effect 713, ability 633, hold_effect 143, gimmick 102, move_effect_secondary 61, item_effect 44, form_change 43, sleep_clause 43, ai 36, trainer_slides 27 등 |
| └ `PASSES_RANDOMLY` observed 0.0 | 129 | 모든 트라이얼이 실패했다. 트라이얼 안의 사유는 출력되지 않지만, 모두 `MESSAGE(`를 포함한 테스트라서 MESSAGE 불일치로 추정한다 |
| **다른 로직 실패** | **117** | |
| └ `task not freed` (`Task_FreeAbilityPopUpGfx`) | 43 | HnS의 `CreateItemPopUp`(`src/battle_interface.c`)에는 `CreateAbilityPopUp`의 `gTestRunnerEnabled/Headless` 분기가 없다. 그래서 헤드리스 테스트에서도 팝업 스프라이트와 정리 태스크를 만들고, 배틀이 끝날 때 태스크가 남는다. 테스트 환경 누수이며 게임에서는 태스크가 스스로 끝난다 |
| └ `test/text.c` 폭 검사 `EXPECT_LE` | 25 | 한글 종/도구/기술/특성 이름이 화면 폭 한도를 넘는다(예: `Move names fit on Battle Screen 66 > 64`) |
| └ INVALID `requires explicit party index` | 16 | HnS는 몬스터볼류 `type`을 `ITEM_USE_PARTY_MENU`로 바꿨다(upstream `ITEM_USE_BAG_MENU`). 그래서 `USE_ITEM(볼)`에 파티 인덱스가 필요하다. 대상: capture, ball_fetch, light/heavy metal, comatose, exp, power_construct |
| └ `test/save.c` 하위 호환 크기 | 4 | SaveBlock1/2/3/PokemonStorage 크기가 HnS에서 바뀌었다 |
| └ 기타 비배틀 | 6 | bag 정렬 2, daycare 지역폼 1, species 폼 테이블 1, pokemon learnset INVALID 1, trainer_control 1 |
| └ 배틀 로직 | 23 | Fairy Aura/Aura Break/type_power/Rage Fist 배율 4, Harvest 2, Sheer Force 1, ANIMATION 매치/미매치 8, HP_BAR 1, AI 5(can_use_all_moves 동점 3, 미매치 1, Power Split 대상 1), Z-Move INVALID 1, sticky_barb `NOT x; NOT y;` 문법 INVALID 1(HnS가 수정한 테스트) |
| **크래시·타임아웃** | **0** | TIMEOUT/CRASH/단언 실패 없음. 의도된 CRASH 테스트 1건은 EXPECT_FAILING으로 통과 |

참고:
- MESSAGE 불일치는 첫 실패에서 테스트를 멈춘다. 따라서 2,080건 뒤에 다른 로직 실패가 숨어 있을 수 있다.
- NOT MESSAGE 검사는 영문 문자열이 절대 매치되지 않으므로 항상 헛통과한다.

### 실험(커밋 안 함): 테스트에 HnS 챌린지 기본값 적용

테스트는 HnS 챌린지 설정이 전부 0인 상태로 돈다. 이 상태에서는 다음 모드가 꺼져 있다.
- `tx_Mode_Fairy_Types`
- `tx_Mode_Sturdy`
- `tx_Mode_New_Citrus`
- `tx_Mode_Modern_Moves`
- `tx_Mode_Legendary_Abilities`

`ClearSav3()` 뒤에 `SetDefaultChallengeSettings()`를 추가해 한 번 돌려 봤다. 결과는 PASS 2,240(+8), FAIL 2,208(+11), ASSUMPTION_FAIL 17(-21)이었다. 페어리 관련 ASSUME가 통과하면서 테스트가 실행되었고, 대부분 다시 MESSAGE 불일치로 실패했다. 지금은 효과가 작다. MESSAGE 문제를 정리한 뒤 다시 검토할 만하다.

## 5. 실행 시간·메모리 주의

- 빈 worktree에서 처음 빌드(툴, 그래픽 변환, 컴파일, 링크, `-j4`): 약 3분 30초.
- 이후 `make check`는 매번 다시 링크하고 패치한다. 테스트 코드만 바꾼 뒤 링크까지 수십 초 걸린다.
- 전체 `make check BUILD=hns -j4`(링크 포함): **4분 55초**. 16코어 WSL2 기준이며 mgba 4개가 동시에 돈다.
- 메모리(수정 후): hydra 최대 약 18MB, mgba 4개 합계 약 170MB, 시스템 사용량 최대 약 2.1GB(실험 실행을 동시에 돌린 상태 포함). `/usr/bin/time`의 최대 RSS는 236MB.
- **주의**: 크래시 테스트가 `Illegal opcode`를 무한 출력하면 hydra가 그 출력을 전부 메모리에 쌓다가 OOM으로 죽는다. 수정 전에는 수십 초 만에 RSS가 2.5GB를 넘었다.
  - 새 크래시가 생기면 `-j`를 줄이는 것으로는 해결되지 않는다.
  - 원시 출력 러너처럼 중복 줄을 접고 감시 타임아웃을 두는 방식으로 먼저 격리한다.
- hydra는 테스트 ELF 복사본을 `<tmp>`(각 약 41MB, `-jN`개)에 만든다. 정상 종료하면 지운다.

## 부록: 조사에 쓴 임시 도구(worktree 밖, 커밋 안 함)

- scratchpad `runtest.sh`: ELF 복사, 패치, hydra 실행
- scratchpad `rawone.sh`: 단일 프로세스 원시 출력, 시간·출력 상한
- scratchpad `rawrun.sh`: N 프로세스 원시 출력, 중복 줄 압축, 15분 무결과 감시
- scratchpad `parse.py`: 결과·사유 분류
- 임시 디버그 코드: `src/text.c` 프린터 덤프, `src/main.c` 프레임 추적, `src/battle_script_commands.c`·`src/pokemon.c` 단계별 출력, `test/battle/zz_tmpdebug_capture.c`. 모두 제거했다.
