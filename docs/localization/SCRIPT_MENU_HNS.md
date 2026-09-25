# HNS 스크립트 메뉴 참조 조사

2026-09-12 기준. `src/data/script_menu.h`의 `gMultichoiceLists` ID와 HNS용 `data/event_scripts.s` 포함 목록을 대조했다. `.if IS_FRLG` 블록은 제외하고 공통 블록 및 `.if IS_HNS` 블록의 `data/maps/*/scripts.inc`, `data/scripts/*.inc`에서 `multichoice`, `multichoicedefault`, `multichoicegrid`의 직접 ID 인수만 센다.

- 메뉴 표의 ID: 178개; HNS 포함 스크립트의 직접 참조: 116개; 그중 HNS 전용 스크립트에서도 참조: 45개.
- 직접 참조가 없는 ID: 62개. 이는 동적 호출이나 C 코드 호출, 다른 포함 파일을 통한 간접 사용까지 배제한 미사용 판정이 아니다.
- 아래의 `HNS 전용`은 전용 블록에서도 호출한다는 뜻이다. `공통`은 공통 블록에서만 직접 호출한다. 어느 쪽도 게임 내 맵 접근 가능성까지 입증하지 않는다.

| 메뉴 ID | 분류 | 직접 호출 수 | 확인할 스크립트 예시 |
| --- | --- | ---: | --- |
| `MULTI_5FLOORS` | HNS 전용 | 1 | `data/maps/GoldenrodCity_DepartmentStoreElevator_hns/scripts.inc:178` |
| `MULTI_7FLOORS` | HNS 전용 | 1 | `data/maps/GoldenrodCity_DepartmentStoreElevator_hns/scripts.inc:44` |
| `MULTI_ACRO_BIKE_INFO` | 공통 | 1 | `data/maps/MauvilleCity_BikeShop/scripts.inc:148` |
| `MULTI_BASE_PC_NO_REGISTRY` | 공통 | 1 | `data/scripts/shared_secret_base.inc:61` |
| `MULTI_BASE_PC_WITH_REGISTRY` | 공통 | 1 | `data/scripts/shared_secret_base.inc:51` |
| `MULTI_BATTLE_ARENA_RULES` | HNS 전용 | 2 | `data/maps/BattleFrontier_BattleArenaLobby_hns/scripts.inc:326` |
| `MULTI_BATTLE_DOME_RULES` | HNS 전용 | 2 | `data/maps/BattleFrontier_BattleDomeLobby_hns/scripts.inc:412` |
| `MULTI_BATTLE_FACTORY_RULES` | HNS 전용 | 2 | `data/maps/BattleFrontier_BattleFactoryLobby_hns/scripts.inc:285` |
| `MULTI_BATTLE_MODE` | 공통 | 2 | `data/scripts/cable_club.inc:287` |
| `MULTI_BATTLE_MODE_HNS` | HNS 전용 | 1 | `data/maps/GoldenrodCity_PokemonCenter_hns/scripts.inc:81` |
| `MULTI_BATTLE_PALACE_RULES` | HNS 전용 | 2 | `data/maps/BattleFrontier_BattlePalaceLobby_hns/scripts.inc:341` |
| `MULTI_BATTLE_PIKE_RULES` | HNS 전용 | 2 | `data/maps/BattleFrontier_BattlePikeLobby_hns/scripts.inc:241` |
| `MULTI_BATTLE_PYRAMID_RULES` | HNS 전용 | 2 | `data/maps/BattleFrontier_BattlePyramidLobby_hns/scripts.inc:498` |
| `MULTI_BATTLE_TOWER_FEELINGS` | HNS 전용 | 2 | `data/maps/BattleFrontier_BattleTowerLobby_hns/scripts.inc:439` |
| `MULTI_BATTLE_TOWER_RULES` | HNS 전용 | 2 | `data/maps/BattleFrontier_BattleTowerLobby_hns/scripts.inc:902` |
| `MULTI_BERRY_PLOT` | 공통 | 1 | `data/scripts/berry_tree.inc:136` |
| `MULTI_BIKE` | 공통 | 1 | `data/maps/MauvilleCity_BikeShop/scripts.inc:24` |
| `MULTI_BRINEY_OFF_DEWFORD` | 공통 | 2 | `data/maps/Route109/scripts.inc:292` |
| `MULTI_BRINEY_ON_DEWFORD` | 공통 | 1 | `data/maps/DewfordTown/scripts.inc:15` |
| `MULTI_CABLE_CLUB_NO_RECORD_MIX` | 공통 | 1 | `data/scripts/cable_club.inc:261` |
| `MULTI_CABLE_CLUB_WITH_RECORD_MIX` | 공통 | 1 | `data/scripts/cable_club.inc:270` |
| `MULTI_CHALLENGEINFO` | HNS 전용 | 26 | `data/maps/SlateportCity_BattleTentLobby_hns/scripts.inc:109` |
| `MULTI_CONTEST_INFO` | 공통 | 1 | `data/scripts/contest_hall.inc:74` |
| `MULTI_CONTEST_RANK` | 공통 | 1 | `data/scripts/contest_hall.inc:118` |
| `MULTI_CONTEST_TYPE` | 공통 | 3 | `data/maps/LilycoveCity_ContestLobby/scripts.inc:667` |
| `MULTI_DAYS_OF_WEEK` | HNS 전용 | 7 | `data/maps/BlackthornCity_hns/scripts.inc:142` |
| `MULTI_ELDERQUIIZ1` | HNS 전용 | 1 | `data/maps/DragonsDen_Shrine_hns/scripts.inc:102` |
| `MULTI_ELDERQUIIZ2` | HNS 전용 | 1 | `data/maps/DragonsDen_Shrine_hns/scripts.inc:116` |
| `MULTI_ELDERQUIIZ3` | HNS 전용 | 1 | `data/maps/DragonsDen_Shrine_hns/scripts.inc:130` |
| `MULTI_ELDERQUIIZ4` | HNS 전용 | 1 | `data/maps/DragonsDen_Shrine_hns/scripts.inc:144` |
| `MULTI_ELDERQUIIZ5` | HNS 전용 | 1 | `data/maps/DragonsDen_Shrine_hns/scripts.inc:158` |
| `MULTI_ENTERINFO` | 공통 | 3 | `data/maps/LilycoveCity_ContestLobby/scripts.inc:629` |
| `MULTI_FALLARBOR_TENT_RULES` | HNS 전용 | 2 | `data/maps/FallarborTown_BattleTentLobby_hns/scripts.inc:266` |
| `MULTI_FLOORS` | 공통 | 5 | `data/maps/LilycoveCity_DepartmentStoreElevator/scripts.inc:25` |
| `MULTI_FOSSIL` | 공통 | 1 | `data/maps/RustboroCity_DevonCorp_2F/scripts.inc:237` |
| `MULTI_FOSSIL_HNS` | HNS 전용 | 1 | `data/maps/RuinsOfAlph_Lab_hns/scripts.inc:565` |
| `MULTI_FRONTIER_GAMBLER_BET` | HNS 전용 | 2 | `data/maps/BattleFrontier_Lounge3_hns/scripts.inc:34` |
| `MULTI_FRONTIER_ITEM_CHOOSE` | HNS 전용 | 2 | `data/maps/BattleFrontier_BattlePyramidLobby_hns/scripts.inc:445` |
| `MULTI_FRONTIER_PASS_INFO` | HNS 전용 | 2 | `data/maps/BattleFrontier_ReceptionGate_hns/scripts.inc:270` |
| `MULTI_FRONTIER_RULES` | HNS 전용 | 2 | `data/maps/BattleFrontier_ReceptionGate_hns/scripts.inc:219` |
| `MULTI_GAME_CORNER_COINS` | 공통 | 5 | `data/maps/MauvilleCity_GameCorner/scripts.inc:36` |
| `MULTI_GAME_CORNER_DOLLS` | 공통 | 1 | `data/maps/MauvilleCity_GameCorner/scripts.inc:275` |
| `MULTI_GAME_CORNER_DOLLS2` | 공통 | 1 | `data/maps/MauvilleCity_GameCorner/scripts.inc:285` |
| `MULTI_GAME_CORNER_TMS` | 공통 | 1 | `data/maps/MauvilleCity_GameCorner/scripts.inc:474` |
| `MULTI_GOLDSILVER` | HNS 전용 | 1 | `data/maps/Route39_hns/scripts.inc:45` |
| `MULTI_GO_ON_RECORD_REST_RETIRE` | HNS 전용 | 18 | `data/maps/BattleFrontier_BattleTowerBattleRoom_hns/scripts.inc:77` |
| `MULTI_GO_ON_RECORD_RETIRE` | HNS 전용 | 2 | `data/maps/BattleFrontier_BattleTowerMultiBattleRoom_hns/scripts.inc:312` |
| `MULTI_GO_ON_REST_RETIRE` | HNS 전용 | 24 | `data/maps/SlateportCity_BattleTentCorridor_hns/scripts.inc:66` |
| `MULTI_GO_ON_RETIRE` | HNS 전용 | 2 | `data/maps/BattleFrontier_BattleTowerMultiBattleRoom_hns/scripts.inc:321` |
| `MULTI_HOENN_STARTERS` | HNS 전용 | 1 | `data/maps/SaffronCity_SilphCo_hns/scripts.inc:18` |
| `MULTI_HOWS_FISHING` | 공통 | 1 | `data/maps/DewfordTown/scripts.inc:98` |
| `MULTI_LEVEL_MODE` | HNS 전용 | 22 | `data/maps/BattleFrontier_BattleTowerLobby_hns/scripts.inc:189` |
| `MULTI_LINK_CONTEST_INFO` | 공통 | 2 | `data/maps/LilycoveCity_ContestLobby/scripts.inc:678` |
| `MULTI_LINK_CONTEST_MODE` | 공통 | 2 | `data/maps/LilycoveCity_ContestLobby/scripts.inc:645` |
| `MULTI_LINK_LEADER` | HNS 전용 | 9 | `data/maps/BattleFrontier_BattleTowerLobby_hns/scripts.inc:854` |
| `MULTI_LINK_SERVICES_HNS` | HNS 전용 | 1 | `data/maps/GoldenrodCity_PokemonCenter_hns/scripts.inc:39` |
| `MULTI_MACH_BIKE_INFO` | 공통 | 1 | `data/maps/MauvilleCity_BikeShop/scripts.inc:110` |
| `MULTI_MECHADOLL1_Q1` | 공통 | 1 | `data/maps/Route110_TrickHousePuzzle5/scripts.inc:308` |
| `MULTI_MECHADOLL1_Q2` | 공통 | 1 | `data/maps/Route110_TrickHousePuzzle5/scripts.inc:316` |
| `MULTI_MECHADOLL1_Q3` | 공통 | 1 | `data/maps/Route110_TrickHousePuzzle5/scripts.inc:324` |
| `MULTI_MECHADOLL2_Q1` | 공통 | 1 | `data/maps/Route110_TrickHousePuzzle5/scripts.inc:332` |
| `MULTI_MECHADOLL2_Q2` | 공통 | 1 | `data/maps/Route110_TrickHousePuzzle5/scripts.inc:340` |
| `MULTI_MECHADOLL2_Q3` | 공통 | 1 | `data/maps/Route110_TrickHousePuzzle5/scripts.inc:348` |
| `MULTI_MECHADOLL3_Q1` | 공통 | 1 | `data/maps/Route110_TrickHousePuzzle5/scripts.inc:356` |
| `MULTI_MECHADOLL3_Q2` | 공통 | 1 | `data/maps/Route110_TrickHousePuzzle5/scripts.inc:364` |
| `MULTI_MECHADOLL3_Q3` | 공통 | 1 | `data/maps/Route110_TrickHousePuzzle5/scripts.inc:372` |
| `MULTI_MECHADOLL4_Q1` | 공통 | 1 | `data/maps/Route110_TrickHousePuzzle5/scripts.inc:380` |
| `MULTI_MECHADOLL4_Q2` | 공통 | 1 | `data/maps/Route110_TrickHousePuzzle5/scripts.inc:388` |
| `MULTI_MECHADOLL4_Q3` | 공통 | 1 | `data/maps/Route110_TrickHousePuzzle5/scripts.inc:396` |
| `MULTI_MECHADOLL5_Q1` | 공통 | 1 | `data/maps/Route110_TrickHousePuzzle5/scripts.inc:404` |
| `MULTI_MECHADOLL5_Q2` | 공통 | 1 | `data/maps/Route110_TrickHousePuzzle5/scripts.inc:412` |
| `MULTI_MECHADOLL5_Q3` | 공통 | 1 | `data/maps/Route110_TrickHousePuzzle5/scripts.inc:420` |
| `MULTI_MOM_MENU` | HNS 전용 | 1 | `data/maps/NewBarkTown_PlayersHouse_1F_hns/scripts.inc:69` |
| `MULTI_OLIVINE_HARBOR` | HNS 전용 | 1 | `data/maps/OlivineCity_PortInside_hns/scripts.inc:122` |
| `MULTI_PRIZE_MONS` | HNS 전용 | 1 | `data/maps/GoldenrodCity_GameCorner_hns/scripts.inc:18` |
| `MULTI_REGISTER_MENU` | 공통 | 1 | `data/scripts/shared_secret_base.inc:100` |
| `MULTI_RIGHTLEFT` | 공통 | 3 | `data/maps/FortreeCity_House2/scripts.inc:10` |
| `MULTI_SATISFACTION` | 공통 | 1 | `data/scripts/interview.inc:252` |
| `MULTI_SHARDS_B` | 공통 | 1 | `data/maps/Route124_DivingTreasureHuntersHouse/scripts.inc:109` |
| `MULTI_SHARDS_BG` | 공통 | 1 | `data/maps/Route124_DivingTreasureHuntersHouse/scripts.inc:181` |
| `MULTI_SHARDS_G` | 공통 | 1 | `data/maps/Route124_DivingTreasureHuntersHouse/scripts.inc:145` |
| `MULTI_SHARDS_R` | 공통 | 1 | `data/maps/Route124_DivingTreasureHuntersHouse/scripts.inc:84` |
| `MULTI_SHARDS_RB` | 공통 | 1 | `data/maps/Route124_DivingTreasureHuntersHouse/scripts.inc:117` |
| `MULTI_SHARDS_RBG` | 공통 | 1 | `data/maps/Route124_DivingTreasureHuntersHouse/scripts.inc:190` |
| `MULTI_SHARDS_RG` | 공통 | 1 | `data/maps/Route124_DivingTreasureHuntersHouse/scripts.inc:153` |
| `MULTI_SHARDS_RY` | 공통 | 1 | `data/maps/Route124_DivingTreasureHuntersHouse/scripts.inc:100` |
| `MULTI_SHARDS_RYB` | 공통 | 1 | `data/maps/Route124_DivingTreasureHuntersHouse/scripts.inc:135` |
| `MULTI_SHARDS_RYBG` | 공통 | 1 | `data/maps/Route124_DivingTreasureHuntersHouse/scripts.inc:210` |
| `MULTI_SHARDS_RYG` | 공통 | 1 | `data/maps/Route124_DivingTreasureHuntersHouse/scripts.inc:171` |
| `MULTI_SHARDS_Y` | 공통 | 1 | `data/maps/Route124_DivingTreasureHuntersHouse/scripts.inc:92` |
| `MULTI_SHARDS_YB` | 공통 | 1 | `data/maps/Route124_DivingTreasureHuntersHouse/scripts.inc:126` |
| `MULTI_SHARDS_YBG` | 공통 | 1 | `data/maps/Route124_DivingTreasureHuntersHouse/scripts.inc:200` |
| `MULTI_SHARDS_YG` | 공통 | 1 | `data/maps/Route124_DivingTreasureHuntersHouse/scripts.inc:162` |
| `MULTI_SLATEPORT_TENT_RULES` | HNS 전용 | 2 | `data/maps/SlateportCity_BattleTentLobby_hns/scripts.inc:244` |
| `MULTI_SSTIDAL_BATTLE_FRONTIER` | 공통 | 1 | `data/maps/BattleFrontier_OutsideWest/scripts.inc:22` |
| `MULTI_SSTIDAL_SLATEPORT_NO_BF` | 공통 | 1 | `data/maps/SlateportCity_Harbor/scripts.inc:174` |
| `MULTI_SSTIDAL_SLATEPORT_WITH_BF` | 공통 | 1 | `data/maps/SlateportCity_Harbor/scripts.inc:182` |
| `MULTI_STATUS_INFO` | 공통 | 1 | `data/maps/RustboroCity_PokemonSchool/scripts.inc:13` |
| `MULTI_STERN_DEEPSEA` | 공통 | 1 | `data/maps/SlateportCity_Harbor/scripts.inc:336` |
| `MULTI_TAG_MATCH_TYPE` | HNS 전용 | 2 | `data/maps/TrainerHill_Entrance_hns/scripts.inc:156` |
| `MULTI_TENT` | 공통 | 2 | `data/maps/Route110_TrickHouseEntrance/scripts.inc:414` |
| `MULTI_TOURNEY_NO_RECORD` | HNS 전용 | 2 | `data/maps/BattleFrontier_BattleDomePreBattleRoom_hns/scripts.inc:44` |
| `MULTI_TOURNEY_WITH_RECORD` | HNS 전용 | 2 | `data/maps/BattleFrontier_BattleDomePreBattleRoom_hns/scripts.inc:33` |
| `MULTI_TV_LATI` | HNS 전용 | 2 | `data/maps/VermilionCity_hns/scripts.inc:483` |
| `MULTI_VENDING_MACHINE` | HNS 전용 | 2 | `data/maps/GoldenrodCity_DepartmentStore_2F_hns/scripts.inc:97` |
| `MULTI_VERMILION_HARBOR` | HNS 전용 | 1 | `data/maps/VermilionCity_PortInside_hns/scripts.inc:41` |
| `MULTI_VIEWED_PAINTINGS` | 공통 | 1 | `data/maps/LilycoveCity_LilycoveMuseum_1F/scripts.inc:13` |
| `MULTI_WHERES_RAYQUAZA` | 공통 | 1 | `data/maps/CaveOfOrigin_B1F/scripts.inc:25` |
| `MULTI_WIRELESS_ALL_SERVICES` | 공통 | 1 | `data/scripts/cable_club.inc:983` |
| `MULTI_WIRELESS_MINIGAME` | 공통 | 2 | `data/scripts/cable_club.inc:1272` |
| `MULTI_WIRELESS_NO_BERRY` | 공통 | 1 | `data/scripts/cable_club.inc:1004` |
| `MULTI_WIRELESS_NO_RECORD` | 공통 | 1 | `data/scripts/cable_club.inc:973` |
| `MULTI_WIRELESS_NO_RECORD_BERRY` | 공통 | 1 | `data/scripts/cable_club.inc:995` |
| `MULTI_YESNO` | HNS 전용 | 50 | `data/maps/SlateportCity_BattleTentCorridor_hns/scripts.inc:82` |
| `MULTI_YESNOINFO` | HNS 전용 | 3 | `data/maps/TrainerHill_Entrance_hns/scripts.inc:142` |
| `MULTI_YESNOINFO_2` | 공통 | 2 | `data/scripts/profile_man.inc:11` |

## 검증 경계

- 이 목록은 소스의 직접 참조 조사다. 실제 ROM 도달 가능성, 조건 분기, 이벤트 플래그 및 화면 표시는 검증하지 않았다.
- `src/script_menu.c`의 PC 메뉴처럼 메뉴 표 밖에서 직접 만드는 항목과 `dynmultichoice`는 이 목록에 포함하지 않았다.
- 다음 작업이 번역이라면 HNS 전용 호출부터 실제 맵·분기 조건을 확인한 뒤 해당 메뉴 문자열을 수정한다. 공통 블록은 HNS 맵의 접근성도 확인한다.
