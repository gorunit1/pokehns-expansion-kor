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
