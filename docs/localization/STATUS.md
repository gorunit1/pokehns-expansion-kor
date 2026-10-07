# 현재 인수인계 상태

## 2026-10-07 저녁 — 친구 답 반영: seq 181 #9730 단위(23커밋) + HnS 수정 10개 완료·push (현재, 데스크탑 — 랩탑에서 Remote Control)

- **다음 할 일(순서)**
  1. ~~검증 도구 저장소 이전~~ **끝**: `2e60559d0b`·`a368916c5c` → `dev_scripts/hns_verify/`(README 참고). 이제 검증 명령은 저장소 도구로: 한글 `ALLOW_REPO=1 dev_scripts/hns_verify/kortests/run.sh <트리> <라벨>`(기대 494/584 = `kortests/expected/summary.txt`), 세이브 정적 비교 `save/save_compat.py collect … && run --pre …`, 세이브 왕복 `save/savetest/run_all.sh`, 경고 `warncheck.sh`, 테스트 목록 `testlist.sh`. 노트북 실측은 아직(정적 비교가 ARM 13.2.Rel1 objdump에서 같은 커밋끼리 WARN 0인지, 한글 기대 요약이 노트북에서도 맞는지). 스크래치 원본(`chunk-1385/verify`, `hnsfix-1007b/kortests-final`)은 남아 있다.
  2. ~~친구 문자~~ 보냄(사용자, `chunk-181/discord-2026-10-07-result.md` 3개). 친구 답은 위 X6.
  3. **full-sync 다음 seq 182 #9865**(Mega Sol test adjustments). 순서표상 이미 적용: 183·193·194·195·201·203·210·211·212·213·234·247·252·258·314·324·326·344·347·395·472(#9730 단위 선반영), 198·199(선반영), 266은 부분(끈적끈적네트 config — 기본 Gen9면 동작 변화, 결정 필요), 433 #10282는 `toxic_thread.c` hunk만. 167 #9819 보류 그대로.
- **이번에 한 것:** 결과 [`full-sync-seq-181-181.md`](../friend-handoff/results/1.17.0-port/full-sync-seq-181-181.md), [`full-sync-hnsfix-2026-10-07b.md`](../friend-handoff/results/1.17.0-port/full-sync-hnsfix-2026-10-07b.md), 친구 답 [`FRIEND_REPLY_2026-10-07.md`](../friend-handoff/FRIEND_REPLY_2026-10-07.md), 회신 `HANDBACK_2026-10-07.md` 9절
  - `f9fe99ab1f` CREDITS(금선 앞모습 예외), `24ff546631` 친구 답 기록
  - #9730 단위 23커밋 `a5eb87ba6b`..`9234efb074`(후속 22개 전부, 독실 Champions −2 + #10282 테스트 hunk, `ignoreDefiant` 손 맞춤)
  - HnS 10커밋: `7687b9acf4` 주눅구슬·충전 문장, `0a28f4489b` 아드레날린오브·약점보험 한계, `7acf285ccf` 눈덩이·충전지·구근·빛이끼 한계, `453256d19d` 토템 문장, `09f14a68d6` 1단계 공백, `c1e44ade43` 불복종 난수, `1e9f3254f9` 위기회피 춤추기, `feeabe2388` 토템 주석, `33fb8df0fd` 경혈찌르기 심술꾸러기·단순, `820148e779` 비축하기 심술꾸러기
  - 검증(최종 `820148e779`): ROM 32,752,244 B(seq 181 전 −2,320), EWRAM 250,404 B(+272, `gSpecialStatuses`), IWRAM 25,516 B, SHA1 `eab66f3f…`, 새 경고 0. 전체 테스트 PASS 2,494 / TOTAL 5,400, 사라진 PASS 0(이름 변경 2 제외) → **기준 [`test-baseline-hnsfix1007b.txt`](../friend-handoff/results/1.17.0-port/test-baseline-hnsfix1007b.txt)**. 한글 회귀 584개 = 기대(494 PASS). 세이브 정적 비교 PASS(WARN 3 설명됨)·왕복 PASS. 리뷰 3개 수정 필요 0. AI 실측 `chunk-181/ai-compare/AI_COMPARE.md`.
  - 메인 검증 스크립트: `/home/hjm0725/hns-sync-work/chunk-181/main/main-verify.sh`, `main-verify2.sh`(세이브 정적 비교 `--pre chunk-181/main/savecompat-pre181`)
- **친구 답(저녁):** 재확인 28 → "본가대로(전부 발동)"(`FRIEND_REPLY_2026-10-07.md` 3절) → **완료** `79129cb876`(막힌 위협에도 발동, 한글 HNSX1 16 갱신) + #9730 회귀 3개 `1a0deef861`·`8979334b2d`·`3accada01e`. 최종 ROM 32,752,628 B, SHA1 `41f6b7ee…`, 전체 테스트 PASS 2,518 / TOTAL 5,424(사라진 PASS 0) → **기준 `test-baseline-hnsfix1007c.txt`**, 한글 584개 기대와 같음(494/584), 세이브 정적 비교 PASS. 회신 HANDBACK 10절(친구 문자 아직 안 보냄).
- **full-sync 묶음 7 사전 분석 끝, 사용자 확인 대기**(결정 6개: #9890 디버그 HnS 1줄, seq 384 #10211 앞당김, SaveBlock3 +4 B, #9865 AI 날씨·#9857 AI 예측 변화 수용, 디버그 날씨 이름표 미추가). 산출물 `chunk-182-187/seq*.md`·`.patch`(+`seq384-10211.patch`). 이전 표기:(사용자 승인 "같이 ㄱㄱ"): seq 182 #9865, 184 #9857, 185 #9910, 186 #9890, **186.5 #9920**(외부결정 확정 A: SaveBlock3 끝 `u32 dailySeed`, 세이브 영향), 187 #9877(`WEATHER_DYNAMIC=24`, `WEATHER_LEAVES=23` 유지). 지시 `/home/hjm0725/hns-sync-work/chunk-182-187/ANALYZE.md`, 기준 사본 `chunk-182-187/base`(HEAD `e34512d599`), 영역 A(182)·B(184)·C(185·186)·D(186.5·187). 결정이 필요한 것은 적용 전에 사용자에게 보인다.
- 실기 미확인(친구 계획: 위협·미러아머·흉내허브·하양허브·승기/오기·토템, 주요 트레이너 AI).

## 2026-10-07 낮 — #9730 사전 분석 완료, 친구 결정 대기 (데스크탑 — 랩탑에서 Remote Control로 진행)

- **멈춘 지점:** seq 181 #9730 사전 분석(영역 A~F)을 끝냈다. 사용자가 결정을 친구에게 넘기기로 해서 **적용은 친구 답을 받은 뒤**에 한다. 결정 요청은 [`HANDBACK_2026-10-07.md`](../friend-handoff/HANDBACK_2026-10-07.md) 8절(1 후속 PR 22개 동반, 2 AI 변화, 3 아드레날린오브·약점보험 +6 발동, 4 독실 −1/−2, 5 토템 능력치 문장, 6 1단계 하락 문장 이중 공백, 7 주눅구슬·충전 문장 유지 제안, 8 기록만 할 변화).
- **친구 문자: 보냄(2026-10-07 낮, 사용자).** 내용 = `/home/hjm0725/hns-sync-work/frontierpass-1007/discord-2026-10-07.md` 6개(= HANDBACK_2026-10-07 1~8절 요약, 3번에 프런티어패스 비교 그림 2장). **친구 답 대기:** 4번 질문 5개(금선 출처, 직업 이름 깨짐 10g, 불복종 난수 3d, 위기회피 춤추기, 도구 이전 17) + 6번 #9730 결정 8개. AI 비교 결과 보충 메시지 `chunk-181/discord-9730-followup.md`도 보냄(사용자). 친구 답 대기: 4번 질문 5개 + #9730 결정(HANDBACK 8절 1~10번, 9번 용의춤류 감점 포함).
- **AI 변화 실측(끝):** `/home/hjm0725/hns-sync-work/chunk-181/ai-compare/AI_COMPARE.md`. 첫 턴 선택이 이식 전과 다른 비율 #9730만 5.8% / AI 3개 뺀 19개 5.7% / 22개 전부 0.9%. 요약·추가 결정(용의춤류 감점)은 HANDBACK 8절 2·9·10번. 친구 보충 메시지 `chunk-181/discord-9730-followup.md` 보냄.
- **사전 분석 산출물(저장소 밖, `/home/hjm0725/hns-sync-work/chunk-181/`):** 지시서 `ANALYZE.md`, `part-A~F.md`·`part-A~F.patch`(기준 사본 `base` = HEAD `364ab51c7f` 코드에 `--check` 통과), 후속 행 `F-<seq>-<PR>.patch` 22개(적용 순서·확인 스크립트 `tmp-F/scripts/check_F.sh`), 한글 회귀 `E-kortests/`(328 세트 + 새 235, 기준 513/563 PASS, 이식 뒤 바뀌는 출력 분류 `expected-changes.tsv`).
  - 합본 실측: `make hns` 성공, ROM 32,750,900 B(−3,664), EWRAM 250,404 B(+272), 새 경고 0. F 22개를 더하면 ROM +752 B, 사라진 PASS 0(F 판정, `part-F.md` 4.2절).
  - 적용 때 확인할 것: A의 `ignoreDefiant` 초기화(upstream 병합 `7e0c2d430e` 형태, B·F가 지적 — 빠지면 룸서비스 오기·승기 테스트 2개와 F-266 아군 오기 1개 FAIL), F-183이 B의 HnS `trynonmovestatchange` 2줄을 `trystatchanges`로 바꿈, 한글 회귀 `HNSFIX1 T13` 기대값(중간 상태용) 갱신, 독실 결정에 따라 `part-D.patch` 1줄 또는 `toxic_thread.c` 1.17.0 형식.
- **친구 답이 오면:** 결정을 `ANALYZE.md` 옆 `APPLY.md`(선례 `chunk-176-179/APPLY.md`, `chunk-1385/APPLY.md`)에 반영해 적용 에이전트 1개 → 리뷰 → 메인 검증(전체 테스트·한글 회귀 563개·세이브 정적 비교/왕복) → 문서 → push.
- 테스트 기준 `test-baseline-seq179.txt`, 빌드 기준 ROM 32,754,564 B·SHA1 `5876c53d…`(프런티어패스 뒤) 그대로.

## 2026-10-07 새벽 — 프런티어패스 2건 완료·push, #9730 사전 분석 준비까지 하고 종료 (데스크탑)

- **다음 할 일(순서)**
  1. ~~프런티어패스 렌더링~~ 끝(종료 직전 완료, 결과 문서 3절). 수정 전 렌더가 친구 스크린샷과 맞고, 수정 후에는 뱅크 1~7 픽셀과 머리 아이콘만 바뀐다. 그림은 `/home/hjm0725/hns-sync-work/frontierpass-1007/render/png/compare-*.png`.
  2. **친구 문자(아직 안 보냄):** 초안 5개(각 2,000자 이하)가 `/home/hjm0725/hns-sync-work/frontierpass-1007/discord-2026-10-07.md`에 있다. 사용자가 보낼 때 3번 메시지에 `render/png/compare-pass.png`·`compare-map.png`를 첨부한다.
  3. **seq 181 #9730(XL, Stat Change Refactor) 사전 분석:** 지시서 `/home/hjm0725/hns-sync-work/chunk-181/ANALYZE.md`(영역 A 엔진 코어 / B 스크립트 / C 나머지 호출부 / D AI·기술 데이터 / E 한글 메시지·한글 회귀 세트 / F upstream 테스트·후속 행). 기준 사본 `chunk-181/base`(HEAD `364ab51c7f` 코드, ROM 32,754,564 B, SHA1 `5876c53d…`). upstream diff `chunk-181/upstream-9730.diff`, 파일별 `--check` 결과 `chunk-181/tmp/check.txt`(155 통과 / 21 실패). 에이전트 6개를 띄웠다가 사용자 종료로 몇 분 만에 멈췄다 — `tmp-A`~`tmp-F`의 중간 파일은 지우고 다시 띄운다(각 영역에 "ANALYZE.md를 읽고 영역 X 담당" 지시, 선례 프롬프트는 SESSION_LOG 이 날 항목). 결정이 필요한 건 적용 전에 사용자에게 짧게 보여 준다.
- **프런티어패스(친구 요청 2부):** 결과 [`frontier-pass-2026-10-07.md`](../friend-handoff/results/frontier-pass-2026-10-07.md), 회신 `HANDBACK_2026-10-07.md` 7절
  - `00461d5cf6` 머리 아이콘(친구 첨부본 그대로, 팔레트 같음), `364ab51c7f` `bg.png` → 8bpp·128색(upstream `bg.png` PLTE = kr `tiles.pal`), 픽셀 인덱스 그대로
  - 확인: `bg.4bpp` 바이트 동일, `bg.gbapal` 256 B = upstream, ROM `gFrontierPassBg_Pal` = upstream
  - 빌드: ROM 32,754,564 B(+224), EWRAM·IWRAM 0, SHA1 `5876c53d…`, 새 경고 0. 전체 테스트 목록이 `test-baseline-seq179.txt`와 바이트 동일
  - 실기 미확인(친구)
- 테스트 기준은 그대로 `test-baseline-seq179.txt`(PASS 2,429 / TOTAL 5,332). 빌드 기준(데스크탑)은 위 ROM·SHA1.

## 2026-10-07 — 친구 지시서 3건 + 엔진 수정 3건(3b·3c·10f) + full-sync 묶음 6(seq 176~179, 198 선반영) (데스크탑)

- **(완료 → 위 새벽 절) 친구 프런티어패스 요청 2건:** 요청 [`FRIEND_REQUEST_2026-10-07.md`](../friend-handoff/FRIEND_REQUEST_2026-10-07.md) 2부. 사용자는 "바로 작업하지 말고 먼저 보라"고 했고, 분석(읽기 전용)까지 끝냈다. 진행 확인을 받은 뒤 커밋 2개로 넣는다.
  - **머리 아이콘:** 첨부본 `/home/hjm0725/hns-sync-work/frontierpass-1007/map_heads_hns_friend.png`는 지금 `graphics/frontier_pass/map_heads_hns.png`와 크기(16×32)·팔레트(16색, 순서까지 같음)가 같고 그림만 다르다 → 그대로 교체.
  - **색 깨짐 원인(확정):** HnS `graphics/frontier_pass/bg.png`는 pokeemerald-kr `graphics/frontier_pass/tiles.png`와 바이트 같은 4bpp·**16색 팔레트** PNG다. kr(옛 구조)은 배경 팔레트를 별도 `tiles.pal`(JASC 128색)에서 만들었지만, expansion은 `bg.png`의 PLTE로 `bg.gbapal`을 만들어 지금 32 B(1뱅크)뿐이다. 코드는 `gFrontierPassBg_Pal`을 8뱅크로 읽는다(`include/config/general.h` `BUGFIX` → `NUM_BG_PAL_SLOTS 8`, 트레이너 별 색은 뱅크 1~5). 뱅크 1~7이 ROM 뒤 데이터로 채워져 8bpp `map_and_card`(작은 맵·트레이너카드)와 맵 화면(같은 팔레트 사용)이 깨지고, 뱅크 0만 쓰는 틀·글자 상자·심볼·배틀포인트는 정상 — 친구 스크린샷과 일치. kr `tiles.pal` 128색은 upstream 1.17.0 `bg.png`(8bpp, PLTE 128) 팔레트와 128색 모두 같다.
  - **수정안:** `bg.png`를 upstream 구조(8bpp, PLTE 128 = kr `tiles.pal`)로 바꾸되 픽셀 인덱스는 그대로(→ `bg.4bpp` 바이트 동일, `bg.gbapal` 32 → 256 B, ROM 약 +224 B). 검증: `bg.4bpp` 동일·`bg.gbapal` = upstream 팔레트, 테스트 러너로 패스 본체·맵 화면 팔레트 RAM/화면 덤프 → 렌더링 전후 비교(HP 박스 때 방법, `hnsfix-1007/tmp-hp/` 참고). 실기는 친구.
- **아직 안 보낸 친구 문자:** `HANDBACK_2026-10-07.md` 요약 문자를 사용자가 아직 보내지 않았다. 프런티어패스 2건 결과를 합쳐 다시 정리해 보낸다.
- **다음 할 일 2 — seq 181 #9730(XL, Stat Change Refactor)** — 단독 단위로 사전 분석부터. 선반영해 둔 행: 180 #9843, 198 #10014, 199 #10020(그 자리에서는 "이미 적용"). 167 #9819는 #10548 직전까지 보류.
  - 테스트 기준: `docs/friend-handoff/results/1.17.0-port/test-baseline-seq179.txt`(PASS 2,429 / TOTAL 5,332). 빌드 기준(데스크탑): ROM 32,754,340 B, SHA1 `94079f56…`. 노트북 툴체인은 SHA1이 다르다(크기는 같음).
  - 한글 통합 회귀: `ALLOW_REPO=1 /home/hjm0725/hns-sync-work/chunk-171-174/tmp-171/kortests/run.sh <저장소> <라벨>`(328개, 기대 요약 `runs/post1007b-summary.txt` = `post179`). K1-05·K1-09(`!!`→`!`)·K8-04·K8-05(3b)·K2-02(HP 999) 기대값 갱신, 원본 `*.bak-before-1007`.
  - 세이브 왕복 도구 주의: 이미지를 바꾼 뒤 테스트를 다시 빌드하지 않아 다른 이미지를 읽은 적이 있다(`LOAD image=` 줄로 확인, 재실행 PASS).
- **친구 지시서 3건(2026-10-07):** [`FRIEND_REQUEST_2026-10-07.md`](../friend-handoff/FRIEND_REQUEST_2026-10-07.md), 결과 [`full-sync-hnsfix-2026-10-07.md`](../friend-handoff/results/1.17.0-port/full-sync-hnsfix-2026-10-07.md)
  - `eeb44ec376` 금선 앞모습·목호·실버 뒷모습 교체, `301f2b59ed` 배틀타워 엘리베이터 직원 (1,5), `58a6e9649f` Gen4 HP 박스 기믹 레벨 한 번만(친구 예시와 달리 SoulGold식 오른쪽 정렬 — 예시대로면 숫자가 아이콘 아래에 깔림, 실측)
- **엔진 수정 3건(친구 결정 2026-10-06):** 같은 결과 문서
  - `ee2da89a61` 3b 탈출 아이템으로 들어온 춤추기(+테스트 4), `6aa0a1b778` 3c 불복종 자해 데미지(+테스트 6, `#if TESTING` 훅·러너 `OTName_`), `0d4f52ea20` 10f 유니온룸·배틀타워 통신 멀티 직업+이름(+오른쪽 상대 애드온)
  - 빌드 ROM 32,754,084 B(+464), 전체 테스트 새 PASS 10·사라진 PASS 0 → `test-baseline-hnsfix1007.txt`, 세이브 왕복 PASS, 리뷰 2개 문제 없음
- **묶음 6:** [`full-sync-seq-176-179.md`](../friend-handoff/results/1.17.0-port/full-sync-seq-176-179.md)
  - 176 #9864 이미 적용, `44465dc322` 177 #9879, `eb8263e754` 178 #9911, `e2a8dbc7ef` 179 #9906(+HnS 사파리 1줄), `519ccf7ef6` 198 #10014 선반영
  - ROM +256 B, 테스트 TO_DO 이름 1줄 말고 같음, 한글 회귀 같음, 세이브 왕복 PASS(재실행), 리뷰 문제 없음
- 문서: 출력 변화 문서 3행(3b·3c·10f), 재확인 3b·3c·10f 해결, 3d·10g·8t 추가, 8r 갱신, 17 이전 검토 메모. 2026-09-28 Gen4 레벨 기록(c327)에 정정 주석. 친구 회신 [`HANDBACK_2026-10-07.md`](../friend-handoff/HANDBACK_2026-10-07.md)(질문 5개: 금선 앞모습 출처, 직업 이름 깨짐 10g, 불복종 난수 3d, 위기회피 춤추기, 도구 저장소 이전 17).
- 실기 미확인.

## 2026-10-06 — 친구 답(2026-10-06) 반영 1차: 문자열 4줄, 프런티어 로비 직원 번호, 심향·금선 그래픽 (현재, 노트북)

- **친구 답:** [`docs/friend-handoff/FRIEND_REPLY_2026-10-06.md`](../friend-handoff/FRIEND_REPLY_2026-10-06.md). AI 기본 교체 HnS 유지, OWE OFF 유지, 질문 5개 모두 "고친다"(3b 춤추기, 10d `메일`, 3c 불복종 자해 데미지, 10e `!`, 10f 직업+이름 형식) + `MENU_READ` `메일을 읽는다`. 배틀타워 질문 3개(직원 통과, `아니`, 프런티어 동선). 그래픽 작업 지시서(심향·금선) [`ETHAN_LYRA_PLAYER_GRAPHICS.md`](../friend-handoff/ETHAN_LYRA_PLAYER_GRAPHICS.md) — **사용자 추가 지시: 심향 배틀 앞모습은 바꾸지 않는다.**
- **이번에 한 것(노트북):** 결과 [`full-sync-hnsfix-2026-10-06.md`](../friend-handoff/results/1.17.0-port/full-sync-hnsfix-2026-10-06.md), 회신 [`HANDBACK_2026-10-06.md`](../friend-handoff/HANDBACK_2026-10-06.md)
  - `0276c7310f` 문자열 4줄: `apdlf`→`메일`, `메일을 읽는`→`메일을 읽는다`(64 px 창 안), `내보냈다!!`→`내보냈다!`, `MULTI_YESNO` `아니`→`gText_No`(`아니오`)
  - `9126e5f8df` 배틀타워·배틀돔 로비 직원 번호: HnS 로비가 Emerald 번호(`LOCALID_TOWER_ATTENDANT_*`, `LOCALID_DOME_ATTENDANT_*`)를 빌려 써 엉뚱한 직원이 움직였다. HnS `map.json`에 `*_HNS` 번호 이름(타워 1·6·7·8, 돔 1·5)을 붙였다. 다른 빌려 쓰는 번호 89곳 대조, 프런티어 이상은 이 두 곳뿐. 별건: 방울탑 옥상 기모노 소녀 번호(재확인 25)
  - 빌드: 종료 코드 0, ROM 32,738,292 B(−16 B), EWRAM 250,132 B, IWRAM 25,516 B. 고친 파일 경고 0. 전체 테스트 목록이 `test-baseline-seq174.txt`와 바이트 동일(PASS 2,419 / TOTAL 5,322, 노트북)
  - 프런티어 동선(질문 C): 아쿠아호 첫 관동 도착(`VAR_SSAQUA_STATE` 7) 뒤 담청시티·갈색시티 항구 선원 메뉴로 간다(표 없음). 실제 콘텐츠다
- **심향·금선 플레이어 그래픽(같은 날, 노트북):** 결과 [`ethan-lyra-player-graphics-2026-10-06.md`](../friend-handoff/results/ethan-lyra-player-graphics-2026-10-06.md). 사용자 결정으로 단계별 커밋을 push했다(지시서의 "push 하지 말 것"은 친구 로컬과 섞지 말라는 뜻으로 봄).
  - 커밋: `fe939d6456` 심향 → Gold(**앞모습·앞모습 팔레트 유지**), `eb46975ff9` 금선 → Kris 그림·팔레트, `f24bfd60de` 금선 비대칭 애니메이션(원본 43 + 회전 4, 표 7), `1a17daf00f` Kris 뒷모습
  - 빌드: ROM 32,753,620 B(+15,328 B), EWRAM·IWRAM 변화 0, SHA1 `60542673…`. 새 경고 0(원본 PNG `bKGD` 청크 제거)
  - 정적 확인: Kris 애니메이션 프레임 번호 전부 범위 안, 동쪽 `hFlip` 0, 표 항목 누락 0. 1.17.0은 반사 팔레트 파일을 읽지 않고 본 팔레트에 `ApplyPondFilter`를 씌운다(반사 `.pal`은 같은 필터 값으로 갱신)
  - 실기 미확인(친구). 스크래치: `/home/jinmo/hns-sync-work/sprites-ethan-lyra/`(원본 sparse clone·도구)
- **다음 할 일(순서)**
  1. 엔진 수정 3건(3b 춤추기 + 탈출버튼·탈출팩 회귀 테스트, 3c 불복종 자해 데미지, 10f 통신 멀티 교체 문장 직업+이름). 데스크탑 한글 회귀(328개)로 같이 확인 권장
  2. full-sync seq 176 #9864부터(아래 2026-10-05 절의 순서·기준 그대로)
- 노트북에는 데스크탑 스크래치 도구(`save_compat.py`, `kortests/run.sh`)가 없다. full-sync 묶음과 엔진 수정의 한글 회귀는 데스크탑에서 하거나, 도구를 저장소로 옮긴 뒤 한다(재확인 17).

## 2026-10-05 — full-sync seq 142~175 완료(167 보류, 199 선반영) + 친구 답 반영, 다음 seq 176 (현재, 데스크탑)

- **멈춘 지점:** seq 175 #8434(+#10020 선반영)까지 끝내고 push했다. 사용자 지시로 여기서 멈추고(2026-10-06 새벽) 친구에게 보고했다(내용 = `HANDBACK_2026-10-05.md` 1~7절 요약). **친구 답 대기:** 질문 5개(재확인 3b 춤추기, 10d `apdlf`, 3c 불복종 HP, 10e `내보냈다!!`, 10f 유니온룸 문장 형식)와 보고 2건(#9847 기본 교체 HnS 유지 `cc8fa0418a`, #8434 OWE 꺼 둠). 답이 오면 그 반영부터 하고, 이어서 다음은 **seq 176 #9864**(S, 교체 화면 뒤 체력 상자), 177 #9879(M, wild_encounter config 이동 — #8434 HnS 이로치 줄 손 병합, 재확인 8s), 178 #9911(S), 179 #9906(M), 181 #9730(XL, Stat Change Refactor). 180 #9843 선반영, 199 #10020 선반영(seq 175와 함께), 167 #9819 보류.
  - 테스트 기준: `docs/friend-handoff/results/1.17.0-port/test-baseline-seq174.txt`(seq 175 뒤에도 목록 같음). 빌드 기준: ROM 32,738,308 B, SHA1 `4d8ecf63…`.
  - 경고 기준: `/home/hjm0725/hns-sync-work/chunk-ahead-130-167/warn-base.txt` + 알려진 도구 경고 `libpng warning: bKGD: invalid index`(#8434 `shiny_sparkle.png`).
  - 세이브 정적 비교: `save_compat.py run --pre <직전 기준>`을 쓰면 묶음별 차이만 본다(예: `chunk-175/tmp-175-impact/savecompat-pre175`, `collect`로 만든 이식 전 사실). 기본 `run`은 묶음 1 이전 기준이라 #7573 표시(FAIL 2)와 #8434 종 데이터 WARN이 섞인다.
  - 한글 통합 회귀: `ALLOW_REPO=1 /home/hjm0725/hns-sync-work/chunk-171-174/tmp-171/kortests/run.sh <저장소> <라벨>`(328개, 기대 요약 `runs/post175-summary.txt`). **할 일:** HNS9799 K2-02를 플레이어 MaxHP 999 형태로 바꿔 기대 요약을 다시 만든다.
- **seq 175 #8434(+199 #10020):** `full-sync-seq-175-175.md`
  - 커밋: `9d0e2f51ee` #8434(`WE_OW_ENCOUNTERS FALSE`, HnS 적응 3곳 + D4 + 이동 타입 번호 + 이로치 줄), `2e148e3122` #10020
  - 빌드 ROM +21,472 B, EWRAM·IWRAM 0, 새 경고 0(허용 도구 경고 1). 전체 테스트 목록 동일, 한글 회귀 328개 같음, 세이브 정적 비교 FAIL 0(`--pre` seq 174), 세이브 왕복 PASS. 리뷰 2개 문제 없음(기계어 비교)
  - 재확인: 8s(뒤 행 손 병합), 24(이동 타입 번호·OWE 켤 때 조건)
- **묶음 5(seq 171·172·173, 174 이미 적용):** `full-sync-seq-171-174.md`
  - 커밋: `729674ecf5` #9799, `0b29f4e551` #9850, `482d67210b` #9847, `cc8fa0418a` HnS B안(스마트 교체가 없는 AI의 기본 교체 유지, 사용자 승인 — 친구에게 보고)
  - 빌드: ROM 32,716,836 B(+384), EWRAM·IWRAM 0, 새 경고 0. 전체 테스트 사라진 PASS 0(이름 변경 1 제외). 한글 회귀 328개 의도한 1개 말고 같음. 리뷰 2개 코드 결함 0
  - 출력 변화 문서: 멀티 녹화 재생 1행, 링크 문장 행 갱신. 재확인: 8p~8r(H1~H5·B안 후속), 10e(`!!`), 10f(유니온룸 형식), 22 보충
- **친구 답(팝업 Q1~Q11·조사 10b) 반영:** `friend-reply-2026-10-05.md`, 친구 기록 `FRIEND_REPLY_2026-10-04.md` 3절, 회신 `HANDBACK_2026-10-05.md` 5절
  - 커밋: `0dd9022851`(주술·팀 가드 조사), `66c1e55f3d`(#10321 헤드리스 팝업 가드 선반영), `e5a5630635`(먹다남은음식·조개껍질방울·풍선 등장·필드 시드 팝업)
  - 빌드: ROM 32,716,452 B, SHA1 `1c4a6c81…`, 새 경고 0. 전체 테스트 사라진 PASS 0, FAIL → PASS 37 → `test-baseline-seq170-friend1005.txt`(묶음 5 전 기준). 한글 회귀 316개 같음
  - 뒤 seq에서 친구 답대로 받을 것: Q3~Q5 회복 문장 삭제(seq 475 #9777, 재확인 8k), Q9 반감열매 문장 위치(seq 483 #10431, 8l), Q10 팝업 순서(seq 394 #10268 + HnS 보완, 8m). 뒤 PR 문맥 주의 8n·8o
- **묶음 1(seq 142~147+155):** `full-sync-seq-142-147.md`, 커밋 `713e6499d7`..`26e66ff4ad` + HnS `9eae3b51dc`(텐트 파티 수)
- **묶음 2(seq 150·152·153·154):** `full-sync-seq-150-154.md`, 커밋 `80341f3e3e`..`97d00ef44b` + HnS `f8a465e3bc`(KO 애니 소프트락)·`8552b5e9a8`(미래예지 두 번 뒤 교체 보존)
- **묶음 3(seq 159·161·162):** `full-sync-seq-159-162.md`, 커밋 `e33ed95e1a`·`93a0354570`·`60f365365a`
- **묶음 4(seq 166·168·169·170):** `full-sync-seq-166-170.md`
  - 커밋: `487898f831` #9784, `c625740524` #9832, `b391fc833c` #9835, `e12e86b186` #9751
  - 빌드: ROM 32,716,452 B(−608), EWRAM 250,132 B(+4, `gBattlersByRawSpeed`), IWRAM 0, SHA1 `32883308…`, 새 경고 0
  - 테스트: `build/port-check-chunk166.log` PASS 2,374 / TOTAL 5,304, 사라진 PASS 0, KNOWN_FAILING → PASS 1, 새 PASS 1 → `test-baseline-seq170.txt`
  - 한글 회귀: `chunk-166-170/tmp-166/kortests/run.sh` 316개, 의도한 변화(신규 18 + T9) 말고 같음. 리뷰 probe 61개 추가
  - 세이브: 정적 비교 새 차이는 `gBattlersByRawSpeed`뿐, 세이브 왕복 PASS
  - 리뷰 3개: 코드 결함 0. 문서 보완(4.2절 밖 순서 변화 2, 탈출팩 미발동 범위, #9835 배치 변화, #9751로 `624ef7d4bd`도 테스트 불가) 반영
  - 출력 변화 문서: 4행 추가, #9494·#9674 행 3개 고침. 재확인: 3b 갱신, 8f~8j 추가
- **사용자·친구에게 물을 것(기존 결함, 고치지 않음, 친구 답 없음):** 불복종 자해 HP가 줄지 않음(재확인 3c), 파티 메뉴 "메일"이 `apdlf`(10d), 탈출버튼·탈출팩으로 들어온 춤추기가 같은 턴 춤을 따라 추지 않음(3b, 1.17.0도 같음) 그 밖에 친구 확인 거리: 재확인 10e(`내보냈다!!`), 10f(유니온룸·배틀타워 통신 멀티 교체 문장 형식), 유니온룸 등 `HANDBACK_2026-10-05.md` 6절.
- 팝업 Q1~Q11·조사 10b는 친구 답을 받아 반영했다(위).
- 실기 미확인.

## 2026-10-04 — 친구 요청 A·B·C 완료, seq 139 #9714 완료 (데스크탑)

- **멈춘 지점:** 사용자 지시대로 (1) A·B·C와 (2) seq 139까지 하고 멈췄다. 다음은 사용자 확인 뒤 **seq 142 #8930**(Automate regional Pokedex orders, L)이다. seq 140·141·143은 이미 적용됐다.
- **(1) 친구 요청 A·B·C:** 결과 `docs/friend-handoff/results/1.17.0-port/friend-requests-abc-2026-10-04.md`, 친구 회신 `HANDBACK_2026-10-04.md` 6절
  - 커밋: A `3464fa89b6`(순풍 시작), B `ef8c779b55`(그래스필드 회복), C `f79f3baf2b`(멘탈허브 팝업. 내던지기 포함, 친구 Q1 대기)
  - 계획서 열매 이름 3곳 정정
  - 리뷰: 문제 없음
- **(2) seq 139 #9714:** `32b62b550f`. 결과 `docs/friend-handoff/results/1.17.0-port/full-sync-seq-139-139.md`, 친구 회신 `HANDBACK_2026-10-04.md` 7절
  - 빌드: ROM 32,715,348 B(+176), EWRAM 250,128, IWRAM 25,516. SHA1 `900161aa…`, 새 경고 0
  - 테스트: 전체 목록이 기준과 같다(`test-baseline-seq139.txt`). 세이브 정적 비교 PASS
  - 한글 회귀: seq 139 세트는 이식 전 34/34, 이식 후 31/3(의도한 upstream 변경 3). 턴 종료 66/4
  - 리뷰 2개: 문제 없음
  - 출력 변화: 안개제거 확인 단계 결함 수정 1행(upstream 유래, HnS에서 닿음)
- **실기(mGBA) 미확인.** 확인 목록은 각 결과 문서와 `HANDBACK_2026-10-04.md` 5~7절이다.
- **친구 답 대기**
  - 팝업 Q1~Q11(`HANDBACK_2026-10-04.md` 3절). 선택 patch는 `/home/hjm0725/hns-sync-work/chunk-popup/p3-opt-*.patch`이고, Q1이 X면 멘탈허브 내던지기 2줄을 뺀다.
  - 같은 유형의 조사 문제 2곳(주술 시작, 팀 가드. 재확인 목록 10b)
- 회귀 테스트(저장소 밖): `/home/hjm0725/hns-sync-work/chunk-132/D-tests/`(턴 종료 70개, 2-10 기대 문장 갱신), `/home/hjm0725/hns-sync-work/chunk-139/B-tests/`(안개제거·정리정돈·방벽 34개. 저장소에서는 `ALLOW_REPO=1 run.sh <저장소> <라벨>`)

## 2026-10-04 — seq 138.5 #8943 단위 완료, 팝업 대조 분석, 친구 디스코드 답 반영 (데스크탑)

- **다음 할 일: 친구 요청 A·B·C 코드 수정**(사용자 지시 순서 (3), 사용자 확인 뒤 시작). 그다음 **seq 139 #9714**(Defog/Tidy Up restructure, L).
  - A 순풍 시작 문장: `STRINGID_TAILWINDBLEW`의 `{B_ATK_PREFIX2}에게` → `우리 편`/`상대`를 내는 팀 토큰. 문장 본문은 그대로다.
  - B 그래스필드 회복 문장: `STRINGID_GRASSYTERRAINHEALS` 본문을 공용 `{B_ATK_NAME_WITH_PREFIX}의\n체력이 회복되었다!`로 바꾼다.
  - C 멘탈허브 팝업: 확정 patch `/home/hjm0725/hns-sync-work/chunk-popup/p2-mentalherb.patch`(Ret+Fling). Fling은 친구 Q1 답에 따라 뺄 수 있다.
  - 선택 patch(`p3-opt-*.patch`)는 친구 답(`HANDBACK_2026-10-04.md` 3절 Q1~Q11)을 받은 뒤 정한다.
  - 한글 임시 테스트로 확인하고 `BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 기록한다.
- **#8943 단위 결과:** `docs/friend-handoff/results/1.17.0-port/full-sync-seq-138.5-138.5.md`. 시작 HEAD `1ce2b6d4bf` 뒤 커밋은 다음과 같다.
  - #8943 `022e666847`(+#9725)
  - 같은 단위 후속 7개(선반영 seq 141·164·180·215·287·307·342): `4b437cd5da`·`8e205d9e92`·`7db72aa705`·`4616fac998`·`262bd0abd7`·`93656e609b`·`b09e11c9b5`
  - HnS 보호 수정 3개: `70fe10ecd8`·`c68e8e13ba`·`cc0576a543`
  - 리뷰 후 HnS 수정 4개: `8bcf557c20`·`624ef7d4bd`·`6c15064bce`·`f3a58f9939`. 원인은 목호 멀티 파티 번호 충돌이고, upstream 1.17.0에도 같은 결함이 있다.
  - 친구 요청 상대 141 분리 `b43032bf03`
  - 단위 밖 인트로 주인공 Y좌표 `5bbd01e69f`
- **검증(최종 코드 `f3a58f9939`)**
  - 빌드: ROM 32,715,172 B(−1,872), EWRAM 250,128 B(+1,192, 여유 12,016), IWRAM 0. SHA1 `4f874584…`, 새 경고 0
  - 세이브: `save_compat.py run` PASS(FAIL 0, WARN 3 = 의도된 차이). 세이브 왕복 PASS(이식 전 세이브 바이트 동일 복원). 녹화 배틀만 A안대로 무효
  - 전체 테스트: PASS 2,350 / TOTAL 5,271, 사라진 PASS 0. 새 기준 목록 `test-baseline-seq138.5.txt`
  - 한글 턴 종료 회귀: 66/4(이전과 같음)
  - 한글 소스 줄: 통신 교체 문장 3쌍의 토큰만
  - 리뷰: 커밋 리뷰 5개 → 수정 커밋 리뷰 FR1·FR2 → FR3(상대 141·R4)
  - **실기(mGBA) 미확인.** 확인 목록은 결과 문서 "실기 확인 항목"과 `HANDBACK_2026-10-04.md` 5절이다.
- **친구 결정(디스코드, `FRIEND_REPLY_2026-10-04.md` 2절)**
  - `B_MULTI_HALF_TEAMS` (a) FALSE 유지. 6쌍 4마리와 배틀타워 멀티 UI 변화는 기록했다.
  - 상대 141 분리를 검토해 달라고 했다 → `b43032bf03`
  - 인트로 Y 통일 → `5bbd01e69f`
- **요청 C 분석 완료(코드 미변경):** `docs/friend-handoff/results/1.17.0-port/popup-champions-compare.md`
  - 경로 56개 가운데 Champions와 다른 곳은 7개다. 확정은 멘탈허브 1개이고, 나머지 6개는 친구 확인이 필요하다.
  - 계획서의 열매 이름 "이스타·캄라"는 오기다(실제 #10268 대상은 애슈·미클·자보·애터·랑사). 3곳에 있고 아직 고치지 않았다.
- 재확인 목록 추가: `RECHECK_BEFORE_COMPLETION.md` 8b~8d(#9799·#10051·#10059, 화이트아웃 #10039~#10568, **#10711 HnS 적응**), 18~23(녹화 섹터 여유 0 B, `struct Trainer` 5/4, `sSavedParties` 정적 배열, HnS 보호 수정 목록, 남은 1.17.0 결함, FALSE 결정).
- 스크래치: `/home/hjm0725/hns-sync-work/chunk-1385/`(적용·리뷰·수정 기록, `verify/`), `/home/hjm0725/hns-sync-work/chunk-popup/`(P1~P3, patch).

## 2026-10-04 — 친구 mGBA 결과 수신, 다음 작업 대기 (데스크탑)

- **친구 mGBA 결과(`60b32d674d` ROM, seq 127~138): 확인 항목 전부 정상.** 기록은 `docs/friend-handoff/results/1.17.0-port/mgba-check-seq127-138.md`다. 10-04 HnS 수정 3건은 아직 실기 확인 전이다.
- **친구 새 요청 3건**(같은 문서 2·3절)
  - A: 순풍 시작 문장 `우리 편은` → `우리 편에게`/`상대에게`
  - B: 그래스필드 회복 문장을 공용 `…의\n체력이 회복되었다!`로
  - C: 지닌 도구 발동 팝업을 Champions와 전수 대조해 다른 곳을 수정. 멘탈허브는 확정 수정 대상이다. upstream #10268·#9777과 같은 방식으로 맞춘다.
- **사용자에게 물은 것(답 대기)**
  1. #8943 `B_MULTI_HALF_TEAMS`: TRUE(이식 전과 완전히 같음, 메인 권장) 또는 계획 메모의 FALSE.
  2. 진행 순서. 메인 제안은 다음과 같다.
     - (1) 친구 결과 기록(완료)
     - (2) #8943 적용 + 팝업 조사(읽기 전용) 병렬
     - (3) A·B 문장 수정 + C 팝업 수정을 HnS 커밋으로
- 다음 세션 시작 프롬프트는 SESSION_LOG 2026-10-04 마지막 항목에 있다.

## 2026-10-04 — seq 138.5 #8943 사전 분석 완료, 적용 대기 (데스크탑)

- 대상: #8943 12v12 capability(upstream `70340c1135`, 168파일, XL, A안). 분석 산출물은 스크래치 `/home/hjm0725/hns-sync-work/chunk-1385/`에 있다(`part-A~E.md`, patch, 세이브 검증 도구 `verify/`). 이식 전 빌드는 `base/`다.
- 분석 결과
  - A+B+C patch를 스크래치에 합쳐 `make hns`가 성공했다. ROM −1.4 KB, EWRAM +1.2 KB(여유 12 KB), 새 경고 0, 테스트에서 사라진 PASS 0.
  - **일반 세이브 호환을 실측했다.** SaveBlock·섹터·세이브 경로 54개가 같고, 이식 전 ROM 세이브 2개를 이식 후 코드로 읽어 바이트 동일하게 복원했다.
  - 녹화 배틀은 A안대로 옛 기록이 무효가 된다. 새 형식은 섹터 여유 0 B다.
- 함께 넣을 것
  - #9725는 #8943 커밋에 포함한다(HnS `multi_2_vs_2` 빌드 오류 방지).
  - 같은 단위 회귀 수정 7개는 바로 뒤 별도 커밋으로 넣는다: #9729(알로라 더블 야생), #9811, #9843, #10102, #10415, #10536, #10662.
- HnS 수정 후보(#8943이 드러낸 upstream 1.17.0 결함, 이식 전 HnS는 정상)
  - 오른쪽 트레이너 대상 강제 교체 실패
  - 파트너 멀티 경험치 오배분
  - 반 팀 멀티에서 파트너 기절 수로 플레이어가 화이트아웃
  - #10536 상대 B 경험치 인덱스
- **결정 대기(사용자):**
  - `B_MULTI_HALF_TEAMS`: 계획 메모는 FALSE다. 하지만 FALSE면 두 트레이너 동시 발견 6쌍이 4마리를 내고 배틀타워 멀티 UI가 바뀐다. TRUE면 이식 전과 완전히 같다(권장).
  - 적용 시점: 친구 mGBA 결과 확인 뒤.
- HnS 적응 확정
  - `struct Trainer` 비트필드 5/4(HnS 음악·머그샷 값)
  - 녹화 배틀 시작 파티 정적 저장(upstream 방식 버그 회피)
  - `sText_LinkTrainerSentOutPkmn`은 #9799 때 처리

## 2026-10-04 — 친구 답 반영: HnS 수정 3건, 재확인 목록 (데스크탑)

- **다음 시작 seq: 138.5** (#8943 12v12, XL, 세이브 영향). 친구 확인: A안 그대로 진행한다(녹화 배틀 기록 무효화 허용, 일반 세이브 호환 유지).
- 친구 답장: `docs/friend-handoff/FRIEND_REPLY_2026-10-04.md`. seq 138까지 승인. mGBA 결과는 따로 보내 준다(친구가 우리 브랜치를 빌드해 확인 중).
- HnS 수정 3건: 결과는 `docs/friend-handoff/results/1.17.0-port/full-sync-hnsfix-2026-10-04.md`
  - `d78de7fdb5`: 녹화 배틀 재생 플레이어 뒷모습을 Gold/Kris로 바꿨다(upstream 1.17.0과 같은 줄).
  - `d37911167c`: 전자부유 종료 문장에 전자부유를 쓴 포켓몬 이름이 나온다.
  - `b07fd88953`: 탈출버튼·탈출팩으로 들어온 포켓몬의 반사·가로채기 assert를 고쳤다. 교체 때 이전 포켓몬의 매직코트·가로채기 상태를 지운다(배턴터치 포함, upstream #10338 선반영 → seq 414 때 HnS 4줄 삭제).
- **재확인 목록 신설:** `docs/friend-handoff/results/1.17.0-port/RECHECK_BEFORE_COMPLETION.md`(full-sync 완료 전 재확인)
  - 가로챈 멀리짖기 assert(친구 결정: 고치지 않고 기록, upstream 수정 우선)
  - 중간 상태, 한글 조사 문제 2건(텔레키네시스 `{이름}는`, 섬광 `{이름}로부터`)
  - 탈출버튼으로 들어온 춤추기 포켓몬 문제 등
- 검증
  - 빌드: ROM 32,717,044 B(+96 B), SHA1 `04c307c5…`. 새 경고 0, 한글 소스 줄 0.
  - 임시 테스트: 수정 1의 16개, 리뷰 8개 모두 PASS
  - 턴 종료 한글 회귀: 66/4 그대로(3-07 기대값을 고친 이름으로 갱신)
  - 전체 테스트: `test-baseline-seq138.txt`와 바이트 동일
  - 독립 리뷰: 세 커밋 모두 문제 없음
- 출력 변화 문서: 탈출 아이템 행을 수정하고 전자부유 행을 추가했다.

## 2026-10-03 — 구간 seq 135~138 완료 (데스크탑)

- **다음 시작 seq: 138.5** (#8943 `U-12v12-8943` 12v12 capability, XL, 세이브 영향 Y, korean_touch Y).
  - 확정 결정 A안: upstream 1.17.0 녹화 배틀 구조를 쓴다. 기존 녹화 기록은 무효화를 허용한다. 일반 게임 세이브 호환은 유지한다(`CLAUDE_FULL_SYNC_PORT_PROMPT.md` 4절).
  - 같은 unit 후속 행: seq 140 #9725, 141 #9729, 180 #9843, 215 #10102. 그다음 seq 139 #9714(deps #8943)다.
- 구간 결과: `docs/friend-handoff/results/1.17.0-port/full-sync-seq-135-138.md`
  - #9709 `a642657907`: `BattlePokemon.affectionHearts`. 구조체 크기 144 B 그대로, 통신·세이브 영향 없음. `config/ai.h` 테스트 hunk는 제외했다.
  - #9711 `62d1876d6f`: 주석 삭제에 더해 `metLevel:7`/`isShiny:1` 비트필드 압축. ROM +32 B, 구조체가 1.17.0과 같아졌다.
  - seq 137·138: 이미 적용
- 검증
  - 리뷰 2개: 모두 문제 없음.
  - 빌드: ROM 32,716,948 B(+48 B), EWRAM·IWRAM 0, SHA1 `dfeec488…`. 새 경고 0, 한글 줄 0.
  - 전체 테스트: `test-baseline-seq132.txt`와 바이트 동일. 기준 목록은 `test-baseline-seq138.txt`(내용 같음)다.
- 실기 확인(선택): 이로치 상대에게 메타몽 변신 색

## 2026-10-03 — seq 132 #9680 완료 (데스크탑)

- **다음 시작 seq: 135** (#9709 `U-9709` Improve AI calc speed with affection hearts, S). seq 133·134는 선진행으로 이미 적용이다. 그 뒤로 136 #9711, 138.5 #8943(XL), 139 #9714가 있다.
  - 선진행한 seq 130·131·133·134·137·143·148·149·151·156·157·158·160·165와 선반영한 seq 274는 "이미 적용"이다.
- #9680(`5f580ab12e`): 턴 종료 효과 스크립트를 `end2`에서 `BattleScriptCall`/`return`으로 바꿨다. 결과 문서는 `docs/friend-handoff/results/1.17.0-port/full-sync-seq-132-132.md`다.
- HnS 보존
  - 맹독구슬·화염구슬 아이템 팝업 문장
  - 아이스바디·치유의마음 문장, 포이즌힐·선파워·젖은접시 무문구
  - 하양허브 내던지기 분기(1.17.0은 병합 중에 잃음)
  - 방벽별 HnS 종료 스크립트, 스위트베일 하품 문구
  - 위 스크립트는 본문을 두고 끝만 `return`으로 바꿨다. 안개제거 `DEFOG_CLEAR` hunk는 뺐다.
- 검증
  - **임시 한글 출력 순서 테스트 70개**(저장소 밖): 이식 전 70/70 → 이식 후 66/4. 4개는 의도한 upstream 변경이다(다이맥스 3개는 HnS 미도달, 매직룸 하양허브 1개).
  - 리뷰 3개: 경미 1, 문제 없음 1, 코드 문제 없음·문서 경미 1. 수정 필요 0. 호출·종료 짝 546곳을 전수 조사했고, 스크립트 스택 계측에서 8칸 초과는 0이었다.
  - 빌드: ROM 32,716,900 B(+16 B), SHA1 `512ccdbe…`. 새 경고 0, 한글 줄 0.
  - 테스트: PASS 2,344 / TOTAL 5,261. 사라진 PASS 0, FAIL→PASS 4(볼주머니·강제 리샘열매). 새 기준 목록은 `test-baseline-seq132.txt`다.
- 출력 변화: 하양허브 강제 발동 경로 1행(트릭·스위처 자뭉열매 시점, 나눔 뒤 공생, 매직룸 종료 루프). upstream 1.17.0과 같다.
- 남은 위험: 미래예지 빗나감마다 스크립트 스택 1칸이 샌다. 아주 깊은 연쇄에서만 넘친다(실측). #9939(seq 470)에서 해소된다.
- **친구에게 물을 것**
  - 새로 1건: 전자부유 종료 문장 이름(이식 전부터, upstream 같음).
  - 이전 질문: seq 129 2건(탈출버튼 뒤 반사·가로채기 assert, 가로챈 멀리짖기), seq 128 1건(녹화 배틀 뒷모습)
- 실기 확인 추가: 결과 문서 "실기 확인 항목"
  - 맹독구슬·화염구슬, 무문구 회복, 하양허브
  - 소란 턴 끝, 방벽·날씨·필드 종료
  - 미래예지, 아레나·팰리스

## 2026-10-03 — seq 129 #9674 unit 완료 (데스크탑)

- **다음 시작 seq: 132** (#9680 `U-endturn-9680` End Turn events use BattleScriptCall, XL). seq 130·131은 선진행으로 이미 적용이다. 그 뒤로 135 #9709, 136 #9711, 138.5 #8943(XL)이 있다.
  - 선진행한 seq 130·131·133·134·137·143·148·149·151·156·157·158·160·165는 "이미 적용(선진행)"이다.
  - 이번에 선반영한 **seq 274 #10386**도 "이미 적용"이다.
- 커밋
  - #9674 `49415e3007`: 매직미러·매직코트를 move end로, 가로채기를 캔슬러로 옮겼다.
  - #10386 `887c8e9ef8`(seq 274 선반영): 같은 unit의 회귀 수정이다.
  - 리뷰 후 HnS 수정 `587f4e7cdc`: 반사자의 `targetsDone` 초기화, `moveTarget` 1.17.0 형태.
  - 결과 문서: `docs/friend-handoff/results/1.17.0-port/full-sync-seq-129-129.md`
- HnS 적응
  - 캔슬러 순서 1.17.0형, `unused5`(seq 166 #9784 때 맞춤)
  - 한글 토큰 2개: 매직미러 ATK→DEF, 가로채기 DEF→ATK. 화면 이름이 이전과 같고 본문은 불변이다. 임시 한글 테스트로 실측했다.
  - **가로채기 연출 `gEffectBattler` HnS 1줄**(upstream 1.17.0에 남은 잠재 버그를 보호)
- 검증
  - 리뷰 3개: 경미 1, 문제 없음 1, 수정 필요 2(`587f4e7cdc`로 반영)
  - 빌드: ROM 32,716,884 B(+928 B), EWRAM·IWRAM 0, SHA1 `f453524c…`. 새 경고 0.
  - 전체 테스트: PASS 2,340 / TOTAL 5,261. 사라진 PASS 2는 되살린 막말내뱉기 테스트로, seq 181에서 풀린다. 기준 목록은 `test-baseline-seq129.txt`이고, 수정 뒤에도 바이트 동일하다.
- 출력 변화: `BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 12행(중간 상태 2행 포함)을 넣었다. 다크홀 사용 문구·PP, 반사 순서, 습기 우선 등이다.
- **중간 상태(seq 181 #9730 전까지)**
  - 튕긴 막말내뱉기는 원래 사용자가 교체된다.
  - 가로챈 쪽의 대상 판정이 엉뚱한 포켓몬에 적용된다.
  - 가로챈 쪽의 `MoveFailure`를 다시 판정하지 않는다.
- **친구에게 물을 것(upstream 1.17.0에도 있는 결함, upstream대로 둠)**
  1. 더블배틀에서 가로챈 멀리짖기 → `make hns`에서 assert 화면.
  2. 탈출버튼·탈출팩으로 들어온 포켓몬이 반사·가로채기 → 그 배틀 끝까지 기술마다 assert 화면. 1줄 가드가 있지만 동작이 바뀐다. 2를 더 중요하게 본다.
  3. (seq 128) 녹화 배틀 플레이어 뒷모습 Brendan/May → Gold/Kris로 바꿀지.
- 실기 확인 추가: 결과 문서 1~9
  - 매직미러(나츠메·추)·매직코트·가로채기 문장과 연출
  - 습기
  - 반사자 목스프레이
  - 반사자의 다음 행동 방어 판정

## 2026-10-03 — seq 128 #9475 완료 (데스크탑)

- **다음 시작 seq: 129** (#9674 `U-magicbounce-9674` Magic Bounce / Magic Coat / Snatch refactor, L, korean_touch Y). 그 뒤로 seq 132 #9680(XL), 135 #9709, 136 #9711, 138.5 #8943(XL)이 있다. 선진행한 seq 130·131·133·134·137·143·148·149·151·156·157·158·160·165는 "이미 적용(선진행)"으로 처리한다.
- seq 128 #9475(`06c6bac8c2`): 트레이너 앞·뒤 그림 표를 `gTrainerPicInfo`(213칸)로 합치고 `TrainerPicID`의 FRONT/BACK 구분을 없앴다. HnS 앞모습 55개와 뒷모습 4개(GOLD·KRIS·SILVER, 목호 → `CHAMPION_LANCE_HNS`)를 같은 그림·팔레트로 옮겼다. HnS 플레이어 분기는 `GetPlayerTrainerPic`(`src/trainer.c`) 한 곳에 모았다.
- 결과 문서: `docs/friend-handoff/results/1.17.0-port/full-sync-seq-128-128.md`. 그림 데이터 비교 도구: `results/1.17.0-port/trainerpic-9475/`(README에 재실행 방법).
- 진행: 영역별 병렬 사전 분석 4개 → 적용 1커밋 → 영역별 병렬 리뷰 3개(**전부 문제 없음**). 결과 문서만 리뷰 지적대로 고쳤다.
- 검증
  - 그림 데이터: 이식 전후 ELF 내용 비교 차이 0. 리뷰 A가 소스·ROM으로 따로 224개를 대조해도 차이 0이었다.
  - 빌드(데스크탑): 종료 코드 0, ROM 32,715,956 B(+160 B), EWRAM 248,936 B, IWRAM 25,516 B, SHA1 `4b3b96bf…`. 새 경고 0.
  - 변경 없음: 한글 줄·config·기존 파일 모드 변경 0, 세이브 영향 없음(그림 번호를 숫자로 저장하는 곳 없음).
  - 테스트: 전체 목록이 `test-baseline-seq127.txt`와 바이트 동일(PASS 2,335 / TOTAL 5,253). 새 기준 목록 `test-baseline-seq128.txt`(내용 같음).
- **친구에게 물을 것:** 녹화 배틀 재생의 플레이어 뒷모습이 이식 전과 같이 Brendan/May다(`#if IS_HNS`로 유지). upstream처럼 Gold/Kris로 바꿀지 묻는다. 한 줄 변경이고, 분석은 HnS가 빠뜨린 줄로 보고 변경을 권한다.
- 별건(기록만): 기본 `make`(Emerald)·FRLG 빌드는 이식 전부터 HnS 전용 `FLAG_DEFEATED_RED`(`pokemon.c`, `party_menu.c`) 때문에 실패한다.
- 실기 확인 추가: 결과 문서 "실기 확인 항목" 1~15.
  - _HNS 트레이너 앞모습·머그샷
  - Gold/Kris 뒷모습
  - 파트너 목호·실버
  - 녹화 배틀
  - 트레이너 카드·포켓기어·전당·프런티어·트레이너힐·유니언룸

## 2026-10-03 — seq 127 친구 답 반영: HnS 보호 수정 2건 (데스크탑)

- **다음 시작 seq: 128** (#9475 `U-trainerpic-9475` Refactor/trainer pic info, XL 단독). 선진행한 seq 130·131·133·134·137·143·148·149·151·156·157·158·160·165는 "이미 적용(선진행)"으로 처리한다.
- 친구 답(HANDBACK_2026-10-01 3절): [`docs/friend-handoff/FRIEND_REPLY_2026-10-03.md`](../friend-handoff/FRIEND_REPLY_2026-10-03.md). seq 127 결과 승인. 가방 도구 마비는 `몸저림` 유지, 소란 status1은 기록만.
- **upstream 1.17.0에도 있는 기존 버그에 대한 HnS 보호 수정(#9655 커밋과 분리)**
  - `2cba47a010`: 모든 상태를 고치는 도구로 헤롱헤롱만 풀 때 기존 헤롱헤롱 문장이 나온다(`ItemHealMonVolatile`, `// HnS:`). 다른 선택 순서는 그대로다.
  - `fdc110d528`: 잠든 대기 포켓몬에게 잠깨는약·만병통치제를 써도 `gBattleMons[4]`에 쓰지 않는다(`BS_ItemCureStatus`, `// HnS:`, 1줄).
- 검증
  - 빌드(데스크탑): 종료 코드 0, ROM 32,715,796 B(+32 B), EWRAM 248,936 B, IWRAM 25,516 B, SHA1 `817f500d…`. 새 경고 0. 한글 줄 변경 0.
  - 툴체인: 데스크탑의 seq 127 빌드 SHA1은 `f12d5c8e…`다(노트북 `a6ad839c…`와 다름, 툴체인 차이. ROM 크기는 같음).
  - 임시 테스트(커밋 안 함): 4개 PASS. 헤롱헤롱 수정을 빼면 혼란 문장이 나와 FAIL한다.
  - 전체 테스트: `test-baseline-seq127.txt`와 바이트 동일(PASS 2,335 / TOTAL 5,253).
- 결과 기록: `full-sync-seq-127-127.md` "친구 답"·"HnS 보호 수정", `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 가방 도구 행, `HANDBACK_2026-10-01.md` 5절.
- AI mGBA MCP는 시험 도입만 유지한다(데스크탑, 첫 시험 seq 107, 아직 설치 안 함). 친구 로컬 커밋은 full-sync가 끝날 때까지 건드리지 않는다.

## 2026-10-01 — seq 127 #9655 unit 완료 (노트북)

- **다음 시작 seq: 128** (#9475 `U-trainerpic-9475` Refactor/trainer pic info, XL 단독). 그 뒤 seq 129 #9674(L), seq 132 #9680(XL), seq 138.5 #8943(XL)이 이어진다. 선진행한 seq 130·131·133·134·137·143·148·149·151·156·157·158·160·165는 "이미 적용(선진행)"으로 처리한다. 일부 줄만 선반영된 seq 390 #10223·seq 298 #10445는 그 줄만 빼고 정상 검토한다.
- **친구 답장(2026-10-01):** [`docs/friend-handoff/FRIEND_REPLY_2026-10-01.md`](../friend-handoff/FRIEND_REPLY_2026-10-01.md). D1~D7 권장안 확정, 선진행 중단, #9006 문장 확정, AI mGBA MCP 시험 도입 승인(데스크탑, 첫 시험 seq 107 #7305, 아직 설치 안 함). 친구는 full-sync 완료까지 로컬 커밋(`dac422efb3` 포함)을 보존하고 pull·push하지 않는다. **full-sync가 끝나면 완료 메시지와 최신 handback을 보내야 한다.**
- 회신: [`docs/friend-handoff/HANDBACK_2026-10-01.md`](../friend-handoff/HANDBACK_2026-10-01.md). 결과: [`full-sync-seq-127-127.md`](../friend-handoff/results/1.17.0-port/full-sync-seq-127-127.md), 분석·리뷰 원문 `results/1.17.0-port/seq127-9655/`.
- 커밋: #9655(+#9856 흡수) `f3b491dfc4`, #10064 `47515a949e`. #10149는 HnS 동등.
- 결정 반영: D1 빗나감 통일(`resultmessage` HnS 1줄), D2·D3 기존 한글, D4 출력 변화 수용·재번역 0, D5 새 문장 14개 기존 본문 재사용, D6b 멘탈허브 이름(스크립트), D6a·D6c 미착수, D7 소란 기상 EFF. REPORT 함정 6개 준수.
- HnS 보호 줄: 힐볼 포획 `MULTISTRING` 저장·복원, `HealStatusConditions()` 범위 밖 읽기 방지(둘 다 upstream 1.17.0에도 있는 문제).
- 빌드(노트북, `47515a949e`): 종료 코드 0, ROM 32,715,764 B(97.50%, +272 B), EWRAM 248,936 B, IWRAM 25,516 B, SHA1 `a6ad839c…`. 새 경고 0. 한글 소스 줄 변경은 결정분 18줄뿐.
- 테스트: PASS 2,335 / FAIL 2,255 / KNOWN_FAILING 10 / TOTAL 5,253. 사라진 PASS 4건(레이징불 중복 3건은 `break_screens.c` 짝이 PASS, steadfast 영문 문구 1건)은 설명됨. 새 FAIL 28건은 모두 영문 `MESSAGE`. 새 기준 목록 `test-baseline-seq127.txt`.
- 리뷰 4개: 수정 필요 3건(범위 밖 읽기 1건이 두 리뷰에서 중복, 출력 변경 문서 1건) 반영. 경미는 기록.
- **친구 답 대기(진행은 막지 않음):** 가방 도구 마비 표기(`몸저림` 유지), 만병통치제·회복약 헤롱헤롱 → 혼란 문장(upstream 결함) 수정 여부, 이식 전부터 있던 `BS_ItemCureStatus` 대기 포켓몬 `gBattleMons[4]` 쓰기 수정 여부.
- 친구 mGBA 확인 대기: seq 127 항목 1~14, 이전 HANDBACK_2026-09-30 항목.
- 노트북 스크래치 `/home/jinmo/hns-sync-work/chunk-127/`(patch·로그 사본)는 저장소 밖이다. 필요한 문서는 저장소 `seq127-9655/`에 복사했다.

## 2026-09-30 — #9655 대기 중 선진행 구간 1 완료 (현재)

- **순서표와 다르게 진행했다.** seq 127 #9655가 D1~D7 결정 대기라서, #9655와 무관한 뒤쪽 행을 앞당겨 이식한다(사용자 승인). 목록·기준·재생성 방법: `docs/friend-handoff/results/1.17.0-port/ahead-of-9655/README.md`, 전체 분류 `ahead_candidates.tsv`. 선진행 대상은 113행이다(처음 114행에서 #9819를 뺌).
- **순서표 기준 다음 시작 seq는 여전히 127(#9655)이다.** D1~D7 답을 받으면 선진행을 멈추고 seq 127로 돌아간다. 선진행한 행은 그 seq에 닿으면 "이미 적용(선진행)"으로 처리한다.
- **선진행 다음 구간: seq 186 #9890부터**(사용자 지시 대기). 후보 순서는 `ahead_candidates.tsv`의 class `선진행` 행이다.
- 구간 1 결과: `docs/friend-handoff/results/1.17.0-port/full-sync-ahead-seq-130-167.md`. 14행 적용, 29커밋(`0f60183f1f`~`c1f24e6121`) + 메인 정리 커밋.
  - 적용: #9575(예측 AI 통합), #9462(잔여분), #9006(기술 떠올리기 개편), #9903(일부), #9713(디버그 사운드 B안), #9721, #9690, #9461(맵 팝업 층 번호, 적응 3곳), #9755, #9774, #8628, #9765, #9762, #9813.
  - **seq 167 #9819는 보류해 seq 446 #10548 직전에 넣는다.** #9819만 넣으면 그때까지 파티 메뉴에 안개제거·록클라임이 회색 항목으로 나오고, 공중날기·플래시 자동 항목이 밀릴 수 있기 때문이다.
  - 메인 결정: #9713 곡 이름 저장 안 함(B안, ROM −1.3 KB). #9006은 #10223의 1줄 선반영(떠올리기 거절 때 하트비늘 유지), 가르침 교체 때 최대 PP 유지(`// HnS:`), 새 문자열 2개는 한글 초안으로 넣고 미결에 올렸다.
- 검증
  - 커밋 리뷰 4개(14커밋): 수정 필요 0, 경미 1(#9006 디버그 메뉴 영문 선택지, upstream대로 두고 기록).
  - 한글 줄: 새 문자열 2줄만 바뀌었다. 기존 한글·모드·config 변경은 0이다.
  - 빌드: ROM 32,715,492 B(−7,360 B), EWRAM 248,936 B(−8 B), IWRAM 25,516 B, SHA1 `a1aa5ea8c9c824ba1c6bfa4d068f8948392a2a8c`. 새 경고 0.
  - 테스트: PASS 2,332 / TOTAL 5,241, 사라진 PASS 0. #9462로 FAIL → PASS 3건이 생겼다. 새 기준 목록은 `test-baseline-seq167-ahead1.txt`이고, seq 127로 돌아갈 때도 이 목록을 기준으로 쓴다.
- 후속 처리
  - seq 390 #10223의 1줄, seq 298 #10445의 박스 번호 줄은 이미 적용됐다.
  - seq 144 #7573 때 HnS `IsBoxMonExcluded`를 유지한다.
- 실기 확인 추가(결과 문서 "실기 확인 항목" 1~8): 예측 AI 트레이너, 검은먹시티 떠올리기(하트비늘·최대 PP)와 가르침 NPC 최대 PP, 디버그 사운드 메뉴, 야드파운드법 도감 무게, 맵 팝업, 도감 분포 화면, 편지, 3세대 배틀 UI.

## 2026-09-30 — full-sync port seq 121~126 완료

- **다음 시작 seq: 127 (#9655 배틀 메시지 리팩터, XL).** 시작 전에 D1~D7 결정이 필요하다(`docs/friend-handoff/HANDBACK_2026-09-30.md` 9절, 사전 조사 `docs/friend-handoff/results/1.17.0-port/pre-9655/REPORT.md`). 결정을 받으면 현재 HEAD 기준으로 dry-run과 분석을 다시 한다.
- seq 121~126: #9594+#9796(얼루기 점, y=25 유지), #9425(상점, `pokemart 0` 적응), #9624(장식), #9667(기술 설명, HnS 문구 유지), #9616(소란 `GEN_4`), #9668(이미 적용). 병렬 리뷰 결과는 모두 문제 없음. seq 163 #9796은 이미 적용으로 처리한다.
- 빌드: ROM 32,722,852 B(97.52%), EWRAM 248,944 B, IWRAM 25,516 B. 테스트는 사라진 PASS 0이고 기준 목록은 `test-baseline-seq126.txt`다.

## 2026-09-30 — full-sync port seq 120 (#9657) 완료 (현재)

- **다음 시작 seq: 121.** 예정: seq 121~126(seq 126 #9668은 이미 적용) → **seq 127 #9655 배틀 메시지 리팩터. 시작 전에 D1~D6 결정이 필요하다**(`docs/friend-handoff/HANDBACK_2026-09-30.md` 9절, `docs/friend-handoff/results/1.17.0-port/pre-9655/REPORT.md`).
- seq 120 #9657(`aa175914e9`): `BattleCalcValues` 전환과 `DamageContext` 이름 되돌리기. HnS 고유 분기를 보존했고, 영역별 리뷰 4개 모두 문제 없음. 프리폴 무게 출력 변화 1건(1.17.0 동작).
- 빌드: ROM 32,719,060 B(97.51%), EWRAM 248,940 B, IWRAM 25,516 B. 한글 변경 0. 테스트 목록은 seq119와 같다(`test-baseline-seq120.txt`).
- 친구 mGBA 확인 대기: Safari 재확인, #9525·#9124·#9551, seq 108~119, seq 120(프리폴·메가솔·잠자기·관통드릴).

## 2026-09-30 — full-sync port seq 108~119 완료 (현재)

- **다음 시작 seq: 120** (#9657 `U-calcvalues-9657` BattleCalcValues, XL 단독). 그다음 예정: seq 121~126(seq 126 #9668은 이미 적용) → seq 127 #9655 배틀 메시지 리팩터(XL, 한글 영향 큼).
- 구간 결과: `docs/friend-handoff/results/1.17.0-port/full-sync-seq-108-119.md`. 진행: 병렬 사전 분석 4개 → 적용 1개 → 병렬 리뷰 3개(9커밋, 전부 문제 없음) + 메인 확인 3커밋.
- 커밋: #9537(+#9668) `70eb6a4271`, #9241 `ec9dca2712`, #9610 `47cd51facf`, #9587 `2d86edca81`, #9596 `f6ef307f76`, #9532 `7e4f61c927`, #9494 `3f7f0ddabf`, #9864(seq 176 선반영) `489c58259c`, #9557 `4440c18163`, #9578 `3960fc0c8e`, #9630 `4e3c6bd5e7`, #9634 `de9b581a28`, 결과 `ef460923b9`. seq 110은 이미 적용이었다.
- 뒤 행 처리: seq 126 #9668·seq 250 #10281은 "HnS 동등", seq 176 #9864는 "이미 적용". seq 166 #9784는 선행(#9717 seq 150, #8943 seq 138.5)이 없어 넣지 않았다.
- HnS가 upstream과 다르게 둔 곳: #9557 미리보기 그림·표 가드(upstream 그대로면 ROM +110 KB·Emerald 빌드 실패), #9532 `animTurn = 1`(방출 연출 유지), #9494 원시 날씨 해제 2줄(`@ HnS:`), #9610 `AccuracyCheck`의 #9929 분기 유지, #9578 ShedSkin hunk 제외, #9596 `turnOrder` hunk 제외.
- upstream대로 둔 출력·동작 변화(`BATTLE_MESSAGE_OUTPUT_CHANGES.md` 5행 추가): 대타출동 상대에게 폴터가이스트를 쓰면 도구 문장이 나오지 않음, 참기 2·3턴째 문장 없음, 탈출버튼·탈출팩·유턴류 교체 순서(발동 문구 → 볼 회수 → 남은 효과 → 교체), 레드카드 보유자의 위기회피 발동, 목스프레이·허탕보험 등 시점. #9557 비 오는 날 BG 팔레트 13 창 색, #9587 교체 AI 경계 사례.
- 한글: 소스에서 한글이 든 줄 변경은 `src/battle_message.c` 세 곳뿐이다(#9610 토큰 교체 2쌍, 본문 바이트 동일 / #9578 `STRINGID_PKMNSXCUREDYPROBLEM`·#9634 `STRINGID_PKMNISGLOWING` 미사용 문장 삭제). 남은 STRINGID 722개 문자열 바이트 동일.
- 빌드(메인 재링크): 종료 코드 0, ROM 32,718,964 B(97.51%, +2,880 B, 주로 #9557), EWRAM 248,940 B(+16 B, #9494 `SpecialStatus`), IWRAM 25,516 B. 새 경고 0.
- 테스트: PASS 2,321 / FAIL 2,243 / KNOWN_FAILING 10 / TOTAL 5,229, assertion·Killed 0. 기준 목록 `test-baseline-seq119.txt`. seq107 대비 PASS 손실 4건은 모두 설명된다(이름 변경 2, upstream 표시와 같은 KNOWN_FAILING 2 — `Eject Button … before Red Card`는 seq 166 #9784, `Blunder Policy … Dragon Darts`는 seq 467 #9841에서 풀림).
- **후속 검토(결정 대기 아님, 기록):** upstream #9494 이후 더블배틀 시작 때 둘째 칸의 기절 포켓몬 특성이 적용되는 잠재 문제(1.17.0에도 있음). 사용 가능한 포켓몬이 한 마리뿐인 더블배틀에서만 생긴다. HnS 가드를 넣을지는 나중에 정한다.
- 실기 확인 추가: 배틀프런티어 트레이너 그림, 내던지기·폴터가이스트(대타출동 상대 포함), 참기 방출 연출, 탈출버튼·레드카드·위기회피·유턴·탈출팩 순서와 원시 날씨 해제 문구·체력 상자, 문 출입·동굴 진입·비 날씨 맵의 창 색.

## 2026-09-30 — full-sync port seq 107 (#7305 나무열매 개편) 완료

- 다음 시작 seq(당시): 108 (#9537 FRLG 오브젝트 그래픽 이름, M).
- 구간 결과: `docs/friend-handoff/results/1.17.0-port/full-sync-seq-107-107.md`. 커밋 `f2a0395e90`(#7305, 41파일, 새 파일 `include/constants/berries.h`), 결과 `2b5ec2bb98`. 진행 방식: 병렬 사전 분석 4개(세이브·C·스크립트·테스트) → 적용 에이전트 1개 → 병렬 리뷰 3개(전부 문제 없음) → 메인 확인.
- **세이브 호환:** `FOREACH_BERRY`를 HnS 아이템 순서로 두었다(upstream과 두 곳만 다름: CHILAN=36, ROSELI=53). 열매 68종 번호·경계 상수가 이식 전 `ITEM_TO_BERRY`와 같고, `berry.c`의 `STATIC_ASSERT` 가드가 순서를 고정한다(upstream 순서로 바꾸면 컴파일 오류). 세이브 구조체 레이아웃 diff 0, 새 게임 나무 118그루(473바이트) 전후 동일.
- HnS가 upstream과 다르게 둔 곳(`// HnS:`): 열매 순서, 순서 가드, `berry_blender.c` NPC 열매 세트(upstream 그대로면 버치~배리열매를 넣을 때 NPC가 같은 열매를 넣는 회귀, 1.17.0에도 남음). HnS 수확량 43종·IS_HNS 성장/재식재·나무 팔레트·태그 화면 단위계 코드 보존. `include/random.h` hunk 제외.
- 그대로 둔 upstream 변화(정상 플레이에서 닿지 않음): 무효 나무 값 → `ITEM_NONE`, E-Reader 의문열매 자연의은혜 0/0, 자뭉열매 표시, 구버전 ROM과의 베리 크래시·도도리오 통신 번호 1칸 차이.
- 빌드(메인 재링크, 노트북): 종료 코드 0, ROM 32,716,084 B(97.50%, −176 B), EWRAM 248,924 B, IWRAM 25,516 B. 새 경고 0. 한글이 든 소스 줄 변경 0.
- 테스트: 이식 전후 모두 PASS 2,314 / FAIL 2,232 / TOTAL 5,211, 목록 바이트 동일(회귀 0). 기준 목록 `test-baseline-seq107.txt`(seq106과 같음).
- 세이브 확인 도구: `docs/friend-handoff/results/1.17.0-port/berry-7305/`(`verify.sh` 4항목, `sav_berry_trees.py`, `sav_set_tree.py`, README).
- 실기 확인 추가: 기존 세이브의 36~65번 나무(카리·오카·바리비·로셀·치리·의문·애터열매, 130번 수로(Route130) 치리열매 treeId 82), 조토·관동 나무 단계별 그림·팔레트, 수확 뒤 재식재, 금빛시티 꽃집, 열매 태그 화면(번호·그림·미터법 슈박열매), 가방 열매 번호, 자연의은혜, 블렌더 NPC.

## 2026-09-30 — full-sync port seq 102~106 완료

- 다음 시작 seq(당시): 107 (#7305 `U-berries-7305`, XL 단독). 세이브 호환 주의: 나무열매 ID를 재구성해도 기존 열매 번호 순서를 유지해야 한다(full-sync plan 4절).
- seq 102 #9172: ROM SHA1이 변환 전과 같다. seq 103~106: #9568(롤 config 전부 MEDIAN, 롤 동작이 이식 전과 같음), #9551, #9579(이미 적용), #8213.
- 빌드: ROM 32,716,260 B(97.50%), EWRAM 248,924 B, IWRAM 25,516 B. 테스트: PASS 2,314 / FAIL 2,232 / TOTAL 5,211, 회귀 0. 기준 목록은 `test-baseline-seq106.txt`다.
- 친구 mGBA 확인 대기: Safari 재확인, #9525 발버둥, #9124·#9551 교체 AI(`HANDBACK_2026-09-30.md`).

## 2026-09-30 — full-sync port seq 102 (#9172) 완료 (현재)

- **다음 시작 seq: 103.** seq 102는 애니 스크립트 16진수 → 10진수 기계 변환이다(`b72fd0c63d`). ROM SHA1이 변환 전과 같아서(`3613568d…`) 동작 변화가 없다. 테스트 기준은 `test-baseline-seq101.txt` 그대로다.
- 미리 알림: seq 497 #9924는 #9986(seq 516)의 애니 매크로가 먼저 필요하다. 그 시점에 처리 방식을 정한다(`full-sync-seq-102-102.md`).

## 2026-09-30 — full-sync port seq 92~101 완료, Safari UI 수정, 순서표 보정 (현재)

- **다음 시작 seq: 102** (#9172, XL 단독). 순서표에 외부결정 6건이 소수 번호로 들어갔다(138.5, 186.5, 385.5, 434.5, 471.5, 475.5). #10299·#10310·#10282는 475.6~475.8로 옮겼다.
- 구간 결과: `docs/friend-handoff/results/1.17.0-port/full-sync-seq-092-101.md`. 이식 7건, 이미 적용 3건, 병렬 리뷰 전부 문제 없음.
- 빌드: ROM 32,714,868 B(97.50%), EWRAM 248,924 B(94.96%), IWRAM 25,516 B(77.87%). 한글 문자열 변경 0.
- 테스트: PASS 2,306 / FAIL 2,232 / TOTAL 5,203. 기준 목록은 `test-baseline-seq101.txt`, 회귀 0.
- Safari UI 한글화 버그 2건 수정(port와 무관, merge `4160fc31f0`). 친구 mGBA 재확인 대기.
- 친구 실기: seq 1~62 PASS(`mgba-check-seq001-062.md`). seq 92~101 확인 요청은 `docs/friend-handoff/HANDBACK_2026-09-30.md`에 있다.

## 2026-09-29 — full-sync port seq 91 (#9507 Species enum) 완료 (현재)

- **다음 시작 seq: 92** (#9429 Encore 타이머, M). 다음 구간 예정: seq 92~101(가중치 23, seq 93·94·98은 이미 적용) → seq 102 #9172(XL 단독).
- 구간 결과: `docs/friend-handoff/results/1.17.0-port/full-sync-seq-091-091.md`. 커밋 #9507 `b0a0fb9033`, #9558(seq 93, 같은 unit) `8de47b965d`, 결과 `3f6ccb8a6e`. seq 93은 도달하면 "이미 적용"으로 처리한다.
- 검증(에이전트 기록, 메인 일부 재확인):
  - 종 ID 값 표: C·asm 경로 모두 diff 0(공통 이름 1,675개 + 파생 상수 4개). 새 이름은 `SPECIES_CUSTOM_START`(1572)·`SPECIES_CUSTOM_END`(1573)뿐이고 `SPECIES_EGG`·`NUM_SPECIES`는 1573 그대로다.
  - 세이브 구조체: DWARF로 구조체·공용체 633종의 오프셋·크기·비트필드를 비교해 차이 0(`SaveBlock1/2/3`·`PokemonStorage`·`BoxPokemon`·TV·릴리코브·프런티어·어프렌티스·데이케어·로밍·트레이너 힐·메일·WonderCard 포함). 기존 `STATIC_ASSERT(sizeof…)` 11개 통과.
  - 코드: 오브젝트 1,524개 중 1,478개 바이트 동일. 달라진 C 함수 170개는 `__LINE__` 상수·16비트 절단/확장 명령과 그에 따른 레지스터 차이다.
- HnS가 upstream과 다르게 둔 곳: `field_effect.c` `InitFieldMoveMonSprite`는 `u32` 유지(bit 31에 울음소리 "배경음 안 줄임" 플래그를 실어 오는데 16비트 enum이면 잘린다, upstream 1.17.0 회귀). HnS에서 이 플래그를 쓰는 경로는 록클라임이다(파도타기는 HnS에서 포켓몬 표시가 꺼져 있다). `RemoveSpeciesFromIconList`는 아이콘 키라 `u16` 유지. HnS 전용 함수는 `u16` 유지.
- 빌드(메인 재링크): 종료 코드 0, ROM 32,713,060 B(97.49%, +448 B), EWRAM 248,924 B, IWRAM 25,516 B. 새 경고 0. 한글이 든 소스 줄 변경 0.
- 테스트: PASS 2,298 / FAIL 2,229 / TOTAL 5,197, 목록이 `test-baseline-seq090.txt`와 바이트 동일(회귀 0). 새 기준 목록 `test-baseline-seq091.txt`. 테스트 빌드의 `-Wenum-conversion` 오류 2곳(`test/battle/capture.c`, `mummy.c`)은 upstream 후속 병합과 같은 형태로 고쳤다.
- 실기 확인 추가: 이식 전 ROM(`9dcd5c15…`) 세이브를 불러와 파티·PC 박스·도감(HGSS 진화·분포), 데이케어 포켓몬·알과 부화·향로 아기·볼트태클 유전, 로밍 포켓몬, TV·릴리코브·비밀기지·프런티어·어프렌티스 기록과 메일 아이콘, DexNav, 록클라임 울음소리(배경음을 줄이지 않음), `givemon`/`giveegg`·이름 짓기·진화·폼체인지·다이맥스.

## 2026-09-29 — full-sync port seq 84~90 완료

- 다음 시작 seq(당시): 91 (#9507).
- 구간 결과: `docs/friend-handoff/results/1.17.0-port/full-sync-seq-084-090.md`. 커밋 #9376 `6e050d6e70`, #9466 `a4c46a4a8f`, #9510 `7a6cd5b51b`, #9135 `fd549f70bc`, #9460 `d81b37f15b`, #9514 `f0c3349daf`, #9539 `7b578d7bb2`, 결과 `2ecd462ad9`. 구간 밖 행은 넣지 않았다.
- 빌드(메인 재링크 확인): 종료 코드 0, ROM 32,712,612 B(97.49%, +64 B), EWRAM 248,924 B(94.96%), IWRAM 25,516 B(77.87%). 새 경고 0.
- 한글: 한글이 든 소스 줄 변경은 `src/battle_message.c` 22쌍(#9514)뿐이다. `{B_DEF_*}/{B_ATK_*}` → `{B_EFF_*}/{B_SCR_*}` 토큰 교체 외에는 바이트가 같고, 에이전트 기록상 가리키는 배틀러가 달라진 출력 경로는 0건이다.
- #9514 참고: upstream이 `STRINGID_PKMNALREADYASLEEP`·`PKMNALREADYPOISONED`·`PKMNISALREADYPARALYZED`에서 `{B_DEF_…}`를 `{B_SCR_…}`로 바꾼 3건은 HnS에서 `B_DEF`로 유지했다. SCR로 바꾸면 공격자 이름이 나오는 upstream 버그이고 #10064(seq 206)가 고친다. g1 plan대로 #10064 때는 엔진 1줄만 넣고 문장은 그대로 둔다.
- 테스트: 인터넷 문제로 에이전트의 구간 끝 실행이 중단돼 메인이 재개 뒤 다시 돌렸다. PASS 2,298 / FAIL 2,229 / TOTAL 5,197, 목록이 `test-baseline-seq083.txt`와 바이트 동일(회귀 0). 새 기준 목록: `results/1.17.0-port/test-baseline-seq090.txt`. AI 더블 테스트 3건은 #9460 뒤에도 FAIL이다(seq 131 #9462에서 재확인).
- 목록 추출은 `LC_ALL=C`와 `grep -a`로 한다. UTF-8 로케일에서는 한글 바이트가 붙은 "~ fit on ~" 23개 줄이 빠진다(`PORT_INSTRUCTIONS.md` "테스트" 절에 명령 기록).
- 실기 확인 추가: #9514 조이기 계열 10개 문장의 이름·조사, 소란피기·불사르기·전광쌍격, 페인트·이차원러시 방어 해제 문구, 불꽃튀기기 파트너 이름, 소금절이·시럽봄, 방벽·오로라베일 문구, 거다이골드러시 돈 문구(플레이어 편만). #9510(선택) 만능우산과 날씨 특성.

## 2026-09-29 — full-sync port seq 83 (#8497 loadspritegfx 제거) 완료 (현재)

- **다음 시작 seq: 84** (#9376 `U-cleanup-9376`, M). 다음 구간 예정: seq 84~90(가중치 25) → seq 91 #9507(XL 단독). **사용자 지시로 구간 3 시작 전 대기 중**이다.
- 구간 결과: `docs/friend-handoff/results/1.17.0-port/full-sync-seq-083-083.md`. 커밋 `f59f50ca17`(#8497), `5121b83c94`(#9595, seq 110), `45b27b9bce`(#10345, seq 265), `715c1b91fc`(#10589, seq 321). 뒤 세 행은 group plan의 "#8497과 같은 unit"에 따라 함께 넣었으므로 도달하면 "이미 적용"으로 처리한다.
- HnS 적응: HnS 애니 스크립트 전체를 #8497판 변환 스크립트로 기계 변환(`loadspritegfx` 2,351줄 삭제, `unloadspritepal` 155줄 추가)했고, HnS 스크립트 고유 차이 4종을 보존했다. upstream 1.17.0에도 남아 있는 팔레트 회귀 3곳(방울소리 음표 팔레트 버퍼 밖 쓰기, 음표 무지개 블렌드 첫 색 누락, GUST 팔레트 미로드)을 `// HnS:` 주석과 함께 막았다.
- 태그 검증: 이식 전 로드 태그 304종·2,452쌍 중 자동 로드 해석 실패 0, HnS 전용 태그 0. 애니 스크립트 정적 검사(호출 깊이 2, 호출 안 `end`·빈 스택 `return` 0) 전후 동일.
- 빌드(`b5d3881de3`, 메인 재링크 확인): 종료 코드 0, ROM 32,712,548 B(97.49%, −4,624 B), EWRAM 248,924 B(94.96%, +16 B), IWRAM 25,516 B. 새 경고 0. 한글이 든 소스 줄 변경 0.
- 테스트: PASS 2,298 / FAIL 2,229 / TOTAL 5,197. seq082 대비 PASS 손실 0. 새로 열린 `test/battle/move_animations/all_anims.c` 가운데 `Move Animations work 1`·`2`는 FAIL이다. 쪼아대기·내던지기·벌레먹음에서 HnS 도구 팝업 태스크가 테스트 배틀 종료 시점에 남기 때문이다(테스트 환경 차이, 애니 로드 검사는 통과). 기준 목록: `results/1.17.0-port/test-baseline-seq083.txt`. AI 더블 테스트 3건(아래 seq 63~82 항목)은 그대로다.
- 실기 확인 추가: 방울소리·노래하기·풀피리·돌림노래·배수의진·옛날노래 음표/이펙트 색, 바람일으키기·날개치기·폭풍 등 회오리 색 순환, 매지컬리프·깜짝베기·오로라빔, 메가진화·원시회귀·날씨 입자, 1000만볼트·나인에볼부스트, 에스퍼윙(마지막 사이코커터가 이제 보임), 콘테스트 기술 애니. 상세는 결과 문서.

## 2026-09-29 — full-sync port seq 63~82 완료

- 다음 시작 seq(당시): 83. 구간 결과: `docs/friend-handoff/results/1.17.0-port/full-sync-seq-063-082.md`
- 구간 밖이지만 unit 구성원이라 먼저 넣은 행: seq 94 #9549, 98 #9564, 138 #9707, 319 #10573, 330 #10648(기존 329 #10647 포함). 도달하면 "이미 적용"으로 처리한다.
- 빌드(`b42872eba2`, 노트북 WSL, ARM 13.2.1, 메인이 재링크해 확인): 종료 코드 0, ROM 32,717,172 B(97.50%, 구간 시작 대비 −22,048 B, 주로 #9086 문자열 병합), EWRAM 248,908 B(94.95%, +16 B), IWRAM 25,516 B(77.87%). 새 경고 0.
- 한글: 한글이 든 소스 줄 변경은 #9086(`challenge_menu.c` 1쌍, `contest.c` 3쌍)과 #9051(`strings.c` → `src/data/script_menu.h` 이동)뿐이다. 메인 대조 결과 제거된 한글 리터럴 48종이 모두 그대로 다시 나타나고, 새 리터럴 7종(`슈퍼`·`하이퍼`·`마스터`·`싱글`·`더블`·`멀티`·`통신 멀티`)은 `strings.c`에 남아 있는 기존 문구의 인라인 사본이다. #9086 preproc 문자열 바이트 비교(C 1,338파일)·#9051 스크립트 메뉴 676항목 비교 모두 불일치 0.
- #9066: OBJ_EVENT_GFX·multichoice·facility class 값 표가 전후 C·asm 양쪽 diff 0이고 ROM이 바이트 동일했다. multichoice 값은 실제로 0~177 연속이다(이전 보고의 "값 틈"은 주석 붙은 줄을 추출에서 놓친 착오).
- 테스트(`make check BUILD=hns -j8`): PASS 2,295 / FAIL 2,226 / TO_DO 618 / TOTAL 5,191, assertion·crash 0. 기준 목록: `results/1.17.0-port/test-baseline-seq082.txt`.
  - **확인 대기 3건:** #9451 이후 AI 더블 테스트 `AI_FLAG_DOUBLE_ACE_POKEMON: Ace mons won't be switched in…`, `Choiced Pokémon won't switch out…`(PASS→FAIL), 새 테스트 `AI can switch out both mons on the same turn…`(FAIL). 원인은 `AI_REVERSE_BATTLER_LOGIC_ORDER_CHANCE 50`(upstream 1.17.0도 50)이 "왼쪽 AI 먼저" 테스트 가정을 깨는 것이다. 값을 0으로 두면 모두 PASS했다(미커밋). upstream은 seq 88 #9460·seq 131 #9462의 `WITH_CONFIG`로 해결하므로, 그 행 이식 뒤 PASS 복귀를 확인한다.
  - 나머지 차이: 새 테스트 14건 PASS, #8664 TO_DO→실제 테스트 4건과 #9249 새 테스트 1건은 영문 `MESSAGE` 불일치로 FAIL.
- 실기 확인 추가: 페이드 unit(맵 전환·날씨·시간대 페이드), #9446 싱크로·치료 열매 발동 순서, #9249 난동 혼란 문구 시점·프리폴 해제, #9142 기술 애니 서브루틴(assert 화면 없음), #9121 더블배틀 대타 표시, #9086 한글 문자열 전반.
- 작업 환경 주의: 이 WSL 셸의 `grep`은 ugrep 래퍼 함수라 파일 인자를 주면 빈 결과가 나올 수 있다. 테스트 목록 비교 등에는 `command grep`을 쓴다.

## 2026-09-29 — full-sync port seq 1~62 완료, 테스트 러너 복구

- **다음 시작 seq: 63** (#9066 `U-enum-9066`, L). seq 329(#10647)는 이미 적용됐다(#9942와 함께).
- 빌드: ROM 32,739,220 B(97.57%), EWRAM 248,892 B(94.94%), IWRAM 25,516 B(77.87%). 한글 문자열 변경 0.
- 테스트: `make check BUILD=hns`를 쓸 수 있다. 기준값은 PASS 2,283 / FAIL 2,218 / TOTAL 5,175(assertion·crash 0)이고, 기준 목록은 `docs/friend-handoff/results/1.17.0-port/test-baseline-seq062.txt`다. 판정은 이식 전후 비교로 한다(`PORT_INSTRUCTIONS.md` "테스트" 절).
- 규칙·프롬프트: `docs/friend-handoff/results/1.17.0-port/PORT_INSTRUCTIONS.md`, `NEW_SESSION_PROMPTS.md`
- 실기 대기: `docs/friend-handoff/HANDBACK_2026-09-29_PORT_PROGRESS.md`의 친구 mGBA 확인 목록

## 2026-09-28 — 포케기어 헤더 51×12 조각 좌표·상단 1px 정렬 최종 보정

- 기존 기록의 좌표 판정은 잘못됐다. 사용자 Aseprite 원본 기준 선택 영역은 위 `(16,6,51×2)`, 가운데 `(69,0,51×8)`, 아래 `(133,0,51×2)`이며, 앞서 기록한 가운데 `x=64`·아래 `x=128` 및 위·아래 반전 설명은 폐기한다.
- `graphics/pokenav/hns/header.png`에서 세 영역을 먼저 추출한 뒤 런타임 타일 2~19·21·22·41에 다시 패킹했다. 위 2행은 가운데 획과 맞도록 합성 위치만 1px 왼쪽으로 보정했다. 구분선용 20번 타일과 `header.bin`의 41번 참조는 보존했으며, 검은 사각형을 만들었던 41→1 임시 변경은 제거됐다.
- 정적 검증: 런타임 역렌더에서 `포켓기어` 51×12 형상을 확인했고, 흰색 구분선은 실제 GBA 표시 폭 0~239px에서 연속이다. 빌드가 생성한 `header.4bpp`와 검증본, `header.bin`과 정상 타일맵이 각각 `cmp` 일치한다.
- 빌드: `build/hns/src/graphics.o`를 강제 재생성한 뒤 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 성공. ROM 32,723,652 B(97.52%), EWRAM 249,112 B(95.03%), IWRAM 25,644 B(78.26%). mGBA 실제 화면은 아직 확인하지 않았다.

## 2026-09-28 — Gen4 배틀 메가진화 아이콘과 레벨 100 겹침 수정

- 원인: `src/battle_interface.c:917`의 Gen4 체력박스 레벨 복사 경로가 메가 아이콘 여백을 위해 `xPos -= 5`를 항상 수행했다. 세 자리 레벨에서는 기본 `xPos`가 0이라 unsigned underflow가 발생해 레벨 타일이 잘못된 위치로 복사됐다.
- 조치: 레벨 자릿수가 3자리 미만일 때만 레벨 텍스트를 5px 왼쪽으로 이동하고, 레벨 100은 SoulGold와 같이 `UpdateIndicatorLevelData()`의 아이콘 -4px 보정만 적용하도록 수정했다. SoulGold의 아이콘 기준 좌표(`sIndicatorPositions`)와 자릿수별 보정 동작은 유지했다.
- 검증: `build/hns/src/battle_interface.o` 컴파일 및 `make hns -j8` 성공(`BUILD_EXIT=0`). ROM 32,723,668 B(97.52%), EWRAM 249,112 B(95.03%), IWRAM 25,644 B(78.26%). 기존 미사용 함수·변수 경고 17건 외에 수정 파일의 새 경고는 없다. 레벨 1·10·99·100 화면은 mGBA에서 직접 확인해야 한다.
- **2026-10-07 정정·대체(`58a6e9649f`):** 이 항목의 원인·수정 설명은 맞지 않게 됐다. 실제 원인은 기믹 아이콘이 있을 때 숫자만 든 `text`를 Gen4 분기가 `text + 2`에 다시 써서 `"5050"`·`"10100"`이 되던 것(`88cd5dc06e`, 2026-05-09부터)이고, Lv.10~99는 `5[아이콘]50`, Lv.1~9는 `9[아이콘]`으로 이 수정 뒤에도 남아 있었다. 이제 Gen4 UI는 숫자만 한 번 써서 24px 레벨 창에 오른쪽 정렬(`24 - GetStringWidth()`)하고 "두 자리 이하 5px 왼쪽" 규칙은 없앴다. 아이콘 보정(Lv.100 −4, Lv.1~9 +5)과 `sIndicatorPositions` 설명은 그대로 맞다. 결과 `docs/friend-handoff/results/1.17.0-port/full-sync-hnsfix-2026-10-07.md`.

## 2026-09-28 — 전체 엔진 동기화 1단계 완료: 이식 계획 확정 (현재)

- 남은 594개 PR의 판정: 이식 517(+테스트 러너 복구), 동등 56, 무관 15, 외부결정 6. 이식 순서는 `docs/friend-handoff/results/1.17.0-sync-plan/port_sequence.tsv`, 요약은 `results/pokeemerald-expansion-1.17.0-full-sync-plan.md`에 있다.
- 결정 완료(갱신 2026-09-29): #8943 녹화 배틀 형식은 **A안으로 확정**됐다. upstream 1.17.0 구조를 쓰고, 기존 HnS 녹화 배틀 무효화를 허용하며, 일반 게임 세이브 호환은 유지한다(`CLAUDE_FULL_SYNC_PORT_PROMPT.md` 4절, full-sync plan 5절). 나머지 5건도 권장안으로 확정됐다.
- 실기 대기: `7e7c38ab10`(`-ffunction-sections`) 이후 ROM의 스모크 테스트, 인접 트레이너 방향.
- 다음: 테스트 러너 복구 → #9881·#9086 빌드 기반 → 1.16 배틀 리팩터 순으로 이식한다.

## 2026-09-27 — 전체 엔진 동기화 1단계: ROM 여유 확보 (현재)

- `-ffunction-sections -fdata-sections` 기본 적용으로 ROM 97.56%(여유 약 820 KB)가 됐다. #10529 인접 트레이너 방향 고정을 복원했다. 빌드 성공, SHA1 `3cf2714617…`. 실기 스모크 테스트가 필요하다.
- 인벤토리 재확정(새 지시서의 4개 판정 + 의존성 순서 계획) 진행 중.

## 2026-09-28 — 포케기어 헤더 순서 정정 및 주인공 이름 3글자 복원

- 포케기어 헤더 원인: 이전 패킹 기록에서 위·아래 선택 영역을 거꾸로 설명하고 적용했다. 원본 타일 시트에서 실제 픽셀 내용은 위 2px=`(128,0,51,2)`, 가운데 8px=`(64,0,51,8)`, 아래 2px=`(16,6,51,2)`이다. 런타임 화면에서는 이 세 영역이 각각 화면 `x=16`, `y=6..7`, `y=8..15`, `y=16..17`에 놓인다. 기존에는 위에 아래 띠를, 아래에 위 띠를 넣어 획이 뒤집혔고, 사용자가 보고한 깨진 `포켓기어`가 출력됐다.
- 조치: `graphics/pokenav/hns/header.4bpp`·`header.png`·`header.bin`을 올바른 순서로 다시 패킹하고 `.smol`·`.smolTM`을 갱신했다. 새 타일맵은 제목에 2~19·21·22·41번 타일을 사용하며, 구분선 타일 20번과 나머지 헤더 타일은 보존했다. 역렌더링한 제목 영역 `x=16..66`, `y=6..18`의 612픽셀이 세 원본 선택 영역(위→가운데→아래)과 정확히 일치하고, PNG→4bpp 재변환도 `cmp` 일치했다. mGBA 실제 화면은 이 세션에서 아직 재확인하지 않았다.
- 주인공 이름 제한: `src/naming_screen.c:2607`의 `sPlayerNamingScreenTemplate.maxChars`를 6에서 3으로 복원했다. `BOX`, 포켓몬 닉네임, 라이벌 템플릿의 6은 별도 제한이므로 건드리지 않았다. `include/constants/global.h:179`의 `PLAYER_NAME_LENGTH = 7`은 저장·통신 버퍼 길이여서 변경하지 않았다.
- 변경 시점/회귀 조사: `git blame`과 전체 refs의 `-G '\.maxChars = [36]'` 검색에서 현재의 6은 `361f1e4a77`(`2026-09-02 23:24:31 +0900`, `Korean patch modifications`)이 네 템플릿에 하드코딩한 것으로 확인됐다. `1821fd6749`(`2026-09-26 03:26:03 +0900`, `Upload current HNS worktree`)는 `sText_RivalsName`, `POKEMON_NAME_LENGTH`, `MOVE_NAME_LENGTH`만 바꾸고 maxChars는 건드리지 않았다. 추적 가능한 커밋·현재 인수인계 문서에는 3글자 변경 기록이 없으므로, 과거의 3글자가 미커밋 작업이었다면 정확한 덮어쓴 순간은 Git으로 확정할 수 없고 9월 26일 업로드 전후에 유실됐을 가능성이 가장 높다. 유사한 의도치 않은 변경으로 확인된 것은 없으며, 저장 길이 상수와 다른 이름 입력 화면의 6 제한은 회귀가 아니다.
- 빌드: `make hns -j8` 성공(`BUILD_EXIT=0`). `pokehns.gba`는 33,554,432바이트이며 SHA-1은 `0c3b4c596c198ac1cd6e8eb314625867e6774f4a`다. `build/hns/src/graphics.o`(1,276,904 bytes)와 `build/hns/src/naming_screen.o`(31,204 bytes)도 새로 생성됐다. mGBA 실제 화면 검증은 아직 남아 있으므로, 기존 ROM을 닫고 새 ROM을 완전히 다시 불러와 상단 `포켓기어`와 주인공 이름 입력 칸을 확인한다.

## 2026-09-28 — 포케기어 옵션 전체 재변환 및 51×12 헤더 재패킹

- 요청/결과: 사용자가 다시 추가·한글화한 `graphics/pokenav/options/*.png` 13개와 `region_map/city_zoom_text.png`, HNS `options/radio.png`, `radio/ui_tiles.png`를 모두 GBA 그래픽으로 다시 변환했다. 삭제 상태였던 공용 옵션 원본은 전부 복구되어 현재는 수정 상태로 존재한다.
- 헤더 최종 수정: 사용자가 다시 편집한 `header.png`의 Aseprite 선택은 같은 51픽셀 폭의 위 2px·가운데 8px·아래 2px 영역이다. 세 선택은 원본 타일 띠에서 위 `(16,6)`, 가운데 `(64,0)`, 아래 `(128,0)`부터 각각 51px를 **그대로** 읽었다. 가운데·아래의 왼쪽 공백은 글자 정렬의 일부이므로 제거하지 않았다. 결합한 51×12 `포켓기어`를 7×3 타일 영역으로 다시 패킹했고, 고유 타일 20개는 2~19번과 기존에 화면에서 미사용인 21·22번에 배치했다. 구분선 배경 20번, 오른쪽 아래 작은 제목과 기타 헤더 타일 23~52는 보존했다. `header.bin`은 새 패킹을 화면 x=2부터 참조하며, 역추출한 51×12가 세 원본 선택과 612픽셀 모두 일치한다.
- 변환/구조 검증: 공용 옵션 13개와 HNS 라디오 옵션은 모두 32×64 규격이고, 라디오 UI는 128×32, 도시 확대 텍스트는 64×64다. 기본 `options.4bpp`와 HNS `options.4bpp`를 각각 규칙상의 조각 순서로 다시 연결한 결과가 `cmp`로 일치했다. 관련 `.smol`, 팔레트 및 `header.bin.smolTM`도 갱신됐다.
- 빌드 검증: `header.4bpp`·팔레트·`.smol`·`header.bin.smolTM`의 대상 변환은 성공했고, 생성된 `header.4bpp`가 재패킹 원본과 `cmp` 일치했다. 의존성 누락으로 `make hns`만으로는 포함 오브젝트가 갱신되지 않아 `build/hns/src/graphics.o`를 재컴파일한 뒤 HNS ROM을 다시 링크했다. 새 ROM SHA-1은 `0e374d5e4f5a6498bc3f64a5799255dbe06eaa01`이다. 실행 세션 제한 때문에 빌드 셸의 마지막 종료 코드는 기록되지 않았지만, `graphics.o`는 정상 크기 1,276,908 bytes로 재생성되고 ROM SHA-1도 변경됐다.
- 실행 검증 구분: 이 환경에는 mGBA 실행 명령이 없어 실제 메뉴 조작은 확인하지 못했다. 새 ROM을 완전히 다시 불러와 메인 옵션 14개, 라디오 화면, 도시 확대, 상단 큰 `포켓기어`를 확인한다.

## 2026-09-27 — 포케기어 PNG 재변환·헤더 타일맵 수정 및 HNS 빌드

- 요청/결과: 사용자가 수정한 `graphics/pokenav` PNG를 다시 변환했다. `graphics/pokenav/hns/header.png`는 424×8 완성 화면이 아니라 53개의 8×8 고유 타일을 한 줄로 둔 시트이므로, 기존 영문용 반복 배치를 그대로 쓰면 한글 획이 섞인다. `header.bin`의 상단 3행을 새 `포켓기어` 타일 순서와 좌측 여백에 맞게 다시 배치했다.
- 정적 화면 검증: 변환된 `header.4bpp`·`header.gbapal`과 새 `header.bin`을 `gbagfx`로 256×256 화면에 다시 렌더링해, 왼쪽 위 큰 `포켓기어`, 가로 구분선, 오른쪽 아래 작은 `포켓기어`가 함께 유지되는 것을 확인했다. `header.bin.smolTM`도 다시 생성했다.
- 변환 범위: 수정된 컨디션 그래프/취소, HNS 헤더·통화 헤더·지도/통화 옵션·라디오 UI, 공용 좌측 헤더 12개, 도시 확대 텍스트의 `.4bpp` 및 빌드에서 참조하는 `.smol`을 재생성했다. HNS `options.4bpp`는 전용 Make 규칙이 14개 조각을 고정 순서로 자동 연결하며, 실제 연결 결과와 바이트 일치했다.
- 남은 원본 문제: `graphics/pokenav/options/{beauty,cancel,condition,cool,cute,hoenn_map,match_call,party,ribbons,search,smart,switch_off,tough}.png` 13개가 현재 삭제 상태다. 생성돼 있던 옛 `.4bpp` 때문에 이번 증분 빌드는 통과했지만, 이 옵션들은 새 PNG 번역이 반영된 것으로 볼 수 없고 깨끗한 clone에서는 재생성할 수 없다. `graphics/pokenav/hns/options/radio.png`도 현재 `RAD` 표기 그대로다. 번역 PNG를 복구·추가한 뒤 다시 변환해야 한다.
- 빌드 검증: `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 성공(`BUILD_EXIT=0`). ROM 사용량은 33,315,908/33,554,432 bytes(99.29%)이고, 생성된 `pokehns.gba` SHA-1은 `872956ba1b350325d17b26aa4bcf172e01d6f5ea`다. 이 환경에는 mGBA 실행 명령이 없어 실제 메뉴 조작 검증은 하지 않았다.
- 다음: 삭제 상태인 공용 옵션 PNG 13개와 HNS 라디오 옵션 PNG를 준비한 뒤 같은 규칙으로 재변환하고, 새 `pokehns.gba`를 mGBA에서 완전히 다시 불러와 메인 메뉴·각 좌측 헤더·지도 도시 확대·컨디션 화면을 확인한다.

## 2026-09-27 — NPC 대화 외 잔여 텍스트 인벤토리 및 직접 그래픽 스프라이트 분류 완료

- 요청/범위: 맵 NPC·트레이너 대사를 제외하고 현재 HNS 작업 트리에서 실제 표시 가능한 메뉴·시스템·배틀·도감·시설 텍스트를 찾아 다음 번역 단위로 분류했다. 결과는 `docs/localization/NON_NPC_TEXT_AUDIT.md`에 기록했다.
- 주요 발견: P0은 플레이어 PC/가방/필드 도구/파티의 영문 UI, 저장·획득·PC 전송 시스템 메시지, HGSS 도감·DexNav, 포케기어 런타임 문구와 픽셀 라벨, 배틀 UI 안내다. 이어 아이템 920개·기술 947개 설명 필드, 특성 설명 321개, 이지챗 1,030개, 열매 설명 204개, 굿즈명 121개, 랜드마크·교환·유니언룸·시설/미니게임이 남는다.
- 실제 상태 정정: 과거 PC 잔여 목록과 달리 현재 `src/pokemon_storage_system.c`의 조작 메뉴와 `gPCText_Give`, 지닌도구 결과 문구는 이미 한글이다. 같은 파일에서 이번에 확인한 영문 표시는 `MENU_ETCETERA = "etc."`뿐이다. `Symbiosis`, 눈 지속, 안개, 강철서지 제거 메시지도 현재 한글임을 다시 확인했다.
- 그래픽 확인: HNS 포케기어 옵션/헤더에는 `MAP`·`CALL`·`RAD`·`CALL`이, 공용 포케기어 그래픽에는 `CONDITION`·`SWITCH OFF`·`MAIN MENU`·`CITY ZOOM` 등이 픽셀 텍스트로 남아 있다. `src/graphics.c:1989-2044`의 실제 ROM 포함 경로와 함께 인벤토리에 기록했다.
- 직접 스프라이트 분류 추가: `NON_NPC_TEXT_AUDIT.md`에 PNG 픽셀 텍스트만을 위한 표를 추가했다. 현재 HNS에서 직접 고쳐야 할 P0은 HGSS 도감 타일셋 6개(`interface_hns`, `menu_list`, `menu1`~`menu3`, `menu_search`), 도감 `AREA UNKNOWN`, 포케기어 옵션/헤더/도시 확대/취소 라벨, 작명 화면 `rwindow`·`roptions`다. 유니언룸 채팅 버튼 라벨은 P1, 타이틀 `PRESS START`는 표기 정책을 확인할 P1로 분리했다.
- 포케기어 옵션 그래픽 주의: `pokenav/hns/options`의 32×64 PNG는 화면에 세로로 표시되는 그림이 아니다. 각 파일을 네 개의 32×16 타일 조각으로 읽어 런타임에서 가로 128×16 라벨로 배치한다. 현행 HNS `options.4bpp`는 14개 라벨 `.4bpp`를 고정 순서로 연결한 결과와 바이트 일치했다. 단일 `options.png`는 없지만 `graphics_file_rules.mk`에 전용 결합 규칙이 있으므로, 원본 PNG와 개별 `.4bpp`가 정상적으로 존재하면 `make hns`가 결합·압축 산출물을 갱신한다.
- 제외/완료 판정: 배틀 기술 정보 L/R·Gen4 L/R, 메뉴 정보·PC 저장 시스템·배틀돔 버튼 그래픽은 이미 한글이다. 이지챗 `START`/`SELECT`는 버튼명 유지 원칙을 따른다. START 기술정보 변형, DECA 도감 타일셋, DexNav 아이콘은 현재 설정에서 작업 대상이 아니다. DexNav의 `captured_all`·`no_data`에는 번역할 글자가 없음을 시각 확인했다.
- 제외/보류: NPC 대사, Match Call 통화 본문, 트레이너 데이터는 요청 범위에서 제외했다. TV·Fame Checker·뉴스·이벤트 티켓 등 대사성 아닌 대용량 콘텐츠는 별도 범위로 분리했다. 버튼/단위 약어, 디버그·미사용 문자열, 교환 OT/닉네임 원본 데이터도 일괄 번역하지 않는다.
- 검증: `rg`로 현재 설정·호출부·그래픽 `INCBIN` 연결을 대조하고, 도감/포케기어/작명/타이틀/유니언룸/배틀의 대상 그래픽 원본을 시각 확인했다. 이번 추가 작업은 문서만 수정했으므로 HNS 빌드·ROM·게임 화면 검증은 수행하지 않았다.
- 다음: P0 1단계의 플레이어 PC·가방·필드 도구와 9개 시스템 텍스트 파일을 한 화면 단위로 번역한다. 이어 HGSS 도감 런타임 문구를 번역할 때 위 6개 도감 타일셋과 `area_unknown.png`을 별도 스프라이트 작업 단위로 처리하고, 포케기어 작업에는 런타임 문구와 픽셀 라벨을 함께 묶는다. 각 PNG 작업 뒤 `make hns -j8` 및 실제 메뉴 화면 검증을 한다.

## 2026-09-27 — PC 포켓몬 데이터 waveform 문제 해결 확인

- 사용자 확인: 박스에서 선택할 포켓몬이 없어 `포켓몬 데이터` 제목이 어둡고 양옆 waveform 애니메이션이 나오지 않았던 것이 원인이었다.
- 결론: `UpdateWaveformAnimation()`의 `SPECIES_NONE` 분기는 의도된 동작이며, 코드·그래픽 수정 없이 해결됐다.
- 상태: PC 포켓몬 데이터 제목과 waveform 애니메이션 문제 해결. 추가 빌드는 필요하지 않다.

## 2026-09-27 — PC 포켓몬 데이터 옆 번개형 waveform 애니메이션 확인

- 확인: 번개처럼 보이는 양옆 그래픽은 `graphics/pokemon_storage/waveform.png`에서 생성되는 `waveform.4bpp` 스프라이트다. `src/pokemon_storage_system.c`의 `CreateWaveformSprites()`가 두 개를 만들고, `UpdateWaveformAnimation()`이 포켓몬 데이터 표시 상태에 따라 애니메이션을 선택한다.
- 현재 화면 해석: 사용자가 올린 화면은 왼쪽 포켓몬 데이터 패널이 비어 있고 `포켓몬 데이터` 제목도 회색이다. 이 상태는 현재 커서가 유효한 포켓몬을 표시하지 않아 `sStorage->displayMonSpecies == SPECIES_NONE`인 경우다. 코드는 이때 `StartSpriteAnim(..., i * 2)`로 정지 프레임을 사용하고 제목을 회색 타일맵으로 바꾼다.
- 유효 포켓몬 선택 시: `displayMonSpecies != SPECIES_NONE`이면 `StartSpriteAnimIfDifferent(..., i * 2 + 1)`로 양쪽 waveform의 3프레임 애니메이션을 시작하고 `포켓몬 데이터`를 색상 상태로 바꾼다.
- 비교: 현재 HNS와 `master` 사이에 waveform 선언·생성·팔레트 로드·애니메이션 코드 및 waveform 원본 그래픽 변경은 없다. 따라서 한글화 때문에 애니메이션 코드가 사라진 것이 아니다.
- 다음: 박스에서 실제 포켓몬이 들어 있는 칸에 커서를 두고 왼쪽 데이터 패널에 포켓몬 정보가 표시되는 상태에서 번개가 움직이는지 확인한다. 그 상태에서도 보이지 않으면 새로 빌드한 `pokehns.gba`를 완전히 다시 로드한 뒤 런타임 문제를 추가 조사한다.

## 2026-09-27 — PC 포켓몬 데이터 화면의 어두운 색상 원인 확인

- 확인: 현재 `include/config/pokemon.h:47`은 이미 `P_GBA_STYLE_SPECIES_GFX FALSE`다. 별도의 작업 트리 변경은 없으며, 요청한 `FALSE` 상태로 유지했다.
- 원인: `src/data/graphics/pokemon.h`에서 `FALSE`는 `anim_front.4bpp.smol`·`normal.gbapal` 등 최신 Gen4/5 계열 그래픽·팔레트를 선택한다. `TRUE`일 때만 `anim_front_gba.4bpp.smol`·`normal_gba.gbapal` 등 GBA 스타일 리소스를 선택한다.
- 비교 근거: 현재 HNS 브랜치의 `1821fd6749` 커밋이 `P_GBA_STYLE_SPECIES_GFX`를 `TRUE`에서 `FALSE`로 바꿨다. `master`에는 `TRUE`가 남아 있다. 따라서 한글 문자열 자체가 포켓몬 팔레트를 어둡게 만든 것이 아니라, HNS 브랜치의 그래픽 스타일 설정 차이가 화면 차이를 만든 것이다.
- 추가 확인: `1821fd6749` 이후 현재 HEAD까지 앞면 포켓몬 그래픽·팔레트 파일 변경은 확인되지 않았다. `src/pokemon_storage_system.c`의 한글화도 표시 문자열과 조사 처리 변경이며, 포켓몬 팔레트 로딩 코드는 건드리지 않았다.
- 주의: 이전 HNS/`pokeemerald-kr`과 같은 GBA 색상을 목표로 한다면 설정은 `FALSE`가 아니라 `TRUE`여야 한다. `FALSE`를 유지하면 현재의 어두운 최신 스타일이 의도된 동작이다. 이번 확인에서는 `TRUE`로 되돌리지 않았고 ROM도 새로 빌드하지 않았다.
- 다음: GBA 색상 일치를 원하면 사용자의 승인 후 `P_GBA_STYLE_SPECIES_GFX TRUE`로 변경하고 HNS 전체 빌드한다. 이 설정은 PC뿐 아니라 배틀·파티·상태 화면·진화 화면의 종 그래픽에도 적용된다.

## 2026-09-27 — PC 동적 이름·도구명 뒤 한국어 조사 확장 수정

- 수정: `src/pokemon_storage_system.c`의 `PrintMessage()`를 두 단계로 바꿨다. 먼저 `DynamicPlaceholderTextUtil_ExpandPlaceholders()`로 `{DYNAMIC n}`을 실제 이름·도구명으로 치환하고, 임시 버퍼의 결과를 `StringExpandPlaceholders()`로 처리해 한국어 조사 placeholder를 확장한다.
- 적용 범위: PC의 선택·박스에 맡김·놓아주기·되돌아오기·가방에 넣기·도구 지니게 하기·도구 교체 메시지에 있는 `{K_EULREUL}`, `{K_IGA}`, `{K_WAGWA}`가 모두 처리된다. 받침이 있는 `글라이온`은 `을/이/과`, 받침이 없는 `피카츄`는 `를/가/와`를 선택한다.
- 검증: 수정 대상 오브젝트 `build/hns/src/pokemon_storage_system.o`를 `NODEP=1 SETUP_PREREQS=0 make BUILD=hns build/hns/src/pokemon_storage_system.o -j2`로 컴파일했고 종료 코드 0이었다. 이어 HNS 전체 링크 산출물 `pokehns.gba`가 2026-09-27 02:40:14 KST에 새로 생성됐으며, SHA-1은 `bd293daf4dce22d8889d9733ca1b032142a3153c`이다. 변경 파일 `git diff --check`도 통과했다.
- 실행 주의: mGBA는 이미 실행 중인 ROM 파일을 자동으로 다시 읽지 않는다. 새 코드를 확인할 때는 mGBA를 완전히 종료한 뒤 이 저장소 최상단의 `pokehns.gba`를 다시 열거나, `파일 → ROM 불러오기`로 다시 로드해야 한다.

## 2026-09-27 — PC 동적 포켓몬 이름 뒤 목적격 조사 누락 원인

- 현상: PC의 파티 포켓몬 선택 메뉴에서 `글라이온` 뒤에 목적격 조사 `을`이 표시되지 않아, 의도된 `글라이온을 어떻게 할까?`가 되지 않는다.
- 원인: `src/pokemon_storage_system.c:1052`의 원문에는 `{DYNAMIC 0}{K_EULREUL}`가 정확히 들어 있다. 그러나 `PrintMessage()`는 `DynamicPlaceholderTextUtil_ExpandPlaceholders()`만 호출한다. 이 확장기는 `{DYNAMIC 0}`만 실제 이름으로 복사하고 `{K_EULREUL}`(내부 placeholder)는 처리하지 않는다. 이어지는 텍스트 프린터는 미확장 placeholder를 건너뛰므로 조사가 화면에 남지 않는다.
- 조치 상태: 위 두 단계 확장으로 수정했다. 임시 버퍼를 사용하므로 `sStorage->messageText`를 입력·출력으로 동시에 쓰지 않는다.

## 2026-09-27 — 친구 묶음 A 및 전체 동기화 지시서 상태 HNS ROM 빌드

- 기준: 친구의 묶음 A 원격 팁 `500f3634b4` 위에 전체 엔진 동기화 지시서 커밋 `84835a32d2`가 있는 `pokehns-expansion-kor` 브랜치.
- 빌드: 도구 사전 빌드(`make -f make_tools.mk`)와 history 검사 성공 뒤 `NODEP=1 SETUP_PREREQS=0 make hns -j8`로 전체 HNS 컴파일·링크를 실행했다. 현재 `pokehns.elf`는 45,217,964 bytes, 패딩된 `pokehns.gba`는 33,554,432 bytes(32MiB)이며 생성 시각은 2026-09-27 02:15:58 KST다.
- 산출물: `pokehns.gba` SHA-1은 `c1b94d256e7e2ea7fa48ca971a329b9c86ba645f`다.
- 구분: 컴파일·링크 산출물 확인만 완료했다. 자동 테스트와 mGBA 실제 플레이 검증은 하지 않았다. 일반 `make -q hns`는 맵 JSON 생성 규칙이 항상 재실행 대상으로 잡혀 종료 코드 1을 반환하므로, 이를 빌드 실패로 해석하지 않는다.
## 2026-09-27 — 1.17.0 전체 엔진 동기화 0단계 완료 (현재)

- 새 지시서 `docs/friend-handoff/POKEEMERALD_EXPANSION_1.17.0_FULL_ENGINE_SYNC.md` 기준으로 시작했다. 묶음 A 35건 존재를 확인했고, TSV 후행 탭을 정리했다(빈 칸 → `-`). 전체 범위 `git diff --check`가 통과한다.
- 기준 빌드: ROM 33,327,220 B(99.32%, 여유 약 227 KB), SHA1 `ac6df9a5…`. 친구 환경 수치와의 차이는 툴체인 차이로 본다(`SESSION_LOG.md` 참고).
- 다음: 1단계 인벤토리 재확정 → ROM 여유 확보 → 의존성 순서 이식.

## 2026-09-26 — 1.17.0 묶음 A 이식 완료 (현재)

- 현존 버그 수정 35개 PR을 이식했다. 불요의검·불굴의방패, 바톤터치 전자부유, AI 괄호·세미콜론, 정적 조우 능력치, 동반 포켓몬 크래시 등이다. `make hns` 성공, ROM +736 B(99.32%). 실기 검증은 하지 않았다.
- 사용자 결정 대기 3건(#10529, #10015, #10344). 다음은 묶음 B. 상세는 결과 보고서 "적용 단위 기록".

## 2026-09-26 — 1.17.0 업데이트 1단계 인벤토리 완료 (현재)

- upstream PR 629개 판정 완료(코드 대조 기준): 이미 적용 73, 부분 17, 이식 가능 225, 선행 필요 105, 충돌 39, 무관 170. 보고서는 `docs/friend-handoff/results/pokeemerald-expansion-1.17.0-update-report.md`.
- HnS는 upstream master 1.15.2 개발 계열(merge-base `3efb836f72`)이다. upcoming 대형 리팩터가 없어 전체 1.17.0이 아니라 선별 이식이 현실적 범위다.
- 소스 변경 없음. 다음: 보고서 9절의 사용자 결정 → 묶음 A(현존 버그 수정)부터 이식.

## 2026-09-26 — 새 clone 기준 빌드 복구 (현재)

- 새 clone에서 `make hns`가 이름 입력 화면 PNG 3개 누락과 한글화 폰트 생성 규칙 누락으로 실패했다. PNG는 `pokeemerald-kr` 원본으로 추가하고 `graphics_file_rules.mk`에 폰트 규칙 16개를 추가했다. 문자열·코드 변경은 없다.
- 기준 빌드: `make hns -j12` 성공, `pokehns.gba` SHA1 `7f3f85c7dce5402d388abf369c3e60574b854973`, ROM 99.32%(여유 약 223 KB), EWRAM 94.99%. 실기 검증 없음.
- 다음: pokeemerald-expansion 1.17.0 업데이트 1단계 인벤토리. 상세는 `SESSION_LOG.md` 같은 날짜 항목.

## 2026-09-27 — 친구용 1.17.0 전체 엔진 동기화 지시서 추가

- 요청: 2026-09-26의 안전한 묶음 A 35개 선별 이식에서 멈추지 않고, HNS 한글화·커스텀과 공존하는 `pokeemerald-expansion` 1.17.0 전체 엔진 동기화를 친구에게 지시한다.
- 구현: `docs/friend-handoff/POKEEMERALD_EXPANSION_1.17.0_FULL_ENGINE_SYNC.md`를 추가하고 `README.md` 작업 목록에서 연결했다. 문서는 694개 upstream PR의 현재 코드 기준 재판정, 남은 engine-relevant 변경의 의존성 순서 이식, HNS 적응 충돌 처리, 배틀 메시지 보존, 저장/ROM 검증, 결과·Git 규칙을 작업자가 바로 실행할 수 있게 명시한다.
- 중요: 전체 동기화는 upstream 전체 merge·파일 덮어쓰기가 아니다. 기존 한글 문장·`{B_...}`·조사·특성 팝업·사용자 지정 배틀 메시지 출력과 HNS 고유 데이터는 보존한다. 단순 충돌은 보류로 방치하지 않고 HNS 구조에 맞춰 해결하며, 사용자 지정 동작 자체를 바꿔야 하는 경우에만 근거·선택지를 보고한다.
- 검증: 새 문서와 README의 `git diff --check`를 통과했다. 코드·데이터·ROM은 수정하거나 빌드하지 않았다.
- 다음: 이 문서를 Fork의 `pokehns-expansion-kor` 브랜치에 반영한 뒤, 친구는 묶음 A 재검증·TSV 후행 탭 정리·전체 인벤토리 재확정부터 시작한다.

## 2026-09-27 — 친구의 2026-09-26 upstream 선별 이식 검토 (미병합 원격 브랜치)

- 대상: Fork `gorunit1/pokehns-expansion-kor`의 `origin/pokehns-expansion-kor` 원격 팁 `500f3634b4300ee412dfcef142ca6fc9d4059165`를, 현재 로컬 기준 `496bca3475`와 대조했다. 로컬 `pokehns-expansion-kor`는 이 원격보다 40커밋 뒤이며, 이 검토에서 merge/rebase/switch는 하지 않았다.
- 작업 내용: 친구는 1.15.2~1.17.0 upstream 694 PR을 인벤토리화한 뒤 35개를 선별 이식했다. 배틀 타이머·동적 불꽃 해동·메가/테라 폼 변경 뒤 효과·AI·오버월드/추종자·저장/유틸리티 수정과, 새 클론에서 누락되던 naming screen PNG 3개 및 한글 폰트 생성 규칙이 포함된다. 상세 범위는 원격의 `docs/friend-handoff/HANDBACK_2026-09-26.md`와 `results/1.17.0-inventory/`에 있다.
- 보존 확인: 변경 경로에 `src/battle_message.c`, `include/constants/battle_string_ids.h`, `include/config/battle.h`가 없고, `docs/`를 제외한 패치에 새 한글 문자열도 없다. 따라서 사용자 한글 문장, `{B_...}` 치환, 배틀 메시지 출력 최신화 변경을 덮어쓰지 않는다.
- 정적 검토: `CanFireMoveThawTarget(move, moveType)` 선언·AI·전투 호출부가 함께 바뀌었고, 폼 변경 뒤 효과는 White Herb → Opportunist → Mirror Herb → Eject Pack 순으로 처리한다. HNS에 없는 `gBattlersByRawSpeed`는 기존 HNS의 battler 순서 처리로 의도적으로 대체했다. 즉시 차단할 코드 충돌·빌드 오류는 찾지 못했다.
- 빌드 검증: 원격 팁을 분리 worktree에서 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8`로 전체 컴파일·링크했고 종료 코드 0이었다. 링크 사용량은 EWRAM `249,016/262,144`, IWRAM `25,704/32,768`, ROM `33,330,740/33,554,432` bytes다. 실기/mGBA 동작 검증은 하지 않았다.
- 보완 필요: 전체 범위의 `git diff --check`는 `docs/friend-handoff/results/1.17.0-inventory/all_prs_checked.tsv`와 `already_in_history.tsv`의 빈 열 후행 탭 때문에 실패한다. 코드 문제가 아닌 문서 TSV 형식 문제지만, 병합 전 친구가 제거해야 한다. 친구 보고서의 ROM 사용 바이트(`33,327,220`)도 이 환경의 재빌드 값과 일치하지 않으므로, 정확한 수치 대신 빌드 성공만 재현된 것으로 취급한다.
- 병합 전 실제 확인: (1) 전자부유/록온 지속 턴 및 Baton Pass, (2) 메가·테라 변신 뒤 White Herb·편승·미러허브·탈출팩, (3) Future Sight/상호작용 도구, (4) AI·더블배틀·추종자·재대결을 새 ROM/게임 내 저장에서 재현한다. 확인 뒤에만 원격 40커밋을 병합할지 결정한다.

## 2026-09-26 — `pokemon_storage_system.c` HGSS 원문 번역 감사 (현재)

- 요청/범위: 현재 한글화된 `src/pokemon_storage_system.c`의 런타임 PC 문자열을 Poké Corpus의 HGSS 한국어 원문 Text File 24·25 및 원래 영문 호출 의미와 대조했다. 이번 작업은 검증·권장 목록만 작성했으며 소스 문자열은 바꾸지 않았다.
- 이미 일치: 기본 박스명·파티 가득 참·박스 메인 메뉴의 네 기능/설명, `박스를 종료하겠습니까?`, `정말 놓아주겠습니까?`, `바이바이`, `싸울 포켓몬이 없어집니다!`, 박스/가방 가득 참, 알 방출 불가, 계속 조작, 되돌아옴, 걱정, 메일 제거, `벽지`·`이름`, 숲~하늘, `포켓센`, `심플`은 HGSS의 같은 PC 문구 또는 같은 벽지명과 일치한다. `MENU_INFO = 정보`도 실제 도구 설명 창을 여는 기능에 맞다.
- 명확한 권장: 아직 영문인 `gPCText_Give`는 두 개의 도구 주기 메뉴에서 실제 출력되므로 `지니게 한다`가 적절하다. `MSG_ITEM_IS_HELD`는 원문과 HGSS의 `[도구] 지니게 했다!`에 맞춰 `"{DYNAMIC 0} 지니게 했다!"`가 맞고, 현재 `…을 맡겼다!`는 도구를 맡긴 것으로 의미가 바뀐다. `MSG_CHANGED_TO_ITEM`은 HGSS에 같은 결과 문장이 없다. 따라서 현재 `{K_WAGWA}`(와/과)가 문법에 맞지 않고 `"{DYNAMIC 0}{K_EU}로 바꾸었다!"`가 원본 `Changed to [item].`의 자연스러운 복원이라는 판단은 가능하지만, 이를 HGSS 직결 원문이라고 주장하지 않는다.
- 공식 표현 권장: HGSS 직결 문구는 선택된 대상의 `"{DYNAMIC 0} 어떻게 할까?"`, 방출 결과의 `"{DYNAMIC 0} 밖에 놓아주었다."`, 파티에서 꺼낼 대상의 `"어느 포켓몬을 데리고 갈까?"`다. 현재 문장도 기능은 전달하지만 위 문구가 HGSS 원문과 같다. `MSG_SURPRISE`도 원문/HGSS 표기는 `… … … … !`이며 현재의 `..... ..... .....!`와 다르다(한글 charmap에는 `…`가 있다). `MSG_HOLDING_POKE`는 HGSS에 직접 대응하는 버튼 안내가 없어 현재 `쥐고`를 공식 근거로 바꿀 수는 없으며, HNS 원문 의미상 자연스러운 대안은 `포켓몬을 잡고 있습니다!`다.
- 벽지 주의: HGSS의 `MACHINE`은 `금속`이므로 현재 `기계`보다 `금속`이 직접 대응한다. 반면 HNS의 `POLKA-DOT`, `SCENERY 1`~`3`, `ETCETERA`, Walda 해금 벽지 `FRIENDS`는 HGSS에 같은 메뉴/자산이 없다. 따라서 `물방울`, `풍경1`~`3`, `etc.`, `애호가`는 HGSS 공식 문구라고 확정할 수 없다. 특히 `FRIENDS`는 Walda 전용 벽지 항목이므로 `애호가`로 의역할 근거가 없다.
- 별도 기능: `MENU_SELECT = 선택한다`는 HGSS에 없는 upstream `ChooseBoxMon` 전용 명령이다. 기존 조사대로 포켓몬을 고르는 행동에는 짧은 메뉴명 `선택`이 적합하며, HGSS 대조로 이를 `결정` 또는 설명문으로 바꿀 근거는 없다.
- 근거: Poké Corpus [HGSS 한국어 원문](https://raw.githubusercontent.com/abcboy101/poke-corpus/main/corpus/HeartGoldSoulSilver/ko_msg.txt) Text File 24·25(확인일 2026-09-26), `src/pokemon_storage_system.c`의 각 `MSG_*` 및 `sMenuTexts` 호출부. 정적 문자열/호출부 대조만 완료했으며, ROM 빌드·실기 화면 검증은 실행하지 않았다.
- 다음 시작점: 사용자가 승인하면 위의 명확한 3개 도구 문자열과 `GIVE`·`MACHINE`부터 별도 패치하고, HGSS에 직접 대응하지 않는 Walda/Polka-dot 벽지명은 기존 `pokeemerald-kr` 또는 실제 한국판 GBA 자료를 확보한 뒤 결정한다.

## 2026-09-26 — PC `MENU_SELECT` 번역 근거 확인 (현재)

- 확인: `MENU_SELECT`는 일반 PC의 이동/맡기기 명령이 아니라 2025년 upstream `ChooseBoxMon` 추가 기능의 전용 항목이다. 선택한 박스 포켓몬을 호출 스크립트로 반환하는 동작이므로 영문 `SELECT`의 의미는 버튼명 `SELECT`가 아니라 동사 명령 `select`다.
- 원문 대조: `pokeemerald-kr`의 `gText_Select`는 영문 `SELECT`가 남은 미사용 문자열이라 근거가 되지 않는다. Poké Corpus HGSS와 XY의 공식 한국어 UI는 포켓몬을 고르는 동작을 `포켓몬을 선택하고 결정`으로, 확정 동작을 `결정`으로 구분한다. 이 항목은 포켓몬을 고르는 단계이므로 `결정`이 아니라 `선택`이 맞다.
- 권장 변경: `[MENU_SELECT] = COMPOUND_STRING("선택")`. 메뉴 폭에 맞는 명사형/명령형 단문이며, 이미 있는 `MSG_PKMN_IS_SELECTED` 등의 `선택` 용례와도 일치한다. 이번 확인에서는 소스 문자열을 변경하지 않았다.
- 실제 표시 조건: 플레이어가 평상시 PC의 `박스를 정리한다`를 열었을 때는 표시되지 않는다. 이벤트 스크립트가 `chooseboxmon`을 호출할 때만 PC가 선택 전용 모드로 열리며, 유효한 포켓몬 위에서 A를 누르면 메뉴 첫 항목으로 `선택`·`요약`·`마킹`·`취소`가 나타난다. `선택`을 누르면 PC를 닫고 해당 포켓몬의 파티/박스 위치를 스크립트에 반환한다. 현재 HNS에서는 기술 가르침/기술 되새김/기술 지우기, 이름 감정, 특정 종을 보여 주는 NPC·교환 등에서 쓰인다.

## 2026-09-26 — PC `MENU_INFO` 번역 근거 확인 (현재)

- 실제 동작: 지닌물건 정리에서 도구를 지닌 포켓몬에 A를 누르면 `TAKE`·`BAG`·`INFO`·`CANCEL` 중 하나로 표시되며, `MENU_INFO`는 `Task_ShowItemInfo()`를 통해 그 도구의 설명 창을 연다.
- 원문 대조: Poké Corpus HGSS에서 영문 `INFO`라는 메뉴 라벨은 공식 한국어판에서 `정보`로 표기된다. `설명을 듣는다`는 시설·미니게임 규칙을 NPC가 설명하는 별도 선택지의 표기이고, `설명을 읽는다`는 이상한 카드의 글을 읽는 동작이다. 도구 설명 창을 여는 짧은 `INFO` 메뉴에는 양쪽 모두 맞지 않는다.
- 권장 변경: `[MENU_INFO] = COMPOUND_STRING("정보")`. 이번 확인에서는 소스 문자열을 변경하지 않았다.

## 2026-09-26 — `pokemon_storage_system.c` 미번역 UI 문자열 점검 (현재)

- 확인 범위: 이 파일의 `_()`·`COMPOUND_STRING()` 런타임 문자열을 전수 대조했다. 박스 메인 메뉴·확인 메시지·가방/도구 메시지·너즐록 메시지와 기본 박스명 `gText_Box`는 현재 한글이다.
- 미번역 문자열: `sMenuTexts`와 `gPCText_Give`에 PC 조작 메뉴 `GIVE`, `CANCEL`, `STORE`, `WITHDRAW`, `MOVE`, `SHIFT`, `PLACE`, `SUMMARY`, `RELEASE`, `MARK`, `JUMP`, `WALLPAPER`, `NAME`, `TAKE`, `SWITCH`, `BAG`, `INFO`, `SELECT`가 영문으로 남아 있다.
- 미번역 벽지 메뉴: 같은 표의 `SCENERY 1`~`3`, `ETCETERA`, `FRIENDS`, `FOREST`, `CITY`, `DESERT`, `SAVANNA`, `CRAG`, `VOLCANO`, `SNOW`, `CAVE`, `BEACH`, `SEAFLOOR`, `RIVER`, `SKY`, `POLKA-DOT`, `POKéCENTER`, `MACHINE`, `SIMPLE`도 영문이다.
- 제외: `"/30"`은 수량 표기이고, 주석·파일 경로·그래픽 바이너리 안의 영문은 C 문자열 번역 범위가 아니다. 이번 점검에서는 문자열을 변경하지 않았다.

## 2026-09-26 — PC 압축 해제 오류 추가 조사 (현재)

- 재현 상태: 화염구슬·맹독구슬 `end2` 수정과 메가찌르호크/영원의꽃 플라엣테 데이터 수정 뒤에도, 기존 게임 내 저장에서 PC의 `Move Pokémon`을 선택할 때 압축 해제 오류가 계속 보고됐다. 사용자는 savestate를 사용한 적이 없으며, 새 게임 내 저장에서는 현재 PC 오류가 재현되지 않는다. 따라서 이 문제는 앞선 배틀 스크립트 오류의 후속 손상이나 PC 공통 리소스 문제가 아니라, 기존 저장의 파티 또는 박스 데이터와 결합된 문제로 좁혀진다.
- 확정된 범위: 오류 입력 `0x08041000`은 현재 ROM의 `AnimTask_NightShadeClone` 직전 배틀 애니메이션 코드 영역이다. `gStorageSystemMenu_Gfx`, `sDisplayMenu_Tilemap`, `gStorageSystemPartyMenu_Tilemap`, 영원의꽃/메가 플라엣테 및 메가찌르호크의 앞면 그림은 모두 ROM 데이터 영역의 정상 압축 리소스를 가리킨다. `pokehns.gba` 전체에서 `0x08041000` 포인터 값도 발견되지 않았다.
- 해석: 정적 PC/종/아이템 그래픽 테이블에 잘못된 리소스 주소가 박혀 있는 문제가 아니라, PC 초기화 중 어떤 호출자가 런타임에 코드 주소를 압축 입력 포인터로 전달하는 문제다. PC는 초기화 중 현재 커서의 박스/파티 포켓몬에 `LoadSpecialPokePicIsEgg()`를 호출하므로, 저장된 포켓몬 데이터 또는 해당 호출 이전의 RAM 손상도 후보이다.
- 디버그 포켓몬 확인: 사용자가 디버그 메뉴로 추가한 글라이온(레벨 100, 경험치 1,059,860)과 리자몽(레벨 4)의 요약 화면상 종·경험치·능력치·기술 PP는 유효한 범위다. 글라이온의 경험치는 느림 성장 그룹 레벨 100의 정상값이다. 디버그 메뉴의 일반/상세 생성 경로도 모두 `CreateMon()`으로 정상 구조체를 만든 뒤 파티에 넣으므로, 화면에 보인 능력치 설정 자체는 손상 근거가 아니다.
- PC 읽기 범위 정정: `OPTION_MOVE_MONS`의 최초 진입은 커서를 현재 박스 0번 칸에 두고, 현재 박스의 30칸 아이콘을 먼저 만든다. 따라서 글라이온·리자몽이 아직 파티에만 있다면 이 둘은 최초 PC 진입 오류의 직접 원인이 아니다. 둘 중 누군가가 현재 박스에 있거나, 현재 박스의 다른 포켓몬이 문제인지가 다음 확인 대상이다.
- 빈 박스 추가 보고/수정: 사용자는 현재 박스에 포켓몬이 전혀 없다고 확인했다. 이 경우 포켓몬 그림은 원인이 될 수 없고, 빈 박스에서도 즉시 읽는 `currentBox` 및 `boxWallpapers[currentBox]` 저장 메타데이터가 가장 유력하다. `StorageGetCurrentBox()`가 범위 밖 값을 0번 박스로 복구하고, `GetBoxWallpaper()`가 범위 밖 벽지 ID를 감지하면 복구 함수를 실행하도록 수정했다. 기존에는 유효성 검사 없이 `sWallpapers[wallpaperId]`를 참조했으므로 잘못된 벽지 ID가 코드 주소를 압축 입력 포인터로 만들 수 있었다.
- 메타데이터 손상 확인/복구 범위: 충돌 방지 수정 뒤 기존 저장에서 박스 2부터 제목이 중복되고 일부 제목 정렬·14번 제목·벽지가 비정상으로 표시됐다. 이는 벽지 ID 하나가 아닌 `boxNames[14]`·`boxWallpapers[14]` 배열의 저장값이 현재 구조와 맞지 않음을 뜻한다. 범위 밖 박스/벽지 값이 한 번이라도 감지되면 `RepairPokemonStorageMetadata()`가 14개 제목을 `BOX 1`~`BOX 14`로, 벽지를 초기 기본 순서로 되돌린다. 박스 속 포켓몬, 파티, 퓨전 저장 데이터는 전혀 변경하지 않는다.
- 금지 조치: 오류 화면에서 START로 계속하지 않는다. 잘못된 데이터를 풀어 그래픽과 실행 흐름을 추가로 손상시키므로, 발생 즉시 mGBA를 완전히 리셋한다. 새 ROM에 과거 ROM의 savestate를 불러오지 말고 게임 내 저장만 사용한다.
- 다음 분기: 기존 `.sav`를 복사해 보관하고, 기존 저장에서 PC가 처음 선택하는 박스 칸/파티의 포켓몬부터 어떤 종·폼·지닌도구가 오류를 유발하는지 분리한다. 필요하면 mGBA 디버거에서 실행 중단점 `0x0810E5E8` (`DecompressionError`)을 설정해 재현하고, 멈춘 시점의 `r0`(입력 포인터), `lr`(호출자 복귀 주소), Call Stack을 기록한다. `lr - 4`를 `arm-none-eabi-addr2line -e pokehns.elf <주소>`로 소스 행에 연결한 뒤 그 호출자만 수정한다.
- 검증/다음: 소스 변경 후 HNS 빌드로 새 ROM을 만들고 기존 `.sav`에서 PC를 연다. 정상 진입하면 저장 과정에서 올바른 박스/벽지 값이 다시 기록된다. 여전히 오류가 나면 mGBA 중단점으로 `DecompressionError` 호출자를 확보하며, PC 그래픽 교체나 압축 오류 무시는 하지 않는다.

## 2026-09-26 — 화염·맹독구슬, 메가찌르호크, 영원의꽃 플라엣테 오류 수정 (현재)

- 구현: `BattleScript_ToxicOrb`·`BattleScript_FlameOrb`가 기존 아이템 팝업·상태 애니메이션·문구를 유지한 채 새 `BattleScript_UpdateEffectStatusIconEnd2`로 끝나게 했다. 이 전용 꼬리는 상태 아이콘 갱신 후 `end2`로 종료하므로 최상위 `BattleScriptExecute()`에서 빈 스크립트 스택 `return`을 실행하지 않는다. 기존 `BattleScript_UpdateEffectStatusIconRet`는 하위 호출자가 사용하는 `return` 경로로 그대로 유지했다.
- 구현: `SPECIES_STARAPTOR_MEGA`에 이미 존재하는 `gMonIcon_StaraptorMega`와 팔레트 인덱스 0을 연결했다. 따라서 NULL 아이콘의 물음표 폴백 대신 메가찌르호크 아이콘을 사용한다.
- 구현: `sFloetteEternalFormChangeTable`의 `FORM_CHANGE_FAINT`와 `FORM_CHANGE_END_BATTLE` 복원 대상을 `SPECIES_FLOETTE_ETERNAL`로 변경했다. 영원의꽃 플라엣테가 메가진화한 뒤 승리·도주 종료 또는 기절해도 영원의꽃 플라엣테로 복귀한다.
- 범위: 한글 메시지 본문은 수정하지 않았으며, 일반 플라엣테·다른 메가진화의 폼 복원 규칙은 건드리지 않았다.
- 검증: 변경 파일 `git diff --check` 통과. `NODEP=1 SETUP_PREREQS=0 make BUILD=hns build/hns/data/battle_scripts_1.o -j2`로 배틀 스크립트 오브젝트가 2026-09-26 06:08 KST에 재생성됐다. 이 세션 환경에서는 `src/pokemon.c`의 대형 종 테이블 컴파일이 출력 없이 종료되어 새 전체 ROM 링크를 완료하지 못했다. 기존 `pokehns.gba`는 수정 전 ROM이므로 테스트에 사용하지 않는다.
- 다음 시작점: 로컬 터미널에서 `GITHUB_ACTION=1 make --jobserver-style=pipe hns -j8`을 성공시켜 새 `pokehns.gba`를 만든 뒤, 화염구슬·맹독구슬 각 1회, 메가찌르호크 파티/PC 아이콘, 영원의꽃 플라엣테의 메가진화 후 승리·기절 종료를 실제 ROM에서 확인한다. PC 압축 해제 오류는 이 세 수정과 별도로 남아 있다.

## 2026-09-26 — 메가찌르호크 아이콘·영원의꽃 플라엣테 폼 복원 오류 진단 (현재)

- 실제 보고: 메가찌르호크의 파티 아이콘이 물음표로 보이며, 영원의꽃 플라엣테가 메가진화한 배틀이 끝나면 영원의꽃 폼이 아닌 일반 플라엣테가 된다.
- 메가찌르호크 원인: `graphics/pokemon/staraptor/mega/icon.4bpp`와 `gMonIcon_StaraptorMega` 심볼은 실제로 존재한다. 그러나 `src/data/pokemon/species_info/gen_4_families.h`의 `SPECIES_STARAPTOR_MEGA` 항목에서 `.iconSprite = gMonIcon_StaraptorMega`와 `.iconPalIndex = 0`가 주석 처리되어 있다. `GetMonIconTilesIsEgg()`는 `iconSprite == NULL`이면 `SPECIES_NONE`의 물음표 아이콘으로 폴백하므로, 이는 의도된 폴백이 아니라 누락된 연결이다.
- 영원의꽃 플라엣테 원인: `sFloetteEternalFormChangeTable`은 영원의꽃 플라엣테와 메가플라엣테 양쪽이 사용한다. 이 테이블의 `FORM_CHANGE_FAINT` 및 `FORM_CHANGE_END_BATTLE` 대상이 `SPECIES_FLOETTE`로 되어 있다. `TryBattleFormChange()`는 테이블의 명시적 대상이 있으면 배틀 시작 종 보존값보다 우선하므로, 메가플라엣테가 배틀 종료/기절 때 일반 플라엣테로 실제 저장된다.
- 안전한 수정 방향: 메가찌르호크에는 기존 `gMonIcon_StaraptorMega`·팔레트 인덱스를 종 정보에 연결한다. 플라엣테 표의 두 복원 대상을 `SPECIES_FLOETTE_ETERNAL`로 바꿔 영원의꽃 폼으로 되돌린다. 이 변경은 일반 플라엣테가 메가진화하는 경로가 없고 영원의꽃 전용 `ITEM_FLOETTITE` 경로만 표에 있으므로, 의도된 영원의꽃 원상복귀만 고친다.
- 조치 상태: 이번 작업은 원인 조사·정적 검증만 수행했으며 소스는 수정하지 않았다. 수정 요청이 오면 두 데이터 파일만 변경하고 HNS 빌드와 (1) 메가찌르호크 파티/PC 아이콘, (2) 영원의꽃 플라엣테 메가진화 후 승리·기절·도주 종료를 실제 ROM에서 확인한다.

## 2026-09-26 — 화염구슬·맹독구슬 `return` 오류 원인 확정, PC 압축 해제 오류 분리 조사 (현재)

- 실제 재현 보고: 화염구슬 또는 맹독구슬이 발동하면 `src/battle_script_commands.c:5034`의 `return used with nothing to return to` assertf가 발생한다. 별도로 PC에서 `Move Pokémon`을 선택하면 `src/decompress_error_handler.c`의 압축 해제 실패가 표시되고, START로 계속하면 그래픽 깨짐 뒤 `Jumped to invalid address: 0451C220` 충돌이 발생한다.
- 화염구슬·맹독구슬의 확정 원인: 두 효과는 `src/battle_hold_effects.c`에서 `BattleScriptExecute()`로 최상위 스크립트를 시작한다. 그런데 `1821fd6749`의 아이템 팝업 이식이 두 스크립트를 `BattleScript_UpdateEffectStatusIconRet`로 보내게 바꾸었고, 이 공통 꼬리는 `return`으로 끝난다. 최상위 실행에는 `battleScriptsStack`에 복귀 주소가 없으므로 팝업 호출이 끝난 직후 해당 `return`이 빈 스택을 만나 assertf를 낸다. 같은 커밋 전에는 두 스크립트가 `end2`로 정상 종료했다.
- 범위/조치 상태: 이는 배틀 메시지 문장이나 mGBA 문제가 아니라 스크립트 종료 명령의 불일치다. 이번 항목은 진단만 수행했으며, 사용자의 별도 수정 요청 전에는 `data/battle_scripts_1.s`를 바꾸지 않았다. 수정 시에는 두 스크립트의 마지막 경로를 `end2` 계열로 종료하도록 하되, 아이템 팝업·상태 애니메이션·현재 한글 문자열은 유지해야 한다.
- PC 압축 해제 오류의 해석: 화면의 `IN: 0x0810E3B9`는 `DecompressDataWithHeaderWram()` 내부, `IN: 0x0810E5FF`는 `DecompressionError()` 내부에 해당한다. 오류가 보고한 입력 주소 `0x08041000`은 ROM 코드 영역(`AnimTask_NightShadeClone` 부근)으로, 유효한 압축 그래픽 주소가 아니다. 그 뒤의 화면 깨짐과 `0x0451C220` 점프는 이 잘못된 포인터를 계속 사용한 후속 손상이다.
- PC 정적 대조: PC 초기화의 `gStorageSystemMenu_Gfx`와 메가플라엣테의 `gMonFrontPic_FloetteMega`는 모두 현재 ROM에서 올바른 SMOL 압축 헤더를 가진 파일 주소를 가리킨다. 따라서 현재 증거로는 PC 배경 파일 또는 메가플라엣테 front pic 자체의 손상으로 단정할 수 없다. PC 진입 중 어느 호출자가 코드 주소를 압축 입력으로 넘기는지 런타임 추적이 필요하다.
- 다음 재현/추적: (1) mGBA를 완전히 재시작해 assertf가 한 번도 발생하지 않은 게임 내 저장으로 PC를 먼저 연다. (2) 메가플라엣테가 파티/현재 박스에 있는 경우와 없는 경우를 분리한다. (3) PC 오류 직전 mGBA 디버거에서 `DecompressionError`의 첫 인수와 호출자 주소를 기록한다. 화염/맹독구슬 오류를 본 뒤에는 START로 계속하지 말고 에뮬레이터를 리셋한다. 두 오류가 같은 저장 세션에서 연이어 발생했다면 후자는 전자의 손상 결과일 수 있다.

## 2026-09-26 — 메가플라엣테 교체 후 배틀 스크립트 `return` 런타임 오류 (현재)

- 실제 재현 보고: 디버그로 `SPECIES_FLOETTE_MEGA`(메가플라엣테)를 만들고 야생 부우부와 배틀을 시작한 뒤, 메가플라엣테를 리아코로 일반 교체하자마자 assertf 화면이 표시됐다.
- 화면 의미: `src/battle_script_commands.c:5034`의 `Cmd_return()`이 호출 스택 크기 0에서 `return`을 실행해 중단한 것이다. 최상위 배틀 스크립트 또는 호출 관계가 깨진 하위 스크립트가 `return` 대신 `end` 계열로 끝나야 할 상황임을 뜻한다. mGBA 자체 오류나 단순 메시지 문자열 오류가 아니다.
- 정적 대조: 일반 교체는 `BattleScript_DoSwitchOut` → `switchineffects` → `switchinevents` 경로다. 리아코의 가능한 특성은 급류/우격다짐으로 교체 등장 메시지를 내지 않으며, 메가플라엣테의 페어리오라도 일반 퇴장 처리에서 별도 반환 스크립트를 만들지 않는다. 따라서 현재 정보만으로 특정 종·특성 또는 최근 `battle_message.c` 주석 변경을 직접 원인으로 단정할 수 없다.
- 주소 해석: 화면의 `:5`와 다음 줄의 `034:`는 합쳐서 현재 소스의 `:5034`다. 화면의 `IN: 000C4936`은 assertf 호출 경로의 반환 주소이며, 맵 파일상 `BattleTv_SetDataBasedOnString` 내부다. 이것만으로 배틀 TV가 원인이라고 단정하지 않으며, 컨트롤러의 문자열 처리 호출 경로에서 assertion이 표시된 흔적이다.
- 우선 확인: 다른 ROM에서 만든 mGBA 세이브 스테이트를 새 ROM에 불러오면 배틀 스크립트 주소가 달라져 같은 오류가 날 수 있으므로, mGBA를 완전히 재시작하고 세이브 스테이트 없이 게임 내 저장만 사용해 같은 절차를 재현한다.
- 남은 재현 분리: (1) 일반 포켓몬 → 리아코, (2) 메가플라엣테 → 다른 포켓몬, (3) 메가플라엣테 → 리아코를 각각 새 배틀에서 시험한다. 세 경우 중 어느 경우에 오류가 나는지, 교체 직전의 메시지·기술·지닌 도구·특성·배틀 형식을 기록한다. 세이브 스테이트 없이도 재현되면 해당 교체 경로를 디버거/테스트로 추적해 수정한다.

## 2026-09-26 — 현재 `pokehns-expansion-kor` HNS ROM 전체 빌드 (현재)

- 요청/범위: 현재 Fork 브랜치 상태로 HNS ROM을 전체 빌드한다.
- 기준: 로컬 및 `origin/pokehns-expansion-kor`가 가리키는 `6e807edbbd` (`Document battle message status and handoff`) 상태에서 실행했다.
- 검증: `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8`가 성공했다. 생성된 `pokehns.gba`는 33,554,432바이트(32MiB)이며 빌드 시각은 2026-09-26 02:42 KST다.
- 구분: 이번 작업은 전체 컴파일·링크 성공 확인이다. 자동 배틀 테스트와 실제 HNS 화면/플레이 검증은 실행하지 않았으며, 빌드 자체로 배틀 메시지·포켓몬피리·테라스탈의 런타임 동작을 검증한 것은 아니다.
- 다음 시작점: 실제 ROM에서 필요한 배틀 메시지와 기능을 재현 검증하거나, `docs/friend-handoff/POKEEMERALD_EXPANSION_1.17.0_UPDATE.md`의 안전 업데이트 작업을 시작한다.

## 2026-09-26 — 포켓몬피리 입수 경로·테라스탈 상태 및 인수인계 정리 (현재)

- 요청/범위: 배틀 중 포켓몬피리 사용이 실제 HNS 플레이에서 가능한지와 아이템 입수 경로를 확인하고, 이전 기록의 GitHub 업로드 상태를 정정해 친구가 바로 읽을 수 있는 인수인계를 정리한다.
- 포켓몬피리 결론: `ITEM_POKE_FLUTE`와 `BattleScript_UsePokeFlute`가 있어, 아이템을 보유한 상태라면 배틀 중 사용 가능하다. 그러나 HNS 활성 맵에는 이를 지급하는 이벤트가 없다. FRLG 호환용 `LavenderTown_VolunteerPokemonHouse_Frlg` 스크립트의 Mr. Fuji 지급 코드는 남아 있지만 HNS의 `headers.inc`·`groups.inc`에 이 맵이 등록되지 않았고 HNS의 `FLAG_GOT_POKE_FLUTE`도 `0`이다.
- 실제 HNS 스토리: 관동에서는 기계 부품을 발전소에 돌려준 뒤 보라타운 라디오타워에서 확장 카드를 받아 `FLAG_KANTO_RADIO_GOT`을 세우고, 포케기어의 포켓몬피리 라디오 채널로 갈색시티 잠만보를 깨운다. 이는 아이템 포켓몬피리 지급이 아니다.
- 테라스탈 결론: 엔진과 테라스탈오브는 존재하지만 `B_FLAG_TERA_ORB_CHARGED`·`B_FLAG_TERA_ORB_NO_COST`가 모두 `0`이라 일반 플레이어 배틀에서는 사용할 수 없다. 테스트 환경의 우회와 실제 플레이 허용을 혼동하지 않는다.
- 인수인계: `docs/friend-handoff/CURRENT_HNS_HANDOFF.md`를 추가하고, 친구용 폴더 색인에서 현재 브랜치·보존 규칙·포켓몬피리·테라스탈·검증 상태를 바로 확인할 수 있게 연결했다.
- GitHub 상태 정정: 현재 로컬 `pokehns-expansion-kor`는 `origin/pokehns-expansion-kor`의 커밋 `791876da59`를 추적하며, 이 시점까지의 친구용 문서와 전체 작업 트리 업로드 커밋은 Fork에 반영되어 있다. 아래의 과거 “푸시 대기” 기록은 당시 상태를 남긴 이력이며, 현재 상태로 해석하지 않는다.
- 검증: 지급 이벤트·맵 등록·플래그·라디오·배틀 스크립트를 정적으로 대조했다. 소스·ROM은 수정하지 않았고, HNS 전체 빌드 및 실제 게임 화면 검증은 이번 확인에서 실행하지 않았다.
- 다음 시작점: 포켓몬피리 아이템을 실제 획득 가능하게 할지, 테라스탈을 일반 플레이에서 활성화할지 사용자가 결정하면 각각 별도 이벤트·플래그·UI·배틀 검증 작업으로 진행한다.

## 2026-09-26 — 친구용 1.17.0 안전 업데이트 작업 지시서 추가 (현재)

- 요청/범위: 친구가 1.15.1 이후 `pokeemerald-expansion` 변경을 `expansion/1.17.0`까지 HNS에 안전하게 이식할 수 있도록 작업 지시서를 작성한다.
- 구현: `docs/friend-handoff/POKEEMERALD_EXPANSION_1.17.0_UPDATE.md`에 기준 태그·이미 선별 이식된 항목·HNS/한글/배틀 메시지 보존 규칙·충돌 보고 형식·검증·결과 보고 양식을 기록했다.
- 핵심 원칙: upstream 전체 덮어쓰기/일괄 병합은 금지하며, HNS와 한글화·배틀 메시지 최신화 코드가 upstream과 다르면 HNS를 우선하고 충돌은 사용자 결정 전 보류한다.
- 현재 상태 정정: 이 문서와 관련 커밋은 이후 `origin/pokehns-expansion-kor`에 푸시되었다. 실제 작업 시작 전에는 현재 브랜치와 `git status --short --branch`를 다시 확인한다.

## 2026-09-26 — 친구 작업 지시서 폴더 추가 (현재)

- 요청/범위: 친구에게 맡길 작업 지시와 완료 조건을 GitHub에서 한곳에 관리할 수 있도록 `docs/friend-handoff/`를 추가한다.
- 구현: `docs/friend-handoff/README.md`에 필수 문서 링크, 작업 목록, 작업 지시서 양식과 검증·인수인계 항목을 작성했다.
- 현재 상태 정정: 폴더·작업 지시서·현재 인수인계 문서는 이후 `origin/pokehns-expansion-kor`에 푸시되었다. 친구에게 맡길 새 작업은 이 폴더에 별도 Markdown 파일로 추가하고 결과를 기록한다.

## 2026-09-26 — `pokehns-expansion-kor` 업로드 범위 정정 (현재)

- 요청/범위: 현재 HNS 작업 상태를 `PokemonHnS-Development/pokehns-expansion`의 `pokehns-expansion-kor` 브랜치로 업로드한다.
- 확인: Fork `gorunit1/pokehns-expansion-kor`의 `pokehns-expansion-kor` 브랜치와 로컬 추적 브랜치가 생성되었고, 기존 커밋에 더해 전체 작업 트리 991개 파일을 `1821fd6749`로 커밋했다.
- 현재 상태 정정: `1821fd6749`와 후속 인수인계 문서는 이후 `origin/pokehns-expansion-kor`에 푸시되었다. 현재 정확한 기준 커밋·원격 상태는 상단의 최신 항목과 `git branch -vv`로 확인한다.

## 2026-09-26 — 끈적끈적바늘 전이 연출 제거 (현재)

- 요청/범위: 도구가 없는 상대에게 `끈적끈적바늘(Sticky Barb)`이 넘어갈 때 전용 문자열과 아이템 탈취 애니메이션 없이 즉시 전이되도록 수정한다.
- 기존 문제: `TryStickyBarbOnTargetHit()`가 이미 `StealTargetItem()`으로 도구를 이동한 뒤 `BattleScript_StickyBarbTransfer`가 아이템 탈취 애니메이션과 `STRINGID_STICKYBARBTRANSFER`를 다시 출력했다.
- 구현: `BattleScript_StickyBarbTransfer`에서 아이템 탈취 애니메이션과 문자열 출력·대기를 제거하고, 기존 전이 후 정리 명령과 반환만 유지했다. 실제 아이템 이동 조건과 턴 종료 피해 처리는 변경하지 않았다.
- 현재 출력: 접촉 공격으로 조건을 만족하면 끈적끈적바늘이 공격자에게 바로 넘어가며 전용 메시지와 아이템 탈취 애니메이션은 없다. 이후 공격자는 끈적끈적바늘을 지닌 상태로 턴 종료 피해를 받는다.
- `STRINGID_STICKYBARBTRANSFER`는 이 전이 경로에서 호출되지 않고 정의만 남는다.
- 수정 파일: `data/battle_scripts_1.s`, `test/battle/hold_effect/sticky_barb.c`, `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`, `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`.
- 검증: `NODEP=1 SETUP_PREREQS=0 timeout 180s make BUILD=hns build/hns/data/battle_scripts_1.o -j1` 성공, `NODEP=1 SETUP_PREREQS=0 timeout 180s make BUILD=hns TEST=1 build/hns-test/test/battle/hold_effect/sticky_barb.o -j1` 성공, `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 전체 빌드·링크 성공. 최종 ROM 사용량은 EWRAM 249,016/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,329,828/33,554,432(99.33%)이다. 실제 HNS 화면에서 전이 직후 연출이 없는지는 아직 미검증이다.

## 2026-09-26 — 습기의 유폭 차단 메시지 제거 (현재)

- 요청/범위: `습기(Damp)`가 `유폭(Aftermath)`을 차단할 때 `STRINGID_PKMNSABILITYPREVENTSABILITY` 문구는 출력하지 않고, 특성 팝업 처리만 유지한다.
- 기존 문제: `BattleScript_DampPreventsAftermath`가 습기·유폭 특성 팝업 뒤 `STRINGID_PKMNSABILITYPREVENTSABILITY`를 출력했다. 이 문구는 최신 출력 기준에 맞지 않는다.
- 구현: 해당 스크립트에서 `printstring STRINGID_PKMNSABILITYPREVENTSABILITY`와 `waitmessage`만 제거했다. `ABILITY_AFTERMATH` 경로가 `BattleScript_DampPreventsAftermath`로 분기되는 판정은 그대로이므로 유폭의 공격자 반격 피해는 계속 발생하지 않는다.
- 명칭 정정: `ABILITY_AFTERMATH`의 공식 한글 명칭은 `유폭`이며, `멸망의바디`는 별도의 `ABILITY_PERISH_BODY`다. 이전 기록의 “멸망의바디의 애프터마스” 표현은 잘못된 설명이다.
- 현재 출력: 습기·유폭 특성 팝업 후 별도의 배틀 텍스트는 출력되지 않는다. 유폭 차단으로 공격자가 피해를 입지 않는 동작은 유지된다.
- `STRINGID_PKMNSABILITYPREVENTSABILITY`는 현재 이 경로에서 호출되지 않고 정의만 남는다. 폭발 기술 사용을 습기가 막는 별도 경로의 `STRINGID_PKMNPREVENTSUSAGE`와는 다른 문자열이다.
- 수정 파일: `data/battle_scripts_1.s`, `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`, `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`.
- 검증: `NODEP=1 SETUP_PREREQS=0 timeout 180s make BUILD=hns build/hns/data/battle_scripts_1.o -j1` 성공, `NODEP=1 SETUP_PREREQS=0 timeout 180s make BUILD=hns TEST=1 build/hns-test/test/battle/ability/damp.o -j1` 성공, `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 전체 빌드·링크 성공. 최종 ROM 사용량은 EWRAM 249,016/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,329,844/33,554,432(99.33%)이다. 실제 HNS 화면에서 특성 팝업만 표시되는지는 아직 미검증이다.

## 2026-09-26 — 배리어프리 방벽 제거 메시지 순차 출력 (현재)

- 요청/범위: `배리어프리(Screen Cleaner)`가 교대해 발동할 때 특성 팝업을 먼저 표시하고, 제거된 방벽을 리플렉터 → 빛의장막 → 오로라베일 순서로 출력하도록 연결했다.
- 기존 문제: `TryRemoveScreens()`가 양쪽 진영의 방벽을 지운 뒤 `bool`만 반환했고, `BattleScript_SwitchInAbilityMsg`가 `STRINGID_SCREENCLEANERENTERS`의 범용 문구를 출력했다. 여러 종류의 방벽을 구분하지 못했다.
- 구현: `TryRemoveScreens()`가 실제로 존재했던 방벽 종류를 `B_MSG_BREAK_REFLECT`·`B_MSG_BREAK_LIGHT_SCREEN`·`B_MSG_BREAK_AURORA_VEIL` 비트마스크로 반환한다. `BattleScript_ScreenCleanerActivates`는 배리어프리 특성 팝업 뒤 `BattleScript_BreakScreensMessages`를 호출해 존재하는 종류만 정해진 순서로 하나씩 출력한다.
- 메시지 주체: 방벽 제거 문구의 기존 `{B_ATK_PREFIX1}`가 이전 공격자를 가리키지 않도록 배리어프리 보유자를 임시 공격자 슬롯에 저장하고, 세 문구 출력 후 원래 공격자를 복원한다. 특성 팝업은 `gBattleScripting.battler`를 사용해 실제 보유자에게 표시한다.
- 현재 출력: `STRINGID_REFLECTWOREOFF` → `STRINGID_LIGHTSCREENWOREOFF` → `STRINGID_AURORAVEILWOREOFF`. 양쪽 진영에 같은 종류의 방벽이 모두 있어도 종류별 문구는 한 번씩만 출력된다. 방벽이 하나도 없으면 특성 팝업과 문구 모두 출력되지 않는다.
- `STRINGID_SCREENCLEANERENTERS`는 정의만 남고 이 배리어프리 발동 경로에서는 더 이상 선택되지 않는다. 기존 방벽별 한글 문장과 토큰은 수정하지 않고 재사용했다.
- 수정 파일: `src/battle_util.c`, `data/battle_scripts_1.s`, `include/battle_scripts.h`, `test/battle/ability/screen_cleaner.c`.
- 검증: `NODEP=1 SETUP_PREREQS=0 timeout 180s make BUILD=hns build/hns/src/battle_util.o build/hns/data/battle_scripts_1.o -j2` 성공, `NODEP=1 SETUP_PREREQS=0 timeout 180s make BUILD=hns TEST=1 build/hns-test/test/battle/ability/screen_cleaner.o -j1` 성공, `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 전체 빌드·링크 성공. 최종 ROM 사용량은 EWRAM 249,016/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,329,844/33,554,432(99.33%)이다. 실제 HNS 화면에서 팝업·접두사·세 문구 순서는 아직 미검증이다.
- 다음 시작점: 실제 교대 상황에서 방벽이 1·2·3종일 때 특성 팝업 뒤 문구 순서와 `{B_ATK_PREFIX1}` 접두사를 확인한다.

## 2026-09-26 — 정화 상태별 회복 메시지 연결 (현재)

- 요청/범위: `정화`가 대상의 상태이상을 치료할 때 기존의 `STRINGID_ATTACKERCUREDTARGETSTATUS` 단일 문구 대신 치료된 상태별 문구를 출력하도록 수정했다.
- 기존 문제: `BattleScript_EffectPurify`가 `curestatus BS_TARGET` 뒤 공격자·대상 공통의 레거시 영문 ID를 직접 출력했다. 이 ID는 현재 정화 경로에서 더 이상 호출되지 않고 정의만 남는다.
- 구현: `BS_CureStatus`가 치료 전 상태를 `GetCuredStatusMessage()`로 선택하고 치료 대상은 `gBattleScripting.battler`에 저장하므로, 정화 스크립트가 새 `gPurifyStatusCureStringIds`를 `printfromtable`로 출력하도록 연결했다. 새 문구는 모두 `{B_SCR_NAME_WITH_PREFIX}`를 사용해 정화 대상의 이름을 가리킨다.
- 현재 출력 매칭:
  - 독/맹독: `{B_SCR_NAME_WITH_PREFIX}의 독은\n말끔하게 해독됐다!`
  - 화상: `{B_SCR_NAME_WITH_PREFIX}의\n화상이 나았다!`
  - 마비: `{B_SCR_NAME_WITH_PREFIX}의\n몸저림이 풀렸다!`
  - 얼음: 기존 `STRINGID_PKMNWASDEFROSTED`(`얼음이 녹았다!`)
  - 동상: 기존 `STRINGID_PKMNFROSTBITEHEALED`(`동상이 나았다!`)
  - 잠듦: `{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 눈을 떴다!`
  - 예외 기본값: 대상 지칭의 `상태이상이 나았다!`를 사용한다. 현재 `STATUS1_ANY`에서 실제로 선택되는 상태는 위 항목들이다.
- 문자열 본문은 기존 한글화 문장을 그대로 재사용했고, 정화 대상 슬롯에 맞추기 위해 새 ID 5개에 `{B_SCR_NAME_WITH_PREFIX}`만 적용했다. 정화는 특성 발동이 아니므로 별도의 특성 팝업은 출력하지 않는다.
- 추가로 대상 빌드에서 발견된 `STRINGID_SYMBIOSISITEMPASS`의 `{B_EFF_NAME_WITH_PREFIX2}}` 오탈자를 `{B_EFF_NAME_WITH_PREFIX2}`로 바로잡았다. 문장 내용은 변경하지 않았다.
- 검증: `timeout 180s make BUILD=hns build/hns/src/battle_message.o build/hns/data/battle_scripts_1.o -j2` 성공. `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 전체 HNS 빌드·링크 성공. EWRAM 249,016/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,329,748/33,554,432(99.33%). 실제 HNS 화면에서 각 상태와 대상 이름이 표시되는 것은 아직 확인하지 않았다.
- 다음 시작점: 실제 HNS에서 공격자와 다른 대상에게 정화를 사용해 독·화상·마비·얼음·동상·잠듦별 문구와 대상 이름을 확인한다.

## 2026-09-26 — 미번역 배틀 문자열의 출력 조건 확인 (현재)

- 실제 출력 경로가 확인된 문자열:
  - `STRINGID_SYMBIOSISITEMPASS`(`src/battle_message.c:629`): 더블배틀에서 동료가 도구를 소모·잃어 지닌 도구가 없어지고, `Symbiosis` 보유자가 도구를 지니고 있으며 양쪽 모두 도구 교환이 가능할 때 동료에게 도구를 넘긴 뒤 출력된다. `MoveEnd`의 열매·보석 처리와 일반 도구 제거 후 `TrySymbiosis()` 경로가 이 ID를 사용하며, Symbiosis 특성 팝업이 먼저 나온다.
  - `STRINGID_ATTACKERCUREDTARGETSTATUS`(`:695`): 이전에는 `정화`가 대상의 상태이상을 치료하는 데 성공한 뒤 출력했지만, 현재는 상태별 `gPurifyStatusCureStringIds`가 대신 선택되고 이 ID는 정의만 남는다(`data/battle_scripts_1.s:864-870`).
  - `STRINGID_PKMNSABILITYPREVENTSABILITY`(`:789`): 이전에는 `유폭` 보유자가 접촉 공격으로 쓰러진 뒤 발동하려는 유폭을 `습기`가 막을 때 팝업 뒤 출력했지만, 현재는 최신 출력 기준에 맞춰 이 ID를 출력하지 않는다. 정의만 남아 있다(`src/battle_util.c:4073-4084`, `data/battle_scripts_1.s:5726-5734`).
  - `STRINGID_SCREENCLEANERENTERS`(`:712`): 이전에는 Screen Cleaner 보유자가 교대해 나와 방벽을 제거할 때 범용 문구로 출력했지만, 현재 배리어프리 경로는 특성 팝업 뒤 방벽 종류별 전용 ID를 순서대로 사용하며 이 ID는 정의만 남는다(`src/battle_util.c:3346-3357`, `data/battle_scripts_1.s:6954-6959`). 방벽이 없으면 팝업과 문구 모두 출력되지 않는다.
  - `STRINGID_PKMNSWILLPERISHIN3TURNS`(`:715`): 멸망의바디가 접촉 공격으로 발동해 공격자와 멸망의바디 보유자 양쪽에 3턴 멸망 카운트를 부여할 때 출력된다(`src/battle_util.c:4271-4286`, `data/battle_scripts_1.s:4939-4943`). 문자열의 “더 이상 표시되지 않는 것 같다” 주석과 달리 현재 호출부가 있다.
  - `STRINGID_STICKYBARBTRANSFER`(`:724`): 이전에는 점착바브 보유자가 접촉 공격으로 피해를 받고, 공격자가 도구를 지니지 않았으며 도구를 받을 수 있을 때 전이 메시지를 출력했지만, 현재는 전용 메시지와 아이템 탈취 애니메이션 없이 즉시 전이된다. 이 ID는 정의만 남는다(`src/battle_hold_effects.c:576-591`, `data/battle_scripts_1.s:7828-7831`).
  - `STRINGID_SNOWCONTINUES`(`:811`): 눈이 내리는 배틀 날씨의 지속 턴에 날씨 지속 시간이 끝나지 않았을 때 턴 종료 처리로 출력된다. 눈이 시작되거나 그칠 때의 문구와는 다르다(`src/battle_util.c:244-267`, `data/battle_scripts_1.s:4482-4488`, `src/battle_message.c:1070-1079`).
  - `STRINGID_FOGISDEEP`(`:860`): 안개 배틀 날씨가 계속되는 턴 종료 시 출력되며, 가로·대각선 오버월드 안개가 시작될 때도 `gWeatherStartsStringIds`에서 선택된다(`src/battle_message.c:1314-1325`).
  - `STRINGID_PKMNBLEWAWAYSHARPSTEEL`(`:824`): `Rapid Spin`이 G-Max Steelsurge의 뾰족한 강철 장판을 제거할 때만 `gSpinHazardsStringIds`에서 선택된다. Defog에는 이미 별도 `STRINGID_SHARPSTEELDISAPPEAREDFROMTEAM`이 연결되어 있다(`src/battle_message.c:1450-1465`, `data/battle_scripts_1.s:5116-5120`).
  - `STRINGID_PKMNTERASTALLIZEDINTO`(`:857`): 테라스탈 발동 애니메이션이 끝난 뒤, 일반 테라스탈 경로와 폼 체인지가 동반된 테라스탈 경로에서 출력된다(`data/battle_scripts_1.s:46-71`, `src/battle_terastal.c:35-44`). 주석상 SV 실기에는 이 후속 문구가 없지만 현재 HNS 스크립트는 출력하도록 되어 있다.
  - `STRINGID_POKEFLUTECATCHY`, `STRINGID_POKEFLUTE`, `STRINGID_MONHEARINGFLUTEAWOKE`(`:865-867`): 배틀 중 포켓몬피리를 사용할 때 `checkpokeflute` 결과에 따라 선택된다. 잠든 포켓몬을 깨울 대상이 없으면 Catchy 문구만 출력되고, 깨울 대상이 있으면 피리 사용 문구·팬파레·깨어났다는 문구가 차례로 출력된다(`data/battle_scripts_2.s:124-141`).
  - `STRINGID_BLOCKEDBYSLEEPCLAUSE`(`:876`): Sleep Clause가 켜져 있고 해당 진영에서 이미 잠든 포켓몬이 있어 추가 수면이 금지될 때 출력된다. 기술·특성·추가 효과의 즉시 수면 시도는 `BattleScript_SleepClauseBlocked`, 하품의 턴 종료 수면은 `BattleScript_SleepClausePreventsEnd2`를 사용한다. 더블배틀에서 아군을 재우는 예외는 별도 면제된다(`src/battle_util.c:5506-5518,5637-5653`, `src/battle_end_turn.c:888-915`). 현재 기본 설정 `B_SLEEP_CLAUSE=FALSE`, `B_FLAG_SLEEP_CLAUSE=0`에서는 일반 HNS 배틀에서 활성화되지 않는다.
  - `STRINGID_REFLECTWOREOFF`, `STRINGID_LIGHTSCREENWOREOFF`, `STRINGID_AURORAVEILWOREOFF`(`src/battle_message.c:529-531`): `깨뜨리다`, `사이코팽`, `레이징불`이 제거한 방벽을 리플렉터 → 빛의장막 → 오로라베일 순서로 각각 출력한다. 둘 이상이 동시에 있으면 존재하는 종류만 차례로 출력하며, 이제 이 경로에서는 `STRINGID_THEWALLSHATTERED`를 사용하지 않는다(`src/battle_script_commands.c:3594-3625`, `data/battle_scripts_1.s:3797-3831`). 방벽이 없거나 기술이 무효라 제거에 실패하면 아무 문구도 출력되지 않는다.
- 이번 변경 검증: 방벽 제거 처리·배틀 스크립트 타깃 빌드, 변경한 두 배틀 테스트 오브젝트 빌드, 전체 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 빌드·링크가 모두 성공했다. 전체 빌드 사용량은 EWRAM 249,016/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,329,572/33,554,432(99.33%)이다. 실제 HNS 화면에서 문구가 연속 표시되는지는 아직 확인하지 않았다.
- 현재 기능이 비활성화되거나 조건부인 문자열:
  - `STRINGID_TIMETODYNAMAX`, `STRINGID_TIMETOGIGANTAMAX`(`:884-885`): Dynamax 시작 시 변신 전에 출력된다. `STRINGID_PKMNDYNAMAXED`, `STRINGID_PKMNGIGANTAMAXED`(`:882-883`)는 변신 애니메이션 뒤 추가 출력 문구지만 `B_SHOW_DYNAMAX_MESSAGE=FALSE`라 현재 기본 설정에서는 나오지 않는다. 플레이어 Dynamax 자체도 `B_FLAG_DYNAMAX_BATTLE=0`이면 허용되지 않는다(`data/battle_scripts_1.s:8229-8254`, `src/battle_dynamax.c:75-103`, `include/config/battle.h:273,349`).
  - `STRINGID_ITISHAILING`(`:869`): Gen9 미만 오버월드 눈을 배틀 날씨로 바꿀 때의 구세대 분기다. 현재 `B_OVERWORLD_SNOW=GEN_LATEST`라 `STRINGID_ITISSNOWING`이 선택되어 이 ID는 HNS 기본 설정에서 출력되지 않는다(`src/battle_message.c:1314-1321`, `include/config/battle.h:320`).
- 정의만 남았거나 현재 호출되지 않는 문자열:
  - `STRINGID_POISONHEALHPUP`(`:612`), `STRINGID_SOLARPOWERHPDROP`(`:619`), `STRINGID_ICEBODYHPGAIN`(`:623`): 각각 독/맹독 상태의 포이즌힐 회복, 햇빛 아래 솔라파워·건조피부의 HP 변화, 눈 날씨의 아이스바디 회복과 관련된 구문이지만 현재 `PoisonHealActivates`, `SolarPowerActivates`, `IceBodyHeal` 스크립트는 특성 팝업·HP 처리만 하고 이 ID를 출력하지 않는다(`data/battle_scripts_1.s:4509-4514,5746-5752,6220-6264`).
  - `STRINGID_NOTDONEYET`(`:647`): `EFFECT_PLACEHOLDER` 기술을 선택하거나 사용하려 할 때의 미완성 기능 안내다(`data/battle_scripts_1.s:2197-2201,7341-7347`, `src/battle_util.c:1648-1660`). 현재 HNS 기술 데이터에는 이 효과를 가진 일반 기술이 없어 통상 배틀에서는 출력되지 않는다.
  - `STRINGID_PKMNBLEWAWAYTOXICSPIKES`, `STRINGID_PKMNBLEWAWAYSTICKYWEB`, `STRINGID_PKMNBLEWAWAYSTEALTHROCK`(`:650-652`): 과거 Rapid Spin 전용 문자열이다. 현재 Rapid Spin 테이블은 세 장판 모두 `...DISAPPEAREDFROMTEAM`으로 연결하므로 이 세 ID에는 호출부가 없다.
  - `STRINGID_THROATCHOPENDS`(`:685`): 의도상 지옥찌르기 효과의 지속 턴이 끝날 때 “소리 기술을 다시 쓸 수 있다”를 알리는 문구지만, 현재 HNS에서는 턴 종료 시 타이머만 감소하고 `BattleScript_ThroatChopEndTurn`을 호출하는 코드가 없어 출력되지 않는다(`src/battle_end_turn.c:60-61`, `data/battle_scripts_1.s:5217-5220`).
- FRLG·Safari 전용 또는 별도 모드 문자열:
  - `sText_GhostAppearedCantId`, `sText_TheGhostAppeared`(`src/battle_message.c:98-99`): `BATTLE_TYPE_GHOST` 전투 시작 시 실프스코프가 없을 때와 있을 때의 유령 출현 안내다.
  - `STRINGID_MONTOOSCAREDTOMOVE`, `STRINGID_GHOSTGETOUTGETOUT`(`:890-891`): 실프스코프가 없는 유령 배틀에서 각각 플레이어 포켓몬의 기술 시도와 유령 포켓몬의 기술 시도에 대한 행동 취소 문구다(`src/battle_move_resolution.c:417-426`, `data/battle_scripts_1.s:8419-8428`).
  - `STRINGID_SILPHSCOPEUNVEILED`, `STRINGID_GHOSTWASMAROWAK`(`:892-893`): 포켓몬타워 유령 배틀에서 실프스코프가 있으면 전투 도입부에 유령의 정체를 공개할 때 출력된다(`src/battle_main.c:3907-3914`, `data/battle_scripts_1.s:8430-8438`).
  - `STRINGID_THREWROCK`, `STRINGID_THREWBAIT`, `STRINGID_PKMNANGRY`, `STRINGID_PKMNEATING`(`:895-898`): Safari 배틀에서 바위·미끼 행동을 선택하거나 그 결과 야생 포켓몬의 반응이 화남·먹는 중으로 결정됐을 때 출력된다(`data/battle_scripts_2.s:331-341`, `src/battle_message.c:1357-1362`).
- 이번 확인은 호출부·선택 테이블의 정적 대조이며, 문자열을 수정하지 않았다. 실제 HNS 화면에서 팝업과 문구의 표시 순서는 아직 확인하지 않았다.

## 2026-09-25 — 특성 발동 메시지 최신 기준 연결 및 하품 경계 수정 (현재)

- 요청 범위: 절대안깸, 리프가드, 플라워베일, 리밋실드, 면역, 불면, 의기양양, 스위트베일, 유연, 마그마의무장, 수의베일, 수포, 열교환, 파스텔베일, 황금몸을 포함한 특성 발동 메시지와 그 밖의 특성 메시지 경로를 최신 SV·Pokémon Champions 원문 및 현재 HNS 스크립트와 대조했다.
- 하품 경계: `HandleEndTurnYawn()`에서 잠듦을 적용하기 전에 `ABILITY_COMATOSE`도 불면·의기양양·소란피우기·리프가드와 함께 검사한다. 하품 상태가 남아 있는 상태에서 절대안깸을 얻어도 잠듦 전환·실패 메시지·특성 팝업이 발생하지 않는다.
- 상태이상 방지: 공통 `gStatusProtectsStringIds`/`BattleScript_StatusProtects`를 사용해 특성 팝업 뒤 독·화상·마비·얼음·잠듦별 최신 문구를 출력한다. 절대안깸·리프가드·리밋실드·면역·정화의소금·유연·수의베일·수포·열교환·파스텔베일이 이 경로를 사용한다. 리프가드·리밋실드의 잠자기 전용 경로도 같은 팝업·잠듦 문구로 연결했다. 플라워베일은 전용 보호 문구, 스위트베일은 특성 팝업 뒤 `STRINGID_PKMNSXMADEITINEFFECTIVE`, 불면·의기양양은 특성 팝업 뒤 `STRINGID_PKMNSTAYEDAWAKEUSING`을 유지한다. 마그마의무장은 아래 재검토 결과에 따라 이 공통 팝업 경로에서 제외해야 한다.
- 마그마의무장 재검토 및 수정: 최신 SV·Champions에서는 얼음 상태를 막는 효과 자체는 있지만, 얼음 부여 시 마그마의무장 발동을 알리는 별도 특성 팝업이나 특성 전용 문구는 사용하지 않는다. `ABILITY_MAGMA_ARMOR` 분기를 `BattleScript_NotAffected`로 바꿔 얼음 부여만 차단하고 `BattleScript_StatusProtects`·`얼지 않는다!`를 거치지 않게 했다. 이미 얼어 있던 포켓몬이 마그마의무장을 새로 얻어 해동되는 별도 상태 회복 경로는 이번 변경 대상이 아니다.
- 황금몸 재확인: 상대의 대상 지정 변화 기술이 황금몸에 막힐 때 `CanAbilityAbsorbMove()`가 `BattleScript_GoodAsGoldActivates`를 선택하고, 이 스크립트가 황금몸 특성 팝업 뒤 `STRINGID_SCR_ITDOESNTAFFECT`를 출력한다. 현재 `src/battle_message.c`의 `{B_SCR_NAME_WITH_PREFIX}에게는\n효과가 없는 것 같다...`는 SV·Champions의 같은 대상별 문구와 일치하므로 소스 수정은 하지 않았다. 필드 대상·전체 대상 기술은 이 경로에 들어가지 않는다.
- 상태 회복: 면역·파스텔베일·유연·불면·의기양양·수의베일·수포·열교환·마그마의무장의 상태 회복 경로는 팝업 뒤 실제 회복 상태별 문구를 선택한다. 혼란 회복은 `혼란이 풀렸다!`, 헤롱헤롱과 도발 회복은 각각 전용 최신 문구를 사용한다. 절대안깸은 상태를 회복시키는 특성이 아니라 상태이상 방지 경로를 사용하며, 치유의마음은 `치유되었다!`를 출력한다.
- 기타 특성 감사: 현재 실제 호출되는 예측·프리즈마아머 계열·날씨·패러독스·스탯 변화·위협·통찰·긴장감·변환·황금몸 등의 팝업/문구 경로를 확인했다. 최신 원문과 직접 대응하지 않는 레거시·미사용·기능별 원문 부재 문자열은 임의로 번역하거나 창작하지 않고 기존 상태로 남겼다.
- 구현 위치: `src/battle_end_turn.c`, `src/battle_util.c`, `src/battle_script_commands.c`, `src/battle_message.c`, `data/battle_scripts_1.s`, `include/constants/battle_string_ids.h`, `include/battle_scripts.h`.
- 원문 근거: [SV 한국어 원문](https://raw.githubusercontent.com/abcboy101/poke-corpus/main/corpus/ScarletViolet/ko_common.txt), [Champions 한국어 원문](https://raw.githubusercontent.com/abcboy101/poke-corpus/main/corpus/Champions/ko_ms.txt). 아래의 같은 날짜 항목들은 이번 보완 전 상태를 기록한 역사이며, 현재 구현과 충돌하는 설명은 이 항목을 우선한다.
- 검증: `battle_util.o` 재컴파일 성공, `git diff --check` 통과, 전체 HNS 링크 성공. `pokehns.gba`는 33,554,432바이트로 생성되었고 링크 보고 ROM 사용량은 33,329,508바이트(99.33%)다. 실제 게임 화면 검증은 아직 하지 않았다.
- 다음 시작점: 표에 적힌 특성별 상태이상 방지·회복·수면 시나리오를 HNS 화면 또는 런타임 테스트로 재현해 팝업 주체와 문구 순서를 확인한다.

## 2026-09-25 — 절대안깸 최신 동작 대조

- 확인 범위: `ABILITY_COMATOSE`(`절대안깸`)의 상태이상 방지, 잠자기 관련 기술, 특성 억제·무시 여부와 배틀 메시지 경로를 현재 HNS 코드 및 최신 SV·Champions 원문과 대조했다. 이번 항목에서는 소스 코드를 수정하지 않았다.
- 현재 일치: `CanSetNonVolatileStatus()`가 독·맹독·화상·마비·얼음·잠듦을 차단하고, `Rest`는 `이미 잠들어 있다` 경로로 실패한다. `Snore`·`Sleep Talk`, `Dream Eater`·`Nightmare`·`Bad Dreams`, Hex/Wake-Up Slap의 수면 취급도 현재 코드에 반영되어 있다. `cantBeSuppressed` 및 비복사·비교환 플래그도 설정되어 있어 일반적인 틀깨기·중화가 절대안깸을 무시하지 않는다.
- 출력 차이: 상태이상 시 현재 `BattleScript_AbilityProtectsDoesntAffect`가 특성 팝업 뒤 모든 상태를 `STRINGID_ITDOESNTAFFECT`(`효과가 없는 것 같다...`)로 보낸다. 최신 SV·Champions 원문은 독·화상·마비·얼음·잠듦에 각각 `독에 중독되지 않는다!`·`화상을 입지 않는다!`·`마비되지 않는다!`·`얼지 않는다!`·`잠들지 않는다!`를 사용하므로, 현재 상태별 ID/선택 테이블은 정화의소금에만 연결되어 절대안깸에는 아직 최신 출력이 적용되지 않았다.
- 예외 경로: `HandleEndTurnYawn()`에는 `ABILITY_COMATOSE` 검사가 없다. 일반적인 하품 사용 시도는 앞선 `CanSetNonVolatileStatus()`에서 막히지만, 절대안깸을 얻기 전에 이미 설정된 하품 volatile이 남는 변환 등의 경계 사례에서는 턴 종료에 실제 잠듦으로 전환될 가능성을 별도 수정해야 한다.
- 일치하는 문구: 교체 시 `STRINGID_COMATOSEENTERS`의 `비몽사몽 상태!`는 SV 원문의 같은 문구와 일치한다. 실제 HNS 화면에서 특성 팝업 순서와 상태별 실패 문구는 아직 검증하지 않았다.
- 참고: [SV 한국어 원문](https://raw.githubusercontent.com/abcboy101/poke-corpus/main/corpus/ScarletViolet/ko_common.txt), [Champions 한국어 원문](https://raw.githubusercontent.com/abcboy101/poke-corpus/main/corpus/Champions/ko_ms.txt). 주요 코드 위치는 `src/battle_util.c:5404-5594`, `src/battle_move_resolution.c:1196-1216`, `src/battle_end_turn.c:857-918`, `data/battle_scripts_1.s:2316-2350`이다.
- 다음 시작점: 절대안깸에도 상태별 실패 문자열 선택을 연결하고, 하품 턴 종료 경로에 절대안깸 방어를 추가할지 결정한 뒤 관련 오브젝트 빌드와 실제 화면 검증을 수행한다.

## 2026-09-25 — battle_message.c 최신 원문 대조 및 정화의소금 상태별 문구 연결

- 요청/범위: `src/battle_message.c`의 이번 번역 작업에서 최신 SV·Pokémon Champions 한국어 원문과 일치하는 영문 항목을 교체하고, 최신 동작이 문구 선택에 영향을 주는 특성 경로를 함께 수정했다. 기존에 한글화된 문장 자체는 임의로 고치지 않았다.
- 원문 대조: 작업 트리에 corpus 파일이 없어 Poké Corpus의 [SV 한국어 원문](https://raw.githubusercontent.com/abcboy101/poke-corpus/main/corpus/ScarletViolet/ko_common.txt)과 [Champions 한국어 원문](https://raw.githubusercontent.com/abcboy101/poke-corpus/main/corpus/Champions/ko_ms.txt)을 임시 대조본으로 사용했다. Champions `btl_set`의 상태 면역 문구(독·화상·마비·얼음·잠듦)와 SV의 눈 날씨 문구를 확인했다.
- 문자열: `STRINGID_SNOWWARNINGHAIL`을 최신 날씨 문구인 `눈이 내리기 시작했다!`로 교체했다. `THUNDERCAGETRAPPED`, `PKMNITEMMELTED`, `HOSPITALITYRESTORATION`의 조사 토큰을 런타임 대상에 맞는 `{B_TXT_EULREUL}`, `{B_TXT_IGA}`로 정정했고, 컴파일러가 지원하지 않는 `\\r` 제어 코드를 `\\n`으로 바로잡았다.
- 정화의소금: 기존에는 `ABILITY_PURIFYING_SALT`가 독·맹독·화상·마비·얼음·잠듦을 모두 `STRINGID_ITDOESNTAFFECT`로 출력했다. 새 상태별 ID 5개와 `gPurifyingSaltProtectsStringIds`를 추가하고, `CanSetNonVolatileStatus()`가 상태별 선택자를 설정하도록 변경했다. `BattleScript_PurifyingSaltProtects`가 정화의소금 특성 팝업 뒤 독/화상/마비/얼음/잠듦 문구를 출력한다. `Rest`의 정화의소금 경로도 일반 효과 없음 문구 대신 `잠들지 않는다!`를 출력한다.
- 유지/미번역: `SNOWCONTINUES`, `FOGISDEEP`, 점착막대 전이, 목소리 기술 봉인 종료, Screen Cleaner, 날카로운 강철 제거, Symbiosis 전용 문구, Tera/Dynamax/FRLG/포켓몬피리/Sleep Clause 등은 SV·Champions의 실제 최신 배틀 원문을 찾지 못했으므로 새 번역을 창작하지 않고 영문으로 남겼다. 대부분 구세대·미사용·기능별 원문 부재 항목이며, 다음 세션에서 각 호출부 사용 여부를 별도로 확인한다.
- 검증: `make BUILD=hns build/hns/src/battle_message.o build/hns/src/battle_util.o -j2`, `make BUILD=hns build/hns/data/battle_scripts_1.o -j1`, `make BUILD=hns build/hns/src/battle_move_resolution.o -j1`이 성공했다. 이어서 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 전체 HNS 빌드·링크도 종료 코드 0으로 성공했고 최신 링크 보고 ROM 사용량은 33,329,556바이트/32MiB(99.33%)다. 이전 전체 빌드 로그는 `build/localization-logs/hns-battle-message-modern-ko-20260925.log`에 남겼고, 마지막 링크 명령은 터미널에서 직접 확인했다.
- 다음 시작점: 실제 화면에서 정화의소금의 5가지 상태 실패가 “특성 팝업 → 상태별 문구” 순서인지 확인한다. 그 다음 영문 잔여 목록의 호출부를 사용/미사용으로 확정한다.

## 2026-09-25 — 배틀 메시지 한글 토큰 연결 감사

- 요청/범위: 현재 `src/battle_message.c`에 한글화된 배틀 문장의 `{B_...}` 토큰만 실제 버퍼 작성 코드·호출부와 대조했다. 문장, 조사, 줄바꿈, 문자열 ID 자체는 변경하지 않았다.
- 수정: `STRINGID_PKMNWASPARALYZEDBY`의 첫 이름을 `B_SCR`에서 `B_EFF`로 바꿨다. 이 문장의 첫 포켓몬은 마비된 효과 대상이고, 둘째 포켓몬은 `B_SCR_NAME_WITH_PREFIX`/`B_SCR_ABILITY`로 특성 보유자를 표시해야 한다. `STRINGID_PKMNMOVEBOUNCED`는 `B_DEF`에서 `B_ATK`로 바꿨다. `BS_SetMagicCoatTarget()`가 반사자를 새 공격자 버퍼에 넣은 뒤 이 문장을 출력하기 때문이다.
- 유지: `STRINGID_PKMNSXMADEITINEFFECTIVE`의 `B_SCR_NAME_WITH_PREFIX`·`B_SCR_ABILITY`는 스위트베일 특성 보유자에 맞고, `STRINGID_PKMNSXPREVENTSFLINCHING`의 `B_EFF_NAME_WITH_PREFIX`·`B_EFF_ABILITY`도 효과 대상의 정신력 특성에 맞다. `STRINGID_PKMNSTAYEDAWAKEUSING`의 능력명 생략은 앞선 특성 팝업을 사용하는 현재 스크립트 의도에 맞다. 그 밖의 차이는 특성 팝업/현대식 일반 문구 또는 현재 선택 테이블에서 사용하지 않는 구형 ID로 확인되어 토큰을 바꾸지 않았다.
- 참고 제약: 이 작업 트리에는 요청된 `corpus/ScarletViolet/ko_common.txt`, `corpus/Champions/ko_ms.txt`, Project Pokémon `champout`가 없어, 로컬 `pokeemerald-kr/src/battle_message.c`와 현재 HNS 호출부·버퍼 작성 코드를 대조 자료로 사용했다.
- 검증: `git diff --check -- src/battle_message.c` 통과. `make BUILD=hns build/hns/src/battle_message.o -j2` 종료 코드 0으로 성공했다. 맵 산출물 선행 조건이 갱신되어 대상 오브젝트 빌드 전에 맵 의존성이 재생성되었다. 검증 근거는 `docs/localization/evidence/2026-09-25-battle-message-token-audit.txt`에 기록했다.
- 게임 화면 확인: 미확인. 매직코트/특성에 의한 마비 문구의 실제 화면 표시와 팝업 순서는 HNS에서 재현하지 않았다. 현재 `gGotParalyzedStringIds[B_MSG_STATUSED_BY_ABILITY]`는 일반 마비 ID를 선택하므로 `STRINGID_PKMNWASPARALYZEDBY`는 런타임상 비활성 상태이며, 토큰은 원문 의도에 맞게 정정한 것이다.
- 다음 시작점: 실제 화면 검증 시 매직코트와 특성에 의한 상태이상 경로를 재현한다. 전체 HNS 빌드가 필요하면 맵 생성 완료 후 `.map_version`을 보존하는 기존 워크플로우 명령을 사용한다.

## 2026-09-25 — 코드 경로 변경으로 달라진 배틀 메시지 출력 목록화

- 사용자의 범위를 “단순 한글화 제외, 기술·특성·도구 효과가 기존과 다른 메시지를 선택하거나 팝업 전용이 된 코드/스크립트 변경”으로 확정했다.
- 새 `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 일격기·장벽/장판·상태이상/회복·잠자기·특성/도구/도주·Champions의 구현 코드 변경(효과 단계·아이템 팝업·생명의구슬 반동 문구)을 이전 출력→현재 출력→변경 지점으로 정리했다. 1.17.0 이식과 PR #9777에서 문자열만 추가·번역한 부분, 출력 조건 조사만 한 ID, 같은 ID의 번역·조사·줄바꿈 변경은 제외했다.
- 검증: 현재 `src/battle_message.c`, 메시지 선택 테이블, 배틀 스크립트 및 기존 세션 기록을 대조했다. 문서 작업만 했으므로 이번 항목에서 ROM 빌드·게임 화면 검증은 하지 않았다.
- 다음 시작점: 실제 화면 검증을 진행할 때는 이 문서의 각 행을 재현 시나리오 목록으로 사용하고, 새 출력 경로를 추가/변경하면 이 목록도 함께 갱신한다.

## 2026-09-23 — 불면·의기양양 및 스위트베일의 수면 방지 메시지 분리

- 불면(`ABILITY_INSOMNIA`)·의기양양(`ABILITY_VITAL_SPIRIT`)으로 수면이 막히면, 일반 수면 상태 부여와 잠자기 모두 새 `BattleScript_StayedAwakeUsingAbility`를 사용한다. 출력 순서는 해당 포켓몬의 특성 팝업 → `STRINGID_PKMNSTAYEDAWAKEUSING`이다.
- 스위트베일(`ABILITY_SWEET_VEIL`)은 일반 수면 상태 부여, 하품의 턴 종료 수면, 잠자기에서 모두 특성 보유자를 `gBattlerAbility`/스크립트 배틀러로 지정한 뒤 특성 팝업 → `STRINGID_PKMNSXMADEITINEFFECTIVE`를 출력한다. 더블배틀에서 아군 스위트베일이 막는 경우에도 팝업은 실제 특성 보유자에게 표시되며, 수면을 막힌 각 포켓몬의 시도마다 이 순서가 적용된다.
- 정화의소금은 이번 요청 범위 밖으로 두어, 기존처럼 특성 팝업 뒤 `STRINGID_ITDOESNTAFFECT` 경로를 유지한다. 리프가드·리밋실드는 잠자기 실패 처리도 변경하지 않았다.
- 검증: 수면 관련 오브젝트 5개(`battle_util.o`, `battle_move_resolution.o`, `battle_end_turn.o`, `battle_script_commands.o`, `battle_scripts_1.o`) 컴파일 성공. 맵 생성 규칙을 단일 작업으로 복구한 뒤 `NODEP=1 SETUP_PREREQS=0 timeout 600s make -o .map_version --jobserver-style=pipe hns -j8` 전체 HNS 빌드·링크 성공, ROM 사용량은 33,330,324바이트/32 MiB(99.33%)다. `git diff --check`도 대상 변경 파일에서 통과했다.
- 게임 화면 검증: 미확인. 다음 시작점은 불면/의기양양의 일반 수면·잠자기, 스위트베일의 단일/더블 수면·하품·잠자기를 실제 HNS 화면에서 재현해 팝업 주체와 문구 순서를 확인하는 것이다. 표준 `make hns -j8`은 이 작업 중 맵 생성 규칙이 `map_groups.h`를 일시 삭제하는 문제가 있어, 위처럼 생성 완료 후 `.map_version`을 보존한 빌드 명령을 사용했다.

## 2026-09-23 — `STRINGID_PKMNSXMADEITINEFFECTIVE`·`STRINGID_PKMNSXPREVENTSFLINCHING` 출력 조건 및 특성 팝업 확인

- `STRINGID_PKMNSXMADEITINEFFECTIVE`의 직접 출력부는 `BattleScript_PrintAbilityMadeIneffective` 하나다. 수면 방지와 텔레포트 도주 방지는 판정 코드가 다르며, 8세대 이후 텔레포트는 더는 이 출력 스크립트에 진입하지 않는다.
  - 수면 상태 부여: `CanSetNonVolatileStatus()`가 `MOVE_EFFECT_SLEEP` 대상의 `의기양양`(`ABILITY_VITAL_SPIRIT`) 또는 `불면`(`ABILITY_INSOMNIA`)을 확인했을 때다. 수면 기술·추가 효과 등 이 공통 판정을 거치는 모든 수면 시도에 적용된다.
  - 텔레포트(8세대 이후): 야생 포켓몬의 도주 경로는 전용 `IsTeleportRunningFromBattleImpossible()`를 사용한다. `B_TELEPORT_BEHAVIOR >= GEN_8`이면 개미지옥·그림자밟기·자력의 도주 방지 특성 검사를 건너뛰므로 `BATTLE_RUN_FAILURE`가 되지 않고, 이 ID와 특성 팝업을 거치지 않은 채 정상 도주한다. Gen 7 이하는 기존 일반 도주 판정을 그대로 사용한다. 트레이너 배틀 또는 플레이어 측 텔레포트는 계속 `BattleScript_EffectBatonPass`로 간다.
- 점착의 도구 조작 방지는 이름이 비슷한 `STRINGID_PKMNSXMADEYINEFFECTIVE`를 사용한다. `BattleScript_StickyHoldActivatesRet`와 `BattleScript_NoItemSteal`이 `탁쳐서떨구기`, `트릭`·`바꿔치기`, `부식가스`, `도둑질`·`탐내다`를 막을 때 이 별도 ID를 출력한다. 벌레먹음·불태우기처럼 점착이 효과를 막지만 이 문자열을 호출하지 않는 경로도 있다.
- 수면 경로는 대상 슬롯을 `gBattlerAbility`에 넣어 특성 팝업을 요청한다. 8세대 이후 텔레포트는 특성 도주 방지 판정 자체를 건너뛰므로, 공통 실패 스크립트와 특성 팝업을 호출하지 않는다.
- `STRINGID_PKMNSXPREVENTSFLINCHING`의 직접 출력부는 `BattleScript_FlinchPrevention` 하나다. `SetMoveEffect()`에서 효과 대상의 특성이 `정신력`(`ABILITY_INNER_FOCUS`)이고, 플린치 효과가 `EFFECT_PRIMARY` 또는 `EFFECT_CERTAIN`으로 전달될 때만 이 스크립트에 진입한다. 현재 기술 데이터에서 대표적인 확정 플린치는 `속이기`의 100% 추가 효과다. 확률형 플린치가 우연히 발동한 경우나 왕의징표석·예리한이빨 또는 특성의 별도 플린치 부여처럼 `NO_FLAGS`로 들어오는 경우에는 이 ID를 선택하지 않는다.
- 두 번째 문자열도 `gBattlerAbility = gEffectBattler` 후 `BattleScript_AbilityPopUp`을 호출하므로 정신력 팝업이 먼저 나오고 문장이 출력된다. 방진·은밀망토의 추가 효과 차단, 비비드바디·여왕의위엄·테일아머 계열의 우선도 차단은 이 문자열과 다른 경로다.
- 검증: `src/battle_main.c`·`src/battle_script_commands.c`의 HNS 오브젝트 빌드와, 개미지옥·그림자밟기·자력 각각을 매개변수화한 `test/battle/move_effect/teleport.c` 테스트 오브젝트 빌드에 성공했다. 선별 런타임 테스트는 기존 `test/test_runner.c`의 `fake_rtc.h` 포함 순서 오류(`struct SaveBlock3` 미정의)로 테스트 실행 전에 중단됐다. `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 전체 HNS 빌드는 성공했고 ROM 사용량은 33,330,196바이트/32 MiB(99.33%)다.
- 게임 화면 확인: 미확인. 실제 HNS ROM에서 야생 포켓몬이 세 특성 각각을 무시하고 텔레포트로 도주하는지 확인할 수 있다.
- 다음 시작점: 기존 테스트 러너의 헤더 포함 문제를 별도로 해결한 뒤 선별 런타임 테스트를 실행하고, 실제 HNS 화면에서 세 특성의 텔레포트 도주를 재현한다. 개미지옥·그림자밟기·자력 이외의 `BATTLE_RUN_FORBIDDEN` 조건은 이번 변경 범위 밖이다.

## 2026-09-23 — 리샘열매 상태 회복을 상태별 메시지로 분리

- `TryCureAnyStatus()`가 리샘열매로 치료할 상태를 `gBattleScripting.lumBerryCureStatusMask`에 보존하고, 새 `BattleScript_LumBerryCureStatusRet`가 열매 팝업·발동 애니메이션을 한 번 실행한 뒤 상태별 문구를 하나씩 출력하도록 변경했다. 기존의 `B_MSG_NORMALIZED_STATUS` → `STRINGID_PKMNSITEMNORMALIZEDSTATUS` 선택은 이 경로에서 제거했다.
- 매핑: 마비→`STRINGID_PKMNSITEMCUREDPARALYSIS`, 독/맹독→`STRINGID_PKMNSITEMCUREDPOISON`, 화상→`STRINGID_PKMNSITEMHEALEDBURN`, 얼음→`STRINGID_PKMNSITEMDEFROSTEDIT`, 잠듦→`STRINGID_PKMNSITEMWOKEIT`, 혼란→새 `B_MSG_CURED_CONFUSION` 선택자→`STRINGID_PKMNSITEMSNAPPEDOUT`. 동상은 기존 전용 `STRINGID_PKMNSITEMHEALEDFROSTBITE`를 유지했다.
- 복수 상태(통상 비휘발성 상태이상+혼란)는 두 문구를 순서대로 출력한다. 예: 독+혼란이면 “리샘열매로 독이 해독됐다!” 다음 “리샘열매로 혼란이 풀렸다!”가 나온다. 상태 아이콘 갱신과 열매 소모는 모든 메시지 뒤 한 번만 실행된다.
- 검증: 관련 오브젝트 컴파일 후 사용자가 `src/battle_message.c:795`의 문자열 문법을 고친 상태에서 `GITHUB_ACTION=1 timeout 600s make hns -j8` 전체 빌드에 성공했다. 링크 보고 ROM 사용량은 33,330,276바이트/32 MiB(99.33%)다. 실제 게임 화면 검증은 미완료다.
- 다음 시작점: 리샘열매의 마비·독·화상·얼음·잠듦·혼란 및 독+혼란 사례를 실제 HNS 화면에서 재현해 팝업, 메시지 순서, 상태 아이콘 갱신, 열매 소모를 확인한다.

## 2026-09-23 — 만병통치제의 상태 회복 메시지 확인

- 배틀 중 가방에서 `만병통치제`(`ITEM_FULL_HEAL`, 현재 아이템명)를 사용하면 `EFFECT_ITEM_CURE_STATUS` → `BattleScript_ItemCureStatus` 경로를 사용한다. 상태이상 치료 성공 시 전장 포켓몬인지 파티 포켓몬인지와 무관하게 `STRINGID_ITEMCUREDSPECIESSTATUS`를 출력한다.
- 현재 해당 문자열은 아직 영문 `"{B_BUFF1} had its status healed!"`이다. `{B_BUFF1}`은 닉네임이 아닌 치료된 포켓몬의 종 이름 버퍼다. 따라서 만병통치제 이름이나 개별 상태명은 이 치료 완료 문구에 들어가지 않는다.
- `STRINGID_PKMNSITEMNORMALIZEDSTATUS`는 지닌 리샘열매의 복수 상태 치료 전용이므로, 만병통치제를 사용해 독·화상·마비·잠듦·얼음 또는 혼란을 치료해도 선택되지 않는다.
- 검증: `src/data/items.h:1093-1107`, `data/battle_scripts_2.s:90-103`, `src/battle_script_commands.c:12150-12202`, `src/battle_message.c:795`를 정적 확인했다. 코드·ROM 변경, HNS 빌드 및 게임 화면 검증은 하지 않았다.

## 2026-09-23 — `STRINGID_PKMNSITEMNORMALIZEDSTATUS` 출력 조건 확인

- 이 ID는 지닌 `리샘열매`(`HOLD_EFFECT_CURE_STATUS`)가 한 번에 둘 이상의 상태를 치료할 때 출력된다. 현재 일반 전투에서 가능한 대표 조건은 독·화상·마비·잠듦·얼음 같은 비휘발성 상태이상과 혼란이 동시에 걸린 경우다.
- `TryCureAnyStatus()`가 치료할 상태의 수를 세어 하나 이하면 `B_MSG_CURED_PROBLEM` → `STRINGID_PKMNSITEMCUREDPROBLEM`을, 둘 이상이면 `B_MSG_NORMALIZED_STATUS` → 이 ID를 선택한다. 따라서 리샘열매가 독만 치료하면 “독 상태가 나았다!”, 독과 혼란을 함께 치료하면 “상태이상이 나았다!”가 출력된다.
- 공통 `BattleScript_BerryCureStatusRet`는 아이템 팝업 → 열매 발동 애니메이션 → 선택된 문구 → 상태 아이콘 갱신 → 열매 소모 순서다. `{B_SCR_NAME_WITH_PREFIX}`는 열매를 발동한 포켓몬, `{B_LAST_ITEM}`은 리샘열매다.
- 검증: `src/data/items.h:11125-11135`, `src/battle_hold_effects.c:760-818,1169-1171`, `src/battle_message.c:472,1371-1380`, `data/battle_scripts_1.s:7134-7141`를 정적 확인했다. 코드·ROM 변경, HNS 빌드 및 게임 화면 검증은 하지 않았다.

## 2026-09-23 — 도주 특성 팝업 후 도망 메시지 출력

- `BattleScript_RanAwayUsingMonAbility`에 `copybyte gBattlerAbility, gBattlerAttacker`와 `call BattleScript_AbilityPopUp`을 추가했다. 따라서 플레이어 포켓몬이 도주 특성으로 도망에 성공하면, 도망한 포켓몬의 `도주` 특성 팝업이 먼저 표시되고 이어서 `STRINGID_PKMNFLEDUSING`이 출력된다.
- `gBattlerAbility`를 현재 도망한 공격자 슬롯으로 명시해 팝업의 주체가 이전 특성 처리의 배틀러에 의존하지 않도록 했다. 연막탄·고스트 타입·일반 도망의 별도 메시지 경로는 변경하지 않았다.
- 검증: `make BUILD=hns build/hns/data/battle_scripts_1.o -j1`과 `GITHUB_ACTION=1 timeout 600s make hns -j8`이 성공했다. 생성된 ROM의 링크 보고 사용량은 33,330,180바이트/32 MiB(99.33%)다. 실제 게임 화면에서 팝업 → 문구 순서는 mGBA 부재로 미확인이다.
- 다음 시작점: 도주 특성 포켓몬으로 야생전에서 도망을 선택해 특성 팝업 후 “무사히 도망쳤다” 문구가 나오는지 실제 HNS 화면에서 확인한다.

## 2026-09-23 — 도구 능력치 상승·특성 대상 변경·도주 메시지 출력 조건 보완

- `STRINGID_USINGITEMSTATOFPKMNROSE`는 지닌 도구의 효과로 실제 능력치 상승이 확정될 때 출력된다. 약점보험, 눈덩이, 빛이끼, 충전지, 흡수벌브, 실책보험, 목스프레이, 아드레날린오브, 능력치 상승 열매 및 필드 시드가 대표 사례다. 약점보험처럼 둘 이상의 능력치가 변하면 능력치별로 반복 출력될 수 있다. 일반 기술·특성의 능력치 변화나 연막탄/고스트 타입의 도망에는 이 ID를 쓰지 않는다.
- `STRINGID_PKMNSXTOOKATTACK`는 더블배틀에서 원래 다른 포켓몬을 향한 전기 타입 기술을 피뢰침이, 물 타입 기술을 마중물이 가로채 새 대상이 되었을 때 출력된다. 이 문자열을 출력하는 `BattleScript_TookAttack`은 특성 팝업을 호출하지 않는다. 단, HNS의 `GEN_LATEST` 설정에서는 이어지는 타입 흡수·특공 상승 처리 `BattleScript_MoveStatDrain`이 특성 팝업을 호출하므로, 실제 순서는 “공격을 받았다!” 문구 → 특성 팝업 → 특공 상승 문구다.
- `STRINGID_PKMNFLEDUSING`는 플레이어가 도망 가능한 전투에서 도주(`ABILITY_RUN_AWAY`) 특성 보유 포켓몬으로 도망을 선택해 성공했을 때 출력된다. 현재 본문은 `{PLAY_SE 0x0011}무사히 도망쳤다\\p`이며, 특성명을 문장에 넣지 않는다. 이후 상단 기록의 변경으로 이 문구 앞에는 도주 특성 팝업이 표시된다. 연막탄은 `STRINGID_PKMNFLEDUSINGITS`, 고스트 타입의 도주 및 일반 성공은 `STRINGID_GOTAWAYSAFELY`가 담당한다.
- 검증: `src/battle_message.c:497`, `src/battle_message.c:320`, `data/battle_scripts_1.s:4332-4345,6575-6579,6606-6614`, `src/battle_move_resolution.c:832-850,1719-1727`, `src/battle_util.c:544-605,2438-2479`, `src/battle_main.c:5781-5793`를 `rg`·`sed`로 정적 확인했다. 코드·ROM 변경 및 빌드는 하지 않았고 게임 화면도 미확인이다.

## 2026-09-23 — `STRINGID_USINGITEMSTATOFPKMNROSE` TODO와 연결 경로 확인

- 이 ID의 `//todo: update this, will require code changes`는 현재 문자열이 작동하지 않는다는 뜻이 아니다. 해당 주석은 상류의 “Gen 5+ 배틀 메시지” 갱신 커밋에서 추가된 것으로, 도구 이름을 문장에 직접 넣는 구형 표현을 현대식 도구 팝업과 일반 능력치 상승 문구로 바꾸려면 배틀 스크립트도 함께 고쳐야 한다는 미완료 메모다.
- 현재 ID는 `gStatUpStringIds[B_MSG_STAT_CHANGED_ITEM]`에 연결된다. 능력치 상승 열매·필드 시드·방어/특방 상승 열매 등의 공통 `BattleScript_ConsumableStatRaiseRet`, 약점보험, 눈덩이, 빛이끼, 충전지, 흡수벌브, 블런더정책, 목스프레이, 위협 후 아드레날린오브 등의 도구 발동 스크립트가 사용한다.
- `{B_SCR_NAME_WITH_PREFIX2}`는 스크립트 활성 배틀러, `{B_LAST_ITEM}`은 발동 도구, `{B_BUFF1}`은 능력치명, `{B_BUFF2}`는 상승 정도(예: `크게 `)다. 예를 들어 약점보험의 공격 2랭크 상승은 “상대 포켓몬은 약점보험으로\n공격이 크게 올라갔다!” 형태로 정상 확장된다.
- 따라서 현재 한글 문장 구현 자체에는 토큰 연결 오류가 없다. 현대식 표현으로 바꾸려면 각 도구 발동 경로에 도구 팝업을 넣고, 이 전용 ID 대신 일반 `gStatUpStringIds` 메시지로 출력하도록 대상 배틀러·선택값을 맞추는 별도 작업이 필요하다. 이번 확인에서는 소스·ROM을 수정하거나 빌드하지 않았다.

## 2026-09-23 — 공통 상태 회복 메시지를 상태별 문구로 분기

- `STRINGID_PKMNSTATUSNORMAL`을 출력하던 브레이브차지·리프레시, 정글힐·초승달의기도, 사이코시프트 경로와 `STRINGID_PKMNSXCUREDYPROBLEM`을 출력하던 촉촉한몸·탈피 경로를 공통 `gStatusCureStringIds` 테이블 출력으로 교체했다.
- 치료 직전 상태를 `GetCuredStatusMessage()`로 판별하여 독은 `STRINGID_PKMNPOISONCURED`, 화상은 `STRINGID_PKMNBURNCURED`, 마비는 `STRINGID_PKMNPARALYSISCURED`, 얼음은 `STRINGID_PKMNWASDEFROSTED`, 잠듦은 `STRINGID_PKMNWOKEUP`을 선택한다. 현재 HNS에서 비활성인 동상과 예외 기본값은 기존 `STRINGID_PKMNSTATUSNORMAL`로 유지했다.
- 사용자가 `src/battle_message.c`에 추가해 둔 독·화상·마비 회복 문자열에 대응하는 `StringID` 열거값이 없어서 `include/constants/battle_string_ids.h`에 세 ID를 추가했다. 해동 문구의 `{B_SCR_NAME_WITH_PREFIX}`가 정확한 치료 대상을 가리키도록 상태를 지우기 전에 `gBattleScripting.battler`도 치료 대상에 맞춘다.
- 촉촉한몸·탈피의 `BattleScript_ShedSkinActivates`는 기존 `call BattleScript_AbilityPopUp`을 그대로 먼저 실행하고 그 다음 상태별 테이블 문구를 출력하므로 특성 팝업 순서는 유지된다.
- 관련 오브젝트 빌드와 `GITHUB_ACTION=1 timeout 600s make hns -j8` 전체 빌드가 성공했다. 생성된 `pokehns.gba`는 33,554,432바이트이며 ROM 사용량은 33,330,196바이트(99.33%)다. 실제 게임 화면 검증은 하지 않았다.

## 2026-09-23 — `STRINGID_PKMNWASDEFROSTEDBY`의 상대·아군 기술 반영 범위 확인

- `{B_SCR_NAME_WITH_PREFIX}`는 기술 사용자의 진영이 아니라 `gBattleScripting.battler`에 저장된 배틀러를 표시한다. 현재 해동 처리에서는 해동된 포켓몬이 이 값에 저장되므로, 자신의 기술로 해동하는 경우와 상대 포켓몬이 자신의 기술로 해동하는 경우 모두 해동된 포켓몬 이름이 반영된다.
- `{B_CURRENT_MOVE}`는 메시지 전송 시점의 `gCurrentMove`를 저장하므로, 양쪽 어느 포켓몬이 기술을 사용했는지와 관계없이 해당 해동 기술명이 반영된다.
- 다만 `STRINGID_PKMNWASDEFROSTEDBY`는 사용자가 해동 기술로 자기 자신을 해동할 때만 `B_MSG_DEFROSTED_BY_MOVE`로 선택된다. 상대의 불꽃 기술이 내 포켓몬을 해동하는 등 대상 해동은 현재 `B_MSG_DEFROSTED` → `STRINGID_PKMNWASDEFROSTED`를 사용하므로 `...BY` 문구는 출력되지 않는다.
- 이번 확인에서는 소스·데이터·ROM을 수정하지 않았고 HNS 빌드 및 실제 게임 화면 검증도 하지 않았다.

## 2026-09-23 — 해동 메시지의 `DEF`→`SCR` 교체 가능성 확인

- `STRINGID_PKMNWASDEFROSTED`의 `{B_DEF_NAME_WITH_PREFIX}`를 `{B_SCR_NAME_WITH_PREFIX}`로 바꾸면 현재 확인된 해동 경로의 이름 불일치가 해결된다. `B_DEF`는 `gBattlerTarget`을, `B_SCR`는 `gBattleScripting.battler`를 참조한다(`src/battle_message.c:3299-3315`).
- `BattleScript_BattlerDefrosted`를 호출하는 자연 해동·기술 해동·불꽃 기술에 의한 대상 해동·상태 제거 경로 모두 해동된 배틀러를 `gBattleScripting.battler`에 먼저 기록한다(`src/battle_move_resolution.c:101-105`, `src/battle_util2.c:190-193`, `src/battle_script_commands.c:3561`). 따라서 해당 문자열만 `B_SCR`로 바꾸면 해동된 포켓몬의 이름이 표시된다.
- `STRINGID_PKMNWASDEFROSTEDBY`는 별도의 공격자/기술 원인 문구이므로 변경 대상이 아니다. 이번 확인에서는 소스·데이터·ROM을 수정하지 않았고 HNS 빌드 및 실제 게임 화면 검증도 하지 않았다.

## 2026-09-23 — 공격자 해동 시 `STRINGID_PKMNWASDEFROSTED` 토큰 불일치 확인

- `STRINGID_PKMNWASDEFROSTED`의 `{B_DEF_NAME_WITH_PREFIX}`는 실제 치환부에서 `gBattlerTarget`을 사용한다(`src/battle_message.c:3299-3301`). 따라서 일반적인 상대 대상 기술을 시도하던 공격자가 행동 시작 시 자연 해동되는 `CancelerAsleepOrFrozen` 경로에서도 `gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_DEFROSTED`로 이 ID가 선택되지만, 문장 이름은 공격자가 아니라 현재 대상 포켓몬으로 확장될 수 있다.
- 반대로 공격자가 자신의 해동 기술로 해동되면 `B_MSG_DEFROSTED_BY_MOVE`를 선택해 `STRINGID_PKMNWASDEFROSTEDBY`가 출력되고, `{B_ATK_NAME_WITH_PREFIX}`와 `{B_CURRENT_MOVE}`로 공격자와 기술명이 표시된다.
- 기술 종료 시 불꽃 기술 등으로 대상이 해동되는 경로는 대상 포켓몬을 `gBattlerTarget`으로 표시하는 `STRINGID_PKMNWASDEFROSTED`와 의도상 일치한다. 즉 현재 코드에서 공격자 자연 해동만 이름 토큰이 어긋날 가능성이 있다.
- 이번 확인에서는 소스·데이터·ROM을 수정하지 않았고 HNS 빌드 및 실제 게임 화면 검증도 하지 않았다. 수정하려면 공격자 자연 해동의 메시지 토큰 또는 출력용 배틀러 설정을 별도 결정해야 한다.

## 2026-09-23 — `STRINGID_PKMNHEALEDPARALYSIS` 출력 조건 확인

- 이 ID는 `data/battle_scripts_1.s:5898-5902`의 `BattleScript_TargetPRLZHeal`에서 직접 출력된다.
- 현재 기술 데이터에서는 마비 상태인 대상에게 `정신차리기`가 명중해 추가 효과로 마비를 제거할 때 사용된다. `MOVE_EFFECT_REMOVE_STATUS`가 실제 마비를 제거한 뒤 대상의 상태 아이콘을 갱신한다.
- `{B_DEF_NAME_WITH_PREFIX}`는 마비가 치료된 대상 포켓몬이다. 일반 기술 공통 회복인 `STRINGID_PKMNSTATUSNORMAL`이나 열매 전용 `STRINGID_PKMNSITEMCUREDPARALYSIS`와는 다른 경로이며, 특성 팝업은 없다.
- 이번 확인에서는 소스·게임 데이터·ROM을 수정하지 않았고 HNS 빌드와 실제 게임 화면 검증도 하지 않았다.

## 2026-09-23 — 해동·기상 메시지 출력 조건 확인

- `STRINGID_PKMNWASDEFROSTED`는 `gGotDefrostedStringIds[B_MSG_DEFROSTED]`에 연결되어 있다. 얼음 상태인 사용 포켓몬이 행동 시작 시 20% 판정에 성공해 자연 해동되거나, 현재 코드의 턴 종료 기술 효과 처리에서 대상이 불꽃 기술·해동 효과로 해동될 때 출력될 수 있다.
- 사용 포켓몬이 자신의 기술로 해동되는 `CancelerThaw` 경로는 별도 `B_MSG_DEFROSTED_BY_MOVE`를 선택하므로 `STRINGID_PKMNWASDEFROSTEDBY`를 사용한다. 아이템 해동은 `STRINGID_PKMNSITEMDEFROSTEDIT`를 사용한다.
- `STRINGID_PKMNWOKEUP`은 `gWokeUpStringIds[B_MSG_WOKE_UP]`에 연결되어 있다. 수면 턴이 줄어들어 해당 포켓몬이 정상적으로 깨어날 때 `BattleScript_MoveUsedWokeUp`이 출력한다. 소란피기로 깨면 `STRINGID_PKMNWOKEUPINUPROAR`, 아이템으로 깨면 `STRINGID_PKMNSITEMWOKEIT`, 기술로 상대의 수면을 제거하면 `STRINGID_TARGETWOKEUP`이 사용된다.
- 두 ID 모두 현재 경로에서 특성 팝업을 호출하지 않는다. 이번 확인에서는 소스·게임 데이터·ROM을 수정하지 않았고 HNS 빌드와 실제 게임 화면 검증도 하지 않았다.

## 2026-09-23 — `STRINGID_PKMNBURNHEALED` 미사용 판정 정정

- 이전 확인에서 `STRINGID_PKMNBURNHEALED`를 미사용으로 잘못 판단했다. 실제로 `data/battle_scripts_1.s:5910-5914`의 `BattleScript_TargetBurnHeal`이 이 ID를 출력한다.
- `src/battle_script_commands.c:3555-3590`의 `MOVE_EFFECT_REMOVE_STATUS` 처리에서 대상의 화상 상태를 제거하면 이 스크립트로 이동한다. 현재 해당 추가 효과를 가진 기술은 `물거품아리아`이므로, 화상 상태인 대상에게 명중하면 영문 화상 회복 문구가 출력된다.
- 같은 상태 제거 경로에서 `정신차리기`의 마비 치료는 `STRINGID_PKMNHEALEDPARALYSIS`를 사용한다. 독 치료 분기는 `STRINGID_PASTELVEILENTERS`를 사용하지만 현재 저장소의 해당 문자열도 영문이며, 현재 기술 데이터에는 독 제거 추가 효과가 없다.
- 이번 확인에서는 소스·게임 데이터·ROM을 수정하지 않았고 HNS 빌드와 실제 게임 화면 검증도 하지 않았다.

## 2026-09-23 — 독·화상·마비 회복 메시지 ID 확인

- 일반 기술의 상태 회복에는 상태별 전용 ID가 모두 있는 것이 아니다. `리프레시`, `브레이브차지`, `정글힐`·`초승달의기도`, `사이코시프트`는 독·화상·마비를 포함해 상태를 치료할 때 공통으로 `STRINGID_PKMNSTATUSNORMAL`을 출력한다.
- 독·화상·마비의 전용 문구는 열매·도구 발동 경로에 있다. 각각 `STRINGID_PKMNSITEMCUREDPOISON`(`독이 해독됐다!`), `STRINGID_PKMNSITEMHEALEDBURN`(`화상이 나았다!`), `STRINGID_PKMNSITEMCUREDPARALYSIS`(`마비가 풀렸다!`)이며, `BattleScript_BerryCureStatusRet`가 도구 팝업 뒤 테이블에서 선택한다.
- 잠듦과 얼음은 일반 회복 경로가 별도로 있다. 잠듦은 `STRINGID_PKMNWOKEUP`, 얼음은 `STRINGID_PKMNWASDEFROSTED`가 사용된다. 마비는 `정신차리기`의 `STRINGID_PKMNHEALEDPARALYSIS`, 화상은 `물거품아리아`의 `STRINGID_PKMNBURNHEALED`가 별도 사용된다.
- 이번 확인에서는 소스·게임 데이터·ROM을 수정하지 않았고 HNS 빌드와 실제 게임 화면 검증도 하지 않았다.

## 2026-09-23 — `STRINGID_PKMNSTATUSNORMAL` 출력 조건 확인

- 이 ID는 상태이상이 실제로 치료된 뒤 출력된다. 현재 직접 호출부는 `브레이브차지`·`리프레시`, `정글힐`·`초승달의기도`, `사이코시프트`의 상태 회복 경로다.
- `브레이브차지`와 `리프레시`는 사용 포켓몬의 상태를 치료한 뒤, `정글힐`·`초승달의기도`는 각 회복 대상의 상태를 치료한 뒤 출력한다. `사이코시프트`는 상태를 상대에게 옮긴 다음 사용 포켓몬의 상태를 치료할 때 출력한다.
- `{B_ATK_NAME_WITH_PREFIX}`는 이 스크립트 시점의 공격자 슬롯이며, 위 경로에서는 상태가 치료된 포켓몬을 가리킨다. 치료할 상태가 없으면 `curestatuswithmove` 또는 기술 실패 분기로 이동하므로 이 ID는 출력되지 않는다. 특성 팝업은 없는 일반 기술 효과 메시지다.
- 이번 확인에서는 소스·게임 데이터·ROM을 수정하지 않았고 HNS 빌드와 실제 게임 화면 검증도 하지 않았다.

## 2026-09-23 — `STRINGID_PKMNSXMADEYINEFFECTIVE`·`STRINGID_PKMNSXCUREDYPROBLEM` 출력 조건 확인

- `STRINGID_PKMNSXMADEYINEFFECTIVE`는 능력 때문에 기술 또는 도구 조작 효과가 무효가 될 때 사용된다. `BattleScript_StickyHoldActivatesRet`·`BattleScript_NoItemSteal`에서 점착이 탁쳐서떨구기·도구 훔치기·도구 교체·부식가스 등의 도구 조작을 막을 때, 그리고 타오르는불꽃이 이미 발동한 상태에서 불꽃 타입 기술을 다시 흡수할 때 `gFlashFireStringIds`를 통해 이 ID를 선택한다. 특성 팝업이 먼저 출력된다.
- 현재 문자열은 `{B_DEF_NAME_WITH_PREFIX}`에 무효화한 포켓몬, `{B_DEF_ABILITY}`에 점착 또는 타오르는불꽃, `{B_CURRENT_MOVE}`에 막힌 기술을 넣는다. 따라서 예를 들어 `상대 포켓몬의 점착 때문에\n탁쳐서떨구기는 효과가 없다!` 형태다. 첫 불꽃 타입 흡수 성공 시에는 이 ID가 아니라 `STRINGID_PKMNRAISEDFIREPOWERWITH`가 사용된다.
- `STRINGID_PKMNSXCUREDYPROBLEM`은 `BattleScript_ShedSkinActivates`에서만 직접 선택된다. 턴 종료 시 비가 내리고 상태이상이 있는 촉촉한몸, 또는 상태이상이 있고 발동 확률 판정에 성공한 탈피가 상태를 치료할 때 특성 팝업 후 출력된다. `{B_SCR_NAME_WITH_PREFIX}`·`{B_SCR_ABILITY}`는 치료한 포켓몬과 특성, `{B_BUFF1}`은 독·잠·마비·화상·얼음 등 치료된 상태명이다.
- 면역 특성의 독·마비·수면·화상·얼음·혼란·헤롱헤롱·도발 해제는 이름이 비슷한 `STRINGID_PKMNSXCUREDITSYPROBLEM`을 사용하므로 현재 요청의 두 번째 ID와 구분해야 한다. `//not in gen 5+` 주석은 실행을 막는 조건이 아니며, 위 경로는 HNS에서도 코드상 선택될 수 있다.
- 이번 확인에서는 소스·게임 데이터·ROM을 수정하지 않았고 HNS 빌드와 실제 게임 화면 검증도 하지 않았다.

## 2026-09-23 — `STRINGID_PKMNSXPREVENTSYLOSS` 출력 조건 확인

- 이 ID는 `src/battle_script_commands.c:7916-7931`의 특정 능력치 하락 방지 분기에서 `BattleScript_AbilityNoSpecificStatLoss`를 통해 출력된다. 대상의 특정 능력치를 낮추려는 시도가 실제 하락으로 이어지지 않을 때 사용된다.
- 현재 대상 특성은 `날카로운눈`·`심안`·`발광`(현재 설정에서 명중률 하락 방지), `괴력집게`(공격 하락 방지), `부풀린가슴`(방어 하락 방지)이다. 명중률 방지의 `발광`은 `B_ILLUMINATE_EFFECT >= GEN_9`일 때만 이 분기에 포함된다.
- `PREPARE_STAT_BUFFER(gBattleTextBuff1, statId)`가 `{B_BUFF1}`에 하락이 막힌 능력치명을 넣는다. `{B_SCR_NAME_WITH_PREFIX}`와 `{B_SCR_ABILITY}`는 그 방지 특성의 포켓몬과 특성명이다. 예를 들어 괴력집게가 공격 하락을 막으면 `상대 포켓몬은\n괴력집게 때문에 공격이 떨어지지 않는다!` 형태다.
- 특성 팝업 뒤 문장이 출력된다. `클리어바디`·`메탈프로텍트`·`하얀연기`처럼 모든 능력치 하락을 막는 특성은 `STRINGID_PKMNPREVENTSSTATLOSSWITH`를 사용하며 이 ID가 아니다. 아이템 `클리어참`, 플라워베일·미러아머 등도 별도 문구/경로다.
- 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다.

## 2026-09-23 — `STRINGID_PKMNSXRESTOREDHPALITTLE2` 출력 조건 확인

- 이 ID는 `data/battle_scripts_1.s:6195-6201`의 공통 `BattleScript_AbilityHpHeal`에서 직접 출력된다. 이 스크립트는 비가 올 때의 `ABILITY_RAIN_DISH`(1/16 회복)·`ABILITY_DRY_SKIN`(1/8 회복)과, 열매를 먹은 뒤 `ABILITY_CHEEK_POUCH`(1/3 회복)의 성공 경로에서 사용된다.
- 비가 오는 중이고 HP가 가득 차 있지 않으며 회복 봉인이 없을 때 Rain Dish·Dry Skin 경로가 실행된다. Cheek Pouch는 열매를 실제로 먹고 HP가 가득 차 있지 않으며 회복 봉인이 없을 때 실행된다.
- 특성 팝업 뒤 이 문장이 먼저 출력되고, 이후 HP 바/HP 데이터가 갱신된다. `gBattlerAttacker`가 특성 보유자로 설정되므로 `{B_ATK_NAME_WITH_PREFIX}`와 `{B_ATK_ABILITY}`는 회복한 포켓몬과 그 특성을 표시한다. 예를 들어 `피카츄는\n저수로 인해 조금 회복했다.` 형태다.
- 얼음몸(`ABILITY_ICE_BODY`)은 별도의 `BattleScript_IceBodyHeal`을 사용하며 이 ID를 출력하지 않는다. 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다.

## 2026-09-23 — 연애 방지 문구와 특성 유발 헤롱헤롱 문구 비교

- `STRINGID_PKMNPREVENTSROMANCEWITH`는 매력/헤롱헤롱 시도가 특성으로 차단될 때 사용된다. `BattleScript_ObliviousPreventsAttraction`이 특성 팝업 뒤 이 ID를 출력하며, 현재 문장은 `{B_DEF_NAME_WITH_PREFIX}에게는 효과가 없는 것 같다...`이다. `{B_DEF_NAME_WITH_PREFIX}`는 효과를 막은 대상 포켓몬이다.
- `STRINGID_PKMNSXINFATUATEDY`는 특성 발동으로 실제 헤롱헤롱 상태가 부여될 때 사용된다. `BattleScript_CuteCharmActivates`가 특성 팝업과 애니메이션 뒤 이 ID를 출력하며, `{B_DEF_NAME_WITH_PREFIX}`/`{B_DEF_ABILITY}`는 원인이 된 특성 보유자, `{B_ATK_NAME_WITH_PREFIX}`는 헤롱헤롱 상태가 된 공격자다.
- 따라서 전자는 “특성 때문에 연애 효과가 막힘”, 후자는 “특성 때문에 공격자가 헤롱헤롱해짐”이라는 차이다. 둘 다 특성 팝업이 먼저 나오는 별도 경로다.
- 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다.

## 2026-09-23 — `STRINGID_PKMNSXINFATUATEDY`가 다시 보이는 이유 확인

- 현재 `src/battle_message.c:1251-1255`의 `gAttractUsedStringIds`가 `B_MSG_STATUSED_BY_ABILITY`를 `STRINGID_PKMNSXINFATUATEDY`에 매핑하고 있다. 따라서 특성으로 헤롱헤롱 상태가 발생하면 이 ID가 선택된다.
- `data/battle_scripts_1.s:6993-6999`의 `BattleScript_CuteCharmActivates`도 특성 팝업 후 `STRINGID_PKMNSXINFATUATEDY`를 직접 출력한다. 현재 문자열의 `{B_DEF_ABILITY}`가 들어간 문장은 이 경로 때문에 정상적으로 다시 사용된다.
- 이전에 바꾼 것으로 기억한 수면·독·화상·마비 특성 메시지 변경에는 이 매력/헤롱헤롱 매핑이 포함되어 있지 않다. 현재 `git diff`에서는 이 줄이 미커밋 한글화 변경으로 표시되고, 이번 세션에서 소스 파일을 되돌린 기록은 없다. 즉 자동 복구가 아니라 현재 작업 트리에 특성 전용 ID와 매핑이 그대로 남아 있는 상태다.
- 특성으로 인한 헤롱헤롱도 일반 문구를 쓰게 하려면 다음 변경이 필요하다: `gAttractUsedStringIds[B_MSG_STATUSED_BY_ABILITY]`를 `STRINGID_PKMNFELLINLOVE`로 바꾸고, Cute Charm 특성 팝업 후 일반 문구가 출력되는지 HNS 빌드로 검증한다. 이번 요청에서는 원인 확인만 했으므로 소스·데이터·ROM은 수정하지 않았다.

## 2026-09-23 — HNS의 유령 배틀 도달 가능성 재확인

- 결론적으로 유령 배틀 콘텐츠는 현재 HNS에 포함되지 않고 FRLG 전용이다. `data/maps/PokemonTower_*_Frlg/map.json`의 `game_version`이 `frlg`이며, HNS용으로 생성된 `data/maps/groups.inc:1220-...`에서 FRLG 던전 그룹의 항목들이 `NULL`로 제외되어 포켓몬타워 맵이 ROM에 연결되지 않는다.
- `src/battle_setup.c:365-391`의 `CheckSilphScopeInPokemonTower`와 `DoGhostBattle`은 공통 소스에 남아 있고 `#if IS_HNS`로 제외되지 않았지만, HNS에서는 해당 맵에 정상적으로 진입할 수 없으므로 일반 야생 배틀 경로에서 호출되지 않는다. `src/data/wild_encounters.h`의 포켓몬타워 야생 데이터도 `#ifdef FIRERED`/`#ifdef LEAFGREEN` 블록에만 있다.
- `STRINGID_ITDODGEDBALL`은 유령 전투의 `BATTLE_TYPE_GHOST`에서만 출력되므로, 현재 HNS 정상 플레이에서는 출력되지 않는다. `StartMarowakBattle`과 포켓몬타워의 마루와크 이벤트도 FRLG 맵 데이터에만 있다. 단, 커스텀 이벤트로 `BATTLE_TYPE_GHOST`를 직접 설정하거나 FRLG 맵을 HNS에 다시 연결하면 해당 메시지 경로를 사용할 수 있다.
- 이전 기록의 “HNS의 `DoGhostBattle` 전투에서 해당된다”는 표현은 공통 소스 경로와 실제 HNS 도달 가능성을 구분하지 않아 부정확했다. 정확한 표현은 “공통 코드에는 남아 있으나 현재 HNS 콘텐츠에서는 도달 불가”이다.
- 이번 확인은 소스·생성 데이터의 정적 확인만 수행했으며, 소스·게임 데이터와 ROM은 수정하지 않았다. HNS ROM 빌드 및 실제 게임 화면 검증은 하지 않았다.

## 2026-09-23 — `STRINGID_ITDODGEDBALL` 출력 조건 확인

- 이 ID는 `data/battle_scripts_2.s:325-329`의 `BattleScript_GhostBallDodge`에서만 직접 출력된다. `src/battle_script_commands.c:11049-11056`의 `Cmd_handleballthrow`가 `BATTLE_TYPE_GHOST` 전투에서 몬스터볼을 던지면 포획 판정 전에 이 스크립트로 분기한다.
- 현재 문장은 `피했다!\n이 녀석은 잡힐 것 같지 않군!`이며, 일반적인 포획 실패처럼 흔들림·탈출 판정을 거친 결과가 아니다. 일반 포획 실패는 `BattleScript_ShakeBallThrow`와 `gBallEscapeStringIds`의 다른 문구를 사용한다.
- 유령 전투의 볼 회피 애니메이션 후 문장이 출력되며, 이 경로에는 특성·도구 팝업이 없다. 현재 HNS 맵·이벤트에는 해당 유령 전투가 연결되어 있지 않아 정상 플레이에서는 출력되지 않는다. 공통 코드에 `DoGhostBattle` 경로가 남아 있을 뿐이다.
- 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다.

## 2026-09-23 — `STRINGID_NOPPLEFT` 출력 조건 확인

- 일반 배틀에서 플레이어가 현재 PP가 0인 기술을 선택하면 `src/battle_util.c:1635-1645`가 `BattleScript_SelectingMoveWithNoPP`를 선택 스크립트로 지정하고, `data/battle_scripts_1.s:5142-5144`가 `STRINGID_NOPPLEFT`를 `printselectionstring`으로 출력한다. 현재 문장은 `남은 PP가 없다!`이며 `\p` 뒤 다시 기술 선택 화면으로 돌아간다.
- 실제 행동 단계까지 진행된 뒤 PP가 0인 기술이 취소되는 경우에는 `src/battle_move_resolution.c:236-246`의 `BattleScript_NoPPForMove`가 `STRINGID_BUTNOPPLEFT`를 출력하므로 이 ID와 다르다.
- 배틀 팰리스에서는 플레이어가 기술을 직접 선택하지 않으므로 이 선택 스크립트를 지정하지 않고 `palaceUnableToUseMove`만 설정한다. 특성 팝업은 없다.
- 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다.

## 2026-09-23 — `STRINGID_DOWNPOURSTARTED` 사용 여부 확인

- 현재 HNS에서 `STRINGID_DOWNPOURSTARTED`를 직접 출력하는 배틀 스크립트는 없다. `src/battle_message.c:1030`의 `gMoveWeatherChangeStringIds`에서 `B_MSG_STARTED_DOWNPOUR`에 매핑되어 있지만 `// Unused`로 표시되어 있고, 저장소 내에서 이 선택자를 설정하는 코드는 없다.
- 일반 비 날씨 시작은 `B_MSG_STARTED_RAIN` → `STRINGID_STARTEDTORAIN`을 사용한다. 원시의바다(`ABILITY_PRIMORDIAL_SEA`) 발동도 `B_MSG_STARTED_PRIMORDIAL_SEA` → `STRINGID_HEAVYRAIN`을 사용하므로 이 문구가 아니다. `BATTLE_WEATHER_RAIN_DOWNPOUR`도 현재 `// unused`다.
- 따라서 현재 게임에서는 `폭우로 변했다!`가 출력되지 않으며, 별도 커스텀 스크립트가 `B_MSG_STARTED_DOWNPOUR`를 선택할 때만 출력될 수 있다. 원본 주석상 이 ID는 GSC의 비바라기/폭우 메시지 계열을 보존한 것이다.
- 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다.

## 2026-09-23 — `STRINGID_ENDUREDSTURDY`의 옹골참 팝업 순서 확인

- 일반 기술의 피해로 HP가 0이 될 상황에서, 현재 HP가 최대이고 옹골참 조건이 활성화되어 `MOVE_RESULT_STURDIED`가 설정되면 `BattleScript_SturdiedMsg`가 이 ID를 출력한다. 현재 문장은 `{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n공격을 버텼다!`다.
- `data/battle_scripts_1.s:5415-5420`의 순서는 `BattleScript_AbilityPopUpTarget` → `STRINGID_ENDUREDSTURDY`이므로 대상 포켓몬의 옹골참 특성 팝업이 먼저 나온다.
- 이 문구는 일격필살기를 막는 `STRINGID_PKMNPROTECTEDBY` 분기와 다르며, 일반 공격을 HP 1로 버틴 경우에 사용된다. 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다.

## 2026-09-23 — `STRINGID_PKMNPROTECTEDBY` 출력 조건과 특성 팝업 확인

- 현재 저장소에서 이 ID를 직접 출력하는 곳은 `data/battle_scripts_1.s:6581-6586`의 `BattleScript_SturdyPreventsOHKO`뿐이다. 대상 포켓몬의 옹골참(`ABILITY_STURDY`) 때문에 일격필살기 판정이 무효가 되었을 때 사용된다.
- `src/battle_script_commands.c:1207-1227`에서 `MOVE_RESULT_ONE_HIT_KO_STURDY` 플래그가 있으면 이 스크립트로 분기한다. 일반 기술의 옹골참 생존이나 소리·탄환·황금몸 특성의 기술 무효화에는 이 ID가 아니라 별도의 문구가 사용된다.
- 스크립트 순서는 `BattleScript_AbilityPopUp` → `STRINGID_PKMNPROTECTEDBY`다. 따라서 옹골참 특성 팝업이 먼저 나온 뒤, 예를 들어 `상대 포켓몬의 옹골참 때문에\n효과가 없었다!`가 출력된다.
- 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다.

## 2026-09-23 — `STRINGID_PKMNRESTOREDHPUSING` 출력 조건과 특성 팝업 확인

- 이 ID는 `data/battle_scripts_1.s:6597-6604`의 `BattleScript_MoveHPDrain`에서만 직접 출력된다. 전기 기술을 흡수하는 축전(`ABILITY_VOLT_ABSORB`), 물 기술을 흡수하는 저수(`ABILITY_WATER_ABSORB`)·건조피부(`ABILITY_DRY_SKIN`), 땅 기술을 흡수하는 흙먹기(`ABILITY_EARTH_EATER`)가 HP를 실제로 회복할 때 사용된다.
- `src/battle_util.c:2438-2512`에서 대상이 최대 HP가 아니고 회복 봉인이 없을 때 이 스크립트를 예약하며, 현재 회복량은 최대 HP의 1/4이다. 최대 HP이거나 회복 봉인 상태면 이 문구가 아니라 실패 문구가 사용된다.
- 스크립트 순서는 `BattleScript_AbilityPopUp` → HP 바/데이터 갱신 → `STRINGID_PKMNRESTOREDHPUSING` 출력이다. 따라서 특성 팝업이 먼저 나온다. 예를 들어 축전 포켓몬이면 `포켓몬은 축전으로 인해\n회복했다!`처럼 출력된다.
- 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다.

## 2026-09-23 — `STRINGID_PKMNTRACED`의 `{B_BUFF1}` 의미 확인

- `src/battle_util.c:3097-3132`에서 Trace가 선택한 상대 포켓몬의 닉네임을 `PREPARE_MON_NICK_WITH_PREFIX_LOWER_BUFFER`로 `gBattleTextBuff1`에 넣는다. 따라서 `{B_BUFF1}`는 능력명이 아니라 그 포켓몬의 닉네임이며, 트레이너 배틀에서는 `상대 `, 야생 배틀에서는 `야생 ` 접두사가 붙고 플레이어 편이면 접두사가 없다.
- `gBattleTextBuff2`에는 선택한 능력명을 `PREPARE_ABILITY_BUFFER`로 넣으므로 `{B_BUFF2}`가 `위협`, `부유` 같은 능력명이다. `{B_SCR_NAME_WITH_PREFIX}`는 Trace를 가진 포켓몬이다.
- `BattleScript_TraceActivates`는 능력 팝업 후 이 문자열을 출력한다. 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다.

## 2026-09-23 — 울퉁불퉁멧의 `STRINGID_PKMNHURTSWITH` 실제 치환 확인

- 울퉁불퉁멧은 `TryRockyHelmet`에서 접촉 기술이 명중하고 공격자가 살아 있으며 접촉 회피 효과가 없을 때 발동한다. 반동 피해량은 현재 코드에서 공격자 최대 HP의 1/6이다.
- `BattleScript_RockyHelmetActivates`는 먼저 `BattleScript_ItemPopUp_ScriptingNoFlush`로 울퉁불퉁멧 아이템 팝업을 출력한 뒤 `BattleScript_HurtAttacker`를 호출해 `STRINGID_PKMNHURTSWITH`를 출력한다. 별도의 특성 팝업은 없다.
- 현재 `{B_DEF_ABILITY}`는 `gBattlerTarget`의 실제 특성명을 읽는 토큰이다. 따라서 현재 문자열은 `상대 포켓몬의 [그 포켓몬의 특성] 때문에\n공격 포켓몬은 상처를 입었다!`처럼 출력되며, `울퉁불퉁멧 때문에`라고 출력되지 않는다. 아이템명을 표시하려면 이 공통 ID와 별도의 아이템 토큰/문자열을 검토해야 한다.
- 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다.

## 2026-09-23 — `STRINGID_PKMNHURTSWITH` 출력 경로와 팝업 순서 확인

- 이 ID의 공통 피해 출력부는 `data/battle_scripts_1.s:6943-6949`의 `BattleScript_HurtAttacker`다. 공격자가 접촉 효과 등으로 반동 피해를 받은 뒤 HP가 갱신되고 이 문구가 출력되며, 이후 기절 여부를 확인한다.
- `src/battle_util.c:4047-4065`의 거친피부(`ABILITY_ROUGH_SKIN`)·철가시(`ABILITY_IRON_BARBS`)가 접촉 기술로 실제 피해를 주면 `BattleScript_RoughSkinActivates`를 호출한다. 이 스크립트는 특성 팝업 후 `BattleScript_HurtAttacker`를 실행하므로 특성 팝업이 먼저 나온다. 현재 `GEN_LATEST`의 반동 피해량은 최대 HP의 1/8이다.
- 같은 ID는 `BattleScript_RockyHelmetActivates`, `BattleScript_JabocaRowapBerryActivates`에서도 재사용된다. 따라서 울퉁불퉁멧·자보열매·애터열매의 반동 피해, 그리고 접촉 기술을 가시방패로 막아 공격자가 피해를 받을 때도 출력된다. 울퉁불퉁멧은 아이템 팝업이 먼저 나오며, 자보·애터열매는 `리펜` 특성이 있을 때만 리펜 팝업이 먼저 나온다. 가시방패 분기에는 특성 팝업이 없다.
- 현재 문자열은 `{B_DEF_ABILITY}`를 원인으로 표시하므로 거친피부·철가시 경로에는 맞지만, 아이템·가시방패 경로에도 같은 ID가 재사용되어 원인명 표시가 문맥과 맞지 않을 수 있다. 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다.

## 2026-09-23 — `STRINGID_PKMNCUTSATTACKWITH` 출력 조건과 특성 팝업 확인

- 현재 직접 출력부는 `data/battle_scripts_1.s:6294-6301`의 `BattleScript_IntimidateEffect`이며, 교체 출전한 `ABILITY_INTIMIDATE`(위협)가 상대 포켓몬의 공격을 실제로 1단계 낮췄을 때 사용된다.
- `BattleScript_IntimidateActivates`가 먼저 위협 특성 팝업을 출력한 뒤 각 상대 포켓몬을 순회한다. 공격이 이미 최저 단계이거나 위협을 막는 특성·상태라서 하락에 실패하면 이 ID 대신 실패·방어용 문구가 출력된다.
- 현재 문자열은 `{B_SCR_NAME_WITH_PREFIX}의 {B_SCR_ABILITY} 때문에\n{B_DEF_NAME_WITH_PREFIX}의 공격력이 떨어졌다!`다. `{B_SCR_NAME_WITH_PREFIX}`와 `{B_SCR_ABILITY}`는 위협을 발동한 포켓몬과 특성명, `{B_DEF_NAME_WITH_PREFIX}`는 공격이 내려간 대상이다.
- 따라서 특성 팝업이 먼저 나온 뒤, 대상별로 `...의 위협 때문에\n...의 공격력이 떨어졌다!`가 출력된다. 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다.

## 2026-09-23 — `STRINGID_PKMNANCHORSITSELFWITH` 출력 조건과 특성 팝업 확인

- 이 ID는 `data/battle_scripts_1.s:6634-6639`의 `BattleScript_AbilityPreventsPhasingOutRet`에서 출력된다. 대상 포켓몬의 `ABILITY_SUCTION_CUPS`(흡반) 때문에 교체·날려버리기가 막히는 경우다.
- `날려버리기`·`울부짖기`가 흡반 포켓몬을 강제로 교체시키려 할 때와, 피해를 준 `배대뒤치기`·`드래곤테일`의 교체 효과가 흡반 때문에 막힐 때 사용된다. 이 경로에서는 `AbilityPopUpTarget`이 먼저 호출되므로 흡반 특성 팝업 후 문구가 나온다.
- 레드카드 발동 중 흡반 때문에 강제 교체가 막히는 `BattleScript_RedCardSuctionCups`에서도 같은 ID가 출력된다. 다만 이 분기에는 능력 팝업 호출이 없고, 레드카드 아이템 팝업·발동 문구 뒤에 바로 이 문구가 출력된다.
- `{B_DEF_NAME_WITH_PREFIX}`와 `{B_DEF_ABILITY}`는 교체를 막은 흡반 포켓몬을 가리킨다. 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다.

## 2026-09-23 — `STRINGID_PKMNPREVENTSUSAGE` 현재 문장 재확인

- 현재 문자열은 `{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_CURRENT_MOVE}{B_TXT_EULREUL} 쓸 수 없다!`이므로, 문장 자체에는 특성명이나 `때문에`가 들어가지 않는다. 예를 들어 `피카츄는\n자폭을 쓸 수 없다!` 형태다.
- 출력 조건은 이전 확인과 동일하다. `ABILITY_DAMP`(습기)가 필드에 있고 사용 기술의 `dampBanned`가 TRUE이면 `src/battle_move_resolution.c:1459-1466`의 `CancelerExplodingDamp`가 기술을 취소하고 `BattleScript_DampStopsExplosion`을 실행한다.
- 대상 기술은 자폭, 대폭발, 깜짝헤드, 미스트버스트다. 스크립트는 `BattleScript_AbilityPopUpScripting`으로 습기 특성 팝업을 먼저 표시한 뒤 이 문자열을 출력하므로, 특성명은 팝업에서 확인된다.
- `{B_ATK_NAME_WITH_PREFIX}`는 사용 포켓몬, `{B_CURRENT_MOVE}`는 습기로 막힌 기술을 가리킨다. 기술은 실제로 실행되지 않는다. 이번 재확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다.

## 2026-09-23 — `STRINGID_PKMNSITEMRESTOREDHPALITTLE` 출력 조건 확인

- 이 문구는 `data/battle_scripts_1.s:7271-7277`의 `BattleScript_ItemHealHP_Ret`에서만 직접 출력된다. 스크립트는 아이템 효과 애니메이션 뒤 `STRINGID_PKMNSITEMRESTOREDHPALITTLE`을 출력하고 HP를 갱신한다.
- `src/battle_hold_effects.c:642-655`의 `TryLeftovers`가 체력이 가득 차지 않고 회복 봉인이 없을 때 이 스크립트를 호출한다. 따라서 먹다남은음식은 턴 종료 회복으로 최대 HP의 1/16을 회복할 때 `... 먹다남은음식으로\n조금 회복했다.`를 출력한다.
- 검은오물도 독 타입 포켓몬이면 같은 `TryLeftovers` 경로를 사용해 최대 HP의 1/16을 회복하므로 이 문구를 출력한다. 독 타입이 아니면 `TryBlackSludgeDamage`로 피해를 받아 이 문구가 나오지 않는다.
- `src/battle_hold_effects.c:538-554`의 `TryShellBell`도 공격 후 피해를 주고, 사용 포켓몬이 살아 있으며 HP가 가득 차지 않은 경우 이 스크립트를 호출한다. `src/data/items.h:10015-10030`에서 조개껍질방울의 회복 분모가 8이므로 준 피해의 1/8을 회복한다.
- `src/battle_hold_effects.c:1172-1177`의 오랭열매·자뭉열매(설정에 따라)·나무열매쥬스 등 일회성 HP 회복은 `BattleScript_ItemHealHP_RemoveItem`과 `STRINGID_PKMNSITEMRESTOREDHEALTH`를 사용하므로 이 ID가 아니다. `{B_SCR_NAME_WITH_PREFIX}`는 아이템 효과를 받은 포켓몬, `{B_LAST_ITEM}`은 발동한 아이템이다.
- 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다. mGBA 실행 파일이 없어 실제 게임 화면은 확인하지 않았다.

## 2026-09-23 — `STRINGID_PKMNREGAINEDHEALTH` 출력 조건 확인

- 이 문구는 실제 HP 회복 명령이 성공한 뒤 출력된다. 현재 호출 경로는 생명의물방울, 치유파동·플라워힐, HP회복·게으름피우기·회복지령, 알낳기·우유마시기, 꿀꺽, 희망사항의 회복 완료, 프레젠트의 회복 결과, 정글힐·초승달의기도의 HP 회복, 그리고 땅 타입 기술을 흡수한 `Earth Eater` 특성의 회복이다.
- 호출부는 `tryhealquarterhealth`, `tryhealpulse`, `tryhealhalfhealth`, `stockpiletohpheal`, `tryhealsixthhealth` 등의 성공 분기 뒤에 있다. 따라서 대상이 이미 체력이 가득 차서 회복 명령이 실패하면 보통 `STRINGID_PKMNHPFULL` 또는 해당 기술의 실패 문구가 출력되고 이 ID는 나오지 않는다. 정글힐 계열에서 HP는 가득하지만 상태이상만 치료하는 경우에는 체력 회복 문구 대신 `STRINGID_PKMNSTATUSNORMAL`이 출력된다.
- `{B_DEF_NAME_WITH_PREFIX}`는 회복된 포켓몬을 가리킨다. 자기 회복 기술에서는 사용 포켓몬, 치유파동·플라워힐·프레젠트·희망사항에서는 회복 대상, 정글힐 계열에서는 순회 중인 자신 또는 아군, Earth Eater에서는 땅 기술을 흡수한 특성 보유 포켓몬의 이름이 들어간다.
- 실제 출력은 예를 들어 `피카츄는 체력을\n회복했다!` 형태다. 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다.

## 2026-09-23 — `STRINGID_PKMNPREVENTSUSAGE` 출력 조건 확인

- 이 ID는 현재 `data/battle_scripts_1.s:6588-6595`의 `BattleScript_DampStopsExplosion`에서만 직접 출력된다. `src/battle_move_resolution.c:1459-1466`의 `CancelerExplodingDamp`가 필드에 `ABILITY_DAMP`를 가진 포켓몬이 있고 해당 기술의 `dampBanned` 플래그가 켜져 있으면 기술을 취소하고 이 스크립트로 이동한다.
- 현재 `dampBanned = TRUE`인 기술은 자폭, 대폭발, 깜짝헤드, 미스트버스트다. 따라서 이 기술을 사용하려 할 때 상대 또는 아군 필드의 포켓몬이 특성 `습기`를 가지고 있으면 출력된다.
- 스크립트 순서는 `pause → BattleScript_AbilityPopUpScripting → STRINGID_PKMNPREVENTSUSAGE`이므로 습기 특성 팝업이 먼저 나온다. 문장에는 특성 보유 포켓몬, `습기`, 사용하려던 기술명이 들어가며, 기술은 실제로 실행되지 않고 폭발·기절 효과도 발생하지 않는다.
- `src/battle_message.c:372`의 토큰은 `{B_DEF_NAME_WITH_PREFIX}`, `{B_DEF_ABILITY}`, `{B_ATK_NAME_WITH_PREFIX}`, `{B_CURRENT_MOVE}`를 사용한다. 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다.

## 2026-09-22 — `STRINGID_PKMNSTATUSNORMAL` 출력 조건 확인

- 이 ID의 현재 호출부는 `data/battle_scripts_1.s`의 네 곳이다. `브레이브차지`(`BattleScript_EffectTakeHeart`)와 `리프레시`(`BattleScript_EffectRefresh`)가 사용 포켓몬의 상태이상을 치료했을 때, `사이코시프트`가 상태이상을 상대에게 옮긴 뒤 사용 포켓몬의 상태를 치료했을 때, `정글힐`·`초승달의기도`(`BattleScript_EffectJungleHealing`)가 각 대상의 상태이상을 치료했을 때 출력된다.
- `브레이브차지`와 `리프레시`는 `curestatuswithmove`가 실제 상태이상이 있을 때만 성공 분기로 들어가므로, 상태이상이 없으면 이 문구가 출력되지 않는다. 정글힐 계열도 `jumpifstatus BS_TARGET, STATUS1_ANY`를 통과한 대상에게만 출력한다.
- `사이코시프트`에서는 상대에게 상태를 옮긴 뒤 `curestatus BS_ATTACKER`로 사용 포켓몬을 치료하면서 이 문구를 출력한다. `{B_ATK_NAME_WITH_PREFIX}`는 이 시점의 치료 대상이 되도록 설정된 포켓몬을 가리킨다.
- 이 ID는 모든 상태 회복의 공통 문구가 아니다. 특성 `Healer`, `Pastel Veil`, `Purify` 등은 각각 별도의 회복 문자열을 사용한다. 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다.

## 2026-09-22 — 특성 상태이상 문구와 화염구슬·맹독구슬 문구 분리 및 HNS 빌드

- `src/battle_message.c`의 상태이상 선택 테이블에서 특성 발생 슬롯을 일반 문구로 통일했다. 수면은 `STRINGID_PKMNFELLASLEEP`, 독은 `STRINGID_PKMNWASPOISONED`, 화상은 `STRINGID_PKMNWASBURNED`, 마비는 `STRINGID_PKMNWASPARALYZED`를 선택한다.
- 특성 스크립트의 `BattleScript_AbilityPopUp` 호출과 `setnonvolatilestatus TRIGGER_ON_ABILITY` 순서는 수정하지 않았으므로, 특성 팝업이 먼저 나오고 일반 상태이상 문구가 출력된다.
- `data/battle_scripts_1.s`의 `BattleScript_ToxicOrb`와 `BattleScript_FlameOrb`는 공통 상태이상 테이블을 사용하지 않고 각각 `STRINGID_PKMNPOISONEDBY`와 `STRINGID_PKMNBURNEDBY`를 직접 출력하도록 분리했다. 기존 아이템 팝업과 상태 애니메이션·아이콘 갱신은 유지했다. 현재 문자열 정의에 따라 각각 `맹독구슬 때문에 맹독에 중독됐다!`, `화염구슬 때문에 화상을 입었다!`가 출력된다.
- HNS 전체 빌드 성공: `build/localization-logs/hns-status-message-selection-20260922.log`, EWRAM `249,012/262,144`, IWRAM `25,704/32,768`, ROM `33,330,148/33,554,432 bytes (99.33%)`. 실제 게임 화면은 mGBA 실행 파일 부재로 확인하지 못했다.

## 2026-09-22 — 특성 수면 메시지 전 특성 팝업 순서 재확인

- `BattleScript_AbilityStatusEffect`와 `BattleScript_SynchronizeActivates`는 `waitstate → call BattleScript_AbilityPopUp → setnonvolatilestatus TRIGGER_ON_ABILITY` 순서로 실행된다. 따라서 `STRINGID_PKMNMADESLEEP`가 출력되는 특성 수면 경로에서는 특성 팝업이 먼저 표시된다.
- 이후 `BattleScript_MoveEffectSleep`의 `printfromtable gFellAsleepStringIds`가 `STRINGID_PKMNMADESLEEP`를 출력한다. 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았고, 실제 화면은 mGBA 부재로 확인하지 못했다.

## 2026-09-22 — `STRINGID_PKMNMADESLEEP`의 HNS 출력 여부 확인

- `STRINGID_PKMNMADESLEEP`는 문자열 테이블에 등록되어 있고, `src/battle_message.c:1209-1213`의 `gFellAsleepStringIds`에서 `B_MSG_STATUSED_BY_ABILITY`에 매핑되어 있다. 따라서 특성으로 수면 상태가 발생하면 이 ID가 간접적으로 출력될 수 있다.
- `data/battle_scripts_1.s:7009-7013`의 `BattleScript_AbilityStatusEffect`가 특성 팝업 후 `setnonvolatilestatus TRIGGER_ON_ABILITY`를 실행하고, `SetNonVolatileStatus()`가 수면 효과에 대해 `BattleScript_MoveEffectSleep`을 선택한다. 이 경로에서 `printfromtable gFellAsleepStringIds`가 `STRINGID_PKMNMADESLEEP`를 출력한다.
- 현재 소스에는 `ABILITY_EFFECT_SPORE`가 접촉한 공격자를 수면시킬 때 이 특성 경로를 호출하는 사례가 있다. 일반 기술로 잠들면 `STRINGID_PKMNFELLASLEEP`가 선택되고, 이 ID는 특성 발동 수면에 한정된다.
- 문자열의 `//not in gen 5+` 주석은 실행을 막지 않는다. HNS `GEN_LATEST`에서도 출력 가능하지만, 실제 화면은 mGBA 실행 파일 부재로 확인하지 못했다. 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다.

## 2026-09-22 — 화염구슬 화상 메시지에서 사용할 이름 토큰 확인

- `data/battle_scripts_1.s:5969-5975`의 `BattleScript_FlameOrb`는 `gEffectBattler`에 `gBattlerAttacker`를 복사한 뒤 `BattleScript_MoveEffectBurn`을 호출한다. 따라서 화염구슬을 지닌 포켓몬 자신이 화상을 입는 현재 경로에서는 `{B_EFF_NAME_WITH_PREFIX}`와 `{B_ATK_NAME_WITH_PREFIX}`가 우연히 같은 포켓몬을 출력한다.
- 문장의 의미가 ‘효과를 받아 화상에 걸린 포켓몬’이므로 주어 자리에는 `{B_EFF_NAME_WITH_PREFIX}`를 사용하는 것이 정확하다. `{B_ATK_NAME_WITH_PREFIX}`는 공격자라는 역할을 표현하는 토큰이라, 다른 화상 유발 경로와 재사용할 때 의미가 달라질 수 있다.
- 현재 HNS는 화염구슬 발동 때 `BattleScript_ItemPopUp_Scripting`으로 아이템 팝업을 먼저 표시한 뒤 `STRINGID_PKMNWASBURNED`의 `...화상을 입었다!`를 출력한다. 문자열에 `화염구슬 때문에`를 직접 포함하는 현재 경로는 없으므로, 해당 한 문장을 새로 출력하려면 별도 문자열/스크립트 변경이 필요하다.
- 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다. 실제 게임 화면은 mGBA 실행 파일 부재로 확인하지 못했다.

## 2026-09-22 — `{B_EFF_NAME_WITH_PREFIX}`의 의미와 출력 형태 확인

- `{B_EFF_NAME_WITH_PREFIX}`는 `include/battle_message.h:35`의 `B_TXT_EFF_NAME_WITH_PREFIX` 토큰이며, `src/battle_message.c:3307-3309`에서 `gEffectBattler`의 닉네임을 접두사와 함께 확장한다. 여기서 `gEffectBattler`는 현재 효과를 받은 포켓몬을 가리키며, 공격자(`gBattlerAttacker`)나 일반 대상(`gBattlerTarget`)과 항상 같은 슬롯은 아니다.
- 플레이어 편 포켓몬은 접두사 없이 닉네임만 출력한다. 트레이너 배틀의 상대 포켓몬은 `상대 `를 붙이고, 야생 배틀의 상대 포켓몬은 `야생 `을 붙인다. 예를 들어 닉네임이 `피카츄`라면 각각 `피카츄`, `상대 피카츄`, `야생 피카츄`가 된다.
- `GetBattlerNick()`이 환상 중인 포켓몬의 실제 표시 닉네임도 고려하므로, 환상 상태에서는 현재 화면에 표시되는 닉네임을 사용한다. `{B_EFF_NAME_WITH_PREFIX2}`는 같은 대상이지만 소문자/문장 중간용 처리 슬롯이다.
- 독·화상 메시지의 `{B_EFF_NAME_WITH_PREFIX}`는 상태이상을 받은 포켓몬을 표시한다. 특성명과 특성 발동 주체는 별도로 `{B_SCR_NAME_WITH_PREFIX}`와 `{B_SCR_ABILITY}`가 표시한다.
- 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다. 실제 게임 화면은 mGBA 실행 파일 부재로 확인하지 못했다.

## 2026-09-22 — 특성 발동 시 특성 팝업과 독·화상 메시지 순서 확인

- 일반 상태이상 경로는 `B_MSG_STATUSED`를 사용해 독에 `STRINGID_PKMNWASPOISONED`, 화상에 `STRINGID_PKMNWASBURNED`를 선택한다. 특성으로 발생한 경우에는 `B_MSG_STATUSED_BY_ABILITY`를 사용해 각각 `STRINGID_PKMNPOISONEDBY`, `STRINGID_PKMNBURNEDBY`를 선택한다.
- `data/battle_scripts_1.s:7009-7013`의 `BattleScript_AbilityStatusEffect`가 `waitstate → call BattleScript_AbilityPopUp → setnonvolatilestatus TRIGGER_ON_ABILITY` 순서로 실행되므로, 특성 팝업이 먼저 뜬 뒤 상태이상 메시지가 출력된다. `BattleScript_SynchronizeActivates`도 같은 순서를 사용한다.
- 현재 HNS에 이미 원하는 순서가 설정되어 있어 소스·데이터를 수정하거나 빌드하지 않았다. 실제 화면은 mGBA 실행 파일 부재로 확인하지 못했다.

## 2026-09-22 — `PKMNPOISONEDBY`·`PKMNBURNEDBY`·`PKMNFROZENBY`의 HNS `GEN_LATEST` 출력 여부

- `STRINGID_PKMNPOISONEDBY`와 `STRINGID_PKMNBURNEDBY`는 현재 HNS `GEN_LATEST`에서도 출력될 수 있다. `gGotPoisonedStringIds`·`gGotBurnedStringIds`가 `B_MSG_STATUSED_BY_ABILITY`를 각각 이 ID로 매핑하고, `BattleScript_AbilityStatusEffect`의 `setnonvolatilestatus TRIGGER_ON_ABILITY`가 해당 테이블을 출력한다. 이 경로에는 `GEN_5` 이상을 이유로 막는 조건이 없다.
- `STRINGID_PKMNFROZENBY`는 `gGotFrozenStringIds[B_MSG_STATUSED_BY_ABILITY]`에 매핑되어 있지만, 현재 소스에는 능력 발동 경로에서 `gBattleScripting.moveEffect = MOVE_EFFECT_FREEZE`를 설정하는 코드가 없다. 따라서 HNS `GEN_LATEST`에서 실제로 호출되지 않는다. 이는 `GEN_LATEST` 차단이 아니라 현재 동결 유발 능력 경로가 없는 것이 원인이다.
- 세 문자열의 `//not in gen 5+` 주석은 현재 HNS 출력 여부를 직접 제어하지 않는다. 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다. 실제 능력 발동 화면은 mGBA 부재로 확인하지 못했다.

## 2026-09-22 — 화염구슬·맹독구슬 상태이상 메시지 확인

- 화염구슬은 `data/battle_scripts_1.s:5969-5974`의 `BattleScript_FlameOrb`에서 일반 상태이상 선택값으로 `BattleScript_MoveEffectBurn`을 호출한다. 따라서 `gGotBurnedStringIds[B_MSG_STATUSED]`의 `STRINGID_PKMNWASBURNED`가 출력되고, 문구는 `{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}` 다음 줄의 `화상을 입었다!`다.
- 맹독구슬은 `data/battle_scripts_1.s:5962-5967`의 `BattleScript_ToxicOrb`에서 `BattleScript_MoveEffectToxic`를 호출한다. 이 스크립트는 `STRINGID_PKMNBADLYPOISONED`를 직접 출력하므로, 문구는 `{B_EFF_NAME_WITH_PREFIX}의` 다음 줄의 `몸에 맹독이 퍼졌다!`다.
- 따라서 화염구슬·맹독구슬은 각각 `STRINGID_PKMNWASBURNED`·`STRINGID_PKMNBADLYPOISONED`를 사용하며, 능력 발동 전용인 `STRINGID_PKMNBURNEDBY`·`STRINGID_PKMNPOISONEDBY`는 사용하지 않는다. 두 아이템 모두 상태 메시지 전에 아이템 팝업을 호출한다.
- 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다. 실제 게임 화면은 mGBA 실행 파일 부재로 확인하지 못했다.

## 2026-09-22 — 고속스핀·킬러스핀 제거 메시지 ID 적용

- `src/battle_message.c:1430-1433`의 `gSpinHazardsStringIds`를 변경해 고속스핀·킬러스핀(`EFFECT_RAPID_SPIN`)으로 제거되는 압정뿌리기는 `STRINGID_SPIKESDISAPPEAREDFROMTEAM`, 독압정은 `STRINGID_TOXICSPIKESDISAPPEAREDFROMTEAM`, 스텔스록은 `STRINGID_STEALTHROCKDISAPPEAREDFROMTEAM`, 끈적끈적네트는 `STRINGID_STICKYWEBDISAPPEAREDFROMTEAM`을 사용하게 했다. 강철서지는 기존 `STRINGID_PKMNBLEWAWAYSHARPSTEEL` 매핑을 유지했다.
- `data/battle_scripts_1.s:5080-5084`의 `BattleScript_WrapFree`를 `STRINGID_PKMNGOTFREE`에서 `STRINGID_PKMNFREEDFROM`으로 바꿨다. 따라서 고속스핀·킬러스핀으로 조이기·김밥말이·회오리불꽃·껍질끼우기·바다회오리·모래지옥·마그마스톰·엉겨붙기·집게덫·썬더프리즌이 해제되면 구속 기술명을 넣어 `...은\n{기술명}로부터 풀려났다!`를 출력한다.
- 씨뿌리기 해제는 기존 `BattleScript_LeechSeedFree`의 `STRINGID_PKMNSHEDLEECHSEED`를 그대로 사용한다. 고속스핀과 킬러스핀은 모두 `MOVE_EFFECT_RAPID_SPIN` 경로를 공유한다.
- HNS 빌드 성공: `build/localization-logs/hns-spin-hazard-release-messages-20260922.log`, EWRAM `249,012/262,144`, IWRAM `25,704/32,768`, ROM `33,330,148/33,554,432 bytes (99.33%)`. mGBA 실행 파일이 없어 실제 게임 화면 검증은 하지 못했다.

## 2026-09-22 — `{B_EFF_TEAM2}` 현재 소스값 정정

- 현재 `src/battle_message.c:1550`의 `sText_Your2`는 `우리 편`으로 정의되어 있고, `sText_Opposing2`는 `상대`로 정의되어 있다.
- 따라서 `{B_EFF_TEAM2}`는 효과 포켓몬이 플레이어 편이면 정확히 `우리 편`, 상대 편이면 `상대`를 출력한다. 이전 기록에서 토큰값을 `우리`라고 적은 부분은 이전 소스 상태를 기준으로 한 설명이므로 현재 상태에는 적용되지 않는다.
- 이번 확인에서는 사용자 소스 변경을 되돌리지 않았고, 소스·데이터·ROM 수정이나 빌드는 하지 않았다.

## 2026-09-22 — 독압정 제거 경로별 메시지 ID 확인

- `STRINGID_TOXICSPIKESDISAPPEAREDFROMTEAM`은 `gDefogHazardsStringIds[HAZARDS_TOXIC_SPIKES]`에 연결되어 있으며, 디포그 또는 정리정돈이 해당 편의 장판을 제거할 때 `BattleScript_DefogClearHazards`에서 출력된다. `{B_ATK_TEAM2}`는 제거 대상 편을 기준으로 `우리` 또는 `상대`를 선택한다.
- `STRINGID_TOXICSPIKESABSORBED`는 교대해 들어온 포켓몬이 땅에 붙어 있고 독 타입일 때 `src/battle_switch_in.c:337-342`에서 독압정을 흡수하면서 직접 출력된다. `{B_EFF_TEAM2}`는 흡수한 포켓몬의 편을 기준으로 `우리` 또는 `상대`를 선택한다.
- 현재 두 문자열의 본문은 모두 영어이므로 실제 치환 결과는 각각 `The poison spikes disappeared from the ground around 우리 team!` 또는 `... around 상대 team!`이다. 두 경로는 토큰의 기준 대상만 다르고, 현재 한국어 팀명 테이블에서는 결과가 같다.
- 고속스핀·킬러스핀은 `gSpinHazardsStringIds`의 `STRINGID_PKMNBLEWAWAYTOXICSPIKES`를 사용하므로 `포켓몬이 Toxic Spikes를 날려버렸다!` 계열의 별도 문구가 출력된다. 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다.

## 2026-09-22 — `{B_EFF_TEAM2}` 플레이어 편 치환값 재확인

- `{B_EFF_TEAM2}`는 `gEffectBattler`가 플레이어 편이면 `sText_Your2`인 `우리`, 상대 편이면 `sText_Opposing2`인 `상대`로 치환된다.
- 따라서 현재 HNS에서 효과 포켓몬이 플레이어 편이면 `우리` 편으로 출력되는 것이 맞다. 소스·데이터·ROM 수정과 빌드는 하지 않았다.

## 2026-09-22 — `{B_EFF_TEAM2}`의 정확한 치환 문자열 구분

- 토큰 자체의 출력값은 플레이어 편에서 `우리`이며, `우리 편`은 의미를 설명한 표현이다. 현재 `sText_Your2`에 `편`은 포함되어 있지 않다.
- 한국어 문장에서 `우리 편`으로 표시하려면 문자열 본문에 `편`을 직접 넣어야 한다. 소스·데이터·ROM 수정과 빌드는 하지 않았다.

## 2026-09-22 — `STRINGID_PKMNFREEDFROM` 출력 조건 확인

- `STRINGID_PKMNFREEDFROM`은 `data/battle_scripts_1.s:5870-5873`의 `BattleScript_WrapEnds`에서만 출력된다.
- 살아 있는 포켓몬이 조이기 계열의 다중 턴 구속(`wrapped`) 상태로 턴을 버틴 뒤 `src/battle_end_turn.c:609-630`에서 `wrapTurns`가 0이 되면 구속을 해제하고 이 메시지를 출력한다. `B_BUFF1`에는 `wrappedMove`의 기술명이 들어간다.
- 대상 기술은 조이기, 김밥말이, 회오리불꽃, 껍질끼우기, 바다회오리, 모래지옥, 마그마스톰, 엉겨붙기, 집게덫, 썬더프리즌이다. `{B_ATK_NAME_WITH_PREFIX}`에는 이 경로에서 구속에서 풀려나는 포켓몬의 이름과 접두사가, `{B_TXT_EU}`에는 기술명에 맞는 `으로/로`가 들어간다.
- 고속스핀·킬러스핀으로 구속을 즉시 제거하는 경우에는 `BattleScript_WrapFree`의 `STRINGID_PKMNGOTFREE`가 사용되므로 이 ID가 출력되지 않는다. 포켓몬이 먼저 기절해도 자연 해제 메시지는 출력되지 않는다.
- 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다. 게임 화면은 mGBA 실행 파일 부재로 확인하지 못했다. 다음 시작점은 새 ROM에서 구속 자연 종료와 고속스핀·킬러스핀 해제를 각각 재현해 두 메시지를 구분하는 것이다.

## 2026-09-22 — `{B_EFF_TEAM2}` 의미와 사용처 확인

- `{B_EFF_TEAM2}`는 `include/battle_message.h:96`의 `B_TXT_EFF_TEAM2` 토큰이며, 두 번째 팀을 가리키는 값이 아니다. 문자열 처리부가 `gEffectBattler`의 편을 확인해 플레이어 편이면 `sText_Your2`(`우리`), 상대 편이면 `sText_Opposing2`(`상대`)를 넣는다.
- 접미사 `2`는 영어 원본의 소문자·문장 중간용 팀명 변형을 위한 슬롯이다. 현재 HNS의 `sText_Your1/2`, `sText_Opposing1/2`가 모두 동일한 한국어라서 `{B_EFF_TEAM1}`과 실제 출력값은 같다.
- 현재 직접 사용처는 `STRINGID_TOXICSPIKESABSORBED`의 `The poison spikes disappeared from the ground around {B_EFF_TEAM2} team!`이며, 독 타입 포켓몬이 교대해 독압정을 흡수할 때 `gEffectBattler`가 해당 포켓몬으로 설정된다. 따라서 토큰은 그 포켓몬의 편을 기준으로 `우리` 또는 `상대`를 출력한다.
- 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다. 다음 시작점은 해당 독압정 흡수 문구를 한국어화할 때 문장 안에서 `우리/상대`가 자연스럽게 연결되는지 확인하는 것이다.

## 2026-09-22 — 신규 배틀 파티클 16개 설정의 ROM 용량 확인

- `B_NEW_SWORD_PARTICLE`부터 `B_NEW_SURF_PARTICLE_PALETTE`까지 16개 설정을 모두 `TRUE`로 바꾼 HNS 빌드와 모두 `FALSE`인 복구 빌드를 비교했다.
- 두 빌드의 링크 사용량은 모두 `33,330,212 / 33,554,432 bytes (99.33%)`로 동일했다. 따라서 현재 프로젝트에서 이 16개 설정을 전부 `TRUE`로 바꿔도 ROM 사용량은 증가하지 않고, 남은 ROM 공간은 `224,220 bytes`다. 실제 `pokehns.gba` 파일은 GBA 최대 크기에 맞춰 `33,554,432 bytes (32 MiB)`로 패딩된다.
- `src/data/battle_anim.h`는 설정값에 따라 신규/기존 그래픽·팔레트 포인터를 선택하지만, 관련 신규 리소스는 `src/graphics.c`에 이미 함께 포함되어 있어 링크 용량이 변하지 않는다. 설정을 TRUE로 바꾸면 용량이 아니라 실제 전투 애니메이션의 파티클 모양·팔레트가 바뀐다.
- 검증 로그는 `build/localization-logs/hns-all-new-particles-20260922.log`(전체 TRUE)와 `build/localization-logs/hns-all-new-particles-restore-final-20260922.log`(전체 FALSE)에 남겼다. 빌드 중 기존 `NO_BAG_INVALID_VaALUE` 오타만 임시로 `NO_BAG_INVALID_VALUE`로 고쳐 컴파일했고, 측정 후 원래 오타와 16개 설정을 모두 복구했다. 이 오타가 남아 있는 현재 작업 트리는 일반 HNS 재빌드가 실패하므로 다음 코드 작업 전에 별도 수정 여부를 결정해야 한다.
- 다음 시작점: 신규 파티클 적용을 결정하면 용량 검증은 완료된 상태이므로, 각 기술 애니메이션의 실제 화면·팔레트만 HNS ROM에서 확인한다.

## 2026-09-22 — `B_SHOW_TYPES` 가능한 값과 타입 아이콘 표시 조건 확인

- `include/config/battle.h`에 정의된 가능한 값은 `SHOW_TYPES_NEVER = 0`, `SHOW_TYPES_ALWAYS = 1`, `SHOW_TYPES_CAUGHT = 2`, `SHOW_TYPES_SEEN = 3` 네 가지다. 현재 HNS는 `SHOW_TYPES_NEVER`라서 기술 선택 화면에서 타입 아이콘을 만들지 않는다.
- `SHOW_TYPES_ALWAYS`는 살아 있는 배틀 포켓몬의 타입 아이콘을 항상 표시한다. `SHOW_TYPES_CAUGHT`는 도감에 해당 종을 잡은 기록이 있을 때만 실제 타입을 표시하고, 잡지 못한 종은 `TYPE_MYSTERY` 아이콘으로 표시한다. `SHOW_TYPES_SEEN`은 본 기록이 있을 때만 실제 타입을 표시하고, 종별 판정에서는 보지 못한 종을 `TYPE_MYSTERY`로 표시한다. 다만 아이콘 로딩 기준 포켓몬 자체가 미발견이면 `LoadTypeIcons()`가 전체 로딩을 먼저 중단한다.
- 타입 아이콘은 `src/battle_controller_player.c`의 기술 선택 초기화에서 `LoadTypeIcons()`를 호출할 때 처리된다. 타입 표시 설정과 별개로 `B_SHOW_EFFECTIVENESS`는 기술 상성 표시를 제어하므로 서로 혼동하지 않는다.
- 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다. 다음 시작점은 표시 설정을 바꿀 경우 HNS 빌드 후 싱글·더블 배틀, 포획/목격 여부, 환상·테라 타입의 실제 아이콘을 확인하는 것이다.

## 2026-09-22 — `B_PREFERRED_ICE_WEATHER` 가능한 값과 매핑 확인

- `include/config/battle.h`에 정의된 가능한 값은 `B_ICE_WEATHER_BOTH = 0`, `B_ICE_WEATHER_HAIL = 1`, `B_ICE_WEATHER_SNOW = 2` 세 가지이며, 현재 HNS 설정은 `B_ICE_WEATHER_BOTH`다. 임의의 다른 숫자보다 이 상수 중 하나를 사용해야 한다.
- `B_ICE_WEATHER_BOTH`: `싸라기눈`은 우박, `설경`과 `썰렁개그`는 눈으로 동작하고, 오로라베일 설명은 우박·눈 양쪽 사용을 안내한다. `차가운바위` 설명은 차가운 날씨 전반을 연장한다고 표시된다.
- `B_ICE_WEATHER_HAIL`: `싸라기눈`은 우박을 유지하고, `설경`·`썰렁개그`를 우박으로 매핑한다. 오로라베일 설명과 기술머신07 설명도 우박 기준으로 바뀐다.
- `B_ICE_WEATHER_SNOW`: `싸라기눈`과 `MOVE_EFFECT_HAIL`을 눈으로 매핑하고, `설경`·`썰렁개그`도 눈으로 동작한다. 오로라베일 설명과 기술머신07 설명은 눈 기준으로 바뀐다.
- 주의: 오로라베일의 실제 사용 가능 날씨 판정은 `src/battle_move_resolution.c`에서 `B_WEATHER_ICY_ANY`를 검사하므로, 현재 세 설정 모두 실제로는 우박 또는 눈에서 사용할 수 있다. 설정값은 기술 설명·기술 효과 매핑·아이템 설명을 바꾸며 이 판정을 단독으로 좁히지는 않는다.
- 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다. 다음 시작점은 설정을 변경할 경우 HNS 빌드 후 싸라기눈·설경·썰렁개그·오로라베일·기술머신07·차가운바위의 실제 효과와 문구를 확인하는 것이다.

## 2026-09-22 — HNS 지형 배경 자산과 `B_NEW_TERRAIN_BACKGROUNDS` 확인

- `B_NEW_TERRAIN_BACKGROUNDS = FALSE`는 지형 배경을 끄는 설정이 아니다. `B_TERRAIN_BG_CHANGE = TRUE`이므로 HNS는 전기·그래스·미스트·사이킥 필드에 따라 배틀 배경을 바꾼다.
- `src/graphics.c`의 조건부 포함에서 FALSE일 때 `electric_terrain`, `grassy_terrain`, `misty_terrain`, `psychic_terrain` 자산을 연결하고, TRUE일 때만 `new_*_terrain` 자산을 연결한다. 두 종류의 원본 PNG/팔레트/타일맵은 `graphics/battle_anims/backgrounds/`에 모두 존재한다.
- `src/battle_bg.c`의 `DrawTerrainTypeBattleBackground()`가 현재 지형에 맞는 `BG_*_TERRAIN`을 선택하고, `src/data/battle_anim.h`의 배경 테이블이 해당 이미지·팔레트·타일맵을 연결한다. 따라서 현재 HNS에는 기존 네 지형 배경이 있고, 새 버전 네 배경은 자산은 있으나 기본 ROM에 연결되지 않는다.
- 이번 확인에서는 소스·그래픽·ROM을 수정하거나 빌드하지 않았다.
- 다음 시작점: 새 지형 배경을 HNS에 적용하려면 `include/config/battle.h:332`를 TRUE로 바꾼 뒤 HNS 빌드와 네 지형의 실제 화면 검증을 수행한다.

## 2026-09-22 — `battle.h` 세대 설정값 범위 확인

- `include/config/general.h`에는 `GEN_1`부터 `GEN_9`까지가 정의되어 있으며, `GEN_CHAMPIONS = GEN_9 + 1`, `GEN_LATEST = GEN_CHAMPIONS`, `GEN_COUNT = GEN_CHAMPIONS + 1`이다. 따라서 `battle.h`의 세대형 설정에는 `GEN_3`·`GEN_LATEST` 외에도 `GEN_1`, `GEN_2`, `GEN_4`, `GEN_5`, `GEN_6`, `GEN_7`, `GEN_8`, `GEN_9`를 지정할 수 있다.
- 세대형 `B_*` 설정은 보통 `GetConfig(setting) >= GEN_X`로 비교되므로, 값을 `GEN_4`로 지정하면 해당 Gen4 이상 동작을 선택한다. `GEN_9`는 Gen9 기준이고 `GEN_LATEST`는 현재 프로젝트의 Champions 단계까지 포함한다. 모든 `B_*`가 세대값만 받는 것은 아니며 `TRUE`·`FALSE` 또는 수치 상수를 받는 항목도 있다.
- `GEN_6_XY`, `GEN_6_ORAS`, `GEN_8_PLA` 같은 별도 상수도 있지만 각각 특수한 오버월드 설정용이므로 일반 `battle.h` 설정값으로 임의 사용하지 않는다. 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다.
- 다음 시작점: 특정 설정을 바꿀 때 `include/config/general.h:62-74`의 상수 정의와 해당 `B_*` 사용부의 비교 연산을 함께 확인한다.

## 2026-09-22 — `//not in gen 5+` 주석과 `GEN_LATEST` 출력 여부 재검증

- 이전 기록에서 `//not in gen 5+`를 단순 주석으로만 설명한 것은 불충분했다. `STRINGID_PKMNATTACK`은 실제로 `include/config/battle.h`의 `B_BEAT_UP = GEN_LATEST`와 `src/battle_script_commands.c`의 `GetConfig(B_BEAT_UP) >= GEN_5` 분기 때문에 HNS에서 출력되지 않는다. 이 부분은 이전 답변의 오류다.
- `src/battle_message.c`의 같은 주석 문자열들을 모두 대조한 결과, 현재 `GEN_LATEST` 설정이 해당 문자열 호출을 직접 차단하는 추가 사례는 확인되지 않았다. `PKMNGOTFREE`, `PKMNSHEDLEECHSEED`, `PKMNBLEWAWAYSPIKES`, `RAINCONTINUES`, `SUNLIGHTSTRONG`, `SNOWCONTINUES`, `NOPPLEFT`, `ITEMSCANTBEUSEDNOW`, `PKMNFLEDUSING`, 각종 능력 발동·상태이상 문자열은 현재 HNS의 스크립트·테이블에서 여전히 선택될 수 있다.
- `THEWALLSHATTERED` 정의는 레거시 호환성을 위해 남아 있지만, 현재 `깨뜨리다`·`사이코팽`·`레이징불`의 방벽 제거 경로에서는 선택되지 않는다. 이 경로는 제거된 방벽 비트를 저장한 뒤 전용 문구를 고정 순서로 직접 출력한다.
- `PKMNWENTTOSLEEP`와 `PKMNSTAYEDAWAKEUSING`은 현재 각각 Rest 테이블과 불면 방지 공통 스크립트가 다른 문자열로 교체되어 출력되지 않는 상태지만, 이는 `GEN_LATEST` 때문이 아니라 이 프로젝트의 기존 변경 때문이다.
- 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았고, 상태·세션 문서만 갱신했다.
- 다음 시작점: 새 문자열을 제거·교체하기 전 `src/battle_message.c`의 주석보다 실제 `printstring`/`printfromtable` 호출부와 `GetConfig`/`GEN_*` 분기를 먼저 확인한다.

## 2026-09-22 — `STRINGID_PKMNATTACK`의 HNS 출력 조건 확인

- 이 문자열의 직접 호출부는 `data/battle_scripts_1.s`의 `BattleScript_BeatUpAttackMessage` 하나이며, 집단구타의 각 공격에 앞서 `{B_BUFF1}의 공격!`을 출력하는 용도다. `src/battle_script_commands.c`는 `{B_BUFF1}`에 집단구타에 참가하는 파티 포켓몬의 이름과 접두사를 넣는다.
- 다만 HNS의 `include/config/battle.h`에서 `B_BEAT_UP`이 `GEN_LATEST`로 설정되어 있다. `MOVE_EFFECT_BEAT_UP_MESSAGE`는 `GetConfig(B_BEAT_UP) >= GEN_5`이면 즉시 종료하므로 현재 HNS의 집단구타 경로에서는 이 문자열이 출력되지 않는다. `//not in gen 5+`는 이 동작을 설명하는 주석이다.
- 따라서 현재 HNS 일반 배틀에서 예상되는 집단구타 문구는 `STRINGID_PKMNATTACK`가 아니라 기본 기술 사용 문구이며, `B_BEAT_UP`을 GEN_4 이하로 낮춘 구성에서만 예를 들어 `피카츄의 공격!`처럼 출력된다. 이번 확인에서는 소스·데이터·ROM을 수정하거나 빌드하지 않았다.
- 다음 시작점: 집단구타의 현재 세대별 설정을 유지할지 결정할 때 `include/config/battle.h:123`, `src/battle_script_commands.c:3683-3700`, `data/battle_scripts_1.s:3451-3453`을 함께 확인한다.

## 2026-09-22 — `STRINGID_PKMNREDUCEDPP` 출력 조건과 버퍼 확인

- 이 문자열은 대상 포켓몬의 마지막 사용 기술 PP를 실제로 줄이는 데 성공했을 때 출력된다. 직접 `원한(Spite)` 성공 시 `BattleScript_EffectSpite`, `섬뜩한주문(Eerie Spell)`의 추가 효과 성공 시 `BattleScript_MoveEffectEerieSpell`, `거다이감쇠(G-Max Depletion)`의 `MOVE_EFFECT_SPITE` 추가 효과 성공 시 `BattleScript_EffectTryReducePP`가 사용한다.
- `{B_DEF_NAME_WITH_PREFIX}`는 PP가 줄어든 대상 포켓몬 이름과 야생/상대 접두사, `{B_BUFF1}`은 대상이 마지막으로 사용한 기술명, `{B_TXT_EULREUL}`은 그 기술명에 맞는 을/를, `{B_BUFF2}`는 실제 감소량이다. PP라는 단위는 문구에 직접 표시되지 않는다.
- 현재 HNS 설정에서는 원한이 기본 4 PP, 섬뜩한주문이 3 PP, 거다이감쇠가 2 PP를 줄이며 남은 PP가 더 적으면 남은 수치까지만 줄인다. 대상이 사용한 기술이 없거나 PP 감소 조건을 만족하지 못하면 이 문자열은 출력되지 않는다.
- 예시는 상대 팬텀의 마지막 기술이 섀도볼일 때 원한은 `상대 팬텀의\n섀도볼을 4 깎았다!`, 섬뜩한주문은 `상대 팬텀의\n섀도볼을 3 깎았다!`다. 이번 확인에서는 소스·ROM을 수정하거나 빌드하지 않았다.
- 다음 시작점: 새 ROM에서 원한·섬뜩한주문·거다이감쇠를 각각 성공시켜 대상 이름 접두사, 기술명 조사, 감소량을 실제 화면에서 확인한다.

## 2026-09-22 — 오로라베일 성공 문구를 물리·특수공격 동시 상승 메시지로 연결 및 HNS ROM 재빌드

- `STRINGID_PKMNRAISEDDEFSPDEF`를 문자열 ID 열거에 등록하고, 사용자가 추가한 `src/battle_message.c` 문구 `{B_ATK_PREFIX2} {B_CURRENT_MOVE}{B_TXT_EU}로\n물리공격과 특수공격에 강해졌다!`를 유지했다.
- 신비의부적과 오로라베일이 기존 `B_MSG_SET_SAFEGUARD`를 공유해 단순 테이블 교체 시 신비의부적 문구까지 바뀌는 문제를 피하기 위해 `B_MSG_SET_AURORA_VEIL`을 별도로 추가했다. 신비의부적은 `STRINGID_PKMNCOVEREDBYVEIL`을 유지하고, 오로라베일의 직접 효과 및 `MOVE_EFFECT_AURORA_VEIL` 추가 효과만 새 ID를 사용한다.
- 정적 확장 결과 아군은 `우리 편은 오로라베일로\n물리공격과 특수공격에 강해졌다!`, 상대는 `상대는 오로라베일로\n물리공격과 특수공격에 강해졌다!`가 된다. `{B_TXT_EU}`는 마지막 글자 `일`의 ㄹ 받침에서 `으`를 생략하므로 `오로라베일로`가 맞다.
- 관련 `git diff --check`에 새 공백 오류가 없고, 대상 오브젝트 빌드는 헤더 의존성 재생성으로 180초 제한에 걸렸지만 전체 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8`는 종료 코드 0으로 성공했다. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,328,676/33,554,432(99.33%)이다. GBA SHA-256은 `5b848b2b78bc90667fafafb7456e4e4726bddf65a22d5d7d90a0beb7531e88bd`, ELF는 `f5a2a74d2e65e03965d113b1d8d017065160e8153e6ba19a3918d7f4f0d8344e`이며 로그는 `build/localization-logs/hns-aurora-veil-stat-message-20260922.log`다.
- mGBA 실행 파일이 없어 실제 게임 화면에서 줄바꿈·글리프는 확인하지 못했다. 다음 시작점은 새 ROM에서 오로라베일 성공 및 신비의부적 성공을 각각 실행해 두 문구가 분리되는지 확인하는 것이다.

## 2026-09-22 — 오로라베일 발동 메시지 재확인

- 오로라베일을 성공적으로 설치하면 `src/battle_script_commands.c`의 `BS_SetAuroraVeil()`이 `B_MSG_SET_SAFEGUARD`를 선택하고, `data/battle_scripts_1.s`의 `BattleScript_MoveEffectAuroraVeil`이 `gReflectLightScreenSafeguardStringIds`를 출력한다. 따라서 `STRINGID_PKMNRAISEDDEF`·`STRINGID_PKMNRAISEDSPDEF`가 아니라 `STRINGID_PKMNCOVEREDBYVEIL`이 사용된다.
- 현재 문구는 `{B_ATK_PREFIX2}\n신비의 베일에 둘러싸였다!`다. 아군 측이면 `우리 편은\n신비의 베일에 둘러싸였다!`, 상대 측이면 `상대는\n신비의 베일에 둘러싸였다!`로 출력된다. 문장에 기술명 `오로라베일`은 직접 표시되지 않는다.
- 이미 같은 편에 오로라베일이 설치된 상태에서 다시 사용하면 `B_MSG_SIDE_STATUS_FAILED`로 `STRINGID_BUTITFAILED`가 출력된다. 이번 확인에서는 소스·데이터와 ROM을 수정하지 않았고 빌드도 실행하지 않았다.
- 다음 시작점: 오로라베일 성공·중복 사용을 새 ROM에서 실제 재현해 줄바꿈과 글리프를 확인한다. mGBA 실행 파일이 없어 이번 세션에서는 화면 검증을 하지 않았다.

## 2026-09-22 — Rest 성공 메시지를 `STRINGID_PKMNSLEPTHEALTHY`로 통일 및 HNS ROM 재빌드

- `src/battle_message.c`의 `gRestUsedStringIds`에서 상태 이상이 없을 때 쓰던 `B_MSG_REST` 항목도 `STRINGID_PKMNSLEPTHEALTHY`를 가리키도록 변경했다. `B_MSG_REST_STATUSED` 항목도 같은 ID이므로 Rest 성공 시 상태 이상 유무와 관계없이 `잠이 들어 건강해졌다!` 문구가 출력된다.
- `Cmd_trysetrest`의 상태 이상 판정, 상태 초기화, 수면 부여, 회복량 계산과 Rest 실패 분기는 수정하지 않았다. `STRINGID_PKMNWENTTOSLEEP` 문자열 정의와 ID는 남아 있지만 현재 Rest 성공 테이블에서는 선택되지 않는다.
- 변경된 `src/battle_message.c` 오브젝트와 전체 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8`가 종료 코드 0으로 완료됐다. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,328,612/33,554,432(99.33%)이다. `pokehns.gba` SHA-256은 `8b33ebd0d16a8fab1c794911c61443f2a33488d472153cb7639403a915d3270c`, `pokehns.elf`는 `88a806049354b7ce047e7334d5a7387f7307835967a2b0dc73a247d296b755c0`이다. 로그는 `build/localization-logs/hns-rest-healthy-message-20260922.log`에 남겼다.
- 작업 트리에 기존 변경이 많아 전체 `git diff --check`에는 기존 파일들의 공백 경고가 함께 나타났다. 이번 변경 줄에는 새 공백 오류가 없다. mGBA 실행 파일이 없어 실제 화면 출력은 확인하지 못했다.

## 2026-09-22 — HNS Rest 성공 메시지 형태 재확인

- 상태 이상 없이 잠자기에 성공하면 `STRINGID_PKMNWENTTOSLEEP`이 선택되어 `{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}` 다음 줄에 `잠을 자기 시작했다!`가 출력된다.
- 수면 이외의 상태 이상을 치료하면서 성공하면 `STRINGID_PKMNSLEPTHEALTHY`가 선택되어 같은 이름·조사 다음 줄에 `건강한 상태로 잠을 자기 시작했다!`가 출력된다. 플레이어 포켓몬은 닉네임, 상대 포켓몬은 야생/상대 접두사가 이름에 반영된다.
- 이번 확인에서는 소스 수정과 빌드를 하지 않았다.

## 2026-09-22 — Rest 수면 시작 문자열 출력 조건 확인

- `STRINGID_PKMNWENTTOSLEEP`와 `STRINGID_PKMNSLEPTHEALTHY`는 다른 수면 기술의 일반 메시지가 아니라 Rest 성공 시 `BattleScript_EffectRest`의 `printfromtable gRestUsedStringIds`에서 선택된다.
- Rest 사용 포켓몬에게 수면 이외의 상태 이상이 없으면 `B_MSG_REST`가 선택되어 `STRINGID_PKMNWENTTOSLEEP`가 출력된다. 독·마비·화상 등 수면 이외의 상태 이상이 있으면 `B_MSG_REST_STATUSED`가 선택되어 기존 상태를 치료하고 `STRINGID_PKMNSLEPTHEALTHY`가 출력된다.
- 이미 잠든 상태, 체력 최대, 소란피기, 불면·의기양양·정화의소금, 전기/안개 필드 등으로 Rest가 실패하면 두 문자열 모두 출력되지 않는다. `//not in gen 5+`는 주석일 뿐이며 현재 HNS의 Rest 성공 경로에서는 첫 문자열이 출력될 수 있다.
- 이번 확인에서는 소스 수정과 빌드를 하지 않았다.

## 2026-09-22 — Rest 특성 방해 메시지를 `STRINGID_ITDOESNTAFFECT`로 변경 및 HNS ROM 재빌드

- `data/battle_scripts_1.s`의 공통 `BattleScript_InsomniaProtects`에서 특성 팝업 뒤 출력하던 `STRINGID_PKMNSTAYEDAWAKEUSING`을 `STRINGID_ITDOESNTAFFECT`로 교체했다. 불면·의기양양·정화의소금이 Rest를 막는 C/스크립트 양쪽 분기는 모두 이 공통 스크립트를 거치므로 별도 분기를 추가하지 않았다.
- 특성 팝업 호출(`call BattleScript_AbilityPopUp`)과 실패 플래그 설정은 유지된다. 따라서 이제 팝업 뒤 `상대 포켓몬에게는\n효과가 없는 것 같다...` 같은 대상 무효 문구가 출력된다. 기존 `STRINGID_PKMNSTAYEDAWAKEUSING` 정의와 Arena 점수용 참조는 호환성을 위해 남겨 두었고, 배틀 스크립트 호출은 제거됐다.
- `git diff --check`와 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8`가 종료 코드 0으로 완료됐다. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,328,612/33,554,432(99.33%)이다. `pokehns.gba` SHA-256은 `130f9cea025095bd6e9dc94bad9ef6b4cfa063a4db917da6938ed526a387044c`, `pokehns.elf`는 `100c90ac3077b89c6f9049780afc7dfdd288442b6dff03b6f2b7ec1e4aa280cf`이다. 로그는 `build/localization-logs/hns-rest-no-effect-20260922.log`에 남겼다.
- mGBA 실행 파일이 없어 실제 게임 화면에서 특성 팝업·문구 순서는 재현하지 못했다.

## 2026-09-22 — `STRINGID_ITDOESNTAFFECT`와 `STRINGID_SCR_ITDOESNTAFFECT` 구분

- 두 문자열의 문장은 같지만 이름을 확장하는 기준이 다르다. `B_DEF_NAME_WITH_PREFIX`는 `gBattlerTarget`(현재 대상)을, `B_SCR_NAME_WITH_PREFIX`는 `gBattleScripting.battler`(배틀 스크립트가 지정한 활성 전투원)를 사용한다.
- 일반적인 기술 무효·상성 없음 경로는 `STRINGID_ITDOESNTAFFECT`를 사용하고, 가루 기술 무효나 특성으로 막힌 스크립트처럼 `BS_SCRIPTING` 전투원을 기준으로 처리하는 경로는 `STRINGID_SCR_ITDOESNTAFFECT`를 사용한다. 둘 다 전투원의 진영에 따라 야생/상대 접두사를 붙인다.
- 이번 확인에서는 소스 수정과 빌드를 하지 않았다.

## 2026-09-22 — `STRINGID_PKMNSTAYEDAWAKEUSING` 출력 조건 확인

- 이 문자열은 `잠자기(Rest)`가 자신을 재우려 할 때 포켓몬의 `불면`, `의기양양`, 또는 최신 규칙의 `정화의소금` 때문에 실패하면 출력된다. `src/battle_move_resolution.c`가 이 경우 `BattleScript_InsomniaProtects`를 선택하며, 기존 `BattleScript_EffectRest`에도 같은 분기가 있다.
- `BattleScript_InsomniaProtects`는 `BattleScript_AbilityPopUp`을 먼저 호출한 뒤 이 문자열을 출력한다. 따라서 현재 HNS에서는 특성 팝업 후 `… 때문에 잠들지 않는다!` 문장이 나온다.
- 이번 확인에서는 소스 수정과 빌드를 하지 않았다.

## 2026-09-22 — `gText_PkmnsXPreventsSwitching` 출력 조건 확인

- 이 문자열은 파티 메뉴에서 교체를 시도했지만 상대 포켓몬의 교체 방지 특성 때문에 교체할 수 없을 때 `SetMonPreventsSwitchingString()`이 확장한다.
- `{B_BUFF1}`은 교체를 막은 포켓몬의 닉네임과 야생/상대 접두사, `{B_LAST_ABILITY}`는 `gBattleStruct->abilityPreventingSwitchout`의 특성명, `{B_BUFF2}`는 교체하려던 플레이어 포켓몬의 닉네임이다. `{B_TXT_EULREUL}`은 닉네임에 맞는 을/를을 선택한다.
- 예시는 `상대 포켓몬의 그림자밟기 때문에\n리자몽을 불러들일 수 없다!`와 같은 형태다. 이번 확인에서는 소스 수정과 빌드를 하지 않았다.

## 2026-09-22 — `{PLAY_SE 0x0011}` 제어 코드 확인

- `{PLAY_SE ...}`는 텍스트에 표시되는 문자가 아니라 문자열 출력 중 효과음을 재생하는 확장 제어 코드다. `0x0011`은 10진수 17이며 `SE_FLEE`(도망 효과음)에 해당한다.
- 이 코드는 `STRINGID_WILDPKMNFLED`, `STRINGID_PKMNFLEDUSINGITS`, `STRINGID_PKMNFLEDUSING`처럼 포켓몬이 도망쳤다는 메시지 앞에 배치되어 효과음을 먼저 재생한 뒤 문장을 계속 출력한다. 이번 확인에서는 소스 수정과 빌드를 하지 않았다.

## 2026-09-22 — `//not in gen 5+` 주석의 의미 확인

- `//not in gen 5+`는 개발자가 본가 세대별 메시지 차이를 적어 둔 설명 주석일 뿐이며, 컴파일러나 게임 런타임이 이 주석을 읽어 출력을 차단하지 않는다.
- 실제로 출력되지 않게 하려면 배틀 스크립트의 호출 제거, 세대 조건 분기, 설정값 확인 등이 별도로 필요하다. 따라서 주석이 있어도 현재 HNS 경로가 해당 문자열을 호출하면 출력될 수 있고, 반대로 호출부가 없으면 출력되지 않는다.
- 이번 확인에서는 소스와 ROM을 수정하거나 빌드하지 않았다.

## 2026-09-22 — 흰안개·신비의부적 안개제거 해제 메시지 연결 및 HNS ROM 재빌드

- `STRINGID_NOLONGERMIST`를 문자열 ID로 등록했다. 흰안개(Mist)의 턴 종료 만료는 `BattleScript_MistWoreOff`, 안개제거 해제는 `BattleScript_MistWoreOffReturn`을 사용해 이 ID를 출력한다.
- 신비의부적(Safeguard)이 안개제거로 제거될 때는 `BattleScript_SafeguardEndsReturn`을 사용해 `STRINGID_PKMNSAFEGUARDEXPIRED`를 출력한다. 턴 종료로 자연 만료되는 기존 `BattleScript_SafeguardEnds` 경로도 같은 ID를 계속 사용한다.
- 사용자가 추가한 `STRINGID_NOLONGERMIST` 문구 본문은 변경하지 않았다. `{B_ATK_PREFIX3}`에 따라 `우리 편을 감싸던 흰안개가 없어졌다!` 또는 `상대를 감싸던 흰안개가 없어졌다!` 형태로 확장된다.
- 변경된 C 오브젝트·배틀 스크립트 오브젝트와 전체 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8`가 종료 코드 0으로 완료됐다. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,328,612/33,554,432(99.33%)이다. `pokehns.gba` SHA-256은 `74f19d0406e68947195e811aed36d3fa73387ffe6e7f236e84175c78d2c0654b`, `pokehns.elf`는 `8653742c2445fa49eaf5b4362c4097d195b3d1c6308a20ad64936df7b7e3e4da`이다. 로그는 `build/localization-logs/hns-mist-safeguard-expiry-20260922.log`에 남겼다.
- mGBA 실행 파일이 없어 실제 화면에서 줄바꿈과 조사 출력을 재현하는 검증은 하지 못했다.

## 2026-09-22 — 리플렉터·빛의장막 표기와 `{B_ATK_TEAM1}` 출력 구분

- 앞서 표의 `우리 편`/`상대 편`은 어느 진영의 화면인지 설명한 대상 라벨이다. 실제 문자열은 `{B_ATK_TEAM1}`을 사용하므로 `우리`/`상대`가 들어가고, `{B_ATK_TEAM1}의`는 `우리의`/`상대의`로 출력된다.
- 따라서 현재 문구의 `우리의 리플렉터가 없어졌다!`, `상대의 리플렉터가 없어졌다!` 등은 코드와 일치하며 잘못된 출력이 아니다. 정확히 `우리 편의`를 넣으려면 `{B_ATK_PREFIX1}의`를 사용해야 하지만, 현재 상대 측 prefix는 `상대`라서 `상대의`가 된다.
- 이번 확인에서는 소스 수정과 빌드를 하지 않았다.

## 2026-09-22 — `B_ATK_PREFIX` 조사 형태 확인

- 별도의 `B_ATK_PREFIX_의` 플레이스홀더는 없다. `{B_ATK_PREFIX1}`이 우리 편에서는 `우리 편`, 상대 편에서는 `상대`로 확장되므로 `{B_ATK_PREFIX1}의`를 쓰면 `우리 편의`/`상대의`가 된다.
- `{B_ATK_TEAM1}`은 `우리`/`상대`이므로 `{B_ATK_TEAM1}의`는 `우리의`/`상대의`가 된다. 현재 리플렉터·빛의장막 문구가 이 형태를 사용한다.
- 참고로 `{B_ATK_PREFIX2}`는 `우리 편은`/`상대는`, `{B_ATK_PREFIX3}`은 `우리 편을`/`상대를`이다. 방어 측에는 대응하는 `B_DEF_PREFIX1~3`가 있다. 이번 확인에서는 소스 수정과 빌드를 하지 않았다.

## 2026-09-22 — `STRINGID_PKMNCOVEREDBYVEIL` 출력 조건 확인

- 이 문자열은 `gReflectLightScreenSafeguardStringIds`의 `B_MSG_SET_SAFEGUARD` 항목이다. 신비의부적(Safeguard)이 성공적으로 설치될 때 `BattleScript_EffectSafeguard`가 출력한다.
- 오로라베일도 설치 성공 시 같은 테이블 항목을 선택하므로 현재 HNS에서는 이 문자열을 함께 사용한다. 리플렉터·빛의장막 설치에는 각각 방어/특수방어 상승 문자열이 사용된다.
- `{B_ATK_PREFIX2}`는 우리 편이면 `우리 편은`, 상대 편이면 `상대는`으로 확장된다. 따라서 출력은 `우리 편은\n신비의 베일에 둘러싸였다!` 또는 `상대는\n신비의 베일에 둘러싸였다!`이다. 이미 같은 상태가 있으면 실패 문구가 출력되고 이 ID는 사용되지 않는다.
- 이번 확인에서는 소스 수정과 빌드를 하지 않았다.

## 2026-09-22 — `STRINGID_PKMNSXWOREOFF` 현재 출력 경로 확인

- 이 문자열은 현재 일반적인 화면 만료 공통 메시지다. `src/battle_end_turn.c`에서 턴 종료로 **안개(Mist)**가 끝날 때 `BattleScript_SideStatusWoreOff`가 호출되고, 해당 스크립트가 이 문자열을 출력한다.
- 안개제거(Defog)로 **안개** 또는 **신비의부적(Safeguard)**이 해제될 때도 `BattleScript_SideStatusWoreOffReturn`을 통해 이 문자열이 출력된다. 신비의부적의 자연 만료는 별도 `STRINGID_PKMNSAFEGUARDEXPIRED`를 사용한다.
- 리플렉터·빛의장막·오로라베일은 현재 전용 `...WoreOff` 문자열로 분리했으므로 이 ID를 사용하지 않는다. 이번 확인에서는 소스와 ROM을 수정하거나 빌드하지 않았다.
- `{B_ATK_PREFIX1}`은 우리 편에서 `우리 편`, 상대 편에서 `상대`로 확장되고, `{B_BUFF1}`에는 해제된 기술명이 들어간다. 따라서 예시는 `우리 편 안개의\n효과가 떨어졌다!` 또는 `상대 안개의\n효과가 떨어졌다!`이다.

## 2026-09-22 — 리플렉터·빛의장막 해제 메시지 연결 및 HNS ROM 재빌드

- `STRINGID_REFLECTWOREOFF`와 `STRINGID_LIGHTSCREENWOREOFF`를 문자열 ID로 등록하고, 턴 종료 만료 및 안개제거(Defog)로 해제되는 경로가 각각 전용 배틀 스크립트를 사용하도록 연결했다. 사용자가 추가한 `STRINGID_AURORAVEILWOREOFF`도 같은 방식으로 등록·연결했다.
- 기술로 즉시 벽을 부수는 경로의 `BattleScript_BreakScreens`는 화면 종류 테이블을 사용한다. 리플렉터·빛의장막·오로라베일 중 하나만 제거하면 해당 전용 문구가 나오고, 둘 이상이 함께 제거되면 기존 `STRINGID_THEWALLSHATTERED`를 유지한다.
- `{B_ATK_TEAM1}`은 우리 편이면 `우리`, 상대 편이면 `상대`로 확장되므로 각각 `우리의 리플렉터가 없어졌다!`·`상대의 리플렉터가 없어졌다!`처럼 출력된다. 더블 배틀에서도 포켓몬 개별명이 아니라 팀 기준 접두사가 사용된다.
- `git diff --check`와 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8`가 종료 코드 0으로 완료됐다. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,328,580/33,554,432(99.33%)이다. `pokehns.gba` SHA-256은 `a3afa978d0124b1fc6b48b93be4378c1ab1b5c4f5e5c6939090ba4caf3e454e9`, `pokehns.elf`는 `50b7124f96fc49bddfcca883d7641bb6517baf59b05d8ed6a23882b1b45046c0`이다. 로그는 `build/localization-logs/hns-screen-woreoff-20260922.log`에 남겼다.
- mGBA 실행 파일이 없어 실제 게임 화면에서 줄바꿈·글리프를 재현하는 검증은 하지 못했다. 새 ROM에서 턴 종료, 안개제거, 벽 파괴 기술의 단일·복수 화면 조합을 각각 확인한다.

## 2026-09-21 — `STRINGID_ITEMSCANTBEUSEDNOW` 출력 조건 확인

- 플레이어가 배틀 행동 선택에서 도구를 골랐을 때, `ShouldBattleRestrictionsApply()`가 참이고 `IsAllowedToUseBag()`가 거짓이면 `BattleScript_ActionSelectionItemsCantBeUsed`가 이 문구를 선택창에 출력한다. `B_VAR_NO_BAG_USE`가 `NO_BAG_AGAINST_TRAINER`이면 트레이너전, `NO_BAG_IN_BATTLE`이면 모든 배틀에서 해당 제한이 적용된다.
- 별도 분기로 링크 배틀, `BATTLE_TYPE_FRONTIER_NO_PYRAMID`, e-Reader 트레이너전, 기록 링크 배틀, 공중날기(Sky Drop) 상태에서도 같은 스크립트를 사용한다. 세대 설정으로 억제하는 조건은 없고, 기본 `B_VAR_NO_BAG_USE = 0`인 일반 야생전·트레이너전에서는 이 경로가 보통 실행되지 않는다.
- 이번 확인에서는 문구를 수정하지 않았다.

## 2026-09-21 — 세대 기준(`GEN_LATEST`)과 햇살 메시지 관계 확인

- `include/config/general.h`의 `GEN_3`~`GEN_CHAMPIONS`는 세대 상수이고, `GEN_LATEST`를 바꾸면 이를 기본값으로 쓰는 여러 `include/config/battle.h` 설정이 함께 바뀐다.
- 햇살 발동 메시지 자체는 세대 비교로 감싸져 있지 않다. CHAMPIONS에서 `B_ABILITY_WEATHER = GEN_LATEST`가 되어 능력 날씨 지속 시간 등은 최신 규칙을 따르지만, 날씨 변경 성공 시 `STRINGID_PKMNSXINTENSIFIEDSUN` 출력 경로는 GEN_3로 바꿔도 유지된다.

## 2026-09-21 — 햇살 강화 메시지의 CHAMPIONS 설정 동작 확인

- `STRINGID_PKMNSXINTENSIFIEDSUN`은 `ABILITY_DROUGHT`와 `ABILITY_ORICHALCUM_PULSE`가 일반 햇살로 날씨를 바꾸는 데 성공할 때 `BattleScript_WeatherAbilityActivates`에서 출력된다. 스크립트는 먼저 특성 팝업을 표시한 뒤 이 문구를 출력한다.
- `B_UPDATED_ABILITY_DATA = GEN_LATEST`(CHAMPIONS 기준)이어도 이 경로를 끄는 분기는 없다. 주석의 “not in gen 5+”는 최신 본가 메시지 정책에 대한 설명일 뿐, 현재 HNS 코드에서 문구가 비활성화됐다는 뜻은 아니다.
- `ABILITY_DESOLATE_LAND`(끝의대지)는 별도의 `STRINGID_EXTREMELYHARSHSUNLIGHT`를 사용한다. 이번 확인에서는 문구를 수정하지 않았다.

## 2026-09-21 — `STRINGID_FOREWARNACTIVATES` 출력 경로 확인

- 이 문자열은 `ABILITY_FOREWARN`인 **예지몽**의 교체 출전 메시지다. 상대편 포켓몬이 있을 때 `ForewarnChooseMove()`가 상대 기술을 선택하고, 특성 팝업 뒤 `gSwitchInAbilityStringIds[B_MSG_SWITCHIN_FOREWARN]`를 통해 출력된다.
- `{B_SCR_ABILITY}`는 예지몽, `{B_SCR_NAME_WITH_PREFIX2}`는 특성 보유자, `{B_EFF_NAME_WITH_PREFIX2}`와 `{B_BUFF1}`은 선택된 상대 포켓몬과 기술을 가리킨다. `ABILITY_ANTICIPATION`인 위험예지와는 별도 특성이다.
- 현재 문구 본문은 영어 상태이며 이번 확인에서는 수정하지 않았다.

## 2026-09-21 — `STRINGID_AFTERMATHDMG` 출력 경로 확인

- `STRINGID_AFTERMATHDMG`는 접촉 기술로 `유폭` 보유 포켓몬이 쓰러진 뒤, 공격자가 살아 있고 접촉 반감·회피 조건이 없을 때 출력된다. 유폭 피해는 이 경로에서 공격자 최대 HP의 1/4이다.
- 같은 `BattleScript_AftermathDmg`를 `최후의발악(Innards Out)`도 사용하므로, 해당 특성의 반격 피해 뒤에도 같은 문자열이 출력될 수 있다. 스크립트는 먼저 특성 팝업을 표시한 뒤 HP 갱신, 이 문자열 출력, 추가 기절 판정을 수행한다.
- 현재 HNS 테이블의 문구는 영어 `{B_ATK_NAME_WITH_PREFIX} was hurt!`로 남아 있다. 이번 확인에서는 문구를 수정하지 않았다.

## 2026-09-21 — 능력치 단계 문구 앞 공백 복원 및 HNS ROM 재빌드

- 앞선 수정에서 `{B_BUFF2}` 앞의 리터럴 공백까지 제거해 `공격이`와 `크게`가 붙는 회귀가 생겼다. `{B_BUFF2}`의 `크게 `·`매우 크게 `는 뒤쪽 공백만 제공하므로, 관련 상승·하락 및 도구·특성 문구에 `{B_TXT_IGA} {B_BUFF2}` 형태의 앞 공백을 복원했다.
- `make BUILD=hns build/hns/src/battle_message.o -j2`와 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8`가 모두 종료 코드 0으로 완료됐다. 사용량은 EWRAM 249,012/262,144, IWRAM 25,704/32,768, ROM 33,328,404/33,554,432(99.33%)이다.
- 새 `pokehns.gba` SHA-256: `bd151d458f172d3823a19f658d8945ec6b33b99816b32640ee3f44b91bedbb91`. mGBA 실행 파일이 없어 실제 화면 확인은 아직 하지 않았다.

## 2026-09-21 — 배틀 메시지 유사 글리프 충돌 정적 점검

- `src/battle_message.c` 전체에서 능력치 문구와 같은 `CHAR_NBSP(0x39)`·한글 글리프 선행 바이트 충돌을 다시 점검했다. `CHAR_NBSP` 생성 지점은 플레이스홀더 복사 공통 경로 하나뿐이며, 해당 경로에서 뒤 문자가 한글 글리프이면 일반 공백을 유지하므로 특정 문구 외에도 같은 유형의 충돌을 방지한다.
- 번역 문자열에 하드코딩된 `CHAR_NBSP`/`0x39`와 `{B_TXT_IGA} {B_BUFF2}` 패턴은 남아 있지 않다. `include/battle_message.h`의 `B_TXT_I 0x39`는 텍스트 바이트가 아닌 플레이스홀더 ID라서 화면 글리프 충돌과 무관하다.
- 일반 소스 리터럴 공백이 포함된 다른 번역 문자열은 플레이스홀더 복사 버퍼의 공백 변환을 거치지 않으므로 이번 문제와 같은 원인은 확인되지 않았다. `git diff --check` 통과. 이 점검은 정적 확인이며 mGBA 실행 파일 부재로 실제 화면 검증은 별도다.

## 2026-09-21 — 능력치 상승 문구 글리프 깨짐 수정 및 HNS ROM 재빌드

- 원인은 `{B_BUFF2}`의 `크게 ` 뒤 공백이 `CHAR_NBSP(0x39)`로 변환되는 과정이었다. `0x39`는 한글 글리프의 첫 바이트 범위에도 포함되므로, 뒤따르는 `올(0x3D 0x0B)`의 첫 바이트와 합쳐져 `0x393D`(`드`)로 잘못 출력되고 다음 바이트도 어긋났다.
- `src/battle_message.c`의 플레이스홀더 복사에서 다음 문자가 한글 글리프의 첫 바이트이거나 해당 공백이 플레이스홀더의 끝 공백이면 `CHAR_SPACE`를 유지하도록 수정했다. 능력치 상승·하락 문구에서 `{B_BUFF2}` 앞의 중복 리터럴 공백도 제거해 `공격이 크게 올라갔다!`처럼 한 칸만 출력되도록 했다.
- `git diff --check -- src/battle_message.c`와 `make BUILD=hns build/hns/src/battle_message.o -j2` 통과. 이어서 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8`가 종료 코드 0으로 완료됐다. EWRAM 249,012/262,144, IWRAM 25,704/32,768, ROM 33,328,388/33,554,432(99.33%).
- 산출물: `pokehns.gba` SHA-256 `4acef6dc44d13ee39829fbdee023dbfbb2d09b7b414294e8b6296323c334d59a`, `pokehns.elf` SHA-256 `409da26ae9412e0be682dee5d4ed76a80986ef7128345836658e5bf2b2a7319e`.
- mGBA 실행 파일이 없어 실제 화면은 확인하지 않았다. 새 ROM에서 약점갑옷 등 2단계 능력치 상승을 재현해 `크게 올라갔다!`의 모든 글자가 정상인지 확인한다.

## 2026-09-21 — 능력치 상승 문구 수정 후 HNS ROM 재빌드

- 사용자의 최신 `src/battle_message.c` 상태를 기준으로 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8`를 다시 실행했다. 빌드와 링크, `gbafix` 처리가 모두 종료 코드 0으로 완료됐다.
- 산출물은 `pokehns.gba`(33,554,432바이트, SHA-256 `eeeadd734595ba180fe8413b9e8c3fad0bf56872214107ff7af3a94e189be97b`)와 `pokehns.elf`(SHA-256 `74a2658080f7be4d5f32f701b59a2bd503118d0cc98d8bd6c3917d288b2f82ac`)이다. 링크 사용량은 EWRAM 249,012/262,144, IWRAM 25,704/32,768, ROM 33,328,628/33,554,432(99.33%)이다.
- 이번 단계에서는 소스나 그래픽을 추가로 수정하지 않았다. mGBA 실행 파일이 없어 실제 전투 화면의 `크게 올라갔다!` 글리프 출력은 아직 확인하지 않았다. 새 ROM으로 같은 전투를 재현해 화면을 확인한다.

## 2026-09-21 — 능력치 상승 조사 수정 확인

- `STRINGID_TARGETABILITYSTATRAISE`와 `STRINGID_ATTACKERABILITYSTATRAISE`가 `{B_BUFF1}{B_TXT_IGA} {B_BUFF2}올라갔다!` 형태로 수정됐다. 이제 `공격이`, `방어가`, `스피드가`처럼 능력치명에 맞는 조사가 선택되고 `{B_BUFF2}` 뒤 중복 공백도 없다.
- `{B_BUFF1}가` 고정 패턴은 `src/battle_message.c`에서 더 이상 발견되지 않았다.
- `git diff --check -- src/battle_message.c`와 `make BUILD=hns build/hns/src/battle_message.o -j2` 통과. 전체 HNS ROM은 재링크하지 않았다.

## 2026-09-21 — 능력치 상승·하락 배틀 메시지 공백 검토

- 사용자가 수정한 `src/battle_message.c`의 한국어 능력치 상승·하락 문구를 확인했다. `gText_StatSharply`·`STRINGID_STATHARSHLY`가 단계 문자열 끝에 공백을 포함하는 구조를 유지하면서, 각 본문에서 `{B_BUFF2}` 뒤의 중복 공백은 제거되어 있다.
- `gText_DefendersStatRose`, `STRINGID_ATTACKERSSTATROSE`, `STRINGID_SCRIPTINGSTATROSE`, `STRINGID_ATTACKERSSTATFELL`, `STRINGID_DEFENDERSSTATFELL`, 아이템에 의한 상승·하락 문구는 `{B_TXT_IGA}`와 `{B_BUFF2}올라갔다!/떨어졌다!` 조합으로 정상이다.
- `STRINGID_TARGETABILITYSTATRAISE`와 `STRINGID_ATTACKERABILITYSTATRAISE`도 공백은 정상이나 `{B_BUFF1}가`가 고정되어 있다. 약점갑옷의 스피드 상승에는 맞지만, 공격·특수공격 상승에서는 `공격가`가 될 수 있으므로 후속으로 `{B_BUFF1}{B_TXT_IGA}`로 바꿀 필요가 있다. 이번 확인에서는 사용자의 문구를 수정하지 않았다.
- `git diff --check -- src/battle_message.c`와 `make BUILD=hns build/hns/src/battle_message.o -j2` 통과. 전체 HNS ROM은 재링크하지 않았다.

## 2026-09-21 — 일격기 성공 메시지 연결 및 HNS 빌드

- `src/battle_util.c`의 `CalculateMoveDamage()`에서 실제 배틀(`ctx->updateFlags`)의 OHKO 피해가 대상의 현재 HP와 같을 때 `MOVE_RESULT_ONE_HIT_KO`를 설정하도록 보완했다. 성공 전에 남아 있던 `MOVE_RESULT_ONE_HIT_KO_NO_AFFECT` 표시는 제거해 기존 결과 메시지 분기에서 `BattleScript_OneHitKOMsg`가 선택되도록 했다.
- 가위자르기·뿔드릴·땅가르기·절대영도는 모두 같은 `EFFECT_OHKO` 경로를 사용하므로 네 기술에 `STRINGID_ONEHITKO`가 다시 연결된다. `STRINGID_ONEHITKO`의 문구는 기존 `일격필살!`을 유지했다.
- Endure·Sturdy·Focus Band/Sash·친밀도 생존은 피해가 대상 HP보다 1 작아 성공 플래그가 설정되지 않는다. 다이맥스 대상·공격자보다 레벨이 높은 대상·Sturdy 차단은 `DoesOHKOMoveMissTarget()`의 기존 실패/특성 플래그와 메시지 분기를 그대로 사용한다.
- `git diff --check -- src/battle_util.c`와 `make BUILD=hns build/hns/src/battle_util.o -j2` 통과. 이어서 표준 명령 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8`가 종료 코드 0으로 완료됐다. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,328,612/33,554,432(99.33%). `pokehns.elf` SHA-256 `2054a1c1466fe95db8655e7a34b64814e213c9342bb23ee192b0eca63e4ccf26`, `pokehns.gba` SHA-256 `16396927c4fbee75aa2fbf59a5cb8e1be0e7de2f23d8b4eba058d6f31d7d5f53`.
- mGBA 실행 파일이 없어 실제 전투 화면은 확인하지 않았다. 새 ROM에서 네 일격기 성공 시 `일격필살!`이 나오고, Sturdy·레벨 차이·다이맥스·Endure/기합의띠 생존 시 기존 메시지가 나오는지 확인한다.

## 2026-09-21 — 일격기 성공 메시지 연결 확인

- 가위자르기·뿔드릴·땅가르기·절대영도는 모두 `EFFECT_OHKO`를 사용한다. `src/battle_message.c:390`에는 `STRINGID_ONEHITKO`가 `일격필살!`로 정의되어 있고, `data/battle_scripts_1.s:5377-5380`의 `BattleScript_OneHitKOMsg`가 이 문구를 출력한다.
- 그러나 현재 HNS 코드에서 성공한 OHKO에 `MOVE_RESULT_ONE_HIT_KO`를 설정하는 호출은 없다. `src/battle_script_commands.c:2125-2131`은 해당 플래그가 있을 때만 `BattleScript_OneHitKOMsg`를 호출하고, `src/battle_util.c:7930-7932`의 OHKO 피해 계산은 대상 HP만 피해량으로 설정한다. 따라서 현재 소스 기준으로는 성공 시 `일격필살!`이 별도로 출력되지 않는다.
- `src/battle_util.c:10710-10713`은 OHKO 명중 시 `MOVE_RESULT_ONE_HIT_KO_NO_AFFECT`를 남긴다. 이 비트는 현재 `Cmd_resultmessage()`의 별도 `case`가 없어, 성공 후 결과 문자열이 선택되지 않고 넘어갈 가능성이 있다. 레벨이 낮거나 다이맥스·특성 `Sturdy`로 막힌 경우는 `DoesOHKOMoveMissTarget()`의 실패/방어 분기를 따른다.
- 이 누락은 HNS가 원래 메시지를 삭제한 흔적이 아니라 upstream 확장팩의 `6c05a08750a`(`Refactor OHKO Moves (#8916)`)에서 기존 `Cmd_tryKO`를 제거하고 OHKO 판정·피해 계산을 분리하는 과정에서 성공 플래그 설정을 옮기지 못한 것으로 보인다. 리팩터링 전 `Cmd_tryKO`에는 대상 HP를 피해량으로 설정한 직후 `MOVE_RESULT_ONE_HIT_KO`를 설정하는 코드가 있었다. 현재 HNS에는 이후 이를 복구한 코드가 없다.
- 이번 확인에서는 코드를 수정하거나 빌드하지 않았다. mGBA를 실행할 수 없어 실제 화면은 확인하지 않았으며, 후속 수정 시 명중 성공·빗나감·Sturdy·Focus Sash/Endure 각 결과 메시지를 함께 검증해야 한다.

## 2026-09-21 — 포이즌힐·솔라파워·아이스바디 특성 팝업 전용 출력 적용

- 최신 포켓몬 게임처럼 세 특성의 별도 회복/피해 텍스트는 출력하지 않고 특성 팝업만 표시하도록 `data/battle_scripts_1.s`의 `BattleScript_PoisonHealActivates`, `BattleScript_SolarPowerActivates`, `BattleScript_IceBodyHeal`에서 해당 `printstring`·`waitmessage` 두 줄씩 제거했다.
- 특성 팝업 호출, 포이즌힐 회복, 아이스바디 회복 애니메이션·HP 갱신, 솔라파워/건조피부 피해·기절 판정은 유지했다. 문자열 ID와 테이블 정의는 삭제하지 않아 다른 참조가 생겨도 상수 호환성을 보존했다.
- 정적 확인: 세 ID의 직접 `printstring` 호출이 `data/battle_scripts_1.s`에서 사라졌고, 각 스크립트의 팝업·HP 처리 명령은 남아 있다. `git diff --check -- data/battle_scripts_1.s` 통과.
- `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,328,468/33,554,432(99.33%). `pokehns.gba` SHA-256 `f865817ed5136dd5f688dfad89439bbed0be52b46e375017eaf53307bd858793`, `pokehns.elf` SHA-256 `02b5734b02118e3d4796f3aa2d32d5b521cef2695d6d1701e670742ec9fea790`; 로그 `build/localization-logs/hns-passive-ability-popup-only-20260921.log`.
- mGBA 실행 파일이 없어 실제 화면은 확인하지 않았다. 새 ROM에서 특성 팝업 뒤 텍스트 창 없이 HP 바·회복 애니메이션·기절 판정으로 바로 넘어가는지 확인한다.

## 2026-09-21 — 포이즌힐·솔라파워·아이스바디 회복/피해 메시지 호출 확인

- 세 메시지 모두 현재 HNS에서 실제 호출된다. `STRINGID_POISONHEALHPUP`은 독/맹독 상태의 포이즌힐이 턴 종료에 HP 1/8을 회복할 때, `STRINGID_SOLARPOWERHPDROP`은 햇빛 아래 솔라파워가 HP 1/8 피해를 받을 때 출력된다. 후자는 현재 공통 경로로 햇빛 아래 건조피부 피해에도 사용된다.
- `STRINGID_ICEBODYHPGAIN`도 아이스바디가 얼음 날씨에서 턴 종료에 HP 1/16을 회복할 때 출력된다. HP가 가득 찼거나 회복봉인·지하/수중 반무적 상태이면 호출되지 않는다.
- 세 스크립트 모두 먼저 `BattleScript_AbilityPopUp`을 호출한 뒤 회복/피해 애니메이션과 문자열을 표시한다. 정의의 “더 이상 표시되지 않는 것 같다” 주석은 현재 HNS 호출 경로와 맞지 않는다. 이번 확인에서는 주석·문자열·코드를 수정하지 않았고 빌드도 실행하지 않았다.
- 확인 위치: `src/battle_end_turn.c:484-489`, `src/battle_util.c:3675-3690`, `src/battle_util.c:3792-3798`, `data/battle_scripts_1.s:4484-4490`, `data/battle_scripts_1.s:5678-5685`, `data/battle_scripts_1.s:6191-6197`.

## 2026-09-21 — `TARGETABILITYSTATRAISE` 한글 문구의 플레이스홀더·공백 확인

- 현재 문구는 약점갑옷의 스피드 상승에서 `{B_DEF_NAME_WITH_PREFIX}`=야생/상대 포켓몬 이름, `{B_BUFF1}`=`스피드`, `{B_BUFF2}`=`크게 `로 확장된다. HNS의 `B_WEAK_ARMOR_SPEED`가 최신 세대 설정이어서 2단계 상승이기 때문이다.
- 사용자가 넣은 `"{B_BUFF1}가 {B_BUFF2} 올라갔다!"`는 `{B_BUFF2}` 자체의 뒤 공백과 문자열의 앞뒤 공백이 겹쳐 `스피드가 크게  올라갔다!`처럼 공백이 두 칸이 된다. 권장 형태는 `"{B_BUFF1}{B_TXT_IGA} {B_BUFF2}올라갔다!"`이며, 이번 확인에서는 소스 문구를 수정하지 않았다.
- Gen6 설정처럼 1단계 상승이면 `{B_BUFF2}`가 빈 문자열이므로 현재 문구는 `스피드가  올라갔다!`가 된다. 권장 형태는 `스피드가 올라갔다!`로 출력된다.
- 검증: `src/battle_script_commands.c`의 능력치 버퍼 생성, `src/battle_message.c`의 통계명·상승 정도 문자열, `data/battle_scripts_1.s:6805-6815`의 호출부를 정적으로 확인했다. 빌드는 실행하지 않았다.

## 2026-09-21 — 깨어진갑옷·분노의경혈·불굴의마음 특성 팝업 확인

- 세 특성 모두 효과 문구와 별도로 `BattleScript_AbilityPopUp`을 호출한다. 깨어진갑옷은 물리 기술 피격 후 방어/스피드 처리 전에, 분노의경혈은 급소 피격 후 공격 최대화 처리 전에, 불굴의마음은 플린치 후 스피드 상승 처리 전에 능력 팝업을 호출한다.
- 따라서 `STRINGID_TARGETABILITYSTATRAISE`, `STRINGID_TARGETSSTATWASMAXEDOUT`, `STRINGID_ATTACKERABILITYSTATRAISE`는 팝업 자체가 아니라 팝업 뒤에 표시되는 효과 문구다. 이번 확인에서는 소스·문자열·ROM을 수정하지 않았고 빌드도 실행하지 않았다.
- 확인 위치: `data/battle_scripts_1.s:5744`, `data/battle_scripts_1.s:6755`, `data/battle_scripts_1.s:6784` 및 `src/battle_util.c:4022`.

## 2026-09-21 — 전투 특성 메시지 11개 출력 조건 조사

- `src/battle_message.c:596-606`의 요청된 11개 ID를 배틀 스크립트와 특성 효과 코드에서 대조했다. 소스 수정과 빌드는 하지 않았다.
- `STRINGID_TARGETABILITYSTATRAISE`는 현재 HNS에서 약점갑옷이 물리 기술을 맞고 스피드를 올릴 때만 직접 출력된다. 저스티파이드·주눅·물굳히기·지구력·스팀엔진·열교환 등의 대상 능력치 상승은 `BattleScript_TargetAbilityStatRaiseRet` 뒤의 일반 능력치 상승 문자열을 사용한다.
- `STRINGID_TARGETSSTATWASMAXEDOUT`은 급소에 맞은 분노의경혈 포켓몬의 공격이 아직 최대가 아닐 때 공격을 최대 랭크로 올리며 출력된다. `STRINGID_ATTACKERABILITYSTATRAISE`는 기절 후 의기양양의 스피드 상승, 교대 시 다운로드의 공격/특수공격 상승, 턴 종료 시 가속의 스피드 상승, 포켓몬이 기절한 뒤 소울하트의 특수공격 상승에 공통으로 사용된다.
- `STRINGID_POISONHEALHPUP`는 독/맹독 상태의 포이즌힐 포켓몬이 HP가 가득 차지 않았고 회복봉인이 없을 때 턴 종료에 1/8 회복하며 출력된다. `STRINGID_BADDREAMSDMG`는 나쁜꿈 특성이 턴 종료에 아군이 아닌 수면·컴어토즈 대상에게 1/8 피해를 줄 때 대상마다 출력된다(매직가드 제외).
- `STRINGID_MOLDBREAKERENTERS`, `STRINGID_TERAVOLTENTERS`, `STRINGID_TURBOBLAZEENTERS`, `STRINGID_SLOWSTARTENTERS`는 해당 특성 포켓몬이 배틀에 나올 때 출력된다. `STRINGID_SLOWSTARTEND`는 슬로스타트 5턴 타이머가 끝날 때 출력된다. `STRINGID_SOLARPOWERHPDROP`는 햇빛 아래 솔라파워의 턴 종료 피해에 사용되며, 현재 공통 경로 때문에 햇빛 아래 건조피부의 피해에도 같은 ID가 사용된다.
- `POISONHEALHPUP`·`SOLARPOWERHPDROP` 정의의 “더 이상 표시되지 않는 것 같다” 주석은 현재 호출 경로와 맞지 않는다. 이번 조사에서는 주석이나 문자열을 수정하지 않았다.
- 검증: 각 `printstring` 호출, `gSwitchInAbilityStringIds`, `AbilityBattleEffects()`의 `ON_SWITCHIN`·`ENDTURN`·`MOVE_END` 분기를 정적으로 확인했다. 빌드는 실행하지 않았다.
- 다음 시작점: 실제 메시지 문구를 한글화할 때 약점갑옷·분노의경혈·포이즌힐·나쁜꿈·솔라파워/건조피부·슬로스타트 상황을 각각 재현하고 줄바꿈과 조사 연결을 확인한다.

## 2026-09-20 — 도구 드롭 메시지 한글 문구 수정 후 재빌드

- 사용자가 수정한 `STRINGID_WILDPKMNDROPPEDITEM`과 `STRINGID_DROPPEDITEMBAGFULL` 문구를 그대로 유지했다. 첫 문구는 `{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}`와 `{B_LAST_ITEM}{B_TXT_EULREUL}`을 사용하고, 가방 가득 참 문구는 `가방이 가득 차서\n아이템을 주울 수 없습니다!`를 출력한다.
- `git diff --check -- src/battle_message.c src/battle_script_commands.c` 통과. `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,328,484/33,554,432(99.33%). `pokehns.gba` SHA-256 `9a37bb06b23c1b5ad00cfc962d9bfadac64e276604d5df0456b68c1497eb7472`, `pokehns.elf` SHA-256 `844123625189a1c37782dc87c35b0d9fd2023a1cb96186ee4decf941f174c0d9`; 로그 `build/localization-logs/hns-drop-item-message-ko-20260920.log`.
- mGBA 실행 파일이 없어 실제 메시지 화면은 확인하지 않았다. 새 ROM에서 도구 획득 성공 시 포켓몬 이름·조사·아이템명이 자연스럽게 연결되는지, 가방 가득 참 분기의 줄바꿈을 확인한다.

## 2026-09-20 — 야생 포켓몬 도구 드롭 메시지에 실제 포켓몬 이름 연결

- 야생 포켓몬 도구 드롭 루틴에서 실제로 도구를 떨어뜨린 배틀러를 `gBattleScripting.battler`에 저장하도록 `BS_TryGiveDroppedItems()`를 보완했다. 더블 배틀에서도 선택된 배틀러가 메시지에 사용된다.
- `STRINGID_WILDPKMNDROPPEDITEM`과 `STRINGID_DROPPEDITEMBAGFULL`은 `{B_SCR_NAME_WITH_PREFIX}`를 사용하도록 바꿨다. 따라서 기존의 고정 문구 대신 `야생 <포켓몬 이름>` 접두사가 붙은 실제 이름이 표시된다. 도구 획득 성공·가방 가득 참 분기와 `B_LAST_ITEM`은 그대로 유지했다.
- 수정 위치: `src/battle_script_commands.c:15178-15180`, `src/battle_message.c:889-890`.
- `git diff --check -- src/battle_message.c src/battle_script_commands.c` 통과. `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,328,468/33,554,432(99.33%). `pokehns.gba` SHA-256 `5ffe014bb97113a7fa1632b036a409747cf036e1f580719be776607e537ea62d`, `pokehns.elf` SHA-256 `790ebc6a38ad319bcdc28525ad03a2ae9ec09863f51a21d9f531c3b895ad1277`; 로그 `build/localization-logs/hns-drop-item-name-20260920.log`.
- mGBA 실행 파일이 없어 실제 야생 배틀·더블 배틀 화면은 확인하지 않았다. 새 ROM에서 도구 떨어뜨리기 옵션을 켜고 도구 획득 성공 및 가방 가득 참 상황에서 이름·접두사·줄바꿈을 확인한다.

## 2026-09-20 — Pokémon Champions 효과 단계별 출력 및 아이템 팝업 HNS 별도 이식

- PR #9777의 전투 로직 중 HNS 구조에 맞춰 효과 단계별 메시지와 아이템 팝업을 별도로 이식했다. `MOVE_RESULT_EXTREMELY_EFFECTIVE`/`MOVE_RESULT_MOSTLY_INEFFECTIVE`를 추가하고 피해 배율이 2배 초과·0.5배 미만일 때 설정하도록 했다. 결과 플래그 저장·스크립트 인수·AI/TV 경로를 32비트로 확장하고, 배틀 스크립트의 플래그 매크로도 `.4byte`로 맞췄다.
- 효과음·결과 메시지·더블 배틀 대상별 메시지·방어 측 크리티컬 메시지를 새 ID에 연결했다. 기존 HNS 한글 문구는 유지하고, 새 단계 및 단일 대상 전용 메시지는 PR에서 추가한 문자열을 사용한다. Weakness Policy·Enigma Berry의 판정도 새 high-effectiveness 플래그를 인식한다. 기존 후속 스크립트의 플래그 초기화 지점에는 새 단계 플래그를 함께 추가했다.
- 아이템 팝업은 HNS의 능력 팝업 시트·팔레트·태그 구조(`TAG_ABILITY_POP_UP_PLAYER1 + battler`)를 재사용한다. `PrintItemOnItemPopUp()`이 HNS `GetItemName()`을 사용하므로 한글 아이템명이 기존 UI 리소스에 출력된다. `BS_ShowItemPopup`/`BS_DestroyItemPopup` 네이티브와 스크립트 helper를 추가하고 Power Herb, Destiny Knot, Toxic/Flame Orb, Ability Shield, Rocky Helmet, Berry, Gem, White Herb, Life Orb, Focus Sash, Quick Claw, Red Card, Eject Button 등 HNS에 대응하는 아이템 발동 지점에 연결했다.
- HNS의 기존 한글 다중 타격 문구(`번 맞았다!`)는 바꾸지 않았고, PR의 동적 복수형 로직은 이식하지 않았다. HNS에 구조가 다른 아이템 스크립트는 무리하게 upstream 형태로 덮어쓰지 않았다.
- `git diff --check` 통과. `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,328,596/33,554,432(99.33%). `pokehns.elf` SHA-256 `452a83800d5f92a550f2c02d79ab11a76af650e05eb2bf762dd2cec6ef7320af`, `pokehns.gba` SHA-256 `dfa6f24789e7fe649d90221dc65ee5202ead1e283c0e08d83b336e1c3acfd8db`; 로그 `build/localization-logs/hns-champions-effects-item-popup-final-20260920.log`.
- 코드·링크 검증은 완료했지만 mGBA를 실행할 수 없어 실제 전투 화면의 단계별 문구, 팝업 위치, 한글 아이템명, 표시 시간은 확인하지 않았다. 다음 시작점은 새 ROM에서 각 아이템 발동과 0.5배·2배 초과 상성 사례를 실행하고, 남은 HNS 전용 아이템 스크립트의 팝업 적용 범위를 검토하는 것이다.

## 2026-09-19 — Pokémon Champions 배틀 메시지 PR #9777 확인 및 신규 문자열 이식

- upstream [PR #9777](https://github.com/rh-hideout/pokeemerald-expansion/pull/9777)의 커밋 `63631f951f`를 확인했다. 현재 HNS `HEAD`에는 이 커밋이 조상으로 포함되어 있지 않으며, HNS의 `src/battle_message.c`는 한글화·기존 사용자 변경으로 upstream과 크게 다르다.
- PR에서 새로 추가한 문자열 ID 11개를 `include/constants/battle_string_ids.h`와 `src/battle_message.c`에 원문 그대로 추가했다: `STRINGID_MOSTLYINEFFECTIVE`, `STRINGID_EXTREMELYEFFECTIVE`, `STRINGID_NOTVERYEFFECTIVEONDEF`, `STRINGID_SUPEREFFECTIVEONDEF`, `STRINGID_MOSTLYINEFFECTIVEONDEF`, `STRINGID_EXTREMELYEFFECTIVEONDEF`, `STRINGID_EXTREMELYEFFECTIVETWOFOES`, `STRINGID_MOSTLYINEFFECTIVETWOFOES`, `STRINGID_CRITICALHITONDEF`, `STRINGID_S`, `STRINGID_LOSTSOMEOFITSHP`. 기존 HNS에 이미 있던 `STRINGID_ITDOESNTAFFECTTWOFOES`와 문구는 그대로 유지했다.
- 사용자가 요청한 대로 기존 문구는 수정하지 않았다. PR에서 수정이 필요한 upstream ID는 다음과 같다. `gText_PkmnShroudedInMist`; `STRINGID_HITXTIMES`; `STRINGID_PKMNRAISEDSPDEF`; `STRINGID_PKMNRAISEDDEF`; `STRINGID_PKMNAURORAVEIL`; `STRINGID_PKMNCOVEREDBYVEIL`; `STRINGID_PKMNSAFEGUARDEXPIRED`; `STRINGID_SPIKESSCATTERED`; `STRINGID_PKMNSXWOREOFF`; `STRINGID_TAILWINDBLEW`; `STRINGID_SHIELDEDFROMCRITICALHITS`; `STRINGID_POISONSPIKESSCATTERED`; `STRINGID_POINTEDSTONESFLOAT`; `STRINGID_PROTECTEDTEAM`; `STRINGID_TAILWINDENDS`; `STRINGID_LUCKYCHANTENDS`; `STRINGID_FRISKACTIVATES`; `STRINGID_UNNERVEENTERS`; `STRINGID_TOXICSPIKESABSORBED`; `STRINGID_STICKYWEBUSED`; `STRINGID_BELCHCANTSELECT`; `STRINGID_BERRYDMGREDUCES`; `STRINGID_NOONEWILLBEABLETORUNAWAY`; `STRINGID_SPIKESDISAPPEAREDFROMTEAM`; `STRINGID_TOXICSPIKESDISAPPEAREDFROMTEAM`; `STRINGID_STICKYWEBDISAPPEAREDFROMTEAM`; `STRINGID_STEALTHROCKDISAPPEAREDFROMTEAM`; `STRINGID_COULDNTFULLYPROTECT`; `STRINGID_ARAINBOWAPPEAREDONSIDE`; `STRINGID_THERAINBOWDISAPPEARED`; `STRINGID_SEAOFFIREENVELOPEDSIDE`; `STRINGID_THESEAOFFIREDISAPPEARED`; `STRINGID_SWAMPENVELOPEDSIDE`; `STRINGID_THESWAMPDISAPPEARED`; `STRINGID_NOTVERYEFFECTIVETWOFOES`; `STRINGID_REFLECTWOREOFF`; `STRINGID_LIGHTSCREENWOREOFF`; `STRINGID_AURORAVEILWOREOFF`. 주요 변경은 `team`→`side`, Safeguard/독압정/끈적끈적네트 문구 정리, 단일 대상 보호 문구의 버퍼 변경, `HITXTIMES`의 단복수 처리, 그리고 일부 문장 전체 교체다. 이 중 `STRINGID_REFLECTWOREOFF`, `STRINGID_LIGHTSCREENWOREOFF`, `STRINGID_AURORAVEILWOREOFF`는 현재 HNS enum·테이블에 항목 자체가 없으므로 관련 화면 해제 로직과 함께 후속 추가가 필요하다. PR은 `STRINGID_STICKYWEBDISAPPEAREDFROMYOU`를 삭제하고 `STRINGID_AFTERMATHDMG`를 `STRINGID_PKMNWASHURT`로 이름 변경하므로, 이 두 ID도 현 HNS에서는 유지했다.
- PR의 메시지를 실제로 선택·출력하는 전투 로직(동적 복수형, 효과 단계별 메시지 선택, 아이템 팝업, 일부 배틀 스크립트·화면 동작)은 기존 HNS 코드와 차이가 커 이번 작업에서는 이식하지 않았다. 따라서 추가한 문자열은 ID·테이블 준비 상태이며, 해당 기능을 활성화하려면 후속 로직 이식과 한글 문구 검토가 필요하다.
- `git diff --check` 통과. `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,326,996/33,554,432(99.32%). `pokehns.elf` SHA-256 `a882a342ff825ffae766b9904aa1f81d5ecc002f14cec142a422bd639bf7ef12`, `pokehns.gba` SHA-256 `683731e1febab570a87eb2c0e84862e5fe60cfc654ed7cf2be151a34f36c9722`; 로그 `build/localization-logs/hns-battle-messages-20260919.log`.
- 코드·빌드 검증은 완료했지만 mGBA에서 전투 메시지 화면은 확인하지 않았다. 후속 작업에서는 위 기존 문구의 한글 대응을 정한 뒤 동적 복수형·효과 단계별 출력 로직을 별도로 이식한다.

## 2026-09-19 — Scarlet/Violet 도감 스키핑 기능 HNS 이식 및 빌드 검증

- upstream PR [#9797](https://github.com/rh-hideout/pokeemerald-expansion/pull/9797)의 네 가지 도감 공백 처리 모드(`DONT_SKIP_GAPS`, `SKIP_GAPS_EXCEPT_ONE`, `SKIP_GAPS_EXCEPT_BEFORE_AFTER`, `SKIP_ALL_GAPS`)와 `P_SKIP_POKEDEX_GAPS` 설정을 [`include/config/pokemon.h`](../include/config/pokemon.h)에 추가했다. 현재 HNS 설정은 `SKIP_GAPS_EXCEPT_BEFORE_AFTER`이며, 미등록 항목 앞·뒤 한 칸을 남기는 SV 방식이다.
- [`include/pokedex.h`](../include/pokedex.h)에 공용 `ShouldSkipPokedexListEntry()` 선언을 추가하고, [`src/pokedex.c`](../src/pokedex.c)에서 seen 플래그와 앞·뒤 도감 번호를 기준으로 공백을 판정하도록 구현했다. 기본 도감과 [`src/pokedex_plus_hgss.c`](../src/pokedex_plus_hgss.c)의 숫자순 목록 생성부 모두 이 판정을 사용한다. `SKIP_GAPS_EXCEPT_ONE` 모드에서는 남겨 둔 미등록 한 칸을 `------`/`----`으로 표시한다.
- `include/constants/pokedex.h`의 대규모 도감 열거 순서 매크로화는 이 기능에 필수적이지 않아 HNS에 일괄 이식하지 않았다. 기존 HNS의 지역·획득 가능 도감 순서와 배열 크기는 보존했다.
- `git diff --check` 통과. `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,326,548/33,554,432(99.32%). `pokehns.elf` SHA-256 `454ebef4e76d7029a50b8947d7cd20867ae8011501bcec276ffae1662b74dbdb`, `pokehns.gba` SHA-256 `9b35731808d956016777d55ebf6ec42d2749ed348e0cc8599a806f11ee00100c`; 로그 `build/localization-logs/hns-pokedex-skipping-20260919.log`.
- 코드·빌드 검증은 완료했지만 mGBA에서 실제 도감 화면은 확인하지 않았다. 새 ROM에서 숫자순 도감의 미등록 구간이 앞·뒤 한 칸만 남기고 건너뛰는지, 지역 도감과 HGSS 도감 양쪽의 스크롤·번호·이름이 어긋나지 않는지 확인한다.

## 2026-09-19 — Scarlet/Violet 도감 스키핑 기능 이식 여부 확인

- Emerald Expansion PR [#9797](https://github.com/rh-hideout/pokeemerald-expansion/pull/9797)은 `DONT_SKIP_GAPS`, `SKIP_GAPS_EXCEPT_ONE`, `SKIP_GAPS_EXCEPT_BEFORE_AFTER`, `SKIP_ALL_GAPS`와 `P_SKIP_POKEDEX_GAPS` 설정을 추가해 도감의 미등록 구간을 건너뛰는 기능이다. PR 설명은 SV 방식이 `SKIP_GAPS_EXCEPT_BEFORE_AFTER`라고 설명한다.
- 현재 HNS의 [`include/config/pokemon.h`](../include/config/pokemon.h)에는 해당 매크로가 하나도 없다. [`src/pokedex.c:2180`](../src/pokedex.c)와 [`src/pokedex_plus_hgss.c:2480`](../src/pokedex_plus_hgss.c)의 `CreatePokedexList()`에도 `P_SKIP_POKEDEX_GAPS`나 스킵 모드 분기가 없다.
- `git log -S`로 현재 저장소 이력을 확인했지만 해당 식별자나 PR의 변경 커밋은 HNS 이력에 없다. 따라서 예전에 추가했던 것으로 기억한 기능은 현재 HNS 작업 트리에는 이식되어 있지 않다. 도감은 기존 HNS 코드의 목록 생성 로직을 사용한다.
- 이번 작업은 정적 조사만 수행했으며 소스·데이터·ROM은 수정하지 않았고 빌드도 실행하지 않았다. 기능을 추가하려면 기본 도감과 HGSS 도감 양쪽 `CreatePokedexList()`를 함께 포팅하고 `P_SKIP_POKEDEX_GAPS` 기본값을 정해야 한다.

## 2026-09-19 — 파동의방호 설명 문자열 변경 후 재빌드

- `src/data/abilities.h:2484-2488`의 파동의방호 설명을 `접촉 기술로 입는 데미지가 반감된다.`로 변경한 것을 확인했다. 특성 효과 코드와 메가루카리오Z의 특성 지정은 건드리지 않았다.
- `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,326,484/33,554,432(99.32%). `pokehns.elf` SHA-256 `aa0cace7496c3496a46fc59503aeeb5768859c574154e32f1ce6e600796fefd4`, `pokehns.gba` SHA-256 `1f1026ae00fc534039d488f6e3b41f0eacce3774a846540a22a5a732f6a9d954`; 로그 `build/localization-logs/hns-aura-guard-description-20260919.log`.
- `git diff --check` 통과. 설명 변경으로 직전 ROM과 해시가 달라졌으며, mGBA가 없어 실제 특성 팝업 화면과 줄바꿈·잘림은 확인하지 않았다. 새 ROM에서 파동의방호 설명이 정상 출력되는지 확인한다.

## 2026-09-19 — 파동의방호 및 메가루카리오Z 특성 빌드 검증

- `src/battle_util.c:7666-7672`의 `GetDefenderAbilitiesModifier()`에 `ABILITY_AURA_GUARD` 분기를 확인했다. `IsMoveMakingContact()`가 참일 때 피해 배율을 `UQ_4_12(0.5)`로 적용하고 `recordAbility`를 설정한다. 기존 `ABILITY_FLUFFY` 분기는 변경하지 않았다.
- `src/data/pokemon/species_info/gen_4_families.h:4887-4906`의 `SPECIES_LUCARIO_MEGA_Z` 특성 세 슬롯이 모두 `ABILITY_AURA_GUARD`이며, `include/constants/abilities.h`의 값은 319이다. 링크된 `pokehns.gba`에서 종 ID 1559의 `gSpeciesInfo` 특성 슬롯을 읽어 `(319, 319, 319)`로 일치함을 확인했다.
- `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,326,500/33,554,432(99.32%). `pokehns.elf` SHA-256 `4de126b0f7296dc1682e7de352911c40284bfba1160eb2c47f33c37d2405a5ec`, `pokehns.gba` SHA-256 `fc279f72af126010adf17d84c7593a92a2cf4ff6160194c790bc1cc41ac194c5`; 로그 `build/localization-logs/hns-aura-guard-lucario-z-20260919.log`.
- `git diff --check` 통과. 컴파일·링크와 ROM 데이터 대조는 완료했지만, 이 환경에 mGBA 실행 파일이 없어 실제 접촉 기술 피해가 0.5배가 되는 인게임 전투 검증은 하지 않았다.

## 2026-09-19 — 메가 포켓몬 특성 변경 HNS 빌드 검증

- `src/data/pokemon/species_info/gen_1_families.h`부터 `gen_9_families.h`까지의 변경을 확인했다. 메가 포켓몬 47개 엔트리의 `.abilities` 배열이 수정됐다: 1세대 6개, 2세대 3개, 3세대 3개, 4세대 4개, 5세대 7개, 6세대 12개, 7세대 5개, 8세대 1개, 9세대 6개. `FERALIGATR_MEGA`의 두 번째 슬롯처럼 소스에서 `NONE`으로 남겨 둔 슬롯도 그대로 보존됐다.
- 현재 소스의 메가 엔트리 99개를 종 ID와 특성 열거값으로 해석해 HNS ROM의 `gSpeciesInfo`(엔트리 간격 0x10C, 특성 오프셋 0x18)와 대조했다. 99개 전부 소스와 ROM의 세 슬롯 값이 일치했으며, 변경된 47개도 링크된 데이터에 반영됐다. 새 특성 식별자는 빌드에서 모두 해석됐다.
- `make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,326,420/33,554,432(99.32%). `pokehns.elf` SHA-256 `88e9757efbf7eb09c10a879c716ec829f67df1a39e8747cd9c188ae2d0b7f677`, `pokehns.gba` SHA-256 `5a1f3bdc0251340c674669bbedcc2dd71886bd16d230f9a52dc0994d85f85f77`; 로그 `build/localization-logs/hns-mega-abilities-20260919.log`.
- `git diff --check` 통과. 이번 확인은 소스·링크 데이터와 빌드 검증까지이며, mGBA에서 실제 메가진화 배틀을 실행하는 런타임 검증은 하지 않았다. 새 ROM에서 변경 대상 메가 포켓몬의 특성 발동과 특성 팝업을 확인한다.

## 2026-09-19 — 포켓몬 능력창 리본 수 중앙 표시

- 능력치 화면의 리본 수 표시를 `현재 16개` 형식에서 `16개` 형식으로 바꿨다. `src/strings.c:436`의 `gText_RibbonsVar1`에서 `현재`와 `{CLEAR_TO 46}`을 제거하고 `{STR_VAR_1}개`만 남겼다.
- `src/pokemon_summary_screen.c:4061-4078`의 기존 `PrintRibbonCount()`가 리본이 없을 때 `gText_None`(`없음`)을 출력하는 것과 같은 중앙 정렬 경로를 그대로 사용한다. 숫자는 `STR_CONV_MODE_LEFT_ALIGN`으로 변환해 `1개`의 숨은 앞 공백도 포함하지 않도록 했다. 리본이 0개일 때는 기존처럼 `없음`이다.
- `git diff --check` 통과. `make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,326,420/33,554,432(99.32%). `pokehns.gba` SHA-256 `379d9b03a47d2ab53a101e658257770c9c52bfedefb9ead0ab3aa9a3a8f92dc6`; 로그 `build/localization-logs/hns-ribbon-count-center-20260919.log`.
- mGBA 실행 파일이 없어 실제 화면 검증은 하지 않았다. 새 ROM에서 리본 탭의 `1개`, `16개`, 리본 없음의 `없음`이 모두 중앙에 배치되는지 확인한다.

## 2026-09-19 — 계속 화면 수량 단위 및 리본 수 표시 수정

- `src/main_menu.c`의 모험 계속 화면 수량 표시를 수정했다. 포켓몬 도감 수에는 `마리`를 붙이고, 배지 수에는 `개`를 붙였다. 배지는 `STR_CONV_MODE_LEFT_ALIGN`으로 바꿔 `08`이 아닌 `8개`처럼 앞자리 0 없이 출력되게 했다. 포켓몬 도감 수는 기존처럼 잡은 포켓몬 수를 사용하며 `12마리` 형식이다.
- 능력치 화면 리본 수는 [`src/pokemon_summary_screen.c:4061-4078`](../src/pokemon_summary_screen.c)의 `PrintRibbonCount()`가 출력한다. 현재 한글 `gText_RibbonsVar1` 문자열(`src/strings.c:436`)에 이미 `{STR_VAR_1}개` 접미사가 있어 `1개` 형식은 기존 코드에서 충족된다. 한 자리 수의 `STR_CONV_MODE_RIGHT_ALIGN` 공백은 `CLEAR_TO 46` 배치 안에서 3픽셀만 차지하므로 시각적 차이가 작고, 접미사 요구와 무관한 정렬 변경은 남기지 않았다.
- 두 화면의 단위 문자열은 [`src/main_menu.c`](../src/main_menu.c)의 `gText_ContinueMenuPokemonCountUnit`·`gText_ContinueMenuBadgeCountUnit`로 분리했다. 기존 `src/main_menu.c`·`src/pokemon_summary_screen.c`의 실행 비트 변경과 요약 화면 HP·타입 아이콘 수정은 보존했다.
- `git diff --check -- src/main_menu.c src/pokemon_summary_screen.c` 통과. `main_menu.c` 변경 오브젝트를 재컴파일했고, `pokemon_summary_screen.c`의 기존 미사용 함수 경고 외 오류는 없었다. 이어 `make --jobserver-style=pipe hns -j8` 종료 코드 0.
- 빌드 결과: EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,326,436/33,554,432(99.32%). `pokehns.gba` SHA-256 `fa51a7aa16501fc626543dd41e304343f3fe17a5781fdd28fb224e98202e3efd`; 로그 `build/localization-logs/hns-count-units-20260919.log`.
- 코드·빌드 검증은 완료했지만 mGBA 실제 화면 검증은 하지 않았다. 새 ROM에서 모험 계속 화면의 `0마리`, `8개`·`16개`와 포켓몬 능력 화면의 `1개` 리본 표시, 숫자·단위 잘림을 확인한다.

## 2026-09-19 — Pokéblock 메뉴 한글 그래픽 변환 및 팔레트 검증

- 사용자가 수정한 `graphics/pokeblock/menu.png`와 게임용 `menu.4bpp`를 HNS 참조와 대조했다. `src/graphics.c:1858-1859`가 `menu.4bpp.smol`·`menu.gbapal`을 포함하고, `src/pokeblock.c:647-658`이 그래픽·타일맵·팔레트(6×16색)를 실제 메뉴에 로드한다.
- 현재 `menu.png`는 64×40 4-bit indexed PNG(16색 표, 실제 사용 인덱스 0~15 중 14개)이며, 첫 16색이 기존 런타임 `menu.gbapal` 6개 뱅크 중 첫 뱅크와 바이트 단위로 일치한다. 192바이트 `menu.gbapal`은 그대로 보존했다. PNG에서 새 16색 팔레트를 만들면 나머지 다섯 뱅크가 사라질 수 있기 때문이다.
- `menu.4bpp` 1,280바이트는 현재 PNG를 독립 변환한 결과와 일치했다. 기존 압축 파일은 현재 4bpp와 일치하지 않아 `menu.4bpp.smol`을 재생성했으며 432바이트가 됐다. 독립 변환·압축본과 압축 해제본까지 모두 `cmp` 일치한다.
- HNS ELF 심볼도 새 산출물과 일치한다: `gMenuPokeblock_Gfx` 0x1B0, `gMenuPokeblock_Pal` 0xC0, `gMenuPokeblock_Tilemap` 0x98.
- `make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,326,388/33,554,432(99.32%). `pokehns.gba` SHA-256 `8e1cd7347280678ad765720cfc0c03e08e1091df0afe345ce722d3a7718067ac`; 로그 `build/localization-logs/hns-pokeblock-menu-palette-20260919.log`.
- 변환·팔레트·링크 검증은 완료했지만 mGBA 실제 화면은 확인하지 않았다. 새 ROM에서 필드·배틀·포켓블록 급식기에서 포켓블록 메뉴를 열어 한글 글자 색상, 6개 팔레트 뱅크, 선택 강조와 잘림을 확인한다. `src/pokeblock.c`의 실행 비트 변경은 기존 작업 트리 상태이며 내용은 수정하지 않았다.

## 2026-09-19 — Frontier Pass·슬롯머신 한글 그래픽 변환 및 팔레트 검증

- 사용자가 `pokeemerald-kr`에서 가져온 `graphics/frontier_pass/bg.png`, `map_and_card.png`, `graphics/slot_machine/menu.png`를 현재 HNS 경로와 대조했다. 소스 참조는 이미 HNS 경로를 사용하므로 C 코드나 파일명 변경은 필요하지 않았다. `src/graphics.c`는 Frontier Pass의 `bg.4bpp.smol`·`map_and_card.8bpp.smol`을, `src/data/graphics/slot_machine.h`는 슬롯머신 `menu.4bpp.smol`을 포함한다.
- 세 PNG에서 게임용 그래픽을 재생성했다. `bg.4bpp` 16,384바이트 및 `.smol` 4,436바이트, `map_and_card.8bpp` 14,336바이트 및 `.smol` 3,792바이트, `menu.4bpp` 8,704바이트 및 `.smol` 2,696바이트다. 임시 경로에서 같은 입력을 독립 변환·압축한 결과와 여섯 산출물이 모두 바이트 단위로 일치한다.
- 팔레트 검증: `bg.png`와 `menu.png`는 각각 128×256·128×136의 4-bit indexed PNG이며 첫 16색이 기존 런타임 `bg.gbapal`의 첫 뱅크 및 `menu.gbapal`의 첫 뱅크와 각각 일치한다. 런타임 팔레트 파일은 각각 8×16색(256바이트), 5×16색(160바이트)이므로 PNG에서 16색 팔레트를 새로 생성하지 않고 기존 다중 뱅크를 보존했다. `map_and_card.png`는 128×112 8-bit indexed PNG(127색, 최대 인덱스 126)이고 그 팔레트가 Frontier Pass `bg.gbapal`의 앞 127색과 일치한다. 별도 map 팔레트를 추가하지 않았다.
- HNS 심볼 크기도 새 압축 파일과 일치한다: `gFrontierPassBg_Gfx` 0x1154, `gFrontierPassBg_Pal` 0x100, `gFrontierPassMapAndCard_Gfx` 0xED0, `gSlotMachineMenu_Gfx` 0xA88, `gSlotMachineMenu_Pal` 0xA0.
- `make --jobserver-style=pipe hns -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,326,340/33,554,432(99.32%). `pokehns.gba` SHA-256 `626a9bfb9f6e7561f573960153fd90387a1ea2120b31e95063bbe6e88d350b60`. 로그 `build/localization-logs/hns-frontier-slot-ko-20260919.log`.
- 코드·변환·링크 기준 검증은 완료했지만 mGBA에서 실제 화면은 아직 확인하지 않았다. Frontier Pass에서 지도·카드·메달 화면, 슬롯머신에서 메뉴·한글 글자의 색상·잘림·배경 팔레트를 확인한다. `src/data/graphics/slot_machine.h`의 실행 비트 변경은 기존 작업 트리 상태이며 이번에 내용은 수정하지 않았다.

## 2026-09-19 — 트레이너카드 엄마 저금액 `원` 표시

- `src/trainer_card.c`의 HNS `PrintMoneyOnCard()`에서 `Mom_GetBalance()`로 만든 저금액 뒤에 `원`을 붙였다. 표시 형식은 `용돈 3000원/0원`이다. 기존 파일의 다른 미커밋 변경은 보존했다.
- `git diff --check -- src/trainer_card.c` 통과. `make --jobserver-style=pipe BUILD=hns NODEP=1 pokehns.gba -j8` 종료 코드 0. ROM 33,326,516/33,554,432바이트(99.32%), `pokehns.gba` SHA-256 `331b5dd57f57b13393c91047d3428b4feeb869ab3af4826523dc0b57d29dd496`. 로그: `build/localization-logs/hns-trainer-card-mom-won-20260919.log`. 실제 화면은 미확인. 다음 단계: 트레이너카드에서 `3000원/0원`과 큰 저금액의 잘림을 확인한다.

## 2026-09-19 — `graphics/contest` HNS 사용 여부 조사 완료

- HNS 소스는 일반 `graphics/contest/`의 인터페이스·관객·심사위원·어필·다음 차례·결과 화면 그래픽과 타일맵을 `INCBIN`하고, `src/contest.c` 및 `src/contest_util.c`가 해당 심볼을 화면에 로드한다. 콘테스트 화면을 한글화할 때 이 일반 경로와 `results_screen/`이 대상이다.
- `graphics/contest/japanese/`의 합성 그래픽·팔레트·타일맵도 `src/graphics.c`에 `INCBIN`되어 현재 `pokehns.elf`에 심볼이 존재하지만, `src/graphics.c` 외에 `gJPContest*` 참조가 없다. 현재 콘테스트 화면의 표시 경로에서는 사용하지 않는다. 단순히 ROM에 있다는 이유로 표시 자산이라고 판단하지 않는다.
- `.png`·`.pal`과 일부 `.bin`·`.4bpp`는 빌드 입력이며, 런타임에서는 대개 변환된 `.smol`·`.smolTM`·`.gbapal`을 로드한다. `text.gbapal`, 결과 화면 `text_window.4bpp/.gbapal`처럼 비압축 파일을 직접 포함하는 예외도 있다. 파일 삭제나 변환은 하지 않았다.
- 검증은 소스 참조, HNS ELF 심볼, 그래픽 생성 규칙의 정적 조사만 수행했다. 새 빌드 및 실제 게임 화면 확인은 하지 않았다. 다음 단계: 콘테스트 번역이 필요하면 일반 경로의 PNG/PAL과 결과 화면 자산부터 검토하고, 수정 후 `make hns -j8` 및 콘테스트 본편·결과 화면을 확인한다.
- 정정: 위 일반 콘테스트 자산과 맵·스크립트가 HNS ROM에 존재하는 사실은 `make hns`의 일반 진행에서 콘테스트에 입장할 수 있다는 뜻이 아니다. HNS 새 게임은 연두마을 플레이어 집(`src/new_game.c:194`)에서 시작한다. 해안시티의 콘테스트 로비 워프는 공통 호연 `data/maps/LilycoveCity/map.json`에 있고, HNS 지역 맵에서 해안시티·콘테스트 홀로 가는 워프 참조는 확인되지 않았다. 따라서 현재 근거로는 HNS 일반 플레이에 해안시티나 포켓몬 콘테스트가 있다고 말할 수 없다. 전용 `_hns` 콘테스트 맵의 존재도 접근성을 증명하지 않는다. 실제 게임 검증은 미수행.

## 인게임 확인 대기 — Gen4 기술 정보 창 색 인덱스 보정 완료

- 색 차이 원인은 Gen4 UI가 `graphics/battle_interface/gen4/ability_pop_up.gbapal`을 창과 공유하고, 새 PNG에서 사용되는 인덱스 8/D의 색이 이 런타임 팔레트에서 서로 뒤바뀌어 있던 것이다.
- 요청대로 Gen4의 `move_info_window_l.png`·`move_info_window_r.png`에서 픽셀 인덱스 8↔D만 맞바꿨다. 각 이미지에서 513개 픽셀이 바뀌었고, PNG의 팔레트 및 모든 비-IDAT 청크는 그대로다. 기본 UI PNG/4bpp, Gen4 공유 팔레트, 코드 및 그 외 그래픽은 수정하지 않았다. 대상 Gen4 `.4bpp`만 PNG에서 재생성했다.
- 기존 `src/battle_interface.c` 선택 로직은 그대로 두었다. 기본 UI는 기본 폴더, Gen4 설정은 `gen4/` 그래픽을 사용하며, L이 기본이고 `L=A`에서는 R을 표시한다.
- 두 `.4bpp`를 재생성했다(각 512바이트). 이전 산출물에서 8↔D 니블만 바꾼 결과와 새 산출물이 바이트 단위로 일치했고, 컴파일된 `battle_interface.o`의 Gen4 L/R 심볼도 각 `.4bpp`와 일치한다.
- `make BUILD=hns NODEP=1 pokehns.gba -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,326,516/33,554,432(99.32%). `pokehns.gba`: 33,554,432바이트, SHA-256 `fc12a055b5dcb0d631f35c1bfa42ca413599b2f3561271f83831acb67c37b90b`. 로그 `build/localization-logs/hns-gen4-move-info-window-20260919.log`.
- 실제 mGBA 재현은 미수행. 새 ROM의 Gen4 기술 선택 화면에서 L 기본 및 `L=A`일 때 R 그래픽의 두 회색 음영이 의도대로 출력되는지 확인한다.

## 인게임 확인 대기 — 배틀돔 토너먼트 버튼 한글 그래픽 반영

- 사용자가 수정한 `graphics/battle_frontier/tourney_buttons.png`와 팔레트 `tourney_buttons.pal`을 보존하고, 게임이 실제 사용하는 `tourney_buttons.4bpp`·`.gbapal`·`.4bpp.smol`을 재생성했다. 각각 1,536바이트·512바이트·464바이트다.
- 별도 임시 경로에서 같은 PNG·PAL을 다시 변환·압축한 결과와 세 산출물이 모두 바이트 단위로 일치한다. PNG는 32×96의 4-bit indexed 그래픽이며, 코드가 로드하는 OBJ 팔레트의 처음 16색 범위와 호환된다.
- `src/graphics.c`의 `gDomeTourneyTreeButtons_Gfx`·`gDomeTourneyTreeButtons_Pal`이 각각 압축 그래픽과 팔레트를 직접 포함하고, `src/battle_dome.c`가 배틀돔 토너먼트 트리 및 정보 카드에서 이를 로드한다. HNS 그래픽 객체에서 두 심볼의 크기 `0x1D0`·`0x200`을 확인했다.
- HNS 링크가 완료되어 `build/hns/src/graphics.o`(20:34), `pokehns.elf`·`.map`·`.gba`(20:36)가 갱신됐다. `pokehns.gba` SHA-256 `d19195c658b195c3d62403c9e4540f2e127b7fc26da3c1f348a4a73294dee50d`.
- 실제 게임 확인: 배틀돔 토너먼트 트리에서 포켓볼 선택 버튼과 취소·나가기 버튼의 한글 글자, 선택/비선택 프레임, 팔레트·잘림을 확인한다.

## 인게임 확인 대기 — 기술 정보 시트 재수정본 한글화 및 타입 아이콘 팔레트 확인

- 사용자가 다시 수정한 `graphics/interface/menu_info.png`(128×128 indexed PNG)를 `menu_info.4bpp`로 재생성했다. 독립 임시 변환본과 실제 생성물은 바이트 단위로 일치하며 출력 크기는 8,192바이트다.
- 타입 아이콘은 이 시트의 일부이므로 한글 표기로 변경됐다. 실제 가방 TM/HM 정보와 유니언룸 교환 목록이 사용하는 `menu_info2.gbapal`의 16개 색상 순서가 수정 PNG의 첫 16개 팔레트 인덱스와 완전히 일치한다. 따라서 타입별 배경·글자 색은 PNG 미리보기와 동일하게 출력되며, 체력바 상태 아이콘에서 있었던 동적 팔레트 불일치 문제는 없다.
- `menu_info1.gbapal`~`menu_info3.gbapal`은 수정하지 않았다. 이들은 화면별 배경에 맞춘 별도 팔레트이고, `menu_info.png`는 픽셀 인덱스만 제공한다.
- `build/hns/src/graphics.o`와 `pokehns.elf`·`.map`·`.gba`가 2026-09-18 19:42~19:43에 새로 생성돼 최신 4bpp를 링크했다. `pokehns.gba` SHA-256은 `dac51e252c8311f3e45e906cf6f20846bb41334200c6d5dd73f76120afdcfbd5`로 직전과 같다. 이번 PNG 수정이 픽셀 인덱스나 공유 팔레트를 바꾸지 않아 ROM 바이트도 동일한 결과다. 로그 `build/localization-logs/hns-menu-info-ko-20260918.log`.
- 실제 게임 확인: 가방에서 TM/HM을 선택해 `타입/위력/명중/PP` 한글 라벨과 여러 타입 아이콘의 색상을 확인한다. 유니언룸 교환 목록도 가능하면 함께 확인한다.

## 인게임 확인 대기 — 상점 화폐·메뉴 상태 아이콘 한글 그래픽 반영

- 사용자가 수정한 `graphics/shop/money.png`와 `graphics/interface/status_icons.png`를 각각 게임용 `.4bpp`·`.4bpp.smol`로 재생성했다. `status_icons.png`는 추가로 자체 `.gbapal`도 재생성했다.
- `status_icons`는 배틀 체력바의 `battle_interface/*/status*.png`와 달리, `gStatusGfx_Icons`와 `gStatusPal_Icons`를 함께 로드하는 독립 스프라이트다. 따라서 상태별로 런타임에서 한 색상 슬롯만 바꾸는 구조가 아니며, 16개 팔레트 인덱스가 모두 생성된 자체 16색 팔레트와 짝을 이룬다. 이전 화상 아이콘의 파란 배경과 같은 동적 슬롯 불일치는 없다.
- `money`는 자체 팔레트가 아니라 `gShopMenu_Pal`을 쓴다. 수정 PNG에서 변환한 팔레트 인덱스 0~14가 상점 공용 팔레트의 같은 인덱스와 바이트 단위로 일치함을 확인했으므로, 상점 화면에서 색상이 바뀌거나 깨지지 않는다.
- 임시 경로에서 두 PNG를 다시 변환·압축해 실제 생성물 다섯 개(`money.4bpp`, `money.4bpp.smol`, `status_icons.4bpp`, `.gbapal`, `.4bpp.smol`)와 모두 바이트 단위로 일치시켰다. HNS 재빌드 후 `build/hns/src/graphics.o`, `pokehns.elf`, `pokehns.map`, `pokehns.gba`가 갱신됐다. 현재 ROM SHA-256: `dab282490b8aa55e82964d21a8f9203a5213596791823f1063dcde7af7612761`; 로그 `build/localization-logs/hns-status-icons-money-20260918.log`.
- 실제 게임 확인: 상점에서 소지금 라벨, 파티 메뉴·요약 화면에서 독/마비/잠듦/얼음/화상/동상 아이콘을 확인한다.

## 인게임 확인 대기 — HNS·Gen4 배틀 상태 아이콘 팔레트 수정

- `graphics/battle_interface/hns/status.png`~`status4.png`, `graphics/battle_interface/gen4/status.png`~`status4.png`의 8-bit indexed PNG 여덟 개를 수정했다. 한글 글자(인덱스 2·3 및 Gen4의 4)는 보존하고, 게임 변환에서 하위 4비트가 `C` 또는 `F`가 되던 상태 배경 픽셀만 각 배틀러의 동적 상태색 슬롯으로 변경했다.
- 슬롯 규칙은 `status.png`=`C`(배틀러 0), `status2.png`=`D`(배틀러 1), `status3.png`=`E`(배틀러 2), `status4.png`=`F`(배틀러 3)이다. `UpdateStatusIconInHealthbox()`가 이 슬롯에 독·마비·수면·얼음/동상·화상 색을 각각 주입하므로, 이제 기본 UI와 Gen4 UI 모두 해당 상태색을 정상 사용한다. Gen4 화상의 파란 배경 원인인 잘못된 `F` 슬롯 참조도 제거됐다.
- 8개 입력은 모두 24×48 indexed PNG이며 재생성된 `.4bpp`는 각각 576바이트다. 독립 임시 변환본과 실제 생성물의 바이트 단위 비교가 모두 일치했고, 각 시트에 다른 배틀러의 `C/F` 동적 슬롯 참조가 남지 않았음을 확인했다.
- `make hns -j8`로 새 ROM 링크를 완료했다. `pokehns.gba`: 33,554,432바이트, SHA-256 `12759db12bfa1bc70a385967295f73b8ad7a1198f25eb12ff1b5df02f3a5bdb4`; `pokehns.elf`와 `pokehns.map`도 같은 시각에 갱신됐다. 이 환경에서 mGBA 실행은 할 수 없다.
- 다음 확인: 새 ROM에서 기본 UI·4세대 UI의 싱글/더블 배틀에 독·마비·수면·얼음(및 기본 UI 동상)·화상을 각각 걸어, 색상과 한글 글자에 회귀가 없는지 확인한다.

## 인게임 확인 대기 — Gen4 메가진화 트리거 위치를 SoulGold 기준으로 조정

- Gen4 전용 아이콘만 바꾸고 기존 UI 위치 상수를 계속 사용한 탓에 메가 트리거가 SoulGold보다 아래에 표시됐다. SoulGold `src/battle_gimmick.c`의 위치값을 확인해 4세대 UI이면서 메가진화일 때만 별도 좌표 프로필을 사용하도록 수정했다.
- SoulGold 값은 싱글 `xSlide=16, xPriority=36, xOptimal=34, yDiff=-7`, 더블 `16, 36, 34, -3`이다. HNS 기존값은 그대로 별도 보존되어 기존 UI와 Gen4 UI의 다른 기믹에는 적용되지 않는다.
- `CreateGimmickTriggerSprite()`의 최초 생성 위치와 `SpriteCb_GimmickTrigger()`의 등장·퇴장 슬라이드, OAM 우선순위 전환, 체력바 추적 위치가 모두 같은 선택 함수를 사용한다. 따라서 Gen4 메가 트리거는 싱글에서 기존보다 왼쪽 4px·위쪽 4px, 더블에서 왼쪽 4px·위쪽 1px인 SoulGold 위치로 이동한다.
- 출처: [Eemeliri/soulgold `src/battle_gimmick.c`](https://github.com/Eemeliri/soulgold/blob/master/src/battle_gimmick.c), 2026-09-18 확인.
- `git diff --check` 통과. `make hns -j8` 성공: EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,325,684/33,554,432(99.32%). `pokehns.gba`: 33,554,432바이트, SHA-256 `5809c9d4f9d04ede99e76c65229ea48ce7fda529d12a2f4a35f832ccfb71bd07`. 로그 `build/localization-logs/hns-gen4-mega-trigger-position-20260918.log`.
- 실제 mGBA 화면은 이 환경에서 확인하지 못했다. 4세대 UI의 싱글·더블 기술 선택 화면에서 꺼짐/켜짐 아이콘의 위치와 슬라이드가 자연스러운지 확인한다.

## 인게임 확인 대기 — Gen4 배틀 UI 전용 메가진화 트리거 적용

- `src/data/graphics/gimmicks.h`에 `graphics/battle_interface/gen4/mega_trigger.4bpp`와 `.gbapal`을 사용하는 Gen4 전용 스프라이트 시트·팔레트를 추가했다. 사용자가 추가한 원본 `mega_trigger.png`·`.pal`은 수정하지 않았다.
- `src/battle_gimmick.c`의 `CreateGimmickTriggerSprite()`는 사용 가능한 기믹이 메가진화이고 `UseGen4BattleUI()`가 참일 때만 Gen4 전용 자산을 로드한다. 설정의 `배틀 UI: 4세대` 값은 기존 `newBattleUI`/`UseGen4BattleUI()` 경로를 그대로 사용한다. 기존 UI의 메가 트리거와 다른 기믹의 트리거는 기존 자산을 유지한다.
- 독립 변환한 Gen4 `.4bpp`·`.gbapal`이 현재 산출물과 각각 바이트 단위로 일치했다. ELF 심볼에서 기본/Gen4 메가 트리거 그래픽과 팔레트가 모두 포함됐음을 확인했다.
- `make hns -j8` 성공: EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,325,572/33,554,432(99.32%). `pokehns.gba`: 33,554,432바이트, SHA-256 `9e64ed53f81633f2f7d599926493b3aceb3e8adb08e0d3f7e95aee722efe221b`. 로그 `build/localization-logs/hns-gen4-mega-trigger-20260918.log`.
- 실제 mGBA 화면은 이 환경에서 확인하지 못했다. 새 ROM에서 `배틀 UI: 4세대`와 기존 UI를 각각 선택한 뒤 메가진화 가능한 포켓몬의 기술 선택 화면에서 아이콘 모양·위치·켜짐 상태를 비교한다.

## 인게임 확인 대기 — 요약 화면 L/R 버튼과 한글 사이 1픽셀 간격 보존

- 사용자가 직전 ROM에서 L/R 버튼 잘림은 해결됐지만 버튼을 오른쪽으로 옮긴 만큼 `노력치`·`개체값` 앞의 1픽셀 간격이 사라졌음을 확인했다. 직전의 버튼 이동 방식은 폐기하고, 버튼과 라벨을 원래 PNG 위치로 복원했다.
- 원래 버튼의 왼쪽 테두리는 사용 타일 앞 칸으로 1픽셀 넘어간다. 사용하지 않던 타일 218·234를 전용 합성 타일로 만들어, 기존 앞 타일 내용은 유지하면서 마지막 1픽셀 열에 버튼 왼쪽 테두리만 포함했다. `page_skills.bin`의 버튼 앞 칸 두 곳만 타일 239→218, 175→234로 바꿨다. HNS 사본 맵도 동일하게 갱신했다.
- 정적 렌더에서 L/R 버튼 전체, 버튼 오른쪽의 1픽셀 배경 간격, `노력치`·`개체값` 글자 위치가 모두 보존됨을 확인했다. PNG 기본 화면 영역은 공통 번역 PNG와 같고, 차이는 전용 합성 타일을 만드는 26픽셀뿐이다.
- `make hns -j8` 성공: EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,452/33,554,432(99.31%). 현재 `pokehns.gba`: 33,554,432바이트, SHA-256 `043c4acabf098c280f4fce87c6b74d309bc202c43ab674244ceb302557c48c05`. 로그 `build/localization-logs/hns-summary-lr-gap-preserved-20260918.log`.
- 독립 변환한 `.4bpp`, `.gbapal`, `.4bpp.smol`, `page_skills.bin.smolTM`이 빌드 산출물과 모두 일치했다. 실제 mGBA 화면은 사용자 확인 대기다.

최종 갱신: 2026-09-19 (한국 시간)

## 조사 경과 — 요약 화면 IV/EV L/R 아이콘 가장자리 잘림 (미해결)

- 직전 시도에서 `tiles.png`의 타일 219–220·235–236을 버튼 프레임 전용으로 오판해 이전 HNS 원본 픽셀로 바꿨고, 사용자가 확인한 화면에서 글자까지 잘리는 회귀가 생겼다. 해당 타일에는 글자 획도 섞여 있었다.
- 그 시도만 되돌려 `graphics/summary_screen/hns/tiles.png`를 직전 사용자 편집 상태의 해시 `e37b7aa80f2171d8cc79731b01da7d7437ebfd8457308aa1c2d15b496b33bd4b`로 정확히 복원했다. 복원본 HNS 빌드 성공; EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,436/33,554,432(99.31%). 새 ROM SHA-256 `9ff4d8a32ce862512dc5e09fe9ffad8b6d66649d04ba846ee43f2d5901409472`, 로그 `build/localization-logs/hns-summary-iv-ev-lr-revert-20260917.log`.
- 재조사 결과, 전체 화면의 픽셀 출력 범위/페이지 스크롤이 원인일 가능성은 낮다. 스킬 페이지 타일맵은 x=10~29타일(화면 x=80~239px)에 있고, `PssScrollRight`는 8회에 걸쳐 `0x2000`씩 이동해 총 `0x10000`(256px)로 정수 타일 경계에 정착한다. BG X 오프셋을 통째로 1px 바꾸면 다른 패널과 BG0 텍스트 창까지 어긋날 수 있으므로 적용하지 않았다.
- 현재 HNS/공통 `tiles.png` 해시가 모두 `e37b7aa80f2171d8cc79731b01da7d7437ebfd8457308aa1c2d15b496b33bd4b`이고, HNS/공통 `page_skills.bin`도 바이트 단위로 동일하다. `src/pokemon_summary_screen.c`에는 HP 자간 및 타입 아이콘 위치 변경만 남아 있고, L/R 페이지 오프셋 변경은 없다. 따라서 잘림이 L/R 상자 각각의 왼쪽 1픽셀에만 있다면 전역 뷰포트보다 해당 그래픽 타일 경계/셀 매핑을 특정해야 한다. 이전의 타일 오수정이 있었으므로 정확한 화면 확대 crop과 대응 타일을 확인하기 전에는 PNG·타일맵을 편집하지 않는다.
- 사용자가 전체 mGBA 캡처를 제공해 수정 범위를 확정했다. `노력치`·`개체값` 글자, L/R 글자, 각 상자의 화면 위치는 유지하고 각 L/R 상자의 왼쪽에서 잘린 세로 1픽셀 열만 복구한다. 전체 화면 이동이나 글자/상자 이동은 금지한다.
- 타일맵에서 헤더 행의 타일 219·220은 (x=24,25, y=6), 235·236은 (x=19,20, y=6)에 놓인다. 이 셀들이 화면에 보인 L/R 상자와 정확히 대응하는지 픽셀 기준으로 최종 확인한 뒤, 해당 테두리 픽셀만 수정한다. 번역 글자가 섞인 타일 전체를 기준 이미지로 덮어쓰지 않는다.
- 위에서 특정한 타일맵 좌표는 화면의 실제 BG 스크롤/버퍼 좌표와 대응을 확인하지 않은 상태였다. 타일 시트와 화면 좌표를 혼동해 잘못된 픽셀을 바꿨으므로, 이전의 8픽셀 변경 및 “완료” 기록은 무효다. 새 ROM에서 실제 버튼 가장자리를 확인하고 해당 픽셀의 BG 타일 ID와 좌표를 확정하기 전까지 추가 그래픽 수정은 보류한다.

최종 갱신: 2026-09-17 (한국 시간)

## 2026-09-17 — HNS 요약 화면 타일 시트 재변환

- 사용자가 수정한 `graphics/summary_screen/hns/tiles.png`(128×120, indexed 8-bit, 2668바이트)를 원본 그대로 보존하고 HNS 빌드 경로에서 다시 변환했다. `.4bpp`, `.gbapal`, `.4bpp.smol`이 갱신됐고 새 ROM에 반영됐다.
- 별도 임시 경로에서 `gbagfx`로 다시 변환한 `.4bpp`·`.gbapal`이 빌드 산출물과 각각 바이트 단위로 일치함을 확인했다. `git diff --check -- graphics/summary_screen/hns/tiles.png` 통과. `make --jobserver-style=pipe hns -j8` 종료 코드 0; EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,436/33,554,432(99.31%). `pokehns.gba` 33,554,432바이트, SHA-256 `9ff4d8a32ce862512dc5e09fe9ffad8b6d66649d04ba846ee43f2d5901409472`. 로그 `build/localization-logs/hns-summary-screen-tiles-reconvert-20260917.log`.
- 변환·빌드 검증 완료. 새 ROM을 실제 게임에 띄워 화면을 확인하지는 않았다. 요약 화면에서 수정한 라벨·타일의 깨짐, 잘림, 팔레트 이상을 점검한다.

## 2026-09-17 — 요약 화면 장소 미상 표기 색상 적용

- `gText_XNatureMetSomewhereAt`, `gText_XNatureHatchedSomewhereAt`의 `어딘가` 앞뒤에 `{DYNAMIC 0}`/`{DYNAMIC 1}`을 넣었다. 이는 실제 장소명을 출력할 때와 같은 빨간 글씨·초록 그림자 스타일을 적용하고, 뒤의 `에서`는 일반 색상으로 복구한다. 문구와 줄바꿈은 유지했다.
- `git diff --check -- src/strings.c` 통과. `make --jobserver-style=pipe hns -j8` 종료 코드 0; EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,420/33,554,432(99.31%). `pokehns.gba` 33,554,432바이트, SHA-256 `140581139f1097d2c28168b3822e417f283c1052b788b0ace9b88f914052f7c2`. 로그 `build/localization-logs/hns-summary-unknown-location-color-20260917.log`.
- 코드·빌드 검증 완료. 실제 mGBA 화면은 미확인이다. 요약 화면의 만난 장소/부화 장소 미상 문구에서 `어딘가`의 색상과 그림자가 실제 장소명과 맞는지 확인한다.

## 2026-09-17 — 요약 화면 타입 라벨과 아이콘 간격 조정

- 기존 요약 화면은 영어 `TYPE/` 기준 고정 좌표 x=120, 160, 200에 타입 아이콘을 그렸다. 한글 `타입/`은 영어보다 좁지만 아이콘은 같은 좌표에 남아 있어 라벨 오른쪽의 빈 공간이 커졌다.
- `src/pokemon_summary_screen.c`의 `SetMonTypeIcons()`에서 타입 라벨 창 시작 좌표와 `GetStringWidth(FONT_NORMAL, gText_TypeSlash, 0)`를 이용해 첫 아이콘을 라벨 폭 + 2px 간격에 놓도록 바꿨다. 두 번째·세 번째 아이콘은 기존 40px 간격을 유지한다.
- `git diff --check -- src/pokemon_summary_screen.c src/strings.c include/strings.h` 통과. `make hns -j8` 종료 코드 0; EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,404/33,554,432(99.31%). `pokehns.gba` 33,554,432바이트, SHA-256 `c25633060e7514d7fcbaeda578f4a9268905587e6d67e6e9938873735eda6d99`. 로그 `build/localization-logs/hns-summary-type-icon-spacing-20260917.log`.
- 코드·빌드 검증은 완료했다. 새 ROM의 mGBA 화면은 아직 확인하지 않았다. 능력치 페이지에서 한글 타입 라벨과 1·2타입 아이콘 간격, 알의 미스터리 타입 아이콘 위치를 확인한다.

## 2026-09-17 — 요약 화면 `앞으로` 추가 원복

- 경험치 창을 원래 6타일(48px), `tilemapLeft=24`로 복원하고 현재/다음 레벨 경험치 값의 정렬 기준을 42px로 되돌렸다. `앞으로` 출력 코드와 `gText_UntilNextLv` 선언은 제거했다. 문자열 정의는 이미 작업 트리에 없었다.
- 사용자가 수정한 `gText_NextLv`의 `다음 레벨까지`는 유지했다. 별도 요청으로 추가했던 HP 앞 `{JPN}` 간격도 이번 원복 범위와 무관하므로 유지했다.
- `git diff --check -- src/pokemon_summary_screen.c include/strings.h src/strings.c` 통과. `make hns -j8` 종료 코드 0; EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,452/33,554,432(99.31%). `pokehns.gba` 33,554,432바이트, SHA-256 `77e27005e2520338ed0dc8e30d0cd8b315035feda0c885ef4d6ee293b913c86c`. 로그 `build/localization-logs/hns-summary-forward-revert-20260917.log`. 일부 다른 코드의 컴파일 경고는 있었지만 오류 없이 링크됐다.
- 게임 화면 검증은 미수행. 새 ROM에서 능력치 페이지의 레벨 텍스트가 `다음 레벨까지`로 표시되고, `앞으로`가 없으며, 경험치 창의 숫자가 원래 영역 안에 표시되는지 확인한다.

## 2026-09-17 — 경험사탕 레벨업 메시지 변수 수정

- 경험사탕 XL 사용 후 레벨이 64가 됐는데 경험치 30000과 레벨 30000으로 표시되던 원인은 `gText_PkmnGainedExpAndElevatedToLvVar3`의 레벨 자리가 경험치와 같은 `{STR_VAR_2}`를 사용한 것이었다. `src/party_menu.c`는 경험치를 `gStringVar2`, 최종 레벨을 `gStringVar3`에 넣으므로 문자열의 레벨 자리를 `{STR_VAR_3}`로 수정했다. 경험치 계산·레벨업 로직은 변경하지 않았다.
- 경험사탕 XS/S/M/L/XL은 모두 `ItemUseCB_RareCandy()`의 같은 레벨업 메시지를 사용하므로 모두 수정 대상이다(각각 100/800/3000/10000/30000 경험치). 레벨업이 없는 경우에는 경험치 전용 메시지만 출력한다. 이상한사탕은 별도의 `gText_PkmnElevatedToLvVar2` 경로를 사용하며 전달 변수도 맞아 문제를 찾지 못했다.
- `git diff --check -- src/strings.c` 통과. `make hns -j8` 종료 코드 0; EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,516/33,554,432(99.31%). `pokehns.gba` 33,554,432바이트, SHA-256 `f5a8c38d40eb17cacfac5fd1875136bd3c24afbbfcaee1de3efedb82eb922f54`. 로그 `build/localization-logs/hns-exp-candy-output-20260917.log`. 일부 다른 코드/맵 어셈블리 경고가 있었지만 오류 없이 링크됐다.
- mGBA에서 캔디 사용 화면은 아직 직접 재검증하지 않았다. 새 ROM에서 XL로 63→64일 때 `30000 경험치를 얻고 / 레벨64로 올랐다!`가 표시되는지, XS~L도 레벨업이 발생하는 경우 각각 실제 획득 경험치와 새 레벨이 구분되는지 확인한다.

## 2026-09-17 — 요약 화면 `앞으로` 위치를 잔여 경험치에 맞춤

- 고정 x=0에 있던 `앞으로`가 HNS의 EXP 값 창에서 남은 경험치 숫자와 멀리 떨어질 수 있어, 숫자의 우측 정렬 위치에서 `앞으로` 문자열 폭과 2px 간격을 뺀 곳에 출력하도록 변경했다. 따라서 HNS 녹색 경험치 박스 안에서 문구와 잔여량이 한 묶음으로 오른쪽 끝에 맞춰진다.
- HNS 빌드 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,516/33,554,432(99.31%). `pokehns.gba` 33,554,432바이트. 로그 `build/localization-logs/hns-summary-forward-position-20260917.log`; SHA-256 `68eb08159e08f92e9889532c481a87275c971b749c9a71413f0790710cd2e0f7`.
- `git diff --check -- src/pokemon_summary_screen.c` 통과. 미사용 `CB2_PssChangePokemonNickname` 경고 한 건 외 빌드 오류 없음. 새 ROM에서 실제 위치는 아직 mGBA 검증 전이므로, 요약 능력 페이지에서 `앞으로`와 잔여 경험치가 붙어 보이는지 확인한다.

## 2026-09-17 — 요약 화면 경험치 숫자 잘림 수정

- mGBA 캡처에서 `앞으로`만 보이고 현재 경험치와 다음 레벨까지 남은 경험치 숫자가 모두 안 보이는 문제를 수정했다. 원인은 `PSS_DATA_WINDOW_EXP`가 6타일(48px)뿐인데 출력 정렬 폭은 72px인 불일치였다.
- `src/pokemon_summary_screen.c`에서 값 창을 pokeemerald-kr과 같은 10타일(80px), 시작 타일 20으로 넓혔다. 이제 72px 정렬 목표와 기존 2px 여백이 창 안에 들어간다. 현재 경험치/남은 경험치 출력과 `FLAG_LIMIT_TO_50` 계산 로직은 변경하지 않았다.
- `git diff --check -- src/pokemon_summary_screen.c` 통과. `make hns -j8` 종료 코드 0; EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,500/33,554,432(99.31%), `pokehns.gba` 33,554,432바이트. 로그 `build/localization-logs/hns-summary-exp-values-visibility-20260917.log`. 사용하지 않는 `CB2_PssChangePokemonNickname` 관련 경고 한 건 외 오류 없음.
- 새 ROM의 mGBA 화면은 아직 확인하지 않았다. 요약 능력치 페이지에서 EXP 숫자와 `앞으로` 다음 잔여량 표시를 확인한다.

## 2026-09-17 — HNS 요약 화면 타일 시트 번역 반영

- 사용자가 번역한 `graphics/summary_screen/hns/tiles.png`(128×120, indexed PNG)를 보존하고 HNS 빌드에 반영했다. PNG 용량은 작업 전 3007바이트에서 2660바이트로 바뀌어 있으며 원본 이미지는 직접 덮어쓰지 않았다.
- `make hns -j8` 종료 코드 0. PNG에서 `tiles.4bpp`·`tiles.gbapal`을 재생성하고 SMOL 압축 그래픽도 갱신한 뒤 `pokehns.gba`를 다시 링크했다. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,500/33,554,432(99.31%). 로그 `build/localization-logs/hns-summary-screen-tiles-ko-20260917.log`; ROM 33,554,432바이트.
- 소스 PNG는 육안으로 번역 이미지가 들어간 것을 확인했다. mGBA 실제 런타임 화면은 미확인이다. 새 ROM에서 요약 화면 제목·고정 라벨·페이지 전환 중 글자 잘림과 타일 깨짐을 확인한다.

## 2026-09-17 — 요약 화면 `tiles.png`와 `iv_ev_tiles.png` 용도 구분

- `graphics/summary_screen/tiles.png`는 일반 요약 화면의 기본 타일 시트다. 비-HNS 빌드에서 `src/graphics.c`가 로드하며, HNS 빌드는 별도 `graphics/summary_screen/hns/tiles.png`를 사용한다.
- `iv_ev_tiles.png`는 과거 IV/EV 요약 화면 기능에서 `STATS` 타일 라벨을 `IVs`/`EVs` 등으로 바꾸기 위한 대체 타일셋으로 추가된 파일이다(추가 커밋 `52666fb545`, 설정 설명 `P_SUMMARY_SCREEN_IV_EV_TILESET`). 현재 트리에는 이 매크로와 파일 참조가 없어 게임에 로드되지 않는다.
- 현재 `P_SUMMARY_SCREEN_IV_EV_VALUES TRUE` 설정은 IV 화면의 글자를 등급 대신 실제 숫자로 출력하는 코드 동작이며, `iv_ev_tiles.png`와는 별개다. 조사만 수행했고 그래픽·코드·ROM 변경이나 빌드는 없다.

## 2026-09-17 — 요약 화면 HP 간격 및 레벨업 경험치 문구

- 포켓몬 능력 페이지의 HP 표시는 `{JPN}` 제어 코드와 `HP`를 합쳐 출력하도록 바꿨다. 이는 pokeemerald-kr의 `hpTextJapanese` 처리와 같아 H/P 사이에 해당 글꼴 간격을 준다.
- 경험치 다음 레벨 행 왼쪽에 `gText_UntilNextLv`(`앞으로`)를 출력하고, 경험치 숫자 두 행은 데이터 창 오른쪽에 정렬하도록 폭을 42에서 72픽셀 기준으로 맞췄다. 기존 `gText_NextLv`(`레벨 업까지`) 표제와 HNS의 50레벨 제한 경험치 계산은 유지했다. `strings.h`에 있던 선언에 맞춰 문자열 정의도 추가했다.
- 검증: `git diff --check -- include/strings.h src/strings.c src/pokemon_summary_screen.c` 통과. `make hns -j8` 성공; EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,580/33,554,432(99.31%), `pokehns.gba` 33,554,432바이트. 로그 `build/localization-logs/hns-summary-screen-hp-exp-spacing-20260917.log`.
- mGBA에서 새 화면은 아직 확인하지 못했다. 새 ROM에서 포켓몬 요약의 능력 페이지에서 HP 간격, `앞으로`와 다음 레벨 경험치 숫자의 위치를 확인한다.

## 2026-09-17 — HNS 기본 플레이의 동상 상태 도달 여부 확인

- 현재 `include/config/battle.h`의 `B_USE_FROSTBITE`는 `FALSE`다. `MOVE_EFFECT_FREEZE_OR_FROSTBITE`는 이 설정에 따라 달라지며, FALSE일 때 일반 빙결 계열 기술·Secret Power는 `MOVE_EFFECT_FREEZE`를 사용한다. `src/data/moves_info.h`에서 `MOVE_EFFECT_FROSTBITE`를 직접 지정한 일반 기술은 찾지 못했고, 맵 스크립트의 일반 동상 부여도 찾지 못했다. 따라서 기본 정상 플레이 경로에서는 동상이 걸리지 않는다.
- 예외적으로 `include/config/debug.h`에서 오버월드 디버그 메뉴가 활성화되어 있고 기본 입력은 R+START다. `data/scripts/debug.inc`의 Inflict Status1 스크립트에는 개별/파티 전체에 `STATUS1_FROSTBITE`를 직접 지정하는 메뉴 항목이 있어 디버그 메뉴에서는 강제로 동상을 걸 수 있다. 이 경로는 `B_USE_FROSTBITE`의 일반 기술 효과 선택을 우회한다.
- 이번 요청은 코드 검색과 경로 확인만 수행했다. 파일·ROM 변경 및 빌드는 없다. 앞서 확인한 Gen4의 동상 라벨 누락은 기본 플레이에서는 드러나지 않지만 디버그 메뉴에서 동상을 걸고 Gen4 UI를 쓸 때 재현될 수 있다.

## 2026-09-17 — Gen4 배틀 UI 동상 아이콘 차이 확인

- `graphics/battle_interface/hns/status.png` 상태 시트의 여섯 번째 아이콘은 `FRB`(동상)지만 `graphics/battle_interface/gen4/status.png`는 `BRN`(화상)을 한 번 더 담고 있다. Gen4의 `status2.png`~`status4.png`도 같은 마지막 칸 구성을 사용한다. Gen4 UI는 2026-05-09의 별도 UI 도입 커밋에서 추가된 구형 자산이며, Frostbite용 그림 갱신 기록은 확인되지 않았다.
- 런타임 C 코드는 UI 종류와 무관하게 Frostbite일 때 `HEALTHBOX_GFX_STATUS_FRB_BATTLER0` 계열을 요청한다. 이 칸이 HNS/Gen3 자산에서는 `FRB`, Gen4 자산에서는 `BRN`이므로 Frostbite가 실제 적용된 포켓몬이 Gen4 UI를 사용하면 상태 글자가 `BRN`으로 잘못 보일 수 있다. `B_USE_FROSTBITE` 현재 기본값은 `FALSE`다.
- 이번 요청은 원인 확인만 수행했다. 코드·그래픽·ROM을 변경하지 않았고 빌드도 하지 않았다. 동상과 Gen4 UI를 함께 지원하려면 `gen4/status.png`~`status4.png`의 마지막 아이콘을 FRB 그림으로 만들고 산출물을 재생성하는 작업이 필요하다.

## 2026-09-17 — 공식 HNS Release-v2.0.6 업데이트 완료

- 공식 `Release-v2.0.5`(`1f42b74dff`)와 `Release-v2.0.6`(`167aa6d537`)의 전체 차이 10개 경로를 현재 작업 트리에 반영했다. 작업 전 대상 파일은 `build/localization-backups/pre-hns-206-20260917.tar.gz`에 보관했다.
- 공식 바이트로 갱신한 자산은 현대식 동굴 물·긴 풀·연못 물·모래·긴 풀 배틀 배경 타일맵 5개와 HNS `press_start.png`다. 타이틀 PNG의 내용 해시는 공식 2.0.6과 일치하며 Git 차이는 `/mnt/c` 실행 권한 메타데이터뿐이다. 빌드에서 관련 `.smolTM`, `.4bpp`, `.gbapal`, `.smol` 산출물도 재생성됐다.
- 기존 변경과 겹친 소스는 파일 전체를 교체하지 않고 2.0.6 수정만 기능 단위로 통합했다. 디버그 1번 스크립트는 라프라스 이벤트로 연결했고, 아이스볼 애니메이션의 `rolloutTimer` 오프셋을 수정했다. 고정·이벤트 포켓몬 및 이로치 스크립트 포켓몬은 Synchronize/Cute Charm의 성격·성별을 반영하도록 갱신했다.
- 기존 한글 성격명, 조사 처리(`gJongCode`), L 버튼 기술 설명, 32MiB/고음 피치 메가 울음소리 설정 등 사용자 한글화·1.17.0 이식 변경은 보존했다. `git diff --check` 통과.
- `make hns -j8` 성공: EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,484/33,554,432(99.31%), 결과 `pokehns.gba` 33,554,432바이트. 로그 `build/localization-logs/hns-206-update-20260917.log`.
- 실제 게임 화면은 이 환경에서 확인하지 못했다. 새 ROM에서 현대식 배틀 배경 5종, 타이틀, 아이스볼 연속 사용 크기, 싱크로나이즈 선두 상태의 고정/이로치 이벤트 포켓몬 성격을 확인한다.

## 2026-09-17 — 포켓몬 요약 화면 표제어 위치 확인

- `PROFILE`, `ABILITY`, `TRAINER INFO`처럼 화면에 고정으로 그려지는 HNS 요약 화면 표제어는 C 문자열이 아니라 `graphics/summary_screen/hns/tiles.png`(128×120, indexed PNG) 타일 그래픽에 포함된다. HNS 빌드에서는 `src/graphics.c:1809-1810`이 이 이미지를 `tiles.4bpp.smol` 및 `tiles.gbapal`로 사용한다. 화면 타일 배치는 정보 페이지의 `graphics/summary_screen/page_info.bin` 타일맵과 `src/pokemon_summary_screen.c:1563` 경로가 담당한다.
- 코드로 출력되는 페이지 제목은 `src/pokemon_summary_screen.c:3512-3519`의 `PrintPageNamesAndStats()`이며, 문자열 정의는 `src/strings.c:440-443`의 `gText_PkmnInfo`, `gText_PkmnSkills`, `gText_BattleMoves`, `gText_ContestMoves`다. 요약 화면의 능력치·상태·기술 정보 라벨도 같은 함수에서 `gText_*`를 호출한다.
- 특성 이름과 설명은 고정 표제어와 별개로 `src/data/abilities.h`의 `gAbilitiesInfo[].name`·`.description` 데이터다. 요약 화면은 `src/pokemon_summary_screen.c:3695-3711`에서 정보 페이지를 그리고 `:3771-3780`에서 해당 이름·설명을 출력한다. 어버이명·ID·성격 메모 등 동적 정보도 이 파일의 `PrintMonOTName()`, `PrintMonOTID()`, `PrintMonTrainerMemo()` 경로를 따른다.
- 이번 요청은 위치 조사만 수행했다. 코드·그래픽·ROM은 변경하지 않았으며 빌드는 하지 않았다. 화면 고정 표제어를 실제로 번역할 때는 PNG 타일 시트와 인덱스 팔레트를 유지하고, 타일 배치가 바뀌지 않으면 `page_info.bin`은 건드리지 않는다.

## 2026-09-17 — 포켓블록 화면 성격 표기 접미어 추가

- `src/use_pokeblock.c`의 성격 창 조립 순서를 성격명 + 공백 + `sText_NatureSlash`로 바꾸고, 해당 문자열을 `성격`으로 설정했다. 현재 `gNaturesInfo[].name`이 활용형으로 편집된 상태이므로 `NATURE_HARDY`는 `노력하는 성격`으로 출력된다. `src/pokemon.c`의 성격명은 이번 작업에서 수정하지 않았다.
- 검증: 대상 파일 `git diff --check` 통과, `make hns -j8` 성공. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,724/33,554,432(99.32%), `pokehns.gba` 33,554,432바이트. 로그: `build/localization-logs/hns-pokeblock-nature-text-20260917.log`.
- mGBA 런타임 화면은 직접 확인하지 못했다. 새 ROM에서 포켓블록 선택 화면의 성격 접미어와 긴 문자열 잘림을 확인한다.

## 2026-09-17 — 포켓몬 성격 한국어 표기 반영

- `src/pokemon.c`의 `gNaturesInfo` 25개 이름은 공식 성격명 어휘를 바탕으로 상태창 문장에 자연스럽게 이어지도록 활용형으로 편집되어 있다(예: `노력하는`, `외로움을 타는`, `용감한`). 이름 내부의 색상·그림자 제어 코드는 보존한다.
- 성격 ID·능력치 보정·배틀 팰리스 관련 데이터·애니메이션 값은 변경하지 않았다.
- `gNaturesInfo[].name`은 요약 화면 트레이너 메모(`src/pokemon_summary_screen.c:3847`, 민트로 바뀐 능력치 성격도 메모에 괄호로 추가), 포켓블록 먹이기 메뉴(`src/use_pokeblock.c:1383-1398`), 디버그 포켓몬 생성의 성격 선택 단계(`src/debug.c:3211-3221`)에서도 출력된다. 사파리 배틀 정보용 포매터(`src/battle_interface.c:1201-1213`)에도 이름 참조는 있으나, 현재 `SwapHpBarsWithHpText()`가 아군만 허용하고 사파리의 아군 경로를 건너뛰므로 반대편 전용 호출 지점(`1313-1317`)은 실제로 도달할 수 없다.
- 포켓블록 선택 화면은 초기 로드 때 선택된 포켓몬 정보를 `UpdateMonInfoText()`로 출력하고, 다른 포켓몬으로 이동할 때도 같은 함수를 다시 호출해 성격명을 갱신한다. `WIN_NATURE`에 성격명 + 공백 + `sText_NatureSlash`를 출력하며, 접미어 값은 현재 `성격`이다. `GetNature()`를 사용해 민트로 능력치 보정만 바뀐 성격은 따로 표시하지 않고, 취소 항목에서는 성격 창을 비운다.
- HGSS 도감의 진화 조건 출력에는 특정 성격 조건(`IF_NATURE`)용 이름 삽입 코드가 있지만(`src/pokedex_plus_hgss.c:6933-6936`), 현재 종 데이터에는 `IF_NATURE` 사용 사례가 없다. 배틀프런티어 라운지의 성격 소녀(`src/field_specials.c:2976-2985`, HNS 대사 `data/maps/BattleFrontier_Lounge5_hns/scripts.inc`)는 이름이 아니라 능력치 경향에 대응하는 고정 설명문을 고르므로, 성격 `.name` 수정과는 별개다.
- 검증: 성격명 항목 25개, 이전 영문 성격 문자열 잔존 0건. 최신 `make hns -j8` 성공; EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,724/33,554,432(99.32%), `pokehns.gba` 33,554,432바이트. 로그: `build/localization-logs/hns-pokeblock-nature-text-20260917.log`.
- 이 환경에서 mGBA를 실행할 수 없어 포켓몬 요약·성격 관련 화면의 런타임 표기는 직접 확인하지 못했다. 다음 확인 때 성격이 표시되는 화면에서 긴 명칭의 잘림과 정렬을 살핀다.

## 2026-09-17 — 타입 아이콘 PNG 4bpp 재변환

- 사용자가 수정한 `graphics/types/`의 타입 아이콘 PNG 24개(일반 타입·콘테스트 타입·페어리·스텔라 포함)를 확인하고, 수정되지 않은 `mystery.png`·`none.png`까지 포함해 개별 아이콘 PNG 26개를 `tools/gbagfx/gbagfx`로 각각 `.4bpp`로 재생성했다. `battle_icons1/2.png`와 `move_types` 시트는 이번 요청에서 수정된 PNG가 아니므로 기존 산출물을 유지했다.
- 모든 개별 아이콘은 32×16 픽셀, 256바이트 4bpp 산출물이며, 입력 PNG는 인덱스 컬러(4-bit 또는 8-bit colormap)로 `gbagfx` 변환에 성공했다. 각 파일을 임시 경로에 독립 변환한 결과와 실제 `.4bpp` 파일을 `cmp`로 대조해 26개 모두 바이트 단위로 일치함을 확인했다. `battle_icons1.png`·`battle_icons2.png`도 각각 기존 640바이트 `.4bpp`와 독립 변환 결과가 일치했다.
- 검증: `make hns -j8` 성공(로그 `build/localization-logs/hns-type-icons-20260917.log`), 링크 메모리 EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,420/33,554,432(99.31%).
- 런타임 참조는 배틀 타입 아이콘의 `battle_icons1/2.4bpp.smol`, 기술·요약 화면 타입 라벨의 `move_types.4bpp.smol` 시트다. 사용자가 제공한 최신 mGBA 캡처에서 요약 화면의 `불꽃`·`비행` 라벨이 실제로 출력되어, 현재 ROM의 압축 시트에도 번역 결과가 반영된 것을 확인했다. 이후 개별 PNG를 다시 수정했는데 화면이 갱신되지 않을 때만 해당 시트를 재생성하면 된다.
- 이 환경에는 mGBA 실행 파일이 없으므로 위 캡처 외의 배틀·기술 정보 화면은 직접 재현하지 못했다. 사용자가 새 `pokehns.gba`에서 각 타입 아이콘의 한글 픽셀·팔레트·타일 경계를 확인한다.

## 2026-09-17 — 파티 메뉴 `MENU_READ` 출력 조건 확인

- `src/data/party_menu.h:710`의 `MENU_READ`는 메일 읽기 콜백(`CursorCb_Read`)에 연결된 파티 메뉴 항목이다. 필드에서 파티 포켓몬을 선택했을 때 해당 포켓몬이 메일을 지니고 있으면 `src/party_menu.c:3000`이 `MENU_ITEM` 대신 `MENU_MAIL`을 추가한다.
- `MAIL`을 선택하면 `ACTIONS_MAIL` 목록(`READ / TAKE / CANCEL`)이 열리며, 여기서 `READ`를 선택할 때만 `CB2_ReadHeldMail()`이 실행되어 포켓몬이 지닌 메일 본문을 표시한다. 메일이 없거나 전투·다른 파티 메뉴 모드에서는 이 `READ` 항목이 나오지 않는다.

## 2026-09-17 — 파티 메뉴 기술·폼 변경 항목 출력 조건 확인

- `LEARN MOVES`는 필드 파티 메뉴에서 기술 복습 기능이 켜져 있고(`P_PARTY_MOVE_RELEARNER`), 해당 포켓몬에게 하나 이상의 복습 가능한 기술이 있을 때 표시된다. 하위 메뉴의 `LEVEL MOVES`·`EGG MOVES`·`TM MOVES`·`TUTOR MOVES`는 각 분류에 실제로 배울 수 있고 아직 습득하지 않은 기술이 있을 때만 개별적으로 추가된다.
- `Light bulb`부터 `Lawn mower`까지의 카탈로그 항목은 `로토무카탈로그`를 사용해 파티 포켓몬을 고른 뒤 열리는 로토무 폼 선택 메뉴다. `Change form`·`Change Ability`는 `지가르데큐브` 사용 후 열리는 폼/특성 선택 메뉴다.
- `{PKMN} FOLLOWER`는 필드 파티 메뉴에서 첫 번째 생존·비알 포켓몬(현재 팔로워 후보)을 선택했을 때만 표시되며, 선택 시 `challengeSettings.followerEnable`을 토글한다.

### `LEARN MOVES` 상세 흐름

- 예를 들어 현재 레벨까지의 레벨업 기술 중 하나를 잊은 리자몽을 필드 파티 메뉴에서 선택하면, `P_PARTY_MOVE_RELEARNER`가 켜져 있는 경우 `LEARN MOVES`가 초기 메뉴에 추가된다. `CanBoxMonRelearnAnyMove()`가 네 분류를 검사해 하나라도 가능할 때만 추가되므로, 알·기술머신·기술 가르침 기술만 가능해도 같은 항목이 나타난다.
- `LEARN MOVES`를 선택하면 `CursorCb_LearnMovesSubMenu()`가 기존 창을 닫고 분류별 목록을 다시 만든다. 이때 실제 가능한 분류만 표시되며, 예를 들어 레벨업과 기술머신만 가능하면 `LEVEL MOVES / TM MOVES / CANCEL`만 출력된다. 분류를 선택해야 `gMoveRelearnerState`가 설정되고 기술 복습 화면이 열린다.

## 2026-09-16 — 특성 팝업 소유격 표기 한글화

- `src/battle_interface.c:2820-2834`의 특성 팝업 포켓몬 이름 뒤에 붙는 동적 영어 `'s` 조합을 제거하고, 항상 한국어 소유격 `의`를 붙이도록 수정했다. 닉네임 마지막 글자의 `s/S` 검사와 불필요한 임시 변수도 함께 제거했다.
- 특성명 자체는 기존과 동일하게 `src/data/abilities.h`의 `gAbilitiesInfo[].name`에서 가져오며, 이번 변경은 팝업 첫 줄의 소유격만 대상으로 한다. `src/battle_message.c`의 일반 배틀 메시지에 남아 있는 별도 영어 `'s` 문구는 변경하지 않았다.
- 검증: `make hns -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,564/33,554,432(99.31%). 로그: `build/localization-logs/hns-ability-popup-possessive-20260916.log`; 대상 파일 `git diff --check` 통과.
- 실제 mGBA 실행 파일은 이 환경에 없어 특성 팝업의 런타임 화면은 확인하지 못했다. 새 ROM에서 포켓몬 이름과 `의`가 연결되어 표시되는지 확인할 것.

## 2026-09-16 — 디버그 메뉴 다단계 선택의 B 버튼 이전 단계 복귀 보완

- `src/debug.c`의 기존 단계별 디버그 입력 구조를 유지하면서 B 버튼의 복귀 경로를 보완했다. 포켓몬(기본/복합)·아이템·워프의 중간 단계는 각각 직전 입력 화면을 다시 그리며, 첫 단계에서 B를 누르면 해당 `Give X`/`Utilities` 부모 메뉴로 돌아간다.
- 복합 포켓몬은 종→레벨→성별→반짝임→성격→특성→테라 타입→다이맥스 레벨→거다이맥스 여부→개체값→노력치→기술의 순서를 유지한다. 단계 사이에서 아이콘을 조기에 해제하지 않고, 완료·취소 시에만 아이콘과 임시 데이터를 해제하도록 정리했다.
- 플래그·변수, 날씨, 장식, 사운드 입력창도 B에서 부모 메뉴로 복귀하도록 통일했다. 플래그 목록은 부모의 특수 목록 모드(`listId=1`)를 보존하며, 부모 복귀 헬퍼가 기존 리스트 태스크·추가 창·콜백 스택을 함께 정리해 중복 태스크가 남지 않도록 했다.
- 검증: `make hns -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,612/33,554,432(99.32%). 로그: `build/localization-logs/hns-debug-b-button-20260916.log`; `git diff --check` 통과.
- 실제 mGBA 실행 파일은 이 환경에 없어 버튼 입력에 따른 런타임 화면은 확인하지 못했다. 새 ROM에서 디버그 메뉴의 각 단계에서 B를 눌러 직전 화면과 부모 메뉴가 복원되는지 확인할 것.

## 2026-09-16 — 20:34 KST 이후 HP 퍼센트 옵션 추가분 롤백 및 move info 재검증

- 사용자가 지정한 20:34 KST 요청 직전 상태를 기준으로, 해당 요청에서 추가된 `ChallengeSettings.hpPercentageDisplay` 비트, 배틀 설정의 `퍼센트 표시` 항목·설명·저장/로드, 새 게임·오박사 전달 코드, 저장값 기반 상대 HP 런타임 분기를 제거했다.
- 1.17.0 이식 과정에서 그보다 먼저 존재하던 `B_HP_PERCENTAGE_DISPLAY` 매크로와 기존 상수 기반 상대 HP 퍼센트 렌더링은 유지했다. 따라서 이번 롤백은 메뉴 토글과 저장값 연동만 대상으로 하며, 20:34 이전의 작업은 되돌리지 않았다. 아래의 이전 HP 토글 기록은 작업 이력으로 남긴다.
- 사용자가 다시 수정한 `graphics/battle_interface/move_info_window_l.png`·`move_info_window_r.png`를 각각 `tools/gbagfx/gbagfx`로 4bpp 변환했다. 두 PNG는 32×32, 8-bit colormap 형식을 유지하며, 직접 변환한 결과와 빌드 산출물을 `cmp`로 대조해 양쪽 모두 일치했다.
- 검증: `make hns -j8` 종료 코드 0, EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,516/33,554,432(99.31%), `pokehns.gba` 32MiB. 로그: `build/localization-logs/hns-rollback-move-info-20260916.log`; 롤백 대상 파일(기존의 별도 `oak_speech_hns.c` 변경 제외)의 `git diff --check`와 옵션 추가 식별자 잔존 검색도 통과했다.
- 실제 mGBA 실행 파일은 이 환경에 없어 롤백된 옵션 메뉴와 move info 팝업의 런타임 화면 확인은 수행하지 못했다.

## 2026-09-16 — 디버그 다단계 선택의 B 버튼 동작 원인 확인

- `expansion/1.17.0`의 PR #10121은 `struct DebugSelection`과 `DebugAction_Selection_StepUpdate()`를 사용하는 공통 선택기이며, `JOY_NEW(B_BUTTON)`에서 단계·하위 단계·데이터 인덱스를 감소시켜 이전 입력으로 돌아간다. 현재 HNS `src/debug.c`에는 이 공통 선택기(`tStep`, `tSubstep`, `DebugAction_Selection_StepUpdate`)가 없다.
- 현재 HNS는 기존 단계별 함수에 일부 B 전환만 수동으로 추가한 구조다. 따라서 `Give X → Pokémon (Basic)`의 레벨 단계(`src/debug.c:3159-3179`)와 워프의 맵 그룹 첫 단계(`src/debug.c:1548-1551`)는 B를 누르면 이전 단계가 아니라 `DebugAction_DestroyExtraWindow()`로 전체 입력 창을 종료한다. 복합 포켓몬·아이템 수량·워프의 일부 중간 단계만 이전 단계로 전환된다.
- `HEAD`와 `expansion/1.17.0` 태그는 공통 조상 `3efb836f72`에서 분기되어 태그가 현재 HNS의 조상이지 않으므로, 다른 1.17.0 코드 업데이트만으로 디버그 공통 선택기가 자동 반영되지 않는다.
- 이번 세션은 원인 조사만 수행했으며 코드·ROM은 수정하지 않았다. 복합 포켓몬 메뉴에서도 B가 동작하지 않는다면 현재 소스의 분기와 실행 중인 ROM이 같은 빌드인지 먼저 확인한다. 완전한 1.17.0 동작이 필요하면 공통 선택기 재이식 또는 누락된 단계별 B 분기 보완이 다음 작업이다.

## 2026-09-16 — HP 퍼센트 화면의 회색 점선 진단 정정

- 사용자가 OFF 화면에서는 해당 점선이 보이지 않는다고 알려 이전 기록의 “아군 체력박스 PNG에 항상 포함된 장식”이라는 설명을 재검토했다.
- 첨부된 OFF 화면에서 HP 박스 아래 `EXP` 글자 오른쪽에 보이는 회색 점선은 `graphics/battle_interface/hns/expbar.png`의 EXP 바 미충전 타일이며, `src/battle_interface.c:2387-2407`의 `EXP_BAR` 갱신 경로가 그린다. 이는 HP 바가 아니며, 캡처상 OFF 화면에도 EXP 바의 회색 구간이 남아 있다.
- 만약 사용자가 가리킨 것이 EXP 바가 아닌 HP 바 옆의 별도 선이고 ON에서만 나타난다면, 현재 퍼센트 코드에는 아군 분기가 없다(`src/battle_interface.c:1156-1163`은 아군 기존 HP 출력, 퍼센트 분기는 상대). 따라서 정상적인 옵션 효과가 아니라 잘못된 스프라이트/VRAM 영역 또는 잔상 가능성으로 분리해 확인해야 한다. 해당 선을 표시한 확대 캡처가 있으면 위치를 확정할 수 있다.
- 기존 화면 아티팩트 기록에서 회색 점선을 아군 PNG의 고정 장식으로 단정한 부분은 이 정정으로 대체한다. 코드·그래픽은 수정하지 않았으며, 다음 작업 전 HP 바/EXP 바 위치를 동일 배틀의 ON·OFF 캡처로 다시 대조한다.

## 2026-09-16 — 상대 HP 퍼센트 표시 화면 아티팩트 원인 확인

- 사용자가 mGBA에서 확인한 싱글 배틀 화면에서 상대 체력박스 아래에 흰 사각형과 퍼센트가 떠 있고, 아군 체력박스의 HP 바 위쪽에 회색 점선이 보이는 현상을 제보했다.
- 흰 사각형·분리된 퍼센트는 계산 오류가 아니다. 현재 `src/battle_interface.c:1025-1067`의 `PrintHpPercentageOnHealthbox()`가 기존 HNS 상대 싱글 자산(`graphics/battle_interface/hns/healthbox_singles_opponent.png`, 64×64)을 64×32 OAM 두 장으로 계속 사용하면서, 투명한 하단 영역에 `FillSpriteRectColor(x=32, y=24, w=24, h=8)`로 배경색을 칠하고 `y=21`에 퍼센트를 그린다. 퍼센트용으로 프레임이 확장되지 않아 배경색 블록과 떠 있는 글자처럼 보인다.
- 공식 1.17.0 구현은 퍼센트 전용 128×64 상대 체력박스 자산, 싱글 상대 OAM의 64×64 전환(오른쪽 타일 +64), 상대 박스 Y 좌표 22를 함께 추가한다. 현재 HNS의 런타임 토글 포트에는 이 레이아웃 전환이 아직 없으므로, 다음 수정에서 HNS용 확장 자산/타일 배치를 통합하거나 기존 영역 안에 퍼센트를 클리핑해야 한다.
- 아군 HP 바 위 회색 점선은 퍼센트 옵션과 무관하다. HNS 아군 자산 `graphics/battle_interface/hns/healthbox_singles_player.png`에 포함된 프레임 장식이며, 아군은 `PrintHpOnHealthbox()` 경로로 기존 현재 HP/최대 HP를 출력한다. 따라서 상대 HP만 바꾸는 옵션이 이 선을 만들지는 않는다.
- 이번 항목은 사용자 화면과 소스·그래픽 자산을 대조한 원인 조사이며 코드·그래픽은 수정하지 않았다. mGBA 실행 파일은 이 환경에 없어 동일 ROM의 로컬 재현은 하지 못했다. 다음 작업은 확장 체력박스 자산을 HNS 팔레트로 준비할지, 현재 64×32 레이아웃에 퍼센트를 맞출지 결정한 뒤 수정·빌드·실기 화면 검증을 수행하는 것이다.

## 2026-09-16 — 배틀 설정 상대 HP 퍼센트 표시 토글 (20:34 KST 이후 작업, 현재 롤백됨)

- `배틀 설정`의 `도망가기 프롬프트` 바로 아래에 `퍼센트 표시` 항목을 추가했다. ON/OFF 설명은 요청한 두 문장(`상대 포켓몬의 HP가\n챔피언스처럼 %로 표시됩니다`, `상대 포켓몬의 HP가\n기존과 동일하게 표시됩니다`)을 그대로 사용한다.
- `ChallengeSettings`의 기존 여유 비트에 저장 필드 `hpPercentageDisplay`를 추가하고, 새 게임 기본값은 `B_HP_PERCENTAGE_DISPLAY`(현재 `FALSE`)를 따르도록 했다. 메뉴 선택값(ON=0/OFF=1)은 저장 필드(직접 boolean)와 반대로 변환하며, HNS 오박사 새 게임 흐름에서도 옵션을 보존한다.
- 상대 체력박스 초기화·HP 갱신 시 저장된 설정을 검사해 ON이면 기존 `B_HP_PERCENTAGE_DISPLAY` 렌더링 경로를 사용하고 OFF이면 기존 HP 표기를 유지한다. 컴파일 매크로는 새 세이브 기본값만 정하며, 메뉴의 OFF 선택을 강제로 무시하지 않는다.
- 검증: `make hns -j8` 종료 코드 0, EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,325,188/33,554,432(99.32%), ROM 파일 32MiB. 로그 `build/localization-logs/hns-hp-percentage-option-20260916-r3.log`.
- 실제 게임 검증: mGBA 실행 파일이 없어 옵션 화면에서 항목 위치·설명과 싱글/더블 배틀 HP 표시 전환은 아직 확인하지 않았다. 다음 세션에서 새 게임/기존 세이브 각각 ON·OFF를 저장한 뒤 야생 싱글과 더블 배틀에서 확인한다.

## 2026-09-16 — 설원동굴·신도마을 표기 반영

- 요청된 `MAPSEC_SNOWSWEPT_CAVERN`을 `설원동굴`, `MAPSEC_NEW_SINJOH`를 `신도마을`로 확정했다. `MAPSEC_SINJOH`의 일반 지역명 `신도`와 `MAPSEC_SINJOH_RUINS`의 `신도유적`은 유지했다.
- JSON 변경을 `src/region_map.c:251,253`의 조토 수동 런타임 표에도 반영하고, `make hns -j8`로 생성 헤더와 ROM을 재생성했다. JSON·C 표·생성 헤더의 해당 이름이 모두 일치한다.
- 검증: JSON 파싱 및 342개 지역명 ASCII 잔존 검사, HNS/조토 이름 일치 검사, `git diff --check`, `make hns -j8` 종료 코드 0. EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,516/33,554,432(99.31%); 로그 `build/localization-logs/hns-region-name-sinjoh-ko-20260916-r5.log`.
- 실제 게임 검증: mGBA 실행 파일이 없어 화면 확인은 미수행이다.

## 2026-09-16 — New Sinjoh 명칭 근거 조사

- 인게임에 `NEW SINJOH`라는 표지판(`data/maps/NewSinjoh_hns/scripts.inc:312`)은 있지만, 왜 그렇게 부르는지 설명하는 직접적인 대사는 없다. `SINJOH RUINS` 표지판은 유적 맵에 별도로 존재한다.
- New Sinjoh의 NPC 대사는 마을 공동체, 현지인·관광객, 새로 온 사람들, 온천을 언급하고, 스토리 후에는 “SINJOH에 균형이 돌아왔다”고 말한다. 이는 배경 설명은 되지만 지명 어원의 설명은 아니다.
- 따라서 현재 데이터에서 `New Sinjoh`는 Sinjoh 유적 인근의 새 설원 마을이라는 개발 설정으로 해석하는 것이 가장 안전하다. 정적 조사만 수행했으며 코드·데이터·ROM은 추가로 수정하지 않았다.

## 2026-09-16 — Sinjoh 지역 ID 차이 확인

- `MAPSEC_SINJOH`는 지역 지도 JSON에만 등록된 일반/예비 슬롯이며, 현재 `data/maps/*/map.json`에서 이 ID를 실제 맵에 배정한 곳은 없다. 팝업 테마와 비행 목적지 처리도 `MAPSEC_NEW_SINJOH` 및 `MAPSEC_SINJOH_RUINS`만 사용한다.
- `MAPSEC_NEW_SINJOH`는 실제 플레이 가능한 설원 마을 `MAP_NEW_SINJOH_HNS`와 포켓몬센터·민가·온천·기모노 은신처를 대표한다. 49번 도로·50번 도로와 연결되고, 50번 도로를 통해 `MAP_SINJOH_RUINS_HNS`로 이동한다.
- 따라서 두 이름이 같은 `신도`로 번역되어 있어도 현재 게임에서 실제로 표시되는 일반 맵은 `NewSinjoh` 쪽이며, 유적은 별도 `신도유적`이다. 정적 조사만 수행했으며 코드·데이터·ROM은 추가로 수정하지 않았다.

## 2026-09-16 — HNS 전용 지역 목록 확인

- `hns_map_sections`의 ID를 `map_sections`와 대조하면 HNS에만 정의된 지역 ID는 62개다. 조토 마을 10개, 26~48번 도로 23개, 조토/HNS 시설·던전 29개로 구성된다.
- 같은 이름이 다른 ID에도 있을 수 있으므로 문자열이 아니라 MAPSEC ID 기준으로 분류했다. 예를 들어 `MAPSEC_ROCKET_HIDEOUT_HNS`/`MAPSEC_VICTORY_ROAD_HNS`와 `MAPSEC_SINJOH`/`MAPSEC_NEW_SINJOH`는 각각 이름이 중복된다.
- 이후 표기 수정 작업에서 `MAPSEC_SNOWSWEPT_CAVERN`의 JSON·조토 수동 표 띄어쓰기를 `설원동굴`로 통일했다.

## 2026-09-16 — `region_map_sections.json` 지역명 전체 한글화

- 요청/범위: `src/data/region_map/region_map_sections.json`의 `map_sections` 219개와 `hns_map_sections` 124개 항목에 들어 있는 `name` 342개를 공식 한국어 지역명으로 번역했다. 이름이 없는 `MAPSEC_DYNAMIC`은 동적 지역명 슬롯이므로 수정하지 않았다.
- 수정 파일: `src/data/region_map/region_map_sections.json`의 지역명과 `src/region_map.c:157-277`의 HNS 조토 수동 테이블 문자열을 함께 변경했다. 전자는 `make hns -j8`에서 자동 생성되는 `src/data/region_map/region_map_entries.h`에 반영됐으며 생성 헤더를 직접 편집하지 않았다.
- 표기 기준: 호연·관동·칠성제도는 프로젝트의 기존 한국어 지역명 데이터(`../pokeemerald-kr`)를 기준으로, 조토는 한국어 골드 지역명 데이터(`../골드 한글 텍스트/pokegold-kr/data/maps/landmarks.asm`)를 기준으로 맞췄다. 도로/수로, 도시·마을, 동굴·탑·섬·시설명과 알로라의 멜레멜레·아칼라·울라울라·포니 표기를 모두 반영했다. 신도·신도유적·낭떠러지동굴·빛남의 등대 등 하트골드·소울실버 공식 표기를 적용했고, 설원동굴·신도마을·매몰탑·49/50번 도로 등 나머지 HNS 추가 지역은 공식 게임에 없는 항목이므로 문맥에 맞는 한글 표기를 사용했다.
- 런타임 일관성: `GetActiveRegionMapEntries()`가 `FLAG_VISITED_KANTO`에 따라 자동 생성 테이블 또는 `sRegionMapEntries_Johto`를 선택하므로 두 경로의 동일 ID가 모두 한글을 반환하도록 맞췄다. 가장 긴 이름도 팝업 폭을 확인할 수 있도록 정적 문자열·글자 수를 점검했다.
- 검증: JSON 파싱, 지역명 342개 중 허용된 `{AQUA}` 제어 토큰을 제외한 ASCII 영문 잔존 여부, `src/region_map.c` 수동 테이블 영문 잔존 여부, `git diff --check`를 통과했다. `make hns -j8` 최종 재빌드 종료 코드 0; EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,516/33,554,432(99.31%), `pokehns.gba` 32MiB. 최종 로그: `build/localization-logs/hns-region-name-sinjoh-ko-20260916-r5.log`.
- 실제 게임 검증: 이 환경에는 mGBA 실행 파일이 없어 맵 워프 후 팝업의 중앙 정렬·긴 이름 잘림은 아직 확인하지 않았다. 다음 시작점은 도라지·연두·금빛시티, 조토·관동 도로, 알로라 지역을 각각 워프해 표시와 `FLAG_VISITED_KANTO` 전환을 확인하는 것이다.

## 2026-09-16 — 오버월드 마을·도로명 팝업 번역 경로 확인

- 스크린샷의 좌측 상단 창은 배틀 문자열이나 이미지 속 글자가 아니라, 맵 전환 때 `src/map_name_popup.c:556-775`가 `GetMapName()` 결과를 `FONT_NARROW`로 출력하는 런타임 지역명 팝업이다. 따라서 픽셀 편집기나 `.4bpp` 변환은 지역명 번역 자체에 필요하지 않다.
- HNS 지역명 입력은 `src/data/region_map/region_map_sections.json`의 `hns_map_sections`다. 이 JSON에서 `name`만 공식 한국어 명칭으로 바꾸고 `src/data/region_map/region_map_entries.h`는 직접 수정하지 않는다(자동 생성 파일). 또한 `src/region_map.c:157-277`의 `sRegionMapEntries_Johto`에도 같은 이름이 별도로 있으므로, `GetActiveRegionMapEntries()`의 플래그 분기(`:280-292`)에서 어느 테이블을 선택하든 번역이 보이게 두 곳을 함께 맞춰야 한다.
- 실제 문자열 복사는 `src/region_map.c:2303-2333`의 `GetMapName()`에서 수행된다. 특수기지 이름은 `src/secret_base.c:734-737`, 배틀 피라미드 층 이름은 `src/map_name_popup.c:508-527`에서 별도로 처리한다.
- 팝업 표시 여부는 각 맵의 `show_map_name=TRUE`(`data/maps/VioletCity_hns/map.json:13` 등)와 전환 경로(`src/overworld.c:2175-2179,2424-2427`)가 결정한다. `FLAG_HIDE_MAP_NAME_POPUP`가 켜져 있거나 같은 Map Section 재진입이면 표시되지 않을 수 있다. GEN_3 창은 `src/menu.c:424-433`의 10×3 타일(80×24픽셀)이고 중앙 정렬 폭도 80픽셀이므로 긴 한글 명칭은 실제 화면에서 잘림을 확인한다.
- 이번 세션은 코드·데이터를 수정하지 않은 정적 조사다. `rg`/줄 번호 대조만 수행했고 HNS ROM 빌드 및 mGBA 실제 화면 확인은 하지 않았다.
- 다음 시작점: 공식 한국어 지명 목록을 확정한 뒤 JSON의 `hns_map_sections`와 `sRegionMapEntries_Johto`의 문자열만 수정하고 `make hns -j8`로 자동 생성·빌드한 다음, 마을/도로 워프에서 팝업 표시·정렬·잘림을 mGBA에서 확인한다.

## 2026-09-16 — L=A 기술 설명 R 전환 및 L/R 스프라이트 재검증

- `GetBattleMoveDescriptionButton()`을 추가해 기본 설정은 L을 유지하고, `gSaveBlock2Ptr->optionsButtonMode == OPTIONS_BUTTON_MODE_L_EQUALS_A`일 때만 유효 기술 설명 버튼을 R로 전환했다. 기술 설명 창 열기/닫기 입력과 안내 스프라이트 선택이 같은 함수 결과를 사용한다.
- `B_MOVE_DESCRIPTION_BUTTON == L_BUTTON` 빌드에서 L/R `.4bpp`를 모두 포함하고, `TryToAddMoveInfoWindow()`가 일반 모드에서는 `move_info_window_l.4bpp`, L=A 모드에서는 `move_info_window_r.4bpp`를 로드하도록 수정했다. L=A에서 기존의 조기 `return`은 제거했다. START 기믹 선택 분기는 그대로 두었다.
- 사용자가 갱신한 `graphics/battle_interface/move_info_window_l.png`와 `move_info_window_r.png`를 재확인했다. 두 파일 모두 32×32, 8-bit colormap이며 번역된 `L/R / 기술 / 정보` 픽셀과 기존 테두리를 유지한다. 각 PNG를 `tools/gbagfx/gbagfx`로 변환한 `.4bpp`가 빌드 산출물과 바이트 단위로 일치한다.
- 검증: `make hns -j8` 종료 코드 0. 로그 `build/localization-logs/hns-move-info-l_equals_a-20260916.log`; EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,868/33,554,432(99.32%), `pokehns.gba` 33,554,432바이트. `git diff --check` 통과. 실제 mGBA 실행 파일이 없어 L=A 옵션의 런타임 화면은 미검증이다.
- 수정 경로: `include/battle_interface.h`, `include/config/battle.h`, `src/battle_interface.c`, `src/battle_controller_player.c`. 기존 파일의 별도 HNS/Champions 변경은 보존했다.
- 다음 시작점: mGBA에서 일반 버튼 모드의 L 팝업과 L=A 모드의 R 팝업을 각각 열어 버튼 아이콘, `기술 / 정보` 글자, R 입력과 마지막 사용 몬스터볼 기능의 충돌 여부를 확인한다.

## 2026-09-16 — START용 move info 스프라이트 사용 조건 확인

- `move_info_window_start.4bpp`는 `B_MOVE_DESCRIPTION_BUTTON`이 `R_BUTTON`이나 `L_BUTTON`이 아닌 경우(통상 `START_BUTTON`) 빌드되는 폴백 이미지다. 선택 조건은 `src/battle_interface.c:3121-3126`에 있다.
- 이 설정에서 기술 선택 화면의 START 입력은 `src/battle_controller_player.c:907-911`의 기술 설명 분기가 먼저 받아 설명 창을 열고 닫는다. 따라서 뒤의 `START_BUTTON` 기믹 선택 분기(`:913-921`)와 같은 입력을 공유하게 되어, 메가진화/Z 기술 선택용 START 동작과 함께 쓰면 충돌한다.
- 현재 HNS는 `B_MOVE_DESCRIPTION_BUTTON L_BUTTON`이므로 START용 이미지는 사용되지 않는다. START용을 활성화하려면 설정 변경·START 기능 충돌 검토·`move_info_window_start.png` 번역·`.4bpp` 재생성·HNS 재빌드가 모두 필요하다.

## 2026-09-16 — 기술 정보 팝업 스프라이트 번역 검수

- 사용자가 수정한 `graphics/battle_interface/move_info_window_l.png`를 확인했다. 32×32, 8-bit colormap 형식과 원본 16색 PLTE를 유지하면서 버튼 표시는 `L`, 안내 문자는 `기술 / 정보`로 변경되어 있다. 시각적으로 테두리와 글자 영역이 보존되며 한글 픽셀이 팝업 안에 배치된다.
- 수정 PNG에서 `tools/gbagfx/gbagfx`로 `graphics/battle_interface/move_info_window_l.4bpp`를 재생성했다. `.4bpp`는 생성 산출물이라 Git 추적 대상이 아니며, 다음 빌드에서 자동 재생성된다.
- `graphics/battle_interface/move_info_window_start.png`는 내용이 HEAD와 동일하고 `/mnt/c` 실행 권한 메타데이터만 변경되었다. 이 파일은 현재 `B_MOVE_DESCRIPTION_BUTTON L_BUTTON` 설정에서는 사용되지 않으며, START용까지 한글화하려면 별도 픽셀 수정이 필요하다.
- 검증: `make hns -j8` 종료 코드 0. 로그 `build/localization-logs/hns-move-info-sprite-20260916.log`; EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,292/33,554,432(99.31%), `pokehns.gba` 33,554,432바이트. 실제 mGBA 런타임 화면 검증은 아직 하지 않았다.
- 다음 시작점: 생성된 `pokehns.gba`를 mGBA에서 열어 기술 선택 화면 진입, L 버튼 팝업 표시, `기술 / 정보` 가독성·슬라이드 위치·테두리 겹침을 확인한다. START 버튼 설정을 사용할 경우 `move_info_window_start.png`도 같은 절차로 번역한다.

### 보충 — `move_info_window_r`와 `move_info_window_l`의 선택 시점

- 두 PNG는 런타임에 각각 출력되는 변형이 아니다. `src/battle_interface.c:3121-3126`의 전처리 조건이 `B_MOVE_DESCRIPTION_BUTTON` 값에 따라 `move_info_window_r.4bpp`, `move_info_window_l.4bpp`, 또는 `move_info_window_start.4bpp` 중 하나를 빌드에 포함한다.
- 현재 설정은 `include/config/battle.h:345`의 `B_MOVE_DESCRIPTION_BUTTON L_BUTTON`이므로 기술 선택 화면에서 L을 눌러 설명을 열 때 `move_info_window_l`만 사용된다. R은 현재 `B_LAST_USED_BALL_BUTTON R_BUTTON`으로 마지막 사용 몬스터볼 기능에 예약되어 있어 `move_info_window_r`는 출력되지 않는다.
- R용 이미지를 쓰려면 설정을 `R_BUTTON`으로 바꾸고 다시 빌드해야 하며, 현재 마지막 사용 몬스터볼의 R 입력과 충돌하므로 두 기능의 버튼을 분리해야 한다. START용도 동일하게 설정값을 `START_BUTTON`으로 정한 뒤 재빌드해야 한다.

### 보충 — move info 픽셀 편집기와 팔레트 보존 방법

- 권장 편집기는 Aseprite다. 픽셀 단위 Pencil, 인덱스 이미지, 고정 팔레트를 한 화면에서 다루기 쉽다. 무료 대안은 mtPaint(인덱스 팔레트 편집에 적합)이며 GIMP도 가능하지만 반드시 Indexed 모드를 유지해야 한다. 현재 WSL 셸에는 이 GUI 편집기 실행 파일이 설치되어 있지 않아 편집기 자체는 실행하지 않았다.
- 원본을 복사해 32×32 PNG를 열고 이미지 색상 모드가 Indexed인지 확인한다. `file graphics/battle_interface/move_info_window_l.png` 결과는 `8-bit colormap`이며 PNG `PLTE` 길이는 0x30(16색)이다. RGB/Grayscale로 변환하거나 색상 수를 재계산하지 않는다.
- Pencil 1px, 안티앨리어스·보간·브러시 smoothing을 끄고, 기존 팔레트 패널에서 배경/테두리/문자 색을 직접 선택한다. 새 RGB 색을 찍으면 팔레트 슬롯이 추가되거나 가장 가까운 색으로 양자화될 수 있으므로 사용하지 않는다. 팔레트 정렬·최적화·미사용 색 제거 옵션도 끈다.
- 픽셀 글리프는 런타임 한글 폰트를 불러오는 기능이 아니라 스프라이트 안에 직접 그리는 작은 비트맵 글자다. `graphics/fonts/font0_korean.png` 등의 8×8 한글 글리프를 참고하되, 팝업의 8×8 타일 경계와 영문 글자 영역에 맞춰 5×7 또는 6×7 단색 획으로 수작업 배치한다. 배경 픽셀은 원래 배경 인덱스로 지우고 테두리 픽셀은 건드리지 않는다.
- 저장 후에도 `file` 결과가 `8-bit colormap`인지 확인한다. `.4bpp`는 PNG의 시각적 RGB가 아니라 인덱스 순서로 생성되므로, `tools/gbagfx/gbagfx graphics/battle_interface/move_info_window_l.png graphics/battle_interface/move_info_window_l.4bpp`로 재생성한다. 생성 파일을 직접 색칠하거나 PNG를 RGB로 저장한 뒤 변환하지 않는다.
- 마지막으로 `make hns -j8`을 실행하고 mGBA 기술 선택 화면에서 L 버튼을 눌러 새 글리프의 가독성·팔레트·타일 경계를 확인한다. R/START용 변형을 번역할 때도 각각 원본 파일의 팔레트를 유지한 채 같은 절차를 반복한다.

## 2026-09-16 — 배틀 기술 정보 팝업 번역 방법 확인

- 기술 선택 화면 왼쪽의 `L / MOVE INFO` 표시는 `src/battle_message.c`의 런타임 문자열이 아니라 `graphics/battle_interface/move_info_window_l.png`에 직접 그려진 32×32, 8-bit colormap 스프라이트다. 현재 `include/config/battle.h`의 `B_MOVE_DESCRIPTION_BUTTON`이 `L_BUTTON`이므로 `src/battle_interface.c`는 이 PNG에서 생성되는 `move_info_window_l.4bpp`를 ROM에 포함한다.
- 번역할 때는 PNG의 테두리와 왼쪽 위 `L` 버튼 표시는 유지하고, `MOVE / INFO` 픽셀만 같은 팔레트 안에서 `기술 / 정보`처럼 두 줄로 다시 그린다. 소스 문자열이나 한글 charmap을 바꿔도 이 그림에는 반영되지 않는다. 32×32 전체 중 실제 글자 공간이 좁으므로 일반 한글 폰트를 그대로 축소하지 말고 2글자씩 읽히는 전용 픽셀 글리프를 사용해야 한다.
- PNG 수정 후 `tools/gbagfx/gbagfx graphics/battle_interface/move_info_window_l.png graphics/battle_interface/move_info_window_l.4bpp`로 파생 파일을 갱신하고 `make hns -j8`로 검증한다. 버튼 설정을 R 또는 START로 바꿀 계획이 있다면 각각 `move_info_window_r.png`, `move_info_window_start.png`도 같은 방식으로 번역해야 한다.
- 이번 작업은 위치·선택 경로·변환 절차를 확인해 안내한 조사 작업이며 그래픽은 아직 수정하지 않았다. 원본 PNG를 다시 변환한 `/tmp/move_info_window_l.4bpp`가 현재 추적 산출물과 바이트 단위로 일치함을 확인했다. 코드/그래픽 변경이 없어 ROM은 재빌드하지 않았고, 실제 게임 화면도 미확인이다.
- 다음 시작점: 최종 표기를 `기술 / 정보`로 확정한 뒤 `move_info_window_l.png`의 영문 픽셀만 수정하고 `.4bpp` 재생성, `make hns -j8`, 기술 선택 화면에서 L 버튼 팝업의 가독성 확인을 순서대로 수행한다.

## 2026-09-16 — 공식 HNS Release-v2.0.5 누락 복구 및 제목 화면 회귀 수정

- 공식 `PokemonHnS-Development/pokehns-expansion`의 `Release-v2.0.4`(`98574d2`)와 `Release-v2.0.5`(`1f42b74dff`)를 실제 파일 바이트로 대조했다. 릴리스 누적 변경은 282개 경로이며, 현재 트리에서 241개는 공식 2.0.5와 일치하고 41개는 HNS·한글 패치·1.17.0·메가진화 설정을 보존하기 위한 의도적 차이다. `Release-v2.0.4` 바이트가 남은 경로와 누락 경로는 0개다.
- 제목의 `graphics/title_screen/hns/press_start.png`가 2.0.4 해시(`1fa29…`, `START`)였기 때문에 화면에 2.0.4처럼 보였다. 공식 2.0.5 해시 `dd9231fd162a9e1ef6493e4ebe82f551520da052`(`PRESS START`)로 복구했다.
- 공식 변경 중 2.0.4 바이트로 남아 있던 171개 경로를 복구하고, 29개는 현재 HNS와 3-way 병합, 16개 충돌은 수동 검토했으며, 이미 일치하던 66개는 그대로 두었다. 그 결과 맵·타일·신조/알로라 콘텐츠·사파리/리매치 스크립트·배틀 수정·학습 목록·알/도감/AI/UI 변경을 현재 HNS 구조에 맞췄다. `src/daycare.c`의 1.17.0 알 생성 코드는 유지하면서 2.0.5의 Mirror Herb/베이비 폼 알기술, 초기 기술·이로치 롤, 너즐록 부화 차단을 통합했고, `all_learnables.json`은 현재 목록에 공식 추가 기술만 합쳤다.
- 한글·HNS 보호 대상인 `src/battle_message.c`, `src/strings.c`, `src/oak_speech_hns.c`, `src/challenge_menu.c`의 문구와 기존 `LOCK_ONEWAY_DOWN`, `include/config/battle.h`의 `B_MOVE_DESCRIPTION_BUTTON L_BUTTON`, `include/config/pokemon.h`의 메가진화 활성화·메가 울음소리 고음 피치 설정, Soulgold/Gold/Oak 그래픽은 덮어쓰지 않았다. 현재 41개 의도적 차이에는 이러한 파일과 HNS 전용 리매치·상수·1.17.0 API가 포함된다.
- 검증: 변경된 PNG의 `.4bpp`·`.smol`·`.gbapal`·맵 타일 파생물을 강제로 재생성한 뒤 `make hns -j8` 종료 코드 0. 최종 로그 `build/localization-logs/hns-205-repair-20260916-r4.log`; 링크 메모리 EWRAM 249,012/262,144(94.99%), IWRAM 25,704/32,768(78.44%), ROM 33,324,292/33,554,432(99.31%), 생성된 `pokehns.gba`는 33,554,432바이트(32MiB)다. 맵 JSON 변경 뒤 `ViridianForest_hns/events.inc`도 재생성해 링크 오류를 제거했다.
- 원인 정정: 이전 2.0.5 확인은 최신 커밋의 제목 파일만 보고 누적 2.0.4 변경을 다시 남긴 상태를 놓쳤다. 이번에는 릴리스 간 전체 282개 경로와 1.17.0 직전 체크포인트를 함께 대조했다. 공식 출처는 [HNS Release-v2.0.5](https://github.com/PokemonHnS-Development/pokehns-expansion/releases/tag/Release-v2.0.5)다.
- 미검증: 실제 mGBA에서 제목 화면·맵/사파리·알 부화·메뉴/배틀 런타임을 아직 확인하지 않았다. 다음 시작점은 생성된 ROM으로 해당 화면을 순서대로 확인하는 것이다.

## 2026-09-16 정정 2차 — 1.17.0 이식 직전 한글화·HNS 회귀 및 화면 문제 복구

- 사용자가 지정한 기준점은 `6da0a16d66d3d6a2691c775cc23780f994512ab5`(1.17.0 이식 질문 직전 보존 체크포인트)로 확정했다. 현재 트리를 이 기준과 파일 내용·바이트 단위로 다시 대조했으며, 이전에 “복구 완료”라고 보고한 내용 중 실제로 남아 있던 회귀를 인정하고 바로잡았다.
- 메가진화가 꺼진 직접 원인은 `include/config/species_enabled.h`의 `P_MEGA_EVOLUTIONS`·`P_PRIMAL_REVERSIONS`가 `FALSE`로 덮인 것이었다. 두 값을 `TRUE`로 되돌렸다. 별도 메가 울음소리 샘플은 사용하지 않는 기존 정책(`P_MODIFIED_MEGA_CRIES FALSE`, `CRY_MODE_HIGH_PITCH`)을 유지한다.
- 오박사 인트로 색상 오류는 `graphics/oak_speech/oak/{pal.pal,pic.png}`와 `oak_speech_bg.{bin,png}`가 기준점과 다른 원본으로 바뀐 탓이었다. 기준 원본을 복구하고 `make hns`에서 `.gbapal`, `.4bpp.smol`, `.smolTM` 산출물을 재생성했다. HNS 중복 원본 `graphics/oak_speech_hns/oak/{pal.pal,pic.png}`도 기준 바이트로 복구했다.
- `심향이로구나?`에서 조사가 빠진 원인은 `src/string_util.c`에서 `korean.h`·`gJongCode`·이전 글자 판정·조사 플레이스홀더 함수/테이블이 통째로 사라졌기 때문이다. 기준 코드 그대로 복원했으며 `data/text/oak_speech_hns.inc`의 `{PLAYER}{K_I}` 문장은 변경하지 않았다.
- 챌린지 설정 추천 글자 정렬은 `src/challenge_menu.c`의 임의 `leftX = 74` 분기를 제거하고 기준값 `104`를 사용하도록 되돌렸다. 현재 사용자의 최신 난이도 잠금 정책(`LOCK_ONEWAY_DOWN`)은 좌표와 무관하므로 유지했다.
- 1.17.0 이식 중 빠진 한글 텍스트도 추가 복구했다. `src/data/items.h`의 기존 한글 복수형 183개, `src/strings.c`·`include/strings.h`의 도감 검색 자모 `ㄱㄴ`~`ㅍㅎ`, `src/strings.c`의 미사용 `카운트`를 살렸다. 혼란 열매 hold effect 통합으로 발생한 최신 API 불일치 5곳은 `HOLD_EFFECT_CONFUSE_FLAVOR`와 `secondaryId`를 사용하도록 수정했으며 기존 한글 열매명은 그대로 두었다.
- 도감 울음소리 화면도 한글 의미가 유지되도록 `gText_CryOf1`(`의`)·`gText_CryOf2`(`울음소리`)를 복원하고 `src/pokedex.c`·`src/pokedex_plus_hgss.c` 호출부를 각각 조사/제목에 맞췄다. upstream의 단일 `gText_CryOf`로 합쳐져 종명 뒤에 `울음소리`가 붙던 회귀를 제거했다.
- 대조 결과 `src/battle_message.c`, `src/data/types_info.h`, `src/oak_speech_hns.c`, `data/text/oak_speech_hns.inc`, Gold 남자 주인공 전·후면/필드 스프라이트는 기준점의 한글/HNS 내용을 유지하고 있었다. 1.17.0과 무관한 현재 타이틀 `press_start.png` 변경은 별도 HNS 2.0.5 작업으로 확인되어 건드리지 않았다.
- 검증: `make hns -j8` 종료 코드 0. 링크 메모리 EWRAM 249,012/262,144 bytes(94.99%), IWRAM 25,704/32,768 bytes(78.44%), ROM 33,322,644/33,554,432 bytes(99.31%); `pokehns.gba`는 33,554,432바이트(32MiB)다. 로그: `build/localization-logs/hns-regression-repair-20260916.log`.
- 알려진 문제: 빌드에는 기존 미사용 코드 경고와 `src/string_util.c`의 겹치는 placeholder 지정 초기화(`0x0E`) 경고가 남지만 종료 코드 0이다. 실제 mGBA에서 오박사 팔레트·조사·챌린지 좌표·메가진화 동작을 재현하는 화면 검증은 아직 하지 않았다.
- 원인: 1.17.0 임시 병합을 파일 단위가 아닌 넓은 범위로 적용하면서 HNS 전용 그래픽/설정과 번역 파일이 upstream 쪽으로 덮였고, 이후 복구 패치 방향을 반대로 적용해 일부 사용자 번역이 다시 영어로 바뀌었다. 이번에는 해당 시점 체크포인트를 기준으로 실제 파일을 대조해 필요한 항목만 복구했다.

## 2026-09-16 정정 — 1.17.0 이식 직전 HNS 변경 회귀 복구

- 이전 그래픽 감사의 결론을 정정한다. 당시에는 Soulgold 저장소와 현재 파일만 비교했기 때문에, 1.17.0 이식 직전의 실제 HNS 작업 트리와 달라진 Gold 남자 주인공 스프라이트를 발견하지 못했다. 사용자가 지적한 회귀가 맞으며, “같다”는 이전 보고는 잘못이었다.
- 1.17.0 작업 직전 보존 체크포인트를 기준으로 `graphics/trainers/front_pics/gold_hns.png`와 `graphics/trainers/back_pics/gold_hns.png`를 정확히 복구했다. 현재 전면 파일 해시는 `caa547164c53d2a5f7e40529483ccfa40a233c5c`, 후면 파일 해시는 `e0c87fbc5417588f68fd81191ad2976cc58d35f4`이며, 전면·후면 팔레트(`graphics/trainers/palettes/gold_hns.pal`, `graphics/trainers/back_pics/gold_hns.pal`)도 같은 기준으로 복구해 `.gbapal` 산출물을 다시 만들었다. 따라서 첨부 화면의 왼쪽 HNS 전용 남주 스프라이트와 색상이 기준이다.
- 같은 체크포인트와 VS Code 사용자 기록을 교차 대조해 1.17.0과 무관하게 영어로 덮인 번역·HNS 코드를 복구했다. 대상은 `include/config/battle.h`의 기술 설명 버튼(`L_BUTTON`), `src/data/types_info.h`의 타입명, `src/data/items.h`의 도구명, `src/data/pokemon/species_info/gen_8_families.h`의 종명·분류명, `src/data/script_menu.h`, `data/text/oak_speech_hns.inc`, `src/oak_speech_hns.c`, `src/strings.c`의 포켓/메뉴 명칭, `src/trainer_card.c`의 HNS 카드 분기다. 사용자 한글 문장을 새로 번역하지 않고 기준 파일의 원문을 그대로 되살렸다.
- `src/battle_message.c`는 사용자 VS Code 기록과 전체 파일이 이미 일치해 변경하지 않았고, `src/challenge_menu.c`도 번역문을 덮어쓴 회귀가 없어 건드리지 않았다. HNS 고유 그래픽과 Soulgold UI 자산의 앞선 복구도 유지했다.
- 후속 전체 대조에서 발견한 HNS 전용 코드·데이터 회귀도 추가 복구했다. `src/battle_setup.c`(리매치·스이쿤 전투음·Whirl Islands 환경·HG 트레이너 음악), `src/trainer_hill.c`, `src/safari_zone.c`, `src/starter_choose.c`, `src/event_object_movement.c`, `src/sound.c`, `src/save.c`, `src/credits_hns.c`, `src/data/trainers_hns.party`, `include/constants/{sound,help_window,safari_zone,trainers,vars_hns,flags_hns,opponents_hns}.h`, `include/graphics.h`, `src/data/help_window.h`, HNS 사파리/신조/비리디안 숲/실버산/버밀리온/루트28 등의 맵 스크립트·이벤트를 1.17.0 직전 기준으로 되돌렸다. 이 과정에서 기존 HNS 라벨과 팬페어/도움말 ID가 다시 연결되도록 복원했다.
- 검증: 팔레트와 HNS 전용 코드 복구 후 `make hns -j8` 성공. EWRAM 248,960/262,144(94.97%), IWRAM 25,704/32,768(78.44%), ROM 32,914,532/33,554,432(98.09%); `pokehns.gba`는 33,554,432바이트(32MiB)다. 최종 로그: `build/localization-logs/hns-translation-regression-restore-20260916-r6.log`. 링크 오류는 없었고, 첨부 mGBA 화면의 왼쪽 스프라이트와 현재 PNG/팔레트를 대조했으며, 실제 ROM에서 전체 메뉴·배틀 런타임 검증은 남아 있다.

## 2026-09-16 이어받기 — 1.17.0 이식 후 HNS/Soulgold 그래픽 보존 점검

- 사용자가 지정한 HNS 기본 그래픽을 Soulgold 원본과 직접 대조했다. 실제로 기본 HNS로 돌아가 있던 `graphics/battle_interface/hns/textbox.*`, Soulgold와 색상·타일이 다른 `graphics/text_window/1..20`·`message_box`·`name_box`, 그리고 `graphics/trainers/front_pics/champion_lance_hns.*`를 Soulgold 원본과 변환 산출물로 복원했다.
- Gold 남자 주인공의 HNS 전용 전면·후면·필드 스프라이트(`graphics/trainers/*gold_hns*`, `graphics/object_events/pics/people/gold/*`)는 Soulgold에 동일한 HNS 슬롯 원본이 없으므로 기존 HNS 자산을 보존했다. `include/constants/trainers.h`, `src/data/graphics/trainers.h`, `src/graphics.c`, `src/text_window.c`의 HNS 선택 연결은 변경하지 않았다.
- 복원 검증: 배틀 텍스트 창 10개 파일, 텍스트 창 1~20번의 PNG/4bpp/GBA 팔레트, 공통 `message_box`·`name_box`, 목호 PNG/4bpp/SMOL/GBA 팔레트와 JASC 팔레트가 각각 `../soulgold`와 바이트 단위로 일치한다. `/mnt/c` Windows 권한 표시로 일부 추적 그래픽이 `100755`로 보이는 메타데이터 차이는 ROM 내용과 무관하다.
- 추가 자산 감사(이전 결과, 위 정정으로 일부 대체): 당시 Gold PNG의 색상 팔레트 회귀를 놓쳤으므로 “현재 파일과 일치” 및 “추가 자산 없음”이라는 결론은 폐기한다. 위 정정 항목에서 Gold 전면·후면 PNG와 팔레트까지 1.17.0 직전 기준으로 다시 맞췄다. `graphics/battle_interface/move_info_window_start.png`와 `src/text_window.c`의 남은 변경 표시는 내용이 아닌 `/mnt/c` 실행 권한 메타데이터다.
- 추가 점검에서 보존 스냅샷과 다른 `graphics/title_screen/hns/press_start.*`는 1.17.0 이식이 아니라 HNS 2.0.5 타이틀 화면 갱신(커밋 `1f42b74dff`)으로 확인되어 유지했다. `src/battle_controller_player.c`의 상성 아이콘과 `src/battle_interface.c`의 상대 HP 백분율 분기는 이미 선별한 1.17.0 기능이므로 유지했다.
- 원인: 이전 1.17.0 범위 병합에서 `graphics/`와 생성 자산을 후보로 취급해 HNS 전용 자산이 기본 자산으로 덮일 수 있었다. 이번에는 Soulgold 원본이 존재하는 파일만 명시적으로 되돌리고 Gold HNS 전용·한글 관련 자산은 건드리지 않았다.
- 검증: `make hns -j8` 성공. EWRAM 247,472/262,144(94.40%), IWRAM 25,704/32,768(78.44%), ROM 32,885,796/33,554,432(98.01%); `pokehns.gba`는 33,554,432바이트(32MiB)다. 로그: `build/localization-logs/hns-soulgold-graphics-restore-20260916.log`. 실제 에뮬레이터 화면에서 창 모양·Gold·목호 위치를 확인하는 작업은 아직 남아 있다.

## 2026-09-16 이어받기 — 1.15.1 이후 6개 릴리스 범위 점검 및 아이템 호환성 정리

- 공식 [pokeemerald-expansion 릴리스 목록](https://github.com/rh-hideout/pokeemerald-expansion/releases)의 1.15.1 이후 1.17.0까지를 Pokémon·Battle General·Moves·Abilities·Items·Battle AI 범위로 대조했다. 기존 선별 이식분(특히 1.17.0 Items·Battle AI, Champions 배틀, 알/GEN_CHAMPIONS/메가 관련 변경)은 유지했다.
- 이 범위의 전체 파일을 한 번에 덮어쓰는 방식은 사용하지 않았다. 1.15.2→1.17.0에는 구조 개편으로 충돌 파일 263개가 있고, 현재 한글 배틀 문자열·HNS 분기와 upstream API가 얽혀 있어 일괄 적용 시 번역 파일과 빌드 호환성이 손상된다. 임시 병합분은 되돌렸으며, 따라서 이번 항목 전체가 1.17.0 기준으로 완료되었다고 기록하지 않는다. 남은 릴리스별 변경은 PR/기능 단위로 계속 이식해야 한다.
- 기존에 반영되어 있던 혼란 열매 hold effect 통합과 새 `SetTypeBeforeUsingMove` API가 서로 맞도록 `HOLD_EFFECT_CONFUSE_FLAVOR` 열거값, 전투 효과 처리, 디버그 표시, 다이맥스 호출부를 정리했다. 한글 문자열·한글화 에셋은 수정하지 않았다.
- 검증: `make hns -j8` 종료 코드 0. EWRAM 247,472/262,144 bytes(94.40%), IWRAM 25,704/32,768 bytes(78.44%), ROM 32,885,700/33,554,432 bytes(98.01%). 로그: `build/localization-logs/hns-expansion-sync-20260916-finalcheck2.log`.
- 32MiB ROM 영역과 `P_MODIFIED_MEGA_CRIES FALSE`/`CRY_MODE_HIGH_PITCH`를 유지했다. 메가진화 울음소리 샘플·사운드 자산은 추가하지 않았다. 실제 에뮬레이터/게임 화면 검증은 아직 하지 않았다.
- 다음 시작점: 공식 릴리스의 남은 Pokémon·Battle General·Moves·Abilities·Items·Battle AI PR을 의존성 순서대로 하나씩 대조하고, 충돌 가능성이 있는 `src/battle_message.c`, `src/strings.c`, HNS 텍스트/에셋은 계속 제외한다.

## 2026-09-16 이어받기 — 1.17.0 PR #10121 디버그 메뉴 이식

- 공식 `expansion/1.17.0`의 PR #10121 커밋 `9df1444765521683907f19757155d3d2d3668ae0`에서 HNS 구조와 대응되는 디버그 기능만 수동 이식했다. 현재 HNS `src/debug.c`는 대규모 랜덤화·대량발생·기타 기능이 이미 분기된 상태라 upstream 파일 전체를 덮어쓰지 않고, 기능 단위로 병합했다.
- Give Pokémon Complex에 성별 선택 단계를 추가했다. 성별 값으로부터 실제 성별을 계산해 `GetMonPersonality`에 전달하므로, 생성된 포켓몬의 성별이 선택값과 일치한다. 종·레벨·성별·이로치·성격·특성·테라 타입·다이맥스·거다이맥스·IV·EV·기술 선택 중 B 버튼을 누르면 직전 단계로 돌아가도록 연결했다.
- 현재 HNS의 다른 다단계 디버그 선택에도 같은 되돌리기 동작을 적용했다. 워프의 그룹/맵/워프 번호, 아이템의 도구/수량, 변수의 변수/값, 포커러스의 strain/기간 선택에서 중간 단계 취소가 가능하다. 첫 단계의 B 버튼은 기존처럼 메뉴를 닫는다.
- PR에 포함된 `src/pokemon_icon.c`의 `usingSheet` 애니메이션 분기도 이식했다. 기존 아이콘 복사 경로는 그대로 두고 시트 사용 시 타일 번호만 갱신한다. 한글 문자열 파일(`src/battle_message.c`, `src/strings.c`, HNS 텍스트)은 변경하지 않았고, `battle_message.c`는 VS Code 로컬 기록 `jHTf.c`와 계속 일치한다.
- PR의 `ResolveEVs` 공개 선언·함수 변경은 현재 HNS 소스에 대응 구현이 없고 복합 포켓몬 메뉴가 기존 EV 합계 검증 경로를 사용하므로, 죽은 선언만 추가하지 않고 기존 EV 동작을 유지했다.
- 검증: `make hns -j8` 성공. EWRAM 247,472/262,144 bytes(94.40%), IWRAM 25,704/32,768 bytes(78.44%), ROM 32,885,700/33,554,432 bytes(98.01%). 로그: `build/localization-logs/hns-debug-10121-20260916.log`. `git diff --check -- src/debug.c src/pokemon_icon.c` 통과.
- 32MiB ROM 및 메가 울음소리 기본 고음 피치 설정은 유지했다. upstream의 전체 generic selection 구조를 HNS에 기계적으로 덮어쓰지는 않았으므로, HNS에 존재하지 않는 upstream 전용 디버그 선택 항목은 이식 대상에서 제외했다. 실제 게임/에뮬레이터에서 각 B 버튼 경로를 확인하는 것은 남아 있다.

## 2026-09-16 이어받기 — 64MiB 확장 철회 및 32MiB 복구

- 공식 `expansion/1.17.0` 태그(`e8bd1cd7b03fc032ea37e3ecd38b379b5d01a1e7`)를 기준으로 전체 동기화 가능 여부를 점검했다. 1.15.2→1.17.0은 764개 커밋 규모이며 구조 개편 충돌 173개가 남아 있어, clean 파일을 일괄 덮어쓰지 않고 임시 병합과 신규 구조체 파일을 제거했다. 이미 선별 이식된 1.17.0 기능은 유지하며 전체 동기화 완료로 기록하지 않는다.
- 사용자가 64MiB ROM을 원하지 않아 이번 세션의 메가 울음소리 변경을 철회했다. `P_MODIFIED_MEGA_CRIES`를 `FALSE`로 복구하고 `P_MODIFIED_MEGA_CRY_MODE CRY_MODE_HIGH_PITCH`를 유지했다. 1.17.0에서 교체했던 메가 울음소리 WAV 26개, Porygon 역방향 조건, PCM 정렬 변경도 작업 전 상태로 되돌렸다. 따라서 메가진화는 다시 기본 울음소리의 고음 피치 효과를 사용한다.
- `ld_script_modern.ld`의 ROM 영역을 32MiB로 복구했다. `make hns -j8` 성공: EWRAM 247,472/262,144(94.40%), IWRAM 25,704/32,768(78.44%), ROM 32,883,988/33,554,432(98.00%). 로그: `build/localization-logs/hns-revert-mega-cries-20260916.log`.
- 한글 관련 파일은 변경하지 않았다. `src/battle_message.c`는 VS Code 로컬 기록 `jHTf.c`와 계속 일치하며, `src/challenge_menu.c`, `src/strings.c`, HNS 텍스트도 이번 철회에서 건드리지 않았다.
- 다음 시작점: 32MiB 영역을 유지한 채 1.15.2→1.17.0 충돌 173개를 PR/기능 단위로 수동 이식한다. 각 단위는 한글 문자열 파일을 보호하고 `make hns -j8`를 별도로 검증한다.

## 지금 이어갈 일

- **완료 — 1.17.0 이식 중 소실된 한글 문자열 복구:** 1.17.0/HNS 통합 전 백업 `build/localization-backups/pre-hns-205-20260914-112129.tar.gz`와 현재 파일을 대조해, 영어로 롤백된 `src/challenge_menu.c`와 `src/battle_message.c`의 한글 표시 문자열을 복구했다. `challenge_menu.c`의 최신 게임 모드 선택지 좌표 보정(`DrawChoices_Two`의 `leftX`)은 유지했고, 기능 코드·제어 코드·조사 토큰은 백업에서 복구한 문장에 맞춰 보존했다. `src/oak_speech_hns.c`는 구형 백업 코드로 덮어쓰지 않았다. 실제 오박사 대사는 현재 `data/text/oak_speech_hns.inc`에 이미 한글로 남아 있고, 현재 소스의 RTC·성별 선택 흐름은 더 최신이기 때문이다.
  - 사용자가 확인한 `src/battle_message.c:566-594`는 VS Code 서버 로컬 기록 `/home/tk_pc/.vscode-server/data/User/History/ed9b1d2/jHTf.c`에서 실제 한글 원문을 찾아 그대로 복구했다. 통합 전 tar 백업에는 이 범위가 영어였지만, 로컬 기록의 조사·제어 토큰과 줄바꿈까지 원문 그대로 대입했으며 문자열 범위 밖 코드는 건드리지 않았다.
- 원인 확인: 이전 복구 과정에서 `diff -u 현재파일 백업파일` 방향의 패치를 적용해, 2026-09-15 20:09 KST에 백업의 영어 566–594번이 사용자 작업을 덮어썼다. 이는 사용자 변경이 자연 소실된 것이 아니라 잘못된 복구 패치로 인한 손실이다.
  - 추가 확인·복구: VS Code 로컬 기록 `jHTf.c`와 현재 `src/battle_message.c`를 전체 파일로 대조한 결과, 해당 기록과 현재 파일이 완전히 일치하도록 메가진화 문구, 링크 상대 이름 처리, 조사 처리 코드, 배틀 메뉴 좌표까지 복원했다. `src/strings.c`도 로컬 기록 `oqPX.c`와 공통인 1,201개 리터럴을 현재 심볼·구조를 유지한 채 사용자 한글 값으로 되돌렸고, 한글 조사 플레이스홀더 15개 정의와 선언을 복구했다. 기록에만 있는 구형/미사용 심볼 10개는 현재 expansion 구조에서 참조되지 않아 추가하지 않았다.
  - `src/challenge_menu.c`도 VS Code 로컬 기록 `t2fy.c`(2026-09-11 22:21 KST)와 대조했으며, 차이는 최신 게임 모드 선택지 좌표를 보존하기 위한 `DrawChoices_Two`의 `leftX` 계산 한 곳뿐이다. 메뉴의 사용자 한글 문구는 기록과 일치한다.
  - 조사 토큰 충돌 복구: `include/battle_message.h`에서 `FD35–FD3B`를 `B_TXT_EUNNEUN`~`B_TXT_AYA` 전용으로 복원하고 기존 전투 토큰을 뒤로 이동해 `charmap.txt`와 파서 ID를 맞췄다. 임시로 만든 번역 문구는 남기지 않았다.
- 추가 번역 후보는 의도적으로 영어로 남겼다. `src/battle_message.c:97-98`(FRLG 전용 GHOST 문구), `:124`(미사용 교체 문구), `:191`(미사용 지형 문구), `:595-889`(아직 한글화하지 않은 `gBattleStringsTable` 배틀 문구), `:1427`(HNS에서 사용하지 않는 FRLG 사파리 메뉴), `:1478`(Battle Frontier `vs`)가 대상이다. `src/challenge_menu.c:337-343,514,524,544-626,1512`의 `OFF`·`ON`·수치·배율은 기능값/짧은 선택지로 남겨 두었다. 이 항목들은 이번 복구에서 새로 번역하지 않았다.
- 정확한 사용자 기록 대조 결과: `src/battle_message.c` 전체가 VS Code 로컬 기록 `jHTf.c`와 일치한다. 따라서 사용자가 번역했던 566–594번만이 아니라 메가진화 문구, 링크 상대 이름 처리, 조사 처리 코드, 배틀 메뉴 좌표를 포함해 해당 기록 시점의 전체 파일을 복구한 상태다. `src/strings.c`는 로컬 기록 `oqPX.c`와 공통인 1,201개 리터럴을 사용자 한글 값으로 복구했고 조사 플레이스홀더 15개와 `include/strings.h` 선언도 복원했다. 기록에만 있는 구형/미사용 심볼 10개는 현재 expansion 구조에서 참조되지 않아 추가하지 않았다.
- 조사 토큰 정합성: `include/battle_message.h`의 `FD35–FD3B`를 `B_TXT_EUNNEUN`~`B_TXT_AYA`로 맞추고 기존 전투 토큰을 뒤로 이동해 `charmap.txt`와 파서 ID 충돌을 제거했다. 임의로 새로 만든 번역 문구는 남기지 않았다.
- 최종 검증: 조건부 문자열 분기까지 복구한 뒤 `timeout 360s make hns -j8` 종료 코드 0(로그 `build/localization-logs/hns-restore-user-strings-20260915.log`). 링크 메모리는 EWRAM 247,472/262,144 bytes(94.40%), IWRAM 25,704/32,768 bytes(78.44%), ROM 32,884,068/33,554,432 bytes(98.00%)이다. 대상 코드의 `git diff --check`도 통과했다. 실제 mGBA 화면 검증은 아직 하지 않았다.

- **완료 — 1.17.0 Items·Battle AI 선별 이식:** 공식 `expansion/1.17.0` 릴리스의 Items 항목 중 #10430(희귀사탕·경험사탕 사용 후 메뉴 유지), #10163(혼란 열매 hold effect 통합), #10592(에니그마베리 hold effect 정리)를 현재 HNS 자료·아이템 API에 맞춰 반영했다. #8893의 영문 문법 대개편은 기존 한글 배틀 문구를 덮어쓰지 않기 위해 이식하지 않았다. Battle AI 항목은 #10243(능력·도구·기술 지식 플래그 분리), #10258(반동 자폭 회피), #10236(두 번 맞는 드래곤다트), #8647(Round·Pledge·Fusion 연계와 턴 순서), #10046(자기 자신 점수화 생략), #10145(가상 능력·도구 컨텍스트), #10427(Wrap 잔여 피해 고려), #10277(사고 시간·명중률 계산), #10453(AI 대미지 계산 구조체), #10461(선택 결과·대상 처리 정리), #10193(명중률 API), #10212(`gCurrentMove` 의존 제거), #9448(파트너 유익 공격을 막는 Protect 회피), #10626(Palafin 교대), #10669(Mind Reader·Lock-On 반복 억제), #10688(Fusion 턴 순서), #10700(확정 치명타 단계)을 적용했다. #10610의 HNS 대응 항목(교체 계산 중 Stomping Tantrum, 최신 필드 상태 접근)도 반영했다. #10464의 구형 `MOVE_EFFECT_STAT_PLUS` 분기/통계 함수는 현재 HNS에 해당 경로가 없어 기존 최신 능력치 점수 로직을 보존하고 Sturdy 설정 조회만 최신화했다. 기존 한글 종명·특성명·기술명·배틀 텍스트는 변경하지 않았다.
  - `make hns -j8` 종료 코드 0. 링크 메모리는 EWRAM 247,472/262,144 bytes(94.40%), IWRAM 25,704/32,768 bytes(78.44%), ROM 32,884,516/33,554,432 bytes(98.00%)이며 로그는 `build/localization-logs/hns-expansion-117-items-ai-20260915.log`다.
  - 코드·데이터 정적 검사는 대상 `git diff --check` 통과. 실제 mGBA/게임 화면 검증은 아직 하지 않았다. 다음 시작점은 더블 배틀에서 Round·Pledge·Fusion·Dragon Darts·Protect 연계와 반동/Palafin AI를 실제 전투로 확인하는 것이다.

- **완료 — 1.17.0 Champions 배틀 메커니즘·Fixed 선별 이식:** 공식 `expansion/1.17.0` 태그(`e8bd1cd`)에서 #10151(Champions 수면 난수), #10025(상대 HP 백분율 표시), #10145(AI 계산 시 가상 특성·도구 컨텍스트 사용 및 Supreme Overlord 카운터 보존), #10257(Champions 기술·교체 카운터), #10324(메가/원시회귀 파티클 팔레트), #10591(Howl 대타출동 무시), #10541(효과 배율 추가 아이콘)을 현재 HNS 구조에 맞춰 반영했다. #10581(세이브 글꼴 초기화), #10597(도감 획득 전 스타터 이로치 제한), #10612(레벨업 중복 기술), #10614(다중 레벨업) Fixed 수정도 적용했다. 기존 한글 텍스트는 변경하지 않았다. #9265는 테스트 전용이라 복사하지 않았고, #10220(취소자 전투 구조)·#10426(흡수/SetMoveEffect 전면 리팩터링) 및 HNS에 구조적으로 대응하지 않는 #10142·#10211·#10523·#10527·#10656은 기존 HNS 동작을 보존하며 기계적 이식을 생략했다. 상대 HP 백분율은 `B_HP_PERCENTAGE_DISPLAY FALSE`로 기본 비활성화해 기존 한글 배틀 UI를 보존한다. 효과 아이콘용 문자·폰트 폭과 공식 폰트 PNG 자산도 갱신했다. `make hns -j8` 성공(로그 `build/localization-logs/hns-expansion-117-champions-mechanics-20260915.log`, ROM 32,882,804 bytes/98.00%, EWRAM 247,472 bytes, IWRAM 25,704 bytes), 실제 에뮬레이터·게임 화면 검증은 아직 하지 않았다.

- **완료 — pokeemerald-expansion 1.17.0 선별 이식:** 공식 `expansion/1.17.0` 태그(`e8bd1cd`)에서 PR #9878(알 생성·부화와 `givemon` IsEgg), #10058(`GEN_CHAMPIONS`·기술 데이터), #10561(메가찌르호크 아이콘), #10603(Z-A 메가 몸색), #10601(포켓덱스 플러스 진화 텍스트 정렬), #10416(메가솔 특성 팝업)을 현재 1.15.2-development 기반 HNS에 반영했다. Champions 메가 특성 6종을 추가하고 메가몰드류·메가장크로다일·메가저리더프·메가니움·메가화염레오·메가스코빌런에 매칭했으며 표시명을 관통드릴·드래곤스킨·천정부지·메가솔라·불꽃의갈기·하바네로분출로 번역했다. HNS에 없는 `INCGFX_U8`·배틀 회복 라벨·포켓몬 애니메이션 테이블은 기존 HNS 자산/스크립트로 호환시켰다. `make hns -j8` 성공(로그 `build/localization-logs/hns-selected-expansion-117-20260915-r20.log`, ROM 32,881,796 bytes/98.00%, EWRAM 247,472 bytes, IWRAM 25,704 bytes), 실제 에뮬레이터·게임 화면 검증은 아직 하지 않았다.

- **현재 pokeemerald-expansion 기반 버전 확인 완료:** 소스의 `include/constants/expansion.h`는 `1.15.2`를 가리키고 `EXPANSION_TAGGED_RELEASE FALSE`로 정의한다. Git 계보에도 1.15.1 릴리스 커밋 `b6c71d33bb`/`fcecfadda6`과 이후 `Start of 1.15.2 cycle` 커밋 `1b79bd80f9`가 모두 현재 HEAD `d7d3194fa1`의 조상이다. 따라서 현재 HNS에 포함된 기반은 **pokeemerald-expansion 1.15.2 개발 사이클(미태그/미릴리스), 직전 정식 릴리스는 1.15.1**로 기록한다. `README.md`의 “based off ... 1.15.1”은 `TODO` 아래의 예시이며 현재 버전 상수보다 이전 정보다. 별개로 HNS 소스는 공식 HNS `Release-v2.0.5` 변경분이 미커밋 작업 트리에 통합된 상태다. 버전 조사는 읽기 전용이었으므로 ROM 빌드는 실행하지 않았다.

- **무한스프레이 명칭 변경 완료:** `INFIN. REPEL`의 이름을 사용자 요청에 따라 `무한스프레이`로 변경했다. `make hns -j8` 종료 코드 0, 로그 `build/localization-logs/hns-infinite-spray-20260914.log`, ROM 사용 33,284,804 bytes (99.20%), EWRAM 249,304 bytes, IWRAM 25,704 bytes. `pokehns.gba`는 2026-09-14 18:44:28 KST에 갱신됐다.

- **특성·도구 이름 한국어화 완료, 화면 확인 필요:** `src/data/abilities.h`의 실제 특성 310개(전체 311개 슬롯에서 `ABILITY_NONE`은 `-------` 유지)와 `src/data/items.h`의 도구 이름·복수형 1,085개를 PokeAPI 한국어 데이터와 공식 한국어 표기로 교체했다. `make hns -j8` 종료 코드 0, 로그 `build/localization-logs/hns-ability-item-names-20260914.log`, ROM 사용 33,284,804 bytes (99.20%), EWRAM 249,304 bytes, IWRAM 25,704 bytes. 긴 이름은 컴파일러 길이 검사를 통과했으며 mGBA에서 팝업·가방·상점의 실제 폭과 잘림을 확인해야 한다.

- **배틀 명령 메뉴 오른쪽 첫 글자 가림 수정·빌드 완료, 화면 재확인 필요:** HNS 화면에서 `가방`의 `가`, `도망간다`의 `도`가 보이지 않았다. 오른쪽 열 시작 `{CLEAR_TO 48}`이 커서 타일의 x=48~55px과 겹쳐 첫 글자를 지웠다. `src/battle_message.c`의 두 오른쪽 열 시작을 `{CLEAR_TO 56}`으로 옮겼다. 한글 좁은 글꼴 8px 기준 최장 `도망간다`는 x=56~87px로 96px 창 안에 든다. 대상 `git diff --check`와 위치 계산 통과. `make hns -j8` 종료 코드 0, 로그 `build/localization-logs/hns-battle-menu-columns-20260914.log`, ROM 사용 33,285,668 bytes (99.20%), EWRAM 249,304 bytes, IWRAM 25,704 bytes. `pokehns.gba`는 2026-09-14 11:59:03 KST에 갱신됐다. 다음 단계는 mGBA에서 가방·도망간다의 전체 표시와 커서 이동 시 지워짐 여부를 실제 화면으로 확인하는 것이다.

- **배틀 메시지 조사 누락 수정·빌드 완료, 화면 재확인 필요:** 실제 야생 구구 화면에서 `앗! 야생 구구 튀어나왔다!`로 `가`가 빠졌다. `charmap.txt`의 `B_TXT_IGA = FD 36`이 C 헤더에서는 다른 토큰으로 정의되어 있었고 배틀 파서에 조사 7종의 분기가 없었다. `include/battle_message.h`의 조사 ID 0x35~0x3B와 이후 ID를 charmap에 맞추고 `src/battle_message.c`에서 직전 글자의 받침에 따라 조사를 출력하도록 수정했다. 68개 토큰 ID 정적 대조 및 대상 diff 공백 검사 통과. `make hns -j8` 종료 코드 0, 로그 `build/localization-logs/hns-battle-particle-20260914.log`, ROM 33,285,668 bytes (99.20%), EWRAM 249,304 bytes, IWRAM 25,704 bytes. `pokehns.gba`는 2026-09-14 11:46:28 KST에 갱신됐다. 다음 단계는 mGBA에서 새 ROM을 다시 열어 구구가·팬텀이 및 다른 조사 조합을 실제 화면으로 확인하는 것이다.

- **HNS 2.0.5 소스 통합·빌드 완료, 게임 화면 미확인:** 공식 `Release-v2.0.5`(`1f42b74dff`)의 265개 변경 파일을 공통 기준 `a9fbb77c6f`에서 현재 작업 트리로 3방향 병합했다. 257개는 공식 버전 그대로, 8개는 기존 사용자 수정과 자동 병합되었으며 충돌은 없었다. 적용 전 대상 파일 백업은 `build/localization-backups/pre-hns-205-20260914-112129.tar.gz`다. 한글 입력 코드·목호 그림·메가 그림의 소스 참조가 남아 있음을 정적 확인했다. `make hns -j8` 종료 코드 0, 로그 `build/localization-logs/hns-205-integration-20260914.log`, ROM 사용 33,285,300 bytes (99.20%), EWRAM 249,304 bytes, IWRAM 25,704 bytes. `pokehns.gba`는 2026-09-14 11:28:57 KST에 갱신됐다. Git HEAD는 기존 로컬 커밋 `d7d3194fa1` 그대로이고 통합은 미커밋 작업 트리에 있다. 다음 단계는 에뮬레이터에서 오박사 인트로, 한글 메뉴·배틀 메시지, 목호·포켓몬 그림 및 메가진화의 실제 화면과 기능을 검사하는 것이다. ROM 여유가 약 269KiB로 적으므로 추가 데이터 변경 시 재확인한다.
- 공백 검사 범위: 인수인계 문서 `git diff --check -- ...`는 통과했다. 전체 작업 트리의 `git diff --check`는 기존/공식 변경에 걸친 다수의 후행 공백 및 EOF 빈 줄로 종료 코드 2가 나왔다. 빌드 오류는 아니며 이번 병합에서 임의로 공백을 대량 수정하지 않았다.

- 이번 조사 완료: `STRINGID_PROTECTEDTEAM`(`src/battle_message.c:565`)은 광역 방어 기술 `Wide Guard`, `Quick Guard`, `Crafty Shield`, `Mat Block` 성공 후 `gProtectLikeUsedStringIds`를 통해 출력된다(`data/battle_scripts_1.s:3175-3183`, `src/battle_script_commands.c:7319-7338`). `{B_CURRENT_MOVE}`는 사용한 기술명이고 `{B_ATK_TEAM2}`는 사용자인 `gBattlerAttacker`의 편을 확인해 플레이어 측이면 `우리`, 상대 측이면 `상대`가 된다(`src/battle_message.c:1522-1525`, `:3647-3658`). 따라서 현재 영문 본문과 한글 팀 토큰이 섞여 플레이어가 Wide Guard를 쓰면 `Wide Guard protected 우리 team!`, 상대가 쓰면 `Wide Guard protected 상대 team!`처럼 표시된다. 이 문구는 단일 대상 보호 기술의 `protected itself` 문구와 다르다. 이번에는 소스 문구를 수정하지 않았다.
- 이번 조사 완료: `STRINGID_PKMNSWITCHEDSTATCHANGES`(`src/battle_message.c:558`)는 `Heart Swap`, `Power Swap`, `Guard Swap`의 성공 스크립트에서 능력치 랭크를 교환한 뒤 출력된다(`data/battle_scripts_1.s:1929-1962`). `{B_ATK_NAME_WITH_PREFIX}`는 기술을 사용한 포켓몬의 닉네임이며, 상대 측이면 `상대 ` 또는 야생전이면 `야생 ` 접두사가 붙고 플레이어 측이면 접두사 없이 출력된다(`src/battle_message.c:2917-2932`, `:3238-3240`). 예를 들어 플레이어 피카츄가 상대 팬텀에게 Power Swap을 쓰면 `Pikachu switched stat changes with its target!`, 상대 팬텀이 플레이어 피카츄에게 Guard Swap을 쓰면 `상대 팬텀 switched stat changes with its target!`처럼 표시된다. 이 ID는 Speed Swap의 전용 문구에는 사용되지 않는다(`data/battle_scripts_1.s:1964-1971`). 이번에는 소스 문구를 수정하지 않았다.
- 이번 출력 검증: `STRINGID_POISONSPIKESSCATTERED`(`src/battle_message.c:557`)는 `Toxic Spikes` 기술 성공 시 `BattleScript_EffectToxicSpikes`가, `Toxic Debris` 특성 발동 시 `BattleScript_ToxicDebrisActivates`가 출력한다(`data/battle_scripts_1.s:2015-2022`, `:4954-4963`). 시작 시 부여된 독압정도 `gStartingStatusStringIds`와 `BattleScript_OverworldHazard`를 통해 같은 ID를 사용한다(`src/battle_message.c:932`, `data/battle_scripts_1.s:4504-4507`). `{B_DEF_PREFIX1}`는 `gBattlerTarget`의 편에 따라 상대 포켓몬이면 `상대`, 플레이어 포켓몬이면 `우리 편`으로 확장된다(`src/battle_message.c:3463-3467`). 따라서 플레이어가 Toxic Spikes를 사용하거나 플레이어 측 Toxic Debris가 발동하면 `상대의 발밑에` 다음 줄 `독압정이 뿌려졌다!`, 상대가 설치하면 `우리 편의 발밑에` 다음 줄 `독압정이 뿌려졌다!`가 된다. 현재 문장 리터럴 자체의 토큰·줄바꿈 정적 검사는 통과했다. 이전 빌드 시도에서는 당시 547번 줄의 사용자 구문 오류로 컴파일이 중단됐고, 그 뒤 최신 소스에서는 해당 줄이 수정된 상태다. 최신 `make hns -j8` 재시도는 도구·생성 파일 선행 작업에서 시간 초과되어 ROM 화면 출력까지 진행하지 못했다. 독압정 문장과 사용자 수정은 건드리지 않았다.
- 이번 조사 완료: `STRINGID_PKMNCANTUSEITEMSANYMORE`(`src/battle_message.c:550`)은 `Embargo` 기술이 대상에게 성공적으로 걸린 직후 `BattleScript_EffectEmbargo`가 출력한다(`data/battle_scripts_1.s:2078-2086`). 명중 판정 후 `setembargo`가 대상의 `VOLATILE_EMBARGO`와 타이머를 설정하고, 이미 금지 상태인 대상이면 실패 분기로 가므로 이 문구가 나오지 않는다(`src/battle_script_commands.c:9293-9306`). 현재 `B_EMBARGO_TIMER` 설정은 5이며, 턴 종료 때마다 감소해 0이 되면 `STRINGID_EMBARGOENDS`로 도구 사용 가능 문구가 출력된다(`include/config/battle.h:212`, `src/battle_end_turn.c:841-854`, `data/battle_scripts_1.s:5890-5894`). `{B_DEF_NAME_WITH_PREFIX}`는 Embargo의 대상 포켓몬 이름이다. 예를 들어 상대 팬텀이 플레이어 피카츄에게 Embargo를 성공시키면 애니메이션 뒤 `Pikachu can't use items anymore!`가 나오고, 5턴 뒤 `Pikachu can use items again!`이 나온다. 이 메시지는 이미 Embargo 상태에서 가방 도구를 선택할 때 나오는 문구가 아니다. 그 경우 파티 메뉴의 `CannotUseItemsInBattle`이 막고 `써도 효과가 없다!`를 표시한다(`src/item_use.c:1290-1311`, `:1430-1435`, `src/strings.c:299`). 이번에는 소스 문구를 수정하지 않았다.
- 이번 조사 완료: `STRINGID_HURTBYITEM`(`src/battle_message.c:545`)은 지닌 도구가 포켓몬에게 패시브 HP 피해를 준 뒤 `BattleScript_ItemHurtRet`에서 출력된다(`data/battle_scripts_1.s:7158-7164`). `Life Orb`는 공격자가 살아 있고 기술 사용이 무효화되지 않았으며 한 명 이상의 대상에게 피해를 준 공격 뒤(매 공격 종료 처리 `src/battle_move_resolution.c:3380-3388`) 최대 HP의 1/10을 잃을 때 사용한다(`src/battle_hold_effects.c:557-571`). `Sticky Barb`는 엔드 턴 도구 처리에서 보유자가 최대 HP의 1/8을 잃을 때 사용하고(`src/battle_hold_effects.c:597-609`, `src/battle_end_turn.c:1286-1297`), `Black Sludge`는 독 타입이 아닌 보유자가 엔드 턴에 최대 HP의 1/8을 잃을 때 사용한다. 독 타입 보유자는 같은 도구의 회복 경로로 빠진다(`src/battle_hold_effects.c:642-668`, `:1145-1150`, `src/battle_end_turn.c:383-388`). 메시지 스크립트는 HP 바 갱신 → `STRINGID_HURTBYITEM` 출력 → `tryfaintmon` 순서라서 이 피해로 HP가 0이 되면 기절 처리가 이어진다. `{B_ATK_NAME_WITH_PREFIX}`는 당시 `gBattlerAttacker`로 설정된 피해 대상 포켓몬이고, `{B_LAST_ITEM}`은 `gLastUsedItem`에서 확장되는 원인 도구명이다(`src/battle_end_turn.c:1519`, `src/battle_message.c:3274-3317`). 예를 들어 라이프오브를 지닌 피카츄가 공격으로 피해를 주면 `Pikachu was hurt by the Life Orb!`, 독 타입이 아닌 피카츄가 블랙슬러지를 지닌 채 턴을 마치면 `Pikachu was hurt by the Black Sludge!`, 스티키바브 보유자는 `Pikachu was hurt by the Sticky Barb!`가 출력된다. 매직가드 보유자는 이 세 피해 경로에서 제외된다. 이번에는 소스 문구를 수정하지 않았다.
- 이번 조사 완료: `STRINGID_ENDUREDSTURDY`(`src/battle_message.c:543`) 직전에 옹골참 특성 팝업을 띄우는 스크립트가 실행된다. `BattleScript_SturdiedMsg`가 `BattleScript_AbilityPopUpTarget`를 호출해 피격 대상의 특성을 표시한 뒤 `STRINGID_ENDUREDSTURDY`를 출력한다(`data/battle_scripts_1.s:5357-5362`). 옹골참 발동 자체는 최대 HP에서 치명적인 피해를 받아 HP 1로 버틸 때 `MOVE_RESULT_STURDIED`를 설정하는 경로다(`src/battle_util.c:8168-8190`). 팝업 위치는 고정 좌측이 아니라 대상 측에 따라 달라진다. 싱글 배틀에서는 플레이어 대상이 x=24(좌측), 상대 대상이 x=178(우측)이며, 더블 배틀도 플레이어 슬롯은 x=24, 상대 슬롯은 x=178이다(`src/battle_interface.c:2673-2685`, `:2838-2869`). 따라서 상대 포켓몬의 옹골참이면 우측, 플레이어 포켓몬의 옹골참이면 좌측에 특성 팝업이 먼저 나타난다. 팝업의 능력명은 `gAbilitiesInfo[ABILITY_STURDY].name`에서 가져오며 현재 값은 `STURDY`다(`src/data/abilities.h:41-47`). 이번에는 소스 문구를 수정하지 않았다.
- 이번 조사 완료: `STRINGID_USINGITEMSTATOFPKMNROSE`와 `STRINGID_USINGITEMSTATOFPKMNFELL`(`src/battle_message.c:492-493`)은 도구 효과로 배틀 중 능력치가 변할 때 출력된다. 492번은 `Weakness Policy`, `Snowball`, `Luminous Moss`, `Cell Battery`, `Absorb Bulb`, `Blunder Policy`, `Throat Spray`, `Adrenaline Orb`, X도구, 능력치 상승 열매 등의 상승 메시지에 사용된다(`data/battle_scripts_1.s:4694-4741`, `:6190-6200`, `:7236-7253`, `data/battle_scripts_2.s:116-122`). 493번은 별도 스크립트가 직접 호출하는 ID가 아니라 `PrepareStringBattle`이 실제 변화 방향이 하락일 때 492번을 하락 문구로 바꿔 선택한다(`src/battle_util.c:1215-1237`). 따라서 `Room Service`로 스피드가 내려가거나, `Contrary`가 원래 상승할 도구 효과를 하락으로 뒤집을 때 출력된다. `{B_LAST_ITEM}`은 발동한 도구, `{B_BUFF1}`은 능력치명, `{B_SCR_NAME_WITH_PREFIX2}`는 대상 포켓몬 이름(소문자 접두사 형식), `{B_BUFF2}`는 상승·하락 단계에 따른 `크게` 등의 수식어다. 예를 들어 약점보험으로 공격이 2단계 오르면 `Using Weakness Policy, the Attack of Pikachu sharply rose!`, 룸서비스로 스피드가 내려가면 `Using Room Service, the Speed of Pikachu fell!`과 같은 형태가 된다. 현재 두 본문 리터럴에는 영문이 남아 있어 실제 출력은 동적 한글 토큰과 섞일 수 있다. 이번에는 소스 문구를 수정하지 않았다.
- 이번 조사 완료: `STRINGID_PKMNGOTENCOREDMOVE`(`src/battle_message.c:311`)는 앙코르 상태인 포켓몬이 지정된 기술이 아닌 다른 기술을 선택했을 때 기술 선택 화면에서 출력된다. `TrySetCantSelectMoveBattleScript`가 `encoredMove`가 존재하고 선택 기술과 다를 때(현 설정 `B_ENCORE_TARGET = GEN_LATEST`, 다이맥스·Z기술 예외 제외) 앙코르된 기술을 `gCurrentMove`에 넣고 `BattleScript_EncoredMove`를 실행한다(`src/battle_util.c:1397-1421`, `src/battle_main.c:4632-4639`). 배틀 팰리스에서는 `BattleScript_EncoredMoveInPalace`가 같은 `printselectionstring`을 사용한다(`data/battle_scripts_1.s:4774-4782`). 따라서 예를 들어 전기자석파가 앙코르된 피카츄가 몸통박치기를 고르면 `Pikachu can only use Thunderbolt!`처럼 표시한 뒤 선택을 다시 받으며, 앙코르된 기술을 고른 경우에는 이 문구가 나오지 않는다. 앙코르가 끝나거나 해당 기술의 PP가 0이 되면 `STRINGID_PKMNENCOREENDED`가 출력된다(`data/battle_scripts_1.s:4784-4787`, `src/battle_end_turn.c:738-752`). `{B_SCR_NAME_WITH_PREFIX}`는 선택을 처리 중인 포켓몬 이름(필요한 접두사 포함), `{B_CURRENT_MOVE}`는 강제된 앙코르 기술 이름이다. 이번에는 소스 문구를 수정하지 않았다.
- 이번 조사 완료: `STRINGID_PLAYERPICKEDUPMONEY`(`src/battle_message.c:298`)는 `고양이돈받기(PAY DAY)` 또는 `거다이금화(G-Max Gold Rush)`로 누적된 돈을 전투 승리 후 회수할 때 출력된다. `HandleEndTurn_BattleWon`이 로컬 야생전·일반 트레이너전의 보상 스크립트에서 `givepaydaymoney`를 실행하고, `gPaydayMoney != 0`이며 링크·기록 링크 배틀이 아니면 금액을 추가한 뒤 `BattleScript_PrintPayDayMoneyString`이 이 문구를 출력한다(`src/battle_main.c:5563-5638`, `src/battle_script_commands.c:8424-8442`, `data/battle_scripts_1.s:4142-4166`). 이번에는 소스 문구를 수정하지 않았다.
- 이번 조사 완료: 일반 플레이어 이름 토큰은 `{B_PLAYER_NAME}`(`charmap.txt:391`, 파서 `src/battle_message.c:3376-3378`)이다. 일반 전투에서는 `gSaveBlock2Ptr->playerName`을, 기록 전투에서는 `gLinkPlayers[0].name`을 사용한다. 링크 배틀 참가자의 이름에는 별도 토큰 `{B_LINK_PLAYER_NAME}`(`charmap.txt:386`, 파서 `:3361-3363`)을 사용하며, 이는 현재 멀티플레이어 ID의 링크 플레이어 이름이다. `{B_PLAYER_MON1_NAME}`·`{B_PLAYER_MON2_NAME}`은 플레이어 이름이 아니라 포켓몬 이름이다. 이번에는 소스 문구를 수정하지 않았다.
- 이번 조사 완료: `src/battle_message.c:210-216`의 다섯 `STRINGID` 사용처를 확인했다. `STRINGID_SCR_ITDOESNTAFFECT`는 풀 타입·Overcoat·기술 무효·프랭스터 차단·에어벌룬·Good as Gold 등으로 스크립팅 대상에게 기술이 통하지 않을 때, `STRINGID_BATTLERFAINTED`는 포켓몬이 기절한 직후, `STRINGID_PLAYERWHITEOUT2_WILD`·`STRINGID_PLAYERWHITEOUT2_TRAINER`·`STRINGID_PLAYERWHITEOUT3`는 모든 포켓몬을 잃은 패배 처리에 사용된다. 일반 트레이너전 포기에서는 현재 설정상 `STRINGID_PLAYERWHITEOUT2_TRAINER`만 출력되고 `STRINGID_PLAYERWHITEOUT3`는 이어지지 않는다. 이번에는 소스 문구를 수정하지 않았다.
- 이번 조사 완료: `sText_PkmnSwitchOut`(`src/battle_message.c:124`)는 현재 저장소에서 선언부 외 참조가 없어 실제 출력되지 않는다. `STRINGID_RETURNMON` 분기는 `sText_PkmnThatsEnough`·`sText_PkmnComeBack`·`sText_PkmnOkComeBack`·`sText_PkmnGoodComeBack` 또는 링크·파트너 전용 문구를 선택한다(`:2532-2607`). 주석의 “currently unused”가 현재 코드와 일치하며, 이번에는 소스 문구를 수정하지 않았다.
- 이번 조사 완료: `sText_LinkPartnerSentOutPkmn1GoPkmn`(`src/battle_message.c:118`)은 `STRINGID_INTROSENDOUT`에서 링크 멀티 더블 배틀의 파트너 측 포켓몬을 내보낼 때 선택된다(`:2487-2493`). 소스 문자열에는 `\n`이 없지만, 확장 후 `BattleStringExpandPlaceholders`가 `BreakStringAutomatic`을 호출해 폭 208·최대 2줄 기준으로 자동 줄바꿈한다(`:3730`). 따라서 항상 한 줄로 고정되는 문구는 아니며 이름 길이에 따라 두 줄로 나뉠 수 있다. 이번에는 소스 문구를 수정하지 않았다.
- 이번 조사 완료: `sText_LegendaryPkmnAppeared`(`src/battle_message.c:94`)는 `BufferStringBattle`의 `STRINGID_INTROMSG` 분기(`:2390-2468`)에서 트레이너·고스트 전투가 아니고 `BATTLE_TYPE_LEGENDARY`가 켜진 야생 전투일 때 선택된다(`:2456-2457`). `{B_OPPONENT_MON1_NAME}`에는 상대편 첫 번째 포켓몬의 이름이 들어간다. 이 플래그는 `BattleSetup_StartLatiBattle`, `BattleSetup_StartLegendaryBattle`, `StartGroudonKyogreBattle`, `StartRegiBattle`에서 설정되며 라티아스·라티오스, 루기아·칠색조·뮤·세레비·지라치·테오키스·카푸 계열·그란돈·가이오가·레쿠쟈·레지 계열 등의 이벤트성 전설 포켓몬 전투에 사용된다. 이번에는 소스 문구를 수정하지 않았다.
- 이번 작업 완료: `../pokeemerald-kr/src/battle_message.c`와 HNS의 `src/battle_message.c`를 대조해 대응하는 한글 텍스트를 HNS에 반영했다. 같은 이름 선언 62개, `STRINGID` 대응 테이블 311개, 포켓블록 맛 문자열 5개를 교체했고, HNS 토큰 이름 차이는 문자열 내부에서만 호환 형태로 조정했다. 직접 선언명 대응이 없는 HNS 선언 39개와 `STRINGID` 359개는 다른 이름의 대응 여부를 포함해 [대조 보고서](BATTLE_MESSAGE_KR_COMPARE.md)에 목록화했다.
- 검증: 사전 스냅샷과 비교해 줄 수·실제 C 문자열 리터럴 수(916개)·문자열 바깥 구조가 동일하고 문자열 리터럴 내부 378곳만 변경됐다. `git diff --check -- src/battle_message.c` 통과, 미지원 별칭 잔존 없음. `make hns -j8` 종료 코드 0으로 컴파일·링크·`pokehns.gba` 생성을 완료했다. ROM 33,256,484 bytes(99.11%), EWRAM 248,844 bytes, IWRAM 25,704 bytes. 로그 `build/localization-logs/battle-message-kr-merge.log`. 실제 에뮬레이터 화면에서 모든 동적 문장을 전수 확인하지는 않았다. `B_TXT_EUNNEUN` 등 조사 토큰은 현재 `charmap.txt`에 있으나 런타임 분기 정의(`include/battle_message.h`)가 별도 범위에 있으므로, 이번 텍스트 전용 변경에서는 헤더를 수정하지 않고 실제 조사 조합을 후속 화면 검증 대상으로 남겼다.
- 이번 작업 완료: Soulgold의 `../soulgold/graphics/trainers/front_pics/champion_lance.png`와 팔레트를 HNS의 기존 목호 전용 배틀 전면 그림 슬롯에 반영했다. `graphics/trainers/front_pics/champion_lance_hns.png` 및 `graphics/trainers/palettes/champion_lance_hns.pal`을 교체하고 4bpp·SMOL·GBA 팔레트를 재생성했다. HNS의 `TRAINER_PIC_FRONT_CHAMPION_LANCE_HNS` 선언·ID와 오버월드 목호 자산은 변경하지 않았다.
- 검증: HNS 대상 PNG·4bpp·SMOL·GBA 팔레트가 Soulgold 원본과 바이트 단위로 일치한다. `gText_Judgment` 수정 후 전체 HNS 빌드는 위 배틀 메시지 병합 빌드에서 통과했다. 로그 `build/localization-logs/battle-message-kr-merge.log`.
- 이번 조사 완료: `gText_DefendersStatRose`는 `gStatUpStringIds[B_MSG_DEFENDER_STAT_CHANGED]`가 선택하는 `STRINGID_DEFENDERSSTATROSE` 메시지다. 대상 포켓몬 이름(`B_DEF_NAME_WITH_PREFIX`), 능력치명(`B_BUFF1`), 상승 정도(`B_BUFF2`)를 조합해 대상의 능력치 상승을 표시한다. 이번 병합에서 문장 끝을 `올라갔다!`로 한글화했다.
- 추가 확인: `gText_DefendersStatRose`의 `{B_DEF_NAME_WITH_PREFIX}`는 `src/battle_message.c:3241`에서 `HANDLE_NICKNAME_STRING_CASE(gBattlerTarget)`로 확장된다. 트레이너 배틀에서 상대편이면 `sText_FoePkmnPrefix`(`:134`, `상대 `)를 붙이고, 야생 배틀이면 `sText_WildPkmnPrefix`(`:133`, `야생 `)를 붙인다. `sText_Opposing1/2`는 팀 표현용 별도 토큰이라 이 문구의 출처가 아니다.
- 이번 조사 완료: `sText_FoePkmnPrefix2/3/4`와 `sText_AllyPkmnPrefix/2/3`은 `B_TXT_ATK_PREFIX1~3`·`B_TXT_DEF_PREFIX1~3` 토큰을 처리하는 접두사 문자열이다. `src/battle_message.c:3445-3480`에서 공격자·방어자의 플레이어 측 여부에 따라 한글 `우리 편` 또는 `상대` 계열을 선택한다. `gText_PkmnShroudedInMist`와 일부 `STRINGID_PKMNRAISED*`·`STRINGID_PKMNCOVEREDBYVEIL` 문구가 `B_ATK_PREFIX2`를 사용한다. `sText_FoePkmnPrefix`(`:134`)와 `sText_Opposing1/2` 팀 표현 토큰은 별도 경로다.
- 이번 조사 완료: `src/battle_message.c:1522-1526`의 문자열 용도를 확인했다. `sText_Your1`·`sText_Opposing1`은 문장 첫머리용 `Your`·`The opposing`, `sText_Your2`·`sText_Opposing2`는 문장 중간용 소문자 형태이며 `{B_ATK_TEAM1/2}`, `{B_DEF_TEAM1/2}`, `{B_EFF_TEAM1/2}` 플레이스홀더를 확장할 때 사용된다. `sText_EmptyStatus`의 `$$$$$$$`는 상태 문자열 비교용 8바이트 패딩·초기값으로 화면에 표시되지 않는다. 이번에는 소스 문구를 수정하지 않았다.
- 추가 확인: `{B_ATK_TEAM1}`은 `src/battle_message.c:3647-3652`에서 공격자(`gBattlerAttacker`)가 플레이어 측이면 `sText_Your1`(`우리`), 상대 측이면 `sText_Opposing1`(`상대`)로 확장된다. 현재 HNS에서는 이 토큰이 남아 있는 확장 `STRINGID_TAILWINDENDS`·`STRINGID_LUCKYCHANTENDS` 등의 영문 문구에서 사용된다. `TEAM2`와 달리 현재 한글 값은 두 형태가 같지만, 원래는 문장 중간의 대소문자를 구분하는 별도 슬롯이다.
- 이번 조사 완료: `gText_SafariZoneMenuFrlg`는 정의 자체는 공통으로 컴파일되지만 HNS 런타임에서는 사용되지 않는다. `include/constants/global.h:73-76`에서 HNS의 `IS_FRLG`가 0이므로 `src/battle_controller_safari.c:345`는 `gText_SafariZoneMenu`를 선택한다. 따라서 HNS 사파리존 UI를 번역할 때는 `gText_SafariZoneMenu`를 대상으로 하고, `gText_SafariZoneMenuFrlg`는 FRLG 전용 문구로 남긴다.
- 이번 조사 완료: 기술 선택창의 `PP` 옆 원형 표시는 `src/battle_controller_player.c:2449-2484`의 상성 아이콘이다. `B_SHOW_EFFECTIVENESS`가 `SHOW_EFFECTIVENESS_ALWAYS`로 설정되어 있어 기술을 고를 때 `B_WIN_PP` 창에 `{CIRCLE_HOLLOW}`(보통 효과), `{CIRCLE_DOT}`(효과가 굉장함), `{TRIANGLE}`(효과가 별로임), `{BIG_MULT_X}`(효과 없음)를 덧붙인다. 이번에는 코드 변경을 하지 않았다.
- 이번 조사 완료: 배틀 UI 번역 대상의 위치를 확인했다. 고정 명령·상태 문구는 `src/battle_message.c:1421-1459`, 전투 중 메시지는 같은 파일의 보조 문자열 `:73-190`과 `gBattleStringsTable` `:193` 이후, 플레이어 입력·출력 호출은 `src/battle_controller_player.c`, 창 좌표는 `src/battle_bg.c:157-383`과 `include/constants/battle.h:684-709`에 있다. 체력박스의 레벨·HP·이름은 `src/battle_interface.c:889-1016` 및 `:1760-1824`에 있다. 이번에는 소스 문구를 수정하지 않았다.
- 이번 작업 완료: `src/data/script_menu.h` 812행 이후에서 HNS 스크립트가 직접 참조하는 메뉴의 원문만 선별했다. HNS 전용 링크 메뉴의 `TRADE`·`BATTLE`·`SINGLE BATTLE`·`DOUBLE BATTLE`을 공식 한글 표기(`교환`·`대전`·`싱글 배틀`·`더블 배틀`)로 반영했다. `SEVII ISLANDS` 등 FRLG 전용 블록은 HNS 참조가 없어 유지한다.
- 추가 확인: `sMultichoiceList_GameCornerPokemonPrizes`와 `sMultichoiceList_CeladonVendingMachine`은 각각 `CeladonCity_GameCorner_PrizeRoom_Frlg`와 `CeladonCity_DepartmentStore_Roof_Frlg`에서만 호출된다. 두 호출 모두 `data/event_scripts.s`의 `IS_FRLG` 블록에 포함되어 HNS 스크립트에서는 사용되지 않는다. 배열과 ID 매핑은 공통 소스에 남아 HNS ROM에 컴파일될 수 있지만 HNS 게임 흐름에서 호출되지는 않는다.
- 추가 확인: `STDSTRING_COINS`(인덱스 38)는 숨겨진 코인을 주울 때 `STR_VAR_2`에 `코인`을 넣어 `gText_FoundXCoins`의 `{STR_VAR_1} {STR_VAR_2}` 자리에 표시한다. 코인 보유량 창의 숫자 표시는 별도의 `gText_Coins`·`PrintCoinsString` 경로를 사용한다.
- 검증: `git diff --check` 통과. `make hns -j8` 종료 코드 0, ROM 사용 33,256,260 bytes (99.11%), EWRAM 248,844 bytes, IWRAM 25,704 bytes. 로그 `build/localization-logs/hns-script-menu-tail.log`. 실제 게임 화면은 미확인이다.
- 다음 시작점: 생성된 `pokehns.gba`로 배틀 메시지의 능력치 변화·기술 사용·도망·포켓블록 화면과 목호 배틀 전면 그림을 실제 에뮬레이터에서 확인한다. 화면 검증 뒤 잘림이나 조사 조합 문제가 있으면 해당 문자열 리터럴만 추가 조정한다.

## 환경과 작업 트리

- HNS: 현재 저장소 루트. WSL 경로 `/mnt/c/Users/tjkel/Desktop/decomps/pokehns-expansion`.
- Soulgold 원본: 형제 폴더 `../soulgold` (Windows: `C:\Users\tjkel\Desktop\decomps\soulgold`). 읽기/참조용이며 HNS와 별개다.
- 기존 사용자 변경 및 여러 작업의 미커밋·미추적 파일이 존재한다. 전체 diff가 이번 세션의 변경을 뜻하지 않는다.
- 실행 목표: `make hns -j8`, 결과 `pokehns.gba`. 기본 Emerald 빌드와 혼동하지 않는다.

## 고정된 사용자 결정

- 1~9세대 포켓몬 이름·기술명·도감 분류는 공식 한글 표기를 따른다.
- 이름에는 폼/성별 표기를 덧붙이지 않는다. 단, 니드런은 정식 표기인 `니드런♀` / `니드런♂`.
- 그림 이식 시 기존 한글화와 능력치·기술 등 게임 데이터를 유지한다.
- 메가 그림은 HNS에 종 데이터가 있는 경우에만 가져온다. Soulgold 고유 메가 종을 추가하지 않는다.

## 완료된 변경과 검증 한계

| 작업 | 적용 내용 | 확인 상태 |
| --- | --- | --- |
| 포켓몬 이름 | `src/data/pokemon/species_info/gen_*_families.h`의 이름 한글화, 니드런 성별 예외 | 이전 세션의 데이터 검사 완료; 모든 게임 화면 확인은 아님 |
| 도감 분류 | `.categoryName` 한글화, 일부 폼별 분류 반영 | 이전 세션 인코딩 검사 완료; 런타임의 `포켓몬` 접미사 중복 금지 |
| 기술명 | `src/data/moves_info.h`, `MOVE_NAME_LENGTH = 22` | 이전 세션 데이터 검사 및 후속 HNS 빌드 통과 |
| 배틀 텍스트 박스 | `graphics/battle_interface/hns/textbox.png`, `textbox_0.pal`, `textbox_1.pal`, `graphics_file_rules.mk` | 원본 에셋 비교 완료, 게임 화면 미검증 |
| 배틀 포켓몬 그림 | 앞뒤·일반/이로치·해당 성별 그림, 크기·좌표·그림자·등장 애니메이션 | 1,571개 종 컨텍스트의 그래픽 필드 비교 및 빌드 통과, 게임 화면 미검증 |

이름/분류/기술명 작업은 이전 세션에서 PokeAPI 한국어 CSV와 개별 예외를 사용했다. 공식 도감 전체와의 별도 전수 대조를 완료했다고 주장하지 않는다. 번역 의심 항목은 공식 자료로 재확인한다.

## Soulgold 이식의 구체적 경계

- 이번 이식 범위는 배틀 스프라이트다. 필드 동행 캐릭터와 파티 아이콘은 이식했다고 보고하지 않는다.
- HNS 기존 1,571개 종 컨텍스트만 대상으로 했다. HNS에 이미 있던 메가한카리아스Z의 비어 있던 그림도 연결했다.
- 제외한 donor-only 종: `SPECIES_DIALGA_PRIMAL`, `SPECIES_LUGIA_SHADOW`, `SPECIES_GARDEVOIR_MEGA_Z`, `SPECIES_MEOWSCARADA_MEGA`, `SPECIES_PRIMARINA_MEGA`, `SPECIES_TYPHLOSION_MEGA`.
- 주요 수정: 종 헤더, `src/data/graphics/pokemon.h`, `include/graphics.h`, `graphics/pokemon/`, `src/data/pokemon/species_info/shared_front_pic_anims.h`.
- 공통 애니메이션에 알로라 레트라·텅구리, 형사구스, 라란티스, 따라큐, 마기아나 테이블을 추가했고 투구뿌논 테이블을 원본과 맞췄다.
- 기존 코드의 개별 게임 데이터 필드는 보존했다. 빌드 호환성 예외로 메가야도란·메가레쿠쟈 `.cryId`에 기본 울음소리 fallback 조건을 추가했다.

현재 설정:

```c
// include/config/species_enabled.h
#define P_MEGA_EVOLUTIONS TRUE
#define P_PRIMAL_REVERSIONS TRUE

// include/config/pokemon.h
#define P_GBA_STYLE_SPECIES_GFX FALSE
#define P_MODIFIED_MEGA_CRIES FALSE
#define P_MODIFIED_MEGA_CRY_MODE CRY_MODE_HIGH_PITCH
```

별도 메가 울음소리 샘플을 켜면 ROM이 32MiB를 넘었다. 그림을 유지하기 위해 별도 샘플을 끄고 기본 울음소리의 음높이 효과를 사용한다. 이유 없이 다시 켜지 않는다. 메가진화는 기존 게임 조건(메가링과 해당 메가스톤 등)을 따른다. 아이템 지급이나 진행 조건은 변경하지 않았다.

## 마지막 검증

- 2026-09-13 `make hns -j8`: **종료 코드 0**. `battle_message.c` 병합 후 컴파일·링크를 통과했고 `pokehns.gba`를 갱신했다. ROM 33,256,484 / 33,554,432 bytes (99.11%), EWRAM 248,844 bytes, IWRAM 25,704 bytes. 로그 `build/localization-logs/battle-message-kr-merge.log`. 실제 게임 화면은 미확인.

디버그 메뉴 확인(2026-09-11): `include/config/debug.h`에서 `DEBUG_OVERWORLD_MENU`와 `DEBUG_BATTLE_MENU`는 이미 TRUE다. 필드에서 R을 누른 채 START, 배틀 행동 선택 화면에서 SELECT로 호출한다. `DEBUG_OVERWORLD_IN_MENU`는 FALSE이므로 일반 시작 메뉴의 항목으로는 표시되지 않는다. 입력 처리 코드를 확인했으며, 이번에는 코드 변경·ROM 재빌드·게임 실행 검증을 하지 않았다.

- `make hns -j8`: **종료 코드 0**, 2026-09-11 23:22 KST ROM 생성 확인.
- ROM 사용: 33,256,180 / 33,554,432 bytes (99.11%), 여유 298,252 bytes.
- EWRAM: 248,844 bytes (94.93%), IWRAM: 25,704 bytes (78.44%).
- 최종 ROM 파일은 패딩 포함 33,554,432 bytes.
- 로그: [evidence/2026-09-11-hns-build.txt](evidence/2026-09-11-hns-build.txt).
- 종 컨텍스트 1,571개 비교 통과: 그래픽 필드는 Soulgold와 일치(행 끝 공백 제외), 나머지 필드는 두 cry fallback 외 보존.
- 이전 검사: 인라인 애니메이션 834개 및 공유 애니메이션 사용 496개 프레임 범위 검사 통과, 변경한 PNG/PAL 255개 원본 일치.
- 관련 파일 `git diff --check` 통과. 에뮬레이터에서의 시각·실행 검증은 미수행.

이 결과는 위 시점의 작업 트리에 대한 결과다. 이후 변경에 자동으로 적용되는 보증이 아니다.

## 임시 자료의 위치와 한계

이전 세션 자료가 남아 있을 수 있는 경로:

- `/tmp/hns_before_soulgold_sprites/`: 이식 전 일부 파일 백업과 `manifest.json`.
- `/tmp/inspect_sprite_contexts.py`, `/tmp/import_soulgold_sprites.py`: 검사/일괄 이식 스크립트. 재실행하지 않는다. 이미 적용된 수정이나 사용자 변경을 손상시킬 수 있다.
- `/tmp/pokemon_species_names.csv`, `/tmp/move_names.csv`: 이전 조회 자료. 필요하면 PokeAPI 저장소 `data/v2/pokemon_species_names.csv`, `data/v2/move_names.csv`를 다시 확보하고 버전·한국어 언어 ID를 확인한다.

이 임시 자료는 다음 세션에서 없어도 된다. 현재 소스가 작업 결과이고, 이 문서와 보존한 빌드 로그가 인수인계 기준이다. 일괄 롤백이 필요하면 임시 백업 존재와 이후 수정부터 확인한다.

### 2026-09-14 — 특성·도구 이름 PokeAPI 한국어화 진행

- 요청/범위: 모든 포켓몬 특성과 포켓몬 도구의 표시 이름을 PokeAPI 한국어 명칭과 공식 한국어 명칭 기준으로 교체한다. 이번 범위는 이름·복수형이며 효과 설명 문장은 유지한다.
- 진행: PokeAPI 공식 저장소의 `ability_names.csv`, `item_names.csv`에서 영어 언어 ID 9와 한국어 언어 ID 3을 대응했다. 소스의 실제 특성 310개와 도구 이름·복수형 1,085개를 자동 대응했으며, `ABILITY_NONE`의 `-------`는 유지했다. API 한국어 행이 비어 있는 FRLG 이벤트 도구·HNS/Z-A 전용 도구 103개는 공식 한국어 표기 또는 종명 기반 이름으로 보완했다.
- 수정 파일: `src/data/abilities.h`, `src/data/items.h` (일괄 번역 적용 및 HNS 빌드 완료).
- 자료: [PokeAPI v2 문서](https://pokeapi.co/docs/v2), [ability_names.csv](https://raw.githubusercontent.com/PokeAPI/pokeapi/master/data/v2/csv/ability_names.csv), [item_names.csv](https://raw.githubusercontent.com/PokeAPI/pokeapi/master/data/v2/csv/item_names.csv). API에 한국어가 없는 3세대 중요 도구는 [포켓몬 위키 중요 도구/3세대](https://pokemon.fandom.com/ko/wiki/%EC%A4%91%EC%9A%94%ED%95%9C_%EB%8F%84%EA%B5%AC/3%EC%84%B8%EB%8C%80)의 한국어 표기를 확인했다.
- 검증: 문자열 개수(특성 310개와 `ABILITY_NONE`, 도구 이름·복수형 1,085개), 대상 파일 `git diff --check`, 한국어 문자열 길이 검사를 통과했다. `make hns -j8` 종료 코드 0, 로그 `build/localization-logs/hns-ability-item-names-20260914.log`, ROM 사용 33,284,804 bytes (99.20%), EWRAM 249,304 bytes, IWRAM 25,704 bytes. `pokehns.gba`는 2026-09-14 13:14:44 KST에 갱신됐다.
- 남은 확인: mGBA에서 특성 팝업·가방·상점·도구 효과 메시지의 한글 표시와 긴 이름 잘림 여부를 실제 화면으로 확인한다. 효과 설명 문장은 이번 요청 범위가 이름 번역이어서 영문으로 유지했다.
### 2026-09-14 — 무한스프레이 명칭 변경

- 요청/범위: `INFIN. REPEL`의 한국어 표시명을 `무한벌레스프레이`에서 `무한스프레이`로 변경.
- 수정 파일: `src/data/items.h`.
- 검증: 문자열 길이 검사와 `make hns -j8` 종료 코드 0을 확인했다. ROM·로그는 위 기록과 `build/localization-logs/hns-infinite-spray-20260914.log`에 남겼다.

### 2026-09-14 — 메가진화 특성 비활성화 표현 정정

- 확인: “비활성화된 메가진화 특성”이라는 별도 분류는 없다. `include/config/species_enabled.h`의 `P_MEGA_EVOLUTIONS`는 `TRUE`이며 메가진화는 활성화돼 있다.
- 구분(원시회귀 활성화 전 당시 설정): `P_PRIMAL_REVERSIONS`, `P_ULTRA_BURST_FORMS`, `P_GIGANTAMAX_FORMS`, `P_TERA_FORMS`가 `FALSE`라 해당 폼과 폼별 특성 할당이 빌드에서 제외됐다. 이는 특성 번역을 비활성화한 것이 아니라 폼 설정이다.
- 예: `시작의바다`·`끝의대지`는 원시회귀, `브레인포스`는 울트라버스트 계열이다. `델타스트림`은 메가레쿠쟈에 쓰이며, `메가런처`는 일반 특성이다.
- 이번 확인에서는 코드·설정을 수정하지 않았다. 실제 화면 검증은 기존 인수인계 항목으로 남아 있다.

### 2026-09-14 — 원시회귀 활성화

- 요청/범위: 원시가이오가·원시그란돈과 원시회귀 기능을 HNS 빌드에서 활성화.
- 수정 파일: `include/config/species_enabled.h`의 `P_PRIMAL_REVERSIONS`를 `FALSE`에서 `TRUE`로 변경했다. 기존 한국어 특성명 `시작의바다`·`끝의대지`, 원시회귀 폼 데이터·폼 변경표·그래픽·울음소리 정의는 유지했다.
- 검증: `P_PRIMAL_REVERSIONS` 조건부 데이터와 `FORM_CHANGE_BATTLE_PRIMAL_REVERSION` 경로를 정적으로 확인했다. `make hns -j8` 종료 코드 0, 로그 `build/localization-logs/hns-primal-reversions-20260914.log`, ROM 사용 33,329,156 bytes(99.33%), EWRAM 249,304 bytes, IWRAM 25,704 bytes. 패딩 후 `pokehns.gba`는 33,554,432 bytes로 생성됐다.
- 남은 확인: 실제 mGBA에서 원시회귀 애니메이션, 원시 폼 스프라이트, 특성명과 블루오브·레드오브 조건을 확인한다.

### 2026-09-14 — 메가진화 문구 사용자 지정 보존

- 요청/범위: 메가진화 반응 문구에서 사용자가 선택한 고정 조사 `와`를 유지한다.
- 확인: `src/battle_message.c`의 `STRINGID_MEGAEVOREACTING`은 현재 `{B_LAST_ITEM}와`를 사용한다. 이 문구는 사용자의 번역 선택으로 확정했으며 다시 조사 토큰으로 바꾸지 않는다. `STRINGID_MEGAEVOEVOLVED`의 줄바꿈과 플레이스홀더도 유지한다.
- 검증: 메가진화 두 문자열의 정의·문맥을 확인했다. 이 기록 이후에는 해당 문구에 추가 소스 수정을 하지 않았고, 사용자 지정 확인만 기록했다.

### 2026-09-14 — 메가진화 `와` 상태 재빌드 검증

- 요청/범위: 사용자가 지정한 `{B_LAST_ITEM}와` 문구를 그대로 둔 상태에서 HNS ROM을 다시 빌드.
- 확인: `src/battle_message.c:647-648`의 메가진화 반응·완료 문구가 요청한 상태로 유지됐다. 대상 파일 `git diff --check` 통과.
- 빌드: `make hns -j8` 종료 코드 0. 로그 `build/localization-logs/hns-mega-message-kr-user-wa-test-20260914.log`, ROM 링크 사용량 33,329,140 bytes(99.33%), EWRAM 249,304 bytes, IWRAM 25,704 bytes. 패딩 후 `pokehns.gba`는 33,554,432 bytes로 21:43:42 KST에 갱신됐다.
- 실제 mGBA에서 문구가 화면에 표시되는지 확인하는 실행 검증은 아직 남아 있다.

### 2026-09-14 — `STRINGID_TARGETCHANGEDTYPE` 출력 예시 확인

- 확인: 이 ID는 `data/battle_scripts_1.s`의 `BattleScript_EffectSoak`에서 `trysoak` 성공 뒤 출력된다. `trysoak`이 `{B_BUFF1}`에 변경 타입을 넣고, 타입 이름은 `gTypesInfo[].name`에서 확장한다.
- 플레이스홀더: `{B_DEF_NAME_WITH_PREFIX}`는 트레이너전에서 `상대 `, 야생전에서 `야생 ` 접두사를 붙인다. `{B_TXT_EUNNEUN}`은 이름의 마지막 받침에 따라 `은/는`을 선택한다.
- 예: 야생 구구에게 Soak을 사용하면 `야생 구구는\n물타입이 됐다!`, 트레이너전 상대 팬텀이면 `상대 팬텀은\n물타입이 됐다!`가 된다.
- 수정/검증: 소스는 수정하지 않았고 호출 스크립트·플레이스홀더 처리·타입명을 정적으로 확인했다. 빌드는 실행하지 않았다.

### 2026-09-15 — `STRINGID_RESETSTARGETSSTATLEVELS` 출력 상황 확인

- 확인: 이 ID는 `BattleScript_MoveEffectClearSmog`에서만 호출되며, `클리어스모그`가 명중해 대상에게 실제 피해를 주고 대상에게 능력치 랭크 변화가 하나 이상 있을 때 출력된다. 대상의 모든 능력치 랭크를 기본값으로 되돌린 뒤 메시지를 표시한다.
- 출력: `{B_DEF_NAME_WITH_PREFIX}`는 트레이너전에서 `상대 `, 야생전에서 `야생 ` 접두사를 붙인다. 현재 본문이 영문이므로 예시는 `상대 팬텀's stat changes were removed!`, `야생 구구's stat changes were removed!`처럼 출력된다.
- 출력되지 않는 경우: 클리어스모그가 빗나가거나, 대미지를 주지 못하거나, 대상의 능력치 랭크가 이미 모두 기본값이면 이 메시지는 나오지 않는다.
- 수정/검증: 소스는 수정하지 않았고 Clear Smog의 추가 효과 분기·배틀 스크립트·대상 이름 플레이스홀더를 정적으로 확인했다. 빌드는 실행하지 않았다.

### 2026-09-15 — `STRINGID_ELECTROMAGNETISM`·`STRINGID_REFLECTTARGETSTYPE` 출력 상황 확인

- `STRINGID_ELECTROMAGNETISM`: 전자부유(Magnet Rise)를 시작할 때 직접 출력되는 ID가 아니다. 전자부유 지속 턴이 끝나면 이 ID가 `{B_BUFF1}`에 저장되고, `STRINGID_BUFFERENDS`가 `{B_SCR_NAME_WITH_PREFIX}'s {B_BUFF1} wore off!`를 출력한다. 현재 본문이 영문이므로 예시는 `상대 팬텀's electromagnetism wore off!` 또는 `야생 구구's electromagnetism wore off!`이다.
- `STRINGID_REFLECTTARGETSTYPE`: 미러타입(Reflect Type)이 성공해 사용자의 타입이 대상의 타입으로 바뀐 직후 출력된다. 예를 들어 팬텀이 상대 피카츄에게 사용하면 현재 문구 그대로 `팬텀 became the same type as 상대 피카츄!`, 상대 팬텀이 플레이어 리자몽에게 사용하면 `상대 팬텀 became the same type as 리자몽!`이다. 아르세우스·실버디, 테라스탈 중인 사용자, 타입리스 대상 등은 실패 분기로 이 문구가 나오지 않는다.
- 수정/검증: 소스는 수정하지 않았고 호출 스크립트·타이머 종료 처리·플레이스홀더와 실패 조건을 정적으로 확인했다. 빌드는 실행하지 않았다.

### 2026-09-15 — `STRINGID_BUFFERENDS` 출력 상황 확인

- 이 ID는 `BattleScript_BufferEndTurn`에서 `{B_BUFF1}`에 저장된 효과 이름을 붙여 지속 효과가 끝났음을 알리는 공통 문구다.
- 현재 호출되는 경우는 도발 타이머 종료(`도발`), 전자부유 타이머 종료(`전자부유`), 회복봉인 타이머 종료(`회복봉인`)다. 현재 본문이 영문이므로 의도된 출력 예시는 각각 `상대 팬텀's 도발 wore off!`, `야생 구구's 전자부유 wore off!`, `팬텀's 회복봉인 wore off!`처럼 표시된다. 트레이너 상대는 `상대 `, 야생 포켓몬은 `야생 ` 접두사가 붙고, 플레이어 포켓몬은 접두사가 없다.
- 전자부유 종료 처리에는 `gBattleScripting.battler = battler` 대입이 없어 `B_SCR_NAME_WITH_PREFIX`가 이전 값의 영향을 받을 가능성이 있다. 전자부유 종료 화면에서 이름이 잘못 나오면 이 부분을 우선 점검한다. 다른 두 호출(도발·회복봉인)은 해당 대입을 수행한다.
- 수정/검증: 소스는 수정하지 않았고 공통 종료 스크립트, 세 가지 호출부, 버퍼에 들어가는 기술·문자열 ID와 이름 접두사 처리를 정적으로 확인했다. 빌드는 실행하지 않았다.

### 2026-09-27 — PC 저장 시스템 `menu.png` 변환 및 HNS 빌드

- 요청/범위: 사용자가 수정한 `graphics/pokemon_storage/menu.png`를 GBA 그래픽 산출물로 변환하고 HNS ROM에 반영되는지 확인했다.
- 변환: generic Make 규칙과 동일하게 `tools/gbagfx/gbagfx`로 `menu.4bpp`를 재생성하고, `tools/compresSmol/compresSmol -w`로 `menu.4bpp.smol`을 재생성했다. 압축 파일을 다시 풀어 생성된 `menu.4bpp`와 `cmp`한 결과가 일치했다.
- 연결: `src/graphics.c`의 `gStorageSystemMenu_Gfx`가 `graphics/pokemon_storage/menu.4bpp.smol`을 `INCBIN_U32`로 포함하며, PC 초기화 경로가 이를 압축 해제해 배경 그래픽으로 로드한다.
- 검증: `make BUILD=hns build/hns/src/graphics.o -j2`와 `GITHUB_ACTION=1 timeout 600s make --jobserver-style=pipe hns -j8`가 모두 성공했다. 링크 사용량은 EWRAM `249016/262144`(94.99%), IWRAM `25704/32768`(78.44%), ROM `33330900/33554432`(99.33%)이며, `pokehns.gba`는 2026-09-27 05:00:30 KST에 갱신됐다. ROM SHA-1은 `d7ec91b5504e54209da1e0c409a412688e35430d`다.
- 한계/다음: 이 환경에서는 mGBA를 실행할 수 없어 실제 PC 화면의 런타임 확인은 하지 않았다. mGBA를 완전히 종료한 뒤 저장소 최상단의 새 `pokehns.gba`를 다시 열어 PC 메뉴 화면을 확인한다.
