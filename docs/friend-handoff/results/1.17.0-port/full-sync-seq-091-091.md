# full-sync 실제 port 결과 — seq 91 (+ 같은 unit의 seq 93)

진행 중: 마지막 완료 seq 91(#9507), 다음 seq 93(#9558, 같은 unit이라 바로 이어서 넣음) → 그다음 구간은 seq 92 #9429부터.

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), [`CLAUDE_FULL_SYNC_PORT_PROMPT.md`](../../CLAUDE_FULL_SYNC_PORT_PROMPT.md), group plan [`g4_ai_tests_pokemon_sprites_plan.md`](../1.17.0-sync-plan/g4_ai_tests_pokemon_sprites_plan.md)
시작 HEAD: `2e403781f3` (작업 트리 clean)

## seq 91 공통 사항

- 빌드 명령: `PATH=/opt/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin:$PATH GITHUB_ACTION=1 make hns -j8 > build/port.log 2>&1` (노트북 WSL, 8코어).
- 기준 빌드(`2e403781f3`, `rm -rf build/hns` 뒤 전체 재빌드, 48.7초): 종료 코드 0, ROM 32,712,612 B(97.49%), EWRAM 248,924 B(94.96%), IWRAM 25,516 B(77.87%). 경고 줄 166개, "파일: 메시지"(줄 번호 제거) 고유 목록 44개(직전 구간 기준 목록과 같음). ROM SHA-1 `9dcd5c1578cbf4a7b117e3681eb030478bb4742b`(직전 구간 최종 ROM과 같음). 이 빌드의 `build/hns` 오브젝트 1,524개와 `pokehns.gba`·`pokehns.map`·`pokehns.elf`를 스크래치에 복사해 두고 이식 후 비교에 썼다.
- 테스트 기준: [`test-baseline-seq090.txt`](test-baseline-seq090.txt)(PASS 2,298 / FAIL 2,229 / TOTAL 5,197). `2e403781f3`는 seq 90 커밋 `7b578d7bb2` 뒤에 문서만 바뀐 상태라(비문서 변경 0) 이 목록을 "이식 전" 결과로 쓴다. 추출 정규식에 없는 상태(INVALID·ASSUMPTIONS_FAILED 등)는 메인이 seq 90에서 돌린 전체 로그(`build/port-check.log`, 스크래치 사본)에서 모든 상태 줄(5,187줄)을 따로 뽑아 비교했다.
- 한글 포함 소스 줄: 커밋마다 `git show <커밋> | LC_ALL=C grep -a -cP '^[-+](?![-+]).*[^\x00-\x7F]'`.
- 스크래치 도구(`s91/`, 재생성 가능):
  - `vals/speciesvals.py`: 종 상수 값 표(C 경로·asm 경로). #9066 때 `constvals.py`와 같은 방식.
  - `layout/mktu.py`·`layout.py`·`cmp.py`: 구조체 배치 비교(DWARF).
  - `rejapply.py`·`rejfilter.py`: 거부 hunk의 줄 치환을 HnS 파일에서 찾아 적용(유일 일치만, 애매하면 보고). `hunkmap.py`: upstream과 HnS의 (둘러싼 함수, 바뀐 줄) 쌍 비교.
  - `sig/sigcmp.py`: HnS·upstream #9507 직후·1.17.0의 함수 시그니처에서 종 인자·반환형 비교.
  - `classify.py`(+ 직전 구간 `p84/objcmp.py`·`fndiff.sh`): 함수 단위 명령열 비교와 분류.

## 동기화 단위: seq 91 #9507 `U-species-enum-9507` Add Species enum

- 현재 판정: 적용(HnS 적응)
- 커밋: `b0a0fb9033`
- upstream 근거: `b4c311a308`(부모 `439b38b990` = #9539, seq 90과 같은 지점)
- 해결한 의존성: 없음. group plan대로 같은 unit의 #9558(seq 93)을 바로 뒤 커밋으로 넣는다.
- 수정 파일(151): upstream #9507의 148파일 전부 + `include/ow_abilities.h`(HnS 대응 헤더) + 테스트 2개(`test/battle/capture.c`, `test/battle/ability/mummy.c`, 아래 "테스트 빌드 수정").

### 적용 방법

- `include/constants/species.h`: HnS 파일이 upstream #9507 부모와 바이트 동일(모드만 100755)임을 확인했다. HnS 전용 종은 없다. `git apply` 결과가 upstream #9507 직후 파일과 바이트 동일하다(모드 100755 유지).
- upstream diff 148파일·725 hunk를 파일별로 나눠 `git apply --check`:
  - 113파일(368 hunk)은 그대로 적용했다. group plan의 "실패 41개"는 계획 당시 기준이고, 현재는 앞 구간 이식으로 35개만 실패했다.
  - 실패 35파일(357 hunk)은 스크래치 사본에 `patch -F0`(문맥 3줄 정확 일치, 위치 이동만 허용)으로 257 hunk를 넣고, 거부된 100 hunk는 HnS에서 같은 역할을 하는 줄(같은 함수, 또는 HnS가 이미 가진 후속 upstream 형태의 대응 함수)에 `u16/u32/int species` → `enum Species` 치환만 옮겼다. 파일 통째 복사·ours/theirs 없음.
  - 옮긴 뒤 `hunkmap.py`로 35파일의 (둘러싼 함수, 바뀐 줄) 쌍을 upstream과 대조해, 위치가 다른 것은 모두 아래 표의 HnS 대응이거나 "해당 없음"임을 확인했다.
- 한글 문자열·STRINGID·`{B_...}`·조사 토큰은 바뀌지 않았다(비ASCII 변경 줄 0).

### 거부 hunk의 HnS 대응 (요약)

| 파일 | upstream hunk | HnS에 넣은 것 |
|---|---|---|
| `include/battle.h` | `prevTurnSpecies` | 같은 줄(주석 문맥만 달랐음) |
| `include/battle_gfx_sfx_util.h` | `DecompressGhostFrontPic`·`BattleGfxSfxDummy2` | 같은 두 줄(HnS에 `DecompressTrainerBackPic` 선언이 없어 문맥 불일치) |
| `include/battle_util.h`, `include/field_effect.h`, `include/pokedex.h`, `include/script_menu.h` | 선언 | 같은 선언 |
| `include/naming_screen.h`, `src/naming_screen.c` | `DoNamingScreen` 인자 이름 `monSpeciesOrPlayerGender`, 필드 `monSpecies` | HnS 추가 인자 `bool8 isShiny`를 둔 채 같은 변경 |
| `include/roamer.h`, `src/roamer.c` | `TryAddRoamer`·`CreateInitialRoamerMon` | HnS 추가 인자 `u8 locationTableId`를 둔 채 종 인자만 |
| `include/script_pokemon_util.h`, `src/script_pokemon_util.c` | `ScriptGiveMon`·`ScriptGiveEgg`·`CreateScriptedWildMon`·`CreateScriptedDoubleWildMon`·`ScriptGiveMonParameterized` | 같은 함수(HnS 추가 인자 `bool8 isEgg` 유지) |
| `include/pokemon.h` | 게터·도감 변환 선언 29쌍 | 같은 선언. `GetLevelUpMovesBySpecies`는 HnS에 없음 |
| `include/daycare.h`, `src/daycare.c` | 알·유전 관련 12 hunk | HnS는 #9878(seq 367) 알 재작업을 원작업자가 선별 반영한 형태다. `SetInitialEggData`·`AlterEggSpeciesWithIncenseItem` 선언, `GetEggSpecies`(HnS는 공개 함수라 헤더 선언까지), `DetermineEggSpeciesAndParentSlots`, `InheritPokeball`(`species`·`species0`·`species1`), `InheritAbility`(`species`), `BuildEggMoveset`(`eggSpecies`, upstream `GiveMoveIfItem`의 대응), `TransferEggMoves`/`TransferEggMovesFromPool`(`learnerSpecies`·`eggSpecies`·`teacherSpecies`). `GetEggMoves`·`GetEggMovesBySpecies`·`SpeciesCanLearnEggMove`는 HnS에 없어 해당 없음 |
| `src/egg_hatch.c` | `CreateHatchedMon`, `AddHatchedMonToParty` | `CreateHatchedMon`은 HnS에 없음. `AddHatchedMonToParty`는 HnS가 이미 `nationalDexNum`을 선언만 해 둔 상태라 `species`만 `enum Species`로(도감 번호 분리 hunk는 `patch`로 적용됨). 기존 경고 "unused variable 'nationalDexNum'"이 사라졌다 |
| `src/battle_ai_switch.c`, `battle_main.c`, `battle_move_resolution.c`, `debug.c`, `dexnav.c`, `field_specials.c`, `frontier_util.c`, `wild_encounter.c` | 지역 변수·인자 | 같은 함수의 같은 줄(문맥만 달랐음). `battle_main.c` `GetDynamicMoveType`은 upstream처럼 `enum Species species; enum Item heldItem;` |
| `src/battle_util.c` | 4 hunk | `CalcBeatUpPower`·`CalcBeatUpDamage`·`CanMonParticipateInSkyBattle` 적용. `DoFutureSightAttackDamageCalcVars`는 HnS가 후속 구조(`DoFutureSightAttackDamageCalc`, 종 변수 없음)라 해당 없음 |
| `src/battle_setup.c` | 파일 끝 빈 줄 삭제 | HnS 파일 끝에 빈 줄이 이미 없어 해당 없음 |
| `src/ow_abilities.c` | 판정 함수 표 `(u32)` | HnS 표기 `static const bool32 (*const …[])` 그대로 인자형만 `(enum Species)` |
| `src/pokedex_area_screen.c` | `FindMapsWithMon`·`MapHasSpecies`·`MonListHasSpecies` | HnS 추가 인자 `u32 headerSectionId`를 둔 채 종 인자만 |
| `src/pokedex_plus_hgss.c` | 진화 화면 선언·함수 9 hunk | HnS 추가 인자(`numLines`·`totalLines`·`baseSpecies`)를 둔 채 종 인자·`targetSpecies`·`baseFormSpecies`·`owned`(`bool32`)만. `CalculateMoves`의 `preSpecies`는 HnS 코드가 이미 `enum Species` |
| `src/pokemon.c` | 16 hunk | 게터·`GetEvolutionTargetSpecies`·경험 사탕·도감 변환·`SanitizeSpeciesId` 인자·`IsSpeciesOfType`·`HasShedinjaHPHandling`(seq 87에서 `u32`로 넣은 것)은 그대로. `NationalPokedexNumToSpecies`는 HnS가 표 조회 구현이라 반환형과 `return 0` → `SPECIES_NONE`만(값 같음). `GetLevelUpMovesBySpecies`는 HnS에 없음 |
| `src/pokemon_sprite_visualizer.c` | 11개 `u16 species = …` | HnS가 `IsSpeciesEnabled(…) ? … : SPECIES_NONE`으로 바꾼 같은 11줄(함수 1:1 대응 확인) |
| `src/pokemon_storage_system.c` | `TryLoadMonIconTiles`, `RemoveSpeciesFromIconList` | 앞은 적용. 뒤는 **HnS에서 인자가 종이 아니라 아이콘 키**(`GetMonIconListKey`: 종 \| 암컷 bit15 \| 알 bit14)라 `u16 key` 유지 |
| `src/scrcmd.c` | `ScrCmd_showmonpic`, `ScrCmd_checkfieldmove` | HnS `u16 species = VarGet(varId)` 줄, `checkfieldmove`의 첫 반복문(upstream과 같은 역할). HnS가 추가한 TM/HM 반복문 2개와 `ScrCmd_checkpartymove`는 HnS 전용이라 `u16` 유지 |
| `src/tv.c` | 무작위 종 함수 2개 | 두 함수의 반환형·인자·첫 함수 지역 변수. 둘째 함수 본문은 HnS가 도감 배열 방식으로 다시 쓴 코드라 해당 없음 |

### upstream과 일부러 다르게 둔 곳

- **`src/field_effect.c` `InitFieldMoveMonSprite` 인자 `u32` 유지(`// HnS:` 주석).** upstream #9507은 이 인자를 `enum Species`로 바꿨다. 그런데 이 인자는 종 번호에 `SHOW_MON_CRY_NO_DUCKING`(bit 31)을 함께 싣는다(`FldEff_FieldMoveShowMonInit`, 파도타기 `tMonId | SHOW_MON_CRY_NO_DUCKING`, 다른 필드 기술 `| 0x80000000`). 16비트 `enum Species` 인자에서는 bit 31이 잘려 울음소리가 항상 배경음을 줄이는 쪽으로 바뀐다. 오브젝트 비교로 확인했다: 기준 빌드 `FldEff_FieldMoveShowMon`은 `lsrs r7, r0, #31; lsls r7, r7, #15`로 `data[6]`에 0x8000을 넣고, upstream 그대로면 `movs r2, #0`으로 항상 0이 된다. upstream 1.17.0에도 같은 코드가 남아 있다. 선언과 정의를 `u32`로 두어 `field_effect.o`가 기준 빌드와 함수 233개 모두 명령열 동일하다.
- `RemoveSpeciesFromIconList(u16 key)`: 위 표 참고(HnS 아이콘 키).
- HnS 전용 함수·인자·변수는 `u16`으로 두었다(upstream hunk가 없고 1.17.0에도 없는 것). 예: `CreateShinyScriptedMon`·`CreateScriptedWildBossMon`·`CreateScriptedDoubleWildBossMon`, `ScriptMenu_ShowShinyPokemonPic`, `CreateShinyMonSprite_PicBox`, `GetMonIconListKey`, `SpeciesToJohtoPokedexNum` 계열, `TransferEggMovesFromPool`의 `poolSpecies`·`TransferEggMoves`의 `nonBabySpecies`, `PrintEvolutionTargetSpeciesAndMethod`의 `baseSpecies`.

### #9507 diff 밖이지만 함께 맞춘 것 (지시: 1.17.0에서 `enum Species`를 받는 HnS 시그니처)

- `include/ow_abilities.h`: HnS가 후속 이름 변경을 먼저 받아 둔 헤더다(`ow_synchronize.h`와 include guard 공유, `daycare.c`만 include). #9507이 `ow_synchronize.h`에서 바꾼 `GetSynchronizedNature`·`GetSynchronizedGender` 선언과 같게 맞췄다(1.17.0 `ow_abilities.h`와 같음).
- upstream #9507 시점에는 없고(원작업자가 후속 PR에서 먼저 받아 둔 함수) 1.17.0에서 `enum Species`를 받는 함수 8개의 종 인자: `daycare.c` `GiveParentEggMoves`·`GiveParentTmMoves`·`GiveParentSharedLevelUpMoves`·`GiveMoveIfParentHeldItem`, `dexnav.c` `GetRandomEggMove`, `pokedex_plus_hgss.c` `CountSpeciesEggMoves`, `pokemon.c`/`pokemon.h` `SpeciesHasEggMove`, `pokemon.h` `GetSpeciesBaseStatTotal`(정의 없는 선언). 형만 바꿨다.
- 기준: `sig/sigcmp.py`로 HnS 함수 시그니처를 upstream #9507 직후·1.17.0과 비교했다. 남은 31건은 모두 #9507 직후 upstream도 `u16/u32`이고 뒤 PR에서 `enum Species`가 되는 것(예: `SanitizeSpeciesId`·`GetFormSpeciesId`·`GetSpeciesPreEvolution` 반환형, `GetFollowerInfo`·`GetMonInfo`의 `u32 *`, `AreBattleTowerLinkSpeciesSame`, `EggHatchCreateMonSprite`)이거나 위 `RemoveSpeciesFromIconList`라 그 PR 순서에서 처리한다. `u16`과 `enum Species`는 C에서 호환형이라 선언·정의가 엇갈려도 컴파일 오류가 나지 않으므로, HnS 안에서 같은 함수의 선언·정의가 종 인자형이 다른 곳도 따로 찾았다: 0건.

### 테스트 빌드 수정

`make check`의 테스트 빌드가 `-Wenum-conversion`(`-Werror`)으로 두 파일에서 멈췄다. 종 상수가 enum이 되면서 드러난 기존 테스트 코드 오류다. upstream도 같은 상태였고 #9507 뒤 병합 커밋 `8c950fb648`("Merge branch 'master' into upcoming")에서 고쳤다. 1.17.0과 같은 형태로 고쳤다(값·동작 불변).
- `test/battle/capture.c` 2곳: `GetSetPokedexFlag(SPECIES_CATERPIE, …)` → `GetSetPokedexFlag(NATIONAL_DEX_CATERPIE, …)`(둘 다 10).
- `test/battle/ability/mummy.c`: `enum Ability ability1, species1, ability2, species2;` → `enum Species species1, species2;` + `enum Ability ability1, ability2;`(upstream 줄 끝 공백은 뺐다).

### 검증

- `git diff --check`: 통과. 비ASCII 변경 줄 0.
- `make hns -j8`(커밋 뒤 `rm -rf build/hns` 전체 재빌드): 종료 코드 0, **ROM 32,713,060 B(+448 B) / EWRAM 248,924 B(0) / IWRAM 25,516 B(0)**. 경고 줄 165개·고유 43개. 기준 목록 대비 새 경고 0, 사라진 경고 1(`egg_hatch.c` 'nationalDexNum' 미사용). ROM SHA-1 `cfdfb88553e6c19e308d58099b7c681645bb9dfa`.
- **종 ID 값 보존(C·asm):** 아래 "필수 검증 1".
- **세이브 구조 불변:** 아래 "필수 검증 2".
- **부호·확장:** 아래 "필수 검증 3".
- **코드 변화:** 아래 "필수 검증 4".
- 자동 테스트(이식 후, 관련 13개 묶음): `test/daycare.c`(5), `test/pokemon.c`(27), `test/species.c`(6), `test/save.c`(4), `test/battle/gimmick/dynamax.c`(81), `move_effect/transform.c`(8), `move_effect/beat_up.c`(15), `ability/illusion.c`(9), `ability/mummy.c`(3), `capture.c`(7), `evolution_tracker.c`(8), `trainer_control.c`(20) — 모든 상태 줄 193개(PASS·FAIL·TO_DO·INVALID·ASSUMPTIONS_FAILED 포함)가 이식 전 전체 로그의 같은 테스트와 상태까지 같다. `capture.c` 7건은 이식 전에도 INVALID(HnS 문자열 문제, 기준 목록 추출 정규식 밖), `evolution_tracker.c` 8건은 이식 전에도 ASSUMPTIONS_FAILED. `test/battle/form_change/` 접두어는 "No tests found"라 구간 끝 전체 실행에 맡겼다.
- 실기 확인: 아래 "실기 확인 필요".
- 남은 위험: 낮음. 바뀐 함수 170개는 모두 형 변환(16비트 절단·확장)과 그에 따른 명령 배치 변화이고, 값이 16비트를 넘거나 음수인 경로는 위 필드 기술 울음소리 1건뿐이었다(유지).

## 필수 검증 결과 (#9507 직후, `b0a0fb9033`)

### 1. 종 ID 값 보존 (C 경로·asm 경로)

- 방법(`vals/speciesvals.py`): 이름 목록 = 현재 트리 `include/constants/species.h`의 `#define`과 enum 멤버 전부(`SPECIES_*` 1,671개 + `NUM_SPECIES` + `SPECIES_SHINY_TAG`) + #9558 이름 2개(`SPECIES_CUSTOM_START/END`, 이식 전에는 없음으로 표시) + 파생 상수(`RANDOMIZER_MAX_MON`(`include/randomizer.h`), `P_SCATTERBUG_LINE_FORM_BREED`(`config/pokemon.h`), `starter_choose.c`의 `SPECIES_BITMAP_SIZE` 식 `((NUM_SPECIES + 8) / 8)`, `sizeof(gFusionTablePointers)`).
  - C 경로: `global.h`·`constants/species.h`·`randomizer.h`·`config/pokemon.h`·`pokemon.h`를 include한 임시 C 파일을 실제 빌드와 같은 `arm-none-eabi-cpp` + `cc1 -O2`로 컴파일하고 `const long long v = (NAME);` 초기값을 어셈블리에서 읽었다.
  - asm 경로: `.4byte NAME` 목록 `.s`를 `data/*.s`와 같은 `preproc -s | cpp -I include | preproc -ie | as`로 조립해 `.data`를 읽었다. 이식 후 값은 preproc가 enum에서 만든 `.equiv`에서 온다(예: `.equiv SPECIES_BULBASAUR, (1) + 0`, `.equiv SPECIES_EGG, ((SPECIES_GLIMMORA_MEGA + 1)) + 0`, `.equiv NUM_SPECIES, (SPECIES_EGG) + 0`).
- 결과: C 1,679행(숫자 1,677, 나머지 2행은 이식 전후 모두 없는 CUSTOM 이름), asm 1,675행(숫자 1,673). **이식 전후 diff: C 0줄, asm 0줄.** 이름 집합도 같다(파일 안 순서만 다름). 주요 값: `SPECIES_NONE` 0, `SPECIES_GLIMMORA_MEGA` 1572, `SPECIES_EGG` = `NUM_SPECIES` = 1573, `SPECIES_SHINY_TAG` 5000, `RANDOMIZER_MAX_MON` 1572, `P_SCATTERBUG_LINE_FORM_BREED` 1455, `SPECIES_BITMAP_SIZE` 197, `sizeof(gFusionTablePointers)` 6292.
- `#if`/`#ifdef`/`defined()`와 asm `.if`에서 종 상수를 쓰는 곳 0건(`src`·`include`·`test`·`data`·`asm`·`tools`). `include/constants/species.h`를 파싱하는 빌드 도구 0건(`tools/learnset_helpers`·`wild_encounters`는 이름만 다룸).
- 참고: `enum Species`의 크기·부호는 `sizeof` 2, `(enum Species)-1 > 0`(무부호 16비트)이다(DWARF: `enumeration_type` byte_size 2, 바탕형 `short unsigned int`).

### 2. 세이브 구조 불변 (DWARF 배치 비교)

- 방법(`layout/`): (a) `global.h` 뒤에 `include/*.h`·`include/constants/*.h` 가운데 함께 컴파일되는 438개(`help_window.h` 1개만 충돌로 제외, 종 필드 없음)를 include한 TU 1개, (b) #9507이 바꾼 C 파일 98개 각각을 HnS와 같은 `cpp → preproc -i → cc1` 경로와 플래그(`-O2 -mabi=apcs-gnu … -ffunction-sections -fdata-sections`)에 `-g -fno-eliminate-unused-debug-types`를 더해 컴파일했다. `arm-none-eabi-readelf --debug-dump=info`에서 이름 있는 struct/union과 익명 struct를 가리키는 typedef를 뿌리로 삼아 멤버를 재귀적으로 펼쳤다(중첩 struct·union·배열 첫 원소까지, 줄마다 경로·오프셋·크기·비트 오프셋·비트 폭·잎 형 분류). 이식 전후 줄을 비교하되 잎 형 분류는 `uint` → `enum:uint`(같은 폭)만 허용했다.
- 결과: TU 99개, 뿌리 비교 25,872회(서로 다른 뿌리 633종). **배치(경로·오프셋·크기·비트필드 위치) 차이 0.** 잎 형 변화 12,897건(중복 포함)은 모두 `uint → enum:uint`이고, 형만 바뀐 뿌리는 82종이다(예: `SaveBlock1`·`SaveBlock2`·`BattleFrontier`·`TVShow`·`DayCare`·`DaycareMon`·`DaycareMail`·`Roamer`·`Apprentice`·`ApprenticeMon`·`PlayersApprentice`가 아니라 `ApprenticeTrainer`, `EmeraldBattleTowerRecord`·`BattleTowerPokemon`·`BattleTowerInterview`·`SecretBase`·`SecretBaseParty`·`ContestWinner`·`Mail`·`WonderCard`·`WonderCardMetadata`·`MysteryGiftSave`·`RecordMixingDaycareMail`·`TrainerHill*`·`BattlePokemon`·`BattleResults`·`BattleStruct`·`Evolution`·`FormChange`).
- 세이브 관련 주요 구조체 크기(전후 같음): `SaveBlock1` 15,760 / `SaveBlock2` 3,892 / `SaveBlock3` 52 / `PokemonStorage` 34,256 / `Pokemon` 100 / `BoxPokemon` 80(`PokemonSubstruct0.species`는 `u16 :11` 비트필드 그대로) / `TVShow` 36 / `LilycoveLady` 64 / `DayCare` 288 / `Roamer` 28 / `BattleFrontier` 2,272 / `Apprentice` 68 / `EmeraldBattleTowerRecord` 236 / `BattleTowerPokemon` 44 / `TrainerHillSave` 12 / `SecretBase` 160 / `ContestWinner` 32 / `Mail` 36 / `WonderCard` 332 / `WonderCardMetadata` 36 / `MysteryGiftSave` 876 / `RecordMixingDaycareMail` 120 / `RecordedBattleSave` 4,008 / `HallofFameMon` 24.
- 기존 `STATIC_ASSERT(sizeof …)` 11개(`save.c` `SaveBlock1/2/3`·`PokemonStorage` 여유 공간·`ChallengeSettings == 32`, `hall_of_fame*.c`, `ereader_helpers.c` `TrainerHillChallenge`, `recorded_battle.c`, `list_menu.c`, `battle.h` `palaceFlags`)가 빌드에서 통과했다.
- asm 데이터 오브젝트(`data/*.o`) 4개는 파일은 달라졌지만 섹션 내용과 재배치가 기준과 같다. 차이는 preproc가 enum에서 만든 `SPECIES_*` 절대 기호(1,670개)가 기호표에 생긴 것뿐이다.

### 3. 부호·확장 동작

- 부호 있는 형에서 `enum Species`(무부호 16비트)로 바뀐 것: `CreateInvisibleSpriteCopy`의 `int species`(본문에서 안 씀), `LoadSpecialPokePic`/`…IsEgg`·`HandleLoadSpecialPokePic`/`…IsEgg`의 `s32 species`(첫 줄 `SanitizeSpeciesId(u16)`에서 이미 16비트로 잘림 → 같음), `battle_dome.c` `InitDomeTrainers`·`InitRandomTourneyTreeResults`의 `int species[3]`(0과 `gFacilityTrainerMons[].species`만 대입·비교), `DecideRoundWinners`의 `int species`(같은 표 값), `trade.c`의 `int speciesArray[]`(`GetMonData`·`SPECIES_NONE`). 음수·16비트 초과 값이 들어가는 경로 0.
- 음수·sentinel 비교: `CalcBeatUpPower`의 `species == 0xFFFF`(`beatUpSpecies[]`를 `memset 0xFF`로 채운 값) — `u16`에서 오던 값이라 무부호 16비트에서도 같다. `species < 0`·`>= 0`·`== -1`을 `enum Species` 변수에 쓰는 곳 0건.
- 16비트 넘는 값을 싣는 경로를 찾으려고 `enum Species`로 바뀐 이름 전부를 파일별로 `-1`·`< 0`·`0xFFFFF…`·`~`·`SHINY_TAG`·`<<`·`>>`·큰 상수 가감과 함께 검색했다(42줄 검토). 문제가 된 것은 `InitFieldMoveMonSprite`(bit 31 울음소리 플래그) 1건이고 `u32`로 유지했다. 나머지: `SetMultiuseSpriteTemplateToPokemon`의 `species + SPECIES_SHINY_TAG`(최대 6,572), `pokemon_storage_system.c` 아이콘 키(bit 14·15, 최대 0xFFFF)는 16비트 안이라 `u16`과 같다. `dexnav.c`의 `(environment << 14) | species`는 `int` 식이라 같다.
- 종 인자가 `u32` → `enum Species`가 된 함수는 호출된 쪽이 인자를 16비트로 자른다(예: `IsNaturalEnemy`·`CheckPartyHasSpecies`·`GetSynchronizedNature`에 `lsls/lsrs #16` 추가). 호출하는 값은 모두 `GetMonData`·`VarGet`·표 값 등 16비트 출처다.

### 4. 코드 변화 (기준 빌드 오브젝트와 비교)

- 오브젝트 1,524개 중 1,478개 바이트 동일. 다른 46개 = asm 데이터 4개(위 2절, 내용·재배치 동일) + C 42개.
- C 42개의 함수 4,610개 중 4,440개 명령열 동일, **170개 다름**(`classify.py`):
  - 33개: 명령·레지스터가 같고 상수만 다름. `pokemon.c`(+1줄)·`battle_script_commands.c`(+2줄)에서 upstream이 `u16 species, heldItem` 같은 선언을 두 줄로 나눠 뒤쪽 `assertf`의 `__LINE__` 상수가 바뀐 것(예: `BS_GetStatValue` 0x3638 → 0x363a, `GetCryIdBySpecies`).
  - 43개: 16/24비트 절단·확장 명령(`lsls/lsrs/asrs #16|#24`)만 늘거나 줄었다.
  - 94개: 같은 원인(인자·지역 변수 폭 변화)에 따른 레지스터 배정·명령 배치·분기 복제 차이. 대표: `GetFormChangeTargetSpecies_Internal`(700 → 886 명령, 반환형 `enum Species`로 분기 꼬리 복제 증가, 호출 대상 집합 같음), `GetDynamicMoveType`(781 → 762), `HandleSpeciesGfxDataChange`, `UpdateFollowingPokemon`, `IsNaturalEnemy`(인자 16비트 절단 4명령 추가, 나머지는 분기 주소).
  - 파일별 다른 함수 수: `pokemon.o` 45, `event_object_movement.o` 18, `battle_util.o` 9, `battle_controllers.o`·`battle_script_commands.o` 8, `debug.o`·`evolution_scene.o`·`trade.o` 6, 그 밖 34개 파일 1~5개(`battle_pyramid.o`는 함수 차이 0, 데이터·기호만).
- `field_effect.o`는 `InitFieldMoveMonSprite` 유지로 함수 233개 모두 명령열 동일.
- ROM 크기: 32,712,612 → 32,713,060 B(+448 B). EWRAM·IWRAM 변화 0.

### 5. 한글

- #9507 커밋의 비ASCII 변경 줄 0(`species.h` 머리 주석 "Pokémon" 줄은 바뀌지 않음).
