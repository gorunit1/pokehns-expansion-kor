# full-sync 묶음 25 — seq 366~375 (+ #10159 앞당김, #10176 흡수)

- 요청: 사용자 "나 혼자 할 수 있는거야? 그렇다면 ㄱㄱ"(2026-10-22). 사전 분석(세션 임시 `b26/w` 로컬 클론에 10개 PR을 차례로 시험 적용)으로 사용자 혼자 정할 수 있는 범위인지 판단했다. 판단 근거는 아래 "결정"이다.
- 범위: seq 366~375. 376 #8893("The big grammar update", 텍스트 대량)은 문장을 건드려 따로 본다.
- upstream: 읽기 전용 사본 `/home/user/pokeemerald-expansion-upstream`에서 `git format-patch`/`git show`로 읽었다.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 366 | #10080 | `524d400b` | 맵 미리보기 이름 끄기 비트(`nameDisabled`). 튜토리얼 hunk 제외(아래) | 32,964,516 |
| 367 | #9878 | `64d2a8e3` | 알 재작업 **잔여분**: 테스트(`test/daycare.c` +1,187, `test/pokemon.c` givemon 알 테스트), `config_changes.h` 공백 | 32,964,516 |
| 368 | #9955 | `86037d5f` | 복권·환상의섬 일일 시드 설정(기본 FALSE) | 32,964,500 |
| 369 | #9966 | `ff04dcd8` | OW 종 변수 명령에서 바닐라 오브젝트 표 삭제 | 32,962,708 |
| 370 | #10076 | `b6d23d42` | `DetermineObjectEventDirectionFromObject` → `GetDirectionToFace` | 32,962,676 |
| 371 | #9080 | `830aee6c` | 밤 BGM 지원(HnS 적응 3개, 아래) | (32,964,804 — 단독으로는 헤더 정렬 깨짐) |
| 462 앞당김 | #10159 | `e13a671b` | 맵 헤더 구조 최적화(정렬 복구) | 32,963,684 |
| 372 | #10127 | `0edad98c` | enum 대량(형식만) | 32,963,524 |
| 373 | #10152 | `4c5718ae` | 안 쓰는 변수·함수 정리 | 32,963,428 |
| 374 | #10167 | `2c6edd5f` | `NightSong` → `night_music`, 맵 규칙이 mapjson에 의존 | 32,963,428 |
| 375 | #10166 | `f34a6921` | `MAX_LEVEL_UP_MOVES` 삭제 | 32,963,396 |
| 377 | #10176 | (371에 흡수) | 시각 기반 BGM 전환 삭제 — 371에서 처음부터 넣지 않음 | — |
| (HnS 테스트) | — | `92e1628d` | `Map names fit in popup`이 NULL 맵 헤더를 건너뜀(아래 검증) | 같음 |

이식 전(`603748e9` = 코드 `c5415296`) 32,964,580 B → **32,963,396 B(−1,184)**, EWRAM 250,408·IWRAM 25,516 같음, 최종 SHA1 **`fcfe16d658ef7c5e87423e12dcb86696ddfa8d52`**.

## PR별 내용과 HnS 판단

### 366 #10080 — 맵 미리보기 이름 끄기

- `struct MapPreviewScreen`의 `type`·`usesAllPalettes`가 비트필드가 되고 `nameDisabled` 비트가 생긴다. HnS 항목은 `type` 0~1(동굴·페이드인), `usesAllPalettes` TRUE/FALSE만 써서 들어간다.
- HnS 항목은 `nameDisabled`를 안 켠다 → 지명 창은 그대로 나온다.
- 제외: `docs/tutorials/how_to_map_preview_screen.md` hunk. 이 튜토리얼은 #9557(FRLG 맵 미리보기, seq 116, "HnS와 무관")에서 오는데 HnS에 없다.

### 367 #9878 — 알 생성·부화 재작업(잔여분)

- 게임 코드는 이미 들어와 있었다(원작업자 2026-09-15 선별 이식: 설정 `P_BALL_INHERITANCE`·`P_MOVE_INHERITANCE`, `givemon`/`createmon` `isEgg`, RNG 태그, 도감·DexNav 알기술 계산). 3-way 비교에서 upstream 변경 대부분이 HnS와 같아 충돌 없이 맞았다.
- 남은 차이는 모두 **HnS가 일부러 바꾼 것**이라 유지했다(게임 코드 변경 0):
  1. **알기술 전수(재확인 79):** HnS `TransferEggMoves`는 Gen 8+에서 같은 종이면 미러허브 없이, 다른 종이면 Gen 9에서 미러허브로 전수하고, 향로 베이비 기술도 본다. 맡길 때마다(어느 칸이든) 돈다. upstream `TransferEggMovesFromBoxmonToBoxmon`은 Gen 9에서 **같은 종이라도 미러허브가 필요**하고, 둘째 칸이 찰 때만 돈다(첫째 칸을 찾아간 뒤 다시 맡기면 안 돎). HnS 쪽이 설정 주석(`include/config/pokemon.h:33`)과 맞는다. **정정(2026-10-24):** 괄호 안 설명은 틀렸다 — upstream도 찾을 때 `ShiftDaycareSlots`가 남은 포켓몬을 칸 0으로 당겨서 다시 맡기면 칸 1이 차고 전수 검사가 돈다. 이 규칙은 친구 결정으로 바뀌었다([`egg-move-bgm-2026-10-24.md`](../egg-move-bgm-2026-10-24.md) 1절, 재확인 79 해결).
  2. `SetInitialEggData`: HnS 줍는 순간 이로치 판정(`P_EGG_SHINY_ROLL_ON_PICKUP`)·초기 기술.
  3. 부화 걸음: 남은 사이클이 감소량보다 작으면 바로 부화(너즐록 부화 차단 포함). upstream은 1 빼고 다음 사이클에 부화.
- 새 테스트 결과: PASS 26(아래 검증). FAIL 2는 **HnS 설정 `P_SEPARATE_REGIONAL_FORMS TRUE`** 때문이다(재확인 80): 알로라 나옹 엄마 × 나옹 → HnS는 알로라 나옹 알(upstream 기대는 지역 기본형 나옹), 식스테일 × 알로라 식스테일은 HnS에서 다른 종이라 아빠 볼을 물려받지 않음. 알기술 전수 테스트 6개는 HnS 이상해씨 알기술 데이터가 달라 `ASSUME`에서 멈춘다(HnS 규칙 차이는 이 테스트로 드러나지 않음).

### 368 #9955 — 일일 시드 설정

- `OW_USE_DAILY_SEED_FOR_VANILLA_VARIABLES FALSE`(기본) → 복권 번호·환상의섬 값은 그대로 세이브 변수에서, 하루 한 번 바뀐다.
- 제외: `src/new_game.c`의 두 번째 `#include "clock.h"`(HnS에 이미 있음).
- upstream 오타: `SetMirageRnd`의 `#if …VARIABLESD == FALSE` — 정의 안 된 이름이라 0으로 읽혀 변수를 계속 쓴다(지금과 같음). 설정을 TRUE로 켜면 이 줄도 고쳐야 한다.

### 369 #9966 — OW 종 변수

- `getoverworldwildencounterspecies`는 HnS에서 `InteractWithOverworldWildEncounter`(OW 야생 조우 오브젝트)만 쓴다. 지운 바닐라 그래픽→종 표(레쿠쟈·인형·전설의 개 등)는 HnS에서 닿지 않는다. ROM −1,792.

### 370 #10076 — 방향 계산

- 두 오브젝트가 **같은 칸**일 때만 결과가 다르다(예전 북, 이제 남). HnS에서 닿는 호출부는 없다: NPC 동행은 꺼져 있고(`FNPC_ENABLE_NPC_FOLLOWERS FALSE`, 같은 칸이면 `DIR_NONE`을 먼저 돌려줌), OW 야생 조우의 동행 포켓몬 갈래는 조우가 플레이어 옆이 아닐 때만 돈다.

### 371 #9080 + 462 #10159 + 377 #10176 — 밤 BGM

- 맵 헤더에 `nightMusic`(map.json `night_music`, 없으면 `MUS_NONE`)과 곡→밤 곡 표(`sNightMusicTable`)가 생긴다. **HnS 맵은 밤 곡을 안 쓰고 표도 비어 있어 BGM은 그대로다.**
- HnS 적응(재확인 81):
  1. `END_MUS`를 558 대신 HnS 마지막 곡 묶음(`DP_MUSIC_END`) 뒤에 둔다 — HGSS/DPPt 곡도 표 안에 들어오고 밤 곡으로 쓸 수 있다(표 395칸 = 790 B, 350 `MUS_LITTLEROOT_TEST` ~ 744).
  2. `GetNightMusicFromTrack`에 범위 검사. upstream은 아무 곡 번호로 표를 읽어서, HnS에서는 밤에 `MUS_NONE`(방울탑 옥상)·HG/DP 곡 번호로 표 끝을 넘어 ROM을 읽는다(읽은 값이 350~558이면 엉뚱한 곡).
  3. upstream #9080은 `OverworldBasic`의 시각 갱신(게임 시간 1분마다)에서 `TransitionMapMusic()`을 부른다. HnS에서는 라디오·자전거·이벤트·`MUS_NONE` 음악을 맵 곡으로 되돌린다. upstream도 #10176(seq 377)에서 지웠으므로 **처음부터 넣지 않았다**(377은 흡수).
- **#10159 앞당김:** upstream #9080은 헤더를 0x1E 바이트로 내보내는데 구조체는 0x20이라 헤더가 2바이트씩 어긋난다(이 빌드에서 `CherrygroveCity_hns`가 `0x0940446e`). upstream이 #10159(seq 462)에서 `cave`를 플래그 비트(`requires_flash`)로 옮기고 빈 바이트를 지워 28바이트로 되돌렸다. 바로 다음 커밋으로 넣었다(`CherrygroveCity_hns` `0x0940446c`). HnS `requires_flash`는 true 11·false 1,488개라 1비트에 들어가고, `gMapHeader.cave`는 참/거짓으로만 읽힌다(`overworld.c`·`fldeff_flash.c`·`battle_script_commands.c`). **`830aee6c` 단독 빌드는 쓰지 말 것.**
- 주의(빌드): #10167 전까지 맵 헤더 규칙이 mapjson에 의존하지 않아, 증분 빌드에서는 옛 헤더가 남는다. 이 묶음 빌드는 `map.json`을 touch해서 다시 만들었고, #10167 뒤로는 자동이다.

### 372 #10127 — enum 대량

- 지역 변수·인자·필드 형식만 바꾼다(171파일 중 168). `B_ACTION_*` define → `enum BattleAction`(값 같음, HnS 목록대로 `B_ACTION_UNK_15` 없음).
- HnS 손 맞춤: `IsAdditionalEffectBlocked`는 이미 enum, 배틀 피라미드 `SaveMonHeldItems`는 `heldItem`만(HnS 레벨 50 재계산 유지), 안 쓰는 `DoStatChangeResolution` 선언과 `ChangeOrderTargetAfterAttacker`의 가려진 `u32 i` 삭제. 테스트 3파일(mega_sol·weather_ball은 HnS판에 해당 지역 변수 없음, spicy_extract 테스트는 HnS에 없음)은 그대로.
- 세이브 구조체는 잎 형식 이름만 바뀐다(`species`·`heldItem` u16 → enum, 비트 위치·폭 같음 — 정적 비교 WARN 9의 내용).

### 373 #10152 / 374 #10167 / 375 #10166

- #10152: 안 쓰는 함수·변수 삭제. `GetSpritePalIdxByBattler`는 인자를 그대로 돌려줬으므로 배경 마스크 팔레트 결과 같음.
- #10167: map.json 키 이름과 맵 규칙 의존성. HnS `map_data_rules.mk` preproc 줄이 달라 손으로 넣었다(바뀐 줄은 upstream과 같음).
- #10166: HnS는 `MAX_LEVEL_UP_MOVES`를 40으로 올려 두었는데, 상한은 기술 떠올리기·도감 반복에만 걸렸다. HnS 빌드의 최장 레벨업 학습표가 30개(`*LevelUpLearnset` 심볼 크기)라 잘린 적이 없고 결과 같음.

## 결정 (사용자 혼자 정할 수 있다고 본 이유)

- HnS 게임 동작 변화 0(코드 해석 + 테스트): 새 기능 2개(맵 미리보기 이름 끄기, 밤 BGM)·설정 1개(일일 시드)는 HnS가 쓰지 않거나 기본 FALSE다. "HnS 일반 진행이 안 쓰는 기능은 켜지 마"를 따랐다.
- #9878의 HnS 규칙 차이는 **바꾸지 않고 유지**했다(기존 동작 보존). 친구가 upstream 규칙을 원하면 그때 바꾼다(질문, 재확인 79).
- 앞당김 2개(#10159, #10176)는 upstream이 #9080 결함을 고친 것이라 회귀 수정 앞당김 원칙(묶음 22·24 선례)과 같다.
- 새 배틀 문장 0, 배틀 밖 새 텍스트 0, 기존 텍스트 변경 0.
- 세이브 구조 변화 0(정적 비교 PASS). 맵 헤더·밤 곡 표는 ROM 데이터라 세이브와 무관.

## 검증

- **빌드:** 커밋마다 `make hns -j8` 종료 0, 새 경고 0(`warncheck.sh`). ROM은 위 표.
- **세이브 정적 비교**(이식 전 사실 = 세션 임시 `b25/save-pre366`, 코드 `603748e9`를 클론에서 빌드 — ROM 32,964,580 B로 `c5415296`과 같음): **PASS(FAIL 0, WARN 9)**. WARN 9 = #10127 형식 이름 변경 8개(SaveBlock1·PokemonStorage(+ASLR)·Pokemon·BoxPokemon·HallofFame 2 — 바이트 배치 같음) + 녹화배틀 1개(같은 형식 이름 변경, 녹화배틀 함수 4개 기계어 차이는 ROM 주소 해시뿐).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 `92e1628d`): 러너 PASSED **2,749 / TOTAL 5,709**(KNOWN_FAILING 25), 목록 PASS 2,744 — `test-baseline-seq363.txt` 대비 **사라진 PASS 0, 새 PASS 27**(#9878 새 키우미집 테스트 25 + `givemon [egg]` + #10166 이름 바뀐 `Pokémon level up learnsets fit within MAX_RELEARNER_MOVES` — 옛 이름은 INVALID였음). 새 기준 [`test-baseline-seq375.txt`](test-baseline-seq375.txt)(**다음 비교 기준**).
  - 중간(`f34a6921`)에 `Map names fit in popup` PASS → FAIL(85 px > 80)이 나왔다. 원인: HnS 빌드는 맵 1,495개 중 935개(대부분 에메랄드 맵)를 빼서 `gMapGroups` 칸이 NULL인데, 테스트가 NULL 헤더를 주소 0(BIOS)으로 읽었다. 헤더 배치가 바뀌어 `floorNumber`(0x17) 자리의 값이 229가 되자 쓰레기 지명에 "B27F" 층 접미사가 붙었다. 테스트가 NULL 헤더를 건너뛰게 고쳤다(`92e1628d`, 게임 코드 변경 0 — 게임은 NULL 맵 헤더를 읽지 않는다). 고친 뒤 PASS.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-b25 8`, 코드 `92e1628d`): **563/652**, 기대와 바이트 같음(`HNSB2021` 30/30), `판정: PASS (기대와 같음)`.
- **실기: 하지 않음(친구).**

## 실기(mGBA) 후보 — 친구

1. 맵 BGM: 아무 마을·도로·동굴에서 **밤**에 들어가고 나가기(밤 곡 표가 비어 있어 낮과 같아야 함), 방울탑 옥상(밤·`MUS_NONE` + 바람)·라디오 점거 금빛시티.
2. 맵 이동 전반(헤더 구조 바뀜): 여러 마을·동굴 출입, 플래시가 필요한 동굴(어두움·플래시 사용), 자전거 가능 여부, 지명 팝업.
3. 키우미집: 맡기기·찾기, 알 받기, 부화(같은 종 둘을 맡겼을 때 빈 칸 알기술 전수가 지금처럼 되는지).
4. 동굴 맵 미리보기(있는 곳)에서 지명 창이 그대로 나오는지.

## 재확인·뒤 PR 메모

- 79·80·81(새).
- 순서표: 366~375 기록, 377은 371에 흡수, 462는 앞당김(다시 넣지 않음).
- 다음은 seq 376 #8893(The big grammar update — 텍스트 대량, 문장 정책 확인 필요).
