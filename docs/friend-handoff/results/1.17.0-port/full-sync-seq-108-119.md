# full-sync 실제 port 결과 — seq 108~119

진행 중: 마지막 완료 seq 114, 다음 seq 115.

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), [`CLAUDE_FULL_SYNC_PORT_PROMPT.md`](../../CLAUDE_FULL_SYNC_PORT_PROMPT.md), 메시지 출력 기록: [`BATTLE_MESSAGE_OUTPUT_CHANGES.md`](../../../localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md)
시작 HEAD: `10077a5d70` (작업 트리 clean)

## seq 108~119 요약

| seq | PR | 판정 | 커밋 | ROM 변화 | 비고 |
|---|---|---|---|---:|---|
| 108 | #9537 (+#9668) | 적용(HnS 적응) | `70eb6a4271` | 0 B (SHA1 동일) | `OBJ_EVENT_GFX_*` 명시 값 유지·이름만 변경, FRLG 맵 2개 수동 hunk, `spritesheet_rules.mk` 규칙 3개 이름(#9668 동등, 오타 제외) |
| 109 | #9241 | 적용(HnS 적응) | `ec9dca2712` | +32 B | `#if IS_HNS` 그래픽을 새 구조체 표 HnS판으로 옮김, (class, 그래픽) 쌍·세이브 값 불변 |
| 110 | #9595 | 이미 적용 | 없음(`5121b83c94`) | 0 | seq 83 unit에서 적용 |
| 111 | #9610 | 적용(HnS 적응) | `47cd51facf` | +112 B | `AccuracyCheck` #9929 분기 유지, 한글 2문장 토큰만 교체(`{B_BUFF1}`→`{B_LAST_ITEM}`), 폴터가이스트+대타출동 문장 미출력(upstream대로, 출력 변화 문서 1행) |
| 112 | #9587 | 적용 | `2d86edca81` | +16 B | upstream 그대로. 교체 후보 시뮬레이션 등 캐시 속도 미반영 경계 사례는 upstream 1.17.0과 같은 동작 |
| 113 | #9596 | 부분 적용(HnS 적응) | `f6ef307f76` | −112 B | `holdEffectParams` 캐시 제거·`ShouldTryOHKO` 기합의띠 판정만. 턴 순서 hunk는 seq 100에서 1.17.0 최종형으로 이미 대체 |
| 114 | #9532 | 적용(HnS 적응) | `7e4f61c927` | +32 B | opcode `UNUSED_32/33`(0xfd/0xfe, upstream과 헤더·명령 표 바이트 동일), 방출 턴 연출 `animTurn = 1`(`// HnS:`), 2·3턴째 공격 문구 생략(출력 변화 문서 1행) |

## 공통 사항

- 빌드 명령: `PATH=/opt/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin:$PATH GITHUB_ACTION=1 make hns -j8 > build/port.log 2>&1`(노트북 WSL).
- 기준 빌드(`10077a5d70`, `rm -rf build/hns` 뒤 전체 재빌드, `build/port-pre108-full.log`, 59초): 종료 코드 0, ROM 32,716,084 B / EWRAM 248,924 B / IWRAM 25,516 B. `pokehns.gba` SHA1 `0c91520caa7dca02ada0115f9657501147c9927a`(seq 107 결과와 같음). 경고 165줄(링커 RWX 2줄 포함), "파일: 메시지"(줄·열 번호 제거) 고유 43개. 매 빌드의 경고를 같은 형식으로 만들어 이 목록과 비교했다.
- 테스트 이식 전 목록: `10077a5d70`은 `f2a0395e90`(seq 107 코드) 뒤에 docs만 바뀌었으므로 seq 107 이식 후 전체 실행 로그(`build/port-check-post107.log`)를 이식 전 결과로 썼다. 표준 목록(`LC_ALL=C`·`grep -a`)은 [`test-baseline-seq107.txt`](test-baseline-seq107.txt)와 바이트 동일(5,142줄), 확장 목록(`ASSUMPTION_FAIL|INVALID|TIMEOUT|CRASH` 포함)은 5,201줄(PASS 2,311 / FAIL 2,207 / TO_DO 611 / KNOWN_FAILING 8 / EXPECTED_FAIL 5 / ASSUMPTION_FAIL 37 / INVALID 21 / CRASH 1).
- 한글 포함 소스 줄: `git show <커밋> | LC_ALL=C grep -a -cP '^[-+](?![-+]).*[^\x00-\x7F]'`와 한글 음절(U+AC00~D7A3) 검사.

## 사전 분석 (읽기 전용 분석 에이전트)

이식 전에 분석 에이전트가 저장소를 수정하지 않고 PR별 이식 계획·적응 패치·검증 도구를 만들었다. 산출물은 세션 스크래치(`/tmp/claude-1000/-home-jinmo-pokehns-expansion-kor/fc35c7ef-06d7-4f6e-94a6-07439474a39e/scratchpad/chunk-108-119/`)에 있어 **세션이 끝나면 사라진다**. 적용 직전에 각 패치를 `git apply --check`로 다시 확인하고 upstream diff와 현재 코드를 대조했다.

| 문서 | 요지 |
|---|---|
| `seq108-9537.md` (+`.patch`, `a108/gfx_values.sh`·`gfx_compare.sh`, 이식 전 표 `seq108-gfx-values-HEAD-10077a5d70.txt`) | `event_objects.h`의 HnS 명시 값 보존, FRLG 맵 2개 문맥 적응, `spritesheet_rules.mk` 규칙 3개(#9668의 오타 없는 결과)를 함께 넣는 권장 패치. hns 오브젝트가 전후 같아 ROM SHA1이 같아야 한다고 예측 |
| `seq109-9241.md` (+`.patch`, `a108/tower_values.py`) | `battle_tower.c`의 `#if IS_HNS` 그래픽 배열을 새 구조체 배열 HnS판으로 옮김. (class, 그래픽) 쌍 전후 동일, 세이브 `facilityClass`(u8) 불변 |
| `seq111-9610.md` (+`.patch`, `w-111-117-119/stringid_table.sh`·`stringid_bytes.py`) | `AccuracyCheck`의 #9929 분기 유지 + 내던지기 분기. 한글 2문장 토큰만 교체(`{B_BUFF1}`→`{B_LAST_ITEM}` 필수). 폴터가이스트+대타출동 문장 미출력(upstream 동작) |
| `seq112-9587.md` (+`.patch`, `ai-harness/run_ai_tests.sh`) | upstream 그대로. 교체 후보 시뮬레이션 등 캐시 속도 미반영 경계 사례는 upstream 1.17.0과 같은 동작 |
| `seq113-9596.md` (+`.patch`) | 최소 패치: `holdEffectParams` 캐시 제거와 `ShouldTryOHKO` 기합의띠 판정만. 턴 순서 hunk는 seq 100에서 1.17.0 최종형으로 이미 대체 |
| `seq114-9532.md` (+`.patch`, 선택 `-animturn-optional.patch`) | 참기 리팩터. opcode `UNUSED_32/33` 추가. 방출 턴 연출 유지용 선택 1줄 |
| `seq115-9494.md` (+`.patch`, 선택 `-primalweather-optional.patch`, `seq176-9864.patch`) | 교체 대기열. HnS 아이템 팝업·Champions 상성·#9790·#9946 보존, `NeutralizingGasExits` sBATTLER 저장·복원 필수. #9864는 바로 뒤, #9784는 넣지 않음 |
| `seq116-9557.md` (+`.patch` 변형 B) | upstream이 `IS_FRLG` 가드를 지워 그대로면 ROM +110 KB·Emerald 빌드 실패. 변형 B는 미리보기 그림·표를 `#if MPS_ENABLE_MAP_PREVIEWS`로 감쌈 |
| `seq117-9578.md` (+`.patch`) | `BattleScript_ShedSkinActivates` hunk 제외, 미사용분만 삭제. group plan의 "HealerActivates hunk 제외" 표기 정정 |
| `seq118-9630.md` (+`.patch`, `ai-harness/roll9630/`) | upstream 그대로. 나눗셈 전수 증명·mGBA 롤 하네스로 롤 값 불변 확인 |
| `seq119-9634.md` (+`.patch`) | 한글 문장 한 줄 삭제. 대체 문장 `STRINGID_CLOAKEDINAHARSHLIGHT`는 이미 한글로 출력 중 |

## 동기화 단위: seq 108 #9537 `U-frlggfx-9537` Fix Kanto object event graphic names (+ seq 126 #9668)

- 현재 판정: 적용(HnS 적응)
- 커밋: `70eb6a4271`
- upstream 근거: `7da42d1a95`(35파일 +180/−180), 같은 unit의 #9668 `ca828643b7`(`spritesheet_rules.mk` 3줄), 관련 #10281 `ea1043afdf`
- 수정 파일(36): FRLG `map.json` 25개, PNG 3개 이름 변경(`battle_girl`→`crush_girl`, `super_nerd`→`poke_maniac_frlg`, `blackbelt`→`black_belt_frlg`, 100% rename), `include/constants/event_objects.h`, `spritesheet_rules.mk`, `src/data/object_events/` 4개, `src/fame_checker.c`, `src/trainer_tower.c`
- 적용 방법: 사전 분석 권장 패치(`seq108-9537.patch`)를 `git apply`로 넣었다. 넣기 전 확인: HnS 적응 4파일(`event_objects.h`, `spritesheet_rules.mk`, `OneIsland_KindleRoad_EmberSpa_Frlg`·`Route15_Frlg` `map.json`)을 뺀 32파일의 `+`/`-` 줄 집합이 upstream `7da42d1a95`와 같다(347줄).
- HnS 적응:
  - `event_objects.h`: HnS는 #9066 이식 때 `= 259` 같은 명시 값을 두었다. 이름만 바꾸고 값(259·279·281)과 열 맞춤을 유지했다.
  - FRLG 맵 2개: HnS 사본에 `"type": "object"` 줄이 없어 `graphics_id` 3줄만 같은 이름으로 바꿨다.
  - `spritesheet_rules.mk`: HnS는 아직 INCBIN + 규칙 파일 방식이다(#9881 INCGFX는 seq 500). 규칙 이름을 바꾸지 않으면 새 이름 `.4bpp`가 일반 규칙(`-mwidth/-mheight` 없음)으로 만들어져 FRLG 빌드에서 그림 타일 순서가 깨진다. #9668의 결과를 오타(`poke_manic_frlg`) 없이 `crush_girl`·`black_belt_frlg`·`poke_maniac_frlg`로 같은 커밋에 넣었다. 따로 두면 seq 108~125 사이 커밋의 FRLG 빌드가 깨지므로 한 커밋이 자연스럽다.
- 제외한 hunk: 없음.
- HnS 보존: HnS 맵은 `*_HNS` 그래픽(`BATTLE_GIRL_HNS`·`SUPER_NERD_HNS`·`BLACK_BELT_HNS`)만 쓴다. 이름이 바뀐 세 상수는 FRLG 맵 25개·`#if IS_FRLG` 데이터·`trainer_tower.c`·`fame_checker.c` 주석에서만 쓰인다. Emerald `OBJ_EVENT_GFX_BLACK_BELT`(44)와 `black_belt.png`는 그대로다.
- 검증:
  - `git diff --check` 통과, 파일 모드 변경 없음(PNG rename만). 추적 파일에 옛 이름 참조 0(`git grep -w`; 남은 것은 빌드가 다시 만드는 무시 대상 `events.inc`뿐).
  - **오브젝트 그래픽 값 표**(`a108/gfx_values.sh`·`gfx_compare.sh`, POKEMON_HNS 554개): 값이 바뀐 이름 0, 값 집합(중복 포함) 동일, 이름만 바뀐 항목 3개(259 `BATTLE_GIRL`→`CRUSH_GIRL`, 279 `SUPER_NERD`→`POKE_MANIAC_FRLG`, 281 `BLACKBELT`→`BLACK_BELT_FRLG`). 이식 전 표는 사전 분석 표(`seq108-gfx-values-HEAD-10077a5d70.txt`)와 바이트 동일. HnS 맵 `graphics_id` 가운데 이식 후 정의되지 않는 이름 0.
  - 빌드(`build/port.log`): 종료 코드 0, **ROM 32,716,084 B / EWRAM 248,924 B / IWRAM 25,516 B(모두 0)**, 새 경고 0. **`pokehns.gba` SHA1 `0c91520caa7dca02ada0115f9657501147c9927a` — 이식 직전과 같다.**
  - 규칙 이름: 빌드가 새 이름 `.4bpp` 3개를 규칙(`-mwidth 2 -mheight 4`)으로 만들었고, 이전 이름 `.4bpp`(`battle_girl`·`super_nerd`·`blackbelt`)와 3개 모두 바이트 동일(`cmp`).
- 테스트: ROM 바이트 동일이라 따로 돌리지 않았다(구간 끝 전체 실행에 포함). 직접 관련 테스트 없음.
- 뒤 행 처리 예정:
  - **seq 126 #9668:** 도달하면 "HnS 동등(seq 108 커밋 `70eb6a4271`에 포함, upstream 오타 `poke_manic_frlg`는 넣지 않음)"으로 기록하고 커밋하지 않는다.
  - **seq 250 #10281:** upstream hunk는 INCGFX 줄(`gObjectEventPic_PokeManiacFrlg`에 `-mwidth 2 -mheight 4`)이다. HnS 규칙 이름이 이미 올바르므로 도달하면 "HnS 동등"으로 기록한다. seq 500 #9881(INCGFX) 이식 때 `gObjectEventPic_PokeManiacFrlg` 줄에 `-mwidth 2 -mheight 4`가 붙는지 확인한다.
- 남은 위험: 없음(ROM 동일). FRLG 빌드는 이 PR과 무관하게 이식 전부터 `src/map_preview_screen.c`의 HnS 줄 `MAPSEC_ROCKET_HIDEOUT_HNS` 때문에 실패한다(seq 116 참고).
- 실기 확인: 필요 없음(ROM SHA1 동일).

## 동기화 단위: seq 109 #9241 `U-9241` Consolidated Battle Tower classes and object events

- 현재 판정: 적용(HnS 적응)
- 커밋: `ec9dca2712`
- upstream 근거: `7b0b0b6fdd`(5파일 +91/−143). 1.17.0의 `include/battle_tower.h`·`gTower*FacilityClasses` 정의와 같은 형태.
- 수정 파일(5): `include/battle_tower.h`, `src/apprentice.c`, `src/battle_special.c`, `src/battle_tower.c`, `src/frontier_util.c`
- 적용 방법: 사전 분석 패치(`seq109-9241.patch`)를 `git apply`로 넣었다. `battle_tower.c`를 뺀 4파일의 `+`/`-` 줄 집합이 upstream과 같다. `battle_tower.c`의 `#else` 블록은 upstream `7b0b0b6fdd`의 배열 정의와 빈 줄을 뺀 글자가 같다(`diff` 0).
- HnS 적응: HnS는 `const u8 g…FacilityClasses[]` 두 개 뒤에 `#if IS_HNS` / `#else`로 그래픽 배열 두 벌(`OBJ_EVENT_GFX_*_HNS` / 원본)을 두었다. 새 형태는 `#if IS_HNS` 안에 HnS 그래픽으로 짝지은 `const struct FacilityClass gTowerMale/FemaleFacilityClasses[]`, `#else` 안에 upstream과 같은 두 표다. `SaveBattleTowerRecord`의 `.class`는 upstream대로.
- 제외한 hunk: 없음. `gTower*` 사용처는 upstream hunk와 1:1이고 HnS 고유 사용처는 없다(`git grep`).
- 검증:
  - `git diff --check` 통과, 파일 모드 변경 없음.
  - **(class, 그래픽) 쌍**(`a108/tower_values.py`, 이식 전 파일은 이식 전 `battle_tower.h`로, 이식 후 파일은 새 헤더로 각각 컴파일): POKEMON_HNS·EMERALD·FIRERED 모두 남 30/30·여 20/20 SAME. 예) HnS 남 첫 쌍 (14, 423) (17, 453) (3, 414) (21, 406).
  - **세이브:** 세이브에 들어가는 것은 표에서 꺼낸 class 값이다. `SaveBattleTowerRecord`가 `u8 class = …[i].class`로 `playerRecord->facilityClass`(u8)에 넣고, `battle_special.c`(`UNUSED` 함수)는 `ereaderTrainer->facilityClass`(u8), `apprentice.c`·`frontier_util.c`는 u8 class와 비교한다. 필드가 `u16 class`가 되어도 값(최대 `FACILITY_CLASSES_COUNT` 139 미만)이 같다. 표는 ROM 상수라 세이브 배치와 무관하다.
  - 빌드: 종료 코드 0, **ROM 32,716,116 B(+32 B) / EWRAM 248,924 B(0) / IWRAM 25,516 B(0)**, 새 경고 0. (표 150 B → 200 B, 코드 약간 감소. 사전 분석 추정 +42 B)
- 테스트: 직접 관련 테스트 없음(`test/`에 `gTower`·`FacilityClassToGraphicsId`·apprentice 참조 0). 구간 끝 전체 실행에 포함.
- 남은 위험: 낮음. 이후 upstream PR이 이 표를 문맥으로 쓰면 HnS `#if IS_HNS` 블록 때문에 hunk 문맥 적응이 필요할 수 있다(1.17.0까지 이 배열 정의를 다시 바꾸는 커밋은 없음).
- 실기 확인: 필요(아래 "실기 확인 항목" 1).

## 동기화 단위: seq 110 #9595 `U-anim-8497` Fix move anim pal blending being discarded

- 현재 판정: 이미 적용
- 커밋: 없음. 적용 커밋 `5121b83c94`(seq 83 #8497 unit에서 선반영, 진행 기록 `838acd9430`)
- 근거: upstream `c1e0532fe2`는 `Cmd_waitforvisualfinish`의 `UnloadAllSpritePalettes()` 호출과 주석 9줄 삭제다. 현재 `src/battle_anim.c` 997행 `Cmd_waitforvisualfinish`에 그 호출이 없다(남은 `UnloadAllSpritePalettes`는 정적 함수 정의와 다른 함수의 호출 1곳뿐, upstream 이후와 같음).
- 실기 확인: seq 83 항목에 포함.

## 동기화 단위: seq 111 #9610 `U-9610` Makes Poltergeist and Fling messages into a pre attack effect

- 현재 판정: 적용(HnS 적응)
- 커밋: `47cd51facf`
- upstream 근거: `e16cc7a1f2`(11파일 +186/−40). 선행 #9176(seq 59, `2e93d86ed1`) 적용됨. #9929(seq 17, `2be66d7011`)가 같은 `AccuracyCheck` 분기를 이미 바꿔 두었다.
- 수정 파일(12): `asm/macros/battle_script.inc`, `data/battle_scripts_1.s`, `include/battle_scripts.h`, `include/constants/battle.h`, `src/battle_message.c`, `src/battle_move_resolution.c`, `src/battle_script_commands.c`, `src/data/battle_move_effects.h`, `src/data/moves_info.h`, `test/battle/move_effect/fling.c`, `test/battle/move_effect/poltergeist.c`, `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`
- 적용 방법: 사전 분석 패치(`seq111-9610.patch`)를 `git apply`로 넣었다. 패치와 upstream의 `+`/`-` 줄 집합 차이는 아래 HnS 적응 두 곳(한글 2문장, `AccuracyCheck` 삼항식)뿐이다. `battle_scripts.h` 선언 위치와 `fling.c` 새 테스트 위치는 문맥만 다르다. 파일 모드 변경 없음.
- HnS 적응:
  - `AccuracyCheck`(`battle_script_commands.c`): HnS #9929 삼항식(`failInstr == BattleScript_ButItFailed ? BattleScript_TargetAvoidsAttackEnd : failInstr`)을 유지하고 앞에 upstream의 `EFFECT_FLING → BattleScript_FlingMissed` 분기를 두었다. 내던지기의 새 스크립트(`BattleScript_EffectHit`)의 실패 경로는 `MoveMissedPause`라 두 분기가 겹치지 않는다. upstream은 master→upcoming 병합에서 #9929 분기를 잃었고 1.17.0에서는 #9939가 이 함수를 캔슬러로 옮긴다(그때 다시 합친다).
  - `battle_scripts.h`: `FlingMissed`·`FlingMessage` 선언을 #9929의 `BattleScript_TargetAvoidsAttackEnd` 줄 뒤에 넣었다.
  - `fling.c`: HnS 파일 끝의 #9782 테스트 2개(멘탈허브·하양허브) 때문에 새 테스트 3개를 1.17.0과 같은 위치(Booster Energy 테스트 뒤)에 넣었다.
  - 한글 2문장(토큰만, 본문 바이트 동일):

    | STRINGID | 이전 | 이후 |
    |---|---|---|
    | `STRINGID_PKMNFLUNG` | `{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_LAST_ITEM}{B_TXT_EULREUL} 내던졌다!` | `{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_LAST_ITEM}{B_TXT_EULREUL} 내던졌다!` |
    | `STRINGID_ABOUTTOUSEPOLTERGEIST` | `{B_BUFF1}{B_TXT_IGA}\n{B_DEF_NAME_WITH_PREFIX}에게 덤벼들었다!` | `{B_LAST_ITEM}{B_TXT_IGA}\n{B_EFF_NAME_WITH_PREFIX}에게 덤벼들었다!` |

    `{B_BUFF1}` 교체는 필수다. upstream이 `BS_SetPoltergeistMessage`(`PREPARE_ITEM_BUFFER(gBattleTextBuff1, 대상 도구)`)를 지우므로 그대로 두면 이전 메시지의 버퍼 내용이 도구명 자리에 나온다. 가리키는 값은 같다: 내던지기 이름은 `gEffectBattler` = 공격자(자기 대상 효과), 도구는 `SetMoveEffect`가 설정하는 `gLastUsedItem`(공격자 도구). 폴터가이스트 도구는 `gLastUsedItem`(대상 도구, 옛 `gBattleTextBuff1`과 같은 `CopyItemName` 결과), 대상은 `gEffectBattler` = `gBattlerTarget`(`TARGET_SELECTED`). 조사 토큰은 직전 두 바이트로 정해지므로 같다.
- 제외한 hunk: 없음.
- 검증:
  - `git diff --check` 통과.
  - **STRINGID→바이트 대응**(`w-111-117-119/stringid_table.sh`·`stringid_bytes.py`, 이식 전 표는 사전 분석 표와 바이트 동일): 728행, removed 0, added 0, 바이트 변경 2(아래 토큰 바이트만), 순번 이동 0, UNSET/REF 13개 같음.
    ```
    STRINGID_PKMNFLUNG             FD 0F → FD 11 (ATK_NAME_WITH_PREFIX → EFF_NAME_WITH_PREFIX), 나머지 18바이트 같음
    STRINGID_ABOUTTOUSEPOLTERGEIST FD 00 → FD 16 (BUFF1 → LAST_ITEM), FD 10 → FD 11 (DEF → EFF), 나머지 같음
    ```
    문장 길이가 같아 창 너비도 같다.
  - 빌드: 종료 코드 0, **ROM 32,716,228 B(+112 B) / EWRAM 248,924 B(0) / IWRAM 25,516 B(0)**, 새 경고 0.
  - 한글이 든 소스 줄 변경: `src/battle_message.c` 2쌍(위 표)뿐. docs 밖 비ASCII 변경 줄도 이 4줄뿐.
- 테스트(이식 후 파일별 실행 25개, 이식 전은 seq 107 전체 실행의 확장 목록과 이름 대조): 이식 후 427건 중 419건이 이식 전 목록과 이름이 같고 **상태 변화 0**, 새 이름 8건, PASS 손실 0.
  - 파일: `move_effect/fling.c`(PASS 8 / FAIL 16), `poltergeist.c`(PASS 3 / FAIL 2, 옛 TO_DO 1 삭제), `move_effect_secondary/break_screens.c`·`steal_stats.c`, `move_effect/beat_up.c`·`raging_bull.c`·`embargo.c`, `status1/paralysis.c`, `hold_effect/blunder_policy.c`·`restore_pp.c`, `ability/cheek_pouch.c`·`corrosion.c`·`harvest.c`·`magician.c`·`pickup.c`·`symbiosis.c`·`unburden.c`·`keen_eye.c`·`clear_body.c`·`big_pecks.c`·`hyper_cutter.c`·`sheer_force.c`, `sleep_clause.c`, `gimmick/dynamax.c`, `test/text.c`.
  - 새 테스트: PASS 5(`Fling doesn't reveal … failed to use / missed`, `Poltergeist doesn't reveal … missed / immune / fails to use`), FAIL 3(`Fling reveals the user's item before dealing damage`, `Poltergeist reveals the target's item before dealing damage`, `Poltergeist fails if the target isn't holding an item 1/2`). FAIL 3건의 사유는 모두 `Unmatched MESSAGE`(영문 기대값, 알려진 한계)다. 사전 분석 예측과 같다.
- **출력 변화(upstream 동작 그대로):**
  1. 폴터가이스트가 대타출동 상태의 대상에게 쓰일 때(공격자 특성이 틈새포착이 아닐 때) 도구 공개 문장이 나오지 않는다. 새 경로 `setpreattackadditionaleffect` → `SetMoveEffect()`에서 `DoesSubstituteBlockMoveEffectOnTarget()`이 효과를 막는다. upstream 1.17.0도 예외가 없다. → `BATTLE_MESSAGE_OUTPUT_CHANGES.md` "기술·필드 상태 효과" 표에 1행 추가(이 커밋).
  2. 내던지기 대기 프레임(결과 문서에만 기록, 문자열 선택 변화가 아니라 출력 변화 문서 범위 밖): 이전 `pause B_WAIT_TIME_SHORT`(32프레임) → 문장 → `waitmessage B_WAIT_TIME_SHORT`(32). 이후 문장 → `waitmessage B_WAIT_TIME_LONG`(64). 총 대기는 같고 문장 앞 멈춤이 문장 뒤로 옮겨졌다.
  3. 공격 전 효과 전체에 `!IsAnyTargetAffected()` 조건이 붙는다. HnS의 기존 공격 전 효과 기술(깨뜨리다·사이코팽·레이징불, 섀도스틸, 집단구타)은 단일 대상이고 면역·방어·빗나감이 먼저 처리되므로 출력 변화는 예상되지 않는다(관련 테스트 전후 같음).
  4. 행동 불가·빗나감·면역일 때 문장이 나오지 않는 것, 부자유친 2회 공격 폴터가이스트에서 두 번 출력되는 것은 이전과 같다.
- 남은 위험: 낮음. #9657·#10220·#9939가 이 경로(공격 전 효과·명중 판정)를 캔슬러로 옮길 때 `MOVE_EFFECT_ITEM_MESSAGE`와 두 한글 문장의 토큰은 이번 결과를 기준으로 유지한다. `AccuracyCheck`의 #9929 분기를 upstream 원문으로 덮지 않는다.
- 실기 확인: 필요(아래 "실기 확인 항목" 2).

## 동기화 단위: seq 112 #9587 `U-9587` Use precalculated speedStats for AI speed comparison

- 현재 판정: 적용
- 커밋: `2d86edca81`
- upstream 근거: `5774efaea1`(1파일 +2/−2). HnS `AI_WhoStrikesFirst`는 upstream 부모와 함수 전체가 같았고, 1.17.0 `battle_ai_util.c`도 같은 형태다.
- 수정 파일(1): `src/battle_ai_util.c`(`speedBattlerAI/speedBattler = GetBattlerTotalSpeedStat(...)` → `gAiLogicData->speedStats[...]`)
- HnS 적응: 없음(오프셋 +24줄). HnS 고유 AI 코드(챌린지 구 시트러스, HP 0 클램프, Supreme Overlord, #9568 Beat Up, 아군 KO `.maximum`)와 겹치는 줄이 없다. `GetBattlerTotalSpeedStat`에 HnS 챌린지 참조는 없다.
- 동작: `speedStats[b]`는 `SetBattlerAiData`에서 `abilities[b]`·`holdEffects[b]`를 채우는 같은 순간에 같은 인자로 계산된다. 턴 시작 판단, 플레이어 기술 예측, 교체 예측 점수는 같은 값이다. **값이 달라지는 경계 사례(upstream 1.17.0과 같은 동작):**
  1. 교체 후보 시뮬레이션(`InitializeSwitchinCandidate`): 캐시한 뒤 적용되는 끈적끈적네트 −1(더블은 상대 수만큼), 심술꾸러기+네트 +1, 스피드업 열매, 룸서비스, 부스트에너지·날씨/필드 고대활성·쿼크차지, 가상 독(속보), 치유소원·초승달춤 상태 해제가 속도 비교에 반영되지 않는다. 영향은 `GetBestMonIntegrated`의 선공 판정(1:1 승리 판정·배턴터치 후보)과 후보 대미지 계산 안의 보복·전격부리·애널라이즈 턴 순서다.
  2. 턴 시작 시 이미 쓰러진 배틀러의 `speedStats`는 0이다(이전에는 남은 `gBattleMons` 수치). `AI_SetBattlerTurnOrder` 위치만 바뀐다(트릭룸이 아니면 맨 뒤).
  3. 턴 도중 재판단(기절 후 교체, 탈출버튼·탈출팩, 유턴)에서 AI 파트너 속도는 턴 시작 값이다.
  - 난수: `AI_WhoStrikesFirst`는 난수를 쓰지 않는다. 위 경계 사례에서 AI 결정이 달라질 때만 이후 난수 소비가 달라진다. 이식 전에 저장한 녹화 배틀을 재생하면 이 경계 사례에서 대미지 난수가 어긋날 수 있다(아주 드묾, 녹화는 #8943 A안으로 어차피 무효화 예정).
- 검증:
  - `git diff --check` 통과.
  - 빌드: 종료 코드 0, **ROM 32,716,244 B(+16 B) / EWRAM 248,924 B(0) / IWRAM 25,516 B(0)**, 새 경고 0.
  - 한글 줄 변경 0.
- 테스트: AI 테스트 파일 26개 + `ability/analytic.c` + `move_effect/focus_punch.c`를 `ai-harness/run_ai_tests.sh`로 이식 직전(`pre112`, seq 111 커밋 상태)과 직후(`post112`)에 돌렸다. `pre112`는 seq 107 전체 목록과 이름·상태가 모두 같다(457건 대조, 변화 0). **`post112` 목록이 `pre112`와 바이트 동일**(485줄, PASS 354). 속도 프로브(`probe-9587-after-seq112.patch`)는 결과가 같아 돌리지 않았다.
- 남은 위험: 낮음(위 경계 사례의 AI 판단이 upstream과 같아지고 이식 전 HnS와는 달라질 수 있음).
- 실기 확인: 선택(아래 "실기 확인 항목" 3).

## 동기화 단위: seq 113 #9596 `U-aicalc-9548` Fix frame counter in multi battles

- 현재 판정: 부분 적용(HnS 적응)
- 커밋: `f6ef307f76`
- upstream 근거: `b9a1dcfbde`(5파일 +47/−54). upstream 원본은 `src/battle_util.c`에서 `git apply`가 실패한다(HnS는 #9548 원형이 아니라 1.17.0 최종형).
- 수정 파일(3): `include/battle.h`(`AiLogicData.holdEffectParams[]` 삭제), `src/battle_ai_main.c`(`SetBattlerAiData`의 대입 삭제), `src/battle_ai_util.c`(`ShouldTryOHKO`: `gAiLogicData->holdEffectParams[battlerDef]` → `GetBattlerHoldEffectParam(battlerDef)`). 사전 분석 최소 패치(`seq113-9596.patch`) 그대로. 세 줄은 1.17.0과 같다(1.17.0 `battle.h`에 `holdEffectParams`·`turnOrder` 없음, `ShouldTryOHKO` 같은 줄).
- 제외한 hunk와 이유:
  - `AiLogicData.turnOrder[]` 추가, `SetBattlerTurnOrder` 신설과 `SetAiLogicDataForTurn` 안의 초기화·정렬: 1.17.0에 없다(#10453이 되돌림). HnS는 seq 100(#9548)에서 1.17.0 최종형(`Ai_AttackerMoves*`가 필요할 때 `AI_SetBattlerTurnOrder`로 계산)을 넣었다. 현재 `GetAiTurnOrder`·`Ai_AttackerMovesAfterTarget`·`Ai_AttackerMovesLast`가 1.17.0과 글자까지 같다(`diff` 0).
  - `BattleContext.aiTurnOrder` 삭제, `AI_CalcDamage` 안의 `AI_SetBattlerTurnOrder` 호출 삭제, `CalcMoveBasePower` 등 호출 3곳 시그니처: HnS에 원래 없거나 이미 새 형태(이미 동등).
  - `SetAiLogicDataForTurn` 선언 인라인화·변수명·공백, `AI_CheckBadMove` 등 줄끝 공백: 동작·코드가 같은 정리(선택 패치와 오브젝트 바이트 동일). 계획 TSV대로 넣지 않았다.
- 동작: 기합의띠 확률 값이 `SetBattlerAiData` 시점 캐시에서 호출 시점 `GetBattlerHoldEffectParam`로 바뀐다. AI 판단 중 대상 도구가 바뀌는 경로는 모두 `SetBattlerAiData`를 다시 부르므로 값이 같다. `Random()` 호출 횟수·순서도 같다(사전 분석 역어셈블 확인). "프레임 카운터" 회귀(#9548 원형의 `AI_CalcDamage`마다 턴 순서 계산)는 HnS에 들어온 적이 없다.
- 검증:
  - `git diff --check` 통과.
  - 빌드: 종료 코드 0, **ROM 32,716,132 B(−112 B) / EWRAM 248,924 B(0) / IWRAM 25,516 B(0)**, 새 경고 0. `gAiLogicData`는 힙이라 `sizeof(struct AiLogicData)`만 4 B 줄었다(정적 배치 무관).
  - 한글 줄 변경 0.
- 테스트: `run_ai_tests.sh post113` 목록이 `post112`와 바이트 동일(485줄, PASS 354). 일격기 AI 테스트(`ai.c`), `ai_multi.c`, `focus_punch.c` 포함.
- 남은 위험: 매우 낮음. 세이브 무관.
- 실기 확인: 필요 없음.

## 동기화 단위: seq 114 #9532 `U-bide-9532` Bide Refactor

- 현재 판정: 적용(HnS 적응)
- 커밋: `7e4f61c927`
- upstream 근거: `124009500d`(부모 `b9a1dcfbde` = #9596). 1.17.0의 `CancelerBide`·스크립트와 구조가 같다(이후 이름만 바뀜). 선행 #9858(seq 3, `5470147fab`) 적용됨.
- 수정 파일(10): `asm/macros/battle_script.inc`, `data/battle_scripts_1.s`, `include/battle_scripts.h`, `include/constants/battle_script_commands.h`, `src/battle_move_resolution.c`, `src/battle_script_commands.c`, `src/battle_util.c`, `src/data/battle_move_effects.h`, `test/battle/move_effect/bide.c`, `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`
- 적용 방법: 사전 분석 패치(`seq114-9532.patch`)와 선택 패치(`seq114-9532-animturn-optional.patch`, 1줄)를 차례로 `git apply`했다. 패치와 upstream의 `+`/`-` 줄 집합 차이는 두 줄뿐이다: (1) 제외한 매크로 공백 줄 hunk, (2) 옛 `BattleScript_BideAttack`에서 지우는 `clearmoveresultflags` 줄이 HnS판(EXTREMELY/MOSTLY 포함)인 것.
- HnS 적응:
  - `BattleScript_BideAttack`: HnS 줄 `clearmoveresultflags … | MOVE_RESULT_EXTREMELY_EFFECTIVE | MOVE_RESULT_MOSTLY_INEFFECTIVE`(Champions) 문맥 때문에 수동 적용. 스크립트 본문과 함께 이 줄도 사라지며, 새 경로에서는 `DoFixedDamageMoveCalc`가 `EFFECT_BIDE`에 고정 피해를 돌려주고 `moveResultFlags &= ~(MOVE_RESULT_LOW_EFFECTIVENESS | MOVE_RESULT_HIGH_EFFECTIVENESS)`를 한다. HnS의 두 매크로(`include/constants/battle.h`)는 EXTREMELY/MOSTLY를 포함하므로 상성 문구가 나오지 않던 동작이 추가 코드 없이 유지된다. 급소도 `criticalHit = FALSE`.
  - **방출 턴 연출(upstream과 다름):** `CancelerBide` 방출 분기에 `gBattleScripting.animTurn = 1; // HnS: keep Bide's unleash animation (old setbyte sB_ANIM_TURN, 1)`을 넣었다. 옛 스크립트는 `setbyte sB_ANIM_TURN, 1`로 `gBattleAnimMove_Bide`의 `choosetwoturnanim BideSetUp, BideUnleash`에서 `BideUnleash`(방출 연출)를 골랐다. upstream 새 경로는 `animTurn`이 0이라 방출 턴에 준비 연출(`BideSetUp`)이 다시 나온다(upstream 1.17.0에도 남은 회귀). `CANCELER_BIDE` 뒤 캔슬러와 `BattleScript_EffectHit`의 `attackanimation` 사이에서 `animTurn`을 0으로 되돌리는 곳은 없다(초기화는 턴 행동 전환·호출 기술·매직미러 재지정 경로뿐).
- 제외한 hunk: `asm/macros/battle_script.inc`의 `tryconfusionafterskydrop` 뒤 공백 줄 정리(HnS가 #9249 이식 때 이미 빈 줄로 만들어 결과 동일).
- **opcode:** `include/constants/battle_script_commands.h`가 upstream `124009500d`·`bede100c3e`(#9494)와 바이트 동일, `gBattleScriptingCommandsTable`도 `124009500d`와 동일. 컴파일러로 계산한 값: `UNUSED_31` 0xfc, **`UNUSED_32` 0xfd, `UNUSED_33` 0xfe**, `CALLNATIVE` 0xff, `TWOTURNMOVESCHARGESTRINGANDANIMATION` 0x82(130). 두 명령 삭제로 뒤 opcode가 upstream과 똑같이 1~2씩 당겨진다. 스크립트는 기호로 조립되고 세이브에 저장되지 않는다. g1 plan의 "UNUSED_31/32 추가"는 #9446·#9249 이식 전 기준이라 틀렸고, 실제 추가는 `UNUSED_32/33`이다(g6 plan과 일치).
- 검증:
  - `git diff --check` 통과, 파일 모드 변경 없음.
  - 빌드: 종료 코드 0, **ROM 32,716,164 B(+32 B) / EWRAM 248,924 B(0) / IWRAM 25,516 B(0)**, 새 경고 0.
  - 한글 줄 변경: docs 밖 0(비ASCII 변경 줄 2개는 영문 테스트 이름의 "Pokémon").
- 테스트(이식 후 12파일, 이식 전은 seq 107 확장 목록과 이름 대조): 169건 중 162건 이름 일치, 상태 변화 1(`Bide hits the last Pokémon that attacked the user, even allies`: TO_DO → **PASS**, 새 테스트로 바뀜), 새 이름 7, **PASS 손실 0**.
  - 파일: `move_effect/bide.c`(PASS 1 / FAIL 7 / TO_DO 1), `instruct.c`(15/4/TO_DO 1), `copycat.c`(1/1/TO_DO 15), `two_turns_attack.c`(8/11), `semi_invulnerable.c`(1/3), `sky_drop.c`(14/4), `solar_beam.c`(0/2), `geomancy.c`(TO_DO 3), `focus_punch.c`(3/13/TO_DO 5/ASSUMPTION_FAIL 1), `ability/dazzling.c`(7/4/INVALID 1, INVALID는 이식 전과 같음), `sheer_force.c`(30/1), `ai/can_use_all_moves.c`(8/4).
  - 새 FAIL 6건(`Bide fails if no damage…`, `…0 total damage…`, `…blocked by Dazzling…`, `…blocked by partner Dazzling`, `…Substitute`, `…through protect`)과 기존 `Bide deals twice…` FAIL의 사유는 모두 `Unmatched MESSAGE`. TO_DO 1건은 이름 변경(`Bide has +1 priority on following turns if called via a different move`).
- **출력 변화(upstream 동작):** 2턴째(축적)·3턴째(방출)에 `…은(는)\n참기를 썼다!`(`sText_AttackerUsedX`)가 더 나오지 않는다(`CancelerAttackstring`이 `bideTurns` 중 건너뜀). 방출은 `…의\n참기가 풀렸다!` → 일반 공격 경로. 빗나감 경로가 `BattleScript_MoveMissed`에서 `MoveMissedPause`로 바뀌어 짧은 멈춤이 한 번 더 있다. 받은 피해 0이면 `…참기가 풀렸다!` → `그러나 실패하고 말았다!`. 설정 턴 이후 캔슬러(변환자재·리베로 등)는 설정 턴에 돌지 않는다. → `BATTLE_MESSAGE_OUTPUT_CHANGES.md` "기술·필드 상태 효과" 표에 1행 추가(이 커밋).
- 남은 위험: 낮음~중간. 참기 축적 중인 포켓몬이 춤추기로 다른 기술을 따라 쓰면 그 기술의 공격 문구도 생략된다(upstream과 같음).
- 실기 확인: 필요(아래 "실기 확인 항목" 4).
