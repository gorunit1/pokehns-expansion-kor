# full-sync 실제 port 결과 — seq 121~126

진행 중: 마지막 완료 seq 121, 다음 seq 122 (#9425)

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md), [`CLAUDE_FULL_SYNC_PORT_PROMPT.md`](../../CLAUDE_FULL_SYNC_PORT_PROMPT.md), 메시지 출력 기록: [`BATTLE_MESSAGE_OUTPUT_CHANGES.md`](../../../localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md)
시작 HEAD: `5afec6304c` (작업 트리 clean. `aa175914e9`(seq 120 코드) 뒤로는 docs만 바뀜)

## 공통 사항

- 툴체인: 이 컴퓨터의 기본 PATH `arm-none-eabi-gcc` 13.2.1(`/usr/bin`). PATH 접두어 없이 빌드했다.
- 빌드 명령: `GITHUB_ACTION=1 make hns -j8 > build/port.log 2>&1`
- 이식 전 기준: `aa175914e9` 빌드(작업 트리의 `pokehns.gba` SHA1 `2ef9b346a463da48dc2a7f41f12d58ca4a619b53`, seq 120 결과와 같음) ROM 32,719,060 B / EWRAM 248,940 B / IWRAM 25,516 B.
- 경고 비교: `build/port-base-full.log`(전체 재빌드 로그)의 경고를 "파일: 메시지"(줄·열 번호 제거) 고유 42개로 만들고, 매 빌드의 경고를 같은 형식으로 만들어 이 목록에 없는 것을 "새 경고"로 셌다.
- 테스트 명령: 파일별 `GITHUB_ACTION=1 make check BUILD=hns -j6 TESTS="<파일>"`. 파일마다 따로 돌려 `PORT_INSTRUCTIONS`의 `LC_ALL=C`·`grep -a` 추출로 목록을 만들고, 같은 테스트 이름의 [`test-baseline-seq120.txt`](test-baseline-seq120.txt) 줄과 비교했다.
- 사전 분석: 읽기 전용 분석 에이전트가 PR별 이식 계획·적응 patch·검증 도구를 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-121-126/`에 만들었다(`seq<SEQ>-<PR>.md`/`.patch`, `tmp-<seq>/`). 적용 직전에 patch를 `git apply --check`로 다시 확인했다. patch의 `index … 100644` 모드 표기는 지운 사본을 썼다(HnS의 대상 파일 다수가 100755라서 그대로 적용하면 모드가 바뀜). 커밋에 기존 파일 모드 변경은 없다.

## 동기화 단위: seq 121 #9594 `U-spots-9594` refactor(graphics): make spinda spots generic (+ seq 163 #9796)

- 현재 판정: 적용(HnS 적응)
- 커밋: `0e6b9c22ec`
- upstream 근거: #9594 `bb1961348a`, 같은 unit의 #9796 `663c6bf3ae`("Add Spot Coord Limits", seq 163). 관련 #10247은 `!isEgg`만 `6480a48924`로 이미 들어가 있었다.
- 수정 파일(5): `include/pokemon.h`, `include/pokemon_spots.h`(신규), `src/decompress.c`, `src/pokemon.c`, `src/pokemon_spots.c`(신규)
- 적용 방법: 사전 분석 권장 patch(`seq121-9594.patch`, md5 `7c20111f…`)를 모드 표기만 지워 `git apply`했다. 충돌 없음. 결과 `src/pokemon_spots.c`는 분석의 목표 파일(`tmp-121/b/`)과 바이트 동일, `include/pokemon_spots.h`는 upstream `bb1961348a`와 바이트 동일.
- 내용:
  - `DrawSpindaSpots`·`gSpindaSpotGraphics`·`struct SpindaSpot`·`SPINDA_SPOT_*`를 지우고, 일반 spot 시스템 `pokemon_spots.c`(`ShouldDrawSpotsOnSpecies`, `DrawPokemonSpotsBothFrames`)로 옮겼다.
  - `LoadSpecialPokePicIsEgg`는 `ShouldDrawSpotsOnSpecies(species) && isFrontPic && !isEgg`에서 `DrawPokemonSpotsBothFrames`를 부른다.
  - **#9796을 같은 커밋에 넣었다.** `DrawPokemonSpots`의 `x`/`y`/`row`/`col`을 `s32`로 하고 버퍼 경계 검사를 추가했다. upstream `663c6bf3ae` 판과 비교하면 아래 "upstream과 다른 점" 3줄만 다르다. #9594만 넣으면 personality `& 0xF0 == 0`(약 1/16)인 얼루기에서 1프레임 첫 점이 통째로(12줄) 그림 버퍼 밖에 그려진다(#9796 PR 본문의 도감 목록 크래시와 같은 증상).
- **upstream과 다른 점:**
  - **spot_2 y = 25 유지(원작 3세대 값 유지).** upstream #9594가 설명 없이 27로 바꿨다. pokeemerald·pokefirered가 25이고, HnS 얼루기 그래픽은 upstream과 바이트까지 같아 27을 쓸 근거가 없다. 해당 줄에 `// HnS: keep vanilla Emerald/FRLG y = 25 (upstream #9594 changed it to 27)` 주석을 달았다. 선택 patch `seq121-9594-y27-optional.patch`는 쓰지 않았다.
  - `!isEgg` 유지(#10247). upstream #9594의 decompress hunk에는 없다.
  - 점 이미지는 `INCBIN_U32("…/spot_N.1bpp")` 그대로(HnS는 아직 INCGFX가 없음, `graphics_file_rules.mk`의 `-plain -data_width 2` 규칙 사용). seq 500 #9881에서 INCGFX로 바뀐다.
  - 공백 2곳 정리: 주석 23행 끝 공백, `case SPECIES_SPINDA: ` 끝 공백(#10247의 남은 hunk와 같음). `git diff --check` 통과용.
- 제외한 hunk: `docs/SUMMARY.md`, `docs/tutorials/how_to_spots.md`(튜토리얼 문서, 게임과 무관. #9557 선례).
- 검증:
  - `git diff --check` 통과. 한글이 든 줄 변경 0, config·세이브 구조체 변경 0.
  - **네이티브 비교(사전 분석 하니스 재실행):** 이식 전 `src/pokemon.c`(HEAD `5afec6304c`)의 `DrawSpindaSpots` 본문과 커밋된 `src/pokemon_spots.c` 본문을 그대로 x86에서 컴파일해 실제 `anim_front.png` 버퍼로 비교했다(하니스는 스크래치 사본에서 재생성, 생성된 비교 코드가 분석 때와 같음). 결과 `cases 2131072: on-sprite mismatches 0; OOB writes: old 129196, new 0`. 즉 personality 2,131,072개(바이트쌍 전수 2×65,536 + 무작위 200만)에서 그림 안 픽셀 차이 0, 버퍼 밖 쓰기는 이식 전 129,196건(원작에도 있는 1행 버그)에서 0건이 됐다.
  - 빌드(`build/port.log`): 종료 코드 0, **ROM 32,719,428 B(97.51%, +368 B) / EWRAM 248,940 B(0) / IWRAM 25,516 B(0)**. 사전 분석 추정 약 +362 B(정렬 전). `pokehns.gba` SHA1 `b53ea1d99902604b60af0ffaeb77e24ca6d8798c`.
  - 새 경고 0(`pokemon.h` 변경으로 넓게 다시 컴파일됐고 경고 163줄이 나왔지만 모두 기준 목록에 있는 기존 경고).
  - `pokehns.map`: `DrawSpindaSpots`·`gSpindaSpotGraphics` 0개, `src/pokemon_spots.o`의 `DrawPokemonSpots`·`DrawPokemonSpotsBothFrames`·`ShouldDrawSpotsOnSpecies`·`sSpindaSpots`·`sSpindaSpotImages`·`gSpindaSpotTemplate` 있음.
- 테스트(사전 분석 지정 스모크, `OPPONENT(SPECIES_SPINDA)` 포함): `test/battle/ability/contrary.c`, `test/battle/ability/opportunist.c` → 24줄(PASS 12)이 seq 120 기준 목록의 같은 줄과 **모두 같다**. 실패 10건은 모두 `Unmatched MESSAGE`(알려진 한계), 크래시·assert 없음. 이 PR·#9796은 `test/**`를 바꾸지 않는다.
- 남은 위험: 낮음. `GetSpotRow`의 `default: errorf`는 얼루기(SCALE_2 고정)로는 닿지 않는다. 링크 순서가 바뀌어 뒤쪽 주소가 이동하지만 세이브에 코드 주소를 저장하지 않는다.
- 실기 확인: 필요(아래 "실기 확인 항목" 1).
