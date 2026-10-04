# full-sync 실제 port 결과 — seq 138.5 (#8943 12v12 capability, + 같은 unit 후속 7개 선반영, HnS 보호 수정 3개)

완료: seq 138.5 unit `U-12v12-8943`을 이식했다. #8943(+#9725) 커밋 뒤에 같은 unit에서 #8943 회귀를 고치는 후속 PR 7개를 PR별 커밋으로 바로 붙였다(선반영). upstream 1.17.0에도 남는 #8943 결함 가운데 이식 전 HnS에서는 정상이던 동작 3개는 HnS 수정 커밋으로 지켰다. 커밋 리뷰(병렬 5개)가 찾은 목호 멀티 파티 번호 충돌 묶음은 리뷰 후 HnS 수정 커밋 3개로 고쳤고, 수정 커밋도 리뷰했다. 전체 테스트·세이브 호환 검증·한글 회귀 테스트는 최종 HEAD에서 다시 돌렸다. 다음 seq는 **139 #9714**다. 이번에 선반영한 seq **140·141·164·180·215·287·307·342**는 닿으면 "이미 적용"으로 처리한다.

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), 메시지 출력 기록: [`BATTLE_MESSAGE_OUTPUT_CHANGES.md`](../../../localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md), 완료 전 재확인: [`RECHECK_BEFORE_COMPLETION.md`](RECHECK_BEFORE_COMPLETION.md)
시작 HEAD: `1ce2b6d4bf`(작업 트리 clean, 코드는 `b07fd88953`과 같음). 작업 컴퓨터: 데스크탑(`/usr/bin` 툴체인).

## 요약

| seq | PR | 판정 | 커밋 | ROM 변화 | 비고 |
|---|---|---:|---|---:|---|
| 138.5 | #8943 (+140 #9725) | 적용(HnS 적응) | `022e666847` | −1,376 B | 파티를 `gParties[MAX_BATTLE_TRAINERS][PARTY_SIZE]`로 트레이너별로 나누고 파티 접근을 트레이너 단위로 일반화. 녹화 배틀 새 형식(A안). #9725(multi_do 지역 레이블)는 분리하면 HnS 빌드가 깨져 같은 커밋에 넣었다 |
| 141 | #9729 | 적용(선반영) | `4b437cd5da` | 0 | 야생 더블 둘째 포켓몬을 `gParties[B_TRAINER_1][1]`로 되돌림(HnS 알로라 섬 스크립트 더블 야생 7곳) |
| 164 | #9811 | 적용(선반영) | `8e205d9e92` | −752 B | `GetBattlerPartyState`를 트레이너 기준으로 |
| 180 | #9843 | 적용(선반영) | `7db72aa705` | 0 | 배틀 중 요약 화면의 파티 순서 |
| 215 | #10102 | 적용(선반영, 이름 적응) | `4616fac998` | +16 B | 통신 싱글·더블 비마스터 쪽 파티. `B_TRAINER_OPPONENT_A/B` → `B_TRAINER_1/3`(#10051 전 이름) |
| 287 | #10415 | 적용(선반영) | `262bd0abd7` | −64 B | 두 트레이너 배틀 오른쪽 트레이너의 일루전 탐색 범위(+테스트) |
| 307 | #10536 | 적용(선반영, HnS 적응) | `93656e609b` | +16 B | 경험치 버그 수정. HnS `Cmd_getexp` 고유 줄 유지, `givenExpMons` 인덱스 `>> 1` HnS 보정(upstream 1.17.0은 배열 범위 초과) |
| 342 | #10662 | 적용(선반영) | `b09e11c9b5` | 0 | 멀티 파트너 일루전의 파티 슬롯 충돌(+테스트) |
| — | HnS 수정 | 강제 교체 | `70fe10ecd8` | +16 B | 울부짖기·날려버리기·드래곤테일·배대뒤치기·레드카드가 오른쪽 트레이너(상대 B·목호)를 대상으로 하면 실패하던 #8943 결함 |
| — | HnS 수정 | 경험치 참가 비트 | `c68e8e13ba` | +80 B | 목호 파트너 멀티에서 한 번도 나가지 않은 플레이어 포켓몬이 참가 경험치를 받던 #8943 결함 |
| — | HnS 수정 | 반 팀 화이트아웃 | `cc0576a543` | +32 B | 반 팀 멀티에서 파트너 기절 수가 플레이어 화이트아웃 판정에 더해지던 #8943 결함 |
| — | 리뷰 후 HnS 수정 | 파티 번호 충돌(배틀 쪽) | `8bcf557c20` | +368 B | 목호 멀티에서 내 포켓몬에게 쓴 도구·배운 기술·레벨업 연출이 같은 번호의 목호 포켓몬에게 가던 것 등(아래 "리뷰 후 HnS 수정") |
| — | 리뷰 후 HnS 수정 | 목호 포켓몬에게 쓰는 도구 | `624ef7d4bd` | −432 B | 반 팀 멀티에서 목호 포켓몬에게 쓴 도구가 효과 없이 사라지던 것, 한 기술용 PP 회복약 기술 목록 |
| — | 리뷰 후 HnS 수정 | 배틀 밖 가드 | `6c15064bce` | +16 B | `GetBattlerPartyStateByPokemon` NULL 가드 복원 |
| — | 친구 요청 HnS 수정 | 배틀타워 상대 141 분리 | `b43032bf03` | +32 B | 시설 배틀 상대는 HnS 트레이너의 `Multi Party`를 보지 않음(아래 "배틀타워 상대 141 분리") |
| — | 수정 커밋 리뷰 후 HnS 수정 | 조수·AI 수·치유방울 | `f3a58f9939` | +176 B | 같은 부류 남은 3곳(아래 "리뷰 후 HnS 수정") |

- 빌드(최종 코드 `f3a58f9939`): 종료 코드 0, **ROM 32,715,172 B(97.50%, −1,872 B) / EWRAM 250,128 B(95.42%, +1,192 B) / IWRAM 25,516 B(77.87%, 0)**. `pokehns.gba` SHA1 `4f87458453ed4d9f701debe458f1a038531ad2ef`. 메인이 저장소에서 다시 빌드해 같은 SHA1을 확인했다(로그 `build/localization-logs/hns-20261004-174522-post8943r4.log`). 중간 값: 리뷰 전 `cc0576a543` 32,715,012 B(SHA1 `355a9b72…`), R1~R3 뒤 `6c15064bce` 32,714,964 B(`2f821353…`). 이 값에는 단위 밖 인트로 수정 `5bbd01e69f`(ROM 0 B)도 들어 있다.
- 새 경고 0(16개 커밋 모두).
- 한글이 든 소스 줄 변경: `battle_message.c` 3쌍(6줄)의 이름 토큰만(아래 "한글 토큰 증명"). 본문·조사·인코딩 길이 불변. 후속·HnS 커밋은 한글 줄 변경 0.
- 세이브: **일반 세이브 호환 유지(실측).** 정적 비교 PASS(FAIL 0, WARN 3 = 의도된 차이), 이식 전 ROM이 만든 세이브 2개를 이식 후 코드로 읽어 바이트 단위로 같게 복원했다. 녹화 배틀만 A안대로 새 형식이 되어 옛 기록은 "기록 없음"이 된다.
- 전체 테스트(`cc0576a543`, `6c15064bce`, 최종 `f3a58f9939` 세 번, 목록 같음): PASSED 2,350 / FAILED 2,260 / KNOWN_FAILING 10 / TOTAL 5,271, Killed 0, INVALID 21(이전과 같은 수). 기준 대비 **사라진 PASS 0**, 새 PASS 6, 새 FAIL 4(모두 알려진 한계). 새 기준 목록 [`test-baseline-seq138.5.txt`](test-baseline-seq138.5.txt).
- 한글 턴 종료 회귀 테스트 70개(seq 132 D, 저장소 밖): `cc0576a543`·`6c15064bce`·`f3a58f9939` 모두 66 PASS / 4 FAIL로 이식 전과 같다(FAIL 4개는 seq 132의 의도된 upstream 변경).
- `B_MULTI_HALF_TEAMS FALSE`(사용자 결정)로 두 트레이너 동시 발견 6쌍과 배틀타워 멀티 UI가 바뀐다. 아래 "출력·동작 변화"와 "친구에게 물을 것".

## 공통 사항

- 툴체인: 데스크탑 기본 PATH `arm-none-eabi-gcc`(`/usr/bin`). PATH 접두어 없이 빌드했다.
- 빌드 명령: `GITHUB_ACTION=1 make hns -j8 > build/port.log 2>&1`(커밋마다).
- 이식 전 기준(데스크탑, `b07fd88953` 코드): ROM 32,717,044 B / EWRAM 248,936 B / IWRAM 25,516 B, SHA1 `04c307c5833443210e1b7f65126312188b82f3f8`. 세션 시작 때 다시 빌드해 같음을 확인했다.
- 경고 비교: `LC_ALL=C grep -a 'warning:' build/port.log | sed -E 's/:[0-9]+:[0-9]+: /: /' | sort -u | comm -13 warn-base.txt -`. 기준은 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-ahead-130-167/warn-base.txt`.
- 사전 분석(2026-10-04, 읽기 전용 5개, 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-1385/`)
  - A(`part-A.md`/`.patch`, 76파일): 파티 코어·헤더·배틀 밖 사용처
  - B(`part-B.md`/`.patch`, 24파일): 배틀 코어·컨트롤러·AI·한글 토큰
  - C(`part-C.md`/`.patch`, 72파일): 파티 메뉴·박스·프런티어·녹화·trainerproc·`.party`·config·테스트
  - D(`part-D.md`, `verify/`): 세이브·메모리 호환 검증 도구(정적 비교 `save_compat.py`, 테스트 러너 세이브 왕복 `savetest/`)
  - E(`part-E.md`, `E-<seq>-<PR>.patch`): 같은 unit 18행 분류, 후속 patch 7개, HnS 보존 제안
  - A+B+C를 스크래치에 합쳐 빌드·전체 테스트·세이브 검증을 미리 마쳤다. 실제 적용 결과(ROM·테스트 목록)는 스크래치 예측과 같다.
- 적용: 적용 담당 1개가 patch를 차례로 `git apply --check` → `git apply`로 쌓았다(손으로 옮긴 곳 0). patch md5: A `412fddee…`, B `80c11f33…`, C `25253682…`.
  - 주의(재사용 시): `part-C.patch`의 파일 삭제·생성 hunk에 `deleted/new file mode` 줄이 없어 `git apply`가 저장소 루트에 빈 `dev/null` 파일을 만들고 새 `ai_twelves.c`를 0755로 만들었다. `dev/`를 지우고 `chmod 644` 뒤 커밋했다(커밋: `create mode 100644 test/battle/ai/ai_twelves.c`, `delete mode 100755 include/test/test_runner_battle.h`). `ai_twelves.c` 내용은 upstream blob과 같다.
- 이번 단위에서 메인이 한 검증: 재빌드(SHA1 동일), 한글 줄 검사, `save_compat.py run`, 세이브 왕복, 전체 테스트, 한글 턴 종료 회귀 D 테스트.

## 사용자·확정 결정

- **A안**(친구 승인 `FRIEND_REPLY_2026-10-04.md`): upstream 1.17.0 녹화 배틀 형식을 쓴다. 옛 녹화 기록 무효화를 허용하고 변환 계층을 만들지 않는다. 일반 게임 세이브 호환은 유지한다.
- **`B_MULTI_HALF_TEAMS = FALSE`**(2026-10-04 사용자 결정, **친구 확정**: 디스코드 답 (a) FALSE 유지 — HGSS도 2인 트레이너전에서 각자 자기 파티를 쓰므로 원작에 더 가깝다. 기록 `FRIEND_REPLY_2026-10-04.md` 2절) + 로켓단 아지트·디버그 멀티 트레이너 4명(`TRAINER_ARIANA_1_HNS`, `TRAINER_GRUNT_23_HNS`, `TRAINER_LANCE_3_HNS`, `TRAINER_CLAIR_3_HNS`)에 `Multi Party: Half`. FALSE로 생기는 변화는 출력·동작 변화로 기록했다. 친구가 적은 확정 결정(계획서 5절 #8943 행)은 "멀티배틀 편성은 별도 기존 정책을 유지한다"이다. TRUE로 바꾸면 이식 전과 완전히 같다(config 1줄, 대안 patch `chunk-1385/tmp-C/alt/alt-B_MULTI_HALF_TEAMS-TRUE.patch`).
- 같은 unit 후속 7개를 #8943 바로 뒤 PR별 커밋으로, HnS 보호 수정 3개를 별도 커밋으로(사용자 지시).

## 동기화 단위: seq 138.5 #8943 `U-12v12-8943` 12v12 capability (+ seq 140 #9725)

- 현재 판정: 적용(HnS 적응)
- 커밋: `022e666847`
- upstream 근거: `70340c1135`(168파일 +3709/−3192). #9725 `53a361902a`. HnS 커밋은 172파일 +3868/−3342(upstream 168파일 + HnS 전용 소스 4개 `bug_contest.c`·`nuzlocke.c`·`surfable.c`·`trainer_card.c`의 이름 변경, `trainers_hns.party`의 Half 4명).
- 내용(upstream 그대로)
  - `gPlayerParty`/`gEnemyParty`(각 6칸)를 `gParties[MAX_BATTLE_TRAINERS][PARTY_SIZE]`·`gPartiesCount[4]`로 바꾸고, 옛 이름은 매크로(`gParties[B_TRAINER_0/1]`)로 남긴다. 파트너는 `gParties[B_TRAINER_2]`, 상대 B는 `gParties[B_TRAINER_3]`에서 0번부터 쓴다(이전에는 플레이어·상대 A 파티의 3~5칸).
  - `GetBattlerParty`·`GetTrainerParty`·`GetBattlerMon`·`GetBattlerTrainer`·`BattleSideHasTwoTrainers`·`BattlersShareParty`·`AreMultiPartiesFullTeams` 등 트레이너 단위 함수, `partyState`·`itemLost`·`activeGimmick`·`AiPartyData`의 `[MAX_BATTLE_TRAINERS]` 배열.
  - 두 트레이너 쪽의 반 팀(3마리) 여부를 `AreMultiPartiesFullTeams()`(`B_MULTI_HALF_TEAMS`, 트레이너별 `Multi Party: Half`, 링크)로 정한다. `struct Trainer.multiTeamSize`와 trainerproc `Multi Party:` 키.
  - 풀 팀 멀티 파티 메뉴(`PARTY_LAYOUT_MULTI_FULL*`, L/R 페이지), 녹화 배틀 `RecordedBattleSave`(파티 4개, 배틀러 기록 664 → 388 B), 배틀 프런티어 상대 B가 자기 포켓몬 세트를 쓴다.
  - 테스트 러너 멀티 지원 재작성(병합 커밋 `8c950fb648` 형식), 새 테스트 `ai_twelves.c` 등.
- **HnS 적응**
  - `struct Trainer`: `encounterMusic:5`·`mugshotColor:4`(upstream 4/3). HnS 곡 0~26과 `MUGSHOT_COLOR_LIGHT_BLUE`(8) 때문이다. `padding:2`를 없애 16비트·`sizeof` 52 B 그대로. upstream 폭이면 `data.c`가 `-Werror=overflow` 478건으로 실패한다(실측).
  - `CalculatePlayerPartyCount`: HnS 파티 크기 제한 챌린지 상한 `GetMaxPartySize()`를 유지하고 이름만 바꿨다.
  - `recorded_battle.c`: 녹화 시작 파티 `sSavedParties`를 upstream의 힙 포인터 대신 **정적 EWRAM 배열 `[4][6]`**로 둔다(+2,400 B). upstream 형식이면 녹화 재생 때 `CB2_InitBattle`의 힙 초기화가 저장한 파티를 덮어 재생 뒤 플레이어 파티가 깨지고, 녹화 저장에 전투 뒤 파티(상대 HP 0)가 들어간다(upstream 1.17.0·master 같음, 미병합 브랜치 `grintoul-recorded-battle-fix`). `RecordedBattleSave` 형식은 upstream과 같다.
  - `asm/macros/battle_frontier/battle_tower.inc`: `multi_do`를 #9725의 숫자 지역 레이블로 넣었다. HnS는 `multi_2_vs_2`를 6번 펼쳐(debug 4, 로켓단 아지트 1, 이끼시티 우주센터 1) #8943 원문 레이블이면 `event_scripts.s`가 어셈블되지 않는다.
  - `include/config/ai.h` AI 사고 시간 상한(테스트 전용): HnS 값과 #8943 값 가운데 큰 쪽 8/22/38/29/33. 사고 시간 테스트 6개 PASS.
  - 테스트 러너 4파일: HnS에 이미 있던 master #9723·#9567과 합친 upstream 병합 커밋 `8c950fb648` 형식.
  - `party_menu.c` 충돌 3곳: HnS 따라가기 메뉴 선언, SELECT 빠른 교체 블록 뒤에 upstream L/R 파티 순환, 합체 파티 가득 판정의 `GetMaxPartySize()`.
  - `battle_main.c`: 사파리 포획계수 HnS 최소 1, 종료 시 도구 복원·폼 복귀를 HnS 위치(미러 복원 앞)에 적용, IV/EV 스케일링 루프 변수, **미러 챌린지**(상대가 두 트레이너면 상대 B를 플레이어 3~5칸에, 파트너 멀티면 파트너 파티에 복사해 이식 전과 같은 편성).
  - `battle_ai_main.c` `Ai_InitPartyStruct`: HnS 능력·도구·기술 개별 전지 플래그를 트레이너 루프 안으로(결과가 1.17.0과 같음). `battle_ai_util.c` `CountUsableSideMons`(#10258 선반영분)를 1.17.0 형태로.
  - upstream diff 밖 HnS 줄의 옛 이름(약 160줄)을 `gParties[...]`로 바꿨다(매크로가 있어 동작 같음, #9885·#10051 대비). `battle_hold_effects.c` 1줄은 매크로로 남았다(동작 같음).
  - `battle_message.c`: 한글 3쌍의 토큰만. **`sText_LinkTrainerSentOutPkmn`은 바꾸지 않았다.** upstream처럼 `{B_BUFF1}`로 바꾸면 인트로 경로에 버퍼가 준비되지 않아 지난 배틀 이름이 나온다(upstream은 #9799에서 인트로 전용 문장을 새로 만들어 고침). #9799(seq 171) 때 처리한다.
  - `B_MULTI_HALF_TEAMS FALSE`(config 추가), `trainers_hns.party`의 4명 `Multi Party: Half`, `trainers.party`의 에메랄드 마그마단 2명 Half(upstream).
- 제외한 hunk: 없음. upstream 줄 끝 공백 몇 줄은 지웠다(`git diff --check`).
- 보존 확인(사전 분석 A의 컴파일 비교): A 담당 소스 68개 중 61개가 이식 전과 어셈블리까지 같고 나머지 7개는 의도한 변경이다. 파티 제한·원타입·너즐록·미러 챌린지·키우미집·부화·교환·Pokegear·동행 포켓몬·필드 특성·트레이너 카드가 함수 단위로 같다. 배틀 쪽 HnS 기능(빠른 인트로·빠른 도주·호부의금화·Champions 분노의주먹·`b07fd88953`·HnS 트레이너 BGM·랜더마이저·레벨업 기술 루프 등)은 3-way 병합으로 보존했다(`HnS` 표기 줄 25 → 26).
- 빌드(`022e666847`): 종료 코드 0, **ROM 32,715,668 B(−1,376 B) / EWRAM 250,128 B(+1,192 B) / IWRAM 25,516 B(0)**, SHA1 `0d6489cfab947baa16b09a3cb551dca7ec4cb741`(사전 분석 스크래치와 같음). 새 경고 0.
- 확인: `gParties` `0x960`, `gPartiesCount` `0x4`, `sSavedParties` `0x960`, `sBattleRecords` `0x610`. 옛 기호(`gPlayerParty`·`gEnemyParty`·`sSavedPlayerParty`·`sSavedOpponentParty`) 없음. 지운 이름(`test_runner_battle.h`, `IsMultibattleTest`, `GetSideParty`, `GetAIPartyIndexes`, `CalculateEnemyPartyCountInSide`, `sMultiPartnerPartyBuffer`) `src`·`include` 0건. 생성 `trainers_hns.h`의 `MULTI_TEAM_SIZE_HALF` 4명. 7개 `.party` 생성 헤더는 `#line`과 새 `.multiTeamSize` 줄을 빼면 이식 전과 바이트가 같다(사전 분석 C).
- 테스트(이 커밋 단독, 전체 1회): PASSED 2,346 / TOTAL 5,266, Killed 0. 사라진 PASS 0, 새 PASS 2(`Celebrate does not need to be explicitly set in a non-AI test`/`in an AI test`), 새 FAIL 3(`12v12: AI can use all 6 party slots in a 12v12 (battler 1/2/3)`, 사유 `Unmatched MESSAGE`).
- 단독 커밋 상태의 회귀(바로 뒤 후속 커밋들이 고침, 중간 상태로 남지 않음): 알로라 스크립트 더블 야생 둘째 포켓몬 소실(#9729), 파트너·상대 B `partyState` 섞임(#9811), 배틀 중 요약 화면 순서(#9843), 통신 싱글·더블 비마스터 파티(#10102), 두 트레이너 경험치 누락·멀티 레벨업 대상(#10536), 일루전(#10415·#10662).

## 같은 unit 후속 7개 (선반영)

계획서 원칙 "리팩터마다 그 회귀를 고치는 후속 PR을 같은 unit으로 바로 붙인다"(선례 #9494+#9864, #9674+#10386)에 따라, #8943이 만든 회귀를 고치고 #8943 밖 미적용 기능에 기대지 않는 행을 골랐다(사전 분석 E 2절). 모두 `--check` 그대로 적용됐다.

| seq | PR (upstream) | 커밋 | 수정 파일 | HnS 적응·확인 | 빌드 ROM | 테스트 |
|---|---|---|---|---|---:|---|
| 141 | #9729 `2b756fd942` Fix wild doubles only having 1 mon | `4b437cd5da` | `battle_ai_main.c`, `battle_main.c`, `script_pokemon_util.c`, `wild_encounter.c` | HnS 전용 `CreateScriptedDoubleWildBossMon`은 원래 `gParties[B_TRAINER_1][1]`이라 그대로 | 32,715,668 | 빌드만 |
| 164 | #9811 `b150c372e6` Use GetBattlerTrainer instead of GetBattlerSide in GetBattlerPartyState | `8e205d9e92` | `battle.h`, `battle_util.h`, `battle_util.c` | — | 32,714,916 | 빌드만 |
| 180 | #9843 `810514990c` Fix in-battle summary preserving battle party order | `7db72aa705` | `party_menu.c` | 멀티 반 팀 분기 그대로 | 32,714,916 | 빌드만 |
| 215 | #10102 `d8f3349d6f` Fix link singles and doubles using incorrect parties | `4616fac998` | `battle_controllers.c` | `B_TRAINER_OPPONENT_A/B` → `B_TRAINER_1/3` | 32,714,932 | 빌드만 |
| 287 | #10415 `2393eb8364` Fix Illusion party search range for two-opponent trainers | `262bd0abd7` | `battle_util.c`, `test/battle/ability/illusion.c` | 테스트 `B_TRAINER_OPPONENT_B` → `B_TRAINER_3` | 32,714,868 | `illusion.c` 새 테스트 PASS, 기존 9개 PASS 7 / FAIL 2 그대로 |
| 307 | #10536 `54a796027f` Multiple xp bugfixes | `93656e609b` | `battle.h`, `battle_main.c`, `battle_script_commands.c`, `battle_util.c`, `test/battle/exp.c` | HnS `Cmd_getexp` 고유 줄(`UseClassicExpSplit`, soft level cap, 레벨 상한 챌린지, HnS 승리 BGM) 유지. **`givenExpMons[(GetBattlerTrainer(x) & BIT_FLANK) >> 1]` HnS 보정**: upstream 1.17.0·master는 `& BIT_FLANK`만 써서 상대 B(트레이너 3)의 인덱스가 2가 되어 `u8 givenExpMons[2]`를 넘고 바로 뒤 `expSentInMons`를 덮는다. 상대 A가 먼저 쓰러진 뒤 상대 B 0번을 쓰러뜨리면 경험치가 빠진다(스크래치 실측 6,992 대 9,099) | 32,714,884 | `exp.c` 새 PASS 2, 새 FAIL 1(HnS 경험치 정책, 아래). 임시 테스트 `zz_e_exp.c` `exp=9099`(보정 형태 기대값) |
| 342 | #10662 `46a1da5cf2` Fixed the Multi Battle party-slot collision | `b09e11c9b5` | `battle_util.c`, `test/battle/ability/illusion.c` | HnS `BATTLE_PARTNER` 표기 유지 | 32,714,884 | `illusion.c` 새 MULTI 테스트 PASS |

- 세 커밋(#9729·#9811·#9843·#10102)은 테스트 러너로 재현할 테스트가 없어 빌드와 코드 리뷰로 확인했다. 실기 확인 항목에 넣었다.
- 원래 자리에 남긴 같은 unit 행(사전 분석 E 2절): #9751(170, TESTING 전용), #9799(171, 링크 인트로 문장·AI 교체), #10051(204, 이름 변경), #9885(205, deprecated 매크로), #10059(207, 풀 팀 멀티 파티 메뉴), #10039(209, 멀티 화이트아웃 재작성), #10568(315)·#10674(340), #10608(323, 수면 클로즈, HnS 꺼짐), #10711(348, 아래 후속 행 메모).

## HnS 보호 수정 3개

upstream 1.17.0(과 확인한 master)에도 남아 있는 #8943 결함 가운데 **이식 전 HnS에서는 정상이던 동작**을 지킨다. 각 줄에 `// HnS:` 주석이 있다.

| 커밋 | 내용 | 근거 | 확인 |
|---|---|---|---|
| `70fe10ecd8` HnS: fix forced switch-outs against the right-hand trainer after 12v12 | `Cmd_forcerandomswitch`가 아직 0~2/3~5 분할을 써서 오른쪽 트레이너(상대 B·목호)가 대상이면 빈 3~5칸만 보고 **실패**했다. 대상 자기 파티 `[0, GetAILastPartyIndex(target))`에서 고르고, 짝 배틀러가 같은 파티일 때만 그 인덱스를 뺀다 | 사전 분석 B 6절 10 | 임시 테스트 4개(1v2 상대 A·B, 멀티 상대 B, 멀티 파트너 날려버리기): 적용 전 1/4 → 적용 후 4/4 PASS. `roar.c`·`hit_switch_target.c`·`red_card.c` 결과·실패 사유 전후 같음 |
| `c68e8e13ba` HnS: keep in-game partner mons from marking player mons as exp participants | 참가 비트 `gSentPokesToOpponent`는 플레이어 자기 파티 인덱스인데, #8943 뒤 목호 포켓몬의 인덱스(0~2)가 같은 번호의 플레이어 포켓몬을 "참가"로 표시했다. `GetBattlerTrainer(i) == B_TRAINER_0`인 배틀러만 비트를 세운다(싱글·더블의 배틀러 2는 `B_TRAINER_0`이라 그대로) | 사전 분석 B 6절 11, E 4.3 | 임시 테스트(파트너 교체 뒤 상대 기절): 적용 전 나가지 않은 L1 메타포드가 L12 → 적용 후 L1 그대로. `exp.c` 전후 같음 |
| `cc0576a543` HnS: keep the half-team multi whiteout check from counting partner faints | #8943 `NoAliveMonsForPlayer`가 파트너 파티의 기절 수를 플레이어 화이트아웃 카운터에 더해, 3마리만 가진 플레이어는 자기 포켓몬이 모두 살아 있어도 목호 3마리가 쓰러지면 화이트아웃이었다. 파트너 기절 수는 `AreMultiPartiesFullTeams()`일 때만 센다(반 팀은 이식 전 판정, 풀 팀은 upstream 그대로) | 사전 분석 E 4.1 | 테스트 러너는 멀티에서 저장 파티 검사를 건너뛰어 재현 불가. 빌드·리뷰만, **실기 확인 필요** |

- ROM: +16 / +80 / +32 B.
- 부작용 확인: `AreMultiPartiesFullTeams()`는 `gSpecialVar_Result`를 설정한다. `NoAliveMonsForPlayer`는 #8943 판에서도 함수 첫머리에서 이미 이 함수를 부르고(`B_MULTI_BATTLE_WHITEOUT` = `GEN_LATEST`), 강제 교체의 `GetAILastPartyIndex`도 배틀 중 AI가 이미 부르는 경로다. 새로 생기는 부작용 경로는 없다.
- 뒤 seq와의 관계: seq 209 #10039가 `NoAliveMonsForPlayer`를 다시 쓰고 seq 315 #10568이 `WillPlayerWhiteOutIfPartnerWinsAlone`으로 대체한다. 그때 HnS 화이트아웃 보정이 필요한지 다시 본다(재확인 목록).

### 수정 커밋 리뷰 (읽기 전용, 결과 `chunk-1385/review-FR{1,2,3}/REVIEW-RESULT.md`)

| 리뷰 | 대상 | 판정 | 요지 |
|---|---|---|---|
| FR1 | R1 `8bcf557c20`, R3 `6c15064bce` | R1 경미 / R3 문제 없음 | R1의 16곳은 형식별(싱글·일반 더블·두 트레이너·반 팀/풀 팀 멀티·프런티어 파트너·통신·녹화)로 맞다. **빠진 같은 부류 3곳**(조수, AI 남은 포켓몬 수·상태 판정, 치유방울 짝 파티 상태)을 실측으로 찾음 → R4 `f3a58f9939` |
| FR2 | R2 `624ef7d4bd` | 문제 없음 | 반 팀 멀티 메뉴 6칸 모두 이식 전과 같은 포켓몬·배틀러·한글 문장 이름(한글 `MESSAGE` 실측, 틀린 이름 대조군 FAIL 확인). `itemPartyIndex` 사용처 11줄 같은 규칙. 경미: 목호 자리가 빈 뒤 내 포켓몬 기력의조각 출력 변화를 출력 변화 문서에 행으로 추가(반영), 주석 2줄(R4에 반영) |
| FR3 | 상대 141 `b43032bf03`, R4 `f3a58f9939` | 둘 다 문제 없음 | 141: 시설 결과가 쓰이는 곳은 모두 배틀 자원 할당 뒤, 필드 호출은 커밋 전과 같은 식, `gBattleTypeFlags` 대입 94곳 전수로 비시설 배틀에 시설 비트가 서지 않음, 탐침 22/22. R4: 치유방울의 SetMonData 두 번 방출은 기존 트릭 패턴과 같고 같은 파티면 방출하지 않음, 2v2 멀티 새 테스트 9/9(R4를 되돌리면 6 FAIL), 남은 포켓몬 수 이식 전과 같음. 경미: 치유방울 역방향 차이를 문서에 보강(반영) |

- 뒤 PR 메모(리뷰): seq 204 #10051은 `ShowMoveSelectWindow` 줄에서, seq 311 #10542(`BATTLE_PARTNER` → `GetPartnerBattler`)는 `BS_Item*`·HnS 도우미·`AnyPartyMemberStatused`·강제 교체에서 손 병합이 필요하다(HnS 줄 유지, 이름만 바꿈). seq 348 #10711은 재확인 목록 8d.

## 한글 토큰 증명 (사전 분석 B 5절, E 6절)

`battle_message.c`의 한글 3쌍만 바뀌었다. 모두 2바이트 토큰끼리 바꿔(`B_BUFF1` = FD 00, `B_OPPONENT_MON2_NAME` = FD 08, `B_LINK_PLAYER_MON1_NAME` = FD 09, `B_LINK_PLAYER_MON2_NAME` = FD 0B) 인코딩 길이가 같다. 조사(`{B_TXT_EULREUL}`)는 앞에 펼쳐진 이름의 받침으로 고르므로 이름이 같으면 조사도 같다.

| 문장 | 이전 → 이후 | 쓰는 곳 | 화면 이름 |
|---|---|---|---|
| `sText_LinkTrainer2SentOutPkmn2` | `{B_OPPONENT_MON2_NAME}` → `{B_BUFF1}` | 교체 등장(`STRINGID_SWITCHINMON`) 통신 오른쪽 상대 | 교체 직전 `switchindataupdate`가 같은 배틀러의 새 포켓몬 이름을 `gBattleTextBuff1`에 넣는다. 같은 이름. 통신에서 인덱스가 늦게 도착하는 순간에는 오히려 새 포켓몬 이름이 정확히 나온다 |
| `sText_LinkPartnerSentOutPkmn2` | `{B_LINK_PLAYER_MON2_NAME}` → `{B_BUFF1}` | 4인 통신 멀티, 오른쪽 통신 파트너 교체 | 같은 이름 |
| `sText_LinkPartnerSentOutPkmn1` | `{B_LINK_PLAYER_MON1_NAME}` → `{B_BUFF1}` | 4인 통신 멀티, 내가 오른쪽일 때 왼쪽 파트너 교체 | 이전에는 **내 포켓몬 이름**이 나오던 버그 → 파트너의 새 포켓몬 이름(4인 통신 멀티에서만 출력 변화) |

- 새 한글 문장 0. 경험치 문장(`STRINGID_PKMNGAINEDEXP` 등)은 #10536 뒤 배틀러 인자가 0으로 고정되어 이름이 모든 배틀 형식에서 플레이어 파티 포켓몬이다(이식 전과 같음).

## 세이브·메모리

- 정적 비교 `python3 /home/hjm0725/hns-sync-work/chunk-1385/verify/save_compat.py run`(최종 HEAD 빌드): **판정 PASS (FAIL 0, WARN 3)**, 기대 보고서(`verify/expected/report-abc.txt`)와 판정 줄 68개가 모두 같다.
  - SaveBlock1/2/3·PokemonStorage·Pokemon·BoxPokemon·명예의 전당 등 17개 구조체의 모든 필드 위치·폭·크기 같음(SaveBlock1 15,760 / SaveBlock2 3,892 / SaveBlock3 52 / PokemonStorage 34,256 B), 세이브 상수 30개·섹터 배치 14칸·`sGFRomHeader` 58개 같음.
  - 세이브 경로 코드 54개 함수(`save.o`·`load_save.o` 전체 + 파티·박스 접근)가 정규화 뒤 같다.
  - WARN 3 = 의도된 차이: `CalculatePlayerPartyCount`(배열 원소가 되며 레지스터 배치만 다름, HnS `GetMaxPartySize` 유지), `HandleSpecialTrainerBattleEnd`(멀티 종료 뒤 세이브 파티 복원을 `!AreMultiPartiesFullTeams()`일 때만 — HnS 반 팀은 이전과 같음), 이 둘을 합친 검토 줄.
  - `RecordedBattleSave` 4,008 → **4,092 B = 섹터 한도(여유 0 B)**. 옛 기록은 새 checksum 자리(4,088)가 0이라 언제나 무효가 된다.
- 세이브 왕복(테스트 러너, 스크래치 사본 `tmp-D/post`): `run_all.sh` **두 이미지 모두 PASS**.
  - 이식 전 ROM이 만든 세이브(`pre-make.sav`, `pre-newgame.sav`)를 이식 후 코드의 `LoadGameSave → LoadPlayerParty`로 읽은 결과(파티 6·박스 몬 6의 원시 바이트와 104필드, 세이브 블록 해시, 다시 저장한 섹터 0~30)가 이식 전과 같다.
  - 같은 게임 상태를 이식 후 코드로 저장한 섹터 0~30이 이식 전과 바이트 단위로 같다.
  - 녹화 기록 유효: 1 → 0(A안). 이식 후 형식으로 새로 쓴 기록은 다시 읽으면 유효.
- EWRAM +1,192 B(남는 공간 13,208 → 12,016 B): `gParties` +1,202, `sSavedParties` 정적(HnS) +1,200, `sBattleRecords` −1,104, `gMultiPartnerParty` −108. IWRAM 0. 힙(`BattleStruct`·`AiPartyData`)은 배틀당 약 +0.5 KB.

## 출력·동작 변화

### 배틀 메시지 (`BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 1행)
- 4인 통신 멀티의 교체·소개 문장 이름·순서(위 한글 토큰 증명, 링크 위치 배정 변경과 짝). 인게임 배틀(로켓단 아지트 멀티, 두 트레이너)의 문장은 같다.

### upstream대로 둔 동작 변화 (배틀 메시지 아님)
1. **`B_MULTI_HALF_TEAMS FALSE`로 바뀌는 것(사용자 결정, 친구 질문)** — 사전 분석 C 6.2
   - **두 트레이너 동시 발견 배틀:** `Multi Party: Half`가 없는 트레이너는 자기 파티 전체를 낸다(이전에는 각 3마리). HnS에서 4마리 트레이너가 끼는 **6쌍**이 바뀐다(벽·높이를 무시한 시선 근사 분석 23쌍 중, 나머지 17쌍은 3마리 이하라 같음).

     | 맵 | 쌍 | 이전 → 이후 |
     |---|---|---|
     | 검은먹시티 체육관(`BlackthornCity_Gym_hns`) | MIKE(3)·**LOLA(4)** | LOLA 3 → 4마리 |
     | 검은먹시티 체육관 | **PAUL(4)**·FRAN(3) | PAUL 3 → 4마리 |
     | 라디오타워 4층(`GoldenrodCity_RadioTower_4F_hns`) | GRUNT_9(3)·**GRUNT_28(4)** | GRUNT_28 3 → 4마리 |
     | 46번 도로(`Route46_hns`) | TED(3)·**ERIN(4)** | ERIN 3 → 4마리 |
     | 아쿠아호 B1F(`SSAqua_B1F_hns`) | DEBRA(1)·**JONAH(4)** | JONAH 3 → 4마리 |
     | 아쿠아호 북서 객실(`SSAqua_RoomNW_hns`) | EDWARD(1)·**COREY(4)** | COREY 3 → 4마리 |
   - **배틀타워 멀티룸(`_hns`):** 편성(2+2)은 같지만 UI가 풀 팀으로 바뀐다. 전투 중 파티 메뉴가 `MULTI_FULL`(L/R로 파트너 페이지), 전투 전 쇼케이스 2페이지, 요약 화면 이동. 프런티어 상대 141(GRUNT_23과 같은 번호)만 반 팀 UI가 되던 부수 효과는 친구 요청으로 `b43032bf03`에서 분리했다(아래 "배틀타워 상대 141" 절). 이제 141도 다른 상대와 같은 풀 팀 UI다.
   - 로켓단 아지트 B2F 멀티(아테나·조무래기23 vs 플레이어+목호)와 디버그 멀티(목호3·이향3 vs 플레이어+실버), 링크 멀티는 이전과 같다(Half/링크 강제 반 팀). 로켓단 아지트에 Half가 꼭 필요한 이유: 풀 팀이면 전투 뒤 `HandleSpecialTrainerBattleEnd`가 `VAR_RESULT`를 덮어 패배해도 "아테나 격파" 분기로 간다.
   - **배틀타워 멀티의 화이트아웃 판정**(리뷰 C 4): 풀 팀 규칙(기준 12, 파트너 기절 수 포함)이 되어 이제 4마리가 모두 쓰러져야 진다. 이식 전에는 선택하지 않은 저장 파티 4칸이 비었거나 알·기절이면 자기 2마리가 쓰러질 때 졌다. 그런 극단적 경우에만 차이가 난다.
   - **요약 화면 트레이너 메모**(리뷰 C 3): 배틀타워 멀티를 파트너 페이지(L/R)를 연 채 끝내면 `MULTI_FULL_PARTNER`가 남아, 그 뒤 PC 박스나 팩토리 요약의 트레이너 메모에 성격만 나온다(표시만, upstream 같음).
   - **친구 확정(2026-10-04 디스코드): (a) FALSE 유지.** HGSS도 2인 트레이너전에서 각 트레이너가 자기 파티를 쓰는 구조라 6쌍의 4마리 사용이 원작에 더 가깝다. 배틀타워 멀티 UI 변화는 #8943에 따른 변화로 받아들인다. (TRUE로 바꾸면 위 변화가 모두 없어지고 이식 전과 같다 — config 1줄, 쓰지 않음.) `ai_twelves.c` 3개는 TRUE여도 `Unmatched MESSAGE`로 실패하므로 테스트 비용은 없다.
2. **배틀타워 멀티 상대 B의 포켓몬**(이식 전 버그 수정): 상대 B도 상대 A의 세트에서 뽑던 것을 자기 세트에서 뽑는다. 중복 검사도 트레이너별이라 A·B가 같은 종·도구를 가질 수 있다. 배틀 피라미드 2인 전투도 같다.
3. **녹화 배틀:** 옛 기록은 프런티어 패스에서 "기록 없음"이 된다(A안). 배틀러당 기록 한도 664 → 388 B라 아주 긴 전투는 재생이 그 지점에서 끝난다(크래시 없음).
4. **인트로 파티 볼 표시:** 파트너 멀티·두 트레이너 배틀에서 한 줄 6칸 → 트레이너마다 한 줄 3칸.
5. **AI `Ai_InitPartyStruct`:** 기절·종·전지 정보를 파티가 있는 모든 트레이너(자기·파트너 포함)에 채운다.
6. **디버그 전용:** `CreateNPCTrainerPartyFromTrainer`의 셋째 인자가 `firstTrainer` → `halfTeam`으로 뜻이 바뀌어 디버그 플레이어 파티(`DEBUG_TRAINER_PLAYER`)와 `CreateTrainerPartyForPlayer`(HnS 스크립트 사용 0)가 3마리로 잘린다(upstream 같음, #9440 seq 400에서 인자 삭제).

## 배틀타워 상대 141 분리 (친구 요청, `b43032bf03`)

- 친구 요청(2026-10-04 디스코드, `FRIEND_REPLY_2026-10-04.md` 2절): 로켓단 아지트용 `Multi Party: Half`(GRUNT_23 = 번호 141)가 같은 번호의 배틀타워 상대 141의 화면까지 바꾸는 부수 효과를 가능하면 분리.
- 원인: `AreMultiPartiesFullTeams()`가 `TRAINER_BATTLE_PARAM.opponentA/B`로 HnS `gTrainers[난이도][id].multiTeamSize`를 읽는데, 시설 배틀에서는 이 id가 프런티어 트레이너 번호다(upstream 1.17.0·master 같음).
- 수정(`src/battle_util.c` +12/−2, `// HnS:`): 정적 도우미 `AreOpponentsFacilityTrainers()` = `gBattleStruct != NULL && (gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_TRAINER_HILL))`. 참이면 `gTrainers[...].multiTeamSize` 두 줄을 보지 않는다(시설 상대는 `B_MULTI_HALF_TEAMS`·링크 규칙만). 마스크는 `CreateNPCTrainerPartyFromTrainer`가 시설 상대를 거르는 것과 같다.
- `gBattleStruct != NULL`을 함께 보는 이유: `multi_do`(로켓단 아지트 스크립트)는 필드에서 이 함수를 불러 지난 배틀의 `gBattleTypeFlags`가 남아 있다. 플래그만 보면 프런티어 전투 직후 로켓단 아지트에서 3마리 선택을 건너뛴다(사본에서 실측으로 기각). 배틀 자원이 있는 동안에만 플래그가 현재 배틀 것이다.
- 호출 시점 26곳 전수: 필드 호출 4곳(`multi_do`, `HandleSpecialTrainerBattleEnd`, `FillPartnerParty`, 요약 복귀)은 `gBattleStruct`가 NULL이고 HnS 번호만 와서 결과가 같다. 프런티어 번호가 오는 곳(쇼케이스, 전투 중 파티 메뉴·요약·AI·교체·화이트아웃·`battle_util2`·트레이너 슬라이드·인트로)은 모두 배틀 자원이 할당된 뒤다.
- 빌드: ROM 32,714,996 B(+32 B), EWRAM·IWRAM 0, 새 경고 0, SHA1 `948c4fab…`.
- 테스트: 저장소 밖 사본(테스트 빌드의 `gTrainers`에 141 Half를 넣음)에서 임시 18개, 수정 전 13/18(타워·녹화·파이크·피라미드의 141이 반 팀) → 수정 뒤 18/18. HnS 멀티 반 팀, 필드 `multi_do`의 지난 플래그, 링크는 전후 같다. 관련 기존 6파일 전후 같고 전체 목록이 기준과 바이트 단위로 같다.
- 뒤 PR: seq 170 #9751의 함수 hunk는 그대로 적용된다(도우미가 함수 밖). #9751 뒤에는 TESTING 분기 때문에 이 판정을 테스트 러너로 다시 잴 수 없다. seq 348 #10711은 손 병합(재확인 목록 8d).
- 실기: 배틀타워 멀티에서 상대 141이 나올 때 쇼케이스 2페이지·전투 중 L/R 메뉴, 로켓단 아지트 3마리 선택·반 팀·승패 분기, 가능하면 프런티어 전투 직후 다른 전투 없이 디버그 목호 멀티.

## 지정 테스트

| 커밋 | 테스트 | 결과 |
|---|---|---|
| `022e666847` #8943 | 전체 1회 | PASS 2,346 / TOTAL 5,266. 사라진 PASS 0, 새 PASS 2(Celebrate), 새 FAIL 3(12v12, `Unmatched MESSAGE`) |
| `262bd0abd7` #10415 | `illusion.c` | PASS 8 / FAIL 2 / TOTAL 10. 새 테스트 PASS, 기존 FAIL 2(`Unmatched MESSAGE`) 그대로 |
| `93656e609b` #10536 | `exp.c` + 임시 `zz_e_exp.c` | 새 PASS 2(`Both player Pokemon gain experience in double battles`, `Partner Pokemon do not gain experience`), 새 FAIL 1(`Both opponent's Pokemon give experience in battle against two opponents` — `EXPECT_EQ(9049, 2771)`, `Cmd_getexp`가 `B_SCALED_EXP`를 컴파일 상수로 써서 테스트의 `WITH_CONFIG`가 먹지 않고 HnS `UseClassicExpSplit` 정책이 적용됨, 알려진 한계. `>> 1` 유무와 무관). 기존 FAIL 4(`Unmatched MESSAGE`)·INVALID 1 그대로. 임시 테스트 `exp=9099` |
| `b09e11c9b5` #10662 | `illusion.c` | PASS 9 / FAIL 2 / TOTAL 11 |
| `70fe10ecd8` 강제 교체 | 임시 `zz_b_forceswitch.c`, `roar.c`·`hit_switch_target.c`·`red_card.c` | 임시 1/4 → 4/4. 기존 `roar.c` 0/6, `hit_switch_target.c` 3/9, `red_card.c` 8/25 PASS로 전후 목록·실패 사유 같음 |
| `c68e8e13ba` 참가 비트 | 임시 `zz_e_sent.c`, `exp.c` | 임시 FAIL(L1→L12) → PASS(L1). `exp.c` 전후 같음 |

임시 테스트는 저장소 `test/battle/`에 잠시 넣고 돌린 뒤 지웠다(원본은 `chunk-1385/tmp-B/`·`tmp-E/`).

## 전체 테스트

- 명령: `GITHUB_ACTION=1 make check BUILD=hns -j6 > build/port-check-post1385.log 2>&1`(`cc0576a543`), 리뷰 후 수정 뒤 `build/port-check-post1385fix.log`(`6c15064bce`)·`build/port-check-post1385r4.log`(`f3a58f9939`). 목록은 PORT_INSTRUCTIONS의 `LC_ALL=C` 추출. 세 목록이 바이트 단위로 같다.
- 결과: PASSED 2,350 / FAILED 2,260 / KNOWN_FAILING 10 / ASSUMPTIONS_FAILED 38 / TO_DO 607 / EXPECT_FAILING 6 / TOTAL 5,271. INVALID 21(이전과 같음), Killed 0.
- `test-baseline-seq138.txt` 대비: **사라진 PASS 0**. 그 밖의 상태 변화 0.
  - 새 PASS 6: Celebrate 2(#8943 새 테스트), 일루전 2(#10415·#10662), 경험치 2(#10536)
  - 새 FAIL 4: `ai_twelves.c` 12v12 3개(`Unmatched MESSAGE`), `Both opponent's Pokemon give experience in battle against two opponents`(HnS 경험치 정책)
- 목록이 사전 분석 E의 스크래치 실행(`tmp-E/list-ABCE.txt`, A+B+C+E+HnS 제안 2건)과 바이트 단위로 같다.
- 새 기준 목록: [`test-baseline-seq138.5.txt`](test-baseline-seq138.5.txt)

## 한글 턴 종료 회귀 테스트 (seq 132 D, 저장소 밖)

- `ALLOW_REPO=1 /home/hjm0725/hns-sync-work/chunk-132/D-tests/run.sh <저장소> seq1385-post 6`: 테스트 4파일을 `test/battle/`에 복사·실행한 뒤 지웠다.
- 결과 **66 PASS / 4 FAIL**(`cc0576a543`, `6c15064bce`, `f3a58f9939` 세 번), 요약이 seq 132 이식 뒤 기대 요약(`baseline/expected-after-abc-summary.txt`)과 같다. FAIL 4개는 모두 이름에 `UPSTREAM…EXPECTED`가 붙은 seq 132의 의도된 변경(다이맥스 3, 매직룸 하양허브 1)이다. #8943 단위가 턴 종료 한글 출력 순서를 바꾸지 않았다.

## 커밋 리뷰 (병렬 5개, 읽기 전용)

리뷰는 저장소를 읽기만 하고 스크래치 사본(`/home/hjm0725/hns-sync-work/chunk-1385/review-{A,B,C,E,H}/`)에서 실측했다. 결과 전문은 각 폴더의 `REVIEW-RESULT.md`.

| 리뷰 | 대상 | 판정 | 요지 |
|---|---|---|---|
| A | `022e666847` 파티 코어·필드·세이브 경로 | 경미 | 76파일 중 74파일은 의미 변경이 upstream과 정확히 같고 2파일은 의도한 적응(`struct Trainer` 폭, `CalculatePlayerPartyCount`). 이식 전후 함수 13,828개 기계어 비교, 세이브 함수 명령어 단위 동일. **경미 1:** `GetBattlerPartyStateByPokemon`의 `gBattleStruct == NULL` 가드를 upstream대로 지워, 배틀 밖 독 기절 때 지가르데 퍼펙트폼·울트라네크로즈마의 종이 망가진다(실측 종 1664). HnS 맵·데이터에 두 종이 없어 사실상 도달 불가 → 리뷰 후 수정 R3 |
| B | `022e666847` 배틀 코어·한글 | 수정 필요 | upstream hunk·HnS 보존·한글 토큰 증명은 문제 없음(24파일 3-way 재병합 비교, 한글 3쌍 토큰 외 바이트 동일). **목호 멀티 파티 번호 충돌(실측):** 내 교체 대기 포켓몬에게 쓴 상처약·해독제·PP에이드가 같은 번호의 목호 현역 포켓몬에게 적용됨, 목호 포켓몬에게 쓴 도구가 효과 없이 소모됨, 내가 배운 기술이 목호 배틀 포켓몬에게 들어감, 레벨업 연출·체력 상자가 목호 쪽. 도달성 낮은 같은 부류 3곳(기사회생의기원, 미래예지 공격자 판정, 급소 횟수 진화) |
| C | `022e666847` 파티 메뉴·박스·프런티어·녹화·데이터·config·테스트 | 수정 필요 | 적용 일치(`part-C.patch`와 `diff -r` 동일), HnS 보존, 녹화 정적 배열이 upstream 힙 포인터 버그를 실제로 피함, `multi_do` 경로·FALSE 변화 사실 확인. **수정 필요 2:** 반 팀 멀티에서 목호 포켓몬(메뉴 1·4·5칸)에게 쓴 도구가 소모만 되고 효과 없음(메뉴의 결합 번호 3~5를 배틀 쪽이 플레이어 파티 빈칸으로 읽음), 한 기술용 PP 회복약의 기술 목록이 다른 포켓몬 것. 경미: FALSE 변화 보충 2건(아래 출력·동작 변화 1) |
| E | 후속 7커밋 | 7커밋 문제 없음 / 단위 잔여 수정 필요 1 | 각 커밋 diff가 `E-*.patch`와 같고 upstream hunk 누락·추가 0. #10536 HnS 고유 줄 보존, `>> 1` 보정의 upstream 범위 초과 주장 실측 확인(9,099 대 6,992), 경험치 문장 이름이 모든 형식에서 플레이어 파티 포켓몬. **수정 필요:** 레벨업 기술 습득이 목호 배틀 포켓몬에 들어감(B와 같은 건). 경미: 레벨업 연출, `battle_util.c` `>> 1` 줄 `// HnS:` 주석 누락 |
| H | HnS 수정 3커밋 | 3커밋 문제 없음 / 같은 결함군 수정 필요 1 | 강제 교체: 형식별 후보·결과가 이식 전과 같음(임시 테스트 12개, 녹화 통신 멀티 재생만 #8943 하위 층 원인으로 다름 — 기록). 참가 비트: 파트너 없는 더블 배틀러 2 유지 확인. 화이트아웃: 668개 조합에서 이식 전과 같음(#8943 판은 234개 다름), 풀 팀 189개는 #8943 식 그대로, `gSpecialVar_Result` 새 부작용 0. **수정 필요:** 레벨업 기술 습득(B·E와 같은 건) |

- 공통 원인: #8943 뒤 목호 파티가 `gParties[B_TRAINER_2]`의 0번부터라, "배틀러 2의 파티 번호가 플레이어 파티 번호와 같으면 같은 포켓몬"이라는 가정이 반 팀 멀티에서 깨진다(이식 전에는 목호가 3~5칸이라 겹치지 않음). upstream 1.17.0·master·upcoming 모두 같은 코드. 아래 "리뷰 후 HnS 수정"에서 고쳤다.
- 기록만 하는 것(리뷰 H 2·3·4, B·C 참고): 녹화 통신 멀티 재생에서 `BattleSideHasTwoTrainers`가 상대를 트레이너 1명으로 판정(#8943 하위 층, upstream 같음, 실기 불가), 프런티어 상대 #141 반 팀 판정(위 FALSE 변화), `NoAliveMonsForPlayer`의 `VAR_RESULT` 덮어쓰기(HnS 도달 없음, #10568에서 사라짐), 미러 챌린지에서 풀 팀 두 트레이너 동시 발견 배틀이면 플레이어가 A·B 각 3마리만 받음(FALSE 결정의 결과), `config/ai.h` 테스트 상한의 1.17.0 최종값(8/21/38/29/31)은 뒤 seq에서 맞춤.

## 리뷰 후 HnS 수정 (커밋 4개)

수정 담당이 리뷰 지적을 고쳤다(지시 `chunk-1385/FIX.md`·`FIX2.md`, 기록 `chunk-1385/fix/PROGRESS.md`). 네 커밋 모두 빌드 종료 코드 0, 새 경고 0, 한글 줄 변경 0. R1~R3은 관련 기존 테스트 35파일 330건, R4는 63파일 789건이 수정 전후 결과·실패 사유 줄이 같다. 수정 커밋은 다시 병렬 리뷰했고(R1+R3 → FR1, R2 → FR2), FR1이 찾은 같은 부류 3곳을 R4로 고친 뒤 R4와 상대 141 커밋을 FR3로 리뷰했다(아래 "수정 커밋 리뷰").

| 커밋 | 고친 것 | ROM | 확인(임시 테스트, 저장소에 넣었다 지움) |
|---|---|---:|---|
| `8bcf557c20` HnS: keep the in-game partner's party slots apart from the player's after 12v12 | 배틀러 2의 파티 번호를 플레이어 파티 번호와 비교하던 곳에 "같은 파티를 쓰는가"(`BattlersShareParty` 또는 `GetBattlerTrainer(B_BATTLER_2) == B_TRAINER_0`) 조건: 도구 대상 짝 판정 3곳(`BS_ItemRestoreHP`·`BS_ItemCureStatus`·`BS_ItemRestorePP`)과 기력의조각의 빈 짝 칸 재투입, 레벨업 기술 습득 2곳(`Cmd_handlelearnnewmove`·`Cmd_yesnoboxlearnmove`), `IsMonGettingExpSentOut`, `BS_UpdateChoiceMoveOnLvlUp`, 레벨업 연출·체력 상자 3줄(`battle_controller_player.c`), 기사회생의기원, `IsFutureSightAttackerInParty`, 급소 횟수 진화 조건, 치유방울·아로마테라피(`Cmd_healpartystatus`). `battle_util.c` `>> 1` 줄에 `// HnS:` 주석 | +368 B | 충돌 재현 테스트 11개 FAIL/INVALID → PASS(내 교체 대기 포켓몬에게 쓴 상처약·해독제·PP에이더가 목호 현역에게 가던 것, 기술 습득이 목호 배틀 포켓몬에 들어가던 것, 레벨업 연출이 목호 쪽에 나오던 것, 목호가 모두 쓰러진 뒤 기력의조각 assert, 기사회생의기원 assert, 치유방울이 목호 방음 때문에 내 대기 포켓몬을 치료하지 않던 것). 대조군(싱글·일반 더블·번호가 다른 경우) 전후 PASS |
| `624ef7d4bd` HnS: fix in-battle item use on the in-game partner's mons in half-team multis | 반 팀 멀티 파티 메뉴는 목호 포켓몬을 결합 번호 3~5로 넘긴다. 배틀 쪽이 이 번호를 플레이어 파티 빈칸으로 읽어 도구만 사라지던 것을, HnS 도우미 `GetItemTargetPartyOwner`(결합 번호 → 목호 파티 `번호−3`, 메뉴의 `CombinedToIndividualPartyId` 역변환, 조건은 `PARTY_LAYOUT_MULTI`와 같은 `IsMultiBattle() && !AreMultiPartiesFullTeams()`)·`GetItemTargetBattler`로 해석. HP·상태·PP 회복 도구 3개 경로 전부. 한 기술용 PP 회복약 기술 목록은 `GetPartyMonFromPartyMenuId(slot)` | −432 B | 목호 포켓몬에게 상처약(현역·대기)·해독제·PP에이더·PP에이드(한 기술)·기력의조각·목호가 모두 쓰러진 뒤 기력의조각(목호 칸 재투입) 7개 FAIL → PASS. 대조군 5개(멀티 내 포켓몬, 싱글·더블) 전후 PASS. X아이템은 이미 맞음(2개 PASS). `itemPartyIndex` 사용처 9곳 전수 확인(메뉴 "효과 없음" 사전 판정은 이미 실제 포켓몬을 봄, AI·녹화·가방은 변환 대상 아님) |
| `6c15064bce` HnS: restore the out-of-battle guard in GetBattlerPartyStateByPokemon | 리뷰 A 경미 1의 `gBattleStruct == NULL` 가드 복원 | +16 B | 필드 독으로 지가르데 퍼펙트폼 기절: 수정 전 종 1664로 손상 → 수정 뒤 이식 전과 같은 `form change target returned NONE` assert 뒤 복구(종 유지). 대조군 PASS |
| `f3a58f9939` HnS: keep Assist, AI party counts and Heal Bell apart from the in-game partner's party (R4, 리뷰 FR1 지적) | 조수 `GetAssistMove`의 자기 편 루프에 `BattlersShareParty`(내 교체 대기 포켓몬 대신 같은 번호의 목호 포켓몬 기술을 후보로 보던 것). AI `CountUsablePartyMons`·`AnyPartyMemberStatused`: 짝이 다른 파티면 짝 번호를 내 파티에서 빼지 않고 짝 상태는 짝 파티에서 읽음(두 트레이너·목호 멀티에서 남은 포켓몬 수가 이식 전과 같아짐). 치유방울·아로마테라피 `Cmd_healpartystatus`: 짝이 다른 파티면 짝 현역의 파티 상태도 `BtlController_EmitSetMonData`로 지움(교체했다 다시 나오면 상태가 되살아나던 것, 상태 아이콘도 함께 고쳐짐). `BS_ItemRestoreHP` 주석 2줄 | +176 B | 리뷰 FR1 테스트 3/7 → 7/7, 새 테스트 6/13 → 13/13(대조군 일반 더블 6개 전후 PASS). `playerStallMons`(PP 소모 방지 AI)는 가드만으로 이식 전 합산을 되살릴 수 없고 HnS에서 닿지 않아(해당 AI 플래그를 쓰는 멀티 트레이너 없음) 기록만 |

- R1~R3 뒤 빌드(`6c15064bce`): ROM 32,714,964 B, SHA1 `2f8213535d…`. R4 뒤 최종 빌드는 위 요약.
- **이식 전과 다르게 둔 곳(지금 구조에서 이식 전 동작이 불가능):**
  - 조수: 이식 전에는 목호(또는 상대 B) 파티 전체도 후보였다. 이제 자기 파티만 후보(upstream 구조).
  - 치유방울·아로마테라피(·반짝반짝스톰 계열 `Cmd_healpartystatus`): 사용자 파티와 짝 현역만 치료하고, **다른 트레이너의 교체 대기 포켓몬은 어느 방향으로도 치료하지 않는다**(내가 쓰면 목호 대기 포켓몬, 목호·프런티어 파트너가 쓰면 내 대기 포켓몬. 이식 전에는 공유 파티라 치료됨, 리뷰 FR3 실측). AI `AnyPartyMemberStatused`의 대기 포켓몬 범위도 같은 기준(자기 파티)이다. 인게임에서는 목호·아테나·조무래기23 파티에 이 기술들과 조수가 없어 플레이어가 쓸 때만 닿는다.
  - 기력의조각: 목호 3마리가 모두 쓰러진 뒤 **내 포켓몬**에게 기력의조각을 쓰면, 이식 전에는(공유 파티라) 그 포켓몬이 비어 있는 목호 자리로 나갔다. 지금 구조에서는 목호 배틀러가 내 파티를 쓸 수 없어 내보내지 않고 파티에서만 회복한다(수정 전에는 assert). 목호 포켓몬에게 쓰면 목호 자리로 다시 나온다.
- 범위 밖으로 기록만 한 것(upstream 1.17.0 그대로): side 기준 `itemLost[B_SIDE_PLAYER][...]`(목호 배틀러도 플레이어 0~2칸 표시를 건드림 — `B_RESTORE_HELD_BATTLE_ITEMS` GEN_LATEST라 배틀 뒤 복원에는 영향 없고 공생만 극히 드물게 다름), `activeGimmick[GetBattlerSide(...)]`(Z기술 불복종 해제의 side/trainer 혼용). 재확인 목록 22.
- 세이브: `save_compat.py run`을 `6c15064bce`·`f3a58f9939`에서 다시 돌려 둘 다 PASS(FAIL 0, WARN 3, 기대 보고서와 판정 줄 68개 같음). 바뀐 함수 중 세이브 경로는 없다.

## 친구에게 물을 것

1. `B_MULTI_HALF_TEAMS`: **친구가 (a) FALSE 유지로 확정했다**(`FRIEND_REPLY_2026-10-04.md` 2절). 6쌍 4마리와 배틀타워 멀티 UI 변화는 받아들이고 친구가 mGBA로 확인한다. 상대 141 반 팀 화면의 분리는 아래 "배틀타워 상대 141" 절.

## 실기 확인 항목 (친구용)

1. **로켓단 아지트 B2F 목호 멀티**(가장 중요)
   - 3마리 선택 화면 → 반 팀 쇼케이스 → 인트로 볼 표시(트레이너마다 한 줄 3칸) → 승리 시 아테나 퇴각 / 패배 시 화이트아웃 분기
   - **화이트아웃 시점**(`cc0576a543`): 플레이어가 3마리만 가진 상태, 4마리 이상 가진 상태 각각에서 목호 포켓몬이 모두 쓰러져도 내 포켓몬이 남아 있으면 계속 싸우는지
   - 경험치 분배: 한 번도 나가지 않은 내 포켓몬이 경험치를 받지 않는지(`c68e8e13ba`), 레벨업 때 목호 포켓몬 능력치가 그대로인지, 경험치 문장 이름(#10536)
   - **선두끼리 번호가 같을 때 내 포켓몬 레벨업**(`8bcf557c20`): 레벨업 연출·체력 상자가 내 쪽에 나오는지, 새 기술을 배울 때(특히 "기술을 잊게 한다") 목호 포켓몬의 기술이 바뀌지 않는지
   - **도구**(`8bcf557c20`·`624ef7d4bd`): 내 교체 대기 포켓몬에게 상처약·해독제·PP에이더를 쓰면 내 포켓몬에게만 듣는지. 목호 포켓몬(파티 화면 1·4·5번)에게 상처약·해독제·PP에이더·한 기술용 PP 회복약을 쓰면 목호 포켓몬에게 듣고 문장 이름이 맞는지, 한 기술용 PP 회복약의 기술 목록이 고른 포켓몬 것인지(내 2·3번째 포함). 목호가 모두 쓰러진 뒤 기력의조각을 목호 포켓몬에게 쓰면 목호 자리에 다시 나오는지, 내 포켓몬에게 쓰면 파티에서만 회복되는지
   - 상대 B(조무래기)·목호 쪽으로 울부짖기·날려버리기·드래곤테일이 정상 교체되는지(`70fe10ecd8`)
2. **알로라 섬 스크립트 더블 야생 7곳**(`AkalaIsle_hns`, `MelemeleIsle_hns`, `PoniIsle_hns`, `UlaUla_Forest_hns`): 두 마리가 나오는지, 둘째 포켓몬의 지닌 물건(#9729)
3. **두 트레이너 동시 발견 배틀:** 상대 B 포켓몬을 쓰러뜨렸을 때 경험치(상대 A를 먼저 쓰러뜨린 경우 포함, #10536 + HnS `>> 1`), 상대 B 포켓몬 도감 "본 적 있음"(#9811). FALSE 결정이 유지되면 위 6쌍의 4마리 트레이너가 4마리를 내는지(예: 46번 도로 TED·ERIN).
4. 일반 배틀 중 파티 → 순서를 바꾼 뒤 "강한 정도를 보다" 요약이 선택한 포켓몬과 같은지(#9843)
5. 미러 챌린지(두 트레이너·멀티), 사파리·벌레잡기대회, 파티 크기 제한 챌린지에서 포획·선물 포켓몬이 PC로 가는지, 키우미집 맡기기·찾기, 부화
6. **세이브:** 이식 전 빌드로 저장한 세이브를 이 빌드에서 불러와 파티·박스가 같은지. 프런티어 패스의 녹화 기록이 "없음"으로 나오는지, 새로 녹화 → 재생 → 재생 뒤 파티·세이브가 정상인지(HnS `sSavedParties` 정적 배열)
7. 배틀타워 멀티룸: 쇼케이스·전투 중 파티 메뉴 레이아웃(FALSE면 풀 팀 UI)
8. 통신(장비가 있을 때만): 금빛시티 포켓몬센터 싱글·더블 인트로·교체 이름, 상대 파티(#10102)

## 후속 행 메모

- **이미 적용(이 단위에서 선반영):** seq 140 #9725(#8943 커밋에 포함), 141 #9729, 164 #9811, 180 #9843, 215 #10102, 287 #10415, 307 #10536, 342 #10662. 닿으면 "이미 적용"으로 처리한다. 307은 HnS `>> 1` 보정 2곳이 upstream과 다르다.
- seq 170 #9751: TESTING 전용(AI 테스트의 반 팀 판정). 게임 동작 같음.
- **seq 171 #9799:** `sText_LinkTrainerSentOutPkmn` 토큰을 `{B_BUFF1}`로 바꾸고 인트로 전용 `sText_LinkTrainerIntroSendOutPkmn`(한글 본문은 기존 `…SentOutPkmn` 재사용)을 넣는다. #9799가 `sText_LinkTrainerSentOutPkmn2`를 MON2 이름으로 바꾸므로 HnS 교체 등장 왼쪽 우회(`battle_message.c` HnS 주석)도 함께 정리한다. AI `IsSwitchinValid` 파티 인덱스 충돌(두 트레이너·멀티에서 AI 교체를 가끔 거름)도 그때 풀린다.
- seq 204 #10051: `B_TRAINER_0..3` → `B_TRAINER_PLAYER` 등 이름 변경. upstream diff가 덮지 않는 HnS 줄(이번에 `gParties[B_TRAINER_*]`로 바꾼 약 160줄, 사전 분석 A `tmp-A/hns_only_converted.txt`, `CreateScriptedDoubleWildBossMon` 등)도 같이 바꿔야 한다. 이번 E-215·E-287의 `B_TRAINER_1/3` 적응도 그때 upstream 이름으로.
- seq 205 #9885: 옛 이름 deprecated 경고. `battle_hold_effects.c`에 매크로 옛 이름 1줄이 남아 있다.
- seq 207 #10059: 풀 팀 멀티 파티 메뉴 전용. HnS 반 팀 정책이면 도달하지 않지만 FALSE 결정 아래 배틀타워 멀티 UI(풀 팀)에는 닿을 수 있으니 그때 확인한다.
- **seq 209 #10039 → seq 315 #10568(+340 #10674):** 멀티 화이트아웃 판정 재작성. HnS 보호 수정 `cc0576a543`과 같은 함수다. 209에서는 최대값이 6+파트너 수로 바뀌어 중간 상태(다른 쓸 포켓몬이 없는 플레이어가 목호가 남은 동안 계속 진행)가 생길 수 있고, 315에서 `WillPlayerWhiteOutIfPartnerWinsAlone`으로 이식 전과 같은 결과로 돌아온다. 두 행 모두 HnS 보정을 다시 판단한다.
- seq 323 #10608: 수면 클로즈 멀티. HnS는 수면 클로즈가 꺼져 있다.
- **seq 348 #10711:** `AreMultiPartiesFullTeams`의 새 조기 반환(`B_MULTI_HALF_TEAMS` TRUE일 때, `gBattleTypeFlags`에 TRAINER가 없을 때)이 `gSpecialVar_Result`를 설정하지 않는다. `multi_do`는 배틀 **전**에 `callnative AreMultiPartiesFullTeams`를 부르므로, 직전 배틀이 야생이거나 이어하기 직후면 지난 값(`ChooseHalfPartyForBattle`이 남긴 `VAR_RESULT=1`)을 읽어 로켓단 아지트에서 3마리 선택을 건너뛰고, 전투 뒤 판정과 어긋난다. **그때 HnS 적응 필요**: 조기 반환에서도 `gSpecialVar_Result`를 세우고 트레이너 여부는 `TRAINER_BATTLE_PARAM.opponentA`로 본다.
- seq 400 #9440: `CreateNPCTrainerPartyFromTrainer` 셋째 인자 삭제(위 디버그 3마리 잘림 해소).
- 녹화 배틀: 섹터 여유 0 B. `struct Pokemon`·`BATTLER_RECORD_SIZE`·`MAX_BATTLE_TRAINERS`를 바꾸는 뒤 PR에서 `STATIC_ASSERT`가 멈추면 형식을 다시 정한다. `sSavedParties` 정적 배열은 upstream이 힙 초기화 문제를 고치면 다시 맞춘다.
- `struct Trainer` HnS 폭 5/4는 upstream 1.17.0(4/3)과 다른 HnS 차이로 계속 남는다. `struct Trainer`를 바꾸는 뒤 PR에서 다시 맞춘다.
- HnS 보호 수정 3개(강제 교체·참가 비트·화이트아웃)는 upstream 1.17.0·확인한 master에 같은 수정이 없다. 뒤 PR이 같은 함수를 고치면 HnS 줄을 정리한다.
- 같은 결함으로 남은 것(1.17.0 같음, 재확인 목록 22): 링크 비멀티의 `SetBattlePartyIds`가 컨트롤러 설정 전에 `GetBattlerParty`를 부른다(선두가 알·기절일 때만 차이), 녹화 통신 멀티 재생의 상대 B 파티 매핑, side 기준 `itemLost`·`activeGimmick`. 레벨업 연출·체력 상자 충돌은 리뷰 후 수정 `8bcf557c20`으로 고쳤다.
