# 재확인 70 PC 포화 — 알만 남은 파티로 배틀에 들어갈 때(임시 확인 테스트)

`zz_tmp70_eggs.c`는 플레이어 파티를 알 1개로 만든 뒤(테스트 기록 파티에 `MON_DATA_IS_EGG`를 직접 설정) 야생 배틀을 돌린다.
**게임 동작을 고정하는 회귀 테스트가 아니라 현재 동작을 보는 확인용이다** — `test/`에 남기지 않는다.

1. `cp dev_scripts/hns_verify/probe/nuzlocke70/zz_tmp70_eggs.c test/`
2. `GITHUB_ACTION=1 make check BUILD=hns -j8 TESTS="TMP70"` — `TMP70A`·`TMP70B` 줄에 배틀 결과·알 플래그·레벨·경험치·부화 걸음(우정 칸)·HP가 찍힌다.
3. `rm test/zz_tmp70_eggs.c build/hns-test/test/zz_tmp70_eggs.*`

2026-10-31 결과는 `docs/friend-handoff/results/1.17.0-port/followup-2026-10-31.md` 3절.

## `zz_tmp70b_wipe.c` (2026-10-31(2), 70 (a) 원본 복원 뒤 다른 경로 확인)

너즐록 켠 야생 배틀에서 마지막 비알 포켓몬이 반동(이판사판태클)으로 상대와 함께 쓰러지는 장면. 배틀 결과 `B_OUTCOME_DREW`(3),
배틀 끝 너즐록 삭제 뒤 파티는 알 1개. 무승부는 `IsPlayerDefeated`(`src/battle_setup.c`)가 패배로 보므로 `CB2_WhiteOut` → `DoWhiteOut`
→ 보충으로 이어진다(필드 진행은 테스트하지 않음). 실행: `cp dev_scripts/hns_verify/probe/nuzlocke70/zz_tmp70b_wipe.c test/ &&
GITHUB_ACTION=1 make check BUILD=hns -j8 TESTS="TMP70B"`, 끝나면 `rm test/zz_tmp70b_wipe.c build/hns-test/test/zz_tmp70b_wipe.*`.
