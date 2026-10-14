# full-sync 묶음 21 — seq 337~350

- 작업: 클라우드 세션, 2026-10-14.
  - 시작 HEAD는 `86c34e07`(코드 = 묶음 20 끝 `b89fb8e9`)이다.
  - 사전 분석 도중 친구 답 2026-10-14 반영(`5e7e97c2`·`4de35b2b`·`da0adeaa`, [`friend-reply-2026-10-14.md`](friend-reply-2026-10-14.md))이 먼저 들어가, 적용은 `95ebea3c`(코드 `da0adeaa`, ROM SHA1 `c95e0c81…`) 위에서 했다.
- 방식:
  - 사전 분석은 3갈래로 나눴다. 사본에서 빌드·테스트했고, 산출물은 세션 임시 `b22/`에 있다. upstream 패치는 `/home/user/pokeemerald-expansion-upstream`에서 `git show`로 뽑았다.
    - a: 337·338·339·341·343·345·346
    - b: 340·342·347·348 — 재확인 8d
    - c: 344·349·350
  - 결정은 사용자 "추천대로"로 했다. 친구가 정할 것은 없고 알림만 한다.
  - 그 뒤 PR별로 적용·빌드·커밋하고 메인 검증을 했다.
- 351 #9927(Mass outbreak, XL·세이브 영향)부터는 다음 묶음이다.
- 실기(mGBA)는 미확인이다(친구가 함).

## 커밋

| seq | PR | 커밋 | 판정·HnS 적응 | ROM(B) |
|---|---|---|---|---:|
| 337 | #10661 Fix Metronome-called spread moves hitting the user's partner | `9b10cb2a` | `ClearDamageCalcResults()`를 `CancelerPPDeduction`(대상 정한 뒤)에서 `Cmd_setcalledmove`(대상 정하기 전)로 옮긴다. 지우는 줄은 HnS 4인자 `SetTypeBeforeUsingMove` 문맥에서 뺐다. 파일 끝 빈 줄 hunk는 제외했다(HnS가 이미 그 상태). 336 사전 분석 메모("upstream 문맥과 같아짐")는 맞았다 | 32,782,484 |
| 338 | #10666 Added clarification for BattleScript_IntimidateActivates | `88f2e6bf` | 주석 2줄 그대로. ROM 바이트 같음 | 32,782,484 |
| 339 | #10669 Fix Mind Reader and Lock-On spam in double battles | `caf5912a` | **테스트만.** 게임 코드는 HnS 첫 업로드 `1821fd67`부터 있다 | 32,782,484 |
| 340 | #10674 | — | **HnS 동등**(기록만). seq 315 HnS판 `WillPlayerWhiteOutIfPartnerWinsAlone`에는 upstream의 `gSelectedOrderFromParty` 루프가 없다. 다른 루프에도 같은 범위 밖 읽기가 없다 | — |
| 341 | #10671 Prevent Red Card from allowing Z-Move reuse | `512f606b` | 그대로. `Cmd_switchoutabilities`가 Z기믹을 해제한다 | 32,782,500 |
| 342 | #10662 | — | 이미 적용(`b09e11c9`, 2026-10-04). 역방향 `--check`가 깨끗하다 | — |
| 343 | #10683 Recalculate Ash-Greninja's stats after Battle Bond activates | `4b5fa7b7` | 그대로. HnS는 `B_BATTLE_BOND`가 9세대라 본 게임에서 닿지 않는다. enum 값이 1씩 밀리지만 상수 폼 표 안에서만 쓰인다 | 32,782,500 |
| 344 | #10687 | — | 이미 적용(`b275b758`, HnS형). 핵심 두 줄은 이미 있다. upstream의 새 스크립트·문장은 #9939(seq 470) 전제라, HnS는 `BattleScript_MissedTarget`에서 대상을 잠시 바꿔 기존 문장을 쓴다. 한글 조사 테스트 2/2(`상대 마자용에게는 맞지 않았다!`). 출력 변화 행은 넣지 않았다(사용자 결정) | — |
| 345 | #10682 Fix Shell Trap being delayed after a spread move hit | `5ad78ec0` | `MoveEndShellTrap(void)` 한 곳만 손으로 맞췄다(`cv->battlerAtk` → `gBattlerAttacker`). `ChangeOrderTargetAfterAttacker`는 이제 대상 배틀러를 인자로 받는다 | 32,782,564 |
| 346 | #10700 Fix AI not calcing crit when high enough crit stage | `8226746e` | **테스트만**(게임 코드 `1821fd67`부터). upstream 앞 문맥이 HnS에 없어 파일 끝에 붙였고, 끝 공백 1개를 지웠다 | 32,782,564 |
| 347 | #10707 | — | 이미 적용(`7c48ba92`). 역방향 `--check`가 깨끗하다 | — |
| 348 | #10711 Fix full team multi battles with follower NPCs | `132797a2` | **부분 적응(재확인 8d 해결).** 팔로워 NPC·파티 메뉴 hunk는 upstream 그대로이고, `AreMultiPartiesFullTeams`만 HnS 판정이다. 아래 "348 HnS 판정" 절 | 32,782,612 |
| 349 | #10656 Fix Underutilisation of CreateSpriteUnchecked meaning a fatal assertf triggers needlessly | `91d4f798` | 47파일. 손 맞춤 6 hunk(알로라 풀, 파랑·초록 날기 아이콘, 마지막 볼 HnS 줄, 박스·도감 분포). `TrySetupObjectEventSprite` hunk는 이미 동등하다. **HnS 전용 같은 패턴 14곳도 같이 바꿨다**(사용자 결정 — `intro_hns.c` 8곳은 upstream `intro_frlg.c`와 줄 단위로 같음, `surfable.c` 5, 낙엽 날씨 1). 반환값을 검사하지 않는 곳(크레딧·포켓기어 라디오·슬라이딩 퍼즐 등)은 그대로 뒀다. 한 커밋 | 32,782,756 |
| 350 | #10652 Fix talking to buried movement npc | `49f2c5e5` | 그대로(349 뒤 — 둘 다 `trainer_see.c`) | 32,782,772 |

- 적용은 10 커밋(seq마다 하나, 사용자 결정)이고, 이미 적용 3·HnS 동등 1이다.
- ROM은 32,782,484 → **32,782,772 B(+288)**, EWRAM 250,408 B·IWRAM 25,516 B 그대로다.
- 커밋마다 `git diff --check` 통과, `make hns -j8` 종료 0, 새 경고 0.

## 348 HnS 판정 (재확인 8d)

upstream #10711은 `AreMultiPartiesFullTeams`에 `if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER)) return TRUE;`(야생 상대 멀티 = 풀 팀)를 넣고, 조기 반환에서 `gSpecialVar_Result`를 세우지 않는다.

- **upstream 그대로면 생기는 문제(코드 해석):**
  - `multi_do`는 이 함수를 필드에서 배틀 전에 부른다. 이때 `gBattleTypeFlags`는 지난 배틀 값이다(이어하기 직후면 0).
  - 그래서 로켓단 아지트 B2F 아테나 멀티를 야생전 바로 뒤나 이어하기 직후에 시작하면 다음 일이 생긴다.
    - 풀 팀으로 보아 3마리로 줄이는 단계를 건너뛴다.
    - `VAR_RESULT`는 지난 값으로 남는다.
    - 고른 3마리 대신 파티 1~3번이 나간다.
    - 배틀 뒤 `HandleSpecialTrainerBattleEnd`·`SaveSelectedParty`를 거치며 포켓몬이 복제되거나 사라진다.
  - #10773 형태(1.17.1·master)도 이 경우는 같다.
- **HnS 판정:**
  - 야생 상대는 `!(flags & TRAINER) && (opponentA == TRAINER_NONE || IsNPCFollowerWildBattle())`로 본다. `setmultitrainerbattle`이 `multi_do` 전에 상대 id를 세우고, 팔로워 야생전은 새 플래그를 먼저 세운다.
  - 모든 조기 반환이 `gSpecialVar_Result`를 세운다(#10773과 같음).
  - `AreOpponentsFacilityTrainers()` 두 줄(`b43032bf03`, 배틀타워 상대 141 분리)은 그대로다.
  - 사전 분석 b의 대안 A~G 중 안 E다(안 D도 HnS 동작은 같음).
- **측정(사전 분석 사본):** TESTING 분기를 게임 분기로 바꾸고 Half 시험 트레이너를 넣어, 함수를 직접 부르는 테스트 13개를 돌렸다.

  | 판 | 통과 |
  |---|---|
  | HEAD | 13/13 |
  | upstream 그대로 | 7/13 |
  | #10773형 | 10/13 |
  | HnS판 | 13/13 |

  - upstream 두 형태가 놓친 01·02·04번이 8d 경우다.
  - 보통 `make check`는 TESTING 분기만 타서 이 판정을 잴 수 없다(재확인 8i).
- **HnS 동작 변화 0**(코드 해석 + 측정). 따라오기 NPC는 HnS에서 꺼져 있다(`FNPC_ENABLE_NPC_FOLLOWERS FALSE`).
- upstream(1.17.0~master)과 판정이 다르지만 스토리 진행 보호용이고 세이브 영향은 없다. 친구에게는 알림만 한다.

## 재확인 64 정정 (아테나 멀티)

- 묶음 19에서 "#8943 이식(10-04) 뒤 반쪽 팀 멀티의 `HandleSpecialTrainerBattleEnd`가 `AreMultiPartiesFullTeams()`로 `VAR_RESULT`를 0으로 덮어써 아테나 승리 뒤 화이트아웃 분기"라고 적고 친구에게도 그렇게 알렸다. **이 원인 해석은 잘못이다**(코드 해석 — 사전 분석 b가 찾고 메인이 확인).
  - 반쪽 팀 멀티에서는 `multi_do`가 `MULTI_BATTLE_CHOOSE_MONS`를 붙인다(`asm/macros/battle_frontier/battle_tower.inc`).
  - 그래서 `DoSpecialTrainerBattle`이 `gBattleScripting.specialTrainerBattleType = 0xFF`로 바꾸고(`src/battle_special.c` 127~128), 배틀 끝 `HandleSpecialTrainerBattleEnd`의 `switch`는 MULTI 경우를 타지 않는다.
  - `VAR_RESULT`는 `ReturnFromBattleToOverworld`의 `gSpecialVar_Result = gBattleOutcome;` 값 그대로이고, 스크립트도 실제 승패를 읽는다.
  - 이 0xFF 분기와 `multi_do`의 선택 플래그는 첫 업로드 `361f1e4a`부터 있었다.
- #10814 한 줄(`32a5fd5a`)은 MULTI 경우를 타는 **풀 팀 `multi_do` 멀티**에 맞는 수정이다(이전에는 승패 대신 `AreMultiPartiesFullTeams()`의 1이 남음). 해가 없으니 유지한다. HnS `multi_2_vs_2`는 모두 `Multi Party: Half`(아테나·조무래기 23, 디버그 목호 3·이블 3)라 일반 진행에서는 닿지 않는다.
- 친구가 요청한 실기(아테나 승리 → 퇴각 대사, 패배 → 화이트아웃, 후속 이벤트·파티 복구)는 그대로 유효하다.
- 정정은 `full-sync-seq-312-322.md` 끝, `friend-reply-2026-10-14.md` 1절, 재확인 64에도 적었다.

## 동작 변화

새 배틀 문장 0, 기존 문장 변경 0, 배틀 외 새 텍스트 0(영문 코드 주석만 upstream 그대로). `BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 3행 추가(337·341·345).

- 337: 더블에서 손가락흔들기 등으로 부른 **"상대 둘" 전체기(TARGET_BOTH — 스피드스타 등)**가 사용자 파트너를 맞히지 않는다. 지진·파도타기처럼 원래 아군도 맞는 전체기(TARGET_FOES_AND_ALLY)는 호출돼도 그대로 아군을 맞힌다. 이 PR은 대상을 정하는 시점만 바꾸므로 "호출한 전체기는 모두 파트너를 맞히지 않는다"가 아니다(친구 2026-10-14(3) 7절 — 친구에게 보낸 문자의 "부른 전체기가 사용자 파트너를 맞히지 않아"는 이 뜻으로 정정).
  - 사전 분석 사본의 한글 조사 테스트로 플레이어 왼쪽이 부른 경우 FAIL → PASS를 확인했다.
  - ~~상대 왼쪽이 부른 경우는 전후 같다.~~ **정정(2026-10-14, 한글 회귀 `HNSB2021` B3):** 상대 왼쪽도 이식 전에는 파트너 HP 바가 깎였다. 옛 조사 테스트가 `NOT HP_BAR`를 0번과 2번 HP 바 사이에 둬서 그 뒤에 맞는 3번을 보지 못했다. 네 위치·잠꼬대 모두 이식 전 FAIL이다. 이식 전 결함은 파트너 HP 바가 깎이는 연출이었고, 테스트에서 배틀 끝 실제 HP는 그대로였다.
  - HnS 트레이너 데이터에 손가락흔들기 등 하위 기술이 있어 본 게임 더블에서 닿는다.
- 345: 트랩셸 포켓몬이 물리 전체기를 맞으면 바로 다음에 트랩셸이 나간다(순서만 바뀜, 본 게임 도달).
- 341·343: 본 게임에서 닿지 않는다(Z크리스탈 없음, 9세대 Battle Bond).
- 349: 평소 화면은 그대로다. `make hns`는 RELEASE=0이라 스프라이트 표가 다 찼을 때 `CreateSprite`가 재개 가능한 크래시 화면을 띄웠는데, 바꾼 곳에서는 그 장식만 빠진다. 기계 코드는 함수 약 130개가 바뀐다.
- 350: 재 속에 묻힌 트레이너에게 말을 걸 때의 범위 밖 함수 호출을 고쳤다. 묻힌 NPC는 Emerald 맵(113번도로·용암마을 체육관)에만 있다.
- 338·339·340·346·348: 동작 변화 없음.

## 친구에게 알릴 것 (결정 필요 없음)

1. **재확인 64 원인 해석 정정**(위 절). #10814는 유지한다.
2. 348은 upstream과 판정이 다르다(스토리 진행 보호, HnS 동작 변화 0).
3. 337·345는 본 게임 더블에서 동작이 바뀐다. 349는 스프라이트가 다 찼을 때 크래시 화면 대신 장식이 빠진다.
4. 보고만(고치지 않음):
   - 기존 `STRINGID_WAGGLINGAFINGER`에 조사가 없어 `손가락을 흔들었더니 스피드스타 나왔다!`로 나온다. 공식 SV·PLZA는 기술 이름 뒤에 이/가가 붙는다. 기존 텍스트라 친구 번역 범위다.
   - HnS 프런티어 동쪽(28,7)에 Emerald 그림공방동굴로 가는 워프가 남아 있다. 데이터로만 확인했고, 실제로 그 칸에 설 수 있는지는 확인하지 않았다.

## 검증

- **빌드:** 위 표.
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j6`, 코드 `49f2c5e5`): 러너 합계 PASS 2,683 → **2,688** / TOTAL 5,608 → 5,614(목록 고유 행 PASS 2,679 → 2,684 — 아래 "집계 기준", 2026-10-14 정정: 처음엔 러너 합계와 목록 수를 섞어 "2,683 → 2,684"로 적었음). `test-baseline-seq336.txt` 대비 새 PASS 6, 사라진 PASS 1.
  - 사라진 1은 345가 이름을 바꾼 `Shell Trap activates on both opposing Targets`다. 새 이름 `… for both opposing targets in speed order`가 PASS다.
  - 새 PASS: 339 2, 346 1, 341 1, 345 2(이름 바뀐 것 포함).
  - 새 FAIL 1은 337 `Metronome's called spread move does not hit the user's partner`다. 메인이 단독 실행해 보니 `metronome.c:76` `Unmatched MESSAGE`(영문 `Wobbuffet used Metronome!`)뿐이다 — 알려진 한계. 한글 조사 테스트로는 파트너가 맞지 않음을 확인했다(사전 분석 a).
  - 목록 [`test-baseline-seq350.txt`](test-baseline-seq350.txt)(**다음 비교 기준**).
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-b21 8`, 코드 `49f2c5e5`): **532/622**, 기대 요약과 바이트 같음.

### 집계 기준 (친구 2026-10-14(3) 10절 확인 요청)

- **러너 합계**는 `make check` 끝의 `- Tests PASSED:`/`- Tests TOTAL:` 줄이다. 실행된 테스트마다 하나씩 센다.
- **목록**은 `dev_scripts/hns_verify/testlist.sh`가 만든다. 로그의 `[n] 이름: 상태` 줄에서 색 코드와 `[n]`을 빼고 `sort -u`로 고유 행만 남긴다. 기준 목록 `test-baseline-*.txt`가 이것이다.
- 둘의 차이는 4다. 이름이 똑같은 테스트 4쌍이 서로 다른 테스트로 두 번 실행되는데, 목록에서는 한 줄로 합쳐지기 때문이다.
  - `Move scoring comparison properly awards bonus point to best OHKO move` — `test/battle/ai/ai.c` 858·887행
  - `Player does whiteout in a multibattle if the Partner still has Pokemon (2v1; Full Team; Gen3)` — `test/battle/multi_battle_whiteout.c` 275·368행
  - `Zacian returns its Hero Form upon battle end` — `test/battle/form_change/end_battle.c` 16·28행
  - `Zamazenta returns its Hero Form upon battle end` — 같은 파일 43·55행
  - 네 쌍 모두 두 번 다 PASS다.
  - PARAMETRIZE 경우는 러너가 테스트 하나로 센다. FAIL일 때만 `1/3` 같은 접미사가 붙은 이름이 목록에 따로 남는다.

| 묶음 | 기준 코드 | 명령 | 러너 PASS / TOTAL | 목록 PASS(고유 행) |
|---|---|---|---|---|
| 20 끝 | `b89fb8e9` | `GITHUB_ACTION=1 make check BUILD=hns -j4` | 2,683 / 5,608 | 2,679 (`test-baseline-seq336.txt`) |
| 21 끝 | `49f2c5e5` | `GITHUB_ACTION=1 make check BUILD=hns -j6` | 2,688 / 5,614 | 2,684 (`test-baseline-seq350.txt`) |
| 22 끝 | `4ae4d7e1` | `GITHUB_ACTION=1 make check BUILD=hns -j8` | 2,695 / 5,621 | 2,691 (`test-baseline-seq351.txt`) |

- 세 실행 모두 같은 4쌍이 겹친다. 원 로그에서 PASS 줄 수와 고유 수를 직접 셌다: 2,683/2,679, 2,688/2,684, 2,695/2,691.
- 묶음 21에서 처음 보고한 "2,683 → 2,684"는 러너 합계(묶음 20)와 목록 수(묶음 21)를 섞은 것이다. 같은 기준으로는 러너 2,683 → 2,688, 목록 2,679 → 2,684다(+5).
  - +5는 새 PASS 6에서 이름이 바뀐 트랩셸 테스트 1(옛 이름 사라짐)을 뺀 값이다.
  - 기준 목록은 바꾸지 않았다.
- 원 로그(세션 임시 `logs/b20-check.log`·`b21-check.log`·`b23-check.log`)는 저장소 밖이다. 목록과 위 수치만 남긴다.
- **세이브 정적 비교**(이식 전 사실 = `da0adeaa` 빌드 `post-f14b`): **PASS(FAIL 0, WARN 0)**, EWRAM·IWRAM 변수 변화 0.
- **최종 ROM(실기용):** 코드 `49f2c5e5`, 클라우드 `make hns -j8`(`arm-none-eabi-gcc 13.2.1`) → 사용 32,782,772 B, SHA1 `6377af696d8b505f4e9c28683c21c7e5e0c5dd99`.

## 실기(mGBA) 후보

1. 아테나 멀티를 B1F 야생전 직후, 그리고 아테나 앞 세이브에서 이어하기 직후 바로 시작한다.
   - 4~6번 칸을 골라도 고른 포켓몬이 나가는지 본다.
   - 끝난 뒤 파티가 복제·소실 없이 원래대로인지 본다.
   - 승리 → 퇴각 대사, 패배 → 화이트아웃인지 본다.
2. 더블에서 손가락흔들기 계열로 나온 스피드스타 같은 전체기가 파트너를 맞히지 않는지 본다.
3. 트랩셸 + 지진을 본다.
4. 349 회귀: HnS 오프닝, 파도타기 포켓몬·낙엽·비·모래바람 맵, 배틀의 마지막 볼 아이콘, 리프블레이드·폴터가이스트·분화·해수스파우팅·싸라기눈 애니, 날기 지도, 도감 분포, PC 박스.

## 재확인·뒤 PR 메모

- 8d: 해결(348). 8i: 닫음(348 측정으로 대신). 64: 원인 해석 정정.
- 345 뒤 `ChangeOrderTargetAfterAttacker(battler)` — 뒤 PR이나 HnS 새 코드가 옛 `(void)` 형태로 부르면 컴파일 오류로 드러난다.
- 349: `CreateSpriteAtEnd` 56곳·HnS 전용 6곳은 seq 509 #10156(`fatal_assertf`) 대비 변경이다. 그때 HnS 전용 호출부를 다시 본다.
- 526 #10773: `battle_util.c` hunk는 348 HnS판으로 동등해진다. 나머지 hunk는 seq 400 #9440이 들이는 `BattleSetup_StartMultiBattle`에 걸려 있어 그때 본다.
