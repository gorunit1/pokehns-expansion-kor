# 재확인 37 팝업 입장 대기 — 화면 모드 프레임 측정(임시 계측)

`IsNewestPopUpSlidingIn`(`src/battle_interface.c`)·`Cmd_pause`(`src/battle_script_commands.c`) 수정이
"배틀 딜레이 생략" ON에서 팝업이 들어오는 동안만 기다리는지 프레임으로 확인할 때 쓴다. **저장소에 계측을 남기지 않는다.**

1. 계측·장면 넣기(작업 트리가 깨끗한 상태에서):
   `git apply dev_scripts/hns_verify/probe/popup37/instrument.patch && cp dev_scripts/hns_verify/probe/popup37/zz_tmp37.c test/`
2. 테스트 ELF 만들기(헤드리스 실행은 통과 확인용): `GITHUB_ACTION=1 make check BUILD=hns -j8 TESTS="TMP37"`
3. 화면 모드(헤드리스 패치 없는 `pokehns-test.elf`)로 실행:
   `tools/mgba-rom-test-hydra/mgba-rom-test-hydra tools/mgba/mgba-rom-test arm-none-eabi-objcopy pokehns-test.elf | grep TMP37`
   - `TMP37 pause value=… waited=…` = 그 `pause`가 기다린 프레임, `TMP37 hpbar … sliding=… framesSincePopUp=…` = HP 바 시작 때 팝업이 아직 들어오는 중인지와 마지막 특성 팝업 생성 뒤 프레임(도구 팝업은 생성 프레임을 기록하지 않아 그 값은 무의미).
   - 화면 모드에서는 배틀이 팝업 직후 끝나는 장면이 `Task_FreeAbilityPopUpGfx: task not freed`로 FAIL로 찍힌다 — 수정 전에도 같음(테스트 러너 화면 모드의 현상).
4. 수정 전과 비교하려면 `Cmd_pause` 조건을 `if (gTestRunnerHeadless || !gSaveBlock3Ptr->challengeSettings.fastBattle)`로 잠시 되돌리고 2~3 반복.
5. 되돌리기: `git apply -R dev_scripts/hns_verify/probe/popup37/instrument.patch && rm test/zz_tmp37.c build/hns-test/test/zz_tmp37.*`

2026-10-31 결과는 `docs/friend-handoff/results/1.17.0-port/followup-2026-10-31.md` 37절.
