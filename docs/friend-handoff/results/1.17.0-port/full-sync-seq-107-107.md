# full-sync 실제 port 결과 — seq 107

완료: seq 107 이식·전체 테스트·기록 완료(다음 구간은 seq 108부터). 아래 "seq 107 요약" 참고. 결과 커밋에 기준 목록 [`test-baseline-seq107.txt`](test-baseline-seq107.txt)와 세이브·블렌더 확인 도구 [`berry-7305/`](berry-7305/README.md)(스크립트와 이식 전 기준 표, 약 290 KB, 바이너리 없음)를 넣었다.

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), [`CLAUDE_FULL_SYNC_PORT_PROMPT.md`](../../CLAUDE_FULL_SYNC_PORT_PROMPT.md), 세이브 방침: [full-sync plan 4절](../pokeemerald-expansion-1.17.0-full-sync-plan.md)(#7305는 기존 열매 번호 36~65 순서를 유지한다)
시작 HEAD: `84fd460dc0` (작업 트리 clean)

## seq 107 요약

| seq | PR | 판정 | 커밋 | ROM 변화 | 비고 |
|---|---|---|---|---:|---|
| 107 | #7305 | 적용(HnS 적응) | `f2a0395e90` | −176 B | `FOREACH_BERRY`를 HnS 아이템 순서로 두어 세이브 열매 번호 유지(`STATIC_ASSERT` 고정), HnS 수확량·`IS_HNS` 나무 코드 유지, 블렌더 NPC 세트 이식 전 동작 유지, `include/random.h` 제외 |

- 마지막 빌드(`f2a0395e90`): 종료 코드 0, **ROM 32,716,084 B(97.50%) / EWRAM 248,924 B(94.96%) / IWRAM 25,516 B(77.87%)**. ROM −176 B, EWRAM·IWRAM 0. 새 경고 0. `pokehns.gba` SHA1 `0c91520caa7dca02ada0115f9657501147c9927a`.
- 세이브 호환(데이터 수준): 열매 번호 68종, 세이브 구조체 레이아웃, 새 게임 나무 118그루, 열매 번호로 찾는 ROM 표가 모두 이식 전과 같다(`verify.sh` 4/4 PASS).
- 전체 테스트: PASS 2,314 / FAIL 2,232 / TOTAL 5,211(이식 전과 같음). 확장·표준 추출 목록 모두 이식 전과 바이트까지 같다. **회귀 0.** 기준 목록 [`test-baseline-seq107.txt`](test-baseline-seq107.txt)(내용은 seq 106과 같음).

## 공통 사항

- 빌드 명령: `PATH=/opt/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin:$PATH GITHUB_ACTION=1 make hns -j8 > build/port.log 2>&1`(노트북 WSL).
- 기준 빌드(`84fd460dc0`, `rm -rf build/hns` 뒤 전체 재빌드, `build/port-pre107-full.log`): 종료 코드 0, ROM 32,716,260 B / EWRAM 248,924 B / IWRAM 25,516 B. `pokehns.gba` SHA1 `e4508999c9a249e926d8a93a5535605ac075f9e1`, `pokehns.elf` SHA1 `da206fba9db98b0a1db887821ec30e9f9b9e6534`(사전 분석 A1이 저장한 기준 ELF와 같음). 컴파일러 경고 163줄, "파일: 메시지"(줄·열 번호 제거) 고유 42개. 링커 RWX 경고 2줄을 넣으면 165줄·43개다. 이 ELF를 스크래치에 복사해 이식 후 비교에 썼다.
- 테스트: 적용 전에 전체를 한 번 돌리고(`build/port-check-pre107.log`, 13분 23초), 적용 뒤 같은 명령으로 다시 돌렸다(`build/port-check-post107.log`, 11분 50초). 명령은 `PATH=… GITHUB_ACTION=1 make check BUILD=hns -j8`. 목록은 `PORT_INSTRUCTIONS`의 `LC_ALL=C`·`grep -a` 추출(표준)과, 사전 분석 A4 2.1절의 확장 정규식(`ASSUMPTION_FAIL|INVALID|TIMEOUT|CRASH` 추가) 두 가지로 만들었다.
- 한글 포함 소스 줄: `git show <커밋> | LC_ALL=C grep -a -cP '^[-+](?![-+]).*[^\x00-\x7F]'`와, 한글 음절(U+AC00~D7A3)만 세는 python 검사.

## 사전 분석 (읽기 전용 분석 에이전트 4개)

이식 전에 분석 에이전트 4개가 저장소를 수정하지 않고 분석했다. 산출물은 세션 스크래치(`/tmp/claude-1000/-home-jinmo-pokehns-expansion-kor/fc35c7ef-06d7-4f6e-94a6-07439474a39e/scratchpad/chunk-107/`)에 있어 **세션이 끝나면 사라진다**. 다시 쓸 도구는 [`berry-7305/`](berry-7305/README.md)로 옮겼다.

| 문서 | 요지 |
|---|---|
| `a1-save-compat.md` (+`a1/`) | 세이브에 열매 번호가 들어가는 곳은 사실상 `SaveBlock1.berryTrees[128].berry`(7비트) 하나다. HnS 아이템 순서로 `FOREACH_BERRY`를 두면 68종 번호가 모두 옛 `ITEM_TO_BERRY` 값과 같다. upstream 순서를 쓰면 36~65번 30종이 바뀐다(새 게임 나무 중에는 Route130 treeId 82가 54→53). 제안 `berries.h`와 검증 스크립트 `verify.sh`(4항목), 세이브 도구 `sav_berry_trees.py`·`sav_set_tree.py`, 이식 전 기준값을 만들었다. 블렌더 NPC 선택이 upstream에서 한 칸 밀린다고 지적했다. |
| `a2-core-c.md` (+`a2-core-c.patch`) | C/헤더/test 34파일 패치(현재 HEAD에 `git apply --check` 통과). 28파일은 upstream 그대로, 6파일은 HnS 적응(`berry.c`의 HnS 수확량 43종·`IS_HNS` 블록, `berry.h`의 `Berry_Ready`, `berry_tag_screen.c` 단위계, `event_object_movement.c` `IS_HNS` 팔레트, 나무 그래픽 표, `berries.h`), `random.h`는 제외. 스크래치 링크로 ROM −224 B(C 부분), 새 경고 0, 문자열 204개 바이트 동일을 확인했다. ELF 비교 스크립트 `verify_berries_elf.py`를 만들었다. |
| `a3-scripts-data.md` (+`a3/`) | 스크립트·데이터 쪽: `new_game.inc` 118줄(호엔 80 + HnS 조토·관동 38), 호엔 맵 5개 랜덤 열매 블록, `asm/macros/event.inc`, 나무 그래픽 표. 이식 전 나무 표(`berry_trees_asm_before.tsv`, 118행 473바이트)와 어셈블·ROM 비교 도구를 만들었다. `setberrytree` 바이트 형식은 그대로이고 `giverandomberry`는 `callnative` 매크로다. |
| `a4-tests-downstream.md` | upstream이 바꾸는 테스트는 ability 6파일(Natural Gift `ASSUME`)이며 HnS에서는 영문 MESSAGE로 원래 FAIL이다. 확장 추출 정규식과 전후 비교 파일 목록, 순서 고정 `STATIC_ASSERT` 예시를 제시했다. 후속 PR(#9735, #8434, #10278, #10181, #10593 등)은 이름만 upstream과 같으면 순서 유지안과 충돌하지 않는다. 블렌더는 "순서 차이로는 같다"고 보았다(아래 블렌더 절에서 실제로 비교함). |

## 동기화 단위: seq 107 #7305 `U-berries-7305` gBerries refactor + untangling berry indices from item IDs

- 현재 판정: 적용(HnS 적응)
- 커밋: `f2a0395e90`
- upstream 근거: `47cac73a61`(부모 `264d99215b` = #8213, seq 106), 42파일 +2956/−2308
- 해결한 의존성: 없음. `include/random.h`의 `RNG_RANDOM_BERRY`는 HnS에 이미 있다(업로드 초기부터, 사용처 0이었음). HnS `berry.c` `#else` 분기의 후속 #9848 형태 `GetBerryTreeAge(berry, stage)`는 그대로 두었다.
- 수정 파일(41, 신규 1):
  - 헤더(9): `include/battle_util.h`, `include/berry.h`, `include/constants/berries.h`(신규), `include/constants/items.h`, `include/event_object_movement.h`, `include/global.berry.h`, `include/item.h`, `include/metaprogram.h`
  - C(22): `src/battle_hold_effects.c`, `src/battle_main.c`, `src/battle_pyramid_bag.c`, `src/battle_script_commands.c`, `src/battle_util.c`, `src/berry.c`, `src/berry_blender.c`, `src/berry_crush.c`, `src/berry_tag_screen.c`, `src/data/object_events/berry_tree_graphics_tables.h`, `src/data/object_events/object_event_graphics_info.h`, `src/debug.c`, `src/dodrio_berry_picking.c`, `src/event_object_movement.c`, `src/item.c`, `src/item_menu.c`, `src/item_menu_icons.c`, `src/scrcmd.c`, `src/script_pokemon_util.c`, `src/wonder_news.c`
  - 스크립트(7): `asm/macros/event.inc`, `data/scripts/new_game.inc`, `data/maps/{LilycoveCity,Route104_PrettyPetalFlowerShop,Route114,Route123_BerryMastersHouse,SootopolisCity}/scripts.inc`
  - 테스트(6): `test/battle/ability/{aerilate,gale_wings,galvanize,normalize,pixilate,refrigerate}.c`
- 커밋을 하나로 둔 이유: `items.h`에서 `ITEM_TO_BERRY`·`NUM_*_BERRIES`를 지우는 것, `asm/macros/event.inc`, 스크립트, 그래픽 표, C 코드가 서로 의존한다. 나누면 중간 커밋의 HnS 빌드가 깨진다.

### 적용 방법

1. 사전 분석 A2 패치(34파일)를 `git apply`로 넣었다. 넣기 전에 패치의 파일별 `+`/`-` 줄 집합을 upstream 커밋과 비교했다. 그대로 적용 28파일은 upstream과 같고, 다른 파일은 `berry.c`·`berry_tag_screen.c`·`event_object_movement.c` 세 개(아래 HnS 적응)뿐이었다.
   - `berry.c`: upstream 이후 파일과의 차이 집합이 "이식 전 upstream 부모와 HnS의 차이 집합"과 같다. 다른 줄은 `ITEM_TO_BERRY(ITEM_LUM_BERRY)/(ITEM_SITRUS_BERRY)` → `BERRY_ID_LUM/SITRUS`, `ITEM_TO_BERRY(FIRST_BERRY_INDEX)` → `BERRY_ID_CHERI` 두 곳이다(값 9·10·1로 같음). 수확량 줄은 따로 비교했다(아래 검증).
   - `berry_tag_screen.c`: HnS 단위계 챌린지 분기가 그대로 남았다. 바뀐 것은 `const struct Berry *berry` → `const struct BerryInfo *berryInfo`와 `ItemIdToBerryType(ITEM_WATMEL_BERRY)` → `BERRY_ID_WATMEL`(33, 같은 값)이다.
2. `include/constants/berries.h`는 A1 제안 파일(목록 + HnS 주석 4줄)로 넣었다. upstream과의 차이는 `F(CHILAN)`·`F(ROSELI)` 두 줄 위치와 주석뿐이다.
3. `asm/macros/event.inc`와 호엔 맵 5개: upstream hunk를 `git apply`로 넣었다(오프셋만 다름). `event.inc`의 HnS 고유 135줄은 그대로다.
4. `data/scripts/new_game.inc`: HnS `#if IS_HNS` 38줄 때문에 upstream hunk가 들어가지 않아 `sed -E 's/ITEM_TO_BERRY\(ITEM_([A-Z0-9_]+)_BERRY\)/BERRY_ID_\1/g'`로 118줄을 바꿨다. upstream 부모 파일에 같은 치환을 하면 upstream 이후 파일과 바이트까지 같다. 따라서 호엔 80줄은 upstream과 같고, HnS 38줄도 같은 규칙으로 바뀌었다.
5. 손으로 넣은 HnS 줄: 블렌더 `SetOpponentsBerryData`(아래 블렌더 절), `berry.c` 순서 고정 가드.
6. 파일 모드는 그대로다(`git diff --summary` 빈 출력).

- 제외한 hunk: `include/random.h`(`RNG_RANDOM_BERRY` 추가). HnS 260행에 이미 있어 넣으면 열거자 중복 오류가 난다.

### upstream과 다르게 둔 곳 (HnS 적응)

| 위치 | upstream | HnS | 이유 |
|---|---|---|---|
| `include/constants/berries.h` `FOREACH_BERRY` | CHILAN을 BABIRI 뒤, ROSELI를 ROWAP 뒤(4세대 번호) | HnS 아이템 ID 순서(CHILAN=36, ROSELI=53). `// HnS:` 주석 | 세이브 `berryTrees[].berry` 번호 유지(plan 4절) |
| `src/berry.c` 파일 머리 | 없음 | `// HnS:` 주석 + `STATIC_ASSERT` 70개: `FOREACH_BERRY`로 67종 `BERRY_ID_X == ITEM_X_BERRY − FIRST_BERRY_INDEX + 1`, 명시 번호 3개(35 BELUE, 36 CHILAN, 37 OCCA, 52 BABIRI / 53 ROSELI, 54 LIECHI, 61 ENIGMA, 65 ROWAP / 66 KEE, 67 MARANGA, 68 E-Reader = `NUM_BERRIES`) | 이후 동기화에서 순서가 바뀌면 컴파일 오류가 난다. ROM 비용 0 |
| `src/berry.c` `gBerries` | upstream 수확량 | HnS 수확량 43종 81필드(`YIELD_RATE` 첫 인자) | HnS 밸런스 유지 |
| `src/berry.c` `IS_HNS` 블록 | 없음 | `sLastPickedBerryType`, `BerryTreeGrow`·`BerryTreeTimeUpdate`·`PlantBerryTree`·`GetStageDurationByBerryType`·`ObjectEventInteractionPlantBerryTree`·`ObjectEventInteractionPickBerryTree` 분기, `Berry_Ready` 유지. 옛 `ITEM_TO_BERRY` 두 곳은 `BERRY_ID_LUM/SITRUS`, `BERRY_ID_CHERI` | HnS 성장·재식재 규칙 |
| `include/berry.h` | 없음 | `#if IS_HNS void Berry_Ready(void); #endif` 유지 | HnS special |
| `src/data/object_events/berry_tree_graphics_tables.h` | 팔레트 슬롯 표 하나 | `#if IS_HNS` 팔레트 슬롯 표(120줄) 유지. `sPicTable_*` → `gPicTable_*`와 포인터 표 삭제는 upstream대로 | HnS 나무 팔레트 |
| `src/event_object_movement.c` `SetBerryTreeGraphicsById` | `#else` 줄만 있음 | `IS_HNS` 분기 `palSlot = gBerries[berryId].berryTreePaletteSlotTable[berryStage] - 2`(1기준 `berryId`), `sHnsBerryPalTags` 유지 | HnS 나무 팔레트. 호출부는 upstream처럼 `- 1` 제거, `> NUM_BERRIES`면 0 |
| `src/berry_tag_screen.c` | 인치 표시만 | HnS 단위계(`challengeSettings.unitSystem`) 분기와 슈박열매 미터법 설명 유지, `BERRY_ID_WATMEL` 사용 | HnS 챌린지 |
| `src/berry_blender.c` `SetOpponentsBerryData` | `opponentSetId = ItemIdToBerryType(id); if (> 5) ((id−1)%5)+5` | `opponentSetId = ItemIdToBerryType(id) - 1; if (>= 5) (id%5)+5`. `// HnS:` 주석 | upstream은 0기준 표를 1기준 번호로 읽어 버치~배리열매에서 한 칸 밀린다(아래 표). 이식 전 동작 유지 |
| `data/scripts/new_game.inc` HnS 38줄 | 없음 | 같은 규칙으로 `BERRY_ID_X` | 조토·관동 나무 유지 |
| `data/maps/GoldenrodCity_FlowerShop_hns/scripts.inc` | (파일 없음) | 바꾸지 않음(`random 8` + `addvar VAR_RESULT, FIRST_BERRY_INDEX`) | 아이템 ID 산술이라 열매 번호와 무관하다. `FIRST_BERRY_INDEX`는 upstream에도 남는다. 바이트·동작이 이식 전과 같다 |
| `include/random.h` | `RNG_RANDOM_BERRY` 추가 | 제외 | 이미 있음 |

`// HnS:` 주석이 붙은 곳은 `include/constants/berries.h`, `src/berry.c`(가드), `src/berry_blender.c` 세 곳이다. 나머지 HnS 차이는 이식 전부터 있던 `#if IS_HNS` 블록이다.

### 그대로 둔 upstream 동작 변화 (세이브 무관, 정상 플레이에서 닿지 않음)

1. **무효 저장값:** 나무 열매 값이 0(단계는 있음)이거나 69~127이면, 이전에는 버치열매로 바꿔 읽었고 이후에는 `BerryTypeToItemId`가 `ITEM_NONE`을 준다. 나무 그림은 버치 표에서 `gBerries[BERRY_ID_NONE]`(두리열매 나무 그림)으로 바뀐다. `GetBerryInfo`는 전후 모두 1로 바꿔 읽는다. `RemoveBerryTree`·`ClearBerryTrees`가 나무를 통째로 0으로 만들므로 정상 플레이·디버그 메뉴로는 이 값이 생기지 않는다.
2. **E-Reader 의문열매의 자연의은혜:** 이전에는 `gNaturalGiftTable[68]`을 표 범위 밖에서 읽었다. 이후에는 `gBerries[68]`의 타입 0·위력 0이다.
3. **자뭉열매(`ITEM_UNUSED_BERRY_1`, ID 897, 쓰지 않는 HnS 아이템):** `FOREACH_BERRY`에 넣지 않았다(넣으면 `NUM_BERRIES`와 E-Reader 번호가 69로 바뀐다). `ItemIdToBerryType`이 0을 돌려주므로 가방 번호 "00", 열매 태그 "?????", 자연의은혜 0/0이 되고, 먹은 열매(`ateBerry`)·불태우기 판정이 주머니 기준으로 바뀌어 이 아이템도 대상이 된다. 스크립트·트레이너·랜덤화 목록 어디에도 없어 입수 경로가 없다. 정식 자뭉열매(SITRUS, 10번)와는 다른 아이템이다.
4. **구버전 ROM과의 통신:** 베리 크래시(`sendCmd[0]`)와 도도리오 열매 따기(`berryResults[][BERRY_PRIZE]`)의 열매 번호가 0기준에서 1기준으로 바뀐다. 이식 전 ROM과 이식 후 ROM을 섞어 통신할 때만 한 칸 어긋난다. 블렌더·교환은 아이템 ID를 보내므로 영향이 없다.
5. **호엔 랜덤 열매 스크립트:** `random N` + `addvar`가 `giverandomberry`(`RandomUniform(RNG_RANDOM_BERRY, lo, hi)`)로 바뀐다. 범위와 분포는 같다. HnS에서는 해당 호엔 일일 플래그(`FLAG_DAILY_LILYCOVE_RECEIVED_BERRY` 등 6개)가 0으로 정의되어 있다. 사전 분석 A3 기준으로 HnS 게임에서 실제로 쓰이는 랜덤 열매 스크립트는 금빛시티 꽃집(바꾸지 않음)뿐이다.
6. **열매 아이콘 팔레트 태그:** `TAG_BERRY_PIC_PAL + berryId`가 30021~30088로 한 칸 옮겨진다. 겹치는 태그가 없다.
7. **열매 변이 표:** upstream `sBerryMutations`에 중복 행이 생기고 변이 열매 이름에 부모 열매를 쓴다. HnS는 `OW_BERRY_MUTATIONS FALSE`이고 `IS_HNS` `PlantBerryTree`가 변이를 쓰지 않아 영향이 없다.
8. **매크로 범위 검사:** `setberrytree`·`giverandomberry`에 어셈블 시 `.error` 검사가 붙는다. 만들어지는 바이트는 같다.

### 세이브·ROM 영향

- 세이브: `SaveBlock1`(15,760 B)/`SaveBlock2`(3,892 B)/`SaveBlock3`(52 B)와 `BerryTree`·`EnigmaBerry` 등 레이아웃이 DWARF 기준 이식 전과 같다. `struct Berry2`는 이름만 `EnigmaBerryInfo`로 바뀌었다. 열매 번호 1~68이 이식 전과 같은 아이템으로 읽힌다. 마이그레이션은 필요 없다.
- ROM −176 B: `gBerries`가 1,904 B → 3,312 B(+1,408 B, 69항목×48 B)가 되고 표 5개가 사라진다(`gNaturalGiftTable`·`gBerryCrush_BerryData`·`gBerryTreePicTablePointers`·`gBerryTreePaletteSlotTablePointers` 각 272 B, `sBerryPicTable` 544 B, 합계 −1,632 B). 표 통합 −224 B에, 인라인 `ItemIdToBerryType`/`BerryTypeToItemId` 호출부·스크립트 크기·정렬 차이 +48 B를 더한 값이다.

### 검증

- `git diff --check`: 통과. 파일 모드 유지.
- `make hns -j8`(`build/port.log`): 종료 코드 0, **ROM 32,716,084 B(−176 B) / EWRAM 248,924 B(0) / IWRAM 25,516 B(0)**. `items.h` 변경으로 C 384개가 다시 컴파일됐다. 경고 163줄·고유 42개(링커 줄 포함 165·43)가 기준과 같다(새 경고 0).
- **세이브 호환 `verify.sh` 4/4 PASS**(`berry-7305/verify.sh <repo> pokehns.elf <out>`, 기준값은 이식 전 `pre/`):
  - (a) 아이템 → 열매 번호 68종과 경계 상수(`BERRY_ID_NONE`=0, `NUM_BERRIES`=`BERRY_ID_ENGIMA_E_READER`=68, `BERRY_ID_ENIGMA`=61, `FIRST/LAST_BERRY_INDEX`=514/581, `BERRY_TREES_COUNT`=128) 같음. 역함수 불일치 0. 무효 저장값 60줄 차이는 예상된 차이(위 1).
  - (b) 세이브 레이아웃 diff 0.
  - (c) 새 게임 나무 118그루(473바이트) 같음, 빌드 ELF 바이트와 일치.
  - (d) 열매 번호로 찾는 ROM 표 68종×29필드(1,972행)가 아이템 기준으로 같음. 차이는 E-Reader 의문열매 자연의은혜 2줄(`OOB` → 0)뿐.
  - 처음 실행에서 (c)가 FAIL(`newgame.log` 빈 파일)이었다. 원인은 스크립트였다. `cpp | awk`에서 awk가 `exit`로 먼저 끝나면 cpp가 EPIPE로 rc=2를 내고 `pipefail`로 스크립트가 멈춘다. 이식 전 파일(HEAD 판 `new_game.inc`)로도 6번 중 2번 같은 실패가 났다. awk가 입력을 끝까지 읽게 고친 뒤 5번 연속 rc=0, 결과 파일 5개가 같았다. 고친 판을 `berry-7305/`에 두었다.
- **나무 표(A3 4절):** `asm_berry_table.sh repo after_port`로 어셈블한 118그루 (treeId, 열매 번호, stage)가 이식 전 `berry_trees_asm_before.tsv`와 같다(SAME). `decode_berry_script.py pokehns.elf --rom pokehns.gba`로 빌드 ROM에서 읽은 표도 같다(SAME). Route130 치리열매 나무(treeId 82)는 54번 그대로다. `giverandomberry` 7곳의 번호 범위(1~10, 1~8, 16~20, 21~30)가 옛 아이템 구간과 같은 열매다. `verify_against_upstream.py --repo-rev 84fd460dc0`: 11파일 모두 PASS(HnS 고유 변경 줄 수가 이식 전후 같다: `new_game.inc` 187, `event.inc` 135, 나무 그래픽 표 120, `object_event_graphics_info.h` 657, `event_object_movement.h` 6, `event_object_movement.c` 471).
- **열매 데이터(ELF):** A2 `verify_berries_elf.py <이식 전 ELF> pokehns.elf` → `checked BerryId 1..68; mismatches = 0`. 이름·설명 문자열 바이트, 나머지 `BerryInfo` 바이트(수확량 포함), 자연의은혜 타입·위력, 크러시 난이도·가루, 열매 그림·팔레트, 나무 그림·팔레트 슬롯 표를 비교한다. A4 `a4_berrydata_cmp.py`(소스 필드 비교)도 `diffs 0`.
- **블렌더 NPC 열매 표:** `berry_blender.c`에서 `NUM_NPC_BERRIES`, `struct BlenderBerry`, `sOpponentBerrySets`, `sBerryMasterBerries`, `SetOpponentsBerryData`를 원문 그대로 잘라 각 시점의 헤더로 호스트 컴파일하는 하네스를 만들었다(`berry-7305/blender/`). 플레이어 열매 68종 × NPC 1~3명 × Blend Master 플래그 2가지(E-Reader는 가장 낮은 맛 5가지) = 432행.
  - 이식 전 vs upstream 그대로: **30행이 다르다.** 플레이어가 버치·유루·복슝·복분·배리열매(1~5번)를 넣을 때만 다르다. 예) NPC 2명, 버치열매: 이식 전 배리·복분열매 → upstream 버치·배리열매(플레이어와 같은 열매). NPC 1명(Blend Master 규칙): 버치열매 → 이식 전 메호키열매, upstream 자야열매. 6번 이상 열매와 E-Reader는 같다. A1·A2의 지적이 맞고, A4의 "같다"는 순서 차이만 본 판단이었다. upstream 1.17.0과 master에도 같은 코드가 남아 있다.
  - HnS 수정 후 vs 이식 전: **432행 모두 같다.** 이 표를 `berry-7305/blender/blender_pre.tsv`로, upstream 그대로일 때의 차이를 `upstream_unfixed.txt`로 두었다.
  - HnS에서는 `FLAG_HIDE_LILYCOVE_CONTEST_HALL_BLEND_MASTER`가 0이라 `FlagGet(0)`이 FALSE다. 따라서 NPC 1명 블렌드는 항상 Blend Master 규칙(메호키~루베 / 토망~노멜열매)을 탄다. 이식 전부터 같은 동작이다.
- **순서 가드 음성 시험:** 가드 줄만 뽑아 호스트 gcc로 컴파일했다. HnS 순서 헤더에서는 통과했다. upstream 순서 `berries.h`로 바꾸면 30종 순서 가드와 명시 번호 가드 2개(`HnsBerryIdSave_35_52`, `HnsBerryIdSave_53_65`)가 오류를 낸다.
- **한글:** 한글이 든 변경 줄 0. 비ASCII 변경 줄 6개는 영문 열매 설명 3줄(위키·리체·마코열매 `description2`의 "Pokémon")이 `.info = { … }` 안으로 들어가며 들여쓰기만 바뀐 것이다. ELF 비교에서 문자열 바이트가 같다. 조사·제어 코드·`STRINGID` 변경 없음.
- **삭제 심볼 잔존:** `ITEM_TO_BERRY`, `gNaturalGiftTable`, `gBerryCrush_BerryData`, `sBerryPicTable`, 나무 포인터 표, `NUM_*_BERRIES`, `struct Berry2`, `struct TypePower`, `sPicTable_*BerryTree`는 주석 2곳 말고 없다. `FIRST_BERRY_INDEX`는 가드와 금빛시티 꽃집만 쓴다.
- **자동 테스트(전체, 이식 전후 같은 명령):**

  | 항목 | 이식 전 | 이식 후 |
  |---|---|---|
  | 요약 | FAILED 2,232 / KNOWN_FAILING 8 / ASSUMPTIONS_FAILED 38 / TO_DO 613 / EXPECT_FAILING 6 / PASSED 2,314 / TOTAL 5,211 | 같음 |
  | 표준 목록(5,142줄) | `test-baseline-seq106.txt`와 바이트 동일 | 이식 전과 바이트 동일 |
  | 확장 목록(5,201줄: PASS 2,311 / FAIL 2,207 / TO_DO 611 / KNOWN_FAILING 8 / EXPECTED_FAIL 5 / ASSUMPTION_FAIL 37 / INVALID 21 / CRASH 1) | — | 이식 전과 바이트 동일 |

  - CRASH 1줄은 러너 자체 시험 `Tests resume after CRASH`다(전후 같음).
  - 바뀐 6개 ability 테스트: 파일 단위로 다시 돌린 결과 실패 사유가 모두 `Unmatched MESSAGE`이고, 실패 줄은 바뀐 `ASSUME`(`gBerries[ItemIdToBerryType(…)].naturalGiftType`) 뒤의 영문 MESSAGE 줄이다(aerilate 115→124, gale_wings 70→76, galvanize 107→115, normalize 222→230, pixilate 86→94, refrigerate 85→93). 파일별 수도 A4 기준과 같다(aerilate PASS 3/FAIL 10, gale_wings FAIL 3, galvanize 4/7, normalize 4/13, pixilate 2/10, refrigerate 2/10). `ASSUMPTION_FAIL`로 바뀐 것은 없다.
  - `Powder doesn't consume Berry from Fire type Natural Gift but prevents using the move`(버치열매 → 불꽃 자연의은혜): PASS 유지. `natural_gift.c` PASS 4 유지.
  - 먹은 열매·불태우기·자연의은혜 경로 테스트(belch, cheek_pouch, sticky_hold, harvest, pickup, magician, unnerve, anticipation 등)도 전체 목록에서 전후 같다.
- 실기 확인: **필요**(아래 "실기 확인 항목").
- 남은 위험:
  - `IS_HNS` 나무 팔레트(`palSlot`)의 1기준 전환은 ROM 표(나무 그림·팔레트 슬롯 표가 열매마다 같은 심볼)로 확인했지만 화면 출력은 실기로만 볼 수 있다.
  - 이후 upstream 동기화가 `include/constants/berries.h`를 upstream 순서로 되돌리면 `berry.c` 가드가 컴파일 오류를 낸다. 그때 가드를 지우지 말고 HnS 순서를 유지한다.

## 커밋 리뷰 (병렬, 읽기 전용)

적용이 끝난 뒤 메인이 리뷰 에이전트 3개를 띄워 `f2a0395e90`을 upstream `47cac73a61`, 1.17.0 최종형, 이식 전 코드, 사전 분석 문서와 대조했다. 리뷰어는 저장소를 수정하지 않았고 `make`를 돌리지 않았다(스크래치에서 하네스만 컴파일).

| 관점 | 판정 | 요지 |
|---|---|---|
| 세이브·열매 번호 순서 | 문제 없음 | 68종 모두 `BERRY_ID_X == 옛 ITEM_TO_BERRY(ITEM_X_BERRY)`(CHILAN 36, ROSELI 53, ENIGMA 61, E-Reader 68)이고 경계 상수가 전후 같다. 아이템↔번호 양방향 차이 0. `STATIC_ASSERT`는 67종 순서와 11개 번호를 고정하며, upstream 순서로 바꾸면 32개가 오류를 낸다. 나무 심기·수확·재식재·그래픽·Wonder News·랜덤 열매 경로에 off-by-one이 없다. `verify.sh` EPIPE 수정은 검사를 약화시키지 않는다. 세이브 구조체는 이름(`Berry`→`BerryInfo`, `Berry2`→`EnigmaBerryInfo`)만 바뀌었다. |
| HnS 동작·C 코드 | 문제 없음 | C/헤더 34파일은 upstream과 줄 집합이 같고, 다른 곳은 결과 문서의 HnS 적응 6곳과 `random.h` 제외뿐이다. 후속 PR 내용은 없다. 옛 `gBerries`·`gNaturalGiftTable`·`gBerryCrush_BerryData`·그림·팔레트 표를 새 필드와 비교해 차이 0(수확량 43종·81필드 포함). IS_HNS `palSlot`은 68종 × 전 단계에서 같은 슬롯을 고른다. 블렌더 `// HnS:` 수정은 이식 전과 모든 조합이 같다(upstream 그대로면 30행 차이). 한글 변경 줄 0. |
| 스크립트·데이터·그래픽·테스트 | 문제 없음 | `setberrytree`·`giverandomberry` 매크로가 upstream과 같고 바이트코드(0x8A + u8×3)가 이식 전과 같다. `new_game.inc` 118줄은 기계 치환 결과와 바이트 동일. ROM에서 랜덤 열매 블록 7곳의 범위가 옛 아이템 구간과 같고, `GoldenrodCity_FlowerShop_hns`는 바이트가 그대로다. `ITEM_TO_BERRY`·`NUM_*_BERRIES` 사용처 0. 나무 그림·팔레트 68종과 HnS `*_hns` 그림 32줄, IS_HNS 팔레트 표가 보존됐다. 테스트 6개가 upstream과 같다. |

리뷰 뒤 메인 조치:
- 도구 README 사소 문제 1건 수정: `TC`만 지정하면 `dwarf_layout.py`가 `READELF`를 찾지 못하던 문제. `layout.sh`가 `READELF=${READELF:-$TC/arm-none-eabi-readelf}`를 export하게 고치고, `/opt`가 아닌 경로를 `TC`로만 줘서 종료 코드 0을 확인했다.
- 실기 목록의 열매 이름을 `src/data/items.h`와 대조해 "수박열매"를 공식 명칭 "슈박열매"로 고쳤다.
- 참고(조치 없음): upstream 변이 코드의 잠재 버그 2개(`sBerryMutations` 중복 행, 변이 열매 이름에 부모 열매 사용)는 HnS에서 `OW_BERRY_MUTATIONS FALSE`라 실행되지 않는다. 변이를 켤 때 고친다.

메인 검증: `rm -f pokehns.elf pokehns.gba` 뒤 `make hns -j8` 종료 코드 0, ROM 32,716,084 B, EWRAM 248,924 B, IWRAM 25,516 B, SHA1 `0c91520caa7dca02ada0115f9657501147c9927a`. 이식 전후 테스트 로그를 메인이 `LC_ALL=C`로 다시 추출해 이식 전 목록 = `test-baseline-seq106.txt`, 이식 후 목록 = `test-baseline-seq107.txt`를 확인했다. docs 밖 한글(U+AC00~D7A3)이 든 변경 줄 0.

## 실기 확인 항목 (친구용)

이식 전 ROM(`84fd460dc0`)으로 만든 세이브를 이식 후 ROM(`f2a0395e90` 이후)에 넣어 본다. 36~65번 열매 나무는 일반 플레이로는 거의 생기지 않으므로 [`berry-7305/README.md`](berry-7305/README.md) 2절의 `sav_set_tree.py`로 세이브 사본을 만들어 쓰면 편하다(예: 31번 도로·도라지시티·37번 도로·42번 도로·33번 도로·고동마을 나무를 카리·오카·로셀·바리비·애터·치리열매로).

1. **기존 세이브의 36~65번 나무:** 카리열매(36), 오카열매(37), 바리비열매(52), 로셀열매(53), 치리열매(54), 의문열매(61), 애터열매(65) 나무의 그림·팔레트, "○○열매가 N개" 문구, 수확 아이템이 이식 전 ROM과 같은지. 에메랄드 130번 수로의 치리열매 나무(treeId 82, 저장값 54)가 있으면 그것도 치리열매로 보여야 한다.
2. **조토·관동 나무 단계별 그림·팔레트:** 30번 도로(오랭열매, 호엔 나무 ID 재사용), 26번 도로·연분홍시티(자뭉열매, 호엔 ID 재사용), 1번 도로(자뭉열매), 31번 도로·도라지시티(버치열매), 고동마을(복슝열매), 46번 도로(리샘열매) 등에서 새싹·성장·열매 단계 그림과 색이 이식 전과 같은지.
3. **수확 뒤 재식재:** 나무에서 열매를 따고 다시 심으면 같은 열매가 새싹 단계로 심기는지(`sLastPickedBerryType`), 시간이 지나 열매 단계가 되는지. 리샘열매·자뭉열매는 다른 열매보다 오래 걸려야 한다.
4. **금빛시티 꽃집:** 하루 한 번 버치~시몬열매 중 하나를 주는지(스크립트는 바꾸지 않았다).
5. **열매 태그 화면:** 가방에서 열매 확인 시 번호(예: 카리열매 No.36, 로셀열매 No.53), 열매 그림·이름·설명, 크기 표시. 단위계 설정이 미터법이면 슈박열매 설명이 미터 문구로 나오는지. 좌우로 다른 열매로 넘길 때 그림이 맞는지.
6. **가방 열매 주머니 번호:** 목록의 번호가 이식 전과 같은지(카리열매 36, 로셀열매 53, 치리열매 54).
7. **자연의은혜:** 버치열매 불꽃 80, 오랭열매 독 80, 자뭉열매 에스퍼 80, 카리열매 노말 80, 로셀열매 페어리 80, 바리비열매 강철 80, 치리열매 풀 100, 애터열매 악 100. 기술이 이 타입·위력으로 나가는지(배틀 메시지는 한글 그대로).
8. **블렌더 NPC:** 해당 맵(예: `LilycoveCity_ContestLobby_hns`)에서 버치~배리열매를 넣었을 때 NPC가 플레이어와 같은 열매를 넣지 않는지. NPC 1명이면 메호키~루베열매 계열을 넣어야 한다.

## 후속 행 메모

- **`include/constants/berries.h`:** 앞으로도 HnS 아이템 순서를 유지한다. upstream 1.17.0까지 이 파일을 바꾸는 후속 커밋은 없다.
- **seq 146 #9735:** 새 `dragonize.c`의 `gBerries[ItemIdToBerryType(ITEM_ORAN_BERRY)].naturalGiftType`는 그대로 들어간다.
- **seq 161 #9168:** 열매 테스트 45파일 기대값이 바뀐다. 그 행에서 기준 목록을 새로 잡는다.
- **seq 175 #8434:** `event_object_movement.c` 문맥이 `sPicTable_PechaBerryTree` 전방 선언 삭제를 전제로 한다(이번에 upstream대로 지웠다). `SetBerryTreeGraphicsById`의 `IS_HNS` 분기는 유지한다.
- **seq 251 #10278, seq 490 #10593:** 불태우기 판정 `GetItemPocket(...) == POCKET_BERRIES` 줄을 upstream 형태로 넣었으므로 문맥이 맞는다.
- **seq 381 #10181:** `SetOpponentsBerryData(enum Item …)`, `enum BerryId opponentBerryId`, `sBerryMasterBerries`를 `enum BerryId` 배열로 바꾼다. hunk가 HnS 수정 줄(`opponentSetId = … - 1`)에 닿지 않는다. 이식 뒤 `berry-7305/blender/run.sh`로 표가 `blender_pre.tsv`와 같은지 본다. `global.berry.h`의 `EnigmaBerry`·`BattleEnigmaBerry` `holdEffect`를 `enum HoldEffect :8`로 바꾸는 hunk는 비트필드라 세이브 레이아웃을 `verify.sh` (b)로 확인한다.
- **seq 373 #10152:** `PrintBlendingResults`의 쓰지 않는 `berryIds` 삭제. 문제없다.
- **seq 401 #10179:** `GetObjectEventBerryTreeIdByLocalIdAndMap` 등 삭제. HnS에서도 호출이 없다.
- **seq 500~501 INCGFX:** `gBerryPic_*` 심볼 이름은 그대로다.
- **열매 이름·설명 한글화(별도 작업):** `gBerries[].info.name`은 `BERRY_NAME_LENGTH`(6바이트)라 한글은 3글자까지다. `EnigmaBerryInfo.name[7]`은 세이브 구조다.
