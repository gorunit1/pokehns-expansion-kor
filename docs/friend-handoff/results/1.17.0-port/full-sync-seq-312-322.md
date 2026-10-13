# full-sync 묶음 19 — seq 312~322 (+ #10814 앞당김)

- 시작 HEAD `648ac622`(코드 = `1b1fa5fe`, ROM SHA1 `7897b54b…`). 작업: 클라우드 세션, 2026-10-13.
- 방식: 사전 분석 3갈래(AI·기타 312·320·322 / 도구 313·318 / 패배 판정·트레이너 슬라이드 315·316·317 — 사본에서 빌드·테스트, 산출물은 세션 임시 `b19/`) → 결정(사용자 "추천대로", 315의 `battle_setup.c` hunk만 친구 결정으로 남김) → PR별 적용·빌드·관련 테스트 → 메인 검증.
- 실기(mGBA) 미확인(친구가 함).

## 커밋

| seq | PR | 커밋 | 판정·HnS 적응 | ROM(B) |
|---|---|---|---|---:|
| (542) | #10814 Fix multi-battle outcome handling | `32a5fd5a` | **앞당김**(재확인 64). #8943 이식 뒤 `HandleSpecialTrainerBattleEnd`의 `AreMultiPartiesFullTeams()`가 `VAR_RESULT`(승패)를 반쪽 팀 멀티에서 0으로 덮어써, 로켓단 아지트 B2F 아테나 멀티를 **이겨도 화이트아웃 분기**로 가던 회귀(코드 해석 — 사전 분석(패배 판정 갈래)이 찾고 메인이 `multi_2_vs_2` → `multi_do` → `DoSpecialTrainerBattle(SPECIAL_BATTLE_MULTI)` 경로로 확인). upstream 한 줄 그대로. 같은 단위 #10773은 seq 526 그대로 | 32,783,700 |
| 312 | #9448 Prevent Protect AI from blocking ally beneficial-hit setups in doubles | `0632ac1f` | **테스트만**(게임 코드는 친구 업로드 `1821fd67`부터 있음). 왼쪽 아군 Beat Up 테스트에 `KNOWN_FAILING; // partner is protecting`(HnS #8647 파트너 예측 때문 — upstream도 뒤 #10615에서 같은 줄 추가) | (테스트) |
| 313·318 | #10483 Fix Symbiosis activation after Knock Off removed and item + #10577 Fix restore item regression | `3535a66f` | **한 커밋**(사용자 결정 — #10577이 #10483의 `TryRestoreHeldItems` 안쪽 변경을 되돌리고, HnS 지금 안쪽 로직이 그 결과와 같음). 공생이 플레이어 쪽 도난 기록(`itemLost[B_SIDE_PLAYER]`)을 보지 않음, 탁쳐서떨구기·`Cmd_givecaughtmon`이 `itemLost`를 트레이너 번호로(배열은 원래 `MAX_BATTLE_TRAINERS` 칸), `TrySaveExchangedItem` 조기 반환, 복원 가드를 함수 안으로. **HnS 적응:** 복원 조건의 `B_RESTORE_HELD_BATTLE_ITEMS >= GEN_9 ||` 유지(`// HnS:`) — 야생전에서 잃은 비열매 도구도 배틀 뒤 돌아오는 지금 동작 유지(upstream 1.17.0·1.17.1은 뺐고 master #10878이 되살림). 새 테스트 1개 | 32,783,620 |
| 314 | #10569 | — | 이미 적용(`cffd2344`, 2026-10-07) | — |
| 315 | #10568 simplify party defeat functions | `c2e499e3` | 부분 적용 + HnS 적응. `DoesPartyHaveBattleReadyMons`·새 `NoAliveMonsForPlayer`·`NoAliveMonsForOpponent`는 upstream 구조, `WillPlayerWhiteOutIfPartnerWinsAlone`은 HnS판 — 반쪽 팀 멀티는 이식 전 개수식(플레이어 자기 몬의 기절·알 + 세이브 파티 사용 불가 ≥ 6, 파트너 기절 안 셈 — `cc0576a543`), 풀 팀은 시설 예외(배틀타워 NPC 멀티 파트너 혼자 계속). upstream의 `gSelectedOrderFromParty` 루프는 쓰지 않음(→ seq 340 #10674는 "HnS 동등"). **`battle_setup.c` hunk 제외 — 친구 결정 대기**(재확인 35: 넣으면 진 미러 챌린지가 화이트아웃). 동등성: 사전 분석이 입력 공간 전수(8.2억 상태)·도달 모델(302만 상태)로 이식 전 HEAD와 비교 — 도달 가능한 상태 차이 0(upstream 그대로면 목호 반쪽 멀티 14만·배틀타워 33만 상태가 달라짐). 메인이 경로별(싱글·더블·두 트레이너·반쪽/풀 팀 멀티·시설·아레나·GEN_3)로 다시 따라감 | 32,783,300 |
| 316 | #10572 Fix trainer slide activating for player in multi battle | `eac6d960` | 그대로(upstream과 줄까지 같음). HnS 트레이너 슬라이드 표가 비어 출력 변화 없음 | 32,783,252 |
| 317 | #10571 Fix trainer slide form change | `09800693` | 그대로(upstream과 줄까지 같음). HnS 메가 스크립트 `effectsafterformchange`·한글 문장은 hunk 밖 | 32,783,332 |
| 318 | #10577 | (313과 한 커밋) | 위 313 행 | — |
| 319 | #10573 | — | 이미 적용(`f76c7bf7`, seq 70 단위, 2026-09-29) | — |
| 320 | #10590 Change Howl Soundproof test to prevent user from blocking with Soundproof | `def04c37` | 테스트만, 그대로. 영문 `MESSAGE` 때문에 FAIL(알려진 한계) — `MESSAGE` 줄을 뺀 임시 변형은 3경우 PASS | (테스트) |
| 321 | #10589 | — | 이미 적용(`715c1b91`, 2026-09-29) | — |
| 322 | #10557 fix bard text speed | `2bd4cdaf` | 그대로. `RunTextPrinters`가 글자를 찍은 뒤(`RENDER_PRINT`)에도 프린터 콜백을 부름(계획서의 "RENDER_FINISH"는 오기). 콜백을 쓰는 HnS 프린터: 음유시인(에메랄드 맵, HnS 일반 진행에서 못 감)·HnS 오프닝·버치 오프닝. 오프닝 콜백 조건(`currentChar-2 == 0x08`)이 한글 둘째 바이트 0x08 뒤 공백·문장부호에서 잘못 발동할 수 있음 — 지금 오프닝 문장에는 그런 자리 0(재확인 65) | 32,783,348 |

- 적용 8 커밋(+ 앞당김 1), 이미 적용 3, 한 커밋으로 합친 1(318). ROM 32,783,684 → **32,783,348 B(−336)**, EWRAM 250,408 B·IWRAM 25,516 B 그대로. 커밋마다 `git diff --check` 통과, `make hns -j8` 종료 0, 새 경고 0.

## 동작 변화

새 배틀 문장 0, 기존 문장 변경 0, 배틀 외 새 텍스트 0.

- #10814: 아테나 멀티(그리고 `multi_do`로 시작해 `VAR_RESULT`로 갈라지는 반쪽 팀 멀티 전부)를 이기면 승리 분기로 감. **10-04(#8943 이식) 이후 ROM에서 생긴 회귀를 되돌림.**
- 313·318: 공생이 위 조건에서 발동(`BATTLE_MESSAGE_OUTPUT_CHANGES.md` "특성·도구·도주" 행 추가). 배틀 뒤 도구 복원 결과는 같음.
- 312·315·316·317·320: 동작 변화 없음(312·320은 테스트만, 315는 HnS 판정 보존, 316·317은 HnS 슬라이드 표가 빔).
- 322: 음유시인 노래 끝 처리·글자 속도(HnS 일반 진행 밖). 오프닝은 지금 문장에서 변화 없음.

## 친구에게 알릴 것·물을 것

1. **아테나 멀티 승리 뒤 화이트아웃 회귀를 고침(#10814 앞당김, 재확인 64)** — 실기: 아테나 멀티 승리 → 퇴각 대사, 패배 → 화이트아웃. 10-04 이후 ROM으로 이 장면을 지난 세이브가 있다면 진행 상태 확인.
2. **질문(재확인 35):** 315 #10568의 `battle_setup.c` hunk를 넣을지 — 넣으면 미러 챌린지에서 진 경우 화이트아웃(지금은 화이트아웃 없이 처치 스크립트가 이어질 수 있음).
3. 오프닝 텍스트 콜백(재확인 65) — 오프닝 문장을 바꾸면 알려 달라.

## 검증

- **빌드:** 위 표.
- **관련 테스트(파일 단위, 기준 `test-baseline-hnsfix1013.txt`):** `ai_doubles.c` 새 3줄(PASS 2·KNOWN_FAILING 1), `symbiosis.c` PASS 2/10 → 3/11(새 테스트 PASS — 이식 전 코드에 이 테스트만 넣으면 `Unmatched ABILITY_POPUP`으로 실패, 사전 분석), `knock_off.c`·`steal_item.c`·`trick.c`·`bestow.c` 기준과 같음, `multi_battle_whiteout.c` 7/7 PASS(기준과 같음).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j4`, 코드 `2bd4cdaf`): PASS 2,659 → **2,662** / TOTAL 5,582 → 5,586. `test-baseline-hnsfix1013.txt` 대비 **사라진 PASS 0**, 차이는 새 테스트 4줄(312 3줄 — PASS 2·KNOWN_FAILING 1, 313 `Opposing Symbiosis ignores the player's stolen-item record: PASS`)과 320 이름 변경 1줄(`Howl does not work on partner if it has Soundproof: FAIL` → `… but doesn't fail on user with Soundproof: FAIL`)뿐. 목록 [`test-baseline-seq322.txt`](test-baseline-seq322.txt)(**다음 비교 기준**).
- **한글 회귀 607개:** 517/607, 기대 요약과 바이트 같음.
- **세이브 정적 비교**(`pre-312` = `648ac622` 빌드): **PASS(FAIL 0, WARN 2)**. WARN 2개는 같은 것 — 검토 대상 `HandleSpecialTrainerBattleEnd`(배틀 뒤 파티를 세이브 블록 `playerParty`에 복사)의 기계어 차이 13줄 = #10814의 `gSpecialVar_Result = gBattleOutcome` 저장 4명령·리터럴 1개와 그로 밀린 분기 주소뿐. 세이브 바이트 해석·복사 내용 변화 없음. EWRAM·IWRAM 변수 변화 0.
- **최종 ROM(실기용):** 코드 `2bd4cdaf`, 클라우드 `make hns -j8`(`arm-none-eabi-gcc 13.2.1`) → 사용 32,783,348 B, SHA1 `241dedd3920f7ad87283b876fb82e269a9de2017`.

## 재확인·뒤 PR 메모

- 22: 공생의 `itemLost[B_SIDE_PLAYER]` 조건 해소.
- 35: `battle_setup.c` hunk 친구 결정 대기(세션 임시 `b19/seq315_optional_battle_setup.patch` — 기권 분기 통합과 필드 재호출 제거는 같이 넣거나 같이 뺌).
- 340 #10674: 315에서 `gSelectedOrderFromParty` 루프를 쓰지 않았으므로 "HnS 동등"으로 기록.
- 526 #10773: #10814와 같은 단위 — 542는 앞당김 적용(다시 넣지 않음), 526은 순서대로.
- 64·65: 위.
