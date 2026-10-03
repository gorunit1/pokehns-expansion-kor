# full-sync 실제 port 결과 — seq 135~138

진행 중: 마지막 완료 seq 136, 다음 seq 137 #9713(이미 적용)·138 #9707(이미 적용) 기록과 구간 끝 전체 테스트.

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), [`CLAUDE_FULL_SYNC_PORT_PROMPT.md`](../../CLAUDE_FULL_SYNC_PORT_PROMPT.md), 메시지 출력 기록: [`BATTLE_MESSAGE_OUTPUT_CHANGES.md`](../../../localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md)
시작 HEAD: `90ae8c6774`(작업 트리 clean, 코드는 seq 132 `5f580ab12e`와 같음). 작업 컴퓨터: 데스크탑(`/usr/bin` 툴체인).

## seq 135~138 요약

| seq | PR | 판정 | 커밋 | ROM 변화 | 비고 |
|---|---|---|---|---:|---|
| 135 | #9709 | 부분 적용 | `a642657907` | +16 B | `BattlePokemon.affectionHearts` 추가(끝 패딩, `sizeof` 144 그대로). `GetBattlerAffectionHearts`가 `gBattleMons`를 읽는다. `include/config/ai.h` 테스트 전용 프레임 상한 hunk 제외(HnS 값 유지) |
| 136 | #9711 | 적용 | `62d1876d6f` | +32 B | `BattlePokemon` 오프셋 주석 삭제 **+ `u8 metLevel:7; u8 isShiny:1;` 비트필드 압축**(주석만의 변경이 아님). `affectionHearts` 오프셋 142 → 141, `sizeof` 144 그대로. 구조체가 1.17.0과 같아짐 |

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
  - 참고(`B_AFFECTION_MECHANICS`를 켤 때만, upstream 1.17.0과 같음): 종 판정이 파티 종에서 배틀 종으로 바뀌고(변신·괴짜로 메가 복사 시 마음 없음), 값이 등장할 때 정해진다(배틀 중 레벨업 친밀도 상승은 다시 나올 때 반영).
- 한글·config·HnS 전용 코드: 한글 문자열·`STRINGID`·조사 토큰 변경 0(patch에 비 ASCII 줄 없음). config 기본값 변경 0. HnS 표식 구간과 겹치지 않는다. 출력 변화(`BATTLE_MESSAGE_OUTPUT_CHANGES.md`) 없음.
- 빌드(`build/port.log`): 종료 코드 0, **ROM 32,716,916 B(97.50%, +16 B) / EWRAM 248,936 B(94.96%, 0) / IWRAM 25,516 B(77.87%, 0)**. `pokehns.gba` SHA1 `d71b4f61559bc7e5a7313791012b4ec02c45bea3`(사전 분석 스크래치 빌드와 같음). 경고 163줄, 새 경고 0. 함수 크기 `GetBattlerAffectionHearts` 0x70, `PokemonToBattleMon` 0x230, `GetBattlerMonData` 0x5cc(사전 분석 실측과 같음).
- 테스트: `GITHUB_ACTION=1 make check BUILD=hns -j6 TESTS="AI thinking time"`(`build/port-check-135-ai.log`) → 6개 모두 **PASS**, `test-baseline-seq132.txt`의 같은 6줄과 같다. upstream·HnS 모두 친밀도·마음 테스트는 없다.
- 남은 위험: 없음(게임 동작은 config로 막혀 있고 구조체 크기 불변). `B_AFFECTION_MECHANICS`를 켜면 위 참고 2가지가 upstream과 같게 달라진다.
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
  - `metLevel` 값 원천은 `struct BoxPokemon`의 `u16 metLevel:7`(0..127)이라 7비트에 들어간다. 읽는 곳은 `battle_util.c` 복종 판정의 `>= GEN_8` 분기뿐인데 HnS `B_OBEDIENCE_MECHANICS GEN_7`이라 컴파일 시 제거된다. `PokemonToBattleMon`은 이전처럼 `metLevel`을 채우지 않는다(upstream과 같음).
  - `isShiny`에는 `IsMonShiny()`/`MON_DATA_IS_SHINY`의 0/1만 들어간다. 읽는 곳은 변신 색 복사(`battle_script_commands.c`, `B_TRANSFORM_SHINY`)다.
  - 비트필드 주소를 잡는 코드(`&…metLevel`, `&…isShiny`)는 `src`·`include`·`test`에 없다(빌드 성공으로도 확인).
- 세이브·녹화·링크: 세이브와 녹화 배틀은 무관하다(`BattlePokemon`은 SaveBlock·`RecordedBattleSave`에 없음). 링크 전송 크기는 144 B 그대로다. 140~141번 바이트 배치만 바뀌어 이식 전 ROM과 이식 후 ROM 사이 링크만 어긋난다(다른 full-sync PR과 같은 "다른 버전 간 링크 불가", 같은 ROM끼리는 영향 없음).
- 한글·config·HnS 전용 코드: 해당 없음(비 ASCII 줄 0, config 변경 0).
- 빌드(`build/port.log`): 종료 코드 0, **ROM 32,716,948 B(97.50%, +32 B) / EWRAM 248,936 B(94.96%, 0) / IWRAM 25,516 B(77.87%, 0)**. `pokehns.gba` SHA1 `dfeec48834b4b1ca000cc2b09b39b6175834357b`(사전 분석 스크래치 빌드 `dfeec488…`와 같음). 경고 163줄, 새 경고 0. 크기가 바뀐 함수: `GetBattlerMonData` 0x5cc → 0x5e8, `PokemonToBattleMon` 0x230 → 0x23c(사전 분석 4개 오브젝트 변화와 같은 방향).
- 테스트(파일별 `GITHUB_ACTION=1 make check BUILD=hns -j6 TESTS="<파일>"`, 로그 `build/port-check-136-{transform,imposter,trainer_control,pokemon}.log`): `test/battle/move_effect/transform.c`, `test/battle/ability/imposter.c`, `test/battle/trainer_control.c`, `test/pokemon.c` → **55줄(PASS 47)이 `test-baseline-seq132.txt`의 같은 이름 줄과 모두 같다.** 비 PASS 8건(FAIL 6, TO_DO 2)은 기준과 같은 기존 실패다.
- 남은 위험: 없음. 구조체가 1.17.0과 같아져 뒤 PR의 `BattlePokemon` hunk 문맥이 upstream과 맞는다.
- 실기 확인: 불필요. 원하면 이로치 상대에게 변신(메타몽)했을 때 색이 맞는지 본다.
