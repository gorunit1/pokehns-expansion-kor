# full-sync 묶음 11 — seq 217~226 (+ upstream #10837 앞당김)

- 시작 HEAD `ad4d9c50aa`(코드 = `f84019a062`, 2026-10-09 친구 수정 뒤). 작업 컴퓨터: 데스크탑(사용자는 랩탑에서 Remote Control). 날짜 2026-10-09 밤.
- 방식: PR별 사전 분석 3개(병렬, 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-217-226/seq*.md`) → 사용자 결정 2개("ㄱㄱ", 혼자 정함) → 적용 1개(커밋 10개, 손 맞춤 0) → 리뷰 2개(R1에서 1건 발견 → upstream #10837 앞당김) → 메인 검증.
- 실기(mGBA) 미확인.

## 커밋

| seq | PR | 커밋 | 판정·HnS 적응 | ROM(B) |
|---|---|---|---|---:|
| 217 | #10106 Use values from `enum PokeBall` | `9c44166fff` | 25 hunk 중 24 그대로, `ComputeBallData`의 `default:`만 HnS `case BALL_GS:` 뒤로. 볼 번호 값 그대로(`ITEM_POKE_BALL`=`BALL_POKE`=1, 럭셔리볼 14, GS볼 28, `BALL_RANDOM` 30), 데이터 심볼 변화 0, 볼 함수 기계어만 줄어듦. 볼 재사용·랜더마이저 볼·`CreateFacilityMon` 난수 그대로 | 32,754,804 |
| 218 | #10110 Cleanup `message 0x0` | `4732661ec2` | `std_msgbox.inc` `message NULL`. ROM 같음 | 32,754,804 |
| 219 | #10109 `SanitizeBagItemId`, `TryTakeMonItemResult` | `a649700e5f` | upstream 6 hunk 그대로 + **HnS 적응 3줄**(사용자 결정 1): 새 assert(`invalid bag item: ITEM_NONE`) 때문에 16번도로 구구, 얼음샛길 B3F 꾸꾸리·딜리버드 열매 이벤트에서 가방을 취소하면 `removeitem ITEM_NONE` → assert 화면(탐침 확인) → `goto_if_eq VAR_ITEM_ID, ITEM_NONE, …DislikedBerry` 1줄씩(이식 전 흐름, 새 문장 없음). 도구 꺼내기 한글 문장 3개 그대로, 세이브 변화 0 | 32,754,964 |
| 220 | #10129 Improve gimmick triggers' graphics | `b1c1400c5c` | 트리거 PNG 5개(HnS가 바꾼 적 없는 upstream 원본)·`mega_trigger.pal` 삭제(`.gbapal`은 PNG 규칙으로), `battle_gimmick.c` HnS 위치 표의 일반 항목만 손 맞춤(싱글 yDiff −11→−5, 더블 −4→−2), Gen4 메가 그대로(사용자 결정 2). 일반 플레이에서 트리거가 나오지 않음(메가링·Z파워링 입수처 0, 다이맥스·테라 플래그 0) — 디버그로만 | 32,754,996 |
| 221 | #10111 assertf for NULL goto/call | `e21a665639` | 그대로. HnS 이벤트 스크립트 goto/call 목적지 12,911개(219의 3줄 포함, 커밋 메시지의 12,908은 그 전 숫자)·미스터리 기프트 25개 모두 라벨 — NULL 0(리뷰 R2가 별도 도구로 재확인, 일부러 넣은 NULL 7개는 모두 잡음). C 쪽 `ScriptJump`/`ScriptCall` 호출부는 라벨이거나 NULL 검사 뒤. RAM 스크립트는 `FREE_MYSTERY_EVENT_BUFFERS TRUE`라 실행 경로 없음 | 32,755,076 |
| 222 | #10131 Remove deprecated `HQ_RANDOM` | `39bee1c3e0` | 손 맞춤 1곳: HnS `src/randomizer.c` `GenerateSeedForRandomizer`의 `#if HQ_RANDOM`이 조용히 `#else`로 넘어가지 않게 지금 쓰는 `Random32()`로 고정(호출처 없어 ROM에 안 들어감). ROM 같음 | 32,755,076 |
| 223 | #10103 Fix Beat Up typeless damage test | `0a027c6246` | 테스트만(이름 `… (up to Gen4)`, 영문 `NONE_OF MESSAGE` 대신 효과음으로 상성 검사 — 이식 전 HnS에서는 한글 메시지라 헛 PASS였음). ROM 같음 | 32,755,076 |
| 224 | #10130 Add `if_comptime` | `0551be0975` | 그대로. 컴파일 시점 50% `RandomPercentage` 24곳이 `RandomUniform`으로 — 같은 난수 값이면 결과 같음(65,536개 전수), 난수 소비 1회 같음, 게임 결과·세이브 불변 | 32,754,660 |
| 225 | #10139 Prevent AI illegally targeting itself in doubles | `ade2ec20fc` | 게임 코드는 HnS에 이미 같은 조건(`battle_ai_main.c`, `1821fd6749`) → 테스트만(`ai.c` 새 3개 PASS, `trainer_slides.c` Memento hunk — 뒤 행 #10046·#10133 일부 선반영). ROM 같음 | 32,754,660 |
| 226 | #10147 Fix HGSS Dex Form Strings | `65ba1c6e4d` | 그대로. 바뀐 영문 2개는 `HGSS_DECAPPED`(HnS FALSE)에서만 쓰여 화면·ROM 영향 없음 | 32,754,660 |
| — | **upstream #10837 Fix ITEM_NONE assertion when canceling mail(1.17.1, 앞당김)** | `e804379040` | 리뷰 R1 발견: 219 뒤 빈손 포켓몬에게 편지를 주다 쓰기를 그만두면 `RemoveBagItem(ITEM_NONE)` → assert(이식 전 아무 일 없음). HnS는 배틀프론티어 라운지 교환(냐옹, 레트로메일)으로 편지를 얻음. upstream과 같은 2곳 `if (sPartyMenuItemId != ITEM_NONE)`. 이식 전 동작 복원(맹세 가드와 같은 원칙, 메인) | 32,754,676 |

## 동작 변화

- 없음(배틀 메시지·세이브·한글 출력 같음). 220 트리거 위치는 디버그로만 보임(Gen3 UI 싱글 12 px·더블 4 px 위로 = upstream 의도, Gen4 UI 메가 그대로, Gen4 UI 메가 외 트리거는 체력박스 테두리 뒤로 약 41 px — 이식 전에도 Gen4에 맞춘 위치가 아니었음).

## 검증

- **빌드:** 커밋마다 종료 코드 0, 새 경고 0. 최종(`e804379040`) ROM **32,754,676 B**(묶음 전 −240), EWRAM 250,408 B, IWRAM 25,516 B(변화 0), SHA1 `b8aa06d036af82dfc6cd6a676a9e0bcb286f6349`. ROM이 같아야 하는 218·222·223·225·226은 SHA1이 직전과 같았다.
- **전체 테스트:** PASS 2,585 → **2,588** / TOTAL 5,499 → 5,502. 사라진 PASS 0(`testlist.sh`의 lost 1은 223 이름 변경 `Beat Up's damage is typeless` → `… (up to Gen4)`, 둘 다 PASS). 새 줄 = 225 `ai.c` 새 PASS 3. 목록 [`test-baseline-seq226.txt`](test-baseline-seq226.txt)(**다음 비교 기준**).
- **한글 회귀 607개:** 기대와 바이트 같음(517/607).
- **세이브:** 정적 비교(이식 전 사실 = HEAD `ad4d9c50aa` 빌드) **PASS(FAIL 0, WARN 0)**, 세이브 왕복 PASS(가방 포함 `load.txt` 95줄 같음).
- **리뷰 2개:** R1(217·219·222·226) — 219 새 assert에 닿는 HnS 경로 전수(스크립트 도구 명령 986곳, C 호출 97곳): 열매 3곳(적응 줄로 막음) 말고 **편지 쓰기 취소 2곳** 발견 → upstream #10837 앞당김(`e804379040`). 볼 값·세이브 값 그대로, `TryTakeMonItemResult` 한글 문장 3개 그대로. R2(218·220·221·223·224·225) — 수정 필요 0(221 NULL 목적지 재확인, 224 난수 소비·결과 같음, 220 그림·`.gbapal` 규칙·위치), 참고: 커밋 메시지 숫자(12,911), 분석 문서 문구 2곳(`returnram` 1건·vgoto 25건 — 결론 같음).
- **메인 검증(최종 HEAD):** 재빌드, 전체 테스트 목록이 적용 뒤 목록과 바이트 같음, 한글 607개 같음, 세이브 정적 PASS(WARN 0)·왕복 PASS.

## 재확인·뒤 PR 메모

- #10837(1.17.1)은 이미 적용 — 1.17.1 이후를 맞출 때 건너뜀(재확인 39).
- 225: 뒤 행 seq 354 #10046의 `trainer_slides.c` Memento hunk는 이미 적용 상태, seq 362 #10133 일부도.
- 38(새, 이식 전부터 HnS 결함, 친구 확인): HnS 볼 바꾸기에서 GS볼을 쓰면 이름이 "상처약"으로 나오고 GS볼은 줄지 않으며 가방에 상처약이 있으면 1개 빠짐(`src/party_menu.c` 7103·7116·7124행 근처). GS볼을 볼 바꾸기에 써도 되는지 HnS 설계 질문. 코드 분석, 실기 미확인.

## 실기(mGBA) 확인 항목

- (선택) 16번도로 구구 / 얼음샛길 B3F 꾸꾸리·딜리버드 열매 이벤트에서 가방을 열었다 취소 → 이식 전처럼 "싫어하는 열매" 흐름.
- (선택) 빈손 포켓몬에게 레트로메일을 주고 쓰기를 그만둠 → 아무 일 없음(오류 화면 없음).
- (선택, 디버그) 메가링·Z파워링을 주고 트리거 위치.
