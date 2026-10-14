# 번역 대기 목록

기존 HnS 텍스트에서 발견했지만 **고치지 않고 남겨 둔** 번역 문제를 모은다. 친구가 직접 번역할 때 처리한다. 처리할 때는 공식 한국어 원문, 조사 토큰, 줄바꿈, 화면 폭을 확인한다. 여기 있는 항목을 임의로 고치거나 다른 기존 문장을 같이 바꾸지 않는다(`TRANSLATION_POLICY.md` 5절).

| 날짜 | 위치 | 지금 문장 | 문제 | 참고 | 발견 경위 |
|---|---|---|---|---|---|
| 2026-10-14 | `src/battle_message.c` 881행 `STRINGID_WAGGLINGAFINGER` | `손가락을 흔들었더니\n{B_CURRENT_MOVE} 나왔다!` | 기술 이름 뒤에 이/가 조사가 없다. 출력 예: `손가락을 흔들었더니⏎스피드스타 나왔다!` | 공식 SV·PLZA는 기술 이름 뒤에 이/가 조사가 붙는다(Poké Corpus `[VAR 1900(0003)]`, 묶음 21 사전 분석 a 확인). HnS 조사 토큰 쓰는 법은 다른 배틀 문장 참고 | 묶음 21 seq 337 #10661 한글 조사 테스트. 친구 답 2026-10-14(3) 9절: 번역 대기로 남김. **고칠 때 같이:** 한글 회귀 `HNSB2021`의 A4a·A4b·A4e·B1~B4·B7~B9 기대 문장(`dev_scripts/hns_verify/kortests/sets/zz_hnsb2021_kor.c`)과 기대 요약 |
| 2026-10-14 | `src/battle_message.c` `STRINGID_PKMNSTORINGENERGY`(테라스탈 스크립트 `BattleScript_Terastallization`·`BattleScript_TeraFormChange`가 씀) | `…참고 있다`(참기 문장) | 같은 ID를 테라스탈 문장으로도 써서 레드의 테라스탈 때 참기 문장이 나온다(이식 전부터, 코드 해석, 실기 미확인). 문장 번역 문제가 아니라 ID 공유 문제일 수 있음 — 고치려면 테라스탈용 문장 분리 여부부터 친구 결정 | 공식 SV 테라스탈 문장 확인 필요 | 묶음 23 seq 361 사전 분석 c |
