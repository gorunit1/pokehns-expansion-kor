# full-sync 묶음 85 — seq 524~528 (1.17.1-A 뒷부분: #10753·#10741·#10773·#10776·#10777)

- 요청: 합본 방침(사용자 2026-10-29, 취침 중 진행).
- 근거 문서: [`1.17.1-extension-plan.md`](1.17.1-extension-plan.md) 3절 표, 7절(친구 결정 2026-10-12c — Q1 #10777 기술 5개 수용, 파동의방호 이름·설명은 HnS 한글).
- upstream: #10753 `7644501f30`, #10741 `cafe0221ce`, #10773 `456a6e65b1`, #10776 `6e87bf6687`, #10777 `d74f64dc9f`.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 524 | #10753 | `e1589f09` | 마을 연결 지점에서 필드 야생 포켓몬이 사라질 때 도망 소리 삭제 — HnS `WE_OWE_DESPAWN_SOUND` `FALSE`라 원래 안 나던 소리(죽은 코드). 바뀐 바이트는 그 파일 뒤쪽 assert 줄 번호 하나 | 32,973,524(같음) |
| 525 | #10741 | `1a88578b` | 우격다짐이 왕의징표석·예리한이빨 풀죽음을 막지 않게(`EFFECT_BYPASS_SHEER_FORCE`) + 테스트 | — |
| 526 | #10773 | `f40be415` | `BattleSetup_StartMultiBattle`가 배틀 끝 콜백을 `CB2_EndSpecialTrainerBattle`로 정함(이전엔 남아 있던 값). `AreMultiPartiesFullTeams` hunk는 HnS가 #10711 때 같은 처리를 이미 넣어 HnS 블록 그대로 | — |
| 527 | #10776 | `daf33773` | 테스트 세이브의 주인공 이름 종결자(테스트 러너만). HnS 한 타입 챌린지 줄 옆에 추가 | — |
| 528 | #10777 | `e6097df6` | Champions M-C 기술 5개: 베어가르기 위력 80, 희망사항·힘흡수 PP 5, 스타어설트 위력 170, 전광쌍격 펀치 기술(철주먹). 특성 상수·메가 5종 특성·파동의방호는 HnS가 이미 같은 값, 파동의방호 이름·설명은 HnS 한글 유지 | 32,973,556(+32) |

이식 전(`e094165a`, 게임 코드 = `3d24ae2f`) SHA1 `2e2814f2…` → 이식 후 SHA1 **`c469d4a803be9e9d1fc02253f6b71c06828ba0d8`**.

## HnS 판단

- **#10773(멀티 배틀 끝):** HnS `multi_do`(로켓단 아지트 B2F 아테나 + 조무래기, 목호 파트너)는 `callnative BattleSetup_StartMultiBattle`을 부르는데 이 함수가 끝 콜백을 정하지 않아 직전 값으로 돌아가던 상태(upstream도 #10773 전엔 같은 구조). 승패 `VAR_RESULT`는 `ReturnFromBattleToOverworld`가 콜백과 상관없이 넣음(재확인 64 정정 내용). 반쪽 팀 멀티는 `specialTrainerBattleType = 0xFF`라 새 콜백은 필드 복귀 + 스크립트 계속만 함 → 스크립트의 승리(`case 1`)·패배(`SetCB2WhiteOut`) 분기는 그대로.
- **#10741(우격다짐 + 왕의징표석):** 배틀 동작 수정(계획 5절 수용). 출력 변화 1행을 `BATTLE_MESSAGE_OUTPUT_CHANGES.md`에(풀죽음 문장이 새로 나올 수 있음).
- **#10777:** 친구 결정 Q1 그대로. HnS `B_UPDATED_MOVE_DATA`·`B_UPDATED_MOVE_FLAGS` = `GEN_LATEST` = `GEN_CHAMPIONS`라 수치가 실제로 바뀜.
- 세이브: 영향 없음.

## 검증

- **빌드:** 각 seq `make hns -j8` 종료 0(524 SHA1 `f8f8dd10…` — assert 줄 번호만, 함수 비교로 확인; 525 `9feae64b…`; 526 `2a9823f1…`; 528 `c469d4a8…`). 저장소 전체 재빌드: 종료 0, SHA1 `c469d4a8…`, 새 컴파일 경고 0(`warncheck.sh`).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `e6097df6`): 기준 `test-baseline-seq522.txt` 대비 **새 PASS 3**(왕의징표석 우격다짐, 색다른 야생 TV, 주인공 이름 종결자 — 이번 PR 테스트), **사라진 PASS 0**. 새 기준 **`test-baseline-seq528.txt`**(`verify_seq.sh`도).
- **한글 테스트:** 새로 쓰지 않음(문장 0).
- **세이브 정적 비교**(이전 = 클론 `e094165a` 빌드): **PASS(FAIL 0, WARN 3)** — `HandleSpecialTrainerBattleEnd` → `CB2_EndSpecialTrainerBattle` 이름 변경(#10773)과 `DoSpecialTrainerBattle` 안 assert 줄 번호, 그에 딸린 검토 항목. 세이브 블록·RAM 변수 같음.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s528 8`): **555/652, `판정: PASS (기대와 같음)`, 상태 줄 차이 0**.
- **ROM**(커밋 `e6097df6`): SHA1 `c469d4a803be9e9d1fc02253f6b71c06828ba0d8`, 사용 32,973,556 B(묶음 84 대비 +32), 파일 33,554,432 B, 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기 후보(친구):** 로켓단 아지트 B2F 아테나 멀티 승리 → 퇴각 대사, 패배 → 화이트아웃(재확인 64와 같은 확인), 우격다짐 + 왕의징표석/예리한이빨 풀죽음, 베어가르기·스타어설트 위력·희망사항·힘흡수 PP 표시.

## 다음

- seq 529 #10772(없는 배틀러 칸 접근 방지), 530 #10781(디버그 치트 시작), 531 #10782, 532 #10627 + 546 #10830(한 단위, 옆 계단), 533 #10779 …(1.17.1-B).
