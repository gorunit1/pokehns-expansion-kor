# full-sync 묶음 55 — seq 488 (#10595 대타출동 판정·변화기 명중 판정을 기술 처리 단계로)

- 요청: 사용자 "ㄱㄱ 488 시작해 혼자해도 되는거라면"(2026-10-27).
- upstream: 읽기 전용 사본, #10595 `88e70d8185`(7파일, +197/−268). 단위 `U-accuracy-9939`(앞 seq 470 #9939 등 적용됨).
- 혼자 진행한 근거: 새 문장 0, 한글 문장 변화 0. 바뀌는 출력은 upstream이 본가 순서(Showdown과 같음)에 맞춘 것 — 변화기가 빗나가는 판정이 "이미 잠듦·이미 혼란" 같은 실패 판정보다 먼저, 부식가스가 대타출동에 막히면 `그러나 실패하고 말았다!`. upstream 결함 1개(압정류가 대타출동 뒤 상대에게 실패)는 이식 전 동작으로 되돌리는 HnS 보정(재확인 105) — 친구 결정이 필요한 선택지가 아님.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 488 | #10595 | `daca9a9c` | 새 `CANCELER_SUBSTITUTE`(능력 변화가 아닌 변화기가 대타출동에 막히면 기술 처리 단계에서 실패), 스크립트의 `accuracycheck`(35곳)·`jumpifsubstituteblocks`(13곳)·두 명령 삭제, 변화기(능력 변화 제외)는 기술 처리 단계의 명중 판정, 다단 기술 코드 위치 이동, 변신의 대타출동 판정 이동, upstream 테스트 2개, 한글 회귀 테스트 | 32,961,188 |
| 488 | (HnS) | `c03e1b92` | HnS 보정: 필드·상대 필드 기술(압정뿌리기·독압정·스텔스록·끈적끈적네트)은 대타출동 판정에서 뺌(재확인 105), 한글 테스트 1개 추가 | 32,961,204 |

이식 전(`05925d4a`, 게임 코드 = `2ae2ad83`) 32,961,492 B → **32,961,204 B(−288)** — 스크립트 명령 48곳·명령 함수 2개 삭제.

## 내용과 HnS 판단

- **적용 방법:** 파일별 3-way 병합(`git merge-file`). 충돌 4곳:
  - `include/constants/battle_script_commands.h`·`battle_script_commands.c` 빈 명령 자리: upstream은 2개 지우고 `UNUSED_41`·`UNUSED_42`를 더함(upstream엔 `UNUSED_40`이 이미 있음). HnS는 `UNUSED_39`까지라 `UNUSED_40`·`UNUSED_41`을 더함 — 명령 수와 `CALLNATIVE` 번호가 이식 전과 같음(seq 484와 같은 방식).
  - `Cmd_accuracycheck` 삭제 부분: HnS 함수가 upstream 이전 형태와 같아 그대로 지움.
  - `battle_move_resolution.c` 다단 기술 코드(`IsMoveParentalBondAffected`~`CancelerMultihitMoves`) 위치 이동: HnS 고유 3줄을 새 위치에 옮김 — `GetPartnerBattler`, `GetMoveEffect(cv->move) == EFFECT_SPECIES_POWER_OVERRIDE`, 몰매(Beat Up)의 플레이어 파티 수 제한 `GetMaxPartySize()`. upstream이 이동하며 지운 "변화기는 여기서 끝" 분기는 upstream처럼 지움(대신 명중 판정·추가 효과 단계에서 처리).
  - 병합 뒤 "upstream 변경 줄 ↔ HnS 변경 줄" 대조: 남는 차이는 위 HnS 3줄과 빈 명령 번호, 아래 보정뿐. HnS 고유 스크립트에 `accuracycheck`·`jumpifsubstituteblocks` 없음(병합 뒤 0).
- **재확인 8v 해결:** seq 240 #10226이 넣은 `jumpifsubstituteblocks`가 이번에 지워짐(1.17.0 최종형과 같음).
- **HnS 보정(upstream 결함, 재확인 105):** 새 `CancelerSubstitute`는 상대 필드 기술(`TARGET_OPPONENTS_FIELD`)의 대상을 상대 포켓몬으로 셈 → 상대가 대타출동 뒤에 있으면 압정뿌리기·독압정·스텔스록·끈적끈적네트가 `그러나 실패하고 말았다!`로 실패(이식 전엔 정상으로 깔림, 본가도 깔림). 한글 회귀 세트 `HNS9714 2-04`(대타출동 4마리 + 독압정·스텔스록 뒤 정리정돈)가 기대와 달라져 발견. 필드·상대 필드 기술은 이 판정에서 빼서 이식 전처럼. 1.17.1·master 같은 코드(고친 커밋 없음).
  - 보정을 빼면 새 한글 테스트 `entry hazards are set against a foe behind a Substitute` FAIL, 보정으로 PASS, 이식 전 코드 PASS.
- **upstream 루프 의심(보정 안 함):** `CancelerSubstitute` 루프는 대상마다 건너뛸지를 `battler`가 아니라 `cv->battlerDef`로 봄(1.17.1·master 같음). 더블 흔들흔들댄스(한쪽 방어·한쪽 대타출동, 두 위치 모두)와 일구다·플라워가드·아로마테라피·유혹·회복봉인·베놈트랩·다크홀·지압(아군 대타출동)을 이식 전 코드와 상태로 비교 → 모두 같음. 출력 차이가 없어 upstream 형태 유지(흔들흔들댄스는 한글 테스트로 고정).
- **바뀌는 출력(upstream 의도, `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 2행):**
  1. 변화기(능력 변화 제외)의 명중 판정이 스크립트 안 실패 판정보다 먼저: 이미 잠든 상대에게 최면술이 빗나가면 이제 `상대 마자에게는 맞지 않았다!`(이전 `상대 마자는 이미 잠들어 있다`), 이미 혼란인 상대에게 초음파가 빗나가면 같은 빗나감 문장(이전 `이미 혼란에 빠져 있다`). 마이페이스·미스트필드·아로마베일·흡반·뿌리박기 등 스크립트 판정도 같음. 명중률 100 미만 기술이 빗나갈 때만.
  2. 부식가스가 대타출동 뒤 상대에게: `그러나 실패하고 말았다!`(이전 `상대 …에게는 효과가 없는 것 같다...`). HnS 트레이너: 도희 재대전(`TRAINER_JANINE_POSTOBC_HNS`) 1마리.
  - 씨뿌리기·록온·악몽·트릭·옥토록 등 대타출동 실패는 이전과 같은 `그러나 실패하고 말았다!`.
- 세이브: 배틀 스크립트·배틀 코드만, 세이브 구조 변화 없음.

## 검증

- **빌드:** `make hns -j8` 종료 0, 전체 재빌드(`build/hns` 지우고) 로그로 새 컴파일 경고 0(`warncheck.sh`).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `c03e1b92`): `test-baseline-seq486.txt` 대비 **새 줄 8개(한글 6 PASS, upstream `Leech Seed's accuracy is 90%` PASS·`Leech Seed is blocked by Substitute` FAIL — 영문 `But it failed!` 검사, 같은 장면 한글 테스트 PASS), 사라진 PASS 0**, INVALID 24(이전 실행과 같은 목록). 새 기준 [`test-baseline-seq488.txt`](test-baseline-seq488.txt)(**다음 비교 기준**).
  - 보정 전 첫 실행(`daca9a9c`)도 사라진 PASS 0 — 압정류 결함은 upstream 테스트에 없음.
- **새 한글 회귀 `test/hns_10595_substitute.c` 6개:** 씨뿌리기 vs 대타출동 실패, 부식가스 vs 대타출동 `그러나 실패하고 말았다!`(도구 그대로), 잠든 상대에게 빗나간 최면술, 혼란인 상대에게 빗나간 초음파, 더블 흔들흔들댄스(한쪽 방어·한쪽 대타출동 — 대타출동 쪽 혼란 없음), 압정류 4종이 대타출동 뒤 상대 쪽에 깔림: **6/6 PASS**.
  - 이식 전 코드(클론 `05925d4a`, `ZZ old` 사본, 커밋 안 함·지움): **3/6** — 부식가스·최면술·초음파 FAIL = 위 의도한 변화, 나머지(씨뿌리기·흔들흔들댄스·압정류) PASS = 출력 같음.
  - upstream 새 테스트 2개는 이식 전 코드에서 명중률 PASS·대타출동 FAIL(영문 문장) — 같은 상태.
- **세이브 정적 비교**(이전 = 클론 `05925d4a` 빌드, ROM SHA1 `58bd67e9…` = 묶음 54 기록과 같음): **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**: 보정 전(`kor-s488`) 554/652 — `HNS9714 2-04` PASS → FAIL(위 결함). 보정 뒤(`kor-s488b`) **555/652, `판정: PASS (기대와 같음)`, 요약 바이트 같음**.
- **ROM**(커밋 `c03e1b92`): SHA1 `f80fa97f942f2b7825cb2327844bb79eb1a7c089`, 사용 32,961,204 B(파일 33,554,432 B), 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 하지 않음(친구).** 후보 — ① 상대가 대타출동을 쓴 뒤 스텔스록·압정뿌리기 등을 깔기(깔려야 함), ② 이미 잠든·혼란인 상대에게 최면술·초음파·수면가루 등(빗나가면 빗나감 문장), ③ 도희 재대전의 부식가스를 내 대타출동에 쓰는 장면, ④ 대타출동 상대에게 씨뿌리기·트릭 등(실패 문장).

## 다음

- seq 489 #10600(Scrappy 특성이 바뀐 경우 고스트 타입 판정). 목표 1.17.1.
