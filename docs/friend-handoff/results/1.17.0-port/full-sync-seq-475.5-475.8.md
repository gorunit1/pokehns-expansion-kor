# full-sync 묶음 51 — seq 475.5 (#10151 Champions 배틀 메커니즘) + 475.8 (#10282 GEN_CHAMPIONS 테스트·소스 수정)

- 요청: 친구 답 2026-10-26([`FRIEND_REPLY_2026-10-26.md`](../../FRIEND_REPLY_2026-10-26.md) 1절, 재확인 99 **A안**) — 사용자 전달.
- 1차(2026-10-26): 친구 지시로 코드 수정과 정적 확인까지. 2차(같은 날, 친구 메시지 "빌드하고 전체 자동 테스트·한글 회귀 검사·세이브 정적 비교까지"): 아래 "검증" 절. mGBA는 친구.
- upstream: 읽기 전용 사본, #10151 `b3a3c16abf`(45파일), #10282 `50c01a1a96`(39파일). 같은 단위의 다른 행 없음.

## 커밋

| seq | PR | 커밋 | 내용 |
|---|---|---|---|
| 475.5 + 475.8 | #10151 + #10282 | `131f14ba` | Champions 설정 14개(`GEN_LATEST`), 선택 제한·실패·피해·얼음·마비·앙코르 우선도·기절 뒤 효과·보이지않는주먹/관통드릴, 새 문장 2개(Champions 공식 한글), `brokeProtect`, #10282 소스 수정 + 테스트, HnS 보정 1, 한글 회귀 테스트 |
| (검증) | — | `ba5922ac` | 9세대 동작을 보는 테스트 6개에 9세대 설정 고정, 한글 속이기 테스트 구분력 보강, 한글 회귀 세트 HNS9680 1-11 마비 난수 고정·기대 요약 갱신(555/652). ROM 같음 |

수정 기준(이식 전): `ad68cdcc`(묶음 50 문서 커밋, 코드 = `15b75297`).

## 설정 값과 적용 분기

HnS `include/config/general.h`는 `GEN_LATEST = GEN_CHAMPIONS`이고(upstream 1.17.0은 `GEN_9`), 새 설정 14개는 모두 `GEN_LATEST`로 넣었다 — **HnS의 기존 Champions 설정에 맞춰 신규 메커니즘을 반영**한 것(upstream 기본값 자체가 Champions인 것은 아니다). `GEN_COUNT (GEN_CHAMPIONS + 1)`은 HnS에 이미 있어 그대로(upstream도 이 PR에서 같은 값으로 바꿈).

| 설정 | 값 | HnS에서 타는 분기 | 구현 상태 |
|---|---|---|---|
| `B_PARALYSIS_CHANCE` | `GEN_LATEST` | 마비 행동 불가 1/8(`RandomWeighted(RNG_PARALYSIS, 7, 1)`, #10282 형태) | 구현 |
| `B_MEGA_EVO_SPEED_SWAP` | `GEN_LATEST` | 스피드스왑(`VOLATILE_SPEED_SWAP`) 뒤 메가진화해도 스피드 유지(`CopyMonLevelAndBaseStatsToBattleMon(..., FALSE)`) | 구현 |
| `B_FREEZE_TURNS` | `GEN_LATEST` | 해동 25%, 얼어서 못 움직인 행동 2번 뒤 3번째 행동에서 해동 | 구현 + **HnS 보정**(아래) |
| `B_FIRST_TURN_MOVE` | `GEN_LATEST` | 속이기·만나자마자 첫 턴 이후 선택 불가(`이 기술은 쓸 수 없다!`) | 구현 |
| `B_SALT_CURE_DAMAGE` | `GEN_LATEST` | 턴 끝 1/16(물·강철 1/8) | 구현 |
| `B_BELCH_SELECTABLE` | `GEN_LATEST` | 트림 선택 가능, 열매를 안 먹었으면 실행에서 실패(새 실패 문장) | 구현 |
| `B_STUFF_CHEEKS_SELECTABLE` | `GEN_LATEST` | 볼가득넣기 선택 가능, 열매가 없으면 실행에서 기존 실패(`BattleScript_ButItFailed`) | 구현 |
| `B_SPIT_UP_SELECTABLE` | `GEN_LATEST` | 비축 0이면 토해내기 선택 불가 | 구현 |
| `B_MOVES_THAT_REMOVE_TYPE` | `GEN_LATEST` | 불사르기·전광쌍격, 해당 타입이 아니면 선택 불가(Z기술 선택 시 예외, #10282) | 구현 |
| `B_LAST_RESORT_SELECTABLE` | `GEN_LATEST` | 비장의무기 조건 미충족이면 선택 불가(Z기술 예외, #10282) — 유일한 기술이면 발버둥 | 구현 |
| `B_FAINT_MOVE_EFFECT_TIMING` | `GEN_LATEST` | 사용자가 울퉁불퉁멧 등으로 쓰러져도 탁쳐서떨구기·도둑질/탐내다·고속스핀(킬러스핀 포함)·아이스스피너 효과 적용. upstream 범위 그대로 — 암석액스·비검천중파는 제외(설정 주석) | 구현 |
| `B_SHEER_FORCE_AGAINST_ABILITIES` | `GEN_LATEST` | **설정만**(upstream 주석 "Currently does nothing") — 우격다짐 vs 발끈·나쁜손버릇 Champions 테스트 2개가 upstream에서 `KNOWN_FAILING` | **미구현** |
| `B_ENCORE_PRIORITY` | `GEN_LATEST` | 앙코르로 강제된 기술의 우선도로 순서 계산 | 구현 |
| `B_UNSEEN_FIST_PIERCING_DRILL` | `GEN_LATEST` | 방어 관통 시 피해 25%(방어 무시 기술은 1.0 먼저 반환 — 예외 유지), 접촉 방어 효과(니들가드·킹실드 등) 적용 | 구현 |

- 선택 허용과 실행 성공은 별개 — 트림·볼가득넣기는 선택만 열리고 실행 조건은 그대로.
- **남은 미구현:** `B_SHEER_FORCE_AGAINST_ABILITIES`. 1.17.1까지(그리고 upstream master까지) 이 설정을 쓰는 후속 PR이 없음(`git log -S` 확인). 목표 밖 upcoming 변경으로 채우지 않음. 다른 13개 설정도 1.17.1 범위에서 다시 바꾸는 PR 없음.
- upstream 남은 사소한 점(바꾸지 않음): `MOVE_LIMITATIONS_ALL`이 `0xFFFF`인데 `MOVE_LIMITATION_PLACEHOLDER`가 `1 << 16`으로 밀려 "전부" 검사에서 빠짐 — HnS 기술 데이터에 `EFFECT_PLACEHOLDER` 0개, 선택 단계엔 따로 검사가 있어 영향 없음(1.17.1·master 같음).

## 이미 있던 것 / 이번에 넣은 것 / 뒤 형태로 맞춘 것

- **이미 있던 것(9/15 선별 이식):** Champions 수면 턴(`RandomWeighted(RNG_SLEEP_TURNS, 0, 0, 1, 2)` — 하품 경로도 HnS 형태 유지, 확률은 upstream `Random() % 3`과 같은 1/3·2/3), Champions 기술 데이터, 메가진화 특성. 손대지 않음. 설정 읽기만 #10151·#10282대로 `B_SLEEP_TURNS` → `GetConfig(B_SLEEP_TURNS)`.
- **이미 있던 뒤쪽 형태:** 선택 제한 함수의 `dynamaxBypassCheck`·`zMoveBypassCheck` 지역 변수(#10282 형태), MoveEnd 대상별 루프, `cv->abilities`, `breaksThroughProtectFully`의 공격자 기준(#10389 HnS 형태). 그래서 `TrySetCantSelectMoveBattleScript`·`IsBelchPreventingMove`·`CheckMoveLimitations`는 **1.17.1 함수 그대로**(HnS 현재 함수와 1.17.1 함수의 차이가 #10151·#10282 부분뿐임을 함수 단위 diff로 확인), MoveEnd 쪽은 HnS 원본 위에 #10151 덩어리 10개를 1.17.1 형태로 하나씩.
- **#10426 뒤 형태:** `brokeProtect`는 #10426 union(`argument`)에 `enum BrokeProtectionStringID brokeProtect:8`, 5기술(페인트·섀도다이브·고스트다이브·이차원홀·이차원러시)에 `.argument.brokeProtect`, 페인트 효과는 `se->additionalEffect->argument.brokeProtect`(묶음 50에서 뺐던 줄 복원). `B_MSG_HYPERSPACE_FURY` → `B_MSG_BROKE_THROUGH_PROTECT`.
- **#10354(seq 262)가 지웠던 `gEffectBattler = cv->battlerDef;`**(도둑질 애니)는 #10151이 다시 넣는 줄이라 1.17.1 형태로 복원.
- 유지: 프레젠트 보정·동적 기술 분류·공격 처리(#10362)·반감열매 출력 순서 — 이번 변경 줄에 없음. `ai_thinking_time.c` 프레임 상한은 HnS 측정값 유지(upstream은 `GetConfigInternal`을 `ARM_FUNC`로 바꾸며 낮춤 — 측정 미실행).
- **#10282 소스 수정 반영 범위:** 마비 가중 난수, `CheckMoveLimitations` 인자 `u16` → `u32`(선택 제한 비트 17개), AI 선택 제한 계산에서 `MOVE_LIMITATION_UNUSABLE` 제외, 잠꼬대 후보에서 `UNUSABLE` 제외, 수면 설정 `GetConfig`, Z기술·다이맥스 예외(HnS에 이미 있음), 프리즈드라이 설명 구조(`#if`를 `COMPOUND_STRING` 밖으로). 테스트 33파일은 3-way 깨끗.

## 문장(Champions 공식 한글)

| ID | HnS 문장 | 근거 |
|---|---|---|
| `STRINGID_CANTUSEMOVE`(새, 선택 단계) | `이 기술은 쓸 수 없다!\p` | Poké Corpus `57e5078` Champions `ko_ms.txt` 2096행 `BTL_STRID_STD_Waza_NG`. `\p`는 선택 창 문장 공통 제어 코드 |
| `STRINGID_BELCHCANTUSE`(새, 실행 단계) | `{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n나무열매를 먹지 않아서 기술을 쓸 수 없다!` | 1313행 `GeppuFail`, 1314행 `GeppuFail_E`(상대 = `상대 ` 접두 — HnS `B_ATK_NAME_WITH_PREFIX`가 붙임) |
| `STRINGID_BELCHCANTSELECT`(기존, 선택 단계) | 그대로(`…{B_TXT_EUNNEUN}\n나무열매를 먹지 않아서 기술을 쓸 수 없다!\p`) | upstream은 영문만 이름 토큰을 넣음 — HnS 한글은 이미 이름 있음 |

- **HnS 차이(재확인 102):** upstream `BattleScript_BelchFails`(실행 실패)는 선택용 `BELCHCANTSELECT`(`\p` 포함)를 찍고 새 `BELCHCANTUSE`는 정의만 함(1.17.1·master 같음). HnS는 실행 실패에서 `BELCHCANTUSE`(`\p` 없음, 배틀 문장 형식)를 찍게 함 — 선택 단계 = `BELCHCANTSELECT`, 실행 단계 = `BELCHCANTUSE`. Champions 설정에선 트림 선택이 열려 선택 문장은 9세대 이하 설정에서만 나옴.
- 폭(정적 추정, 실측 미실행): 한글 글리프 8 px(`GetGlyphWidth_Normal`), 공백 3 px, `!` 4 px 기준 — 트림 실패 둘째 줄 약 155 px(배틀 문장 창 208 px), `이 기술은 쓸 수 없다!` 약 80 px. 같은 둘째 줄은 기존 `BELCHCANTSELECT`(팰리스 배틀 문장 경로)에서도 쓰던 줄.

## HnS 보정(upstream 결함, 재확인 101)

- **기술로 얼면 다음 행동에서 무조건 녹음:** upstream #10151은 `MOVE_EFFECT_FREEZE`에서 `freezeTurns = 2`를 넣고, 얼음 판정은 `freezeTurns < 2`일 때만 얼어 있게 함 → 냉동빔 등으로 언 포켓몬은 **모든 `B_FREEZE_TURNS` 설정에서** 다음 행동에 녹는다(Champions 테스트는 전부 `Status1(STATUS1_FREEZE)`로 시작해 `freezeTurns = 0` 경로만 봄, 1.17.1·master 같음, upstream·HnS 테스트·한글 회귀 세트 어디에도 기술로 얼린 뒤 얼어 있는지 보는 검사 없음). HnS는 0을 넣어 처음부터 언 포켓몬과 같게 셈(친구 지시 "3번째 얼음 행동 판정 기준"과 같음, 9세대 설정이면 이식 전과 같음). 확인 테스트 `HnS 10151: a Pokemon frozen by a move stays frozen for two actions and thaws on the third`(**미실행**).

## 출력 변화(`BATTLE_MESSAGE_OUTPUT_CHANGES.md` 행)

- 새 문장 2개(위). 트림 실패, 선택 불가 4종.
- 섀도다이브·고스트다이브·이차원홀이 방어를 뚫을 때 `…은(는)\n페인트에 걸렸다!` → `…의\n방어를 깨뜨렸다!`(이전 HnS·upstream은 이차원러시만 "깨뜨렸다"). 페인트는 그대로.
- 프리즈드라이 설명(영문, 미번역 그대로): HnS 데이터가 이미 Champions 분기(얼음 효과 없음)라 `…May cause frostbite/freezing.` → `Super effective on Water-\ntypes.`
- 트림 애니 효과음 `SE_SUCCESS` → `SE_FALL`(upstream 그대로).

## 정적 확인(빌드 안 함)

- 3-way: #10151 45파일 중 33개 깨끗, 12파일 충돌 23곳 → 위 방식으로. #10282 39파일 중 소스 6파일은 손으로, 테스트 33개 3-way 깨끗.
- 새로 쓰는 식별자 21개(`SetCantSelectScript`, `BattleScript_SelectingCantUseMove(InPalace)`, `BattleScript_BelchFails`, `BattleScript_ItemStealNoAnim`, `STRINGID_CANTUSEMOVE`/`BELCHCANTUSE`, `B_MSG_BROKE_THROUGH_PROTECT`, `VOLATILE_SPEED_SWAP`, `CalculateMonStatsCont`, `freezeTurns`, `MOVE_LIMITATION_UNUSABLE` 등)의 정의·선언·헤더 경로(`battle.h` → `battle_script_commands.h`·`config_changes.h`·`battle_util.h`) 확인. `GetConfig(B_…)`로 읽는 설정은 모두 `config_changes.h` 목록에 있음. 지워진 이름(`B_MSG_HYPERSPACE_FURY`, `GetZMaxMoveAgainstProtectionModifier`, `.multistring`, `DYNAMAX_BYPASS_CHECK`)은 소스·테스트·한글 회귀 세트에 0.
- 세이브: `PartyState`(배틀 전용, `padding:5` → `freezeTurns:2` + `padding:3`, 32비트 그대로), 휘발 상태 비트 1개 추가, `STRINGID` 2개 추가 — 세이브 블록 밖(아래 검증의 세이브 정적 비교로 확인).
- **녹화 배틀:** 세이브 구조는 바뀌지 않지만, 마비(가중 난수로 바뀜)·얼음·수면 판정과 선택 제한이 바뀌어 **이식 전에 녹화한 배틀이 같은 결과로 재생된다는 보장은 없다**(묶음 49의 트라이어택 목록 순서 변화도 같은 종류 — 확률·문장은 같아도 같은 난수 값의 결과가 다를 수 있음).
- **RNG 태그 의미 변화:** `WITH_RNG(RNG_PARALYSIS, x)`가 이제 가중 난수 인덱스(1 = 행동 불가, 이전 `RandomPercentage`는 FALSE = 행동 불가). #10282가 고친 upstream 테스트는 새 의미로 들어옴. 한글 회귀 세트는 `zz_hns9168_k1_berry.c` 3곳(K05·K26)에서 `WITH_RNG(RNG_PARALYSIS, FALSE)`를 씀(1차 문서에 "사용 0"이라 적은 것은 grep 출력이 잘려 놓친 것 — 정정). 세 곳 모두 열매로 마비가 먼저 풀려 판정을 타지 않고(상태 그대로 PASS), 작성 의도(움직임)는 새 의미와 맞음. 테스트 러너는 `WITH_RNG` 없는 가중 난수에 마지막 칸(= 마비로 못 움직임)을 줘서 HNS9680 1-11에 고정을 넣음(아래).

## 한글 회귀 테스트

`test/hns_10151_champions.c` 8개 — 트림 실패 문장(내 쪽·상대 쪽), 속이기 두 번째 턴 선택 불가(속이기만 가진 포켓몬이 발버둥), 기술로 언 포켓몬 2행동 유지 후 3번째 해동(HnS 보정), 마비 1/8, 소금절이 1/16, 페인트·고스트다이브 방어 관통 문장, 탁쳐서떨구기 사용자 기절 뒤 적용.

## 검증(2차, 2026-10-26)

- **빌드:** `make hns -j8` 전체 재빌드(`build/hns` 지우고) 종료 0, 새 컴파일 경고 0(`warncheck.sh`). 1차 정적 이식이 한 번에 컴파일됨.
- **전체 테스트**(코드 `ba5922ac`): `test-baseline-seq484.txt` 대비 **새 PASS 36, 사라진 PASS 11 — 11개 모두 upstream 이름 변경**(`(Gen9)`·`(Gen6-9)`·`(Champions)` 등이 붙은 같은 테스트가 새 이름으로 PASS), **실제로 사라진 PASS 0**. INVALID 24·EXPECTED_FAIL 6은 이전 실행과 같은 목록. 새 기준 [`test-baseline-seq475.8.txt`](test-baseline-seq475.8.txt)(**다음 비교 기준**).
  - 첫 실행(`131f14ba`~`efca3359`)에서 바뀐 것과 처리(`ba5922ac`): HnS `GEN_LATEST = GEN_CHAMPIONS` 때문에 9세대 기본값을 전제한 upstream 테스트 6개가 상태가 바뀜 — `Unseen Fist continues bypassing protect effects after being replaced during a multi-strike move` PASS → FAIL(Champions는 접촉 방어 효과 적용), `Last Resort works only when all of the known moves have been used - 2/3/4 moves` FAIL → INVALID(Champions는 비장의무기 선택 불가), `EXPECT_FAIL detects failures caused by EXPECT_MOVE/EXPECT_SWITCH` EXPECTED_FAIL → KNOWN_FAILING_PASS(구애스카프로 묶인 속이기를 2턴째에 고를 수 없어 발버둥으로 넘어가 AI 선택 검사가 안 일어남 — 엔진 결함 아님). #10282가 다른 테스트에 한 것처럼 각 테스트에 `WITH_CONFIG(…, GEN_9)`(`// HnS:`) → 이전 상태로 돌아옴(대상 테스트만 다시 돌려 확인).
  - 새 FAIL 28개(새·이름 바뀐 upstream 테스트)는 영문 문장 검사로 HnS 기준에서도 FAIL인 종류(같은 동작의 한글 테스트는 PASS). upstream `KNOWN_FAILING` 4개 추가(우격다짐 vs 발끈·분노의껍질·변색·나쁜손버릇 Champions — 설정만 있는 부분).
- **새 한글 회귀 `test/hns_10151_champions.c` 8개: 8/8 PASS.**
  - 이식 전 코드(클론 `ad68cdcc`, 새 설정 `ASSUME`만 뺀 `ZZ old` 사본, 커밋 안 함·지움): **0/8** — 모두 의도한 변화(트림 2개는 이전엔 선택 불가라 ERROR). 처음 쓴 속이기 테스트는 이식 전에도 PASS여서(`allowed: FALSE`는 그 기술을 기록에서 빼기만 함) 발버둥 확인으로 바꿈.
  - HnS 얼음 보정: `freezeTurns = 0`을 upstream 형태(2)로 되돌리면 `HnS 10151: a Pokemon frozen by a move …` FAIL, 보정으로 PASS(되돌린 것은 복구, 커밋 안 함).
- **세이브 정적 비교**(이전 = 클론 `ad68cdcc` 빌드, ROM SHA1 `9b5935b0…` = 묶음 50 기록과 같음): **PASS(FAIL 0, WARN 4)** — ① `BoxMonRestorePP` assert 줄 번호 상수 244 → 245(`move.h` 1줄), ② `CalculateMonStats`가 `CalculateMonStatsCont(mon, TRUE)`를 부르는 껍데기가 되고 `BoxMonToMon` 등이 그 경로로(검토 묶음 3건) — 새 `CalculateMonStatsCont` 본문을 옛 `CalculateMonStats`와 objdump 정규화로 비교해 256줄 **완전히 같음**(인자 `updateSpeedStat`은 upstream처럼 본문에서 안 씀). 세이브 블록 크기·섹터 배치·EWRAM/IWRAM 같음.
- **한글 회귀**(`kortests/run.sh <저장소> …`): 첫 실행 `kor-s51` 554/652 — 기대와 다른 2줄: `HNS9680 2-08` 소금절이 칸(`HP_BAR(opponent, damage: 37)` 불일치 — Champions 1/16, 의도한 변화)·`HNS9680 1-11`(마비 상태로 비바라기 — 위 가중 난수 기본값). 1-11에 `WITH_RNG(RNG_PARALYSIS, FALSE)`(같은 줄) → `kor-s51b` 555/652, 차이는 2-08 한 줄 → 기대 요약을 이 실행으로 바꾸고 `summary.meta`에 이유(선례 묶음 48) → `kor-s51c` **`판정: PASS (기대와 같음)`, 요약 바이트 같음**. 1차에 바뀔 수 있다고 적은 `HNS9717 K1-06`은 상태 변화 없음.
- **ROM**(커밋 `ba5922ac` = 게임 코드 `3ab91838`): SHA1 **`37e70fcf34e042c1b5b826f20870c5c54f1dfb59`**, 사용 32,961,236 B(이전 32,959,044 B, **+2,192**), 파일 33,554,432 B, 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 하지 않음(친구).** 후보 — 트림(열매 없이), 속이기 2턴째 선택, 냉동빔으로 얼린 상대가 2턴 얼어 있는지, 마비 빈도, 소금절이 피해, 고스트다이브로 방어 뚫기 문장, 앙코르 우선도.

## 순서표

475.5·475.8 적용(`131f14ba`). 475.6 `28407e92`·475.7 `78571464` 기록 유지.
