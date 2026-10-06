# full-sync 완료 전 재확인 목록

1.17.0 full-sync를 끝내고 최종 handback을 보내기 **전에** 다시 확인할 항목이다. 2026-10-04 친구 답장(`../../FRIEND_REPLY_2026-10-04.md`)에서 "완료 전 재확인할 버그 목록"을 두기로 했다. 항목을 해결하면 지우지 말고 "해결" 칸에 커밋·seq를 적는다.

## 1. 알려진 버그 (일부러 남겨 둔 것)

| # | 내용 | 원인·위치 | 왜 남겼나 | 해결 조건 | 해결 |
|---|---|---|---|---|---|
| 1 | **더블배틀에서 가로챈 멀리짖기 → assert 화면**(`make hns` RELEASE=0). 이식 전에는 능력이 하나도 오르지 않았다 | seq 129 #9674. 두 번째 대상(파트너)으로 move end를 다시 돌 때 `MoveEndUpdateLastMoves()`가 `RestoreAttacker()`를 한 번 더 불러 저장 스택이 빈다(`full-sync-seq-129-129.md` "친구에게 물을 것" 1) | 친구 결정(2026-10-04): 임시로 고치지 않는다. 이식 전 동작으로 억지로 되돌리지도 않는다 | 가로챈 쪽과 그 파트너가 능력이 오르도록 대상·주체를 제대로 정리할 수 있을 때 고친다. **이후 upstream에 수정이 들어오면 그것을 우선 반영**한다(이식할 때마다 이 경로를 확인) | |
| 2 | 미래예지·파멸의소원이 빗나갈 때마다 스크립트 스택 1칸 누수. 더블 4회 빗나감 뒤 깊은 연쇄(위기회피·위협·오기)에서만 8칸 초과(assert, 없으면 소프트락) | seq 132 #9680. `MonTookFutureAttack`의 `accuracycheck` → `MoveEnd`의 `end`(`full-sync-seq-132-132.md` 남은 위험) | upstream과 같은 코드, 실제 게임에서는 사실상 도달하지 않음 | seq 470 #9939가 `accuracycheck`를 없앤다. 그때 해소 확인 | |
| 3 | 턴 끝 소란 기상 때 깬 포켓몬 파티 데이터에 소란 사용자의 status1을 보냄 | seq 125·127 기록(`full-sync-seq-121-126.md` 별건 2) | 친구 결정: 기록만(2026-10-03) | upstream 수정 여부 확인 | |
| 3c | **불복종으로 자해할 때 HP가 실제로 줄지 않음**(이식 전부터, upstream 1.17.1도 같음) | `moveDamage`↔`passiveHpUpdate` 불일치(seq 153 #9710 사전 분석 D6). HnS는 배지 수에 따른 불복종이 있어 실제 게임에서 닿는다 | 이번 범위 밖 기존 결함으로 발견만 함 | upstream 수정 여부 확인, 또는 HnS 수정 여부를 사용자·친구와 결정 | 친구 결정(2026-10-06): HnS에서 고친다(혼란 자해처럼 데미지). 다음 작업 |
| 3b | 탈출버튼으로 교체돼 들어온 춤추기 포켓몬이 같은 턴의 춤 기술을 따라 추지 않음(특성 팝업만 뜨고 랭크 변화 없음) | 2026-10-04 HnS 수정(`b07fd88953`) 관찰용 테스트에서 발견. **원인(seq 166 사전 분석):** 복사한 기술이 `Cmd_attackcanceler`를 지날 때 그 자리의 `battlerState[].usedEjectItem`이 아직 TRUE라 조기 반환이 춤 복사도 끝낸다(`b07fd88953`이 반사·가로채기만 예외로 둔 분기). seq 166 #9784 뒤에도 같고 upstream 1.17.0도 같다(`full-sync-seq-166-170.md`). 탈출팩으로 들어온 경우도 같고(`TryEjectPack`도 `usedEjectItem`을 세움), 같은 턴만 해당한다 | upstream과 같은 결함, 이식 범위 밖 | 고칠지 친구 결정. 고친다면 `usedEjectItem` 조기 반환에 춤추기 복사 예외를 더하는 HnS 커밋 | 친구 결정(2026-10-06): 고친다. 탈출 아이템 처리·교체가 끝난 뒤 현재 필드의 춤추기를 판정하는 순서, 탈출버튼·탈출팩 회귀 테스트. 다음 작업 |

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
| 8e | **seq 150 #9717 과도기 결함 보정 `8552b5e9a8`**: 같은 턴 끝 미래예지 두 번째 공격의 `clearspecialstatuses`가 위기회피 대기 교체를 지우던 것을 HnS가 막음(`BS_ClearSpecialStatuses`가 `queuedSwitch` 보존) | seq 228 #10161(미래예지 스크립트 `moveendall`) | 그때 HnS 보존 줄을 지우고 리뷰 probe(`chunk-150-155/review-switch/probe/`)를 다시 돌린다 | |
| 8d | **#10711 HnS 적응 필요:** `AreMultiPartiesFullTeams`의 새 조기 반환(`B_MULTI_HALF_TEAMS` TRUE, `gBattleTypeFlags`에 TRAINER 없음)이 `gSpecialVar_Result`를 설정하지 않는다. `multi_do`는 배틀 전에 부르므로 직전 배틀이 야생이거나 이어하기 직후면 지난 `VAR_RESULT=1`을 읽어 로켓단 아지트에서 3마리 선택을 건너뛴다 | seq 348 #10711 | 조기 반환에서도 `gSpecialVar_Result`를 세우고 트레이너 여부는 `TRAINER_BATTLE_PARAM.opponentA`로 보는 HnS 보정. `AreOpponentsFacilityTrainers()` 두 줄(`b43032bf03`, 배틀타워 상대 141 분리)을 유지한 채 손 병합한다. TRAINER 조기 반환의 HnS 판정은 `multi_do`(필드, 지난 플래그)와 따라오기 NPC(필드, 새 플래그)를 구분한다. seq 170 #9751 뒤에는 이 판정을 테스트 러너로 실측할 수 없다(TESTING 분기) | |
| 8f | **seq 166 #9784 중간 상태:** `gBattlersByRawSpeed`가 `MoveEndNextTarget`에서만 정렬된다. HnS `BS_EffectsAfterFormChange`(#10180 선반영)는 배틀러 번호 순 루프를 유지했다(1턴 메가진화 때 정렬 전 배열을 읽지 않게). 플라워가드(`moveendto MOVEEND_NEXT_TARGET` 뒤 `moveendfrom`)가 배틀 첫 move end일 때만 흉내허브 복사가 빠질 수 있다(확률 매우 낮음) | seq 193 #9957(등장 처리 정렬) | `BS_EffectsAfterFormChange`를 1.17.0 형태(`gBattlersByRawSpeed`)로 바꾸고 HnS 주석을 지운다. 한글 회귀 K4-07(1턴 메가진화 위협 → 하양허브 둘). 레드카드가 먼저 발동한 뒤 더 느린 탈출버튼이 빠지는지(1.17.0 실측, 리뷰 A1) 보고 출력 변화 문서 레드카드·탈출버튼 행을 고친다 | |
| 8g | 탈출버튼·탈출팩의 프리폴 조건 | seq 196 #9988 | `TryEjectButton`·`TryEjectPack`·`TrySwitchInEjectPack`에 `IsBattlerInvolvedInSkyDrop()`(#10180 제외 hunk 포함). `full-sync-seq-063-082.md`의 "#9784 이식 때" 메모는 #9988 때로 정정 | |
| 8h | 미래예지 피격과 레드카드·탈출버튼: HnS 미래예지 스크립트는 `MOVEEND_CARD_BUTTON`에 닿지 않아 #10344의 `EFFECT_FUTURE_SIGHT` 조건을 뺐다(seq 166 뒤에도 같음, 한글 K9-01·02) | seq 228 #10161(미래예지 `moveendall`) | 그 조건을 `TryRedCard`·`TryEjectButton`에 넣는다(1.17.0 형태). 8e와 같은 때 | |
| 8i | **seq 170 #9751 뒤 테스트 빌드:** `AreMultiPartiesFullTeams`가 AI 테스트 말고는 항상 `TRUE`라 반 팀 판정에 기대는 HnS 수정 둘 — 배틀타워 상대 141 분리(`b43032bf03`, 스크래치 `zz_f141.c` 18/18 → 9/18)와 목호 포켓몬 도구 사용(`624ef7d4bd`, 스크래치 `zz_fix_r2.c` 14/14 → 7/14) — 을 테스트 러너로 잴 수 없다(게임 코드는 같음). 반환값이 바뀐 저장소 두 트레이너 테스트 64개(17파일)는 결과가 같았다 | seq 209 #10039(`IsAITest` 조건 제거, `multi_battle_whiteout.c`) | 64개 결과 다시 비교(`chunk-166-170/tmp-170/probe/zzmdiff-files.txt`). 두 HnS 수정은 사본에서 `#if TESTING` → `#if 0`, `#if !TESTING` → `#if 1`로 바꿔 `chunk-1385/f141/zz_f141.c`·`chunk-1385/fix/zz_fix_r2.c`(seq 348 #10711 손 병합 때도) | |
| 8j | 편승 문장 주체: 사용자 자신을 대상으로 하는 기술(껍질깨기·칼춤)을 편승이 따라 하면 편승 팝업 뒤 문장이 원래 사용자 이름으로 나온다(`BattleScript_OpportunistCopyStatChange`가 `B_DEF` 사용, 이식 전부터) | seq 181 #9730(`trybattlerstatchange`) | 그 뒤에도 남으면 HnS 수정 여부 결정(12번과 같은 때) | |
| 8k | **친구 답 Q3~Q5(2026-10-05): 먹다남은음식·조개껍질방울·자뭉열매·오랭열매는 팝업만, 별도 HP 회복 문장 없음.** 지금은 팝업 + 회복 문장(`STRINGID_PKMNSITEMRESTOREDHPALITTLE`·`STRINGID_PKMNSITEMRESTOREDHEALTH`) | seq 475 #9777(아이템 회복 문장 삭제·회복 연출) | #9777의 회복 문장 삭제 hunk를 upstream대로 받는다(HnS가 2026-09-20 별도 이식 때 남긴 회복 문장도 이때 정리). HnS 팝업 줄(`e5a5630635` 등)과 3-way 병합 | |
| 8l | **친구 답 Q9: 반감열매는 팝업·열매 연출을 공격 애니메이션 전, "데미지를 약하게 했다" 처리를 그 뒤 명중 처리 쪽.** 지금은 셋 다 공격 애니메이션 전 | seq 483 #10431 | upstream대로 받는다 | |
| 8m | **친구 답 Q10: 특성 팝업이 끝난 뒤 도구 팝업(겹치지 않게).** 지금은 `waitabilitypopup`이 없어 숙성 → 열매, 헤롱헤롱바디·저주받은바디 → 멘탈허브 경로에서 두 팝업이 겹쳐 보일 수 있다 | seq 394 #10268 | `waitabilitypopup`·`BS_DestroyItemPopup` 대기를 받고 위 경로 확인. **HnS 팝업 추가 `e5a5630635`로 생긴 겹침(턴 끝 젖은접시·건조피부·아이스바디·선파워 → 먹다남은음식·검은오물, 기본 설정에서 일렉트릭·사이코메이커 → 시드, 자기과신 → 조개껍질방울)은 #10268만으로 풀리지 않는다** — HnS 추가로 `ItemHealHP_Ret`·`ConsumableItemStatRaise`·`AirBalloonMsgInRet`의 팝업 호출 앞(또는 `_Attacker`/`_Scripting` 헬퍼 안)에 `waitabilitypopup`(`friend-reply-2026-10-05.md` 리뷰) | |
| 8n | upstream #10321(seq 413)의 헤드리스 팝업 가드 1줄을 HnS `66c1e55f3d`로 선반영(테스트 전용, 게임 ROM 불변) | seq 413 #10321 | 가드 3줄을 upstream 블록(기록 호출 포함)으로 바꾸고 나머지 hunk 적용 | |
| 8o | HnS 팝업 `e5a5630635`과 뒤 PR 문맥: 시드·룸서비스 팝업 줄(`ConsumableItemStatRaise`), 회복 팝업 줄(`ItemHealHP_Ret`), 풍선 등장 팝업 줄 | seq 181 #9730, seq 475 #9777, upcoming #10759 | #9730이 능력 상승 블록을 옮길 때 시드 팝업 줄도 함께 옮긴다(빠뜨리면 seq 475까지 시드·룸서비스 팝업이 사라짐). #9777-001은 시드 팝업을 두 번 넣지 않고, #9777-024는 `@ HnS:` 주석 한 줄 3-way. #10759 풍선 hunk는 helper 이름만 다름 | |
| 8p | **seq 171 #9799 HnS 보정 H1:** `GetAiFlags`의 특수 트레이너 판정에 `BATTLE_TYPE_TRAINER` 조건(통신·유니온룸 배틀 뒤 `opponentA`가 남아 배회·사파리·스마트 야생 AI가 꺼지는 것을 막음) | seq 182 #9865(`IsSmartBattle`로 이동) | 같은 보정을 새 위치로 옮긴다 | |
| 8q | **seq 173 #9847 HnS B안 블록(`cc8fa0418a`):** 스마트 교체가 없는 AI의 기본 교체 유지(`// HnS:`) | seq 184 #9857, seq 311 #10542(`BATTLE_PARTNER` 1곳) 등 뒤 AI hunk | 블록 위치·`BATTLE_PARTNER` 손보기. 친구가 upstream대로(A)를 원하면 이 커밋만 되돌린다 | 친구 결정(2026-10-06): HnS 유지(되돌리지 않음). 뒤 AI hunk 때 블록만 다시 맞춘다 |
| 8r | **seq 171 #9799 HnS 보정 H2~H5 유지:** H2 `IsSwitchinValid` override 비교값 `AI_monToSwitchIntoId`(`battle_ai_switch.c`), H3 `sText_LinkTrainerSentOutPkmn2` `{B_BUFF1}`, H4 `TOWER_LINK_MULTI` 먼저, H5 상대 넣기·교체 조건의 `BattlerIsLink()`(`battle_message.c` `BufferStringBattle` 4곳) | seq 294 #10436(`INTROSENDOUT` 분기), seq 311 #10542(`IsSwitchinValid`의 `BATTLE_PARTNER` 포함) | upstream 줄로 덮지 않고 손 병합. 한글 회귀 HNS9799·K2-01로 확인 | |
| 8s | **seq 175 #8434 HnS 적응(`9d0e2f51ee`):** `wild_encounter_ow.c` 3곳(FALSE면 `UpdateOverworldWildEncounter` 첫 줄 return, 칸 없음 `return TRUE`, 배틀 시작 범위 검사), `LoadObjectEvents`의 `if (WE_OW_ENCOUNTERS)`(D4), `ComputePlayerShinyOdds`의 `GetShinyOdds()`·`FLAG_SYS_POKEDEX_GET` | seq 177 #9879(이로치 hunk 손 병합, `WE_FLAG_NO_ENCOUNTER` = HnS `FLAG_DISABLE_ENCOUNTERS`), seq 185 #9910(칸 없음 자리를 upstream 결과로, HnS 주석 삭제, `InitObjectEventStateFromTemplate` 기계어 재확인), seq 285 #10343(`updateBlend` 손 병합, 가드 유지·주석 수정), seq 680 #9970(이로치 HnS 줄 보존). 뒤 행 patch는 공백만 있는 줄 정리(`sed -E 's/^ ([ \t]+)$/ /'`) 뒤 적용 | `full-sync-seq-175-175.md` 후속 행 메모 | |

## 3. 한글 문구 (이식과 무관하게 찾은 것, 친구 확인 대상)

| # | 내용 | 위치 | 해결 |
|---|---|---|---|
| 9 | 텔레키네시스 문장 조사 고정: `{B_DEF_NAME_WITH_PREFIX}는\n높이 뛰어올랐다!` → 받침 뒤에서 `마자용는`(→ `{B_TXT_EUNNEUN}`) | `src/battle_message.c` `STRINGID_HURLEDINTOTHEAIR` (2026-10-04 턴 종료 테스트에서 발견) | |
| 10 | 섬광 문장 조사 고정: `{B_ATK_NAME_WITH_PREFIX}로부터` → 받침 뒤에서 `으로부터`여야 함 | `src/battle_message.c` `STRINGID_CLOAKEDINAHARSHLIGHT` | |
| 10b | **`{B_ATK_PREFIX2}` 뒤 조사(친구 요청 A와 같은 유형, 2026-10-04 리뷰 발견):** 주술 시작 `STRINGID_SHIELDEDFROMCRITICALHITS` `주술의 힘으로\n우리 편은의 급소가 숨겨졌다!`/`상대는의`, 팀 가드 `STRINGID_PROTECTEDTEAM` `우리 편은을\n와이드가드가 지켜 줬다!`/`상대는을`(패스트가드·마룻바닥세워막기·트릭가드 공용). 제안: 토큰만 `{B_ATK_TEAM1}의`, `{B_ATK_PREFIX3}`(+`{B_TXT_EULREUL}` 삭제) | `src/battle_message.c` 567·577 | **해결** 2026-10-05 HnS `0dd9022851`(친구 답: `우리 편의/상대의`, `우리 편을/상대를`, 토큰만 `{B_ATK_TEAM1}의`·`{B_ATK_PREFIX3}`) |
| 10c | `Time to Gigantamax!` 영문(HnS 미도달, seq 139 회귀 테스트에서 발견) | `src/battle_message.c` | |
| 10d | 파티 메뉴 "메일" 항목 문자열이 `apdlf`(한글 자판이 아닐 때 "메일"을 친 것으로 보임, seq 162 #9693 이식 중 발견). 메일을 지닌 포켓몬을 필드 파티 메뉴에서 고르면 화면에 나온다. 제안: `메일` | `src/data/party_menu.h` `MENU_MAIL` | `0276c7310f` `메일`(2026-10-06 친구 결정). `MENU_READ` `메일을 읽는다`도 같은 커밋 |
| 10e | `sText_LinkTrainer2SentOutPkmn2` 본문 끝 `!!`(4인 통신 멀티 오른쪽 상대 교체, 이식 전부터, seq 171 사전 분석에서 발견) | `src/battle_message.c` | `0276c7310f` `내보냈다!`(2026-10-06 친구 결정) |
| 10f | 유니온룸 교체 문장 형식: HnS는 이름만(`{이름}은(는) {포켓몬}을(를) 내보냈다!`), upstream #9799 형식은 직업+이름(인트로와 같은 형식). seq 171에서 HnS 출력 유지(H5). 배틀타워 통신 멀티 비마스터 화면도 같은 형식 차이(이름은 맞음). 형식만의 차이라 친구 선택(`full-sync-seq-171-174.md`) | `src/battle_message.c` `BufferStringBattle` | 친구 결정(2026-10-06): upstream 방향(직업+이름)으로 바꾼다. 다음 작업 |
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
| 25 | `TinTower_RoofDay_hns`가 `WhirlIslands_LugiaChamber_hns`의 `LOCALID_KIMONO_1`(3)·`LOCALID_KIMONO_4`(5)·`LOCALID_KIMONO_MID`(4)를 빌려 쓴다. 두 맵의 3번·5번 기모노 소녀 스크립트가 서로 반대(TinTower 3번 미키·5번 나오코, Lugia 3번 나오코·5번 미키)라 의도한 소녀가 움직이는지 미확인(2026-10-06 프런티어 로비 번호 조사 중 발견, `full-sync-hnsfix-2026-10-06.md`). 실기에서 어긋나면 HnS 번호 이름을 붙인다 | |

## 5. HnS가 upstream과 다르게 둔 곳 (뒤 PR을 이식할 때 다시 맞출 것)

| # | 내용 | 위치 | 다시 볼 때 | 해결 |
|---|---|---|---|---|
| 18 | **녹화 배틀 섹터 여유 0 B.** `RecordedBattleSave` 4,092 B = `SECTOR_COUNTER_OFFSET`(seq 138.5 #8943, A안) | `include/recorded_battle.h`, `STATIC_ASSERT(RecordedBattleSaveFreeSpace)` | `struct Pokemon`·`BATTLER_RECORD_SIZE`·`MAX_BATTLE_TRAINERS`를 바꾸는 PR. `/home/hjm0725/hns-sync-work/chunk-1385/verify/save_compat.py run` 재실행 | |
| 19 | **`struct Trainer` 비트필드 HnS 5/4**(`encounterMusic:5`, `mugshotColor:4`, upstream 1.17.0은 4/3). HnS 곡 0~26·`MUGSHOT_COLOR_LIGHT_BLUE` 때문이며 upstream 폭이면 `data.c` 빌드 실패 | `include/data.h` | `struct Trainer`를 바꾸는 PR | |
| 20 | **녹화 시작 파티 정적 저장:** `sSavedParties`를 upstream 힙 포인터 대신 정적 EWRAM 배열(+2,400 B)로 둔다. upstream 형식은 재생 때 힙 초기화로 저장 파티가 덮여 재생 뒤 플레이어 파티가 깨지고, 녹화에 전투 뒤 파티를 저장한다(1.17.0·master 같음, 미병합 `grintoul-recorded-battle-fix`) | `src/recorded_battle.c` | `recorded_battle.c`를 바꾸는 PR, upstream이 이 버그를 고칠 때 | |
| 21 | HnS 보호 수정(upstream 1.17.0에도 남은 #8943 결함): 강제 교체 오른쪽 트레이너 `70fe10ecd8`, 멀티 경험치 참가 비트 `c68e8e13ba`, 반 팀 화이트아웃 `cc0576a543`, 리뷰 후 수정 파티 번호 충돌 `8bcf557c20`(도구 대상·기술 습득·레벨업 연출·기사회생의기원·미래예지·급소 횟수·치유방울)·목호 포켓몬 도구 사용 `624ef7d4bd`(도우미 `GetItemTargetPartyOwner`)·배틀 밖 가드 `6c15064bce`. #10536 `givenExpMons` `>> 1` 보정 2곳(`93656e609b`) | `battle_script_commands.c` `Cmd_forcerandomswitch`·`NoAliveMonsForPlayer`, `battle_util.c` 참가 비트·경험치 | 같은 함수를 고치는 upstream PR이 들어오면 그쪽을 우선하고 HnS 줄을 정리 | |
| 22b | upstream 1.17.0~master 결함(고치지 않음, seq 153 #9710 리뷰): `CalcPartyMonTypeEffectivenessMultiplier`·`GetOverworldTypeEffectiveness`의 DamageContext가 `{0}`이라 방어 측 특성을 공격 측 배짱·심안 판정에도 읽는다(고스트 대 노말·격투 상성). 실전 데미지 아님 — AI 교체 판단·동행 포켓몬 감정·배틀돔 모의 판정. 특성·타입 랜더마이저에서만 닿음 | `src/battle_util.c` | upstream 수정 여부 확인. 고치면 `CanAbilityAbsorbMove`의 공격자 칸 읽기도 함께 확인 | |
| 21b | upstream 1.17.0에도 남은 소프트락 보정 `f8a465e3bc`: 교체 대기로 볼에 들어간 배틀러에게 KO 애니메이션을 걸지 않음(`AnimateMonAfterKnockout`의 `IsBattlerPresent`) | `src/battle_script_commands.c` | seq 311 #10542(`GetOppositeBattler`) 때 `IsBattlerPresent` 유지, upstream이 고치면 그쪽 우선 | |
| 22 | 1.17.0에도 남은 #8943 결함(고치지 않음): 링크 비멀티 `SetBattlePartyIds`가 컨트롤러 설정 전에 `GetBattlerParty`를 부른다(선두가 알·기절일 때만). 녹화 통신 멀티 재생에서 `BattleSideHasTwoTrainers`가 상대를 트레이너 1명으로 판정(배틀러 3이 상대 A 파티를 읽음, 실기 불가). seq 171 #9799 뒤로 4인 통신 멀티·배틀타워 통신 멀티 녹화 재생의 오른쪽 상대 넣기·교체 문장도 상대 1 이름으로 나온다(4인 통신 멀티는 인트로 `{PAUSE 49}`도 빠짐, `full-sync-seq-171-174.md`). `AreMultiPartiesFullTeams`가 `0xFFFF`(2vs1)로 HnS `gTrainers`를 읽는다(범위 밖 ROM 읽기, 크래시 없음. 프런티어 번호로 읽던 것은 `b43032bf03`에서 시설 배틀이면 읽지 않게 분리). side 기준 `itemLost[B_SIDE_PLAYER]`·`activeGimmick[GetBattlerSide]`(목호 배틀러가 플레이어 칸 표시를 건드림, 배틀 뒤 도구 복원 결과 영향 없음, 공생만 극히 드물게 다름). (레벨업 연출·체력 상자 충돌은 `8bcf557c20`으로 해결) | `battle_main.c`, `battle_controllers.c`, `battle_util.c`, `battle_move_resolution.c` | upstream 수정 여부 확인 | |
| 23 | **`B_MULTI_HALF_TEAMS FALSE`(친구 확정 2026-10-04, `FRIEND_REPLY_2026-10-04.md` 2절).** 친구가 mGBA로 배틀타워 멀티 UI·6쌍을 확인한다. 두 트레이너 동시 발견 6쌍의 4마리 트레이너가 4마리를 내고 배틀타워 멀티룸이 풀 팀 UI. TRUE면 이식 전과 같다 | `include/config/battle.h` | 친구 답에 따라 config 1줄 | |
| 24 | **이동 타입 번호:** HnS는 `MOVEMENT_TYPE_TOWER_BEAM` 0x53을 유지해 #8434의 OWE 이동 타입이 0x54~0x59, `NUM_MOVEMENT_TYPES` 0x5A로 upstream보다 1씩 크다(세이브에 저장되는 값, 영구 유지). `WE_OW_ENCOUNTERS`는 FALSE — 켜려면 #9910 뒤여야 하고 색조 회귀·켰다 끈 세이브 정리·RELEASE=0 assert·VRAM·피라미드 `_HNS` 레이아웃 등 추가 작업 필요(`full-sync-seq-175-175.md`) | `include/constants/event_object_movement.h`, `include/config/wild_encounter.h` | 뒤 OWE 행 이식 때 번호 확인. 켜는 것은 친구 결정 | 친구 결정(2026-10-06): OFF 유지. 켜기로 하면 별도 작업 |
