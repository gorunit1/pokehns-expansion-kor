# full-sync 묶음 12 — seq 227~239 (+ HnS 가드 2개·리뷰 수정 2개)

- 시작 HEAD `1ba0227073`(코드 = `738d089968`, 2026-10-10 실버 멀티 머그샷 뒤). 작업 컴퓨터: 데스크탑(사용자는 랩탑에서 Remote Control). 날짜 2026-10-10.
- 방식: PR별 사전 분석 4개(병렬, 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-227-239/seq*.md`) → 사용자 결정 4개("ㄱㄱ", 혼자 정함) → 적용 1개(커밋 13개, 손 맞춤 0) → 리뷰 2개(R1에서 1건 수정 필요 + 경미 1건 → HnS 커밋 2개, 경미 건은 사용자 결정) → 메인 검증.
- 실기(mGBA) 미확인.

## 커밋

| seq | PR | 커밋 | 판정·HnS 적응 | ROM(B) |
|---|---|---|---|---:|
| 227 | #10141 More Gen VII animation data added | `41e4f16717` | 13종(달콤아·달무리나·달코퀸·치갈기·큐아링·코스모그·타타륜·짜랑랑·자말라·자마슈·마셰이드·모래꿍·모래성이당) 앞모습 2프레임 `anim_front.png`, 옛 `front.png` 삭제, `gen_7_families.h` 13 hunk 그대로. `pokemon.h`는 HnS INCBIN 형태(`anim_front.4bpp.smol`, #9881 미이식), 달콤아 줄 끝 탭 제거. HnS·친구가 바꾼 적 없는 그림, 팔레트 = `normal.pal`. 정지 그림(프레임 0)도 upstream 다시 그림이라 최대 66 px 다름 | 32,759,188 |
| 228 | #10161 Fixes Future Sight attacker in party issues | `18a69b4d96` | 손 맞춤: `8bcf557c20` `BattlersShareParty` 조건을 새 static 함수로, `CancelerMultihitMoves`는 `GetMoveEffect(cv->move)`(HnS `cv`에 `moveEffect` 없음), 나쁜손버릇 `gBattlerAttacker`, #10344 남은 레드카드·탈출버튼 2줄 포함(재확인 8h, 빼면 한글 K9-01·02 FAIL), `8552b5e9a8` 되돌림(재확인 8e — 보존 유무 trace 같음). 출력 변화 아래 | 32,759,028 |
| — | **HnS: 미래예지 착탄 그 자리 포켓몬 기록 보존**(사용자 결정 1) | `2c3c403c66` | 1.17.0에서는 턴 끝 착탄의 `moveendall`이 그 자리 포켓몬의 난동(1턴 단축)·길동무(사라짐)·연속 사용 카운터(0)·분함의발구르기 타이머·진화 카운터(+1, 세이브 값)·`unableToUseMove`를 갱신 → `IsEndTurnFutureSightHit()`로 건너뜀(이식 전 = 본가). 관련 테스트 14파일 상태 변화 0, 한글 607 trace 같음 | 32,759,284 |
| 229 | #10169 Fixes Toxic Poison counter not being incremented on Poison Heal | `a8dd91c1a0` | 플라엣테 폼 2줄은 친구 커밋 `db5e5f9c51`로 이미 있음(제외), `MoveEndEmergencyExit`는 HnS move end에 `cv`가 없어 `GetBattlerAbility(i)`. 새 테스트 2 PASS | 32,759,444 |
| — | **HnS: 포이즌힐 AI 맹독 교체 가드**(사용자 결정 2) | `d87055c301` | 229 뒤 AI가 포이즌힐 포켓몬도 맹독 카운터로 교체 후보로 봄(웅 재전 글라이온이 4턴째부터 50%, 1.17.0 같음) → `monAbility != ABILITY_POISON_HEAL` 1줄(이식 전 AI, 매직가드 조건과 같은 값) | 32,759,460 |
| 230 | #9642 Add test support for inventory management in battle | `89537a1ec3` | 그대로. 게임 ROM SHA1 같음, 테스트 새 PASS 3 | 32,759,460 |
| 231 | #10186 Fixes Supreme Overlord and Last Respects wrong faint counter | `eb23e9a308` | HnS `MoveEndFaintBlock(void)` 형태에 맞춰 `SetValuesOnFaint(gBattlerTarget)`. 원념·길동무 순서·맹세 가드 hunk 밖. HnS AI 총대장 예측 1줄도 트레이너별(사용자 결정 3). `BattleStruct` +4 B(heap, 세이브 무관). 새 PASS 4 | 32,759,604 |
| 232 | #10194 Check if species is disabled when filling pc boxes | `d6b593497f` | 그대로(디버그). HnS 비활성 종은 1104부터라 빈 PC 결과 같음 | 32,759,732 |
| 233 | #10210 Fix declaration after label errors | `ea28f61165` | 중괄호 2곳. 크기 같음, `__LINE__` 상수로 SHA1만 다름 | 32,759,732 |
| 234 | #10216 Fixes Curse wrong animation | — | 이미 적용(`1713d764ce`, #9730 단위) | — |
| 235 | #10208 More Gen VII Animation Data | `daa23a4a75` | 14종 그림·`pokemon.h`·애니메이션 값은 친구 업로드 `1821fd6749`로 이미 있음 → 모래꿍 1.17.0 그림, 쓰지 않는 옛 `front.png` 14개 삭제, 춤추새 3폼 `frontAnimId` 줄 위치만(값 같음, `-Woverride-init` 0) | 32,759,700 |
| 236 | #10198 mapjson·how_to_frlg.md | — | 이미 같음(#9949 이식 `643874efb5`의 `layout_matches_version()`), 문서 hunk 넣지 않음(사용자 결정 4) | — |
| 237 | #10225 Remove the preproc around Strange Ball OW graphic | `5127f5c148` | `object_event_graphics.h`는 HnS INCBIN 유지. 볼 번호 0 동행 포켓몬의 볼 그림이 이상한볼(GS볼 28은 여전히 몬스터볼 그림), 팔레트 태그 0x116A 충돌 없음 | 32,761,588 |
| 238 | #10227 Fix Sea of Fire damage immunities | `02b87b322c` | 그대로. #9918 팀 토큰 hunk·#10214와 다른 블록. 새 테스트 1은 영문 MESSAGE라 FAIL(한글 임시 테스트로 이식 전 FAIL → 이식 후 PASS 확인) | 32,761,636 |
| 239 | #10229 Fix OHKO moves ignoring Glaive Rush accuracy bypass | `563dd7af11` | 그대로. 새 PASS 1 | 32,761,652 |
| — | **HnS: 빗나간 미래예지도 끝 스크립트로**(리뷰 R1, 수정 필요) | `e25578410d` | HnS에만 남은 착탄 명중 판정(`accuracycheck`, upstream은 #9939 seq 470에서 없앰)의 빗나감이 `BattleScript_MoveEnd`(`end`)로 가 `clearspecialstatuses`를 건너뜀 → 228 뒤 `attackerInParty`가 남아 그 자리 포켓몬의 특성·도구가 다음 턴 첫 행동까지 없는 것으로(먹다남은음식 회복 없음, 포이즌힐이 독 피해, 구애스카프 순서 뒤집힘 — 사본 실측). 빗나감을 `BattleScript_DoFutureAttackResult`로 합류(문장 같음). **재확인 2(빗나감마다 스크립트 스택 1칸 누수)도 해결**(깊이 0 실측). 이식 전 동작 복원(메인) | 32,761,668 |
| — | **HnS: 미래예지 착탄 때 그 자리 포켓몬의 숨은 상태 유지**(리뷰 R1 경미, 사용자 결정 5) | `6155f43690` | 그 자리 포켓몬이 구멍파기·공중날기 등 충전 중일 때 착탄이 효과 없음·빗나감이면 1.17.0은 숨은 상태를 풀어 다음 턴 공격을 맞음 → 아직 숨어 있으면 `MOVEEND_ATTACKER_VISIBLE`을 건너뜀(본가와 같음. 이식 전은 효과 없음일 때 유지, 빗나감일 때 풀림). Stellar 소모는 실게임에서 닿지 않아 그대로 | 32,761,764 |

## 동작 변화 (`BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 행 추가)

- **228 미래예지·파멸의소원 턴 끝 착탄:** move end 전체가 돈다 — 위기회피·도망태세 교체가 팝업 직후 그 자리에서(같은 턴 소원·두 번째 미래예지보다 먼저), 쓴 포켓몬이 필드에 있으면 생명의구슬 반동·자기과신·독사슬 발동. 쓴 포켓몬이 파티에 있으면 그 자리 포켓몬의 특성·도구는 끼어들지 않음. 레드카드·탈출버튼은 이식 전처럼 발동하지 않음. 그 자리 포켓몬의 기록·숨은 상태는 HnS 가드로 이식 전과 같음. 새 문장 없음.
- **231 총대장·성묘:** 멀티배틀에서 자기 트레이너 파티의 기절만 센다(본가). HnS 트레이너 데이터에 해당 특성·기술 0건 → 플레이어 포켓몬·랜더마이저에서만.
- **238 불바다:** 불꽃 타입·매직가드는 피해·문장 없음.
- **239 일격기:** 돌진을 쓴 상대에게는 반드시 명중.
- **227·235 그림:** Gen 7 13종 앞모습 2프레임 애니메이션(227), 235의 14종은 이미 같았고 모래꿍만 1.17.0 그림으로. **237** 이상한볼 동행 볼 그림(디버그로만 보임).
- 그 밖 229·230·232·233은 화면 출력 변화 없음.

## 검증

- **빌드:** 커밋마다 종료 코드 0, 새 경고 0, EWRAM 250,408 B·IWRAM 25,516 B 그대로. 최종(`6155f43690`) ROM **32,761,764 B**(묶음 전 +6,864), SHA1 `b45491dac46a6188ea80a86d6596c2bdf4a00575`. 230은 SHA1이 직전과 같고, 233은 크기 같고 SHA1만 다름(예측대로).
- **전체 테스트:** PASS 2,588 → **2,598** / TOTAL 5,502 → 5,515. 사라진 PASS 0, 기존 줄 상태 변화 0. 새 줄 13 = 예측: 228 새 FAIL 2(독사슬+미래예지, 영문 MESSAGE), 229 새 PASS 2, 230 새 PASS 3, 231 새 PASS 4, 238 새 FAIL 1(영문 MESSAGE), 239 새 PASS 1. 리뷰 수정 2개 뒤에도 목록 같음. 목록 [`test-baseline-seq239.txt`](test-baseline-seq239.txt)(**다음 비교 기준**).
- **한글 회귀 607개:** 기대와 바이트 같음(517/607). 607개 trace도 적용 전후·리뷰 수정 전후 같음.
- **세이브:** 정적 비교(이식 전 사실 = HEAD `e76b4165ff` 빌드) **PASS(FAIL 0, WARN 0)** — INFO 1(`RecordedBattle_CheckMovesetChanges`의 `gBattleStruct` 오프셋 상수 +4, 231 `faintCounter`, heap). 세이브 왕복 PASS(섹터 0~30·`load.txt` 95줄 같음).
- **리뷰 2개:** R1(배틀 엔진) — 228 수정 필요 1건(빗나간 미래예지 → `e25578410d`), 경미 1건(숨은 상태 → 사용자 결정으로 `6155f43690`), 미래예지 가드 조건이 턴 끝 착탄 밖에서 참이 되는 경우 0(전체·한글 실행 계측 110건 모두 착탄 안), `8552b5e9a8` 되돌림 뒤 대기 교체가 지워지는 경로 0, 229 AI 가드·231·233·238·239 문제 없음. R2(227·235·237·230·232) — 문제 없음. 정보 2건: 230 테스트 가방이 PARAMETRIZE 사이에 비워지지 않음(upstream 같음, seq 513 #10537, 재확인 8u), 237 살아 있는 포켓몬이 없을 때 동행 볼 그림 기본값이 이상한볼(1.17.0 같음).
- **메인 검증(최종 HEAD `6155f43690`):** 재빌드 SHA1 `b45491da…`·새 경고 0, 전체 테스트 목록이 적용 뒤 목록과 바이트 같음(= `test-baseline-seq239.txt`), 한글 607개 기대와 같음, 세이브 정적 PASS(WARN 0)·왕복 PASS(`chunk-227-239/main/main-verify.sh`).

## 재확인·뒤 PR 메모

- 해결: 재확인 2(`e25578410d`), 8e·8h(`18a69b4d96`).
- 새로: 8u(테스트 가방, seq 513), 43(미래예지 그 자리 포켓몬 가드 2커밋), 44(포이즌힐 AI 가드), 45(HnS 빗나감 분기 — seq 470 #9939 때 정리), 46(231 AI 1줄 — seq 383 #10145 손 병합).
- 사전 분석 문서의 "#9939 seq 281"은 seq 470이 맞다(281은 order 열).
- 228 미래예지 빗나감(공중날기 회피 경로)의 `그러나 실패하고 말았다!` 한 줄은 이식 전부터 같은 출력(seq 470 때 확인).

## 실기(mGBA) 확인 항목

- (선택) 초련·일목 재전: 미래예지가 떨어질 때 생명의구슬·자기과신 등, 교체한 뒤 떨어지면 그 자리 포켓몬이 먹다남은음식 등을 정상으로 쓰는지(빗나간 경우 포함).
- (선택) 웅 재전 글라이온(포이즌힐+맹독구슬)이 맹독으로 교체되지 않는지.
- (선택) Gen 7 앞모습(예: 달콤아·큐아링·코스모그·모래꿍, 235의 춤추새 4폼) 배틀 등장 애니메이션.
