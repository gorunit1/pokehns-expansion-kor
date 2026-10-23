# full-sync 묶음 29 — seq 386 (#10217)

- 요청: 사용자 "ㄱㄱ 혼자 되는지 보자"(사전 분석 — 혼자 가능으로 보고) → "ㄱㄱ"(2026-10-23, 일루전 보정 포함).
- 범위: seq 386 #10217(Fixes Pickpocket activation when the original attacker isn't on the field). 다음은 387.
- upstream: 읽기 전용 사본 `/home/user/pokeemerald-expansion-upstream`에서 `git format-patch`로 읽었다.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 386 | #10217 | `467845f6` | 소매치기 원래 공격자 처리 + HnS 보정 3개 + HnS 회귀 테스트 | 32,963,348 |

이식 전(`6002c047`) 32,962,948 B → **32,963,348 B(+400)**, EWRAM 250,408·IWRAM 25,516 같음, SHA1 **`f82541bc993741cf3a573cce1ed867140eb09b47`**.

## 내용(upstream)

- 레드카드 플래그(`BattlerState.redCardSwitched`)가 `originalBattlerPartyId`(4비트, 기본 `PARTY_SIZE`)로 바뀐다. 배틀러가 빠질 때(`switchoutabilities`) 원래 파티 번호를 적고, 기술 끝(`MoveEndClearBits`)에 지운다. 생명의구슬·조개껍질방울·목청스프레이·실패보험·유턴류·기술 뒤 폼 체인지·땅구르기 타이머의 "같은 배틀러" 판정이 이 값을 본다(레드카드뿐 아니라 탈출버튼 등 기술 중 교체 전부).
- 소매치기: 원래 공격자가 기술 중에 빠졌으면 파티에 있는 그 포켓몬의 도구를 훔친다(도구 칸을 컨트롤러로 비움, 트레이너전 플레이어 쪽이면 배틀 뒤 복원 표시). 점착 판정이 스크립트에서 C로 옮겨지고(`BattleScript_PickpocketPrevented`), 빠진 공격자의 특성·도구는 보지 않는다.
- 빼앗는 문장 `STRINGID_PKMNSTOLEITEM`의 도구 주인 이름이 `StealTargetItem`이 준비하는 `gBattleTextBuff2`가 된다(더블 마술사 등에서 이름이 정확해짐).
- 마술사의 점착 판정에서 "점착 포켓몬이 살아 있음" 조건 삭제.

## HnS 적응·판단

1. **MoveEnd 형태:** HnS `MoveEndPickpocket`은 아직 `BattleCalcValues` 전 형태라 `gBattlerAttacker`·`GetBattlerAbility`·`GetBattlerHoldEffect`로 옮겼다. HnS가 앞당겨 둔 `attackerInParty` 검사(미래예지 공격자가 파티에 있을 때)는 upstream HEAD처럼 함수 맨 앞 조기 반환.
2. **한글 문장 토큰 1개(`TRANSLATION_POLICY.md` 74):** `{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_DEF_NAME_WITH_PREFIX}{B_TXT_EU}로부터\n{B_LAST_ITEM}{B_TXT_EULREUL} 빼앗았다!`에서 `{B_DEF_NAME_WITH_PREFIX}`만 `{B_BUFF2}`로. 문구·줄바꿈 그대로. 버퍼 접두어(`상대 `/`야생 `)는 이름 토큰과 같은 문자열이고, 조사 토큰은 앞 글자 받침으로 고르므로 같게 나온다(아래 프로브).
3. **일루전 이름 유지:** upstream 버퍼는 파티의 실제 별명을 써서 일루전 변장을 무시한다(이전 이름 토큰은 변장 이름). HnS `StealTargetItem`은 일루전이 켜져 있으면 변장 대상의 파티 번호로 버퍼를 만든다(`// HnS:`). 프로브: 일루전 조로아크가 소매치기 포푸니를 할퀴면 이식 전·후 모두 `포푸니는 상대 마자용으로부터⏎상처약을 빼앗았다!`, 보정을 빼면 FAIL(정체 이름).
4. **upstream 회귀 보정 — 기술 밖 교체 뒤 첫 기술(재확인 84):** upstream은 `originalBattlerPartyId`를 기술 끝에서만 지운다. 그런데 `switchoutabilities`는 기술 밖 교체(턴 시작 교체 행동, 상대 기절 뒤 "교체" 스타일로 바꾸기)에서도 불린다. 그 뒤 다른 배틀러의 기술 끝이 오기 전에 바뀐 포켓몬이 기술을 쓰면 생명의구슬 반동·유턴 교체·조개껍질방울·목청스프레이·실패보험·기술 뒤 폼 체인지가 빠진다. 프로브(양쪽이 같은 턴에 교체 → 다음 턴 첫 기술): 이식 전 PASS 2, upstream 그대로 FAIL 2(생명의구슬 반동 없음, 유턴 교체 없음). HnS는 `HandleAction_UseMove`에서 공격자의 값을 `PARTY_SIZE`로 지운다 → PASS 2. upstream HEAD도 지우는 곳이 같아(시작·기술 끝·`SwitchPartyOrder`) 같은 결함이 있다고 본다. 회귀 테스트 `test/battle/hns_original_battler_reset.c`(2개, 언어 무관).
- 새 배틀 문장 0, 세이브 구조 변경 0(`BattlerState` 비트만, 배틀 중 구조체).
- HnS 트레이너 데이터: 소매치기 0, 도둑·탐내다·마술사 1. 보통 상황의 출력은 같다(아래 프로브).

## 검증

- **빌드:** `make hns -j8` 종료 0, 새 경고 0(`warncheck.sh`).
- **한글 프로브**(세션 임시 클론, 저장소에 넣지 않음): 소매치기 보통 `상대 포푸니는 마자용으로부터⏎몬스터볼을 빼앗았다!` 이식 전·후 PASS, 도둑 `마자용은 상대 포푸니로부터⏎상처약을 빼앗았다!` 이식 전·후 PASS, 레드카드 뒤 소매치기 이식 후만 PASS(새 동작), 일루전 위 3번.
- **소매치기 테스트**(`TESTS="Pickpocket"`): 새 3개 포함 FAIL은 모두 `Unmatched MESSAGE`(영어 문장), 스프레드 1개는 `ASSUME`(HnS 데이터) — 이식 전과 같은 종류.
- **세이브 정적 비교**(이식 전 사실 = 세션 임시 `b30/save-pre`, 코드 `6002c047`을 클론에서 빌드 — ROM 32,962,948 B로 같음): **PASS(FAIL 0, WARN 0)**.
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 `467845f6`): 러너 PASSED 2,752 / TOTAL 5,726(KNOWN_FAILING 25), 목록 — `test-baseline-seq385_5.txt` 대비 **사라진 PASS 0**, 새 PASS 2(HnS 회귀 테스트), 새 upstream 소매치기 테스트 3개 FAIL(영어 `MESSAGE`). 바뀐 줄 1개: `Battle strings fit on the battle message window`는 이식 전·후 모두 FAIL(`EXPECT_LE(216, 208)`)인데 첫 실패 문장이 123번(`STRINGID_PKMNSTOLEITEM`)에서 416번(`STRINGID_SYMBIOSISITEMPASS`, 원래 넘치던 문장)으로 바뀌었다. 이 테스트는 `gBattleTextBuff2`를 빈 문자열로 채워서 `{B_BUFF2}`가 된 빼앗기 문장의 이름 폭을 재지 못한다. 실제 게임에서는 버퍼가 이전 이름 토큰과 같은 접두어+별명을 넣으므로 폭이 같다(긴 라틴 별명 12자 기준 216 px 초과는 이식 전부터, 기존 HnS 문장). 새 기준 [`test-baseline-seq386.txt`](test-baseline-seq386.txt)(**다음 비교 기준**).
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-b29 8`, 코드 `467845f6`): **558/652, 기대와 바이트 같음, `판정: PASS (기대와 같음)`**(`HNSB2021` 30/30 포함).
- **실기: 하지 않음(친구).**

## 실기(mGBA) 후보 — 친구

1. 생명의구슬을 든 포켓몬으로 교체(교체 스타일로 상대 기절 뒤 바꾸기 포함) → 다음 턴 첫 공격에 반동이 있는지. 유턴·볼트체인지도 같은 상황에서 교체되는지.
2. 도둑·탐내다로 도구 빼앗기 문장(`…로부터⏎…을(를) 빼앗았다!`) 이름·조사.
3. (있으면) 소매치기·레드카드·탈출버튼.

## 재확인·뒤 PR 메모

- 새 재확인 84(기술 밖 교체 뒤 `originalBattlerPartyId` HnS 초기화), 85(도구 빼앗기 문장 일루전 이름 HnS 보정).
- 출력 변화 문서 1행(레드카드 등으로 빠진 공격자에게서 소매치기).
- 순서표: 386 적용.
