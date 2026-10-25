# full-sync 묶음 46 — seq 479 (#10330 대상 실패 판정을 검사별로)

- 요청: 사용자 "seq 479 #10330(Adjust TargetFailure)부터 이어서"(2026-10-25).
- upstream: 읽기 전용 사본, #10330 `ffb8dafb24`(2파일 — `include/constants/battle_move_resolution.h`, `src/battle_move_resolution.c`, 게임 코드만·테스트 변경 없음). kor=N, 세이브 영향 N.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 479 | #10330 | `d1e2dcae` | `CancelerTargetFailure`를 "대상마다 모든 검사"에서 "검사마다 모든 대상"(`enum TargetFailure`, `eventState.moveEndBlock`으로 이어 돌기)으로, 꿈먹기·싱크로노이즈·프리폴 무효를 `IsTargetUnaffectedByMoveEffect`로, 대미지 문맥에 모든 배틀러의 특성·도구, `ShouldSkipFailureCheckOnBattler`가 항상 무효 대상을 건너뜀, HnS 보정 1, 한글 회귀 테스트 | 32,959,124 |

이식 전(`7b2c0b9e`, 코드 = `00ff6d5a`) 32,958,340 B → **32,959,124 B(+784)** — 검사 루프가 6개로 펼쳐진 것(upstream 그대로 +704, HnS 보정 +80).

## 내용과 HnS 판단

- **적용 방법:** upstream 전·후 파일로 3-way 병합(`git merge-file`). 충돌 4곳을 손으로 — ① `CancelerMoveEffectFailureTarget`의 HnS `checkUserFailure`(먼저 이식한 #10175 형태) 유지 + 새 시그니처, ② `CancelerPriorityBlock`의 `GetPartnerBattler`(#10542 이식분) 유지, ③ HnS에서 위치가 달랐던 `IsTargetUnaffectedByDreamEater` 삭제 후 새 함수 2개, ④ `moveBouncedBack` 블록 삭제. 결과의 `IsTargetUnaffectedByMoveEffect` ~ `CancelerTargetFailure` 끝은 HnS 보정 줄을 빼면 upstream `ffb8dafb24`와 글자까지 같음. HnS `ctx->baseMove`(#10616) 유지.
- **동작 변화(upstream 의도, 새 문장 0):** 더블 전체기에서 여러 대상이 서로 다른 이유로 막히면 문장 순서가 검사 순서(공중·땅속 → 사이코필드 → 방어 → 반사 → 특성·도구 → 타입 무효)를 따름. 예: 왼쪽 상대 방어·오른쪽 상대 공중날기 → 이전 `…은 공격으로부터 몸을 지켰다!` → `…에게는 맞지 않았다!`, 지금은 반대. 꿈먹기는 싱크로노이즈와 같은 무효 스크립트(같은 문장, 쉼 없이 짧은 대기 — 코드 감사만). `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 2행.
- **HnS 보정(upstream 결함, 재확인 97):** upstream은 이번에 "쓰러진 대상 → 실패" 분기를 지웠고 다시 거르는 건 #10566(seq 486)이다. 그 사이엔 그 턴에 먼저 쓰러진 상대가 타입 무효일 때 전체기가 `상대 고오스에게는 효과가 없는 것 같다...`를 띄움(테스트로 확인). HnS는 첫 검사 블록에서 쓰러진 대상을 이식 전처럼 `MOVE_RESULT_FAILED`로 두고 넘김(7줄). HP 바·부유 팝업·빗나감 문장·매직미러 반사는 다른 코드가 이미 막고 있어 보정 없이도 PASS — 테스트로 남겨 seq 486에서 다시 봄.
- HnS 고유 기능(메가 특성·파동의방호·AI 교체 가드·챌린지 옵션)은 이 함수를 지나지 않거나(파동의방호는 `CanMoveBeBlockedByTarget` 밖) 판정 내용이 같음. 세이브 구조 변화 없음.

## 검증

- **빌드:** `make hns -j8` 종료 0, 새 컴파일 경고 0(`warncheck.sh` — 처음 전체 빌드 로그, 클라우드 libpng `iCCP` 줄은 기존 제외 줄).
- **전체 테스트**
  - upstream 그대로(보정 전, 새 테스트 파일 없이): 목록이 `test-baseline-seq478.txt`와 **바이트 같음**(새 PASS 0·사라진 PASS 0).
  - 보정 + 새 테스트: 기준 대비 **새 PASS 6(새 한글 테스트)뿐**, 사라진 PASS 0, 다른 줄 0. 새 기준 [`test-baseline-seq479.txt`](test-baseline-seq479.txt)(**다음 비교 기준**). `make check` 종료 2는 기대 FAIL 때문(이전과 같음).
- **새 한글 회귀 `test/hns_10330_target_failure.c` 6개:** 쓰러진 고스트에 하이퍼보이스(무효 문장 없음), 쓰러진 상대에 HP 바 없음, 쓰러진 부유 고오스에 지진(팝업 없음), 쓰러진 상대 빗나감 문장 없음, 쓰러진 매직미러 브리무음이 울음소리를 반사하지 않음, 공중 → 방어 순서: **6/6 PASS**.
  - 보정을 뺀 upstream 그대로: **5/6** — 쓰러진 고스트 테스트만 FAIL(위 결함).
  - 이식 전 코드(클론 `7b2c0b9e`, 임시 이름 `ZZ old`, 커밋 안 함·지움): **5/6** — 순서 테스트만 FAIL(위 동작 변화).
- **세이브 정적 비교**(이전 = 클론 `7b2c0b9e` 빌드, ROM SHA1 `117e8515…` = seq 478 기록과 같음): **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`kortests/run.sh <저장소> kor-s479 8`, 코드 `d1e2dcae`): **558/652, `판정: PASS (기대와 같음)`**.
- **ROM**(커밋 `d1e2dcae`): SHA1 `aa9b0c7a8d17f3a8bacd52ae673106a91b6878c8`, 사용 32,959,124 B(파일 33,554,432 B), 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 하지 않음(친구).** 후보 — ① 더블에서 한 상대는 방어, 다른 상대는 공중날기·구멍파기 중일 때 전체기(하이퍼보이스·지진 등) 문장 순서, ② 더블에서 상대 하나가 먼저 쓰러진 턴에 전체기(특히 쓰러진 쪽이 고스트·부유) — 쓰러진 포켓몬 이름 문장이 없어야 함, ③ 깨어 있는 상대에 꿈먹기 무효 문장 템포.

## 다음

- seq 481 #10326(Battle Terrain Refactor, L). 480은 478 때 적용함, 482 #10362는 프레젠트 한 줄만 앞당긴 상태(재확인 96).
