# full-sync 묶음 10 — seq 204·205·207·208·209·214·216

- 시작 HEAD `4824539af9`(묶음 9 뒤, 코드 = `f9df186f69`). 작업 컴퓨터: 데스크탑(사용자는 랩탑에서 Remote Control). 날짜 2026-10-08~09.
- 방식: PR별 사전 분석 3개(병렬, 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-204-216/seq*.md`) → 사용자 결정 7개("ㄱㄱ") → 적용 1개(커밋 7개, 손 맞춤 0) → 리뷰 2개 → 메인 검증. 검증은 저장소 도구 `dev_scripts/hns_verify/`.
- 이미 적용이라 뺀 행: 206 #10064(`47515a949e`), 215 #10102(`4616fac998`), 210~213(#9730 단위 선반영).
- 실기(mGBA) 미확인.

## 커밋

| seq | PR | 커밋 | 판정·HnS 적응 | ROM(B) |
|---|---|---|---|---:|
| 204 | #10051 Give enum BattleTrainer more user friendly entry names | `660496e0f1` | `B_TRAINER_0..3` → `B_TRAINER_PLAYER`·`OPPONENT_A`·`PARTNER`·`OPPONENT_B`. upstream diff 대신 HnS 트리 전체(src·include·test 132파일, HnS 전용 줄 186줄)를 기계적으로 바꿈 + 도구 한글 회귀 테스트 `zz_hnssw_probe.c` 1줄(사용자 결정 A1). 이름 말고 다른 변경은 `include/constants/battle.h` 머리 주석뿐. **ROM 바이트 같음**(SHA1 `1d961bba…`). #9896 후속 `test/random_mon_generation.c` hunk 포함(재확인 32) | 32,753,828 |
| 205 | #9885 Adds deprecation warnings to old party and party count globals | `c33ff01616` | upstream hunk 6개 그대로·문맥, 이미 같은 형태인 2개(`battle_main.c`, `test/pokemon.c`) 제외. HnS 전용 부적금화 줄(`src/battle_hold_effects.c`)을 `gParties[B_TRAINER_PLAYER][i]`로(동작 같음, 결정 A2 — 안 바꾸면 새 경고 1), 도구 세이브 왕복 테스트 `zz_hns8943_savecompat.c` 7곳(결정 A3). ROM 크기 같고 SHA1만 바뀜 — assert 줄 번호(`pokemon.c` +5줄)와 링커 스텁 배치(리뷰 R1이 변형 빌드로 분리 확인) | 32,753,828 |
| 207 | #10059 Fix 12v12 partner party menu slot ptr | `36ddd374fb` | upstream 그대로 + #10039의 `CalculatePartnerPartyCount` 2줄 선반영(결정 B1 — 없으면 207~208 사이 배틀타워 멀티 파트너 페이지 커서가 −1·빈 칸). 배틀타워 멀티 L/R 페이지 전환 때 커서가 첫 칸으로(결정 B2). 목호·실버는 반쪽 팀 레이아웃이라 영향 없음. 새 `test/party_menu.c` 3개 | 32,754,004 |
| 208 | #10066 Fix Generated OWEs Accessing Garbage Data | `4c4b8a8c97` | 빈 줄 문맥만 맞춤. HnS `WE_OW_ENCOUNTERS FALSE`라 도달하지 않음 | 32,754,036 |
| 209 | #10039 Fixed NoAliveMonsForPlayer for multibattles | `94a1744bfa` | upstream 형태 + HnS 적응 3곳: ① 반쪽 팀 멀티 한계 6 유지, ② 시설 배틀에는 새 "풀 팀 즉시 패배" 미적용(결정 B4 — 배틀타워 멀티는 지금처럼 파트너 혼자 계속 = 본가 4세대 이후), ③ 파트너 기절은 풀 팀일 때만 셈(HnS 보호 `cc0576a543` 유지, 결정 B3). 새 `test/battle/multi_battle_whiteout.c` 8개 | 32,754,164 |
| 214 | #10100 Fix colosseum warps frlg | `ba73dcbd9b` | FRLG 전용(`.if IS_FRLG`), HnS `waitstate` 문맥 유지. HnS ROM 같음 | 32,754,164 |
| 216 | #10104 Add missing item sort types | `f838255c83` | 그대로. 특성가드·선택만두·교환사탕·두번양념무 `sortType`만(ROM 4바이트). 가방 TYPE 정렬 때 이 4개 위치만 바뀜(일반 플레이에서 얻을 수 있는 것은 랜덤 필드 아이템 특성가드뿐), 세이브 구조·저장된 가방 순서 그대로 | 32,754,164 |

## 사용자 결정(2026-10-08 밤 "ㄱㄱ", 모두 추천대로)

A1 도구 한글 회귀 테스트 새 이름, A2 부적금화 줄 새 형태, A3 도구 세이브 왕복 테스트 새 형태, B1 #10039 2줄 선반영, B2 배틀타워 멀티 커서 초기화 수용, B3 HnS 보호 `cc0576a543` 유지 + 반쪽 팀 한계 6, B4 배틀타워 멀티 즉시 패배 규칙 미적용.

## 동작 변화

- #10059: 배틀타워 멀티 파티 메뉴에서 L/R로 페이지를 바꿀 때 커서가 첫 칸으로 간다.
- #10039: HnS에서 실제로 닿는 멀티(목호·실버 반쪽 팀, 배틀타워 NPC 멀티, 링크 멀티)는 이식 전과 같다. 다른 곳은 "시설 밖 풀 팀 멀티"뿐이고 HnS에서는 디버그 메뉴 `Try Battle`에서 파트너와 반쪽이 아닌 상대를 고른 경우에만 닿는다(그 경우 upstream 규칙: 내 포켓몬이 모두 쓰러지면 즉시 패배). 필드 재호출(`battle_setup.c`)이 `VAR_RESULT`를 덮던 부수 효과가 사라졌으나 화이트아웃 스크립트는 그 값을 읽지 않는다.
- #10104: 가방 TYPE 정렬에서 위 4개 아이템 위치.
- 배틀 메시지 출력 변화 없음(한글 회귀 607개 같음).

## 검증

- **빌드:** 커밋마다 종료 코드 0, 새 경고 0, `deprecated` 0. 최종(`f838255c83`) ROM **32,754,164 B**(묶음 전 +336), EWRAM 250,408 B, IWRAM 25,516 B(변화 0), SHA1 `6995c392ac2a916eef5acb9ace88998b45a37975`.
- **전체 테스트:** PASS 2,561 → **2,572** / TOTAL 5,475 → 5,486. **사라진 PASS 0.** 새 줄 10 = `test/party_menu.c` 3 + `multi_battle_whiteout.c` 7줄(8개, 이름 중복 1). 목록 [`test-baseline-seq216.txt`](test-baseline-seq216.txt)(**다음 비교 기준**).
- **한글 회귀 607개:** 기대와 바이트 같음(517/607). 바꾼 도구 테스트 `zz_hnssw_probe.c` 컴파일·PASS.
- **세이브:** 정적 비교(이식 전 사실 = HEAD `4824539af9` 빌드) **PASS(FAIL 0, WARN 2)** — 두 줄 모두 `DoSpecialTrainerBattle`에 207이 넣은 `CalculatePartnerPartyCount` 호출과 그 분기 오프셋·레지스터 배정(세이브 데이터를 읽고 쓰지 않음). 엄격 54함수·섹터 배치·SaveBlock1/2/3·Storage 크기 같음, RAM +0. 세이브 왕복 **PASS**(바꾼 도구 테스트 경고 0).
- **리뷰 2개:**
  - R1(#10051·#9885): 바뀐 133파일을 "옛 파일 + 이름만 sed"와 대조 — 132파일 같고 머리 주석 1곳만 다름(upstream과 같음). 숫자별 개수 1:1(0/1/2/3 = 1,394/291/72/44), HnS 전용 PLAYER 아닌 26줄 의미 확인, C 밖(데이터·스크립트·JSON·도구) 옛 이름 0, 처음부터 빌드 SHA1 = 기준. #9885 바뀐 줄 upstream과 글자 같음, ROM SHA1 변화 원인을 변형 빌드로 분리(줄 번호·스텁 배치, 기능 바이트 같음). 정정: 분석의 "부적금화 줄을 안 바꾸면 CI가 깨진다"는 HnS에서는 근거가 아님(HnS 트리의 Emerald 빌드는 원래 HnS 상수 때문에 깨져 있고 CI는 master·upcoming·PR만) — A2는 "새 경고 0" 규칙으로 유지. 수정 필요 0.
  - R2(#10059·#10066·#10039·#10100·#10104): `NoAliveMonsForPlayer` 이식 전·HEAD 원문을 호스트에서 컴파일해 66,382,848 상태 비교 — 반쪽 팀 전 플래그·배틀타워 NPC 멀티 차이 0(upstream 원형은 211,171 상태 차이 → 비교기 유효). 목호전 위치 모델 208,896 상태(예비가 있으면 계속, 없으면 패배, 목호 3마리가 먼저 쓰러져도 계속, 둘 다 전멸이면 패배) 같음. 수정 필요 0. 문서 2건(재확인 8c·8i, 아래), 범위 밖 발견 1건(재확인 35).
- **메인 검증(최종 HEAD):** 재빌드 SHA1 같음, 전체 테스트 목록이 적용 뒤 목록과 같음, 한글 607개 같음, 세이브 정적 PASS(WARN 2 같은 항목)·왕복 PASS.

## 재확인·뒤 PR 메모

- 8b: #10051(204)·#10059(207) 해결.
- 8c 갱신: 209에서 HnS 규칙을 지켜 중간 상태 없음. seq 315 #10568(1.17.0 판)은 예비가 있어도 즉시 화이트아웃(7,440/14,376 상태), 예비가 없는데 계속(2,430/7,128), 시설에서도 즉시 패배 — upstream master도 같음 → 315에서 같은 규칙(시설 예외 포함)으로 다시 HnS 적응. 비교기 저장소 밖 `chunk-204-216/review-R2/harness/harness2.c`.
- 8i: seq 209 뒤에도 테스트 빌드로는 `zz_f141` 9/18, `zz_fix_r2` 7/14(측정 불가 그대로), 사본에서 게임 분기로 바꾸면 18/18·14/14(두 HnS 수정 동작). 64개 테스트 결과 같음. seq 348 때 다시.
- 32: #10051의 `test/random_mon_generation.c` hunk 반영(이름 변경에 포함).
- 35(새, 범위 밖·코드 해석): 미러 챌린지에서 트레이너에게 지면 원래 파티가 복원된 뒤라 화이트아웃 없이 스크립트가 계속되어(`battle_main.c` 미러 복원 → `battle_setup.c` 필드 재호출) 패배 뒤 처치 스크립트(배지 지급 등)가 실행될 수 있음. 이식 전부터, 의도인지 친구 확인.

## 실기(mGBA) 확인 항목

- 로켓단 아지트 B2F 목호 멀티: 내 포켓몬 3마리 / 4마리 이상, 내 쪽 전멸 + 목호 남음(예비 있음/없음), 목호 먼저 전멸.
- 배틀타워 멀티: 파티 메뉴 L/R 페이지 전환 커서, 내 2마리가 쓰러진 뒤 파트너 혼자 계속.
- (선택) 가방 TYPE 정렬에서 특성가드 위치.
