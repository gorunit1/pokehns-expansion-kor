# full-sync 묶음 44 — seq 477 (#10311 `end2` 삭제 — `end`로 통합)

- 요청: 사용자 "ㄱㄱ 477 시작해 혼자해도 되는거라면"(2026-10-25).
- upstream: 읽기 전용 사본, #10311 `c8ea079e83`(12파일 — 스크립트·명령·아레나·튜토리얼 문서, 테스트 변경 없음). kor=Y(결과 문서 132 메모: HnS `End2` 라벨 이름 정리).

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 477 | #10311 | `63b6a114` | `end2` 명령 삭제(스크립트 43곳 `end`로), `Trainer{A,B,Partner}SlideMsgEnd2` → `…End`, 대타출동꼬리·게으름·불복종(낮잠·잠든 채 무시)·팰리스 사용 불가 기술 스크립트가 `BattleScript_MoveEnd`로 끝남, 아레나 기술 점수를 `Cmd_end` → `MoveEndClearBits`, 한글 회귀 테스트 | 32,963,380 |

이식 전(`a8f6dfb8`, 코드 = `0fed230b`) 32,963,588 B → **32,963,380 B(−208)**.

## 내용과 HnS 판단

- **`end2` → `end`:** `battle_scripts_1.s` 34곳·`battle_scripts_2.s` 9곳(첫 턴 `ArenaTurnBeginning`·`PalacePrintFlavorText` 래퍼, 경험치·기절 처리, 트레이너 슬라이드 등). 두 명령의 차이는 `end`의 아레나 점수 한 줄뿐이었다. 결과 문서 132 메모의 HnS 전용 `End2` 라벨은 이전 seq에서 이미 정리돼 이번엔 트레이너 슬라이드 3개만 남아 있었음(`grep` — 배틀 스크립트·C 코드에 `end2`/`End2` 0).
- **명령 번호:** `B_SCR_OP_END2` 삭제. upstream은 같은 커밋에서 끝에 `UNUSED_39`를 더하지만 HnS는 #10404(seq 427) 때 이미 `UNUSED_39`를 두었으므로 새로 더하지 않음(번호는 enum 이름으로만 쓰여 개수 차이는 영향 없음, 세이브 무관).
- **MoveEnd 전체로 끝나게 된 경로(upstream과 같음):** 대타출동꼬리(교체 뒤 `BattleScript_MoveEnd`), 게으름·불복종 낮잠·잠든 채 무시·팰리스 사용 불가(`moveendto MOVEEND_NEXT_TARGET` + `end` → `goto BattleScript_MoveEnd`). 화면 문장·순서는 같음(새 한글 테스트로 이식 전 코드와 비교).
  - **동작 변화 1개:** 위 실패 턴도 `MOVEEND_CLEAR_BITS`까지 가서 **분함의발구르기 타이머**가 세워진다 → 게으름으로 쉰 다음 턴(또는 불복종으로 낮잠·잠든 채 무시·게으름 피운 다음 턴)의 분함의발구르기 위력 2배. 마비·풀죽음 실패와 같은 취급이고 Showdown 구현(게으름 턴 = 실패로 침)과 같은 방향. 이식 전 코드에서는 2배가 아니었음(새 테스트로 확인 — 145 → 145, 이식 뒤 145 → 290). HnS 일반 진행에서 보이는 차이라 친구에게 알림(되돌리길 원하면 말해 달라고).
- **아레나 기술 점수(HnS 적응):** upstream은 `BattleArena_AddSkillPoints`를 기술 끝(`MoveEndClearBits`)으로 옮기면서 `gBattlerTarget` 하나 대신 모든 배틀러를 돌게 바꿨는데, 공격자 자신의 칸도 세어(결과 플래그 0 → +1) **모든 기술에 +1이 더 붙고**, 방어 검사가 공격자 → 대상으로 바뀌어 방어(지키기)를 쓴 턴도 +1이 된다. HnS는 자신의 칸을 건너뛰고 방어 검사를 공격자에 둬 **기술당 점수를 이식 전과 같게** 유지(`src/battle_arena.c` `// HnS:` 주석). 재확인 95 — upstream 방식으로 바꿀지 친구 질문.
- `Cmd_end`의 assert 문구·`Cmd_return`·`Cmd_end3` 문구는 upstream대로(`end/end3`, `end`). 튜토리얼 문서 `docs/tutorials/how_to_new_trainer_slide.md`도 upstream대로(라벨 이름).

## 검증

- **빌드:** `make hns -j8` 종료 0, 새 경고 0(`warncheck.sh`).
- **전체 테스트**(아레나 적응·새 테스트 넣기 전 작업 트리): 목록이 `test-baseline-seq476.txt`와 **바이트 같음**(새 PASS 0, 사라진 PASS 0).
- **새 한글 회귀 `test/hns_10311_end.c` 5개** — 게으름(쉬는 턴 팝업·문장, 다음 턴 공격), 게으름 뒤 분함의발구르기 2배, 불복종 낮잠, 잠든 채 명령 무시(코골기), 대타출동꼬리(교체 뒤 대타가 공격을 받음): **5/5 PASS**. 이식 전 코드(클론 `a8f6dfb8`)에서는 **4/5** — 분함의발구르기 2배만 FAIL(위 동작 변화), 나머지 화면 출력 같음.
- **전체 테스트**(커밋 `63b6a114`): `test-baseline-seq476.txt` 대비 새 PASS 5(새 한글 테스트)뿐, 사라진 PASS 0, 다른 줄 0. 새 기준 [`test-baseline-seq477.txt`](test-baseline-seq477.txt)(**다음 비교 기준**).
- **세이브 정적 비교**(이전 = 코드 `a8f6dfb8`, 클론 빌드): **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`kortests/run.sh <저장소> kor-b58 8`, 커밋 `63b6a114`): **558/652, `판정: PASS (기대와 같음)`**.
- **ROM**(커밋 `63b6a114`): SHA1 `78d51dd498c6c5f534a3666e66f382274b868eac`, 사용 32,963,380 B(파일 33,554,432 B), 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **아레나 점수:** 코드 감사만(테스트 러너가 배틀아레나 배틀을 돌리지 않음). 실기 후보.
- **실기: 하지 않음(친구).** 후보 — 게으름(게을킹) 쉬는 턴 뒤 분함의발구르기, 대타출동꼬리 교체, 불복종 낮잠, 배틀아레나 3턴 판정(기술 점수).

## 다음

- seq 478 #10300(Refactor dynamic move category and remove Present script).
