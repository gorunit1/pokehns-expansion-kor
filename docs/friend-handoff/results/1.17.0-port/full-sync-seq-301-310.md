# full-sync 묶음 17 — seq 301~310

- 시작 HEAD `de50ef32`(코드 = `3fd38faf`, ROM SHA1 `0913b751…`). 작업: 클라우드 세션, 2026-10-12~13(도중에 컨테이너 재시작 1번 — 저장소·세션 임시 폴더는 그대로였다).
- 방식: 사전 분석 3갈래(그림 301·302·305, 배틀 303·304·307·309, 기타 306·308·310 — 저장소 사본에서 빌드·테스트, 산출물은 세션 임시 폴더 `b17/`) → 결정(모두 사용자 혼자 정할 수 있는 범위, 사용자 "추천대로") → PR별 적용·빌드·관련 테스트 → 리뷰 1개(R1, 읽기 전용) → 메인 검증.
- 311 #10542(XL 리팩터)는 다음 묶음.
- 실기(mGBA) 미확인(친구가 함).

## 커밋

| seq | PR | 커밋 | 판정·HnS 적응 | ROM(B) |
|---|---|---|---|---:|
| 301 | #10346 PokeCommunity sprites batch (June 2026) | `80e76462` | 남은 부분 적용. 메가찌르호크 아이콘·종 줄은 이미 있어 뺌. 그림 11개(아이콘 7 — 메가모단단게·메가냐오닉스·메가펜드라·자시안 2·자마젠타 2, 메가한카리아스Z 앞·뒤·팔레트 2)를 PR 뒤 blob으로 — 11개 모두 HnS blob이 PR 부모와 같아 HnS가 고친 그림을 덮지 않음. 자시안·자마젠타 4곳 `iconPalIndex` 2 → 0(새 아이콘과 짝). 메가한카리아스Z(`gen_4`, 종명 한카리아스) `.enemyMonElevation = 8`·`SHADOW(0, 18, SHADOW_SIZE_L)`는 한글 종 블록에 손으로 | 32,787,156 |
| 302 | #10414 Alolan and Mega Form Animation Frames | `09593e0e` | 대부분 친구 업로드(`1821fd67`)로 이미 있음(anim_front 32개, `pokemon.h` 32줄, 종 필드 92개 중 86개, 특성 hunk 없음). 남은 것: `shared_front_pic_anims.h` 표 3개(레트라 알로라·텅구리 알로라·라란티스) + 투구뿌논 표 갱신, 종 자리표시자 6곳(레트라 알로라·텅구리 알로라·라란티스 + 각 토템 — 라란티스는 업로드 때 자리표시자로 퇴행해 있었음), 꼬마돌 알로라·페르시온 알로라·메가뮤츠Y `normal.pal`·`shiny.pal`·`back.png`(일부 색이 검정으로 보이던 버그, back은 PLTE만 다름), 텅구리 알로라 `shiny.pal`, 안 쓰는 옛 `front.png` 31개 삭제(모두 PR 부모 blob, 참조 0). **HnS 텅구리 알로라 `normal.pal`·`front.png`·`back.png`는 유지**(사용자 결정 — HnS 커스텀 일반 팔레트에서 새 back은 명암 1단계 45 px가 다른 색과 합쳐짐) | 32,787,268 |
| 303 | #10433 Fix Opportunist and Mirror Herb not triggering during the end turn | `197be548` | 그대로(테스트 2파일 끝 빈 줄만). 턴 종료 세 번째 블록 뒤 `ENDTURN_OPPORTUNIST`·`ENDTURN_MIRROR_HERB`, `gQueuedStatBoosts` 초기화 위치 이동, 구슬·하양허브 턴 종료 판정을 도구 플래그 하나로(`HoldEffectInfo` 비트 −1 +1, padding 15 그대로). HnS 토템·하양허브 내던지기·열매 코드 그대로(`hns_totem_boost.c` 5/5). 새 문장 0, **출력 변화 → `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 행 추가**(같은 커밋) | 32,787,476 |
| 304 | #10477 Fix Misty Terrain grounding check | `1b23f7af` | **테스트만**(코드는 #9710 이식 `1fa0d489`가 이미 방어자 특성으로 판정). 새 `ASSUME(문포스 = 페어리)`는 HnS 페어리 챌린지 설정(`tx_Mode_Fairy_Types == 0`)으로 ASSUMPTION_FAIL(원래도 FAIL) | (테스트) |
| 305 | #10535 update mega magearna pokedex entry | — → **`306b0cc0`(2026-10-13 적용)** | (2026-10-13 친구 결정: 미번역 영문 도감 설명은 upstream대로 → 적용, 아래는 당시 기록) **skip**(사용자 결정, 번역 정책). HnS 메가마기아나 설명은 upstream 옛 영문 그대로(도감 설명 1,315개 중 한글 0) — 넣으면 기존 텍스트를 새 영문으로 덮어씀. 도감 설명은 친구 로컬 번역 범위(파찌파찌액셀 설명 `#if`를 뺀 seq 293과 같은 처리). 참고: PLZA 공식 한국어 `숨겨진 모드를 발동한 모습.⏎싹텄던 감정은 자취를 감췄고⏎그저 눈앞의 적을 쓰러뜨린다.`(Poké Corpus `LegendsZA/ko_common.txt` 39551행) | — |
| 306 | #10527 Fix FRLG Trainer Card Setup | `fcfc9fd4` | HnS 맞춤. `GetHnSTrainerStars`·`CARD_TYPE_HNS` case 유지(별 대입을 switch 앞으로 — 결과 같음), `VersionToCardType(GAME_VERSION)` = `CARD_TYPE_HNS`(옛 `CARD_TYPE_EMERALD`와 같은 case). 간호사 스크립트 골드카드 판정에 `.if !IS_FRLG`(HnS 문맥 — 챌린지 판정·`FLAG_EVEN_FASTER_JOY` — 그대로, HnS 바이트 같음). `pokedex.c` `HasAllMons` hunk 제외(HnS 자체 구현이 이미 국가 번호로 조회). 바뀌는 것 1개: **링크 카드**(유니온룸·케이블 클럽에서 상대에게 보이는 카드)가 프런티어 심벌을 모두 가졌을 때 별 +1을 더하지 않음 — HnS 자기 카드는 원래 +1을 안 했으므로 두 카드가 같아짐 | 32,787,460 |
| 307 | #10536 Multiple xp bugfixes | — | 이미 적용(`93656e60`, 2026-10-04 — hunk 10개 모두 있음, HnS `>> 1` 보정 2곳 그대로. upstream 1.17.0·1.17.1·master는 아직 `& BIT_FLANK`) | — |
| 308 | #10539 Fix pokemon shadow reverting to species_none shadow size for one frame | `228b8cc0` | 그대로. 배틀 끝 종 지우기 루프 → `FreeResetData_ReturnToOvOrDoEvolutions` 첫머리 `gBattleMons` 전체 memset. 그 뒤 경로(도감 내비·진화·필드 복귀)는 `gBattleMons`를 읽지 않음 | 32,787,460 |
| 309 | #10547 Adds alive check to CanTargetBattler | `085eda38` | 그대로. 쓰러진 포켓몬은 대상이 될 수 없음(플레이어 커서의 `gAbsentBattlerFlags` 따로 검사는 빠짐). 야생 더블 대상 루프·천적 아군 공격·AI(이미 HP 거름) 호출부 — 플레이어 쪽 둘 다 쓰러진 채 기술을 고르는 경우는 없어 무한 루프 없음 | 32,787,332 |
| 310 | #10550 Restore singmsg and normalmsg functionality | `a51d28d2` | 그대로(스크립트 명령 0xca·0xcb 함수 복구). 사용처는 `.if IS_FRLG` 안 FRLG 스크립트 3곳뿐, HnS 맵 0 | 32,787,364 |

- 적용 8(커밋 8, 304는 테스트만), 이미 적용 1, skip 1. ROM 32,787,220 → **32,787,364 B(+144)**, EWRAM 250,408 B·IWRAM 25,516 B 그대로. 커밋마다 `git diff --check` 통과, `make hns -j8` 종료 0, 새 경고 0(`warncheck.sh`, 클라우드 libpng `iCCP` 줄 제외).

## 동작 변화

새 배틀 문장 0, 기존 문장 변경 0. 포트로 새로 생긴 배틀 메시지 밖 텍스트 0(TRANSLATION_POLICY 5절 5번 목록 0개).

- 303(출력 변화, `BATTLE_MESSAGE_OUTPUT_CHANGES.md` "특성·도구·도주"): 턴 종료에 상대 능력치가 오르면(가속·변덕쟁이 등) 편승·흉내허브가 따라 함(편승 전부 → 흉내허브 전부, 빠른 순). HnS 트레이너 데이터에 편승·흉내허브 보유자 없음 — 플레이어가 쓸 때만.
- 301: 메가한카리아스Z 그림·배틀 위치·그림자, 아이콘 7개, 자시안·자마젠타 아이콘 색.
- 302: 레트라 알로라·텅구리 알로라·라란티스(+토템) 앞모습 2프레임 애니메이션, 투구뿌논 애니메이션 3번 반복, 꼬마돌 알로라·페르시온 알로라·메가뮤츠Y 색 정상화(일반·이로치), 텅구리 알로라 이로치 색.
- 306: 링크 카드 별(프런티어 심벌 전부일 때 +1 없음).
- 308: 배틀이 끝날 때 상대 그림자 크기가 한 프레임 튀던 결함 수정.
- 309: 더블에서 쓰러진 자리를 대상으로 고를 수 없음.

## 친구에게 알릴 것

1. **305 #10535 skip** — 메가마기아나 도감 설명(`gen_7_families.h`)은 기존 영문 그대로 두었다. 번역할 때 참고: PLZA 공식 한국어(위 표).
2. **302 텅구리 알로라** — HnS 커스텀 `normal.pal`·`back.png`는 그대로 두었고 `shiny.pal`만 upstream 새 값이다(이로치 앞모습 일부가 회색으로 보이던 버그 수정). 앞모습은 원래부터 upstream `anim_front.png` + HnS 커스텀 `normal.pal`로 그려지고 있었다 — 의도대로 보이는지 화면으로 확인해 달라. HnS `front.png`는 참조 없는 옛 파일(지울지 친구 결정).
3. 303 출력 변화 기록.

## 검증

- **빌드:** 위 표. 최종(`a51d28d2`) ROM 32,787,364 B.
- **관련 테스트(파일 단위, 기준 `test-baseline-seq300.txt`):** `opportunist.c` 16줄 중 PASS 13(새 PASS), `mirror_herb.c` 8줄 중 PASS 7(새 `copies the stats boosted by Speed Boost` PASS, 나머지 같음), `white_herb.c`·`flame_orb.c` 같음, `hns_totem_boost.c` 5/5, `misty_terrain.c` 새 PASS 2(`decreases power of Dragon-type moves`·`uses the defender's ability…`), Fairy 테스트 ASSUMPTION_FAIL(위). 사라진 PASS 0. 사전 분석 사본에서 `form_change/end_battle.c`·`mega_evolution.c`(308) 결과 같음.
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j4`, 코드 `a51d28d2`): PASS 2,653 → **2,658** / TOTAL 5,576 → 5,580. `testlist.sh` 기준 `test-baseline-seq300.txt` 대비 **사라진 PASS 0**, 새 PASS 5 — `Mirror Herb copies the stats boosted by Speed Boost`, `Opportunist activates before Mirror Herb during the end turn`, `Opportunist copies the stats boosted by Speed Boost`(303), `Misty Terrain uses the defender's ability when checking if it is grounded`, `Misty Terrain decreases power of Dragon-type moves by 50 percent`(304 — 이전 `1/2: FAIL`, 테스트가 영문 `MESSAGE` 대신 애니메이션을 보게 바뀜). `Misty Terrain does not increase the power of Fairy-type moves`는 `1/2: FAIL` → ASSUMPTION_FAIL(목록 밖, 위). 목록 [`test-baseline-seq310.txt`](test-baseline-seq310.txt)(**다음 비교 기준**).
- **한글 회귀 607개**(`ALLOW_REPO=1 dev_scripts/hns_verify/kortests/run.sh <저장소> kor-seq310 8`): 517/607, 기대 요약과 바이트 같음.
- **최종 ROM:** 코드 `a51d28d2`, 클라우드 `make hns -j8`(`arm-none-eabi-gcc 13.2.1`) → 사용 32,787,364 B, SHA1 `9bed33b5a097524c3f591c5f6617267ade2420ef`.
- **세이브 정적 비교**(`save_compat.py run --pre pre-301`, 이식 전 사실은 같은 세션에서 `de50ef32` 빌드로 만듦): **PASS(FAIL 0, WARN 0)**. 세이브 경로 코드·섹터 배치·SaveBlock 크기·EWRAM/IWRAM 변수 변화 없음. 세이브 왕복은 돌리지 않음(세이브 코드·구조 변화 없음).
- **리뷰 R1**(읽기 전용, 커밋 8개 대 upstream): 수정 필요 0. 301·302 바뀐 그림 모두 PR 뒤 blob이고 바뀌기 전 모두 PR 부모 blob(HnS 커스텀 덮음 0), 지운 31개 참조 0(`pokemon.h`는 모두 `anim_front.4bpp.smol`), upstream이 바꾼 종 40개의 애니·Y 오프셋·그림자·아이콘 필드가 PR 뒤와 같음(필드 순서만 다름), `shared_front_pic_anims.h`가 upstream과 바이트 같음. 303 흐름이 upstream 최종과 같음(차이는 HnS 기존 방음 줄·`pledgeState` 줄), `HoldEffectInfo` 17비트 + padding 15 = 32. 306 플레이어 카드 별 경로 전후 같음. 309 무한 루프 위험 없음(2대1 상대 빈자리·파트너 배틀·다이맥스·회복봉인 포함). 308·310 upstream과 같음.
  - **정정(302 텅구리 알로라):** 커밋 메시지는 HnS `front.png`를 "커스텀이라 유지"로 적었지만, 실제 화면은 `pokemon.h`가 가리키는 `anim_front.png`(이미 upstream PR 뒤 blob)와 HnS `normal.pal`로 그려진다. 남긴 `front.png`는 참조 없는 옛 파일(픽셀은 PR 부모와 같고 PNG 안 팔레트만 다름) — 지운 31개와 같은 처지인 32번째 파일이다. 해롭지 않아 그대로 두었다(지울지는 친구가 정함). HnS 커스텀으로 실제 쓰이는 것은 `normal.pal`뿐.
  - 텅구리 알로라 이로치(R1 픽셀 분석): PR 부모 → 뒤 `shiny.pal`은 10·11·12번만 바뀜. 앞모습(`anim_front`)의 11번 175 px가 이전 이로치에서 회색 (88,80,80)으로 보이던 것이 바로 upstream이 고친 버그 — 이제 고쳐짐. 뒷모습(유지한 PR 부모 `back.png`)은 10·12·14번만 쓰고 11번을 쓰지 않음 → 새 이로치에서 10번은 거의 검정(HnS 일반과 같은 명암 순서), 12번 하이라이트 45 px는 upstream 새 back(11번으로 옮김)보다 한 단계 밝음. upstream back을 받았다면 HnS 일반 팔레트에서 11 = 14라 45 px가 14번 색과 합쳐졌을 것 → 유지가 맞음.

## 재확인·뒤 PR 메모

- 302: `shared_front_pic_anims.h`는 upstream PR 뒤와 바이트 같음(R1). 텅구리 알로라 `normal.pal`(커스텀)·`back.png`(유지)·`front.png`(참조 없음) — 뒤 PR이 바꾸면 같은 방식(덮지 않고 보고).
- 305: 뒤 PR이 `[SPECIES_MAGEARNA_MEGA]` 설명을 문맥으로 쓰면 HnS 설명 그대로.
- 306: `trainer_card.c` HnS 분기(`GetHnSTrainerStars`, `CARD_TYPE_HNS`) — 뒤 트레이너 카드 PR에서 같은 방식으로 보존.
- 307: upstream이 `givenExpMons` 인덱스를 고치면 HnS `>> 1` 줄 정리(재확인 21).
