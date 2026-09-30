# full-sync 실제 port 결과 — seq 108~119

완료: seq 108~119 이식·전체 테스트·기록 완료(다음 구간은 seq 120 #9657부터). 결과 커밋에 기준 목록 [`test-baseline-seq119.txt`](test-baseline-seq119.txt)를 넣었다. 메인의 커밋 리뷰는 이 문서 밖에서 진행한다.

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
| 113 | #9596 | 부분 적용(HnS 적응) | `f6ef307f76` | −112 B | `holdEffectParams` 캐시 제거·`ShouldTryOHKO` 기합의머리띠 판정만. 턴 순서 hunk는 seq 100에서 1.17.0 최종형으로 이미 대체 |
| 114 | #9532 | 적용(HnS 적응) | `7e4f61c927` | +32 B | opcode `UNUSED_32/33`(0xfd/0xfe, upstream과 헤더·명령 표 바이트 동일), 방출 턴 연출 `animTurn = 1`(`// HnS:`), 2·3턴째 공격 문구 생략(출력 변화 문서 1행) |
| 115 | #9494 | 적용(HnS 적응) | `3f7f0ddabf` | +352 B (EWRAM +16 B) | 교체 대기열. HnS 아이템 팝업·Champions 상성·#9790/#9946/#9818 형태 보존, `NeutralizingGasExits` sBATTLER 저장·복원, **원시 날씨 해제 2줄 유지(`@ HnS:`)**, 드래곤애로 허탕보험 테스트 `KNOWN_FAILING`(upstream 병합과 같음), 출력 변화 문서 3행 |
| (176) | #9864 | 적용(같은 unit, 선반영) | `489c58259c` | +32 B | upstream 그대로(`reshow_battle_screen.c` 2줄). seq 176 도달 시 "이미 적용" |
| 116 | #9557 | 적용(HnS 적응, 변형 B) | `4440c18163` | +2,416 B | 미리보기 그림·표를 `#if MPS_ENABLE_MAP_PREVIEWS`로 감쌈(`// HnS:`), `MAPSEC_ROCKET_HIDEOUT_HNS` 보존, BG 팔레트 13 날씨 색 변환 해제는 upstream대로, docs 제외 |
| 117 | #9578 | 부분 적용(HnS 적응) | `3960fc0c8e` | −64 B | `BattleScript_ShedSkinActivates` hunk 제외, 미사용 스크립트·`STRINGID_PKMNSXCUREDYPROBLEM`(한글 1줄) 삭제, `STRINGID_PKMNPREVENTSROMANCEWITH` 유지 |
| 118 | #9630 | 적용 | `4e3c6bd5e7` | +96 B | upstream 그대로. 롤 값 불변(나눗셈 전수 증명, 실제 커밋으로 mGBA 롤 하네스 재실행 exit 0) |
| 119 | #9634 | 적용(HnS 적응) | `de9b581a28` | −32 B | 한글 `STRINGID_PKMNISGLOWING` 한 줄 삭제, 불새는 이미 출력 중인 `STRINGID_CLOAKEDINAHARSHLIGHT` 고정 |

- 마지막 빌드(`de9b581a28`): 종료 코드 0, **ROM 32,718,964 B(97.51%) / EWRAM 248,940 B(94.96%) / IWRAM 25,516 B(77.87%)**. 구간 전체 ROM +2,880 B(대부분 #9557 변형 B +2,416 B), EWRAM +16 B(#9494 `gSpecialStatuses`), IWRAM 0. 매 빌드 새 경고 0. `pokehns.gba` SHA1 `d439ac3b54a464827093a1a0d80d33e74c83ea5e`.
- 값 표: #9537 오브젝트 그래픽 값 표 값 변화 0(이름만 3개, ROM SHA1 이식 직전과 동일), #9241 (class, 그래픽) 쌍 남 30·여 20 전후 동일(HnS·Emerald·FRLG), 세이브 `facilityClass` 불변. STRINGID→바이트 대응: 구간 누적 removed 2(`PKMNISGLOWING`, `PKMNSXCUREDYPROBLEM`), 바이트 변경 2(#9610 토큰만), 나머지 722개 바이트 동일.
- 한글이 든 소스 줄 변경(docs 밖): `src/battle_message.c`의 #9610 토큰 교체 2쌍(`STRINGID_PKMNFLUNG`, `STRINGID_ABOUTTOUSEPOLTERGEIST`, 본문 바이트 동일), #9578 `STRINGID_PKMNSXCUREDYPROBLEM` 삭제 1줄, #9634 `STRINGID_PKMNISGLOWING` 삭제 1줄. 그 밖 0.
- upstream과 다르게 둔 곳(`HnS:` 표시): #9537 `event_objects.h` 명시 값 유지·`spritesheet_rules.mk` 규칙 이름(#9668 오타 제외), #9241 `#if IS_HNS` 그래픽 표, #9610 `AccuracyCheck` #9929 분기 유지, #9532 방출 턴 `animTurn = 1`, #9494 `BattleScript_QueuedSwitch` 원시 날씨 해제 2줄(`@ HnS:`)·드래곤애로 허탕보험 테스트 `KNOWN_FAILING`(upstream 병합 상태와 동일), #9557 미리보기 그림·표 `#if MPS_ENABLE_MAP_PREVIEWS` 가드, #9578 `ShedSkinActivates` hunk 제외, #9596 턴 순서 hunk 제외(이미 1.17.0형).
- `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 추가 행: 5행 — "기술·필드 상태 효과"에 #9610 폴터가이스트+대타출동 1행, #9532 참기 1행, "특성·도구·도주"에 #9494 3행(대기열 교체 순서, 레드카드 뒤 위기회피, 목스프레이·과사열매·허탕보험·옛노래 시점). 지시의 "3행(#9532 1행 포함)"은 사전 분석의 #9494 3행 초안과 #9532 1행을 모두 넣는 것으로 해석했다(내용이 서로 겹치지 않아 합치지 않음).
- 구간 밖 행: #9668(seq 126, seq 108 커밋에 포함), #9864(seq 176, 별도 커밋 `489c58259c`)만 넣었다. #9784(seq 166)는 넣지 않았다.
- 전체 테스트: 아래 "구간 끝 전체 테스트".

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
| `seq113-9596.md` (+`.patch`) | 최소 패치: `holdEffectParams` 캐시 제거와 `ShouldTryOHKO` 기합의머리띠 판정만. 턴 순서 hunk는 seq 100에서 1.17.0 최종형으로 이미 대체 |
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
  1. 교체 후보 시뮬레이션(`InitializeSwitchinCandidate`): 캐시한 뒤 적용되는 끈적끈적네트 −1(더블은 상대 수만큼), 심술꾸러기+네트 +1, 캄라열매 등 스피드 상승 열매, 룸서비스, 부스트에너지·날씨/필드 고대활성·쿼크차지, 가상 독(속보), 치유소원·초승달춤 상태 해제가 속도 비교에 반영되지 않는다. 영향은 `GetBestMonIntegrated`의 선공 판정(1:1 승리 판정·배턴터치 후보)과 후보 대미지 계산 안의 보복·전격부리·애널라이즈 턴 순서다.
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
- 동작: 기합의머리띠 발동 확률 값이 `SetBattlerAiData` 시점 캐시에서 호출 시점 `GetBattlerHoldEffectParam`로 바뀐다. AI 판단 중 대상 도구가 바뀌는 경로는 모두 `SetBattlerAiData`를 다시 부르므로 값이 같다. `Random()` 호출 횟수·순서도 같다(사전 분석 역어셈블 확인). "프레임 카운터" 회귀(#9548 원형의 `AI_CalcDamage`마다 턴 순서 계산)는 HnS에 들어온 적이 없다.
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
- 남은 위험: 낮음~중간. 참기 축적 중인 포켓몬이 특성 무희로 다른 기술을 따라 쓰면 그 기술의 공격 문구도 생략된다(upstream과 같음).
- 실기 확인: 필요(아래 "실기 확인 항목" 4).

## 동기화 단위: seq 115 #9494 `U-queuedswitch-9494` Adds queued switches for Move End switches (+ seq 176 #9864)

- 현재 판정: 적용(HnS 적응). 같은 unit의 #9864(seq 176)는 바로 뒤 별도 커밋으로 넣었다. #9784(seq 166)는 넣지 않았다(아래).
- 커밋: #9494 `3f7f0ddabf`, #9864 `489c58259c`
- upstream 근거: `bede100c3e`(23파일 +336/−145, 부모 `124009500d` = #9532), `47f01e61ba`(#9864, 1파일 +2/−2). deps #9176(seq 59)·#9417(seq 71)·#9249(seq 80) 적용됨.
- 수정 파일(#9494, 25): `data/battle_scripts_1.s`, `include/battle.h`, `include/battle_hold_effects.h`, `include/battle_scripts.h`, `include/constants/battle.h`, `include/constants/battle_move_resolution.h`, `src/battle_ai_switch.c`, `src/battle_anim_mons.c`, `src/battle_end_turn.c`, `src/battle_hold_effects.c`, `src/battle_main.c`, `src/battle_move_resolution.c`, `src/battle_script_commands.c`, `src/battle_switch_in.c`, `src/battle_util.c`, `src/data/hold_effects.h`, `src/reshow_battle_screen.c`, 테스트 7개(`ability/emergency_exit.c`, `ability/magician.c`, `form_change/battle_after_move.c`, `hold_effect/blunder_policy.c`, `hold_effect/eject_button.c`, `hold_effect/red_card.c`, `move_effect/hit_escape.c`), `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`. #9864: `src/reshow_battle_screen.c`.
- 적용 방법: 사전 분석 패치(`seq115-9494.patch`)를 `git apply`로 넣고, 선택 패치(`seq115-9494-primalweather-optional.patch`, 2줄)에 `@ HnS:` 주석 1줄을 더했다(`.s` 파일은 `@` 주석을 쓴다). 패치와 upstream의 `+`/`-` 줄 집합 차이는 세 줄뿐이다: 조개껍질방울 중복 조건 삭제 줄이 HnS의 1인자 `IsAnyTargetTurnDamaged(battlerAtk)` 형태인 것, 매지션 점착 조건 hunk 2줄이 없는 것(HnS는 #9818 형태라 원래 없음). 나머지 HnS 적응은 문맥만 다르다. #9864는 upstream과 `+`/`-` 줄이 같다. 파일 모드 변경 없음.
- HnS 적응(보존):
  - `BattleScript_EjectButtonActivates`: HnS 아이템 팝업(`call BattleScript_ItemPopUp_Scripting`)과 `STRINGID_EJECTBUTTONACTIVATE`는 hunk 밖이라 그대로. 뒷부분(교체 화면·등장)만 upstream처럼 삭제.
  - `BattleScript_EjectPackActivates`: HnS #9946 `jumpifcantswitch SWITCH_IGNORE_ESCAPE_PREVENTION | BS_SCRIPTING` 유지, `goto` 대상만 변경.
  - `BattleScript_NeutralizingGasExits`: HnS는 이미 후속판(`gBattlerTarget` 루프)이라 1.17.0과 같게 시작에 `copybyte sSAVED_BATTLER, sBATTLER`, `restoretarget` 뒤 `copybyte sBATTLER, sSAVED_BATTLER`만 추가(필수: 옛 탈출버튼 스크립트의 sBATTLER 보존 줄이 사라지므로 화학변화가스 보유자가 탈출버튼·탈출팩으로 나갈 때 `BS_SCRIPTING`이 덮이지 않게 함). 결과가 1.17.0 스크립트와 같다.
  - `battle_hold_effects.c`: 약점보험·`TrySetEnigmaBerry`의 Champions `MOVE_RESULT_HIGH_EFFECTIVENESS` 유지(`IsBattlerAlive`만 제거), 허탕보험 #9790 형태에서 `IsBattlerAlive` → `!redCardSwitched`만, 목스프레이·조개껍질방울·생명의구슬 HnS 1인자 `IsAnyTargetTurnDamaged`. `ItemBattleEffects()` 첫머리의 HnS `IsBattlerAlive` 검사는 남는다.
  - `FaintClearSetData`: HnS Champions `B_RAGE_FIST` 두 줄 유지, `keepGastroAcid` 두 줄만 삭제.
  - `MoveEndHitEscape`: HnS 조건 `IsBattlerTurnDamaged(gBattlerTarget, INCLUDING_SUBSTITUTES)` 유지, `!HasAnyBattlerQueuedSwitch()`·`!redCardSwitched`·`queuedSwitch = QUEUED_SWITCH_OPEN_PARTY_SCREEN`만 추가. `FAINT_BLOCK_CHECK_TARGET_FAINTED`의 HnS #9409 조건 유지.
  - 자기과신 계열 case 목록의 HnS `ABILITY_EELEVATE` 유지.
  - `test/battle/ability/magician.c`: HnS 파일이 1.17.0 순서(#9818)라 새 테스트를 1.17.0처럼 파일 끝에 넣었다.
- **upstream과 다르게 둔 곳 1 — 원시 날씨 해제(`data/battle_scripts_1.s` `BattleScript_QueuedSwitch`):** upstream은 옛 탈출버튼 스크립트와 유턴 경로(`BattleScript_MoveSwitchOpenPartyScreenReturnWithNoAnim`)에 있던 `trytoclearprimalweather`를 대기열 교체 경로에 옮기지 않아, 끝의대지·시작의바다·델타스트림 보유자가 탈출버튼·탈출팩·위기회피·유턴·볼트체인지·퀵턴으로 나가도 날씨가 남는다(1.17.0에도 남음). HnS는 `hpthresholds` 뒤에 `@ HnS:` 주석과 `trytoclearprimalweather`·`flushtextbox`를 두어 이식 전 동작(날씨 해제와 기존 한글 해제 문구)을 유지했다.
  - 확인(임시 테스트, 저장소에 남기지 않음): `desolate_land.c` 끝에 "그란돈(주홍구슬)이 유턴으로 나간 뒤 `gBattleWeather & B_WEATHER_SUN_PRIMAL`이 0" 테스트와 대조 테스트를 붙여 돌렸다. HnS 2줄이 있으면 PASS, 2줄을 지우면(upstream 상태) FAIL, 대조 테스트는 두 경우 모두 PASS. 확인 뒤 두 파일을 원래대로 되돌렸다(`git diff` 없음 확인).
- **upstream과 다르게 둔 곳 2 — `test/battle/hold_effect/blunder_policy.c`:** `Blunder Policy activates for Dragon Darts if one target misses for accuracy but the other target is hit twice`에 `KNOWN_FAILING;` 한 줄을 넣었다. 이 테스트는 HnS가 upstream master에서 받은 #9790(`69b1891140`)의 것으로, 허탕보험이 드래곤애로 첫 타격 뒤에 발동한다고 기대한다. #9494로 허탕보험이 모든 타격 뒤(`MOVEEND_SPRAY_LEPPA_BLUNDER`)로 옮겨져 `Unmatched ANIMATION`으로 PASS→FAIL이 됐다. upstream도 #9790을 upcoming(#9494 포함)에 병합할 때(`7ff83c6542`) 같은 줄을 넣었고, **seq 467 #9841**이 `KNOWN_FAILING`을 지우고 기대 순서를 "두 타격 뒤"로 바꾼다. 결과 파일이 upstream `7ff83c6542`~#9841 직전 blob(`fbba72441b`)과 바이트 동일하므로 #9841이 그대로 적용된다.
  - 동작 확인: 기대 순서만 #9841처럼 바꾼 임시 사본으로 돌리면 이 테스트가 PASS(파일 11건 모두 PASS)였다. 즉 허탕보험은 드래곤애로 두 타격 뒤 한 번 발동하며 1.17.0과 같다. 확인 뒤 원래대로 되돌렸다.
- 제외한 hunk: `src/battle_util.c` 매지션 점착 조건(`ABILITY_STICKY_HOLD || !IsBattlerAlive`) 2줄 — HnS #9818 형태에는 이 절이 없다(1.17.0도 없음).
- **#9864(seq 176):** 의존은 #9494 하나. `notOnField`가 만든 UI 회귀(교체 대기 중 파티 화면을 열었다 닫으면 이전 포켓몬 체력 상자가 다시 보임)를 고친다. seq 176 도달 시 "이미 적용(`489c58259c`)"으로 처리한다.
- **#9784(seq 166)는 넣지 않았다:** plan deps가 #9494·#9717(seq 150)·#8943(seq 138.5)인데 HnS에 `BattleScript_EjectItemActivates`·`ENDTURN_SEND_OUT_REPLACEMENTS`·`gBattlersByRawSpeed`·`gParties`가 아직 없다. 그때까지 upstream과 같이 `Eject Button will activate before Red Card if holder is faster`가 `KNOWN_FAILING; // #9499`다(아래 테스트).
- 검증:
  - `git diff --check` 통과(두 커밋).
  - opcode 헤더·명령 표 변경 없음(#9532 뒤 upstream `bede100c3e`와 바이트 동일).
  - 빌드(#9494): 종료 코드 0, **ROM 32,716,516 B(+352 B) / EWRAM 248,940 B(+16 B) / IWRAM 25,516 B(0)**, 새 경고 0. EWRAM +16 B는 `struct SpecialStatus` 4→8 B(`enum QueuedSwitch`)라 `gSpecialStatuses[4]`가 커진 것(사전 분석 예측과 같음). `BattlerState`·`BattleStruct`·`HoldEffectInfo` 크기는 그대로. 세이브 무관(배틀 중 메모리).
  - 빌드(#9864): 종료 코드 0, **ROM 32,716,548 B(+32 B) / EWRAM 248,940 B / IWRAM 25,516 B**, 새 경고 0.
  - 한글 줄 변경: docs 밖 0. `STRINGID` 추가·삭제·본문 변경 없음(`printstring 0x3` → `printstring STRINGID_SWITCHINMON`은 같은 ID).
- 테스트(이식 후 57파일 + 원시 날씨 3파일, 이식 전은 seq 107 확장 목록과 이름 대조, ` i/n` 접미사 정규화): 797건 중 788건 이름 일치.
  - **PASS 손실 2건(모두 upstream과 같은 예상 변화):**
    1. `Eject Button will activate before Red Card if holder is faster`: PASS → **KNOWN_FAILING**. upstream #9494가 `KNOWN_FAILING; // #9499`로 표시(#9784가 해소). 사전 분석 예상.
    2. `Blunder Policy activates for Dragon Darts…`: PASS → FAIL(`Unmatched ANIMATION`) → 위 `KNOWN_FAILING;` 추가로 **KNOWN_FAILING**. 사전 분석에 없던 항목(HnS가 #9790을 master에서 먼저 받았기 때문).
  - 이름 변경(이전 이름은 전체 목록에서 사라짐): `Eject Button activates after Wandring Spirit`(PASS) → `…Wandering Spirit`(PASS), `Red Card prevents Emergency Exit activation when triggered`(PASS) → `Red Card doesn't prevent Emergency Exit activation when triggered`(PASS, 기대 동작도 바뀜), `Relic Song transformation is the last thing that happens after it hits`(FAIL) → `Relic Song transformation activates after target faints`(FAIL, `Unmatched MESSAGE`).
  - 새 테스트: PASS 3(`Eject Button activates and the attacker takes Life Orb recoil before replacement comes out`, `Emergency Exit activates and attacker's Throat Spray activates before replacement enters`, `Hit Escape: U-Turn switches user out and target activates Pickpocket before replacement enters`), FAIL 2(`Magician allows activation of stolen Throat Spray`, `Relic Song transforms Meloetta before taking Life Orb damage`, 모두 `Unmatched MESSAGE`).
  - 나머지 상태 변화 0. 이식 전부터 있던 비 MESSAGE 실패(버서크·위기회피 팝업 태스크 미해제, AI 교체 확률 테스트 등)는 전후 같다.
  - 대상 파일: 탈출버튼·레드카드·탈출팩·위기회피·도망태세·유턴 계열·매지션·폼체인지, 도구(목스프레이·허탕보험·과사열매·생명의구슬·조개껍질방울·약점보험·의문열매·하양허브·흉내허브·룸서비스), 특성(나쁜손버릇·화학변화가스·변색·발끈·분노의껍질·비스트부스트·자기과신·유대변화·사령탑·편승·긴장감·개미지옥·프레셔·무희·기분파·고대활성·재생력·마이티체인지·내용물분출·위협·점착·자연회복·달마모드), 교체 기술(배턴터치·막말내뱉기·순간이동·꼬리자르기·썰렁개그·따라가때리기·드래곤테일 계열·울부짖기·분함의발구르기·회생의기도·프리폴), AI 교체 3파일, 다이맥스, 원시 날씨 3파일.
- **출력 변화(upstream 동작, 원시 날씨 제외):** `BATTLE_MESSAGE_OUTPUT_CHANGES.md` "특성·도구·도주" 표에 3행 추가(이 커밋): (1) 탈출버튼·탈출팩·유턴 계열·위기회피·도망태세 교체가 move end 끝으로 밀려 발동 문구 → 볼 회수 → 남은 move end 출력(생명의구슬 반동 등) → 교체 화면 → `가랏! …!` 순서가 됨(탈출버튼이 발동해도 공격자의 생명의구슬·조개껍질방울·옛노래 폼체인지가 이제 적용됨), (2) 레드카드 뒤 보유자의 위기회피·도망태세가 발동, (3) 목스프레이·과사열매·허탕보험이 move end 후반으로 늦춰지고(드래곤애로 허탕보험은 두 타격 뒤), 매지션으로 빼앗은 목스프레이가 발동, 옛노래 폼체인지 문구가 생명의구슬 반동보다 먼저. 즉시 교체 경로(배턴터치·막말내뱉기 등)는 문구가 같고 볼 회수가 교체 화면보다 먼저 나온다.
- 남은 위험: 중간. `GetBattlerAbility`·`GetBattlerHoldEffect`가 `notOnField` 배틀러에 NONE을 돌려주는 것에 기대어 여러 `IsBattlerAlive` 검사가 지워졌다. HnS 고유 코드 중 이 함수를 거치지 않고 특성·도구를 읽는 경로가 교체 대기 중인 배틀러에 반응할 수 있다. 영문 `MESSAGE` 실패가 많은 파일(따라가때리기·울부짖기·레드카드 등)은 로직 회귀가 가려질 수 있다. 이후 #9717·#9784가 `BattleScript_QueuedSwitch` 주변을 고칠 때 HnS 원시 날씨 2줄의 문맥 적응이 필요하다.
- 실기 확인: 필요(아래 "실기 확인 항목" 5).

## 동기화 단위: seq 116 #9557 `U-9557` Improved FRLG Map Previews

- 현재 판정: 적용(HnS 적응, 사전 분석 변형 B)
- 커밋: `4440c18163`
- upstream 근거: `74e4e2efe2`(47파일 +813/−146: 코드 12파일, docs md 1개 + 이미지 34개 9.2 MB + `SUMMARY.md`). 같은 unit 뒤 행 seq 366 #10080은 그 순서에 넣는다(이번에 넣지 않음).
- 수정 파일(11, 신규 1): `asm/macros/event.inc`(`mappreview` 매크로), `include/config/map_preview_screen.h`(신규, `MPS_ENABLE_MAP_PREVIEWS IS_FRLG` 등), `include/constants/global.h`, `include/fldeff.h`, `include/map_preview_screen.h`, `include/overworld.h`, `src/field_screen_effect.c`, `src/field_weather.c`, `src/fldeff_flash.c`, `src/map_preview_screen.c`, `src/overworld.c`
- 적용 방법: 사전 분석 변형 B 패치(`seq116-9557.patch`)를 `git apply`로 넣었다. 패치와 upstream 코드 부분의 `+`/`-` 줄 집합 차이는 HnS 가드 줄(주석 3줄, `#if MPS_ENABLE_MAP_PREVIEWS`/`#else`/`#endif`, `#else` 쪽 표 선언·`[0 ... MPS_COUNT - 1] = { .mapsec = MAPSEC_NONE }`·`};`)뿐이다. upstream 줄은 모두 들어갔다.
- **HnS 적응(upstream과 다름):**
  - plan(g6)은 "`map_preview_screen.c`가 `#if IS_FRLG` 안이라 HnS ROM 영향 없음"이라고 했지만 upstream #9557은 그 가드를 지우고 모든 빌드의 `overworld.c`·`fldeff_flash.c`·`field_screen_effect.c`에서 미리보기 함수를 부른다(끄고 켜기는 `ShouldRunMapPreview()` 안의 `MPS_ENABLE_MAP_PREVIEWS`). HnS는 LTO 없이 `--gc-sections`만 쓰므로 그대로 넣으면 호출부에서 `sMapPreviewScreenData`까지 참조가 이어져 FRLG 미리보기 그림 21종(약 108 KB)이 ROM에 들어간다(변형 A, ROM 약 +110 KB).
  - 변형 B: 첫 `INCBIN_U8`부터 `sMapPreviewScreenData` 끝까지 `#if MPS_ENABLE_MAP_PREVIEWS … #else`(모든 칸 `MAPSEC_NONE`인 표) `#endif`로 감쌌다(주석 `// HnS:` 3줄). 가드 조건이 config 자체라 HnS에서 미리보기를 켜면 upstream 표·그림이 그대로 돌아온다. 꺼져 있을 때는 `ShouldRunMapPreview()`가 FALSE라 표를 읽는 경로가 불리지 않는다.
  - 표의 HnS 줄 `.mapsec = MAPSEC_ROCKET_HIDEOUT_HNS`(`f1f5cd0ec2`)를 보존했다(upstream에서는 문맥 줄 `MAPSEC_ROCKET_HIDEOUT`). 같은 칸의 `.type`은 upstream대로 `MPS_TYPE_FADE_IN`.
- 제외: `docs/**`(튜토리얼 md·이미지 34개 9.2 MB·`SUMMARY.md` 1줄). 코드 hunk는 모두 넣었다.
- HnS 동작:
  - 미리보기 화면은 HnS에서 뜨지 않는다(컴파일 상수 FALSE). 맵 이름 팝업·문 출입(`Task_ExitDoor`/`Task_ExitNonAnimDoor`/`Task_ExitNonDoor`)·동굴 진입 전환은 이식 전과 같은 분기를 탄다(`FadeInMapPreviewScreenIsRunning()`이 항상 FALSE라 기존처럼 `UnlockPlayerFieldControls()`).
  - **BG 팔레트 13의 날씨 색 변환 해제(upstream대로 둠):** `field_weather.c` `sBasePaletteColorMapTypes`의 BG 13이 `COLOR_MAP_DARK_CONTRAST` → `COLOR_MAP_NONE`. upstream 의도는 미리보기 그림을 BG 13~15에 올릴 때 날씨로 어두워지지 않게 하는 것이다. HnS 필드에서 팔레트 13을 쓰는 창은 엘리베이터 층 표시 창, 상점 금액 창, `mom_savings.c` 금액 창 정도다(필드 금액 상자·대화창은 14·15). 비·뇌우·가뭄처럼 색 변환을 쓰는 날씨에서 이 창들의 테두리가 더는 어두워지거나 밝아지지 않고 팔레트 14·15 창과 같게 보인다. 페이드와 시간대 색은 그대로 적용된다.
- 검증:
  - `git diff --check` 통과, 새 파일 `include/config/map_preview_screen.h` 모드 100644, 기존 파일 모드 변경 없음.
  - 빌드: 종료 코드 0, **ROM 32,718,964 B(+2,416 B) / EWRAM 248,940 B(0) / IWRAM 25,516 B(0)**, 새 경고 0. 사전 분석 예상(변형 B 약 +2.4~2.7 KB, EWRAM +1 B는 정렬로 0~4 B)과 같다. ROM에 들어간 것은 미리보기 코드와 `MAPSEC_NONE` 표뿐이다(`pokehns.map`에 FRLG 미리보기 그림 심볼 없음).
  - **기본 `make`(Emerald) 영향:** 바뀐 C 파일 6개(`map_preview_screen.c`, `overworld.c`, `fldeff_flash.c`, `field_screen_effect.c`, `field_weather.c`, `scrcmd.c`)를 Makefile과 같은 파이프라인(`-DEMERALD`, `-Werror`)으로 스크래치에서 컴파일했다(`make`는 공유 생성 파일을 Emerald용으로 다시 만들 수 있어 쓰지 않음). `map_preview_screen.c`·`fldeff_flash.c`·`field_screen_effect.c`·`field_weather.c`는 성공. `overworld.c`(HnS `FLAG_NIGHT_POKEMON`·`FLAG_DAY_POKEMON`)와 `scrcmd.c`(HnS `FLAG_STARTER_PREVIEW_CHECKED_*`·`FLAG_SHINY_STARTER_*`)는 실패하지만 이식 전 파일·헤더로도 같은 오류(이식 전후 오류 목록 동일)라 **이 PR로 새로 깨진 것은 없다**(Emerald 빌드는 이식 전부터 HnS 플래그 때문에 실패). 변형 A였다면 `map_preview_screen.c`가 `MAPSEC_ROCKET_HIDEOUT_HNS`로 추가 실패한다.
  - 한글 줄 변경 0.
- 테스트: 직접 관련 테스트 없음. `test/script.c`(PASS 2)·`test/event_object_movement.c`(PASS 1) 이식 전과 같음.
- 남은 위험: 낮음(동작 변화는 팔레트 13 한 줄). FRLG 빌드는 이식 전부터 `MAPSEC_ROCKET_HIDEOUT_HNS` 때문에 실패한다(이 PR 범위 밖 기존 HnS 수정, 기록만).
- 후속: seq 366 #10080(`map_preview_screen.h` 비트필드, `map_preview_screen.c` 4곳)은 변형 B 위에 적용 가능(사전 분석 확인). seq 378 #10162(`enum MapPreviewScreenType`)는 `#else` 표와 충돌 없음. seq 500 #9881(INCGFX) 때 가드 안의 `INCBIN_U8` 63줄을 `INCGFX_U8`로 옮긴다(upstream INCGFX 커밋의 hunk는 #9557 이전 문맥).
- 실기 확인: 필요(아래 "실기 확인 항목" 6).

## 동기화 단위: seq 117 #9578 `U-9578` identified deprecated values

- 현재 판정: 부분 적용(HnS 적응)
- 커밋: `3960fc0c8e`
- upstream 근거: `f13b73ee6d`(5파일 +1/−12)
- 수정 파일(5): `data/battle_scripts_1.s`(`BattleScript_ObliviousPreventsAttraction` 삭제), `include/battle_scripts.h`, `include/constants/battle_string_ids.h`, `src/battle_message.c`, `test/text.c`. 사전 분석 패치(`seq117-9578.patch`) 그대로.
- 제외한 hunk: `BattleScript_ShedSkinActivates`의 `printstring STRINGID_PKMNSXCUREDYPROBLEM` → `STRINGID_PKMNSXCUREDITSYPROBLEM`. HnS 탈피 스크립트는 이미 `printfromtable gStatusCureStringIds`로 치료한 상태별 문장을 출력한다(HnS 배틀 메시지 정책, `BATTLE_MESSAGE_OUTPUT_CHANGES.md` "…촉촉한몸·탈피" 행). upstream 문맥 줄이 HnS에 없고, 넣으면 HnS 정책을 되돌린다.
- **group plan 표기 정정:** `g5_general_cleanup_docs_plan.tsv`·`.md`의 "BattleScript_HealerActivates hunk는 제외"는 git hunk 머리글(`@@ -6134,7 +6134,7 @@ BattleScript_HealerActivates::`)의 함수 문맥 표시를 잘못 읽은 것이다. upstream #9578은 치유의마음 스크립트를 바꾸지 않는다. 실제로 제외한 hunk는 바로 아래 `BattleScript_ShedSkinActivates`다. HnS `BattleScript_HealerActivates`(`STRINGID_HEALERCURE`)는 그대로다.
- 보존: `STRINGID_PKMNPREVENTSROMANCEWITH`는 `battle_arena.c`와 문자열 표에서 쓰이므로 남겼다(upstream도 남김).
- 삭제한 한글 문장(1줄, 사용처 없음): `[STRINGID_PKMNSXCUREDYPROBLEM] = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_SCR_ABILITY} 때문에\n{B_BUFF1}상태가 나았다!")`
- 검증:
  - `git diff --check` 통과. 빌드: 종료 코드 0, **ROM 32,718,900 B(−64 B) / EWRAM 248,940 B / IWRAM 25,516 B**, 새 경고 0.
  - STRINGID→바이트 대응(#9610 뒤 표와 비교): removed 1(`STRINGID_PKMNSXCUREDYPROBLEM`, 순번 292), added 0, 바이트 변경 0, `STRINGID_ITSUCKEDLIQUIDOOZE`부터 435개 순번 −1. `enum StringID`에 명시 값이 없고 문자열 표는 지정 초기화, 숫자 `printstring`은 매크로 기본값 `printstring 0`뿐(#9494가 `0x3`을 `STRINGID_SWITCHINMON`으로 바꿈), 세이브·녹화에 저장 안 함 → 영향 없음.
  - 테스트(`test/text.c`, `ability/oblivious.c`·`shed_skin.c`·`hydration.c`·`healer.c`): 이식 전과 상태 변화 0.
- 출력 변화: 없음(삭제 대상 모두 미사용). 실기 확인: 필요 없음.

## 동기화 단위: seq 118 #9630 `U-9630` Damage roll speed wizardry

- 현재 판정: 적용
- 커밋: `4e3c6bd5e7`
- upstream 근거: `c107917e6c`(1파일 +3/−3). deps #9568(seq 103 `4bc61b3ffc`) 적용됨.
- 수정 파일(1): `src/battle_ai_util.c` — `LowestRollDmg`·`DmgRoll`·`RandomRollDmg`를 `static inline`에서 `static __attribute__((noinline)) ARM_FUNC`로(패치 `+`/`-` 줄이 upstream과 같음, 오프셋만 다름). `HighestRollDmg`는 upstream도 inline 유지. 본문은 바뀌지 않는다.
- HnS 적응: 없음. #9568 HnS 적응(롤 config 전부 MEDIAN, 아군 KO `.maximum`, Beat Up `// HnS:` 줄)과 겹치는 줄이 없다. `ARM_FUNC`는 IWRAM 배치가 아니라 ROM의 ARM 코드이며, HnS에 이미 같은 형태(`PercentToUQ4_12`)가 있다.
- 동작: C 계산식은 그대로이고 100으로 나누기가 `__divsi3` 호출에서 곱셈(`smull` 매직 넘버)으로 바뀐다.
  - 사전 분석 결과 인용: 호스트 전수 증명(`ai-harness/roll9630/host_div100_proof.c`, 32비트 입력 2^32개 전부에서 매직 나눗셈 = 절삭 나눗셈, 85×/93× 곱 동일, 불일치 0), mGBA 롤 하네스(HnS와 같은 플래그로 이전·이후 롤 함수를 추출해 GBA ROM으로 비교, dmg −65,536~1,048,575 전수 × LOWEST/MEDIAN/HIGHEST/RANDOM, s32 표본, RANDOM 85~100% 전부, 롤마다 RNG 호출 1회·인자 `(RNG_AI_DMG_ROLL_RANDOM, 85, 100)` 검사, 확장 실행 0~2^24 전수, 음성 대조로 92% 변조 검출 확인).
  - **이식 세션 재실행:** `OLD_SRC=<seq 117 커밋의 battle_ai_util.c> NEW_SRC=<4e3c6bd5e7의 파일> build_run.sh` → `bad-mask 00000000`, 검사한 dmg 값 0x210000(2,162,688), RNG 호출 0xC201E0(12,714,464), 잘못된 인자 0, **mgba-rom-test exit 0**.
  - AI 대미지 소비처는 모두 `simulatedDmg[..].{minimum,median,maximum,random}`만 읽고, 이 값을 만드는 곳은 `AI_CalcDamage`의 `GetDamageByRollType` 8회뿐이라 위 비교가 모든 호출처의 동등성이다.
- 검증: `git diff --check` 통과. 빌드: 종료 코드 0, **ROM 32,718,996 B(+96 B, ARM 함수 3개와 Thumb↔ARM 베니어) / EWRAM 248,940 B / IWRAM 25,516 B**, 새 경고 0. 한글 줄 변경 0.
- 테스트: 마감 때문에 AI 파일별 실행은 생략하고 구간 끝 전체 테스트로 대신했다(아래 "구간 끝 전체 테스트").
- 남은 위험: 매우 낮음. 실제 게임에서는 AI 계산이 빨라져 사고 중 지나가는 VBlank 수가 달라지면 이후 난수열이 달라질 수 있다(타이밍 차이, 동작 차이 아님). 실기 확인: 필요 없음(선택: 더블 첫 턴 AI 사고 시간이 길어지지 않았는지).

## 동기화 단위: seq 119 #9634 `U-9634` Removed STRINGID_PKMNISGLOWING

- 현재 판정: 적용(HnS 적응: 한글 문장 줄만 수동 삭제)
- 커밋: `de9b581a28`
- upstream 근거: `fb8819b572`(3파일 +8/−10)
- 수정 파일(3): `include/constants/battle_string_ids.h`, `src/battle_message.c`(한글 1줄 삭제), `src/data/moves_info.h`(불새 `stringId`를 `STRINGID_CLOAKEDINAHARSHLIGHT`로 고정, `.stringId =  X` 두 칸 공백 7곳 → 한 칸). 패치와 upstream의 `+`/`-` 줄 차이는 삭제되는 문장이 한글인 것뿐이다.
- 삭제한 한글 문장: `[STRINGID_PKMNISGLOWING] = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EULREUL}\n세찬 빛이 감쌌다!")`. 사용처는 불새의 `B_UPDATED_MOVE_DATA >= GEN_4 ? STRINGID_CLOAKEDINAHARSHLIGHT : STRINGID_PKMNISGLOWING` Gen3 이하 분기뿐이었고 HnS(`GEN_LATEST`)에서는 선택되지 않았다.
- **대체 문장 확인:** `STRINGID_CLOAKEDINAHARSHLIGHT` = `{B_ATK_NAME_WITH_PREFIX}로부터\n눈부신 빛이 넘쳐흐른다!`(`src/battle_message.c`, 한글). 이식 전후 모두 불새 1턴째에 이 문장이 나온다.
- 검증:
  - `git diff --check` 통과. 빌드: 종료 코드 0, **ROM 32,718,964 B(−32 B) / EWRAM 248,940 B / IWRAM 25,516 B**, 새 경고 0. `pokehns.gba` SHA1 `d439ac3b54a464827093a1a0d80d33e74c83ea5e`.
  - **STRINGID→바이트 대응(#9578 뒤 표와 비교):** removed 1(`STRINGID_PKMNISGLOWING`, 순번 79), added 0, 바이트 변경 0, `STRINGID_PKMNFLEWHIGH`부터 647개 순번 −1.
  - **구간 누적(이식 전 `10077a5d70` 표 대비):** 728 → 726행. removed 2(`PKMNISGLOWING`, `PKMNSXCUREDYPROBLEM`), added 0, 바이트 변경 2(`PKMNFLUNG`·`ABOUTTOUSEPOLTERGEIST` 토큰 바이트만, #9610), 순번 이동 647개(−1/−2), UNSET/REF 13개 동일. 나머지 722개 STRINGID의 인코딩 바이트는 이식 전과 같다. 결과 표가 사전 분석이 세 패치를 스크래치에 적용해 예측한 표(`verify-after.tsv`)와 바이트 동일.
- 테스트: 구간 끝 전체 테스트로 대신했다. `test/text.c` `Battle strings fit on the battle message window` 줄이 `(125/125)`에서 `(124/124)`로 바뀌는 것은 예상된 차이(같은 문장 `STRINGID_PKMNSTOLEITEM`, FAIL 유지).
- 출력 변화: 없음. 실기 확인: 선택(불새 1턴째 문장).

## 구간 끝 전체 테스트

- 명령: `PATH=… GITHUB_ACTION=1 make check BUILD=hns -j8 > build/port-check-post119.log 2>&1`(`de9b581a28`, 12분 4초, 종료 코드 2 = 실패 테스트 있음). 목록은 `PORT_INSTRUCTIONS`의 `LC_ALL=C`·`grep -a` 표준 추출과 확장 추출(`ASSUMPTION_FAIL|INVALID|TIMEOUT|CRASH` 포함) 두 가지.

| 항목 | 이식 전(seq 107) | 이식 후(seq 119) |
|---|---|---|
| 러너 요약 | PASSED 2,314 / FAILED 2,232 / KNOWN_FAILING 8 / ASSUMPTIONS_FAILED 38 / TO_DO 613 / EXPECT_FAILING 6 / TOTAL 5,211 | PASSED **2,321** / FAILED 2,243 / KNOWN_FAILING 10 / ASSUMPTIONS_FAILED 38 / TO_DO 611 / EXPECT_FAILING 6 / TOTAL 5,229 |
| 표준 목록 | 5,142줄(`test-baseline-seq107.txt`) | 5,160줄(PASS 2,318 / FAIL 2,218 / TO_DO 609 / KNOWN_FAILING 10 / EXPECTED_FAIL 5) |
| 확장 목록 | 5,201줄(PASS 2,311 / FAIL 2,207 / TO_DO 611 / KNOWN_FAILING 8 / EXPECTED_FAIL 5 / ASSUMPTION_FAIL 37 / INVALID 21 / CRASH 1) | 5,219줄(PASS 2,318 / FAIL 2,218 / TO_DO 609 / KNOWN_FAILING 10 / EXPECTED_FAIL 5 / ASSUMPTION_FAIL 37 / INVALID 21 / CRASH 1) |

- 표준·확장 목록의 차이는 같은 36줄이다(ASSUMPTION_FAIL·INVALID·CRASH 줄은 전후 같음). **설명되지 않는 PASS 손실 0.**
- **PASS 손실 4건(모두 분류됨):**
  1. `Eject Button activates after Wandring Spirit` → 이름 변경 `…Wandering Spirit`(PASS). #9494.
  2. `Red Card prevents Emergency Exit activation when triggered` → 이름·기대 변경 `Red Card doesn't prevent…`(PASS). #9494.
  3. `Eject Button will activate before Red Card if holder is faster`: PASS → KNOWN_FAILING. upstream #9494의 `KNOWN_FAILING; // #9499`(seq 166 #9784가 해소). 예상 변화.
  4. `Blunder Policy activates for Dragon Darts…`: PASS → KNOWN_FAILING. #9494로 허탕보험이 두 타격 뒤로 옮겨져 HnS가 master에서 먼저 받은 #9790 테스트의 기대 순서와 달라짐. upstream 병합(`7ff83c6542`)과 같은 `KNOWN_FAILING;`, seq 467 #9841이 해소(seq 115 항목).
- 그 밖의 차이: 이름 변경 FAIL/TO_DO 3건(`Relic Song transformation is the last thing…` → `…activates after target faints` FAIL, `Bide has +1 priority if called…` → `…on following turns…` TO_DO, `TODO: Write Poltergeist (Move Effect) test titles` TO_DO 삭제), TO_DO → PASS 1건(`Bide hits the last Pokémon that attacked the user, even allies`), 새 테스트 19건(PASS 8, FAIL 11 — FAIL은 모두 영문 `MESSAGE` 불일치; 삭제된 폴터가이스트 TO_DO 1건을 빼면 순증 18건), `Battle strings fit on the battle message window: (125/125)` → `(124/124)`(같은 문장 `STRINGID_PKMNSTOLEITEM`, #9634로 순번 −1, FAIL 유지, 예상 차이).
- 새 기준 목록: [`test-baseline-seq119.txt`](test-baseline-seq119.txt)(표준 추출 5,160줄).

## 커밋 리뷰 (병렬, 읽기 전용)

마감(16:50) 때문에 메인이 적용 도중에 이미 커밋된 PR부터 리뷰 에이전트 3개를 띄웠다. 리뷰어는 작업 트리를 읽지 않고 `git show`/`git archive`로 커밋만 읽었으며 `make`를 돌리지 않았다. 마지막 3건(#9578, #9630, #9634)은 메인이 직접 확인했다.

| 커밋 | PR | 판정 | 요지 |
|---|---|---|---|
| `70eb6a4271` | #9537(+#9668) | 문제 없음 | HnS·Emerald 두 설정에서 `OBJ_EVENT_GFX_*` 554개 값 변화 0, 이름만 259·279·281. 규칙 이름 `crush_girl`·`black_belt_frlg`·`poke_maniac_frlg`(오타 없음). PNG 이동 3개는 100% rename이고 덮어쓴 파일 없음. `*_hns` 그림·규칙·맵 변경 0. |
| `ec9dca2712` | #9241 | 문제 없음 | HNS·EMERALD·FIRERED 세 설정에서 (class, 그래픽) 쌍 남 30·여 20이 이식 전과 같다. `facilityClass` 최대 139로 u8 값 불변. |
| `4440c18163` | #9557 | 문제 없음 | upstream과 다른 곳은 `// HnS:` 주석·`#if` 가드·`MAPSEC_NONE` 대체 표뿐. `MPS_ENABLE_MAP_PREVIEWS` 0이면 새 경로가 모두 FALSE라 문 출입·동굴 전환·맵 이름 팝업이 이식 전 분기를 탄다. 동작 변화는 BG 팔레트 13 한 줄(upstream대로). |
| `47cd51facf` | #9610 | 문제 없음 | hunk가 upstream `e16cc7a1f2`와 같고(선언 위치·빈 줄만 다름) `AccuracyCheck`에서 #9929 삼항식이 유지된다. 두 문장은 토큰만 바뀌었고 가리키는 배틀러가 같다. 폴터가이스트+대타출동 변화가 출력 변화 문서에 있다. |
| `2d86edca81` | #9587 | 문제 없음 | upstream `5774efaea1`과 같다. `speedStats`는 `SetBattlerAiData`에서 채운다. |
| `f6ef307f76` | #9596 | 문제 없음 | `holdEffectParams` 제거·`ShouldTryOHKO` 변경이 upstream과 같고, `turnOrder` 제외는 1.17.0 최종형과 같아 타당하다. |
| `7e4f61c927` | #9532 | 문제 없음 | upstream과 다른 곳은 `// HnS: animTurn = 1`(방출 분기, `BattleScriptCall(BideAttack)` 앞), 빈 줄 hunk 제외, Champions 문맥뿐. opcode 헤더·명령 표가 upstream과 바이트 동일(`UNUSED_32`=0xfd, `UNUSED_33`=0xfe). Champions 상성 문구는 `DoFixedDamageMoveCalc`가 지워 계속 나오지 않는다. |
| `3f7f0ddabf` | #9494 | 문제 없음 | upstream `bede100c3e`와 다른 곳은 원시 날씨 `@ HnS:` 2줄(옛 탈출버튼 경로와 같은 자리), 1인자 `IsAnyTargetTurnDamaged`, 매지션 점착 hunk 제외, blunder_policy `KNOWN_FAILING`(upstream `7ff83c6542`와 같음). 보존 대상 HnS 코드 모두 남음. `NeutralizingGasExits` 저장·복원 2줄이 upstream과 같은 자리. AI `notOnField` 저장·복원 사이 조기 return 없음. |
| `489c58259c` | #9864 | 문제 없음 | upstream `47f01e61ba`와 내용이 같다. |
| `3960fc0c8e`·`4e3c6bd5e7`·`de9b581a28` | #9578·#9630·#9634 | 문제 없음(메인 확인) | 한글 줄 변경은 #9578·#9634의 미사용 문장 1줄씩 삭제뿐이고, 삭제된 두 `STRINGID` 참조가 남지 않았다. #9634의 대체 문장 `STRINGID_CLOAKEDINAHARSHLIGHT`("…로부터\n눈부신 빛이 넘쳐흐른다!")가 있다. #9630의 +/− 줄이 upstream `c107917e6c`와 같다. |

참고(이식 결함 아님, upstream #9494에서 생겨 1.17.0에도 있음, 후속 검토): 더블배틀 시작 때 둘째 칸이 기절한 포켓몬이면 그 칸은 부재 처리되지만 `notOnField`가 켜지지 않는다. #9494가 `IsBattlerAlive` 검사를 지우면서 이 포켓몬의 특성이 이제 적용된다(예: 기절한 클라우드나인 → 날씨 무효, 기절한 프레셔 → PP 추가 감소, 피뢰침 → 전기 기술 유도). 사용 가능한 포켓몬이 한 마리뿐인 더블배틀에서만 생긴다. HnS 가드를 넣을지는 나중에 정한다.

메인 검증: `rm -f pokehns.elf pokehns.gba` 뒤 `make hns -j8` 종료 코드 0, ROM 32,718,964 B(97.51%), EWRAM 248,940 B, IWRAM 25,516 B, SHA1 `d439ac3b54a464827093a1a0d80d33e74c83ea5e`. `build/port-check-post119.log`를 `LC_ALL=C`로 다시 추출해 `test-baseline-seq119.txt`와 같음을 확인했다. seq107 대비 PASS 손실 4건은 위 "구간 끝 전체 테스트" 분류와 같다. docs 밖 한글 줄 변경은 #9610 토큰 교체 2쌍, #9578·#9634 문장 삭제 1줄씩이다.

## 실기 확인 항목 (친구용)

이식 전 ROM(`10077a5d70`, SHA1 `0c91520c…`)과 이식 후 ROM(`de9b581a28` 이후)을 비교한다. 기술·특성·도구·지명은 `src/data/moves_info.h`·`abilities.h`·`items.h`·`src/data/region_map/region_map_sections.json`·`src/strings.c`의 표기다.

1. **배틀프런티어 트레이너 그림(#9241):** 배틀타워 대전 방에 들어갈 때 상대 트레이너의 필드 그림, 배틀피라미드 층 트레이너 그림, 배틀팩토리·배틀팰리스·배틀돔·배틀튜브·배틀아레나 중 한 곳 이상. 이식 전 세이브를 불러와 배틀타워 기록 트레이너·견습생 그림이 같은지.
2. **내던지기·폴터가이스트(#9610):**
   - 내던지기(상처약·화염구슬·오랭열매 등 소지): `{공격자}은(는)\n{도구}을(를) 내던졌다!` → 공격 애니메이션 → 대미지. 조사와 상대 접두어(`상대의 …`). 문장 앞 짧은 멈춤이 없어지고 문장 뒤 대기가 길어진 것. 빗나감(상대 반짝가루)·행동 불가 때는 문장 없이 도구만 소모.
   - 폴터가이스트(대상이 도구 소지): `{도구}이(가)\n{대상}에게 덤벼들었다!` → 애니메이션 → 대미지. 받침 있는/없는 도구명(예: 상처약이, 오랭열매가)에서 도구명이 제대로 나오는지(`{B_LAST_ITEM}` 교체 확인). 대상이 도구 없음 → `그러나 실패하고 말았다!`, 노말 타입 대상 → 효과 없음 문구만, **대상이 대타출동 상태 → 도구 공개 문장 없이 대타출동 대미지(변경점)**.
   - 깨뜨리다·사이코팽·레이징불 방벽 제거, 섀도스틸 능력치 빼앗기 문장이 이전과 같은지.
3. **(선택) 교체 AI(#9587):** Smart Trainer 싱글에서 AI 쪽에 끈적끈적네트가 깔린 상태로 기절 후 교체할 때 후속 포켓몬 선택·멈춤 여부, 더블에서 한 자리가 비었을 때 애널라이즈·보복 보유 AI의 기술 선택.
4. **참기(#9532):** 3턴 흐름(싱글·더블). 2·3턴째 `…은(는)\n참기를 썼다!`가 없어지고 `…은(는) 참고 있다` / `…의\n참기가 풀렸다!`만 나오는지. **방출 턴 연출이 준비 연출(흔들림)이 아니라 방출 연출인지(HnS `animTurn = 1`)**. 방출 피해가 받은 피해의 2배인지, 고스트 타입 대상 효과 없음, 받은 피해 0이면 `그러나 실패하고 말았다!`, 대타출동·방어·칼등치기 0 피해, 비비드바디 상대, 더블에서 아군에게 맞았을 때 아군을 반격, 손가락흔들기로 나온 참기.
5. **교체 대기열(#9494·#9864):**
   - 탈출버튼(싱글·더블): 아이템 팝업 → `…은(는)\n탈출버튼 때문에 돌아간다!` → 볼 회수 → 교체 화면 → `가랏! …!`. 공격자가 생명의구슬을 들었으면 교체 화면 전에 `…의\n생명이 조금 깎였다!`.
   - 레드카드: 공격자 강제 교체. 레드카드 보유자가 위기회피(예: 갑주무사)면 이어서 교체되는지.
   - 위기회피·도망태세: 목스프레이를 든 공격자의 하이퍼보이스 뒤 목스프레이가 교체 전에 발동. 트레이너·야생 배틀.
   - 유턴·볼트체인지·퀵턴: 상대 나쁜손버릇이 교체 전에 발동, 상대 탈출버튼·레드카드가 먼저 발동하면 유턴 교체가 없는지, 따라가때리기와의 조합.
   - 탈출팩: 위협, 막말내뱉기, 드래곤테일·배대뒤치기. 배턴터치: 볼 회수 뒤 파티 화면.
   - 화학변화가스 보유자가 탈출버튼·탈출팩으로 나갈 때 `화학변화가스의 효과가 사라졌다!` 뒤 올바른 포켓몬의 교체 화면.
   - 옛노래 메로엣타(생명의구슬): 폼체인지 문구 `…의\n모습이 변화했다!`가 반동보다 먼저. 매지션으로 빼앗은 목스프레이 발동. 드래곤애로 허탕보험이 두 타격 뒤 한 번.
   - **원시 날씨(HnS 유지):** 끝의대지 그란돈(주홍구슬)이 유턴·탈출버튼 등으로 나가면 교체 직후 `햇살이 원래대로 되돌아왔다!`(시작의바다 `강한 비가 그쳤다!`, 델타스트림 `수수께끼의 난기류가 가라앉았다!`).
   - 교체 대기 중 파티 화면을 열었다 닫을 때 이전 포켓몬 체력 상자가 숨겨지는지(#9864). 파트너 멀티 배틀에서 파트너 포켓몬의 탈출버튼.
6. **맵 미리보기 이식(#9557, 미리보기 자체는 HnS에서 꺼짐):** 문으로 드나든 뒤 바로 조작이 풀리는지(예: 고동마을 집), 동굴 진입 전환과 맵 이름 팝업이 그대로인지(어둠의 동굴, 연결동굴, 절구산), 지역 이동 팝업(고동마을 → 33번 도로, 로켓단아지트). 비 날씨 맵(진청시티·담청시티 `WEATHER_RAIN`, 분노의 호수 `WEATHER_RAIN_THUNDERSTORM`)에서 필드·대화창 색. 팔레트 13 창(엘리베이터 층 표시, 상점 금액 창)을 비 날씨 맵에서 열면 테두리 색만 달라질 수 있다(upstream 의도).

## 후속 행 메모

- **seq 126 #9668:** "HnS 동등(seq 108 커밋 `70eb6a4271`에 포함, 오타 `poke_manic_frlg` 제외)", 커밋 없음.
- **seq 166 #9784:** 넣지 않았다. 선행 #9717(seq 150)·#8943(seq 138.5) 뒤에 이식한다. 그때 `Eject Button will activate before Red Card if holder is faster`의 `KNOWN_FAILING`이 풀리는지 보고, `BattleScript_QueuedSwitch`의 HnS 원시 날씨 2줄(`@ HnS:`) 문맥을 맞춘다.
- **seq 176 #9864:** "이미 적용(`489c58259c`)", 커밋 없음.
- **seq 250 #10281:** "HnS 동등"(규칙 이름이 이미 올바름). seq 500 #9881(INCGFX) 때 `gObjectEventPic_PokeManiacFrlg` INCGFX 줄에 `-mwidth 2 -mheight 4`가 붙는지 확인.
- **seq 366 #10080:** #9557 변형 B 위에 적용 가능(사전 분석 확인).
- **seq 467 #9841:** `blunder_policy.c`가 upstream 병합 상태(`KNOWN_FAILING;`, blob `fbba72441b`)와 같으므로 그대로 적용되고 드래곤애로 허탕보험 테스트가 PASS로 돌아와야 한다.
- **seq 500 #9881(INCGFX):** `map_preview_screen.c` 가드 안의 `INCBIN_U8` 63줄을 `INCGFX_U8`로 옮긴다.
- **#9657·#10220·#9939(공격 전 효과·명중 판정 캔슬러 이동):** `AccuracyCheck`의 #9929 분기(`BattleScript_TargetAvoidsAttackEnd`)와 #9610 내던지기 분기, `MOVE_EFFECT_ITEM_MESSAGE`, 한글 2문장 토큰을 이번 결과 기준으로 유지한다.
