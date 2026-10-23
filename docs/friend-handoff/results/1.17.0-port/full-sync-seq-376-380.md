# full-sync 묶음 26 — seq 376~380 (+ #10214 앞당김)

- 요청: 사용자 "나 혼자 할 수 있는게 있을까" → 사전 분석 범위 설명 → "ㄱㄱ"(2026-10-23). 사전 분석(세션 임시 `b26/w` 클론 시험 적용 + 3-way 병합)으로 사용자 혼자 정할 수 있는 범위인지 판단했다. 판단 근거는 아래 "결정"이다.
- 범위: seq 376~380(377 #10176은 묶음 25에서 371에 흡수). 381 #10181("Type checking paradise", XL)부터는 다음 묶음.
- upstream: 읽기 전용 사본 `/home/user/pokeemerald-expansion-upstream`에서 `git format-patch`/`git show`로 읽었다.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 376 | #8893 | `410fddfe` | 영어 설명·설정 주석 문법 정리(3-way 병합, 아래) | 32,962,340 |
| 377 | #10176 | (묶음 25 `830aee6c`에 흡수) | — | — |
| 378 | #10162 | `8155c3e5` | enum·형식 보강(`EggGroup` packed enum, `CanTradeMon`·`MapPreviewScreenType` enum 등) | 32,962,372 |
| 379 | #10178 | `8742b635` | 정의 없는 선언 삭제 | 32,962,372 |
| 380 | #10170 | `4dbc706d` | 트레이너 시작 상태에 날씨 12종 | 32,962,948 |
| 385 앞당김 | #10214 | `cf44d86e` | 시작 상태 메시지 회귀 수정(#10170 뒤 바로) | 32,963,268 |

이식 전(`c25ec76d`, BGM 10-23 반영 뒤) 32,963,316 B → **32,963,268 B(−48)**, EWRAM 250,408·IWRAM 25,516 같음, 최종 SHA1 **`3c8bfa09be2fc46afbf2e5db62a61fa479900243`**.

## PR별 내용과 HnS 판단

### 376 #8893 — The big grammar update

- upstream은 5파일(`include/config/battle.h`·`item.h`, `src/data/abilities.h`·`items.h`·`moves_info.h`)의 **영어** 특성·도구·기술 설명과 설정 주석을 고친다. 일부 설명은 세대별 `#if` 갈래가 되고, 도구 공용 설명 배열(`sFullHealDesc` 등)은 각 도구 안으로 들어간다.
- HnS는 이름(`.name`)만 한글이고 설명은 아직 영어다. 바뀐 설명 바로 옆 줄이 거의 다 한글 이름이라 패치가 705 hunk 거부됐다. 그래서 upstream 전·후와 HnS를 3-way 병합했다(세션 임시 `b28/merge3.py` — 겹치지 않는 인접 변경은 양쪽 다 적용, 겹치는 곳만 따로 판단).
- **한글 이름 보존:** 특성 317·도구 900·기술 934개 한글 이름 줄이 전부 그대로다(병합 전후 대조).
- **HnS 유지(도구 48곳):** HnS가 직접 쓴 도구 설명이다.
  - 볼 7종("Good for NORMAL, FLYING, ICE, and low level POKéMON." 등) — HnS 볼 성능 변경을 설명하는 문장.
  - 기술머신 41개 — upstream은 아직 `"?????"` 자리표시인데 HnS는 기술 설명을 채워 뒀다.
  - 그 밖에 HnS가 바꾼 가방 칸(`.pocket`)·가격·기술머신 번호 등 비문장 필드는 병합으로 그대로 남는다.
- **upstream 채택(기술 12곳):** HnS 기술 설명에 "An attack" → "Attack" 기계적 치환 흔적이 있다(예: `"is preparing Attack."`). `TRANSLATION_POLICY.md` 6항(기술 설명은 미번역 영어라 upstream대로)에 따라 upstream 문장을 넣었다. 겹치지 않는 다른 "Attack that …" 문장은 upstream이 안 바꿔서 HnS 그대로다.
- **설정 파일:** HnS가 값을 바꾼 define(`I_KEY_FOSSILS GEN_3`, `I_EXP_SHARE_*`, `B_UPROAR GEN_4`, `B_OVERWORLD_FOG GEN_3`, `B_THUNDERSTORM_TERRAIN FALSE`, `B_MIRROR_ARMOR_STICKY_WEB GEN_9`, 파티클 TRUE 등)은 값·주석을 HnS 그대로 두고, 값을 안 바꾼 define 84개는 upstream 새 주석으로 맞췄다(이어지는 주석 줄 1곳 손 맞춤 — `B_ABILITY_TRIGGER_CHANCE`). 값은 하나도 안 바뀐다.
- upstream이 넣은 줄 끝 공백 1개(`abilities.h` 발광 `#if`)는 지웠다.
- **새 텍스트:** 배틀 밖 영어 설명만 바뀐다(정책상 영어 그대로, 한글화는 나중에 친구). 새 배틀 문장 0.

### 378 #10162 / 379 #10178

- #10162: 형식만. `EggGroup`은 packed enum(1 B, `SpeciesInfo` 크기 같음), 거래 가능 여부·맵 미리보기 종류는 값이 같은 enum, 스위치에 `default: break;`. HnS 문맥 차이 4곳 손 맞춤(`SpeciesInfo` 포인터 공백에 HnS `overworldShinyPaletteModern` 포함, AI 날씨 스위치, Soak 지역 변수, `CanLearnTeachableMove` — TRUE/FALSE만 돌려줌).
- #10178: 정의도 호출도 없는 선언 삭제. HnS `ChooseStatBoostAnimation` 선언 위치만 달라 손으로 지웠다.

### 380 #10170 + 385 #10214 — 시작 상태 날씨

- #10170은 트레이너 시작 상태에 날씨(맑음·비·모래바람·싸라기눈·설경·안개, 영구·5턴)를 더한다. **HnS 트레이너(`trainers_hns.party` 등)·스크립트는 시작 상태를 쓰지 않는다** → HnS 배틀 변화 없음.
- 그런데 #10170은 시작 상태 블록 끝의 공용 메시지 호출을 지워서, 룸·순풍·맹세 시작 상태가 메시지·애니 없이 걸린다(upstream 회귀). upstream이 #10214(seq 385)로 고쳤으므로 바로 다음 커밋으로 앞당겼다(묶음 22·24·25 선례).
- #10214의 `battle_message.c` hunk(불바다·습지 문장 팀 토큰 `B_EFF_TEAM2` → `B_ATK_TEAM2`)는 HnS 한글 문장에 이미 들어 있다(`{B_ATK_TEAM2} 주변의\n불바다가 사라졌다!` 등) → 문장 변경 0.
- 턴 끝 무지개·불바다·습지 종료 때 `gBattlerAttacker`를 각 종료 시점에 넣는다. **불바다 종료 문장의 진영이 바뀐다(버그 수정, 재확인 6 해결):** HnS 턴 끝 불바다 단계는 `gBattlerAttacker`를 넣지 않아 `{B_ATK_TEAM2}`가 그 턴의 다른 배틀러를 가리켰고, 진영이 반대로 나오는 경우가 있었다(한글 회귀 세트가 `#10214 CHANGE EXPECTED`로 고정해 둔 출력). 이제 불바다가 깔린 쪽 이름이 나온다. `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 행 추가, 한글 회귀 기대 갱신(아래 검증).
- **커밋 메시지 정정:** `cf44d86e` 메시지의 "Sea of Fire ending prints the same team as before"는 틀렸다(코드만 보고 쓴 판단 — 한글 회귀가 바로잡음). 위 내용이 맞다.
- 안개 시작 상태 메시지 `B_MSG_STARTED_FOG`(HnS `B_OVERWORLD_FOG GEN_3`라 배틀 안개는 시작 상태로만 생김 — HnS 미사용).

## 결정 (사용자 혼자 정할 수 있다고 본 이유)

- #8893은 `TRANSLATION_POLICY.md` 5·6항(친구 확정 2026-10-12·10-13) 그대로다: 미번역 영어 설명은 upstream 변경을 넣고, 한글 이름·친구가 끝낸 텍스트는 보존. HnS가 직접 쓴 도구 설명(볼·기술머신)은 HnS 기능 설명이라 보존.
- 게임 동작 변화는 1개뿐이다: 불바다 종료 문장의 진영(재확인 6 — 해결 조건이 처음부터 "seq 385 #10214"였다). 그 밖에는 설정 값 0개 변경, 형식 변경은 크기·값 같음, 시작 상태 날씨는 HnS 미사용, #10214의 나머지는 #10170 회귀를 되돌리는 것.
- 새 배틀 문장 0, 기존 한글 텍스트 변경 0.

## 검증

- **빌드:** 커밋마다 `make hns -j8` 종료 0, 새 경고 0(`warncheck.sh`). ROM은 위 표.
- **세이브 정적 비교**(이식 전 사실 = 세션 임시 `b28/save-pre376`, 코드 `c25ec76d`를 클론에서 빌드 — ROM 32,963,316 B로 같음): **PASS(FAIL 0, WARN 0)**.
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 `cf44d86e`): 러너 PASSED 2,749 / TOTAL 5,713(KNOWN_FAILING 25), 목록 PASS 2,744 — `test-baseline-seq375.txt` 대비 **사라진 PASS 0, 새 PASS 0**. 새 테스트 4개(`starting_status/weather.c` 3, `general.c` 1)는 FAIL: 영어 `MESSAGE` 비교가 한글 문장과 안 맞음(`Unmatched MESSAGE`, 같은 구조의 기존 지형 시작 상태 테스트도 기준 목록에서 FAIL). 새 기준 [`test-baseline-seq380.txt`](test-baseline-seq380.txt)(**다음 비교 기준**).
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-b26b 8`, 코드 `cf44d86e`): 558/652. 기대(563)와 다른 5개는 모두 불바다 종료 문장 진영(`Unmatched MESSAGE`, 기대 문장이 이식 전의 반대 진영): HNS9918 K12·K22·K23(`#10214 CHANGE EXPECTED`), HNS9680 2-13(1/3), HNS9714 3-04(2/12). 받아들인 변화라 README "기준 갱신" 절차대로 `kortests/expected/summary.txt`를 이 실행 요약으로 바꾸고 `summary.meta`·README 기대(558/652, 기대 FAIL·INVALID 94)를 고쳤다. 다시 돌린 `kor-b26c` → **기대와 바이트 같음, `판정: PASS (기대와 같음)`**. 다른 647개 상태 줄은 그대로(`HNSB2021` 30/30 포함).
- **실기: 하지 않음(친구).**

## 실기(mGBA) 후보 — 친구

1. 가방에서 도구 설명 몇 개(회복약·상태 회복·볼·기술머신) — 영어 설명 문장이 창 안에 들어가는지, HnS 볼·기술머신 설명이 그대로인지.
2. 배틀 기술 설명(L 버튼)·요약 화면 특성 설명 — 영어 줄바꿈이 창 안에 들어가는지.
3. 교환(링크·스핀 교환이 있으면) 가능/불가 문구.
4. 더블배틀 불꽃의맹세+풀의맹세 불바다: 상대 쪽·내 쪽 각각 끝나는 턴의 `…주변의 불바다가 사라졌다!` 진영.

## 재확인·뒤 PR 메모

- 새 재확인 항목 없음. 재확인 6 해결(#10214).
- 순서표: 376·378·379·380 기록, 385는 앞당김(다시 넣지 않음).
- 다음은 seq 381 #10181(Type checking paradise, XL).
