# 재확인 114 최종 지급 실패 분기 — 강제 재현(임시, 게임·test/에 남기지 않음)

`TryGiveNextStolenWildItem`(`src/battle_util.c`)의 마지막 분기(가방·PC 모두 실패)는 정상 흐름에서 닿지 않는다(이유: `followup-2026-10-31b.md` 8절).
도구가 실제로 어디로 가는지 보려고, "훔친 도구를 드롭 도구보다 먼저 지급"하는 처리를 잠시 빼서 일부러 닿게 한다.

1. `src/battle_script_commands.c` `BS_TryGiveDroppedItems`의 `if (TryGiveNextStolenWildItem(TRUE)) return;` 두 줄을 잠시 지운다.
2. `cp dev_scripts/hns_verify/probe/stolen114/zz_tmpq114.c test/ && GITHUB_ACTION=1 make check BUILD=hns -j8 TESTS="TMPQ"`
   — `TMPQ after:` 줄에 가방·PC의 자뭉열매 수, 야생 파티 0번 칸 도구가 찍힌다.
3. 되돌리기: 1을 원래대로(`git checkout -- src/battle_script_commands.c`는 그 파일에 다른 변경이 없을 때만), `rm test/zz_tmpq114.c build/hns-test/test/zz_tmpq114.*`.

2026-10-31(2) 결과: 마지막 분기 실행(승리 끝, 가방 999는 드롭된 열매가 차지, PC 꽉 참) → 가방·PC 증가 없음, 훔친 열매는 야생 파티 0번 칸 데이터에 들어감(배틀 뒤 쓰이지 않고 다음 야생 조우에서 지워짐 = 플레이어는 얻지 못함, 안내 없음).
