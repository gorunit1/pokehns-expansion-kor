# 친구 답 2026-10-13 반영 — 폭주족 그림 ID 정리, #10428 설명, #10829 앞당김

- 친구 답 원문 [`FRIEND_REPLY_2026-10-13.md`](../../FRIEND_REPLY_2026-10-13.md)(묶음 16·트레이너 마무리 문자에 대한 답, 묶음 17 결과 문자보다 먼저 쓴 것). 작업: 클라우드 세션, 2026-10-13. 시작 HEAD `bc44b5a1`(코드 = 묶음 17 끝 `a51d28d2`).
- 2절(JIMMY·GEOFF)·3절(크레딧)·6절(실기 ROM 정보)은 확인뿐 — 코드 변경 없음. 금선 `kris_hns.png` 크레딧은 기존 줄(HnS용 수정 그림) 그대로.
- 실기(mGBA) 미확인(친구가 함).

## 커밋

| 커밋 | 내용(친구 답 절) | ROM(B) |
|---|---|---:|
| `ca819f90` | **폭주족 그림 ID 정리(1절).** `graphics/trainers/front_pics/biker_hns.png`·`graphics/trainers/palettes/biker_hns.pal` = 친구 zip(`hns_trainer_sprites_16c.zip`)의 `biker_hns.png`·`biker_hns.pal`(바이트 그대로 — PNG는 지금까지 쓰던 `biker_hgss_hns.png`와 같은 파일). 스토리 폭주족 14명 `Pic: Biker Hgss Hns` → `Pic: Biker Hns`, 미배치 KAZU `Pic: Biker Hns` → `Pic: Biker Frlg`. `TRAINER_PIC_BIKER_HGSS_HNS`·`gTrainerFrontPic_BikerHgssHns`·`gTrainerPalette_BikerHgssHns`·`biker_hgss_hns.png` 삭제. `CREDITS.md` 파일 이름만 `biker_hns.png`로. 폭주족 오버월드 `OBJ_EVENT_GFX_BIKER_HNS` 그대로 | 32,786,196 |
| `02ca8cfe` | **#10428 파찌파찌액셀 설명 `#if`(4절).** seq 293 때 뺀 설명 hunk를 upstream대로 — `B_UPDATED_MOVE_DATA >= GEN_8`(HnS)이면 `Electric bursts always go⏎first and raise evasion.`, 아니면 기존 문장. 기술 이름 `파찌파찌액셀` 그대로 | 32,786,196 |
| `6d08adc6` | **#10829 앞당김(5절).** HnS `MOVE_EFFECT_BREAK_SCREEN`(`src/battle_script_commands.c`)에서 상태 비트와 함께 `reflectTimer`·`lightscreenTimer`·`auroraVeilTimer`를 0으로. 친구가 말한 다른 절반(`gBattleCommunication[MULTISTRING_CHOOSER]` 0 초기화)은 HnS에 원래 있다(깨기 직전 `= 0` 뒤 비트 OR — 아래 B 경우가 이식 전에도 PASS). upstream 테스트 2개를 `test/battle/move_effect_secondary/break_screens.c`에 추가. `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 기술·필드 절 행 추가, `port_sequence.tsv` seq 551 `rom_est` = 앞당김 적용, `1.17.1-extension-plan.md` 표 갱신(seq 551에서 다시 넣지 않음) | 32,786,196 |

## 검증

### 폭주족 정리(코드/데이터 — 실기는 친구)

- 빌드 종료 0, 새 경고 0, ROM 32,787,364 → 32,786,196 B(−1,168 — 그림 1장·팔레트 1개 줄어듦), EWRAM·IWRAM 같음.
- 생성 트레이너 데이터(`src/data/trainers_hns.h`, `#line` 제외): `trainerPic` 15줄만 다름(14명 `BIKER_HGSS_HNS` → `BIKER_HNS`, KAZU `BIKER_HNS` → `BIKER_FRLG`).
- ROM 그림(정리 전 `a51d28d2` 빌드 대 후): 새 `gTrainerFrontPic_BikerHns`·`gTrainerPalette_BikerHns` = 옛 `…BikerHgssHns`(스토리 폭주족 그림 그대로), 새 `gTrainerFrontPic_BikerFrlg`·`gTrainerPalette_BikerFrlg` = 옛 `…BikerHns`(KAZU 그림 그대로). 그 밖의 트레이너 그림·팔레트·시설 표 심볼 515개 바이트 같음(`gBattleFrontierTrainers`·배틀텐트·트레이너힐 포함).
- 파티: 트레이너 포켓몬 1,832마리의 `TrainerMon` 바이트·개인값·성별 모두 같음(이번에는 데이터 위치가 밀려도 바뀐 것 0).
- **그림 번호가 하나씩 당겨진 영향:** 지운 번호 뒤의 HnS 그림 2개(`SWIMMER_M_HGSS_HNS` 229 → 228, `BLACK_BELT_HGSS_HNS` 230 → 229)는 코드·데이터가 모두 이름으로 가리켜 빌드 안에서 같이 바뀐다. **세이브와 녹화 배틀에는 트레이너 그림 번호가 저장되지 않는다** — 녹화 배틀 `struct RecordedBattleSave`는 트레이너 ID(`opponentA/B`, `partnerId`)·파티·시설 번호만, 프런티어·유니온룸 기록은 시설 클래스만 저장하고 그림은 실행 중 `gFacilityClassToPicIndex` 등으로 찾는다. 세이브 정적 비교(이식 전 사실 `pre-301`) PASS(FAIL 0, WARN 0), `recorded_battle.o` 37개 함수 같음, `RecordedBattleSave` 크기 4,092 B 같음. 다른 트레이너 ID 변화 없음(트레이너 표는 그림 번호와 무관).

### #10428 설명

- 빌드 종료 0, 새 경고 0, ROM 사용량 같음. 설명 hunk가 upstream `40b0bbb35a`와 줄까지 같음.

### #10829(친구 5절의 네 가지)

임시 한글 테스트 `test/battle/zz_tmp_10829.c`(저장소에 넣지 않음, 실행 뒤 지움)를 수정 전(타이머 3줄을 잠시 뺀 상태)과 수정 후에 돌렸다.

| 경우 | 내용 | 수정 전 | 수정 후 |
|---|---|---|---|
| A | 리플렉터·빛의장막·오로라베일(설경 아래) × 깨뜨리다·사이코팽·레이징불 9경우 — 깬 뒤 4턴 동안 세 방벽의 끝 문장이 다시 나오지 않음 | **FAIL**(1번째 경우부터 다시 나옴) | **PASS** 9/9 |
| B | 리플렉터를 깬 다음 턴에 빛의장막을 깸(기술 3종) — 두 번째에는 빛의장막 문장만, 리플렉터 문장 없음 | PASS | PASS |
| C | 깨지 않은 리플렉터·빛의장막·오로라베일이 지속 턴이 끝나 해제 — 끝 문장이 나옴 | PASS | PASS |
| D | 깨뜨리다·사이코팽의 피해가 리플렉터가 있을 때와 없을 때 같음(깨는 기술의 피해·대상·진행 불변) | PASS | PASS |

- upstream `break_screens.c` 파일 결과는 수정 전후 같다(새 테스트 2개는 영문 `MESSAGE` 기대라 HnS 한글에서는 판정이 안 됨 — `do not announce expiration again`은 영문 문장이 없어 PASS, `do not announce Reflect…`는 영문 문장 불일치로 FAIL. 기존 줄은 `test-baseline-seq310.txt`와 같음). 실제 한글 동작은 위 A~D로 확인.
- 빌드 종료 0, 새 경고 0, ROM 그대로.

### 전체

- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j4`, 코드 `6d08adc6`): PASS 2,658 → **2,659** / TOTAL 5,580 → 5,582. `test-baseline-seq310.txt` 대비 **사라진 PASS 0**, 목록 차이는 #10829 upstream 테스트 2줄뿐(`do not announce expiration again on later turns: PASS`, `do not announce Reflect when breaking a subsequent Light Screen 1/3: FAIL` — 영문 `MESSAGE`). 목록 [`test-baseline-hnsfix1013.txt`](test-baseline-hnsfix1013.txt)(**다음 비교 기준**).
- **한글 회귀 607개:** 517/607, 기대 요약과 바이트 같음.
- **세이브 정적 비교:** 폭주족 정리 뒤 PASS(FAIL 0, WARN 0). #10428 설명·#10829는 세이브와 무관(문자열·배틀 변수).
- **최종 ROM(실기용):** 코드 `6d08adc6`, 클라우드 `make hns -j8`(`arm-none-eabi-gcc 13.2.1`) → 사용 32,786,196 B, SHA1 `fb6658eb0279882b8705cf2069d7ccdd36841b33`. 묶음 17(seq 301~310)도 들어 있다.

## 친구에게 알릴 것(새로 찾은 것 — 고치지 않음, 재확인 63)

1. **방벽 해제 문장의 진영이 반대로 나오는 경우(기존 HnS 텍스트).** `STRINGID_REFLECTWOREOFF`·`STRINGID_LIGHTSCREENWOREOFF`·`STRINGID_AURORAVEILWOREOFF`는 `{B_ATK_PREFIX1}의 리플렉터가⏎없어졌다!`처럼 **공격자 쪽** 접두어를 쓴다(upstream은 `{B_DEF_TEAM1}` — 방벽 쪽). 턴 끝 만료·안개제거는 스크립트가 방벽 쪽 포켓몬을 공격자로 옮겨 맞게 나오지만, 깨뜨리다·사이코팽·레이징불과 배리어프리는 **깨는 쪽이 공격자**라서 플레이어가 상대 방벽을 깨면 `우리 편의 리플렉터가⏎없어졌다!`, 상대가 플레이어 방벽을 깨면 `상대의 …`로 나온다(앞의 경우는 임시 테스트로 확인 — 위 A·B는 이 문장으로 맞춰 잼, 뒤의 경우는 코드 해석). 수정안: (가) `BattleScript_BreakScreens`·`BattleScript_ScreenCleanerActivates`에서 출력 전에 방벽 쪽 포켓몬을 공격자로 옮김(문장 그대로, 만료 스크립트와 같은 방식), (나) 세 문장의 토큰을 방어 쪽으로 바꾸고 만료 스크립트도 맞춤. 친구 결정 대기.
2. **배리어프리도 #10829와 같은 타이머 결함.** 특성 배리어프리(`TryRemoveScreens`)가 상태 비트만 지워, 지운 방벽의 끝 문장이 뒤 턴에 다시 나온다(임시 테스트 재현). upstream 1.17.1·master도 고치지 않았다. 수정안: 양쪽 진영에 타이머 3줄씩. 친구 결정 대기.
3. 도감 설명(`species_info` `.description`)도 기술 설명처럼 "아직 미번역 → upstream 변경을 넣어도 됨"으로 볼지(#10535 메가마기아나, seq 305 — 묶음 17에서 skip).
