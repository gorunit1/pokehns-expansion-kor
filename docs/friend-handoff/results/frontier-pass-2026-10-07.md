# HnS 수정 — 2026-10-07 친구 요청 2부: 프런티어패스 머리 아이콘·색 깨짐

- 요청: [`FRIEND_REQUEST_2026-10-07.md`](../FRIEND_REQUEST_2026-10-07.md) 2부. 첨부(저장소 밖): `/home/hjm0725/hns-sync-work/frontierpass-1007/map_heads_hns_friend.png`, 스크린샷 `screenshot-pass-main.png`·`screenshot-pass-map.png`(같은 폴더, `.webp` 원본도 있음).
- 시작 HEAD `3d76ed478b`. 작업 컴퓨터: 데스크탑(2026-10-07 새벽). 사용자 승인: 이전 세션 분석(STATUS) 뒤 "진행해도 돼".
- 실기(mGBA) 미확인.

## 요약

| # | 커밋 | 내용 | ROM 변화 |
|---|---|---|---:|
| 1 | `00461d5cf6` | `graphics/frontier_pass/map_heads_hns.png`를 친구 첨부본(심향·금선 머리)으로 교체 | 0 |
| 2 | `364ab51c7f` | `graphics/frontier_pass/bg.png`를 upstream 구조(8bpp, 128색 팔레트)로 바꿔 배경 팔레트 8뱅크 복원. 픽셀 인덱스 그대로 | +224 B |

- 빌드(최종 `364ab51c7f`): 종료 코드 0, **ROM 32,754,564 B(+224 B) / EWRAM 250,132 B / IWRAM 25,516 B**, SHA1 `5876c53db0291538…`, 새 경고 0(`build/localization-logs/hns-20261007-024001-fpass-bgpal.log`). 커밋 1 뒤 빌드는 ROM 크기 같음(`…-023940-fpass-heads.log`, SHA1 `4a6fc394…`).
- 전체 테스트(`build/port-check-fpass.log`): 목록이 [`1.17.0-port/test-baseline-seq179.txt`](1.17.0-port/test-baseline-seq179.txt)와 **바이트 동일**(PASS 2,429 / TOTAL 5,332, INVALID 21 이전과 같음). 그래픽만 바뀌어 새 기준 목록은 만들지 않았다.
- 한글·코드 변경 없음(PNG 2개만).

## 1. 머리 아이콘

- 첨부본은 지금 파일과 크기(16×32)·팔레트(16색, 순서까지 같음)가 같고 그림만 다르다. 첨부본(8bpp PNG, 인덱스 0~15)을 그대로 넣었다. 파일 모드(755)는 기존 그대로.
- 생성물: `map_heads_hns.gbapal` 바이트 같음, `map_heads_hns.4bpp`만 달라짐(스크래치에서 gbagfx로 만든 것과 같음). `src/frontier_pass.c` 180~182행 HnS 분기(`sMaleHead_Pal`·`sFemaleHead_Pal` = `map_heads_hns.gbapal`, `sHeads_Gfx` = `map_heads_hns.4bpp.smol`)는 그대로라 남(심향)·여(금선) 모두 새 그림을 쓴다.
- 원래 파일(4bpp)의 `tIME`·`tEXt`·`tpNG`·`bKGD` 청크는 첨부본(8bpp, `gAMA PLTE IDAT`)에 없다. 빌드 경고 0.

## 2. 색 깨짐

- **원인:** HnS `bg.png`는 pokeemerald-kr `graphics/frontier_pass/tiles.png`와 바이트가 같은 4bpp·**16색 팔레트** PNG다. kr(옛 구조)은 배경 팔레트를 별도 `tiles.pal`(JASC 128색)에서 만들지만, expansion은 `bg.png`의 PLTE로 `bg.gbapal`을 만든다 → 32 B(1뱅크)뿐이었다. 코드는 `gFrontierPassBg_Pal`을 `NUM_BG_PAL_SLOTS`(BUGFIX → 8)뱅크로 읽고(`src/frontier_pass.c` 779·1423행), 뱅크 1을 `gFrontierPassBg_Pal[1 + trainerStars]`로 다시 채운다(780행). 그래서 뱅크 1~7이 ROM에서 뒤에 오는 데이터로 채워졌다.
  - 8bpp `map_and_card`(본체의 작은 맵·트레이너카드)는 팔레트 0~127 전체를 쓰고, 맵 화면도 같은 팔레트를 읽는다 → 깨짐.
  - 뱅크 0만 쓰는 틀·글자 상자·심볼·배틀포인트는 정상 → 친구 스크린샷과 같다.
- **수정:** `bg.png`를 upstream 1.17.0 `bg.png` 구조(8bpp, PLTE 128)로 다시 썼다. PLTE는 upstream `bg.png` 것을 그대로 썼고, kr `tiles.pal` 128색과 128색 모두 같다(8비트 RGB 비교). 픽셀 인덱스는 하나도 바꾸지 않았다(최대 인덱스 15, 다시 읽어 바이트 같음). 청크는 upstream처럼 `IHDR sRGB PLTE IDAT IEND`.
  - 스크립트(저장소 밖): `/home/hjm0725/hns-sync-work/frontierpass-1007/work/mkbg.py`(`python3 mkbg.py bg_new.png`)
- **확인**
  - `bg.4bpp`: 이전과 바이트 동일(→ `bg.4bpp.smol`도 같음).
  - `bg.gbapal`: 32 → 256 B. upstream `bg.png`를 같은 gbagfx로 바꾼 것, kr `tiles.pal`을 바꾼 것과 모두 바이트 동일. 뱅크 0(첫 32 B)은 이전과 같음.
  - ROM: `gFrontierPassBg_Pal`(0x086AC254, 256 B)이 upstream 팔레트와 같음(`pokehns.elf` 심볼 + ROM 바이트 비교).
- 그림 쪽 다른 파일(`map_and_card.png`, `map_screen.png`, 타일맵)은 바꾸지 않았다. 레이아웃·한글 문구 변경 없음.

## 3. 테스트 러너 렌더링 (수정 전후)

- 방법: 스크래치 사본 두 개(`render/before` = `3d76ed478b`, `render/after` = 두 PNG 교체)에 테스트(`HNSFP` 접두사)와 `#if TESTING` 계측을 넣어 패스 본체·맵 화면을 띄우고 팔레트 RAM·VRAM·OAM·GPU 레지스터를 덤프했다. 덤프 로그 `render/run-before.log`·`run-after.log`.
- 렌더링·비교 그림: 세션 종료로 정리가 끝나지 않았다. 남은 기록은 `/home/hjm0725/hns-sync-work/frontierpass-1007/render/`(`RENDER.md`가 있으면 그것을 따른다). **다음 세션에서 마무리**한다(STATUS "다음 할 일").

## 4. mGBA 확인 (친구)

- 패스 본체: 배경·작은 맵·트레이너카드 색, 배틀 기록·배틀포인트·심볼 영역 색(바뀌지 않아야 함)
- 트레이너 별 0개와 1개 이상(카드 색 뱅크 1)
- 배틀프런티어 맵 화면: 맵 그림·오른쪽 시설 목록 글자 색, 남주 심향·여주 금선 머리 아이콘
