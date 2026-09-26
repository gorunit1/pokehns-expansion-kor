# g5_general_cleanup_docs 인벤토리 판정 보고서

- 대상: `g5_general_cleanup_docs.tsv`의 PR 151개. General, Other Cleanup, Documentation, Branch Synchronisation 항목이다.
- 기준: HNS `pokehns-expansion-kor` HEAD `0d89762071`. 검사는 스냅샷 `<HnS 작업트리>/`에서 `git apply --check`로만 했다.
- 기반 확인: HNS는 upstream master 계열로, merge-base `3efb836f72`는 1.15.2 개발 중이다. upcoming 전용 리팩터(#8943 `gParties`/12v12, #9529 `config_changes`, #9920 daily seed, 9881 INCGFX 등)는 HNS에 없다.
- 방법
  - merge 커밋으로 들어온 PR(9724, 9881, 9986 등)은 `커밋^1..커밋` diff로 판정했다.
  - 파일별로 `--include`를 걸어 정방향과 역방향 `--check`를 따로 돌렸다.
  - 실패한 hunk는 HNS 코드를 직접 읽고 대조했다.
  - ROM 수치는 `pokehns.elf`의 `arm-none-eabi-nm -S` 결과로 추정했다. 읽기만 했다.
- 판정 기준(코디네이터 지시 반영)
  - upcoming 리팩터가 만든 회귀를 고친 PR은 `HNS와 무관`으로 분류했다.
  - 표기만 다른 PR은 `미적용·안전 이식 가능`으로 분류하고, 비고에 "수동"이라고 적었다.
  - 리팩터가 먼저 있어야 하는 PR은 `미적용·선행 필요`로 분류했다.
  - 동작 변화가 없는 순수 리팩터는 `HNS와 무관(비기능)`으로 분류하고 이식 불필요로 봤다.

## 요약

### 판정별 개수

| 판정 | 개수 |
|---|---|
| 이미 적용 | 14 |
| 부분 적용 | 2 |
| 미적용·안전 이식 가능 | 43 (그중 `--check` 실패로 수동 적응 필요 17: 9231, 9420, 9542, 9562, 8213, 9634, 9911, 9906, 9608, 9835, 9892, 10194, 10656, 10152, 10166, 10179, 10659) |
| 미적용·선행 필요 | 15 |
| 충돌 | 9 |
| HNS와 무관 | 68 (문서·CI·도구 약 25, 비기능 리팩터 약 25, FRLG·비활성 config·upcoming 회귀 수정 등) |

### 이식 추천 상위 항목

1. **#9973 + #9813 + #10349 + #10392 `FillSpriteRect` 수정 묶음**
   - HNS 체력바, 사파리 볼 수, 상대 HP% 표시가 `FillSpriteRectColor`를 쓴다.
   - 네 패치는 한꺼번에 `--check`를 통과한다.
2. **#9882(battle_interface.c 폭만) + #9912 사파리 볼 수 영역 지우기 폭 31→40**
   - 위치는 `src/battle_interface.c:2190`이다.
   - 비-Gen4 UI 경로만 해당한다. Gen4 UI는 `gSaveblock3.challengeSettings.newBattleUI` 옵션이다.
3. **#10015 첫 전투 핸들러 버그 수정 (1줄)**
   - 첫 전투에서 기술 순서를 바꾸면 비FRLG에서도 OakOldMan 핸들러로 바뀌는 버그다.
   - 위치는 `src/battle_controller_player.c:1091·1111`이다.
4. **#9963 더블 트레이너 리매치가 싱글로 시작되는 문제**
   - 위치는 `BattleSetup_StartRematchBattle`이다.
5. **#9231 + #9207 RAM 절감**
   - EWRAM 약 236B, IWRAM 약 100B를 줄인다. 현재 EWRAM 사용률은 95%다.
   - #9231의 `link_rfu_2.c`는 `EWRAM_DATA` 컨텍스트 차이가 있어 수동으로 적용해야 한다.
6. **#9086 COMPOUND_STRING 공유(preproc), #9463은 수동**
   - ROM 감소 추정치는 3~4KB 이상이다. 한 줄 리터럴 중복 243건만 센 값이다.
   - 이식하려면 HNS 전용 `src/challenge_menu.c:507`을 `_()`로 바꿔야 한다. 그대로 두면 빌드가 실패한다.
7. **#10179 미사용 함수 제거 (수동, 34파일)**
   - 삭제 대상 145개 함수(11.5KB)가 현 ELF에 남아 있다. HNS가 쓰는 함수를 빼면 약 10KB가 줄어든다.
8. **#9906 + #10014 배틀 중 메뉴 진입 시 애니 BG 버퍼 해제**
   - 파티, 가방, 진화, 도감 화면에 들어갈 때 힙 피크가 약 12KB 줄어든다.
   - `gParties`→`gPlayerParty` 컨텍스트 차이만 수동으로 맞추면 된다.
9. **#10656 스프라이트 부족 시 `CreateSpriteUnchecked` 사용**
   - `make hns`는 비릴리스 빌드라서 `CreateSprite` assert가 크래시 화면으로 뜬다.
   - 36파일은 그대로 적용되고 9파일은 수동이다.
10. **소형 버그 수정**: #10241(`RandomWeightedIndex` 경계), #9990(팔로워 스프라이트), #9608(트레이너 백스프라이트 복사 제거), #9855, #10147.

아래 13개는 한 번의 `git apply --check`로 함께 통과한다.
9207, 9870, 9872, 9888, 9855, 9963, 10015, 9990, 10241, 10130, 10147, 9505, 10110.

### 충돌 목록 (상세는 아래 절)

| PR | 요지 | 심각도 |
|---|---|---|
| #10429 | ZWS 문자 `0x3A`가 한글 2바이트 인코딩의 선두·후속 바이트와 겹침. 적용하면 '루·룡·료…' 등이 깨짐 | 높음(이식 금지) |
| #9881 | pret#2283 INCGFX 이행이 HNS 한글 폰트 빌드 규칙이 있는 `graphics_file_rules.mk`를 삭제 | 높음(빌드) |
| #10131 | `HQ_RANDOM` 삭제. HNS 전용 `src/randomizer.c:354`가 사용 | 중간 |
| #9578 | `BattleScript_HealerActivates` 문구 변경과 한글 문자열 삭제. HNS는 이미 `STRINGID_HEALERCURE`로 출력 변경 | 중간(배틀 메시지) |
| #10314 | 특성 팝업 인쇄 방식 교체. HNS 한글 팝업(`"의"` 접미) 구현과 겹침 | 중간 |
| #8678 | 동적 trainerbattle 스크립트. HNS `battle_setup.c` 리매치·트레이너 배틀 커스텀(+468줄)과 겹침 | 중간 |
| #9051, #10335 | 한글 문자열(`strings.c`·`script_menu.h`·도감)을 다른 파일로 옮기는 비기능 리팩터 | 낮음(비기능) |
| #9713 | 디버그 사운드 메뉴. HNS `songs.h`의 DP 음악 확장과 겹침 | 낮음(디버그) |

### 용량 주의

- **ROM 증가**
  - #8678은 코드와 스크립트 매크로로 수 KB가 늘 수 있다.
  - #9641 퀵스타트는 코드와 HUD 그래픽이 추가된다.
  - #9667은 #9086 없이 적용하면 공용 기술 설명이 중복돼 ROM이 는다.
  - 이 그룹에 큰 증가 항목은 없다.
- **ROM 감소**
  - #10179 약 10KB, #9086 약 3~4KB 이상, #10152 약 0.5KB.
  - #9911 168B, #9562·#9578·#9634·#9608은 소량.
- **RAM 감소**: #9231 EWRAM 약 230B·IWRAM 약 100B, #9207 EWRAM 6B. #9906은 힙 피크만 줄인다.
- **참고(PR 범위 밖)**
  - `make hns`는 `LTO ?= 0`(Makefile:53)이다. `-ffunction-sections -fdata-sections`는 LTO일 때만 붙는다(Makefile:183).
  - 그래서 `--gc-sections`가 있어도 사용 중인 오브젝트 안의 미사용 비정적 함수는 ROM에 남는다. #10179 대상 함수가 ELF에 남아 있는 것이 그 근거다.
  - hns 빌드에 섹션 분리를 켜 보는 것이 PR 단위 정리보다 ROM을 더 많이 회수할 수 있다. 별도 검증이 필요하다.

### 세이브 호환성

- #9542는 SaveBlock1 `giftRibbons`를 11바이트에서 7바이트+`padding[4]`로 바꾼다. 크기와 오프셋이 같으므로 기존 세이브와 호환된다.
- #9066(OBJ_EVENT_GFX enum화)은 이식 비권장이다. 이식하더라도 값이 바뀌면 SaveBlock1 `objectEventTemplates`의 graphicsId가 어긋난다.
- #10050은 #9920(daily seed, 다른 그룹)을 전제로 한다. #9920이 SaveBlock 필드를 추가하는지는 그 그룹에서 확인해야 한다.
- 이 그룹에 SaveBlock 필드를 추가하거나 삭제하는 항목은 없다.

### 불확실 항목

- #9086
  - ROM 감소량은 한 줄 리터럴만 센 대략치다.
  - 한글 charmap이 preproc 변환과 함께 동작하는지는 빌드로 확인해야 한다.
- #10179: 절감량은 nm 기준 추정치다. HNS 사용 함수는 빌드 오류로 드러나므로 제외 목록을 빌드하며 확정해야 한다.
- #10404: HNS `MoveInfo.argument`는 이미 u16 기반으로 보인다. 4바이트 고정으로 ROM이 줄어드는지 불확실하다.
- #9892, #9855: 추가되는 assert가 HNS 전용 창 템플릿이나 애니 스크립트에서 걸릴 수 있다. 비릴리스 빌드에서는 크래시 화면으로 드러난다.
- #9865: Mega Sol 관련 AI·속도·날씨 치환 중 미적용분을 배틀 그룹 판정과 맞춰야 한다.
- #10166: HNS에 레벨업 기술이 40개를 넘는 종이 있는지 확인하지 않았다.

## 판정 표

### 1.15.2

| PR | 제목 | 판정 | 근거 | 고위험파일 | 용량 영향 | 의존·비고 |
|---|---|---|---|---|---|---|
| #9724 | Pret merge (8th of April, 2026) | 이미 적용 | pret 동기화 merge 커밋 e2b76f5f2d가 HNS 계보(merge-base 3efb836f72 이전)에 포함. |  | 없음 | - |
| #9881 | Merge pret#2283 | 충돌 | pret#2283 INCGFX 이행: `graphics_file_rules.mk`·`spritesheet_rules.mk`를 삭제하고 INCBIN→INCGFX로 대량 치환. HNS HEAD 0d89762071이 같은 파일(`graphics_file_rules.mk:275~`)에 한글 폰트 16개 생성 규칙을 추가했고 HNS 전용 그래픽도 INCBIN 기반(`src/data/graphics/pokemon.h` INCBIN 14,545개). |  | 없음(빌드 방식) | 9986·10036·10184·10334·10555의 선행. 충돌 절 참조 |
| #9845 | Fix fishing chain behaviour on shiny encounter | 이미 적용 | merge 커밋 072224e804가 HNS 계보에 포함. |  | 없음 | - |
| #9870 | Improve error handling of smol compressor | 미적용·안전 이식 가능 | `tools/compresSmol/compressAlgo.cpp` 오류 처리만 변경, 깨끗이 적용. |  | 없음(도구) | 빌드 도구 전용 |
| #9855 | Move animation fixes | 미적용·안전 이식 가능 | `Task_InitUpdateMonBg` assert 추가와 electric anim의 `CreateSpriteUnchecked` 전환. HNS에 `CreateSpriteUnchecked` 존재, 깨끗이 적용. |  | 없음 | 비릴리스(`make hns`) 빌드에서 monbg 중복 시 assert 화면 가능 |
| #9888 | Fix compiling with -O0 | 미적용·안전 이식 가능 | `#if TESTING` 가드와 `ow_abilities.c` const 위치 수정. 비기능, 깨끗이 적용. |  | 없음 | - |
| #9882 | Safari fixes | 부분 적용 | `data/scripts/safari_zone.inc`의 `#if IS_FRLG`는 이미 반영. `src/battle_interface.c:2190` `UpdateLeftNoOfBallsTextOnHealthbox` 비-Gen4 UI 경로의 폭 31→39만 미적용. |  | 없음 | 9912와 합쳐 폭 40으로 적용 권장 |

### 1.15.3

| PR | 제목 | 판정 | 근거 | 고위험파일 | 용량 영향 | 의존·비고 |
|---|---|---|---|---|---|---|
| #9900 | Expansion 1.15.2 release | HNS와 무관 | 1.15.2 릴리스 PR(changelog, `expansion.h` 버전 상수). |  | 없음 | 버전 상수는 최종 이식 후 별도 결정 |
| #9915 | chore: removed old changelog file | HNS와 무관 | 구 `CHANGELOG.md` 삭제(문서). |  | 없음 | - |
| #9990 | Various Follower Sprite Fixes from issue #5135 | 미적용·안전 이식 가능 | 팔로워 OW 스프라이트 PNG/팔레트 24개 수정. `OW_FOLLOWERS_ENABLED TRUE`, 깨끗이 적용. |  | 없음(동일 크기 그래픽) | - |
| #10078 | chore(credits): change kildemal to miriam in credits | HNS와 무관 | CREDITS 문서. |  | 없음 | - |
| #9872 | fix(debug_menu): fix flicker when used when map pop-up active | 미적용·안전 이식 가능 | `Debug_ShowMenu`에 `CopyWindowToVram` 1줄 추가. 깨끗이 적용. |  | 없음 | 디버그 전용 |
| #9913 | Cleanup of Dexnav checks | HNS와 무관 | DexNav null 검사. HNS `DEXNAV_ENABLED FALSE`(include/config/dexnav.h:4). 적용해도 무해. |  | 없음 | - |
| #9912 | Fix font shadow not being cleared in safari healthbox | 미적용·선행 필요 | 같은 줄의 폭 39→40. 9882의 39 변경이 전제. |  | 없음 | 9882와 함께 31→40 |
| #9963 | Fixes double battle rematches being single battles | 미적용·안전 이식 가능 | `BattleSetup_StartRematchBattle`(src/battle_setup.c:1798)에서 더블 트레이너면 `BATTLE_TYPE_DOUBLE` 추가. `GetTrainerBattleType`(include/data.h:316) 존재, 깨끗이 적용. |  | 없음 | HNS 리매치 테이블은 유지 |
| #9973 | Fix `FillSpriteRect` wrongly triggering an assert | 미적용·안전 이식 가능 | `FillSpriteRect` X축 스프라이트 전환 위치 수정. HNS 체력바·사파리·HP%에서 `FillSpriteRectColor` 사용. 9973→9813→10349→10392 연쇄가 함께 `--check` 통과. |  | 없음 | 묶음 이식 권장 |
| #9995 | Prevent error in title screen cinematic if Kyogre/Groudon are disabled | HNS와 무관 | Emerald 인트로 장면 3 울음소리 가드. HNS는 `SetUpCopyrightScreenHns`(src/intro.c:1057) 경로이고 두 종 모두 활성. |  | 없음 | - |
| #10015 | Fix incorrect action handler being set after rearranging moves in Emerald tutorial battle | 미적용·안전 이식 가능 | `HandleMoveSwitching`의 첫 전투 핸들러에 `IS_FRLG` 조건 추가(src/battle_controller_player.c:1091·1111). HNS 비FRLG 경로에서 OakOldMan 핸들러로 잘못 바뀌는 버그. |  | 없음 | 1줄 |
| #9949 | mapjson assume undefined layout and maps match the compiled version | HNS와 무관 | mapjson region/layout 기본값. HNS `tools/mapjson/mapjson.cpp`는 `hns` 버전·레이아웃 필터를 자체 구현(region 로직 없음). map.json 518개는 Emerald/FRLG 맵. |  | 없음 | 이식 금지(HNS mapjson 덮어쓰기 위험) |
| #10061 | 📜 Migration script for pret#2200 | 미적용·선행 필요 | pret#2200(암묵적 waitstate)용 마이그레이션 스크립트. 9986 이식 시 HNS 맵 스크립트(`*_hns` 560개 디렉터리)에 실행해야 함. |  | 없음 | 선행: 9986 |
| #9909 | Update maintainer list | HNS와 무관 | CONTRIBUTING 문서. |  | 없음 | - |
| #10054 | Updated expansion scope to include Champions | HNS와 무관 | 문서(scope/styleguide). |  | 없음 | - |
| #10067 | updated DNS tutorial to be more clear | HNS와 무관 | 문서(DNS 튜토리얼). |  | 없음 | - |
| #9986 | Branch Synchronisation (pret) | 미적용·선행 필요 | pret 동기화(5/15): pret#2200 암묵적 waitstate(`special`/`specialvar` 뒤 자동 waitstate), INCGFX 성능 개선, preproc 줄번호, match call 문서화 등. INCGFX(9881)가 전제이고, 암묵 waitstate는 HNS 스크립트에 떨어진 explicit waitstate가 있으면 이중 대기 위험. |  | 작음 | 선행: 9881. 10061 마이그레이션을 HNS 스크립트에 실행 필요 |

### 1.16.0

| PR | 제목 | 판정 | 근거 | 고위험파일 | 용량 영향 | 의존·비고 |
|---|---|---|---|---|---|---|
| #9641 | feat(debug): add quickstart from titlescreen | HNS와 무관 | 타이틀 화면 SELECT 퀵스타트(개발 편의). 기본 `ENABLE_QUICKSTART TRUE`이고 릴리스에서만 꺼지므로 `make hns`(비릴리스)에서 HUD가 HNS 타이틀에 노출. `src/title_screen.c` HNS 수정과 겹침. |  | 작음(+) | 이식한다면 기본값 FALSE |
| #9207 | Remove unnecessary EWRAM allocation for cable car animation | 미적용·안전 이식 가능 | 케이블카 EWRAM 정적 변수 6개를 지역 변수로. 깨끗이 적용. |  | EWRAM -6B | - |
| #8816 | Cleaned up Match Call | HNS와 무관 | Match Call 데이터 표현만 정리(비기능). HNS가 `src/pokenav_match_call_data.c`에 HNS 테이블 522줄 추가. |  | 없음 | 비기능 리팩터, 이식 불필요 |
| #9415 | Add Instructions on how to build FRLG by default | HNS와 무관 | FRLG 빌드 문서·마이그레이션 스크립트. |  | 없음 | - |
| #9231 | Random EWRAM and IWRAM savings | 미적용·안전 이식 가능 | `RfuDebug` 미사용 필드 제거(HNS는 `static EWRAM_DATA`로 약 230B), tv.c/save.c 미사용 COMMON 변수 제거. `link_rfu_2.c`만 컨텍스트(EWRAM_DATA vs COMMON_DATA) 차이라 수동. |  | EWRAM 약 -230B, IWRAM 약 -100B | EWRAM 95% 여유 확보 |
| #9066 | Typeless enums for OBJ_EVENT_GFX, Multichoice Ids, and Facility Classes | HNS와 무관 | OBJ_EVENT_GFX·multichoice·facility class를 typeless enum으로(비기능). HNS 전용 gfx 상수 다수와 충돌. |  | 없음 | 값이 바뀌면 SaveBlock1 objectEventTemplates의 graphicsId 호환 주의 |
| #9086 | preproc: (shared) COMPOUND_STRING | 미적용·안전 이식 가능 | preproc가 COMPOUND_STRING을 SHF_MERGE 섹션으로 만들어 동일 문자열을 공유. 9개 파일 깨끗이 적용. HNS 전용 `src/challenge_menu.c:507`의 `static const u8 sText_ConfirmSave[] = COMPOUND_STRING(...)`은 배열 초기화라 `_()`로 바꿔야 빌드됨. |  | ROM 감소(한 줄 리터럴 중복 243건 기준 약 3~4KB 이상 추정) | 9463(PRIx64)은 컨텍스트가 달라 수동. 9667의 전제 |
| #9420 | Removes unused files | 미적용·안전 이식 가능 | 빈 헤더 `partner_parties.h`/`trainer_parties.h` include 제거(비기능). 컨텍스트만 차이. |  | 없음 | - |
| #9410 | pr adding telekinesis ban list to species data | HNS와 무관 | Telekinesis 금지 목록을 종 플래그로 이동(동일 목록, 비기능). 종 데이터·`battle_script_commands.c` 겹침. | Y | 없음 | 비기능, 이식 불필요 |
| #9467 | Change `TrainersMon`'s ball from `u8` to `enum PokeBall` | 미적용·안전 이식 가능 | `TrainerMon.ball`을 `enum PokeBall ball:8`로(동일 크기). 깨끗이 적용. |  | 없음 | 비기능 |
| #9051 | Converted script_menu.h strings to COMPOUND_STRINGs | 충돌 | script_menu 문자열을 `strings.c`에서 `src/data/script_menu.h` COMPOUND_STRING으로 이동. HNS 한글 문자열이 있는 두 파일 모두 FAIL. |  | 없음(9086 없으면 중립) | 비기능. 충돌 절 참조 |
| #9376 | Clean up boolean comparisons | HNS와 무관 | 불리언 비교 표기 정리(비기능). `TryRunFromBattle`(HNS 도주 메시지 변경 영역), `text.c`, `sound.c` 겹침. | Y | 없음 | 이식 금지 권장 |
| #9466 | Fix enum usage | HNS와 무관 | enum 타입 표기 정리(비기능, 표본 확인). 배틀 파일 다수. | Y | 없음 | - |
| #9522 | Master merge 2026-03-14 | 이미 적용 | master→upcoming 병합. master 쪽 부모(#9520, 2026-03-14)가 HNS 계보에 포함. |  | 없음 | 컨테이너 병합 |
| #9539 | Some bool cleanup | 미적용·안전 이식 가능 | easy_chat 반환형 bool32화(비기능). 깨끗이 적용. |  | 없음 | - |
| #9542 | Replace MAX_GIFT_RIBBONS with NUM_GIFT_RIBBONS | 미적용·안전 이식 가능 | `GIFT_RIBBONS_COUNT`(11)→`NUM_GIFT_RIBBONS`(7)+`padding[4]`. SaveBlock1 `giftRibbons`(include/global.h:1280) 크기 11바이트 유지. |  | 없음 | 세이브 호환 유지. constants/global.h 컨텍스트 수동 |
| #9562 | Remove PARTNER_DUMMY need by adding additional difficulty when TESTING | 미적용·안전 이식 가능 | `PARTNER_DUMMY` 제거와 테스트용 `DIFFICULTY_TEST`. HNS는 파트너 1~5(HNS 전용 4명)+DUMMY=6이므로 번호 유지한 채 DUMMY만 제거 가능. | Y | ROM 소량 감소 | 수동, 저우선 |
| #9529 | `GenConfig` naming cleanup | HNS와 무관 | `generational_changes`→`config_changes` 파일명 변경(비기능). HNS가 `include/constants/generational_changes.h`에 알 상속 설정 5개를 추가함. | Y | 없음 | 후속 이식 시 파일명 매핑 필요 |
| #9570 | fix(pokedex_plus_hgss): revert incorrect usage of species enum | 이미 적용 | `git apply -R --check` 성공. |  | 없음 | - |
| #8213 | Remove leftover DebugPrintfs | 미적용·안전 이식 가능 | 남은 `DebugPrintf` 3줄 제거(fishing.c 526·654, sprite.c 1479). sprite.c는 줄 위치만 차이. |  | 없음 | 비기능 |
| #9537 | Fix Kanto object event graphic names | HNS와 무관 | FRLG(Kanto) 오브젝트 그래픽 이름 변경. HNS 빌드 비대상 맵. |  | 없음 | - |
| #9241 | Consolidated Battle Tower classes and object events | HNS와 무관 | Battle Tower 클래스/그래픽 배열 구조체 통합(비기능). | Y | 없음 | - |
| #9578 | identified deprecated values | 충돌 | `BattleScript_HealerActivates` 문구를 `STRINGID_PKMNSXCUREDITSYPROBLEM`으로 바꾸고 `STRINGID_PKMNSXCUREDYPROBLEM`·`BattleScript_ObliviousPreventsAttraction` 삭제. HNS는 이미 `STRINGID_HEALERCURE`로 출력 변경(BATTLE_MESSAGE_OUTPUT_CHANGES.md 50행). 삭제 대상에 한글 문자열 포함(battle_message.c:490). | Y | ROM 소량 감소 | 충돌 절 참조 |
| #9634 | Removed STRINGID_PKMNISGLOWING | 미적용·안전 이식 가능 | `STRINGID_PKMNISGLOWING` 삭제, Sky Attack를 `STRINGID_CLOAKEDINAHARSHLIGHT`로 고정. HNS `B_UPDATED_MOVE_DATA GEN_LATEST`라 현재 출력 동일. 한글 문자열 1개(battle_message.c:267) 삭제. | Y | ROM 소량 감소 | 배틀 메시지 파일. 삭제 전 사용자 확인 권장 |
| #9624 | 📜 Consolidate decoration values | HNS와 무관 | 장식 데이터 통합(비기능, 마이그레이션 스크립트). |  | 없음 | - |
| #9667 | Convert move description variables into COMPOUND_STRINGs | 미적용·선행 필요 | 기술 공용 설명(sMegaDrainDescription 등)을 COMPOUND_STRING 인라인으로. 9086 없이 하면 같은 설명이 여러 번 들어가 ROM 증가. `src/data/moves_info.h`는 한글 기술명 파일. |  | 9086 없으면 증가 | 선행: 9086. 이식 비권장 |
| #9677 | Converted move animation magic numbers to defines | HNS와 무관 | 기술 애니 매직넘버를 define으로(비기능). |  | 없음 | - |
| #9711 | Remove hex values from BattlePokemon | HNS와 무관 | BattlePokemon 오프셋 주석 제거(비기능). |  | 없음 | - |
| #9713 | Support non-contiguous SE/MUS IDs in debug menu | 충돌 | 디버그 사운드 메뉴 비연속 ID 지원. HNS `include/constants/songs.h`가 DP 음악을 추가해 `END_MUS DP_MUSIC_END`(690행) 등 목록이 다름. |  | 작음 | 디버그 전용, 저우선 |
| #9725 | Fix multi_do multiple-definition error | HNS와 무관 | `multi_do` 매크로 로컬 레이블. HNS 매크로에는 해당 레이블이 없음(#8943 12v12 미도입). |  | 없음 | upcoming 회귀 수정 |
| #9721 | Learnset Helper: Remove unnecessary order-only dependencies | 미적용·안전 이식 가능 | Makefile learnset helper order-only 의존성 제거. 깨끗이 적용. |  | 없음 | 빌드 |
| #9761 | Fix spaces, spelling, and grammar | HNS와 무관 | 주석·공백·철자(비기능). 한글화 파일(battle_message.c, strings.c) 다수 겹침. | Y | 없음 | 이식 금지 권장 |
| #9780 | Revert an overzealous find-replace | HNS와 무관 | 주석만 되돌림. |  | 없음 | - |
| #9813 | Improve `FillSpriteRect` performance | 미적용·안전 이식 가능 | `FillSpriteRect` 모듈러를 비트마스크로(성능). 9973 뒤에 적용. |  | 없음 | 9973 묶음 |
| #9879 | Move wild encounter related config to `wild_encounter` config file | HNS와 무관 | 야생 조우 설정을 `config/wild_encounter.h`로 옮기고 `B_*`/`OW_*`→`WE_*` 이름 변경(비기능). HNS 설정값(`FLAG_SMART_WILD_AI`, `FLAG_NO_WILD_CATCHING`, `FLAG_NO_WILD_RUNNING`, `FLAG_DISABLE_ENCOUNTERS`) 보존 필요. | Y | 없음 | 후속 이식에서 WE_* 참조 시 이름 매핑 |
| #9914 | Fix stray tab in C file | HNS와 무관 | 탭 문자 1개(비기능). |  | 없음 | - |
| #9911 | Remove two unused functions from src/event_object_movement.c | 미적용·안전 이식 가능 | 미사용 함수 제거. HNS에는 `Unref_TryInitLocalObjectEvent`(168B)만 있고 `UpdateObjectEventCoords`는 없음. |  | ROM -168B | 해당 hunk만 수동 |
| #9906 | Reduce battle heap usge outside main battle screen | 미적용·안전 이식 가능 | 배틀 중 파티/가방/진화/도감 화면 진입 시 배틀 애니 BG 버퍼(0x3000) 해제 후 복귀 시 재할당. 기능 부분은 HNS와 같고 `battle_main.c`는 `gParties`→`gPlayerParty` 컨텍스트만 차이. | Y | 없음(힙 피크 -12KB) | 수동. 10014와 함께 |
| #9788 | 📜 Adds migration script for trainer pic refactor | HNS와 무관 | 트레이너 그림 마이그레이션 스크립트. |  | 없음 | - |
| #9890 | Refactor a more generic way to handle complex debug menu actions | 미적용·선행 필요 | 디버그 메뉴 리스트 생성 구조 리팩터. HNS `src/debug.c`는 upstream 대비 528줄 추가된 포크이고 이 구조가 없음. |  | 없음 | 선행: upcoming debug.c 구조. 저우선 |
| #9965 | config/wild_encounter.h cleanup | HNS와 무관 | `config/wild_encounter.h` 가드 정리. 9879 전제, 비기능. |  | 없음 | - |
| #10051 | Give enum BattleTrainer more user friendly entry names | 미적용·선행 필요 | `enum BattleTrainer` 항목명 변경(비기능, 126파일). `gParties`/`B_TRAINER_*` 전제. | Y | 없음 | 선행: #8943 |
| #9885 | Adds deprecation warnings to old party and party count globals | 미적용·선행 필요 | 구 파티 전역 변수 deprecation. HNS에 `gParties` 없음. |  | 없음 | 선행: #8943 |
| #10036 | Describe compressed tilemaps with INCGFX | 미적용·선행 필요 | 압축 tilemap을 INCGFX로 기술. |  | 없음 | 선행: 9881 |
| #10070 | 28//05/26 Master to upcoming merge | HNS와 무관 | master→upcoming 병합 컨테이너. master 쪽 PR은 개별 항목으로 판정. |  | 없음 | - |
| #9463 | Use PRIu64 to print appropriate variable length in preproc | 미적용·선행 필요 | preproc 해시 출력 `%lx`→`PRIx64`. 9086 뒤에 적용해야 하고 c_file.cpp 컨텍스트가 달라 수동. |  | 없음(도구) | 선행: 9086. WSL(LP64)에선 기능 차이 없음 |
| #9505 | fix: battle message assert no longer calls for gText_Blank | 미적용·안전 이식 가능 | `BattleStringGetOpponentNameByTrainerId` assert 실패 경로 반환값을 `sText_EmptyString4`로(battle_message.c:3090). 정상 출력 불변, 깨끗이 적용. | Y | 없음 | 배틀 메시지 파일이지만 출력 불변 |
| #9579 | Fix upcoming compile | HNS와 무관 | `SetBattlerStatStagesForSwitchin` 중괄호. HNS에 해당 함수 없음(upcoming 회귀). |  | 없음 | - |
| #9608 | Removes DecompressTrainerBackPic fixing multibattles with 5-frame partner back sprites | 미적용·안전 이식 가능 | `DecompressTrainerBackPic` 제거. 백스프라이트 템플릿이 `backPic`을 직접 쓰므로(src/pokemon.c:4311) `spritesGfx` 복사는 불필요하고 5프레임이면 넘침. HNS 백스프라이트는 4프레임이라 예방적. | Y | ROM 소량 감소 | 헤더 컨텍스트만 수동 |
| #9668 | Fix spritesheet_rules.mk for renamed FRLG objects | HNS와 무관 | FRLG spritesheet 규칙 이름(9537 전제). |  | 없음 | - |
| #9767 | fix meganium abilities | 이미 적용 | 메가니움 기본 특성 복구. HNS 기본 메가니움은 이미 OVERGROW/LEAF_GUARD(gen_2_families.h:168), 메가만 MEGA_SOL. |  | 없음 | upcoming 회귀 |
| #9835 | Fix declaration after label | 미적용·안전 이식 가능 | `case` 뒤 선언을 중괄호로 감쌈(`CalcMoveBasePowerAfterModifiers` EFFECT_SOLAR_BEAM, 파티 메뉴). HNS에도 같은 패턴(battle_util.c:6692). 비기능. | Y | 없음 | 컴파일 이식성 |
| #9964 | Fix trainer defines | 이미 적용 | HNS `include/constants/trainers.h`에 이미 같은 괄호식 정의. |  | 없음 | - |
| #9861 | Fix Dynamic Summary Screen Type | HNS와 무관 | 요약 화면 동적 타입. HNS `P_SHOW_DYNAMIC_TYPES FALSE`(config/pokemon.h:63)라 호출 경로 비활성. |  | 없음 | - |
| #10014 | Fix battle debug not properly freeing some tilemaps | 미적용·선행 필요 | 배틀 디버그 메뉴에서 `CloseMainBattleScreen` 사용. 9906이 만든 함수. |  | 없음 | 선행: 9906 |
| #10050 | Set daily seed on new game | 미적용·선행 필요 | 새 게임에서 일일 시드 설정. HNS에 `UpdateDailySeed` 없음. |  | 없음 | 선행: #9920 Basic daily seed |
| #9841 | Remove known failing for Blunder Policy/Darts interaction | HNS와 무관 | 테스트 파일만(Blunder Policy KNOWN_FAILING 제거). | Y | 없음 | 테스트 전용 |
| #9865 | Mega Sol test adjustments | 부분 적용 | Mega Sol 날씨 처리. HNS는 `GetAttackerWeather`의 MEGA_SOL 분기(battle_util.c:9676), 회복·2턴 기술(`Cmd_recoverbasedonsunlight` 9530행, battle_move_resolution.c:1488)은 이미 다른 형태로 반영. `GetSwitchinWeatherImpact`(battle_ai_switch.c:1583)의 AI 교체 날씨, 속도 특성의 `GetWeather()` 치환, `weatherHasEffect` 제거는 미적용. | Y | 없음 | 배틀 그룹과 조율 |

### 1.16.1

| PR | 제목 | 판정 | 근거 | 고위험파일 | 용량 영향 | 의존·비고 |
|---|---|---|---|---|---|---|
| #10130 | Add `if_comptime` | 미적용·안전 이식 가능 | `if_comptime`으로 50% 판정은 `RandomUniform(0,1)` 사용. 깨끗이 적용. |  | 없음 | 같은 시드의 50% 판정 결과가 바뀜(녹화 배틀·테스트) |
| #10110 | Cleanup 'message 0x0' | 미적용·안전 이식 가능 | `message 0x0`→`message NULL`(비기능). |  | 없음 | - |
| #10122 | Fix Trainer Party Pool documentation | HNS와 무관 | 문서. |  | 없음 | - |
| #10131 | Remove deprecated ``HQ_RANDOM`` config | 충돌 | `HQ_RANDOM` 정의 삭제. HNS 전용 `src/randomizer.c:354`가 `#if HQ_RANDOM == TRUE`를 사용해 삭제 시 `_SFC32_Next(&gRngValue)` 경로로 바뀜. |  | 없음 | 충돌 절 참조 |
| #10123 | chore(credits): change kildemal to miriam in credits | HNS와 무관 | 크레딧 문서. |  | 없음 | - |
| #9892 | Misc Fixes/Cleanups | 미적용·안전 이식 가능 | 부분·수동. 안전한 부분: `save_failed_screen.c` DUMMY_WIN_TEMPLATE 누락 수정, battleScriptsStack 경계 검사, `InitWindowsChecked`/`BgTileAllocOp` assert, `TryBoxMonFormChange` assert 분리, malloc 정리. 제외: `config_changes.c`(9529 전제), `text.c` 함수명 교정(HNS 한글 text.c에 정의 없음), test runner. |  | 없음 | 비릴리스 빌드에서 HNS 전용 창 템플릿의 종결자 누락이 assert 화면으로 드러날 수 있음 |
| #10147 | Fix HGSS Dex Form Strings | 미적용·안전 이식 가능 | HGSS 도감 폼 화면 버튼 안내 문구 복구(pokedex_plus_hgss.c:228·230). HNS의 이 파일은 미번역, 깨끗이 적용. |  | 극소 | 추후 한글화 대상 |
| #10103 | Fix Beat Up typeless damage test effectiveness check | 미적용·안전 이식 가능 | 테스트만(Beat Up). 깨끗이 적용. | Y | 없음 | 테스트 전용 |
| #10120 | Fix broken/old links in CREDITS.md | HNS와 무관 | CREDITS 링크. |  | 없음 | - |
| #10113 | Branch Synchronisation (pret) | HNS와 무관 | pret 동기화(6/1): `{CLEAR_TO 0x03}`→`{CLEAR_TO 3}` 같은 동일 바이트 표기 정리, `sSpriteTileRanges` 2차원화 등 비기능. `strings.c`·`script_menu.h` 한글 파일 FAIL. |  | 없음 | 이식 불필요 |

### 1.16.2

| PR | 제목 | 판정 | 근거 | 고위험파일 | 용량 영향 | 의존·비고 |
|---|---|---|---|---|---|---|
| #10156 | fatal_assertf | 미적용·선행 필요 | `fatal_assertf` 도입. malloc/window/bg/sprite가 9892 이후 형태를 전제. |  | 작음 | 선행: 9892. 릴리스 빌드에서도 복구 불가 크래시 화면이 되므로 신중 |
| #10332 | Fix excessive indentation in trainer_pools.c | HNS와 무관 | trainer_pools.c 들여쓰기(비기능). |  | 없음 | - |
| #10353 | Update how to pokemon | HNS와 무관 | 문서. |  | 없음 | - |
| #10194 | Check if species is disabled when filling pc boxes | 미적용·안전 이식 가능 | 디버그 PC 박스 채우기에서 비활성 종 건너뛰기. HNS `DebugAction_PCBag_Fill_PCBoxes_Fast` 존재, 수동. |  | 없음 | 디버그 전용 |
| #10210 | Fix declaration after label errors | HNS와 무관 | `case` 뒤 선언 수정. HNS에는 `MoveEndProtectLikeEffect`나 `REALLOW_MOVEMENT` 케이스가 없음. |  | 없음 | - |
| #10241 | Fix off by one error in randomweightedIndex | 미적용·안전 이식 가능 | `RandomWeightedIndex` 경계 `<=`→`<`(src/random.c). HNS 호출처는 팔로워 감정(event_object_movement.c:2887). 깨끗이 적용. |  | 없음 | - |
| #10198 | Changes to mapjson.cpp and how_to_frlg.md | HNS와 무관 | mapjson+FRLG 문서. HNS mapjson 자체 구현(9949와 같음). |  | 없음 | - |

### 1.16.3

| PR | 제목 | 판정 | 근거 | 고위험파일 | 용량 영향 | 의존·비고 |
|---|---|---|---|---|---|---|
| #10323 | refactor: unified trainer slide and script comparison operators | HNS와 무관 | 트레이너 슬라이드·스크립트 비교 연산자 enum화(비기능). event.inc/scrcmd.c/trainer_slide.c FAIL. |  | 없음 | - |
| #10437 | cleanup: remove comment with incorrect time labels | HNS와 무관 | 주석. |  | 없음 | - |
| #10442 | fix(cleanup): remove unused variables | 미적용·안전 이식 가능 | 미사용 지역 변수 제거(battle_ai_items, battle_anim_water, string_util). 깨끗이 적용. |  | 없음 | 비기능 |
| #10469 | Cleanup debug.inc | HNS와 무관 | debug.inc의 10199 예제 제거. HNS에는 그 예제가 없음. |  | 없음 | - |
| #10349 | Fix sprite fill function | 미적용·안전 이식 가능 | `FillSpriteRect` 마스크 수정. 9973 묶음. |  | 없음 | 9973 묶음 |
| #10392 | Fix sprite fills being broken in some cases | 미적용·안전 이식 가능 | `FillSpriteRect` 부분 타일 마스크 수정. 9973 묶음. |  | 없음 | 9973 묶음 |
| #10385 | Fix FRLG Magikarp and Heracross size units | HNS와 무관 | FRLG 맵 스크립트. |  | 없음 | - |
| #10301 | Fix buffer overrun in ReformatItemDescription | HNS와 무관 | `ReformatItemDescription` 버퍼 넘침 수정. HNS `OW_SHOW_ITEM_DESCRIPTIONS OFF`(config/overworld.h:21). 켜더라도 새 구현은 한글 미지원 `line_break.c`를 사용. |  | 없음 | - |
| #10429 | Add Zero-Width-Space and fix some line break handling | 충돌 | ZWS 문자 `0x3A` 추가(charmap, `RenderText`, `IsWordSplittingChar`, 폭 0). HNS charmap에서 `0x3A`는 한글 2바이트 선두 바이트('뢸'=3A 01, '루'=3A 0C 등)이자 후속 바이트('겠'=37 3A). |  | 없음 | 이식 금지. 충돌 절 참조 |
| #10364 | Fix incorrect changelog in 1.16.2 | HNS와 무관 | changelog. |  | 없음 | - |
| #10199 | Dynamic Multichoice Tutorial | HNS와 무관 | dynmultichoice 튜토리얼 문서. 콜백 상수 enum화는 비기능, debug.inc 예제는 10469가 되돌림. |  | 없음 | - |

### 1.16.4

| PR | 제목 | 판정 | 근거 | 고위험파일 | 용량 영향 | 의존·비고 |
|---|---|---|---|---|---|---|
| #10555 | Pret merge (30th of July 2026) | 미적용·선행 필요 | pret 동기화(7/30): TextPrinterCallback 타입, PP 표기, 조우 슬롯 상수 생성, 비교 연산자 enum 등 대부분 비기능. 20개 파일 FAIL(`text.c`, `battle_message.c` 한글 파일 포함). |  | 없음 | 선행: 9881·9986·10323. 비기능 위주라 이식 불필요에 가까움 |
| #10641 | docs: updated PR template to be more explicit about AI | HNS와 무관 | PR 템플릿. |  | 없음 | - |
| #10721 | Pret merge (31st of August, 2026) | HNS와 무관 | pret 동기화(8/31): `SetAnimBgAttribute` 등 코드 위치 이동과 link.c UB 수정 위주(비기능). |  | 없음 | - |
| #10527 | Fix FRLG Trainer Card Setup | HNS와 무관 | FRLG 트레이너 카드. `HasAllMons`는 HNS 자체 구현(pokedex.c:4710)이 올바른 번호를 사용. |  | 없음 | - |
| #10581 | fix(save): fix reload_save functionality | 이미 적용 | `git apply -R --check` 성공. SESSION_LOG 2026-09-15에 기록. |  | 없음 | - |
| #10612 | Fix form change sometimes not getting level up moves | 이미 적용 | `git apply -R --check` 성공. |  | 없음 | - |
| #10597 | Fixes starter never being shiny when P_NO_SHINIES_WITHOUT_POKEBALLS | 이미 적용 | HNS pokemon.c:3335에 `FlagGet(FLAG_SYS_POKEDEX_GET)` 조건 반영(함수 구조만 다름). |  | 없음 | - |
| #10614 | Fix leveling up multiple levels at once sometmes teaching wrong moves | 이미 적용 | HNS battle_script_commands.c:5993에 `< currLvl` 후 증가 형태로 반영. | Y | 없음 | - |
| #10656 | (Joint with Syreldar) Fix Underutilisation of CreateSpriteUnchecked meaning a fatal assertf triggers needlessly | 미적용·안전 이식 가능 | 배틀 애니·필드 등에서 소진 가능 스프라이트를 `CreateSpriteUnchecked`+`MAX_SPRITES` 검사로. HNS `CreateSprite`는 assert가 있어(sprite.c:434) 비릴리스 빌드에서 스프라이트 부족 시 크래시 화면. 36개 파일 적용, 9개 수동. |  | 없음 | SESSION_LOG에서는 보류로 기록됨 |
| #10706 | Fixed a typo in how_to_new_pokemon.md | HNS와 무관 | 문서. |  | 없음 | - |

### 1.17.0

| PR | 제목 | 판정 | 근거 | 고위험파일 | 용량 영향 | 의존·비고 |
|---|---|---|---|---|---|---|
| #9797 | Scarlet and Violet Pokédex Skipping | 이미 적용 | 2026-09-19 이식(`ShouldSkipPokedexListEntry`, `P_SKIP_POKEDEX_GAPS`). `constants/pokedex.h` 매크로화만 의도적으로 제외. |  | 없음 | - |
| #8678 | Dynamic trainerbattle Scripts | 충돌 | 동적 trainerbattle 스크립트. `src/battle_setup.c`(HNS가 upstream 대비 +468줄: HNS 리매치 테이블 등), `battle_pyramid` FAIL, `trainer_battle.inc`·`trainer_see.c` 재구성. | Y | 작음~중간(+) | 충돌 절 참조. 기능 필요성 낮음 |
| #10293 | refactor(sprite): add sprite flag to create OBJWIN copies of oams | HNS와 무관 | 스프라이트 OAM을 OBJWIN으로 복제하는 플래그. 유일한 사용처가 `ShowItemIconSprite`(OW_SHOW_ITEM_DESCRIPTIONS OFF). |  | 없음 | 스프라이트 코어 변경 |
| #10108 | Cleanup battle factory screen constants | 미적용·안전 이식 가능 | 배틀 팩토리 화면 매직넘버→상수(비기능). 깨끗이 적용. |  | 없음 | - |
| #10119 | Cleanup hard-coded values from battle room scripts | 미적용·안전 이식 가능 | 배틀룸 스크립트 `7`→`FRONTIER_STAGES_PER_CHALLENGE`(비기능). 깨끗이 적용. |  | 없음 | - |
| #10058 | Add GEN_CHAMPIONS + move data | 이미 적용 | SESSION_LOG 2026-09-15 이식. HNS moves_info.h의 `GEN_CHAMPIONS` 55곳(PR 54곳). |  | 없음 | - |
| #10127 | Even more enums | HNS와 무관 | enum화(171파일, 비기능, 표본 확인). | Y | 없음 | - |
| #10152 | Clean up unused variables and functions | 미적용·안전 이식 가능 | 미사용 변수·함수 정리. 대부분 `static UNUSED`라 이미 컴파일러가 제거함. 실제 ROM 감소는 `StartMonScrollingBgMask`(508B) 정도. | Y | ROM 약 -0.5KB | 4파일 수동 |
| #10166 | Remove obsolete MAX_LEVEL_UP_MOVES | 미적용·안전 이식 가능 | `MAX_LEVEL_UP_MOVES` 상한 제거. HNS는 20→40으로 올려 완화(include/constants/pokemon.h:184). move_relearner 1183·1343, pokedex_plus_hgss 5275. |  | 없음 | 부분 완화 상태 |
| #10178 | Clean up header files | HNS와 무관 | 헤더 미사용 선언 삭제(비기능). | Y | 없음 | - |
| #10181 | Type checking paradise | HNS와 무관 | 타입 검사 강화(87파일, 비기능, 표본 확인). | Y | 없음 | - |
| #10184 | remove unused tilesets graphics | 미적용·선행 필요 | 미사용 타일셋 그래픽 정리(INCGFX_U32 사용). |  | 작음(-) | 선행: 9881 |
| #10251 | Fixes to move fields and descriptions | 이미 적용 | Sandstorm `windMove = TRUE`(moves_info.h:5488) 이미 있음. Stone Axe·Ceaseless Edge는 HNS가 이미 치명타 문구 없는 자체 설명 사용. |  | 없음 | - |
| #10274 | Fix anim tests | HNS와 무관 | 애니 테스트(`T_SHOULD_RUN_MOVE_ANIM`→`TESTING`). | Y | 없음 | 테스트 전용 |
| #10179 | Remove unused functions | 미적용·안전 이식 가능 | upstream에서 미사용이 된 함수 약 150개 삭제. 현 ELF에 145개 11.5KB가 남아 있음(`make hns`는 LTO=0이라 `-ffunction-sections` 없음). 단 HNS가 쓰는 `AI_ShouldSpicyExtract`, `CanBoxMonRelearnAnyMove`, `ResetTrainerOpponentIds`, `AddValToTilemapBuffer`, `CheckMemBlockInternal` 등은 빼야 함. 34파일 수동. | Y | ROM 약 -10KB 추정 | ROM 절감 후보 1순위 |
| #10314 | Replaced original sprite print behavior with actual sprite printer | 충돌 | 특성 팝업 인쇄를 스프라이트 프린터로 교체. HNS `PrintOnAbilityPopUp`/`PrintBattlerOnAbilityPopUp`(battle_interface.c:2794~)은 한글 조사 `"의"`를 붙이는 등 HNS 형태. `battle_interface.c` FAIL. | Y | ROM 소량 감소 | 충돌 절 참조 |
| #10328 | refactor(string): consolidate stringvar arrays with getter function | HNS와 무관 | stringvar 배열을 getter로(비기능). `scrcmd.c`는 `gParties` 컨텍스트. |  | 없음 | - |
| #10335 | Migrated Pokedex strings | 충돌 | 도감 문자열을 `strings.c`/`script_menu.h`에서 `pokedex.c` COMPOUND_STRING으로 이동(비기능). HNS 한글 도감 문자열(자모 등 복구 이력) 파일 FAIL. |  | 없음 | 충돌 절 참조 |
| #10404 | Fix argument field taking to much space in moves info | 미적용·선행 필요 | `MoveInfo.argument` 4바이트 고정, `Cmd_trymemento` 삭제, 2턴 기술 필드명 변경. 배틀 파일 11개 FAIL. HNS argument는 이미 u16 기반이라 ROM 이득 불확실. | Y | 불확실 | 선행: upcoming 배틀 리팩터 |
| #10531 | Rename sBattleWeatherInfo to  gBattleWeatherInfo | HNS와 무관 | `sBattleWeatherInfo`→`gBattleWeatherInfo` 이름 변경(비기능). | Y | 없음 | - |
| #10499 | Remove empty file from repo | 미적용·안전 이식 가능 | 빈 `item_icon_table.h` 삭제(비기능). 깨끗이 적용. |  | 없음 | - |
| #10625 | Remove leftover DebugPrintf statements | HNS와 무관 | AI `DebugPrintf` 제거. HNS에 해당 줄 없음. |  | 없음 | - |
| #10142 | Fix Debug Menu's `Fly to map` not working immediately | HNS와 무관 | 디버그 창 ID 초기화/비교 불일치 수정. HNS는 `tSubWindowId = 0` 초기화와 `!= 0` 비교(debug.c:933·950)로 일관돼 버그 전제가 없음. |  | 없음 | - |
| #10211 | fix(debug): incorrect use of const | HNS와 무관 | `generateListFunctions` const. 9890 구조 전제, HNS에 없음. |  | 없음 | - |
| #10523 | Fix smartTera assignment in MakePartnerGenerator | HNS와 무관 | `MakePartnerGenerator`. HNS에 `src/trainer_util.c` 없음. |  | 없음 | - |
| #10336 | Consolidate Battle Frontier streak appearance lists | HNS와 무관 | 프런티어 브레인 데이터 통합(비기능). |  | 없음 | - |
| #10334 | Consolidate contest values | HNS와 무관 | 콘테스트 값 통합(비기능, INCGFX 사용). |  | 없음 | - |
| #10619 | Fix test summary alignment for FAILED | HNS와 무관 | 테스트 러너 출력 정렬(도구). |  | 없음 | - |
| #10659 | Unify default move targeting between tests and in-game | 미적용·안전 이식 가능 | 기본 기술 대상 선택을 `GetDefaultSelectionTarget`으로 통일, 녹화 배틀 기본 대상. HNS에 대응 코드 있음(battle_controller_player.c:695~, battle_controller_opponent.c:506). 수동. | Y | 없음 | 컨트롤러는 고위험 파일, 저우선 |
| #10168 | Apply preproc to `GetRematchTrainerId()` function call inside `BattleSetup_ConfigureTrainerBattle()` | 이미 적용 | HNS battle_setup.c:1418에 `#if FREE_MATCH_CALL == FALSE` 가드 이미 있음. |  | 없음 | - |
| #10162 | A smidge of enums and type checking additions | HNS와 무관 | enum·타입 표기(비기능). | Y | 없음 | - |

## 충돌 상세

### #10429 Add Zero-Width-Space and fix some line break handling (1.16.3, `f90dfc5679`)

**upstream 목적**
- `ZWS = 3A`(charmap)와 `CHAR_ZWS 0x3A`(`include/constants/characters.h`)를 추가한다.
- `RenderText`는 `do { … } while (currChar == CHAR_ZWS)`로 ZWS를 건너뛴다.
- `IsWordSplittingChar`는 ZWS를 줄바꿈 가능한 지점으로 본다.
- 폭 함수는 ZWS에 0을 돌려준다.
- `StripLineBreaks`는 하이픈 뒤 줄바꿈을 ZWS로 바꾼다.
- 목적은 영문 하이픈 단어의 줄바꿈 개선이다.

**HNS 현재 동작과 보존 이유**
- HNS charmap에서 `0x3A`는 한글 2바이트 문자의 선두 바이트다. 예: `charmap.txt:1856~` '뢸'=`3A 01`, '루'=`3A 0C`, '룡'=`3A 0B`, '료'=`3A 06`.
- 후속 바이트로도 쓰인다. 예: '겠'=`37 3A`(1196행), '날'=`38 3A`(1435행), '듈'=`39 3A`(1674행).
- 일본어 'げ'도 `3A`다(220행).
- HNS `src/text.c:1587`은 1바이트를 읽은 다음 `IsKoreanGlyph(currChar)`일 때 다음 바이트를 합친다.
- upstream의 ZWS 건너뛰기가 그보다 먼저 실행되므로 '루·료·룡…'의 선두 바이트가 사라지고 이어지는 글자가 깨진다.
- `src/line_break.c:349 IsWordSplittingChar`는 바이트 단위로 판정한다. 그래서 '겠·날' 같은 글자의 후속 바이트에서도 단어가 잘린다.

**파일·함수·라인**
- `charmap.txt`(ZWS 추가 위치 48행 부근)
- `include/constants/characters.h:57`
- `src/text.c` `RenderText`(1587행 부근), 폭 함수(`GetFontWidthFunc` 이하)
- `src/line_break.c:349`

**선택지**
1. 이식하지 않는다(권장). 영문 하이픈 줄바꿈 개선은 한글 텍스트에 이득이 거의 없다.
2. 한글 선두·후속 바이트로 쓰이지 않는 코드(예: `FC`/`FD` 확장 제어 코드 체계)로 ZWS를 재정의한다. 그리고 `RenderText`의 ZWS 처리를 `IsKoreanGlyph` 판정 뒤로 옮긴다. charmap 재검증과 전체 한글 문자열 회귀 확인이 필요하다.

### #9881 Merge pret#2283 (INCGFX 이행, 1.15.2)

**upstream 목적**
- `INCBIN_*`과 `graphics_file_rules.mk`/`spritesheet_rules.mk`의 개별 변환 규칙을 없앤다.
- 대신 소스 안의 `INCGFX_*("x.png", ".4bpp.smol", 인자…)`로 gbagfx 인자를 기술한다.
- 규칙 파일 약 5,700줄을 삭제하고 그래픽 헤더 약 45,000줄을 치환한다.

**HNS 현재 동작과 보존 이유**
- HNS HEAD `0d89762071`이 `graphics_file_rules.mk:275~`에 pokeemerald-kr의 한글 폰트 16개 생성 규칙을 추가했다. `font0_korean.latfont` 등이며 `data/fonts.s`와 `src/text.c`가 incbin한다. 새 clone의 `make hns` 실패를 고친 변경이다.
- HNS 전용 그래픽(Gold/Kris/Lance/Silver, Soulgold UI, HNS 타일셋 등)은 모두 INCBIN과 규칙 파일 기반이다. 예: `src/data/graphics/trainers.h:646~`.
- 2026-09-16 SESSION_LOG에는 1.17.0 범위를 일괄 병합하다가 HNS 그래픽이 기본본으로 덮인 회귀가 기록돼 있다.

**선택지**
1. 보류하고 INCBIN을 유지한다(권장).
   - 9881을 전제로 하는 #9986, #10036, #10184, #10334, #10555는 대부분 비기능이라 함께 보류해도 손실이 작다.
   - 단 #9986의 pret#2200(암묵적 waitstate)은 따로 판단해야 한다.
2. 전면 INCGFX로 이행한다.
   - `migrate_incgfx.py`를 HNS 전용 헤더에도 돌려야 한다.
   - 한글 폰트 규칙을 INCGFX 인자나 남은 규칙으로 다시 작성해야 한다.
   - 모든 산출물(`.4bpp`, `.gbapal`, `.smol`, `.latfont`)을 바이트 단위로 비교해야 한다. 작업량이 크다.

### #10131 Remove deprecated `HQ_RANDOM` config (1.16.1)

**upstream 목적**: 항상 SFC32를 쓰게 됐으므로 쓰이지 않는 설정을 삭제한다.

**HNS 현재 동작**
- HNS 전용 `src/randomizer.c:350~358 GenerateSeedForRandomizer()`가 `#if HQ_RANDOM == TRUE`이면 `Random32()`를 쓰고, 아니면 `_SFC32_Next(&gRngValue)`를 쓴다.
- 정의를 삭제하면 전처리 결과가 `#else` 경로로 바뀐다. 랜더마이저 시드 생성 방식이 달라지거나 빌드가 실패할 수 있다.

**선택지**
1. `include/config/general.h:78`의 `HQ_RANDOM` 정의를 유지하고 PR을 건너뛴다.
2. PR을 이식하면서 `randomizer.c`의 조건부를 없애고 `Random32()`로 고정한다(현재 동작 유지, 권장).

### #9578 identified deprecated values (1.16.0)

**upstream 목적**
- `BattleScript_HealerActivates`의 문구를 `STRINGID_PKMNSXCUREDYPROBLEM`에서 `STRINGID_PKMNSXCUREDITSYPROBLEM`으로 바꾼다.
- 더 이상 쓰지 않는 `STRINGID_PKMNSXCUREDYPROBLEM`과 `BattleScript_ObliviousPreventsAttraction`을 삭제한다.

**HNS 현재 동작과 보존 이유**
- HNS `data/battle_scripts_1.s:6299 BattleScript_HealerActivates`는 사용자 요청으로 `STRINGID_HEALERCURE`("치유되었다!")를 출력한다. `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md` 50행에 기록돼 있다.
- 촉촉한몸·탈피 등도 상태별 문구로 바뀌어 있다(같은 문서 31·40행).
- 삭제 대상에는 한글 문자열 `src/battle_message.c:490`이 포함된다.
- 그 밖의 관련 위치: `include/constants/battle_string_ids.h:299`, `include/battle_scripts.h:187`, `data/battle_scripts_1.s:6712`, `test/text.c:794`.

**선택지**
1. 보류한다.
2. HealerActivates hunk는 빼고 미사용 문자열·스크립트만 삭제한다. ROM이 소량 줄지만 한글 문자열 삭제에 사용자 승인이 필요하다.

### #10314 Replaced original sprite print behavior with actual sprite printer (1.17.0)

**upstream 목적**
- 특성 팝업이 창을 만들어 타일을 복사하던 방식을 `SetupSpritesForTextPrinting`과 `AddSpriteTextPrinterParameterized6` 스프라이트 프린터로 바꾼다.

**HNS 현재 동작**
- `src/battle_interface.c:2752~2860`에 `AddTextPrinterAndCreateWindowOnAbilityPopUp`, `TextIntoAbilityPopUp`, `PrintOnAbilityPopUp`, `PrintBattlerOnAbilityPopUp`이 있다.
- 2829행에서 upstream의 영문 소유격(`'`/`'s`) 대신 한글 `"의"`를 붙인다.
- `SetupSpritesForTextPrinting` 자체는 HNS `sprite.c:2088`에 이미 있다.

**선택지**
1. 보류한다(현재 동작 정상, 이득은 코드 크기 소폭 감소).
2. 이식하면서 `"의"` 접미 처리와 한글 폭을 유지하고, Gen4 UI와 기본 UI 양쪽에서 팝업을 실기 확인한다.

### #8678 Dynamic trainerbattle Scripts (1.17.0)

**upstream 목적**
- trainerbattle 스크립트를 동적으로 구성한다. 인트로 없이 시작하거나 전투 전후 스크립트를 삽입할 수 있다.
- `battle_setup.c`(약 408줄 변경), `trainer_see.c`, `script.c`, `trainer_battle.inc`를 크게 재구성한다.

**HNS 현재 동작**
- HNS `src/battle_setup.c`는 merge-base 대비 +468줄이다.
  - HNS 리매치 테이블: `gRematchTable`, `#if IS_HNS` 160행~
  - `FREE_MATCH_CALL` 가드: 1418행
  - 파트너·HNS 트레이너 처리
- `battle_setup.c`, `battle_pyramid.c`, `include/battle_pyramid.h`가 FAIL이다.
- HNS 맵 560개의 trainerbattle 스크립트가 이 경로를 쓴다.

**선택지**
1. 보류한다(권장). HNS 스크립트에 필요한 기능이 아니다.
2. 배틀·오버월드 그룹의 이식이 끝난 뒤 수동으로 이식한다. 그 경우 HNS 트레이너·리매치·더블·파트너 전투를 전수 회귀 테스트해야 한다.

### #9051 Converted script_menu.h strings to COMPOUND_STRINGs / #10335 Migrated Pokedex strings (1.16.0 / 1.17.0)

**upstream 목적**: `strings.c`의 전역 문자열을 사용처(`src/data/script_menu.h`, `src/pokedex.c`)의 COMPOUND_STRING으로 옮긴다(비기능).

**HNS 현재 동작**
- 해당 문자열이 한글로 번역돼 있다. 예: `src/strings.c:556 gText_Items "도구"`, `576 gText_Single2 "싱글"`, `76 gText_PokedexRegistration "포켓몬 도감 등록 완료!"`, `92/94 gText_DexHoennTitle "성도도감"/"호연도감"`.
- 2026-09-16에 도감 자모 문자열을 복구한 이력이 있다.

**선택지**
1. 보류한다(권장, 기능 이득 없음).
2. 이식하되 한글 본문을 그대로 옮기고 이식 전후 문자열을 바이트 비교한다. #9086이 없으면 ROM은 중립이다.

### #9713 Support non-contiguous SE/MUS IDs in debug menu (1.16.0)

**upstream 목적**: 디버그 사운드 메뉴가 연속되지 않은 SE/MUS ID를 건너뛰게 하고, `END_SE`, `START_MUS`, `END_MUS`를 삭제한다.

**HNS 현재 동작**
- `include/constants/songs.h:279·282·690`에 `END_SE`, `START_MUS 350`, `END_MUS DP_MUSIC_END`가 있다(DP 음악 확장).
- `src/debug.c:4111~4260`의 `sBGMNames`/`sSENames`가 이를 사용한다.

**선택지**
1. 보류한다(디버그 전용).
2. 이식하면서 DP 음악을 포함한 `sSongNames`를 생성하고 `FIRST_PHONEME_SONG`(songs.h:694) 계산을 보존한다.

## 기타 메모

- **#9986(pret 5/15)의 pret#2200 암묵적 waitstate**
  - `special`/`specialvar` 뒤에 waitstate가 자동으로 들어간다.
  - 매크로는 바로 뒤에 오는 explicit waitstate만 무시한다. 떨어져 있는 explicit waitstate가 남아 있으면 이중 대기(softlock) 위험이 있다.
  - 이식한다면 #10061 마이그레이션 스크립트를 HNS 전용 맵·스크립트에도 실행해야 한다.
- **#9086 이식 시 확인 사항**
  - C 파이프라인은 CPP 다음에 preproc 순서로 돈다.
  - 배열 초기화 형태(`u8 x[] = COMPOUND_STRING(...)`)는 PR이 고친 contest 계열 외에 HNS `src/challenge_menu.c:507` 1곳뿐이다.
- **비릴리스 빌드의 assert**
  - `make hns`는 `RELEASE`가 아니다(Makefile:56·68). 그래서 `assertf`가 재개 가능한 크래시 화면으로 뜬다.
  - #9855, #9892, #10156처럼 assert를 추가하거나 강화하는 PR은 HNS 전용 데이터에서 새 화면을 띄울 수 있다.
  - #10156의 `fatal_assertf`는 릴리스에서도 복구할 수 없다.
- **#9865(부분 적용)**: 배틀 그룹과 겹치므로 AI 교체 날씨 영향과 `GetWeather()` 치환은 그쪽 판정과 함께 처리하는 것이 좋다.
