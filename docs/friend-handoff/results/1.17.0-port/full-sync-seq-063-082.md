# full-sync 실제 port 결과 — seq 63~82

진행 중: seq 80 (#9249) unit 진행 중 — #9249 커밋 완료, 이어서 같은 unit #10648. 그다음 seq 81 (#9051).

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), [`CLAUDE_FULL_SYNC_PORT_PROMPT.md`](../../CLAUDE_FULL_SYNC_PORT_PROMPT.md)
시작 HEAD: `72f40563ad`

## seq 63~82 공통 사항

- 빌드 명령: `PATH=/opt/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin:$PATH GITHUB_ACTION=1 make hns -j8 > build/port.log 2>&1` (노트북 WSL, 8코어).
- 기준 빌드(`72f40563ad`, `rm -rf build/hns` 뒤 전체 재빌드, 약 49초): 종료 코드 0, ROM 32,739,220 B(97.57%), EWRAM 248,892 B(94.94%), IWRAM 25,516 B(77.87%). 경고 줄 166개, 파일·메시지 기준 고유 목록 44개(기존 `-Woverride-init`·미사용 함수/변수·링커 RWX 계열). 시작 전 `pokehns.gba`(11:26 빌드)와 바이트 동일.
- 경고 비교: 매 빌드의 경고를 "파일: 메시지"(줄 번호 제거) 목록으로 만들어 기준 목록과 비교하고 새 경고만 확인했다.
- 테스트 기준: [`test-baseline-seq062.txt`](test-baseline-seq062.txt)(PASS 2,283 / FAIL 2,218 / TOTAL 5,175, 이름 중복 제거 목록 5,107행). PR마다 관련 테스트 파일을 이식 전후로 돌려 비교했다. 통과하던 것이 실패로 바뀌면 회귀, `Unmatched MESSAGE`만이 사유면 알려진 한계로 본다.
- 한글 포함 소스 줄: 커밋마다 `git show <커밋> | grep -aP '^[-+](?![-+]).*[^\x00-\x7F]'`로 비ASCII 줄 변경을 확인했다.

## 동기화 단위: seq 63 #9066 `U-enum-9066` Typeless enums for OBJ_EVENT_GFX, Multichoice Ids, and Facility Classes

- 현재 판정: 적용(HnS 적응: 모든 값 명시 대입)
- 커밋: `3cbed2e8f7`
- upstream 근거: `5e3e3b1a28`
- 해결한 의존성: 없음
- 수정 파일: `include/constants/event_objects.h`, `include/constants/script_menu.h`, `include/constants/trainers.h`
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - upstream은 `#define` 목록을 값 없는 typeless `enum { A, B, ... }`으로 바꿔 값이 순번으로 다시 매겨진다(upstream 자신도 `GREEN_NORMAL` 251 → 250으로 바뀜). HnS는 **모든 enumerator에 원래 숫자값을 명시 대입**했다(`OBJ_EVENT_GFX_X = N,`, 16진 표기는 그대로 `0x..`).
  - OBJ_EVENT_GFX: 숫자 상수 537개(0~547) + `NUM_OBJ_EVENT_GFX = 548`. 값 틈 250과 388~397은 비워 두고 `// 250 is unused`, `// 388-397 are unused` 주석만 달았다. `*_HNS` 상수 전부 포함. 파일 뒤쪽(NOTE 주석 아래)에 있던 `NURSE_CHANSEY_HNS`·HnS 주인공 22개 줄을 enum 안(조명 뒤)으로 옮기고, `// FRLG equivalents`·NOTE 주석은 upstream처럼 enum 뒤에 그대로 두었다(비ASCII `Pokémon` 줄 불변). `OBJ_EVENT_GFX_VARS`/`VAR_0`~`VAR_F`, `OBJ_EVENT_GFX_MON_BASE`, `PLAYER_AVATAR_GFX_*`, `FIRST_DECORATION_SPRITE_GFX`, `OBJ_EVENT_GFX_SPECIES()` 등 파생 정의는 `#define` 그대로.
  - Facility class: 139개(0x00~0x8A) + `FACILITY_CLASSES_COUNT = 0x8B`를 enum으로. 별칭 `#define`(`FACILITY_CLASS_SUPER_NERD` 등 13개 → HIKER, HnS `FACILITY_CLASS_FIREBREATHER` → KINDLER)은 그대로. upstream처럼 `RS_FACILITY_CLASS_*` 77개 + `RS_FACILITY_CLASSES_COUNT = 0x4D`도 enum으로.
  - Multichoice: `MULTI_*` 178개(0~177, HnS 전용 159~177 포함)를 enum으로. 실제 값은 0~177 연속이다(지시서의 "값 틈"은 뒤에 주석이 붙은 줄 `MULTI_PC`·`SSTIDAL_LILYCOVE`·`UNUSED_ASH_VENDOR`·`UNUSED_SSTIDAL_1~4`이 정규식에서 빠져 보인 것). `MULTI_NONE 255`, `MULTI_B_PRESSED 127`은 upstream처럼 `#define` 유지. 다른 헤더의 `MULTI_BATTLE_*`, `MULTI_PARTY_SIZE`는 무관(값 비교표에 포함해 불변 확인).
  - 새 헤더 주석 2줄(영문): 명시 대입 이유(세이브·맵 데이터 값 보존).
- 저장·ROM·그래픽 영향: **세이브 값 불변**(SaveBlock1 `objectEventTemplates`·`objectEvents`의 `graphicsId`, `facilityClass`, 맵 데이터). ROM **바이트 동일**.
- 검증:
  - **값 표 비교(C 경로):** 이식 전후 각각 세 헤더(+`global.h`, `constants/battle_special.h`)를 include한 임시 C 파일을 실제 빌드와 같은 `arm-none-eabi-cpp` + `preproc` + `cc1 -O2` 경로로 컴파일해, 상수마다 `const long long v = (NAME);` 초기값을 어셈블리 출력에서 읽었다. 이름 목록은 이식 전 헤더의 object-like `#define` 전부 + 이식 후 enum 멤버 + include 폴더 전체의 `MULTI_*` + `OBJ_EVENT_GFX_SPECIES*(PIKACHU)` 표본 4개.
    - event_objects 746행(숫자 746), script_menu 239행(숫자 237, 나머지 2개는 HnS 설정상 `#if I_COMBINE_BAG_POCKETS == FALSE` 밖이라 정의되지 않는 `STDSTRING_BATTLE_ITEMS`·`TREASURES`), trainers 674행(숫자 674). 이 중 OBJ_EVENT_GFX 계열 560, FACILITY_CLASS 계열 154(별칭 포함), RS_FACILITY_CLASS 78, MULTI_ 185.
    - 전후 diff: **세 계열 모두 0줄.**
  - **값 표 비교(asm 경로):** 같은 이름으로 `.4byte NAME` 목록을 만든 `.s`를 `data/*.s` 빌드와 같은 `preproc -s | cpp -I include | preproc -ie | as`로 조립하고 `.data` 내용을 읽었다(재배치가 걸린 항목은 미정의로 표시). event_objects 742행(숫자 697, 나머지는 C 전용 식 `IS_HNS ? :`·`1u` 접미사 18개와 asm에 정의 없는 팔레트 태그 27개), script_menu 239행(숫자 239), trainers 674행(숫자 668). 전후 diff: **세 계열 모두 0줄.** 이식 후 값은 preproc가 만든 `.equiv NAME, (N) + 0`에서 온 것임을 확인했다(예: `OBJ_EVENT_GFX_GREEN_NORMAL, (251) + 0`, `FACILITY_CLASSES_COUNT, (0x8B) + 0`).
  - 표 파일(임시, 재생성 가능): 스크래치 `enum9066/pre_{event_objects,script_menu,trainers}_{c,asm}.tsv`, `post_*` 같은 이름. 생성 스크립트 `enum9066/constvals.py`, 변환 스크립트 `enum9066/convert.py`.
  - `#if`/`#ifdef`로 이 상수들을 쓰는 곳 0건(`src`·`include`·`data`·`asm`·`test`·`tools`), 이 헤더를 파싱하는 도구 0건.
  - `git diff --check`: 통과. 비ASCII 줄 변경 0건.
  - `make hns -j8`: 종료 코드 0, ROM 32,739,220 B / EWRAM 248,892 B / IWRAM 25,516 B, 새 경고 없음. **`pokehns.gba`가 기준 ROM과 `cmp` 바이트 동일.**
  - 자동 테스트: 해당 테스트 없음(ROM 동일). 전체 테스트는 구간 끝에서 확인.
  - 실기 확인: 불필요(ROM 동일)
- 남은 위험: 없음. 이후 upstream PR이 이 enum에 새 멤버를 값 없이 추가하면 HnS에서는 반드시 명시 값을 붙여야 한다(틈 250·388~397에는 넣지 않는다).

## 동기화 단위: seq 64 #9121 `U-heap-9121` Reduce heap usage in battle (+ seq 76 #9474)

- 현재 판정: 적용(같은 unit의 #9474를 같은 커밋에 포함)
- 커밋: `2ba44de454`
- upstream 근거: #9121 `fc9427b1bf`, #9474 `4ce8738dae`
- 해결한 의존성: #9474(seq 76)는 #9121이 만든 회귀(대타 프레임 복사 `i < 4`가 2프레임 버퍼를 넘어 다음 배틀러 버퍼를 덮음)의 수정이라 group plan대로 같은 커밋에 넣었다. **seq 76은 "이미 적용"으로 처리한다.**
- 수정 파일: `src/battle_gfx_sfx_util.c`(`AllocateMonSpritesGfx` 배틀러당 버퍼 `MON_PIC_SIZE * 4` → `* MAX_MON_PIC_FRAMES`(2), `BattleLoadSubstituteOrMonSpriteGfx` 복사 루프 `i < 4` → `i < 2`)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용(HnS 코드가 upstream 부모와 같음). HnS의 다른 `spritesGfx` 사용처(유령 앞모습 `DecompressGhostFrontPic`, 기절 낙하 `battle_main.c` 지우기 루프 0x100 단위, 요약·진화·교환·콘테스트 등)는 upstream과 같은 목록이고 모두 1~2프레임 안이다.
- 저장·ROM·그래픽 영향: 배틀 힙 16 KB(`MON_PIC_SIZE`×2×4) 절감. ROM −48 B. 세이브 무관.
- 검증:
  - `git diff --check`: 통과
  - `make hns -j8`: 종료 코드 0, ROM 32,739,172 B / EWRAM 248,892 B / IWRAM 25,516 B, 새 경고 없음
  - 자동 테스트: `test/battle/move_effect/substitute.c` 5건 — FAIL 4(모두 `Unmatched MESSAGE`, 기준과 같음), TO_DO 1. 기준 대비 변화 없음.
  - 실기 확인: 권장. 더블배틀에서 상대 오른쪽(opponentRight)·왼쪽이 대타를 쓸 때 대타 인형 표시와 다른 배틀러 스프라이트가 깨지지 않는지, 대타가 깨진 뒤 원래 모습 복귀.
- 남은 위험: 낮음

## 동기화 단위: seq 65 #9086 `U-cstring-9086` preproc: (shared) COMPOUND_STRING

- 현재 판정: 적용(HnS 적응 1곳)
- 커밋: `6c019cffe4`
- upstream 근거: `44e9991e8b`
- 해결한 의존성: 없음. 같은 unit의 #9463(seq 73)은 순서대로 뒤에서, #9667(seq 124)은 그 순서에서 이식한다(선행 조건일 뿐 같은 커밋 요구 없음).
- 수정 파일: `tools/preproc/c_file.cpp`·`c_file.h`(COMPOUND_STRING을 preproc가 직접 변환, 파일 앞에 `.rodata.compound_string.<hash>` SHF_MERGE 배열로 출력), `include/metaprogram.h`(`COMPOUND_STRING` 매크로 삭제, `COMPOUND_STRING_SIZE_LIMIT` 재정의), `include/global.h`(IDE용 더미), `include/test/battle.h`(`MESSAGE`), `ld_script_modern.ld`·`ld_script_test.ld`(`.rodata.compound_string` 출력 섹션을 `.rodata` 앞에), `src/contest.c`·`src/contest_painting.c`(배열 초기화 → `_()`), `src/challenge_menu.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - HnS `tools/preproc/c_file.cpp`·`c_file.h`는 upstream 부모와 같아 그대로 적용. 한글 2바이트 변환은 `charmap.txt`·`StringParser`가 하며 바뀌지 않았다(새 `ConvertString`은 기존 `TryConvertString` 본문을 옮긴 것).
  - HnS 전용 `src/challenge_menu.c:507` `static const u8 sText_ConfirmSave[] = COMPOUND_STRING("이 설정으로 결정하겠습니까?")`는 배열 초기화라 새 방식(포인터)으로는 컴파일되지 않으므로 `_()`로 바꿨다(group plan 지시). 다른 배열 초기화 사용처는 0건.
  - 링커 스크립트: HnS `.rodata`가 `*.o(.rodata*)`로 잡으므로 새 섹션을 반드시 앞에 둬야 한다. upstream hunk 위치가 그대로 `.rodata` 앞이다. `-ffunction-sections -fdata-sections`(`7e7c38ab10`)와는 무관(명시 섹션 이름, 미참조분은 `--gc-sections`로 제거).
- **한글 인코딩 검증(필수):**
  - 방법: 이식 전후 각각 Makefile과 같은 파일 집합(`src/`·`test/`의 `*.c`, `*/*.c`, `*/*/*.c`, `.inc.c` 제외) 1,338개를 실제 빌드 경로(`arm-none-eabi-cpp` → `preproc`, src는 `TESTING=0`, test는 `TESTING=1`)로 전처리해, preproc가 만든 모든 문자열 바이트 열을 소스 순서대로 뽑았다. 이전 출력은 인라인 `{ 0x.., 0xFF }`, 이후 출력은 `_()` 인라인 + `sCompoundString_<hash>` 참조를 파일 앞 배열 정의로 풀어 바이트 열로 바꿨다(종결 `0xFF`, `__()`의 무종결 포함). 섹션 속성·형식 차이는 비교에서 뺐다.
  - 결과: 파일 1,338개 전부 오류 없음. **1,337개 파일은 문자열 바이트 열 순서까지 완전히 같다.** 나머지 1개 `src/item.c`는 `ITEM_NAME`/`ITEM_PLURAL_NAME`(=`COMPOUND_STRING_SIZE_LIMIT`)이 새 매크로에서 같은 문자열을 여러 번 전개해 개수만 늘었고(5,108 → 7,274, 1,083개 × 2), 연속 중복을 합치면 순서까지 같다. 파일별 고유 문자열 집합은 1,338개 모두 같다. 해시 참조 미해결 0건. **불일치 0.**
  - 규모: 이전 문자열 28,879개(연속 중복 합친 24,177개, src 고유 15,409개), 이후 31,045개(COMPOUND_STRING 배열 정의 12,371개). 한글 문자열 리터럴이 있는 소스 줄 7,144개(그중 COMPOUND_STRING 2,768개) 포함.
  - ROM 대조: 이전 ROM에 들어 있던 src 고유 문자열 14,752개 중 14,751개가 새 ROM에도 그대로 있다. 남은 1개(`00 C7 C9 D0 BF FF` = `" MOVE"`, `src/data/trade.h` 미사용 `sText_SpaceMove`)는 이전 ROM에서도 문자열 자체가 아니라 `easy_chat.o` 포인터 표 바이트와 우연히 일치했던 것이라(주소가 바뀌어 사라짐) 누락이 아니다.
  - 비ASCII 소스 줄 변경 4줄(`challenge_menu.c` 한글 1줄, `contest.c` `…` 3줄)은 따옴표 안 리터럴이 바이트 동일하고(`COMPOUND_STRING(` → `_(`만 다름), 두 파일의 preproc 바이트 열도 전후 동일하다.
  - 스크립트·결과(임시): 스크래치 `cstr9086/cstrings.py`, `cstr9086/old/`, `cstr9086/new/`(파일별 목록과 `_summary.txt`), `cstr9086/romcheck.py`.
- 저장·ROM·그래픽 영향: 같은 문자열이 번역 단위 사이에서 병합돼 **ROM −23,424 B**(32,739,172 → 32,715,748). `.rodata.compound_string` 0x57242(356,930 B), `.rodata` 0x1b14db8 → 0x1ab8498. 세이브 무관.
- 검증:
  - `git diff --check`: 통과
  - `rm -rf build/hns` 뒤 `make hns -j8`(preproc 재빌드 포함): 종료 코드 0, ROM 32,715,748 B(97.50%) / EWRAM 248,892 B / IWRAM 25,516 B, 경고 목록 기준과 같음(새 경고 0)
  - 자동 테스트: `MESSAGE` 매크로가 모든 배틀 테스트에 걸리므로 **전체 테스트**를 돌렸다. PASS 2,283 / FAIL 2,218 / KNOWN_FAILING 8 / TO_DO 622 / EXPECT_FAILING 6 / ASSUMPTIONS_FAILED 38 / TOTAL 5,175. 테스트별 상태 목록(5,107행)이 `test-baseline-seq062.txt`와 **바이트 동일**(회귀 0, 새·사라진 테스트 0).
  - 참고: 목록을 만들 때 `LC_ALL=C`를 써야 한다. 이름에 한글 바이트가 섞인 "… fit on …" 테스트 23건은 UTF-8 로캘의 GNU grep `.`에 걸리지 않아 빠진다(기준 목록은 이 23건을 포함).
  - 실기 확인: 권장. 메뉴·배틀·필드 대화의 한글 문자열 전반(특히 도구 이름/설명, 챌린지 메뉴 "이 설정으로 결정하겠습니까?", 콘테스트 문구).
- 남은 위험: 낮음. preproc 해시는 파일명을 넣지 않는 64비트 FNV-1a라 이론상 충돌 가능성이 있다(upstream 주석과 같음). 이번 비교에서 충돌로 인한 잘못된 참조는 0건.

## 동기화 단위: seq 66 #9420 `U-9420` Removes unused files

- 현재 판정: 적용(문맥만 수동)
- 커밋: `e773b47f32`
- upstream 근거: `eb1f68323d`
- 수정 파일: `src/battle_partner.c`, `src/data.c`(include 1줄씩 삭제), `src/data/partner_parties.h`·`src/data/trainer_parties.h`(1바이트 빈 파일 삭제)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: HnS는 include 바로 뒤에 `#if !TESTING`이 있어 `git apply`가 실패하므로 include 줄만 직접 지웠다. 두 헤더를 쓰는 곳은 이 2곳뿐(`migration_scripts/1.9`의 설명 문자열 제외).
- 저장·ROM·그래픽 영향: 없음(ROM 크기 동일)
- 검증:
  - `git diff --check`: 통과
  - `make hns -j8`: 종료 코드 0, ROM 32,715,748 B / EWRAM 248,892 B / IWRAM 25,516 B, 새 경고 없음
  - 자동 테스트: 해당 없음(빌드 전용). 구간 끝 전체 테스트에서 테스트 빌드 의존성 확인.
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 67 #9410 `U-9410` pr adding telekinesis ban list to species data

- 현재 판정: 적용(HnS 비트필드 적응)
- 커밋: `1513e3840b`
- upstream 근거: `b3114ae9d3`
- 수정 파일: `include/pokemon.h`, `src/battle_script_commands.c`, `src/data/pokemon/species_info/gen_1_families.h`, `src/data/pokemon/species_info/gen_7_families.h`
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - `SpeciesInfo`에 `isTelekinesisBanned:1`을 upstream처럼 `isSkyBattleBanned` 뒤에 넣고, HnS 전용 `randomizerMode:2`·`dexNotRequired:1`은 그대로 둔 채 `padding4`를 6 → 5로 줄였다(32비트 워드 크기 불변).
  - `sTelekinesisBanList`(디그다·닥트리오·알로라 2종·모래꿍·모래성이당·메가팬텀 7종)를 종 플래그로 옮기고 `IsTelekinesisBannedSpecies`에 `SanitizeSpeciesId`를 넣었다. HnS 알로라 폼은 `.dexNotRequired = TRUE` 줄이 있어 `gen_1_families.h` 패치가 실패하므로, 7종 블록마다 `.levelUpLearnset` 바로 앞에 스크립트로 넣었다(종 블록이 파일 안에 각각 1개뿐임 확인, 플래그 총 7개).
- 저장·ROM·그래픽 영향: ROM 크기 변화 없음(32,715,748 B). 세이브 무관(ROM 데이터).
- 검증:
  - `git diff --check`: 통과
  - `make hns -j8`: 종료 코드 0, ROM 32,715,748 B / EWRAM 248,892 B / IWRAM 25,516 B, 새 경고 없음
  - 자동 테스트: `test/battle/move_effect/telekinesis.c` 5건 — FAIL 3(모두 `Unmatched MESSAGE`), TO_DO 2. 기준과 같음.
  - 실기 확인: 불필요(동작 동일, 목록 동일)
- 남은 위험: 없음

## 동기화 단위: seq 68 #9388 `U-9388` update poparraywithBattlers Arg to prevent warning

- 현재 판정: 적용
- 커밋: `6cd587e185`
- upstream 근거: `d20d14a15d`
- 수정 파일: `src/battle_main.c`(`PopulateArrayWithBattlers(u8 *)` → `(enum BattlerId *)`)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. 호출처 2곳은 이미 `enum BattlerId battlers[MAX_BATTLERS_COUNT]`를 넘기고, `enum BattlerId`는 `__attribute__((packed))`(1바이트)라 원소 크기가 같다.
- 저장·ROM·그래픽 영향: 없음
- 검증:
  - `git diff --check`: 통과
  - `make hns -j8`: 종료 코드 0, ROM 32,715,748 B / EWRAM 248,892 B / IWRAM 25,516 B, 새 경고 없음
  - 자동 테스트: 해당 없음
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 69 #9107 `U-9107` AI scores Order Up stat boosts under Commander

- 현재 판정: 적용
- 커밋: `34aaa7f6d9`
- upstream 근거: `1f81dd27ef`
- 수정 파일: `src/battle_ai_main.c`(`AI_CalcAdditionalEffectScore`에 `MOVE_EFFECT_ORDER_UP` case), `test/battle/ai/ai_check_viability.c`(새 테스트 1개)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용(오프셋만). 엔진 `MOVE_EFFECT_ORDER_UP`도 CURLY/DROOPY/STRETCHY 3종만 처리하므로 HnS 메가 싸리룡 3종은 AI·엔진 모두 default(효과 없음)로 일관된다. #9730 이식 때 1.17.0 형태(`IncreaseStatUpScore(stat, 1)`)로 함께 바뀐다.
- 저장·ROM·그래픽 영향: ROM +160 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j8`: 종료 코드 0, ROM 32,715,908 B / EWRAM 248,892 B / IWRAM 25,516 B, 새 경고 없음
  - 자동 테스트: `test/battle/ai/ai_check_viability.c` 31건 — PASS 24(새 테스트 "AI scores Order Up's stat boost only with Commander" PASS 포함), FAIL 6(모두 `Unmatched MESSAGE`, 기준과 같음), ASSUMPTION_FAIL 1(기준 목록 형식에 없는 상태, 기존과 같음). 기준 대비 회귀 없음.
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 70 #9407 `U-fade-9407` Fade background and sprites simulatneously

- 현재 판정: 적용
- 커밋: `f09cd6f890`
- upstream 근거: `8f123fb8b7`
- 해결한 의존성: 같은 unit의 #9549(seq 94), #9707(seq 138), #10573(seq 319)을 group plan("#9549·#9707·#10573을 같은 unit으로 연속 적용")대로 바로 뒤 커밋으로 넣는다(아래 항목). #9549·#9707은 #9407 회귀 수정이고, #10573은 동시 페이드를 `FadeSelectedPals` opt-in으로 되돌리는 재작업이라 unit 최종 동작이 1.17.0과 같아진다.
- 수정 파일: `src/palette.c`(`UpdateTimeOfDayPaletteFade`·`UpdateNormalPaletteFade`가 배경·스프라이트 팔레트를 2프레임마다 동시에 블렌드)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: HnS `src/palette.c`는 upstream 부모와 같아 그대로 적용. upstream이 넣은 공백만 있는 줄 1개는 `git diff --check` 통과를 위해 빈 줄로 뒀다(동작 무관).
- 저장·ROM·그래픽 영향: ROM −96 B. 화면 페이드 방식 변화(이 커밋 단독으로는 모든 소프트웨어 페이드가 동시 방식, #10573 뒤에는 기존 번갈아 방식으로 복귀).
- 검증:
  - `git diff --check`: 통과
  - `make hns -j8`: 종료 코드 0, ROM 32,715,812 B / EWRAM 248,892 B / IWRAM 25,516 B, 새 경고 없음
  - 자동 테스트: 해당 없음
  - 실기 확인: unit 끝(#10573)에서 함께 확인
- 남은 위험: unit 끝에서 기록

### unit U-fade-9407: #9549 Fix blend-immune sprite not fading properly (순서표 seq 94)

- 현재 판정: 적용(같은 unit, #9407 바로 뒤). **seq 94 담당은 "이미 적용"으로 처리한다.**
- 커밋: `b00b2cb140`
- upstream 근거: `eebd085c24`
- 수정 파일: `src/palette.c`(`UpdateTimeOfDayPaletteFade`의 `copyPalettes` u16 → u32: 32비트 선택 마스크에서 스프라이트 팔레트 쪽이 잘리던 회귀 수정)
- HNS 적응: 그대로 적용
- 저장·ROM·그래픽 영향: ROM 크기 변화 없음
- 검증: `git diff --check` 통과, `make hns -j8` 종료 코드 0, ROM 32,715,812 B / EWRAM 248,892 B / IWRAM 25,516 B, 새 경고 없음. 자동 테스트 해당 없음.

### unit U-fade-9407: #9707 Reset objPaletteToggle after Software Fade (순서표 seq 138)

- 현재 판정: 적용(같은 unit). **seq 138 담당은 "이미 적용"으로 처리한다.**
- 커밋: `8e6f16bf71`
- upstream 근거: `833f1de49a`
- 수정 파일: `src/palette.c`(`IsSoftwarePaletteFadeFinishing` 끝에서 `objPaletteToggle = 0`)
- HNS 적응: 그대로 적용
- 저장·ROM·그래픽 영향: ROM 크기 변화 없음
- 검증: `git diff --check` 통과, `make hns -j8` 종료 코드 0, ROM 32,715,812 B / EWRAM 248,892 B / IWRAM 25,516 B, 새 경고 없음. 자동 테스트 해당 없음.

### unit U-fade-9407: #10573 Rework simultaneous palette fade (순서표 seq 319)

- 현재 판정: 적용(같은 unit 마지막). **seq 319 담당은 "이미 적용"으로 처리한다.**
- 커밋: `f76c7bf7ed`
- upstream 근거: `4a39c64fe4`
- 수정 파일: `include/palette.h`(`PaletteFadeControl.simultaneousFade:1`, padding 15 → 14), `src/field_weather.c`(`FadeSelectedPals`의 페이드아웃에서 `simultaneousFade = TRUE`), `src/palette.c`(`UpdateNormalPaletteFade`를 `_Alternate`/`_Simultaneous`로 나눔)
- HNS 적응: 그대로 적용(오프셋만). 적용 뒤 `palette.c`의 페이드 함수들(`UpdateTimeOfDayPaletteFade`, `UpdateNormalPaletteFade*`, `IsSoftwarePaletteFadeFinishing`)은 upstream 1.17.0과 같다. 남은 차이는 다른 PR 몫(#2309 DMA 매크로, HnS가 쓰는 `TimeBlendPalette`·`TintPalette_RGB_Copy`, 공백 줄)이다.
- **unit 최종 동작(= 1.17.0):** 일반 소프트웨어 페이드(메뉴·배틀 전환 등)는 이식 전 HnS와 같은 배경/스프라이트 번갈아 방식. 필드 `FadeScreen`/`FadeSelectedPals`의 페이드아웃(검정·흰색)은 동시 방식. 자연광 맵 페이드인에 쓰는 시간대 페이드(`UpdateTimeOfDayPaletteFade`, `OW_ENABLE_DNS = TRUE`)는 #9407부터 2프레임마다 배경·스프라이트를 함께 블렌드한다(같은 속도).
- 저장·ROM·그래픽 영향: ROM +240 B(unit 전체 +144 B: 32,715,908 → 32,716,052). `gPaletteFade` 비트필드 1개 추가(크기 불변). 세이브 무관.
- 검증: `git diff --check` 통과, `make hns -j8` 종료 코드 0, ROM 32,716,052 B / EWRAM 248,892 B / IWRAM 25,516 B, 새 경고 없음. 자동 테스트 해당 없음.
- 실기 확인(unit 전체): **필요.** 맵 이동(문·계단·워프) 페이드아웃/인, 비·안개 등 날씨 맵과 자연광 맵의 시간대별 페이드인, 동반 포켓몬·NPC 스프라이트가 배경과 같은 속도로 페이드되는지(스프라이트만 늦게/먼저 바뀌는 깜빡임 없음), 블렌드 면역 스프라이트(조명 등) 색, 메뉴 열고 닫기·배틀 진입/종료 페이드.
- 남은 위험: 낮음(1.17.0 최종형과 같음). 시간대 페이드 경로는 HnS DNS 설정에서 실기로만 확인 가능.

## 동기화 단위: seq 71 #9417 `U-9417` Minor dancer clean up/consolidation

- 현재 판정: 적용(HnS 적응)
- 커밋: `ce841b0cb7`
- upstream 근거: `138a8f90c6`, 1.17.0 `TryDancer`(대상 저장 필드 대조)
- 해결한 의존성: #9446(seq 72)의 선행
- 수정 파일: `include/battle.h`(`SpecialStatus`), `include/battle_util.h`(`ABILITYEFFECT_MOVE_END_OTHER` → `ABILITYEFFECT_DANCER`), `src/battle_move_resolution.c`(`CancelerMoveFailure` 2곳, `MoveEndClearBits`, `MoveEndDancer`), `src/battle_script_commands.c`(`BS_TryInstruct`), `src/battle_util.c`(`TryDancer` 신설, `AbilityBattleEffects`의 춤추기 case 교체)
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - 4개 파일이 HnS 문맥 차이로 패치가 실패해 의미 단위로 옮겼다.
  - HnS에 이미 있는 #9515(`gBattleStruct->dancerSavedTarget`/`dancerSavedAttacker`)를 `TryDancer`에서 그대로 쓴다(upstream #9417 당시의 `gBattleScripting.savedBattler` 인코딩은 쓰지 않음, 1.17.0 `TryDancer`와 같은 필드). 춤추기 순서 config `B_DANCER_ORDER`는 후속 PR 몫이라 넣지 않았다(기존 "가장 느린 배틀러부터" 유지).
  - `SpecialStatus`: upstream처럼 `changedStatsBattlerId:3`과 같은 바이트에 `neutralizingGasRemoved`·`berryReduced`·`mindBlownRecoil`을 모으고, HnS가 #10047 이식으로 가진 `poisonPuppeteer:1`도 이 바이트로 옮겼다(`padding:1`). `instructedChosenTarget`/`dancerOriginalTarget`(`| 0x4` 인코딩) → `backUpTarget`(`+ 1` 인코딩). `changedStatsBattlerId`에는 배틀러 번호(0~3)만 들어간다.
  - 메시지·스크립트 변화 없음(`BattleScript_DancerActivates` 그대로).
  - **동작 차이(upstream 유래):** 춤추기 발동 시 `gLastUsedAbility = ABILITY_DANCER`와 `RecordAbilityBattle`이 추가된다(AI 특성 기록).
- 저장·ROM·그래픽 영향: ROM +240 B, EWRAM −16 B(`gSpecialStatuses` 배틀러당 1바이트 감소). 세이브 무관.
- 검증:
  - 옛 필드·enum 사용처 0건(`instructedChosenTarget`, `dancerOriginalTarget`, `ABILITYEFFECT_MOVE_END_OTHER`)
  - `git diff --check`: 통과
  - `make hns -j8`: 종료 코드 0, ROM 32,716,292 B / EWRAM 248,876 B / IWRAM 25,516 B, 새 경고 없음
  - 자동 테스트(기준 대비 변화 없음): `ability/dancer.c` 35건(PASS 22, FAIL 13 = `Unmatched MESSAGE` 12 + 확률 테스트 1, 모두 기준과 같음), `move_effect/instruct.c` 20건(PASS 15, FAIL 4 `Unmatched MESSAGE`, TO_DO 1), `first_turn_only.c` TO_DO 4, `mat_block.c` TO_DO 1.
  - 실기 확인: 선택(더블배틀 춤추기 연쇄, 지시(Instruct) 뒤 대상 복원, 속이다·마룻바닥세워막기 첫 턴 판정)
- 남은 위험: 낮음

## 동기화 단위: seq 72 #9446 `U-synchronize-9446` Refactor synchronize and cure berry timing

- 현재 판정: 적용(HnS 적응)
- 커밋: `c1afbb563d`
- upstream 근거: `0c20d91508`
- 해결한 의존성: #9176(seq 59), #9417(seq 71) 뒤. #9532(UNUSED_33)·#9249(`B_SCR_OP_TRY_CONFUSION_AFTER_SKY_DROP`)의 opcode 번호 전제.
- 수정 파일: `asm/macros/battle_script.inc`, `data/battle_scripts_1.s`, `include/battle.h`, `include/battle_move_resolution.h`, `include/battle_util.h`, `include/constants/battle_move_resolution.h`, `include/constants/battle_script_commands.h`, `src/battle_move_resolution.c`, `src/battle_script_commands.c`, `src/battle_util.c`, `test/battle/ability/synchronize.c`, `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - 헤더 6개와 `battle_move_resolution.c`는 그대로 적용(오프셋만). `battle_scripts_1.s`·`battle_script_commands.c`·`battle_util.c`·테스트는 HnS 문맥 차이로 의미 단위 수동 이식.
  - `movevaluescleanup`(스크립트 3곳: 티타임·플라워가드·중력)·`setmultihit`·`decrementmultihit`와 `MoveValuesCleanUp()`을 없애고, opcode 자리는 upstream처럼 끝의 `B_SCR_OP_UNUSED_31/32`로 채워 이후 PR과 번호를 맞췄다. `B_SCR_OP_TRY_SYNCHRONIZE`는 `TRYOVERWRITEABILITY` 뒤. HnS `include/constants/battle_script_commands.h`는 upstream 부모와 같아 결과도 upstream과 같다.
  - `SetNonVolatileStatus`에 `battlerAtk` 인자를 추가하고 호출처 7곳을 upstream대로 바꿨다. 끝에서 `TrySynchronizeActivation()`이 상태를 받은 쪽의 싱크로를 예약하고, `BattleScript_UpdateEffectStatusIconRet`의 `trysynchronize`가 발동, 이어서 `tryactivateitem BS_EFFECT_BATTLER, ACTIVATION_ON_STATUS_CHANGE`가 상태 치료 열매를 확인한다. `Cmd_tryactivateitem`은 발동해도 다음 명령으로 진행한다(upstream).
  - 삭제한 HnS `ABILITYEFFECT_(ATK_)SYNCHRONIZE`에는 #9828(2026-09-26 선별 이식)의 `gEffectBattler == gBattlerTarget/Attacker` 조건이 있었다. 새 `TrySynchronizeActivation`의 `battlerAtk == effectBattler` 반환과 "상태를 받은 쪽 특성이 싱크로" 검사가 같은 경우를 막는다(1.17.0도 같은 구조). #9828 테스트 "Synchronize does not trigger when holder inflicts status with its own move"가 계속 PASS.
  - HnS `B_MSG_STATUSED_BY_ABILITY` 선택(특성으로 건 상태 문구), `poisonPuppeteer` 표시, 필드 싱크로(`ow_abilities.c` `IsSynchronizeActive`)는 그대로. 문자열·STRINGID 변화 없음.
  - **출력 순서 변화(upstream 유래):** 싱크로 팝업·되돌린 상태 문구와 상태 치료 열매 발동이 move end에서 상태 문구 직후로 앞당겨진다. 광역기는 대상마다 싱크로가 반응한다. `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md` "특성·도구·도주" 표에 1행 추가(plan 지시).
- 저장·ROM·그래픽 영향: ROM −240 B(32,716,292 → 32,716,052). `BattleStruct`에서 `u16 synchronizeMoveEffect`가 빠지고 3비트 `synchronizeState`가 기존 `unused:3` 자리에 들어갔다(힙). 세이브 무관.
- 검증:
  - 옛 심볼 사용처 0건(`synchronizeMoveEffect`, `MoveValuesCleanUp`, `movevaluescleanup`, `setmultihit`, `decrementmultihit`, `ABILITYEFFECT_(ATK_)SYNCHRONIZE`, `MOVEEND_SYNCHRONIZE_*`)
  - `git diff --check`: 통과. 비ASCII 소스 줄 변경 0건.
  - `make hns -j8`: 종료 코드 0, ROM 32,716,052 B / EWRAM 248,876 B / IWRAM 25,516 B, 새 경고 없음
  - 자동 테스트(기준 대비 PASS→FAIL 0): `ability/synchronize.c` 9건 PASS 8(새 테스트 2개 "Synchronize will trigger on both targets", "…can trigger again during the same attack if user cured it's status" PASS 포함), FAIL 1(기준에서도 FAIL인 "…Toxic Orb or Flame Orb 2/2", `Task_FreeAbilityPopUpGfx` task not freed). `hold_effect/cure_status.c` 14건(PASS 2, FAIL 12), `move_effect/teatime.c` 12건 FAIL(`Unmatched MESSAGE`), `flower_shield.c`(PASS 3, FAIL 1), `gravity.c`(PASS 1, FAIL 1, TO_DO 3), `psycho_shift.c` TO_DO 1, `ability/poison_touch.c`·`static.c`·`flame_body.c`·`effect_spore.c`·`poison_point.c`(FAIL은 `Unmatched MESSAGE`와 기존 확률 테스트) — 모두 기준과 같은 상태.
  - 실기 확인: **필요.** 싱크로(독·마비·화상) 되돌리기와 팝업 순서, 광역 독 공격에 두 싱크로 포켓몬, 리샘열매·복숭열매 등 상태 치료 열매 발동 시점과 한글 문구·아이템 팝업, 독수(Poison Touch)+싱크로+리샘열매 조합, 티타임·플라워가드·중력.
- 남은 위험: 중간(배틀 스크립트 흐름 변경). 자동 테스트로 핵심 경로는 확인.

## 동기화 단위: seq 73 #9463 `U-cstring-9086` Use PRIu64 to print appropriate variable length in preproc

- 현재 판정: 적용
- 커밋: `db75d9c0e1`
- upstream 근거: `80933058c2`
- 해결한 의존성: #9086(seq 65) 뒤
- 수정 파일: `tools/preproc/c_file.cpp`(해시 출력 `%016lx` → `%016" PRIx64`), `tools/preproc/c_file.h`(`inttypes.h`)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. 도구 이식성 수정이라 LP64(WSL)에서는 출력이 같다. 이전/이후 preproc로 `src/challenge_menu.c`·`src/item.c`·`src/battle_message.c`·`src/strings.c`·`test/battle/ability/synchronize.c`를 전처리한 출력이 **바이트 동일**함을 확인했다(한글 문자열 포함).
- 저장·ROM·그래픽 영향: 없음
- 검증:
  - `git diff --check`: 통과, `tools/preproc` 재빌드 성공(`-Werror`)
  - `make hns -j8`: 종료 코드 0, ROM 32,716,052 B / EWRAM 248,876 B / IWRAM 25,516 B, 새 경고 없음
  - 자동 테스트: 해당 없음
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 74 #8664 `U-8664` Smarter Doubles Fake Out AI + EFFECT_FIRST_TURN_ONLY tests

- 현재 판정: 적용(`include/random.h` hunk 제외)
- 커밋: `f7a32cc1ff`
- upstream 근거: `62b91cf57d`
- 수정 파일: `include/config/ai.h`(`FAKE_OUT_SAVE_ALLY_CHANCE 50`), `src/battle_ai_main.c`(`AI_CalcMoveEffectScore`: 확정 풀죽음 가산을 싱글 한정, `EFFECT_FIRST_TURN_ONLY` 더블 분기), `test/battle/move_effect/first_turn_only.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - `RNG_AI_FAKE_OUT_SAVE_ALLY`는 HnS `include/random.h:256`에 이미 있어 그 hunk를 뺐다(group plan).
  - upstream 추가 줄의 후행 공백 8줄과 테스트 파일 끝 빈 줄은 `git diff --check` 통과를 위해 지웠다(내용 동일).
  - `predictedMoveSpeedCheck`는 HnS `AI_CalcMoveEffectScore`에 이미 있다(#9857 개명 전 이름, upstream 당시와 같음).
  - **AI 동작 변화(upstream 유래):** 더블배틀에서 속이다 점수가 상대·아군의 확정 KO/속도 관계로 달라진다(난이도 소폭 상승 가능).
- 저장·ROM·그래픽 영향: ROM +704 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j8`: 종료 코드 0, ROM 32,716,756 B / EWRAM 248,876 B / IWRAM 25,516 B, 새 경고 없음
  - 자동 테스트: `test/battle/move_effect/first_turn_only.c` 9건 — 새 AI 테스트 5개 PASS. 기준에서 TO_DO였던 4개("Fake Out/First Impression can only be used on the user's first turn", "… fails if it's called via Instruct")가 upstream에서 실제 테스트로 바뀌어 FAIL, 사유는 모두 `Unmatched MESSAGE`(영문 기대값, 알려진 한계). 통과하던 테스트의 회귀 없음.
  - 실기 확인: 선택(더블배틀 AI 속이다 사용)
- 남은 위험: 낮음

## 동기화 단위: seq 75 #9142 `U-animcall-9142` Create functions for repeated move animations (+ seq 78 #9473, seq 98 #9564)

- 현재 판정: 적용(unit 3개 PR을 한 커밋에)
- 커밋: `125e893903`
- upstream 근거: #9142 `6d1fec9df3`, #9473 `913aaae7e7`, #9564 `070f31e384`
- 해결한 의존성: group plan "같은 unit의 #9473(호출 깊이)·#9564(Stuff Cheeks 좌표)를 반드시 함께 넣는다". #9142는 서브루틴 안에서 다시 `call`하는 중첩 호출을 만드는데, 이식 전 HnS `sBattleAnimScriptRetAddr`는 반환 주소 1개뿐이라 #9142만 넣으면 중첩 호출 뒤 반환 위치가 깨진다. 그래서 #9473까지 한 커밋으로 넣었고, #9142가 `BiteOpponent`의 이빨 x 좌표를 `-33`(0xffDF) → `33`으로 잘못 바꾼 것을 되돌리는 #9564도 함께 넣었다. **seq 78 #9473과 seq 98 #9564는 "이미 적용"으로 처리한다.** #8497(seq 83)의 선행 조건(#9142·#9473)이 충족됐다.
- 수정 파일: `asm/macros/battle_anim_script.inc`(`create_magic_powder_particle_sprite`), `data/battle_anim_scripts.s`(반복 시퀀스를 서브루틴 `call`로, DefendOrder·SaltCure 꼬리를 `goto`로, BiteOpponent x=-33), `include/battle_anim.h`(`MAX_ANIM_CALL_DEPTH 4`), `src/battle_anim.c`(반환 주소 스택, `Cmd_call`/`Cmd_return`/`Cmd_end` assert)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 세 PR 모두 그대로 적용(오프셋만, HnS 애니 스크립트와 충돌 없음). 문자열 무관.
- **호출 구조 정적 검사:** HnS `data/battle_anim_scripts.s` 전체를 파싱해 `gBattleAnim*` 진입점 1,022개에서 가능한 모든 흐름(`goto`, `jump*`·`choosetwoturnanim` 양쪽 분기, `.if` 양쪽, 중첩 `call`/`return`)을 따라갔다(스크래치 `anim/animcalls.py`).
  - 이식 전: `call` 2,678개, 최대 깊이 1, "호출 안에서 `end` 도달" 2곳(DefendOrder → BideSetUp, SaltCure → SaltCureDamage = #9473이 고친 곳).
  - 이식 후: `call` 2,725개, **최대 깊이 2**(assert 한도 3 이내), 호출 안 `end` 도달 **0**, 빈 스택 `return` **0**. 새 assert(`Call depth not 0 at end`, `Max animation call depth exceeded`, `return with empty call stack`)에 걸리는 정적 경로 없음.
  - 동적 전수 검사(`test/battle/move_animations/all_anims.c`)는 `T_SHOULD_RUN_MOVE_ANIM = FALSE`(매우 무거움)라 돌리지 않았다. 구간 끝 전체 테스트에서 일반 배틀 테스트의 기술 애니 재생은 확인한다.
- 저장·ROM·그래픽 영향: ROM −1,408 B(32,716,756 → 32,714,644), EWRAM +16 B(반환 주소 배열·깊이 카운터). 애니 연출은 같다(서브루틴화, Bite 좌표 원복).
- 검증:
  - `git diff --check`: 통과
  - `make hns -j8`: 종료 코드 0, ROM 32,714,644 B / EWRAM 248,892 B / IWRAM 25,516 B, 새 경고 없음
  - 자동 테스트: 해당 전용 테스트 없음(위 참고)
  - 실기 확인: **권장.** HnS는 비릴리스 빌드라 assert 실패 시 크래시 화면이 뜬다. 서브루틴으로 바뀐 애니 표본(물기·깨물어부수기·사이코팽 계열, 흡수 계열, 방어지령·소금절이, 볼 부풀리기, 매직파우더)과 HnS 추가 기술 애니를 재생해 크래시 화면·연출 차이가 없는지.
- 남은 위험: 낮음. 호출 깊이 카운터는 애니 시작 때 초기화되지 않는다(upstream 1.17.0도 같음). 정적 검사상 깊이가 0이 아닌 채 끝나는 경로는 없다.

## 동기화 단위: seq 76 #9474 `U-heap-9121` Fix Substitute breaking when used by opponentRight in double battles

- 현재 판정: **이미 적용**(seq 64 커밋 `2ba44de454`에 #9121과 함께 포함, group plan의 "반드시 같은 커밋")
- upstream 근거: `4ce8738dae`
- 근거: `src/battle_gfx_sfx_util.c` `BattleLoadSubstituteOrMonSpriteGfx`의 복사 루프가 `for (u32 i = 1; i < 2; i++)`이고 `s32 i` 선언이 없다(upstream 결과와 같음).
- 커밋 없음

## 동기화 단위: seq 77 #9451 `U-9451` Allow both AI opponents in doubles to switch out on the same turn

- 현재 판정: 적용(HnS 적응)
- 커밋: `1c2ca8ec50`
- upstream 근거: `f32f3a8415`
- 수정 파일: `include/battle_ai_util.h`, `src/battle_ai_util.c`(`IsPartyMonPlannedToBeSwitchedInByPartner`), `src/battle_ai_main.c`(`AI_TrySwitchOrUseItem`), `src/battle_ai_switch.c`(교체 후보 루프 9곳, `FindMonWithMoveOfEffectiveness`·`ShouldSwitchIfAllMovesBad`·`ShouldSwitchIfWonderGuard`에 `battlerIn1/2`, `GetNextMonInParty`에 `battler`), `test/battle/ai/ai_choice.c`, `test/battle/ai/ai_switching.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - `GetBestMonIntegrated`·`GetBestMonVanilla` 두 hunk는 HnS의 3인자 `InitializeSwitchinCandidate(battler, monIndex, &party[monIndex])` 문맥 때문에 수동 병합(group plan). 나머지 hunk는 그대로 적용. 파트너 계획 검사 호출 수가 upstream 결과와 같다(10곳).
  - 새 테스트 "AI can switch out both mons on the same turn in double battles"는 HnS `ai_switching.c` 끝에 붙였다(HnS에 뒤쪽 테스트가 더 있어 패치 위치가 다름).
  - **AI 동작 변화(upstream 유래):** 더블배틀에서 두 AI가 같은 교체 후보를 고르지 않아 같은 턴에 둘 다 교체할 수 있다(교체하는 모든 AI 트레이너).
- 저장·ROM·그래픽 영향: ROM +96 B
- 검증:
  - `git diff --check`: 통과
  - `make hns -j8`: 종료 코드 0, ROM 32,714,740 B / EWRAM 248,892 B / IWRAM 25,516 B, 새 경고 없음
  - 자동 테스트:
    - `ai/ai_switching.c` 119건: PASS 101, FAIL 16(기준에서도 FAIL인 `Expected 1.0 passes` 11건·`Unmatched MESSAGE` 4건 + 새 테스트 1건), KNOWN_FAILING 1, ASSUMPTION_FAIL 1(기준 목록 형식 밖, 기존과 같음). 새 테스트 "AI can switch out both mons on the same turn in double battles"가 FAIL.
    - `ai/ai_choice.c` 10건: PASS 9, FAIL 1 — **"Choiced Pokémon won't switch out if they can still affect one opposing Pokémon in doubles 1/2"(기준 PASS)가 FAIL.** 이 테스트는 #9451이 상대를 4마리 → 3마리로 줄이고 기대값을 `EXPECT_SWITCH(opponentLeft, 3)` → `2`로 바꾼 것이라 이전과 다른 시나리오다.
    - **원인 분류(코드 회귀 아님):** HnS에는 이미 더블배틀 AI 판단 순서를 50% 확률로 뒤집는 `AI_REVERSE_BATTLER_LOGIC_ORDER_CHANCE 50`(HnS 기반 커밋 `1821fd6749`)이 있다. 두 테스트는 "왼쪽 AI가 먼저 판단"을 전제하는데 테스트 RNG에서 순서가 뒤집혀 오른쪽 AI가 먼저 후보를 가져간다. 확인을 위해 `include/config/ai.h`의 값을 임시로 0으로 바꿔(커밋 안 함, 되돌림) 돌리자 **`ai_choice.c` 10건 전부 PASS, `ai_switching.c` 새 테스트 PASS**(그 밖의 상태는 기준과 같거나 1건 더 PASS). 뒤집힌 순서에서의 결과(오른쪽이 교체, 왼쪽은 공격)는 upstream 1.17.0의 "(reversed)" 테스트 기대값과 같다.
    - 해소 예정: upstream은 #9460(seq 88)·#9462(seq 131)에서 `WITH_CONFIG(AI_REVERSE_BATTLER_LOGIC_ORDER_CHANCE, 0/100)`와 "(reversed)" 테스트를 넣는다. 그 이식 때 두 테스트가 PASS로 돌아와야 한다(**seq 88/131 담당 확인 필요**).
  - 실기 확인: 선택(더블배틀 AI 트레이너의 동시 교체)
- 남은 위험: 낮음(테스트 전제 차이만)

## 동기화 단위: seq 78 #9473 `U-animcall-9142` Move animation call depth increase

- 현재 판정: **이미 적용**(seq 75 커밋 `125e893903`에 #9142와 함께 포함, group plan "반드시 함께")
- upstream 근거: `913aaae7e7`
- 근거: `include/battle_anim.h` `MAX_ANIM_CALL_DEPTH 4`, `src/battle_anim.c` `sBattleAnimScriptRetAddr[MAX_ANIM_CALL_DEPTH]`·`sBattleAnimScriptCallDepth`, DefendOrder·SaltCure의 `goto`가 upstream 결과와 같다.
- 커밋 없음

## 동기화 단위: seq 79 #9467 `U-9467` Change `TrainersMon`'s ball from `u8` to `enum PokeBall`

- 현재 판정: 적용
- 커밋: `d85cf78e9a`
- upstream 근거: `a338550479`
- 수정 파일: `include/data.h`(`#include "constants/pokeball.h"`, `enum PokeBall ball:8`), `src/battle_partner.c`(비트필드 주소를 넘기지 않도록 지역 변수로 복사)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용(오프셋만). HnS의 다른 사용처(`battle_main.c` 트레이너 파티 생성은 이미 지역 변수로 복사, 테스트 `trainer_control.h`의 지정 초기화)는 수정 불필요.
- 저장·ROM·그래픽 영향: 없음. 실제 컴파일러로 이전/이후 `struct TrainerMon`을 평가해 크기 36, `lvl` 오프셋 26, `friendship` 오프셋 28로 같음을 확인. ROM 크기 동일.
- 검증:
  - `git diff --check`: 통과
  - `make hns -j8`: 종료 코드 0, ROM 32,714,740 B / EWRAM 248,892 B / IWRAM 25,516 B, 새 경고 없음
  - 자동 테스트: `test/battle/trainer_control.c` 20건 — PASS 19, FAIL 1(`EXPECT failed`, 기준에서도 FAIL). 기준과 같음.
  - 실기 확인: 불필요
- 남은 위험: 없음

## 동기화 단위: seq 80 #9249 `U-skydrop-9249` Refactor Sky Drop and rampage confusion

- 현재 판정: 적용(HnS 적응)
- 커밋: `399dc07d5b`(코드), `b836436e60`(`BATTLE_MESSAGE_OUTPUT_CHANGES.md` 기록)
- upstream 근거: `c114dfbc84`, 1.17.0 대조(`EFFECT_SMACK_DOWN`, 독조종(Poison Puppeteer), `CanBeConfused`)
- 해결한 의존성: #9446(seq 72), #9358(seq 55) 뒤. 같은 unit의 회귀 수정 #10648(seq 330)을 바로 뒤 커밋으로 넣는다(아래).
- 수정 파일(21): `asm/macros/battle_script.inc`, `data/battle_scripts_1.s`, `include/battle.h`, `include/battle_main.h`, `include/battle_scripts.h`, `include/battle_util.h`, `include/config/battle.h`, `include/constants/battle.h`, `include/constants/battle_move_resolution.h`, `include/constants/battle_script_commands.h`, `include/constants/generational_changes.h`, `src/battle_ai_items.c`, `src/battle_ai_util.c`, `src/battle_anim_effects_1.c`, `src/battle_end_turn.c`, `src/battle_main.c`, `src/battle_move_resolution.c`, `src/battle_script_commands.c`, `src/battle_util.c`, `test/battle/move_effect/sky_drop.c`, `test/battle/move_effect_secondary/thrash.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - 17개 파일은 그대로 적용, 거부된 hunk 7개(스크립트 2, `battle_main.h` 1, `battle_script_commands.c` 1, `battle_util.c` 3)는 수동 병합했다. 중력 스크립트는 HnS의 `VOLATILE_MAGNET_RISE_TIMER` 문맥, 기절 스크립트는 HnS의 `call BattleScript_TryRevertWeatherform` 문맥에 맞췄다.
  - `gBattleStruct->skyDropTargets[]`·`enum SkyDropState`·`CheckSkyDropState`·`BS_SkyDropYawn`·`skydropyawn`·`MOVEEND_SKY_DROP_CONFUSE`·`STATE_SKY_DROP`를 없애고 `VOLATILE_SKY_DROP_TARGET`(`skyDropTarget`, +1 인코딩)·`VOLATILE_CONFUSE_AFTER_DROP`, `STATE_SKY_DROP_ATTACKER`/`STATE_SKY_DROP_TARGET`, `MOVEEND_RAMPAGE`·`MOVEEND_CONFUSION_AFTER_SKY_DROP`, `tryconfusionafterskydrop`(opcode `B_SCR_OP_TRY_CONFUSION_AFTER_SKY_DROP`, `UNUSED_32` 자리 사용)로 바꿨다. `CancelMultiTurnMoves(battler)`, `CanBeConfused(atk, effect)`, `void FaintClearSetData`.
  - config: HnS `B_RAMPAGE_CANCELLING GEN_LATEST` → `B_RAMPAGE_CONFUSION GEN_LATEST`(이름만, 값 유지, plan).
  - upstream #9249 뒤에 만들어져 HnS에 먼저 들어와 있던 코드 3곳을 새 상태로 맞췄다(1.17.0 형태와 같음): #10213(2026-09-26 선별 이식)의 떨어뜨리기(`EFFECT_SMACK_DOWN`) move end 검사 → `!= STATE_SKY_DROP_ATTACKER && != STATE_SKY_DROP_TARGET`, #10047 형태의 독조종 혼란 → `CanBeConfused(gBattlerAttacker, gBattlerTarget)`, `BattleCalcValues`를 쓰는 HnS `CanMoveSkipAccuracyCalc`의 노가드 검사 → `IsSkyDropInvolved()`.
  - HnS 텔레포트 도주의 `FaintClearSetData(battler)` 호출은 반환값을 쓰지 않아 그대로 둔다. HnS `CanSetNonVolatileStatus` 수정(마그마의무장 등)은 이 PR과 무관해 불변.
  - **출력 시점 변화(upstream 유래):** Gen5+ 난동 종료 혼란("지쳐서 혼란에 빠졌다", `STRINGID_PKMNFATIGUECONFUSION`)이 턴 종료에서 기술 직후로, 프리폴 해제 혼란은 프리폴 공격 뒤 또는 프리폴 사용자 기절 시로 모였다. 신비의부적이 있으면 자기 편 혼란도 막는다(`CanBeConfused`에 신비의부적 검사). `BATTLE_MESSAGE_OUTPUT_CHANGES.md` "기술·필드 상태 효과" 표에 1행 추가(`b836436e60`).
  - upstream이 넣은 공백만 있는 줄 1개(`battle_script.inc`)는 `git diff --check` 통과를 위해 비웠다.
- 저장·ROM·그래픽 영향: ROM +2,400 B(32,714,740 → 32,717,140), EWRAM +16 B(`Volatiles` 필드 추가, `BattleStruct.skyDropTargets[4]` 삭제). 세이브 무관(배틀 중 데이터).
- 검증:
  - 옛 심볼 사용처 0건(`skyDropTargets`, `SKY_DROP_NO_TARGET`, `SkyDropState`, `STATE_SKY_DROP`, `B_RAMPAGE_CANCELLING`, `ThrashConfusesRet`, `CheckSkyDropState`, `skydropyawn`)
  - `git diff --check`: 통과. 비ASCII 소스 줄 변경 0건.
  - `make hns -j8`: 종료 코드 0, ROM 32,717,140 B / EWRAM 248,908 B / IWRAM 25,516 B, 새 경고 없음
  - 자동 테스트(통과하던 테스트의 회귀 0):
    - `move_effect/sky_drop.c` 16건: PASS 12(새 테스트 3개 PASS: 비행 타입도 난동 후 떨어지면 혼란, 사용자가 상태이상·상대 특성으로 기절했을 때 즉시 혼란), FAIL 4(모두 `Unmatched MESSAGE`, 새 테스트 "…confusion occurs immediately" 포함).
    - `move_effect_secondary/thrash.c` 8건 전부 PASS(새 테스트 "Thrash confuses the user after it finishes even if move failed" 포함).
    - `gravity.c`, `uproar.c`, `ally_switch.c`, `ability/own_tempo.c`, `ability/dancer.c`, `move_effect/instruct.c`, `sleep_clause.c`, `ability/parental_bond.c`, `move_effect/me_first.c`, `ability/infiltrator.c`: 기준과 같은 상태(FAIL은 `Unmatched MESSAGE`·기존 확률 테스트).
  - 실기 확인: **필요.** 역린·난동부리기·꽃잎댄스 종료 혼란 문구가 기술 직후 나오는지, 프리폴로 난동 중인 포켓몬을 잡았다 놓을 때(일반·비행 타입·사용자 기절·중력), 신비의부적 아래 난동 종료, 떨어뜨리기로 공중/프리폴 상태 대상, 노가드+프리폴, 하품으로 잠든 프리폴 대상.
- 남은 위험: 중간(배틀 흐름 리팩터). 참고: #10180(이미 이식, `f2d3008825`)에서 #9249 부재로 뺐던 `IsBattlerInvolvedInSkyDrop()`(탈출팩·탈출버튼이 프리폴 중에는 발동하지 않음)은 이제 전제가 충족됐다. HnS `TrySwitchInEjectPack`에는 아직 이 검사가 없다 — #9784(seq 166, `TryEjectPack`/`TryEjectButton` 신설) 이식 때 1.17.0 형태로 함께 넣을 것.
