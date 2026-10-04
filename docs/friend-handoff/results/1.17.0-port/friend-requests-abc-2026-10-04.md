# 친구 요청 A·B·C 수정 결과 (2026-10-04, 데스크탑)

친구 mGBA 보고([`mgba-check-seq127-138.md`](mgba-check-seq127-138.md) 2·3절)의 새 요청 3건을 HnS 커밋으로 넣었다. Pokémon Champions 최신 한국어 출력에 맞추는 수정이다. 새 한글 문장은 없다.

- 시작 HEAD `12d6d98dc4`(코드 `f3a58f9939`, seq 138.5 #8943 단위 뒤). 작업 컴퓨터: 데스크탑.
- 진행: 적용 담당 1개(지시 `/home/hjm0725/hns-sync-work/chunk-abc/APPLY.md`, 기록 `PROGRESS.md`) → 커밋 리뷰 1개 → 메인 재빌드·전체 테스트.
- C(팝업)의 분석과 남은 후보는 [`popup-champions-compare.md`](popup-champions-compare.md).

## 요약

| 요청 | 커밋 | 바뀐 것 | ROM | 확인 |
|---|---|---|---:|---|
| A 순풍 시작 문장 | `3464fa89b6` HnS: name the side, not the "-은" prefix, in the Tailwind start message | `STRINGID_TAILWINDBLEW` `{B_ATK_PREFIX2}에게\n순풍이 불기 시작했다!` → `{B_ATK_TEAM1}에게\n순풍이 불기 시작했다!` | 0 B | 임시 한글 테스트 4개(플레이어·상대 순풍, 바람타기·풍력발전 연계) 수정 전 FAIL → 수정 뒤 PASS |
| B 그래스필드 회복 문장 | `ef8c779b55` HnS: use the shared HP restored message for Grassy Terrain healing | `STRINGID_GRASSYTERRAINHEALS` `{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n그래스필드의 힘으로 회복했다!` → `{B_ATK_NAME_WITH_PREFIX}의\n체력이 회복되었다!` | −16 B | 임시 한글 테스트 2개(플레이어·상대 회복) FAIL → PASS |
| C 멘탈허브 팝업 | `f79f3baf2b` HnS: show the item pop-up when Mental Herb activates | `BattleScript_MentalHerbCureRet`·`BattleScript_MentalHerbCureFling`의 애니메이션 앞에 `call BattleScript_ItemPopUp_ScriptingNoFlush`(`@ HnS:`) | +16 B | 팝업 → 애니메이션 → 해제 문장 순서 trace 실측 12시나리오(분석 단계, 같은 줄인지 diff로 재확인), 한글 문장·애니메이션 바이트 불변 |

- 최종 빌드(`f79f3baf2b`): 종료 코드 0, **ROM 32,715,172 B / EWRAM 250,128 B / IWRAM 25,516 B**(시작과 같은 크기), SHA1 `b826980d2c2dda90f76c1bfe99d3482ee66c1557`. 메인이 저장소에서 다시 빌드해 같은 SHA1을 확인했다(`build/localization-logs/hns-20261004-204651-abc.log`). 새 경고 0.
- 한글 소스 줄 변경: 위 2줄(A·B)뿐. C는 스크립트 주석·호출만.
- 전체 테스트(`build/port-check-abc.log`): PASSED 2,350 / TOTAL 5,271, INVALID 21. 목록이 `test-baseline-seq138.5.txt`와 바이트 단위로 같다(영문 `MESSAGE` 기대값 테스트는 원래 HnS에서 실패하는 것이라 상태 변화가 없다).
- 관련 기존 테스트 전후 같음: `tailwind.c` 0/3, `wind_rider.c` 1/7, `wind_power.c` 0/8, `end_turn_effects.c` 2/6, `mental_herb.c` 0/7, `fling.c` 8/24(실패 사유도 같음).
- 한글 턴 종료 회귀 테스트(seq 132 D, 저장소 밖): 2-10의 그래스필드 기대 문장을 새 문장으로 고친 뒤(`마자용의 체력이 회복되었다!`, `상대 마자의 체력이 회복되었다!`, 원본 `zz_hns9680_endturn2.c.bak-before-B`) **66 PASS / 4 FAIL**, 요약이 seq 132 기대 요약과 같다.

## A 순풍 시작 문장

- 원인: `B_ATK_PREFIX2`는 `우리 편은`/`상대는`을 내서 `{B_ATK_PREFIX2}에게`가 `우리 편은에게`가 됐다.
- 수정: 종료 문장 `STRINGID_TAILWINDENDS`(`{B_ATK_TEAM1}의\n순풍이 멈췄다!`)와 같은 `B_ATK_TEAM1`(`우리 편`/`상대`). 2바이트 토큰 하나만 바뀐다(`FD 2A` → `FD 3E`, 새 ROM에서 확인). 문장 길이 28 B 그대로.
- 출력 경로 두 곳 모두 `gBattlerAttacker`가 순풍을 건 쪽이다: 기술 사용(사용자), 필드 시작 상태 `SetStartingSideStatus`(그 진영을 attacker로 둠). 바람타기·풍력발전은 문장 뒤에 돌아 영향 없다.
- 화면: `우리 편에게\n순풍이 불기 시작했다!` / `상대에게\n순풍이 불기 시작했다!`

## B 그래스필드 회복 문장

- 수정: 공용 HP 회복 문장 `STRINGID_PKMNREGAINEDHEALTH`(`{B_DEF_NAME_WITH_PREFIX}의\n체력이 회복되었다!`)와 같은 본문, 토큰만 ATK. 34 B → 23 B. 새 문장 아님.
- 이 ID는 턴 종료 그래스필드 회복에서만 나오고, 그때 `gBattlerAttacker`가 회복 배틀러다. `의`는 받침과 무관해 조사 처리 없음.
- 화면: `마자용의\n체력이 회복되었다!` / `상대 마자의\n체력이 회복되었다!`
- upstream 1.17.0도 같은 ID(영문 "is healed by the grassy terrain!")를 쓰므로 나중 이식에서 이 한글 줄만 유지하면 된다.

## C 멘탈허브 팝업

- 친구 실기(Champions에서는 멘탈허브 발동 때 도구 팝업이 뜬다)에 맞춰, 하양허브와 같은 형태(`NoFlush`, `BS_SCRIPTING`, 애니메이션 앞)로 넣었다. upstream 1.17.0·upcoming에도 멘탈허브 팝업은 없어 HnS 고유 줄이다.
- **내던지기 경로는 친구 확인(Q1) 대기**다. Champions에서 팝업이 없다고 하면 `BattleScript_MentalHerbCureFling`의 두 줄만 뺀다.
- 앞 문장 창을 비우지 않는 `NoFlush`도 하양허브와 같은 선택이다(친구 Q2). 창을 비우는 것이 맞다면 `BattleScript_ItemPopUp_Scripting`으로 한 단어만 바꾼다.
- 나중 이식: #10268(seq 394)·#9777(seq 475)·#9168(seq 161) hunk의 적용 가능 여부를 바꾸지 않는다(분석 단계 실측). upcoming #10759(1.17.x 범위 밖)를 들이면 새 서브루틴 이름으로 바꾼다.

## 커밋 리뷰 (읽기 전용 1개, 결과 `/home/hjm0725/hns-sync-work/chunk-abc/review/REVIEW-RESULT.md`)

- **세 커밋 모두 문제 없음.**
  - A: 순풍 시작 문장 경로 3개(기술, 가로채기, 필드 시작 상태)를 싱글·더블 오른쪽·멀티(파트너·상대 B)·야생에서 실측해 모두 진영이 맞다. 통신 대전도 기기마다 자기 편이 `우리 편`.
  - B: 이 ID를 쓰는 곳은 `BattleScript_GrassyTerrainHeals` 1곳. 접두어 `상대 `·`야생 `·아군(없음) 실측.
  - C: 멘탈허브 발동 경로 3개 모두 보유자(내던지기는 맞은 쪽)에 `ITEM_MENTAL_HERB` 팝업. 분석 단계 12시나리오를 HEAD에서 다시 실측해 trace가 바이트까지 같고 순서 검사 PASS. 더블 오른쪽 보유자, 같은 배틀러 특성 팝업 직후, 내던지기도 정상. #10268·#9777·#9168 hunk 60개 적용 가능 여부 전후 같음.
  - 관련 테스트 533개가 기준 목록과 같고, 최종 ROM에 새 문장 각 1회·옛 문장 0회.
- 참고(고치지 않음)
  - **같은 유형의 조사 문제 2곳**(`{B_ATK_PREFIX2}` 뒤 조사, 이번 범위 밖, 친구 확인 대상): `STRINGID_SHIELDEDFROMCRITICALHITS` `주술의 힘으로\n우리 편은의 급소가 숨겨졌다!`/`상대는의`, `STRINGID_PROTECTEDTEAM` `우리 편은을\n와이드가드가 지켜 줬다!`/`상대는을`(패스트가드·마룻바닥세워막기·트릭가드도 같은 문장). 토큰만 바꾸는 수정 제안: 주술 `{B_ATK_TEAM1}의`, 팀 가드 `{B_ATK_PREFIX3}`(`우리 편을`/`상대를`)로 바꾸고 `{B_TXT_EULREUL}` 삭제. 재확인 목록 13b.
  - 멘탈허브가 배틀 마지막 턴에 발동하면 헤드리스 테스트가 `Task_FreeAbilityPopUpGfx: task not freed`로 FAIL할 수 있다(#10321 seq 413 전, 하양허브도 같음). 게임에서는 다음 문장 동안 닫힌다. 한글 멘탈허브 테스트를 쓰면 끝에 빈 `TURN {}`.
  - upcoming #10759를 이식하면 이 두 줄의 서브루틴 이름을 바꿔야 한다(어셈블 오류로 드러남).
  - 사이코노이즈 회복봉인을 멘탈허브로 풀어도 해제 문장이 한 번 더 나오는 기존 결함(#10307 seq 254).

## 실기 확인 항목 (친구용)

1. 순풍: 내 포켓몬·상대가 순풍을 쓸 때 `우리 편에게`/`상대에게 … 순풍이 불기 시작했다!`. 필드 시작 상태로 순풍이 걸리는 배틀이 있으면 그것도
2. 그래스필드: 싱글·더블에서 턴 끝 회복 문장 `…의 체력이 회복되었다!`(상대는 `상대 …의`)
3. 멘탈허브: 도발·헤롱헤롱·앙코르 등이 풀릴 때 팝업 → 애니메이션 → 문장. 헤롱헤롱바디·저주받은바디로 바로 풀릴 때 특성 팝업과 겹쳐 보이지 않는지. 내던지기로 맞은 경우(Q1)
