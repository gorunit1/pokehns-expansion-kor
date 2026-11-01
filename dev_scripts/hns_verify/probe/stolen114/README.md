# 재확인 114 최종 지급 실패 분기 — 강제 재현(임시, 게임·test/에 남기지 않음)

> **지급 순서를 일부러 바꾼 시험 결과다. 현재 정상 코드(`265139dc`)에서 일어난 실패가 아니다**(친구 2026-11-01, `followup-2026-10-31b.md` 9-2).

`TryGiveNextStolenWildItem`(`src/battle_util.c`)의 마지막 분기(가방·PC 모두 실패)는 정상 흐름에서 닿지 않는다(이유: `followup-2026-10-31b.md` 8절).
도구가 실제로 어디로 가는지 보려고, "훔친 도구를 드롭 도구보다 먼저 지급"하는 처리를 잠시 빼서 일부러 닿게 한다.

1. `src/battle_script_commands.c` `BS_TryGiveDroppedItems`의 `if (TryGiveNextStolenWildItem(TRUE)) return;` 두 줄을 잠시 지운다.
2. `cp dev_scripts/hns_verify/probe/stolen114/zz_tmpq114.c test/ && GITHUB_ACTION=1 make check BUILD=hns -j8 TESTS="TMPQ"`
   — `TMPQ after:` 줄에 가방·PC의 자뭉열매 수, 야생 파티 0번 칸 도구가 찍힌다.
3. 되돌리기: 1을 원래대로(`git checkout -- src/battle_script_commands.c`는 그 파일에 다른 변경이 없을 때만), `rm test/zz_tmpq114.c build/hns-test/test/zz_tmpq114.*`.

2026-10-31(2) 결과: 마지막 분기 실행(승리 끝, 가방 999는 드롭된 열매가 차지, PC 꽉 참) → 가방·PC 증가 없음, 훔친 열매는 야생 파티 0번 칸 데이터에 들어감(배틀 뒤 쓰이지 않고 다음 야생 조우에서 지워짐 = 플레이어는 얻지 못함, 안내 없음).

## 2026-11-01 보강 테스트의 민감도 확인(임시 변경, 모두 되돌림)

`test/hns_stolen_item_supply.c`(`HnS stolen item supply:`)가 실제로 잡는지 본 방법. 확인 뒤 되돌리고 `git diff --stat src include`가 비어 있는지 본다.

1. 재생성 누수 흉내: `src/battle_script_commands.c` `StealTargetItem`의 `stolen->partyIndex = …;` 다음 줄에
   `GetBattlerPartyState(itemBattler)->usedHeldItem = gLastUsedItem; gBattleStruct->battlerState[itemBattler].canPickupItem = TRUE;`
   → `GITHUB_ACTION=1 make check BUILD=hns -j8 TESTS="HnS stolen item"`: 새 배틀 테스트 3개 + 기존 "두 번 훔치기" FAIL.
2. 목록 크기: `include/battle.h` `HNS_STOLEN_WILD_ITEMS_MAX`를 7로 → 공급 테스트의 8개 경우와 크기 확인 (data) FAIL, 7개 경우 PASS.
3. THEN 실행: 각 THEN 첫 줄에 `EXPECT(FALSE);` → 배틀 테스트 모두 FAIL(일반 TEST 5개는 THEN 없음).

테스트를 쓸 때 알게 된 테스트 러너 성질:
- 야생 배틀 테스트에서는 `MOVE(opponent, …)`가 적용되지 않고 야생이 가진 기술 중에서 고른다. 야생 기술을 하나만 준다.
- 한 테스트 16턴까지.
- 도구를 지정하지 않은 야생은 배틀 시작 때 종 도구를 무작위로 받는다.
