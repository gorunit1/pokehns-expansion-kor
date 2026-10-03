# full-sync 실제 port 결과 — seq 128 (#9475 트레이너 그림 정보 리팩터)

완료: seq 128 unit `U-trainerpic-9475` 이식·그림 데이터 비교·전체 테스트·커밋 리뷰(병렬 3개, 수정 필요 0)·기록 완료. 다음 seq는 **129 #9674**다. 선진행으로 넣은 seq 130·131·133·134·137·143·148·149·151·156·157·158·160·165는 닿으면 "이미 적용(선진행)"으로 처리한다.

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), group plan: [`g6_overworld_refactors_plan.tsv`](../1.17.0-sync-plan/g6_overworld_refactors_plan.tsv)
시작 HEAD: `ea90cbff3a`(작업 트리 clean, 코드는 `fdc110d528`과 같음). 작업 컴퓨터: 데스크탑(`/usr/bin` 툴체인).

## 요약

| seq | PR | 판정 | 커밋 | ROM 변화 | 비고 |
|---|---|---|---|---:|---|
| 128 | #9475 | 적용(HnS 적응) | `06c6bac8c2` | +160 B | `TrainerPicID`의 앞/뒤 구분을 없애고 `gTrainerSprites`·`gTrainerBacksprites`를 `gTrainerPicInfo`(213칸) 하나로 합쳤다. HnS 앞모습 55개·뒷모습 4개를 같은 그림·팔레트로 옮겼다. HnS 플레이어 분기는 `GetPlayerTrainerPic`(`src/trainer.c`) 한 곳으로 모았다. 녹화 배틀 재생의 플레이어 뒷모습은 Brendan/May를 유지했다(`#if IS_HNS`, 친구 확인 대상) |

- 빌드(작업 트리 = 커밋 내용): 종료 코드 0, **ROM 32,715,956 B(97.50%, +160 B) / EWRAM 248,936 B(94.96%, 0) / IWRAM 25,516 B(77.87%, 0)**. `pokehns.gba` SHA1 `4b3b96bf8f924f4da6f1bb464ca8e48ebc789cec`.
  - ROM 내역(map): 옛 표 `gTrainerSprites` 6,720 B + `gTrainerBacksprites` 5,376 B가 빠지고 `gTrainerPicInfo` 1,704 B와 앞·뒤 정보 구조체가 들어왔다. upstream none 그림(뒷모습 2,048 B, 앞모습 400 B, 팔레트 32 B)이 추가됐다. getter·assertf 호출 등 코드 변화까지 합쳐 +160 B.
- 새 경고 0(경고 163줄이 모두 `warn-base.txt` 42개 안에 있음).
- 한글이 든 소스 줄 변경 0(diff의 `+`/`-` 줄에 비 ASCII 0). `battle_partners.party`는 `Back Pic:` 6줄만 지웠고 이름·대사 줄은 그대로다.
- 세이브: 영향 없음. `TrainerPicID` 숫자 값은 모두 바뀌었지만 세이브·통신·녹화 배틀에 저장되지 않는다. 저장되는 시설 클래스 enum(명시 값)은 그대로다(사전 분석 C 4절). EWRAM 0 B 변화.
- 그림 데이터 비교 도구: **`RESULT: 차이 0 (OK)`**, 종료 코드 0(아래 "그림 데이터 비교").
- 생성 헤더 7개: 사전 분석 A의 스크래치 생성 결과와 바이트 동일.
- 전체 테스트: PASSED 2,335 / FAILED 2,255 / KNOWN_FAILING 10 / TOTAL 5,253(seq 127과 같음). 표준 추출 목록 5,184줄이 [`test-baseline-seq127.txt`](test-baseline-seq127.txt)와 **바이트 단위로 같다**(사라진 PASS 0, 새 PASS 0). 새 기준 목록 [`test-baseline-seq128.txt`](test-baseline-seq128.txt)(seq127과 같은 내용).

## 공통 사항

- 툴체인: 데스크탑 기본 PATH `arm-none-eabi-gcc` 13.2.1(`/usr/bin`). PATH 접두어 없이 빌드했다.
- 빌드 명령: `GITHUB_ACTION=1 make hns -j8 > build/port.log 2>&1`
- 이식 전 기준(데스크탑, `fdc110d528` 코드): ROM 32,715,796 B / EWRAM 248,936 B / IWRAM 25,516 B, SHA1 `817f500dbbb7a808274d30d284eb00b5e304a607`(작업 트리의 `pokehns.gba`로 확인).
- 경고 비교: `LC_ALL=C grep -a 'warning:' build/port.log | sed -E 's/:[0-9]+:[0-9]+: /: /' | sort -u | comm -13 warn-base.txt -`. 기준은 앞 구간과 같은 "파일: 메시지" 고유 42개 목록(저장소 밖 `/home/hjm0725/hns-sync-work/chunk-ahead-130-167/warn-base.txt`)이다.
- 사전 분석: 읽기 전용 분석 4개가 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-128/`에 patch와 검증 도구를 만들었다.
  - A(`part-A.md`/`.patch`, 10파일): 상수·자료 구조·그림 표·시설 클래스 표·trainerproc·party·`src/trainer.c`
  - B(`part-B.md`/`.patch`, 14파일): 배틀 컨트롤러와 배틀 그림 호출부
  - C(`part-C.md`/`.patch`, 8파일): 배틀 밖 호출부(머그샷·필드 효과·포켓기어 전화·트레이너 그림 공용 함수), 세이브·통신 저장 여부, Emerald·FRLG 컴파일
  - D(`part-D.md`, `verify/trainer_pic_verify.py`): 이식 전후 ELF의 그림 데이터 내용 비교 도구
  - 세 patch는 파일이 겹치지 않는다. 각 patch에 `index`·모드 줄이 없어서 그대로 `git apply --check` → `git apply`했다(A → B → C). 기존 파일의 모드 변경은 없다.

## 동기화 단위: seq 128 #9475 `U-trainerpic-9475` Refactor/trainer pic info

- 현재 판정: 적용(HnS 적응)
- 커밋: `06c6bac8c2`
- upstream 근거: `becfa70a97`(37파일 +1,242/−648)
- 수정 파일(35): 기존 30 + 새 파일 5
  - 상수·구조·표: `include/constants/trainers.h`, `include/data.h`, `include/trainer.h`(신규), `src/trainer.c`(신규), `src/data/graphics/trainers.h`, `src/data/pokemon/trainer_class_lookups.h`
  - 도구·데이터: `tools/trainerproc/main.c`, `src/data/battle_partners.party`, `test/battle/partner_control.party`, `migration_scripts/1.9/convert_partner_parties.py`
  - 그림(신규, upstream blob 그대로): `graphics/trainers/front_pics/none.png`(`ceb74c2e8f`), `graphics/trainers/back_pics/none.png`(`22cbdc68a1`), `graphics/trainers/palettes/none.pal`(`681f1cefe9`)
  - 배틀: `include/battle_gfx_sfx_util.h`, `src/battle_controller_{link_partner,oak_old_man,opponent,player,player_partner,recorded_opponent,recorded_partner,recorded_player,safari,wally}.c`, `src/battle_controllers.c`, `src/battle_gfx_sfx_util.c`, `src/reshow_battle_screen.c`
  - 배틀 밖: `include/decompress.h`, `include/field_effect.h`, `src/battle_transition.c`, `src/decompress.c`, `src/field_effect.c`, `src/pokemon.c`, `src/pokenav_match_call_gfx.c`, `src/trainer_pokemon_sprites.c`
- 넣지 않은 upstream 파일 2개: `migration_scripts/1.13/convert_trainers.py`, `migration_scripts/1.9/convert_trainer_parties.py`. HnS 파일이 이미 upstream `becfa70a97`·1.17.0과 같은 blob이다(이미 동등).
- 적용 방법: `git apply` A → B → C, upstream none 그림 3개를 `git -C <upstream> show becfa70a97:<경로>`로 복사. 충돌 없음. 커밋 diff 35파일 +1,557/−798.
  - 커밋된 none 그림 3개의 blob은 upstream과 같다(`git ls-files -s`). `none.pal`은 `.gitattributes`의 `*.pal text eol=crlf` 대상이라, 작업 트리 사본(`git show` 출력, LF)은 다음 checkout 때 CRLF로 바뀐다. 저장소 내용과 빌드 결과에는 영향이 없다.
  - upstream과 바이트가 같은 파일: `include/data.h`, `include/decompress.h`, `include/trainer.h`, `tools/trainerproc/main.c`, `test/battle/partner_control.party`, `migration_scripts/1.9/convert_partner_parties.py`, 배틀 컨트롤러 7개(`link_partner`, `oak_old_man`, `opponent`, `player_partner`, `recorded_opponent`, `recorded_partner`, `wally`), `src/pokenav_match_call_gfx.c`, none 그림 3개.
  - 나머지 파일은 HnS 고유 줄이 원래 있던 파일이다. upstream hunk를 넣고 HnS 줄은 그대로 두었다.
- 내용:
  - `enum TrainerPicID`에서 `TRAINER_PIC_FRONT_`/`TRAINER_PIC_BACK_` 접두를 없애고 같은 인물의 앞·뒤를 한 ID로 합쳤다. 새 값 `TRAINER_PIC_NONE = 0`이 생겼다. `TRAINER_PIC_COUNT` = 213(upstream 158 + HnS 55, packed u8).
  - 표 `gTrainerPicInfo[TRAINER_PIC_COUNT]`(`struct TrainerPicInfo { frontPic, backPic }`)와 getter(`GetTrainerFrontPicData/Palette/MugshotCoords/MugshotRotation`, `GetTrainerBackPicCoords/Image/Anims/Palette`, `Sanitize*TrainerPic`의 assertf). 팔레트 태그는 `GetTrainerPicTag(id, isFront)`: 앞 = ID(0~212), 뒤 = 213 + ID(213~425).
  - `struct Trainer`의 `trainerBackPic` 필드와 party `Back Pic:` 키, `GetTrainerBackPicFromId`, `DecompressPicFromTable`을 지웠다. 파트너 뒷모습은 `trainerPic` ID의 `backPic`을 쓴다.
  - 플레이어 그림: `GetPlayerTrainerPic(gender, version)`(`src/trainer.c`).
- **HnS 적응:**
  - `include/constants/trainers.h`: upstream 새 목록 158줄 뒤, `TRAINER_PIC_COUNT` 앞에 HnS 55개를 기존 순서대로 넣었다(`TRAINER_PIC_FRONT_X_HNS` → `TRAINER_PIC_X_HNS`). 뒷모습 4개는 같은 인물 ID에 합쳤다: `BACK_GOLD_HNS` → `GOLD_HNS`, `BACK_KRIS_HNS` → `KRIS_HNS`, `BACK_SILVER_HNS` → `SILVER_HNS`, `BACK_LANCE_HNS` → `CHAMPION_LANCE_HNS`. `TRAINER_BACK_PIC_PLAYER_MALE/FEMALE` 매크로(HnS `IS_HNS` 분기)는 지웠다. 시설 클래스 enum(명시 값, 세이브 저장)은 손대지 않았다.
  - `src/data/graphics/trainers.h`: HnS 앞모습 INCBIN 55쌍, 뒷모습 INCBIN 4개, 뒷모습 전용 팔레트 4개(`gTrainerBackPicPalette_{Gold,Kris,Silver,Lance}Hns`)를 옮기고 `gTrainerPicInfo`에 HnS 55항목을 넣었다. GOLD·KRIS·SILVER·CHAMPION_LANCE에는 `.backPic = TRAINER_BACK_PIC(4, …, sBackAnims_Hoenn)`을 붙였다. 이 4명은 앞모습과 뒷모습 팔레트 바이트가 달라서 뒷모습 전용 팔레트를 유지했다. INCBIN 경로는 HEAD 줄 그대로다(2026-09-16 복구 자산 포함, 사무엘 오박사 팔레트의 `front_pics/samson_oak_hns.gbapal` 경로도 그대로).
  - `trainer_class_lookups.h`: HnS 20줄 포함 이름만 바꿨다. `FACILITY_CLASS_CHAMPION_STEVEN_HNS = TRAINER_PIC_STEVEN` 유지.
  - `src/data/battle_partners.party`: HnS에 `Back Pic:` 줄이 6개 있어(Brendan·Steven·목호·실버 3) 모두 지웠다. 앞모습 `Pic:`이 각각 같은 인물이라 뒷모습이 그대로다.
  - **`src/trainer.c` `GetPlayerTrainerPic`의 HnS 분기**(`// HnS:`): `IS_HNS`이면 버전과 관계없이 남 → `TRAINER_PIC_GOLD_HNS`, 여 → `TRAINER_PIC_KRIS_HNS`. 이식 전 `LinkPlayerGetTrainerPicId`의 `#if IS_HNS gender + TRAINER_PIC_BACK_GOLD_HNS`와 `TRAINER_BACK_PIC_PLAYER_*` 결과와 같다. 새 enum에서 GOLD(172)·KRIS(175)가 이웃하지 않으므로 `gender + ID` 계산은 하나도 남기지 않았다(도구 [6] BAD 0).
  - `battle_controller_player.c` `LinkPlayerGetTrainerPicId`: HnS `#if IS_HNS` 블록을 지우고 upstream처럼 `GetPlayerTrainerPic(gender, version)`을 반환한다(주석 1줄).
  - 멀티 테스트 경로 2곳(`battle_controller_player.c`, `battle_controller_recorded_player.c`의 `IsMultibattleTest()`): upstream은 `TRAINER_PIC_BRENDAN`이지만 HnS가 Gold로 고쳐 둔 줄을 지켜 `GetPlayerTrainerPic(MALE, GAME_VERSION)`으로 했다(메인 결정, `// HnS:`). 테스트 빌드 전용이다.
  - **`battle_controller_recorded_player.c` 녹화 재생 그리기**(메인 결정): 이식 전 HnS는 이 줄만 `gender + TRAINER_PIC_BACK_BRENDAN`으로 Brendan/May를 그렸다. upstream 새 코드(`GetPlayerTrainerPic`)를 그대로 쓰면 Gold/Kris로 바뀐다. 이번에는 표시를 바꾸지 않도록 `#if IS_HNS (gender == MALE) ? TRAINER_PIC_BRENDAN : TRAINER_PIC_MAY` / `#else` upstream으로 두었다(`// HnS:` 주석). Gold/Kris로 바꿀지는 아래 "친구에게 물을 것" 1.
  - `include/field_effect.h`: HnS `#if IS_HNS AddNewGameOakObject` 3줄 때문에 문맥만 맞췄다. `src/battle_gfx_sfx_util.c`(HnS Gen4 체력바 UI +196줄)·`include/battle_gfx_sfx_util.h`(HnS 선언 3줄)·`src/battle_controllers.c`(`fastIntro` 분기)는 HnS 줄을 건드리지 않고 upstream hunk만 넣었다(사전 분석 patch가 HnS 문맥에 맞춰 둠).
  - `migration_scripts/1.9/convert_partner_parties.py`: HnS는 #8789 이전 정규식(`TRAINER_BACK_PIC_(\w+)`)이었다. upstream 1.17.0 최종값과 같게 맞췄다(빌드와 무관). 이식 전 HnS 값은 HnS 이력에 있는 upstream #9749(`dbce4df2f6`)가 되돌린 값이고, 1.17.0 값은 1.16.0 release 병합(`d52631c8a9`)에서 왔다(리뷰 A 참고).
  - 그대로 둔 HnS 코드: `PlayerGenderToFrontTrainerPicId`의 `IS_HNS` 분기(시설 클래스 GOLD/KRIS_HNS), `PlayerGenderToFrontTrainerPicId_Debug`, `AddNewGameOakObject`, `GetUnionRoomTrainerPic`, `GetSecretBaseTrainerPicIndex`.
- 제외한 hunk: 없음(이미 동등한 migration 스크립트 2개만 넣지 않음).
- 검증:
  - `git diff --check` 통과. 새 파일 `include/trainer.h`·`src/trainer.c`도 줄끝 공백·비 ASCII 0.
  - 옛 이름 잔존: `git grep -nE "TRAINER_PIC_FRONT_|TRAINER_PIC_BACK_|TRAINER_BACK_PIC_PLAYER_|gTrainerSprites|gTrainerBacksprites|trainerBackPic|GetTrainerBackPicFromId|DecompressPicFromTable|TRAINER_PIC_(FRONT|BACK)_COUNT|Back Pic:" -- src include test tools data` 0건. `docs/tutorials/how_to_trainer_{front,back}_pic.md`·`ai_dynamic_functions.md`의 옛 이름은 upstream 1.17.0도 그대로라 두었다.
  - 빌드(`build/port.log`): 종료 코드 0, ROM 32,715,956 B(+160 B) / EWRAM 248,936 B(0) / IWRAM 25,516 B(0), 새 경고 0. 빌드가 새 trainerproc로 party 7개를 다시 만들고 none 그림 3개를 일반 규칙(`%.4bpp: %.png`, `%.gbapal: %.pal`, `%.smol`)으로 변환했다.
  - 그림 데이터 비교(`verify/trainer_pic_verify.py run`): 차이 0(아래 절).
  - 생성 헤더(`.gitignore` 대상, 커밋 안 함): 빌드가 만든 `src/data/{battle_partners,trainers,trainers_frlg,trainers_hns,debug_trainers}.h`와 `test/battle/{trainer,partner}_control.h` 7개가 사전 분석 A의 스크래치 생성 결과(`tmp-A/gen/new/`)와 `cmp`로 **모두 같다**. 사전 분석 A는 HEAD 도구로 만든 결과가 저장소의 이식 전 생성 헤더와 바이트 동일함을 확인했고, 새 결과는 옛 결과와 `TRAINER_PIC_FRONT_` → `TRAINER_PIC_` 변경과 `.trainerBackPic` 줄 삭제만 다르다.
  - Emerald·FRLG 컴파일(기본 `make`는 쓰지 않음, 아래 "후속 행 메모" 별건): 사전 분석 C의 `tmp-C/cc.sh`(Makefile과 같은 `cpp | preproc | cc1 -Werror`, 출력 버림)를 실제 작업 트리에 돌렸다. 대상은 이번에 바뀐 C 파일 전부와 그림 번호를 쓰는 호출 파일(`battle_controller_link_opponent.c`, `frontier_util.c`, `battle_special.c`, `trainer_hill.c`, `trainer_tower.c`, `union_room.c`, `battle_main.c`, `main_menu.c`, `trainer_card.c`, `battle_dome.c`, `hall_of_fame.c`, `pokedex.c`, `pokenav_match_call_list.c`) 33개다. 리뷰 C가 그림 표를 정의·생성하는 `src/graphics.c`(`gTrainerPicInfo`, Makefile처럼 `-Wno-missing-braces`), `src/data.c`, `src/battle_partner.c`와 `debug.c`, `oak_speech_hns.c`, `pokedex(_plus_hgss).c`, `hall_of_fame_frlg.c`를 EMERALD·FIRERED·HNS로 더 컴파일했고 모두 성공했다(실패는 알려진 `party_menu.c:4943` `FLAG_DEFEATED_RED`뿐, 저장소 밖 `chunk-128/review-C/cc-extra*.txt`). EMERALD·FIRERED 각 33개 가운데 32개 성공, 실패는 `src/pokemon.c:7749` `FLAG_DEFEATED_RED` undeclared 1건씩뿐이다. 이 오류는 HEAD에도 있다(사전 분석 C `tmp-C/real-head.txt`: HEAD `pokemon.c:7748` 같은 오류, 줄 번호 차이는 `#include "trainer.h"` 1줄). 사전 분석 C의 `HEAD + A + B + C` 결과(`tmp-C/real-abc.txt`, `real-abc-b.txt`)와 같다. **이번 이식이 만든 새 오류는 0이다.**
- 테스트: 아래 "전체 테스트".
- 남은 위험:
  - **비 RELEASE assert.** `make hns`는 `RELEASE=0`이다. `GetTrainerBack*`·`GetTrainerFront*`에 그림이 없는 ID가 들어오면 assertf 크래시 화면이 뜬다(이전에는 0칸을 조용히 읽음). 지금 데이터는 도구로 누락 0을 확인했다. **새 파트너·트레이너를 추가할 때 그 `Pic:` ID에 앞모습(파트너면 뒷모습도)이 있는지 확인해야 한다.** 예: 뒷모습이 없는 `BURGLAR_HNS`를 파트너 `Pic:`으로 쓰면 배틀 시작 때 멈춘다. 테스트 빌드에서는 `INVALID`가 된다.
  - 녹화 배틀 + AI 파트너 멀티(프런티어) 재생의 플레이어 앞모습 경로는 이식 전에 표 범위 밖(`gTrainerSprites[210/211]`)을 읽던 정의되지 않은 동작이었다. 이식 직후에는 Brendan/May 앞모습이 나왔고, 2026-10-04 수정(`d78de7fdb5`) 뒤에는 Gold/Kris 앞모습이 나온다. 이전 표시는 재현할 수 없다.
  - 통신 상대 앞모습(`battle_controller_link_opponent.c`)은 버전별로 Red/Leaf·RS를 고르고 뒷모습은 버전과 무관하게 Gold/Kris다. 이식 전부터 있던 차이이며 바꾸지 않았다.
  - trainerproc에서 `Pic:` 값이 비면 `.trainerPic`이 빠져 0이 된다. 0의 뜻이 이식 전 HIKER에서 이제 NONE이다(upstream 같음). `Pic: Pokedude`/`Old Man`은 컴파일되지만 앞모습이 없어 assertf가 뜬다. 지금 데이터에는 해당 줄이 없다(리뷰 A).
  - 그림 번호를 `u8`로 돌려주는 함수 4개(`GetFrontierTrainerFrontSpriteId` 등)는 `TRAINER_PIC_COUNT`가 256을 넘으면 잘린다. 지금 213이다(upstream도 같은 타입).
  - 이후 HnS 그림 enum 항목을 `#if IS_HNS`로 감싸면 `if (IS_HNS)` 식 참조(`GetPlayerTrainerPic`, 시설 클래스 표)가 Emerald 빌드를 깬다. 지금은 무조건 정의라 문제없다.
  - 코드 경로(어느 ID를 고르는지, 태그·팔레트 슬롯, y 위치 계산)는 데이터 비교 밖이다. 실기 확인이 필요하다.
- 실기 확인: 필요(아래 "실기 확인 항목" 전체).

## 그림 데이터 비교

명령: `python3 /home/hjm0725/hns-sync-work/chunk-128/verify/trainer_pic_verify.py run`(새 ELF = 이번 빌드 `pokehns.elf`, 새 헤더 = 작업 트리, 옛 ELF = 이식 전 빌드 `chunk-128/base/pokehns.elf`(SHA1 `817f500d…` ROM의 ELF), 옛 헤더 = `bc7c625b79`). 종료 코드 0. 출력 전문은 저장소 밖 `chunk-128/verify-run-post.txt`·`verify/report.txt`, 덤프는 `verify/new_dump.tsv`.

도구는 심볼 주소·태그·enum 숫자가 아니라 **내용**을 비교한다. 앞모습은 압축 그림 바이트 해시·팔레트 32 B 해시·머그샷 좌표·회전·애니메이션, 뒷모습은 좌표(size·y_offset)·프레임 전체 해시·팔레트·애니메이션이다.

| 구간 | 대상 | 비교 | 같음 | 차이 | 비고 |
|---|---|---:|---:|---:|---|
| [1] | `gTrainers` 앞모습 | 1,959 | 651 | 0 | 미사용 칸(party NULL) 1,308: 옛 0=HIKER → 새 0=NONE, 게임에서 쓰지 않음 |
| [1] | `sDebugTrainers` 앞모습 | 6 | 2 | 0 | 미사용 4 |
| [1] | `gBattlePartners` 앞모습 | 18 | 6 | 0 | 미사용 12 |
| [2] | 파트너 뒷모습(옛 `trainerBackPic` ↔ 새 `trainerPic.backPic`) | 18 | 6 | 0 | Brendan·Steven·목호·실버 3. 미사용 12 |
| [3] | 플레이어·고정 뒷모습 경우(`player_cases.tsv` 16개) | 16 | — | 0 | 의도한 변화 0(녹화 재생 Brendan/May 유지) |
| [4] | `gFacilityClassToPicIndex` 경유 앞모습 | 139 | 139 | 0 | 트레이너 카드·프런티어·유니언룸 등 |
| [5] | 옛 그림 ID 224개 → 새 ID 전체 | 224 | — | 0 | 새 쪽에만 있는 항목 2 = `TRAINER_PIC_NONE` 앞·뒤(INFO) |
| [6] | 성별 짝 인접성 | — | — | 0 | BRENDAN/MAY·RED/LEAF·RS 인접, GOLD_HNS 172·KRIS_HNS 175 비인접. 새 소스의 `TRAINER_PIC_*` 덧셈 0곳, BAD 0 |
| [7] | 새 소스의 옛 이름 | — | — | 0곳 | |

`RESULT: 차이 0 (OK) (미사용 슬롯 정보 1336)`. 사전 분석 D의 기대 출력과 수치가 모두 같다(리허설 때 0이던 [5] NONE INFO 2줄은 실제 빌드에서 예상대로 나옴).

## 전체 테스트

- 명령: `GITHUB_ACTION=1 make check BUILD=hns -j6 > build/port-check-post128.log 2>&1`(데스크탑). `include/data.h`·`include/constants/trainers.h`가 바뀌어 테스트 빌드 전체가 다시 컴파일됐다. `make` 종료 코드 2는 실패 테스트가 있을 때의 정상 종료다(seq 127 전체 실행과 같음).
- 결과: PASSED 2,335 / FAILED 2,255 / KNOWN_FAILING 10 / ASSUMPTIONS_FAILED 38 / TO_DO 609 / EXPECT_FAILING 6 / TOTAL 5,253. seq 127(`fdc110d528`, `build/port-check-hnsfix127.log`)과 모든 수가 같다.
- 목록 비교: `PORT_INSTRUCTIONS.md`의 `LC_ALL=C`·`grep -a` 추출로 만든 목록(5,184줄: PASS 2,332 / FAIL 2,230 / KNOWN_FAILING 10 / TO_DO 607 / EXPECTED_FAIL 5, 이름 중복 제거 기준)이 `test-baseline-seq127.txt`와 `cmp`로 **같다**. 사라진 PASS 0, 새 PASS 0, 상태가 바뀐 테스트 0.
- INVALID(추출 목록 밖): 이름 21개가 seq 127 로그와 같다(진행 번호 `n/m`을 빼고 비교). 이번 이식으로 새로 `INVALID`가 된 테스트는 없다. 그림 getter의 assertf(`Sanitize*TrainerPic`)가 테스트에서 실패한 곳이 없다는 뜻이다. `Killed`·러너 크래시 0.
- 관련 테스트(같은 목록 안): 트레이너 슬라이드·멀티 배틀 테스트 경로(`IsMultibattleTest`의 Gold 뒷모습), 파트너 Steven 뒷모습(`ai_multi.c` 등), `trainer_control.c`/`partner_control.party`를 쓰는 테스트가 모두 이전과 같은 상태다. 테스트 party는 upstream 출신 그림(Hiker·Red·Leaf, 파트너 Brendan·Steven)만 쓰므로 **_HNS 그림은 테스트로 검증되지 않는다.** _HNS 그림은 위 데이터 비교와 실기 확인으로 본다.
- 새 기준 목록: [`test-baseline-seq128.txt`](test-baseline-seq128.txt). 내용은 `test-baseline-seq127.txt`와 같다.

## 커밋 리뷰 (병렬 3개, 읽기 전용)

리뷰 지시: 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-128/REVIEW.md`. 리뷰어는 `git show`로 커밋 기준 내용을 읽고 upstream `becfa70a97`·1.17.0·사전 분석·메인 결정과 대조했다. 검증 도구 결과를 그대로 믿지 않고 소스·ROM·ELF 역어셈블로 따로 확인했다.

| 리뷰 | 영역 | 판정 | 확인한 것 |
|---|---|---|---|
| A | 상수·표·도구 | 문제 없음 | 도구 없이 소스로 옛 앞 210·뒤 14 → 새 213 대조(불일치·누락 0, INCBIN 심볼→경로 442쌍 동일), ROM 구조체를 직접 읽어 224개 내용 해시 차이 0, HnS 합친 ID 4개의 뒷모습 전용 팔레트·좌표·애니메이션, 비기본 머그샷 9개, 시설 클래스 139개, `GetPlayerTrainerPic` 역어셈블(성별 0 → 172 Gold, 그 밖 → 175 Kris), party 7개 `Pic:` 2,159줄, 파트너 6명. 참고 2건(migration 정규식 출처, trainerproc 빈 `Pic:`)은 위에 반영 |
| B | 배틀 호출부 | 문제 없음 | upstream B diff와 줄 단위 비교(차이는 의도한 HnS 적응 3가지), HnS 고유 줄 수 보존, 경로별 ID(플레이어·사파리·reshow·통신·파트너 슬롯·녹화 재생·목호·실버·성호·웰리) 이식 전과 같은 인물, 태그 범위(앞 0~212, 뒤 213~425)와 짝 호출, assertf 대상 ID 모두 그림 있음. 정보 2건·문서 1건은 위에 반영 |
| C | 배틀 밖·호환성 | 문제 없음 | C 8파일이 upstream C diff와 같고 HnS 차이가 이식 전후 같음, 배틀 밖 그림 경로, 옛 이름 0건, 그림 번호를 돌려주는 함수 13종 호출부와 세이브·통신 구조체 필드(`RecordedBattleSave`, `TrainerCard`, `LinkPlayer`, 배틀 타워 기록, `Apprentice`, `SecretBase`, `TrainerHillSave`)를 직접 확인해 숫자 저장 없음, Emerald·FRLG 추가 컴파일, 결과 문서 수치. 경미 3건·정보 1건은 위에 반영 |

"수정 필요" 0건. 코드 변경 없이 결과 문서만 고쳤다.

## 친구에게 물을 것

1. **녹화 배틀 재생의 플레이어 뒷모습(Brendan/May 유지 vs Gold/Kris).**
   - 지금(이식 전과 같음): 배틀 프런티어 기록 재생 등 녹화 배틀에서 플레이어 뒷모습만 **Brendan/May**로 그린다(`battle_controller_recorded_player.c` `RecordedPlayerHandleDrawTrainerPic`, `#if IS_HNS`). 같은 파일의 볼 던지기 팔레트 인자(`RecordedPlayerHandleIntroTrainerBallThrow`, `BtlController_HandleIntroTrainerBallThrow`가 읽지 않아 화면에 쓰이지 않음), 실시간 배틀·통신·사파리 뒷모습, 녹화 통신 상대 앞모습은 모두 Gold/Kris다.
   - upstream처럼 바꾸면: `#if IS_HNS … #else … #endif` 6줄(주석 포함)을 `trainerPicId = GetPlayerTrainerPic(gender, GAME_VERSION);` 한 줄로 바꾼다. HnS에서 Gold/Kris가 나온다.
   - 바꾸기를 권한다. 이식 전 HnS가 목호·실버 파트너를 넣을 때(`f206a6c007`) 이웃 줄만 Gold/Kris로 고치고 이 줄을 빠뜨린 것으로 보인다. AI 파트너 멀티 기록의 앞모습 경로도 실시간 프런티어 태그 배틀(Gold/Kris)과 맞게 된다.
   - 바꾸면 그림 데이터 비교 도구의 `player_cases.tsv`에서 `RECORDED_LINK_DRAW_MALE/FEMALE` 두 행을 `new_id=TRAINER_PIC_GOLD_HNS/KRIS_HNS`, `expect=changed`로 고치고 다시 돌린다(기대: [3] EXPECTED 2, 차이 0).

## 실기 확인 항목 (친구용)

이식 전 ROM(`fdc110d528` 코드, SHA1 `817f500d…`)과 이식 후 ROM(`06c6bac8c2`)을 같은 `.sav`로 비교한다. 그림·팔레트(색)·위치(높이)·애니메이션과 크래시 화면 여부를 본다. 데이터는 도구로 바이트 동일을 확인했으므로 주로 **코드가 고르는 ID와 화면 배치**를 본다.

1. **_HNS 트레이너 앞모습 표본:** 체육관 관장(꼭두·비상 등), 로켓단 조무래기·간부, 사천왕·챔피언 목호, 레드, 실버(라이벌전), 일반 트레이너 몇 명. 배틀 시작 슬라이드 인과 배틀 중 트레이너 슬라이드(대사) 인/아웃.
2. **머그샷:** 사천왕·챔피언 목호·레드, 그리고 관장 1명(HnS는 `Mugshot:`이 있는 트레이너가 107명)의 배틀 전환 머그샷 좌표·회전. 상대 B·파트너 머그샷 위치(`x-240`)는 로켓단 아지트 멀티배틀 `TRAINER_ARIANA_1_HNS + GRUNT_23_HNS` + 파트너 목호(`data/maps/RocketHideout_B2F_hns/scripts.inc:310`, 아테나 `Mugshot: Dark Red`)로 본다.
3. **플레이어 뒷모습 Gold/Kris(남·여 각각):** 일반 트레이너전·야생전의 슬라이드, 볼 던지기 동작과 색.
4. **사파리존:** 플레이어 뒷모습, HnS 가방(볼 주머니)을 열었다 닫은 뒤(reshow)에도 같은 그림·색.
5. **파트너 뒷모습:** 로켓단 아지트 목호 태그 배틀, 실버 파트너전 3종(메가니움·블레이범·장크로다일). 파트너 뒷모습·색·높이와 시작 머그샷.
6. **녹화 배틀 재생(배틀 프런티어 기록):** ~~지금은 플레이어 뒷모습이 Brendan/May여야 한다(이식 전과 같음).~~ **2026-10-04 친구 결정으로 Gold/Kris로 바꿨다(`d78de7fdb5`). 지금은 Gold/Kris가 정상이다.** AI 파트너 멀티 기록은 이전에 깨졌던 경로이므로 새 표시(앞모습)를 확인한다. 코드상 이 경우 파트너 자리(B2)도 RecordedPlayer 컨트롤러로 그려져 B0·B2 모두 `gLinkPlayers[0].gender` 기준 Brendan/May 앞모습이 x=90에 나오는 것으로 보인다(upstream 같은 구조, 리뷰 B). Gold/Kris로 바꾼 뒤에는 B0·B2 모두 Gold/Kris 앞모습이 x=90에 나온다(커밋 리뷰 2026-10-04).
7. **트레이너 카드:** 자신의 카드(Gold/Kris), 유니언룸 상대 카드.
8. **포켓기어 전화:** 등록된 트레이너 확인 화면의 그림.
9. **전당:** 전당 등록 화면의 플레이어 앞모습.
10. **배틀 프런티어:** 배틀 타워·돔 등 상대 앞모습, 돔 대진표 정보 카드, 프런티어 AI 파트너 멀티(실시간)의 플레이어·파트너 앞모습.
11. **트레이너힐(트레이너 타워):** 상대 앞모습.
12. **유니언룸·통신 배틀(가능하면):** 상대 앞모습, 내 쪽·상대 쪽 뒷모습(Gold/Kris).
13. **오박사 강의(새 게임):** Gold/Kris 그림. 사무엘 오박사는 트레이너 그림 표가 아니라 `AddNewGameOakObject`의 자체 그림(`field_effect.c`)이라 이번 이식과 무관하다(리뷰 C).
14. **도감 크기 비교:** 플레이어 실루엣.
15. **(선택) 포획 튜토리얼:** HnS 맵 스크립트에서는 쓰지 않고 디버그 스크립트 `Debug_EventScript_WallyTutorial`로만 열린다. 열면 Wally 뒷모습과 가방을 연 뒤(reshow) 같은 그림인지 본다.

## 후속 행 메모

- **별건: 기본 `make`(Emerald)·FRLG 빌드는 HEAD에서 이미 실패한다.** HnS 전용 `FLAG_DEFEATED_RED`를 `src/pokemon.c`(이식 후 7749행)와 `src/party_menu.c`(4943행)가 조건 없이 쓴다(HnS 커밋 `43dab00bb5`·`086b47c96e`). 이번 이식과 무관하며 고치지 않았다. 이번 변경이 이 밖의 새 오류를 만들지 않았다는 근거는 위 "검증"의 `cc.sh` 결과다.
- **새 파트너·트레이너 추가 때:** `Pic:` ID에 그림이 있는지 확인한다(위 "남은 위험" 1). `Back Pic:` 키는 이제 trainerproc 파싱 오류다. 파트너 뒷모습은 `Pic:` ID의 `backPic`이다.
- **upstream 후속 PR이 같은 파일을 문맥으로 쓴다:** #8943(`field_effect.c` 1줄, `pokemon.c` 여러 곳, seq 138.5), #10656, INCGFX 전환(seq 500 #9881 등), #9518·#10172(도감 `CreateSizeScreenTrainerPic` 이동), #9788(migration 스크립트). `becfa70a97..expansion/1.17.0`에서 #9475 hunk 줄 자체를 다시 바꾸는 커밋은 없다(사전 분석 C `-S`/`-G` 검색).
- 녹화 재생 그림을 Gold/Kris로 바꾸면(친구에게 물을 것 1) `player_cases.tsv` 두 행도 함께 고친다.
- **지금은 닿지 않는 경로(리뷰 B):** 프런티어 AI 파트너 멀티에서 `PlayerPartnerHandleTrainerSlide`(`battle_controller_player_partner.c:261`)·`RecordedPartnerHandleTrainerSlide`(`battle_controller_recorded_partner.c:251`)가 불리면 파트너 번호가 아닌 프런티어 트레이너 번호로 `gBattlePartners`를 표 밖에서 읽는다. 이식 뒤에는 assertf로 이어질 수 있다. HnS는 `sTrainerSlides`·`sFrontierTrainerSlides`가 비어 있어 실행되지 않는다(upstream 같은 구조). 프런티어 파트너 슬라이드 대사를 넣을 때 다시 본다.
- 그림 데이터 비교 도구는 저장소 [`trainerpic-9475/`](trainerpic-9475/)에 보존했다(README에 재실행 방법). INCGFX 전환(seq 500 #9881) 등 트레이너 그림 INCBIN을 건드리는 PR 뒤에 다시 돌린다.
- 선택(넣지 않음): `src/trainer.c`에 `STATIC_ASSERT(TRAINER_PIC_COUNT <= 256)`. upstream에 없는 줄이고 지금 213이라 급하지 않다.
