# full-sync 실제 port 결과 — seq 108~119

진행 중: 마지막 완료 seq 110, 다음 seq 111.

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), [`CLAUDE_FULL_SYNC_PORT_PROMPT.md`](../../CLAUDE_FULL_SYNC_PORT_PROMPT.md), 메시지 출력 기록: [`BATTLE_MESSAGE_OUTPUT_CHANGES.md`](../../../localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md)
시작 HEAD: `10077a5d70` (작업 트리 clean)

## seq 108~119 요약

| seq | PR | 판정 | 커밋 | ROM 변화 | 비고 |
|---|---|---|---|---:|---|
| 108 | #9537 (+#9668) | 적용(HnS 적응) | `70eb6a4271` | 0 B (SHA1 동일) | `OBJ_EVENT_GFX_*` 명시 값 유지·이름만 변경, FRLG 맵 2개 수동 hunk, `spritesheet_rules.mk` 규칙 3개 이름(#9668 동등, 오타 제외) |
| 109 | #9241 | 적용(HnS 적응) | `ec9dca2712` | +32 B | `#if IS_HNS` 그래픽을 새 구조체 표 HnS판으로 옮김, (class, 그래픽) 쌍·세이브 값 불변 |
| 110 | #9595 | 이미 적용 | 없음(`5121b83c94`) | 0 | seq 83 unit에서 적용 |

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
