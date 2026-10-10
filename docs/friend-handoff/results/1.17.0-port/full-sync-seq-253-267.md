# full-sync 묶음 14 — seq 253~267 (+ HnS 가드 1개)

- 시작 HEAD `8c90e56c8a`(코드 = `51f6f5f85d`). 작업 컴퓨터: 데스크탑(사용자는 랩탑에서 Remote Control). 날짜 2026-10-10.
- 방식: PR별 사전 분석 4개(병렬, 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-253-267/seq*.md`) → 결정(모두 사용자 혼자 정할 수 있는 범위, 사용자 "혼자 가능하면 ㄱㄱ") → 적용 1개(커밋 13개, 손 맞춤 0) → 리뷰 2개(수정 필요 0, 경미 2 — 기존 결함 1건은 HnS 가드 커밋) → 메인 검증.
- 실기(mGBA) 미확인.

## 커밋

| seq | PR | 커밋 | 판정·HnS 적응 | ROM(B) |
|---|---|---|---|---:|
| 253 | #10306 Fix Eerie Spell PP reduction for Max Moves | `30ebf75ff9` | 그대로. 다이맥스 기술 뒤 섬뜩한주문이 원래 기술 PP를 3 깎고 기존 문장 `…의\n…을(를) 3 깎았다!`(HnS 본 게임은 다이맥스 불가 — 테스트·랜더마이저 밖) | 32,763,268 |
| 254 | #10307 Fix Mental Herb leaving volatile status timers uncleared | `f162b0f3c7` | 그대로. 멘탈허브가 회복봉인·트집을 푼 뒤 원래 타이머 끝에 `…효과가 사라졌다!`가 한 번 더 나오던 결함 제거(한글 `HNSPOPUP 06` trace 1줄, 요약 같음) | 32,763,300 |
| 255 | #10295 Fix Commander simultaneous switch and unintended Dondozo switch-outs | `bad0bc21e1` | 손 맞춤 3곳(`TryEjectButton`·`TryEjectPack`·`TrySwitchInEjectPack` 문맥, 묶음 13 #10231·#10288 줄 뒤, 1.17.0 자리). 사령탑 어써러셔의 탈출버튼·탈출팩·위기회피 없음, 배턴터치·순간이동·꼬리자르기 등 `그러나 실패하고 말았다!`, 동시 교체로 들어온 어써러셔에 사령탑 없음 | 32,763,476 |
| 256 | #10309 Fix moveanim being played after hitanim and healthbarupdate | `34c741f969` | 그대로 2줄(HnS는 이식 전에도 순서가 맞았음 — 방어용 초기화, trace 변화 0) | 32,763,492 |
| 257 | #10315 Fixes Dragon Darts after hit messages | `4ef95ae3b2` | 손 맞춤 2곳: HnS 스케일샷 분기 유지, **HnS Champions 4배·1/4배 상성 분기에도 `TARGET_SMART` 조건**(사용자 결정 — 1.17.0은 이 경우 상성 문장이 없어지는데 HnS는 이식 전 `효과가 매우 굉장했다!!`를 다트마다 유지). 다트마다 상성 문장, `N번 맞았다!` 없음 | 32,763,572 |
| 258 | #10285 | — | 이미 적용(`fefd96fa4f`, #9730 단위) | — |
| 259 | #10322 Handle weather blending for BLEND_IMMUNE_FLAG | `5dea0f72a6` | `money.c` 손 맞춤(HnS BP 라벨 줄 옆, `MONEY_LABEL_TAG`만 면역). 야외 돈 창(분노의호두과자 상인 등)의 돈 라벨이 날씨·밤·번개에 혼자 물들지 않음. 대화창·돈 창 BG 팔레트 전후 같음 | 32,763,652 |
| 260 | #10332 Fix excessive indentation in trainer_pools.c | `409d59498b` | 그대로(ROM SHA1 같음) | 32,763,652 |
| 261 | #10325 Fix Symbiosis not triggering after Cheek Pouch | `4c623aea8f` | `MoveEndThirdMoveBlock`만 손 맞춤(HnS `gBattlerAttacker`). 볼주머니 회복 뒤 공생 팝업·`…은(는) …(으)로부터\n…을(를) 받았다!`(기존 문장) | 32,763,652 |
| 262 | #10354 Fix Thousand Arrows not grounding both targets | `badf2b354d` | `MoveEndMoveBlock`을 HnS 인자 없는 함수 형태로 다시 씀(#9859 미이식, 도둑질 vs 점착·`magnetRiseTimer`·스케일샷 case 없음 유지 — 리뷰 R2가 upstream과 정규화 비교). **`battle_message.c` 토큰 4개 교체 제외**(사용자 결정 — 1.17.0에서 날려버리기·울부짖기·드래곤테일 장면에 엉뚱한 이름이 나오는 것을 실측, HnS는 바른 이름), 떨어뜨리기 스크립트 3줄. 빈 `smack_down.c`를 upstream 파일로(#10213 테스트 6개 동반). 더블에서 공중 상대 둘 다 접지, **킬러스핀 마지막 대상이 방어·독 무효여도 압정 제거**(리뷰 R2 발견, upstream과 같음) | 32,763,908 |
| 263 | #10357 Fix Commander suppression and overwrite flags | `15545d918d` | 그대로. 사령탑이 위액·화학변화가스·고민씨·심플빔·동료만들기·미라·역할에 걸림(SV Ver.4.0.0 실기 기준, 스킬스왑은 그대로 실패) | 32,763,908 |
| 264 | #10358 Fix incorrect Mummy and Doodle ability tests | `a00f55ce28` | 테스트만 | 32,763,908 |
| 265 | #10345 Gust colour cycling | — | 이미 적용(`45b27b9bce`, seq 83 단위, HnS `TryLoadPal` 가드 유지) | — |
| 266 | #10350 Fix some Mirror Armor issues(남은 부분) | `598b9db0c3` | 앞부분은 `e4fe919194`. **`B_MIRROR_ARMOR_STICKY_WEB GEN_8`**(사용자 결정 — 지금 HnS = 8세대: 끈적끈적네트를 깐 포켓몬이 필드에 있으면 그쪽 스피드가 떨어짐, 9세대는 아무도 안 떨어짐 — 출처 결과 문서 아래). **HnS 적응:** 깐 포켓몬이 나간 뒤에는 팝업만(1.17.0 `GEN_8`은 엉뚱한 포켓몬 스피드를 내림 — 리뷰 R1 9경우 실측). 출력 변화 없음 | 32,763,924 |
| 267 | #10317 Fix Commander edge cases with Multi Battles, switching, and gimmicks | `568a20f8f0` | 손 맞춤 2곳 + 테스트 줄 끝 공백. 같은 편에 다른 트레이너가 있으면(멀티·파트너·배틀타워 멀티) 사령탑 미발동, 사이드체인지 실패, 숨은 싸리용 탈출팩 막힘, 대기 기믹 취소. HnS 데이터에 싸리용·어써러셔 없음 | 32,764,100 |
| — | **HnS: 코트체인지 뒤 미러아머 범위 밖 쓰기 가드**(리뷰 R1, 기존 결함) | `68035b152d` | 깐 포켓몬이 나간 끈적끈적네트(설치자 0xFF)를 코트체인지로 옮기면 0xFE가 되어 미러아머 반사가 `gSpecialStatuses` 범위 밖(필드 맵 백업 버퍼)에 쓰고 다음 턴 그 포켓몬 행동이 사라짐(이식 전·1.17.0도 같음, 랜더마이저로만 도달). 설치자 번호가 배틀러가 아니면 팝업만. 코트체인지 매크로는 seq 311 #10542가 다시 쓰므로 그대로 | 32,764,116 |

## 동작 변화 (`BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 행 추가)

- 253 섬뜩한주문 다이맥스 PP, 254 멘탈허브 중복 문장 제거, 255·263·267 사령탑, 257 드래곤애로 다트별 상성 문장, 261 공생 + 볼주머니, 262 사우전드애로 두 대상·킬러스핀 압정. 새 한글 문장 없음.
- 259 돈 라벨 색(필드), 256·260·264·266·코트체인지 가드는 화면 문장 변화 없음(가드는 오류 상황에서 팝업만).

## 검증

- **빌드:** 커밋마다 종료 코드 0, 새 경고 0, EWRAM 250,408 B·IWRAM 25,516 B 그대로. 최종(`68035b152d`) ROM **32,764,116 B**(묶음 전 +848), SHA1 `27ac2c483cc8fe3327d79a4c2e7f57bae16ae12b`.
- **전체 테스트:** PASS 2,621 → **2,648** / TOTAL 5,536 → 5,566. 사라진 PASS는 **267의 1줄**(`Commander will not activate if Dondozo fainted right before Tatsugiri came in` — upstream이 영문 `MESSAGE`를 넣어 HnS 한글 출력과 불일치, 그 줄만 빼면 PASS = 한글 `어써러셔는 쓰러졌다!`), `testlist.sh`의 다른 1줄은 드래곤애로 테스트 이름 대소문자 변경(PASS 그대로). 이름 변경·삭제(263 4·1줄 등)·새 줄은 분석 예측과 같음, 새 FAIL은 모두 영문 `MESSAGE`. 목록 [`test-baseline-seq267.txt`](test-baseline-seq267.txt)(**다음 비교 기준**).
- **한글 회귀 607개:** 기대와 바이트 같음(517/607). trace는 이식 전과 606개 같고 1개(`HNSPOPUP 06`, 254 의도 변화) 다름.
- **세이브:** 정적 비교 **PASS(FAIL 0, WARN 0)**, 세이브 왕복 PASS.
- **리뷰 2개:** R1(254·255·261·263·266·267) — 266 HnS 적응 14경우 trace가 이식 전과 바이트 같음, 사령탑 멀티·파트너 실측, 분석 임시 테스트 35개 재실행 같음, 기존 결함 1건(코트체인지 범위 밖 쓰기 → `68035b152d`). R2(253·256·257·259·260·262·264) — 262 재작성 upstream 정규화 비교, 묶음 12 가드 무변경, 257 손 맞춤 출력 실측, 킬러스핀 출력 변화 기록 누락 1건(위 표·출력 변화 문서에 반영).
- **메인 검증(최종 HEAD):** 재빌드, 전체 테스트 목록, 한글 607개, 세이브 정적·왕복(`chunk-253-267/main/main-verify.sh`).

## 266 config 출처 (분석 C, 2026-10-10 확인)

- 일본 ポケモンWiki: 8세대만 깐 포켓몬에게, 9세대는 아무도 떨어지지 않음(Wayback 사본). upstream #10331·#10350. Bulbapedia(8세대 설명만). Showdown은 9세대 동작이 이들과 달라 참고만. URL·인용은 저장소 밖 `chunk-253-267/seq266-10350.md`.

## 재확인·뒤 PR 메모

- 49(5절): 266 `B_MIRROR_ARMOR_STICKY_WEB GEN_8` + HnS 적응·코트체인지 가드 — 9세대로 바꿀지는 친구 질문, upstream이 이 분기를 고치면 정리.
- 50(1절, 기존): 코트체인지로 옮겨진 설치자 번호가 범위 안이지만 기절해 빈 자리를 가리키면 기절한 포켓몬에게 `…의\n스피드가 떨어졌다!`(이식 전·HEAD 같음, 랜더마이저로만) — 선택 patch 저장소 밖 `chunk-253-267/review-R1/fixCC-alive-optional.patch`(빌드·전체 테스트 전).
- 8y: 더블 지진에서 공중 포켓몬이 피할 때 `…에게는\n맞지 않았다!`에 첫 대상 이름(이식 전후 같음) — seq 470 #9939가 고침.
- 재확인 13 D6c(사령탑 두 번째 이름): 끈적끈적네트 위로 싸리용이 교체돼 들어올 때 `싸리용은 사령탑이 되어\n싸리용에게 삼켜졌다!`를 trace로 확인(분석 B, 267 문서) — 고치려면 두 번째 토큰을 `{B_EFF_NAME_WITH_PREFIX2}`로(출력 변화라 친구 질문).
- 262 토큰 교체 제외 근거(리뷰 R2 추가): 1.17.0에서 더블 드래곤테일이 오른쪽 상대 흡반에 막히면 공격자 파트너 이름이 나옴.

## 실기(mGBA) 확인 항목

- (선택) 드래곤애로(다트마다 상성 문장), 더블 사우전드애로(공중 상대 둘 다 접지), 야외 돈 창(분노의호두과자 상인)의 돈 라벨 색이 밤·비에서 돈 창과 같은지.
