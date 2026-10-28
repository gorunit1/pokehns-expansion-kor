# full-sync 묶음 66 — seq 500 (#9881 pret#2283 합치기: INCGFX 그래픽 포함)

- 요청: 사용자 "ㄱㄱ 500 시작해 혼자해도 되는거라면"(2026-10-28).
- upstream: 읽기 전용 사본, #9881 `9f21d869e5`(1.15.2, pret 쪽 `9f23b871f9`를 expansion `3efb836f72`에 합친 커밋, 141파일 +24,574/−29,772). 같은 단위(`U-incgfx-9881`)의 뒤 seq: 501 #10036, 502 #10116, 503 #10561, 505 #10184, 507 #10334.
- 혼자 진행한 근거: 빌드 체계 변경(문장·동작 0). 판정 기준을 **ROM 바이트 같음**으로 두고 그대로 확인됨. HnS 그래픽·폰트 규칙 보존은 계획서(`g5_general_cleanup_docs_plan.md` 4절, tsv 9881 행) 그대로.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 500 | #9881 | `2bfd086c` | preproc·scaninc의 `INCGFX("원본", ".확장자", "인자")`, Makefile `ASSETS_DIR_NAME`(`build/assets`)·`-g`, `include/global.h` IDE 정의, mid2agb, `migration_scripts/1.15/migrate_incgfx.py`, 문서 3개. HnS 트리 전체 C 소스를 이관(INCGFX 25,208곳), 규칙 파일 정리, warncheck iCCP 허용 | 32,960,436(같음) |

이식 전(`04e2cfd6`, 게임 코드 = `12f11105`) SHA1 `5ffecc87…` → 이식 후 **SHA1 `5ffecc87…` 같음(바이트 같음)**.

## 내용과 HnS 판단

- **방법(파일 병합 대신 스크립트 재실행):** upstream 커밋의 C 변경은 `migrate_incgfx.py`의 기계적 결과다. 확인 — upstream 기반 트리(`git archive`, 생성물 없음)에 스크립트를 돌리면 upstream 결과와 2파일만 다름(손으로 고친 것: `src/data/graphics/pokemon.h`의 `INCGFX_COMP` 별칭과 주석 처리된 줄 297개, `src/graphics.c` `gMoveTypes_Gfx`). 그래서 HnS도 깨끗한 내보내기 트리에서 **HnS 규칙을 지우기 전에** 스크립트를 HnS 전체 C 소스(HnS 전용 그림·Soulgold UI·HnS 타일셋 포함)에 돌리고, upstream의 손 고침을 패치로 덧붙임(실패 2곳은 주석 줄 — 한카리아스 메가Z OW 3줄은 손으로 같게, 자루도 대장은 HnS가 이미 켠 줄이라 그대로). `test/compression/smol.c`는 HnS = 기반이라 upstream 그대로.
- **도구 3-way 병합:** Makefile·`include/global.h`·tools·문서는 충돌 0, `tools/preproc/c_file.cpp`·`c_file.h` 각 1곳. HnS preproc는 COMPOUND_STRING(seq 65 #9086)으로 출력을 `CFile::printf`에 모아 두므로 upstream이 새로 넣은 `std::printf` 4곳(INCBIN·INCGFX 출력)을 멤버 `printf`로(그대로면 출력 순서가 섞임). 1.17.1 upstream도 같은 형태.
- **규칙 정리(재확인 109):** 규칙 파일은 HnS 규칙이 사이사이 끼어 3-way 병합이 거의 안 지워짐 → 정리 도구 `dev_scripts/hns_verify/incgfx/prune_rules.py`를 만듦: 아직 쓰는 규칙만 남김(C `INCBIN` 인자, asm `.incbin`, 다른 make 파일의 선행 파일, 남은 규칙의 선행 파일, 압축 접미사 `.smolTM`·`.smol`·`.lz`·`.rl`을 따라감) + upstream이 남긴 기반 규칙 그대로 유지. upstream 기반에 돌려 upstream 결과와 같은지(빈 줄·빈 주석 1줄 빼고) 먼저 확인.
  - `graphics_file_rules.mk` 764 → 379줄, `spritesheet_rules.mk` 6,792 → 37줄.
  - HnS가 남긴 규칙: `data/fonts.s`가 `.incbin`하는 폰트 15개(한글 `font0/1/2/7/8_korean.latfont` 5 + 라틴 5 + 일본어 5)와 `FONTGFXDIR`(upstream은 폰트 규칙을 지움), 포케기어 옵션 합치기(`pokenav/hns/options/options.4bpp`와 `PKNAVGFXDIR`), 타이틀 로고 팔레트(`title_screen/hns/pokemon_logo.gbapal`, `-num_colors 240`), 금·크리스 걷기·달리기 4개(여러 파일 INCBIN).
  - 지운 HnS 규칙 예: `font9.hwjpnfont`(`src/text.c`가 INCGFX로 바뀌어 규칙 불필요), 오버월드·포켓몬·타일셋 규칙(모두 INCGFX 인자로 옮겨짐).
  - 남은 INCBIN은 압축 타일맵 `.bin.smolTM`(seq 501 #10036이 바꿈)·합치기 결과·여러 파일 INCBIN·메타타일 — upstream과 같은 종류.
- **바뀌는 빌드 동작:** 그림은 이제 `build/assets/`에 변환(예: `build/assets/graphics/pokemon/…/front.png.4bpp.smol`). 저장소 `graphics/` 안의 옛 생성물(`*.4bpp` 등, git 무시)은 남은 INCBIN 대상만 계속 쓰임. PNG를 고치면 해당 자산만 다시 변환됨(확인).
- **도구 경고:** 모든 PNG를 빌드 때 변환하면서 libpng `iCCP: known incorrect sRGB profile`이 보임(PNG 메타데이터, upstream PNG와 HnS 오버월드 PNG 일부) → `warncheck.sh`에서 기존 `bKGD`처럼 허용.
- **재확인 106 정정:** `create_electric_puff_sprite` 매크로는 #9881에 없음 — upstream 커밋 `76799d84eb`가 처음 들어간 PR은 seq 516 #9986(`3ea7f0d71b`) → 516에서 정리.
- 뒤 seq 참고: 502 #10116(한카리아스 메가Z INCGFX)는 HnS가 이미 켠 줄을 이번 스크립트가 바꿔서 같은 결과, 503 #10561(찌르호크 메가 아이콘)은 주석 줄이 남아 있음.
- 세이브: 영향 없음.

## 검증

- **빌드:** `make hns -j8` 종료 0. ① 생성물 없는 깨끗한 내보내기 트리 전체 빌드(빠진 규칙이 옛 생성물에 가려지지 않게) → SHA1 `5ffecc87…`, ② 저장소 `build/hns` 지우고 전체 재빌드 → 같은 SHA1, 새 컴파일 경고 0(`warncheck.sh`, iCCP 허용 후). `build/assets` 자산 32,164개.
- **ROM 바이트 비교:** 이식 전 클론(`04e2cfd6`) 빌드 SHA1 `5ffecc87b61f14d079450f0d2886e83ef5117178`과 **같음**.
- **증분 빌드:** PNG 3개(`staraptor/mega/front.png`·`font0_korean.png`·`gold/walking_hns.png`)를 건드리면 각각 `build/assets`·폰트·합치기 규칙으로 다시 변환되고 ROM 같음.
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `2bfd086c`): 목록이 `test-baseline-seq492.txt`와 **바이트 같음**(INVALID 24, 새 PASS 0, 사라진 PASS 0).
- **한글 테스트:** 새로 쓰지 않음(ROM 바이트 같음).
- **세이브 정적 비교**(이전 = 클론 `04e2cfd6` 빌드): **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s500 8`): **555/652, `판정: PASS (기대와 같음)`, 요약 바이트 같음**.
- **ROM**(커밋 `2bfd086c`): SHA1 `5ffecc87b61f14d079450f0d2886e83ef5117178`(묶음 65와 같은 파일), 사용 32,960,436 B, 파일 33,554,432 B, 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 새 후보 없음**(ROM이 묶음 65와 바이트 같음). 대신 친구 PC에서 **새 clone 빌드 확인** 부탁 — `make hns -j8`이 되는지, SHA1이 같은지(빌드 체계가 바뀌었으므로).

## 다음

- seq 501 #10036(압축 타일맵 INCGFX, HnS 전용 타일맵 포함) — 같은 도구로 규칙 다시 정리. 목표 1.17.1.
