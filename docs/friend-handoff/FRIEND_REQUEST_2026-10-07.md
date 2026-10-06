# 친구 지시서 — 2026-10-07 전달 (트레이너 그림 3장, 배틀타워 엘리베이터 직원, Gen4 HP 박스 레벨 중복)

사용자가 2026-10-07 데스크탑 세션에 전달한 친구 지시서를 결정이 빠지지 않게 옮긴 요약이다. 첨부 그림 3장은 저장소 밖 `/home/hjm0725/hns-sync-work/hnsfix-1007/friend-sprites/`에 보관했고 커밋 `eeb44ec376`로 그대로 넣었다. 결과: [`results/1.17.0-port/full-sync-hnsfix-2026-10-07.md`](results/1.17.0-port/full-sync-hnsfix-2026-10-07.md)

기준: `gorunit1/pokehns-expansion-kor` `pokehns-expansion-kor`. 요청 범위만 최소 수정.

## 1. 트레이너 스프라이트 교체
- `graphics/trainers/front_pics/kris_hns.png`, `graphics/trainers/back_pics/lance_hns.png`, `graphics/trainers/back_pics/silver_hns.png`를 첨부 수정본으로 그대로 교체. 이미지 규격·팔레트·빌드 처리는 기존 HnS 규칙 유지. 빌드에서 그래픽 오류 없는지 확인.
- 첨부본 sha1: `kris_hns.png` `71a6b6c0…`, `lance_hns.png` `94c7187c…`, `silver_hns.png` `8d1e393c…`

## 2. 배틀타워 엘리베이터 직원 위치
- 로비 직원 이동은 mGBA에서 정상 확인.
- `data/maps/BattleFrontier_BattleTowerElevator_hns/map.json` 직원 시작 (2,5) → Emerald 원본 (1,5). 플레이어 워프 (1,6), 이동 스크립트·`BattleElevator_hns/map.bin`은 Emerald와 같은 blob. 지금 HnS는 이동 뒤 플레이어 (1,4)·직원 (3,4)로 한 칸 떨어짐, Emerald는 (1,4)·(2,4). x 2→1로 수정하고 최종 (1,4)·(2,4) 확인.

## 3. HnS Gen4식 HP 박스 기믹 아이콘 + 레벨 중복
- 재현: 배틀타워 Lv.50, Lv.50 포켓몬, 메가진화, HnS Gen4 배틀 UI → `50 [메가 아이콘] 50`.
- 원인 추정: `src/battle_interface.c` `UpdateLvlInHealthbox()`에서 기믹 표시가 있으면 `text`에 레벨을 쓰고, `UseGen4BattleUI()` 처리에서 `ConvertIntToDecimalStringN(text + 2, …)`로 또 씀.
- 요청: Gen4 UI에서 레벨 문자열을 다시 쓰지 않고 `levelDigits`만 따로 계산(예: lvl ≥100 → 3, ≥10 → 2, 그 밖 1; `xPos = 5 * (3 - levelDigits)`). 정확한 구현은 현재 구조에 맞게. SoulGold Gen4 HP 박스 구현 참고 가능(대규모 이식 금지).
- 유지: 기믹 아이콘 위치, 아이콘·레벨 간격, Lv.1/50/100 자릿수 보정, 기믹 없을 때 표시, Gen4가 아닌 UI 동작.

## 4. 확인 항목
- A: Gen4 UI, 기믹 없음, Lv.50 → 레벨 한 번
- B: Gen4 UI, Lv.50, 메가진화 → 중복 사라짐, 간격 유지, 가능하면 SoulGold 배치와 비교
- C: Gen4 UI, Lv.100, 기믹 → 잘림·겹침·위치 이상·중복 없음
- D: 다른 `GetIndicatorPalTag()` 기믹 하나 이상

## 5. 완료 후 알려줄 것
1. 변경 파일 목록 2. 변경 요약 3. 최종 커밋 SHA 4. 빌드 성공 여부 5. 검증 결과(엘리베이터 최종 위치, A~D) 6. 그래픽 교체 뒤 빌드·표시 문제 여부 7. SoulGold 참고 내용과 HnS 반영 방식.
- 요청 외 번역·UI·배틀 동작은 임의로 바꾸지 말 것.
