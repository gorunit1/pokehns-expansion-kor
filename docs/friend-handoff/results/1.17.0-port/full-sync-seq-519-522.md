# full-sync 묶음 84 — seq 519~522 (1.17.1-A 앞부분: #10718·#10731·#10732·#10698)

- 요청: 사용자 "혼자 할 수 있는 작업은 최대한 많이 하고 한 번에 메시지"(2026-10-29, 친구 문자 합본).
- 근거 문서: [`1.17.1-extension-plan.md`](1.17.1-extension-plan.md) 3절 표·5절(사용자가 혼자 정할 수 있는 것: #10698 등 upstream 배틀 수정 수용)·7절(친구 결정 2026-10-12c — #10698 출력 변화는 `BATTLE_MESSAGE_OUTPUT_CHANGES.md`에).
- upstream: 읽기 전용 사본. #10718 `df3830de04`, #10731 `789a236c1a`, #10732 `fbe12dbbf4`, #10698 `803c03ddc7`(113파일, 그중 테스트 97).

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 519 | #10718 | `729fda6d` | 피죤 `isSkyBattleBanned = B_SKY_BATTLE_STRICT_ELIGIBILITY` — HnS 설정 `FALSE` | 32,980,836(같음, SHA1 같음) |
| 520 | #10731 | `05e143f8` | AI `EFFECT_TERRAIN` case 중괄호 | 같음(SHA1 같음) |
| 521 | #10732 | `d850b057` | 성장·덫 기술 타입 조건을 `B_UPDATED_MOVE_TYPES`로 — HnS는 `B_UPDATED_MOVE_DATA`와 같은 값 | 같음(SHA1 같음) |
| 522 | #10698 | `3d24ae2f` | 아래 | 32,973,524(−7,312) |
| 522(테스트) | - | `4cb39b0e` | HnS 회귀 테스트 `HnS 10488` 혼란 자해 1개를 새 동작으로 | 같음 |

이식 전(`b3cf74ee`, 게임 코드 = `0bf43797`) SHA1 `78bb1362…` → 이식 후 SHA1 **`2e2814f2f2f423bf055d02c5d11c4f5e4a88a0ba`**.

## #10698 내용

- **`GetConfig`:** 설정마다 인라인 getter(테스트 밖에서는 상수). 이전 `GetConfigInternal`은 테스트 밖에서 상수 표(`sConfigChanges`)만 읽었고 쓰는 쪽은 아무것도 안 함 → 동작 같음, 호출 코드가 사라져 **ROM −7,312 B**.
- **배틀 수정(KNOWN_FAILING·TO_DO 해소):** 발버둥 타입 없음(`TYPE_MYSTERY`), 비밀의힘 `EFFECT_SECRET_POWER`(추가효과를 기술 끝 단계에서), 혼란 자해도 승부굳히기 2배 조건, 부적금화·행운의향로가 금제·서투름·매직룸에서도 상금 2배, 텔레키네시스 금지 종 바톤터치, 고정 기술(역린 등·반동 턴)의 우선도, 바디퍼지 폼 체인지 초기화, 위력 0이면 피해 0.
- **새 세대 설정 5개**(발버둥 반동·은혜갚기/분풀이 최소 위력·고정 피해 상성·흑안개 기충전·바디퍼지 폼 체인지): 모두 `GEN_LATEST` → HnS 동작 같음.
- **테스트:** `TO_DO` 165개를 실제 테스트로.

## HnS 판단

- **충돌 2곳:** `include/config/battle.h`는 HnS 블록(HnS 결정 `B_UPROAR GEN_4`·뒤에 들어온 설정들) 유지 + 새 설정 2줄(나머지 3줄은 충돌 없이 들어감). 필레어웨이 테스트는 빈 줄만 달라 upstream 쪽.
- **`IsAnyTargetTurnDamaged`:** HnS는 seq 115(#9494) 때 1인자판(대타출동 포함 고정)을 들여왔는데 upstream은 그 뒤 `SubCheck` 인자를 받음(1.17.1도 2인자, 모든 호출이 `INCLUDING_SUBSTITUTES`). #10698이 2인자로 부르므로 1.17.1과 같게 2인자로 바꾸고 HnS 호출 2곳에 `INCLUDING_SUBSTITUTES`(동작 같음), `enum SubCheck`를 1.17.1 위치(`include/constants/battle_util.h`)로 옮김.
- **출력 변화**(친구 결정대로 `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 2행 + 문단): 비밀의힘 추가효과 시점, 부적금화 상금 2배 조건. 새 문장 0.
- **HnS 회귀 테스트 1개 고침:** `HnS 10488`(seq 485에서 쓴 승부굳히기 테스트)의 "혼란 자해 뒤에는 2배 아님"이 #10698로 "2배"가 됨(upstream도 같은 테스트를 "doubles"로 이름까지 바꿈). HnS 고유 정책이 아니라 당시 upstream 동작을 적은 테스트라 새 동작으로 고치고 머리 주석에 이유. 나머지 5개(배북·대타·아픔나누기·생명의구슬·아이스페이스)는 그대로 PASS.
- 세이브: 영향 없음.

## 검증

- **빌드:** 519·520·521은 `make hns -j8` 종료 0, SHA1 `78bb1362…`(이식 전과 같음). 522: 종료 0, 저장소 전체 재빌드(`build/hns` 지우고) 종료 0, SHA1 `2e2814f2…`(첫 빌드와 같음), 새 컴파일 경고 0(`warncheck.sh`).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `3d24ae2f`): 기준 `test-baseline-seq511.txt` 대비 **새 PASS 169**(이 PR이 채운 테스트), **사라진 PASS 5** — 모두 확인:
  - 이름만 바뀜 4: "Struggle can hit ghost types" → "Struggle is Normal-type in Gen 1 and typeless in Gen 2+"(PASS), "Struggle does not receive normal-type STAB" → "… from Normal-type users"(PASS), "Strength Sap will restore hp if target has Contrary and is at +6 Atk" → "… +6 Attack"(FAIL — 새로 붙은 영문 `MESSAGE` 검사 때문, 한글판에서 늘 FAIL인 꼴), "Assurance does not double … confusion" → "Assurance doubles … confusion"(FAIL — 같은 이유, 피해 2배는 맞음).
  - 동작 바뀜 1: `HnS 10488` 혼란 자해 → 위처럼 테스트를 고쳐 PASS(6/6).
  - 새 기준 **`test-baseline-seq522.txt`**(이 실행 목록에서 고친 HnS 테스트 줄만 바꿈), `verify_seq.sh`도 이 기준으로.
- **한글 테스트:** 새로 쓰지 않음(문장 0). `HnS 10488` 6개 PASS.
- **세이브 정적 비교**(이전 = 클론 `b3cf74ee` 빌드): **PASS(FAIL 0, WARN 2)** — `BoxMonRestorePP` 안 assert 줄 번호 상수(`move.h` 줄이 늘어 246 → 247)와 그에 딸린 검토 항목. 세이브 블록·RAM 변수 같음.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s522 8`): **555/652, `판정: PASS (기대와 같음)`, 상태 줄 차이 0**.
- **ROM**(커밋 `3d24ae2f`, 테스트 커밋 `4cb39b0e`도 같은 ROM): SHA1 `2e2814f2f2f423bf055d02c5d11c4f5e4a88a0ba`, 사용 32,973,524 B(묶음 83 대비 −7,312), 파일 33,554,432 B, 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기 후보(친구):** 비밀의힘 추가효과 문장 순서(장소별), 승부굳히기(상대가 혼란 자해한 턴), 부적금화 + 금제/서투름/매직룸 상금. 셋 다 일반 진행에서는 드묾.

## 다음

- seq 524 #10753, 525 #10741, 526 #10773, 527 #10776, 528 #10777(친구 Q1 수용 — 기술 5개 수치, 파동의방호 이름·설명은 HnS 한글 유지).
