# full-sync 묶음 62 — seq 496 (#9676 주먹·발 애니메이션 매크로)

- 요청: 사용자 "ㄱㄱ 496 시작해 혼자해도 되는거라면"(2026-10-28).
- upstream: 읽기 전용 사본, #9676 `057a4095bd`(1.16.0, 2파일, +54/−42). 같은 단위(`U-9676`)에 다른 seq 없음.
- 혼자 진행한 근거: 매크로로 묶기만 하는 정리(새 문장 0), ROM이 이식 전과 바이트 같음.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 496 | #9676 | `e254fc86` | `asm/macros/battle_anim_script.inc`에 `create_fist_sprite`·`create_wide_foot_sprite`·`create_tall_foot_sprite`(모두 `gFistFootSpriteTemplate`), `data/battle_anim_scripts.s`가 그 매크로를 씀 | 32,962,756 |

이식 전(`165fb8ae`, 게임 코드 = `2fdced80`) → **ROM 바이트 같음**(SHA1 `67a944b0…` 그대로).

## 내용과 HnS 판단

- **적용 방법:** 3-way 병합(`git merge-file`) — 2파일 모두 충돌 없음.
- **동작:** 매크로가 같은 `createsprite` 인자를 내므로 같은 바이트. 이식 전 ROM(묶음 61)과 `cmp` 차이 0.
- 세이브: 영향 없음.

## 검증

- **빌드:** `make hns -j8` 종료 0, 전체 재빌드(`build/hns` 지우고) 로그로 새 컴파일 경고 0(`warncheck.sh`). ROM SHA1 `67a944b0a6484520ec1f2d6f0f7fcb5c60d8939a` = 묶음 61과 같음.
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `e254fc86`): 목록이 `test-baseline-seq492.txt`와 **바이트 같음**(INVALID 24).
- **한글 테스트:** 새로 쓰지 않음(ROM 바이트 같음).
- **세이브 정적 비교:** **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s496 8`): **555/652, `판정: PASS (기대와 같음)`, 요약 바이트 같음**.
- **ROM**(커밋 `e254fc86`): SHA1 `67a944b0a6484520ec1f2d6f0f7fcb5c60d8939a`(묶음 61과 같은 파일), 사용 32,962,756 B, 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기:** 묶음 61 ROM과 같은 파일이라 새 실기 후보 없음.

## 다음

- seq 497 #9924(공통 애니메이션 함수 추가, 1.17.0, XL). 목표 1.17.1.
