# 현재 HNS 작업 인수인계

2026-09-26 기준으로 친구가 이 저장소의 현재 상태를 파악하고 작업을 시작하기 위한 요약이다. 세부 변경 이력은 이 문서 대신 연결된 원본 기록을 기준으로 한다.

## 저장소와 브랜치

- 작업 브랜치: `pokehns-expansion-kor`
- 개인 Fork 원격: `origin` → `https://github.com/gorunit1/pokehns-expansion-kor.git`
- HNS 원본 원격: `upstream` → `https://github.com/PokemonHnS-Development/pokehns-expansion`
- 이 문서 작성 시점의 현재 커밋: `791876da59` (`Add 1.17.0 update handoff`)
- 이후 작업(2026-09-26, 빌드 복구·1.17.0 인벤토리·묶음 A 35개 PR 이식)은 [`HANDBACK_2026-09-26.md`](HANDBACK_2026-09-26.md)를 먼저 본다.
- 위 커밋은 `origin/pokehns-expansion-kor`에 반영되어 있다. 이 브랜치에서 생성했던 upstream Pull Request #36은 닫혔으며 병합되지 않았다.

작업을 시작할 때에는 먼저 `git status --short --branch`를 실행한다. 작업 트리에 변경이 있으면 그것은 이전 작업 또는 사용자의 변경일 수 있으므로 되돌리거나 일괄 추가하지 않는다.

## 반드시 읽을 문서

1. [`AGENTS.md`](../../AGENTS.md) — 저장소 공통 작업 규칙
2. [`docs/localization/STATUS.md`](../localization/STATUS.md) — 현재 상태·남은 검증
3. [`docs/localization/WORKFLOW.md`](../localization/WORKFLOW.md) — 작업·검증·인수인계 절차
4. [`docs/localization/SESSION_LOG.md`](../localization/SESSION_LOG.md) — 날짜별 상세 이력
5. [`docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`](../localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md) — 사용자 요청으로 바뀐 배틀 메시지 출력 동작
6. [`docs/localization/BATTLE_MESSAGE_KR_COMPARE.md`](../localization/BATTLE_MESSAGE_KR_COMPARE.md) — 한글 배틀 메시지와 `{B_...}` 지정 코드 대조 기록
7. [`POKEEMERALD_EXPANSION_1.17.0_UPDATE.md`](POKEEMERALD_EXPANSION_1.17.0_UPDATE.md) — upstream 1.17.0 안전 이식 작업 지시서

## 보존 우선순위

- 현재 한글화된 문자열 본문·줄바꿈·제어 코드·`{B_...}` 지정 코드·조사 토큰을 임의로 바꾸지 않는다.
- HNS 전용 맵·이벤트·그래픽·자산과 한글화용 추가 파일을 upstream 파일로 덮어쓰지 않는다.
- `src/battle_message.c`, `data/battle_scripts_1.s`, `data/battle_scripts_2.s`, `src/battle_util.c`, `src/battle_script_commands.c`, `src/battle_hold_effects.c`, `src/battle_end_turn.c` 및 연결 헤더·테스트는 특히 주의한다.
- 기술·특성·도구별 메시지, 특성 팝업 순서, 상태별 회복, 텔레포트 도주, 방벽 제거 등의 HNS 최신화 변경은 `BATTLE_MESSAGE_OUTPUT_CHANGES.md`와 실제 코드를 함께 보고 보존한다.
- 충돌이나 의도가 불명확한 차이는 적용하지 말고 upstream 근거, 파일·함수·라인, HNS 쪽 동작, 선택지를 문서화해 사용자에게 먼저 보고한다.

## 현재 확인된 기능 상태

### 포켓몬피리

- `ITEM_POKE_FLUTE`에는 배틀 사용 효과 `EFFECT_ITEM_USE_POKE_FLUTE`가 연결되어 있다. 가방에 이미 들어 있다면 배틀 중 사용 가능하며, 잠든 포켓몬이 있으면 깨우는 스크립트가 실행된다.
- 그러나 **일반 HNS 플레이에서 이 아이템을 획득하는 활성 이벤트는 없다.** `LavenderTown_VolunteerPokemonHouse_Frlg`의 FRLG 호환 스크립트에는 Mr. Fuji가 포켓몬피리를 주는 코드가 남아 있지만, 이 맵은 HNS의 `data/maps/headers.inc` 및 `data/maps/groups.inc`에 등록되어 있지 않다. 또한 HNS의 `FLAG_GOT_POKE_FLUTE`는 `0`이다.
- HNS의 실제 관동 스토리는 아이템이 아니라 포케기어 라디오를 사용한다. 블루시티 체육관에서 기계 부품을 찾아 발전소에 돌려준 뒤, 보라타운 라디오타워의 국장에게 말을 걸어 확장 카드를 받고 `FLAG_KANTO_RADIO_GOT`을 세운다. 관동에서 라디오의 포켓몬피리 채널을 맞춘 상태로 갈색시티의 잠만보를 조사하면 잠만보 전투가 시작된다.
- 따라서 포켓몬피리의 배틀 사용 자체를 플레이에서 시험하려면 별도의 의도적인 지급 이벤트를 구현하거나, 디버그·세이브 편집 등으로 아이템을 넣어야 한다. 이 문서 작성에서는 지급 코드를 추가하지 않았다.

### 테라스탈

- 테라스탈 엔진과 `테라스탈오브` 아이템은 소스에 존재한다.
- 일반 플레이어 배틀에서는 `B_FLAG_TERA_ORB_CHARGED`와 `B_FLAG_TERA_ORB_NO_COST`가 모두 `0`이라 테라스탈 사용 조건을 충족하지 못한다. 테라스탈오브만 추가해도 사용할 수 없다.
- 테스트 코드의 테라스탈 허용은 테스트 환경이 아이템·플래그 일부를 우회하기 때문이며, 일반 HNS 플레이 가능 여부의 근거가 아니다.
- 테라스탈을 활성화하려면 사용하지 않는 실제 플래그를 두 설정에 배정하고, 테라스탈오브 지급·충전·재충전 이벤트와 플레이어 UI를 함께 검증해야 한다. 이 문서 작성에서는 설정을 바꾸지 않았다.

## 최근 중요한 변경과 미검증 항목

- 배리어프리는 특성 팝업 뒤 리플렉터 → 빛의장막 → 오로라베일 순으로 실제 제거된 방벽만 출력한다.
- 깨뜨리다·사이코팽·레이징불도 같은 방벽 순서로 출력한다.
- 정화, 상태 회복 특성, 수면 방지 특성, 마그마의무장, 습기의 유폭 차단, 끈적끈적바늘 전이, 8세대 이후 텔레포트 도주는 최신 출력 규칙에 맞추기 위해 조정됐다.
- 이 변경들은 대상 빌드와 당시 전체 `make hns -j8` 성공 기록이 있으나, 특성 팝업·문구 순서 등 많은 항목의 실제 HNS 화면 검증은 남아 있다. 정확한 검증 상태는 `STATUS.md`와 `SESSION_LOG.md`의 최신 항목을 확인한다.

## 작업과 검증

- HNS 전체 빌드는 `make hns -j8`이다. 기본 `make` 성공은 HNS 검증으로 기록하지 않는다.
- 코드 대조, 대상 오브젝트 빌드, 전체 빌드, 자동 테스트, 실제 게임 화면 검증을 구분해서 기록한다.
- 작업 단위가 끝나거나 중단되면 `STATUS.md`와 `SESSION_LOG.md`를 갱신한다.
- 사용자가 별도로 요청하지 않는 한 원격 push, Pull Request 생성, upstream 병합을 하지 않는다.
