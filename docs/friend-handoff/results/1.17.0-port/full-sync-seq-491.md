# full-sync 묶음 58 — seq 491 (#10630 금제·회복봉인 중복 플래그 정리)

- 요청: 사용자 "ㄱㄱ 491 시작해 혼자해도 되는거라면"(2026-10-28).
- upstream: 읽기 전용 사본, #10630 `bab3911c35`(14파일, +43/−53). 같은 단위(`U-10630`)에 다른 seq 없음.
- 혼자 진행한 근거: 동작을 바꾸지 않는 정리(새 문장 0). upstream 그대로(HnS 보정 0), 충돌 0.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 491 | #10630 | `c311121f` | 금제·회복봉인을 타이머 하나로만 판정(`volatiles.embargo`·`healBlock` 플래그 삭제, 타이머가 배턴터치로 넘어감), 스크립트·AI·턴 끝·도구·디버그 메뉴의 판정을 타이머로, 한글 회귀 세트 1줄, 한글 회귀 테스트 | 32,962,692 |

이식 전(`e83c57fb`, 게임 코드 = `98b25dbc`) 32,962,740 B → **32,962,692 B(−48)**.

## 내용과 HnS 판단

- **적용 방법:** 파일별 3-way 병합(`git merge-file`) — 14파일 모두 충돌 없음. HnS 고유 코드·테스트에서 지워지는 이름(`volatiles.embargo`·`volatiles.healBlock`·`VOLATILE_EMBARGO`·`VOLATILE_HEAL_BLOCK`) 검색 → 한글 회귀 세트 `zz_hnspopup_mentalherb.c` 1곳(`HNSPOPUP 06` THEN의 `healBlock == FALSE`).
- **한글 회귀 세트 1줄:** `EXPECT(player->volatiles.healBlockTimer == 0);`(upstream이 같은 테스트 `Mental Herb cures Heal Block volatile status`에서 남긴 검사와 같음, 주석 `#10630 (seq 491)`). 판정 상태 변화 없음(`HNSPOPUP 06` PASS 그대로) — 기대 요약 수정 없음.
- 플래그를 타이머 없이 켜는 곳이 이식 전에 없었는지 확인: 플래그를 켜는 곳은 모두 타이머도 같이 설정(회복봉인 기술·사이코노이즈·금제) → 판정 결과 같음.
- 세이브: 배틀 중 변수(volatile 비트필드)만, 세이브 구조 변화 없음.

## 검증

- **빌드:** `make hns -j8` 종료 0, 전체 재빌드(`build/hns` 지우고) 로그로 새 컴파일 경고 0(`warncheck.sh`).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `c311121f`): `test-baseline-seq490.txt` 대비 **새 PASS 3(한글 3), 사라진 PASS 0**, 다른 줄 같음, INVALID 24(이전 실행과 같은 목록). 새 기준 [`test-baseline-seq491.txt`](test-baseline-seq491.txt)(**다음 비교 기준**).
- **새 한글 회귀 `test/hns_10630_heal_block_embargo.c` 3개:** 회복봉인 → HP회복 막힘(`마자용은 회복봉인 때문에 HP회복을 쓸 수 없다!`) → 5턴 뒤 `마자용의 회복봉인 효과가 사라졌다!`, 금제 → 먹다남은음식 회복 없음 → 5턴 뒤 `마자용은 도구를 쓸 수 있게 되었다!`, 배턴터치로 회복봉인이 넘어감(HP회복만 가진 교체 포켓몬이 발버둥): **3/3 PASS**.
  - 이식 전 코드(클론 `e83c57fb`, `ZZ old` 사본, 커밋 안 함·지움): **3/3 PASS**(출력 같음 — 의도대로).
  - 처음 쓴 배턴터치 테스트는 교체 포켓몬에게 HP회복을 고르게 해서 선택 단계에서 막혀 테스트 기록 오류 → 기술을 HP회복 하나로 두고 발버둥 확인으로 바꿈.
- **세이브 정적 비교**(이전 = 클론 `e83c57fb` 빌드, ROM SHA1 `ebed1c44…` = 묶음 57 기록과 같음): **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s491 8`): **555/652, `판정: PASS (기대와 같음)`, 요약 바이트 같음**.
- **ROM**(커밋 `c311121f`): SHA1 `36d422441aee9b57d49c3bf3b4320a9834794f60`, 사용 32,962,692 B(파일 33,554,432 B), 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 하지 않음(친구).** 후보(기대: 이식 전과 같은 화면) — 회복봉인(사이코노이즈 포함)·금제가 걸린 상태의 회복 기술·도구·턴 끝 문장, 멘탈허브로 회복봉인 해제.

## 다음

- seq 492 #10640(`MSG_DISPLAY` 상수와 대기 포함 문자열 함수). 목표 1.17.1.
