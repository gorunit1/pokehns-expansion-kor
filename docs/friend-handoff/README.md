# 친구 작업 지시서

친구에게 맡길 작업의 목표, 범위, 진행 상황, 검증 결과를 이 폴더에 모아 둡니다.

## 작업 시작 전에 읽을 문서

- [`docs/localization/STATUS.md`](../localization/STATUS.md) — 현재 작업 상태와 남은 문제
- [`docs/localization/WORKFLOW.md`](../localization/WORKFLOW.md) — 작업·검증·인수인계 절차
- [`docs/localization/SESSION_LOG.md`](../localization/SESSION_LOG.md) — 최근 작업 기록

## 작업 목록

구체적인 지시는 작업별 Markdown 파일로 추가합니다.

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
