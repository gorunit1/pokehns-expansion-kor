# 알기술 전수(재확인 79)·트레이너 조우 BGM·목호 패배 장면 음악 (친구 답 2026-10-24)

원문 [`FRIEND_REPLY_2026-10-24.md`](../FRIEND_REPLY_2026-10-24.md). 작업은 기존 작업 브랜치 `pokehns-expansion-kor`에서 필요한 파일만 고쳤다(메인 로컬 저장소 동기화·강제 변경 없음). mGBA 실기는 친구가 한다.

## 커밋

| 커밋 | 파일 | 내용 |
|---|---|---|
| `7f69a3be` | `src/daycare.c`, `test/hns_daycare_egg_moves.c`(새) | 알기술 전수 규칙(1절) |
| `17755b4d` | `src/data/trainers_hns.party` | 수영팬티(남) 23명 조우 BGM(2절) |
| `4ec770aa` | `data/maps/PokemonLeague_ChampionsRoom_hns/scripts.inc` | 목호 패배 뒤 문 열림 음악(3절) |
| (이 문서와 같은 커밋) | 원문, 이 문서, `RECHECK_BEFORE_COMPLETION.md` 79, `full-sync-seq-366-375.md` 정정, `test-baseline-seq468b.txt`, STATUS·SESSION_LOG | 기록 |

---

## 1. 키우미집 알기술 전수(재확인 79)

**기록 문구:** SV식 도구 조건(흉내허브)을 HnS 키우미집에 적용한 것. 아래 기술 고르는 순서는 upstream 순서이고, **공식 SV의 순서와 같다고 주장하지 않는다**(SV에는 키우미집이 없고 피크닉에서 전수한다).

### 1.1 조건

| 설정 `P_EGG_MOVE_TRANSFER` | 전수 조건 | 비고 |
|---|---|---|
| Gen 8 | 두 마리가 **같은 종**(기본 종 ID 비교, `GET_BASE_SPECIES_ID`)일 때만 | 흉내허브는 조건에도 예외에도 안 씀(지녀도 다른 종이면 전수 없음) — 이전 HnS와 같음 |
| **Gen 9 이상(HnS 기본 `GEN_LATEST`)** | **받는 쪽이 흉내허브를 지닐 때만**, 종 무관 | 이전 HnS의 "같은 종이면 도구 없이" 예외를 없앰. 허브는 **소모되지 않음**. 성별·알 그룹은 막지 않음(검사 자체가 없음). 가르치는 쪽이 지닌 허브는 효과 없음 |
| Gen 7 이하 | 전수 없음 | 호출부(`StorePokemonInDaycare`, `>= GEN_8`) 그대로 |

### 1.2 시점

- **맡길 때마다** 검사한다(HnS 유지). 키우미집 두 칸 모두에 유효한 포켓몬(`MON_DATA_SANITY_HAS_SPECIES`)이 있을 때만 하고, 칸 0 ← 칸 1, 칸 1 ← 칸 0 두 방향을 본다.
- 한 마리만 맡겨져 있으면 상대 칸이 비어 있어 아무 일도 없다.
- 찾았다가 다시 맡기면 다시 검사한다.

### 1.3 순서

- 가르치는 쪽의 **기술칸 1→4 순서**(upstream `TransferEggMovesFromBoxmonToBoxmon`과 같은 순서). 이전 HnS는 받는 쪽 알기술 목록 순서였다.
- 건너뜀: 빈 칸(`MOVE_NONE`), 받는 쪽이 이미 아는 기술, 받는 쪽 알기술이 아닌 기술.
- 받는 쪽 빈 칸에만 넣고(`GiveMoveToBoxMon`), 칸이 다 차면 멈춘다. **덮어쓰지 않는다.** 같은 칸을 두 번 쓰지 않는다(이미 아는 기술 검사로 같은 기술 중복도 없음).
- 가르치는 쪽의 기술·PP·도구는 바꾸지 않는다.

### 1.4 유지한 것

- 받는 쪽 알기술 판정은 HnS 향로 베이비 처리 그대로: 진화한 향로 계열(예: 마릴)은 Gen 9 이상에서 베이비(루리리) 목록과 자기 목록 둘 다, Gen 9 미만에서는 자기 목록만(`IsDaycareEggMove`).
- 알 생성·폼/볼 유전·부화·이로치·너즐록 알 규칙·향로 베이비/비베이비 표: 코드 변경 없음(바꾼 함수는 `TransferEggMoves` 계열 둘뿐, `StorePokemonInDaycare` 호출부 그대로).

### 1.5 upstream 설명 정정(재확인 79·seq 367 문서)

이전 기록 "upstream은 둘째 칸이 찰 때만 돈다(**첫째 칸을 찾아간 뒤 다시 맡기면 안 돎**)"는 **틀렸다.** upstream 1.17.1 `TakeSelectedPokemonMonFromDaycareShiftSlots` → `ShiftDaycareSlots`가 남은 포켓몬을 칸 0으로 당기므로 다시 맡기면 항상 칸 1이 채워지고, `StorePokemonInEmptyDaycareSlot`이 칸 1을 채울 때 `TransferEggMoves`를 부른다. 즉 upstream도 사실상 "두 마리가 될 때마다" 검사한다. HnS의 "맡길 때마다"와는 한 마리만 있을 때(어차피 아무 일 없음)만 다르다. `RECHECK_BEFORE_COMPLETION.md` 79와 `full-sync-seq-366-375.md` 367절에 정정을 적었다.

### 1.6 회귀 테스트 — `test/hns_daycare_egg_moves.c`

- 실제 HnS 학습 데이터 조합. 새 게임 기본값(챌린지 `tx_Mode_Modern_Moves = 1`, `src/new_game.c`)을 먼저 켜고 조합을 `ASSUME`으로 확인한다:
  - 받는 쪽 **이상해씨**: 알기술 저주·뿌리박기·자연의힘·망각술, 몸통박치기는 레벨업(알기술 아님).
  - 같은 종 선생 **이상해씨**, 다른 종 선생 **야돈**(저주·망각술을 레벨업으로 배움).
  - 향로 계열 **마릴**: 배북은 마릴 자기 목록, 흉내쟁이는 루리리 목록에만.
- 맡기기는 실제 스크립트 경로(`givemon` → `StoreSelectedPokemonInDaycare`, 찾기 `TakePokemonFromDaycare`).
- 결과(코드 `4ec770aa`, 전체 `make check` 안에서 실행 — 4절). 이전 코드 열은 세션 중 `7f69a3be^`의 `daycare.c`로 같은 테스트만 돌린 결과(`TESTS='HnS egg move transfer'`):

| # | 테스트 이름("HnS egg move transfer: …") | 내용 | 새 코드 | 이전 코드(`7f69a3be^`) |
|---|---|---|---|---|
| 1 | Gen 8 shares only between the same species | Gen 8: 같은 종 허브 없음·있음 → 배움, 다른 종 허브 없음·있음 → 안 배움(4조) | PASS | PASS |
| 2 | Gen 9 needs the receiver's Mirror Herb | Gen 9: 같은 종·다른 종 × 받는 쪽 허브/없음/가르치는 쪽만 허브(6조) | PASS | **FAIL**(같은 종 허브 없이 배움) |
| 3 | full slots kept, known and non-Egg Moves skipped | 칸 꽉 참 → 그대로, 이미 앎 → 다음 기술, 첫 기술이 알기술 아님 → 다음 기술(3조) | PASS | PASS |
| 4 | one empty slot takes the teacher's first transferable move | 빈 칸 1개 — 가르치는 쪽 순서 망각술/저주를 바꿔 2조 | PASS | **FAIL**(알기술 목록 순서) |
| 5 | several empty slots follow the teacher's slot order | 빈 칸 3개 — 자연의힘·뿌리박기·저주 순 | PASS | **FAIL** |
| 6 | either slot can receive, in either deposit order | 받는 쪽이 먼저/나중(2조), 가르치는 쪽은 그대로 | PASS | PASS |
| 7 | withdraw and deposit again, and a single Pokemon | 칸 0/칸 1을 찾았다가 기술 하나 잊고 다시 맡김 → 배움(2조), 한 마리만 → 아무 일 없음 | PASS | PASS |
| 8 | Mirror Herb kept, other data unchanged | 허브 그대로, 새 기술 PP 최대, 성격값·ID·경험치·기존 PP 그대로, 가르치는 쪽 기술·PP·도구(오렌열매)·성격값 그대로 | PASS | **FAIL**(순서) |
| 9 | incense baby lists kept | 마릴: Gen 8 배북 O·흉내쟁이 X, Gen 9 둘 다 O(4조) | PASS | PASS |

- **새 코드 9/9 PASS**, 이전 코드에서는 4개 FAIL(위 표) — 바뀐 규칙을 테스트가 실제로 잡는다는 확인. ASSUME에서 멈춘 테스트 0.
- upstream `test/daycare.c`의 알기술 전수 6개는 HnS 이상해씨 Gen 3 학습표(챌린지 0 = 테스트 기본 세이브)로 **여전히 `ASSUME`에서 멈춘다 — 검증으로 세지 않는다.** 결과는 이전 기준과 같다(바꾸지 않음). 같은 파일의 `Only Pokémon with the same base species can share egg moves (Gen 8)`는 "배우지 않음"만 보는 테스트라 PASS 그대로.
- 공식 게임 알기술 목록과 HnS 목록 차이는 이번 범위가 아니어서 조사하지 않았다(테스트는 HnS 데이터에 있는 조합만 씀).

---

## 2. 트레이너 조우(시선) BGM — HGSS 클래스 대응

### 2.1 기준과 방법

- 기준: pret pokeheartgold `src/field_bgm.c` `sTrainerEncounterMusicParam[][3]` = {클래스, 성도 곡, 관동 곡}. 지역은 `MapHeader_GetRegionNo`(0 성도), 표에 없는 클래스는 기본 `SEQ_GS_EYE_J_SHOUNEN`.
- 곡 대응: J_SHOUNEN=`MUS_HG_ENCOUNTER_BOY_1`, K_SHOUNEN=`BOY_2`, J_SHOUJO=`GIRL_1`, K_SHOUJO=`GIRL_2`, J_AYASHII=`SUSPICIOUS_1`, K_AYASHII=`SUSPICIOUS_2`, BOUZU=`SAGE`, MAIKO=`KIMONO_GIRL`, ROCKET=`ROCKET`.
- HnS 쪽: `src/data/trainers_hns.party`의 모든 HnS 트레이너(651명) `Music`을 표시 클래스(`Class`)로 위 표와 대조. 트레이너에 `Behavior Class`가 있으면 2026-10-10b·10-11 결정("동작 클래스가 돈·볼·BGM·랜더마이저 동작을 보존")대로 그 클래스의 원래 곡이므로 따로 분류. 세션 임시 스크립트로 비교(저장소에 넣지 않음).
- 결과: 맞음 502(성도 과학자 5명은 동작 클래스라 비교 스크립트가 맞음으로 셌다 — 아래 2.4에 따로), **틀림 23(수영팬티 남 — 고침)**, 동작 클래스 보존분 20(질문, 안 고침), 특수·HnS 고유 106(그대로).

### 2.2 클래스별 대응표

| HnS 클래스 | HGSS 기준 | HnS 지정(수) | 판정 |
|---|---|---|---|
| Beauty | GIRL_1 | GIRL_1 ×17 | 맞음 |
| Biker | SUSPICIOUS_2 | SUSPICIOUS_2 ×15 | 맞음 |
| Bird Keeper | BOY_1 | BOY_1 ×17, SUSPICIOUS_2 ×1, SUSPICIOUS_1 ×1 | 2명 동작 클래스(질문), 나머지 맞음 |
| Black Belt | SUSPICIOUS_2 | SUSPICIOUS_2 ×8 | 맞음 |
| Boarder | SUSPICIOUS_2 | BOY_2 ×3 | 3명 모두 동작 클래스(질문) |
| Bug Catcher | BOY_1 | BOY_1 ×23 | 맞음 |
| Burglar | SUSPICIOUS_1 | SUSPICIOUS_1 ×3, SUSPICIOUS_2 ×1 | 1명 동작 클래스(질문), 나머지 맞음 |
| Camper | BOY_1 | BOY_1 ×25 | 맞음 |
| Cooltrainer (여) | GIRL_1 | GIRL_1 ×22 | 맞음 |
| Cooltrainer (남) | SUSPICIOUS_2 | SUSPICIOUS_2 ×20 | 맞음 |
| Double Team | BOY_2 | GIRL_1 ×1 | 동작 클래스(질문) |
| Firebreather | SUSPICIOUS_2 | SUSPICIOUS_2 ×9 | 맞음 |
| Fisherman | SUSPICIOUS_2 | SUSPICIOUS_2 ×27 | 맞음 |
| Gentleman | SUSPICIOUS_2 | SUSPICIOUS_2 ×5 | 맞음 |
| Guitarist | SUSPICIOUS_2 | SUSPICIOUS_2 ×2 | 맞음 |
| Hiker | SUSPICIOUS_2 | SUSPICIOUS_2 ×29 | 맞음 |
| Juggler | SUSPICIOUS_1 | SUSPICIOUS_1 ×7 | 맞음 |
| Kimono Girl | KIMONO_GIRL | KIMONO_GIRL ×8 | 맞음 |
| Lass | GIRL_2 | GIRL_2 ×15 | 맞음 |
| Medium | SAGE | SAGE ×3, GIRL_2 ×1 | 1명 동작 클래스(질문), 나머지 맞음 |
| Officer | BOY_2 | BOY_2 ×3 | 맞음 |
| Picnicker | GIRL_2 | GIRL_2 ×29 | 맞음 |
| Pokefan (여) | GIRL_1 | GIRL_1 ×7 | 맞음 |
| Pokefan (남) | SUSPICIOUS_2 | SUSPICIOUS_2 ×15, SUSPICIOUS_1 ×3, GIRL_1 ×3, BOY_1 ×1 | 7명 동작 클래스(질문), 나머지 맞음 |
| Pokemaniac | SUSPICIOUS_1 | SUSPICIOUS_1 ×15 | 맞음 |
| Psychic M | BOY_1 | BOY_1 ×12 | 맞음 |
| Rocket Admin | ROCKET | ROCKET ×10 | 맞음 |
| Sage | SAGE | SAGE ×11 | 맞음 |
| Sailor | SUSPICIOUS_2 | SUSPICIOUS_2 ×13 | 맞음 |
| School Kid | BOY_1 | BOY_1 ×26 | 맞음 |
| Scientist | 성도 ROCKET / 관동 SUSPICIOUS_1 | SUSPICIOUS_1 ×5 | **5명 모두 성도·동작 클래스 Collector(질문 2)** |
| Skier | GIRL_1 | GIRL_1 ×3 | 맞음 |
| Super Nerd | SUSPICIOUS_1 | SUSPICIOUS_1 ×15 | 맞음 |
| Swimmer F | GIRL_1 | GIRL_1 ×26 | 맞음 |
| **Swimmer M** | **SUSPICIOUS_2** | BOY_2 ×23 → **SUSPICIOUS_2 ×23** | **틀림 → 고침** |
| Teacher | GIRL_1 | SAGE ×2, GIRL_2 ×3 | 5명 모두 동작 클래스(질문) |
| Team Rocket | ROCKET | ROCKET ×36 | 맞음 |
| Twins | GIRL_2 | GIRL_2 ×5 | 맞음 |
| Young Couple | GIRL_1 | GIRL_1 ×1 | 맞음 |
| Youngster | BOY_1 | BOY_1 ×15 | 맞음 |

특수·HnS 고유 클래스(그대로 둠, 이유):

| 클래스 | HnS 지정 | 이유 |
|---|---|---|
| Leader ×31, Leader Kanto ×17, Champion ×4, Elite Four ×12, Professor ×1, Rocket Boss ×1, Elder ×1 | 각 전용곡 또는 BOY_1·SAGE | 관장·챔피언·Elder Li·Giovanni·Eusine·Samson Oak·Steven은 `trainerbattle_no_intro`라 조우곡이 안 나옴. 사천왕·챔피언 전용 음악 보존 |
| Rival ×21 | RIVAL | 라이벌 전용 조우곡 보존 |
| Arena Tycoon·Dome Ace·Factory Head·Palace Maven·Pike Queen·Pyramid King·Salon Maiden | 에메랄드 `MUS_ENCOUNTER_MALE` | 배틀프론티어 브레인 — HGSS 프론티어는 HnS와 구성이 다르고 기존 HnS 지정 유지 |
| Pkmn Trainer 1 ×3, Developer ×1 | 챔피언 배틀곡 | HnS 고유 특수 트레이너 — HnS 의도 유지 |
| Hex Maniac ×4 | SAGE | HnS 고유(HGSS 표 없음). 오컬트 계열이라 기도사(Medium)와 같은 SAGE가 합리적, 맵 배치 없음 |
| Parasol Lady ×1 | GIRL_1 | HnS 고유. 여성 성인 계열(Beauty와 같은 GIRL_1), 맵 배치 없음 |
| Tuber M ×1 | BOY_1 | HnS 고유. 어린 남자아이 계열(Youngster와 같은 BOY_1) |
| Mystery Man ×1 | BOY_1 | HnS 고유. HGSS 표에 없는 클래스의 기본값(J_SHOUNEN)과 같음 |

### 2.3 고친 것 — 수영팬티(남)

- HGSS `TRAINERCLASS_SWIMMER_M` = `SEQ_GS_EYE_K_AYASHII`(성도·관동 모두) = `MUS_HG_ENCOUNTER_SUSPICIOUS_2`. HnS는 `MUS_HG_ENCOUNTER_BOY_2`였다.
- `Music: Hg Boy 2` → `Hg Suspicious 2`: HAROLD, SIMON, RANDALL, CHARLIE, GEORGE, BERKE, KIRK, MATHEW, HAL, PATON, DARYL, WALTER, TONY, JEROME, TUCKER, RICK, CAMERON, SETH, JAMES, LEWIS, PARKER, `MELEMELE_SWIMMER_2`(JIMMY), `AKALA_SWIMMER_3`(GEOFF) — 23명, 모두 동작 클래스 없음. 생성 헤더(`trainers_hns.h`)에서 23명 모두 `TRAINER_ENCOUNTER_MUSIC_HG_SUSPICIOUS_2`로 바뀐 것을 확인.
- 배틀곡·승리곡은 이 필드와 무관 — 바뀌지 않음.

### 2.4 고치지 않은 것 — 동작 클래스(Behavior Class) 보존분(질문 1)

| 트레이너 | 표시 / 동작 클래스 | 지금 | HGSS 표시 클래스 기준 |
|---|---|---|---|
| ROY | Bird Keeper / Biker | SUSPICIOUS_2 | BOY_1 |
| BRET | Bird Keeper / Juggler | SUSPICIOUS_1 | BOY_1 |
| COLETTE, SHIRLEY | Teacher / Expert | SAGE | GIRL_1 |
| HILLARY ×3 | Teacher / Lass | GIRL_2 | GIRL_1 |
| COREY | Burglar / Firebreather | SUSPICIOUS_2 | SUSPICIOUS_1 |
| DORIS | Medium / Psychic | GIRL_2 | SAGE |
| RONALD, BRAD, DOUGLAS | Boarder / Skier | BOY_2 | SUSPICIOUS_2 |
| TREVOR ×3 | Pokefan (남) / Juggler | SUSPICIOUS_1 | SUSPICIOUS_2 |
| ALEX ×3 | Pokefan (남) / Battle Girl | GIRL_1 | SUSPICIOUS_2 |
| ALLAN | Pokefan (남) / Camper | BOY_1 | SUSPICIOUS_2 |
| THOM_AND_KAE | Double Team / Young Couple | GIRL_1 | BOY_2 |
| ROSS, MITCH(로켓단 아지트 B3F), JED(B1F), MARC(라디오타워 3F), RICH(4F) | Scientist / Collector | SUSPICIOUS_1 | 성도 → ROCKET |

10-10b 결정상 지금 곡이 HnS 의도라 그대로 뒀다. HGSS 표시 클래스 기준으로 맞추길 원하면 위 표대로 `Music`만 바꾸면 된다(돈·볼·랜더마이저 동작은 그대로).

---

## 3. 목호 패배 장면 음악

### 3.1 배치

- `PokemonLeague_WillsRoom_EventScript_LanceOnDefeatJohto`·`…Kanto` 둘 다, 문 여는 효과(`setmetatile` → `SE_BANG` → `ShakeCamera` → `special DrawWholeMapView`) **바로 뒤**, 목호 느낌표(`playse SE_PIN`)·이동·`addobject LOCALID_LEAGUE_MARY`/`OAK` **앞**에 `playbgm MUS_HG_ENCOUNTER_GIRL_1, FALSE` 1줄.
- HGSS 원작 `scr_seq_0824_T10R0601`: 문 열림 뒤 `StopBGM 0` + `PlayBGM SEQ_GS_EYE_J_SHOUJO`, 그다음 마리·오박사 등장. HnS `playbgm`은 이전 곡을 끊고 바로 새 곡을 튼다(`PlayNewMapMusic`) — StopBGM + PlayBGM과 같은 결과.
- 그대로 둔 것: 목호 배틀곡·대사·이동·분기(첫 도전 `…Johto` / 챔피언 뒤 재도전 `…Kanto`)·전당 화면 `MUS_HG_HALL_OF_FAME`.

### 3.2 전당 이동 정적 확인

1. `playbgm …, FALSE`는 `savedMusic`에 저장하지 않는다(`ScrCmd_playbgm`, `save == TRUE`일 때만 `Overworld_SetSavedMusic`). 챔피언 방 스크립트에 `savebgm` 없음.
2. 장면 끝 `warpsilent MAP_POKEMON_LEAGUE_HALL_OF_FAME_HNS` → `DoDiveWarp` → `TryFadeOutOldMapMusic`: 현재 곡(`GIRL_1`)이 전당 맵 곡(`MUS_HG_POKEMON_LEAGUE`)과 다르므로 페이드아웃(예외 조건은 소용돌이섬·로켓 점거 곡뿐).
3. 도착 시 `Overworld_PlaySpecialMapMusic`: `savedMusic` 없음 → 전당 맵 곡 `MUS_HG_POKEMON_LEAGUE`. → **`GIRL_1`이 전당까지 남거나 다시 나오지 않는다.**
4. 전당 화면은 `src/hall_of_fame.c` `MUS_HG_HALL_OF_FAME` 그대로.
- 이전과 달라지는 것은 문 열림 ~ 전당 워프 사이 곡뿐(이전: 배틀 뒤 챔피언 방 맵 곡).

---

## 4. 검증

| 단계 | 했나 | 결과 |
|---|---|---|
| 빌드 `make hns -j8` | 함(커밋마다 + 마지막에 바뀐 3파일을 다시 컴파일) | 종료 0, 마지막 로그 새 경고 0(`warncheck.sh`). ROM 32,963,556 → **32,963,636 B**(+80, 코드 `4ec770aa`) |
| 자동 테스트 `GITHUB_ACTION=1 make check BUILD=hns -j8` | 함(코드 `4ec770aa`) | 러너 PASSED 2,797 / TOTAL 5,773(KNOWN_FAILING 12, ASSUMPTIONS_FAILED 45). `test-baseline-seq468.txt` 대비 **새 PASS 9(위 1.6의 9개 전부), 사라진 PASS 0, 다른 줄 없음**. 새 기준 [`test-baseline-seq468b.txt`](1.17.0-port/test-baseline-seq468b.txt)(**다음 비교 기준**). upstream `(Daycare)` 알기술 6개는 ASSUMPTION_FAIL 그대로 — 검증으로 세지 않음 |
| 한글 회귀 kortests(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-b44 4`) | 함(코드 `4ec770aa`) | 558/652, `판정: PASS (기대와 같음)` — 이 작업과 관련된 항목은 없음(바뀌지 않았다는 확인) |
| 세이브 정적 비교(이전 사실 = 코드 `4d9f0306`) | 함 | **PASS(FAIL 0, WARN 0)** — 세이브 구조체 크기·배치 같음 |
| 3절 음악 전환 | 정적 코드 추적만 | 위 3.2 — 실행 확인 아님 |
| 2절 조우 BGM | 생성 헤더 대조만 | 23명 값 확인 — 실행 확인 아님 |
| **mGBA 실기** | **하지 않음(친구)** | — |

실기용 ROM: 기준 커밋 **`4ec770aa`**, `pokehns.gba` SHA1 **`9d319c09a201732f9f05fecf0a0bc8e9503c015c`**, 빌드 환경 클라우드 컨테이너(Linux) `make hns -j8`, `arm-none-eabi-gcc 13.2.1 20231009`. 문서만 바뀐 뒤 커밋도 ROM은 같다.

## 5. mGBA 실기 항목 — 친구

1. 키우미집(HnS 기본 Gen 9): 이상해씨(빈 칸 있음, 흉내허브) + 다른 종 선생 → 선생 기술칸 순서대로 빈 칸에 들어가는지. 허브를 안 지닌 같은 종끼리 → 안 배우는지. 허브가 그대로 남는지.
2. 수영팬티(남) 시선 마주침 곡이 `SUSPICIOUS_2`(괴인/수상한 남자 계열)인지(예: 40·41번 수로, 19번 도로 수영팬티).
3. 목호 격파 → 문이 열리면 `GIRL_1` 시선곡 → 마리·오박사 장면 → 전당 이동 시 곡이 사라지고 리그 곡 → 전당 화면 곡. 첫 도전·챔피언 뒤 재도전 둘 다(가능하면).
