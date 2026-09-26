# g3 — Battle General / Moves / Abilities / Items 변경 인벤토리 (89개 PR)

- 대상: `inv/g3_battle_changes_moves_abilities_items.tsv`의 89개 PR(1.15.3~1.17.0의 Battle General Added/Changed, Moves, Abilities, Items)
- 기준: HnS 스냅샷 `<HnS 작업트리>/`(HEAD `0d89762071`), upstream `<upstream clone>/`. 라인 번호는 스냅샷 기준이다.
- 조사 방법: PR별 upstream 커밋 diff와 HnS 코드를 직접 비교했다. 추가된 줄이 HnS에 있는지 세는 스크립트(`scratchpad/g3/presence.py`), `git apply --check`와 `-R --check`, `git log 3efb836f72..<c>^1`로 선행 커밋을 찾는 방법, SESSION_LOG·STATUS의 이식 기록을 함께 썼다. 쓰기 작업은 없다.
- 전제: HnS는 upstream **master(1.15.2 개발) 계열**이다(merge-base `3efb836f72`). 1.16.0의 upcoming 전용 리팩터는 HnS에 없다. 예: #8497 loadspritegfx 제거, #9730 Stat Change(`battle_stat_change.c`), #9655 Battle Messages, #9939 accuracy canceler, #9657 DamageContext, #10426 SetMoveEffect 시그니처, `config_changes.h`. HnS는 `struct BattleContext *ctx`와 `generational_changes.h`를 쓴다.
- 분류 규칙:
  - 1.16.x/1.17.0 PR 중 upcoming 리팩터가 만든 회귀만 고친 것은 `HNS와 무관`으로 분류했다.
  - 표기(cv/ctx·함수명·매크로)만 다른 것은 `미적용·안전 이식 가능(수동 적응)`으로 분류했다.
  - 리팩터가 먼저 있어야 하는 것은 `미적용·선행 필요`로 분류했다.
- 거대 PR(#8943, #9777, #10593, #9172, #8893, #10151)은 핵심 hunk만 표본으로 확인했고, 표에 "표본 확인"이라고 적었다.

## 요약

### 판정별 개수 (합계 89)

| 판정 | 개수 | PR |
|---|---|---|
| 이미 적용 | 14 | #9678, #9740, #9735, #9769, #10249, #10256, #10416, #10257, #10591, #10592, #10324, #10541, #10189(HnS 자체 커밋 977fa3bae1), #10423(원 브랜치 커밋이 HEAD의 조상) |
| 부분 적용 | 4 | #10151(Champions 수면 턴만), #9777(STRINGID 11개와 효과 단계 메시지·아이템 팝업만), #10025(코드만, 전용 체력박스 자산·OAM 없음, 기본 FALSE), #10145(HNS식 이식, 잔여분 불필요) |
| 미적용·안전 이식 가능 | 40 | 그대로 또는 수동 적응. 실익 있는 것은 아래 "이식 추천" 참조. 나머지 다수는 순수 리팩터라 이식 가치가 낮다: #9417, #9786, #9916, #10253, #10338, #10384, #10459, #10471, #10630, #10640, #10421, #10106, #9688, #10297, #9429, #9408 |
| 미적용·선행 필요 | 10 | #9657, #9832(#9784), #9928·#10057(#9730), #9172·#9676·#9924·#10374(#8497 애니 리팩터 체인), #10454(#10170·#10214·#10395), #10595(#9939·#9730) |
| 충돌 | 7 | #8943, #9168, #9514, #9714, #9939, #10593, #8893 |
| HNS와 무관 | 14 | upcoming 리팩터 회귀 수정 8개(#9474, #9564, #9595, #10345, #10589, #10265, #10634, #10713), upcoming 형태 코드의 주석 변경(#10666), 1.17.0 안에서 되돌려진 것(#10576→#10588), 기본 비활성 config(#9008), 테스트 전용(#10590, #10310), 마이그레이션 스크립트(#9511) |

### 이식 추천 (실익 기준 상위)
1. **#10318**: 전자부유·레이저포커스 중복 플래그를 정리한다.
   - HnS 실제 버그: 바톤터치가 `magnetRise`(V_BATON_PASSABLE)만 넘기고 `magnetRiseTimer`는 넘기지 않는다(`battle_main.c:3398-3405`). 그래서 받은 포켓몬이 영구히 떠 있게 된다(코드 추론).
   - 수정할 곳 20개가 upstream과 1:1로 대응한다. 수동 적응이 필요하다.
2. **#10428**: 기술 데이터 수정.
   - HnS 파찌파찌액셀은 `alwaysCriticalHit=TRUE`와 회피 상승을 동시에 가진다(Gen8+ 기준 버그).
   - 초승달의기도·브레이브차지 PP도 고친다.
   - 브레이브차지의 `.attack→.spDef` hunk는 빼야 한다. HnS `EFFECT_TAKE_HEART` 스크립트는 이미 정상이다.
3. **#10016**: 모래바람 `windMove` 제거. 1줄이고 fwd=ok다.
4. **#10262**: 트릭룸·원더룸·매직룸 애니가 대상 쪽에서 재생되는 버그 수정(`InitRoomAnimation` `data/battle_anim_scripts.s:2521`). 1줄이고 fwd=ok다.
5. **#10401**: 씨앗플레어·잎사귀의 `target_both` 버그 수정. merge-base부터 있던 버그라 값 9개를 수동으로 바꾼다.
6. **#9898**: `IsAnyTargetAffected`의 TARGET_ALL_BATTLERS·USER_AND_ALLY 판정 수정(목구멍스프레이 등, `battle_util.c:10946`).
   - 이 hunk는 `--check`를 통과한다.
   - Opportunist 순서 hunk는 #9784가 먼저 필요하므로 뺀다.
7. **#9616**: Gen5+ 소란. HnS가 GEN_LATEST이므로 실제 동작이 바뀐다.
   - `STRINGID_TARGETWOKEUP`은 한글 본문을 그대로 두고 토큰만 B_DEF→B_EFF로 바꾼다.
   - `STRINGID_PKMNWOKEUPINUPROAR`는 더 이상 출력되지 않는다. 사용자 확인 뒤 출력 변경 문서(`BATTLE_MESSAGE_OUTPUT_CHANGES.md`)를 갱신해야 한다.
8. **#10011**: 킹실드·페이탈클로 등 기술 설명 4개를 현재 메커니즘에 맞춘다. HnS 기술 설명은 전부 영문이다. 이름 문맥이 한글이라 수동으로 옮긴다.
9. **#10109, #10104, #10225**: 아이템 쪽 안전 수정.
   - #10109: `AddBagItem(ITEM_NONE)` 허점을 막는다. item.c 부분은 그대로 적용된다.
   - #10104: 누락된 가방 정렬 타입을 추가한다.
   - #10225: Strange Ball 동료 포켓몬 그래픽이 `#ifdef` 때문에 빠져 있던 것을 복구한다(ROM 약 +1.9KB).
10. **#9121 + #9474**: 배틀 스프라이트 버퍼를 4→2프레임으로 줄여 힙 16KB를 확보한다. 반드시 두 PR을 짝으로 넣는다. #9121만 넣으면 대타출동 로드 때 오버플로가 난다.
- 그 밖에 연출만 바뀌고 위험은 낮은 것: #10361(끈적끈적네트 애니), #9063(흡수 이펙트), #9473(애니 call 깊이, #9142 이식 시 필수), #10027(도구 아이콘 애니, 스크립트의 `loadspritegfx ANIM_TAG_ITEM_BAG` 삭제 필수).

### 충돌 목록 (상세는 아래 "충돌 상세")
- **#8943 12v12**
  - `gParties[4][6]`로 바꾸면서 HnS가 대폭 수정한 파일들과 정면 충돌한다: `pokemon.c`, `party_menu.c`, `battle_main.c`, `pokemon_storage_system.c`.
  - 한글 `sText_Link*` 문구와 겹친다.
  - EWRAM이 늘어난다. 비추천한다.
- **#9168 열매 전용 애니**: HnS의 #9777 아이템 팝업 삽입 스크립트와 겹친다. HnS 고유 `BattleScript_LumBerryCureStatusRet`도 같은 영역에 있다.
- **#9514 SetMoveEffect cleanup**: 한글 문자열 약 30개의 토큰 의미가 바뀐다. HnS 방벽 설치 메시지 스크립트와도 겹친다.
- **#9714 안개제거·정리정돈**: HnS가 바꾼 방벽·흰안개·신비의부적·장판 해제 메시지 경로(`TryDefogClear`, `*Return` 스크립트)와 정면 충돌한다.
- **#9939 명중 판정 canceler 통합**: 빗나감 STRINGID를 개명·삭제한다. HnS 한글 빗나감 문구 체계와 충돌하고, 선행 체인이 38커밋이다.
- **#10593 SetMoveEffect 테이블화**: HnS `SetMoveEffect` 안의 고유 수정이 사라진다(Gen1 반동 챌린지, 오로라베일 메시지, 방벽별 순차 출력, StealStats). #10426은 의도적으로 미이식돼 있다.
- **#8893 문법 대개편**: 영문 설명만 바꾸지만 한글화된 데이터 파일의 문맥과 전부 어긋난다. 원작업자가 이미 미이식을 결정했다.
- 부분 적용 PR 중 잔여분이 충돌하는 것:
  - #10151: Champions 규칙 기본 활성화 여부와 `STRINGID_BELCHCANTSELECT` 한글 문구.
  - #9777: 기존 문구 38개 영문 개정, 동적 복수형, `AFTERMATHDMG` 개명, Unseen Fist 메시지.
  - #10025: 퍼센트 전용 체력박스 자산과 HnS 전용 체력박스.
- 그룹1 충돌 PR과의 STRINGID 교차 확인 결과:
  - #9777 ↔ #10144: `STRINGID_ITDOESNTAFFECTTWOFOES`와 상성 메시지 순서.
  - #9939 ↔ #10144: 효과 없음·빗나감 메시지 묶음.
  - #9916 ↔ #10149: 끈끈손 경로의 `STRINGID_PKMNSXMADEYINEFFECTIVE`. HnS는 `PKMNSXMADEITINEFFECTIVE`를 스위트베일 문구로 따로 쓴다.
  - #8943 ↔ #9799: 링크·교체 메시지.
  - #9008은 `STRINGID_VICTORYCATCH`를 추가만 하므로 겹치지 않는다.

### 용량 주의
- **큼**: #8943 12v12. 정적 EWRAM +약 1,200B(struct Pokemon 12개 추가), 힙 +약 0.5KB, ROM 수 KB. 현재 EWRAM 여유는 약 13KB다.
- **작음이지만 기록할 것**:
  - #10225: ROM +약 1.9KB
  - #9008: 활성화해 이식하면 약 2~3KB이고 `BattleStruct` 비트가 늘어난다
  - #10593: 약 +0.5KB
  - #10151 잔여분: 작음. `freezeTurns`는 padding을 사용한다
  - #10027: 수백 B
  - #9473: EWRAM +13B
- **감소**:
  - #9121: 배틀 힙 -16KB. EWRAM 95% 상황의 힙 단편화를 줄여 준다.
  - #9142: 애니 서브루틴화로 ROM이 줄어들 것으로 추정한다.
  - #10253·#10338·#9916: 소폭 감소.

### 사용자 결정이 필요한 config·동작 변경 (판정과 별개로 주의)
HnS는 `GEN_LATEST = GEN_CHAMPIONS`다. 따라서 upstream 기본값 `GEN_LATEST`를 그대로 들이면 동작이 바로 바뀐다.
- **#10151 잔여분**: 마비 12.5%, 동결 상한, 첫 턴 기술 선택 제한 등 Champions 규칙이 켜진다.
- **#10454**: `B_OVERWORLD_WEATHER_OVERRIDE`를 GEN_LATEST로 두면 필드 날씨가 있는 배틀에서 날씨 특성이 팝업 + "하지만 실패했다!"로 바뀐다. 현 동작을 유지하려면 `GEN_8`로 둔다.
- **#9616 소란**: 메시지 경로가 바뀐다.
- **#10395**: 오리진펄스·하드론엔진 발동 문구가 새로 필요하다. 공식 한국어 문구를 확인하기 전까지는 미결이다.
- **#9525**: 스텔라 타입의 damageCategory가 바뀌어 HnS 챌린지 "타입별 물리/특수" 모드의 분류가 달라진다.
- **#10384**: `TYPE_MYSTERY` 가드가 빠져 무속성 리베레이션댄스에 자속 보정이 붙을 수 있다. 보류를 권장한다.
- **#10170**: 단독으로 옮기면 회귀가 생긴다. 반드시 #10214와 함께 옮긴다.

### 테스트 관련 참고
HnS 테스트(`test/battle/**`)는 영문 메시지를 기대값으로 쓴다. 그런데 게임 문자열은 한글이다. 그래서 테스트 전용 PR(#9265, #10590, #10310)은 ROM에 영향이 없고, 이식해도 통과 여부를 따로 확인해야 한다.

## 판정표 (TSV 순서)

고위험파일은 지시서가 정한 배틀 고위험 파일을 건드리는지(Y/N)를 뜻한다. 근거의 라인 번호는 HnS 스냅샷 기준이다.

| PR | 버전·분류 | 제목 | 판정 | 근거(파일·함수) | 고위험파일 | 용량 영향 | 의존·비고 |
|---|---|---|---|---|---|---|---|
| #10011 | 1.15.3 Battle General/Changed | Align outdated move descriptions with current mechanics | 미적용·안전 이식 가능(수동 적응) | HnS `src/data/moves_info.h`의 킹실드(15216)·페이탈클로(19369)·Stone Axe·Ceaseless Edge 설명이 옛 영문 그대로. 설명 필드는 전부 영문(899개 중 한글 0)이라 한글 훼손 없음. fwd 실패는 `.name` 문맥줄이 한글이라서임 | N | 없음 | 수동 이식(설명 4개만 교체). `B_KINGS_SHIELD_LOWER_ATK`=GEN_LATEST 확인 |
| #10016 | 1.15.3 Battle General/Changed | Align wind move flags with current mechanics | 미적용·안전 이식 가능 | HnS 모래바람(`MOVE_SANDSTORM`)에 `.windMove = TRUE`가 남아 있어 바람타기/풍력발전이 모래바람에 반응함. 1줄 삭제, fwd=ok | N | 없음 | 단독 이식 가능 |
| #9429 | 1.16.0 Battle General/Added | Add Gen 2-3 and Gen 4 Encore timers | 미적용·안전 이식 가능 | HnS `Cmd_trysetencore`(`src/battle_script_commands.c:8878-8883`)는 현재 Gen5+ 고정식(행동했으면 `B_ENCORE_TIMER`, 아니면 -1)으로 PR의 Gen5+ 분기와 같다. `B_ENCORE_TURNS`/`RNG_ENCORE_TURNS` 없음. config·generational_changes.h·random.h·battle_script_commands.c hunk는 `--check` 통과. | Y | 없음(설정 1개·분기 몇 줄) | **HnS GEN_LATEST에서 동작 차이 없음 → 실이득 0.** moves_info.h hunk는 이름이 "앙코르"로 한글화되어 컨텍스트 불일치(설명문은 영문 그대로 `src/data/moves_info.h:6149-6151`). 새 메시지·스크립트·한글 문자열 불필요. 테스트 encore.c hunk는 실패(ROM 영향 없음). |
| #9616 | 1.16.0 Battle General/Added | Gen 5+ Uproar with config | 미적용·안전 이식 가능(수동 적응) | HnS는 Gen3-4식 소란만 구현: 턴 종료 깨우기(`src/battle_end_turn.c:1223-1238` `THIRD_EVENT_BLOCK_UPROAR`), 행동 전 깨우기(`src/battle_move_resolution.c:114` `UproarWakeUpCheck`), `EFFECT_UPROAR` 존재. PR은 `B_UPROAR`(GEN_LATEST) 추가·Gen5+에서 시전 첫 턴 전원 기상(`BS_TryWakeBattlersUproar`)·지옥찌르기로 소란 종료. | Y | 작음 | **GEN_LATEST에서 실제 동작 변경(이득).** 수동 이식 필요: 설정 표는 `generational_changes.h`에 넣기, `ctx`(BattleContext) 이름으로 치환, end_turn/asm/cmd hunk는 컨텍스트만 불일치. `STRINGID_TARGETWOKEUP` 한글 문자열(`src/battle_message.c:561`, `{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n눈을 떴다!`)의 토큰만 `B_EFF`로 바꾸고 `BattleScript_TargetWokeUp`(`data/battle_scripts_1.s:5955`) `updatestatusicon BS_EFFECT_BATTLER`로 변경. 조사(`B_TXT_EUNNEUN`)는 직전 출력 글자 기준이라 토큰 교체에 안전. Gen5+에서는 `STRINGID_PKMNWOKEUPINUPROAR` 경로가 더 이상 나오지 않으므로 STATUS.md:363 기록·출력 변경 문서 갱신 필요. 테스트 3파일은 `--check` 통과. |
| #8943 | 1.16.0 Battle General/Added | 12v12 capability | 충돌 | (표본 확인) `gPlayerParty/gEnemyParty`→`gParties[MAX_BATTLE_TRAINERS][PARTY_SIZE]`(`include/pokemon.h`, `src/pokemon.c`), `partyState/itemLost/activeGimmick/AiPartyData`를 트레이너 4명 기준으로 확장, `struct Trainer` 비트필드 재배치와 trainerproc 변경. HnS가 크게 고친 `src/pokemon.c`(+2995줄), `party_menu.c`(+251), `battle_main.c`(+435), `pokemon_storage_system.c`(+495)와 정면 충돌. `battle_message.c` hunk는 한글화된 `sText_LinkTrainerSentOutPkmn`(:106)·`sText_LinkPartnerSentOutPkmn1`(:121) 등의 토큰을 바꿈. | Y | 큼 | **EWRAM 정적 +1,200B**(struct Pokemon 100B × 12, 맵 기준 `gEnemyParty` 0x02034648 ~ 0x258B), gBattleStruct/AI 힙 약 +0.5KB, ROM 수 KB 증가 추정(168파일). 선행: upcoming 131커밋(#9507 Species enum 등). `B_MULTI_HALF_TEAMS` 기본 FALSE → HnS 멀티전(`data/maps/RocketHideout_B2F_hns/scripts.inc:310` `multi_2_vs_2`)의 편성 규칙이 바뀔 수 있음. HnS 실이득 없음 → **이식 비추천**. 교차 확인: 그룹1이 충돌로 본 #9799(972bffb89f)도 같은 한글 `sText_LinkTrainerSentOutPkmn2`·`sText_LinkPartnerSentOutPkmn1GoPkmn/2GoPkmn` 계열을 고치므로 함께 처리해야 함. |
| #9168 | 1.16.0 Battle General/Added | Add Held Berry Animation | 충돌 | 스크립트 hunk가 HnS의 #9777 아이템 팝업 이식(`call BattleScript_ItemPopUp_*`)과 같은 열매 스크립트에서 겹쳐 실패한다. `src/battle_hold_effects.c`·battle_anim.c·constants는 단독 적용된다. | Y(battle_scripts_1.s, battle_hold_effects.c, battle_script_commands.c, include/battle_scripts.h, test) | 작음 | 아래 충돌 절 참조. 문자열 변경은 없다. |
| #9408 | 1.16.0 Battle General/Changed | Simplify Scale Shot effect activation | 미적용·안전 이식 가능(수동 적응) | HnS는 구 방식 그대로: `MoveEndMultihitMove`에서 `MOVE_EFFECT_SCALE_SHOT` 검사(`src/battle_move_resolution.c:2871-2872`), `BattleScript_ScaleShot`(`data/battle_scripts_1.s:2588`), `moves_info.h:18697` additionalEffect. Scale Shot 이동 부분(효과 `EFFECT_SCALE_SHOT` 신설, MoveEndMoveBlock 처리)은 `--check` 통과. | Y | 없음 | 머지 커밋 7cf1062976(첫 부모 기준). 같은 PR의 `IsBattlerTurnDamaged(gBattlerTarget)`→`IsAnyTargetTurnDamaged(gBattlerAttacker)` 치환 hunk는 HnS의 `IsBattlerTurnDamaged(x, INCLUDING/EXCLUDING_SUBSTITUTES)`(master #9487)과 API가 달라 실패. 이식한다면 1.16.0 최종형 `IsAnyTargetTurnDamaged(atk, subCheck)`로 대타 조건 보존 필요. 동작 변화는 공격자 기절 시 등 예외 상황뿐이라 **이식 가치 낮음**. 메시지 순서(MULTIHIT → MOVE_BLOCK)는 동일. (머지 커밋 `7cf1062976`, TSV엔 커밋 없음) |
| #9121 | 1.16.0 Battle General/Changed | Reduce heap usage in battle | 미적용·안전 이식 가능 | HnS `AllocateMonSpritesGfx`(1484~)가 배틀러당 4프레임을 할당하는데, 실제 사용은 `MAX_MON_PIC_FRAMES`=2다. 힙이 16KB 줄어든다. fwd ok. | N | 없음(힙 여유 +16KB) | **#9474와 반드시 짝.** 빠지면 대타출동 로드 시 다음 배틀러 버퍼/힙이 오버플로한다. HnS의 다른 spritesGfx 쓰기(battle_main.c:2894 기절 지우기 ≤1792B, battle_intro.c:648 UNUSED)는 2프레임 안이다. |
| #9388 | 1.16.0 Battle General/Changed | update poparraywithBattlers Arg to prevent warning | 미적용·안전 이식 가능 | HnS `src/battle_main.c:5369` `PopulateArrayWithBattlers(u8 *battlers)`의 호출부는 `enum BattlerId battlers[]`를 넘김(:5394, :5415). `enum BattlerId`는 packed(1바이트, `include/constants/battle.h:38`)라 경고만 난다. fwd `--check` 통과. | N | 없음 | 1줄 수정, ROM 변화 없음. |
| #9417 | 1.16.0 Battle General/Changed | Minor dancer clean up/consolidation | 미적용·안전 이식 가능(수동 적응) | HnS는 `MoveEndDancer`(`src/battle_move_resolution.c:3828-3863`)와 `ABILITYEFFECT_MOVE_END_OTHER`(`src/battle_util.c:4471-4499`)의 구 구조를 쓰며 `instructedChosenTarget`/`dancerOriginalTarget` 필드가 있다(`include/battle.h:114,127`). 여기에 master #9515 수정(`gBattleStruct->dancerSavedTarget/Attacker`)이 이미 들어 있어 upstream 부모(savedBattler 비트)와 다르다. | Y | 없음 | 순수 리팩터(추가된 동작은 `gLastUsedAbility`·RecordAbility뿐이고 HnS `AbilityBattleEffects`가 이미 `gLastUsedAbility = ability` 설정). 이식한다면 1.16.0 최종 `TryDancer`(#9515 반영형)를 참고. **이식 가치 낮음**. |
| #9514 | 1.16.0 Battle General/Changed | SetMoveEffect cleanup | 충돌 | `battle_message.c`의 약 30개 문자열이 토큰 의미를 바꿈(B_DEF→B_EFF, B_ATK→B_SCR: ALREADYASLEEP/POISONED/PARALYZED, 조이기·소용돌이 계열, FELLFORFEINT, BROKETHROUGHPROTECTION, 소금절이 등). 모두 HnS 한글 문자열이라 문자열마다 토큰만 수동으로 바꿔야 함. `battle_scripts_1.s`는 HnS 고유 변경 영역인 방벽 스크립트(`BattleScript_MoveEffectReflect/LightScreen` :684-692, `BattleScript_MoveEffectAuroraVeil` :2810 → `BattleScript_MoveEffectScreens` 통합)와 겹침. | Y | 작음 | 순수 리팩터(테스트는 ASSUME 변경뿐)라 HnS 실이득 없음. 선행: #9176 Fling Refactor, #9249, #9446 등 upcoming. battle_scripts_1.s/battle_message.c/battle_move_resolution.c/battle_script_commands.c 모두 `--check` 실패. 후속 upcoming PR(#9616의 `STRINGID_PKMNCAUSEDUPROAR` 토큰 등)의 전제. **이식 가치 낮음, 비추천**. |
| #9525 | 1.16.0 Battle General/Changed | Fill out missing type info | 미적용·안전 이식 가능(수동 적응) | 팔레트·graphics.h·items.h는 적용된다. `types_info.h`는 한글 `.name`(`"없음"` 등) 문맥 때문에 실패하지만, 필드 추가뿐이라 수동으로 넣을 수 있다. HnS `TYPE_NONE`에 zMove/maxMove가 없어 `battle_z_move.c:257`, `battle_dynamax.c:261`의 failsafe(`gTypesInfo[0]`)가 MOVE_NONE을 돌려주는 허점을 메운다. | N | 작음(+32B 팔레트) | **주의:** damageCategory 변경(Mystery SPECIAL→PHYSICAL, Stellar 미지정(0=PHYSICAL)→SPECIAL)이 HnS 챌린지 옵션 `optionStyle==1`(타입별 물리/특수, battle_util.c:9315)에서 Stellar 기술 분류를 바꾼다. |
| #9657 | 1.16.0 Battle General/Changed | BattleCalcValues usage and rename BattleContext back to DamageContext | 미적용·선행 필요 | HnS는 `struct BattleContext`를 쓰고 canceler·moveend를 `ctx` 기준으로 운용한다. `UpdateStallMons`는 공개 함수로 즉시 호출(`src/battle_util.c:10244`, `battle_move_resolution.c:2694`). presence 0/602. | Y | 작음 | 선행: #9116, #9176, #9249, #9446, #9514, #9532, #9610, #9494 등 `battle_move_resolution.c` upcoming 체인(16커밋). 순수 이름·구조 리팩터 → **이식 가치 낮음**. AI 파일 hunk는 HnS의 1.17 AI 선별 이식과도 겹칠 수 있음. |
| #9714 | 1.16.0 Battle General/Changed | Slight restructure for Defog and Tidy Up | 충돌 | HnS는 안개제거 해제 경로를 고유 스크립트로 바꿈. `TryDefogClear`(`src/battle_script_commands.c:7181-7229`)의 `DEFOG_CLEAR`가 `BattleScript_ReflectWoreOffReturn/LightScreenWoreOffReturn/MistWoreOffReturn/AuroraVeilWoreOffReturn/SafeguardEndsReturn`을 호출하고, `DefogClearHazards`(:7155)는 `BattleScript_DefogClearHazards`/`gDefogHazardsStringIds`를 쓴다. 턴 종료 방벽 만료도 전용 스크립트를 `BattleScriptExecute`로 실행(`src/battle_end_turn.c:975-1070`). PR은 이 경로의 주체를 `gBattlerAttacker`에서 `gBattleScripting.battler`로 옮긴다. | Y | 없음 | BATTLE_MESSAGE_OUTPUT_CHANGES의 방벽·흰안개·신비의부적·장판 제거 항목과 정면 충돌. 선행: #9680(End Turn BattleScriptCall화; HnS는 end2 방식), #8943(컨텍스트). `STRINGID_PKMNSUBSTITUTEFADED` 한글 문자열(`src/battle_message.c:311`, B_DEF) 토큰 변경 포함. 순수 리팩터 → **이식 비추천**. |
| #9735 | 1.16.0 Battle General/Changed | Mega Sol and Dragonize | 이미 적용 | `GetAttackerWeather()`(battle_util.c:9674), 드래곤스킨(battle_util.c:6818, battle_main.c:6186), 메가솔 회복/2턴기(battle_script_commands.c:9532~, battle_move_resolution.c:1623) 존재. 삭제 대상 31줄 모두 HnS에 없음 | Y | 없음 | 1.17.0 최종 형태(#10416 포함)로 이식됨 |
| #9786 | 1.16.0 Battle General/Changed | Clarify isFirstTurn usage with wrappers | 미적용·안전 이식 가능(수동 적응) | HnS는 `isFirstTurn` 값을 직접 비교(`src/battle_util.c:3746,6573,6578`, `battle_script_commands.c:2949` 등 21곳)하며 의미(교체 시 2, 턴 종료마다 감소; `battle_main.c:3413,5338`)는 upstream과 같다. 래퍼 2개 추가 후 치환만 하면 된다. | Y | 없음 | 순수 리팩터, 동작 변화 없음 → **이식 가치 낮음**. 여러 hunk는 BattleCalcValues/DamageContext 컨텍스트 차이로 `--check` 실패(수동 치환). battle_ai_items.c·battle_ai_util.c는 통과. |
| #9769 | 1.16.0 Battle General/Changed | Add Spicy Spray | 이미 적용 | `battle_util.c:4343` `ABILITY_SPICY_SPRAY`, AI(battle_ai_util.c:2712) 존재 | Y | 없음 | — |
| #9832 | 1.16.0 Battle General/Changed | Clean up for eject item changes | 미적용·선행 필요 | HnS `MoveEndCardButton`(`src/battle_move_resolution.c:3362-`)은 master식(속도 정렬 + 비트마스크)이고 `redCardActivated`가 없다. PR은 #9784가 만든 구조를 정리하는 후속이다. | Y | 없음 | 선행: #9784(Adjustments for Eject Items / Mirror Herb / White Herb). Acrobatics hunk는 HnS에 동등 코드가 이미 있음(`src/battle_util.c:6510` `ctx->holdEffectAtk == HOLD_EFFECT_GEMS`). |
| #9898 | 1.16.0 Battle General/Changed | IsAnyTargetAffected and GetDynamicMoveType changes | 미적용·안전 이식 가능(수동 적응) | `IsAnyTargetAffected`(`src/battle_util.c:10946-10966`)는 HnS가 구 로직이고 `--check` 통과. `TARGET_ALL_BATTLERS`/`TARGET_USER_AND_ALLY` 판정을 고쳐 목구멍스프레이(`battle_hold_effects.c:498`)·파이널찬스(`battle_move_resolution.c:1508`) 등의 판정이 정확해진다. `GetDynamicMoveType`의 `GetBattlerTypes` 사용은 HnS 함수 시그니처가 달라(`src/battle_main.c:6202-6225`) 수동 이식. | Y | 없음 | 부분 의존: `MoveEndOpportunist`의 `gBattlersByRawSpeed` 순서 hunk는 #9784가 선행되어야 함(HnS에 해당 배열 없음). **IsAnyTargetAffected 부분 이식 추천.** |
| #9928 | 1.16.0 Battle General/Changed | Rename follow up for Stat Change Refactor | 미적용·선행 필요 | `tryanystatchange`/`trynonmovestatchange` 매크로·opcode가 HnS `asm/macros/battle_script.inc`에 없음(0건). PR은 #9730 결과물을 이름만 바꾼다. | Y | 없음 | 선행: #9730 Stat Change Refactor(`battle_stat_change.c`). |
| #9916 | 1.16.0 Battle General/Changed | Minor Battle Struct clean up and script clean up | 미적용·안전 이식 가능(수동 적응) | HnS에는 `friskedBattler/friskedAbility/quickClawBattlerId`(`include/battle.h:621,650-651`), `BattleScript_FriskMsgWithPopup`(`data/battle_scripts_1.s:6979`), `BattleScript_NoItemSteal`(:7126)이 남아 있다. 변경은 구조체 정리, 미사용 명령(critcalc/pursuitdoubles; HnS 스크립트 사용 0건) 제거, 특성 Frisk 흐름 정리(대상 도구가 없으면 발동하지 않음). | Y | 없음(소폭 감소) | 수동 이식 필요: `STRINGID_FRISKACTIVATES` 한글 문자열(`src/battle_message.c:625`)의 토큰을 B_ATK→B_EFF, B_DEF→B_SCR로 교체. Quick Claw 루프 카운터를 `gBattleScripting.battler`로 바꾸므로 HnS 아이템 팝업 helper가 이 변수를 쓰지 않는지 확인(현재 Quick Claw 팝업은 `BS_ATTACKER`, :7672). `stellarBoostFlags[MAX_BATTLE_TRAINERS]`는 HnS에 상수·`GetBattlerTrainer`가 있어 컴파일은 되나 #8943 없이는 의미가 적음. **이식 가치 낮음**. 교차 확인: 스티키홀드 경로(`BattleScript_NoItemSteal`→`BattleScript_StickyHoldActivatesRet`)가 쓰는 `STRINGID_PKMNSXMADEYINEFFECTIVE`(HnS :489 「{B_DEF}에게는 효과가 없는 것 같다...」)는 그룹1 충돌 #10149(Y/IT INEFFECTIVE 재정의)와 같은 STRINGID 묶음이다. HnS는 `PKMNSXMADEITINEFFECTIVE`를 스위트베일 「잠들지 않는다!」(:522)로 따로 쓰고, upstream은 #9655에서 Y 문자열을 "item cannot be removed"로 바꿨으므로 한글 의미를 확인한 뒤 이식. |
| #9939 | 1.16.0 Battle General/Changed | Integrate accuracy check into canceler | 충돌 | 빗나감 메시지 체계를 재편한다. `STRINGID_PKMNEVADEDATTACK`→`STRINGID_PKMNAVOIDEDATTACK` 개명, `STRINGID_BATTLERAVOIDEDATTACK` 신설, `gMissStringIds` 삭제, `accuracycheck` 인자 제거. HnS는 한글 의미를 따로 나눠 둠: `:285` EVADED=「…는 공격을 피했다!」, `:521` AVOIDED=「…에게는 맞지 않았다!」, `gMissStringIds` MISSED=`STRINGID_ATTACKMISSED`「그러나 …의 공격은 빗나갔다!」(`src/battle_message.c:1025-1029`), 거머리씨 빗나감 `:1114`. | Y | 작음 | 선행: #9655(Refactor Battle Messages; upstream은 여기서 빗나감을 AVOIDED로 바꿈), #9657, #9514 등 canceler 체인(38커밋). HnS `accuracycheck <fail>` 42곳 이상과 HnS 고유 스크립트(`BattleScript_BideAttack`의 `accuracycheck BattleScript_MoveMissed`) 전환 필요. 동작 이득(범위기 빗나감 메시지 순서, 떼쓰기+면역 특성)은 있으나 **선행 체인이 커서 단독 이식 불가**. 교차 확인: `BattleScript_TookAttack`이 쓰는 `STRINGID_ITDOESNTAFFECTSCR`는 HnS에 없음(#9655 산물). 그룹1 충돌 #10144(효과 메시지 순서, `STRINGID_ITDOESNTAFFECTTWOFOES` 삭제; HnS는 Champions 이식으로 TWOFOES 계열 사용)과 같은 "빗나감/효과 없음" 메시지 묶음이라 함께 판단해야 함. |
| #9989 | 1.16.0 Battle General/Changed | Small Dome updates | 미적용·안전 이식 가능(수동 적응) | HnS `IsDomeDefensiveMoveEffect(enum BattleMoveEffects effect)`는 효과값을 switch하는 올바른 형태다(`src/battle_dome.c:3892`, 호출 :4335 `effect`). PR이 고치는 `switch (move)` 버그는 #9730이 만든 것이라 HnS에는 없다. 새로 의미 있는 부분은 `IsDomeRiskyMove`에 `IsExplosionMove` 추가(:3923-3935)뿐이다. | N | 없음 | HnS에 배틀프런티어 `_hns` 맵(BattleDome 포함)이 있어 사용 경로는 있음. 이식은 폭발 계열 risky 판정 2~3줄 수동 추가만 권장. 선택 사항(카드 표시용). `switch(move)` 수정은 upcoming 리팩터(#9730) 회귀 수정이라 HnS엔 해당 버그 없음. |
| #10057 | 1.16.0 Battle General/Changed | Further stat change refactor clean up | 미적용·선행 필요 | `statbuffchange` 제거, `STAT_CHANGE_BY_MIRROR_ARMOR` 개명, `battle_stat_change.c` 주석. HnS는 `Cmd_statbuffchange`가 현역이고(`src/battle_script_commands.c:721,8076`) `battle_stat_change.c`가 없다. AI hunk 대상(`AI_GetAdjustedStatStage`, `GetFoeStatChangeScore`)도 #9730 산물로 HnS에 없다. | Y | 없음 | 선행: #9730, #9928. AI hunk 2개(`AI_GetAdjustedStatStage` 날씨 인자 오용, `GetFoeStatChangeScore`의 gCurrentMove→move)는 upcoming 리팩터 회귀 수정이라 HnS엔 해당 코드·버그 없음. |
| #9063 | 1.16.0 Moves/Changed | Made moves' animation's AbsorbEffects more consistent | 미적용·안전 이식 가능 | HnS `gBattleAnimMove_DrainPunch`(1751~1763)는 아직 `MegaDrainAbsorbEffect`, DrainingKiss는 `AbsorbEffect`다. `GigaDrainAbsorbEffect`(26551)와 `B_UPDATED_MOVE_DATA`가 있어 `git apply --check`가 그대로 통과한다. | N | 없음 | 연출 차이만 있음. |
| #9142 | 1.16.0 Moves/Changed | Create functions for repeated move animations | 미적용·안전 이식 가능 | fwd ok(3개 hunk 모두 적용)이고, 사용 상수(`SHAKE_BG_Y`, `ANIM_SURF_PAL_*`, `gMagicPowderBluePowderTemplate`)가 HnS에 모두 있다. 순수 서브루틴화 리팩터라 기능 변화는 없다(일부 효과음 순서만 이동). | N | 작음(ROM 감소 추정) | **#9473 + #9564 필수 동반.** #9142가 PsychicFangsCommon→CreateCrunch1 중첩 call(HnS 단일 반환주소로는 오동작)과 Stuff Cheeks x=33 버그를 만든다. 단독 이득은 없고 이후 upstream 애니 패치 적용성만 조금 좋아진다(#8497/#9172 없이는 여전히 대부분 실패). |
| #9473 | 1.16.0 Moves/Changed | Move animation call depth increase | 미적용·안전 이식 가능 | HnS `src/battle_anim.c:101,1466,1472`는 `sBattleAnimScriptRetAddr` 단일 포인터다. DefendOrder→`BideSetUp`, SaltCure→`gBattleAnimGeneral_SaltCureDamage`처럼 call한 서브루틴이 `end`로 끝나는 곳은 HnS 전체에서 이 2곳뿐이고 PR이 둘 다 goto로 바꾼다. fwd ok. | Y(include/battle_anim.h) | 작음(EWRAM +약 13B) | #9142 이식 시 필수. 단독으로도 안전한 견고성 개선이다. `Cmd_return`의 `RetAddr[depth]=0`은 한 칸 어긋나 있지만 무해하다(upstream 1.17.0도 동일). |
| #9172 | 1.16.0 Moves/Changed | Converts move animation hex numbers to decimal | 미적용·선행 필요 | 4399줄 기계 치환이다. `data/battle_anim_scripts.s:701` 첫 hunk부터 실패하는데, 원인은 #8497(loadspritegfx 제거) 문맥이다. | N | 없음 | 선행: #8497(+#9142/#9473/#9564). 기능 변화가 없어 이득이 없고 충돌 위험이 매우 크다. **비권장.** |
| #9676 | 1.16.0 Moves/Changed | Created macros for fist and foot animations | 미적용·선행 필요 | 매크로(.inc)는 적용되지만 스크립트 BulletPunch 등에서 실패한다. 10진수·`SOUND_PAN_*` 문맥 때문이다. | N | 없음 | 선행: #8497, #9172, #9677. 순수 리팩터라 **비권장.** |
| #9474 | 1.16.0 Moves/Fixed | Fix Substitute breaking when used by opponentRight in double battles | HNS와 무관 | upcoming 리팩터(#9121) 회귀 수정이라 HnS에는 해당 버그가 없다. fwd ok. HnS `BattleLoadSubstituteOrMonSpriteGfx`(1106~1109)는 프레임 1~3을 복사하지만 HnS 버퍼가 배틀러당 `MON_PIC_SIZE*4`(1491)라 **현재 HnS에는 버그가 없다**. | N | 없음 | #9121을 이식할 때는 반드시 함께 넣는다(순차 적용 확인). 단독으로 넣어도 무해하다. |
| #9564 | 1.16.0 Moves/Fixed | Fixed Stuff Cheeks animation | HNS와 무관 | upcoming 리팩터(#9142) 회귀 수정이라 HnS에는 해당 버그가 없다. HnS `BiteOpponent`(13397~13401)는 `x=0xffDF`(s16 = -33)로 이미 올바르다. x=33 버그는 #9142의 10진수 변환 과정에서 생겼다. | N | 없음 | #9142를 이식할 때만 함께 필요하다(스크래치 체인 적용 확인). |
| #9595 | 1.16.0 Moves/Fixed | Fix move anim pal blending being discarded | HNS와 무관 | upcoming 리팩터(#8497) 회귀 수정이라 HnS에는 해당 버그가 없다(결과적으로 수정 후와 동등). PR은 #8497이 `Cmd_waitforvisualfinish`에 넣은 `UnloadAllSpritePalettes()`를 삭제하는데, HnS `src/battle_anim.c:854~865`에는 처음부터 없다(rev ok). | N | 없음 | 이식 기록이 아니다. #8497을 나중에 이식하면 함께 필요하다. |
| #9511 | 1.16.0 Moves/Fixed | Fix move anim migration script | HNS와 무관 | 수정 대상이 `migration_scripts/1.16/remove_loadspritegfx.py` 하나뿐이다. HnS에는 `migration_scripts/1.16`이 없고 #8497도 없다. | N | 없음 | 마이그레이션 스크립트 전용. |
| #9678 | 1.16.0 Abilities/Changed | Add basic Z-A Mega ability data (from Champions Regulation M-A) | 이미 적용 | 특성 ID 311~318이 1.17.0 최종 배치와 동일(`include/constants/abilities.h:335-342`), 메가 종 특성(메가피클 매직미러, 메가스타미 공격 100·천하장사 등) 반영. 메가장크로다일만 `{DRAGONIZE, NONE, DRAGONIZE}`로 표기 차이(기능 동일) | N | 없음 | 특성명은 한글(관통드릴 등), `ABILITY_AURA_GUARD`(319)는 HnS 고유 |
| #9740 | 1.16.0 Abilities/Changed | ABILITY_PIERCING_DRILL | 이미 적용 | `battle_util.c:5991`, `battle_move_resolution.c:2125`, `battle_ai_util.c:5519`(upstream 후속 수정된 `&&` 형태)에 관통드릴 처리 존재 | Y | 없음 | — |
| #10129 | 1.16.1 Battle General/Changed | feat (battle): improve gimmick triggers' graphics | 미적용·안전 이식 가능(수동 적응) | HnS의 트리거 PNG 5개와 mega_trigger.pal은 upstream PR 직전과 동일(blob 일치)하고 크기도 32x64 그대로다. 다만 HnS가 `src/battle_gimmick.c`를 `struct GimmickTriggerPosition` + Gen4 UI 메가 전용 위치로 재구성했고 `gimmicks.h`는 INCBIN 방식이라 C hunk가 실패한다. | N | 없음 | **수동:** `--binary`로 PNG 교체, mega_trigger.pal 삭제(Makefile `%.gbapal: %.png` 규칙이 받음), `sSinglesGimmickTriggerPosition`/`sDoublesGimmickTriggerPosition`의 yDiff를 -11→-5, -4→-2로 변경. Gen4 메가 트리거(`gen4/mega_trigger`)는 건드리지 않는다. 불확실 목록 참조. |
| #10104 | 1.16.1 Items/Changed | Add missing item sort types | 미적용·안전 이식 가능 | Ability Shield와 Choice Dumpling/Swap Snack/Twice-Spiced Radish에 `.sortType`을 추가한다. hunk 4개가 모두 정확한 항목에 적용된다(offset +236, `src/data/items.h:14905/16016/16030/16045`). 한글 `.name` 줄은 건드리지 않는다. | N | 없음 | 가방 정렬(`item_menu.c:3102`)에만 영향. |
| #10109 | 1.16.1 Items/Changed | SanitizeBagItemId, TryTakeMonItemResult | 미적용·안전 이식 가능(수동 적응) | `src/item.c`는 그대로 적용된다. HnS에서 `gItemsInfo[ITEM_NONE].pocket`=POCKET_ITEMS라 `AddBagItem(ITEM_NONE)`이 통과하는 허점이 있는데, 이 PR이 막는다. party_menu.c는 HnS가 구 시그니처 `TryItemHoldFormChange(&gPlayerParty[...], slot)`(2087)를 써서 문맥이 실패한다. | N | 없음 | party_menu 부분은 enum 이름만 바꾸는 것이라 수동으로 넣거나 생략해도 된다. |
| #10106 | 1.16.1 Items/Fixed | Use values from `enum PokeBall` for Poké Balls | 미적용·안전 이식 가능(수동 적응) | 타입만 정리한 PR이다. `ITEM_POKE_BALL`=`BALL_POKE`=1, `ITEM_LUXURY_BALL`=`BALL_LUXURY`=14로 값이 같아 동작은 바뀌지 않는다. battle_script_commands.c `ComputeBallData` 끝의 `default: break;` hunk만 HnS 고유 `BALL_GS`/`IS_HNS` 분기(10696~10916) 때문에 실패한다. | Y(battle_script_commands.c, include/battle_anim.h) | 없음 | **이득 없음.** 넣는다면 1 hunk만 수동 처리한다. |
| #10189 | 1.16.2 Moves/Changed | Fix Twister using Secret Power's animation | 이미 적용 | merge-base `3efb836f72`에도 `end`가 없던 실제 버그다. HnS `gBattleAnimMove_SecretPower`(30365~30367)에 `end`가 이미 있다. HnS 자체 커밋 `977fa3bae1 "Twister/secret power battle anim fix"`(dylanfalzone1, 2026-08-11)로 들어갔다. | N | 없음 | — |
| #10262 | 1.16.2 Moves/Fixed | Fix Trick Room animation playing on wrong battler | 미적용·안전 이식 가능 | merge-base 버전부터 있던 버그다(upcoming 회귀 아님). HnS `InitRoomAnimation`(2519~2522)은 `AnimTask_ScaleMonAndRestore ... ANIM_TARGET`이라 버그가 그대로 있다. 이 서브루틴은 TrickRoom·WonderRoom·MagicRoom(2509, 4146, 4328)이 공용으로 쓴다. fwd ok. | N | 없음 | 1줄 수정. |
| #10345 | 1.16.2 Moves/Fixed | fix(battle-anim): fix gust colour cycling animation | HNS와 무관 | upcoming 리팩터(#8497) 회귀 수정이라 HnS에는 해당 버그가 없다. HnS `AnimTask_AnimateGustTornadoPalette_Step`(src/battle_anim_flying.c:370~390)의 회전 로직은 새 memmove 코드와 동작이 같다. Gust 태그를 쓰는 HnS 애니 7곳은 모두 `loadspritegfx ANIM_TAG_GUST`로 팔레트를 먼저 로드하므로 캐시한 인덱스가 유효하다. | N | 없음 | #8497의 지연 로드 회귀를 고친 PR이다. fwd ok라 넣어도 무해하지만 방어적 리팩터일 뿐이다. |
| #10225 | 1.16.2 Items/Changed | Remove the preproc around Strange Ball OW graphic | 미적용·안전 이식 가능(수동 적응) | `ITEM_STRANGE_BALL`은 enum이라 `#ifdef`가 항상 거짓이다. HnS(`OW_FOLLOWERS_POKEBALLS TRUE`)에서 Strange Ball 동료 그래픽이 빠져 `ObjectEventSetPokeballGfx`(event_object_movement.c:8132~)가 몬스터볼로 대체한다. pic_tables·followers·event_object_movement hunk는 적용되고, `object_event_graphics.h`만 INCBIN 문맥이라 수동이다. | N | 작음(+약 1.9KB, 112x32 4bpp) | `spritesheet_rules.mk:5907` 패턴 규칙이 ball_strange.4bpp를 처리한다. HnS 고유 `BALL_GS`도 동료 그래픽이 없어 몬스터볼로 표시된다(별건). |
| #10395 | 1.16.3 Battle General/Changed | Fix Orichalcum Pulse and Hadron Engine Activation Messages | 미적용·안전 이식 가능(수동 적응, 사용자 확인 필요) | HnS는 `ABILITY_ORICHALCUM_PULSE`를 가뭄과 같은 case로 처리(battle_util.c:3388)하고 하드론엔진도 일반 필드 메시지. upstream 선행 형태와 동일해 구조상 이식 가능 | Y | 작음 | 새 STRINGID 4개·스크립트 4개 필요 → **공식 한국어 문구 확인 전에는 미결**. 특성 발동 메시지 출력이 바뀌므로 BATTLE_MESSAGE 정책상 사용자 확인 후 진행. `GetWeather()`→HnS `gBattleWeather & … && HasWeatherEffect()` 적응 |
| #10428 | 1.16.3 Battle General/Changed | Fix outdated move data | 미적용·안전 이식 가능(브레이브차지 스탯 hunk 제외) | HnS 파찌파찌액셀이 `.alwaysCriticalHit = TRUE` + 회피율 상승을 동시에 가져 Gen8+ 기준 버그(moves_info.h `MOVE_ZIPPY_ZAP`). 초승달의기도/브레이브차지 PP, 대격분 명중 조건식도 미적용 | N | 없음 | 브레이브차지 `.attack→.spDef` hunk는 **이식 불필요**: HnS는 `EFFECT_TAKE_HEART` 스크립트(battle_scripts_1.s:380)로 특공·특방을 이미 올림(버그는 upstream 스탯 리팩터 산물). 이름 문맥이 한글이라 수동 이식 |
| #10423 | 1.16.3 Items/Fixed | Fix bag list truncation when tossing a whole stack in berry pocket | 이미 적용 | HnS `src/item_menu.c:3038~3043` `MergeSort`가 이미 수정본(`usedCapacity = i + 1`)이다. 커밋 `6b673c46d8`(PR 원 브랜치 커밋)이 HnS HEAD의 조상이다. rev ok. | N | 없음 | — |
| #10590 | 1.16.4 Battle General/Changed | Change Howl Soundproof test to prevent user from blocking with Soundproof | HNS와 무관(테스트 전용) | `test/battle/move_effect/attack_up_user_ally.c`만 변경. HnS 테스트는 merge-base의 옛 기대 문구("Voltorb's Soundproof blocks Howl!")라 선행 테스트 갱신 없이 적용 불가 | Y(test) | 없음 | ROM 영향 없음 |
| #10666 | 1.16.4 Battle General/Changed | Added clarification for BattleScript_IntimidateActivates | HNS와 무관 | 주석 2줄만 바꾼다. 대상 코드(`intimidateActivated` 플래그, `SetStatChange`)는 #9730 이후 형태이고, HnS는 `SET_STATCHANGER(STAT_ATK, 1, TRUE)` 방식이다(`src/battle_util.c:3457-3465`). | N | 없음 | ROM 영향 없음. |
| #10589 | 1.16.4 Moves/Fixed | Fix out-of-bounds palette writes in certain move animations | HNS와 무관 | upcoming 리팩터(#8497) 회귀 수정이라 HnS에는 해당 버그가 없다. MagicalLeaf(30415~), NightSlash(1475~), AuroraBeam(26124~)은 HnS에서 모두 `loadspritegfx`로 해당 팔레트를 먼저 로드한다. PR이 쓰는 `TryLoadPal()`은 #8497에만 있다. | N | 작음 | 선택: `src/util.c` `BlendPalette` 경계 `assertf` 가드만 단독 이식할 수 있다(`assertf`는 global.h에 있음). OBJ 팔레트 16슬롯이 가득 찬 극단 상황에 대한 방어다. |
| #9688 | 1.17.0 Battle General/Added | Add B_TERA_ORB_ALWAYS_CHARGED | 미적용·안전 이식 가능 | `IsTeraOrbCharged()` 도입 + config. HnS battle_terastal.c/script_pokemon_util.c는 merge-base 그대로. 실패 원인은 `include/config/battle.h` `B_SLEEP_CLAUSE` 주석 문맥 차이뿐 | Y(battle_terastal.h) | 없음 | 기본 FALSE, HnS `B_FLAG_TERA_ORB_CHARGED`=0이라 동작 변화 없음 → 이식 가치 낮음 |
| #9008 | 1.17.0 Battle General/Added | Adds Victory Catch | HNS와 무관(기본 비활성 config) | 새 config `B_FLAG_VICTORY_CATCH_RANDOM/GUARANTEED`의 기본값이 `0`이다. 추가로 `IsVictoryCatch()`가 참이 되는 경우는 `BATTLE_TYPE_RAID`뿐인데, HnS는 이 값을 정의만 하고 레이드를 쓰지 않는다(`battle_util.c:3819`의 검사 1곳). 따라서 이식해도 동작 변화가 없다. HnS에는 관련 심볼이 전혀 없다(`VICTORY_CATCH`·`B_CATCH_OR_NOT` 0건). | Y (battle_message.c, battle_scripts_1/2.s, battle_script_commands.c, battle_util.c, battle_string_ids.h, battle.h) | 작음(이식 시 코드·문자열 약 2~3KB. `victoryCatchState:2`는 HnS `BattleStruct`의 `battle.h:702-705`에 padding이 없어 새 비트가 필요하다. 힙 할당이라 정적 EWRAM 영향은 없다) | 기능을 쓰려면 수동 이식이 가능하다(선행 PR 없음). 이때 한글 신규 문자열 3개가 필요하다: `STRINGID_VICTORYCATCH`, `gText_BattleCatchOrNot`(8타일 창 `B_CATCH_OR_NOT`=26), `sText_BeCarefulPkmn`(레이드 전용이라 사실상 미사용). STRINGID 교차 확인: `STRINGID_VICTORYCATCH`를 **추가만** 하고 개명·삭제는 없다. HnS 열거 끝(`battle_string_ids.h:733 STRINGID_PKMNCANNOTSLEEP`) 뒤에 붙이면 되고, 그룹1 충돌 PR(#10144의 `STRINGID_ITDOESNTAFFECTTWOFOES` 삭제 등)과 겹치지 않는다. `FAINT_BLOCK_*` 열거는 HnS에서 `constants/battle_script_commands.h:389`에 있다(upstream은 battle_move_resolution.h). `Cmd_handleballthrow`→`SetBallThrowShakes` 분리는 동작 변화가 없고, HnS의 포획 커스텀(`ComputeBallData`·`ComputeCaptureOdds`)과도 겹치지 않는다. |
| #9777 | 1.17.0 Battle General/Added | Battle Messages from Pokémon Champions | 부분 적용 | HnS는 새 STRINGID 11개(2026-09-19)와 효과 단계 메시지·아이템 팝업(2026-09-20)을 HNS식으로 이식. 미반영: `multihitplurality`(동적 복수형), Unseen Fist 보호 관통 메시지(`MOVEEND_PROTECT_BYPASS_EFFECTS`, `BattleScript_UnseenFist`), `STRINGID_AFTERMATHDMG→PKMNWASHURT` 개명, 기존 문구 38개 영문 개정, `FreeAbilityPopUpGfx`, battle_tv 신규 ID 처리 | Y | 작음 | **충돌 절 참조**. 그룹1 충돌 #10144가 `STRINGID_ITDOESNTAFFECTTWOFOES`를 삭제하고 상성 메시지 순서를 바꾸므로 #9777 잔여분(효과 단계·TWOFOES 선택)과 함께 판단해야 함 |
| #10151 | 1.17.0 Battle General/Added | Add Champions battle mechanics changes (v1.0.X) | 부분 적용 | HnS는 Champions 수면 턴(battle_script_commands.c:2358, battle_end_turn.c:902)만 이식. 신규 config 14개(`B_PARALYSIS_CHANCE`, `B_FREEZE_TURNS`, `B_MEGA_EVO_SPEED_SWAP`, `B_FIRST_TURN_MOVE`, `B_SALT_CURE_DAMAGE`, `B_BELCH/STUFF_CHEEKS/SPIT_UP/LAST_RESORT_SELECTABLE`, `B_MOVES_THAT_REMOVE_TYPE`, `B_ENCORE_PRIORITY`, `B_FAINT_MOVE_EFFECT_TIMING`, `B_SHEER_FORCE_AGAINST_ABILITIES`, `B_UNSEEN_FIST_PIERCING_DRILL`)와 `freezeTurns`, `VOLATILE_SPEED_SWAP`, `STRINGID_CANTUSEMOVE/BELCHCANTUSE` 전부 없음 | Y | 작음 | **충돌 절 참조**. HnS `GEN_LATEST=GEN_CHAMPIONS`라 나머지 이식 시 기본값으로 마비 12.5%·동결 상한 등 게임 밸런스가 바뀜 → 사용자 결정 필요. 캔슬러 시그니처가 HnS `struct BattleContext *ctx`라 수동 적응 필요 |
| #10416 | 1.17.0 Battle General/Added | Add Ability Pop-Ups to Mega Sol | 이미 적용 | `BattleScript_MegaSolActivatesHealing`/`...TwoTurnMove`(battle_scripts_1.s:3347/3354) 존재. HnS는 `BattleScript_HealTargetContinue` 대신 기존 `BattleScript_PresentHealTarget`에 연결(SESSION_LOG 2026-09-15) | Y | 없음 | — |
| #10025 | 1.17.0 Battle General/Added | Implement opponent HP percentage display | 부분 적용 | `PrintHpPercentageOnHealthbox()`와 분기(battle_interface.c:1021/1157/1190/2255), `B_HP_PERCENTAGE_DISPLAY FALSE` 존재. 퍼센트 전용 체력박스 PNG 3종·파일명 변경·graphics.c/gfx_sfx_util의 64×64 OAM 전환은 미이식 | N | 작음(자산 켜면) | HnS 체력박스는 HnS/Soulgold 전용 자산이라 upstream 자산 교체는 충돌 성격. 기본 FALSE 유지면 추가 작업 불필요 |
| #10170 | 1.17.0 Battle General/Changed | Starting Status for Weather | 미적용·안전 이식 가능(수동 적응) | HnS `TryFieldEffects`(`battle_util.c:2668`, `isTerrain` 2671~2960)와 `STARTING_STATUS_*` 46종(`constants/battle.h:771-817`)은 PR 직전과 같다. battle_util.c·battle_scripts_1.s·battle_scripts.h는 `apply --check`를 통과하고, constants/battle.h는 주석 공백 hunk만 실패한다. 새 문자열은 없다(기존 `gMoveWeatherChangeStringIds` 한글 재사용). | Y | 작음(`StartingStatuses` 46→58비트로 여전히 8바이트) | **이 PR 단독으로는 회귀가 있다.** 필드·사이드 시작상태(트릭룸·순풍 등)의 메시지가 출력되지 않는데, 1.17.0의 **#10214**가 이를 고친다. 반드시 함께 이식하거나 1.17.0 최종본 기준으로 옮길 것. HnS 트레이너 데이터는 시작상태를 쓰지 않는다(`trainers_hns.party` 0건, `scrcmd.c:4184`에서만 설정 가능). 실익은 낮다. |
| #10145 | 1.17.0 Battle General/Changed | Improve AI thinking time by removing direct ability calls in calc dmg | 부분 적용 | HnS는 HNS식으로 이식돼 있다: `GetDamageCalcAbility`/`IsDamageCalcAbilityOnField`(`battle_util.c:6153-6171`), 3인자 `GetBattlerWeight`(`battle_util.c:6117`, 호출부 모두 반영), `CalcDynamicMoveDamage(ctx, simDamage)`(`battle_ai_util.c:758`). Supreme Overlord 카운터는 `gBattleStruct`에 그대로 두고 AI 가상 교체 때 저장·복원한다(`battle_ai_switch.c:65-92`). | Y | 없음 | 미반영분은 세 가지다. ① 카운터의 volatile 이동: HNS식 대체로 동등. ② 오라 특성 `RecordAbilityBattle`: `battle_util.c:6846` 쪽에 없고 영향이 미미하다. ③ AI 교체 시뮬레이션의 `aiHoldEffect`·Booster Energy 부분: HnS에 선행 #9124(`SetBattlerStatStagesForSwitchin` 등)가 없어 해당 없음. 문서: STATUS.md 1493·1497, SESSION_LOG.md 1937·1952. 추가 이식은 불필요하다. |
| #10249 | 1.17.0 Battle General/Changed | Add Fire Mane | 이미 적용 | `battle_util.c:7206` 불꽃 기술 1.5배, 메가화염레오 특성 연결 | Y | 없음 | — |
| #10256 | 1.17.0 Battle General/Changed | Add Eelevate | 이미 적용 | 부유·비스트부스트 겸용 처리(battle_util.c:4581/4589/6078/8513/8606/9945), AI·스위치·돔 반영 | Y | 없음 | upstream `ctx->abilities[]` 대신 HnS `ctx->abilityDef` 형태 |
| #10253 | 1.17.0 Battle General/Changed | Remove redundant Sub checks | 미적용·안전 이식 가능(수동 적응) | HnS에 중복 `!DoesSubstituteBlockMove` 검사가 그대로 있다: `battle_hold_effects.c:349/371/392/518`(자보카·로플·에니그마·스탯열매), `battle_move_resolution.c:3073`(`EFFECT_KNOCK_OFF`), `battle_util.c:4524`(Magician). 모두 `IsBattlerTurnDamaged(…, EXCLUDING_SUBSTITUTES)`와 함께 쓰이고, 이 함수는 `battle.h:1085`에서 대타를 이미 제외한다. | Y | 없음(소폭 감소) | 동작 동일. battle_util.c hunk만 그대로 적용되고, 나머지는 문맥(에니그마의 `IsBattlerAlive`, 전역 `gBattlerAttacker`)이 달라 수동 적응이 필요하다. 실익은 낮다. |
| #9265 | 1.17.0 Battle General/Changed | New tests: Eviolite, Charge, Grassy Terrain & Misty Terrain | 미적용·안전 이식 가능(테스트 전용) | 테스트 3개 추가만, fwd=ok. SESSION_LOG에 "테스트라 미이식" 기록 | Y(test) | 없음 | ROM 영향 없음. HnS 테스트는 영문 메시지 기준이라 실제 통과 여부 별도 |
| #10310 | 1.17.0 Battle General/Changed | Tests for Victory Star not being ignored by Ally | HNS와 무관(테스트 + 공백 정리) | src 변경은 `TrySetCantSelectMoveBattleScript`/`DoesBattlerKOItselfWithRecoil` 등의 후행 공백 제거뿐(해당 Champions 선택 제한 코드는 HnS에 없음). 나머지는 victory_star.c 테스트 | Y | 없음 | 승리의별 아군 보정은 HnS `battle_util.c:10695` `cv->abilities[atkAlly]` 사용 — 테스트 이식 시 틀깨기 무시 여부만 확인 |
| #10297 | 1.17.0 Battle General/Changed | New Knock Off cases | 미적용·안전 이식 가능 | HnS `CanBattlerGetOrLoseItem()`(battle_util.c 약 9048)에 상대 종의 폼 변경 아이템 조건 없음. 1줄 조건 + config 주석. src hunk는 오프셋만 다르고 적용됨 | Y | 없음 | `B_KNOCK_OFF_REMOVAL`=GEN_LATEST(=Champions)라 HnS 기본 설정에선 추가 조건이 비활성 → 실효 없음(저가치) |
| #10318 | 1.17.0 Battle General/Changed | Remove redundant Magnet Rise / Laser Focus flags | 미적용·안전 이식 가능(수동 적응) | HnS 참조 20곳이 upstream hunk와 1:1 대응한다: `constants/battle.h:227,230`, `battle_scripts_1.s:926,2028,2149`, `Cmd_trysetvolatile` switch(`battle_script_commands.c:10218-10222`), `BS_GravityOnAirborneMons`(13838), `battle_end_turn.c:69-70,798`, `battle_move_resolution.c:3178`, `battle_util.c:6074,8141,8219`, AI 4곳, debug 2곳. `setvolatile`의 value 인자도 HnS 매크로에 있다(`battle_script.inc:916`). | Y (battle_scripts_1.s, battle_script_commands.c, battle_end_turn.c, battle_util.c) | 없음 | **동작 수정을 포함한다(이식 추천).** HnS는 `magnetRise` 플래그만 바톤터치로 넘기고 `magnetRiseTimer`는 넘기지 않는다(`battle_main.c:3403-3404`는 embargo·healBlock 타이머만 복사). 그래서 바톤터치로 전자부유를 받은 포켓몬은 타이머가 0이 되어 `HandleEndTurnMagnetRise`가 끝내 해제하지 못하고 영구 부유한다(코드 추론). PR은 타이머를 `V_BATON_PASSABLE`로 바꿔 이를 고친다. 메시지 변화는 없다. |
| #10257 | 1.17.0 Battle General/Changed | Add Champions battle mechanics changes (v1.1.0) | 이미 적용 | `B_RAGE_FIST`(config/battle.h:166) 및 교체·기절 시 `timesGotHit=0`(battle_main.c:3356, 3486), 골드러시 Champions 특공 -2(moves_info.h `MOVE_MAKE_IT_RAIN`) 존재 | N | 없음 | 테스트(rage_fist.c)만 미반영 |
| #10338 | 1.17.0 Battle General/Changed | Consolidate switch in and faint clear/set data | 미적용·안전 이식 가능(수동 적응) | HnS `SwitchInClearSetData`(`battle_main.c:3329`)와 `FaintClearSetData`(3479, `const u8*` 반환·`keepGastroAcid`·Champions Rage Fist 추가 유지)는 PR 직전과 구조가 다르다. upstream의 `gSpecialStatuses.queuedSwitch`(#9494), `notOnField`(#9864, HnS는 `fainted`), void 반환(#9249)은 HnS에 없다. 해당 줄을 빼고 `ClearSetDataOnLeave`를 HnS 필드로 재구성하면 된다. | N | 없음(소폭 감소) | 동작 변화가 있는 부분이 있다. 교체 등장 시 `quash`·`helpingHand`·`bounceMove`·`chargingTurn`·`fleeType`·`statRaised`·`pranksterElevated`·`moldBreakerActive`도 초기화된다. 기절 시에는 `stompingTantrumTimer`·`canPickupItem`·`wasAboveHalfHp`·AI eject 플래그가 초기화된다. 교체 등장 시 `moveResultFlags[battler]=0`은 제거된다(불확실 목록 참조). 실익은 낮다. |
| #10324 | 1.17.0 Battle General/Changed | Add palette blend to Mega Evolution and Primal Reversion particle | 이미 적용 | HnS `data/battle_anim_scripts.s:31534, 31761, 31795`에 `AnimTask_BlendParticle ... ANIM_TAG_MEGA/ALPHA/OMEGA_STONE`이 있다. STATUS.md:1497과 SESSION_LOG.md:1937에 이식 기록이 있다. | Y(test/battle) | 작음 | 테스트(`all_anims.c` "Gimmick Form Change animations work")는 복사하지 않았고 ROM 영향은 없다. |
| #10384 | 1.17.0 Battle General/Changed | Make GetSameTypeAttackBonusModifier a little more readable | 미적용·안전 이식 가능(수동 적응) | HnS `GetSameTypeAttackBonusModifier`(`battle_util.c:7517-7526`)는 `ctx->abilityAtk`·`gBattleStruct->pledgeMove`를 쓴다(upstream은 `ctx->abilities[]`·`pledgeState`, #9918). 표기만 다르다. | Y | 없음 | **주의: 동작 변화 hunk가 있다.** `moveType == TYPE_MYSTERY` 가드와 `MOVE_NONE` 검사가 사라진다. HnS에서 `types[2]`는 기본값이 `TYPE_MYSTERY`라 `IS_BATTLER_OF_TYPE(atk, TYPE_MYSTERY)`가 참이 된다. 그래서 무속성 사용자의 리베레이션댄스(`battle_main.c:6321`이 `TYPE_MYSTERY` 반환) 같은 극단 경우에 자속 1.5배가 붙을 수 있다. 가독성 리팩터라 이식 실익이 없고 보류를 권장한다. |
| #10454 | 1.17.0 Battle General/Changed | Add Gen 9 Overworld Weather Behavior | 미적용·선행 필요 | HnS `TryChangeBattleWeather`(`battle_util.c:2113`)는 bool을 반환하고, 날씨 특성 분기(3388~, Sand Spit 4259, Teraform Zero 3487)는 옛 구조다. PR은 #10170+#10214의 `SetStartingWeatherStatus`와 #10395(Orichalcum Pulse 메시지, HnS에 `OrichalcumPulseActivates*` 없음)를 전제로 하고, config 등록도 `config_changes.h`(HnS는 `generational_changes.h`)를 쓴다. | Y (battle_scripts_1.s, battle_util.c, battle_script_commands.c, battle.h) | 작음 | 선행: #10170·#10214·#10395. **동작·메시지 영향:** 새 config `B_OVERWORLD_WEATHER_OVERRIDE`의 기본값은 `GEN_LATEST`다. HnS는 필드 날씨를 배틀에 반영하므로(`FIELD_EFFECT_OVERWORLD_WEATHER`, `battle_util.c:2990`), 이식하면 비·해 등 필드 날씨에서 날씨 특성·기술이 실패하고 특성 팝업 뒤 `STRINGID_BUTITFAILED`(새 `BattleScript_BlockedByOverworldWeather`)가 나온다. 현 HnS 동작을 유지하려면 `GEN_8`로 둘 것. Sand Spit은 선택값이 `moveStartMessage`에서 `abilityStartMessage`로 바뀌지만, HnS 스크립트가 `BattleScript_SandSpitActivates`에서 고정 문자열을 출력하므로 출력 문자열은 같다. 새 문자열 ID는 없다. |
| #10471 | 1.17.0 Battle General/Changed | Remove infiniteConfusion flag | 미적용·안전 이식 가능(수동 적응) | HnS에 `infiniteConfusion`·`RemoveConfusionStatus`·`CanBeInfinitelyConfused`가 그대로 있다(`battle_hold_effects.c:135-155`, `battle_util.c:4824,9243,9794,10440-10454`, `battle_move_resolution.c:379`). `confusionTurns`는 42곳에서 쓰인다. C 쪽은 이름 변경과 표기(1인자 `CanBeConfused`, `SetMoveEffect` 구 시그니처) 차이뿐이다. | Y (22파일: battle_scripts_1.s, battle_util.c, battle_hold_effects.c, battle_script_commands.c 등) | 없음 | Berserk Gene 스크립트는 HnS가 #9730 이전 형태다(`battle_scripts_1.s:8333 BattleScript_BerserkGeneRet`, `statbuffchange` 사용, 아이템 팝업 없음). upstream hunk는 그대로 적용되지 않는다. C에서 먼저 `confusionTimer=PERMANENT`를 설정하면 HnS `SetMoveEffect` 혼란 분기(`battle_script_commands.c:2567-2570`)가 이미 혼란 상태로 보고 메시지를 건너뛴다. 따라서 `jumpifvolatile …→STRINGID_PKMNWASCONFUSED` 분기를 HnS 스크립트에 직접 넣어야 한다. `TryImmunityAbilityHealStatus`의 `IMMUNITY_CONFUSION_CLEARED`(9241-9244)는 HnS 상태회복 메시지 변경부와 인접하므로 `RemoveConfusionStatus` 줄만 바꿀 것. 리팩터라 실익이 낮다. |
| #10459 | 1.17.0 Battle General/Changed | Replaced / Removed some battlerAbsent checks in favor of IsBattlerAlive | 미적용·안전 이식 가능(수동 적응) | HnS는 PR 이전 상태다: `gAbsentBattlerFlags` 직접 검사(예: `battle_util.c:379`, `CountAliveMonsInBattle` `pokemon.c:4165`, Dragon Cheer `battle_script_commands.c:8631`, `CancelerSetTargets` `battle_move_resolution.c:869`). `IsBattlerAlive`(`battle.h:1067`)가 absent 검사를 이미 포함하므로 대부분 표기 차이다. | Y (battle_util.c, battle_script_commands.c, include/battle_util.h) | 없음 | **#10634가 이 PR 일부를 되돌렸으므로**(컨트롤러 B버튼, `HandleTurnActionSelectionState` 3곳) 이식하려면 그 부분을 빼야 한다. 동작 변화 hunk가 있다: `FaintClearSetData`의 타입 초기화 3줄 제거(HnS `battle_main.c:3579-3581`), `GetMoveTargetCount`·`CountAliveMonsInBattle`이 HP 0이지만 아직 absent가 아닌 배틀러를 제외. `include/constants/battle_util.h` 신설(상수 이동, `CountTrue` 매크로)도 포함한다. 실익은 낮다. |
| #10576 | 1.17.0 Battle General/Changed | Simplify IsAnyTargetAffected | HNS와 무관(1.17.0 안에서 전면 되돌림) | upstream이 곧바로 **#10588 "Revert Simplify IsAnyTargetAffected"(f119b5586f)** 로 되돌렸다. 두 diff의 변경 줄이 정확히 역관계이고, 1.17.0 태그의 `IsAnyTargetAffected`는 PR 이전 형태다. HnS `IsAnyTargetAffected`(`battle_util.c:10946`)는 원래 #10566/#9939 이전 형태라 이식 대상이 없다. | Y | 없음 | 참고: 선행이던 `MOVE_RESULT_NOT_PRESENT`(#10566)·`CancelerAccuracyCheck`(#9939)도 HnS에 없다. |
| #10591 | 1.17.0 Battle General/Changed | Update Howl's ignoreSubstitute flag to account for Champions behavior | 이미 적용 | `IsSubstituteProtected()`의 자기 대상 변화기 예외와 `MOVE_HOWL` `.ignoresSubstitute = B_UPDATED_MOVE_FLAGS >= GEN_CHAMPIONS` 반영(`.target` 공백만 차이) | Y | 없음 | 테스트 미반영 |
| #10595 | 1.17.0 Battle General/Changed | Move Substitute check to move resolution and remove acc check in scripts | 미적용·선행 필요 | HnS에는 `CancelerAccuracyCheck`/`CANCELER_ACCURACY_CHECK`가 없고(canceler 표 `battle_move_resolution.c:2022-2068`), `IsStatChangeMove`·`IsSubstituteProtected`도 없다. 스크립트는 여전히 `accuracycheck`/`jumpifsubstituteblocks`를 84회 쓴다(`battle_scripts_1.s`). | Y (battle_scripts_1.s, battle_script_commands.c, battle_script.inc, battle_move_resolution.c) | 작음(스크립트 감소, 코드 증가) | 선행: **#9939**(명중 판정 canceler 통합), **#9730**(Stat Change Refactor, `IsStatChangeMove`·`IsSubstituteProtected`). 스크립트 hunk는 대타·명중 실패 경로를 `BattleScript_ButItFailed`로 통일할 뿐 새 문자열은 없다. 다만 HnS가 출력 경로를 바꾼 효과 스크립트(정화·잠자기·상태 회복 등)와 같은 파일이므로 이식 시 수동 대조가 필요하다. |
| #10593 | 1.17.0 Battle General/Changed | Change SetMoveEffect into a function table and move to a new file | 충돌 | 표본 확인 결과 HnS `SetMoveEffect`(`battle_script_commands.c:2499`)는 구 시그니처 `(battlerAtk, effectBattler, moveEffect, battleScript, effectFlags)`이고, 그 안에 HNS 고유 수정이 있다. `battle_set_effect.c`로 옮기면 이 수정이 사라진다(아래 충돌 절). | Y | 작음(핸들러 포인터 표 약 0.5KB) | 선행: #10426(SetMoveEffect 시그니처·흡수 리팩터. HnS는 의도적으로 **미이식**, SESSION_LOG.md 1942), #9657/#9859(`BattleCalcValues`). 새 STRINGID는 없고 `enum MoveEffect`는 `constants/battle_set_effect.h`로 옮겨진다. |
| #10541 | 1.17.0 Battle General/Changed | Add new effectiveness indicators from Pokémon Champions | 이미 적용 | charmap.txt:1083~1084(`STAR`, `TRIANGLE_UPSIDE_DOWN`), fonts.c 폭 테이블, 라틴 폰트 PNG 9개(upstream 신버전과 blob 동일), battle_controller_player.c:2429·2458~2482가 반영돼 있다. 변수명 오타 `extremeleyEffectiveIcon`만 `extremelyEffectiveIcon`으로 바뀌어 있다. | N | 작음 | 한글 폰트와는 충돌하지 않는다(바뀐 건 라틴 폰트 PNG뿐이고 HnS 라틴 PNG가 upstream과 동일). 기존 아이콘에는 HnS 색 코드가 있지만 새 두 아이콘에는 없다(불확실 목록). |
| #10630 | 1.17.0 Battle General/Changed | Remove redundant embargo and heal block flags | 미적용·안전 이식 가능(수동 적응) | HnS의 `healBlock`/`embargo`/`VOLATILE_HEAL_BLOCK`/`VOLATILE_EMBARGO` 참조 51곳이 upstream hunk와 대응한다. upstream `battle_set_effect.c` Psychic Noise hunk는 HnS `SetMoveEffect` `MOVE_EFFECT_PSYCHIC_NOISE`(`battle_script_commands.c:3093-3105`)에 적용한다. Mental Herb는 `battle_hold_effects.c` `TryMentalHerb`(~463-465)에 해당한다. | Y | 없음 | 동작 동일. HnS는 바톤터치 때 embargo·healBlock 타이머를 명시적으로 복사하므로(`battle_main.c:3403-3404`) #10318 같은 버그가 없다. battle_debug.c 문맥은 #10318 이후 기준이다. 순수 정리라 실익이 낮다. |
| #10634 | 1.17.0 Battle General/Changed | Partially reversion of gAbsentBattler pr | HNS와 무관(upcoming 리팩터 회귀 수정) | #10459(IsBattlerAlive 치환)가 만든 회귀를 되돌리는 PR이다. HnS는 #10459가 없어서 해당 줄이 이미 되돌림 후 상태와 같다(`battle_controller_player.c:371`, `battle_main.c:4392,4395,4807`이 `gAbsentBattlerFlags` 사용. `BATTLE_PARTNER` 표기만 다름). | N | 없음 | HnS엔 해당 버그가 없다. #10459를 이식할 경우에만 이 되돌림도 반영할 것. |
| #10640 | 1.17.0 Battle General/Changed | Add MSG_DISPLAY constants and helper function for strings with wait | 미적용·안전 이식 가능(수동 적응) | HnS는 `gBattleCommunication[MSG_DISPLAY] = 0/1`을 직접 쓴다(`battle_script_commands.c` 18곳, `Cmd_critmessage` 1880-1898, `Cmd_resultmessage`는 Champions 효과 배율 분기가 들어간 HnS 수정본). 상수·헬퍼 추가만으로는 동작 변화가 없다. | Y (battle_script_commands.c, battle_util.c) | 없음 | **`cMISS_TYPE` 정의와 `MISS_TYPE` 초기화를 지우는 hunk는 빼야 한다.** upstream은 선행 #9939/#9730/#9446에서 사용처를 없앴지만 HnS는 아직 쓴다(`battle_scripts_1.s:3645`, `battle_arena.c:392`, `battle_script_commands.c:1174,9687`, `battle_util.c:457`, `battle_move_resolution.c:4057`). `Cmd_givecaughtmon` hunk는 HnS 파티 제한 수정 줄(`GetMaxPartySize`·`IsPartyLimitChallengeActive`)과 인접한다. |
| #10713 | 1.17.0 Battle General/Changed | Restore GetDefaultSelectionTarget usage | HNS와 무관(upcoming 리팩터 회귀 수정) | upstream #10659가 만든 `GetDefaultSelectionTarget` 사용이 이후 병합으로 사라져 복원한 PR이다. HnS `HandleInputChooseMove`(`battle_controller_player.c:709-713`)의 인라인 분기(`battler`/`BATTLE_PARTNER`/`GetOpposingSideBattler`)는 그 헬퍼와 동작이 같다. | N | 없음 | HnS엔 해당 문제가 없다. 헬퍼를 쓰려면 #10659(+#10542 `GetPartnerBattler`·`GetBattlerLeftFoe`)가 필요하나 실익은 없다. |
| #10027 | 1.17.0 Moves/Changed | Add Item Icon to Knock Off, Covet, and Thief animations | 미적용·안전 이식 가능(수동 적응) | C 파일(battle_anim_effects_1.c)은 그대로 적용된다(`AddItemIconSprite`, `gFallingBagAffineAnimTable`, `AnimKnockOffItem`/`AnimItemSteal` 모두 존재). 스크립트는 `loadspritegfx ANIM_TAG_ITEM_BAG` 줄 때문에 문맥이 실패한다(31086~31089, 31341~31347). `gLastUsedItem`은 Knock Off(`src/battle_move_resolution.c:3087`)와 Steal(`StealTargetItem`, battle_script_commands.c:2266)에서 애니 전에 설정된다. | N | 작음(+수백 B) | **수동:** 두 애니에서 `loadspritegfx ANIM_TAG_ITEM_BAG`를 반드시 지울 것. 남겨 두면 같은 태그의 가방 팔레트가 먼저 로드돼 아이콘 색이 틀어진다. 기능 추가(연출)다. |
| #9924 | 1.17.0 Moves/Changed | Added more common anim functions | 미적용·선행 필요 | 74 hunk 대량 리팩터이고 첫 실패 지점부터 막힌다. | N | 없음(감소 추정) | 선행: #8497, #9142, #9172, #9677, #9676, #9168, #10027 문맥. 기능 변화가 없어 **비권장.** |
| #10361 | 1.17.0 Moves/Changed | Updated Sticky Web to call StringShotThread | 미적용·안전 이식 가능 | HnS `gBattleAnimMove_StickyWeb`(약 7682)는 `SpiderWebThread`(targets_both=0)를 14회 호출한다. 장판기라 양쪽 대상 연출(`StringShotThread`)로 바꾸는 PR이다. fwd ok이고 체인 적용도 확인했다. | N | 없음 | 연출 개선. |
| #10374 | 1.17.0 Moves/Changed | Cleaned up some move animations | 미적용·선행 필요 | 스크립트 hunk가 `CreateTailwindCrescents`/`UnsetHighSpeedBg`(#9924 산물) 문맥에서 실패한다. .inc·constants·battle_anim_water.c·src/data/battle_anim.h hunk는 단독으로 적용된다. | N | 없음 | 선행: #9924와 그 선행. 단독으로 의미 있는 부분은 `ANIM_TAG_TOXIC_SPIKES`가 `B_NEW_SPIKES_PARTICLE`(HnS TRUE)이면 새 Spikes 그래픽을 쓰게 한 1줄뿐(src/data/battle_anim.h)이다. |
| #10421 | 1.17.0 Moves/Changed | Updated `battle_anim_new.c` to match style guide | 미적용·안전 이식 가능(수동 적응) | `src/battle_anim_new.c` 부분은 HnS 고유 변경(`AnimTask_GetTimeOfDay`→`GetTimeOfDay()`, 7566~)과 겹치지 않아 그대로 적용된다. 스크립트 6줄(CorrosiveGas 15149~15159의 `gSpriteTemplate_CorrosiveGasSmoke`→`gSmokeBallEscapeCloudSpriteTemplate`, 내용 동일)은 hex 문맥 때문에 수동이다. | N | 없음 | 순수 스타일(콤마·중괄호)이고 기능 변화가 없다. **이득 없음, 비권장**(diff 1900줄 소모). |
| #10265 | 1.17.0 Moves/Fixed | Fix stat change berries showing held item anim twice | HNS와 무관 | upcoming 리팩터(#9730 + #9168) 회귀 수정이라 HnS에는 해당 버그가 없다. 이중 애니 버그는 upstream #9730(Stat Change Refactor)이 #9168 위에서 `BattleScript_ConsumableBerryStatRaise`→`call BattleScript_ConsumableItemStatRaise`로 바꾸며 생겼다. HnS에는 #9168도 `trybattlerstatchange`도 없다. | Y(battle_scripts_1.s) | 없음 | #9168을 PR 원형대로(`..._AnimContinue` 호출) 이식하면 이중 애니는 생기지 않는다. #9730 계열까지 가져올 때만 필요하다. |
| #10401 | 1.17.0 Moves/Fixed | Set Leafage and Seed Flare's `target_both` to FALSE | 미적용·안전 이식 가능(수동 적응) | merge-base 버전부터 있던 버그다(upcoming 회귀 아님). HnS SeedFlare(3882~3897, cutter 6줄)와 Leafage(10496~10498, 3줄)가 `target_both=1`이라 단일 대상기인데도 양쪽 대상 연출이 나온다. 버그가 그대로 있다. | N | 없음 | **수동:** 값만 1→0으로 바꾸면 된다. upstream 형태(`create_razor_leaf_cutters` 매크로, `CreateRazorLeafCutters` 삭제)는 #9142/#9172 문맥이라 적용되지 않는다. MagicalLeaf의 duration 32→22 변경도 PR에 섞여 있다. |
| #8893 | 1.17.0 Abilities/Changed | The big grammar update | 충돌 | config 주석·특성/도구/기술 **영문 설명** 대량 교정(items.h 2970줄 등). HnS 설명은 영문이지만 같은 파일의 이름·복수형이 한글화돼 있어 hunk 문맥이 모두 어긋남. SESSION_LOG 2026-09-15에서 "한글 덮어쓰기 위험으로 미이식" 결정 | N | 작음 | **충돌 절 참조**. 이식하더라도 설명 문자열만 ID 기준 수동 치환 |
| #10592 | 1.17.0 Items/Changed | enigma berry hold effect cleanup | 이미 적용 | `GetItemHoldEffect()`의 E-Reader 에니그마 분기(item.c:860), pokemon.c 호출부 단순화(6084/6786/7129 등) 반영. SESSION_LOG 2026-09-15 이식 기록 | Y | 없음 | 역적용 실패는 HnS 형태 차이 |

## 충돌 상세

각 항목에 upstream 목적, HnS 현재 동작과 보존 이유, 파일·함수·라인, 선택지를 적었다. 사용자가 결정하기 전에는 해당 hunk를 적용하지 않는다.

### #9168 Add Held Berry Animation (`1079577272`)
- **upstream 목적:** 열매 발동 시 범용 `B_ANIM_HELD_ITEM_EFFECT` 대신 새 일반 애니 `B_ANIM_HELD_ITEM_BERRY`(63, 내용은 `gBattleAnimMove_StuffCheeks`와 같은 라벨)를 재생한다. 이를 위해 `BattleScript_ItemHealHP_RemoveItem`을 `..._RemoveBerry`/`..._RemoveItem`으로, `BattleScript_ConsumableStatRaiseRet`을 `BattleScript_ConsumableBerryStatRaise`/`BattleScript_ConsumableItemStatRaise`로 나누고, `jumpifnotberry`/`BS_JumpIfNotBerry`를 삭제한다. Fling·Stuff Cheeks·Bug Bite의 `HITMARKER_DISABLE_ANIMATION` 토글도 없애 이 경우에도 애니가 나오게 한다. 문자열 변경은 없다.
- **HnS 현재 동작과 보존 이유:** HnS는 #9777 아이템 팝업을 HNS식으로 별도 이식해 열매 스크립트마다 `call BattleScript_ItemPopUp_Scripting(NoFlush)`를 넣었다(STATUS.md 2026-09-20 절). 또 HnS 고유 `BattleScript_LumBerryCureStatusRet`(7205, 상태별 메시지 루프, BATTLE_MESSAGE_OUTPUT_CHANGES.md:32)이 있는데 upstream에는 없다. upstream hunk를 그대로 적용하면 팝업 호출 위치와 HnS 고유 라벨이 어긋난다.
- **파일·라인(HnS):** `data/battle_scripts_1.s`
  - Fling 543~549, StuffCheeks 699~701, BugBite 900~907
  - BerryCureStatusRet 7196, LumBerryCureStatusRet 7205, BerryReduceDmg 7227, BerryCureConfusionRet 7236
  - ItemHealHP_RemoveItem 7282, BerryPPHeal 7297, BerryConfuseHeal 7398, ConsumableStatRaiseRet 7413(`jumpifnotberry` 7414), BerryFocusEnergy 7432
  - CustapBerryActivation 7686, MicleBerryActivate 7695, JabocaRowapBerryActivates 7707
  - 파일 전체의 `B_ANIM_HELD_ITEM_EFFECT` 사용: 33곳

  `src/battle_hold_effects.c`: 91, 106, 401, 529, 823, 911, 930, 984(스크립트 포인터 교체, hunk는 그대로 적용됨). `src/battle_script_commands.c:13821` `BS_JumpIfNotBerry`, `include/constants/battle_anim.h:607~608`(`NUM_B_ANIMS_GENERAL 63`, HnS 고유 일반 애니 없음).
- **선택지:**
  - (a) 미이식: 현 상태를 유지한다. 열매도 일반 도구 애니를 쓴다.
  - (b) 수동 이식: 애니 ID·라벨·`sBattleAnims_General`·hold_effects 포인터만 upstream대로 넣는다. 열매 스크립트는 HnS의 `call BattleScript_ItemPopUp_*` 줄과 LumBerry 스크립트를 보존한 채 `playanimation` ID만 `B_ANIM_HELD_ITEM_BERRY`로 바꾸고, ItemHealHP/ConsumableStatRaise만 PR 원형대로 둘로 나눈다(#10265 불필요). Fling/BugBite/StuffCheeks의 DISABLE_ANIMATION 제거는 연출(애니 추가 재생)이 바뀌므로 선택 사항이다.
  - (c) 최소 이식: 열매 전용 스크립트의 `playanimation` ID만 교체하고 분리·삭제 작업은 생략한다.

### #10151 Champions 배틀 메커니즘 v1.0.X (나머지)
- upstream 목적: Champions 규칙(마비 불발 12.5%, 동결 해제 25%+3턴 상한, 메가진화가 스피드스웝 값을 덮어쓰지 않음, 속이다/만나자마자 첫 턴 이후 선택 불가, 소금절이 피해 절반, 트림/볼부풀리기/토해내다/라스트리조트/불태우기 선택 제한, 앵콜 우선도, 기절 후 기술 부가효과 타이밍, 우격다짐과 특성 상호작용, 보이지않는주먹/관통드릴의 방어 관통 시 1/4 피해)을 GEN_CHAMPIONS config로 추가.
- HnS 현재: 수면 턴 난수만 Champions식. `include/config/general.h:74` `GEN_LATEST=GEN_CHAMPIONS`이므로, 나머지를 upstream 기본값(GEN_LATEST)으로 넣으면 HnS 배틀 규칙이 즉시 Champions식으로 바뀐다(게임 밸런스 변경).
- 메시지 충돌: `STRINGID_BELCHCANTSELECT` 문구 교체(HnS 한글 유지 대상, `src/battle_message.c`), 신규 `STRINGID_CANTUSEMOVE`·`STRINGID_BELCHCANTUSE` 한글 문구 필요. `B_MSG_HYPERSPACE_FURY→B_MSG_BROKE_THROUGH_PROTECT` 개명과 `SetMoveEffect`의 Feint 메시지 선택 제거는 HnS의 `gProtectLikeUsedStringIds`와 맞춰야 함.
- 구조: upstream 캔슬러는 `struct BattleCalcValues *cv`, HnS는 `struct BattleContext *ctx`(battle_move_resolution.c:40~). `MOVE_LIMITATION_UNUSABLE`/`SetCantSelectScript` 선택 제한 구조, `CopyMonLevelAndBaseStatsToBattleMon(…, updateSpeedStat)` 시그니처 변경도 필요.
- 선택지: (a) config만 추가하고 기본값을 이전 세대(GEN_9)로 두어 동작 불변 이식, (b) Champions 규칙을 기능별로 골라 GEN_LATEST로 활성, (c) 보류. 문자열은 공식 한국어 확인 후 추가.

### #9777 Champions 배틀 메시지 (미반영분)
- upstream 목적: Champions 문구로 기존 문자열 38개 영문 개정, 다단히트 횟수 복수형(`multihitplurality`, `STRINGID_S`), 보이지않는주먹/관통드릴 보호 관통 메시지, 유폭 피해 문자열 개명, 도구 회복 메시지 정리, 아이템 팝업.
- HnS 현재: 한글 `번 맞았다!` 유지(복수형 불필요), 기존 한글 문구 유지, 맹독·화염구슬은 HnS 전용 스크립트(BATTLE_MESSAGE_OUTPUT_CHANGES.md "맹독구슬·화염구슬 발동"), `STRINGID_AFTERMATHDMG`·`STRINGID_STICKYWEBDISAPPEAREDFROMYOU` 유지, `CANCELER_NOT_FULLY_PROTECTED`(“완전히 막지 못했다”) 경로 유지(include/constants/battle_move_resolution.h).
- 파일: `src/battle_message.c`, `data/battle_scripts_1.s`(BattleScript_ToxicOrbActivates/FlameOrbActivates/UnseenFist/HurtAttackerNoMsg), `src/battle_move_resolution.c`(`CantFullyProtectFromMove`→`GetProtectBypassMethod`), `src/battle_tv.c`.
- 선택지: (a) 현 상태 유지(권장; 문구 개정은 한글에 무의미), (b) Unseen Fist 보호 관통 메시지만 새 한글 문구와 함께 기능 이식, (c) battle_tv의 신규 효과 단계 ID 처리만 보완(TV 기록 정확도).

### #8893 The big grammar update
- upstream 목적: 영문 설명·config 주석 문법 통일.
- HnS 현재: 설명 문자열은 영문이지만 `src/data/{abilities,items,moves_info}.h`의 이름·복수형은 한글화(도구 복수형 183개 복구 이력). 원작업자가 "한글 덮어쓰기 위험으로 미이식" 결정.
- 선택지: (a) 보류(권장, 용량·가치 대비 위험 큼), (b) config 주석만 이식, (c) 설명 문자열만 ID 매칭 스크립트로 치환(이름·복수형 제외) — 추후 한글 설명 번역 계획이 있으면 무의미.

### #8943 12v12 capability (표본 확인)
- upstream 목적: 전투 파티를 트레이너 4명 × 6마리(`gParties[4][6]`)로 일반화해 멀티배틀 12대12를 지원하고, 파티 접근을 `GetBattlerParty/GetTrainerParty`로 통일.
- HnS 현재 동작과 보존 이유: `gPlayerParty/gEnemyParty` 2파티 구조(`src/pokemon.c:116-118`)를 유지한다. HnS는 `pokemon.c`, `party_menu.c`, `battle_main.c`, `pokemon_storage_system.c`, `trade.c`, `battle_tv.c`를 대량으로 고쳤다(merge-base 대비 +3,928줄/-371줄). 한글화된 링크 배틀 문구 `sText_Link*`(`src/battle_message.c:106,121` 등)도 이 PR이 수정하는 대상이다.
- 영향: EWRAM 정적 +1,200B(여유 약 13.1KB의 9%), 힙 약 +0.5KB, 트레이너 구조체와 trainerproc 형식 변경(HnS `trainers.party` 재생성 필요), `B_MULTI_HALF_TEAMS=FALSE`에서 로켓단 아지트 멀티전(`RocketHideout_B2F_hns/scripts.inc:310`)의 편성 변화 가능성.
- 선택지: (1) 미이식(권장, HnS에 12v12 수요 없음). (2) 이식하려면 upcoming 선행 131커밋을 먼저 정리해야 하고, 한글 `sText_Link*`는 토큰만 수동 반영해야 함.

### #9514 SetMoveEffect cleanup
- upstream 목적: `SetMoveEffect`에서 `gEffectBattler`/`gBattleScripting.battler`를 일관되게 쓰도록 하고, 스크립트 이름을 `BattleScript_MoveEffect*`로 통일하며, 방벽 3종 스크립트를 `BattleScript_MoveEffectScreens`로 통합.
- HnS 현재 동작과 보존 이유: 대상 문자열 약 30개가 한글이다(예: `src/battle_message.c`의 조이기·소용돌이·"이미 잠들어 있다" 계열). 방벽 설치 메시지는 HnS가 오로라베일 선택값을 바꾼 `gReflectLightScreenSafeguardStringIds`(`src/battle_message.c:1100-1109`)를 3개 스크립트(`data/battle_scripts_1.s:684-692, 2810-2813`)에서 출력한다. 브레이크 방벽 순차 출력 `BattleScript_BreakScreens`(:3797-3829)는 HnS 고유 코드다.
- 선택지: (1) 미이식(권장, 순수 리팩터). (2) 이식하려면 한글 문자열마다 토큰만 교체하고, `MoveEffectScreens` 통합 뒤 `saveattacker/copybyte gBattlerAttacker, gEffectBattler`가 HnS 방벽 메시지(`{B_ATK_TEAM}` 계열)에 맞는지 검증해야 함.

### #9714 Slight restructure for Defog and Tidy Up
- upstream 목적: 안개제거·정리정돈에서 `gBattlerAttacker`를 임시로 진영 번호로 덮어쓰던 방식을 없애고 `gBattleScripting.battler`와 스크립트 안의 save/restore로 바꿈. 턴 종료 방벽 만료도 같은 규약으로 맞춤.
- HnS 현재 동작과 보존 이유: BATTLE_MESSAGE_OUTPUT_CHANGES의 "리플렉터·빛의장막·오로라베일 개별 해제 문구", "흰안개 → STRINGID_NOLONGERMIST", "신비의부적 → STRINGID_PKMNSAFEGUARDEXPIRED", "장판별 DISAPPEAREDFROMTEAM" 구현이 이 코드 위에 있다.
  - `src/battle_script_commands.c:7135-7229` `DEFOG_CLEAR`/`DefogClearHazards`/`TryDefogClear`는 HnS 전용 `*Return` 스크립트(`data/battle_scripts_1.s:4590-4624, 4695-4698`)와 `BattleScript_DefogClearHazards`(:5149)를 호출한다.
  - `src/battle_end_turn.c:975,987,999,1009,1070`은 `BattleScriptExecute`(end2 방식)로 전용 만료 스크립트를 실행한다.
  - 한글 문구는 `{B_ATK_PREFIX1}`·`{B_ATK_PREFIX3}`·`{B_ATK_TEAM2}`(`src/battle_message.c:261,527-532,797`)로 `gBattlerAttacker = 진영`을 전제로 한다.
- 선택지: (1) 미이식(권장, 동작 이득 없음). (2) 이식하려면 #9680을 먼저 반영하고, HnS 전용 `*Return` 5종과 `BattleScript_DefogClearHazards`에 `saveattacker; copybyte gBattlerAttacker, sBATTLER; … restoreattacker`를 추가해야 함. `STRINGID_PKMNSUBSTITUTEFADED` 한글(:311) 토큰 B_DEF→B_SCR 교체, 정리정돈 대타 제거 메시지 재검증 필요.

### #9939 Integrate accuracy check into canceler
- upstream 목적: 명중 판정을 canceler 단계로 통합해 범위기 빗나감 메시지 순서(아군 → 왼쪽 상대 → 오른쪽 상대)와 떼쓰기의 면역 특성 처리를 바로잡고, 빗나감 문자열 ID를 정리.
- HnS 현재 동작과 보존 이유: HnS는 빗나감을 `STRINGID_ATTACKMISSED`(「그러나 {공격자}의 공격은 빗나갔다!」)로 출력한다(`src/battle_message.c:207, 1025-1029`). `PKMNEVADEDATTACK`(「…는 공격을 피했다!」, :285)와 `PKMNAVOIDEDATTACK`(「…에게는 맞지 않았다!」, :521)도 서로 다른 한글 의미로 번역했다. upstream은 #9655에서 빗나감을 AVOIDED로 바꾼 뒤 이 PR에서 ID 두 개를 개명·통합하므로, 그대로 들이면 HnS 한글 빗나감 문구와 선택 경로가 바뀐다.
- 선택지: (1) 미이식(권장). #9655·#9657 등 선행 체인이 커서 단독 이식이 불가능. (2) 동작 수정만 원하면 1.16.0 최종 코드의 명중 판정 순서 로직만 참고해 HnS 구조에 재구현하고, 문자열 ID·한글 매핑은 HnS 것을 유지.

### 경미한 한글 토큰 충돌(판정은 안전 이식 가능, 수동 병합)
- #9616: `STRINGID_TARGETWOKEUP`(`src/battle_message.c:561`) B_DEF→B_EFF.
- #9916: `STRINGID_FRISKACTIVATES`(`src/battle_message.c:625`) B_ATK→B_EFF, B_DEF→B_SCR.
- 두 경우 모두 한글 본문은 유지하고 토큰만 바꾸면 된다. 조사 매크로는 직전 출력 글자(`GetJongCode`, `src/battle_message.c:3238`) 기준이라 영향이 없다.

### #10593 — SetMoveEffect 함수 표화·`battle_set_effect.c` 분리 (표본 확인)
- **upstream 목적:** 거대한 `SetMoveEffect` switch를 `sSetEffectHandlers[]` 함수 표로 나눠 새 파일 `src/battle_set_effect.c`(1543줄)로 옮긴다. 입구는 `SetMoveEffect(struct BattleCalcValues *cv, struct SetEffect *se)`다. `enum MoveEffect`와 관련 상수는 `include/constants/battle_set_effect.h`로 이동한다. 대체로 동작은 같다(표본: `HandleSetEffectBreakScreen`·`HandleSetEffectAuroraVeil`·`HandleSetEffectStealStats`·`SetMoveEffect` 진입부 비교).
- **HnS 현재 동작과 보존 이유:** HnS `src/battle_script_commands.c`의 `SetMoveEffect`(2499~)는 #10426 이전 시그니처다. 그 안에 HNS 고유 변경이 들어 있다.
  - `MOVE_EFFECT_RECHARGE`(2819-2820): 챌린지 설정 `gSaveblock3.challengeSettings.genOneRecharge`에 따라 Gen1식으로 반동을 생략(HnS 고유 기능).
  - `MOVE_EFFECT_AURORA_VEIL`(3430-3438): `B_MSG_SET_AURORA_VEIL` 선택. BATTLE_MESSAGE_OUTPUT_CHANGES.md의 "오로라베일 성공 → `STRINGID_PKMNRAISEDDEFSPDEF`" 항목이다.
  - `MOVE_EFFECT_BREAK_SCREEN`(3594-3633): 제거된 방벽 마스크를 `B_MSG_BREAK_REFLECT/LIGHT_SCREEN/AURORA_VEIL`로 저장한 뒤 `BattleScript_BreakScreens`에서 종류별 문구를 차례로 출력한다. 같은 문서의 "깨뜨리다·사이코팽·레이징불 복수 방벽 제거" 항목이다.
  - `MOVE_EFFECT_STEAL_STATS`(3634-3680): `BattleScript_StealStats` 호출을 루프 밖으로 옮긴 수정.
  - `SetNonVolatileStatus`(2340~, 2355-2358): Champions 수면 턴(#10151 HNS 이식). 이 함수는 upstream에서도 battle_script_commands.c에 남는다.
  - 호출부 전역(`gEffectBattler`, `battleScript` 인자)도 HnS 전용 스크립트들이 전제로 한다.
  - 한 번에 옮기면 이 분기들을 upstream 원형으로 덮어쓰게 된다. upstream에도 BreakScreens·AuroraVeil 계열 구현이 있으나 선택값 정의(비트 상수, `B_MSG_BREAK_*` 명명)와 스크립트 해석 방식이 HnS와 다를 수 있다.
- **파일·함수·라인(HnS):** `src/battle_script_commands.c` `SetMoveEffect` 2499, 2819-2820, 3430-3438, 3594-3633, 3634-3680. `SetNonVolatileStatus` 2340. 관련 스크립트는 `data/battle_scripts_1.s`의 `BattleScript_BreakScreens`, `BattleScript_MoveEffectAuroraVeil`.
- **선택지:**
  - (a) 이식하지 않는다(권장). #10426·#9657이 없어 기계적 적용이 불가능하고, 동작 이득이 없다.
  - (b) 꼭 필요하면 #10426을 먼저 HNS식으로 이식한 뒤 핸들러별로 옮긴다. 이때 위 HNS 분기 5곳을 각 `HandleSetEffect*`에 다시 적용하고, 방벽·오로라베일 메시지를 실제 게임에서 회귀 확인한다.
  - (c) 이후 upstream PR(#10630 Psychic Noise 등)이 `battle_set_effect.c`를 건드리면 해당 hunk만 HnS `SetMoveEffect`의 같은 case에 수동으로 반영한다.

### 충돌 인접(주의) — 판정은 충돌이 아니지만 HnS 메시지 변경부와 맞닿는 곳
- **#10454:** `B_OVERWORLD_WEATHER_OVERRIDE` 기본값 `GEN_LATEST`면 필드 날씨 배틀에서 날씨 특성이 특성 팝업 + "하지만 실패했다!"로 바뀐다. HnS 동작을 보존하려면 `GEN_8`로 둘 것.
- **#10471:** `TryImmunityAbilityHealStatus` 혼란 회복(`battle_util.c:9241-9244`)은 HnS 상태 회복 메시지 변경부다. Berserk Gene 스크립트는 HnS식으로 수동 보강해야 한다.
- **#10640:** `Cmd_critmessage`·`Cmd_resultmessage`·`Cmd_givecaughtmon`은 HnS·Champions 수정본이다. 상수 치환만 할 것.
- **#10595:** 대상 효과 스크립트 중 일부(정화·잠자기 등)는 HnS 출력 변경 스크립트이므로 선행 이식 후에도 수동 대조가 필요하다.

## 불확실 항목 (화면·빌드·실기 확인 필요)

### 배치 A(애니·그래픽·아이템)

- **#10129:** upstream 새 트리거 PNG가 8비트 인덱스 PNG(color type 3, bit depth 8)다. HnS `tools/gbagfx`가 4bpp로 제대로 변환하는지는 빌드로 확인해야 한다. 새 y 위치(-5/-2)가 HnS 비-Gen4 체력바와 Gen4 UI의 Z/Tera/Dynamax 트리거에 시각적으로 맞는지도 화면 확인이 필요하다.
- **#10541:** HnS는 기존 효과 아이콘(◎·△·×)에 `COLOR_HIGHLIGHT_SHADOW` 색 코드를 붙였지만, 새 `{STAR}`·`{TRIANGLE_UPSIDE_DOWN}`(battle_controller_player.c:2458, 2461)은 색 코드가 없다. 의도인지 누락인지는 원작업자 확인이 필요하다(upstream 판정과는 무관).
- **#9525:** damageCategory 변경이 HnS 챌린지 "타입별 물리/특수" 모드(`optionStyle==1`)에서 Stellar 기술(Tera Starstorm 등) 분류를 PHYSICAL→SPECIAL로 바꾼다. 의도대로인지 HnS 정책 확인이 필요하다.
- **#9142 ROM 감소량:** 빌드하지 않아 수치를 측정하지 못했다(서브루틴화라 감소로 추정).
- **#10027:** `AnimTask_StealItem`/`KnockOffItem`이 아이콘 생성 직후 `FreeSpriteTilesByTag`를 호출한다(upstream과 동일). HnS의 다른 애니와 VRAM 재할당이 겹치는지는 실제 화면 확인이 필요하다.
- **#9473:** 호출 깊이 카운터를 애니 시작 시 초기화하는 코드가 없다(upstream 1.17.0도 없음). HnS 고유 애니 중 `call`한 곳에서 `end`로 끝나는 경우는 스크립트 파서로 2곳(PR이 수정)만 찾았고, 동적 분기(`jumpargeq` 등)를 통한 경로는 완전히 추적하지 않았다.

### 배치 B(기술·특성·Champions)

- #10590: HnS에서 방음 보유 사용자가 자기 멀리짖기를 막는지(동작)는 테스트 없이 확인하지 않음.
- #10395: 공식 한국어 문구 출처 미확인.
- #10151: 이식 시 AI(`battle_ai_util.c`의 Belch 판정) 변경이 HnS AI 이식분과 맞는지 미확인.

### 배치 C(1.16.0 배틀 엔진)

1. #9616: Gen5+로 바꾸면 턴 종료·행동 전 기상 경로가 막히는데, 배틀 팰리스 경로 `BattlePalace_TryEscapeStatus`(`src/battle_util2.c:137`)는 upstream PR에서도 수정되지 않았다. HnS 배틀프런티어에서 팰리스를 쓰면 Gen3-4 동작이 일부 남을 수 있다. 실제 화면에서 여러 마리가 잠든 더블배틀 순차 기상 메시지도 미검증.
2. #9408: 구 방식(`!NoAliveMonsForEitherParty` 조건, 공격자 생존 미검사)과 신 방식(`IsBattlerAlive(attacker)` + 대상 피격)의 차이가 게임에서 드러나는 정확한 시나리오(울퉁불퉁멧·까칠한피부로 공격자가 기절)는 테스트로 확인하지 않음.
3. #9898: `IsAnyTargetAffected` 변경이 HnS의 `battle_move_resolution.c:3988`(increment 판정) 등 다른 호출부에 주는 영향은 코드 열람만 했고 빌드·테스트는 하지 않음.
4. #8943: ROM 증가량은 빌드 없이 추정(수 KB). EWRAM +1,200B는 맵 주소로 계산한 값.
5. #9916: Frisk에서 대상 도구가 없을 때 `effect` 반환값이 바뀌는 것(HnS는 무조건 `effect++`, `src/battle_util.c:3253-3259`)이 등장 특성 처리 순서·대기 시간에 실제 차이를 만드는지 미확인.
6. #9989: HnS에서 배틀돔 트레이너 카드 화면이 실제로 도달 가능한지(맵은 존재, 진행 조건은 미확인).
7. #9657/#9786: AI 파일 hunk가 HnS의 1.17 AI 선별 이식(#10243 등)과 겹치는 정확한 범위는 따지지 않음(순수 리팩터라 판정에는 영향 없음).

### 배치 D(1.17.0 배틀 엔진)

1. **#10318 바톤터치 전자부유 영구화 버그:** 코드 추론 결과다(`battle_main.c` 바톤터치 복사 목록과 `HandleEndTurnMagnetRise` 조건). 실제 게임·테스트로는 확인하지 않았다.
2. **#10384:** `TYPE_MYSTERY` 가드 제거로 무속성 리베레이션댄스 등에 자속이 붙는지는 upstream이 의도한 것인지 확인하지 못했다. 1.17.0 최종본도 같은 코드이고, 후속 수정은 찾지 못했다.
3. **#10338:** 교체 등장 시 `gBattleStruct->moveResultFlags[battler] = 0` 제거가 HnS에서 부작용(교체 직후 결과 플래그 잔존)을 낼지 불명이다. HnS는 `HandleAction_UseMove`의 `ClearDamageCalcResults` 등으로 초기화된다고 보이나 모든 경로를 추적하지는 않았다.
4. **#10459:** `FaintClearSetData`의 타입 초기화 제거 의도(교체 등장 때 `CopyMonAbilityAndTypesToBattleMon`으로 재설정하니 중복이라 본 것으로 추정)와, 기절 슬롯의 타입을 읽는 HnS 고유 경로가 있는지는 확인하지 않았다.
5. **#10145:** "부분 적용"은 HnS 문서의 HNS식 이식 기록과 코드 대조에 근거한다. 오라 특성 `RecordAbilityBattle` 미반영이 AI 특성 인지에 미치는 영향은 평가하지 않았다.
6. **#9008:** `B_CATCH_OR_NOT` 창(8×4 타일, "Catch/Don't catch")에 한글 문구가 폭 안에 들어가는지는 기능을 쓸 때 확인이 필요하다.
7. **#10593:** 1716줄 신규 파일 중 BreakScreen·AuroraVeil·StealStats·SetMoveEffect 진입부만 표본 비교했다. 나머지 핸들러에 미세한 동작 변화가 있는지는 전수 확인하지 않았다.

## 작업 산출물

- 배치별 원본 판정: `scratchpad/g3/batch{A,B,C,D}.md`
- 줄 존재 여부 휴리스틱: `scratchpad/g3/presence.py`, `scratchpad/g3/summary_presence.txt`
