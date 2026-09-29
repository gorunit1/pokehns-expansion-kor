# full-sync 실제 port 결과 — seq 103~106

진행 중: 마지막 완료 seq 104, 다음 seq 105.

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), [`CLAUDE_FULL_SYNC_PORT_PROMPT.md`](../../CLAUDE_FULL_SYNC_PORT_PROMPT.md)
사전 분석: PR마다 분석 에이전트가 쓴 이식 계획(`hns-sync-work/chunk-103-106/seq<N>-<PR>.md`, 저장소 밖)을 따랐다.
시작 HEAD: `3e745b621e` (작업 트리 clean)

## seq 103~106 공통 사항

- 빌드 명령: `GITHUB_ACTION=1 make hns -j8 > build/port.log 2>&1`. 이 컴퓨터(데스크톱 WSL)에는 `/opt/arm-gnu-toolchain-13.2.Rel1…`이 없어 기본 PATH의 `/usr/bin/arm-none-eabi-gcc` 13.2.1을 썼다.
- 기준 빌드(`3e745b621e`, `build/hns`를 옮겨 두고 전체 재빌드): 종료 코드 0, **ROM 32,714,868 B(97.50%), EWRAM 248,924 B(94.96%), IWRAM 25,516 B(77.87%)**. `pokehns.gba` SHA-1 `3613568d3329886ca25b4f54148bf2c5d267e275`(seq 102와 같음). 경고 163줄, "파일: 메시지"(줄·열 번호 제거) 고유 목록 42개(seq 92~101 기준과 같은 수). 이 빌드의 오브젝트를 스크래치에 복사해 두고 비교에 썼다.
- 경고 비교: 매 빌드의 경고를 같은 형식으로 만들어 기준 목록과 비교했다.
- 테스트: 파일마다 `GITHUB_ACTION=1 make check BUILD=hns -j6 TESTS="<파일>"`로 돌리고, `PORT_INSTRUCTIONS` "테스트" 절의 `LC_ALL=C` 추출 목록을 [`test-baseline-seq101.txt`](test-baseline-seq101.txt)의 같은 이름 줄, 그리고 이식 직전 같은 파일 실행 결과와 비교했다. 이식 직전 실행 결과는 10개 파일 모두 기준 목록에 있는 줄만 나왔다(기준 목록에 없는 줄 0).
- 파일 단위 실행에서만 나오는 상태(추출 정규식 밖, seq 92~101 문서와 같음): `ai_switching.c`의 `AI will not choose to switch out Dondozo with Commander Tatsugiri (1/50): INVALID`, `AI_FLAG_SMART_SWITCHING: AI will stay in if Encore'd into super effective move: ASSUMPTION_FAIL`. `ai_check_viability.c`의 `First Impression …` 2건과 `AI sees increased base power of Grav Apple`도 `ASSUMPTION_FAIL`로 기존과 같다. 이식 전후 모두 같은 상태였다.
- 한글 포함 소스 줄: 커밋마다 `git show <커밋> | LC_ALL=C grep -a -cP '^[-+](?![-+]).*[^\x00-\x7F]'`.

## 동기화 단위: seq 103 #9568 `U-aiconfig-9460` Add AI contextual damage roll configs

- 현재 판정: 적용(HnS 적응: config 전부 MEDIAN, 아군 분기 hunk 제외, 방어 계열 분기 `battlerDef`, Beat Up 버그 교정)
- 커밋: `4bc61b3ffc`
- upstream 근거: `d12a6a46e5`(10파일 +151/−35)
- 해결한 의존성: #9460 `d81b37f15b`(AI config 게터), #9529 `bc3c30671a`(`config_changes.h` 경로), #10342 `ea3b2d9955`(`AI_DEFENDING`이 `battlerDef` 플래그를 봄). `RNG_AI_DMG_ROLL_RANDOM`은 `include/random.h`에 이미 있다.
- 수정 파일(9): `include/battle.h`, `include/battle_ai_util.h`, `include/config/ai.h`, `include/constants/config_changes.h`, `src/battle_ai_main.c`, `src/battle_ai_switch.c`, `src/battle_ai_util.c`, `test/battle/ai/ai.c`, `test/battle/ai/ai_doubles.c`
- 적용 방법: 사전 분석 부록의 HnS 적응 패치를 `git apply`로 넣었다(`--check` 통과). 그 뒤 1줄을 손으로 더했다.
  - **분석 패치에서 빠진 1줄 보충:** `CalcDynamicMoveDamage`의 Skill Link 분기에 `random *= 5;`가 빠져 있었다(분석 표 18번에는 적혀 있음). upstream 커밋과 1.17.0에 있는 줄이라 넣었다. 넣은 뒤 이 함수의 `random` 관련 줄은 1.17.0과 같다. 1.17.0과 다른 곳은 Beat Up 줄과, 이 PR 밖의 `BattleContext`/`GetBattlerParty`/`abilityAtk` 표기뿐이다.
  - 제외한 hunk:
    - `include/random.h`: `RNG_AI_DMG_ROLL_RANDOM`이 이미 있다.
    - `CalcDynamicMoveDamage` 4개 포인터 인자형 signature: HnS는 이미 1.17.0형(`struct SimulatedDamage *simDamage`)이다.
    - `CanIndexMoveFaintTarget` 아군 분기: `.maximum` 하드코드를 유지하고 `// HnS:` 주석만 붙였다(아래 "롤이 이식 전과 같은 근거").
  - HnS 적응:
    - `include/config/ai.h`: `AI_ROLL_ATTACKING`·`AI_ROLL_SHOULD_SETUP_DEFENDING`·`AI_ROLL_ATTACKING_PARTNER`를 upstream `AI_ROLL_MAX` 대신 `AI_ROLL_MEDIAN`으로 두고 `// HnS:` 주석을 달았다. 6개 컨텍스트가 모두 MEDIAN이다(확정 결정 "median으로 현재 난이도 유지").
    - `AI_GetDamage`: 새 `AI_SWITCHIN_DEFENDING`·`AI_SHOULD_SETUP_DEFENDING` 분기는 upstream 원 커밋의 `aiFlags[battlerAtk]` 대신 `aiFlags[battlerDef]`를 본다(#10342형, 1.17.0 최종형과 같음). `AI_SWITCHIN_ATTACKING`·`AI_ATTACKING_PARTNER`는 upstream대로 `battlerAtk`.
    - `AI_CONFIG_DEFINITIONS`에는 `AI_ROLL_ATTACKING` 한 줄만 넣었다(upstream과 같음). 테스트에서 `WITH_CONFIG(AI_ROLL_ATTACKING, …)`로 바꿀 수 있다.
- **upstream 1.17.0과 다른 점 — Beat Up 버그 교정:** upstream `CalcDynamicMoveDamage`는 파티원 수만큼 더한 median을 만든 직후 `maximum = minimum = median = random;`을 실행한다. 대입이 오른쪽부터 되므로 median·min·max가 모두 아직 계산하지 않은 `random`(한 번 친 대미지의 무작위 롤)으로 덮인다. 이 버그는 1.17.0에도 그대로다. HnS는 `maximum = minimum = random = median; // HnS: upstream has 'median = random', which drops the Beat Up sum`으로 넣었다. 이식 전과 같은 합산값을 쓰고 `random`도 합산값이 된다. HnS는 `B_BEAT_UP`=GEN_LATEST라 이 분기가 쓰이고, HnS 트레이너가 Beat Up을 쓴다(`trainers_hns.party`, `trainer_hill.h`). seq 426 #10236이 이 줄을 문맥으로 쓰므로 그때 HnS 줄을 유지한다.
- **롤이 이식 전과 같은 근거(모든 호출처):** 이식 전 `AI_GetDamage`는 ATTACKING(공격자 AI)·DEFENDING(방어자 AI)에서 RISKY/CONSERVATIVE가 없으면 median, 그 밖의 컨텍스트나 AI가 없는 배틀러면 median이었다. 이식 뒤 HnS 설정에서 컨텍스트별로 다음과 같다.

  | 컨텍스트 | 호출처 | 이식 전 | 이식 뒤 |
  |---|---|---|---|
  | `AI_ATTACKING` | 기존 호출 전부 | 공격자 플래그 → 없으면 median | 공격자 플래그 → 없으면 `GetConfig(AI_ROLL_ATTACKING)` = MEDIAN |
  | `AI_DEFENDING` | 기존 호출 전부 | 방어자 플래그 → 없으면 median | 방어자 플래그 → 없으면 `AI_ROLL_DEFENDING` = MEDIAN |
  | `AI_SWITCHIN_ATTACKING` | `battle_ai_switch.c` `ShouldSwitchIfHasBadOdds`, `FindMonThatAbsorbsOpponentsMove`, `GetBestMonIntegrated`(2곳), `GetBestMonVanilla`, `AI_SelectRevivalBlessingMon` | `AI_ATTACKING`(공격자 플래그 → median) | 공격자 플래그 → MEDIAN |
  | `AI_SWITCHIN_DEFENDING` | `GetMaxDamagePlayerCouldDealToSwitchin`, `GetMaxPriorityDamagePlayerCouldDealToSwitchin` | `AI_DEFENDING`(방어자 플래그 → median) | 방어자 플래그(`battlerDef` 적응) → MEDIAN |
  | `AI_SHOULD_SETUP_DEFENDING` | `IncreaseStatUpScoreInternal` → `NoOfHitsForTargetToFaintBattler` | 함수 안 `AI_DEFENDING` 고정 | 방어자 플래그(`battlerDef` 적응) → MEDIAN |
  | `AI_ATTACKING_PARTNER` | `AI_DoubleBattle`, `ShouldUseSpreadDamageMove`의 `GetNoOfHitsToKOBattler` | `AI_ATTACKING`(공격자 플래그 → median) | 공격자 플래그 → MEDIAN |
  | 아군 KO 판정 | `CanIndexMoveFaintTarget` 아군 분기(`AI_DoubleBattle`의 `wouldPartnerFaint` 등) | 항상 max | 항상 max(hunk 제외) |
  | `RecoveryEnablesWinning1v1` | `NoOfHitsForTargetToFaintBattler(…, AI_DEFENDING, …)` | 함수 안 `AI_DEFENDING` 고정 | 인자로 `AI_DEFENDING` — 같음 |
  | `GetBestDmgMovesFromBattler`·`IsBestDmgMove` | `CanIndexMoveFaintTarget(…, AI_ATTACKING)` 고정 → 받은 `calcContext` | 공격자 플래그 → median | `calcContext`가 `AI_DEFENDING`이면 방어자 플래그 → median. 플래그가 없으면 둘 다 median |

  - 새 분기의 조건(`BattlerHasAi(battlerAtk/battlerDef)`)도 이식 전 ATTACKING/DEFENDING과 같은 쪽을 본다. AI가 없는 배틀러면 이식 전후 모두 else 분기(median)다.
  - 결과가 달라질 수 있는 경우는 RISKY/CONSERVATIVE 플래그가 있을 때뿐이다(`GetBestDmgMovesFromBattler`/`IsBestDmgMove`가 보는 플래그 주체가 바뀜). HnS 빌드 트레이너 데이터(`trainers_hns.party`, `battle_partners.party`, `trainers_frlg.party`, `debug_trainers.party`)에 `Risky`/`Conservative`가 0건이다. `AI_FLAG_SMART_TRAINER` 같은 합성 플래그에도 없다. `src/`에서 두 플래그를 켜는 곳은 없다. 디버그 메뉴에서 AI 플래그를 직접 켜는 경우만 예외다. `trainers.party`의 Risky 5건은 HnS 빌드에서 쓰지 않는다(`src/data.c`가 `IS_HNS`면 `trainers_hns.h`).
  - 테스트로도 확인했다: RISKY/CONSERVATIVE를 쓰는 `ai_flag_risky.c`·`ai_choice.c`·`ai_multi.c`와 교체·셋업·아군 판단 파일의 결과가 이식 전과 같다(아래).
- **난수 소비(upstream과 같은 동작, 그대로 둠):** `AI_CalcDamage`가 대미지 기술마다 `RandomRollDmg`(`RandomUniform(RNG_AI_DMG_ROLL_RANDOM, 85, 100)`)를 한 번 부른다. Triple Kick은 타격마다, 2~5회 기술과 Loaded Dice는 한 번 더 부른다. 메인 RNG를 쓰므로 같은 시드에서 이후 난수(명중·급소·실제 대미지 롤·AI 확률 판단)의 순서가 밀린다. `random` 값은 `AI_ROLL_RANDOM`일 때만 쓰이고 HnS 설정에는 그런 컨텍스트가 없으므로 AI 판단과 분포는 같다. 연동 배틀은 양쪽 모두 AI가 없어 소비가 대칭이고, 녹화 배틀은 #8943 A안으로 무효화된다. 테스트 러너는 지정되지 않은 태그의 `RandomUniform`에 `hi`를 돌려주고 RNG를 쓰지 않으므로 이 소비 때문에 결과가 바뀐 테스트는 없다(전후 비교 결과 변화 0).
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 비ASCII 변경 줄 0. 문자열·STRINGID·배틀 메시지 순서 변경 없음.
- 저장·ROM·그래픽 영향: 세이브 영향 없음(`AiLogicData`는 전투 중 힙). `-mabi=apcs-gnu`에서 `struct SimulatedDamage`는 이식 전에도 8바이트라 `u16 random`이 패딩 자리에 들어간다(EWRAM 변화 0으로 확인).
- 검증:
  - `git diff --check`: 통과. 파일 모드(100755) 유지.
  - `make hns -j8`: 종료 코드 0, **ROM 32,715,396 B(+528 B) / EWRAM 248,924 B(0) / IWRAM 25,516 B(0)**. 헤더 변경으로 전체가 다시 컴파일됐고, 경고 163줄·고유 42개가 기준 목록과 같다(새 경고 0). 오브젝트 text+data: `battle_ai_util.o` +396 B, `config_changes.o` +140 B(`GetConfigInternal` 점프 테이블), `battle_ai_main.o`·`battle_ai_switch.o` 0.
  - 자동 테스트(분석 문서 지정 10개 파일, 이식 전후 같은 명령):

    | 파일 | 이식 전(추출 목록) | 이식 뒤 | 변화 |
    |---|---|---|---|
    | `ai.c` | 74줄: PASS 60 / FAIL 14 | 같음 | 이름 변경 1줄(FAIL→FAIL) |
    | `ai_doubles.c` | 50줄: PASS 43 / FAIL 3 | 51줄: PASS 44 / FAIL 3 | 새 PASS 1 |
    | `ai_switching.c` | 121줄: PASS 104 / FAIL 16 / KNOWN_FAILING 1 | 같음 | 0 |
    | `ai_flag_risky.c` | PASS 5 | 같음 | 0 |
    | `ai_choice.c` | PASS 9 / FAIL 1 | 같음 | 0 |
    | `ai_multi.c` | PASS 11 | 같음 | 0 |
    | `ai_flag_attacks_partner.c` | PASS 2 | 같음 | 0 |
    | `ai_smart_tera.c` | PASS 2 / FAIL 2 | 같음 | 0 |
    | `ai_trytofaint.c` | PASS 1 / FAIL 4 | 같음 | 0 |
    | `ai_check_viability.c` | PASS 24 / FAIL 6 | 같음 | 0 |

    - **회귀 0**(PASS→FAIL 없음). 추출 정규식 밖 상태(INVALID·ASSUMPTION_FAIL)도 전후 같다.
    - AI 사고 시간 테스트 6개(`AI thinking time doesn't explode (singles/doubles/Steven multi, no flags/smart)`): 전후 모두 PASS. 릴리스 빌드에서 `GetConfig(AI_ROLL_ATTACKING)`가 함수 호출이 되지만 상한 안이다.
    - 이름 변경: `AI prefers a weaker move over a one with a downside effect … 1/2: FAIL` → `AI prefers a weaker move over one with a downside effect … 1/2: FAIL`(hp 300→320). 실패 사유는 전후 모두 `Unmatched MESSAGE`(`ai.c:371`, 영문 기대값)뿐이다. 옛 이름 줄이 기준 목록에서 빠지는 것은 회귀가 아니다.
    - `AI has a chance to prioritize last chance priority damage over slow KO`(Floatzel Lv90→85): PASS 유지.
    - `AI sees corresponding absorbing abilities on partners`(HP_AWARE 제거, Scratch→Constrict): PASS 유지.
    - 새 테스트 `AI sees random rolls correctly`(`WITH_CONFIG(AI_ROLL_ATTACKING, AI_ROLL_RANDOM)`, `PASSES_RANDOMLY(3, 15, RNG_AI_DMG_ROLL_RANDOM)`): PASS.
    - 난수 소비 증가로 결과가 바뀐 테스트: 없음(위 이유).
  - 실기 확인: 필수 아님(판단 로직이 이식 전과 같음). 아래 "실기 확인 항목"의 Beat Up 트레이너·더블 광역기 항목을 권장.
- 남은 위험: 낮음. 이후 upstream 테스트가 `AI_ROLL_ATTACKING=MAX`를 가정하면 HnS에서 실패할 수 있다. 그때는 "HnS config 차이(결정 6절)"로 기록한다(아래 "후속 행 메모").

## 동기화 단위: seq 104 #9551 `U-9551` Add Intimidate cycling logic for ai switching

- 현재 판정: 적용(선언부·INTIMIDATE case 수동 문맥, Zero to Hero hunk 제외)
- 커밋: `34aec8afd7`
- upstream 근거: `a354d21142`(부모 `d12a6a46e5` = #9568, 7파일 +222/−11)
- 해결한 의존성: #9124(seq 99 `fac71f54b0`), #9568(seq 103 `4bc61b3ffc`). `GetBestDmgFromBattler`·`HasPhysicalBestMove`·`GetIncomingMove`·`AI_IsAbilityOnSide`·`AnyUsefulStatIsRaised`·`GetConfig(B_UPDATED_INTIMIDATE)`가 모두 HnS에 있다.
- 수정 파일(7): `include/battle_ai_switch.h`, `include/battle_ai_util.h`, `include/config/ai.h`, `include/random.h`, `src/battle_ai_switch.c`, `src/battle_ai_util.c`, `test/battle/ai/ai_switching.c`
- 적용 방법:
  - `include/*`, `src/battle_ai_util.c`, `test/battle/ai/ai_switching.c`: upstream diff를 `git apply`로 그대로 넣었다(오프셋만 다름). `SHOULD_SWITCH_INTIMIDATE`/`_STATS_RAISED` 시나리오, `SHOULD_SWITCH_INTIMIDATE_PERCENTAGE 25`/`_STATS_RAISED_PERCENTAGE 10`, `RNG_AI_SWITCH_INTIMIDATE`, `DoesIntimidateRaiseStats` 공개 선언, 주눅(`ABILITY_RATTLED`)은 `GetConfig(B_UPDATED_INTIMIDATE) >= GEN_8`일 때만 TRUE, 새 테스트 7개.
  - `src/battle_ai_switch.c`: 사전 분석 부록 A 패치로 넣었다. upstream과 `+`/`-` 줄을 비교해 새 함수 3개(`IsOpponentPhysicalAttacker`, `CanIntimidateLowerOpponentAtk`, `ShouldSwitchIfIntimidateBenefit`)와 `GetSwitchChance` case, `case ABILITY_INTIMIDATE:` 블록이 upstream과 글자까지 같음을 확인했다.
    - static 선언 3줄은 `IsSwitchinTSpikesAffected` 선언 바로 뒤에 넣었다. HnS는 다음 줄이 `GetPartyMonAbilityForSwitchCalc` 정의라서 upstream 문맥(`InitializeSwitchinCandidate`)과 다르다(seq 99 후속 행 메모).
    - 새 함수 3개는 `CanPalafinZeroSafelyUseHitEscape`(HnS의 1.17.0형 함수) 뒤, `ShouldSwitchIfAbilityBenefit` 앞에 넣었다. upstream은 `GetHitEscapeTransformState` 뒤지만 끝 문맥이 같다.
  - 제외한 hunk: `ShouldSwitchIfAbilityBenefit`의 Zero to Hero 루프 교체. HnS는 이미 1.17.0 최종형(`GetBattlerMoveIndexWithEffect`, `SPECIES_PALAFIN_ZERO` 검사, `CanPalafinZeroSafelyUseHitEscape`)이다. upstream #9551 형태(`hitEscapeMove`/`GetHitEscapeTransformState`)로 되돌리지 않았다.
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - 비ASCII 변경 줄 0. 문자열·STRINGID·배틀 메시지 순서 변경 없음. AI가 교체를 고르는 빈도만 바뀐다. 교체·위협 문구는 기존 HnS 경로를 탄다.
  - `IsOpponentPhysicalAttacker`는 upstream대로 `GetBattleMoveCategory`를 쓴다. HnS `GetBattleMoveCategory`에는 챌린지 `optionStyle == 1`(타입별 물리/특수) 분기가 있으므로, 이 옵션을 켜면 "물리 공격자" 판정도 자동으로 타입 기준이 된다. `GetMoveCategory`로 바꾸지 않았다.
  - `B_UPDATED_INTIMIDATE`는 HnS에서 `GEN_LATEST`라 주눅은 이전처럼 TRUE이고, `CanIntimidateLowerOpponentAtk`의 Gen8+ 4특성(정신력·배짱·마이페이스·둔감) 차단이 켜진다. 엔진의 위협 처리와 같다.
  - HnS 전용 코드(`GetPartyMonAbilityForSwitchCalc`, seq 99의 `// HnS:` 구 시트러스 분기·HP 0 클램프, Supreme Overlord 카운터)는 건드리지 않았다.
- 저장·ROM·그래픽 영향: 세이브 영향 없음. `enum ShouldSwitchScenario`와 `RandomTag` 중간 삽입으로 뒤 값이 1씩 밀리지만 저장하지 않는다.
- 실전 영향: `AI_FLAG_SMART_SWITCHING`(= Smart Trainer) 트레이너가 위협 몬을 들고 있고, 플레이어 쪽에 위협으로 공격을 낮출 수 있는 물리 공격자가 있으면 25%(능력 랭크 상승 중이면 10%) 확률로 위협 몬을 교체한다. 상대 가운데 위협으로 이득을 보는 특성(오기·승기·주눅·미러아머 등)이 하나라도 있으면 교체하지 않는다. HnS에서 해당하는 트레이너는 `TRAINER_FINLEY_HNS`(핀리, 더블, 보만다), `TRAINER_BUGSY_POSTOBC_HNS`(호일 PostOBC, 비나방), `TRAINER_BRUNO_POSTOBC_HNS`(시바 PostOBC, 켄타로스 팔데아 블레이즈), `TRAINER_LANCE_POSTOBC_HNS`(목호 PostOBC, 보만다·갸라도스)다. 모두 `Smart Trainer / Prediction`이다.
- 검증:
  - `git diff --check`: 통과. 파일 모드(100755) 유지(`git diff --summary` 빈 출력).
  - `make hns -j8`: 종료 코드 0, **ROM 32,716,324 B(+928 B) / EWRAM 248,924 B(0) / IWRAM 25,516 B(0)**. 경고 163줄·고유 42개가 기준과 같다(새 경고 0). 오브젝트 text+data: `battle_ai_switch.o` +764 B, `battle_ai_util.o` +48 B, `battle_ai_main.o` 0. 분석 추정 +812 B보다 조금 크다(링크 정렬 차이).
  - 자동 테스트(이식 전 = seq 103 커밋 뒤 같은 명령 결과):
    - `ai_switching.c`: 121줄 → 128줄. **새 테스트 7개 모두 PASS**(싱글 3: 물리 공격자 상대 교체 `PASSES_RANDOMLY` 25%, 클리어바디·오기 상대 미교체, 특수 공격자 상대 미교체 / 더블 4: 한쪽이라도 유효하면 교체, 양쪽 차단 시 미교체, 한쪽 오기 시 미교체, 양쪽 특수 시 미교체). 기존 121줄은 같다. `AI_REVERSE_BATTLER_LOGIC_ORDER_CHANCE` 순서 문제로 실패한 더블 테스트는 없었다.
    - `ai_flag_predict_switch.c`(PASS 11), `ai_doubles.c`(51줄), `ai.c`(74줄), `ai_double_ace.c`(PASS 3 / FAIL 1), `ai_multi.c`(PASS 11), `ai_flag_sequence_switching.c`(PASS 2 / FAIL 2), `test/battle/ability/rattled.c`(FAIL 4), `intimidate.c`(PASS 5 / FAIL 11): 이식 전과 같다. 이 파일들의 FAIL은 기존 `Unmatched MESSAGE` 등 기준 목록 그대로다.
    - **회귀 0.** 추출 정규식 밖 상태(Dondozo INVALID, Encore ASSUMPTION_FAIL)도 전후 같다.
  - 실기 확인: 권장(아래 "실기 확인 항목" 1).
- 남은 위험: 낮음. upstream과 같은 한계로, 현재 몬이 1:1에서 이기는 경우에도 교체할 수 있다(코드의 TODO, 1.17.0에도 있음). 예측 트레이너(Smart Trainer / Prediction)가 플레이어의 위협 몬 교체를 예측할 때 `RNG_AI_SWITCH_INTIMIDATE`를 소비한다(upstream과 같은 동작).
