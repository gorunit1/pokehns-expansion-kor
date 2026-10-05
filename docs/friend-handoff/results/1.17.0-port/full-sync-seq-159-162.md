# full-sync 실제 port 결과 — 묶음 3: seq 159, 161, 162

완료: 순서표 seq 156~163 가운데 남은 3행을 PR별 커밋으로 이식했다(156·157·158·160은 선진행, 163은 이미 적용). 다음은 **묶음 4: seq 166 #9784, 168 #9832, 169 #9835, 170 #9751**이다(164는 #8943 단위, 165는 선진행, 167 #9819는 #10548 직전까지 보류).

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md)
시작 HEAD: `dd1b56fca3`. 작업 컴퓨터: 데스크탑(2026-10-05).

## 요약

| seq | PR | 판정 | 커밋 | ROM 변화 | 비고 |
|---|---|---|---|---:|---|
| 159 | #9786 Clarify isFirstTurn usage with wrappers | 적용(HnS 적응) | `e33ed95e1a` | −96 B | `BattlerJustSwitchedIn`·`IsBattlersFirstTurn` 래퍼. 원래 식과 같은 매크로로 바꾼 빌드가 기준 SHA1과 같음(동작 동일 증명) |
| 161 | #9168 Add Held Berry Animation | 적용(HnS 적응) | `93a0354570` | −64 B | 열매 전용 애니메이션 `B_ANIM_HELD_ITEM_BERRY`. **HnS 아이템 팝업 줄 전부 유지**, 애니메이션 ID만 바뀜. 먹은 열매도 애니메이션 재생(upstream). 숙성 특성 팝업 2곳 변화(쥬스에서 사라짐, 먹는 기술의 능력 상승 열매에서 새로 뜸) |
| 162 | #9693 Remove relearner from party menu | 적용(HnS 적응) | `60f365365a` | −736 B | HnS는 원래 꺼져 있던(`P_PARTY_MOVE_RELEARNER FALSE`) 파티 메뉴 relearner 경로 제거. 남은 메뉴 29개 문자열·순서 같음 |

- 빌드(최종 `60f365365a`): 종료 코드 0, **ROM 32,717,060 B(−896 B) / EWRAM 250,128 B(0) / IWRAM 25,516 B(0)**, SHA1 `a50009270f81f897e4af94a87cc26fcfed5c72e3`(메인 재빌드 같음, `build/localization-logs/hns-20261005-164805-chunk159.log`). 새 경고 0.
- 한글이 든 소스 줄 변경: #9693의 relearner 메뉴 문자열 5줄 삭제(`레벨업 기술`·`알 기술`·`기술머신 기술`·`가르침 기술`·`기술을 배운다`, HnS에서 쓰이지 않던 항목)뿐. 나머지 한글 줄 바이트 불변.
- 전체 테스트: PASSED 2,372 / TOTAL 5,303. 사라진 PASS는 upstream이 둘로 나눈 1개(`Restore HP Item effects do not miss timing after a recoil move` → `(Berries)`·`(Held Items)`, 둘 다 PASS). FAIL → PASS 5(기존 `task not freed`가 풀림). 새 기준 목록 [`test-baseline-seq162.txt`](test-baseline-seq162.txt).
- 한글 회귀(저장소 밖): 열매·팝업 세트 이식 전 71/71 → 이식 후 68/71(의도: 애니메이션 ID를 옛 값으로 고정한 팝업 분석 테스트 3개). 턴 종료 세트는 열매 애니메이션 기대값 3줄을 새 ID로 맞춘 뒤 66/70(이전과 같은 실패 목록, 아래). 그 밖 세트 그대로.
- 세이브: 정적 비교는 묶음 1부터의 표시와 같음(새 차이 없음).

## 공통 사항

- 사전 분석(읽기 전용 3개, 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-159-162/`): `seq159-9786.md`, `seq161-9168.md`(+한글 회귀 `tmp-161/kortests/`), `seq162-9693.md`. 기준 사본은 묶음 1·2 patch를 얹은 코드.
- 적용: 적용 담당 1개가 patch 3개를 그대로 넣었다(손으로 옮긴 곳 0). 지시 `chunk-159-162/APPLY.md`, 기록 `apply/PROGRESS.md`.
- 메인 결정
  - #9786: 코어퍼니셔는 원래 비교 유지(1.17.0·master 같음), 줌렌즈는 1.17.0처럼 래퍼.
  - #9168 결정 1 = (a): 내던지기·벌레먹음·쪼아대기·볼가득채우기의 `HITMARKER_DISABLE_ANIMATION`을 upstream대로 지운다(먹은 열매도 애니메이션 재생, 볼가득채우기는 같은 애니메이션이 두 번 — 1.17.0과 같음). HnS 유지판 `seq161-9168-optional-keep-noanim.patch`는 쓰지 않았다. 결정 2: 나무열매쥬스+숙성 팝업 소멸(upstream 버그 수정)을 출력 변화 문서에 기록. 결정 3: 친구 Q7용 필드 시드 팝업 patch는 이 PR 뒤 구조로 옮긴 `seq161-after-p3-opt-seed-rebased.patch`를 보관(답 뒤 적용).
  - #9693: `config/summary_screen.h`는 문맥만 맞춤(HnS `P_SUMMARY_SCREEN_MOVE_RELEARNER FALSE` 유지).

## seq 159 #9786 (`e33ed95e1a`)

- upstream `09c06ca859`. upstream이 래퍼로 바꾼 `isFirstTurn` 호출부 21곳 중 20곳을 그대로 옮겼다. 빠진 1곳은 변덕쟁이(HnS에 #9798 `e01d5a9352`가 이미 있어 그 조건이 없음). 여기에 HnS 줌렌즈 1곳을 더해 바뀐 호출부는 21곳이다(리뷰 집계. 사전 분석 문서의 "20곳/19곳"은 하나 적게 센 것).
- HnS 사용처 전수: 줌렌즈(`battle_util.c`)는 1.17.0(#10193)처럼 `!BattlerJustSwitchedIn`, 코어퍼니셔는 1.17.0·master·#10593도 원래 비교라 그대로.
- **동작 동일 증명(사전 분석):** 래퍼를 `!=0`/`==2` 매크로로 바꿔 빌드한 ROM SHA1이 기준 사본과 같다. 전체 테스트·실패 사유 바이트 동일, 첫 턴 관련 27파일 252줄 같음. `isFirstTurn==3` assert 탐침에도 걸리지 않음.

## seq 161 #9168 Add Held Berry Animation (`93a0354570`)

- upstream `1079577272`. 새 일반 애니메이션 `B_ANIM_HELD_ITEM_BERRY`와 `sBattleAnims_General` 항목, 열매 스크립트의 `playanimation` ID 변경, `ItemHealHP_RemoveItem`·`ConsumableStatRaiseRet`을 열매/비열매로 분리.
- **HnS 적응**
  - `battle_scripts_1.s` 16 hunk 중 6개를 HnS 아이템 팝업 줄 때문에 손으로 맞춤 — 팝업 `call` 줄을 그대로 두고 `playanimation` ID만 바꿈. 팝업 `call`은 24 → 25개(회복 분리로 `_BerryItemAnim`·`RemoveItem` 두 라벨에 모두 둠, 1.17.0과 같은 자리), 지운 팝업 줄 0.
  - HnS 고유 `LumBerryCureStatusRet`(상태별 메시지 반복)은 그대로 두고 ID만 바꿈.
  - 능력 상승 쪽(`ConsumableStatRaiseRet`)은 HnS에 원래 팝업이 없어 넣지 않음(친구 Q7 대기).
  - upstream 원문의 "공백+탭" 들여쓰기를 고쳐 넣어 #9914(seq 515)는 "이미 적용"이 된다.
  - 선이식 #10047 테스트(`poison_puppeteer.c`)의 애니메이션 ID도 맞춤.
- 한글 문자열 변화 0. 멘탈허브 팝업(`f79f3baf2b`), 시몬·리샘 팝업, 친구 팝업 질문 대상 줄은 바뀌지 않음.
- **출력 변화**(애니메이션·팝업만, 문장 불변)
  - 열매 발동 애니메이션이 `B_ANIM_HELD_ITEM_EFFECT` → `B_ANIM_HELD_ITEM_BERRY`(43곳). 비열매 도구(먹다남은음식·조개껍질방울·검은오물 등)는 그대로.
  - 내던지기·벌레먹음·쪼아대기·볼가득채우기로 먹은 열매도 애니메이션 재생(9곳 새로 생김). 볼가득채우기는 같은 애니메이션이 두 번.
  - 그 뒤 볼주머니 회복도 `B_ANIM_SIMPLE_HEAL`을 재생(이전 `Pausex20`, 리뷰 발견. 문장·팝업 같음).
  - 숙성 특성 포켓몬이 나무열매쥬스로 회복할 때 숙성 팝업이 사라짐(이전 HnS는 쥬스에도 숙성 팝업을 띄웠으나 회복량은 원래 2배가 아니었음 — upstream 버그 수정). `BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 행 추가.
  - **숙성 특성 팝업이 새로 뜨는 경우(리뷰 발견):** 숙성 포켓몬이 벌레먹음·쪼아대기로 빼앗거나 내던지기로 맞아 능력 상승 열매를 먹고, 자신은 열매를 지니지 않았을 때. 옛 `jumpifnotberry BS_SCRIPTING`이 먹은 열매가 아니라 지닌 도구를 봐서 팝업만 빠졌고 상승 폭은 이미 2배였다. 1.17.0 최종형과 같은 upstream 수정이다. 숙성 종은 HnS 야생·트레이너 데이터에 없다(챌린지 랜덤 특성 등에서만 닿음). `BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 행 추가.
- 한글 회귀(`tmp-161/kortests/`, 상태 치료·회복·능력 상승·반감·내던지기·벌레먹음·볼가득채우기·되새김질·티타임·수확·볼주머니·숙성·비열매 대조군 36개 + 팝업 분석 테스트 35개): 이식 전 71/71 → 이식 후 68/71(애니메이션 ID를 옛 값으로 고정한 팝업 분석 테스트 S05·S06·S08). trace 비교: 문장 바이트·팝업(배틀러·도구) 그대로, 바뀐 것은 위 출력 변화뿐.
- 턴 종료 회귀(seq 132 세트)는 열매 애니메이션 기대값 3줄(1-14 수확→되새김질, 3-06 금제 종료→자뭉열매, 3-11 매직룸 종료→자뭉열매)이 옛 ID라 FAIL이 됐다. 스크래치 테스트에 `B_ANIM_HELD_ITEM_BERRY`가 있으면 그것을 기대하는 매크로를 넣어 고쳤고(원본 `*.bak-before-9168`), 다시 66/70(같은 실패 목록, 요약 차이는 매크로로 밀린 줄 번호 하나).

## seq 162 #9693 Remove relearner from party menu (`60f365365a`)

- upstream `7c1033a479`(4파일 −98줄). HnS에서 원래 실행되지 않던 경로라 게임 동작 같음.
- 손으로 맞춘 곳 3: `config/summary_screen.h` 문맥(HnS FALSE 값 유지), `src/data/party_menu.h`에서 지우는 줄이 한글 5줄, `party_menu.c` 두 hunk의 문맥(HnS 함께 걷기 항목 색 분기, `HMsOverwriteOptionActive` 분기 유지).
- 남은 파티 메뉴 항목 29개의 문자열 바이트·콜백·순서가 이식 전 ROM과 같고, 항목 목록 값만 `MENU_TOSS` 이후 5씩 줄어든다(`party_menu.c` 안에서만 쓰임, 세이브·스크립트 무관). 검은먹시티 기술 떠올리기 NPC(`ChooseMonForMoveRelearner`)는 그대로.

## 커밋 리뷰

리뷰 2개(읽기 전용, 저장소 밖 스크래치 사본 빌드). 결과: `/home/hjm0725/hns-sync-work/chunk-159-162/review-berry/REVIEW-RESULT.md`, `review-misc/REVIEW-RESULT.md`.

| 대상 | 판정 | 내용 | 처리 |
|---|---|---|---|
| `e33ed95e1a` #9786 | 문제 없음 | HEAD 문맥에서 다시 증명: #9786만 되돌린 ROM과 래퍼를 원래 비교식 매크로로 바꾼 ROM의 SHA1이 같다(`170baa0d…`). `IsBattlersFirstTurn`과 원래 `!=0`이 갈리는 값 3은 코드상 나오지 않음(쓰기 전수). 줌렌즈는 1.17.0과 글자까지 같고, 코어퍼니셔 원래 비교 유지도 1.17.0과 같다 | 호출부 개수만 정정(21곳) |
| `93a0354570` #9168 | 경미 | 코드 결함 없음. 팝업 호출 24 → 25, 빠진 줄 0, 멘탈허브·시몬·리샘 팝업 유지, 열매/비열매 분리 8곳이 upstream과 같음, 애니메이션 63번은 볼가득채우기 별칭(새 그림 없음). 한글 회귀 재현 + 추가 21개(트릭·매직룸·공생·픽업·수확·볼주머니·숙성·더블 동시 발동 등)에서 문장 바이트·아이템 팝업·HP 같음. **누락 기록 2건:** 숙성 팝업이 새로 뜨는 경우, 볼주머니 회복 애니메이션 추가 | 두 건 모두 위 출력 변화에 추가, 숙성 행은 `BATTLE_MESSAGE_OUTPUT_CHANGES.md`에도 추가 |
| `60f365365a` #9693 | 문제 없음 | 지운 98줄이 upstream과 같음(한글 5줄만 다름), 추가 줄 0. SELECT 빠른 교체·함께 걷기·필드 기술·HM 덮어쓰기·#8943 L/R 파티 순환·노력치 금지·사탕 뒤 복귀 등 HnS 기능 유지. 남은 `sCursorOptions` 29항목과 액션 목록 16개가 이식 전 ROM과 같음 | — |

- [참고, 이 커밋 결함 아님] #9168로 열매·시드 경로의 배틀 스크립트 호출이 한 단계 깊어졌다. 전체 테스트 계측 최대 깊이는 전후 모두 6/8(깊이 6 도달 5 → 13회). HnS `BattleScriptPush`·`Cmd_call`의 assert가 `size < UINT8_MAX`라 8 초과를 막지 못하는데, seq 508 #9892(`4548ffca2e`)가 `ARRAY_COUNT`로 고친다.

## 범위 밖 발견 (고치지 않음)

- 파티 메뉴 "메일" 항목 문자열이 `apdlf`로 들어가 있다(`src/data/party_menu.h` `MENU_MAIL`, "메일"을 한글 자판 상태가 아닐 때 친 것으로 보임). 메일을 지닌 포켓몬을 필드 파티 메뉴에서 고르면 "지닌물건" 대신 나온다. HnS에서는 금빛시티–35번도로 게이트의 켄야 깨비참(`ITEM_RETRO_MAIL` 지님)과 배틀프론티어 교환 나옹으로 닿는다(리뷰 확인). 같은 하위 메뉴의 "메일을 읽는"도 어미가 빠진 형태인데 창 폭 때문일 수 있어 후보로만 둔다. 친구 확인 대상(재확인 목록 10d).

## 실기 확인 항목 (친구용)

1. 열매 발동 애니메이션(자뭉열매·오랭열매·상태 회복 열매·반감열매 등)이 새 열매 애니메이션인지, 아이템 팝업·문장 순서는 이전과 같은지
2. 내던지기·벌레먹음·볼가득채우기로 열매를 먹을 때 애니메이션(볼가득채우기는 두 번)
3. 파티 메뉴 항목·커서·함께 걷기·필드 기술 메뉴, 검은먹시티 기술 떠올리기

## 후속 행 메모

- seq 515 #9914: 이번에 들여쓰기를 고쳐 넣어 "이미 적용".
- 친구 Q7 답이 O면 `chunk-159-162/seq161-after-p3-opt-seed-rebased.patch`(필드 시드·룸서비스·능력 상승 열매 팝업)를 쓴다(원래 `p3-opt-seed.patch`는 이 PR 뒤 적용 안 됨).
- seq 233 #10210: `follower_npc.c`의 label 뒤 선언 1곳(#9835와 같은 유형)을 고친다.
- seq 508 #9892: `battleScriptsStack` assert를 `ARRAY_COUNT`로(위 리뷰 참고).
