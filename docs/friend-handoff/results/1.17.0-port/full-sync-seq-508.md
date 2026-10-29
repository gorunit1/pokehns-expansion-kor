# full-sync 묶음 74 — seq 508 (#9892 Misc Fixes/Cleanups)

- 요청: 사용자 "ㄱㄱ 508 시작해 혼자해도 되는거라면"(2026-10-29).
- upstream: 읽기 전용 사본, #9892 병합 커밋 `3180f2767d`(1.16.1, 첫 부모 대비 25파일 +261/−116, 묶은 커밋 25개). 단위 `U-assert-9892`(다음 #10156 = seq 509).
- 혼자 진행한 근거: 디버그 검사(assert)·정리·테스트 러너. 새 문장 0. 계획서 경고(비릴리스 `make hns`에서 HnS 창 템플릿 종결자 누락·배틀 스크립트 스택 초과가 새 assert 화면으로 드러날 수 있음)는 아래처럼 전수 확인.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 508 | #9892 | `d84f57bf` | upstream 24파일(`include/test/battle.h` 주석 포함) — 아래 목록 | 32,980,660(+4,464) |

이식 전(`ca00e564`, 게임 코드 = `9dbdb90d`) SHA1 `29ee420b…` → 이식 후 SHA1 **`71e7788aff8b4ad1957e30d21d090f81e1d04113`**.

## 내용

- `InitWindows`가 `InitWindowsChecked` 매크로가 됨: 배열이면 마지막 원소, 포인터면 앞에서부터 찾아 `DUMMY_WIN_TEMPLATE`(bg 0xFF) 종결자가 없으면 assert. `CB2_FlashNotDetectedScreen`의 빠진 종결자 추가.
- `BattleScriptPush`·`BattleScriptPushCursor` 경계가 `UINT8_MAX`(255) → 실제 배열 크기(8). 넘치면 assert 후 넣지 않음(이전엔 배열 밖에 씀).
- 배틀 컨트롤러 비트 함수 10개에 `battler < MAX_BATTLERS_COUNT` assert.
- `Alloc`/`AllocZeroed`가 실패하면 `errorf`(위치·크기), 검사 없는 `AllocUnchecked`·`AllocZeroedUnchecked`, `PrintHeap`. malloc 내부 함수 static. `FREE_AND_SET_NULL`을 `do { } while (0)`로.
- `BgTileAllocOp` = 검사판(`result > 0`), `BgTileAllocOpUnchecked`. `InitWindows`·`AddWindow`의 할당은 검사 없는 쪽.
- assert 화면 `%%`·`%.*s`/`%.*S`, `mini_pchar_decode` 공개, `RunBattleScriptCommands` 인라인 해제(같은 동작 3곳 공용).
- `map_data_rules.mk`: 맵 데이터 preproc에 `-s`(`.size` 처리, HnS preproc 지원 확인).
- 테스트 러너: `config_changes.c` 누수 예외 없앰(`TestInitConfigData`가 먼저 `Free`, 배틀 테스트에서 초기화 위치 이동), 결과 enum, 셀러브레이트 애니 생략 등.

## HnS 판단

- **창 템플릿 전수 검사(새 도구 `dev_scripts/hns_verify/port/wincheck.py`):** `src`의 `InitWindows(` 호출 82곳 — 넘기는 배열 정의를 찾아 마지막 원소 확인. 79곳 바로 OK, 3곳은 손으로: `battle_dome.c`·`roulette.c`는 종결자가 `#ifdef UBFIX` 안인데 HnS `include/config/general.h`가 `UBFIX`를 켬 → OK. `battle_bg.c`의 `gBattleWindowTemplates[...]`는 3종(일반·아레나·관동 튜토리얼) 모두 종결자 있음. HnS 전용 창(이름 입력·포켓기어·옵션·HnS 새 게임 소개 등)도 모두 종결자 있음.
- **`BgTileAllocOp` 새 assert:** FRLG가 아니면 늘 0을 돌려주는 함수라 assert에 걸릴 수 있지만, 부르는 곳은 `window.c`의 `gWindowTileAutoAllocEnabled == TRUE` 안뿐이고 이걸 켜는 곳은 FRLG 크레딧(`credits_frlg.c`)뿐 → HnS에서 안 닿음.
- **배틀 스크립트 스택:** 배열 8칸. 이전 HnS 누수(재확인 2 미래예지)는 해결된 상태. 8칸 넘는 연쇄가 있으면 전체 테스트·한글 회귀에서 assert로 드러남 → 결과 아래.
- **충돌 4곳:**
  - `src/malloc.c`: HnS는 seq 401 #10179에서 안 쓰는 `CheckMemBlockInternal`·`CheckMemBlock`·`CheckHeap`을 지운 상태(1.17.1과 같음) → HnS 쪽.
  - `src/text.c`: HnS `text.c`에는 `AddSpriteTextPrinterParametrerized` 본체가 없었음(선언만). upstream 쪽(철자 고친 `AddSpriteTextPrinterParameterized` 본체, 1.17.1에 있음)으로 — 안 쓰는 함수라 링크에서 빠짐.
  - `test/test_runner.c`: HnS의 `config_changes.c` 누수 예외 → upstream처럼 지움(누수 원인을 upstream이 고침).
  - `test/test_runner_battle.c` 2곳: HnS `InitTestBattlers(test)` 유지 + upstream의 `TestInitConfigData()` 위치 이동.
- `SanitizeSpeciesId` 분리는 HnS에 이미 있었음(같은 줄).
- ROM +4,464 B: 배틀 컨트롤러 인라인 assert가 호출부마다 들어가는 것 등(계획 추정 +0.5 KB보다 큼). 비릴리스 빌드라 assert 코드·문자열이 남음.
- 세이브: 영향 없음.

## 검증

- **빌드:** `make hns -j8` 종료 0. 저장소 전체 재빌드(`build/hns` 지우고): 종료 0, SHA1 `71e7788a…`(첫 빌드와 같음), 새 컴파일 경고 0(`warncheck.sh`).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `d84f57bf`): 기준 `test-baseline-seq507.txt` 대비 **새 PASS 0, 사라진 PASS 0**(PASS 3,010). 목록 27줄이 다른데 모두 원래 FAIL인 글자 폭 테스트(능력 팝업·배틀 문장 창·도구 이름 등)의 **이름 라벨**뿐 — upstream "테스트 러너 `%S`를 mini_printf로" 때문에 라벨 속 한글 바이트가 `?`로 찍힘. 테스트별 `(n/n)` 이름과 상태로 맞추면 차이 0. 새 기준 **`test-baseline-seq508.txt`**(`verify_seq.sh`도 이 기준으로). 새 assert(창 종결자·스택·배틀러 범위·메모리)에 걸린 테스트 0.
- **한글 테스트:** 새로 쓰지 않음(문장 0, 검사 추가). 한글 회귀·전체 테스트가 새 assert에 걸리는지가 이번 판정.
- **세이브 정적 비교**(이전 = 클론 `ca00e564` 빌드): **PASS(FAIL 0, WARN 2)** — 둘 다 `BoxMonRestorePP` 안 assert 문자열 포인터 2개(`include/move.h`, `%s:%d: invalid move: %d`)가 주소만 옮겨진 것(문자열 내용·코드 같음). 세이브 블록 4종 크기·RAM 변수 같음.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s508 8`): **555/652, `판정: PASS (기대와 같음)`, 요약 바이트 같음** — 한글 회귀 배틀 652개 중 새 assert에 걸린 것 0.
- **ROM**(커밋 `d84f57bf`): SHA1 `71e7788aff8b4ad1957e30d21d090f81e1d04113`, 사용 32,980,660 B(묶음 73 대비 +4,464), 파일 33,554,432 B, 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기:** 새 화면 변화 없음. 다만 비릴리스 빌드라 혹시 창 종결자·스택 초과·잘못된 배틀러 접근이 있으면 그 자리에서 **assert 화면**이 뜸 — 실기 중 assert 화면이 나오면 화면 사진(파일·줄·메시지)을 받기로.

## 다음

- seq 509 #10156(`fatal_assertf`), 510 #10155·511 #9807·512 #10390(테스트 러너). 목표 1.17.1.
