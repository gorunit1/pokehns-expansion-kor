# full-sync 묶음 8 — seq 188·189·190·191·192

- 시작 HEAD `35d0fba9fc`(묶음 7 뒤). 작업 컴퓨터: 데스크탑(사용자는 랩탑에서 Remote Control). 날짜 2026-10-07 밤.
- 방식: PR별 사전 분석 3개(병렬, 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-188-192/seq*.md`) → 사용자 결정 4개("ㄱㄱ") → 적용 1개(커밋 6개) → 리뷰 2개 → 메인 검증. 검증은 저장소 도구 `dev_scripts/hns_verify/`.
- 실기(mGBA) 미확인.

## 커밋

| seq | PR | 커밋 | 판정·HnS 적응 | ROM(B) |
|---|---|---|---|---:|
| — | (HnS) 한글 회귀 세트 HNS9918 | `2250518d81` | 맹세 기술 한글 출력 23개(대기·콤비 문구, 무지개·불바다·습지 시작·턴 끝·종료와 진영, 무지개 추가 효과 2배, 습지 스피드 1/4, 불바다 1/8 피해, 대기 뒤 파트너 잠듦 등)를 **이식 전 출력으로 고정**해 `dev_scripts/hns_verify/kortests/sets/zz_hns9918_pledge.c`에 넣음. 기대 요약 517/607 | — |
| 188 | #9918 Customizeable Pledge Moves | `55e6738f64` | 맹세 조합을 기술 데이터(`argument.pledge`)·새 캔슬러(`CANCELER_PLEDGE_ATTACK`)·추가 효과(`MOVE_EFFECT_RAINBOW/SEA_OF_FIRE/SWAMP`)로. 손 맞춤 3곳: `CancelerInterruptibleMoves`는 #9942(seq 25)로 이미 있어 그 자리에 미래예지·맹세 분기를 합침(enum·표 hunk 제외), `switch`는 `GetMoveEffect(cv->move)`(HnS `DoAttackCanceler`가 `cv.moveEffect`를 채우지 않음 — #9859 미이식), `MoveEndClearBits(void)` 형태. **`battle_message.c` 팀 토큰 hunk 제외**(사용자 결정 1): 불바다·습지 문장 3개를 `{B_EFF_TEAM2}`로 바꾸면 진영 이름이 틀어짐(실측 HNS9918 5개 FAIL), upstream도 #10214(seq 385)에서 되돌렸고 1.17.0 최종 문장은 지금 HnS와 같다 | 32,752,132 |
| 189 | #9805 Adds Ghost battle tests | `538a1256d8` | 러너 3파일(`include/test/battle.h` `BATTLE_TEST_GHOST`, `test_runner_battle.c`, 새 `test/battle/ghost.c`) 그대로. `battle_message.c`는 미번역 FRLG `STRINGID_GHOSTWASMAROWAK` 끝 `\n` 한 글자만(`//frlg` 유지). HnS 러너 수정(`OTName_` 등) 그대로. 유령 배틀은 HnS 실게임에서 닿지 않음 | 32,752,132 |
| 190 | #9968 OWE despawn by movement type | `cbc37d06d5` | `include/config/wild_encounter.h`는 HnS 값 4줄(재확인 8s) 문맥으로 손 맞춤. 새 config `WE_OWE_PREVENT_SPECIAL_MOVEMENT_DESPAWN` = upstream 기본 TRUE(사용자 결정 4 — HnS는 `WE_OW_ENCOUNTERS FALSE`라 동작 같음) | 32,752,180 |
| 191 | #9896 Random Mon Generation | `1b6f5e19eb` | 새 `random_mon_generation.c/h`·스크립트 매크로. 이미 있던 2 hunk(`GetSpeciesBaseStatTotal` 선언, `RNG_RANDOM_BALL`) 제외, `event.inc`·`pokemon.c`·`script_pokemon_util.c` 문맥만 맞춤(HnS `isEgg`·랜더마이저 종 변경 훅·`nuzlocke.h` 유지). 새 스크립트 opcode·config 없음(`callnative`), 이벤트 스크립트 바이트 같음 | 32,752,660 |
| 192 | #9965 wild_encounter config cleanup | `66bbc9b5cf` | 가드 이름·끝 주석(#9968 뒤에서만 붙음). ROM 바이트 동일 | 32,752,660 |
| — | (HnS, 리뷰 R1 발견) 맹세 가드 | `b857f809e0` | #9918 뒤 대기 중인 맹세 사용자 다음 기술이 늘 콤보가 되어, 파트너가 맹세를 실제로 쓰지 않으면(앙코르·불복종·Z기술·다이맥스·탈출버튼 교체, 지휘·잠꼬대·손가락흔들기·선취로 부른 맹세) `assertf` 화면 → 엉뚱한 콤보(자기 편 불바다 등). upstream 1.17.0·master 같은 코드. 지금 기술이나 파트너가 고른 기술이 맹세가 아니면 콤보를 건너뛰고 대기를 풂 + `TurnValuesCleanUp`에서 다시 초기화(#9918 전처럼). 이식 전에는 5가지 정상·4가지 멈춤이었다. HnS는 트레이너·교습에 맹세가 없어 랜더마이저로 배운 맹세로만 닿는다. 테스트 `hns_pledge_guard.c` 10개 | 32,752,756 |
| — | (HnS, 리뷰 R1) HNS9918 K07 보강 | `a1f5e58f85` | 테스트 러너는 추가 효과를 항상 성공으로 처리해 K07이 무지개 2배를 확인하지 못했다 → 맹독엄니 두 번에 `WITH_RNG(RNG_SECONDARY_EFFECT, FALSE)`(무지개 2배를 빼면 K07만 FAIL), `summary.meta` 문구 정정 | — |

## 동작 변화 (받아들인 것, 사용자 결정 2)

- **#9918 드문 더블 맹세 상황 4개**(새 한글 문장 없음, 순서·발생만):
  - 첫 사용자의 대상이 방어 중: 이전 `상대 …은(는) 공격으로부터 몸을 지켰다!` → 파트너 단독 / 이후 대기 문구 → 콤비 → 결과 기술이 파트너 대상에 → 무지개 등
  - 두 사용자 변환자재: 이전 첫 사용자 타입 변경 / 이후 대기하는 첫 사용자는 발동 없음, 두 번째는 콤비 문구 뒤 결과 기술 타입
  - 물+불꽃 콤비 대상이 타오르는불꽃: 이전 흡수(콤비 없음) / 이후 결과 물의맹세가 맞음 + 무지개
  - 불꽃+물 콤비(결과 물의맹세) 대상이 저수: 이전 흡수만 / 이후 콤비 문구가 먼저 나오고 흡수
- #9896: `givemon`/`createmon`에 범위 밖 볼 값·같은 기술 두 칸을 주는 경우만 처리가 바뀜(현재 HnS 스크립트에 해당 사용처 없음, 정상 입력은 결과·난수 같음 — 1,296개 조합 시뮬레이션). HnS `BALL_RANDOM` 30, 무작위 볼 후보에 GS볼 포함, `MOVE_RANDOM_TEACHABLE`은 학습 랜더마이저 대리 종을 따르지 않음.
- 불바다 종료 문장이 가장 빠른 배틀러의 진영을 쓰는 기존 버그(재확인 6)는 그대로(사용자 결정 3 — seq 385 #10214).

## 검증

- **빌드(적용 HEAD `66bbc9b5cf`):** 커밋 6개 모두 종료 코드 0, 새 경고 0. ROM 32,751,812 → **32,752,660 B(+848)**, EWRAM 250,408 B, IWRAM 25,516 B(변화 0), SHA1 `97d6ce7378479f2823afd1c8458ca16bd63c0c4d`.
- **전체 테스트(적용 HEAD `66bbc9b5cf`):** PASS 2,521 → **2,537** / TOTAL 5,426 → 5,447. **사라진 PASS 0.** 새 줄 19 = #9896 새 PASS 16(Random mon generation 15, 폼 표 크기 1) + `ghost.c` FAIL 3(한글 종명·`가랏!` 문장 때문에 영문 `MESSAGE` 불일치). INVALID 21 → 23(`ghost.c` 2개: HnS 볼 재사용 기능 `f8613ba153`으로 몬스터볼이 파티 메뉴 아이템이 되어 러너가 파티 번호를 요구 — 기존 `Capture:` 테스트와 같은 원인, 알려진 한계). (최종 목록은 아래).
- **한글 회귀(607개 = 584 + HNS9918 23):** 이식 전후 모두 517/607, 요약·이벤트 기록 607개 모두 같음. HNS9918 23/23.
- **세이브:** 정적 비교 PASS(FAIL 0, WARN 2 = `move.h` 줄 번호 상수와 그 요약), SaveBlock·RAM 변수 변화 0. 세이브 왕복 PASS.
- **리뷰 2개:**
  - R1(HNS9918 세트·#9918): upstream과 다른 곳은 손 맞춤 3곳과 제외 hunk뿐, `cv->moveEffect`를 읽는 캔슬러 0개, opcode 차이 4줄 그대로, 지운 명령 호출처 0, #9730·HnS 수정·#8647 AI와 겹침 없음, 미래예지 저장 값 같음, HNS9918 trace 이식 전후 같음, P1~P4 재측정 같음, 그 밖 고위험 시나리오(파트너 기절·마비·혼란·교체·풀죽음·잠듦, 상대 둘 다 같은 맹세, 양쪽 같은 턴 콤보, 춤추기, 드래곤테일, 프리폴, 사슬묶기, 순서미루기) 이식 전후 같음. **발견: 위 맹세 assert 9종(중, upstream 같음) → 가드 `b857f809e0`**, K07 보강 `a1f5e58f85`.
  - R2(#9805·#9968·#9896·#9965): 네 커밋 모두 바뀐 줄이 upstream과 바이트 같음, HnS 러너 수정·config 값 4줄·`isEgg`·챌린지 줄 보존, `BATTLE_TEST_GHOST` enum 이동 뒤 `switch` 4곳 모두 분기 있음, OWE 경로는 실게임에 안 닿음, 이벤트 스크립트·맵 데이터 바이트 같음, `givemon`/`createmon` 5건 같은 시드 실측(성격·볼·개체값·기술·도구·직후 난수 같음), 랜더마이저와 난수 공유·이름 충돌 없음. 정보 3건(아래).
- **메인 검증:** 적용 HEAD `66bbc9b5cf`에서 재빌드 SHA1 같음·전체 테스트 목록 같음·한글 607개 같음·세이브 정적 PASS·왕복 PASS.
- **최종(`a1f5e58f85`, 가드·K07 뒤):** ROM **32,752,756 B**(묶음 전 +944), EWRAM 250,408 B, IWRAM 25,516 B, SHA1 `e617f3b1c4d180e44dcedf0be74ea49e5b77d2d3`, 새 경고 0. 전체 테스트 PASS 2,521 → **2,547** / TOTAL 5,426 → 5,457, 사라진 PASS 0, 새 줄 = #9896 16 + 맹세 가드 10 + ghost FAIL 3·INVALID 2 → 목록 [`test-baseline-seq192.txt`](test-baseline-seq192.txt)(**다음 비교 기준**). 한글 607개 기대와 바이트 동일(517/607). 세이브 정적 비교 PASS(WARN 2)·왕복 PASS.

## 재확인·뒤 PR 메모

- #10220(seq 476) 전에는 HnS `DoAttackCanceler`에 `cv.moveEffect = GetMoveEffect(cv.move);`가 없어 #9918 캔슬러는 `GetMoveEffect(cv->move)`로 우회 — 그때 맞출 것(재확인 27).
- seq 385 #10214: 불바다 종료 진영(재확인 6)과 함께 HNS9918 K12·K22·K23 기대가 바뀐다.
- #9896 후속: seq 204 #10051의 `test/random_mon_generation.c` hunk 같이, seq 403 #9970의 `RNG_RANDOM_BALL` 이동 hunk는 이미 적용 상태.
- `MOVE_EFFECT_*`는 HnS 고유 `MOVE_EFFECT_SCALE_SHOT` 때문에 upstream보다 번호가 1 크다(저장 안 됨, 이식 전과 같은 상태, #9408 seq 465 때 정리).

## 실기(mGBA) 확인 항목

- 더블배틀 맹세 콤보 3종: 대기 문구, `2개의 기술이 하나가 되었다!`, 무지개·불바다·습지 시작·종료 문장의 "우리 편/상대"
- 미래예지(설정 시점이 캔슬러로 옮겨짐)
- #9896 정보(현재 HnS 사용처 없음): `BALL_RANDOM`이 이상한볼·프레셔스볼·GS볼까지 뽑음(쓸 때 GS볼 제외 여부), `MOVE_RANDOM_TEACHABLE`이 학습 랜더마이저 대리 종을 따르지 않음, 무작위 포켓몬 선택지 표가 비어 전국 도감 전체에서 뽑음 — `getrandom*`를 HnS가 쓰게 되면 정한다.
- 맹세 가드 손 병합: seq 311 #10542(같은 블록 `BATTLE_PARTNER` → `GetPartnerBattler`), seq 378 #10162(`GetPledge*Move` 반환형) — 재확인 32.
