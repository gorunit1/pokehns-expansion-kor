# full-sync 실제 port 결과 — seq 139 (#9714 Slight restructure for Defog and Tidy Up)

완료: seq 139 unit `U-9714`를 이식했다. 다음 seq는 **142 #8930**(Automate regional Pokedex orders, L)이다. seq 140·141은 #8943 단위에서 이미 적용했다.

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), 메시지 출력 기록: [`BATTLE_MESSAGE_OUTPUT_CHANGES.md`](../../../localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md)
시작 HEAD: `7a81c1e9bf`(작업 트리 clean, 코드는 친구 요청 A·B·C 뒤 `f79f3baf2b`). 작업 컴퓨터: 데스크탑.

## 요약

| seq | PR | 판정 | 커밋 | ROM 변화 | 비고 |
|---|---|---|---|---:|---|
| 139 | #9714 | 적용(HnS 적응) | `32b62b550f` | +176 B | 안개제거·정리정돈·턴 끝 방벽 만료에서 `gBattlerAttacker`를 진영 번호로 덮어쓰던 방식을 `gBattleScripting.battler` + 스크립트 저장/복원으로 바꿨다. HnS 고유 스크립트 9개와 `RemoveHazards`에 같은 저장/복원을 넣어 한글 진영 문구를 유지했다 |

- 빌드: 종료 코드 0, **ROM 32,715,348 B(+176 B) / EWRAM 250,128 B(0) / IWRAM 25,516 B(0)**, SHA1 `900161aa4719a126211e11751d6d4d6d6884e7f6`. 새 경고 0.
- 한글이 든 소스 줄 변경: `STRINGID_PKMNSUBSTITUTEFADED` 1줄의 이름 토큰만(`{B_DEF_NAME_WITH_PREFIX}` → `{B_SCR_NAME_WITH_PREFIX}`, 화면 이름 같음).
- **정상 경로의 화면 출력 변화 0**(한글 회귀 테스트 34개로 실측). upstream 구조 변경이 기존 결함 2건을 고친다(아래 "출력·동작 변화").
- 세이브: 영향 없음(구조체·SaveBlock·EWRAM 변화 0).
- 전체 테스트: PASSED 2,350 / TOTAL 5,271, INVALID 21, Killed 0. 기준 목록과 바이트 단위로 같다(사라진 PASS 0, 상태 변화 0). 새 기준 목록 [`test-baseline-seq139.txt`](test-baseline-seq139.txt)(내용은 `test-baseline-seq138.5.txt`와 같음).
- 세이브 정적 비교 `save_compat.py run`: 기대 보고서와 판정 줄 68개 같음(PASS, WARN 3 = #8943 의도된 차이).

## 공통 사항

- 빌드 명령: `GITHUB_ACTION=1 make hns -j8 > build/port.log 2>&1`. 이식 전 기준 ROM 32,715,172 B, SHA1 `b826980d…`.
- 사전 분석(읽기 전용 2개, 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-139/`)
  - A(`part-A.md`/`part-A.patch`, md5 `2fcfa3c1…`): upstream 25 hunk 대조와 HnS 적응
  - B(`part-B.md`/`B-tests/`): 이식 전 한글 출력·순서를 고정한 회귀 테스트 34개(배틀 약 117회)와 실행 스크립트
- 적용: 적용 담당 1개가 `part-A.patch`를 `git apply --check` → `git apply`로 그대로 넣었다(손으로 옮긴 곳 0). 지시 `chunk-139/APPLY.md`, 기록 `chunk-139/apply/PROGRESS.md`.

## 동기화 단위: seq 139 #9714 `U-9714` Slight restructure for Defog and Tidy Up

- 현재 판정: 적용(HnS 적응)
- 커밋: `32b62b550f`
- upstream 근거: `1606fa912d`(5파일 +33/−62). HnS 커밋은 5파일 +57/−58(HnS 고유 스크립트 적응 포함).
- 수정 파일: `data/battle_scripts_1.s`, `include/battle_scripts.h`, `src/battle_end_turn.c`, `src/battle_message.c`, `src/battle_script_commands.c`
- 내용(upstream): `TryDefogClear`·`TryTidyUpClear`와 턴 끝 방벽 만료가 `gBattlerAttacker`에 진영 번호를 넣던 것을 `gBattleScripting.battler`(`sBATTLER`)로 넘기고, 스크립트가 문구를 찍기 직전에만 `saveattacker` / `copybyte gBattlerAttacker, sBATTLER` / `restoreattacker`로 진영 주체를 맞춘다. 대타 소멸 문구가 `gBattleScripting.battler`를 쓴다.
- **HnS 적응**
  - upstream 25 hunk 중 16개 그대로, 7개 손으로 맞춤(HnS 고유 스크립트 이름·한글 줄·HnS에 없는 끈적끈적네트 플레이어 분기), 2개 제외(줄 끝 공백 정리, HnS에는 이미 공백 없음).
  - HnS 고유 스크립트 9개(턴 종료 방벽별 `*WoreOff` 4개, 안개제거 `*WoreOffReturn` 4개, `SafeguardEndsReturn`)와 `RemoveHazards`에 upstream과 같은 저장/복사/복원을 넣었다. 한글 `{B_ATK_TEAM*}`·`{B_ATK_PREFIX*}` 문구가 `printstring` 시점에 이전과 같은 진영을 가리킨다.
  - 방벽 `*WoreOff`/`*WoreOffReturn` 4쌍은 본문이 같지만 seq 132 결정대로 합치지 않았다.
  - 정의 없는 `BattleScript_DefogClearHazards` 선언은 upstream 1.17.0처럼 남겼다.
- 제외한 hunk: 줄 끝 공백 정리 2개.

## 한글 토큰 증명 (`STRINGID_PKMNSUBSTITUTEFADED`)

- `{B_DEF_NAME_WITH_PREFIX}의\n대타는 사라져 버렸다...\p` → `{B_SCR_NAME_WITH_PREFIX}의\n대타는 사라져 버렸다...\p`. 두 토큰 모두 2바이트(`FD 10` → `FD 13`)이고 같은 전개 매크로로 같은 접두어·별명을 낸다. 본문 `의`는 받침과 무관하다.
- 호출 지점 2곳이 모두 `gBattleScripting.battler`에 이전 `gBattlerTarget`과 같은 배틀러를 넣는다: 공격으로 대타가 사라질 때(`MoveDamageDataHpUpdate`, 스크립트 2곳 모두 `BS_TARGET`), 정리정돈(`TryTidyUpClear`, 이전 `gBattlerTarget = i` → 이후 `sBATTLER = i`). 대타 소멸 애니메이션도 같은 배틀러.

## 출력·동작 변화

| 변화 | 이전 | 이후 | HnS 도달 |
|---|---|---|---|
| 정상 경로 화면 문구·순서 | — | 같음(한글 회귀 31개가 메시지·팝업·애니메이션 순서까지 같음) | — |
| **안개제거 확인 단계가 안개·필드에서 돌아올 때**(upstream 수정) | 대상이 대타출동 뒤에 있거나 회피율 −6일 때만 도는 확인 단계에서, 지울 것이 안개나 필드뿐이면 `gBattlerAttacker`가 진영 0(플레이어 왼쪽)으로 남았다. 상대가 쓴 안개제거라면: 애니메이션이 플레이어에게서 나감 → 대타를 뚫고 `마자용의 회피율이 떨어졌다!` → 필드 문구 → **상대 자기 편** `상대의 리플렉터가 없어졌다!`. 더블에서 플레이어 오른쪽이 쓰면 애니메이션·마지막 기술 기록이 플레이어 왼쪽으로 감 | 사용자가 끝까지 실제 사용자. 회피율 문구 없음(대타가 막음), 상대 리플렉터 유지, 애니메이션·마지막 기술 기록 정상 | **도달 가능(드묾).** 상대가 대타출동 뒤의 플레이어에게 안개제거를 쓸 때(안개·필드가 있고 플레이어 쪽 장판·방벽이 없을 때), 더블 플레이어 오른쪽 |
| 거다이풍격(upstream 수정) | 자기 편 방벽·흰안개·신비의부적을 지웠다 | 상대 편을 지운다(1.17.0과 같음) | 도달 안 함(HnS 다이맥스 없음) — 이 결과 문서에만 기록 |
| 컨트롤러 배틀러(내부) | 안개·필드 문구, 정리정돈 끝·대타 문구가 진영 번호 배틀러의 컨트롤러로 출력될 수 있었다 | 실제 사용자 컨트롤러 | 화면 같음 |

- `BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 안개제거 확인 단계 행을 넣었다(새 한글 문장 없음).

## 한글 출력 회귀 테스트 (저장소 밖 `chunk-139/B-tests/`)

- 대상: 안개제거(사용자·상대 쪽, 방벽 5종 각각·복수, 장판 4종+강철, 필드, 순서, 대타, 부스터에너지·매직미러), 정리정돈(장판·대타·더블), 고속스핀·킬러스핀, 공격 대타 소멸(스프레드·연속기), 턴 끝 방벽·흰안개·신비의부적·순풍·주술·불바다·무지개·습지·필드 만료, 더블 오른쪽 사용자, 거다이풍격. 친구 요청 A·B 문장은 기대값에서 뺐다.
- 저장소에서 `ALLOW_REPO=1 B-tests/run.sh`로 4파일을 복사·실행·삭제했다.
  - 이식 전: **34/34 PASS**(`baseline/base-summary.txt`와 같음)
  - 이식 후: **31 PASS / 3 FAIL**(`baseline/expected-after-A-summary.txt`와 같음). FAIL 3개는 처음부터 이름에 `UPSTREAM CHANGE EXPECTED`를 붙인 1-11·4-02(안개제거 확인 단계 수정)·4-03(거다이풍격)이다.
- seq 132 턴 종료 한글 회귀 70개: 66/4로 이전과 같다.
- 사전 분석의 이식 후 확인 테스트 4개(이식 후 출력을 기대값으로 씀): 이식 전 0/4 → 이식 후 4/4(사본).

## 지정 테스트

- 관련 기존 테스트 19파일 102개(`defog.c`·`tidy_up.c`·방벽·흰안개·신비의부적·주술·순풍·`substitute.c`·`rapid_spin.c`·장판 4종·`end_turn_effects.c`·`move_effect_secondary/{reflect,light_screen,stealth_rock}.c`): 이식 전후 상태 목록이 같다(PASS 20). 실패 72개는 모두 `Unmatched MESSAGE`이고 실패 사유 줄도 같다. 기준 목록 대비 PASS → 비PASS 0.

## 전체 테스트

- 명령: `GITHUB_ACTION=1 make check BUILD=hns -j6 > build/port-check-seq139.log 2>&1`, 목록은 PORT_INSTRUCTIONS의 `LC_ALL=C` 추출.
- 결과: PASSED 2,350 / FAILED 2,260 / KNOWN_FAILING 10 / ASSUMPTIONS_FAILED 38 / TO_DO 607 / EXPECT_FAILING 6 / TOTAL 5,271, INVALID 21. `test-baseline-seq138.5.txt`와 목록이 같다.
- 재빌드: `build/localization-logs/hns-20261004-211044-seq139.log`, SHA1 `900161aa…`(적용 담당 빌드와 같음).

## 커밋 리뷰 (병렬 2개, 읽기 전용, 결과 `chunk-139/review-{C,S}/REVIEW-RESULT.md`)

| 리뷰 | 영역 | 판정 | 요지 |
|---|---|---|---|
| C | C 코드(`battle_script_commands.c`, `battle_end_turn.c`, 헤더) | 문제 없음 | upstream 직후와 비교한 차이는 HnS 방벽별 스크립트 이름 5줄뿐. 새로 `gBattleScripting.battler`를 덮는 6곳 뒤 읽는 곳을 모두 확인(턴 끝 15개·이동 끝은 직전 대입). 안개제거 확인 단계 수정·거다이풍격 판단 맞음, 그 밖의 동작 변화 0. 세이브·EWRAM 0. 탐침 4개: 부스트에너지가 정리 루프 중간에 터지는 경우·정리정돈 대타 소멸은 전후 PASS, 확인 단계+필드+부스트에너지는 이식 전 FAIL → 이식 후 PASS |
| S | 스크립트·한글(`battle_scripts_1.s`, `battle_message.c`) | 문제 없음 | 저장/복원 대상 스크립트 11개 모두 분기 없는 직선(저장 → 복사 → 출력 → 복원), 진입은 C 호출뿐이고 호출 직전 대입. 저장 스택 최대 깊이 2(매직미러 반사), 스크립트 호출 스택 증가 0. 모든 출력 시점의 진영 값이 이식 전과 같음. 회귀 34개를 HEAD^/HEAD에서 다시 돌려 trace가 기준과 같고, 덮지 못한 경로 4개(미래예지·파멸의소원 대타 파괴, 야생 대타 소멸·안개제거·방벽, 더블 매직미러 반사, 진영 메시지 배틀러 0 기절)를 추가 실측해 trace 390줄 전후 동일. 대타 토큰 증명 맞음 |

- 참고(조치 없음): `DefogClearHazards`의 빈 줄과 `DefogDoAnim`의 `BS_SCRIPTING` 사용, `TryTidyUpClear` 첫 인자 미사용은 upstream 직후와 같고 seq 181 #9730에서 1.17.0 형태로 맞춰진다. `RemoveHazardsRet` 라벨은 부르는 곳이 없다(upstream 같음).

## 발견한 별건 (이번 PR과 무관, 고치지 않음)

- 주술 시작 문장 `주술의 힘으로\n우리 편은의 급소가 숨겨졌다!`(`상대는의`)와 팀 가드 문장 `우리 편은을\n…가 지켜 줬다!` — 친구 요청 A와 같은 유형. `RECHECK_BEFORE_COMPLETION.md` 10b, 친구 확인 대기.
- `Time to Gigantamax!` 영문(HnS 미도달). 재확인 목록 10c.

## 실기 확인 항목 (친구용)

1. 안개제거: 내 쪽·상대 쪽 방벽(리플렉터·빛의장막·오로라베일·흰안개·신비의부적) 각각과 여러 개, 장판 4종, 필드가 지워질 때 문구의 진영(`우리 편의`/`상대의` 등)이 이전과 같은지
2. 정리정돈: 장판·대타 문구
3. 턴 끝 리플렉터·빛의장막·오로라베일·흰안개·신비의부적·순풍·주술 만료 문구(아군·상대)
4. (드묾) 상대가 대타출동 뒤의 내 포켓몬에게 안개제거를 썼고 필드가 있을 때: 회피율이 떨어지지 않고, 상대 자기 편 리플렉터가 남는지

## 후속 행 메모

- 이 단위 뒤 재확인: 사전 분석 문서(part-A 1절)의 "B 33개 중 30개"는 최종 세트 34/31이다.
- 다음 seq 142 #8930(Automate regional Pokedex orders, L). seq 143 #9721은 선진행으로 이미 적용.
- 방벽 `*WoreOff`/`*WoreOffReturn` 4쌍 통합은 계속 보류(합칠 때 `DEFOG_CLEAR` 4줄과 헤더 4줄을 함께 바꾼다).
