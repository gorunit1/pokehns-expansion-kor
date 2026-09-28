# full-sync 실제 port 결과 — seq 1~62

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`CLAUDE_FULL_SYNC_PORT_PROMPT.md`](../../CLAUDE_FULL_SYNC_PORT_PROMPT.md)
시작 HEAD: `88d72d436e`

## 동기화 단위: seq 1 `U-testrunner-fix`

- 현재 판정: 적용
- upstream 근거: #9892 커밋 `35a45557e1`의 첫 hunk와 동일
- 해결한 의존성: 없음
- 수정 파일: `test/test_runner.c` (`fake_rtc.h` include를 `global.h` 뒤로 이동)
- HNS 적응과 보존한 한글화/배틀 메시지 동작: 해당 없음(테스트 러너 전용, 게임 ROM 불변)
- 저장·ROM·그래픽 영향: 없음
- 검증:
  - 코드 대조: HnS `16ca5376eb`가 `fake_rtc.h`에 `gSaveBlock3Ptr`를 쓰는 inline 함수를 넣어, `global.h`보다 먼저 include되면 컴파일되지 않았다.
  - `git diff --check`: 통과
  - `make check BUILD=hns -j8`: 테스트 ELF 컴파일·링크 성공. 실행 도중 프로세스가 종료되어(데스크탑 WSL 재시작) 전체 결과는 없다.
  - 부분 결과: PASS 243, FAIL 3,641, KNOWN_FAILING 9, ASSUMPTION/SKIP 36
    - FAIL 중 3,381건이 같은 원인이다: `src/malloc.c:120` `AGB_ASSERT(block->allocated == TRUE)`와 `Illegal opcode: 0000efff`. 이미 해제된 블록을 해제하는 것으로 보이며, 영문 `MESSAGE` 불일치와는 다른 문제다.
    - 84건에서 `src/text.c:2696`(upstream `AllocateTextPrinter`)의 56바이트 누수 경고가 함께 나왔다.
    - HnS가 upstream 대비 추가한 `Free` 호출 중 배틀 경로에 있는 것은 없다(`challenge_menu`, `credits_hns`, `naming_screen`, `tv` 등만 해당).
  - 실제 ROM: 해당 없음
- 다음 단위 또는 외부 결정 필요 사항: 테스트 힙 assertion 원인을 별도로 조사한다. 해결 전까지 port 검증은 `make hns` 빌드 기준으로 한다.
