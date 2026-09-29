# full-sync 실제 port 결과 — seq 83~83

완료: seq 83(#8497 unit, 같은 unit의 seq 110 #9595·seq 265 #10345·seq 321 #10589 포함) 이식·전체 테스트·기록 완료. 다음 구간은 seq 84부터. 아래 "seq 83 요약" 참고.

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), [`CLAUDE_FULL_SYNC_PORT_PROMPT.md`](../../CLAUDE_FULL_SYNC_PORT_PROMPT.md)
시작 HEAD: `152910621b`

## seq 83 공통 사항

- 빌드 명령: `PATH=/opt/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin:$PATH GITHUB_ACTION=1 make hns -j8 > build/port.log 2>&1` (노트북 WSL, 8코어).
- 기준 빌드(`152910621b`, `rm -rf build/hns` 뒤 전체 재빌드, 약 60초): 종료 코드 0, ROM 32,717,172 B(97.50%), EWRAM 248,908 B(94.95%), IWRAM 25,516 B(77.87%). 경고 줄 166개, "파일: 메시지"(줄 번호 제거) 고유 목록 44개. ROM SHA-1 `1d57bfdf33f1115161cbfcf34733123ea6dc64d1`(직전 구간 최종 ROM과 같음).
- 경고 비교: 매 빌드의 경고를 같은 형식으로 만들어 기준 목록과 비교하고 새 경고만 확인했다.
- 테스트 기준: [`test-baseline-seq082.txt`](test-baseline-seq082.txt)(PASS 2,295 / FAIL 2,226 / TOTAL 5,191). 알려진 예외: AI 더블 테스트 3건(`AI_REVERSE_BATTLER_LOGIC_ORDER_CHANCE 50`, seq 88·131에서 해소 예정).
- 한글 포함 소스 줄: 커밋마다 `git show <커밋> | grep -aP '^[-+](?![-+]).*[^\x00-\x7F]'`로 확인했다.
- 애니 스크립트 분석 도구(스크래치 `a8497/`, 재생성 가능):
  - `animtags.py`: 스크립트(매크로 전개 포함)를 파싱해 `gBattleAnim*` 진입점 1,022개마다 도달 가능한 명령(`goto`·`call`·`jump*`·`choosetwoturnanim` 양쪽, `.if` 양쪽)을 모으고, ELF 심볼 + ROM에서 스프라이트 템플릿의 `tileTag`·`paletteTag`·콜백을 읽는다. C 쪽 `TryLoadSpriteAssets`·`TryLoadPal`·`TryLoadGfx`·`Store*Tag` 호출을 함수별로 파싱해 "이식 후 자동 로드되는 태그"를 계산한다. `gBattleAnimTable`(ROM)에서 태그별 그림·팔레트 항목이 유효한지(`pic.tag == tag`, `palette.tag == tag`, 데이터 포인터 ≠ 0)도 확인한다.
  - `animorder.py`: 순서 기반 검사. 각 진입점의 가능한 모든 실행 경로를 따라가며 "태그로 팔레트를 찾는 C 코드"(`IndexOfSpritePaletteTag(ANIM_TAG_…)` 등 17곳)가 실행되는 시점에 그 팔레트가 로드돼 있는지를 이식 전(loadspritegfx/unloadspritegfx)과 이식 후(자동 로드/unloadspritepal/unloadallspritepals)로 비교한다. 경로별 추적 슬롯 수(그림·팔레트 각 8개 한도)도 센다.
  - `anim/animcalls.py`(직전 구간 도구): 호출 깊이, 호출 안 `end`, 빈 스택 `return`.

## 동기화 단위: seq 83 #8497 `U-anim-8497` Remove loadspritegfx

- 현재 판정: 적용(HnS 적응)
- 커밋: `f59f50ca17`
- upstream 근거: `947874ae49`(변환 스크립트 수정 #9511 `105c69c575`은 참고만)
- 해결한 의존성: 선행 #9142·#9473은 seq 75 커밋 `125e893903`에 들어 있다.
- 수정 파일: `asm/macros/battle_anim_script.inc`(0x00 `unloadspritegfx`, 0x01 `unloadspritepal`, 0x35 `unloadallspritepals`), `data/battle_anim_scripts.s`, `include/battle_anim.h`, `include/constants/battle_anim.h`(`ANIM_SPRITE_GFX_COUNT`·`ANIM_SPRITE_PAL_COUNT` 8), `include/global.h`, `migration_scripts/1.16/remove_loadspritegfx.py`(새 파일), `src/battle_anim.c`, `src/battle_anim_{effects_1,effects_2,effects_3,electric,fight,fire,flying,ghost,ice,mons,new,normal,psychic,rock,status_effects,utility_funcs,water}.c`, `src/battle_script_commands.c`, `src/sprite.c`, `test/battle/move_animations/all_anims.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - **스크립트 변환:** upstream 결과 파일을 복사하지 않고 HnS `data/battle_anim_scripts.s`(이식 전)에 #8497판 `remove_loadspritegfx.py`를 돌렸다. `loadspritegfx` 2,351줄 삭제, `unloadspritegfx` 155줄 뒤에 같은 태그의 `unloadspritepal` 추가. 이식 전 파일의 `loadspritegfx` 2,506회는 전부 탭으로 시작하는 명령 줄(로드 2,351 + 언로드 155)이고 주석 줄에는 없다. 별도로 짠 정규식 변환기와 결과가 주석 복사 여부(81줄)만 다르다.
  - **#9511판 스크립트는 쓰지 않았다:** HnS 파일에 돌려 보면 연속된 `unloadspritegfx` 두 줄 중 뒷줄을 지운다(`"loadspritegfx" in line2` 조건이 `unloadspritegfx`에도 걸림). 결과가 #8497판과 108줄 다르다. 저장소에 넣은 마이그레이션 스크립트도 #8497판 그대로다(#9511은 group plan상 "무관", 이식하지 않음).
  - **upstream 수동 수정 5곳도 반영:** upstream 부모 파일에 같은 스크립트를 돌린 결과와 upstream #8497 결과를 비교해 스크립트 밖 수정 5곳을 찾아 같은 위치에 넣었다 — GhostGetOut `delay 1`→`2`, Punishment 두 번째 타격 전 `delay 1`, Poltergeist 끝 `unloadspritegfx/unloadspritepal ANIM_TAG_ITEM_BAG` 삭제, 1000만볼트 `unloadallspritepals`, Extreme Evoboost 배경 전환을 `unloadspritepal ANIM_TAG_LEER` 뒤 `delay 1` 다음으로 이동.
  - **HnS 스크립트 차이 보존 확인:** "이식 전 HnS ↔ upstream 부모" diff와 "이식 후 HnS ↔ upstream #8497" diff의 변경 줄이 완전히 같다. 즉 HnS 차이 4종(HealingEffect 호출 4곳 없음, Bite 이빨 `x=-33`(#9564), SecretPower `end`, 메가·원시회귀 `AnimTask_BlendParticle` 3줄(#10324))이 그대로 남았다.
  - **C 코드:** upstream hunk 전부 그대로 적용(문맥 충돌 없음). `src/battle_anim.c`의 HnS 추가분(`monbg` 중복 assert, 배경 타일맵 버퍼 크기 보정)과 HnS `CreateSpriteUnchecked` 사용부(`battle_anim_electric.c`)는 hunk 밖이라 그대로다. 파일 모드(100755)는 바뀌지 않았다.
  - **HnS 추가(upstream과 다름) 2곳(이 커밋) + 1곳(#10345 커밋):** 아래 순서 검사에서 upstream 1.17.0에도 남아 있는 회귀를 찾아 #10589와 같은 방식(`TryLoadPal`)으로 막았다. 세 번째(`AnimTask_AnimateGustTornadoPalette`, BloomDoom·HydroVortex)는 upstream #10345가 같은 함수를 고치므로 그 커밋에 넣었다.
    - `AnimTask_LoadMusicNotesPals`(HealBell): 스크립트에서 음표 스프라이트보다 먼저 실행되어 `ANIM_TAG_MUSIC_NOTES_2` 팔레트가 없을 때 `IndexOfSpritePaletteTag`가 0xFF를 돌려주고 `LoadPalette(…, OBJ_PLTT_ID(0xFF) = 0x10F0, …)`로 팔레트 버퍼(512색) 밖 EWRAM 32바이트×2를 덮어쓴다. 함수 앞에 `TryLoadPal(ANIM_TAG_MUSIC_NOTES_2)`(실패 시 작업 종료)를 넣었다.
    - `AnimTask_MusicNotesRainbowBlend`(Sing·BellyDrum·GrassWhistle·RelicSong·Round): 음표 팔레트가 아직 없어 첫 색(흰색→분홍) 블렌드가 빠진다(0xFF 검사가 있어 메모리 문제는 없음). 함수 앞에 `TryLoadPal(gParticlesColorBlendTable[0][0])`를 넣었다(실패해도 기존 검사로 진행).
  - 문자열·STRINGID·조사·배틀 메시지 무관.
- **HnS 전용 그래픽 태그 검증(필수 항목):**
  - HnS 전용 태그·그림: **0개.** `include/constants/battle_anim.h`와 `src/data/battle_anim.h`(태그→그림/팔레트 표 `gBattleAnimTable`)가 upstream #8497 부모와 줄 단위로 같다. HnS의 GS볼 입자 태그(`TAG_PARTICLES_GS_BALL` 65058)는 볼 입자 전용 표(`sBallParticles`)로 따로 로드되며 애니 태그 범위(10000~10412) 밖이다. HnS 스크립트의 애니 수·`loadspritegfx` 수(2,506)도 upstream 부모와 같다.
  - 이전 태그: `loadspritegfx` 2,351개, 로드가 있는 진입점 942개, (진입점, 태그) 2,452쌍, 서로 다른 태그 **304개**.
  - 자동 로드 경로 해석: 304개 모두 `gBattleAnimTable`에서 그림·팔레트가 유효하다 → **해석 실패 0.** 이식 후 스크립트의 `createsprite*`가 쓰는 모든 템플릿의 `tileTag`·`paletteTag`도 0/`TAG_NONE`이거나 유효한 애니 태그다(범위 밖 태그로 표를 읽는 경로 0). **표에 추가한 HnS 항목: 없음.**
  - 애니별 "이전 로드 ⊆ 이후 자동 로드" 검사(진입점별 합집합 기준): 이전에 로드되던 2,452쌍 중 726쌍이 같은 종류(그림/팔레트)로는 자동 로드되지 않는다. 분류:
    - 248쌍: 그림은 안 쓰고 팔레트로만 쓰는 태그(`loadspritegfx ANIM_TAG_POISON_BUBBLE @Poison`처럼 색만 빌리던 로드). 팔레트는 템플릿 경로로 자동 로드된다.
    - 283쌍: 그림으로만 쓰고 팔레트는 다른 태그를 쓰는 템플릿. 필요 없는 팔레트를 더 이상 올리지 않는다.
    - 195쌍: 그 애니의 템플릿·C 로드 어디에도 안 쓰이는 태그. 애니가 부르는 C 함수(작업·스프라이트 콜백과 그 호출 폐포)가 그 태그 상수나 그 태그의 템플릿을 참조하는지 다시 확인해 **191쌍은 참조 0(원래 로드만 하고 쓰지 않던 태그)**, 4쌍은 `AnimTask_MoonlightEndFade`의 `ANIM_TAG_GREEN_SPARKLE` 팔레트 번호 조회(Moonblast·MaxStarfall·GMaxSmite·GMaxFinale, 아래 순서 검사 참고)다.
    - 참조 0인 191쌍(73개 태그, 괄호는 애니 수): IMPACT(26) BLUE_STAR(14) SMALL_EMBER(7) POISON_BUBBLE(6) ICE_CRYSTALS(6) CLAW_SLASH(6) SMALL_BUBBLES(5) HANDS_AND_FEET(5) ELECTRICITY(5) SPARK_2(4) SPARK(4) ROUND_SHADOW(4) ROCKS(4) ITEM_BAG(4) ELECTRIC_ORBS(4) CIRCLE_OF_LIGHT(4) SPEED_DUST(3) ORBS(3) GRAY_SMOKE(3) DUCK(3) BLACK_BALL_2(3) WHITE_FEATHER(2) WATER_IMPACT(2) TORN_METAL(2) THOUGHT_BUBBLE(2) SPARKLE_6(2) SPARKLE_2(2) PINK_HEART(2) LEER(2) FLOWER(2) FLAT_ROCK(2) EYE_SPARKLE(2) ECLIPSING_ORB(2) BUBBLE(2) BLUE_ORB(2) BLUE_LIGHT_WALL(2) BLACK_BALL(2) 그리고 1개씩: WOOD_HAMMER_HAMMER WHITE_CIRCLE_OF_LIGHT WHIRLWIND_LINES WEB_THREAD WATER_ORB WATER_GUN VERTICAL_HEX THIN_RING TAG_HAND SWEAT_DROP SPLASH SHOCK_3 RAZOR_LEAF QUICK_GUARD_HAND PURPLE_HAND_OUTLINE PUNISHMENT_BLADES POISON_JAB PINK_PETAL PINK_CLOUD MUSIC_NOTES METEOR LIGHTNING LEAF JAGGED_MUSIC_NOTE ICE_SPIKES GUST GREEN_SPIKE GREEN_SPARKLE FOCUS_ENERGY FLYING_DIRT EXPLOSION_6 CUT BREATH BLACK_SMOKE ASSURANCE_HAND ACUPRESSURE. (애니별 목록은 스크래치 `a8497/out/classB.tsv`)
  - C에서 직접 만드는 스프라이트: C 로드를 빼고 스크립트 템플릿만으로 다시 계산하면 115쌍이 추가로 "안 쓰임"이 되는데, 모두 upstream이 해당 작업·콜백에 넣은 `TryLoadSpriteAssets`(우박·비·눈 입자, 전기 볼트·충전 입자·볼트태클 계열, 에어컷터, 물 계열, 연속펀치 타격, 길동무·다크홀, 동결 얼음, 째려보기, 원념 불꽃, 하트스웝·스킬스웝, 봉인, 이온, 리프블레이드, 충격파, 폴터가이스트, 구르기 흙·바위, 트집 등)로 해결된다. 변수 템플릿을 쓰는 곳(충전 입자 `MOVE_FLASH_CANNON/STEEL_BEAM`, 볼트태클 `FAIRY_LOCK/COLLISION_COURSE`, 길동무 `DARK_VOID/POLTERGEIST`)은 로드하는 템플릿과 생성하는 템플릿의 선택 조건이 같음을 확인했다. 충격파 진행 볼트는 로드용(`gVoltTackleBoltSpriteTemplate`)과 생성용(`gShockWaveProgressingBoltSpriteTemplate`) 템플릿이 다르지만 태그가 둘 다 SPARK(10001/10001)로 같다.
  - **순서 검사(`animorder.py`, 최종 unit 상태 기준):** 태그로 팔레트를 찾는 C 코드 17곳 × 진입점 조합 408건 중 이식 전에는 로드돼 있었는데 이식 후 그 시점에 없는 곳:
    - 범위 밖 쓰기 위험: AuroraBeam(`RAINBOW_RINGS`)·MagicalLeaf(`LEAF`/`RAZOR_LEAF`)·NightSlash(`SLASH`) → **#10589**가 `TryLoadPal`로 해결. WingAttack·SteelWing·DoubleIronBash·VeeveeVolley의 바람 회오리 팔레트(`GUST`) → **#10345**가 조회를 스프라이트 생성 뒤 프레임으로 옮겨 해결(같은 프레임에 GUST 팔레트 템플릿 스프라이트가 만들어짐). HydroVortex·BloomDoom은 GUST **그림**에 다른 팔레트(WATER_ORB·RAZOR_LEAF)를 쓰는 템플릿뿐이라 GUST 팔레트가 끝까지 로드되지 않아 #10345 뒤에도 단계마다 `OBJ_PLTT_ID(0xFF)` 위치를 `memmove`로 덮어쓴다(upstream 1.17.0도 같음) → **HnS 추가 3**(#10345 커밋, 작업 시작 때 `TryLoadPal(ANIM_TAG_GUST)`)으로 해결. HealBell(`MUSIC_NOTES_2`) → **HnS 추가 1**로 해결.
    - 시각 차이: 음표 무지개 블렌드 5개 애니 → **HnS 추가 2**로 해결. `AnimUproarRing` 콜백을 쓰는 9개 애니(NightDaze·NaturesMadness·OriginPulse·Electrify·SimpleBeam·FusionFlare·MagneticFlux·RevelationDance·ZingZap)는 THIN_RING 그림에 다른 팔레트를 쓰는 템플릿이라 블렌드 대상인 THIN_RING 팔레트를 쓰는 스프라이트가 그 애니에 없다 → 화면 차이 없음.
    - `AnimTask_MoonlightEndFade`의 `GREEN_SPARKLE`(Moonblast·MaxStarfall·GMaxSmite·GMaxFinale): 팔레트 번호 0xFF로 `0x10000 << 0xFF`를 계산한다. 이 네 애니에는 그 팔레트를 쓰는 스프라이트가 없어 화면 차이는 없고, 같은 계산이 이식 전에도 HealingWish(`MOON`)·LunarBlessing·MoongeistBeam·RevivalBlessing(`GREEN_SPARKLE`)에서 이미 일어나고 있었다(ARM 레지스터 시프트 32 이상 = 0 → 페이드 마스크에서 빠질 뿐). upstream 1.17.0과 같아 **손대지 않고 기록만 한다.**
    - 추가 후 결과: 회귀 0(위 GUST·AuroraBeam·MagicalLeaf·NightSlash는 #10345·#10589 커밋 뒤 기준, MoonlightEndFade 4건은 화면 차이 없음으로 제외), 이식 전부터 있던 것 4건(위 MoonlightEndFade).
  - **슬롯 한도:** 경로별로 동시에 추적되는 태그 수 최대 그림 7 / 팔레트 8(한도 8, `unloadspritegfx`·`unloadspritepal`·`unloadallspritepals` 반영) → 한도 초과로 `failed to store gfx/pal` assert가 날 경로 0. 진입점 합집합으로는 테라버스트(27/26)·트랩(12/12) 등 13개가 8을 넘지만 모두 서로 배타적인 분기 합계다.
  - 새로 로드되는 것(이전에 빠져 있던 것): EsperWing 마지막 `gPsychoCutSpriteTemplate`는 이식 전 `PSYCHO_CUT`을 로드하지 않아 그림이 없었는데 이제 자동 로드된다(화면 변화). 사파리 포켓몬스낵 던지기(`gBattleAnimGeneral_PokeblockThrow`)는 `AnimTask_LoadPokeblockGfx`가 직접 올린 그림을 `createsprite`가 추적 표 기준으로 한 번 더 올린다(같은 태그 타일 2벌, `AnimTask_FreePokeblockGfx`와 `end`가 한 벌씩 해제해 누수 없음, upstream과 같음).
- 해제 경로 확인:
  - `unloadspritegfx`/`unloadspritepal`은 추적 표에 있을 때만 해제하고 표에서 지운다. `end`는 표에 남은 것만 해제한다. `FreeSpriteTilesByTag`/`FreeSpritePaletteByTag`는 없는 태그에 대해 아무 것도 하지 않으므로, 스크립트가 해제한 뒤 `end`가 다시 해제하거나 C 작업(`AnimTask_FreeMusicNotesPals`, 볼 입자, 포켓몬스낵)이 먼저 해제한 태그를 `end`가 해제해도 이중 해제가 되지 않는다.
  - HealBell은 `AnimTask_FreeMusicNotesPals` 뒤에 음표 스프라이트를 만들지 않는다(추적 표에만 남은 태그로 재로드가 막히는 경로 없음).
  - 이 PR 단독 상태에서는 `waitforvisualfinish`마다 모든 애니 팔레트를 해제한다. 이것은 #9595가 되돌린다(같은 unit 바로 다음 커밋).
- **호출 구조 정적 검사(`animcalls.py`):** 이식 전후 모두 진입점 1,022개, `call` 2,725개, 최대 깊이 2, 호출 안 `end` 0, 빈 스택 `return` 0.
- 저장·ROM·그래픽 영향: ROM −4,752 B(32,717,172 → 32,712,420; 스크립트 3바이트×2,351 삭제 − `unloadspritepal` 155개 + 로더 코드), EWRAM +16 B(추적 표 u16×8 → u16×8 두 개; plan 추정 "+32 B"는 새 표 전체 크기), IWRAM 0. 세이브 무관. `loadspritegfx`는 한 번에 1프레임씩 기다렸으므로 애니 시작이 로드 수만큼(대개 1~5프레임) 빨라진다(upstream과 같음, 위 GhostGetOut·Punishment `delay` 보정 포함).
- 검증:
  - `git diff --check`: 통과. 한글이 든 소스 줄 변경 0.
  - `make hns -j8`: 종료 코드 0, ROM 32,712,420 B / EWRAM 248,924 B / IWRAM 25,516 B, 새 경고 0.
  - 자동 테스트: `test/battle/move_animations/all_anims.c`는 이식 전 전부 `#if T_SHOULD_RUN_MOVE_ANIM`(FALSE) 안이라 0건이었고, #8497이 가벼운 판 6건을 새로 연다. HnS 결과: PASS 3(`Move Animations work 3`·`4`, `Tera Blast animations work`), FAIL 2, INVALID 1.
    - `Move Animations work 1`(222/222)·`2`(221/221) FAIL: 사유는 둘 다 `Task_FreeAbilityPopUpGfx: task not freed`뿐이다(`gLoadFail`·`gSpriteAllocs` 단정은 통과). 러너는 매개변수별 누수 검사에서 실패해도 다음 매개변수로 계속 가고 마지막 번호로 보고한다. 임시 디버그 출력(커밋 안 함)으로 찾은 원인 기술은 쪼아대기(`MOVE_PLUCK` 365)·내던지기(`MOVE_FLING` 374)·벌레먹음(`MOVE_BUG_BITE` 450) — 상대의 오랭열매를 먹거나 도구를 던지는 기술이다. HnS는 기반 커밋부터 도구 팝업(`CreateItemPopUp`, `BattleScript_ItemPopUp_*`)을 쓰는데 이 테스트는 그 턴 직후 배틀을 끝내므로 팝업 그림 해제 작업이 남는다. upstream #8497 시점에는 도구 팝업이 없다. 애니 로드와 무관한 **HnS 테스트 환경 차이**로 분류한다. 끝쪽 기술(833~847)만 돌리면 네 테스트 모두 PASS.
    - `Z-Moves animations work` 17/37 INVALID(`Cannot turn … into a Z-Move`): 0부터 센 17번 = 18번째 매개변수 `MOVE_TWINKLE_TACKLE`(문포스 + 페어리Z). HnS `GetMoveType()`은 도전 설정 `tx_Mode_Fairy_Types`가 0이면 페어리 기술을 다른 타입으로 바꾸는데(문포스 → 악), 테스트 러너는 이 설정을 0으로 두므로 페어리Z로 변환할 수 없다. 역시 **HnS 설정 차이**이며 애니와 무관하다. INVALID로 테스트가 끝나 나머지 Z기술 19~37번 애니는 이 테스트에서 돌지 않았다(아래 "동적 검증"에서 설정을 켜고 37개 모두 확인).
  - 실기 확인: 필요(아래 구간 끝 목록).
- 남은 위험: upstream `CreateSpriteAt`의 테스트용 검사 조건 `tileTag > ANIM_SPRITES_START && tileTag < ANIM_TAG_COUNT`는 `ANIM_TAG_COUNT`가 413(태그 개수)이라 항상 거짓이다(1.17.0도 같음). 그래서 upstream 테스트는 "로드 안 된 그림/팔레트로 스프라이트 생성"을 실제로 잡지 못한다. 그대로 이식했고, 검증은 위 정적 분석으로 했다.

## 동기화 단위: seq 110 #9595 `U-anim-8497` Fix move anim pal blending being discarded (seq 83 unit에 포함)

- 현재 판정: 적용(group plan "#8497과 같은 unit으로 넣는다"). **seq 110은 도달 시 "이미 적용"으로 처리한다.**
- 커밋: `5121b83c94`
- upstream 근거: `c1e0532fe2`
- 수정 파일: `src/battle_anim.c`(`Cmd_waitforvisualfinish`에서 #8497이 넣은 `UnloadAllSpritePalettes()`와 주석 9줄 삭제)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용. 결과 `Cmd_waitforvisualfinish`가 이식 전 HnS(`152910621b`) 함수와 글자 단위로 같다(group plan "결과적으로 HnS 현재 코드와 같은 상태"). `UnloadAllSpritePalettes()`는 `unloadallspritepals` 명령(1000만볼트)에서만 쓰인다.
- 저장·ROM·그래픽 영향: ROM −48 B. `waitforvisualfinish` 뒤에도 애니 팔레트(블렌드 결과 포함)가 유지된다. 위 순서 검사·슬롯 한도 검사는 이 상태 기준이다.
- 검증:
  - `git diff --check`: 통과. 한글 줄 변경 0.
  - `make hns -j8`: 종료 코드 0, ROM 32,712,372 B / EWRAM 248,924 B / IWRAM 25,516 B, 새 경고 0.
  - 자동 테스트: `all_anims.c` 6건 결과가 #8497 직후와 같다(PASS 3, FAIL 2 = HnS 도구 팝업 태스크, INVALID 1 = HnS 페어리 타입 설정).
  - 실기 확인: #8497과 함께(팔레트 블렌드가 도중에 풀리지 않는지).
- 남은 위험: 없음

## 동기화 단위: seq 265 #10345 `U-anim-8497` fix(battle-anim): fix gust colour cycling animation (seq 83 unit에 포함)

- 현재 판정: 적용(HnS 적응 1줄 블록). group plan "#8497과 같은 unit으로 넣는다". **seq 265는 도달 시 "이미 적용"으로 처리한다.**
- 커밋: `45b27b9bce`
- upstream 근거: `651edaf8f6`
- 수정 파일: `src/battle_anim_flying.c`(`#include "sprite.h"`, `AnimTask_AnimateGustTornadoPalette`에서 팔레트 번호 캐시 삭제, `_Step`에서 매번 `IndexOfSpritePaletteTag(ANIM_TAG_GUST)`로 찾고 `memmove`로 회전)
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - upstream hunk 그대로 적용(문맥 충돌 없음). 회전 결과(색 1~8 오른쪽으로 한 칸, 8→1)는 이식 전 루프와 같다.
  - **HnS 추가 3:** `AnimTask_AnimateGustTornadoPalette` 앞에 `TryLoadPal(ANIM_TAG_GUST)`(실패 시 작업 종료). BloomDoom(`gBloomDoomHurricaneSpriteTemplate` 그림 GUST/팔레트 RAZOR_LEAF)과 HydroVortex(`gHydroVortexHurricaneSpriteTemplate` 그림 GUST/팔레트 WATER_ORB, `gWhirlpoolSpriteTemplate`)는 GUST 팔레트를 쓰는 스프라이트가 없어 #8497 뒤로 그 팔레트가 로드되지 않는다. 이식 전에는 `loadspritegfx ANIM_TAG_GUST`가 팔레트도 올렸으므로, 이렇게 해야 이식 전과 같이 (보이지 않는) GUST 팔레트를 회전하고 범위 밖 쓰기가 없다. 다른 GUST 애니(Gust·WingAttack·SteelWing·Hurricane·LeafTornado·DoubleIronBash·VeeveeVolley)는 이미 로드된 팔레트를 그대로 쓴다. 스크립트에서 GUST를 해제하는 곳은 없다.
  - 추가 뒤 순서 검사: GUST 회귀 0, 경로별 최대 추적 팔레트 8(한도 이내).
- 저장·ROM·그래픽 영향: ROM 변화 0(32,712,372 B, 코드 감소와 추가가 상쇄). 화면은 이식 전과 같아야 한다.
- 검증:
  - `git diff --check`: 통과. 한글 줄 변경 0.
  - `make hns -j8`: 종료 코드 0, ROM 32,712,372 B / EWRAM 248,924 B / IWRAM 25,516 B, 새 경고 0.
  - 자동 테스트: `all_anims.c` 6건 결과가 #8497 직후와 같다(PASS 3, FAIL 2 = HnS 도구 팝업 태스크, INVALID 1 = HnS 페어리 타입 설정).
  - 실기 확인: 바람일으키기·날개치기·강철날개·폭풍 회오리 색 순환, BloomDoom·HydroVortex(Z기술) 진행 중 화면·다음 동작 이상 없음.
- 남은 위험: 낮음

## 동기화 단위: seq 321 #10589 `U-anim-8497` Fix out-of-bounds palette writes in certain move animations (seq 83 unit에 포함)

- 현재 판정: 적용(group plan "#8497과 같은 unit으로 넣는다"). **seq 321은 도달 시 "이미 적용"으로 처리한다.**
- 커밋: `715c1b91fc`
- upstream 근거: `de3e43897e`
- 수정 파일: `src/battle_anim_effects_1.c`(`AnimTask_CycleMagicalLeafPal`에 `TryLoadPal(LEAF)`·`TryLoadPal(RAZOR_LEAF)`, `AnimTask_BlendNightSlash`에 `TryLoadPal(SLASH)`와 `OBJ_PLTT_ID` 표기), `src/battle_anim_water.c`(`AnimTask_RotateAuroraRingColors`에 `TryLoadPal(RAINBOW_RINGS)`), `src/util.c`(`BlendPalette` 경계 `assertf`)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 그대로 적용(문맥 충돌 없음). 문자열 무관.
  - `BlendPalette` 경계 검사는 HnS 디버그 빌드에서 "재개 가능한 크래시 화면 + 함수 종료", 테스트 빌드에서 INVALID다. 동적 오프셋으로 `BlendPalette`를 부르는 곳(72곳 중 14곳)을 확인했다. 태그 조회 결과를 그대로 쓰는 곳은 이 PR이 막는 NightSlash뿐이고, 나머지는 스프라이트 `paletteNum`(4비트)·배틀러 번호·가드된 인덱스라 범위를 벗어나지 않는다. HnS 전용 호출부는 없다.
- 저장·ROM·그래픽 영향: ROM +176 B(32,712,548 B). 세 애니의 팔레트 색 순환·블렌드가 이식 전과 같아진다.
- 검증:
  - `git diff --check`: 통과. 한글 줄 변경 0.
  - `make hns -j8`: 종료 코드 0, ROM 32,712,548 B / EWRAM 248,924 B / IWRAM 25,516 B, 새 경고 0.
  - 순서 검사(unit 최종 상태): 이식으로 생긴 범위 밖 쓰기 0. 남은 것은 `AnimTask_MoonlightEndFade` 4건(화면 차이 없음)과 `AnimUproarRing` 27건(해당 팔레트를 쓰는 스프라이트 없음)뿐이다.
  - 자동 테스트: 아래 "동적 검증"과 구간 끝 전체 테스트.
  - 실기 확인: 매지컬리프 잎 색 순환, 깜짝베기 붉은 베기, 오로라빔 고리 색 순환.
- 남은 위험: 낮음(`BlendPalette` assert가 이전에 조용히 넘어가던 범위 밖 호출을 크래시 화면으로 드러낼 수 있으나 해당 경로를 찾지 못함)

## unit 최종 상태 동적 검증 (임시 계측, 커밋 안 함)

upstream 테스트가 실제로 잡지 못하는 부분을 보완하려고, unit 최종 코드(`715c1b91fc`)에 아래 임시 수정을 넣고 `all_anims.c`를 돌린 뒤 파일을 원래대로 되돌렸다(작업 트리 clean 확인).

- 임시 수정: ① `CreateSpriteAt` 검사 조건을 `ANIM_SPRITES_START <= tag < ANIM_SPRITES_START + ANIM_TAG_COUNT`로 고치고 assert 대신 기록만, ② `gLoadFail`(타일 할당 실패) 설정·초기화를 `TESTING`으로(upstream은 seq 398 #10274에서 이렇게 바꿈), ③ 테스트 러너에서 `tx_Mode_Fairy_Types = 1`(페어리 타입 켬), ④ 애니 실행 중 애니 태그로 `IndexOfSpritePaletteTag`/`GetSpriteTileStartByTag`가 실패하면 호출 주소와 함께 기록, ⑤ `LoadSpritePalette`/`AllocSpritePalette`가 16칸 부족으로 실패하면 그때의 팔레트 태그 표 기록, ⑥ 매 실행 끝에 기술 번호와 `gLoadFail`·`gSpriteAllocs` 기록.
- 범위: 싱글배틀 기술 1~847 전부(변형 포함 891회), Z기술 37개, 테라버스트 19타입 — 모두 947회. 결과 PASS 4 / FAIL 2(위와 같은 HnS 도구 팝업 태스크). 페어리 설정을 켜면 `Z-Moves animations work`도 PASS.
- **로드 안 된 그림/팔레트로 스프라이트 생성: 0건.** `gLoadFail`(타일 부족) 0건, 실행마다 `gSpriteAllocs` 0(타일 할당·해제 균형).
- 애니 중 태그 조회 실패(호출자별, 괄호 = 기술 번호):
  - `AnimTask_MoonlightEndFade` GREEN_SPARKLE(585 문포스, 668, 777, 791)·MOON(361) — 정적 검사와 같다. 668·777·791·361은 이식 전부터.
  - `AnimUproarRing` THIN_RING 9개 기술 — 정적 검사와 같고 화면 차이 없음.
  - `AnimSlowFlyingMusicNotes` BENT_SPOON·LARGE_FRESH_EGG(706 `MOVE_DRUM_BEATING`, Z 876·877): 음표 색 표 팔레트는 무지개 블렌드 작업이 따로 할당해야 생기는 태그인데 이 애니들은 그 작업을 부르지 않는다. 0xFF 검사가 있어 음표 자체 팔레트로 표시된다. 이식 전에도 `loadspritegfx`로 로드되지 않던 태그라 변화 없음.
  - `FreeSpritePaletteByTag` MUSIC_NOTES_2(215 방울소리): `AnimTask_FreeMusicNotesPals`가 먼저 해제한 태그를 `end`가 다시 해제하려다 없음 → 아무 것도 하지 않음(이중 해제 아님).
  - `CreateSpriteAt`·`FreeSpritePaletteByTag`의 IMPACT(547 `MOVE_RELIC_SONG`)·CIRCLE_OF_LIGHT(Z 867 1000만볼트)·LEAF·POISON_BUBBLE(Z 869 Extreme Evoboost): 아래 팔레트 16칸 소진.
- **OBJ 팔레트 16칸 소진(세 애니):** 테스트 싱글배틀에서 예약 4칸 + 전투 UI 6칸(태그 55039·55044·30004·55160~55162)을 빼면 애니가 쓸 수 있는 칸은 6칸이다. 이 한도를 넘으면 `TryLoadPal`은 추적 표에는 기록하지만 실제 로드는 실패하고, 그 팔레트의 스프라이트는 `paletteNum` 15(그때 15번 칸 팔레트 색)로 표시된다.
  - 1000만볼트(Z 867): CIRCLE_OF_LIGHT 1개 실패. **이식 전에는 시작 때 `loadspritegfx` 21개를 한꺼번에 올렸으므로** 같은 6칸 조건이라면 15개 안팎이 실패했을 것이다(추정, 이식 전 코드는 이 계측으로 돌리지 않음). upstream이 넣은 `unloadallspritepals`까지 포함해 크게 나아졌다.
  - Extreme Evoboost(Z 869): LEAF·POISON_BUBBLE 2개 실패. 이식 전에는 10개를 한꺼번에 올렸다(같은 조건이면 4개 안팎 실패, 추정).
  - RelicSong(547): 이식 전후 모두 7개가 필요해 1개가 실패하는데, 실패하는 쪽이 바뀐다. 이식 전에는 시작 때 4개(JAGGED_MUSIC_NOTE·THIN_RING·MUSIC_NOTES·IMPACT)를 먼저 올려 음표 무지개 셋째 색(LARGE_FRESH_EGG) 할당이 실패했고(일부 음표가 기본색), 이식 후에는 무지개 색 3개가 먼저 할당되어 마지막에 필요한 IMPACT(타격 이펙트)가 실패한다(타격 이펙트가 15번 칸 = 들쭉날쭉 음표 팔레트 색). upstream도 같은 순서다. 스크립트 순서를 HnS 단독으로 바꾸지 않고 **실기 확인 항목**으로 둔다.
  - 더블배틀은 UI 팔레트가 더 많을 수 있어 여유 칸이 더 적다. 다만 이식 후에는 쓰는 태그만 필요할 때 올리므로 전체적으로 이식 전보다 팔레트를 적게 쓴다.

## seq 83 요약

- 처리 범위: seq 83 한 행(#8497, XL). group plan에 따라 같은 unit `U-anim-8497`의 #9595(seq 110)·#10345(seq 265)·#10589(seq 321)를 바로 뒤 커밋으로 함께 넣었다(세 PR 모두 group plan이 "#8497과 같은 unit으로 넣는다"고 적은 #8497 회귀 수정). #9511(변환 스크립트 수정)은 group plan상 "무관"이라 넣지 않았다. seq 84 이후는 손대지 않았다.
- 시작 `152910621b` → 이식 마지막 커밋 `715c1b91fc`(그 뒤 기록 커밋). 이식 커밋 4개, 진행 기록 커밋 4개.

| seq | PR | 판정 | 커밋 | 비고 |
|---:|---|---|---|---|
| 83 | #8497 | 적용(HnS 적응) | `f59f50ca17` | 스크립트 기계 변환 + upstream 수동 수정 5곳, HnS `TryLoadPal` 2곳 추가 |
| 110 | #9595 | 적용(unit 선행 반영) | `5121b83c94` | `Cmd_waitforvisualfinish`가 이식 전 HnS와 같아짐 |
| 265 | #10345 | 적용(HnS 적응) | `45b27b9bce` | HnS `TryLoadPal(ANIM_TAG_GUST)` 추가(BloomDoom·HydroVortex) |
| 321 | #10589 | 적용(unit 선행 반영) | `715c1b91fc` | 그대로 |

- 마지막 빌드(`rm -rf build/hns` 뒤 전체 재빌드, HEAD `7f159191ff`, 코드 기준 `715c1b91fc`): 종료 코드 0, **ROM 32,712,548 B(97.49%) / EWRAM 248,924 B(94.96%) / IWRAM 25,516 B(77.87%)**, 경고 줄 166개·고유 44개(기준과 같음, 새 경고 0). ROM SHA-1 `09927f92473a555ef521349aee72e8367125452d`.
- 기준 대비: ROM −4,624 B, EWRAM +16 B(애니 태그 추적 표), IWRAM 0.
- 한글 포함 소스 줄 변경: 0(네 커밋 모두). 문자열·STRINGID·조사·배틀 메시지 출력 무관.
- 태그 검증 요약: 이식 전 `loadspritegfx` 태그 304종 전부 새 자동 로드 표에서 해석됨(실패 0), HnS 전용 태그 0, 표 추가 항목 없음. 이전 로드 ⊆ 이후 자동 로드가 아닌 쌍은 모두 분류됨(색만 빌림 248, 다른 팔레트 템플릿 283, 원래 안 쓰던 로드 191, 페이드 마스크 조회 4). C 생성 스프라이트 115쌍은 upstream `TryLoadSpriteAssets`로 모두 덮임. 순서 검사로 upstream 1.17.0에도 남은 범위 밖 쓰기 2건(HealBell, BloomDoom/HydroVortex GUST)과 음표 색 블렌드 1건을 찾아 HnS에서 막았다. 경로별 추적 슬롯 최대 그림 7 / 팔레트 8(한도 8).
- 동적 검증(임시 계측): 947회 실행에서 로드 안 된 그림/팔레트로 생성 0, 타일 할당 실패 0, 할당·해제 균형. 하드웨어 팔레트 16칸 소진은 1000만볼트·Extreme Evoboost·RelicSong에서만(앞 둘은 이식 전보다 줄어든 것으로 추정, RelicSong은 실패하는 팔레트가 음표 색 → 타격 이펙트로 바뀜).
- 호출 구조 정적 검사: 이식 전후 `call` 2,725, 최대 깊이 2, 호출 안 `end` 0, 빈 스택 `return` 0.

### upstream과 일부러 다르게 둔 곳 (이후 port 담당 참고)

| seq | PR | 내용 |
|---|---|---|
| 83 | #8497 | `AnimTask_LoadMusicNotesPals` 앞 `TryLoadPal(ANIM_TAG_MUSIC_NOTES_2)`(실패 시 작업 종료) — HealBell 범위 밖 `LoadPalette` 방지 |
| 83 | #8497 | `AnimTask_MusicNotesRainbowBlend` 앞 `TryLoadPal(gParticlesColorBlendTable[0][0])` — 음표 기본 색 블렌드 유지 |
| 83 | #8497 | 마이그레이션 스크립트는 #8497판 그대로(#9511판은 연속 `unloadspritegfx` 뒷줄을 지우는 문제가 있어 쓰지 않음, #9511 무관 판정) |
| 265 | #10345 | `AnimTask_AnimateGustTornadoPalette` 앞 `TryLoadPal(ANIM_TAG_GUST)`(실패 시 작업 종료) — BloomDoom·HydroVortex 범위 밖 `memmove` 방지 |

세 곳 모두 `//  HnS:` 주석이 붙어 있다. 이후 upstream PR이 이 함수들을 바꾸면 HnS 줄을 유지한다. upstream `CreateSpriteAt` 테스트 검사 조건(`< ANIM_TAG_COUNT`, 항상 거짓)은 그대로 두었다(upstream 1.17.0과 같음).

### 실기 확인 필요 (mGBA)

HnS에는 upstream에 없는 전용 기술 애니·태그가 없다(스크립트 차이는 Bite 좌표·SecretPower `end`·메가/원시회귀 입자 블렌드·HealingEffect 호출 없음 4종). 그래서 upstream과 같은 기술 애니 전체가 새 로드 방식으로 바뀐 것으로 보고 대표 항목을 확인한다. 가능하면 싱글·더블 둘 다.

1. 음표 계열: 방울소리(음표 3색·방울·고리, 멈춤·깨진 화면 없음), 노래하기·풀피리·돌림노래·배수의진(음표가 흰색~분홍 무지개 색인지), 옛날노래(타격 이펙트 색이 들쭉날쭉 음표 색으로 섞여 보이는지 — 알려진 변화 후보).
2. 바람 회오리 색 순환: 바람일으키기·날개치기·강철날개·폭풍·그래스믹서. Z기술 블룸샤인엑스트라(BloomDoom)·하이드로볼텍스(HydroVortex) 재생 뒤 화면·다음 행동 이상 없음.
3. 팔레트 순환·블렌드: 매지컬리프(잎 색 순환), 깜짝베기(붉게 블렌드되는 베기), 오로라빔(무지개 고리 회전), 문포스.
4. 파티클 색·팔레트 섞임: 메가진화·원시회귀 입자(HnS #10324 흰색 블렌드 입자), 전기 계열(10만볼트·전기쇼크·볼트태클 번개 조각), 물 계열(해수스파우팅·하이드로펌프 물방울), 날씨(비·싸라기눈·눈 입자), 얼음 계열 상태이상(얼음 큐브), 구르기 흙·바위.
5. Z기술 1000만볼트(10000000VoltThunderbolt)·나인에볼부스트(ExtremeEvoboost): 파티클 색이 엉뚱한 팔레트로 보이는 정도(이식 전보다 줄어야 함).
6. 폴터가이스트·Bestow 등 도구 아이콘 애니, 에스퍼윙(마지막 사이코커터 그림이 이제 보임).
7. 타이밍: 기술 애니 시작이 1~5프레임 빨라짐. 벌(Punishment, 두 번째 타격)·`GhostGetOut` 일반 애니·나인에볼부스트 배경 전환이 어색하지 않은지.
8. 콘테스트에서 기술 애니 몇 개(콘테스트는 예약 팔레트 수가 달라 팔레트 여유가 다름).
9. 메시지·한글: 변화 없음(확인 불필요).

### 다음 구간 담당 참고

- **이미 적용(재이식 금지):** seq 110 #9595(`5121b83c94`), seq 265 #10345(`45b27b9bce`), seq 321 #10589(`715c1b91fc`). 앞 구간에서 넣은 seq 94 #9549, 98 #9564, 138 #9707, 319 #10573, 330 #10648도 계속 유효.
- seq 102 #9172(애니 hex→10진, `U-9172`)의 선행 #8497이 충족됐다. #9172는 스크립트 기계 치환이므로, 이 구간처럼 upstream 부모에 치환을 돌린 결과와 upstream 결과를 비교해 수동 수정만 옮기는 방식을 권한다. HnS 스크립트 차이 4종과 이 구간의 HnS C 추가 3곳은 스크립트 치환과 겹치지 않는다.
- seq 356 #10027(도구 아이콘 애니)은 #8497 뒤라 `loadspritegfx ANIM_TAG_ITEM_BAG` 문맥 문제가 없어졌다. Poltergeist처럼 `AddItemIconSprite` 전에 `StoreGfxTag`/`StorePalTag`를 부르는지 확인.
- seq 398 #10274(애니 테스트 수정)에서 `gLoadFail`이 `TESTING`으로 바뀐다. 그때 `all_anims.c`의 HnS 전용 실패 2종(아래)이 어떻게 되는지 다시 볼 것.
- 새 기술 애니를 넣는 upstream PR(1.16 이후 전부 `loadspritegfx` 없음)은 이제 그대로 들어온다. 다만 C 작업이 `IndexOfSpritePaletteTag(ANIM_TAG_…)`로 팔레트를 찾는 경우, 그 시점에 템플릿으로 이미 로드됐는지(또는 `TryLoadPal`이 있는지) 확인한다. 스크래치 `a8497/animorder.py` 방식(순서 검사)이 유용하다.

### 전체 테스트 (구간 끝)

- 명령: `PATH=… make check BUILD=hns -j8 > build/port-check.log 2>&1`(코드 기준 `715c1b91fc`). 약 9분 43초.
- 결과: **PASS 2,298 / FAIL 2,229 / KNOWN_FAILING 8 / TO_DO 618 / EXPECT_FAILING 6 / ASSUMPTIONS_FAILED 38 / TOTAL 5,197**(기준 seq082: PASS 2,295 / FAIL 2,226 / TOTAL 5,191). assertion·crash 0.
- 새 기준 목록: [`test-baseline-seq083.txt`](test-baseline-seq083.txt)(5,128행, `LC_ALL=C`. PASS 2,295 / FAIL 2,204 / KNOWN_FAILING 8 / TO_DO 616 / EXPECTED_FAIL 5 — 이름 중복 제거 목록 기준).
- `test-baseline-seq082.txt` 대비 차이 전부와 원인:

| 테스트 | 기준 → 지금 | 원인 분류 |
|---|---|---|
| Move Animations work 3 / 4, Tera Blast animations work (`move_animations/all_anims.c`) | 새 테스트 → PASS | #8497이 가벼운 전체 애니 테스트를 새로 엶 |
| Move Animations work 1 (222/222), 2 (221/221) | 새 테스트 → FAIL | 사유 `Task_FreeAbilityPopUpGfx: task not freed`뿐. 쪼아대기·내던지기·벌레먹음에서 HnS 도구 팝업(기반 커밋부터 있음)이 배틀 종료 시점에 남는 **HnS 테스트 환경 차이**. 애니 로드 단정(`gLoadFail`·`gSpriteAllocs`)은 통과 |
| Z-Moves animations work (17/37) | 새 테스트 → INVALID(목록 형식 밖, 요약 FAIL 수에 포함) | HnS 도전 설정 `tx_Mode_Fairy_Types = 0`(테스트 기본)에서 문포스가 악 타입이 되어 페어리Z 변환 불가. 설정을 켜면 PASS(동적 검증에서 확인) |

- **회귀 판정:** 로직 회귀 0. 기준 목록의 PASS 2,295건(이름 중복 제거 2,292건)은 모두 그대로 PASS. 알려진 AI 더블 테스트 3건은 기준과 같은 상태(FAIL)다.
