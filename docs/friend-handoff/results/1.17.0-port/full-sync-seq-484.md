# full-sync 묶음 50 — seq 484 (#10426 흡수 리팩터·`SetMoveEffect` 인자 구조체)

- 요청: 사용자 "ㄱㄱ 484 시작해 혼자해도 되는거라면" → 475.5~475.8 누락 발견(묶음 49) 뒤 "ㅇㅇ 혼자할 수 있다면 ㄱㄱ"(2026-10-26, #10151 없이 484 먼저).
- upstream: 읽기 전용 사본, #10426 `763b39a07c`(25파일, kor=Y — 문장 3개의 이름 토큰). 같은 단위(`U-absorb-10426`)에 다른 seq 없음.
- 혼자 진행한 근거: 새 문장 0, 한글 문장은 정책 74대로 이름 토큰만 바꿈(출력 같음 — 테스트로 확인). 화면 순서 변화는 휘적휘적포 한 기술(HnS 트레이너·야생 데이터에 쓰는 포켓몬 없음). #10151이 넣는 `brokeProtect` 줄만 빠지고 나머지는 upstream 그대로.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 484 | #10426 | `15b75297` | 흡수 기술 13개가 `EFFECT_ABSORB` → 추가 효과 `MOVE_EFFECT_ABSORB`(인자 `absorbPercentage`, 꿈먹기는 `EFFECT_DREAM_EATER` + 같은 추가 효과), `SetMoveEffect(cv, &se)` 구조체 인자 + 옛 인자용 `SetMoveEffectHelper`, `seteffectsecondary` 명령 삭제, 씨뿌리기 턴 끝(공격자 = 받는 쪽, 스크립트 배틀러 = 빨리는 쪽), 문장 3개 이름 토큰, AI 판정을 추가 효과 쪽으로, HnS 보정 1(upstream master와 같은 줄), 한글 회귀 테스트 | 32,959,044 |

이식 전(`989080a0`, 코드 = `78571464`) 32,958,644 B → **32,959,044 B(+400)** — 흡수 기술마다 추가 효과 목록, `SetMoveEffect` 코드.

## 내용과 HnS 판단

- **적용 방법:** 파일별 3-way 병합(`git merge-file`), 충돌은 손으로. 병합 뒤 "upstream 변경 줄 ↔ HnS 변경 줄" 대조로 남는 차이가 모두 아래 HnS 형태인지 확인. 지워진 식별자(`EFFECT_ABSORB`, `seteffectsecondary`, `SETEFFECTSECONDARY`)를 HnS 고유 코드·테스트·한글 회귀 세트에서 검색 — 0.
- **HnS 형태 유지:**
  - `include/move.h` 인자 union에 `brokeProtect` 없음, 보호 뚫기 5기술(페인트·섀도다이브·고스트다이브·이차원홀·이차원러시)의 `.argument.brokeProtect` 줄 없음, 페인트 계열 문장 선택은 HnS 그대로 기술 효과로(B_MSG_HYPERSPACE_FURY/B_MSG_FEINT) — `brokeProtect`는 #10151(seq 475.5, 재확인 99 보류)이 넣는 필드.
  - 빈 명령 자리: upstream은 `UNUSED_40`을 더하지만 HnS는 `UNUSED_38`까지라 `B_SCR_OP_UNUSED_39` — `CALLNATIVE` 번호는 이식 전과 같음(249).
  - 흡혈(Leech Life) 위력 60 유지(HnS 데이터, 주석), 상대를 쓰러뜨리면 반동 턴 없음(1세대 규칙) 챌린지 옵션 `genOneRecharge` 조건 유지(upstream의 `gBattlerTarget` → `cv->battlerDef`만 받음).
  - 한글 문장 3개는 이름 토큰만 upstream처럼(`흡수했다`: `B_DEF` → `B_SCR`, 씨뿌리기: `B_ATK` → `B_SCR`, 해감액: `B_ATK` → `B_EFF`) — 조사 토큰·줄바꿈 그대로.
- **HnS 보정(upstream 결함, 재확인 100):** #10426의 해감액 씨뿌리기 스크립트가 `jumpifability BS_EFFECT_BATTLER, ABILITY_MAGIC_GUARD`로 씨 뿌린 쪽의 매직가드를 보는데 턴 끝 코드가 `gEffectBattler`를 채우지 않음 → 지난 값(예: 씨앗을 맞은 상대)을 봐서 매직가드 포켓몬도 해감액 대미지를 받음. 첫 전체 테스트에서 upstream 테스트 `Liquid Ooze HP loss from Leech Seed is blocked by Magic Guard`가 사라진 PASS로 나와 발견. upstream은 뒤 합치기 커밋 `5340d262a4`(master)에서 `copybyte gEffectBattler, gBattlerAttacker` 한 줄로 고침 → 같은 줄을 받음. 빼면 그 upstream 테스트와 새 한글 테스트 둘 다 FAIL 확인.
- **화면 순서 변화(새 문장 0, `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 1행):** 휘적휘적포(흡수 + 화상 20%)가 화상을 걸 때 이전엔 `…은(는) 화상을 입었다!` → `…로부터 체력을 흡수했다!`, 이제 흡수 문장 → 화상 애니·문장(추가 효과 목록 순서, upstream 새 테스트 `Matcha Gatcha restores HP before applying burn to target`). 다른 흡수 기술은 추가 효과가 흡수 하나뿐이라 순서 같음.
- **같은 출력으로 확인한 것:** 흡수·기가드레인·꿈먹기·흡혈·큰뿌리·파라볼라차지(더블 대상별 이름)·힘흡수(클리어바디로 공격이 안 떨어져도 대상 이름 — `B_SCR`가 흡수 단계에서 대상으로 채워지는지)·씨뿌리기 양쪽·해감액(흡수·씨뿌리기)·HP 가득일 때 문장 없음.
- 세이브: 기술 데이터·배틀 코드만, 세이브 구조 변화 없음.

## 검증

- **빌드:** `make hns -j8` 종료 0, 전체 재빌드(`build/hns` 지우고) 로그로 새 컴파일 경고 0(`warncheck.sh`).
- **전체 테스트:**
  - 첫 실행(보정·한글 테스트 전): `test-baseline-seq475.7.txt` 대비 새 PASS 1(upstream 휘적휘적포 테스트), **사라진 PASS 1**(위 매직가드 테스트) → 보정.
  - 보정 + 한글 테스트 13개: 새 PASS 14(한글 13 + upstream 1), 사라진 PASS 0. INVALID 24(이전 실행들과 같은 목록).
  - 휘적휘적포 한글 테스트 추가 뒤 마지막 실행(커밋 `15b75297` 코드): 새 PASS **15**(한글 14 + upstream 1), **사라진 PASS 0**, INVALID 24 같음. 새 기준 [`test-baseline-seq484.txt`](test-baseline-seq484.txt)(**다음 비교 기준**).
- **새 한글 회귀 `test/hns_10426_absorb.c` 14개:** 흡수 대상 이름, 기가드레인 `마자용으로부터`(받침 조사), 해감액 흡수, HP 가득일 때 문장 없음, 꿈먹기, 흡혈 위력 60·절반 흡수, 큰뿌리 ×1.3, 더블 파라볼라차지 대상별 이름, 씨뿌리기 양쪽 이름, 해감액 씨뿌리기 `상대 마자는 해감액을 흡수했다!`, 힘흡수, 힘흡수 vs 클리어바디, 매직가드가 해감액 씨뿌리기를 막음, 휘적휘적포 흡수 → 화상 순서: **14/14 PASS**.
  - 이식 전 코드(클론 `989080a0`, 효과 상수만 옛 이름으로 바꾼 `ZZ old` 사본, 커밋 안 함·지움): **13/14** — 휘적휘적포 순서만 FAIL = 위 의도한 변화. 매직가드 테스트도 이식 전 PASS(= 보정으로 이식 전 동작 유지).
- **세이브 정적 비교**(이전 = 클론 `989080a0` 빌드, ROM SHA1 `286847e7…` = 묶음 49 기록과 같음): **PASS(FAIL 0, WARN 2)** — WARN은 `move.h`가 4줄 늘어 `SanitizeMoveId` assert 줄 번호 상수가 240 → 244(`BoxMonRestorePP`, 같은 원인으로 검토 묶음 1건). 세이브 블록 크기·섹터 배치·EWRAM/IWRAM 같음.
- **한글 회귀**(`kortests/run.sh <저장소> kor-s484 8`, 코드 = 커밋 `15b75297`의 게임 코드 — 휘적휘적포 한글 테스트 추가 전 작업 트리): **556/652, `판정: PASS (기대와 같음)`, 요약 바이트 같음**.
- **ROM**(커밋 `15b75297`): SHA1 `9b5935b021414181acd83d6303b8215062402a1b`, 사용 32,959,044 B(파일 33,554,432 B), 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 하지 않음(친구).** 후보 — ① 흡수·기가드레인·메가드레인 등 흡수 기술의 HP 바·`…로부터 체력을 흡수했다!`, ② 씨뿌리기 턴 끝(양쪽) 애니·HP 바·문장, ③ 해감액(왕눈해·독파리 등) 상대에게 흡수 기술·씨뿌리기, ④ 힘흡수.

## 다음

- seq 485 #10488(Assurance inconsistencies), 486 #10566(재확인 97 정리). 475.5·475.8은 친구 답(재확인 99) 뒤 — 그때 `brokeProtect` 줄(이번에 뺀 5기술 + union)도 함께.
