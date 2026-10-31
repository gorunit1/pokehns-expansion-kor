# 재확인 34 노가드 × 프리폴 — 지금 HnS와 #10180 제외 hunk 비교(임시 확인 테스트)

친구 결정(2026-10-31): **hunk는 넣지 않고 지금 HnS 유지.** 이 폴더는 비교 조건을 다시 돌려 볼 수 있게 남긴 것이다 — `test/`나 게임 코드에 남기지 않는다.

- 장면(`zz_r34_noguard_skydrop.c`): 더블. 플레이어 왼쪽 마자용(스피드 10, 가장 빠름)이 프리폴로 상대 왼쪽을 들어 올린 턴에
  명중 50 `MOVE_DYNAMIC_PUNCH`를 **강제 빗나감 난수(`hit: FALSE`)**로 쓴다. 명중·회피 랭크, 도구, 날씨 등 보정 없음.
  - case 0: 노가드 괴력몬(상대 오른쪽) → 프리폴 사용자(플레이어 왼쪽)
  - case 1: 노가드 괴력몬(상대 오른쪽) → 들린 아군(상대 왼쪽)
  - case 2: 노가드 괴력몬(플레이어 오른쪽) → 들린 상대(상대 왼쪽)
  - case 3: 일반 마자(플레이어 오른쪽) → 들린 노가드 괴력몬(상대 왼쪽, 노가드 방어자)
  - case 4: 대조 — 노가드 없음, 마자 → 들린 상대
- 지금 HnS 기대(파일 그대로): case 0~3 기술 애니메이션 나옴(명중 판정 생략), case 4 안 나옴(반무적).
- hunk 판: `src/battle_util.c` `CanMoveSkipAccuracyCalc`의 노가드 두 분기 조건 `!IsSkyDropInvolved(battlerDef, moveEffect)`를
  upstream 1.17.1처럼 `!(moveEffect != EFFECT_SKY_DROP && IsBattlerInvolvedInSkyDrop(battlerDef))`로 바꾸고, 이 파일의 case 0~3 기대를
  `NOT ANIMATION(...)`으로 뒤집어 돌린다(2026-10-30 결과: case 0~3 모두 빗나감, case 4 같음). 끝나면 `src/battle_util.c`를 되돌리고 ROM SHA1 확인.
- 실행: `cp dev_scripts/hns_verify/probe/noguard34/zz_r34_noguard_skydrop.c test/battle/ && GITHUB_ACTION=1 make check BUILD=hns -j8 TESTS="ZZ r34"`,
  끝나면 `rm test/battle/zz_r34_noguard_skydrop.c build/hns-test/test/battle/zz_r34_noguard_skydrop.*`.

결과·한계(명중 100 기술은 이 보정 없는 조건에서만 차이 없음 등)는 `docs/friend-handoff/results/1.17.0-port/followup-2026-10-31.md` 4절.
