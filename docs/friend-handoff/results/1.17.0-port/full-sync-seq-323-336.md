# full-sync 묶음 20 — seq 323~336

- 시작 HEAD `267b8ec6`(코드 = `d4e3b8b6`, ROM SHA1 `e51b1c5b…`). 작업: 클라우드 세션, 2026-10-13~14.
- 방식: 사전 분석 3갈래(323·331·335 + 이미 적용 확인 / Z기술·다이맥스 325·328·336 / 경혈찌르기·흉내쟁이·공생 327·332·333·334 — 사본에서 빌드·테스트, 산출물은 세션 임시 `b21/`) → 결정(사용자 "추천대로", 332의 `GetBattleMoveTarget` hunk만 친구 확인으로 남김 — 재확인 66) → PR별 적용·빌드·커밋 → 메인 검증.
- 실기(mGBA) 미확인(친구가 함).

## 커밋

| seq | PR | 커밋 | 판정·HnS 적응 | ROM(B) |
|---|---|---|---|---:|
| 323 | #10608 Fix sleep clause in Multibattles + test | `465a3897` | 손 병합 4 hunk(호출 인자만). `monCausingSleepClause` → `struct SleepClause`(트레이너 + 파티 번호), `TryDeactivateSleepClause(battler, …)`. HnS 줄 보존: `battle.h` `stellarBoostFlags[NUM_BATTLE_SIDES]`, HnS 상태별 회복 문구(`TryCureAnyStatus` `lumBerryCureStatusMask`, `Cmd_curestatuswithmove`·`BS_CureStatus`의 `GetCuredStatusMessage`). 계획표의 "HnS는 현재 버그 없음"은 #8943 이식(seq 138.5) 전 기준이라 낡음 — 새 upstream 테스트가 적용 전 HnS에서 FAIL(파트너의 버프열매가 다른 트레이너가 건 클로즈를 풂). **HnS 게임은 수면 클로즈 꺼짐**(`B_SLEEP_CLAUSE FALSE`, `B_FLAG_SLEEP_CLAUSE 0`) → 게임 변화 없음 | 32,783,460 |
| 324 | #10464 | — | 이미 적용(`719bea0d`) | — |
| 325 | #10616 Fix Z-Move and Dmax moves incorrect dmg calcs | `3a67ba3b` | 부분 적응. `gBattleStruct->baseMove`·`DamageContext.baseMove`(비트필드 풀림 — ROM −1,216). 손 맞춤 3곳: `MovesWithCategoryUnusable`(HnS `SetTypeBeforeUsingMove` 인자 4개), `AI_CalcDamage` 자연의힘(HnS `aiCalc->move = move;` 유지), `HandleAction_UseMove`. **HnS 불복종(재확인 8t) 보존:** `CancelerObedience`에는 `dmgCtx.baseMove` 한 줄만, `passiveHpUpdate`·`#if TESTING` 훅·`OTName_` 그대로. **제외:** `SetDamageContextValues` hunk(#10144 seq 471.5가 들이는 함수 — HnS는 `Cmd_damagecalc`에서 계산, 결과 같음 → 재확인 8z), `ai.c` 끝 공백 hunk(해당 테스트 HnS에 없음). 한글 회귀 `HNS9680 3-04` 상대에 `HP(999)`(위력 90 → 130으로 1턴에 끝나서 — upstream `court_change.c`와 같은 조정, 사용자 결정) | 32,782,244 |
| 326 | #10631 | — | 이미 적용(`8ec45995`) | — |
| 327 | #10636 Fix Acupressure targetting opposing mon | `7f874300` | 그대로(HnS 코드 = PR 부모). 재확인 8w 해소 | 32,782,244 |
| 328 | #10639 Fix Cursed Body triggering on Max Moves | `36bcb0f1` | 그대로 | 32,782,308 |
| 329 | #10647 | — | 이미 적용(`7afa5225`) | — |
| 330 | #10648 | — | 이미 적용(`ed6ef1df`) | — |
| 331 | #10626 Fix Palafin-Hero repeatedly switching to activate Zero to Hero | `bce0ad13` | **테스트 + 주석만.** 게임 코드는 #9847(`482d6721`) 때부터 HnS에 있음. upstream 주석 3줄(사용자 결정 — 넣음), 테스트 3 hunk. ROM 바이트 같음 | 32,782,308 |
| 332 | #10645 Fix Acupressure targetting opposing mon | `aa31b7a4` | 부분 적용. 플레이어 선택(`HandleInputChooseMove` — `GetDefaultMoveTarget` 삭제, HnS `currentPp` 문맥)·`SetFinalChosenTarget`은 그대로. **`GetBattleMoveTarget` `case TARGET_USER_OR_ALLY` hunk 제외 — upstream 결함, 친구 확인 대기(재확인 66).** 아래 "332 제외 hunk" 절 | 32,782,244 |
| 333 | #10654 Fix Copycat failing to copy a move that fainted its target | `d45e2fe2` | 같은 변경을 HnS `MoveEndUpdateLastMoves(void)`에 맞춰 `cv->` → `gCurrentMove`·`gBattlerAttacker`·`gBattlerTarget` | 32,782,260 |
| 334 | #10653 Fix Symbiosis items being consumed as resistance Berries by spread moves | `d1c3e4e5` | 그대로 | 32,782,276 |
| 335 | #10540 Fix trying to remove non existing speaker box | `ba71f99e` | 그대로. HnS 데이터에 `{SPEAKER}` 0건, 매치콜 호출은 늘 speaker가 있음 → 지금 HnS에서 닿지 않는 방어 수정. 포켓기어 출력 변화 없음 | 32,782,292 |
| 336 | #10657 Fix Z-Moves being blocked by move restrictions at selection | `b89fb8e9` | 그대로(325 뒤에만 컴파일) | 32,782,500 |

- 적용 10 커밋, 이미 적용 4. ROM 32,783,748 → **32,782,500 B(−1,248)**, EWRAM 250,408 B·IWRAM 25,516 B 그대로. 커밋마다 `git diff --check` 통과, `make hns -j8` 종료 0, 새 경고 0.
- 커밋을 seq마다 따로 함(327·332는 같은 unit `U-acupressure-10636`이지만 독립 빌드·동작 — 사용자 결정).

## 332 제외 hunk (재확인 66 — 친구 확인)

upstream #10645(`57f11c47ce`)의 `src/battle_util.c` hunk:

```c
+    case TARGET_USER_OR_ALLY:
+        if (!IsBattlerAlive(GetPartnerBattler(gBattlerAttacker)))
+            targetBattler = gBattlerAttacker;
+        break;
     case TARGET_USER:
     default:
         targetBattler = gBattlerAttacker;
```

- `GetBattleMoveTarget`은 `u32 targetBattler = 0;`으로 시작한다(HnS·upstream 1.17.1 `bb1093a7c7`·master 2026-10-11 셋 다 — 이 세션에서 본문 확인). 이 hunk 전에는 TARGET_USER_OR_ALLY가 `default`로 가서 늘 사용자였다.
- hunk 뒤에는 **파트너가 살아 있으면** `targetBattler`가 0(플레이어 왼쪽) 그대로다. 이 함수는 호출 기술(손가락흔들기·잠꼬대·흉내쟁이·조수, `TARGET_DEPENDS` 경로), 명령 불복 무작위 기술, 앵콜·기술 변경 뒤 대상, `BS_GetMoveTarget`이 쓴다.
- 그래서 상대가 손가락흔들기 등으로 경혈찌르기를 부르면 **플레이어 포켓몬의 능력치가 오른다**. 사전 분석 사본의 조사용 테스트(`WITH_RNG(RNG_METRONOME, MOVE_ACUPRESSURE)`, 상대 왼쪽이 손가락흔들기, 상대 파트너 생존)가 기준·327 뒤 PASS, upstream 그대로의 332 뒤 FAIL(플레이어 쪽 +2), 이 hunk만 뺀 뒤 PASS. upstream 테스트에는 이 경로가 없다.
- 제외하면 파트너 기절 → 사용자(hunk의 의도)는 그대로이고 결함만 빠진다. 친구가 "upstream대로"를 고르면 위 4줄을 넣으면 된다(위 결함이 생김).

## 동작 변화

새 배틀 문장 0, 기존 문장 변경 0, 배틀 외 새 텍스트 0. `BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 8행 추가(기술·필드 6, 특성·도구 2).

- 327: 더블에서 경혈찌르기로 고른 아군이 같은 턴 먼저 쓰러지면 `그러나 실패하고 말았다!` 대신 사용자 자신이 오름.
- 332: 파트너가 기절한 더블에서 플레이어·AI가 경혈찌르기를 고르면 사용자(이전: 플레이어는 상대가 1마리 남으면 그 상대의 능력치가 올랐음 — 플레이어 컨트롤러 경로는 테스트 러너가 타지 않아 코드 해석).
- 333: 앞 기술이 대상을 쓰러뜨려도 흉내쟁이가 그 기술을 씀. 같은 이유로 크로스플레임/크로스썬더·윤창 짝 기술 위력 2배 판정(대미지만).
- 334: 공생으로 받은 도구가 전체기 반감열매로 한 번 더 처리되지 않음(도구 팝업·`…입는 데미지를⏎…이 약하게 했다!` 없음, 도구 유지).
- 325·328·336(**HnS 본 게임은 다이맥스·Z크리스탈을 쓸 수 없음** — 테스트·랜더마이저 밖 경로): Z기술·맥스기술 위력이 원래 기술 기준(예: 거다이스트림 90 → 130), 맥스기술에 저주받은바디 미발동, 도발 등 8가지 제한 상태에서도 Z기술 선택 가능, 중력에서는 선택은 되고 실행 때 기존 문장 `마자용은⏎중력이 강해서⏎파이널다이브클래시를 쓸 수 없다!`(셋째 줄 약 125 px — 창 208 px 안, 사전 분석 사본의 임시 한글 테스트로 출력 확인).
- 323·331·335: 동작 변화 없음.

## 친구에게 알릴 것·물을 것

1. **질문(재확인 66):** 332 #10645의 `GetBattleMoveTarget` hunk — 지금은 제외(상대가 손가락흔들기 등으로 경혈찌르기를 부르면 상대 쪽이 오름). upstream대로 넣으면 그 경우 플레이어 포켓몬이 오름(upstream 1.17.1·master 미수정 결함).
2. 알림: 323 수면 클로즈는 HnS에서 꺼져 있어 게임 변화 없음(나중에 켜면 목호·실버 멀티 판정이 맞음). 325·328·336은 HnS 본 게임에서 닿지 않음.
3. 알림: 한글 회귀 `HNS9680 3-04`에 `HP(999)` — 문장 기대는 그대로, 위력 변화로 배틀이 1턴에 끝나지 않게 한 조정.

## 검증

- **빌드:** 위 표.
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j4`, 코드 `b89fb8e9`): 진행 중(결과 나오면 이 줄 갱신)
- **한글 회귀**(`kortests/run.sh <저장소> <라벨> 8`): 전체 테스트 뒤 실행 예정(기대 532/622)
- **세이브 정적 비교**(`pre-323` = 코드 `d4e3b8b6` 빌드, ROM SHA1 `e51b1c5b…`에서 모은 사실): **PASS(FAIL 0, WARN 0)**. 엄격 상수 30·섹터 배치 14칸·`sGFRomHeader` 58개 같음, 세이브 경로 코드 엄격 54·검토 18 같음(녹화 배틀 37 같음), EWRAM·IWRAM 변수 변화 0, 세이브 블록 크기 같음. 새 필드(`SleepClause`·`baseMove`)는 배틀 중 힙 구조체(`gBattleStruct`·`DamageContext`)라 세이브되지 않음.
- **최종 ROM(실기용):** 코드 `b89fb8e9`, 클라우드 `make hns -j8`(`arm-none-eabi-gcc 13.2.1`) → 사용 32,782,500 B, SHA1 `bdc39338ce89f8fbd55c6cab276405ee9ab62e93`.

## 재확인·뒤 PR 메모

- 8w: 해소(327 + 332).
- 8z(새): seq 471.5 #10144가 `SetDamageContextValues`를 들일 때 `ctx->baseMove = gBattleStruct->baseMove;`를 함께 넣는다.
- 8za(새): seq 458 #10659 — `GetDefaultMoveTarget`은 332에서 없어짐. 재확인 66 결정에 맞춰 `GetBattleMoveTarget` 문맥을 맞춘다.
- 66(새): 위 "332 제외 hunk".
- 8t: 325 `CancelerObedience`에 `baseMove` 한 줄만 — HnS 불복종 코드 그대로(`hns_disobedience.c` 상태는 아래 전체 테스트 비교).
- seq 336 뒤 #10661은 upstream 문맥(`gCurrentMove = gBattleStruct->baseMove = gCalledMove;`)과 같아져 그대로 들어갈 것으로 보임(사전 분석 b).
