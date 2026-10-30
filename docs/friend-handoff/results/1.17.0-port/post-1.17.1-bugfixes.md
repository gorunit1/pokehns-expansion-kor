# 1.17.1 뒤 upstream 버그 수정 8건 선택 이식 (2026-10-30)

- 요청: 친구 [`FRIEND_REQUEST_2026-10-30.md`](../../FRIEND_REQUEST_2026-10-30.md)(사용자 전달). 1.17.2 개발판 기능이 아니라 1.17.1 기반 HnS 안정성 — 8건만, 각 PR의 실제 diff와 HnS 코드를 먼저 대조, 이미 해결됐으면 손대지 않음, 대규모 후속 변경 제외.
- 기준: HnS `cc6fe552`(seq 1~556 이식 끝 + 완료 점검 + 매지컬리프·버전 플래그). upstream 읽기 전용 사본(`origin/master`·`origin/upcoming`, 2026-10-30 fetch).
- 방법: PR마다 upstream 커밋 diff → HnS 같은 함수 대조 → upstream 테스트(또는 HnS용으로 고친 같은 장면)를 **수정 전 코드에서 먼저 돌려 재현** → 필요한 줄만 넣고 같은 테스트 PASS → 관련 기존 테스트가 기준 목록과 같은지.

## 1. 결과 요약

| PR | upstream 커밋 (브랜치) | 결과 | HnS 커밋 |
|---|---|---|---|
| ① #10953 날씨 + 반무적 기술 assert | `7e33db242a` (upcoming) | **NOT AFFECTED** | 없음(회귀 테스트만 `aa04eeef`) |
| ② #10866 2대1 HP 박스 손상 | `6057b187f9` (master) | **APPLIED**(상대 쪽만, HnS 구조) | `dddceca7` |
| ③ #10974 맹독구슬·화염구슬 방지 판정 | `897e98eaab` (master) | **APPLIED** | `77888fde` (+ HnS 테스트 `aa04eeef`) |
| ④ #10972 난기류 비행 약점 판정 | `44bc3e0473` (master) | **APPLIED** | `9cfa17b2` (+ HnS 테스트 `aa04eeef`) |
| #10933 포획 포켓몬 파티 편입 취소 | `2a76d9eb3c` (upcoming) | **NOT AFFECTED** | 없음 |
| #10878 야생 포켓몬 지닌 도구 복구 | `7b95be15d8` (master) | **APPLIED(일부)** — 복구 자체는 ALREADY FIXED, 도둑질 + 포획 때 도구 복제만 고침 | `4c447b25` |
| #10910 프런티어 다이맥스·테라스탈 | `0fcf8fb4a8` (master) | **ALREADY FIXED**(HnS 재확인 86 규칙) | 없음 |
| #10596 녹화 배틀 파티 손상 | `30988a58b1` (master) | **ALREADY FIXED**(HnS 재확인 20) | 없음 |

`#10850`(`OBJ_KIND_NORMAL`) 등 요청 밖 PR은 보지 않음. 1.17.1 뒤 다른 upstream 커밋은 넣지 않음.

## 2. PR별

### ① #10953 — NOT AFFECTED

- **upstream:** `include/move.h` `GetTwoTurnMoveWeather`의 assert에 `EFFECT_SEMI_INVULNERABLE`·`EFFECT_GEOMANCY` 추가 + 테스트 1개.
- **원인(upstream):** upcoming의 #10803(`a01cea43d0` "Combine two turn move checks…")이 `CanTwoTurnMoveFireThisTurn`의 첫 줄 `if (moveEffect == EFFECT_GEOMANCY || gBattleMoveEffects[moveEffect].semiInvulnerableEffect) return FALSE;`를 없애서, 날씨가 있으면 구멍파기 등이 `GetTwoTurnMoveWeather`까지 가 assert. **1.17.1·master에는 #10803이 없음.**
- **HnS:** `include/move.h:620` assert는 옛 조건 그대로(두 턴 공격·솔라빔만)지만, `src/battle_move_resolution.c` `CanTwoTurnMoveFireThisTurn` 첫 줄이 반무적·지오컨트롤을 먼저 걸러 이 함수에 닿지 않음. 다른 호출(`battle_ai_util.c` `IsTwoTurnNotSemiInvulnerableMove`)은 `EFFECT_SOLAR_BEAM`·`EFFECT_TWO_TURNS_ATTACK`일 때만 부름 → assert 조건을 바꿀 필요 없음(바꾸지 않음).
- **확인:** HnS 테스트 `HnS post-1.17.1 #10953`(비바라기+구멍파기, 쾌청+공중날기, 비바라기+다이빙, 모래바람+뛰어오르다, 설경+고스트다이브, 쾌청+지오컨트롤) — 수정 없이 PASS.

### ② #10866 — APPLIED (HnS 구조에 맞춤)

- **upstream:** `src/battle_gfx_sfx_util.c` `BattleLoadAllHealthBoxesGfx`를 switch로 정리하면서, 더블배틀에서 **상대 왼쪽이 싱글 배치(`BATTLE_COORDS_SINGLES`)면 싱글 상대 HP 박스 그림을 로드**.
- **원인(HnS도 같음):** `GetBattlerCoordsIndex`는 더블배틀이라도 상대 트레이너 파티가 1마리(두 트레이너 아님)면 상대 왼쪽을 싱글 배치로 그림(`CreateBattlerHealthboxSprites` 싱글 갈래 — 0x1000 그림 기준). 그런데 그림 로드 state 4는 늘 더블 상대 그림(0x800)을 `TAG_HEALTHBOX_OPPONENT1_TILE`로만 올려, 싱글 박스를 그리는 동안 뒤에 붙은 오른쪽 상대 칸(`OPPONENT2`) 타일을 덮음. 플레이어 쪽 같은 경우(state 2)는 이미 처리돼 있었음.
- **수정:** state 4에 플레이어 쪽(state 2)과 같은 꼴의 switch — 상대 왼쪽이 싱글 배치면 `GetSinglesOpponentHealthbox()`, 아니면 지금처럼 더블 그림. **upstream과 다른 점:** 함수 전체 리팩터는 받지 않고 그 갈래만, 그림은 HnS getter(Gen3/Gen4 배틀 UI 선택 `UseGen4BattleUI()`)로 — HnS 싱글 경로와 같게 `B_HP_PERCENTAGE_DISPLAY` 큰 박스 갈래는 없음(HnS 값 FALSE). 메가진화 아이콘·레벨 표시·HP 박스 HnS 수정은 다른 함수라 그대로.
- **HnS에서 닿는 경로:** HnS 맵 트레이너 중 1마리짜리 더블 트레이너는 없음(`trainers_hns.party` 더블 9명 모두 2마리), 따라오기 NPC(`FNPC_ENABLE_NPC_FOLLOWERS FALSE`)도 꺼짐 → **일반 진행에서는 거의 안 닿고, 디버그 메뉴 Trainers…의 Double Battle 켜기**로 닿음.
- **확인:** 빌드·전체 테스트(헤드리스는 그림을 보지 않음). 화면은 실기(아래 4절).

### ③ #10974 — APPLIED

- **upstream·HnS 같은 원인:** `TryToxicOrb`·`TryFlameOrb`가 `CanBePoisoned`/`CanBeBurned` → `CanSetNonVolatileStatus`를 거치는데, 그 안의 플라워베일·신비의부적 검사가 자기 자신에게 거는 구슬도 막음.
- **수정:** upstream 그대로 — `BattleStruct`의 빈 비트 `unused2` → `ignoreOrbFailure`, 두 함수가 검사 동안만 TRUE, `CanSetNonVolatileStatus`의 플라워베일·신비의부적 두 갈래에 `!gBattleStruct->ignoreOrbFailure`. 독·화상 면역 타입·특성(면역·수의베일·불꽃/독/강철 타입 등)과 부식 판정은 그 앞에서 그대로. **HnS `BattleScript_ToxicOrb`·`BattleScript_FlameOrb`(도구 팝업 + 한글 문장) 호출과 발동 순서는 그대로.**
- **테스트:** upstream 2개(`Flower Veil has no effect on Flame Orb or Toxic Orb`, `Safeguard has no effect on Flame Orb or Toxic Orb`) 그대로 넣음 — 수정 전 **각 1/2 FAIL(재현)**, 수정 뒤 PASS. HnS 테스트 `#10974`(불꽃 타입·수의베일·독 타입·면역은 신비의부적 아래서도 계속 막음) PASS.

### ④ #10972 — APPLIED

- **upstream·HnS 같은 원인:** `CancelerPreAnimActivations` `PRE_ANIM_STRONG_WINDS`가 "효과가 굉장함 + 대상이 비행 타입"이면 난기류 문장을 냄 → 메가레쿠쟈(드래곤/비행)가 용의파동(드래곤 약점)을 맞아도 문장이 나옴. **대미지·상성 계산은 원래 맞음(문장만 틀림).**
- **수정:** upstream 그대로 — 대상 타입 3칸(테라 포함, `GetBattlerTypes(…, FALSE, …)`) 중 비행이 있고 `GetTypeModifier(기술 타입, TYPE_FLYING) > 1.0`일 때만.
- **테스트:** upstream 새 테스트는 영문 `NOT MESSAGE`를 애니 **뒤**에 둬서(난기류 문장은 애니 앞에 나옴) HnS에서 의미가 없음 → 넣지 않고(이름 바꾸기 hunk도 안 받음) HnS 테스트 `#10972`를 한글 문장으로: 용의파동(드래곤 약점) = 문장 없음·효과 굉장함, 돌떨구기(비행 약점만)·냉동빔(둘 다) = 문장. 수정 전 **1/3 FAIL(재현 — 용의파동에 문장)**, 수정 뒤 PASS. 기존 난기류 테스트는 기준 목록과 같음(반전배틀 PASS 유지, FAIL 5개는 원래 영문 비교 FAIL).

### #10933 — NOT AFFECTED

- **upstream 원인:** upcoming #10784가 `PARTY_MON_CANCEL`을 `PARTY_MON_NONE`과 같은 값(`PARTY_SIZE`, `include/constants/global.h`)으로 바꿔, `GIVECAUGHTMON_HANDLE_CHOSEN_MON`의 `if (gSelectedMonPartyId != PARTY_MON_NONE)`이 취소를 "아직 고르지 않음"으로 보고 다음 단계로 안 넘어감 → 진행 불능.
- **HnS(= 1.17.1):** 파티 메뉴 취소가 `gSelectedMonPartyId = PARTY_SIZE + 1`(`src/party_menu.c` `PARTY_ACTION_SEND_MON_TO_BOX`), `Cmd_givecaughtmon`은 `> PARTY_SIZE`를 취소로 받아 `GIVECAUGHTMON_GIVE_AND_SHOW_MSG`로 넘어감 — 진행 불능 경로 없음. **수정하지 않음.**

### #10878 — APPLIED(일부)

- **upstream:** 10파일(`LostItem` 구조 변경, 복구 플래그, `StealTargetItem` 인자 추가, `TryRestoreHeldItems` 재작성, 포획 때 `RestoreCaughtWildMonHeldItem`, 테스트 3개).
- **HnS 대조·재현(upstream 테스트 장면을 HnS용으로 — HnS 볼은 볼 바꾸기 때문에 파티 메뉴형 도구라 `USE_ITEM`에 `partyIndex` 필요, upstream 테스트 그대로는 INVALID):**
  - 야생전 뒤 플레이어 도구 복구(이 PR 제목의 핵심, upstream 1.17.1은 #10483 뒤 야생전에서 복구 안 함): **HnS는 이미 함** — seq 313 HnS 적응(`TryRestoreHeldItems`의 `B_RESTORE_HELD_BATTLE_ITEMS >= GEN_9 ||` 유지).
  - 트릭으로 준 도구 + 포획: **복제 없음**(PASS, 수정 없이).
  - 도둑질·탐내다로 훔친 야생 도구 + 포획: **복제 재현** — Gen9 규칙으로 훔치는 순간 가방에 들어가고, 포획 때 `GIVECAUGHTMON_GIVE_AND_SHOW_MSG`가 잡은 포켓몬의 원래 도구를 되돌려 같은 도구가 둘(가방 + 잡은 포켓몬). 수정 전 HnS 테스트 FAIL(가방에 있음 + 잡은 포켓몬도 지님).
- **수정(필요한 것만):** `struct LostItem`에 `wildItemPending:1` 1비트, `StealTargetItem`의 Gen9 야생 갈래는 바로 `AddBagItem` 대신 그 표시만, 포획 때 표시가 있으면 잡은 포켓몬에게 원래 도구를 주고 표시를 지움, `TryRestoreHeldItems` 첫머리에서 남은 표시를 가방에 넣음(배틀이 어떻게 끝나든 `HandleEndTurn_FinishBattle`에서 한 번). → upstream과 같은 결과: **잡으면 그 포켓몬이 도구를 지니고 가방에는 없음, 안 잡으면 배틀 끝에 가방.**
- **upstream과 다른 점:** `originalItem` 폭(15비트 유지), `restoreAfterBattle` 플래그·`StealTargetItem` 새 인자·`GetLostItemState`·복구 함수 재작성·`TryRestoreHeldItems`가 모든 트레이너를 도는 부분은 받지 않음(HnS 복구 규칙과 seq 313 적응을 바꾸지 않으려고). upstream 테스트 3개는 위 이유로 넣지 않고 HnS 테스트 3개로.
- **바뀌는 것(실기):** 훔친 순간이 아니라 배틀이 끝날 때 가방에 들어감(문장 `…을 빼앗았다!`은 그대로 훔칠 때).

### #10910 — ALREADY FIXED

- **upstream:** `CreateFacilityMon`이 `shouldUseDynamax`가 없으면 다이맥스 레벨에 `BLOCK_AI_DYNAMAX`, 테라 타입이 없으면 `TYPE_MYSTERY`를 넣음(일반 트레이너 생성 `trainer_util.c`와 같게).
- **HnS:** `CreateFacilityMon`은 수정 전 upstream과 같지만, `src/battle_gimmick.c` `ShouldTrainerBattlerUseGimmick`이 HnS 규칙(재확인 86, seq 400/438 `6029378d` — "#9440 전처럼 트레이너 파티 데이터로 만든 상대만 기믹")으로 **프런티어·e리더·트레이너힐·비밀기지·링크·녹화 배틀 상대는 테라·다이맥스를 쓰지 않음** → 의도하지 않은 발동 없음. 프런티어 기믹 규칙을 보존하려고 **수정하지 않음.**

### #10596 — ALREADY FIXED

- **upstream:** ① `MoveRecordedBattleToSaveData`가 `gParties` 대신 `sSavedParties`(배틀 시작 파티)를 저장 ② `CB2_InitBattle`의 heap 초기화 조건을 `!gTestRunnerEnabled` → `!(gBattleTypeFlags & BATTLE_TYPE_RECORDED)`(upstream `sSavedParties`가 heap 포인터라 녹화 재생 시작 때 지워지던 것).
- **HnS:** ①은 이미 같음(`src/recorded_battle.c:294`, 재확인 20). HnS `sSavedParties`는 heap이 아니라 정적 EWRAM 배열(재확인 20 HnS 수정)이라 ②가 지키려던 데이터가 heap에 없음 — `recorded_battle.c`의 다른 `AllocZeroed`는 모두 같은 함수 안에서 `Free`. ②는 테스트 러너 heap 동작도 바꾸므로 **받지 않음.**

## 3. 검증

- **재현 → 수정 확인(지정 테스트, `GITHUB_ACTION=1 make check BUILD=hns -j8 TESTS=…`):** 위 각 절. 수정 뒤 `HnS post-1.17.1` 6/6 PASS, upstream `Flower Veil has no effect…`·`Safeguard has no effect…` PASS. 관련 기존 테스트 묶음(플라워베일·신비의부적·난기류·도둑질/탐내다·탁쳐서떨구기·트릭·픽포켓·매지션·끈적끈적바늘·건네주기·공생)의 PASS 수가 `test-baseline-seq556.txt`와 같음.
- **전체 검증(`verify_seq.sh post1171 cc6fe552`, 코드 = `4c447b25`):**
  - 빌드: 이식 전 클론(`cc6fe552`) SHA1 `83e55d27…`(= 친구 답 반영 ROM). 저장소 전체 재빌드 종료 0, SHA1 `c5132aa0…`, 새 컴파일 경고 0(`warncheck.sh`).
  - 전체 테스트(`GITHUB_ACTION=1 make check BUILD=hns -j8`): `test-baseline-seq556.txt` 대비 **새 PASS 8**(HnS `HnS post-1.17.1` 6 + upstream `Flower Veil has no effect on Flame Orb or Toxic Orb`·`Safeguard has no effect on Flame Orb or Toxic Orb`), **사라진 PASS 0**, 다른 줄 같음(FLAKY 1 = 재확인 113 Teraform Zero, INVALID 26 이전 실행과 같은 수). 새 기준 **`test-baseline-post1171.txt`**(`verify_seq.sh`도).
  - 세이브 정적 비교(이전 = 클론 `cc6fe552` 빌드): **PASS(FAIL 0, WARN 0)** — 바뀐 구조체(`BattleStruct` 비트·`struct LostItem` 1비트)는 배틀 중 heap 데이터라 세이브와 무관.
  - 한글 회귀(`kor-spost1171`): **555/652, `PASS (기대와 같음)`, 상태 줄 차이 0**.
- **ROM**(커밋 `4c447b25`): SHA1 **`c5132aa0808210d23b4ab52d4cc2e3d13adfe080`**, 사용 32,978,292 B(친구 답 반영 ROM 대비 +320), 파일 33,554,432 B, 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **mGBA 실기: 미실행(친구)** — 4절.

## 4. 친구 mGBA 확인 항목

1. **#10866 2대1 HP 박스** — 디버그 메뉴(필드에서 R을 누른 채 START) → `Trainers…` → `Trainer 1:`에서 1마리 트레이너(예: `TRAINER_JOEY_HNS` 오성, 꼬렛 Lv4) → `Double Battle: ON` → `Try Battle`. 플레이어 포켓몬은 2마리 이상. **기대:** 상대 HP 박스가 싱글 모양으로 깨지지 않음(이름·레벨·HP 바·성별), 오른쪽 위 빈 칸에 쓰레기 타일 없음, HP가 줄어도·가방/포켓몬 화면을 열고 돌아와도 같음. 옵션의 배틀 UI(Gen3/Gen4) 둘 다. 비교: 쌍둥이(루비&루미) 일반 더블배틀 HP 박스 2개가 이전과 같은지.
2. **#10974 구슬** — 화염구슬(또는 맹독구슬)을 지닌 포켓몬으로 신비의부적을 쓰고 턴 끝. **기대:** 도구 팝업 → `…은(는) 화염구슬 때문에 화상을 입었다!`(이전엔 아무 일 없음). 더블에서 플라워베일 파트너 + 풀 타입 구슬 보유자도 같음. 불꽃 타입·수의베일(화염구슬), 독/강철 타입·면역(맹독구슬)은 여전히 안 걸림.
3. **#10972 난기류** — 델타스트림이 있을 때(메가레쿠쟈, 디버그로만 닿음) 상대가 드래곤 기술 → **`수수께끼의 난기류가 공격을 약하게 만들었다!` 안 나오고 효과가 굉장함**, 바위·얼음·전기 기술 → 문장이 나옴(이전과 같음).
4. **#10878 훔친 야생 도구** — 도구를 지닌 야생 포켓몬에게 도둑질/탐내다 → ① 몬스터볼로 잡기: 잡은 포켓몬이 그 도구를 지님, 가방에는 하나도 늘지 않음 ② 다른 배틀에서 훔친 뒤 도망/쓰러뜨림: 배틀이 끝난 뒤 가방에 1개. 훔칠 때 문장은 이전과 같음.
5. (같은 날 친구 답) 매지컬리프 잎 이동·타격 타이밍, 디버그 메뉴 `Expansion version` = `v1.17.1` + `Development Build` — `full-sync-completion-1.17.1.md` 6절.

## 5. 남은 위험·미해결

- **#10866:** 화면은 헤드리스 테스트로 못 봄 — 실기 전까지는 코드 대조 근거만. 상대 쪽 싱글 그림을 올리면 `OPPONENT1` 태그가 0x1000을 쓰고 `OPPONENT2`(0x800)는 그 뒤에 붙음(upstream과 같은 배치) — 스프라이트 타일 여유가 모자라면 그림이 안 나올 수 있으나, 플레이어 쪽 같은 경우와 같은 양이라 문제를 예상하지 않음.
- **#10878:** 가방이 가득 차 있으면 배틀 끝의 `AddBagItem`이 실패해 그 도구가 사라짐(이전에도 훔칠 때 가득 차 있으면 같음). 같은 야생 포켓몬에게 트릭으로 준 뒤 다시 훔치는 등 얽힌 경우는 upstream처럼 "배틀 시작 때 원래 도구" 기준.
- **#10972·#10974:** 출력이 바뀜 → `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 2행.
- **#10953·#10933:** upcoming(1.17.2 개발판)에서 생긴 회귀라 1.17.1 기반에는 없음 — 나중에 upcoming 쪽을 받을 때는 그 수정도 같이 받아야 함.
