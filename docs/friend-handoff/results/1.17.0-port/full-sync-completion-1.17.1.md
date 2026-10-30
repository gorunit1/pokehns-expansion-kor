# full-sync 완료 단계 — 1.17.1 기준 (2026-10-30)

- 요청: 사용자 "버전 상수 맞추고 완료 단계도 혼자 할 수 있는 거라면 ㄱㄱ"(2026-10-30).
- 근거: [`1.17.1-extension-plan.md`](1.17.1-extension-plan.md) 6절 4("마지막 묶음 뒤 `RECHECK_BEFORE_COMPLETION.md` 전체 재확인 → 최종 handback"), seq 514 보류분(버전 상수).
- 상태: **순서표 seq 1~556(목표 upstream `expansion/1.17.1`) 이식 끝 + 버전 상수 1.17.1 + 재확인 표 점검 끝.** 남은 것은 친구 결정·한글 문구·실기(아래 4절)뿐 — 코드로 혼자 더 할 수 있는 항목은 없음.

## 1. 커밋

| 커밋 | 내용 | ROM 영향 |
|---|---|---|
| `17ec8742` | `include/constants/expansion.h`를 1.17.1 파일 그대로(1·17·1, `EXPANSION_TAGGED_RELEASE TRUE`) — seq 514 보류분. **플래그는 친구 답으로 `6f13ac74`에서 FALSE로(6절)** | 디버그 메뉴 "Development Build" 문자열 빠짐, ROM 헤더 RHH 버전 필드. 사용 −16 B, 뒤 데이터 위치 이동 |
| `8436136c` | 재확인 88 (1): 테스트 러너 `TestRunner_Battle_RecordItemPopUp`에 upstream "Did you mean: ITEM_POPUP" 갈래(1.17.1 그대로) | 없음(테스트 빌드만) |
| `1fee21fa` | 재확인 8m: HnS가 넣은 도구 팝업 3곳(`BattleScript_ItemHealHP_Ret`·`ConsumableItemStatRaise`·`AirBalloonMsgInRet`) 앞에 `waitabilitypopup`(`@ HnS:`) | 배틀 스크립트 +16 B, 겹치는 장면에서만 도구 팝업이 늦게 뜸 |

SHA1: 이식 끝(`1c4126d3`) `ba9c8243…` → 버전 상수(`17ec8742`) `d224fea6…` → 지금(`1fee21fa`) **`39f00d95b8414660879502db9f13a1ebed377a31`**.

## 2. 재확인 표 점검 (자세한 것은 `RECHECK_BEFORE_COMPLETION.md` 0절과 각 행 끝 "1.17.1 완료 점검 2026-10-30")

- **방법:** 해결 칸이 비었거나 "해결" 표시가 없던 행을 upstream `expansion/1.17.1` 태그(필요하면 `origin/master`·`origin/upcoming`)와 HnS HEAD 코드로 대조. 동작을 다시 봐야 하는 행은 임시 테스트(커밋 안 함).
- **닫은 행:** 8(#10445 seq 298·#10223 seq 390), 8g, 8n, 8s, 8t, 8u(seq 513), 8x(seq 500), 27, 39(seq 548), 48(seq 286), 52(seq 500), 57, 61(seq 541), 76(seq 516·517 암묵 `waitstate`), 100(1.17.1에 같은 줄), 106(seq 516), 60 코드 부분.
- **점검 중 찾은 빠뜨림 2건(계획에 있었는데 그 seq 때 넣지 않은 것):**
  - **88 (1)** — seq 413 때 "#9807(seq 511)이 `queuedEventsFailIndex`를 들이면 upstream 갈래 추가"로 적었는데 511 때 "#9807에는 제안이 없다"로만 보고 넘어감(갈래 자체는 #10321 블록에 있었음). upstream 줄은 도구 이름을 `gAbilitiesInfo[item].name` + `%C`로 찍어 제안 속 도구 이름이 엉뚱한 글자 1개로 나옴(1.17.1·master·upcoming 같음, 테스트 출력만) — 그대로 받음.
  - **8m** — 친구 Q10("특성 팝업이 끝난 뒤 도구 팝업, 겹치지 않게") + `friend-reply-2026-10-05.md` 리뷰가 "seq 394 #10268만으로는 안 풀리니 그때 HnS 추가로 세 라벨 앞에 `waitabilitypopup`"이라 정했는데 394 묶음에서 빠짐. 계획대로 넣음. 출력 변화 표 1행(`BATTLE_MESSAGE_OUTPUT_CHANGES.md`).
- **재확인 1(더블배틀 가로챈 멀리짖기 assert) 다시 재현:** 임시 테스트(더블, 플레이어 왼쪽 가로채기 → 상대 왼쪽 멀리짖기)가 지금 코드에서 `MoveEndUpdateLastMoves` → `RestoreAttacker` `No savedBattlerAttackers` assert. 1.17.1·master 같은 함수(미수정), upcoming은 이 함수가 바뀜(`RestoreTarget` 추가 — 범위 밖이라 고쳐졌는지 확인 안 함). 친구 결정(2026-10-04: 임시로 고치지 않음)대로 열어 둠.
- **upstream 미수정 확인(HnS 줄 유지):** 3·20·21b·22b·101·102·103·107·111·112 — 각 행 끝.

## 3. 검증

- **버전 상수(`verify_seq.sh 514v 9aecbc8e`):** 이식 전 클론 빌드 SHA1 `ba9c8243…`(= 묶음 87). 저장소 전체 재빌드 종료 0, SHA1 `d224fea6de4df1bea41ecf1267a9fb32ee3f1fd3`, 새 컴파일 경고 0. 바뀐 오브젝트는 `debug.o`·`rom_header_rhh.o`뿐. 전체 테스트 목록 `test-baseline-seq556.txt`와 **바이트 같음**. 세이브 정적 비교 PASS(FAIL 0, WARN 0). 한글 회귀 555/652 `PASS (기대와 같음)`.
- **점검 수정 2건(`verify_seq.sh 557c 17ec8742`, 코드 = `1fee21fa`):** 이식 전 클론(`17ec8742`) 빌드 `d224fea6…`. 저장소 전체 재빌드 종료 0, SHA1 `39f00d95…`, 새 컴파일 경고 0. 전체 테스트(`GITHUB_ACTION=1 make check BUILD=hns -j8`) 목록 `test-baseline-seq556.txt`와 **바이트 같음**(새 PASS 0, 사라진 PASS 0 — 팝업 대기는 헤드리스에서 즉시 통과, 제안 갈래는 실패 때 출력만). 세이브 정적 비교 PASS(FAIL 0, WARN 0). 한글 회귀(`kor-s557c`) 555/652 `PASS (기대와 같음)`, 상태 줄 차이 0.
- **ROM**(커밋 `1fee21fa`): SHA1 `39f00d95b8414660879502db9f13a1ebed377a31`, 사용 32,977,956 B, 파일 33,554,432 B, 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 하지 않음(친구).**

## 4. 최종 인계 — 친구에게 남은 것

1. **결정이 필요한 것:** 3b(춤추기 — 지금 코드 유지 / 되돌리는 수정안 b·b2), 23(`B_MULTI_HALF_TEAMS FALSE` 배틀타워 멀티 확인), **34(#10180 노가드 × 프리폴 hunk — "full-sync 마지막에 넣을지 결정"으로 남긴 항목)**, 37(팝업 등장 대기 수정 2 patch), 70 (a)(b), 71(빈 트레이너 슬라이드 표 축소), 72(디버그 Set Ability 범위 검사 1줄), 82(`B_OVERWORLD_WEATHER_OVERRIDE GEN_8`), 89 (1)(드래곤애로우 AI 테스트 FAIL 메모). ~~108(매지컬리프)~~ → 친구 결정 반영(6절).
2. **한글 문구:** 9(텔레키네시스 조사), 10(섬광 `으로부터`), 10c(`Time to Gigantamax!`), 10g(직업 이름 공백 깨짐), 11(멸망의바디 영문), 42(볼 바꾸기 영문), 47(불사르기 조사).
3. **실기:** 이번 8m 팝업 순서(턴 끝 젖은접시·건조피부·아이스바디·선파워 → 먹다남은음식·검은오물, 일렉트릭·사이코메이커 → 시드, 자기과신 → 조개껍질방울, 풍선 — 두 팝업이 겹치지 않고 멈추지 않는지), 디버그 메뉴 버전 표시, 재확인 25·60·65·81·104·111, 각 묶음 결과 문서의 실기 후보(묶음 47~87, 합본 문자로 보낸 것).
4. **기록만 / 그대로 둔 HnS 차이:** `RECHECK_BEFORE_COMPLETION.md` 0절 목록 — upstream이 고치면 그때 정리.
5. **사용자 확인:** HnS `README.md` 크레딧 줄("pokemonHnS-expansion 2.0 is Based off … 1.15.1") — HnS 문장이라 손대지 않음.

## 6. 친구 답 2026-10-30 반영 — 매지컬리프·엔진 버전 표시 ([`FRIEND_REPLY_2026-10-30.md`](../../FRIEND_REPLY_2026-10-30.md))

| 커밋 | 내용 |
|---|---|
| `860292b1` | **재확인 108:** `gBattleAnimMove_MagicalLeaf`의 HnS `duration=32` 두 줄(+`@ HnS:` 주석)을 upstream #10401의 `create_razor_leaf_cutters target_both=FALSE` 한 줄로. 매크로가 펼치는 두 줄은 `duration=22`·`wave_amplitude=±20`·`target_both=FALSE`(선택한 상대만), 그 뒤 `delay 30` → 타격(`create_basic_hitsplat_sprite` 2개) 그대로 — 잎이 이전보다 10프레임 빨리, 타격보다 약 8프레임 먼저 닿음(친구가 받아들임, 타격은 앞당기지 않음). upstream 연출·코드 구조에 맞춘 선택이고 본가와 같다고 검증한 변경은 아님 |
| `6f13ac74` | **엔진 버전 표시:** `include/constants/expansion.h` 버전 숫자 1·17·1은 `17ec8742`에서 이미 바꿨고, 그때 함께 TRUE로 바꾼 `EXPANSION_TAGGED_RELEASE`를 친구 요청대로 **FALSE로 되돌림**(HnS는 upstream 태그 릴리스가 아님). HnS 게임 이름·타이틀·한글패치 배포 버전은 손대지 않음 |

- **upstream 대조(정적):** 매지컬리프 애니 블록(`gBattleAnimMove_MagicalLeaf` ~ 다음 라벨 앞)이 `expansion/1.17.1`과 **같음**, `create_razor_leaf_cutters` 매크로 정의도 같음, 파일 안 `create_razor_leaf_cutters` 사용 6곳(나뭇잎 3·`target_both=FALSE` 1·`TRUE` 1·매지컬리프 1)이 1.17.1과 같은 목록 — 나뭇잎·시드플레어 대상 수정은 그대로.
- **엔진 버전 정적 확인(빌드 = `6f13ac74`):** ROM `0x204`의 `RHHEXP` 헤더 뒤 4바이트 = `[1, 17, 1, 0]`(major·minor·patch·flags — flags 0 = 태그 릴리스 아님). 디버그 메뉴 `BufferExpansionVersion`(`build/hns/src/debug.o` 디스어셈블)이 `v` + 1·17·1을 숫자로 찍고 `sText_Unreleased`("\nDevelopment Build")를 붙임 → 화면 표시는 `v1.17.1` + `Development Build`(실기 미확인).
- **빌드:** 두 커밋 각각 `make hns -j8` 종료 0, 새 경고 없음(빌드 로그 `warning` 0줄). `860292b1`만: SHA1 `a9de000282bf9d6e6ae7924e37316f5cc3744c70`, 사용 32,977,956 B(매크로가 같은 크기로 펼쳐짐). `6f13ac74`: SHA1 **`83e55d273168e3c9cb243d5440b84c468dca7d3c`**, 사용 32,977,972 B(+16 — "Development Build" 문자열), 파일 33,554,432 B, 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실행하지 않은 검사:** 전체 `make check`·세이브 정적 비교·한글 회귀는 이 두 커밋만으로는 다시 돌리지 않음(친구 말대로 — 애니 스크립트 1곳과 버전 플래그뿐). 같은 날 이어서 하는 버그 수정 8건 묶음 끝의 전체 검증에 함께 들어감. **mGBA 실기: 미실행(친구)** — 매지컬리프 잎 이동·타격 타이밍, 디버그 메뉴 `Expansion version` 표시.
- **구분:** 엔진 버전이 1.17.1로 표시되는 것 = 이식 기반이 1.17.1이라는 뜻. 최종 검증(재확인 표 남은 행·친구 실기)·통합·배포 완료는 아님.

## 5. 다음

- 친구 답·실기 결과가 오면 위 4절 행별로 반영. 1.17.2·upcoming은 범위 밖(확장 계획 7절).
