# full-sync 실제 port 결과 — 묶음 1: seq 142, 144, 145, 146, 147 (+155)

완료: 순서표 seq 142~147 가운데 남은 5행과, #9761과 같은 unit인 seq 155 #9780을 PR별 커밋 6개로 이식했다. seq 143 #9721은 선진행(2026-09-30)으로 이미 적용돼 있었다. 다음은 **묶음 2: seq 150 #9717, 152 #9757, 153 #9710, 154 #9779**다(148·149·151은 선진행으로, 155는 이번에 적용).

기준: [`port_sequence.tsv`](../1.17.0-sync-plan/port_sequence.tsv), 지시: [`PORT_INSTRUCTIONS.md`](PORT_INSTRUCTIONS.md)
시작 HEAD: `09af0a4231`(코드 `32b62b550f`). 작업 컴퓨터: 데스크탑. 2026-10-05 새벽 적용 뒤 사용자 사정으로 검증 전에 멈췄고, 같은 날 오후 리뷰·검증을 마쳤다.

## 요약

| seq | PR | 판정 | 커밋 | ROM 변화 | 비고 |
|---|---|---|---|---:|---|
| 142 | #8930 Automate regional Pokedex orders | 적용(HnS 적응) | `713e6499d7` | 0 B | 호연·관동 도감 enum을 FOREACH 자동 생성으로. **도감 번호·표 바이트 동일(실측).** HnS 조토 도감은 그대로 |
| 143 | #9721 | 이미 적용(선진행) | — | — | `full-sync-ahead-seq-130-167.md` |
| 144 | #7573 Reworked event evolution `tryspecialevo` | 적용(HnS 적응) | `811d70a039` | +144 B | 스크립트 진화 재작성. **스핀 진화의 파티 확인 HnS 유지(D1 = A안)** |
| 145 | #8472 Implement Wish Passing in ShouldSwitch | 적용 | `0742d9306b` | +336 B | AI가 소원을 다음 포켓몬에게 넘기려 교체. 새 테스트 4개 PASS |
| 146 | #9735 Mega Sol and Dragonize | 일부 적용(잔여분) | `ebf537b3a8` | +16 B | 본체는 첫 업로드부터 있음. 방어 날씨 보정·플라워기프트·바다의 몸 등 잔여분과 테스트 |
| 147 | #9761 Fix spaces, spelling, and grammar | 적용 | `f243869c9d` | 0 B | 주석·공백만. ROM 바이트 동일 |
| 155 | #9780 Revert an overzealous find-replace | 적용(선반영, 같은 unit `U-9761`) | `26e66ff4ad` | 0 B | #9761 일부를 되돌리는 주석 1줄. **seq 155 도달 시 이미 적용** |
| — | 리뷰 후 HnS 수정 | 슬레이트포트 배틀텐트 이어하기 파티 수 | `9eae3b51dc` | 0 B | #7573의 `ZeroPlayerPartyMons` 파티 수 초기화 뒤 다시 세지 않던 HnS 텐트 스크립트에 `special CalculatePlayerPartyCount`(배틀 팩토리 스크립트와 같은 방식) |

- 빌드(최종 `9eae3b51dc`): 종료 코드 0, **ROM 32,715,844 B(+496 B) / EWRAM 250,128 B(0) / IWRAM 25,516 B(0)**, SHA1 `b223236eec2ffd2ef9c2837b49a1a59864ae5b17`(`build/localization-logs/hns-20261005-135659-tentcount.log`). 새 경고 0(7커밋 모두). 6커밋 뒤 `26e66ff4ad`는 SHA1 `2ba17086…`로 적용 담당 빌드와 같았다(`hns-20261005-132908-chunk142.log`).
- 한글이 든 소스 줄 변경: **0**.
- 전체 테스트: PASSED 2,366 / TOTAL 5,298, 사라진 PASS 0(새 테스트 27개 추가). 새 기준 목록 [`test-baseline-seq147.txt`](test-baseline-seq147.txt).
- 세이브: 일반 세이브 호환 유지(세이브 왕복 PASS). 정적 비교는 #7573의 `ZeroPlayerPartyMons` 1줄 때문에 "엄격 함수 차이"로 FAIL 표시(아래 "세이브").
- 한글 회귀: seq 132 턴 종료 66/4, seq 139 안개제거 31/3 — 이전과 같다.

## 공통 사항

- 사전 분석(읽기 전용 4개, 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-142-147/`): `seq142-8930.md`, `seq144-7573.md`, `seq145-8472.md`·`seq146-9735.md`, `seq147-9761.md`(+#9780). 각 patch를 기준 사본에서 빌드·테스트했다.
- 적용: 적용 담당 1개가 patch 6개를 `git apply --check` → `git apply`로 그대로 넣었다(손으로 옮긴 곳 0). 지시 `chunk-142-147/APPLY.md`, 기록 `apply/PROGRESS.md`.
- 메인 결정
  - #8930: HnS 조토 도감은 **손대지 않는다**(upstream에 없는 HnS 블록이라 나중 이식과 충돌하지 않고, 매크로로 바꿔도 얻는 것이 적음). 같은 매크로 방식 선택 patch는 쓰지 않았다(`seq142-8930-johto-optional.patch`).
  - #7573: **D1 = A안** — `CanTriggerSpinEvolution`의 파티 확인을 남긴다(`// HnS:`). upstream 그대로면 마빌크가 없는 파티에서도 제자리 회전마다 필드가 다시 로드된다(1.17.0·master 같음). D2(이름 표기)·D3(닿지 않는 upstream 결함: `ChooseBoxMon_CanEvolve` 메모리 누수 등)는 기록만.
  - #8472: `SHOULD_SWITCH_WISH_PASSING_PERCENTAGE` 50.
  - #9735: 바다의 몸 줄은 1.17.0과 맞춤(A안). 메가니움 기본 폼을 메가솔라로 바꾸는 upstream hunk는 제외(upstream 실수, #9767이 되돌림).
  - #9761: `strings.c` 포켓 이름 표 정렬 hunk 제외(HnS 표는 한글, 공백만 바뀜). #9780은 PR별 커밋 원칙에 따라 다음 커밋으로 분리.

## seq 142 #8930 Automate regional Pokedex orders (`713e6499d7`)

- upstream `cc5348e295`. `include/constants/pokedex.h`의 호엔·관동 도감 enum을 `FOREACH_*` 매크로 자동 생성으로, `src/pokemon.c`의 `sHoennToNationalOrder`·`sKantoToNationalOrder` 본문을 매크로 한 줄로 바꾼다. migration 스크립트는 파일만 넣고 실행하지 않았다(#8497·#9624 선례. HnS에서 돌리면 `pokedex.h` 3곳이 깨짐).
- **HnS 적응:** `pokedex.h`는 upstream hunk 그대로. `pokemon.c`는 HnS 정렬을 두고 `KANTO_TO_NATIONAL`·`HOENN_TO_NATIONAL` 끝에 `,`만 붙였다. docs는 upstream도 고치지 않아 제외.
- **값 불변 실측(사전 분석):** C probe 컴파일과 asm `event_scripts.s`의 `.equiv` 638개가 같고, 관동·호연·조토·획득 가능·국가→종 표 5개 바이트와 `nm` 심볼 주소·크기가 모두 같다. ROM 차이 123 B는 모두 `src/pokemon.c`의 `__LINE__`이 494줄 당겨진 것(`verify_line_shift.py`).
- 테스트: `test/species.c` 결과 같음.

## seq 144 #7573 Reworked event evolution `tryspecialevo` (`811d70a039`)

- 스크립트 진화(`tryspecialevo`)를 재작성. HnS에서 `tryspecialevo`를 쓰는 곳은 0건이고 HnS 이벤트 진화도 없다(옛 매크로는 등록되지 않은 `TrySpecialScriptEvolution`을 불러 원래 쓸 수 없었다).
- **HnS 적응**
  - `SELECT_PC_MON_EVOLUTION`을 HnS 전용 `SELECT_PC_MON_PLA_TUTOR` 뒤에 넣어 `PLA_TUTOR` 번호 불변.
  - `chooseboxmon.c` `LearnMove`의 HnS 너즐록 `IsBoxMonExcluded`, PLA 가르침 코드 유지.
  - 종 데이터: HnS 도구 진화(Peat Block, 예리한손톱·물의돌) 보존. `EVO_SCRIPT_TRIGGER` param에 이름만 붙였다. 진화 조건·`EvolutionMethods` 번호 불변.
  - `evolution_scene.c` 중복 include hunk 제외.
  - **스핀 진화 D1 = A안**(위).
- upstream 변경 그대로 받은 것: `ZeroPlayerPartyMons`·`ZeroEnemyPartyMons`가 파티 수(`gPartiesCount`)도 0으로 만든다. `sTriedEvolving`이 `pokemon.c`의 `gTriedEvolving`으로 옮겨졌다(크기 같음). 세이브 영향은 아래 "세이브".
- 테스트: `pokemon.c`·`pokerus.c`·`daycare.c`·`species.c`·`evolution_tracker.c` 결과가 기준과 같다. 스핀 진화 임시 테스트 3/3(A안 확인).
- 기록(이 PR과 무관): 이식 전부터 `SPIN_CW_SHORT`가 `EVO_NONE`과 같은 0이라 5초 미만 시계방향 회전으로는 진화가 일어나지 않는다.

## seq 145 #8472 Implement Wish Passing in ShouldSwitch (`0742d9306b`)

- AI가 소원을 쓴 뒤 상성이 나쁘면 다음 포켓몬에게 회복을 넘기려 교체한다(싱글 전용). 의존 #9124·#9551 적용 확인. upstream 커밋이 #8943 뒤라 파티 구조가 같다. 손댄 곳은 선언 1줄 위치(HnS `GetPartyMonAbilityForSwitchCalc` 선언 뒤)뿐.
- HnS AI 수정 `f3a58f9939`(`CountUsablePartyMons`·`AnyPartyMemberStatused` 가드)와 얽히지 않는다(새 함수는 싱글 전용, 두 함수를 부르지 않음).
- HnS 스마트 교체 트레이너 27명의 파티에 소원은 0이다. 다만 (1) 예측 트레이너 25명과 싱글로 싸울 때 플레이어가 소원을 쓰면, AI의 플레이어 교체 예측에 이 분기가 쓰인다(50% 판정, `battle_ai_main.c`가 플레이어 배틀러에 상대 AI 플래그를 넣어 `ShouldSwitch(플레이어)`를 돌림). (2) 손가락흔들기·흉내내기로 소원이 나오면 AI 자신에게도 드물게 발동한다(이슬 `TRAINER_MISTY_POSTOBC_HNS` 왕구리, 비상 `TRAINER_FALKNER_POSTOBC_HNS` 돈크로우, `SaffronCity_FightingDojoVIP_hns`). 모두 upstream 의도 동작이다(리뷰 battle).
- 테스트: `ai_switching.c` 새 테스트 4개 PASS, 기존 결과 같음.

## seq 146 #9735 Mega Sol and Dragonize — 잔여분 (`ebf537b3a8`)

- 본체(`GetAttackerWeather`, 드래곤스킨, 회복·2턴기·명중)는 첫 업로드부터 이후 형태로 들어 있었다. 넣은 잔여분: `CalcDefenseStat`의 모래바람·설경 보정을 공격측 날씨 기준으로, 플라워기프트(방어)를 ctx 값으로, `IsBattlerWeatherAffected` 검사 순서를 1.17.0과 같게(동작 같음), 바다의 몸 1줄 `attackerWeather`, 테스트 4파일(`dragonize.c`·`mega_sol.c` 새로, `growth.c`, `ai_switching.c` 1줄).
- 새 한글 문자열 없음(특성 이름 드래곤스킨·메가솔라는 이미 있음). 메가니움나이트·장크로다일나이트가 맵·트레이너 어디에도 없어 정상 플레이 영향 없음.
- 테스트: 21파일 결과가 분석과 같다. `mega_sol.c` 바위 특방 테스트가 patch 전 FAIL → 후 PASS. growth TO_DO 1 → PASS. 새 FAIL 12(mega_sol 2·dragonize 10)는 모두 `Unmatched MESSAGE`.

## seq 147 #9761 / seq 155 #9780 (`f243869c9d`, `26e66ff4ad`)

- 주석·공백 철자 수정. upstream 149 hunk 가운데 124 그대로, 9 손으로 맞춤(주석 부분만 — `debug.h`·`overworld.h`의 HnS config 값과 `battle.h`의 `givenExpMons[2]`는 유지), 16 제외(HnS에 줄 끝 공백이 이미 없는 13개, #9780이 되돌리는 `pokemon.c` 1줄, `strings.c` 정렬 1개, 나머지 1개는 분석 문서 참고).
- 한글 변경 0. 바뀐 문자열 리터럴 줄은 HnS에서 쓰지 않는 `sText_PkmnSwitchOut`의 주석 부분뿐.
- ROM 바이트 동일(두 커밋 모두 SHA1 = `2ba17086…`). 테스트·문서 변경 없음.
- #9780: `pokemon.c` 팰리스 주석을 최종형 `Pokémon's nature`로. 다른 1줄(STATIC_ASSERT 주석)은 HnS 문장이 달라 바꿀 것이 없다.

## 세이브

- 정적 비교 `python3 /home/hjm0725/hns-sync-work/chunk-1385/verify/save_compat.py run`: **판정 FAIL(FAIL 2, WARN 4)**. 구조체·섹터 배치·세이브 상수·RAM 세이브 블록 크기는 모두 같다. FAIL 2는 같은 원인 하나다.
  - `strict ZeroPlayerPartyMons`: #7573(upstream 그대로)이 함수 끝에 `gPlayerPartyCount = 0;`을 더했다(`ZeroEnemyPartyMons`도 같은 방식). 파티를 비우는 함수가 파티 수도 0으로 만드는 변경이다. 이 함수는 세이브 경로 엄격 목록에 있어 코드 차이만으로 FAIL로 표시되고, 그 요약 줄이 FAIL 1을 더한다.
  - WARN 4: #8943 때 기대한 3건(`CalculatePlayerPartyCount`, `HandleSpecialTrainerBattleEnd`, 검토 요약) + `sTriedEvolving`(battle_main·pokemon 정적) → `gTriedEvolving`(pokemon 전역) 이동 1건(크기 1 B 같음, 세이브 무관).
- **실제 세이브 바이트는 같다(세이브 왕복 `verify/savetest/run_all.sh`, 사본 `tmp-D/post142`): 두 이미지 모두 PASS.** 이식 전 ROM이 만든 세이브를 읽은 결과 95줄과 다시 저장한 섹터 0~30, 같은 상태를 저장한 섹터, 새 게임(+`ZeroPlayerPartyMons` 경로) 세이브 섹터가 모두 바이트 동일.
- `ZeroPlayerPartyMons` 호출처: 새 게임 3곳(직후 파티를 채우고 수를 다시 계산), HnS 너즐록 화이트아웃(`overworld.c` `DoWhiteOut`, 파티를 비운 뒤 1마리를 만들고 수를 직접 1로 설정 — 영향 없음), 배틀 팩토리·텐트·디버그·`CreateTrainerPartyForPlayer`(upstream 코드). 커밋 리뷰에서 호출처별 판정(아래).
- 정적 비교 도구의 기대 보고서(#8943 A+B+C 기준)는 바꾸지 않았다. 다음 단위부터 이 FAIL 2·WARN 1이 계속 나오는 것이 정상이다.

## 전체 테스트

- 명령: `GITHUB_ACTION=1 make check BUILD=hns -j6 > build/port-check-chunk142.log 2>&1`, PORT_INSTRUCTIONS의 `LC_ALL=C` 추출.
- 결과: PASSED 2,366 / KNOWN_FAILING 10 / ASSUMPTIONS_FAILED 38 / TO_DO 606 / EXPECT_FAILING 6 / TOTAL 5,298, INVALID 21.
- `test-baseline-seq139.txt` 대비: **사라진 PASS 0.**
  - 새 PASS 16: Wish 4(#8472), Mega Sol 8(광합성·웨더볼 포함)·Dragonize 2·Growth 2(#9735, Growth 1은 TO_DO → PASS). 2,350 + 16 = 2,366
  - 새 FAIL 12: Dragonize 10·Mega Sol 2, 모두 `Unmatched MESSAGE`(영문 기대값, 알려진 한계)
  - TO_DO: growth의 옛 TO_DO 3개가 이름이 바뀐 테스트로 대체, 새 TO_DO 2(Dragonize Max Strike, -ate 비교)
- 새 기준 목록: `test-baseline-seq147.txt`.
- 한글 회귀: `ALLOW_REPO=1 chunk-132/D-tests/run.sh` 66/4, `ALLOW_REPO=1 chunk-139/B-tests/run.sh` 31/3(둘 다 요약 동일).

## 커밋 리뷰 (병렬 3개, 읽기 전용, 결과 `chunk-142-147/review-{dex,evo,battle}/REVIEW-RESULT.md`)

| 리뷰 | 대상 | 판정 | 요지 |
|---|---|---|---|
| dex | #8930 | 문제 없음 | 분석과 다른 4가지로 재확인: `#line 654` 한 줄로 줄 번호만 되돌린 빌드가 이식 전 ROM SHA1과 같음, 오브젝트 1527개 중 다른 것은 `pokemon.o`뿐(도감 화면·스크립트·트레이너 카드 등 지역 번호 사용처 오브젝트 모두 같음), DWARF enum 덤프(호연 215·관동 189·조토 283·획득 가능 483·전국 1081 이름·값·순서 같음), 전처리 토큰 차이는 `__LINE__` 12개뿐. 조토 블록 그대로, migration 스크립트는 빌드에 끼지 않음 |
| evo | #7573 | 경미 | 진화 표 535종 725항목 덤프 비교 — 차이는 의도한 치고마 `EVO_SCRIPT_TRIGGER` param 1줄. 활성 종 전체 레벨·교환·도구·배틀·스핀 진화 결과 해시 전후 같음, HnS 도구 진화 개별 PASS. 스핀 A안 = 이식 전 동작. `SELECT_PC_MON_PLA_TUTOR` 6 그대로. `ZeroPlayerPartyMons` 파티 수 초기화는 정상 경로에서 파티 수·세이브 바이트 같음(호출처 전수), `gTriedEvolving` 주소 같음. **경미 F1:** 슬레이트포트 배틀텐트 이어하기에서 첫 배틀 전까지 파티 수 0(첫 파티 메뉴 커서가 "취소", 세이브 바이트는 같음, upstream 1.17.0도 같음) → HnS 수정 `9eae3b51dc` |
| battle | #8472·#9735·#9761·#9780 | #8472 경미(문서), 나머지 문제 없음 | #8472: upstream과 같은 줄, 파티 번호 바름, `f3a58f9939` 함수 미호출. 결과 문서의 "현재 게임에서 발동 안 함" 문장이 틀려 고침(플레이어 교체 예측, 손가락흔들기·흉내내기). #9735: 모래바람·설경·플라워기프트·바다의 몸 × 만능우산·날씨부정·에어록 조합 데미지·회복 실측 — 메가솔라 없는 조합은 전후 같고 메가솔라 칸만 1.17.0대로 바뀜, 날씨 회귀 15파일 같음. #9761·#9780: 53파일 주석을 걷어낸 코드 토큰 차이 0, 합본 patch와 `cmp` 일치, 되돌린 빌드와 SHA1 같음 |

## 실기 확인 항목 (친구용)

0. 슬레이트포트 배틀텐트: 도전을 저장하고 이어하기 → 첫 배틀에서 파티 메뉴를 열었을 때 커서가 첫 포켓몬에 있는지(`9eae3b51dc`)

1. 도감(HGSS 화면): 조토·전국 도감 번호와 순서, 호연·관동 번호를 쓰는 곳이 있으면 그 표시
2. 진화: 레벨업·통신 교환·도구·친밀도 진화가 이전과 같은지. 마빌크 없는 파티로 제자리에서 돌 때 화면이 다시 로드되지 않는지(D1 A안)
3. (선택) AI가 소원을 쓰는 트레이너가 생기면 교체 판단

## 후속 행 메모

- seq 155 #9780: 이미 적용(이번 묶음).
- seq 204 #10051·205 #9885: #7573의 새 줄은 upstream 원문(`gPlayerParty` 매크로)이라 그대로 얹힌다.
- #8930 조토 도감 매크로 변환은 하지 않았다. 원하면 `chunk-142-147/seq142-8930-johto-optional.patch`(별도 `HnS:` 커밋).
