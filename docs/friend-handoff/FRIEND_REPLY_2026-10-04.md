# 친구 답장 — 2026-10-04 전달 (HANDBACK_2026-10-01 5~9절에 대한 답)

친구가 보낸 답장을 사용자가 2026-10-04에 전달했다. 아래는 결정을 빠짐없이 옮긴 요약이다.

## 결정 요약

| 항목 | 결정 |
|---|---|
| seq 138까지 진행 | 괜찮다. 다음 #8943도 정해 둔 A안 그대로 진행 |
| 질문 1. 탈출버튼·탈출팩 뒤 매직미러·매직코트·가로채기 assert | **고친다.** assert만 막는 것이 아니라, 교체 전 포켓몬의 일시적인 매직코트·가로채기 상태가 새로 나온 포켓몬에게 승계되는 기존 동작도 정리한다. 새로 나온 포켓몬 자체가 매직미러 특성이라 실제로 반사하는 것은 괜찮다 |
| 질문 2. 더블배틀에서 가로챈 멀리짖기 assert | **지금은 임시로 고치지 않고 기록만 한다.** 이식 전의 "능력이 하나도 안 오르는 동작"으로 억지로 되돌리지도 않는다. 대상·능력 상승 주체를 제대로 정리할 수 있을 때 고치고, 이후 upstream 수정이 있으면 그것을 우선 반영한다. **full-sync 완료 전 재확인할 버그 목록에 남긴다** |
| 질문 3. 녹화 배틀 플레이어 뒷모습 | **Gold/Kris로 바꾼다.** 실제 배틀·통신·사파리가 이미 Gold/Kris인 것과 맞춘다 |
| 질문 4. 전자부유 종료 때 다른 포켓몬 이름 | **한 줄 수정한다** |
| #8943 | 기존 결정 그대로: 녹화 배틀 기록 무효화 허용, 일반 세이브 호환 유지 |
| mGBA 확인 | 친구가 별도로 확인 결과를 보낸다(지금 우리 브랜치를 빌드 중) |

## 반영

| 내용 | 위치 |
|---|---|
| 질문 1 | HnS 수정 커밋(아래 `full-sync-hnsfix-2026-10-04.md`) |
| 질문 2 | [`results/1.17.0-port/RECHECK_BEFORE_COMPLETION.md`](results/1.17.0-port/RECHECK_BEFORE_COMPLETION.md) 1번 |
| 질문 3 | `d78de7fdb5` HnS: draw Gold/Kris as the player in recorded battle playback |
| 질문 4 | `d37911167c` HnS: name the Magnet Rise user when Magnet Rise ends |
| 결과 기록 | [`results/1.17.0-port/full-sync-hnsfix-2026-10-04.md`](results/1.17.0-port/full-sync-hnsfix-2026-10-04.md), `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`, STATUS, SESSION_LOG, `HANDBACK_2026-10-01.md` 10절 |

## 2. 두 번째 답 — `B_MULTI_HALF_TEAMS`, 인트로 주인공 위치 (디스코드, 2026-10-04)

사용자가 #8943 이식 전에 디스코드로 물은 질문과 친구 답을 2026-10-04에 전달했다.

**사용자 질문 요지:** 승인된 A안대로 `B_MULTI_HALF_TEAMS = FALSE`로 하고 로켓단 아지트 멀티 트레이너 4명(ARIANA_1, GRUNT_23, LANCE_3, CLAIR_3)에 `Multi Party: Half`를 붙인다. 그러면 로켓단 아지트 말고도 바뀌는 곳이 있다.
- 두 트레이너 동시 발견 6쌍(이단 체육관 MIKE+LOLA, PAUL+FRAN, 라디오타워 4층 GRUNT_9+GRUNT_28, 46번 도로 TED+ERIN, 아쿠아호 DEBRA+JONAH, EDWARD+COREY)에서 4마리 트레이너가 4마리를 낸다.
- 배틀타워 멀티룸 화면이 풀 팀 방식이 된다(프런티어 상대 141만 반 팀 화면).
- 선택지는 (a) FALSE 유지, (b) TRUE로 이식 전과 같게, (c) FALSE + 6쌍의 4마리 트레이너도 Half였다.

| 항목 | 친구 결정 |
|---|---|
| `B_MULTI_HALF_TEAMS` | **(a) FALSE 유지.** HGSS도 2인 트레이너전에서 각 트레이너가 자기 파티를 쓰는 구조라, 6쌍의 4마리 트레이너가 4마리를 쓰는 쪽이 원작에 더 가깝다. (b)·(c)로 바꾸지 않는다 |
| 배틀타워 멀티 | 편성 2+2는 그대로이므로 파티 메뉴 L/R 파트너 페이지와 전투 전 2페이지 표시는 #8943 UI 변화로 받아들이고 출력·동작 변화에 기록한다. 친구가 나중에 mGBA로 확인한다 |
| 로켓단 아지트 4명 `Multi Party: Half` | 그대로 진행 |
| 배틀타워 상대 141 반 팀 화면 | 원작 동작이 아니라 ID 공유 부수 효과로 본다. **가능하면 로켓단 아지트용 Half가 배틀타워 상대 141의 화면까지 바꾸지 않도록 분리할 수 있는지 검토.** #8943 진행을 막을 필요는 없고, 어려우면 변화로 기록 |
| 새 요청: 인트로 주인공 위치 | `src/oak_speech_hns.c`에서 이름 입력 화면에서 돌아온 뒤(`CB2_NewGameHnsSpeech_ReturnFromNamingScreen`)만 주인공 스프라이트가 `y = 60`이라 20px 위로 올라가 보인다. 다른 구간처럼 **전 구간 x = 120, y = 80으로 통일**하고, 숫자 대신 기존 상수 `NEW_GAME_SPEECH_PLAYER_Y`를 쓴다. 친구가 인트로를 고치다 빠뜨린 것이라, 나중 통합을 위해 이쪽에서 일괄로 고쳐 달라고 했다 |

### 반영

| 내용 | 위치 |
|---|---|
| FALSE 유지·배틀타워 UI 변화 기록 | `results/1.17.0-port/full-sync-seq-138.5-138.5.md` "사용자·확정 결정"·"출력·동작 변화" 1, `RECHECK_BEFORE_COMPLETION.md` 23 |
| 상대 141 분리 검토 | `results/1.17.0-port/full-sync-seq-138.5-138.5.md` "배틀타워 상대 141" 절 |
| 인트로 주인공 위치 | `5bbd01e69f` HnS: keep the new-game speech player sprite at the same height after naming(이름 입력 복귀·마지막 등장·스프라이트 생성 좌표를 `NEW_GAME_SPEECH_PLAYER_Y`로) |
