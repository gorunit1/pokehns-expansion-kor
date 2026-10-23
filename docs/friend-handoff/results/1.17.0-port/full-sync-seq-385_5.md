# full-sync 묶음 28 — seq 385.5 (#10454)

- 요청: 사용자 "ㄱㄱ 혼자 되는지, 친구 물어봐야하는지 보자"(사전 분석) → "1번으로 가자"(2026-10-23). 1번 = 코드는 넣고 새 설정은 HnS 동작 유지 값(`GEN_8`)으로 두고, Gen 9 동작으로 바꿀지는 친구에게 질문.
- 범위: seq 385.5 #10454(Add Gen 9 Overworld Weather Behavior). 385 #10214는 묶음 26에서 앞당김. 다음은 386 #10217.
- upstream: 읽기 전용 사본 `/home/user/pokeemerald-expansion-upstream`에서 `git format-patch`로 읽었다.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 385.5 | #10454 | `6002c047` | 필드 날씨 Gen 9 설정(HnS 값 `GEN_8`), 날씨 특성 처리 통합 | 32,962,948 |

이식 전(`a3572d8d`) 32,963,060 B → **32,962,948 B(−112)**, EWRAM 250,408·IWRAM 25,516 같음, SHA1 **`472d4622106d5a4e48e49bd954661df0c8bc88e0`**.

## 내용

- 새 설정 `B_OVERWORLD_WEATHER_OVERRIDE`(upstream 기본 `GEN_LATEST`): Gen 9 이상이면 필드 날씨(또는 시작 상태 날씨)로 시작한 배틀에서 날씨를 바꿀 수 없다(`gBattleStruct->overworldWeatherPresent`). 날씨 기술은 `그러나 실패했다!`(기존 `STRINGID_BUTITFAILED`), 날씨 특성은 팝업 뒤 같은 문장. 테라폼제로는 필드(지형)만 지운다.
- `TryChangeBattleWeather`가 실패 이유(`enum WeatherFailure` — 같은 날씨·필드 날씨·원시 날씨·성공)를 돌려주고, 날씨 특성 9개(잔비·모래날림·가뭄·진홍빛고동·눈퍼뜨리기·끝의대지·시작의바다·델타스트림·모래뿜기)가 `TryChangeWeatherWithAbility` 하나로 처리된다.

## HnS 적응·판단

- **설정 값 `GEN_8`(HnS 동작 유지, 친구 질문):** HnS에는 비·뇌우·눈 맵 15곳(`map.json` 기준 — 비: `CianwoodCity`·`OlivineCity`·`OlivineCity_PortOutside`·`Route17`·`Route40`, 뇌우: `LakeOfRage`·`Route41`, 눈: `MtSilver_Snow`·`MtSilver_SummitDay`·`MtSilver_SummitNight`·`Route49`·`Route50`·`SinjohRuins`·`SinjohRuins_RegiceRoom`·`NewSinjoh`, 모두 `_hns`)과 스크립트 날씨 4곳(얼음샛길 B3F 눈, 야돈우물 B2F 비, 방울탑 옥상 가뭄, 소용돌이섬 B1F 비)이 있다. Gen 9 동작이면 이 배틀들에서 날씨 기술·특성이 실패한다. 특히 은빛산 정상(눈) **레드 OBC 후 팀**(`TRAINER_RED_POSTOBC_HNS`)은 이상해꽃 엽록소 + 쾌청 + 솔라빔을 쓰는데 쾌청이 실패하게 된다. 지금 HnS(그리고 HGSS 원작)는 쾌청이 눈을 덮는다. 진청시티 수호의 비바라기는 비 맵이라 원래도 같은 날씨로 실패. → 이번엔 `GEN_8`로 넣어 **게임 동작 변화 0**, Gen 9로 바꿀지는 친구 결정(바꾸면 `include/config/battle.h` 한 줄).
- **모래뿜기 `gBattleScripting.battler` 유지:** upstream은 이 줄을 지웠다. 그런데 호출하는 `MoveEndAbilities`가 스크립트 배틀러를 넣지 않고, 팝업과 `BattleScript_WeatherAbilityActivates`의 모래뿜기 분기(`jumpifability BS_SCRIPTING, ABILITY_SAND_SPIT`)가 그 값을 읽는다. upstream 테스트에서는 앞 단계에서 남은 값이 우연히 맞았다(아래 테스트에서 팝업 일치). 더블배틀 등에서 남은 값이 다른 배틀러면 팝업·문장이 틀릴 수 있어 HnS는 피격 확인 뒤 `gBattleScripting.battler = battler`를 넣는다(`// HnS:` 주석).
- **지형 판정:** HnS는 아직 지형 타이머 리팩터 전이라 테라폼제로 분기·새 스크립트가 `gFieldStatuses & STATUS_FIELD_TERRAIN_ANY`(`jumpifhalfword`)로 지형을 본다(upstream은 `gFieldTimers.terrain`·`jumpifterrain`).
- **`GEN_8`에서도 바뀌는 것(upstream 정리, 드묾):**
  - 원시 날씨 막힘 판정이 `GetWeather()`(구름제거·에어록이면 날씨 없음으로 봄) 대신 실제 날씨(`gBattleWeather`)를 본다. 구름제거·에어록 + 끝의대지/시작의바다 아래에서 잔비 등이 들어오면 이전엔 아무 일도 없었고, 이제 원시 날씨 막힘 문장이 나온다.
  - 진홍빛고동의 "이미 쾌청" 판정도 같은 이유로 구름제거·에어록 아래에서 달라질 수 있다.
  - 모래뿜기가 특성으로 날씨를 시작한 것으로 처리돼 애니메이션 값(`animArg1`)이 모래바람으로 정해진다(이전엔 기술 경로라 비어 있어 앞의 값이 남을 수 있었음). 문장은 모래뿜기 전용 문장 그대로.
- 새 배틀 문장 0, 기존 한글 텍스트 변경 0, 세이브 구조 변경 0(`BattleStruct` 비트 1개, 세이브 밖).

## 검증

- **빌드:** `make hns -j8` 종료 0, 새 경고 0(`warncheck.sh`).
- **모래뿜기 표적 테스트**(`make check TESTS="Sand Spit"`, 가드 넣기 전): 새 테스트 `fails if overworld weather is present (Gen9)` PASS. 기존 3개는 `ABILITY_POPUP(player, ABILITY_SAND_SPIT)`까지 맞고 영어 `MESSAGE`에서 FAIL(이식 전 기준과 같은 상태).
- **세이브 정적 비교**(이식 전 사실 = 세션 임시 `b29/save-pre`, 코드 `a3572d8d`를 클론에서 빌드 — ROM 32,963,060 B로 같음): **PASS(FAIL 0, WARN 0)**.
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 `6002c047`): 러너 PASSED 2,750 / TOTAL 5,721(KNOWN_FAILING 25), 목록 PASS 2,745 — `test-baseline-seq384.txt` 대비 **사라진 PASS 0**, 새 테스트 8개(새 PASS 1 = 모래뿜기 Gen 9). 나머지 7개(델타스트림·끝의대지·시작의바다·테라폼제로 2·진홍빛고동 필드 쾌청·시작 상태 덮어쓰기 Gen 9)는 따로 돌려 이유 확인 — 모두 `Unmatched MESSAGE`(영어 문장 비교, 한글이라 안 맞음). 실패 위치는 `ABILITY_POPUP` 뒤 문장이라 Gen 9 분기(`WITH_CONFIG`)는 팝업까지 동작했다. 새 기준 [`test-baseline-seq385_5.txt`](test-baseline-seq385_5.txt)(**다음 비교 기준**).
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-b28 8`, 코드 `6002c047`): **558/652, 기대와 바이트 같음, `판정: PASS (기대와 같음)`**(`HNSB2021` 30/30 포함).
- **실기: 하지 않음(친구).**

## 실기(mGBA) 후보 — 친구

1. 비 맵(진청시티·40번도로 등)·눈 맵(은빛산·49/50번도로) 배틀: 배틀 시작 날씨 문장, 쾌청·비바라기 등으로 날씨를 덮을 수 있는지(지금과 같아야 함).
2. 은빛산 정상 레드(OBC 후): 이상해꽃 쾌청이 눈을 덮고 솔라빔을 바로 쏘는지.
3. 잔비·가뭄·모래날림·눈퍼뜨리기 등장, 모래뿜기 피격 시 팝업·`모래바람이 불기 시작했다!`(랜더마이저 등으로 있을 때).

## 친구 질문(사용자에게 보여 줌 — 아직 안 보냄)

- 필드 날씨 Gen 9 동작(필드 날씨 고정)을 켤지. A) 지금 동작 유지(추천, 이번 적용 값) B) Gen 9 켜기.

## 재확인·뒤 PR 메모

- 새 재확인 항목: 82(필드 날씨 설정 값 — 친구 답 대기), 83(모래뿜기 스크립트 배틀러 HnS 가드).
- 순서표: 385.5 적용.
- 다음은 seq 386 #10217(Pickpocket activation fix, L).
