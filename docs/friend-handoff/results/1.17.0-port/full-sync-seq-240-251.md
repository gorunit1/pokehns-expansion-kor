# full-sync 묶음 13 — seq 240~251 (+ #10424 앞당김)

- 시작 HEAD `487a56e64c`(코드 = `7fff683741`, 2026-10-10 친구 요청 2커밋 뒤 — 분석 기준 사본은 `6155f43690`, 그 사이 커밋은 그림·볼 창 팔레트뿐). 작업 컴퓨터: 데스크탑(사용자는 랩탑에서 Remote Control). 날짜 2026-10-10.
- 방식: PR별 사전 분석 4개(병렬, 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-240-251/seq*.md`, 2026-10-10 API 주간 한도로 A·B·C가 한 번 끊겼다가 리셋 뒤 이어서 — 끊긴 동안 끝난 백그라운드 빌드·테스트는 완료 표시를 확인하고 썼다) → 결정(모두 사용자 혼자 정할 수 있는 범위, 사용자 "혼자 가능하면 ㄱㄱ") → 적용 1개(커밋 11개, 손 맞춤 0) → 리뷰 2개(수정 필요 0) → 메인 검증.
- 실기(mGBA) 미확인.

## 커밋

| seq | PR | 커밋 | 판정·HnS 적응 | ROM(B) |
|---|---|---|---|---:|
| 240 | #10226 Fix Psycho Shift status transfer interactions | `1e17ecd60e` | 그대로. `waitstate`·`trysynchronize`가 `printfromtable gCureStatusStringIds`(#9655 이식 뒤 upstream과 같은 형태) 바로 앞. 출력: 대타에게 `그러나 실패하고 말았다!`, 틈새포착이 신비의부적 무시, 싱크로 상대면 상태 문장과 치료 문장 사이 특성 팝업(새 문장 없음). AI: 상태이상일 때 사이코시프트 −10 버그 수정(HnS 네이티오·야부엉 트레이너 6명), 교체 AI 맹독 계산 괄호 | 32,763,076 |
| 241 | #10231 Fix Emergency Exit being skipped after Shell Bell recovery | `ec35733cc2` | 그대로. `SpecialStatus` 남는 비트(크기·EWRAM 같음). 조개껍질방울로 HP가 절반 위로 돌아와도 위기회피·도망태세 발동(기존 문장만). `hns_dancer`(X4) 21/21 | 32,763,172 |
| 242 | #10219 Fix incorrect style flagging for Battle Factory | `69daed80cc` | 그대로. 자폭·대폭발이 하이리스크(원래 pokeemerald 표) — 팩토리 대기실 힌트 대사 선택만(대사는 영문), 무작위 파티 20,000개 중 4.0% | 32,763,172 |
| 243 | #10185 Fixes Mirror Herb erroneous activations | `5e66c100f4` | 그대로(테스트 끝 빈 줄만). 흉내허브·편승의 헛발동 제거(이미 +6, 흉내허브끼리 재복사, 문장만 나오고 안 오르던 경우), 편승 같은 턴 재복사는 upstream·본가 쪽. `ProtectStruct`·`StatChange` 크기 같음, HnS 토템 테스트 5/5 | 32,763,236 |
| 244 | #10263 Fixes TARGET_FIELD moves failing when partner is not on the field | `dbfb910aa0` | 그대로(`Controller_WaitForHealthBar` 줄 끝 공백 hunk만 제외 — HnS가 이미 바꿈). **이식 전에는 싱글(더블도) AI의 경혈찌르기가 플레이어 포켓몬을 +2 올렸다(버그) → 이제 AI 자신**(사도 2차 요가램 등 5명). 필드·전원 기술 24개는 메시지·판정 같고 애니메이션 대상만 사용자 쪽. 상대·파트너 컨트롤러는 upstream과 바이트 같음(재확인 41 무관) | 32,763,252 |
| 245 | #10272 Fix Mimikyu Busted and Eiscue Noice reverting when revived | `fad5a4cc52` | 폼 표 3줄 삭제 그대로, `revival_blessing.c` 마지막 hunk만 HnS 문맥(#10144 미이식, 바뀌는 줄은 같음). 배틀 중 되살린 정체 드러난 따라큐·노아이스 빙큐보는 탈·아이스페이스 재발동 없음. 배틀 끝 폼 복귀가 Mirror·Nuzlocke 처리보다 먼저라 세이브에 남지 않음(리뷰 R2 재확인). HnS 기본 데이터에 없음(랜더마이저·교환) | 32,763,220 |
| 246 | #10262 Fix Trick Room animation playing on wrong battler | `4aed6b8978` | 그대로(애니메이션 스크립트 1바이트). 트릭룸·원더룸·매직룸을 쓸 때 작아지는 포켓몬이 사용자 | 32,763,220 |
| 247 | #10287 Totem aura message | — | 이미 적용(#9730 단위 `a5eb87ba6b`, `{B_ATK…}`) | — |
| 248 | #10289 Fixes Binding Band no longer affecting bound battler's hp decrease when nullified | `66b3d97df9` | 그대로. 휘발 비트 `wrappedBindingBand`(`gBattleMons` 0x240 그대로). 묶은 쪽 도구가 엠바고·탁쳐서떨구기 등으로 무효화돼도 턴 끝 피해 1/6 유지(문장 같음). HnS에서 조임밴드는 랜더마이저로만 | 32,763,268 |
| 286→ | **#10424 Fix AI Binding Band check for Wrap damage(앞당김)** | `d8f5827101` | 같은 unit 후속 1줄. 248만 넣으면 AI 추정(1/8)과 실제(1/6)가 어긋나고(전지 아닌 AI가 HP 71에서 칼춤 → 쓰러짐), 기존 HnS의 오래된 `wrappedBy` 오판도 고침. seq 286 때 "이미 적용"(재확인 48) | 32,763,220 |
| 249 | #10288 Fixes Pursuit activating Eject Button on switching foe | `56bed89ebc` | 손 맞춤 2곳(`TryEjectButton`·`TryEjectPack`에 `IsPursuitTargetSet()`을 1.17.0 자리에 — HnS에 이미 스카이드롭 헬퍼·미래예지 제외 줄). 교체 중 따라가때리기에 맞은 포켓몬의 탈출버튼·탈출팩·위기회피가 발동하지 않음(목호 재전 갸라도스가 도구를 지킴), 따라가때리기를 쓴 쪽의 위기회피도 그 턴 발동하지 않음(1.17.0·1.17.1·master와 같음, 리뷰 R1 실측). 영문 FAIL 옛 테스트 1개 삭제(upstream) | 32,763,268 |
| 250 | #10281 FRLG maniac OW rule | — | 이미 같음(#9537 `70eb6a4271`, `#if IS_FRLG`라 HnS ROM 밖) | — |
| 251 | #10278 Fix Incinerate item handling and Core Enforcer targeting | `51f6f5f85d` | 그대로(테스트 1줄 끝 공백만). 불사르기가 서투름·금제·매직룸으로 효과가 막힌 쥬얼도 태움, 코어퍼니셔 `TARGET_BOTH`(`moves_info.h` `.target` 1줄, 한글 이름·설명 그대로) | 32,763,268 |

## 동작 변화 (`BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 행 추가)

- 240 사이코시프트(대타 실패·틈새포착·싱크로 팝업), 241 조개껍질방울 뒤 위기회피, 244 AI 경혈찌르기 자기 강화·필드 기술 애니메이션 대상, 245 되살린 따라큐·빙큐보, 249 따라가때리기와 탈출 도구·위기회피, 251 불사르기 쥬얼·코어퍼니셔. 새 한글 문장 없음.
- 243은 헛발동이 사라질 뿐(문장이 빠지는 경우만), 242·246·248은 화면 문장 변화 없음.

## 검증

- **빌드:** 커밋마다 종료 코드 0, 새 경고 0, EWRAM 250,408 B·IWRAM 25,516 B 그대로. 최종(`51f6f5f85d`) ROM **32,763,268 B**(묶음 전 +448), SHA1 `e90a4680b873cf790b18928746850102c61de19f`(리뷰 R1이 오브젝트 없이 다시 빌드해도 같음).
- **전체 테스트:** PASS 2,598 → **2,621** / TOTAL 5,515 → 5,536. 사라진 PASS 0. 새 줄 23 = 분석 예측(240 PASS 3, 241 1, 243 3, 244 1, 245 2, 248 2, 249 3, 251 7 + 수확 TO_DO → PASS), 249의 영문 FAIL 옛 테스트 1줄 삭제(upstream). 목록 [`test-baseline-seq251.txt`](test-baseline-seq251.txt)(**다음 비교 기준**).
- **한글 회귀 607개:** 기대와 바이트 같음(517/607). 607개 trace가 이식 전과 바이트 같음(리뷰 R1).
- **세이브:** 정적 비교 **PASS(FAIL 0, WARN 0)** — INFO 1(`RecordedBattle_CheckMovesetChanges` 비트 시프트 #30 → #26, 248 휘발 비트 추가로 뒤 비트 위치 이동, 배틀 중 RAM이라 세이브·녹화·링크 무관). 세이브 왕복 PASS.
- **리뷰 2개:** R1(240·241·243·248·#10424·249) 문제 없음 — 분석 임시 테스트 52개를 최종 HEAD에서 다시 돌려 분석 "이식 후"와 같음, 참고 4건(아래). R2(242·244·245·246·251) 문제 없음 — 이식 전 되돌림 사본과 trace 비교, AI 경혈찌르기·필드 기술 24개·애니메이션 6상황 실측, 참고 2건(아래).
- **메인 검증(최종 HEAD):** 재빌드, 전체 테스트 목록, 한글 607개, 세이브 정적·왕복(`chunk-240-251/main/main-verify.sh`).

## 재확인·뒤 PR 메모

- 47(한글 문구, 친구 확인): 불사르기 문장 `STRINGID_INCINERATEBURN` 출력이 `상대 마자용의\n고스트주얼은 녹여 버렸다!` — 타동사 앞 조사가 `은/는`(이식 전부터). 251 뒤에는 효과가 막힌 쥬얼에도 나온다.
- 8v: seq 488 #10595가 대타 판정을 move resolution으로 옮기며 240의 `jumpifsubstituteblocks` 줄을 지운다 — 그때 정리 확인.
- 8w: 244 뒤 AI 파트너의 경혈찌르기가 플레이어 포켓몬을 고를 수 있고 그 포켓몬이 먼저 쓰러지면 실패(upstream 중간 상태, seq 332 #10645가 보완, HnS 파트너 데이터에 경혈찌르기 없음).
- 8x: 250 규칙은 seq 500 #9881 INCGFX 때 1.17.0 형태(인자 포함)로.
- 48: #10424(seq 286)는 이미 적용 — 그 자리에서 건너뜀.
- 참고(리뷰 R1, 조치 없음): 241 `shellBellEmergencyExit` 비트는 행동 끝까지 남아 상대 탈출버튼 대기 교체로 위기회피를 건너뛴 뒤 같은 행동의 춤추기에서 발동할 수 있음(upstream 같은 코드, HnS 고정 트레이너에 위기회피 없음). #10424 AI `ShouldTrap`이 예전 묶임의 `wrappedBindingBand`를 읽음(1.17.0 같음, 이식 전에도 옛 `wrappedBy`).
- 뒤 행: #10295(seq 255, #10231·#10288 의존), #10433(seq 303, #10185 뒤), #10645(seq 332, #10263 뒤).

## 실기(mGBA) 확인 항목

- (선택) 사도 2차·포스트 OBC 요가램 등 AI 경혈찌르기 → `상대 …의 …이 크게 올라갔다!`(자기 강화). AI 중력 애니메이션.
- (선택) 목호 재전 갸라도스(탈출버튼): 따라가때리기로 교체를 쫓을 때 탈출버튼이 발동하지 않는지.
- (선택) 트릭룸을 쓸 때 작아지는 포켓몬이 사용자인지.
