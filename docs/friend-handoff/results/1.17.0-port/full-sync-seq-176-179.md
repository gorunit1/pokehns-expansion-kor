# full-sync 실제 port 결과 — 묶음 6: seq 177, 178, 179 (+176 이미 적용, +198 #10014 선반영)

완료: 순서표 seq 176~179를 처리했다. 177·178·179는 PR별 커밋으로 이식했고, 176 #9864는 seq 115 #9494 이식 때 이미 들어가 있었다. 같은 unit의 **seq 198 #10014를 #9906 바로 뒤로 선반영**했다(순서표와 다르게 진행한 것). 다음은 **seq 181 #9730(XL, Stat Change Refactor)**이다(180 #9843은 #8943 단위에서 선반영).

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md)
시작 HEAD: `0d4f52ea20`(2026-10-07 HnS 수정 6개 뒤). 작업 컴퓨터: 데스크탑(2026-10-07).

## 요약

| seq | PR | 판정 | 커밋 | ROM 변화 | 비고 |
|---|---|---|---|---:|---|
| 176 | #9864 Make healthbox invisible for previous mon after opening party screen | 이미 적용 | (`489c58259c`, seq 115) | — | `src/reshow_battle_screen.c`가 upstream `47f01e61ba`판과 diff 0. HnS에서 실제로 필요함을 실측(빼면 교체 때 나간 포켓몬 체력 상자가 새 포켓몬 등장 직전까지 보임) |
| 177 | #9879 Move wild encounter related config to `wild_encounter` config file | 적용(HnS 적응) | `44465dc322` | +224 B | `B_*`/`OW_*` 야생 설정을 `WE_*`로. HnS 값 유지 |
| 178 | #9911 Remove two unused functions from src/event_object_movement.c | 적용 | `eb8263e754` | −64 B | 참조 0 함수 2개 삭제, 남은 호출 인라인 |
| 179 | #9906 Reduce battle heap usge outside main battle screen | 적용(HnS 적응) | `e2a8dbc7ef` | +96 B | 배틀 중 파티·가방·기술 배우기·도감 진입 때 배틀 애니 BG 버퍼 해제. HnS 사파리 볼 가방 1줄 |
| 198 | #10014 Fix battle debug not properly freeing some tilemaps | 적용(선반영) | `519ccf7ef6` | 0 | #9906 짝 수정. HnS는 배틀 디버그 메뉴가 모든 빌드에서 켜져 있어 지금 필요 |

- 빌드(최종 `519ccf7ef6`): 종료 코드 0, **ROM 32,754,340 B(+256 B) / EWRAM 250,132 B(0) / IWRAM 25,516 B(0)**, SHA1 `94079f561db0c6201cd295418ec6a9e6c9935e48`(메인 재빌드 같음, `build/localization-logs/hns-20261007-015728-chunk179.log`). 새 경고 0.
- 한글이 든 소스 줄 변경: 0.
- 전체 테스트(`build/port-check-chunk179.log`): PASSED 2,429 / TOTAL 5,332. 직전 기준(`test-baseline-hnsfix1007.txt`)과 다른 줄은 TO_DO 테스트 이름 1개(`B_FLAG_NO_CATCHING` → `WE_FLAG_NO_CATCHING`)뿐. 새 기준 목록 [`test-baseline-seq179.txt`](test-baseline-seq179.txt).
- 한글 회귀(저장소 밖 328개): 요약 파일 전체가 직전 기준(`post1007b`)과 같다(적용 담당 `post179`).
- 세이브: 정적 비교는 직전과 같은 표시. 세이브 왕복 PASS — 첫 실행에서 새 게임 비교가 FAIL로 나왔으나, 검증 도구가 세이브 이미지를 `pre-newgame.sav`로 바꾼 뒤 테스트를 다시 빌드하지 않아 `pre-make.sav`를 읽은 것이었다(로그 `LOAD image=pre-make.sav`). 새 사본에서 다시 돌려 두 비교 모두 PASS(`chunk-176-179/apply/savetest-post179r.log`). 도구 문제는 재확인 17(도구 이전) 메모에 적었다.

## 공통 사항

- 사전 분석(읽기 전용 3개, 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-176-179/`): `seq176-9864.md`·`seq178-9911.md`(한 담당), `seq177-9879.md`, `seq179-9906.md`. 기준 사본은 `ba9824bba9` 코드(HnS 수정 6개 전)라, 적용 전에 메인이 지금 HEAD에서 네 patch의 `--check`를 다시 확인했다.
- 적용: 적용 담당 1개, patch 그대로(손으로 옮긴 곳 0, patch 안의 HnS 손 맞춤 포함). 지시 `chunk-176-179/APPLY.md`, 기록 `apply/PROGRESS.md`.
- 메인 결정
  - #9879: upstream 형태 그대로(A안). upstream이 `TryDoDoubleWildBattle`의 `WE_FLAG_FORCE_DOUBLE_WILD != 0 &&` 가드를 지워 실행되지 않는 더블배틀 경로 코드 +224 B가 생긴다(`FlagGet(0)`은 늘 FALSE, 난수 소비 없음). 가드를 남기는 B안(ROM 0)은 쓰지 않았다.
  - #9906: HnS 사파리 볼 가방 진입 1줄 추가(없으면 사파리에서 가방을 열 때마다 12,320 B 누수).
  - #10014: seq 198에서 선반영(아래).

## seq 177 #9879 (`44465dc322`)

- upstream `ceb0ce5cb1`. 17파일 중 13파일 그대로, 4파일(`config/battle.h`, `config/overworld.h`, `item_use.c`, `pokemon.c`) HnS 값·문맥으로 손 맞춤(patch에 포함).
- HnS 값 그대로: `WE_FLAG_NO_ENCOUNTER` = `FLAG_DISABLE_ENCOUNTERS`(0x965), `WE_SMART_WILD_AI_FLAG` = `FLAG_SMART_WILD_AI`(0x969), `WE_FLAG_NO_RUNNING` = `FLAG_NO_WILD_RUNNING`(0x96A), `WE_FLAG_NO_CATCHING` = `FLAG_NO_WILD_CATCHING`(0x96B), `WE_FLAG_FORCE_DOUBLE_WILD 0`, `WE_DOUBLE_WILD_CHANCE 0`, `WE_DOUBLE_WILD_REQUIRE_2_MONS FALSE`, `WE_WILD_NATURAL_ENEMIES TRUE`. 옛 이름 잔여 0.
- seq 175 메모(재확인 8s)대로 `pokemon.c` `ComputePlayerShinyOdds`는 `WE_FLAG_NO_CATCHING` 줄만 바꾸고 `FLAG_SYS_POKEDEX_GET`·`GetShinyOdds()` 줄 유지. `item_use.c` `GetBallThrowableState`도 한 줄만 바꾸고 챌린지 4줄 유지. `*_hns` 맵 스크립트·`event_scripts.s`는 `FLAG_*` 상수를 직접 써서 고칠 곳 없음.
- ROM이 이름만 바뀐 부분은 바이트 같다(분석: 가드 삭제와 `debug.c` 줄 번호만 되돌린 사본의 SHA1이 기준과 같음).

## seq 178 #9911 (`eb8263e754`)

- upstream `8c5cbb1363`. `Unref_TryInitLocalObjectEvent`와 `UpdateObjectEventCoords`(seq 175 #8434가 들여옴) 삭제. 참조 0, 새 경고 0. 남은 호출부 하나로 GCC가 `InitObjectEventStateFromTemplate`를 인라인해 ROM −64 B(동작 같음). #9910(seq 185)의 `event_object_movement.c` hunk는 이 커밋이 먼저 있어야 붙는다.

## seq 179 #9906 (`e2a8dbc7ef`) + seq 198 #10014 (`519ccf7ef6`, 선반영)

- upstream `63fbcce1cd`. 배틀 중 파티·가방(피라미드 포함)·가방→파티·기술 배우기 요약·포획 도감·이름 짓기·파티 가득 교체에 들어갈 때 `CloseMainBattleScreen`으로 배틀 애니 BG 버퍼를 풀고, 돌아올 때 다시 잡는다. 진화는 원래 `FreeBattleResources` 뒤라 변화 없음.
- HnS 적응: `Cmd_trygivecaughtmonnick`의 `GetMaxPartySize()` 문맥 유지, **HnS 사파리 볼 가방 진입(`battle_controller_safari.c` `SafariOpenBagAndChooseItem`, `IS_HNS`)에도 같은 처리 1줄.** HnS 화면 경로(사파리 포케블록·파티, Wally 튜토리얼, 벌레잡기 대회, HGSS 도감, Gen4 배틀 UI 포함)에서 해제한 버퍼를 복귀 전에 읽는 곳 없음(분석).
- 힙 실측(분석, 사본 테스트 러너): 메뉴가 열려 있는 동안 사용량이 12,320 B 줄고 최대 연속 빈 공간이 커진다(파티 37,832 → 50,144 B, 가방 32,040 → 44,324 B, 기술 배우기 요약 24,616 → 36,720 B). 배틀 한 번의 최대 사용량은 기술 배우기 경로만 줄어든다(91,336 → 86,368 B). 복귀 뒤 단편화는 −176 B 정도로 두 번 왕복해도 나빠지지 않는다.
- **#10014 선반영 이유:** HnS는 `DEBUG_BATTLE_MENU`가 모든 빌드에서 켜져 있어(`include/config/debug.h`), 행동 선택 중 SELECT로 배틀 디버그 메뉴가 열린다. #9906만 넣으면 디버그 메뉴를 한 번 왕복할 때마다 12,320 B가 새고 세 번째 왕복에서 애니 버퍼 할당이 실패한다. #10014(디버그 메뉴의 타일맵 해제 1줄)로 막는다. 순서표 seq 198 자리에서는 "이미 적용"이다.
- Battle Arena는 재표시 순서 때문에 bg1·bg2 타일맵이 창 소유 버퍼로 바뀐다(upstream·1.17.0과 같음, 크래시 없음) — 실기 확인 대상.

## 커밋 리뷰

리뷰 1개(읽기 전용, 스크래치 사본). 결과 `/home/hjm0725/hns-sync-work/chunk-176-179/review/REVIEW-RESULT.md`. **네 커밋 모두 문제 없음.**

- #9879: upstream과 17파일·hunk 25개가 1:1, 줄 차이는 HnS 값 4개뿐. 플래그 값 4개와 `flags.h` 그대로, 옛 이름 잔여 0(docs·changelog 말고). `nm` 비교에서 크기가 바뀐 기호는 야생 조우 함수 3개뿐, `IsWildMonSmart`는 그대로. 되살아난 더블배틀 경로는 역어셈블상 `FlagGet(0)`과 분기만 있어 실행되지 않고 난수 소비도 같다.
- #9911: 지운 38줄이 upstream과 같고 참조 0, 인라인된 함수에 문제 없음.
- #9906: upstream 해제 13곳·재할당 2곳과 대조해 HnS는 사파리 1곳만 더함(필요·정확). 해제한 버퍼를 복귀 전에 읽는 곳·이중 해제·누수 없음. **힙 실측**(실제 메뉴를 열고 B로 닫아 복귀): 디버그 메뉴 ×3, 파티·가방·기술 배우기 요약·볼 가방·포케블록 각 ×2, Gen4 UI 혼합 왕복 모두 복귀 뒤 사용량 변화 0, 메뉴가 열린 동안 두 버퍼 NULL.
- #10014: 되돌리면 디버그 메뉴 왕복마다 12,320 B가 새고 세 번째 복귀에서 메모리 부족(`battle_util2.c:19`)을 재현 — 반드시 필요. 순서를 바꾼 영향 없음(seq 180~197 중 `battle_debug.c`를 건드리는 PR 없음), seq 198에서 다시 대면 `--check`가 실패해 두 번 들어가지 않는다.
- [정보] Battle Arena에서 메뉴에 다녀오면 bg1·bg2가 일반 배틀과 같은 배치로 바뀐다(크래시·누수 없음, upstream 1.17.0과 같음) — 실기 확인 항목.

## 실기 확인 항목 (친구용)

1. 배틀 중 파티 교체·가방 회복약·사파리 볼 가방 진입/복귀 뒤 배틀 애니 BG를 쓰는 기술 애니(예: 파도타기)와 볼 던지기
2. 레벨업 기술 배우기 요약 화면 왕복, 포획 → 도감 → 이름 짓기 → (파티 가득) 박스/교체, 배틀 뒤 진화
3. 배틀 디버그 메뉴(SELECT) 여러 번 왕복
4. Battle Arena에서 파티 메뉴를 연 뒤 애니
5. 야생 조우·포획 금지·도주 금지 플래그가 쓰이는 이벤트(이름만 바뀜)

## 후속 행 메모

- seq 185 #9910: #9911 뒤라 `event_object_movement.c` hunk가 붙는다. `wild_encounter_ow.c`는 재확인 8s대로 손 맞춤.
- seq 190 #9968: `include/config/wild_encounter.h` 문맥 줄(`WE_FLAG_NO_RUNNING 0`)이 HnS 값과 달라 손 맞춤. 그 뒤 #9965까지 얹으면 upstream 1.17.0과 HnS 값 4줄만 다르다.
- seq 198 #10014: 이미 적용(이번 선반영).
