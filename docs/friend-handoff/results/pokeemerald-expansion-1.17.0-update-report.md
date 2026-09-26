# pokeemerald-expansion 1.17.0 업데이트 결과 보고서

작업 지시서: [`POKEEMERALD_EXPANSION_1.17.0_UPDATE.md`](../POKEEMERALD_EXPANSION_1.17.0_UPDATE.md)

## 진행 상태 (2026-09-26)

| 단계 | 상태 |
|---|---|
| 1. 사전 상태 기록 | 완료 |
| 2. upstream 변경 인벤토리 | 완료 (코드 대조 기준, 빌드·실기 미검증) |
| 3. 작은 단위 이식 | 진행 중. 묶음 A 완료(35개), B 이후 대기 |
| 4. 충돌 보고 | 1차 목록 작성, 사용자 결정 대기 |
| 5·6. 검증·문서화 | 이식 단위마다 진행 예정 |

## 1. 사전 상태

- 브랜치 `pokehns-expansion-kor`, HEAD `0d89762071`, 작업 트리 깨끗함.
- `include/constants/expansion.h`: 1.15.2(미태그, 직전 릴리스 1.15.1).
- 새 clone은 원래 빌드되지 않았다. 이름 입력 화면 PNG 3개와 한글 폰트 생성 규칙이 커밋에서 빠져 있었고, `0d89762071`에서 복구했다.
- 기준 빌드 `make hns -j12`: 성공. `pokehns.gba` SHA1 `7f3f85c7dce5402d388abf369c3e60574b854973`.
- 메모리 사용량:
  - **ROM 33,326,484 B / 32 MB (99.32%, 여유 약 223 KB)**
  - EWRAM 94.99%
  - IWRAM 78.37%

## 2. 조사 방법

- upstream `rh-hideout/pokeemerald-expansion`을 HnS와 별도로 clone했다. 태그 `expansion/1.15.1`~`expansion/1.17.0`을 쓰고, 1.17.0 태그 커밋은 `e8bd1cd7b0`이다.
- HnS 브랜치를 그 clone으로 fetch해서 계보를 비교했다.
  - **merge-base는 `3efb836f72`**(#9875, 2026-04-29)다.
  - HnS는 upstream **master(1.15.2 개발 중) 계열**이다. 1.16.0 upcoming 전용 리팩터는 HnS에 하나도 없다. 예: #9730 Stat Change, #8943 12v12, #9655 Battle Messages, #9939, #9494, #9859, #9446.
- 대상 PR 목록은 changelog `1.15.2`, `1.15.3`, `1.16.0`~`1.16.4`, `1.17.0`에서 뽑았다. 모두 694개다.
  - 65개는 이미 HnS 계보에 포함되어 있다([`already_in_history.tsv`](1.17.0-inventory/already_in_history.tsv)).
  - 나머지 629개는 분리된 스냅샷에서 PR마다 `git apply --check`와 `git apply -R --check`를 자동으로 돌렸다. 결과는 [`all_prs_checked.tsv`](1.17.0-inventory/all_prs_checked.tsv)의 `fwd`/`rev` 열에 있다.
  - 그 뒤 영역별 6개 그룹으로 나눠 HnS 코드와 upstream diff를 직접 대조해 판정했다.
- 판정 근거는 코드 대조까지다. **빌드·자동 테스트·실기 검증은 아직 하지 않았다.**

## 3. 전체 판정

| 그룹 | PR | 이미 적용 | 부분 적용 | 이식 가능* | 선행 필요 | 충돌 | 무관 | 상세 |
|---|---:|---:|---:|---:|---:|---:|---:|---|
| g1 배틀 Fixed (앞) | 85 | 7 | 1 | 44 | 9 | 4 | 20 | [보고서](1.17.0-inventory/g1_battle_fixed_a_report.md) |
| g2 배틀 Fixed (뒤) | 85 | 5 | 0 | 31 | 20 | 4 | 25 | [보고서](1.17.0-inventory/g2_battle_fixed_b_report.md) |
| g3 배틀 변경·기술·특성·도구 | 89 | 14 | 4 | 40 | 10 | 7 | 14 | [보고서](1.17.0-inventory/g3_battle_changes_moves_abilities_items_report.md) |
| g4 AI·테스트·포켓몬·스프라이트 | 101 | 25 | 7 | 39 | 5 | 0 | 25 | [보고서](1.17.0-inventory/g4_ai_tests_pokemon_sprites_report.md) |
| g5 일반·정리·문서 | 151 | 14 | 2 | 43 | 15 | 9 | 68 | [보고서](1.17.0-inventory/g5_general_cleanup_docs_report.md) |
| g6 필드·리팩터 | 118 | 8 | 3 | 28 | 46 | 15 | 18 | [보고서](1.17.0-inventory/g6_overworld_refactors_report.md) |
| **합계** | **629** | **73** | **17** | **225** | **105** | **39** | **170** | |

\* "이식 가능"은 그대로 적용되는 것과, 같은 논리를 HnS의 옛 구조에 손으로 맞춰야 하는 것을 합친 수다. 동작 변화가 없는 순수 리팩터도 들어 있어 이식 가치는 제각각이다.

**결론:** 1.17.0 전체를 따라잡으려면 upcoming 대형 리팩터(#9730, #8943, #9655, #9939, #9881 등)가 먼저 들어가야 한다. 그런데 이 리팩터들은 한글 배틀 문자열, HnS 배틀 메시지 최신화, HnS 전용 데이터와 정면으로 충돌하고 ROM 여유도 부족하다. 따라서 현실적인 범위는 **"master 계열 수정 + 옛 구조에 맞출 수 있는 1.16/1.17 버그 수정의 선별 이식"**이다. "선행 필요" 105건은 리팩터를 들이지 않는 한 보류된다.

## 4. 이식 계획 (사용자 승인 필요)

각 묶음은 PR 1개 단위로 적용한다. 단위마다 `git diff --check`, 대상 오브젝트 빌드, `make hns`, ROM 사용량을 기록한다.

### 묶음 A — HnS에 지금 존재하는 버그, 작은 수정 (최우선)

| PR | 내용 | 비고 |
|---|---|---|
| #10406 | 불요의검·불굴의방패가 `== GEN_9` 비교 때문에 매 교체마다 발동 (`battle_util.c:3497`, `:3511`) | 코드 확인함 |
| #10342 (스탯 외) | AI 버그 5개: `GetMovePower(playerMove != 0)` 괄호 오류(`battle_ai_main.c:679`), 앵콜 검사 뒤 `;`(`battle_ai_util.c:5202`) 등 | 두 곳 코드 확인함 |
| #10318 | 바톤터치가 전자부유 타이머를 넘기지 않음 (`battle_main.c:3398`) | 실기 미확인 |
| #10093, #10213, #10228, #10207, #10132 잔여 | 명령 불복 무작위 기술, 떨어뜨리기, 불꽃화 기술 해동, 나이트메어 팝업, 난기류 웨더볼 | g1 |
| #10514, #10476, #10543, #10554, #10622, #10475, #10675, #10344, #10180 | 분노의주먹 카운터, 코트체인지 겹수, 재생력 슬롯, 절대영도 명중, 메가진화 후 허브류 등 | g2 |
| #10412, #10302, #10425, #10409, #10411, #10006, #9985 | AI 교체·대상·필터 수정 | g4 |
| #10191, #10242, #10247, #10551, #10329+#10529 | 정적 조우 능력치, 순결의부적, 얼룩 알, 동반 포켓몬 크래시, 접근 트레이너 | g6 |
| #10015, #9963, #10241, #9990, #9608 | 첫 전투 핸들러, 더블 리매치 등 | g5 |

### 묶음 B — 1.15.2·1.15.3 master 수정 중 깨끗이 적용되는 것
g1 보고서의 1.15.x 25건이 대상이다. #9929는 변화기 빗나감 문구를 바꾸므로 `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 기록이 필요하다.

### 묶음 C — 용량 확보 (ROM 99.3% 대응)
- #10179: 미사용 함수 삭제, 약 10 KB. HnS가 쓰는 함수는 제외한다.
- #9086: 문자열 공유, 3~4 KB 추정. `src/challenge_menu.c:507`을 같이 조정해야 한다.
- #9765: 약 5 KB.
- #9231·#9207: EWRAM 약 236 B, IWRAM 약 100 B.
- #9121+#9474: 배틀 힙 16 KB. 반드시 함께 넣는다.
- #9906·#10014: 배틀 중 메뉴 힙 피크 약 12 KB.
- 별도 검토: `make hns`는 `LTO ?= 0`이라 `-ffunction-sections`가 붙지 않아 미사용 코드가 ROM에 남는다. 켜면 큰 절감이 가능하지만 동작 검증이 필요하다.

### 묶음 D — 그래픽·UI
- #9973·#9813·#10349·#10392: `FillSpriteRect` 묶음.
- #9882 일부·#9912: 사파리 볼 수 영역.
- #9974·#10270·#10414 잔여: 1.17.0 수동 이식 때 앞모습 애니메이션만 들어와 미니어·실바디·타입:널 등의 팔레트가 틀어졌고, 애니메이션 자리표시자 10곳이 남아 있다. #10270의 대여르 메가 특성 hunk와 #10414의 HnS 커스텀 팔레트는 제외한다.

### 묶음 E — 1.16.x 수동 적응 (이후)
각 그룹 보고서의 "수동 적응 가능" 항목이다. A~D가 끝난 뒤 가치 순으로 진행한다.

## 5. 적용하면 안 되는 항목 (깨끗이 적용되더라도)

| PR | 이유 |
|---|---|
| #10429 | ZWS를 `0x3A`로 추가한다. HnS charmap에서 `0x3A`는 한글 바이트다(`'루' = 3A 0C`, `'겠' = 37 3A`). 한글이 깨진다. |
| #10365 | HnS에 없는 `DAMAGE_CATEGORY_NONE` 자리가 생겨 상태 화면 분류 아이콘이 한 칸씩 밀린다. |
| #10586 | HnS는 필드(장판) 상태를 `gFieldStatuses`에 저장하는데, 이 PR은 그 초기화를 없앤다. 빌드가 실패하거나 동작이 퇴행한다. |
| #10415 | 12v12 트레이너별 파티를 전제로 한다. HnS에서는 두 번째 상대의 일루전이 잘못된 파티를 참조한다. |
| #10388 | HnS `IsAnyTargetAffected`에 `TARGET_USER_AND_ALLY` 처리가 없다. |
| #10432 | 한글 `STRINGID_WALLYUSEDITEM`을 삭제한다. |
| #9211 | ROM이 약 170 KB 늘어난다. HnS는 트레이너 슬라이드를 쓰지 않는다. |
| #10117 (그대로) | upstream 1.17.0의 연산자 우선순위 버그를 그대로 가져온다. 이식한다면 괄호를 고친다. |

## 6. 충돌 — 사용자 결정 필요

기본 권장은 **미이식**이다. 각 항목의 upstream 목적, HnS 동작, 파일·라인, 선택지는 그룹 보고서의 "충돌 상세" 절에 있다.

- **한글 배틀 문자열·STRINGID·지정 코드:**
  - #9799 (+#10436): 교체 메시지
  - #10144 + #9777 잔여 + #9939: 상성 메시지
  - #10149 + #9916: 스위트베일 문구
  - #10315 + #10606: 드래곤애로우
  - #10354: `{B_DEF_NAME}`→`{B_EFF_NAME}` 4곳
  - #10217: 나쁜손(Pickpocket) `STRINGID_PKMNSTOLEITEM`
  - #10443: 3곳
  - #9514
  - #9578: HealerActivates
  - #10314: 특성 팝업 `"의"`
  - #9051, #10335: 문자열 이동
- **HnS 배틀 메시지 최신화 출력 순서:**
  - #10268: 열매·아이템 팝업
  - #9168: 열매 애니메이션
  - #9714: 안개제거
  - #10593: SetMoveEffect 테이블화. Gen1 반동 챌린지와 방벽 순차 출력이 사라진다.
  - #9655, #9680: 구슬·허브 스크립트
- **대형 upcoming 리팩터:**
  - #8943: 12v12, EWRAM 약 +1.2 KB
  - #9730: Stat Change, 176개 파일
  - #9881: INCGFX. 한글 폰트 규칙이 들어 있는 `graphics_file_rules.mk`를 삭제한다.
  - #8893: 문법 개편. 원작업자가 이미 미이식을 결정했다.
- **HnS 고유 데이터·맵:**
  - #7305: 조토 나무열매
  - #9475: 트레이너 사진 `_HNS` 137개
  - #8930, #9518: 도감
  - #9147: 문
  - #10548: 배지 해금
  - #9927: TV 대량발생
  - #8678: `battle_setup.c` 리매치
  - #10131: `HQ_RANDOM`을 HnS randomizer가 사용
  - #9713: 디버그 사운드 메뉴와 DP 음악
- **세이브·스크립트:**
  - #9920: `SaveBlock1.unused_9C2`는 HnS `saveVersionMagic`이다.
  - #9335: `_hns` 스크립트 약 90곳
  - #9986/pret#2200: `special` 뒤 자동 waitstate. #10061 마이그레이션이 필요하다.
- **한글 표시:**
  - #9461: 층수 팝업 폰트
  - #10521: 설명문 폰트 자동 축소
  - #9006: 새 한글 문자열이 필요하다.

## 7. 설정(config) 주의

HnS는 `GEN_LATEST = GEN_CHAMPIONS`다. upstream 기본값을 그대로 들이면 동작이 즉시 바뀐다.

- #10151 잔여를 넣으면 Champions 규칙(마비 12.5% 등)이 켜진다. 기존 미이식이 의도였는지 기록이 없다.
- #10454: `B_OVERWORLD_WEATHER_OVERRIDE`를 `GEN_8`로 둬야 현재 동작이 유지된다.
- #9568: AI가 최대 대미지 롤을 기본으로 가정하게 되어 난이도가 달라진다.

## 8. 기존 기록과의 차이 (정정)

- 지시서의 "PR #10426 흡수 리팩터링 반영"과 달리, 코드에 신규 심볼이 없고 SESSION_LOG에도 "이식하지 않았다"고 되어 있다. 구조는 미적용이다.
- 기록된 "#10561 메가찌르호크 아이콘"은 실제로는 #10346의 아이콘 부분이다.
- #10124는 SESSION_LOG 목록에 없지만 코드에는 들어가 있다.
- #10461 이식본은 아군 대상 기준이 HnS `<=`(`battle_ai_main.c:1031`), upstream `<`로 다르다. 의도한 차이인지 확인이 필요하다.
- `make check`(자동 테스트)는 `fake_rtc.h` 포함 순서 오류로 실행 전에 멈춘다. 테스트 PR은 무관으로 분류했다.

## 9. 사용자 결정 요청

1. 이식 범위를 "선별 이식(4절 A→E)"으로 확정할지. 대형 리팩터를 들이지 않는다는 뜻이다.
2. 6절 충돌 항목의 기본 처리를 미이식으로 할지. 개별로 부분 이식을 원하는 항목이 있는지.
3. 7절 config 항목(Champions 규칙, AI 대미지 롤)을 현재 값으로 유지할지.
4. 용량 확보를 위해 `LTO`/`-ffunction-sections` 도입을 별도로 검토할지.

## 적용 단위 기록

PR별 상세(지시서 결과 보고 양식)는 [`1.17.0-port/batch-a-battle.md`](1.17.0-port/batch-a-battle.md)와 [`1.17.0-port/batch-a-ai-field.md`](1.17.0-port/batch-a-ai-field.md)에 있다.

### 묶음 A (2026-09-26) — 35개 PR 적용, 보류 0

- 두 worktree(`port/a-battle`, `port/a-ai-field`)에서 PR마다 `git diff --check`와 `make hns -j6`를 통과시킨 뒤 1커밋씩 만들었다. 메인 브랜치에는 fast-forward와 cherry-pick으로 합쳤고 충돌은 없었다.
- 통합 빌드 `make hns -j12`: 성공.
  - ROM **33,327,220 B**(기준 대비 +736 B, 99.32%)
  - EWRAM·IWRAM 변화 없음
  - SHA1 `ac6df9a5d50a32edea8dd5a8e4e34d1d5332b18d`
  - 새 컴파일 경고 없음(변경 파일 기준)
- 한글 문자열·STRINGID·`{B_...}`·배틀 메시지 순서·config·`test/**` 변경 없음(diff 전수 확인).
- 부분 적용:
  - #10132: 웨더볼만. 회복기 부분은 이미 반영되어 있었다.
  - #10342: 지정한 AI 버그 5개만.
  - #10247: `!isEgg`만.
  - #10344: 잭열매·로플열매·저주받은바디만.
  - #10180: Sky Drop hunk 제외.
- 자동 테스트·실기 검증은 하지 않았다. 특히 바톤터치 전자부유, 메가진화 뒤 허브류, 접근 트레이너, 동반 포켓몬 스프라이트는 실기 확인이 필요하다.

| PR | 영역 | 메인 커밋 | worktree 커밋 | 제목 |
|---|---|---|---|---|
| #10406 | 배틀 | `51cc80bf74` | (동일) | Intrepid and Dauntless futureproofing |
| #10318 | 배틀 | `be5123a300` | (동일) | Remove redundant Magnet Rise / Laser Focus flags |
| #10093 | 배틀 | `455c2c0b7a` | (동일) | Fixes Random Move from disobedience |
| #10213 | 배틀 | `e2c65a3b47` | (동일) | Fixes Smack Down not clearing correct values |
| #10228 | 배틀 | `0ed3b6440b` | (동일) | Fix dynamic Fire-type moves not thawing frozen targets |
| #10207 | 배틀 | `07f6879d21` | (동일) | Fix Bad Dreams leaving ABILITY_POPUP stuck open |
| #10132 | 배틀 | `0108ca38ba` | (동일) | Weather Ball power under Strong Winds |
| #10514 | 배틀 | `0ae8ecbe78` | (동일) | Fix Rage Fist Hit counter overflow |
| #10476 | 배틀 | `5030d382da` | (동일) | Fix Court Change hazard count swapping |
| #10543 | 배틀 | `c2eec58645` | (동일) | Fix Regenerator/Natural Cure applying to the wrong party slot |
| #10554 | 배틀 | `91be750f79` | (동일) | Fix Hunger Switch Persisting on Ability pop up |
| #10622 | 배틀 | `cbeac58035` | (동일) | Fix Flying Press using its secondary type for damage modifiers |
| #10475 | 배틀 | `06a9eeafdf` | (동일) | Fix Sheer Cold move type check and rename flag |
| #10675 | 배틀 | `6a16bb059f` | (동일) | Fix Solar Beam, Solar Blade and Electro Shot skipping their charging turn with Utility Umbrella |
| #10344 | 배틀 | `26afbae484` | (동일) | Fix Future Sight triggering reactive items and abilities |
| #10180 | 배틀 | `f2d3008825` | (동일) | Add Effect activation after Mega Evolution |
| #10342 | AI·필드·일반 | `ea3b2d9955` | `bc2903faf8` | Fix reversed AI battlerAtk and battlerDef usage |
| #10412 | AI·필드·일반 | `9f426274d3` | `e5a39d6b14` | Reset move data between switch-in calculations |
| #10302 | AI·필드·일반 | `d12fd065c8` | `bdfd13fe9f` | Fix AI partner seeing all moves bad on dead adjacent foe |
| #10425 | AI·필드·일반 | `1b0339910a` | `2f6cf4e90d` | Fix AI Focus Punch checks on Present and Fixed HP moves |
| #10409 | AI·필드·일반 | `f1902b64d5` | `7345201ae4` | Fix AI target filtering and debug score highlighting |
| #10411 | AI·필드·일반 | `58e6231efa` | `ee4e42c910` | Avoid rewarding Levitate ally immunity |
| #10006 | AI·필드·일반 | `aa88736d74` | `50867ebb87` | Fix AI partner flags set to Battler1 instead of Battler2 |
| #9985 | AI·필드·일반 | `369aebb5f1` | `4ba9e18670` | Remove Defiant and Competitive from partner ability check |
| #10191 | AI·필드·일반 | `ff34dcb902` | `5df45529d7` | Fix event mon stat calculate without IVs |
| #10242 | AI·필드·일반 | `95ee322687` | `e2d48d8b1a` | Check hold effect instead of item id for HOLD_EFFECT_REPEL |
| #10247 | AI·필드·일반 | `6480a48924` | `5f3058a3db` | Do not draw Spinda spots on eggs |
| #10551 | AI·필드·일반 | `5f6e897080` | `1746ed266b` | Fix follower crashing when interrupting spin movement |
| #10329 | AI·필드·일반 | `496d653ef8` | `92844b787b` | Fix SEE_ALL_DIRECTIONS trainers not facing player |
| #10529 | AI·필드·일반 | `c52c18075e` | `34b18c3cd5` | Fix movement type playing between trainer move and player face |
| #10015 | AI·필드·일반 | `40d1661079` | `3fadb3c31b` | Fix wrong action handler after rearranging moves in first battle |
| #9963 | AI·필드·일반 | `d489ca10ec` | `46c226ad6f` | Fix double battle rematches being single battles |
| #10241 | AI·필드·일반 | `a18477c727` | `703282acfd` | Fix off by one error in RandomWeightedIndex |
| #9990 | AI·필드·일반 | `e0baafe8fd` | `08d2efebed` | Various follower sprite fixes from issue #5135 |
| #9608 | AI·필드·일반 | `100c0c6a53` | `cb82d098e2` | Remove DecompressTrainerBackPic |

#### 묶음 A 사용자 결정 대기

1. #10529: upstream 그대로면 거리 1에서 발견한 트레이너가 "플레이어 쪽 바라보기"로 고정되지 않는다. 전투 후 맵을 다시 읽으면 원래 이동 타입으로 돌아간다. 이식 전 HnS와 바닐라는 고정했다. 권장은 `TrainerTurnToFacePlayer`의 range-0 분기에 3줄을 추가해 기존 동작을 유지하는 것이다.
2. #10015: upstream은 교체 확정 경로만 고쳤지만 취소 경로에도 같은 조건을 넣었다. 같은 버그이므로 유지를 권장한다.
3. #10344: 레드카드·탈출버튼 조건은 넣지 않았다. HnS의 `BattleScript_FutureAttackEnd`는 `MOVEEND_CARD_BUTTON`을 실행하지 않아 지금도 발동하지 않는다. 방어용 추가는 동작 변화 없이 ROM만 늘리므로 미적용을 권장한다.
