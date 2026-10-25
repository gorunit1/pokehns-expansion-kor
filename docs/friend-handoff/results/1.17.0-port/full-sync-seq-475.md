# full-sync 묶음 42 — seq 475 (#9777 Pokémon Champions 배틀 메시지)

- 요청: 사용자 "혼자할 수 있는 것이라면 ㄱㄱ"(2026-10-25, seq 475 진행).
- upstream: 읽기 전용 사본, `git diff 63631f951f^1 63631f951f`(84파일 — 게임 코드 16, 테스트 68).
- 이전 상태: 2026-09-19·20 선별 이식으로 새 문자열 11개(이미 한글), 효과 단계(매우 굉장/매우 별로), 도구 팝업 대부분, 급소·효과 대상별 문장이 HnS 형태로 들어와 있었다.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 475 | #9777 | `5cd8e493` | 남은 hunk: 도구 회복 문장 삭제·회복 애니, 도구 팝업 추가분, 유폭·까칠한피부 문장 삭제, 연속기 뒤 타격 빗나감 문장 삭제, 방어 우회 문장을 기술 종료 단계로(보이지않는주먹 팝업), 그래스필드 회복 애니, 배틀 TV, `STRINGID_AFTERMATHDMG`→`PKMNWASHURT` 이름, 안 쓰는 `ITDOESNTAFFECTTWOFOES` 복원, upstream 테스트 | 32,963,812 |

이식 전(`c96eea27`, 코드 = `9f8f0858`) 32,963,396 B → **32,963,812 B(+416)**.

## hunk 분류(정적 대조)

- `battle_scripts_1.s`: #9777이 바꾼 라벨 52개를 HnS와 블록 단위로 비교 — 이미 같음 28, 이번에 맞춤 14 + HnS 이름이 다른 2(`BattleScript_BerserkGene`·`BoosterEnergy` = upstream `…Ret`), 의도적으로 다르게 둠 8(아래 "HnS로 둔 것").
- 게임 코드: 효과 단계 플래그·`Cmd_resultmessage`·`Cmd_critmessage`·`Cmd_effectivenesssound`·약점보험·악마의열매(Enigma)·`UpdateMoveResultFlags`·`DoFixedDamageMoveCalc`·선택 화면 효과 표시·아레나는 9/20 이식분으로 이미 같은 동작(HnS는 뒤 PR #10315의 용대미사일 조건까지 있음). 새로 넣은 것은 `battle_tv.c` 3곳, 방어 우회(`battle_move_resolution.c`), 복수형 native.
- 문자열: upstream이 영문을 바꾼 기존 문자열 40여 개(`team`→`side`, 통찰·흰안개·순풍 문장 등)는 **HnS 한글 유지**(정책 5절 1). 토큰이 바뀐 것은 `STRINGID_COULDNTFULLYPROTECT`(`B_DEF`→`B_SCR`) 하나 — 한글은 토큰만(정책 74). `STRINGID_HITXTIMES`의 `{B_BUFF2}` 복수형은 한글 `{B_BUFF1}번 맞았다!`에 필요 없어 문장 그대로(native는 넣음 — `gBattleTextBuff2`만 씀).

## 바뀐 것과 HnS 판단

1. **도구 회복 문장 삭제(친구 답 Q3~Q5, 재확인 8k):** 먹다남은음식·검은오물·조개껍질방울, HP 회복 열매·도구, 혼란 회복 열매에서 "…회복했다" 문장이 없어지고 회복 애니(`B_ANIM_SIMPLE_HEAL`)가 나온다. 열매·도구 소모형의 회복 애니는 upstream #10294가 고치는 대로 `BS_SCRIPTING`(재확인 8zb — 공격자에게 애니가 가지 않음).
2. **도구 팝업 추가분:** 능력 상승 열매(10-05 Q6 A안), 약점보험·눈덩이·빛이끼·충전지·구근·허탕보험, 거울허브, Berserk Gene, 부스트에너지, 끈적끈적바늘·검은오물 피해, 혼란 회복 열매, 주눅구슬. 열매 스크립트는 HnS가 이미 1.17.1 구조(팝업 한 번)라 #9777 중간 형태(팝업 두 번)를 거치지 않는다.
3. **주눅구슬(친구 결정 7 유지, 재확인 30):** 팝업을 붙이고 문장의 도구 이름은 유지(`…은(는) 주눅구슬로\n스피드가 올라갔다!`). upstream은 이 경로에서 `gLastUsedItem`을 정하지 않아 팝업에 이전 도구 이름이 뜰 수 있다(upstream 결함) — HnS는 기존 `setlastuseditem`을 팝업 앞으로 옮겨 주눅구슬이 뜨게 했다(테스트). **친구 질문:** 팝업이 생겼으니 문장에서 도구 이름을 뺄지(upstream형 `…의\n스피드가 올라갔다!`) / 지금처럼 둘지.
4. **유폭·까칠한피부·철가시:** 특성 팝업과 HP 바만(문장 삭제). 니들가드의 같은 문장은 ID 이름만 `STRINGID_PKMNWASHURT`.
5. **연속기 뒤 타격 빗나감:** seq 471 #9983이 넣은 `…에게는\n맞지 않았다!`를 #9777이 다시 뺀다 → `N번 맞았다!`만. 묶음 41 친구 문자의 실기 후보 3번 기대가 이렇게 바뀐다.
6. **방어 우회:** Z기술·다이맥스의 "막아 내지 못하고" 문장이 공격 취소 단계(애니 전) → 기술 종료 단계(피해 뒤). 보이지않는주먹·관통드릴로 방어를 뚫으면 특성 팝업 + 같은 문장(대상마다).
   - HnS 적응: HnS는 #10389의 `breaksThroughProtectFully`를 공격자 기준으로 갖고 있어(upstream은 뒤에 대상 기준으로 옮김) 그 플래그와 #9777의 "대상이 방어 중" 조건을 함께 본다 → 비접촉 기술은 막히고 팝업 없음(테스트). #10434의 첫 줄(`IsBattlerUnaffectedByMove`면 우회 없음, 재확인 89(2))을 함께 넣음.
   - 문장 이름: upstream은 이 경로에서 `gBattleScripting.battler`를 정하지 않는다 — HnS는 대상으로 정해 `{B_SCR}`가 맞는 이름이 되게 했다(테스트).
7. **그래스필드 회복:** 회복 애니 뒤 문장.
8. **배틀 TV:** 매우 굉장/매우 별로/대상별 급소 문장도 점수에 셈(통신 배틀 TV, HnS 실사용 없음).
9. **`STRINGID_ITDOESNTAFFECTTWOFOES`:** seq 471.5 #10144에서 지웠는데 upstream은 #9777에서 쓰이지 않는 채로 다시 두고 1.17.1까지 그대로 → HnS도 이전 한글 그대로 복원(출력 영향 없음).

## HnS로 둔 것(의도적 차이)

- **맹독구슬·화염구슬:** upstream은 팝업 + 일반 상태이상 문장(`…의\n몸에 맹독이 퍼졌다!`·`…은(는)\n화상을 입었다!`, `BattleScript_ToxicOrbActivates`/`FlameOrbActivates`). HnS는 친구가 "정상"으로 확인한 HnS 스크립트(팝업 → `…은(는)\n맹독구슬 때문에 맹독에 중독됐다!`)를 계속 쓴다. upstream 라벨은 뒤 이식을 위해 추가만(호출 안 함). **친구 질문:** 지금처럼 / upstream형.
- `BS_DestroyItemPopup`·팝업 대기: HnS 형태(재확인 36, 394 친구 답 대기). upstream `FreeAbilityPopUpGfx`·`BS_ShowItemPopupScripting`(쓰이지 않음)은 넣지 않음.
- 거울허브 `STAT_CHANGE_MIRROR_HERB`, 위험 요소 제거 스크립트의 HnS 주석(`RemoveHazards`) 등 HnS 줄.
- `BattleScript_BerryCureStatusAndConfusionRet`: HnS에 없는 라벨(뒤 PR) — 대응 없음.

## 테스트

- upstream 테스트 변경 68파일: 65파일은 그대로, 3파일(보이지않는주먹·용대미사일·벽깨기)은 HnS 문맥에 맞춰 손으로. 새 upstream 파일 `test/battle/effectiveness.c`.
- 새 한글 회귀 `test/hns_9777_champions.c` 13개 — **13/13 PASS**. 이식 전 코드(클론 `c96eea27`)에서는 8개 FAIL(바뀐 출력: 먹다남은음식·자뭉열매·유폭·까칠한피부·보이지않는주먹·능력 상승 열매 팝업·주눅구슬 팝업·그래스필드 애니), 5개 PASS(같은 출력: 니들가드 문장, 비접촉 보이지않는주먹, 효과 단계 싱글·더블, 대상별 급소 — 9/20 이식분).
- `test/hns_9939_accuracy.c`의 연속기 뒤 타격 테스트를 새 출력(`N번 맞았다!`만)으로 바꿈.
- **빌드:** `make hns -j8` 종료 0, 새 경고 0.
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 `5cd8e493`): 러너 PASSED 2,865 / TOTAL 5,843. `test-baseline-bgmfont.txt` 대비:
  - 새 PASS 23 = 새 한글 테스트 13 + 이름 바뀐 9939 연속기 테스트 1 + upstream 테스트에서 영어 회복·피해 문장 줄이 빠져(이번 동작 변화) 판정되게 된 6(먹다남은음식 1/16, 자뭉열매 다이맥스, Enigma Berry, 대타출동 HP 비용 열매, 까칠한피부·철가시, 따라큐 까칠한피부) + 이름만 바뀐 2(독치장 대타출동, 보이지않는주먹 Gens 8-9) + upstream 새 테스트 1(`Unseen Fist shows its ability pop-up on each affected target (Champions)`).
  - 사라진 PASS 3 = 이름이 바뀐 테스트 3개(9939 연속기, 독치장 대타출동, 보이지않는주먹) — 새 이름으로 PASS.
  - 새 FAIL 3(영어 `MESSAGE` — upstream 새 테스트): `Effectiveness strings are correct in Single/Double Battles 1/4`, `Critical hits mention each affected target` — 같은 동작을 한글 테스트(`HnS 9777: effectiveness …`, `critical hits …`)로 PASS. 이름만 바뀐 FAIL 3(독치장 연속기, 반짝반짝빛, 스텔스록 G-Max 오타 수정).
  - 새 기준 [`test-baseline-seq475.txt`](test-baseline-seq475.txt)(**다음 비교 기준**).
- **세이브 정적 비교**(이식 전 = 코드 `c96eea27`, 클론 빌드): **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`kortests/run.sh <저장소> kor-b54 8`, 코드 `5cd8e493`): 처음 실행 531/652 — 기대와 다른 27개 모두 이번에 지운 도구 회복 문장(`…로\n체력을 회복했다!`·`…로 인해 조금 회복했다.`)을 기다리던 줄이 원인(실패 줄 확인). 세트 7파일의 그 37줄을 **같은 자리의 회복 애니(`B_ANIM_SIMPLE_HEAL`, 그 포켓몬) 검사**로 바꿈(문장 순서 검사를 유지 — 단순 삭제로는 K7-01이 거짓 PASS가 됨을 확인하고 되돌림). 다시 실행 **558/652, `판정: PASS (기대와 같음)`**(`kor-b54c`).
- **ROM**(코드 `5cd8e493`): SHA1 `819b0d9e21c786a735b9a7b320e73a24cbd2b77d`, 사용 32,963,812 B(파일 33,554,432 B), 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 하지 않음(친구).**

## 실기(mGBA) 후보 — 친구

1. 먹다남은음식(턴 끝)·자뭉열매(HP 절반 이하): 팝업 → 애니 → HP 바, 회복 문장 없음.
2. 능력 상승 열매(치리열매 등)·약점보험: 팝업이 먼저 뜨는지.
3. 유폭·까칠한피부: 팝업과 HP 바만.
4. 트리플킥 중간 빗나감: `N번 맞았다!`만(묶음 41 기대에서 바뀜).
5. (가능하면) 보이지않는주먹(우라오스) vs 방어, Z기술 vs 방어: 피해 뒤 문장.
6. 주눅구슬: 팝업 + 도구 이름 있는 문장.

## 재확인

- 8k 해결(회복 문장 삭제), 8zb 해결(`BS_SCRIPTING`), 8o 해결(시드·회복·주눅구슬 팝업 문맥 정리), 89(2) 해결, 30 주눅구슬 항목은 질문으로(새 행 93), 맹독구슬·화염구슬 질문(새 행 94).

## 다음

- seq 476부터 순서표대로.
