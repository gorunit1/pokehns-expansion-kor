# full-sync 묶음 24 — seq 365 #8678 Dynamic trainerbattle Scripts + 회귀 수정 앞당김

- 요청: 사용자 "나 혼자 할 수 있는거야? 그렇다면 ㄱㄱ"(2026-10-21). 사전 분석(세션 임시 `b26/` — 로컬 클론 + 빌드 산출물 사본)으로 사용자 혼자 정할 수 있는 범위인지 판단했다. 판단 근거는 아래 "결정"이다.
- upstream: `7266185199`(#8678, 28파일 +737/−291), 회귀 수정 `12e14507c0`(#10168), `c2e2dbfda6`(#10233), `d530e3608f`(#10286), `b17f73bb35`(#10339), `7c4c378f4f`(#10462). 읽기 전용 사본 `/home/user/pokeemerald-expansion-upstream`에서 `git show`/`format-patch`로 읽었다.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 365 | #8678 | `0a086b20` | 동적 trainerbattle(아래 HnS 적응 포함) | 32,964,404 |
| (동등) | #10168 | `71325bc5` | 새 함수의 `GetRematchTrainerId` 호출에 `#if FREE_MATCH_CALL == FALSE`(1.17.0 형태, HnS는 FALSE라 코드 같음) | 32,964,404 |
| 389 | #10233 | `1c037f41` | 매크로 `trainerBattle_flags` 오타(`playMusicB`가 안 켜짐), 스크립트 두 트레이너 배틀의 `gNoOfApproachingTrainers = 2` | 32,964,452 |
| 402 | #10286 | `fdb0be06` | `trainerbattle_earlyrival`의 `continueScript TRUE`(FRLG 맵만) | 32,964,452 |
| 421 | #10339 | `c6e59024` | `skipFlagCheck` — `trainerbattle_no_intro`가 다시 트레이너 플래그를 보지 않음 | 32,964,468 |
| 435 | #10462 | `f7a23eb4` | 트레이너 방향 snippet을 `facePlayer`일 때만 | 32,964,468 |

이식 전(`4bc77b1c`) 32,963,652 B → **32,964,468 B(+816)**, EWRAM 250,408·IWRAM 25,516 같음, 최종 SHA1 **`34bb27986ebbcc57fabd79f5ef6bd53561e0f183`**. 사전 분석 사본(같은 변경을 한 번에)의 ROM SHA1과 같다.

## 무엇이 바뀌나

- 예전: `trainerbattle`의 `mode`(SINGLE, CONTINUE_SCRIPT, DOUBLE, REMATCH, NO_INTRO …)에 따라 정해진 스크립트(`EventScript_TryDoNormalTrainerBattle` 등)로 점프했다.
- 이제: `trainerbattle`이 플래그(`continueScript`·`facePlayer`·`earlyRival`·`skipFlagCheck`)를 싣고, C(`BattleSetup_Configure*TrainerBattle`)가 `EventSnippet_*`를 스크립트 스택에 쌓아 그 자리에서 배틀 스크립트를 만든다.
  - 시선 트레이너는 `ConfigureApproachingTrainerBattle`, 시설(피라미드·트레이너힐)은 `ConfigureApproachingFacilityTrainerBattle`로 간다.
  - `include/constants/battle_setup.h`(`TRAINER_BATTLE_*` 모드)와 `GetTrainerBattleMode` 등 옛 함수 5개가 없어진다.
- **HnS 맵 스크립트는 그대로다.** `trainerbattle_*` 래퍼의 인자가 같다. HnS가 쓰는 래퍼는 single 1,203·rematch 308·no_intro 238·double 87·rematch_double 44회다. 날 `trainerbattle`은 Emerald 라바리지 체육관(upstream이 고침), `earlyrival`은 FRLG 맵에만 있다. 옛 상수를 쓰는 HnS 코드는 조우 음악 함수 하나였다(아래).
- **흐름 대조(코드 해석):** 회귀 수정까지 넣은 뒤 HnS가 쓰는 경로를 옛 스크립트와 비교했다.

  | 경로 | 옛 스크립트 | 새 snippet | 차이 |
  |---|---|---|---|
  | 말 걸기 single·continue | lock, faceplayer, reveal, 플래그 → 음악, 방향, 대사, 배틀, `gotobeatenscript` | 같은 순서 | 없음 |
  | double | + 두 마리 확인 → 부족 대사 | 같음(`HasEnoughMonsForDoubleBattle2` = 같은 `GetMonsStateToDoubles`) | 없음 |
  | no_intro | reveal, 음악, 배틀, `gotopostbattlescript`(플래그 안 봄, 방향 안 바꿈) | lock, reveal, 음악, 배틀, 포스트 스크립트(#10339로 플래그 안 봄, #10462로 방향 안 바꿈) | `lock` 하나 더(이미 잠긴 스크립트에서 다시 잠금 — 결과 같음) |
  | rematch | reveal, 준비 확인, 음악, 방향, 대사, 재대결, `waitstate`, `releaseall` | lock, faceplayer, reveal, 준비 확인, 음악, 방향, 대사, 재대결(+`waitstate`), `gotobeatenscript`(NULL → `EventScript_TryGetTrainerScript` → `releaseall`) | `lock`·`faceplayer` 더 — 재대결은 말 걸기 스크립트 안에서 불려 이미 잠기고 마주 본 상태 |
  | 시선 트레이너(1·2명) | 음악, `DoTrainerApproach`, `waitstate`, 대사, (둘째) …, 배틀 | 같은 순서(HnS `waitstate` 적응) | 없음 |
  | 피라미드 시선 트레이너 | `facilitytrainerbattle` 별도 갈래 | `ConfigureApproachingFacilityTrainerBattle`(HnS `_hns` 스크립트) | 없음 |

## HnS 적응 (`0a086b20`)

1. **snippet `waitstate` 2줄(재확인 76):**
   - upstream은 400788ddf6(1.15.3~)부터 `def_special …, waitstate=1` special 뒤에 `waitstate`를 자동으로 붙인다. `DoTrainerApproach`·`BattleSetup_StartRematchBattle`가 그 대상이다. 새 snippet `EventSnippet_TrainerApproach`·`EventSnippet_DoRematchTrainerBattle`은 이 자동 대기를 전제로 `waitstate` 없이 쓰여 있다.
   - HnS `special` 매크로와 `specials.inc`에는 이 장치가 없다. HnS 옛 스크립트(`EventScript_TrainerApproach`, 재대결 스크립트)는 `waitstate`를 직접 썼다.
   - 그래서 두 snippet에 `waitstate`를 넣었다. 빠지면 `DoTrainerApproach`가 걷기 task만 만들고 바로 다음 snippet으로 넘어간다. 그러면 **시선이 마주친 트레이너가 다가오기 전에 대사와 배틀이 시작된다**.
2. **피라미드(재확인 77):** 새 `GetBattlePyramidTrainerScript()`는 `#if IS_HNS`면 `BattlePyramid_TrainerBattle_hns`를 돌려준다. `InBattlePyramid_` → `InBattlePyramid`로 이름만 바뀌고 HnS 레이아웃 조건은 그대로다(거부 hunk 손 맞춤).
3. **조우 음악:** `PlayTrainerEncounterMusic`에서 옛 no-music 모드 검사를 upstream대로 지웠다. 음악을 켤지는 이제 snippet을 쌓을지로 정한다. HnS HG 조우 음악 case 13개(`#if IS_HNS`)는 유지했다.
4. **`ScriptCall`:** HnS(#10111)의 NULL 검사는 유지하고 메시지만 1.17.0 형태(`could not push %p to %p`)로 맞췄다. `InitScriptStack`은 HnS 위치에 넣었다.
5. 손 맞춤 거부 hunk: `docs/SUMMARY.md`(튜토리얼 줄). 추가한 줄의 끝 공백 36줄을 지웠고(묶음 21·22 관례), 새 튜토리얼에 마지막 줄바꿈을 넣었다. 회귀 수정 4개도 공백 차이 때문에 upstream 내용 그대로 손으로 넣었다.

함수 단위 대조: `battle_setup.c`·`trainer_see.c`·`script.c`에서 #8678이 바꾼 함수는 회귀 수정까지 넣은 뒤 upstream 1.17.1과 같다(공백 무시). 다른 것은 HnS HG 음악 case뿐이다. 매크로 블록(`event.inc`)도 1.17.1과 같다. `trainer_battle.inc`의 차이는 HnS 옛 스크립트 `waitstate` 3줄과 snippet `waitstate` 2줄이다.

## 결정 (사용자 혼자 정할 수 있다고 본 이유)

- HnS 일반 진행의 트레이너 흐름이 옛 동작과 같게 유지된다(위 표, 코드 해석). 적응 3개는 기존 HnS 동작을 지키는 것이다.
- 회귀 수정 앞당김은 계획 원칙("리팩터를 들이면 회귀 수정도 함께", `g2_battle_fixed_b_plan.md`)과 묶음 22(#10089·#10142) 선례를 따랐다.
- 새 배틀 문장 0, 배틀 밖 새 텍스트 0(디버그 `DebugPrint*` 매크로·튜토리얼은 영문 문서). 기존 텍스트 파일 수정은 Emerald 맵 2개(라바리지 체육관, 피라미드 층 주석 1줄)뿐이다. HnS `_hns` 맵 파일은 손대지 않았다.
- 세이브 구조 변화 0(`TrainerBattleParameter`는 크기 같은 EWRAM 전역이고 세이브에 들어가지 않음 — 정적 비교 PASS).

## upstream 결함 (고치지 않음, 재확인 78)

1. `trainerbattle_single …, event_script, FALSE`(음악 끔)는 1.17.1까지 `playMusicA TRUE`를 넘긴다. 그 뒤 master가 `\music` 전달로 고쳤다. HnS 맵은 이 갈래를 쓰지 않는다.
2. `HandleEndTurn_BattleLost`의 early rival 문구 판정이 늘 참이다(1.17.0~master 같음). FRLG 22번도로 라이벌전만 해당하고 HnS는 쓰지 않는다.
3. `DebugPrintTrainerParams`가 없어진 `padding`을 가리킨다(쓰는 곳 없음, 1.17.1 같음).

## 검증

- **빌드:** 커밋마다 `make hns -j8` 종료 0, 새 경고 0(`warncheck.sh`). ROM은 위 표. `0a086b20`→`71325bc5`는 크기가 같고 SHA1만 다르다(`battle_setup.c` 줄 번호가 바뀌어 `assertf`의 `__LINE__`이 달라짐).
- **세이브 정적 비교**(이식 전 사실 = 세션 임시 `verify/save/pre-365`, 코드 `4bc77b1c`): **PASS(FAIL 0, WARN 0)**.
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 `f7a23eb4`): 러너 PASSED 2,721 / TOTAL 5,675(KNOWN_FAILING 25), 목록 PASS 2,717 — **`test-baseline-seq363.txt`와 바이트 같음**(새 PASS 0, 사라진 PASS 0). 트레이너 배틀 스크립트를 직접 도는 테스트는 저장소에 없어 흐름은 위 코드 대조로만 확인했다. 다음 기준도 `test-baseline-seq363.txt` 그대로.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-b26 8`, 코드 `f7a23eb4`): **563/652**, 기대와 바이트 같음, `판정: PASS (기대와 같음)`.
- **실기: 하지 않음(친구).**

## 실기(mGBA) 후보 — 친구

1. **시선 트레이너:** 걸어가다 눈이 마주침 → 느낌표 → 다가와서 멈춘 **뒤** 대사 → 배틀 → 패배 대사·이긴 뒤 다시 말 걸기. 두 명이 동시에 보는 곳(있으면)도 본다.
2. 말 걸기 single(이기기 전·후), 이벤트 스크립트가 이어지는 트레이너(체육관 관장 등), double(포켓몬 1마리일 때 "부족" 대사 포함).
3. `trainerbattle_no_intro` 스크립트 배틀(실버 라이벌전 등): 대사 없이 바로 배틀, 트레이너 방향이 바뀌지 않는지, 끝난 뒤 스크립트 이어짐.
4. 재대결(전화 재대결 트레이너): 재대결 준비 전·후 말 걸기.
5. 조우 음악: HG 음악(로켓단·실버·기모노걸 등)이 그대로 나오는지.
6. 배틀 프런티어 피라미드: 시선 트레이너 배틀, 배틀 뒤 힌트 문장.

## 재확인·뒤 PR 메모

- 76·77·78(새, 위).
- 순서표: 365 부분 적용, 389·402·421·435 앞당김(다시 넣지 않음). #10168은 순서표에 없는 "동등" PR이라 커밋만 남겼다.
- 뒤 PR 중 #8678에 기대는 #9970(seq 403) 등 `trainer_see.c`·`battle_setup.c` hunk는 이 구조 위에서 다시 본다(계획 `g6_overworld_refactors_plan.md` 148~150행).
