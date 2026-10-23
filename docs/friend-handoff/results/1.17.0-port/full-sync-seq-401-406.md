# full-sync 묶음 34 — seq 401·403·404·406 (+ #10320 앞당김)

- 요청: 사용자 "혼자 할 수 있는 거라면 다 끝내고 보낼 보고 메시지는 마지막에 한 번에"(2026-10-23 밤, 사용자 취침 중 자율 진행). 사전 분석에서 재확인 목록을 대조했다 — 403은 재확인 8s("seq 680 #9970 이로치 HnS 줄 보존"), 406은 2026-09-16 사용자 결정(메가 울음소리 되돌림)에 걸리고, 둘 다 정해진 방침대로 처리했다.
- 402는 앞당김(`fdb0be06`), 405·408은 이 순서표에 없음(1.16.2 쪽 표).
- upstream: 읽기 전용 사본 `/home/user/pokeemerald-expansion-upstream`에서 `git format-patch`로 읽었다.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 401 | #10179 | `c9cb789a` | 쓰이지 않는 함수 삭제(114파일, −2,331줄) | 32,968,164 |
| 403 | #9970 | `e01cf28b` | givemon/createmon을 `PokemonTemplate`·`CreateMonFromTemplate`로 + HnS 적응 + HnS 테스트 | 32,969,700 |
| 410 앞당김 | #10320 | `5fba1e7d` | givemon 지정 기술 PP 복구(#9970 회귀 수정) | 32,969,700 |
| 404 | #10294 | (변경 없음) | 대상 줄이 HnS에 아직 없음 — 아래 | — |
| 406 | #9987 | (적용 안 함) | 메가 울음소리 WAV — 사용자 결정 | — |

이식 전(`6029378d`) 32,967,428 B → **32,969,700 B(+2,272)**, EWRAM 250,408 → **250,404**(−4, 아래), IWRAM 25,516 같음, SHA1 **`25ede450cfd7bb19c43a8331f3a15d12da3b707c`**.

## 401 #10179 — Remove unused functions

- 호출하는 곳이 없는 함수·선언 약 2,300줄 삭제. 23개 hunk가 HnS 문맥 차이로 안 붙어서, 함수마다 HnS(src·include·data·asm) 호출 0을 확인한 뒤 손이나 퍼지로 지웠다. 지운 함수 목록은 upstream과 같다(149개, `CheckMemBlockInternal` 하나만 HnS에서 `static`이 아니었음).
- ROM +736 B는 다른 호출처를 잃은 함수가 인라인되어서다(`IncreaseStat`/`DecreaseStat` → `TryStatChange`, `ListMenuInitInternal`, 트레이너 팬 함수).
- EWRAM −3 B(정렬 −4): 쓰기만 하던 `event_object_movement.c`의 `sCurrentReflectionType`·`sCurrentSpecialObjectPaletteTag` 삭제 — 세이브 무관.
- 동작 변화 0.

## 403 #9970 — More mon generation cleanup

- upstream: givemon/createmon이 `struct PokemonTemplate`(사용자 입력 그대로)를 만들고 `CreateMonFromTemplate`이 종·레벨·성격·도구·볼·이로치·특성·개체값·노력치·기술·다이맥스·테라·알을 검증해 만든다. `ShinyMode`가 `SHINY_MODE_NEVER`(0)·`SHINY_MODE_ALWAYS`(1)·`SHINY_MODE_RANDOM` 순서로(FALSE/TRUE와 같은 값), `GeneratedMonOrigin`이 `constants/pokemon.h`로 옮겨지며 `UNDEFINED_MON_ORIGIN`(0)이 앞에 붙는다(`ow_abilities.c` 표는 지정 초기화라 영향 없음). 랜덤 볼 범위가 `BALL_STRANGE`부터 → `BALL_POKE`부터. `ComputePlayerShinyOdds`의 강제 플래그 `!= 0` 검사 삭제(`FlagGet(0)`은 FALSE라 같음 — HnS 이로치 줄(재확인 8s)은 그대로).
- **HnS 적응(이식 전 동작 유지):**
  1. **특성:** 신작 스타팅 스크립트(`NewBarkTown_Lab_hns`)는 `abilityNum`에 `NUM_ABILITY_PERSONALITY`를 직접 넘긴다. upstream 그대로면 "지정한 특성"으로 처리돼 assert 뒤 늘 0번 특성 → HnS `ScrCmd_createmon`은 이 값이면 지정하지 않음(성격값 특성).
  2. **챌린지 "파티 개체값 최대":** 이식 전처럼 템플릿 개체값을 덮는다(`ScriptGiveMonParameterized`에서 `SetBoxMonIVs` + 능력치 재계산).
  3. **기본 기술:** 기술 4개가 모두 기본이면 이식 전처럼 `GiveMonInitialMoveset`(랜더마이저 기술 무작위화, 스타팅 첫 공격기 보장 포함). 지정 기술이 섞이면 upstream `ResolveMoves`.
  4. **싱크로·메로메로바디:** upstream `ResolvePersonality`가 종 대신 템플릿 값(성별·성격 상수)을 넘긴다(upstream HEAD도 같음) → HnS는 종을 넘김. HnS 챌린지 싱크로 모드·고정 성별 검사가 종을 보기 때문.
  5. **고정 조우 랜더마이저**(`FLAG_SYS_POKEMON_GET` 뒤 `RANDOMIZE_FIXED_MON`) 유지 — 템플릿 종을 바꾼다.
  6. **`src/debug.c`는 옮기지 않음:** HnS 디버그 포켓몬 주기는 성별 선택(`genderPersonality`)이 있는 자체 흐름이라 기존 `DebugMonData` 경로 유지(디버그 전용, upstream 쪽은 리팩터).
- **HnS 회귀 테스트** `test/hns_givemon_template.c`(2개): `NUM_ABILITY_PERSONALITY` 특성, 개체값 최대 챌린지. 보정을 빼면 각각 INVALID(assert)·FAIL 확인.
- **남는 차이(실제 HnS 진행 영향 없음):** 랜더마이저 고정 조우에서 최소 3V 판정이 원래 종 대신 바뀐 종 기준(더 맞는 쪽), 스크립트 노력치 252 초과는 0으로(HnS 스크립트는 0만 씀), 잘못된 특성 번호는 0번으로(HnS 스크립트 없음).
- HnS 선물 포켓몬 호출: 스타팅(이로치 NEVER/ALWAYS·볼·성격·특성 직접 지정, 기술 기본 4개), 그 밖 약 40곳(종·레벨·도구만) → 위 적응으로 이식 전과 같은 경로.

## 410 앞당김 #10320 — givemon PP

- #9970은 지정 기술을 `SetMonData(MOVE)`로 넣어 PP가 0이 된다(이식 전 HnS는 `SetMonMoveSlot`). upstream #10320(seq 410)이 고쳤다 → 바로 다음 커밋으로 앞당김(선례). upstream PP 검사를 givemon 테스트 4개에 추가(PASS). HnS 스크립트는 지정 기술을 쓰지 않아(기본 4개는 위 3번 경로) 실제 진행 영향은 없었지만 디버그·뒤 스크립트를 위해.

## 404 #10294 — 변경 없음

- upstream은 `BattleScript_ItemHealHP_RemoveItemRet_AnimContinue`의 `playanimation BS_ATTACKER, B_ANIM_SIMPLE_HEAL`을 `BS_SCRIPTING`으로 고친다. 그 줄은 #9777(Champions 배틀 문장, seq 475)이 넣는 줄이라 HnS에는 아직 없다(HnS는 이전 회복 문장 형태). **seq 475 때 `BS_SCRIPTING`으로 넣는다(재확인 8zb).**

## 406 #9987 — 적용 안 함

- Legends Z-A 메가 울음소리 WAV 26개 고음질 교체. 2026-09-16 사용자 결정(64MiB 철회, `STATUS.md`)으로 HnS는 `P_MODIFIED_MEGA_CRIES FALSE`(메가는 기본 울음소리 고음)이고 1.17.0의 이 WAV 26개를 되돌렸다. 넣어도 ROM에 안 들어가지만 결정대로 넣지 않는다.

## 검증

- **빌드:** 커밋마다 `make hns -j8` 종료 0, 새 경고 0(`warncheck.sh`).
- **세이브 정적 비교:**
  - 401(이식 전 사실 = 세션 임시 `b35/save-pre`, 코드 `6029378d`를 클론에서 빌드 — ROM 32,967,428 B로 같음): **PASS(FAIL 0, WARN 1)** — WARN은 위 EWRAM 변수 2개(−3 B, 세이브 무관).
  - 403·410(이식 전 사실 = 세션 임시 `b36/save-pre`, 코드 `c9cb789a` — ROM 32,968,164 B로 같음): **PASS(FAIL 0, WARN 0)**.
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`):
  - 401(`c9cb789a`): 목록이 `test-baseline-seq400.txt`와 **바이트 같음**.
  - 403·410(`5fba1e7d`): 러너 PASSED 2,760 / TOTAL 5,737(KNOWN_FAILING 25) — seq400 기준 사라진 줄 2 = upstream이 지운 `resolves random ball`, 이름이 바뀐 `resolves teachable moves`(→ `ResolveMoves resolves teachable moves` PASS), 새 PASS 2(HnS givemon 테스트). 새 기준 [`test-baseline-seq403.txt`](test-baseline-seq403.txt)(**다음 비교 기준**).
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-b35/kor-b36 4`, 코드 `c9cb789a`·`5fba1e7d`): 둘 다 **558/652, `판정: PASS (기대와 같음)`**.
- **실기: 하지 않음(친구).**

## 실기(mGBA) 후보 — 친구

1. 새 게임 → 신작 스타팅 받기: 특성(성격값에 따라)·기술 4개·PP·볼이 전과 같은지.
2. 선물 포켓몬 몇 마리(예: 이브이·라프라스·화석 부활): 기술·PP·도구.
3. (챌린지를 쓰면) 개체값 최대 챌린지에서 선물 포켓몬 개체값 31.

## 재확인·뒤 PR 메모

- 새 재확인 87(403 HnS 적응 6개), 8zb(404 → seq 475 때 `BS_SCRIPTING`). 재확인 8s의 seq 680(#9970) 줄 확인 끝.
- 순서표: 401·403 적용, 404 변경 없음, 406 적용 안 함, 410 앞당김(다시 넣지 않음).
- 다음은 seq 407 #10293.
