# full-sync 실제 port 결과 — seq 92~101

진행 중: 마지막 완료 seq 92, 다음 seq 93.

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), [`CLAUDE_FULL_SYNC_PORT_PROMPT.md`](../../CLAUDE_FULL_SYNC_PORT_PROMPT.md)
사전 분석: PR마다 병렬 분석 에이전트가 쓴 이식 계획(`hns-sync-work/chunk-092-101/seq<N>-<PR>.md`, 저장소 밖)을 따랐다.
시작 HEAD: `1364cb18fb` (작업 트리 clean)

## seq 92~101 공통 사항

- 빌드 명령: `PATH=/opt/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin:$PATH GITHUB_ACTION=1 make hns -j8 > build/port.log 2>&1` (노트북 WSL, 8코어).
- 기준 빌드(`1364cb18fb`, `rm -rf build/hns` 뒤 전체 재빌드, 26초): 종료 코드 0, **ROM 32,713,060 B(97.49%), EWRAM 248,924 B(94.96%), IWRAM 25,516 B(77.87%)**(seq 91 최종과 같음). 경고 줄 163개, "파일: 메시지"(줄·열 번호 제거) 고유 목록 42개. 이 빌드의 오브젝트와 `pokehns.gba/.elf/.map`을 스크래치에 복사해 두고 이식 후 비교에 썼다.
- 경고 비교: 매 빌드의 경고를 같은 형식으로 만들어 기준 목록과 비교하고 새 경고만 확인했다.
- 테스트: 분석 문서가 지정한 테스트 파일을 파일마다 `GITHUB_ACTION=1 make check BUILD=hns -j6 TESTS="<파일>"`로 돌리고, `PORT_INSTRUCTIONS` "테스트" 절의 `LC_ALL=C` 추출 목록을 [`test-baseline-seq091.txt`](test-baseline-seq091.txt)(= HEAD `7b4e3dba40` 전체 실행 결과)의 같은 이름 줄과 비교했다.
- 파일 단위 실행에서만 나오는 상태(기준 목록의 추출 정규식 밖): `ai_switching.c`의 `AI will not choose to switch out Dondozo with Commander Tatsugiri (1/50): INVALID`("SWITCH not required"). 전체 실행에서는 PASS이고, 이 테스트만 따로 돌려도 PASS다. **seq 92 변경을 되돌린 트리에서도 파일 단위 실행 결과가 같아서**(INVALID) 이식과 무관한 실행 순서 의존 상태로 본다. 같은 파일의 `ASSUMPTION_FAIL` 1건(`AI will stay in if Encore'd into super effective move`, `focus_punch.c`의 1건도)은 이전 전체 로그에도 있던 것이다.
- 한글 포함 소스 줄: 커밋마다 `git show <커밋> | LC_ALL=C grep -a -cP '^[-+](?![-+]).*[^\x00-\x7F]'`.

## 동기화 단위: seq 92 #9429 `U-9429` Add Gen 2-3 and Gen 4 Encore timers

- 현재 판정: 적용(문맥 2곳 수동)
- 커밋: `838e441612`
- upstream 근거: `72d635fc96`
- 해결한 의존성: 없음(deps `-`). seq 101 #9529(`generational_changes.h` → `config_changes.h` 이름 변경)의 기준 blob이 #9429 적용 뒤 파일이라 이 순서가 맞다.
- 수정 파일(6): `include/config/battle.h`, `include/constants/generational_changes.h`, `include/random.h`, `src/battle_script_commands.c`, `src/data/moves_info.h`, `test/battle/move_effect/encore.c`
- 적용 방법:
  - `include/*`·`src/battle_script_commands.c`: `git apply` 그대로(오프셋만 다름). `B_ENCORE_TURNS GEN_LATEST`, `F(B_ENCORE_TURNS, encoreTurns, …)`, `RNG_ENCORE_TURNS`(`RNG_TAUNT_TURNS` 뒤), `Cmd_trysetencore`의 세대 분기(`>= GEN_5`: 4턴, 대상이 이번 턴에 아직 행동하지 않았으면 3턴 / `GEN_4`: 3~7 / 그 이하: 2~6).
  - `src/data/moves_info.h` `[MOVE_ENCORE]`: upstream 문맥 줄 `.name = COMPOUND_STRING("Encore")`가 HnS에서는 `"앙코르"`라 hunk가 실패한다. `.name`은 그대로 두고 영문 description 안에만 `#if B_ENCORE_TURNS >= GEN_5`/`#elif >= GEN_4`/`#else`/`#endif`를 넣었다. 블록이 1.17.0과 `.name` 한 줄만 다르다.
  - `test/battle/move_effect/encore.c`: 기존 테스트 2개의 이름·config 변경은 그대로 적용. upstream은 TO_DO 5줄이 파일 끝이라고 가정하지만 HnS에는 그 뒤에 #9940/#9999 테스트 2개가 이미 있다. TO_DO 5줄과 뒤 빈 줄을 지우고 새 테스트 5개를 파일 끝에 붙였다. 결과 파일이 upstream 1.17.0 `encore.c` 앞 336행과 바이트 동일하다(뒤의 Champions 테스트 3개는 #10151 등 후속 PR 몫).
  - 제외한 hunk: 없음.
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 한글이 든 소스 줄 변경 0(`"앙코르"` 줄은 문맥으로만 남김). 앙코르 배틀 문구(`STRINGID_PKMNGOTENCORE`·`GOTENCOREDMOVE`·`ENCOREENDED`)와 출력 순서 변화 없음(`Cmd_trysetencore`는 메시지를 내지 않는다). config는 upstream과 같게 `GEN_LATEST`(= `GEN_CHAMPIONS`)로 두었다. `>= GEN_5` 분기라 현재 동작(3/4턴)과 같고 `Random()`을 부르지 않아 배틀 난수열도 같다.
- 저장·ROM·그래픽 영향: 세이브 영향 없음(`encoreTimer`는 `gBattleMons[].volatiles` 3비트 필드, 최대 7로 Gen4 7턴도 들어감). `RNG_*` 태그와 `CONFIG_*` 태그 번호가 1씩 밀리지만 저장하는 곳이 없다(`data/battle_scripts_1.s`의 `CONFIG_B_*` 참조는 다시 조립됨을 빌드 로그로 확인). 기술 설명은 `GEN_LATEST`에서 같은 영문 문자열이다.
- 검증:
  - `git diff --check`: 통과. 비ASCII 변경 줄 0. 파일 모드(100755) 유지.
  - `make hns -j8`: 종료 코드 0, **ROM 32,713,092 B(+32 B) / EWRAM 248,924 B(0) / IWRAM 25,516 B(0)**. 새 경고 0.
  - 자동 테스트: `encore.c` 15개(PASS 7 / FAIL 8).
    - 이름 변경 2개: `Encore forces consecutive move uses for 3 turns: Encore used before move (Gen5+)`(옛 이름 `… (Gen5+)` 없이), `… for 4 turns: Encore used after move (Gen5+)`(옛 이름 `… for 3 turns for player: …`) — 둘 다 PASS → PASS. 옛 이름 PASS 2줄이 목록에서 빠지는 것은 회귀가 아니다.
    - 새 PASS 2개: `Encore randomly chooses an opponent target (Gen 2-4)`(옛 TO_DO), `Encore allows choosing an opponent target (Gen 5+)`(신규).
    - 새 FAIL 3개(알려진 한계, 영문 MESSAGE): `Encore's effect ends if the encored move runs out of PP`(`Unmatched MESSAGE`), `Encore lasts for 2-6 turns (Gen 2-3) 1/5 (5/5)`·`… 3-7 turns (Gen 4) 1/5 (5/5)`(`PASSES_RANDOMLY`라 사유가 `Expected 0.19 passes/successes, observed 0.0`으로 표시됨). **로직 확인:** 로컬에서만 세 테스트의 영문 MESSAGE를 HnS 문구(`상대 마자용의\n앙코르 상태가 풀렸다!`, `마자용의\n앙코르 상태가 풀렸다!`)로 바꿔 돌리면 3개 모두 PASS였다(확인 뒤 upstream 원문으로 되돌리고 1.17.0 앞 336행과 `cmp` 동일 재확인). MESSAGE를 지우기만 하면 `NOT ANIMATION`의 끝 기준점이 없어져 항상 통과(0.99)하므로 로직 확인이 되지 않는다.
    - TO_DO 1개(`Encore lasts for 3 turns (Gen 5+)`)는 삭제됐다(위 두 Gen5+ 테스트가 대신함). 나머지 8개는 이식 전과 같다(PASS 3 / FAIL 5).
    - 관련 파일 9개(`taunt.c`, `mental_herb.c`, `aroma_veil.c`, `dancer.c`, `cant_use_twice.c`, `focus_punch.c`, `shell_trap.c`, `sleep_clause.c`, `ai/ai_switching.c`): 추출 목록이 기준 목록과 같다(회귀 0). `ai_switching.c`의 Dondozo INVALID는 위 공통 사항 참고.
  - 실기 확인: 불필요(기본 설정 동작과 문자열 변화 없음). 원하면 앙코르가 3턴(대상이 이미 행동한 턴이면 4턴) 유지되는지만 본다.
- 남은 위험: 낮음. `B_ENCORE_TURNS`를 `GEN_4` 이하로 바꾸는 경우에만 난수를 소비한다.
