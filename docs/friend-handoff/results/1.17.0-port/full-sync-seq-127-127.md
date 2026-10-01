# full-sync 실제 port 결과 — seq 127 (#9655 배틀 메시지 리팩터)

완료: seq 127 unit `U-battlemsg-9655` 이식·전체 테스트·리뷰·기록 완료. 다음 seq는 **128 #9475**(XL, `U-trainerpic-9475`)다. 선진행으로 넣은 seq 130·131·133·134·137·143·148·149·151·156·157·158·160·165는 닿으면 "이미 적용(선진행)"으로 처리한다.

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), 결정: [`FRIEND_REPLY_2026-10-01.md`](../../FRIEND_REPLY_2026-10-01.md)(D1~D7), 사전 조사: [`pre-9655/REPORT.md`](pre-9655/REPORT.md), 메시지 출력 기록: [`BATTLE_MESSAGE_OUTPUT_CHANGES.md`](../../../localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md)
시작 HEAD: `05319fd9b7`(작업 트리 clean). 작업 컴퓨터: 노트북(WSL, ARM 공식 툴체인 `/opt/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi`).

## 요약

| seq | PR | 판정 | 커밋 | ROM 변화 | 비고 |
|---|---|---|---|---:|---|
| 127 | #9655 | 적용(HnS 적응) | `f3b491dfc4` | +480 B | 결정 D1~D7 반영. 새 STRINGID 13개 + 이름 변경 1개(기존 한글 본문 재사용), `ATTACKMISSED` 삭제. 멘탈허브 비트마스크 순차 출력, 상태 회복 표 `gCureStatusStringIds`·`gPartyCureStatusStringIds`, 장판 표 통합 `gRemoveHazardsStringIds`, `gHurtByStringIds`, 회복봉인 턴 종료 스크립트. HnS 보호 줄 2곳 |
| 174 | #9856 | 이미 적용(seq 127 unit에 흡수) | `f3b491dfc4` | 0 | `BattleScript_HurtAttacker`가 처음부터 `printfromtable gHurtByStringIds` 한 번만 출력. 작업 트리에서 `git apply -R --check 9856.patch` 통과(#9856 뒤 형태와 같음) |
| 206 | #10064 | 적용(엔진 1줄) | `47515a949e` | −208 B | `IsNonVolatileStatusBlocked`가 특성 여부와 관계없이 `gBattleScripting.battler = battlerDef`. HnS "이미 ~" 한글은 `{B_DEF}`라 화면 출력 변화 없음. 테스트 5파일 upstream 그대로 |
| — | #10149 | HnS 동등(g1 plan) | — | — | 타오르는불꽃 `gFlashFireStringIds` 매핑과 `PKMNSXMADEYINEFFECTIVE` 한글 유지(D2) |

- 빌드(`47515a949e`): 종료 코드 0, **ROM 32,715,764 B(97.50%, +272 B) / EWRAM 248,936 B(94.96%, 0) / IWRAM 25,516 B(77.87%, 0)**. `pokehns.gba` SHA1 `a6ad839c5c1699c7404846506e551de418d7d4b7`(노트북 툴체인). #9655 커밋 단독 빌드도 종료 코드 0(ROM 32,715,972 B).
- 이식 전 기준(`05319fd9b7`, 노트북): ROM 32,715,492 B로 데스크탑 기록과 같음(SHA1 `9e9feda9…`, 툴체인 차이).
- 새 경고 0(이식 전 전체 빌드 경고 목록과 비교).
- 한글이 든 소스 줄 변경(18줄): `ATTACKMISSED` 삭제 1(D1), `PKMNWOKEUPINUPROAR` 토큰 ATK→EFF 1쌍(D7), `PASTELVEILENTERS`→`PKMNHEALEDPOISON` 이름 변경 1쌍(본문 같음), 새 문장 14줄(아래 "새 STRINGID"). 그 밖의 기존 한글 문장 변경 0.
- `BATTLE_MESSAGE_OUTPUT_CHANGES.md`: 새 행 20개, 기존 행 고침 다수(아래 "출력 변화").
- 전체 테스트: PASS 2,335 / FAIL 2,255 / KNOWN_FAILING 10 / TOTAL 5,253. 사라진 PASS 4건은 모두 설명된다(아래). 새 기준 목록 [`test-baseline-seq127.txt`](test-baseline-seq127.txt).

## 진행 방식

1. 읽기 전용 사전 분석 4개(노트북 스크래치 `/home/jinmo/hns-sync-work/chunk-127/`, 산출물 사본은 [`seq127-9655/`](seq127-9655/))
   - A 문자열(`battle_message.c`, `battle_string_ids.h`): hunk 39개 — 그대로 11 / 수정 9 / 제외 19
   - B 배틀 스크립트(`battle_scripts_1.s`·`_2.s`, `battle_scripts.h`, `battle_script_commands.h`): hunk 41개 — 그대로 23 / 수정 12 / 제외 6, #9856 흡수
   - C C 코드(`battle_script_commands.c`, `battle_util.c`, `battle_hold_effects.c`, `battle_end_turn.c`, `pokemon.c`): hunk 32개 — 그대로 24 / 수정 2 / 제외 6, #10064 1줄, HnS 추가 3곳
   - D 테스트·출력 변경 문서: 테스트 hunk 415개(#9655 408 + #10064 7) — 그대로 398 / 수정 4 / 제외 13
   - 각 파트는 다른 파트와의 "계약"(제공·가정 심볼)을 적었고, 메인이 대조했다. 삭제되는 STRINGID 2개(`ATTACKMISSED`, `PASTELVEILENTERS`)가 upstream과 같고, B·C가 쓰는 새 이름 30개가 모두 A·B 안에 정의돼 있다.
2. 메인이 네 patch를 합쳐 적용 → 빌드 한 번에 통과 → 전체 테스트
3. 병렬 리뷰 4개(R1 문자열·표, R2 스크립트, R3 C 코드, R4 문서·테스트) → 지적 반영 → 재빌드·관련 테스트 → 커밋

## 결정 반영 (친구 2026-10-01)

| 결정 | 반영 |
|---|---|
| D1 C안 | `STRINGID_ATTACKMISSED` 삭제. `Cmd_resultmessage` 빗나감 = `STRINGID_PKMNAVOIDEDATTACK`(`// HnS:`), `gMissStringIds[B_MSG_MISSED]` = `PKMNAVOIDEDATTACK`(upstream). 모든 빗나감이 `…에게는\n맞지 않았다!`. HnS #9929 `AccuracyCheck` 분기 유지 |
| D2 A안 | `PKMNSXMADEYINEFFECTIVE` 한글(`…에게는\n효과가 없는 것 같다...`)과 타오르는불꽃 매핑 유지 |
| D3 A안 | `RanAwayUsingMonAbility` hunk 제외, `무사히 도망쳤다` 유지 |
| D4 | upstream 출력 변화 수용. 기존 한글 재번역 0. 출력 변경 문서에 전부 기록 |
| D5 | 새 ID 14개 본문은 `new_sentences.tsv`의 기존 본문 재사용, 마비는 `몸저림` |
| D6 | a(방벽 해제 진영)·c(사령탑)는 손대지 않음. b: 멘탈허브 사슬묶기·앙코르 분기에 `saveattacker`/`copybyte gBattlerAttacker, sBATTLER`/`restoreattacker`(`@ HnS:`). 문장 그대로 |
| D7 | `STRINGID_PKMNWOKEUPINUPROAR` `{B_ATK_NAME_WITH_PREFIX}` → `{B_EFF_NAME_WITH_PREFIX}`. 조사 `{B_TXT_EUNNEUN}`은 직전에 출력된 이름 끝 글자로 고르므로 그대로 맞음(`battle_message.c` 조사 처리, `korean.c` `GetJongCode`). 소란 기상 3경로 모두 `gEffectBattler` = 깬 포켓몬(R3 확인) |
| 함정 1 | 멘탈허브 Ret·HnS Fling 모두 공용 서브루틴 `BattleScript_MentalHerbCureMessages`를 `call`하고 `jumpifbyte CMP_BITMASK`로 순차 출력. `gMentalHerbCureStringIds`를 읽는 경로 없음. R2가 효과 조합 63가지 × 두 경로를 시뮬레이션해 순서·`return`·공격자 저장 스택이 모두 맞음을 확인 |
| 함정 2 | HnS `BattleScript_BreakScreens*`는 `CMP_COMMON_BITS`·값형 `B_MSG_BREAK_*` 그대로. `CMP_BITMASK`는 멘탈허브 6줄에만 |
| 함정 3 | `gCureStatusStringIds`·`gPurifyStatusCureStringIds`·열매 표 모두 PROBLEM/NORMALIZED 칸 유지. `FREEEZE` → `FREEZE` 교정(헤더·표·코드) 잔여 0 |
| 함정 4 | `{B_SCR}` 회복 문장 5경로 모두 `gBattleScripting.battler` = 회복 대상(R3 확인) |
| 함정 5 | HnS 전용 스크립트 보존: HealerActivates(`HEALERCURE`), DampPreventsAftermath, RanAwayUsingMonAbility, LumBerryCureStatusRet(리샘 반복), BreakScreens, 포이즌힐·솔라파워 무문구, ItemPopUp |
| 함정 6 | `HurtAttacker` 1회 출력. C 호출자 4곳이 `B_MSG_HURT`(까칠한피부·철가시)/`B_MSG_HURT_BY_ITEM`(울퉁불퉁멧·자보·애터열매) 설정 |

## 새 STRINGID (기존 본문 재사용, 새 번역 0)

| ID | 한글 | 원본 |
|---|---|---|
| `SCRCUREDPARALYSIS`/`POISON`/`BURN`/`SLEEP` | `{B_SCR_NAME_WITH_PREFIX}의\n몸저림이 풀렸다!` 등 | `PURIFYTARGET*CURED`와 바이트 동일 |
| `SCRCUREDCONFUSION` | `{B_SCR_NAME_WITH_PREFIX}의\n혼란이 풀렸다!` | `PKMNHEALEDCONFUSION`에서 토큰만 SCR |
| `PARTYCURED{PARALYSIS,POISON,BURN,SLEEP,FREEZE,FROSTBITE}` | `{B_BUFF1}의\n몸저림이 풀렸다!` 등 | 상태별 기존 본문 + 주어 `{B_BUFF1}`(종족명) |
| `PKMNAURORAVEIL` | `PKMNRAISEDDEFSPDEF`와 같은 본문 | 표 매핑은 HnS `PKMNRAISEDDEFSPDEF` 유지라 현재 미사용 |
| `PKMNHEALEDPOISON` | `PASTELVEILENTERS` 이름 변경 | 본문 동일 |
| `PKMNATKNOTLOWERED` | `{B_SCR_NAME_WITH_PREFIX}의\n공격은 떨어지지 않는다!` | `PKMNSXPREVENTSYLOSS`에 `공격`을 넣은 화면과 같음 |

넣지 않은 것: `STRINGID_STICKYWEBDISAPPEAREDFROMYOU`(#9777 seq 475에서 삭제됨. HnS는 플레이어 쪽 끈적끈적네트도 계속 `…TEAM` 문장), upstream `gBreakScreensStringIds`(HnS 방벽 구현 유지), `BattleScript_BerryCureStatusAndConfusionRet`(HnS 리샘 반복 유지).

## HnS가 upstream과 다르게 둔 곳

- `Cmd_resultmessage` 빗나감 ID(D1, `// HnS:`)
- 멘탈허브: HnS 전용 Fling 경로도 Ret과 같은 비트마스크 출력, D6b 공격자 교체(`@ HnS:`)
- `BattleScript_RemoveHazards`: 끈적끈적네트 YOU 분기 없음(`@ HnS:`)
- 방벽(`BreakScreens`), 치유의마음(`HEALERCURE`), 도주 특성, 리샘열매 반복, 습기-유폭·포이즌힐 무문구: HnS 유지(hunk 제외)
- `gCureStatusStringIds` 등에 PROBLEM/NORMALIZED 칸 유지(upstream 표에는 없음)
- **보호 줄 2곳(upstream 1.17.0에도 있는 문제)**
  - `FinalizeCapture` 힐볼(`// HnS:`): `HealStatusConditions()`가 #9655부터 `MULTISTRING_CHOOSER`를 바꾸는데, 잠듦 회복 값 5가 `B_MSG_SWAPPED_INTO_PARTY`(5)와 같다. 파티가 가득 찬 상태에서 잠든 포켓몬을 힐볼로 잡고 교체를 거절하면 박스 전송 문구 대신 묵은 문구가 나온다. 앞뒤로 값을 저장·복원한다(R3 확인).
  - `HealStatusConditions()`(`// HnS:`): 필드 사용과 전투 중 대기 포켓몬(`battler = MAX_BATTLERS_COUNT`)에서 `gBattlerPartyIndexes[4]`를 읽지 않도록 닉네임 버퍼 준비에 조건을 걸었다(리뷰 R2·R3 지적, 출력 변화 없음, ROM +16 B).
- `TryImmunityAbilityHealStatus`의 HnS 중복 2줄 삭제(출력 같음)

## 출력 변화

`BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 기록했다. 요지(이전 → 이후):

- 일반 빗나감(공격 기술과 `accuracycheck BattleScript_MoveMissedPause` 변화기): `그러나 {공격자}의\n공격은 빗나갔다!` → `{대상}에게는\n맞지 않았다!`(D1)
- 회복봉인 턴 종료: `…은(는)\n회복봉인의 효과가 풀렸다!` → `…의\n회복봉인 효과가 사라졌다!`
- 멘탈허브: 마지막 효과 한 문장 → 풀린 효과마다 한 문장(헤롱헤롱→트집→사슬묶기→회복봉인→앙코르→도발), 사슬묶기·앙코르 이름은 실제로 풀린 포켓몬(D6b)
- 방음·방탄·치료방울 동료·옹골참 일격기: 각각의 특성 문장 → `…에게는\n효과가 없는 것 같다...`
- 정신력 풀죽음 방지: 팝업·문구 없음. 피뢰침·마중물: 팝업 추가
- 위협 방지: `…은(는)\n{특성}의 효과로 능력이 떨어지지 않는다!` → `…의\n공격은 떨어지지 않는다!`
- 불요의검·불굴의방패·바람타기·초상투영: 특성명 없는 능력 상승 문장. 깨어진갑옷: `…의\n방어가 떨어졌다!`(오역 경로 해소)
- 젖은접시·건조피부(비)·볼주머니: 문구 없이 회복 애니메이션. 포이즌힐: 회복 애니메이션 추가. 울퉁불퉁멧: 도구 애니메이션 없음
- 가방 상태회복 도구: `{종족}은(는)\n건강해졌다!` → 상태별 문장. **만병통치제·회복약으로 헤롱헤롱만 풀면 혼란 문장이 나온다(upstream 1.17.0과 같은 결함, 아래 질문 2)**
- 제로포밍의 원시 날씨 해제: `햇살이 원래대로 되돌아왔다!` / `강한 비가 그쳤다!`
- 상태 회복 문장 이름: 공격자 → 실제 회복 대상. 동상: `상태이상이 나았다` → `동상이 나았다`
- 소란 턴 끝 기상(D7): 소란 사용자 이름 → 깬 포켓몬 이름
- 기록만(문서 범위 밖): 배틀 아레나 기술 점수 — 방음·방탄·옹골참·습기·위협 방지에 막혀도 −3점이 붙지 않는다(스크립트 ID 교체, upstream 같음). 옹골참 일격기 방지가 `ITDOESNTAFFECT`라 TV "효과 없음" 집계 대상이 된다(upstream 같음).

## 전체 테스트

- 명령: `PATH=/opt/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin:$PATH GITHUB_ACTION=1 make check BUILD=hns -j6`(노트북 13분 53초, 로그 `build/port-check-post127.log`), 목록 추출은 `PORT_INSTRUCTIONS.md`의 `LC_ALL=C` 명령.
- 이식 전(`05319fd9b7`, 노트북): 목록이 `test-baseline-seq167-ahead1.txt`와 바이트 동일(PASS 2,332 / TOTAL 5,241).
- 이식 후: PASS 2,335 / FAIL 2,255 / KNOWN_FAILING 10 / TOTAL 5,253. assertion·Killed 0. 사전 분석 D의 예측(`seq127-9655/part-D-tests.tsv`)과 PASS 증감이 정확히 같다.
  - 사라진 PASS 4: `Raging Bull doesn't remove … if it misses / if the target Protected / if the target is immune`(upstream이 `raging_bull.c`의 중복 테스트 삭제. `break_screens.c`의 짝 `Brick Break, Psychic Fangs, and Raging Bull don't remove … if it misses / if the target Protected / if the target is immune` 3개는 PASS), `Steadfast boosts Speed when the user attempts to move but is flinched`(영문 양성 `MESSAGE` 추가)
  - 새 PASS 7: `Dry Skin causes 1/8th Max HP damage in Sun`, `Dry Skin heals 1/8th Max HP in Rain`, `Poison Heal heals from (Toxic) Poison damage`, `Poison Heal heals from Toxic Poison damage are constant`, `Rain Dish recovers 1/16th of Max HP in Rain`(무문구 회복과 기대가 맞게 됨), 새 테스트 `Light Screen applies for 5 turns`, `Light Screen reduces special damage`
  - 새 FAIL 28: 관련 테스트 파일 15개를 따로 돌려 실패 사유를 확인했다. 25건은 `Unmatched MESSAGE`, 탈피 확률 테스트 3건은 매 시행이 영문 `MESSAGE`에서 실패해 성공률 0으로 집계된 것이다(알려진 한계). 이름 변경·분할(Shed Skin 1→3, Jaboca·Rowap "dies"→"faints", Ice Heal 1→2, "Old Gateu"→"Old Gateau")은 FAIL→FAIL.
- 전체 실행은 리뷰 반영 전 트리에서 했다. 반영한 `HealStatusConditions()` 보호 줄이 닿는 `item_effect/cure_status.c`·`heal_and_cure_status.c`·`move_effect/take_heart.c`(32건)를 다시 돌려 결과가 같음을 확인했다. 문서 수정은 테스트와 무관하다.

## 커밋 리뷰 (병렬 4개, 읽기 전용)

결과 원문은 [`seq127-9655/review-R*.md`](seq127-9655/).

| 리뷰 | 수정 필요 | 경미 | 확인만 | 처리 |
|---|---:|---:|---:|---|
| R1 문자열·표 | 0 | 3 | 6 | 기록(미사용 데이터, `gKOFailedStringIds[B_MSG_KO_MISS]` = EVADED는 미사용 표, 폭 검사 한계) |
| R2 스크립트 | 1 | 4 | 12 | 수정 필요 1 = `HealStatusConditions()` 범위 밖 읽기 → 보호 줄 반영. 정의 없는 `extern BattleScript_DefogClearHazards`는 upstream과 같게 둠 |
| R3 C 코드 | 1 | 2 | 9 | 수정 필요 1은 R2와 같은 건(반영). 힐볼 보호 줄 필요성 확인. 별건 2(아래) |
| R4 문서·테스트 | 1 | 11 | 9 | 수정 필요 1 = 가방 도구 행의 헤롱헤롱 설명 → 실제 동작대로 고침. 경미 M1~M9(빗나감 범위, 초승달의기도, 특성명 촉촉바디·마이페이스·둔감, 경로 설명 등) 반영 |

## 친구에게 물을 것

1. **D5 범위:** 가방 도구 회복(`PARTYCURED*`)도 `몸저림이 풀렸다!`로 넣었다(승인된 `new_sentences.tsv`대로). 답장의 "도구에 의한 회복은 `마비가 풀렸다!`"는 지닌 열매(HnS 열매 문장이 이미 `마비가 풀렸다`)를 가리킨다고 보았다. 가방 도구도 `마비`로 바꿔야 하면 알려 달라(기존 열매 본문 조합이라 새 번역은 아님).
2. **만병통치제·회복약의 헤롱헤롱:** 헤롱헤롱만 걸린 싸우는 포켓몬에게 쓰면 `…의\n혼란이 풀렸다!`가 나온다. `ItemHealMonVolatile()`이 모든 상태를 고치는 도구의 선택값을 혼란으로 고정하기 때문이다(upstream 1.17.0도 같음). HnS에서 고칠지(헤롱헤롱 전용 문장 선택, `// HnS:`) 정해 달라.
3. **기존 범위 밖 쓰기(이번 이식과 무관, 이식 전부터 있음):** 전투 중 잠든 **대기** 포켓몬에게 잠깨는약·만병통치제를 쓰면 `BS_ItemCureStatus`가 `gBattleMons[MAX_BATTLERS_COUNT].volatiles.nightmare = FALSE`로 배열 밖 비트를 지운다(`battle_script_commands.c` `BS_ItemCureStatus`, upstream 같음). 한 줄 보호 조건으로 막을 수 있다. 별도 수정할지 정해 달라.
4. 기록만: 턴 끝 소란 기상 때 `BtlController_EmitSetMonData`가 깬 포켓몬 데이터에 소란 사용자의 status1을 보낸다(upstream 1.17.0 같음, #9616 때 기록한 별건 2와 같음).

## 실기 확인 항목 (친구용)

이식 전 ROM(`05319fd9b7`)과 이식 후 ROM(`47515a949e`)을 같은 `.sav`로 비교한다. 문장·이름·조사·줄바꿈·창 넘침을 함께 본다. 디버그 메뉴(필드 R+START, 전투 중 SELECT)로 포켓몬·도구·특성을 준비할 수 있다.

1. **일반 빗나감(D1):** 명중 랭크를 낮춘 뒤 공격 기술, 사이코시프트·독실 같은 변화기 → `{대상}에게는\n맞지 않았다!`
2. **멘탈허브(D6b 포함):** 멘탈허브를 지닌 포켓몬에게 상대가 앙코르·사슬묶기·도발을 걸기 → 효과마다 한 문장, 사슬묶기·앙코르 문장 이름이 지닌 포켓몬. 더블에서 동료에게 여러 효과를 건 뒤 멘탈허브 내던지기 → 맞은 포켓몬 이름으로 순서대로. 턴 종료 사슬묶기·앙코르 해제 문장은 이전과 같아야 함
3. **방음·방탄·치료방울:** 방음에 소리 기술, 방탄에 탄 기술 → 팝업 + `…에게는\n효과가 없는 것 같다...`. 더블에서 방음 동료가 있을 때 치료방울 → 팝업 없이 같은 문장
4. **위협 방지:** 정신력·둔감·마이페이스·배짱 포켓몬 앞에서 위협 → 팝업 + `…의\n공격은 떨어지지 않는다!`
5. **등장 시 능력 상승·깨어진갑옷:** 불요의검·불굴의방패·초상투영 등장 → 특성명 없는 상승 문장. 깨어진갑옷이 물리 공격을 맞음 → `…의\n방어가 떨어졌다!` → 스피드 상승
6. **무문구 회복:** 비 오는 날 젖은접시·건조피부 턴 종료, 볼주머니 → 팝업 → 회복 애니메이션 → HP, 문장 없음. 포이즌힐 회복 애니메이션
7. **가방 상태회복 도구:** 싸우는 포켓몬과 대기 포켓몬에게 해독제·마비치료제·잠깨는약·만병통치제 → 싸우는 쪽은 별명, 대기 쪽은 종족명으로 상태별 문장. 헤롱헤롱만 걸린 포켓몬에게 만병통치제(질문 2 확인)
8. **정글힐·초승달의기도 동료 회복:** 회복 문장 이름이 동료
9. **소란 턴 끝 기상(D7):** 상대가 더 빠르고 잠든 상태에서 이쪽이 소란피기 첫 턴 → 턴 끝 `{깬 포켓몬}은(는)\n소란스러워서 눈을 떴다!`
10. **회복봉인 5턴 경과:** `…의\n회복봉인 효과가 사라졌다!`
11. **제로포밍:** 끝의대지·시작의바다 날씨에서 테라파고스 테라스탈 → `햇살이 원래대로 되돌아왔다!` / `강한 비가 그쳤다!`
12. **힐볼 포획(보호 줄):** 파티 6마리에서 잠든 야생 포켓몬을 힐볼로 잡고 교체 거절 → 박스로 보냈다는 정상 문구
13. **D6a 확인(이번엔 고치지 않음):** 깨뜨리다로 상대 리플렉터를 깸, 배리어프리 등장 → 진영 표기가 실제 방벽 편과 맞는지. 틀리면 알려 달라
14. **회귀 확인(문서 각 행대로):** 옹골참 버팀·일격기, 정신력+속이기(팝업·문구 없음), 피뢰침 끌어들임(팝업), 울퉁불퉁멧·까칠한피부(문장 1회, 도구 애니메이션 없음), 치유의마음, 도주 특성 `무사히 도망쳤다`, 리샘열매 반복, 습기-유폭, 고속스핀·안개제거 장판 문장(끈적끈적네트 포함)

## 후속 행 메모

- **seq 174 #9856, seq 206 #10064:** 이미 적용(seq 127 unit, `f3b491dfc4`·`47515a949e`).
- **seq 394 #10268(Champions 메시지 사후 수정):** D4대로 이때 Champions 공식 한국어 기준으로 #9655 출력 변화 행을 다시 대조한다.
- **seq 470 #9939:** D1(C안)과 이어진다. `gMissStringIds`·`resultmessage` HnS 줄 유지. 미사용 `gKOFailedStringIds[B_MSG_KO_MISS]` = `PKMNEVADEDATTACK`이 이때 쓰이면 다시 본다(R1 m2).
- **seq 475 #9777:** upstream이 끈적끈적네트 YOU 분기를 지운다. HnS는 처음부터 넣지 않았으므로 그 hunk는 "이미 같음".
- **seq 270 #9730:** 분노의경혈 `TARGETSSTATWASMAXEDOUT` 본문의 누락(`의`, 줄바꿈, 조사)을 그때 함께 처리(REPORT 별건).
- **별건(기록):** 멸망의바디 `PKMNSWILLPERISHIN3TURNS`는 아직 영문이다. D6c 사령탑 두 번째 이름은 별도 과제다. D6a는 실기 확인 뒤 결정한다.
- 스크립트 공백: upstream `BattleScript_AbilityHpHeal`의 ` \t`를 탭으로 맞췄다. 뒤 PR이 이 줄을 문맥으로 쓰면 fuzz가 필요할 수 있다.
