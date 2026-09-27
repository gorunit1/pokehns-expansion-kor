# g3 — 배틀 변경·기술·특성·도구: 1.17.0 전체 엔진 동기화 판정 재확정

- 입력: `g3_battle_changes_moves_abilities_items.tsv`(88 PR). 결과: `g3_battle_changes_moves_abilities_items_plan.tsv`(88행).
- 기준: 스냅샷 `e95679c545`(묶음 A 35건, `-ffunction-sections` 반영), upstream `expansion/1.17.0`(`e8bd1cd7b0`), merge-base `3efb836f72`.
- 판정 기준: 새 지시서(`POKEEMERALD_EXPANSION_1.17.0_FULL_ENGINE_SYNC.md`)의 4개 판정만 쓴다. 어제 판정(`g3_…_report.md`)은 출발점으로만 썼다.
- 조사 방법:
  - PR마다 upstream diff와 HnS 코드를 직접 대조했다.
  - `git blame <PR>^`로 hunk 문맥을 만든 upstream 전용 커밋을 찾아 `deps`를 정했다(스크립트: 스크래치패드 `g3/deps.py`).
  - 추가 줄 존재 여부는 휴리스틱으로 확인했다(`g3/pres.py`).
  - `git apply --check`는 입력 표의 `fwd_now`/`rev_now`를 쓰고, #10416만 따로 확인했다.
- 한계: 빌드·테스트·실기 검증은 하지 않았다. 라인 번호는 스냅샷 기준이다.

## 1. 판정별 개수 (합계 88)

| 판정 | 개수 | PR |
|---|---:|---|
| 이식 | 70 | 아래 외 전부 |
| 동등 | 14 | #9678, #9740, #9769, #10189, #10249, #10256, #10257, #10324, #10423, #10541, #10576, #10591, #10592, #10713 |
| 무관 | 1 | #9511(마이그레이션 스크립트 전용) |
| 외부결정 | 3 | #8943, #10151, #10454 |

### 어제 판정에서 바뀐 것

- **어제 "이미 적용"이었지만 이번에 이식(잔여분)으로 바꾼 것**
  - **#9735**: `CalcDefenseStat`의 모래바람 바위 특방·설경 얼음 방어 보정이 공격자 날씨(`GetAttackerWeather`)가 아니라 방어측 기준으로 남아 있다(`src/battle_util.c:7483·7486`). 그래서 메가솔 공격자를 상대로 1.17.0과 결과가 다르다.
  - **#10416**: HnS는 날개쉬기·달빛·알낳기·아침햇살 애니에서 `HealingEffect`를 뺐다. 그런데 회복 경로(`BattleScript_PresentHealTarget`, `data/battle_scripts_1.s:3495`)에 `B_ANIM_SIMPLE_HEAL`을 넣지 않았다. 그 결과 회복 애니 없이 HP만 오른다(코드 추론, 현재 HnS 버그).
- **새 기준에 따라 이식으로 바꾼 것**
  - upcoming 회귀 수정: #9474, #9564, #9595, #10345, #10589, #10265, #10634.
  - 리팩터 후속: #9928, #10057, #9989, #10666.
  - 테스트: #10590, #10310, #9265.
  - 부분 적용 잔여: #10145, #9777, #10025.
  - 과거 충돌: #9514, #9714, #9939, #10593, #9168, #8893.
- **#10576**: 1.17.0 안의 #10588(g2)이 정확히 역으로 되돌린다(diff 역관계 확인). 그래서 남는 효과가 없어 `동등`으로 두었다.
- **#10713**: HnS `HandleInputChooseMove`의 인라인 분기가 1.17.0 `GetDefaultSelectionTarget`과 결과가 같아 `동등`으로 두었다.

## 2. 대형 리팩터 unit과 권장 이식 순서

괄호 안은 다른 그룹 PR이다. 화살표는 먼저 넣을 순서다.

### A. 애니 스크립트 체인

- 순서: #9063 → **U-animcall-9142**(#9142·#9473·#9564) → **U-anim-8497**((#8497 g6)·#9595·#10345·#10589, #9511은 무관·도구 참고) → **U-9172**(#9172) → (#9677 g5) → U-9676(#9676) → **U-animcommon-9924**(#9924) → U-10374(#10374)
- 체인 뒤에 넣을 것: U-10401, U-10421, U-10027(#8497 뒤).
- 체인과 독립: #10262, #10361, #10189(동등).
- #9172는 4399줄 기계 치환이다. patch보다 변환 스크립트를 HnS 고유 애니까지 돌리고 조립 결과가 같은지 비교하는 편이 안전하다.

### B. 배틀 코어 체인

1. 기반: (#9116 g4), (#9176·#9446·#9610 g6), (#9249·#9494 g2), (#9532 g1), #9417
2. **U-9514**(#9514 SetMoveEffect cleanup; 선행 #9176·#9249·#9446)
3. **U-calcvalues-9657**(#9657; 선행 #9116·#9176·#9249·#9417·#9494·#9532·#9610) → (#9859 g2)
4. (#9655 g6 Battle Messages) · (#9730 Stat Change) → **U-statchange-9730** 후속(#9928·#10057·#9989·#10666·#10265, #10428의 브레이브차지 hunk)
5. **U-accuracy-9939**(#9939; 선행 #9655·#9657·#9730·#9859·#9494·#9610·#9176·#9514) → #10595(추가 선행: (#10362·#10220 g2), (#10426 g6)) → #10640의 MISS_TYPE 삭제 hunk
6. (#10426 g6) → **U-setmoveeffect-10593**(#10593) → #10630의 Psychic Noise hunk
7. (#9784 g1) → **U-eject-9784** #9832 → #9898의 Opportunist hunk
8. **U-champmsg-9777**(#9777 잔여; 선행 #9655·#9730·#9859·#9939) → **U-champions-10151**(#10151, 외부결정; 선행 #9657·#9859·#9514·#9777·#9616)
9. **U-12v12-8943**(#8943, 외부결정; 선행 (#9507 g4)·(#9451 g4)·(#9494 g2), 후속 (#10415·#10536·#10568→#10674·#10711))
10. **U-weather-10170**: #10170 + (#10214 g4) → #10395 → #10454(외부결정)
11. **U-absent-10459**: #10459 + #10634

### C. 독립 소형 단위

체인과 무관하게 먼저 넣을 수 있는 것:

- 동작 수정: #10016, #10428(브레이브차지 제외), #10262, #10401(값만), #10104, #10109, #10225, #9616, #9429, #9525, #10297
- 힙 절감: **U-heap-9121**(#9121+#9474, 반드시 한 커밋)
- 데이터·그래픽: #10011, #10129
- 잔여 이식: #9735, #10416

**#8893**은 #10011·#10428·#9429의 설명 hunk와 (#9667 g5) 뒤에 넣는다.

## 3. 외부결정 상세

### #8943 12v12 capability (`70340c1135`)

- **파일·함수**
  - 파티 구조: `include/pokemon.h`의 `gPlayerParty`/`gEnemyParty`를 `gParties` 매크로로 바꾼다. HnS 현재 정의는 `src/pokemon.c:116-118`이다.
  - 녹화 세이브: `include/recorded_battle.h`의 `RecordedBattleSave`, `src/recorded_battle.c:262` `IsRecordedBattleSaveValid`(체크섬 검사).
  - 트레이너 데이터: `tools/trainerproc/main.c`, `struct Trainer`.
  - 멀티전: `data/maps/RocketHideout_B2F_hns/scripts.inc:310`(`multi_2_vs_2`, 파트너 `PARTNER_LANCE_HNS`), 배틀타워 멀티룸 `_hns`.
- **HnS 현재 동작**
  - 파티는 2개(플레이어·상대)다.
  - 멀티전은 편 당 3마리씩 나눈다.
  - 녹화 배틀은 옛 형식이다(파티 2개, `BATTLER_RECORD_SIZE` 664).
- **선택지 A: upstream 형식 그대로**
  - 녹화 형식이 `parties[4][6]`, 비트필드, `BATTLER_RECORD_SIZE` 388로 바뀐다.
  - 기존 녹화는 체크섬 불일치로 무효가 된다. 필요하면 `SAVE_VERSION`에 연동해 녹화 섹터를 초기화한다.
  - 멀티전 편성은 두 방법 중 하나로 유지한다.
    - HnS 멀티 트레이너에 `Multi Party: Half`를 지정한다.
    - 또는 `B_MULTI_HALF_TEAMS TRUE`로 둔다.
  - 영향: 기존 녹화가 사라진다. 본 세이브(SaveBlock1 파티)는 그대로다.
- **선택지 B: 녹화 형식은 옛 2파티 배치로 유지하도록 적응**
  - 기존 녹화가 보존된다.
  - 멀티 기록을 변환하는 코드가 추가로 필요하다(작업량 증가).
- **공통 영향**
  - EWRAM 정적 약 +1,200B(현재 여유 약 13KB).
  - `trainers_hns.party` 재생성이 필요하다.
  - 한글 `sText_Link*`는 토큰만 바꾼다.

### #10151 Champions 배틀 메커니즘 v1.0.X (`b3a3c16abf`)

- **파일·함수**
  - `include/config/battle.h`에 새 config가 들어온다: `B_PARALYSIS_CHANCE`, `B_FREEZE_TURNS`, `B_MEGA_EVO_SPEED_SWAP`, `B_FIRST_TURN_MOVE`, `B_SALT_CURE_DAMAGE`, `B_*_SELECTABLE`, `B_MOVES_THAT_REMOVE_TYPE`, `B_ENCORE_PRIORITY`, `B_FAINT_MOVE_EFFECT_TIMING`, `B_SHEER_FORCE_AGAINST_ABILITIES`, `B_UNSEEN_FIST_PIERCING_DRILL`.
  - 캔슬러(`src/battle_move_resolution.c`)와 선택 제한(`MOVE_LIMITATION_UNUSABLE`)이 바뀐다.
  - HnS 설정: `include/config/general.h:74` `GEN_LATEST=GEN_CHAMPIONS`.
- **HnS 현재 동작**
  - Champions 수면 턴만 들어가 있다(`battle_script_commands.c:2358`, `battle_end_turn.c:902`).
  - SESSION_LOG.md:1980(2026-09-15)은 "대응 가능한 Champions 동작만 적용"이라고 기록했다.
- **선택지 A: 새 config를 GEN_LATEST 그대로 둔다**
  - 1.17.0 기본값이고 HnS의 GEN_LATEST 규약과도 맞는다.
  - Champions 규칙이 켜진다:
    - 마비 불발 12.5%
    - 얼음 3턴 상한
    - 속이다·만나자마자 첫 턴 이후 선택 불가
    - 트림·볼부풀리기·토해내다·라스트리조트 선택 제한
    - 소금절이 피해 절반
  - 결과적으로 전투 난이도가 바뀐다.
- **선택지 B: 새 Champions config만 GEN_9로 고정한다**
  - 현재 밸런스가 유지된다.
  - 대신 HnS의 GEN_LATEST 규약·1.17.0 기본값과 어긋난다.
- **공통**
  - `STRINGID_CANTUSEMOVE`·`STRINGID_BELCHCANTUSE`에 새 한글 문구가 필요하다(공식 한국어 확인 전 미결).
  - `STRINGID_BELCHCANTSELECT` 한글은 유지한다.

### #10454 Gen 9 필드 날씨 우선 (`e30b117d69`)

- **파일·함수**
  - `src/battle_util.c:2113` `TryChangeBattleWeather`
  - 날씨 특성 분기: `src/battle_util.c:3388~`
  - `FIELD_EFFECT_OVERWORLD_WEATHER`: `src/battle_util.c:2991`
  - 새 config `B_OVERWORLD_WEATHER_OVERRIDE`, 새 스크립트 `BattleScript_BlockedByOverworldWeather`.
- **HnS 현재 동작**: 날씨 특성·기술이 필드 날씨를 덮어쓴다.
- **선택지 A: GEN_LATEST**
  - 필드 날씨가 있는 배틀에서는 날씨 특성·기술이 실패한다.
  - 출력은 특성 팝업 뒤 기존 한글 `STRINGID_BUTITFAILED`("하지만 실패했다!")다.
  - HnS 날씨 맵에서의 공략과 메시지가 바뀐다.
- **선택지 B: GEN_8로 고정**
  - 현재 동작을 유지한다.
  - 코드는 이식되지만 해당 경로는 쓰이지 않는다.

## 4. 한글 메시지와 겹치는 항목의 적응 방법

- **토큰만 교체, 한글 본문 유지**
  - #9514: 약 30개. `{B_DEF_*}`→`{B_EFF_*}`, `{B_ATK_*}`→`{B_SCR_*}`.
  - #9616: `STRINGID_TARGETWOKEUP`.
  - #9916: `STRINGID_FRISKACTIVATES`.
  - #9714: `STRINGID_PKMNSUBSTITUTEFADED`.
  - #8943: `sText_Link*`.
  - HnS에 `B_EFF_TEAM1/2` 토큰이 있다(`charmap.txt:427`). 조사 토큰은 직전 출력 글자 기준이라 토큰을 바꿔도 안전하다.
- **HnS 메시지 최신화 경로를 보존하며 구조만 교체**
  - #9714: 방벽·흰안개·신비의부적·장판 해제 `*Return` 스크립트에 `saveattacker/copybyte/restoreattacker`를 넣는다.
  - #10593: HnS 분기를 각 `HandleSetEffect*`로 다시 옮긴다.
    - 오로라베일 성공 문구
    - 깨뜨리다 계열 방벽 순차 출력
    - Gen1 반동 챌린지
    - StealStats
  - #9514: 방벽 스크립트 통합 뒤에도 `gReflectLightScreenSafeguardStringIds` 선택값을 유지한다.
  - #9168: 열매 스크립트의 아이템 팝업 `call`과 리샘열매 상태별 루프를 유지한다.
  - #10471: `TryImmunityAbilityHealStatus` 혼란 회복 출력 ID를 유지한다.
  - #10595: HnS 출력 변경 효과 스크립트(정화·잠자기 등)의 실패 경로를 대조한다.
- **ID 개명·통합 대응**
  - #9939: upstream이 `PKMNEVADEDATTACK`/`PKMNAVOIDEDATTACK`를 개명하고 `BATTLERAVOIDEDATTACK`를 신설한다.
  - HnS 한글 3종(`ATTACKMISSED`·`EVADED`·`AVOIDED`)은 그대로 둔다. 어느 문구를 쓸지는 #9655(g6) 판정의 매핑을 따른다.
  - #9777: `AFTERMATHDMG`·`STICKYWEBDISAPPEAREDFROMYOU`는 HnS ID를 유지한다. 영문 38문구 개정 hunk는 적용하지 않는다.
- **새 한글 문구가 필요해 문구만 미결인 것**
  - #10395: 오리진펄스·하드론엔진 4문구
  - #9777: 보이지않는주먹 보호 관통
  - #9008: Victory Catch 3문구, `B_CATCH_OR_NOT` 창 폭 확인
  - #10151: `CANTUSEMOVE`·`BELCHCANTUSE`
- **출력 경로가 바뀌므로 `BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 기록할 것**
  - #9616: 소란 기상 문구가 턴 종료 `PKMNWOKEUPINUPROAR`에서 시전 직후 `TARGETWOKEUP`으로 바뀐다.
  - #9777: Z/다이맥스 "완전히 막지 못했다"가 피해 전에서 피해 뒤로 옮겨진다.
  - #9939: 범위기 빗나감 순서가 바뀐다.
  - #9916: 끈끈손 도둑 방지에 pause가 추가된다(문구 동일).

## 5. ROM·세이브 위험

- 현재 수치는 SESSION_LOG 2026-09-27 기준이다.
  - ROM 97.56%(여유 약 820KB)
  - EWRAM 249,000/262,144B(여유 약 13KB)
- **세이브 영향**: #8943 하나다.
  - `RecordedBattleSave` 형식이 바뀌어 기존 녹화가 무효가 된다.
  - SaveBlock1/2와 HnS `saveVersionMagic`은 영향이 없다.
- **EWRAM**
  - #8943: 약 +1.2KB
  - #9473: +13B
- **배틀 힙**
  - #9121+#9474: -16KB(이득). 반드시 둘을 한 번에 넣는다.
- **ROM 증가 추정(빌드 미측정)**
  - #8943 +4KB, #9008 +2.5KB, #10151 +2KB, #10225 +1.9KB
  - #9777 +1KB, #10025 +1KB, #8893 약 +1KB, #10593 +0.5KB, #9939 +0.5KB
- **ROM 감소 추정**
  - #9924 약 -2KB, #9142 약 -1KB, #10595·#9916·#10253·#10338 소폭
- **합계**: 수십 KB 이내로 보여 현재 여유로 충분하다고 추정한다.

## 6. 불확실 항목

1. **#10416 회복 애니 누락**: 코드 추론이다. 날개쉬기·달빛·알낳기·아침햇살을 실기에서 확인해야 한다.
2. **#9735 방어 보정 차이**: 메가솔 대 모래바람 바위 타입의 특방 차이를 계산 코드로만 확인했다.
3. **#10384**: `TYPE_MYSTERY` 가드 제거로 무속성 리베레이션댄스 등에 자속이 붙는다. 1.17.0 최종 코드도 같다. upstream 의도인지 확인하지 못했다.
4. **#10338**: 교체 등장 시 `moveResultFlags[battler]=0` 제거가 HnS 경로에서 부작용을 내는지 전 경로를 추적하지 못했다.
5. **#10459**: `FaintClearSetData` 타입 초기화 제거와 HnS 고유 기절 슬롯 참조의 관계를 확인하지 못했다.
6. **#9525**: 챌린지 "물리/특수 분리 끔" 모드에서 스텔라·??? 분류가 바뀐다. HnS 정책과 맞는지는 원작업자 확인이 필요하다(판정은 이식).
7. **#8893**
   - 과거 "한글 덮어쓰기 위험으로 미이식" 결정(SESSION_LOG.md:1994)을 ID 기준 치환으로 우회해 이식으로 판정했다.
   - 설명 길이가 요약 화면 창을 넘는지는 확인하지 않았다.
8. **#10129**: 새 트리거 PNG(8비트 인덱스)의 gbagfx 4bpp 변환과, yDiff -5/-2가 HnS 비Gen4 체력박스에 맞는지를 화면으로 확인해야 한다.
9. **#10025**: HnS 체력박스용 퍼센트 전용 그림은 새로 그려야 할 수 있다(기본 FALSE면 불필요).
10. **deps 한계**: 블레임 기반이라 문맥 선행과 기능 선행이 섞일 수 있다. 행마다 기능 선행만 `deps`에 두고 문맥 선행은 `notes`에 적었다.
11. **테스트**
    - 영문 `MESSAGE` 의존: #10590, #9265, #9616, #10595.
    - `make check`는 현재 `fake_rtc.h` 포함 순서 오류로 실행 전에 멈춘다.
12. **동등 판정의 테스트 잔여**: #10257, #10591, #10324, #9769, #10249, #10256, #9735는 테스트 파일이 이식되지 않았다. 테스트 동기화 단위에서 따로 다룬다.
