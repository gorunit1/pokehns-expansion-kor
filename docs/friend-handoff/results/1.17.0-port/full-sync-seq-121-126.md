# full-sync 실제 port 결과 — seq 121~126

진행 중: 마지막 완료 seq 122, 다음 seq 123 (#9624)

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), [`CLAUDE_FULL_SYNC_PORT_PROMPT.md`](../../CLAUDE_FULL_SYNC_PORT_PROMPT.md), 메시지 출력 기록: [`BATTLE_MESSAGE_OUTPUT_CHANGES.md`](../../../localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md)
시작 HEAD: `5afec6304c` (작업 트리 clean. `aa175914e9`(seq 120 코드) 뒤로는 docs만 바뀜)

## 공통 사항

- 툴체인: 이 컴퓨터의 기본 PATH `arm-none-eabi-gcc` 13.2.1(`/usr/bin`). PATH 접두어 없이 빌드했다.
- 빌드 명령: `GITHUB_ACTION=1 make hns -j8 > build/port.log 2>&1`
- 이식 전 기준: `aa175914e9` 빌드(작업 트리의 `pokehns.gba` SHA1 `2ef9b346a463da48dc2a7f41f12d58ca4a619b53`, seq 120 결과와 같음) ROM 32,719,060 B / EWRAM 248,940 B / IWRAM 25,516 B.
- 경고 비교: `build/port-base-full.log`(전체 재빌드 로그)의 경고를 "파일: 메시지"(줄·열 번호 제거) 고유 42개로 만들고, 매 빌드의 경고를 같은 형식으로 만들어 이 목록에 없는 것을 "새 경고"로 셌다.
- 테스트 명령: 파일별 `GITHUB_ACTION=1 make check BUILD=hns -j6 TESTS="<파일>"`. 파일마다 따로 돌려 `PORT_INSTRUCTIONS`의 `LC_ALL=C`·`grep -a` 추출로 목록을 만들고, 같은 테스트 이름의 [`test-baseline-seq120.txt`](test-baseline-seq120.txt) 줄과 비교했다.
- 사전 분석: 읽기 전용 분석 에이전트가 PR별 이식 계획·적응 patch·검증 도구를 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-121-126/`에 만들었다(`seq<SEQ>-<PR>.md`/`.patch`, `tmp-<seq>/`). 적용 직전에 patch를 `git apply --check`로 다시 확인했다. patch의 `index … 100644` 모드 표기는 지운 사본을 썼다(HnS의 대상 파일 다수가 100755라서 그대로 적용하면 모드가 바뀜). 커밋에 기존 파일 모드 변경은 없다.

## 동기화 단위: seq 121 #9594 `U-spots-9594` refactor(graphics): make spinda spots generic (+ seq 163 #9796)

- 현재 판정: 적용(HnS 적응)
- 커밋: `0e6b9c22ec`
- upstream 근거: #9594 `bb1961348a`, 같은 unit의 #9796 `663c6bf3ae`("Add Spot Coord Limits", seq 163). 관련 #10247은 `!isEgg`만 `6480a48924`로 이미 들어가 있었다.
- 수정 파일(5): `include/pokemon.h`, `include/pokemon_spots.h`(신규), `src/decompress.c`, `src/pokemon.c`, `src/pokemon_spots.c`(신규)
- 적용 방법: 사전 분석 권장 patch(`seq121-9594.patch`, md5 `7c20111f…`)를 모드 표기만 지워 `git apply`했다. 충돌 없음. 결과 `src/pokemon_spots.c`는 분석의 목표 파일(`tmp-121/b/`)과 바이트 동일, `include/pokemon_spots.h`는 upstream `bb1961348a`와 바이트 동일.
- 내용:
  - `DrawSpindaSpots`·`gSpindaSpotGraphics`·`struct SpindaSpot`·`SPINDA_SPOT_*`를 지우고, 일반 spot 시스템 `pokemon_spots.c`(`ShouldDrawSpotsOnSpecies`, `DrawPokemonSpotsBothFrames`)로 옮겼다.
  - `LoadSpecialPokePicIsEgg`는 `ShouldDrawSpotsOnSpecies(species) && isFrontPic && !isEgg`에서 `DrawPokemonSpotsBothFrames`를 부른다.
  - **#9796을 같은 커밋에 넣었다.** `DrawPokemonSpots`의 `x`/`y`/`row`/`col`을 `s32`로 하고 버퍼 경계 검사를 추가했다. upstream `663c6bf3ae` 판과 비교하면 아래 "upstream과 다른 점" 3줄만 다르다. #9594만 넣으면 personality `& 0xF0 == 0`(약 1/16)인 얼루기에서 1프레임 첫 점이 통째로(12줄) 그림 버퍼 밖에 그려진다(#9796 PR 본문의 도감 목록 크래시와 같은 증상).
- **upstream과 다른 점:**
  - **spot_2 y = 25 유지(원작 3세대 값 유지).** upstream #9594가 설명 없이 27로 바꿨다. pokeemerald·pokefirered가 25이고, HnS 얼루기 그래픽은 upstream과 바이트까지 같아 27을 쓸 근거가 없다. 해당 줄에 `// HnS: keep vanilla Emerald/FRLG y = 25 (upstream #9594 changed it to 27)` 주석을 달았다. 선택 patch `seq121-9594-y27-optional.patch`는 쓰지 않았다.
  - `!isEgg` 유지(#10247). upstream #9594의 decompress hunk에는 없다.
  - 점 이미지는 `INCBIN_U32("…/spot_N.1bpp")` 그대로(HnS는 아직 INCGFX가 없음, `graphics_file_rules.mk`의 `-plain -data_width 2` 규칙 사용). seq 500 #9881에서 INCGFX로 바뀐다.
  - 공백 2곳 정리: 주석 23행 끝 공백, `case SPECIES_SPINDA: ` 끝 공백(#10247의 남은 hunk와 같음). `git diff --check` 통과용.
- 제외한 hunk: `docs/SUMMARY.md`, `docs/tutorials/how_to_spots.md`(튜토리얼 문서, 게임과 무관. #9557 선례).
- 검증:
  - `git diff --check` 통과. 한글이 든 줄 변경 0, config·세이브 구조체 변경 0.
  - **네이티브 비교(사전 분석 하니스 재실행):** 이식 전 `src/pokemon.c`(HEAD `5afec6304c`)의 `DrawSpindaSpots` 본문과 커밋된 `src/pokemon_spots.c` 본문을 그대로 x86에서 컴파일해 실제 `anim_front.png` 버퍼로 비교했다(하니스는 스크래치 사본에서 재생성, 생성된 비교 코드가 분석 때와 같음). 결과 `cases 2131072: on-sprite mismatches 0; OOB writes: old 129196, new 0`. 즉 personality 2,131,072개(바이트쌍 전수 2×65,536 + 무작위 200만)에서 그림 안 픽셀 차이 0, 버퍼 밖 쓰기는 이식 전 129,196건(원작에도 있는 1행 버그)에서 0건이 됐다.
  - 빌드(`build/port.log`): 종료 코드 0, **ROM 32,719,428 B(97.51%, +368 B) / EWRAM 248,940 B(0) / IWRAM 25,516 B(0)**. 사전 분석 추정 약 +362 B(정렬 전). `pokehns.gba` SHA1 `b53ea1d99902604b60af0ffaeb77e24ca6d8798c`.
  - 새 경고 0(`pokemon.h` 변경으로 넓게 다시 컴파일됐고 경고 163줄이 나왔지만 모두 기준 목록에 있는 기존 경고).
  - `pokehns.map`: `DrawSpindaSpots`·`gSpindaSpotGraphics` 0개, `src/pokemon_spots.o`의 `DrawPokemonSpots`·`DrawPokemonSpotsBothFrames`·`ShouldDrawSpotsOnSpecies`·`sSpindaSpots`·`sSpindaSpotImages`·`gSpindaSpotTemplate` 있음.
- 테스트(사전 분석 지정 스모크, `OPPONENT(SPECIES_SPINDA)` 포함): `test/battle/ability/contrary.c`, `test/battle/ability/opportunist.c` → 24줄(PASS 12)이 seq 120 기준 목록의 같은 줄과 **모두 같다**. 실패 10건은 모두 `Unmatched MESSAGE`(알려진 한계), 크래시·assert 없음. 이 PR·#9796은 `test/**`를 바꾸지 않는다.
- 남은 위험: 낮음. `GetSpotRow`의 `default: errorf`는 얼루기(SCALE_2 고정)로는 닿지 않는다. 링크 순서가 바뀌어 뒤쪽 주소가 이동하지만 세이브에 코드 주소를 저장하지 않는다.
- 실기 확인: 필요(아래 "실기 확인 항목" 1).

## 동기화 단위: seq 122 #9425 `U-shop-9425` feat (shopMenu): conditional item appearances

- 현재 판정: 적용(HnS 적응)
- 커밋: `2397ef4e08`
- upstream 근거: `02943cb4fc`(11파일 +305/−2, docs 3파일 포함)
- 수정 파일(8): `asm/macros/event.inc`, `data/event_scripts.s`, `data/text/mart_clerk.inc` → `data/scripts/mart_clerk.inc`(이름 변경 + 끝 10줄), `include/item.h`, `include/shop_criteria.h`(신규), `src/item.c`, `src/shop.c`, `src/shop_criteria.c`(신규)
- 적용 방법: 사전 분석 patch(`seq122-9425.patch`, md5 `e9179255…`)를 모드 표기만 지워 `git apply`했다. 충돌 없음. `src/shop_criteria.c`·`include/shop_criteria.h`·`data/scripts/mart_clerk.inc`는 upstream `02943cb4fc`와 바이트 동일(옮긴 `mart_clerk.inc`는 기존 모드 100755 유지, 기존 10줄의 영문 3문장은 그대로).
- 내용:
  - `struct ItemInfo` 끝에 `ShopCriteriaFunc shopCriteriaFunc`를 추가하고, `GetItemShopCriteriaFunc`·`IsItemShopCriteriaFulfilled`를 `item.c`에 넣었다.
  - 일반 마트(`MART_TYPE_NORMAL`) 구매 메뉴를 열 때 `TryBuildDynamicShopItemList`가 조건을 만족하는 품목만 힙 배열로 복사하고, 닫을 때 `TryFreeDynamicShopItemList`가 해제·복원한다.
  - `pokemart` 매크로 기본 인자 `Pokemart_DefaultItemList`(7품목, `data/scripts/mart_clerk.inc` 끝).
- **HnS 적응:**
  - **`pokemart 0` NULL 분기 유지:** HnS `SetShopItemsForSale`은 `items == NULL`을 배지 수 기반 목록(`sShopInventories[badgeCount]`, PC 챌린지면 `sShopInventories_PC`)으로 쓴다. 체리그로브 마트(`CherrygroveCity_Mart_hns:39`)·도라지 마트(`VioletCity_Mart_hns:9`)·트레이너힐 입구(`TrainerHill_Entrance_hns:265`) 3곳이 쓴다. upstream assertf(`items != NULL`, 함수 첫머리)를 그대로 넣으면 이 3곳에서 크래시 화면이 뜬다. 그래서 assertf를 HnS가 목록을 정한 **뒤**에 두고 조건을 `sMartInfo.itemList != NULL`로 바꿨다(`// HnS:` 주석). HnS 빌드에서는 실패하지 않는다.
  - `CB2_InitBuyMenu` case 0과 `BuyMenuFreeMemory`: HnS 나무열매 아이콘 줄 때문에 문맥이 달라 같은 의미 위치에 넣었다(Build는 `BuyMenuBuildListMenuTemplate()` 바로 앞, Free는 `RemoveBerryIcon()` 앞). Kurt·BP·장식 상점은 `MART_TYPE_NORMAL`이 아니라 Build/Free를 타지 않는다(정적 목록 `sKurtBallShopItems`·`sBPItemList`를 `Free`하지 않음).
  - upstream의 `// Read items until ITEM_NONE / DECOR_NONE is reached` 주석은 HnS에 원래 없는 문맥 줄이라 넣지 않았다.
- 제외한 hunk: `docs/SUMMARY.md`, `docs/tutorials/how_to_dynamic_shop.md`, `docs/tutorials/img/dynamic_shop/showcase.gif`(404 KB). #9557 선례, 게임과 무관.
- 검증:
  - `git diff --check` 통과. 한글이 든 줄 변경 0. 인자 없는 `pokemart` 호출 0곳이라 매크로 기본값이 기존 스크립트 바이트를 바꾸지 않는다.
  - 빌드(`build/port.log`): 종료 코드 0, **ROM 32,723,508 B(97.52%, +4,080 B) / EWRAM 248,944 B(94.96%, +4 B) / IWRAM 25,516 B(0)**. 사전 분석 추정 약 +4.2 KB·EWRAM +4 B. 대부분 `gItemsInfo` 44 B → 48 B × 901(+3,604 B). `pokehns.gba` SHA1 `70ccae6b9731d125693f4652991348f9bcee4a9b`. 새 경고 0.
  - `arm-none-eabi-nm -S`: `gItemsInfo` 크기 `0xa8f0`(48 × 901), `sDynamicShopItemListRef` 4 B(EWRAM `0x0203a570`), `Pokemart_DefaultItemList` `0x0832bfcc`(짝수, HnS에서는 쓰지 않음).
  - ROM에서 `gItemsInfo` 901개 항목의 새 필드(오프셋 44)가 **모두 0**(`901 True`). 즉 조건 함수가 붙은 품목이 없어 동적 목록은 원래 목록과 같은 사본이고, 모든 상점의 판매 목록·순서·가격이 이식 전과 같다.
- 테스트:
  - 지정 파일 `test/bag.c`, `test/script.c`, `test/save.c` → 9줄(PASS 3)이 seq 120 기준 목록의 같은 줄과 **모두 같다**. FAIL 6은 기존 FAIL(가방 정렬 개수, 세이브 구조체 크기 기대값: HnS 세이브 구조가 upstream과 다름)이고 값도 이식 전과 같다(`SaveBlock1` 15760 등).
  - L 단위라 전체도 돌렸다(`build/port-check-post122.log`, 4분 5초): 러너 요약 PASSED 2,321 / FAILED 2,243 / TOTAL 5,229로 seq 120과 같고, 표준 목록(5,160줄)이 `test-baseline-seq120.txt`와 **바이트 동일**.
- 남은 위험:
  - 낮음(upstream 잠재 버그, 1.17.1까지 그대로): `TryFreeDynamicShopItemList`는 목록 포인터만 되돌리고 `sMartInfo.itemCount`는 되돌리지 않는다. 걸러지는 품목이 생기면 같은 상점에서 두 번째 구매 메뉴가 원래 목록의 앞 N개만 검사한다. 지금 HnS에는 조건 함수가 없어 드러나지 않는다. HnS가 이 기능을 쓰기 전에 고쳐야 한다.
  - 문서: `docs/localization/NON_NPC_TEXT_AUDIT.md` 17행의 `data/text/{…,mart_clerk,…}.inc` 경로가 옛 경로가 됐다. 문서 수정은 메인 판단에 맡긴다(이번에 고치지 않음).
- 실기 확인: 필요(아래 "실기 확인 항목" 2).
