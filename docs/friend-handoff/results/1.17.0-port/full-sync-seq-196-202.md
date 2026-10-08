# full-sync 묶음 9 — seq 196·197·200·202 (+ 같은 날 HnS 수정)

- 시작 HEAD `2beb2bd19f`(묶음 8 뒤, 코드 = `a1f5e58f85`). 작업 컴퓨터: 데스크탑(사용자는 랩탑에서 Remote Control). 날짜 2026-10-08.
- 방식: PR별 사전 분석 2개(병렬, 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-196-202/seq*.md`) → 사용자 결정 6개("ㄱㄱ") → 적용 1개(커밋 4개) → 리뷰 2개 → HnS 수정(리뷰 결과·친구 요청) → 메인 검증. 검증은 저장소 도구 `dev_scripts/hns_verify/`.
- 실기(mGBA) 미확인.

## 커밋

| seq | PR | 커밋 | 판정·HnS 적응 | ROM(B) |
|---|---|---|---|---:|
| 196 | #9988 Dancer activation order config + upcoming bugfixes | `11988b1d6a` | 18파일. 새 config `B_DANCER_ORDER`는 **`GEN_7`**(사용자 결정 1 — upstream `GEN_LATEST`는 빠른 순이고 HnS에서는 교체 때 정렬한 값이라 스피드 변화·트릭룸을 못 따라감). 손 맞춤: `MoveEndFaintBlock(void)` 형태, `TurnValuesCleanUp` 맹세 가드 문맥, `TryRedCard` 2인자, 탈출버튼·탈출팩·`TrySwitchInEjectPack` 프리폴 조건을 1.17.0 형태 `IsBattlerInvolvedInSkyDrop()`로(**재확인 8g 해결**), sky_drop 테스트 위치. #10180의 노가드 × 프리폴 hunk는 계속 제외(사용자 결정 4 → 재확인 34). 3b·X4 HnS 춤추기 코드 6곳은 hunk에 들지 않음 | 32,752,788 |
| 197 | #9861 Fix Dynamic Summary Screen Type | `19b2a21b38` | HnS `GetDynamicMoveType`는 아직 `type1/2/3`·`ability`/`holdEffect` 매개변수 형태(#9898·#10277은 뒤 행) → `battle_main.c` 5곳 손 맞춤, 요약 화면 hunk 그대로. `P_SHOW_DYNAMIC_TYPES` FALSE라 요약 화면 출력 같음(함수 88개 역어셈블 같음). 대지의파동 AI hunk 포함(사용자 결정 5) | 32,752,756 |
| 200 | #10024 Fixes Trace not activating when reobtained after activation | `1e1ed852f4` | 그대로(`RemoveAbilityFlags` 1줄 + 테스트). 196만 넣으면 스킬스왑 등으로 트레이스를 다시 얻어도 발동하지 않던 회귀를 해소(이식 전 출력과 같음) | 32,752,772 |
| 202 | #10050 Set daily seed on new game | `7e8d48f64f` | 재확인 31대로 `gSaveBlock3Ptr->dailySeed`. 호출은 `ResetLotteryCorner()` 다음(`ClearSav3`·챌린지 복원·트레이너 ID 뒤). 세이브 왕복 NEWGAME 비교 기준 분리(사용자 결정 6): `dev_scripts/hns_verify/save/savetest/baseline/expect-newgame-flash.bin`(SHA1 `8d5a0726…`) + `savetest.py`·README 5절 | 32,752,788 |
| — | (HnS, 친구 요청) 대화창·창틀·배틀 메시지창 그래픽 | `3bc975f558` | 아래 "UI" 절 | 32,753,252 |
| — | (HnS, 리뷰 R2) 세이브 왕복 도구 주석 | `4eda6b4197` | NEWGAME 기준 파일 설명 3곳, `newgame.sav` 한쪽 없음 `[INFO]` | — |
| — | (HnS, 친구 요청) AI 날씨부정·에어록 추측 | `04f558cf9f` | 아래 "AI 날씨" 절 | 32,753,684 |
| — | (HnS, 리뷰 R1) 춤추기 Gen7 순서 | `f695e1b637` | 아래 "춤추기 순서" 절 | |
| — | (HnS, 친구 요청) 목호·실버 파트너 트레이너 슬라이드 | `437e1bd9d6` | 아래 "그 밖의 친구 요청" 절 | |
| — | (HnS, 친구 요청) 포켓기어 헤더 | `f9df186f69` | 아래 "그 밖의 친구 요청" 절 | 32,753,828(앞 3개 합쳐) |

## 사용자 결정(2026-10-08 아침 "ㄱㄱ")

1. `B_DANCER_ORDER` = `GEN_7`(이식 전 HnS 순서 유지).
2. #9988 upstream 버그 수정 받아들임: 원한 문장이 기절 앞, 풍선 빼앗기(나쁜손·도둑질·탐내다·매지션), 트레이스 + 특성보호대, 페인트 뒤 방어 확정 성공, 프리폴 중 탈출 아이템 미발동(출력 변화 문서 5행).
3. 방어류 첫 사용에 난수를 쓰지 않음: 확률은 같은 난수에서 비트 단위로 같고 그 배틀의 뒤 난수 위치만 당겨짐. AI는 페인트로 방어가 깨진 다음 턴 다시 방어하려는 경향이 커짐.
4. #10180 노가드 × 프리폴 hunk 계속 제외(동작이 반대로 바뀌는 별도 변화 → 재확인 34).
5. #9861 대지의파동 AI hunk 포함(AI가 아는 특성·도구로 접지 판단, 1.17.0 최종과 같음, HnS 데이터에 사용 트레이너·기술머신 없음).
6. #10050 뒤 세이브 왕복 NEWGAME 비교 기준을 입력 이미지와 분리(값 변화 — 견습생 번호 2→1, `dailySeed` 0→`0x45B982B2`, 릴리코브 퀴즈, 섹터 15·19. 형식 변화 없음, LOAD 입력은 그대로).

## 동작 변화

- **#9988**(새 한글 문장 없음, 순서·발생만): 위 결정 2의 5가지(`BATTLE_MESSAGE_OUTPUT_CHANGES.md`). 방어 난수(결정 3). 트레이스 + 특성보호대는 이제 스크립트 없이 발동 처리되어 AI가 트레이스를 알게 되고 더블에서 `RNG_TRACE`를 1회 쓴다(upstream과 같음).
- **#10050:** 새 게임 난수열이 한 칸 밀린다(트레이너 ID·랜더마이저 씨앗·복권 번호는 그 앞이라 같음). 실게임에서는 난수가 매 프레임 진행되어 관측되지 않는다.
- **#9861·#10024:** 화면·배틀 출력 변화 없음(#10024는 #9988의 트레이스 회귀를 되돌림).

## 춤추기 순서 (`f695e1b637`, 재확인 3b·X4·8t)

- 친구 결정 3b(`ee2da89a61`, 탈출버튼·탈출팩)·X4(`1e9f3254f9`, 위기회피·도망태세)의 "누가·언제 춤추나"는 #9988 뒤에도 같다(분석 A·리뷰 R1: 3b·X4 코드 6곳이 hunk에 들지 않음, 관련 테스트 29파일 이식 전후 상태 같음, 한글 회귀 607개 이벤트 기록 같음).
- 리뷰 R1이 `GEN_7`에서도 순서가 이식 전과 다른 경우를 찾았다: upstream GEN_7 가지는 교체 때만 정렬하는 `gBattlersByRawSpeed`를 써서 스피드스와프·사이드체인지·배틀 중 레벨업·폼체인지 뒤(교체 없이)와 일부 동속에서 순서가 바뀐다. 사용자 결정(이식 전 HnS 동작 유지)으로 GEN_7 가지만 이식 전 선택(현재 원시 스피드가 느린 쪽, 동속이면 번호가 작은 쪽)으로 되돌렸다. GEN_8+ 가지는 upstream 그대로.
- 테스트 `test/battle/hns_dancer.c` 3개 추가(스피드스와프, 사이드체인지, 위기회피로 들어온 춤추기와 동속) — 모두 PASS. `dancer.c`·나머지 `hns_dancer.c` 결과는 같다.

## AI 날씨 (`04f558cf9f`, 친구 요청 2026-10-08)

- 친구 보고가 맞다. #9865(seq 182) 전에는 AI가 턴 시작 때 실제 `HasWeatherEffect()`를 저장해 썼다(주석 "weather damping abilities are announced"). 뒤에는 `AI_WeatherHasEffect`가 `gAiLogicData->abilities[]`를 보는데, 특성이 기록되지 않은 플레이어 포켓몬은 `AI_DecideKnownAbilityForTurn`의 무작위 추측이라 고라파덕·골덕·내루미·내룸벨트·할비롱(1/3), 파비코·파비코리(1/2)를 날씨부정으로 보고 날씨를 없는 것으로 계산했다. 날씨부정·에어록은 배틀 시작·교체 등장·특성 변경 때 항상 팝업과 기록(`recordability`)이 있고, 변신·괴짜는 변신 처리에서 기록된다 → 팝업이 없으면 아닌 게 맞다. upstream 1.17.0·master·upcoming은 #9865 형태.
- 수정(친구가 말한 좁은 형태): AI가 모르는 상대의 날씨부정·에어록은 AI가 그 특성을 아는 경우(자기 편·전지 플래그·기록·특성 덮어쓰기)에만 날씨 무효로 본다. 추측 자체와 난수 소비는 그대로. 넓은 안(추측 후보에서 빼기)은 다른 판단 138판이 바뀌어 비추천.
- 실측(HnS 트레이너 650명 + 프런티어, 플레이어 선두 6종 × 5턴, 11,405판): 수정 전 #9865 전과 399판(3.5%) 다름(류옹·규리·이슬·마티스·민화·강연·실버·비주기가 비바라기·쾌청·모래바람을 안 쓰거나 같은 날씨를 또 쓰거나, 비에서 불꽃 기술을 고름) → 수정 후 0판(AI 점수·행동 로그 152,751줄 바이트 같음). 새 테스트 `test/battle/ai/hns_weather_negation.c` 5개(수정 전 2/5 → 5/5). 남는 차이: 변신으로 날씨부정을 복사한 포켓몬이 교체로 변신이 풀린 뒤 다시 나오면 파티 기록 때문에 날씨 없음으로 봄(수정 전과 같음, HnS 트레이너 날씨부정 1마리뿐). 상세 저장소 밖 `hnsfix-1008-weather/WEATHER-AI.md`.

## UI (`3bc975f558`, 친구 요청 2026-10-08)

- 친구 PNG 7개를 그대로 사용(재디자인·팔레트 재배열 없음): `graphics/text_window/hns/message_box.png`(새 14타일), `graphics/text_window/{1,2,4,6,8}.png`(공용 파일이라 다른 빌드도 바뀜), `graphics/battle_interface/hns/textbox.png`. 생성물(`.4bpp`·`.gbapal`·`textbox.4bpp.smol`·`textbox_map.bin.smolTM`)은 지우고 새 PNG에서 다시 만들었다(따로 변환한 결과와 바이트 같음, ROM 안에 들어감 확인).
- `src/menu.c`(HnS만, `#if IS_HNS`): 표준 대화창 폭 27→26, `WindowFunc_DrawDialogueFrame`·`WindowFunc_RedrawDialogueFrame` 새 14타일 배치(테두리 `left - 2 .. left + width + 1`), `WindowFunc_ClearDialogWindowAndFrame`·`…NullPalette` 지우는 영역 `left - 2`, `width + 4`. 간판 테두리·일반 창 경로는 그대로.
- `textbox_map.bin`: (27,14) 타일 5→27, (27,19) 15→33만(플립·팔레트 비트 유지).
- **추가(사용자 결정 B):** 대화 테두리를 쓰는 다른 HnS 창 10개도 모두 `left 2, width 27`이라 새 테두리 오른쪽 끝이 보이지 않는 30열에 그려져 잘림 → HnS에서만 폭 26: 새 게임 인트로(`oak_speech_hns.c`, HnS 전용 파일), 가방, 상점, 엄마 저금, 슬롯머신, 포켓블럭, 피라미드 가방, 트레이너 카드 링크, 시계 리셋, 나무열매 블렌더.
- 글 폭이 216→208px: 필드 대사 중 209~216px 줄 12개(HnS 맵 10, 아직 영어)가 마지막 글자 하나를 잃는다(예: 블랙손 체육관 `…GYM BADGE!`의 `!`). 아래 표 12줄 + 이미 216px를 넘던 1줄(마지막 행). 계산: 한글 8px, 라틴 `gFontNormalLatinGlyphWidths`, 창 밖 글리프는 `CopyGlyphToVRAM`이 자름(재측정 스크립트 저장소 밖 `hgss-ui-1008/textwidth/run_all.sh`).
  | 파일:줄 | 라벨 | 폭(px) | 줄 |
  |---|---|---:|---|
  | `data/maps/BlackthornCity_Gym_hns/scripts.inc:411` | `BlackthornGym_Text_GuideGotoDragonsDen` | 212 | DRAGON’S DEN if you want that GYM BADGE!$ |
  | `data/maps/GoldenrodCity_GameCorner_hns/scripts.inc:863` | `GoldenrodCity_GameCorner_Text_VF_HowToPlay` | 210 | But if you flip a VOLTORB, it’s game over.\n |
  | `data/maps/MelemeleIsle_hns/scripts.inc:727` | `Alola_Melemele_SamsonOak_Trigger_Beginning_Text_4` | 211 | This whole island reminds me of MELEMELE\n |
  | `data/maps/Melemele_PlayerHouse_hns/scripts.inc:575` | `Alola_Melemele_SamsonOakVilla_Dialogue_Text_1` | 210 | The ALOLA ISLES must truly be special if\n |
  | `data/maps/Melemele_PlayerHouse_hns/scripts.inc:613` | `Alola_Melemele_SamsonOakVilla_Dialogue_Text_6` | 210 | If you see them fly over the ISLES, head\n |
  | `data/maps/NewSinjoh_hns/scripts.inc:99` | `NewSinjoh_MilkMan_GotMilk` | 212 | It’s fortified with what the world wants.$ |
  | `data/maps/Route15_hns/scripts.inc:172` | `Route15_Text_BillyRegister` | 211 | I should’ve paid more attention in class.\n |
  | `data/maps/Route4_hns/scripts.inc:45` | `Route4_Text_BirdKeeperHank_AfterBattle` | 209 | If you have a specific POKéMON that you\n |
  | `data/maps/SinjohRuins_House1_hns/scripts.inc:657` | `SinjohRuins_Lab_Text_Note4` | 209 | Our ancestors. Before they moved north.\p |
  | `data/maps/UlaulaIsle_hns/scripts.inc:353` | `Alola_Ulaula_TrainerBattleYoungster_Text_2` | 210 | We’ve always just called it MT. LANIKALA.$ |
  | `data/scripts/debug.inc:447` | `Debug_Enable_To_Use_Follower_NPCs` | 213 | TRUE in ’include/config/follower{F9 09}npc.h’.$ |
  | `data/text/move_relearner.inc:21` | `MoveRelearner_Text_AnythingElse` | 210 | Is there anything else I may do for you?$ |
  | `data/maps/Melemele_PlayerHouse_hns/scripts.inc:767` | `Alola_Melemele_SamsonOakVilla_Dialogue_Text_30` | 219 | {COLOR 4}VULPIX{COLOR 2}, {COLOR 4}NINETALES{COLOR 2}, {COLOR 4}SANDSHREW{COLOR 2}, {COLOR 4}SANDSLASH{COLOR 2},\n | 추가 창 10개는 0줄. 지금 한글 필드 대사 최대 폭은 123px. 이름 최대 7자 가정에서만 넘는 줄 21개(5자 이하면 모두 들어감).
- 정적 미리보기(실기 아님): 새 대화창은 오른쪽 회색 띠가 왼쪽보다 넓다(타일 8이 회색, 흰 타일 7은 쓰지 않음). 배틀 메시지창도 같은 모양.

## 그 밖의 친구 요청 (2026-10-08)

- `437e1bd9d6`: 목호·실버 멀티배틀 트레이너 슬라이드에서 파트너 뒷모습 손이 화면 왼쪽에 먼저 보이던 것 → `PlayerPartnerHandleTrainerSlide`에서 `PARTNER_LANCE_HNS`·`PARTNER_SILVER_*_HNS` 4개만 시작 `x2` −96 → −112(속도 2의 배수). 다른 파트너·플레이어 슬라이드 그대로.
- `f9df186f69`: `graphics/pokenav/hns/header.png` 교체(같은 424×8·팔레트, 타일 2~19·21·22·41 변경). `header.4bpp`·`.smol`·`.gbapal` 재생성, `header.bin` 그대로. 빌드 산출물로 조립한 큰 `포켓기어` 정상.

## 검증

- **빌드:** 커밋마다 종료 코드 0, 새 경고 0. 최종(`f9df186f69`) ROM **32,753,828 B**(묶음 전 +1,072: 묶음 9 +32, UI +464, AI 날씨 +432, 나머지 +144), EWRAM 250,408 B, IWRAM 25,516 B(변화 0), SHA1 `1d961bba5c6aad3780d20627de75627aa44aa2d2`.
- **전체 테스트(최종):** PASS 2,547 → **2,561** / TOTAL 5,457 → 5,475. 사라진 PASS 2 = 이름만 바뀐 `Dancer triggers from slowest to fastest`·`… during Trick Room`(새 이름 `(Gen 7)`로 PASS). 새 PASS 16 = #9988·#10024 8(춤추기 Gen 8+ 1, `(Gen 7)` 이름 변경 2, 방어 연속 실패 2·3·4번째 3, 페인트 뒤 방어 1, 프리폴 탈출 아이템 1) + AI 날씨 5 + 춤추기 Gen7 HnS 3. 새 FAIL 3(알려진 한계: 나쁜손 풍선·트레이스 2 — 영문 `MESSAGE`, 탁쳐서떨구기가 랄토스를 한 번에 쓰러뜨려 `2 TURNs specified, but 1 ran`), 이름 변경 FAIL 1(`Dancer still activates after Red Card…`, 원래 FAIL). 목록 [`test-baseline-seq202.txt`](test-baseline-seq202.txt)(**다음 비교 기준**). 묶음 9 적용 뒤 목록은 분석 예측과 바이트 같았다.
- **한글 회귀 607개:** 이식 전·적용 뒤·최종 모두 기대와 바이트 같음(517/607). 분석 때 이벤트 기록 607개 이식 전후 같음.
- **세이브:** 정적 비교(이식 전 사실 = HEAD `2beb2bd19f` 빌드) **PASS(FAIL 0, WARN 0)**, SaveBlock1/2/3·PokemonStorage 크기 같음. 세이브 왕복 **PASS**(두 이미지, `newgame.sav`가 새 기준과 같음, `load.txt` 95줄 같음). 이식 전 트리에서 새 도구를 돌리면 섹터 [15, 19] FAIL로 변화를 잡아냄(리뷰 R2).
- **리뷰 2개:**
  - R1(#9988·#10024): upstream과 다른 곳은 계획한 손 맞춤뿐, 3b·X4 동작 같음, 원한·길동무 재배열 상태 흐름 안전, 방어 확률 비트 단위 같음, 크래시·assert 없음. 발견: GEN_7 순서 예외(경미) → `f695e1b637`. 정보: 풍선 예외 삭제가 도둑질·탐내다·매지션에도 적용(결정 2 범위, 출력 변화 문서), 트레이스 + 특성보호대 AI·`RNG_TRACE`, `traceActivated` 해제 경로(떠도는영혼 대상 쪽·배틀 중 폼체인지는 지우지 않음 — upstream과 같음).
  - R2(#9861·#10050): 손 맞춤 5곳 의미 같음, AI 경로 8곳에 AI가 아는 값, 요약 화면 역어셈블 같음, `gSaveBlock3Ptr`·호출 위치·초기화 순서 맞음, 기준 파일 추적 규칙 맞음. 발견: 도구 주석 3곳(경미) → `4eda6b4197`.

## 재확인·뒤 PR 메모

- 8g 해결(seq 196). 8t 확인(3b·X4 호환). 31: #10050 적용, #10012(359)·#9955(368) 남음. 새 33(묶음 9·HnS 수정 위치), 34(#10180 노가드 × 프리폴 hunk).
- `B_DANCER_ORDER GEN_7` + HnS 선택: `TryDancer`를 바꾸는 뒤 PR은 3-way로.
- 세이브 왕복 `expect-newgame-flash.bin`: `NewGameInitData` 값을 바꾸는 행마다 다시 만든다(도구 README 5절).
- 대화창 폭 26: 한글 필드 대사를 옮길 때 208px 안인지 저장소 밖 `hgss-ui-1008/textwidth/run_all.sh`로 다시 잰다.

## 실기(mGBA) 확인 항목

- 필드 대화창(새 테두리, 이름 상자가 사라질 때 윗줄 다시 그리기), 예/아니오·가방·상점·PC·새 게임 인트로 메시지창 오른쪽 테두리, 창틀 1·2·4·6·8번(옵션), 배틀 메시지창 오른쪽 모서리, 포켓기어 헤더.
- 목호·실버 멀티배틀 인트로의 파트너 트레이너 슬라이드(손이 먼저 보이지 않는지).
- 원한으로 쓰러질 때 문장 순서, 페인트 뒤 방어.
- 고라파덕 등(특성 미공개)을 상대로 비 트레이너(예: 관장)가 비바라기·물 기술을 쓰는지.
