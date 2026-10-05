# full-sync 실제 port 결과 — 묶음 5: seq 171, 172, 173 (+174 이미 적용)

완료: 순서표 seq 171~174를 처리했다. 171·172·173은 PR별 커밋으로 이식했고(173은 HnS 보정 커밋 1개 추가), 174 #9856은 seq 127 #9655 이식 커밋 `f3b491dfc4`에 이미 들어 있었다. 다음은 **seq 175 #8434(XL, Overworld Encounters)**다(별도 단위로 다룬다). 180 #9843은 #8943 단위에서 선반영.

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md)
시작 HEAD: `a29e0c57a2`(친구 답 반영 뒤). 작업 컴퓨터: 데스크탑(2026-10-05).

## 요약

| seq | PR | 판정 | 커밋 | ROM 변화 | 비고 |
|---|---|---|---|---:|---|
| 171 | #9799 Adds tests for switch battle messages and switch AI bugfix | 적용(HnS 적응) | `729674ecf5` | +320 B | 링크 인트로 전용 문장(기존 한글 본문 재사용, 새 문장 0). AI 교체 버그 수정(`BattlersShareParty`). HnS 보정 5곳(H1~H5) |
| 172 | #9850 Cleanup and tests for ow_abilities.c | 일부 적용 | `0b29f4e551` | 0(SHA1 같음) | 싱크로 판정을 새 헬퍼로, 옛 헤더 삭제, 새 테스트 7개 |
| 173 | #9847 Shouldswitch refactor to use `SwitchContext` | 적용(HnS 적응) | `482d67210b` | −560 B | AI 교체 판단 리팩터. upstream은 스마트 교체가 없는 AI의 기본 교체를 지운다 |
| 173 | (HnS) keep the pre-#9847 default switch for non-smart AI | HnS 보정(B안) | `cc8fa0418a` | +624 B | 기본 교체를 `// HnS:` 블록으로 유지(사용자 승인). HnS 트레이너 624/651명의 교체 판단이 이식 전과 같다 |
| 174 | #9856 Fix contact damage message printing | 이미 적용 | (`f3b491dfc4`) | — | 중복 `printstring` 1줄을 HnS가 처음부터 넣지 않음. 한글 테스트 6개로 문장 1회 확인 |

- 빌드(최종 `cc8fa0418a`): 종료 코드 0, **ROM 32,716,836 B(+384 B) / EWRAM 250,132 B(0) / IWRAM 25,516 B(0)**, SHA1 `1a03f47f55b7e8b5aceb7ae66508dbeb85788931`(메인 재빌드 같음, `build/localization-logs/hns-20261005-221348-chunk171.log`). 새 경고 0.
- 한글이 든 소스 줄 변경: #9799의 링크 문장 토큰 1개, 인트로 전용 문장 1개(기존 본문 그대로 복제), 이름 변경 1개뿐. 새 한글 문장·STRINGID 0.
- 전체 테스트(`build/port-check-chunk171.log`): PASSED 2,411 → **2,419** / TOTAL 5,304 → 5,322. 기준 목록(`test-baseline-seq170-friend1005.txt`)에서 사라진 PASS는 upstream이 이름을 바꾼 1개(`AI_FLAG_SMART_SWITCHING: AI will switch out if it has an absorber` → `… and current mon loses the 1v1`, PASS)뿐. 더해진 줄: #9850 테스트 7개 PASS, #9847 `AI can switch out if it loses the 1v1` PASS, #9799 `Battle Message:` 10개 FAIL(영문 `MESSAGE`). upstream이 지운 기본 교체 테스트는 B안이 되살려 그대로 PASS. 새 기준 목록 [`test-baseline-seq174.txt`](test-baseline-seq174.txt).
- 한글 회귀(저장소 밖 `chunk-171-174/tmp-171/kortests` 328개. K2-02는 #9847 뒤 FAIL이라 #9799 `BattlersShareParty` 수정을 더는 지키지 못한다 — 다음 묶음 전에 플레이어 HP 999 형태로 바꾸기를 권함(리뷰) = 기존 316 + #9799용 12): #9799 뒤 기존 316개는 이식 전과 같고 HNS9799 12개는 모두 PASS. #9847 뒤 HNS9799 K2-02 하나가 PASS → FAIL(스마트 AI "1:1을 이기면 남는다", 아래). 메인 재실행 결과가 적용 담당 결과와 같다.
- 세이브: 정적 비교는 묶음 4 뒤와 같은 표시(새 차이 없음). 세이브 구조·enum 변화 없음.

## 공통 사항

- 사전 분석(읽기 전용 4개, 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-171-174/`): `seq171-9799.md`(+한글 회귀 `tmp-171/kortests/`), `seq172-9850.md`, `seq173-9847.md`(+같은 입력 비교 도구 `tmp-173/shadow-tools/`), `seq174-9856.md`. 기준 사본은 `a29e0c57a2` 코드.
- 적용: 적용 담당 1개가 patch 4개를 그대로 넣었다(손으로 옮긴 곳 0). 지시 `chunk-171-174/APPLY.md`, 기록 `apply/PROGRESS.md`. 네 빌드 모두 새 경고 0(게임·테스트 빌드).
- 메인 결정
  - #9799 결정 1~5: 분석 기본값(HnS 보정 H1~H5). 결정 6(녹화 통신 멀티 재생): upstream대로.
  - #9847 결정 1: **B안(HnS 동작 유지)** — 사용자 승인("굳이 허락을 받을 필요는 없을 것 같고 B로 가고 친구한테 보고"). #10151·#10454의 "HnS 밸런스 유지" 선례와 같은 방향. 결정 2: `SHOULD_SWITCH_LOSES_1V1_PERCENTAGE` upstream 기본 0(= HnS 현재 동작).

## seq 171 #9799 (`729674ecf5`)

- upstream `972bffb89f`. 링크·녹화 링크·인게임 파트너·2대1 인트로/넣기/교체 문장 분기 재작성, 링크 인트로 전용 `sText_LinkTrainerIntroSendOutPkmn`, `BattleSideHasTwoTrainers`에 `opponentB` 조건, `GetAiFlags` 특수 트레이너 판정, `IsSwitchinValid`에 `BattlersShareParty`(#8943 뒤 두 트레이너가 같은 슬롯 번호를 고르면 둘째가 막히던 버그), 새 테스트 10개.
- **한글:** 인트로 전용 문장은 기존 `sText_LinkTrainerSentOutPkmn` 본문을 그대로 쓰고 토큰만 `{B_LINK_OPPONENT_MON1_NAME}`(기존 `{B_OPPONENT_MON1_NAME}`과 같은 펼침 코드). `full-sync-seq-138.5-138.5.md`에서 미뤄 둔 분리를 이번에 했다.
- **표시 이름 증명(사전 분석 6절):** 통신 싱글·더블, 4인 통신 멀티(내가 왼쪽/오른쪽), 유니온룸, 배틀타워 통신 멀티, 인게임 파트너 멀티(로켓단 아지트), 2대1, 1대2, 통신 싱글·더블 녹화 재생에서 고르는 문장과 이름이 이식 전과 같다.
- **HnS 보정 5곳**(upstream 줄을 그대로 넣으면 생기는 회귀를 막음)
  - H1 `GetAiFlags`: 특수 트레이너 판정에 `BATTLE_TYPE_TRAINER` 조건. 없으면 통신·유니온룸 배틀 뒤 `opponentA`가 남아, 다음 트레이너 배틀 전까지 배회 포켓몬이 도망가지 않고 사파리·스마트 야생의 AI 플래그가 0이 된다.
  - H2 `IsSwitchinValid` override 분기: HnS 비교값(`AI_monToSwitchIntoId`) 유지 + `BattlersShareParty`. upstream 줄이면 같은 트레이너의 둘째 포켓몬이 같은 흡수 포켓몬 교체를 고르고 조용히 취소돼 턴을 잃는다(1.17.0·master도 같음).
  - H3 `sText_LinkTrainerSentOutPkmn2`: `{B_BUFF1}` 유지. upstream MON2 토큰이면 통신 더블 비마스터 화면에 이전 포켓몬 이름이 나온다.
  - H4 배틀타워 통신 멀티 인트로: `TOWER_LINK_MULTI`를 먼저 본다(upstream 순서면 상대 소개가 통신 문장으로 바뀌어 직업 표기 없이 이름만 나옴 — 이름은 `gLinkPlayers[2]`·`[3]`의 프런티어 이름이라 맞다, 리뷰 정정).
  - H5 상대 넣기·교체 조건 4곳의 `BattlerIsLink()` 유지: 유니온룸·배틀타워 통신 멀티 출력이 이식 전과 같다. upstream 형식(직업+이름)과의 차이는 형식뿐이라 친구 확인 거리로 남김(재확인 10f).
- **출력 변화(결정 6):** 멀티 **녹화 재생**에서만 바뀐다. 4인 통신 멀티 녹화는 인트로 끝 `{PAUSE 49}`가 빠지고 오른쪽 상대 넣기·교체 이름이 상대 1로, 배틀타워 통신 멀티 녹화(`BattleFrontier_BattleTowerMultiBattleRoom_hns` 저장, 프런티어 패스 재생)는 오른쪽 상대 넣기·교체 이름이 상대 1로 나온다(인트로는 H4로 그대로, 리뷰 발견). 녹화에는 `LINK`가 빠지고 `TWO_OPPONENTS`도 없어 `BattleSideHasTwoTrainers`가 거짓이다. 이 재생들은 #8943 뒤 상대 B 파티 매핑부터 어긋나 있어(재확인 22) upstream을 따랐다.
- 테스트: `test/battle/battle_message.c` 새 10개는 영문 `MESSAGE`라 HnS 빌드에서 모두 FAIL(Unmatched MESSAGE). 같은 10개를 한글로 옮긴 HNS9799 K1과 AI 교체 K2(2개)는 이식 뒤 모두 PASS. `ai_switching.c` 결과 전후 같음.

## seq 172 #9850 (`0b29f4e551`)

- upstream `fc6d083821`. `ow_abilities.h`와 헬퍼 두 개는 HnS에 이미 있다(#9878 일부 선반영). 남은 것: `GetSynchronizedNature`의 `IsSynchronizeActive` → `DoesLeadingMonHaveAbilityEffect(sForceNatureAbilities)`(HnS 싱크로 챌린지 분기·헤롱헤롱바디 가드 유지), 옛 `ow_synchronize.h` 삭제(include 5곳, HnS 전용 `pokemon.c` 포함), 새 `test/ow_abilities.c`(7개 PASS).
- **게임 ROM 바이트 동일**(SHA1이 #9799 커밋 뒤와 같음). HnS 조우 경로(조토 표, 바위깨기·박치기, 낚시, 사파리, 벌레잡기 대회, 대량발생, 랜더마이저, 배회, 정적 조우)는 같은 함수로 모이고 RNG 소비도 같다.

## seq 173 #9847 (`482d67210b`) + HnS B안 (`cc8fa0418a`)

- upstream `099aff910c`. `ShouldSwitch` 계열 20여 함수가 `struct SwitchAiContext`를 받도록 리팩터. HnS 형태(#10302 3줄, #10626 Palafin, `GetDynamicMoveType`, EELEVATE)를 유지한 채 context 필드로 맞춘 hunk 6개는 patch에 들어 있다. `ai_thinking_time.c` 상한 hunk는 제외(HnS 29 > upstream 27).
- **이름은 리팩터지만 AI 결정이 바뀐다(사전 분석 1절).**
  - 가장 큰 변화: 스마트 교체가 없는 AI의 기본 교체(`FindMonWithFlagsAndSuperEffective` — 마지막으로 맞은 기술을 반감·무효로 받고 상대에게 효과 굉장 기술이 있는 파티 포켓몬으로 무효 50%·반감 33% 교체, 원작 에메랄드 규칙) 삭제. HnS 트레이너 624/651명(이야기 관장·사천왕·라이벌·레드 전원)과 동행 파트너 5명, 프런티어형 배틀이 해당.
  - **HnS B안 커밋이 이 규칙을 `// HnS:` 블록으로 되살렸다.** 사전 분석의 같은 입력 비교(이식 전 함수를 그림자로 같이 돌림)와 리뷰의 독립 비교(9개 난수 흐름)에서 비스마트 AI의 판단·난수열 차이가 0이다. 예외(리뷰): B로 되돌리지 않은 A 변경 세 가지 — 좋은 후보가 없을 때의 대체 후보 순서, 불가사의부적 대응 검사(실제로는 Gorilla Tactics 구애 잠금만), 더블의 게으름 판정 — 는 비스마트에도 남지만 HnS 데이터로는 사실상 닿지 않고 닿아도 난수를 쓰지 않는다.
  - 스마트 교체 AI 27명(`*_POSTOBC_HNS` 재대결 22명, `TRAINER_STEVEN_HNS`·`TRAINER_FINLEY_HNS`·`TRAINER_MUALANI_HNS`·`TRAINER_SAMSON_OAK_HNS`, 미사용 1명)에는 upstream 변화가 그대로 들어간다: 흡수 특성·차지·공중 무적·소원 교체가 "1:1을 이기면 남는다"로, 좋은 후보가 없을 때 교체 대상이 첫 후보 → 마지막(또는 무작위). `SHOULD_SWITCH_LOSES_1V1_PERCENTAGE` 0이라 "1:1에서 지면 교체"는 꺼져 있다.
- 테스트: AI 테스트에서 upstream이 이름을 바꾼 1개(PASS → PASS), 새 PASS 1개(`AI can switch out if it loses the 1v1`), upstream이 지운 기본 교체 테스트는 B안이 되살려 PASS.
- 한글 회귀: HNS9799 K2-02(두 트레이너가 각자 흡수 포켓몬으로 교체)가 PASS → FAIL. 1턴에 두 상대의 할퀴기가 모두 왼쪽 플레이어 포켓몬을 때려 HP가 120/186이 되고, 그 포켓몬과 마주한 왼쪽 상대(스마트 AI)가 "1:1을 이긴다"고 보고 남는다. #9847 스마트 AI 변경 3번(의도)이며 B 블록과 무관하다. 플레이어 HP를 999로 둔 탐색 테스트에서는 두 상대 모두 교체(#9799 수정 유지), K2-01은 PASS 유지.

## 커밋 리뷰

리뷰 2개(읽기 전용, 스크래치 사본). 결과: `/home/hjm0725/hns-sync-work/chunk-171-174/review-link/REVIEW-RESULT.md`, `review-ai/REVIEW-RESULT.md`.

| 대상 | 판정 | 내용 | 처리 |
|---|---|---|---|
| `729674ecf5` #9799 | 문제 없음(문서 보완 3) | 바뀐 비ASCII 줄 3줄, 새 한글 본문·STRINGID 0(인트로 전용 문장은 이식 전 문장과 토큰 바이트 하나만 다르고 펼침 코드 같음). upstream과 문장 분기 diff: 다른 곳은 HnS 챌린지 2쌍·H4·H5뿐. 모든 통신·인게임 경로가 이식 전 문장을 고름(코드 추적). H1: 임시 테스트 5개 중 upstream 줄이면 배회·사파리·스마트 야생 3개 FAIL → 필요. H2: upstream 줄이면 K2-01 FAIL → 필요 | 배틀타워 통신 멀티 녹화 재생도 출력 변화에 추가, "비마스터 빈 이름" 설명을 "형식 차이"로 정정, H2~H5 후속 행(재확인 8r) 추가 |
| `0b29f4e551` #9850 | 문제 없음 | ROM SHA1이 #9799 커밋과 같음(바이트 동일), 새 테스트 7/7, 싱크로 챌린지·헤롱헤롱바디 가드 유지 | — |
| `482d67210b` #9847 | 문제 없음 | 12파일 중 11개는 변경 줄이 upstream과 같음. `battle_ai_switch.c`는 HnS 고유 차이(#10302·6인자 `GetDynamicMoveType`·EELEVATE·#10626)가 변수 이름만 다르고 유지. #9799 H1·H2, #8943 가드, #8472, AI 상한과 충돌 없음 | — |
| `cc8fa0418a` HnS B안 | 경미 | 블록 위치·순서, 확률(50/33), `RNG_AI_SWITCH_SE_DEFENSIVE` 태그·단락 평가 순서가 이식 전과 같음. A가 지운 `IsBattlerAlive`를 B가 조건식에서 다시 검사. 전체 테스트 입력 5,483개 + 이식 전 테스트 3파일 + HnS형 시나리오(싱글·더블·동행 파트너 멀티·2대1·1대2 × 플래그 6종)를 이식 전·후 코드로 같은 상태에서 돌려, **비스마트 교체 판단 5,687회의 난수열 차이 0**, 점수 뒤 교체 판단 8,361회 차이 0. 스마트 AI 차이 26건은 모두 A 변경(의도) | 되살린 테스트에 `// HnS:` 표시 추가(커밋 수정, 원래 `18d216675f`), 예외 세 가지 문서화 |

- 스마트 트레이너 27명에게 보일 변화(리뷰 예시): 흡수 특성 교체 기준이 "한 방"에서 "1:1 승리"로(예: 엔딩 뒤 재대결 관장의 선봉이 1:1을 이길 때는 흡수 포켓몬으로 바꾸지 않고 공격), 공중날기·구멍파기 대응도 1:1을 이기면 남음, 더블(`TRAINER_FINLEY_HNS`·`TRAINER_MUALANI_HNS`)은 정면 상대 하나로 판정해 좌우가 다르게 움직일 수 있음(K2-02와 같은 현상), 좋은 교체 대상이 없으면 무작위 선택. 예측 트레이너 25명은 seq 184 #9857 전까지 예측한 변화기·속이기를 무시하는 중간 상태.
- [참고] upstream 기존 코드의 구애 잠금 검사 괄호 오류(`battle_ai_switch.c` 311행 근처, 1.17.0에도 그대로), 지역 변수가 매개변수를 가림(`battle_ai_main.c` 386행 근처, 값 같음) — 조치 없음.

## 범위 밖 발견 (고치지 않음, #9799 사전 분석 9절)

- `sText_LinkTrainer2SentOutPkmn2` 본문 끝 `!!`(4인 통신 멀티 오른쪽 상대 교체). 한글 본문 수정이라 친구 확인 거리.
- 배틀타워 통신 멀티에서 마스터 화면의 오른쪽 상대 넣기·교체 문장이 트레이너1 문장(이식 전부터, `TWO_OPPONENTS` 플래그 없음). 비마스터 화면은 통신 문장이라 직업 표기 없이 이름만 나온다(이름은 맞음, 리뷰 정정).
- `sText_LinkTrainer2WithdrewPkmn`이 HnS는 `{B_LINK_SCR_TRAINER_NAME}`(upstream #8247 이후 `{B_LINK_OPPONENT2_NAME}`). #9799 diff 밖.

## 실기 확인 항목 (친구용)

1. 이야기 관장·라이벌전에서 상대가 반감·무효 + 효과 굉장 포켓몬으로 교체하는지(이전과 같아야 함)
2. 엔딩 뒤 재대결 1명(스마트 AI)을 싱글로 상대해 교체가 지나치게 늘거나 줄지 않는지
3. 동행 파트너 더블배틀, 로켓단 아지트 목호 멀티 인트로·교체 문장
4. 장비가 있을 때: 통신 싱글·더블 상대 교체 이름(마스터·비마스터), 유니온룸 교체 이름, 통신 배틀 직후 배회 포켓몬이 도망가는지

## 후속 행 메모

- seq 182 #9865: `GetAiFlags` 특수 트레이너 판정이 `IsSmartBattle`로 옮겨질 때 H1(`BATTLE_TYPE_TRAINER`) 보정도 함께 옮긴다.
- seq 184 #9857: #9847의 `GetPredictedMoveSpeedCheck`를 `GetIncomingMove`로 되돌린다.
- seq 204 #10051: 이름 변경, 그 테스트 hunk는 #9850이 먼저 들어가야 붙는다(이번에 들어감).
- seq 294 #10436: `INTROSENDOUT` 조건(`IsOnPlayerSide`)이 #9799가 바꾼 분기와 같은 곳.
- seq 311 #10542: `BATTLE_PARTNER` 매크로가 없어진다 — #9799 줄과 B안 블록(`battle_ai_switch.c` 1525행 근처, 2회)을 `GetPartnerBattler`로 바꾸지 않으면 빌드가 실패한다.
- seq 182 #9865: 소원 함수의 줄 끝 공백 hunk는 HnS가 이미 정리해 뺀다.
- seq 403 #9970: `pokemon.c`의 `ow_abilities.h` include는 #9850 때 이미 들어갔다.
