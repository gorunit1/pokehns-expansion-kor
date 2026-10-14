# seq 348 #10711 — `AreMultiPartiesFullTeams` 강제 게임 분기 측정 (묶음 21 사전 분석 b, 2026-10-14)

보통 `make check`는 `AreMultiPartiesFullTeams`의 `#if TESTING` 분기만 타서 게임 판정을 잴 수 없다(재확인 8i). 그래서 세션 임시 사본에서만 소스를 바꿔 측정했다. 실제 저장소 코드와 테스트 목록에는 넣지 않았다. 이 폴더는 그 재료와 결과를 그대로 보존한 것이다.

## 측정 조건

- 사본 기준 코드: `da0adeaa`. `seq348_proposed.patch`는 커밋 `132797a2`와 같다.
- `src/battle_util.c`를 네 판으로 바꿔 끼워 돌렸다. 판마다 `#if TESTING` → `#if 0`(게임 분기 강제)이고, `AreOpponentsFacilityTrainers`의 `#if !TESTING` → `#if 1`이다. 각 판이 HEAD와 어떻게 다른지는 `battle_util_<판>_forced.diff`에 있다.
  - `HEAD`: 348 전.
  - `UP10711`: upstream #10711 그대로.
  - `UP10773`: 1.17.1·master의 #10773 형태.
  - `HNS`: 348 HnS 판정 = 커밋 `132797a2`.
- `test/battle/trainer_control.party`에 `Multi Party: Half` 시험 트레이너(id 15)를 더했다(`trainer_control.party.with15`).
- 테스트는 `zz_b22_multi.c`(일반 `TEST` 13개)다.
  - 테스트마다 다음을 세우고 `AreMultiPartiesFullTeams()`의 반환값과 `gSpecialVar_Result`를 본다: `gBattleTypeFlags`, 상대 A·B id, `gBattleStruct`(NULL = 필드, 더미 = 배틀 중), 이전 `gSpecialVar_Result`.
  - 난이도 변수는 `DIFFICULTY_NORMAL`로 세웠다. 테스트 빌드 기본값은 EASY라 빈 표를 읽기 때문이고, HnS 게임은 늘 NORMAL이다.
- 명령은 `run_measure.sh` 첫 루프다(사본 `b22/b/m`에서 판마다 `GITHUB_ACTION=1 make check BUILD=hns -j3 TESTS='B22'`).
- 결과 줄은 `result_<판>.txt`에 있다(원 로그에서 색 코드를 빼고 결과·합계 줄만 뽑음).

## 결과

| 판 | 통과 |
|---|---|
| HEAD | 13/13 |
| UP10711 | 7/13 |
| UP10773 | 10/13 |
| HNS | 13/13 |

- upstream 두 형태가 모두 놓친 01·02·04번이 재확인 8d 경우다. 지난 야생 플래그가 남은 채로 트레이너 반쪽 멀티를 풀 팀으로 본다.
- 06·07·09번은 #10711이 조기 반환에서 결과를 세우지 않은 경우다. #10773이 이 부분을 고쳤다.
- 표 전체는 [`../../full-sync-seq-337-350.md`](../../full-sync-seq-337-350.md)의 "348 HnS 판정" 절과 세션 임시 보고서 `seq340-348-b.md` 5절에 있다.
- 따라오기 NPC 야생전(`IsNPCFollowerWildBattle()`)은 HnS에서 켤 수 없어 측정하지 않았다(코드 해석만).
- 이 측정은 함수 판정만 본다. 필드 흐름(`multi_do` → 배틀 → `SaveSelectedParty`의 복제·소실)은 코드 해석이고, mGBA 실기는 친구가 한다.
