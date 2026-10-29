# full-sync 묶음 70 — seq 504 (#9518 도감 파일 통합)

- 요청: 사용자 "full-sync seq 502 #10116부터 port_sequence 순서대로 이어서 (목표 1.17.1)"(2026-10-29).
- upstream: 읽기 전용 사본, #9518 `58405c5044`(1.17.0, 5파일 +4,055/−7,814). 단위 `U-pokedex-9518`(다음 #10172 = seq 506).
- 혼자 진행한 근거: upstream 구조 정리(새 문장 0). HnS 도감 수정은 하나도 빼지 않고 새 위치로 옮김(아래 대조). `POKEDEX_PLUS_HGSS TRUE`라 HnS가 실제로 쓰는 화면이므로 **실기 확인 대상**.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 504 | #9518 | `dda68a3f` | 공통 도감 코드를 `src/pokedex.c`로, HGSS 화면은 `*_HGSS` 훅 뒤로(`include/pokedex.h` 공통 정의 +280줄). HnS 도감 수정 hunk 72개 + 원래 도감 13개를 새 위치에 다시 적용. 새 테스트 `test/hns_9518_pokedex.c` | 32,977,012(+16,576) |

이식 전(`3b2694a1`, 게임 코드 = `a5a0c700`) SHA1 `5ffecc87…` → 이식 후 SHA1 **`ed4348cce5d108aef334eb22ca911b82361ca03f`**.

## 내용과 HnS 판단

- **upstream 구조:** `pokedex_plus_hgss.c`(8,813→4,804줄)에서 원래 도감과 겹치던 목록·정보·울음소리·크기·검색 코드를 지우고, `pokedex.c`의 공통 함수가 `TryOpenPokedexPage_HGSS()` 같은 훅을 먼저 부름. `NationalPokedexNumToSpeciesHGSS` → 공통 `NationalPokedexNumToSpeciesForm`, `CB2_OpenPokedexPlusHGSS` → `CB2_OpenPokedex`, `struct PokedexView` 등은 `pokedex.h`로. `enum GrowthRate`는 packed(도감 통계 구조체뿐, 세이브 구조체 아님).
- **방법:** 3-way 병합은 `pokedex_plus_hgss.c` 충돌 32곳(대부분 upstream이 지운 구역에 HnS 수정이 있던 곳). 그래서 upstream 결과에서 출발해, HnS 수정(이식 전 upstream 기반 → HnS)을 hunk 하나씩 문맥이 정확히 맞는 곳(fuzz 0)에 붙이고(`hgss` 먼저, 안 맞으면 `pokedex.c`), 남은 13개는 손으로 옮김. fuzz를 허용하면 엉뚱한 구조체·함수에 붙는 것을 확인해 쓰지 않음.
- **옮긴 HnS 수정(요지):**
  - 획득 가능 도감(`OBTAINABLE_DEX_COUNT`, `NationalToObtainableOrder`) — 목록 크기·초기화·번호 표시·정보/통계 화면 번호·검색 범위, `pokedexList[OBTAINABLE_DEX_COUNT + 1]`(`pokedex.h`).
  - 목록 위아래 끝에서 반대쪽으로 넘어가기, 성도 도감 수(`GetJohtoPokedexCount`)·HnS 인터페이스 그림(`tileset_interface_hns`)·우측 숫자 위치·검색 결과 화면 전국 수.
  - 요약 화면에서 도감 정보 열기(`OpenPokedexInfoScreen` 등) — `LoadInfoScreen`·`CB2_Pokedex`가 정적인 `pokedex.c`로. 도감을 보통으로 열 때 되돌아갈 콜백 지우기는 공통 `CB2_OpenPokedex` 앞으로.
  - 진화 화면(줄 수 계산·아이콘 칸 `data[5+]`·랜덤화·지역 이름)과 진화·폼 화면에서 정보 화면으로 갈 때의 페이드 작업(`Task_WaitForFadeFromEvoFormsScreen`, `pokedex_plus_hgss.c`), 통계 화면 기술 칸 갱신·알 기술 수·아이템 아이콘 검사.
  - 울음소리 화면 "…의 / 울음소리"(`gText_CryOf1`·`gText_CryOf2`) — HGSS·원래 도감 둘 다 이제 공통 `PrintCryScreenSpeciesName`.
  - 원래 도감 쪽 HnS 수정(단위 설정, 획득 가능/성도 도감 수·완성 판정)은 위치가 그대로라 자동 병합.
- **대조:** 두 파일 합쳐 "HnS 변경 줄(이식 전 base → HnS)"과 "최종 변경 줄(upstream → 최종)"을 줄 단위로 비교 → 차이는 의도한 둘뿐: ① 정렬 4종·가나다 순 조건 앞의 upstream 경계 `temp_dexNum <= NATIONAL_DEX_COUNT &&` 유지, ② 새 프로토타입 1줄. 지워지는 HGSS 외부 심볼은 `CB2_OpenPokedexPlusHGSS`뿐이고 이 3파일 밖에서 안 씀. 지워지는 문자열은 `sText_No0000`(공통 쪽으로 이동)뿐, 한글 문자열(`src/strings.c`)은 그대로.
- **upstream이 바꾼 HGSS 표시 1곳:** 공통 `PrintCryScreenSpeciesName`은 원래 도감 형태라 이름을 60 px에 맞춰 글꼴을 줄임(`WrapFontIdToFit`). 한글 이름은 모두 60 px 안이라 기본 글꼴 그대로(새 테스트). 버퍼도 `POKEMON_NAME_LENGTH + 1` → `POKEMON_NAME_BUFFER_SIZE`(20)로 넓어져 "의"를 붙여도 여유.
- **ROM +16,576 B:** HGSS 쪽 `.text` 48,760 → 31,264 B로 줄었지만, 훅이 다른 파일 함수라 `POKEDEX_PLUS_HGSS`가 켜져도 원래 도감 경로가 링크에 남음(이식 전엔 원래 도감 코드가 컴파일에서 통째로 빠졌음). upstream 설계 그대로 두고 재확인 110에 기록.
- 세이브: 영향 없음(`struct PokedexView`는 도감을 열 때 잡는 메모리, 세이브 블록 아님 — 정적 비교로 확인).

## 검증

- **빌드:** `make hns -j8` 종료 0(세션 임시 클론에서 먼저 빌드해 같은 SHA1). 저장소 전체 재빌드(`build/hns` 지우고): 종료 0, 새 컴파일 경고 0(`warncheck.sh`). SHA1 `ed4348cc…`(클론 빌드와 같음).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `dda68a3f`): 기준 `test-baseline-seq492.txt` 대비 **새 PASS 2(위 한글 테스트), 사라진 PASS 0**, 그 밖 줄은 같음(PASS 3,008). 새 기준 **`test-baseline-seq504.txt`**(`verify_seq.sh`도 이 기준으로).
- **한글 테스트** `test/hns_9518_pokedex.c`(`TESTS='HnS 9518'`): 이식 후 2/2 PASS, 이식 전 코드(`a5a0c700` 클론 + 테스트 파일만)에서도 2/2 PASS(같은 결과 — 이식으로 바뀌지 않아야 하는 성질을 지키는 테스트) — ① 모든 종 이름이 울음소리 화면 60 px에서 `FONT_NORMAL` 그대로, ② 지역 도감 수 = 성도 도감 수(성도 1번을 잡은 상태).
- **세이브 정적 비교**(이전 = 클론 `3b2694a1` 빌드): **PASS(FAIL 0, WARN 1)** — WARN은 도감 작업 변수 5개(`sPokedexView`·`sPokedexListItem`·`sLastSelectedPokemon`·`sPokeBallRotation`·`sExternalReturnCallback`)와 `partyMonItem`이 `pokedex_plus_hgss.o` → `pokedex.o`로 옮겨 간 것(합계 +0 B). 세이브 블록 4종 크기 같음.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s504 8`): **555/652, `판정: PASS (기대와 같음)`, 요약 바이트 같음**.
- **ROM**(커밋 `dda68a3f`): SHA1 `ed4348cce5d108aef334eb22ca911b82361ca03f`, 사용 32,977,012 B(묶음 69 대비 +16,576), 파일 33,554,432 B, 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 하지 않음(친구) — 도감 전 화면 후보:**
  1. 도감 목록: 성도/전국 전환, 번호 표시(획득 가능 순서), 맨 위에서 위·맨 아래에서 아래로 반대쪽 넘어가기, 우측 본/잡은 수 숫자 위치.
  2. 정보 화면 → 분포·울음소리("○○의 / 울음소리")·크기 화면, 좌우 전환.
  3. 통계 화면(기술 목록 위아래로 움직일 때 위 칸 갱신, 알 기술), 진화 화면(진화 여러 개인 종 — 이브이 등, 아이콘·줄 간격), 폼 화면, 진화·폼 화면에서 A로 정보 화면 돌아가기.
  4. 검색 → 결과 목록(전국 수 표시) → 정보 화면.
  5. 포켓몬 요약 화면에서 도감 열기 → 닫으면 요약 화면으로 돌아오기.
  6. 새로 잡은 포켓몬 도감 등록 화면.

## 다음

- seq 505 #10184(안 쓰는 타일셋 그림 지우기), 506 #10172(도감 공통 함수 파일 분리 — 재확인 110). 목표 1.17.1.
