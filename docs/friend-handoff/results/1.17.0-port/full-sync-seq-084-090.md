# full-sync 실제 port 결과 — seq 84~90

진행 중: 마지막 완료 seq 84, 다음 seq 85

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), [`CLAUDE_FULL_SYNC_PORT_PROMPT.md`](../../CLAUDE_FULL_SYNC_PORT_PROMPT.md)
시작 HEAD: `79af946ddf`

## seq 84~90 공통 사항

- 빌드 명령: `PATH=/opt/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin:$PATH GITHUB_ACTION=1 make hns -j8 > build/port.log 2>&1` (노트북 WSL, 8코어).
- 기준 빌드(`79af946ddf`, `rm -rf build/hns` 뒤 전체 재빌드, 약 40초): 종료 코드 0, ROM 32,712,548 B(97.49%), EWRAM 248,924 B(94.96%), IWRAM 25,516 B(77.87%). 경고 줄 166개, "파일: 메시지"(줄 번호 제거) 고유 목록 44개. ROM SHA-1 `09927f92473a555ef521349aee72e8367125452d`(직전 구간 최종 ROM과 같음).
- 경고 비교: 매 빌드의 경고를 같은 형식으로 만들어 기준 목록과 비교하고 새 경고만 확인했다.
- 테스트 기준: [`test-baseline-seq083.txt`](test-baseline-seq083.txt)(PASS 2,298 / FAIL 2,229 / TOTAL 5,197). PR마다 관련 테스트 파일을 돌려 기준 목록과 테스트 이름별로 비교했다. 알려진 예외: AI 더블 테스트 3건(`AI_REVERSE_BATTLER_LOGIC_ORDER_CHANCE 50`), `Move Animations work 1`·`2`(HnS 도구 팝업 태스크).
- 한글 포함 소스 줄: 커밋마다 `git show <커밋> | grep -aP '^[-+](?![-+]).*[^\x00-\x7F]'`로 확인했다.
- 코드 비교 도구(스크래치 `p84/`, 재생성 가능): `objcmp.py`는 기준 빌드 오브젝트(`obj-base/`)와 새 오브젝트를 `objdump -d -r`로 함수별 명령열(주소 제거, 재배치 기호 포함)로 바꿔 비교한다. `fndiff.sh`는 함수 하나의 명령 차이를 보여 준다. 비기능 PR은 이 방법으로 "바뀐 함수 목록 = 설명 가능한 것뿐"인지 확인했다(ROM 전체는 함수 크기 변화로 주소가 밀려 바이트 비교가 의미 없다).

## 동기화 단위: seq 84 #9376 `U-cleanup-9376` Clean up boolean comparisons

- 현재 판정: 적용(hunk 단위 수동 적용)
- 커밋: `6e050d6e70`
- upstream 근거: `f2c4aa4b99`
- 해결한 의존성: 없음. 같은 unit의 #9466(seq 85)은 바로 다음 행이라 순서대로 따로 커밋한다.
- 수정 파일(22): `include/librfu.h`, `src/battle_controller_player.c`, `src/battle_main.c`, `src/battle_script_commands.c`, `src/battle_tower.c`, `src/battle_util.c`, `src/dodrio_berry_picking.c`, `src/follower_npc.c`, `src/librfu_intr.c`, `src/librfu_stwi.c`, `src/link.c`, `src/link_rfu_2.c`, `src/menu_helpers.c`, `src/mystery_event_menu.c`, `src/mystery_gift_menu.c`, `src/party_menu.c`, `src/pokemon.c`, `src/sound.c`, `src/text.c`, `src/trainer_card.c`, `src/union_room.c`, `test/battle/hold_effect/berserk_gene.c`
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
  - 18파일은 `git apply`로 그대로 들어갔다(파일 모드 100755 유지). 문맥이 달라 실패한 4파일은 같은 표현이 남아 있는 줄만 손으로 바꿨다.
    - `battle_controller_player.c` `SetLinkBattleEndCallbacks`: HnS GBS 인자(`m4aSongNumStop(SE_LOW_HEALTH, FlagGet(FLAG_SYS_GBS_ENABLED))`)는 그대로, `gReceivedRemoteLinkPlayers == 0` → `!…`만.
    - `battle_util.c` `TryRunFromBattle`: HnS 도전 설정 분기(`tx_Challenges_LessEscapes`, `Random() & 512`)는 그대로 두고, 함수 안의 `effect++` 11개(그중 HnS 분기 1개 포함)를 `effect = TRUE`, `if (effect != 0)` → `if (effect)`. 분기가 서로 배타적이라 `effect`는 원래 0/1이다. 도주 메시지 경로(`HandleAction_Run` 이후)는 건드리지 않았다.
    - `sound.c` `PlaySE`: HnS GBS 분기(`ClearPlayerForGBSSoundEffect`)는 그대로, `== 0` → `== MUSIC_DISABLE_OFF`와 `#include "overworld.h"`(값 0, `include/overworld.h` enum). HnS의 로컬 `extern u8 gDisableMapMusicChangeOnMapLoad;`는 upstream처럼 남겼다(같은 형 중복 선언).
    - `text.c`: `DrawDownArrow` `drawArrow == 0` → `!drawArrow`, `DecompressGlyph_Small` `isJapanese == 1` → `isJapanese`. HnS 한글 글리프 경로(`glyphId >= 0x3700` → `gFont0KoreanGlyphs`)는 그 앞에 그대로 있다.
  - `!= 1`/`== 1` → 진릿값 치환의 전제 확인: `gReceivedRemoteLinkPlayers`(`bool8`)에 쓰는 곳 10곳이 모두 0/1/`TRUE`/`FALSE`. `gSTWIStatus->sending`(`vu8` → `vbool8`, `typedef vu8 vbool8`) 쓰기 8곳 모두 0/1. `TextPrinter.japanese`는 `u8 :1` 비트필드. `IsOverworldLinkActive()`는 `TRUE`/`FALSE`만 반환.
- 저장·ROM·그래픽 영향: ROM 크기 불변(32,712,548 B). 세이브 무관.
- **비기능 확인(오브젝트 코드 비교):** 바뀐 C 파일 21개의 오브젝트 함수 2,074개 중 2,063개가 명령열까지 같다. 다른 11개는 모두 `cmp rX, #1; beq` → `cmp rX, #0; bne` 형태의 비교 상수·분기 방향 변화와 그에 따른 블록 배치 변화뿐이다.
  - `battle_main.o` `CB2_AskRecordBattle`·`CB2_EndLinkBattle`(`AskRecordBattle`·`EndLinkBattleInSteps` 인라인, `gReceivedRemoteLinkPlayers != 1`), `librfu_stwi.o` `STWI_init`·`STWI_poll_CommandEnd`(`sending == 1`), `link_rfu_2.o` `LinkManagerCB_Child`·`_Parent`·`_UnionRoom`, `menu_helpers.o` `MenuHelpers_IsLinkActive`·`MenuHelpers_ShouldWaitForLinkRecv`(인라인), `trainer_card.o` `CreateTrainerCardTrainerPic`(`== 1`), `text.o` `RenderText`(`DecompressGlyph_Small` 인라인, `isJapanese == 1`, 블록 순서만 바뀜).
  - 위 전제(값이 0/1뿐)로 모두 동작이 같다. `battle_util.o`(`TryRunFromBattle` 포함)·`battle_script_commands.o`·`party_menu.o`·`pokemon.o`·`sound.o` 등 나머지 파일은 함수 전부 명령열 동일.
- 검증:
  - `git diff --check`: 통과. 비ASCII 줄 변경 0.
  - `make hns -j8`: 종료 코드 0, ROM 32,712,548 B / EWRAM 248,924 B / IWRAM 25,516 B, 새 경고 없음.
  - 자동 테스트: `test/battle/hold_effect/berserk_gene.c` 13건 — PASS 4 / FAIL 9, 기준 목록과 테스트별로 같음(FAIL은 영문 `MESSAGE`).
  - 실기 확인: 불필요(동작 동등). 선택: 통신(유니온룸·무선) 기능은 원래 실기 확인 범위 밖.
- 남은 위험: 없음.
