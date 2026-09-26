> 이 문서의 커밋 해시는 작업용 worktree 기준이다. 메인 브랜치 해시는 [결과 보고서](../pokeemerald-expansion-1.17.0-update-report.md)의 "적용 단위 기록" 표를 본다.

# 묶음 A 배틀 이식 결과 (worktree `<worktree>`, 브랜치 `port/a-battle`, 시작점 `7c4dd0c096`)

기준 빌드(시작점, 결과 보고서 1절): ROM 33,326,484 B (99.32%), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%).
모든 빌드는 `make hns -j6 > build/port.log 2>&1`로 실행했다(다른 worktree 동시 빌드 때문에 -j8 대신 -j6).

## 적용 단위: #10406 Intrepid and Dauntless futureproofing

- 판정: 적용
- upstream 근거: PR #10406, 커밋 `8e3d66d940`
- 수정 파일: `src/battle_util.c` (`AbilityBattleEffects` 불요의검·불굴의방패 2곳)
- 보존한 HNS·한글화 차이: 해당 없음(조건식 2개만 변경). config `B_INTREPID_SWORD`/`B_DAUNTLESS_SHIELD`는 `GEN_LATEST`(=GEN_CHAMPIONS) 그대로.
- 충돌 여부와 상세: 없음. upstream hunk와 동일.
- 검증:
  - 코드 대조: HnS가 `== GEN_9`이라 GEN_CHAMPIONS에서 배틀당 1회 제한이 꺼져 있었음을 확인.
  - 대상 빌드: 전체 빌드에 포함
  - `make hns -j6`: 성공(exit 0). ROM 33,326,484 B (99.32%), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%) — 기준과 동일
  - 자동/실기 테스트: 미실시
- 커밋: `51cc80bf74`
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10318 Remove redundant Magnet Rise / Laser Focus flags

- 판정: 적용
- upstream 근거: PR #10318, 커밋 `574ea8dd6f`
- 수정 파일: `data/battle_scripts_1.s`(레이저포커스·전자부유 스크립트, 중력 루프), `include/constants/battle.h`(플래그 2개 삭제, `magnetRiseTimer`에 `V_BATON_PASSABLE`), `src/battle_ai_main.c`(4곳), `src/battle_debug.c`(디버그 목록 2줄), `src/battle_end_turn.c`, `src/battle_move_resolution.c`(떨어뜨리기), `src/battle_script_commands.c`(`Cmd_trysetvolatile` switch 제거, `BS_GravityOnAirborneMons`), `src/battle_util.c`(접지 판정, 급소 2곳)
- 보존한 HNS·한글화 차이: 문자열·`printstring` 순서 변경 없음(`STRINGID_LASERFOCUS`, `STRINGID_PKMNLEVITATEDONELECTROMAGNETISM` 그대로). HnS의 `gBattlerTarget`/`ctx->` 구조와 `BattleScriptExecute`(end turn) 유지.
- 충돌 여부와 상세: 없음. upstream hunk 20곳이 HnS에 1:1 대응(diffstat도 16+/31- 동일). `test/`에는 해당 심볼 참조가 없음.
- 검증:
  - 코드 대조: HnS는 바톤터치 때 `magnetRise` 플래그만 넘기고 타이머는 넘기지 않아(`battle_main.c` 복사 목록) 받은 쪽이 영구 부유하던 문제. 이제 타이머 자체가 `UNPACK_VOLATILE_BATON_PASSABLES`로 복사된다. 남은 `magnetRise`/`laserFocus` 참조 0건 확인.
  - 대상 빌드: 헤더 변경으로 사실상 전체 재빌드
  - `make hns -j6`: 성공(exit 0). ROM 33,326,148 B (99.32%, −336 B), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시(바톤터치 전자부유 해제 실기 확인 필요)
- 커밋: `be5123a300`
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10093 Fixes Random Move from disobedience

- 판정: 적용
- upstream 근거: PR #10093, 커밋 `63e5951d94`
- 수정 파일: `src/battle_move_resolution.c` (`CancelerObedience`의 `DISOBEYS_RANDOM_MOVE`)
- 보존한 HNS·한글화 차이: `BattleScript_IgnoresAndUsesRandomMove`(`STRINGID_PKMNIGNOREDORDERS`) 출력 그대로. HnS의 `ctx->battlerAtk`, `CANCELER_RESULT_BREAK`(= upstream RUN_SCRIPT_AND_INCREMENT와 같은 의미) 유지.
- 충돌 여부와 상세: 없음. 1줄 추가.
- 검증:
  - 코드 대조: `BattleScriptCall`이 현재 `gBattlescriptCurrInstr`(원래 기술 스크립트의 `attackcanceler`)를 push하므로 복귀 후 원래 기술 스크립트로 실행되던 버그. 이제 무작위 기술 스크립트의 `attackcanceler`로 복귀해 다음 canceler 단계부터 진행.
  - 대상 빌드: `battle_move_resolution.o`
  - `make hns -j6`: 성공(exit 0). ROM 33,326,276 B (99.32%, +128 B), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 커밋: `455c2c0b7a`
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10213 Fixes Smack Down not clearing correct values

- 판정: 적용(HnS 구조에 맞춤)
- upstream 근거: PR #10213, 커밋 `14ebdb3591`
- 수정 파일: `src/battle_move_resolution.c` (`MoveEndMoveBlock`의 `EFFECT_SMACK_DOWN`)
- 보존한 HNS·한글화 차이: `BattleScript_MoveEffectSmackDown` 스크립트·문자열 그대로. HnS의 `gBattlerTarget`·`IsBattlerTurnDamaged(gBattlerTarget, EXCLUDING_SUBSTITUTES)` 유지.
- 충돌 여부와 상세: 없음. 맞춤 내용:
  - HnS에는 `STATE_SKY_DROP_ATTACKER/_TARGET`(#9249)이 없다. 프리폴 대상은 `STATE_SKY_DROP`, 프리폴 사용자는 `STATE_ON_AIR` + `skyDropTargets[b] != SKY_DROP_NO_TARGET`(HnS `BS_SkyDropYawn`·`CancelMultiTurnMoves`와 같은 판별)로 제외했다.
  - `DoesSubstituteBlockMove` 조건은 upstream처럼 삭제(`EXCLUDING_SUBSTITUTES`의 `damagedByAttack`가 이미 대신 막음).
  - `src/pokemon.c` 공백 hunk 제외.
- 검증:
  - 코드 대조: 공중날기·뛰어오르기 중 맞으면 `semiInvulnerable`와 `multipleTurns`를 함께 해제, 비공중 상태(예: 부유 특성의 구멍파기+노가드)는 반무적 상태를 건드리지 않음. 땅에 붙은 뛰어오르기 사용자도 이제 떨어진다(메시지는 기존 떨어뜨리기 문구 그대로, upstream 의도).
  - 대상 빌드: `battle_move_resolution.o`
  - `make hns -j6`: 성공(exit 0). ROM 33,326,356 B (99.32%, +80 B), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시(더블배틀 프리폴+떨어뜨리기 실기 확인 권장)
- 커밋: `e2c65a3b47`
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10228 Fix dynamic Fire-type moves not thawing frozen targets

- 판정: 적용(HnS 구조에 맞춤)
- upstream 근거: PR #10228, 커밋 `2bb2deb3cf`
- 수정 파일: `include/battle_script_commands.h`, `src/battle_script_commands.c`(`CanFireMoveThawTarget(move, moveType)`), `src/battle_move_resolution.c`(`MoveEndDefrost`), `src/battle_ai_main.c`(`AI_CheckBadMove` 해동 검사)
- 보존한 HNS·한글화 차이: 해동 스크립트·문자열(`BattleScript_BattlerDefrosted` 등) 그대로.
- 충돌 여부와 상세: 없음. 맞춤 내용:
  - `MoveEndDefrost`는 HnS에 `cv`가 없어 `GetBattleMoveType(gCurrentMove)` 사용.
  - AI 루프: upstream 그대로(`CheckDynamicMoveType(GetBattlerMon(battlerAtk), ...)`를 루프 안에서 호출) 넣으면 `AI_CheckBadMove` 레지스터 할당이 바뀌어 +1,190 B가 됐다. `GetBattlerMon(battlerAtk)`를 루프 밖 지역 변수로 뺀 같은 논리로 바꿔 +264 B로 줄였다.
- 검증:
  - 코드 대조: 햇빛 속 웨더볼·테크노버스터(불꽃 카세트) 등 동적 불꽃 타입 기술도 `gBattleStruct->dynamicMoveType`을 통해 해동 판정.
  - 대상 빌드: 헤더 변경으로 관련 오브젝트 재빌드. 변형별 `AI_CheckBadMove` 크기를 별도 컴파일로 비교함.
  - `make hns -j6`: 성공(exit 0). ROM 33,326,628 B (99.32%, +272 B), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 커밋: `0ed3b6440b`
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10207 Fix Bad Dreams leaving ABILITY_POPUP stuck open

- 판정: 적용
- upstream 근거: PR #10207, 커밋 `92a4c68d71`
- 수정 파일: `data/battle_scripts_1.s` (`BattleScript_BadDreamsIncrement`에 `setbyte sFIXED_ABILITY_POPUP, FALSE` 1줄)
- 보존한 HNS·한글화 차이: HnS 구조(`end2` 종료, `BattleScript_BadDreams_ShowPopUp` 분기)와 `STRINGID_BADDREAMSDMG` 출력 순서 그대로. 팝업 표시 순서 변화 없음.
- 충돌 여부와 상세: 없음.
- 검증:
  - 코드 대조: 나이트메어 뒤 `sFIXED_ABILITY_POPUP`이 TRUE로 남아 다음 특성 팝업이 닫히지 않던 문제. 다른 스크립트(`BattleScript_AbilityPopUpOverwriteThenNormal`)와 같은 해제 방식.
  - 대상 빌드: `battle_scripts_1.o`
  - `make hns -j6`: 성공(exit 0). ROM 33,326,644 B (99.32%, +16 B), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 커밋: `07f6879d21`
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10132 잔여분 Fix Strong Winds interactions with Weather Ball

- 판정: 부분 적용(잔여분 적용 — 회복기 hunk는 이미 적용)
- upstream 근거: PR #10132, 커밋 `08e503fb66`
- 수정 파일: `src/battle_util.c` (`CalcMoveBasePower`의 `EFFECT_WEATHER_BALL` 마스크)
- 보존한 HNS·한글화 차이: 해당 없음. HnS의 `ctx->holdEffectAtk`/`ctx->abilityAtk` 이름 유지.
- 충돌 여부와 상세: 없음. `Cmd_recoverbasedonsunlight`의 `healingWeather` 두 곳은 HnS에 이미 있음(9533, 9550, 9585행)을 확인하고 건드리지 않음.
- 검증:
  - 코드 대조: 난기류(`B_WEATHER_STRONG_WINDS`)만 있을 때 웨더볼 위력 2배가 되지 않음.
  - 대상 빌드: `battle_util.o`
  - `make hns -j6`: 성공(exit 0). ROM 33,326,644 B (99.32%, ±0), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 커밋: `0108ca38ba`
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10514 Fix Rage Fist Hit counter overflow

- 판정: 적용
- upstream 근거: PR #10514, 커밋 `12de80e9c0`
- 수정 파일: `include/battle.h` (`struct PartyState`: `timesGotHit:5`→`:8`, `padding:8`→`:5`)
- 보존한 HNS·한글화 차이: 해당 없음. 구조체 크기 동일(비트 합계 32 유지).
- 충돌 여부와 상세: 없음. upstream hunk와 동일.
- 검증:
  - 코드 대조: `CalcMoveBasePower`의 350 상한은 그대로이므로 32회 이상 피격 후 위력이 50으로 되돌아가던 문제만 사라짐.
  - 대상 빌드: 헤더 변경으로 배틀 관련 오브젝트 재빌드
  - `make hns -j6`: 성공(exit 0). ROM 33,326,644 B (99.32%, ±0), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 커밋: `0ae8ecbe78`
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10476 Fix Court Change hazard count swapping

- 판정: 적용
- upstream 근거: PR #10476, 커밋 `0d045a1d67`
- 수정 파일: `src/battle_script_commands.c` (`BS_CourtChangeSwapSideStatuses`에 `numHazards` SWAP 1줄)
- 보존한 HNS·한글화 차이: 해당 없음. 코트체인지 메시지·장판 해제 문구(HnS 장판별 `*DISAPPEAREDFROMTEAM`) 경로 변경 없음.
- 충돌 여부와 상세: 없음. upstream hunk와 동일.
- 검증:
  - 코드 대조: `hazardsQueue`는 교환하면서 `numHazards`는 교환하지 않아, 교환 뒤 장판 추가·제거(`battle_util.c` 10464/10516행)가 틀린 개수를 쓰던 문제.
  - 대상 빌드: `battle_script_commands.o`
  - `make hns -j6`: 성공(exit 0). ROM 33,326,660 B (99.32%, +16 B), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 커밋: `5030d382da`
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10543 Fix Regenerator/Natural Cure applying to the wrong party slot

- 판정: 적용
- upstream 근거: PR #10543, 커밋 `a7f7e5b83a`
- 수정 파일: `src/battle_script_commands.c` (`Cmd_switchoutabilities` 2곳)
- 보존한 HNS·한글화 차이: 해당 없음(메시지 없음).
- 충돌 여부와 상세: 없음. upstream hunk와 동일.
- 검증:
  - 코드 대조: `gBattleStruct->battlerPartyIndexes`는 교체 처리 중 새로 나올 슬롯으로 먼저 갱신될 수 있어(5580·5758행) 재생력·자연회복이 다른 파티 슬롯의 HP/상태를 바꿀 수 있었다. 같은 함수의 `TryDeactivateSleepClause`는 이미 `gBattlerPartyIndexes`를 씀.
  - 대상 빌드: `battle_script_commands.o`
  - `make hns -j6`: 성공(exit 0). ROM 33,326,660 B (99.32%, ±0), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 커밋: `c2eec58645`
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10554 Fix Hunger Switch Persisting on Ability pop up

- 판정: 적용
- upstream 근거: PR #10554, 커밋 `f0a63ea025`
- 수정 파일: `data/battle_scripts_1.s` (`BattleScript_BattlerFormChangeNoPopup`에 `sethword sABILITY_OVERWRITE, 0` 1줄)
- 보존한 HNS·한글화 차이: 문자열·팝업 순서 변경 없음. 폼체인지 스크립트 구조는 upstream과 동일.
- 충돌 여부와 상세: 없음.
- 검증:
  - 코드 대조: `HandleEndTurnFormChange`가 `abilityPopupOverwrite = ability`(배고픈스위치)로 두고 지우지 않아, 이후 같은 배틀의 다른 특성 팝업에 "배고픈스위치"가 계속 표시되던 문제. `BattleScript_BattlerFormChangeFromAfterAnimation`으로 직접 들어가는 호출(5588·5649행)은 upstream과 같이 영향 없음.
  - 대상 빌드: `battle_scripts_1.o`
  - `make hns -j6`: 성공(exit 0). ROM 33,326,676 B (99.32%, +16 B), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 커밋: `91be750f79`
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10622 Fix Flying Press using its secondary type for damage modifiers

- 판정: 적용
- upstream 근거: PR #10622, 커밋 `fc938da350`
- 수정 파일: `src/battle_util.c` (`CalcTypeEffectivenessMultiplier`)
- 보존한 HNS·한글화 차이: 해당 없음. HnS는 `struct BattleContext *ctx`(upstream은 `DamageContext`) — 필드명이 같아 논리 그대로.
- 충돌 여부와 상세: 없음.
- 검증:
  - 코드 대조: 두 번째 타입(비행) 상성 계산 후 `ctx->moveType`이 비행으로 남아, 이후 자속 보정·타입 강화 도구 등이 격투가 아닌 비행 기준으로 계산되던 문제.
  - 대상 빌드: `battle_util.o`
  - `make hns -j6`: 성공(exit 0). ROM 33,326,692 B (99.32%, +16 B), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 커밋: `cbeac58035`
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10475 Fix Sheer Cold move type check and rename flag

- 판정: 적용
- upstream 근거: PR #10475, 커밋 `8aae9a242f`
- 수정 파일: `include/move.h`(필드·접근자 이름), `src/data/moves_info.h`(절대영도), `src/battle_ai_util.c`(`ShouldTryOHKO`), `src/battle_util.c`(`GetTotalAccuracy` ×1.1→×0.9, `DoesOHKOMoveMissTarget`)
- 보존한 HNS·한글화 차이: 해당 없음. config `B_SHEER_COLD_ACC`(GEN_LATEST) 그대로. `move.h` 주석은 upstream 그대로 둠(upstream도 옛 주석 유지).
- 충돌 여부와 상세: 없음. HnS도 `GetTotalAccuracy(cv, ...)` 구조라 hunk가 1:1 대응. `test/battle/move_effect/ohko.c` 제외.
- 검증:
  - 코드 대조: 얼음 타입이 아닌 사용자의 절대영도 명중을 1.1배로 올리던 버그 수정. 옛 이름 참조 0건 확인.
  - 대상 빌드: `move.h` 변경으로 광범위 재빌드
  - `make hns -j6`: 성공(exit 0). ROM 33,326,692 B (99.32%, ±0), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 커밋: `06a9eeafdf`
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10675 Fix Solar Beam, Solar Blade and Electro Shot skipping their charging turn

- 판정: 적용(HnS 구조에 맞춤)
- upstream 근거: PR #10675, 커밋 `29fd94b9b8`
- 수정 파일: `src/battle_move_resolution.c` (`CanTwoTurnMoveFireThisTurn`)
- 보존한 HNS·한글화 차이: HnS의 메가솔 특성 팝업 분기(`showAbilityPopUp`, #10416 이식분)를 그대로 둠. 충전 메시지·팝업 순서 변화 없음.
- 충돌 여부와 상세: 충돌 없음. 맞춤 내용:
  - upstream은 `(attackerWeather & flag) || IsBattlerWeatherAffected(holdEffect, weather, flag)` 한 줄이지만, HnS는 "실제 날씨 → 팝업 없이 발사"와 "메가솔 → 팝업 후 발사" 두 갈래다. HnS의 `IsBattlerWeatherAffected(battler, flags)`는 시그니처가 다르고 메가솔까지 포함해 팝업 분기를 깨므로 쓰지 않았다.
  - 버그가 있던 첫 갈래 `weather & moveWeather`만 `GetAttackerWeather(holdEffectAtk, ABILITY_NONE, weather) & moveWeather`로 바꿨다. 만능우산이 쾌청·비를 지우는 규칙을 기존 함수로 재사용한다.
  - `test/battle/move_effect/two_turns_attack.c` 제외.
- 검증:
  - 코드 대조: 만능우산 소지 시 쾌청 속 솔라빔·솔라블레이드, 비 속 일렉트릭빔이 충전 턴을 거침. 메가솔은 우산보다 우선한다는 기존 가정(`GetAttackerWeather` 주석) 유지. 메가솔 보유자는 메가스톤을 들고 있어 우산+메가솔 조합은 실전에서 나오지 않음.
  - 대상 빌드: `battle_move_resolution.o`
  - `make hns -j6`: 성공(exit 0). ROM 33,326,708 B (99.32%, +16 B), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 커밋: `6a16bb059f`
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 적용 단위: #10344 Fix Future Sight triggering reactive items and abilities

- 판정: 적용(레드카드·탈출버튼 hunk는 HnS에서 불필요해 제외)
- upstream 근거: PR #10344, 커밋 `59ae72a107`
- 수정 파일: `src/battle_hold_effects.c`(`TryJabocaBerry`·`TryRowapBerry`), `src/battle_util.c`(저주받은바디)
- 보존한 HNS·한글화 차이: 열매·특성 팝업 스크립트와 문자열 그대로. HnS 미래예지 스크립트(`BattleScript_MonTookFutureAttack`) 구조 변경 없음.
- 충돌 여부와 상세: 충돌 없음. 맞춤 내용:
  - 저주받은바디: HnS `MoveEndAbilities`는 `move=0`을 넘기지만 `AbilityBattleEffects`가 `MOVE_NONE`이면 `gCurrentMove`로 채우므로 upstream처럼 `GetMoveEffect(move)` 사용.
  - 레드카드·탈출버튼(`TryRedCard`/`TryEjectButton`) hunk 제외: upstream은 당시 미래예지 뒤 `moveendall`을 돌려 `MOVEEND_CARD_BUTTON`까지 실행했지만, HnS의 `BattleScript_FutureAttackEnd`는 `SET_VALUES, RAGE, ABILITIES, ITEM_EFFECTS_TARGET, SYMBIOSIS~UPDATE_LAST_MOVES, COLOR_CHANGE`만 실행한다. HnS에서는 미래예지가 레드카드·탈출버튼을 이미 발동시키지 않으므로 도달하지 않는 조건을 넣지 않았다.
  - `test/battle/move_effect/future_sight.c` 제외.
- 검증:
  - 코드 대조: 잭열매·로플열매(`MOVEEND_ITEM_EFFECTS_TARGET` → `ItemBattleEffects` → 1073/1076행)와 저주받은바디(`MOVEEND_ABILITIES`)는 HnS 미래예지 경로에서 실제로 실행됨을 확인.
  - 대상 빌드: `battle_hold_effects.o`, `battle_util.o`
  - `make hns -j6`: 성공(exit 0). ROM 33,326,868 B (99.32%, +160 B), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시
- 커밋: `26afbae484`
- 남은 문제 및 사용자 결정 필요 사항: 레드카드·탈출버튼 hunk를 방어 목적으로라도 넣을지(현재 동작 차이 없음, ROM 수십 B).

## 적용 단위: #10180 Add Effect activation after Mega Evolution

- 판정: 적용(HnS 구조에 맞춤, Sky Drop hunk 제외)
- upstream 근거: PR #10180, 커밋 `90e25f1cf1`
- 수정 파일: `asm/macros/battle_script.inc`(`effectsafterformchange` 매크로), `data/battle_scripts_1.s`(`BattleScript_TeraFormChange`·`BattleScript_MegaEvolution`(=`BattleScript_WishMegaEvolution` 공용 꼬리)·`BattleScript_UltraBurst`의 `switchinabilities` 뒤 1줄씩), `src/battle_script_commands.c`(`BS_EffectsAfterFormChange`)
- 보존한 HNS·한글화 차이: 메가진화·울트라버스트·테라스탈 문자열(`STRINGID_MEGAEVOEVOLVED` 등)과 출력 순서 그대로. 명령은 기존 출력 뒤에만 추가되며, 발동 시 기존 하양허브·편승·흉내허브·탈출팩 스크립트를 그대로 사용.
- 충돌 여부와 상세: 충돌 없음. 맞춤 내용:
  - HnS에는 `struct BattleCalcValues`와 `gBattlersByRawSpeed`가 없다. 특성·도구는 `GetBattlerAbility`/`GetBattlerHoldEffect`로 직접 구하고, 순회는 HnS의 같은 효과 moveend(`MoveEndWhiteHerb`/`MoveEndOpportunist`/`MoveEndMirrorHerb`)와 같은 배틀러 번호 순서로 했다(여럿이 동시에 발동할 때만 순서가 upstream의 속도순과 다를 수 있음).
  - 제외 hunk(#9249 Sky Drop 리팩터 의존): `IsBattlerInvolvedInSkyDrop` 추가(`battle_util.h/.c`), `TryEjectButton`·`TryEjectPack`·`TrySwitchInEjectPack`의 Sky Drop 조건, `CanMoveSkipAccuracyCalc`의 `IsSkyDropInvolved` 대체.
  - 테스트 4개 파일 제외.
- 검증:
  - 코드 대조: 네 효과 모두 `BattleScriptCall`로 복귀형 스크립트를 부르고(하양허브 `IsWhiteHerbActivation`→`BattleScript_WhiteHerbRet`, 흉내허브, 편승 `activateOpportunist` 감소, 탈출팩 `START_OF_TURN`→`BattleScript_EjectPackActivate_Ret`), 발동 뒤 같은 명령으로 돌아와 다음 효과를 확인하는 upstream 구조가 HnS에서도 성립함을 확인. 원시회귀는 upstream도 추가하지 않아 제외.
  - 대상 빌드: `battle_scripts_1.o`, `battle_script_commands.o`. `pokehns.elf`에 `BS_EffectsAfterFormChange`(0x128 B) 링크 확인.
  - `make hns -j6`: 성공(exit 0). ROM 33,327,188 B (99.32%, +320 B), EWRAM 249,016 B (94.99%), IWRAM 25,680 B (78.37%)
  - 자동/실기 테스트: 미실시(메가진화 직후 위협·다운로드 등으로 하양허브·흉내허브·편승·탈출팩 발동 실기 확인 권장)
- 커밋: `f2d3008825`
- 남은 문제 및 사용자 결정 필요 사항: 없음

## 요약

| 순서 | PR | 판정 | 커밋 | ROM(B) |
|---|---|---|---|---|
| 1 | #10406 | 적용 | 51cc80bf74 | 33,326,484 |
| 2 | #10318 | 적용 | be5123a300 | 33,326,148 |
| 3 | #10093 | 적용 | 455c2c0b7a | 33,326,276 |
| 4 | #10213 | 적용(HnS Sky Drop 판별로 맞춤, pokemon.c 공백 hunk 제외) | e2c65a3b47 | 33,326,356 |
| 5 | #10228 | 적용(AI 루프 형태 조정) | 0ed3b6440b | 33,326,628 |
| 6 | #10207 | 적용 | 07f6879d21 | 33,326,644 |
| 7 | #10132 잔여 | 부분 적용(잔여분 적용, 회복기는 이미 적용) | 0108ca38ba | 33,326,644 |
| 8 | #10514 | 적용 | 0ae8ecbe78 | 33,326,644 |
| 9 | #10476 | 적용 | 5030d382da | 33,326,660 |
| 10 | #10543 | 적용 | c2eec58645 | 33,326,660 |
| 11 | #10554 | 적용 | 91be750f79 | 33,326,676 |
| 12 | #10622 | 적용 | cbeac58035 | 33,326,692 |
| 13 | #10475 | 적용 | 06a9eeafdf | 33,326,692 |
| 14 | #10675 | 적용(메가솔 팝업 분기 보존) | 6a16bb059f | 33,326,708 |
| 15 | #10344 | 적용(레드카드·탈출버튼 hunk 불필요로 제외) | 26afbae484 | 33,326,868 |
| 16 | #10180 | 적용(Sky Drop hunk 제외) | f2d3008825 | 33,327,188 |

- 최종 빌드: ROM 33,327,188 B / 32 MB (99.32%, 기준 대비 +704 B), EWRAM 249,016 B (94.99%, 변화 없음), IWRAM 25,680 B (78.37%, 변화 없음).
- 보류(충돌) 항목: 없음. 한글 문자열·STRINGID·지정 코드·배틀 메시지 출력 순서 변경 없음. config 기본값 변경 없음. `test/**` 미반입. `STATUS.md`·`SESSION_LOG.md`·저장소 결과 보고서 미수정.
