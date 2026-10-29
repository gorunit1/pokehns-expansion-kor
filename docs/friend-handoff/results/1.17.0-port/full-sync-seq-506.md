# full-sync 묶음 72 — seq 506 (#10172 도감 공통 정의를 `pokedex_common.h`로)

- 요청: 사용자 "full-sync seq 502 #10116부터 port_sequence 순서대로 이어서 (목표 1.17.1)"(2026-10-29).
- upstream: 읽기 전용 사본, #10172 `215fbb88b9`(1.17.0, 5파일 +284/−283). 단위 `U-pokedex-9518`의 마지막(seq 504 다음).
- 혼자 진행한 근거: 헤더 나누기 + 안 쓰는 변수 삭제(문장·동작 0). 판정 기준 = 쓰는 코드 같음, ROM 차이는 안 쓰는 변수 쓰기만(충족).

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 506 | #10172 | `d4fc70b2` | `include/pokedex.h`의 두 도감 공통 정의(화면·검색 열거형, `struct PokedexView` 등)를 새 `include/pokedex_common.h`로, `pokedex.c`·`pokedex_plus_hgss.c`가 include. 안 쓰는 `gUnusedPokedexU8`(쓰기만 하고 안 읽음) 삭제 | 32,976,996(−16) |

이식 전(`bfce163d`, 게임 코드 = `1b640f80`) SHA1 `fdbe7fd7…` → 이식 후 SHA1 **`54c559b71a31714705156a5a3a9508b3335702ca`**.

## 내용과 HnS 판단

- **충돌 1곳:** `pokedex.h`의 옮겨 가는 블록 안에 HnS 수정(`pokedexList[OBTAINABLE_DEX_COUNT + 1]`, seq 504)이 있어서. → `pokedex.h`는 upstream 쪽(블록 삭제), 새 `pokedex_common.h`는 upstream 파일 + 그 1줄. HnS 성도 도감 선언 2줄(`GetJohtoPokedexCount`·`HasAllJohotoMons`)은 `pokedex.h` 위쪽이라 자동 병합으로 남음.
- **옮긴 블록 대조:** HnS `pokedex.h`의 공통 블록과 새 `pokedex_common.h`가 upstream이 바꾼 3곳 말고 같음 — `sSearchMenu_WindowTemplate[]`·`sPokemonList_WindowTemplate[]` → `[2]`(9518과 10172 사이 upstream 병합 커밋 `5ab619eba2`의 선언 크기, 정의는 그대로 2개), `Pokedex_CreateCaughtMonSprite` 선언 이동. `pokedex_common.h`는 1.17.1과 같음(OBTAINABLE 1줄 빼고).
- `gUnusedPokedexU8`: HnS 저장소에서 쓰는 곳은 이 커밋이 지우는 3곳(선언·정의·`ResetPokedex`/`ClearPokedexFlags` 쓰기)뿐. 세이브 데이터 아님(IWRAM 1바이트).
- 재확인 110: 이 PR은 헤더만 나눠서 HnS 전용 함수(`OpenPokedexInfoScreen` 등) 위치는 그대로 맞음(더 옮길 것 없음).

## 검증

- **빌드:** `make hns -j8` 종료 0(세션 임시 클론에서 먼저 빌드해 같은 SHA1). 저장소 전체 재빌드(`build/hns` 지우고): 종료 0, SHA1 `54c559b7…`(첫 빌드와 같음), 새 컴파일 경고 0(`warncheck.sh`).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `d4fc70b2`): 목록이 `test-baseline-seq504.txt`와 **바이트 같음**(PASS 3,008, 새 PASS 0, 사라진 PASS 0).
- **한글 테스트:** 새로 쓰지 않음(헤더 나누기, 도감 테스트 `hns_9518_pokedex.c`는 전체 테스트에 포함).
- **세이브 정적 비교**(이전 = 클론 `bfce163d` 빌드): **PASS(FAIL 0, WARN 1)** — WARN은 지운 `gUnusedPokedexU8`(IWRAM 1 B, −1 B). 세이브 블록 4종 크기 같음.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s506 8`): **555/652, `판정: PASS (기대와 같음)`, 요약 바이트 같음**.
- **ROM**(커밋 `d4fc70b2`): SHA1 `54c559b71a31714705156a5a3a9508b3335702ca`, 사용 32,976,996 B(묶음 71 대비 −16), 파일 33,554,432 B, 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 새 후보 없음**(묶음 70 도감 후보는 이 ROM으로 봐도 됨).

## 다음

- seq 507 #10334(컨테스트 값 정리, M), 508 #9892(Misc Fixes/Cleanups, L). 목표 1.17.1.
