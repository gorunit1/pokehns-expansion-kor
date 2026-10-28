# full-sync 묶음 60 — seq 494 (#9641 타이틀 화면 빠른 시작, HnS에선 끔)

- 요청: 사용자 "ㄱㄱ 494 시작해 혼자해도 되는거라면"(2026-10-28).
- upstream: 읽기 전용 사본, #9641 `9fbfb39797`(1.16.0, 11파일, +284). 같은 단위(`U-9641`)에 다른 seq 없음.
- 혼자 진행한 근거: 디버그 기능이라 사용자 지침("HnS 일반 진행에서 쓰지 않는 기능은 켜지 않는다")대로 코드만 받고 끔 — 새 결정 없음. 새 문장 0.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 494 | #9641 | `44978ff2` | 타이틀 화면에서 SELECT로 기본 이름 새 게임을 바로 시작하는 디버그 기능(`src/quickstart.c`, `include/quickstart.h`, `include/config/quickstart.h`, HUD 그림·팔레트, 문서), 에메랄드·FRLG 타이틀 화면 연결. **HnS 설정 `ENABLE_QUICKSTART FALSE`** | 32,962,756 |

이식 전(`e4ade92a`, 게임 코드 = `8da1491c`) 32,962,756 B → **32,962,756 B(같음)**.

## 내용과 HnS 판단

- **왜 끄는가:** upstream은 릴리스 빌드에서만 꺼짐(`#if RELEASE`). `make hns`는 릴리스 빌드가 아니라(`RELEASE ?= 0`, `make release`일 때만 1) 그대로면 친구 실기용 ROM에서도 켜짐 → 타이틀에서 SELECT 한 번에 HnS 인트로(성별·이름·라이벌)를 건너뛰고 영문 기본 이름(`BRENDAN`/`MAY`)으로 새 게임. HnS 타이틀 화면의 SELECT 조합(B+SELECT+↑ 세이브 삭제, B+SELECT+← 시계 초기화, B+SELECT 열매 갱신)과 SELECT를 같이 써서 조합 입력이 빠른 시작으로 샐 수도 있음. → `include/config/quickstart.h`의 `ENABLE_QUICKSTART`만 `FALSE`(`// HnS:` 주석), 나머지는 upstream 그대로.
- **적용 방법:** 새 파일 8개는 upstream 파일 그대로(blob 같음 확인), `src/title_screen.c`·`src/title_screen_frlg.c`는 3-way 병합(충돌 0 — HnS 타이틀 화면의 `IS_HNS` 부분과 겹치지 않음), `docs/SUMMARY.md`는 HnS 목록 + `Quickstart Documentation` 1줄.
- **ROM 영향 확인:** `QUICKSTART`가 컴파일 시 `FALSE`라 타이틀 화면의 호출 2곳이 빠지고, `quickstart.o`(텍스트 484 B)는 아무 데서도 쓰지 않아 링크에서 버려짐(최종 ELF에 `quickstart` 심볼 0). ROM 크기 같음, 바이트 차이 2,036 B는 링커가 만드는 ARM↔Thumb 중계 스텁(`__…_from_thumb`/`__…_from_arm`)의 배치 순서와 그 호출 주소뿐(스텁 수·크기 같음) — 동작 같음.
- 생성 그림 파일(`*.gbapal`, `*.4bpp`, `*.4bpp.smol`)은 `.gitignore` 대상이라 커밋에 없음.
- 세이브: 영향 없음.

## 검증

- **빌드:** `make hns -j8` 종료 0, 전체 재빌드(`build/hns` 지우고) 로그로 새 컴파일 경고 0(`warncheck.sh`).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `44978ff2`): 목록이 `test-baseline-seq492.txt`와 **바이트 같음**(새 PASS 0, 사라진 PASS 0, INVALID 24). 기준 파일은 그대로 seq492(새 파일 안 만듦).
- **한글 테스트:** 새로 쓰지 않음 — 타이틀 화면 입력은 배틀 테스트 러너로 재현할 수 없고, 기능이 컴파일에서 빠져 출력할 것이 없음. 대신 위 ROM·ELF 비교로 확인.
- **세이브 정적 비교**(이전 = 클론 `e4ade92a` 빌드, ROM SHA1 `116c9899…` = 묶음 59 기록과 같음): **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s494 8`): **555/652, `판정: PASS (기대와 같음)`, 요약 바이트 같음**.
- **ROM**(커밋 `44978ff2`): SHA1 `78627b4eb060e574edef5e81a3e3c5692d8eceb5`, 사용 32,962,756 B(파일 33,554,432 B), 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 하지 않음(친구).** 후보 — 타이틀 화면에서 SELECT를 눌러도 아무 일 없음(빠른 시작 HUD도 없음), B+SELECT+↑ 세이브 삭제 등 기존 조합이 그대로.

## 다음

- seq 495 #9677(기술 애니메이션 숫자를 상수로, 1.16.0). 목표 1.17.1.
