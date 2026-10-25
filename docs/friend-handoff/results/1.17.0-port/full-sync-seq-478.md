# full-sync 묶음 45 — seq 478 (#10300 동적 기술 분류 정리·프레젠트 스크립트 삭제) + 480 #10365 앞당김

- 요청: 사용자 "ㄱㄱ 478 시작해 혼자해도 되는거라면"(2026-10-25).
- upstream: 읽기 전용 사본, #10300 `1edd080a8f`(15파일, 게임 코드만 — 테스트 변경 없음). kor=N. 같은 단위(`U-category-10300`)의 #10365 `240e9dc3b8`(seq 480, 1줄)과 #10362(seq 482)의 프레젠트 한 줄을 앞당겨 같은 커밋에.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 478 (+480, 482 일부) | #10300 (+#10365, #10362 1줄) | `00ff6d5a` | `swapDamageCategory`·`categoryOverride` → `dynamicMoveCategory` 하나로, `DAMAGE_CATEGORY_NONE`(0)을 맨 앞에, 동적 타입 표식 비트(`F_DYNAMIC_TYPE_*`) 삭제, 프레젠트는 일반 공격 스크립트 + 취소 단계에서 회복 분기(`presentdamagecalculation` 명령 삭제), `BattleScript_PresentHealTarget` → `BattleScript_HealTarget`, 분류 아이콘 표 첫 칸 `NULL`(#10365), HnS 보정 3개, 한글 회귀 테스트 | 32,958,340 |

이식 전(`014ea81b`, 코드 = `63b6a114`) 32,963,380 B → **32,958,340 B(−5,040)**. 줄어든 건 `IsBattleMoveStatus`가 표 직접 읽기(인라인) 대신 함수 호출이 된 영향이 여러 함수에 퍼진 것(심볼 비교: AI·우선도·입력 함수 등 수십 개가 −100~−260 B씩, 지운 명령 2개 −264 B).

## 내용과 HnS 판단

- **분류 번호가 1씩 밀림(물리 1·특수 2·변화 3):** 분류 값을 숫자로 쓰는 곳 점검 — 기술 데이터·타입 데이터는 모두 이름으로 명시(빠진 항목 0), `category:2`·`damageCategories:4` 비트 폭 충분, 세이브에 분류 값 없음. **분류 아이콘**은 요약·HGSS 도감·기술 떠올리기·배틀 기술 정보가 모두 같은 애니 표(`gSpriteTemplate_CategoryIcons`)를 분류 값으로 고르는데 upstream #10300 시점엔 표가 0·1·2 세 칸이라 아이콘이 한 칸씩 어긋나고 변화기는 표 밖 — upstream이 seq 480 #10365에서 표 앞에 `NULL`을 넣어 고침 → **같은 커밋으로 앞당김**.
- **HnS 물리·특수 챌린지(`optionStyle == 1`, 타입 기준 분류):** 새 `GetBattleMoveCategory`에 HnS 조건 유지. 덤으로 배틀 밖(요약 화면)에서 변화기가 타입 기준 물리/특수로 나오던 이전 동작이 변화기로 바로잡힘(이식 전 코드에서 울음소리 = 물리, 이식 뒤 = 변화 — 새 테스트로 확인).
- **프레젠트:** 위력 추첨이 `SetDynamicMoveCategory`(대상 지정 단계)로, 회복(1/5)은 분류를 변화로 두고 대상 실패 단계에서 체력이 가득이면 "그러나 …은(는) 체력이 가득찬 상태다!", 아니면 `BattleScript_PresentHeal`. HnS 회복 반짝임(seq 428 A안)을 회복 스크립트에도 넣음.
- **HnS 보정 1 — 프레젠트 회복이 아예 안 되던 upstream 결함:** upstream은 대상 실패 단계에서 `gBattlescriptCurrInstr`만 회복 스크립트로 바꾸고 성공을 돌려, `attackcanceler`가 끝에서 공격 스크립트의 다음 명령으로 덮어쓴다 → 회복 추첨이 나와도 체력이 그대로(1.17.0·master 같음 — upstream 프레젠트 테스트는 HP 막대 유무만 봐서 못 잡음). 회복 스크립트가 `attackcanceler`로 시작하는 구조라 손가락흔들기류처럼 `CANCELER_RESULT_RUN_SCRIPT_AND_INCREMENT`로 실행하게 함.
- **HnS 보정 2 — 동적 타입 초기화 유지:** upstream은 `SetTypeBeforeUsingMove`의 `dynamicMoveType = 0`을 지웠는데 `GetDynamicMoveType`은 일반 기술에 `TYPE_NONE`을 돌려줘 값을 덮지 않는다 → 잠꼬대·손가락흔들기로 나온 기술, AI가 연달아 평가하는 기술이 앞 기술의 동적 타입(예: 페어리스킨 잠꼬대의 페어리)을 이어받음. 초기화 줄을 남김.
- **HnS 보정 3 — 동적 분류를 행동마다 초기화:** upstream은 분류를 기술 끝에서 지우지 않고 스마트 AI 턴 시작에만 지운다(`SetAiLogicDataForTurn`은 `IsSmartBattle()`이 아니면 바로 반환) → 야생·일반 트레이너전에서 턴 마지막 기술이 프레젠트 회복이면 다음 턴 모든 기술이 변화기로 취급(짓궂은마음 우선도 +1, 기술 정보 창·배틀 중 요약 화면 아이콘). 동적 타입과 같은 자리(행동 끝·턴 시작)에서 같이 지움.
- **#10362 한 줄 앞당김:** 회복 추첨 때 `presentBasePower`가 지난 추첨 값(또는 AI 계산의 80)을 그대로 가져 회복이 공격으로 바뀌는 문제 — upstream이 seq 482에서 넣은 `presentBasePower = 0` 한 줄만 먼저.
- upstream 그대로 받은 것: Z기술은 원래 기술의 실제 분류로 판정, 다이맥스 기술은 고른 기술의 분류, 셸사이드암 애니는 `IsBattleMovePhysical`, AI 대미지 계산 순서(타입 → 분류)와 끝 정리. 참고로 AI 대미지 계산도 프레젠트 추첨을 굴려 1/5 확률로 프레젠트를 변화기로 본다(재확인 96에 메모).
- 제외: upstream `IsTargetingBothFoes`의 `GetMoveCategory` 줄 — HnS엔 그 조건(매직미러 주석 블록)이 아직 없음(뒤 seq).

## 검증

- **빌드:** `make hns -j8` 종료 0, 새 경고 0.
- **전체 테스트**(보정 1·3 넣기 전, 새 테스트 6개 포함): `test-baseline-seq477.txt` 대비 새 PASS 6(새 한글 테스트)뿐, 사라진 PASS 0. 그 뒤 보정 1·3을 넣고 다시 전체 — 새 PASS 7(새 한글 테스트)뿐, 사라진 PASS 0, 다른 줄 0. 새 기준 [`test-baseline-seq478.txt`](test-baseline-seq478.txt)(**다음 비교 기준**).
  - 보정 1 전에는 `HnS heal sparkle: Present's heal ...`가 FAIL이었음(회복 자체가 안 돼서) → 보정 뒤 PASS.
- **새 한글 회귀 `test/hns_10300_category.c` 7개:** 분류 아이콘 표, 챌린지 타입 분류의 변화기, 프레젠트 회복(1/4 회복·문장), 공격 추첨 다음 회복 추첨, 체력 가득, 잠꼬대 + 페어리스킨 → 불꽃세례는 불꽃(드래곤에 별로), 프레젠트 회복 다음 턴 짓궂은마음 우선도: **7/7 PASS**.
  - 이식 전 코드(클론 `014ea81b`): **6/7** — 챌린지 타입 분류의 변화기만 FAIL(위 바로잡힘).
  - 보정 확인(일부러 되돌려 실행 후 원복): 동적 타입 초기화를 빼면 잠꼬대 테스트 FAIL, `presentBasePower = 0`을 빼면 공격 다음 회복 테스트 FAIL, 행동마다 분류 초기화를 빼면 짓궂은마음 테스트 FAIL.
- **세이브 정적 비교**(이전 = 코드 `014ea81b`, 클론 빌드): **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`kortests/run.sh <저장소> kor-b59 8`, 커밋 `00ff6d5a`): **558/652, `판정: PASS (기대와 같음)`**.
- **ROM**(커밋 `00ff6d5a`): SHA1 `117e85156c5dfa172c012ae4456dd76783af40e0`, 사용 32,958,340 B(파일 33,554,432 B), 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 하지 않음(친구).** 후보 — ① 요약 화면·기술 떠올리기·HGSS 도감·배틀 기술 정보 창의 물리/특수/변화 아이콘(번호가 밀린 곳이라 꼭), ② 프레젠트 회복(얼음샛길 밤·쌍둥이섬 낮의 야생 딜리버드 등 — 회복 반짝임·문장·체력, 다음 턴 기술 정보 창 아이콘), ③ 챌린지 물리·특수 옵션을 켠 상태의 요약 화면 변화기 아이콘.

## 다음

- seq 479 #10330(Adjust TargetFailure to be slightly more accurate). 480은 이번에 적용함.
