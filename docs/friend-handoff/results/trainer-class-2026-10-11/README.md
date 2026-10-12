# 트레이너 클래스명·이름·배틀 그림 HGSS 복원 (2026-10-10 ~ 10-11)

- 요청: 친구 [`FRIEND_REQUEST_2026-10-10.md`](../../FRIEND_REQUEST_2026-10-10.md)(최종 방침), 답 [`FRIEND_REPLY_2026-10-10b.md`](../../FRIEND_REPLY_2026-10-10b.md)(질문 13개 답·작업 방침). 목표: **메인 스토리에서 실제로 만나는 트레이너**를 HGSS 원작의 같은 인물(위치·NPC 배치·파티·재전으로 판정)의 클래스명·이름·배틀 그림으로 복원하고, 게임 동작(상금·몬스터볼·AI·파티·BGM·랜더마이저·트레이너 ID·프런티어)은 보존.
- 방식: 1단계 조사 3개(원작 대조·Poké Corpus 공식 한국어·기술 설계) → 친구 질문 13개 → 2단계 조사 3개(배치 확정·이름 확정·공개 그림 허가·오버월드) → 확정된 부분 적용 12커밋(친구 "확정된 부분은 단계별로 반영해도 좋아") → 리뷰 2개(수정 필요 0) → 메인 검증. 조사·적용 자료는 저장소 밖 `/home/hjm0725/hns-sync-work/trainer-class-1010/`.
- 실기(mGBA) 미확인 — 헤드리스 mGBA로 실제 게임 함수를 돌린 실측은 했다(아래).
- 비교표(이 폴더): `trainers_before_after.tsv`(651항목 — 이름·클래스·표시명·동작 클래스·그림·성별 전후), `class_names_before_after.tsv`(179클래스), `frontier_names_before_after.tsv`(프런티어·텐트·견습생·브레인), `new_trainer_pics.tsv`(새 그림 ID), `hold_list.tsv`(보류 목록·이유), `sprites_needed.tsv`(원작 그림이 없는 클래스), `public_assets.tsv`(공개 그림 허가·크레딧), `overworld_plan.tsv`(오버월드 조사).

## 1. 커밋

| 커밋 | 내용 |
|---|---|
| `b4aa082379` | 클래스명 한글화 — HnS 56·에메랄드 노출 39(최신 공식 표기, 출처는 Poké Corpus `abcboy101/poke-corpus` `5d1da078` 등; `체육관 관장`·`라이벌`·`로켓단`·`로켓단간부`·`로켓단보스`·`괴짜 연구원`; 프런티어 `타워타이쿤`(HGSS·Pt 같은 칭호)·`아레나캡틴`·`피라미드킹`(마스터즈 EX)·`돔슈퍼스타`·`팰리스가디언`(pokeemerald-kr 팬 번역); DEVELOPER 영문). 클래스명 칸 `TRAINER_CLASS_NAME_LENGTH 18`(최장 `불난집 전문털이범` 17바이트), 유니언룸 카드·배틀 메시지 지역 버퍼. 세이브 구조 무변경 |
| `a0a08bdf0f` | 동작 클래스 — `struct Trainer.behaviorClass`(패딩 자리, 52바이트 그대로), trainerproc `Behavior Class:`, 동작 지점 10곳(상금·볼·랜더마이저 시드·배틀 BGM·승리 BGM·전환·배경 2곳·친밀도·라디오 제외)이 `behaviorClass ?: trainerClass`를 읽음. 데이터 변경 없음 |
| `29bb228de6` | 새 그림 ID(기존 끝): 213 `SCIENTIST_HNS`(SoulGold `scientist.png`), 214 `TEACHER_HNS`·215 `MEDIUM_HNS`(친구 제작 — 픽셀 그대로, teacher는 PNG 청크만 정리) |
| `26af9245f8` | 로켓단 조무래기 ♂♀ 그림(`rocket_grunt_m/f_hns`, 스토리 전용)을 SoulGold HGSS 그림으로(옛 그림은 `_frlg`와 같아 보존) |
| `d618e65415` | 새 클래스 7개(괴짜 연구원·선생님·무당·보더·장로·로켓단보스·더블팀, 기존 끝) + 확정 42항목 `Class:` = 원작, `Behavior Class:` = 바꾸기 전 클래스, `Pic:` = 원작 그림(그림 확보분 30) |
| `890dfce9ab` | 프런티어 Collector 그림을 SoulGold 이식 전 에메랄드 그림(blob `15187ce1c8`)으로 — 스토리 사용 0이 된 뒤(연구원 5명은 `SCIENTIST_HNS`) |
| `887bf5b81f` | 테스트 트레이너 이름 상수(`AI_TRAINER_NAME` 등)를 `포켓몬 트레이너 …`로 — 클래스명 번역으로 한글 회귀 HNS9799 5개가 512/607로 떨어진 것을 517로 복구 |
| `8b367c121d` | 공개 그림 6클래스(새 ID 216~221): 피크닉걸·쌍둥이(Pawkkie, Team Aqua's Asset Repo), 엘리트 트레이너♀(MrDollSteak)·♂(Rizon/Falsever, PokéCommunity 308798), 러브러브커플(spilledpizza 외, TAH), 초능력자♂(Rubire4, TAH) — 모두 64×64·16색 이하라 감색 없음. 배치된 80항목(피크닉걸 26·엘리트♀ 21·엘리트♂ 16·초능력자♂ 11·쌍둥이 5·러브러브커플 1) |
| `0f980605f7` | `CREDITS.md` 크레딧(308798 스레드 — 이미 넣은 SoulGold 25종 중 15종도 이 스레드 그림, SoulGold, TAH 제작자들). 친구 그림 줄의 이름 표기는 비워 둠 |
| `fb776b2466` | 트레이너 이름 505(HGSS 공식 473 등 + 일반 호칭 `조무래기` 28), `Name:` 줄만 |
| `b039cae14b` | 프런티어 300·텐트 90·견습생 16·브레인 7 = 413(공식 있으면 공식, 없으면 pokeemerald-kr — 출처 표시), 7바이트 이내, 저장 구조 그대로 |
| `3b356beb2e` | 성별 127(확정 인물 Male → Female, GRUNT_27 제외) — 지금은 결과 불변(포켓몬 성별 지정이 덮어씀, 실측) |

## 2. 확인한 것

- **Collector 5명**(ROSS·MITCH·JED·MARC·RICH) = HGSS 괴짜 연구원(로켓단 아지트 B1F·B3F, 라디오탑 3F·4F, 대사 원작과 같음) → 복원. **Parasol Lady**: BEVERLY(재전 포함)·RUTH = HGSS 애호가클럽♀ → 복원(`pokefan_f`), GEORGIA = 미배치(제외), JAIME·ALLAN = 크리스탈 애호가클럽(HGSS에 없음) → 크리스탈 클래스로. 프런티어 Parasol Lady는 그대로.
- 배치 확정 577(맵 오브젝트 440, 좌표·맵 스크립트 23, 재전 114), 브레인 7, 미배치 67(데이터 유지). 원작과 클래스가 다른 배치 트레이너는 적용 뒤 RICHARDO 1명(보류)뿐.
- 원작 그림이 없어 클래스만 바꾸고 그림은 그대로: 드래곤 조련사 4(→ 엘리트 트레이너 — 공개 그림으로 이번에 그림도 바뀜)·스키어 3(→ 보더)·ROY·BRET(→ 새 조련사)·THOM&KAE(→ 더블팀).
- 정정: THOM&KAE의 원작 클래스는 `더블팀`(1차 보고의 `엘리트 콤비` 아님), SoulGold `psychic_m`은 BW 그림(가져오지 않음).

## 3. 검증

- 적용 담당: 커밋마다 새 경고 0, `trainer_behavior_verify2.py`(저장소 밖) 동작 불변 위반 0, 기존 클래스·그림 번호 이동 0, 시설 그림 변경은 Collector 1건(의도).
- 리뷰 R1: 이식 전·후 ROM에서 헤드리스 mGBA로 실제 게임 함수를 돌려 651명 전원의 상금·배틀 BGM·전환·배경·승리 BGM·볼·종·랜더마이저 종이 같음(예: BEVERLY 상금 800, CODY 몬스터볼, PARKER 다이브볼, GIOVANNI 로켓 BGM). 클래스를 읽는 코드 전수 — 빠진 동작 지점 0. 버퍼·창 폭(최장 67 px, 포켓기어 69 px), 세이브·통신 구조체 크기 불변.
- 리뷰 R2: 새 그림 11개 원본과 픽셀 차이 0, ROM 그림 데이터 = PNG, 시설 그림·시설 대응 표 바이트 같음, 이름 505 = 표, 이름 918개 조사(은/는·이/가·을/를·과/와) 불일치 0, "클래스 + 이름 + 와의" 줄 최대 106 px, 프런티어 이름 칸 밖 바이트 변화 0, 성별 127 파티 결과 바이트 같음.
- 메인 검증(최종 HEAD): `3b356beb2e`(문서 커밋 `9bfd5bc189` 위) `make hns -j8` 종료 0, 새 경고 0, ROM 사용 32,772,452 B(시작 대비 +8,336), EWRAM 250,408 B·IWRAM 25,516 B 같음, SHA1 `e3ccf1fe5322dc07c2a4ce8409852dee27f6694e`(리뷰 재현값과 같음). 전체 `make check BUILD=hns -j8` 목록이 `test-baseline-hnsfix1010e.txt`와 바이트 같음(새 PASS 0·사라진 PASS 0 — 새 기준 목록 불필요). 한글 회귀 517/607, 기대 요약과 바이트 같음. 세이브 정적 비교(이식 전 사실 = 시작 HEAD 빌드) PASS(FAIL 0, WARN 0), 세이브 왕복 PASS.
- 알려진 성질: 트레이너 포켓몬 58마리의 개인값 해시 입력이 바뀜(데이터 배치 이동으로 EV 포인터 값이 바뀜 — 성격·특성은 고정이라 겉모습 값만, 원래 있던 성질).

## 4. 보류(친구 질문 대기)와 추가로 필요한 그림

> 2026-10-11 친구 답으로 대부분 정리됨 — 6절. 2026-10-12 친구 답으로 PARKER(HnS 수영팬티소년)·6클래스 그림(친구 제작 5 + SoulGold 태권왕) 해결 — 7절. 남은 것: 보더 오버월드(친구 제작 검토), 선생님·무당·웅·새 그림 5종 제작자 이름(친구가 알려 줄 때), 알로라 수영팬티소년 2명 그림(친구 질문).

- 보류: 이과계의 남자 7명 그림(`super_nerd_hns` 교체 또는 새 ID), GRUNT_27(대사 ♀·그림 ♂), NARD·RICHARDO, 추정 이름 11개(ROB·ED·DOUG·SCOTT·CHARLES·CAMERON·DAWN·WAYNE·REX)·ANN&ANNE 이름, Bugsy·Will·추정 6항목 성별, DEVELOPER 클래스·ETO×3 이름(영문), 신규 인물 30·크리스탈 전용 5 이름(공식 없음 — 영문), 연구원 그림 출처(SoulGold 판 원 제작자 미확인 — MrDollSteak 판 여부), 새 웅 그림(받아야 함). 전체 `hold_list.tsv`.
- ANN&ANNE은 동일 인물이 "추정"이라 이름·성별은 그대로 두고 그림만 HGSS 쌍둥이 공통 그림으로 바꿨다(어느 쪽 인물이든 쌍둥이 클래스 그림은 하나 — 리뷰 R2도 친구 원칙에 맞다고 봄). 되돌릴 때는 `.party`의 `Pic: Twins Hgss Hns` 한 줄.
- 원작 그림이 없는 클래스: 학원 끝난 아이 26·새 조련사 19·포켓몬 매니아 15·폭주족 14·수영팬티소년 12·태권왕 8(공개 그림 없음), 엘리트 콤비(허가 필요 — 30색이라 감색 필요)·보더(허가 문구 없음). `sprites_needed.tsv`, `public_assets.tsv`.
- 오버월드(조사만, 바꾸지 않음): 새 그림 필요 — 포켓몬 매니아 12오브젝트·중년 여성 6·보더 3, 기존 그림으로 바로 가능 — ALEX·TREVOR(`BALDING_MAN_HNS`)·ROY·BRET(`ROCKER_HNS`)·PARKER(`SAILOR_HNS`). `overworld_plan.tsv`.

## 5. 뒤 PR·다음 작업 메모

- seq 400 #9440: 볼 코드가 `trainer_util.c`로 옮겨지고 개인값 시드가 `struct Trainer` 전체 CRC가 된다 → 이식 때 `MakeTrainerGenerator`·랜더마이저가 `GetTrainerBehaviorClass`를 쓰게 하고, 새로 들어오는 `trainerClass`·`gTrainerClasses[` 사용은 표시용/동작용을 판정, 시드는 안정 값(트레이너 ID·behaviorClass·파티 내용)만 쓰게(재확인 51).
- seq 500 #9881: 새 그림 INCBIN 줄을 INCGFX 형식으로.
- 프런티어 브레인·파트너는 표시용 클래스를 읽는다(`frontier_util.c:2618`·`:3150`) — 나중에 복원하면 동작 클래스로.

## 6. 친구 답 반영 (2026-10-11, 4단계)

- 친구 답 [`FRIEND_REPLY_2026-10-11.md`](../../FRIEND_REPLY_2026-10-11.md)(질문 12개 답 + 추가 지시), 웅 그림은 디스코드 첨부 zip(SHA1 `90e74e8d…`, 저장소 밖 `/home/hjm0725/hns-sync-work/trainer-class-1011/friend-zip/`). 방식: 준비 조사 2개(그림·크레딧 `prep-G/`, 오버월드·GRUNT_27·이름 `prep-O/`) → 적용 1개(커밋 11개, 손 맞춤 0) → 리뷰 2개(수정 필요 0) → 크레딧 보완 1커밋 → 메인 검증. 자료는 저장소 밖 `trainer-class-1011/`.
- 전후 비교표(이 폴더): `phase4_trainers_before_after.tsv`(51항목 — 이름·그림·성별·오버월드), `phase4_map_objects.tsv`(맵 오브젝트 24개 + PARKER 보류), `phase4_new_ids.tsv`(새 그림 ID 222~224, 새 오버월드 ID 388·389, 출처).

| 커밋 | 내용(친구 답 절) |
|---|---|
| `d1fd6b3c0d` | NARD → `청명`, RICHARDO → `정덕`(1절, 이름만 — 클래스·그림·파티·대사 그대로). HGSS 공식: Ned = 청명, Richard = 정덕. NARD 대사 = HGSS Ned, RICHARDO 대사 = Richard 대사 고친 것이라 친구 지정과 맞다(보류 목록 후보 열은 파티 기준이라 반대였음). 조사 `청명은/이/을/과/으로`·`정덕은/…` 실행 확인 |
| `c2cc45b32e` | 웅 = 친구 최종 그림·팔레트(12절, 픽셀·색 그대로, 4비트 형식만). 세로 위치 보정은 그림 안(발끝 y60 같음, 좌표 표 변경 없음). 디버그 전용 프런티어 웅도 같은 그림 |
| `776fd00e5f` | 초능력자♂ 입 (32,20) 1픽셀을 기존 흰색(7번)으로(13절). 바깥과 이어지지 않은 투명 4곳 중 얼굴 안은 이것뿐(DP 원작 같은 자리 흰색). 턱–어깨 2·소매–허리 1은 원작도 투명이라 그대로 |
| `6d79843ce3` | 새 그림 222 `SUPER_NERD_HGSS_HNS`(SoulGold `collector.png`) → HGSS 이과계의 남자 확정 7명(ERIC·SAM·TOM·PAT·TERU·HUGH·MARKUS)(2절). `super_nerd_hns.png`(STAN·WESSEL 등)·프런티어 Collector(에메랄드 그림) 그대로 |
| `bd11e5363c` | 새 그림 223 `BOARDER_HGSS_HNS`(玉虫エリカ, 308798 3쪽, imgur `7fpQBkd` 원본과 바이트 같음) → 황토마을 체육관 보더 3명(3절). 사용 금지 문구 없음(본인 허가 문구도 없음 — 스레드 첫 글 "use these for your hack … credit this thread"가 근거) |
| `33323bae31` | 새 그림 224 `DOUBLE_TEAM_HGSS_HNS`(MrDollSteak, imgur `fmJk0yH`) → THOM & KAE(3절). 31색이 GBA 5비트 통합만으로 투명 포함 정확히 16색 — 추가 감색 0, 게임 안 색 4,096픽셀 모두 원본과 같음 |
| `37eb087f13` | GRUNT_27 = 여자 조무래기(5절): 금은·HGSS와 같은 방(금빛시티 지하 스위치 방)·같은 자리(조무래기 4명 중 오른쪽 아래), 대사 3줄 = GS `TrainerGruntF3`·HGSS `TEAM_ROCKET_F_GRUNT_3`, HGSS 같은 방 4번째 오브젝트 `rocketw`. 성별·배틀 그림(`rocket_grunt_f_hns`)·오버월드(`ROCKET_F_HNS`) — 상금·BGM·동작 클래스·파티 그대로 |
| `4e07eabe99` | 새 오버월드 388 `POKE_MANIAC_HNS`(Delta231, 그림 수정 없음 — 형식만)·389 `POKEFAN_F_HNS`(spilledpizza DP_pokefan_f), 새 팔레트 태그 `0x119D`·`0x119E`(9절). **비어 있던 388·389 사용** → `NUM_OBJ_EVENT_GFX` 548·`OBJ_EVENT_GFX_VAR_*` 549~564 등 기존 상수 1,004개 그대로(끝에 붙이면 VAR 18개가 +2 밀려 주인공 방 등 VAR 맵의 세이브가 어긋남). 남은 빈 번호 390~397 |
| `de7cbcaf0a` | 포켓몬 매니아 12개·중년 여성(선생님·애호가클럽♀·JAIME) 6개 맵 오브젝트 그래픽(9-1·9-2) |
| `bc2946c213` | ALEX·TREVOR·ALLAN → `BALDING_MAN_HNS`, ROY·BRET → `ROCKER_HNS`(9-4). **PARKER는 보류**(블루 체육관 수영장 물 칸에 서 있어 선원 그림이면 물 위의 선원 — 친구 질문) |
| `eb932ad2cc`, `6a09e9b94d` | `CREDITS.md`: 308798(Battle Trainer 1823·玉虫エリカ 추가, 더블팀·보더), SoulGold(개발자 허락, 이과계 = Battle Trainer 1823 축소 + SoulGold가 로봇 제거 — Wayback 원본과 사람 픽셀 1,085개 일치, 연구원 = 원 축소자 미확인), 오버월드 Delta231(408123 "credit this resource")·spilledpizza 외(README의 The Spriters Resource·Radiant Quartz/Prismatic Platinum team 포함), 선생님·무당·웅 = 친구 제작(이름 칸 비움, 10절) |

- 하지 않은 것(친구 답대로): 공개 그림 없는 6클래스(학원 끝난 아이·새 조련사·포켓몬 매니아·폭주족·수영팬티소년·태권왕 — 친구 제작 대기, 지금 그림 유지), 보더 오버월드(친구 제작 검토), 추정 이름 11·ANN&ANNE 이름·성별, Bugsy·Will·추정 6 성별(Male·현행 유지), DEVELOPER·ETO×3(영문 유지), 연구원 그림 교체(SoulGold 판 유지). 하인즈·우근 확정(11절 — 보류에서 제외).
- 검증
  - 적용 담당: 커밋마다 새 경고 0, `trainer_behavior_verify2.py` 동작 불변 위반 0, 기존 그림·오버월드 번호 이동 0, ROM 그림 = PNG, 오버월드 상수 717 같음·추가 4, ROM 그래픽 표 546 같음, 맵 오브젝트 24개만.
  - 리뷰 R1(오버월드·세이브·맵·GRUNT_27·이름): 정수 1,004개 독립 덤프 같음, 팔레트 태그 충돌 0, 대상 화면 오브젝트 팔레트 최대 8(기존 HnS 최대 13 안), 바뀐 맵 필드는 그래픽 번호뿐, GRUNT_27 남·여 파티 생성 시드 8개 같음, 이름 조사·폭(67·102 px). 수정 필요 0(문서 1: 이식 규칙의 빈 번호 예외).
  - 리뷰 R2(배틀 그림·크레딧): 웅 인덱스 4,096·팔레트 = 친구 파일, 222~224 원본과 픽셀·색 같음, 시설 표 3종 ROM 바이트 같음, 동작 검사 의도 변경 27(그림 24·이름 2·GRUNT_27 1), 크레딧 조건 충족. 수정 필요 0(크레딧 보완 1 → `6a09e9b94d`).
  - 메인 검증(최종 HEAD `6a09e9b94d`): `make hns -j8` 종료 0, 새 경고 0, ROM 사용 32,781,524 B(시작 대비 +7,792 — 배틀 그림 +2,640·오버월드 +5,152), EWRAM 250,408 B·IWRAM 25,516 B 같음, SHA1 `496faa3f94f1c194f8d1d3e168f4e59468a5f704`(리뷰 재현값과 같음). 전체 `make check` 목록이 `test-baseline-seq285.txt`와 바이트 같음(새 기준 목록 불필요). 한글 회귀 517/607 기대와 바이트 같음. 세이브 정적 비교(적용 전 빌드 사실) PASS(FAIL 0, WARN 0), 세이브 왕복 PASS.
- 세이브 성질: 대상 맵 안에서 저장한 세이브를 이어하면 그 맵을 다시 들어가기 전까지 그 오브젝트가 옛 그림(옛 번호도 유효)으로 보인다. 새 ROM에서 대상 맵 안에 저장한 세이브를 **이번 커밋 이전 ROM**으로 열면 388·389가 비어 있어 그 맵에서 그림이 깨질 수 있다(반대 방향은 안전) — 친구 버그 보고 때 ROM 커밋 확인.
- 실기(mGBA) 확인 항목: 웅 관장전 머그샷(새 그림은 머리가 y10부터라 머그샷 띠에서 머리가 이전보다 16px 낮게 보임 — 좌표로는 못 고침, 어색하면 친구에게)·배틀 그림·팔레트, 초능력자♂ 입(36번도로 MARK 등), 이과계(8번도로 SAM·TOM, 지하통로 ERIC·TERU), 보더(황토마을 체육관), 더블팀(47번도로), GRUNT_27(금빛시티 지하), 포켓몬 매니아(연결동굴 등)·중년 여성(15번도로 등) 오버월드 걷기, ALEX·TREVOR·ALLAN·ROY·BRET.

## 7. 친구 답 반영 (2026-10-12, 5단계 — 남은 그림 6종·웅 머그샷·PARKER)

- 친구 답 [`FRIEND_REPLY_2026-10-12b.md`](../../FRIEND_REPLY_2026-10-12b.md), 그림은 디스코드 첨부 `hns_trainer_sprites_16c.zip`(SHA1 `e419182b11ad550d286c8e062996e8012042d6b1`, PNG 5·PAL 5·README). zip은 클라우드 세션 임시 폴더에 원본 그대로 두고 파이썬 `zipfile`로 이름·속성을 확인한 뒤 풀었다(저장소에는 PNG만, 바이트 그대로). PNG 5개 모두 64×64·4비트 인덱스·PLTE 16·0번 투명, PLTE = 같은 이름 `.pal`(CRLF) → 팔레트는 PNG에서(216~224와 같은 방식).
- 전후 비교표(이 폴더): `phase5_trainers_before_after.tsv`(95항목 — 그림 전후, PARKER 클래스), `phase5_new_ids.tsv`(새 그림 ID 225~230, 파일 SHA1·git blob·출처·배정 수).

| 커밋 | 내용(친구 답 절) |
|---|---|
| `4b885582` | 웅 머그샷만 8 px 위로(1절): `Mugshots_CreateTrainerPics()`에서 상대 A(또는 B)의 그림이 `TRAINER_PIC_LEADER_BROCK_HNS`이면 `y2 -= 8`. 슬라이드는 x만 움직여 시점 그대로. `leader_brock_hns.png`·팔레트·일반 배틀 위치(트레이너 앞모습은 그림별 좌표 없이 고정 위치, 웅의 `gTrainerPicInfo` 항목 그대로)·다른 트레이너 머그샷 그대로. ELF에서 크기가 바뀐 심볼은 `Mugshot_Init` 하나(+72 B). 웅 2명(`TRAINER_BROCK_HNS`·`TRAINER_BROCK_POSTOBC_HNS`) 모두 `Mugshot: Orange` |
| `6d7ae678` | PARKER = HnS 수영팬티소년(2절, 재확인 59 해결): `d618e65415`가 바꾼 줄만 되돌림 — `Class: Swimmer M Hns`, `Pic: Swimmer M`, `Behavior Class` 줄 없음. 이름 `지광`·오버월드(물속 수영 NPC)·(9,13)·파티 그대로. 동작 클래스는 원래도 Swimmer M Hns(명시 → `behaviorClass ?: trainerClass`)라 상금·볼·BGM·AI 같음 |
| `5cc5ed84` | 친구 그림 5종(3절) — 새 ID 225 `SCHOOL_KID_HGSS_HNS`·226 `BIRD_KEEPER_HGSS_HNS`·227 `POKEMANIAC_HGSS_HNS`·228 `BIKER_HGSS_HNS`·229 `SWIMMER_M_HGSS_HNS`(기존 끝 뒤). 파일 이름만 `*_hgss_hns.png`(저장소에 이미 `biker_hns.png` = 기존 HnS 폭주족 그림이 있어 덮어쓰지 않으려고). 배정 87항목: 학원 끝난 아이 26·새 조련사 19(ROY·BRET 포함 — 폭주족·저글러 그림이던 2명)·포켓몬 매니아 15·폭주족 14·수영팬티소년 12 + PARKER(3절 "PARKER를 포함해 … 새 그림") — 대상은 `sprites_needed.tsv`의 HGSS 스토리 트레이너 목록 |
| `7e59e720` | SoulGold 태권왕(4절) — 새 ID 230 `BLACK_BELT_HGSS_HNS`. 원본 `Eemeliri/soulgold` `acfbed1dfd`(2026-10-11) `graphics/trainers/front_pics/black_belt.png`(SoulGold 마지막 변경 `77dece957` 2026-06-26 "lots of sprite updates"). 이미 64×64·팔레트 16칸(0번 투명, 사용 14)이라 형식만 8비트 → 4비트(sRGB 청크 제거) — 픽셀·PLTE·tRNS 그대로. 배정 8항목(KENJI·YOSHI·LAO·NOB·KIYO·LUNG·WAI·KENJI_2) |
| `a9f64486` | `CREDITS.md`: SoulGold 태권왕(원 축소자 미확인 — SoulGold README의 64×64 트레이너 그림 크레딧 MrDollSteak·Young-Dante를 함께 적음, 연구원 줄과 같은 방식), 친구 그림 5종을 HnS 제작 그림 줄에(이름 칸은 계속 비움) |

- 바꾸지 않은 것(보고): 목록 밖 같은 클래스 — 미배치 KAZU(폭주족)·수영팬티소년 8명(HAL·PATON·DARYL·WALTER·TONY·RICK·JAMES·LEWIS), **배치된 HnS 고유 수영팬티소년 `MELEMELE_SWIMMER_2`(JIMMY)·`AKALA_SWIMMER_3`(GEOFF)** — HGSS 원작이 없는 알로라 콘텐츠라 지금 그림(`Swimmer M`) 유지, 새 그림을 쓸지는 친구 질문. 시설(프런티어·텐트·트레이너힐) 클래스 그림은 에메랄드 그림 그대로.
- 검증(코드/데이터 — 실기는 친구)
  - 단계마다 `make hns -j8` 종료 0, 새 경고 0, 새 PNG의 libpng 경고 0. 최종 ROM 사용 32,786,900 B(시작 32,781,524 B 대비 +5,376 — 머그샷 +80·그림 6장 +5,296), EWRAM 250,408 B·IWRAM 25,516 B 같음, SHA1 `675c8d80e317292496e154e6acd27b0cc91fcbf6`.
  - 새 그림 6장: PNG 픽셀·PLTE = 원본(친구 PNG, SoulGold PNG), 빌드 `.4bpp` = PNG 인덱스, `.gbapal` = PLTE, ROM의 `gTrainerFrontPic_*HgssHns` = `.4bpp.smol`, ROM 팔레트 = `.gbapal` — 6장 모두 같음.
  - 기존 그림·시설 표: 적용 전 ROM(`600646ca43` 빌드, SHA1 `16ea4db5…`)과 이름 있는 그림·팔레트·시설 표 심볼 507개 비교 — 504개 바이트 같음, 다른 3개(`gBattleFrontierTrainers`·`gSlateportBattleTentMons`·`gSlateportBattleTentTrainers`)는 데이터 위치가 밀린 포인터 419개뿐이고 가리키는 내용은 모두 같음.
  - 트레이너 데이터(생성된 `src/data/trainers_hns.h`, `#line` 제외): `trainerPic` 95개 + PARKER `trainerClass`(선원 → 수영팬티소년)·`behaviorClass`(삭제)만 다름. 옛 그림 번호를 특별 취급하는 코드 없음.
  - 파티 생성: 1,832마리의 `CreateNPCTrainerParty` 개인값을 두 ROM에서 다시 계산 — 52마리(EV 배열이 있는 트레이너 19명: `SAMSON_OAK`·`PHANTONOMY_UNUSED`·`MELEMELE_SWIMMER_1`·`AKALA_SWIMMER_1`·`PONI_SWIMMER_1`·`PONI_SWIMMER_2`·`MILKMAN`·`MIHARU`·`YOUKO`·`SHIZUKO`·`STEVEN`·`FINLEY`·`MUALANI`·`WESSEL`·`SKYLAR`·`KOGA_POSTOBC`·`BRUNO_POSTOBC`·`RED_POSTOBC`·`MISTY_POSTOBC`, 모두 `_HNS`)의 `TrainerMon` 해시 입력이 EV 포인터 이동으로 바뀌어 개인값 위쪽 비트가 달라짐. 성격(파티 값 고정)·특성(`B_TRAINER_MON_RANDOM_ABILITY` FALSE)·IV·EV·기술·도구·성별(고정 성별 0건 변화, 10마리는 원래 `Random()` 성별)은 같음 — 3절 "알려진 성질"(데이터 배치가 밀릴 때마다 생기는 같은 현상).
  - 세이브 정적 비교(적용 전 사실 = `2b26ef58` 헤더 + `600646ca43` 빌드 ELF): PASS(FAIL 0, WARN 0) — 세이브 블록·RAM 변수 크기·위치 변화 없음. 그림 번호는 세이브에 저장되지 않음(시설 클래스만 저장).
  - 한글 회귀 517/607(기대 요약과 바이트 같음), 전체 `make check BUILD=hns -j4` 목록이 `test-baseline-seq285.txt`와 바이트 같음(새 PASS 0·사라진 PASS 0, PASS 2,650 / 5,572).
- 실기(mGBA, 친구) 확인 항목: 웅 관장전 머그샷(8 px 위 — 상대 그림은 위 32줄만 2배로 보이는 구조라 그림 아래 경계도 8 px 올라감, 화면 y 10~74 → 2~66: 띠 아래쪽에 빈 줄이 보이는지), 새 그림 6종 배틀 화면·팔레트(대상 트레이너는 `phase5_trainers_before_after.tsv`), 블루 체육관 PARKER의 클래스명 `수영팬티소년`·새 수영팬티소년 그림.

## 8. 친구 답 반영 (2026-10-12, 6단계 — 폭주족 그림 사용처·알로라 수영팬티소년 2명·크레딧)

- 친구 답 [`FRIEND_REPLY_2026-10-12d.md`](../../FRIEND_REPLY_2026-10-12d.md). 1·2절(웅 머그샷·PARKER)은 확인뿐 — 코드 변경 없음, 실기는 친구.
- **3절 `biker_hns.png` 사용처(조사만, 바꾸지 않음):** 배틀 앞모습 `graphics/trainers/front_pics/biker_hns.png`(+ `palettes/biker_hns.pal`)는 `TRAINER_PIC_BIKER_HNS` 하나에만 쓰이고, 이 그림을 쓰는 트레이너는 **미배치 `TRAINER_KAZU_HNS` 1명뿐**이다. 배틀프런티어·배틀텐트·트레이너힐의 시설 클래스 → 그림 표(`src/data/pokemon/trainer_class_lookups.h`)와 시설 데이터에는 없다. 시설에서 쓰는 `OBJ_EVENT_GFX_BIKER_HNS`(`battle_tower.c`·`battle_pike.c`)는 오버월드 그림(`graphics/object_events/pics/people/biker_hns.png`)이라 다른 파일이다. 또 `biker_hns.png`·`.pal`은 `biker_frlg.png`·`.pal`과 바이트가 같다(FRLG 폭주족 사본). 그래서 덮어쓰면 KAZU 그림도 바뀌는데, 친구 4절이 KAZU는 그대로 두라고 했으므로 지금 구조(새 그림 = 별도 ID 228 `BIKER_HGSS_HNS`)를 유지했다. 다시 정리할 필요 없음 — 친구가 원하면 `biker_hns.png`를 새 그림으로 덮고 228을 지우는 정리도 가능(그 경우 KAZU도 새 그림).
- **4절 알로라 수영팬티소년 2명:** `3fd38faf` — `TRAINER_MELEMELE_SWIMMER_2_HNS`(JIMMY)·`TRAINER_AKALA_SWIMMER_3_HNS`(GEOFF)의 `Pic: Swimmer M` → `Pic: Swimmer M Hgss Hns`만. 생성 `src/data/trainers_hns.h`(`#line` 제외) 차이 = 두 항목의 `.trainerPic` 2줄뿐, ROM 차이 = 그 2바이트뿐(`cmp`: 18 → 229) — 클래스·파티(개인값 포함)·AI·상금·볼·BGM·오버월드·시설 데이터 그대로. KAZU·미배치 수영팬티소년 8명은 그대로.
- **5절 크레딧:** `2ca118b9` — 목호 `champion_lance_hns.png` = SoulGold `champion_lance.png`(`d6354122f` 2026-07-03 이후, 현재 `acfbed1dfd`까지 같음), 심향(남자 주인공) `gold_hns.png` = SoulGold `brendan.png`(`b9a0b8128` 2026-07-22 이후 같음). 인덱스 4,096개·보이는 색 모두 같고, 빌드 팔레트(`palettes/*.pal` → `.gbapal`)도 PNG 팔레트와 같다. 두 그림 모두 `1821fd67`(친구 업로드 2026-09-26)부터 이 상태. 원 제작자는 확인하지 못해 "original artist not identified"로 적었다. `CREDITS.md`의 Ethan 플레이어 그림 출처(RafaPierangeli) 절에는 "배틀 앞모습 `gold_hns.png`는 이 출처가 아님" 예외 줄을 더했다(10-06 사용자 지시로 앞모습은 바꾸지 않았음). 태권왕 원 축소자 미확인, 친구 그림 5종 이름 칸 비움은 그대로.
  - 참고(바꾸지 않음): 금선(여자 주인공) `kris_hns.png`는 `361f1e4a`~`1821fd67` 때 SoulGold `may.png`(`84c874ed2`판)와 같았고, 지금 파일은 `eb46975f`(RafaPierangeli Lyra) → `eeb44ec3`(친구 수정본 2026-10-07)이라 SoulGold 어떤 판과도 1,245~1,264 px 다르다. 크레딧은 기존 예외 줄(HnS용으로 다시 그림) 그대로.
- **실기용 ROM:** 코드 기준 커밋 `3fd38faf`(뒤 문서 커밋은 ROM 무관), `make hns -j8`(클라우드 `arm-none-eabi-gcc 13.2.1 20231009`) → `pokehns.gba` 33,554,432 B, 사용 32,787,220 B, **SHA1 `0913b75119b0d800947261dd344589fa4ae4e076`**. full-sync seq 287~300(`full-sync-seq-287-300.md`)도 들어 있다. 다른 컴퓨터·툴체인으로 빌드하면 해시가 다를 수 있다.
- 검증(코드/데이터 — 실기는 친구): `make hns -j8` 종료 0, 새 경고 0, EWRAM·IWRAM 같음. 위 생성 데이터·ROM 바이트 비교. 그림 번호는 세이브에 저장되지 않음(세이브 비교 생략 — ROM 차이가 트레이너 표 2바이트뿐).
- 실기(mGBA, 친구) 확인 항목 추가: 멜레멜레 JIMMY·아칼라 GEOFF 배틀 앞모습 = 새 수영팬티소년 그림.
