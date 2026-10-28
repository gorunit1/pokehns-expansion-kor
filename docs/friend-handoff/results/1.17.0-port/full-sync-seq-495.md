# full-sync 묶음 61 — seq 495 (#9677 기술 애니메이션의 숫자를 상수로)

- 요청: 사용자 "ㄱㄱ 495 시작해 혼자해도 되는거라면"(2026-10-28).
- upstream: 읽기 전용 사본, #9677 `813e1c66c0`(1.16.0, 2파일, +115/−114). 같은 단위(`U-9677`)에 다른 seq 없음.
- 혼자 진행한 근거: 이름만 바꾸는 정리(새 문장 0). 값이 같은지 하나씩 확인했고, 실제로 바뀌는 것은 싸라기눈 효과음 위치 1바이트(upstream 의도).

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 495 | #9677 | `2fdced80` | `data/battle_anim_scripts.s`의 효과음 번호·소리 위치(pan) 숫자를 상수로(114줄), `SOUND_PAN_MIDDLE`(0) 추가 | 32,962,756 |

이식 전(`9d11e2f2`, 게임 코드 = `44978ff2`) 32,962,756 B → **32,962,756 B(같음)**.

## 내용과 HnS 판단

- **적용 방법:** 3-way 병합(`git merge-file`). 충돌 1곳 — upstream이 바꾸는 `gBattleAnimGeneral_FutureSightHit`·`gBattleAnimGeneral_DoomDesireHit`를 HnS는 seq 482(#10362)에서 이미 지움 → HnS 형태(삭제). 그 애니메이션이 옮겨 간 `DoomDesireHit`의 `playsewithpan SE_M_CONFUSE_RAY, 0` 1줄은 1.17.1과 같게 `SOUND_PAN_MIDDLE`로.
- **값 확인:** 바뀐 114줄을 줄 단위로 맞대 바뀐 낱말 17쌍을 뽑아 확인 — 효과음 번호 13개(`119` → `SE_M_THUNDERBOLT2` … `228` → `SE_M_MORNING_SUN`)는 HnS `include/constants/songs.h` 값과 모두 같음, pan `192` → `SOUND_PAN_ATTACKER`(−64, 같은 바이트 0xC0), `63` → `SOUND_PAN_TARGET`, `0` → `SOUND_PAN_MIDDLE`.
- **바뀌는 것 1곳:** 싸라기눈(`gBattleAnimMove_Hail`) `loopsewithpan SE_M_HAIL, 0, …` → `SOUND_PAN_TARGET`(가운데 → 상대 쪽 소리). 이식 전 ROM과 바이트 비교: **차이 1바이트**(0x08275CDF, `gBattleAnimMove_Hail` 안, 0 → 63). 문장 변화 없음.
- 세이브: 영향 없음.

## 검증

- **빌드:** `make hns -j8` 종료 0, 전체 재빌드(`build/hns` 지우고) 로그로 새 컴파일 경고 0(`warncheck.sh`).
- **ROM 바이트 비교**(이전 = 클론 `9d11e2f2` 빌드, SHA1 `78627b4e…` = 묶음 60 기록과 같음): 1바이트(위).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `2fdced80`): 목록이 `test-baseline-seq492.txt`와 **바이트 같음**(INVALID 24). 기준 파일은 그대로 seq492.
- **한글 테스트:** 새로 쓰지 않음 — 애니메이션 효과음 위치는 배틀 테스트 러너가 검사하지 않음. 대신 위 ROM 바이트 비교로 확인.
- **세이브 정적 비교:** **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s495 8`): **555/652, `판정: PASS (기대와 같음)`, 요약 바이트 같음**.
- **ROM**(커밋 `2fdced80`): SHA1 `67a944b0a6484520ec1f2d6f0f7fcb5c60d8939a`, 사용 32,962,756 B(파일 33,554,432 B), 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 하지 않음(친구).** 후보 — 싸라기눈 애니메이션 효과음이 상대 쪽(오른쪽)에서 들림. 나머지 기술 애니메이션 소리는 같음.

## 다음

- seq 496 #9676(주먹·발 애니메이션 매크로, 1.16.0). 목표 1.17.1.
