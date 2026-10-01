# seq 127 #9655 사전 분석 — 공통 지시 (읽기 전용 분석 에이전트용)

## 배경

- 저장소: `/home/jinmo/pokehns-expansion-kor` (브랜치 `pokehns-expansion-kor`, HEAD `fcf855d4e8`). HnS(Heart&Soul) 한글화 프로젝트다. 루트 `AGENTS.md`를 먼저 읽는다.
- upstream: `/home/jinmo/pokeemerald-expansion-upstream` (읽기만).
- 이식 대상 unit `U-battlemsg-9655`(full-sync seq 127):
  - #9655 `32fcd64868` Refactor Battle Messages (XL) — patch `chunk-127/9655.patch`
  - #9856 `c1eaced09e` Fix contact damage message printing (seq 174, 같은 unit) — `9856.patch`
  - #10064 `3ed1ce5570` Fix already-status messages using the wrong battler (seq 206, 같은 unit, 엔진 1줄만) — `10064.patch`
  - #10149 는 "HnS 동등"(적용하지 않음, g1 plan)
- 현재 HEAD 기준 dry-run 결과: `dry-9655.txt`, `dry-9856.txt`, `dry-10064.txt`. 코드 hunk 112개 중 47개 실패(`battle_message.c` 24/30, `battle_scripts_1.s` 13/35, `battle_string_ids.h` 3/9, `battle_script_commands.c` 5/17, `battle_hold_effects.c` 1/6, `battle_scripts.h` 1/4). 테스트 hunk 408개 중 16개 실패(4파일). #9856은 실패(HnS에는 upstream이 만든 중복 줄이 없음), #10064는 성공.
- 작업 폴더: `/home/jinmo/hns-sync-work/chunk-127/` (아래 `$W`).

## 반드시 읽을 자료

1. `docs/friend-handoff/FRIEND_REPLY_2026-10-01.md` — **친구 결정(D1~D7, 함정 준수). 최우선 기준.**
2. `docs/friend-handoff/results/1.17.0-port/pre-9655/REPORT.md` 와 같은 폴더의 `impact_stringids.tsv`(112행), `new_sentences.tsv`, `output_changes_overlap.tsv`
   - 이 조사는 `17aa03192f` 기준이다. 그 뒤 seq 120~126과 선진행(seq 130~165)이 `battle_util.c`(171줄), `pokemon.c`(108줄 삭제), `battle_script_commands.c`(34줄), `battle_end_turn.c`(14줄), `battle_scripts_1.s`(6줄), `battle_message.c`(2줄)를 바꿨다. 행 번호와 문맥은 현재 HEAD로 다시 확인한다.
3. `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md` — HnS가 사용자 요청으로 바꾼 출력. 보존 대상.
4. `docs/friend-handoff/results/1.17.0-sync-plan/g6_overworld_refactors_plan.tsv`의 `9655` 행(approach), `g1_battle_fixed_a_plan.tsv`의 `9856`·`10064`·`10149` 행
5. `docs/friend-handoff/results/1.17.0-port/PORT_INSTRUCTIONS.md` — 보존 규칙(한글 문자열·인코딩·STRINGID 매핑·`{B_...}`·조사 토큰·HnS 출력 정책·HnS config)

## 확정 결정 (친구, 2026-10-01)

- **D1 C안:** `STRINGID_ATTACKMISSED` 삭제(upstream). HnS `resultmessage` 빗나감은 upstream의 `STRINGID_PKMNEVADEDATTACK` 대신 **`STRINGID_PKMNAVOIDEDATTACK`**을 쓴다(1줄, `// HnS:` 주석). `gMissStringIds[B_MSG_MISSED]`는 upstream대로 `PKMNAVOIDEDATTACK`. 모든 빗나감이 기존 한글 `…에게는\n맞지 않았다!`로 나온다.
- **D2 A안:** `STRINGID_PKMNSXMADEYINEFFECTIVE`의 HnS 한글(`…에게는\n효과가 없는 것 같다...`)을 유지. upstream이 이 ID를 "도구를 뺏을 수 없다" 용도로 써도 문장은 바꾸지 않는다. 타오르는불꽃 매핑도 유지(#10149 동등).
- **D3 A안:** 도주 특성 도망은 HnS `무사히 도망쳤다\p` 유지. 새 번역(느낌표 등) 금지.
- **D4:** upstream 출력 변화 17건(REPORT "upstream이 바꾼 출력" 표)을 받아들인다. **기존 한글 문장을 최신 영문에 맞춰 다시 번역하지 않는다.** 구조·출력 경로 변화만 반영하고 `BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 전부 기록한다.
- **D5:** 새 ID 14개는 `new_sentences.tsv`의 기존 한글 본문을 다시 쓴다. 일반 마비 회복은 `몸저림이 풀렸다!` 계열.
- **D6:** a(방벽 해제 진영 `{B_ATK_PREFIX1}`)는 **이번에 고치지 않는다**(실기 확인 뒤). b(멘탈허브의 사슬묶기·앙코르 해제 문장 이름)는 **문장을 바꾸지 않고 스크립트에서 실제 대상 이름이 나오도록 고친다.** c(사령탑 두 번째 이름)는 별도 과제라 건드리지 않는다.
- **D7:** `STRINGID_PKMNWOKEUPINUPROAR`의 `{B_ATK_NAME_WITH_PREFIX}` → `{B_EFF_NAME_WITH_PREFIX}`. 한글 본문·조사는 그대로.
- **REPORT 함정 준수:**
  1. HnS 전용 `BattleScript_MentalHerbCureFling`과 Ret 스크립트 모두 비트마스크 방식(`CMP_BITMASK` 분기)으로 대응
  2. HnS `B_MSG_BREAK_*`(값형 1,2,4)는 `CMP_COMMON_BITS` 유지. upstream 인덱스형과 섞지 않음
  3. `CureStatusBerryEffectStringID`의 PROBLEM/NORMALIZED 유지(`GetCuredStatusMessage()`가 씀). `FREEEZE` 오타 교정은 헤더·코드·표 전부 일관되게
  4. `{B_SCR}` 전환 경로에서 `gBattleScripting.battler` 설정 확인
  5. HnS 전용 스크립트 보존: HealerActivates, DampPreventsAftermath, RanAwayUsingMonAbility, LumBerryCureStatusRet, BreakScreens, 포이즌힐·솔라파워 무문구, ItemPopUp
  6. #9856: `BattleScript_HurtAttacker`에 upstream의 중복 `printstring`을 넣지 않는다
- g6 plan: HnS가 이미 같은 이름으로 넣은 방벽 제거(`B_MSG_BREAK_*`, `BattleScript_BreakScreens*`, `STRINGID_*WOREOFF`)는 중복 추가하지 않는다. 한국어 어순용 `gText_StatSharply`("크게 ")·drastically는 upstream의 영어 접미 이동을 따르지 않는다.
- 새 번역 금지 범위: `STRINGID_STICKYWEBDISAPPEAREDFROMYOU`는 넣지 않는 것이 권장(#9777 seq 475에서 삭제됨, HnS는 이미 TEAM 문장 출력) — 넣지 않으려면 그 분기를 어떻게 처리할지 명시. 멸망의바디 `PKMNSWILLPERISHIN3TURNS` 영문은 지금 고치지 않는다(별건, 기록만). `new_sentences.tsv`의 "(선택)" 3건은 하지 않는다.

## 규칙

- **메인 저장소를 수정하지 않는다.** 메인 저장소에서 쓰기 git 명령(add/commit/checkout/stash/reset/apply 등) 금지. 읽기(`git show`, `git log`, `git blame`, `git grep`, 파일 읽기)는 자유.
- **make·빌드·테스트를 실행하지 않는다.** 이 노트북은 메모리 7 GB이고 메인이 전체 테스트를 돌리는 중이다. 컴파일 확인이 필요하면 메모로 남긴다.
- 편집은 자기 파트의 스크래치 git 저장소 `$W/part-X/tree/`(HEAD `fcf855d4e8`의 해당 파일 사본, 첫 커밋 있음)에서만 한다. 경로는 저장소 상대 경로와 같다. 다른 파트 파일이 필요하면 같은 폴더에 `git show HEAD:path`로 추가해도 되지만, **자기 파트 담당 파일 밖의 변경은 patch에 넣지 말고 md의 "다른 파트에 필요한 것"에 적는다.**
- 결과물:
  - `$W/part-X.patch` = `git -C $W/part-X/tree diff` (메인 저장소에 `git apply`로 바로 적용 가능해야 한다. 마지막에 `git -C /home/jinmo/pokehns-expansion-kor apply --check $W/part-X.patch`로 확인한다 — `--check`는 쓰기가 아니다)
  - `$W/part-X.md`: 아래 항목
    1. upstream hunk 표: 파일·hunk 번호·내용 요약·판정(그대로 / 수정해서 / 제외)·이유
    2. HnS 보존·적응 내용(`// HnS:` 주석을 단 곳 목록)
    3. **다른 파트와의 계약**: 이 patch가 정의·제공하는 심볼(STRINGID, 표, 스크립트 라벨, enum 값)과 다른 파트에서 있다고 가정하는 심볼
    4. 한글이 든 줄 변경 전부(전/후). 새 문자열은 바이트 길이·줄바꿈·토큰 점검 결과
    5. 출력 변화(이전 → 이후)와 그것이 D4 17건 중 몇 번인지, 또는 새로 생긴 것인지
    6. 위험·미해결 질문, 실기 확인 후보
- 판단이 결정 범위를 넘으면(새 HnS 정책, 새 번역, 한글 문장 수정) 고치지 말고 md의 "질문"에 적는다.
- 최종 응답은 30줄 이내: 산출물 경로, hunk 집계, 계약 요약, 질문.
