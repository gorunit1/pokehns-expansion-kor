# full-sync 실제 port 결과 — seq 92~101

진행 중: 마지막 완료 seq 100, 다음 seq 101.

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

## 동기화 단위: seq 99 #9124 `U-9124` Switch AI sees stat, volatile, status, and HP changes on switchin in calcs

- 현재 판정: 적용(HnS 적응 + upstream과 다른 가드 1곳)
- 커밋: `fac71f54b0`
- upstream 근거: `2c965d02b6`
- 해결한 의존성: 선행 PR 없음(쓰는 심볼·시그니처가 모두 HnS에 있음). 같은 unit으로 기록된 #9579(seq 105)의 `HOLD_EFFECT_TERRAIN_SEED` 중괄호를 이 커밋에 함께 넣었다 → **seq 105는 "이미 적용"으로 처리**한다.
- 수정 파일(6): `include/battle_ai_util.h`, `include/battle_util.h`, `src/battle_ai_switch.c`, `src/battle_ai_util.c`, `src/battle_util.c`, `test/battle/ai/ai_switching.c`
- 적용 방법:
  - `battle_ai_switch.c` 외 5파일: upstream patch 그대로(오프셋만). `SetBattlerFieldStatusForSwitchin` 삭제(→ `SetBattlerVolatilesForSwitchin`으로 이동·확장), `AI_GetSwitchinWeather`에 `ABILITY_ORICHALCUM_PULSE`(쾌청), `AbilityBattleEffects`의 다운로드 스탯 선택을 `GetDownloadStat()`로 추출(동작 불변), 테스트 4개 추가.
  - `battle_ai_switch.c`: 사전 분석 문서의 검증 patch(부록 A, `git apply --check` 통과)로 적용했다. upstream 9 hunk 중 3개는 그대로, 6개는 HnS 문맥(`GetPartyMonAbilityForSwitchCalc` 선언, 3인자 `IsMoldBreakerTypeAbility`, HnS 회복 도구 switch)에 맞춰 손으로 맞춘 형태다. 새 함수 7개: `IsSwitchinTSpikesAffected`, `GetSwitchinSingleUseItemHealing`, `SetBattlerStatusForSwitchin`, `SetBattlerStatStagesForSwitchin`, `SetBattlerHPChangeForSwitch`, `SetBattlerVolatilesForSwitchin`.
  - 제외 hunk: 없음. upstream 추가 줄의 줄 끝 공백 3줄은 지웠다(1.17.0 원문도 지운 형태).
- HNS 적응:
  - **구 시트러스 챌린지**: `GetSwitchinHitsToKO`에 있던 HnS 분기(`tx_Mode_New_Citrus == 0`이면 시트러스 30 고정)를 새 `GetSwitchinSingleUseItemHealing`의 `HOLD_EFFECT_RESTORE_PCT_HP` case로 옮겼다(`// HnS:` 주석). 새 `SetBattlerHPChangeForSwitch`에도 같은 규칙이 적용된다(엔진과 일치).
  - **혼란 열매**: upstream `HOLD_EFFECT_CONFUSE_SPICY/DRY/SWEET/BITTER/SOUR` 5 case 대신 HnS에 있는 `HOLD_EFFECT_CONFUSE_FLAVOR` 1 case(#10163 최종형과 같은 본문).
  - **Supreme Overlord(Champions)**: 교체 후보 카운터 설정을 `InitializeSwitchinCandidate`에서 `SetBattlerVolatilesForSwitchin`의 `case ABILITY_SUPREME_OVERLORD`로 옮겼다(1.17.0 #10145 위치, HnS는 카운터가 `gBattleStruct`에 있고 `B_UPDATED_ABILITY_DATA >= GEN_CHAMPIONS` 게이트 유지). 저장·복원(`savedSupremeOverlordCounter`)은 `InitializeSwitchinCandidate`에 그대로 둔다. 카운터를 읽는 곳(`GetSupremeOverlordModifier`)은 후보 루프의 대미지 계산뿐이라 설정 시점 이동의 영향이 없다. upstream이 `SetBattlerStatStagesForSwitchin`에 넣는 no-op `case ABILITY_SUPREME_OVERLORD: break;`는 #10145 최종형에 맞춰 생략했다.
  - HnS 전용 유지: `GetPartyMonAbilityForSwitchCalc`, 앞쪽 `gBattlerPartyIndexes = monIndex` + `CopyMonAbilityAndTypesToBattleMon`, `switchInCalc` 플래그.
- **upstream 1.17.0과 다른 점(메인 결정):** `SetBattlerHPChangeForSwitch`에서 `if (currentHP < 0) currentHP = 0;` 가드를 넣었다(`// HnS:` 주석). upstream은 등장 장판 대미지가 현재 HP 이상이면 `s32` 음수를 `u16 hp`에 그대로 넣어 약 65,000으로 감긴다. 그러면 싱글 Integrated 경로의 `GetSwitchinHitsToKO`가 1 대신 매우 큰 값을 돌려줘 장판에 바로 쓰러질 빈사 몬을 "튼튼한 방어 후보"로 보고, 대미지가 작으면 루프가 수천 번 돈다(1.17.0·현 master에도 남아 있음). 가드 뒤에는 hp 0 → `hazardDamage >= hp`로 1을 돌려준다(이식 전 HnS 동작 `hazardDamage >= hp → 1`과 같음).
- 동작 변화(의도된 upstream 변화): 교체 후보 평가가 등장 효과(재앙 4종, 쿼크차지/고대활성, 풍력발전, 독압정 상태, 불굴의검/불요의방패/다운로드/위협/감미로운꿀/바람타기, 끈적끈적네트, 시드·핀치 열매·룸서비스·미러허브, 장판 대미지 후 1회용 회복 도구)를 반영한다. 영향 범위는 싱글 `AI_FLAG_SMART_MON_CHOICES`(`GetBestMonIntegrated`)뿐 아니라 그 밖의 모든 AI 트레이너·더블배틀의 `GetBestMonVanilla`(후보 대미지 계산), `AI_SelectRevivalBlessingMon`, 예측 트레이너의 플레이어 교체 예측까지다.
- HNS 보존: 한글 문자열·STRINGID·배틀 메시지 변화 없음(비ASCII 변경 줄 0). 다운로드 리팩터는 스탯 선택만 함수로 뺐고 `CompareStat` → `SET_STATCHANGER` → `PREPARE_STAT_BUFFER` → `BattleScriptCall` 순서 그대로. 새 config 없음.
- 저장·ROM·그래픽 영향: 세이브 영향 없음(`challengeSettings.tx_Mode_New_Citrus`는 읽기만). 새 전역 없음.
- 검증:
  - `git diff --check`: 통과. 파일 모드 유지.
  - `make hns -j8`: 종료 코드 0, **ROM 32,714,516 B(+1,424 B) / EWRAM 248,924 B(0) / IWRAM 25,516 B(0)**. 새 경고 0. 섹션 변화(맵 비교): `InitializeSwitchinCandidate` 448 → 2,064 B(새 static 함수 대부분이 인라인) + switch 표 116 B, `IsSwitchinTSpikesAffected` +320, `GetSwitchinSingleUseItemHealing` +184, `GetDownloadStat` +204, `AbilityBattleEffects` −600, `GetSwitchinHitsToKO` −172, `GetSwitchinStatusDamage` −116, `SetBattlerFieldStatusForSwitchin` −132, `AI_GetSwitchinWeather` +16.
  - 자동 테스트(9개 파일): `ai/ai_switching.c` 123개(PASS 104 / FAIL 16 / KNOWN_FAILING 1, 기준 목록 밖 INVALID 1·ASSUMPTION_FAIL 1은 위 공통 사항), `ai_flag_predict_switch.c` 11(PASS 11), `ai_double_ace.c` 4(3/1), `ai_doubles.c` 50(43/3), `ai_multi.c` 11(11), `ai_flag_sequence_switching.c` 4(2/2), `ai/ai.c` 75(PASS 58 / FAIL 14, 같은 이름 1쌍, `First Impression …` ASSUMPTION_FAIL 2건은 이전 전체 로그에도 있음), `ability/download.c` 4(FAIL 4, 모두 `Unmatched MESSAGE`), `ability/supreme_overlord.c` 5(1/4, FAIL은 모두 `Unmatched MESSAGE`). 기존 테스트는 기준 목록과 같고(회귀 0), **새 테스트 4개(`AI_FLAG_SMART_MON_CHOICES: AI sees stat stage / status / volate / HP changes …`) 모두 PASS**.
  - 실기 확인: **필요**(아래 "실기 확인 필요").
- 남은 위험(upstream과 같은 한계, 원문대로 둠): 스탯 단계·HP 변화가 다른 배틀러마다 반복 적용된다(더블에서 끈적끈적네트 −3, 장판 대미지 3배, 파트너에게도 위협). 위협·감미로운꿀로 낮춘 상대 스탯 단계가 같은 선택 과정의 다음 후보 평가에 누적된다(`FreeRestoreBattleMons`에서만 복원). 장판 대미지가 `SetBattlerHPChangeForSwitch`와 `GetSwitchinHitsToKO`에서 두 번 빠진다. 상대가 +6일 때 오기/승기 +2가 붙으면 스탯 단계 표 범위를 넘는다(드묾). 뒤 PR(seq 104 #9551, 112 #9587, 145 #8472, 173 #9847, 383 #10145, 481 #10326)이 이 함수들을 다시 바꾼다(아래 "후속 행 메모").

## 동기화 단위: seq 100 #9548 `U-aicalc-9548` Adds ai calcs for Bolt Beak, Payback and Analytic

- 현재 판정: 적용(1.17.0 최종형, 원 커밋 hunk 4개 제외)
- 커밋: `3c09c95b2a`
- upstream 근거: `f6a838e1d8`(원형). 최종형 근거: #9596 `b9a1dcfbde` → #10453 `2f7029e124` → #8647 `64c5044083`(= 1.17.0).
- 해결한 의존성: seq 99 #9124 뒤에 적용(같은 `battle_util.c`, 함수 겹침 없음). HnS에는 `AI_SetBattlerTurnOrder`가 이미 공개 함수로 있어(`battle_ai_util.h`, 1.17.0 본문과 같음) 중간 단계 없이 최종형을 넣었다.
- 수정 파일(2): `src/battle_util.c`, `test/battle/ai/ai.c`
- 적용 방법:
  - `src/battle_util.c`: `IsLastMonToMove` 뒤에 `GetAiTurnOrder`·`Ai_AttackerMovesAfterTarget(battlerAtk, battlerDef)`·`Ai_AttackerMovesLast(battlerAtk)`(호출할 때 `AI_SetBattlerTurnOrder`로 로컬 순서 계산). 세 함수는 1.17.0 `battle_util.c`와 글자까지 같다. `CalcMoveBasePower`의 `EFFECT_PAYBACK`·`EFFECT_BOLT_BEAK`, `CalcMoveBasePowerAfterModifiers`의 `ABILITY_ANALYTIC`에 `ctx->aiCalc` 분기를 넣었다. 1.17.0과 다른 줄은 실전(else) 분기의 `gBattleStruct->battlerState[battlerDef].isFirstTurn`(1.17.0은 #9786 `BattlerJustSwitchedIn`, seq 159 몫) 2줄뿐이고 들여쓰기는 #9786 hunk가 그대로 붙도록 upstream과 같게 뒀다.
  - `test/battle/ai/ai.c`: upstream hunk 그대로(`TURN { ` 끝 공백 제거 1줄 + Bolt Beak AI 테스트 2개). 새 테스트는 1.17.0과 같다.
  - **제외 hunk(4):** `include/battle_util.h`의 `BattleContext.aiTurnOrder` 필드(최종형에 없음, #9596이 제거), `src/battle_ai_util.c`의 `static void AI_SetBattlerTurnOrder` 선언·정의(HnS 공개 함수와 충돌)와 `AI_CalcDamage` 안의 호출(원형의 프레임 회귀 원인, #9596·#10453이 바꿈).
- 동작 변화(의도된 AI 개선): AI 대미지 계산에서만 보복(공격자가 늦으면 ×2)·전격부리/아가미물기(공격자가 빠르면 ×2)·애널라이즈(살아 있는 배틀러 중 마지막이면 ×1.3, 미래예지 제외)를 AI 예측 속도 순서로 판정한다. 이전에는 지난 턴의 행동 기록으로 판정했다. 실전 대미지·메시지 경로(`aiCalc == FALSE`)는 식이 그대로다. HnS 트레이너 중 해당: `TRAINER_BLUE_HNS` 마기라스(보복), `TRAINER_AKALA_SWIMMER_1_HNS` 아쿠스타·`TRAINER_JASMINE_POSTOBC_HNS` 자포코일(애널라이즈)(사전 분석 기준).
- HNS 보존: 한글·STRINGID·메시지 변화 없음(비ASCII 변경 줄 0). `AI_SetBattlerTurnOrder`·HnS `AI_CalcDamage`(Nature Power 분기)·`GetDamageCalcAbility` 가드 수정 없음. `BattleContext` 크기 그대로.
- 저장·ROM·그래픽 영향: 세이브 영향 없음.
- 검증:
  - `git diff --check`: 통과. 파일 모드 유지.
  - `make hns -j8`: 종료 코드 0, **ROM 32,714,948 B(+432 B) / EWRAM 248,924 B(0) / IWRAM 25,516 B(0)**. 새 경고 0. 섹션 변화: `Ai_AttackerMovesAfterTarget` +96(나머지 둘은 인라인), `CalcMoveBasePower` +120, `DoMoveDamageCalcVars` +208(애널라이즈 분기 인라인), `BattleScriptPush` +4(`assertf`의 `__LINE__` 1264 → 1301이 즉치값으로 표현되지 않아 리터럴로).
  - 자동 테스트: 코드 넣기 전 테스트만 먼저 넣고 돌리면 새 테스트 2개가 FAIL(`Bolt Beak … (singles) 2/2`는 `Unmatched EXPECT_MOVE`, `(doubles)`는 `Unmatched SCORE_EQ_VAL`)이었고, 코드를 넣은 뒤 둘 다 PASS. `ai/ai.c` 77개(PASS 60 / FAIL 14, 기준 대비 새 PASS 2, 나머지 같음), `bolt_beak.c`(PASS 2), `payback.c`(TO_DO 1), `analytic.c`(PASS 3 / TO_DO 2), `sheer_force.c`(30/1), `ai/ai_doubles.c`(43/3) — 기준 목록과 같다(회귀 0).
  - 실기 확인: 권장(필수 아님). 아래 "실기 확인 필요".
- 남은 위험(upstream과 같은 한계): AI 순서 계산이 우선도·교체 예측을 보지 않는다. `Ai_AttackerMovesLast`는 살아 있는 수만 세지만 정렬에는 쓰러진 칸도 들어가 더블에서 ×1.3을 잘못 줄 수 있다. `Ai_AttackerMoves*` 호출마다 `AI_WhoStrikesFirst`를 16번 부른다(1.17.0과 같은 비용).
