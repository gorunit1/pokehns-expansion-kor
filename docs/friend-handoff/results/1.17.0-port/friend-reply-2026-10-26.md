# 친구 답 2026-10-26 반영 — 재확인 92~95·36 결정, 인주시티 무용극장 연출

- 원문: [`FRIEND_REPLY_2026-10-26.md`](../../FRIEND_REPLY_2026-10-26.md). 재확인 99(Champions)는 [`full-sync-seq-475.5-475.8.md`](full-sync-seq-475.5-475.8.md).
- **코드 수정과 정적 확인까지(친구 지시).** 빌드·자동 테스트·세이브 정적 비교 도구·음악 청취·mGBA는 **실행하지 않았다(미실행).** 새 ROM·SHA1 없음. 수정 기준 `ad68cdcc`.

## 커밋

| 항목 | 커밋 | 파일 |
|---|---|---|
| 재확인 92 | `d0995087` | `src/pokemon_summary_screen.c` |
| 무용극장 | `3ab91838` | `asm/macros/event.inc`, `data/maps/EcruteakCity_Theater_hns/scripts.inc` |
| 재확인 93·94·95 | 코드 변경 없음(현재 구현 유지) | — |

## 재확인 92 — 실제 남은 폭(A)

- 계산 위치: `PrintDescriptionOnWindowToFit`(요약 화면 설명 전용)에서 `WindowWidthPx(windowId) - x`(x가 창 폭보다 크면 0). 기술·콘테스트 설명은 창 `PSS_DATA_WINDOW_MOVE_DESCRIPTION`(20타일 = 160 px)의 x = 6이라 **154 px**. 특성 설명은 x = 0이라 창 폭 그대로.
- 다른 창: 떠올리기 설명(`menu_specialized.c`)·배틀 기술 정보(`battle_controller_player.c`)는 x = 0 경로라 손대지 않음(이 함수 밖). `GetDescriptionFontIdToFit`·`WrapDescriptionFontIdToFit` 안전 처리(기본 글꼴로 들어가면 그대로, 넘칠 때만 실제로 좁아지는 후보, 끝까지 넘치면 넘친 채로) 그대로. 문장·줄바꿈·창 크기 그대로, 설명 새 번역 없음.
- 화면 변화(결정에 따른 것): 영문 콘테스트 설명 20개(FONT_NORMAL 156 px — 칼바람·회전부리·솔라빔·로켓박치기·불새·화염바퀴·에어로블라스트·강철날개·크로스촙·정신차리기·깜짝베기·드래곤다이브·눈사태·스톤에지·원수갚기·얼음숨결·드릴라이너·프리즈볼트·콜드플레어·부리캐논이 쓰는 설명)가 FONT_NARROW로. 측정은 2026-10-25 [`bgm-font-2026-10-25.md`](../bgm-font-2026-10-25.md)의 값(이번엔 다시 재지 않음). 한글 설명은 후보 글꼴이 더 좁게 재질 때만 바뀜(한글 글리프는 FONT_NARROWER에서 더 넓음).

## 재확인 93·94 — 구슬 문장(A, 현재 유지)

| 도구 | HnS 문장(현재, 변경 없음) | 팝업 | SV 원문(Poké Corpus `57e5078` `ScarletViolet/ko_common.txt`) |
|---|---|---|---|
| 주눅구슬 | `{B_SCR_NAME_WITH_PREFIX2}{B_TXT_EUNNEUN} {B_LAST_ITEM}{B_TXT_EU}로\n{B_BUFF1}{B_TXT_IGA} {B_BUFF2}올라갔다!` → `…은(는) 주눅구슬로⏎스피드가 올라갔다!` | 도구 팝업 유지(`setlastuseditem` + `BattleScript_ItemPopUp_Scripting`) | 7495행 `[이름][은/는] [도구][로]\n스피드가 올라갔다!` |
| 맹독구슬 | `{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n맹독구슬 때문에 맹독에 중독됐다!` | 유지 | 6535행 `[이름][은/는]\n[도구] 때문에 맹독에 중독됐다!` |
| 화염구슬 | `{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n화염구슬 때문에 화상을 입었다!` | 유지 | 6559행 `[이름][은/는]\n[도구] 때문에 화상을 입었다!` |

- 단어·조사·줄바꿈이 SV 원문과 같음을 대조(코드 변경 없음). 근거는 SV 공식 한국어 원문 일치 — 팝업은 #9777 이식에서 온 것으로, 전체 연출이 SV와 같다는 뜻은 아님.

## 재확인 95 — 배틀아레나(A, 현재 유지)

- 계산 위치: `MoveEndClearBits`(`src/battle_move_resolution.c`)에서 `BATTLE_TYPE_ARENA`면 `BattleArena_AddSkillPoints(cv->battlerAtk)` — #10311 `end`·`end2` 통합 구조 그대로.
- `BattleArena_AddSkillPoints`(`src/battle_arena.c`): 대상 루프에서 **공격자 자신의 칸 건너뜀**, 방어 검사는 **`gProtectStructs[battlerAtk]`(공격자 기준)** — 두 HnS 보정 모두 있음(코드 확인, 중복 추가 안 함).
- 기록: "HnS가 기존 점수를 유지하기로 결정한 보정" — upstream이 +1을 의도했는지·버그로 인정했는지는 미확인.
- 추후 검증 범위(이번엔 정적 확인만): 일반 공격·효과 굉장/별로·실패(무효)·방어·자기에게 쓰는 기술 각각의 기술당 가감과 계산 횟수, 3턴 뒤 판정 결과 비교.

## 재확인 36 — 팝업 대기 B(승인 확인)

- 친구: B안은 이미 승인한 결정. 저장소 기록에는 승인 문장이 없어(질문 대기로 남아 있었음) 이번 메시지를 결정 기록으로 남김.
- **실제 반영 상태: 아직 반영 안 됨.** seq 394 #10268(도구·특성 팝업 끝까지 대기, `waitabilitypopup`)은 보류 상태 그대로이고, seq 483 #10431의 `BattleScript_BerryReduceAnimation`도 `waitabilitypopup` 없이(`@ HnS:` 주석) 들어가 있다. 다음 full-sync 작업에서 394를 B안으로 적용하면서 그 줄도 넣는다.

## 인주시티 무용극장 — HGSS 조우곡·연출 순서

### 원작과 HnS 명령

- HGSS(`pret/pokeheartgold` `scr_seq_0928_T27R0501.s`): 다섯 배틀마다 `StopBGM 30` → `PlayBGM SEQ_GS_EYE_MAIKO` → 그 무용수 이동(회전·접근) → 대사 → `TrainerBattle`. 옥희는 첫 설명(00011/00012) 또는 재도전 질문(00013)의 예/아니오에서 "예"일 때만 이 흐름(`_0944`), "아니오"는 음악 없이 거절.
- `StopBGM 30`의 실제 동작: `src/scrcmd_sound.c` `ScrCmd_StopBGM`이 인자를 읽기만 하고 `StopBGM(bgm, 0)` — **페이드 없이 즉시 정지, 기다리지 않음**. 바로 다음 `PlayBGM`이 새 곡 시작.
- HnS: `playbgm MUS_HG_ENCOUNTER_KIMONO_GIRL, FALSE` 한 줄 — `PlayNewMapMusic` → 다음 프레임 `MapMusicMain`이 `PlayBGM`(즉시 교체). `fadeoutbgm n`은 m4a 페이드(4n 프레임마다 1/16씩, 약 64n 프레임)를 **끝까지 기다리므로**(`IsBGMPausedOrStopped`) 넣으면 원작보다 늦어져 넣지 않음. 페이드가 없으니 "페이드 중 새 곡이 무시되는" 경우도 없음. `save_song = FALSE`라 저장된 맵 BGM은 그대로.

### 순서 비교

| 무용수 | HGSS | 수정 전 HnS | 수정 후 HnS |
|---|---|---|---|
| 옥희(Zuki) | 설명+예/아니오 → (예) 음악 → 회전 → 00045 대사 → 배틀 | 설명 → 회전 → 예/아니오 → (예) 방향 보정 → 배틀(직전에 자동 조우곡) | 설명 → 예/아니오 → (예) 창 닫기 → 방향 보정 → **음악** → 회전 → **00045 대사(한글)** → 배틀(자동 조우곡 없음) |
| 수정(Naoko) | 음악 → 이동 → 00015 → 배틀 | 꽃잎 소리 → 회전 → 접근 → 대사 → 배틀(직전 자동 조우곡) | **음악** → 꽃잎 소리 → 회전 → 접근 → 대사 → 배틀(자동 없음) |
| 난주(Miki) | 음악 → 이동 → 00016 → 배틀 | 같은 구조 | 같은 방식 |
| 예솔(Sayo) | 음악 → 이동 → 00017 → 배틀 | 같은 구조 | 같은 방식 |
| 태희(Kuni) | 음악 → 이동 → 00018 → 배틀 | 같은 구조 | 같은 방식 |

- 음악 시작 위치: 각 `…Battle` 라벨의 첫 줄(`playse SE_M_PETAL_DANCE` 앞). 옥희는 방향 보정 뒤·회전 앞(`delay 20`·꽃잎 소리·회전 블록은 옮기기만 함).
- 배틀 직전 중복 재시작 방지: `trainerbattle_no_intro`에 선택 인자 `music`(기본 `TRUE` = 이전과 같음)을 더하고 다섯 호출만 `NO_MUSIC` — `ConfigureTrainerBattle`이 `playMusicA`가 꺼지면 `EventSnippet_PlayTrainerEncounterMusic`을 넣지 않음. 공통 음악 함수·스크립트 복귀(`continueScript`)·방향(`facePlayer` FALSE)·플래그(`skipFlagCheck`) 인자는 그대로.
- 다음 무용수 연출 시작 때 조우곡을 새로 트는 것은 원작대로(앞 배틀 뒤 맵 BGM으로 돌아온 상태에서 다시 시작).
- 옥희 대사(00045, HGSS 공식 한국어): `그럼 당신과 포켓몬의\n유대감이 얼마나 강한지\l똑똑히 봐 두겠어요-` — Poké Corpus `57e5078` `HeartGoldSoulSilver/ko_msg.txt` 26935행(한국판은 파일 번호가 `Text File : 613`으로 붙지만 영어판 618 00045와 같은 위치·내용). 4세대 `\r`(스크롤 줄바꿈)은 3세대 `\l`로, 끝의 `\c`는 문장 끝으로. `-`는 원문 그대로. 폭(정적 추정, 실측 미실행): 가장 긴 둘째 줄 약 86 px.

### 분기·상태

- 첫 도전: 트리거 → `LegendaryCutscene`(설명) → `VAR_ECRUTEAK_CITY_THEATER = 6` → `ZukiBattle`(예/아니오). 회전은 이제 "예" 뒤 한 번만(첫 설명 뒤의 회전은 없앰).
- 거절: `LegendaryDenied`(기존 문장, `release`) — 음악·회전 없음. 상태 6 유지.
- 재도전: 옥희에게 말 걸기 → `EventScript_Zuki`(상태 6) → `ZukiBattle` → "예"면 음악·회전 한 번. 중복 회전 없음.
- 방향 보정(`VAR_FACING` 남·서·동)·`settotemboost`·트레이너 데이터·배틀 순서 그대로.
- 5연전 뒤: 배틀에서 돌아오면 맵 BGM(저장 안 했으므로 맵 기본) → 옥희 이동·방울(`ITEM_TIDAL_BELL`/`ITEM_CLEAR_BELL`) → 루기아·칠색조 분기 그대로. 패배: 기존 `no_intro` 패배 처리(전멸) 그대로, 상태 6이라 다시 옥희부터.
- 영향 범위: 다른 `trainerbattle_no_intro` 232곳은 세 번째 인자가 없어 기본값(자동 조우곡) 그대로. 신도지방 미하루·요코·시즈코(`NewSinjoh_KimonoHideout_hns`)는 손대지 않음. 옥희 00045 외 텍스트(영문 그대로)·패배 대사 변경 없음.

## 확인하지 않은 것(미실행)

- 빌드(스크립트 어셈블 포함), 자동 테스트, 세이브 정적 비교 도구, 실제 음악 청취, mGBA. 기존 실기 PASS 기록은 당시 ROM 범위 그대로.
- 실기 후보: ① 옥희 첫 도전(설명 → 예 → 조우곡 → 회전 → 한글 대사 → 배틀), ② 거절 후 재도전, ③ 수정~태희 각각 조우곡이 회전 전에 시작되고 배틀 직전에 다시 시작하지 않는지, ④ 각 배틀 뒤 맵 BGM 복귀, ⑤ 요약 화면 콘테스트 설명 20개 좁은 글꼴.
