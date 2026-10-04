# 지닌 도구 발동 팝업 — Pokémon Champions 대조 (친구 요청 C)

친구 mGBA 보고([`mgba-check-seq127-138.md`](mgba-check-seq127-138.md) 3절)의 요청 C에 대한 분석 결과다. 2026-10-04, 데스크탑. **확정 수정(멘탈허브, 지닌 경우+내던지기)은 `f79f3baf2b`로 넣었다**([`friend-requests-abc-2026-10-04.md`](friend-requests-abc-2026-10-04.md)). 선택 수정은 patch로 준비해 두었고, 친구 답을 받은 뒤 넣는다.

- 기준 코드: HnS `cc0576a543`(#8943 단위 적용 뒤, 팝업 경로는 #8943과 무관), upstream `expansion/1.17.0`.
- 분석: 읽기 전용 3개(저장소 밖 `/home/hjm0725/hns-sync-work/chunk-popup/`). P1 = HnS 발동 경로 전수(R01~R56)와 1.17.0 대조, P2 = Champions 근거·설계·멘탈허브 patch, P3 = 통합 대조표·선택 patch.
- Champions 근거
  - 도구별로 확실한 것은 **친구가 실기로 확인한 멘탈허브 O** 하나다.
  - upstream 저자들의 일반 규칙: "As of Pokemon Champions, Item Popups show the item being activated"(이슈 #10255), "Recovery items now use item pop-ups"(#9777 설명). 도구별 공개 목록은 없다(확인일 2026-10-04).
  - Champions에 실제로 있는 지닌 도구는 데이터 덤프 `projectpokemon/champout` v1.2.0(2026-09-09)으로 85종(메가스톤 제외)을 확정했다. **맹독구슬·화염구슬, #10268 열매 5종(애슈·미클·자보·애터·랑사)은 Champions에 없다.**
- 검증 방법: 테스트 러너에 배틀 이벤트 기록(trace)을 붙인 스크래치 사본에서 팝업 → 애니메이션 → 문장 순서와 한국어 문장 바이트를 실측했다(코드/데이터 검증). mGBA 실기는 하지 않았다.

## 1. 요약 (요청 1~3에 대한 답)

- HnS의 지닌 도구 발동 경로는 **56개**다. 그중 Champions에 있는 도구가 걸린 경로는 25개, Champions에 없는 도구라 대조할 수 없는 경로는 31개다.
- **Champions와 다른(또는 다를 가능성이 있는) HnS 경로: 7개.** 모두 HnS에 팝업이 없다.
  - **확정 1개:** 멘탈허브를 지닌 포켓몬이 발동(R17).
  - **가능성 6개(친구 확인 필요):** 멘탈허브를 내던지기로 맞음(R18), 먹다남은음식(R25), 조개껍질방울(R20), 풍선 등장(R09), 필드 시드(R02), 풍선 터짐(R10, 근거 약함).
- 나머지 16개 경로는 HnS에 이미 팝업이 있어 같다고 추정한다(하양허브, 상태 회복 열매, 과사·오랭·자뭉열매, 반감열매, 기합의띠, 선제공격손톱, 레드카드, 탈출버튼, 생명의구슬, 울퉁불퉁멧, 노말주얼 등).
- HnS에만 팝업이 있는 시몬열매(R28)·리샘열매 혼란만 치료(R30)는 Champions에 있는 도구이고 일반 규칙에 맞아 **유지**를 권한다.
- 팝업이 아닌 차이(참고): 회복 도구의 회복 문장(1.17.0 #9777은 지움, HnS는 유지), 반감열매 문장 시점(#10431).

## 처리 계획

| 대상 | patch(저장소 밖) | 상태 | 비고 |
|---|---|---|---|
| R17 멘탈허브 보유 | `p2-mentalherb.patch` hunk 1 | **확정, (3) 단계에서 HnS 커밋** | `BattleScript_MentalHerbCureRet`의 애니메이션 앞에 `call BattleScript_ItemPopUp_ScriptingNoFlush`(하양허브와 같은 형태, `@ HnS:`). upstream 1.17.0·upcoming에도 없는 팝업이다. 실측 12개 시나리오: 팝업 → 애니메이션 → 문장, 한국어 문장·애니메이션 바이트 불변, 기존 `Mental Herb`·`Fling` 테스트 결과 같음, ROM +16 B |
| R18 멘탈허브 내던지기 | 같은 patch hunk 2 | 친구 Q1 답에 따라 | X면 이 hunk만 뺀다 |
| R20·R25 회복 도구 | `p3-opt-healhp.patch` | 친구 Q3·Q4 답 대기 | 1.17.0과 같은 자리. 회복 문장은 남긴다 |
| R09 풍선 등장 | `p3-opt-airballoon-in.patch` | 친구 Q6 답 대기 | upstream upcoming #10759와 같은 자리 |
| R10 풍선 터짐 | `p3-opt-airballoon-pop.patch` | 친구가 O라고 할 때만 | upstream도 넣지 않음 |
| R02 필드 시드 | `p3-opt-seed.patch` | 친구 O여도 seq 161(#9168) 뒤 권장 | 같은 라벨의 룸서비스·스탯 열매에도 팝업이 생기고 seq 161과 충돌 1 |
| R42 빨간실 팝업 위치 | `p3-opt-destinyknot.patch` | 선택(Champions 무관) | 팝업이 보유자 반대편에 뜨는 버그(HnS·1.17.0 같음, upcoming #10759가 고침). 1줄 |
| 테스트 가드 | `p3-opt-testguard.patch` | 팝업을 더하는 선택 patch와 함께 | 헤드리스 테스트에서 팝업 태스크가 남아 `task not freed`로 PASS 26개가 FAIL이 되는 것을 막는다(#10321 seq 413의 1줄 선반영, 게임 ROM 불변) |

- #10268(seq 394)·#9777(seq 475)에 있는 나머지 팝업(Champions에 없는 도구 18경로)은 확정 결정 A안대로 그 PR 순서에서 이식한다. 지금 앞당기면 seq 161·181·394 hunk와 충돌한다.
- 확정 patch는 #10268·#9777·#9168 hunk의 적용 가능성을 바꾸지 않는다(전후 `git apply --check` 비교).

### 선택 patch 상세 (모두 HEAD 기준, `a/`·`b/`, 모드·index 줄 없음)

| patch | 대상 | 바꾸는 것 | trace | 전체 테스트 PASS→FAIL | 이식 충돌 평가 |
|---|---|---|---|---|---|
| `p3-opt-healhp.patch` | R20 조개껍질방울, R25 먹다남은음식(+검은오물 회복) | `ItemHealHP_Ret` 첫 줄에 1.17.0과 같은 `call BattleScript_ItemPopUp_Attacker` | 5개, PASS | 14개(모두 `task not freed`) | seq 475: #9777 hunk 024가 F-→--. 3-way 병합은 **충돌 0** |
| `p3-opt-airballoon-in.patch` | R09 풍선 등장 | `AirBalloonMsgInRet`에 `call BattleScript_ItemPopUp_Scripting`(upcoming #10759와 같은 자리) | 8개, PASS | 3개(같은 원인) | seq 161·181·394·475 hunk **변화 0** |
| `p3-opt-airballoon-pop.patch` | R10 풍선 터짐 | `AirBalloonMsgPop`에 같은 호출(upstream 참고 형태 없음) | 4개, PASS | 1개(같은 원인) | **변화 0** |
| `p3-opt-seed.patch` | R02 시드. 같은 라벨을 쓰는 R01·R19·R35·R37 포함 | `ConsumableStatRaiseRet_Anim`의 확인 뒤·애니메이션 앞에 1줄 | 9개, PASS | 8개(같은 원인) | seq 161: #9168 hunk 011이 F-→--, 3-way **충돌 1**. seq 181(#9730 이동)도 손으로 옮겨야 함 |
| `p3-opt-destinyknot.patch` | R42 빨간실(Champions 무관 버그) | `TryDestinyKnotAttacker`의 `_Attacker`→`_Target` | 3줄 배틀러 수정, PASS | 0개 | seq 475: #9777 hunk 005가 -R→--(그 hunk를 건너뛰면 됨) |
| `p3-opt-testguard.patch`(보조, 테스트 전용) | 헤드리스 테스트 | `CreateItemPopUp`에 #10321의 `gTestRunnerHeadless` 조기 반환만 미리 넣음 | — | **0개.** 기준 대비 FAIL→PASS 41개. 6개 patch와 함께 넣어도 목록이 가드 단독과 같다 | #10321 hunk는 원래 `--`. 변화 0. ROM SHA1이 HEAD와 같다 |

- **ROM 크기:**
  - 선택 patch는 하나씩 넣으면 ROM 사용량이 0 B 늘었다(정렬 여유 안에서 스크립트 +5 B).
  - 7개 patch를 모두 넣으면 32,715,044 B(+32 B)다.
- **위험:**
  - 팝업을 더하는 4개 patch는 #10321(seq 413) 전까지 테스트 목록에 PASS→FAIL 26개를 만든다. 원인은 헤드리스 테스트에서 팝업 태스크가 남는 것이다. 게임 동작 문제는 아니다.
  - seed patch는 Champions에 없는 열매 3경로와 룸서비스에도 팝업을 앞당겨 붙인다. 또 바로 다음 큰 단위인 seq 161과 충돌한다.
  - Champions 쪽 판정은 멘탈허브 보유를 빼면 모두 추정이다.
- **계획서 표기 오류:** "이스타·미클·자보·애터·캄라"가 **3곳**에 있다. 실제 #10268 대상은 애슈·미클·자보·애터·랑사다.

## 2. 판정 기준

| Champions 팝업 판정 | 기준 |
|---|---|
| **O(친구 실기 확인)** | 친구 mGBA 보고(`mgba-check-seq127-138.md` 3절). 멘탈허브 보유뿐이다 |
| **규칙상 O 추정 — 친구 확인 필요** | Champions에 도구가 있고(`tmp-P2/champions-held-items.tsv`, champout v1.2.0) 발동 표시가 있는 경로다. 근거는 일반 규칙뿐이다: #10255 "as of Pokemon Champions, Item Popups show the item being activated"와 #9777 "Recovery items now use item pop-ups". 도구별 공개 근거는 없다(P2 2절) |
| **대조 불가(Champions에 없음)** | tsv 85종에 없는 도구 |
| **해당 없음(표시 없는 도구)** | HnS·1.17.0 모두 발동 연출·문장이 없거나, 선택 화면 거부 문장이다 |

"HnS가 다른가"는 다음처럼 쓴다.

| 표기 | 뜻 |
|---|---|
| 다름(확정) | 친구 실기로 확인된 차이 |
| 다를 가능성 | HnS 팝업 X, 규칙상 Champions O |
| 같음(추정) | HnS 팝업 O, 규칙상 Champions O |
| — | 대조 불가 또는 해당 없음 |

## 3. 최종 대조표 (R01~R56)

- 도구·팝업 O/X는 P1 표(코드 대조)를 따른다. "1.17.0"은 `expansion/1.17.0`이다.
- "Champions에 있나"의 숫자는 tsv의 `champ_id`다.
- 처리 제안의 seq는 `port_sequence.tsv` 순번이다.

| # | 도구 | HnS 팝업 | 1.17.0 팝업 | Champions에 있나 | Champions 팝업 판정 | HnS가 다른가 | 처리 제안 |
|---|---|---|---|---|---|---|---|
| R01 | 룸서비스 | X | O | 없음 | 대조 불가(Champions에 없음) | — | seq 161(#9168)·475(#9777)에 맡김. seed patch를 넣으면 함께 O |
| R02 | 일렉트릭·그래스·미스트·사이코시드 | X | O | 있음(881~884) | 규칙상 O 추정 — 친구 확인 필요 | **다를 가능성** | 선택 `p3-opt-seed`(친구 O 확인 뒤, 가능하면 seq 161 뒤 1.17.0 형태로, 5.2절) |
| R03 | 분노의유전자 | X | O | 없음 | 대조 불가(Champions에 없음) | — | seq 440·475 |
| R04 | 부스트에너지 | X | O | 없음 | 대조 불가(Champions에 없음) | — | seq 440·475 |
| R05 | 하양허브 보유 | O | O | 있음(214) | 규칙상 O 추정 — 친구 확인 필요 | 같음(추정) | 유지 |
| R06 | 하양허브 내던지기 | O | O | 있음(214, 내던지기 사용 가능) | 규칙상 O 추정 — 친구 확인 필요 | 같음(추정) | 유지 |
| R07 | 흉내허브 | X | O | 없음 | 대조 불가(Champions에 없음) | — | seq 475 |
| R08 | 왕의징표석·예리한이빨 | X | X | 왕의징표석만(221) | 해당 없음(표시 없는 도구) | — | 유지(친구 질문 선택 항목 11) |
| R09 | 풍선 등장 | X | X | 있음(541) | 규칙상 O 추정 — 친구 확인 필요(upcoming #10759가 같은 자리에 팝업을 넣음) | **다를 가능성** | 선택 `p3-opt-airballoon-in` |
| R10 | 풍선 터짐 | X | X | 있음(541) | 규칙상 O 추정 — 친구 확인 필요(근거 약함: upcoming도 넣지 않음) | **다를 가능성(낮음)** | 선택 `p3-opt-airballoon-pop`. 친구가 O라고 할 때만 |
| R11 | 울퉁불퉁멧 | O | O | 있음(540) | 규칙상 O 추정 — 친구 확인 필요 | 같음(추정) | 유지 |
| R12 | 약점보험 | X | O | 없음 | 대조 불가(Champions에 없음) | — | seq 181·475 |
| R13 | 눈덩이·빛이끼·충전지·구근 | X | O | 없음 | 대조 불가(Champions에 없음) | — | seq 181·475 |
| R14 | 자보·애터열매 | X | O | 없음 | 대조 불가(Champions에 없음) | — | A안 seq 394(#10268) |
| R15 | 의문열매 | O | O | 없음 | 대조 불가(Champions에 없음) | — | 유지 |
| R16 | 허탕보험·목스프레이 | X | O | 없음 | 대조 불가(Champions에 없음) | — | seq 181·475 |
| R17 | **멘탈허브 보유** | X | X | 있음(219) | **O(친구 실기 확인)** | **다름(확정)** | **확정 `p2-mentalherb` hunk 1** |
| R18 | 멘탈허브 내던지기로 맞음 | X | X | 있음(219, 내던지기 사용 가능) | 규칙상 O 추정 — 친구 확인 필요(하양허브 내던지기와 대칭) | **다를 가능성** | `p2-mentalherb` hunk 2. 친구가 X라고 하면 이 hunk만 뺀다 |
| R19 | 악키·타라프열매 | X | O | 없음 | 대조 불가(Champions에 없음) | — | seq 161·475. seed patch를 넣으면 함께 O |
| R20 | 조개껍질방울 | X | O | 있음(253) | 규칙상 O 추정 — 친구 확인 필요 | **다를 가능성** | 선택 `p3-opt-healhp` |
| R21 | 생명의구슬 | O | O | 있음(270) | 규칙상 O 추정 — 친구 확인 필요 | 같음(추정) | 유지 |
| R22 | 끈적끈적바늘 이동 | X | X | 없음 | 대조 불가(Champions에 없음) | — | 유지 |
| R23 | 끈적끈적바늘 턴 종료 피해 | X | O | 없음 | 대조 불가(Champions에 없음) | — | seq 475 |
| R24 | 맹독구슬·화염구슬 | O | O | 없음 | 대조 불가(Champions에 없음) | — | 유지(친구 "정상") |
| R25 | 먹다남은음식(+독 타입의 검은오물 회복) | X | O | 먹다남은음식만(234) | 규칙상 O 추정 — 친구 확인 필요 | **다를 가능성** | 선택 `p3-opt-healhp`(검은오물 회복도 함께 O. 1.17.0과 같음) |
| R26 | 검은오물 피해 | X | O | 없음 | 대조 불가(Champions에 없음) | — | seq 475 |
| R27 | 버치·유루·복슝·복분·배리열매 | O | O | 있음(149~153) | 규칙상 O 추정 — 친구 확인 필요 | 같음(추정) | 유지 |
| R28 | 시몬열매 | O | **X** | 있음(156) | 규칙상 O 추정 — 친구 확인 필요 | 같음(추정) | **유지** |
| R29 | 리샘열매(상태이상 포함) | O | O | 있음(157) | 규칙상 O 추정 — 친구 확인 필요 | 같음(추정) | 유지 |
| R30 | 리샘열매(혼란만) | O | **X** | 있음(157) | 규칙상 O 추정 — 친구 확인 필요 | 같음(추정) | **유지** |
| R31 | 오랭·자뭉열매(+나무열매쥬스) | O | O | 오랭·자뭉(155·158) | 규칙상 O 추정 — 친구 확인 필요 | 팝업 같음(추정). 회복 문장은 다를 가능성(4.2) | 팝업 유지. 문장은 seq 475 별도 결정 |
| R32 | 과사열매 | O | O | 있음(154) | 규칙상 O 추정 — 친구 확인 필요 | 같음(추정) | 유지 |
| R33 | 무화·위키·마고·아바·파야(싫지 않은 맛) | O | O | 없음 | 대조 불가(Champions에 없음) | — | 유지 |
| R34 | 같은 열매(싫어하는 맛) | X | O | 없음 | 대조 불가(Champions에 없음) | — | seq 475 |
| R35 | 치리·용아·캄라·야타비·규살열매 | X | O | 없음 | 대조 불가(Champions에 없음) | — | seq 161·475. seed patch를 넣으면 함께 O |
| R36 | 랑사열매 | X | O | 없음 | 대조 불가(Champions에 없음) | — | A안 seq 394 |
| R37 | 스타열매 | X | O | 없음 | 대조 불가(Champions에 없음) | — | seq 161·475. seed patch를 넣으면 함께 O |
| R38 | 미클열매 | X | O | 없음 | 대조 불가(Champions에 없음) | — | A안 seq 394 |
| R39 | 부적금화·행운의향로 | X | X | 없음 | 대조 불가(Champions에 없음). 표시도 없음 | — | 유지 |
| R40 | 파워풀허브 | O | O | 없음 | 대조 불가(Champions에 없음) | — | 유지 |
| R41 | 빨간실(보유자=공격자, 헤롱헤롱바디) | O | O | 없음 | 대조 불가(Champions에 없음) | — | 유지 |
| R42 | 빨간실(보유자=헤롱헤롱 기술의 대상) | O(배틀러 틀림) | O(배틀러 틀림) | 없음 | 대조 불가(Champions에 없음) | — | 선택 `p3-opt-destinyknot` |
| R43 | 주얼 | O | O | 노말주얼만(564) | 규칙상 O 추정 — 친구 확인 필요 | 같음(추정) | 유지 |
| R44 | 반감열매 18종 | O | O | 있음(184~200, 686) | 규칙상 O 추정 — 친구 확인 필요 | 팝업 같음(추정). 문장 시점은 다를 가능성(4.2) | 유지(#10431 seq 483) |
| R45 | 기합의머리띠·기합의띠 | O | O | 있음(230·275) | 규칙상 O 추정 — 친구 확인 필요 | 같음(추정) | 유지 |
| R46 | 선제공격손톱 | O | O | 있음(217) | 규칙상 O 추정 — 친구 확인 필요 | 같음(추정) | 유지 |
| R47 | 애슈열매 | X | O | 없음 | 대조 불가(Champions에 없음) | — | A안 seq 394. P1 위험 1(팝업 배틀러) 확인 필요 |
| R48 | 레드카드 | O | O | 있음(542) | 규칙상 O 추정 — 친구 확인 필요 | 같음(추정) | 유지 |
| R49 | 탈출버튼 | O | O | 있음(547) | 규칙상 O 추정 — 친구 확인 필요 | 같음(추정) | 유지 |
| R50 | 탈출팩 | O | O | 없음 | 대조 불가(Champions에 없음) | — | 유지 |
| R51 | 특성가드 | O | O | 없음 | 대조 불가(Champions에 없음) | — | 유지 |
| R52 | 클리어참 | X | X | 없음 | 대조 불가(Champions에 없음) | — | 유지(upcoming #10759는 넣었지만 1.17.x 범위 밖) |
| R53 | 주눅구슬 | X | O | 없음 | 대조 불가(Champions에 없음) | — | seq 181·475. P1 위험 2 |
| R54 | 연막탄 | X | X | 없음 | 대조 불가(Champions에 없음) | — | 유지 |
| R55 | 방진고글 | X | X | 없음 | 대조 불가(Champions에 없음) | — | 유지 |
| R56 | 구애 시리즈·돌격조끼 선택 제한 | X | X | 구애스카프만(287) | 해당 없음(선택 화면 문장) | — | 유지 |

집계(56행):

| 분류 | 행 수 | 해당 행 |
|---|---:|---|
| Champions에 있는 도구가 걸린 경로 | 25 | R02 R05 R06 R08 R09 R10 R11 R17 R18 R20 R21 R25 R27~R32 R43~R46 R48 R49 R56 |
| ├ O(친구 실기 확인) | 1 | R17 |
| ├ 규칙상 O 추정 | 22 | |
| └ 해당 없음 | 2 | R08 R56 |
| 대조 불가 | 31 | |
| HnS가 다른 경로 | 7 | 확정 1, 가능성 6 |
| 같음(추정) | 16 | |

P1의 "1.17.0에만 팝업" 21개 가운데 Champions에 있는 도구는 R02·R20·R25 셋뿐이다. 나머지 18개는 해당 PR seq에서 이식하면 된다.

## 4. 친구 요청 3의 답: Champions와 다른(또는 다를 가능성이 있는) HnS 경로

### 4.1 팝업 차이 (7개)

| 순위 | # | 도구·경로 | HnS 현재 (스크립트, 출력 순서) | Champions 판정 | 근거 | 처리 | 친구 질문 |
|---:|---|---|---|---|---|---|---|
| 1 | R17 | 멘탈허브 보유 | X. `MentalHerbCureRet`(:7199) 애→문 | **O(실기)** | 친구 mGBA | **확정 patch** | — |
| 2 | R18 | 멘탈허브를 내던지기로 맞음 | X. `MentalHerbCureFling`(:7207) | O 추정 | 하양허브 내던지기(R06)에는 팝업이 있다. Champions에 내던지기가 있다 | `p2-mentalherb` hunk 2 | Q1 |
| 3 | R25 | 먹다남은음식 턴 종료 회복 | X. `ItemHealHP_Ret`(:7340) 애→문→HP | O 추정 | #9777 "Recovery items now use item pop-ups", 1.17.0 같은 라벨에 팝업 | `p3-opt-healhp` | Q3 |
| 4 | R20 | 조개껍질방울 회복 | X. 같은 라벨 | O 추정 | 같음 | `p3-opt-healhp` | Q4 |
| 5 | R09 | 풍선 등장 문장 | X. `AirBalloonMsgInRet`(:7306) 문만 | O 추정 | #10255 일반 규칙. upcoming #10759(`e2dcade9ba`)가 같은 자리에 팝업을 넣고, upcoming 테스트가 `ITEM_POPUP(player, ITEM_AIR_BALLOON)`을 등장 문장 앞에서 검사한다(`test/battle/hold_effect/air_balloon.c:19`) | `p3-opt-airballoon-in` | Q6 |
| 6 | R02 | 필드 시드 4종 | X. `ConsumableStatRaiseRet`(:7405) 애→능력 상승 문장 | O 추정 | 1.17.0 `ConsumableItemStatRaise`(1.17.0:217)에 팝업이 있다. upcoming `terrain_seed.c`에 `ITEM_POPUP` 16개 | `p3-opt-seed` | Q7 |
| 7 | R10 | 풍선 터짐 | X. `AirBalloonMsgPop`(:7311) 문→소 | O 추정(**약함**) | 일반 규칙뿐. 1.17.0·upcoming 모두 팝업이 없다. upcoming 테스트도 터짐 앞 팝업을 검사하지 않는다 | `p3-opt-airballoon-pop`(친구 O일 때만) | Q6 |

### 4.2 팝업이 아닌 차이 (참고, 이번 patch 범위 밖, 새 문장·문장 삭제 없음)

| # | 차이 | HnS | 1.17.0 / Champions 근거 | 결정 시점 |
|---|---|---|---|---|
| R20·R25·R31 (+R15·R33) | 회복 문장 | `PKMNSITEMRESTOREDHPALITTLE`, `PKMNSITEMRESTOREDHEALTH` 출력 | #9777(seq 475)이 회복 문장을 지우고 `B_ANIM_SIMPLE_HEAL`로 바꿨다. PR 설명은 "no longer prints anything pertaining to recovering HP"다. HnS는 2026-09-20 별도 이식 때 아이템 회복 문장을 유지했다(특성 회복인 포이즌힐·아이스바디는 문장을 지움, `BATTLE_MESSAGE_OUTPUT_CHANGES.md:66-68`) | seq 475에서 별도 결정. 친구 질문 Q3~Q5로 근거 확보 |
| R44 | 반감열매 문장 시점 | 팝업→애니메이션→문장이 모두 공격 애니메이션 **전** | #10431(seq 483)은 문장을 공격 뒤(move end)로 옮긴다 | seq 483. Q9 |

### 4.3 HnS에만 팝업이 있는 R28·R30

판단은 **유지**다. 두 도구 모두 Champions에 있고, "발동하면 팝업"이라는 일반 규칙에 맞는다. upstream #9777이 이 두 경로에 팝업을 넣지 않은 것은 누락으로 본다(같은 PR이 다른 상태 회복 열매에는 팝업을 넣음). 친구 Q8로 확인한다.

## 5. 계획서 표기 오류 ("이스타·미클·자보·애터·캄라")

- 실제 #10268(`94780b0c86`) hunk 008·009의 팝업 대상은 4개 라벨이다: `CustapBerryActivation`, `MicleBerryActivate`, `JabocaRowapBerryActivates`, `BerryFocusEnergy`.
- HnS `src/data/items.h`의 이름으로는 **애슈열매**(`ITEM_CUSTAP_BERRY`, :12118), 미클열매(:12099), 자보열매(:12137), 애터열매(:12155), **랑사열매**(`ITEM_LANSAT_BERRY`, :12043)다.
- **캄라열매**(`ITEM_SALAC_BERRY`, :11986)는 #9777의 `ConsumableBerryStatRaise` 대상(R35)이다.
- "이스타"라는 이름의 HnS 도구는 없다(`src/`·`include/`·`data/` 0건).
- 이 표기는 `60cbbf5712`(2026-09-28)에서 처음 들어왔고, `c339f2b749`(A안 확정)가 그대로 옮겼다.

| # | 위치 | 원문 | 비고 |
|---|---|---|---|
| 1 | `docs/friend-handoff/results/pokeemerald-expansion-1.17.0-full-sync-plan.md:167` | "#10268: **A안 확정.** 이스타·미클·자보·애터·캄라 열매 발동 시 upstream 아이템 팝업…" | **오류**(이스타→애슈, 캄라→랑사) |
| 2 | `docs/friend-handoff/results/1.17.0-sync-plan/g2_battle_fixed_b_plan.md:122` | "팝업이 없는 이스타·미클·자보/애터·캄라 열매에는 HnS 아이템 팝업 helper를 붙인다" | **오류** |
| 3 | `docs/friend-handoff/results/1.17.0-sync-plan/g2_battle_fixed_b_plan.tsv:59` | "팝업이 없는 이스타·미클·자보/애터·캄라 열매(7693/7702/7714/7439)" | **오류.** 이름만 틀리고 줄 번호는 옛 기준으로 Custap/Micle/JabocaRowap/BerryFocusEnergy를 가리킨다. 현재는 :7682/:7691/:7703/:7424다 |
| (참고) | `docs/friend-handoff/CLAUDE_FULL_SYNC_PORT_PROMPT.md:70` | "#10268 — A안: 열매 발동 시 upstream 아이템 팝업을 사용한다." | 이름이 없어 오류는 아니다. 다만 "열매 전체"로 읽힐 수 있다 |
| (참고) | `g2_battle_fixed_b_plan.md:157`, `HANDBACK_2026-09-29_FULL_SYNC_PORT_START.md:92`, `mgba-check-seq127-138.md:51` | "열매 발동 시 upstream 아이템 팝업" | 같은 모호함 |
| (참고) | `results/1.17.0-inventory/g2_battle_fixed_b_report.md:44`·`:130` | "Custap·Micle·Jaboca…", "Custap 7686, Micle 7695, Jaboca 7707, BerryFocusEnergy 7432" | 영문명이라 맞다. :44는 Lansat·Rowap을 빼고 적었다 |

- 그 밖에 저장소 안 "이스타"는 0건이다(`grep -rn 이스타`, `.git`·`build` 제외).
- "캄라"가 나오는 다른 곳(`full-sync-seq-108-119.md:158`)은 스피드 열매 설명이라 맞다.
- 2026-10-04에 3곳 모두 "애슈·…·랑사"로 고치고 정정 표시를 남겼다(결정 내용은 같음).

## 6. 친구에게 확인할 것 (우선순위 순)

1. 멘탈허브를 지닌 포켓몬이 **내던지기**로 멘탈허브를 던져, 도발(또는 헤롱헤롱·앙코르) 상태인 상대에게 맞혔을 때 **상대 쪽에 멘탈허브 팝업**이 뜨나요?
2. 멘탈허브(또는 하양허브) 팝업이 뜰 때 바로 앞 문장(예: "도발에 넘어가 버렸다!")이 창에 **남아 있나요**, 아니면 **창이 비워진 뒤** 팝업이 뜨나요?
3. **먹다남은음식** 턴 끝 회복 때 팝업이 뜨나요? 그리고 "먹다남은음식으로 인해 조금 회복했다" 같은 **회복 문장이 나오나요**, 아니면 팝업과 HP 바만 움직이나요?
4. **조개껍질방울**로 공격 뒤 회복할 때도 같은 질문입니다(팝업 유무, 회복 문장 유무).
5. **자뭉열매·오랭열매**로 회복할 때 팝업 뒤에 "체력을 회복했다" 문장이 나오나요?
6. **풍선**: 나올 때 "풍선 때문에 떠 있다!"와 함께 팝업이 뜨나요? 맞아서 "풍선이 터졌다!"가 나올 때도 팝업이 뜨나요?
7. **필드 시드**(일렉트릭·그래스·미스트·사이코시드)가 필드에서 발동해 방어·특방이 오를 때 팝업이 뜨나요?
8. **시몬열매**로 혼란이 나을 때, **리샘열매**로 혼란만 나을 때 팝업이 뜨나요?
9. (선택) **반감열매** 팝업과 "데미지를 약하게 했다" 문장은 공격 애니메이션 **전**에 나오나요, **후**에 나오나요?
10. (선택) 같은 포켓몬의 **특성 팝업 바로 뒤에 도구 팝업**이 이어질 때 두 팝업이 겹쳐 보이나요, 특성 팝업이 사라진 뒤 도구 팝업이 뜨나요?
11. (선택) **왕의징표석** 때문에 상대가 풀죽을 때 도구 팝업이 뜨나요?
