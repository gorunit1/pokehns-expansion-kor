# full-sync 실제 port 결과 — 묶음 2: seq 150, 152, 153, 154

완료: 순서표 seq 150~155 가운데 남은 4행을 PR별 커밋으로 이식했다(151은 선진행, 155는 묶음 1에서 적용). 리뷰가 찾은 upstream 결함 2건은 HnS 수정 커밋으로 막았다. 다음은 **묶음 3: seq 159 #9786, 161 #9168, 162 #9693**이다(156·157·158·160은 선진행, 163은 이미 적용).

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md)
시작 HEAD: `0be93173c8`. 작업 컴퓨터: 데스크탑(2026-10-05).

## 요약

| seq | PR | 판정 | 커밋 | ROM 변화 | 비고 |
|---|---|---|---|---:|---|
| 150 | #9717 Adds queued switches for EndTurn switches | 적용(HnS 적응) | `80341f3e3e` | +608 B | 턴 종료 교체를 대기열(`ENDTURN_SEND_OUT_REPLACEMENTS_1~5`)로. HnS 탈출 아이템 팝업 유지. **턴 종료 위기회피 출력 순서가 바뀜(upstream 동작)** |
| — | HnS 수정 | KO 애니메이션 소프트락 | `f8a465e3bc` | +48 B | 교체를 기다리며 볼에 들어간 배틀러에게 KO 애니메이션을 걸어 영구히 멈추던 upstream 결함(HEAD에도 탈출버튼+생명의구슬 경로로 존재, 1.17.0·master 같음) |
| 152 | #9757 Remove redundant SetBattlerAiData call | 적용 | `4de2d11bcd` | −32 B | `config/ai.h` 테스트 상한 hunk 제외(HnS 값 유지) |
| 153 | #9710 Use ctx in GetDefenderPartnerAbilitiesModifier | 적용(HnS 적응) | `1fa0d4891d` | +1,456 B | DamageContext 특성·도구를 배틀러별 배열로. 실전 데미지·메시지 불변(실측), AI 예상 데미지만 upstream 의도대로 바뀜 |
| 154 | #9779 Isolate AI thinking time tests | 적용(HnS 적응) | `97d00ef44b` | 0 B | 사고 시간 테스트 6개를 새 파일로. 상한은 HnS 값 유지 |
| — | 리뷰 후 HnS 수정 | 미래예지 두 번 뒤 위기회피 교체 | `8552b5e9a8` | +32 B | 같은 턴 끝에 미래예지·파멸의소원이 두 번 떨어지면 기다리던 위기회피 교체가 사라지던 #9717 과도기 결함(1.17.0은 seq 228 #10161로 해소) |

- 빌드(최종 `8552b5e9a8`): 종료 코드 0, **ROM 32,717,956 B(+2,112 B) / EWRAM 250,128 B(0) / IWRAM 25,516 B(0)**, SHA1 `92571c71694cbe486cb94d0c7356b449efdf5cb5`. 새 경고 0(6커밋 모두).
- 한글이 든 소스 줄 변경: **0**.
- 전체 테스트: PASSED 2,366 / TOTAL 5,301. 사라진 PASS는 upstream이 이름을 바꾼 1개뿐(`Emergency Exit will trigger due to confusion damage` → `... will not trigger ...`, PASS). 새 FAIL 3(#9717이 넣은 `emergency_exit.c` 테스트, 영문 `MESSAGE` 2·팝업 뒤 바로 끝나는 `task not freed` 1). 새 기준 목록 [`test-baseline-seq154.txt`](test-baseline-seq154.txt).
- 한글 회귀(저장소 밖 166개): 이식 전 seq 150 세트 37/37 → 이식 후 22/37(FAIL 15 = 이름에 `CHANGE EXPECTED`를 붙인 턴 종료 위기회피 순서 변화), 턴 종료 66/70·안개제거 31/34·HnS 수정 16/16·REV 8/8은 그대로.
- 세이브: 정적 비교는 묶음 1과 같은 표시(FAIL 2·WARN 4, #7573 `ZeroPlayerPartyMons`), 새 차이 없음.

## 공통 사항

- 사전 분석(읽기 전용 3개, 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-150-155/`): `seq150-9717.md`(+한글 회귀 `tmp-150/kortests/`, 소프트락 probe `tmp-150/probe/`), `seq153-9710.md`, `seq152-9757.md`·`seq154-9779.md`. 기준 사본은 묶음 1 patch 6개를 얹은 코드.
- 적용: 적용 담당 1개가 patch 5개를 그대로 넣었다(손으로 옮긴 곳 0). 지시 `chunk-150-155/APPLY.md`, 기록 `apply/PROGRESS.md`.
- 메인 결정
  - #9717 결정 1: KO 애니메이션 소프트락 보정을 별도 HnS 커밋으로 넣는다. 결정 2: 턴 종료 출력 순서 변화를 upstream 동작으로 받아들인다.
  - #9757·#9710: `config/ai.h` 사고 시간 상한 hunk 제외(#9779 patch와 맞춤).
  - #9710: D3 과도기 결함(AI가 자기 편을 치는 기술을 계산할 때) upstream 그대로(#10145에서 해소), D4 불복종 자해 데미지 문맥을 혼란 자해와 같이 채우는 HnS 3줄, D5 AI 예상 데미지 변화 수용. 미스트필드 판정은 #10477 최종형(방어자 특성).
  - #9779: 상한 A안(HnS 값 3/8/22/38/29/33).

## seq 150 #9717 Adds queued switches for EndTurn switches (`80341f3e3e`)

- upstream 38 hunk 가운데 37개 그대로, 1개(`battle_scripts_1.s`) 손으로 맞춤: 지워지는 `BattleScript_EjectPackActivates`에 #9946 선이식으로 있던 `SWITCH_IGNORE_ESCAPE_PREVENTION`은 C 쪽 `CanEjectPackTrigger`·`TrySwitchInEjectPack`이 같은 검사를 하므로 지워도 동작이 같다.
- HnS 보존: 탈출버튼·탈출팩 아이템 팝업(`call BattleScript_ItemPopUp_Scripting`)이 새 `BattleScript_EjectItemActivates` 앞부분에 남는다(1.17.0 최종형과 같음). `b07fd88953`과 #8943 단위 HnS 수정은 이 diff와 겹치지 않는다.
- **출력 변화(upstream 동작):** 턴 종료 피해로 위기회피·도망태세가 발동하면 피해 직후 팝업·퇴장하고, 교체는 다음 교체 단계에서 한다. 마지막 포켓몬끼리일 때와 야생 배틀의 경우도 바뀐다. `BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 행을 넣었다.

## HnS: KO 애니메이션 소프트락 보정 (`f8a465e3bc`)

- 교체를 기다리며 볼에 들어간 포켓몬이 있을 때 반대편 포켓몬이 쓰러지면 `AnimateMonAfterKnockout`이 `IsBattlerAlive`만 보고 볼 안의 포켓몬에게 KO 애니메이션을 걸어 `waitanimation`에서 영구히 멈췄다. `IsBattlerPresent`로 2줄을 바꿨다.
- 이식 전 HEAD에도 탈출버튼으로 대상이 빠진 뒤 공격자가 생명의구슬 반동으로 쓰러지는 경로가 있었다(HnS에 갸라도스 @ 탈출버튼 트레이너). upstream `expansion/1.17.0`·master·upcoming에서도 TIMEOUT으로 재현된다.
- probe: 보정 전 P1·P2·P3·P6 TIMEOUT → 보정 뒤 P4만 FAIL(의도). 리뷰는 더 넓은 경우(교체 후보가 남은 상대 기절, 더블 등)도 보정 뒤 PASS이고, 정상 KO 애니메이션이 빠지지 않음을 trace로 확인했다(한글 166개, upstream 32파일 362개 같음).
- 뒤 PR: seq 311 #10542가 같은 함수를 바꾼다(`BATTLE_OPPOSITE` → `GetOppositeBattler`). 그때 `IsBattlerPresent`를 유지한다.

## seq 152 #9757 (`4de2d11bcd`)

- `GetBestMonIntegrated`의 `SetBattlerAiData` 1줄만 삭제(upstream 그대로). `GetBestMonVanilla`·`AI_SelectRevivalBlessingMon`의 같은 줄은 1.17.0에도 남아 둔다.
- 리뷰 계측: 지운 줄이 바꿨을 값을 읽는 곳이 없고, 난수는 HnS 데이터에서 닿지 않는 조합(OMNISCIENT 없는 예측+스마트 교체)에서만 달라진다. 전체 테스트 결과가 줄 복원본과 바이트 동일.

## seq 153 #9710 Use ctx in GetDefenderPartnerAbilitiesModifier (`1fa0d4891d`)

- #9657(seq 120)은 이미 적용. DamageContext의 `abilityAtk/Def`·`holdEffectAtk/Def`를 `abilities[]`/`holdEffects[]`로 바꾸고 사용처를 치환.
- **HnS 적응**: `config/ai.h` 상한 hunk 제외. 미스트필드 판정을 #10477 최종형(방어자 특성)으로 — upstream 그대로면 scratch 테스트 2개 FAIL, 이렇게 하면 seq 304에는 테스트 변경만 남는다. 불복종 자해 데미지 문맥을 혼란 자해와 같이 채우는 HnS 3줄(upstream 1.17.x에는 없음). HnS `GetDamageCalcAbility`(#10145 선반영)는 공격자·방어자 분기만 배열 조회로, 나머지는 seq 383까지 둔다. `tx_Mode_Sturdy`·오라가드·#9735 잔여 줄 등 1.17.0형 30여 곳을 1.17.0 문구와 글자까지 맞춤.
- **실측(사전 분석):** 전체 테스트 상태·실패 사유 바이트 동일, 전 테스트 trace(메시지 바이트·HP·애니메이션·팝업·실전 데미지/상성) 274,565줄 전후 같음. 바뀌는 것은 upstream이 의도한 AI 예상 데미지(틀깨기 공격자가 프렌드가드 무시, 드래곤애로·피뢰침/마중물 파트너 검사가 파트너 자신의 특성을 봄)뿐이고 판정은 바뀌지 않음.
- **기존 결함(이번 PR과 무관, 고치지 않음):** 불복종으로 자해할 때 HP가 실제로 줄지 않는다(`moveDamage`↔`passiveHpUpdate` 불일치, 이식 전부터, upstream 1.17.1도 같음). 재확인 목록 3c.

## seq 154 #9779 Isolate AI thinking time tests (`97d00ef44b`)

- `test/battle/ai/ai.c`의 사고 시간 테스트 6개를 새 `ai_thinking_time.c`로 옮기고 `include/config/ai.h`의 상한 정의를 테스트 파일 상단으로. 상한은 HnS 값(3/8/22/38/29/33). 테스트 이름·개수 불변, 6/6 PASS. ROM 바이트 동일.
- 리뷰 실측 여유: singles 1프레임, doubles 7~9프레임. 뒤 #10046·#10277·#8647에서 상한을 바꿀 때 HnS 실측으로 다시 정한다.

## HnS: 미래예지 두 번 뒤 위기회피 교체 보존 (`8552b5e9a8`, 리뷰 switch F1)

- #9717 뒤 턴 종료 위기회피로 볼에 들어간 포켓몬은 `gSpecialStatuses[].queuedSwitch`를 들고 다음 교체 단계를 기다린다. 같은 턴 끝에 미래예지·파멸의소원이 두 번 떨어지면 두 번째 스크립트의 `clearspecialstatuses`(미래예지 스크립트만 씀)가 이 값까지 지워, 그 포켓몬이 볼 안에 남은 채 다음 턴부터 기술을 쓰고 맞았다(기절 문구도 없음).
- `BS_ClearSpecialStatuses`가 `queuedSwitch`만 보존(+8줄). 리뷰 probe S1·S1p·S1ai·S12가 PASS로 돌아오고, 결함을 전제로 만든 probe 4개는 상황이 생기지 않아 INVALID. 전체 테스트·한글 회귀 166개 결과 같음. ROM +32 B.
- upstream `expansion/1.17.0`은 seq 228 #10161(미래예지 스크립트 `moveendall`)로 해소된다. **seq 228 이식 때 이 HnS 줄을 지운다**(재확인 목록).
- 도달 조건: 위기회피·도망태세 포켓몬(랜더마이저 특성 무작위나 플레이어 반입)이 미래예지를 두 번 맞는 드문 경우.

## 커밋 리뷰 (병렬 3개, 읽기 전용, 결과 `chunk-150-155/review-{switch,ctx,ai}/REVIEW-RESULT.md`)

| 리뷰 | 대상 | 판정 | 요지 |
|---|---|---|---|
| switch | #9717, KO 애니 보정 | #9717 경미, 보정 문제 없음 | 한글 회귀를 재현(이식 전 37/37, HEAD 22/37, trace 166개 분석과 바이트 동일). 싱글·더블·녹화·트레이너 AI·파트너 멀티·두 트레이너·야생 도주·동시 기절·화이트아웃·경험치·탈출팩·탈출버튼·레드카드 실측에서 F1 말고 회귀 없음, 무한 루프 없음. **F1**(미래예지 두 번) → `8552b5e9a8`. F2: 출력 변화 행에 마지막 포켓몬끼리·야생 경우 추가(반영). F3: 보정이 막는 범위가 더 넓고 정상 KO 애니는 그대로 |
| ctx | #9710 | 경미(수정 없음) | 대조 도구로 치환 167쌍·배열 접근 229곳·지역 변수 첨자 27곳 전수 — 공격자/방어자 뒤바뀜 0, 옛 필드 0. ctx를 채우지 않고 읽는 새 경로 0. 독립 계측(기존 테스트 + 무작위 800·특성 집중 1,000전투·표적 13): 실전 줄 365,787줄 전후 같음, AI 줄 차이는 분석과 같은 범주. **경미:** 포켓몬 상성 미리 계산 함수 2개(`CalcPartyMonTypeEffectivenessMultiplier`·`GetOverworldTypeEffectiveness`)가 ctx `{0}`이라 방어 측 특성을 공격 측 배짱·심안 판정에도 읽음(upstream 1.17.0~master 같음, 랜더마이저에서만 닿음, AI 교체·동행 감정·배틀돔 모의만 영향) — 재확인 목록 22b, 고치지 않음. D6(불복종 자해 HP) 실측 확인 |
| ai | #9757, #9779 | 둘 다 문제 없음 | 위 각 절 |

## 실기 확인 항목 (친구용)

1. 턴 종료 피해(모래바람·독·화상 등)로 위기회피·도망태세가 발동할 때: 팝업 → 퇴장 → (다음 단계에서) 교체 화면 → 교체 문장. 퇴장 뒤 남은 턴 종료 문장. 더블에서 두 마리 동시 발동. 턴 종료 탈출팩의 아이템 팝업·교체
2. 탈출버튼으로 대상이 빠진 뒤 공격자가 생명의구슬 반동으로 쓰러지는 경우 게임이 멈추지 않는지(`f8a465e3bc`)
3. 일반 배틀 데미지·AI 행동이 이상하지 않은지(#9710)

## 후속 행 메모

- seq 228 #10161: 미래예지 스크립트가 `moveendall`로 바뀌면 `8552b5e9a8`의 HnS 보존 줄을 지운다.
- seq 304 #10477: 미스트필드 판정은 이미 최종형 — 테스트 변경만 남는다.
- seq 311 #10542: `AnimateMonAfterKnockout`의 `IsBattlerPresent` 유지.
- seq 383 #10145: HnS `GetDamageCalcAbility` 나머지 분기, #9710 D3 과도기 결함 해소 확인.
