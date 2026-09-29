# HnS Safari 배틀 UI 한글화 버그 2건 수정 보고

- 작업 위치: `<worktree>/` (브랜치 `fix/safari-ui`, 시작 `1364cb18fb`)
- 커밋 (메인 브랜치에 merge `4160fc31f0`)
  - `6fe11839e4` 버그 1: 남은 사파리볼 수 어순 (`개 남음30` → `30개 남음`)
  - `ebb94a174d` 버그 2: `사파리볼` 라벨 위 흰 픽셀 줄
- 성격: full-sync port와 무관하다. 실기 확인 중 발견한 기존 HnS Safari 한글화·UI 버그다.
- 수정 파일: `src/battle_interface.c` 하나. 한글 문자열(`battle_message.c`), 그래픽, 공용 헬퍼 함수는 바꾸지 않았다.
- 메인 브랜치 병합과 STATUS·SESSION_LOG 기록은 메인이 했다(`4160fc31f0`).

## 1. 원인

### 버그 1: `개 남음30`

- `src/battle_message.c:1513`
  `gText_SafariBallLeft = "{HIGHLIGHT DARK_GRAY}{ENG}개 남음$" "{HIGHLIGHT DARK_GRAY}"`
  이 문자열은 pokeemerald-kr에서 가져왔다. kr은 숫자를 먼저 만들고 이 문자열을 뒤에 붙인다.
- HnS의 `UpdateLeftNoOfBallsTextOnHealthbox`는 영어판 순서(`"Left: "` + 숫자)를 그대로 썼다. `StringCopy(text, gText_SafariBallLeft)` 다음에 `ConvertIntToDecimalStringN`로 숫자를 붙였다. 그래서 화면에 `개 남음30`이 나왔다.
- 문자열은 `UseGen4BattleUI()` 분기 전에 만든다. 그래서 배틀 UI 3세대(OFF)와 4세대(ON) 모두 같은 증상이었다.

### 버그 2: `사파리볼` 위 흰 픽셀 줄 (배틀 UI 3세대 = `newBattleUI` OFF에서만 발생)

흰 줄은 `CopyGlyphToVRAM()` 안의 `GLYPH_COPY()`(`src/text.c:787`)가 칠한다. 칠한 행은 한글 글리프 셀의 0행과 1행이다. 이 두 행이 스프라이트 y=3, y=4에 색 2로 쓰였다.

1. `src/battle_message.c:1512` `gText_SafariBalls = "{HIGHLIGHT DARK_GRAY}사파리볼"`
   바이트는 `FC 02 02 3B 95 3F C0 3A 31 3B 0C FF`이다. 한글화 커밋 `1821fd6749`에서 kr 문자열로 바뀌었다. HnS 영어 원문은 `"SAFARI BALLS"`였고 HIGHLIGHT가 없었다.
2. `src/text.c:1423` `EXT_CTRL_CODE_HIGHLIGHT` 처리: `color.background = color.accent = 2`
3. `src/text.c:2240` `DecompressGlyph_Small`: 한글 글리프는 8×13(`font0_korean.png`)이다. `GenerateFontHalfRowLookupTable` 기준으로 글리프 값 0은 bg, 1은 fg, 2는 shadow, 3은 accent가 된다.
   `사/파/리/볼` 네 글리프 모두 0행은 bg(0), 1~3행은 box(3→accent), 12행은 box다. HIGHLIGHT 때문에 이 행들이 모두 색 2가 된다.
4. `GLYPH_COPY`는 0이 아닌 nibble을 모두 쓴다. 마스크를 `pixels != 0`으로 만든다. bg가 0이면 투명이라 쓰지 않지만, 여기서는 2라서 쓴다.
5. 수정 전 `UpdateSafariBallsTextOnHealthbox`의 3세대(else) 분기는 `AddSpriteTextPrinterParameterized6(healthboxSpriteId, FONT_SMALL, 16, 3, …, gText_SafariBalls)`로 healthbox 스프라이트에 직접 그렸다. 글리프 0~12행이 스프라이트 y=3~15, x=16~47(8px × 4글자)에 찍혔다.
   호출 경로는 `AddTextPrinter → RenderFont → RenderText → PrintGlyph → CopyGlyphToVRAM(SPRITE_TEXT_PRINTER) → GLYPH_COPY`다.
6. `graphics/battle_interface/hns/healthbox_safari.png` 윗부분과 실제 팔레트(`hns/ball_status_bar`)는 다음과 같다.

   | y | 색 번호 | 색 | 의미 |
   | --- | --- | --- | --- |
   | 2 | 6 | (82,106,98) | 프레임 바깥선 |
   | **3** | **7** | **(32,57,0)** | **프레임 진한 선** |
   | **4** | **3** | **(222,214,222)** | **프레임 음영** |
   | 5 이상 | 2 | (255,255,255) | 내부 흰색 |

   글리프 0행(bg)이 y=3을, 1행(box)이 y=4를 x=16~47에서 색 2(흰색)로 덮었다. 모두 64픽셀이다. y=3의 진한 선이 흰색으로 끊기므로 눈에 보이는 흰 줄이 된다. 글리프 2~3행(y=5~6)은 원래 내부 흰색이라 차이가 없다.

영어 HnS, pokeemerald-kr, 4세대 분기에서 이 문제가 없던 이유:

- 영어 HnS: 문자열에 HIGHLIGHT가 없다. bg와 accent가 0(투명)이므로 0~3행을 쓰지 않는다.
- pokeemerald-kr과 HnS 4세대 분기: 8×2 타일 창을 색 2로 먼저 채우고 거기에 그린다. 그 뒤 `TextIntoHealthboxObject`가 창의 **5~15행만** 스프라이트로 복사한다(윗줄 타일은 `+20` 바이트부터 12바이트, 즉 5~7행). 창의 0~4행은 복사하지 않으므로 healthbox 0~4행의 프레임이 남는다. 4세대 그래픽도 0~4행이 프레임이고 5행부터 내부 색 2다.

## 2. 수정 내용

### 커밋 `6fe11839e4` 버그 1

```c
    u8 text[24];
    u8 *txtPtr;

    // Korean word order, as in pokeemerald-kr: number first, then gText_SafariBallLeft ("개 남음").
    txtPtr = ConvertIntToDecimalStringN(text, gNumSafariBalls, STR_CONV_MODE_LEFT_ALIGN, 2);
    StringAppend(txtPtr, gText_SafariBallLeft);
```

- 분기 앞에서 문자열을 만든다. 그래서 3세대와 4세대 두 경로에 모두 적용된다.
  - 4세대: 기존 `GetStringRightAlignXOffset(FONT_SMALL, text, 0x2F)` 오른쪽 정렬 + `SafariTextIntoHealthboxObject`를 그대로 쓴다. kr과 같은 구조다.
  - 3세대: 기존 `FillSpriteRectColor(healthboxSpriteId, 55, 19, 40, 12, HEALTHBOX_BG_INDEX)`와 x=55 왼쪽 정렬을 그대로 둔다. `#9882`/`#9912`의 지우기 폭 40을 유지한다.
- 버퍼는 16에서 24로 늘렸다. kr과 같은 크기다. 현재 최대 사용량은 15바이트다.
- kr이 숫자 앞에 넣는 `EXT_CTRL_CODE_JPN`은 넣지 않았다. 3절에서 이유를 설명한다.

### 커밋 `ebb94a174d` 버그 2

```c
static void UpdateSafariBallsTextOnHealthbox(u8 healthboxSpriteId)
{
    u32 windowId, spriteTileNum;
    u8 *windowTileData;

    // (주석 생략)
    windowTileData = AddTextPrinterAndCreateWindowOnHealthbox(gText_SafariBalls, UseGen4BattleUI() ? 2 : 0, 3, HEALTHBOX_BG_INDEX, &windowId, FALSE);
    spriteTileNum = gSprites[healthboxSpriteId].oam.tileNum * TILE_SIZE_4BPP;
    TextIntoHealthboxObject((void *)(OBJ_VRAM0 + 0x40) + spriteTileNum, windowTileData, 6);
    TextIntoHealthboxObject((void *)(OBJ_VRAM0 + 0x800) + spriteTileNum, windowTileData + 0xC0, 2);
    RemoveWindowOnHealthbox(windowId);
}
```

- 3세대 분기도 kr과 4세대가 쓰는 창 경로를 쓴다.
  - 창을 `HEALTHBOX_BG_INDEX`(2)로 먼저 채운다.
  - FONT_SMALL로 y=3에 그린다.
  - `TextIntoHealthboxObject`로 5~15행만 복사한다.
- 4세대: x=2, y=3, bg 2, 복사 대상(0x40 6타일, 0x800 2타일)이 이전과 같다. 동작 변화가 없다.
- 3세대: 창 x=0은 healthbox x=16(타일 2)에 놓인다. 이전 스프라이트 프린터 위치(16,3)와 같다. 그래서 글자가 그려지는 5~15행은 픽셀 단위로 이전과 같다. 0~4행은 더 이상 쓰지 않는다.
- y좌표를 옮기지 않았고 한글 문자열도 바꾸지 않았다.
- 검토 후 쓰지 않은 대안:
  - y=4로 이동: 요청에서 금지했다. 원인도 남는다.
  - 문자열에서 `{HIGHLIGHT DARK_GRAY}` 제거: 한글 문자열 변경이다. kr과도 어긋난다. 글리프 셀 내용에 계속 의존하게 된다.

## 3. pokeemerald-kr와 비교

| 항목 | pokeemerald-kr | HnS 수정 전 | HnS 수정 후 |
| --- | --- | --- | --- |
| 라벨 문자열 | `{HIGHLIGHT DARK_GREY}사파리볼` | 같음(`DARK_GRAY`) | 변경 없음 |
| 라벨 그리기 | 창 8×2타일, FONT_SMALL, 색 2 채움, x=0 y=3 → `TextIntoHealthboxObject`(5~15행) → 0x40(6타일), 0x800(2타일) | 4세대: kr과 같고 x=2 / 3세대: 스프라이트 프린터 (16,3), 채움 없음, 0~12행 전부 씀 | 두 UI 모두 kr 창 경로. x는 4세대 2, 3세대 0 |
| 남은 개수 문자열 | `JPN` + 숫자 + `StringAppend(gText_SafariBallLeft)`, `text[24]` | `gText_SafariBallLeft` + 숫자, `text[16]` | 숫자 + `StringAppend(gText_SafariBallLeft)`, `text[24]`, JPN 없음 |
| 남은 개수 그리기 | 창 y=3, 오른쪽 정렬 0x2F, `SafariTextIntoHealthboxObject`(0~15행) → 0x2C0(2타일), 0xA00(4타일) | 4세대: kr과 같음 / 3세대: `FillSpriteRectColor(55,19,40,12,2)` + 스프라이트 프린터 (55,19) | 그리기 경로는 그대로 두고 어순만 바뀜 |
| 헬퍼 `TextIntoHealthboxObject` / `SafariTextIntoHealthboxObject` | 원본 | kr과 동일 | 변경 없음 |
| 창 생성 `AddTextPrinterAndCreateWindowOnHealthbox` | 색 {bg,1,3}, 폰트 0 | 폰트 인자와 `isHP`가 추가됨(`isHP=FALSE`면 kr과 같은 색) | 변경 없음 |

JPN을 넣지 않은 이유:

- kr의 JPN은 바닐라 레벨·HP 숫자처럼 일본어 폰트 숫자를 쓰는 관례를 따른 것이다.
- HnS에서 JPN 모드면 FONT_SMALL 숫자가 `gFontSmallJapaneseGlyphs`로 바뀌고 폭이 8px로 고정된다(`GetGlyphWidth_Small`). `30개 남음`의 폭이 37px에서 43px로 늘어난다. 그러면 3세대 경로에서 지우기 영역 55~94(40px)을 넘는다(55~97).
- 문자열 안에 `{ENG}`가 있으므로 한글 부분은 JPN 여부와 상관없이 정상이다.

## 4. 문자열 바이트 확인

확인 방법:

- 작은 호스트 C 프로그램을 만들었다.
- 함수는 `src/string_util.c`의 실제 `StringCopy`, `StringAppend`, `ConvertIntToDecimalStringN`, `sPowersOfTen` 소스를 `sed`로 잘라 넣었다.
- `sDigits`와 `gText_SafariBallLeft`는 빌드한 `pokehns.elf` 심볼 주소로 `pokehns.gba`에서 읽었다.
- 호출문은 `battle_interface.c`의 새 두 줄을 그대로 가져왔다.

ROM의 원본 문자열:

- `gText_SafariBalls` = `FC 02 02 3B 95 3F C0 3A 31 3B 0C FF`
- `gText_SafariBallLeft` = `FC 02 02 FC 16 37 13 00 38 3D 3D 63 FF FC 02 02 FF`
  - 첫 `FF` 뒤의 `FC 02 02 FF`는 `StringAppend`가 복사하지 않는다.

| 값 | 결과 바이트 (EOS 포함 길이) | 표시 | 폭 | 3세대 x (지우기 55~94) | 4세대 창 x → healthbox x |
| --- | --- | --- | --- | --- | --- |
| 30 | `A4 A1 FC 02 02 FC 16 37 13 00 38 3D 3D 63 FF` (15) | 30개 남음 | 37 | 55~91 | 10 → 58~94 |
| 29 | `A3 AA FC 02 02 FC 16 37 13 00 38 3D 3D 63 FF` (15) | 29개 남음 | 37 | 55~91 | 10 → 58~94 |
| 10 | `A2 A1 FC 02 02 FC 16 37 13 00 38 3D 3D 63 FF` (15) | 10개 남음 | 37 | 55~91 | 10 → 58~94 |
| 9 | `AA FC 02 02 FC 16 37 13 00 38 3D 3D 63 FF` (14) | 9개 남음 | 32 | 55~86 | 15 → 63~94 |
| 1 | `A2 FC 02 02 FC 16 37 13 00 38 3D 3D 63 FF` (14) | 1개 남음 | 32 | 55~86 | 15 → 63~94 |
| 0 | `A1 FC 02 02 FC 16 37 13 00 38 3D 3D 63 FF` (14) | 0개 남음 | 32 | 55~86 | 15 → 63~94 |

- 바이트 해석:

  | 바이트 | 뜻 |
  | --- | --- |
  | `A1`~`AA` | 숫자 0~9 |
  | `FC 02 02` | HIGHLIGHT DARK_GRAY |
  | `FC 16` | ENG |
  | `37 13` | 개 |
  | `00` | 공백 |
  | `38 3D` | 남 |
  | `3D 63` | 음 |
  | `FF` | EOS |

- 수정 전 30의 바이트는 `FC 02 02 FC 16 37 13 00 38 3D 3D 63 A4 A1 FF`였다. 이것이 `개 남음30`으로 보였다.
- 폭 계산: 라틴 small 숫자 5px, 공백 3px, 한글 8px(`GetGlyphWidth_Small`, `GetStringWidth`). 가장 긴 텍스트는 37px이다. 지우기 폭 40 안에 들어간다.

## 5. 픽셀 수준 검증 (호스트 시뮬레이션, 에뮬레이터 아님)

코드 동작을 Python으로 옮겨 실제 그래픽·폰트·ROM 문자열로 돌렸다.

- 옮긴 동작: `GenerateFontHalfRowLookupTable`, HIGHLIGHT 처리, `GLYPH_COPY`의 0 아닌 nibble만 쓰기, 창 채우기, `TextIntoHealthboxObject`(5~15행), `SafariTextIntoHealthboxObject`(0~15행), `FillSpriteRectColor`
- 입력: `hns/`·`gen4/` `healthbox_safari.png`, `latin_small.png`, `font0_korean.png`, 폭 표 `src/fonts.c`, ROM 문자열
- 좌우 스프라이트(`tileNum`, `tileNum+64`, 각 64×64)는 폭 128인 캔버스 하나로 모델링했다.

| 확인 항목 | 3세대 (OFF) | 4세대 (ON) |
| --- | --- | --- |
| 수정 전 라벨이 0~4행을 바꾼 픽셀 | **64** (y=3·4, x=16~47, 색 7→2, 3→2) | 0 |
| 수정 후 라벨이 0~4행을 바꾼 픽셀 | **0** | 0 |
| 라벨 5~15행, 수정 전과 후 차이 | 0 (글자 모양·위치 동일) | 0 |
| 남은 개수 30→29→28→11→10→9→8→1→0 연속 갱신 후 새로 그린 결과와 다른 픽셀(잔상) | 0 | 0 |
| 남은 개수 갱신이 내부색(2) 밖 픽셀을 덮은 수 | 0 | 0 |

- 3세대에서 10 다음 9가 되면 텍스트 끝이 91에서 86으로 줄어든다. 87~94는 `FillSpriteRectColor`가 19~30행을 지운다. 31행에 오는 글리프 12행은 box다. 한글은 HIGHLIGHT로 색 2가 되고, 숫자는 투명이라 쓰지 않는다. 그래서 31행은 항상 내부색 2다.
- 4세대는 창(48×16)을 통째로 다시 복사한다. 그래서 잔상이 생길 수 없다.

## 6. 일반 배틀 HP UI 회귀 여부

- 바꾼 함수는 `UpdateSafariBallsTextOnHealthbox`, `UpdateLeftNoOfBallsTextOnHealthbox` 둘이다. 모두 `static`이다.
- 두 함수는 `UpdateHealthboxAttribute`에서 `HEALTHBOX_SAFARI_ALL_TEXT` / `HEALTHBOX_SAFARI_BALLS_TEXT`일 때만 호출된다.
- 이 elementId를 넘기는 곳은 Safari 경로뿐이다.
  - `battle_controller_safari.c:405`
  - `reshow_battle_screen.c:393`: `BATTLE_TYPE_SAFARI`일 때만
  - `battle_controllers.c:2736`: `IsControllerSafari`일 때만
- 공유 헬퍼는 수정하지 않았다: `AddTextPrinterAndCreateWindowOnHealthbox`, `TextIntoHealthboxObject`, `SafariTextIntoHealthboxObject`, `FillSpriteRectColor`, `AddSpriteTextPrinterParameterized6`, HP 숫자·바 함수(`UpdateHpTextInHealthbox`, `PrintHpOnHealthbox`, `MoveBattleBar` 등)
- diff는 위 두 함수 안에만 있다. 따라서 코드상 일반 배틀 HP UI와 공유하는 변경은 없다.

## 7. 빌드와 테스트

- `git diff --check`: 두 커밋 모두 통과
- 빌드 1: 커밋 1 직후, 이 worktree의 첫 전체 빌드. `GITHUB_ACTION=1 make hns -j6`
  - 종료 코드 0
  - ROM 32,713,060 B (97.49%), EWRAM 248,924 B (94.96%), IWRAM 25,516 B (77.87%)
  - 로그: `build/localization-logs/hns-safari-fix1-20260930-004225.log`
- 빌드 2: 커밋 2 직후, 증분 빌드. 같은 명령
  - 종료 코드 0
  - ROM **32,712,980 B (97.49%, −80 B)**, EWRAM 248,924 B (94.96%), IWRAM 25,516 B (77.87%)
  - 로그: `build/localization-logs/hns-safari-fix2-20260930-004523.log`
  - `src/battle_interface.c` 경고 0건
- `make check BUILD=hns -j4`: 커밋 2 상태에서 실행
  - **PASS 2,298 / FAIL 2,229 / TOTAL 5,197**
  - KNOWN_FAILING 8, ASSUMPTIONS_FAILED 38, TO_DO 618, EXPECT_FAILING 6
  - 기대값(PASS 2,298 / FAIL 2,229 / TOTAL 5,197)과 같다.
  - `make` 종료 코드 2는 기존 FAIL 때문이며 기준선과 같다.
  - Safari를 다루는 테스트는 없다. 이 테스트는 무회귀 확인용이다.
  - 로그: `build/localization-logs/hns-safari-check-20260930-004620.log`

## 8. 친구 mGBA 확인 체크리스트

ROM: `<worktree>/pokehns.gba` (커밋 `ebb94a174d` 빌드). 옵션 `배틀 UI`에서 `3세대`는 `newBattleUI` OFF(기본값), `4세대`는 ON이다.

도움말: `gNumSafariBalls`는 EWRAM `0x02038550`(1바이트)이다. mGBA 메모리 뷰어에서 `0x0A`(10)로 바꾼 뒤 공을 한 번 던지면 10→9 전환을 바로 볼 수 있다.

- [ ] **흰 픽셀 없음, 3세대(OFF)**: 사파리 배틀에 들어가면 `사파리볼` 글자 위 x=16~47 구간에서도 상단 프레임 진한 선이 끊기지 않고 이어진다.
- [ ] **흰 픽셀 없음, 4세대(ON)**: 원래 문제가 없던 경로다. 라벨 위치(2px 들여쓰기)와 프레임이 전과 같다.
- [ ] **라벨 모양**: 3세대에서 `사파리볼`의 글자 위치·모양이 수정 전과 같다. 흰 줄만 없어지고 글자는 옮겨지지 않았다.
- [ ] **30/29/9개 남음**: 진입 시 `30개 남음`, 한 번 던지면 `29개 남음`, 계속 던져 `9개 남음`이 나온다. 두 UI 모두 확인한다.
- [ ] **두 자리→한 자리 잔상 없음** (10→9):
  - 3세대: `9개 남음`이 x=55부터 왼쪽 정렬로 다시 그려진다. `개 남음`이 5px 왼쪽으로 옮겨지는 것은 정상이다. 오른쪽 끝(이전 `음` 자리)에 글자·그림자 잔상이 없어야 한다.
  - 4세대: 오른쪽 정렬이라 `개 남음`은 고정된다. 앞자리 숫자 잔상이 없어야 한다.
- [ ] **clear 폭 유지**: 3세대에서 `30개 남음`(37px)이 지우기 영역 40px 안에 있다. 오른쪽 프레임을 침범하지 않고, 폰트 그림자 잔상(#9912 증상)도 없다.
- [ ] **재표시 경로**: 포켓몬스넥 등 메뉴에서 배틀 화면으로 돌아왔을 때(`reshow_battle_screen`) 라벨과 개수가 다시 정상으로 그려진다. 흰 줄이 다시 생기지 않는다.
- [ ] **(가능하면) 0개 남음**: 마지막 공을 던진 뒤 표시가 갱신되면 `0개 남음`으로 나온다. 배틀이 바로 끝나면 생략해도 된다.
- [ ] **일반 HP UI 회귀 없음**: 일반 야생·트레이너 배틀(싱글·더블)에서 HP 숫자, HP 바, 레벨, 이름이 3세대와 4세대 모두 전과 같다.
- [ ] **UI 전환**: `배틀 UI` 옵션을 3세대/4세대로 바꿔 사파리 배틀에 각각 한 번 이상 들어간다.

## 9. 재현 자료

- 호스트 바이트 확인 스크립트와 시뮬레이션은 세션 스크래치 디렉터리(`<tmp>`, `sim_safari.py`)에 있다. 사라질 수 있다.
- 다시 만들 때는 4절과 5절에 적은 입력과 동작을 그대로 쓰면 된다.
