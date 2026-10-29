# full-sync 묶음 73 — seq 507 (#10334 콘테스트 부문 값을 표로)

- 요청: 사용자 "ㄱㄱ 507 시작해 혼자해도 되는거라면"(2026-10-29).
- upstream: 읽기 전용 사본, #10334 `d2d594e7a5`(1.17.0, 5파일 +96/−272). 단위 `U-incgfx-9881`.
- 혼자 진행한 근거: 동작 같은 정리 + upstream 결함 1곳은 이식 전 동작으로 되돌림(지침 "동작을 의도 없이 바꾸지 않는다"). 새 문장 0. 콘테스트는 HnS 상록시티 콘테스트 로비로 닿으므로 **실기 후보**.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 507 | #10334 | `9dbdb90d` | 부문별 switch 5종(리본, 그림 액자·효과, 결과 타이틀, 부문 문자열, TV 콘테스트 상태)을 `gContestCategoryInfo` 표 필드로. HnS 보정 1줄(그림 효과). 새 테스트 `test/hns_10334_contest_values.c` | 32,976,196(−800) |

이식 전(`74bff041`, 게임 코드 = `d4fc70b2`) SHA1 `54c559b7…` → 이식 후 SHA1 **`29ee420b58ea763a1b94bfbb2ab32ae925ced93f`**.

## 내용과 HnS 판단

- **병합:** 5파일 3-way 충돌 0. HnS가 손댄 곳(`tv.c` 조토 TV 메타타일·비밀기지 TV 정적 변수·본 포켓몬 고르기, `contest.c` 콘테스트 끝 VBlank 정리)은 이 PR이 바꾸는 자리와 안 겹침. 부문 이름은 `gText_Cool` 등 같은 포인터라 한글("근사함" 등) 그대로.
- **HnS 보정(재확인 111, upstream 결함 — 1.17.1·master·upcoming 모두 같음):** upstream `GetImageEffectForContestWinner`가 `.imageEffect`가 아니라 `.stdString`(0~4)을 돌려줌 → 콘테스트 그림 효과가 근사함·아름다움·슬기로움·강인함에서 정의 안 된 값(0·1·3·4)으로 바뀜(귀여움만 `IMAGE_EFFECT_POINTILLISM` = `STDSTRING_CUTE` = 2로 우연히 같음). HnS는 `.imageEffect`를 돌려줌 = 이식 전과 같은 효과.
- **그대로 둔 upstream 변경:** 박물관 액자 타일맵을 VRAM으로 풀 때 `DecompressDataWithHeaderVram` → `…Wram`. 액자 타일맵은 smolTM이라 두 함수 모두 `SmolDecompressTilemap`으로 같은 경로(LZ77만 다름) → 동작 같음. 결과 타이틀 팔레트 `palette = gSpecialVar_ContestCategory`도 이식 전 0~4 대응과 같음. `DoesContestCategoryHaveMuseumPainting`의 `- 1` 위치 이동도 같은 인덱스.
- 세이브: 영향 없음(리본 비트·`contestWinners` 인덱스 같음).

## 검증

- **빌드:** `make hns -j8` 종료 0. 저장소 전체 재빌드(`build/hns` 지우고): 종료 0, SHA1 `29ee420b…`(첫 빌드와 같음), 새 컴파일 경고 0(`warncheck.sh`).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `9dbdb90d`): 기준 `test-baseline-seq504.txt` 대비 **새 PASS 2(아래 한글 테스트), 사라진 PASS 0**, 그 밖 줄은 같음(PASS 3,010). 새 기준 **`test-baseline-seq507.txt`**(`verify_seq.sh`도 이 기준으로).
- **한글 테스트** `test/hns_10334_contest_values.c`(`TESTS='HnS 10334'`) 2개, 이식 후 2/2 PASS: ① 5부문 표 값(리본·그림 효과·결과 타이틀·문자열 인덱스·문자열 포인터·TV 상태 2종)이 지워진 switch의 값과 같음 — 이식 전 코드에는 표 필드가 없어서 이 테스트는 지워진 switch 값을 그대로 옮겨 적은 것과 비교, ② `CopyContestCategoryToStringVar`가 한글 부문 이름("근사함"·"아름다움"·"귀여움"·"슬기로움"·"강인함")을 넣음 — 이식 전 코드에서도 PASS(이식 전 클론 `74bff041`에 ②만 임시 파일로 넣어 확인, 지움).
- **세이브 정적 비교**(이전 = 클론 `74bff041` 빌드): **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s507 8`): **555/652, `판정: PASS (기대와 같음)`, 요약 바이트 같음**.
- **ROM**(커밋 `9dbdb90d`): SHA1 `29ee420b58ea763a1b94bfbb2ab32ae925ced93f`, 사용 32,976,196 B(묶음 72 대비 −800), 파일 33,554,432 B, 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 하지 않음(친구) — 콘테스트 후보(상록시티 콘테스트 로비):**
  1. 콘테스트 결과 화면 위쪽 부문 타이틀 그림·색(근사함/아름다움/귀여움/슬기로움/강인함).
  2. 우승 뒤 리본 받기, 같은 랭크 다시 참가 때 "이미 이겼다" 판정.
  3. 우승 그림(화가) — 부문별 액자와 그림 효과(이식 전과 같아야 함).
  4. TV 콘테스트 방송 문장의 부문 이름("근사함" 등).

## 다음

- seq 508 #9892(Misc Fixes/Cleanups, L, 1.16.1, `U-assert-9892`), 509 #10156(`fatal_assertf`). 목표 1.17.1.
