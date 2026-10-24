# full-sync 묶음 37 — seq 434~445 (441 일부, 444 보류)

- 요청: 사용자 "혼자 할 수 있는 거라면 다 끝내고 보낼 보고 메시지는 마지막에 한 번에"(2026-10-23 밤, 사용자 취침 중 자율 진행)의 연속. 435·436·438은 앞당김 끝.
- upstream: 읽기 전용 사본 `/home/user/pokeemerald-expansion-upstream`에서 `git format-patch`로 읽었다.

## 커밋

| seq | PR | 커밋 | 내용 |
|---|---|---|---|
| 434 | #8647 | `3e3af39f` | 회전·맹세·합체 기술 AI — 게임 코드는 9/15 선별 이식분으로 이미 있음. upstream 테스트(`ai_combo_attack.c` 새 파일, 더블·스플래시 비교·수면 규칙)와 죽은 함수 `GetAllyChosenMove` 삭제만 |
| 434.5 | #10461 | `c26873e5` | 남은 hunk 1개: `BattleAI_SetupAIData`가 무작위 대상을 `gBattlerTarget`에 쓰지 않음(`chosenTarget`만) |
| 437 | #10452 | `8aaadb00` | 동적 선택지에 포켓몬 아이콘 콜백(`DYN_MULTICHOICE_CB_SHOW_PKMN`, `CreateTaggedMonIcon`). HnS 스크립트는 안 씀. 스프라이트 슬롯이 없을 때의 HnS 가드는 `ChangeSpriteOnSelection` 안에 유지 |
| 439 | #10443 | `e81ae8cd` | 부리캐논·집중펀치·트랩셸 준비 처리 정리(아래) — 한글 문장 3개의 이름 토큰만 `B_ATK`→`B_SCR`(정책 74) |
| 440 | #10471 | `98a3b6fc` | `infiniteConfusion` 삭제 — 혼란 타이머 하나(`confusionTimer`), 버서크유전자는 `PERMANENT_VOLATILE` |
| 441 일부 | #10467 | `d7b990b4` | 와이드가드가 싱글 배틀에서도 막음(대상 종류 직접 비교). **"방어함" 애니메이션은 보류(친구 질문)**, `CancelerTargetFailure` 반복 범위는 HnS 구조가 달라 해당 없음 |
| 442 | #10523 | `dcd28c3f` | 파트너 생성기 `smartTera` 1비트에 마스크를 넣던 결함 — HnS는 `smartTera`를 읽지 않아(재확인 86) 동작 변화 0 |
| 443 | #10531 | `a4d0b7c4` | `sBattleWeatherInfo` → `gBattleWeatherInfo` 이름만 |
| 444 | #10521 | **보류** | 아래 "친구 질문" |
| 445 | #10499 | `1d086e41` | 빈 파일 `src/data/item_icon_table.h` 삭제 |

이식 전(`0b76d041`) 32,962,052 B → **32,962,948 B(+896)**, EWRAM 250,404 B·IWRAM 25,516 B 같음, SHA1 **`301e48e08141286502e3762f71ffda45fc351e02`**.

## PR별 내용과 HnS 판단

### 434 #8647 + 434.5 #10461 — AI

- AI 쪽 게임 코드(파트너 행동 예측, 회전/합체/맹세 점수, `AI_SetBattlerTurnOrder`, `struct ChosenAction`)는 9/15 선별 이식으로 이미 HnS 형태로 있다. 남은 것만: 테스트, 죽은 함수, `gBattlerTarget` 대입 삭제.
- AI 테스트 변화는 개선만: `AI values moves above Splash` 1-100·101-200, `AI uses Power Split` FAIL → PASS(이름의 파라미터 수가 바뀐 행 포함), 합체 기술 테스트 4개 새로 PASS. AI 생각 시간 상한은 HnS 값 유지.

### 437 #10452 — 동적 선택지 포켓몬 아이콘

- 깨끗이 적용, 화면 영향 0(HnS 스크립트가 새 콜백을 안 씀). 9/15에 넣은 HnS 가드(스프라이트 슬롯이 모자라면 필드 설정 안 함)는 upstream이 옮긴 함수 안으로.

### 439 #10443 — 부리캐논·집중펀치·트랩셸

- 준비 스크립트를 `GetChargingSetUpScript`로 고르고, 준비 문장·애니가 `BS_SCRIPTING`(준비하는 배틀러)을 가리킨다. **한글 문장 3개(`STRINGID_PKMNTIGHTENINGFOCUS`, `HEATUPBEAK`, `PREPARESHELLTRAP`)는 이름 토큰만 `{B_ATK_NAME_WITH_PREFIX}` → `{B_SCR_NAME_WITH_PREFIX}`**(정책 74, 문구·줄바꿈 그대로 — 턴 시작 처리에서 공격자 변수가 그 배틀러가 아니어서 생기던 이름 오류를 막는 upstream 수정).
- 동작(upstream대로, HnS 설정 `B_MOVE_EFFECTS_BEFORE_MOVES GEN_LATEST` = Champions):
  1. 잠듦·게으름·쓸 기술 없음이어도 턴 시작 준비 문장이 나온다(이전 HnS는 건너뜀).
  2. 앵콜로 이 기술에 묶이면 앵콜 문장 바로 뒤에 준비 애니·문장이 나온다(`trydomoveeffectsbeforemoves`).
  3. 부리캐논 화상은 자체 MoveEnd 단계(흡수 회복·분노 뒤, 특성 앞)에서 **접촉 기술에만**. 준비 없이 앵콜로 끌려 나가면 Gen 9 이하에서만 실패.
- `BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 3행. 새 테스트는 영어 `MESSAGE` 때문에 FAIL(알려진 한계).

### 440 #10471 — 혼란 하나의 타이머

- 버서크유전자는 혼란될 수 있을 때만 `PERMANENT_VOLATILE`. HnS 적응: 버서크유전자 스크립트는 도구 팝업 없는 HnS 형태에 `jumpifvolatile` 분기만, `ItemHealMonVolatile`의 헤롱헤롱만 문장 유지, 리샘열매 경로·턴 끝 난동 혼란 이름은 손으로. 관련 테스트 74개 결과 같음(upstream이 지운 `Berserk Gene causes infinite confusion` 1줄만 사라짐).
- 한글 회귀 세트 `zz_hns9168_k1_berry.c`의 `volatiles.confusionTurns` 3곳을 새 이름 `confusionTimer`로(필드 이름만, 기대 결과 그대로). 이 수정 없이는 한글 회귀가 컴파일되지 않는다.

### 441 #10467 일부 — 와이드가드

- `IsBattlerProtected`가 `IsSpreadMove`(싱글에서는 항상 거짓) 대신 대상 종류(`TARGET_BOTH`·`TARGET_FOES_AND_ALLY`)를 직접 본다 → **싱글 배틀에서도 와이드가드가 전체기를 막는다**(본가와 같음). 메시지 표 1행. 새 테스트는 영어 `MESSAGE`로 FAIL.

## 친구 질문

### 441 #10467 — "방어함" 애니메이션(새 질문, 재확인 90(1))

- upstream은 방어·판별·니들가드 등으로 공격을 막을 때 `"…은(는) 공격으로부터 몸을 지켰다!"` 앞에 **막은 포켓몬에게 방어 막 스프라이트 + 흔들림 + 효과음 애니(`B_ANIM_PROTECTED_ITSELF`)**를 새로 넣었다. 막힐 때마다 짧은 애니만큼 길어진다.
- A) upstream대로 넣기 / B) 넣지 않기(지금 HnS 그대로, 문장만).

### 444 #10521 — 설명 글자 폭 맞춤(새 질문, 재확인 90(2))

- upstream은 기술·특성 설명이 창보다 넓으면 더 좁은 글꼴로 바꿔 찍는다(요약 화면·배틀 기술 정보·떠올리기).
- HnS 한글 설명을 실제로 재 본 결과(세션 임시 테스트, 저장소에 넣지 않음):
  - 요약 화면 기술 설명(160 px)·콘테스트 설명·배틀 기술 정보 창(144 px, 원래 `FONT_NARROW`): **바뀌는 것 0**.
  - **기술 떠올리기 설명(128 px, `FONT_NARROW`): 74개가 129~134 px로 넘침** → `FONT_NARROWER`로 바뀌어 약 97~104 px(예: 더블니들·폭포오르기·불새·발버둥·코골기·버티기·치료방울·정신차리기·볼트태클).
  - **요약 화면 특성 설명(144 px, `FONT_NORMAL`): 18개가 145~146 px로 넘침** → `FONT_NARROW`로 약 124~128 px(예: 인분·불가사의부적·천진·플라워기프트·애널라이즈).
- 지금 넘치는 폭은 1~6 px(마지막 글자 오른쪽 여백 수준)이라 실제로 잘려 보이는지는 실기로만 알 수 있다.
- A) upstream대로 넣기(해당 설명만 좁은 글꼴) / B) 넣지 않기(지금 그대로).

## 검증

- **빌드:** 커밋마다 `make hns -j8` 종료 0, 새 경고 0(`warncheck.sh`).
- **세이브 정적 비교**(이식 전 사실 = 세션 임시 `b39/save-pre`, 코드 `0b76d041`을 클론에서 빌드 — ROM 32,962,052 B로 같음): **PASS(FAIL 0, WARN 0)**.
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 `1d086e41`): 러너 PASSED 2,778 / TOTAL 5,762(KNOWN_FAILING 25) — `test-baseline-seq431.txt` 대비 **사라진 PASS 0**(upstream이 지운 버서크유전자 테스트 1줄 제외), 새 PASS 8(AI 3·합체 4·부리캐논 Gen 9 이하 실패 1), 새 FAIL은 모두 영어 `MESSAGE` 행(부리캐논 8 — TO_DO에서 구현된 1 포함, 집중펀치 2, 와이드가드 1)과 `AI values moves above Splash, 201-300`(4/67 → 60/67 FAIL, 개선). 새 기준 [`test-baseline-seq445.txt`](test-baseline-seq445.txt)(**다음 비교 기준**).
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-b39b 4`, 코드 `1d086e41` + 위 세트 이름 수정): **558/652, `판정: PASS (기대와 같음)`**.
- **실기: 하지 않음(친구).**

## 실기(mGBA) 후보 — 친구

1. 집중펀치를 고른 포켓몬이 잠들어 있을 때 턴 시작에 `…은(는)\n집중력을 높이고 있다!`가 나오는지(이름이 맞는지).
2. 싱글 배틀에서 와이드가드 → 상대 지진·눈보라 등이 막히는지.
3. (가능하면) 혼란·버서크유전자 — 혼란이 풀리지 않고 계속되는지, 리샘열매로 풀리는지.

## 재확인·뒤 PR 메모

- 새 재확인 90(441 애니, 444 질문).
- 순서표: 434·434.5·437·439·440·442·443·445 적용, 441 일부, 444 보류.
