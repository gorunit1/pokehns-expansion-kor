# seq 127 #9655 unit 리뷰 — 공통 지시 (읽기 전용 리뷰 에이전트용)

## 대상

- 저장소 `/home/jinmo/pokehns-expansion-kor`의 **커밋되지 않은 작업 트리 변경**(`git diff`, 기준 HEAD `05319fd9b7`). 154파일, upstream #9655 `32fcd64868` + #9856 `c1eaced09e`(흡수) + #10064 `3ed1ce5570`(엔진 1줄) 이식분이다.
- 이 변경은 사전 분석 4개(`/home/jinmo/hns-sync-work/chunk-127/part-{A,B,C,D}.md`·`.patch`)를 합친 것이다. 빌드는 메인이 확인했다(`make hns` 종료 코드 0, ROM +256 B, 새 경고 0). 지금 메인이 전체 테스트를 돌리는 중이다.
- upstream 저장소: `/home/jinmo/pokeemerald-expansion-upstream`(읽기만). 1.17.0 최종형은 `expansion/1.17.0` 태그 또는 해당 브랜치로 확인한다.

## 기준 문서

- `/home/jinmo/hns-sync-work/chunk-127/BRIEF.md` — 사전 분석 공통 지시. **"확정 결정(D1~D7)"과 "REPORT 함정 준수" 절이 리뷰 기준이다.**
- `docs/friend-handoff/FRIEND_REPLY_2026-10-01.md` — 친구 결정 원문
- `docs/friend-handoff/results/1.17.0-port/pre-9655/REPORT.md`
- `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`(HEAD판 = 이식 전 HnS 출력 정책)
- 해당 파트의 `part-X.md`(분석 에이전트의 의도와 근거)

## 규칙

- **아무것도 수정하지 않는다.** 메인 저장소의 파일·git 상태를 바꾸지 않는다(`git diff`, `git show`, `git grep`, 파일 읽기만). make·빌드·테스트를 실행하지 않는다(메인이 테스트 중, 메모리 7 GB).
- 결과는 `/home/jinmo/hns-sync-work/chunk-127/review-RX.md`에 쓴다. 각 지적에 다음을 적는다.
  - 등급: **수정 필요**(버그·결정 위반·한글 손상·범위 밖 읽기 등) / **경미**(동작 영향 없음, 기록 권장) / **확인만**(의도된 차이)
  - 파일:줄, 근거(코드 인용), 구체적 실패 시나리오(입력 → 잘못된 출력), 제안 수정
- 추정과 확인을 구분한다. 확인하지 못한 것은 "미확인"으로 적는다.
- 최종 응답은 20줄 이내: 등급별 건수와 "수정 필요" 항목 요약.
