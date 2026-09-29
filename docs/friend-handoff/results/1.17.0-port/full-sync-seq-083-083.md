# full-sync 실제 port 결과 — seq 83~83

진행 중: 마지막 완료 seq 83(#8497 커밋 `f59f50ca17`), 다음: 같은 unit의 #9595(seq 110) → #10345(seq 265) → #10589(seq 321) → 전체 테스트

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
  - **upstream 수동 수정 5곳도 반영:** upstream 부모 파일에 같은 스크립트를 돌린 결과와 upstream #8497 결과를 비교해 스크립트 밖 수정 5곳을 찾아 같은 위치에 넣었다 — GhostGetOut `delay 1`→`2`, Punishment 두 번째 타격 전 `delay 1`, Poltergeist 끝 `unloadspritegfx/unloadspritepal ANIM_TAG_ITEM_BAG` 삭제, 10만볼트 `unloadallspritepals`, Extreme Evoboost 배경 전환을 `unloadspritepal ANIM_TAG_LEER` 뒤 `delay 1` 다음으로 이동.
  - **HnS 스크립트 차이 보존 확인:** "이식 전 HnS ↔ upstream 부모" diff와 "이식 후 HnS ↔ upstream #8497" diff의 변경 줄이 완전히 같다. 즉 HnS 차이 4종(HealingEffect 호출 4곳 없음, Bite 이빨 `x=-33`(#9564), SecretPower `end`, 메가·원시회귀 `AnimTask_BlendParticle` 3줄(#10324))이 그대로 남았다.
  - **C 코드:** upstream hunk 전부 그대로 적용(문맥 충돌 없음). `src/battle_anim.c`의 HnS 추가분(`monbg` 중복 assert, 배경 타일맵 버퍼 크기 보정)과 HnS `CreateSpriteUnchecked` 사용부(`battle_anim_electric.c`)는 hunk 밖이라 그대로다. 파일 모드(100755)는 바뀌지 않았다.
  - **HnS 추가(upstream과 다름) 2곳:** 아래 순서 검사에서 upstream 1.17.0에도 남아 있는 회귀 2건을 찾아 #10589와 같은 방식으로 막았다.
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
    - 범위 밖 쓰기 위험: AuroraBeam(`RAINBOW_RINGS`)·MagicalLeaf(`LEAF`/`RAZOR_LEAF`)·NightSlash(`SLASH`) → **#10589**가 `TryLoadPal`로 해결. WingAttack·SteelWing·HydroVortex·BloomDoom·DoubleIronBash·VeeveeVolley의 바람 회오리 팔레트(`GUST`) → **#10345**가 조회를 스프라이트 생성 뒤 프레임으로 옮겨 해결(바로 다음 줄들에서 GUST 템플릿 스프라이트가 만들어짐). HealBell(`MUSIC_NOTES_2`) → **HnS 추가 1**로 해결.
    - 시각 차이: 음표 무지개 블렌드 5개 애니 → **HnS 추가 2**로 해결. `AnimUproarRing` 콜백을 쓰는 9개 애니(NightDaze·NaturesMadness·OriginPulse·Electrify·SimpleBeam·FusionFlare·MagneticFlux·RevelationDance·ZingZap)는 THIN_RING 그림에 다른 팔레트를 쓰는 템플릿이라 블렌드 대상인 THIN_RING 팔레트를 쓰는 스프라이트가 그 애니에 없다 → 화면 차이 없음.
    - `AnimTask_MoonlightEndFade`의 `GREEN_SPARKLE`(Moonblast·MaxStarfall·GMaxSmite·GMaxFinale): 팔레트 번호 0xFF로 `0x10000 << 0xFF`를 계산한다. 이 네 애니에는 그 팔레트를 쓰는 스프라이트가 없어 화면 차이는 없고, 같은 계산이 이식 전에도 HealingWish(`MOON`)·LunarBlessing·MoongeistBeam·RevivalBlessing(`GREEN_SPARKLE`)에서 이미 일어나고 있었다(ARM 레지스터 시프트 32 이상 = 0 → 페이드 마스크에서 빠질 뿐). upstream 1.17.0과 같아 **손대지 않고 기록만 한다.**
    - 추가 후 결과: 회귀 0(위 GUST·오로라빔·매지컬리프·깜짝베기는 #10345·#10589 커밋 뒤 기준), 이식 전부터 있던 것 4건(위 MoonlightEndFade).
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
    - `Z-Moves animations work` 17/37 INVALID(`Cannot turn … into a Z-Move`): 0부터 센 17번 = 18번째 매개변수 `MOVE_TWINKLE_TACKLE`(문포스 + 페어리Z). HnS `GetMoveType()`은 도전 설정 `tx_Mode_Fairy_Types`가 0이면 페어리 기술을 다른 타입으로 바꾸는데(문포스 → 악), 테스트 러너는 이 설정을 0으로 두므로 페어리Z로 변환할 수 없다. 역시 **HnS 설정 차이**이며 애니와 무관하다. INVALID로 테스트가 끝나 나머지 Z기술 19~37번 애니는 이 테스트에서 돌지 않았다(아래 추가 확인 참고).
  - 실기 확인: 필요(아래 구간 끝 목록).
- 남은 위험: upstream `CreateSpriteAt`의 테스트용 검사 조건 `tileTag > ANIM_SPRITES_START && tileTag < ANIM_TAG_COUNT`는 `ANIM_TAG_COUNT`가 413(태그 개수)이라 항상 거짓이다(1.17.0도 같음). 그래서 upstream 테스트는 "로드 안 된 그림/팔레트로 스프라이트 생성"을 실제로 잡지 못한다. 그대로 이식했고, 검증은 위 정적 분석으로 했다.
