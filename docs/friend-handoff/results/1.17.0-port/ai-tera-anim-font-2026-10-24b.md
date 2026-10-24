# 묶음 32~39 후속 결정 반영 — 회복·방어 애니 / 설명 글꼴 / AI 테라 설정 / 화학의힘 (친구 답 2026-10-24b)

원문 [`FRIEND_REPLY_2026-10-24b.md`](../../FRIEND_REPLY_2026-10-24b.md). 기존 작업 브랜치 `pokehns-expansion-kor`에서 필요한 파일만 고쳤다(메인 로컬 저장소 동기화·강제 변경 없음). mGBA 실기는 친구가 한다.

## 커밋

| 순서(친구 6절) | 커밋 | 파일 | 내용 |
|---|---|---|---|
| 1 회복 반짝임 | `0142ed9c` | `data/battle_scripts_1.s`, `data/battle_anim_scripts.s`, `test/hns_heal_sparkle.c`(새) | 1절 |
| 2 방어 애니 | `689686c2` | `include/constants/battle_anim.h`, `src/battle_anim.c`, `include/battle_anim_scripts.h`, `data/battle_anim_scripts.s`, `src/battle_anim_new.c`, `data/battle_scripts_1.s`, `include/battle_scripts.h`, `src/battle_move_resolution.c`, `test/hns_protect_anim.c`(새) | 2절 |
| 3 설명 글꼴 | `e8997de1` | `src/pokemon_summary_screen.c`, `src/menu_specialized.c`, `src/battle_controller_player.c`, `test/text.c`, `test/hns_desc_font_fit.c`(새) | 3절 |
| 4 스마트 테라·AI 테라 설정 | `19a6002e` | `include/config/battle.h`, `include/constants/config_changes.h`, `include/battle_terastal.h`, `src/battle_terastal.c`, `src/battle_main.c`, `src/trainer_util.c`, `src/data/trainers_hns.party`, `test/hns_ai_terastallization.c`(새) | 4절 |
| 5 화학의힘 | `84432cac` | `src/data/abilities.h` | 5절 |
| 문서 | (이 문서와 같은 커밋) | 원문, 이 문서, `RECHECK_BEFORE_COMPLETION.md` 86·89·90·91, `full-sync-seq-423-431.md` 솔라빔 정정, `BATTLE_MESSAGE_OUTPUT_CHANGES.md` 1행, `test-baseline-seq468c.txt`, STATUS·SESSION_LOG | |

커밋 순서는 5 → 4 → 1 → 2 → 3(작업한 순서)이고, 서로 독립이다.

---

## 1. 회복 기술 반짝임 — seq 428 #10416 A안 (`0142ed9c`)

### 반영 범위

- 공통 회복 처리 `BattleScript_PresentHealTarget`(upstream 이름 `BattleScript_HealTarget`)에서 HP 바 앞에 `playanimation BS_TARGET, B_ANIM_SIMPLE_HEAL` 한 번.
  - 이 경로를 타는 기술: 날개쉬기·잠자기·알낳기·우유마시기(`EFFECT_SOFTBOILED`)·아침햇살·광합성·달의불빛·모래모으기·프레젠트(회복)·메가솔 회복.
  - **대상 `BS_TARGET`:** 실제로 HP를 회복하는 포켓몬. 자기 회복 기술은 대상 = 사용자. **프레젠트는 상대를 회복**하므로 상대에게 나온다. upstream은 `BS_ATTACKER`라 프레젠트 회복 때 쓴 쪽에 나온다(HnS는 따르지 않음).
  - 일반 애니(`B_ANIM_SIMPLE_HEAL`)에서 공격자=대상=그 배틀러라 반짝임(`HealingEffect`, `ANIM_ATTACKER`)이 그 포켓몬 위에 그려진다(`TryHandleLaunchBattleTableAnimation(battler, battler, battler, …)`).
- 메가솔 회복 경로 `BattleScript_MegaSolActivatesHealing`: 공격 애니 → 특성 팝업 뒤 새 라벨 `BattleScript_PresentHealTargetContinue`로(upstream과 같음). **이전 HnS는 이 경로에서 광합성 애니가 두 번 나왔다**(테스트로 확인 — 이전 스크립트에서 FAIL).
- 기술 애니 안의 개별 반짝임 정리(같은 회복에서 두 번 나오지 않게):
  - upstream이 지운 것: 광합성·모래모으기 → 지움. 날개쉬기·달의불빛·알낳기·아침햇살은 HnS에 이미 없었다(9/15 이식 때 빠짐 — 그래서 반짝임이 사라졌던 것).
  - **upstream이 놓친 것: 우유마시기**(`HealingEffect2`) — 같은 공통 경로라 upstream대로면 두 번 나온다 → 지움.
  - **그대로 둔 것: 회복·태만함·회복지령**(`EFFECT_RESTORE_HP`). 이 셋은 공통 경로가 아니라 `BattleScript_EffectRestoreHp`를 써서 기술 애니 안 반짝임이 유일한 반짝임이다.
- 정적 대조(세션 임시 스크립트): 공통 경로를 타는 기술 9개 애니에 `HealingEffect` 0개, `EFFECT_RESTORE_HP` 3개는 1개씩.
- 바뀌지 않은 것: 회복량·발동 조건·기술 효과·한국어 문장과 줄바꿈(같은 `STRINGID_PKMNREGAINEDHEALTH`). 배틀 애니 끄기 설정에서는 다른 `playanimation`과 같은 규칙(`PlayAnimation` — 애니 대신 짧은 대기 `BattleScript_Pausex20`).
- 다른 회복 경로(치유파동·꽃의치유·생명의물방울·바라는것 등)는 upstream도 바꾸지 않았고 이번에도 그대로다.
- 콘테스트에서 광합성·모래모으기·우유마시기 애니의 반짝임도 같이 빠진다(같은 애니 스크립트, upstream도 같음).

### 테스트 — `test/hns_heal_sparkle.c` (언어 무관, 애니 순서만 봄)

| 테스트("HnS heal sparkle: …") | 내용 | 새 코드 | 이전 스크립트 |
|---|---|---|---|
| self-healing moves play it once on the user | 8기술(알낳기·우유마시기·날개쉬기·아침햇살·광합성·달의불빛·모래모으기·잠자기): 기술 애니 → 상대에게 없음 → 사용자에게 1번 → HP 바 → 더 없음 | PASS | FAIL |
| not played when the move fails at full HP | HP 가득 — 애니·반짝임·HP 바 없음 | PASS | PASS |
| Present's heal plays it on the healed target | 프레젠트 회복(RNG 254): 쓴 쪽 없음, 상대에게 1번 | PASS | FAIL |
| goes on the Pokemon that heals in a double battle | 더블 오른쪽 아군의 알낳기: 왼쪽·상대 둘에게 없음, 자신에게 | PASS | FAIL |
| Mega Sol healing plays the move animation once, then the sparkle once | 메가솔 광합성: 애니 → 팝업 → 애니 다시 없음 → 반짝임 1번 | PASS | FAIL(애니 두 번) |
| Recover-type moves don't add the general one | 회복·태만함·회복지령: 공통 반짝임 없음 | PASS | PASS |

---

## 2. 방어 성공 애니 — seq 441 #10467 A안 (`689686c2`)

### 반영 범위

- upstream 그대로 추가: 일반 애니 64번 `B_ANIM_PROTECTED_ITSELF` = `gBattleAnimGeneral_ProtectedItself`(방어막 스프라이트 `gProtectTemplate`·`SpriteCB_Protect`, 흔들림 `AnimTask_ShakeMon2`, 효과음 `SE_ICE_STAIRS`). 스프라이트 그래픽은 HnS `createsprite`가 자동으로 불러온다(`TryLoadSpriteAssets`).
- **HnS 적응:** HnS는 방어 판정 뒤 회피·방어 공용 `BattleScript_TargetAvoidsAttack`(문장 표 `gMissStringIds`, `B_MSG_PROTECTED`)으로 문장을 찍는다(upstream은 따로 `BattleScript_TargetProtected` + `printstring`). 그래서 `IsBattlerProtected` 분기만 새 HnS `BattleScript_TargetProtected`(·던지기 판 `…ConsumeFlingItem`)를 부르게 했다.
  - 내용: 짧은 대기 → **막은 포켓몬(`BS_SCRIPTING`)에게 애니** → 이전과 같은 `printfromtable gMissStringIds`(= `STRINGID_PKMNPROTECTEDITSELF` "…은(는) 공격으로부터\n몸을 지켰다!").
  - `BS_SCRIPTING`은 스크립트 실행 전에 막은 대상으로 정해진다(`gBattleScripting.battler = cv->battlerDef`, 기존 코드).
  - 회피(공중날기 등)·특성 무효 등 다른 분기는 그대로 `BattleScript_TargetAvoidsAttack`.
- 바뀌지 않은 것: 방어 성공 여부·연속 사용 확률, 니들가드·킹실드·토치카·블로킹·실크트랩·불꽃방패의 접촉 판정과 추가 효과, 와이드가드 등 진영 방어 범위, `MULTISTRING_CHOOSER = B_MSG_PROTECTED`(배틀아레나 판정이 읽음), 한국어 문장·줄바꿈. 이미 이식된 #10467 와이드가드 싱글 동작 등은 다시 건드리지 않았다.
- 효과: 막을 때마다 짧은 애니만큼 길어진다(친구 OK).

### 테스트 — `test/hns_protect_anim.c`

| 테스트("HnS protect anim: …") | 내용 | 새 코드 | 이전 경로 |
|---|---|---|---|
| played once on the Pokemon that protected itself | 방어·판별·니들가드·킹실드·토치카·블로킹·실크트랩·불꽃방패 8개: 공격 애니 없음, 공격 쪽에 없음, 막은 쪽에 1번, 더 없음 | PASS | FAIL |
| contact effects still follow | 니들가드 반동 피해, 킹실드 공격 하락(설정대로 1단계) | PASS | FAIL |
| a multi-hit move shows it once | 두번치기 — 1번 | PASS | FAIL |
| not played when the attack isn't blocked | 방어 없이 맞음 — 없음 | PASS | PASS |
| each protecting target once against a spread move | 더블, 상대 둘 다 방어 + 매지컬샤인: 각각 1번, 아군에게 없음 | PASS | FAIL |
| Wide Guard shows it on each protected Pokemon | 더블, 와이드가드 vs 매지컬샤인 + 단일 몸통박치기: 둘에게 1번씩, 단일 공격은 맞음 | PASS | FAIL |
| Fling into Protect still uses up the item | 던지기 막힘: 애니, 도구 소모 | PASS | FAIL |

---

## 3. 설명 글꼴 폭 맞춤 — seq 444 #10521 A안 (`e8997de1`)

### 반영 범위(upstream이 고친 5곳 전부)

| 화면 | 창 폭 | 기본 글꼴 | 바뀐 코드 |
|---|---|---|---|
| 요약 화면 특성 설명 | 144 px | FONT_NORMAL | `PrintTextOnWindowToFit` |
| 요약 화면 기술 설명 | 160 px | FONT_NORMAL | `PrintTextOnWindowToFit` |
| 요약 화면 콘테스트 설명(2곳) | 160 px | FONT_NORMAL | `PrintTextOnWindowToFit` |
| 기술 떠올리기 기술 설명 | 128 px | FONT_NARROW | `GetFontIdToFit(…, FONT_NARROW, …)` |
| 배틀 기술 정보 창 | 144 px | 창 글꼴 FONT_NARROW | 설명 부분만 `WrapFontIdToFit(…, FONT_NORMAL, …)` — NORMAL로 들어가면 그대로(창 글꼴), 넘칠 때만 좁은 글꼴 코드로 감쌈 |

- 설명 내용·수동 줄바꿈·창 크기·기술/특성 이름은 그대로. 새 번역·문장 줄이기 없음.
- 설명 문자열에 제어 코드는 없다(줄바꿈만, 정적 대조). 배틀 정보 창은 한글 머리줄(위력·명중, `CLEAR_TO`) 뒤 설명 앞뒤에 글꼴 코드가 들어가고 머리줄 바이트는 그대로다(테스트).

### 측정(현재 문자열·설정 기준 — 고정 기준 아님, 세션 임시 테스트, 저장소에 넣지 않음)

| 화면 | 대상 | 그중 한글 | 넘치던 것 | 글꼴 바뀜 | 바뀐 것 중 한글 | 바뀐 뒤에도 넘침 |
|---|---|---|---|---|---|---|
| 떠올리기 기술 설명 | 934 | 0 | 74 | **74** | 0 | 0 |
| 요약 기술 설명 | 934 | 0 | 0 | 0 | — | — |
| 배틀 기술 정보 | 934 | 0 | 0 | 0 | — | — |
| 요약 특성 설명 | 319 | 1(파동의방호) | 18 | **18** | 0 | 0 |
| 요약 콘테스트 설명 | 934(기술별) | 0 | 0 | 0 | — | — |

### 영문 설명 검증과 한글 설명 검증의 구분

- **지금(미번역 영문 설명):** 넘치는 92개가 모두 영문이고, 좁은 글꼴로 바뀐 뒤 모두 창 안에 들어간다(측정). 한글 설명 1개(파동의방호)는 원래 글꼴로 들어가 바뀌지 않는다.
- **앞으로 한글 설명: 문제 있음(질문 — 재확인 91).** HnS 한글 글리프 폭은 FONT_NORMAL 8 px, FONT_NARROW 8 px, **FONT_NARROWER 11 px**(더 큰 한글 글꼴 `gFont2KoreanGlyphs`)다(테스트로 확인). `GetFontIdToFit`은 넘치면 한 단계씩 좁은 글꼴로 내려가고 마지막 단계는 맞는지 보지 않으므로, **넘치는 한글 설명은 FONT_NARROWER로 바뀌어 오히려 더 넓어진다.** 지금은 해당 0이라 화면 영향 없음. 한글 글리프 자체는 좁은 글꼴들에서도 그려진다(글리프 표 있음).
- upstream 폭 테스트(`test/text.c` 3개 — 요약 기술·배틀 정보·요약 특성)는 "FONT_NARROWER가 가장 좁다"고 보고 그 글꼴로 재는데, 한글은 그렇지 않아 파동의방호가 FAIL했다. HnS에서는 **화면이 실제로 고르는 글꼴(`GetFontIdToFit`)로 재도록** 바꿨다 → 나중에 넘치는 한글 설명이 생기면 이 테스트가 FAIL로 잡는다.

### 테스트 — `test/hns_desc_font_fit.c` + `test/text.c`

| 테스트 | 결과 |
|---|---|
| HnS desc font: Korean glyph widths per font (8/8/11) | PASS |
| HnS desc font: Aura Guard's Korean description keeps FONT_NORMAL | PASS |
| HnS desc font: move descriptions fit on the Move Relearner screen(934, 고른 글꼴로 128 px 이하, 들어가면 FONT_NARROW 그대로) | PASS |
| HnS desc font: battle move info wraps only an overflowing description(넘칠 때 코드 위치·바이트, 안 넘칠 때 그대로) | PASS |
| Move descriptions fit on Pokemon Summary Screen(HnS 적응) | PASS |
| Move descriptions fit on battle move info window(upstream 새 테스트, HnS 적응) | PASS |
| Ability descriptions fit on Pokemon Summary Screen(HnS 적응 — upstream 그대로면 파동의방호 FAIL) | PASS |

실제 가독성·잘림은 실기로만 판정(친구).

---

## 4. 스마트 테라 이식 + AI 테라 설정 (`19a6002e`)

### 새 설정

- **이름·위치: `B_AI_TERASTALLIZATION`, `include/config/battle.h`**(`B_TERA_ORB_ALWAYS_CHARGED` 바로 아래). HnS가 추가한 설정이다(upstream 설정 아님 — 이름은 친구 제안을 저장소 `B_…` 명명에 맞춰 그대로 씀).
- 값: **HnS 빌드(`POKEMON_HNS`) FALSE**, 그 밖의 빌드 TRUE(기존 동작 그대로).
- 테스트에서 `WITH_CONFIG(B_AI_TERASTALLIZATION, …)`로 바꿀 수 있게 `include/constants/config_changes.h`에 등록(ROM 상수, 세이브와 무관). 게임 옵션·세이브 필드 추가 없음.

| 값 | AI가 조종하는 트레이너(상대·인게임 파트너) |
|---|---|
| FALSE(HnS) | 테라 타입 지정·스마트 테라와 무관하게 테라스탈 불가 |
| TRUE | upstream 후보 선정 + 기존 조건(사용 횟수·배틀 종류·다른 기믹·도구) + AI 판단대로 |

### 구현

- **스마트 테라 후보(upstream #10463 나머지):** `CreateNPCTrainerParty…`에서 테라 타입을 지정하지 않은 포켓몬도 트레이너가 `AI_FLAG_SMART_TERA`(HnS `AI: Smart Trainer`에 포함)면 `TYPE_MYSTERY`로 막지 않고 엔진이 만든 테라 타입을 유지 → 후보가 된다. 실제 사용은 기존 판정(`CanTerastallize` — 이미 썼는지, 다른 기믹, 메가스톤·Z크리스탈)과 AI 판단(`DecideTerastal` — 스마트 테라 AI가 이득일 때만)을 통과해야 한다. 아무 타입으로 바꾸는 것이 아니다(생성 때 정해진 타입). 트레이너 데이터 상대가 아닌 배틀(야생·프런티어·통신 등) 제외 규칙(HnS)은 그대로.
- **AI 판정:** "AI가 조종하는가"는 배틀러 컨트롤러로 본다(`BattlerHasAi`: 상대 `OPPONENT`·파트너 `PLAYER_PARTNER` 등). 플레이어(`PLAYER`)·통신(`LINK_*`)·녹화(`RECORDED_*`)는 막지 않는다. "플레이어 반대편" 기준이 아니라서 목호 같은 파트너도 포함된다.
- **두 단계에서 막음:** (1) 공통 판정 `CanTerastallize`(→ 쓸 수 있는 기믹 배정 `AssignUsableGimmicks`) — 쓸 수 있는 기믹이 테라가 되지 않으므로 AI가 고를 수 없다. (2) 발동 단계 `TryActivateGimmick` — 테라 발동 직전에 다시 확인. 테라 타입을 지워서 막는 방식은 쓰지 않았다.
- **테스트 전용 우회:** 테스트에서는 `ShouldTrainerBattlerUseGimmick`이 테스트가 정한 기믹을 그대로 돌려준다(TESTING 경로). 새 제한은 그 앞(`CanTerastallize` 안, TESTING과 무관)에 있어 이 우회로 건너뛰지 않는다 — 아래 제한 제거 실험으로 확인.
- 바뀌지 않은 것: 플레이어 테라스탈오브 소지·충전·배틀별 허용, 메가진화·Z기술·다이맥스 조건, 세이브 구조.
- **레드(OBC 후, `TRAINER_RED_POSTOBC_HNS`):** 이상해꽃·팔데아 켄타로스의 `Tera Type` 줄 삭제(이번 원문 4-4). 앞서 요청한 기록은 저장소 문서에서 찾지 못했지만 이번 원문대로 했다. **참고:** 레드는 `AI: Smart Trainer`라 설정을 TRUE로 바꾸면 테라 지정이 없어도 6마리 모두 스마트 테라 후보가 된다(이번 이식의 결과). HnS 트레이너 중 Smart Trainer는 27명, 파트너(목호 등)는 Basic Trainer라 TRUE여도 지정이 없으면 후보가 아니다.

### 테스트 — `test/hns_ai_terastallization.c` (OFF/ON 둘 다 같은 빌드에서 `WITH_CONFIG`)

| 테스트("HnS AI Tera: …") | OFF | ON |
|---|---|---|
| B_AI_TERASTALLIZATION is FALSE in HnS | PASS(기본값 확인) | — |
| AI opponent with a set Tera Type Terastallizes only when allowed | PASS(안 함) | PASS(함) |
| Smart Tera AI opponent Terastallizes only when allowed | PASS(안 함) | PASS(KO 가능할 때 함) |
| AI partner Terastallizes only when allowed (2대1, 파트너 AI) | PASS(안 함) | PASS(함) |
| an opponent not controlled by the AI is not blocked (테스트 스크립트 입력 = 녹화 컨트롤러, 통신 상대와 같은 취급) | PASS(함) | PASS(함) |
| the player Terastallizes whatever the setting | PASS | PASS |
| AI opponent still Mega Evolves when Tera is not allowed | PASS | — |

- **제한 제거 실험(세션 중, 저장소에 안 남김):** `IsAiTerastallizationAllowed`를 늘 TRUE로 바꿔 돌리면 OFF 3조(상대 지정·스마트 테라·파트너)가 FAIL → 테스트가 제한을 실제로 잡는다.
- 스마트 테라 후보 생성: upstream `CreateNPCTrainerPartyForTrainer doesn't set mystery tera type when Smart Tera AI flag is set` **FAIL → PASS**.
- "OFF: 타입 미지정 스마트 테라 상대"는 (1) 생성기 테스트(후보가 됨) + (2) 제한이 테라 타입을 보지 않는다는 코드 감사 + (3) 위 OFF 테스트의 조합으로 확인. 실제 게임의 비테스트 경로(`ShouldTrainerBattlerUseGimmick`의 `TYPE_MYSTERY` 검사)는 테스트 빌드에서 실행되지 않는다 — 코드 감사.
- 기본 OFF에서 upstream 테스트 중 AI 상대가 테라스탈하는 것은 FAIL로 바뀐다(의도). ON 빌드 전체 비교는 아래 6절.

---

## 5. 화학의힘 (`84432cac`)

- `ABILITY_POWER_OF_ALCHEMY` 이름 `과학의힘` → **`화학의힘`**(`src/data/abilities.h` 1곳). 인코딩 길이 같음(한글 4자 8바이트 + 끝).
- 같은 오역의 다른 복사본 없음(`git grep` — 소스·테스트·한글 회귀 기대값 모두 0). 번호·식별자·효과·포켓몬 할당·영문 설명 그대로. 다른 "과학" 표현 일괄 치환 없음.
- ROM에서 새 이름 바이트 1곳, 옛 이름 0곳(정적). 요약 화면·특성 팝업은 같은 `gAbilitiesInfo[].name`을 읽는다 — 화면 확인은 실기.

---

## 6. 검증

| 단계 | 했나 | 결과 |
|---|---|---|
| 빌드 `make hns -j8` | 함(작업 단위마다) | 종료 0, 새 경고 0(`warncheck.sh` — 테라 변경 때 전체 재빌드 로그 포함) |
| 새 회귀 테스트(위 각 절) | 함 | 24개 모두 PASS(테라 7·회복 6·방어 7·글꼴 4) + upstream 적응 3 PASS |
| 이전 코드로 같은 테스트 | 함(회복·방어·테라 제한) | 회복 4·방어 6·테라 3 FAIL → 테스트가 변경을 잡음 |
| 전체 `make check`(기본 OFF, `GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 `e8997de1`) | 함 | 러너 PASSED 2,819 / TOTAL 5,798(KNOWN_FAILING 12, ASSUMPTIONS_FAILED 45). 아래 표 |
| 전체 `make check`(AI 테라 ON 빌드 — 클론에서 `e8997de1` + HnS 값만 TRUE) | 함 | 러너 PASSED 2,822 / TOTAL 5,798. 아래 표 |
| 한글 회귀 kortests(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-b45 4`, 코드 `e8997de1`) | 함 | **558/652, `판정: PASS (기대와 같음)`** |
| 세이브 정적 비교(이전 사실 = 코드 `6a6e7f29`, 클론 빌드) | 함 | **PASS(FAIL 0, WARN 0)** — `B_AI_TERASTALLIZATION`은 ROM 상수라 세이브와 무관 |
| **mGBA 실기** | **하지 않음(친구)** | — |

### 전체 테스트 — 기존 기준(`test-baseline-seq468b.txt`)과 비교

| 빌드 | 새 PASS | PASS → FAIL·판정 못함 | 그 밖 |
|---|---|---|---|
| **기본 OFF(HnS 실제 값)** | 26 = 새 HnS 테스트 24 + `CreateNPCTrainerPartyForTrainer doesn't set mystery tera type when Smart Tera AI flag is set`(스마트 테라 이식, FAIL → PASS) + upstream 새 테스트 `Move descriptions fit on battle move info window` | **4: `Trainer Slide: Singles/Doubles/Multi: Attacker Tera`, `Multi: Opponent Tera`** — AI 상대가 테라스탈해야 하는 AI 테스트라 OFF에서 FAIL(의도) | `AI_FLAG_SMART_TERA: AI might tera if it gets saved from a ko` FAIL(2/2) → INVALID(OFF에서는 한 번도 테라하지 않아 확률 테스트가 판정 불가 — 의도) |
| **AI 테라 ON** | 25 = 새 HnS 테스트 중 23(+ OFF 기본값 확인 테스트는 ON에서 FAIL — 의도) + 위 2 | **0** | 트레이너 슬라이드 4개 PASS 그대로, `AI might tera…` FAIL(2/2) 그대로(영어 `MESSAGE`, 이전과 같음). 스마트 테라 이식으로 깨진 것 없음 |

- 새 기준 [`test-baseline-seq468c.txt`](test-baseline-seq468c.txt)(기본 OFF, **다음 비교 기준**).
- 실기용 ROM: 기준 커밋 **`e8997de1`**(문서 커밋 뒤에도 같음), `pokehns.gba` SHA1 **`ce168c0377436700da53929d04f5127c3dd0e5e6`**, ROM 32,964,052 B(이전 `4ec770aa` 32,963,636 B, +416), 빌드 환경 클라우드 컨테이너(Linux) `make hns -j8`, `arm-none-eabi-gcc 13.2.1 20231009`. 포함 변경: 이 문서 1~5절 + 10-24 알기술·조우 BGM·목호 장면 음악(`7f69a3be`·`17755b4d`·`4ec770aa`).

## 7. 기존 요청 상태(이번 보고와 구분)

- 재확인 79 알기술 전수, HGSS 조우 BGM(수영팬티 남), 목호전 뒤 마리·오박사 등장 음악: **반영 끝**(`7f69a3be`·`17755b4d`·`4ec770aa`, [`egg-move-bgm-2026-10-24.md`](../egg-move-bgm-2026-10-24.md)). 그 보고의 질문 2개(동작 클래스 조우곡 20명, 성도 과학자 5명 ROCKET)는 **답 대기**.
- 솔라빔 실기 기대 동작 정정: `full-sync-seq-423-431.md` 실기 1번을 "쾌청의 솔라빔·솔라블레이드, 비의 일렉트로빔이 충전 생략, 비의 솔라빔은 충전"으로 고침(데이터 확인: 솔라빔 `.weather = BATTLE_WEATHER_SUN`, 일렉트로빔 `BATTLE_WEATHER_RAIN`).
- 남은 친구 답: 394(36 팝업 대기), 89 (1)(2) 메모, 82, 72, 70 (a)(b), 71, 69, 새 91(한글 글꼴 폭), 실기 결과.

## 8. mGBA 실기 항목 — 친구

1. 회복: 날개쉬기·알낳기·우유마시기·아침햇살·광합성·달의불빛·모래모으기·잠자기 — 반짝임 한 번(우유마시기·광합성·모래모으기는 두 번 나오지 않는지). 회복은 이전처럼 한 번. 프레젠트 회복은 상대 위에.
2. 방어: 방어·판별·니들가드·킹실드 등으로 막을 때 막은 포켓몬에게 방어막+흔들림 → "몸을 지켰다!". 더블에서 두 마리 방어 + 전체기, 와이드가드.
3. 설명 글꼴: 기술 떠올리기 설명(긴 영문 설명), 요약 화면 특성 설명 — 좁은 글꼴로 창 안에 들어가는지, 짧은 설명은 그대로인지. 파동의방호 한글 설명.
4. 테라: 은빛산 레드(OBC 후)가 테라스탈하지 않는지(지정 해제 + 설정 OFF). 목호 동행 배틀.
5. 화학의힘: 요약 화면·특성 팝업 이름.
6. 솔라빔 정정 기대: 쾌청 솔라빔·비 일렉트로빔 바로 발사, 비 솔라빔은 충전.
