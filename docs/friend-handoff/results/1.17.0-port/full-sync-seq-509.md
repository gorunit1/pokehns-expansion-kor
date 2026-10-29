# full-sync 묶음 75 — seq 509 (#10156 `fatal_assertf`)

- 요청: 사용자 "ㄱㄱ 509 시작해 혼자해도 되는거라면"(2026-10-29).
- upstream: 읽기 전용 사본, #10156 `79e34b656d`(1.16.2, 9파일 +95/−26). 단위 `U-assert-9892`(seq 508 다음).
- 혼자 진행한 근거: 디버그 검사 종류 변경(새 문장 0). HnS가 이식 전부터 다르게 둔 곳 1곳은 그대로(재확인 112).

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 509 | #10156 | `9ecda50d` | `fatal_assertf`/`fatalf`(START로 못 넘기는 크래시 화면, 따로 팔레트) 추가, 메모리 부족·`CreateSprite` 슬롯·`InitWindows`·`BgTileAllocOp`을 그쪽으로. `graphics/crash_screen/palette.pal` → `assertf.pal`, 새 `fatalf.pal`. 테스트 러너 테스트 1개 | 32,980,692(+32) |

이식 전(`8859e5c0`, 게임 코드 = `d84f57bf`) SHA1 `71e7788a…` → 이식 후 SHA1 **`e71242c84ea01e72a8e58228244957fab6af1372`**.

## 내용과 HnS 판단

- **바뀌는 검사:** `Alloc`/`AllocZeroed` 메모리 부족 `errorf` → `fatalf`, `CreateSprite` 슬롯 부족·`InitWindows` 종결자/초기화 실패·`BgTileAllocOp` `assertf` → `fatal_assertf`. 이전 `make hns`(비릴리스)에서는 START로 넘길 수 있는 화면이었고, 이제는 멈춤(릴리스 빌드도). 정상 진행에서는 안 닿는 경로들:
  - `InitWindows`: seq 508에서 HnS 창 템플릿 82곳 모두 종결자 있음 확인(`port/wincheck.py`).
  - `BgTileAllocOp`: HnS에서 켜는 경로 없음(seq 508 확인).
  - 메모리 부족·`CreateSprite` 슬롯: 정상 진행에서 생기면 이전에도 assert 화면이 떴을 자리.
- **충돌 1곳 — HnS 유지(재확인 112):** HnS `CreateSpriteAtEnd`에는 이식 전부터 슬롯 검사가 없음(upstream은 #9766부터 있음). 이 PR로 넣으면 꽉 찼을 때 새로 멈추는 화면이 생겨서 이전 동작 그대로 두고 `// HnS:` 주석. 호출 22곳 중 HnS 전용 줄은 포켓몬센터 모니터 1곳.
- **팔레트:** `palette.pal`은 upstream과 줄 끝(CRLF, HnS `.gitattributes`)만 다르고 내용 같음 → HnS 파일을 그대로 이름만 바꿈. `fatalf.pal`은 upstream blob.
- 세이브: 영향 없음.

## 검증

- **빌드:** `make hns -j8` 종료 0. 저장소 전체 재빌드(`build/hns` 지우고): 종료 0, SHA1 `e71242c8…`(첫 빌드와 같음), 새 컴파일 경고 0(`warncheck.sh`).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `9ecda50d`): 목록이 `test-baseline-seq508.txt`와 **바이트 같음**(PASS 3,010, 새 PASS 0, 사라진 PASS 0). upstream 새 테스트 "fatalf counts as CRASH"는 `KNOWN_CRASHING`(예상 실패 7 → 8). 새 `fatal` 경로에 걸린 테스트 0.
- **한글 테스트:** 새로 쓰지 않음(문장 0). 한글 회귀·전체 테스트가 새 `fatal` 경로에 걸리는지가 판정.
- **세이브 정적 비교**(이전 = 클론 `8859e5c0` 빌드): **PASS(FAIL 0, WARN 2)** — seq 508과 같은 꼴: `BoxMonRestorePP` 안 assert 문자열 포인터 2개의 주소 이동(내용 같음). 세이브 블록·RAM 변수 같음.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s509 8`): **555/652, `판정: PASS (기대와 같음)`, 요약 바이트 같음**.
- **ROM**(커밋 `9ecda50d`): SHA1 `e71242c84ea01e72a8e58228244957fab6af1372`, 사용 32,980,692 B(묶음 74 대비 +32), 파일 33,554,432 B, 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기:** 화면 변화 없음. 묶음 74와 같이, 크래시 화면(이제 START로 안 넘어가는 것도 있음)이 뜨면 사진.

## 다음

- seq 510 #10155(테스트 러너 `:L`), 511 #9807(배틀 테스트 실패 제안), 512 #10390. 목표 1.17.1.
