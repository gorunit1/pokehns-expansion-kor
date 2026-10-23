# full-sync 묶음 27 — seq 381~384

- 요청: 사용자 "나 혼자 할 수 있는거야? 그렇다면 ㄱㄱ"(2026-10-23). 사전 분석(세션 임시 `m10145/` 등 3-way 병합)으로 사용자 혼자 정할 수 있는 범위라고 보고 바로 적용했다. 판단 근거는 아래 "결정"이다.
- 범위: seq 381~384. 385 #10214는 묶음 26에서 앞당김(다시 넣지 않음). 다음은 385.5 #10454.
- upstream: 읽기 전용 사본 `/home/user/pokeemerald-expansion-upstream`에서 `git format-patch`/`git show`로 읽었다.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 381 | #10181 | `ddbef189` | 형식 정리(87파일, `u16`/`u32` → `enum Item`·`Species`·`Move`·`Ability` 등) | 32,963,828 |
| 382 | #9147 | `add4d68b` | 문 크기 리팩터(`enum DoorSize`, 크기별 열기·닫기 프레임) + HnS 적응 | 32,963,796 |
| 383 | #10145 | `a3572d8d` | 데미지 계산이 파트너·필드 특성을 문맥에서 읽음, 총대장 카운터를 volatile로 | 32,963,060 |
| 384 | #10211 | (변경 없음) | 디버그 표 `const` 위치 — HnS에 이미 있음(9/15 선별 이식분) | — |

이식 전(`cf44d86e`) 32,963,268 B → **32,963,060 B(−208)**, EWRAM 250,408·IWRAM 25,516 같음, 최종 SHA1 **`d0b08d51dd6dc6aa7400d7509be655d29c23cbd5`**(코드 `a3572d8d`).

## PR별 내용과 HnS 판단

### 381 #10181 — Type checking paradise

- 형식만 바꾼다(지역 변수·인자·필드·표의 `u16`/`u32`를 enum으로). 값·크기는 같다.
- HnS 문맥이 달라 손으로 넣은 곳: `PartyState.usedHeldItem`, `GetMoveSlot` 선언, `SetTypeBeforeUsingMove`의 `heldItem`, 잡은 포켓몬 잃은 도구(HnS는 `B_TRAINER_OPPONENT_A`), 도감 분포 화면 숨김 종 목록, `GetEvolutionTargetSpecies`, 트레이너힐 상품 목록(HnS·Emerald 목록 모두). `TryRestoreHeldItems`의 HnS `u32 lostItem`은 그대로, upstream 빈 줄 공백 hunk는 넣지 않았다.
- ROM +560 B는 enum 형식 때문에 바뀐 코드 생성(값 변화 없음, 아래 테스트·세이브 비교).

### 382 #9147 — Door Size Refactor

- upstream: 문 크기가 `enum DoorSize`(1x1, 1x2, 2x2 왼쪽/오른쪽)가 되고 크기마다 열기·닫기 프레임 짝(`sDoorSizeInfo`)을 쓴다. Emerald·FRLG 표는 지정 초기화, 멀티 통로 문은 오른쪽 절반 항목과 팔레트 16개가 생긴다.
- **HnS 적응(문 동작 유지):** HnS 레이아웃은 FRLG 코드로 표 값보다 한 단계 작은 크기, 타일 `DOOR_TILE_START_HNS`(434, 알로라섬은 마지막 16타일)에서 그렸다. HnS 표는 Emerald 크기 값을 그대로 두고(일반 센터·마트 문이 Emerald 맵과 같은 항목) `GetDoorSize()`·`GetDoorTileStart()`가 HnS 레이아웃에 1x1/1x2와 HnS 타일 시작을 돌려준다. HnS 프레임 표(`OpenSmallHns` 등)는 새 1x1/1x2 프레임과 같아 지웠다.
- 유지: HnS INCBIN 타일 선언, HnS 타일·팔레트, 배틀프런티어 동쪽 문 2항목(Emerald 표에도 넣음), `ShouldUseMultiCorridorDoor`의 HnS 맵 조건. HnS 표의 멀티 통로 문은 이름이 바뀐 `_Left` 메타타일 + `_Right` 항목(upstream과 같음).
- **upstream이 고친 것(HnS에 닿는 곳):** 이미 열린 채로 그리는 문(`FieldSetDoorOpened` — 문 워프로 도착할 때)이 자기 크기의 마지막 프레임을 쓴다. 이전엔 HnS 문이 모두 작은 문 마지막 프레임(8타일 위치)을 썼는데, HnS에서 1x2인 문(바다 오두막 텔레포터)은 이제 16타일 위치(완전히 열린 그림)를 쓴다. 일반 HnS 1x1 문은 같다.

### 383 #10145 — Improve AI thinking time by removing direct ability calls in calc dmg

- upstream: 데미지 계산이 파트너·필드 특성과 도구 효과를 실제 배틀러 상태 대신 `DamageContext`(`ctx->abilities`·`ctx->holdEffects`)에서 읽고, `AI_CalcDamage`는 모든 배틀러 칸을 채운다. 총대장 카운터가 `BattleStruct` 배열에서 배틀러 volatile(`VOLATILE_SUPREME_OVERLORD_COUNTER`)로 옮겨진다.
- **HnS 적응:**
  - 총대장: 등장 발동과 AI 가상 등장 예측 모두 #10186(seq 231)의 트레이너별 기절 수(`faintCounter[GetBattlerTrainer]`)를 유지 — upstream 최종형(1.17.0 태그·HEAD)과 같다. AI 예측의 HnS `GEN_CHAMPIONS` 게이트는 지웠다(`B_UPDATED_ABILITY_DATA` = `GEN_LATEST` = `GEN_CHAMPIONS`라 항상 참). `InitializeSwitchinCandidate`의 HnS 저장·복원(`savedSupremeOverlordCounter`)도 지웠다 — 카운터가 `gBattleMons`에 있고 후보 루프 호출 3곳이 모두 `AllocSaveBattleMons`/`FreeRestoreBattleMons`로 감싼다(재확인 46 해결).
  - `GetBattlerSideFaintCounter` 공개 선언은 넣지 않았다(#10186이 이미 지움, upstream HEAD에도 없음).
  - 파트너 특성 블록(배터리·파워스폿·강철정신, 플러스·마이너스, 플라워기프트 공격·방어)은 upstream HEAD처럼 `GetPartnerBattler`.
  - HnS 과도기 함수 `GetDamageCalcAbility`·`IsDamageCalcAbilityOnField`(9/15 선별 이식 때 들어온 것)를 지우고 upstream `ctx->abilities` 읽기와 `DmgCalc_IsAbilityOnField`로 바꿨다. **#9710 D3 과도기 결함 해소**(full-sync-seq-150-154 메모): 예전 AI 문맥은 공격자 파트너·방어자 파트너 칸을 따로 채워서, AI가 자기 편을 치는 기술을 계산할 때 공격자 칸이 덮였다. 이제 배틀러마다 한 번씩 채운다.
  - `AI_GetSwitchinFieldStatus`는 HnS 지역 변수 `ability` 유지, `test/battle/ai/ai_thinking_time.c`는 HnS 프레임 상한 유지(upstream이 낮춘 값 대신).
- **동작 차이(upstream과 같음, 실제 배틀):**
  - 파트너·필드 특성 판정이 `IsBattlerAlive` 대신 문맥의 특성 값을 본다. 실전 데미지 문맥은 `GetBattlerAbility`로 모든 배틀러를 채우고, 이 함수는 필드에 없는 배틀러(기절 처리 뒤 `notOnField`)에 `ABILITY_NONE`을 돌려주므로 보통은 같다. 차이는 같은 행동 안에서 파트너가 HP 0이 됐지만 기절 처리 전인 짧은 틈뿐이다.
  - 다크오라·페어리오라·오라브레이크가 데미지에 반영될 때 AI 특성 기록(`RecordAbilityBattle`)을 남긴다(AI 지식만, 화면 변화 없음).
  - 플라워기프트 파트너의 날씨·도구 판정이 문맥 값(`ctx->weather`·`ctx->holdEffects`)으로 — 실전에서는 같은 값.
- **AI 예상 데미지:** AI 문맥이 모든 배틀러 특성을 채우므로, AI가 더블배틀 파트너·필드 특성을 계산에 넣는 방식이 upstream과 같아진다(HnS 과도기 함수는 `gAiLogicData`로 대신 읽었음 — 결과는 대부분 같고, 위 D3 경우만 바로잡힘). 형틀 깨기류 특성을 무시해야 하는 상대 칸은 upstream처럼 `AI_GetMoldBreakerSanitizedAbility`를 거친다.

### 384 #10211 — fix(debug): incorrect use of const

- `src/debug.c` `generateListFunctions`의 `const` 위치. HnS에는 이미 `static u32 (*const generateListFunctions[])(...)`로 들어 있다(2026-09-15 1.17.0 선별 이식분). 변경 0, 순서표에 "이미 적용"으로 기록.

## 결정 (사용자 혼자 정할 수 있다고 본 이유)

- 381·384는 형식·디버그만이다.
- 382는 HnS 문 동작을 그대로 두게 맞췄고, 바뀌는 건 upstream이 고친 "열린 채 그리는 1x2 문 프레임" 1곳뿐이다(텔레포터).
- 383의 HnS 고유 부분(총대장 트레이너별 기절 수)은 사용자 결정 3(2026-10-10, 재확인 46)과 upstream 최종형이 같아 그대로 이어진다. 지운 HnS 게이트·저장 복원은 결과가 같은 코드다. 나머지는 upstream이 받아들인 AI 계산 변화다(#9710 D5와 같은 종류).
- 새 배틀 문장 0, 기존 한글 텍스트 변경 0, 설정 값 변경 0, 세이브 구조 변경 0. AI 교체 가드(재확인 56)는 건드리지 않았다.

## 검증

- **빌드:** 커밋마다 `make hns -j8` 종료 0, 새 경고 0(`warncheck.sh`). ROM은 위 표.
- **세이브 정적 비교**(이식 전 사실 = 세션 임시 `b27-save-pre`, 코드 `cf44d86e`를 클론에서 빌드 — ROM 32,963,268 B로 같음): **PASS(FAIL 0, WARN 0)**. `BattleStruct`(−4 B)·`BattlePokemon` volatile 비트는 세이브 밖.
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 `a3572d8d`): 러너 PASSED 2,749 / TOTAL 5,713(KNOWN_FAILING 25), 목록 PASS 2,744 — **`test-baseline-seq380.txt`와 바이트 같음**(사라진 PASS 0, 새 PASS 0). AI 생각 시간 테스트 6개 PASS(HnS 프레임 상한), 총대장 멀티배틀 테스트 2개 PASS. 총대장 `upon switch in` 2/2 FAIL은 이식 전 기준에서도 같은 상태. 새 기준 [`test-baseline-seq384.txt`](test-baseline-seq384.txt)(내용은 seq380과 같음, **다음 비교 기준**).
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-b27 8`, 코드 `a3572d8d`): **558/652, 기대와 바이트 같음, `판정: PASS (기대와 같음)`**(`HNSB2021` 30/30 포함).
- **실기: 하지 않음(친구).**

## 실기(mGBA) 후보 — 친구

1. 문: 여러 마을의 센터·마트·집 문 출입(열림·닫힘 애니), 문으로 나올 때 열린 문 그림, 바다 오두막 텔레포터.
2. 더블배틀(가능하면): 파트너가 플러스/마이너스·배터리·플라워기프트(쾌청)인 경우 데미지, 다크오라·페어리오라.
3. 총대장(랜더마이저 등으로 있을 때): 동료가 쓰러진 뒤 등장 문장·위력.
4. 트레이너힐 상품, 도감 분포 화면(형식 변경 — 값 같음, 확인만).

## 재확인·뒤 PR 메모

- 재확인 46 해결(#10145에서 같은 줄 손 병합 — 위).
- 새 재확인 항목 없음.
- 순서표: 381·382·383 적용, 384 이미 적용.
- 다음은 seq 385.5 #10454(Add Gen 9 Overworld Weather Behavior, 시작 상태 날씨 뒤).
