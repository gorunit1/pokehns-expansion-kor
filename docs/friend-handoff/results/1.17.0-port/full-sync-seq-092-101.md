# full-sync 실제 port 결과 — seq 92~101

진행 중: 마지막 완료 seq 98, 다음 seq 99.

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), [`CLAUDE_FULL_SYNC_PORT_PROMPT.md`](../../CLAUDE_FULL_SYNC_PORT_PROMPT.md)
사전 분석: PR마다 병렬 분석 에이전트가 쓴 이식 계획(`hns-sync-work/chunk-092-101/seq<N>-<PR>.md`, 저장소 밖)을 따랐다.
시작 HEAD: `1364cb18fb` (작업 트리 clean)

## seq 92~101 공통 사항

- 빌드 명령: `PATH=/opt/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin:$PATH GITHUB_ACTION=1 make hns -j8 > build/port.log 2>&1` (노트북 WSL, 8코어).
- 기준 빌드(`1364cb18fb`, `rm -rf build/hns` 뒤 전체 재빌드, 26초): 종료 코드 0, **ROM 32,713,060 B(97.49%), EWRAM 248,924 B(94.96%), IWRAM 25,516 B(77.87%)**(seq 91 최종과 같음). 경고 줄 163개, "파일: 메시지"(줄·열 번호 제거) 고유 목록 42개. 이 빌드의 오브젝트와 `pokehns.gba/.elf/.map`을 스크래치에 복사해 두고 이식 후 비교에 썼다.
- 경고 비교: 매 빌드의 경고를 같은 형식으로 만들어 기준 목록과 비교하고 새 경고만 확인했다.
- 테스트: 분석 문서가 지정한 테스트 파일을 파일마다 `GITHUB_ACTION=1 make check BUILD=hns -j6 TESTS="<파일>"`로 돌리고, `PORT_INSTRUCTIONS` "테스트" 절의 `LC_ALL=C` 추출 목록을 [`test-baseline-seq091.txt`](test-baseline-seq091.txt)(= HEAD `7b4e3dba40` 전체 실행 결과)의 같은 이름 줄과 비교했다.
- 파일 단위 실행에서만 나오는 상태(기준 목록의 추출 정규식 밖): `ai_switching.c`의 `AI will not choose to switch out Dondozo with Commander Tatsugiri (1/50): INVALID`("SWITCH not required"). 전체 실행에서는 PASS이고, 이 테스트만 따로 돌려도 PASS다. **seq 92 변경을 되돌린 트리에서도 파일 단위 실행 결과가 같아서**(INVALID) 이식과 무관한 실행 순서 의존 상태로 본다. 같은 파일의 `ASSUMPTION_FAIL` 1건(`AI will stay in if Encore'd into super effective move`, `focus_punch.c`의 1건도)은 이전 전체 로그에도 있던 것이다.
- 한글 포함 소스 줄: 커밋마다 `git show <커밋> | LC_ALL=C grep -a -cP '^[-+](?![-+]).*[^\x00-\x7F]'`.

## 동기화 단위: seq 92 #9429 `U-9429` Add Gen 2-3 and Gen 4 Encore timers

- 현재 판정: 적용(문맥 2곳 수동)
- 커밋: `838e441612`
- upstream 근거: `72d635fc96`
- 해결한 의존성: 없음(deps `-`). seq 101 #9529(`generational_changes.h` → `config_changes.h` 이름 변경)의 기준 blob이 #9429 적용 뒤 파일이라 이 순서가 맞다.
- 수정 파일(6): `include/config/battle.h`, `include/constants/generational_changes.h`, `include/random.h`, `src/battle_script_commands.c`, `src/data/moves_info.h`, `test/battle/move_effect/encore.c`
- 적용 방법:
  - `include/*`·`src/battle_script_commands.c`: `git apply` 그대로(오프셋만 다름). `B_ENCORE_TURNS GEN_LATEST`, `F(B_ENCORE_TURNS, encoreTurns, …)`, `RNG_ENCORE_TURNS`(`RNG_TAUNT_TURNS` 뒤), `Cmd_trysetencore`의 세대 분기(`>= GEN_5`: 4턴, 대상이 이번 턴에 아직 행동하지 않았으면 3턴 / `GEN_4`: 3~7 / 그 이하: 2~6).
  - `src/data/moves_info.h` `[MOVE_ENCORE]`: upstream 문맥 줄 `.name = COMPOUND_STRING("Encore")`가 HnS에서는 `"앙코르"`라 hunk가 실패한다. `.name`은 그대로 두고 영문 description 안에만 `#if B_ENCORE_TURNS >= GEN_5`/`#elif >= GEN_4`/`#else`/`#endif`를 넣었다. 블록이 1.17.0과 `.name` 한 줄만 다르다.
  - `test/battle/move_effect/encore.c`: 기존 테스트 2개의 이름·config 변경은 그대로 적용. upstream은 TO_DO 5줄이 파일 끝이라고 가정하지만 HnS에는 그 뒤에 #9940/#9999 테스트 2개가 이미 있다. TO_DO 5줄과 뒤 빈 줄을 지우고 새 테스트 5개를 파일 끝에 붙였다. 결과 파일이 upstream 1.17.0 `encore.c` 앞 336행과 바이트 동일하다(뒤의 Champions 테스트 3개는 #10151 등 후속 PR 몫).
  - 제외한 hunk: 없음.
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 한글이 든 소스 줄 변경 0(`"앙코르"` 줄은 문맥으로만 남김). 앙코르 배틀 문구(`STRINGID_PKMNGOTENCORE`·`GOTENCOREDMOVE`·`ENCOREENDED`)와 출력 순서 변화 없음(`Cmd_trysetencore`는 메시지를 내지 않는다). config는 upstream과 같게 `GEN_LATEST`(= `GEN_CHAMPIONS`)로 두었다. `>= GEN_5` 분기라 현재 동작(3/4턴)과 같고 `Random()`을 부르지 않아 배틀 난수열도 같다.
- 저장·ROM·그래픽 영향: 세이브 영향 없음(`encoreTimer`는 `gBattleMons[].volatiles` 3비트 필드, 최대 7로 Gen4 7턴도 들어감). `RNG_*` 태그와 `CONFIG_*` 태그 번호가 1씩 밀리지만 저장하는 곳이 없다(`data/battle_scripts_1.s`의 `CONFIG_B_*` 참조는 다시 조립됨을 빌드 로그로 확인). 기술 설명은 `GEN_LATEST`에서 같은 영문 문자열이다.
- 검증:
  - `git diff --check`: 통과. 비ASCII 변경 줄 0. 파일 모드(100755) 유지.
  - `make hns -j8`: 종료 코드 0, **ROM 32,713,092 B(+32 B) / EWRAM 248,924 B(0) / IWRAM 25,516 B(0)**. 새 경고 0.
  - 자동 테스트: `encore.c` 15개(PASS 7 / FAIL 8).
    - 이름 변경 2개: `Encore forces consecutive move uses for 3 turns: Encore used before move (Gen5+)`(옛 이름 `… (Gen5+)` 없이), `… for 4 turns: Encore used after move (Gen5+)`(옛 이름 `… for 3 turns for player: …`) — 둘 다 PASS → PASS. 옛 이름 PASS 2줄이 목록에서 빠지는 것은 회귀가 아니다.
    - 새 PASS 2개: `Encore randomly chooses an opponent target (Gen 2-4)`(옛 TO_DO), `Encore allows choosing an opponent target (Gen 5+)`(신규).
    - 새 FAIL 3개(알려진 한계, 영문 MESSAGE): `Encore's effect ends if the encored move runs out of PP`(`Unmatched MESSAGE`), `Encore lasts for 2-6 turns (Gen 2-3) 1/5 (5/5)`·`… 3-7 turns (Gen 4) 1/5 (5/5)`(`PASSES_RANDOMLY`라 사유가 `Expected 0.19 passes/successes, observed 0.0`으로 표시됨). **로직 확인:** 로컬에서만 세 테스트의 영문 MESSAGE를 HnS 문구(`상대 마자용의\n앙코르 상태가 풀렸다!`, `마자용의\n앙코르 상태가 풀렸다!`)로 바꿔 돌리면 3개 모두 PASS였다(확인 뒤 upstream 원문으로 되돌리고 1.17.0 앞 336행과 `cmp` 동일 재확인). MESSAGE를 지우기만 하면 `NOT ANIMATION`의 끝 기준점이 없어져 항상 통과(0.99)하므로 로직 확인이 되지 않는다.
    - TO_DO 1개(`Encore lasts for 3 turns (Gen 5+)`)는 삭제됐다(위 두 Gen5+ 테스트가 대신함). 나머지 8개는 이식 전과 같다(PASS 3 / FAIL 5).
    - 관련 파일 9개(`taunt.c`, `mental_herb.c`, `aroma_veil.c`, `dancer.c`, `cant_use_twice.c`, `focus_punch.c`, `shell_trap.c`, `sleep_clause.c`, `ai/ai_switching.c`): 추출 목록이 기준 목록과 같다(회귀 0). `ai_switching.c`의 Dondozo INVALID는 위 공통 사항 참고.
  - 실기 확인: 불필요(기본 설정 동작과 문자열 변화 없음). 원하면 앙코르가 3턴(대상이 이미 행동한 턴이면 4턴) 유지되는지만 본다.
- 남은 위험: 낮음. `B_ENCORE_TURNS`를 `GEN_4` 이하로 바꾸는 경우에만 난수를 소비한다.

## 동기화 단위: seq 93 #9558 `U-species-enum-9507` Add some constants nudging users where to put their custom species

- 현재 판정: **이미 적용**. 커밋 없음.
- 근거: `8de47b965d` "Port upstream #9558: …"(seq 91 구간에서 #9507과 같은 unit이라 먼저 넣음). upstream `34cf7172bc`. 현재 `include/constants/species.h`에 `SPECIES_CUSTOM_START`/`SPECIES_CUSTOM_END`가 있다.

## 동기화 단위: seq 94 #9549 `U-fade-9407` Fix blend-immune sprite not fading properly

- 현재 판정: **이미 적용**. 커밋 없음.
- 근거: `b00b2cb140` "Port upstream #9549: Fix blend-immune sprite not fading properly"(seq 70 unit, #9407과 같은 unit). upstream `eebd085c24`(`src/palette.c` 1줄). 뒤에 `f76c7bf7ed`(#10573 팔레트 페이드 재작업)도 이미 들어와 있다.

## 동기화 단위: seq 95 #9542 `U-9542` Replace MAX_GIFT_RIBBONS with NUM_GIFT_RIBBONS

- 현재 판정: 적용(1줄 수동)
- 커밋: `8c7978ad50`
- upstream 근거: `7084eabd49`
- 해결한 의존성: 없음. `NUM_GIFT_RIBBONS`(7)는 `include/constants/pokemon.h`에 이미 있다.
- 수정 파일(4): `include/constants/global.h`, `include/global.h`, `src/pokemon_size_record.c`, `src/trade.c`
- 적용 방법: `include/constants/global.h`를 뺀 3파일은 `git apply` 그대로. `include/constants/global.h`는 upstream 뒤 문맥 `#define ROAMER_COUNT 1`이 HnS에서 `#if IS_HNS` / `ROAMER_COUNT 4` / `#else` / `1` / `#endif` 블록이라 hunk가 실패해서 `#define GIFT_RIBBONS_COUNT 11` 한 줄만 지웠다(HnS `ROAMER_COUNT` 블록 보존). 적용 뒤 `git grep GIFT_RIBBONS_COUNT`는 문서 외 0건. 제외 hunk 없음.
  - `SaveBlock1`: `u8 giftRibbons[NUM_GIFT_RIBBONS]; u8 padding[4];`(11바이트 유지). `/*0x31A8*/` 주석은 upstream 값 그대로 둠(HnS 실제 오프셋은 0x38C8).
  - `sTradeMenu`(교환 메뉴, 힙): 같은 형태.
  - `pokemon_size_record.c`: `sGiftRibbonsMonDataIds[NUM_GIFT_RIBBONS]`(7 → 7), `GiveGiftRibbonToParty`의 인덱스 상한 11 → 7.
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 문자열 변경 없음. 비ASCII 변경 줄 0.
- 저장·ROM·그래픽 영향:
  - **세이브 배치 불변.** 빌드한 ROM 헤더(`sGFRomHeader`)의 오프셋 필드가 이식 전 ROM과 같다: `SaveBlock2` 크기 0xF34, `SaveBlock1` 크기 0x3D90, `externalEventFlags` 0x38E7, `externalEventData` 0x38D3, `giftRibbons` 0x38C8. `rom_header_gf.o` 바이트 동일. 기존 세이브의 `giftRibbons[7..10]` 바이트는 `padding`으로 이름만 바뀌고 읽는 코드가 없다(`pokenav_ribbons_summary.c`는 0..6만 읽음).
  - 동작 변화(의도됨): `GiveGiftRibbonToParty`(외부 이벤트 스크립트 `MEScrCmd_giveribbon`만 호출)에 인덱스 7~10이 오면 이전에는 7칸 지역 배열 밖을 읽었고, 이제는 무시한다. 링크 교환에서 주고받는 선물 리본 바이트는 11 → 7(블록 크기 `BLOCK_REQ_SIZE_40`은 그대로). 이식 전 HnS와 교환해도 상대가 쓰는 4바이트는 미사용 자리다.
- 검증:
  - `git diff --check`: 통과. 파일 모드 유지.
  - `make hns -j8`: 종료 코드 0, **ROM 32,713,092 B(0) / EWRAM 248,924 B(0) / IWRAM 25,516 B(0)**. 새 경고 0. 바뀐 오브젝트는 `trade.o`(`CB2_CreateTradeMenu` 0x910 → 0x918, +8 B, 정렬 여유에 흡수되어 ROM 크기 변화 없음)와 `pokemon_size_record.o`(`GiveGiftRibbonToParty` 비교 즉치값만, 크기 같음).
  - 자동 테스트: 이 PR이 바꾸는 테스트 없음. `test/pokemon.c` 27개가 기준과 같다(PASS 26, 기준 목록 밖 `… learnsets fit within MAX_LEVEL_UP_MOVES … 1436/1573: INVALID` 1건은 이전 전체 로그에도 있음).
  - 실기 확인: 필수 아님. 가능하면 링크 교환 1회(교환 메뉴 진입·교환 완료).
- 남은 위험: 낮음. **seq 516 #9986**이 `GiveGiftRibbonToParty`를 새 파일 `src/give_gift_ribbon_to_party.c`로 옮길 때 upstream 커밋은 옛 `GIFT_RIBBONS_COUNT` 형태를 담고 있다. 이 상수가 없어졌으므로 그때는 1.17.0 최종 형태(`[NUM_GIFT_RIBBONS]`, `index < NUM_GIFT_RIBBONS`)로 옮긴다(아래 "후속 행 메모").

## 동기화 단위: seq 96 #9525 `U-9525` Fill out missing type info

- 현재 판정: 적용(1 hunk 수동)
- 커밋: `6c2c954693`
- upstream 근거: `0f3c8d5bfe`
- 해결한 의존성: 없음. 쓰는 심볼(`MOVE_BREAKNECK_BLITZ`, `MOVE_MAX_STRIKE`, `DAMAGE_CATEGORY_*`)은 모두 있다. 새 심볼 `gItemIconPalette_MysteryTMHM`은 이 PR에서 정의한다.
- 수정 파일(4): `graphics/items/icon_palettes/mystery_tm_hm.pal`(신규, blob이 upstream과 같음, 작업 트리는 `.gitattributes`대로 CRLF), `include/graphics.h`, `src/data/graphics/items.h`, `src/data/types_info.h`
- 적용 방법:
  - 팔레트·`graphics.h`·`items.h`와 `types_info.h`의 `[TYPE_MYSTERY]`·`[TYPE_STELLAR]` hunk는 `git apply` 그대로.
  - `types_info.h` `[TYPE_NONE]` hunk는 문맥 `.name = _("None")`이 HnS에서 `_("없음")`이라 실패해서 `.palette = 15` 아래에 `.zMove = MOVE_BREAKNECK_BLITZ`, `.maxMove = MOVE_MAX_STRIKE` 두 줄을 손으로 넣었다(`.rej`는 지움).
  - 결과를 upstream `0f3c8d5bfe:src/data/types_info.h`와 비교하면 한글 `.name` 줄을 빼고 4줄만 다르다: HnS `841b07a3c0` "Ghost special dark physical"의 `[TYPE_GHOST] SPECIAL`·`[TYPE_DARK] PHYSICAL`(보존).
  - 제외 hunk: 없음.
- 바뀐 값: `TYPE_NONE`·`TYPE_MYSTERY`에 zMove/maxMove(`MOVE_NONE` failsafe 구멍 메움), `TYPE_MYSTERY` `damageCategory` SPECIAL → PHYSICAL과 `paletteTMHM` NULL → `gItemIconPalette_MysteryTMHM`, `TYPE_STELLAR` `damageCategory` 미지정(0 = PHYSICAL) → SPECIAL. upstream 1.17.0까지 `types_info.h` 변경이 더 없으므로 1.17.0 최종값이다.
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 한글 타입 이름과 HnS 고스트·악 분류 유지. 비ASCII 변경 줄 0. 배틀 메시지 변화 없음.
- **동작 변화(HnS 챌린지 "물리/특수 구분: 끔"):** `damageCategory`를 읽는 곳은 `GetBattleMoveCategory()`의 `B_PHYSICAL_SPECIAL_SPLIT < GEN_4 || gSaveBlock3Ptr->challengeSettings.optionStyle == 1` 분기 하나뿐이다. 새 게임 기본값(`optionStyle = 0`)과 추천 모드(구분 켬)에서는 변화가 없고, **커스텀 모드에서 구분을 끈 세이브**에서만 달라진다.
  - 발버둥은 배틀 중 동적 타입이 `TYPE_MYSTERY`라 이전에는 **특수**(특공·특방, 빛의장막 적용, 화상 반감 없음)였고, 이제 **물리**(공격·방어, 리플렉터 적용, 화상 반감)다. 3세대·이후 모든 세대의 발버둥과 같다.
  - 혼란 자해(`MOVE_NONE`, 선택 기술의 동적 타입을 따름)도 발버둥을 고른 턴에는 특수 → 물리다.
  - 타입을 잃은 사용자의 리베레이션댄스도 특수 → 물리(드묾).
  - 스텔라(스텔라 테라버스트·테라클러스터)는 특수가 되지만 HnS 플레이어는 테라스탈할 수 없고 스텔라 트레이너 데이터도 없어 도달하지 않는다.
  - **확인:** 로컬 임시 테스트 2개(`optionStyle = 1`, 커밋하지 않음)로 이식 전후를 비교했다. 공격 200/특공 10 마자용의 발버둥 피해: 이식 전 `optionStyle` 0/1 = 73/5(특수 계산), 이식 후 같음. 상대 리플렉터: 이식 전 피해 22/22(반감 안 됨), 이식 후 반감됨.
- 저장·ROM·그래픽 영향: 세이브 영향 없음(`gTypesInfo`는 ROM 상수). ??? 타입 TM이 없어 아이콘 표시 변화 없음(NULL 팔레트 failsafe만 메움).
- 검증:
  - `git diff --check`: 통과. 파일 모드 유지.
  - `make hns -j8`: 종료 코드 0, **ROM 32,713,124 B(+32 B, 16색 팔레트) / EWRAM 248,924 B(0) / IWRAM 25,516 B(0)**. 새 경고 0. `gItemIconPalette_MysteryTMHM`이 `pokehns.map`에 링크됨.
  - 자동 테스트: 이 PR이 바꾸는 테스트 없음(테스트 러너는 `optionStyle == 0`이라 타입별 분류 분기를 타지 않는다). `struggle.c`(4), `tera_blast.c`(8), `tera_starstorm.c`(4), `revelation_dance.c`(5), `gimmick/dynamax.c`(81), `gimmick/zmove.c`(44), `gimmick/terastal.c`(44, 같은 이름 2개라 목록 43줄) — 추출 목록이 기준과 같다(회귀 0).
  - 실기 확인: **필요**(아래 "실기 확인 필요").
- 남은 위험: 낮음. seq 478 #10300(`DAMAGE_CATEGORY_NONE` 추가) 이식 때 HnS의 `optionStyle == 1` 조건을 새 `GetBattleMoveCategory`로 옮겨야 한다(아래 "후속 행 메모").

## 동기화 단위: seq 97 #9562 `U-9562` Remove PARTNER_DUMMY need by adding additional difficulty when TESTING

- 현재 판정: 적용(HnS 파트너 번호에 맞춰 2파일 수동)
- 커밋: `cc2ce789c3`
- upstream 근거: `03d82af1c6`
- 해결한 의존성: 없음. 선행 #9419(`GetBattlePartnerDifficultyLevel`)는 HnS `976e642137`로 이미 들어와 있다. `PARTNER_STEVEN_TEST`(1)는 `test/test_runner_battle.c`에 있다.
- 수정 파일(5): `include/constants/battle_partner.h`, `include/constants/difficulty.h`, `src/data/battle_partners.party`, `test/battle/partner_control.party`, `test/battle/trainer_control.c`
- 적용 방법:
  - `difficulty.h`·`partner_control.party`·`trainer_control.c`는 HnS가 upstream 부모와 같아 `git apply` 그대로. 테스트 2파일은 upstream `03d82af1c6` 판과 바이트 동일. `difficulty.h`는 upstream 원문 `#endif ` 끝 공백만 빼고 넣었다(`git diff --check` 통과, 동작 같음).
  - `battle_partner.h`: HnS는 원작업자 `f206a6c007`가 2~5번(`PARTNER_LANCE_HNS`, `PARTNER_SILVER_{MEGANIUM,TYPHLOSION,FERALIGATR}_HNS`)을 끼워 DUMMY가 6번이다. 2~5번은 그대로 두고 `PARTNER_DUMMY 6`과 주석을 지운 뒤 **`PARTNER_COUNT 7 → 6`**(upstream은 3 → 2).
  - `battle_partners.party`: 끝의 `=== PARTNER_DUMMY ===` 블록(앞 빈 줄 + 헤더 8줄 + 빈 줄 + `Wynaut` + 빈 줄, 12줄)만 지웠다. 파일은 Silver Gengar의 `- Hypnosis`로 끝나고 파트너 블록은 6개(NONE, STEVEN, LANCE, SILVER×3).
  - 제외 hunk: 없음. `git grep PARTNER_DUMMY`(docs·migration_scripts 제외) 0건.
- `PARTNER_DUMMY` 의존 확인: 정의와 데이터 블록 외 참조 0. HnS 파트너 배틀(`multi_2_vs_2`)은 1~5번만 쓰고(스페이스센터 성호, 로켓 아지트 B2F 목호, 디버그 메뉴 Lance/Silver Multi), `NPCfollower.battlePartner`에 6을 넣는 경로가 없다. HnS는 실제 파트너가 5명이라 테스트용 `PARTNER_COUNT ≥ 3` 조건이 DUMMY 없이 이미 충족됐다.
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 파트너 2~5번 번호·데이터·`Name: {B_RIVAL_NAME}` 그대로. 비ASCII 변경 줄 0. 게임 빌드는 `B_VAR_DIFFICULTY 0`이라 항상 Normal이고 `DIFFICULTY_TEST`는 `#if TESTING`에서만 생긴다(릴리스 `DIFFICULTY_COUNT` 3 그대로).
- 부수 효과(의도됨): 디버그 트레이너 메뉴의 파트너 선택 상한(`PARTNER_COUNT - 1`)이 6 → 5라 빈 DUMMY 파티를 더 고를 수 없다. `IsPartnerTrainerId`가 `TRAINER_PARTNER(6)`을 더는 파트너로 보지 않는다(쓰는 곳 없음).
- 저장·ROM·그래픽 영향: 세이브 구조 변화 없음(`PARTNER_COUNT`·`DIFFICULTY_COUNT` 크기의 세이브 필드 없음).
- 검증:
  - `git diff --check`: 통과. 파일 모드 유지.
  - `make hns -j8`: 종료 코드 0, **ROM 32,713,092 B(−32 B) / EWRAM 248,924 B(0) / IWRAM 25,516 B(0)**. 새 경고 0.
  - 크기 내역(맵 비교): `gBattlePartners` 0x444 → 0x3A8(−156), `sTrainerSlides` 파트너 1행(−168), DUMMY의 Wynaut 파티(−36) = rodata −360 B. 대신 `gBattlePartners` 행 간격이 7×52 → 6×52 = 312가 되어 `GetTrainerStructFromId` 인라인을 쓰는 함수 42개가 `movs/lsls/muls`(×364) 대신 시프트·가감(×312)으로 곱해 4~28 B씩 늘었다(`GetTrainerMoneyToGive`는 그 밖에 `IsPartnerTrainerId`의 `cmp #5` → `#4`만 다름). 섹션 합계 −48 B, 정렬 후 ROM −32 B.
  - 자동 테스트: `trainer_control.c` 20개(PASS 19 / FAIL 1), `trainer_slides.c` 39개(PASS 12 / FAIL 27), `ai/ai_multi.c` 11개(PASS 11) — 추출 목록이 기준과 같다(회귀 0). 바뀐 파트너 난이도 테스트 4개(`… for partner … (EASY/HARD/NORMAL)`, `Difficulty default to Normal if the partner doesn't have a member …`)는 모두 PASS. `trainer_control.c`의 FAIL 1건(`CreateNPCTrainerPartyForTrainer generates customized Pokémon`)은 기준에서도 FAIL.
  - 실기 확인: 필수 아님(파트너 번호 불변). 원하면 디버그 메뉴 `Lance Multi`·`Silver Multi`로 파트너 파티·뒷모습 1회.
- 남은 위험: 낮음. seq 128 #9475(트레이너 그림 정보 재작업)는 이 PR 뒤 문맥(`Difficulty: Normal`, DUMMY 없음)을 전제로 하고, HnS LANCE·SILVER×3의 `Back Pic:`을 따로 처리해야 한다(아래 "후속 행 메모").

## 동기화 단위: seq 98 #9564 `U-animcall-9142` Fixed Stuff Cheeks animation

- 현재 판정: **이미 적용**. 커밋 없음.
- 근거: `125e893903` "Port upstream #9142: Create functions for repeated move animations (+ #9473, #9564)"(같은 unit으로 한 커밋). upstream `070f31e384`. 현재 `data/battle_anim_scripts.s` `BiteOpponent`의 두 `create_sharp_teeth_sprite`가 upstream 수정 후와 같은 `x=-33`이다.
