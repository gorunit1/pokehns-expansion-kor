# full-sync 묶음 83 — seq 518 (#10555 pret 병합 2026-07-30) + seq 545 (#10816 앞당김)

- 요청: 사용자 "ㄱㄱ 518 시작해 혼자해도 되는거라면"(2026-10-29). 진행 중 사용자 "혼자 할 수 있는 작업은 최대한 많이 하고 한 번에 메시지"(친구 문자는 합본으로).
- upstream: 읽기 전용 사본, #10555 병합 커밋 `d5545d0199`(1.16.4, 첫 부모 대비 73파일 +857/−944). 단위 `U-pret-10555`(선행 #10323 = seq 271, #9986 = 516, #9881 = 500 모두 적용됨). #10816 `0a9c697c76`(1.17.1, seq 545).
- 혼자 진행한 근거: pret 쪽 이름 정리·같은 계산 다시 쓰기 위주. 실제로 바뀐 함수를 오브젝트 단위로 모두 뽑아 설명했고(새 도구), 그 과정에서 upstream이 만든 회귀 1개를 찾아 upstream 수정(#10816)을 앞당김.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 518 | #10555 | `6a935eef` | 아래 목록 | 32,980,836(같음) |
| 545(앞당김) | #10816 | `0bf43797` | 레벨이 여러 번 오를 때 다음 기술 배우기가 이어지게(#10555 회귀 수정) | 같음 |

이식 전(`4fdda034`, 게임 코드 = `6c4ca257`) SHA1 `35adc84a…` → 이식 후(`0bf43797`) SHA1 **`78bb1362c2ed20452984cbe8d7bf2d9aee992bf0`**.

## upstream 내용

- 이름 정리: 텍스트 출력 콜백 타입 `TextPrinterCallback`, `Pp` → `PP`(`gText_BoostPP`, `gText_MoveInterfacePPType`, `currentPP`/`maxPP`, `BattleScript_GrudgeTakesPP` 등 12개), `GetDaycareCost` → `GetDaycareCostAndPrepareString`(special), 비교 연산자·`ITEM_NONE` 표기 등.
- 야생 조우 칸 수 상수를 `wild_encounters.json`에서 자동 생성(`include/constants/wild_encounter.h`가 생성 파일이 됨, `LAND_WILD_COUNT` → `NUM_LAND_MONS_ENCOUNTER_SLOTS` 등). 매치콜용 조우 칸 고르기 함수 2개를 `wild_encounter.c`로.
- `DmaCopyLarge`가 남은 크기를 먼저 확인(크기가 블록보다 작을 때 넘쳐 쓰던 UB 수정), `DmaClearLarge` = `DmaFillLarge`(0).
- 같은 계산 다시 쓰기: `fieldmap.c`(맵 불러오기·카메라), `battle_pyramid.c`, `pokenav_list.c`, `hall_of_fame.c`, `item_use.c`(도구 탐지기), 배틀 애니 몇 개, `sprite.c`(OAM 개수 검사), `walda_phrase.c` 등.
- `graphics/title_screen/press_start.png` 타일 순서 + 비HnS 타이틀의 INCGFX 인자.

## HnS 판단

- **충돌 6파일:**
  - `src/battle_message.c`·`src/strings.c`: HnS 한글 문장 블록 안에서 이름만 바뀐 줄 → **한글 문장 그대로, 이름(`gText_MoveInterfacePPType`, `gText_BoostPP`)만 upstream**.
  - `include/menu.h`: HnS가 지운 `AddTextPrinterParameterized6` 선언은 다시 넣지 않고 새 콜백 타입만.
  - `src/sprite.c` 4곳: HnS(= 1.17.1) `AddToOamBuffer` 구조 유지 + upstream OAM 개수 검사 2줄 → 그 구간이 1.17.1과 같음.
  - `src/battle_pyramid.c`: HnS `#if IS_HNS` 피라미드 배치 블록 유지(upstream은 안 쓰는 변수 줄만 지움).
  - `src/item_use.c`: 숨겨진 도구 좌표 비교의 `(u16)` 캐스트를 upstream처럼 지움(음수 좌표에서만 다른데 숨겨진 도구에는 없음). HnS `hiddenItemId == 0` 처리는 그대로.
- **HnS 전용 코드의 옛 이름 3곳:** `wild_encounter.c` 호엔 사운드 조우 2곳(`NUM_*_ENCOUNTER_SLOTS`), `data/maps/Route34_DayCare_hns/scripts.inc`의 `special GetDaycareCostAndPrepareString`.
- **HnS 조우 JSON:** 풀숲 12·물 5·바위깨기 5·낚시 10(낡은/좋은/대단한 낚싯대 2·3·5) — 생성된 상수가 이전 값과 같음(생성 헤더 확인).
- **타이틀:** HnS는 `graphics/title_screen/hns/press_start.png`를 따로 씀 → 이번 그림·인자 변경은 비HnS 빌드에만.
- **upstream 회귀 1개 → #10816 앞당김:** #10555가 `Task_LearnNextMoveOrClosePartyMenu`의 조건을 `gPartyMenu.learnMoveState == 1`에서 `gPartyMenu.data1 == 1`로 바꿈(`data1`은 기술 번호) → 이상한사탕 등으로 한 번에 기술 2개 이상 배울 차례일 때 다음 기술로 넘어가지 않고 메뉴가 닫힐 수 있음. 1.17.1·master·upcoming은 upstream #10816으로 되돌려져 있어 그대로 앞당김(2줄). (확장 계획 3절 표는 #10816을 "HnS 동등, 다시 넣지 않음"으로 적었는데, 그건 #10555 전 HnS 기준 — 518이 같은 줄을 다시 깨서 이제 필요함.) #10555가 넣은 줄 중 1.17.1에 없는 건 이 2줄과 생성 파일로 바뀐 헤더뿐(파일별 대조).
- **실제로 바뀐 함수 전수(새 도구 `port/objcmp.py`, 이식 전 클론 빌드와 오브젝트별 함수 디스어셈블 비교):** 배틀 애니 5개(같은 계산), `battle_controller_player`(이름 + `DmaCopyLarge16`), `battle_message`(이름), `battle_pyramid` 2개, `contest` `CB2_StartContest`(VRAM 지우기 매크로), `event_object_movement` 4개(switch 표 이름·지우기 매크로), `fieldmap` 4개(맵 불러오기·카메라, 같은 계산), `item_use` 2개(도구 탐지기), `overworld` `CB1_OverworldLink`, `party_menu`(위 회귀 → #10816 뒤 이전과 같음), `sprite` 4개(OAM 검사·assert), `walda_phrase` 1개(분기 위치), `wild_encounter`(새 함수 2개·표 이름), `pokemon` 종 정보 getter들(파일 줄이 하나 줄어 assert 줄 번호 상수만 −1). 그 밖 오브젝트는 같음.
- 세이브: 영향 없음(구조체 변경 없음; `SavedMapViewIsEmpty`는 HnS `UBFIX` 쪽 그대로).

## 검증

- **빌드:** `make hns -j8` 종료 0. 저장소 전체 재빌드(`build/hns` 지우고): 종료 0, SHA1 `78bb1362…`(첫 빌드와 같음), 새 컴파일 경고 0(`warncheck.sh`).
- **함수 비교:** 위 "실제로 바뀐 함수 전수".
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `0bf43797`): 목록이 `test-baseline-seq511.txt`와 **바이트 같음**(PASS 3,010).
- **한글 테스트:** 새로 쓰지 않음(문장 0 — 한글 줄은 이름만).
- **세이브 정적 비교**(이전 = 클론 `4fdda034` 빌드): **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s518 8`): **555/652, `판정: PASS (기대와 같음)`, 상태 줄 차이 0**.
- **ROM**(커밋 `0bf43797`): SHA1 `78bb1362c2ed20452984cbe8d7bf2d9aee992bf0`, 사용 32,980,836 B, 파일 33,554,432 B, 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기 후보(친구):** 맵 이동·이어하기(저장한 곳에서 불러오기)·카메라 이동이 이전과 같은지, 도구 탐지기(숨겨진 도구), 34번도로 키우미집 맡긴 포켓몬 찾을 때 비용 문장, 이상한사탕으로 기술 2개 이상 배울 차례(배우기가 이어지는지), 데리고 다니기 포켓몬. 대부분 같은 계산 다시 쓰기라 눈에 띄는 차이가 없어야 함.

## 다음

- seq 519 #10718(피죤 스카이배틀 — HnS 설정 꺼짐), 520 #10731, 521 #10732, 그 뒤 522 #10698(XL, 테스트). 목표 1.17.1.
