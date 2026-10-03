# full-sync 실제 port 결과 — seq 132 (#9680 턴 종료 효과의 BattleScriptCall 전환)

완료: seq 132 #9680 `U-endturn-9680`을 이식했다. 영역별 사전 분석 4개(A 엔진·B 스크립트·C 호출부·D 출력 순서 회귀 테스트)의 patch를 메인이 합쳐 커밋 1개로 넣었다. 빌드·D 회귀 테스트(이식 전후)·관련 테스트·전체 테스트를 마쳤다. 다음 seq는 **135 #9709**다(133·134는 선진행으로 이미 적용). 선진행으로 넣은 seq 133·134·137·143·148·149·151·156·157·158·160·165와 선반영한 seq 274는 닿으면 "이미 적용"으로 처리한다.

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), g6 plan `9680` 행([`g6_overworld_refactors_plan.tsv`](../1.17.0-sync-plan/g6_overworld_refactors_plan.tsv)), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), 메시지 출력 기록: [`BATTLE_MESSAGE_OUTPUT_CHANGES.md`](../../../localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md)
시작 HEAD: `18f412e9ff`(작업 트리 clean, 코드는 `587f4e7cdc`과 같음). 작업 컴퓨터: 데스크탑(`/usr/bin` 툴체인).

## 요약

| seq | PR | 판정 | 커밋 | ROM 변화 | 비고 |
|---|---|---|---|---:|---|
| 132 | #9680 | 적용(HnS 적응) | `5f580ab12e` | +16 B | 턴 종료 진행을 `BattleTurnPassed` → `BattleScriptExecute(BattleScript_EndTurnEvents)` → `endturnevents`(`BS_EndTurnEvents` → `EndTurnEvents()`)로 바꾸고, 턴 종료 처리기가 `BattleScriptCall`로 `return`형 스크립트를 부른다. 단계 enum에 `ENDTURN_ARENA_TURN_END`·`ENDTURN_FAINTED_MON_ACTIONS`를 넣고 `ENDTURN_DYNAMAX`를 맨 뒤로 옮겼다. HnS 턴 종료 스크립트(맹독·화염구슬, 아이스바디, 치유의마음, 스위트베일 하품, 방벽별 `*WoreOff`)는 본문을 두고 끝만 `return`으로 바꿨다. 하양허브 내던지기 분기 유지, 안개제거 `DEFOG_CLEAR` hunk 제외 |

- 빌드(`5f580ab12e` 작업 트리): 종료 코드 0, **ROM 32,716,900 B(97.50%, +16 B) / EWRAM 248,936 B(94.96%, 0) / IWRAM 25,516 B(77.87%, 0)**. `pokehns.gba` SHA1 `512ccdbecee5dbcde2f777fcb96e20557bdb1015`(사전 분석 A의 스크래치 빌드 SHA1 `512ccdbe…`와 같음).
- 새 경고 0(경고 21줄 모두 기준 안).
- 한글이 든 소스 줄 변경: **0**(diff의 `+`/`-` 줄 가운데 비 ASCII 줄 0). 문자열·`STRINGID`·토큰 변경 없음, 새 한글 문장 없음.
- 세이브: 영향 없음(아래 "세이브·메모리").
- **D 출력 순서 회귀 테스트(임시, 커밋 안 함):** 이식 전 **70/70 PASS** → 이식 후 **66 PASS / 4 FAIL**. FAIL 4개는 모두 이름에 `UPSTREAM … CHANGE EXPECTED`를 붙인 upstream 의도 변경이다(다이맥스 3·매직룸 종료 루프의 하양허브 1). 사전 분석 D 예측 목록과 바이트 단위로 같다.
- 관련 테스트 57파일(420개): 이식 전 스크래치 결과와 결과·실패 사유 줄까지 같다.
- 전체 테스트: PASSED **2,344** / FAILED 2,256 / TOTAL 5,261. `test-baseline-seq129.txt` 대비 **사라진 PASS 0**, FAIL→PASS 4(볼주머니 2·강제 리샘열매 2). 새 기준 목록 [`test-baseline-seq132.txt`](test-baseline-seq132.txt).
- `BATTLE_MESSAGE_OUTPUT_CHANGES.md`: 새 행 1개(하양허브 강제 발동 경로)와 검증 상태 문단 1개.

## 공통 사항

- 툴체인: 데스크탑 기본 PATH `arm-none-eabi-gcc` 13.2.1(`/usr/bin`). PATH 접두어 없이 빌드했다.
- 빌드 명령: `GITHUB_ACTION=1 make hns -j8 > build/port.log 2>&1`.
- 이식 전 기준(데스크탑, `587f4e7cdc` 코드): ROM 32,716,884 B / EWRAM 248,936 B / IWRAM 25,516 B, SHA1 `f453524c011ec4ed23dc7ff3e1225d98c9752c05`.
- 경고 비교: `LC_ALL=C grep -a 'warning:' build/port.log | sed -E 's/:[0-9]+:[0-9]+: /: /' | sort -u | comm -13 warn-base.txt -` → 0줄. 기준은 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-ahead-130-167/warn-base.txt`.
- 사전 분석: 읽기 전용 분석 4개가 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-132/`에 patch와 스크래치 검증을 남겼다.
  - A(`part-A.md`/`.patch`, 4파일): 턴 종료 엔진(`battle_end_turn.c`, `battle_end_turn.h`, `battle_main.c/.h`). upstream hunk 69개 중 59개 그대로, 10개 손으로 맞춤, 제외 0
  - B(`part-B.md`/`.patch`, 3파일): 배틀 스크립트(`battle_scripts_1.s`, `battle_scripts.h`, `battle_script.inc`). `battle_scripts_1.s` 66 hunk 중 56 그대로, 헤더 14 중 8 그대로, 나머지 손으로 맞춤. HnS 전용 스크립트 7개 `return`형 변환
  - C(`part-C.md`/`.patch`, 5파일): 나머지 호출부(`battle_util.c/.h`, `battle_hold_effects.c`, `battle_script_commands.c`, `battle_switch_in.c`). hunk 27개 중 20 그대로, 4 손으로 맞춤, 3 제외. upstream diff 밖 사용처 전수 조사 0곳
  - D(`part-D.md`, `D-tests/`): 이식 전 턴 종료 출력을 고정하는 임시 한글 테스트 4파일 70개, 실행 스크립트, 이식 전 기준 로그·trace
- 적용: `git apply --check` 뒤 A → B → C 순서로 쌓았다(모드 줄 없음, 충돌 없음). `git diff --check` 통과. patch md5 A `fd001691…`, B `4cc862cb…`, C `d0d6bfbb…`(사전 분석 D가 66/4 예측에 쓴 patch와 같음). 적용 뒤 12파일이 사전 분석 사본 `tmp-D/wabc`의 같은 파일과 바이트 단위로 같다.

## 동기화 단위: seq 132 #9680 `U-endturn-9680` Make End Turn events use BattleScriptCall (return instead of end2)

- 현재 판정: 적용(HnS 적응)
- 커밋: `5f580ab12e`
- upstream 근거: `611ed45952`(12파일 +331/−339). HnS 커밋도 같은 12파일, +335/−327(차이는 HnS 스크립트 유지·HnS 전용 스크립트 변환·제외 hunk).
- 수정 파일(12): `src/battle_end_turn.c`, `include/constants/battle_end_turn.h`, `src/battle_main.c`, `include/battle_main.h`, `data/battle_scripts_1.s`, `include/battle_scripts.h`, `asm/macros/battle_script.inc`, `src/battle_util.c`, `include/battle_util.h`, `src/battle_hold_effects.c`, `src/battle_script_commands.c`, `src/battle_switch_in.c`
- 적용 방법: 사전 분석 `part-A.patch` → `part-B.patch` → `part-C.patch`(위 "공통 사항").
- 내용:
  - 흐름: `BattleTurnPassed()`는 `BattleScriptExecute(BattleScript_EndTurnEvents)`만 한다. `BattleScript_EndTurnEvents`(`endturnevents` + `end2`)가 `BS_EndTurnEvents` → `EndTurnEvents()`를 부르고, 처리기가 효과를 내면 `BattleScriptCall`로 효과 스크립트를 부른 뒤 `return`으로 `endturnevents`에 돌아와 다음 효과로 간다. 끝나면 `SetBattleCallback(HandleTurnActionSelectionState)`로 콜백 스택 맨 위를 바꾸고, 배틀팰리스 맛 문구·배틀아레나 턴 시작 문구는 `BS_EndTurnEvents`가 `…Ret` 스크립트를 Call한다.
  - 단계: `ENDTURN_ARENA_TURN_END`(아레나 판정, `BattleArenaTurnEnd()` 삭제)·`ENDTURN_FAINTED_MON_ACTIONS`(기절 처리)를 슬라이드 뒤에 넣고 `ENDTURN_DYNAMAX`를 맨 뒤로 옮겼다. 승패가 정해지면 `DoEndTurnEffects()`가 `ENDTURN_TRAINER_A_SLIDES`로 건너뛴다.
  - `battle_end_turn.c`의 `BattleScriptExecute` 74줄(설명 주석 2줄 포함) → 0줄(`BattleScriptCall` 75줄, upstream 뒤 파일과 같은 수). `battle_hold_effects.c` Execute 0. `battle_util.c` Execute는 정의와 `GiveExp`·`HandleFaintedMon`·`DancerActivates` 3곳만 남는다(upstream 1.17.0과 같은 집합).
  - 스크립트: 턴 종료 스크립트 73개(지역 라벨 포함, 사전 분석 B `tmp-B/converted.txt`)의 `end2` → `return`, 이름 변경(`TerrainPrevents`·`SleepClausePrevents`·`YawnMakesAsleep`·`ItemHurtWithAnim`·`EjectPackActivate_NoQueuedSwitch`·`EmergencyExitSendReplacement`·`AttackerAbilityStatRaiseRestoreAttacker`·`ImmunityProtectedRet`), 래퍼 삭제(`BattlerFormChangeEnd2`·`…End3NoPopup`·`EmergencyExitEnd2`·`RainDishActivates`·`AttackerAbilityStatRaiseEnd2`·`WhiteHerbEnd2`·`ItemHealHP_End2`·`MentalHerbCureEnd2`·`MirrorHerbCopyStatChangeEnd2`·`QueuedSwitch*End2`), `BattleScript_AbilityHpHeal` 전역화, `PalacePrintFlavorTextRet`·`ArenaTurnBeginningRet` 분리. opcode 표 불변(`endturnevents`는 `callnative`).
- **HnS 적응(메인 결정: part-B 결정 1~3 모두 patch 기본안):**
  - 맹독구슬·화염구슬: upstream은 `BattleScript_ToxicOrb`·`FlameOrb`를 지우고 `BattleScript_MoveEffectToxic/Burn`을 부르지만, HnS 스크립트(아이템 팝업 + `STRINGID_PKMNPOISONEDBY`/`PKMNBURNEDBY`)를 `@ HnS:` 주석과 함께 남겼다. 꼬리 `BattleScript_UpdateEffectStatusIconEnd2`를 `return`형 지역 라벨 `BattleScript_UpdateEffectStatusIconOrbRet`(명령 같음)으로 바꾸고, C는 `BattleScriptCall(BattleScript_ToxicOrb/FlameOrb)`(`// HnS:` 주석)만 부른다. 공용 꼬리 `UpdateEffectStatusIconRet`(싱크로·`ON_STATUS_CHANGE` 검사)과 합치지 않았다(결정 1).
  - 아이스바디(`IceBodyHeal`, 문구 없음)·치유의마음(`HealerActivates`, `STRINGID_HEALERCURE`): upstream 새 본문(`ICEBODYHPGAIN` 문구, `gCureStatusStringIds`)을 들이지 않고 HnS 본문 + `return`.
  - 스위트베일 하품: HnS `BattleScript_AbilityMadeIneffectiveEnd2`(`STRINGID_PKMNSXMADEITINEFFECTIVE`)를 `BattleScript_AbilityMadeIneffectiveRet`로 이름을 바꾸고 `return`. `battle_end_turn.c`는 upstream의 `ImmunityProtectedRet` 대신 이것을 부른다(`// HnS:` 1줄, 결정 3 이름 규칙).
  - 리플렉터·빛의장막·흰안개·오로라베일 종료: upstream 공용 `SideStatusWoreOff` 대신 HnS 방벽별 `*WoreOff`를 Call하고, 네 스크립트를 이름 그대로 `return`형으로 바꿨다. 안개제거용 `*WoreOffReturn`과 본문이 같아지지만 합치지 않았다(결정 2).
  - 하양허브: `RestoreWhiteHerbStats()`의 `timing` 인자와 HnS 내던지기 분기(`BattleScript_WhiteHerbFling`, `removeitem` 없음)를 유지하고, 나머지 두 분기만 `BattleScriptCall(BattleScript_WhiteHerbRet)` 하나로 합쳤다(팝업은 `WhiteHerbRet` 첫 줄이라 유지).
  - 나이트메어: upstream 팝업 블록 재배치를 따르고 HnS 선이식분 `setbyte sFIXED_ABILITY_POPUP, FALSE`를 유지(1.17.0 본문과 같음). 시럽봄 `BS_TARGET`(선이식 #10042), 멘탈허브 HnS 비트마스크 본문 유지.
  - 젖은접시·건조피부(비): `RainDishActivates`는 `call AbilityHpHeal`/`end2`일 뿐이라 upstream처럼 `AbilityHpHeal` 직접 Call로 합쳤다(같은 명령, 문구 없음 유지).
  - 문맥만 다른 hunk: 미래예지 4인자(`SetTypeBeforeUsingMove`), 탁탁폭탄 대상 2줄(#10042 선반영), 전자부유(#10318 선반영), 변덕쟁이(`isFirstTurn != 2` 감싸기 없음), `endturnevents` 매크로 위치(HnS `destroyitempopup` 뒤), `battle_main.c` 뒤 함수 문맥.
- 제외한 hunk(3):
  - `battle_hold_effects.c` `RestoreWhiteHerbStats(battler, timing)` → `(battler)` 시그니처와 호출부 2곳: HnS 내던지기 분기에 `timing`이 필요하다.
  - `battle_script_commands.c` 안개제거 `DEFOG_CLEAR`의 `SideStatusWoreOffReturn` → `SideStatusWoreOff`: HnS는 방벽별 `*WoreOffReturn`·`SafeguardEndsReturn`(원래 `return`형)을 쓴다.
- 보존 확인: `// HnS:` 소란 방음 규칙(#9616), 하품 `GEN_CHAMPIONS` 수면 턴·절대안깸 조건, 첫 턴 `BattleScript_ArenaTurnBeginning`(래퍼 유지)·`Trainer*SlideMsgEnd2`(첫 턴 경로) 그대로. upstream diff 밖에서 지워지거나 이름이 바뀌는 심볼을 쓰는 곳 0(`test/`·AI·HnS 전용 파일 포함, 사전 분석 C 6절). config 기본값 변경 없음.
- 정적 종결자 검사(`tmp-B/reach.py`, 저장소 작업 트리): A·C가 `BattleScriptCall`로 부르는 스크립트 93개 가운데 `return` 외 종결자에 닿는 것은 upstream과 같은 3개뿐이다 — `ArenaDoJudgment`(`end2`), `MonTookFutureAttack`(빗나감 경로 `MoveEnd`의 `end`), `EmergencyExitSendReplacement`(야생 경로 `finishaction`). 나머지 1개 `BattleScript_X`는 주석 예시다.
- 빌드: 종료 코드 0, ROM 32,716,900 B(+16 B) / EWRAM 248,936 B(0) / IWRAM 25,516 B(0), SHA1 `512ccdbe…`, 새 경고 0.
- 검증: 아래 "D 출력 순서 회귀 테스트", "관련 테스트", "전체 테스트".
- 남은 위험:
  - **upstream 본문에서도 `return`으로 끝나지 않는 경로 3개**(위 정적 검사, upstream과 같음).
    - 배틀아레나 판정 `ArenaDoJudgment`가 `end2`로 `EndTurnEvents` 스크립트 전체를 끝낸다(스크립트 스택 1칸 남음, 다음 턴 시작에 정리). 테스트 러너가 아레나 배틀을 만들 수 없어 실기로만 볼 수 있다.
    - **미래예지·파멸의소원이 빗나가면** `MonTookFutureAttack`의 `accuracycheck` → `MoveMissedPause` → `MoveEnd`의 `end`에 닿는다. D 테스트(2-11·2-17)와 관련 테스트(`future_sight.c`)는 맞히는 경우와 대상 기절만 다뤄 **빗나감 경로는 확인하지 못했다.** upstream #9939(seq 470)가 이 `accuracycheck`를 없앤다.
    - 야생 배틀 턴 종료 위기회피(`setteleportoutcome` 뒤 `finishaction`). 트레이너 배틀 경로는 D 4-04·4-05로 고정했다.
  - 기절 처리(`HandleFaintedMonActions`)는 upstream처럼 여전히 `BattleScriptExecute(GiveExp/HandleFaintedMon)`다. 스크립트 스택이 0일 때만 안전하며, 기절 처리 앞의 Call 스크립트가 모두 `return`하는 것을 위 정적 검사로 확인했다.
  - 승패 결정 뒤 점프 대상이 `ENDTURN_TRAINER_A_SLIDES`라 #9211(seq 361)에서 슬라이드가 맨 뒤로 가면 아레나·기절·다이맥스 처리도 건너뛰는 쪽으로 바뀐다(그때 재확인).
  - 맹독·화염구슬 꼬리가 upstream 공용 꼬리와 다르다(싱크로·상태 회복 도구 검사 없음, 이식 전과 같은 동작). #9777(seq 475)에서 다시 정한다.
  - 턴 종료 단계가 컨트롤러 대기 뒤에 진행되고 `end2`→`return` 처리 프레임이 1~2프레임 다르다. 화면 순서는 D 테스트·trace로 같음을 확인했지만 대기 시간은 실기로만 볼 수 있다.
- 실기 확인: 필요(아래 "실기 확인 항목").

## D 출력 순서 회귀 테스트 결과

사전 분석 D가 만든 임시 테스트 4파일(`/home/hjm0725/hns-sync-work/chunk-132/D-tests/zz_hns9680_endturn{1,2,3,4}.c`, md5 `3b25b7f1…`·`343455ea…`·`b6b26b1b…`·`b5329562…`)을 저장소 `test/battle/`에 잠시 복사해 `make check BUILD=hns -j8 TESTS="HNS9680 "`로 돌리고 바로 지웠다(`ALLOW_REPO=1 D-tests/run.sh`). 두 번 모두 실행 뒤 `git status`에 테스트 파일이 남지 않았다. 결과는 저장소 밖 `D-tests/runs/repo-pre*`·`repo-post*`에 있다. `trace.patch`는 저장소에 넣지 않았다.

| 실행 | 트리 | 결과 | 기준과 비교 |
|---|---|---|---|
| 이식 전 | `18f412e9ff`(코드 `587f4e7cdc`) | **PASS 70 / TOTAL 70** | `baseline/before-wb-summary.txt`와 바이트 동일 |
| 이식 후 | A+B+C 적용 작업 트리(= `5f580ab12e`) | **PASS 66 / FAIL 4 / TOTAL 70** | `baseline/expected-after-abc-summary.txt`(사전 분석 D 예측)와 바이트 동일 |

- 테스트 목록 요약(모두 한글 `MESSAGE`·`ABILITY_POPUP`·`ANIMATION`·`HP_BAR`·`STATUS_ICON` 순서 고정, 상세 표는 `part-D.md` 3절):
  - 1파일(18개) HnS 고유 도구·특성: 맹독구슬·화염구슬(팝업 → 상태 애니 → `…맹독구슬 때문에 맹독에 중독됐다!`/`…화염구슬 때문에 화상을 입었다!`), 포이즌힐, 아이스바디, 선파워·건조피부, 젖은접시·건조피부(비), 먹다남은음식·검은오물·끈적끈적바늘, 하양허브 턴 종료 경로 2(변덕쟁이·문어굳히기), 탈피·촉촉바디, 치유의마음(`치유되었다!`), 가속·변덕쟁이, 수확·되새김질, 픽업, 나이트메어(팝업 재표시) 2, 슬로스타트
  - 2파일(18개) 날씨·잔류 피해·지연 효과: 비·모래바람·싸라기눈·쾌청·설경 지속/종료, 독·맹독·화상·동상, 씨뿌리기(해감액·회복봉인), 김밥말이, 저주·악몽, 소금절이·문어굳히기·시럽봄, 아쿠아링·뿌리박기, 그래스필드, 희망사항·미래예지, 맹세 조합 필드, 멸망의노래, 하품(일렉트릭/미스트필드·스위트베일 HnS 문구·잠듦 조항), 거다이옥염
  - 3파일(16개) 시간 제한 효과·소란: 도발·앙코르·사슬묶기·트집·회복봉인·금제·텔레키네시스·전자부유 종료, 방벽·신비의부적·흰안개·순풍·주술·오로라베일 HnS 진영별 문구, 트릭룸·중력·물놀이·흙놀이·원더룸·매직룸 종료, 필드 종료, 소란 지속·종료·Gen 4 깨우기·방음
  - 4파일(18개) 폼 체인지·교체·기절·승패·다이맥스·슬라이드·단계 순서: 달마모드·배고픈스위치·스웜체인지·어군·리밋실드, 위기회피 2, 탈출팩 2, 턴 끝 기절 → 남은 효과 → 교체(싱글·더블), 승리, 다이맥스 종료, 트레이너 슬라이드, 한 턴 끝 단계 순서(더블)
- FAIL 4개(모두 이름에 `UPSTREAM … CHANGE EXPECTED`, upstream 의도 변경):

| 테스트 | 이식 전 | 이식 후 | 원인 | HnS 게임 |
|---|---|---|---|---|
| 4-14 | 독 기절 → 다이맥스 종료 애니 → 교체 문장 | 독 기절 → 교체 문장 → 다이맥스 종료 애니 | `ENDTURN_DYNAMAX`가 `ENDTURN_FAINTED_MON_ACTIONS` 뒤로 이동 | 미도달 |
| 4-16 | 다이맥스 종료 애니 → 트레이너 슬라이드 문장 | 슬라이드 문장 → 다이맥스 종료 애니 | `ENDTURN_DYNAMAX`가 슬라이드 뒤로 이동 | 미도달 |
| 4-18 | 플레이어 다이맥스 중 마지막 상대가 턴 끝에 기절하면 다이맥스 종료 애니 없이 승리 | 기절 뒤 다이맥스 종료 애니(`FORM_CHANGE`) → 승리 | upstream `HandleEndTurnDynamax`가 플레이어 승리 시 즉시 해제 | 미도달 |
| 3-16 | 매직룸 종료 루프가 하양허브에서 끝나고, 상대 자뭉열매는 다음 턴 내 행동 뒤 발동 | 같은 루프에서 하양허브 → 상대 자뭉열매 | `RestoreWhiteHerbStats`의 `Execute(WhiteHerbEnd2)` → `Call(WhiteHerbRet)` | 도달(출력 변화 행) |

- 다이맥스 3개: HnS는 `B_FLAG_DYNAMAX_BATTLE 0`이고 HnS 트레이너 다이맥스 0건, 트레이너 슬라이드 표도 비어 있다. 테스트 러너의 시험용 트레이너에서만 보인다. 출력 변화 문서에는 넣지 않았다.
- 이식 후 결과가 사전 분석 D 예측과 같으므로, 사전 분석 D의 trace 비교(같은 patch로 만든 사본 `tmp-D/wabct`: 이식 전과 다른 줄이 이 4개 테스트 안에만 있음, 아이템 팝업 17개·HnS 문장·팝업·애니메이션 순서 동일)가 이번 커밋에도 그대로 적용된다(위 "공통 사항"의 12파일 바이트 동일). 저장소에서는 trace를 다시 돌리지 않았다.

## 출력 변화

- **`BATTLE_MESSAGE_OUTPUT_CHANGES.md` 새 행 1개(「특성·도구·도주」): 실행 중인 배틀 스크립트 안에서 하양허브가 강제 발동하는 경로**(트릭·스위처로 받음, 나눔으로 줌, 나쁜손으로 훔침, 매직룸 종료 루프). 이전에는 `BattleScriptExecute(WhiteHerbEnd2)`가 커서를 덮어써 호출한 스크립트의 나머지(받은 도구 발동, 공생, `MoveEnd`, 매직룸 루프의 나머지 배틀러)가 실행되지 않고 아이템 팝업 태스크가 남았다. 이식 뒤에는 이어서 실행된다. upstream 1.17.0과 같은 동작이라 D4로 받아들였다(메인 결정). 새 한글 문장 없음. 근거: 사전 분석 C의 스크래치 임시 테스트 5개(트릭 3·매직룸 1·턴 종료 순서 1)가 이식 전 PASS 1·FAIL 4(`Unmatched ANIMATION` 1, `task not freed` 3) → 이식 뒤 PASS 5, D 3-16.
- 다이맥스 종료 위치·승리 시 해제(위 D 4-14·4-16·4-18): HnS 미도달이라 이 문서에만 적는다.
- 승패가 정해진 뒤에도 트레이너 슬라이드 단계가 실행된다(upstream). HnS `sTrainerSlides`·`sFrontierTrainerSlides`가 비어 있어 게임 출력은 없다.
- 턴 종료 효과의 HnS 출력은 바뀌지 않는다(D 66개 PASS). 맹독구슬·화염구슬 팝업과 문장, 아이스바디·포이즌힐·선파워·젖은접시·건조피부 무문구, 치유의마음 HnS 문장, 방벽별 해제 문구, 스위트베일 하품 문구, 나이트메어 팝업 재표시, 소란 Gen 4 깨우기·방음, 하양허브 턴 종료 경로(팝업 포함) 모두 이식 전과 같다.
- 배틀팰리스 맛 문구·배틀아레나 턴 시작 문구는 `BattleTurnPassed` 끝의 Execute에서 `BS_EndTurnEvents`의 Call로 옮겨졌다(같은 스크립트 본문). 테스트 러너가 두 배틀을 만들 수 없어 실기로만 확인할 수 있다.

## 관련 테스트

사전 분석 B(45파일)·C(38파일)가 적은 파일의 합집합 57파일을 저장소에서 파일별로 돌렸다(`GITHUB_ACTION=1 make check BUILD=hns -j8 TESTS="test/battle/<파일>.c"`, 스크립트·결과는 저장소 밖 `chunk-132/apply/related.sh`·`related-post/`). 이식 전 결과는 사전 분석의 HEAD 사본 실행 결과(B `tmp-B/res-head/`, C 전용 12파일은 `tmp-C/logs/before-*.log`를 같은 형식으로 추출, `apply/related-pre/`)다.

- 대상: 도구(먹다남은음식·검은오물·끈적끈적바늘·맹독구슬·화염구슬·하양허브·탈출팩·미러허브), 특성 24(나이트메어·볼줍기·되새김질·건조피부·위기회피·도망태세·수확·치유의마음·배고픈스위치·촉촉바디·아이스바디·변덕쟁이·픽업·포이즌힐·스웜체인지·젖은접시·탈피·슬로스타트·선파워·가속·스위트베일·달마모드·다운로드·부식), 기술 18(오로라베일·안개제거·사슬묶기·금제·앙코르·내던지기·미래예지·씨뿌리기·빛의장막·매직룸·문어굳히기·멸망의노래·리플렉터·신비의부적·도발·소란·희망사항·하품), 날씨 5, `end_turn_effects.c`, `sleep_clause.c`
- 결과: 420개 PASS 167 / FAIL 206 / KNOWN_FAILING 2 / TO_DO 45. **57파일 모두 이식 전과 결과 목록·실패 사유 줄·합계가 바이트 단위로 같다.** 실패 사유는 이식 전부터 있던 것(`Unmatched MESSAGE` 등)이다.

## 전체 테스트

- 명령: `GITHUB_ACTION=1 make check BUILD=hns -j6 > build/port-check-post132.log 2>&1`(데스크탑, 작업 트리 = `5f580ab12e`). `make` 종료 코드 2는 실패 테스트가 있을 때의 정상 종료다. `Killed`·러너 크래시 0.
- 결과: PASSED **2,344** / FAILED 2,256 / KNOWN_FAILING 10 / ASSUMPTIONS_FAILED 38 / TO_DO 607 / EXPECT_FAILING 6 / TOTAL **5,261**. seq 129(`587f4e7cdc`, PASS 2,340 / FAILED 2,260) 대비 PASS +4, FAILED −4, TOTAL 같음. 사전 분석 A·B의 스크래치 예측(PASS 2,344 / TOTAL 5,261)과 같다.
- 목록 비교: `PORT_INSTRUCTIONS.md`의 `LC_ALL=C`·`grep -a` 추출(5,192줄: PASS 2,341 / FAIL 2,231 / KNOWN_FAILING 10 / TO_DO 605 / EXPECTED_FAIL 5)을 [`test-baseline-seq129.txt`](test-baseline-seq129.txt)(5,192줄)와 비교했다.
  - **사라진 PASS 0.** 새 이름·사라진 이름 0.
  - **FAIL → PASS 4개:** `Cheek Pouch restores 33% max HP`, `Cheek Pouch restores HP after the berry's effect`, `Forced Leppa Berry consumption restores a move at 0 PP before other missing PP`, `Forced Leppa Berry consumption restores the first move found missing PP when none are at 0`. 사전 분석 A·B가 HEAD 사본에서 다시 돌린 결과, 4개 모두 이식 전 실패 사유가 `<Task_FreeAbilityPopUpGfx>: task not freed`(테스트 끝에 팝업 해제 태스크가 남음)였고 기대값 불일치가 아니다. 원인 추정: 턴 종료가 스크립트(`endturnevents`)로 돌면서 컨트롤러 대기·`return` 처리로 프레임이 늘어 테스트가 끝나기 전에 해제 태스크가 끝난다(A·B·C를 따로 빌드할 수 없어 분리 확인은 못 함). 같은 사유의 `Cheek Pouch doesn't activate when user uses Fling`은 여전히 FAIL이다.
  - 그 밖에 상태가 바뀐 테스트 0.
- INVALID(추출 목록 밖): 이름 21개가 seq 129 로그(`build/port-check-post129fix.log`)와 같다. 로그 끝 실패 사유 목록(87줄)도 같다.
- 새 기준 목록: [`test-baseline-seq132.txt`](test-baseline-seq132.txt)(5,192줄).

## 세이브·메모리

- 바뀐 enum은 `enum EndTurnResolutionOrder`(값 2개 추가·1개 이동, 약 52개)이고, 쓰는 곳은 `struct BattleStruct`의 `eventState.endTurn:8`(배틀 중 힙)뿐이다. 8비트 안에 들어간다. 세이브 블록·녹화 배틀·링크 코드와 무관하다. **일반 세이브 영향 없음.**
- 새 전역 변수·구조체 필드 없음. 정적 EWRAM·IWRAM 변화 0 B.
- 배틀 스크립트 opcode 표 불변(`endturnevents`는 `callnative BS_EndTurnEvents`). 뒤 PR(#10311 `END2`→`UNUSED_39`, #10426 `UNUSED_40`)의 번호도 맞는다(사전 분석 B 2.7).

## 친구에게 물을 것

이식 전부터 있던 특이 출력 2개다. upstream `611ed45952`·1.17.0도 같은 코드라 이번 이식에서 고치지 않았다(D 테스트 3-07·2-13이 지금 동작을 고정한다). 고친다면 각각 `battle_end_turn.c` 한 줄 정도의 HnS 수정이며, seq 127처럼 친구 승인 뒤 별도 커밋으로 한다.

1. **전자부유 종료 문장의 이름이 다른 배틀러다.** 내가 쓴 전자부유가 끝났는데 `상대 마자는 전자부유의 효과가 풀렸다!`처럼 다른 포켓몬 이름이 나올 수 있다. `HandleEndTurnMagnetRise`가 `gBattleScripting.battler`를 정하지 않고, `BattleScript_BufferEndTurn`이 `{B_SCR_NAME_WITH_PREFIX}`를 쓴다.
2. **불바다 종료 문장의 진영이 반대다.** 상대 쪽 불바다가 끝났는데 `우리 편 주변의 불바다가 사라졌다!`가 나온다. 합체기 필드 종료 가운데 `SECOND_EVENT_BLOCK_SEA_OF_FIRE`만 `gBattlerAttacker`를 정하지 않는다(무지개·습지는 정함).

## 실기 확인 항목 (친구용)

이식 전 ROM(`587f4e7cdc` 코드, SHA1 `f453524c…`)과 이식 후 ROM(`5f580ab12e`, SHA1 `512ccdbe…`, 데스크탑 툴체인)을 같은 `.sav`로 비교한다. 문장·팝업·애니메이션 순서와 **아이템 팝업이 화면에서 사라지는 시점**, 턴 종료 사이의 대기 시간을 본다.

1. **맹독구슬·화염구슬:** 지닌 포켓몬의 첫 턴 끝 → 아이템 팝업 → 상태 애니 → `…은(는) 맹독구슬 때문에 맹독에 중독됐다!`/`…은(는) 화염구슬 때문에 화상을 입었다!` → 상태 아이콘. 팝업이 남지 않고 사라지는지.
2. **아이스바디(싸라기눈·설경)·포이즌힐·선파워·젖은접시·건조피부:** 특성 팝업과 회복·피해 애니·HP 바만, 문구 없음.
3. **하양허브:** (a) 턴 종료 경로 — 변덕쟁이·문어굳히기로 능력이 떨어진 턴 끝 → 아이템 팝업 → `…은(는) 하양허브로\n상태를 원래대로 되돌렸다!`. (b) 트릭·스위처로 하양허브를 주고받기 → 이제 상대가 받은 도구(자뭉열매 등)도 같은 행동 안에서 발동하는지. (c) 매직룸이 끝나는 턴에 하양허브와 다른 배틀러의 열매가 같은 턴 끝에 이어서 발동하는지.
4. **소란 턴 끝:** 소란피기 중 `…은(는) 소란피우고 있다!`, 잠든 포켓몬을 깨운 턴의 `…소란스러워서 눈을 떴다!`, 종료 `…은(는)\n얌전해졌다`.
5. **방벽·날씨·필드 종료 문구:** `우리 편의 리플렉터가 없어졌다!`·`상대의 빛의장막이 없어졌다!`·오로라베일·신비의부적·흰안개·순풍, 날씨 그침 문구, 필드 종료 문구와 배경 복귀.
6. **미래예지:** 2턴 뒤 `…은(는) 미래예지 공격을 받았다!` → 피해. (가능하면) 빗나가는 경우(대상의 회피 상승 등) 턴 종료가 이어서 진행되는지(남은 위험).
7. **배틀 아레나·배틀 팰리스:** 아레나 3턴 판정(심판 문구·결과), 아레나 턴 시작 문구, 팰리스 턴 시작 맛 문구가 이전과 같이 나오는지.
8. (가능하면) **독·화상으로 마지막 포켓몬 기절:** 턴 끝 피해 → 기절 → 경험치 → 교체 화면/승리 문구 순서.
9. (가능하면) **야생 배틀 위기회피:** 턴 끝 피해로 HP가 절반 아래가 된 야생 위기회피 포켓몬이 도망가고 배틀이 끝나는지.

## 후속 행 메모

- **seq 150 #9717(EndTurn queued switches):** `ENDTURN_EMERGENCY_EXIT_1~4` → `SEND_OUT_REPLACEMENTS_*`, `DoEndTurnEffects()` 첫머리 비상탈출 루프가 이번 승패 점프 줄과 문맥이 겹친다. 1.17.0 `TrySwitchInEjectPack`은 `BattleScript_EjectPackActivates_SendReplacement`를 쓴다(이번 `EjectPackActivate_NoQueuedSwitch` 자리). HnS 탈출버튼·탈출팩 아이템 팝업 유지.
- **seq 166 #9784(Eject Items / Mirror Herb / White Herb):** 하양허브 hunk를 HnS `RestoreWhiteHerbStats(battler, timing)`·`WhiteHerbFling` 분기에 맞춘다.
- **seq 181 #9730(Stat Change Refactor):** `EndTurnEvents()`에 `gQueuedStatBoosts` memset 1줄이 들어온다(이번 함수는 1.17.0과 이 줄만 다름).
- **seq 303 #10433(Opportunist / Mirror Herb end turn):** 턴 종료 단계 enum과 처리기 표에 단계가 끼어든다.
- **seq 361 #9211(Additional trainer slides):** 슬라이드 단계를 맨 뒤(다이맥스 뒤)로 옮긴다. 승패 결정 뒤 점프 대상(`ENDTURN_TRAINER_A_SLIDES`) 때문에 아레나·기절·다이맥스 처리가 건너뛰어지는지 확인한다.
- **seq 440 #10471(Remove infiniteConfusion flag):** upstream `92845d3304`의 `battle_end_turn.c` hunk는 난동 혼란(`FIRST_EVENT_BLOCK_THRASH`)의 `confusionTurns` → `confusionTimer` 2줄이고, 문맥이 이번에 들어온 `BattleScriptCall(BattleScript_ThrashConfuses)`다(HnS는 `B_RAMPAGE_CONFUSION`이 `GEN_LATEST`라 이 경로에 도달하지 않음). `battle_hold_effects.c`·`battle_util.c` hunk도 이번 커밋 뒤 문맥에 맞춘다.
- **seq 470 #9939(accuracy check into canceler):** `MonTookFutureAttack`의 `accuracycheck`를 없애 미래예지 빗나감 `end` 경로가 사라진다(남은 위험 해소 확인).
- **seq 475 #9777(Champions battle messages):** 1.17.0 `TryToxicOrb`는 `BattleScript_ToxicOrbActivates`(아이템 팝업 + `MoveEffectToxic`)를 부른다. HnS `BattleScript_ToxicOrb`·`FlameOrb`(`STRINGID_PKMNPOISONEDBY`/`BURNEDBY`)와 꼬리 `UpdateEffectStatusIconOrbRet`의 관계를 이때 다시 정한다. (사전 분석 C가 함께 적은 #9782 내던지기 + 하양허브·멘탈허브 수정은 1.15.2에 들어간 history 안 PR이라(`all_prs_master.tsv` `in-history`) 따로 이식할 행이 없다.)
- **seq 477 #10311(Remove end2):** `end2` → `end`. 이번에 남은 HnS `end2`(첫 턴 `ArenaTurnBeginning`·`PalacePrintFlavorText` 래퍼, `Trainer*SlideMsgEnd2`, `BattleScript_EndTurnEvents`, `ArenaDoJudgment` 등)와 HnS 전용 라벨 이름의 `End2`→`End`를 함께 바꾼다. 이번에 `return`형으로 바꾼 HnS 스크립트(`AbilityMadeIneffectiveRet`, `*WoreOff`, `UpdateEffectStatusIconOrbRet`)는 대상이 아니다.
- **seq 481 #10326(Battle Terrain Refactor):** 필드 종료(`TerrainEnds`)·그래스필드 회복 처리기가 이번에 Call/`return`형이 됐다. 지형 표 이식 때 이 형태를 유지한다.
- **seq 484 #10426(absorb / SetMoveEffect):** `UNUSED_40` opcode 번호가 맞는다. 씨뿌리기 턴 종료(`LeechSeedTurnDrain*`, `return`형) 문맥.
- 방벽 `*WoreOff`/`*WoreOffReturn` 중복(본문 같음)과 `SideStatusWoreOff`(HnS 미사용)는 남겨 두었다. 합친다면 `TryDefogClear`의 `DEFOG_CLEAR` 4줄도 같이 바꾼다(`SafeguardEndsReturn`은 `pause` 차이로 유지).
