# 친구 답 2026-10-14 반영 — 미러 챌린지 패배 처리(재확인 35)·너즐록 보호, 웅 머그샷 표시 범위

- 친구 답 원문: [`FRIEND_REPLY_2026-10-14.md`](../../FRIEND_REPLY_2026-10-14.md)(묶음 18·19 확인, 미러 패배 결정), [`FRIEND_REPLY_2026-10-14b.md`](../../FRIEND_REPLY_2026-10-14b.md)(웅 머그샷). 작업: 클라우드 세션, 2026-10-14. 시작 HEAD `86c34e07`(코드 = 묶음 20 끝 `b89fb8e9`, ROM SHA1 `bdc39338…`).
- 아래는 모두 빌드·자동 테스트·코드 감사·정적 비교 결과다. **실기(mGBA)는 친구가 한다** — 친구 로컬 실기 PASS(웅 머그샷 한 장면)는 친구 결과로 따로 적었다.

## 커밋

| 커밋 | 내용(친구 답 절) | ROM(B) |
|---|---|---:|
| `5e7e97c2` | **웅 머그샷 표시 범위**(14b 1·2절) — `4b885582`의 `y2 -= 8`을 지우고, 친구가 준 코드 그대로 상대 A/B 그림이 `TRAINER_PIC_LEADER_BROCK_HNS`이면 `sheetTileStart += 8` + `SetSpriteSheetFrameTileNum()`. 주석도 친구 문구. 두 방식을 같이 두지 않음 | 32,782,532 |
| `4de35b2b` | **미러 챌린지 패배 → 화이트아웃**(14 2절, 재확인 35) — seq 315 #10568에서 뺐던 `battle_setup.c` hunk를 upstream 그대로(`b19/seq315_optional_battle_setup.patch`). `CB2_EndTrainerBattle`에서 필드 재호출 `(!NoAliveMonsForPlayer())`를 지우고 기권 분기를 패배 분기에 합침(예외는 배틀피라미드·트레이너힐·`B_FLAG_NO_WHITEOUT` 그대로). 결과가 upstream `expansion/1.17.0`·`1.17.1`·`master`의 같은 분기와 글자까지 같음(이 세션에서 세 판 본문 확인) | 32,782,468 |
| `da0adeaa` | **너즐록 화이트아웃 보호(HnS, 친구 확인 요청 — 재확인 67)** — 위 변경 검토 중 찾음. `DoWhiteOut`의 너즐록 보충(박스 첫 포켓몬을 파티 1번에 / 박스가 비면 파티를 지우고 꼬렛)을 **파티에 산 포켓몬(알 제외)이 없을 때만** 하도록 조건 하나 추가. 아래 1절 "너즐록과 함께" | 32,782,484 |

## 1. 미러 챌린지 패배 처리 (재확인 35)

### 기록 구분(친구 지시)

- **이 동작은 full-sync 전 HnS부터 있었다.** 이번 이식이 만든 회귀가 아니다. 개발자 의도나 인정된 버그라는 근거는 없다. 이번 변경은 **우리 한글패치 프로젝트에서 미러 챌린지의 패배 처리를 실제 승패에 맞추기로 선택한 기존 동작 변경**이다.
- 아테나 멀티(재확인 64, `32a5fd5a` #10814 앞당김)는 미러 패배 처리와 다른 항목이다. **정정(2026-10-14, 묶음 21):** "#8943 이식 뒤 아테나 승리값이 덮였다"는 원인 해석은 잘못이었다 — 반쪽 팀 멀티는 그 덮어쓰기 경로를 타지 않는다(재확인 64, [`full-sync-seq-337-350.md`](full-sync-seq-337-350.md) "재확인 64 정정"). #10814 한 줄은 풀 팀 멀티용으로 유지.

### 바뀐 경로 (코드 감사)

- 일반 미러(도둑미러 아님)의 동작을 따라가 보면 다음과 같다.
  1. 배틀 시작 때 `src/battle_main.c`가 원래 파티를 `gPlayerPartyBackup`에 저장하고, 플레이어 칸에 상대 파티를 복사한다.
  2. 배틀 끝 `HandleEndTurn_FinishBattle`이 승패와 관계없이 원래 파티를 복구한다.
  3. 그 뒤 `CB2_EndTrainerBattle`이 이전에는 `NoAliveMonsForPlayer()`를 다시 불렀다. 복구된 원래 파티가 살아 있으면 FALSE가 되어 화이트아웃 없이 `CB2_ReturnToFieldContinueScriptPlayMapMusic`으로 갔다.
  4. 그래서 `trainerbattle` 다음 줄이 실행됐다.
- **비상 체육관 예시**(`data/maps/VioletCity_Gym_hns/scripts.inc` `VioletCity_Gym_EventScript_Falkner`): `trainerbattle_no_intro TRAINER_FALKNER_1_HNS` 바로 뒤에 다음이 승패 검사 없이 이어진다.
  - 배지 메시지와 팡파르
  - `setflag FLAG_BADGE01_GET`, `setflag FLAG_DEFEATED_VIOLET_GYM`
  - `addvar VAR_NUM_BADGES, 1`
  - `giveitem ITEM_TM_ROOST`, `setvar VAR_VIOLET_CITY_STATE, 3`, `setrespawn`
  - 그래서 이전 코드에서는 미러로 져도 원래 파티가 살아 있으면 이 보상이 지급될 수 있었다. 친구가 코드에서 본 경로와 같다.
  - 이제는 `IsPlayerDefeated(gBattleOutcome)`이면 `CB2_WhiteOut`으로 가서 위 스크립트가 실행되지 않는다.
  - 트레이너 처치 플래그(`SetBattledTrainersFlags`)는 원래도 승리 분기에서만 세운다.

### `CB2_EndTrainerBattle`을 타는 배틀과 결과

- 이 콜백을 쓰는 곳은 `BattleSetup_StartTrainerBattle`(일반 `trainerbattle_*`, 피라미드·트레이너힐)과 `CB2_EndDebugBattle`뿐이다.
- 이전 코드와 결과가 갈리려면 "패배인데 필드에서 플레이어 파티에 산 포켓몬이 있음"이어야 한다. 아래 표가 그 경우를 따진 것이다.

| 경우 | 이전 | 지금 | 근거 |
|---|---|---|---|
| **일반 미러, 일반 트레이너전 패배** | 원래 파티가 살아 있으면 스크립트 계속(보상 지급 가능) | **화이트아웃**(상금 감소, 리스폰), 스크립트 실행 안 됨 | 위 경로 |
| 일반 미러, 승리 | 원래 파티 복구 → 승리 분기 | 같음 | 승리 분기 안 바뀜 |
| 도둑미러(`tx_Challenges_Mirror_Thief`) 패배 | 복구 없음 → 플레이어 파티 = 복사한 상대 포켓몬(모두 배틀에 나와 기절) → 화이트아웃 | 같음 | 복사는 6칸 전부 상대 파티(빈 칸 포함) — 패배 = 전멸 |
| 도둑미러 승리 | 상대 포켓몬을 가진 채 승리 분기 | 같음 | 파티 보유 규칙 안 바뀜 |
| 미러 끈 일반 트레이너전 패배 | 전멸 → 화이트아웃 | 같음 | 패배면 산 포켓몬 없음 |
| 기권(`B_RUN_TRAINER_BATTLE TRUE`) | 기권 분기: 예외 3개 아니면 화이트아웃 | 패배 분기에 합침: 같은 예외 3개, 아니면 화이트아웃 | `IsPlayerDefeated`에 `B_OUTCOME_FORFEITED` 포함, 기권 결과는 `CanPlayerForfeitNormalTrainerBattle()`일 때만 생김 |
| 초기 라이벌(`trainerbattle_earlyrival`) | 그 앞 분기(`VAR_RESULT`, 회복 또는 화이트아웃) | 같음 | hunk 앞 분기 |
| 재전(`trainerbattle_rematch*`) | `CB2_EndRematchBattle`: 패배면 항상 화이트아웃(미러에서도) | 같음 | 다른 콜백. 이제 일반 트레이너전이 재전과 같은 규칙이 됨 |
| 배틀피라미드·트레이너힐·`B_FLAG_NO_WHITEOUT` | 스크립트 계속 | 같음 | 예외 조건 그대로(HnS `B_FLAG_NO_WHITEOUT` = 0) |
| 목호 반쪽 팀 멀티(`multi_2_vs_2` — 아테나 등)·배틀타워 NPC 멀티·시설 | `HandleSpecialTrainerBattleEnd`·시설 콜백 | 같음 | 이 콜백을 타지 않음. 배틀 중 패배 판정(`NoAliveMonsForPlayer` HnS판, 묶음 19)도 안 바뀜 |
| 팔로워 NPC 파트너 배틀 | (HnS에서 없음) | — | `FNPC_ENABLE_NPC_FOLLOWERS FALSE` |
| Sky Battle | (HnS에서 없음) | — | `B_FLAG_SKY_BATTLE 0` |
| 디버그 파트너 배틀 패배 | 원래 파티 복구 뒤 스크립트 계속 | 화이트아웃 | `CB2_EndDebugBattle`(디버그 메뉴 전용) |

- 그대로 둔 것: #8943 파티 가드, 맹세 가드, AI 교체 무한 루프 방지 가드, HnS 반쪽 팀 멀티·배틀타워 NPC 멀티 패배 판정(`battle_script_commands.c`), 미러 파티 복사·복구 코드(`battle_main.c`), 세이브 구조. diff는 `src/battle_setup.c` 한 함수 −9/+2줄.
- 원래 파티·도구: 일반 미러는 승패와 관계없이 `HandleEndTurn_FinishBattle`에서 원래 파티로 돌아간다(안 바뀜). 복사한 파티가 남지 않는다. 화이트아웃은 원래 파티를 회복시킨다(`DoWhiteOut`의 기존 동작).

### 너즐록과 함께 켠 경우 — 보호 장치 `da0adeaa` (재확인 67, 친구 확인 요청)

- 미러와 너즐록(`tx_Challenges_Nuzlocke`·쉬움 모드)은 챌린지 메뉴에서 같이 켤 수 있다.
- `DoWhiteOut`(`src/overworld.c`)의 너즐록 분기는 화이트아웃 때 파티가 "죽은 포켓몬이 지워지거나 PC로 간 뒤의 빈 파티"라고 가정한다(`HandleEndTurn_FinishBattle`의 `NuzlockeDeleteFaintedPartyPokemon`).
  - 박스에 산 포켓몬이 있으면 그 포켓몬을 **파티 1번 자리에 덮어쓴다**(`MoveFirstBoxPokemonToParty` → `BoxMonAtToMon(…, &gParties[B_TRAINER_PLAYER][0])`).
  - 박스가 비어 있으면 **파티를 지우고 꼬렛 1마리**를 준다.
- 일반 미러는 원래 파티를 너즐록 삭제보다 먼저 복구한다(같은 함수 안, 복구가 앞). 그래서 `4de35b2b` 뒤 미러로 진 일반 트레이너전은 **산 원래 파티로 화이트아웃**한다.
  - 보호 장치가 없으면 쓰러진 적도 없는 1번 포켓몬이 영구히 사라진다(박스가 비면 파티 전체).
  - 이 경로는 `4de35b2b`가 새로 연다. 그 전에는 화이트아웃 자체가 없었다(대신 보상 경로).
- 같은 일이 **이전부터 있던 경로**:
  - 너즐록에서 기권(`B_RUN_TRAINER_BATTLE TRUE`, 산 파티로 화이트아웃)
  - 미러 재전 패배(`CB2_EndRematchBattle`은 원래도 패배면 화이트아웃)
- 보호 장치: 너즐록 분기 조건에 `CountPartyAliveNonEggMonsExcept(PARTY_SIZE) == 0`을 더했다. 파티에 산 포켓몬(알 제외)이 하나라도 있으면 보충하지 않고 일반 화이트아웃(회복·리스폰)만 한다.
  - 원래 의도인 "전멸 → 삭제된 뒤 화이트아웃"은 산 포켓몬이 0이라 그대로 보충한다.
  - 삭제를 건너뛰는 배틀 종류(파트너·시설 등)에서 기절한 채 남은 파티도 산 포켓몬이 0이라 그대로다.
  - 필드 독으로 전멸한 경우도 그대로다.
- **결정 구분:** 너즐록 규칙은 HnS 고유 기능이라 최종 결정은 친구다. 사용자 결정(2026-10-14)으로 친구 답 전까지 안전한 쪽(보호 장치 넣음)으로 두었다. 친구가 빼라고 하면 이 커밋만 되돌리면 된다.

### 실기 (친구, 새 ROM)

친구 2절 검증 1~5를 아래 장면에서 확인하면 된다.

1. 배지 없는 상태에서 일반 미러로 비상에게 진다.
   - 화이트아웃되는지 본다.
   - 배지·기술머신(날개쉬기)·배지 수·`FLAG_DEFEATED_VIOLET_GYM`·마을 상태가 그대로인지 본다.
   - 다시 비상에게 말을 걸면 배틀이 다시 시작되는지 본다.
2. 같은 조건에서 이기면 기존 보상과 진행이 그대로인지 본다.
3. 이기든 지든 원래 파티와 도구가 돌아오는지 본다.
4. 도둑미러로 이기면 상대 포켓몬을 가진 채인지, 지면 화이트아웃되는지 본다.
5. 다음 예외가 그대로인지 본다.
   - 미러 끈 일반 패배
   - 기권
   - 초기 라이벌(회복 후 계속)
   - 아테나 멀티(재확인 64)
   - 배틀타워 멀티
6. (보호 장치) 너즐록 + 일반 미러로 트레이너에게 지면 화이트아웃 뒤 원래 파티 6마리가 그대로인지 본다(박스에 산 포켓몬이 있을 때와 없을 때). 너즐록에서 일반적으로 전멸했을 때 박스 포켓몬 보충이 이전과 같은지도 본다.

- 재확인 35 = **수정 반영, 친구 실기 대기**. 재확인 67(너즐록 보호) = **친구 확인 대기**.

## 2. 웅 머그샷 표시 범위

- 머그샷 상대 그림은 `CreateTrainerSprite`가 64×64 그림 시트(`TRAINER_PIC_SIZE` 0x800 B = 64타일, `tileTag` 있음 → `usingSheet`)를 올린 스프라이트다. 머그샷에서는 OAM을 64×32로 줄여 2배 확대(`ST_OAM_AFFINE_DOUBLE`)한다.
  - 원래는 그림의 0~31행이 보였다.
  - `sheetTileStart += 8`이면 64 px 폭의 한 타일 줄(8타일)을 건너뛴다. 그래서 **원본 8~39행**(타일 8~39)이 보인다.
  - 참조 타일 8~39는 시트 할당 0~63 안이다.
  - `SetSpriteSheetFrameTileNum()`이 `oam.tileNum = sheetTileStart + 프레임 값`(기본 애니 프레임 0)으로 반영한다. 애니메이션이 `tileNum`을 다시 계산해도 같은 `sheetTileStart`를 쓴다.
- 이전 `y2 -= 8`과 다른 점: 표시 영역(띠 안 위치, 아래 경계)은 그대로이고 읽는 줄만 바뀐다.
  - 화면에서는 원본 8행 = 16 px 위로 올라간 그림이 된다.
  - 몸이 띠 아래 경계까지 이어진다.
- 그대로인 것:
  - 64×32 크기, 2배 확대, 기본 좌표
  - 슬라이드(위치 판정·타이밍·표시/숨김 — `UpdateMugshotTrainerPicVisibility`는 x만 봄)
  - `leader_brock_hns.png`·팔레트, 일반 배틀 위치·그림
  - 다른 트레이너·파트너 머그샷, 실버 멀티 목호·이향 x 보정
  - 새 그림·그림 ID 없음
- 이 그림을 쓰는 트레이너는 `TRAINER_BROCK_HNS`(일반 웅)와 `TRAINER_BROCK_POSTOBC_HNS`(OBC 뒤 웅) 둘 다 `TRAINER_PIC_LEADER_BROCK_HNS`다(`src/data/trainers_hns.h`).
  - 상대 B 분기도 같은 처리다(`opponentSpriteB != NULL && trainerBPicId == …`).
  - 시설 웅(`FACILITY_CLASS_LEADER_BROCK_HNS`)도 같은 그림 ID지만, 시설 배틀은 머그샷 전환을 쓰지 않는다.
- diff는 `src/battle_transition.c` `Mugshots_CreateTrainerPics()` 끝의 웅 처리만이다(+12/−5줄, ROM +32 B).

### 친구 로컬 실기와 이 저장소 결과의 구분

- **친구 로컬 실기 PASS**(친구 기록 — 이 저장소에서 재현 못 함):
  - 기준 HEAD `267b8ec66778e06ac2411b5ef11a471499d67cd4`에 미커밋 로컬 수정(위 코드)을 넣은 ROM이다.
  - `make hns -j16`, GCC 14.2.1, 종료 0, SHA1 `6bb6acc048ae98e0290e0c5d74744b83c806023c`.
  - 화면 `pokehns-12.png`에서 머리 위치와 몸의 아래 경계를 확인했다.
  - 그 한 장면만이다. 일반·OBC 양쪽 이벤트, 슬라이드 전체, 일반 배틀, 다른 트레이너는 아직 실기 대기다.
  - 미커밋 수정이라 HEAD만으로는 그 ROM을 재현할 수 없다.
- 이 저장소의 `5e7e97c2`는 같은 코드(`battle_transition.c` 줄 단위 같음)를 묶음 20 위에 넣은 것이다. 컴파일러·기준 커밋이 달라 ROM 해시는 친구 ROM과 비교 대상이 아니다.

## 검증

- **빌드:** 커밋마다 `make hns -j8` 종료 0, 새 경고 0, EWRAM 250,408 B·IWRAM 25,516 B 그대로. ROM 32,782,500(묶음 20 끝) → 32,782,532(+32) → 32,782,468(−64) → **32,782,484 B(+16)**.
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j4`, 코드 `da0adeaa`): PASS 2,683 / TOTAL 5,608, 목록이 `test-baseline-seq336.txt`와 **바이트 같음**(사라진 PASS 0, 새 PASS 0 — 바뀐 세 곳 모두 테스트 러너가 타지 않는 필드·전환 화면 코드)
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-f14 8`, 코드 `da0adeaa`): **532/622**, 기대 요약과 바이트 같음
- **세이브 정적 비교**(이식 전 사실 = 묶음 20 끝 `b89fb8e9` 빌드 — `post-336`): `4de35b2b`·`da0adeaa` 빌드 모두 **PASS(FAIL 0, WARN 0)**. `CB2_EndTrainerBattle`·`Mugshots_CreateTrainerPics`·`DoWhiteOut`은 세이브 경로 코드가 아니다. 세이브 블록·RAM 변수는 그대로다.
- **배틀 테스트로 잴 수 없는 것:** 세 변경 모두 필드 콜백·배틀 전환 화면이라 테스트 러너가 타지 않는다. 그래서 위 코드 감사와 친구 실기로 확인한다.
- **실기용 ROM:** 코드 `da0adeaa`, 클라우드 `make hns -j8`(`arm-none-eabi-gcc 13.2.1`) → 사용 32,782,484 B, SHA1 `c95e0c815ebc4f1138a78fe14bafff6198eab2d9`.
  - 묶음 16~20 전부, #10814·#10829 앞당김, 폭주족 정리, #10428·#10535 설명, 방벽 진영·배리어프리, 이번 세 변경이 들어 있다.
  - 친구가 기록한 `d4e3b8b6`/`e51b1c5b…` 뒤로 묶음 20(`b89fb8e9`/`bdc39338…`)이 더 들어갔다.
