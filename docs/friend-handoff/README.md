# 친구 작업 지시서

친구에게 맡길 작업의 목표, 범위, 진행 상황, 검증 결과를 이 폴더에 모아 둡니다.

## 작업 시작 전에 읽을 문서

- [`CURRENT_HNS_HANDOFF.md`](CURRENT_HNS_HANDOFF.md) — 현재 브랜치·보존 대상·핵심 기능 상태 요약
- [`docs/localization/STATUS.md`](../localization/STATUS.md) — 현재 작업 상태와 남은 문제
- [`docs/localization/WORKFLOW.md`](../localization/WORKFLOW.md) — 작업·검증·인수인계 절차
- [`docs/localization/SESSION_LOG.md`](../localization/SESSION_LOG.md) — 최근 작업 기록

## 작업 회신

- [2026-09-26 작업 회신](HANDBACK_2026-09-26.md) — 빌드 복구, 1.17.0 인벤토리, 묶음 A 35개 PR 이식
- [2026-09-28 작업 회신](HANDBACK_2026-09-28.md) — 전체 엔진 동기화 0~1단계: ROM 여유 확보, 인벤토리 재확정·이식 계획
- [2026-09-29 full-sync port 시작 인수인계](HANDBACK_2026-09-29_FULL_SYNC_PORT_START.md) — 로컬 통합·선행 이식·확정 결정을 반영한 517개 실제 port 시작점
- [Claude full-sync 실행 지시문](CLAUDE_FULL_SYNC_PORT_PROMPT.md) — 친구가 Claude Code/Claude에 전달해 실제 port를 시작할 완성 지시문
- [2026-09-29 port 진행 회신](HANDBACK_2026-09-29_PORT_PROGRESS.md) — full-sync seq 1~62 이식, 테스트 러너 복구, 다음 seq 63
- [2026-09-30 회신](HANDBACK_2026-09-30.md) — seq 92~101, Safari UI 한글화 버그 2건 수정(재확인 요청), 순서표 보정, 다음 seq 102 (이후 seq 127 대기·선진행 구간 1까지 갱신)
- [2026-10-01 친구 답장](FRIEND_REPLY_2026-10-01.md) — #9655 D1~D7 확정, 선진행 중단, #9006 문장 확정, MCP 시험 도입 승인
- [2026-10-01 회신](HANDBACK_2026-10-01.md) — seq 127 #9655(+#9856·#10064) 이식, 질문 3건, mGBA 확인 요청, 다음 seq 128
- [2026-10-04 회신](HANDBACK_2026-10-04.md) — seq 138.5 #8943 12v12 단위(+후속 7개·HnS 보호 수정), 지닌 도구 팝업 Champions 대조(확인 질문 11개), `B_MULTI_HALF_TEAMS` FALSE 확정 반영, 인트로 주인공 위치, 다음 seq 139
- [2026-10-05 회신](HANDBACK_2026-10-05.md) — full-sync 묶음 1~4(seq 142~170, 167 보류), 슬레이트포트 텐트 파티 수·KO 애니 소프트락 등 HnS 수정, 친구 답(팝업 Q1~Q11·조사 문제) 반영, 묶음 5(seq 171~174, AI 기본 교체 HnS 유지 보고), seq 175 #8434 OWE(꺼 둠, +#10020), 다음 seq 176
- [2026-10-06 친구 답장](FRIEND_REPLY_2026-10-06.md) — AI 교체·OWE 유지, 질문 5개 모두 수정, 배틀타워 질문 3개, 심향·금선 그래픽 지시서([`ETHAN_LYRA_PLAYER_GRAPHICS.md`](ETHAN_LYRA_PLAYER_GRAPHICS.md), 2026-10-06 적용·실기 대기)
- [2026-10-06 회신](HANDBACK_2026-10-06.md) — 문자열 4줄(`메일`·`메일을 읽는다`·`!`·`아니오`), 배틀타워·배틀돔 로비 직원 번호, 배틀프런티어 동선, 심향·금선 그래픽([결과](results/ethan-lyra-player-graphics-2026-10-06.md)), 다음 엔진 수정 3건
- [2026-10-07 회신](HANDBACK_2026-10-07.md) — 친구 지시서 3건(금선 앞모습·목호·실버 뒷모습, 배틀타워 엘리베이터 직원, Gen4 HP 박스 레벨), 엔진 수정 3건(3b 춤추기·3c 불복종 자해·10f 통신 문장), full-sync 묶음 6(seq 176~179, 198 선반영), 다음 seq 181

## 시험 도입 중인 제안

- [AI가 mGBA MCP로 실기 확인 일부를 대신하기](PROPOSAL_AI_MGBA_CHECKS.md) — **2026-10-01 시험 도입 승인**(`AI 사전 확인` 용도). 첫 시험은 seq 107 #7305 세이브 호환 대조이고, 아직 설치하지 않았다.

## 작업 목록

구체적인 지시는 작업별 Markdown 파일로 추가합니다.

- [x] [현재 HNS 작업 인수인계](CURRENT_HNS_HANDOFF.md)
- [ ] [pokeemerald-expansion 1.17.0 안전 업데이트](POKEEMERALD_EXPANSION_1.17.0_UPDATE.md) — 진행 중: 인벤토리·묶음 A 완료, [결과 보고서](results/pokeemerald-expansion-1.17.0-update-report.md)
- [ ] [pokeemerald-expansion 1.17.0 전체 엔진 동기화](POKEEMERALD_EXPANSION_1.17.0_FULL_ENGINE_SYNC.md) — 묶음 A 이후 남은 engine-relevant upstream 변경을 HNS 한글화·커스텀과 공존하도록 끝까지 이식 — 진행 중: 0~1단계 완료, [이식 계획](results/pokeemerald-expansion-1.17.0-full-sync-plan.md)
- [ ] 작업 제목과 목표 작성
- [ ] 수정 대상 파일과 변경 범위 작성
- [ ] 재현·검증 방법 작성
- [ ] 완료 후 결과와 남은 문제 기록

## 작업 지시서 양식

```markdown
# 작업 제목

## 목표

무엇을 완료해야 하는지 적습니다.

## 작업 범위

- 수정할 파일:
- 수정하지 않을 파일:
- 참고할 자료:

## 완료 조건

- [ ] 코드 또는 데이터 변경
- [ ] 필요한 테스트·빌드 실행
- [ ] 결과 기록

## 주의사항

기존 변경사항을 되돌리거나 일괄 `checkout`, `reset`, `clean`하지 않습니다.

## 결과

작업 후 변경 파일, 검증 명령, 결과, 남은 문제를 적습니다.
```
