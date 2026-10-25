# full-sync 묶음 41 — seq 470~474 (명중 판정 단위 `U-accuracy-9939` + 472·473)

- 요청: 사용자 "ㅇㅋㅇㅋ 가자"(2026-10-25, seq 470 #9939 진행).
- upstream: 읽기 전용 사본 `/home/user/pokeemerald-expansion-upstream`, 커밋마다 `git diff <c>^1 <c>`.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 470 | #9939 | `9850ebd6` | 데미지 기술 명중 판정을 공격 취소 단계로(`CANCELER_ACCURACY_CHECK`), 빗나감 문장 대상별, `MOVE_RESULT_PROTECTED`, `gMissStringIds` 삭제, 미래예지 착탄 명중 판정 삭제 | 32,963,748 |
| 471 | #9983 | `7adfa3f7` | 연속기 뒤 타격 빗나감 문장 + 맞은 횟수, 테라셸 판정을 취소 단계로 | 32,963,476 |
| 471.5 | #10144 | `e3181496` | 대상별 대미지 문맥, 사이코필드 차단 순서, 타입 무효를 취소 단계에서 대상마다(두 상대 한 줄 문장 삭제) | 32,963,300 |
| 472 | #10034 | (이미 적용) | 흡수력: 빗나가면 회복 없음·능력치 버퍼 — HnS에 이미 있음(코드·테스트) | — |
| 473 | #10388 | `f675759b` | 치료방울·아로마테라피 대상 `TARGET_USER_AND_ALLY`(아군 초식에 막힘) | 32,963,300 |
| 474 | #10606 | `00bfe24a` | 용대미사일이 고르지 않은 대상의 명중도 판정 | 32,963,236 |
| (8z) | #10616 남은 줄 | `4be4b109` | `SetDamageContextValues`에 `ctx->baseMove = gBattleStruct->baseMove;`(재확인 8z — 325에서 함수가 없어 미뤘던 줄, upstream 최종형과 같은 자리) | 32,963,204 |
| (테스트) | — | `0f1284db` | 471.5에서 손본 `Trainer Slide: Multi: Self Mon Unaffected` 장면의 연속 `NOT` 두 줄을 `NONE_OF`로(러너가 INVALID 처리 → 원래대로 영어 문장 FAIL) | — |

이식 전(`f1d4a9a6`) 32,963,748 B → **32,963,204 B(−544)**.

## PR별 내용과 HnS 판단

### 470 #9939 — 명중 판정을 공격 취소 단계로

- **흐름:** 데미지 기술(위력 > 0)은 `attackcanceler` 안의 새 마지막 단계 `CancelerAccuracyCheck`에서 대상마다(아군 → 왼쪽 상대 → 오른쪽 상대 슬롯 순) 명중을 본다. 빗나간 대상마다 `BattleScript_BattlerAvoidedAttack`(그 대상 이름)이 **기술 애니 전에** 나오고, 모두 빗나가면 애니 없이 끝. 용대미사일은 첫 대상이 빗나가면 조용히 파트너로 바꾼다. 일격기 vs 옹골참은 여기서 팝업 + `…에게는 효과가 없는 것 같다...`(`ITDOESNTAFFECTSCR`). 스크립트 `accuracycheck`는 이제 변화기만(인자 없음, 빗나가면 `BattleScript_MoveMissedPause`). 연속기 재타격은 명중 단계로 돌아간다.
- **스크립트:** `accuracycheck X` 36곳 → `accuracycheck`(upstream과 같은 수), 일반 공격·`MoveBlockedByDynamax`·미래예지 착탄의 줄 삭제. HnS가 `ButItFailed`를 넘기던 변화기 10곳(9/15 이식 때 #9929 형태로 `TargetAvoidsAttackEnd`로 보냈던 것)도 같은 빗나감 경로로 — 문장은 같다(D1). HnS 전용 `TargetAvoidsAttackEnd`·`MissedTarget`(대상 바꿔치기 트릭, #10687 HnS형)·`FlingMissed`·`AbilityAvoidsDamage` 삭제.
- **결과 플래그:** 방어는 `MOVE_RESULT_PROTECTED`(HnS가 비워 둔 비트 16), 막기 특성·프랭크스터 등은 `FAILED`. `moveResultFlags`는 HnS가 이미 u32. 파일별 `MISSED`/`PROTECTED` 사용 수가 upstream 결과물과 같음(정적 대조).
- **한글 문장(D1 유지, 정책 74):** upstream은 `STRINGID_PKMNEVADEDATTACK`을 `STRINGID_PKMNAVOIDEDATTACK`으로 바꾸고 `STRINGID_BATTLERAVOIDEDATTACK`(B_SCR)을 새로 둔다(영어는 둘 다 "avoided the attack!"). HnS는 친구 결정 D1("모든 빗나감은 `…에게는\n맞지 않았다!`")대로 **둘 다 기존 문구** `{이름}에게는\n맞지 않았다!`(이름 토큰만 B_DEF/B_SCR). 옛 `…은(는)\n공격을 피했다!`는 쓰이지 않게 되어 지움 → 씨뿌리기를 이미 씨 심긴 상대에게 쓸 때 문장이 D1로 바뀜(메시지 표).
- **방어 애니(10-24b A안):** HnS `BattleScript_TargetProtected`의 애니는 그대로, 문장은 `printstring STRINGID_PKMNPROTECTEDITSELF`(표 삭제에 따라).
- **미래예지·파멸의소원:** 떨어질 때 명중 판정이 upstream처럼 없어짐 → HnS 빗나감 분기(재확인 45)째 정리. 떨어질 때 빗나가지 않는다(메시지 표).
- 분함의발구르기는 빗나간 대상도 "효과 없음"으로 셈, 매직가드 없는 대폭발(Mind Blown) 반동은 실패 플래그와 무관하게.

### 471 #9983 — 연속기 빗나감 문장

- 연속기의 뒤 타격이 빗나가면 `{대상}에게는\n맞지 않았다!` → `N번 맞았다!`(`BattleScript_BattlerAvoidedMultiHit`). 이전 `resultmessage`의 연속기 갈래 삭제.
- 테라셸 타입 왜곡은 취소 단계에서 결정(`ShouldTeraShellDistortTypeMatchups(ctx)` 공개), 대미지 계산은 적용만, AI는 자체 판정. `damagecalc`가 `isCrit`·`fixedBasePower`를 직접 넣지 않음 — HnS `DoMoveDamageCalc`가 `isCrit = IsCriticalHit(ctx)`로 이미 넣고, `fixedBasePower`는 새 문맥이라 0(코드 확인).

### 471.5 #10144 — 대상별 문맥, 사이코필드, 타입 무효

- 대상마다 `SetDamageContextValues`로 문맥 하나. 사이코필드 선공 차단은 공중 회피 다음·방어 앞, `CanMoveBeBlockedByTarget`은 흡수 특성·프랭크스터·가루 기술만(AI 쪽도 같은 목록). 타입 무효면 그 자리에서 `BattleScript_DoesntAffectScripting`(트레이너 슬라이드 "무효" 유지 — HnS 함수명 `TryInitializeTrainerSlideMonUnaffected`).
- **문장:** 두 상대 모두 무효일 때의 한 줄 `STRINGID_ITDOESNTAFFECTTWOFOES`(한글 포함) 삭제 → 대상마다 한 줄, 기술 애니 전(메시지 표).

### 472 #10034 — 이미 적용

- 흡수력이 빗나가면 회복 없음(`!IsBattlerUnaffectedByMove`), 능력치 이름 버퍼 — HnS에 이미 있다(코드·upstream 테스트 `Strength Sap fails if move missed`).

### 473 #10388 — 치료방울·아로마테라피 대상

- `TARGET_USER` → `TARGET_USER_AND_ALLY`. 싱글은 변화 없음. 더블에서 아군 초식이 아로마테라피를 흡수(공격 +1), 사용자 회복은 그대로. 기술 데이터 변경은 이 대상 칸 2개뿐.

### 474 #10606 — 용대미사일

- 명중 단계가 스마트 대상 기술도 슬롯 순으로 돌아, 고르지 않은 대상의 명중도 본다. upstream 테스트 이름 3개 바뀜(사라진 이름 3 = 새 이름 3).

## 검증

- **빌드:** 커밋마다 `make hns -j8` 종료 0, 새 경고 0(`warncheck.sh`).
- **새 한글 회귀 테스트:** `test/hns_9939_accuracy.c` 7개(단일·변화기·더블 전체기 순서·옹골참 일격기·씨뿌리기·분함의발구르기·연속기 뒤 타격), `test/hns_10144_unaffected.c` 3개 — **10/10 PASS**. 이식 전 코드(클론 `f1d4a9a6`/`7adfa3f7`)에서는 더블 전체기 이름·씨뿌리기 문장·두 상대 무효 한 줄·한쪽 무효 순서가 FAIL(바뀐 출력), 나머지는 PASS(같은 출력).
- `hit: FALSE`는 명중률 100 미만에서만 빗나감을 고정하므로(`RandomPercentage` 100 이상은 판정 없이 명중) 테스트는 반짝가루를 들린다(upstream 테스트와 같은 방법).
- **이전 동작 확인(클론, 이식 전 코드 `f1d4a9a6`, 임시 테스트 — 커밋 안 함):** 연속기 뒤 타격 빗나감은 이전엔 `N번 맞았다!`만(대상 문장 없음), 아로마테라피는 아군 초식에 막히지 않음(공격 그대로) — 메시지 표의 "이전" 칸이 맞다(새 기대 2개 FAIL, 이전 기대 2개 PASS).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 `00bfe24a`): 러너 PASSED 2,837 / TOTAL 5,818. `test-baseline-seq469.txt` 대비:
  - 새 PASS 16 = 새 한글 테스트 10 + upstream 새 테스트 6(`Dragon Darts checks the unchosen target for accuracy`, 이름 바뀐 용대미사일 2, `Psychic Terrain prevents priority status moves before Magic Bounce can bounce them`, `Stomping Tantrum will deal double damage if hit into an immunity ability`, `Water Veil lets Rawst and Lum Berries cure Mold Breaker burns first`).
  - 사라진 PASS 3 = upstream이 이름을 바꾼 용대미사일 테스트 3개(새 이름 2개 PASS + 위 새 테스트 — 474 절).
  - 새 FAIL 3(영어 `MESSAGE`라 원래 판정 안 됨): `Aromatherapy will be blocked on ally by Sap Sipper but not user`, `Spread Moves: Doesn't affect any target`(옛 `…both opposing mons` 대체), `Spread Moves: Earthquake fails due to accuracy in order of ally, left foe, right foe` — 같은 동작은 한글 테스트로 PASS.
  - `Trainer Slide: Multi: Self Mon Unaffected` FAIL → INVALID: 471.5에서 고친 장면의 연속 `NOT` 탓. `0f1284db`로 고쳐 FAIL(이전과 같음)로 돌아온 것 확인.
  - 새 기준 [`test-baseline-seq474.txt`](test-baseline-seq474.txt)(b48 목록 + 위 멀티 줄 FAIL, **다음 비교 기준**).
- **재확인 8z 뒤**(코드 `4be4b109`+`0f1284db`): `(Z-MOVE)`·`Dynamax:`·멀티 슬라이드 묶음 134개가 b48과 같음(`Z-Move power is based on the base move`·`Max Move power is based on the base move` PASS), `HnS 9939` 7/7·`HnS 10144` 3/3 PASS.
- **세이브 정적 비교**(이식 전 = 코드 `11a628da`, 클론 빌드): **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-b48 4`, 코드 `00bfe24a`): **558/652, `판정: PASS (기대와 같음)`**.
- **ROM**(코드 `0f1284db`): SHA1 `841971fca9804c7242f36e16b944119ffbc5d68c`, 사용 32,963,204 B(파일 33,554,432 B), 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1, 새 경고 0.
- **실기: 하지 않음(친구).**

## 실기(mGBA) 후보 — 친구

1. 공격 기술이 빗나갈 때(모래뿌리기 등으로 명중 낮춤): 기술 이름 → 애니 없이 `상대 …에게는 맞지 않았다!`.
2. 더블 지진·하이퍼보이스가 여러 대상에게 빗나가거나 고스트 둘에게 무효: 대상마다 이름이 맞는 한 줄씩, 애니 전.
3. 트리플킥·트리플악셀이 중간에 빗나감: `…에게는 맞지 않았다!` → `N번 맞았다!`.
4. 미래예지가 떨어질 때 빗나가지 않는지(같은 턴 상대가 회피 랭크를 올려도).
5. (가능하면) 더블 아로마테라피 + 아군 초식.

## 재확인

- 8y 해결(더블 빗나감 이름), 45 해결(미래예지 착탄 분기 정리), 8z 해결(`4be4b109`, 위 테스트), 27의 #10687 HnS형 항목 해소(#10631 블런더폴리시 줄은 이미 upstream과 같음).
- 43(미래예지 착탄 숨김 가드) 다시 확인: 착탄 스크립트는 `attackcanceler`를 안 거쳐 새 명중 단계와 무관, 가드 그대로, 임시 테스트 2/2 PASS(커밋 안 함). 행은 열어 둠.
- 2(미래예지 빗나감 스택 누수)는 이미 해결 — 이번에 빗나감 분기 자체가 없어짐.

## 다음

- seq 475 #9777(Champions 배틀 메시지, L, kor=Y — 이미 9/15 선별 이식분과 대조 필요).
