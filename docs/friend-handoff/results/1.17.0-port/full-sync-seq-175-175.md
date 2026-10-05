# full-sync 실제 port 결과 — seq 175 #8434 Overworld Encounters (+ seq 199 #10020 선반영)

완료: 순서표 seq 175(XL)를 단독 단위로 이식했다. 같은 unit의 #8434 회귀 수정 **seq 199 #10020을 바로 뒤로 선반영**했다(순서표와 다르게 진행한 것). 다음은 **seq 176 #9864**(S)부터다. 같은 unit 뒤 행: 185 #9910, 190 #9968(177 #9879 뒤), 208 #10066, 363 #10096, 369 #9966, 370 #10076.

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md)
시작 HEAD: `cbbd8a629d`. 작업 컴퓨터: 데스크탑(2026-10-05).

## 요약

| seq | PR | 판정 | 커밋 | ROM 변화 | 비고 |
|---|---|---|---|---:|---|
| 175 | #8434 Overworld Encounters | 적용(HnS 적응) | `9d0e2f51ee` | +21,440 B | 필드 포켓몬 조우(OWE) 시스템 이식, **`WE_OW_ENCOUNTERS FALSE`로 HnS 야생 조우 동작 유지.** upstream 그대로면 꺼 둔 상태에서도 달라지는 3곳을 HnS로 막음 |
| 199 | #10020 Re-add `DetermineFollowerNPCDirection` same coord check | 적용(선반영) | `2e148e3122` | +32 B | #8434 회귀 수정. HnS는 NPC follower가 꺼져 있어(`FNPC_ENABLE_NPC_FOLLOWERS FALSE`) 동작 영향 없음 |

- 빌드(최종 `2e148e3122`): 종료 코드 0, **ROM 32,738,308 B(+21,472 B, 97.57%) / EWRAM 250,132 B(0) / IWRAM 25,516 B(0)**, SHA1 `4d8ecf63872ff875531af235cf0c8f04a532223b`(메인 재빌드 같음, `build/localization-logs/hns-20261005-232717-seq175.log`). 새 컴파일 경고 0, 도구 경고는 허용한 `libpng warning: bKGD: invalid index` 1줄.
- 한글이 든 소스 줄 변경: 0(추가된 비ASCII 8줄은 upstream 영문 문서·주석의 `é`·`…`).
- 전체 테스트(`build/port-check-seq175.log`): PASSED 2,419 / TOTAL 5,322, **목록이 `test-baseline-seq174.txt`와 한 줄도 다르지 않다**(기준 목록 그대로 사용).
- 한글 회귀(저장소 밖 328개): 이식 전·후 요약 파일 전체가 같다(적용 담당).
- 세이브: 정적 비교를 seq 174 기준(`chunk-175/tmp-175-impact/savecompat-pre175`, `run --pre`)으로 하면 **PASS(FAIL 0, WARN 5)**. WARN 5는 `gSpeciesInfo` 종 크기 268 → 272 B로 바뀐 포켓몬 데이터 읽기 함수 4개(`GetBoxMonData3`·`CalculateMonStats`·`GetBoxMonGender`·`GetLevelFromBoxMonExp`, 표 간격만 다름)와 그 요약 1줄로 영향 분석 예측과 같다. 세이브 경로(`save.o`·`load_save.o`) 기계어는 D4로 같다. 묶음 1부터의 기본 비교도 이전 표시(FAIL 2)에 위 WARN만 더해졌다. **세이브 왕복 PASS**(이식 전 세이브를 새 ROM으로 불러와 다시 저장: 일반 세이브 섹터·새 게임 섹터 바이트 같음, 불러오기 결과 95줄 같음).

## 공통 사항

- 사전 분석(읽기 전용 2개, 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-175/`): `seq175-8434.md`(patch 작성·config FALSE 동일 증명), `seq175-8434-impact.md`(HnS 영향 독립 분석). 두 분석이 같은 문제 5건(아래 A~E)을 독립적으로 찾았다.
- 적용: 적용 담당 1개, patch 그대로 + 메인 결정 손 수정 1곳(D4). 지시 `chunk-175/APPLY.md`, 기록 `apply/PROGRESS.md`.
- 메인 결정
  - 분석 HnS 적응 전부 승인, ROM +21 KB 수용(아래).
  - D4: `LoadObjectEvents`(세이브 불러오기)에 생기는 `SetMinimumOWESpawnTimer()` 호출을 `if (WE_OW_ENCOUNTERS)`로 감싸 세이브 경로 기계어를 이식 전과 같게 유지(`// HnS:`).
  - upstream png(`shiny_sparkle.png`)의 도구 경고 `libpng warning: bKGD: invalid index` 1줄을 알려진 경고로 받음.
  - #10020을 seq 199에서 #8434 바로 뒤로 선반영.

## seq 175 #8434 (`9d0e2f51ee`)

- upstream `10ae7f9f54`(46파일 +3641/−181). 새 `src/wild_encounter_ow.c`(2,178줄)·`include/wild_encounter_ow.h`·`include/config/wild_encounter.h`·`data/scripts/wild_encounter.inc`, 이동 타입 6개와 이동 함수, 출현 필드 효과·이로치 반짝임 그림, 스크립트 매크로 4개, `struct SpeciesInfo`에 `overworldEncounterBehavior`, 기존 오브젝트·야생 조우·포켓몬 생성 코드 정리와 OWE 훅.
- config: `WE_OW_ENCOUNTERS FALSE`, `WE_VANILLA_RANDOM TRUE`, `OW_AMBIENT_CRIES_VANILLA` — 모두 upstream 기본값이 HnS 동작 유지값이다.
- **upstream 그대로면 꺼 둔 상태(FALSE)에서도 달라지는 것과 HnS 처리**(사전 분석 4.1절·영향 분석 1절)
  - (A) **시간대 색조 회귀(화면에 보임):** 매 프레임 `UpdateOverworldWildEncounter`가 `WE_OW_ENCOUNTERS` 검사 전에 `GetTimeOfDay()`를 불러, 야생 표가 있는 도로·동굴·물 맵에서 1분마다의 아침/낮/저녁/밤 색조 갱신이 사실상 멈추고 매 프레임 RTC를 읽는다. upstream #10343(seq 285)도 블렌드 쓰기만 고친다. → HnS: FALSE면 함수 첫 줄에서 return(`bx lr` 2바이트로 컴파일, 관련 변수는 링크에서 빠짐).
  - (B) **메모리 덮어쓰기 가능:** 오브젝트 16칸이 다 찼을 때 FALSE 경로가 초기화 안 된 번호로 `ClearObjectEvent`를 한다. HnS는 맵 오브젝트가 많고(최대 64개) 동반 포켓몬도 칸을 써서 실제로 닿는다. → HnS: `return TRUE`(이전처럼 "칸 없음"). #9910(seq 185)이 같은 곳을 고친다.
  - (C) **이로치 확률:** upstream `ComputePlayerShinyOdds`를 그대로 넣으면 HnS 챌린지 이로치 확률(`GetShinyOdds()`)과 `FLAG_SYS_POKEDEX_GET` 조건이 빠진다. → HnS 줄 유지.
  - (D) **이동 타입 번호 충돌:** HnS `MOVEMENT_TYPE_TOWER_BEAM` 0x53(스프라우트 탑·스즈의 탑·알프 유적 등, 세이브에 저장되는 값)과 upstream OWE 번호가 겹친다. → HnS 0x53 유지, OWE 6종 0x54~0x59, `NUM_MOVEMENT_TYPES` 0x5A.
  - (E) **배틀 시작 때 배열 밖 읽기:** 모든 배틀 시작에 `gObjectEvents[16]`을 읽는다(현재 배치에서는 무해). → HnS 범위 검사.
  - 그 밖에 소용돌이 우선순위, 동반 NPC 문 나가기 조건, 동반 포켓몬 스크립트 분기, 트레이너힐 분기 순서, 호연 사운드 조우 선언, `CountFreeSpriteTiles`, 피라미드 `IsSpeciesEnabled`, `IsMetatileDirectionallyImpassable` 가드 등 HnS 줄 유지.
- 무조건 실행되는 경로 27개를 하나씩 이식 전과 같은 결과인지 판정했다(사전 분석 4.2절). 동반 포켓몬은 칸 할당이 원래와 같고, 서핑 포켓몬은 오브젝트가 아니라 필드 효과 스프라이트라 ID 할당과 충돌하지 않는다. 새 화면 문자열·그림은 FALSE에서 나오지 않는다.
- **ROM +21,440 B:** `gSpeciesInfo`가 종마다 1바이트 필드 추가로 +6,296 B, 반짝임 그림 약 2 KB, 이동 타입 표가 참조해 링크에 남는 OWE 코드 약 10.5 KB 등. upstream 구조를 그대로 둔다. EWRAM·IWRAM 0.
- D4 확인: `load_save.o`의 `LoadObjectEvents` 기계어가 이식 전과 같다(objdump, 주소만 이동).
- 테스트: PR이 더하는 테스트 없음. 관련 14파일 119개 전후 같음(이식 전부터 FAIL인 `species.c` 포인터 값 사유 줄 하나만 ROM 배치로 바뀜).

## seq 199 #10020 (`2e148e3122`, 선반영)

- upstream `b3bc856ffe`. #8434가 지운 `DetermineFollowerNPCDirection`의 "같은 좌표면 DIR_NONE"을 되살린다. seq 176~198 사이에 `follower_npc.c`를 바꾸는 행이 없어 문맥이 같다. HnS 호출부는 NPC follower가 꺼져 있어 게임 동작 영향이 없다.

## 커밋 리뷰

리뷰 2개(읽기 전용, 스크래치 사본 빌드·임시 테스트). 결과: `/home/hjm0725/hns-sync-work/chunk-175/review-runtime/REVIEW-RESULT.md`, `review-code/REVIEW-RESULT.md`.

### 런타임(세이브·메모리·실행 시간): 두 커밋 문제 없음
- 세이브: 이동 타입 표(`sMovementTypeCallbacks`) 84 → 90칸, 0x00~0x53은 이식 전과 같은 함수(0x53 = `MovementType_TowerBeam`), 초기 방향·이동 범위 표도 0~0x53 바이트 같음. 맵 데이터의 `movement_type`은 전부 이름(숫자 0건), TOWER_BEAM은 21개 맵 40개 오브젝트 + `setobjectmovementtype` 2곳. `LoadObjectEvents` 기계어 같음(D4 유효). `SpeciesInfo`는 ROM 상수라 세이브와 무관.
- RAM: EWRAM·IWRAM 심볼 1,074개의 주소·크기가 완전히 같다(`gObjectEvents`·`gSaveblock1` 포함).
- ROM +21,472 B 내역: 새 코드 12,514 B, 새 데이터 2,767 B(반짝임 그림 2,048 B), `gSpeciesInfo` +6,296 B(구조체 끝 1바이트 필드 → 4바이트 정렬로 268 → 272 B × 1,574종).
- 실행 시간: `OverworldBasic`에 빈 함수(`bx lr`) 호출 한 줄만 늘었고 `GetTimeOfDay`·`UpdateTimeOfDay`·`RtcCalcLocalTime` 호출부는 전후 같다. 기존 함수의 OWE 훅 16곳 모두 RTC를 읽지 않는다.
- 테스트 러너 실측(사본 임시 테스트): 야생 생성·이로치 24조합(챌린지 이로치 설정 × 도감 플래그 × 빛나는부적), 성별 요청 22조합, 29·30·32·46번 도로 풀숲·물·낚시 조우(가짜 RTC), 방향 반전, NPC·동반 포켓몬 생성(빈 칸/16칸 참) — 이식 전후 같음. HnS 적응 (2)·(3)이 기계어·실측으로 동작.
- #10020: 호출부 전부가 `PlayerHasFollowerNPC()`(상수 0) 뒤라 도달하지 않는다.
- [정보] 16칸이 찬 상태에서 localId 249~252(생성 OWE 전용)를 만들면 배열 뒤 36 B를 덮어쓴다 — HnS는 이 localId를 쓰지 않고 FALSE라 도달하지 않음, #9910(seq 185)이 고친다. 화면 밖 제거 직전 `offScreen` 비트가 세이브 비활성 칸에 1로 남을 수 있으나 읽히지 않는다. `GetOppositeDirection`에 9 이상이 들어오면 "Invalid direction." 크래시 화면 — 호출부 18곳 입력은 모두 0~8(실기에서 보이면 이 변경을 먼저 의심).

### 코드(HnS 보존·동작 동일): 두 커밋 문제 없음
- upstream `10ae7f9f54`와 줄 단위 대조: 커밋이 지운 HnS 줄은 `NUM_MOVEMENT_TYPES 0x54`(번호 변경, 의도)와 `pokemon.c` 이로치 3줄(`ComputePlayerShinyOdds`로 그대로 옮겨짐)뿐, **그 밖에 사라진 HnS 줄 0.** 3-way 병합 결과와 비교해 33파일은 같고 13파일은 충돌 구간만 다르며 모두 맞게 들어갔다(소용돌이 블록 뒤 훅, #9991 걸음 조건, 트레이너힐 분기 순서, `CountFreeSpriteTiles`, 호연 사운드 선언, D4). png는 upstream과 sha1 같음.
- **config FALSE 동작 독립 검증(기계어):** 이식 전(pre), `SpeciesInfo` 끝에 u8 하나만 더한 사본(mid), 이식 후(post)를 빌드해 함수 섹션 단위로 비교. 구조체 크기 변화를 뺀 mid → post에서 바뀐 함수 88개 말고는 기계어가 같다(`load_save.o`, 서핑 포켓몬, 배회, 피라미드, 울음소리, `CheckStandardWildEncounter`, 헤드버트·바위깨기·사파리·벌레잡기 대회·대량발생, `rtc`, 팔레트, 날씨 포함). 88개는 리터럴만 다름 38(assert 줄 번호 등), 아무 일도 하지 않는 훅 호출 추가(피호출 함수가 `bx lr`이거나 `active && trainerType == 0xFF`가 아니면 바로 return), 코드는 다르지만 결과가 같음(static → 전역으로 인라인 변화, 레지스터 재배치, `GetOppositeDirection` 표 0~8 같은 값, `FishingWildEncounter` 순수 계산 순서, `ComputePlayerShinyOdds` 챌린지 표 인라인·호출 순서 같음, `GetMonPersonality`는 이식 전 무한 루프였던 요청만 다름)으로 분류된다. RAM 기호 1,312개 주소 같음. `gSpeciesInfo`는 mid와 post가 바이트 같음. trainerType 0xFF 오브젝트 0(맵 JSON 전수).
- HnS 적응 3곳·D4·이동 타입 번호 모두 정확(칸 없음 처리 함수의 호출자는 하나, 이동 타입 값 중복 없음·표 3개 90칸).
- #10020: HnS 직전 `follower_npc.c`가 upstream #10020 부모와 같은 blob, #8434만 있으면 같은 좌표가 DIR_NORTH가 되는 것을 되돌림(HnS는 도달하지 않음).
- [경미] `wild_encounter_ow.c` HnS 가드 주석의 근거("`gTimeBlend`를 일찍 갱신")는 seq 285 #10343 뒤 맞지 않게 된다 — 그때 가드는 유지하고 주석만 고친다.
- [정보] 피라미드 `forceSpecies` 무한 루프(upstream 설계, OWE + 무작위 피라미드를 함께 켤 때만), 분석 4.2절 B5(DexNav 경로는 컴파일에서 빠짐) 정정.

## 실기 확인 항목 (친구용)

1. 야생이 나오는 도로·동굴에서 1분 넘게 서 있거나 걸을 때 시간대 색조(아침/낮/저녁/밤)가 이전처럼 바뀌는지
2. 오브젝트가 많은 맵(예: `BellchimeTrail_hns`)에서 동반 포켓몬과 함께 이동, NPC가 사라지거나 깨지지 않는지
3. 탑 빛(`MOVEMENT_TYPE_TOWER_BEAM`) 오브젝트를 쓰는 맵(21개 맵 40개 오브젝트 — 스즈의 탑 1~9F, 금빛시티, 41번 도로 등)에서 기존 세이브를 불러왔을 때 오브젝트 움직임
4. 풀숲·낚시·헤드버트·바위깨기·사파리·벌레잡기 대회·피라미드 야생 배틀 시작, 서핑 포켓몬 표시, 챌린지 이로치 확률

## 후속 행 메모

- seq 177 #9879: config 이동. `pokemon.c`의 `ComputePlayerShinyOdds` hunk는 HnS `FLAG_SYS_POKEDEX_GET` 문맥 때문에 거부된다 — `WE_FLAG_NO_CATCHING` 줄만 손으로 바꾸고 `GetShinyOdds()`·도감 조건 유지. `OW_FLAG_NO_ENCOUNTER` → `WE_FLAG_NO_ENCOUNTER`는 HnS 값 `FLAG_DISABLE_ENCOUNTERS` 유지. HnS 가드 3곳은 겹치지 않는다.
- seq 185 #9910: `wild_encounter_ow.c`의 HnS `return TRUE` 자리를 upstream 결과(`u32 …(void)`, `return OBJECT_EVENTS_COUNT`)로 바꾸고 HnS 주석을 지운다. HnS 호출부는 `InitObjectEventStateFromTemplate` 1곳.
- seq 190 #9968: #9879 뒤.
- seq 208 #10066 등 뒤 행: 이번 patch가 upstream의 공백만 있는 줄 48줄을 지웠으므로 뒤 행 patch는 `sed -E 's/^ ([ \t]+)$/ /'`로 정리한 뒤 적용한다.
- seq 285 #10343: HnS `UpdateTimeOfDay`에 `sHoursOverride`가 있어 거부된다 — `updateBlend` 인자를 손으로 넣고, (A) 첫 줄 가드는 유지(매 프레임 `RtcCalcLocalTime`·`gTimeOfDay` 쓰기가 남으므로)하되 주석을 고친다. HnS의 다른 `GetTimeOfDay()` 호출자가 블렌드 갱신 부수효과에 기대는지 점검.
- seq 363 #10096: 반짝임 그림 hunk는 같지만 같은 PR의 `object_event_pic_tables.h` 대규모 변경은 HnS 그림 표와 따로 대조.
- seq 680 #9970: `ComputePlayerShinyOdds`의 HnS 줄(`GetShinyOdds()`·도감 조건) 보존.
- #9968·#10066·#9966·#10076: HnS 적응과 겹치지 않음(#10076 뒤에도 #10020 같은 좌표 가드 유지).
- 친구가 나중에 OWE를 켤 때(참고, 영향 분석 5절·런타임 리뷰 6절, 결정하지 않음): #9910(seq 185) 전에는 켜면 안 됨(배열 밖 쓰기), 매 프레임 `GetTimeOfDay`로 색조 회귀 재발(#10343 + HnS `overworld.c` 맞춤 필요), 켰다 끈 세이브의 생성 OWE 정리 누락(HnS 적응 (1)이 1회 정리를 건너뜀), `make hns` RELEASE=0이라 팔레트·타일 부족 assert가 플레이어에게 보임, `OW_GFX_COMPRESS TRUE`와의 VRAM, HnS 파이크·피라미드 `_HNS` 레이아웃 미인식, OWE 전투에서 싱크로·헤롱헤롱바디·포케블록 효과 빠짐, OWE 이동 타입 0x54~0x59를 영구 유지, 동반 포켓몬 충돌로도 전투 시작.
