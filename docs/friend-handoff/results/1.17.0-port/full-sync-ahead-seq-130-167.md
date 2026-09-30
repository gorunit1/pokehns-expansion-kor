# full-sync 선진행 결과 — 구간 1 (seq 130~167 중 15행)

진행 중: 마지막 완료 seq 130, 다음 seq 131

**순서표와 다르게 진행한 구간이다.** 순서표([`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv))의 다음 행 seq 127 #9655(배틀 메시지 리팩터)가 D1~D7 결정 대기라서, #9655와 무관한 뒤쪽 행을 앞당겨 이식한다. 선정 기준과 전체 분류는 [`ahead-of-9655/README.md`](ahead-of-9655/README.md)(`ahead_candidates.tsv`)에 있다. 이 구간은 그 "선진행" 114행 가운데 앞쪽 15행(seq 130, 131, 133, 134, 137, 143, 148, 149, 151, 156, 157, 158, 160, 165, 167)이다.

- 이 구간에서 건너뛴 seq 127~129·132·135·136·138.5·139~142·144~147·150·152~155·159·161~164·166은 보류(#9655 의존·#9655 파일·unit/deps 선행 제외·줄 겹침) 또는 제외(이미 적용: seq 138·163) 행이다. 원래 자리에서 진행한다.
- **seq 167 #9819도 이번 구간에서 적용하지 않았다(보류, 중간 회귀 회피: #10548 직전 적용).** 이유는 아래 seq 167 항목.

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), [`CLAUDE_FULL_SYNC_PORT_PROMPT.md`](../../CLAUDE_FULL_SYNC_PORT_PROMPT.md), 메시지 출력 기록: [`BATTLE_MESSAGE_OUTPUT_CHANGES.md`](../../../localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md)
시작 HEAD: `e629de8bdf` (작업 트리 clean. `673240f6ae`(seq 126 코드) 뒤로는 docs만 바뀜)

## 요약

| seq | PR | 판정 | 커밋 | ROM 변화 | 비고 |
|---|---|---|---|---:|---|
| 130 | #9575 | 적용(HnS 적응) | `0f60183f1f` | −192 B | AI 예측 처리 통합. `battle_ai_main.c` 수동 맞춤(HnS `battlerMovesScored` 줄·무조건 디버그 타이머 유지). **예측 AI 트레이너 25명의 AI 동작이 1.17.0과 같아짐** |

## 공통 사항

- 툴체인: 이 컴퓨터의 기본 PATH `arm-none-eabi-gcc` 13.2.1(`/usr/bin`). PATH 접두어 없이 빌드했다.
- 빌드 명령: `GITHUB_ACTION=1 make hns -j8 > build/port.log 2>&1`
- 이식 전 기준: HEAD `e629de8bdf` 빌드(= `673240f6ae` 빌드, 작업 트리의 `pokehns.gba` SHA1 `197afe076fa94dccc2579157efc696d93ca3a63b`) ROM 32,722,852 B / EWRAM 248,944 B / IWRAM 25,516 B.
- 경고 비교: 기준 경고 목록(`build/port-base-full.log`에서 만든 "파일: 메시지" 고유 42개, 사본 `/home/hjm0725/hns-sync-work/chunk-ahead-130-167/warn-base.txt`)과 매 빌드 경고를 같은 형식으로 비교해 목록에 없는 것을 "새 경고"로 셌다.
  `LC_ALL=C grep -a 'warning:' build/port.log | LC_ALL=C sed -E 's/:[0-9]+:[0-9]+: /: /' | LC_ALL=C sort -u | comm -13 warn-base.txt -`
- 테스트 명령: 파일별 `GITHUB_ACTION=1 make check BUILD=hns -j6 TESTS="<파일>"`. 파일마다 따로 돌려 `PORT_INSTRUCTIONS`의 `LC_ALL=C`·`grep -a` 추출로 목록을 만들고, 같은 테스트 이름의 [`test-baseline-seq126.txt`](test-baseline-seq126.txt) 줄과 비교했다. 전체 테스트는 메인이 구간 끝에 돌린다.
- 사전 분석: 읽기 전용 분석 에이전트가 PR별 이식 계획·적응 patch·검증 도구를 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-ahead-130-167/`에 만들었다(`seq<SEQ>-<PR>.md`/`.patch`, `tmp-<seq>/`). 메인이 patch 14개(+#9819)를 스크래치 사본에서 순서대로 쌓아 모두 적용되고 `git diff --check`도 통과함을 확인했다(`stackcheck/`). 적용 직전에 patch를 `git apply --check`로 다시 확인했다. patch에는 파일 모드 줄이 없다.

## 동기화 단위: seq 130 #9575 `U-9575` Unify AI prediction handling

- 현재 판정: 적용(HnS 적응)
- 커밋: `0f60183f1f`
- upstream 근거: `c06999df7d`(5파일 +50/−65, 테스트 변경 없음). deps #9596(seq 113, `f6ef307f76`) 적용됨.
- 수정 파일(5): `include/battle_ai_main.h`, `include/battle_ai_util.h`, `src/battle_ai_main.c`, `src/battle_ai_util.c`, `src/battle_main.c`
- 적용 방법: 사전 분석 patch(`seq130-9575.patch`)를 `git apply`했다. 충돌 없음. `battle_ai_main.c`의 두 hunk는 사전 분석이 HnS 문맥에 맞춰 손으로 만든 것이다(upstream hunk는 HnS 문맥과 달라 그대로 적용되지 않음).
- 내용(upstream):
  - `CanAiPredictMove` 삭제, `ComputeBattlerDecisions` → `ComputeAiBattlerDecisions`. AI 배틀러 판정(`isAiBattler`)을 `HandleTurnActionSelectionState` 호출부로 옮겨 AI 배틀러일 때만 부른다.
  - 턴 시작 `SetAiLogicDataForTurn`: 플레이어 쪽 배틀러마다(플래그 무관) `BattleAI_SetupAIData` + `SetupAIPredictionData(battler)`. 교체 예측(`PREDICT_SWITCH`)과 기술 예측(`PREDICT_MOVE`: `BattleAI_ChooseMoveIndex` → `predictedMove`)을 여기서 한다.
  - `predictingSwitch`/`predictingMove`는 행동을 고르는 AI 배틀러에게 해당 플래그가 있을 때만 굴린다. `IsBattlerPredictedToSwitch`·`GetIncomingMove*`의 플래그 검사 제거.
  - 첫 턴 `SetShellSideArmCategory`·`SetAiLogicDataForTurn`을 `AssignUsableGimmicks()` 바로 뒤로 옮김(AI 코드는 사이의 `gQueuedStatBoosts`·`gBattleCommunication`·`eventState`를 읽지 않음).
- HnS 적응(모두 기존 차이, 이번 PR이 만든 차이 아님):
  - `ComputeAiBattlerDecisions` 끝의 `gAiLogicData->battlerMovesScored |= 1u << battler;` 유지(HnS 원본부터 있던 #9448 형태, 1.17.0에도 같은 자리에 있음).
  - `AIDebugTimerStart/End`에 `if (DEBUG_AI_DELAY_TIMER)`가 없는 HnS 형태(#9585 `d50afab833`) 유지.
  - `SetAiLogicDataForTurn`의 `turnOrder` 두 줄은 HnS에 없다(seq 113에서 제외, 1.17.0에도 없음).
  - g6 plan의 "HnS 난이도 설정 분기"는 이 함수들에 없다(`battle_ai_main.c`의 `IS_HNS`/`HnS:` 0건).
- 제외한 hunk: 없음.
- **upstream대로 둔 변화(AI 동작, 1.17.0과 같아짐):**
  - `AI_FLAG_PREDICTION`을 쓰는 HnS 트레이너 **25명**(`src/data/trainers_hns.party`의 `AI: … Prediction`): `STEVEN_HNS`, `FINLEY_HNS`·`MUALANI_HNS`(더블배틀), `*_POSTOBC_HNS` 22명(관장 16, 사천왕 4, 목호, 레드).
    - 교체 예측 대상이 "AI 배틀러의 맞은편 한 명"에서 "플레이어 쪽 전원"으로 바뀐다(더블배틀에서 차이).
    - 기술 예측이 턴 시작 때 `BattleAI_ChooseMoveIndex`로 한 번 계산된다. 이전의 행동 선택 때 플레이어 배틀러 점수 재계산이 없어져 AI 계산량이 줄었다.
  - 예측 플래그 없는 AI 배틀: `RNG_AI_PREDICT_SWITCH` 굴림이 사라지고, 더블배틀에서는 턴 시작마다 플레이어 배틀러의 `BattleAI_SetupAIData`(`SetRandomTarget`)가 새로 불린다. 난수 소비 순서만 바뀌고 판단 결과는 같다.
  - 인게임 파트너 더블배틀: 플레이어 배틀러의 `battlerMovesScored` 비트가 켜지지 않아 파트너 AI의 `ShouldAvoidProtectingAgainstPartnerMove`가 플레이어 기술을 보지 않는다(1.17.0과 같음).
  - 배틀 메시지 출력 변화는 없다(`BATTLE_MESSAGE_OUTPUT_CHANGES.md` 변경 없음).
- 검증:
  - `git diff --check` 통과. `ComputeBattlerDecisions`·`CanAiPredictMove` 남은 곳 0(`src`·`include`·`test`). 한글 줄·config·세이브 구조체 변경 0.
  - 빌드(`build/port.log`): 종료 코드 0, **ROM 32,722,660 B(97.52%, −192 B) / EWRAM 248,944 B(0) / IWRAM 25,516 B(0)**. 사전 분석 추정 약 −236 B(정렬 전). `pokehns.gba` SHA1 `d3c855dd3ccac508a230b67172f6f3b84db7f070`. 새 경고 0.
- 테스트(지정 9파일: `test/battle/ai/` 아래 `ai_flag_predict_move.c`, `ai_flag_predict_switch.c`, `ai.c`, `ai_check_viability.c`, `check_bad_move.c`, `gimmick_z_move.c`, `ai_switching.c`, `ai_doubles.c`, `ai_multi.c`) → 336줄(PASS 286)이 seq 126 기준 목록의 같은 줄과 **모두 같다**. 예측 테스트 `ai_flag_predict_move.c` PASS 3, `ai_flag_predict_switch.c` PASS 11, `ai.c`의 "AI thinking time doesn't explode" 6건 PASS 유지. 이 PR은 `test/**`를 바꾸지 않는다.
- 남은 위험:
  - 낮음: 녹화 배틀(더블) 재생 중 난수 소비가 바뀌어 이식 전 녹화가 어긋날 수 있다(#8943 A안의 기존 녹화 무효화 허용 범위).
  - 낮음: `SetupAIPredictionData`는 플레이어 쪽 배틀러의 생존 여부를 보지 않는다(1.17.0과 같음).
- 실기 확인: 필요(아래 "실기 확인 항목" 1).

## 실기 확인 항목 (친구용)

이식 전 ROM(`e629de8bdf`, SHA1 `197afe07…`)과 이식 후 ROM을 같은 세이브로 비교한다.

1. **예측 AI 트레이너(#9575):** 예측 AI 트레이너(예: 재대전 관장 1명, 더블배틀 `FINLEY_HNS` 또는 `MUALANI_HNS`, `STEVEN_HNS`)와 싸워 AI의 교체·기술 선택이 멈추거나 이상하지 않은지, 턴 시작 지연이 늘지 않았는지 본다. AI 판단이 1.17.0과 같아지는 변화라 이식 전과 다른 선택을 할 수 있다.

## 후속 행 메모

- (진행하며 추가)
