# full-sync 실제 port 결과 — seq 121~126

진행 중: 마지막 완료 seq 126, 다음은 구간 끝 전체 테스트·요약

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

## 동기화 단위: seq 123 #9624 `U-9624` Consolidate decoration values

- 현재 판정: 적용(HnS 적응)
- 커밋: `5fa2bc3746`
- upstream 근거: `1dbf630ad0`(6파일 +787/−989)
- 수정 파일(6): `include/decoration.h`, `src/decoration.c`, `src/data/decoration/header.h`, `src/data/decoration/description.h`(삭제), `src/data/decoration/icon.h`(삭제), `migration_scripts/1.16/consolidate_decorations.py`(신규)
- 적용 방법: 사전 분석 patch(`seq123-9624.patch`, sha1 `1bc51f35…`)를 `git apply --index`로 넣었다. 수정 파일의 모드 표기를 지우고, 삭제 파일 2개의 `deleted file mode`를 저장소의 실제 모드(100755)에 맞춘 사본을 썼다. 충돌 없음.
- 내용:
  - 장식 아이콘(`gDecorIconTable`)과 설명(`DecorDesc_*`)을 `gDecorations[i].icon`·`.description = COMPOUND_STRING(...)`으로 합쳤다. `header.h`는 upstream `1dbf630ad0`과 바이트 동일(sha1 `7a3ed72e…`, 사전 분석에서 HnS 사본에 마이그레이션 스크립트를 돌린 결과와도 같음). 스크립트는 저장소에서 다시 돌리지 않았다.
  - `decoration.c`: `sDecorationMovementInfo`·`sDecorShapeSizes`를 `sDecorShapes[]` 하나로 합치고 shape별 switch/if 3곳을 표 조회로 바꿨다(13 hunk 그대로, 오프셋만 다름). 마이그레이션 스크립트 파일도 upstream과 바이트 동일.
- HnS 적응:
  - `include/decoration.h`: `struct DecorItem`을 HnS `enum DecorationCategory_HnS` 블록의 `#endif` **뒤**, `struct Decoration` 앞에 넣었다(upstream 문맥 `DECORCAT_COUNT, };` 바로 뒤는 HnS에서 `#if` 안이라 비 HnS 빌드에서 빠짐).
  - HnS 추가분 9곳(`#if IS_HNS` 7곳 + 골드/크리스 팔레트 배열·`SpritePalette` 2곳)은 한 줄도 바꾸지 않았다. 확인: `diff(upstream 부모 → HnS 이식 전)`과 `diff(upstream 결과 → 이식 후)`의 `decoration.c` 추가·삭제 줄이 46줄로 **같다**.
- 제외한 hunk: 없음.
- 검증:
  - `git diff --check` 통과. 비 ASCII가 든 `+`/`-` 줄 6개는 옮겨진 영문 설명의 `POKé`(3줄 삭제·3줄 추가)뿐이고, 한글이 든 줄 변경은 0. HnS 장식 이름·설명은 원래 영문이다(`NON_NPC_TEXT_AUDIT.md` 24행의 P1 미번역).
  - **`tmp-123/verify.sh` → `VERIFY OK`:**
    - [1] 소스 5개(`decoration.c`, `decoration.h`, `header.h`, `tiles.h`, `tilemaps.h`)가 사전 분석에서 검증한 트리와 바이트 동일, 삭제 대상 2개 없음
    - [2] 실제 빌드 경로(cpp → preproc + charmap → cc1 → as)로 컴파일한 `gDecorations` **121개 항목의 필드 차이 0**(`id`, `name` 인코딩 바이트, `permission`, `shape`, `category`, `price`, `description` 인코딩 바이트, `tiles` 심볼·내용, 아이콘 `pic`/`pal` 심볼)
    - [3] ID·세이브 상수 비교에서 달라진 것은 `sizeof_Decoration 40`(32 → 40, ROM 표 전용)뿐. `DECOR_*` 121개 값, `DECORCAT_*`·`DECORSHAPE_*`·`DECORPERM_*`, `SaveBlock1/2/3` 크기와 비밀기지·방 장식·교환남·TV 장식 오프셋이 모두 같다(세이브 영향 없음).
  - 빌드(`build/port.log`): 종료 코드 0, **ROM 32,722,788 B(97.52%, −720 B) / EWRAM 248,944 B(0) / IWRAM 25,516 B(0)**. 사전 분석 추정 약 −0.7 KB. `pokehns.gba` SHA1 `366bd56c60aff2f37234927cd063ae6cdf2be3dc`. 새 경고 0(경고 7줄은 `field_specials.c`가 include하는 `battle_frontier_exchange_corner.h`의 기존 미사용 경고).
- 테스트:
  - 지정 파일 `test/script.c`(`checkdecorspace`/`checkdecor` 사용) → PASS 2, seq 120 기준과 같음.
  - L 단위라 전체도 돌렸다(`build/port-check-post123.log`): 러너 요약 PASSED 2,321 / FAILED 2,243 / TOTAL 5,229로 같고, 표준 목록(5,160줄)이 `test-baseline-seq120.txt`와 **바이트 동일**.
- 남은 위험:
  - 낮음: 예전 switch는 shape 0–9 밖이면 아무것도 하지 않았지만 새 코드는 표 밖을 읽는다. shape는 ROM 표에서만 오고 121개 모두 0–9라 도달할 수 없다.
  - 낮음: `rom_header_gf.c`의 `gDecorations` 포인터가 40 B 간격 구조체를 가리킨다(upstream 1.17.0과 같음, HnS는 GF 헤더 연동을 쓰지 않음).
  - 문서: `NON_NPC_TEXT_AUDIT.md` 24행의 `src/data/decoration/{header,description}.h` 표기는 이제 `header.h` 하나다. 문서 수정은 메인 판단에 맡긴다. 번역할 때는 `header.h`의 인라인 `COMPOUND_STRING`을 바로 한글로 바꾸면 된다.
- 실기 확인: 필요(아래 "실기 확인 항목" 3).

## 동기화 단위: seq 124 #9667 `U-cstring-9086` Convert move description variables into COMPOUND_STRINGs

- 현재 판정: 적용(HnS 적응)
- 커밋: `d77ed650ae`
- upstream 근거: `49590a0709`(`src/data/moves_info.h` 1파일 +161/−150). 선행 #9086(`6c019cffe4`)·#9463(`db75d9c0e1`) 적용됨.
- 수정 파일(1): `src/data/moves_info.h`
- 적용 방법: 사전 분석 patch(`seq124-9667.patch`, md5 `e2e7b274…`, HnS 문구로 만든 50 hunk)를 모드 표기만 지워 `git apply`했다. 충돌 없음. upstream diff를 직접 적용하지 않았다(문맥의 기술 이름이 한글이고, 13개 hunk가 upstream 문구를 넣음).
- 내용: 공용 설명 변수 22개와 미사용 `sNullDescription`(및 `sHyperBeamDescription`의 `#else` 판) 정의를 지우고, 사용처 48곳을 `.description = COMPOUND_STRING(...)`으로 인라인했다(파괴광선·기가임팩트·암석포는 `#if B_SKIP_RECHARGE`를 인자 안에 둠). `gNotDoneYetDescription`은 upstream처럼 남겼다. Crunch의 `additionalEffects` `#if` 블록을 삼항식으로 줄이는 정리 hunk도 넣었다(컴파일 시점 상수, 데이터 동일).
- **HnS 적응(upstream과 다른 점):** HnS 원작자 커밋 `384dcb99b8`("Tm desc fixes")가 바꾼 문구 6개를 한 글자도 바꾸지 않고 인라인했다.
  - `"Attack that absorbs\n"`(메가드레인·드레인펀치·우드혼), `"Attack that moves last\n"`(리벤지·눈사태), `"Attack that leaves the\n"`(칼등치기·적당히손봐주기), `"Attack that absorbs over\n"`(드레인키스·데스윙), `"is preparing Attack."`(기습·질풍신뢰), `"Attack that hits foes\n"`(페인트·파워풀에지). upstream은 모두 `"An attack …"`/`"… an attack."`이다.
  - 확인: 이 6개 문구를 upstream 문구로 치환하면 커밋의 `+`/`−` 311줄이 upstream diff와 정렬 비교로 **같다**.
- 제외한 hunk: 없음.
- 검증:
  - `git diff --check` 통과. 비 ASCII가 든 `+`/`−` 줄 0(한글 기술 이름은 문맥 줄에만 있음). 남은 `s…Description` 변수 0(`gNotDoneYetDescription`만 남음).
  - 이식 전 ELF 보관: seq 123 빌드의 `pokehns.elf`(SHA1 `04c7dea8…`)를 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-121-126/tmp-124/pre124.elf`로 복사했다.
  - **`tmp-124/verify_move_text.py cmp pre124.elf pokehns.elf` → `RESULT: OK (0 differences)`** (종료 코드 0). `sizeof=68 moves=935` 전후 같음, 기술 935개의 이름·설명 바이트가 모두 같다. 설명 포인터 932개가 바뀌고 고유 설명 주소가 886 → 883으로 줄었다(메가드레인 문구 = 흡수·비터블레이드, 파괴광선 문구 = 블래스트번 등, 인파이트 문구 = 아머캐논과 병합).
  - 빌드(`build/port.log`): 종료 코드 0, **ROM 32,722,612 B(97.52%, −176 B) / EWRAM 248,944 B(0) / IWRAM 25,516 B(0)**. 사전 분석 추정 약 −160 B(−175 ~ −155 B, compound 섹션 끝 정렬에 따라 달라짐). `pokehns.gba` SHA1 `faa0aa8aab3dd07dacc4aa87b4732b7284d61c8d`. 경고 0줄(새 경고 0).
- 테스트: `test/text.c` → 37줄(PASS 11)이 seq 120 기준 목록의 같은 줄과 **모두 같다**. **`Move descriptions fit on Pokemon Summary Screen: PASS` 유지.** `Move names fit …` 5개 FAIL(한글 이름 폭)은 이식 전과 같다.
- 남은 위험: 없음(바이트 동일 증명). 설명 문자열이 여러 기술 사이에서 같은 주소를 공유하지만 모두 읽기 전용이다.
- 실기 확인: 불필요(바이트 동일). 원하면 요약 화면에서 메가드레인·파괴광선·기습 설명을 한 번 본다.

## 동기화 단위: seq 125 #9616 `U-9616` Gen 5+ Uproar with config

- 현재 판정: 적용(HnS 적응, **메인 결정 A안**)
- 커밋: `673240f6ae`
- upstream 근거: `fdea5d56ee`(15파일 +163/−32), 부모 `813e1c66c0`. 관련 선행 #7714(`2f22780c17`)·#9685(`699315a1ec`)는 HnS에 있음.
- 수정 파일(17): `asm/macros/battle_script.inc`, `data/battle_scripts_1.s`, `include/config/battle.h`, `include/constants/battle_move_effects.h`, `include/constants/config_changes.h`, `src/battle_end_turn.c`, `src/battle_message.c`, `src/battle_move_resolution.c`, `src/battle_script_commands.c`, `src/battle_tv.c`, `src/data/battle_move_effects.h`, `src/data/moves_info.h`, `test/battle/ability/parental_bond.c`, `test/battle/move_effect/uproar.c`, `test/battle/move_effect_secondary/throat_chop.c`, `test/battle/sleep_clause.c`, `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`
- 적용 방법: 사전 분석 patch(`seq125-9616.patch`, md5 `0e6bb849…`, A안)를 모드 표기만 지워 `git apply`했다. `moves_info.h` hunk만 seq 124 때문에 오프셋 −85로 들어갔고 나머지는 그대로. 충돌 없음.
- 내용(upstream):
  - 새 config `B_UPROAR`. `GEN_5` 이상이면 소란 첫 턴 기술 직후 새 `BS_TryWakeBattlersUproar`(`trywakebattlersuproar`)가 잠든 배틀러를 모두 깨우고, 행동 전·턴 종료 기상을 끈다.
  - 지옥찌르기 타이머가 남은 배틀러의 소란을 그 턴 끝에 끝낸다(`HandleEndTurnVarious`, config 무관).
  - `EFFECT_UPROAR` 삭제(소란피기는 `EFFECT_HIT` + self `MOVE_EFFECT_UPROAR`). 배틀 TV 점수는 `battle_tv.c`의 `MOVE_EFFECT_UPROAR` +3으로 보전(1+3 = 이전 4).
  - `BattleScript_TargetWokeUp`의 아이콘 대상 `BS_TARGET` → `BS_EFFECT_BATTLER`, `B_UPROAR_IGNORE_SOUNDPROOF`를 `GetConfig`로 읽음, `config_changes.h` 표 정리(`B_COUNTER_*` 2개 이동 포함).
  - 테스트: `uproar.c` 재작성(4개), `throat_chop.c` 1개 추가, `sleep_clause.c` 1개 수정.
- **HnS 적응(A안):**
  - **`B_UPROAR = GEN_4`**(`// HnS:` 주석). `B_UPROAR`는 세 곳에서 `GEN_5`와 대소 비교만 하므로 소란 기상 규칙(행동 전 `소란스러워서 눈을 떴다!`, 턴 종료 기상)과 문구가 이식 전과 같고 첫 턴 전원 기상은 없다. upstream 기본은 `GEN_LATEST`.
  - **턴 종료 방음 줄 HnS 유지(`// HnS:`):** `hasSoundproof = GetBattlerAbility(gEffectBattler) == ABILITY_SOUNDPROOF`. upstream 줄(`GetConfig(B_UPROAR_IGNORE_SOUNDPROOF) < GEN_5 && …`)을 그대로 넣으면 HnS의 `B_UPROAR_IGNORE_SOUNDPROOF = GEN_LATEST` 때문에 턴 종료에 방음 포켓몬도 깨게 된다.
  - **지옥찌르기 소란 종료 hunk 이식**(출력 변화, 아래).
  - `STRINGID_TARGETWOKEUP` 토큰 교체 `{B_DEF_NAME_WITH_PREFIX}` → `{B_EFF_NAME_WITH_PREFIX}`(한글 본문·조사 그대로). 이 문장은 `BattleScript_TargetWokeUp` 한 곳에서만 나오고, 기존 경로는 잠깨움뺨치기의 `MOVE_EFFECT_REMOVE_STATUS` 수면 분기(`battle_script_commands.c` `SetMoveEffect`) 하나뿐이다. 이 경로는 `gEffectBattler = effectBattler = gBattlerTarget`으로 두고 출력까지 둘을 바꾸지 않으므로 **표시되는 배틀러가 모든 기존 경로에서 같다**(사전 분석 5절 증명, 적용 뒤 사용처 2곳 — 기존 경로와 새 `BS_TryWakeBattlersUproar` — 재확인). 새 경로(`B_UPROAR >= GEN_5`, 테스트에서만 도달)는 EFF여야 올바르다.
  - `asm/macros/battle_script.inc`의 새 매크로와 `battle_script_commands.c`의 새 함수는 HnS 전용 `showitempopup`/`destroyitempopup`, `BS_ShowItemPopup`/`BS_DestroyItemPopup` 뒤(파일 끝)에 붙였다. `B_ABSORB_MESSAGE` 주석의 "No" → "no"는 upstream과 같게.
  - **`test/battle/ability/parental_bond.c`** "Parental Bond does not trigger on Uproar"의 `ASSUME`을 1.17.0형 `MoveHasAdditionalEffectSelf(MOVE_UPROAR, MOVE_EFFECT_UPROAR)`로 바꿨다(HnS에 먼저 들어온 #9685 테스트, 바꾸지 않으면 `EFFECT_UPROAR` 삭제로 `make check` 컴파일 실패).
  - **넣지 않은 것:** `STRINGID_PKMNWOKEUPINUPROAR`의 `{B_ATK_NAME_WITH_PREFIX}` → `{B_EFF_NAME_WITH_PREFIX}`(사전 분석 별건 1). 친구 결정 대기(아래 "결정 대기"). 한글 줄은 `STRINGID_TARGETWOKEUP` 1줄만 바뀌었다.
- 제외한 hunk: 없음(upstream hunk 전부 반영, 턴 종료 방음 줄만 HnS식).
- 출력 변화(`BATTLE_MESSAGE_OUTPUT_CHANGES.md` "기술·필드 상태 효과"에 1행 추가): 소란피기 중인 포켓몬이 지옥찌르기를 맞으면, 이전에는 그 턴 끝 `소란피우고 있다!` → 다음 턴 강제 소란피기가 `지옥찌르기 효과로 기술을 쓸 수 없다!` 뒤 문구 없이 끝났다. 이제 맞은 턴 끝에 `STRINGID_PKMNCALMEDDOWN`(`…은(는)\n얌전해졌다`)으로 끝나고 다음 턴 기술을 자유롭게 고른다(upstream 1.17.0과 같음). 문자열 ID·본문 변화 없음.
- 검증:
  - `git diff --check` 통과. `EFFECT_UPROAR`(`MOVE_EFFECT_UPROAR` 제외) 0곳. `git diff -U0 src/battle_message.c`는 `STRINGID_TARGETWOKEUP` 1쌍뿐. 비 ASCII `+`/`−` 줄은 이 1쌍과 테스트 이름 3줄(`Pokémon`)뿐.
  - 빌드(`build/port.log`): 종료 코드 0, **ROM 32,722,852 B(97.52%, +240 B) / EWRAM 248,944 B(0) / IWRAM 25,516 B(0)**. 사전 분석 추정 약 +0.3~0.5 KB(링크 전). `pokehns.gba` SHA1 `197afe076fa94dccc2579157efc696d93ca3a63b`. 새 경고 0(배틀 헤더 변경으로 경고 163줄이 나왔지만 모두 기준 목록의 기존 경고).
- 테스트(지정 5파일: `move_effect/uproar.c`, `move_effect_secondary/throat_chop.c`, `sleep_clause.c`, `ability/parental_bond.c`, `move_effect_secondary/remove_status.c`):
  - 94줄(PASS 15). 이름이 seq 120 기준에 있는 89줄(PASS 13)은 **모두 같다.**
  - **`Parental Bond does not trigger on Uproar: PASS` 유지.** `throat_chop.c` 기존 PASS 2건 유지.
  - 새 줄 5개: `Uproar status prevents any battler from falling asleep`(GEN_4/GEN_5 PARAMETRIZE) **PASS**, `Uproar doesn't wake up other pokemon on field after first turn (Gen 5+)` **PASS**, `Uproar status causes … before they move except those with Soundproof (Gen 3-4)` FAIL, `… immediately after damage is dealt on the first turn (Gen 5+)` FAIL, `Throat Chop usage causes Uproar to end at the end of the turn` FAIL. FAIL 3건은 모두 `Unmatched MESSAGE`(영문 기대값, 알려진 한계).
  - 사라진 이름 2개(`Uproar status causes sleeping Pokémon to wake up during an attack (2/2)`, `Uproar wakes up other pokemon on field`)는 upstream이 재작성한 옛 테스트이고 seq 120 기준에서 FAIL이었다(회귀 아님).
  - `sleep_clause.c` "…woken up forcefully by Uproar"(이제 `WITH_CONFIG(B_UPROAR, GEN_5)`)는 FAIL 유지, 단독 실행으로 사유가 `Unmatched MESSAGE`뿐임을 확인. `remove_status.c`의 잠깨움뺨치기 FAIL 2건도 토큰 교체 뒤 사유가 `Unmatched MESSAGE`뿐.
  - 그 밖의 FAIL 사유: `Unmatched MESSAGE`와, MESSAGE 불일치로 생기는 `PASSES_RANDOMLY`의 `observed 0.0`(`parental_bond.c` 연속기 4건, `sleep_clause.c` 포자·탈피·치유의마음 10건). 모두 상태가 seq 120과 같다.
- 남은 위험:
  - 낮음: A안이라 1.17.0 기본 동작(5세대 이후 소란)과 config 값이 다르다. seq 475.5 #10151(deps에 #9616)에서 다시 본다.
  - 기존 버그(이번에 고치지 않음): 턴 종료 소란 기상의 `BtlController_EmitSetMonData(gEffectBattler, …, &gBattleMons[gBattlerAttacker].status1)`가 깬 포켓몬 파티 데이터에 소란 사용자의 status1을 보낸다(upstream 1.17.0에도 있음). 기록만 한다.
- 실기 확인: 필요(아래 "실기 확인 항목" 4).

## 동기화 단위: seq 126 #9668 `U-frlggfx-9537` Fix spritesheet_rules.mk for renamed FRLG objects

- 현재 판정: 이미 적용(HnS 동등)
- 커밋: 없음. seq 108 커밋 `70eb6a4271`(#9537)에 포함됐다.
- upstream 근거: `ca828643b7`(`spritesheet_rules.mk` 3줄)
- 근거: 현재 `spritesheet_rules.mk`에 `crush_girl.4bpp`(897행), `black_belt_frlg.4bpp`(909행), `poke_maniac_frlg.4bpp`(1113행) 규칙이 있다. upstream의 오타 `poke_manic_frlg`는 넣지 않았다(HnS는 올바른 이름을 씀, `full-sync-seq-108-119.md` seq 108 항목). 코드 변경 없음.
