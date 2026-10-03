# full-sync 실제 port 결과 — seq 135~138

완료: seq 135~138 이식·전체 테스트·커밋 리뷰(병렬 2개, 모두 문제 없음)·기록 완료. **다음 구간은 seq 138.5 #8943(12v12, XL)**이고 그 뒤가 seq 139 #9714다. 이식 커밋 2개(#9709, #9711), 이미 적용 2개(#9713, #9707). 결과 커밋에 기준 목록 [`test-baseline-seq138.txt`](test-baseline-seq138.txt)를 넣었다.

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), [`CLAUDE_FULL_SYNC_PORT_PROMPT.md`](../../CLAUDE_FULL_SYNC_PORT_PROMPT.md), 메시지 출력 기록: [`BATTLE_MESSAGE_OUTPUT_CHANGES.md`](../../../localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md)
시작 HEAD: `90ae8c6774`(작업 트리 clean, 코드는 seq 132 `5f580ab12e`와 같음). 작업 컴퓨터: 데스크탑(`/usr/bin` 툴체인).

## seq 135~138 요약

| seq | PR | 판정 | 커밋 | ROM 변화 | 비고 |
|---|---|---|---|---:|---|
| 135 | #9709 | 부분 적용 | `a642657907` | +16 B | `BattlePokemon.affectionHearts` 추가(끝 패딩, `sizeof` 144 그대로). `GetBattlerAffectionHearts`가 `gBattleMons`를 읽는다. `include/config/ai.h` 테스트 전용 프레임 상한 hunk 제외(HnS 값 유지) |
| 136 | #9711 | 적용 | `62d1876d6f` | +32 B | `BattlePokemon` 오프셋 주석 삭제 **+ `u8 metLevel:7; u8 isShiny:1;` 비트필드 압축**(주석만의 변경이 아님). `affectionHearts` 오프셋 142 → 141, `sizeof` 144 그대로. 구조체가 1.17.0과 같아짐 |
| 137 | #9713 | 이미 적용(선진행) | 없음(`d057cee5c2`) | 0 | 선진행 구간에서 B안으로 이식(`full-sync-ahead-seq-130-167.md`) |
| 138 | #9707 | 이미 적용 | 없음(`8e6f16bf71`) | 0 | 예전 구간에서 #9407 unit으로 이식(`full-sync-seq-063-082.md`) |

- 마지막 빌드(`62d1876d6f` 작업 트리): 종료 코드 0, **ROM 32,716,948 B(97.50%) / EWRAM 248,936 B(94.96%) / IWRAM 25,516 B(77.87%)**. 구간 전체 ROM +48 B(#9709 +16 B, #9711 +32 B), EWRAM 0, IWRAM 0. 매 빌드 새 경고 0. `pokehns.gba` SHA1 `dfeec48834b4b1ca000cc2b09b39b6175834357b`(사전 분석 스크래치 빌드와 같음).
- `sizeof(struct BattlePokemon)`은 이식 전후 **144 그대로**(게임 빌드), `gBattleMons` `0x240` 그대로. 링크 전송 버퍼 크기·세이브·녹화 배틀 영향 없음.
- 한글이 든 소스 줄 변경: **0**(두 커밋 모두 비 ASCII `+`/`-` 줄 0). config 기본값 변경 0(`include/config/ai.h`는 건드리지 않음).
- upstream과 다르게 둔 곳: #9709의 `include/config/ai.h` 프레임 상한 hunk 제외(HnS 값 유지). 그 밖에는 upstream 그대로이고, `struct BattlePokemon`이 1.17.0과 같아졌다.
- `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 추가 행: 없음.
- 전체 테스트: 표준 목록이 `test-baseline-seq132.txt`와 **바이트 동일**, 사라진 PASS 0(아래 "구간 끝 전체 테스트").

## 공통 사항

- 툴체인: 데스크탑 기본 PATH `arm-none-eabi-gcc` 13.2.1(`/usr/bin`). PATH 접두어 없이 빌드했다.
- 빌드 명령: `GITHUB_ACTION=1 make hns -j8 > build/port.log 2>&1`.
- 이식 전 기준(`5f580ab12e` 코드, 작업 트리의 `pokehns.gba`): ROM 32,716,900 B / EWRAM 248,936 B / IWRAM 25,516 B, SHA1 `512ccdbecee5dbcde2f777fcb96e20557bdb1015`.
- 경고 비교: `LC_ALL=C grep -a 'warning:' build/port.log | sed -E 's/:[0-9]+:[0-9]+: /: /' | sort -u | comm -13 warn-base.txt -`. 기준은 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-ahead-130-167/warn-base.txt`.
- 테스트 비교: `PORT_INSTRUCTIONS`의 `LC_ALL=C`·`grep -a` 표준 추출로 목록을 만들어 [`test-baseline-seq132.txt`](test-baseline-seq132.txt)의 같은 이름 줄과 비교했다.
- 사전 분석: 읽기 전용 분석이 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-135/`에 PR별 계획과 patch를 남겼다(`seq135-9709.md`/`.patch`, `seq136-9711.md`/`.patch`, 스크래치 검증 `tmp-135/`·`tmp-136/`). patch에는 파일 모드 줄이 없다(대상 파일이 100755). 적용 직전 `git apply --check`로 다시 확인했고, 두 patch의 `+`/`-` 줄이 upstream `ed3557ed04`(ai.h 제외)·`52d8b6b801`의 `+`/`-` 줄과 같음을 diff로 확인했다.

## 동기화 단위: seq 135 #9709 `U-9709` Improve AI calc speed with affection hearts

- 현재 판정: 부분 적용(게임 코드 4파일 그대로, `include/config/ai.h` hunk 제외)
- 커밋: `a642657907`
- upstream 근거: `ed3557ed04`(5파일 +10/−10). `git log --grep='9709'` 이식 커밋 0건, `affectionHearts`가 `include`·`src`·`test`에 없었다 → 미적용이었다. deps 없음.
- 수정 파일(4): `include/pokemon.h`, `src/battle_controllers.c`, `src/battle_util.c`, `src/pokemon.c`
- 적용 방법: 사전 분석 patch(`seq135-9709.patch`, sha1 `70489855…`)를 `git apply`했다. 충돌 없음. `git diff --check` 통과.
- 내용(upstream 그대로):
  - `struct BattlePokemon` 끝에 `u8 affectionHearts`(upstream과 같은 13칸 들여쓰기, 오프셋 주석 없음).
  - `PokemonToBattleMon`과 `GetBattlerMonData`(`REQUEST_ALL_BATTLE`)가 `GetMonAffectionHearts(파티 몬)`로 채운다.
  - `GetBattlerAffectionHearts`는 `GetBattlerMon`·`GetMonData` 대신 `gBattleMons[battler].species`·`.affectionHearts`를 읽는다. seq 115 #9494가 넣은 `|| gBattleStruct->battlerState[battler].notOnField` 조건은 upstream 문맥과 같아 그대로 유지된다. 1.17.0의 `attackerInParty` 조건은 #10161 소관이라 넣지 않았다.
- 제외한 hunk: `include/config/ai.h`의 `AI_FRAME_CEILING_*` 5개(8→7, 23→22, 40→37, 29→26, 32→30).
  - 테스트 전용이다. `test/battle/ai/ai.c`의 `AI thinking time doesn't explode` 6개만 쓰고 게임 코드 참조는 0이다.
  - 문맥이 다르다. HnS 값 3/8/21/38/29/31은 #9592 원래 값이고, upstream은 PR이 아닌 merge 보정 커밋 `e913b13acd`("Accomodate `upcoming`'s higher thinking times")로 23/40/32로 올린 뒤 #9709가 그 값에서 낮췄다. 그래서 hunk가 HnS에 적용되지 않는다.
  - HnS 값을 유지한다(g4 계획). 이 정의는 seq 154 #9779가 `test/battle/ai/ai_thinking_time.c`로 옮기며, 값은 그때 HnS 실측으로 다시 맞춘다. 사전 분석 실측 프레임(이식 전 = 이식 후): singles 2/7, doubles 14/27, Steven multi 19/22. 모두 HnS 상한 안이다.
- **구조체 크기·메모리:** `sizeof(struct BattlePokemon)` **144 그대로**. 이전 끝 필드 `isShiny`가 오프셋 141이고 142·143이 끝 패딩이라, `affectionHearts`(오프셋 142)가 패딩에 들어간다(테스트 빌드는 140 → 140, 오프셋 138). 빌드 ELF의 `gBattleMons`도 `0x240`(= 4 × 144) 그대로다. 그래서 `AllocSaveBattleMons` 힙 사본, 스택 지역 변수, 컨트롤러 버퍼 전송 길이(`sizeof(battleMon)`), 아군 교대 `SwapStructData` 크기가 모두 같다. full-sync-plan 4절 추정 "EWRAM 최대 약 16 B"는 실제 0 B다.
- **세이브·링크·녹화 배틀:** 영향 없음.
  - `BattlePokemon`은 SaveBlock에 없다. 녹화 배틀(`RecordedBattleSave`)은 `struct Pokemon` 파티와 입력만 저장한다.
  - 링크 전송 구조의 바이트 수가 같다(144). **g4 plan의 "버퍼 전송 구조 1 B 증가로 링크 호환 깨짐"은 해당하지 않는다.** 142번 바이트의 의미만 바뀐다(이식 전에는 초기화하지 않은 스택 값). 링크·녹화 링크·프런티어 배틀에서는 `GetBattlerAffectionHearts`가 이 값을 읽기 전에 `AFFECTION_NO_HEARTS`를 돌려준다.
- **게임 동작:** `B_AFFECTION_MECHANICS FALSE`(`include/config/battle.h:377`)라 결과를 쓰는 곳(`battle_util.c:8000` 급소 랭크, `:8159` `GetAdjustedDamage` 버티기, `:10581` 명중률, `battle_end_turn.c:213` 턴 끝 상태 회복, `battle_anim_new.c:7787` 마음 버티기 애니메이션 — 이 애니메이션은 TRUE일 때만 재생)이 모두 막혀 있다. **이식 전후 같다.** `GetAdjustedDamage`의 `Random()` 호출 횟수도 같다. 빌드에서는 LTO가 `GetAdjustedDamage` 안의 쓰이지 않는 호출을 없애 `GetBattlerAffectionHearts` 호출 지점이 1곳(`AnimTask_AffectionHangedOn`)만 남았다(`objdump` 확인).
  - 참고(`B_AFFECTION_MECHANICS`를 켤 때만, upstream 1.17.0과 같음): 종 판정이 파티 종에서 배틀 종으로 바뀌고(변신·괴짜로 메가 복사 시 마음 없음), 값이 등장할 때 정해진다(배틀 중 레벨업 친밀도 상승은 다시 나올 때 반영). 또 AI가 교체 후보를 `gBattleMons`에 넣고 계산할 때(`battle_ai_switch.c`, `battle_ai_main.c`) 현재 몬이 아니라 후보의 마음 값을 쓴다(커밋 리뷰 보충).
- 한글·config·HnS 전용 코드: 한글 문자열·`STRINGID`·조사 토큰 변경 0(patch에 비 ASCII 줄 없음). config 기본값 변경 0. HnS 표식 구간과 겹치지 않는다. 출력 변화(`BATTLE_MESSAGE_OUTPUT_CHANGES.md`) 없음.
- 빌드(`build/port.log`): 종료 코드 0, **ROM 32,716,916 B(97.50%, +16 B) / EWRAM 248,936 B(94.96%, 0) / IWRAM 25,516 B(77.87%, 0)**. `pokehns.gba` SHA1 `d71b4f61559bc7e5a7313791012b4ec02c45bea3`(사전 분석 스크래치 빌드와 같음). 경고 163줄, 새 경고 0. 함수 크기 `GetBattlerAffectionHearts` 0x70, `PokemonToBattleMon` 0x230, `GetBattlerMonData` 0x5cc(사전 분석 실측과 같음).
- 테스트: `GITHUB_ACTION=1 make check BUILD=hns -j6 TESTS="AI thinking time"`(`build/port-check-135-ai.log`) → 6개 모두 **PASS**, `test-baseline-seq132.txt`의 같은 6줄과 같다. upstream·HnS 모두 친밀도·마음 테스트는 없다.
- 남은 위험: 없음(게임 동작은 config로 막혀 있고 구조체 크기 불변). `B_AFFECTION_MECHANICS`를 켜면 위 참고 3가지가 upstream과 같게 달라진다.
- 실기 확인: 불필요.

## 동기화 단위: seq 136 #9711 `U-9711` Remove hex values from BattlePokemon

- 현재 판정: 적용(upstream 그대로)
- 커밋: `62d1876d6f`
- upstream 근거: `52d8b6b801`(`include/pokemon.h` 1파일 +34/−34), 부모 `ed3557ed04`(#9709). `git log --grep=9711` 없음, `struct BattlePokemon`에 `/*0x..*/` 주석이 그대로 있었다 → 미적용이었다. 선행: seq 135 #9709(문맥에 `affectionHearts` 줄).
- 수정 파일(1): `include/pokemon.h`
- 적용 방법: 사전 분석 patch(`seq136-9711.patch`, seq 135 patch 위에 쌓이는 patch)를 `git apply`했다. 충돌 없음. `git diff --check` 통과. 적용 뒤 `struct BattlePokemon` 블록이 upstream `expansion/1.17.0`의 같은 블록과 **diff 0**이다(HnS 고유 필드 없음).
- 내용:
  - 34필드 앞의 `/*0x..*/` 오프셋 주석을 지웠다. 주석은 이미 실제 오프셋과 맞지 않았다(예: `isShiny` 주석 `0x62`, 실제 141).
  - **주석 외 변경:** `u8 metLevel;` / `bool8 isShiny;` → `u8 metLevel:7;` / `u8 isShiny:1;`. 두 필드가 바이트 140 하나를 함께 쓴다(`metLevel` 비트 0–6, `isShiny` 비트 7). 그래서 **g5 plan의 "주석만 34줄, 빌드 산출물 불변"은 틀렸다**: 비트필드를 읽고 쓰는 코드가 바뀌어 ROM이 +32 B 늘었다.
- **구조체 측정(저장소 헤더로 스크래치 컴파일, `-DPOKEMON_HNS`, apcs-gnu):** 게임 빌드 `sizeof` **144 그대로**, `offsetof(pp)` 37 그대로(변신 복사 범위 불변), `otId` 136, **`affectionHearts` 142 → 141**. 테스트 빌드(`TESTING=1`)는 `sizeof` 140, `affectionHearts` 137. ELF의 `gBattleMons`는 `0x240` 그대로, EWRAM·IWRAM 0.
- 의미 변화: 없음.
  - `metLevel` 값 원천은 `struct PokemonSubstruct3`의 `u16 metLevel:7`(0..127, `include/pokemon.h:187`)이라 7비트에 들어간다. 읽는 곳은 `battle_util.c` 복종 판정의 `>= GEN_8` 분기뿐인데 HnS `B_OBEDIENCE_MECHANICS GEN_7`이라 컴파일 시 제거된다. `PokemonToBattleMon`은 이전처럼 `metLevel`을 채우지 않는다(upstream과 같음).
  - `isShiny`에는 `IsMonShiny()`/`MON_DATA_IS_SHINY`의 0/1만 들어간다. 읽는 곳은 변신 색 복사(`battle_script_commands.c`, `B_TRANSFORM_SHINY`)다.
  - 비트필드 주소를 잡는 코드(`&…metLevel`, `&…isShiny`)는 `src`·`include`·`test`에 없다(빌드 성공으로도 확인).
- 세이브·녹화·링크: 세이브와 녹화 배틀은 무관하다(`BattlePokemon`은 SaveBlock·`RecordedBattleSave`에 없음). 링크 전송 크기는 144 B 그대로다. 140~141번 바이트 배치만 바뀌어 이식 전 ROM과 이식 후 ROM 사이 링크만 어긋난다(다른 full-sync PR과 같은 "다른 버전 간 링크 불가", 같은 ROM끼리는 영향 없음).
- 한글·config·HnS 전용 코드: 해당 없음(비 ASCII 줄 0, config 변경 0).
- 빌드(`build/port.log`): 종료 코드 0, **ROM 32,716,948 B(97.50%, +32 B) / EWRAM 248,936 B(94.96%, 0) / IWRAM 25,516 B(77.87%, 0)**. `pokehns.gba` SHA1 `dfeec48834b4b1ca000cc2b09b39b6175834357b`(사전 분석 스크래치 빌드 `dfeec488…`와 같음). 경고 163줄, 새 경고 0. 크기가 바뀐 함수: `GetBattlerMonData` 0x5cc → 0x5e8, `PokemonToBattleMon` 0x230 → 0x23c(사전 분석 4개 오브젝트 변화와 같은 방향).
- 테스트(파일별 `GITHUB_ACTION=1 make check BUILD=hns -j6 TESTS="<파일>"`, 로그 `build/port-check-136-{transform,imposter,trainer_control,pokemon}.log`): `test/battle/move_effect/transform.c`, `test/battle/ability/imposter.c`, `test/battle/trainer_control.c`, `test/pokemon.c` → **55줄(PASS 47)이 `test-baseline-seq132.txt`의 같은 이름 줄과 모두 같다.** 비 PASS 8건(FAIL 6, TO_DO 2)은 기준과 같은 기존 실패다.
- 남은 위험: 없음. 구조체가 1.17.0과 같아져 뒤 PR의 `BattlePokemon` hunk 문맥이 upstream과 맞는다.
- 실기 확인: 불필요. 원하면 이로치 상대에게 변신(메타몽)했을 때 색이 맞는지 본다.

## 동기화 단위: seq 137 #9713 `U-9713` Support non-contiguous SE/MUS IDs in debug menu

- 현재 판정: 이미 적용(선진행)
- 커밋: 없음. 선진행 커밋 `d057cee5c2`("Port upstream #9713: Support non-contiguous SE/MUS IDs in debug menu", 진행 기록 `b6a564cf7c`).
- upstream 근거: `48a165c403`(`include/constants/songs.h`, `src/debug.c`)
- 근거: `d057cee5c2`가 HEAD의 조상이고, `src/debug.c`에 `FindSong`·`sSongNames`가 있다. 선진행 때 B안(곡 이름 저장 안 함, `SE_`/`MUS_` 접두어만)으로 넣었다. 판정·검증·실기 확인 항목은 [`full-sync-ahead-seq-130-167.md`](full-sync-ahead-seq-130-167.md)의 seq 137 항목에 있다. 재이식하지 않았다.

## 동기화 단위: seq 138 #9707 `U-fade-9407` Reset objPaletteToggle after Software Fade

- 현재 판정: 이미 적용
- 커밋: 없음. 예전 구간 커밋 `8e6f16bf71`("Port upstream #9707: Reset objPaletteToggle after Software Fade", #9407 unit).
- upstream 근거: `833f1de49a`(`src/palette.c` +1줄 `gPaletteFade.objPaletteToggle = 0;`)
- 근거: `8e6f16bf71`이 HEAD의 조상이다. 뒤 커밋 `f76c7bf7ed`(#10573)가 `src/palette.c`를 다시 고쳤지만 그 줄은 남아 있다(`src/palette.c:839`, 1.17.0 `:837`과 같은 위치). [`full-sync-seq-063-082.md`](full-sync-seq-063-082.md)의 "unit U-fade-9407: #9707" 항목에 기록돼 있다. 재이식하지 않았다.

## 구간 끝 전체 테스트

`GITHUB_ACTION=1 make check BUILD=hns -j6 > build/port-check-post136.log 2>&1`(작업 트리 = `62d1876d6f` 코드, 4분 47초, `make` 종료 코드 2 = 실패 테스트가 있을 때의 정상 종료). `Killed`·러너 크래시 0. 목록은 `PORT_INSTRUCTIONS`의 `LC_ALL=C`·`grep -a` 표준 추출로 만들었다.

| 항목 | 이식 전(seq 132) | 이식 후(seq 136) |
|---|---|---|
| 러너 요약 | PASSED 2,344 / FAILED 2,256 / KNOWN_FAILING 10 / ASSUMPTIONS_FAILED 38 / TO_DO 607 / EXPECT_FAILING 6 / TOTAL 5,261 | **같음** |
| 표준 목록 | 5,192줄(PASS 2,341 / FAIL 2,231 / TO_DO 605 / KNOWN_FAILING 10 / EXPECTED_FAIL 5) | **바이트 동일** |
| 확장 상태(`ASSUMPTION_FAIL`·`INVALID`·`TIMEOUT`·`CRASH`) | 60줄(ASSUMPTION_FAIL 38 / INVALID 21 / CRASH 1) | `build/port-check-post132.log`와 같은 줄 |

- **사라진 PASS 0, 새 PASS 0, 상태가 바뀐 테스트 0.** 로그 끝 실패 위치 목록(`  - test/…` 줄)도 seq 132 로그와 같다.
- `AI thinking time doesn't explode` 6개는 HnS 상한(`include/config/ai.h` 유지)으로 모두 PASS다.
- 새 기준 목록: [`test-baseline-seq138.txt`](test-baseline-seq138.txt)(5,192줄, `test-baseline-seq132.txt`와 내용 같음).

## 커밋 리뷰 (병렬 2개, 읽기 전용)

| 리뷰 | 커밋 | 판정 | 확인한 것 |
|---|---|---|---|
| #9709 | `a642657907` | 문제 없음 | upstream hunk(`config/ai.h` 제외)와 줄 단위 동일. `AI_FRAME_CEILING_*`는 테스트 6곳에서만 쓰이고 HnS 값은 #9592 그대로라 제외가 맞음. `affectionHearts`를 채우는 경로(전투 시작·교체·사파리·변신·괴짜·아군 교대) 확인, 되돌려 쓰는 경로 없음. `B_AFFECTION_MECHANICS FALSE`라 ELF에서 `GetBattlerAffectionHearts` 호출이 애니메이션 태스크 1곳만 남음. `sizeof` 144·통신 패킷 4+144 B 이식 전과 같음. 정보 1건(TRUE일 때 세 번째 차이)은 위에 반영 |
| #9711 | `62d1876d6f` | 문제 없음 | `struct BattlePokemon`이 1.17.0과 diff 0. 비트필드 두 필드에 쓰는 값이 범위 안(`metLevel` 0..127, `isShiny` 0/1), 주소·오프셋 접근 없음, 바이트 복사는 구조체 전체 또는 `pp` 앞까지. seq 137·138 이미 적용 판정 확인. 변신 이로치 색 복사 경로는 테스트가 직접 보지 않으므로 아래 선택 실기 항목으로 본다 |

"수정 필요" 0건.

## 실기 확인 항목 (친구용)

- 필요한 항목 없음. #9709는 `B_AFFECTION_MECHANICS FALSE`로 게임 동작이 막혀 있고, #9711은 값 범위 안의 비트필드 압축이다.
- 선택: 이로치 상대에게 메타몽이 변신했을 때 이로치 색이 맞는지(#9711 `isShiny:1` 읽기 경로).

## 후속 행 메모

- **g4 plan `9709` 행 정정:** "버퍼 전송 구조가 1 B 늘어 구버전 ROM과 링크 호환이 깨진다"는 해당하지 않는다(끝 패딩에 들어가 `sizeof` 144 그대로). 또 "HnS 조건은 `!IsOnPlayerSide`만"은 seq 115 #9494가 `notOnField` 조건을 넣어 이미 upstream 문맥과 같았다.
- **g5 plan `9711` 행 정정:** "주석만 34줄, 빌드 산출물 불변"이 아니다. `metLevel:7`/`isShiny:1` 비트필드 압축으로 ROM +32 B(EWRAM·IWRAM 0), `affectionHearts` 오프셋 142 → 141.
- **seq 154 #9779(Isolate AI thinking time tests):** `include/config/ai.h`의 `AI_FRAME_CEILING_*`를 `test/battle/ai/ai_thinking_time.c`로 옮길 때 HnS 값(3/8/21/38/29/31)을 출발점으로 하고, HnS 실측으로 다시 맞춘다. upstream #9709 값(3/7/22/37/26/30)이나 merge 보정 `e913b13acd` 값(23/40/32)을 그대로 쓰지 않는다. 사전 분석 실측(이식 전 = 이식 후): singles 2/7, doubles 14/27, Steven multi 19/22.
- **seq 228 #10161:** `GetBattlerAffectionHearts`의 첫 조건에 `|| gSpecialStatuses[battler].attackerInParty`를 더한다. 이번에 넣지 않았다. 그 줄의 나머지 문맥(`gBattleMons[battler].species`, `notOnField`)은 1.17.0과 같다.
- `struct BattlePokemon`이 1.17.0과 같아졌으므로, 뒤 PR의 이 구조체 hunk는 upstream 문맥 그대로 맞는다.
