# 조사 2026-10-30 — 페어리 설정 테스트 영향(89(1) 후속) · 노가드×프리폴 HnS 경로(34)

- 사용자 요청: 친구 후속 요청(2) 보고 뒤 "승인 없이 할 수 있는 조사만" 두 건. **코드·테스트 기대값·설정 변경 없음.**
- 이 작업환경(클라우드 컨테이너)에서만 함. 실기 없음.

## 1. 테스트 세이브의 페어리 타입 설정 때문에 PASS가 안 되는 테스트

89(1) 원인(테스트 세이브의 `challengeSettings.tx_Mode_Fairy_Types` = 0 → 페어리 없음)이 다른 테스트에도 있는지 확인.

- **방법:** 테스트 러너의 `ClearSaveBlocks`(`test/test_runner.c`)에 `tx_Mode_Fairy_Types = 1`(새 게임 기본값, `src/new_game.c` 147행) 한 줄을 **임시로** 넣고 전체 테스트(`GITHUB_ACTION=1 make check BUILD=hns -j8`). 끝난 뒤 백업으로 원래 파일을 되돌렸고, `git status` 깨끗한 것을 확인함(커밋 없음). 비교 기준 `test-baseline-f1030b.txt` + 같은 기준 실행 로그(검증 f1030b)의 `INVALID`·`ASSUMPTION_FAIL` 줄(기준 목록 파일에는 안 들어가는 상태).
- **결과 요약:** 새 PASS **14**, 사라진 PASS **0**. 새로 돌아가지만 FAIL인 것 16(아래 C).

### A. 기준 목록의 FAIL → PASS (6)

| 테스트 | 기준 |
|---|---|
| `AI sees Dragon Darts damage redirecting if one target is immune` (89(1)) | 1/2 FAIL |
| `(Z-MOVE) Z-Nature Power transforms into different Z-Moves based on the current terrain` | 4/4 FAIL |
| `Aura Break inverts Fairy Aura's effect` | FAIL |
| `Fairy Aura increases the power of all Fairy-type attacks by 33%` | FAIL |
| `Trace doesn't try to reactivate if activation was prevented by Ability Shield` | FAIL |
| `Type-enhancing items increase the base power of moves by 20%` | 36/36 FAIL |

### B. 기준 실행에서 INVALID·ASSUMPTION_FAIL(목록에 없음) → PASS (8)

- INVALID → PASS: `Dazzling, Queenly Majesty and Armor Tail do not block a move's Z-Status effect`, `Z-Moves animations work`(기준 17/37 INVALID).
- ASSUMPTION_FAIL → PASS: `AI won't use status moves if the player's best attacking move is Focus Punch`, `AI_FLAG_SMART_SWITCHING: AI will stay in if Encore'd into super effective move`, `Dragon Rage is unaffected by type immunities (Gen 1)`, `Inverse battle reverses type matchups`, `Misty Terrain does not increase the power of Fairy-type moves`, `Weakness berries decrease the base power of moves by half`(기준 34/36).

### C. ASSUMPTION_FAIL → 실행되지만 FAIL (16)

- `Roost …` 15개(1개는 `KNOWN_FAILING`), `Pickpocket checks contact/effect per target for spread moves` 1개.
- 원인: 파일 공통 전제에 페어리 기술 타입(`GetMoveType(MOVE_DISARMING_VOICE) == TYPE_FAIRY` 등)이 있어 기준에선 전제 실패로 건너뛰었던 것. 페어리를 켜면 실행되고, **영문 `MESSAGE` 대조**에서 실패(예: `roost.c:38 Unmatched MESSAGE("Wobbuffet's HP is full!")`) — HnS의 다른 영문 문장 FAIL과 같은 종류. 게임 동작 문제 아님.

### 해석·제안(적용 안 함)

- 위 14개는 HnS 게임 코드가 아니라 **테스트 세이브 설정(페어리 없음)** 때문에 PASS가 아니었음. 새 게임 기본(페어리 있음)으로 돌리면 PASS.
- 테스트 쪽에서 맞추려면 `ClearSaveBlocks`에 `gSaveBlock3Ptr->challengeSettings.tx_Mode_Fairy_Types = 1;` 한 줄(같은 함수의 `tx_Challenges_OneTypeChallenge` 처리와 같은 꼴). 효과: 새 PASS 14 / 사라진 PASS 0, 새로 실행되는 FAIL 16(영문 문장). **친구 결정 대기.** 적용하면 기준 목록을 새로 만들고, 한글 회귀(kortests — 이번 조사에선 안 돌림)도 함께 확인해야 함.
- 반대 방향(페어리 없음 모드)을 확인하는 HnS 테스트는 지금 없음 — 원하면 별도 제안.

## 2. 노가드 × 프리폴(34) — HnS에서 실제로 닿는 경로

34의 hunk는 "프리폴이 아닌 기술로, 프리폴로 떠 있는 포켓몬(사용자 또는 들린 쪽)을 노가드가 관여해 칠 때 명중 판정을 할지"만 바꿈. 그 장면은 **더블 배틀**에서만 생김. 데이터 대조(스크립트, 저장소 밖 `scratchpad`에서 실행)로 HnS 경로를 찾음.

### 판정 규칙(코드 확인)

- HnS 트레이너 포켓몬: 특성 지정이 없으면 **1번 슬롯만**(`B_TRAINER_MON_RANDOM_ABILITY 0`, `src/trainer_util.c`). 기술 지정이 없으면 **레벨업 기본 기술**(`GiveMonInitialMoveset`, 레벨업 표 `P_LVL_UP_LEARNSETS GEN_7`).
- 프리폴을 레벨업으로 배우는 종(GEN_7 표): 피죤 40, 피죤투 41, 프테라 49, 수리둥보·워글 50, 루차불 55. 기술 가르침: 울라울라섬 가르침(`data/maps/UlaulaIsle_hns/scripts.inc` 17번 항목).
- 노가드가 1번 슬롯인 종: 단칼빙·쌍검킬, 메가피죤투·메가루차불·메가라이츄Y. 알통몬 계열은 2번 슬롯, 골비람·골루그·딱정곤·루가루암(한밤중)은 숨겨진 특성.

### 결과

| 경로 | 프리폴 | 노가드 | 더블 가능? | 판단 |
|---|---|---|---|---|
| HnS 트레이너(`trainers_hns.party` 651명) | 피죤투(기술 미지정 → 기본 기술에 프리폴) **8명**: 새 조련사 하늘(`TRAINER_HANK_HNS`, Route 4), 새 조련사 영진 첫 대전·재대전 2·3(`JOSE`, Route 27), 짧은 치마 미나(`LAURA`, Route 25), 엘리트 트레이너 용기(`JAKE`, Route 26), 새 조련사 소홍 재대전 2·3(`VANCE`, Route 44) | 특성 직접 지정 2명: 캠프보이 `TRAINER_AKALA_CAVE_CAMPER_HNS`(이름 `RONNY`, 근육몬 Lv61), 그린 `TRAINER_BLUE_POSTOBC_HNS`(괴력몬 Lv70) | 10명 모두 **싱글**. 더블 트레이너 9명 중엔 프리폴·노가드 없음 | 아래 "동시 발견" 1건만 |
| 동시 발견 더블(두 트레이너가 한 번에 발견) | 위 피죤투 트레이너 | — | 시야 겹침(지형·벽 무시한 기하 계산): 영진·미나·용기·소홍은 **겹치는 트레이너 없음**. 하늘(Route 4)은 처음 자리엔 겹침이 없지만 **돌아다니기(범위 x 7·y 4)** 라 자리에 따라 같은 길의 피크닉걸 민지(`HOPE`)·미정(`SHARON`)과 시야가 겹칠 수 있음 | 하늘만 이론상 가능(실제 지형으로 생기는지는 미확인) |
| 파트너 배틀(`battle_partners.party` 6명) | 없음 | 없음 | 더블 | 해당 없음 |
| 프런티어 시설 포켓몬 | 없음(데이터에 프리폴 없음) | 근육몬 2·괴력몬 8 — 특성·성별 지정이 없어 성격값 0 + 성격 보정(0x100 단위) → 특성 슬롯 0 = **근성**(노가드 아님). 배틀팩토리 대여도 같은 생성 | 더블 시설 있음 | 상대 쪽엔 없음. **플레이어가** 프리폴·노가드를 가져가면 해당 |
| 배틀피라미드 야생 괴력몬 | — | `ABILITY_RANDOM` → 노가드 가능 | 피라미드 야생은 싱글 | 해당 없음 |
| 야생 더블 | — | — | `WE_DOUBLE_WILD_CHANCE 0`, 강제 플래그 0, 동행 야생 배틀 꺼짐 | 해당 없음 |
| 트레이너 힐(`TrainerHill_*_hns`) | 없음(`trainer_hill.h`에 프리폴 없음) | 괴력몬·근육몬 각 1 — `abilityNum = 0` = **근성** | 층에 따라 더블 있음 | 상대 쪽엔 없음(플레이어 쪽 경로만) |
| 배틀텐트(`SlateportCity`·`FallarborTown`·`VerdanturfTown_BattleTent*_hns` 지도 있음) | 없음 | 근육몬 3 — 특성 지정 없음(프런티어 시설 포켓몬과 같은 판정이면 근성, 생성 경로는 이번에 따로 확인 안 함) | 텐트 배틀은 싱글 | 해당 없음 |

### 결론

- **상대(AI) 쪽에서 프리폴과 노가드가 한 배틀에 같이 나오는 HnS 데이터는 없음.** 유일한 예외 후보는 Route 4 하늘(피죤투 프리폴)이 다른 트레이너와 동시 발견 더블이 되는 경우 + 플레이어 쪽 노가드 포켓몬.
- **현실적인 경로는 플레이어 쪽:** 플레이어가 구할 수 있는 알통몬 계열(야생 — 2번 특성 노가드)과 피죤투(레벨업 프리폴)·울라울라 가르침 프리폴을 **둘 다 더블 배틀에 데리고 갈 때.** 예: 플레이어 피죤투가 상대를 들어 올린 턴에 플레이어 노가드 괴력몬이 명중 100 미만 기술(크로스촙 80, 폭발펀치 50 등)로 들린 상대를 침 → 지금 HnS는 항상 명중, hunk 적용 시 명중 판정(34 보고서의 경우 2). 더블은 HnS 더블 트레이너 9명·동시 발견 더블(기하 계산으로 시야가 겹치는 트레이너 쌍 92개, `_hns` 지도 전체)·파트너 배틀·프런티어 더블 시설·트레이너 힐 더블. 알통몬 계열 야생 출현은 `wild_encounters.json`에 알통몬 71·근육몬 56칸(2번 특성이 노가드라 특성은 개체마다 다름).
- 랜덤마이저(트레이너 종 교체)를 켜면 상대 쪽 조합도 생길 수 있음 — 이번 조사 범위 밖.
- 이 조사는 데이터·코드 대조만. 실기·자동 테스트 아님.
