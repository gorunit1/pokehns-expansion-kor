# HnS 수정 — 2026-10-04 친구 결정 반영 (seq 128·129·132 후속)

친구 답장 [`FRIEND_REPLY_2026-10-04.md`](../../FRIEND_REPLY_2026-10-04.md)의 질문 1·3·4를 HnS 수정 커밋 3개로 넣었다. 질문 2(더블배틀에서 가로챈 멀리짖기 assert)는 고치지 않고 [`RECHECK_BEFORE_COMPLETION.md`](RECHECK_BEFORE_COMPLETION.md) 1번에 남겼다. 세 커밋 모두 upstream 이식과 섞지 않은 별도 커밋이고 `// HnS:` 주석을 달았다.

시작 HEAD: `60b32d674d`(seq 135~138 완료). 작업 컴퓨터: 데스크탑(`/usr/bin` 툴체인).

## 요약

| 질문 | 커밋 | 파일 | ROM | 내용 |
|---|---|---|---:|---|
| 3. 녹화 배틀 플레이어 뒷모습 | `d78de7fdb5` | `src/battle_controller_recorded_player.c`, 그림 비교 도구 표 | +48 B | `#if IS_HNS` Brendan/May 분기를 지우고 `GetPlayerTrainerPic(gender, GAME_VERSION)`(upstream 1.17.0과 같은 줄). HnS에서 Gold/Kris |
| 4. 전자부유 종료 문장 이름 | `d37911167c` | `src/battle_end_turn.c` | +16 B | `HandleEndTurnMagnetRise()`에 `gBattleScripting.battler = battler;`. upstream 1.17.0·master도 같은 결함 |
| 1. 탈출 아이템 뒤 반사·가로채기 assert, 매직코트·가로채기 승계 | `b07fd88953` | `src/battle_script_commands.c`, `src/battle_main.c` | +32 B | `usedEjectItem` 조기 반환을 반사·가로채기 중에는 건너뜀, 교체 때 `bounceMove`·`stealMove` 초기화(배턴터치 포함) |

- 빌드(`b07fd88953`): 종료 코드 0, **ROM 32,717,044 B(97.50%, +96 B) / EWRAM 248,936 B / IWRAM 25,516 B**, SHA1 `04c307c5833443210e1b7f65126312188b82f3f8`. 세 빌드 모두 새 경고 0.
- 한글 소스 줄 변경 0. 세이브 영향 없음.
- 전체 테스트(`b07fd88953`, 로그 `build/port-check-hnsfix1004.log`): PASSED 2,344 / FAILED 2,256 / KNOWN_FAILING 10 / TOTAL 5,261. 표준 추출 목록 5,192줄이 `test-baseline-seq138.txt`와 **바이트 단위로 같다**(사라진 PASS 0). INVALID 21개 같음, Killed 0. 기준 목록은 그대로 `test-baseline-seq138.txt`를 쓴다.

## 질문 3 — 녹화 배틀 플레이어 뒷모습 Gold/Kris (`d78de7fdb5`)

- seq 128 #9475 이식 때는 이식 전 표시를 유지하려고 `RecordedPlayerHandleDrawTrainerPic`에 `#if IS_HNS (gender == MALE) ? TRAINER_PIC_BRENDAN : TRAINER_PIC_MAY`를 남겼다(`full-sync-seq-128-128.md` "친구에게 물을 것" 1).
- 친구 결정대로 이 분기를 지워 upstream 1.17.0과 같은 `GetPlayerTrainerPic(gender, GAME_VERSION)`로 돌렸다. HnS 분기에서 이 함수는 성별만 보고 Gold/Kris를 돌려준다. 실제 배틀·통신·사파리·볼 던지기 팔레트와 같아진다.
- 그림 비교 도구(`trainerpic-9475/`): `player_cases.tsv`의 `RECORDED_LINK_DRAW_MALE/FEMALE`를 `new_id=TRAINER_PIC_GOLD_HNS/KRIS_HNS`, `expect=changed`(의도한 변화)로, `player_cases_new.tsv`를 GOLD/KRIS로 고쳤다. README도 맞췄다.
- 실기 확인: 배틀 프런티어 기록 재생 등 녹화 배틀에서 플레이어 뒷모습이 Gold/Kris.

## 질문 4 — 전자부유 종료 문장 (`d37911167c`)

- `STRINGID_BUFFERENDS`는 `{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_BUFF1}의 효과가 풀렸다!`다. `HandleEndTurnMagnetRise()`만 `gBattleScripting.battler`를 정하지 않아 직전 배틀러 이름이 나올 수 있었다. 도발 종료 처리기와 같은 방식으로 1줄을 넣었다.
- 검증: seq 132 턴 종료 한글 회귀 테스트(저장소 밖 `hns-sync-work/chunk-132/D-tests/`) 3-07의 기대값을 `상대 마자는 전자부유의 효과가 풀렸다!` → `마자용은 전자부유의 효과가 풀렸다!`로 바꿔 저장소에서 돌렸다. 70개 중 66 PASS / 4 FAIL(의도한 upstream 변경 4개, 이전과 같음). 3-07 PASS.
- `BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 1행을 넣었다.

## 질문 1 — 탈출 아이템 뒤 반사·가로채기, 매직코트·가로채기 승계 (`b07fd88953`)

- 근거: `full-sync-seq-129-129.md` "친구에게 물을 것" 2, 친구 결정(새 포켓몬 자체의 매직미러 반사는 괜찮고, 교체 전 포켓몬의 일시적 매직코트·가로채기만 승계되지 않게). 계획·patch는 저장소 밖 `hns-sync-work/chunk-hnsfix-1004/FIX1.md`·`fix1.patch`.
- 원인
  - 반사(`MoveEndBouncedMove`)·가로채기(`CancelerSnatch`)가 공격자·대상을 저장한 뒤 `attackcanceler`를 다시 지날 때, 그 자리가 이번 턴 탈출버튼·탈출팩으로 들어왔으면 `usedEjectItem` 조기 반환이 행동을 끝내 복원이 일어나지 않았다. `make hns`(RELEASE=0)에서 그 배틀 끝까지 `ValidateBattlers` assert가 반복됐다. upstream 1.17.0·master도 같은 코드다.
  - `SwitchInClearSetData()`가 `bounceMove`·`stealMove`를 지우지 않아, 기절 교체 말고 모든 교체에서 새 포켓몬이 이전 포켓몬의 매직코트·가로채기를 물려받았다(이식 전부터).
- 수정(2파일 +9/−1, `// HnS:`)
  - `Cmd_attackcanceler`: `usedEjectItem` 조기 반환은 반사·가로채기가 진행 중이 아닐 때만 한다. 비트는 남겨 두어 그 자리의 자기 행동은 계속 건너뛴다.
  - `SwitchInClearSetData()`: `bounceMove`·`stealMove`를 지운다. upstream #10338(seq 414) `ClearSetDataOnLeave`와 같은 동작의 선반영이다. 그 PR을 이식할 때 이 4줄을 지운다.
- **배턴터치(메인 결정):** 배턴터치로 교체할 때도 지운다. 친구 결정("교체 전 포켓몬의 일시적 상태가 승계되지 않게")과 upstream 1.17.0(#10338)을 따랐다. 이 경우는 명령(Instruct)으로 매직코트·가로채기를 다시 쓴 뒤 같은 턴에 배턴터치할 때만 생긴다. 사전 분석에 따르면 Showdown은 배턴터치로 두 상태를 복사한다. 본가 동작은 확인하지 못했다. 예외로 두려면 `effect != EFFECT_BATON_PASS` 조건을 붙인다.
- 바뀌는 동작
  - 탈출버튼·탈출팩으로 들어온 매직미러 포켓몬이 같은 턴에 정상적으로 반사한다. assert 화면이 없다.
  - 이전 포켓몬의 매직코트·가로채기는 어떤 교체(탈출버튼·탈출팩·위기회피·강제 교체·유턴류·배턴터치)에서도 새 포켓몬에게 넘어가지 않는다. 기절 교체와 교체가 없는 경우는 그대로다.
- 검증
  - 빌드: ROM +32 B, 새 경고 0.
  - 임시 테스트 16개(커밋하지 않음, 저장소에 잠시 두고 지움): 수정 전 PASS 2 / FAIL 5 / INVALID 9(사전 분석 스크래치 실측) → 수정 뒤 **16/16 PASS**(저장소 실측). 커밋 리뷰 A의 R3·R3b·R4도 INVALID → PASS. R5(가로챈 멀리짖기)는 친구 결정대로 그대로다.
  - 관련 기존 테스트 21파일: 테스트별 결과가 수정 전과 바이트 동일(사전 분석 스크래치).
  - 관찰용 테스트 1개(탈출버튼으로 들어온 춤추기 포켓몬이 같은 턴의 춤 기술을 따라 추지 않음)는 수정 전후 모두 FAIL이다. 이번 수정과 무관한 별건으로 재확인 목록에 넣었다.
  - 출력 변화 문서의 해당 행을 바뀐 동작으로 고쳤다.
- 후속
  - seq 414 #10338 이식 때 `SwitchInClearSetData()`의 HnS 4줄을 지운다(문맥 충돌 예상).
  - `Cmd_attackcanceler`의 HnS 조건은 upstream master까지 같은 코드라 계속 유지한다.
  - seq 166 #9784(탈출팩 재구성) 이식 뒤 임시 테스트를 다시 돌린다.

## 커밋 리뷰

독립 리뷰 1개(읽기 전용, 저장소 밖 `hns-sync-work/chunk-hnsfix-1004/review/`)가 세 커밋을 함께 보고 스크래치 사본(`b07fd88953`과 내용 동일)에서 실측했다. **세 커밋 모두 문제 없음.**

- 빌드 SHA1·ROM이 위와 같다. 분석의 임시 테스트 16/16 PASS. 리뷰어가 새로 만든 임시 테스트 8개도 8/8 PASS다.
  - 전자부유 1줄을 빼면 이름 테스트 2개가 FAIL, 커밋 3을 되돌리면 탈출 아이템 경우 4개가 INVALID다(대조군 2개는 PASS).
  - 확인한 경우: 매직코트 사용자가 튕겨나고 매직미러 교체 포켓몬이 반사, 같은 자리에서 탈출버튼 2회 사이에 반사가 끼어도 자기 행동은 1번만 건너뜀, 이미 행동한 자리의 교체 포켓몬이 반사하고 다음 턴 정상 행동, 반사된 무서운얼굴로 공격자의 탈출팩 발동, 아군 교체는 가로채기를 지우지 않음, 배턴터치는 랭크·전자부유를 그대로 넘김.
- 턴 종료 한글 회귀 66/4 그대로. 관련 기존 테스트 30파일 547개가 수정 전 로그와 테스트별로 같고 새 INVALID 0.
- 부작용 평가(코드 근거)
  - `bouncedMoveIsUsed`·`snatchedMoveIsUsed`는 매 행동 끝(`HandleAction_ActionFinished`)에 지워져 플래그 누수가 없다.
  - `bounceMove`·`stealMove`를 같은 턴 안에서 기대하는 곳은 이미 행동한 자리의 `stealMove`를 보는 `CancelerSnatch`뿐이고, 교체는 그 자리만 지운다.
  - 모든 교체가 `switchindataupdate`를 거친다.
  - Showdown도 매직코트·가로채기를 1턴짜리 포켓몬 상태로 두어 교체되면 사라진다. 배턴터치만은 Showdown이 복사하지만, HnS는 upstream #10338과 친구 결정을 따랐다.
- 리뷰가 짚은 문서·도구 정리를 반영했다.
  - seq 128 결과 문서·`HANDBACK_2026-10-01.md`의 "녹화 배틀은 Brendan/May가 정상" 문구에 이제 Gold/Kris가 정상이라는 주석을 달았다.
  - 그림 비교 도구의 내장 기본 표(`DEFAULT_PLAYER_CASES`)를 `player_cases.tsv`와 맞췄다.

## 실기 확인 항목 (친구용)

1. 녹화 배틀 재생(배틀 프런티어 기록 등): 플레이어 뒷모습이 Gold/Kris.
2. 전자부유: 내 포켓몬이 쓴 전자부유가 5턴 뒤 끝날 때 `[내 포켓몬]은(는) 전자부유의 효과가 풀렸다!`.
3. 탈출버튼: 더블배틀에서 탈출버튼으로 매직미러 포켓몬(에브이 등)이 나온 같은 턴에 상대 변화기를 반사 → 이후 몇 턴 assert 화면이 없는지.
4. (가능하면) 매직코트를 쓴 포켓몬이 같은 턴에 탈출버튼으로 빠진 뒤, 새 포켓몬에게 온 변화기가 반사되지 않고 그대로 맞는지.
