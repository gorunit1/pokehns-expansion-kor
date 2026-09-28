# 작업 회신 — 2026-09-29 (full-sync 실제 port seq 1~62, 테스트 러너 복구)

[`HANDBACK_2026-09-29_FULL_SYNC_PORT_START.md`](HANDBACK_2026-09-29_FULL_SYNC_PORT_START.md)와 [`CLAUDE_FULL_SYNC_PORT_PROMPT.md`](CLAUDE_FULL_SYNC_PORT_PROMPT.md)를 받아 실제 port를 시작했다.

## 한눈에 보기

- 범위 확인: `git log --oneline 88d72d436e..origin/pokehns-expansion-kor`
- **port 진행:** `port_sequence.tsv` seq 1~62 완료, 중단 없음. **다음 시작은 seq 63**(#9066, L)이다.
  - 같은 unit의 회귀 수정 #10647(seq 329)은 #9942와 함께 넣었다. seq 329에 도달하면 "이미 적용"으로 처리한다.
- PR마다 커밋 1개. 결과 기록: [`results/1.17.0-port/full-sync-seq-001-062.md`](results/1.17.0-port/full-sync-seq-001-062.md)
- 한글 문자열이 들어간 소스 줄의 변경은 0이다. `battle_message.c`는 include 한 줄만 바뀌었다. config 기본값은 바꾸지 않았다.
- 빌드(`3a4a1f9fd5` 기준):
  - ROM 32,739,220 B (97.57%, 시작 대비 +3,952 B)
  - EWRAM 248,892 B (−220 B), IWRAM 25,516 B (−120 B)
  - 새 경고 없음. SHA1 `879f6333d0b32e9fa309551e5511f24173279292`

## 자동 테스트 복구 — 이제 쓸 수 있다

- seq 1 `733543267f`: `test/test_runner.c`의 include 순서를 고쳤다(upstream #9892 첫 hunk).
- 그 뒤에도 거의 모든 배틀 테스트가 `malloc.c:120` double-free assertion으로 실패했다. 원인은 두 가지였고 **모두 테스트 러너 쪽만 고쳤다(게임 ROM 불변)**. 상세는 [`results/1.17.0-port/TEST_RUNNER_FIX.md`](results/1.17.0-port/TEST_RUNNER_FIX.md)에 있다.
  - `7df90335e4`: HnS가 `gText_EmptyString3`를 `" "`로 바꿔 턴 시작 공백 프린터가 테스트 강제 종료 시점까지 남았다. 그 상태에서 다음 테스트가 힙을 초기화한 뒤 옛 포인터를 `Free`했다. 테스트 종료 시 `DeactivateAllTextPrinters()`를 부르도록 고쳤다. 실제 게임에서는 프린터가 끝날 시간이 있어 문제가 없다.
  - `a04eae0499`: 테스트용 세이브를 0으로 지우면 HnS의 One Type Challenge가 "TYPE_NONE만 허용"으로 켜진다. 그래서 포획 테스트에서 EWRAM이 덮어써지고 출력이 무한히 반복됐다. 테스트 세이브에서 이 챌린지를 끔(31)으로 설정했다.
- 전체 결과(`make check BUILD=hns -j6`, 약 5분):
  - **PASS 2,283 / FAIL 2,218 / TOTAL 5,175**
  - assertion 0, crash 0
  - 실패 대부분은 영문 `MESSAGE` 기대값과 HnS 한글 문자열의 불일치다.
- 앞으로는 이식 전후 비교로 회귀를 잡는다. 기준 목록: [`results/1.17.0-port/test-baseline-seq062.txt`](results/1.17.0-port/test-baseline-seq062.txt)

## HnS를 위해 upstream과 다르게 옮긴 곳 (seq 2~62)

- #9925: 반사 팔레트 태그 오프셋을 `0x3000`이 아니라 `0x3100`으로 두었다. HnS 서핑 포켓몬 팔레트 태그(0x3001~0x308B)와 겹치기 때문이다.
- #9231: `sTVSecretBaseSecretsRandomValues`는 static으로 유지했다. upstream처럼 지역 변수로 바꾸면 상태 사이에 값이 사라지는 회귀가 생긴다. 이 회귀는 1.17.0에도 그대로 있다.
- #7360: 효과음 판정만 1.17.0 최종형으로 바꿨다. 애니메이션 판정은 #10362까지 기존 함수를 유지한다.
- #9929: 명중 판정이 있는 변화기가 빗나갈 때 "그러나 실패하고 말았다!"(`STRINGID_BUTITFAILED`) 대신 "…에게는 맞지 않았다!"(`STRINGID_PKMNAVOIDEDATTACK`)가 나온다. 기존 한글 문장을 쓰며 출력 경로만 바뀐다. `BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 기록했다.

## 친구 mGBA 확인 요청 (seq 2~62)

1. #9929: 명중 판정 변화기(예: 최면술)가 빗나갈 때 문구
2. #9942/#10647: 교체된 시전자의 미래예지, 상성 무효 대상
3. #9176: 내던지기 전반(도구별 효과, 열매, 인분·비밀망토, 빗나감, 공생)
4. #9974: 냐오불·시마사리·메테노(코어·이로치) 색, 형사구스·따라큐 앞모습 애니메이션
5. #9925: 물가 반사 색, 서핑 중 높은 다리 위 NPC 반사
6. #9905·#8816: 포케기어 전화부·매치콜 화면, 이름 상자가 뜨고 사라지는지
7. #9883: Trainer Hill 안과 일반 맵에서 동반 포켓몬에게 말 걸기
8. #9855: 전기 계열 애니메이션과 HnS 추가 애니메이션에서 assert 크래시 화면이 나오지 않는지
9. #9882·#9912·#9973: 비-Gen4 UI에서 사파리 볼 수, 체력바·HP 숫자
10. #9207: 케이블카

## 다음 세션

- 작업 기기를 옮겨(노트북 또는 클라우드) 이어간다. 프롬프트와 규칙은 저장소에 있다.
  - [`results/1.17.0-port/NEW_SESSION_PROMPTS.md`](results/1.17.0-port/NEW_SESSION_PROMPTS.md)
  - [`results/1.17.0-port/PORT_INSTRUCTIONS.md`](results/1.17.0-port/PORT_INSTRUCTIONS.md)
- seq 63부터는 대형 리팩터(#9066, #9249, #8497, #9514, #9507 …)가 시작된다.
