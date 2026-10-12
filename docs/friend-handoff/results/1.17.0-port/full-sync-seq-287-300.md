# full-sync 묶음 16 — seq 287~300

- 시작 HEAD `3875ecbb`(코드 = 트레이너 그림 후속 `a9f64486`). 작업: 클라우드 세션, 날짜 2026-10-12.
- 방식: 사전 분석(메인, 패치 파일은 세션 임시 폴더) → 결정(모두 사용자 혼자 정할 수 있는 범위, 사용자 "추천대로") → PR별 적용·빌드·관련 테스트 → 메인 검증. 리뷰 1개(R1, 읽기 전용 — 커밋 11개를 upstream diff와 대조, 아래 "검증").
- 실기(mGBA) 미확인(친구가 함).

## 커밋

| seq | PR | 커밋 | 판정·HnS 적응 | ROM(B) |
|---|---|---|---|---:|
| 287 | #10415 Fix Illusion party search range for two-opponent trainers | — | 이미 적용(`262bd0ab`, 2026-10-04 선반영) | — |
| 288 | #10410 assert trainer script that needs an object event | `9cece8f0` | 그대로. `SetTrainerFacingDirection`에 assert. HnS 맵 스크립트의 인트로 `trainerbattle` 1,650곳을 훑었고 오브젝트 없이 닿는 곳은 에메랄드 맵의 노먼 재대결 1곳(HnS 일반 진행에서 갈 수 없음)뿐 | 32,787,044 |
| 289 | #10301 Fix buffer overrun in ReformatItemDescription | `5738eedf` | 그대로(`StringCopy`·`StripLineBreaks`·`BreakStringAutomatic`). HnS는 `OW_SHOW_ITEM_DESCRIPTIONS`가 꺼져 있어 일반 진행에서 닿지 않음 | 32,787,060 |
| 290 | #10432 Fix frlg old man text | — | **skip**(사용자 결정, 번역 정책). 넣으면 HnS 한글 `STRINGID_WALLYUSEDITEM`("민진은 …을 썼다!" 계열)이 지워지고 HnS `TRAINER1USEDITEM`(`{B_TRAINER1_CLASS} {B_TRAINER1_NAME}{B_TXT_EUNNEUN}⏎{B_LAST_ITEM}{B_TXT_EULREUL} 썼다!`)으로 바뀌는데, upstream 새 문장은 `{B_ATK_TRAINER_NAME_WITH_CLASS} used {B_LAST_ITEM}!`이라 토큰이 달라 이름이 엉뚱하게 나온다. 잡기 튜토리얼은 에메랄드 등화 맵·디버그에서만 쓰여 HnS 일반 진행에 없음 | — |
| 291 | #10429 Add Zero-Width-Space and fix some line break handling | — | 이미 적용(HnS 구현 `8280caf1`, `CHAR_ZWS=0x42` — 재이식 금지) | — |
| 292 | #10298 New config for transformed pokemon properties | `e2c2f9a6` | 손 맞춤 2곳. `battle_main.c` `FaintClearSetData`에서 `transformed`와 함께 `transformedMonSpecies`를 지킴(HnS 줄 구조). `Cmd_getexp`는 `GetConfig(B_SCALED_EXP)`·`GetConfig(B_TRAINER_EXP_MULTIPLIER)`를 받고 **HnS 경험치 분배 선택 `UseClassicExpSplit()`은 `GetConfig(B_SPLIT_EXP)` 대신 그대로 유지**. 새 config `B_TRANSFORM_CATCH_RATE`·`B_TRANSFORM_BATTLE_REWARDS`(둘 다 `GEN_LATEST`). 변신한 포켓몬(메타몽 등)을 쓰러뜨리면 경험치·노력치를 원래 종 기준으로, 포획률도 원래 종 기준(본가 5세대 이후) | 32,787,252 |
| 293 | #10428 Fix outdated move data | `2331a1fa` | **데이터 4줄만.** 파찌파찌액셀 8세대부터 급소 고정 아님, 초승달의기도 PP 10 → 5, 브레이브차지 PP 10 → 15, 대격분 명중은 `GEN_LATEST`에서 100 그대로(아르세우스 85 분기만 생김). **설명 `#if` hunk 제외**(친구 로컬 한글화 파일 — 아래 "친구에게 알릴 것") | 32,787,252 |
| 294 | #10436 Fix send mon text in frlg first battle | `794688b4` | `STRINGID_INTROSENDOUT` 조건만 `IsOnPlayerSide(battler)`로(한글 문장 그대로). HnS 출력 같음 | 32,787,220 |
| 295 | #10437 cleanup: remove comment with incorrect time labels | `941c901a` | 그대로(`asm/macros/event.inc` 주석 1줄). 따로 빌드하지 않고 296 빌드에 포함 | (296과 같음) |
| 296 | #10442 fix(cleanup): remove unused variables | `209e0cf9` | 그대로(`battle_ai_items.c`·`battle_anim_water.c`·`string_util.c` 안 쓰는 변수) | 32,787,204 |
| 297 | #10441 Fix in which gens B_TRANSFORM_BATTLE_REWARDS effects apply | `1fe96564` | 그대로(292와 한 단위). `== GEN_3 || == GEN_4`일 때만 변신 뒤 종, 그 밖은 원래 종. 테스트 이름 변경 | 32,787,204 |
| 298 | #10445 Fix Box ID Var not being updated when opening Move Relearner from Summary Screen | `7e2f2a12` | HnS에 `gSpecialVar_MonBoxId` 줄이 이미 있어 박스 분기 앞 중복 `gSpecialVar_MonBoxPos` 줄 1개만 지움. HnS는 요약 화면 기술 떠올리기가 꺼져 있음(`P_SUMMARY_SCREEN_MOVE_RELEARNER FALSE`). 커밋 제목 끝 "Screen"이 빠짐(이미 다음 커밋을 쌓아 고치지 않음) | 32,787,204 |
| 299 | #10448 Fix Struggle recoil damage not using standard rounding | `6c7607d4` | 그대로 + 새 테스트. 최대 HP를 4로 나눈 나머지가 2 이상이면 반동 +1(5세대 이후). 커밋 제목은 줄임("Fix Struggle recoil rounding") | 32,787,220 |
| 300 | #10397 Fix ANIM_RAPID_H_HOPS freezing the game when opponent is wild shiny | `0597bd5d` | 손 맞춤 1곳. HnS `Anim_RapidHorizontalHops`에는 `361f1e4a`(한글 패치 수정)로 `x2 = 0`이 이미 있어 `y2 = 0`만 더함. 테스트 훅(`forceMoveAnim`)과 새 `test/battle/front_anim.c`는 그대로 | 32,787,220 |

- 적용 11(커밋 11), 이미 적용 2, skip 1. ROM 32,786,900 → **32,787,220 B(+320)**, EWRAM 250,408 B·IWRAM 25,516 B 그대로. 커밋마다 `git diff --check` 통과, `make hns -j8` 종료 0, 새 경고 0(`warncheck.sh`, 클라우드 libpng `iCCP` 줄 제외).

## 동작 변화

새 배틀 문장 0, 기존 문장 변경 0. 배틀 메시지 출력(어떤 문장이 나오는지)이 바뀌는 PR이 없어 `BATTLE_MESSAGE_OUTPUT_CHANGES.md`는 고치지 않았다. 문장 밖 변화:

- 292·297: 변신한 포켓몬을 쓰러뜨렸을 때 경험치·노력치, 변신한 포켓몬 포획률 = 원래 종 기준.
- 293: 파찌파찌액셀 급소 고정 없어짐, 초승달의기도 PP 5, 브레이브차지 PP 15(세 기술 모두 HnS 일반 진행 트레이너·야생 데이터에서는 확인 안 함 — 9세대 기술).
- 299: 발버둥 반동 1 차이(최대 HP % 4 ≥ 2).
- 300: 이로치 야생 포켓몬이 `ANIM_RAPID_H_HOPS` 앞모습 애니메이션을 쓸 때 멈추던 결함 수정(테스트 `Front anims work`가 애니메이션 종류마다 종 하나씩 이로치로 실행).

포트로 새로 생긴 배틀 메시지 밖 텍스트: **없음**(TRANSLATION_POLICY 5절 5번 목록 0개).

## 친구에게 알릴 것

1. **재확인 62 재현됨(방벽 타이머).** 임시 한글 테스트(저장소에 넣지 않음, 실행 뒤 지움)에서 상대가 방벽을 쓴 다음 턴에 깨뜨리다류로 깨고 4턴을 더 진행하면, 깬 방벽의 끝 문장이 한 번 더 나왔다 — 리플렉터 + 깨뜨리다 `상대의 리플렉터가 없어졌다!`, 빛의장막 + 깨뜨리다 `상대의 빛의장막이 없어졌다!`, 리플렉터 + 사이코팽 `상대의 리플렉터가 없어졌다!` 3경우 모두(seq 300 코드에서 다시 확인). 대조(깨지 않고 스스로 끝나는 리플렉터·빛의장막 문장) PASS. 원인: HnS `MOVE_EFFECT_BREAK_SCREEN`(`src/battle_script_commands.c`)이 상태 비트만 지우고 `reflectTimer`·`lightscreenTimer`·`auroraVeilTimer`를 남김. upstream #10829(seq 551)는 이 세 타이머를 0으로 하는 3줄(+ HnS에 이미 있는 `MULTISTRING_CHOOSER = 0`) + 테스트 `break_screens.c`. 친구 지시대로 순서는 바꾸지 않았고, 지금 앞당길지 seq 551에서 고칠지 묻는다.
2. **#10432 skip** — 위 표 290.
3. **파찌파찌액셀 설명** — `moves_info.h` 설명은 지금 영문 "Electric bursts always go⏎first and land a critical hit."이고, 이제 HnS 설정(`GEN_LATEST`)에서는 급소 고정이 아니라 설명과 동작이 어긋난다. upstream은 8세대 이상에서 "Electric bursts always go⏎first and raise evasion."으로 나누는 `#if`를 넣었지만 친구 로컬 번역 파일이라 넣지 않았다. 친구가 이 기술 설명을 번역할 때 8세대 이후 동작(회피율 상승, 급소 고정 없음)에 맞추면 된다.

## 검증

- **빌드:** 위 표. 최종(`0597bd5d`) ROM 32,787,220 B, SHA1 앞 12자리 `fcc6394b2df4`.
- **관련 테스트(파일 단위):** `exp.c`·`capture.c`(292·297 새 테스트 PASS, HnS 포획 테스트는 원래처럼 INVALID), `move_effect/struggle.c` 5줄(새 `Struggle recoil is subject to standard rounding (Gen 5+)` PASS, 나머지 이식 전과 같음), `front_anim.c` `Front anims work` PASS.
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j4`, 코드 `0597bd5d`): PASS 2,650 → **2,653** / TOTAL 5,572 → 5,576. `testlist.sh` 기준 `test-baseline-seq285.txt` 대비 **사라진 PASS 0**, 목록 차이는 새 줄 3개뿐 — `Front anims work: PASS`(300), `Struggle recoil is subject to standard rounding (Gen 5+): PASS`(299), `Transformed Pokemon gives the experience points of the copied species in Gen 3 and 4: PASS`(292·297). 나머지 1개는 292의 `Capture: Tranformed Pokemon get the catch rate of the copied species in gen 3/4`(HnS 포획 테스트가 모두 그렇듯 INVALID — 목록에 안 들어감). 목록 [`test-baseline-seq300.txt`](test-baseline-seq300.txt)(**다음 비교 기준**).
- **한글 회귀 607개**(`ALLOW_REPO=1 dev_scripts/hns_verify/kortests/run.sh <저장소> kor-seq300 8`): 517/607, 기대 요약과 바이트 같음.
- **리뷰 R1**(읽기 전용, 커밋 11개 대 upstream diff): 수정 필요 0. 의도한 적응(위 표) 밖 차이 없음. 확인한 것: 292·297 뒤 변신 보상·포획률 판정이 upstream 최종과 같음(3·4세대만 변신 뒤 종), `FaintClearSetData`가 `transformed`·`transformedMonSpecies`를 지키는 순서가 upstream과 같음, 292 포획 함수의 HnS 사파리 줄(`IS_HNS || ITEM_SAFARI_BALL`) 보존, 293 브레이브차지의 특공·특방 +1은 HnS에 이미 있음, 새 테스트 파일 4개가 upstream과 바이트 같음, 게임 한글 문자열·토큰 변경 0.
- **세이브 정적 비교**(`save_compat.py run --pre pre-287`, 이식 전 사실은 같은 세션에서 `3875ecbb` 빌드로 만듦): **PASS(FAIL 0, WARN 0)**. 세이브 경로 코드 엄격 54·검토 18 같음, `recorded_battle.o` 37 같음, EWRAM·IWRAM 변수 변화 0, SaveBlock1·2·3·`gPokemonStorage` 크기 같음. 세이브 왕복은 돌리지 않음(세이브 코드·구조 변화 없음).

## 재확인·뒤 PR 메모

- 62: 재현됨(위 1번, 3경우). 친구 답 뒤 앞당기거나 seq 551에서. 앞당기면 HnS `MOVE_EFFECT_BREAK_SCREEN`에 타이머 3줄 + upstream `test/battle/move_effect_secondary/break_screens.c`(영문 `MESSAGE` — HnS에서는 FAIL 예상, 한글 확인은 임시 테스트로).
- 292: HnS `UseClassicExpSplit()` 유지 — 뒤 PR이 `Cmd_getexp`의 `B_SPLIT_EXP` 줄을 문맥으로 쓰면 HnS 줄로 맞춘다.
- 293: 파찌파찌액셀 설명 `#if`는 넣지 않았다 — 뒤 PR(#10822 seq 541 등)이 이 설명 줄을 문맥으로 쓰면 HnS 설명 그대로 두고 구조만 맞춘다(재확인 61과 같은 방식).
- 300: `pokemon_animation.c`의 `x2`·`y2` 순서는 HnS(`callback` 뒤)대로.
