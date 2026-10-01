# seq 127 #9655 리뷰 R1 — 문자열·표

- 대상: `git diff` (기준 HEAD `05319fd9b7`) 중 `src/battle_message.c`(+76/−48), `include/constants/battle_string_ids.h`(+36/−8). 참고 `part-A.md`, `part-A-stringids.tsv`, upstream `32fcd64868`(전·후), `expansion/1.17.0`.
- 방법: 읽기 전용. diff 정독, upstream 전·후와 HnS 전·후의 enum 기호·`u16` 표 매핑·`COMPOUND_STRING` 본문을 스크립트로 기계 대조, `charmap.txt`로 바이트 계산, 표를 읽는 asm·C 경로를 `git grep`으로 추적.
- 검증 범위: **코드·데이터 대조만 했다.** 빌드·테스트·실기는 실행하지 않았다(규칙). `git diff --check` 두 파일 통과.

## 결론

| 등급 | 건수 |
|---|---|
| 수정 필요 | **0** |
| 경미 | 3 (m1~m3) |
| 확인만 | 6 (c1~c6) |

두 파일에서 upstream hunk가 빠진 것은 없다. 기존 한글 문장은 D7 1줄만 바뀌었다. 바뀌거나 새로 생긴 표를 읽는 경로를 모두 따라가 보았고, 표 밖을 읽거나 빈 칸(STRINGID 0)을 출력하는 경로는 찾지 못했다.

---

## 1. 한글 보존 — 이상 없음

HEAD와 작업 트리의 `gBattleStringsTable` 본문을 ID별로 전부 비교했다. 차이는 아래 18줄뿐이다(diff에서 한글이 든 `+`/`−` 줄도 18줄).

- 삭제 2
  - `ATTACKMISSED`: D1에 따른 삭제다.
  - `PASTELVEILENTERS`: 본문은 그대로 두고 ID만 `PKMNHEALEDPOISON`으로 바꿨다. 본문 문자열이 같다(28 B).
- 기존 문장 수정 1: `PKMNWOKEUPINUPROAR`(battle_message.c:288). `{B_ATK_NAME_WITH_PREFIX}`가 `{B_EFF_NAME_WITH_PREFIX}`로 바뀌었고 나머지 바이트는 같다(28 B [4,23]). D7과 일치한다.
- 새 문장 14: `new_sentences.tsv`의 "제안 한글"과 **문자열이 모두 같다**(14/14 MATCH).
  - 원 문장과 바이트가 같은 것: SCRCURED 마비·독·화상·잠듦 ↔ `PURIFYTARGET*CURED`, `PKMNAURORAVEIL` ↔ `PKMNRAISEDDEFSPDEF`.
  - 토큰만 다른 것: `SCRCUREDCONFUSION`은 `PKMNHEALEDCONFUSION`의 ATK를 SCR로 바꿨다. `PARTYCURED*`는 각 원문의 이름 토큰을 `{B_BUFF1}`로 바꿨다. 둘 다 tsv의 "재사용(토큰만)" 범위에 든다.
  - `PKMNATKNOTLOWERED`: charmap으로 다시 세어 보니 28 B [4,23]이다. `PKMNSXPREVENTSYLOSS`(26 B [4,21])에 `공격`을 넣은 화면과 같고, part-A 수치와도 일치한다.
- 그 밖의 `_("…")` 텍스트(`gText_StatSharply` `크게 `, `gText_drastically` `매우 크게 `, `gText_DefendersStatRose`)는 바뀌지 않았다.

## 2. 표 인덱스 안전 — 표 밖 읽기·빈 칸 출력 경로 없음

모든 표가 enum 이름으로 지정 초기화를 한다. 위치형(비지정) 초기화를 쓰는 표 4개(`gTrainerUsedItemStringIds` 등)는 이번 enum 변경과 관계없다. 이 표들을 C에서 직접 인덱싱하는 곳은 없다(`gMissStringIds` extern 선언 하나만 있음).

| 표(크기) | 읽는 곳 | MULTISTRING 설정 | 판정 |
|---|---|---|---|
| `gCureStatusStringIds`(11, 0~10 모두 채움) | bs1 :391 :761 :1280 :3885 :6321 :7130, bs2 :101 | `GetCuredStatusMessage`(0~5 또는 PROBLEM 9), 탈피·촉촉한몸 0~5, `TryImmunityAbilityHealStatus` 0~8, `ItemHealMonVolatile` 6·7, `HealStatusConditions` 0~5 | 안전. 빈 칸 없음 |
| `gPartyCureStatusStringIds`(9) | bs2 :94 | 비활성 파티 몬만 이 경로로 온다. `ItemHealMonVolatile`이 돌지 않고 `HealStatusConditions`가 0~5만 넣는다. 상태가 없으면 `noStatusInstr`로 간다 | 안전. [6~8] SCR/INFATUATION/TAUNT는 이 경로에서 닿지 않는다 |
| `gPurifyStatusCureStringIds`(11, [7][8] 빈 칸) | bs1 :851 | `curestatus` → 0~5 또는 9(`jumpifstatus STATUS1_ANY`로 막혀 있어 실제로는 0~5) | 안전. 빈 칸에 닿지 않는다 |
| `CureStatusBerryEffectStringID`(11, [7][8] 빈 칸) | bs1 :7195 :7206 | `TryCure*` 0~5(hold_effects :678~747), 리샘 루프 0~6(bsc :12403~12433) | 안전. CONFUSION이 8에서 6으로 옮겼지만 지정 초기화와 설정 코드가 모두 이름을 쓰므로 맞물린다 |
| `gHurtByStringIds`(2) | bs1 :7011 (`BattleScript_HurtAttacker`) | 까칠한피부 `B_MSG_HURT`(util :4069), 울퉁불퉁멧·자보·애터 `B_MSG_HURT_BY_ITEM`(hold :258 :355 :379). 호출 경로는 이 셋뿐 | 안전. 두 값의 한글 본문이 같아서 이전 값이 남아 있어도 출력은 같다 |
| `gRemoveHazardsStringIds`(6, [0] 빈 칸) | bs1 :5116 | `hazardType`(1~5 루프, bsc :7277 :9630) | 안전(HEAD와 같음) |
| `gWeatherEndsStringIds`(9) | bs1 :4507 :7688 | `sWeatherFlagsInfo[].endMessage`(이름), `RemoveAllWeather` 0~8 | 안전. `B_MSG_WEATHER_END_COUNT` 대비값(bsc :7215)은 `B_WEATHER_ANY` 검사(bs1 :7683) 뒤에서만 불려 닿지 않는다. 이 부분은 HEAD 때부터 있던 코드다 |
| `gMissStringIds`(3) | bs1 :546 :2236 :8368 (:6802는 호출자가 없는 스크립트) | `B_MSG_AVOIDED_ATK`·`B_MSG_PROTECTED`(move_resolution :1824 :1835, bs1 :2235) | 안전. [0]=AVOIDED(D1) |
| `gMentalHerbCureStringIds` | **읽는 곳 없음**(HEAD의 `printfromtable` 2곳은 삭제됨). upstream도 같다 | — | 표 밖을 읽을 위험이 없다. 비트 순서로 재배치했지만 지정 초기화라 문제없다 |
| 멘탈허브 비트 | bs1 :7256~7266 `CMP_BITMASK` (`*ptr & (1 << value)`, bsc :4811) | `TryMentalHerb`가 0으로 시작해 `|= 1 << B_MSG_MENTALHERBCURE_*`로 모은다(hold :428~473). 비트 0~5라 u8 안에 들어간다 | 일치한다. Ret·Fling 모두 같은 분기를 쓴다 |
| `gReflectLightScreenSafeguardStringIds`, `gSwitchInAbilityStringIds`, `gAbilityWeatherChangeStringId` | 기존과 같음 | enum 값 변화 없음 | 안전 |

- 방벽 해제 `B_MSG_BREAK_*`는 값형(1, 2, 4)과 `CMP_COMMON_BITS`(bs1 :3786~3792, bsc :3718~3722, util :9233~9249)를 그대로 쓴다. 인덱스형 표·enum은 넣지 않았다(함정 2를 지킴).
- enum 재배열(`B_MSG_CURED_*`, 멘탈허브, `B_MSG_WEATHER_END_*`)의 값을 asm·C·test에서 숫자 리터럴로 쓰는 곳은 없다(`cMULTISTRING_CHOOSER, <숫자>` 전수 확인).

## 3. 결정 준수 — 위반 없음

| 결정 | 확인 |
|---|---|
| D1 | `STRINGID_ATTACKMISSED`는 enum·표 어디에도 남지 않았다(전 저장소 grep, docs 제외). `gMissStringIds[B_MSG_MISSED]`=`PKMNAVOIDEDATTACK`(:1036). `Cmd_resultmessage`는 bsc :2051 `STRINGID_PKMNAVOIDEDATTACK; // HnS:` |
| D2 | `PKMNSXMADEYINEFFECTIVE` 한글(:488)은 바뀌지 않았다. `gFlashFireStringIds[NO_BOOST]`(:1416)도 그대로다 |
| D3 | `PKMNFLEDUSING` `{PLAY_SE 0x0011}무사히 도망쳤다\p`(:514)는 바뀌지 않았다 |
| D5 | SCR/PARTY 마비 회복 둘 다 `몸저림이 풀렸다!`다. 열매 `PKMNSITEMCUREDPARALYSIS`(`…로\n마비가 풀렸다!`)는 그대로다 → c5 참고 |
| D6-a | `REFLECT/LIGHTSCREEN/AURORAVEILWOREOFF` `{B_ATK_PREFIX1}`는 바뀌지 않았고 다시 추가하지도 않았다 |
| D6-c | `COMMANDERACTIVATES`(:867)는 바뀌지 않았다 |
| D7 | :288 토큰만 바꿨다. `gEffectBattler`를 넣는 곳은 세 경로 모두 확인했다: 턴 종료 end_turn :1237~1245, 행동 전 move_resolution :120, 배틀팰리스 util2 :140 |
| 함정 3 | `CureStatusBerryEffectStringID`의 PROBLEM/NORMALIZED를 유지했다(:1402~1403). FREEEZE→FREEZE는 헤더·표 3곳·코드 전부 일관되게 바꿨다(`FREEEZE`는 저장소에 0건) |
| `gText_StatSharply` | `크게 `(:77)·`매우 크게 `(:190)·`{B_BUFF2}올라갔다`(:80) 그대로다. upstream도 C 코드는 바꾸지 않았고 문자열 어순만 바꿨으므로 충돌하지 않는다 |
| HnS 출력 정책(HEAD판) | 장판별 `*DISAPPEAREDFROMTEAM`: `gRemoveHazardsStringIds` 5종 그대로이고, 강철은 `SHARPSTEELDISAPPEAREDFROMTEAM`=`PKMNBLEWAWAYSHARPSTEEL`로 바이트가 같다 · 방벽 순차: 값형 비트 유지 · 치유의마음: bs1 :6315 `HEALERCURE` 유지 · 생명의구슬 `LOSTSOMEOFITSHP` 문장 그대로 · 오로라베일: `[B_MSG_SET_AURORA_VEIL]`=`PKMNRAISEDDEFSPDEF`(:1126) 유지 |

## 4. upstream hunk 누락 — 없음

- 헤더 9/9, battle_message.c 30/30 hunk를 part-A 표와 대조했다.
- 기계 대조 결과(upstream 전→후 대비 HnS 전→후, enum 기호와 `u16` 표 전부)를 보면, upstream과 다른 곳은 다음뿐이다.
  - `STRINGID_STICKYWEBDISAPPEAREDFROMYOU` 미추가
  - `gBreakScreensStringIds` 미추가
  - `[B_MSG_SET_AURORA_VEIL]` 매핑 HnS 유지
  - `gCureStatusStringIds`·`CureStatusBerryEffectStringID`의 PROBLEM/NORMALIZED 항목 추가·유지
  - `B_MSG_BREAK_*`·`B_MSG_SET_AURORA_VEIL`·WOREOFF 3개는 HnS에 이미 있어서 다시 추가하지 않음
  - 모두 part-A가 "제외/수정해서"로 적고 `// HnS:` 주석을 단 의도된 차이다.
- "제외" 판정 근거 점검
  - 영문 본문만 바뀐 hunk(#5~#20)
    - upstream 토큰이 바뀐 줄을 따로 뽑아 보았다(PKMNCUTSATTACKWITH, PKMNMOVEBOUNCED, CURSEDBODYDISABLED, PKMNGOTFREE, SYMBIOSISITEMPASS 등 20여 줄).
    - HnS 한글은 이미 upstream 이후 토큰과 같거나, 빠진 것이 특성명 토큰뿐(팝업과 중복)이다.
    - 이 hunk를 제외해서 한글 출력의 대상 배틀러가 어긋나는 곳은 찾지 못했다.
  - 토큰을 새로 추가하는 hunk는 선택 개선이라 하지 않았다. 해당하는 것은 NATUREPOWERTURNEDINTO·ITEMRESTOREDSPECIESPP·ATKGOTOVERINFATUATION이다. 기존 한글이 새 토큰 없이도 성립하므로 타당하다.
  - `ENEMYABOUTTOSWITCHPKMN` `\p` 제거: upstream이 짝이 되는 스크립트를 바꾸지 않았으므로(영문 문장만 바뀜) HnS 두 쪽 구성을 유지해도 된다.
  - `PKMNSWILLPERISHIN3TURNS`는 영문으로 남아 있다(별건, BRIEF대로).
- #9856과 #10064는 이 두 파일을 건드리지 않는다.

## 5. STRINGID 번호 변화의 영향 — 숫자 의존 없음

- 값 변화(헤더 파싱으로 계산)
  - 738개 중 70개가 바뀐다. `ATTACKMISSED` 다음부터 `PKMNAURORAVEIL` 전까지는 −1, 그 뒤는 상쇄되어 0이다. `ZENMODEENDED` 뒤 HnS/Champions 블록은 +12다.
  - `STRINGID_TABLE_START`(7)와 그 이하 값은 그대로다. 그래서 `battle_controller_oak_old_man.c`의 `*stringId == 1`, `battle_tv.c:170`의 `stringId > STRINGID_TABLE_START`는 영향을 받지 않는다.
  - `STRINGID_COUNT`는 725에서 737이 된다.
- 세이브: STRINGID를 저장하는 세이브 구조체는 없다(`savedStringId`는 전투 중 임시값이다).
- 통신: 링크 전투는 PrintString에 STRINGID를 실어 보낸다. 같은 빌드끼리만 맞는 것은 원래 그렇다.
- 녹화 배틀: 입력만 기록하므로 영향이 없다.
- 배틀 아레나: `BattleArena_DeductSkillPoints`는 `case STRINGID_…` 기호로 비교한다. 번호 변화와 무관하다. 다만 스크립트가 출력 ID를 바꾼 데 따른 판정 변화가 있다 → c1.
- `gBattleCommunication[MISS_TYPE]`(u8)에 이제 STRINGID 값 `STRINGID_PKMNEVADEDATTACK`이 들어간다(bsc :1170).
  - HnS 값은 96이라 u8에서 잘리지 않고, `B_MSG_PROTECTED`(1)와 같아지지도 않는다.
  - 이 값과 비교하는 곳은 Memento bs1 :3626, bsc :9807, arena :392이다. 이전 값(`B_MSG_MISSED`=0)과 결과가 같다.

## 6. 다른 파일의 참조 — 지워진 ID·옛 이름 참조 없음

- 저장소 전체(docs 제외)에 다음 이름은 0건이다: `STRINGID_ATTACKMISSED`, `STRINGID_PASTELVEILENTERS`, `B_MSG_CURED_FREEEZE`, `gStatusCureStringIds`, `gSpinHazardsStringIds`, `gDefogHazardsStringIds`. 옛 enum 타입명 `enum CureStatusBerryEffectStringID`도 C 코드에서 쓰는 곳이 없다.
- `test/` 아래에는 새로 생기거나 바뀐 ID·enum을 C 기호로 쓰는 곳이 없다. `test/text.c`의 변경은 주석 1줄(upstream과 같음)이다.
- `battle_tv.c`의 특수 문자열 목록과 case에는 지워진 ID가 없다. 의미가 바뀐 경로는 c2에 적었다.

---

## 지적 사항

### 경미

**m1. 아무도 읽지 않는 데이터가 남음 (ROM만 차지)**
- 위치
  - battle_message.c:258 `STRINGID_PKMNAURORAVEIL`: 표 매핑을 HnS 쪽으로 유지해서 아무 곳에서도 출력하지 않는다(41 B).
  - :948 `gMentalHerbCureStringIds`: 읽는 곳이 없다. upstream도 같다.
  - 이번에 쓰이지 않게 된 문자열: `PKMNPARALYSISCURED`·`PKMNPOISONCURED`·`PKMNBURNCURED`·`PKMNSTATUSNORMAL`·`ITEMCUREDSPECIESSTATUS`·`PKMNBLEWAWAYSHARPSTEEL`. grep으로 확인해 보니 HEAD에서 1~3곳이던 사용처가 0곳이 되었다.
- 실패 시나리오: 없다. 출력과 동작에는 영향이 없다.
- 제안: upstream 구조를 맞추는 쪽이 낫다고 보고 그대로 둔다. part-A가 이미 기록했다.

**m2. `gKOFailedStringIds[B_MSG_KO_MISS]` = `STRINGID_PKMNEVADEDATTACK`이 D1 통일과 어긋남(죽은 표)**
- 위치·근거: battle_message.c:1283. HnS와 upstream 모두 `printfromtable gKOFailedStringIds`가 없다.
- 실패 시나리오: 지금은 없다. 나중에 이 표를 쓰는 upstream 경로가 들어오면 일격기 빗나감만 `…은(는)\n공격을 피했다!`로 나와 D1("모든 빗나감은 `…에게는 맞지 않았다!`")과 다르다.
- 제안: 지금은 upstream과 같이 둔다. 이 표를 쓰는 upstream 커밋이 오면 `// HnS:`로 `PKMNAVOIDEDATTACK`을 쓸지 다시 본다. 이 내용을 STATUS에 메모해 두기를 권한다.

**m3. `test/text.c` 폭 검사에서 `STRINGID_PARTYCURED*`의 `{B_BUFF1}`이 빈 채로 검사됨**
- 근거: test/text.c :760~763의 `PREPARE_SPECIES_BUFFER` case에 `ITEMCUREDSPECIESSTATUS`는 있지만 PARTYCURED 6개는 없다. 기본값은 빈 문자열이다(:622). upstream 1.17.0도 같다.
- 실패 시나리오: 실제로 넘칠 문장은 아니다. 첫 줄은 `{종족명}의`이고 둘째 줄은 기존 문장과 같다. 다만 테스트가 PASS여도 종족명이 들어간 폭은 검증하지 않은 것이다.
- 제안(선택, 파트 D): 위 case에 PARTYCURED 6개를 더한다. 통합 테스트 결과를 기록할 때 "PARTYCURED 폭은 수동 계산"이라고 구분해 적는다.

### 확인만

**c1. 배틀 아레나 기술 점수 판정이 바뀜(upstream과 같은 부작용, 기록 권장)**
- 근거
  - `BattleArena_DeductSkillPoints`(battle_arena.c:414~439)는 특정 ID가 출력될 때 −3점을 준다.
  - #9655 스크립트 교체로 그 ID들이 더는 출력되지 않는다. 사용처를 HEAD→작업 트리로 세면 다음과 같다.
    - `PKMNSXBLOCKSY` 3→0(`SCR_ITDOESNTAFFECT`로 교체)
    - `PKMNSXMADEYUSELESS` 1→0
    - `PKMNPROTECTEDBY` 1→0(`ITDOESNTAFFECT`로 교체)
    - `PKMNPREVENTSUSAGE` 1→0(`POKEMONCANNOTUSEMOVE`로 교체)
    - `PKMNSXPREVENTSFLINCHING` 1→0
    - `PKMNPREVENTSSTATLOSSWITH` 2→1(위협 방지는 `PKMNATKNOTLOWERED`로 교체)
  - HnS에도 `BattleFrontier_BattleArena*_hns` 맵이 있다.
- 시나리오(추정): 아레나에서 방음·방탄·옹골참·습기·위협 방지에 막힌 쪽이 −3점을 받지 않는다.
- upstream `32fcd64868`도 아레나 목록을 바꾸지 않았으므로 동작은 upstream과 같다.
- 실기 확인은 하지 않았다.
- 제안: 수정하지 않는다. seq 127 결과 문서의 "문자 외 동작 변화"에 한 줄 적는다.

**c2. 옹골참 일격기 방지가 `STRINGID_ITDOESNTAFFECT`를 출력하게 되어 `battle_tv.c`(:186~190)가 "효과 없음"으로 집계함**
- 근거: 이 ID는 비링크 전투에서도 `TrySetBattleSeminarShow()` 후보가 된다(싱글, 플레이어 공격 조건). upstream도 같다.
- 실기 확인은 하지 않았다.
- 제안: 수정하지 않는다. 기록만 해 둔다.

**c3. `gCureStatusStringIds`의 PROBLEM/NORMALIZED 대비값은 닿지 않는 방어용 항목임**
- 근거
  - :1493~1494가 대비값이다.
  - `GetCuredStatusMessage()`가 PROBLEM을 반환하려면 6상태 비트가 모두 없어야 한다. 그런데 모든 호출 경로가 `STATUS1_ANY`나 `STATUS1_CAN_MOVE` 검사 뒤에 있다.
  - `CureStatusBerryEffectStringID`의 PROBLEM/NORMALIZED(:1402~1403)도 지금 설정하는 곳이 없다.
  - :1401 주석 "so these indexes stay inside the table"은 함정 3을 지키려고 남긴 것이다. 지금 이 값을 넣는 경로는 없다.
- 판단: 표가 넘칠 위험을 없애는 의도된 차이이므로 그대로 둔다.

**c4. `gPartyCureStatusStringIds[CONFUSION]` = `SCRCUREDCONFUSION`({B_SCR}) 등 [6~8]**
- 근거: 파티(비활성) 경로에서는 닿지 않는다(2절). upstream과 같다.
- 판단: 그대로 둔다.

**c5. part-A 질문 1(D5 범위)**
- 친구 답장의 "도구에 의한 회복은 따로 `마비가 풀렸다!`"는 Champions 기준이다.
- Champions에는 전투 중 가방 도구가 없다. 그래서 이 말은 지닌 도구·열매 문장(`PKMNSITEMCUREDPARALYSIS` `…{도구}로\n마비가 풀렸다!`)을 가리킨다고 읽는 것이 자연스럽다.
- REPORT D5 선택지 A("새 ID에 `몸저림` 본문 재사용")와도 맞는다.
- 현재 `PARTYCUREDPARALYSIS` `몸저림이 풀렸다!`는 결정에 맞는다고 판단한다. 다만 이것은 추정이므로 친구 확인이 필요하면 질문으로 남긴다.

**c6. `ItemHealMonVolatile()`의 `ITEM3_STATUS_ALL` 분기는 헤롱헤롱만 풀어도 `B_MSG_CURED_CONFUSION`을 넣음**
- 근거·시나리오: battle_util.c :10289. 전투 중인 몬이 헤롱헤롱만 걸린 상태에서 만병통치제를 쓰면 `…의\n혼란이 풀렸다!`가 나온다.
- upstream 1.17.0과 같다. 파트 C 범위이고 이 표의 크기 안이다.
- 판단: 수정하지 않는다.

## 미확인

- 통합 빌드에서 `test/text.c` "Battle strings fit…"이 새 문자열 12개에 대해 PASS하는지. 메인이 전체 테스트 중이라 확인하지 않았다.
- c1·c2 아레나·TV 영향의 실기 재현.
