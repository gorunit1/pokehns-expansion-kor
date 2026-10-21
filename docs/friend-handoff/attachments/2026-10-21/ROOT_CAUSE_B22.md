# 친밀도·포켓러스 선택 대상 오류 원인 분석 — 2026-10-15-003

두 FAIL은 **같은 원인**이다. 포켓몬 선택 화면은 내부 슬롯 2를 정상 전달한다. 각 기능의 입력창 초기화가 그 번호를 0으로 덮어써, A 확정 때 첫 번째 포켓몬을 수정한다. 현재 검사 ROM과 최신 원격 소스 모두에 이 코드가 있다.

이번 작업은 읽기 분석이다. 게임 코드 수정, 새 ROM 빌드, mGBA 재실행, Git 갱신·통합·push를 하지 않았다. 원본 QA·DEBUG_REPRO·AUTOMATION_RESULTS·분류·기존 판정은 변경하지 않는다. 아래 수정안은 제안이며 적용하지 않았다.

## 1. 검사 대상과 실제 증거 대조

- 검사 ROM SHA256: `ab3d858c6aada3aeebe1cbd12ac366ad46a9e9f46c38c2c3865c7f368706990e`.
- 대응 ELF: `601aa471ed5c131f1b2d944909a77fbd1e53912088a7b9ed01f63270f89da931`.
- 로컬 HEAD: `c4c521175c80470d843e44c528fcb8d8c87acc21`.
- 현재 src/debug.c·party_menu.c·pokemon.c·data/scripts/debug.inc의 SHA는 002 실행 프로필과 같다. 지정 핵심 소스 5개는 `git show HEAD:<path>`와 실제 파일 바이트가 동일하다. 지정 경로 `git diff`는 30초 타임아웃이어서 전체 작업 트리를 확인했다고 주장하지 않는다.
- 002의 원시 FAIL result/메모리/버튼/PNG는 그대로 읽었다. 신규 실기 결과를 만들지 않았다.

| 항목 | 선택/전달 | A 확정 후 첫 번째 | A 확정 후 세 번째 | 근거 |
| --- | --- | --- | --- | --- |
| b22-06 친밀도 | 화면 slot2 / VAR_0x8004=2 / 입력 Task.data[6]=0 | 84→120 | 97→97 | 6마리 UI 재조회와 복호화/체크섬/100바이트 비교가 일치 |
| b22-05 포켓러스 | 화면 slot2 / VAR_0x8004=2 / 입력 Task.data[6]=0 | 0/0→5/2 (`0x52`) | 0/0→0/0 | 균주 A 중간 첫 번째 `0x50`, 일수 A 후 `0x52`; 균주 재조회 및 정확한 일수 메모리 일치 |

나머지 4마리도 각각 100바이트 그대로다. 첫 A 전 입력창 B 취소에서는 slot2/slot1 모두 전체 파티가 보존됐다. 따라서 선택 화면·검사기의 버튼 타깃·파티 복호화가 틀려서 생긴 FAIL이 아니다. 초기값을 읽을 때까지는 선택 슬롯이 맞고, 실제 쓰기에 사용할 슬롯만 초기화 뒤 달라진다.

## 2. 메뉴 → 선택 → 전달 → 쓰기 경로

공통 경로(현재 test 소스의 실제 줄 번호):

1. `src/debug.c:661`의 Edit Pokemon 배열(연결 항목666)은 Set Friendship을 `Debug_EventScript_SetFriendship`에 연결한다. `src/debug.c:671`의 Party 배열(연결 항목679)은 Give Pokerus를 `Debug_EventScript_GivePokerus`에 연결한다.
2. `DebugAction_ExecuteScript`(1581)는 `Debug_DestroyMenu_Full_Script`(1059)를 통해 메뉴를 정리하고 `ScriptContext_SetupScript(script)`로 스크립트를 시작한다.
3. `data/scripts/debug.inc:596/603`의 두 스크립트는 `special ChoosePartyMon` → `waitstate` → 각 `callnative`를 실행한다.
4. `src/party_menu.c:8328`의 `ChoosePartyMon`이 `Task_ChoosePartyMon`(8335)을 만든다. 이 Task는 `PARTY_ACTION_CHOOSE_AND_CLOSE`, `Task_HandleChooseMonInput`, `BufferMonSelection`으로 실제 파티 선택 화면을 연다.
5. `Task_HandleChooseMonInput`(1476)이 A를 받으면 `HandleChooseMonSelection`의 CHOOSE_AND_CLOSE 경로(1584)로 창을 닫는다. 종료 callback은 `BufferMonSelection`(8274)이다.
6. `BufferMonSelection`은 `GetCursorSelectionMonId()`(1466), 즉 `gPartyMenu.slotId`를 `gSpecialVar_0x8004`에 넣는다. 세 번째는 0 기반 번호 2다. 취소의 범위 밖 값은 `PARTY_NOTHING_CHOSEN=0xFF`로 정규화한다. 필드 fade 종료 Task(8290 부근)가 `ScriptContext_Enable()`로 `waitstate`를 재개한다.
7. 두 native 진입 함수는 선택값이 `PARTY_SIZE`보다 작은지 확인한 뒤 Task를 만들고 **선택값을 Task.data[6]에 복사**한다. `CreateTask`의 데이터 0 초기화는 이 복사보다 먼저 끝난다. 일반 선택 처리나 Task 생성 자체가 잘못된 것이 아니다.

### 친밀도

- `DebugNative_Party_SetFriendship`(5809): `gTasks[taskId].tPartyId = gSpecialVar_0x8004;` → 2 저장.
- `DebugNativeStep_Party_SetFriendshipMain`(5793): 해당 슬롯의 친밀도 97을 먼저 읽고 표시한 뒤 **5806의 `gTasks[taskId].tPartyId = 0;`**으로 슬롯을 바꾼다.
- `DebugNativeStep_Party_SetFriendshipSelect`(5772): A가 오면 `&gParties[B_TRAINER_PLAYER][gTasks[taskId].tPartyId]`를 `SetMonData(..., MON_DATA_FRIENDSHIP, &tInput)`에 전달한다. 따라서 첫 번째에 120을 저장한다.

이 때문에 화면에는 선택한 세 번째의 초기값이 정상 표시되지만, 확정 대상은 첫 번째다. 세 번째를 다시 조회할 때도 새로운 Task가 처음에는 2를 받아 읽으므로 97이 표시된다.

### 포켓러스

- `DebugNative_Party_SetPokerus`(5911): 같은 방식으로 슬롯 2 저장.
- `DebugNativeStep_Party_SetPokerusMain`(5895): 선택 개체의 균주를 읽고 표시한 뒤 **5908의 `gTasks[taskId].tPartyId = 0;`**으로 슬롯을 바꾼다.
- `DebugNativeStep_Party_SetPokerusStrainSelect`(5870): A가 균주 5를 첫 번째에 저장한다. 이어서 일수 초기값도 같은 잘못된 첫 번째에서 읽는다.
- `DebugNativeStep_Party_SetPokerusDaysLeftSelect`(5837): A가 일수 2를 같은 첫 번째에 저장한다.

`src/pokemon.c:4518`의 SetMonData는 해당 필드를 SetBoxMonData에 넘긴다. 친밀도는 growth substruct의 friendship 1바이트(4633), 포켓러스는 misc substruct의 한 바이트를 고친다. 균주는 상위 4비트(4699), 일수는 하위 4비트(4702)다. 필드 저장 함수는 전달받은 개체에 요청한 값을 쓰고 있다. 잘못된 부분은 그 함수에 넘기는 포켓몬 주소다.

## 3. 원인의 공통성·영향 범위

현재 `tPartyId=data[6]`, `tFriendship=data[7]`, `tStrain=data[7]`이다. 친밀도와 포켓러스는 별도 Task이며, 현재 포켓러스의 슬롯 6과 균주 7은 서로 겹치지 않는다. 이번 두 FAIL을 tStrain 충돌이나 세이브 구조·체크섬 문제로 설명하면 틀린다. 각각의 Main에 남은 동일한 슬롯 0 초기화가 원인이다.

실제 확인은 세 번째 선택 표본이다. 소스상 영향은 2~6번째 유효 개체(내부 슬롯 1~5)의 두 디버그 편집으로 확대된다. 첫 번째 선택은 슬롯 0이 이미 맞아서 문제가 가려진다. 포켓러스는 균주·일수 쓰기뿐 아니라 균주 확정 후 일수 초기 조회도 잘못된 첫 번째를 사용한다.

일반 플레이의 친밀도 증가·포켓러스 감염 전체, 일반 SetMonData, 모든 파티/NPC 선택이 실패한다고 확대하지 않는다. 이 분석은 디버그 기능의 선택 대상 경로다.

현재 ROM의 실제 Main 기계어도 대조했다. 두 함수 모두 Task.data[6]을 읽은 뒤 같은 위치(구조체 시작+20)에 0을 저장한다.

- 친밀도: `0x0810af78: strh r3, [r6, #20]` (r3=0).
- 포켓러스: `0x0810dbea: strh r3, [r6, #20]` (r3=0).

해당 ROM 바이트 SHA가 002 런타임 검사 프로필 fingerprint와 일치한다. 증거: `tested-binary.json`. 새로 컴파일한 결과가 아니다.

## 4. 같은 선택 함수를 쓰는 다른 디버그 기능

`data/scripts/debug.inc`의 `special ChoosePartyMon` 호출 **9개를 전부** 조사했다. 표의 결론은 소스 감사이며, 추가 실기 PASS/FAIL이 아니다.

| 기능 | 선택값 사용·취소 방어 | 조사 결과 |
| --- | --- | --- |
| Check EVs (163) | >=PARTY_SIZE면 종료, Script_GetChosenMon*EVs가 VAR_0x8004로 조회 | 동일 0 덮어쓰기 없음; 실기 미검사 |
| Check IVs (184) | 같은 방어와 직접 조회 | 동일 0 덮어쓰기 없음; 실기 미검사 |
| Hatch an Egg (377) | 범위 검사 후 알 여부 확인·선택값 전달 | 동일 0 덮어쓰기 없음; 실기 미검사 |
| Inflict Status1 Single (507) | 범위·알 검사 후 setstatus1에 VAR_0x8004 전달 | 동일 0 덮어쓰기 없음; 실기 미검사 |
| Faint Pokemon (533) | 범위·알 검사 후 setko에 VAR_0x8004 전달 | 동일 0 덮어쓰기 없음; 실기 미검사 |
| Set Hidden Nature (543) | 범위·알 검사 후 SetHiddenNature가 VAR_0x8004로 수정 | 동일 0 덮어쓰기 없음; 실기 미검사 |
| Set Ability (581) | DebugNative_GetAbilityNames·SetAbility가 VAR_0x8004 직접 사용 | 정상 선택 경로에 동일 0 덮어쓰기 없음. **별도 취소값 검사 누락 위험** |
| Set Friendship (596) | native 진입의 <PARTY_SIZE 방어 후 Task에 전달 | Main에서 0 덮어쓰기, 기존 실제 FAIL |
| Give Pokerus (603) | 같은 native 방어와 전달 | Main에서 0 덮어쓰기, 기존 실제 FAIL |

**추가 발견 — 특성 변경의 별도 위험:** Set Ability 스크립트에는 선택 취소의 `VAR_0x8004 >= PARTY_SIZE` 검사 없이 `DebugNative_GetAbilityNames` 호출이 이어진다. 이 함수(5752)도 직접 `gParties[player][VAR_0x8004]`를 읽는다. 취소값 0xFF가 들어오면 파티 범위 밖 주소를 읽을 수 있다. 잘못된 선택을 그대로 계속 확정하면 SetAbility(src/field_specials.c:4683)도 범위 검사 없이 쓴다. 이는 슬롯을 0으로 만드는 두 FAIL과 **다른 원인**이며, 이번에는 실기 재현하지 않았으므로 확정 게임 FAIL·크래시·세이브 손상으로 판정하지 않는다.

특성 취소 위험을 별도로 수정할 경우에는 `data/scripts/debug.inc`의 SetAbility에서 `waitstate` 다음, getter 호출 전에 기존 SetAbilityClose로 가는 범위 검사를 넣는 방안이 작다. 공용 ChoosePartyMon이나 SetAbility 전체를 임의 변경하지 않는다. 이 별도 수정 역시 제안이며 승인/적용한 것이 아니다.

참고: Party 직접 Move Relearner는 Common 스크립트를 호출하며 이번 9개 Debug.inc 호출과 경로가 다르다. 기존 NPC 떠올리기 PASS도 이 두 Task 문제와 자동으로 동일시하지 않는다. Clear Pokerus는 의도적으로 전체 파티를 순회하는 기능이며 선택 한 개 보존 검사 대상으로 바꾸지 않는다.

## 5. 권장 최소 수정안 — 파일 1개, 삭제 2줄

| 수정 파일 | 함수 | 변경 |
| --- | --- | --- |
| src/debug.c | DebugNativeStep_Party_SetFriendshipMain | `gTasks[taskId].tPartyId = 0;` 삭제 |
| src/debug.c | DebugNativeStep_Party_SetPokerusMain | 같은 줄 삭제 |

native 진입 함수가 이미 유효 선택값을 Task에 저장한다. 그 값을 끝까지 유지하면 모든 A 쓰기와 포켓러스 일수 읽기가 선택 개체를 사용한다. Main에서 전역 VAR_0x8004를 다시 읽는 변경보다 기존 Task의 선택 번호를 보존하는 편이 작고 명확하다.

`tInput=data[3]`, `tDigit=data[4]`, `tPartyId=data[6]`, `tFriendship/tStrain=data[7]`을 유지한다. 특히 균주를 data[6]으로 되돌리면 슬롯과 충돌한다. 숫자 범위·A 즉시 확정·B 복귀·세이브 구조·친밀도/포켓러스 효과·번역을 바꿀 이유가 없다. 공용 파티 선택이나 src/pokemon.c 수정도 필요 없다.

[적용하지 않은 최소 diff](../../evidence/qa-root-cause-2026-10-15-003/PROPOSED_ONLY-minimal.patch)는 로컬 소스를 기준으로 만든 검토용 텍스트다. 최신 원격에서는 줄 번호가 +9 이동하지만 두 함수 내용은 동일하다. 이 파일을 게임 저장소에 적용하지 않았다.

## 6. 최신 원격 확인 — 이미 해결됐는가

현재 작업 브랜치는 `gorunit1/pokehns-expansion-kor` 저장소의 **pokehns-expansion-kor**다. 기본 master를 현재 full-sync 코드로 간주하지 않았다.

- 로컬/로컬 추적 ref: c4c521175c… (갱신하지 않음).
- 실제 GitHub GET로 확인하고 끝에 재확인한 원격 작업 HEAD: **d4cc5b4e1150c325ae5b6c4f7d242b8491655c28**, 2026-10-14 11:21:41 UTC, 묶음 23 검증 완료 커밋.
- 두 SHA 비교: 원격 +13커밋, 뒤처짐 0, 변경 파일 67개.
- src/debug.c 차이는 시간 메뉴의 Redo daily events 등 **9줄 추가**다. 문제 두 Main·두 native 진입·세 Select·특성 getter까지 **8개 함수 내용이 정확히 같다**.
- remote friendship reset: 5815; remote pokerus reset: 5917. [친밀도 원격 소스](https://github.com/gorunit1/pokehns-expansion-kor/blob/d4cc5b4e1150c325ae5b6c4f7d242b8491655c28/src/debug.c#L5802-L5816), [포켓러스 원격 소스](https://github.com/gorunit1/pokehns-expansion-kor/blob/d4cc5b4e1150c325ae5b6c4f7d242b8491655c28/src/debug.c#L5904-L5918).
- data/scripts/debug.inc는 동일 blob이고 src/party_menu.c·field_specials.c·pokemon.c는 비교 변경 목록에 없다. 변경된 script_pokemon_util.c의 1줄은 Tera Orb 충전 검사로, 조사한 선택·EV/IV·상태 처리와 무관하다.

따라서 **확인한 최신 원격 소스에도 미해결**이다. 원격 최신 ROM을 받거나 빌드·실기 검사한 것이 아니므로 해당 ROM에 새 실기 FAIL을 부여하지 않는다.

추가로 git 객체를 읽어 묶음 21의 49f2c5e5에서도 두 Main의 0 초기화가 있었음을 확인했다. 이때 tPartyId=data[5]/tStrain=data[6]이었다. 7c17b9f1에서 partyId가6으로 이동하며 strain6과 겹쳤던 별도 충돌은 현재 strain7로 분리됐지만 0 초기화는 남았다. **현재 두 실제 FAIL은 오래된 선택 보존 오류**이며 새 묶음22/23에서 처음 생겼다고 단정하지 않는다. 과거 ROM 실기는 하지 않았으므로 당시 실제 재현 여부까지 소스 결과로 대체하지 않는다. 증거: history.json.

## 7. 수정 승인·반영 후 재검사할 항목

아래는 재검사 계획이다. 이번 작업에서는 실행하지 않는다.

1. 새 ROM/ELF/map의 해시·동일 빌드·해당 함수 및 Task ABI를 대조하고, 이전 ROM용 state/cache를 결과로 재사용하지 않는다. 기존 준비기는 ROM 해시가 바뀌면 fixture를 다시 생성한다.
2. **b22-06:** 세 번째 선택→초기값→120 A→B→재조회. Task.data[6]이2로 유지되고 세 번째만120, 첫 번째와 나머지 전체100바이트가 그대로인지 확인한다. 첫 번째 선택도 정상인지, 파티 개수에 맞춰 둘째·여섯째 등 비0 슬롯으로 확대한다. 첫 A 전 B 취소와 선택 화면 B 취소에서는 전체 파티 불변을 확인한다.
3. **b22-05:** 세 번째→균주5 A→일수2 A. 중간·최종 스냅샷에서 세 번째만0x5?→0x52이고 첫 번째/나머지는 불변인지 확인한다. 균주·일수 초기값을 서로 다르게 준비해 일수 창이 선택 개체 값을 읽는지 확인한다. 둘째·마지막 유효 슬롯과 숫자 경계도 별도 확인한다.
4. 포켓러스 일수 B는 균주 화면으로 돌아오는 현재 동작을 유지한다. 이미 균주 A로 확정한 변경이 일수 B 때문에 롤백된다고 기대하지 않는다. 확정 전 B 취소와 확정 후 단계 복귀를 구분한다. 친밀도도 A 확정 후 B는 창 종료이며 취소 롤백이 아니다.
5. 특성 변경은 별도 사본에서 선택 화면 B/취소 선택, 정상 첫째·셋째, 특성 메뉴 B를 확인한다. 아직 확정 FAIL로 등록하거나 무단 수정하지 않는다. EV/IV·성격·상태·기절·알 선택도 필요 범위를 별도 실기로 확인한다. 분석상 같은 오류가 없다는 이유로 PASS하지 않는다.
6. 기존 **PASS15 + 수정한 두 항목 = 17개**를 실제 Windows 전체 회귀로 실행한다. 두 항목은 새 실제 PASS/증거를 확보한 뒤에만 PassedOnly에 추가한다. 그때 성공 대상이17개가 된다. 2-5-26은 공식 종·폼 그림/타입 기준 전 계속 INCONCLUSIVE다.

## 기록·보존

[세션 기록](../../sessions/2026-10-15-003-qa-root-cause.md), [분석 증거](../../evidence/qa-root-cause-2026-10-15-003/analysis-summary.json). 원격 소스·Git blob SHA 검증·67파일 비교·8함수 동일성·현재 ROM 기계어·9개 호출 감사·과거 코드·원본21개 해시를 보존한다. 이번 조사로 기존 PASS15/FAIL2/2-5-26 INCONCLUSIVE를 바꾸지 않았다. 원격을 보기 위해 fetch/pull하지 않았으며 메인 저장소 조작이나 친구에게 메시지 발송도 없다.
