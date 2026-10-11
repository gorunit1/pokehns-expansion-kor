# full-sync 묶음 15 — seq 268~285 (+ HnS 가드 1개)

- 시작 HEAD `9fa4aa95f1`(코드 = `3b356beb2e`). 작업 컴퓨터: 데스크탑(사용자는 랩탑에서 Remote Control). 날짜 2026-10-11.
- 방식: PR별 사전 분석 4개(병렬, 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-268-285/seq*.md`) → 결정(모두 사용자 혼자 정할 수 있는 범위, 사용자 "추천대로" — 281 문장만 친구 확인 요청) → 적용 1개(커밋 17개, 손 맞춤 0) → 리뷰 2개(R1 수정 필요 1 = 284가 드러낸 upstream 경계 버그 → HnS 가드 커밋, R2 문제 없음) → 메인 검증.
- 실기(mGBA) 미확인.

## 커밋

| seq | PR | 커밋 | 판정·HnS 적응 | ROM(B) |
|---|---|---|---|---:|
| 268 | #10368 Fix duplicate CANCEL entry in move relearner list after replacing a move | `0c6480fb33` | 그대로. 기술을 배운 뒤 목록 끝에 `취소`가 두 번 그려지던 결함(목록 작업을 다시 만든다). HnS는 요약 화면 기술 떠올리기가 꺼져 있고 NPC는 배우면 화면을 닫아 닿지 않음. seq 390 #10223 때 1.17.0 형태(`RedrawMoveList`)로 | 32,772,516 |
| 269 | #10369 Fix single-target moves hitting all foes in the shared shadow animation | `9018d735ba` | 그대로. 더블에서 야습·이차원홀·섀도스틸·엄습하는일격·폴터가이스트·고스트다이브의 그림자가 실제 대상에게만(다크홀·길동무는 그대로 양쪽) | 32,772,708 |
| 270 | #10366 Fix Orichalcum Pulse and Hadron Engine Ability Flags | `e32b02a351` | 그대로(`abilities.h`는 한글 특성명 문맥). 두 특성이 스킬스왑·역할·배껴그리기·리시버·화학변화·동료만들기·고민씨·심플빔·떠도는영혼 대상이 됨, AI는 만능우산 든 진홍빛고동을 쾌청 이득으로 보지 않음. 테스트 9파일 정리(상태 변화 0) | 32,772,692 |
| 271 | #10323 Unify trainer slide and script comparison operators | `949787aa77` | 줄 끝 공백 hunk 2개는 이미 같아 뺌, `scrcmd.c` include 위치만 맞춤. 새 `include/constants/comparison_operators.h`. HnS 맵 스크립트 2,034곳(258파일) 바이트 같음(ROM 차이 6 B = `scrcmd.c` 줄 번호 문자열). 트레이너 슬라이드는 클래스를 읽지 않음(재확인 51 해당 없음) | 32,772,692 |
| 272 | #10199 Dynamic Multichoice Tutorial(코드) | `10d2270093` | `event.inc` 주석·매크로 이동과 `script_menu.h` enum만. **튜토리얼 문서·PNG·`debug.inc` 예제 제외**(사용자 결정 — seq 236 선례, 예제는 HnS `Script_1` 자리이고 upstream #10469도 지움). `DYN_MULTICHOICE_CB_NONE` 255 → 2, HnS 스크립트 12곳·C 5곳이 이름으로 써서 ROM 차이 17 B 모두 `FF → 02`, 동작 같음 | 32,772,692 |
| 273 | #10349 Fix sprite fill function | `69cf21d87e` | 그대로. 타일 중간에서 시작하는 HnS 호출은 Gen3 UI 사파리 남은 볼 수 칸 하나 — 덮이던 칸이 원래 같은 색이라 화면 변화 0(VRAM 실측) | 32,772,692 |
| 274 | #10386 Fixes bounced moves not activating some effects | — | 이미 적용(`887c8e9ef8`, seq 129 단위 선반영 + HnS `587f4e7cdc`) | — |
| 275 | #10392 Fix sprite fills being broken in some cases | `aed1e5bfbc` | 그대로. `FillSpriteRect`가 1.17.0과 같아짐, 고쳐지는 경로를 타는 HnS 호출 0 | 32,772,628 |
| 276 | #10389 Fix Unseen Fist / Mummy interaction | `9cf3691ff4` | `MoveEndProtectLikeEffect`만 HnS `(void)`·`gBattlerAttacker` 형태로 맞춤. `SpecialStatus` 비트를 같은 바이트 안에서 나눔(EWRAM·세이브 같음). 보이지않는주먹 연속 공격 중 미라로 특성이 바뀐 뒤 잘못 나오던 블로킹·니들가드 등 문장 없어짐 | 32,772,596 |
| 277 | #10387 Remove text encoding error in za.json | `8aa37f0632` | 그대로(도구 입력, ROM SHA1 같음). HnS `all_learnables.json` 정리 선택 patch는 넣지 않음(사용자 결정) | 32,772,596 |
| 278 | #10385 Fix FRLG Magikarp and Heracross size units | `9122381832` | 그대로(`.if IS_FRLG` 안 — HnS ROM SHA1 같음) | 32,772,596 |
| 279 | #10382 Fix debug time scripts leaving followers frozen | `44960af08b` | 시간 스크립트 3개만(272에서 뺀 예제 hunk 제외). HnS 디버그 메뉴(R+START)에서 적용됨 | 32,772,596 |
| 280 | #10381 Rename and rework Can Use All Moves to Values Moves Over Splash | `8a5b4aae64` | 테스트만(파일 이름 변경). 단독 테스트 3개 삭제·이름 변경 9줄 | 32,772,596 |
| 281 | #10395 Fix Orichalcum Pulse and Hadron Engine Activation Messages | `68384e2780` | 코드 그대로(HnS에 `GetWeather()` 있음, #10170 선행 불필요), `battle_message.c` 문맥만 한글. **새 문장 4개 = SV 공식 한국어**(Poké Corpus `abcboy101/poke-corpus` `5d1da078` `ScarletViolet/ko_common.txt` 8441·8445·8449·8453행, 진홍빛고동 2개는 `LegendsZA/ko_common.txt` 6379·6383행도 같음), HnS 줄바꿈 관례(주어 줄 + 한 줄, 2줄째 175/148/180/172 px, 창 208) — **친구 확인 대기**(재확인 55) | 32,773,124 |
| 282 | #10379 Fix inverse battle matchup parametrization | `b6245648d0` | 테스트만. HnS 테스트는 페어리 모드가 꺼져 이식 전후 모두 ASSUMPTION_FAIL(줄 번호만) | 32,773,124 |
| 283 | #10270 Even More Gen VII Animation Data + Gen VIII Clean Up | `b52bbcc557` | 대부분 친구 업로드 `1821fd6749`로 이미 있음. 남은 것: 마기아나 2폼 애니메이션, **실버디·타입:널 팔레트·뒷모습**(지금 앞모습 다리 색이 틀린 버그 수정 — 렌더 비교 저장소 밖 `chunk-268-285/review-R2/render-283.png`), 단지래플 그림, 8세대 12종 애니메이션, 안 쓰는 옛 `front.png` 19개 삭제. **대여르 메가 특성 hunk 제외**(사용자 결정 — Defiant×3 유지, 애니메이션 PR에 섞인 병합 실수로 추정). 바뀐·지운 그래픽 60개는 HnS가 고친 적 없는 파일(바뀐 41개 = 1.17.0 blob), `.pal`은 CRLF | 32,773,364 |
| 284 | #10399 Fix invalid switch in if ace flag is present | `94a944bce6` | 그대로(#8943·#9575 이미 있음). HnS 에이스 트레이너는 0명이지만 같은 파티 더블(쌍둥이 등)에서 짝이 마지막 벤치 몬을 계획한 턴의 파티 번호 255 교체(오류 화면 뒤 임의 교체)가 막힘. 재확인 51 해당 없음(클래스 읽지 않음) | 32,773,572 |
| 285 | #10343 Fix time-of-day blend settings being updated every frame | `3acad18968` | 손 맞춤: HnS 낮/밤 필드 포켓몬 플래그 블록은 `updateBlend` 조건 밖에 유지, #8434 OWE 가드 주석만. 1분 갱신 사이에 `GetTimeOfDay()`가 불리면(풀숲 한 걸음 등) 시간대 색조가 다음 페이드까지 멈추던 결함 수정(사본 덤프) | 32,773,620 |
| — | **HnS: 짝이 마지막 벤치 몬을 고른 턴의 일반 교체 포기**(리뷰 R1, 284가 드러낸 upstream 경계 버그) | `c8e7bcc866` | 284 뒤 같은 파티 AI 더블에서 반대 방향(오른쪽 상대가 좋은 후보 없이 교체, 왼쪽이 마지막 벤치 몬, 판단 순서가 뒤집힌 턴 — `AI_REVERSE_BATTLER_LOGIC_ORDER_CHANCE` 50%)은 6을 저장 → 엔진이 행동 취소로 읽고 같은 선택을 반복 → **무한 루프로 진행 멈춤**(이식 전은 255 → 오류 화면 뒤 복구). 1.17.0·upstream master도 같음. `IsSwitchinValid` 일반 교체에서 남은 후보가 없으면 교체하지 않고 기술 사용. 닿는 HnS 트레이너 LEA_AND_PIA·FINLEY·MUALANI. 회귀 테스트 2개(`test/battle/ai/ai.c`) | 32,773,732 |

## 동작 변화 (`BATTLE_MESSAGE_OUTPUT_CHANGES.md`에 행 추가)

- 270 두 특성 교체·복사 성공(기존 문장), 276 보이지않는주먹 + 미라 뒤 방어 효과 문장 없음, 281 진홍빛고동·하드론엔진 전용 문장(새 문장 4개, 하드론엔진은 `발밑에 전기가 흐르기 시작했다!` 대신, 이미 같은 날씨·필드면 팝업과 문장이 새로 생김). 두 특성은 HnS 일반 진행(트레이너·야생·프런티어) 데이터에 없어 랜더마이저·디버그에서만 보임.
- 문장 밖: 269 고스트 그림자 애니메이션 대상, 283 실버디·타입:널 색, 284·HnS 가드 AI 교체(같은 파티 더블), 285 필드 시간대 색조 갱신.

## 검증

- **빌드:** 커밋마다 종료 코드 0, 새 경고 0, EWRAM 250,408 B·IWRAM 25,516 B 그대로. 최종(`c8e7bcc866`) ROM **32,773,732 B**(묶음 전 +1,280), SHA1 `e7377c2c0a03b9927f80dbcf6ec15e3bceff040c`.
- **전체 테스트:** PASS 2,649 → **2,650** / TOTAL 5,566 → 5,572. 목록 차이는 모두 분석 예측과 같다:
  - 280: 단독 테스트 3개 삭제(`AI uses Final Gambit/Guillotine/Sheer Cold: PASS`) + `AI can use all moves, …` 9줄 → `AI values moves above Splash, …` 9줄(PASS 5 그대로, FAIL 4는 같은 원인). `testlist.sh`의 사라진 PASS 8 = 삭제 3 + 이름 변경 5. **그 밖의 사라진 PASS 0.**
  - 새 줄: 284 `AI does not switch in into invalid Pokemon: PASS`, HnS 가드 테스트 2개 PASS, 276 PASS 1·KNOWN_FAILING 1(upstream 그대로), 281 FAIL 4(upstream 테스트의 영문 `MESSAGE`·영문 종명 — HnS 한글 출력과 불일치, 이식 전 같은 파일의 기존 FAIL과 같은 이유).
  - 282는 요약의 ASSUMPTION_FAIL 줄 번호만(`inverse_battle.c:49` → `:64`). 271은 `touch test/script.c` 뒤 `Script_HasNoEffect` PASS 그대로.
  - 목록 [`test-baseline-seq285.txt`](test-baseline-seq285.txt)(**다음 비교 기준**).
- **한글 회귀 607개:** 기대와 바이트 같음(517/607). 영역 A 사본에서 270+276+281(한글 안) trace도 이식 전과 607개 모두 같았다(두 특성 장면 없음).
- **세이브:** 정적 비교 **PASS(FAIL 0, WARN 0)**(EWRAM·IWRAM 변수 변화 0), 세이브 왕복 PASS.
- **리뷰 2개:**
  - R1(270·276·281·284·269·280·282): 270·276·281·269 upstream과 같은 줄(손 맞춤 1곳 뜻 같음), 281 한글 4줄이 SV 공식 문장과 글자까지 같고 게임 함수로 잰 폭 1줄째 최대 99 px·2줄째 175/148/180/172 px, 메시지 순서(고대활성·쿼크차지·트레이스·더블·원시 날씨·날씨부정·화학변화가스 뒤 재발동) 분석 trace와 같음. **284 수정 필요 1건** → `c8e7bcc866`.
  - R2(268·271·272·273·275·277·278·279·283·285): 모두 문제 없음. 스크립트 레이블 30,846개 단위 비교에서 바뀐 것은 272의 12개(`FF → 02`)·279 디버그 3개뿐, 283 HnS 그림을 덮지 않음(ROM 팔레트 38/38 = `.pal`), 285 걸음·날씨·실내외 페이드·새 오브젝트 팔레트 장면 덤프가 대조와 같음.
- **메인 검증(최종 HEAD `c8e7bcc866`):** 재빌드, 전체 테스트 목록, 한글 607개, 세이브 정적·왕복(`chunk-268-285/main/main-verify.sh`, 이식 전 세이브 사실 `main/save/pre-268`).

## 재확인·뒤 PR 메모

- 55(새): 281 새 문장 4개 SV 공식 문장 — 친구 확인. 이의가 있으면 `battle_message.c` 4줄만.
- 56(새): `c8e7bcc866` HnS AI 교체 가드 — upstream이 이 경계(짝이 마지막 벤치 몬을 고른 턴의 일반 교체)를 고치면 정리. upstream 보고 후보.
- 57(새): 대여르 메가 특성 Defiant×3 유지(#10270 hunk 제외) — 1.17.0 최종은 `{ BATTLE_ARMOR, NONE, DEFIANT }`. 뒤 PR이 이 줄을 문맥으로 쓰면 HnS 값으로 맞춘다. 1.17.0 값으로 바꿀지는 게임 수치라 친구 질문(HnS 일반 진행에 대여르 없음).
- 268: seq 390 #10223 때 `UIEndTask`를 1.17.0 `RedrawMoveList()` 형태로 맞추고, #9006 이식이 앞당긴 `VAR_RESULT` 초기화 줄은 다시 넣지 않는다.
- 271: seq 361 #9211 때 `trainer_slide.c` `BattlerHPPercentage` 선언 줄 손 병합. 증분 `make check`는 `event.inc` 변경 뒤 `test/script.o`를 다시 만들지 않는다 → 전체 테스트 전 `touch test/script.c`.
- 281: #10454(외부 결정)가 진홍빛고동 case를 `TryChangeWeatherWithAbility()`로 다시 쓴다(문장·스크립트는 1.17.0에도 남음).
- 283: `shared_front_pic_anims.h`의 `#endif //P_FAMILY_JANGMO_O` 주석 오타는 upstream·1.17.0과 같게 둠(seq 302 #10414 문맥).

## 실기(mGBA) 확인 항목

- (선택) 레아&피아·FINLEY·MUALANI 더블에서 두 상대가 같은 턴에 교체하려 할 때 진행이 멈추지 않는지, 실버디·타입:널 앞·뒷모습 색, 필드에서 풀숲을 걷는 중 시간대가 바뀔 때 색조가 바로 바뀌는지.
