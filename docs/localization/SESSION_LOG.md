# 세션 기록

오래된 기록은 이력으로 유지하고, 현재 상태는 STATUS.md에서 확인한다.

### 2026-10-04 — 친구 요청 A·B·C 수정, 계획서 열매 이름 정정 (데스크탑)

- 요청/범위: 사용자가 "추천대로" 진행을 지시했다. 순서는 A·B·C → seq 139이고, (1) 보고를 남긴 뒤 (2)까지 이어서 한다.
- 수정 파일
  - `src/battle_message.c`(A·B 각 1줄), `data/battle_scripts_1.s`(C 4줄)
  - 계획서 3곳: `pokeemerald-expansion-1.17.0-full-sync-plan.md:167`, `g2_battle_fixed_b_plan.md:122`, `g2_battle_fixed_b_plan.tsv:59`
  - 문서: `friend-requests-abc-2026-10-04.md`(새 파일), `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 3행, `popup-champions-compare.md`, `RECHECK_BEFORE_COMPLETION.md` 10b·10c, `HANDBACK_2026-10-04.md` 6절
- 커밋: `3464fa89b6`(A), `ef8c779b55`(B), `f79f3baf2b`(C). 적용 에이전트 1개 → 리뷰 1개(문제 없음) → 메인 재빌드·전체 테스트.
- 결정
  - A는 친구 제안대로 `{B_ATK_TEAM1}`을 썼다(종료 문장과 같은 토큰).
  - B는 친구가 지정한 공용 본문을 썼다.
  - C는 하양허브와 같은 NoFlush 팝업을 지닌 경우와 내던지기 모두에 넣었다. 내던지기는 친구 Q1에 따라 뺄 수 있다.
  - A·B는 원래 출력 변화 문서 범위 밖(문자열만 변경)이지만, 사용자 지시에 따라 기록했다.
- 검증
  - 빌드: `make hns -j8` 종료 0, ROM 32,715,172 B, SHA1 `b826980d2c2dda90f76c1bfe99d3482ee66c1557`, 로그 `build/localization-logs/hns-20261004-204651-abc.log`
  - 전체 테스트: `build/port-check-abc.log`가 기준 목록과 같다
  - 임시 한글 테스트: A 4개, B 2개가 수정 전 FAIL → 수정 뒤 PASS. C는 trace 12시나리오(리뷰에서 다시 실측)
  - 한글 회귀: `chunk-132/D-tests` 2-10 기대 문장을 갱신한 뒤 66/4
- 게임 화면 확인: 하지 않았다.
- 남은 문제: 같은 유형의 조사 문제 2곳(주술 시작, 팀 가드)은 친구 확인 대기다. 선택 팝업 patch는 친구 Q3~Q8 답을 받은 뒤 정한다.
- 다음 시작점: seq 139 #9714 적용(`/home/hjm0725/hns-sync-work/chunk-139/part-A.patch`, 회귀 `B-tests/run.sh`).

### 2026-10-04 — seq 138.5 #8943 단위 적용, 지닌 도구 팝업 대조, 친구 디스코드 답 반영 (데스크탑)

- **요청/범위:** 사용자 지시 (1) #8943 적용 + 같은 단위 후속 7개 + HnS 보호 수정, (2) 친구 요청 C 읽기 전용 분석(병렬). (3) A·B·C 코드 수정은 다음 단계. 작업 중 사용자가 친구 디스코드 답을 전달했다(`B_MULTI_HALF_TEAMS` (a) FALSE 확정, 상대 141 분리 검토, 인트로 주인공 Y 통일). 사용자는 데스크탑을 켜 둔 채 랩탑에서 원격으로 지시했다.
- **진행 방식:** 사전 분석(이전 세션, `chunk-1385/part-A~E`) → 적용 에이전트 1개 → 커밋 리뷰 5개(A·B·C·E·H) → 수정 에이전트(R1~R3) → 수정 커밋 리뷰 FR1·FR2 → 상대 141 에이전트 → R4 → 리뷰 FR3. 팝업 분석 P1(경로 전수)·P2(Champions 근거·멘탈허브 patch)·P3(통합). 메인은 재빌드·세이브 검증·전체 테스트·한글 회귀·문서를 맡았다. 에이전트는 저장소 밖 스크래치만 썼고, 임시 테스트는 저장소에 잠시 넣었다 지웠다.
- **커밋(코드):** `022e666847` #8943(+#9725), 후속 `4b437cd5da` #9729, `8e205d9e92` #9811, `7db72aa705` #9843, `4616fac998` #10102, `262bd0abd7` #10415, `93656e609b` #10536, `b09e11c9b5` #10662, HnS `70fe10ecd8`·`c68e8e13ba`·`cc0576a543`, 리뷰 후 HnS `8bcf557c20`·`624ef7d4bd`·`6c15064bce`·`f3a58f9939`, 친구 요청 `b43032bf03`(상대 141)·`5bbd01e69f`(인트로 Y).
- **결정과 이유**
  - `B_MULTI_HALF_TEAMS FALSE` + 로켓단 아지트 4명 Half(사용자 결정 → 친구 확정, HGSS도 각자 파티 사용).
  - `struct Trainer` 5/4, 녹화 시작 파티 정적 배열, #10536 `>> 1`, `sText_LinkTrainerSentOutPkmn` 유지(#9799 때): 사전 분석 권장 그대로.
  - 리뷰가 찾은 "목호 파티 번호가 플레이어 파티 번호와 겹침" 결함 묶음(upstream 1.17.0·master·upcoming 같음)은 이식 전 HnS 동작을 기준으로 HnS 보호 수정했다. 지금 구조에서 불가능한 이식 전 동작 3곳은 차이로 기록했다(목호 자리가 빈 뒤 내 포켓몬 기력의조각, 조수 후보, 치유방울의 목호 대기 포켓몬).
  - 상대 141: 배틀 자원이 있을 때만 시설 배틀 플래그를 믿는 조건(필드의 `multi_do`가 지난 플래그를 읽는 문제를 실측으로 기각).
- **검증(최종 코드 `f3a58f9939`)**
  - 빌드 `make hns -j8` 종료 0: ROM 32,715,172 B(−1,872) / EWRAM 250,128(+1,192) / IWRAM 25,516. SHA1 `4f87458453ed4d9f701debe458f1a038531ad2ef`, 새 경고 0. 로그 `build/localization-logs/hns-20261004-174522-post8943r4.log`
  - `python3 /home/hjm0725/hns-sync-work/chunk-1385/verify/save_compat.py run`: PASS(FAIL 0, WARN 3)
  - 세이브 왕복 `verify/savetest/run_all.sh`: PASS(`cc0576a543`에서)
  - 전체 테스트 `make check BUILD=hns -j6`: PASS 2,350 / TOTAL 5,271, 사라진 PASS 0. 목록이 세 번 모두 같다 → `test-baseline-seq138.5.txt`
  - 한글 회귀 `ALLOW_REPO=1 chunk-132/D-tests/run.sh`: 66/4
- **게임 화면 확인:** 하지 않았다(mGBA 없음). 친구 확인 목록은 `HANDBACK_2026-10-04.md` 5절이다(로켓단 아지트 멀티 위주).
- **문서:** 결과 문서 `full-sync-seq-138.5-138.5.md`, 팝업 대조 `popup-champions-compare.md`, 친구 회신 `HANDBACK_2026-10-04.md`(+README 링크), `FRIEND_REPLY_2026-10-04.md` 2절, `RECHECK_BEFORE_COMPLETION.md` 8b~8d·18~23, `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 2행, `test-baseline-seq138.5.txt`.
- **남은 문제**
  - 실기 확인
  - 친구 Champions 확인 답(Q1~Q11)
  - 계획서 열매 이름 오기 3곳(애슈·랑사로 정정할지)
  - #10711(seq 348) HnS 적응, #10039~#10568 화이트아웃 중간 상태
- **다음 세션 시작 프롬프트:**

```text
HnS 한글화 저장소에서 pokeemerald-expansion 1.17.0 full-sync 이식을 이어서 해.
1. git status --short --branch가 깨끗하면 git pull --ff-only origin pokehns-expansion-kor.
2. AGENTS.md, docs/localization/STATUS.md 맨 위 항목, docs/friend-handoff/HANDBACK_2026-10-04.md,
   docs/friend-handoff/results/1.17.0-port/popup-champions-compare.md,
   docs/friend-handoff/results/1.17.0-port/PORT_INSTRUCTIONS.md를 읽어.
3. 할 일: 친구 요청 A(순풍 시작 문장)·B(그래스필드 회복 문장)·C(멘탈허브 팝업,
   /home/hjm0725/hns-sync-work/chunk-popup/p2-mentalherb.patch; 친구가 Q1~Q11에 답했으면 선택 patch도)를
   HnS 커밋으로. 새 한글 문장 금지, 한글 임시 테스트로 문장·팝업 순서 확인, BATTLE_MESSAGE_OUTPUT_CHANGES.md 기록.
   그 뒤 seq 139 #9714.
4. 방식: 병렬 사전 분석 → 적용 에이전트 1개 → 커밋별 병렬 리뷰 → 메인이 한글 줄 검사·재빌드·테스트
   → 결과 문서·STATUS/SESSION_LOG·HANDBACK 갱신 → push. 에이전트는 스크래치 절대 경로만 쓰게 해.
```

### 2026-10-04 — #8943 사전 분석, 친구 mGBA 결과 수신 (데스크탑, 세션 종료)

- #8943 사전 분석 5개를 끝냈다(스크래치 `/home/hjm0725/hns-sync-work/chunk-1385/`, STATUS의 "#8943 사전 분석 완료" 항목).
  - A+B+C를 스크래치에서 빌드·테스트했다.
  - 세이브 호환을 실측했다.
  - 같은 단위 후속 7개와 HnS 보호 수정 후보 4개를 정했다.
- 친구 mGBA 결과(`60b32d674d`): 전부 정상. 새 요청 3건이 있다. 기록은 `docs/friend-handoff/results/1.17.0-port/mgba-check-seq127-138.md`다.
- 사용자에게 `B_MULTI_HALF_TEAMS`(TRUE 권장)와 진행 순서를 물었다. 답을 받기 전에 컨텍스트가 차서 세션을 넘긴다.
- **다음 세션 시작 프롬프트:**

```text
HnS 한글화 저장소에서 pokeemerald-expansion 1.17.0 full-sync 이식을 이어서 해.
1. git status --short --branch가 깨끗하면 git pull --ff-only origin pokehns-expansion-kor.
2. AGENTS.md, docs/localization/STATUS.md 맨 위 3개 항목,
   docs/friend-handoff/results/1.17.0-port/PORT_INSTRUCTIONS.md,
   docs/friend-handoff/results/1.17.0-port/mgba-check-seq127-138.md,
   docs/friend-handoff/results/1.17.0-port/RECHECK_BEFORE_COMPLETION.md를 읽어.
3. 스크래치 /home/hjm0725/hns-sync-work/chunk-1385/ 의 part-A~E.md(+patch, verify/)가
   seq 138.5 #8943 사전 분석 결과야. 이식 전 빌드는 chunk-1385/base/.
4. 할 일 (내 결정: B_MULTI_HALF_TEAMS = [TRUE/FALSE], 순서는 아래대로)
   (1) #8943 적용: part-A→B→C(+#9725 포함) 한 커밋, 이어서 같은 단위 후속 7개
       (#9729, #9811, #9843, #10102, #10415, #10536, #10662, E-*.patch)를 PR별 커밋,
       그 뒤 HnS 보호 수정(강제 교체 오른쪽 트레이너, 멀티 경험치 참가 비트,
       반 팀 멀티 화이트아웃, #10536 상대 B 경험치 인덱스)을 리뷰 거쳐 별도 커밋.
       빌드, 세이브 검증(chunk-1385/verify/save_compat.py run), 전체 테스트를
       test-baseline-seq138.txt와 비교(LC_ALL=C 추출).
   (2) 동시에 친구 요청 C(지닌 도구 팝업 전수 대조) 읽기 전용 분석을 병렬로.
   (3) 친구 요청 A·B 문장 수정과 C 팝업 수정을 HnS 커밋으로(한글 임시 테스트로 확인).
5. 방식: 병렬 사전 분석 → 적용 에이전트 1개 → 커밋별 병렬 리뷰 → 메인이 한글 줄 검사·재빌드·테스트
   → 결과 문서·STATUS/SESSION_LOG·HANDBACK 갱신 → push. 에이전트는 저장소에 파일을 만들지 말고
   스크래치 절대 경로(/home/hjm0725/hns-sync-work/)만 쓰게 해. 단위 하나가 끝나면 보고하고 멈춰.
   멀티 에이전트 병렬 오케스트레이션을 써.
```

### 2026-10-04 — 친구 답 반영 HnS 수정 3건, 재확인 목록 (데스크탑)

- 요청: 친구 답장(HANDBACK_2026-10-01 5~9절에 대한 답)을 사용자가 전달했다. 기록은 `docs/friend-handoff/FRIEND_REPLY_2026-10-04.md`다.
  - 질문 1·3·4는 고치고, 2는 기록만 한다.
  - #8943은 A안 그대로 진행한다.
- 커밋
  - `d78de7fdb5` 녹화 배틀 Gold/Kris(그림 비교 도구 표 포함): 메인이 직접 넣었다.
  - `d37911167c` 전자부유 종료 이름: 메인이 직접 넣었다. 턴 종료 한글 회귀 3-07 기대값을 갱신해 저장소에서 66/4를 확인했다.
  - `b07fd88953` 탈출 아이템 뒤 반사·가로채기 assert, 매직코트·가로채기 승계 제거: 분석 에이전트 1개 → 메인 적용 → 독립 리뷰 1개. 스크래치는 `/home/hjm0725/hns-sync-work/chunk-hnsfix-1004/`다.
- 결정: 배턴터치에서도 매직코트·가로채기를 지운다. 근거는 친구 결정과 upstream #10338이고, Showdown은 복사한다는 점을 기록했다.
- 발견(고치지 않음, 재확인 목록): 한글 조사 고정 2건(`STRINGID_HURLEDINTOTHEAIR` `{이름}는`, `STRINGID_CLOAKEDINAHARSHLIGHT` `{이름}로부터`), 탈출버튼으로 들어온 춤추기 포켓몬이 춤을 따라 추지 않음.
- 검증
  - 빌드: ROM +96 B. 새 경고 0, 한글 소스 줄 0.
  - 임시 테스트 16+8개 PASS(저장소에 잠시 두고 지움).
  - 전체 테스트: seq138 기준과 바이트 동일
  - 리뷰: 세 커밋 문제 없음. 문서·도구 정리 2건을 반영했다.
- 새 문서: `docs/friend-handoff/results/1.17.0-port/RECHECK_BEFORE_COMPLETION.md`, `full-sync-hnsfix-2026-10-04.md`
- 다음 시작점: **seq 138.5 #8943**(12v12, XL, 세이브 영향). 사용자 지시를 받은 뒤 진행한다.

### 2026-10-03 — full-sync port seq 135~138 (데스크탑)

- 요청: seq 135 #9709부터 진행한다. 작은 행이라 seq 136 #9711까지 한 구간으로 묶었다. seq 137은 선진행, 138은 예전에 이미 적용이다.
- 진행: PR별 병렬 사전 분석 2개 → 적용 1개 → 커밋별 병렬 리뷰 2개 → 메인 확인. 스크래치는 `/home/hjm0725/hns-sync-work/chunk-135/`다.
- 커밋
  - #9709 `a642657907`, #9711 `62d1876d6f`
  - 진행 기록 `f4f62e1182`·`dded2bcc34`, 결과 `68d3be50e5`, 시작 기록 `90ae8c6774`
- 결정과 확인
  - #9709 `config/ai.h` 테스트 hunk 제외: HnS 값 유지
  - #9711: 계획서의 "빌드 산출물 불변"이 틀렸다(비트필드 압축, ROM +32 B). 커밋 메시지에 이 점을 적었다.
  - 구조체 크기 144 B 그대로라 통신·세이브 영향 없음
- 검증
  - 메인 검사: 한글·모드·HnS 표시 변경 0. 재빌드 SHA1이 같다.
  - 빌드: ROM +48 B. 전체 테스트는 seq 132 기준과 바이트 동일.
  - 리뷰 2개: 모두 문제 없음
- 다음 시작점: **seq 138.5 #8943**(12v12, XL, 세이브 영향). 사용자 지시를 받은 뒤 진행한다.

### 2026-10-03 — full-sync port seq 132 #9680 (데스크탑)

- 요청: 순서표대로 seq 132 #9680(End Turn events use BattleScriptCall, XL, 한글 닿음)을 이식한다.
- 진행: 영역별 병렬 사전 분석 4개 → 적용 1개 → 병렬 리뷰 3개 → 메인 확인.
  - 분석 영역: A 엔진, B 스크립트, C 호출부, D 이식 전 한글 출력 순서 회귀 테스트 70개(저장소 밖)
  - 스크래치: `/home/hjm0725/hns-sync-work/chunk-132/`
- 커밋: #9680 `5f580ab12e`, 결과 문서 `00c8698be8`, 시작 기록 `18f412e9ff`
- 결정
  - HnS 스크립트는 본문을 두고 끝만 `return`으로 바꿨다(맹독구슬·화염구슬·아이스바디·치유의마음·스위트베일 하품·방벽 `*WoreOff`).
  - 하양허브 내던지기 분기를 유지하고, 안개제거 `DEFOG_CLEAR` hunk는 뺐다.
  - part-B 정리 결정 3건은 기본안이다.
  - 하양허브 강제 발동 경로 변화는 D4로 받아들였다.
- 검증
  - D 테스트: 저장소에 잠시 두고 돌린 뒤 지웠다. 이식 전 70/70, 이식 후 66/4(의도한 4개).
  - 빌드: ROM 32,716,900 B(+16 B), SHA1 `512ccdbe…`. 새 경고 0, 한글 줄 0.
  - 전체 테스트: PASS 2,344 / TOTAL 5,261. 사라진 PASS 0, FAIL→PASS 4.
  - 리뷰 3개: 수정 필요 0. 문서 정정 반영 — 미래예지 빗나감 실측, 기절 처리·아레나 문구, 불바다 질문을 seq 385 메모로, 하양허브 행의 "이전" 칸.
- 게임 화면 확인: 안 했다. 친구 확인 항목은 결과 문서에 있다.
- 다음 시작점: **seq 135 #9709**(S). 사용자 지시를 받은 뒤 진행한다.

### 2026-10-03 — full-sync port seq 129 #9674 unit (+ seq 274 #10386 선반영) (데스크탑)

- 요청: 순서표대로 seq 129 #9674(Magic Bounce / Magic Coat / Snatch refactor, L, 한글 닿음)를 이식한다.
- 진행: 영역별 병렬 사전 분석 3개 + #10386 분석 1개 → 적용 1개(커밋 2개) → 병렬 리뷰 3개 → 메인이 리뷰 수정 반영·확인.
  - 분석 영역: A 판정, B 스크립트, C 메시지·테스트, D #10386
  - 스크래치: `/home/hjm0725/hns-sync-work/chunk-129/`
- 커밋
  - #9674 `49415e3007`
  - #10386 `887c8e9ef8`(seq 274 선반영)
  - 결과 문서 `58af055d4c`
  - 리뷰 후 HnS 수정 `587f4e7cdc`
  - 시작 기록 `eb9c747212`
- 결정
  - #10386 선반영: 계획서 "리팩터 회귀 수정 후속 PR은 같은 단위로 바로 붙인다"를 따랐다. 선례는 #9494+#9864, #9594+#9796이다.
  - 가로채기 연출 `gEffectBattler` HnS 1줄: upstream 1.17.0의 잠재 버그를 보호한다.
  - 가로챈 멀리짖기 assert: upstream대로 두고 친구에게 묻는다. 단순 가드는 엉뚱한 포켓몬이 오를 수 있어 쓰지 않는다.
  - 되살린 막말내뱉기 테스트 2개: 넣는다. seq 181까지 알려진 FAIL이다.
  - 분함의발구르기 위력 2배 조건: upstream 동작으로 둔다.
  - 반사 순서를 스피드 순으로 바꾸는 후속 PR은 #9784로 바로잡았다.
- 리뷰 결과와 처리
  - 리뷰 D가 이식 전에는 정상이던 회귀 2건을 찾았다. 반사자의 다음 행동에서 방어 판정 누락, 더블에서 반사자 행동의 대상 변경이다.
  - 메인이 리뷰어 수정안을 넣어 `587f4e7cdc`로 고쳤다. 리뷰어 임시 테스트 16개는 저장소에 잠시 두고 PASS를 확인한 뒤 지웠다.
  - 리뷰 A가 찾은 탈출버튼·탈출팩 뒤 assert(upstream 같음)는 친구 질문으로 남겼다. 가로챈 쪽 대상 판정 중간 상태는 출력 변화 행으로 남겼다.
- 검증
  - 메인 검사: 한글 줄은 토큰 2개뿐이다. 모드·config 변경 0.
  - 빌드: ROM 32,716,884 B(+928 B), SHA1 `f453524c…`. 새 경고 0.
  - 전체 테스트: PASS 2,340 / TOTAL 5,261. 수정 뒤에도 `test-baseline-seq129.txt`와 바이트 동일, INVALID 21개 같음.
- 문서: `full-sync-seq-129-129.md`, `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 12행, STATUS, `HANDBACK_2026-10-01.md` 7절.
- 게임 화면 확인: 안 했다. 친구 확인 항목은 결과 문서 1~9다.
- 다음 시작점: **seq 132 #9680**(XL). 사용자 지시를 받은 뒤 진행한다.

### 2026-10-03 — full-sync port seq 128 #9475 (데스크탑)

- 요청: 순서표대로 seq 128 #9475(Refactor/trainer pic info, XL)를 이식한다.
- 진행: 영역별 병렬 사전 분석 4개 → 적용 1개 → 영역별 병렬 리뷰 3개 → 메인 확인.
  - 분석 영역: A 상수·표·도구, B 배틀 호출부, C 배틀 밖·세이브 호환, D 이식 전후 그림 데이터 비교 도구
  - 스크래치: `/home/hjm0725/hns-sync-work/chunk-128/`(이식 전 ELF `base/` 포함)
- 커밋: #9475 `06c6bac8c2`(35파일), 결과 문서 `0c3f45b29a`. 시작 기록 `ea90cbff3a`.
- 결정
  - ID 이름: upstream 새 이름을 쓴다. HnS 뒷모습 4개는 같은 인물 앞모습 ID에 합친다(목호 → `CHAMPION_LANCE_HNS`).
  - `GetPlayerTrainerPic`: `IS_HNS`이면 성별만 보고 Gold/Kris를 돌려준다.
  - 녹화 배틀 재생: Brendan/May를 유지한다(친구에게 질문).
  - 멀티 테스트 2곳: `GetPlayerTrainerPic(MALE, GAME_VERSION)`
  - Emerald·FRLG 기존 빌드 실패(`FLAG_DEFEATED_RED`): 기록만 한다.
- 검증
  - 메인 검사: 한글 줄·config·기존 파일 모드 변경 0. HnS 항목과 `GetPlayerTrainerPic` 분기를 확인했다.
  - 그림 데이터 비교: 차이 0. 리뷰 A가 소스·ROM으로 따로 224개를 대조해도 차이 0이었다.
  - 빌드: 종료 코드 0, ROM 32,715,956 B(+160 B), SHA1 `4b3b96bf…`. 새 경고 0.
  - 전체 테스트: `test-baseline-seq127.txt`와 바이트 동일. 새 기준 `test-baseline-seq128.txt`.
  - 리뷰 3개: 전부 문제 없음. 경미·정보 사항은 결과 문서에 반영했다(Emerald 추가 컴파일 대상, 볼 던지기 팔레트 설명, 머그샷 확인 대상, 닿지 않는 파트너 슬라이드 경로 등).
- 도구 보존: 그림 데이터 비교 도구와 이식 전후 덤프를 `docs/friend-handoff/results/1.17.0-port/trainerpic-9475/`에 넣었다. 새 구조끼리 비교용 `player_cases_new.tsv`를 더했다. README 명령으로 같은 ELF를 비교해 차이 0을 확인했다.
- 게임 화면 확인: 안 했다. 친구 확인 항목은 결과 문서 1~15다.
- 다음 시작점: **seq 129 #9674**(L, korean_touch Y). 사용자 지시를 받은 뒤 진행한다.

### 2026-10-03 — seq 127 친구 답 반영, HnS 보호 수정 2건 (데스크탑)

- 시작: 노트북 세션의 `e43d6fc997`(seq 127 완료)까지 fast-forward로 pull했다.
  - 데스크탑 `make hns -j8`: 종료 코드 0, ROM 32,715,764 B로 노트북 기록과 같다.
  - SHA1 `f12d5c8e…`는 노트북 `a6ad839c…`와 다르다. 툴체인 차이다.
- 요청: 친구 답(HANDBACK_2026-10-01 3절)을 사용자가 전달했다. 기록은 `docs/friend-handoff/FRIEND_REPLY_2026-10-03.md`다.
  - 2번(헤롱헤롱 문장)과 3번(대기 포켓몬 범위 밖 쓰기)은 HnS 보호 수정으로 넣는다.
  - 1번은 `몸저림` 유지, 4번은 기록만 한다.
- 수정 파일과 커밋(#9655와 분리)
  - `src/battle_util.c` `ItemHealMonVolatile`: `2cba47a010`
  - `src/battle_script_commands.c` `BS_ItemCureStatus`: `fdc110d528`
- 결정 이유: upstream 1.17.0에도 있는 기존 버그다. 친구 지시대로 최소 수정으로 고쳤다.
  - 헤롱헤롱만 풀 때만 문장을 바꾸고, 혼란·상태이상 우선 순서는 그대로 두었다.
  - 대기 포켓몬 경로에는 `targetBattler != MAX_BATTLERS_COUNT` 조건 1줄을 더했다.
- 검증
  - 빌드: 종료 코드 0, ROM 32,715,796 B(+32 B), SHA1 `817f500d…`. 새 경고 0, 한글 줄 0.
  - 임시 테스트 4개(`test/battle/item_effect/`에 잠시 두고 지움): 모두 PASS다. 헤롱헤롱 수정을 되돌리면 2개가 `Matched MESSAGE`로 FAIL한다.
  - 관련 5파일 110줄이 기준과 같다.
  - 전체 테스트: `test-baseline-seq127.txt`와 바이트 동일, 로그 `build/port-check-hnsfix127.log`.
- 문서: `full-sync-seq-127-127.md`("친구 답"·"HnS 보호 수정"·실기 확인 7번), `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 가방 도구 행, `HANDBACK_2026-10-01.md` 5절, STATUS.
- 게임 화면 확인: 안 했다. 친구 확인은 seq 127 실기 확인 7번이다.
- 다음 시작점: **seq 128 #9475**(XL). 사용자 지시를 받은 뒤 병렬 사전 분석 → 적용 → 리뷰 순서로 진행한다.

### 2026-10-01 — 친구 답장 반영, full-sync port seq 127 #9655 unit (노트북)

- 요청: 친구 답장(D1~D7 결정, 선진행 중단, MCP 시험 도입)을 기록하고 seq 127 #9655로 돌아가 진행한다. 노트북에서 진행하기로 사용자가 정했다.
- 시작: `bf35dce76a..254b12c226`(데스크탑 작업 47커밋)을 fast-forward pull했다. 친구가 올린 커밋은 없었다.
- 답장 기록(`fcf855d4e8`): `FRIEND_REPLY_2026-10-01.md` 새로 작성. 반영 위치는 HANDBACK_2026-09-30 9절, REPORT, seq 121~126 결정 대기, ahead README(구간 2 중단), #9006 문장 확정, MCP 제안 상태, README다.
- 이식 전 기준(노트북, `05319fd9b7`): `make hns -j8` 종료 코드 0, ROM 32,715,492 B(데스크탑과 같음). 전체 테스트 목록이 `test-baseline-seq167-ahead1.txt`와 바이트 동일(12분 20초).
- 진행
  - 읽기 전용 사전 분석 4개(A 문자열, B 스크립트, C C 코드, D 테스트·문서)
  - 메인이 patch 4개를 합쳐 적용 → 빌드 한 번에 통과 → 전체 테스트(13분 53초)
  - 병렬 리뷰 4개 → 지적 반영 → 재빌드
  - #9655·#10064 커밋을 나눴다. #9655 단독 빌드도 확인했다.
  - 스크래치는 `/home/jinmo/hns-sync-work/chunk-127/`이고, 문서 사본은 `results/1.17.0-port/seq127-9655/`에 있다.
- 커밋: #9655(+#9856 흡수) `f3b491dfc4`, #10064 `47515a949e`.
- 메인 결정
  - 힐볼 포획 `MULTISTRING` 저장·복원(분석 C 제안, 리뷰 R3 확인)과 `HealStatusConditions()` 범위 밖 읽기 방지(리뷰 R2·R3)를 `// HnS:` 보호 줄로 넣었다. 둘 다 upstream 1.17.0에도 있는 문제다.
  - 가방 도구 마비는 승인된 `new_sentences.tsv`대로 `몸저림`을 쓰고 친구에게 확인을 요청했다.
  - 레이징불 중복 테스트 삭제(upstream)와 iron_barbs TO_DO 유지(1.17.0 최종형)는 upstream을 따랐다.
- 검증
  - 빌드: 종료 코드 0, ROM 32,715,764 B, EWRAM 248,936 B, IWRAM 25,516 B, SHA1 `a6ad839c…`. 새 경고 0.
  - 한글 줄: 결정분 18줄뿐이다.
  - 테스트: PASS 2,335 / TOTAL 5,253. 사라진 PASS 4·새 PASS 7은 분석 D 예측과 같다. 새 FAIL 28은 관련 파일 15개를 따로 돌려 모두 영문 `MESSAGE`임을 확인했다. 보호 줄 반영 뒤 관련 테스트 3파일 32건을 다시 돌려 결과가 같았다.
  - 리뷰: 수정 필요 3건(중복 1)을 반영했다. 출력 변경 문서는 R4 지적 M1~M9를 고쳤다.
- 게임 화면 확인: 하지 않았다. 친구용 항목은 결과 문서 "실기 확인 항목" 1~14다.
- 노트북이 덮개를 닫아 약 35분 잠든 동안 작업이 멈췄다가 깨어난 뒤 이어서 진행됐다(손실 없음).
- 남은 문제: 친구 질문 3건(HANDBACK_2026-10-01 3절). MCP 시험은 데스크탑에서 진행할 예정이다.
- 다음 시작점: seq 128 #9475(XL 단독). `port_sequence.tsv` 128행과 g6 plan의 approach부터 읽는다.

### 2026-09-30 — #9655 대기 중 선진행 목록 작성, 선진행 구간 1 (데스크탑)

- 요청: seq 127 #9655가 D1~D7 결정 대기라서, #9655와 무관한 뒤쪽 행을 골라 목록으로 보여 주고 승인을 받아 구간 단위로 진행한다.
  - 조건 1: deps 사슬로 #9655에 의존하지 않는다.
  - 조건 2: #9655의 11개 파일을 건드리지 않는다.
  - 조건 3: 이미 적용된 행이 아니다.
- 시작
  - `77dc9d4b68`는 pull 결과 최신이었다.
  - `make hns -j8` 종료 코드 0, ROM 32,722,852 B(기록과 같음).
- 목록 작성(`docs/friend-handoff/results/1.17.0-port/ahead-of-9655/`, 커밋 `e629de8bdf`)
  - 방법: upstream 변경 파일로 조건을 거르고, 후보 hunk를 upstream `git blame`으로 추적했다. "앞 seq의 보류 행이 쓴 줄을 고치는지"를 보고 줄 겹침 행 41개와 거기 딸린 12개를 보류했다.
  - 결과: 397행 중 선진행 114, 보류 53, 나머지는 제외였다.
  - 사용자 결정: 114행으로 진행하고 pokemon.c 조건은 완화하지 않는다.
- 진행: 병렬 사전 분석 5개 → 메인이 patch 15개를 스크래치에서 쌓아 확인 → 적용 1개(14행) → 병렬 리뷰 4개 → 메인 확인. 스크래치는 `/home/hjm0725/hns-sync-work/chunk-ahead-130-167/`다.
- 커밋: #9575 `0f60183f1f`, #9462 `99388d6158`, #9006 `fc205e1a40`, #9903 `cf71e21e56`, #9713 `d057cee5c2`, #9721 `f3893a4cb4`, #9690 `aeab20beac`, #9461 `9f6c5b5c58`, #9755 `ce1fc01da9`, #9774 `adb22cd5f0`, #8628 `5381abba16`, #9765 `2cef59f506`, #9762 `8d46f0241b`, #9813 `4442f1a559`, 결과 문서 `c1f24e6121`.
- 메인 결정
  - #9713: B안(곡 이름 저장 안 함)으로 현재 화면을 유지한다. ROM −1.3 KB이고, 이름 복사 EWRAM 오버런 잠재 버그가 없어진다.
  - #9006: #10223 1줄 선반영, 가르침 최대 PP `// HnS:`, 새 문자열 2개 한글 초안(미결).
  - #9819: 보류하고 #10548 직전에 넣는다(파티 메뉴 중간 회귀 회피). **승인받은 114행 목록에서 1행을 뺀 것이므로 보고한다.**
- 검증
  - 메인 한글 줄 검사: 새 문자열 2줄뿐이다. 모드·config 변경, `// HnS:` 삭제는 0이다.
  - 재빌드: SHA1 `a1aa5ea8…`, ROM 32,715,492 B, EWRAM 248,936 B, IWRAM 25,516 B. 새 경고 0.
  - 전체 테스트: PASS 2,332 / TOTAL 5,241, 사라진 PASS 0. 기준 목록 `test-baseline-seq167-ahead1.txt`.
  - 리뷰: 수정 필요 0, 경미 1(디버그 메뉴 영문 선택지, 기록). 결과 문서 사실 오류 1건(ROM 사용률)을 고쳤다.
- 게임 화면 확인: 하지 않았다. 친구 mGBA 확인 항목은 결과 문서 "실기 확인 항목" 1~8이다.
- 다음 시작점: 사용자 지시가 있으면 선진행 구간 2(seq 186 #9890부터)를 진행한다. D1~D7 답이 오면 seq 127로 돌아간다.

### 2026-09-30 — full-sync port seq 121~126 (데스크탑)

- 진행: 병렬 사전 분석 5개 → 적용 1개 → 병렬 리뷰 3개(전부 문제 없음).
- 커밋
  - #9594+#9796 `0e6b9c22ec`
  - #9425 `2397ef4e08`
  - #9624 `5fa2bc3746`
  - #9667 `d77ed650ae`
  - #9616 `673240f6ae`
  - #9668: 이미 적용
  - 결과 문서: `12f07d0a29`
- HnS 적응
  - 얼루기 점 y=25를 유지했다.
  - `pokemart 0` assert 위치를 조정했다.
  - 기술 설명 HnS 문구 6개를 유지했다.
  - `B_UPROAR = GEN_4`로 두고 턴 끝 방음 HnS 줄을 유지했다.
  - `STRINGID_PKMNWOKEUPINUPROAR` ATK→EFF는 결정 대기(D7)로 두고 적용하지 않았다.
- 출력 변화: 지옥찌르기로 소란이 끝나는 동작(upstream). `BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 1행을 추가했다.
- 검증
  - 장식 `verify.sh` OK
  - 기술 문자열 `cmp` 0 differences
  - 얼루기 네이티브 비교 그림 차이 0
  - 메인: 한글 줄은 `TARGETWOKEUP` 토큰 1쌍뿐, config는 `B_UPROAR` 추가(`B_ABSORB_MESSAGE`는 주석만), 모드 변경 0
  - 빌드: ROM 32,722,852 B, EWRAM 248,944 B, IWRAM 25,516 B, SHA1 `197afe076fa94dccc2579157efc696d93ca3a63b`
  - 테스트: 사라진 PASS 0. 기준 목록은 `test-baseline-seq126.txt`다.
- 문서: `NON_NPC_TEXT_AUDIT.md`의 옛 경로 2개를 고쳤다. `HANDBACK_2026-09-30.md`에 D7을 추가하고 10절을 새로 썼다.
- 다음 시작점: **seq 127 (#9655)**. D1~D7 답을 받은 뒤, seq 127 직전 HEAD 기준으로 다시 분석해서 진행한다.

### 2026-09-30 — full-sync port seq 120 (#9657), #9655 한글 영향 사전 조사 (데스크탑)

- 시작: 노트북 세션의 `bf35dce76a`까지 pull했다(seq 108~119 완료). clean build ROM 32,718,964 B가 기록과 같았다. 이어서 `17aa03192f` 제안 문서를 커밋했다.
- 진행: #9657을 4개 영역으로 나눠 병렬 사전 분석했다. 산출물은 스크래치 `/home/hjm0725/hns-sync-work/chunk-120/`에만 있다. A~D patch를 한 커밋 `aa175914e9`로 적용했다. 결과 문서·기준 목록은 `c6e4a4e29a`다. 영역별 병렬 리뷰 4개는 전부 문제 없음이었다.
- HnS 보존: 메가솔, 불면·의기양양 문구, 관통드릴(`IsBattlerProtected(cv)`), 집단구타 챌린지, 참기 연출, `// HnS:` 전부. `BattleContext` 146곳을 모두 `DamageContext`/cv로 바꿨다.
- 출력 변화: 프리폴 무게가 대상의 헤비메탈·라이트메탈·가벼운돌을 반영하게 됐다(1.17.0 동작, `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 1행 추가).
- 검증:
  - 메인: `git diff --check` 통과. 한글 줄·모드·config 변경 0. 코드에 `BattleContext`와 `CANCELER_RESULT_BREAK`/`PAUSE`가 0개다.
  - 빌드: ROM 32,719,060 B(+96 B), EWRAM 248,940 B, IWRAM 25,516 B, SHA1 `2ef9b346a463da48dc2a7f41f12d58ca4a619b53`
  - 테스트: 지정 160파일은 이식 전후가 같다. 전체 목록은 seq119와 바이트 동일하다(`test-baseline-seq120.txt`). 관통드릴·프리폴 임시 테스트는 커밋하지 않았다.
- #9655 사전 조사(읽기 전용, `17aa03192f` 기준):
  - 결과: `docs/friend-handoff/results/1.17.0-port/pre-9655/`(REPORT.md와 tsv 3개)
  - 요약: hunk 112개, 게임 화면 문장 변화 18곳, 권장안이면 새 번역 0, 결정 필요 D1~D6
- 다음 시작점: **seq 121**. seq 127 #9655 전에 D1~D6 답이 필요하다(`HANDBACK_2026-09-30.md` 9절).

### 2026-09-30 — 제안 문서: AI가 mGBA MCP로 실기 확인 일부 대체 (검토 중, 확정 아님)

- 요청: 사용자가 mGBA를 조작하는 MCP 서버로 실기 확인의 일부(메모리 값, 특정 화면)를 Claude가 대신하는 방안을 검토하고, 다른 세션과 친구가 볼 수 있게 문서로 남기라고 했다. **확정이 아니라 검토 중인 제안**이다.
- 조사:
  - 후보 5개를 비교했다: mgba-live-mcp, mcp-mgba, mgba-mcp, pymgba-mcp, PokeMCP.
  - mgba-live-mcp 코드를 검토했다(커밋 `bacaf68`). 실행하는 명령은 `mgba-qt`뿐이고, 네트워크를 쓰지 않으며 포트도 열지 않는다. 파일은 `~/.mgba-live-mcp/runtime/`에 둔다.
  - 주의점: `run_lua`는 권한이 크다. PyPI 0.5.0과 GitHub 코드가 달라서 커밋 고정 설치를 권한다.
- 산출물: `docs/friend-handoff/PROPOSAL_AI_MGBA_CHECKS.md`, `docs/friend-handoff/README.md` "검토 중인 제안" 절. 소스 변경은 없고 설치도 하지 않았다.
- 다음: 친구와 합의(시험 여부, 범위, 확인용 세이브, 운영 규칙). 합의하면 첫 시험은 seq 107 #7305 세이브 호환 확인으로 한다.

### 2026-09-30 — full-sync port seq 108~119 (노트북, 16:50 마감)

- 진행: 병렬 사전 분석 4개(필드 그래픽 #9537·#9241·#9557 / 배틀 메시지 #9610·#9578·#9634 / 배틀 엔진 #9532·#9494 / AI #9587·#9596·#9630, 스크래치 `chunk-108-119/`) → 적용 에이전트 1개(14:20~16:01) → 병렬 리뷰 3개. 마감 때문에 리뷰는 적용 도중에 이미 커밋된 9개부터 커밋 기준(`git show`/`git archive`)으로 돌렸고, 마지막 3건은 메인이 직접 확인했다. 리뷰 판정은 전부 "문제 없음".
- 메인 판단:
  - 폴터가이스트+대타출동 도구 문장 미출력은 upstream대로 두었다(버그라고 단정할 근거가 없음). 사용자에게 한 줄로 되돌릴 수 있다고 알렸다.
  - #9532 방출 연출 1줄, #9494 원시 날씨 해제 2줄은 upstream 회귀를 막는 `// HnS:` 수정으로 넣었다(기존 원칙).
  - #9557은 변형 B(가드)로 넣었다. 계획서의 "HnS ROM 영향 없음" 전제가 틀렸다(그대로면 +110 KB).
  - 같은 unit의 #9668(seq 126)·#9864(seq 176)는 함께 넣고, #9784(seq 166)는 선행이 없어 미뤘다.
- 사고: AI 분석 에이전트가 objdump 출력을 상대 경로로 써서 저장소 루트에 빈 `*.dis` 파일 4개가 생겼다. 에이전트 확인 후 메인이 지웠다(0바이트, 추적 안 됨).
- 메인 검증:
  - docs 밖 한글 줄 변경: `battle_message.c`의 #9610 토큰 교체 2쌍, #9578·#9634 미사용 문장 삭제 1줄씩. 삭제된 STRINGID 참조 0, #9634 대체 한글 문장 존재 확인. #9630 +/− 줄이 upstream `c107917e6c`와 같음.
  - `rm -f pokehns.elf pokehns.gba` 뒤 `make hns -j8`: 종료 코드 0, ROM 32,718,964 B, EWRAM 248,940 B, IWRAM 25,516 B, SHA1 `d439ac3b54a464827093a1a0d80d33e74c83ea5e`.
  - 테스트 로그 목록 = `test-baseline-seq119.txt`. seq107 대비 PASS 손실 4건의 분류(이름 변경 2, upstream과 같은 KNOWN_FAILING 2)를 확인했다. `Blunder Policy … Dragon Darts`의 `KNOWN_FAILING`은 upstream 병합 `7ff83c6542`와 같고 seq 467 #9841에서 풀린다.
- 기타: mGBA를 조작하는 MCP 서버(mgba-live-mcp, mcp-mgba)를 조사했다. 데스크탑에 설치할 예정이며 설치 안내 프롬프트를 사용자에게 줬다(이 노트북에는 설치하지 않음).
- 다음 시작점: **seq 120 (#9657, XL 단독)**. 이 세션에서는 구간마다 멈추고 사용자 지시를 받은 뒤 시작한다.

### 2026-09-30 — full-sync port seq 107 (#7305 나무열매 개편, XL 단독, 노트북)

- 시작: 데스크탑에서 seq 92~106을 진행한 뒤 노트북으로 돌아왔다. pull(`7b4e3dba40..84fd460dc0`, 34커밋). 노트북 증분 빌드 결과 ROM 32,716,260 B / EWRAM 248,924 B / IWRAM 25,516 B로 데스크탑과 크기가 같았다(SHA1만 `e4508999…`로 툴체인 차이).
- 진행: 병렬 사전 분석 4개(a1 세이브 호환, a2 C 코드, a3 스크립트·데이터, a4 테스트·후속 PR, 스크래치 `chunk-107/`) → 적용 에이전트 1개 → 병렬 리뷰 3개(세이브·순서, HnS 동작·C, 스크립트·데이터·그래픽). 리뷰는 모두 "문제 없음".
- 메인 판단:
  - 순서 고정 `STATIC_ASSERT`를 넣었다. 확정 방침(열매 번호 36~65 유지)을 지키는 장치이고, 새 정책이 아니다.
  - 블렌더 NPC 열매 세트는 분석이 엇갈렸다. 적용 에이전트가 실제 코드로 432행을 비교해 upstream 회귀(30행 차이)를 확인했고, `// HnS:` 최소 수정으로 이식 전 동작을 유지했다.
  - 통신 번호 차이(구버전 ROM과 베리 크래시·도도리오), 무효 저장값 처리는 upstream대로 두고 기록만 했다.
- 커밋: `f2a0395e90`(#7305), 진행 기록 `20877875be`, 결과 `2b5ec2bb98`. 자동 push가 13:32에 이 세 커밋을 올렸다.
- 메인 검증:
  - docs 밖 한글(U+AC00~D7A3)이 든 변경 줄 0(비ASCII 6줄은 영문 설명의 "Pokémon" 들여쓰기).
  - `rm -f pokehns.elf pokehns.gba` 뒤 `make hns -j8`: 종료 코드 0, ROM 32,716,084 B, EWRAM 248,924 B, IWRAM 25,516 B, SHA1 `0c91520caa7dca02ada0115f9657501147c9927a`.
  - 이식 전후 전체 테스트 로그(`build/port-check-pre107.log`, `build/port-check-post107.log`)를 `LC_ALL=C`로 다시 추출해 이식 전 목록 = `test-baseline-seq106.txt`, 이식 후 목록 = `test-baseline-seq107.txt`를 확인했다(PASS 2,314 / FAIL 2,232 / TOTAL 5,211, assertion·Killed 0).
- 리뷰 뒤 메인 수정:
  - `berry-7305/layout.sh`: `TC`만 지정해도 `READELF`가 따라가게 했다. `/opt`가 아닌 경로로 실행해 종료 코드 0을 확인했다.
  - 결과 문서: 열매 이름을 `src/data/items.h`와 대조해 "수박열매"를 "슈박열매"로 고쳤다. 지명 띄어쓰기를 `region_map_sections.json`에 맞췄다("30번 도로", "130번 수로").
- 주의(도구): `perl`로 한글 리터럴을 검색할 때는 `-Mutf8 -CSD`를 함께 줘야 한다. 없으면 일치 0으로 조용히 끝난다.
- 다음 시작점: **seq 108 (#9537)**. 이 세션에서는 구간마다 멈추고 사용자 지시를 받은 뒤 시작한다.

### 2026-09-30 — full-sync port seq 103~106 (데스크탑, 사용자 부재 중 자율 진행)

- 진행: 병렬 사전 분석 3개 → 적용 1개 → 병렬 리뷰 2개(#9568, #9551) + 메인이 #8213 확인. 전부 문제 없음.
- 커밋:
  - #9568 `4bc61b3ffc`: AI 롤 config 6개를 MEDIAN으로 두었다. 아군 KO는 `.maximum`을 유지했고, Beat Up은 `// HnS:`로 수정했다.
  - #9551 `34aec8afd7`
  - #9579: 이미 적용
  - #8213 `1660ce2f81`
  - 결과 문서: `9294b1eb3b` `docs/friend-handoff/results/1.17.0-port/full-sync-seq-103-106.md`
- 검증:
  - 메인 clean rebuild: 종료 코드 0, ROM 32,716,260 B(97.50%, +1,392 B), EWRAM 248,924 B, IWRAM 25,516 B, SHA1 `61ad9a09375b9052880e60f32a0b5ed06a66432e`
  - 한글 변경 0, `git diff --check` 통과
  - 새 config는 AI_ROLL 6개(전부 MEDIAN)와 위협 교체 확률 2개다.
- 테스트(적용 에이전트 전체 실행): PASS 2,314 / FAIL 2,232 / TOTAL 5,211. seq101 대비 PASS 손실 0이고, 새 PASS는 8개다. 새 기준 목록은 `test-baseline-seq106.txt`다.
- 실기 확인 요청: `docs/friend-handoff/HANDBACK_2026-09-30.md` 5절에 있다.
- 다음 시작점: **seq 107 (#7305 gBerries 재구성, XL)**. 세이브 호환 주의: 기존 열매 번호 36~65 순서를 유지해야 한다(full-sync plan 4절).

### 2026-09-30 — full-sync port seq 102 (#9172 애니 스크립트 16진수 → 10진수) (데스크탑)

- 커밋: `b72fd0c63d`(코드), `fa7cd4ef9c`(결과 문서 `docs/friend-handoff/results/1.17.0-port/full-sync-seq-102-102.md`)
- 방식:
  - upstream `48bf615a67`의 토큰 쌍 318종에서 변환 규칙을 도출했다. `0x8000` 초과 16비트 값은 음수로 바꾸고, 주석 안의 16진수도 바꾼다. `@` 앞에는 탭을 넣고, 빠진 콤마 15곳을 보충하며, 예외 1곳은 수동으로 처리한다.
  - 스크립트를 upstream 부모 파일에 돌린 결과가 upstream 결과와 바이트 동일했다. 그다음 HnS 파일 전체(4,399줄, HnS 고유 줄 포함)에 적용했다.
- 검증:
  - 메인이 `data/battle_anim_scripts.s`를 다시 조립해 확인했다. ROM SHA1 `3613568d3329886ca25b4f54148bf2c5d267e275`가 **변환 전과 같다**. ROM 32,714,868 B, EWRAM·IWRAM 변화 없음.
  - 테스트 빌드용 오브젝트(`TESTING=1`)도 바이트 동일하므로 테스트 기준은 `test-baseline-seq101.txt` 그대로다(`all_anims.c` 재실행 결과도 같음).
  - 한글 변경 0. `git diff --check` 통과. KowtowCleave 한 줄은 space-before-tab 공백만 제거했다.
- 후속 결정 필요(나중에): seq 497 #9924는 hunk 7개가 거부된다. 뒤 순서인 #9986(seq 516) pret 병합의 애니 매크로(`76799d84eb`, `edc58c881f`)가 선행 조건이다. 그때 #9986의 애니 매크로 부분을 먼저 가져올지, 손으로 옮길지 정한다. 상세는 결과 문서에 있다.
- 다음 시작점: **seq 103**.

### 2026-09-30 — full-sync port seq 92~101, Safari UI 수정, 순서표 보정 (데스크탑)

- 기준: 노트북 세션의 `7b4e3dba40`(seq 91 완료)을 fast-forward pull했다. 이 PC의 clean build ROM 32,713,060 B가 기록과 같았고, 전체 테스트 목록이 `test-baseline-seq091.txt`와 바이트 동일했다.
- 순서표 보정(`1364cb18fb`):
  - 외부결정 6건을 `port_sequence.tsv`에 넣었다: #8943 138.5, #9920 186.5, #10454 385.5, #10461 434.5, #10144 471.5, #10151 475.5.
  - 40개 행의 `deps`를 복원했다.
  - #10299·#10310·#10282를 475.6~475.8로 옮겼다. 의존 위반은 0이다.
- port seq 92~101:
  - 진행: 병렬 사전 분석 7개 → 적용 에이전트 1개 → 병렬 커밋 리뷰 5개(전부 문제 없음)
  - 적용 7건: #9429 `838e441612`, #9542 `8c7978ad50`, #9525 `6c2c954693`, #9562 `cc2ce789c3`, #9124 `fac71f54b0`, #9548 `3c09c95b2a`, #9529 `bc3c30671a`
  - 이미 적용 3건: seq 93, 94, 98
  - upstream과 다른 점: #9124 교체 후보 HP 음수 방지 `// HnS:` 가드
  - 결과 문서: `docs/friend-handoff/results/1.17.0-port/full-sync-seq-092-101.md`
- Safari UI 수정(port와 무관, 친구 실기 요청):
  - `6fe11839e4` 어순 `30개 남음`
  - `ebb94a174d` 3세대 라벨이 상단 프레임을 흰색으로 덮던 문제
  - merge `4160fc31f0`
  - 보고서: `docs/friend-handoff/results/SAFARI_UI_FIX_2026-09-30.md`
- 친구 실기 결과: seq 1~62 전 항목 PASS. `docs/friend-handoff/results/1.17.0-port/mgba-check-seq001-062.md`에 정리했다.
- 최종 검증(`4160fc31f0` 이후, clean build):
  - 빌드: 종료 코드 0, ROM 32,714,868 B(97.50%), EWRAM 248,924 B, IWRAM 25,516 B, SHA1 `3613568d3329886ca25b4f54148bf2c5d267e275`
  - 테스트: PASS 2,306 / FAIL 2,232 / TO_DO 613 / TOTAL 5,203. `test-baseline-seq101.txt`와 바이트 동일
  - 한글 변경: 한글이 든 소스 변경 줄은 Safari 코드 주석 1줄뿐이다(문자열 변경 0)
- 참고: seq 91의 ROM SHA1(`cfdfb885…`, 노트북 ARM 공식 툴체인)과 이 PC의 SHA1(Ubuntu `gcc-arm-none-eabi` 13.2.1)은 다르지만 크기는 같다. 툴체인 빌드 차이로 본다.
- 다음 시작점: **seq 102 (#9172, XL 단독)**. 친구 mGBA 확인 대기: Safari 재확인 6항목, #9525 발버둥, #9124 교체 AI(`HANDBACK_2026-09-30.md`).

### 2026-09-29 — full-sync port 구간 4(seq 91 #9507 Species enum, XL 단독)

- 에이전트 1개가 약 67분 동안 진행했다(약 55만 토큰). 커밋 `b0a0fb9033`(#9507), `8de47b965d`(#9558, group plan상 같은 unit), 결과 `3f6ccb8a6e`. 148파일 중 113파일은 그대로 적용, 35파일은 `patch -F0` 뒤 거부 hunk 100개를 `enum Species` 치환만 수동으로 옮겼다.
- 메인 검증:
  - docs 밖 한글이 든 줄 변경 0(151파일, +2,709/−2,677).
  - `InitFieldMoveMonSprite` `u32` 유지 판단을 호출 경로로 확인했다. bit 31 플래그를 싣는 HnS 활성 경로는 `RockClimb_ShowMon`이고, `SurfFieldEffect_ShowMon`은 `!IS_HNS`로 막혀 있다.
  - `rm -f pokehns.elf pokehns.gba` 뒤 `make hns -j8`: 종료 코드 0, ROM 32,713,060 B, EWRAM 248,924 B, IWRAM 25,516 B, SHA1 `cfdfb88553e6c19e308d58099b7c681645bb9dfa`.
  - `test-baseline-seq091.txt`가 `seq090`과 바이트 동일하고, 구간 끝 로그의 합계(PASS 2,298 / FAIL 2,229 / TOTAL 5,197)도 같다.
- 사용자 질문에 따라 #9507을 여러 서브에이전트로 나누는 것을 검토했으나 하지 않았다. 나눌 수 있는 부분은 수동 치환 35~41파일뿐이고, 빌드·값 표·DWARF·테스트 검증은 순차로 한 번만 할 수 있어 시간 이득이 작다. 공유 빌드 폴더 충돌과 토큰 증가가 더 컸다.
- 다음 시작점: **seq 92 (#9429)**. 이 세션에서는 구간마다 멈추고 사용자 지시를 받은 뒤 시작한다.

### 2026-09-29 — full-sync port 구간 3(seq 84~90): 마무리 전 중단 후 재개·완료

- 에이전트 1개가 16:20쯤 시작해 17:37까지 7개 PR을 커밋했다(#9376·#9466·#9510·#9135·#9460·#9514·#9539). 커밋 범위 `79af946ddf..e1b1914846`.
- 17:40에 인터넷 연결이 끊길 수 있다는 사용자 알림을 받고 현재 커밋을 push했다. 뒤이어 사용자 지시로 에이전트와 자동 push 루프를 멈췄다. 에이전트는 구간 끝 전체 테스트 도중이었고, 남은 프로세스는 없다.
- 메인 검증(부분): docs 밖 한글 줄 변경은 `src/battle_message.c` 22줄이고 모두 B_ 토큰 교체만 다르다. 재빌드·전체 테스트·기준 목록 저장은 재개 때 한다(STATUS 맨 위 "재개 절차").
- 자동 push 루프 주의: 정지 신호 파일을 지운 뒤 새 루프를 띄우면 옛 루프가 신호를 보지 못하고 계속 돈다. 이번에 두 루프가 동시에 돌았다(둘 다 fast-forward 전용이라 피해 없음). 루프는 신호 파일 대신 작업 자체를 종료해서 멈춘다.
- 다음 시작점: 구간 3 마무리(전체 테스트 → `test-baseline-seq090.txt` → 결과 커밋) 뒤 **seq 91 (#9507)**.
- 재개(18:47): 작업 트리 clean, 친구 커밋 없음을 확인했다. 메인이 `make check BUILD=hns -j8`를 다시 돌렸다(약 8분). PASS 2,298 / FAIL 2,229 / TOTAL 5,197, assertion·Killed 0.
  - 처음 추출한 목록에서 23줄이 빠졌다. "~ fit on ~" 테스트 이름 줄에 붙은 한글 인코딩 바이트 때문에 UTF-8 로케일의 `grep`이 그 줄을 놓친 것이다. `LC_ALL=C`와 `grep -a`로 다시 추출하니 5,128행이 `test-baseline-seq083.txt`와 바이트 동일했다. `test-baseline-seq090.txt`로 저장했고, 추출 명령을 `PORT_INSTRUCTIONS.md`에 적었다.
  - 재링크: `make hns -j8` 종료 코드 0, ROM 32,712,612 B, EWRAM 248,924 B, IWRAM 25,516 B, SHA1 `9dcd5c1578cbf4a7b117e3681eb030478bb4742b`.
  - 결과 문서에 구간 요약·실기 확인 목록을 추가했다(`2ecd462ad9`). 실기 목록의 기술명은 `src/data/moves_info.h`의 한글 이름과 대조했다(집게덫·썬더프리즌·소란피기·불사르기·전광쌍격·시럽봄, 특성 기분파).
- 다음 시작점: **seq 91 (#9507 Species enum, XL 단독)**. 이 세션에서는 구간마다 멈추고 사용자 지시를 받은 뒤 시작한다.

### 2026-09-29 — full-sync port seq 83 (#8497 loadspritegfx 제거, XL 단독 구간)

- 에이전트 1개가 약 68분 동안 진행했다. 커밋: `f59f50ca17`(#8497), `5121b83c94`(#9595), `45b27b9bce`(#10345), `715c1b91fc`(#10589), 결과 문서 `d1ba480f41`·`b5d3881de3`. 뒤 세 PR은 group plan에 "#8497과 같은 unit으로 넣는다"로 적혀 있음을 메인이 확인했다.
- 메인 검증:
  - docs 밖 소스에서 한글이 든 줄 변경 0(28파일, +1,059/−2,462).
  - `rm -f pokehns.elf pokehns.gba` 뒤 `make hns -j8`: 종료 코드 0, ROM 32,712,548 B(97.49%), EWRAM 248,924 B(94.96%), IWRAM 25,516 B, SHA1 `09927f92473a555ef521349aee72e8367125452d`.
  - 테스트 목록 `test-baseline-seq082.txt` → `seq083.txt`: 추가 5줄(`Move Animations work 1`·`2` FAIL, `3`·`4`·`Tera Blast animations work` PASS)뿐이고 PASS 손실 0.
- 판단: `Move Animations work 1`·`2` FAIL은 HnS 도구 팝업 태스크가 테스트 배틀 종료 시점에 남는 테스트 환경 차이로 보고 회귀로 보지 않았다. 실제 게임에서 쪼아대기·내던지기·벌레먹음 뒤 도구 팝업이 정상적으로 사라지는지는 실기로 확인한다.
- 사용자 지시로 구간 3(seq 84~90) 시작 전 대기한다. 메인의 15분 주기 자동 push도 멈췄다.
- 다음 시작점: **seq 84 (#9376 `U-cleanup-9376`, M)**.

### 2026-09-29 — full-sync port seq 63~82 (노트북 WSL 첫 세션)

- 작업 환경 메모:
  - 노트북 WSL(Ubuntu, 8코어·메모리 7 GB). ARM 공식 툴체인 `/opt/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi`(13.2.1)를 쓴다. apt 패키지가 아니므로 PATH에 없고, 빌드·테스트 명령마다 `PATH=/opt/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin:$PATH`를 붙인다.
  - 호스트 gcc 15.2.0에서 tools/ 빌드(-Werror 포함)가 정상이다.
  - seq 62(`72f40563ad`) 전체 빌드: 종료 코드 0, ROM 32,739,220 B / EWRAM 248,892 B / IWRAM 25,516 B로 데스크탑과 같다. SHA1 `1008a1fa69a582c2a0279b6b5837ca3113df99cf`(데스크탑 `879f6333d0…`와 다른 것은 툴체인 차이).
  - seq 62 전체 테스트 목록이 `test-baseline-seq062.txt`와 완전히 같았다(PASS 2,283 / FAIL 2,218 / TOTAL 5,175). 그래서 별도 `-wsl` 기준 목록은 만들지 않았다.
  - push: 노트북은 `credential.helper=store`로 `gorunit1/pokehns-expansion-kor`에 직접 push할 수 있다. 클라우드 세션(claude.ai/code)은 gorunit1 계정에 Claude GitHub App이 설치돼 있지 않아 push가 403으로 막힌다. 친구가 앱을 설치하면 클라우드에서도 push할 수 있다.
  - 이 셸의 `grep`은 ugrep 래퍼 함수라 파일 인자를 주면 빈 결과가 나올 수 있다. 목록 비교에는 `command grep`을 쓴다.
- 진행 방식: 구간(가중치 S=1·M=3·L=8·XL=25, 합 40~60, XL 단독)을 에이전트 하나에 순차로 맡겼다. 사용량 한도 대비로 에이전트는 PR마다 결과 문서를 커밋하고, 메인은 15분마다 fetch 뒤 fast-forward일 때만 push한다. 병렬 진행은 의존성(배틀 핵심 파일 공유), 메모리 7 GB, 한도 소모를 이유로 하지 않았다.
- port: seq 63~82 20행 + unit 구성원 5행(seq 94·98·138·319·330). 커밋 `3cbed2e8f7`~`b42872eba2`. 상세는 `docs/friend-handoff/results/1.17.0-port/full-sync-seq-063-082.md`.
- 메인 검증:
  - docs 밖 소스에서 한글이 든 줄 변경: `challenge_menu.c` 1쌍, `contest.c` 3쌍, `strings.c` −38, `src/data/script_menu.h` −6/+51. 제거된 한글 리터럴 48종이 모두 그대로 다시 나타나고, 새 7종은 기존 `strings.c` 문구의 사본이다.
  - `rm -f pokehns.elf pokehns.gba` 뒤 `make hns -j8`: 종료 코드 0, ROM 32,717,172 B(97.50%), EWRAM 248,908 B(94.95%), IWRAM 25,516 B(77.87%), SHA1 `1d57bfdf33f1115161cbfcf34733123ea6dc64d1`.
  - 테스트 목록 차이(`command grep`으로 재확인): PASS→FAIL 2건과 새 FAIL 1건은 모두 #9451 이후 AI 더블 테스트이고, 원인은 `AI_REVERSE_BATTLER_LOGIC_ORDER_CHANCE 50`이다. 로직 회귀가 아니라 테스트 전제 문제로 판정했다. seq 88·131 이식 뒤 다시 확인한다.
- 실기: 하지 않았다. 확인 항목은 STATUS 맨 위와 결과 문서에 있다.
- 다음 시작점: **seq 83 (#8497 `U-anim-8497`, XL)**. 이어서 84~90, 91(#9507 XL) 순이다.

### 2026-09-29 — full-sync 실제 port seq 1~62, 테스트 러너 복구

- 기준: 친구 인수인계 `88d72d436e`(HANDBACK_2026-09-29_FULL_SYNC_PORT_START, CLAUDE_FULL_SYNC_PORT_PROMPT). 브랜치가 깨끗한 것을 확인하고 fast-forward pull했다.
- port:
  - seq 1 `733543267f`: 테스트 러너 include 순서
  - seq 2~62: 에이전트가 PR별 1커밋으로 이식했다(`d037d758c8`~`670f578e53`, 결과 문서 `6ede50ee77`). #10647(seq 329)은 #9942 unit과 함께 적용했다.
  - 상세 기록: `docs/friend-handoff/results/1.17.0-port/full-sync-seq-001-062.md`
- 메인 검증:
  - `git diff --check 733543267f..HEAD` 통과
  - docs 밖 소스에서 한글이 들어간 줄 변경 0
  - `battle_message.c`는 include 1줄만 변경
  - config 변경 없음
- 테스트 러너 힙 문제: 별도 worktree에서 원인을 찾아 `7df90335e4`·`a04eae0499`로 고쳤다(test/ 전용, ROM 불변). `3a4a1f9fd5`로 merge했다. 원인은 `results/1.17.0-port/TEST_RUNNER_FIX.md`에 있다.
- 빌드(`3a4a1f9fd5`, `GITHUB_ACTION=1 make hns -j8`): 종료 코드 0. ROM 32,739,220 B(97.57%), EWRAM 248,892 B, IWRAM 25,516 B, SHA1 `879f6333d0b32e9fa309551e5511f24173279292`.
- 전체 테스트(`make check BUILD=hns -j6`): PASS 2,283 / FAIL 2,218 / KNOWN_FAILING 8 / ASSUMPTIONS_FAILED 38 / TO_DO 622 / EXPECT_FAILING 6 / TOTAL 5,175. assertion 0, illegal opcode 0. 테스트별 기준 목록은 `results/1.17.0-port/test-baseline-seq062.txt`에 있다.
- 실기: 하지 않았다. 친구 확인 목록은 `docs/friend-handoff/HANDBACK_2026-09-29_PORT_PROGRESS.md`에 있다.
- 다음 시작점: **seq 63 (#9066, `U-enum-9066`, L)**. 새 세션에서는 `results/1.17.0-port/NEW_SESSION_PROMPTS.md`의 1번→2번 프롬프트로 재개한다.

### 2026-09-28 — 포케기어 헤더 실제 조각 좌표 및 상단 1px 정렬 보정

- 사용자 확인으로 올바른 원본 선택 영역을 위 `(16,6,51×2)`, 가운데 `(69,0,51×8)`, 아래 `(133,0,51×2)`로 재확정했다. 이전 작업의 가운데 `x=64`, 아래 `x=128` 판정과 위·아래 반전 기록은 잘못된 것이므로 이 항목으로 대체한다.
- 세 조각을 위→가운데→아래 순서로 패킹하고, 화면에서 위 획이 오른쪽으로 1px 밀린 현상을 없애기 위해 위 2행만 합성 위치를 1px 왼쪽으로 옮겼다. 아래 마지막 조각은 41번 타일에 두고 `header.bin`의 41번 참조를 복원해 검은 상자 없이 글자와 흰색 선을 모두 유지했다.
- `header.4bpp`·`header.bin.smolTM`·팔레트와 `build/hns/src/graphics.o`를 재생성했다. 변환된 4bpp/타일맵은 검증본과 바이트 일치하고 실제 화면 폭 240px의 구분선도 연속이다.
- 전체 HNS 빌드 종료 코드 0. ROM 32,723,652 B, EWRAM 249,112 B, IWRAM 25,644 B. 새 ROM을 mGBA에서 완전히 다시 열어 최종 화면을 확인해야 한다.

### 2026-09-28 — Gen4 배틀 메가진화 아이콘과 레벨 100 겹침 수정

- 현상: 메가진화 후 Gen4 배틀 체력박스에서 레벨 100의 레벨 표시가 메가 아이콘에 가려질 수 있었다.
- 원인/비교: SoulGold는 `sIndicatorPositions`를 기준으로 아이콘을 배치하고 레벨 자릿수에 따라 `UpdateIndicatorLevelData()`에서 아이콘을 보정한다. HNS의 Gen4 레벨 타일 복사 경로는 같은 보정을 사용하면서도 세 자리 레벨의 `xPos=0`에 다시 5px를 빼 unsigned underflow를 일으켰다.
- 수정: `src/battle_interface.c`에서 메가/기믹 아이콘이 있을 때 두 자리 이하 레벨만 텍스트를 5px 왼쪽으로 옮기고, 레벨 100은 텍스트를 더 이동하지 않도록 했다. 아이콘의 레벨 100 -4px 보정은 그대로 둔다.
- 검증: `build/hns/src/battle_interface.o` 컴파일과 `make hns -j8`가 종료 코드 0으로 통과했다. ROM 32,723,668 B, EWRAM 249,112 B, IWRAM 25,644 B. 빌드 경고는 기존 미사용 함수·변수 17건이며 `battle_interface.c` 새 경고는 없다. mGBA에서 메가진화 포켓몬의 레벨 1·10·99·100을 직접 확인하는 작업이 남았다.

### 2026-09-28 — 전체 엔진 동기화 1단계: 인벤토리 재확정·이식 계획

- 요청/범위: 새 지시서 기준으로 남은 upstream PR 594개를 4개 판정(이식/동등/무관/외부결정)으로 다시 확정하고 의존성 순서 이식 계획을 만든다. 읽기 전용이며 소스 변경은 없다.
- 방법:
  - 현재 브랜치 `e95679c545` 스냅샷에서 PR별 `git apply --check`/`-R --check`를 다시 실행했다.
  - 6개 영역을 에이전트로 병렬 판정했다. 결과는 `/tmp` 대신 `/home/hjm0725/hns-sync-work/`에 점진 저장해 세션이 끊겨도 남게 했다.
  - 산출물 전수 검증: 594/594 포함, 중복 0, 12열.
  - 의존성 위상 정렬(동률은 upstream 순서) 결과 순환 0.
- 결과: 이식 517(+테스트 러너 복구 1), 동등 56, 무관 15, 외부결정 6. 이식 예정의 크기 분포는 S 281 · M 167 · L 45 · XL 25다. 한글 경로 겹침 85건, 세이브 영향 5건(모두 호환 적응). ROM 증가 추정은 약 +182 KB이고 그중 #9211이 약 +170 KB다.
- 산출물:
  - `docs/friend-handoff/results/pokeemerald-expansion-1.17.0-full-sync-plan.md`
  - `docs/friend-handoff/results/1.17.0-sync-plan/` (그룹별 plan, `port_sequence.tsv`, `all_prs_master.tsv`)
  - `docs/friend-handoff/HANDBACK_2026-09-28.md`
- 발견:
  - `make check`는 `test/test_runner.c`의 include 순서 1줄 때문에 멈춘다(HnS `16ca5376eb`에서 발생).
  - HnS 회복 기술 애니메이션 누락 가능성(#10416 잔여)과 방어측 날씨 보정 차이(#9735)는 코드 추론만 했고 실기 확인이 필요하다.
- 검증: 코드 대조만 했다. 빌드·자동 테스트·실기는 해당 없음.
- 다음 시작점:
  - 결과 문서 5절의 외부결정을 받는다. #8943 녹화 배틀 형식 결정이 필요하다.
  - `port_sequence.tsv` 1번(테스트 러너 복구)부터 이식한다.
  - `7e7c38ab10` 이후 ROM의 실기 스모크 테스트를 한다.

### 2026-09-27 — 전체 엔진 동기화: ROM 여유 확보, 묶음 A 결정 반영

- 사용자 결정: 1·2단계 진행을 승인했고, 묶음 A 결정 3건은 권장대로 한다(#10529 기존 동작 유지, #10015 유지, #10344 레드카드·탈출버튼 미적용). push는 작업 후 판단해 진행하도록 위임받았다.
- ROM 여유 확보(`7e7c38ab10`):
  - LTO=0 빌드에도 `-ffunction-sections -fdata-sections`를 추가했다. `Makefile`의 LTO 분기이며 TEST 빌드는 제외했다.
  - 링커 스크립트가 이미 `KEEP(.text.header_*)`·`.text.consts`와 `*(.rodata*)` 같은 와일드카드를 쓰고 있어 섹션 배치가 동일하다.
  - ROM 33,327,220 → 32,734,692 B(-579 KB, 97.56%, 여유 약 820 KB). EWRAM 249,000 B, IWRAM 25,636 B. `.text` -49 KB, `.rodata` -540 KB.
  - 기존 빌드 트리는 `rm -rf build/hns` 후 다시 빌드해야 효과가 난다.
- #10529 후속(`cdd2d096fa`): 인접(거리 0) 트레이너는 `TrainerMoveToPlayer`를 건너뛴다. 그래서 `PlayerFaceApproachingTrainer`에서 트레이너의 돌아보기가 끝난 뒤 이동 타입을 고정하도록 이전 동작을 복원했다.
- 검증: `git diff --check` 통과. `GITHUB_ACTION=1 make hns -j8` 종료 코드 0. ROM 32,734,772 B(97.56%), SHA1 `3cf27146177a561532d59a4429c5724fa7024c0e`. 실기 검증은 하지 않았다. 실기 스모크 테스트가 필요하다: 타이틀, 새 게임, 기존 게임 내 저장 로드·저장, 배틀 1회, PC, 인접 트레이너 발견.
- 다음: 1단계 인벤토리 재확정(6개 그룹 병렬, 읽기 전용) 결과를 종합해 의존성 순서 이식 계획을 만든다.

### 2026-09-27 — 1.17.0 전체 엔진 동기화 0단계 (시작 상태·묶음 A 재검증)

- 기준: `pokehns-expansion-kor` `84835a32d2`(친구의 FULL_ENGINE_SYNC 지시서 커밋)를 fast-forward로 받았다. 작업 트리는 깨끗했다. upstream merge-base는 `3efb836f72`, 목표 태그는 `expansion/1.17.0`(`e8bd1cd7b0`)이다.
- 묶음 A: `496bca3475..HEAD`에 `Port upstream #` 커밋 35개가 모두 존재한다.
- TSV 정리: `results/1.17.0-inventory/all_prs_checked.tsv`·`already_in_history.tsv`의 빈 칸을 `-`로 채워 후행 탭을 없앴다. 모든 행이 14열을 유지하며 판정 값은 바꾸지 않았다. `git diff --check 496bca3475`가 통과한다.
- 기준 빌드: `GITHUB_ACTION=1 make hns -j8`(이 환경의 GNU Make 4.3은 `--jobserver-style`을 지원하지 않는다) 종료 코드 0. ROM 33,327,220 B(99.32%), EWRAM 249,016 B, IWRAM 25,680 B, SHA1 `ac6df9a5d50a32edea8dd5a8e4e34d1d5332b18d`.
- 수치 차이 해석: 친구 환경의 ROM 33,330,740 B·IWRAM 25,704 B와 다르다. `GITHUB_ACTION`은 `check_history.sh`만 건너뛰므로 원인이 아니다. 이 환경은 Ubuntu `gcc-arm-none-eabi` 13.2.1과 newlib 4.4.0이므로 툴체인 차이로 본다. 같은 환경에서는 수치가 재현된다.
- 다음: 1단계 인벤토리 재확정(4개 판정으로 재분류하고 의존성 순서로 계획) 및 ROM 여유 확보 방안.

### 2026-09-28 — 포케기어 헤더 위·아래 띠 정정 및 주인공 이름 제한 복원

- 사용자가 계속 깨진 상단 글자를 보고해 이전 51×12 패킹을 원본 타일 시트와 다시 대조했다. 이전 적용은 위 2px에 `(16,6)`, 아래 2px에 `(128,0)`을 사용해 두 띠를 역순으로 넣은 것이 원인이었다. 실제 선택 영역의 내용은 위 `(128,0,51,2)`, 가운데 `(64,0,51,8)`, 아래 `(16,6,51,2)`이며, 런타임 타일맵의 화면 위치는 각각 `y=6..7`, `8..15`, `16..17`이다.
- `graphics/pokenav/hns/header.4bpp`, `header.png`, `header.bin`을 다시 생성하고 `header.4bpp.smol`·`header.bin.smolTM`을 갱신했다. 제목 타일에 기존 20번 구분선 타일을 덮어쓰지 않도록 미사용 41번 타일을 추가 슬롯으로 사용했다. `gbagfx` 역렌더 결과의 `x=16..66,y=6..18` 51×12 영역은 원본 세 띠를 위→가운데→아래로 이어 붙인 결과와 612픽셀 전부 일치했고, PNG를 다시 4bpp로 변환한 결과도 원본과 일치했다.
- `src/naming_screen.c:2607`의 주인공 작명 템플릿 `.maxChars`를 6에서 3으로 변경했다. `PLAYER_NAME_LENGTH=7`은 저장/링크 버퍼 상수라 유지했으며, 박스·포켓몬 닉네임·라이벌 입력 템플릿의 6은 별도 동작이므로 유지했다.
- 이력 조사: 현재 6은 `361f1e4a77`(2026-09-02 23:24:31 +0900)의 한글 패치에서 네 템플릿에 도입됐다. `1821fd6749`(2026-09-26 03:26:03 +0900)의 전체 worktree 업로드에는 maxChars 변경이 없었고, 모든 refs의 maxChars 3 검색 및 현재 인수인계 문서 검색에서도 추적 가능한 3글자 커밋/기록은 발견되지 않았다. 따라서 예전 3글자가 미커밋 작업이었다면 업로드 전후 유실 시점만 추정할 수 있으며, 별도의 유사 회귀 코드는 확인되지 않았다.
- 검증: `build/hns/src/graphics.o`를 강제 재생성한 뒤 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8`가 `BUILD_EXIT=0`으로 완료됐다. `pokehns.gba`는 33,554,432바이트, SHA-1 `0c3b4c596c198ac1cd6e8eb314625867e6774f4a`다. mGBA 실제 화면 검증은 아직 하지 않았으므로, 기존 ROM을 완전히 닫고 새 ROM을 다시 불러와 상단 헤더와 주인공 이름 입력 제한을 확인해야 한다.

### 2026-09-26 — 1.17.0 묶음 A 이식 (현존 버그 수정 35개)

- 요청/범위: 사용자가 결과 보고서 9절 권장안을 승인했다(선별 이식, 충돌 미이식, config 유지, LTO는 이후 검토). 묶음 A를 배틀 엔진과 AI·필드·일반 두 worktree로 나눠 PR 1개 단위로 이식했다.
- 결과: 35개 PR 적용, 보류 0. 5개는 부분 적용이다. 메인 브랜치에 fast-forward와 cherry-pick으로 충돌 없이 합쳤다. PR별 기록은 `docs/friend-handoff/results/1.17.0-port/`에 있다.
- 보존: 한글 문자열·STRINGID·`{B_...}`·배틀 메시지 순서·config 기본값·`test/**` 변경 없음(diff 전수 확인).
- 검증: PR마다 `git diff --check`와 `make hns -j6`가 성공했다. 통합 `make hns -j12` 성공. ROM 33,327,220 B(+736 B, 99.32%), EWRAM·IWRAM 변화 없음, SHA1 `ac6df9a5d50a32edea8dd5a8e4e34d1d5332b18d`. 변경 파일에서 새 경고 없음. 자동 테스트·실기 검증은 하지 않았다.
- 사용자 결정 대기: #10529 거리 1 트레이너 방향 고정 유지(3줄), #10015 취소 경로 추가 수정 유지, #10344 레드카드·탈출버튼 방어 조건 미적용. 모두 결과 보고서 "묶음 A 사용자 결정 대기"에 있다.
- 다음 시작점: 위 결정 반영 → 묶음 B(1.15.2·1.15.3 master 수정, g1 보고서 25건).

### 2026-09-26 — pokeemerald-expansion 1.17.0 업데이트 1단계 인벤토리

- 요청/범위: `docs/friend-handoff/POKEEMERALD_EXPANSION_1.17.0_UPDATE.md`의 사전 상태 기록과 upstream 변경 인벤토리. 소스 코드는 변경하지 않았다.
- 방법: upstream을 HnS와 별도로 clone하고, HnS 브랜치를 fetch해 계보를 비교했다. merge-base는 `3efb836f72`(#9875)이며, HnS는 master 1.15.2 개발 계열이고 upcoming 리팩터는 없다. changelog 1.15.2~1.17.0의 PR 694개 가운데 계보 포함 65개를 제외한 629개에 대해 분리된 스냅샷에서 `git apply --check`/`-R --check`를 실행했다. 그 뒤 6개 영역으로 나눠 HnS 코드와 대조했다.
- 결과: 이미 적용 73, 부분 적용 17, 이식 가능 225, 선행 필요 105, 충돌 39, 무관 170. 상세는 `docs/friend-handoff/results/pokeemerald-expansion-1.17.0-update-report.md`와 `docs/friend-handoff/results/1.17.0-inventory/`에 있다.
- 주요 발견:
  - 현재 HnS 버그를 코드로 확인했다. 불요의검·불굴의방패 `== GEN_9` 비교(`src/battle_util.c:3497`, `:3511`), AI `GetMovePower(playerMove != 0)`(`src/battle_ai_main.c:679`), 앵콜 검사 뒤 세미콜론(`src/battle_ai_util.c:5202`).
  - #10429는 `0x3A`가 한글 바이트와 겹쳐 적용 금지다.
  - `make hns`는 `LTO ?= 0`이다.
- 기록 정정: #10426은 구조 미적용이다. "#10561 아이콘"은 실제로는 #10346 일부다. #10124는 코드에 적용되어 있다.
- 검증: 코드 대조만 했다. 빌드·자동 테스트·실기 검증은 하지 않았다.
- 다음 시작점: 결과 보고서 9절의 사용자 결정(이식 범위, 충돌 처리, config, LTO)을 받은 뒤 4절 묶음 A부터 PR 1개 단위로 이식한다.

### 2026-09-26 — 새 clone 빌드 실패(누락 자산·폰트 규칙) 복구

- 요청/범위: 친구 인수인계 후 새 clone(`496bca3475`)에서 1.17.0 작업 전 기준 빌드를 확인했다.
- 원인: 한글화 브랜치의 `src/graphics.c`가 참조하는 `graphics/naming_screen/rwindow.png`·`roptions.png`·`page_button.png`가 커밋되지 않았다. 또 `data/fonts.s`·`src/text.c`가 읽는 `font{0,1,2,7,8}.latfont`, `font{0,1,2,7,8}_korean.latfont`, `font{0,1,9}.hwjpnfont`, `font2.fwjpnfont`, `unused_frlg_{male,female}.fwjpnfont`의 생성 규칙이 `graphics_file_rules.mk`에 없었다. 생성물은 `.gitignore` 대상이라 기존 로컬 작업 트리에서만 빌드가 성공했던 것으로 보인다.
- 수정: 세 PNG를 `poketony/pokeemerald-kr`(원 출처)에서 그대로 추가했다. `graphics_file_rules.mk`에 pokeemerald-kr과 같은 16개 폰트 규칙(`$(GFX) $< $@`)을 추가했다. 기존 폰트 PNG·한글 문자열·소스 코드는 변경하지 않았다.
- 검증: 생성 폰트·`.4bpp`와 `fonts.o`·`text.o`·`graphics.o`를 지운 뒤 `make hns -j12` 종료 코드 0. 새 규칙으로 16개 폰트와 3개 `.4bpp`가 모두 재생성됐다. 수동 생성본으로 빌드한 ROM과 SHA1 `7f3f85c7dce5402d388abf369c3e60574b854973`이 같다. 메모리: ROM 33,326,484 B/32 MB(99.32%), EWRAM 94.99%, IWRAM 78.37%. 실기 화면 검증은 하지 않았다.
- 주의: 원작업자가 세 PNG를 로컬에서 따로 수정했다면 이름 입력 화면이 다를 수 있다. 확인되면 원작업자 파일로 교체한다.
- 다음 시작점: `docs/friend-handoff/POKEEMERALD_EXPANSION_1.17.0_UPDATE.md`의 1단계 upstream 인벤토리. ROM 여유가 약 223 KB뿐이므로 이식 항목마다 용량을 기록한다.

### 2026-09-26 — `pokemon_storage_system.c` HGSS 기준 번역 검증

- 요청/범위: 현재 한글화된 PC 보관 시스템의 모든 런타임 문자열을 HGSS 공식 한국어 원문과 원래 영문 의미·실제 메뉴 호출부에 대조하고, 더 정확한 공식 표현만 보고한다.
- 수정 파일: `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`만 수정했다. `src/pokemon_storage_system.c`의 기존 한글 문장은 변경하지 않았다.
- 결과: 메인 PC 메뉴·설명과 대부분의 메시지, 벽지명 숲~하늘·포켓센·심플은 HGSS와 일치한다. `gPCText_Give`의 영문 잔존, `MSG_ITEM_IS_HELD`의 의미 역전, `MSG_CHANGED_TO_ITEM`의 `와/과` 조사 오용, `MENU_MACHINE`의 `기계`, `MSG_SURPRISE`의 줄임표 표기는 명확한 수정 후보로 확인했다. 선택/방출/파티에서 꺼낼 대상 메시지는 HGSS와 직접 같은 문구를 따로 기록했다.
- 보류: `POLKA-DOT`, `SCENERY 1`~`3`, `ETCETERA`, Walda 전용 `FRIENDS` 벽지 그룹은 HGSS에 동등 항목이 없어 공식 번역을 단정하지 않았다. `MENU_SELECT`는 HGSS 이후에 추가된 ChooseBoxMon 기능이라 기존 권장 `선택`을 유지한다.
- 출처: [Poké Corpus HGSS 한국어 Text File 24·25](https://raw.githubusercontent.com/abcboy101/poke-corpus/main/corpus/HeartGoldSoulSilver/ko_msg.txt), 확인일 2026-09-26.
- 검증: `src/pokemon_storage_system.c`의 `sMessages`, `sMenuTexts`, `Task_GiveMovingItemToMon`·`Task_GiveItemFromBag`·`Task_SwitchSelectedItem` 및 벽지 메뉴 분기를 정적으로 대조했다. 이번 작업은 문서/검증만이므로 빌드·실기 테스트는 하지 않았다.
- 다음 시작점: 사용자가 승인한 문자열만 별도 패치하고 HNS ROM에서 PC의 도구 주기/교체, 벽지 선택 메뉴를 확인한다.

### 2026-09-26 — `MSG_CHANGED_TO_ITEM` HGSS 직접 근거 정정

- 정정: 앞선 감사에서 `MSG_CHANGED_TO_ITEM`의 `"{DYNAMIC 0}{K_EU}로 바꾸었다!"`를 제안했지만, 이는 원래 영문 `Changed to {DYNAMIC 0}.`과 한국어 조사 규칙을 결합한 의미상 제안이다. HGSS PC 원문에는 이와 같은 도구 교체 완료 문장이 없다.
- HGSS에서 확인되는 가까운 PC 도구 문구는 `[도구] 가져왔다!`, `[도구] 지니게 했다!`뿐이며, 도구 교체 문구는 배틀의 `서로의 도구를 교체했다!`처럼 다른 상황이다. 따라서 HNS의 교체 완료 문구를 HGSS 공식 원문이라고 단정할 수 없다.
- 조치: 소스 문자열은 변경하지 않았다. STATUS의 해당 표현도 HGSS 직접 근거와 의미상 권장을 구분하도록 정정했다.

### 2026-09-26 — `pokemon_storage_system.c` UI 번역 잔여 항목 확인

- 이미 한글화된 박스 메인 메뉴·확인/도구 메시지·기본 박스명 외에, PC 조작 메뉴와 벽지 선택 메뉴가 `sMenuTexts`에 영문으로 남아 있음을 확인했다.
- 조작 메뉴: `GIVE`, `CANCEL`, `STORE`, `WITHDRAW`, `MOVE`, `SHIFT`, `PLACE`, `SUMMARY`, `RELEASE`, `MARK`, `JUMP`, `WALLPAPER`, `NAME`, `TAKE`, `SWITCH`, `BAG`, `INFO`, `SELECT`.
- 벽지 메뉴: `SCENERY 1`~`3`, `ETCETERA`, `FRIENDS` 및 각 벽지 이름. 현재 번역 문장은 수정하지 않았으며, 원문 대조 후 별도 번역 작업으로 처리한다.

### 2026-09-26 — PC 압축 해제 오류 재현 지속, 정적 범위 추가 확인

- 화염구슬·맹독구슬 및 특수 종 데이터 수정 후에도 기존 게임 내 저장에서 `Move Pokémon` 진입 시 PC 압축 해제 오류가 계속 보고됐다. 사용자는 savestate를 사용한 적이 없고, 새 게임 내 저장에서는 현재 재현되지 않는다. 따라서 구슬 스크립트 assertf나 새 ROM의 PC 공통 리소스가 직접 원인이라는 가설은 배제하고, 기존 저장의 파티/박스 데이터에 범위를 좁힌다.
- 오류 입력 `0x08041000`은 `pokehns.map`에서 `AnimTask_NightShadeClone` 직전의 배틀 애니메이션 코드 영역이다. 현재 `pokehns.gba`에는 이 값을 담은 정적 포인터가 없고, PC 메뉴/타일맵과 관련 포켓몬 앞면 그림은 정상 ROM 압축 리소스를 가리킨다.
- 사용자 제공 글라이온·리자몽 요약 화면을 대조했다. 글라이온의 레벨 100 경험치 1,059,860은 느림 성장 그룹의 정상값이고, 양쪽의 능력치·PP는 유효 범위다. 디버그 메뉴도 `CreateMon()`으로 포켓몬을 만든 뒤 파티에 넣으므로 이 두 화면만으로 손상을 지목할 수 없다. `Move Pokémon` 최초 진입은 파티보다 현재 박스 30칸의 아이콘과 0번 칸을 먼저 읽으므로, 두 포켓몬이 파티에만 있으면 직접 원인이 아니다.
- 사용자는 현재 박스가 비어 있다고 확인했다. 빈 박스에도 읽는 `currentBox`와 `boxWallpapers[]` 저장 메타데이터로 범위를 좁혔다. 기존 `GetBoxWallpaper()`는 저장된 ID를 검증하지 않고 `sWallpapers[]` 인덱스로 사용했다. `StorageGetCurrentBox()`의 범위 밖 값은 0번 박스로, 벽지 ID의 범위 밖 값은 해당 박스 기본 벽지로 자동 복구하도록 수정했다. 이 변경은 유효한 저장의 박스/벽지를 바꾸지 않고, 잘못된 값이 압축 포인터로 해석되는 것을 차단한다.
- 이후 기존 저장의 PC가 열리지만 박스 2부터 제목이 중복되고 일부 제목 위치·14번 제목·벽지가 비정상으로 보고됐다. 손상 범위가 `boxNames[]`와 `boxWallpapers[]` 전체 메타데이터임을 확인했다. 범위 밖 박스/벽지 값 감지 시 `RepairPokemonStorageMetadata()`로 모든 제목과 벽지를 기본값으로 재생성하도록 보완했다. 포켓몬이 든 박스 내용, 파티, 퓨전 저장 데이터는 수정하지 않는다.
- 결론: PC 초기화 중 런타임에서 잘못된 포인터가 만들어진다. PC가 선택 위치의 포켓몬 그림을 즉시 풀기 때문에 저장 포켓몬 데이터 또는 그 전의 RAM 손상 가능성을 분리해야 한다.
- 다음 절차: 오류에서 START를 누르지 말고 리셋한다. 기존 `.sav` 사본을 보관한 뒤, PC 최초 선택 위치의 박스/파티 포켓몬과 최근 디버그로 만든 특수 종·폼·지닌도구를 하나씩 분리한다. 디버거 추적이 필요하면 mGBA에서 `0x0810E5E8` 실행 중단점을 걸어 `r0`, `lr`, Call Stack을 확보하고, `lr - 4`를 현재 `pokehns.elf`에 `arm-none-eabi-addr2line`으로 대응시켜 호출자별로 수정한다.

### 2026-09-26 — 화염·맹독구슬, 메가찌르호크, 영원의꽃 플라엣테 오류 수정

- 구슬: 최상위 실행되는 화염구슬·맹독구슬 스크립트에만 `BattleScript_UpdateEffectStatusIconEnd2`를 추가·연결했다. 상태 아이콘 갱신·상태 폼 트리거·텍스트 정리 뒤 `end2`로 끝나므로 빈 스택 `return` assertf가 발생하지 않는다. 기존 `return` 공용 꼬리는 다른 하위 스크립트용으로 유지했다.
- 메가찌르호크: `SPECIES_STARAPTOR_MEGA`의 주석 처리돼 있던 `gMonIcon_StaraptorMega`와 팔레트 0 연결을 복구했다.
- 영원의꽃 플라엣테: `sFloetteEternalFormChangeTable`의 기절·배틀 종료 복원 대상을 일반 플라엣테에서 `SPECIES_FLOETTE_ETERNAL`로 변경했다. 메가진화 후에도 원래 영원의꽃 폼으로 돌아간다.
- 검증: 변경 범위 `git diff --check` 통과. 배틀 스크립트 오브젝트는 재컴파일됐다. 이 세션의 `src/pokemon.c` 대형 종 테이블 컴파일은 실행 환경 문제로 출력 없이 종료돼 새 ROM 전체 링크는 미완료다. 기존 ROM은 수정 전 것이므로 실제 검증에 사용하지 않는다.

### 2026-09-26 — 메가찌르호크 파티 아이콘·영원의꽃 플라엣테 배틀 종료 폼 진단

- 메가찌르호크: `SPECIES_STARAPTOR_MEGA`의 아이콘 파일·심볼은 존재하지만 `src/data/pokemon/species_info/gen_4_families.h`에서 `.iconSprite = gMonIcon_StaraptorMega`와 `.iconPalIndex = 0`가 주석 처리돼 있다. 아이콘 선택 함수는 NULL일 때 물음표 아이콘으로 폴백하므로, 보고된 파티 물음표의 직접 원인이다.
- 영원의꽃 플라엣테: `sFloetteEternalFormChangeTable`의 메가진화 대상은 올바르게 `SPECIES_FLOETTE_MEGA`지만, 기절·배틀 종료 복원 대상이 `SPECIES_FLOETTE`로 잘못 지정돼 있다. 이 표는 영원의꽃과 메가플라엣테가 함께 사용하며, 명시적 대상이 배틀 시작 종 기록보다 우선하므로 일반 플라엣테로 저장된다.
- 필요한 수정: 메가찌르호크 종 정보에서 기존 아이콘 심볼을 연결하고, 영원의꽃 표의 `FORM_CHANGE_FAINT`/`FORM_CHANGE_END_BATTLE` 대상을 `SPECIES_FLOETTE_ETERNAL`로 바꾼다. 이번 요청은 원인 조사이므로 소스·ROM은 수정하지 않았다.

### 2026-09-26 — 화염구슬·맹독구슬 assertf 및 PC 압축 해제/충돌 보고 진단

- 실제 보고: 화염구슬·맹독구슬 발동 시 `src/battle_script_commands.c:5034`의 빈 배틀 스크립트 스택 `return` assertf가 재현된다. PC에서 `Move Pokémon`을 선택하면 압축 해제 실패가 나오며, START로 계속할 경우 그래픽이 깨진 뒤 mGBA가 `Jumped to invalid address: 0451C220`으로 중단된다.
- 확정 원인(구슬): `TryToxicOrb()`·`TryFlameOrb()`는 `BattleScriptExecute()`로 스크립트를 최상위 실행한다. `1821fd6749`에서 두 스크립트가 아이템 팝업과 상태 메시지를 거친 뒤 `BattleScript_UpdateEffectStatusIconRet`로 분기하도록 바뀌었지만, 이 꼬리는 `return`이다. 최상위 스크립트에 복귀 주소는 없으므로 정확히 이 `return`에서 assertf가 난다. 이전 버전의 두 스크립트는 `end2`로 끝났다.
- PC 경로: 오류의 입력 주소 `0x08041000`은 압축 데이터가 아니라 ROM 코드 영역이고, `IN: 0x0810E3B9`/`0x0810E5FF`는 각각 `DecompressDataWithHeaderWram()`/`DecompressionError()`에 대응한다. `gStorageSystemMenu_Gfx`와 메가플라엣테 front pic은 ROM에서 정상 SMOL 헤더를 확인했다. 따라서 PC 오류는 런타임의 잘못된 압축 입력 포인터이며, 정확한 호출자는 아직 미확정이다.
- 조치: 진단 요청 범위이므로 소스는 수정하지 않았다. 이후에는 assertf 화면에서 계속하지 말고 리셋하고, 오류가 전혀 발생하지 않은 게임 내 저장에서 PC를 먼저 열어 독립 재현 여부를 확인한다. 재현 시 mGBA 디버거의 `DecompressionError` 호출자와 첫 인수를 확보한다.

### 2026-09-26 — 메가플라엣테 교체 후 배틀 스크립트 `return` assertf 보고

- 실제 보고: 디버그로 만든 메가플라엣테를 야생 부우부전에서 리아코로 일반 교체하자마자, `src/battle_script_commands.c:5034: return used with nothing to return to` assertf 화면이 표시됐다.
- 의미: `Cmd_return()`이 빈 `battleScriptsStack`에서 실행됐다. 호출된 하위 배틀 스크립트가 반환할 호출자를 잃었거나, 최상위 스크립트가 잘못 `return`한 상태다.
- 정적 대조: 해당 수동 교체는 `BattleScript_DoSwitchOut`의 `switchineffects`·`switchinevents`로 이어진다. 메가플라엣테는 페어리오라, 리아코는 급류/우격다짐이라 이 두 종의 알려진 교체 특성 스크립트만으로는 오류를 설명하지 못했다. `battle_message.c`의 주석 변경은 실행 바이트코드를 바꾸지 않는다.
- 주의: 화면의 `:5`와 줄바꿈 뒤 `034:`는 하나의 `:5034`이며, 현재 소스의 assert 위치와 일치한다. 과거 빌드 ROM 혼용으로 단정하지 않는다. 다만 세이브 스테이트는 ROM 코드 주소를 보존하므로 새 ROM과 혼용하지 않고, 게임 내 저장만 사용해 먼저 재현한다.
- 다음 시작점: 세이브 스테이트 없는 새 배틀에서 일반 포켓몬 → 리아코, 메가플라엣테 → 다른 포켓몬, 메가플라엣테 → 리아코를 비교 재현한다. 재현되면 교체 직전의 출력·배틀 형식·도구·특성 정보를 기준으로 `switchinevents` 하위 호출을 추적한다.

### 2026-09-26 — 현재 `pokehns-expansion-kor` HNS ROM 전체 빌드

- 요청: 현재 작업 상태로 HNS ROM을 빌드한다.
- 기준: `6e807edbbd` (`Document battle message status and handoff`)이며, 로컬 브랜치와 `origin/pokehns-expansion-kor`가 같은 커밋을 가리키는 상태에서 실행했다.
- 검증: `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 성공. `pokehns.gba`가 33,554,432바이트(32MiB)로 생성됐고 파일 시각은 2026-09-26 02:42 KST다.
- 범위: 컴파일·링크 검증만 수행했다. 자동 테스트와 실제 HNS 화면/플레이 검증은 하지 않았으며, 소스 문자열·배틀 로직은 변경하지 않았다.

### 2026-09-26 — 포켓몬피리 입수 경로·테라스탈 상태 및 인수인계 정리

- 요청: 배틀 포켓몬피리의 실제 HNS 입수 가능 여부와 방법을 확인하고, 이전 대화부터의 인수인계 문서가 누락되지 않도록 정리한다.
- 포켓몬피리: `src/data/items.h`의 `ITEM_POKE_FLUTE`는 배틀 효과 `EFFECT_ITEM_USE_POKE_FLUTE`를, `data/battle_scripts_2.s`는 `BattleScript_UsePokeFlute`를 연결한다. 따라서 아이템이 가방에 있으면 배틀 중 사용 가능하다. 하지만 HNS에서 활성화된 맵·이벤트에는 `ITEM_POKE_FLUTE`를 주는 경로가 없다.
- 근거: FRLG 호환 `LavenderTown_VolunteerPokemonHouse_Frlg/scripts.inc`에는 Mr. Fuji의 지급 코드가 남아 있으나, HNS의 `data/maps/headers.inc`와 `data/maps/groups.inc`에는 그 맵이 등록되지 않았다. HNS의 `FLAG_GOT_POKE_FLUTE`도 `0`이다.
- 실제 스토리 대체 경로: 블루시티 체육관의 기계 부품 사건을 해결해 발전소에 부품을 돌려주면 `FLAG_RETURNED_MACHINE_PART`가 설정된다. 이후 보라타운 라디오타워 국장이 확장 카드를 주며 `FLAG_KANTO_RADIO_GOT`을 설정한다. 관동에서 포케기어의 포켓몬피리 라디오를 맞춘 뒤 갈색시티 잠만보를 조사하는 것이 HNS의 잠만보 진행 경로다. 이 과정에서 아이템 포켓몬피리는 지급되지 않는다.
- 테라스탈: 일반 플레이어 사용은 현재 비활성이다. `include/config/battle.h`의 `B_FLAG_TERA_ORB_CHARGED`와 `B_FLAG_TERA_ORB_NO_COST`가 `0`이며 `CanTerastallize()`가 충전 플래그를 요구한다. 테스트의 허용 분기는 일반 플레이 설정을 뜻하지 않는다.
- 인수인계: `docs/friend-handoff/CURRENT_HNS_HANDOFF.md`를 추가하고 `README.md`에서 연결했다. 이 문서는 Fork/브랜치, 보호해야 할 한글화·배틀 메시지 변경, 핵심 기능 상태, 검증 규칙을 한곳에 정리한다.
- GitHub 상태 정정: `git branch -vv`에서 `pokehns-expansion-kor`는 `origin/pokehns-expansion-kor`의 `791876da59`를 추적하는 것을 확인했다. 아래의 푸시 대기 기록은 당시 상태이며, 현재는 적용되지 않는다.
- 검증: 소스·맵 헤더·맵 그룹·이벤트·플래그·라디오·배틀 스크립트를 정적으로 확인했다. 이번 문서 작업에서는 소스·ROM을 수정하거나 빌드·실기 검증을 실행하지 않았다.


### 2026-09-26 — 친구용 1.17.0 안전 업데이트 작업 지시서 추가

- 요청: 친구에게 현재 HNS를 1.15.1 이후 upstream 변경부터 `pokeemerald-expansion` 1.17.0까지 업데이트하는 작업을 맡길 수 있도록 지시서를 작성한다.
- 구현: `docs/friend-handoff/POKEEMERALD_EXPANSION_1.17.0_UPDATE.md`에 보존 대상, 이미 선별 이식된 1.17.0 PR, 충돌 시 중단·보고 절차, HNS 빌드 검증과 결과 보고 양식을 기록했다.
- 범위: 기존 한글 텍스트, HNS 전용 파일·코드, 기술/특성별 배틀 메시지와 특성 팝업·상태별 메시지 최신화 변경을 임의 변경하지 않는 것을 명시했다.
- 상태 정정: 이후 사용자 인증 터미널의 푸시가 완료되어 이 문서는 현재 Fork 브랜치에 반영되어 있다.

### 2026-09-26 — 친구 작업 지시서 폴더 추가

- 요청: 친구에게 맡길 작업 지시사항을 GitHub에서 한곳에 관리할 폴더를 만든다.
- 구현: `docs/friend-handoff/README.md`를 추가하고 현재 상태·작업 절차·세션 기록 링크와 작업 지시서 양식을 넣었다.
- 결과 정정: `b977946ae7` 및 후속 문서는 이후 Fork에 푸시되었다. 친구에게 맡길 항목별 Markdown 파일을 `docs/friend-handoff/` 아래에 추가하고 결과를 기록한다.

### 2026-09-26 — `pokehns-expansion-kor` 업로드 범위 정정

- 요청: 현재 HNS를 `pokehns-expansion` 저장소의 `pokehns-expansion-kor` 브랜치로 업로드한다.
- 확인: Fork `gorunit1/pokehns-expansion-kor`의 `pokehns-expansion-kor` 브랜치로 기존 커밋된 3개 커밋은 푸시되었다.
- 상태 정정: 전체 작업 트리의 991개 변경·새 파일을 `1821fd6749` 커밋으로 기록했고, 이 커밋과 후속 문서는 이후 Fork에 푸시되었다. 현재 원격 상태는 새 작업 전 `git branch -vv`로 다시 확인한다.

### 2026-09-26 — 끈적끈적바늘 전이 연출 제거

- 요청/범위: 도구가 없는 공격자에게 끈적끈적바늘이 넘어갈 때 `STRINGID_STICKYBARBTRANSFER`와 아이템 탈취 애니메이션을 출력하지 않고 즉시 전이한다.
- 확인: `TryStickyBarbOnTargetHit()`가 `StealTargetItem(battlerAtk, battlerDef)`을 스크립트 호출 전에 실행하므로 실제 도구 이동은 이미 완료된다. 이후 `BattleScript_StickyBarbTransfer`의 애니메이션·문자열은 연출만 담당한다.
- 구현: `BattleScript_StickyBarbTransfer`에서 `playanimation`, `printstring STRINGID_STICKYBARBTRANSFER`, `waitmessage`를 제거하고 기존 `removeitem BS_TARGET` 및 반환은 유지했다. 전이 조건과 끈적끈적바늘의 턴 종료 피해는 변경하지 않았다.
- 테스트: `test/battle/hold_effect/sticky_barb.c`에서 접촉 전이 시 `B_ANIM_ITEM_STEAL`과 기존 전이 메시지가 나오지 않는지 확인하도록 변경했다. 비접촉 공격의 기존 무전이 조건도 유지했다.
- 수정 파일: `data/battle_scripts_1.s`, `test/battle/hold_effect/sticky_barb.c`, `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`, `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`.
- 검증: `NODEP=1 SETUP_PREREQS=0 timeout 180s make BUILD=hns build/hns/data/battle_scripts_1.o -j1` 성공. `NODEP=1 SETUP_PREREQS=0 timeout 180s make BUILD=hns TEST=1 build/hns-test/test/battle/hold_effect/sticky_barb.o -j1` 성공. `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 전체 빌드·링크 성공(ROM 33,329,828바이트, 99.33%). 실제 HNS 화면 검증은 미완료다.

### 2026-09-26 — 습기의 유폭 차단 메시지 제거

- 요청/범위: 습기가 유폭을 막을 때 `STRINGID_PKMNSABILITYPREVENTSABILITY`를 출력하지 않고 특성 팝업 뒤 텍스트 없이 처리한다. 유폭 피해 차단은 유지한다.
- 명칭 정정: 코드의 `ABILITY_AFTERMATH`는 공식 한글 명칭 `유폭`이다. `멸망의바디`는 `ABILITY_PERISH_BODY`로 별개의 특성이므로, 이전 STATUS 기록의 표현을 정정했다.
- 구현: `data/battle_scripts_1.s`의 `BattleScript_DampPreventsAftermath`에서 `printstring STRINGID_PKMNSABILITYPREVENTSABILITY`와 `waitmessage`를 제거했다. `src/battle_util.c`의 습기 감지와 `BattleScript_DampPreventsAftermath` 분기는 건드리지 않아 유폭 반격 피해 계산 경로에 들어가지 않는다.
- 현재 출력: 습기·유폭 특성 팝업 후 배틀 텍스트 없음. `STRINGID_PKMNSABILITYPREVENTSABILITY`는 이 경로에서 호출되지 않는다.
- 수정 파일: `data/battle_scripts_1.s`, `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`, `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`.
- 검증: `NODEP=1 SETUP_PREREQS=0 timeout 180s make BUILD=hns build/hns/data/battle_scripts_1.o -j1` 성공. 기존 습기·유폭 시나리오의 `test/battle/ability/damp.c` 오브젝트도 `NODEP=1 SETUP_PREREQS=0 timeout 180s make BUILD=hns TEST=1 build/hns-test/test/battle/ability/damp.o -j1`로 컴파일 성공했다. `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 전체 빌드·링크 성공(ROM 33,329,844바이트, 99.33%). 실제 HNS 화면 검증은 미완료다.

### 2026-09-26 — 배리어프리 방벽 제거 메시지 순차 출력

- 요청: `배리어프리` 발동 시 특성 팝업을 먼저 표시하고, `깨뜨리다`·`사이코팽`·`레이징불`과 같은 방식으로 제거된 방벽을 리플렉터 → 빛의장막 → 오로라베일 순서로 출력한다.
- 기존 경로: `TryRemoveScreens()`가 양쪽 진영의 방벽을 한 번에 지우고 `bool`만 반환했다. 이후 `BattleScript_SwitchInAbilityMsg`가 `STRINGID_SCREENCLEANERENTERS` 범용 문구를 출력해 방벽 종류와 순서를 표현하지 못했다.
- 구현: `TryRemoveScreens()`가 세 방벽 종류의 메시지 비트마스크를 반환하게 변경했다. `BattleScript_ScreenCleanerActivates`는 `BattleScript_AbilityPopUpScripting`으로 실제 배리어프리 보유자의 팝업을 표시한 뒤 `BattleScript_BreakScreensMessages`를 호출한다. `gBattlerAttacker`는 문구의 기존 `{B_ATK_PREFIX1}`를 위해 특성 보유자로 임시 저장하고, 메시지 출력 후 `restoreattacker`로 복원한다.
- 출력: 존재하는 방벽만 `STRINGID_REFLECTWOREOFF`, `STRINGID_LIGHTSCREENWOREOFF`, `STRINGID_AURORAVEILWOREOFF` 순서로 각각 출력한다. 방벽이 없으면 특성 발동 자체가 성립하지 않아 팝업·문구가 없다. 기존 한글 문장과 지정 코드는 재사용했다.
- 테스트: `test/battle/ability/screen_cleaner.c`의 TODO를 교대·세 방벽·팝업·순서 검증 시나리오로 교체했다.
- 수정 파일: `src/battle_util.c`, `data/battle_scripts_1.s`, `include/battle_scripts.h`, `test/battle/ability/screen_cleaner.c`, `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`, `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`.
- 검증: `NODEP=1 SETUP_PREREQS=0 timeout 180s make BUILD=hns build/hns/src/battle_util.o build/hns/data/battle_scripts_1.o -j2` 성공. `NODEP=1 SETUP_PREREQS=0 timeout 180s make BUILD=hns TEST=1 build/hns-test/test/battle/ability/screen_cleaner.o -j1` 성공. `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 전체 빌드·링크 성공(ROM 33,329,844바이트, 99.33%). 실제 HNS 화면 검증은 남아 있다.

### 2026-09-26 — 정화 상태별 회복 메시지 연결

- 요청/범위: `정화`가 상태이상을 치료할 때 `STRINGID_ATTACKERCUREDTARGETSTATUS` 대신 최신 기준의 상태별 문구를 대상별로 출력하도록 수정했다.
- 수정 파일: `include/constants/battle_string_ids.h`, `src/battle_message.c`, `data/battle_scripts_1.s`, `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`, `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`.
- 구현: `BattleScript_EffectPurify`의 고정 `printstring`을 `printfromtable gPurifyStatusCureStringIds`로 교체했다. 독·화상·마비·잠듦은 `{B_SCR_NAME_WITH_PREFIX}`를 사용하는 새 ID를 추가했고, 얼음·동상은 이미 대상 슬롯을 사용하는 기존 ID를 재사용했다. 선택자는 `BS_CureStatus`가 치료 전 상태에서 설정하므로 정화 대상별 문구가 나온다.
- 매칭: 독은 `말끔하게 해독됐다!`, 화상은 `화상이 나았다!`, 마비는 `몸저림이 풀렸다!`, 얼음은 `얼음이 녹았다!`, 동상은 `동상이 나았다!`, 잠듦은 `눈을 떴다!`를 사용한다. 문장 본문은 기존 한글화 문장에서 가져왔고 새로 창작하지 않았다.
- 팝업: 정화는 특성 발동이 아니므로 특성 팝업을 추가하지 않았다.
- 빌드 중 `STRINGID_SYMBIOSISITEMPASS`에 남은 `{B_EFF_NAME_WITH_PREFIX2}}` 토큰 오탈자를 발견해 `{B_EFF_NAME_WITH_PREFIX2}`로 수정했다. 문자열 문장은 그대로다.
- 검증: 대상 오브젝트 빌드 성공. 전체 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 성공(ROM 33,329,748바이트, 99.33%). 실제 게임 화면 검증은 미완료다.
- 다음 시작점: 실제 HNS에서 정화 대상이 공격자와 다른 상황을 포함해 각 상태별 회복 문구를 재현한다.

### 2026-09-26 — `STRINGID_THEWALLSHATTERED` 출력 조건 확인

- 당시 확인 기준으로 `gBreakScreensStringIds[B_MSG_BREAK_SCREENS_GENERIC]`가 `STRINGID_THEWALLSHATTERED`에 연결되어 있고, `BattleScript_BreakScreens`가 실제로 이 테이블을 출력했다.
- 이후 복수 방벽 처리 구현을 변경해, 현재는 이 설명이 과거 상태를 기록하는 이력으로만 남는다. 현재 동작은 아래의 후속 기록을 기준으로 한다.
- 방벽이 없거나 기술이 무효라 제거가 진행되지 않으면 이 ID도 출력되지 않는다. Defog·Rapid Spin의 장판 제거는 별도 테이블을 사용한다.
- 이번 확인은 정적 호출부·선택 테이블 대조만 수행했으며 소스 코드는 수정하지 않았다. 실제 HNS 화면 출력은 아직 검증하지 않았다.

### 2026-09-26 — 복수 방벽 제거 메시지 순차 출력

- 요청: `깨뜨리다`·`사이코팽`·`레이징불`로 여러 방벽을 제거할 때 `STRINGID_THEWALLSHATTERED` 대신 실제 존재하는 방벽별 문구를 리플렉터 → 빛의장막 → 오로라베일 순서로 출력한다.
- 구현: 제거 직전의 방벽 비트를 `gBattleCommunication[MULTISTRING_CHOOSER]`에 비트마스크로 저장한 뒤, `BattleScript_BreakScreens`가 리플렉터 → 빛의장막 → 오로라베일 순서로 검사하고 전용 ID를 각각 직접 출력하도록 변경했다. 따라서 2개면 2개, 3개면 3개만 출력하며 방벽 종류의 설치 순서는 영향을 주지 않는다.
- `STRINGID_THEWALLSHATTERED` 정의는 레거시 호환성을 위해 남겨 두었지만 현재 세 기술의 방벽 제거 경로에서는 선택되지 않는다. 방벽이 없거나 기술이 무효인 경우 기존처럼 제거·문구 출력이 없다.
- 수정 파일: `include/constants/battle_string_ids.h`, `src/battle_script_commands.c`, `src/battle_message.c`, `data/battle_scripts_1.s`, `test/battle/move_effect_secondary/break_screens.c`, `test/battle/move_effect/raging_bull.c`, `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`, `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`.
- 검증: `NODEP=1 SETUP_PREREQS=0 timeout 180s make BUILD=hns build/hns/src/battle_script_commands.o build/hns/data/battle_scripts_1.o -j2` 성공. 변경한 테스트 오브젝트도 `NODEP=1 SETUP_PREREQS=0 timeout 180s make BUILD=hns TEST=1 build/hns-test/test/battle/move_effect_secondary/break_screens.o build/hns-test/test/battle/move_effect/raging_bull.o -j2`로 컴파일 성공했다. `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 전체 HNS 빌드·링크도 성공했으며, EWRAM 249,016/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,329,572/33,554,432(99.33%)이다. 실제 HNS 화면 출력은 별도 확인이 필요하다.

### 2026-09-26 — 미번역 배틀 문자열의 출력 조건 확인

- `src/battle_message.c`에 남은 영문 잔여 ID를 배틀 스크립트·선택 테이블·호출부와 대조했다.
- 실제 출력 가능: Symbiosis 도구 전달, 정화의 상태 치료, 습기의 애프터마스 방지, Screen Cleaner의 방벽 제거, 멸망의바디의 3턴 카운트, 점착바브 전이, 눈·안개 지속, Rapid Spin의 뾰족한 강철 제거, 테라스탈 후속 문구, 배틀 중 포켓몬피리, Sleep Clause 차단이다.
- 현재 호출되지 않음: Poison Heal·Solar Power·Ice Body의 구형 HP 문구, `EFFECT_PLACEHOLDER` 안내(현재 일반 기술 없음), 독압정·끈적끈적네트·스텔스록의 구형 Rapid Spin 문구, 지옥찌르기 종료 문구다. 특히 지옥찌르기 타이머는 줄어들지만 종료 스크립트 호출은 없다.
- 조건부/구세대: Dynamax 4개 ID는 시작 문구만 조건부로 가능하고 현재 추가 완료 문구는 설정상 비활성화되어 있다. `ITISHAILING`은 Gen9 미만 오버월드 눈 분기에서만 선택된다. FRLG 유령 출현·실프스코프·유령 행동 문구와 Safari 바위·미끼·화남·먹는 중 문구는 해당 모드에서만 사용된다.
- 현재 기본 HNS 설정에서 Sleep Clause와 플레이어 Dynamax는 비활성화되어 있음을 함께 확인했다. 소스 문자열은 수정하지 않았고, 실제 게임 화면 검증도 하지 않았다.
- 근거 위치와 세부 조건은 `docs/localization/STATUS.md`의 동일 날짜 항목에 기록했다.

### 2026-09-26 — 황금몸 배틀 메시지 최신 원문 대조

- 확인: `CanAbilityAbsorbMove()`의 `ABILITY_GOOD_AS_GOLD` 분기는 대상 지정 변화 기술을 `BattleScript_GoodAsGoldActivates`로 보내며, 이 스크립트는 황금몸 특성 팝업 뒤 `STRINGID_SCR_ITDOESNTAFFECT`를 출력한다.
- 현재 출력: `src/battle_message.c`의 `{B_SCR_NAME_WITH_PREFIX}에게는\n효과가 없는 것 같다...`는 SV `ko_common.txt`와 Champions `ko_ms.txt`의 대상별 `효과가 없는 것 같다...` 문구와 일치한다. 따라서 코드나 문장을 수정하지 않았다. 필드 대상·전체 대상 기술은 황금몸의 이 메시지 경로에 들어가지 않는다.
- 근거: [SV 한국어 원문](https://raw.githubusercontent.com/abcboy101/poke-corpus/main/corpus/ScarletViolet/ko_common.txt)의 `btl_set` 대상별 효과 없음 문구, [Champions 한국어 원문](https://raw.githubusercontent.com/abcboy101/poke-corpus/main/corpus/Champions/ko_ms.txt)의 `btl_set` 대상별 효과 없음 문구, `src/battle_util.c:2493-2512`, `data/battle_scripts_1.s:7899-7904`.
- 검증 상태: 정적 코드·문자열 대조 완료. 실제 HNS 화면에서 특성 팝업과 문구 순서는 아직 확인하지 않았다.

### 2026-09-26 — 마그마의무장 얼음 방지 메시지 제거

- 요청: 최신 사양에 맞춰 마그마의무장이 얼음을 막을 때 별도 특성 팝업과 `얼지 않는다!`를 출력하지 않게 수정했다.
- 수정: `src/battle_util.c`의 `ABILITY_MAGMA_ARMOR` 분기를 `BattleScript_NotAffected`로 변경했다. 따라서 얼음 부여는 계속 차단되지만 `abilityAffected`가 설정되지 않고 `BattleScript_StatusProtects`도 호출되지 않는다.
- 구분: 이미 얼어 있던 포켓몬이 전투 중 마그마의무장을 새로 얻어 해동되는 `TryImmunityAbilityHealStatus()` 경로는 별도 동작이므로 유지했다. 이번 변경은 얼음 부여 방지 시도의 팝업·전용 문구만 대상으로 한다.
- 검증: `NODEP=1 SETUP_PREREQS=0 timeout 180s make BUILD=hns build/hns/src/battle_util.o -j1` 성공, 전체 `NODEP=1 SETUP_PREREQS=0 GITHUB_ACTION=1 timeout 600s make -o .map_version --jobserver-style=pipe hns -j8` 링크 성공. ROM 사용량은 33,329,508바이트(99.33%)다. 실제 게임 화면은 미검증이다.
- 원문 근거: [SV 한국어 원문](https://raw.githubusercontent.com/abcboy101/poke-corpus/main/corpus/ScarletViolet/ko_common.txt), [Champions 한국어 원문](https://raw.githubusercontent.com/abcboy101/poke-corpus/main/corpus/Champions/ko_ms.txt).
- 다음 시작점: 실제 HNS에서 마그마의무장 대상에게 얼음 기술·상태 효과를 사용해 얼음 차단, 일반 실패 메시지, 팝업 부재를 확인한다.

### 2026-09-25 — 특성 발동 메시지 최신 기준 연결 및 하품 경계 수정

- 요청: 하품 턴 종료 시 절대안깸 경계를 수정하고, 지정된 특성과 그 밖의 실제 특성 발동 메시지를 최신 SV·Pokémon Champions 기준으로 대조·연결했다.
- 수정 파일: `src/battle_end_turn.c`, `src/battle_util.c`, `src/battle_script_commands.c`, `src/battle_message.c`, `data/battle_scripts_1.s`, `include/constants/battle_string_ids.h`, `include/battle_scripts.h`, `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`, `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`.
- 구현: `HandleEndTurnYawn()`에 절대안깸 검사를 추가했다. 상태이상 방지 특성은 `SetStatusProtectsStringId()`와 `gStatusProtectsStringIds`를 통해 특성 팝업 뒤 상태별 실패 문구를 출력한다. 리프가드·리밋실드의 잠자기 전용 native 판정도 특성 보유자와 수면 선택값을 설정해 같은 스크립트로 연결했다. 플라워베일·스위트베일·불면·의기양양은 각각 최신 전용 스크립트/문구를 유지했다.
- 회복: 면역·파스텔베일·유연·불면·의기양양·수의베일·수포·열교환·마그마의무장의 상태 회복은 회복된 상태별 문구를 선택하고, 혼란·헤롱헤롱·도발은 전용 최신 문구를 사용하도록 연결했다. 절대안깸은 회복 경로가 아니라 상태이상 방지 경로를 사용한다. 치유의마음은 `치유되었다!`를 사용한다.
- 기타 감사: 실제 특성 호출부의 예측·날씨·패러독스·스탯 변화·위협·통찰·긴장감·변환·황금몸 등은 최신 원문과 대조했다. 최신 원문을 확인할 수 없는 레거시·미사용 문자열은 번역을 창작하지 않았다.
- 후속 확인: 마그마의무장은 최신 게임에서 얼음 방지 효과는 유지하지만, 별도 특성 팝업이나 특성 전용 출력이 없는 경로로 재분류했다. 현재 HNS는 `CanSetNonVolatileStatus()`에서 이를 `BattleScript_StatusProtects`로 보내 팝업과 `얼지 않는다!`를 출력하므로 아직 불일치가 남아 있다. 이번 턴은 확인만 요청된 범위라 코드는 수정하지 않았다.
- 원문: [SV `ko_common.txt`](https://raw.githubusercontent.com/abcboy101/poke-corpus/main/corpus/ScarletViolet/ko_common.txt), [Champions `ko_ms.txt`](https://raw.githubusercontent.com/abcboy101/poke-corpus/main/corpus/Champions/ko_ms.txt).
- 검증: 관련 오브젝트 컴파일 성공, `git diff --check` 통과, 전체 HNS 링크 성공. `pokehns.gba`는 33,554,432바이트로 생성되었고 링크 보고 ROM 사용량은 33,329,524바이트(99.33%)다. 실제 게임 화면은 미검증이다.
- 다음 세션: 마그마의무장의 얼음 방지 경로를 팝업·특성 전용 문구 없이 바꾸고, 목록 특성의 상태이상 방지·회복 및 수면 경로를 실제 HNS 화면/런타임 테스트로 재현한다. 현재 구현과 이전 같은 날짜 기록이 다르면 최신 STATUS 항목을 따른다.

### 2026-09-25 — 절대안깸 최신 동작 대조

- `ABILITY_COMATOSE`(`절대안깸`)를 최신 SV·Champions 기준으로 정적 대조했다. 이번 확인에서는 코드를 수정하지 않았다.
- 상태이상 6종 차단, `Rest`의 이미 잠든 상태 처리, Snore/Sleep Talk와 Dream Eater/Nightmare/Bad Dreams의 수면 취급, 절대안깸의 억제·복사 방지 플래그는 현재 구현과 대체로 일치한다.
- 다만 상태이상 차단 메시지는 현재 특성 팝업 뒤 `STRINGID_ITDOESNTAFFECT` 하나로 통합되어 있다. 최신 원문과 이미 추가된 상태별 ID에 맞추려면 절대안깸도 독·화상·마비·얼음·잠듦 선택자를 사용해야 한다.
- `HandleEndTurnYawn()`에 절대안깸 검사가 없어, 일반 하품은 선행 판정에서 막히더라도 절대안깸 획득 후 남은 하품 volatile 같은 경계 사례는 추가 확인이 필요하다. 교체 시 출력 문구 `비몽사몽 상태!`는 최신 SV 원문과 일치한다.
- 근거와 다음 작업은 `STATUS.md`의 동일 날짜 항목에 기록했다. 실제 HNS 화면 검증은 하지 않았다.

### 2026-09-25 — battle_message.c 최신 원문 대조 및 정화의소금 상태별 문구 연결

- 요청/범위: `src/battle_message.c`의 남은 영문을 SV·Pokémon Champions 한국어 원문과 대조하고, 상태별 실패 문구처럼 최신 게임의 출력 로직과 HNS가 다른 경우 코드까지 수정했다. 이미 한글화된 문장 자체는 임의로 변경하지 않았다.
- 원문 근거: 작업 트리에 corpus 파일이 없어 Poké Corpus의 SV `ko_common.txt`와 Champions `ko_ms.txt` 원격 원문을 대조했다. Champions `btl_set`의 5개 상태 면역 문구와 SV의 `눈이 내리기 시작했다!`를 사용했다.
- 문자열 수정: `STRINGID_SNOWWARNINGHAIL`을 `눈이 내리기 시작했다!`로 교체했다. 번역 과정에서 잘못 남은 조사 리터럴을 `THUNDERCAGETRAPPED`·`PKMNITEMMELTED`·`HOSPITALITYRESTORATION`의 `{B_TXT_...}` 토큰으로 바꿨고, 지원되지 않는 `\\r`을 `\\n`으로 수정했다.
- 코드 수정: `include/constants/battle_string_ids.h`에 독·화상·마비·얼음·잠듦 실패 ID와 선택자 열거를 추가했다. `src/battle_message.c`에 `gPurifyingSaltProtectsStringIds`와 5개 원문 문구를 추가하고, `src/battle_util.c`의 `ABILITY_PURIFYING_SALT` 분기가 상태별 선택자를 설정하도록 변경했다. `data/battle_scripts_1.s`의 새 스크립트는 특성 팝업 뒤 선택된 문구를 출력하며, `BattleScript_InsomniaProtects`는 수면 실패 문구를 직접 출력한다.
- 보류: 최신 원문이 없는 레거시/미사용/기능별 문자열은 영문으로 유지했다. 이 목록은 STATUS.md의 동일 날짜 항목에 기록했다. 실제 게임 화면 검증은 하지 않았다.
- 검증: `make BUILD=hns build/hns/src/battle_message.o build/hns/src/battle_util.o -j2`, `make BUILD=hns build/hns/data/battle_scripts_1.o -j1`, `make BUILD=hns build/hns/src/battle_move_resolution.o -j1`이 성공했다. 이어서 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 전체 HNS 빌드·링크도 종료 코드 0으로 성공했고 최신 링크 보고 ROM 사용량은 33,329,556바이트/32MiB(99.33%)다. 이전 전체 빌드 로그는 `build/localization-logs/hns-battle-message-modern-ko-20260925.log`에 남겼고, 마지막 링크 명령은 터미널에서 직접 확인했다.
- 다음 시작점: 정화의소금 상태별 5개 경로와 특성 팝업 순서를 실기/에뮬레이터에서 확인한다. 이어서 영문 잔여 ID를 호출부별로 확정한다.

### 2026-09-25 — 배틀 메시지 한글 토큰 연결 감사

- 요청/범위: `src/battle_message.c`의 현재 한글화 문장에서 `{B_...}` 지정 코드가 연결된 코드의 원래 의미대로 출력되는지 점검했다. 문장 내용은 수정하지 않고 토큰만 대상으로 삼았다.
- 수정 파일: `src/battle_message.c`, `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`, `docs/localization/evidence/2026-09-25-battle-message-token-audit.txt`.
- 변경과 결정 이유: `STRINGID_PKMNWASPARALYZEDBY`의 첫 토큰을 `B_SCR_NAME_WITH_PREFIX`→`B_EFF_NAME_WITH_PREFIX`로 바꿨다. 원문 구조상 마비 대상과 특성 보유자가 다르고, 현재 HNS의 특성 보유자는 `B_SCR`에 기록된다. `STRINGID_PKMNMOVEBOUNCED`의 이름 토큰을 `B_DEF_NAME_WITH_PREFIX`→`B_ATK_NAME_WITH_PREFIX`로 바꿨다. `BS_SetMagicCoatTarget()`가 반사자를 공격자 슬롯에 둔 뒤 문장을 출력한다.
- 유지한 항목: 스위트베일용 `STRINGID_PKMNSXMADEITINEFFECTIVE`는 `B_SCR` 이름·특성, 정신력용 `STRINGID_PKMNSXPREVENTSFLINCHING`은 `B_EFF` 이름·특성으로 올바르다. 불면·의기양양의 `STRINGID_PKMNSTAYEDAWAKEUSING`은 특성 팝업이 능력명을 먼저 보여 주므로 문장에 능력 토큰을 추가하지 않았다.
- 참고/제약: SV·Champions 원문 corpus와 `champout`는 현재 작업 트리에 없어 로컬 `pokeemerald-kr` 원문 및 HNS 호출부를 사용했다. `STRINGID_PKMNWASPARALYZEDBY`는 현재 HNS 선택 테이블에서 일반 마비 ID로 대체되어 런타임상 사용되지 않지만, 재사용될 경우를 위해 토큰을 원문 의미에 맞췄다.
- 검증: `git diff --check -- src/battle_message.c` 성공. `make BUILD=hns build/hns/src/battle_message.o -j2` 성공(종료 코드 0). 전체 ROM 빌드와 실제 게임 화면 검증은 하지 않았다.
- 남은 문제: 현재 `gGotParalyzedStringIds[B_MSG_STATUSED_BY_ABILITY]`가 `STRINGID_PKMNWASPARALYZED`를 선택하므로 특성 유발 마비 전용 문구는 출력되지 않는다. 이는 이번 요청의 토큰 범위를 넘어서는 기존 출력 선택 문제다.
- 다음 시작점: 매직코트 반사 문구와 특성 유발 마비 경로를 실제 HNS 화면에서 확인하고, 필요할 때 선택 테이블 변경을 별도 작업으로 검토한다.

### 2026-09-25 — 코드 경로 변경으로 달라진 배틀 메시지 출력 목록화

- 요청/범위: 단순 한글화가 아니라, 사용자 요청으로 배틀 코드·스크립트를 바꿔 같은 기술/특성/도구 효과가 다른 메시지를 출력하게 된 모든 사례를 문서화했다. 1.17.0 이식과 PR #9777에서 문자열만 추가·번역한 부분은 제외하고, PR #9777의 구현 코드 변경은 포함했다.
- 결과: `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 이전 출력·현재 출력·변경 지점을 표로 정리했다. 일격기, 장벽/장판, 상태이상/리샘열매/잠자기, 포이즌힐·솔라파워·아이스바디, 도주·야생 도구 드롭·텔레포트, Champions 효과 단계·아이템 팝업과 생명의구슬 반동 문구 변경을 포함한다.
- 제외: 같은 ID의 번역·조사·줄바꿈 수정, 실제 코드 변경이 없는 출력 조건 조사, 1.17.0 이식 또는 PR #9777에서 문자열만 추가한 부분은 넣지 않았다.
- 검증: 현재 메시지 테이블·배틀 스크립트·기존 작업 기록을 정적으로 대조했다. 문서 작업만 했으므로 ROM 빌드와 게임 화면 검증은 실행하지 않았다.
- 다음 시작점: 새 메시지 출력 경로를 추가하거나 실제 화면 검증을 끝내면 해당 행의 동작/검증 상태를 이 문서에 갱신한다.

### 2026-09-23 — 불면·의기양양 및 스위트베일 수면 방지 메시지 분리

- 요청/범위: 불면·의기양양으로 잠들지 않을 때는 특성 팝업 뒤 `STRINGID_PKMNSTAYEDAWAKEUSING`을, 스위트베일로 잠들지 않을 때는 특성 팝업 뒤 `STRINGID_PKMNSXMADEITINEFFECTIVE`를 출력하도록 변경했다.
- 구현: `BattleScript_StayedAwakeUsingAbility`를 추가해 불면·의기양양의 일반 수면 판정과 잠자기에 연결했다. 스위트베일은 일반 수면 판정, 하품의 턴 종료 수면, 잠자기 전용 판정에서 특성 보유자를 팝업 주체로 기록하고 각각 일반 실패 스크립트 또는 종료 스크립트로 연결했다.
- 범위 유지: 정화의소금은 기존 `STRINGID_ITDOESNTAFFECT` 경로를 유지했다. 리프가드·리밋실드의 잠자기 실패 처리도 변경하지 않았다.
- 검증: 관련 오브젝트 5개 컴파일 성공. `make MAP_VERSION=hns generated -j1`로 맵 산출물을 복구한 뒤 `NODEP=1 SETUP_PREREQS=0 timeout 600s make -o .map_version --jobserver-style=pipe hns -j8` 전체 HNS 빌드·링크 성공(ROM 사용량 33,330,324바이트/32 MiB, 99.33%). 대상 파일 `git diff --check` 통과. 실제 게임 화면·자동 런타임 테스트는 미실행이다.
- 주의/다음 시작점: 표준 병렬 HNS 빌드는 이 세션에서 맵 생성 중 `include/constants/map_groups.h`를 일시 삭제했다. 다음에는 먼저 `make MAP_VERSION=hns generated -j1`을 완료한 후 위의 `.map_version` 보존 빌드 명령을 사용한다. 실제 HNS에서 불면/의기양양의 수면 기술·잠자기와 스위트베일의 단일/더블 수면·하품·잠자기를 확인해 팝업 주체와 문구 순서를 검증한다.

### 2026-09-23 — 8세대 이후 텔레포트의 개미지옥·그림자밟기·자력 도주 방지 제거

- 요청/범위: `GEN_LATEST`에서 야생 포켓몬이 `텔레포트`를 사용하면 개미지옥·그림자밟기·자력을 무시해 정상 도주하고, 실패 문구와 특성 팝업도 표시하지 않도록 변경했다.
- 구현: 일반 도주 판정을 `IsRunningFromBattleImpossibleInternal()`로 분리하고, 기존 `IsRunningFromBattleImpossible()`는 모든 특성 도주 방지 검사를 유지했다. 새 `IsTeleportRunningFromBattleImpossible()`만 `B_TELEPORT_BEHAVIOR >= GEN_8`에서 `IsAbilityPreventingEscape()`를 건너뛴다. `BS_IsRunningImpossible`은 이 전용 함수를 호출한다.
- 결과: 야생 텔레포트는 `BATTLE_RUN_FAILURE`가 되지 않아 `BattleScript_PrintAbilityMadeIneffective` 및 `BattleScript_AbilityPopUp`으로 가지 않고 도주 애니메이션·성공 처리로 계속된다. Gen 7 이하는 원래 특성 방지 판정을 유지한다. 특성 이외의 `BATTLE_RUN_FORBIDDEN` 조건은 변경하지 않았다.
- 회귀 테스트: `test/battle/move_effect/teleport.c`에 개미지옥·그림자밟기·자력 세 경우를 매개변수화한 야생전 테스트를 추가했다. 각 경우 텔레포트 애니메이션이 실행되고, 세 특성의 팝업 및 `But it failed!`가 없는지를 검증한다.
- 검증: `make BUILD=hns build/hns/src/battle_main.o build/hns/src/battle_script_commands.o -j2`와 `make BUILD=hns TEST=1 build/hns-test/test/battle/move_effect/teleport.o -j2`가 성공했다. 선별 런타임 테스트는 기존 `test/test_runner.c`의 `fake_rtc.h` 포함 순서 오류(`struct SaveBlock3` 미정의)로 테스트 실행 전에 중단됐다. `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 전체 HNS 빌드는 성공했고 ROM 사용량은 33,330,196바이트/32 MiB(99.33%)다.
- 게임 화면 확인: 미확인. 다음 시작점은 기존 테스트 러너의 헤더 포함 문제를 별도로 해결한 뒤 선별 런타임 테스트를 실행하고, 실제 HNS에서 세 특성별 야생 텔레포트 도주를 확인하는 것이다.

### 2026-09-23 — 8세대 이후 텔레포트 도주 방지 메시지 판정 정정

- 정정 범위: `STRINGID_PKMNSXMADEITINEFFECTIVE`가 `GEN_LATEST` 텔레포트에서도 올바르게 출력되는지 재검토했다.
- 결론: 8세대 이후 텔레포트는 트랩 상태·개미지옥·그림자밟기·자력과 무관하게 성공해야 한다. 현재 HNS 소스는 트레이너 배틀과 플레이어 측 텔레포트를 `BattleScript_EffectBatonPass`로 처리하지만, 야생 포켓몬의 텔레포트는 `BattleScript_DoEffectTeleport` → `isrunningimpossible`로 보내므로 해당 특성이 있으면 `STRINGID_PKMNSXMADEITINEFFECTIVE`와 특성 팝업을 출력할 수 있다. 따라서 야생 텔레포트 경로는 8세대 이후 동작과 불일치하는 잔여 코드다.
- 구분: 수면 방지의 `CanSetNonVolatileStatus()` 경로는 여전히 `BattleScript_PrintAbilityMadeIneffective`와 해당 문자열을 올바르게 사용한다. 이번 확인에서는 코드 수정이나 빌드를 하지 않았다.
- 검증: `data/battle_scripts_1.s:3432-3445`, `src/battle_main.c:4280-4291`, `src/battle_util.c:5083-5110`, `include/config/battle.h:122,174`를 정적 확인하고 8세대 이후 텔레포트의 야생 포켓몬·트랩 동작 설명을 대조했다. 실제 HNS 화면 검증은 미완료다.
- 다음 시작점: 야생 포켓몬의 텔레포트 처리에서 `B_TELEPORT_BEHAVIOR >= GEN_8`일 때 도주 방지 특성 검사를 우회할지 결정하고, 변경 시 `make hns -j8` 및 실제 전투를 검증한다.

### 2026-09-23 — 리샘열매 상태 회복을 상태별 메시지로 분리

- 요청/범위: 리샘열매로 마비·독·화상·얼음·잠듦·혼란이 치료될 때 공통 `STRINGID_PKMNSITEMNORMALIZEDSTATUS` 대신 각 상태 전용 ID가 출력되도록 변경했다.
- 구현: `TryCureAnyStatus()`가 치료 전 상태와 혼란 여부를 `gBattleScripting.lumBerryCureStatusMask`에 저장한다. 새 `BS_TryPrintNextLumBerryCureStatus`와 `BattleScript_LumBerryCureStatusRet`가 이 비트를 하나씩 꺼내 `CureStatusBerryEffectStringID` 테이블의 전용 ID를 선택·출력한다. 아이템 팝업과 애니메이션은 첫 메시지 전에 한 번만, 상태 아이콘 갱신과 열매 소모는 모든 메시지 뒤 한 번만 실행한다.
- 매핑/예시: 마비→“리샘열매로 마비가 풀렸다!”, 독/맹독→“리샘열매로 독이 해독됐다!”, 화상→“리샘열매로 화상이 나았다!”, 얼음→“리샘열매로 얼음 상태가 나았다!”, 잠듦→“리샘열매로 눈을 떴다!”, 혼란→“리샘열매로 혼란이 풀렸다!”. 독+혼란처럼 복수 상태면 독 문구 뒤 혼란 문구를 연속 출력한다. 동상은 기존 동상 전용 ID를 유지했다.
- 수정 파일: `include/battle.h`, `include/constants/battle_string_ids.h`, `include/battle_scripts.h`, `asm/macros/battle_script.inc`, `src/battle_message.c`, `src/battle_hold_effects.c`, `src/battle_script_commands.c`, `data/battle_scripts_1.s`, 상태·세션 문서.
- 검증: `build/hns/src/battle_hold_effects.o`, `build/hns/src/battle_script_commands.o`, `build/hns/data/battle_scripts_1.o`가 성공했다. 이후 사용자가 `src/battle_message.c:795`의 문자열 문법을 수정한 상태에서 `GITHUB_ACTION=1 timeout 600s make hns -j8` 전체 빌드도 성공했다. 링크 보고 ROM 사용량은 33,330,276바이트(99.33%)다. 실제 게임 화면 검증은 미완료다.
- 다음 시작점: 여섯 단일 상태와 독+혼란 복수 상태에서 리샘열매 메시지 순서를 실제 화면으로 검증한다.

### 2026-09-23 — 만병통치제의 상태 회복 메시지 확인

- 요청/범위: 배틀 중 만병통치제 사용으로 상태이상을 치료했을 때 출력되는 문구를 확인했다.
- 결론: `STRINGID_ITEMCUREDSPECIESSTATUS`가 출력된다. 현재 문자열 본문은 영문 `"{B_BUFF1} had its status healed!"`이며, `{B_BUFF1}`은 치료된 포켓몬의 종 이름이다. 만병통치제 이름이나 독·화상 등 치료된 상태명은 이 문구에 들어가지 않는다.
- 경로: `ITEM_FULL_HEAL`은 `EFFECT_ITEM_CURE_STATUS`를 사용하며 `BattleScript_ItemCureStatus` → `itemcurestatus`로 처리한다. 전장 포켓몬이면 상태 아이콘을 갱신한 뒤, 파티 포켓몬이면 곧바로 같은 문구를 출력한다.
- 구분: 리샘열매의 복수 상태 회복 문구 `STRINGID_PKMNSITEMNORMALIZEDSTATUS`는 지닌 열매 전용이며, 만병통치제에는 사용되지 않는다.
- 검증: `src/data/items.h:1093-1107`, `data/battle_scripts_2.s:90-103`, `src/battle_script_commands.c:12150-12202`, `src/battle_message.c:795`를 `rg`·`sed`로 정적 확인했다. 소스·ROM 수정, HNS 빌드 및 게임 화면 검증은 하지 않았다.
- 다음 시작점: 상태이상 포켓몬에게 만병통치제를 사용해 사용 알림, 영문 치료 문구, 전장 포켓몬의 상태 아이콘 갱신을 실제 HNS 화면에서 확인하고 필요하면 이 ID를 한글화한다.

### 2026-09-23 — `STRINGID_PKMNSITEMNORMALIZEDSTATUS` 출력 조건 확인

- 요청/범위: 지닌 도구로 상태이상이 치료될 때의 `STRINGID_PKMNSITEMNORMALIZEDSTATUS` 선택 조건을 확인했다.
- 결론: 리샘열매(`HOLD_EFFECT_CURE_STATUS`)가 한 번에 둘 이상의 상태를 치료할 때 출력한다. 일반적인 재현은 비휘발성 상태이상 하나와 혼란이 동시에 걸린 경우다. 비휘발성 상태이상 하나만 또는 혼란만 치료하면 `STRINGID_PKMNSITEMCUREDPROBLEM`을 사용한다.
- 흐름: `TryCureAnyStatus()`가 치료 대상 수를 세어 복수면 `B_MSG_NORMALIZED_STATUS`를 설정하고, `BattleScript_BerryCureStatusRet`가 아이템 팝업 → 발동 애니메이션 → 이 문구 → 상태 아이콘 갱신 → 리샘열매 소모 순으로 처리한다.
- 토큰: `{B_SCR_NAME_WITH_PREFIX}`는 열매를 발동한 포켓몬, `{B_LAST_ITEM}`은 리샘열매다.
- 검증: `src/data/items.h:11125-11135`, `src/battle_hold_effects.c:760-818,1169-1171`, `src/battle_message.c:472,1371-1380`, `data/battle_scripts_1.s:7134-7141`를 `rg`·`sed`로 정적 확인했다. 소스·ROM 수정, HNS 빌드 및 게임 화면 검증은 하지 않았다.
- 다음 시작점: 독/화상/마비/잠듦/얼음 중 하나와 혼란을 함께 부여한 리샘열매 보유 포켓몬으로 팝업·문구·상태 아이콘 갱신·열매 소모 순서를 실제 HNS 화면에서 확인한다.

### 2026-09-23 — 도주 특성 팝업 후 `STRINGID_PKMNFLEDUSING` 출력

- 요청/범위: 도주 특성으로 도망에 성공할 때 `STRINGID_PKMNFLEDUSING`보다 먼저 다른 특성처럼 특성 팝업을 표시하도록 변경했다.
- 수정: `data/battle_scripts_1.s`의 `BattleScript_RanAwayUsingMonAbility`에 `copybyte gBattlerAbility, gBattlerAttacker`와 `call BattleScript_AbilityPopUp`을 추가했다. 도망에 성공한 포켓몬을 팝업 주체로 지정한 뒤 특성 팝업을 표시하므로, 출력 순서는 `도주` 팝업 → “무사히 도망쳤다”다.
- 범위: `FLEE_ABILITY` 경로만 변경했다. 연막탄(`FLEE_ITEM`), 고스트 타입 및 일반 도망 경로는 그대로다.
- 검증: `make BUILD=hns build/hns/data/battle_scripts_1.o -j1` 성공 후 `GITHUB_ACTION=1 timeout 600s make hns -j8` 전체 빌드에 성공했다. 링크 보고 ROM 사용량은 33,330,180바이트(99.33%)다.
- 게임 화면 확인: mGBA 실행 파일 부재로 미확인.
- 다음 시작점: 도주 특성 포켓몬으로 야생전 도망을 재현해 팝업의 포켓몬·특성명 및 뒤따르는 메시지 순서를 실제 화면에서 확인한다.

### 2026-09-23 — 도구 능력치 상승·대상 변경·도주 메시지 출력 조건 보완

- 요청/범위: `STRINGID_USINGITEMSTATOFPKMNROSE`의 정확한 출력 시점과, 이어서 문의한 `STRINGID_PKMNSXTOOKATTACK`의 출력 조건·특성 팝업 순서 및 `STRINGID_PKMNFLEDUSING`의 출력 조건을 확인했다.
- 도구 메시지: `STRINGID_USINGITEMSTATOFPKMNROSE`는 지닌 도구의 능력치 상승 효과가 실제로 적용될 때 출력한다. 약점보험·눈덩이·빛이끼·충전지·흡수벌브·실책보험·목스프레이·아드레날린오브·능력치 상승 열매·필드 시드가 해당하며, 약점보험처럼 여러 능력치가 오르면 능력치별로 반복될 수 있다.
- 대상 변경 메시지: `STRINGID_PKMNSXTOOKATTACK`는 더블배틀의 피뢰침(전기)/마중물(물) 대상 가로채기에서 출력한다. `BattleScript_TookAttack`에는 특성 팝업 호출이 없으므로 이 문구 자체의 직전 팝업은 없다. 다만 `GEN_LATEST`에서는 그 뒤 타입 흡수와 특공 상승을 처리하는 `BattleScript_MoveStatDrain`이 특성 팝업을 띄우므로, 전체 흐름은 대상 변경 문구 → 팝업 → 특공 상승 문구다.
- 도주 메시지: `STRINGID_PKMNFLEDUSING`는 플레이어의 도주 특성 보유 포켓몬이 도망을 선택해 성공할 때 출력한다. 현재 문자열 본문은 `{PLAY_SE 0x0011}무사히 도망쳤다\\p`다. 초기 기록에서 이 본문을 “도주를 써서 도망쳤다”라고 잘못 적었으며, 상단 항목에서 정정했다. 연막탄·고스트 타입·일반 도주는 각각 별도 ID를 사용한다. 현재는 상단 변경으로 이 문구 앞에 도주 특성 팝업이 표시된다.
- 검증: `rg`·`sed`로 `src/battle_message.c:497`, `data/battle_scripts_1.s:4332-4345,6575-6579,6606-6614`, `src/battle_move_resolution.c:832-850,1719-1727`, `src/battle_util.c:544-605,2438-2479`, `src/battle_main.c:5781-5793`을 정적 확인했다. 소스·ROM 수정, HNS 빌드 및 게임 화면 확인은 하지 않았다.
- 다음 시작점: 해당 문구를 현대식 팝업 중심 표현으로 바꾸려면 도구·피뢰침/마중물·도주를 서로 독립된 작업으로 나누어 배틀 스크립트 수정 범위를 결정하고 HNS 빌드 및 실제 화면 검증을 수행한다.

### 2026-09-23 — 도구 능력치 상승 메시지 TODO의 의미와 사용처 확인

- 요청/범위: `STRINGID_USINGITEMSTATOFPKMNROSE`의 TODO 주석 의미, 현재 구현의 문제 여부, 연결된 도구 발동 경로를 확인했다.
- 결론: TODO는 런타임 오류가 아니라 메시지 표현 현대화 메모다. 상류의 Gen 5+ 메시지 갱신에서 도구명을 직접 문장에 넣는 형식을 남겨 두고, 도구 팝업과 일반 능력치 상승 메시지로 완전히 바꾸려면 코드·배틀 스크립트 변경이 필요하다는 의미로 추가됐다.
- 연결: `gStatUpStringIds[B_MSG_STAT_CHANGED_ITEM]`가 이 ID를 가리킨다. 공통 소비형 능력치 상승 스크립트와 약점보험·눈덩이·빛이끼·충전지·흡수벌브·블런더정책·목스프레이·아드레날린오브의 전용 스크립트가 출력한다.
- 토큰: `B_SCR_NAME_WITH_PREFIX2`=발동 포켓몬, `B_LAST_ITEM`=발동 도구, `B_BUFF1`=능력치, `B_BUFF2`=상승 정도다. 현재 코드가 이 값을 채워서 한글 문장도 정상 출력 가능하다.
- 검증: `git show`·`git log`로 TODO가 `Update battle messages to Gen 5+ standards` 커밋에서 도입된 것을 확인했고, `src/battle_message.c:497,1142-1149`, `data/battle_scripts_1.s:4752-4799,6271-6281,7336-7353`, `src/battle_hold_effects.c:264-340,408-418,490-530,946-1013,1085-1203`를 정적 확인했다. 이번 확인에서는 소스·ROM을 수정하거나 빌드하지 않았다.
- 다음 시작점: 도구 팝업을 적용하려면 각 전용 스크립트와 공통 소비형 스크립트를 분리 검토하고, 도구 팝업 뒤 일반 능력치 상승 메시지로 바꾼 뒤 HNS 빌드 및 실제 화면 검증을 수행한다.

### 2026-09-23 — 일반 기술·특성의 상태 회복 메시지 상태별 분리

- 요청/범위: `STRINGID_PKMNSTATUSNORMAL` 및 `STRINGID_PKMNSXCUREDYPROBLEM`이 사용되던 상태 회복 경로를 독·화상·마비·얼음·잠듦별 문자열로 분리하고, 특성 팝업을 유지했다.
- 수정 파일: `include/constants/battle_string_ids.h`, `include/battle_util.h`, `src/battle_message.c`, `src/battle_util.c`, `src/battle_script_commands.c`, `data/battle_scripts_1.s`, `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`.
- 구현: `gStatusCureStringIds`에 상태별 문자열을 매핑하고 `GetCuredStatusMessage()`가 치료 전 `status1`을 공용 선택값으로 변환한다. `Cmd_curestatuswithmove`, `BS_CureStatus`, 촉촉한몸·탈피 상태 제거부가 상태를 지우기 전에 선택값을 저장하며, 다섯 배틀 스크립트의 고정 `printstring`을 `printfromtable`로 바꿨다.
- 출력 매핑: 독→`STRINGID_PKMNPOISONCURED`, 화상→`STRINGID_PKMNBURNCURED`, 마비→`STRINGID_PKMNPARALYSISCURED`, 얼음→`STRINGID_PKMNWASDEFROSTED`, 잠듦→`STRINGID_PKMNWOKEUP`. 동상·예외 기본값은 `STRINGID_PKMNSTATUSNORMAL`이다.
- 팝업: `BattleScript_ShedSkinActivates`의 `call BattleScript_AbilityPopUp`은 수정하지 않았고 상태별 문구는 그 뒤에 출력된다.
- 검증: 관련 네 오브젝트(`battle_message.o`, `battle_util.o`, `battle_script_commands.o`, `battle_scripts_1.o`) 빌드 성공. 이어서 `GITHUB_ACTION=1 timeout 600s make hns -j8` 성공, ROM 33,554,432바이트, 링크 보고 ROM 사용 33,330,196바이트(99.33%). `git diff --check`도 통과했다.
- 게임 화면 확인: 미확인. 다음 시작점은 브레이브차지/리프레시 또는 정글힐과 촉촉한몸/탈피를 이용해 다섯 상태의 문구 및 특성 팝업 순서를 실제 HNS에서 재현하는 것이다.

### 2026-09-23 — `STRINGID_PKMNWASDEFROSTEDBY`의 기술 사용자·기술명 토큰 확인

- 질문/범위: `...BY` 문구를 `{B_SCR_NAME_WITH_PREFIX}`와 `{B_CURRENT_MOVE}`로 작성하면 아군·상대 기술이 모두 반영되는지 확인했다.
- 결론: 해동 기술을 사용한 쪽이 아군인지 상대인지와 관계없이 반영된다. `B_SCR`는 현재 해동된 포켓몬, `B_CURRENT_MOVE`는 메시지 전송 시점의 현재 기술을 표시한다.
- 제한: 이 ID는 `CancelerThaw`가 해동 기술로 사용자 자신을 해동할 때만 선택된다(`B_MSG_DEFROSTED_BY_MOVE`). 상대의 불꽃 기술이 대상 포켓몬을 해동하는 `MoveEndThaw` 경로는 `B_MSG_DEFROSTED`를 선택해 `STRINGID_PKMNWASDEFROSTED`를 출력한다.
- 검증: `src/battle_message.c:236-237,3313-3322,1242-1245`, `src/battle_move_resolution.c:574-584,2930-2942`, `src/battle_controllers.c:1007-1016`을 `nl`·`sed`로 정적 확인했다. 소스 수정과 HNS 빌드는 하지 않았다.
- 다음 시작점: 상대 기술로 내 포켓몬이 해동될 때도 원인 기술명을 표시하려면 `MoveEndThaw`의 메시지 선택자를 별도로 `B_MSG_DEFROSTED_BY_MOVE`로 바꿀지 검토해야 한다.

### 2026-09-23 — 해동 메시지의 `DEF`→`SCR` 교체 검토

- 질문/범위: `STRINGID_PKMNWASDEFROSTED`의 대상 토큰을 스크립트 활성 배틀러 토큰으로 바꾸면 공격자 자연 해동 시 이름 문제가 해결되는지 확인했다.
- 결론: 해결된다. `{B_DEF_NAME_WITH_PREFIX}`는 `gBattlerTarget`, `{B_SCR_NAME_WITH_PREFIX}`는 `gBattleScripting.battler`를 사용한다. 해동 스크립트의 모든 확인된 호출부는 자연 해동·기술 해동·대상 해동·상태 제거 전에 해동된 배틀러를 `gBattleScripting.battler`에 저장한다.
- 검증: `src/battle_message.c:236,3299-3315`, `src/battle_move_resolution.c:101-105,175-178,580-583,2932-2942`, `src/battle_util2.c:190-194`, `src/battle_script_commands.c:3561-3580`을 `nl`·`rg`·`sed`로 정적 확인했다. 소스 수정과 HNS 빌드는 하지 않았다.
- 다음 시작점: 실제 변경을 적용할 때 `src/battle_message.c:236`의 토큰만 `{B_SCR_NAME_WITH_PREFIX}`로 교체한 뒤 `make hns -j8` 빌드와 자연 해동·기술 해동·불꽃 기술 해동 화면 검증을 수행한다.

### 2026-09-23 — 공격자 해동 시 해동 메시지의 이름 토큰 확인

- 요청/범위: 얼음 상태인 공격자가 행동하려다 해동될 때 `STRINGID_PKMNWASDEFROSTED`가 어떤 포켓몬 이름을 표시하는지 확인했다.
- 확인 결과: `CancelerAsleepOrFrozen`의 자연 해동은 `B_MSG_DEFROSTED`를 선택해 `STRINGID_PKMNWASDEFROSTED`를 사용하지만, 이 문자열의 `{B_DEF_NAME_WITH_PREFIX}`는 `src/battle_message.c`의 치환부에서 `gBattlerTarget`을 참조한다. 일반적인 상대 대상 기술에서는 공격자가 아닌 대상 이름이 표시될 수 있는 토큰 불일치다.
- 예외/정상 경로: 공격자가 해동 기술을 사용해 해동되면 `B_MSG_DEFROSTED_BY_MOVE` → `STRINGID_PKMNWASDEFROSTEDBY`로 가며 `{B_ATK_NAME_WITH_PREFIX}`와 `{B_CURRENT_MOVE}`가 공격자와 기술명을 표시한다. 불꽃 기술이 상대 대상을 해동하는 기술 종료 경로에서는 `STRINGID_PKMNWASDEFROSTED`와 대상 토큰이 맞는다.
- 검증: `src/battle_message.c:236-237,1239-1243,3296-3301`, `data/battle_scripts_1.s:5745-5749`, `src/battle_move_resolution.c:101-106,108-180,570-584,2917-2943`를 `rg`·`sed`로 정적 확인했다. 소스 수정과 HNS 빌드는 하지 않았다.
- 게임 화면 확인: 미확인(mGBA 실행 파일 부재).
- 다음 시작점: 공격자 자연 해동 메시지의 의도에 맞춰 `{B_DEF_NAME_WITH_PREFIX}`를 다른 토큰으로 바꿀지, 해동 스크립트 직전에 출력 대상 슬롯을 조정할지 결정한 뒤 HNS 빌드와 실제 화면 검증을 수행한다.

### 2026-09-23 — `STRINGID_PKMNHEALEDPARALYSIS` 출력 조건 확인

- 요청/범위: 마비 회복 문자열의 직접 호출부와 현재 연결된 기술을 확인했다.
- 수정 파일: `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`.
- 변경과 결정 이유: `BattleScript_TargetPRLZHeal`이 `MOVE_EFFECT_REMOVE_STATUS`의 마비 분기에서 이 ID를 출력한다. 현재 기술 데이터상 마비 제거 추가 효과를 가진 기술은 `정신차리기`다.
- 검증: `rg`·`nl`·`sed`로 `src/battle_message.c:242`, `data/battle_scripts_1.s:5898-5902`, `src/battle_script_commands.c:3555-3571`, `src/data/moves_info.h:7189-7205`를 정적 확인했다. 문서 수정 후 ROM 빌드는 하지 않았다.
- 게임 화면 확인: 미확인.
- 남은 문제: 실제 HNS 화면에서 마비 대상에게 `정신차리기`를 사용했을 때 문구와 조사 확장을 재현할 필요가 있다.
- 다음 시작점: 새 HNS ROM에서 `정신차리기`의 마비 해제 효과를 재현하거나 필요하면 이 문자열의 문장 형태를 조정한다.

### 2026-09-23 — 해동·기상 메시지 출력 조건 확인

- 요청/범위: `STRINGID_PKMNWASDEFROSTED`와 `STRINGID_PKMNWOKEUP`의 실제 선택 조건 및 유사 메시지와의 차이를 확인했다.
- 수정 파일: `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`.
- 변경과 결정 이유: 해동 ID는 `gGotDefrostedStringIds`의 일반 해동 선택지이며, 기상 ID는 일반 수면 턴 종료 선택지다. 소란피기·도구·기술로 상태를 제거하는 경로는 각각 별도 문자열을 사용한다.
- 검증: `rg`·`nl`·`sed`로 `src/battle_message.c:1127-1130,1239-1242`, `data/battle_scripts_1.s:5695-5699,5745-5749`, `src/battle_move_resolution.c:108-180,570-584,2918-2943`, `src/battle_util2.c:135-194`를 정적 확인했다. 문서 수정 후 ROM 빌드는 하지 않았다.
- 게임 화면 확인: 미확인.
- 남은 문제: 실제 HNS 화면에서 자연 해동, 불꽃 기술에 의한 해동, 일반 기상, 소란피기 기상을 각각 재현할 필요가 있다.
- 다음 시작점: 새 HNS ROM에서 네 상태 회복 경로를 재현하거나 필요하면 메시지 한글화와 빌드를 수행한다.

### 2026-09-23 — `STRINGID_PKMNBURNHEALED` 미사용 판정 정정

- 요청/범위: `STRINGID_PKMNBURNHEALED`의 실제 사용 여부를 재확인했다.
- 수정 파일: `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`.
- 변경과 결정 이유: 이 ID는 미사용이 아니며 `BattleScript_TargetBurnHeal`에서 직접 출력된다. `MOVE_EFFECT_REMOVE_STATUS`가 화상을 제거하는 경로에서 사용되고, 현재 기술 데이터상 `물거품아리아`가 대상의 화상을 치료할 때 호출된다. `정신차리기`는 유사한 `STRINGID_PKMNHEALEDPARALYSIS`를 사용한다.
- 검증: `rg`·`nl`·`sed`로 `data/battle_scripts_1.s:5898-5920`, `src/battle_script_commands.c:3555-3590`, `src/data/moves_info.h:16190-16220`, `src/battle_message.c:717`을 정적 확인했다. 문서 수정 후 ROM 빌드는 하지 않았다.
- 게임 화면 확인: 미확인.
- 남은 문제: `STRINGID_PKMNBURNHEALED`와 `STRINGID_PASTELVEILENTERS`의 영문 문구를 HNS 한글화 문구로 바꿀지는 별도 결정이 필요하다.
- 다음 시작점: `물거품아리아`로 화상 대상의 회복 메시지를 재현하거나, 필요하면 두 문자열의 한글화와 HNS 빌드를 수행한다.

### 2026-09-23 — 독·화상·마비 회복 메시지 ID 확인

- 요청/범위: 독·화상·마비가 치료될 때 사용할 수 있는 상태 회복 문자열과 실제 호출부를 확인했다.
- 수정 파일: `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`.
- 변경과 결정 이유: 일반 기술 회복은 `STRINGID_PKMNSTATUSNORMAL`을 공통 사용한다. 열매·도구 발동은 `STRINGID_PKMNSITEMCUREDPOISON`, `STRINGID_PKMNSITEMHEALEDBURN`, `STRINGID_PKMNSITEMCUREDPARALYSIS`를 각각 사용한다. 이후 재확인 결과 `STRINGID_PKMNHEALEDPARALYSIS`는 `정신차리기`, `STRINGID_PKMNBURNHEALED`는 `물거품아리아`의 대상 상태 제거 경로에서도 사용되는 것으로 정정했다.
- 검증: `rg`·`sed`로 `src/battle_message.c:236,242,345,463-469,717,1127-1130,1239-1242,1356-1365,1398-1401`, `data/battle_scripts_1.s:5695-5749,7132-7139`, `src/battle_hold_effects.c:673-755`를 정적 확인했다. 문서 수정 후 ROM 빌드는 하지 않았다.
- 게임 화면 확인: 미확인.
- 남은 문제: 실제 HNS 화면에서 일반 기술 회복과 열매 발동 회복의 문구를 각각 재현할 필요가 있다.
- 다음 시작점: 필요하면 `STRINGID_PKMNSTATUSNORMAL`을 상태별 일반 회복 문구로 분리할지 결정한 뒤 관련 배틀 스크립트를 수정·빌드한다.

### 2026-09-23 — `STRINGID_PKMNSTATUSNORMAL` 출력 조건 확인

- 요청/범위: 상태이상 회복 메시지의 직접 호출부와 실제 출력 조건을 확인했다.
- 수정 파일: `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`.
- 변경과 결정 이유: `BattleScript_EffectTakeHeart`, `BattleScript_EffectRefresh`, `BattleScript_EffectJungleHealing`, `BattleScript_EffectPsychoShift`가 상태를 실제로 치료한 뒤 이 ID를 출력한다. `curestatuswithmove`는 치료할 상태가 없으면 실패 분기로 보내므로 `브레이브차지`·`리프레시`의 무상태 사용에서는 출력되지 않는다. `정글힐`·`초승달의기도`는 대상별로, `사이코시프트`는 상태를 넘긴 뒤 사용 포켓몬에게 출력된다.
- 검증: `rg`·`nl`·`sed`로 `data/battle_scripts_1.s:380-390,755-780,1278-1297,3842-3850`, `src/battle_script_commands.c:9715-9740`, `src/data/moves_info.h`의 관련 기술 정의를 정적 확인했다. 문서 수정 후 ROM 빌드는 하지 않았다.
- 게임 화면 확인: 미확인.
- 남은 문제: 실제 HNS 화면에서 네 기술의 치료 대상 이름과 조사 확장은 별도 재현이 필요하다.
- 다음 시작점: 새 HNS ROM에서 상태이상이 있는/없는 경우를 나눠 `리프레시`, `브레이브차지`, `정글힐`, `초승달의기도`, `사이코시프트`를 재현한다.

### 2026-09-23 — `STRINGID_PKMNSXMADEYINEFFECTIVE`·`STRINGID_PKMNSXCUREDYPROBLEM` 출력 조건 확인

- 요청/범위: 두 특성 관련 메시지 ID의 실제 출력 경로와 치환 토큰을 확인했다.
- 수정 파일: `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`.
- 변경과 결정 이유: `STRINGID_PKMNSXMADEYINEFFECTIVE`는 `BattleScript_StickyHoldActivatesRet`·`BattleScript_NoItemSteal` 및 타오르는불꽃 재흡수의 `gFlashFireStringIds`에서 사용된다. `STRINGID_PKMNSXCUREDYPROBLEM`은 `BattleScript_ShedSkinActivates`에서 사용되며 촉촉한몸·탈피의 상태 회복 경로가 이 스크립트를 호출한다. 면역 특성의 상태 해제는 유사한 `STRINGID_PKMNSXCUREDITSYPROBLEM`을 사용한다.
- 검증: `rg`·`nl`·`sed`로 `src/battle_message.c:481-482,1375-1379`, `data/battle_scripts_1.s:366,6623-6627,6716-6725,7062-7072`, `src/battle_util.c:2481-2555,3709-3735,9123-9214`, `src/battle_move_resolution.c:3057-3114`, `src/battle_script_commands.c:9857-9863`을 정적 확인했다. 문서 수정 후 ROM 빌드는 하지 않았다.
- 게임 화면 확인: 미확인.
- 남은 문제: 실제 HNS 화면에서 점착·타오르는불꽃·촉촉한몸·탈피 경로의 팝업 및 한글 조사 확장은 별도 재현이 필요하다.
- 다음 시작점: 새 HNS ROM에서 도구 조작 방지, 타오르는불꽃 재흡수, 비/턴 종료 상태 회복을 각각 재현하거나 필요 시 `make hns -j8`로 빌드한다.

### 2026-09-23 — `STRINGID_PKMNSXPREVENTSYLOSS` 출력 조건 확인

- `ChangeStatBuffs()`가 특정 능력치 하락 방지 특성을 확인하면 `BattleScript_AbilityNoSpecificStatLoss`를 예약하고 이 ID를 출력한다. 대상 특성은 현재 코드상 날카로운눈·심안·발광(명중률), 괴력집게(공격), 부풀린가슴(방어)이다.
- `PREPARE_STAT_BUFFER(gBattleTextBuff1, statId)`가 `{B_BUFF1}`에 공격·방어·명중률 등의 능력치명을 넣는다. 특성 팝업이 먼저 나오고, 예를 들어 괴력집게가 공격 하락을 막으면 `상대 포켓몬은\n괴력집게 때문에 공격이 떨어지지 않는다!`가 된다.
- 클리어바디·메탈프로텍트·하얀연기는 `BattleScript_AbilityNoStatLoss` 및 `STRINGID_PKMNPREVENTSSTATLOSSWITH`를 사용한다. 검증: `src/battle_script_commands.c:7839-7931,11852-11864`, `data/battle_scripts_1.s:6641-6646,6707-6714`를 정적 확인했다. 소스·데이터 수정과 HNS 빌드는 하지 않았다.

### 2026-09-23 — `STRINGID_PKMNSXRESTOREDHPALITTLE2` 출력 조건 확인

- `BattleScript_AbilityHpHeal`이 이 ID를 출력하며, 비의날씨의 `Rain Dish`·`Dry Skin`과 열매 섭취 후 `Cheek Pouch`의 회복 성공 경로가 이를 호출한다. 회복량은 현재 코드에서 각각 Rain Dish 1/16, Dry Skin 1/8, Cheek Pouch 1/3 최대 HP다.
- 특성 팝업 → 메시지 → HP 바/데이터 갱신 순서다. 해당 스크립트에서 공격자 슬롯을 특성 보유자로 맞추므로 `{B_ATK_NAME_WITH_PREFIX}`와 `{B_ATK_ABILITY}`는 회복한 포켓몬과 특성명을 가리킨다.
- `Ice Body`는 이 ID가 아니라 특성 팝업과 회복 애니메이션만 있는 `BattleScript_IceBodyHeal`을 사용한다. 검증은 `src/battle_util.c:3650-3700`, `src/battle_script_commands.c:6611-6625`, `data/battle_scripts_1.s:6195-6212,4484-4489` 정적 확인으로 수행했으며, 소스·데이터 수정과 HNS 빌드는 하지 않았다.

### 2026-09-23 — 연애 방지 문구와 특성 유발 헤롱헤롱 문구 비교

- `STRINGID_PKMNPREVENTSROMANCEWITH`는 `BattleScript_ObliviousPreventsAttraction`에서 특성 팝업 후 출력되며, 매력/헤롱헤롱 시도가 특성으로 차단되었음을 표시한다. `{B_DEF_NAME_WITH_PREFIX}`는 차단 특성 보유자다.
- `STRINGID_PKMNSXINFATUATEDY`는 `BattleScript_CuteCharmActivates`에서 특성 팝업·애니메이션 후 출력되며, 특성으로 공격자가 실제 헤롱헤롱 상태가 되었음을 표시한다. `{B_DEF_NAME_WITH_PREFIX}`/`{B_DEF_ABILITY}`는 특성 보유자, `{B_ATK_NAME_WITH_PREFIX}`는 상태이상 대상이다.
- 결론: 두 ID는 같은 문구의 중복이 아니라, 효과 차단과 효과 성공을 각각 나타낸다. 이번 확인에서는 소스·데이터·ROM 수정 및 HNS 빌드를 하지 않았다.

### 2026-09-23 — `STRINGID_PKMNSXINFATUATEDY` 재사용 상태 확인

- `gAttractUsedStringIds[B_MSG_STATUSED_BY_ABILITY]`가 현재도 `STRINGID_PKMNSXINFATUATEDY`를 가리키고, `BattleScript_CuteCharmActivates`가 특성 팝업 뒤 이 ID를 출력한다. 따라서 `{B_DEF_ABILITY} 때문에 ... 헤롱헤롱해졌다!` 문장이 다시 보이는 것은 이 매핑이 남아 있기 때문이다.
- 이전에 처리한 특성 수면·독·화상·마비 메시지 교체에는 헤롱헤롱 매핑 변경이 포함되지 않았다. 현재 `git diff`에서는 문자열과 매핑이 미커밋 사용자 변경으로 남아 있으며, 이번 세션에서 소스 파일을 수정하거나 되돌리지 않았다.
- 일반 헤롱헤롱 문구를 특성 발동에도 사용하려면 `B_MSG_STATUSED_BY_ABILITY` 매핑을 `STRINGID_PKMNFELLINLOVE`로 바꾸는 별도 작업이 필요하다. 이번에는 진단만 수행했다.
- 검증: `src/battle_message.c:480,1251-1255`, `data/battle_scripts_1.s:6993-6999`, `git diff`, `git blame`을 정적 확인했다. HNS 빌드와 실제 게임 화면 검증은 하지 않았다.

### 2026-09-23 — HNS 유령 배틀 도달 가능성 재확인

- 이전 답변에서 `DoGhostBattle` 공통 코드가 있다는 이유로 HNS에서 유령 배틀이 가능하다고 표현한 것은 부정확했다. 현재 유령 배틀 콘텐츠는 FRLG 전용이다.
- `data/maps/PokemonTower_*_Frlg/map.json`의 `game_version: "frlg"`, HNS 생성물 `data/maps/groups.inc`의 `gMapGroup_Dungeons_Frlg` 항목 전부 `NULL`, `src/data/wild_encounters.h`의 포켓몬타워 데이터가 `#ifdef FIRERED`/`#ifdef LEAFGREEN`인 것을 확인했다. 따라서 HNS 정상 진행에는 포켓몬타워 유령 배틀과 `STRINGID_ITDODGEDBALL` 출력 경로가 없다.
- `src/battle_setup.c`의 `CheckSilphScopeInPokemonTower`·`DoGhostBattle`, `StartMarowakBattle`은 공통 소스에 남아 있으므로 커스텀으로 FRLG 맵을 연결하거나 `BATTLE_TYPE_GHOST`를 직접 설정하면 재사용 가능하다.
- 검증: `src/battle_setup.c:365-391,502-510,592-606`, `data/maps/PokemonTower_1F_Frlg/map.json:2-5`, `data/maps/PokemonTower_6F_Frlg/scripts.inc:4-10`, `data/maps/groups.inc:1220-...`, `src/data/wild_encounters.h:29424`를 `rg`·`sed`·`nl`로 정적 확인했다. 문서는 갱신했으며 소스·게임 데이터 수정과 HNS ROM 빌드는 하지 않았다.

### 2026-09-23 — `STRINGID_ITDODGEDBALL` 출력 조건 확인

- `Cmd_handleballthrow`가 `BATTLE_TYPE_GHOST`를 먼저 검사하므로 유령 전투에서 볼을 던지면 포획 판정 없이 `BattleScript_GhostBallDodge`로 분기하고 이 문장을 출력한다.
- 일반 포획 실패의 흔들림·탈출 메시지와 다르며, 특성·도구 팝업은 없다. 공통 소스의 `DoGhostBattle`이 `BATTLE_TYPE_GHOST`를 설정하지만, 현재 HNS 맵·이벤트에는 해당 경로가 연결되어 있지 않다.
- 검증: `src/battle_script_commands.c:11042-11069`, `data/battle_scripts_2.s:325-329`, `src/battle_setup.c:502-510`, `src/battle_message.c:430`을 `rg`·`sed`로 확인했다.
- 수정 파일은 문서 두 개뿐이며, 정적 확인만 수행했고 ROM 빌드는 하지 않았다. 게임 화면 확인은 mGBA 실행 파일 부재로 미확인이다.

### 2026-09-23 — `STRINGID_NOPPLEFT` 출력 조건 확인

- 일반 배틀의 기술 선택 중 PP가 0인 기술을 고르면 `BattleScript_SelectingMoveWithNoPP`가 이 ID를 선택 문자열로 출력한다. 현재 문자열은 `남은 PP가 없다!\p`이며, 메시지 뒤 기술 선택으로 돌아간다.
- 행동 실행 단계에서 PP가 0이면 `BattleScript_NoPPForMove`와 `STRINGID_BUTNOPPLEFT`가 사용된다. 배틀 팰리스에서는 `STRINGID_NOPPLEFT` 선택 스크립트를 사용하지 않는다.
- 검증: `src/battle_util.c:1635-1645`, `data/battle_scripts_1.s:5138-5151`, `src/battle_move_resolution.c:236-246`, `src/battle_message.c:424-425`를 `rg`·`sed`로 확인했다.
- 수정 파일은 문서 두 개뿐이며, 정적 확인만 수행했고 ROM 빌드는 하지 않았다. 게임 화면 확인은 mGBA 실행 파일 부재로 미확인이다.

### 2026-09-23 — `STRINGID_DOWNPOURSTARTED` 사용 여부 확인

- `STRINGID_DOWNPOURSTARTED`는 `gMoveWeatherChangeStringIds`의 `B_MSG_STARTED_DOWNPOUR` 매핑에만 남아 있으며 해당 선택자를 설정하는 현재 코드가 없어 HNS의 일반 흐름에서는 출력되지 않는다. 매핑 자체에도 `// Unused` 주석이 있다.
- 일반 비는 `STRINGID_STARTEDTORAIN`, 원시의바다는 `STRINGID_HEAVYRAIN`을 사용한다. `BATTLE_WEATHER_RAIN_DOWNPOUR`도 미사용으로 표시되어 있다.
- 검증: `src/battle_message.c:405,1027-1036,1040-1048`, `src/battle_util.c:128-145,2113-2150,3535-3542`, `include/constants/battle.h:429-449` 및 `B_MSG_STARTED_DOWNPOUR` 참조를 `rg`·`sed`로 확인했다.
- 수정 파일은 문서 두 개뿐이며, 정적 확인만 수행했고 ROM 빌드는 하지 않았다. 게임 화면 확인은 mGBA 실행 파일 부재로 미확인이다.

### 2026-09-23 — `STRINGID_ENDUREDSTURDY`의 옹골참 팝업 순서 확인

- 일반 기술 피해를 옹골참으로 HP 1에서 버틴 `MOVE_RESULT_STURDIED` 분기에서 `BattleScript_SturdiedMsg`가 이 ID를 출력한다. 현재 문장은 `대상 포켓몬은\n공격을 버텼다!` 형태다.
- `BattleScript_SturdiedMsg`가 `BattleScript_AbilityPopUpTarget`을 먼저 호출하므로 옹골참 특성 팝업 후 문장이 출력된다. 일격필살기 방어 문구인 `STRINGID_PKMNPROTECTEDBY`와는 별도 경로다.
- 검증: `src/battle_util.c:8243-8249`, `src/battle_script_commands.c:1382-1389,2125-2144`, `data/battle_scripts_1.s:5415-5420`, `src/battle_message.c:549`를 `rg`·`sed`로 확인했다.
- 수정 파일은 문서 두 개뿐이며, 정적 확인만 수행했고 ROM 빌드는 하지 않았다. 게임 화면 확인은 mGBA 실행 파일 부재로 미확인이다.

### 2026-09-23 — `STRINGID_PKMNPROTECTEDBY` 출력 조건과 특성 팝업 확인

- `STRINGID_PKMNPROTECTEDBY`의 유일한 직접 출력부는 `BattleScript_SturdyPreventsOHKO`다. 옹골참이 일격필살기 판정을 막아 `MOVE_RESULT_ONE_HIT_KO_STURDY`가 설정된 경우에만 이 문구가 나온다.
- `BattleScript_SturdyPreventsOHKO`는 `BattleScript_AbilityPopUp`을 먼저 호출한 뒤 이 ID를 출력하므로 특성 팝업이 먼저 나온다. 일반 기술의 옹골참 생존, 소리·탄환·황금몸 특성 무효화는 다른 ID를 사용한다.
- 검증: `src/battle_util.c:10674-10699`, `src/battle_script_commands.c:1207-1227`, `data/battle_scripts_1.s:6581-6586`, `src/battle_message.c:371`을 `rg`·`sed`로 확인했다.
- 수정 파일은 문서 두 개뿐이며, 정적 확인만 수행했고 ROM 빌드는 하지 않았다. 게임 화면 확인은 mGBA 실행 파일 부재로 미확인이다.

### 2026-09-23 — `STRINGID_PKMNRESTOREDHPUSING` 출력 조건과 특성 팝업 확인

- `BattleScript_MoveHPDrain`에서 이 ID가 직접 출력되며, 축전·저수·건조피부·흙먹기가 각각 전기·물·물·땅 기술을 흡수해 HP를 회복한 성공 분기에서 사용된다. 현재 회복량은 최대 HP의 1/4이다.
- `BattleScript_MoveHPDrain`의 `BattleScript_AbilityPopUp` 호출이 HP 갱신과 문구 출력보다 앞서므로 특성 팝업이 먼저 나온다. HP가 가득 차 있거나 회복 봉인으로 흡수가 회복으로 처리되지 않으면 이 ID는 출력되지 않는다.
- 검증: `src/battle_util.c:2438-2527`, `data/battle_scripts_1.s:2792-2795,6597-6604`, `src/battle_message.c:373`을 `rg`·`sed`로 확인했다.
- 수정 파일은 문서 두 개뿐이며, 정적 확인만 수행했고 ROM 빌드는 하지 않았다. 게임 화면 확인은 mGBA 실행 파일 부재로 미확인이다.

### 2026-09-23 — `STRINGID_PKMNTRACED`의 `{B_BUFF1}` 의미 확인

- Trace 성공 시 `src/battle_util.c:3130`의 `PREPARE_MON_NICK_WITH_PREFIX_LOWER_BUFFER`가 선택한 상대 포켓몬의 닉네임을 `gBattleTextBuff1`에 저장하므로 `{B_BUFF1}`는 상대/야생 포켓몬 이름이다. `PREPARE_ABILITY_BUFFER`로 저장한 복사 능력명은 `{B_BUFF2}`다.
- Trace 능력 팝업이 먼저 나온 뒤 문구가 출력된다. 수정 파일은 문서 두 개뿐이며, 정적 확인만 수행했고 ROM 빌드는 하지 않았다.
- 검증: `src/battle_util.c:3093-3132`, `data/battle_scripts_1.s:6178-6185`, `include/battle_message.h:227-234`, `src/battle_message.c:3211-3221,3856-...`를 `rg`·`sed`로 확인했다.
- 게임 화면 확인: mGBA 실행 파일 부재로 미확인.
- 다음 시작점: 새 HNS ROM에서 Trace를 가진 포켓몬이 상대의 능력을 복사하는 트레이너/야생 배틀을 각각 재현한다.

### 2026-09-23 — 울퉁불퉁멧의 `STRINGID_PKMNHURTSWITH` 실제 치환 확인

- `TryRockyHelmet`의 접촉 반동 조건과 `BattleScript_RockyHelmetActivates`의 순서를 재확인했다. 울퉁불퉁멧 팝업 후 `BattleScript_HurtAttacker`가 같은 ID를 출력하며, 특성 팝업은 없다.
- `B_DEF_ABILITY`는 `gBattlerTarget`의 특성명을 가져오므로 현재 문구는 울퉁불퉁멧이 아니라 보유자의 실제 특성명을 원인처럼 출력한다. 예: `상대 포켓몬의 [특성] 때문에\n우리 포켓몬은 상처를 입었다!`
- 수정 파일: `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`만 갱신했다.
- 검증: `TryRockyHelmet`, `BattleScript_RockyHelmetActivates`, `BattleScript_HurtAttacker`, `B_TXT_DEF_ABILITY` 토큰 처리부를 `rg`·`sed`로 확인했다. ROM 빌드는 하지 않았다.
- 게임 화면 확인: mGBA 실행 파일 부재로 미확인.
- 다음 시작점: 울퉁불퉁멧 전용 문자열/아이템 토큰을 추가할지 결정한 뒤 `src/battle_hold_effects.c:245-261`, `data/battle_scripts_1.s:6956-6964`, `src/battle_message.c:381`을 수정·빌드한다.

### 2026-09-23 — `STRINGID_PKMNHURTSWITH` 출력 경로와 팝업 순서 확인

- `BattleScript_HurtAttacker`가 HP 갱신 후 `STRINGID_PKMNHURTSWITH`를 출력한다. 거친피부·철가시, 바위가시, 자보열매·애슈열매, 가시방패 반동 피해가 이 공통 스크립트를 사용한다.
- 거친피부·철가시는 접촉 기술이 실제로 명중해 공격자가 살아 있고 반동 효과를 피하지 못할 때 발생한다. `BattleScript_RoughSkinActivates`의 특성 팝업이 먼저 출력된다. 울퉁불퉁멧은 아이템 팝업, 자보·애터열매는 리펜 보유 시 리펜 팝업이 먼저 나온다. 가시방패는 특성 팝업 없이 직접 문구를 출력한다.
- 현재 메시지 정의의 `{B_DEF_ABILITY}`는 능력 반동 경로에는 적합하지만 아이템·가시방패 경로에도 그대로 적용되므로, 원인별 문구를 정확히 분리하려면 별도 문자열/스크립트 검토가 필요하다.
- 수정 파일: `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`만 갱신했다.
- 검증: 문자열 정의, `BattleScript_HurtAttacker`, 거친피부·철가시 처리, 바위가시·자보/애슈열매·가시방패 스크립트를 `rg`·`sed`로 정적 확인했다. ROM 빌드는 하지 않았다.
- 게임 화면 확인: mGBA 실행 파일 부재로 미확인.
- 다음 시작점: `src/battle_util.c:4047-4065`, `data/battle_scripts_1.s:6943-6975,7630-7642` 및 새 HNS ROM에서 능력·아이템·가시방패 경로를 각각 재현한다.

### 2026-09-23 — `STRINGID_PKMNCUTSATTACKWITH` 출력 조건과 특성 팝업 확인

- `BattleScript_IntimidateEffect`가 위협(`ABILITY_INTIMIDATE`)으로 대상의 공격을 실제로 낮춘 성공 분기에서 이 ID를 출력한다. 현재 직접 출력부는 `data/battle_scripts_1.s:6299`다.
- 교체 출전한 위협의 특성 팝업이 먼저 표시되고, 여러 상대의 공격이 내려가면 대상별로 이 문구가 반복된다. 공격이 최저 단계이거나 위협을 막는 경우에는 다른 문구가 사용된다.
- 현재 문장 정의는 `{B_SCR_NAME_WITH_PREFIX}의 {B_SCR_ABILITY} 때문에\n{B_DEF_NAME_WITH_PREFIX}의 공격력이 떨어졌다!`다.
- 수정 파일: `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`만 갱신했다.
- 검증: 문자열 정의, `BattleScript_IntimidateActivates`·`BattleScript_IntimidateEffect`, 위협 능력 호출부와 실패 분기를 `rg`·`sed`로 정적 확인했다. ROM 빌드는 하지 않았다.
- 게임 화면 확인: mGBA 실행 파일 부재로 미확인.
- 다음 시작점: `data/battle_scripts_1.s:6285-6326`, `src/battle_util.c:3451-3458` 및 새 HNS ROM에서 위협 발동·방어 특성·공격 최저 단계를 각각 재현한다.

### 2026-09-23 — `STRINGID_PKMNANCHORSITSELFWITH` 출력 조건과 특성 팝업 확인

- `BattleScript_AbilityPreventsPhasingOutRet`가 흡반(`ABILITY_SUCTION_CUPS`)으로 교체를 막을 때 이 ID를 출력한다. 날려버리기·울부짖기, 배대뒤치기·드래곤테일의 해당 경로가 포함된다.
- 이 스크립트는 `AbilityPopUpTarget` 후 `STRINGID_PKMNANCHORSITSELFWITH`를 실행하므로, 해당 네 기술 경로에서는 흡반 특성 팝업이 먼저 출력된다.
- 레드카드의 흡반 분기(`BattleScript_RedCardSuctionCups`)도 같은 ID를 출력하지만 능력 팝업은 호출하지 않고 레드카드 아이템 발동 흐름 뒤에 바로 문구를 출력한다.
- 수정 파일: `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`만 갱신했다.
- 검증: 문자열 정의, `BattleScript_EffectRoar`, `EFFECT_HIT_SWITCH_TARGET` 처리, `BattleScript_AbilityPreventsPhasingOutRet`, 레드카드 분기를 `rg`·`sed`로 정적 확인했다. ROM 빌드는 하지 않았다.
- 게임 화면 확인: mGBA 실행 파일 부재로 미확인.
- 다음 시작점: `data/battle_scripts_1.s:2540-2546,6630-6639,7775-7796`, `src/battle_move_resolution.c:3129-3154` 및 새 HNS ROM에서 네 기술과 레드카드 경로를 각각 재현한다.

### 2026-09-23 — `STRINGID_PKMNPREVENTSUSAGE` 현재 문장 재확인

- 현재 문장 정의는 `src/battle_message.c:372`의 `{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_CURRENT_MOVE}{B_TXT_EULREUL} 쓸 수 없다!`다. 특성명과 `때문에`는 문자열 본문에 없고, 앞서 실행되는 습기 특성 팝업에서 표시된다.
- `ABILITY_DAMP`가 필드에 있고 자폭·대폭발·깜짝헤드·미스트버스트 중 하나를 사용하면 `CancelerExplodingDamp`가 `BattleScript_DampStopsExplosion`으로 보내며, 습기 팝업 뒤 이 ID를 출력한다. 기술 실행은 취소된다.
- 수정 파일: `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`만 갱신했다.
- 검증: 문자열 정의, `CancelerExplodingDamp`, `BattleScript_DampStopsExplosion`, `dampBanned` 기술 목록을 `rg`·`sed`로 정적 재확인했다. ROM 빌드는 하지 않았다.
- 게임 화면 확인: mGBA 실행 파일 부재로 미확인.
- 다음 시작점: `src/battle_move_resolution.c:1459-1466`, `data/battle_scripts_1.s:6588-6595`, 새 HNS ROM에서 습기와 네 폭발 기술의 메시지 순서를 재현한다.

### 2026-09-23 — `STRINGID_PKMNREGAINEDHEALTH` 출력 조건 확인

- 생명의물방울, 치유파동·플라워힐, HP회복 계열, 알낳기·우유마시기, 꿀꺽, 희망사항, 프레젠트 회복, 정글힐·초승달의기도, Earth Eater의 HP 회복 성공 뒤에 출력된다.
- 회복 대상은 `{B_DEF_NAME_WITH_PREFIX}`이며, 회복 실패·HP 최대 상황에서는 이 ID가 출력되지 않는다. 정글힐의 상태이상만 치료하는 경우에는 `STRINGID_PKMNSTATUSNORMAL`이 사용된다.
- 이번 확인에서는 소스·데이터·ROM 수정과 빌드를 하지 않았다.

### 2026-09-23 — `STRINGID_PKMNPREVENTSUSAGE` 출력 조건 확인

- `CancelerExplodingDamp`가 습기 보유 포켓몬이 있는 상태에서 자폭·대폭발·깜짝헤드·미스트버스트 사용을 감지하면 `BattleScript_DampStopsExplosion`으로 이동한다. 이 스크립트가 특성 팝업 후 `STRINGID_PKMNPREVENTSUSAGE`를 출력한다.
- 따라서 문장은 `습기 보유 포켓몬의 습기 때문에 사용 포켓몬은 해당 폭발 기술을 할 수 없다!` 의미가 되며, 기술 실행과 폭발 피해는 취소된다. 이번 확인에서는 소스·데이터·ROM 수정과 빌드를 하지 않았다.

### 2026-09-22 — `STRINGID_PKMNSTATUSNORMAL` 출력 조건 확인

- 현재 직접 호출부는 브레이브차지, 리프레시, 정글힐·초승달의기도, 사이코시프트의 상태 회복 경로다. 실제 상태이상이 치료된 경우에만 해당 문구가 나온다.
- 출력 주체는 `{B_ATK_NAME_WITH_PREFIX}`이며, 정글힐은 각 치료 대상이 해당 슬롯에 들어가고 사이코시프트는 치료된 사용 포켓몬이 공격자 슬롯에 남는다. Healer·Pastel Veil·Purify 등은 별도 문구를 사용한다.
- 이번 확인에서는 소스·데이터·ROM 수정과 빌드를 하지 않았다.

### 2026-09-22 — 특성 상태이상 문구와 화염구슬·맹독구슬 문구 분리 및 HNS 빌드

- 특성으로 수면·독·화상·마비가 발생할 때 각각 `STRINGID_PKMNFELLASLEEP`, `STRINGID_PKMNWASPOISONED`, `STRINGID_PKMNWASBURNED`, `STRINGID_PKMNWASPARALYZED`를 선택하도록 테이블을 변경했다. 특성 팝업 호출 순서는 유지했다.
- 화염구슬·맹독구슬은 공통 상태이상 처리 대신 각각 `STRINGID_PKMNBURNEDBY`·`STRINGID_PKMNPOISONEDBY`를 직접 출력하도록 `BattleScript_FlameOrb`·`BattleScript_ToxicOrb`를 분리했다. 아이템 팝업과 상태 애니메이션은 유지했다.
- `make hns -j8` 성공. 로그는 `build/localization-logs/hns-status-message-selection-20260922.log`, 메모리 사용량은 EWRAM `249,012`, IWRAM `25,704`, ROM `33,330,148/33,554,432 bytes`다. mGBA 부재로 실제 화면은 확인하지 못했다.

### 2026-09-22 — 특성 수면 메시지 전 특성 팝업 순서 재확인

- 특성 수면 스크립트는 `BattleScript_AbilityPopUp`을 먼저 호출한 뒤 `setnonvolatilestatus TRIGGER_ON_ABILITY`를 실행하므로, 특성 팝업 후 `STRINGID_PKMNMADESLEEP`가 출력된다. 소스·데이터·ROM 수정과 빌드는 하지 않았다.

### 2026-09-22 — `STRINGID_PKMNMADESLEEP`의 HNS 출력 여부 확인

- `gFellAsleepStringIds[B_MSG_STATUSED_BY_ABILITY]`가 `STRINGID_PKMNMADESLEEP`를 선택하므로, 특성으로 수면이 발생하면 HNS `GEN_LATEST`에서도 출력될 수 있다. 특성 팝업 후 상태이상 문구가 나오는 구조다.
- `ABILITY_EFFECT_SPORE`가 접촉한 공격자를 재우는 경로에서 `BattleScript_AbilityStatusEffect`를 호출하며, 일반 기술 수면은 `STRINGID_PKMNFELLASLEEP`를 사용한다. `//not in gen 5+`는 차단 조건이 아니다.
- 이번 확인에서는 소스·데이터·ROM 수정과 빌드를 하지 않았고, mGBA 부재로 실제 화면은 확인하지 못했다.

### 2026-09-22 — 화염구슬 화상 메시지에서 사용할 이름 토큰 확인

- `BattleScript_FlameOrb`가 `gEffectBattler = gBattlerAttacker`로 설정하므로 현재 화염구슬 자기 화상에서는 `{B_EFF_NAME_WITH_PREFIX}`와 `{B_ATK_NAME_WITH_PREFIX}`의 실제 출력값이 같다.
- 그래도 효과를 받은 포켓몬을 주어로 삼는 문장이므로 `{B_EFF_NAME_WITH_PREFIX}`가 의미상 올바른 토큰이다. `{B_ATK_NAME_WITH_PREFIX}`는 공격자 역할을 나타낸다.
- 현재 출력 순서는 아이템 팝업 후 `STRINGID_PKMNWASBURNED`의 일반 화상 문구이며, `화염구슬 때문에`라는 문구는 아직 출력하지 않는다. 이번 확인에서는 소스·데이터·ROM 수정과 빌드를 하지 않았다.

### 2026-09-22 — `{B_EFF_NAME_WITH_PREFIX}`의 의미와 출력 형태 확인

- `B_TXT_EFF_NAME_WITH_PREFIX`는 `gEffectBattler`의 닉네임을 읽어 출력하며, 플레이어 편에는 접두사가 없고 트레이너 상대에는 `상대 `, 야생 상대에는 `야생 `을 붙인다. 따라서 닉네임 `피카츄`는 `피카츄`·`상대 피카츄`·`야생 피카츄`로 확장된다.
- `gEffectBattler`는 현재 효과를 받은 포켓몬을 가리킨다. 독·화상 문자열에서는 상태이상을 받은 포켓몬이고, 특성명/발동 주체는 `{B_SCR_NAME_WITH_PREFIX}`·`{B_SCR_ABILITY}`가 별도로 표시한다. 환상 중이면 표시용 닉네임을 고려한다.
- `{B_EFF_NAME_WITH_PREFIX2}`는 같은 대상의 소문자/문장 중간용 변형이다. 이번 확인에서는 소스·데이터·ROM 수정과 빌드를 하지 않았고, mGBA 부재로 실제 화면은 확인하지 못했다.

### 2026-09-22 — Rest 성공 메시지를 `STRINGID_PKMNSLEPTHEALTHY`로 통일 및 HNS ROM 재빌드

- `gRestUsedStringIds[B_MSG_REST]`를 `STRINGID_PKMNSLEPTHEALTHY`로 바꿔 `B_MSG_REST_STATUSED`와 같은 문구를 사용하게 했다. 따라서 Rest가 성공하면 상태 이상이 있었는지와 관계없이 사용자가 번역한 `잠이 들어 건강해졌다!`가 출력된다.
- 상태 이상 제거와 수면 부여 로직은 유지했다. `STRINGID_PKMNWENTTOSLEEP` 정의는 남아 있지만 Rest 성공 경로에서 더 이상 선택되지 않는다.
- 대상 오브젝트 및 전체 HNS 빌드 성공. EWRAM 249,012/262,144, IWRAM 25,704/32,768, ROM 33,328,612/33,554,432. GBA SHA-256 `8b33ebd0d16a8fab1c794911c61443f2a33488d472153cb7639403a915d3270c`, ELF SHA-256 `88a806049354b7ce047e7334d5a7387f7307835967a2b0dc73a247d296b755c0`; 로그 `build/localization-logs/hns-rest-healthy-message-20260922.log`.
- 전체 `git diff --check`에는 기존 변경 파일의 공백 경고가 있었고 이번 수정 줄에는 새 경고가 없다. mGBA 부재로 화면 검증은 하지 못했다.

### 2026-09-22 — HNS Rest 성공 메시지 형태 재확인

- HNS의 Rest 성공 문구는 상태 이상이 없으면 `포켓몬은\n잠을 자기 시작했다!` 형식이고, 수면 이외의 상태 이상을 치료하면 `포켓몬은\n건강한 상태로 잠을 자기 시작했다!` 형식이다. 실제 이름과 `은/는`은 `{B_ATK_NAME_WITH_PREFIX}`·`{B_TXT_EUNNEUN}`이 확장한다.
- 이번 확인에서는 소스 수정과 빌드를 하지 않았다.

### 2026-09-22 — Rest 수면 시작 문자열 출력 조건 확인

- `data/battle_scripts_1.s`의 `BattleScript_EffectRest`가 성공적으로 `trysetrest`를 실행한 뒤 `gRestUsedStringIds`를 출력한다. `src/battle_message.c`의 테이블에서 `B_MSG_REST`는 `STRINGID_PKMNWENTTOSLEEP`, `B_MSG_REST_STATUSED`는 `STRINGID_PKMNSLEPTHEALTHY`에 연결되어 있다.
- `Cmd_trysetrest`는 대상 포켓몬의 상태에 `STATUS1_SLEEP` 이외의 상태 비트가 있으면 건강한 상태 문구를 선택하고, 그렇지 않으면 일반 수면 문구를 선택한다. 이후 상태를 수면으로 바꾸고, 메시지·상태 아이콘 갱신 뒤 회복을 진행한다.
- 이미 잠듦·체력 최대·소란피기·수면 방지 특성·필드 방해 분기에서는 이 두 ID가 출력되지 않는다. 이번 확인에서는 소스 수정과 빌드를 하지 않았다.

### 2026-09-22 — Rest 특성 방해 메시지를 `STRINGID_ITDOESNTAFFECT`로 변경 및 HNS ROM 재빌드

- 요청에 따라 `data/battle_scripts_1.s`의 `BattleScript_InsomniaProtects`에서 `printstring STRINGID_PKMNSTAYEDAWAKEUSING`을 `printstring STRINGID_ITDOESNTAFFECT`로 교체했다. 이 스크립트는 Rest의 불면·의기양양·정화의소금 분기에서 공통으로 호출된다.
- `BattleScript_AbilityPopUp` 호출, `MOVE_RESULT_FAILED` 설정, 대기 시간은 그대로 두었다. 새 출력은 특성 팝업 뒤 대상 무효 문구가 된다. `rg`로 확인한 결과 이전 ID는 문자열 정의·`battle_arena.c` 점수 처리 외의 배틀 스크립트 호출이 남아 있지 않다.
- `git diff --check`와 전체 HNS 빌드 성공. EWRAM 249,012/262,144, IWRAM 25,704/32,768, ROM 33,328,612/33,554,432. GBA SHA-256 `130f9cea025095bd6e9dc94bad9ef6b4cfa063a4db917da6938ed526a387044c`, ELF SHA-256 `100c90ac3077b89c6f9049780afc7dfdd288442b6dff03b6f2b7ec1e4aa280cf`; 로그 `build/localization-logs/hns-rest-no-effect-20260922.log`.
- mGBA 부재로 실제 화면 출력은 확인하지 못했다.

### 2026-09-22 — `STRINGID_ITDOESNTAFFECT`와 `STRINGID_SCR_ITDOESNTAFFECT` 구분

- `src/battle_message.c`의 `{B_DEF_NAME_WITH_PREFIX}`는 `gBattlerTarget`을, `{B_SCR_NAME_WITH_PREFIX}`는 `gBattleScripting.battler`를 이름과 접두사 확장에 사용한다. 따라서 단일 배틀에서 두 슬롯이 같으면 결과가 같아 보일 수 있지만, 더블 배틀이나 특수 스크립트에서는 서로 다른 전투원을 가리킬 수 있다.
- `data/battle_scripts_1.s`에서 일반 대상 무효 메시지는 `STRINGID_ITDOESNTAFFECT`, 가루 기술·특성 무효처럼 `BattleScript_DoesntAffectScripting`으로 이어지는 경로는 `STRINGID_SCR_ITDOESNTAFFECT`를 출력한다. 이번 확인에서는 소스 수정과 빌드를 하지 않았다.

### 2026-09-22 — `STRINGID_PKMNSTAYEDAWAKEUSING` 출력 조건 확인

- `EFFECT_REST`의 사전 판정에서 사용 포켓몬의 특성이 불면·의기양양·정화의소금이면 `BattleScript_InsomniaProtects`를 선택한다. 배틀 스크립트의 Rest 분기에도 불면·의기양양·정화의소금 검사가 남아 있다.
- 해당 스크립트는 `call BattleScript_AbilityPopUp` 후 `printstring STRINGID_PKMNSTAYEDAWAKEUSING` 순서이므로 특성 팝업이 먼저 표시된다. 이번 확인에서는 소스 수정과 빌드를 하지 않았다.

### 2026-09-22 — `gText_PkmnsXPreventsSwitching` 출력 조건 확인

- `src/party_menu.c`에서 교체 시도 대상이 `PARTY_ACTION_ABILITY_PREVENTS`이면 `SetMonPreventsSwitchingString()`이 이 문자열을 확장한다. 교체 방지 특성 보유 포켓몬과 현재 선택 포켓몬을 각각 버퍼에 넣는다.
- `{B_BUFF1}`은 방해 포켓몬 닉네임·접두사, `{B_LAST_ABILITY}`는 교체를 막은 특성명, `{B_BUFF2}`는 교체 대상 닉네임, `{B_TXT_EULREUL}`은 을/를 조사다. 이번 확인에서는 소스 수정과 빌드를 하지 않았다.

### 2026-09-22 — `{PLAY_SE 0x0011}` 제어 코드 확인

- `PLAY_SE`는 문자열 출력 중 `PlaySE()`를 호출하는 확장 제어 코드이며, `0x0011`은 `include/constants/songs.h`의 `SE_FLEE`(17)다.
- 따라서 도망 메시지 시작 부분에서 도망 효과음을 재생하고 글자 출력은 계속한다. 화면에 `0x0011`이 표시되거나 별도 텍스트가 추가되지는 않는다. 이번 확인에서는 소스 수정과 빌드를 하지 않았다.

### 2026-09-22 — `//not in gen 5+` 주석의 의미 확인

- 해당 표기는 세대별 본가 메시지 차이를 설명하는 주석이다. 주석 자체에는 코드 실행 효과가 없으므로 HNS에서 문자열 출력을 막지 않는다.
- 출력 여부는 실제 배틀 스크립트 호출과 세대 조건·설정 분기로 결정된다. 이번 확인에서는 소스 수정과 빌드를 하지 않았다.

### 2026-09-22 — 흰안개·신비의부적 안개제거 해제 메시지 연결 및 HNS ROM 재빌드

- 요청/범위: 신비의부적이 안개제거로 제거될 때 `STRINGID_PKMNSAFEGUARDEXPIRED`를 출력하고, 사용자가 추가한 `STRINGID_NOLONGERMIST`를 흰안개의 턴 종료·안개제거 해제에 연결.
- `include/constants/battle_string_ids.h`에 `STRINGID_NOLONGERMIST`를 등록했다. `src/battle_end_turn.c`의 흰안개 만료는 `BattleScript_MistWoreOff`, `src/battle_script_commands.c`의 안개제거는 `BattleScript_MistWoreOffReturn`으로 연결했다.
- 안개제거의 신비의부적 분기는 `BattleScript_SafeguardEndsReturn`으로 바꿔 `STRINGID_PKMNSAFEGUARDEXPIRED`를 사용한다. 신비의부적 자연 만료는 기존 `BattleScript_SafeguardEnds`를 유지한다. 문자열 본문은 사용자가 작성한 그대로 두었다.
- 검증: 변경 오브젝트 빌드와 전체 HNS 빌드가 모두 종료 코드 0. EWRAM 249,012/262,144, IWRAM 25,704/32,768, ROM 33,328,612/33,554,432. GBA SHA-256 `74f19d0406e68947195e811aed36d3fa73387ffe6e7f236e84175c78d2c0654b`, ELF SHA-256 `8653742c2445fa49eaf5b4362c4097d195b3d1c6308a20ad64936df7b7e3e4da`; 로그 `build/localization-logs/hns-mist-safeguard-expiry-20260922.log`.
- mGBA 실행 파일이 없어 실제 게임 화면 검증은 하지 못했다.

### 2026-09-22 — 리플렉터·빛의장막 표기와 `{B_ATK_TEAM1}` 출력 구분

- 이전 표의 `우리 편`/`상대 편`은 진영을 설명하는 라벨이고, 실제 삽입값은 `{B_ATK_TEAM1}`의 `우리`/`상대`다. 그러므로 `{B_ATK_TEAM1}의`가 만드는 `우리의`/`상대의`는 현재 소스와 일치한다.
- `{B_ATK_PREFIX1}의`를 쓰면 `우리 편의`/`상대의`가 된다. 이번 확인에서는 소스 수정과 빌드를 하지 않았다.

### 2026-09-22 — `B_ATK_PREFIX` 조사 형태 확인

- `B_ATK_PREFIX1`은 `우리 편`/`상대`, `B_ATK_PREFIX2`는 `우리 편은`/`상대는`, `B_ATK_PREFIX3`은 `우리 편을`/`상대를`로 확장된다. 따라서 별도 소유격 ID 없이 `{B_ATK_PREFIX1}의`를 사용해 `우리 편의`/`상대의`를 만들 수 있다.
- `{B_ATK_TEAM1}의`는 `우리의`/`상대의`를 만든다. 대응하는 `B_DEF_PREFIX1~3`는 방어 측 포켓몬 기준이다. 이번 확인에서는 소스 수정과 빌드를 하지 않았다.

### 2026-09-22 — `STRINGID_PKMNCOVEREDBYVEIL` 출력 조건 확인

- `gReflectLightScreenSafeguardStringIds[B_MSG_SET_SAFEGUARD]`가 이 ID를 가리킨다. 신비의부적 성공 시 `BattleScript_EffectSafeguard`가, 오로라베일 성공 시 `BattleScript_MoveEffectAuroraVeil`이 같은 테이블을 출력한다.
- `{B_ATK_PREFIX2}`는 우리 편에서 `우리 편은`, 상대 편에서 `상대는`으로 확장된다. 결과는 각각 `우리 편은\n신비의 베일에 둘러싸였다!`와 `상대는\n신비의 베일에 둘러싸였다!`이다. 이미 설치된 상태에서 재사용하면 `STRINGID_BUTITFAILED` 경로다.
- 리플렉터와 빛의장막은 이 ID를 사용하지 않는다. 이번 확인에서는 소스 수정과 빌드를 하지 않았다.

### 2026-09-22 — `STRINGID_PKMNSXWOREOFF` 현재 출력 경로 확인

- `BattleScript_SideStatusWoreOff`는 턴 종료로 안개가 만료될 때 호출되며 `STRINGID_PKMNSXWOREOFF`를 출력한다. 안개제거가 안개 또는 신비의부적을 해제할 때는 반환형 `BattleScript_SideStatusWoreOffReturn`이 같은 ID를 사용한다.
- 신비의부적의 일반적인 턴 종료 만료는 `STRINGID_PKMNSAFEGUARDEXPIRED`를 사용한다. 리플렉터·빛의장막·오로라베일은 앞선 작업에서 전용 해제 ID로 분리되어 더 이상 이 문자열을 사용하지 않는다.
- `{B_ATK_PREFIX1}`은 `우리 편`/`상대`, `{B_BUFF1}`은 `안개` 또는 `신비의부적` 같은 해제된 기술명으로 확장된다. 이번 확인에서는 소스 수정과 빌드를 하지 않았다.

### 2026-09-22 — 리플렉터·빛의장막 해제 메시지 연결 및 HNS ROM 재빌드

- 요청/범위: 리플렉터와 빛의장막이 끝날 때 generic `STRINGID_THEWALLSHATTERED`가 아닌 사용자가 추가한 전용 문자열을 출력하도록 연결하고, 출력 주체가 우리 편인지 상대 편인지 확인.
- `include/constants/battle_string_ids.h`에 `STRINGID_REFLECTWOREOFF`·`STRINGID_LIGHTSCREENWOREOFF`를 등록했다. 사용자가 함께 추가한 `STRINGID_AURORAVEILWOREOFF`도 등록했다. `src/battle_end_turn.c`의 턴 종료 만료와 `src/battle_script_commands.c`의 Defog 해제는 각각 전용 `...WoreOff`/`...WoreOffReturn` 스크립트를 사용한다.
- `data/battle_scripts_1.s`의 `BattleScript_BreakScreens`는 `gBreakScreensStringIds`를 참조한다. 단일 리플렉터·빛의장막·오로라베일 제거에는 각 전용 문구를, 복수 화면 제거에는 기존 `STRINGID_THEWALLSHATTERED`를 선택한다. 따라서 `THEWALLSHATTERED`는 일반 타이머 만료가 아니라 즉시 화면 파괴의 generic 복수 분기에 남는다.
- `{B_ATK_TEAM1}`은 `B_ATK`가 우리 쪽이면 `우리`, 상대 쪽이면 `상대`로 확장된다. 싱글·더블 모두 `우리의/상대의 리플렉터가 없어졌다!`, `우리의/상대의 빛의장막이 없어졌다!` 형태이며, 오로라베일도 같은 규칙이다.
- `git diff --check`와 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 성공(종료 코드 0). EWRAM 249,012/262,144, IWRAM 25,704/32,768, ROM 33,328,580/33,554,432. GBA SHA-256 `a3afa978d0124b1fc6b48b93be4378c1ab1b5c4f5e5c6939090ba4caf3e454e9`, ELF SHA-256 `50b7124f96fc49bddfcca883d7641bb6517baf59b05d8ed6a23882b1b45046c0`; 로그 `build/localization-logs/hns-screen-woreoff-20260922.log`.
- mGBA 실행 파일이 없어 실제 화면 검증은 하지 못했다. 새 ROM에서 세 해제 경로와 단일·복수 화면 조합을 재현한다.

### 2026-09-21 — `STRINGID_ITEMSCANTBEUSEDNOW` 출력 조건 확인

- `src/battle_main.c`의 아이템 행동 선택 분기에서 가방 제한 변수로 사용이 금지되거나 링크·프런티어·e-Reader·기록 링크 배틀, Sky Drop 상태이면 `BattleScript_ActionSelectionItemsCantBeUsed`를 실행한다.
- 이 문구에는 세대 조건이 없다. 기본 `B_VAR_NO_BAG_USE = 0`에서는 일반 배틀의 가방 사용 제한으로 출력되지 않는다. 소스는 수정하지 않았다.

### 2026-09-21 — 세대 기준(`GEN_LATEST`)과 햇살 메시지 관계 확인

- `general.h`의 세대 상수와 `GEN_LATEST`는 여러 전투 규칙의 기본값을 정하지만, 햇살 능력 발동 메시지에는 세대 조건이 없다.
- CHAMPIONS에서 바뀌는 `B_ABILITY_WEATHER`는 능력 날씨 지속 시간 관련 설정이고, 날씨 변경 성공 후 팝업과 `STRINGID_PKMNSXINTENSIFIEDSUN`을 출력하는 흐름은 그대로다.

### 2026-09-21 — 햇살 강화 메시지의 CHAMPIONS 설정 동작 확인

- `ABILITY_DROUGHT`·`ABILITY_ORICHALCUM_PULSE`의 날씨 변경 성공 시 특성 팝업 뒤 `STRINGID_PKMNSXINTENSIFIEDSUN`이 출력된다. CHAMPIONS 기준의 `GEN_LATEST` 설정이 이 메시지를 억제하지 않는다.
- `ABILITY_DESOLATE_LAND`는 다른 메시지 ID를 사용한다. 소스는 수정하지 않았다.

### 2026-09-21 — `STRINGID_FOREWARNACTIVATES` 출력 경로 확인

- `STRINGID_FOREWARNACTIVATES`는 `ABILITY_FOREWARN`(예지몽)의 교체 출전 메시지다. 상대가 있을 때 선택된 상대 기술을 예지몽이 알려 주며, 특성 팝업 다음에 출력된다.
- `ABILITY_ANTICIPATION`(위험예지)의 `STRINGID_ANTICIPATIONACTIVATES`와 혼동하지 않도록 구분했다. 문구는 영어 상태이고 소스는 수정하지 않았다.

### 2026-09-21 — `STRINGID_AFTERMATHDMG` 출력 경로 확인

- `src/battle_util.c`의 `ABILITY_AFTERMATH` 분기에서 접촉 기술로 보유자가 쓰러지고 공격자가 생존한 경우 `BattleScript_AftermathDmg`를 호출한다. 이 스크립트가 특성 팝업 뒤 `STRINGID_AFTERMATHDMG`를 출력한다.
- `ABILITY_INNARDS_OUT`도 같은 스크립트를 호출하므로 최후의발악 피해 뒤에도 재사용된다. 현재 문구는 영어 상태이며 코드 수정은 하지 않았다.

### 2026-09-21 — 능력치 단계 문구 앞 공백 복원 및 HNS ROM 재빌드

- `{B_BUFF2}` 앞 공백을 제거했던 이전 수정 때문에 `공격이 크게`가 `공격이`와 `크게` 사이 공백 없이 출력되는 회귀를 확인했다. `{B_BUFF2}` 문자열 자체의 후행 공백은 `크게 올라갔다!`에서 `크게` 뒤만 담당하므로, 관련 문구의 `{B_TXT_IGA} {B_BUFF2}` 앞 공백을 복원했다.
- 대상 오브젝트 빌드와 전체 HNS 빌드가 종료 코드 0으로 완료됐다. ROM 사용량은 EWRAM 249,012/262,144, IWRAM 25,704/32,768, ROM 33,328,404/33,554,432(99.33%)이다.
- 새 `pokehns.gba` SHA-256은 `bd151d458f172d3823a19f658d8945ec6b33b99816b32640ee3f44b91bedbb91`이다. mGBA가 없어 실제 화면 검증은 하지 못했다.

### 2026-09-21 — 배틀 메시지 유사 글리프 충돌 정적 점검

- `src/battle_message.c`의 `CHAR_NBSP` 사용 지점과 한글 플레이스홀더 주변 공백을 전수 검색했다. `CHAR_NBSP` 변환은 플레이스홀더 복사 공통 경로에만 있고, 한글 선행 바이트가 뒤따를 때 일반 공백으로 보존된다.
- 하드코딩된 `CHAR_NBSP`/`0x39` 및 수정 전 `{B_TXT_IGA} {B_BUFF2}` 패턴은 발견되지 않았다. `B_TXT_I 0x39`는 `include/battle_message.h`의 플레이스홀더 토큰 정의이므로 텍스트 바이트가 아니다.
- 이번 문제와 같은 정적 원인은 추가로 확인되지 않았다. `git diff --check` 통과. 정적 점검만 수행했으며 mGBA 부재로 실제 화면 출력은 확인하지 못했다.

### 2026-09-21 — 능력치 상승 문구 글리프 깨짐 수정 및 HNS ROM 재빌드

- `{B_BUFF2}`의 `크게 ` 끝 공백이 `CHAR_NBSP(0x39)`로 바뀌고, 이 값이 한글 선행 바이트로 다시 해석되는 문제를 확인했다. `올(0x3D 0x0B)`과 결합해 `드(0x39 0x3D)`처럼 보이고 이후 바이트가 밀리는 원인이었다.
- 플레이스홀더 공백이 뒤의 한글 선행 바이트를 흡수하지 않도록 `src/battle_message.c`의 복사 조건을 수정했다. 능력치 상승·하락 본문에서 `{B_BUFF2}` 앞의 리터럴 공백도 제거했다. 이제 `크게 `의 공백 하나만 남는다.
- `git diff --check -- src/battle_message.c` 통과. 오브젝트 컴파일과 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 전체 빌드 모두 종료 코드 0.
- HNS ROM 사용량: EWRAM 249,012/262,144, IWRAM 25,704/32,768, ROM 33,328,388/33,554,432(99.33%). `pokehns.gba` SHA-256 `4acef6dc44d13ee39829fbdee023dbfbb2d09b7b414294e8b6296323c334d59a`; `pokehns.elf` SHA-256 `409da26ae9412e0be682dee5d4ed76a80986ef7128345836658e5bf2b2a7319e`.
- mGBA가 없어 실제 화면 검증은 하지 못했다. 새 ROM으로 2단계 능력치 상승 상황을 확인한다.

### 2026-09-21 — 능력치 상승 문구 수정 후 HNS ROM 재빌드

- 사용자의 최신 `src/battle_message.c` 상태를 기준으로 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8`를 다시 실행했다. 빌드·링크·`gbafix`가 종료 코드 0으로 완료됐다.
- `pokehns.gba` SHA-256: `eeeadd734595ba180fe8413b9e8c3fad0bf56872214107ff7af3a94e189be97b`; `pokehns.elf` SHA-256: `74a2658080f7be4d5f32f701b59a2bd503118d0cc98d8bd6c3917d288b2f82ac`. EWRAM 249,012/262,144, IWRAM 25,704/32,768, ROM 33,328,628/33,554,432(99.33%).
- 소스·그래픽은 이번 단계에서 추가 수정하지 않았다. mGBA 실행 파일이 없어 실제 전투 화면의 `크게 올라갔다!` 글리프는 확인하지 못했으며, 새 ROM에서 같은 상황을 재현해야 한다.

### 2026-09-21 — 능력치 상승 조사 수정 확인

- 사용자가 `STRINGID_TARGETABILITYSTATRAISE`와 `STRINGID_ATTACKERABILITYSTATRAISE`의 고정 조사 `가`를 `{B_TXT_IGA}`로 수정했다.
- 두 문구가 `{B_BUFF1}{B_TXT_IGA} {B_BUFF2}올라갔다!`로 출력되며, `src/battle_message.c`에 `{B_BUFF1}가` 패턴이 남아 있지 않음을 확인했다.
- `git diff --check -- src/battle_message.c` 통과. `make BUILD=hns build/hns/src/battle_message.o -j2` 종료 코드 0. 전체 ROM 빌드는 실행하지 않았다.

### 2026-09-21 — 능력치 상승·하락 배틀 메시지 공백 검토

- 요청/범위: 사용자가 수정한 능력치 상승·하락 및 유사 배틀 메시지의 `{B_BUFF2}` 주변 공백을 확인.
- `gText_StatSharply`와 `STRINGID_STATHARSHLY`의 후행 공백을 고려해 관련 본문이 `{B_BUFF2}올라갔다!`·`{B_BUFF2}떨어졌다!` 형태인지 점검했다. 일반 능력치 상승·하락, 아이템 상승·하락 문구는 중복 공백 없이 연결된다.
- `STRINGID_TARGETABILITYSTATRAISE`와 `STRINGID_ATTACKERABILITYSTATRAISE`는 공백은 수정됐지만 `{B_BUFF1}가`가 고정되어 있다. 공격·특수공격에는 `이`가 필요하므로 `{B_BUFF1}{B_TXT_IGA}`가 후속 수정안이다. 이번에는 코드에 손대지 않았다.
- `git diff --check -- src/battle_message.c` 통과. `make BUILD=hns build/hns/src/battle_message.o -j2` 종료 코드 0. 전체 ROM 빌드는 실행하지 않았다.

### 2026-09-21 — 일격기 성공 메시지 연결 및 HNS 빌드

- 요청/범위: 가위자르기·땅가르기·절대영도·뿔드릴의 성공 시 `STRINGID_ONEHITKO`와 `일격필살!` 결과 메시지를 복원하되 Sturdy·레벨 차이·다이맥스 차단 분기는 유지.
- `src/battle_util.c`의 `CalculateMoveDamage()`에서 실제 배틀 계산에 한해 `GetAdjustedDamage()` 결과가 대상 HP와 같은 `EFFECT_OHKO`일 때 `MOVE_RESULT_ONE_HIT_KO`를 설정하도록 추가했다. 성공 경로의 `MOVE_RESULT_ONE_HIT_KO_NO_AFFECT`를 지우므로 기존 `Cmd_resultmessage()`가 `BattleScript_OneHitKOMsg`를 선택한다. AI 계산(`ctx->updateFlags == FALSE`)에는 결과 플래그를 쓰지 않는다.
- Endure·Sturdy·Focus Band/Sash·친밀도 생존은 HP 1을 남겨 성공 조건에 해당하지 않는다. 다이맥스·공격자보다 높은 레벨·Sturdy는 피해 계산 전의 기존 `DoesOHKOMoveMissTarget()` 분기와 실패/특성 메시지를 그대로 유지한다.
- 검증: `git diff --check -- src/battle_util.c` 통과, `make BUILD=hns build/hns/src/battle_util.o -j2` 성공. 전체 ROM은 표준 명령 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144, IWRAM 25,704/32,768, ROM 33,328,612/33,554,432. ELF `2054a1c1466fe95db8655e7a34b64814e213c9342bb23ee192b0eca63e4ccf26`, GBA `16396927c4fbee75aa2fbf59a5cb8e1be0e7de2f23d8b4eba058d6f31d7d5f53`.
- 게임 화면 검증: mGBA 실행 파일이 없어 미수행. 새 ROM에서 네 일격기 성공·Sturdy·레벨 차이·다이맥스·Endure/기합의띠 생존의 각 메시지 분기를 확인한다.

### 2026-09-21 — 일격기 성공 메시지 연결 확인

- 질문/범위: HNS에서 가위자르기·땅가르기·절대영도·뿔드릴 명중 시 `일격필살!`이 출력되는지 정적 확인.
- 네 기술 모두 `EFFECT_OHKO`다. `src/battle_message.c`의 `STRINGID_ONEHITKO`와 `data/battle_scripts_1.s`의 `BattleScript_OneHitKOMsg`는 존재하지만, 현재 `src/battle_script_commands.c`에서 이 스크립트를 호출하는 조건은 `MOVE_RESULT_ONE_HIT_KO` 플래그뿐이다.
- 현재 소스 전체에서 성공 경로에 `MOVE_RESULT_ONE_HIT_KO`를 설정하는 코드는 찾지 못했다. OHKO 명중 시 `DoesOHKOMoveMissTarget()`가 `MOVE_RESULT_ONE_HIT_KO_NO_AFFECT`를 남기고, 피해 계산은 대상 HP를 피해량으로 설정한다. 이 비트는 `Cmd_resultmessage()`에서 별도 `case`가 없어 결과 문자열이 선택되지 않을 가능성이 있다. 따라서 소스 기준으로 성공 일격기의 별도 `일격필살!` 출력은 연결되지 않았다.
- 원인 추적: upstream 커밋 `6c05a08750a` `Refactor OHKO Moves (#8916)` 전에는 `Cmd_tryKO`가 대상 HP를 피해량으로 넣은 뒤 `MOVE_RESULT_ONE_HIT_KO`를 설정했다. 해당 커밋은 `Cmd_tryKO`를 제거하고 `DoesOHKOMoveMissTarget()`·`EFFECT_OHKO` 피해 계산으로 분리했지만 성공 플래그 설정을 대체하지 않았다. 따라서 HNS만 임의로 삭제한 것이 아니라 현재 upstream 계열 리팩터링의 누락을 HNS가 그대로 물려받은 것으로 판단한다.
- 실패 시에는 레벨 차이·다이맥스는 효과 없음 분기, `Sturdy`는 특성 팝업 및 방어 메시지 분기를 사용한다. 이번 확인에서는 파일·ROM 수정과 빌드를 하지 않았고, mGBA 부재로 실제 화면은 확인하지 않았다.

### 2026-09-21 — 포이즌힐·솔라파워·아이스바디 특성 팝업 전용 출력 적용

- 요청/범위: 최신 포켓몬 게임처럼 포이즌힐·솔라파워/건조피부·아이스바디 발동 시 특성 팝업만 표시하고 기존 회복/피해 텍스트는 건너뛴다.
- 수정 파일: `data/battle_scripts_1.s`. `BattleScript_PoisonHealActivates`, `BattleScript_SolarPowerActivates`, `BattleScript_IceBodyHeal`에서 `printstring`과 `waitmessage`를 제거했다. `BattleScript_AbilityPopUp`, 상태/회복 애니메이션, HP 갱신, 솔라파워의 `tryfaintmon`은 유지했다. 문자열 ID·테이블은 삭제하지 않았다.
- 검증: 세 ID의 직접 `printstring` 참조가 사라졌고 스크립트 구조를 확인했다. `git diff --check -- data/battle_scripts_1.s` 통과. HNS 빌드 종료 코드 0, EWRAM 249,012/262,144, IWRAM 25,704/32,768, ROM 33,328,468/33,554,432. ELF `02b5734b02118e3d4796f3aa2d32d5b521cef2695d6d1701e670742ec9fea790`, GBA `f865817ed5136dd5f688dfad89439bbed0be52b46e375017eaf53307bd858793`; 로그 `build/localization-logs/hns-passive-ability-popup-only-20260921.log`.
- 게임 화면 검증: mGBA 실행 파일이 없어 미수행. 새 ROM에서 세 특성의 팝업 후 텍스트 생략과 HP 처리 타이밍을 확인한다.

### 2026-09-21 — 포이즌힐·솔라파워·아이스바디 회복/피해 메시지 호출 확인

- 질문/범위: `STRINGID_POISONHEALHPUP`, `STRINGID_SOLARPOWERHPDROP`, `STRINGID_ICEBODYHPGAIN`이 HNS에서 실제 출력되는지 확인.
- 확인: 포이즌힐은 독/맹독 상태에서 HP 1/8 회복, 솔라파워는 햇빛에서 HP 1/8 피해, 아이스바디는 얼음 날씨에서 HP 1/16 회복 시 각각 배틀 스크립트가 문자열을 출력한다. 솔라파워 스크립트는 햇빛 아래 건조피부 피해에도 공유된다. 아이스바디는 HP 최대·회복봉인·지하/수중 반무적이면 호출되지 않는다.
- 세 경로 모두 능력 팝업 호출이 문자열보다 앞선다. “미사용 추정” 주석은 실제 호출과 불일치한다. 소스·ROM 수정과 빌드는 하지 않았다.
- 검증 위치: `src/battle_end_turn.c`, `src/battle_util.c`, `data/battle_scripts_1.s`의 직접 호출부를 정적으로 확인했다.

### 2026-09-21 — `TARGETABILITYSTATRAISE` 한글 문구의 플레이스홀더·공백 확인

- 사용자가 번역한 문구의 확장값을 확인했다. 약점갑옷 스피드 상승에서는 `{B_DEF_NAME_WITH_PREFIX}`가 야생/상대 포켓몬 이름, `{B_BUFF1}`이 `스피드`, 현재 2단계 상승인 `{B_BUFF2}`가 `크게 `로 들어간다.
- 현재 입력의 `{B_BUFF1}가 {B_BUFF2} 올라갔다!`는 `B_BUFF2`의 후행 공백과 문자열의 추가 공백이 겹친다. 따라서 예시는 `야생 포켓몬의\n스피드가 크게  올라갔다!`이며, `{B_BUFF1}{B_TXT_IGA} {B_BUFF2}올라갔다!` 형태가 공백·조사까지 안전하다. 소스는 수정하지 않았다.
- 검증: `ChangeStatBuffs()`의 `PREPARE_STAT_BUFFER`·상승 정도 버퍼, `B_WEAK_ARMOR_SPEED`, 약점갑옷 배틀 스크립트를 정적으로 확인했다. 빌드는 하지 않았다.

### 2026-09-21 — 깨어진갑옷·분노의경혈·불굴의마음 특성 팝업 확인

- 질문: 깨어진갑옷, 분노의경혈, 불굴의마음 발동 시 특성 팝업이 별도로 표시되는지 확인.
- 확인: 깨어진갑옷 스크립트는 `BattleScript_AbilityPopUp` 후 방어/스피드 변화 문구를 출력한다. 분노의경혈은 `BattleScript_TargetsStatWasMaxedOut`에서 팝업 후 공격 최대화 문구를 출력한다. 불굴의마음은 플린치 후 `BattleScript_AbilityPopUp`을 호출하고 스피드 상승 문구를 출력한다. 세 ID의 문자열은 팝업이 아니라 후속 효과 메시지다.
- 검증: `data/battle_scripts_1.s`와 `src/battle_util.c` 호출부를 정적으로 확인했다. 소스·ROM 수정과 빌드는 하지 않았다.

### 2026-09-21 — 전투 특성 메시지 11개 출력 조건 조사

- 요청/범위: `TARGETABILITYSTATRAISE`, `TARGETSSTATWASMAXEDOUT`, `ATTACKERABILITYSTATRAISE`, `POISONHEALHPUP`, `BADDREAMSDMG`, Mold Breaker/Teravolt/Turboblaze/Slow Start/Solar Power 관련 메시지의 실제 출력 조건 확인.
- 확인: 대상 능력치 상승 ID는 약점갑옷 스피드 상승에서만 직접 사용된다. 분노의경혈은 급소 피격 후 공격을 최대 랭크로 올릴 때 `TARGETSSTATWASMAXEDOUT`을 사용한다. 공격자 능력치 상승 ID는 의기양양·다운로드·가속·소울하트 경로에서 사용된다. 포이즌힐은 독/맹독 상태의 HP 1/8 회복, 나쁜꿈은 수면/컴어토즈 상대에 대한 턴 종료 1/8 피해, 솔라파워 ID는 솔라파워뿐 아니라 햇빛 아래 건조피부 피해에도 공유된다.
- 교대 메시지: Mold Breaker, Teravolt, Turboblaze, Slow Start는 `ON_SWITCHIN`에서 출력된다. Slow Start 종료 메시지는 `B_SLOW_START_TIMER`(현재 5) 카운터가 0이 되는 턴 종료에 출력된다.
- 검증: `data/battle_scripts_1.s`, `src/battle_util.c`, `src/battle_end_turn.c`, `src/battle_script_commands.c`의 호출부를 정적으로 확인했다. 소스·문자열·ROM은 수정하지 않았고 빌드도 실행하지 않았다. `POISONHEALHPUP`와 `SOLARPOWERHPDROP`의 “미사용 추정” 주석은 실제 호출과 불일치한다.
- 게임 화면 확인: 수행하지 않음. 실제 재현은 후속 한글 문구 검토 때 진행한다.

### 2026-09-20 — 도구 드롭 메시지 한글 문구 수정 후 재빌드

- 사용자가 `src/battle_message.c`의 두 메시지를 수정했다. `STRINGID_WILDPKMNDROPPEDITEM`은 `{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}` 및 `{B_LAST_ITEM}{B_TXT_EULREUL}`을 사용해 `떨어뜨렸다!`를 출력하고, `STRINGID_DROPPEDITEMBAGFULL`은 `가방이 가득 차서\n아이템을 주울 수 없습니다!`로 변경됐다. 이 문구는 추가로 수정하지 않았다.
- 검증: `git diff --check -- src/battle_message.c src/battle_script_commands.c` 통과. `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144, IWRAM 25,704/32,768, ROM 33,328,484/33,554,432. ELF `844123625189a1c37782dc87c35b0d9fd2023a1cb96186ee4decf941f174c0d9`, GBA `9a37bb06b23c1b5ad00cfc962d9bfadac64e276604d5df0456b68c1497eb7472`; 로그 `build/localization-logs/hns-drop-item-message-ko-20260920.log`.
- 게임 화면 검증: mGBA 실행 파일이 없어 미수행. 새 ROM에서 성공·가방 가득 참 두 분기의 줄바꿈과 조사 연결을 확인한다.

### 2026-09-20 — 야생 포켓몬 도구 드롭 메시지에 실제 포켓몬 이름 연결

- 요청/범위: `STRINGID_WILDPKMNDROPPEDITEM`과 `STRINGID_DROPPEDITEMBAGFULL`에 도구를 떨어뜨린 야생 포켓몬 이름을 표시.
- `src/battle_script_commands.c`의 `BS_TryGiveDroppedItems()`에서 실제 선택된 `battlers[i]`를 `gBattleScripting.battler`에 저장했다. 두 메시지는 `{B_SCR_NAME_WITH_PREFIX}`를 사용하므로 싱글·더블 배틀 모두 선택된 포켓몬의 이름과 `야생` 접두사를 참조한다. 도구 획득 분기, 가방 가득 참 분기, `{B_LAST_ITEM}`은 유지했다.
- 검증: `git diff --check -- src/battle_message.c src/battle_script_commands.c` 통과. `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144, IWRAM 25,704/32,768, ROM 33,328,468/33,554,432. ELF `790ebc6a38ad319bcdc28525ad03a2ae9ec09863f51a21d9f531c3b895ad1277`, GBA `5ffe014bb97113a7fa1632b036a409747cf036e1f580719be776607e537ea62d`; 로그 `build/localization-logs/hns-drop-item-name-20260920.log`.
- 게임 화면 검증: mGBA 실행 파일이 없어 미수행. 새 ROM에서 야생 싱글·더블 배틀의 도구 드롭 성공·가방 가득 참 메시지와 이름·줄바꿈을 확인한다.

### 2026-09-20 — Pokémon Champions 효과 단계별 출력 및 아이템 팝업 HNS 별도 이식

- 요청/범위: PR #9777의 동적 복수형 전체가 아니라 효과 단계별 출력과 아이템 팝업을 HNS UI 리소스·한글 아이템명 방식에 맞춰 별도로 포팅.
- 효과 단계: `include/constants/battle.h`에 극도로 효과적·대체로 효과가 적음 플래그와 high/low 묶음을 추가했다. 배율 `>2.0`·`<0.5`에서 새 플래그를 설정하며, `moveResultFlags`와 관련 네이티브 스크립트 인수를 `u32`/`.4byte`로 확장해 상위 비트가 잘리지 않도록 했다. 효과음, 단일·더블 대상 결과 문구, 방어 측 크리티컬 문구, Weakness Policy·Enigma Berry 판정을 연결했고 기존 한글 문구는 수정하지 않았다.
- 아이템 팝업: `src/battle_interface.c`의 HNS 능력 팝업 시트·팔레트·`TAG_ABILITY_POP_UP_PLAYER1 + battler` 태그를 재사용했다. `PrintItemOnItemPopUp()`은 `GetItemName()`을 호출하므로 한글 아이템명 테이블을 그대로 사용한다. `BS_ShowItemPopup`/`BS_DestroyItemPopup` 및 `BattleScript_ItemPopUp_*` helper를 추가하고 HNS에서 실제 대응하는 아이템 발동 스크립트에 호출을 넣었다. Life Orb에는 PR의 HP 손실 문자열도 연결했다.
- 범위 결정: HNS의 다중 타격 한글 문구는 유지했고 PR의 동적 복수형 native는 추가하지 않았다. upstream에만 있거나 HNS 구조가 다른 아이템 라벨은 무리하게 추가하지 않았다. 효과 플래그 초기화 두 곳도 새 high/low 비트를 포함하도록 보완했다.
- 검증: `git diff --check` 통과. `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144, IWRAM 25,704/32,768, ROM 33,328,596/33,554,432. ELF `452a83800d5f92a550f2c02d79ab11a76af650e05eb2bf762dd2cec6ef7320af`, GBA `dfa6f24789e7fe649d90221dc65ee5202ead1e283c0e08d83b336e1c3acfd8db`; 로그 `build/localization-logs/hns-champions-effects-item-popup-final-20260920.log`.
- 게임 화면 검증: mGBA를 실행할 수 없어 미수행. 새 ROM에서 효과 단계별 메시지, 아이템 팝업 한글명·위치·시간과 각 아이템 발동 지점을 확인한다.

### 2026-09-19 — Pokémon Champions 배틀 메시지 PR #9777 확인 및 신규 문자열 이식

- upstream [PR #9777](https://github.com/rh-hideout/pokeemerald-expansion/pull/9777)의 `63631f951f`가 현재 HNS `HEAD`에 포함되어 있지 않음을 확인했다. HNS의 배틀 메시지 파일에는 기존 한글화 변경이 많아 PR 전체를 그대로 적용하지 않았다.
- PR의 신규 문자열 11개(`MOSTLYINEFFECTIVE`, `EXTREMELYEFFECTIVE`, `NOTVERYEFFECTIVEONDEF`, `SUPEREFFECTIVEONDEF`, `MOSTLYINEFFECTIVEONDEF`, `EXTREMELYEFFECTIVEONDEF`, `EXTREMELYEFFECTIVETWOFOES`, `MOSTLYINEFFECTIVETWOFOES`, `CRITICALHITONDEF`, `S`, `LOSTSOMEOFITSHP`)를 enum/table에 원문 그대로 추가했다. HNS에 이미 있던 `ITDOESNTAFFECTTWOFOES`는 수정하지 않았다.
- 기존 문구는 변경하지 않았다. 수정 대상 전체 ID와 upstream 변경 방향(`team`→`side`, 효과 문장 교체, 동적 복수형, `AFTERMATHDMG` 이름 변경 등)은 `STATUS.md` 최신 항목에 기록했다. 현재 HNS에 없는 화면 해제 메시지 ID도 관련 로직과 함께 후속 추가 대상으로 남겼으며, `STICKYWEBDISAPPEAREDFROMYOU` 삭제는 보류했다.
- 동적 복수형·효과 단계 출력·아이템 팝업 등 PR의 전투 로직은 후속 작업으로 남겼다. `git diff --check` 통과.
- HNS 전체 빌드 성공: EWRAM 249,012/262,144, IWRAM 25,704/32,768, ROM 33,326,996/33,554,432. ELF `a882a342ff825ffae766b9904aa1f81d5ecc002f14cec142a422bd639bf7ef12`, GBA `683731e1febab570a87eb2c0e84862e5fe60cfc654ed7cf2be151a34f36c9722`; 로그 `build/localization-logs/hns-battle-messages-20260919.log`. mGBA 실행은 하지 않았다.

### 2026-09-19 — Scarlet/Violet 도감 스키핑 기능 HNS 이식 및 빌드 검증

- upstream [PR #9797](https://github.com/rh-hideout/pokeemerald-expansion/pull/9797)의 도감 공백 스키핑 설정과 `ShouldSkipPokedexListEntry()` 구현을 HNS에 포팅했다. `include/config/pokemon.h`에 네 모드와 `P_SKIP_POKEDEX_GAPS`를 추가했고, 현재 값은 `SKIP_GAPS_EXCEPT_BEFORE_AFTER`다.
- `include/pokedex.h`에 공용 helper 선언을 추가했다. `src/pokedex.c`와 `src/pokedex_plus_hgss.c`의 숫자순 목록 생성부에서 seen 플래그·앞뒤 도감 번호를 검사해 미등록 구간을 건너뛴다. `SKIP_GAPS_EXCEPT_ONE`을 선택하면 남은 미등록 칸의 번호를 `------`/`----`으로 표시한다. 기존 HNS의 지역·획득 가능 도감 매핑은 유지했다.
- `include/constants/pokedex.h`의 열거 순서 매크로화는 스키핑 구현에 필요한 변경이 아니므로 일괄 이식하지 않았다.
- `git diff --check` 통과. `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,326,548/33,554,432(99.32%). `pokehns.elf` SHA-256 `454ebef4e76d7029a50b8947d7cd20867ae8011501bcec276ffae1662b74dbdb`, `pokehns.gba` SHA-256 `9b35731808d956016777d55ebf6ec42d2749ed348e0cc8599a806f11ee00100c`; 로그 `build/localization-logs/hns-pokedex-skipping-20260919.log`.
- mGBA가 없어 실제 도감 화면 검증은 남아 있다. 새 ROM에서 기본·HGSS 숫자순 도감의 공백, 스크롤, 번호·이름 정렬을 확인한다.

### 2026-09-19 — Scarlet/Violet 도감 스키핑 기능 이식 여부 확인

- 요청/범위: Emerald Expansion 1.17.0의 PR #9797(Scarlet and Violet Pokédex Skipping)이 HNS에 추가됐는지 확인.
- upstream 확인: PR 설명은 `DONT_SKIP_GAPS`, `SKIP_GAPS_EXCEPT_ONE`, `SKIP_GAPS_EXCEPT_BEFORE_AFTER`, `SKIP_ALL_GAPS` 및 `P_SKIP_POKEDEX_GAPS`를 추가하고, SV와 유사한 모드로 `SKIP_GAPS_EXCEPT_BEFORE_AFTER`를 제시한다. 출처: https://github.com/rh-hideout/pokeemerald-expansion/pull/9797
- HNS 확인: `include/config/pokemon.h`에 위 식별자가 없고, `src/pokedex.c:2180` 및 `src/pokedex_plus_hgss.c:2480`의 두 `CreatePokedexList()`에도 해당 설정 분기가 없다. 저장소 이력에서 `git log -S`로 식별자·PR 변경을 찾지 못했다.
- 결론: 현재 HNS에는 PR #9797의 도감 스키핑 기능이 추가되어 있지 않다. 이번에는 파일·ROM을 수정하지 않았고 빌드도 하지 않았다. 이식할 경우 기본 도감과 HGSS 도감 로직을 모두 확인해야 한다.

### 2026-09-19 — 파동의방호 설명 문자열 변경 후 재빌드

- 요청/범위: 파동의방호 특성 설명 변경을 확인하고 HNS ROM을 다시 빌드.
- 확인: `src/data/abilities.h:2484-2488`의 설명은 `COMPOUND_STRING("접촉 기술로 입는 데미지가 반감된다.")`이다. 기존 `ABILITY_AURA_GUARD` 효과 코드와 메가루카리오Z 특성 슬롯은 그대로 유지했다.
- 검증: `git diff --check` 통과. `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,326,484/33,554,432(99.32%). `pokehns.elf` SHA-256 `aa0cace7496c3496a46fc59503aeeb5768859c574154e32f1ce6e600796fefd4`, `pokehns.gba` SHA-256 `1f1026ae00fc534039d488f6e3b41f0eacce3774a846540a22a5a732f6a9d954`, 로그 `build/localization-logs/hns-aura-guard-description-20260919.log`.
- mGBA를 실행할 수 없어 실제 특성 팝업에서 문장 줄바꿈과 잘림은 확인하지 않았다. 다음 시작점은 새 ROM에서 파동의방호 설명을 열어 문장 표시를 확인하는 것이다.

### 2026-09-19 — 파동의방호 및 메가루카리오Z 특성 빌드 검증

- 요청/범위: 사용자가 추가한 파동의방호 효과와 메가루카리오Z의 특성 변경을 HNS 빌드에서 확인.
- 코드 확인: `src/battle_util.c:7666-7672`의 `GetDefenderAbilitiesModifier()`가 `ABILITY_AURA_GUARD`일 때 접촉 기술을 `0.5`배로 만들고 `recordAbility`를 설정한다. 기존 복슬복슬 분기는 그대로 남아 있다. `src/data/abilities.h:2484-2488`에는 파동의방호 이름·설명이 있다.
- 종 데이터 확인: `SPECIES_LUCARIO_MEGA_Z`(종 ID 1559)의 `.abilities` 세 슬롯이 모두 `ABILITY_AURA_GUARD`이고, 상수 값은 319이다. HNS ROM의 `gSpeciesInfo` 엔트리(간격 0x10C, 특성 오프셋 0x18)를 직접 읽어 `(319, 319, 319)`와 일치시켰다.
- 빌드: `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,326,500/33,554,432(99.32%). `pokehns.elf` SHA-256 `4de126b0f7296dc1682e7de352911c40284bfba1160eb2c47f33c37d2405a5ec`, `pokehns.gba` SHA-256 `fc279f72af126010adf17d84c7593a92a2cf4ff6160194c790bc1cc41ac194c5`, 로그 `build/localization-logs/hns-aura-guard-lucario-z-20260919.log`.
- `git diff --check` 통과. mGBA를 실행할 수 없어 실제 전투에서 접촉 기술 피해가 반감되는지는 확인하지 않았다. 다음 시작점은 메가루카리오Z가 파동의방호 상태에서 접촉·비접촉 기술을 각각 받을 때의 피해와 특성 팝업을 확인하는 것이다.

### 2026-09-19 — 메가 포켓몬 특성 변경 HNS 빌드 검증

- 요청/범위: 사용자가 수정한 일부 메가 포켓몬의 특성이 HNS 빌드에 정상 반영되는지 확인.
- 확인 대상: Gen1~Gen9 `species_info` 파일에서 HEAD와 현재 `.abilities` 배열이 다른 메가 엔트리 47개를 추출했다. 세대별 변경 수는 6/3/3/4/7/12/5/1/6개다. 기존 메가 엔트리까지 포함해 현재 ROM의 메가 엔트리 99개를 모두 검사했다.
- 링크 데이터 검증: `gSpeciesInfo` 심볼의 엔트리 크기 0x10C와 `GetSpeciesAbility()`가 사용하는 특성 오프셋 0x18을 기준으로 ROM 바이트를 읽었다. 99개 메가 엔트리의 세 특성 슬롯이 현재 헤더의 열거값과 모두 일치했다. `FERALIGATR_MEGA`의 두 번째 슬롯 `NONE` 등 소스의 빈 슬롯은 변경하지 않았다.
- 빌드: `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,326,420/33,554,432(99.32%). `pokehns.elf` SHA-256 `88e9757efbf7eb09c10a879c716ec829f67df1a39e8747cd9c188ae2d0b7f677`, `pokehns.gba` SHA-256 `5a1f3bdc0251340c674669bbedcc2dd71886bd16d230f9a52dc0994d85f85f77`, 로그 `build/localization-logs/hns-mega-abilities-20260919.log`.
- `git diff --check` 통과. mGBA를 실행할 수 없어 실제 배틀에서 특성 효과가 발동하는지는 확인하지 않았다. 다음 시작점은 새 ROM에서 변경된 메가 포켓몬의 특성 팝업과 전투 효과를 확인하는 것이다.

### 2026-09-19 — 포켓몬 능력창 리본 수 중앙 표시

- 요청/범위: 포켓몬 능력창 리본 탭에서 `현재`를 제거하고 리본 수만 `1개`, `16개`처럼 중앙 정렬. 리본이 없을 때의 `없음` 동작은 유지.
- 변경: `src/strings.c:436`의 `gText_RibbonsVar1`을 `"{STR_VAR_1}개"`로 변경해 `현재`와 `CLEAR_TO 46`을 제거했다. `src/pokemon_summary_screen.c:4072`의 숫자 변환은 `STR_CONV_MODE_LEFT_ALIGN`으로 두어 한 자리 수 앞의 정렬용 공백을 제거했다. `PrintRibbonCount()`의 중앙 정렬 계산과 0개일 때 `gText_None` 분기는 유지했다.
- 검증: `git diff --check` 통과. `make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,326,420/33,554,432(99.32%). ROM SHA-256 `379d9b03a47d2ab53a101e658257770c9c52bfedefb9ead0ab3aa9a3a8f92dc6`; 로그 `build/localization-logs/hns-ribbon-count-center-20260919.log`.
- 게임 확인: mGBA 실행은 하지 않았다. 리본 수 1개·16개와 리본 없음의 `없음`이 창 중앙에 맞는지 확인한다.

### 2026-09-19 — 계속 화면 수량 단위 및 리본 수 표시 수정

- 요청/범위: 모험 계속 화면에서 포켓몬 도감 수를 `12마리`, 배지 수를 `8개`·`16개`처럼 표시하고, 포켓몬 능력 화면의 리본 수 뒤에도 `개`를 표시.
- 변경: `src/main_menu.c`에 `마리`·`개` 단위 문자열을 추가해 도감/배지 숫자에 붙였다. 배지 변환 모드를 앞자리 0을 채우는 `STR_CONV_MODE_LEADING_ZEROS`에서 `STR_CONV_MODE_LEFT_ALIGN`으로 바꿨다. 능력치 화면의 리본 수는 `src/pokemon_summary_screen.c:4061-4078`에서 출력하며, 현재 `src/strings.c:436`의 `gText_RibbonsVar1`에 이미 `{STR_VAR_1}개`가 들어 있어 별도 코드 변경 없이 `1개` 형식이 유지된다. 한 자리 수 앞의 `STR_CONV_MODE_RIGHT_ALIGN` 공백은 `CLEAR_TO 46` 뒤 3픽셀 정렬용이라 시각적으로 거의 드러나지 않으며, 접미사와 관계없는 정렬 변경은 되돌렸다.
- 기존 사용자 변경: `src/main_menu.c`와 `src/pokemon_summary_screen.c`의 기존 실행 비트 변경, 요약 화면 HP 표시 및 타입 아이콘 위치 변경은 건드리지 않았다.
- 검증: `git diff --check` 통과. `make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,326,436/33,554,432(99.32%). ROM SHA-256 `fa51a7aa16501fc626543dd41e304343f3fe17a5781fdd28fb224e98202e3efd`; 로그 `build/localization-logs/hns-count-units-20260919.log`.
- 게임 확인: mGBA 실행은 하지 않았다. 새 ROM에서 계속 화면과 포켓몬 능력 화면의 단위 출력 및 우측 정렬 영역의 잘림을 확인한다.

### 2026-09-19 — Pokéblock 메뉴 한글 그래픽 변환 및 팔레트 검증

- 요청/범위: 수정된 `graphics/pokeblock/menu.png`·`menu.4bpp`의 색상과 HNS 출력 호환성을 확인.
- 참조 확인: `src/graphics.c`가 `menu.4bpp.smol`과 `menu.gbapal`을 포함하고 `src/pokeblock.c`가 메뉴 그래픽과 6×16색 팔레트를 로드한다. 경로·파일명 변경은 필요하지 않았다.
- 팔레트 결정: 현재 PNG는 64×40 4-bit indexed(16색)이고 첫 16색이 192바이트 `menu.gbapal`의 첫 뱅크와 일치한다. 런타임의 나머지 5개 뱅크를 보존하기 위해 `menu.gbapal`은 재생성하지 않았다.
- 변환/검증: 현재 PNG를 `gbagfx`로 재변환한 1,280바이트 `menu.4bpp`가 실제 파일과 `cmp` 일치했다. 기존 `.smol`은 최신 4bpp와 달라 `compresSmol`로 재생성해 432바이트가 됐다. 독립 변환본·압축본·압축 해제본이 모두 `cmp` 일치했다.
- 빌드: ELF에서 `gMenuPokeblock_Gfx` 0x1B0, `_Pal` 0xC0, `_Tilemap` 0x98 확인. `make --jobserver-style=pipe hns -j8` 종료 코드 0, EWRAM 94.99%, IWRAM 78.44%, ROM 99.32%. ROM SHA-256 `8e1cd7347280678ad765720cfc0c03e08e1091df0afe345ce722d3a7718067ac`; 로그 `build/localization-logs/hns-pokeblock-menu-palette-20260919.log`.
- 게임 화면 확인: mGBA 실행 파일이 없어 미수행. 필드·배틀·포켓블록 급식기 메뉴에서 한글 글자 색상, 선택 강조, 팔레트 뱅크와 잘림을 확인한다.
- 남은 문제/다음 시작점: 실제 화면 검증만 남았다. `src/pokeblock.c`는 기존 실행 비트 변경만 있었고 내용은 건드리지 않았다.

### 2026-09-19 — Frontier Pass·슬롯머신 한글 그래픽 변환 및 팔레트 검증

- 요청/범위: `pokeemerald-kr`에서 가져온 Frontier Pass `map_and_card.png`, `bg.png`, 슬롯머신 `menu.png`를 HNS용 산출물로 변환하고 한글 글자에 기존 런타임 팔레트가 올바르게 적용되는지 확인.
- 수정/산출물: 입력 PNG 세 개는 추가 편집하지 않았다. `bg.4bpp`/`.4bpp.smol`, `map_and_card.8bpp`/`.8bpp.smol`, `menu.4bpp`/`.4bpp.smol`을 `gbagfx`·`compresSmol`로 재생성했다. HNS 소스의 참조 경로와 파일명은 이미 맞아 코드 수정은 하지 않았다.
- 팔레트 결정: `bg.png`·`menu.png`는 현재 4-bit PNG라 `gbagfx`로 팔레트를 새로 만들면 16색만 생성된다. 실제 런타임은 각각 8×16색 `bg.gbapal`과 5×16색 `menu.gbapal`을 사용하므로, 기존 다중 뱅크 팔레트는 보존했다. 두 PNG의 첫 16색은 각 런타임 팔레트 첫 뱅크와 바이트 단위로 일치한다. `map_and_card.png`의 127색 팔레트는 `bg.gbapal`의 앞 127색과 일치하며, 현재 코드처럼 별도 팔레트 없이 사용한다.
- 변환 결과: `bg.4bpp` 16,384바이트/압축 4,436바이트, `map_and_card.8bpp` 14,336바이트/압축 3,792바이트, `menu.4bpp` 8,704바이트/압축 2,696바이트. 임시 경로 독립 변환본과 여섯 파일 모두 `cmp` 일치.
- 링크 검증: `nm -S --defined-only pokehns.elf`에서 `gFrontierPassBg_Gfx` 0x1154, `gFrontierPassBg_Pal` 0x100, `gFrontierPassMapAndCard_Gfx` 0xED0, `gSlotMachineMenu_Gfx` 0xA88, `gSlotMachineMenu_Pal` 0xA0를 확인했다. `make --jobserver-style=pipe hns -j8` 종료 코드 0; EWRAM 94.99%, IWRAM 78.44%, ROM 99.32%. ROM SHA-256 `626a9bfb9f6e7561f573960153fd90387a1ea2120b31e95063bbe6e88d350b60`; 로그 `build/localization-logs/hns-frontier-slot-ko-20260919.log`.
- 게임 화면 확인: 미수행. 새 ROM에서 Frontier Pass 지도·카드·메달 화면과 슬롯머신 메뉴를 열어 한글 글자 색상, 팔레트 뱅크, 잘림을 확인한다.
- 남은 문제/다음 시작점: 실제 mGBA 확인만 남았다. `src/data/graphics/slot_machine.h`는 기존 작업 트리의 실행 비트 변경만 있었고 내용은 건드리지 않았다.

### 2026-09-19 — 트레이너카드 엄마 저금액 단위 추가

- 요청/범위: HNS 트레이너카드 `용돈 3000원/0`에서 엄마 저금액 뒤에도 `원`을 표시.
- 수정 파일: `src/trainer_card.c`, `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`.
- 변경: `PrintMoneyOnCard()`의 HNS 분기에서 `Mom_GetBalance()` 숫자를 `gStringVar4`에 복사한 직후 `원`을 덧붙였다. 소지금·저금액 값과 다른 카드 항목은 변경하지 않았다. `src/trainer_card.c`의 기존 미커밋 변경은 보존했다.
- 검증: `git diff --check -- src/trainer_card.c` 통과. 첫 전체 `make hns -j8`은 남아 있던 jobserver FIFO 충돌로 지연돼 중단했다. 첫 의존성 생략 빌드는 `_()` 매크로를 함수 인자에 직접 쓴 컴파일 오류가 나서 HNS 전용 `sText_Won` 상수로 수정했다. 최종 `make --jobserver-style=pipe BUILD=hns NODEP=1 pokehns.gba -j8` 종료 코드 0, EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,326,516/33,554,432(99.32%). ROM SHA-256 `331b5dd57f57b13393c91047d3428b4feeb869ab3af4826523dc0b57d29dd496`, 로그 `build/localization-logs/hns-trainer-card-mom-won-20260919.log`.
- 게임 화면 확인: 미수행. 새 ROM에서 저금액 0 및 큰 금액일 때 `원`과 카드 안의 표시 폭을 확인한다.
- 다음 시작점: 새 `pokehns.gba`의 트레이너카드에서 저금액 0과 큰 금액을 각각 표시해 `원` 및 잘림을 확인한다.

### 2026-09-19 — `graphics/contest` HNS 사용 여부 조사

- 요청/범위: `graphics/contest` 아래 파일이 HNS에 쓰이는지 확인. 사용자 표기의 `gtaphics/contest`는 실제 존재하는 `graphics/contest`로 해석했다.
- 수정 파일: `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`만 갱신. 그래픽·코드·ROM 변경 없음.
- 확인: `src/graphics.c:853-882,988-989,1453-1466,1970-1980`에서 일반 및 일본어 콘테스트 자산을 포함한다. `src/contest.c:1087-1091,1368-1385`는 일반 인터페이스·관객·타일맵·팔레트를 로드하고, `src/contest_util.c:444-449,1386-1437`은 결과 화면 자산을 로드한다. `src/contest.c:672` 및 `src/contest_util.c:191-192`는 비압축 텍스트 자산을 직접 포함한다.
- 일본어 경로: `graphics_file_rules.mk:355-370`은 `japanese/` 합성 파일의 생성 규칙을 정의한다. 현재 `pokehns.elf`에는 `gJPContest*` 심볼이 존재한다. 그러나 `rg -n 'gJPContest' src include`에서 `src/graphics.c` 정의 외의 사용처는 없으므로 현재 표시 경로에는 연결되지 않는다.
- 검증: `rg`로 `INCBIN`·소비 코드·규칙을 확인하고 `nm -S --defined-only pokehns.elf`로 HNS 링크 심볼을 확인했다. 데이터 수정이 없어 빌드는 실행하지 않았다. 실제 콘테스트 화면은 확인하지 않았다.
- 남은 문제: HNS 맵에서 콘테스트 입장 가능 여부와 실제 화면의 영어 픽셀은 이번 정적 조사 범위에서 검증하지 않았다.
- 다음 시작점: 콘테스트 그래픽 한글화 시 일반 `graphics/contest/` 및 `results_screen/` 입력 PNG/PAL을 대상으로 확인하고, 수정 뒤 `make hns -j8`과 본편·결과 화면을 검증한다.
- 정정(후속 질문): 위 해안시티 워프는 호연 공통 맵의 데이터다. `make hns`는 `MAP_VERSION=hns`이고 새 게임은 `src/new_game.c:194`에서 연두마을 플레이어 집으로 설정한다. HNS 지역 맵의 JSON/스크립트에서 해안시티 또는 포켓몬 콘테스트 홀로 향하는 워프 참조는 확인되지 않았다. 따라서 앞선 “HNS에서도 콘테스트 참여 경로가 있다”는 결론은 잘못된 추론이다. 콘테스트 소스·자산 및 `_hns` 맵이 빌드에 들어간다는 사실과 일반 플레이 접근성은 구분한다. 실제 게임 확인은 미수행.

### 2026-09-19 — Gen4 기술 정보 창 색 인덱스 보정

- 요청/범위: Gen4 `move_info_window_l/r`의 색을 맞추되 다른 자산·공유 팔레트는 바꾸지 않고, 두 PNG에서만 픽셀 인덱스 8과 D를 교환했다.
- 수정 파일: `graphics/battle_interface/gen4/move_info_window_l.png`, `move_info_window_r.png` 및 이 PNG들의 빌드 산출물 `.4bpp`. PNG의 색표와 비-IDAT 청크, 기본 UI 이미지, `gen4/ability_pop_up.gbapal`, 소스 코드는 수정하지 않았다.
- 변경과 결정 이유: HNS Gen4의 공용 런타임 팔레트에서는 색 인덱스 8/D 값이 입력 PNG 팔레트와 서로 바뀌어 있었다. 공유 팔레트 수정은 동일 태그를 쓰는 능력 팝업 등에도 영향을 줄 수 있어 창 그래픽의 8↔D 인덱스만 교환했다.
- 검증: PNG 디코딩 전후 비교에서 L/R 각 513픽셀의 인덱스만 8↔D로 바뀌었고 PNG 팔레트 및 모든 비-IDAT 청크는 보존됐다. 재생성한 `.4bpp` 각각 512바이트이며, 원래 `.4bpp`의 각 니블을 8↔D로 바꾼 예상 데이터와 정확히 일치했다. PNG에서 추출한 팔레트도 전후 동일했다. `build/hns/src/battle_interface.o`의 `sMoveInfoWindowGfxGen4L/R` payload가 새 `.4bpp`와 각각 `cmp` 일치. `make BUILD=hns NODEP=1 pokehns.gba -j8` 종료 코드 0; EWRAM 94.99%, IWRAM 78.44%, ROM 99.32%. 새 `pokehns.gba` 크기 33,554,432바이트, SHA-256 `fc12a055b5dcb0d631f35c1bfa42ca413599b2f3561271f83831acb67c37b90b`. 로그 `build/localization-logs/hns-gen4-move-info-window-20260919.log`.
- 게임 화면 확인: mGBA에서 별도 재현하지 않았다. 새 ROM에서 Gen4 UI의 L 기본 및 `L=A`/R 상태로 각각 기술 선택 창 색상·잘림을 확인한다.
- 남은 문제: 실제 게임 화면에서 두 회색 음영이 의도한 색으로 보이는지 확인.
- 다음 시작점: 새 ROM `pokehns.gba`를 mGBA에서 실행하고 Gen4 UI의 L 및 R 안내 창을 점검한다.

### 2026-09-19 — Gen4 기술 정보 창 출력 색상 차이 원인 확인

- 요청/범위: 사용자가 올린 화면에서 Gen4 `move_info_window`의 색이 PNG/Soulgold 예상과 다른 이유를 조사했다. 이번 진단에서는 코드·이미지·팔레트를 수정하지 않았다.
- 확인: `src/battle_interface.c`의 `sSpriteTemplate_MoveInfoWindow`는 `TAG_ABILITY_POP_UP`을 사용한다. `GetAbilityPopUpPal()`은 Gen4 UI일 때 `graphics/battle_interface/gen4/ability_pop_up.gbapal`을 반환한다. `.4bpp`에는 색상 인덱스만 있고 PNG의 자체 팔레트는 런타임에 전달되지 않는다.
- 비교: `gbagfx`로 Gen4 L/R PNG의 팔레트를 추출한 결과 두 PNG 팔레트는 서로 동일하다. 각 `.4bpp`의 픽셀 인덱스 사용량도 확인했다(0, 7, 8, A, B, D, E). 이 중 런타임 팔레트 값이 다른 인덱스 8·D가 서로 뒤바뀌어 있다: PNG는 8=`0x5EF7`, D=`0x675A`; Gen4 런타임은 8=`0x675A`, D=`0x5EF7`. 이 때문에 두 회색 계열 음영이 반대로 표시된다. 이미지에서 사용하지 않는 다른 팔레트 슬롯의 차이는 출력 원인이 아니다.
- 비교 기준: 확인한 Soulgold `src/battle_interface.c`는 `TAG_ABILITY_POP_UP`에 루트 `graphics/battle_interface/ability_pop_up.gbapal`을 연결하고 move-info 스프라이트도 같은 태그를 사용한다. HNS Gen4는 `gen4/ability_pop_up.gbapal`을 고르므로 팔레트 소스 경로가 다르다. 출처: https://github.com/Eemeliri/soulgold/blob/master/src/battle_interface.c#L2434-L2455 및 #L2933-L2939 (2026-09-19 확인).
- 검증: `od -tx2`로 두 PNG에서 생성한 `.gbapal`과 `graphics/battle_interface/gen4/ability_pop_up.gbapal`을 비교했고, 소스에서 팔레트 선택 및 sprite palette tag를 확인했다. 빌드는 수행하지 않았다(코드 변경 없음). 사용자 캡처의 인게임 출력은 확인했으나 별도 mGBA 재현은 하지 않았다.
- 남은 문제: 원하는 색을 실제로 맞추는 수정은 아직 하지 않았다. 공용 Gen4 팔레트를 바꾸면 능력 팝업 등 공유 태그 사용처까지 색이 변할 수 있다.
- 다음 시작점: 사용자가 색상 수정을 요청하면 `move_info_window` 전용 palette tag/gbapal을 분리하는 안을 우선 검토하거나, 공유 팔레트를 유지하려면 PNG 픽셀 인덱스를 해당 팔레트에 맞춘다.

### 2026-09-19 — Gen4 전용 기술 정보 창 적용 및 기본 그래픽 재변환

- 요청/범위: 수정된 기본 UI L/R 창을 다시 변환하고, 사용자가 추가한 Gen4 전용 L/R PNG도 4bpp로 변환해 4세대 배틀 UI에서만 선택되도록 했다. L 기본, `L=A`이면 R을 표시하는 동작은 기본 UI와 동일하게 유지했다.
- 수정 파일: `src/battle_interface.c`, 기본/Gen4의 `move_info_window_l.png`·`move_info_window_r.png`에 대응하는 네 `.4bpp`, `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`. PNG 원본은 편집하지 않았다.
- 변경과 결정 이유: `GetMoveInfoWindowSpriteSheet()`에서 `UseGen4BattleUI()`를 확인해 Gen4에서는 `gen4/` 전용 시트를 고르고, 기본 UI에서는 기존 기본 시트를 고른다. `GetBattleMoveDescriptionButton()`의 L 및 `L=A` 분기를 재사용해 Gen4에서도 L/R 상태와 입력 동작을 맞췄다. 네 PNG는 모두 32×32 indexed 형식이라 각 `.4bpp`가 512바이트다. 팔레트는 `TAG_ABILITY_POP_UP` 공용 팔레트 구조를 그대로 사용하고 교체하지 않았다.
- 검증: `gbagfx` 변환 후 독립 변환 결과와 네 `.4bpp`가 모두 `cmp` 일치. `make BUILD=hns NODEP=1 build/hns/src/battle_interface.o` 종료 코드 0. 컴파일된 `.rodata`에서 기본/Gen4 L/R 심볼 각각 `0x200`바이트를 추출해 각 산출물과 모두 `cmp` 일치했다. HNS 링크 로그에 메모리 사용량(EWRAM 94.99%, IWRAM 78.44%, ROM 99.32%)과 `ld`, `gbafix`, `objcopy`가 출력됐고 `pokehns.elf`·`.map`·`.gba`가 05:24에 갱신됐다. 최종 ROM SHA-256 `53fb987ae00c326aaf5f454c473852cd76a4d7fc186d025a0ab79b1d86385be7`. 전체 make 실행 래퍼가 종료 코드를 반환하지 않아 전체 빌드 종료 코드는 미확정으로 기록한다. 로그 `build/localization-logs/hns-gen4-move-info-window-20260919.log`.
- 게임 화면 확인: 미수행. 새 ROM에서 기본 UI L, 기본 UI `L=A`/R, Gen4 UI L, Gen4 UI `L=A`/R로 각각 기술 선택 화면을 띄워 그래픽, 라벨, 팔레트, 잘림을 확인한다.
- 남은 문제: 실제 인게임 화면 확인 및 전체 make의 정상 종료 코드 확인.
- 다음 시작점: `make hns -j8` 전체 빌드의 종료 코드/로그를 확인하고, 생성된 `pokehns.gba`에서 위 네 설정 조합을 화면 검증한다.

### 2026-09-19 — 기술 정보 창 L/R 그래픽 재변환

- 요청/범위: 사용자가 다시 수정한 `graphics/battle_interface/move_info_window_l.png` 및 `move_info_window_r.png`를 게임용으로 변환하고 HNS ROM 링크를 확인했다. 사용자 PNG는 추가 편집하지 않았다.
- 산출물/검증: 두 입력은 각각 32×32 indexed PNG이며 `gbagfx`로 각 512바이트 `.4bpp`를 재생성했다. 임시 경로의 독립 변환본과 실제 두 산출물이 모두 `cmp`에서 일치했다.
- 팔레트/참조 확인: 두 창은 `sSpriteTemplate_MoveInfoWindow`의 `TAG_ABILITY_POP_UP`을 통해 기본 HNS 또는 Gen4 능력 팝업 공용 OBJ 팔레트를 사용한다. 수정 전 기준 PNG도 공용 팔레트와는 다른 내장 PNG 팔레트를 갖는 기존 자산이므로, 팔레트를 새로 생성·교체하지 않았다. 중요한 점은 `src/battle_interface.c`가 두 `.4bpp`를 직접 `INCBIN`한다는 것이다. 첫 시도에서 `graphics.o`만 재생성한 것은 잘못이었고, 실제 표시 데이터는 갱신되지 않았다.
- 수정 및 최종 빌드 검증: `build/hns/src/battle_interface.o`를 다시 컴파일했다(04:14:12). `.rodata`에서 `sMoveInfoWindowGfxL`·`sMoveInfoWindowGfxR`(각 `0x200`)을 추출해 최신 L/R `.4bpp`와 각각 `cmp` 일치를 확인했다. 이후 HNS ROM을 다시 링크해 `pokehns.elf`·`.map`은 04:16:18, `pokehns.gba`는 04:16:19에 생성됐다. 최종 ROM SHA-256은 `4410c0475048307ba50c5e3fd39f50c75f2f556d8124b0e642c26b11ac40e1c0`이다.
- 게임 화면 확인: 이 환경에서는 mGBA를 실행하지 못했다. 기술 선택 화면에서 L/R 설명 버튼 설정별로 안내 창의 한글, 테두리, 색상, 잘림을 확인한다.

### 2026-09-18 — 배틀돔 토너먼트 버튼 그래픽 재변환

- 요청/범위: 사용자가 수정한 `graphics/battle_frontier/tourney_buttons` 자산을 확인하고 HNS ROM에 반영한다. 원본 PNG 및 PAL의 픽셀·색상 내용은 편집하지 않았다.
- 산출물: `tourney_buttons.png`(32×96, 4-bit indexed)에서 `tourney_buttons.4bpp`(1,536바이트)를, `tourney_buttons.pal`에서 `tourney_buttons.gbapal`(512바이트)를 재생성했다. 이어서 4bpp를 `tourney_buttons.4bpp.smol`(464바이트)로 압축했다. 독립 임시 변환·압축본과 세 산출물이 모두 `cmp`에서 일치했다.
- 참조 확인: `src/graphics.c`의 `gDomeTourneyTreeButtons_Gfx`/`_Pal`이 각각 이 `.smol`/`.gbapal`을 포함하며, `src/battle_dome.c`가 배틀돔 토너먼트 트리·정보 카드에서 로드한다. 새 HNS `graphics.o`에서 그래픽 `0x1D0`, 팔레트 `0x200` 심볼을 확인했다.
- 빌드 검증: HNS 그래픽 객체는 20:34:21, `pokehns.elf`·`.map`은 20:36:30, `pokehns.gba`는 20:36:30에 생성됐다. 새 ROM SHA-256은 `d19195c658b195c3d62403c9e4540f2e127b7fc26da3c1f348a4a73294dee50d`다.
- 게임 화면 확인: 이 환경에서는 mGBA를 실행하지 못했다. 배틀돔 토너먼트 트리에서 포켓볼, 취소, 나가기 버튼의 한글·선택 프레임·색상·잘림을 확인한다.

### 2026-09-18 — `menu_info.png` 재수정본 재변환

- 요청/범위: 사용자가 다시 수정한 `graphics/interface/menu_info.png`를 4bpp로 변환하고 HNS 빌드에 반영했다. 사용자 PNG는 추가 편집하지 않았다.
- 변환/팔레트 검증: `gbagfx`로 `menu_info.4bpp`를 재생성했다. 별도 임시 경로의 독립 변환본과 실제 8,192바이트 산출물이 `cmp`에서 일치했다. 수정 PNG의 첫 16색은 가방 TM/HM·유니언룸이 쓰는 `menu_info2.gbapal`과 같은 순서이므로 타입 아이콘의 색상 인덱스가 어긋나지 않는다.
- 빌드 반영: `build/hns/src/graphics.o`(19:42:49), `pokehns.elf`·`.map`(19:43:05), `pokehns.gba`(19:43:06)가 최신 4bpp보다 뒤의 시각으로 생성된 것을 확인했다. ROM SHA-256은 `dac51e252c8311f3e45e906cf6f20846bb41334200c6d5dd73f76120afdcfbd5`다. 이번 수정이 픽셀 인덱스·공유 팔레트를 바꾸지 않아 직전 ROM과 해시가 같은 것은 정상이다. 로그 `build/localization-logs/hns-menu-info-ko-20260918.log`.
- 게임 화면 확인: 이 환경에서는 mGBA를 실행하지 못했다. 가방의 TM/HM 정보와 유니언룸 교환 목록에서 `타입/위력/명중/PP` 및 여러 타입 아이콘의 한글·색상·잘림을 확인한다.

### 2026-09-18 — `menu_info.png` 한글화 재변환 및 타입 아이콘 색상 확인

- 요청/범위: 사용자가 번역·수정한 `graphics/interface/menu_info.png`를 게임용 그래픽으로 변환하고, 타입 아이콘 색상에 팔레트 문제가 없는지 확인했다.
- 수정 파일/산출물: 사용자 PNG 내용은 추가로 편집하지 않았다. `graphics/interface/menu_info.4bpp`만 `gbagfx`로 재생성했다. `menu_info1.gbapal`~`menu_info3.gbapal`은 사용자 변경이 없고 수정하지 않았다.
- 확인/결정: `src/menu.c`의 `BlitMenuInfoIcon()`은 타입 아이콘과 `타입/위력/명중/PP` 라벨을 같은 시트에서 복사한다. 가방 TM/HM 정보 및 유니언룸 교환 목록은 `ListMenuLoadStdPalAt(..., 1)`로 `menu_info2.gbapal`을 사용한다. 수정 PNG의 첫 16색 팔레트 순서가 이 런타임 팔레트와 정확히 일치했고, 변환 4bpp도 0~F 인덱스를 정상 사용한다. 따라서 번역된 타입 아이콘은 기존 색상을 유지하며 동적 팔레트 방식도 아니다.
- 검증: 독립 임시 경로에서 변환한 `menu_info.4bpp`와 실제 생성물을 `cmp`로 대조해 일치했다(8,192바이트). `build/hns/src/graphics.o` 재생성 후 HNS를 재링크해 `pokehns.elf`·`.map`·`.gba`가 갱신됐다. ROM SHA-256 `dac51e252c8311f3e45e906cf6f20846bb41334200c6d5dd73f76120afdcfbd5`; 로그 `build/localization-logs/hns-menu-info-ko-20260918.log`.
- 게임 화면 확인: 이 환경에는 mGBA 실행 파일이 없어 미수행.
- 다음 시작점: 가방에서 TM/HM을 골라 한글 라벨·타입 아이콘의 색상과 잘림을 확인한다. 가능하면 유니언룸 교환 목록의 타입 아이콘도 확인한다.

### 2026-09-18 — 상점 화폐·메뉴 상태 아이콘 재변환 및 팔레트 검사

- 요청/범위: 사용자가 수정한 `graphics/shop/money.png`, `graphics/interface/status_icons.png`를 변환하고, 메뉴 상태 아이콘에서 이전 배틀 체력바 화상처럼 색이 틀어질 위험이 있는지 확인했다.
- 수정 파일/산출물: 사용자 PNG 내용은 추가 편집하지 않았다. `money.4bpp`, `money.4bpp.smol`, `status_icons.4bpp`, `status_icons.gbapal`, `status_icons.4bpp.smol`을 공식 `gbagfx`·`compresSmol` 도구로 재생성했다.
- 확인/결정: `status_icons`는 `gStatusGfx_Icons`와 `gStatusPal_Icons`를 함께 로드하는 파티 메뉴·요약 화면용 독립 스프라이트이며, 동적 상태색 팔레트 슬롯을 쓰지 않는다. 변환 결과는 1024바이트 4bpp와 32바이트(16색) 팔레트이고 0~F 인덱스가 모두 자체 팔레트에 유효하다. 따라서 체력바 상태 아이콘에서 발생했던 팔레트 인덱스 불일치가 없다. `money`는 `gShopMenu_Pal`을 사용하는데, 수정 PNG의 파생 팔레트 0~14가 상점 공용 팔레트의 같은 순서와 일치해 색상 호환성을 확인했다.
- 검증: 독립 임시 변환·압축본과 실제 생성물 다섯 개를 `cmp`로 모두 대조했다. `build/hns/src/graphics.o`를 재생성해 새 INCBIN 자산을 포함하도록 HNS를 재빌드했고, `pokehns.elf`·`.map`·`.gba`가 갱신됐다. `pokehns.gba` SHA-256 `dab282490b8aa55e82964d21a8f9203a5213596791823f1063dcde7af7612761`; 로그 `build/localization-logs/hns-status-icons-money-20260918.log`.
- 게임 화면 확인: 이 환경에는 mGBA 실행 파일이 없어 미수행.
- 다음 시작점: 상점의 소지금 라벨, 파티 메뉴·요약 화면의 모든 상태 아이콘을 새 ROM에서 확인한다.

### 2026-09-18 — HNS·Gen4 배틀 상태 아이콘 한글화 재변환

- 요청/범위: 사용자가 번역·수정한 HNS 및 Gen4 배틀 인터페이스 `status.png`~`status4.png` 여덟 PNG를 게임용 4bpp로 변환하고 HNS 빌드에서 테스트한다.
- 수정 파일/산출물: 원본 PNG 여덟 개는 사용자 변경을 보존하고 편집하지 않았다. 각각의 대응 `.4bpp`만 `gbagfx`로 재생성했다. `src/graphics.c` 변경은 없다.
- 변환 확인: 입력 PNG는 모두 24×48 indexed PNG이고 출력 `.4bpp`는 모두 576바이트다. 임시 디렉터리에서 원본 여덟 개를 독립 재변환해 현재 `.4bpp`와 각각 `cmp`로 비교했고 모두 일치했다. HNS 네 PNG의 내용 해시는 동일하므로 네 산출물도 같은 해시인 것이 정상이며, Gen4 네 PNG·산출물은 서로 구별된다.
- 빌드 검증: `src/graphics.c:731-757`의 Gen4/HNS 체력바 요소 테이블이 여덟 `.4bpp`를 직접 `INCBIN`하는 것을 확인했다. `make --jobserver-style=pipe hns -j8` 종료 코드 0; `graphics.c` 재컴파일 후 링크 완료. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,325,684/33,554,432(99.32%). `pokehns.gba` 33,554,432바이트, SHA-256 `cccf0e6d0c28cc1e0729cc2d9f28a9cd3ce72d4fa57a07f8ad0739ae29825307`. 로그 `build/localization-logs/hns-battle-status-icons-ko-20260918.log`.
- 게임 화면 확인: 이 환경에서는 mGBA를 실행하지 못해 미확인이다.
- 남은 문제: 코드·변환·빌드 기준 문제 없음. 기본 UI 및 4세대 UI의 싱글/더블 체력바에서 각 상태 아이콘의 한글 표기, 팔레트, 잘림을 실제 확인한다.
- 다음 시작점: 새 `pokehns.gba`에서 UI를 각각 선택한 뒤 디버그 메뉴 또는 상태이상 기술로 독·마비·화상·빙결/동상을 재현해 4개 상태 시트의 모든 칸을 점검한다.

### 2026-09-18 — HNS·Gen4 상태 아이콘 동적 팔레트 복구

- 요청/범위: 기본 HNS와 4세대 UI의 상태 아이콘에서 화상을 원래 빨간색으로 복구하고, 독·마비·수면·얼음/동상도 같은 팔레트 오류가 없는지 확인·수정했다.
- 수정 파일: `graphics/battle_interface/hns/status.png`~`status4.png`, `graphics/battle_interface/gen4/status.png`~`status4.png`.
- 변경과 결정 이유: 상태 아이콘은 자체 팔레트 색이 아니라 `UpdateStatusIconInHealthbox()`가 배틀러별 동적 슬롯에 주입하는 상태색을 사용한다. PNG의 8-bit 인덱스 하위 4비트가 `C` 또는 `F`인 배경 픽셀을 배틀러별 대상 슬롯으로 통일했다: `status`=`C`, `status2`=`D`, `status3`=`E`, `status4`=`F`. 한글 글자·그림자 색상 인덱스(2·3, Gen4의 4)는 바꾸지 않았다. 이로써 화상에는 `PAL_STATUS_BRN`의 적색, 독·마비·수면·얼음/동상에도 각각 코드의 원래 상태색이 적용된다.
- 검증: 모든 PNG가 24×48 indexed 형식이고 각 생성 `.4bpp`가 576바이트임을 확인했다. 임시 경로에서 여덟 PNG를 독립 변환해 실제 산출물과 `cmp`로 모두 일치시켰다. 각 시트에서 대상 슬롯 이외의 `C/F` 참조가 0개임을 확인했다. `make hns -j8`로 `pokehns.gba`·`.elf`·`.map` 재링크를 완료했고, ROM SHA-256은 `12759db12bfa1bc70a385967295f73b8ad7a1198f25eb12ff1b5df02f3a5bdb4`다. 빌드 출력 경로는 `build/localization-logs/hns-status-icon-palette-fix-20260918.log`이나, 환경의 실행 래퍼가 표준 출력을 남기지 않아 로그 내용은 비어 있다.
- 게임 화면 확인: 이 환경에 mGBA 실행 파일이 없어 미수행.
- 남은 문제: 코드·변환·ROM 링크 기준 문제 없음. 실제 싱글/더블 배틀에서 기본 UI와 Gen4 UI의 모든 상태색을 확인한다.
- 다음 시작점: 새 `pokehns.gba`에서 독·마비·수면·얼음(기본 UI 동상 포함)·화상을 차례로 걸고, 배틀러 0~3 상태 아이콘의 배경색 및 한글 글자를 점검한다.

### 2026-09-18 — Gen4 화상 아이콘 파란 배경 원인 확인

- 요청/범위: 4세대 배틀 UI에서 `화상` 아이콘의 배경이 빨간색 대신 파랗게 나오는 원인을 조사했다. 코드·그래픽·ROM은 변경하지 않았다.
- 확인: `src/battle_interface.c`의 `UpdateStatusIconInHealthbox()`는 `STATUS1_BURN`에 대해 `PAL_STATUS_BRN`을 선택하고, 배틀러별 동적 상태 색상 슬롯(플레이어 첫 배틀러는 4bpp 팔레트 인덱스 `C`)만 붉은 계열로 교체한다. 따라서 상태 판정은 정상이다.
- 원인: `graphics/battle_interface/gen4/status.4bpp`의 화상 행(타일 12–14)을 원본과 비교하면 인덱스 `C`는 105픽셀에서 20픽셀로 줄었고, 92픽셀이 인덱스 `F`로 바뀌었다. 이 UI의 인덱스 `F`는 고정 파란색이므로, 번역 과정에서 `C`를 유지하지 못한 화상 배경 픽셀이 파란색으로 출력된다.
- 다음 작업: 사용자가 수리를 요청하면 `gen4/status.png`의 화상 배경을 동적 색상 인덱스 `C`로 복구하고, 배틀러별 시트 `status2.png`~`status4.png`도 같은 팔레트 인덱스 규칙을 점검한 뒤 HNS 빌드와 게임 화면으로 확인한다.

### 2026-09-18 — Gen4 메가진화 트리거를 SoulGold 위치로 조정

- 요청/범위: Gen4 전용 메가 트리거 그래픽은 적용됐지만 기존 UI 위치에 남아 아래로 처져 보이는 문제를 SoulGold 코드 기준으로 수정한다. 4세대 UI의 메가진화 트리거에만 적용하고 기존 UI는 보존한다.
- 수정 파일: `src/battle_gimmick.c`, `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`. 앞서 추가한 Gen4 그래픽·팔레트와 사용자 PNG/PAL 내용은 변경하지 않았다.
- 출처: [Eemeliri/soulgold `src/battle_gimmick.c`](https://github.com/Eemeliri/soulgold/blob/master/src/battle_gimmick.c), 2026-09-18 확인. SoulGold 싱글 좌표는 `34/36/16/-7`(`xOptimal/xPriority/xSlide/yDiff`), 더블은 `34/36/16/-3`이다.
- 변경과 결정 이유: HNS 기존 좌표 `30/31/15/-11`(싱글), `30/31/15/-4`(더블)는 기존 UI용으로 유지했다. `UseGen4BattleUI()`이고 해당 배틀러의 사용 기믹이 `GIMMICK_MEGA`인 경우에만 SoulGold 좌표 구조체를 반환하는 `GetGimmickTriggerPosition()`을 추가했다. 최초 생성과 콜백이 같은 구조체를 사용해 등장/퇴장 이동, 최종 위치, 우선순위, 체력바 이동 추적이 서로 어긋나지 않는다. Z기술·울트라버스트·다이맥스·테라스탈은 Gen4 UI에서도 기존 위치를 유지한다.
- 검증: `git diff --check -- src/battle_gimmick.c src/data/graphics/gimmicks.h` 통과. `make --jobserver-style=pipe hns -j8` 종료 코드 0; EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,325,684/33,554,432(99.32%). `pokehns.gba` 33,554,432바이트, SHA-256 `5809c9d4f9d04ede99e76c65229ea48ce7fda529d12a2f4a35f832ccfb71bd07`. 로그 `build/localization-logs/hns-gen4-mega-trigger-position-20260918.log`.
- 게임 화면 확인: 이 환경에서는 mGBA를 실행하지 못해 미확인이다.
- 남은 문제: 코드·빌드 기준 문제 없음. 4세대 UI 싱글·더블 화면에서 아이콘 두 프레임 및 등장/퇴장 슬라이드를 실제 확인해야 한다.
- 다음 시작점: 새 `pokehns.gba`에서 4세대 UI로 싱글 및 가능하면 더블 배틀의 메가진화 기술 선택 화면을 띄워 위치를 SoulGold 참조 화면과 비교한다. 기존 UI에서도 위치가 바뀌지 않았는지 확인한다.

### 2026-09-18 — Gen4 배틀 UI 전용 메가진화 트리거 적용

- 요청/범위: 설정에서 4세대 배틀 UI를 선택한 경우에만 사용자가 `graphics/battle_interface/gen4/`에 추가한 SoulGold용 `mega_trigger`를 사용하고, 기존 UI 자산과 다른 기믹은 유지한다.
- 수정 파일: `src/data/graphics/gimmicks.h`, `src/battle_gimmick.c`, `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`. 사용자 원본 `graphics/battle_interface/gen4/mega_trigger.png`·`.pal`은 내용 수정 없이 참조했으며, 빌드 입력인 `.4bpp`·`.gbapal`도 독립 변환 결과와 일치했다.
- 변경과 결정 이유: 기존 메가 트리거는 `gGimmicksInfo[GIMMICK_MEGA]`의 한 시트·팔레트만 항상 로드했다. Gen4 전용 시트·팔레트를 별도로 정의하고 `CreateGimmickTriggerSprite()`에서 `usableGimmick == GIMMICK_MEGA && UseGen4BattleUI()`일 때만 로드 포인터를 교체했다. 따라서 설정의 기존 `newBattleUI` 값과 정확히 연동되며 기본 UI, Z기술, 울트라버스트, 다이맥스, 테라스탈 경로는 바뀌지 않는다.
- 검증: `git diff --check` 통과. 임시 디렉터리에서 `gbagfx`로 변환한 Gen4 `.4bpp`·`.gbapal`과 저장소 산출물을 `cmp`로 비교해 모두 일치(종료 코드 0)했다. ELF에 기본/Gen4 `sMegaTriggerGfx`·`sMegaTriggerPal`이 모두 포함된 것도 확인했다. `make --jobserver-style=pipe hns -j8` 종료 코드 0; EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,325,572/33,554,432(99.32%). `pokehns.gba` 33,554,432바이트, SHA-256 `9e64ed53f81633f2f7d599926493b3aceb3e8adb08e0d3f7e95aee722efe221b`. 로그 `build/localization-logs/hns-gen4-mega-trigger-20260918.log`.
- 게임 화면 확인: 이 환경에서는 mGBA를 실행하지 못해 미확인이다.
- 남은 문제: 코드·빌드 기준 문제 없음. 실제 화면에서 Gen4 UI의 꺼짐/켜짐 두 프레임이 체력바와 맞는지 확인이 필요하다.
- 다음 시작점: 새 `pokehns.gba`에서 설정을 `배틀 UI: 4세대`로 바꿔 메가진화 가능한 포켓몬의 기술 선택 화면을 확인하고, 기존 UI로도 다시 확인해 각 자산이 선택되는지 비교한다.

### 2026-09-18 — L/R 버튼 전체와 라벨 앞 1픽셀 간격 동시 보존

- 사용자가 직전 수정에서 버튼의 잘림은 없어졌지만 `노력치`·`개체값` 앞의 1픽셀 간격이 사라졌다고 확인했다. 버튼을 오른쪽으로 옮기는 방식은 간격을 소비하므로 폐기하고, 버튼과 한글 라벨은 공통 번역 PNG의 원래 위치로 복원했다.
- 타일 경계를 넘은 버튼 왼쪽 열을 표시하기 위해 미사용 타일 218·234를 전용 합성 타일로 구성했다. 각각 원래 버튼 앞 칸의 7개 열을 복사하고 마지막 열에는 R/L 버튼의 왼쪽 테두리를 넣었다. `graphics/summary_screen/page_skills.bin`과 HNS 사본에서 y=6, x=23의 타일을 239→218, x=18의 타일을 175→234로 바꿨다.
- 정적 렌더로 버튼 왼쪽 테두리, 버튼 오른쪽 1픽셀 간격, L/R 및 `노력치`·`개체값` 글자가 모두 유지됨을 확인했다. 기본 PNG와의 차이는 전용 타일 영역의 26픽셀뿐이다.
- 독립 변환 결과 `.4bpp`, `.gbapal`, `.4bpp.smol`, `page_skills.bin.smolTM`이 빌드 산출물과 모두 일치했다. `make --jobserver-style=pipe hns -j8` 성공. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,452/33,554,432(99.31%). `pokehns.gba` SHA-256 `043c4acabf098c280f4fce87c6b74d309bc202c43ab674244ceb302557c48c05`. 로그 `build/localization-logs/hns-summary-lr-gap-preserved-20260918.log`.
- 실제 mGBA 화면은 이 환경에서 실행할 수 없어 사용자 확인 대기다.

### 2026-09-18 — L/R 버튼을 타일 경계 안으로 복원

- 원인: `개체치`를 `개체값`으로 바꾸며 라벨과 L/R 버튼을 왼쪽으로 옮긴 결과, 버튼 그림 범위가 x=87~99가 됐다. 그러나 스킬 페이지가 사용하는 버튼 타일은 x=88부터여서 x=87의 왼쪽 세로 테두리가 앞 타일에 남고 인게임에서는 표시되지 않았다.
- 수정: `graphics/summary_screen/hns/tiles.png`의 두 버튼 영역(x=87~99, y=104~111 및 112~119)을 각각 x=88~100으로 1픽셀 오른쪽 이동하고 x=87을 배경색으로 정리했다. 변경 104픽셀은 x=87~100, y=104~119에만 존재한다. `노력치`·`개체값` 글자, 타일맵, C 코드는 변경하지 않았다.
- 검증: `page_skills.bin`을 현재 타일 시트로 정적으로 렌더해 두 버튼의 온전한 왼쪽 테두리와 L/R 글자 및 라벨을 확인했다. `make --jobserver-style=pipe hns -j8` 성공. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,436/33,554,432(99.31%). `pokehns.gba` 33,554,432바이트, SHA-256 `1decc74809f74bb85130ca359f4cbf213386e1ea104613bb8a61a1777a64cc04`. 로그 `build/localization-logs/hns-summary-lr-tile-boundary-fix-20260918.log`.
- 실제 mGBA 화면은 이 환경에서 실행할 수 없어 사용자 확인 대기다. 이전의 x=88 세로선 추가 방식과는 다른 수정이며, 그 실패 ROM SHA `6165a48e…`는 사용하지 않는다.

### 2026-09-18 — L/R 테두리 재발 보고, 이전 실패 후보 재생성 실수 및 기준 상태 복원

- 사용자가 기준 ROM에서 선이 사라지고 왼쪽 잘림이 다시 나타났다고 보고했다. 현재 작업 트리에서 누락 영역을 확인한다며 x=88, y=106–109 및 y=114–117의 8픽셀을 테두리 인덱스 6으로 바꿨으나, 빌드 결과 SHA-256 `6165a48e7a7378611703097310441d8d50c923083fc85750c3d6275714f65b16`가 이미 사용자가 잘못된 선을 보고 거부했던 후보 ROM과 완전히 같았다. 이 수정은 다시 실패로 판정하고 사용하면 안 된다.
- 공통 `graphics/summary_screen/tiles.png`와 HNS 파일이 후보 편집 전 동일 SHA `e37b7aa80f2171d8cc79731b01da7d7437ebfd8457308aa1c2d15b496b33bd4b`임을 확인한 뒤, 그 바이트를 이용해 `graphics/summary_screen/hns/tiles.png`를 해당 상태로 복구했다. 타일맵·C 코드·글자와 버튼의 배치는 수정하지 않았다.
- 기준 상태로 `make --jobserver-style=pipe hns -j8` 성공. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,436/33,554,432(99.31%). 최종 `pokehns.gba` 33,554,432바이트, SHA-256 `9ff4d8a32ce862512dc5e09fe9ffad8b6d66649d04ba846ee43f2d5901409472`. 로그 `build/localization-logs/hns-summary-lr-baseline-restore-20260918.log`.
- 상태: 문제 미해결. 타일맵의 정적 계산과 사용자가 실제 게임에서 본 선 위치가 맞지 않으므로, 같은 8픽셀 변경을 재적용하지 않는다. 다음 작업에서는 기준 잘림 화면과 이전 실패 후보 화면을 동일 배율로 좌표 대조한 후에만 픽셀을 조정한다.

### 2026-09-17 — L/R 왼쪽 테두리 수정 오판 정정 및 잘못된 선 제거

- 사용자가 확인한 대로 이전 `tiles.png` x=88 수정은 L/R 상자 가장자리를 복구하지 못했고 인게임 화면 가장자리에 엉뚱한 선을 만들었다. 타일 시트 픽셀과 화면 좌표를 잘못 대응한 판단이었다. 아래의 “왼쪽 테두리 1픽셀 복구” 기록은 잘못된 결과이므로 무효 처리한다.
- `graphics/summary_screen/hns/tiles.png`를 해당 8픽셀 변경 직전 사용자 편집 상태(SHA-256 `e37b7aa80f2171d8cc79731b01da7d7437ebfd8457308aa1c2d15b496b33bd4b`)로 되돌렸다. 타일맵, C 코드, 글자와 버튼 배치는 이 되돌림에서 변경하지 않았다.
- 복구된 그래픽으로 HNS 재빌드 성공. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,436/33,554,432(99.31%). `pokehns.gba`: 33,554,432바이트, SHA-256 `9ff4d8a32ce862512dc5e09fe9ffad8b6d66649d04ba846ee43f2d5901409472`. 로그 `build/localization-logs/hns-summary-lr-corrective-rebuild-20260917.log`.
- 이 ROM은 잘못 생긴 선을 제거하기 위한 복구본이며 실제 mGBA 화면은 미확인이다. 사용자가 지적한 L/R 상자 왼쪽 1픽셀 자체는 아직 해결되지 않았다. 다음에는 새 ROM의 실제 화면에서 버튼 위치와 BG 타일 좌표를 정확히 대응시킨 뒤 필요한 픽셀만 수정한다.

### 2026-09-17 — 요약 화면 L/R 왼쪽 테두리 1픽셀 복구

- 요청/범위: `노력치`·`개체값` 글자와 L/R 글자 및 상자의 위치는 유지하고, 상자의 왼쪽에 빠진 세로 1픽셀만 복구.
- 수정: `graphics/summary_screen/hns/tiles.png`에서 시트 좌표 x=88, y=106–109 및 y=114–117의 픽셀 8개만 팔레트 인덱스 41에서 테두리색 인덱스 6으로 변경했다. 전후 픽셀 비교로 다른 변화가 없음을 확인했다. 공통 `graphics/summary_screen/tiles.png`, 타일맵, 텍스트 및 C 코드는 수정하지 않았다.
- 검증: 별도 `gbagfx`/`compresSmol` 변환 결과 `.4bpp`(7680바이트), `.4bpp.smol`(1892바이트), `.gbapal`(512바이트)이 빌드 산출물과 각각 바이트 단위로 일치했다. `make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,436/33,554,432(99.31%). `pokehns.gba`: 33,554,432바이트, SHA-256 `6165a48e7a7378611703097310441d8d50c923083fc85750c3d6275714f65b16`. 로그 `build/localization-logs/hns-summary-lr-left-edge-20260917.log`.
- 화면 검증: 이 환경에는 mGBA 실행 파일이 없어 새 ROM의 실제 화면을 확인하지 못했다. 사용자가 새 ROM의 능력치 페이지에서 왼쪽 테두리가 복구됐는지와 글자/위치가 유지됐는지 확인해야 한다. 이전의 잘못된 타일 수정 시도 및 원복 기록은 그 당시 이력이며, 이번 8픽셀 수정이 최신 상태다.

### 2026-09-17 — 요약 화면 L/R 왼쪽 1픽셀 잘림 재조사

- 요청/범위: L/R 안내 상자의 왼쪽 픽셀 열이 계속 잘리는 현상을 인게임 출력 범위/배경 오프셋 문제인지 재확인.
- 확인: `ChangeBgX()`는 BG X를 8비트 소수 고정소수점으로 레지스터에 기록한다. 스킬 페이지 전환은 `PssScrollRight()`에서 `0x2000`을 8회 더해 `0x10000`(256px)을 이동하므로 잔여 1px 오프셋이 남지 않는다. 스킬 페이지 맵의 프레임은 타일 열 10~29, 즉 화면 x=80~239px에 있어 화면 왼쪽 경계 바깥에서 잘리는 배치도 아니다. HNS/공통 `tiles.png`는 현재 SHA-256 `e37b7aa80f2171d8cc79731b01da7d7437ebfd8457308aa1c2d15b496b33bd4b`로 동일하고 `page_skills.bin`도 동일하다.
- 결정: BG 전체를 1px 이동하면 스킬 페이지의 모든 프레임과 고정 BG0 창의 글자가 서로 어긋날 수 있어 변경하지 않았다. 이전 타일 추정 수정에서 글자 잘림 회귀가 있었으므로 정확한 L/R 화면 확대 crop과 해당 map cell을 대응시키기 전에는 픽셀/타일을 수정하지 않는다.
- 변경/검증: 문서만 갱신. 소스·그래픽·ROM 변경 및 빌드는 없다. `git diff --check -- docs/localization/STATUS.md docs/localization/SESSION_LOG.md`로 확인한다.
- 다음 단계: 실제 게임에서 잘리는 L/R 상자만 1:1 또는 nearest-neighbor 확대로 캡처/표시해 map 좌표와 8×8 타일을 특정하고, 확인된 타일만 조정한 뒤 HNS 재빌드 및 인게임에서 글자/테두리를 확인한다.

### 2026-09-17 — 요약 화면 타입 아이콘 간격을 한글 라벨에 맞춤

- 요청/범위: 요약 화면에서 `타입/` 라벨과 타입 아이콘 사이 간격이 기존 HNS 화면보다 넓어 보이는 원인을 찾고 조정.
- 원인: 원래 코드가 영문 `TYPE/` 문자열을 기준으로 첫·둘째·셋째 아이콘을 고정 x=120/160/200에 배치했다. HNS 한글 문자열 `타입/`은 폭이 더 좁지만 고정 좌표는 그대로라 라벨 뒤 빈 공간이 커졌다. 기존 영문 정의는 `git show HEAD:src/strings.c`에서도 확인했다.
- 수정: `src/pokemon_summary_screen.c`의 `SetMonTypeIcons()`에서 실제 타입 라벨 창의 시작 x와 `GetStringWidth(FONT_NORMAL, gText_TypeSlash, 0)`로 첫 아이콘 x를 계산하고 2px 간격을 뒀다. 두 번째·세 번째 아이콘 상대 간격(각 40px), 타입·팔레트 선택은 그대로다. 알의 미스터리 아이콘도 같은 앵커를 쓴다.
- 검증: `git diff --check -- src/pokemon_summary_screen.c src/strings.c include/strings.h` 통과. `make hns -j8` 종료 코드 0; EWRAM 249,012B(94.99%), IWRAM 25,704B(78.44%), ROM 33,324,404/33,554,432B(99.31%). `pokehns.gba` 33,554,432바이트, SHA-256 `c25633060e7514d7fcbaeda578f4a9268905587e6d67e6e9938873735eda6d99`. 로그 `build/localization-logs/hns-summary-type-icon-spacing-20260917.log`.
- 게임 화면 확인: 수정 빌드를 mGBA에서 아직 실행하지 않았다. 스크린샷은 수정 전 간격을 확인하는 근거로 사용했다.
- 남은 문제/다음 시작점: 새 ROM에서 일반 포켓몬의 1·2타입 및 알의 미스터리 타입 아이콘이 `타입/` 뒤 적절한 간격으로 나타나는지 확인한다.

### 2026-09-17 — 요약 화면 `앞으로` 추가 원복

- 요청/범위: `앞으로` 문자열을 추가하기 위해 변경했던 요약 화면 코드를 이전 상태로 복원하고, EXP 창을 원래 48px로 되돌린다.
- 수정: `src/pokemon_summary_screen.c`의 EXP 창을 `.tilemapLeft = 24`, `.width = 6`(48px)로 복원하고 현재/다음 레벨 경험치 값 정렬 폭을 72px에서 42px로 되돌렸다. 다음 레벨 행의 `gText_UntilNextLv` 출력도 제거했다. `include/strings.h`에 남아 있던 같은 문자열의 미사용 선언을 삭제했다.
- 보존: 사용자가 수정한 `src/strings.c`의 `gText_NextLv = "다음 레벨까지"`는 유지했다. 이전 별도 요청에서 추가했던 HP `{JPN}` 간격 처리도 이번 되돌리기 범위가 아니므로 유지했다. 현재 작업 트리에서 `gText_UntilNextLv` 정의는 이미 삭제되어 있었다.
- 검증: `git diff --check -- src/pokemon_summary_screen.c include/strings.h src/strings.c` 통과. `make hns -j8` 종료 코드 0; EWRAM 249,012B(94.99%), IWRAM 25,704B(78.44%), ROM 33,324,452/33,554,432B(99.31%). `pokehns.gba` 33,554,432바이트, SHA-256 `77e27005e2520338ed0dc8e30d0cd8b315035feda0c885ef4d6ee293b913c86c`. 로그 `build/localization-logs/hns-summary-forward-revert-20260917.log`. 일부 다른 코드 경고는 있었지만 오류 없이 컴파일·링크됐다.
- 게임 화면 확인: 새 빌드의 mGBA 확인은 아직 수행하지 않았다.
- 남은 문제/다음 시작점: 새 ROM의 요약 화면에서 `다음 레벨까지` 표시, `앞으로` 제거, 48px EXP 창 안의 숫자 배치를 확인한다.

### 2026-09-17 — 경험사탕 레벨업 문구의 경험치/레벨 혼동 수정

- 요청/범위: 경험사탕 XL을 썼을 때 실제 레벨 64로 상승했는데 메시지에 경험치 30000과 레벨 30000이 함께 출력되는 문제를 조사하고, 다른 경험사탕에도 같은 출력 오류가 있는지 확인·수정.
- 원인: `src/party_menu.c`의 `ItemUseCB_RareCandy()`는 경험치 값을 `gStringVar2`, 사탕 적용 후 최종 레벨을 `gStringVar3`에 저장한다. 그런데 한글 메시지 `gText_PkmnGainedExpAndElevatedToLvVar3`가 두 자리 모두 `{STR_VAR_2}`를 사용해 레벨 대신 경험치를 반복 출력했다.
- 수정: `src/strings.c`에서 레벨 자리 토큰만 `{STR_VAR_2}`에서 `{STR_VAR_3}`으로 수정했다. XP 계산이나 사탕 효과는 변경하지 않았다. XS/S/M/L/XL 모두 해당 공통 레벨업 문자열을 사용하므로 모두 포함되며, 레벨업 없는 경험사탕은 기존 경험치 전용 문자열을 사용한다. 이상한사탕은 별도 레벨 전용 문자열과 변수 경로로 처리되어 이상이 없음을 확인했다.
- 검증: `git diff --check -- src/strings.c` 통과. `make hns -j8` 종료 코드 0. EWRAM 249,012B(94.99%), IWRAM 25,704B(78.44%), ROM 33,324,516/33,554,432B(99.31%). `pokehns.gba` 33,554,432바이트, SHA-256 `f5a8c38d40eb17cacfac5fd1875136bd3c24afbbfcaee1de3efedb82eb922f54`. 로그 `build/localization-logs/hns-exp-candy-output-20260917.log`. 빌드 경고는 있었지만 컴파일·링크가 정상 완료됐다.
- 게임 화면 확인: 새 ROM에서 경험사탕 사용을 mGBA로 실행하지 않았다. XL로 63→64일 때 30000 경험치와 레벨64가 각각 맞게 출력되는지, XS/S/M/L이 레벨업을 일으키는 조건에서도 값이 분리되는지 확인한다.
- 다음 시작점: 위 케이스를 새 `pokehns.gba`에서 확인한다.

### 2026-09-17 — 요약 화면 `앞으로` 위치를 잔여 경험치에 맞춤

- 요청/범위: HNS 녹색 경험치 박스 경계에 맞게 `앞으로`의 위치 조정.
- 원인/수정: `앞으로`를 경험치 창의 고정 x=0에 찍어 남은 경험치가 짧은 숫자일 때 둘 사이 간격이 크게 벌어졌다. `PrintExpPointsNextLevel()`에서 남은 값의 x 좌표를 구한 뒤 `GetStringWidth(FONT_NORMAL, gText_UntilNextLv, 0)`와 2px 여백을 빼서 문구를 배치하도록 했다. 숫자와 문구가 오른쪽 끝 기준 한 묶음으로 표시되며 현재 경험치와 레벨업 경험치 계산은 건드리지 않았다.
- 수정 파일: `src/pokemon_summary_screen.c`.
- 검증: `git diff --check -- src/pokemon_summary_screen.c` 통과. `make hns -j8` 종료 코드 0; EWRAM 249,012B(94.99%), IWRAM 25,704B(78.44%), ROM 33,324,516/33,554,432B(99.31%). 새 `pokehns.gba` 33,554,432바이트, SHA-256 `68eb08159e08f92e9889532c481a87275c971b749c9a71413f0790710cd2e0f7`. 로그 `build/localization-logs/hns-summary-forward-position-20260917.log`. 미사용 함수 경고 1건 외 오류 없음.
- 게임 화면 확인: 새 ROM을 mGBA에서 실행해 보지는 않았다. 인게임에서 `앞으로`와 잔여 EXP 사이 간격, HNS 녹색 박스 오른쪽 여백을 확인한다.
- 다음 시작점: 요약 능력 페이지에서 새 문구 위치 확인.

### 2026-09-17 — 요약 화면 경험치 숫자 잘림 수정

- 요청/범위: 요약 화면에 현재 경험치 및 다음 레벨까지 남은 경험치가 보이지 않고 `앞으로`만 보이는 현상 수정.
- 원인: `PSS_DATA_WINDOW_EXP`의 실제 폭은 6타일(48px)인데 값은 72px 기준 오른쪽 정렬되어 대부분 창 밖으로 잘렸다. pokeemerald-kr의 같은 창은 10타일이다.
- 수정 파일: `src/pokemon_summary_screen.c`의 EXP 데이터 창을 `tilemapLeft=20`, `width=10`(80px)으로 변경했다. 값의 정렬 폭(+기존 2px 여백), `앞으로` 출력, 50레벨 제한 계산은 그대로 뒀다.
- 검증: `git diff --check -- src/pokemon_summary_screen.c` 통과. `make hns -j8` 종료 코드 0, ROM 링크 완료. EWRAM 249,012B(94.99%), IWRAM 25,704B(78.44%), ROM 33,324,500/33,554,432B(99.31%); `pokehns.gba` 33,554,432바이트. 로그 `build/localization-logs/hns-summary-exp-values-visibility-20260917.log`. 남은 경고는 `CB2_PssChangePokemonNickname` 미사용 1건이다.
- 게임 화면 확인: 아직 mGBA에서 새 ROM을 실행하지 않았다. EXP 숫자 두 행과 `앞으로` 다음 잔여량이 표시되는지 확인한다.
- 다음 시작점: 요약 능력치 페이지에서 새 ROM의 두 숫자 표시와 오른쪽 가장자리 잘림을 검증한다.

### 2026-09-17 — HNS 요약 화면 타일 시트 번역 반영

- 요청/범위: 사용자가 수정한 HNS 전용 `graphics/summary_screen/hns/tiles.png`를 빌드에 반영하고 결과 확인.
- 수정 파일: 원본 PNG는 사용자의 번역을 보존했다(128×120, indexed PNG, 3007→2660바이트). 빌드 산출물 `tiles.4bpp`, `tiles.gbapal`, `tiles.4bpp.smol`은 재생성됐다.
- 검증: `git diff --check -- graphics/summary_screen/hns/tiles.png docs/localization/STATUS.md docs/localization/SESSION_LOG.md` 통과. `make hns -j8` 종료 코드 0, HNS 그래픽 객체 컴파일 및 ROM 링크 성공. EWRAM 249,012B(94.99%), IWRAM 25,704B(78.44%), ROM 33,324,500/33,554,432B(99.31%). `pokehns.gba` 33,554,432바이트, SHA-256 `82d7e633616de2613ecdce0e053175c5b7795aa0a57b7995ccc3ccd8ab83c3f7`. 로그 `build/localization-logs/hns-summary-screen-tiles-ko-20260917.log`.
- 화면 확인: 소스 PNG의 한글 타일은 육안 확인했다. mGBA에서 새 ROM의 요약 화면은 아직 실행하지 않았다.
- 남은 문제/다음 시작점: 게임에서 제목·라벨의 타일 배치와 한글 가독성/잘림을 확인한다.

### 2026-09-17 — 요약 화면 타일 시트 두 파일 비교

- 요청/범위: `graphics/summary_screen/tiles.png`와 `iv_ev_tiles.png`의 차이 및 HNS 빌드에서 사용 여부 확인.
- 확인 결과: `tiles.png`는 일반 요약 화면 타일 시트이며, HNS 빌드는 `graphics/summary_screen/hns/tiles.png`를 선택하고 비-HNS 빌드만 루트 `tiles.png`를 사용한다. `iv_ev_tiles.png`는 과거 IV/EV 페이지에서 `STATS` 타일을 `IVs`/`EVs`로 바꿀 별도 타일셋으로 추가됐지만, 현재 소스에는 연결 참조와 `P_SUMMARY_SCREEN_IV_EV_TILESET` 설정이 없어 사용되지 않는다.
- IV/EV 수치와의 구분: 현재 `P_SUMMARY_SCREEN_IV_EV_VALUES TRUE`는 요약 화면의 코드가 IV 등급 문자 대신 실제 수치를 출력하게 한다. 이 설정은 `iv_ev_tiles.png`의 로드 여부와 무관하다.
- 근거/검증: 현재 `src/graphics.c`, `src/pokemon_summary_screen.c`, `include/config/summary_screen.h` 및 전체 소스 참조 검색을 대조했다. `iv_ev_tiles.png`와 `tiles.png`는 모두 128×120, 8-bit indexed PNG다. 확장 측 파일 추가 커밋 `52666fb545`의 원래 설정 주석도 확인했다.
- 변경/빌드: 설명을 위해 STATUS/SESSION_LOG만 갱신했으며 코드·그래픽·ROM 변경 및 빌드는 없다.
- 다음 시작점: 이 파일을 실제 화면에 반영하려면 HNS 전용 `hns/tiles.png`와 타일맵/로딩 코드 연결을 별도 검토한다.

### 2026-09-17 — 요약 화면 HP 및 다음 레벨 경험치 표기

- 요청/범위: pokeemerald-kr 상태 화면처럼 HP 글자 간격을 두고 다음 레벨 경험치 옆에 `앞으로`를 표시.
- 수정: `src/pokemon_summary_screen.c`에서 `PrintPageNamesAndStats()`가 `{JPN}`+`HP` 버퍼를 출력하게 해 H/P 사이 간격을 적용했다. `PrintExpPointsNextLevel()`은 다음 레벨 행 왼쪽에 `gText_UntilNextLv`를 출력하고, 경험치 값 두 행의 오른쪽 정렬 기준을 72픽셀로 조정했다. 50레벨 제한 관련 기존 계산은 보존했다. `src/strings.c`에 `gText_UntilNextLv = "앞으로"` 정의를 추가하고 `include/strings.h` 선언을 사용했다.
- 기준 대조: `../pokeemerald-kr/src/pokemon_summary_screen.c`의 HP `{JPN}` 접두 구성과 `gText_UntilNextLv` 출력 순서를 참고하고 현재 HNS의 폰트·창 좌표 체계를 유지했다.
- 검증: 변경 대상 `git diff --check` 통과. `make hns -j8` 종료 코드 0; EWRAM 249,012B, IWRAM 25,704B, ROM 33,324,580/33,554,432B(99.31%), ROM 파일 33,554,432B. 로그 `build/localization-logs/hns-summary-screen-hp-exp-spacing-20260917.log`.
- 게임 화면 확인: 미수행. 생성된 `pokehns.gba`에서 능력 페이지 HP 간격, `앞으로`와 경험치 숫자 간격·정렬을 확인한다.
- 남은 문제/다음 시작점: 런타임 표시만 확인하면 된다.

### 2026-09-17 — HNS 기본 플레이에서 동상 상태 사용 여부

- 요청/범위: 현재 HNS 빌드에서 Frostbite가 일반 플레이 중 실제 부여될 수 있는지, 관련 설정 메뉴가 없어도 가능한 경로가 있는지 확인.
- 확인 결과: `include/config/battle.h`의 `B_USE_FROSTBITE`는 `FALSE`; `MOVE_EFFECT_FREEZE_OR_FROSTBITE`는 이 경우 `MOVE_EFFECT_FREEZE`로 치환된다. 일반 기술 데이터에 `MOVE_EFFECT_FROSTBITE` 직접 지정은 없고, 일반 맵 스크립트의 동상 부여도 검색되지 않아 통상 플레이 경로에서는 동상이 발생하지 않는다.
- 예외: `include/config/debug.h`에서 `DEBUG_OVERWORLD_MENU TRUE`, 기본 키는 R+START다. 디버그 메뉴의 `Inflict Status1`은 `data/scripts/debug.inc`에서 Frostbite 선택 시 개별 또는 파티 전체에 `STATUS1_FROSTBITE`를 직접 지정하므로 강제 부여가 가능하다. 일반 설정 메뉴가 아닌 디버그 경로다.
- 수정/검증: 파일과 호출부 검색만 수행했고 코드·ROM 변경 및 빌드는 없다. 결론은 `B_USE_FROSTBITE` 매크로 치환, 일반 기술 효과 참조, debug status script를 대조했다.
- 남은 점: Gen4 UI에서 디버그로 동상을 부여하면 `gen4/status*.png`의 FRB 위치에 중복 BRN 그림이 표시될 수 있다는 기존 확인과 일치한다.

### 2026-09-17 — Gen4 UI Frostbite 아이콘 조사

- 요청/범위: 기본/HNS 상태 아이콘 시트와 Gen4 상태 아이콘 시트에서 마지막 항목이 `FRB`와 `BRN`으로 다른 이유 확인.
- 확인 결과: `hns/status.png`~`status4.png`의 여섯 상태 아이콘은 PSN, PRZ, SLP, FRZ, BRN, FRB 순이고, `gen4/status.png`~`status4.png`는 마지막 칸이 BRN 중복이다. 파일들은 24×48이며 같은 여섯 상태 칸을 각 배틀러 그래픽 슬롯에 제공한다. Gen4 UI 자산은 2026-05-09 `88cd5dc06e`에서 별도 구현으로 추가됐다.
- 코드 경로: `UpdateStatusIconInHealthbox()`는 `STATUS1_FROSTBITE` 때 `HEALTHBOX_GFX_STATUS_FRB_BATTLER0`을 요청하고 Gen4 여부에 관계없이 같은 인덱스를 쓴다. 그 인덱스의 그림이 HNS/Gen3에는 FRB지만 Gen4에는 BRN이다. 따라서 Frostbite가 적용되고 Gen4 UI가 선택되면 BRN 그림으로 잘못 표시될 수 있다. 현재 `B_USE_FROSTBITE` 기본값은 `FALSE`다.
- 수정/검증: 조사만 했으며 파일·ROM 변경과 빌드는 없다. Gen4의 Frostbite 지원을 완성하려면 `gen4/status*.png` 네 파일의 마지막 아이콘과 4bpp 산출물을 갱신해야 한다.
- 다음 시작점: 사용자가 Gen4 UI와 Frostbite 동시 지원을 원하면 Gen4 UI 레이아웃/팔레트에 맞춘 FRB 타일을 준비하고 네 파일을 변환한 뒤 HNS 빌드 및 런타임에서 확인한다.

### 2026-09-17 — 공식 HNS Release-v2.0.6 업데이트

- 요청/범위: 현재 2.0.5 작업 트리의 한글 패치, HNS 전용 파일, pokeemerald-expansion 1.17.0 선별 이식 및 이후 사용자 변경을 유지하면서 공식 HNS 2.0.6으로 업데이트했다.
- 기준/백업: `git fetch origin --tags`로 `Release-v2.0.6`(`167aa6d537b109bb229c231ddce4616974c4da71`)을 가져와 `Release-v2.0.5`(`1f42b74dff0e9fe942419845d040663dd829a973`)와 직접 대조했다. 변경은 10개 경로이며 작업 전 파일은 `build/localization-backups/pre-hns-206-20260917.tar.gz`에 저장했다.
- 수정 파일: `data/scripts/debug.inc`, `src/battle_anim_ice.c`, `src/pokemon.c`, `src/script_pokemon_util.c`, `graphics/battle_environment/{cave_water_modern,long_grass_modern,pond_water_modern,sand_modern,tall_grass_modern}/map.bin`, `graphics/title_screen/hns/press_start.png` 및 빌드가 재생성한 대응 그래픽 산출물.
- 변경과 결정 이유: 현재 트리에 미커밋 변경이 800개 이상 누적되어 일반 merge/파일 전체 교체를 사용하지 않았다. 2.0.5와 같던 여섯 바이너리는 공식 2.0.6 내용으로 교체하고, 기존 변경과 겹친 네 텍스트 파일에는 공식 델타만 수동 통합했다. 디버그 테스트 진입점, 아이스볼 단계 인덱스, 고정 포켓몬 및 이로치 이벤트 포켓몬의 Synchronize/Cute Charm 반영이 들어갔다.
- 보존 확인: `src/pokemon.c`의 한글 성격 활용형, `src/string_util.c` 조사 코드, `B_MOVE_DESCRIPTION_BUTTON L_BUTTON`, `P_MODIFIED_MEGA_CRIES FALSE`/`CRY_MODE_HIGH_PITCH`가 남아 있음을 확인했다. 타이틀 PNG SHA-256 `d9e86934...399491`은 공식 2.0.6과 일치하고, 배틀 배경 5개도 태그와 바이트가 일치한다. PNG의 남은 Git 차이는 Windows 마운트의 실행 권한 표시다.
- 검증: 대상 소스 `git diff --check` 통과. `make hns -j8` 종료 코드 0; EWRAM 249,012B(94.99%), IWRAM 25,704B(78.44%), ROM 33,324,484/33,554,432B(99.31%), 결과 ROM 33,554,432B. 로그 `build/localization-logs/hns-206-update-20260917.log`, ROM SHA-256 `520eb643...b5d86`.
- 게임 화면 확인: mGBA 실행 환경이 없어 미수행. 타이틀과 현대식 배틀 배경 5종, 아이스볼 연속 사용 애니메이션, 선두 싱크로나이즈/헤롱헤롱바디 상태에서 고정 및 이로치 이벤트 포켓몬의 성격·성별을 확인한다.
- 남은 문제/다음 시작점: 코드·빌드 검증은 완료됐다. 실제 게임 검증에서 이상이 있으면 위 재현 항목과 `Release-v2.0.6`의 네 커밋을 기준으로 조사한다.

### 2026-09-17 — 포켓블록 화면 성격 표기 접미어 추가

- 요청/범위: 포켓블록을 먹일 포켓몬 선택 화면에서 성격명이 `노력하는 성격`처럼 출력되도록 요청받았다.
- 수정: `src/use_pokeblock.c`에서 `sText_NatureSlash` 값을 `성격`으로 설정하고, `UpdateMonInfoText()`가 `gNaturesInfo[nature].name` + 공백 + 접미어 순서로 문자열을 만든다. 사용자가 활용형으로 변경한 `gNaturesInfo` 명칭은 건드리지 않았다.
- 검증: `git diff --check -- src/use_pokeblock.c` 통과, `make hns -j8` 종료 코드 0. EWRAM 249,012B, IWRAM 25,704B, ROM 33,324,724/33,554,432B(99.32%), `pokehns.gba` 33,554,432B. 로그 `build/localization-logs/hns-pokeblock-nature-text-20260917.log`.
- 게임 화면 확인: mGBA 실행이 불가능해 미수행. 새 ROM의 포켓블록 선택 화면에서 `노력하는 성격`과 긴 성격 문구의 잘림 여부를 확인한다.

### 2026-09-17 — 포켓몬 성격 공식 한국어 명칭 반영

- `src/pokemon.c`의 `gNaturesInfo` 25개 `.name`을 공식 한국어 명칭으로 교체했다: 노력, 외로움, 용감, 고집, 개구쟁이, 대담, 온순, 무사태평, 장난꾸러기, 촐랑, 겁쟁이, 성급, 성실, 명랑, 천진난만, 조심, 의젓, 냉정, 수줍음, 덜렁, 차분, 얌전, 건방, 신중, 변덕. 성격 ID와 능력치·배틀 팰리스·애니메이션 관련 값은 유지했다.
- 같은 이름 필드는 포켓몬 요약 화면의 트레이너 메모와 민트로 바뀐 능력치 성격, 포켓블록 먹이기 메뉴의 `NATURE/이름`, 디버그 메뉴의 포켓몬 생성 성격 선택에도 사용된다. 사파리 배틀 정보용 포매터에도 참조는 있지만, 현재 호출 함수가 아군만 허용하고 사파리의 아군 표시는 건너뛰어 그 상대 전용 호출 지점은 도달 불가다. 도감 진화 조건의 `IF_NATURE` 분기에도 이름 삽입 코드는 있지만 현재 종 데이터에는 해당 조건이 없다.
- 포켓블록 화면에서는 처음 선택된 포켓몬을 표시할 때와 커서 이동 뒤 `UpdateMonInfoText()`를 호출하며, `WIN_NATURE`에 `sText_NatureSlash` + 성격명을 출력한다. 접두어는 별도 `NATURE/` 문자열이므로 현재 완전 한글 표기는 아니며, `GetNature()` 기준이라 민트 성격은 이 줄에 반영되지 않는다. 취소 선택에서는 해당 창을 비운다.
- 배틀프런티어 라운지의 성격 소녀는 선택된 포켓몬의 성격에 맞는 고정 메시지를 보여줄 뿐 성격명 필드는 출력하지 않는다. 따라서 해당 맵 대사는 이번 `.name` 변경의 영향 대상이 아니다.
- 성격 이름 25개와 영문 잔존 0건을 확인하고 `git diff --check -- src/pokemon.c`를 통과했다. `make hns -j8` 성공, EWRAM 249,012B(94.99%), IWRAM 25,704B(78.44%), ROM 33,324,468B(99.31%); 결과 ROM은 33,554,432B다. 로그: `build/localization-logs/hns-natures-ko-20260917.log`.
- 첫 빌드 링크는 선행 중단 빌드의 0바이트 `build/hns/src/pokemon.o` 때문에 실패했다. 이 생성 오브젝트만 제거해 재컴파일했고 재빌드는 성공했다. mGBA 실행 파일이 없어 게임 화면 검증은 미수행이다.

### 2026-09-17 — 타입 아이콘 PNG 4bpp 재변환

- `graphics/types/`에서 수정된 타입 아이콘 PNG 24개를 확인했다. 변환 범위를 빠뜨리지 않도록 `mystery.png`·`none.png`를 포함한 개별 타입/콘테스트 아이콘 26개를 `tools/gbagfx/gbagfx`로 `.4bpp`에 재생성했다. `battle_icons1/2.png`와 `move_types` 시트는 수정되지 않아 기존 `.4bpp`·압축 산출물을 보존했다.
- 개별 아이콘 입력은 32×16 인덱스 컬러 PNG이며 `file`로 4-bit/8-bit colormap 형식을 확인했다. 26개 산출물은 각각 256바이트이고, 임시 경로에 같은 명령으로 재변환한 결과와 실제 파일을 `cmp`해 전부 일치했다. 함께 확인한 `battle_icons1/2.png`도 기존 각 640바이트 `.4bpp`와 독립 변환 결과가 일치했다.
- `make hns -j8` 성공(로그 `build/localization-logs/hns-type-icons-20260917.log`). GNU make의 FIFO 작업 큐가 stale 상태여서 `/tmp/GMfifo3`·`GMfifo4`를 정리한 뒤 `--jobserver-style=pipe`를 명시해 빌드했다. 링크 메모리는 EWRAM 249,012B(94.99%), IWRAM 25,704B(78.44%), ROM 33,324,420B(99.31%)이다.
- `src/graphics.c`의 런타임 참조는 배틀 아이콘의 `battle_icons1/2.4bpp.smol`, 기술·요약 화면 타입 라벨의 `move_types.4bpp.smol` 시트다. 사용자가 제공한 최신 mGBA 캡처에서 요약 화면 `불꽃`·`비행` 라벨 출력이 확인되어 현재 ROM의 압축 시트에도 번역이 반영된 상태임을 정정 기록했다. 향후 개별 PNG 수정 후 화면이 갱신되지 않을 경우에만 시트를 재생성한다.
- 로컬에는 mGBA 실행 파일이 없어 캡처 외의 배틀·기술 선택 화면은 직접 재현하지 못했다. 새 ROM에서 한글 픽셀, 색상 인덱스, 타일 경계와 팔레트를 추가 확인한다.

### 2026-09-17 — 파티 메뉴 `MENU_READ` 출력 조건 확인

- `MENU_READ`는 포켓몬이 지닌 메일을 읽는 선택지다. 필드 파티 메뉴에서 선택한 포켓몬의 `MON_DATA_HELD_ITEM`이 `ItemIsMail()`이면 `MENU_MAIL`이 추가되고, `MAIL` 하위 메뉴의 `ACTIONS_MAIL` 배열이 `READ / TAKE / CANCEL`을 출력한다.
- `READ` 선택 시 `CursorCb_Read()`가 `CB2_ReadHeldMail()`과 `ReadMail()`을 호출해 메일 화면으로 전환한다. 메일을 지니지 않은 포켓몬 또는 전투/특수 파티 메뉴에서는 해당 항목이 생성되지 않는다.

### 2026-09-17 — 파티 메뉴 기술·폼 변경 항목 출력 조건 확인

- 필드 파티 메뉴의 `LEARN MOVES`는 `P_PARTY_MOVE_RELEARNER`가 활성화되고 포켓몬에게 복습 가능한 기술이 하나라도 있을 때 출력된다. 하위 메뉴의 네 항목은 레벨업·알·기술머신·기술 가르침 분류별로 미습득 기술이 있을 때만 추가되며, 선택 시 해당 `gMoveRelearnerState`로 기술 복습 화면을 연다.
- `Light bulb`·`Microwave oven`·`Washing machine`·`Refrigerator`·`Electric fan`·`Lawn mower`는 로토무카탈로그 사용 뒤의 로토무 폼 선택지다. `Change form`·`Change Ability`는 지가르데큐브 사용 뒤의 다중 폼 변경 선택지다.
- `{PKMN} FOLLOWER`는 필드에서 첫 생존·비알 포켓몬에게만 추가되고, `CursorCb_PkmnFollower()`가 `followerEnable` 설정을 켜고 끈다.

#### `LEARN MOVES` 상세 흐름

- 필드 파티에서 예를 들어 현재 레벨의 레벨업 기술을 잊은 리자몽을 선택하면, `P_PARTY_MOVE_RELEARNER`와 `CanBoxMonRelearnAnyMove()` 조건을 만족해 `LEARN MOVES`가 추가된다. 네 분류 중 하나만 가능해도 이 항목은 표시된다.
- `CursorCb_LearnMovesSubMenu()` 이후 `SetPartyMonLearnMoveSelectionActions()`가 분류별 가능 여부를 다시 검사한다. 레벨업·기술머신만 가능하면 `LEVEL MOVES / TM MOVES / CANCEL`처럼 일부 항목만 표시되고, 분류 선택 뒤 해당 `gMoveRelearnerState`로 복습 화면이 시작된다.

### 2026-09-16 — 20:34 KST 이후 HP 퍼센트 옵션 추가분 롤백 및 move info 재검증

- 사용자가 지정한 20:34 KST 요청 직전 상태로 범위를 한정해 `hpPercentageDisplay` 저장 비트, 배틀 설정 `퍼센트 표시` 메뉴와 두 설명, 저장/로드·새 게임·오박사 전달, 저장값 기반 상대 HP 분기를 제거했다. 20:34 이전부터 있던 `B_HP_PERCENTAGE_DISPLAY` 상수 기반 코드와 다른 HNS 변경은 보존했다.
- `move_info_window_l.png`·`move_info_window_r.png`를 `tools/gbagfx/gbagfx`로 각각 4bpp 변환하고, 빌드 후 독립 변환 결과와 바이트 단위로 대조했다. L 결과 SHA-256은 `75855e5511e28d9f22cf867c786faa40c0c67599e0bac2814bb046472a329e47`, R 결과는 `2129d2f02056f1fb5ecff5660fee9f2970b471279baaff6060121180e41d20e7`이다.
- `make hns -j8` 성공(로그 `build/localization-logs/hns-rollback-move-info-20260916.log`), 롤백 대상 파일(기존의 별도 `oak_speech_hns.c` 변경 제외)의 `git diff --check` 및 옵션 추가 식별자 잔존 검색 통과. mGBA가 없어 실제 화면 검증은 미수행이다.

### 2026-09-16 — HP 퍼센트 화면의 회색 점선 진단 정정

- 사용자가 OFF 화면에서는 회색 점선이 출력되지 않는다고 지적해 이전 진단을 재검토했다.
- OFF 캡처에서 HP 박스 아래 `EXP` 글자 오른쪽의 회색 점선은 HNS `expbar.png`의 미충전 EXP 바 타일이며, `MoveBattleBarGraphically()`의 `EXP_BAR` 경로가 그린다. HP 바와 구분해야 한다.
- HP 바 옆의 선이 정말 ON에서만 추가로 나타나는 것이라면, 현재 `IsHpPercentageDisplayEnabled()` 분기는 상대 체력박스에만 적용되고 아군 출력 코드는 변경하지 않는다. 따라서 옵션의 정상 출력이 아니라 스프라이트/VRAM 잔상 또는 위치 오인 여부를 확대 캡처로 추가 확인해야 한다.
- 이전 세션 기록의 “아군 PNG에 항상 포함된 점선” 단정은 정정한다. 코드·그래픽 수정은 하지 않았다.

### 2026-09-16 — 상대 HP 퍼센트 표시 화면 아티팩트 원인 확인

- 사용자 mGBA 캡처에서 퍼센트 표시 ON 시 상대 체력박스 아래 흰 사각형과 박스 밖 퍼센트가 보이고, 아군 HP 바 위에 회색 점선이 보이는 현상을 확인 요청받았다.
- 소스 대조 결과, `PrintHpPercentageOnHealthbox()`는 HNS 상대 싱글의 기존 64×32 OAM 두 장에 `FillSpriteRectColor(32, 24, 24, 8, HEALTHBOX_BG_INDEX)`와 y=21 텍스트를 적용한다. 퍼센트 아래 프레임을 제공하는 128×64 확장 자산/OAM 전환이 빠져 투명 영역이 흰 블록으로 드러나는 구조적 문제다. 공식 1.17.0의 `healthbox_singles_opponent_large`·64×64 OAM·Y=22 조합이 이 경로의 전제다.
- 아군 점선은 `graphics/battle_interface/hns/healthbox_singles_player.png`에 원래 포함된 프레임 장식이며, 퍼센트 코드는 상대 분기에만 실행되므로 옵션과 인과관계가 없다. 아군 숫자 `18/20`도 기존 `PrintHpOnHealthbox()` 경로의 증거다.
- 코드·그래픽 수정은 하지 않았다. 사용자 캡처를 런타임 증거로 기록하고 소스/PNG를 정적으로 대조했다. 로컬에는 mGBA 실행 파일이 없어 추가 재현은 불가했다. 후속은 HNS 확장 자산 방식과 기존 박스 내 클리핑 방식 중 하나를 정해 수정하고 `make hns -j8` 및 mGBA에서 재확인하는 것이다.

### 2026-09-16 — 배틀 설정 상대 HP 퍼센트 표시 토글 (20:34 KST 이후 작업, 현재 롤백됨)

- 요청/범위: 1.17.0에서 가져온 `B_HP_PERCENTAGE_DISPLAY` 상대 HP 퍼센트 렌더링을 배틀 설정에서 ON/OFF할 수 있게 하고, `도망가기 프롬프트` 바로 아래에 `퍼센트 표시`와 지정 설명을 배치했다.
- 구현: `include/global.h`의 `ChallengeSettings` 여유 비트에 `hpPercentageDisplay`를 추가했다. `src/option_menu.c`에서 항목·설명·선택값을 등록하고 저장/로드하며, ON=0/OFF=1인 기존 메뉴 선택 관례와 직접 boolean 저장값을 명시적으로 변환한다. `src/new_game.c`는 `B_HP_PERCENTAGE_DISPLAY`를 새 게임 기본값으로 사용하고, `src/oak_speech_hns.c`는 새 게임 전 옵션 보존 경로에 필드를 포함한다.
- 배틀 반영: `src/battle_interface.c`에 런타임 검사 함수를 추가하고 상대 싱글/더블 HP 갱신 경로에 연결했다. 상대 체력박스 초기 갱신도 퍼센트 표시 ON일 때 실행되며, OFF에서는 기존 막대/HP 숫자 동작을 유지한다. `include/config/battle.h`의 매크로 주석은 인게임 옵션의 기본값임을 명시했다.
- 저장 호환성: 새 필드는 `ChallengeSettings`의 기존 비트 여유분을 사용해 구조 크기를 32바이트로 유지했다. 기존 저장 데이터의 해당 미사용 비트(0)는 직접 boolean 기준 OFF로 해석된다.
- 검증: `make hns -j8` 종료 코드 0, EWRAM 249,012/262,144, IWRAM 25,704/32,768, ROM 33,325,188/33,554,432(99.32%), `pokehns.gba` 33,554,432바이트. 로그 `build/localization-logs/hns-hp-percentage-option-20260916-r3.log`.
- 게임 화면 확인: mGBA가 없어 실제 옵션 메뉴 렌더링, 세이브 재로드, 싱글/더블 상대 체력박스의 퍼센트 가독성·위치는 미확인이다.

### 2026-09-16 — 설원동굴·신도마을 표기 반영 및 재빌드

- 요청/범위: 사용자가 수정한 `MAPSEC_SNOWSWEPT_CAVERN`=`설원동굴`, `MAPSEC_NEW_SINJOH`=`신도마을`을 확인하고 런타임 경로에 반영했다.
- 수정: `src/region_map.c`의 `sRegionMapEntries_Johto`에서 같은 두 이름을 갱신했다. `MAPSEC_SINJOH`=`신도`, `MAPSEC_SINJOH_RUINS`=`신도유적`은 유지했다.
- 검증: `make hns -j8` 종료 코드 0, JSON·C 표·생성 `region_map_entries.h`의 해당 문자열 일치, JSON 파싱·ASCII 검사·`git diff --check` 통과. EWRAM 249,012 bytes, IWRAM 25,704 bytes, ROM 33,324,516/33,554,432 bytes(99.31%); 로그 `build/localization-logs/hns-region-name-sinjoh-ko-20260916-r5.log`.
- 게임 화면 확인: mGBA 실행 파일 부재로 실제 팝업은 미확인.

### 2026-09-16 — New Sinjoh 명칭 근거 조사

- 요청/범위: 인게임 텍스트에 `New Sinjoh`라는 명칭의 유래나 설명이 있는지 조사했다.
- 확인 결과: `NewSinjoh_Text_Welcome_Sign`은 `NEW SINJOH`만 표시하고, NPC 대사는 공동체·관광객·새로 온 사람들과 Sinjoh의 균형을 언급할 뿐 명칭의 유래를 설명하지 않는다. `SinjohRuins_Text_Sign`은 `SINJOH RUINS`를 표시한다.
- 검증: `data/maps/NewSinjoh_hns/scripts.inc`, `data/maps/SinjohRuins_hns/scripts.inc`, Mt. Silver의 Sinjoh 해금 대사와 도움말을 검색했다. 코드·데이터·ROM은 수정하지 않았다.

### 2026-09-16 — Sinjoh 지역 ID 차이 확인

- 요청/범위: `MAPSEC_SINJOH`와 `MAPSEC_NEW_SINJOH`의 실제 맵 역할과 차이를 확인했다.
- 확인 결과: `MAPSEC_SINJOH`는 현재 JSON에만 있고 실제 `map.json`의 `region_map_section`으로 사용되지 않는 일반/예비 지역 슬롯이다. `MAPSEC_NEW_SINJOH`는 `MAP_NEW_SINJOH_HNS`와 부속 맵을 대표하는 설원 마을이며 49·50번 도로 및 신도유적 방면으로 연결된다.
- 검증: `data/maps`의 지역 섹션·연결·워프, `src/map_name_popup.c`의 테마 배열, `src/region_map.c`의 방문/비행 처리와 `rg` 결과를 대조했다. 코드·데이터·ROM은 수정하지 않았다.

### 2026-09-16 — HNS 전용 지역 목록 확인

- 요청/범위: `region_map_sections.json`에서 HNS에만 존재하는 지역을 확인했다.
- 확인 결과: `hns_map_sections` ID와 `map_sections` ID의 차집합은 62개이며, 조토 마을 10개·26~48번 도로 23개·조토/HNS 시설·던전 29개다. 이름 문자열 기준으로는 `신도`가 두 ID에 사용되어 61종이다.
- 주의: `로켓단아지트`와 `챔피언로드`도 HNS 전용 ID가 별도로 있지만 다른 지역 ID에 같은 이름이 있으며, `MAPSEC_SNOWSWEPT_CAVERN`은 현재 JSON `설원동굴`과 `src/region_map.c` `설원 동굴` 사이에 띄어쓰기 차이가 있다.
- 검증: JSON을 파싱하고 두 배열의 ID 차집합을 정적으로 계산했다. 코드·데이터·ROM은 추가 수정하지 않았다.

### 2026-09-16 — `region_map_sections.json` 지역명 전체 한글화

- 요청/범위: 지역명 팝업에 사용되는 `src/data/region_map/region_map_sections.json`의 두 배열을 공식 한국어 지명으로 번역했다.
- 변경: `map_sections` 219개와 `hns_map_sections` 124개에 있는 `name` 342개를 모두 갱신했고, 이름이 없는 `MAPSEC_DYNAMIC`은 유지했다. 호연·관동·칠성제도는 기존 프로젝트 한국어 지역명, 조토는 한국어 골드 지역명, 알로라는 공식 섬·지역 명칭을 적용했다. 하트골드·소울실버의 공식 `신도`·`신도유적`·`낭떠러지동굴`·`빛남의 등대` 표기도 반영했으며, 설원 동굴·매몰탑·49/50번 도로 등 HNS 고유 지역은 공식 대응명이 없어 자연스러운 한글 표기로 정했다.
- 런타임 대응: `FLAG_VISITED_KANTO`가 켜졌을 때 선택되는 `src/region_map.c:157-277`의 `sRegionMapEntries_Johto` 119개 문자열도 JSON과 같은 한글명으로 변경했다. 좌표·크기·ID·제어 토큰은 변경하지 않았다.
- 생성/검증: `make hns -j8` 최종 재빌드로 `src/data/region_map/region_map_entries.h`를 재생성하고 링크까지 성공했다. EWRAM 249,012 bytes, IWRAM 25,704 bytes, ROM 33,324,516/33,554,432 bytes(99.31%); 최종 로그 `build/localization-logs/hns-map-names-ko-20260916-r3.log`. JSON 파싱, 허용된 `{AQUA}` 토큰을 제외한 ASCII 잔존 확인, `git diff --check`도 통과했다.
- 미검증: mGBA 실행 파일 부재로 실제 맵 팝업 화면의 중앙 정렬·긴 명칭 잘림과 조토/관동 플래그 전환은 정적으로만 확인했다.

### 2026-09-16 — 오버월드 마을·도로명 팝업 번역 경로 조사

- 요청/범위: 첨부 화면처럼 마을·도로 진입 시 좌측 상단에 나타나는 지역명 창의 번역 방법 확인.
- 수정 파일: `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`만 갱신했다. 코드·맵 데이터·그래픽은 수정하지 않았다.
- 확인 결과: `src/map_name_popup.c:556-775`의 `ShowMapNamePopup()`/`ShowMapNamePopUpWindow()`가 `GetMapName()` 결과를 `FONT_NARROW`로 출력한다. 일반 HNS 지역명은 `src/data/region_map/region_map_sections.json`의 `hns_map_sections`에서 자동 생성되는 `src/data/region_map/region_map_entries.h`에 들어가며, `src/region_map.c:157-277`의 `sRegionMapEntries_Johto`가 별도 런타임 테이블로 사용된다. 두 테이블의 `name` 문자열을 모두 맞춰야 진행도 플래그에 따라 영어로 되돌아가지 않는다.
- 표시 조건/제약: 맵 헤더의 `show_map_name=TRUE`, `FLAG_HIDE_MAP_NAME_POPUP`, 같은 Map Section 재진입 억제에 따라 팝업이 나타난다. GEN_3 창은 `src/menu.c:424-433`의 80×24픽셀 영역이며 중앙 정렬되므로 긴 한글 지명은 폭을 확인해야 한다. 프레임·배경 `.4bpp`는 장식 그래픽이라 문자열 번역에는 건드리지 않는다.
- 특수 경로: 특수기지는 `src/secret_base.c:734-737`, 배틀 피라미드 층 표기는 `src/map_name_popup.c:508-527`의 별도 문자열을 사용한다.
- 검증: `rg`와 소스 줄 번호로 호출·테이블·생성 규칙을 대조했다. 코드/데이터 변경이 없어 빌드와 mGBA 런타임 검증은 수행하지 않았다.
- 남은 문제/다음 시작점: 공식 한국어 지명을 확정하고 JSON 및 `sRegionMapEntries_Johto`의 `name`만 번역한 뒤 `make hns -j8`; 생성된 ROM에서 Violet City 등 마을과 Route 워프의 팝업 정렬·잘림을 확인한다.

### 2026-09-16 — L=A 기술 설명 R 전환 구현 및 스프라이트 재검증

- 요청/범위: 옵션의 `L=A` 모드에서 L 대신 R로 기술 설명 창을 열고 R용 안내 스프라이트를 사용하도록 구현. 사용자가 수정한 L/R PNG를 재검사하고 `.4bpp` 변환·HNS 빌드를 수행했다.
- 수정 파일: `include/battle_interface.h`, `include/config/battle.h`, `src/battle_interface.c`, `src/battle_controller_player.c`, 사용자가 수정한 `graphics/battle_interface/move_info_window_l.png`·`move_info_window_r.png`는 유지했다.
- 구현: `GetBattleMoveDescriptionButton()`이 기본 L 설정에서만 L=A 옵션을 검사해 R을 반환한다. 입력 처리의 설명 창 열기/닫기와 `TryToAddMoveInfoWindow()`의 이미지 선택이 이 유효 버튼을 공유한다. L 설정 빌드에는 L/R 그래픽을 모두 포함하며 START 기믹 선택 분기는 변경하지 않았다.
- 그래픽 확인: L/R PNG 모두 32×32, 8-bit colormap 형식이고 `L/R / 기술 / 정보` 픽셀과 테두리가 유지된다. `tools/gbagfx/gbagfx` 재변환 결과를 추적 산출물과 `cmp`해 양쪽 모두 일치했다.
- 검증: `make hns -j8` 종료 코드 0. EWRAM 249,012/262,144, IWRAM 25,704/32,768, ROM 33,324,868/33,554,432(99.32%), ROM 32MiB. 로그 `build/localization-logs/hns-move-info-l_equals_a-20260916.log`; `git diff --check` 통과.
- 게임 화면 확인: mGBA 실행 파일이 없어 일반 모드 L 및 L=A 모드 R의 실제 입력/팝업은 미확인.
- 남은 문제/다음 시작점: mGBA에서 두 옵션 모드를 각각 테스트하고, L=A 모드에서 R 기술 설명과 마지막 사용 몬스터볼 기능의 입력 충돌이 없는지 확인한다.

### 2026-09-16 — START용 move info 스프라이트 사용 조건 확인

- 확인: `src/battle_interface.c`의 전처리 분기에서 `B_MOVE_DESCRIPTION_BUTTON`이 R/L이 아닐 때 `move_info_window_start.4bpp`가 선택된다. 현재 L 설정에서는 해당 파일이 ROM에 포함되지 않는다.
- 입력 충돌: START 설정 시 `HandleInputChooseMove`의 기술 설명 분기(`src/battle_controller_player.c:907-911`)가 뒤의 START 기믹 선택 분기(`:913-921`)보다 먼저 실행된다. 따라서 START를 기술 설명과 메가진화/Z 기술 선택에 동시에 배정할 수 없다.
- 수정/검증: 설명 작업만 수행했으며 코드·그래픽·ROM은 변경하지 않았다. 결과는 STATUS.md에 기록했다.

### 2026-09-16 — 기술 정보 팝업 스프라이트 번역 검수

- 요청/범위: 사용자가 번역한 배틀 기술 선택용 move info 스프라이트를 검수하고 파생 그래픽·HNS 빌드를 확인했다.
- 확인 결과: `move_info_window_l.png`는 32×32 8-bit colormap이며 기존 16색 PLTE가 유지됐다. 시각적으로 `L / 기술 / 정보` 픽셀이 들어가 있고 테두리 영역은 보존됐다. `move_info_window_start.png`는 영문 내용이 그대로이며 Git상 실행 권한 메타데이터만 달라졌다.
- 수정 파일: 사용자가 수정한 `graphics/battle_interface/move_info_window_l.png`는 유지했다. 해당 PNG에서 `graphics/battle_interface/move_info_window_l.4bpp`를 `tools/gbagfx/gbagfx`로 재생성했다(생성 파일은 Git 비추적). 문서도 갱신했다.
- 검증: `make hns -j8` 종료 코드 0. 로그 `build/localization-logs/hns-move-info-sprite-20260916.log`; EWRAM 249,012/262,144, IWRAM 25,704/32,768, ROM 33,324,292/33,554,432, ROM 파일 32MiB. `git diff --check`는 문서 대상에서 통과했다.
- 게임 화면 확인: mGBA에서의 실제 팝업 가독성·슬라이드·겹침은 아직 미확인.
- 다음 시작점: `pokehns.gba`를 mGBA에서 실행해 L 버튼 팝업을 확인하고, START 설정을 사용할 경우 `move_info_window_start.png`를 별도로 번역한다.

### 2026-09-16 — move info 버튼 이미지 선택 조건 보충

- 확인: `move_info_window_r/l/start`는 런타임 상황별 이미지가 아니라 `src/battle_interface.c`의 `#if`에 의해 `B_MOVE_DESCRIPTION_BUTTON` 설정에 맞는 하나만 컴파일된다. 현재 `L_BUTTON`이므로 `move_info_window_l.4bpp`만 사용된다.
- 현재 배치: R은 `B_LAST_USED_BALL_BUTTON`으로 마지막 사용 몬스터볼 기능에 사용되므로 R용 move info 이미지는 출력되지 않는다. R용을 활성화하려면 설정 변경·버튼 충돌 해소·파생 파일 재생성·HNS 재빌드가 필요하다. START용도 같은 방식이다.
- 수정/검증: 문서만 보충했으며 코드·그래픽과 ROM은 변경하지 않았다.

### 2026-09-16 — move info 픽셀 편집기·인덱스 팔레트 절차 보충

- 요청/범위: 사용할 픽셀 편집기, 픽셀 글리프의 의미와 제작법, 기존 팔레트·인덱스 컬러를 보존하는 방법을 구체화했다.
- 확인: `move_info_window_l.png`는 32×32 PNG color type 3, 8-bit colormap이며 PLTE 청크 길이 0x30으로 16색이다. GBA 4bpp 변환은 이 인덱스 순서를 사용하므로 RGB 변환·팔레트 재계산·안티앨리어스가 금지된다.
- 권장 절차: Aseprite를 우선 추천하고 mtPaint를 무료 대안으로 제시했다. GIMP 사용 시에도 Indexed 모드를 유지하고 Pencil 1px과 기존 팔레트 슬롯만 사용한다. 글리프는 `graphics/fonts/font0_korean.png`의 8×8 글리프를 참고한 5×7/6×7 단색 픽셀 비트맵으로 설명했다.
- 수정/검증: 문서만 변경했다. GUI 편집기는 현재 WSL에 설치되어 있지 않아 실행하지 않았으며, 그래픽·코드·ROM은 변경하지 않았다.

### 2026-09-16 — 배틀 기술 정보 팝업 번역 경로 조사

- 요청/범위: 배틀 기술 선택 화면 왼쪽에 나타나는 move info 팝업의 문자를 번역하는 방법 확인. 이번에는 표시 경로와 안전한 수정·검증 절차를 정리하고 실제 그래픽 디자인은 변경하지 않았다.
- 수정 파일: `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`.
- 확인 결과와 결정 이유: 표시는 런타임 문자열이 아니라 `graphics/battle_interface/move_info_window_l.png`에 픽셀로 포함되어 있다. PNG는 32×32 8-bit colormap이고, `src/battle_interface.c`가 `B_MOVE_DESCRIPTION_BUTTON == L_BUTTON`일 때 대응 `.4bpp`를 포함한다. 따라서 `src/battle_message.c`를 번역하는 방식이 아니라 PNG의 `MOVE / INFO` 영역을 `기술 / 정보`용 2×2글자 픽셀 글리프로 다시 그려야 한다. 테두리·`L` 버튼·기존 팔레트 인덱스는 보존한다.
- 검증: `tools/gbagfx/gbagfx graphics/battle_interface/move_info_window_l.png /tmp/move_info_window_l.4bpp` 실행 후 현재 `graphics/battle_interface/move_info_window_l.4bpp`와 `cmp` 결과 일치(종료 코드 0). 문서 외 파일을 수정하지 않았으므로 HNS ROM 재빌드는 하지 않았다.
- 게임 화면 확인: 미수행. 현재 영문 팝업의 L 버튼 표시 경로만 정적으로 확인했다.
- 남은 문제: 32×32 안에서 `기술 / 정보`의 가독성을 확보할 실제 픽셀 디자인을 확정해야 한다. 나중에 버튼을 R/START로 바꾸면 그 버튼용 PNG도 별도로 번역해야 한다.
- 다음 시작점: `graphics/battle_interface/move_info_window_l.png`를 픽셀 단위로 편집한 뒤 같은 이름의 `.4bpp`를 재생성하고 `make hns -j8`; mGBA 기술 선택 화면에서 L 버튼으로 열고 글자·테두리·슬라이드 위치를 확인한다.

### 2026-09-16 — 공식 HNS Release-v2.0.5 누락 복구 및 제목 화면 회귀 수정

- 요청/범위: 어제 11:42 전 HNS 상태와 공식 2.0.5를 대조해, 1.17.0·한글 패치·메가진화·Soulgold 변경을 보존하면서 누락된 2.0.5 코드를 복구했다.
- 기준/출처: 1.17.0 이식 질문 직전 체크포인트 `6da0a16d66d3d6a2691c775cc23780f994512ab5`, 로컬 `Release-v2.0.4`(`98574d2`), `Release-v2.0.5`(`1f42b74dff`), 공식 [릴리스 페이지](https://github.com/PokemonHnS-Development/pokehns-expansion/releases/tag/Release-v2.0.5).
- 대조 결과: 2.0.4→2.0.5 누적 변경은 282개 경로다. 그중 2.0.4 바이트로 남아 있던 171개를 공식 내용으로 복구하고, 29개는 현재 HNS와 3-way 병합, 16개 충돌은 수동 검토했으며, 이미 일치하던 66개는 유지했다. 최종 바이트 분류는 공식 2.0.5 일치 241개, HNS/한글/1.17.0/Mega 보존에 따른 의도적 차이 41개, 2.0.4 잔류·누락 0개다.
- 제목 원인/수정: `graphics/title_screen/hns/press_start.png`가 2.0.4(`1fa29…`, `START`)였고 공식 2.0.5(`dd9231fd162a9e1ef6493e4ebe82f551520da052`, `PRESS START`)와 달랐다. 공식 파일로 복구해 사용자가 본 2.0.4 제목 회귀를 해소했다.
- 주요 반영: 공식 HNS 맵/레이아웃/타일·신조/알로라·사파리·리매치 스크립트, 배틀 일반/AI/UI 버그 수정, 도감·진화·학습 목록을 반영했다. `src/daycare.c`는 현재 1.17.0 알 생성 구조를 기준으로 Mirror Herb·베이비 폼 알기술 전달, 초기 알 기술/이로치 롤, 너즐록 부화 차단을 병합했다. `src/data/pokemon/all_learnables.json`은 기존 HNS 목록을 유지하면서 공식 추가 기술만 합쳤다.
- 보호한 사용자 변경: `src/battle_message.c`, `src/strings.c`, `src/oak_speech_hns.c`, `src/challenge_menu.c`의 한글 문구와 기존 잠금 정책, `B_MOVE_DESCRIPTION_BUTTON L_BUTTON`, 메가진화 TRUE·고음 피치 울음소리, Soulgold/Gold/Oak 그래픽은 변경하지 않았다. `src/nuzlocke.c`의 중복 `IsNuzlockeCaptureSuspended`는 기존 HNS 정의 하나만 남겼고, 공식 map JSON 반영 후 생성된 `ViridianForest_hns/events.inc`를 갱신했다.
- 검증: `make hns -j8` 첫 실행은 중복 너즐록 함수와 오래된 Viridian Forest 생성 이벤트 때문에 링크가 실패했다. 중복 제거·맵 이벤트 재생성 후, 공식 2.0.5에서 바뀐 제목/서핑 드래고나이트/지도 PNG의 파생 `.4bpp`·`.smol`·`.gbapal`·맵 타일도 강제로 재생성했다. 최종 `make hns -j8` 종료 코드 0, 로그 `build/localization-logs/hns-205-repair-20260916-r4.log`; EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,292/33,554,432(99.31%), ROM 파일 32MiB.
- 미검증/다음 시작점: 실제 mGBA에서 제목의 `PRESS START`, 오박사/Gold/Soulgold 그래픽, 한글 조사·메뉴, 사파리·알 부화·리매치와 배틀 UI를 확인한다. 공식 비교 결과와 빌드 근거는 `docs/localization/evidence/2026-09-16-hns-205-repair.txt`에 남긴다.

### 2026-09-16 — 1.17.0 이식 직전 한글화·HNS 회귀 2차 복구

- 요청/범위: 1.17.0 이식 직전(체크포인트 `6da0a16d66d3d6a2691c775cc23780f994512ab5`)과 현재 트리를 비교해 오박사 팔레트, 조사 토큰, 메가진화 활성화, 챌린지 추천 좌표 및 소실된 한글 텍스트를 복구했다.
- 수정 파일: `include/config/species_enabled.h`, `include/constants/global.h`, `src/string_util.c`, `src/challenge_menu.c`, `include/battle_message.h`, `src/data/items.h`, `src/strings.c`, `include/strings.h`. 오박사 원본·산출물은 `graphics/oak_speech/{oak/pal.pal,oak/pic.png,oak_speech_bg.bin,oak_speech_bg.png}`와 `graphics/oak_speech_hns/oak/{pal.pal,pic.png}`를 기준 바이트로 복원했다.
- 변경과 결정 이유: 메가진화/원시회귀 플래그가 `FALSE`로 덮인 것을 `TRUE`로 복구했다. 별도 메가 울음소리(`P_MODIFIED_MEGA_CRIES`)는 사용하지 않고 `CRY_MODE_HIGH_PITCH`를 유지해 32MiB를 보존했다. `src/string_util.c`에서 삭제된 `gJongCode`, `GetJongCode`, 조사 함수·플레이스홀더 테이블을 기준 코드 그대로 되살려 `{PLAYER}{K_I}`가 `심향이`로 확장되게 했다. `DrawChoices_Two`는 임의 74px 분기를 제거하고 104px 기준으로 맞췄다. 기존 한글 아이템 복수형 183개, 도감 자모 7개, 미사용 `카운트`를 복구했다. 1.17.0의 혼란 열매 hold-effect 통합에 맞춰 5개 열매만 `HOLD_EFFECT_CONFUSE_FLAVOR`/`secondaryId`로 연결했다. 도감 울음소리 표기는 `gText_CryOf1`(`의`)과 `gText_CryOf2`(`울음소리`)로 분리해 종명 뒤 조사와 제목의 의미를 보존했다.
- 검증: `make hns -j8` 종료 코드 0. 최종 링크 메모리 EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,322,644/33,554,432(99.31%); `pokehns.gba` 33,554,432바이트(32MiB). 로그 `build/localization-logs/hns-regression-repair-20260916.log`.
- 대조 결과: `src/battle_message.c`, `src/data/types_info.h`, `src/oak_speech_hns.c`, `data/text/oak_speech_hns.inc`, Gold 남자 주인공 스프라이트는 기준점의 한글/HNS 내용을 유지하고 있었다. 현재 타이틀 `press_start.png` 변경은 1.17.0과 무관한 HNS 2.0.5 변경으로 유지했다.
- 알려진 문제/다음 시작점: 기존 미사용 코드 경고와 조사 테이블의 겹치는 `0x0E` 지정 초기화 경고는 남아 있으나 빌드는 성공했다. 실제 mGBA에서 오박사 색상·`심향이로구나?`·챌린지 정렬·메가진화 동작을 확인해야 한다. 원인은 넓은 임시 병합으로 HNS/한글 파일이 upstream으로 덮인 뒤 복구 패치 방향까지 뒤집혀 일부 번역이 재손실된 것이다.

### 2026-09-16 — 정정: 1.17.0 직전 HNS 변경 회귀 복구

- 사용자는 1.17.0 이식 직전의 남주인공 스프라이트(왼쪽 화면)와 현재 이식 후 스프라이트(오른쪽 화면)가 다르고, 기술 설명 키/표시 이미지의 `L` 설정과 타입 번역도 영어로 돌아갔다고 지적했다. 이전 감사는 Soulgold 원본과 현재 파일만 비교해 이 회귀를 놓쳤으며, “동일하다/복구 완료”라고 보고한 것은 잘못이었다.
- 1.17.0 작업 직전 체크포인트를 실제 기준으로 삼아 `graphics/trainers/front_pics/gold_hns.png`·`back_pics/gold_hns.png`를 이전 HNS 파일로 복구했다. 현재 전면/후면 PNG는 각각 `caa547164c53d2a5f7e40529483ccfa40a233c5c`·`e0c87fbc5417588f68fd81191ad2976cc58d35f4`와 일치한다. 병합본으로 바뀐 전면·후면 팔레트(`graphics/trainers/palettes/gold_hns.pal`, `graphics/trainers/back_pics/gold_hns.pal`)도 복구해 `.gbapal` 산출물을 다시 생성했다.
- 동일 대조에서 확인된 번역·코드 회귀를 원문 그대로 복구했다: `include/config/battle.h`의 `B_MOVE_DESCRIPTION_BUTTON L_BUTTON`, `src/data/types_info.h` 타입명, `src/data/items.h` 도구명, Gen 8 종명·분류명, `src/data/script_menu.h`, `data/text/oak_speech_hns.inc`, `src/oak_speech_hns.c`, `src/strings.c` 포켓/메뉴 명칭, `src/trainer_card.c` HNS 카드 분기. 임의의 새 번역은 만들지 않았다.
- 후속 전체 대조에서 HNS 전용 코드·데이터도 추가로 복구했다. `src/battle_setup.c`, `src/trainer_hill.c`, `src/safari_zone.c`, `src/starter_choose.c`, `src/event_object_movement.c`, `src/sound.c`, `src/save.c`, `src/credits_hns.c`, `src/data/trainers_hns.party`, HNS 상수·도움말·그래픽 헤더, HNS 사파리/신조/비리디안 숲/실버산/버밀리온/루트28 맵 이벤트를 1.17.0 직전 내용으로 맞췄다. 팬페어·도움말 ID와 HNS 이벤트 라벨이 다시 링크되도록 확인했다.
- `src/battle_message.c`는 VS Code 기록 `jHTf.c`와 전체가 일치했고 `src/challenge_menu.c`도 번역 회귀가 없어 보존했다. 앞선 Soulgold 배틀/UI/목호 자산 복구와 1.17.0 기능 코드는 유지했다.
- 팔레트·HNS 전용 코드 복구 후 최종 검증: `make hns -j8` 종료 코드 0, 로그 `build/localization-logs/hns-translation-regression-restore-20260916-r6.log`; EWRAM 248,960/262,144, IWRAM 25,704/32,768, ROM 32,914,532/33,554,432(98.09%), `pokehns.gba` 32MiB. 링크 오류는 없었다. 첨부 mGBA 화면의 왼쪽 스프라이트와 현재 PNG/팔레트를 대조했으며, 실제 에뮬레이터 전체 화면 검증은 남아 있다.

### 2026-09-16 — 1.17.0 이식 후 HNS/Soulgold 그래픽 보존 점검

- 요청: 1.17.0 이식 과정에서 Soulgold에서 가져온 배틀 텍스트 창·텍스트 창·남자 주인공 전체 스프라이트·목호 스프라이트가 기본 HNS로 돌아갔는지 확인하고 원래 변경본으로 복구.
- 비교 결과: Soulgold 원본과 직접 대조해 기본 HNS로 돌아가 있던 `graphics/battle_interface/hns/textbox.*`, `graphics/text_window/1..20`·`message_box`·`name_box`, `graphics/trainers/front_pics/champion_lance_hns.*`를 찾아 원본 및 4bpp/SMOL/GBA 산출물로 복원했다. Gold HNS 전면·후면·필드 스프라이트는 Soulgold에 동일 슬롯이 없어 현재 HNS 자산을 보존했다. HNS 그래픽 선택 코드와 한글 텍스트 코드는 수정하지 않았다.
- 복원 검증: 배틀 텍스트 창 10개, 텍스트 창 1~20번과 공통 상자, 목호 PNG/4bpp/SMOL/GBA/JASC 팔레트가 `../soulgold`와 바이트 단위로 일치했다. 일부 `100755` 표시는 `/mnt/c` Windows 파일시스템 권한 메타데이터이며 ROM 내용에는 영향을 주지 않는다. `graphics/title_screen/hns/press_start.*`만 HNS 2.0.5 커밋 `1f42b74dff` 변경으로 확인되어 되돌리지 않았다.
- 추가 감사(이전 결과, 위 정정으로 대체): Gold 전면·후면 PNG의 팔레트 회귀를 놓쳤으므로 당시 “추가 자산 회귀 없음”이라는 결론은 폐기한다. 위 정정 항목에서 Gold PNG·팔레트와 HNS 코드·이벤트를 기준 체크포인트로 다시 복구했다. `move_info_window_start.png`와 `src/text_window.c`의 남은 diff는 파일 내용이 아닌 `/mnt/c` 권한 표시다. HNS 선택기(`src/graphics.c`, `include/constants/trainers.h`, `src/data/graphics/trainers.h`) 연결은 그대로 유지했다.
- 원인 기록: 1.17.0 범위 병합에서 `graphics/`와 생성 파일까지 후보로 취급해 HNS 전용 배틀/UI·목호 자산이 기본본으로 덮일 수 있었다. 이번에는 Soulgold 원본이 확인되는 파일만 명시적으로 복원하고 Gold HNS 전용·한글 관련 자산은 건드리지 않았다. 이미 반영된 1.17.0 상성 아이콘·상대 HP 기능 코드도 유지했다.
- 검증: `make hns -j8` 성공, 로그 `build/localization-logs/hns-soulgold-graphics-restore-20260916.log`. EWRAM 247,472/262,144(94.40%), IWRAM 25,704/32,768(78.44%), ROM 32,885,796/33,554,432(98.01%), `pokehns.gba` 32MiB. 실제 게임 화면 검증은 남아 있다.

### 2026-09-16 — 1.15.1 이후 6개 릴리스 범위 점검 및 아이템 호환성 정리

- 요청/범위: 공식 릴리스의 1.15.1 이후 Pokémon·Battle General·Moves·Abilities·Items·Battle AI 변경을 HNS에 반영하되 기존 한글 패치와 32MiB/고음 피치 메가 울음소리를 보존.
- 확인·결정: 1.15.2→1.17.0 전체 차이는 구조 개편 충돌 263개와 상호 의존 API가 있어 파일 일괄 병합을 적용하지 않았다. 임시 병합 결과는 롤백했고, 기존에 선별 완료된 1.17.0 기능은 유지했다. 이번 기록은 전체 범위 완료가 아니라 남은 PR별 이식이 필요한 상태를 명시한다.
- 수정 파일: `include/constants/hold_effects.h`, `src/data/hold_effects.h`, `src/data/items.h`와 이미 반영된 hold effect API에 맞춰 `src/battle_hold_effects.c`, `src/battle_debug.c`, `src/battle_dynamax.c`의 호출·열거값을 정합화했다. 혼란 열매는 `HOLD_EFFECT_CONFUSE_FLAVOR`와 `GetItemSecondaryId`를 사용하고, 다이맥스 경로는 능력·도구 컨텍스트를 전달한다. 한글 텍스트 파일과 에셋은 수정하지 않았다.
- 검증: `make hns -j8` 종료 코드 0. 링크 메모리 EWRAM 247,472 bytes, IWRAM 25,704 bytes, ROM 32,885,700/33,554,432 bytes. 로그 `build/localization-logs/hns-expansion-sync-20260916-finalcheck2.log`.
- 사운드/용량: `P_MODIFIED_MEGA_CRIES FALSE`, `CRY_MODE_HIGH_PITCH`, ROM 32MiB 유지. 메가진화 울음소리 파일은 추가하지 않았다.
- 게임 화면 확인: 미수행. 혼란 열매 발동·디버그 hold effect 표시·다이맥스 기술 타입을 실제 전투에서 확인해야 한다.
- 남은 문제/다음 시작점: 남은 릴리스 PR을 Pokémon→Battle General→Moves/Abilities→Items→Battle AI 의존성 순서로 수동 이식한다. `src/battle_message.c`, `src/strings.c`, HNS 전용 텍스트/에셋은 변경하지 않는다.

### 2026-09-16 — 1.17.0 PR #10121 디버그 메뉴 이식

- 공식 `expansion/1.17.0` PR #10121 커밋 `9df1444765521683907f19757155d3d2d3668ae0`을 현재 HNS의 분기된 `src/debug.c`에 기능 단위로 반영했다. upstream `src/debug.c` 전체 교체는 기존 HNS 디버그 기능과 충돌하므로 수행하지 않았다.
- 복합 포켓몬 주기 메뉴에 성별 값 선택을 추가하고, 선택한 성별을 `GetMonPersonality`에 전달했다. 종·레벨·성별·이로치·성격·특성·테라 타입·다이맥스·거다이맥스·IV·EV·기술 단계의 B 버튼 되돌리기를 연결했다.
- 워프, 아이템/수량, 변수/값, 포커러스 strain/기간의 다단계 선택도 중간 취소 시 직전 단계로 돌아가도록 수정했다. `src/pokemon_icon.c`에는 PR의 `usingSheet` 아이콘 애니메이션 분기를 추가했다.
- upstream의 `ResolveEVs` 공개화는 현재 HNS에 해당 함수가 없고 기존 복합 포켓몬 메뉴의 EV 검증 경로를 유지해야 하므로, 선언만 추가하지 않았다.
- 한글 번역 파일은 수정하지 않았다. `cmp`로 `src/battle_message.c`와 VS Code 기록 `jHTf.c`가 계속 동일함을 확인했다.
- `make hns -j8` 성공: EWRAM 247,472(94.40%), IWRAM 25,704(78.44%), ROM 32,885,700/33,554,432(98.01%). 로그 `build/localization-logs/hns-debug-10121-20260916.log`. 소스 대상 `git diff --check` 통과.
- 실제 게임에서 모든 디버그 단계의 B 버튼과 성별 생성 결과를 확인하지 않았다. 32MiB ROM과 메가 울음소리 고음 피치 기본 설정은 유지한다.

### 2026-09-16 — 64MiB 확장 철회 및 메가 울음소리 기본 모드 복구

- 1.15.2 공통 조상부터 1.17.0 태그까지 전체 병합을 점검했으나 구조 개편 충돌 173개가 있어 일괄 적용하지 않았다. 임시 신규 파일은 제거했고, 기존 선별 이식분과 한글 파일은 유지했다.
- 사용자의 32MiB 유지 요청에 따라 `P_MODIFIED_MEGA_CRIES FALSE`와 `CRY_MODE_HIGH_PITCH`를 복구했다. 1.17.0 고음질 메가 WAV 26개와 Porygon/PCM 변경을 작업 전 상태로 되돌렸으며, 64MiB 링크 설정도 32MiB로 복구했다.
- `make hns -j8` 성공: EWRAM 247,472(94.40%), IWRAM 25,704(78.44%), ROM 32,883,988/33,554,432(98.00%). 로그 `build/localization-logs/hns-revert-mega-cries-20260916.log`.
- `src/battle_message.c`는 VS Code 기록 `jHTf.c`와 일치한다. 챌린지 메뉴·문자열·HNS 한글 텍스트는 철회 과정에서 변경하지 않았다.

### 2026-09-15 — 1.17.0 이식 중 한글 문자열 롤백 복구

- 요청/범위: 1.17.0 업데이트 뒤 영어로 되돌아간 기존 한글패치 문자열을 기능 코드와 분리해 복구. 우선 사용자가 확인한 `src/challenge_menu.c`, `src/battle_message.c`와 백업 대조에서 발견된 관련 범위를 처리했다.
- 기준 백업: `build/localization-backups/pre-hns-205-20260914-112129.tar.gz`. 백업과 현재의 한글 문자 수를 소스별로 대조한 결과 소실이 확인된 파일은 `src/challenge_menu.c`와 `src/battle_message.c`였다. `src/oak_speech_hns.c`의 구형 한글 주석·시간대 코드 차이는 최신 HNS 흐름을 되돌릴 수 있어 복구하지 않았고, 실제 대사는 `data/text/oak_speech_hns.inc`에 한글로 보존되어 있음을 확인했다.
- 복구: `challenge_menu.c`의 모드·기능·랜더마이저·너즐록·난이도·챌린지 탭 이름, 설명, 선택지, 상단 버튼 문구를 복원했다. 최신 `TAB_MODE/ITEM_MODE_GAMEMODE` 선택지 좌표 보정은 다시 적용했다. `battle_message.c`의 상단 배틀 문구, 상태·도구·기술 메시지, 배틀 프런티어·심판·배틀 UI 문자열을 백업의 한글 문장과 조사/제어 토큰 그대로 복원했다.
- 추가 복구: 사용자가 지적한 `src/battle_message.c:566-594`는 VS Code 서버 로컬 기록 `/home/tk_pc/.vscode-server/data/User/History/ed9b1d2/jHTf.c`에서 실제 사용자 원문을 찾아 그대로 대입했다. 이전 tar 백업에는 이 범위가 영어였으며, 로컬 기록의 한글 문장·조사/제어 토큰·줄바꿈이 현재 파일과 일치한다. 이후 미번역 구간은 `:595-889`로 좁혀졌다.
- 전체 파일 대조: `diff -u /home/tk_pc/.vscode-server/data/User/History/ed9b1d2/jHTf.c src/battle_message.c` 결과가 빈 출력으로, 현재 `battle_message.c` 전체가 사용자 로컬 기록과 일치함을 확인했다. 메가진화 문구, 링크 상대 이름 처리, 조사 처리 코드, 배틀 메뉴 좌표도 기록 원문대로 복원했다.
- `src/challenge_menu.c`는 VS Code 로컬 기록 `t2fy.c`(2026-09-11 22:21 KST)와 대조했다. 사용자 한글 문자열은 모두 일치했고, 현재 파일에만 있는 차이는 최신 게임 모드 선택지 좌표를 위한 `DrawChoices_Two`의 `leftX` 계산 한 곳이다.
- 원인 분석: 이전 세션은 `diff -u src/battle_message.c <(tar -xOzf build/localization-backups/pre-hns-205-20260914-112129.tar.gz src/battle_message.c)` 결과를 그대로 패치에 넣었다. 이 명령의 방향은 현재 파일을 백업 파일로 바꾸는 방향이었고, 2026-09-15 20:09 KST에 사용자 번역이 포함된 현재 566–594번을 백업의 영어 문장으로 덮어썼다. VS Code 기록의 16:11 KST 원문과 작업 로그를 대조해 확인했다.
- 번역 보류 위치: `src/battle_message.c:97-98` FRLG 전용 GHOST, `:124` 미사용 교체 문구, `:191` 미사용 지형 문구, `:595-889` 미번역 `gBattleStringsTable` 구간, `:1427` HNS 미사용 FRLG 사파리 메뉴, `:1478` Battle Frontier `vs`. `src/challenge_menu.c:337-343,514,524,544-626,1512`의 `OFF`·`ON`·수치·배율도 이번에는 기능값으로 유지했다.
- 문자열·토큰 복구: VS Code 기록 `oqPX.c`와 `src/strings.c`를 공통 심볼 기준으로 대조해 1,201개 리터럴을 사용자 한글 값으로 복원했다. 조사 플레이스홀더 15개 정의와 `include/strings.h` 선언을 복구했으며, 기록에만 있는 구형/미사용 심볼 10개(`gText_Count`, `gText_CryOf1/2`, `gText_DexSearchAlpha1–7`)는 현재 expansion 구조 때문에 추가하지 않았다. 현재 구조의 `gText_EggNickname`·`gText_Pokemon`은 배열 길이 선언만 달라 이미 한글 값이 유지되어 있다. `include/battle_message.h`의 `FD35–FD3B` 조사 토큰과 이후 전투 토큰도 `charmap.txt`와 일치하도록 정렬했다.
- 정확한 원문 대입 후 재검증: VS Code 로컬 기록의 566–594번 `STRINGID` 29개와 `battle_message.c` 전체를 대조해 모두 일치함을 확인했다. `src/strings.c`는 앞선 대조에서 누락된 공통 리터럴 109개도 `oqPX.c`의 실제 값으로 추가 복원했고, `#if IS_HNS`·`#if OW_POISON_DAMAGE` 분기의 네 리터럴도 기록과 일치시켰다. 최종 occurrence 대조에서 공통 심볼의 모든 문자열 값에 차이가 없었다. 이후 `git diff --check -- src/battle_message.c include/battle_message.h include/strings.h src/strings.c`와 `timeout 360s make hns -j8`이 성공했다(로그 `build/localization-logs/hns-restore-user-strings-20260915.log`). 최종 링크는 EWRAM 247,472 bytes(94.40%), IWRAM 25,704 bytes(78.44%), ROM 32,884,068 bytes(98.00%)이다.
- 게임 검증: mGBA 화면 확인은 수행하지 않음. 다음 시작점은 챌린지 메뉴 탭/설명 폭, 배틀 야생 출현·교체·프런티어 심판 문구를 실제 화면에서 확인하고, 위 보류 구간을 순차적으로 한글화하는 것이다.

### 2026-09-13 — Soulgold 목호 배틀 전면 그림 이식

- 요청/범위: 오버월드 그림은 유지하고 Soulgold의 목호(Lance) 트레이너 배틀 스프라이트를 HNS에 반영.
- 수정 파일: `graphics/trainers/front_pics/champion_lance_hns.png`, `graphics/trainers/palettes/champion_lance_hns.pal`; 변환 산출물 `champion_lance_hns.4bpp`, `champion_lance_hns.4bpp.smol`, `champion_lance_hns.gbapal`도 재생성. `src/data/graphics/trainers.h`의 기존 `TRAINER_PIC_FRONT_CHAMPION_LANCE_HNS` 연결은 그대로 사용.
- 변경과 결정 이유: HNS에는 이미 `TRAINER_PIC_FRONT_CHAMPION_LANCE_HNS`와 목호 오버월드 자산이 있어 새 슬롯을 만들지 않았다. Soulgold의 `../soulgold/graphics/trainers/front_pics/champion_lance.png`·`champion_lance.gbapal`을 HNS 전용 파일명으로 복사·변환했다. 오버월드 `OBJ_EVENT_GFX_LANCE_HNS` 자산은 건드리지 않았다.
- 검증: HNS PNG·4bpp·SMOL·GBA 팔레트와 Soulgold 원본의 `cmp`가 모두 일치했고, 관련 `git diff --check` 통과. `make hns -j8`은 `src/battle_message.c:1421`의 사용자 작업 중 오류 `B_ACTIVE_NAME_WITH_PREFIX` 미정의 및 구문 오류로 종료 코드 2. 로그 `build/localization-logs/hns-trainer-lance.log`; ROM 갱신과 실제 게임 화면 확인은 하지 못함.
- 남은 문제: `battle_message.c` 오류를 해결한 뒤 빌드를 재실행해야 하며, 게임에서 목호 배틀 전면 그림의 크기·팔레트·위치를 확인해야 한다.
- 다음 시작점: `src/data/graphics/trainers.h:482-483,837`의 기존 연결을 확인하고, `make hns -j8` 재실행 후 `TRAINER_CLASS_CHAMPION_HNS` 또는 `FACILITY_CLASS_CHAMPION_LANCE_HNS` 배틀을 테스트한다.

### 2026-09-12 — battle_message.c 1522~1526 문자열 사용처 확인

- 요청/범위: `sText_Your1`, `sText_Opposing1`, `sText_Your2`, `sText_Opposing2`, `sText_EmptyStatus`의 실제 사용 경로 확인.
- 확인 결과: 앞의 네 문자열은 `src/battle_message.c:3647-3681`에서 공격자·방어자·효과 대상의 팀 플레이스홀더(`B_TXT_ATK/DEF/EFF_TEAM1/2`)를 확장할 때 선택된다. `1`은 문장 첫머리(`Your`, `The opposing`), `2`는 문장 중간(`your`, `the opposing`) 형태다. `gBattleStringsTable`에는 팀 필드를 설명하는 메시지에서 해당 플레이스홀더가 사용된다.
- `sText_EmptyStatus`는 `TryGetStatusString` `src/battle_message.c:2876-2903`에서 상태 버퍼를 7개의 `$`와 종료 바이트로 채우는 비교용 초기값이다. `gStatusConditionStringsTable`의 일본어 상태 키와 8바이트 단위로 비교하기 위한 값이며 화면 출력용 문구가 아니다.
- 검증: 정의·플레이스홀더 분기·상태 비교 함수를 `rg`와 줄 번호로 대조. 코드 변경 없음, ROM 재빌드와 실제 게임 검증은 하지 않음.
- 다음 시작점: 전투 메시지 번역 시 팀 플레이스홀더가 들어간 문장을 함께 검토하고, `sText_EmptyStatus`는 번역하지 않는다.

### 2026-09-12 — SafariZoneMenuFrlg HNS 사용 여부 확인

- 요청/범위: `src/battle_message.c`의 `gText_SafariZoneMenuFrlg`가 HNS에서 표시되는지 확인.
- 확인 결과: 문구 정의는 조건부가 아니어서 심볼이 공통 빌드에 포함될 수 있으나, `src/battle_controller_safari.c:345`에서 `IS_FRLG ? gText_SafariZoneMenuFrlg : gText_SafariZoneMenu`로 선택된다. HNS는 `include/constants/global.h:73-76`에서 `IS_FRLG 0`, `IS_HNS 1`이므로 런타임 표시 문구는 `gText_SafariZoneMenu`다. HNS 사파리 메뉴의 첫 행동 처리도 `src/battle_controller_safari.c:134-138`에서 별도 분기된다.
- 결론: `gText_SafariZoneMenuFrlg`는 HNS 번역 대상이 아니며 FRLG 전용이다. HNS에서 보이는 사파리 메뉴를 번역할 때는 `gText_SafariZoneMenu`를 수정한다.
- 검증: 정의·호출·게임 버전 매크로와 HNS 사파리 스크립트 포함을 소스 대조. 코드 변경 없음, ROM 재빌드와 실제 게임 화면 확인은 하지 않음.
- 다음 시작점: `gText_SafariZoneMenu`의 HNS 화면 출력과 `BALL`/첫 행동의 실제 의미를 확인한 뒤 번역·정렬을 적용한다.

### 2026-09-12 — 기술 선택창 PP 옆 상성 아이콘 원인 확인

- 요청/범위: 배틀 기술 선택창에서 `PP` 옆에 보이는 원형 문자의 출처 확인.
- 확인 결과: `src/battle_controller_player.c:2449`의 `MoveSelectionDisplayMoveEffectiveness`가 `gText_MoveInterfacePP` 뒤에 상성 아이콘을 붙여 `B_WIN_PP`에 다시 출력한다. 빈 원(`{CIRCLE_HOLLOW}`)은 보통 효과, 점이 있는 원(`{CIRCLE_DOT}`)은 효과가 굉장함, 삼각형(`{TRIANGLE}`)은 효과가 별로임, X(`{BIG_MULT_X}`)는 효과 없음이다. 스크린샷의 `o`는 빈 원 아이콘에 해당한다.
- 설정: `include/config/battle.h:420`의 `B_SHOW_EFFECTIVENESS`가 `SHOW_EFFECTIVENESS_ALWAYS`라서 상성 표시가 항상 켜져 있다. 상태 기술이나 상성을 확인할 수 없는 경우에는 아이콘을 표시하지 않는다.
- 검증: 소스의 아이콘 문자열·상성 분기·호출 위치를 `rg`와 줄 번호로 대조. 코드 변경 없음, ROM 재빌드와 실제 게임 검증은 하지 않음.
- 다음 시작점: 상성 아이콘을 유지할지 결정한 뒤, 유지한다면 원형·삼각형·X가 한글 폰트에서 의도한 모양으로 보이는지 실제 기술 선택창에서 확인한다.

### 2026-09-12 — 배틀 UI 텍스트 위치 조사

- 요청/범위: 배틀 UI 한글화를 시작하기 전에 번역 대상 텍스트와 출력 위치를 확인.
- 확인 결과: 명령 메뉴·행동 질문·PP·타입·교체·예/아니오는 `src/battle_message.c:1421-1438`에 정의되고 `src/battle_controller_player.c:882`, `:1656-1802`, `:2019-2085`에서 창에 출력된다. 전투 진행 메시지는 `src/battle_message.c:73-190` 및 `gBattleStringsTable` `:193` 이후에서 관리되며 `BattlePutTextOnWindow` `:3890-3965`가 폰트·정렬·출력 속도를 적용한다. 체력박스의 레벨·HP·닉네임·성별 기호는 `src/battle_interface.c:889-1016`, `:1760-1824`에서 그려진다.
- 화면 배치: 표준 배틀 창 템플릿은 `src/battle_bg.c:157-383`에 있고, 창 ID 의미는 `include/constants/battle.h:684-709`에 있다. 일반 배틀의 메시지 창은 `B_WIN_MSG`, 행동 질문은 `B_WIN_ACTION_PROMPT`, 명령 메뉴는 `B_WIN_ACTION_MENU`, 기술 선택은 `B_WIN_MOVE_NAME_1..4`·`B_WIN_PP`·`B_WIN_PP_REMAINING`·`B_WIN_MOVE_TYPE`·`B_WIN_MOVE_DESCRIPTION`을 사용한다.
- 번역 시 주의: `battle_controller_player.c:1768-1794`의 `CAT:`·`PWR:`·`ACC:`와 `:2038-2048`의 파트너 안내처럼 컨트롤러에 직접 작성된 영문도 별도 대상이다. `{CLEAR_TO ...}`, `{B_*}`, `\n`, 팔레트·색상 제어 코드는 문구와 함께 보존해야 한다.
- 검증: `rg`와 줄 번호 대조로 정의·호출·창 템플릿을 확인. 코드 변경 없음, ROM 재빌드와 실제 게임 화면 확인은 하지 않음.
- 다음 시작점: `src/battle_message.c:1421-1459`의 고정 배틀 UI 문구를 공식 한글로 옮긴 뒤 HNS 빌드와 실제 배틀 메뉴 화면을 확인한다.

### 2026-09-12 — STDSTRING_COINS 사용 경로 확인

- 요청/범위: `src/data/script_menu.h`의 `[STDSTRING_COINS]`가 실제로 사용되는 문맥 확인.
- 확인 결과: `gStdStrings[STDSTRING_COINS]`는 `COMPOUND_STRING("코인")`으로 정의되고, `data/scripts/obtain_item.inc:335,352,361`의 `bufferstdstring` 명령이 `STR_VAR_2`에 복사한다. 숨겨진 코인 습득·코인케이스가 가득 참·코인케이스 없음의 세 분기에서 `gText_FoundXCoins`가 `{STR_VAR_1} {STR_VAR_2}`를 확장한다.
- 구분: 코인 보유량 창은 `src/coins.c:13-21`의 `PrintCoinsString`과 `gText_Coins`를 사용하므로 `[STDSTRING_COINS]`와 별개다.
- 검증: `rg`로 정의·호출·문자열 템플릿을 소스 전체 대조. 코드 변경 없음, 재빌드 불필요.
- 다음 시작점: 코인 습득 메시지 자체를 한글화할 때 `data/text/obtain_item.inc`의 관련 템플릿을 함께 검토한다.

### 2026-09-12 — FRLG 전용 메뉴 호출 재확인

- 요청/범위: `sMultichoiceList_GameCornerPokemonPrizes[]`와 `sMultichoiceList_CeladonVendingMachine[]`의 HNS 사용 여부 확인.
- 확인 결과: 전자는 `MULTI_GAME_CORNER_POKEMON_PRIZES`를 통해 `data/maps/CeladonCity_GameCorner_PrizeRoom_Frlg/scripts.inc:24`에서만 호출되고, 후자는 `MULTI_CELADON_VENDING_MACHINE`을 통해 `data/maps/CeladonCity_DepartmentStore_Roof_Frlg/scripts.inc:207`에서만 호출된다. 두 파일은 `data/event_scripts.s`의 `IS_FRLG` 블록에 포함된다.
- 결론: 두 메뉴 모두 HNS 스크립트에서는 사용되지 않는다. `script_menu.h`의 배열·ID 매핑은 공통 소스라 HNS ROM에 포함될 수 있으나 HNS 흐름에서 호출되지 않는다.
- 검증: `rg`로 ID·배열·이벤트 스크립트 호출을 소스 전체 대조. 코드 변경 없음, 재빌드 불필요.
- 다음 시작점: HNS 전용으로 확인된 메뉴의 실제 화면 검증을 계속한다.

### 2026-09-12 — script_menu.h 후반 HNS 문자열 번역

- 요청/범위: `src/data/script_menu.h` 812행부터 마지막까지의 텍스트 중 HNS 스크립트에 직접 반영되는 부분만 공식 포켓몬 한글 표기로 번역.
- 수정 파일: `src/data/script_menu.h`의 `MultichoiceList_LinkServicesHns`와 `MultichoiceList_BattleModeHns`, `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`.
- 변경과 결정 이유: `data/event_scripts.s`의 공통 및 `.if IS_HNS` 스크립트에서 메뉴 ID 직접 참조를 대조한 결과, 해당 범위의 HNS 원문 항목은 링크 메뉴 4개였다. `TRADE`와 `BATTLE`은 기존 공식 표기 문자열인 `gText_Trade`(`교환`)·`gText_Battle`(`대전`)을 사용하고, `SINGLE BATTLE`·`DOUBLE BATTLE`은 기존 배틀 명칭 표기에 맞춰 `싱글 배틀`·`더블 배틀`로 번역했다. `SEVII ISLANDS`를 비롯한 FRLG 전용 메뉴는 HNS에서 직접 참조하지 않아 변경하지 않았다.
- 출처: 프로젝트 내 기존 한글 문자열 `src/strings.c:857-858`, `src/strings.c:989-990` 및 HNS 직접 참조 목록 `docs/localization/SCRIPT_MENU_HNS.md`를 대조했다(2026-09-12).
- 검증: `git diff --check -- src/data/script_menu.h docs/localization/STATUS.md docs/localization/SESSION_LOG.md` 통과. `make hns -j8` 종료 코드 0, ROM 사용 33,256,260 bytes (99.11%), EWRAM 248,844 bytes, IWRAM 25,704 bytes. 로그 `build/localization-logs/hns-script-menu-tail.log`.
- 게임 화면 확인: 미수행. 금빛시티 포켓몬센터 HNS 링크 메뉴의 항목 표시와 선택 동작을 확인해야 한다.
- 남은 문제: 없음. 화면 검증만 남았다.
- 다음 시작점: 금빛시티 포켓몬센터 HNS에서 링크 메뉴를 열어 `교환`·`대전`과 싱글·더블 배틀 항목이 잘리지 않는지 확인한다.

### 2026-09-12 — 코인 교환 메뉴 전체 열 재조정

- 요청/범위: `50개`·`500개` 행을 아래 네 자리 수량 행의 위치에 맞춤. 사용자가 말한 `100개`는 현재 파일의 `500개`를 뜻하는 것으로 해석.
- 수정 파일: `src/data/script_menu.h`의 코인 교환 메뉴 첫 두 행, `STATUS.md`, `SESSION_LOG.md`.
- 변경과 결정 이유: 앞 공백을 두 칸씩 늘려 `개`를 네 자리 수량 행과 같은 열에 놓고, 가격 앞 공백을 줄여 `원` 열은 유지했다. 기존 가격 문자열과 `CLEAR_TO 0x03`은 보존.
- 검증: `git diff --check -- src/data/script_menu.h` 통과. `make hns -j8` 종료 코드 0, ROM 사용 33,256,276 bytes. 로그 `build/localization-logs/hns-script-menu-coins-all-align.log`.
- 게임 화면 확인: 미수행. 게임코너 코인 교환 메뉴에서 다섯 행의 `개`·`원` 위치 확인 필요.
- 다음 시작점: 빌드 종료 코드 확인 후 화면 검증.

### 2026-09-12 — 게임코너 코인 교환 메뉴 번역·정렬

- 요청/범위: `MultichoiceList_GameCornerCoins`의 50개·500개 기준 행 정렬을 확인하고, 1000개·2500개·5000개 행을 번역해 `개`와 `원` 위치를 맞춤.
- 수정 파일: `src/data/script_menu.h`의 코인 교환 메뉴 세 행, `STATUS.md`, `SESSION_LOG.md`.
- 변경과 결정 이유: 기존 사용자 수정인 첫 두 행은 유지. 네 자리 수량과 다섯 자리 가격은 각각 같은 폭이므로 `1000개 10000원`, `2500개 25000원`, `5000개 50000원`으로 번역해 한 칸 간격을 사용했다. 기존 첫 두 행과 가격 단위의 끝점을 맞춘다.
- 검증: `git diff --check -- src/data/script_menu.h` 통과. `make hns -j8` 종료 코드 0, ROM 사용 33,256,276 bytes. 로그 `build/localization-logs/hns-script-menu-coins-translate.log`.
- 게임 화면 확인: 미수행. 마을 게임코너의 코인 교환 메뉴에서 다섯 행의 `개`·`원` 위치 확인 필요.
- 다음 시작점: 빌드 종료 코드 확인 후 화면 검증.

### 2026-09-12 — 세 자리 가격 끝점 재계산

- 요청/범위: 사용자가 다시 저장한 `MultichoiceList_PrizeMons`에서 세 자리 가격만 네 자리 가격과 `개`의 끝 위치가 맞도록 수정.
- 수정 파일: `src/data/script_menu.h`의 케이시·삐삐 두 행, `STATUS.md`, `SESSION_LOG.md`.
- 변경과 결정 이유: 현재 네 자리 항목이 쉼표 없는 `2800개`·`5500개`·`6500개`이므로 일반 글꼴 숫자 폭(한 자리 6px) 기준 세 자리 항목은 `{CLEAR_TO 0x40}`(64px), 네 자리 항목은 `{CLEAR_TO 0x3a}`(58px)로 계산했다. 숫자와 `개` 사이는 분리하지 않았다. 사용자가 바꾼 가격 표기와 다른 행은 유지.
- 검증: `git diff --check -- src/data/script_menu.h docs/localization/STATUS.md docs/localization/SESSION_LOG.md` 통과. `make hns -j8` 종료 코드 0, ROM 사용 33,256,308 bytes. 로그 `build/localization-logs/hns-script-menu-three-digit-end-align.log`.
- 게임 화면 확인: 미수행. 게임코너 포켓몬 경품 메뉴에서 `120개`·`500개`와 네 자리 가격의 `개` 위치 확인 필요.
- 다음 시작점: 빌드는 종료 코드 0으로 완료했다. 게임코너 포켓몬 경품 메뉴에서 실제 가격 정렬을 확인한다.

### 2026-09-12 — 세 자리 게임코너 가격의 끝점 정렬

- 요청/범위: 사용자가 다시 수정한 `MultichoiceList_PrizeMons`에서 세 자리 가격만 네 자리 가격과 `개`의 끝 위치가 맞도록 보정.
- 수정 파일: `src/data/script_menu.h`의 삐삐 경품 한 줄, `STATUS.md`, `SESSION_LOG.md`.
- 변경과 결정 이유: 일반 글꼴 숫자 폭 기준 `500`은 18px, `2,800`은 32px(쉼표 포함)라 14px 차이. `삐삐{CLEAR_TO 0x48}500개`로 가격 시작을 14px 늦춰 숫자와 `개` 사이를 분리하지 않고 끝점을 맞춤. `12개`와 네 자리 항목은 유지.
- 검증: `git diff --check -- src/data/script_menu.h` 통과. 이번 좌표 수정 후 `make hns -j8` 실행 중, 로그 `build/localization-logs/hns-script-menu-three-digit-end-align.log`.
- 게임 화면 확인: 미수행. 게임코너 포켓몬 경품 메뉴에서 `500개`와 네 자리 가격의 `개` 위치를 확인해야 함.
- 다음 시작점: 빌드 종료 코드 확인 후 화면 검증.

### 2026-09-12 — 인형 가격 기준 복원 및 게임코너 경품·기술머신 번역

- 요청/범위: 나무지기·아차모·물짱이 인형의 기존 가격 위치를 기준으로 레지 인형을 맞추고, `MultichoiceList_GameCornerTMs`까지 번역·가격 정렬. 세 자리 가격의 `개` 위치도 통일.
- 수정 파일: `src/data/script_menu.h`의 인형·포켓몬 경품·기술머신 메뉴, `STATUS.md`, `SESSION_LOG.md`.
- 변경과 결정 이유: 사용자가 제시한 일반 인형 세 행은 그대로 복원. 레지 인형의 `9000개`는 같은 `0x3a` 위치에서 시작. 포켓몬 경품 이름은 프로젝트의 공식 한글 명칭 원칙을 적용하고, 기술명은 `src/data/moves_info.h`의 기존 한글 이름과 대조. 경품의 세 자리 가격은 `0x64`, 네 자리 가격은 `0x58`에서 시작하고 `개`는 모두 `0x80`에 둠. 기술머신 가격도 `0x58`/`0x80` 사용.
- 검증: `git diff --check -- src/data/script_menu.h` 통과. `make hns -j8` 종료 코드 0, ROM 사용 33,256,340 bytes. 로그 `build/localization-logs/hns-script-menu-prizes-tms.log`.
- 게임 화면 확인: 미수행. 보라시티 게임코너 경품 메뉴에서 가격 열과 잘림 확인 필요.
- 다음 시작점: 빌드 종료 코드를 확인하고 인형·포켓몬 경품·기술머신 화면을 검증.

### 2026-09-12 — 두 인형 메뉴의 가격 열 통일

- 요청/범위: 나무지기·아차모·물짱이 인형과 레지락·레지아이스·레지스틸 인형의 코인 수 시작 위치를 맞춤.
- 수정 파일: `src/data/script_menu.h`의 일반 인형 세 행, `STATUS.md`, `SESSION_LOG.md`.
- 변경과 결정 이유: 레지아이스인형의 이름 길이를 수용하는 `0x50` 가격 열을 두 메뉴 모두 사용. 기존의 `1000개`와 `9,000개` 표기는 유지.
- 검증: `git diff --check -- src/data/script_menu.h` 통과. `make hns -j8` 종료 코드 0, ROM 사용 33,256,324 bytes. 로그 `build/localization-logs/hns-script-menu-doll-column.log`.
- 게임 화면 확인: 미수행. 두 게임코너 인형 메뉴의 가격 열과 잘림 확인 필요.
- 다음 시작점: 빌드 결과 확인 후 해당 메뉴 화면 검증.

### 2026-09-12 — 레지 인형 가격 좌표 재검토

- 요청/범위: 길이가 다른 레지아이스·레지스틸 이름에 같은 `{CLEAR_TO 0x3a}`를 써도 되는지 확인하고 정렬 보정.
- 수정 파일: `src/data/script_menu.h`의 레지 인형 세 행, `STATUS.md`, `SESSION_LOG.md`.
- 원인과 결정: `src/text.c`의 일반 글꼴 한글 폭은 8px이고 `CLEAR_TO`는 현재 X가 지정 좌표보다 작을 때만 앞으로 이동한다. 레지아이스인형 7글자만으로 최소 56px이므로 58px 지점은 간격이 부족하다. 세 행을 80px(`0x50`) 가격 시작과 `9,000개` 표기로 통일.
- 검증: `git diff --check -- src/data/script_menu.h` 통과. `make hns -j8` 종료 코드 0, ROM 사용 33,256,340 bytes. 로그 `build/localization-logs/hns-script-menu-regi-width.log`. 실제 메뉴 화면 미확인.
- 다음 시작점: 빌드 종료 코드와 ROM 갱신을 확인한 뒤 게임코너 메뉴에서 정렬 확인.

### 2026-09-12 — 레지아이스인형 가격 위치 조정

- 요청/범위: 레지아이스인형 행의 가격 시작 위치를 다른 레지 인형 행과 맞춤.
- 수정 파일: `src/data/script_menu.h`의 레지아이스인형 한 줄, `STATUS.md`, `SESSION_LOG.md`.
- 변경과 결정 이유: 사용자가 저장한 세 행 중 레지아이스인형의 단순 공백을 레지스틸인형과 동일한 `{CLEAR_TO 0x3a}`로 교체. 다른 사용자의 메뉴 번역 변경은 유지.
- 검증: `git diff --check -- src/data/script_menu.h` 통과. `make hns -j8` 종료 코드 0, ROM 사용 33,256,340 bytes. 로그 `build/localization-logs/hns-script-menu-regice-align.log`.
- 게임 화면 확인: 미수행. 금빛시티 게임코너에서 가격 정렬 확인 필요.
- 다음 시작점: 빌드 결과 확인 뒤 해당 메뉴 화면을 확인.

### 2026-09-12 — 레지 시리즈 인형 가격 정렬

- 요청/범위: `src/data/script_menu.h`의 레지락·레지아이스·레지스틸 인형 세 항목을 한글 가격 표기로 맞춤.
- 수정 파일: `src/data/script_menu.h`, `STATUS.md`, `SESSION_LOG.md`.
- 변경과 결정 이유: 디스크에는 IDE 예시의 한글 문구가 저장돼 있지 않아 해당 세 행만 한글로 교체. 세 가격은 같은 `{CLEAR_TO 0x58}` 위치에서 `9,000개`로 시작하게 함. 다른 인형 메뉴는 유지.
- 검증: `git diff --check -- src/data/script_menu.h` 통과. `make hns -j8` 종료 코드 0, ROM 사용 33,256,340 bytes. 로그 `build/localization-logs/hns-script-menu-regi.log`.
- 게임 화면 확인: 미수행. 금빛시티 게임코너의 레지 인형 메뉴에서 줄 정렬 확인 필요.
- 다음 시작점: 빌드 종료 코드와 `pokehns.gba` 갱신 확인 후, 실제 메뉴 화면의 잘림을 확인.

### 2026-09-13 — battle_message.c 번역 수정 후 오류 재확인
- 요청/범위: 사용자가 수정한 `src/battle_message.c`의 HNS 빌드 오류 재확인. 기존 목호 배틀 스프라이트 이식 파일은 검사만 하고 변경하지 않음.
- 수정 파일: 소스는 수정하지 않음. `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`에 재확인 결과를 기록.
- 확인 결과: 이전 `src/battle_message.c:1421`의 미정의 `B_ACTIVE_NAME_WITH_PREFIX` 오류는 사라졌다. 현재 1489·1490행의 `{B_PARTNER_NAME_WITH_CLASS{...}` 토큰은 닫는 `}`가 누락되어 있다. 1512행에는 `{B_OPPONENT_MON 1_NAME}`처럼 토큰 중간에 공백이 들어갔고, 1516행에는 후행 공백이 있다.
- 검증: `make hns -j8` 종료 코드 2. 컴파일러가 `src/battle_message.c:1489`에서 `unexpected character '{' within curly brackets` 및 `expected expression at end of input`을 보고했다. `git diff --check -- src/battle_message.c`는 1516행 후행 공백 때문에 종료 코드 2. 로그 `build/localization-logs/hns-battle-message-recheck.log`.
- 게임 화면 확인: 빌드 실패로 미수행. `pokehns.gba`는 갱신되지 않았음.
- 남은 문제: 1489·1490행 토큰 닫기, 1512행 토큰 오탈자, 1516행 후행 공백을 사용자 수정 후 HNS 빌드를 재실행해야 함.
- 다음 시작점: 위 세 지점을 수정한 뒤 `make hns -j8`을 실행하고, 성공하면 목호 배틀 전면 그림과 배틀 메시지 화면을 확인.

### 2026-09-13 — battle_message.c 토큰 수정 후 링크 오류 재확인
- 요청/범위: 사용자가 수정한 `src/battle_message.c`의 토큰·후행 공백 오류 재확인 및 HNS 전체 빌드.
- 수정 파일: 소스는 수정하지 않음. `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`에 최신 결과를 기록.
- 확인 결과: 1489·1490행의 파트너 토큰, 1512행의 상대 포켓몬 토큰, 1516행 후행 공백은 모두 정리되어 `battle_message.c` 컴파일이 통과했다. 링크 단계에서 `src/battle_arena.c`가 요구하는 `gText_Judgment`를 찾지 못했다. 현재 [src/battle_message.c:1483]의 심볼명이 `gText_Judgement`로 정의되어 헤더·호출부와 불일치한다.
- 검증: `git diff --check -- src/battle_message.c` 종료 코드 0. `make hns -j8` 종료 코드 2; 메모리 출력은 ROM 33,256,292 bytes, EWRAM 248,844 bytes, IWRAM 25,704 bytes였으나 링크 실패로 `pokehns.gba`는 갱신되지 않았다. 로그 `build/localization-logs/hns-battle-message-recheck-fixed.log`.
- 게임 화면 확인: 링크 실패로 미수행.
- 남은 문제: `gText_Judgement`를 프로젝트가 참조하는 `gText_Judgment`와 동일한 심볼명으로 맞춘 뒤 링크를 다시 확인해야 함.
- 다음 시작점: [src/battle_message.c:1483]의 심볼명을 수정하고 `make hns -j8` 재실행.

### 2026-09-13 — gText_DefendersStatRose 사용처 확인
- 요청/범위: `src/battle_message.c`의 `gText_DefendersStatRose`가 배틀에서 조합되는 방식과 번역 시 주의할 점 확인.
- 수정 파일: 소스는 수정하지 않음. `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`에 조사 결과를 기록.
- 확인 결과: `gBattleStringsTable[STRINGID_DEFENDERSSTATROSE]`에 등록되고 `gStatUpStringIds[B_MSG_DEFENDER_STAT_CHANGED]`에서 선택된다. 기술·특성 등으로 대상 포켓몬의 능력치가 상승할 때 `B_DEF_NAME_WITH_PREFIX`(대상 이름), `B_BUFF1`(능력치명), `B_BUFF2`(상승 정도)를 채운 뒤 고정된 `rose!`를 붙인다. `UseStatIncreaseItem`도 같은 템플릿을 호출한다.
- 번역 주의: 현재 `gText_StatSharply`·`gText_StatRose`는 한글로 바뀌었지만 `gText_DefendersStatRose`의 고정 `rose!`는 영문이다. 주변 버퍼가 이미 문장 끝을 포함하는 경로가 있으므로 `rose!`를 단순 치환하기 전에 일반 기술·특성·도구 경로의 조합을 함께 확인해야 한다.
- 검증: `rg`로 `gText_DefendersStatRose`, `STRINGID_DEFENDERSSTATROSE`, `B_MSG_DEFENDER_STAT_CHANGED`, `BufferStatRoseMessage`의 참조를 확인했다. 코드 변경이 없어 빌드는 실행하지 않음. 게임 화면은 미확인.
- 남은 문제: 능력치 상승 메시지의 한글 문장 구조와 `B_BUFF2` 버퍼 조합을 실제 게임에서 확인해야 함.
- 다음 시작점: `gText_DefendersStatRose`와 `gText_StatRose`의 최종 한글 문장 구조를 정한 뒤 `make hns -j8` 및 배틀 능력치 상승 화면을 확인.

### 2026-09-13 — 상대 포켓몬 접두사 출처 확인
- 요청/범위: `gText_DefendersStatRose`의 `{B_DEF_NAME_WITH_PREFIX}`에서 `The opposing`이 생성되는 위치 확인.
- 수정 파일: 소스는 수정하지 않음. `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`에 조사 결과를 기록.
- 확인 결과: `B_TXT_DEF_NAME_WITH_PREFIX` 처리부(`src/battle_message.c:3241`)가 `HANDLE_NICKNAME_STRING_CASE(gBattlerTarget)`를 호출한다. 매크로는 상대편 트레이너 배틀에서 `sText_FoePkmnPrefix`(`:134`, `The opposing `)를 붙인 뒤 대상 포켓몬 이름을 추가한다. 야생 배틀은 `sText_WildPkmnPrefix`(`:133`, `The wild `)를 사용한다. `sText_Opposing1/2`는 팀 관련 플레이스홀더용으로 별개다.
- 번역 주의: 현재 `sText_FoePkmnPrefix`와 `gText_DefendersStatRose`의 소유격·문장 구조가 영문으로 남아 있으므로, 접두사만 `상대`로 바꾸기보다 한국어 조사와 함께 조정해야 한다.
- 검증: `rg`와 `nl`로 매크로 정의·플레이스홀더 처리부·접두사 정의·호출부를 확인했다. 코드 변경이 없어 빌드는 실행하지 않음. 게임 화면은 미확인.
- 남은 문제: 상대 포켓몬 이름 접두사와 능력치 상승 문장의 최종 한글 조합을 결정해야 함.
- 다음 시작점: `sText_FoePkmnPrefix`, `sText_WildPkmnPrefix`, `gText_DefendersStatRose`의 한국어 문장 구조를 정한 뒤 HNS 빌드와 배틀 메시지 화면을 확인.

### 2026-09-13 — 공격·방어 포켓몬 접두사 상수 사용처 확인

- 요청/범위: `src/battle_message.c`의 `sText_FoePkmnPrefix2/3/4`와 `sText_AllyPkmnPrefix/2/3`이 배틀 메시지에서 어떻게 사용되는지 확인.
- 수정 파일: 소스는 수정하지 않음. `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`에 조사 결과를 기록.
- 확인 결과: `B_TXT_ATK_PREFIX1`·`B_TXT_DEF_PREFIX1`은 플레이어 측이면 `sText_AllyPkmnPrefix`, 상대 측이면 `sText_FoePkmnPrefix2`를 선택한다. `PREFIX2` 토큰은 각각 `sText_AllyPkmnPrefix2`·`sText_FoePkmnPrefix3`, `PREFIX3` 토큰은 `sText_AllyPkmnPrefix3`·`sText_FoePkmnPrefix4`를 선택한다(`src/battle_message.c:3445-3480`). 토큰 ID는 `include/battle_message.h:58-63`에 정의되어 있다.
- 현재 상태: 저장소의 배틀 메시지 문자열에는 `{B_ATK_PREFIX1}`·`{B_DEF_PREFIX1}`·`{B_ATK_PREFIX2}`·`{B_DEF_PREFIX2}`·`{B_ATK_PREFIX3}`·`{B_DEF_PREFIX3}` 직접 참조가 없어 현재 HNS 화면에서 이 상수들이 호출되는 경로는 확인되지 않았다. 여섯 상수의 값이 모두 `Ally`/`Opposing`인 것은 문맥별 확장 지점을 나눠 둔 레거시 구조 때문이다. `sText_FoePkmnPrefix`의 `The opposing ` 및 `sText_Opposing1/2` 팀 표현은 별도 문자열이다.
- 검증: `rg`, `nl`로 상수 정의·토큰 ID·파서 분기와 메시지 참조를 확인했다. 코드 변경이 없어 `make hns -j8`은 실행하지 않았고, 실제 게임 화면도 확인하지 않았다.
- 다음 시작점: 이 토큰을 새 한글 배틀 메시지에서 사용할 경우 공격자·방어자 측 분기와 뒤따르는 조사·소유격을 함께 설계한 뒤 HNS 빌드 및 배틀 화면을 확인한다.

### 2026-09-13 — B_ATK_TEAM1 출력 경로 확인

- 요청/범위: `{B_ATK_TEAM1}`이 배틀 메시지에서 어떤 문자열로 확장되는지 확인.
- 수정 파일: 소스는 수정하지 않음. `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`에 조사 결과를 기록.
- 확인 결과: `src/battle_message.c:3647-3652`의 `B_TXT_ATK_TEAM1` 분기는 `IsOnPlayerSide(gBattlerAttacker)`를 검사한다. 플레이어 측 공격자면 `sText_Your1`(`우리`), 상대 측 공격자면 `sText_Opposing1`(`상대`)를 선택한다. 현재 `gText_PkmnShroudedInMist`(`:73`)에서는 `{B_ATK_TEAM1} 편은`이 `우리 편은` 또는 `상대 편은`으로 출력된다.
- 참고: `{B_ATK_TEAM2}`는 같은 공격자를 검사하지만 `sText_Your2`·`sText_Opposing2`를 사용한다. 영문 원문에서는 문장 중간 소문자 접두사를 위한 슬롯이지만 현재 HNS 값은 두 슬롯 모두 `우리`·`상대`다.
- 검증: `rg`, `nl`로 토큰 정의·문자열 값·파서 분기를 확인했다. 코드 변경이 없어 `make hns -j8`은 실행하지 않았고, 실제 게임 화면도 확인하지 않았다.
- 다음 시작점: `{B_DEF_TEAM1/2}` 또는 `{B_EFF_TEAM1/2}`를 번역할 때도 각각 방어 대상·효과 주체의 측 판정과 뒤따르는 한국어 조사를 함께 확인한다.

### 2026-09-12 — HNS 스크립트 메뉴 직접 참조 조사

- 요청/범위: `src/data/script_menu.h`에서 HNS에 반영되는 스크립트의 메뉴만 찾기. 번역과 코드 수정은 하지 않음.
- 수정 파일: `docs/localization/SCRIPT_MENU_HNS.md`, `STATUS.md`, `SESSION_LOG.md`.
- 방법과 결과: `data/event_scripts.s`의 공통 및 `.if IS_HNS` 포함 파일에서 `multichoice` 계열 명령의 메뉴 ID를 `gMultichoiceLists`와 대조. 178개 ID 중 116개 직접 참조, 그중 45개 HNS 전용 스크립트 참조. 62개는 조사 범위에서 직접 참조 없음. 각 ID의 스크립트 예시와 호출 수는 조사 문서에 기록.
- 검증: 소스 정적 조사. 코드·문자열 변경이 없어 ROM 재빌드는 하지 않음. 실제 게임 화면은 미확인.
- 남은 문제: 소스 포함 여부만으로 맵 접근 가능성이나 조건 분기 도달 가능성을 판정할 수 없음. 동적 메뉴와 C 코드에서 직접 생성하는 메뉴는 별도 대상.
- 다음 시작점: `SCRIPT_MENU_HNS.md`의 HNS 전용 항목부터 해당 맵 스크립트와 이벤트 조건을 확인.

### 2026-09-11 — 이전 작업 정리 및 Soulgold 이식 완료

- 요청: 1~9세대 명칭 한글화, Soulgold 배틀 텍스트 박스와 배틀 포켓몬 그림 적용, HNS 기존 메가진화 활성화.
- 변경 파일: 종 데이터 헤더, 기술 데이터와 이름 길이 상수, 배틀 그래픽·팔레트와 선언 헤더, 공통 애니메이션, 종/포켓몬 설정. 세부 범위는 STATUS 참고.
- 결정: 니드런만 이름에 성별 표기. Soulgold 고유 종 6개 제외. 필드 동행/파티 아이콘은 이번 이식 범위에 포함하지 않음.
- 문제 해결: 누락된 공유 애니메이션 테이블 추가. ROM 923,860 bytes 초과 문제는 별도 메가 울음소리 샘플을 꺼서 해결. 메가야도란·레쿠쟈의 기본 cry fallback도 추가.
- 검증: `make hns -j8` 종료 코드 0. ROM 33,256,180 bytes 사용. 그래픽 필드와 비그래픽 데이터 보존 검사 통과(cry fallback 두 건 예외). 관련 diff 공백 검사 통과.
- 로그: `evidence/2026-09-11-hns-build.txt`.
- 게임 화면 확인: 미수행. 싱글/더블 배틀, 이로치, 애니메이션·그림자, 메가진화 확인이 남아 있음.
- 다음 시작점: 완료된 이식 재실행 없이 최신 사용자 요청 대상부터 확인.

### 2026-09-11 — 세션 간 인수인계 체계 구성

- 요청: 다른 세션에서 같은 한글화 프로젝트를 이어갈 수 있는 워크플로우 설계.
- 수정 파일: 루트 `AGENTS.md`, `docs/localization/{STATUS,WORKFLOW,SESSION_LOG}.md`, 보존용 빌드 로그.
- 결정: 자동 진입 지침, 현재 상태, 작업 절차, 누적 이력을 분리. 의미 있는 작업 지점마다 상태를 갱신해 갑작스러운 세션 중단에 대비.
- 검증: 문서 내부 로컬 링크와 필수 파일 확인. 현재 메가/그림 설정과 이름 길이 상수, 기존 빌드 로그를 실제 파일과 대조. 문서 작업이므로 ROM 재빌드는 하지 않음.
- 남은 문제: 인수인계 문서는 실제 파일의 백업이 아님. 다른 PC 이동 시 미추적 PNG/PAL을 포함한 작업 트리도 전달해야 함.
- 다음 시작점: AGENTS → STATUS → WORKFLOW → 이 기록 순으로 읽고, 사용자가 지정하는 다음 한글화 범위를 진행.

### 2026-09-11 — 디버그 메뉴 설정 및 호출 방법 확인

- 요청: 디버그 메뉴가 꺼져 있으면 켜고 게임 내 호출 방법 안내.
- 확인: `include/config/debug.h`의 필드·배틀 디버그 설정은 이미 TRUE. 중복 정의/해제 여부와 `src/field_control_avatar.c`, `src/battle_controller_player.c`의 입력 처리 확인.
- 사용법: 필드에서 R을 누른 채 START. 배틀 행동 선택 화면에서 SELECT. 일반 START 메뉴 항목으로 표시하는 옵션은 FALSE.
- 수정 파일: STATUS와 SESSION_LOG만 갱신. 코드 변경이 필요 없어 ROM 재빌드는 하지 않음.
- 게임 화면 확인: 미수행. 에뮬레이터의 GBA R/START/SELECT 버튼 매핑을 사용해야 함.
- 다음 시작점: 다음 사용자 한글화 요청에 따라 진행.

### 2026-09-11 — 기술 설명 L / 메가진화 START 입력 분리

- 요청: 기술 설명 키와 표시 이미지를 L로 복구하고 START로 메가진화 선택.
- 수정 파일: `include/config/battle.h`의 `B_MOVE_DESCRIPTION_BUTTON`을 START_BUTTON에서 L_BUTTON으로 변경.
- 원인: `HandleInputChooseMove`에서 기술 설명 분기가 START 기믹 분기보다 먼저 실행되어 START 입력을 가로챔.
- 버튼 이미지: `src/battle_interface.c`의 기존 조건부 선언이 `move_info_window_l.4bpp`를 선택하므로 그림 파일을 수정할 필요 없음.
- 사용법: 기술 선택 화면에서 L로 설명 열기/닫기, START로 메가진화 선택 후 기술 확정. 기존 메가진화 조건은 유지. 버튼 모드 L=A에서는 설명 기능이 비활성화되므로 일반 모드 사용.
- 검증: 대상 diff 공백 검사 통과. `make hns -j8` 실행 중, 로그 `build/localization-logs/hns-move-info-l.log`.
- 게임 화면 확인: 미수행.
- 다음 시작점: 실행 중 빌드 종료 코드와 갱신된 ROM 확인 후 STATUS 및 이 기록 갱신.

### 2026-09-13 — pokeemerald-kr 배틀 메시지 대조·반영

- 요청/범위: `../pokeemerald-kr/src/battle_message.c`의 텍스트를 현재 HNS `src/battle_message.c`의 대응 항목에 반영. 코드 구성·위치·순서·주석은 보존하고 문자열 리터럴 내부만 변경.
- 수정 파일: `src/battle_message.c`, `docs/localization/BATTLE_MESSAGE_KR_COMPARE.md`, `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`.
- 변경과 결정 이유: 같은 이름의 고정 문자열 62개, `STRINGID`로 대응하는 테이블 문자열 311개, 포켓블록 맛 문자열 5개를 pokeemerald-kr의 한글 텍스트로 교체했다. KR 파일과 HNS의 토큰 이름 차이는 `B_SCR_ACTIVE_NAME_WITH_PREFIX` → `B_SCR_NAME_WITH_PREFIX`, `B_SCR_ACTIVE_ABILITY` → `B_SCR_ABILITY`, `DARK_GREY` → `DARK_GRAY`로 문자열 내부에서만 호환시켰다. HNS는 기술명을 `B_BUFF3`에 채우므로 `sText_AttackerUsedX`도 HNS 버퍼와 조사 토큰을 사용하도록 조정했다. 같은 선언명이 없는 HNS 선언 39개(일부는 다른 선언명·공통 `STRINGID`로 같은 텍스트가 있음)와 대응 `STRINGID`가 없는 HNS 359개는 대조 보고서에 고정 문자열과 소스 위치를 모두 기록했다.
- 검증: `/tmp/battle_message_hns.before.c`와 비교해 줄 수(4024), 실제 문자열 리터럴 수(916), 문자열 바깥 구조가 동일하고 리터럴 내부 378곳만 변경됨을 확인했다. `git diff --check -- src/battle_message.c` 통과. 미지원 토큰 별칭 검색 결과 없음. `make hns -j8` 종료 코드 0, 컴파일·링크·`pokehns.gba` 생성 성공. ROM 33,256,484 bytes(99.11%), EWRAM 248,844 bytes, IWRAM 25,704 bytes. 로그: `build/localization-logs/battle-message-kr-merge.log`.
- 게임 화면 확인: 에뮬레이터에서 배틀 메시지를 전수 확인하지 않음. 빌드와 문자열 토큰 전처리·컴파일 검증은 완료했으며, 실제 출력의 줄바꿈·창 너비·동적 조사 조합은 후속 화면 검증이 필요하다.
- 남은 문제: HNS 확장 메시지 359개와 이름이 달라 KR 파일에서 직접 대응하지 않은 고정 선언 39개는 이번 범위에서 번역하지 않았고 보고서에 남겼다. 실제 화면에서 긴 한글 문장과 능력치 상승·기술 사용·도망·포켓블록 메시지를 확인해야 한다. `B_TXT_EUNNEUN` 등 조사 토큰의 `charmap.txt` 값과 런타임 분기(`include/battle_message.h`)는 별도 영역이므로, 텍스트 전용 변경으로는 동적 조사 조합을 보장하지 않는다. 헤더 구조는 이번 요청 범위 밖이라 수정하지 않았다.
- 다음 시작점: 생성된 `pokehns.gba`를 에뮬레이터에서 실행해 배틀 메시지 및 목호 배틀 전면 그림을 확인한다. 문제 발견 시 `src/battle_message.c`의 해당 문자열 리터럴만 조정하고 `make hns -j8`을 재실행한다.

### 2026-09-13 — sText_LegendaryPkmnAppeared 사용처 확인
- 요청/범위: `src/battle_message.c`의 `sText_LegendaryPkmnAppeared`가 출력되는 전투 조건 확인.
- 수정 파일: 소스는 수정하지 않음. `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`에 조사 결과를 기록.
- 확인 결과: `BufferStringBattle`의 `STRINGID_INTROMSG`에서 트레이너 전투가 아니며 고스트 전투도 아닐 때 `BATTLE_TYPE_LEGENDARY` 플래그를 검사하고, 플래그가 켜져 있으면 해당 문구를 선택한다(`src/battle_message.c:2390-2468`, 선택부 `:2456-2457`). `{B_OPPONENT_MON1_NAME}`은 상대편 첫 번째 배틀 포켓몬 이름으로 확장된다. 플래그는 `BattleSetup_StartLatiBattle`, `BattleSetup_StartLegendaryBattle`, `StartGroudonKyogreBattle`, `StartRegiBattle`에서 설정된다.
- 대표 상황: 라티아스·라티오스, 루기아·칠색조, 뮤·세레비·지라치·테오키스, 카푸 계열, 그란돈·가이오가·레쿠쟈, 레지 계열 등의 이벤트성 전설 포켓몬과 시작하는 야생 배틀. 문구 선택 함수가 종 ID를 직접 검사하지 않으므로, 해당 플래그를 사용하는 다른 야생 전투에도 적용된다.
- 검증: `rg`와 `nl`로 선언·참조·플래그 설정 함수와 이벤트 스크립트를 확인했다. 코드 변경이 없어 빌드는 실행하지 않았고, 실제 게임 화면도 확인하지 않았다.
- 남은 문제: 전설 포켓몬 이벤트에서 실제 출력되는 이름과 줄바꿈은 에뮬레이터에서 확인해야 한다.
- 다음 시작점: `STRINGID_INTROMSG` 전설 분기를 실제 전투에서 확인하고, 이름 표시나 창 너비 문제가 있으면 `sText_LegendaryPkmnAppeared` 문자열만 조정한다.

### 2026-09-13 — sText_LinkPartnerSentOutPkmn1GoPkmn 줄바꿈 확인
- 요청/범위: `sText_LinkPartnerSentOutPkmn1GoPkmn`이 한 줄에 고정 출력되는지와 사용 조건 확인.
- 수정 파일: 소스는 수정하지 않음. `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`에 조사 결과를 기록.
- 확인 결과: `STRINGID_INTROSENDOUT`의 아군·더블·링크 멀티 분기에서 파트너 측 배틀러가 처리될 때 선택된다(`src/battle_message.c:2470-2493`). `{B_LINK_PARTNER_NAME}`, `{B_LINK_PLAYER_MON1_NAME}`, `{B_LINK_PLAYER_MON2_NAME}`이 각각 링크 파트너 이름과 두 포켓몬 이름으로 확장된다.
- 줄바꿈: 원본 리터럴에는 수동 `\n`이 없지만, `BattleStringExpandPlaceholders`가 확장 후 `BreakStringAutomatic(dst, BATTLE_MSG_MAX_WIDTH, BATTLE_MSG_MAX_LINES, ...)`를 호출한다(`src/battle_message.c:3730`). 폭 208을 넘으면 공백 기준으로 자동 줄바꿈하며 최대 2줄을 사용하므로 이름 길이에 따라 한 줄 또는 두 줄로 표시된다.
- 검증: `rg`, `nl`로 선택 분기와 자동 줄바꿈 호출을 확인했다. 코드 변경이 없어 빌드는 실행하지 않았고, 실제 게임 화면도 확인하지 않았다.
- 남은 문제: 한글 이름·파트너 이름 조합의 실제 폭과 줄바꿈 위치는 에뮬레이터에서 확인해야 한다.
- 다음 시작점: 링크 멀티 더블 배틀에서 파트너 출전 문구를 확인하고, 잘림이 있으면 해당 문자열에만 의도적인 `\n`을 검토한다.

### 2026-09-13 — sText_PkmnSwitchOut 실제 사용 여부 확인
- 요청/범위: `sText_PkmnSwitchOut`이 주석처럼 미사용인지 현재 코드 참조를 확인.
- 수정 파일: 소스는 수정하지 않음. `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`에 조사 결과를 기록.
- 확인 결과: 저장소 전체에서 선언부(`src/battle_message.c:124`) 외 참조가 없다. 현재 `STRINGID_RETURNMON` 처리부(`src/battle_message.c:2532-2607`)는 플레이어·링크·파트너·상대 상황별 다른 철회 문구를 선택하며 `sText_PkmnSwitchOut`을 선택하지 않는다. 따라서 실제 게임 출력 경로가 없는 레거시 문자열이다.
- 검증: `rg`로 저장소 전체 심볼 참조와 `STRINGID_RETURNMON` 선택 분기를 확인했다. 코드 변경이 없어 빌드는 실행하지 않았고, 실제 게임 화면도 확인하지 않았다.
- 남은 문제: 없음. 향후 Shift 모드 등 새 경로를 구현할 때만 이 문자열을 연결할 수 있다.
- 다음 시작점: 현재 사용 중인 `sText_PkmnComeBack` 계열을 번역·검증할 경우 `STRINGID_RETURNMON`의 `hpScale`, 더블 배틀, 링크·파트너 분기를 함께 확인한다.

### 2026-09-13 — battle_message.c 210-216번 STRINGID 사용처 확인
- 요청/범위: `STRINGID_SCR_ITDOESNTAFFECT`, `STRINGID_BATTLERFAINTED`, `STRINGID_PLAYERWHITEOUT2_WILD`, `STRINGID_PLAYERWHITEOUT2_TRAINER`, `STRINGID_PLAYERWHITEOUT3`의 실제 출력 상황 확인.
- 수정 파일: 소스는 수정하지 않음. `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`에 조사 결과를 기록.
- 확인 결과: `STRINGID_SCR_ITDOESNTAFFECT`는 `BattleScript_PowderMoveNoEffect`에서 풀 타입·Overcoat 대상, `BattleScript_DoesntAffectScripting`에서 기술 무효·프랭스터 차단·에어벌룬 차단 대상, `BattleScript_GoodAsGoldActivates`에서 상태 기술을 막은 Good as Gold 대상에게 출력된다(`data/battle_scripts_1.s:5708-5721`, `:7749-7761`, `src/battle_move_resolution.c:1736-1808`, `src/battle_util.c:2405-2416`).
- 확인 결과: `STRINGID_BATTLERFAINTED`는 `BattleScript_FaintBattler`가 기절 애니메이션·울음 뒤에 출력하며, 피해 처리나 `tryfaintmon`으로 HP가 0이 된 플레이어·상대 포켓몬 모두에 적용된다(`data/battle_scripts_1.s:4005-4013`, `src/battle_move_resolution.c:2558-2581`, `src/battle_script_commands.c:3832-3888`).
- 확인 결과: `STRINGID_PLAYERWHITEOUT2_WILD`는 일반 야생전에서 파티가 전멸해 화이트아웃할 때 `STRINGID_PLAYERWHITEOUT` 뒤에 출력되고, `STRINGID_PLAYERWHITEOUT2_TRAINER`는 일반 트레이너전 화이트아웃 또는 일반 트레이너전에서 도망을 선택해 포기했을 때 출력된다. 파티 전멸 경로에서는 현재 `B_WHITEOUT_MONEY = GEN_LATEST` 설정에 따라 금액 문구 뒤에 `STRINGID_PLAYERWHITEOUT3`가 이어지지만, 포기 경로인 `BattleScript_ForfeitBattleGaveMoney`에서는 `STRINGID_PLAYERWHITEOUT2_TRAINER`만 출력되고 `STRINGID_PLAYERWHITEOUT3`는 호출되지 않는다(`data/battle_scripts_1.s:4192-4223`, `:8203-8216`, `src/battle_main.c:5704-5708`).
- 검증: `git grep`, `nl`로 다섯 ID의 테이블·배틀 스크립트·C 호출 조건을 확인했다. 코드 변경이 없어 빌드는 실행하지 않았고, 실제 게임 화면도 확인하지 않았다.
- 남은 문제: 다섯 문구 모두 현재 HNS에서 영문 또는 확장 텍스트로 남아 있으므로, 번역할 경우 동적 이름·금액·화이트아웃 문장 순서를 화면에서 확인해야 한다.
- 다음 시작점: `BATTLE_MSG_MAX_WIDTH` 기준 줄바꿈과 한국어 조사 토큰을 포함해 기술 무효·기절·화이트아웃 화면을 실제 에뮬레이터에서 확인한다.

### 2026-09-13 — 플레이어 이름 플레이스홀더 확인
- 요청/범위: 배틀 메시지의 플레이어 이름에 해당하는 `{B_...}` 토큰 확인.
- 수정 파일: 소스는 수정하지 않음. `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`에 조사 결과를 기록.
- 확인 결과: 일반 플레이어 이름은 `{B_PLAYER_NAME}`이며 `BattleStringExpandPlaceholders`의 `B_TXT_PLAYER_NAME` 분기(`src/battle_message.c:3376-3378`)에서 `BattleStringGetPlayerName`을 호출한다. 일반 전투는 `gSaveBlock2Ptr->playerName`, 기록 전투는 `gLinkPlayers[0].name`을 사용한다(`src/battle_message.c:3044-3055`).
- 링크 구분: `{B_LINK_PLAYER_NAME}`은 링크 참가자 이름용 별도 토큰이며 현재 멀티플레이어 ID의 `gLinkPlayers[multiplayerId].name`을 사용한다(`src/battle_message.c:3361-3363`). `{B_PLAYER_MON1_NAME}`·`{B_PLAYER_MON2_NAME}`은 플레이어 포켓몬 이름 토큰이다.
- 검증: `rg`, `nl`로 charmap 정의와 플레이스홀더 파서를 확인했다. 코드 변경이 없어 빌드는 실행하지 않았고, 실제 게임 화면도 확인하지 않았다.
- 남은 문제: 링크·기록·게임 내 파트너 전투에서 실제 이름이 의도한 참가자를 가리키는지는 화면 확인이 필요하다.
- 다음 시작점: 플레이어 이름을 번역 문장에 넣을 때 일반 문구는 `{B_PLAYER_NAME}`, 링크 참가자 문구는 `{B_LINK_PLAYER_NAME}`을 사용한다.

### 2026-09-13 — STRINGID_PLAYERPICKEDUPMONEY 사용처 확인
- 요청/범위: `src/battle_message.c:298`의 `STRINGID_PLAYERPICKEDUPMONEY`가 출력되는 상황 확인.
- 수정 파일: 소스는 수정하지 않음. `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`에 조사 결과를 기록.
- 확인 결과: 이 문구는 일반 야생전 또는 일반 트레이너전에서 `고양이돈받기(PAY DAY)`나 `거다이금화(G-Max Gold Rush)`로 누적한 돈을 전투 승리 후 회수할 때 출력된다. 승리 처리의 `BattleScript_PayDayMoneyAndPickUpItems`가 `givepaydaymoney`를 호출하고, `Cmd_givepaydaymoney`가 링크·기록 링크가 아니며 `gPaydayMoney != 0`일 때 금액을 플레이어 소지금에 추가하고 `BattleScript_PrintPayDayMoneyString`을 실행한다(`src/battle_main.c:5563-5638`, `data/battle_scripts_1.s:4142-4166`, `:5802-5805`, `src/battle_script_commands.c:8424-8442`).
- 출력 순서: 트레이너전은 승리·상대 패배 문구와 상금 문구 뒤에, 야생전은 승리 처리 중 `givepaydaymoney` 단계에서 이 문구가 나온다. 그 뒤 떨어진 도구 줍기와 일반적인 전투 종료 처리가 이어진다. `gBattleTextBuff1`에 계산된 금액이 들어가므로 예시는 `You picked up ¥{금액}!`이다.
- 출력되지 않는 경우: `PAY DAY`로 얻은 금액이 없거나 링크·기록 링크 배틀이면 `Cmd_givepaydaymoney`가 즉시 건너뛰므로 이 문구도 나오지 않는다.
- 검증: `git grep`, `nl`로 ID 참조·승리 스크립트·`givepaydaymoney` 조건을 확인했다. 코드 변경이 없어 빌드는 실행하지 않았고, 실제 게임 화면도 확인하지 않았다.
- 남은 문제: 현재 문구는 영문이므로 번역 시 금액 형식과 전투 종료 화면에서의 표시 시점을 확인해야 한다.
- 다음 시작점: 로컬 야생전에서 `고양이돈받기`를 사용해 승리하고, 링크 배틀에서는 문구가 나오지 않는지 실제 화면에서 확인한다.

### 2026-09-13 — STRINGID_PLAYERWHITEOUT3 출력 순서 확인
- 요청/범위: `src/battle_message.c:216`의 `STRINGID_PLAYERWHITEOUT3`가 어떤 순서로 출력되는지 확인하고 예시를 구성.
- 수정 파일: 소스는 수정하지 않음. `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`에 분기와 예시를 기록.
- 확인 결과: 파티 전멸로 `BattleScript_LocalBattleLost`가 실행되면 먼저 `STRINGID_PLAYERWHITEOUT`가 출력된다. 야생전은 `STRINGID_PLAYERWHITEOUT2_WILD` 다음 `STRINGID_PLAYERWHITEOUT3`, 트레이너전은 `STRINGID_PLAYERWHITEOUT2_TRAINER` 다음 `STRINGID_PLAYERWHITEOUT3` 순서다(`data/battle_scripts_1.s:4192-4217`). 각 문구 사이에 `waitmessage B_WAIT_TIME_LONG`이 있다.
- 예외: 일반 트레이너전에서 도망을 선택한 `BattleScript_ForfeitBattleGaveMoney`는 현재 `B_WHITEOUT_MONEY = GEN_LATEST` 조건에서 `STRINGID_PLAYERWHITEOUT2_TRAINER`만 출력하고 `STRINGID_PLAYERWHITEOUT3`는 출력하지 않는다(`data/battle_scripts_1.s:8208-8216`).
- 검증: `nl`로 스크립트 순서와 `rg`로 ID 참조를 확인했다. 코드 변경이 없어 빌드는 실행하지 않았고, 실제 게임 화면도 확인하지 않았다.
- 남은 문제: 현재 214~216번 문구에 영문이 남아 있어, 번역 시 금액 버퍼와 문장 간 화면 전환을 확인해야 한다.
- 다음 시작점: 야생전 파티 전멸과 트레이너전 파티 전멸을 각각 실행해 `PLAYERWHITEOUT → WHITEOUT2 → WHITEOUT3` 순서를 확인한다.
### 2026-09-13 — STRINGID_PKMNGOTENCOREDMOVE 사용처 확인

- 요청/범위: `src/battle_message.c:311`의 `STRINGID_PKMNGOTENCOREDMOVE`가 출력되는 앙코르 기술 선택 상황 확인.
- 수정 파일: 소스는 수정하지 않음. `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`에 조사 결과를 기록.
- 확인 결과: 앙코르 상태(`gBattleMons[battler].volatiles.encoredMove`가 존재)인 포켓몬이 앙코르된 기술과 다른 기술을 선택하면 `TrySetCantSelectMoveBattleScript`가 `BattleScript_EncoredMove`를 선택하고, `BattleScript_EncoredMove`의 `printselectionstring STRINGID_PKMNGOTENCOREDMOVE`가 선택 화면 메시지를 출력한다(`src/battle_util.c:1397-1421`, `src/battle_main.c:4632-4639`, `data/battle_scripts_1.s:4774-4776`). 배틀 팰리스는 `BattleScript_EncoredMoveInPalace`에서 같은 문구를 사용한다(`data/battle_scripts_1.s:4778-4782`). 메시지 출력 뒤 선택 스크립트가 끝나고 같은 행동 선택 상태로 돌아가므로, 해당 기술만 다시 선택해야 한다.
- 현재 설정: `B_ENCORE_TARGET = GEN_LATEST`이며, 다이맥스·Z기술을 선택한 경우에는 해당 선택 제한을 우회한다(`include/config/battle.h:153`, `src/battle_util.c:1406-1407`). `B_SCR_NAME_WITH_PREFIX`는 선택 중인 포켓몬 이름과 접두사, `B_CURRENT_MOVE`는 앙코르된 기술 이름으로 확장된다.
- 구분: 앙코르된 기술을 선택하면 이 문구는 출력되지 않는다. 앙코르 턴이 끝나거나 PP가 0이면 `BattleScript_EncoredNoMore`가 `STRINGID_PKMNENCOREENDED`를 출력한다(`data/battle_scripts_1.s:4784-4787`, `src/battle_end_turn.c:738-752`).
- 검증: `nl`과 `rg`로 문자열 선언, 선택 제한 조건, 스크립트 분기, 앙코르 종료 처리를 확인했다. 코드 변경이 없어 빌드는 실행하지 않았고, 실제 게임 화면도 확인하지 않았다.
- 다음 시작점: 앙코르 기술과 다른 기술을 선택하는 일반 배틀 및 배틀 팰리스 화면에서 문구와 재선택 동작을 확인한다.
### 2026-09-13 — STRINGID_USINGITEMSTATOFPKMNROSE/FELL 사용처 확인

- 요청/범위: `src/battle_message.c:492-493`의 도구 능력치 상승·하락 메시지가 출력되는 상황을 확인하고 예시를 정리.
- 수정 파일: 소스는 수정하지 않음. `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`에 조사 결과를 기록.
- 확인 결과: 492번 `STRINGID_USINGITEMSTATOFPKMNROSE`는 약점보험 등 직접 스크립트에서 출력되거나, X도구·능력치 상승 열매·일부 소비 아이템의 `B_MSG_STAT_CHANGED_ITEM` 경로에서 선택된다. 대표적인 직접 경로는 `BattleScript_WeaknessPolicy`, `BattleScript_TargetItemStatRaise`, `BattleScript_AttackerItemStatRaise`, `BattleScript_TryIntimidateHoldEffects`다(`data/battle_scripts_1.s:4694-4741`, `:6190-6200`). 소비 아이템 경로는 `BattleScript_ConsumableStatRaiseRet`와 `BattleScript_ItemIncreaseStat`가 능력치 변경 테이블을 통해 이 ID를 선택한다(`data/battle_scripts_1.s:7236-7253`, `data/battle_scripts_2.s:116-122`).
- 확인 결과: 493번 `STRINGID_USINGITEMSTATOFPKMNFELL`은 직접 `printstring`으로 호출되지 않는다. `PrepareStringBattle`이 492번을 준비할 때 `gBattleScripting.statChanger`가 하락 방향이면 493번으로 교체한다(`src/battle_util.c:1215-1237`). `Room Service`처럼 원래 하락하는 도구 효과와 `Contrary`가 상승 효과를 하락으로 반전한 경우가 해당한다.
- 플레이스홀더: `{B_LAST_ITEM}`은 발동 도구명, `{B_BUFF1}`은 능력치명, `{B_SCR_NAME_WITH_PREFIX2}`는 능력치가 변한 포켓몬 이름, `{B_BUFF2}`는 변화 단계에 따른 수식어(`크게` 등)다. +2 단계 예시는 `Using Weakness Policy, the Attack of Pikachu sharply rose!`, 하락 예시는 `Using Room Service, the Speed of Pikachu fell!` 형태다. 본문 문자열이 영문이므로 동적 한글 문자열과 혼합될 수 있다.
- 검증: `rg`와 `nl`로 ID 테이블, 직접 호출 스크립트, 소비 아이템 경로, `PrepareStringBattle`의 상승·하락 치환 조건을 확인했다. 코드 변경이 없어 빌드는 실행하지 않았고, 실제 게임 화면도 확인하지 않았다.
- 다음 시작점: X도구·약점보험·룸서비스·특성 `Contrary` 상황을 실제 배틀에서 실행해 문장과 능력치 변화 순서를 확인한다.
### 2026-09-13 — STRINGID_ENDUREDSTURDY 직전 특성 팝업 확인

- 요청/범위: `src/battle_message.c:543`의 `STRINGID_ENDUREDSTURDY` 출력 직전에 옹골참 특성 팝업이 나타나는지와 위치 확인.
- 수정 파일: 소스는 수정하지 않음. `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`에 조사 결과를 기록.
- 확인 결과: 피해 계산에서 최대 HP의 포켓몬이 옹골참으로 HP 1에 남으면 `MOVE_RESULT_STURDIED`가 설정된다(`src/battle_util.c:8168-8190`). 결과 메시지 처리에서 `BattleScript_SturdiedMsg`가 실행되고, `BattleScript_AbilityPopUpTarget` → `showabilitypopup` → 짧은 대기 후 `printstring STRINGID_ENDUREDSTURDY` 순서로 명령이 진행된다(`data/battle_scripts_1.s:5357-5362`). 따라서 능력 팝업이 이 텍스트보다 먼저 생성된다.
- 위치/문구: 팝업은 대상 배틀러의 편에 따라 싱글 기준 플레이어 x=24(좌측), 상대 x=178(우측)에 생성된다(`src/battle_interface.c:2673-2685`, `:2838-2869`). 상대 포켓몬의 옹골참이면 우측, 플레이어 포켓몬의 옹골참이면 좌측이다. 팝업의 능력명은 `gAbilitiesInfo[ABILITY_STURDY].name`을 사용하며 현재 소스 값은 `STURDY`다(`src/data/abilities.h:41-47`).
- 검증: `rg`와 `nl`로 Sturdy 피해 조건, 결과 플래그, 스크립트 명령 순서, 팝업 좌표와 능력명 출처를 확인했다. 코드 변경이 없어 빌드는 실행하지 않았고, 실제 에뮬레이터 화면은 확인하지 않았다.
- 다음 시작점: 플레이어·상대 포켓몬이 각각 옹골참으로 버티는 전투를 실행해 팝업의 좌우 위치와 텍스트 표시 타이밍을 화면에서 확인한다.

### 2026-09-13 — STRINGID_HURTBYITEM 사용처 확인

- 요청/범위: `src/battle_message.c`의 `STRINGID_HURTBYITEM`이 출력되는 상황을 도구별로 확인하고 예시를 정리.
- 수정 파일: 소스는 수정하지 않음. `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`에 조사 결과를 기록.
- 확인 결과: `BattleScript_ItemHurtRet`가 HP 바 갱신 후 `STRINGID_HURTBYITEM`을 출력하고, 이어 `tryfaintmon`으로 기절 여부를 확인한다(`data/battle_scripts_1.s:7158-7164`). `Life Orb`는 공격 종료 시 피해를 준 공격 뒤 최대 HP의 1/10을 깎고 이 스크립트를 호출한다(`src/battle_hold_effects.c:557-571`, `src/battle_move_resolution.c:3380-3388`). `Sticky Barb`와 독 타입이 아닌 `Black Sludge` 보유자는 엔드 턴 도구 처리에서 최대 HP의 1/8 피해를 받아 이 문구를 출력한다(`src/battle_hold_effects.c:597-609`, `:658-668`, `:1145-1150`, `src/battle_end_turn.c:1286-1297`, `:383-388`). 독 타입의 Black Sludge는 회복 경로라 이 문구가 나오지 않는다. 세 경로 모두 Magic Guard 보유자는 피해를 받지 않는다.
- 플레이스홀더/예시: 엔드 턴 처리에서 해당 포켓몬을 `gBattlerAttacker`로 지정하므로 `{B_ATK_NAME_WITH_PREFIX}`는 피해를 받은 포켓몬 이름으로 확장된다(`src/battle_end_turn.c:1519`, `src/battle_message.c:3238-3240`). `{B_LAST_ITEM}`은 `gLastUsedItem`에서 도구명을 가져온다(`src/battle_message.c:3274-3317`). 예시는 라이프오브를 지닌 피카츄가 공격 성공 후 `Pikachu was hurt by the Life Orb!`, 독 타입이 아닌 포켓몬이 블랙슬러지를 지닌 채 턴을 끝내면 `… was hurt by the Black Sludge!`, 스티키바브 보유자는 `… was hurt by the Sticky Barb!`이다. 현재 HNS 본문은 영문이므로 실제 출력도 이 영문 형태다.
- 검증: `rg`와 `nl`로 메시지 스크립트, 세 도구 효과 함수, 엔드 턴·공격 종료 호출부, 플레이스홀더 확장부를 확인했다. 코드 변경이 없어 빌드와 에뮬레이터 화면 확인은 실행하지 않았다.
- 다음 시작점: 라이프오브 공격 성공, 스티키바브·비독 타입 블랙슬러지 엔드 턴 피해를 각각 실제 화면에서 확인하고, HP 1 상태에서 후속 기절 문구와 한글 도구명 폭을 점검한다.

### 2026-09-13 — STRINGID_PKMNCANTUSEITEMSANYMORE 사용처 확인

- 요청/범위: `src/battle_message.c`의 `STRINGID_PKMNCANTUSEITEMSANYMORE`가 출력되는 상황을 확인하고 예시를 정리.
- 수정 파일: 소스는 수정하지 않음. `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`에 조사 결과를 기록.
- 확인 결과: `BattleScript_EffectEmbargo`에서 명중 판정과 `setembargo` 성공 뒤 애니메이션을 재생하고 이 ID를 출력한다(`data/battle_scripts_1.s:2078-2086`). `Cmd_setembargo`는 대상이 이미 Embargo 상태면 실패 분기로 보내고, 그렇지 않으면 `VOLATILE_EMBARGO`와 `B_EMBARGO_TIMER`를 설정한다(`src/battle_script_commands.c:9293-9306`, `include/config/battle.h:212`). 현재 타이머는 5이며, 종료 시 `STRINGID_EMBARGOENDS`가 출력된다(`src/battle_end_turn.c:841-854`, `data/battle_scripts_1.s:5890-5894`).
- 플레이스홀더/구분: `{B_DEF_NAME_WITH_PREFIX}`는 Embargo를 받은 대상 포켓몬이다. 예를 들어 팬텀이 피카츄에게 Embargo를 성공시키면 `Pikachu can't use items anymore!`이 나오고 5턴 뒤 `Pikachu can use items again!`이 나온다. 이미 Embargo 상태인 포켓몬에게 다시 사용하면 이 문구 대신 기술 실패 처리가 된다. Embargo 상태에서 플레이어가 파티 메뉴로 도구를 선택했을 때는 이 ID가 호출되지 않고 `CannotUseItemsInBattle`이 `gText_WontHaveEffect`(`써도 효과가 없다!`)를 표시한다(`src/item_use.c:1290-1311`, `:1430-1435`, `src/strings.c:299`).
- 검증: `rg`와 `nl`로 Embargo 기술 스크립트, 상태 설정·해제, 타이머, 파티 메뉴의 도구 차단 경로를 확인했다. 코드 변경이 없어 빌드와 에뮬레이터 화면 확인은 실행하지 않았다.
- 다음 시작점: Embargo 명중·재사용 실패·5턴 종료를 실제 배틀에서 확인하고, 대상 이름 접두사와 한글 문장 폭을 점검한다.

### 2026-09-14 — STRINGID_POISONSPIKESSCATTERED 출력 검증

- 요청/범위: `STRINGID_POISONSPIKESSCATTERED`의 실제 출력 경로와 `{B_DEF_PREFIX1}` 치환 결과 확인, HNS 빌드 검증.
- 수정 파일: 소스는 수정하지 않음. `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`, `docs/localization/evidence/2026-09-14-poison-spikes-output-test.txt`에 검증 결과를 기록.
- 호출 경로: `BattleScript_EffectToxicSpikes`의 독압정 설치 성공 메시지, `BattleScript_ToxicDebrisActivates`의 물리 공격 피격 후 특성 발동 메시지, 시작 상태 독압정 처리의 `gStartingStatusStringIds`가 이 ID를 사용한다(`data/battle_scripts_1.s:2015-2022`, `:4954-4963`, `:4504-4507`, `src/battle_message.c:932`).
- 출력 결과: `{B_DEF_PREFIX1}`는 `B_TXT_DEF_PREFIX1` 분기에서 대상 측이 상대면 `상대`, 플레이어 측이면 `우리 편`으로 선택된다(`src/battle_message.c:3463-3467`). 플레이어 측이 독압정을 설치하면 `상대의 발밑에\n독압정이 뿌려졌다!`, 상대 측이 설치하면 `우리 편의 발밑에\n독압정이 뿌려졌다!`가 된다. 리터럴의 토큰과 줄바꿈을 임시 치환하는 정적 검사는 통과했다.
- 빌드: `make hns -j8`를 실행했으나 `src/battle_message.c:547`의 별도 사용자 수정에 `{B_DEF_NAME_WITH_PREFIX}` 닫는 `}`가 없어 `unexpected character '{'` 및 후속 구문 오류로 종료 코드 2가 됐다. 독압정 문자열이나 오류가 난 줄은 수정하지 않았고, 따라서 현재 ROM의 실제 화면 출력은 확인하지 못했다. 전체 로그는 `build/localization-logs/poison-spikes-output-test-20260914.log`에 보관했다.
- 이후 확인: 최신 소스에서는 547번 줄의 닫는 `}`가 수정되어 있다. 최신 `make hns -j8` 재시도와 `make NODEP=1 -j1 build/hns/src/battle_message.o`는 도구·생성 파일 선행 작업에서 시간 초과되어 ROM 링크까지 진행하지 못했다. 독압정 문장과 사용자 수정은 건드리지 않았다.
- 다음 시작점: 선행 도구·생성 파일 작업이 끝난 뒤 `make hns -j8`를 다시 실행하고, Toxic Spikes·Toxic Debris 상황에서 두 편의 문장을 실제 에뮬레이터 화면으로 확인한다.

### 2026-09-14 — STRINGID_PKMNSWITCHEDSTATCHANGES 사용·출력 조사

- 요청/범위: `STRINGID_PKMNSWITCHEDSTATCHANGES`의 호출 기술과 플레이스홀더 출력 예시 확인.
- 확인 결과: `BattleScript_EffectHeartSwap`, `BattleScript_EffectPowerSwap`, `BattleScript_EffectGuardSwap`가 각각 능력치 랭크를 교환한 뒤 같은 ID를 출력한다(`data/battle_scripts_1.s:1929-1962`). Heart Swap은 모든 능력치 랭크, Power Swap은 공격·특수공격, Guard Swap은 방어·특수방어 랭크를 교환한다. `{B_ATK_NAME_WITH_PREFIX}`는 기술 사용자 닉네임이며 상대 측에는 `상대 `, 야생전에는 `야생 ` 접두사가 붙는다(`src/battle_message.c:2917-2932`, `:3238-3240`). Speed Swap은 별도 `STRINGID_ATTACKERSWITCHEDSTATWITHTARGET`를 사용한다(`data/battle_scripts_1.s:1964-1971`).
- 출력 예시: 플레이어 피카츄가 상대 팬텀에게 Power Swap을 사용하면 `Pikachu switched stat changes with its target!`, 상대 팬텀이 플레이어 피카츄에게 Guard Swap을 사용하면 `상대 팬텀 switched stat changes with its target!`이 출력된다. 현재 본문이 영문이므로 포켓몬 이름·접두사만 한글화된 형태가 될 수 있다.
- 수정/검증: 소스는 수정하지 않았고 `rg`와 스크립트 범위를 확인했다. 빌드와 에뮬레이터 출력 검증은 실행하지 않았다.
- 다음 시작점: 이 문구를 한글화할 경우 능력치 교환 기술 3종의 실제 창 너비와 줄바꿈을 HNS ROM에서 확인한다.

### 2026-09-14 — STRINGID_PROTECTEDTEAM 사용·출력 조사

- 요청/범위: `STRINGID_PROTECTEDTEAM`의 호출 기술과 `{B_CURRENT_MOVE}`, `{B_ATK_TEAM2}` 출력 예시 확인.
- 확인 결과: `BattleScript_EffectProtect`/`BattleScript_EffectEndure`에서 `setprotectlike` 후 `gProtectLikeUsedStringIds`를 통해 선택된다. 광역 방어 판정인 `Wide Guard`, `Quick Guard`, `Crafty Shield`, `Mat Block`은 `B_MSG_PROTECTED_TEAM`을 선택한다(`data/battle_scripts_1.s:3175-3183`, `src/battle_script_commands.c:7319-7338`, `src/battle_util.c:5967-5986`). `{B_CURRENT_MOVE}`는 사용 기술명이며 `{B_ATK_TEAM2}`는 공격자 측에 따라 `우리` 또는 `상대`로 확장된다(`src/battle_message.c:1522-1525`, `:3647-3658`).
- 출력 예시: 플레이어가 Wide Guard를 사용하면 현재 본문 기준 `Wide Guard protected 우리 team!`, 상대가 사용하면 `Wide Guard protected 상대 team!`이다. 단일 대상 Protect 계열은 `STRINGID_PKMNPROTECTEDITSELF2`를 사용하므로 이 문구가 나오지 않는다.
- 수정/검증: 소스는 수정하지 않았고 관련 스크립트·플레이스홀더 분기를 `rg`와 `nl`로 확인했다. 빌드와 에뮬레이터 출력 검증은 실행하지 않았다.
- 다음 시작점: 이 문구를 한글화할 경우 기술명과 `우리/상대` 뒤의 영어 `team`을 함께 번역하고, 더블 배틀 창 너비를 HNS ROM에서 확인한다.
### 2026-09-14 — HNS 2.0.5 통합 조사 시작

- 요청/범위: 현재 2.0.4 기반 작업을 공식 2.0.5로 올리면서 한글화·오박사 인트로·스프라이트·메가진화와 모든 사용자 수정을 보존.
- 수정 파일: 공식 2.0.5의 변경 파일 265개와 인수인계 문서. 적용 전 대상 파일은 `build/localization-backups/pre-hns-205-20260914-112129.tar.gz`에 백업.
- 확인: `git fetch origin --tags`로 공식 `Release-v2.0.5`(`1f42b74dff`)를 가져왔다. 현재 HEAD는 `d7d3194fa1`, 공통 기준은 `a9fbb77c6f`, `Release-v2.0.4`는 현재 HEAD의 선조다. 공식 2.0.5는 공통 기준에서 265개 파일을 바꿨고, 현재 사용자 변경과 겹치는 경로는 43개다. 공식 태그 자체에는 현 HEAD의 한국어 처리 파일이 없으므로 태그로 단순 교체하면 기존 패치가 사라진다. 공식 변경은 `graphics/pokemon/` 및 `graphics/trainers/`의 그림 파일을 직접 변경하지 않는다.
- 병합: 265개 중 257개는 공식 파일 그대로 적용, 8개는 공통 기준에서 `git merge-file`로 기존 변경과 자동 병합. 충돌·파일 삭제 없음. 기존 한국어 입력 코드, 목호 그림, 메가 그림 참조를 정적 확인했다. Git HEAD는 기존 로컬 커밋 그대로이며 공식 태그로 checkout하지 않았다.
- 검증: 변경 경로 및 3방향 병합 검사 통과, 문서 `git diff --check` 통과. `make hns -j8` 종료 코드 0(`build/localization-logs/hns-205-integration-20260914.log`), ROM 사용 33,285,300 bytes (99.20%), EWRAM 249,304 bytes, IWRAM 25,704 bytes. `pokehns.gba`는 2026-09-14 11:28:57 KST에 갱신. 실제 게임 화면은 미확인.
- 전체 트리 `git diff --check`는 후행 공백·EOF 빈 줄 다수로 종료 코드 2. 기존 사용자/공식 내용과 섞인 광범위한 공백을 이번에는 변경하지 않았다. 문서 범위 검사에는 오류가 없다.
- 남은 문제: 인트로·메뉴·배틀·목호·포켓몬 그림과 메가진화 기능을 게임 화면에서 확인해야 한다. Git HEAD는 아직 기존 로컬 커밋으로, 2.0.5 통합 결과는 미커밋 상태다.
- 다음 시작점: 생성된 `pokehns.gba`를 에뮬레이터에서 열어 오박사 인트로, 한글 메뉴·배틀 메시지, 목호·포켓몬 그림, 메가진화를 검사한다. 문제 수정 시 `make hns -j8`을 다시 실행한다. 버전 관리 정리가 필요하면 사용자 변경과 2.0.5 통합 파일을 명시적으로 구분해 커밋한다.
### 2026-09-14 — 야생 배틀 등장 문구 조사 누락 수정

- 요청/범위: 실제 화면의 `앗! 야생 구구 튀어나왔다!`에서 `구구가`가 나오지 않는 원인 확인과 수정.
- 수정 파일: `include/battle_message.h`, `src/battle_message.c`, 인수인계 문서.
- 원인: `charmap.txt`의 조사 토큰은 FD 35~3B였으나 C 헤더의 같은 ID는 다른 배틀 토큰을 뜻했다. 조사 뒤의 C 정의도 charmap보다 7칸 작았고, `BattleStringExpandPlaceholders`에는 조사 처리 분기가 없었다. `B_TXT_IGA`가 트레이너 직업 토큰으로 처리되어 야생전에서 빈 문자열이 출력됐다.
- 변경: C ID를 charmap과 일치시키고 조사 7종을 배틀 파서에서 직전 글자의 받침에 따라 출력하게 했다. 예: `구구` → `가`, `팬텀` → `이`.
- 검증: charmap과 C 헤더의 배틀 토큰 68개 ID 정적 대조 통과, 대상 파일 `git diff --check` 통과. `make hns -j8` 종료 코드 0(`build/localization-logs/hns-battle-particle-20260914.log`), ROM 33,285,668 bytes (99.20%), EWRAM 249,304 bytes, IWRAM 25,704 bytes. `pokehns.gba`는 2026-09-14 11:46:28 KST에 갱신. 실제 게임 화면 재확인 전.
- 다음 시작점: mGBA에서 갱신된 ROM을 다시 열어 야생 구구의 `구구가`, 받침 있는 이름의 `팬텀이` 및 다른 조사 토큰을 확인.
### 2026-09-14 — 배틀 명령 메뉴 오른쪽 첫 글자 가림 수정

- 요청/범위: HNS 배틀 명령 메뉴에서 `가방`, `도망간다`의 첫 글자가 잘리는 문제 수정.
- 수정 파일: `src/battle_message.c`, 인수인계 문서.
- 원인: `gText_BattleMenu`의 두 오른쪽 열은 x=48px에서 시작하지만 오른쪽 메뉴 커서의 타일 지우기/그리기가 같은 x=48~55px을 차지한다(`src/battle_controller_player.c`의 `ActionSelectionCreateCursorAt`/`ActionSelectionDestroyCursorAt`). 그래서 첫 글자가 커서 타일에 덮였다.
- 변경: 오른쪽 열 시작을 두 줄 모두 `{CLEAR_TO 56}`으로 이동. 좁은 한글 글꼴 8px에서 최장 4글자는 x=56~87px로 메뉴 창 폭 96px 이내다.
- 검증: 수정 파일 `git diff --check` 통과, 열 위치·문자 폭 정적 계산 통과. `make hns -j8` 종료 코드 0(`build/localization-logs/hns-battle-menu-columns-20260914.log`), ROM 사용 33,285,668 bytes (99.20%), EWRAM 249,304 bytes, IWRAM 25,704 bytes. `pokehns.gba`는 2026-09-14 11:59:03 KST에 갱신. 실제 게임 화면 재확인 전.
- 다음 시작점: mGBA에서 갱신된 ROM을 열어 `가방`, `도망간다` 전체 표시 및 네 칸 커서 이동을 확인.

### 2026-09-14 — 특성·도구 이름 PokeAPI 한국어화 진행

- 요청: 모든 포켓몬의 특성과 포켓몬 도구를 PokeAPI 기반의 공식 한국어 명칭으로 번역.
- 자료 확보: PokeAPI 공식 `ability_names.csv`·`item_names.csv`를 사용하고 언어 ID 3(한국어), 9(영어)을 확인했다. API 데이터에 없는 3세대 이벤트 도구와 HNS/Z-A 전용 도구는 한국어 공식 표기와 종명 기반 수동 대응표로 보완했다.
- 적용: `src/data/abilities.h`의 실제 특성 310개(전체 311개 슬롯)와 `src/data/items.h`의 `ITEM_NAME`/`ITEM_PLURAL_NAME` 1,085개를 교체했다. 한국어는 문법상 복수형을 별도로 만들지 않아 복수형도 같은 한국어 명칭으로 지정했다. `ABILITY_NONE`의 `-------`와 도구의 `????????`는 보존했다.
- 백업: 작업 전 대상 파일은 `build/localization-backups/20260914-ability-item-names/abilities.h`, `items.h`에 보관했다.
- 검증: 정적 치환 수·문자열 길이와 `make hns -j8` 종료 코드 0을 확인했다. 빌드 로그와 ROM 사용량은 아래 완료 기록에 남겼다.
- 남은 확인: mGBA에서 특성 팝업·가방·상점·도구 효과 메시지의 한글 표시와 긴 이름 잘림 여부를 확인한다.

### 2026-09-14 — 특성·도구 이름 PokeAPI 한국어화 완료

- 빌드: `make hns -j8` 종료 코드 0. 로그 `build/localization-logs/hns-ability-item-names-20260914.log`에 메모리 사용량과 링크 결과를 보관했다. ROM 33,284,804 bytes(99.20%), EWRAM 249,304 bytes, IWRAM 25,704 bytes이며 `pokehns.gba`는 13:14:44 KST에 갱신됐다.
- 길이 예외: `INFIN. REPEL`의 직역 `무한벌레회피스프레이`가 `ITEM_NAME_LENGTH`의 종료 제어 바이트를 초과해 `무한벌레스프레이`로 줄였다. 나머지 특성·도구 이름은 컴파일러의 배열 길이 검사를 통과했다.
- 정적 검증: 특성 310개와 `ABILITY_NONE`, 도구 `ITEM_NAME` 902개, `ITEM_PLURAL_NAME` 183개가 모두 한국어화되었고, `ABILITY_NONE`·`????????`·플레이스홀더 토큰은 유지됐다. 복수형은 한국어 문법에 따라 단수와 같은 문자열을 사용한다.
- 남은 확인: 실제 mGBA에서 특성 팝업, 가방·상점 목록, 도구 효과 메시지의 이름 폭과 잘림을 확인한다. 효과 설명 문장은 이름 번역 요청 범위 밖이므로 영문으로 유지했다.

### 2026-09-14 — 무한스프레이 명칭 변경

- 요청: `무한벌레스프레이`를 `무한스프레이`로 변경.
- 수정 파일: `src/data/items.h`의 `INFIN. REPEL` 이름을 `ITEM_NAME("무한스프레이")`로 변경했다.
- 검증: 대상 파일 공백·문자열 길이 검사를 통과했고 `make hns -j8` 종료 코드 0을 확인했다. 로그 `build/localization-logs/hns-infinite-spray-20260914.log`, ROM 사용 33,284,804 bytes(99.20%), EWRAM 249,304 bytes, IWRAM 25,704 bytes, `pokehns.gba` 갱신 시각 18:44:28 KST.

### 2026-09-14 — 메가진화 특성 비활성화 표현 정정

- 문의: 이전 설명의 “비활성화된 메가진화 특성”이 무엇인지 확인.
- 확인: `P_MEGA_EVOLUTIONS`는 `TRUE`라 메가진화는 활성화돼 있다. 별도의 “비활성화된 메가진화 특성” 목록은 없다. `메가런처`는 일반 특성이고, `델타스트림`은 메가레쿠쟈에 실제로 사용된다.
- 비활성화된 것은 특성이 아니라 폼 설정이다. 원시회귀 활성화 전 당시에는 원시회귀(`시작의바다`, `끝의대지`), 울트라버스트(`브레인포스`), 거다이맥스·테라 폼 설정이 각각 꺼져 있어 관련 폼 데이터가 빌드에서 제외됐다. 특성 이름 자체는 `src/data/abilities.h`에 번역돼 있다.
- 수정/검증: 소스·설정은 수정하지 않았고, 상태 문서와 세션 기록만 정정했다. 빌드는 실행하지 않았다.

### 2026-09-14 — 원시회귀 활성화

- 요청: 원시회귀를 활성화.
- 수정: `include/config/species_enabled.h`의 `P_PRIMAL_REVERSIONS`를 `TRUE`로 변경했다. 원시가이오가(`ABILITY_PRIMORDIAL_SEA`, `시작의바다`)와 원시그란돈(`ABILITY_DESOLATE_LAND`, `끝의대지`)의 폼 데이터가 빌드에 포함되며, 블루오브·레드오브를 사용하는 `FORM_CHANGE_BATTLE_PRIMAL_REVERSION` 경로도 활성화된다.
- 검증: 조건부 종 데이터·폼 변경표·그래픽·울음소리 정의를 `rg`로 확인했다. `make hns -j8` 종료 코드 0, 로그 `build/localization-logs/hns-primal-reversions-20260914.log`, ROM 링크 사용량 33,329,156 bytes(99.33%), EWRAM 249,304 bytes, IWRAM 25,704 bytes. 실제 mGBA 화면 검증은 남아 있다.

### 2026-09-14 — 메가진화 문구 사용자 지정 보존

- 요청: 메가진화 반응 문구의 `와`를 사용자가 직접 선택했으므로 다시 바꾸지 않는다.
- 확인: `src/battle_message.c`의 `STRINGID_MEGAEVOREACTING`은 `{B_LAST_ITEM}와`와 사용자가 작성한 나머지 문장을 그대로 유지한다. 이 턴에서는 소스 문구를 재수정하지 않았다.
- 검증: 해당 문자열을 직접 확인했다. 추가 빌드는 실행하지 않았다.

### 2026-09-14 — 메가진화 `와` 상태 재빌드 검증

- 요청: 사용자가 지정한 `와`를 유지한 상태로 다시 테스트.
- 확인: `src/battle_message.c:647`의 `{B_LAST_ITEM}와`와 `:648`의 메가진화 완료 문구를 확인했다. 소스는 수정하지 않았다.
- 검증: 대상 파일 `git diff --check` 통과, `make hns -j8` 종료 코드 0. 로그 `build/localization-logs/hns-mega-message-kr-user-wa-test-20260914.log`, ROM 링크 사용량 33,329,140 bytes(99.33%), EWRAM 249,304 bytes, IWRAM 25,704 bytes, `pokehns.gba` 갱신 시각 21:43:42 KST. 실제 mGBA 화면 확인은 미수행.

### 2026-09-14 — `STRINGID_TARGETCHANGEDTYPE` 출력 예시 확인

- 문의: `{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_BUFF1}타입이 됐다!`의 실제 출력 확인.
- 확인: `BattleScript_EffectSoak`의 `trysoak` 성공 후 이 ID를 출력한다. `{B_BUFF1}`은 `PREPARE_TYPE_BUFFER`로 저장된 타입명이며, `{B_DEF_NAME_WITH_PREFIX}`는 트레이너전 `상대 `·야생전 `야생 ` 접두사를 사용한다. `{B_TXT_EUNNEUN}`은 받침에 따라 `은/는`으로 확장된다.
- 예시: `야생 구구는\n물타입이 됐다!`, `상대 팬텀은\n물타입이 됐다!`.
- 수정/검증: 소스·ROM은 수정하지 않았고 관련 스크립트와 정적 플레이스홀더 처리를 확인했다. 빌드는 실행하지 않았다.

### 2026-09-15 — `STRINGID_RESETSTARGETSSTATLEVELS` 출력 상황 확인

- 문의: `{B_DEF_NAME_WITH_PREFIX}'s stat changes were removed!`의 실제 호출 조건과 출력 예시 확인.
- 확인: `MOVE_EFFECT_CLEAR_SMOG`가 명중·대미지 처리 후 대상의 능력치 랭크가 하나라도 변해 있을 때 모든 랭크를 초기화하고 `BattleScript_MoveEffectClearSmog`를 호출한다. 이 스크립트가 해당 ID를 출력한다.
- 예시: 트레이너전 상대 팬텀의 특수공격 랭크가 올라간 상태에서 클리어스모그를 맞으면 `상대 팬텀's stat changes were removed!`, 야생 구구라면 `야생 구구's stat changes were removed!`이다. 랭크 변화가 없으면 메시지가 나오지 않는다.
- 수정/검증: 소스·ROM은 수정하지 않았고 관련 분기와 플레이스홀더를 `rg`로 확인했다. 빌드는 실행하지 않았다.

### 2026-09-15 — `STRINGID_ELECTROMAGNETISM`·`STRINGID_REFLECTTARGETSTYPE` 출력 상황 확인

- 문의: 두 문자열이 실제로 어느 상황에서 호출되는지와 출력 예시 확인.
- 확인: `STRINGID_ELECTROMAGNETISM`은 전자부유 시작 메시지가 아니라 지속 턴 종료 시 버퍼에 들어가며, `STRINGID_BUFFERENDS`가 `상대 팬텀's electromagnetism wore off!`와 같은 문장을 출력한다. `STRINGID_REFLECTTARGETSTYPE`은 미러타입 성공 뒤 타입 복사 결과를 알리는 문구이며, 예시는 `팬텀 became the same type as 상대 피카츄!`이다. 아르세우스·실버디, 테라스탈 중인 사용자, 타입리스 대상은 실패 분기다.
- 수정/검증: 소스·ROM은 수정하지 않았고 배틀 스크립트, 전자부유 종료 처리, `BS_TryReflectType` 조건을 정적으로 확인했다. 빌드는 실행하지 않았다.

### 2026-09-15 — `STRINGID_BUFFERENDS` 출력 상황 확인

- 문의: `{B_SCR_NAME_WITH_PREFIX}'s {B_BUFF1} wore off!`가 호출되는 상황과 예시 확인.
- 확인: `BattleScript_BufferEndTurn`은 도발·전자부유·회복봉인의 지속 턴이 끝날 때 공통으로 호출된다. 현재 버퍼 이름을 적용하면 `상대 팬텀's 도발 wore off!`, `야생 구구's 전자부유 wore off!`, `팬텀's 회복봉인 wore off!`처럼 출력된다. 현재 문장 본문은 영어다.
- 주의: 전자부유 종료 핸들러는 `gBattleScripting.battler = battler`를 설정하지 않고 공통 스크립트를 실행하므로, `B_SCR_NAME_WITH_PREFIX` 이름이 이전 값으로 남을 가능성이 있다. 도발과 회복봉인은 해당 값을 설정한다.
- 수정/검증: 소스·ROM은 수정하지 않았고 `BattleScript_BufferEndTurn` 호출부와 버퍼 준비 코드를 정적으로 확인했다. 빌드는 실행하지 않았다.

### 2026-09-15 — HNS에 포함된 pokeemerald-expansion 버전 확인

- 요청/범위: 현재 HNS 작업 트리에 포함된 `pokeemerald-expansion` 버전을 소스 상수와 Git 계보로 확인.
- 수정 파일: `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`. 코드와 데이터는 수정하지 않음.
- 확인: `include/constants/expansion.h` 상수는 `EXPANSION_VERSION_MAJOR 1`, `MINOR 15`, `PATCH 2`이고 `EXPANSION_TAGGED_RELEASE FALSE`이다. 주석의 직전 버전은 `1.15.1`이다.
- Git 근거: 1.15.1 릴리스 커밋 `b6c71d33bb`/`fcecfadda6`과 `Start of 1.15.2 cycle` 커밋 `1b79bd80f9`가 모두 현재 HEAD `d7d3194fa1`의 조상임을 `git merge-base --is-ancestor` 및 `git log --follow -- include/constants/expansion.h`로 확인했다.
- 결론: 현재 기반은 **pokeemerald-expansion 1.15.2 개발 사이클**이다. 다만 태그된 1.15.2 정식 릴리스가 아니므로, 외부에 표기할 때는 `1.15.2 development (last release: 1.15.1)`로 구분한다. `README.md`의 1.15.1 표기는 `TODO` 아래 예시로, 현재 소스 상수와 계보를 반영하지 못한다.
- HNS 버전과의 구분: 위 결론은 기반 엔진 버전이며, 별개로 현재 작업 트리에는 HNS `Release-v2.0.5`(`1f42b74dff`) 변경분이 미커밋 상태로 통합돼 있다.
- 검증: 버전 상수, 해당 파일의 `git blame`/이력, 커밋 조상 관계, `Release-v2.0.5` README를 정적 교차확인했다. 문서만 수정했으므로 `make hns -j8`은 실행하지 않았다.
- 게임 화면 확인: 버전 조사에는 불필요하여 미수행.
- 다음 시작점: 버전을 문서나 크레딧에 노출할 경우 `1.15.2` 정식 릴리스로 오해하지 않도록 개발 사이클임을 함께 명시한다.

### 2026-09-15 — pokeemerald-expansion 1.17.0 선별 이식 완료

- 요청/범위: 공식 [expansion/1.17.0 릴리스](https://github.com/rh-hideout/pokeemerald-expansion/releases/tag/expansion/1.17.0)에서 PR #9878(알 생성·부화 재작업 및 `givemon` IsEgg), #10058(`GEN_CHAMPIONS`와 기술 데이터), #10561(메가찌르호크 아이콘), #10603(Z-A 메가 몸색), #10601(포켓덱스 플러스 진화 문구 정렬), #10416(메가솔 능력 팝업)을 현재 HNS 작업 트리에 선별 이식했다. 릴리스 태그 커밋은 `e8bd1cd`이다.
- 알 시스템: 부모 능력·성격·볼·기술·개체값·반짝임 보존 규칙과 부화 시 속성 보존을 적용했고, 현재 HNS의 파티/배틀 API에 맞춰 `gPlayerParty`·기존 이동 효과명·설정 매크로를 어댑트했다. `createmon`/`givemon` 스크립트의 26번째 옵션(0부터 세면 `IsEgg`)이 `MON_DATA_IS_EGG`에 반영된다.
- GEN_CHAMPIONS: `include/config/general.h`에서 `GEN_CHAMPIONS`를 `GEN_9 + 1`로 정의하고 `GEN_LATEST`를 `GEN_CHAMPIONS`로 설정했다. `GEN_COUNT`와 세대별 기술 조건을 갱신했으며, 현재 브랜치의 설정 구조에 맞춰 알 상속 설정 5개를 `include/constants/generational_changes.h`에 추가했다.
- Champions 특성: 새 ID `PIERCING_DRILL`, `DRAGONIZE`, `EELEVATE`, `MEGA_SOL`, `FIRE_MANE`, `SPICY_SPRAY`(중간 예약 슬롯 `ABILITY_314`, `ABILITY_317` 보존)를 추가했다. 종 매칭은 메가몰드류→관통드릴, 메가장크로다일→드래곤스킨, 메가저리더프→천정부지, 메가니움→메가솔라, 메가화염레오→불꽃의갈기, 메가스코빌런→하바네로분출이다. 표시명은 `src/data/abilities.h`에 한글로 반영했다. 능력 효과 본문은 공식 영문 설명을 유지했다.
- 그래픽/화면 코드: 메가찌르호크 아이콘은 공식 PNG를 HNS의 기존 `gbagfx` 경로로 4bpp 변환해 `graphics/pokemon/staraptor/mega/icon.4bpp`로 연결했다. 로컬 브랜치에는 `INCGFX_U8` 매크로가 없으므로 아이콘만 해당 등가 경로를 사용하며 앞·뒤 그림과 팔레트는 기존 HNS 압축 자산을 유지했다. Z-A 몸색, 도감 플러스 정렬, 메가솔 회복 팝업도 반영했다. HNS에 없는 애니메이션 테이블 6개는 단일 프레임 자리표시자로 호환했다.
- 추가 호환 수정: 현 HNS 코드가 참조하지만 선언이 빠진 `IsNuzlockeCaptureSuspended`를 사파리/곤충채집 플래그 조합으로 보완했고, 기존 배틀 회복 라벨이 없어 메가솔 스크립트를 `BattleScript_PresentHealTarget`에 연결했다. 종 설명의 문자표 비호환 이스케이프 큰따옴표는 ASCII 작은따옴표로 바꿨다.
- 백업/로그: 적용 전 선별 경로 백업은 `build/localization-backups/pre-selected-expansion-117-20260915.tar.gz`, 경로 목록과 패치는 `build/localization-port/`에 보관했다. 최종 빌드 로그는 `build/localization-logs/hns-selected-expansion-117-20260915-r20.log`이다.
- 검증: `make hns -j8` 종료 코드 0. 링크 메모리는 EWRAM 247,472/262,144 bytes(94.40%), IWRAM 25,704/32,768 bytes(78.44%), ROM 32,881,796/33,554,432 bytes(98.00%)이며 `pokehns.gba`와 `pokehns.elf`를 갱신했다. 주요 ID·종 매칭·`GEN_LATEST`·아이콘 참조를 정적으로 재확인했고, 실제 에뮬레이터/게임 화면 검증은 아직 하지 않았다.
- 다음 시작점: mGBA에서 알 부화 시 반짝임·성격·능력·볼 보존, `givemon` IsEgg, 여섯 메가 특성 팝업/효과, 메가찌르호크 아이콘, Z-A 메가 몸색, 도감 진화 문구 정렬을 실제 화면에서 확인한다. 빌드 산출물은 ROM 여유가 약 672KiB이므로 추가 데이터 변경마다 메모리 사용량을 재확인한다.

### 2026-09-15 — 1.17.0 Champions battle mechanics 및 Fixed 선별 이식

- 요청/범위: 공식 [expansion/1.17.0 릴리스](https://github.com/rh-hideout/pokeemerald-expansion/releases/tag/expansion/1.17.0)의 #10151(Champions 배틀 메커니즘 v1.0.x), #10025(상대 HP 백분율 표시), #10145(AI 계산 시간 개선), #10220(공격 전 효과·대미지 계산 취소자 이동), #9265(신규 테스트), #10257(Champions v1.1.0), #10324(메가/원시회귀 파티클 팔레트), #10426(흡수 리팩터링), #10591(Howl 대타출동 무시), #10541(효과 배율 아이콘)과 릴리스 `Fixed` 항목을 현재 HNS 구조에 맞춰 선별 반영했다. 릴리스 태그 커밋은 `e8bd1cd`다.
- 배틀 메커니즘: Champions 수면 턴 난수와 Yawn, Rage Fist 세대 설정, Make It Rain 특수공격 하락량, 교체 시 피격 카운터 초기화, Supreme Overlord AI 후보 저장·복원, Howl의 대타출동 무시, 극도로 효과적/대부분 효과 없음 배율 아이콘을 적용했다. AI 대미지 계산에서는 가중치·파트너·필드 특성 조회가 실전 배틀 상태를 다시 읽지 않고 `BattleContext`/AI 컨텍스트의 가상 값을 사용하도록 보강했다. 새 아이콘은 HNS 문자표의 `STAR`·`TRIANGLE_UPSIDE_DOWN`과 공식 갱신 라틴 폰트 PNG·폭 테이블을 사용한다.
- 상대 HP 표시: `PrintHpPercentageOnHealthbox`와 싱글·더블 상대 체력바 분기를 추가했지만 `include/config/battle.h`의 `B_HP_PERCENTAGE_DISPLAY`는 `FALSE`로 두었다. 따라서 현재 한글 HNS 체력바 배치와 기존 숫자 디버그 표시가 기본적으로 바뀌지 않으며, 호환되는 경우에만 설정을 `TRUE`로 바꿔 기능을 켤 수 있다.
- 그래픽: 메가진화·원시회귀 파티클에 흰색 팔레트 블렌드를 추가했다.
- Fixed 적용: #10581 세이브 재로드 후 기본 글꼴 초기화, #10597 도감 획득 전 스타터 이로치 제한, #10612 레벨업 중복 기술 처리, #10614 여러 레벨 상승 시 학습 레벨 처리를 반영했다. #10142·#10211 디버그 수정, #10523 파트너 생성기, #10527 FRLG 트레이너 카드, #10656 광범위한 스프라이트 안전성 변경은 현재 HNS에 대응하는 구조가 없거나 적용 범위를 벗어나 기존 코드를 보존했다.
- 이식하지 않은 항목: #9265는 테스트만 추가하는 PR이라 게임 코드에 복사하지 않았다. #10220의 최신 `CancelerPreAttackMoveEffect` 구조와 #10426의 흡수/`SetMoveEffect` 전면 리팩터링은 현재 HNS의 전투 실행 구조와 시그니처가 달라 기계적으로 덮어쓰지 않았다. HNS의 기존 효과 처리와 동작을 유지하면서 대응 가능한 Champions 동작만 적용했다.
- 한글화 보존: 이번 작업에서 기존 한글 종명·특성명·기술명·배틀 문구를 변경하지 않았다. 상대 HP 기능은 기본 비활성화이며, 새 효과 아이콘의 문자 토큰만 추가했다.
- 백업/로그: 앞선 1.17.0 선별 이식 백업·패치 기록은 `build/localization-backups/pre-selected-expansion-117-20260915.tar.gz`와 `build/localization-port/`에 있다. 이번 빌드 로그는 `build/localization-logs/hns-expansion-117-champions-mechanics-20260915.log`다.
- 검증: `make hns -j8` 종료 코드 0. 링크 메모리는 EWRAM 247,472/262,144 bytes(94.40%), IWRAM 25,704/32,768 bytes(78.44%), ROM 32,882,804/33,554,432 bytes(98.00%)이며 `pokehns.gba`·`pokehns.elf`를 갱신했다. 대상 변경 경로 `git diff --check`도 통과했다. 실제 mGBA·게임 화면 검증은 아직 하지 않았다.
- 다음 시작점: mGBA에서 수면 턴·Rage Fist·Howl 대타출동 상호작용, Supreme Overlord 교체 후 대미지, 배율 아이콘, 메가/원시회귀 파티클, Fixed 네 항목을 확인한다. HP 백분율을 켤 경우 HNS 64×32 체력바에서 숫자·퍼센트 폭을 별도로 점검하고 `make hns -j8` 메모리를 다시 기록한다.

### 2026-09-15 — 1.17.0 Items·Battle AI 선별 이식

- 요청/범위: 공식 [expansion/1.17.0 릴리스](https://github.com/rh-hideout/pokeemerald-expansion/releases/tag/expansion/1.17.0)의 `Items`와 `Battle AI` 섹션을 현재 HNS 작업 트리에 이식했다. 기존 한글 텍스트는 유지했다.
- Items: #10430의 희귀사탕·경험사탕 사용 후 필드 파티 메뉴 유지, #10163의 다섯 혼란 열매 hold effect 통합(`HOLD_EFFECT_CONFUSE_FLAVOR`와 flavor secondary ID), #10592의 에니그마베리 hold effect/아이템 효과 정리를 반영했다. #8893의 영문 문법 대개편은 한글 문구를 덮어쓸 위험이 있어 이식하지 않았다.
- Battle AI: #10243의 능력·도구·기술 지식 플래그 분리, #10258 반동 자폭 회피, #10236 Dragon Darts 양쪽 대상 계산, #8647 Round·Pledge·Fusion 연계와 AI 턴 순서, #10046 자기 자신 점수화 생략, #10145 AI 가상 능력·도구 컨텍스트, #10427 Wrap 잔여 피해, #10277 사고 시간·명중률, #10453 `AiCalcValues`, #10461 선택 결과·대상 정리, #10193 명중률 구조체 API, #10212 `gCurrentMove` 인자화, #9448 Protect 파트너 설정 회피, #10626 Palafin Zero 교대, #10669 Mind Reader/Lock-On, #10688 Fusion 턴 순서, #10700 확정 치명타 단계, #10610의 HNS 대응 필드·Stomping Tantrum 수정을 적용했다. #10464는 현재 HNS에 구형 `MOVE_EFFECT_STAT_PLUS` 경로가 없으므로 동일한 통계 점수 수정 대신 이미 존재하는 최신 점수 로직과 Sturdy 설정 조회만 보존·갱신했다.
- HNS 호환 결정: 최신 소스의 `BattleSideHasTwoTrainers`, `BattlerJustSwitchedIn`, `CONFIG_B_MULTI_BATTLE_WHITEOUT`는 HNS에 없어서 기존 배틀 타입 플래그, `battlerState.isFirstTurn`, `B_MULTI_BATTLE_WHITEOUT`로 의미를 맞췄다. AI `fieldStatuses`는 HNS `BattleContext` 구조를 유지했다.
- 수정 경로: `src/party_menu.c`, `src/item.c`, `src/pokemon.c`, `src/battle_script_commands.c`, `src/data/hold_effects.h`, `src/data/items.h`, `include/constants/hold_effects.h`, `src/battle_hold_effects.c`, `src/battle_debug.c`, `include/constants/battle_ai.h`, `include/battle.h`, `include/battle_ai_util.h`, `include/config/ai.h`, `include/random.h`, `src/battle_ai_main.c`, `src/battle_ai_switch.c`, `src/battle_ai_util.c`, `src/battle_main.c`, `src/battle_move_resolution.c`, `include/battle_main.h`, `include/battle_util.h`, `src/battle_util.c` 등이다.
- 검증: `make hns -j8` 종료 코드 0. 링크 메모리는 EWRAM 247,472/262,144 bytes(94.40%), IWRAM 25,704/32,768 bytes(78.44%), ROM 32,884,516/33,554,432 bytes(98.00%)이며 로그는 `build/localization-logs/hns-expansion-117-items-ai-20260915.log`다. 대상 코드·데이터 `git diff --check`는 통과했다.
- 게임 화면 확인: mGBA/실기 검증은 아직 하지 않았다. 특히 더블 배틀의 Round·Pledge·Fusion·Dragon Darts·Protect와 반동/Palafin 교대 AI를 확인해야 한다.
- 다음 시작점: 새 ROM으로 위 더블 배틀 AI 조합과 혼란 열매, 에니그마베리, 캔디 사용 후 메뉴 복귀를 실제 게임에서 재현한다.
### 2026-09-16 — 디버그 다단계 선택의 B 버튼 동작 원인 확인

- 사용자 문의: 1.17.0에서 추가된 디버그 메뉴 다단계 선택의 중간 뒤로 가기가 현재 동작하지 않는 이유 조사.
- `expansion/1.17.0`의 PR #10121(`9df1444765521683907f19757155d3d2d3668ae0`)은 `DebugAction_Selection_StepUpdate()`의 공통 B 처리(`tStep`/`tSubstep`/`tStepsDataIndex` 감소)를 사용하지만, 현재 HNS `src/debug.c`에는 해당 구조체·함수·태스크 필드가 없다. 현재 트리는 기존 단계별 함수에 선택적으로 B 전환을 수동 포트한 상태다.
- 구체적으로 `Give X → Pokémon (Basic)` 레벨 단계는 B에서 `DebugAction_DestroyExtraWindow()`를 호출해 종료하고(`src/debug.c:3159-3179`), 워프 맵 그룹 첫 단계도 동일하게 종료한다(`src/debug.c:1548-1551`). 복합 포켓몬과 일부 아이템/워프 중간 단계는 별도 분기로 이전 단계로 돌아간다.
- `HEAD`와 `expansion/1.17.0`은 `3efb836f72`에서 분기되어 1.17.0 태그가 현재 HNS에 자동 병합되지 않은 것도 원인이다. 실제 mGBA 실행 검증은 하지 않았으며 코드·ROM은 변경하지 않았다.
- 다음 작업: 완전한 공통 선택기 재이식 또는 메뉴별 누락 B 분기 보완 후 `make hns -j8`, 기본/복합 포켓몬·아이템·워프 각 단계에서 B 동작을 실제 확인한다.

### 2026-09-16 — 디버그 메뉴 B 버튼 이전 단계 복귀 구현

- 요청: 디버그 메뉴의 다단계 선택에서 B 버튼을 누르면 이전 단계로 돌아가도록 수정.
- 수정: `src/debug.c`에 부모 메뉴 복귀 헬퍼를 추가하고, 포켓몬(기본/복합)·아이템·워프의 단계별 B 분기에서 직전 화면을 복원하도록 보완했다. 첫 단계의 B는 `Give X` 또는 `Utilities`로 돌아가며, 플래그·변수(`listId=1`)·날씨·장식·사운드 입력창도 부모 메뉴 복귀를 사용한다. 복귀 시 추가 창·기존 리스트 태스크·콜백 스택을 정리하고, 복합 포켓몬 아이콘은 완료/취소 시점까지 유지한다.
- 함께 확인: 장식 추가 시 아이템 아이콘 정리 함수를 호출하던 기존 오타를 장식 아이콘 정리 함수로 바로잡았다. 기존 한글 데이터와 기타 작업 트리 변경은 건드리지 않았다.
- 검증: `make hns -j8` 종료 코드 0. EWRAM 249,012/262,144 bytes(94.99%), IWRAM 25,704/32,768 bytes(78.44%), ROM 33,324,612/33,554,432 bytes(99.32%). 빌드 로그는 `build/localization-logs/hns-debug-b-button-20260916.log`에 저장했다. `git diff --check -- src/debug.c` 통과.
- 실제 게임 검증: 이 환경에는 mGBA 실행 파일이 없어 버튼 입력 화면은 확인하지 못했다. 다음 세션에서는 새 `pokehns.gba`로 기본/복합 포켓몬, 아이템 수량, 워프(그룹→맵→워프), 플래그·변수 각 단계에서 B 복귀와 메뉴 중복/잔상 여부를 확인한다.

### 2026-09-16 — 특성 팝업 영어 소유격 제거

- 요청: 특성 팝업에서 포켓몬 이름 뒤에 표시되는 영어 `'s`를 한국어 소유격으로 변경.
- 수정: `src/battle_interface.c`의 `PrintBattlerOnAbilityPopUp()`에서 닉네임 뒤에 붙이던 `'` 및 조건부 `s`를 제거하고 `의`를 직접 추가했다. 닉네임 길이·마지막 글자 검사 변수와 반복문도 삭제했다. 특성명 데이터(`src/data/abilities.h`)와 일반 배틀 메시지 테이블은 변경하지 않았다.
- 검증: `make hns -j8` 종료 코드 0. EWRAM 249,012/262,144 bytes(94.99%), IWRAM 25,704/32,768 bytes(78.44%), ROM 33,324,564/33,554,432 bytes(99.31%). 빌드 로그는 `build/localization-logs/hns-ability-popup-possessive-20260916.log`에 저장했다.
- 실제 게임 검증: mGBA 실행 파일이 없어 특성 팝업 화면은 확인하지 못했다. 다음 세션에서 야생·트레이너 배틀의 아군/상대 특성 팝업에 `포켓몬의` 형태가 올바르게 표시되는지 확인한다.
### 2026-09-17 — 포켓몬 요약 화면 표제어 코드·그래픽 위치 조사

- 요청/범위: 요약(능력치) 화면의 `PROFILE`, `ABILITY`, `TRAINER INFO` 등 고정 표제어를 번역할 경로와 C 코드 위치 확인.
- 확인: 해당 고정 영어 표제어는 HNS 요약 화면 타일 시트 `graphics/summary_screen/hns/tiles.png`(128×120, 8-bit colormap)에 픽셀 그래픽으로 들어 있으며, `src/graphics.c:1809-1810`에서 4bpp 타일과 팔레트를 로드한다. 정보 화면 배치는 `graphics/summary_screen/page_info.bin` 타일맵을 `src/pokemon_summary_screen.c:1563`에서 사용한다.
- C 문자열 경로: 페이지 제목·능력치/상태/기술 라벨은 `src/pokemon_summary_screen.c:3512-3544`의 `PrintPageNamesAndStats()`가 출력하고, 페이지 제목 정의는 `src/strings.c:440-443`이다. 현재 `PROFILE`처럼 이미지에 박힌 표제어와 `gText_PkmnInfo` 등의 C 출력 문자열은 구분해야 한다.
- 동적 특성 데이터: 요약 정보 페이지는 `src/pokemon_summary_screen.c:3695-3711`에서 어버이·ID·특성명·특성 설명·트레이너 메모를 호출한다. 특성 이름/설명은 `src/data/abilities.h`의 `gAbilitiesInfo[].name` 및 `.description`에서 온다.
- 수정 파일: 인수인계 문서 `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`만 갱신. 소스·그래픽·ROM은 변경하지 않았다.
- 검증: 경로 및 호출부 정적 확인. 문서만 변경했으므로 HNS 빌드는 실행하지 않았다.
- 게임 화면 확인: 수행하지 않음(위치 안내 요청).
- 남은 문제: 실제 PNG 번역 시 타일별 글자 경계·인덱스 팔레트를 보존하고, 페이지 레이아웃을 바꾸지 않는다면 타일맵은 그대로 둔다.
- 다음 시작점: 고정 라벨 번역은 `graphics/summary_screen/hns/tiles.png`; C 기반 문구는 `src/strings.c` 및 `src/pokemon_summary_screen.c`; 특성명/설명은 `src/data/abilities.h`.

### 2026-09-17 — 요약 화면 `어딘가` 색상 적용

- 요청/범위: 요약 화면의 `gText_XNatureMetSomewhereAt`, `gText_XNatureHatchedSomewhereAt`에서 `어딘가`를 구체적인 장소명과 동일한 빨간 글씨·그림자 색으로 표시.
- 수정 파일: `src/strings.c`; 두 문자열의 `어딘가` 앞뒤에 `{DYNAMIC 0}` 및 `{DYNAMIC 1}` 색상 제어 코드를 넣었다. `{DYNAMIC 0}`은 `sMemoNatureTextColor`(LIGHT_RED/GREEN shadow), `{DYNAMIC 1}`은 일반 문구 색상으로 복귀한다. 나머지 문구·줄바꿈은 변경하지 않았다.
- 검증: `git diff --check -- src/strings.c` 통과. `make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,420/33,554,432(99.31%). `pokehns.gba` 33,554,432바이트, SHA-256 `140581139f1097d2c28168b3822e417f283c1052b788b0ace9b88f914052f7c2`. 빌드 로그 `build/localization-logs/hns-summary-unknown-location-color-20260917.log`.
- 게임 화면 확인: 실제 mGBA 화면은 미확인. 해당 문구에서 `어딘가`와 실제 장소명의 색상·그림자 일치 여부를 새 ROM으로 확인한다.
- 남은 문제: 없음(코드·빌드 기준).
- 다음 시작점: `pokehns.gba`에서 요약 화면의 미상 만난 장소 및 미상 부화 장소 문구를 띄워 시각 검증한다.

### 2026-09-23 — `STRINGID_PKMNSXMADEITINEFFECTIVE`·`STRINGID_PKMNSXPREVENTSFLINCHING` 출력 조건 및 특성 팝업 확인

- 요청/범위: 두 문자열이 실제로 선택되는 배틀 경로와 특성 팝업 출력 여부를 확인했다.
- 수정 파일: `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`만 갱신했다. 소스·게임 데이터·ROM은 수정하지 않았다.
- 정정한 확인 결과: `STRINGID_PKMNSXMADEITINEFFECTIVE`는 `의기양양`·`불면`의 수면 방지와 상대 특성 `그림자밟기`·접지 대상의 `개미지옥`·강철 대상의 `자력`에 의한 텔레포트 도주 방지에서만 사용된다. 점착의 탁쳐서떨구기·트릭·바꿔치기·부식가스·도둑질·탐내다 도구 조작 방지는 이름이 비슷한 `STRINGID_PKMNSXMADEYINEFFECTIVE`를 사용한다. `STRINGID_PKMNSXPREVENTSFLINCHING`은 `정신력`이 `EFFECT_PRIMARY` 또는 `EFFECT_CERTAIN` 플린치를 막을 때만 사용되며, 현재 `속이기`가 확정 플린치 사례다.
- 팝업 결과: 수면·텔레포트 경로는 `BattleScript_AbilityPopUp`을 호출하므로 특성 팝업 후 문장이 출력된다. 단, 텔레포트 경로는 `gBattleScripting.battler`만 설정하고 `gBattlerAbility`를 설정하지 않아 팝업 대상이 이전 전역값에 의존할 수 있다. 점착 경로의 팝업은 별도 `...MADEYINEFFECTIVE` ID에 해당한다.
- 검증: `rg`, `nl`, `sed`로 `data/battle_scripts_1.s`, `src/battle_main.c`, `src/battle_util.c`, `src/battle_script_commands.c`, `src/battle_move_resolution.c`, `src/data/moves_info.h`의 호출부·조건·팝업 구현을 정적 확인했다. 문서만 수정했으므로 HNS 빌드는 하지 않았다.
- 게임 화면 확인: mGBA 실행 파일 부재로 두 팝업과 문장 순서는 미확인.
- 남은 문제: 텔레포트의 팝업 대상 전역값과 점착 경로의 `B_SCR` 치환 대상은 수정 여부를 별도로 결정해야 한다.
- 다음 시작점: HNS ROM에서 각 특성 경로를 재현해 실제 표시를 확인하거나, 대상 대입을 보완하기로 결정하면 관련 배틀 스크립트·C 코드를 수정하고 `make hns -j8`로 빌드한다.

### 2026-09-17 — HNS 요약 화면 타일 시트 재변환

- 요청/범위: 사용자가 다시 수정한 HNS 요약 화면 `tiles.png`를 게임용 그래픽으로 변환하고 적용 여부를 확인.
- 수정 파일/산출물: 원본 `graphics/summary_screen/hns/tiles.png`는 편집하거나 덮어쓰지 않았다. HNS 빌드가 `tiles.4bpp`, `tiles.gbapal`, `tiles.4bpp.smol`을 재생성해 ROM에 포함했다.
- 검증: 원본은 128×120, indexed 8-bit PNG(2668바이트)다. `gbagfx`로 임시 경로에서 별도 재변환한 `.4bpp`와 `.gbapal`이 실제 산출물과 각각 바이트 단위 일치했다. `git diff --check -- graphics/summary_screen/hns/tiles.png` 통과. `make --jobserver-style=pipe hns -j8` 종료 코드 0; EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,436/33,554,432(99.31%). `pokehns.gba` 33,554,432바이트, SHA-256 `9ff4d8a32ce862512dc5e09fe9ffad8b6d66649d04ba846ee43f2d5901409472`. 로그 `build/localization-logs/hns-summary-screen-tiles-reconvert-20260917.log`.
- 게임 화면 확인: 실제 게임에서 새 ROM 화면을 띄우지는 않았다. 소스 PNG를 확인했고 변환 산출물의 재현성을 확인했다.
- 남은 문제: 런타임 화면 점검만 남아 있다.
- 다음 시작점: 새 `pokehns.gba`의 요약 화면에서 타일 글자 잘림·깨짐과 색상/팔레트가 의도와 맞는지 확인한다.

### 2026-09-17 — 요약 화면 L/R 타일 수정 시도 (오판, 아래 롤백 기록 참조)

- 요청/범위: 요약 화면 개체값·노력치 안내 옆 L/R 상자 가장자리 잘림 조사.
- 잘못된 수정: 타일맵이 참조하는 219–220·235–236을 프레임만 있는 셀로 오인해 이전 HNS 이미지의 픽셀로 덮었다. 실제 셀에는 글자 획도 포함되어 있어 화면에서 글자까지 잘리는 회귀가 발생했다.
- 이 시도에서 생성된 ROM은 SHA-256 `1c423f960fbc391e3ebdb2a9672371daa4559052f5c89267b54f1aaf6bff9ed5`; 사용자가 화면 이상을 보고해 바로 아래 기록과 같이 수정 픽셀을 롤백하고 재빌드했다. 이 ROM은 폐기 대상이다.
- 원래 L/R 일부 잘림의 근본 원인은 이 조사로 확정되지 않았다.

### 2026-09-17 — L/R 타일 오수정 롤백 및 ROM 복구

- 사용자 확인 후 직전 시도가 원인이었음을 인정했다. HNS `tiles.png`를 수정 전 바이트 해시 `e37b7aa80f2171d8cc79731b01da7d7437ebfd8457308aa1c2d15b496b33bd4b`와 일치하는 이미지로 복원했다. 이는 직전 편집 상태와 동일한 파일이며 128×120 indexed PNG, 2668바이트다. 타일맵이나 C 코드는 건드리지 않았다.
- `make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,436/33,554,432(99.31%). 로그 `build/localization-logs/hns-summary-iv-ev-lr-revert-20260917.log`; 복구 ROM SHA-256 `9ff4d8a32ce862512dc5e09fe9ffad8b6d66649d04ba846ee43f2d5901409472` (33,554,432바이트).
- 게임 화면 확인: 에이전트 환경에 mGBA 실행 파일이 없어 복구본 런타임 화면은 미확인. 사용자가 글자 잘림을 보고한 ROM은 롤백한 ROM이며, 복구 ROM에서 먼저 확인할 부분은 글자 복구 여부다.
- 남은 문제: 원래 보고된 L/R 상자 일부 잘림 자체는 미해결. 정확한 픽셀/타일 경계와 화면상의 잘림 방향을 확인하기 전 타일 데이터를 더 수정하지 않는다.
- 다음 시작점: 복구 ROM의 능력 페이지 캡처와 수정 전 참조 이미지를 나란히 보고 잘린 방향(좌측/윗줄) 및 타일 ID를 확정한 뒤, 필요한 픽셀만 별도 검증하고 변경한다.

### 2026-09-17 — L/R 상자 왼쪽 1픽셀 복원 범위 확인

- 사용자가 전체 mGBA 캡처를 제공하고 수정 기준을 재확인했다. `노력치`·`개체값` 글자 위치와 L/R 글자·상자 위치는 그대로 두고, 상자 왼쪽에서 잘린 세로 1픽셀 열만 복구한다. 화면 전체 이동이나 상자/문자 재배치는 원하지 않는다.
- `page_skills.bin` 대조 결과 타일 235·236은 헤더 행 (x=19,20, y=6), 타일 219·220은 (x=24,25, y=6)에 놓인다. 이미지 셀에 한글 픽셀이 섞여 있으므로 타일 전체를 영문 기준본으로 교체하지 않는다. 화면 위치와 왼쪽 테두리 픽셀의 대응은 다음 단계에서 검증한다.
- 이번 회차에는 소스/그래픽/ROM을 수정하지 않았고 빌드도 하지 않았다. 다음 단계는 해당 화면 좌표와 원본 셀의 픽셀 경계를 확정한 뒤 가장자리 열만 수정하고, HNS 빌드 및 글자 보존을 확인하는 것이다.

### 2026-09-22 — 오로라베일 성공 문구를 물리·특수공격 동시 상승 메시지로 연결 및 HNS ROM 재빌드
- 요청/범위: 오로라베일 성공 시 `STRINGID_PKMNCOVEREDBYVEIL` 대신 새 `STRINGID_PKMNRAISEDDEFSPDEF`를 사용하고, `우리 편/상대는 오로라베일로 물리공격과 특수공격에 강해졌다!` 형태로 확장되는지 확인.
- 수정 파일: `include/constants/battle_string_ids.h`, `src/battle_message.c`, `src/battle_script_commands.c`. 상태·세션 기록도 갱신했다. 기존 `STRINGID_PKMNCOVEREDBYVEIL` 문구와 신비의부적 경로는 유지했다.
- 변경: 새 문자열 ID와 `B_MSG_SET_AURORA_VEIL`을 등록했다. `gReflectLightScreenSafeguardStringIds[B_MSG_SET_AURORA_VEIL]`은 `STRINGID_PKMNRAISEDDEFSPDEF`를 가리키며, 직접 오로라베일 경로의 `BS_SetAuroraVeil()`과 `MOVE_EFFECT_AURORA_VEIL` 추가 효과 경로가 이 선택값을 사용한다. 신비의부적은 계속 `B_MSG_SET_SAFEGUARD`→`STRINGID_PKMNCOVEREDBYVEIL`이다.
- 문구 확인: `{B_ATK_PREFIX2}`는 `우리 편은/상대는`, `{B_CURRENT_MOVE}`는 직접 사용한 `오로라베일`, `{B_TXT_EU}`는 ㄹ 받침인 `일` 뒤에서 `으`를 생략하는 `로`로 확장된다. 실제 줄바꿈 포함 결과는 `우리 편은 오로라베일로\n물리공격과 특수공격에 강해졌다!` 또는 `상대는 오로라베일로\n물리공격과 특수공격에 강해졌다!`다.
- 검증: 관련 문자열·테이블·두 오로라베일 경로의 정적 참조 확인 및 `git diff --check` 통과. 헤더 변경으로 대상 오브젝트 명령은 180초 제한에 걸렸지만 컴파일 오류는 없었고, 전체 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144, IWRAM 25,704/32,768, ROM 33,328,676/33,554,432. GBA SHA-256 `5b848b2b78bc90667fafafb7456e4e4726bddf65a22d5d7d90a0beb7531e88bd`, ELF SHA-256 `f5a2a74d2e65e03965d113b1d8d017065160e8153e6ba19a3918d7f4f0d8344e`; 로그 `build/localization-logs/hns-aurora-veil-stat-message-20260922.log`.
- 게임 화면 확인: mGBA 실행 파일이 없어 실제 오로라베일·신비의부적 메시지와 줄바꿈은 미확인.
- 남은 문제: 새 ROM에서 오로라베일과 신비의부적 성공 메시지를 각각 실행해 실제 화면을 확인할 것.
- 다음 시작점: `pokehns.gba`에서 오로라베일 성공 및 중복 사용, 신비의부적 성공을 재현한다.

### 2026-09-22 — `STRINGID_PKMNREDUCEDPP` 출력 조건과 버퍼 확인
- 요청/범위: `{B_DEF_NAME_WITH_PREFIX}`, `{B_BUFF1}`, `{B_TXT_EULREUL}`, `{B_BUFF2}`가 실제로 무엇을 표시하는지와 해당 문자열의 출력 조건 확인.
- 확인: `STRINGID_PKMNREDUCEDPP`는 `원한`의 `BattleScript_EffectSpite`, `섬뜩한주문`의 `BattleScript_MoveEffectEerieSpell`, `거다이감쇠`의 `BattleScript_EffectTryReducePP`에서 PP 감소 성공 뒤 출력된다. 공통 감소 함수는 대상의 `gLastMoves[gBattlerTarget]`를 찾아 `gBattleTextBuff1`에 기술명을 넣고, `gBattleTextBuff2`에 실제 감소량을 넣는다.
- 버퍼 확장: 대상 이름은 야생/상대 접두사가 포함된 `{B_DEF_NAME_WITH_PREFIX}`이고, `{B_TXT_EULREUL}`은 기술명 받침에 따라 을/를을 선택한다. 현재 설정에서 원한은 4, 섬뜩한주문은 3, 거다이감쇠는 2 PP를 기본 감소시키며 남은 PP가 적으면 그 수치로 제한된다.
- 결과 예시: 상대 팬텀의 마지막 기술이 섀도볼이면 원한은 `상대 팬텀의\n섀도볼을 4 깎았다!`, 섬뜩한주문은 `상대 팬텀의\n섀도볼을 3 깎았다!`가 된다. PP 단위는 문자열에 별도로 붙지 않는다. 실패 조건에서는 이 ID가 출력되지 않는다.
- 검증: `rg`로 문자열 호출부·PP 감소 코드·현재 설정값을 확인하고 관련 C/배틀 스크립트 구간을 `sed`로 확인했다. 소스·ROM 수정과 빌드는 하지 않았다.
- 게임 화면 확인: mGBA 실행 파일이 없어 실제 줄바꿈·조사·야생/상대 접두사는 미확인.
- 남은 문제: 새 ROM에서 세 기술의 PP 감소 성공 분기를 각각 재현할 것.
- 다음 시작점: `pokehns.gba`에서 원한, 섬뜩한주문, 거다이감쇠를 대상의 마지막 기술·PP가 남은 상태에서 실행한다.

### 2026-09-22 — 오로라베일 발동 메시지 재확인
- 요청/범위: 빛의장막·리플렉터가 각각 `STRINGID_PKMNRAISEDSPDEF`·`STRINGID_PKMNRAISEDDEF`를 출력하는 것과 비교해, 오로라베일 성공 시 출력되는 문자열 ID와 실제 문구를 확인.
- 수정 파일: `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`만 갱신했다. 소스·데이터·ROM은 수정하지 않았다.
- 확인: `src/battle_script_commands.c`의 `BS_SetAuroraVeil()`과 `MOVE_EFFECT_AURORA_VEIL` 경로가 `B_MSG_SET_SAFEGUARD`를 선택하고, `data/battle_scripts_1.s`의 `BattleScript_EffectAuroraVeil`/`BattleScript_MoveEffectAuroraVeil`이 `gReflectLightScreenSafeguardStringIds`를 출력한다. 테이블의 해당 항목은 `STRINGID_PKMNCOVEREDBYVEIL`이다.
- 결과: 성공 시 `{B_ATK_PREFIX2}`에 따라 `우리 편은\n신비의 베일에 둘러싸였다!` 또는 `상대는\n신비의 베일에 둘러싸였다!`가 출력된다. `오로라베일`이라는 기술명은 성공 문구에 직접 들어가지 않는다. 이미 설치된 상태에서 재사용하면 `B_MSG_SIDE_STATUS_FAILED`→`STRINGID_BUTITFAILED`다.
- 검증: `rg`와 관련 C/어셈블리 구간 `sed` 확인. 두 문서가 Git 미추적 상태라 `git diff --no-index --check /dev/null <문서>`를 사용했으며 공백 진단은 없었다(종료 코드 1은 `/dev/null`과의 차이 때문에 발생). 문서만 수정했으므로 HNS 빌드는 실행하지 않았다.
- 게임 화면 확인: mGBA 실행 파일이 없어 성공·중복 사용의 실제 화면과 글리프는 미확인.
- 남은 문제: 새 ROM에서 오로라베일 성공 및 중복 사용 시 줄바꿈·글리프를 확인할 것.
- 다음 시작점: `pokehns.gba`에서 오로라베일을 성공시키고, 같은 편에 재사용해 각각 두 메시지를 확인한다.

### 2026-09-22 — `STRINGID_PKMNATTACK`의 HNS 출력 조건 확인
- 요청/범위: `src/battle_message.c`의 `{B_BUFF1}의 공격!` 문자열이 언제 출력되는지와 HNS에서 실제 출력 가능한지 확인.
- 확인: 직접 호출부는 `data/battle_scripts_1.s:3451-3453`의 `BattleScript_BeatUpAttackMessage`뿐이다. `src/battle_script_commands.c:3683-3700`은 집단구타 참가 포켓몬의 이름과 접두사를 `{B_BUFF1}`에 준비한 뒤 이 스크립트를 호출한다.
- 결과: HNS `include/config/battle.h:123`의 `B_BEAT_UP`은 `GEN_LATEST`이고, `MOVE_EFFECT_BEAT_UP_MESSAGE`는 `GetConfig(B_BEAT_UP) >= GEN_5`에서 즉시 종료한다. 따라서 현재 HNS 집단구타에서는 `STRINGID_PKMNATTACK`가 출력되지 않는다. 이 설정을 GEN_4 이하로 낮춘 경우에만 `피카츄의 공격!`처럼 각 참가 포켓몬의 이름을 넣어 출력된다.
- 검증: 관련 문자열 정의, 유일한 배틀 스크립트 호출부, 버퍼 준비 코드, `B_BEAT_UP` 조건을 `rg`·`sed`로 정적 확인했다. 소스·데이터·ROM 수정 및 HNS 빌드는 하지 않았다.
- 게임 화면 확인: mGBA 실행 파일이 없어 화면 확인은 수행하지 않았다.
- 남은 문제: 없음. 세대별 집단구타 동작을 변경할 경우 `B_BEAT_UP` 설정 변경 여부를 먼저 결정한다.

### 2026-09-22 — `//not in gen 5+` 주석 문자열의 `GEN_LATEST` 차단 여부 재검증
- 요청/범위: `src/battle_message.c`에서 `//not in gen 5+` 주석이 붙은 문자열 중 `GEN_LATEST` 설정 때문에 HNS에서 출력되지 않는 항목이 더 있는지 확인.
- 정정: 이전 답변에서 해당 주석을 단순 설명으로만 취급한 것은 오류였다. `STRINGID_PKMNATTACK`은 `B_BEAT_UP = GEN_LATEST`일 때 `src/battle_script_commands.c:3684`에서 `MOVE_EFFECT_BEAT_UP_MESSAGE`가 즉시 종료되므로 HNS에서 출력되지 않는다.
- 대조 결과: 나머지 주석 문자열은 `GEN_LATEST`가 호출을 직접 차단하지 않는다. 능력 발동·상태이상 문자열은 `gGot*StringIds`·`gAbilityWeatherChangeStringId` 등의 테이블과 배틀 스크립트에서 사용되고, `PKMNGOTFREE`·`PKMNSHEDLEECHSEED`·`PKMNBLEWAWAYSPIKES`·날씨 지속·선택 화면·도주·방벽 파괴 일반 문구도 현재 경로가 남아 있다.
- 별도 미사용 상태: `PKMNWENTTOSLEEP`은 현재 Rest 테이블이 `PKMNSLEPTHEALTHY`로 통일되어, `PKMNSTAYEDAWAKEUSING`은 불면 방지 스크립트가 `ITDOESNTAFFECT`를 사용하도록 바뀌어 출력되지 않는다. 둘 다 `GEN_LATEST` 분기 때문은 아니다.
- 검증: `src/battle_message.c`의 해당 주석 목록과 모든 ID 참조를 `rg`로 검색하고, `B_BEAT_UP`, 방벽 파괴, 날씨·상태·Rest 관련 테이블 및 스크립트의 실제 호출을 `sed`로 확인했다. 소스·데이터·ROM 수정 및 HNS 빌드는 하지 않았다.
- 게임 화면 확인: mGBA 실행 파일이 없어 실제 화면 검증은 수행하지 않았다.
- 다음 시작점: 주석이 붙은 문자열을 정리할 때는 주석이 아니라 실제 호출부와 세대 분기를 기준으로 유지·교체 여부를 결정한다.

### 2026-09-22 — `include/config/battle.h` 세대 설정값 범위 확인
- 요청/범위: `GEN_3`와 `GEN_LATEST` 외에 `battle.h`에서 사용할 수 있는 세대 설정값 확인.
- 확인: `include/config/general.h:62-74`에 `GEN_1`~`GEN_9`, `GEN_CHAMPIONS`, `GEN_COUNT`, `GEN_LATEST`가 정의되어 있다. `GEN_LATEST`는 `GEN_CHAMPIONS`와 같고 `GEN_9`보다 한 단계 큰 값이다.
- 결과: 세대형 `B_*` 설정에는 `GEN_1`~`GEN_9`를 사용할 수 있으며, 코드는 대체로 `GetConfig(setting) >= GEN_X`로 기준 세대 이상을 선택한다. `GEN_6_XY`, `GEN_6_ORAS`, `GEN_8_PLA`는 특수 오버월드 설정용이므로 일반 배틀 설정에 임의로 쓰지 않는다. 일부 `B_*`는 세대값 대신 `TRUE`·`FALSE`·수치 상수를 받는다.
- 예시: `B_BEAT_UP`을 `GEN_4`로 바꾸면 `GEN_5` 이상 조건이 거짓이어서 구세대 집단구타 추가 메시지 경로가 살아나고, `GEN_5` 이상이면 최신 집단구타 동작이 선택된다.
- 검증: 세대 상수 정의, `battle.h` 설정값, `B_BEAT_UP` 비교부를 `rg`·`sed`로 확인했다. 소스·데이터·ROM 수정 및 HNS 빌드는 하지 않았다.
- 다음 시작점: 특정 `B_*` 값을 조정하기 전 그 설정의 비교 연산이 `>=`, `==`, `<` 중 무엇인지 확인한다.

### 2026-09-22 — HNS 지형 배경 자산과 `B_NEW_TERRAIN_BACKGROUNDS` 확인
- 요청/범위: `B_NEW_TERRAIN_BACKGROUNDS = FALSE`인 현재 HNS에 지형별 배틀 배경이 존재·사용되는지 확인.
- 확인: `B_TERRAIN_BG_CHANGE`는 TRUE이며 `src/battle_bg.c:1427-1445`가 전기·그래스·미스트·사이킥 지형에 각각 `BG_*_TERRAIN`을 선택한다. `src/data/battle_anim.h:1485-1488`의 배경 테이블이 해당 이미지·팔레트·타일맵을 연결한다.
- 결과: `src/graphics.c:1613-1645`의 조건부 포함에서 FALSE는 `electric_terrain`, `grassy_terrain`, `misty_terrain`, `psychic_terrain`을 사용하고, TRUE일 때만 `new_*_terrain`을 사용한다. 두 세트의 원본 그래픽은 모두 존재하지만 현재 HNS는 기존 네 배경을 사용하며 새 네 배경은 기본 빌드에 연결되지 않는다.
- 검증: 설정, 조건부 `INCBIN`, 지형 배경 선택부, 배경 테이블 및 원본 자산을 `rg`·`sed`·`stat`로 확인했다. 소스·그래픽·ROM 수정 및 HNS 빌드는 하지 않았다.
- 게임 화면 확인: mGBA 실행 파일이 없어 네 지형의 실제 화면은 확인하지 않았다.
- 다음 시작점: 새 배경을 적용할 경우 `B_NEW_TERRAIN_BACKGROUNDS`를 TRUE로 바꾸고 HNS 빌드 후 네 지형의 색상·타일맵·화면 전환을 확인한다.

### 2026-09-22 — `B_PREFERRED_ICE_WEATHER` 가능한 값과 실제 매핑 확인
- `include/config/battle.h`의 가능한 값은 `B_ICE_WEATHER_BOTH = 0`, `B_ICE_WEATHER_HAIL = 1`, `B_ICE_WEATHER_SNOW = 2` 세 가지이며 현재 HNS 기본값은 `B_ICE_WEATHER_BOTH`다.
- `B_ICE_WEATHER_BOTH`에서는 `싸라기눈`이 우박, `설경`·`썰렁개그`가 눈으로 동작하고, 오로라베일 설명은 우박·눈 양쪽을 안내한다. `B_ICE_WEATHER_HAIL`에서는 `설경`·`썰렁개그`가 우박으로, `B_ICE_WEATHER_SNOW`에서는 `싸라기눈`과 `MOVE_EFFECT_HAIL`까지 눈으로 매핑된다.
- `차가운바위`와 기술머신07의 설명도 설정값에 따라 바뀐다. 실제 오로라베일 사용 판정은 `src/battle_move_resolution.c:1119-1122`의 `B_WEATHER_ICY_ANY` 검사라서, 현재 세 설정 모두 우박 또는 눈에서 사용할 수 있다. 설정값만으로 사용 가능 날씨가 한쪽으로 제한되지는 않는다.
- 검증: 설정 상수·전체 참조부·기술/아이템 설명·날씨 효과·오로라베일 사용 판정을 `rg`·`sed`로 확인했다. 소스·데이터·ROM 수정 및 HNS 빌드는 하지 않았다. 다음 시작점은 설정을 변경할 때 HNS 빌드 후 관련 기술, 오로라베일, 기술머신07, 차가운바위의 효과와 문구를 실제 게임에서 확인하는 것이다.

### 2026-09-22 — `B_SHOW_TYPES` 가능한 값과 타입 아이콘 표시 조건 확인
- `include/config/battle.h:412-416`의 가능한 값은 `SHOW_TYPES_NEVER = 0`, `SHOW_TYPES_ALWAYS = 1`, `SHOW_TYPES_CAUGHT = 2`, `SHOW_TYPES_SEEN = 3` 네 가지다. 현재 HNS의 `B_SHOW_TYPES`는 `SHOW_TYPES_NEVER`다.
- `SHOW_TYPES_NEVER`는 타입 아이콘을 표시하지 않고, `SHOW_TYPES_ALWAYS`는 살아 있는 배틀 포켓몬의 타입을 항상 표시한다. `SHOW_TYPES_CAUGHT`는 포획 기록이 없는 종을 `TYPE_MYSTERY`로, `SHOW_TYPES_SEEN`은 종별 판정에서 목격 기록이 없는 종을 `TYPE_MYSTERY`로 표시한다. 단, `SHOW_TYPES_SEEN`은 아이콘 로딩 기준 포켓몬이 미발견이면 `LoadTypeIcons()`가 전체 로딩을 먼저 중단한다.
- 타입 아이콘은 기술 선택 초기화의 `LoadTypeIcons()`에서 생성되며, 타입 표시와 기술 상성 표시를 담당하는 `B_SHOW_EFFECTIVENESS`는 별도의 설정이다. 이번 확인에서는 소스·데이터·ROM 수정 및 HNS 빌드를 하지 않았다.
- 검증: 설정 상수, 타입 아이콘 로딩·종별 공개 타입 판정, 기술 선택 초기화 호출부를 `rg`·`sed`로 확인했다. 다음 시작점은 설정 변경 후 싱글·더블 배틀과 포획/목격 조건, 환상·테라 타입을 실제 화면에서 확인하는 것이다.

### 2026-09-22 — 신규 배틀 파티클 16개 설정의 ROM 용량 비교
- `B_NEW_SWORD_PARTICLE`~`B_NEW_SURF_PARTICLE_PALETTE` 16개를 모두 `TRUE`로 설정한 빌드와 모두 `FALSE`인 빌드를 각각 `make hns -j8`로 링크했다. 두 결과 모두 ROM 사용량은 `33,330,212 / 33,554,432 bytes (99.33%)`였고, 증가량은 `0 bytes`, 남은 공간은 `224,220 bytes`였다.
- 관련 리소스는 `src/graphics.c`에 신규·기존 세트가 함께 포함되어 있고 `src/data/battle_anim.h`의 설정식이 선택 포인터·팔레트만 바꾸므로, TRUE 전환은 ROM 용량이 아니라 전투 애니메이션 외형을 바꾼다. 출력 ROM 파일은 32 MiB로 패딩된다.
- 검증 로그: 전체 TRUE는 `build/localization-logs/hns-all-new-particles-20260922.log`, 전체 FALSE 복구는 `build/localization-logs/hns-all-new-particles-restore-final-20260922.log`다. 기존 `NO_BAG_INVALID_VaALUE` 오타 때문에 빌드가 처음 실패해 측정 빌드 동안만 올바른 심볼명으로 임시 보정했고, 측정 후 오타와 16개 설정을 원래대로 복구했다. 현재 작업 트리의 해당 오타는 일반 HNS 빌드를 막는 기존 문제로 남아 있다.
### 2026-09-23 — `STRINGID_PKMNSITEMRESTOREDHPALITTLE` 출력 조건 확인
- 요청/범위: `src/battle_message.c`의 `{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_LAST_ITEM}{B_TXT_EU}로\n조금 회복했다.`가 언제 출력되는지 확인.
- 확인: 유일한 직접 출력 스크립트는 `data/battle_scripts_1.s:7271-7277`의 `BattleScript_ItemHealHP_Ret`다. `TryLeftovers`가 호출하는 `BattleScript_ItemHealHP_End2`가 이 스크립트로 이어지고, `TryShellBell`은 직접 이 스크립트를 호출한다.
- 결과: 체력이 가득 차지 않고 회복 봉인이 없는 포켓몬이 턴 종료에 먹다남은음식으로 최대 HP의 1/16을 회복하거나, 독 타입으로 검은오물을 지녀 같은 회복 효과를 받을 때 출력된다. 공격 후 조개껍질방울을 지닌 포켓몬이 준 피해의 1/8을 회복할 때도 출력된다. 검은오물을 지닌 비독 타입 포켓몬은 피해 경로라 이 문구가 나오지 않는다.
- 구분: 오랭열매·자뭉열매(설정에 따라)·나무열매쥬스 등 소모성 HP 회복은 `BattleScript_ItemHealHP_RemoveItem`과 `STRINGID_PKMNSITEMRESTOREDHEALTH`를 사용하므로 이 ID가 아니다.
- 수정 파일: `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`만 갱신했다.
- 검증: `src/battle_hold_effects.c`, `data/battle_scripts_1.s`, `src/data/items.h`, `src/battle_message.c`를 `rg`·`sed`로 정적 확인했다. ROM 빌드는 하지 않았다.
- 게임 화면 확인: mGBA 실행 파일 부재로 미확인.
- 남은 문제: 실제 HNS ROM에서 먹다남은음식·독 타입의 검은오물·조개껍질방울의 세 경로를 각각 재현해 이름/아이템 치환과 표시 순서를 확인해야 한다.
- 다음 시작점: `src/battle_hold_effects.c:538-554,642-655`, `data/battle_scripts_1.s:7233-7277` 및 새 HNS ROM의 아이템 효과 메시지.

### 2026-09-22 — `STRINGID_PKMNFREEDFROM` 출력 조건 확인
- 요청/범위: `src/battle_message.c`의 `{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_BUFF1}{B_TXT_EU}로부터 풀려났다!`가 언제 출력되는지 확인.
- 확인: `data/battle_scripts_1.s:5870-5873`의 `BattleScript_WrapEnds`가 유일한 직접 출력부다. `src/battle_end_turn.c:609-630`에서 살아 있는 포켓몬의 `wrapped` 상태가 마지막 구속 턴에 도달하면 `wrappedMove`를 `B_BUFF1`에 넣고 이 스크립트를 실행한다.
- 결과: 조이기·김밥말이·회오리불꽃·껍질끼우기·바다회오리·모래지옥·마그마스톰·엉겨붙기·집게덫·썬더프리즌의 자연 종료 시 사용된다. 고속스핀·킬러스핀으로 즉시 해제할 때는 `STRINGID_PKMNGOTFREE`가 사용된다.
- 검증: `src/battle_end_turn.c`, `data/battle_scripts_1.s`, `src/battle_message.c`, 구속 기술의 `MOVE_EFFECT_WRAP` 설정을 `rg`·`sed`로 정적 확인했다. 소스·데이터·ROM 수정 및 HNS 빌드는 하지 않았고, mGBA 부재로 실제 화면 검증도 하지 않았다.
- 다음 시작점: 새 HNS ROM에서 구속 자연 종료와 고속스핀·킬러스핀 해제를 각각 재현해 실제 메시지를 확인한다.

### 2026-09-22 — `{B_EFF_TEAM2}` 의미와 사용처 확인
- 요청/범위: `src/battle_message.c`의 `{B_EFF_TEAM2}`가 나타내는 값과 실제 사용처 확인.
- 확인: `src/battle_message.c:3732-3736`에서 `gEffectBattler`가 플레이어 편이면 `sText_Your2`(`우리`), 상대 편이면 `sText_Opposing2`(`상대`)를 선택한다. `2`는 팀 번호가 아니라 영어 원본의 문장 중간용 변형 슬롯이다.
- 결과: 현재 한국어 테이블에서는 `{B_EFF_TEAM2}`가 `우리` 또는 `상대`로 출력되며, `{B_EFF_TEAM1}`과 두 값이 동일하다. 직접 사용처는 `STRINGID_TOXICSPIKESABSORBED`이고, 독 타입 포켓몬이 교대해 독압정을 흡수할 때 사용된다.
- 검증: 토큰 정의, 문자열 변환부, `STRINGID_TOXICSPIKESABSORBED` 호출부와 독압정 흡수 시 `gEffectBattler` 설정을 `rg`·`sed`로 확인했다. 소스·데이터·ROM 수정 및 HNS 빌드는 하지 않았다.
- 다음 시작점: 독압정 흡수 메시지를 한국어화할 때 `우리/상대`가 조사와 자연스럽게 연결되는지 검토한다.

### 2026-09-22 — 독압정 제거 경로별 메시지 ID 확인
- 요청/범위: `STRINGID_TOXICSPIKESDISAPPEAREDFROMTEAM`과 `STRINGID_TOXICSPIKESABSORBED`의 차이 및 독압정 제거 시 실제 출력 ID 확인.
- 확인: 전자는 `gDefogHazardsStringIds`를 통해 디포그와 정리정돈이 장판을 제거할 때 사용하며 `{B_ATK_TEAM2}`는 `TryDefogClear`가 설정한 제거 대상 편을 기준으로 한다. 후자는 독 타입·접지 상태 포켓몬의 교대 진입 시 `BattleScript_ToxicSpikesAbsorbed`에서 사용하며 `{B_EFF_TEAM2}`는 흡수한 포켓몬 편을 기준으로 한다.
- 결과: 현재 두 문자열 본문은 동일한 영어 문장이라 팀명 치환 결과도 `우리`/`상대` 기준으로 동일하다. 고속스핀·킬러스핀은 `STRINGID_PKMNBLEWAWAYTOXICSPIKES`를 사용한다.
- 검증: 문자열 정의·테이블 매핑, 디포그/정리정돈·독 타입 교대·고속스핀 계열의 스크립트 경로를 `rg`·`sed`로 확인했다. 소스·데이터·ROM 수정 및 HNS 빌드는 하지 않았다.
- 다음 시작점: 세 제거 경로의 영어 문구를 한국어화할 때 실제 화면에서 편명과 조사 연결을 확인한다.

### 2026-09-22 — `{B_EFF_TEAM2}` 플레이어 편 치환값 재확인
- 확인: 문자열 처리부가 `IsOnPlayerSide(gEffectBattler)`를 검사하므로 플레이어 편이면 `sText_Your2`인 `우리`, 상대 편이면 `sText_Opposing2`인 `상대`가 출력된다.
- 결론: 효과 포켓몬이 플레이어 편일 때 `{B_EFF_TEAM2}`는 `우리` 편을 나타낸다. 소스·데이터·ROM 수정 및 HNS 빌드는 하지 않았다.

### 2026-09-22 — `{B_EFF_TEAM2}` 정확한 출력값 확인
- 정정/구분: `{B_EFF_TEAM2}` 토큰 자체는 `우리`를 출력하며, `우리 편`은 의미 설명이다. `sText_Your2` 정의에는 `편`이 없다.
- 한국어 문장에서 `우리 편`을 만들려면 문자열 본문에 `편`을 추가해야 한다. 소스·데이터·ROM 수정 및 HNS 빌드는 하지 않았다.

### 2026-09-22 — `{B_EFF_TEAM2}` 현재 소스값 정정
- 재확인: 현재 `src/battle_message.c:1550`의 `sText_Your2`는 `우리 편`이며, `sText_Opposing2`는 `상대`다.
- 정정: 현재 소스 기준 `{B_EFF_TEAM2}`의 정확한 출력값은 플레이어 편에서 `우리 편`, 상대 편에서 `상대`다. 앞선 `우리`라는 설명은 현재 사용자 변경 전의 소스 상태를 기준으로 한 잘못된 답변이다.
- 조치: 사용자 소스 변경은 유지했으며, 이번 확인에서는 소스·데이터·ROM 수정 및 HNS 빌드를 하지 않았다.

### 2026-09-22 — 고속스핀·킬러스핀 제거 메시지 ID 적용 및 HNS 빌드
- 요청/범위: 고속스핀·킬러스핀의 압정뿌리기·독압정·스텔스록·끈적끈적네트 제거 메시지를 각각 `...DISAPPEAREDFROMTEAM` ID로 바꾸고, 씨뿌리기 및 바인드 계열 해제 ID를 지정.
- 수정: `src/battle_message.c`의 `gSpinHazardsStringIds`에서 네 장판을 각각 `STRINGID_SPIKESDISAPPEAREDFROMTEAM`, `STRINGID_TOXICSPIKESDISAPPEAREDFROMTEAM`, `STRINGID_STEALTHROCKDISAPPEAREDFROMTEAM`, `STRINGID_STICKYWEBDISAPPEAREDFROMTEAM`으로 변경했다. `data/battle_scripts_1.s`의 `BattleScript_WrapFree`는 `STRINGID_PKMNFREEDFROM`을 출력하도록 변경했다.
- 유지: 씨뿌리기 해제는 기존 `STRINGID_PKMNSHEDLEECHSEED`를 사용하며, 강철서지는 기존 `STRINGID_PKMNBLEWAWAYSHARPSTEEL` 매핑을 유지했다. 고속스핀·킬러스핀은 모두 `EFFECT_RAPID_SPIN`을 사용한다.
- 검증: `make hns -j8` 성공. 로그 `build/localization-logs/hns-spin-hazard-release-messages-20260922.log`, EWRAM `249,012/262,144`, IWRAM `25,704/32,768`, ROM `33,330,148/33,554,432 bytes (99.33%)`. mGBA 부재로 실제 화면 검증은 하지 않았다.
- 다음 시작점: HNS ROM에서 각 장판 제거, 씨뿌리기 해제, 바인드 해제를 재현해 전용 문구와 `{B_ATK_TEAM2}`·`{B_BUFF1}` 치환을 확인한다.

### 2026-09-22 — 상태이상 능력 발동 문자열의 HNS `GEN_LATEST` 출력 여부 확인
- 요청/범위: `STRINGID_PKMNPOISONEDBY`, `STRINGID_PKMNBURNEDBY`, `STRINGID_PKMNFROZENBY`가 현재 HNS `GEN_LATEST`에서 실제 출력되는지 확인.
- 확인: `gGotPoisonedStringIds`, `gGotBurnedStringIds`, `gGotFrozenStringIds`는 `B_MSG_STATUSED_BY_ABILITY`를 각각 세 ID에 매핑한다. `BattleScript_AbilityStatusEffect`가 `setnonvolatilestatus TRIGGER_ON_ABILITY`를 실행하면 `BattleScript_MoveEffectPoison/Burn/Freeze`가 `printfromtable`을 수행한다.
- 결과: 독·화상 능력 경로는 `src/battle_util.c`에서 `gBattleScripting.moveEffect`를 `MOVE_EFFECT_POISON`·`MOVE_EFFECT_BURN`으로 설정하므로 `PKMNPOISONEDBY`·`PKMNBURNEDBY`가 출력될 수 있다. 동결은 능력 경로에서 `MOVE_EFFECT_FREEZE` 설정부가 없어 `PKMNFROZENBY`가 실제로 호출되지 않는다. `GEN_LATEST` 조건으로 차단되는 것은 아니다.
- 검증: 문자열 정의·상태 문자열 테이블·능력 상태이상 스크립트·`gBattleScripting.moveEffect` 대입부를 `rg`·`sed`로 확인했다. 소스·데이터·ROM 수정 및 HNS 빌드는 하지 않았다.
- 다음 시작점: 실제 HNS ROM에서 Poison Point/Flame Body 등 능력 발동을 재현해 두 문구의 버퍼와 화면 출력을 확인한다.

### 2026-09-22 — 화염구슬·맹독구슬 상태이상 메시지 확인
- 요청/범위: 화염구슬과 맹독구슬이 각각 화상·맹독을 발생시킬 때 선택되는 메시지 ID 확인.
- 확인: `BattleScript_FlameOrb`는 일반 상태이상 선택값으로 `BattleScript_MoveEffectBurn`을 호출하므로 `gGotBurnedStringIds[B_MSG_STATUSED]`의 `STRINGID_PKMNWASBURNED`가 선택된다. `BattleScript_ToxicOrb`는 `BattleScript_MoveEffectToxic`에서 `STRINGID_PKMNBADLYPOISONED`를 직접 출력한다.
- 결과: 화염구슬은 포켓몬 이름 뒤에 줄바꿈 후 `화상을 입었다!`, 맹독구슬은 포켓몬 이름의 `의` 뒤에 줄바꿈 후 `몸에 맹독이 퍼졌다!`를 출력한다. `PKMNPOISONEDBY`·`PKMNBURNEDBY`는 이 경로에서 사용되지 않는다.
- 검증: 화염구슬·맹독구슬 스크립트, 화상/독 상태 메시지 테이블과 직접 출력부를 `rg`·`sed`로 확인했다. 소스·데이터·ROM 수정 및 HNS 빌드는 하지 않았다.
- 다음 시작점: HNS ROM에서 각 구슬을 장착한 포켓몬의 턴 종료를 재현해 아이템 팝업 뒤 상태 문구를 실제 화면에서 확인한다.

### 2026-09-22 — 특성 발동 시 특성 팝업과 독·화상 메시지 순서 확인
- 요청/범위: 특성으로 독·화상이 발생할 때 특성 팝업이 `PKMNPOISONEDBY`·`PKMNBURNEDBY`보다 먼저 출력되는지 확인하고, 누락 시 수정.
- 확인: `BattleScript_AbilityStatusEffect`는 `waitstate`, `call BattleScript_AbilityPopUp`, `setnonvolatilestatus TRIGGER_ON_ABILITY` 순서로 구성되어 있다. 이후 `SetNonVolatileStatus`가 `B_MSG_STATUSED_BY_ABILITY`를 설정하고 상태 메시지 테이블에서 특성 전용 ID를 선택한다. `BattleScript_SynchronizeActivates`도 같은 구조다.
- 결론: 현재 원하는 순서가 이미 구현되어 있어 소스 수정 및 HNS 빌드는 하지 않았다. `PKMNWASPOISONED`·`PKMNWASBURNED`는 일반 상태이상 경로이고, 특성 경로는 `PKMNPOISONEDBY`·`PKMNBURNEDBY`다.
- 검증: 특성 상태이상 스크립트, `SetNonVolatileStatus`의 선택값 설정, 독·화상 문자열 테이블을 `rg`·`sed`로 확인했다. 실제 화면 검증은 mGBA 부재로 하지 않았다.
### 2026-09-26 — PC `MENU_SELECT` 번역 근거 확인

- 요청/범위: `src/pokemon_storage_system.c`의 `[MENU_SELECT] = COMPOUND_STRING("SELECT")`에 사용할 한국어 문구를 `pokeemerald-kr`과 Poké Corpus HGSS~XY 공식 한국어 원문으로 대조했다.
- 결론: `COMPOUND_STRING("선택")`을 사용한다. 이 ID는 2025년 추가된 `ChooseBoxMon` 기능에서 선택한 박스 포켓몬을 호출 스크립트로 반환하는 메뉴 명령이다. XY 공식 UI의 `Choose Pokémon and confirm.`→`포켓몬을 선택하고 결정` 및 HGSS의 같은 표기가 보여 주듯, 고르는 동작은 `선택`, 확정은 `결정`으로 구분된다. 따라서 이 메뉴에 `결정`을 쓰지 않는다.
- 참고: `pokeemerald-kr/src/strings.c`의 `gText_Select`는 영문 `SELECT`가 그대로인 미사용 문자열이라 번역 근거로 쓰지 않았다. 조사만 했고 소스 문자열은 변경하지 않았다.
- 출력 조건: 일반 PC의 `박스를 정리한다`에서는 나오지 않는다. 이벤트의 `chooseboxmon`이 PC를 `OPTION_SELECT_MON`으로 연 경우에만, 유효한 포켓몬을 A로 선택해 여는 메뉴의 첫 항목으로 표시된다. 이때 메뉴는 `선택`·`요약`·`마킹`·`취소`이고, `선택`은 PC를 닫아 그 포켓몬의 위치를 이벤트 스크립트로 돌려준다.

### 2026-09-26 — PC `MENU_INFO` 번역 근거 확인

- 요청/범위: PC 지닌물건 정리의 `MENU_INFO`를 HGSS 한국어 원문 기준으로 `설명을 읽는다`·`설명을 듣는다`·`정보` 중 어느 것으로 옮길지 확인했다.
- 결론: `COMPOUND_STRING("정보")`을 사용한다. 현재 코드는 이 항목을 누르면 `Task_ShowItemInfo()`에서 도구 설명 창을 열지만, 메뉴 원문 자체는 짧은 `INFO`다. HGSS에서 `INFO` 메뉴 라벨은 `정보`이고, `설명을 듣는다`는 시설/미니게임 설명을 NPC에게 듣는 별도 항목, `설명을 읽는다`는 이상한 카드의 본문을 읽는 별도 항목이다. 조사만 했고 소스는 변경하지 않았다.

### 2026-09-27 — 친구의 2026-09-26 upstream 선별 이식 검토

- 대상: `origin/pokehns-expansion-kor`의 `500f3634b4`(jinmo, 2026-09-26 23:38 KST)와 현재 로컬 `496bca3475`를 대조했다. 원격은 40커밋 앞서며 로컬만의 커밋은 없다. 로컬 브랜치를 이동·병합하지 않았다.
- 확인: 35개 upstream 선별 이식, 1.17.0 PR 인벤토리·인수인계 문서, naming screen PNG 3개, 한글 폰트 생성 make 규칙이 추가됐다. `src/battle_message.c`·문자열 ID·배틀 config 및 사용자가 번역한 한글 코드 문자열은 변경되지 않았다.
- 정적 검토: 동적 타입을 받는 불꽃 해동 함수의 선언/모든 호출부가 일치하며, 새 폼 변경 후 효과 명령은 현재 HNS 구조에 맞춰 battler 순서로 White Herb·편승·미러허브·탈출팩을 처리한다. 즉시 중단할 충돌은 발견하지 못했다.
- 빌드: `/tmp/pokehns-friend-review-20260926` 분리 worktree에서 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 종료 코드 0. 컴파일·링크만 검증했으며 런타임/실기는 미검증이다.
- 발견 사항: `git diff --check 496bca3475..origin/pokehns-expansion-kor`는 inventory TSV 2개(`all_prs_checked.tsv`, `already_in_history.tsv`)의 후행 탭 때문에 실패한다. 코드가 아닌 문서 형식 문제이지만, 병합 전 제거가 필요하다. 친구의 보고 ROM 사용량은 이 환경의 재빌드와 정확히 같지 않아, 수치 재현까지 통과한 것은 아니다.
- 다음: 친구에게 TSV 후행 탭 제거를 요청하고, 전자부유/록온·Baton Pass, 변신 뒤 반응 도구, Future Sight 상호작용, AI/더블/추종자/재대결을 실제 ROM에서 확인한 뒤 병합 여부를 결정한다.

### 2026-09-27 — 친구용 1.17.0 전체 엔진 동기화 지시서

- 요청: 묶음 A의 35개 선별 이식만으로 끝내지 않고, 남은 upstream 변경을 HNS 커스텀·한글화와 충돌 없이 이식해 `pokeemerald-expansion` 1.17.0 엔진 전체 동기화를 진행하도록 친구에게 지시한다.
- 구현: `docs/friend-handoff/POKEEMERALD_EXPANSION_1.17.0_FULL_ENGINE_SYNC.md`를 추가하고 friend-handoff README에 링크했다. 문서는 모든 PR의 현재 코드 기준 재판정, 의존성 순서 이식, 배틀 메시지/한글화 보존, 저장·ROM 영향 검사, 단위별·전체 HNS 빌드와 실기 검증, 결과 기록을 완료 기준으로 지정한다.
- 원칙: 전체 동기화는 upstream 전체 merge나 파일 덮어쓰기가 아니다. 기존 한글 문장과 HNS 배틀 메시지 최신화 동작을 보존하며, 단순 코드 충돌은 정확한 HNS 적응으로 해결한다. 사용자 지정 동작의 선택이 필요한 경우에만 근거·영향·선택지를 보고한다.
- 검증: 문서 변경 `git diff --check` 통과. 소스·데이터·ROM 변경과 빌드는 하지 않았다.

### 2026-09-27 — 친구 묶음 A 상태 HNS ROM 빌드

- 기준: 친구의 1.17.0 묶음 A와 전체 동기화 지시서가 반영된 `84835a32d2`.
- 빌드: `make -f make_tools.mk` 및 `check_history.sh` 성공 뒤 `NODEP=1 SETUP_PREREQS=0 make hns -j8`로 전체 컴파일·링크를 완료했다.
- 결과: `pokehns.gba` 32MiB, 생성 시각 02:15:58 KST, SHA-1 `c1b94d256e7e2ea7fa48ca971a329b9c86ba645f`; `pokehns.elf` 45,217,964 bytes.
- 한계: 자동 테스트와 실제 ROM 플레이는 실행하지 않았다. `make -q hns`는 강제 실행되는 mapjson 생성 규칙 때문에 최신 산출물이 있어도 1을 돌려줄 수 있으므로 빌드 실패 근거가 아니다.

### 2026-09-27 — PC `{DYNAMIC}` 이름 뒤 `{K_EULREUL}` 미출력 조사

- 사용자 제보 화면의 `글라이온` 선택 메시지는 `src/pokemon_storage_system.c:1052`의 `gText_PkmnIsSelected`이며, 문자열 자체에는 `{DYNAMIC 0}{K_EULREUL} 어떻게 할까?`가 들어 있다.
- `PrintMessage()`(`src/pokemon_storage_system.c:4390-4425`)는 이름을 dynamic placeholder 0에 넣고 `DynamicPlaceholderTextUtil_ExpandPlaceholders()`만 실행한다. 해당 유틸리티(`src/dynamic_placeholder_text_util.c:31-50`)는 `CHAR_DYNAMIC`만 확장하고 `PLACEHOLDER_BEGIN`을 일반 바이트로 복사한다.
- 일반 조사 처리는 `StringExpandPlaceholders()`(`src/string_util.c:362-403`)가 담당하지만 PC 경로에서는 호출되지 않는다. 텍스트 프린터(`src/text.c:1383-1385`)는 남은 placeholder를 건너뛰므로 `을`이 보이지 않는다.
- 후속 수정: `PrintMessage()`에 `dynamicExpandedText` 임시 버퍼를 두고, dynamic 치환 뒤 `StringExpandPlaceholders()`를 실행하도록 변경했다. 해당 함수는 `{K_EULREUL}`·`{K_IGA}`·`{K_WAGWA}` 모두 처리하므로 PC `sMessages`의 모든 조사 경로가 적용 범위다.
- 검증: `NODEP=1 SETUP_PREREQS=0 make BUILD=hns build/hns/src/pokemon_storage_system.o -j2` 종료 코드 0, 변경 파일 `git diff --check` 통과. 이어 HNS 전체 링크 산출물 `pokehns.gba`가 2026-09-27 02:40:14 KST에 생성됐고 SHA-1은 `bd293daf4dce22d8889d9733ca1b032142a3153c`이다. `온`의 charmap 값 `0x3D0A`는 `GetJongCode()`에서 받침 코드 4가 되어 `{K_EULREUL}`은 `을`을 선택한다.
- 실행 확인 주의: mGBA는 빌드로 덮어쓴 ROM을 실행 중인 메모리에 자동 반영하지 않는다. 완전히 종료 후 저장소 최상단 `pokehns.gba`를 다시 열어야 수정 전 ROM을 계속 보는 일을 막을 수 있다.

### 2026-09-27 — PC 저장 시스템 `menu.png` 변환 및 HNS 빌드

- 요청/범위: 사용자가 수정한 `graphics/pokemon_storage/menu.png`를 GBA 그래픽 산출물로 변환하고 HNS ROM에 반영되는지 확인했다.
- 변환: `tools/gbagfx/gbagfx`로 `menu.4bpp`를 재생성하고 `tools/compresSmol/compresSmol -w`로 `menu.4bpp.smol`을 재생성했다. 압축 파일을 다시 풀어 생성된 `menu.4bpp`와 `cmp`한 결과가 일치했다.
- 연결: `src/graphics.c`의 `gStorageSystemMenu_Gfx`가 `graphics/pokemon_storage/menu.4bpp.smol`을 `INCBIN_U32`로 포함하며, PC 초기화 경로가 이를 압축 해제해 배경 그래픽으로 로드한다.
- 검증: `make BUILD=hns build/hns/src/graphics.o -j2`와 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8`가 모두 성공했다. 링크 사용량은 EWRAM `249016/262144`(94.99%), IWRAM `25704/32768`(78.44%), ROM `33330900/33554432`(99.33%)이며, `pokehns.gba`는 2026-09-27 05:00:30 KST에 갱신됐다. ROM SHA-1은 `d7ec91b5504e54209da1e0c409a412688e35430d`다.
- 한계/다음: 이 환경에서는 mGBA를 실행할 수 없어 실제 PC 화면의 런타임 확인은 하지 않았다. mGBA를 완전히 종료한 뒤 저장소 최상단의 새 `pokehns.gba`를 다시 열어 PC 메뉴 화면을 확인한다.

### 2026-09-27 — PC 포켓몬 데이터 화면의 어두운 색상 원인 확인
- 요청/범위: 한글화 전 HNS/에메랄드와 비교해 현재 HNS의 PC 포켓몬 데이터 스프라이트가 어두운 원인을 확인하고 `P_GBA_STYLE_SPECIES_GFX`를 `FALSE`로 유지한다.
- 확인: 현재 소스의 `include/config/pokemon.h:47`은 이미 `FALSE`이며 작업 트리 변경은 없다. `src/data/graphics/pokemon.h`의 `FALSE` 분기는 `anim_front.4bpp.smol`·`normal.gbapal`, `TRUE` 분기는 `anim_front_gba.4bpp.smol`·`normal_gba.gbapal`을 사용한다.
- 근거: 현재 HNS 브랜치의 `1821fd6749`가 `P_GBA_STYLE_SPECIES_GFX`를 `TRUE`에서 `FALSE`로 변경했고, `master`에는 `TRUE`가 남아 있다. `1821fd6749` 이후 현재 HEAD까지 앞면 종 그래픽·팔레트 변경은 없으며, 한글화된 `src/pokemon_storage_system.c`의 변경도 포켓몬 팔레트 로딩부가 아니다.
- 결론: 한글 텍스트가 색상을 바꾼 것이 아니라 GBA 스타일 설정 차이 때문이다. 이전 HNS/`pokeemerald-kr`과 같은 색상을 원하면 `TRUE`가 필요하고, `FALSE`를 유지하면 최신 Gen4/5 스타일의 어두운 색상이 선택된다.
- 검증: `git diff master..HEAD -- include/config/pokemon.h`, `git show 1821fd6749 -- include/config/pokemon.h`, `src/data/graphics/pokemon.h` 조건부 리소스, `src/pokemon.c` 팔레트 선택부를 대조했다. 소스 변경 및 새 ROM 빌드는 하지 않았다.
- 남은 문제: GBA 색상 일치를 선택하면 `P_GBA_STYLE_SPECIES_GFX TRUE`로 변경 후 HNS 전체 빌드와 PC/배틀/파티 화면 검증이 필요하다.
- 다음 시작점: 사용자가 GBA 색상 일치를 승인하면 `include/config/pokemon.h:47`을 `TRUE`로 변경하고 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8`를 실행한다.

### 2026-09-27 — PC 포켓몬 데이터 옆 waveform 애니메이션 확인
- 요청/범위: PC의 `포켓몬 데이터` 글자 양옆에서 움직이는 번개형 그래픽이 현재 HNS에서 보이지 않는 원인을 확인했다.
- 확인: `src/pokemon_storage_system.c`는 `waveform.4bpp`를 `sSpriteSheet_Waveform`으로 로드하고, `CreateWaveformSprites()`에서 양쪽 스프라이트를 생성한다. `UpdateWaveformAnimation()`은 `displayMonSpecies != SPECIES_NONE`일 때만 활성 애니메이션(`i * 2 + 1`)을 선택하고, 포켓몬이 없을 때는 정지 프레임(`i * 2`)과 회색 `pkmn_data` 타일맵을 선택한다.
- 화면 근거: 제보 화면의 왼쪽 포켓몬 데이터 패널이 비어 있고 제목이 회색이므로, 현재 커서가 포켓몬을 표시하지 않는 `SPECIES_NONE` 상태로 해석된다. 박스 안에 다른 칸의 아이콘이 있어도 현재 선택 칸에 포켓몬 데이터가 표시되지 않으면 waveform은 활성화되지 않는다.
- 비교: 현재 HNS와 `master`의 저장 시스템 코드·waveform 원본 그래픽을 대조했으며 해당 애니메이션 구현 삭제나 한글화에 의한 변경은 발견하지 않았다.
- 검증: `rg`로 waveform 선언/호출부와 `UpdateWaveformAnimation()` 조건을 확인하고 `git diff master..HEAD`에서 관련 코드·원본 그래픽 변경이 없음을 확인했다. 소스·데이터·ROM은 변경하지 않았고 빌드도 실행하지 않았다.
- 다음: 유효한 포켓몬이 있는 박스 칸에 커서를 두어 왼쪽 데이터 패널을 먼저 띄운 뒤 waveform이 움직이는지 확인한다. 그 상태에서도 없으면 새 ROM을 완전히 다시 로드하고 그래픽·팔레트 로딩을 추가 조사한다.

### 2026-09-27 — PC 포켓몬 데이터 waveform 문제 해결 확인
- 사용자 재확인: 현재 박스에 선택할 포켓몬이 없어서 제목이 어둡고 waveform 애니메이션이 나오지 않았던 것이 원인이었다.
- 결과: 포켓몬을 선택하면 의도한 동작이 확인되어 문제를 해결된 것으로 종결했다. 코드·그래픽·ROM은 추가로 변경하지 않았다.
- 다음: 별도 후속 작업 없음.
### 2026-09-27 — NPC 대화 외 잔여 텍스트 인벤토리

- 요청/범위: NPC·트레이너의 맵 대사가 아닌, 메뉴·시스템·배틀·도감·시설·그래픽에 남은 한글화 대상을 실제 현재 HNS 작업 트리에서 찾고 다음 작업 단위로 정리했다.
- 수정 파일: `docs/localization/NON_NPC_TEXT_AUDIT.md`, `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`.
- 변경과 결정 이유: 새 인벤토리에 P0(플레이어 PC/가방/필드 도구·시스템 메시지·HGSS 도감/DexNav·포케기어·배틀 UI), P1(설명문·열매/굿즈·랜드마크·교환/유니언룸), P2(이지챗·시설/미니게임), 대용량 서사성 텍스트를 분리했다. 아이템/기술 설명 필드는 920/947개, 특성 설명은 321개, 이지챗은 1,030개, 열매 설명은 204개, 굿즈명은 121개라는 검색 기준 수치와 대표 경로·주의사항을 함께 기록했다.
- 실제 상태 확인: 과거 PC 감사의 영문 메뉴 목록은 현행 파일에 적용되지 않는다. `src/pokemon_storage_system.c`의 `gPCText_Give`, 메뉴와 도구 결과 문구는 한글이며, 이 파일의 확인된 영문 메뉴는 `MENU_ETCETERA`의 `etc.`만 남아 있다. `src/battle_message.c`의 Symbiosis·눈·안개·강철서지 제거는 한글이고, 멸망의바디·테라스탈 후속·FRLG 포켓몬피리/유령/사파리·Sleep Clause는 영문 잔여 후보로 남아 있다.
- 그래픽: `graphics/pokenav/hns/options/{hoenn_map,match_call,radio}.png`와 `graphics/pokenav/hns/left_headers/match_call.png`, 공용 옵션/헤더/도시 확대 PNG를 시각 확인했다. HNS 옵션·헤더의 `MAP`/`CALL`/`RAD` 및 공용 `CONDITION`/`SWITCH OFF`/`MAIN MENU`/`CITY ZOOM` 픽셀 텍스트가 남아 있으며 `src/graphics.c:1989-2044`가 이 리소스를 포함한다.
- 제외: `data/maps/**/scripts.inc`, `data/text/trainers*.inc`, `data/text/match_call*.inc`와 Match Call 통화 본문은 NPC 대사로 제외했다. TV·뉴스·Fame Checker·이벤트 티켓은 NPC 대사는 아니나 대용량 서사성 콘텐츠라 별도 사용자 범위로 보류했다. 버튼/단위 약어, 디버그·미사용 문자열, 교환 OT/닉네임은 자동 번역하지 않는다.
- 검증: `rg`로 런타임 문자열·호출부·`INCBIN`을 확인하고 대상 PNG 8개를 시각 대조했다. 시작·종료 `git status --short`는 비어 있었고 `git diff --check`를 통과했다. 문서만 수정했으므로 HNS 빌드·ROM·게임 화면 검증은 수행하지 않았다.
- 다음 시작점: `docs/localization/NON_NPC_TEXT_AUDIT.md` P0의 1단계인 `src/player_pc.c`, `src/item_menu.c`, `src/item_use.c`, `src/party_menu.c`와 `data/text/{pc,pc_transfer,save,obtain_item,surf,check_furniture,record_mix,mart_clerk,abnormal_weather}.inc`를 화면 단위로 번역한다. 제어 코드·버퍼 폭을 점검하고 `make hns -j8` 및 실제 메뉴 화면을 검증한다.

### 2026-09-27 — 잔여 텍스트 중 직접 그래픽 스프라이트 분류

- 요청/범위: NPC 대화 외 잔여 텍스트 인벤토리에서 C 문자열이 아니라 PNG 픽셀을 직접 고쳐야 하는 항목을 현재 HNS 사용 경로별로 분리했다.
- 직접 수정 P0: `POKEDEX_PLUS_HGSS=TRUE`인 도감의 `tileset_interface_hns.png`, `tileset_menu_list.png`, `tileset_menu1.png`~`tileset_menu3.png`, `tileset_menu_search.png`에 `SELECT`·`SEARCH`·`SEEN`·`OWN`·`MODE`·`TYPE` 등 고정 영문 라벨이 남아 있다. `area_unknown.png`의 `AREA UNKNOWN`도 HGSS 도감 서식 화면이 `DisplayPokedexAreaScreen()`을 호출하므로 실제 표시 가능하다. 포케기어는 HNS 옵션의 `MAP`·`CALL`·`RAD`, 공용 옵션/헤더의 `CONDITION`·`SWITCH OFF`·`MAIN MENU` 등, 도시 확대의 시설명과 `CANCEL`이 실제 로드되는 픽셀 글자다. 작명 화면은 개별 `back_button.png`·`ok_button.png`가 아니라 `rwindow.png`와 `roptions.png`를 `src/naming_screen.c`가 오프셋으로 사용하므로 그 두 원본이 직접 수정 대상이다.
- P1/정책: `union_room_chat/r_button_labels.png`는 `FREE_UNION_ROOM_CHAT=FALSE`에서 실제 로드되는 영문 버튼 라벨이다. HNS 타이틀의 `press_start.png`는 실제 `PRESS START` 원본이나, 포켓몬/하트골드 로고는 브랜드 표기로 유지하고 시작 안내만 한글화할지 먼저 결정한다.
- 수정 불필요/조건부: 배틀 기술 정보 L/R 및 Gen4 L/R은 모두 `기술 / 정보`, 메뉴 정보·PC 저장 시스템·배틀돔 버튼도 한글이다. 이지챗의 `START`/`SELECT`는 버튼명 유지로 분류했다. START 기술정보, DECA 도감 타일셋은 현재 설정에서 미사용이고, `DEXNAV_ENABLED=FALSE`의 DexNav `captured_all.png`·`no_data.png`는 이름과 달리 글자가 아닌 아이콘/표식이다.
- 검증: `src/graphics.c`, `src/pokedex_plus_hgss.c`, `src/pokedex_area_screen.c`, `src/naming_screen.c`, `src/battle_interface.c`, `src/union_room_chat.c` 및 설정 헤더의 실제 로드 조건을 `rg`/`sed`로 대조하고 대상 PNG를 시각 확인했다. 문서만 수정했으므로 HNS 빌드·ROM·실기 검증은 하지 않았다.
- 다음: `NON_NPC_TEXT_AUDIT.md`의 새 표를 기준으로 런타임 문자열과 원본 PNG를 혼동하지 않는다. PNG 작업은 타일 수·팔레트·순서를 보존하고 `.4bpp`/`.smol`을 재생성한 뒤 `make hns -j8`과 실제 화면 검증을 수행한다.

### 2026-09-27 — 포케기어 HNS 옵션 PNG의 분할 표시 구조 확인

- 질문/범위: `graphics/pokenav/hns/options`의 32×64 PNG가 정적인 이미지로 보면 잘려 보이는데 실제 포케기어 메뉴에서 어떤 방식으로 온전한 라벨이 표시되는지 확인했다.
- 구조: 각 옵션 PNG는 1,024바이트(32×64, 4bpp)다. `sOamData_MenuOption`은 32×16 OAM이고, `CreateMenuOptionSprites()`가 라벨마다 네 스프라이트를 만든 뒤 `x2 = 32 * j`로 가로 배치한다. `DrawOptionLabelGfx()`는 시작 타일에서 `8 * j`씩 건너뛰며 네 조각을 지정한다. 그러므로 PNG에서는 32×16 조각 네 개가 세로로 쌓여 보이지만, 화면에서는 128×16 라벨 하나가 된다.
- HNS 결합 순서: `graphics/pokenav/hns/options/options.4bpp`(0x3800)는 지도(HNS)→컨디션→통화(HNS)→리본→전원→파티→검색→쿨→뷰티→큐트→스마트→터프→취소→라디오(HNS)의 14개 0x400 바이트 `.4bpp`를 연결한 결과와 `cmp`로 일치했다. `sOptionsLabelGfx_*`의 시작 타일 `0x000`~`0x1A0`도 이 순서를 가리킨다.
- 재생성 주의(정정): `options.4bpp`에 대응하는 단일 `options.png`는 없지만, `graphics_file_rules.mk:702-716`에 14개 개별 `.4bpp`를 위 순서로 연결하는 HNS 전용 Make 규칙이 있다. 원본 PNG와 개별 생성물이 존재하면 `make hns`가 결합 시트와 `.smol`을 자동 갱신한다. 원본이 삭제된 채 옛 `.4bpp`만 남은 경우에는 증분 빌드가 낡은 결과를 재사용할 수 있으므로 원본 존재 여부도 확인해야 한다. `options.bin`은 이 라벨 스프라이트를 가로 배치하는 파일이 아니다.
- 검증: `src/pokenav_menu_handler_gfx.c:368-405,891-905,928-943`의 OAM·스프라이트·타일 대입을 확인하고, HNS 통합 `.4bpp`와 14개 조각을 바이트 대조했다. 당시에는 문서만 수정했으므로 HNS 빌드·ROM·실기 검증은 하지 않았다.

## 2026-09-27 — 포케기어 PNG 변환과 `포켓기어` 헤더 타일맵 수정

- 요청: `graphics/pokenav`에서 사용자가 한글화한 PNG들을 변환·테스트하고, HNS 포케기어 화면 왼쪽 위의 `POKéMON GEAR`를 새 `포켓기어` 픽셀 그림대로 표시한다.
- 원인: `graphics/pokenav/hns/header.png`는 424×8 크기의 완성 헤더가 아니라 53개 8×8 타일을 가로로 저장한 시트다. `header.bin`은 영문 로고의 반복 타일 ID를 계속 가리키고 있어서 PNG만 바꾸면 한글 획 조각이 영문 배치대로 반복되어 깨졌다.
- 수정: `graphics/pokenav/hns/header.bin`의 화면 상단 3개 타일 행을 새 한글 타일 2~22번과 왼쪽 2타일 여백에 맞춰 재배치했다. 세 번째 행의 빈 영역에는 구분선을 포함한 배경 타일을 사용해 기존 가로선을 보존했다. 나머지 타일맵은 건드리지 않아 오른쪽 아래 작은 `포켓기어`도 유지했다.
- 변환: 수정된 컨디션 그래프/취소, HNS 헤더·통화 헤더·지도/통화 옵션·라디오 UI, 공용 좌측 헤더 12개, 도시 확대 텍스트를 `.4bpp`로 다시 만들었다. 빌드 과정에서 관련 `.smol`, 팔레트, `header.bin.smolTM`도 갱신됐다. HNS 옵션 합성물은 지도(HNS)→컨디션→통화(HNS)→리본→전원→파티→검색→쿨→뷰티→큐트→스마트→터프→취소→라디오(HNS) 연결 결과와 `cmp`가 일치했다.
- 정적 시각 검증: 새 `header.4bpp`·`header.gbapal`·`header.bin`을 `gbagfx`로 역렌더링했다. 왼쪽 위 큰 `포켓기어` 네 글자가 올바르게 연결되고, 상단 구분선과 오른쪽 아래 작은 `포켓기어`가 유지되는 것을 확인했다.
- 빌드: 첫 `make hns -j8`은 전체 소스 재컴파일 도중 600초 제한으로 종료됐다. 변환 산출물을 확인한 뒤 같은 HNS 전체 빌드를 재실행했고 `BUILD_EXIT=0`으로 컴파일·링크·ROM 생성을 완료했다. ROM 사용량은 EWRAM 249,016/262,144 bytes, IWRAM 25,704/32,768 bytes, ROM 33,315,908/33,554,432 bytes다. `pokehns.gba` SHA-1은 `872956ba1b350325d17b26aa4bcf172e01d6f5ea`다.
- 미완료/주의: 공용 메인 옵션 원본 `graphics/pokenav/options/{beauty,cancel,condition,cool,cute,hoenn_map,match_call,party,ribbons,search,smart,switch_off,tough}.png` 13개는 작업 트리에서 삭제 상태다. 기존 `.4bpp`가 남아 있어 증분 빌드는 성공했지만 이 부분의 번역은 반영됐다고 판정할 수 없고, 깨끗한 빌드도 재현할 수 없다. `graphics/pokenav/hns/options/radio.png` 역시 `RAD` 그대로다. 이 원본들을 복구·한글화한 뒤 다시 빌드해야 한다.
- 실행 검증 구분: 현재 환경에서 `mgba-qt`, `mgba`, `mgba-sdl` 명령을 찾지 못해 실제 게임 화면은 실행하지 않았다. 새 `pokehns.gba`를 mGBA에서 완전히 다시 불러온 뒤 포케기어 메인 메뉴와 각 하위 화면을 확인해야 한다.

## 2026-09-28 — 복구된 포케기어 PNG 전체 변환과 큰 헤더 재수정

- 요청: 다시 추가·수정한 공용 옵션 PNG, `city_zoom_text.png`, HNS `radio.png`·`ui_tiles.png`를 변환하고, 여전히 깨져 보이던 화면 상단 큰 `포켓기어`를 사용자가 제시한 스프라이트 모양으로 바로잡는다.
- 원본 상태: 이전에 삭제 상태였던 `graphics/pokenav/options/{beauty,cancel,condition,cool,cute,hoenn_map,match_call,party,ribbons,search,smart,switch_off,tough}.png` 13개가 모두 수정 파일로 복구됐다. 각 옵션과 HNS 라디오는 32×64, 라디오 UI는 128×32, 도시 확대 텍스트는 64×64 규격이다.
- 헤더 원인/수정: 이전 타일맵은 `header.png`의 한글 상·중·하단을 모두 x=2에 놓아 가운데와 아래 획이 오른쪽으로 밀렸다. 큰 제목은 실제로 6·8·7타일 폭의 세 띠이므로 `header.bin`에서 위쪽 타일 2~7은 x=2, 가운데 8~15와 아래 16~22는 x=1부터 배치했다. 아래 행의 나머지는 구분선 배경 타일로 채우고 다른 행은 보존했다.
- 정적 시각 검증: 갱신한 `header.4bpp`·팔레트·타일맵으로 256×256 헤더를 역렌더링하고 제목 부분을 최근접 확대했다. 큰 `포켓기어`의 네 글자 획이 사용자가 제시한 오른쪽 스프라이트와 같은 형태이고, 구분선 및 오른쪽 아래 작은 제목도 유지되는 것을 확인했다.
- 변환/합성 검증: 수정된 `graphics/pokenav` PNG 전부를 `.4bpp`로 강제 재변환했다. 공용 13개 옵션 합성물과 HNS 14개 옵션 합성물을 각 Make 규칙의 순서대로 `cat`한 임시 스트림과 `cmp`했으며 둘 다 종료 코드 0이었다. 전체 빌드가 관련 `.smol`, `header.bin.smolTM`, 라디오 UI 및 도시 확대 압축물도 갱신했다.
- HNS 빌드: `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8`가 `BUILD_EXIT=0`으로 성공했다. EWRAM은 249,016/262,144 bytes, IWRAM은 25,704/32,768 bytes, ROM은 33,316,276/33,554,432 bytes다. 생성된 `pokehns.gba` SHA-1은 `251f8e38a680ca0f85a1453d386e02635f30284a`다.
- 실행 검증 구분: 현재 환경에 mGBA 명령이 없어 실제 메뉴 입력 검증은 하지 않았다. mGBA를 완전히 종료한 뒤 새 ROM을 다시 열어 메인 메뉴의 지도/컨디션/전화/리본/전원/파티/검색/다섯 능력/취소/라디오, 라디오 화면, 도시 확대 및 큰 헤더를 확인한다.

## 2026-09-28 — Aseprite 선택 영역 기준 큰 `포켓기어` 헤더 재패킹

- 재요청/정정: 앞선 수정은 상·중·하단 타일 띠 자체를 좌우 이동했기 때문에 사용자가 지정한 스프라이트 영역 밖의 여백까지 섞였다. 사용자가 새 Aseprite 화면으로 지정한 실제 구성은 동일한 50픽셀 폭의 위 2px, 가운데 8px, 아래 2px를 세로로 결합한 50×12 그림이다.
- 추출: 현행 헤더 타일에서 위 조각은 타일 2~7의 마지막 2행(48px 뒤 배경 2px), 가운데 조각은 타일 8~15의 8행 중 선택된 x=7~56, 아래 조각은 타일 16~22의 첫 2행 중 선택된 x=5~54로 읽었다. 결합 결과의 첫 글자는 상단 가로획, 가운데 `ㅍ`, 하단 `ㅗ`가 정확히 연결된다.
- 재패킹: 완성된 50×12 팔레트 인덱스를 화면상의 56×24(7×3타일) 영역에 넣고 구분선 행을 보존했다. 21개 위치의 중복 패턴을 제거해 고유 타일 18개를 `header.png`의 타일 2~19에 기록하고, `header.bin`이 새 타일 ID 배열을 x=2부터 참조하도록 바꿨다. 기존 타일 20의 빈 구분선 배경과 타일 23 이후의 작은 제목/기타 헤더는 유지했다.
- 정확성 검증: 새 `header.png`를 다시 `.4bpp`로 변환한 결과가 임시 재패킹 데이터와 `cmp`로 일치했다. 이어 최종 `header.4bpp`와 `header.bin`에서 화면의 50×12 픽셀을 역추출해 세 선택 조각의 결합 원본과 비교했으며 `FINAL_50X12_MATCH=True`였다. 이번 검증은 글자를 눈대중으로 판정한 것이 아니라 팔레트 인덱스 600개가 모두 같은지 확인한 것이다.
- 빌드: `header.4bpp.smol`과 `header.bin.smolTM`을 재생성한 뒤 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8`를 실행했고 `BUILD_EXIT=0`으로 성공했다. EWRAM 249,016/262,144 bytes, IWRAM 25,704/32,768 bytes, ROM 33,316,276/33,554,432 bytes이며 `pokehns.gba` SHA-1은 `26159eedba9d17edae529fb0efae8ef95dd96c10`이다.
- 실행 확인: 현재 환경에는 mGBA 명령이 없으므로 실제 화면은 사용자가 새 ROM을 완전히 다시 불러 확인해야 한다. 이전 SHA-1 ROM이 열린 상태라면 이번 재패킹은 반영되지 않는다.

## 2026-09-28 — 재편집한 Aseprite 51×12 선택 영역으로 큰 `포켓기어` 헤더 갱신

- 요청/입력: 사용자가 `graphics/pokenav/hns/header.png`를 다시 편집하고, 위 2px·가운데 8px·아래 2px의 각 선택 영역을 같은 51×12 완성 그림으로 결합해 출력하도록 요청했다.
- 추출/재패킹: 원본 타일 데이터에서 위쪽은 타일 2~7 마지막 2행(48px 뒤 팔레트 인덱스 4의 배경 3px), 가운데는 타일 8~15의 x=7~57, 아래는 타일 16~22의 x=4~54를 읽었다. 612개 팔레트 인덱스를 7×3 타일 영역으로 패킹할 때 고유 타일이 20개여서 2~19 및 화면에서 기존에 미사용인 21·22번을 사용했다. 구분선용 20번과 작은 제목/기타 헤더의 23~52번은 그대로 보존했다. 16비트 BG 타일맵 엔트리의 화면 x=2~8, y=0~2만 새 타일 ID로 갱신했다.
- 정적 검증: 생성한 `header.png`를 다시 `header.4bpp`로 변환한 결과가 패킹 데이터와 `cmp` 일치했다. 최종 `header.4bpp`·`header.bin`으로부터 화면의 51×12를 역추출해 선택 조각 결합 원본과 비교했고 `FINAL_51X12_MATCH=True`(612/612 픽셀)였다. 타일 20 및 23~52 보존도 바이트 비교로 확인했다. 대상 변환으로 `header.4bpp.smol`과 `header.bin.smolTM`도 갱신했다.
- ROM/한계: HNS 전체 빌드는 새 ROM 링크까지 진행되어 `pokehns.gba` 생성 시각과 SHA-1이 `5476f96067b6a4829c6f28ea53672b770d2dd27c`으로 갱신됐다. 그러나 이 환경의 셸 실행 제한이 빌드 프로세스의 최종 종료 코드 기록보다 먼저 적용돼 `BUILD_EXIT=0`은 아직 확보하지 못했다. 실제 mGBA도 이 환경에 없으므로, 다음 세션은 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8`의 종료 코드를 먼저 재확인한 뒤 새 ROM을 완전히 다시 불러 큰 `포켓기어`를 확인한다.

## 2026-09-28 — 51×12 선택 공백 보존 재패킹 및 ROM 재링크

- 정정: 직전 재패킹은 이전 50px 작업의 가운데·아래 X 보정을 그대로 적용해, 새 선택 영역의 앞쪽 공백을 제거했다. 따라서 글자 획을 왼쪽으로 밀어 실제 완성 스프라이트와 다르게 만들었다.
- 최종 추출: 원본 타일 시트 백업에서 위 `(16,6,51,2)`, 가운데 `(64,0,51,8)`, 아래 `(128,0,51,2)`를 그대로 결합했다. 선택 안의 배경 인덱스 4도 데이터로 보존했다. 20개 고유 타일을 2~19, 21~22에 배치하고 20번 구분선 및 23~52번 작은 제목/기타 헤더 타일은 보존했다.
- 검증: 완성 `header.4bpp`와 16비트 BG `header.bin`에서 화면상의 51×12를 역추출해 원본 선택 조합과 비교했고 `FINAL_51X12_MATCH=True`(612/612 픽셀)였다. `.4bpp` 변환 결과도 패킹 데이터와 `cmp` 일치했다.
- 빌드: 그래픽 바이너리의 의존성 누락을 피하기 위해 `build/hns/src/graphics.o`를 재컴파일(1,276,908 bytes)한 뒤 HNS ROM을 재링크했다. 새 `pokehns.gba` SHA-1은 `0e374d5e4f5a6498bc3f64a5799255dbe06eaa01`이다. 현재 환경에는 mGBA가 없으므로, 다음 확인은 이 SHA-1의 ROM을 완전히 다시 로드해 큰 `포켓기어`가 사용자 완성 스프라이트와 같은지 실제 화면에서 보는 것이다.
