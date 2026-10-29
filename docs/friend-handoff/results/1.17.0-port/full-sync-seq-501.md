# full-sync 묶음 67 — seq 501 (#10036 압축 타일맵 INCGFX)

- 요청: 사용자 "ㄱㄱ 501 시작해 혼자해도 되는거라면"(2026-10-29).
- upstream: 읽기 전용 사본, #10036 `ca63343b1e`(1.16.0, 45파일 +389/−387). 같은 단위 `U-incgfx-9881`(seq 500 다음).
- 혼자 진행한 근거: 빌드 체계 변경(문장·동작 0). 판정 기준 = ROM 바이트 같음(충족). 계획서 tsv 10036 행대로 HnS 전용 타일맵도 같은 방식으로 옮김.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 501 | #10036 | `4358b753` | `INCBIN_U32("x.bin.smolTM")` → `INCGFX_U32("x.bin", ".smolTM")` 446곳(upstream 387 + HnS 59), `tools/scaninc/scaninc.cpp`가 `.smolTM`에 `$(SMOLTM)` 규칙을 냄 | 32,960,436(같음) |

이식 전(`e56a58ef`, 게임 코드 = `2bfd086c`) SHA1 `5ffecc87…` → 이식 후 **SHA1 `5ffecc87…` 같음(바이트 같음)**.

## 내용과 HnS 판단

- **방법:** upstream의 C·테스트 변경은 한 가지 치환(`INCBIN_U32("….bin.smolTM")` → `INCGFX_U32("….bin", ".smolTM")`)뿐. 이 정규식을 upstream 기반 `src`·`test`에 돌리면 upstream 결과와 **파일 차이 0**인 것을 먼저 확인하고, 같은 정규식을 HnS `src`·`test`에 적용. `INCBIN_U8`/`INCBIN_U16`·괄호 안을 띄어 쓴 형태(`intro_frlg.c`, HnS `intro_hns.c` 같은 꼴)는 upstream처럼 그대로 INCBIN.
- **HnS 전용 타일맵 59곳**(같은 방식): 배틀 텍스트 박스(`battle_interface/hns/textbox_map`), 포케기어(헤더·배경 점·기기 외곽·라디오), 파티 메뉴(`party_menu/hns/bg`), 타이틀(`title_screen/hns/pokemon_logo`·`rayquaza`), 조토·관동 지도(`pokenav/region_map/map_johto`·`map_jk`, 도감 분포 지도), 오박사 소개 배경, 배틀 환경 23종(HnS가 더한 눈·화산·파란 건물 등과 모던 환경), 멀티 배틀 소개 모던 배경, UI 스크린숏 지도, 가방 메뉴, 타이틀 화면 1개.
- **scaninc:** 3-way 병합 충돌 0(HnS scaninc는 이 커밋 부모와 다른 곳이 seq 516 #9986 쪽 변경이라 이번 hunk와 겹치지 않음).
- **규칙 파일:** 변화 없음(upstream도 안 바꿈). 정리 도구(`dev_scripts/hns_verify/incgfx/prune_rules.py`)로 다시 확인 — 새로 필요 없어진 규칙 없음(합치기로 만드는 `.bin`은 INCGFX 원본으로 계속 쓰임). 재확인 109에 기록.
- 세이브: 영향 없음.

## 검증

- **빌드:** 생성물 없는 깨끗한 내보내기 트리 전체 빌드 → SHA1 `5ffecc87…`, 새 경고 0, `build/assets`의 `.smolTM` 439개. 저장소 전체 재빌드(`build/hns` 지우고): 종료 0, 같은 SHA1, 새 컴파일 경고 0(`warncheck.sh`).
- **ROM 바이트 비교:** 이식 전 클론(`e56a58ef`) 빌드와 **같음**(클론 SHA1 `5ffecc87b61f14d079450f0d2886e83ef5117178`).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `4358b753`): 목록이 `test-baseline-seq492.txt`와 **바이트 같음**(새 PASS 0, 사라진 PASS 0). (`test/compression/smol.c` 타일맵 4개 테스트가 INCGFX로 만든 압축 파일을 씀.)
- **한글 테스트:** 새로 쓰지 않음(ROM 바이트 같음).
- **세이브 정적 비교**(이전 = 클론 `e56a58ef` 빌드): **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s501 8`): **555/652, `판정: PASS (기대와 같음)`, 요약 바이트 같음**.
- **ROM**(커밋 `4358b753`): SHA1 `5ffecc87b61f14d079450f0d2886e83ef5117178`(묶음 65·66과 같은 파일), 사용 32,960,436 B, 파일 33,554,432 B, 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 새 후보 없음**(ROM 바이트 같음).

## 다음

- seq 502 #10116(한카리아스 메가Z INCGFX — HnS는 seq 500에서 같은 결과가 됐을 것으로 보임, 확인), 503 #10561. 목표 1.17.1.
