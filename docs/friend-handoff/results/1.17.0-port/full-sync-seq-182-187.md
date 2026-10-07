# full-sync 묶음 7 — seq 182·184·185·186·186.5·187 (+ seq 384 #10211 선반영)

- 시작 HEAD `ecdd8f8944`(seq 181 단위 + 친구 답 HnS 수정 뒤). 작업 컴퓨터: 데스크탑(사용자는 랩탑에서 Remote Control). 날짜 2026-10-07 밤.
- 방식: PR별 사전 분석 4개(병렬, 저장소 밖 `/home/hjm0725/hns-sync-work/chunk-182-187/seq*.md`) → 사용자 결정 6개("ㄱㄱ") → 적용 1개(커밋 7개) → 리뷰 2개 → 메인 검증. 검증에는 저장소 도구 `dev_scripts/hns_verify/`를 썼다.
- 실기(mGBA) 미확인.

## 커밋

| seq | PR | 커밋 | 판정·HnS 적응 | ROM(B) |
|---|---|---|---|---:|
| 182 | #9865 Mega Sol test adjustments | `d82b5d252f` | 일부 적용: upstream 31 hunk 중 25개 그대로, 3곳 손 맞춤, 4개 제외(`CanTwoTurnMoveFireThisTurn`은 HnS가 이미 #10416·#10675 형태 — 넣으면 만능우산 솔라빔 버그가 되살아남, `Cmd_recoverbasedonsunlight` 2개는 이미 1.17.0과 같음, 소원 함수 공백은 이미 정리). 새 `IsSmartBattle`에 HnS H1 조건(`BATTLE_TYPE_TRAINER &&`, 재확인 8p — 없으면 배회·사파리·스마트 야생 탐침 3/5 실패), `padding2:9`(HnS 필드 수), `SetAiLogicDataForTurn` HnS 선언 유지 | 32,752,068 |
| 184 | #9857 Incoming and predicted move function cleanup | `a867e4e056` | 그대로(순수 이름 바꾸기: 같은 규칙 변환을 upstream 부모에 돌리면 upstream 커밋과 바이트 같음). 옛 이름 0회. `CanAiPredictMove`는 #9575 선진행으로 이미 없음. HnS B안 교체 블록(8q) 그대로 | 32,751,924 |
| 185 | #9910 GetAvailableObjectEventId by value | `fc0d223782` | 그대로(`wild_encounter_ow.c` HnS 적응 `return TRUE`를 upstream 결과 `return OBJECT_EVENTS_COUNT`로 — 의미 같음). 호출부는 `InitObjectEventStateFromTemplate` 한 곳(동행·서핑 포켓몬 영향 없음). 슬롯 배정 기계어 같음 | 32,751,908 |
| 186 | #9890 debug menu complex actions | `5b878273c2` | 2곳 손 맞춤(HnS 선언 문맥, `UseFakeRtc()` 유지), `DebugAction_ReturnToSubMenu`가 새 메뉴 종류 값을 받게(호출 9곳 이름 상수, 값 같음). **HnS 1줄 `TRAINER1 = 1`**(사용자 결정 1): upstream 그대로면 디버그 Trainers의 "Trainer 1:" 선택이 저장되지 않음(인자 0 = NULL, upstream은 #10121 seq 449까지 같음) | 32,751,652 |
| 384 | #10211 fix(debug): incorrect use of const | `d4c0f3f1c7` | **seq 384에서 선반영**(같은 unit `U-debug-9890`, 사용자 결정 2). ROM 바이트 동일 | 32,751,652 |
| 186.5 | #9920 Basic daily seed | `e3637df42b` | **확정 결정 A**: `u32 dailySeed`를 `SaveBlock3` 끝(`registeredItemHold` 뒤)에. SaveBlock1 `saveVersionMagic`(0x9C2) 불변. `UpdateDailySeed`는 `gSaveBlock3Ptr`, `Crc32B`는 upstream대로 `battle_main.c` → `random.c` | 32,751,716 |
| 187 | #9877 Add Dynamic Weather Option | `ccbf3123a9` | **`WEATHER_LEAVES=23` 유지, `WEATHER_DYNAMIC=24`, `WEATHER_COUNT=25`**(확정 결정). `TranslateWeatherNum` HnS 낙엽 분기 보존, 동적 날씨 해시는 `gSaveBlock3Ptr->dailySeed`. upstream 문서 2개(`docs/SUMMARY.md` 1줄, `docs/tutorials/how_to_dynamic_weather.md`) 포함. 디버그 날씨 이름표에 `DYNAMIC`은 넣지 않음(사용자 결정 6, upstream과 같이 `NOT DEFINED!!!`) | 32,751,812 |

- 순서표 처리: seq 384 #10211은 닿으면 **"이미 적용(seq 186 단위 선반영)"**. 같은 unit 뒤 행: `U-debug-9890` 232 #10194(독립), `U-dailyseed-9920` 202 #10050(새 게임 daily seed), 359 #10012, 368 #9955, 420 #10383.

## 동작 변화 (받아들인 것, 사용자 결정 4·5 — 친구 보고)

- **#9865 AI 날씨 판단:** 전지(omniscient) 플래그가 없는 AI(일반·Basic 트레이너, 프런티어)는 AI가 아는 특성으로 날씨 억제를 판단한다. 플레이어가 고라파덕·골덕·내루미·내룸벨트·파비코·파비코리·할비롱을 구름보호가 아닌 특성으로 내고 특성이 아직 안 드러났으면, 날씨 턴마다 1/3~1/2 확률로 날씨를 무시한다(탐침: 왕구리가 웨더볼 대신 누르기). 스마트 트레이너 27명은 전지 플래그가 있어 영향 없음. 부스터에너지 속도 1.5배가 특성이 사라진 뒤에도 남는 upstream 동작(HnS는 랜더마이저에서만 닿음).
- **#9857 AI 예측:** `AI_FLAG_PREDICT_MOVE` 트레이너 25명(엔딩 뒤 재대결 22명, 스티븐·FINLEY·MUALANI)만. 예측한 기술이 변화기·속이기여도 그 우선도로 속도 비교, 스마트 교체가 예측 기술을 그대로 씀. 실측(seq 181 AI 비교 도구, 트레이너 650명): 첫 턴 0/15,328, 더블 0/208, 5턴 결정 57,379건 중 5건 변화(예측 용의춤 때문에 저수·건조피부로 교체하지 않음 4, 예측 방어 때문에 날개쉬기 1).
- #9890: 디버그 메뉴 B 버튼 복귀·Flags 토글 등 구조 변경(동작 같음).

## 검증

- **빌드(최종 `ccbf3123a9`):** 커밋 7개 모두 종료 코드 0, 새 경고 0. ROM 32,752,628 → **32,751,812 B(−816)**, **EWRAM 250,404 → 250,408 B(+4, `gSaveblock3`)**, IWRAM 25,516 B, SHA1 `b6626f5b4f8909c9ad4f2cd610978a85904607b1`.
- **전체 테스트:** PASS 2,518 → **2,521** / TOTAL 5,424 → 5,426. 목록 차이 6줄은 모두 `mega_sol.c`(새 PASS 4 = 이름 바뀐 Growth·Solar Beam + 새 Aurora Veil·Electro Shot, 사라진 줄 = 옛 이름). **사라진 PASS 0.** 목록 [`test-baseline-seq187.txt`](test-baseline-seq187.txt)(다음 비교 기준).
- **한글 회귀(584개):** 494/584, 기대 요약과 바이트 동일.
- **세이브(#9920):**
  - 정적 비교(이식 전 대비): FAIL 7·WARN 1이 모두 `SaveBlock3` 52 → 56 B 하나에서 나옴 — 끝 4 B에 `dailySeed`(기존 필드 위치·폭 변화 0), 크기 상수가 바뀐 4함수(`ClearSav3`, `HandleWriteSector`, `HandleReplaceSector`, `LoadGameSave`), `gSaveblock3` +4 B. SaveBlock1(15,888 B)·SaveBlock2(4,020 B)·PokemonStorage(34,384 B)·섹터 배치 14칸·`sGFRomHeader`·상수 30개 같음. #9877은 세이브 변화 0.
  - 세이브 왕복: 이식 전 세이브 두 개 모두 LOAD/MAKE/NEWGAME/RESAVE PASS, 섹터 0~30 바이트 동일. `load.txt` 차이는 sb3 해시 1줄(`3b703d18` → `919cf698`)뿐이고, 이식 전 세이브의 `dailySeed`는 0(이미지에서 직접 계산). → 검증 도구 기준 `dev_scripts/hns_verify/save/savetest/baseline/pre-load-*/load.txt`의 sb3 해시를 갱신했다(README 5절에 기록).
  - 기존 세이브는 그대로 불러와지고, 첫 날짜 변경 때 `dailySeed`가 생긴다.
- **동적 날씨 기능(사본 전용 테스트, 커밋 안 함):** 날씨 번호, `Crc32B` 표준값, 동적 날씨가 결정적이고 기본 날씨 7종 안에서만 나옴, 날짜가 바뀔 때만 씨앗 갱신 — 4/4 PASS. 동적 날씨를 쓰는 HnS 맵은 없어 일반 게임 날씨는 바뀌지 않는다.
- **리뷰 2개: 수정 필요 0.**
  - R1(#9865·#9857): 커밋 = 사전 patch, 제외 4 hunk·손 맞춤 3곳 근거 맞음. #9857 88 hunk의 바뀐 이름이 upstream과 같고 옛 이름 0회, `GetPredictedMove`·`GetIncomingMove` 본문이 1.17.0과 바이트 동일. H1(8p)·B안 블록(8q, md5 같음) 보존, X6과 겹치는 함수 없음(X6 테스트 24개 PASS). 두 커밋만 되돌린 사본과 비교한 AI 실측이 분석 때 로그와 바이트 동일(첫 턴 0, 5턴 5건, 더블 0).
  - R2(#9910·#9890·#10211·#9920·#9877): 5개 모두 upstream과 같고 다른 곳은 의도한 HnS 적응뿐. 디버그 메뉴 118항목 인자 개수 이상 0, ROM Trainers 표 1/2/3. 세이브: 다른 이식 전 빌드(`820148e779`)로 따로 만든 기준과 비교해도 차이 파일이 적용 결과와 바이트 동일, `dailySeed` 바이트 52, 섹터 여유 56 ≤ 116, `saveVersionMagic` 바이트 4320 그대로, HnS 맵 헤더 539개 데이터 차이 0(LEAVES 1개, 24 이상 0개), 이식 전 세이브의 52~55 바이트 0, 새 게임은 `ClearSav3`로 0, 진짜·가짜 RTC 모두 같은 `UpdatePerDay` 경로로 갱신. 처음부터 다시 빌드해 SHA1 같음.
  - 정보: 뒤 행 #10050·#10012·#9955는 upstream이 `gSaveBlock1Ptr->dailySeed`라 `gSaveBlock3Ptr`로 바꿔야 함(재확인 31). #9890 HnS 1줄은 #10121(seq 449) 뒤 지워도 됨.
- **메인 검증(같은 HEAD):** 재빌드 SHA1 같음, 새 경고 0, 전체 테스트 목록이 적용 결과와 같음, 한글 584개 기대와 바이트 동일, 세이브 정적 비교 FAIL 7·WARN 3(전부 SaveBlock3 +4 B와 이전부터의 WARN), 갱신한 기준으로 세이브 왕복 PASS.

## 실기(mGBA) 확인 항목

- 디버그 메뉴(필드에서 R을 누른 채 START): Trainers "Trainer 1:" 선택, Flags 토글 색, 각 하위 메뉴 B 버튼 복귀, 날씨 메뉴 24번(`NOT DEFINED!!!`)
- 기존 세이브 불러오기·저장·다시 불러오기(SaveBlock3 +4 B), 날짜 변경 뒤 저장
- 날씨 맵(낙엽 날씨 포함) 진입
- 비·쾌청 날씨 트레이너전에서 플레이어 고라파덕류를 낼 때 상대 AI 기술 선택(참고용)
