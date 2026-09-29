# full-sync 실제 port 결과 — seq 63~82

진행 중: 마지막 완료 seq 67 (#9410), 다음 seq 68 (#9388). seq 76 #9474는 seq 64 커밋에 포함(이미 적용).

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
