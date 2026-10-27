# full-sync 묶음 52 — seq 394 (#10268 Champions 배틀 메시지 후속 수정, 재확인 36 B)

- 요청: 사용자 "ㄱㄱ 394 시작해 혼자해도 되는거라면"(2026-10-27). 재확인 36은 친구가 **B(upstream 방식, 팝업 끝까지 대기)**로 결정(2026-10-26, `friend-reply-2026-10-26.md`) — 새 결정 없이 진행.
- upstream: 읽기 전용 사본, #10268 `94780b0c86`(7파일, +40/−3). 같은 단위(`U-10268`)에 다른 seq 없음. 묶음 31에서 보류할 때 세션 임시로 둔 작업분(`b32/10268-wip.patch`)은 쓰지 않고 지금 코드에 다시 3-way.
- 혼자 진행한 근거: 새 문장 0, 한글 문장 변화 0(이름 토큰만 upstream처럼 — 출력 같음, 테스트로 확인). 대기 방식은 친구 결정 B. HnS 보정 1(애슈열매 팝업 위치 — upstream 결함, 출력 위치만).

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 394 | #10268 | `5fca546a` | `waitabilitypopup` 명령(`BS_WaitAbilityPopup`), `destroyitempopup`이 화면의 팝업이 모두 사라질 때까지 대기, 열매·까칠한피부 등 15곳 끝에 `waitabilitypopup`, 랑사·애슈·미클·자보·애터열매 도구 팝업, 까칠한피부 텍스트 상자 비우기·대기, `COULDNTFULLYPROTECT` 이름 토큰 `B_DEF`, 테스트 오타 1, HnS 보정 2, 한글 회귀 테스트 | 32,961,284 |

이식 전(`73cdc0b8`, 게임 코드 = `3ab91838`) 32,961,236 B → **32,961,284 B(+48)** — 새 명령 함수, 도구 팝업 호출 5곳·대기 15곳(upstream 예상 +250 B는 1.17.0 기준).

## 내용과 HnS 판단

- **적용 방법:** 파일별 3-way 병합(`git merge-file`), 충돌은 손으로(`battle_script_commands.c` `BS_DestroyItemPopup`, `battle_interface.c` `IsAnyAbilityPopUpActive`, `battle_scripts_1.s` `BattleScript_BerryReduceAnimation`의 HnS 주석). 병합 뒤 "upstream 변경 줄 ↔ HnS 변경 줄" 대조로 남는 차이가 모두 아래 HnS 형태인지 확인.
- **재확인 36 B 반영:**
  - `destroyitempopup`: upstream처럼 `IsAnyAbilityPopUpActive()`가 참이면 그 명령에 머묾 → 모든 팝업이 사라진 뒤 `DestroyAbilityPopUp`(배틀러마다) 후 다음 명령. HnS 도구 팝업은 특성 팝업과 같은 타이머로 저절로 퇴장하므로(`f84019a062`) 그때는 닫을 팝업이 없음 — 퇴장 연출이 잘리지 않음.
  - `waitabilitypopup`: upstream 14곳 + 묶음 48(seq 483 #10431)에서 HnS가 뺀 `BattleScript_BerryReduceAnimation` 1곳(`@ HnS:` 주석을 지우고 upstream 줄로) = 15곳.
- **HnS 보정 1 — 팝업 대기 판정(재확인 104):** upstream `IsAnyAbilityPopUpActive`는 `activeAbilityPopUps` 표시만 셈. HnS는 파티·가방 화면이 스프라이트를 초기화해도 이 표시가 남을 수 있어(`f84019a062`가 `IsBattlerPopUpSpriteLive`·`ClearStaleAbilityPopUpMarks`로 다룬 경우) 표시만 세면 두 대기 명령이 끝나지 않을 수 있음(배틀 멈춤). HnS는 같은 함수를 "표시 + 그 배틀러의 가장 새 팝업 스프라이트가 실제로 화면에 있음"(`IsBattlerPopUpSpriteLive`)으로 셈. 표시가 정상인 경우 결과는 upstream과 같음.
- **HnS 보정 2 — 애슈열매 팝업 위치(upstream 결함, 재확인 103):** upstream은 `BattleScript_CustapBerryActivation`에 `BattleScript_ItemPopUp_Scripting`(스크립트 배틀러)을 넣는데, 이 스크립트를 부르는 `battle_main.c`는 그 전에 `gBattleScripting.battler`를 다음 배틀러로 옮겨 둠 — 싱글에서 팝업이 상대 쪽에 뜸(지닌 쪽 이름 문장과 어긋남). 애니·문장·`removeitem`처럼 `BS_ATTACKER`를 쓰는 `BattleScript_ItemPopUp_Attacker`로. upstream 형태로 되돌리면 새 한글 테스트 `the Custap Berry pop-up shows on its holder` 2개 FAIL, 보정으로 PASS(되돌린 것은 복구, 커밋 안 함). 1.17.1·master는 upstream 형태 그대로, `upcoming` 브랜치는 같은 수정(`call BattleScript_ItemPopUpAttacker`, 라벨 이름만 다름) — 뒤에 들어오면 그쪽 이름으로 정리.
- **`STRINGID_COULDNTFULLYPROTECT` 토큰:** HnS는 이전에 `{B_SCR_NAME_WITH_PREFIX}` + `MoveEndProtectBypassEffects`에서 `gBattleScripting.battler = cv->battlerDef`(HnS 2줄)로 대상 이름을 냈음. upstream은 `{B_DEF_NAME_WITH_PREFIX}` — MoveEnd 대상별 단계에서 `gBattlerTarget`이 현재 대상이라 같은 사람을 가리킴 → upstream 형태로(HnS 2줄 삭제). 한글 본문·조사 토큰 그대로. 더블에서 오른쪽 상대를 친 경우를 한글 테스트로 확인(이식 전 코드도 같은 출력 PASS).
- **묶음 31 질문 문안 정정:** 그때 "(3) 캄라·이바·초나·자보·애터열매에 도구 팝업 추가"라 적었는데 실제 대상은 **랑사(기합 올리기)·애슈·미클·자보·애터열매**(`BattleScript_BerryFocusEnergy`·`CustapBerryActivation`·`MicleBerryActivate`·`JabocaRowapBerryActivates`). "토큰은 HnS가 이미 `{B_DEF}`"도 틀림(위 — `B_SCR` + 배틀러 지정, 출력은 같았음). 결정(대기 여부)에는 영향 없음.
- **그 밖:** `test/battle/move_animations/all_anims.c`의 `enum move` → `enum Move`(upstream 오타 수정, 테스트 빌드만).
- 세이브: 배틀 스크립트·배틀 코드만, 세이브 구조 변화 없음.

## 화면 변화(새 문장 0, `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 3행)

1. 도구 팝업(열매·도구 전반)이 사라질 때까지 다음 명령을 기다림 — "배틀 딜레이 생략" ON(새 게임 기본)에서 도구 팝업마다 **약 +110프레임**(약 1.8초, `hnsfix-2026-10-09b.md` 계측: 등장 32 → 유지 49 → 퇴장 33; 이번에 다시 재지 않음 — 헤드리스 테스트는 팝업 스프라이트를 만들지 않음). OFF는 `pause 64` 뒤 남은 시간만큼.
2. 랑사·애슈·미클·자보·애터열매가 발동할 때 지닌 포켓몬 쪽에 도구 팝업(이전엔 팝업 없이 애니·문장).
3. 까칠한피부: 앞 문장을 비운 뒤 특성 팝업 → 잠깐 대기 → 대미지, 팝업이 사라진 뒤 다음 진행.

## 검증

- **빌드:** `make hns -j8` 종료 0, 전체 재빌드(`build/hns` 지우고) 로그로 새 컴파일 경고 0(`warncheck.sh`).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `5fca546a`): `test-baseline-seq475.8.txt` 대비 **새 PASS 5(한글 5), 사라진 PASS 0**, 다른 줄 같음(FAIL 2,304·KNOWN_FAILING 15·TO_DO 590·EXPECTED_FAIL 6 같음), INVALID 24(이전 실행과 같은 목록). 새 기준 [`test-baseline-seq394.txt`](test-baseline-seq394.txt)(**다음 비교 기준**).
- **새 한글 회귀 `test/hns_10268_popup.c` 5개(애슈 플레이어·상대, 자보·애터는 매개변수 2개씩):** 애슈열매 팝업이 지닌 쪽(플레이어·상대), 미클열매, 자보·애터열매, 랑사열매, 더블 보이지않는주먹 `상대 마자는 공격을 막아 내지 못하고 데미지를 입었다!`: **5/5 PASS**.
  - 이식 전 코드(클론 `73cdc0b8`, `ZZ old` 사본, 커밋 안 함·지움): **1/5** — 열매 4개는 팝업이 없어 FAIL = 의도한 변화, 보이지않는주먹 문장은 PASS(출력 같음).
  - 팝업 대기 자체는 헤드리스 테스트에서 팝업 스프라이트가 없어 즉시 통과 — 시간·멈춤 여부는 실기 확인 대상.
- **세이브 정적 비교**(이전 = 클론 `73cdc0b8` 빌드, ROM SHA1 `37e70fcf…` = 묶음 51 검증 기록과 같음): **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s394 8`): **555/652, `판정: PASS (기대와 같음)`, 요약 바이트 같음**(`HNSPOPUP` 32/35 그대로).
- **ROM**(커밋 `5fca546a`): SHA1 `677e531dc69fc0edbbae6a7fb19a03e13c272624`, 사용 32,961,284 B(파일 33,554,432 B), 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 하지 않음(친구).** 후보 — ① 열매 팝업(오랭·시몬·자뭉·리샘·랑사·애슈·미클·자보·애터열매 등)이 지닌 쪽에 뜨고 퇴장까지 끝난 뒤 진행, ② "배틀 딜레이 생략" ON·OFF에서 도구 팝업 대기 템포, ③ 까칠한피부·울퉁불퉁멧, ④ 열매를 지닌 포켓몬으로 교체·가방 사용 뒤 팝업이 뜨는 장면에서 배틀이 멈추지 않는지(보정 1), ⑤ 더블에서 보이지않는주먹이 방어를 뚫을 때 문장의 대상 이름.

## 다음

- seq 485 #10488(Assurance inconsistencies), 486 #10566(재확인 97 정리). 목표 1.17.1.
