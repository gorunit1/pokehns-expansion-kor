# full-sync 묶음 47 — seq 481 (#10326 배틀 필드(지형) 리팩터) + 487 #10586 앞당김

- 요청: 사용자 "ㄱㄱ 481 시작해 혼자해도 되는거라면"(2026-10-25).
- upstream: 읽기 전용 사본, #10326 `560df6d0b5`(48파일 — 게임 코드 31, 테스트 16, 테스트 러너 1). kor=N, 세이브 영향 N. 같은 단위(`U-terrain-10326`)의 #10586 `c65b99009f`(seq 487, 1줄)을 같은 커밋에 앞당김.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 481 (+487) | #10326 (+#10586) | `15bb0685` | 지형을 `gFieldStatuses` 비트 4개 → `gFieldTimers.terrain` 값 하나 + `gBattleTerrainInfo` 표(타입·비밀의힘·자연의힘·배경·씨앗·시작/끝 문장), 지형 기술 효과 4개 → `EFFECT_TERRAIN`(기술 인자에 지형), `jumpifterrain` 명령 추가, `jumpifterrainaffected` 인자 바이트로, `FieldTimer` 필드 u8, 대미지 문맥에 지형, AI 필드 판정 재구성, HnS 보정 1, #10586, 한글 회귀 테스트 | 32,958,804 |

이식 전(`588d3ea1`, 코드 = `d1e2dcae`) 32,959,124 B → **32,958,804 B(−320)**. EWRAM `gFieldTimers` 16 → 12 B.

## 내용과 HnS 판단

- **적용 방법:** 파일별 3-way 병합(upstream 전·후 파일, `git merge-file`) — 48파일 중 35개 깨끗, 13파일 충돌 37곳을 손으로. `battle_ai_field_statuses.c`는 HnS 차이가 매크로 → 함수 이름(#10542)과 유틸리티우산 한 줄뿐이라 upstream 이후 파일에 그 차이만 다시 얹음. 병합 뒤 파일마다 "upstream 변경 줄 ↔ HnS 변경 줄"을 맞대 남는 차이가 모두 아래 HnS 형태로 설명되는지 확인(정렬을 잘못 잡은 하드론엔진·일렉트릭메이커 두 줄을 이 단계와 빌드 전 식별자 검색으로 찾아 고침).
- **HnS가 이미 가진 뒤쪽 PR 형태 유지:** `AiCalcValues`(#10277 — `fieldStatuses` 자리를 `terrain`으로, upstream master와 같은 모양), 날씨 실패 enum 반환, 하드론엔진 두 갈래(지금 지형이 일렉트릭이면 "힘으로" 문장 — upstream #10326 시점은 이 갈래가 깨져 있고 master에서 고쳐진 형태), Teraform Zero 필드 날씨 갈래, 일루전 문맥 복사, 아이스스피너의 기존 조건, 무한 혼란 플래그 삭제(#10471), 트레이너 슬라이드 함수 정리, `MovesInfo` 인자 4바이트 정적 검사(그대로 통과), 테스트 러너 요약 줄. 지워진 매크로는 `GetPartnerBattler`·`GetBattlerLeftFoe`/`RightFoe`로, `LEFT_ALLY`는 `GetBattlerAtPosition(GetBattlerSide(battler))`로 풀어 씀(upstream master는 이 자리를 `GetBattlerLeftFoe`로 바꿔 놓아 트릭룸 "둘 다 동의" 판정이 사실상 꺼짐 — HnS는 #10326 의도대로).
- **upstream 그대로 받은 동작 변화(새 문장 0):**
  - AI(더블): 날씨 판단이 "공격자·파트너 중 하나라도 원하는 결과"로(이전엔 모래바람·싸라기눈은 둘 다 동의해야), 날씨·필드·지형 판단이 쓰러진 파트너를 뺌, 지형 판단이 따로(`TerrainChecker`), 교체 시뮬레이션의 지형이 스카이배틀을 반영. 덤으로 AI가 교체 후보의 사이코메이커 지형을 대미지 계산에서 보게 돼 upstream `KNOWN_FAILING` 테스트 1개가 HnS에서 통과(`KNOWN_FAILING_PASSING: 1` — upstream 테스트 파일은 그대로 둠, `testlist.sh` 목록에는 이 상태가 안 나와 줄이 빠짐).
  - 대미지 문맥의 중력·매직룸 판정은 전역 `gFieldStatuses`를 직접 읽음(이전과 같은 값 — AI 시뮬레이션은 지형만 바꿔 넣음).
- **HnS 보정(upstream 결함, 재확인 98):** Teraform Zero가 날씨를 지운 뒤 지형이 없으면 upstream은 지형 제거로 그대로 흘러가 배경 복원 애니를 틀고 `gTerrainStringIds[B_MSG_TERRAIN_COUNT]`(표 밖)를 출력함(master 같음). HnS는 이식 전처럼 지형이 없으면 효과 단계로 건너뜀(스크립트 한 줄). 보정을 빼면 `HnS 10326: Teraform Zero clears rain and does not run the terrain removal without a terrain` FAIL을 확인.
- **#10586 앞당김:** 배틀이 지형을 남긴 채 끝나면 `gFieldTimers.terrain`이 다음 배틀까지 남고, 배틀 시작 배경(`LoadBattleTextboxAndBackground`)이 초기화 전에 그려져 지형 배경으로 시작함 — HnS는 `B_TERRAIN_BG_CHANGE = TRUE`라 실제로 보임(코드 추적). upstream 수정 그대로(`FreeBattleResources`에서 지형을 지움 — 전역 `gFieldStatuses` 초기화 줄은 upstream대로 빠지고 배틀 시작 초기화에만 남음, 배틀 밖에서 읽는 곳 0).
- 한글 문장: 지형 시작·끝 문장은 표가 같은 `B_MSG_TERRAIN_*`를 골라 이전과 같은 한글. 기술 데이터는 지형 기술 4개 효과·인자만(능력치·명중·PP 그대로).
- 한글 회귀 세트 `zz_hns9717_k1_ee.c`: `EFFECT_GRASSY_TERRAIN` → `EFFECT_TERRAIN`(이름만, 선례 seq 440). `test/battle/ai/values_moves_over_splash.c`(upstream `can_use_all_moves.c`가 HnS에선 이 이름)도 같은 이름만.
- HnS 고유 기능(메가 특성·파동의방호·AI 교체 가드·챌린지 옵션·세이브)은 바뀐 줄에 없음(AI 파일 diff에 HnS 표시 줄 0).

## 검증

- **빌드:** `make hns -j8` 종료 0, 새 컴파일 경고 0(`warncheck.sh`).
- **전체 테스트**(#10586 넣기 전·후 두 번, 목록 같음): `test-baseline-seq479.txt` 대비 **새 PASS 8(새 한글 테스트)뿐**, 사라진 PASS 0, 위 `KNOWN_FAILING` 1개가 통과로 바뀌어 목록에서 빠짐. 새 기준 [`test-baseline-seq481.txt`](test-baseline-seq481.txt)(**다음 비교 기준**).
- **새 한글 회귀 `test/hns_10326_terrain.c` 8개:** 일렉트릭필드 시작·5턴 뒤 끝, 같은 지형 다시 쓰면 실패, 일렉트릭필드 위 하드론엔진 "힘으로" 문장, Teraform Zero 비만(지형 제거 없음)·비+일렉트릭필드, 미스트필드 혼란 방지(인자 폭이 바뀐 스크립트 명령), 일렉트릭·미스트시드 능력 상승, 대지의파동 전기 타입: **8/8 PASS**.
  - 보정을 뺀 upstream 그대로: 7/8 — Teraform Zero 비만 FAIL(위 결함).
  - 이식 전 코드(클론 `588d3ea1`, 지형 상수만 옛 이름으로 바꾼 `ZZ old` 사본, 커밋 안 함·지움): **8/8 PASS** — 확인한 경우는 이식 전과 같음.
- **세이브 정적 비교**(이전 = 클론 `588d3ea1` 빌드, ROM SHA1 `aa9b0c7a…` = seq 479 기록과 같음): **PASS(FAIL 0, WARN 3)** — WARN은 ① `BoxMonRestorePP` 정규화 코드 2줄: `move.h`에 2줄이 늘어 `SanitizeMoveId` assert 줄 번호 상수가 234 → 236(같은 원인으로 "검토" 묶음 1건), ② `gFieldTimers` −4 B(배틀 전용 EWRAM, 세이브 밖). 세이브 블록 크기·섹터 배치·엄격 함수 54개 같음.
- **한글 회귀**(`kortests/run.sh <저장소> kor-s481 8`, 코드 `15bb0685` — 세트 `zz_hns9717_k1_ee.c` 이름 맞춤 포함): **558/652, `판정: PASS (기대와 같음)`**.
- **ROM**(커밋 `15bb0685`): SHA1 `66c764e90a803fbc347d8c63fec46441043e7987`, 사용 32,958,804 B(파일 33,554,432 B), 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 하지 않음(친구).** 후보 — ① 지형을 깐 채로 배틀을 끝낸 다음 배틀의 시작 배경(#10586 — 예: 카푸 계열 없이도 일렉트릭필드·그래스필드 기술을 쓴 직후 승리), ② 지형 기술(그래스필드 등) 시작·5턴 끝 문장과 지형 배경 전환, ③ 미스트필드 위 혼란 기술 막힘 문장, ④ 더블 트레이너전 AI의 날씨·지형 기술 선택(눈에 띄게 이상하면).

## 다음

- seq 482 #10362(Adds moveanim, sound, hitanim and hpupdate to Canceler, L — 483·486·493이 같은 단위, 재확인 96의 프레젠트 보정 3개 다시 확인). 487은 이번에 적용함.
