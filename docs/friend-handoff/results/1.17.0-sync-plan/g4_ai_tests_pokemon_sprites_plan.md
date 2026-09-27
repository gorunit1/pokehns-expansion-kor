# g4_ai_tests_pokemon_sprites 이식 계획 (1.17.0 전체 엔진 동기화, 1단계 재확정)

- **대상**
  - 입력 `g4_ai_tests_pokemon_sprites.tsv`의 93개 PR과, 추가 과제로 만든 테스트 러너 복구 unit `U-testrunner-fix` 1행이다. 결과는 `g4_ai_tests_pokemon_sprites_plan.tsv`(헤더 + 94행)에 있다.
  - 구성은 Battle AI 46, Test Runner 18, Pokémon/Sprites 29다.
- **기준**
  - 스냅샷 `e95679c545`(묶음 A 35건 포함)와 upstream `expansion/1.17.0`을 직접 대조했다. HnS 원 저장소도 같은 커밋이다.
  - 새 지시서(`POKEEMERALD_EXPANSION_1.17.0_FULL_ENGINE_SYNC.md`)를 따랐다. 목표는 전체 엔진 동기화다. 최종 판정은 이식·동등·무관·외부결정 4가지뿐이다.
- **방법**
  - `git apply --check`/`-R --check`와 파일·함수·라인 대조만 했다.
  - 빌드·테스트 실행·실기 검증은 하지 않았다.

## 1. 판정별 개수

| 판정 | 계 | Battle AI (46) | Test Runner (18) | Pokémon/Sprites (29) |
|---|---:|---:|---:|---:|
| 이식 | 86 | 44 | 18 | 24 |
| 동등 | 6 | 1 — #10427 | 0 | 5 — #9945, #10250, #10327, #10601, #10603 |
| 무관 | 0 | 0 | 0 | 0 |
| 외부결정 | 1 | 1 — #10461 | 0 | 0 |
| (추가) U-testrunner-fix | 1 | | | `이식`, kind `build`, pr 칸 `testrunner` |

- **어제와 크게 달라진 점**
  - 어제 `이미 적용` 20건(AI) 가운데, 게임 코드는 지금 HnS에 줄 단위로 같지만 **테스트 hunk가 없는 14건**은 `이식`(kind=test, 잔여=테스트 파일)으로 바꿨다: #9359 #10124 #10139 #10046 #9448 #10626 #10669 #10700 #10243 #10258 #10236 #10277 #10610 #10688.
  - 게임 코드 잔여가 있는 이미 적용분 4건도 `이식`이다: #9462(GetConfig 래핑), #10464(#9730 경로 스탯 점수), #10453(Payback 턴순서), #8647(정리·캐시).
  - 어제 `HNS와 무관`이던 25건은 모두 바뀌었다.
    - 테스트·러너 PR은 `이식`(kind test/tool)이 됐다.
    - 리팩터 회귀 수정은 `이식`(deps=리팩터)이 됐다: #10285←#9730, #9596←#9548, #10214←#10170, #10368←#9006, #10116·#10561←#9881.
    - #10387(za.json)은 툴 데이터라 `이식`(tool)이다.
    - #9987(Z-A 울음소리 WAV)은 설정 OFF를 유지한 채 파일만 반입하는 `이식`이다(아래 3절).
- **kind 분포**: test 31, refactor 18, fix 14, feature 12, gfx 10, data 4, build 3, tool 2.
- **size 분포**: S 56, M 33, L 3(#10155, #9124, #9857), XL 2(#9507, #9710).
- **save_impact=Y는 0건이다.**
- **korean_touch=Y는 10건이다.** 4절에서 다룬다.

## 2. 대형 리팩터 unit과 권장 이식 순서

`deps`의 PR 가운데 다른 그룹 것은 괄호에 그룹을 적었다. 화살표는 먼저 넣을 것 → 나중에 넣을 것이다.

1. **테스트 러너 (먼저 하면 이후 모든 단위의 자동 검증이 가능해진다)**
   - `U-testrunner-fix`(include 1줄) → #10665(콜백 초기화)
   - → #9892(g5, 러너 커밋 11개) → #7360(효과음 판정 + EFFECTIVENESS_SE) → #9642(테스트 인벤토리) → #9805(GHOST 테스트)
   - → `U-runnerinfra-10155`: #10155 → #9807 → #10390 → #10537
   - → #10321(ITEM_POPUP)
   - 테스트 전용 PR(#10000 #10017 #10021 #10358 #10379(←#7360) #10133)은 러너 복구 뒤 아무 때나 넣는다.
2. **AI config·대미지 롤 `U-aiconfig-9460`**
   - #9529(g2 GenConfig 개명) ‥ #9460 → #9462 잔여 → #9568 → `U-9630`
   - #9448 테스트는 #9462 뒤다.
3. **대미지 컨텍스트 `U-damagecontext-9710`**
   - #9657(g2, BattleContext→DamageContext) → #9710
   - 단, Friend Guard 혼란 자해 예외 1줄(`battle_util.c:7747`)은 선행 없이 지금 넣을 수 있다.
4. **AI 턴순서 계산 `U-aicalc-9548`**
   - #9548(1.17.0 최종형 `Ai_AttackerMovesAfterTarget/Last`가 HnS `AI_SetBattlerTurnOrder`를 호출) → #9596(holdEffectParams 제거) → #10453 잔여 → #8647 잔여
   - #8647은 #9710, #9460/#9462, #9779, #10381도 필요하다.
5. **스탯 변화 `U-statchange-9730`**
   - #9730(g2) → #10057(g3) → #10285, #10464 잔여
   - 같은 경로를 쓰는 것: #10381 테스트(`EFFECT_STAT_CHANGE`), #10124의 `IsSubstituteProtected` 교체
6. **AI 교체·예측**
   - #9124 → #9551 → #8472
   - #9451은 독립이다.
   - #9575·#9847(g6), #9358 → #9857(L)
   - #10289(g1) → #10424
7. **배틀 시작 상태 `U-startstatus-10170`**: #10170(g3) → #10214. 단독 적용 금지다.
8. **#10151 잔여(g3) → #10282.** Champions 선택 제한·마비 config가 선행돼야 한다.
9. **그래픽**
   - `U-gen7anim-sprites`: #9974 → #10141 → #10208 → #10270 → #10414. 팔레트·back 불일치 색 버그와 `shared_front_pic_anims.h` 자리표시자 12곳(토템·오리지널 포함)과 `sAnims_Vikavolt` 퇴행 복구를 포함한다.
   - 독립 PR: #10346, #10252, #10206
   - `U-incgfx-9881`: #9881(g5) → #10116, #10561
   - 주의: #9881은 `graphics_file_rules.mk`에서 폰트 규칙 절을 포함한 367줄을 지운다. HnS `font*_korean.latfont` 규칙(275-322행)은 옮겨 보존해야 한다(보고서 6절 충돌). 그 전에는 #10141/#10252/#10206의 `pokemon.h` 줄을 INCBIN 표기로 넣는다.
10. **기타 리팩터**
    - `U-spots-9594`: #9594 → #9796
    - `U-relearner-9006`: #9006(g6) → #10368
    - `U-species-enum-9507`: #9507(XL, 148파일) → #9558. 규모가 커서 Pokémon 묶음의 마지막을 권장한다. 이후 PR의 `enum Species`는 u16/u32로 적응해서 먼저 넣어도 된다.
    - 순수 리팩터 #9116(battle_ai_record), #10071, #9173, #9135는 우선순위가 낮다.

## 3. `외부결정` 상세

### #10461 Minor AI score calculation clean up — `src/battle_ai_main.c:1037-1038` `ChooseMoveOrAction_Doubles`

- **HnS 현재 동작**
  - 코드: `bestMovePointsForTarget[battlerDef] <= AI_SCORE_DEFAULT`(upstream은 `<`)
  - 결과: 점수가 기본값 100 그대로인 아군 대상 기술을 AI가 고르지 않는다.
- **출처**
  - 원작 HnS 브랜치 `hns-master`의 커밋 `97ad372298` "Fix battle factory doubles ai bug"(dylanfalzone1, 2026-09-11)다.
  - 한글화 저장소에는 첫 업로드 `1821fd6749`부터 들어 있다. 이식 실수가 아니라 원작자의 의도된 수정이다.
- **선택지**
  - **A(권장): `<=` 유지.**
    - 배틀 팩토리 더블의 아군 오폭 방지가 유지된다.
    - 기본 점수의 아군 대상 기술을 기대하는 upstream 테스트는 HnS 기대값으로 조정해야 한다.
  - **B: `<`로 동기화.**
    - 1.17.0과 완전히 일치한다.
    - 동점일 때 AI가 무작위로 아군을 칠 수 있어 원작 버그가 재발할 수 있다(AI 체감·난이도 변화).
- **어느 쪽이든 이식할 것**: `BattleAI_SetupAIData`(`:352-353`)의 전역 `gBattlerTarget` 쓰기를 `gAiBattleData->chosenTarget[battler]`로 바꾼다. 1줄짜리 이식 누락이다.

### PR 판정은 `이식`이지만 hunk 단위로 사용자 확인이 필요한 항목

| PR | 파일·함수 | HnS 현재 | A | B |
|---|---|---|---|---|
| #10270 | `gen_8_families.h` `[SPECIES_FALINKS_MEGA]` `.abilities` | Defiant×3(#10250 Champions M-B 값) | HnS 유지, 이 hunk 제외(권장, upstream 병합 실수로 추정) | 1.17.0 값 Battle Armor/None/Defiant(밸런스 변화, 트레이너·AI 재점검) |
| #10414 | `graphics/pokemon/marowak/alola/back.png` | HnS 커스텀 `normal.pal`(`993b7bdf50`) + 옛 back | 옛 back 유지, shiny.pal·애니 표만(권장) | 새 back 반입. 일반색에서 45px가 idx14 진보라로 합쳐져 HnS 커스텀 명암이 바뀜 |
| #9568 | `include/config/ai.h` AI 롤 설정 | 모든 AI 계산 median | `AI_ROLL_ATTACKING`·`..._SHOULD_SETUP_DEFENDING`=MEDIAN으로 현재 난이도 유지(권장). `..._ATTACKING_PARTNER`는 MAX(약간 신중)나 MEDIAN(완전 동일) | upstream 기본 MAX 채택. AI가 확정 타수를 낙관해 공격 선택이 늘어남 |
| #9594 | `src/pokemon_spots.c` 알록반 점 2 기준 y | 25(바닐라와 같음) | 25 유지 | upstream 27. 기존 개체의 점이 2px 아래로 그려짐(세이브 불변) |
| #9987 | `sound/direct_sound_samples/cries/*mega*.wav` 26개 | 옛 WAV, `P_MODIFIED_MEGA_CRIES` FALSE | WAV만 반입하고 설정 FALSE 유지(ROM 불변) | 사용자 결정 "32MiB 유지"를 "파일도 넣지 않음"으로 해석해 미반입 |

## 4. 한글 메시지와 겹치는 항목의 적응 방법

- **#10214**
  - `battle_message.c:847-849`(불바다·습지)는 HnS 한글 문자열이 이미 목표 토큰 `{B_ATK_TEAM2}`/`{B_DEF_TEAM2}`를 쓰므로 그 hunk는 건너뛴다.
  - `battle_end_turn.c`는 무지개 블록의 `gBattlerAttacker` 대입을 옮기는 동시에 불바다·습지 블록에 대입을 추가해 출력 대상을 보존한다.
- **#9805**: 미번역 FRLG 문자열 `STRINGID_GHOSTWASMAROWAK`(`:898`)의 끝 `\n` 한 글자만 지운다. `//frlg` 주석과 본문은 유지한다.
- **#9977**: 꿈먹기 스크립트 분기만 런타임 config로 바꾼다. 실패 출력(`BattleScript_DoesntAffectTargetAtkString`)의 문구·순서는 그대로다.
- **#10282**
  - `moves_info.h` 프리즈드라이 블록(한글 기술명, 영문 설명)에는 `B_UPDATED_MOVE_DATA` 분기만 추가한다.
  - 테스트 기대값은 HnS가 바꾼 출력(리프레시·아로마테라피의 상태별 치료 문구, 생명의구슬 팝업 + `STRINGID_LOSTSOMEOFITSHP`)을 기준으로 둔다.
- **#9710**: `gLastUsedAbility`/`BattleScriptCall`에 인접한 줄을 같은 값의 배열 참조로 바꿀 뿐이다. 출력 순서는 불변이다.
- **종 데이터(#9945 #10250 #10603 #10270 #10346)**: hunk 문맥에 영문 `speciesName`이 있어 기계 적용이 안 된다. 한글 종명 블록은 두고 값만 수동으로 반영한다.
- **테스트 전반**: HnS는 배틀 문자열 706개 중 669개, 종명 1,403개 전부, 기술명이 한글이다. 그래서 영문 `MESSAGE()` 기대값은 일치하지 않는다(5절). 테스트 기대값을 한글화할지, 메시지 검사를 ABILITY_POPUP/ITEM_POPUP/EFFECTIVENESS_SE 같은 언어 무관 검사로 바꿀지 정책이 필요하다. 문자열 본문은 절대 영문화하지 않는다.
  - 영문 의존이 **새로 생기는 PR**: #10000(SEND_IN_MESSAGE), #10021, #10214(general.c 53개), #9805(ghost.c 22개), #8664(first_turn_only.c 15곳), #9462(ai_switching.c 3개)
  - 영문 의존을 **줄이는 PR**: #10379(상성 MESSAGE→EFFECTIVENESS_SE), #10321(ITEM_POPUP)

## 5. 테스트 러너 복구 `U-testrunner-fix` (추가 과제, 코드 검토만)

### 원인

- HnS 커밋 `16ca5376eb`("fake rtc")가 `include/fake_rtc.h:7-14`에 `static inline bool32 UseFakeRtc(void)`를 추가했다.
- 이 함수는 `gSaveBlock3Ptr->challengeSettings.tx_Features_RTCType`을 역참조하므로 `struct SaveBlock3`의 완전한 정의(`include/global.h:343`)가 필요하다.
- `fake_rtc.h`를 포함하는 파일은 10개다.
  - src 9개(`load_save.c:4`, `rtc.c:9`, `scrcmd.c:22`, `overworld.c:17`, `debug.c:72`, `match_call.c:10`, `play_time.c:3`, `reset_rtc_screen.c:4`, `fake_rtc.c:7`)는 모두 `global.h` 뒤에서 포함한다.
  - **`test/test_runner.c`만 2행에서 `fake_rtc.h`를, 3행에서 `global.h`를 포함한다.**
- 함수 안의 `extern struct SaveBlock3 *gSaveBlock3Ptr;`가 블록 범위의 불완전 형식을 새로 선언하고, 그 형식을 역참조해서 컴파일 오류가 난다.
- 러너 파일(`test/test_runner*.c`, `include/test/*`, `ld_script_test.ld`)은 merge-base `3efb836f72`와 같다(모드 변경만 있음). 즉 HnS 쪽 헤더 변경이 옛 포함 순서를 깨뜨린 것이다.

### 최소 수정 (1줄)

```c
// test/test_runner.c
#include <stdarg.h>
#include "global.h"
#include "fake_rtc.h"   // global.h 뒤로 이동
```

- upstream #9892(g5)의 커밋 `35a45557e1` 첫 hunk와 같다. 그래서 나중에 #9892를 이식할 때 충돌하지 않는다.
- 게임 ROM에는 영향이 없다.
- `fake_rtc.h`가 `global.h`를 직접 포함하게 하는 대안도 있지만, 게임 오브젝트 9개에 영향을 주므로 러너 파일만 고치는 쪽을 권장한다.
- **실행은 `make check BUILD=hns`로 한다.**
  - 기본 `make check`는 `BUILD_NAME=emerald`·`IS_HNS=0` 변형을 빌드한다.
  - 원작업자도 `make BUILD=hns TEST=1 …`로 테스트 오브젝트를 컴파일했다.
- 함께 넣을 것을 권장한다: #10665. 테스트 사이에 콜백·`gBattleTypeFlags`를 초기화하며, fwd ok다.
- 정적 확인 결과: src가 호출하는 `TestRunner_*` 훅 19개는 모두 `include/test_runner.h`·`test_runner_battle.c`에 정의돼 있다. 러너가 참조하는 함수 이름 260개도 모두 존재한다.

### 다음 장애 후보 (빌드하지 않은 추정 — 가능성 높음)

- **테스트 ELF의 ROM 초과**
  - TEST 빌드에는 HnS가 켠 `-ffunction-sections -fdata-sections`가 빠진다(`Makefile`의 `else ifneq ($(TEST),1)`).
  - 테스트 링크는 `--gc-sections` 없이 `ld_script_test.ld`(ROM 32M, `dacs` 섹션 0x9FFC000 고정)를 쓴다.
  - 그 결과 게임 부분만 약 33.33MB(function-sections 전 기준 99.33%)이고, `dacs` 앞 여유는 약 200KB다.
  - 여기에 테스트 944파일(정의 약 5.7천 개, compression 테스트 INCBIN 740개)이 들어가야 해서 수 MB가 부족할 것으로 보인다.
- **대책 후보**
  - (a) TEST 빌드에도 function/data sections와 `--gc-sections`를 적용한다. `ld_script_test.ld`의 `src/*.o(.text)`·`test/*.o(.text)`를 `.text*`로 바꾸고 `.tests`·`.dacs`·ROM 헤더는 KEEP한다. 약 0.58MB를 되찾지만 부족할 수 있다.
  - (b) `TEST_SRCS` 필터로 폴더 단위 부분 링크를 한다. 예: `test/battle/move_effect`만.
  - 필요하면 별도 unit(`U-testrom-size`)으로 분리한다.

### 복구 후 영문 메시지 기대값 때문에 실패할 테스트 추정 (정적 스캔)

| 항목 | 현재 HnS test/ | 참고: 1.17.0 test/ |
|---|---:|---:|
| 테스트 파일 | 944 | 966 |
| 실행 대상 정의(TO_DO 제외) | 5,117 (배틀형 4,271 + 함수형 846) | 5,816 |
| **양성 MESSAGE/SEND_IN_MESSAGE/SWITCH_OUT_MESSAGE 포함 → 실패 예상** | **2,131 (배틀형의 약 50%, 전체의 약 42%)** | 2,325 |
| NOT/NONE_OF 안에만 MESSAGE → 항상 통과(검출력 상실) | 200 | 231 |
| MESSAGE 없음 | 2,786 | 3,260 |

- **실패 이유**
  - 러너(`TryMessage`)는 공백 유연성만 두고 바이트 단위로 비교한다. 첫 불일치 MESSAGE에서 큐가 멈추므로, 그 뒤의 ABILITY_POPUP·HP_BAR 검사도 함께 Unmatched가 된다.
  - HnS 배틀 문자열은 706개 중 669개가 한글이고, 종명(1,403개 전부)·기술명도 한글이다. 사실상 모든 양성 MESSAGE가 실패한다.
  - 예외: `"GHOST: Get out…… Get out……"`처럼 미번역 FRLG 문자열에 종명이 없는 극소수만 일치한다.
- **폴더별 실패 예상**: move_effect 770, ability 671, hold_effect 157, gimmick 107, move_effect_secondary 62, sleep_clause 55, ai 55, item_effect 48, form_change 43, trainer_slides 39 등.
- HnS가 직접 추가·수정한 테스트 11개(teleport.c, break_screens.c, encore.c 등)도 영문 기대값이다.
- 영문 메시지와 별개로, HnS가 의도적으로 바꾼 출력(BATTLE_MESSAGE_OUTPUT_CHANGES)과 `GEN_CHAMPIONS` 설정 때문에 추가로 실패하는 테스트도 있을 것이다. 이 수는 실행해 봐야 알 수 있다.

## 6. ROM·세이브 위험

- **ROM**
  - 이 그룹 `이식` 전체 추정은 약 +12.7KB다(현재 여유는 function-sections 도입 후 약 800KB).
  - 큰 항목: #10141 +4.0KB, #9124 +2.5KB, #10252 +1.2KB, #8664·#9551 각 +0.8KB, #9568 +0.6KB, #8472 +0.5KB, #10206 +0.4KB, 팔레트·back 복구 +0.2KB.
  - #9987은 설정을 FALSE로 유지하면 0이다. 켜면 32MiB를 초과한다(SESSION_LOG 1675 기록 "923,860B 초과").
- **EWRAM(현재 95%)**
  - #9709: `BattlePokemon` +u8 1개. gBattleMons와 복사본에서 수~16B이며, 패딩에 들어가면 0이다.
  - #9568: AiLogicData 힙 +128B, 교체 계산 저장본까지 치면 +256B다.
  - #9596: −4B.
  - 이식할 때마다 링크 맵으로 실측해야 한다.
- **세이브**
  - save_impact=Y는 0건이다.
  - #9507(Species enum)은 세이브 구조체 필드 타입을 packed enum으로 바꾸지만 크기·정렬은 그대로다. 이식 후 `sizeof` STATIC_ASSERT로 확인한다.
  - #9709 이후에는 구버전 ROM과 링크 배틀 전송 구조가 호환되지 않는다.
- **테스트 ELF**: 5절의 ROM 초과가 러너 복구의 두 번째 장애가 될 가능성이 높다. 게임 ROM과는 무관하다.

## 7. 불확실 항목

1. **테스트 ELF의 ROM 초과 규모**
   - 테스트 코드·데이터 크기는 빌드하지 않고 추정했다. `fake_rtc` 수정 뒤 다른 컴파일 오류가 있는지도 빌드해 봐야 확정된다.
   - 러너 참조 심볼은 이름 수준에서만 확인했고, 구조체 필드는 확인하지 않았다.
2. **MESSAGE 스캔 정밀도**
   - 정규식 기반이다. 주석은 제거했고 NOT/NONE_OF 블록을 추적했다.
   - 매개변수화된 테스트는 정의 단위로 셌다. 조건부 MESSAGE(`if` 안)는 양성으로 셌다.
3. **#10453**: HnS `AI_CalcDamage`의 Nature Power 분기에 upstream에 없는 `aiCalc->move = move;`가 있다. 이 때문에 명중률이 변환된 기술 기준으로 계산된다. 의도 기록이 없어 유지할지 결정이 필요하다.
4. **#10258**: 12v12(#8943) 전 HnS 구조에 맞춰 적응한 부분(`BattleSideHasTwoTrainers` 대신 배틀 타입 플래그)이다. #8943·#10039를 이식할 때 다시 봐야 한다.
5. **#9857**
   - 동작 변화(예측 기술 필터 제거)는 Prediction 트레이너 25명에 한정된다. 난이도 의도 기록이 없어 외부결정으로 보지 않았다.
   - 정정: 어제 보고서와 달리 `CanAiPredictMove`는 HnS가 추가한 것이 아니다. merge-base 코드 그대로다.
6. **#9124**: 챌린지 모드의 구 시트러스 30 고정 회복(`battle_ai_switch.c:1862`), `HOLD_EFFECT_CONFUSE_FLAVOR`, Supreme Overlord 어댑트를 새 함수로 옮겨야 한다.
7. **#10399**: HnS는 firstId~lastId 구조다. 폴백 조건을 `<firstId`로 적응해야 하고, 교체 예정 몬 중복 검사도 잔여분이다(Ace 트레이너 0명).
8. **#10424 / #10427**: #10289(g1)와 함께 이식해야 오래된 `wrappedBy`를 읽는 문제가 해결된다. AI 자신의 조임밴드는 upstream도 반영하지 않는다.
9. **#7360**: 효과음 논리를 공격 애니 판정 호출부(`:1553`)에도 적용할지(upstream 원형) 결정이 필요하다. 1.17.0은 #10362에서 두 판정을 분리했다. 효과음 경로에만 먼저 적용하고 더블 범위기를 실기로 확인하는 것을 권장한다.
10. **#9507**: 실패 파일 41개 가운데 표본 외 35개 파일에 upcoming 의존이 숨어 있을 가능성을 배제하지 못했다.
11. **그래픽**: 팔레트 불일치 색 버그와 텅구리 알로라 조합은 PNG 분석으로만 확인했다. mGBA 화면 확인이 필요하다.
12. **AI 사고 시간 상한**(#9779/#10046/#10277/#8647): 러너 복구 후 HnS에서 다시 측정해야 한다.
