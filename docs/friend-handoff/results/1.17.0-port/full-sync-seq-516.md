# full-sync 묶음 81 — seq 516 (#9986 pret 병합 2026-05-15)

- 요청: 사용자 "ㄱㄱ 516 시작해 혼자해도 되는거라면"(2026-10-29).
- upstream: 읽기 전용 사본, #9986 병합 커밋 `3ea7f0d71b`(1.15.3, 첫 부모 `82dc0eb576` 대비 109파일 +543/−582). 단위 `U-waitstate-9986`(다음 #10061 = seq 517, HnS 스크립트 정리).
- 혼자 진행한 근거: 스크립트 대기 방식이 바뀌는 큰 병합이지만, 이식 전·후 이벤트 스크립트를 라벨 단위로 바이트 비교해 **이식 전 동작을 그대로 지키는 것**을 기계적으로 확인할 수 있음(아래 새 도구). 판단이 필요한 차이는 3곳뿐이었고 모두 이식 전 동작 유지 쪽으로.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 516 | #9986 | `376139af` | 아래 목록 | 32,980,836(−32) |

이식 전(`1cd6f5e3`, 게임 코드 = `9bd02555`) SHA1 `eb35fac1…` → 이식 후 SHA1 **`35adc84a4085c2007a2abaeb30ab370ae47398dd`**.

## upstream 내용

- **pret#2200 암묵 `waitstate`:** `data/specials.inc`의 `def_special`에 `waitstate=1` 표시(98개). `special`/`specialvar` 매크로가 그런 special 뒤에 `waitstate`를 자동으로 붙이고, 바로 뒤에 명시 `waitstate`가 또 있으면 어셈블러 경고를 내고 하나는 버림(이중 대기 방지). 바닐라 스크립트·매크로(`checkspecies`, 교환, 배틀 프런티어 등)의 명시 `waitstate` 삭제.
- **빌드 도구:** preproc 줄 표시(`# 줄 "파일"`) 처리, scaninc INCGFX 속도 개선.
- **이름 정리:** 체육관 관장 매치콜 문장 레이블 32개(`Roxanne1` → `Roxanne_Preparing` 등)와 `REMATCH_CALL_START`(0xFFFE), 배틀 애니 매크로 `create_narrow_foot_sprite`·`create_electric_puff_sprite`, 크기 비교 상수 `COMPARE_SIZE_*`(새 `include/constants/pokemon_size_record.h`).
- **파일 분리:** `GiveGiftRibbonToParty`를 `src/give_gift_ribbon_to_party.c`(+ 헤더)로.
- **동작 변경 1개 — 로머 BUGFIX:** `BUGFIX`가 켜져 있으면(HnS 켬) 배틀이 무승부(둘 다 기절)로 끝날 때도 로머를 비활성화. 이전 HnS(BUGFIX 판)는 무승부를 빠뜨려 기절한 로머가 계속 돌아다닐 수 있었음 — 바닐라·1.17.1과 같은 결과로.
- **같은 계산 다시 쓰기:** 배틀 애니 3함수(`AnimTrickBag`·`AnimLockOnMoveTarget`·`AnimWavyMusicNotes_Step`), `DynamicPlaceholderTextUtil_Reset`, `RecordedBattle_GetBattlerAction`(`0xFF` → `B_ACTION_NONE`), `BattleAction`에 `B_ACTION_UNK_15`.

## HnS 판단

- **충돌 12파일**:
  - `data/battle_anim_scripts.s` 39곳 + `asm/macros/battle_anim_script.inc`: HnS는 seq 496·497(#9676·#9924)에서 같은 자리를 expansion 매크로로 이미 바꿔 둠(upstream은 pret 쪽 변환과 겹침). 1.17.1도 expansion 형식이라 HnS 줄 유지, `create_tall_foot_sprite` → `create_narrow_foot_sprite` 이름만, HnS가 앞당겨 둔 `create_electric_puff_sprite` 정의는 upstream 자리로(매크로 파일이 1.17.1과 같아짐). **`battle_anim_scripts.o` 이식 전과 바이트 같음.**
  - 스크립트 5파일(배틀 타워 매크로·기술 떠올리기·모텔·PC·이벤트 스크립트): HnS 줄 유지(HnS `callnative BattleSetup_StartMultiBattle` + `waitstate`, HnS 가짜 PC 등). 명시 `waitstate`는 남겨 둠 — 바로 앞이 암묵 대기면 어셈블러가 버리므로 바이트 같음. 지우는 건 seq 517(upstream 정리 스크립트).
  - `src/pokenav_match_call_data.c` 8곳·`data/text/match_call.inc`: HnS는 표를 구조체 안에 바로 쓰는 형식(1.17.1과 같음)과 HnS 전용 블록(`#if IS_HNS`)을 가짐 → HnS 파일에서 출발해 upstream 레이블 이름·`REMATCH_CALL_START`·주석과 정렬을 입힘. 1.17.1 파일의 모든 줄이 들어 있음(HnS 블록만 더). 이름 바뀐 문장 32개 바이트 같음.
  - `tools/preproc/c_file.cpp` 4곳: HnS는 출력 버퍼(멤버 `putchar`, COMPOUND_STRING 합치기)가 있음 → HnS 버퍼 + upstream 줄 표시 처리. **결과가 1.17.1과 바이트 같음**(`c_file.h`는 멤버 순서 1줄만 다름).
  - `src/pokemon_size_record.c`: HnS 함수(`gParties`·`NUM_GIFT_RIBBONS`)를 지우고 새 파일은 1.17.1판(같은 본문).
  - `include/battle.h`: HnS `enum BattleAction`(1.17.1 형식) 유지 + `B_ACTION_UNK_15`.
- **새 이중 대기·대기 추가 3곳 — 이식 전 동작으로(HnS 신오 유적 아르세우스 방):**
  - 이식 전·후 `event_scripts.o`를 라벨별로 비교(새 도구 `dev_scripts/hns_verify/port/scriptcmp.py`, 같은 섹션 포인터는 "라벨+오프셋"으로 바꿔 배치 이동 무시)하니 대기 명령이 새로 생긴 곳이 HnS 스크립트 3곳뿐.
  - `special BattleSetup_StartLegendaryBattle` → `playbgm MUS_HG_VS_ARCEUS` → `waitstate`(1차전·2차전): 새 매크로대로면 special 바로 뒤 자동 대기 + 뒤의 명시 대기 = **배틀 뒤 스크립트가 영원히 멈춤(소프트락)**. 순서를 바꾸면 아르세우스 전용 곡(배틀 설정의 기본 전설 곡을 바로 덮어씀)이 배틀 뒤로 밀림.
  - `special FadeOutOrbEffect`(뒤에 대기 없음): 자동 대기가 생기면 구슬 빛이 다 사라질 때까지 날씨·천둥 연출이 멈춰 연출 타이밍이 바뀜(멈추지는 않음 — 페이드 끝에 스크립트를 다시 켬).
  - → 세 곳을 `callnative`로(자동 대기 없음, `// HnS:` 주석). `special`과 `callnative`는 같은 함수를 부르고 효과 요청 처리도 같음(`ScrCmd_special`·`ScrCmd_callnative` 비교). HnS가 이미 `callnative BattleSetup_StartMultiBattle`로 쓰는 방식.
- **스크립트 비교 최종 결과(라벨 30,863개):** 다른 라벨 5개 = 위 3곳(`special` 3바이트 → `callnative` 5바이트, 나머지 바이트 같음) + 정렬 패딩 1곳(`Alola_Pokemart_EventScript_Clerk`) + 새 내부 심볼 `_last_implicit_waitstate`가 라벨을 가른 1곳(`Alola_Ulaula_BattleVsTapuBulu`, 이어 붙이면 같음). upstream이 바꾼 바닐라 스크립트(크기 비교 상수 등)는 바이트 차이 0.
- **어셈블러 경고 125개:** "explicit waitstate follows implicit waitstate, ignoring" — 위 HnS 명시 대기가 버려진 자리. seq 517에서 upstream 정리 스크립트로 지움(그때 0개가 목표). 컴파일 경고 검사(`warncheck.sh`)는 이 형식을 세지 않음.
- 세이브: 영향 없음(구조체·저장 위치 변경 없음).

## 검증

- **빌드:** `make hns -j8` 종료 0. 저장소 전체 재빌드(`build/hns` 지우고): 종료 0, SHA1 `35adc84a…`(첫 빌드와 같음), 새 컴파일 경고 0(`warncheck.sh`). 어셈블러 경고 125개(위).
- **스크립트 바이트 비교:** 위 "스크립트 비교 최종 결과". 그 밖의 `build/hns/data/*.o` 14개는 파일째로 같거나(13) 섹션 바이트·재배치가 같음(`mystery_gift.o`).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `376139af`): 목록이 `test-baseline-seq511.txt`와 **바이트 같음**(PASS 3,010, 새 PASS 0, 사라진 PASS 0).
- **한글 테스트:** 새로 쓰지 않음(문장 0, 스크립트는 바이트 비교로 확인).
- **세이브 정적 비교**(이전 = 클론 `1cd6f5e3` 빌드): **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s516 8`): **555/652, `판정: PASS (기대와 같음)`, 상태 줄 차이 0**(요약 파일은 실패 목록 순서만 다름).
- **ROM**(커밋 `376139af`): SHA1 `35adc84a4085c2007a2abaeb30ab370ae47398dd`, 사용 32,980,836 B(묶음 78 대비 −32), 파일 33,554,432 B, 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기 후보(친구):**
  1. 신오 유적 아르세우스 방: 피리 연출(구슬 빛 → 날씨·천둥 → 아르세우스 등장)이 이전과 같은 흐름인지, 아르세우스 1차전·2차전이 아르세우스 전용 곡으로 시작하고 **배틀 뒤 스크립트가 이어지는지**(멈추면 소프트락).
  2. 아무 특수 화면(PC·교환·상점·기술 떠올리기·배틀 타워 등)이 이전처럼 열리고 닫히는지 — 바이트 비교로 같음을 확인했지만 범위가 넓어 눈에 띄는 멈춤이 있으면 알려 달라고.
  3. (드묾) 로머와 무승부(자폭·길동무로 둘 다 기절) 뒤 그 로머가 사라지는지.

## 다음

- seq 517 #10061(upstream 정리 스크립트로 HnS 명시 `waitstate` 125개 삭제, `callnative waitstate=`·`def_special` 인자 순서, preproc 위치 출력, HnS 전용 special 14개 `waitstate=1` 후보 — 스크립트 바이트 비교로 같음 확인). 목표 1.17.1.
