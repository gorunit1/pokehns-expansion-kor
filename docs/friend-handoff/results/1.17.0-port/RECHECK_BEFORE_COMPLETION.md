# full-sync 완료 전 재확인 목록

1.17.0 full-sync를 끝내고 최종 handback을 보내기 **전에** 다시 확인할 항목이다. 2026-10-04 친구 답장(`../../FRIEND_REPLY_2026-10-04.md`)에서 "완료 전 재확인할 버그 목록"을 두기로 했다. 항목을 해결하면 지우지 말고 "해결" 칸에 커밋·seq를 적는다.

## 1. 알려진 버그 (일부러 남겨 둔 것)

| # | 내용 | 원인·위치 | 왜 남겼나 | 해결 조건 | 해결 |
|---|---|---|---|---|---|
| 1 | **더블배틀에서 가로챈 멀리짖기 → assert 화면**(`make hns` RELEASE=0). 이식 전에는 능력이 하나도 오르지 않았다 | seq 129 #9674. 두 번째 대상(파트너)으로 move end를 다시 돌 때 `MoveEndUpdateLastMoves()`가 `RestoreAttacker()`를 한 번 더 불러 저장 스택이 빈다(`full-sync-seq-129-129.md` "친구에게 물을 것" 1) | 친구 결정(2026-10-04): 임시로 고치지 않는다. 이식 전 동작으로 억지로 되돌리지도 않는다 | 가로챈 쪽과 그 파트너가 능력이 오르도록 대상·주체를 제대로 정리할 수 있을 때 고친다. **이후 upstream에 수정이 들어오면 그것을 우선 반영**한다(이식할 때마다 이 경로를 확인) | |
| 2 | 미래예지·파멸의소원이 빗나갈 때마다 스크립트 스택 1칸 누수. 더블 4회 빗나감 뒤 깊은 연쇄(위기회피·위협·오기)에서만 8칸 초과(assert, 없으면 소프트락) | seq 132 #9680. `MonTookFutureAttack`의 `accuracycheck` → `MoveEnd`의 `end`(`full-sync-seq-132-132.md` 남은 위험) | upstream과 같은 코드, 실제 게임에서는 사실상 도달하지 않음 | seq 470 #9939가 `accuracycheck`를 없앤다. 그때 해소 확인 | |
| 3 | 턴 끝 소란 기상 때 깬 포켓몬 파티 데이터에 소란 사용자의 status1을 보냄 | seq 125·127 기록(`full-sync-seq-121-126.md` 별건 2) | 친구 결정: 기록만(2026-10-03) | upstream 수정 여부 확인 | |
| 3b | 탈출버튼으로 교체돼 들어온 춤추기 포켓몬이 같은 턴의 춤 기술을 따라 추지 않음 | 2026-10-04 HnS 수정(`b07fd88953`) 관찰용 테스트에서 발견. 수정 전후 같음(`full-sync-hnsfix-2026-10-04.md`) | 원인 미조사, 이번 수정 범위 밖 | seq 166 #9784(탈출 아이템 재구성) 뒤 다시 확인, upstream 동작 대조 | |

## 2. 중간 상태 (뒤 PR이 들어오면 풀릴 것)

| # | 내용 | 풀리는 seq | 확인할 것 | 해결 |
|---|---|---|---|---|
| 4 | 매직미러·매직코트로 튕긴 막말내뱉기: 반사자 대신 원래 사용자가 교체된다. 되살린 `parting_shot.c` 테스트 2개 FAIL | seq 181 #9730 | 테스트 2개 PASS, 출력 변화 문서 행 수정·삭제 | |
| 5 | 가로챈 쪽의 대상 판정이 엉뚱한 포켓몬에 적용(원래 사용자 전기엔진·방음 팝업 등), 가로챈 쪽 `MoveFailure` 재판정 없음 | seq 181 #9730 | 커밋 리뷰 A 실측 사례가 풀리는지, 출력 변화 문서 행 | |
| 6 | 불바다 종료 문장의 진영이 반대 | seq 385 #10214 | D 회귀 테스트 2-13 기대값, 출력 변화 문서 행 | |
| 7 | #9819(록클라임·안개제거 항상 정의) 미적용 | seq 446 #10548 **직전**에 #9819 → #10548 | 파티 메뉴 필드 기술 항목 | |
| 8 | 일부만 선반영된 PR: #10223(seq 390) 1줄, #10445(seq 298) 1줄 | 해당 seq | 선반영 줄만 빼고 나머지 hunk 정상 검토 | |
| 8b | seq 138.5 #8943 단위 뒤 남은 같은 unit 행: #9799(171) 링크 인트로 문장 `sText_LinkTrainerSentOutPkmn` 토큰과 HnS 교체 등장 왼쪽 우회 정리·AI `IsSwitchinValid` 파티 인덱스, #10051(204) `B_TRAINER_*` 이름 변경(HnS 전용 줄 약 160줄과 E-215·E-287 적응 포함), #10059(207) 풀 팀 멀티 파티 메뉴 | seq 171·204·207 | `full-sync-seq-138.5-138.5.md` "후속 행 메모" | |
| 8c | 멀티 화이트아웃: HnS 보호 수정 `cc0576a543`(반 팀은 파트너 기절 수를 세지 않음)이 들어간 `NoAliveMonsForPlayer`를 #10039가 다시 쓰고 #10568이 `WillPlayerWhiteOutIfPartnerWinsAlone`으로 대체한다. 209~315 사이에는 다른 쓸 포켓몬이 없는 플레이어가 목호가 남은 동안 계속 진행하는 중간 상태가 생길 수 있다 | seq 209 #10039, 315 #10568(+340 #10674) | 두 행 모두 HnS 보정 필요 여부, 로켓단 아지트 실기(3마리/4마리 이상) | |
| 8d | **#10711 HnS 적응 필요:** `AreMultiPartiesFullTeams`의 새 조기 반환(`B_MULTI_HALF_TEAMS` TRUE, `gBattleTypeFlags`에 TRAINER 없음)이 `gSpecialVar_Result`를 설정하지 않는다. `multi_do`는 배틀 전에 부르므로 직전 배틀이 야생이거나 이어하기 직후면 지난 `VAR_RESULT=1`을 읽어 로켓단 아지트에서 3마리 선택을 건너뛴다 | seq 348 #10711 | 조기 반환에서도 `gSpecialVar_Result`를 세우고 트레이너 여부는 `TRAINER_BATTLE_PARAM.opponentA`로 보는 HnS 보정. `AreOpponentsFacilityTrainers()` 두 줄(`b43032bf03`, 배틀타워 상대 141 분리)을 유지한 채 손 병합한다. TRAINER 조기 반환의 HnS 판정은 `multi_do`(필드, 지난 플래그)와 따라오기 NPC(필드, 새 플래그)를 구분한다. seq 170 #9751 뒤에는 이 판정을 테스트 러너로 실측할 수 없다(TESTING 분기) | |

## 3. 한글 문구 (이식과 무관하게 찾은 것, 친구 확인 대상)

| # | 내용 | 위치 | 해결 |
|---|---|---|---|
| 9 | 텔레키네시스 문장 조사 고정: `{B_DEF_NAME_WITH_PREFIX}는\n높이 뛰어올랐다!` → 받침 뒤에서 `마자용는`(→ `{B_TXT_EUNNEUN}`) | `src/battle_message.c` `STRINGID_HURLEDINTOTHEAIR` (2026-10-04 턴 종료 테스트에서 발견) | |
| 10 | 섬광 문장 조사 고정: `{B_ATK_NAME_WITH_PREFIX}로부터` → 받침 뒤에서 `으로부터`여야 함 | `src/battle_message.c` `STRINGID_CLOAKEDINAHARSHLIGHT` | |
| 10b | **`{B_ATK_PREFIX2}` 뒤 조사(친구 요청 A와 같은 유형, 2026-10-04 리뷰 발견):** 주술 시작 `STRINGID_SHIELDEDFROMCRITICALHITS` `주술의 힘으로\n우리 편은의 급소가 숨겨졌다!`/`상대는의`, 팀 가드 `STRINGID_PROTECTEDTEAM` `우리 편은을\n와이드가드가 지켜 줬다!`/`상대는을`(패스트가드·마룻바닥세워막기·트릭가드 공용). 제안: 토큰만 `{B_ATK_TEAM1}의`, `{B_ATK_PREFIX3}`(+`{B_TXT_EULREUL}` 삭제) | `src/battle_message.c` 567·577 | |
| 10c | `Time to Gigantamax!` 영문(HnS 미도달, seq 139 회귀 테스트에서 발견) | `src/battle_message.c` | |
| 11 | 멸망의바디 `STRINGID_PKMNSWILLPERISHIN3TURNS` 영문 | `full-sync-seq-127-127.md` 별건 | |
| 12 | 분노의경혈 `TARGETSSTATWASMAXEDOUT` 본문 누락(`의`, 줄바꿈, 조사) | seq 181 #9730 때 함께(`full-sync-seq-127-127.md`) | |
| 13 | D6a 방벽 해제 진영 이름(실기 확인 뒤 결정), D6c 사령탑 두 번째 이름(별도 과제) | `HANDBACK_2026-09-30.md` 9절, `FRIEND_REPLY_2026-10-01.md` | |

9·10은 친구 로컬 번역 커밋과 겹칠 수 있어 이식 중에는 고치지 않았다. 통합 때 친구 쪽과 맞춰 처리한다.

## 4. 빌드·기타

| # | 내용 | 해결 |
|---|---|---|
| 14 | 기본 `make`(Emerald)·FRLG 빌드가 HnS 전용 `FLAG_DEFEATED_RED`(`src/pokemon.c`, `src/party_menu.c`) 때문에 실패(이식 전부터, `full-sync-seq-128-128.md`) | |
| 15 | `make hns`는 RELEASE=0이라 그림 없는 트레이너 ID, 비어 있는 저장 스택 등에서 assertf 화면이 뜬다. 새 파트너·트레이너 추가 때 그림 존재 확인(`full-sync-seq-128-128.md`) | |
| 16 | 프런티어 AI 파트너 멀티의 트레이너 슬라이드가 `gBattlePartners`를 표 밖에서 읽을 수 있음(지금은 슬라이드 대사가 없어 도달 안 함, `full-sync-seq-128-128.md`) | |
| 17 | 한글 출력 회귀 테스트(턴 종료 70개 등)가 저장소 밖에만 있다. 저장소에 둘지 친구와 정한다 | |

## 5. HnS가 upstream과 다르게 둔 곳 (뒤 PR을 이식할 때 다시 맞출 것)

| # | 내용 | 위치 | 다시 볼 때 | 해결 |
|---|---|---|---|---|
| 18 | **녹화 배틀 섹터 여유 0 B.** `RecordedBattleSave` 4,092 B = `SECTOR_COUNTER_OFFSET`(seq 138.5 #8943, A안) | `include/recorded_battle.h`, `STATIC_ASSERT(RecordedBattleSaveFreeSpace)` | `struct Pokemon`·`BATTLER_RECORD_SIZE`·`MAX_BATTLE_TRAINERS`를 바꾸는 PR. `/home/hjm0725/hns-sync-work/chunk-1385/verify/save_compat.py run` 재실행 | |
| 19 | **`struct Trainer` 비트필드 HnS 5/4**(`encounterMusic:5`, `mugshotColor:4`, upstream 1.17.0은 4/3). HnS 곡 0~26·`MUGSHOT_COLOR_LIGHT_BLUE` 때문이며 upstream 폭이면 `data.c` 빌드 실패 | `include/data.h` | `struct Trainer`를 바꾸는 PR | |
| 20 | **녹화 시작 파티 정적 저장:** `sSavedParties`를 upstream 힙 포인터 대신 정적 EWRAM 배열(+2,400 B)로 둔다. upstream 형식은 재생 때 힙 초기화로 저장 파티가 덮여 재생 뒤 플레이어 파티가 깨지고, 녹화에 전투 뒤 파티를 저장한다(1.17.0·master 같음, 미병합 `grintoul-recorded-battle-fix`) | `src/recorded_battle.c` | `recorded_battle.c`를 바꾸는 PR, upstream이 이 버그를 고칠 때 | |
| 21 | HnS 보호 수정(upstream 1.17.0에도 남은 #8943 결함): 강제 교체 오른쪽 트레이너 `70fe10ecd8`, 멀티 경험치 참가 비트 `c68e8e13ba`, 반 팀 화이트아웃 `cc0576a543`, 리뷰 후 수정 파티 번호 충돌 `8bcf557c20`(도구 대상·기술 습득·레벨업 연출·기사회생의기원·미래예지·급소 횟수·치유방울)·목호 포켓몬 도구 사용 `624ef7d4bd`(도우미 `GetItemTargetPartyOwner`)·배틀 밖 가드 `6c15064bce`. #10536 `givenExpMons` `>> 1` 보정 2곳(`93656e609b`) | `battle_script_commands.c` `Cmd_forcerandomswitch`·`NoAliveMonsForPlayer`, `battle_util.c` 참가 비트·경험치 | 같은 함수를 고치는 upstream PR이 들어오면 그쪽을 우선하고 HnS 줄을 정리 | |
| 22 | 1.17.0에도 남은 #8943 결함(고치지 않음): 링크 비멀티 `SetBattlePartyIds`가 컨트롤러 설정 전에 `GetBattlerParty`를 부른다(선두가 알·기절일 때만). 녹화 통신 멀티 재생에서 `BattleSideHasTwoTrainers`가 상대를 트레이너 1명으로 판정(배틀러 3이 상대 A 파티를 읽음, 실기 불가). `AreMultiPartiesFullTeams`가 `0xFFFF`(2vs1)로 HnS `gTrainers`를 읽는다(범위 밖 ROM 읽기, 크래시 없음. 프런티어 번호로 읽던 것은 `b43032bf03`에서 시설 배틀이면 읽지 않게 분리). side 기준 `itemLost[B_SIDE_PLAYER]`·`activeGimmick[GetBattlerSide]`(목호 배틀러가 플레이어 칸 표시를 건드림, 배틀 뒤 도구 복원 결과 영향 없음, 공생만 극히 드물게 다름). (레벨업 연출·체력 상자 충돌은 `8bcf557c20`으로 해결) | `battle_main.c`, `battle_controllers.c`, `battle_util.c`, `battle_move_resolution.c` | upstream 수정 여부 확인 | |
| 23 | **`B_MULTI_HALF_TEAMS FALSE`(친구 확정 2026-10-04, `FRIEND_REPLY_2026-10-04.md` 2절).** 친구가 mGBA로 배틀타워 멀티 UI·6쌍을 확인한다. 두 트레이너 동시 발견 6쌍의 4마리 트레이너가 4마리를 내고 배틀타워 멀티룸이 풀 팀 UI. TRUE면 이식 전과 같다 | `include/config/battle.h` | 친구 답에 따라 config 1줄 | |
