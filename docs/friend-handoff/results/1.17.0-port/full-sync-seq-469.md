# full-sync 묶음 40 — seq 469 (#9916)

- 요청: 사용자 "ㄱㄱ 469 시작해 나 혼자 할 수 있는 것이라면 말이야"(2026-10-24).
- upstream: 읽기 전용 사본 `/home/user/pokeemerald-expansion-upstream`, `git diff 49bdf428aa^1 49bdf428aa`.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 469 | #9916 | `f1d4a9a6` | 배틀 구조체·스크립트 정리: 통찰(Frisk) 흐름, 점착의 도둑질 차단 스크립트, 선제 발동 루프 변수, 스텔라 부스트 트레이너별, 안 쓰는 명령 2개 삭제 | 32,963,748 |

이식 전(`e8997de1`) 32,964,052 B → **32,963,748 B(−304)**.

## PR 내용과 HnS 판단

- 11파일 중 5개(`asm/macros/battle_script.inc`, `data/battle_scripts_1.s`, `include/battle_scripts.h`, `src/battle_terastal.c`, `src/battle_util.c`)는 그대로 적용, 6개는 HnS 줄에 맞춰 손으로.
- **통찰(Frisk):** 등장 때 상대 중 도구를 가진 포켓몬이 있는지 먼저 보고, 있으면 팝업 한 번 → 대상마다 문장(`gBattleScripting.battler`로 대상 순회, 문장 뒤 `addbyte sBATTLER, 1`). 쓰는 쪽은 `gEffectBattler`.
  - **한글 문장 `STRINGID_FRISKACTIVATES`는 이름 토큰만** `{B_ATK_NAME_WITH_PREFIX}` → `{B_EFF_NAME_WITH_PREFIX}`, `{B_DEF_NAME_WITH_PREFIX2}` → `{B_SCR_NAME_WITH_PREFIX2}`(정책 74, 문구·줄바꿈·조사 토큰 그대로).
- **점착:** 도둑질·탐내다를 막을 때 `BattleScript_NoItemSteal`(대상 팝업 + 문장) 대신 `BattleScript_StickyHoldActivatesRet`(점착 가진 쪽 팝업 + 같은 문장 `STRINGID_PKMNSXMADEYINEFFECTIVE`). HnS는 대상별 루프라 `gBattlerAbility = battlerDef`(upstream과 같음). 팝업 앞에 짧은 대기가 하나 생긴다(`B_WAIT_TIME_SHORT`).
- **선제공격손톱·퀵드로·이바열매:** 턴 순서 루프 변수가 `gBattleStruct->quickClawBattlerId` → `gBattleScripting.battler`. 루프 안 스크립트(`BattleScript_QuickClawActivation` — HnS는 도구 팝업 포함, `QuickDraw`, `Custap`)가 `sBATTLER`를 쓰지 않음을 확인. 안 쓰는 `savedTurnActionNumber` 대입 삭제(필드는 upstream처럼 남김).
- **스텔라 부스트:** 진영별 → 트레이너별(`MAX_BATTLE_TRAINERS`). 멀티배틀에서 파트너와 따로 센다. HnS 본 게임은 플레이어 테라 불가·AI 테라 OFF라 영향 없음.
- **삭제:** `critcalc`·`pursuitdoubles` 명령(HnS 스크립트가 안 씀 — `git grep` 0), 명령 번호는 `B_SCR_OP_UNUSED_35`로(HnS가 미리 둔 "#9916 (UNUSED_35) not ported yet" 주석 자리 정리). `field_93` → `recordedActionSet`, `friskedAbility`·`friskedBattler`·`quickClawBattlerId` 삭제. 배틀 구조체는 세이브되지 않는다.

## 검증

- **빌드:** `make hns -j8` 종료 0, 새 경고 0(`warncheck.sh`).
- **새 회귀 테스트 `test/hns_9916_frisk_sticky_hold.c`(한글 문장):** 통찰 내 쪽·상대 쪽·더블(팝업 1번, 대상마다 이름)·도구 없음, 점착 vs 도둑질(팝업·문장·도구 유지) — **5/5 PASS**. 같은 테스트를 이식 전 코드(클론 `e8997de1`)에서도 **5/5 PASS** → 화면 문장·이름은 이전과 같다.
- **관련 upstream 테스트:** Frisk·Sticky Hold·Quick Claw·Quick Draw·Custap·Stellar·Thief 접두 묶음 — `test-baseline-seq468c.txt`와 결과 같음(통찰·점착 문장 테스트는 영어 `MESSAGE`라 원래 FAIL).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 `f1d4a9a6`): 러너 PASSED 2,824 / TOTAL 5,803 — `test-baseline-seq468c.txt` 대비 **다른 줄은 새 테스트 5개(PASS)뿐, 사라진 PASS 0**. 새 기준 [`test-baseline-seq469.txt`](test-baseline-seq469.txt)(**다음 비교 기준**).
- **세이브 정적 비교**(이식 전 사실 = 코드 `a308a8d1`, 클론 빌드): **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-b46 4`, 코드 `f1d4a9a6`): **558/652, `판정: PASS (기대와 같음)`**.
- **ROM**(코드 `f1d4a9a6`): SHA1 `e9cafe93cd5ecfe736d909153d6650960c097efe`, 32,963,748 B, 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 하지 않음(친구).**

## 실기(mGBA) 후보 — 친구

1. 통찰 특성 포켓몬 등장(싱글·더블): 팝업 한 번 뒤 "…은(는) 상대 …의 [도구]을(를) 통찰했다!"의 두 이름이 맞는지.
2. (가능하면) 도둑질·탐내다를 점착 상대에게: 점착 팝업 → "상대 …에게는 효과가 없는 것 같다...".

## 다음

- seq 470 #9939(XL, 명중 판정을 취소 단계로 — 471 #9983·471.5 #10144·474 #10606과 같은 묶음 단위).
