# full-sync 실제 port 결과 — 묶음 4: seq 166, 168, 169, 170

완료: 순서표 seq 164~170 가운데 남은 4행을 PR별 커밋으로 이식했다(164는 #8943 단위에서 선반영, 165는 선진행, 167 #9819는 #10548 직전까지 보류). 이로써 2026-10-05 사용자 지시 범위(seq 142~170)가 167을 빼고 끝났다.

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md)
시작 HEAD: `aeae8d92dc`. 작업 컴퓨터: 데스크탑(2026-10-05).

## 요약

| seq | PR | 판정 | 커밋 | ROM 변화 | 비고 |
|---|---|---|---|---:|---|
| 166 | #9784 Adjustments for Eject Items / Mirror Herb / White Herb | 적용(HnS 적응) | `487898f831` | −640 B (EWRAM +4 B) | 레드카드·탈출버튼·하양허브·탈출팩·흉내허브를 **실제 스피드 순**으로 처리. 한 기술에 레드카드와 탈출버튼이 둘 다 발동 가능. HnS `BS_EffectsAfterFormChange` 배틀러 순서 루프 유지(주석만) |
| 168 | #9832 Clean up for eject item changes | 일부 적용 | `c625740524` | +16 B | `MoveEndCardButton` 정리. Acrobatics hunk는 #9710 뒤 이미 같아 제외. 동작 같음 |
| 169 | #9835 Fix declaration after label | 적용 | `b391fc833c` | 0 | `case` 라벨 뒤 선언 2곳을 중괄호로. 동작 같음 |
| 170 | #9751 Adds AreMultiPartiesFullTeams and Test_BattlersShareParty handling to tests | 적용(HnS 적응) | `e12e86b186` | +16 B(정렬 여백) | 테스트 러너 전용 변경. HnS `AreOpponentsFacilityTrainers()`를 `#if !TESTING`으로 감쌈. 게임 코드 같음 |

- 빌드(최종 `e12e86b186`): 종료 코드 0, **ROM 32,716,452 B(−608 B) / EWRAM 250,132 B(+4 B, `gBattlersByRawSpeed`) / IWRAM 25,516 B(0)**, SHA1 `32883308031f8bfbec4483948495696b14f0cef8`(메인 재빌드 같음, `build/localization-logs/hns-20261005-173107-chunk166.log`). 새 경고 0.
- 한글이 든 소스 줄 변경: 0(네 커밋 모두 비ASCII 변경 줄 0).
- 전체 테스트(`build/port-check-chunk166.log`): PASSED 2,374 / TOTAL 5,304. **사라진 PASS 0.** 기준 목록과 다른 줄은 2개뿐: eject_button "Eject Button will activate before Red Card if holder is faster" `KNOWN_FAILING` → PASS, throat_spray 새 테스트 PASS. 새 기준 목록 [`test-baseline-seq170.txt`](test-baseline-seq170.txt).
- 한글 회귀(저장소 밖, `tmp-166/kortests` 316개 = 기존 세트 264 + 이 묶음 신규 52): 메인 재실행 요약이 적용 담당의 seq 168 뒤 요약과 같다. 바뀐 것은 #9784의 의도한 순서 변화 18개와 기존 세트 T9(동속 반사 순서) 하나.
- 세이브: 정적 비교는 묶음 1부터의 표시와 같고 새 변수 차이는 `gBattlersByRawSpeed` +4 B(배틀 전용, 저장 안 됨) 하나. 세이브 왕복(이식 전 세이브를 이식 후 ROM으로 불러와 다시 저장) PASS: 일반 세이브 섹터·새 게임 섹터 바이트 같음, 불러오기 결과 95줄 같음.

## 공통 사항

- 사전 분석(읽기 전용 4개, 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-166-170/`): `seq166-9784.md`(+한글 회귀 `tmp-166/kortests/`, 1.17.0 대조 `tmp-166/up-117`), `seq168-9832.md`, `seq169-9835.md`, `seq170-9751.md`. 기준 사본은 묶음 3 뒤 코드.
- 적용: 적용 담당 1개가 patch 4개를 그대로 넣었다(손으로 옮긴 곳 0). 지시 `chunk-166-170/APPLY.md`, 기록 `apply/PROGRESS.md`. 네 빌드 모두 새 경고 0(게임·테스트 빌드), #9784·#9832 SHA1은 사전 분석 사본과 같다.
- 메인 결정
  - #9784 결정 1: HnS `BS_EffectsAfterFormChange`의 배틀러 순서 루프를 유지하고 주석만 고쳤다. upstream 계획은 이번에 `gBattlersByRawSpeed`로 바꾸라고 했지만, 이 배열은 seq 193 #9957 전까지 `MoveEndNextTarget`에서만 정렬돼 1턴 메가진화 때 정렬 전 값을 읽는다. seq 193 때 1.17.0 형태로 바꾼다.
  - #9784 결정 2: 출력 순서 변화(사전 분석 4.2절 18개 + 기존 세트 T9)는 upstream 1.17.0 동작이라 그대로 받았다. 문자열은 바뀌지 않는다.
  - #9832: Acrobatics hunk 제외(이미 같음).
  - #9751: 도우미 가드 A안(`#if !TESTING`). 테스트 빌드의 `-Wunused-function` 경고를 막는다. 게임 빌드의 `AreMultiPartiesFullTeams` 기계어는 이식 전과 같다(사전 분석 실측).

## seq 166 #9784 (`487898f831`)

- upstream `910e018fc9`(9파일 +109/−171). 25 hunk 중 그대로 19, HnS 문맥에 맞춘 5(patch에 포함), 제외 1(공백, 이미 같음).
- **바뀌는 것**
  - `gBattlersByRawSpeed`(EWRAM 4 B): 매 기술의 `MoveEndNextTarget`에서 능력치 스피드(랭크·마비·순풍·느림보·스카프·트릭룸 무시)로 정렬. 동속이면 고정 순서(무작위 아님, RNG 소비 변화 없음).
  - 레드카드·탈출버튼: "유효 스피드 최고 1개"에서 "실제 스피드 순, 레드카드 1회·탈출버튼 1회까지 둘 다". `redCardActivated`는 #9674 때 남겨 둔 `BattleStruct` 빈 비트(`unused5`)를 쓴다.
  - 하양허브·흉내허브·탈출팩: `MOVEEND_ITEM_ON_STAT_CHANGE` 한 단계로 합쳐 목스프레이·과사열매·허탕보험 뒤에 실제 스피드 순으로. 흉내허브가 목스프레이 상승까지 복사한다.
  - 교체가 대기 중인 기술(유턴 등)의 move end에서 능력치가 떨어진 탈출팩(약한갑옷, 솜털 등): 교체로 들어온 포켓몬의 등장 처리에서 늦게 발동하던 것이 사라짐(`MOVEEND_ITEM_ON_STAT_CHANGE`가 `tryEjectPack`을 항상 지움, `disableEjectPack` 삭제, 1.17.0 실측과 같음). 솜털 경우는 리뷰 실측. 위기회피와 겹치면 전후 같음.
  - `MOVEEND_ITEMS_EFFECTS_ALL`이 다시 돈다(`moveEndBattler` 초기화): 매지션으로 빼앗은 열매를 빼앗은 직후 사용.
  - 반사(매직미러·매직코트) 순서가 항상 실제 스피드 순.
- **HnS 보존**: 레드카드·탈출버튼·탈출팩·하양허브 아이템 팝업(스크립트 쪽이라 diff 밖), HnS #9976 공격자 생존 검사, `b07fd88953`, `f8a465e3bc`, `8552b5e9a8` 줄이 그대로다. 한글 문자열·배틀 스크립트 변경 0.
- 테스트: eject_button "Eject Button will activate before Red Card if holder is faster" `KNOWN_FAILING` → PASS, throat_spray 새 테스트 1개 PASS.
- 한글 회귀(`tmp-166/kortests`, 이 PR 신규 52개 + 기존 세트): 신규 52개 중 이름에 `CHANGE EXPECTED`인 18개만 바뀌고(문장 같음, 순서·발동 배틀러만), 기존 세트는 HNSFIX1 T9(동속 반사 순서) 하나만 바뀜. 적용 담당 결과가 사전 분석 기준 요약과 같다.

## seq 168 #9832 (`c625740524`)

- upstream `7ad665dfd2`. `TryRedCard`·`TryEjectButton` 안의 지닌 도구 검사를 `MoveEndCardButton`의 `switch`로 옮김. 한 배틀러가 두 도구를 지닐 수 없어 동작 같음(한글 회귀 post166과 같음).

## seq 169 #9835 (`b391fc833c`)

- upstream `be6874cf59`. `CalcMoveBasePowerAfterModifiers`의 솔라빔 `case`, `UpdatePartySelectionDoubleLayout`의 `MENU_DIR_RIGHT` `case`를 중괄호로 감쌈.
- ROM 크기 0. map 차이는 `.text.TryUpdateEvolutionTracker` +4 하나: 그 함수의 assert 줄 번호(`__LINE__`)가 11008 → 11010이 되어 `movs+lsls` 두 명령에서 리터럴 워드로 바뀌었다(objdump 확인, 정렬 여백에 흡수).
- 줄 번호와 무관한 차이 하나(리뷰 발견): `PartyMenuButtonHandler`(974 B)가 크기는 같은 채 기계어 배치만 바뀌었다. 중괄호로 `party`·`partySlot`의 수명 끝 위치가 달라져 컴파일러의 블록 병합이 달라진 것이다(GIMPLE 비교). 함수 호출 13개·리터럴 6개가 같고 동작은 같다. 솔라빔 쪽 중괄호는 기계어를 바꾸지 않는다(줄 번호를 고정한 변형 빌드로 `battle_util.o` 바이트 동일).

## seq 170 #9751 (`e12e86b186`)

- upstream `f25594e8c7`. 테스트 러너: `IsAITest()` 공개, `Test_BattlersShareParty` 추가(교체 중복 검사 4곳), `AreMultiPartiesFullTeams()`에 `#if TESTING` 분기(AI 테스트는 러너 파티 크기, 그 밖은 `TRUE`).
- HnS: `AreOpponentsFacilityTrainers()`(배틀타워 상대 141 분리, `b43032bf03`)를 `#if !TESTING`으로 감쌈.
- 게임 빌드: map 차이는 `.text.BattleScriptPush` +4(assert 줄 번호) 하나, ROM +16 B는 주소 이동으로 생긴 정렬 여백. 리뷰가 #9751 전체에 `#line`으로 줄 번호를 #9835 뒤와 같게 맞춘 빌드를 만들었더니 SHA1이 #9835 뒤 빌드와 같았다(게임 코드 불변 증명). `Test_BattlersShareParty` 4곳은 HnS 테스트 빌드에서 게임 쪽 `BattlersShareParty`와 결과가 같다.
- 테스트: 테스트 빌드에서 `AreMultiPartiesFullTeams` 반환값이 바뀌는 두 트레이너 테스트 64개(17파일)도 결과·실패 사유가 같다(사전 분석 8절). 적용 담당 실측: 그 17파일 + `ai_twelves.c` 결과 382줄과 실패 사유까지 전후 같음.
- **알아 둘 것**: 이 PR 뒤로 테스트 빌드는 `AreMultiPartiesFullTeams`의 게임 판정을 거치지 않는다(AI 테스트 말고는 항상 풀 팀). 그래서 반 팀 판정에 기대는 HnS 수정 둘을 테스트 러너로 잴 수 없다. 게임 코드는 같다.
  - 배틀타워 상대 141 분리(`b43032bf03`): `chunk-1385/f141/zz_f141.c`가 18/18 → 9/18(사전 분석 5절).
  - 반 팀 멀티에서 목호 포켓몬에게 쓰는 도구(`624ef7d4bd`, `GetItemTargetPartyOwner`): `chunk-1385/fix/zz_fix_r2.c`가 14/14 → 7/14(리뷰 발견). seq 209 #10039 모양으로 바꿔도 7/14(플레이어 4~5마리면 풀 팀 판정).
  - 다시 재려면 사본에서 `#if TESTING` → `#if 0`, `#if !TESTING` → `#if 1`로 바꿔 두 파일을 돌린다(둘 다 다시 18/18·14/14, 저장소에 넣지 않음). 위 두 테스트는 원래 저장소 밖 스크래치라 저장소 테스트 목록에는 영향이 없다.

## 커밋 리뷰

리뷰 3개(읽기 전용, 저장소 밖 스크래치 사본 빌드, 1.17.0 사본 대조). 결과: `/home/hjm0725/hns-sync-work/chunk-166-170/review-ejectcode/`, `review-ejectout/`, `review-misc/`의 `REVIEW-RESULT.md`.

| 대상 | 판정 | 내용 | 처리 |
|---|---|---|---|
| `487898f831` #9784 (코드) | 문제 없음 | upstream `+/-` 줄 대조에서 빠진 줄 0. `gBattlersByRawSpeed`를 읽는 3곳은 모두 정렬 뒤(예외는 플라워가드뿐, 배열이 깨져도 `IsBattlerAlive` 가드로 건너뛰기만 함). `redCardActivated` 켜고 끄는 곳 각 1곳, 기술·턴·배틀 사이에 남지 않음. `BS_EffectsAfterFormChange` 루프 유지가 맞음(호출부가 모두 턴 시작). HnS 팝업·#9976·`b07fd88953`·`f8a465e3bc`·`8552b5e9a8` 유지. 같은 편 탈출버튼 + 상대 레드카드(새 조합)도 소프트락·assert 없음 | 탈출팩 미발동 범위(솜털 등) 보충 |
| `487898f831` #9784 (출력·도달) | 문제 없음(문서 보완 2) | 신규 52개 재현 + 독립 probe 61개(pre·post·1.17.0). 새 문자열·사라진 문장·팝업 0. **분석 4.2절 밖 변화 2개**: 흉내허브가 목스프레이·허탕보험 상승도 바로 복사(이전에는 복사 못 하고 다음 기술 뒤 문장만 나오고 소모), 역린 마지막 턴 혼란 문장이 하양허브보다 먼저. 둘 다 1.17.0과 같음 | 출력 변화 문서 새 행 2에 반영 |
| `c625740524` #9832 | 문제 없음 | Acrobatics 제외가 맞음(이미 같음). 동작 같음 | — |
| `b391fc833c` #9835 | 문제 없음(문서 보완 1) | 줄 번호를 고정한 변형 빌드로 분해: 솔라빔 쪽은 기계어 불변, `PartyMenuButtonHandler`만 크기 같은 배치 변화(동작 같음) | 위 seq 169 절에 보충 |
| `e12e86b186` #9751 | 문제 없음(기록 1) | `#line`으로 줄 번호를 맞춘 빌드 SHA1이 #9835 뒤와 같음(게임 코드 불변). 141 분리 기계어 같음. **반 팀 판정에 기대는 HnS 수정 `624ef7d4bd`도 테스트 러너로 잴 수 없게 됨**(스크래치 `zz_fix_r2.c` 14/14 → 7/14) | 위 "알아 둘 것"과 재확인 8i에 기록 |

- HnS 기본 게임 도달(리뷰 eject-out): 레드카드·탈출버튼은 BP 교환(64 BP)으로만 얻고 상대 트레이너는 레드카드 0, 탈출버튼은 목호(post-OBC, 싱글) 하나라 **레드카드·탈출버튼 변화는 플레이어가 더블에서 내 편 둘에 쥘 때만** 닿는다. 하양허브는 상대 트레이너(블레인, 멜레멜레 수영선수, 더블 핀리 등)·프런티어·텐트·트레이너힐과 플레이어 BP 교환·픽업으로 **실제로 닿는다**. 탈출팩·흉내허브·목스프레이는 얻는 경로·보유 트레이너가 없어 닿지 않는다. 챌린지 "랜덤 아이템"을 켜면 여섯 가지 모두 플레이어 쪽에서 닿는다. 야생 지닌 도구·야생 더블·다이맥스로는 닿지 않는다.

## 출력 변화

`BATTLE_MESSAGE_OUTPUT_CHANGES.md` 「특성·도구·도주」에 #9784 4행을 더하고 기존 3행을 고쳤다. 문장은 모두 기존 문자열이다.

- 새 행: 레드카드·탈출버튼 둘 다 발동(seq 193 전 기준), 하양허브·흉내허브·탈출팩 단계 통합(흉내허브의 목스프레이 복사, 난동 혼란 순서 포함), 교체 대기 중 탈출팩 미발동, 매지션으로 빼앗은 열매 즉시 사용.
- 고친 행: #9494 교체 대기열 행(남은 move end 순서, 탈출버튼 추가), #9494 목스프레이 단계 행(하양허브·흉내허브·탈출팩보다 앞), #9674 상대 둘 반사 행(차례가 실제 스피드 순).
- 전후 같음(리뷰 확인): 레드카드·탈출버튼 하나만 맞는 경우, 대타출동·프리폴·끈적끈적네트·유턴으로 들어온 위협, 교체할 포켓몬이 없는 경우, 1턴 메가진화 위협 뒤 하양허브·탈출팩, 1대2 오른쪽 트레이너 파티에서 끌려 나옴(#8943 HnS 가드).

## 범위 밖 발견 (고치지 않음)

- 편승 문장 주체(이식 전후 같음): 껍질깨기·칼춤처럼 사용자 자신을 대상으로 하는 기술을 편승이 따라 하면, 편승 팝업 뒤 문장이 원래 사용자 이름(`마자용의 공격이 크게 올라갔다!`)으로 나온다. `BattleScript_OpportunistCopyStatChange`가 `B_DEF`를 쓴다. seq 181 #9730(`trybattlerstatchange`) 뒤 다시 확인.
- 재확인 3b(탈출버튼으로 들어온 춤추기 포켓몬이 팝업만 뜨고 춤을 추지 않음): 이식 전후 같고 upstream 1.17.0도 같다. 탈출팩으로 들어온 경우도 같고, 같은 턴만 해당한다(다음 턴부터 정상, 레드카드로 끌려 나온 경우는 정상). 원인 `Cmd_attackcanceler`의 `usedEjectItem` 조기 반환.
- 이식 전후 같은 그 밖의 차이(리뷰 eject-out, 이 묶음과 무관): 마지막 포켓몬의 막말내뱉기에 HnS는 탈출팩이 발동하고 1.17.0은 발동하지 않는다. 야생 하양허브 테스트 종료 때 HnS만 `task not freed`(도구 팝업 쪽 테스트 종료 처리).

## 실기 확인 항목 (친구용)

1. 더블배틀에서 레드카드·탈출버튼이 같은 범위기에 맞을 때 둘 다 발동하는지(빠른 쪽 먼저)
2. 하양허브·탈출팩·흉내허브가 같은 기술 뒤 실제 스피드 순으로 나오는지, 목스프레이 뒤인지
3. 유턴으로 약한갑옷 + 탈출팩 포켓몬을 칠 때 탈출팩이 발동하지 않는지

## 후속 행 메모

- seq 193 #9957: `BS_EffectsAfterFormChange`를 1.17.0 형태(`gBattlersByRawSpeed`)로 바꾸고 이번 HnS 주석을 지운다. 등장 처리 정렬로 바뀌어 중간 상태(정렬 전 배열)가 사라진다. 이때 레드카드가 먼저 발동해 끌려 나온 포켓몬이 등장하면 배열이 다시 정렬돼, 더 느린 탈출버튼이 빠질 수 있다(1.17.0 실측, 리뷰 A1) — 출력 변화 문서 레드카드·탈출버튼 행을 다시 확인. 플라워가드 첫 move end 흉내허브 문제도 사라지는지 확인.
- seq 181 #9730: 흉내허브 연쇄(빠른 쪽 흉내허브가 상대의 복사를 다시 복사)가 지금은 문장만 나오고 소모된다(1.17.0은 연쇄 없이 도구가 남음). HnS에서 닿지 않지만 그때 리뷰 probe R3(`review-ejectout/`)를 다시 돌린다.
- seq 196 #9988: `TryEjectButton`·`TryEjectPack`의 프리폴 조건(`IsBattlerInvolvedInSkyDrop()`)과 `TrySwitchInEjectPack` 프리폴 조건. `full-sync-seq-063-082.md`의 "#9784 이식 때" 메모는 #9988 때로 정정.
- seq 228 #10161: 미래예지가 `moveendall`로 바뀌면 #10344에서 뺀 `TryRedCard`·`TryEjectButton`의 `EFFECT_FUTURE_SIGHT` 조건을 넣는다(한글 K9-01·02 기준). 같은 때 HnS `8552b5e9a8`를 지운다.
- seq 181 #9730: 편승 문장 주체 재확인.
- seq 249 #10288: 추적(Pursuit) 대상이 교체하는 중에 탈출버튼이 발동하는 upstream 결함(이식 전부터, 이 묶음과 무관). HnS는 추적이 흔하니 그때 한글 회귀로 확인(리뷰 권고).
- seq 209 #10039: `AreMultiPartiesFullTeams`의 `IsAITest()` 조건이 빠지면 일반 `TEST()`도 러너 상태로 판정한다. 두 트레이너 테스트 64개 영향 확인(재확인 목록).
- seq 233 #10210: `follower_npc.c`의 label 뒤 선언(#9835와 같은 유형).
