# full-sync 묶음 78 — seq 512 (#10390 알려진 실패 `and 0 more`·출력 형식 추가)

- 요청: 사용자 "ㄱㄱ 혼자 할수있다면"(2026-10-29).
- upstream: 읽기 전용 사본, #10390 `70591aacda`(1.16.3, 4파일 +53/−8). 단위 `U-runnerinfra-10155`(seq 511 다음, 513은 묶음 77에서 앞당김 → 다음 514).
- 혼자 진행한 근거: 테스트 러너·hydra·assert 화면 출력 형식만(새 문장 0, 충돌 0). 게임 ROM은 assert 화면 코드만 바뀜.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 512 | #10390 | `9bd02555` | hydra가 "알려진 실패인데 통과" 목록 끝에 `and 0 more...`를 찍지 않게, assert 화면 `%c`(ASCII)·`%C`(게임 글자 1개), 테스트 러너 `%c`·`%C`·`%U`(게임 문자열 대문자, 그 밖 글자는 `_`), 배틀 테스트 "Did you mean" 제안의 특성·기술 이름 `%C` → `%U` | 32,980,868(+176) |

이식 전(`d4d845ef`, 게임 코드 = `139c9931`) SHA1 `e71242c8…` → 이식 후 SHA1 **`eb35fac1273b5ca925ba2d0c0c1aeb4932d38df8`**.

## 내용과 HnS 판단

- 4파일(`src/assertf.c`·`test/test_runner.c`·`test/test_runner_battle.c`·`tools/mgba-rom-test-hydra/main.c`) 모두 3-way 충돌 0. HnS 쪽에서 `%c`/`%C`/`%U`를 쓰는 곳 없음(검색).
- **ROM +176 B:** `assertf.c`는 게임에도 들어가는 assert 화면 코드 — 새 형식 2개와 `Puts`/`PutS` 검사 순서(길이 0이면 문자열을 읽지 않음). HnS에서 이 형식을 쓰는 assert는 없어 화면 출력은 그대로.
- **이전 HnS에서 실제로 고쳐진 것:** 이식 전 러너는 `%C`를 몰라 인자를 건너뛰지 않아서, 기술 애니 제안 줄 뒤쪽 배틀러 자리에 기술 이름 바이트가 찍힘 — 한글 회귀 로그 `ANIMATION(ANIM_TYPE_MOVE, MOVE_, <깨진 바이트>)` → 이식 후 `ANIMATION(ANIM_TYPE_MOVE, MOVE__, opponent)`(배틀러 바름).
- **HnS 한계(기록만):** HnS 특성·기술 이름은 한글(`부유`, `몸통박치기`)이라 `%U`가 영문자만 대문자로 찍고 나머지를 `_` 하나로 줄여 `ABILITY__`·`MOVE__`처럼 나옴 — 테스트 실패 진단 줄일 뿐 판정·게임과 무관. 1.17.1도 같은 코드.
- **HnS 주석 정리:** seq 394 때 `ITEM_POPUP` 검사에 단 "#9807이 들어오면 제안 가능" 주석이 seq 511 뒤로 낡아(#9807에도 `ITEM_POPUP` 제안은 없음) 문장만 고침(이 문서 커밋, 컴파일 결과 같음).
- 세이브: 영향 없음.

## 검증

- **빌드:** `make hns -j8` 종료 0. 저장소 전체 재빌드(`build/hns` 지우고): 종료 0, SHA1 `eb35fac1…`(첫 빌드와 같음), 새 컴파일 경고 0(`warncheck.sh`).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `9bd02555`): 목록이 `test-baseline-seq511.txt`와 **바이트 같음**(PASS 3,010, 새 PASS 0, 사라진 PASS 0, FLAKY 1 그대로). hydra 요약은 `KNOWN_FAILING` 14.
- **한글 테스트:** 새로 쓰지 않음(문장 0).
- **세이브 정적 비교**(이전 = 클론 `d4d845ef` 빌드): **PASS(FAIL 0, WARN 2)** — seq 508·509와 같은 꼴: `BoxMonRestorePP` 안 assert 문자열 포인터 2개의 주소 이동(내용 같음). 세이브 블록·RAM 변수 같음.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s512 8`): **555/652, `판정: PASS (기대와 같음)`, 상태 줄 차이 0**. 요약 파일은 hydra 실패 목록(앞 50줄, `-j` 순서에 따라 달라짐, 판정에 안 씀) 2줄만 다름 → 기대 요약은 갱신하지 않음.
- **ROM**(커밋 `9bd02555`): SHA1 `eb35fac1273b5ca925ba2d0c0c1aeb4932d38df8`, 사용 32,980,868 B(묶음 77 대비 +176), 파일 33,554,432 B, 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 새 후보 없음**(assert 화면 코드만, HnS에서 새 형식 쓰는 곳 0). 묶음 74·75처럼 크래시 화면이 뜨면 사진.

## 다음

- seq 514 #9900(Expansion 1.15.2 릴리스 표시 — 513은 묶음 77에서 앞당김). 목표 1.17.1.
