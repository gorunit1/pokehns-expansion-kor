# 코드 변경으로 달라진 배틀 메시지 출력

## 범위

이 문서는 단순 한글 번역·조사·줄바꿈 수정과 출력 조건 조사 기록을 제외한다. `pokeemerald-expansion` 1.17.0 이식 또는 `Pokémon Champions` PR #9777에서 문자열만 추가·번역한 부분도 제외한다. 아래에는 사용자 요청에 따라 배틀 코드 또는 배틀 스크립트를 바꿔, **같은 기술·특성·도구·전투 상황이 이전과 다른 문자열을 선택하거나 팝업만 표시하게 된 경우**만 기록한다.

따라서 PR #9777에서 왔더라도 효과 단계 판정, 결과 메시지 분기, 아이템 팝업, 전용 배틀 스크립트처럼 **실제 구현 코드가 바뀐 경우**는 포함한다. “이전”은 이 저장소에서 해당 사용자 요청 직전의 동작, “현재”는 현재 소스 기준이다.

## 기술·필드 상태 효과

| 상황 | 이전 출력 | 현재 출력 | 변경 지점 |
| --- | --- | --- | --- |
| 일격기 성공(가위자르기·뿔드릴·땅가르기·절대영도) | 성공 플래그가 없어 별도 성공 문구가 나오지 않을 수 있었음 | `STRINGID_ONEHITKO` (`일격필살!`) | `CalculateMoveDamage()`가 실제 OHKO 성공에 `MOVE_RESULT_ONE_HIT_KO`를 설정 |
| 오로라베일 성공 | 신비의부적과 같은 `STRINGID_PKMNCOVEREDBYVEIL` | `STRINGID_PKMNRAISEDDEFSPDEF` | `gReflectLightScreenSafeguardStringIds`의 오로라베일 선택값 변경 |
| 리플렉터·빛의장막·오로라베일 하나가 만료 또는 안개제거로 해제됨 | 범용 `STRINGID_THEWALLSHATTERED` | 각각 `STRINGID_REFLECTWOREOFF`, `STRINGID_LIGHTSCREENWOREOFF`, `STRINGID_AURORAVEILWOREOFF` | 턴 종료·안개제거에 전용 스크립트와 선택 테이블 추가 |
| `깨뜨리다`·`사이코팽`·`레이징불`로 복수 방벽 제거 | 복수 방벽 제거 시 `STRINGID_THEWALLSHATTERED` 하나만 출력 | 존재하는 방벽만 리플렉터 → 빛의장막 → 오로라베일 순서로 각각 출력 | 제거된 방벽 마스크를 저장하고 `BattleScript_BreakScreens`에서 종류별 문구를 순차 출력 |
| `배리어프리` 발동으로 방벽 제거 | 특성 팝업 뒤 `STRINGID_SCREENCLEANERENTERS`의 범용 문구 하나 | 특성 팝업 뒤 존재하는 방벽만 `STRINGID_REFLECTWOREOFF` → `STRINGID_LIGHTSCREENWOREOFF` → `STRINGID_AURORAVEILWOREOFF` 순서로 출력 | `TryRemoveScreens()`가 방벽 종류 비트마스크를 반환하고 `BattleScript_ScreenCleanerActivates`가 기존 순차 출력 스크립트를 호출. 메시지 주체는 특성 보유자로 임시 설정 후 복원 |
| 흰안개 만료·안개제거 해제 | 범용 `STRINGID_PKMNSXWOREOFF` | `STRINGID_NOLONGERMIST` | 흰안개 전용 만료/반환 스크립트 추가 |
| 신비의부적을 안개제거로 해제 | 범용 측면 상태 해제 문구 | `STRINGID_PKMNSAFEGUARDEXPIRED` | 안개제거 반환 스크립트 변경 |
| 고속스핀·킬러스핀·안개제거가 압정뿌리기·독압정·스텔스록·끈적끈적네트를 제거 | 범용 장판 해제 문구 | 각 장판의 `STRINGID_*DISAPPEAREDFROMTEAM` | `gSpinHazardsStringIds`·`gDefogHazardsStringIds`를 장판별 ID로 변경 |
| 고속스핀·킬러스핀이 바인드 계열을 해제 | `STRINGID_PKMNGOTFREE` | `STRINGID_PKMNFREEDFROM` | `BattleScript_WrapFree`의 출력 ID 변경 |
| 0.5배 미만 또는 2배 초과 상성(Champions 이식) | 기존 0.5배·2배 단계 문구에만 수렴 | `STRINGID_MOSTLYINEFFECTIVE`·`STRINGID_EXTREMELYEFFECTIVE` 및 단일/복수 대상 전용 ID | 효과 배율 플래그와 결과 메시지 분기 추가 |
| 방어 측 대상명을 넣는 급소·상성 메시지(Champions 이식) | 대상별 전용 문구 없음 | `STRINGID_CRITICALHITONDEF`와 `...ONDEF`·`...TWOFOES` 계열 | 결과 메시지 선택·대상별 출력 경로 추가 |
| 명중 판정이 있는 변화기가 빗나감(`accuracycheck BattleScript_ButItFailed` 사용 기술, upstream #9929 이식) | `STRINGID_BUTITFAILED` (`그러나 실패하고 말았다!`) | `gMissStringIds[B_MSG_AVOIDED_ATK]` = `STRINGID_PKMNAVOIDEDATTACK` (`…에게는 맞지 않았다!`) | `AccuracyCheck()`가 실패 경로가 `BattleScript_ButItFailed`일 때 새 `BattleScript_TargetAvoidsAttackEnd`로 이동 |
| 난동 계열(역린·난동부리기·꽃잎댄스) 종료 시 지쳐서 혼란, 프리폴로 잡혀 있던 난동 중인 포켓몬이 풀려날 때의 혼란 (upstream #9249 이식, `B_RAMPAGE_CONFUSION` = `GEN_LATEST`) | 난동이 끝난 턴의 **턴 종료** 단계에서 `BattleScript_ThrashConfuses`로 `STRINGID_PKMNFATIGUECONFUSION`. 프리폴 해제 혼란은 해제 경로마다(행동 불가·중력·하품·기절 등) 따로 처리 | 같은 `STRINGID_PKMNFATIGUECONFUSION`이 그 기술 **직후** move end(`MOVEEND_RAMPAGE`)에서 `BattleScript_ConfusionAfterRampage`로 출력. 프리폴 해제 혼란은 프리폴 공격 뒤 `MOVEEND_CONFUSION_AFTER_SKY_DROP` 또는 프리폴 사용자가 기절할 때 `tryconfusionafterskydrop`에서 출력. 신비의부적(자기 편)·미스트필드·마이페이스·이미 혼란이면 혼란·문구 없음 | `MoveEndRampage()`, `MoveEndConfusionAfterSkyDrop()`, `Cmd_tryconfusionafterskydrop`, `CanBeConfused(atk, effect)`에 신비의부적 검사 추가. 문자열 ID 변화 없음, 출력 시점만 변경 |
| 트레이너 배틀에서 상대 편이 G-Max Gold Rush(거다이 골드러시, `MOVE_EFFECT_CONFUSE_PAY_DAY_SIDE`)를 사용 (upstream #9514 이식) | 혼란 문구 뒤 `STRINGID_COINSSCATTERED`(`돈이 주위에 흩어졌다!`)가 출력되고 플레이어가 받을 돈(`gPaydayMoney`)이 상대 레벨×100만큼 늘어남 | 상대가 쓰면 돈 증가·문구 없음. 플레이어 편이 쓸 때만 같은 `STRINGID_COINSSCATTERED` 출력 | `SetMoveEffect()`의 `MOVE_EFFECT_CONFUSE_PAY_DAY_SIDE`에 `IsOnPlayerSide(battlerAtk)` 조건 추가. 문자열 ID 변화 없음 |
| 폴터가이스트가 대타출동 상태의 대상을 공격(공격자 특성이 틈새포착이 아닐 때, upstream #9610 이식) | 명중 판정 뒤 `STRINGID_ABOUTTOUSEPOLTERGEIST`(`{도구}이(가)\n{대상}에게 덤벼들었다!`)를 출력하고 대타출동에 대미지 | 도구 공개 문장 없이 공격 애니메이션과 대타출동 대미지만 표시. 대타출동이 없으면 이전과 같은 문장·순서 | 도구 공개 문장을 공격 전 추가 효과 `MOVE_EFFECT_ITEM_MESSAGE`(`setpreattackadditionaleffect` → `SetMoveEffect()` → `BattleScript_PoltergeistMessage`)로 옮김. `DoesSubstituteBlockMoveEffectOnTarget()`이 이 효과를 막음(upstream 1.17.0과 같음). 문자열 ID 변화 없음(도구·대상 토큰만 `{B_LAST_ITEM}`·`{B_EFF_NAME_WITH_PREFIX}`로 교체, 같은 값) |
| 참기 축적(2턴째)·방출(3턴째) (upstream #9532 이식) | 두 턴 모두 `{공격자}은(는)\n참기를 썼다!`(`sText_AttackerUsedX`) 뒤 `STRINGID_PKMNSTORINGENERGY`(`…은(는) 참고 있다`) 또는 `STRINGID_PKMNUNLEASHEDENERGY`(`…의\n참기가 풀렸다!`) | 공격 문구 없이 `…은(는) 참고 있다` / `…의\n참기가 풀렸다!`만 출력. 방출 피해는 일반 공격 경로(명중·HP·결과 문구)로 처리하며 상성·급소 문구는 이전처럼 나오지 않음(Champions 0.5배 미만·2배 초과 포함). 받은 피해가 없으면 `…참기가 풀렸다!` → `STRINGID_BUTITFAILED` | `CancelerAttackstring()`이 `bideTurns` 중 공격 문구를 건너뛰고 `CancelerBide()`가 설정·축적·방출을 처리. `BattleScript_EffectBide`·`setbide`·`copybidedmg` 삭제, `EFFECT_BIDE`는 `BattleScript_EffectHit`, 피해는 `DoFixedDamageMoveCalc()`의 `gBideDmg × 2`. 방출 턴 연출은 HnS가 `animTurn = 1`로 유지. 문자열 ID·본문 변화 없음 |

## 상태이상·회복·잠자기

| 상황 | 이전 출력 | 현재 출력 | 변경 지점 |
| --- | --- | --- | --- |
| 특성으로 독·화상·마비·수면 발생 | 특성 유발 상태이상용 선택값 | 특성 팝업 뒤 일반 `STRINGID_PKMNWASPOISONED`, `STRINGID_PKMNWASBURNED`, `STRINGID_PKMNWASPARALYZED`, `STRINGID_PKMNFELLASLEEP` | 상태 메시지 테이블의 `B_MSG_STATUSED_BY_ABILITY` 매핑 변경 |
| 맹독구슬·화염구슬 발동 | 공통 상태이상 처리 문구 | 아이템 팝업 뒤 각각 `STRINGID_PKMNPOISONEDBY`·`STRINGID_PKMNBURNEDBY` | 두 구슬의 배틀 스크립트를 직접 상태 메시지 출력으로 분리 |
| 브레이브차지·리프레시·정글힐·초승달의기도·사이코시프트, 촉촉한몸·탈피가 상태를 치료 | `STRINGID_PKMNSTATUSNORMAL` 또는 `STRINGID_PKMNSXCUREDYPROBLEM` | 실제 치료 상태별 독·화상·마비·얼음·잠듦 문구 (`STRINGID_PKMNPOISONCURED`, `...BURN...`, `...PARALYSIS...`, `...WASDEFROSTED`, `...WOKEUP`) | 치료 전 상태를 선택값으로 보존하고 `gStatusCureStringIds`를 출력 |
| 리샘열매가 상태를 치료 | 단일 상태는 `STRINGID_PKMNSITEMCUREDPROBLEM`, 복수 상태는 `STRINGID_PKMNSITEMNORMALIZEDSTATUS` | 열매 팝업 뒤 상태별 `STRINGID_PKMNSITEMCUREDPARALYSIS`·`...CUREDPOISON`·`...HEALEDBURN`·`...DEFROSTEDIT`·`...WOKEIT`·`...SNAPPEDOUT`을 하나씩 출력 | 리샘열매 전용 상태 마스크·반복 출력 스크립트 추가 |
| `정화`가 대상의 상태이상을 치료 | `STRINGID_ATTACKERCUREDTARGETSTATUS` 하나로 대상의 문제를 치료했다고 출력 | 정화 대상의 상태에 따라 독·화상·마비·얼음·동상·잠듦 전용 문구를 출력 | `gPurifyStatusCureStringIds`를 추가하고 `BattleScript_EffectPurify`에서 `GetCuredStatusMessage()` 선택값을 `printfromtable`로 출력 |
| 잠자기 성공 | 일반 수면 또는 상태 치료 수면으로 서로 다른 ID | 항상 `STRINGID_PKMNSLEPTHEALTHY` | `gRestUsedStringIds[B_MSG_REST]`의 선택값 통일 |
| 불면·의기양양이 일반 수면 또는 잠자기를 방지 | 특성 팝업 뒤 `STRINGID_ITDOESNTAFFECT` | 특성 팝업 뒤 `STRINGID_PKMNSTAYEDAWAKEUSING` | 전용 `BattleScript_StayedAwakeUsingAbility`를 일반 수면·잠자기에 연결 |
| 스위트베일이 일반 수면·하품의 턴 종료 수면·잠자기를 방지 | 일반/하품은 `STRINGID_ITDOESNTAFFECT`, 잠자기는 보통 실패 처리 | 스위트베일 보유자 팝업 뒤 `STRINGID_PKMNSXMADEITINEFFECTIVE` | 세 수면 경로에서 특성 보유자를 팝업 주체로 설정하고 전용 종료 스크립트 또는 실패 스크립트로 연결 |
| 리프가드·리밋실드가 잠자기를 방지 | `BattleScript_AbilityPreventsRest`의 `STRINGID_BUTITFAILED`만 출력 | 해당 특성 팝업 뒤 `STRINGID_PKMNCANNOTSLEEP` (`잠들지 않는다!`) | `BS_JumpIfAbilityPreventsRest`가 능력 보유자·선택값을 설정하고 `BattleScript_StatusProtects`로 이동 |
| 상태이상 방지 특성이 독·화상·마비·잠듦을 방지 (절대안깸·리프가드·리밋실드·면역·정화의소금·유연·수의베일·수포·열교환·파스텔베일) | 모든 상태이상 실패가 특성 팝업 뒤 `STRINGID_ITDOESNTAFFECT` | 특성 팝업 뒤 상태별 `독에 중독되지 않는다!`·`화상을 입지 않는다!`·`마비되지 않는다!`·`잠들지 않는다!` | `SetStatusProtectsStringId()`, `gStatusProtectsStringIds`, `BattleScript_StatusProtects`를 공통 경로로 연결 |
| 마그마의무장이 얼음 상태를 방지 | 특성 팝업 뒤 `STRINGID_PKMNCANNOTFREEZE` (`얼지 않는다!`) | 별도 특성 팝업·`얼지 않는다!` 없이 얼음 부여만 차단 | `CanSetNonVolatileStatus()`의 `ABILITY_MAGMA_ARMOR` 분기를 `BattleScript_NotAffected`로 변경 |
| 특성에 의한 상태 회복 (면역·파스텔베일·유연·불면·의기양양·수의베일·수포·열교환·마그마의무장·자기 페이스·천진) | 특성 팝업 뒤 공통 `STRINGID_PKMNSXCUREDITSYPROBLEM` 또는 `STRINGID_PKMNSTATUSNORMAL` | 특성 팝업 뒤 실제 회복 상태별 문구, 혼란·헤롱헤롱·도발은 전용 문구 | `TryImmunityAbilityHealStatus()`, `gStatusCureStringIds`, `BattleScript_AbilityCuredStatus`, `BattleScript_BattlerGotOverItsInfatuation`, `BattleScript_BattlerShookOffTaunt` |
| 습기가 유폭을 차단 | 습기·유폭 특성 팝업 뒤 `STRINGID_PKMNSABILITYPREVENTSABILITY` | 특성 팝업 뒤 별도 텍스트 없음. 유폭 반격 피해는 계속 차단 | `BattleScript_DampPreventsAftermath`에서 해당 `printstring`·대기 제거 |

## 특성·도구·도주

| 상황 | 이전 출력 | 현재 출력 | 변경 지점 |
| --- | --- | --- | --- |
| 포이즌힐 턴 종료 회복 | 특성 팝업 뒤 `STRINGID_POISONHEALHPUP` | 특성 팝업과 HP 회복 애니메이션만 표시 | `BattleScript_PoisonHealActivates`에서 텍스트 출력 제거 |
| 솔라파워·건조피부의 햇빛 턴 종료 피해 | 특성 팝업 뒤 `STRINGID_SOLARPOWERHPDROP` | 특성 팝업과 HP 감소/기절 처리만 표시 | `BattleScript_SolarPowerActivates`에서 텍스트 출력 제거 |
| 아이스바디 턴 종료 회복 | 특성 팝업 뒤 `STRINGID_ICEBODYHPGAIN` | 특성 팝업과 HP 회복 애니메이션만 표시 | `BattleScript_IceBodyHeal`에서 텍스트 출력 제거 |
| 치유의마음이 아군의 상태이상을 치료 | 기존 공통/영문 상태 회복 문구 | 특성 팝업 뒤 `STRINGID_HEALERCURE` (`치유되었다!`) | `BattleScript_HealerActivates`와 `STRINGID_HEALERCURE` 연결 |
| 도주 특성으로 도망 성공 | `STRINGID_PKMNFLEDUSING`만 출력 | 도주 특성 팝업 → 같은 `STRINGID_PKMNFLEDUSING` | `BattleScript_RanAwayUsingMonAbility`에 팝업 호출 추가 |
| 도구가 없는 공격자에게 끈적끈적바늘 전이 | 아이템 탈취 애니메이션과 `STRINGID_STICKYBARBTRANSFER` 출력 후 전이 | 전용 메시지·아이템 탈취 애니메이션 없이 즉시 전이 | `BattleScript_StickyBarbTransfer`에서 연출·문자열 출력 제거. `StealTargetItem()`과 이후 도구 피해는 유지 |
| 야생 포켓몬 도구 드롭 | 드롭 주체를 메시지 버퍼에 확정하지 않아 실제 포켓몬 이름을 안정적으로 표시하지 못함 | `STRINGID_WILDPKMNDROPPEDITEM`이 실제 드롭 포켓몬과 `B_LAST_ITEM`을 표시, 가방 가득 참은 `STRINGID_DROPPEDITEMBAGFULL` | 드롭 배틀러를 `gBattleScripting.battler`에 저장하고 두 문자열의 이름 토큰을 `B_SCR`로 변경 |
| 야생 포켓몬의 텔레포트가 개미지옥·그림자밟기·자력에 막힘(Gen 8+) | 특성 팝업 → `STRINGID_PKMNSXMADEITINEFFECTIVE` 실패 | 해당 팝업·실패 문구 없이 정상 도주 | 텔레포트 전용 도주 불가 판정에서 세 특성 검사를 건너뜀 |
| 생명의구슬 반동(Champions 이식) | `STRINGID_HURTBYITEM` | 생명의구슬 팝업 뒤 `STRINGID_LOSTSOMEOFITSHP` | `BattleScript_LifeOrbActivates`를 전용 HP 갱신·메시지 스크립트로 분리 |
| 아이템 발동(Champions 이식) | 아이템 발동 애니메이션/효과만, 아이템 팝업 없음 | 아이템 팝업 → 기존 효과 처리 | 파워허브, 운명의매듭, 맹독·화염구슬, 특성가드, 울퉁불퉁멧, 열매, 보석, 하얀허브, 생명의구슬, 기합의띠, 선제공격손톱, 레드카드, 탈출버튼 등에 팝업 helper 연결. 본문은 고정 `STRINGID`가 아니라 아이템명 데이터 |
| 기술·특성이 독·맹독·마비·화상을 걸 때 대상의 싱크로 발동, 상태이상에 걸린 포켓몬의 상태 치료 열매(리샘열매 등) 발동 (upstream #9446 이식) | 싱크로는 기술 처리 뒤 move end 단계(`MOVEEND_SYNCHRONIZE_TARGET`/`_ATTACKER`)에서 팝업·상태 문구, 열매는 move end 도구 단계에서 발동. 광역기로 두 대상이 모두 싱크로면 하나만 반응할 수 있었음 | 같은 문구(`BattleScript_SynchronizeActivates`, 특성 팝업, 열매 팝업·치료 문구)가 상태 문구 직후(`BattleScript_UpdateEffectStatusIconRet`의 `trysynchronize` → `tryactivateitem ACTIVATION_ON_STATUS_CHANGE`)로 앞당겨져 출력. 광역기는 대상마다 싱크로 반응. 열매로 치료한 뒤 같은 공격의 다른 효과로 다시 걸리면 싱크로가 다시 반응 | `TrySynchronizeActivation()`·`Cmd_trysynchronize`, `SetNonVolatileStatus(battlerAtk, …)`, `MOVEEND_SYNCHRONIZE_*`·`ABILITYEFFECT_(ATK_)SYNCHRONIZE` 삭제. 문자열 ID 변화 없음, 출력 순서만 변경 |
| 탈출버튼·탈출팩(기술 뒤), 유턴·볼트체인지·퀵턴, 트레이너 배틀의 위기회피·도망태세로 교체 (upstream #9494 이식) | 발동 문구(`STRINGID_EJECTBUTTONACTIVATE`·특성 팝업·`STRINGID_PKMNWENTBACK`) 직후 교체 화면 → 볼 회수 → `STRINGID_SWITCHINMON`. 탈출버튼·위기회피 발동 시 공격자의 생명의구슬·조개껍질방울·옛노래 폼체인지 단계를 건너뜀 | 같은 발동 문구 → 볼 회수 → 남은 move end 출력(생명의구슬 반동 `STRINGID_LOSTSOMEOFITSHP`, 조개껍질방울, 폼체인지, 나쁜손버릇, 하양허브·편승·흉내허브, 목스프레이·과사열매·허탕보험) → 교체 화면 → `STRINGID_SWITCHINMON`. 교체 대기열이 있으면 위기회피·탈출팩·유턴은 추가로 발동하지 않음. 끝의대지·시작의바다·델타스트림 보유자가 나가면 교체 직후 날씨 해제 문구(HnS가 `BattleScript_QueuedSwitch`에 `trytoclearprimalweather` 유지, upstream 1.17.0은 생략) | `gSpecialStatuses[].queuedSwitch`, `MOVEEND_SEND_OUT_REPLACEMENTS`, `BattleScript_QueuedSwitch*`·`BattleScript_SwitchOutEffects`. `BattleScript_EjectButtonActivates`의 HnS 아이템 팝업 유지. 문자열 ID 변화 없음 |
| 레드카드로 공격자가 강제 교체된 뒤 (upstream #9494 이식) | 레드카드 뒤 move end가 유턴 다음 단계로 점프해 보유자의 위기회피·도망태세가 발동하지 않음 | 원래 공격자 관련 효과(생명의구슬·조개껍질방울·목스프레이·허탕보험·폼체인지·유턴)는 계속 생략, 레드카드 보유자의 위기회피·도망태세는 특성 팝업 뒤 교체 | `BattlerState.redCardSwitched`, `MoveEndCardButton()` 점프 삭제. 문자열 ID 변화 없음 |
| 목스프레이·과사열매·허탕보험 발동, 옛노래 폼체인지 (upstream #9494 이식) | 공격자 도구 단계에서 즉시 발동(드래곤애로는 첫 타격 뒤 허탕보험). 옛노래 폼체인지는 생명의구슬 반동 뒤 | move end 후반 `MOVEEND_SPRAY_LEPPA_BLUNDER`에서 발동(모든 타격·하양허브·흉내허브·난동 혼란 뒤). 매지션으로 빼앗은 목스프레이도 발동. 옛노래 폼체인지(`STRINGID_PKMNTRANSFORMED`)가 생명의구슬 반동보다 먼저 | `HoldEffectInfo.sprayLeppaBlunder`, `MOVEEND_FORM_CHANGE`↔`MOVEEND_LIFE_ORB_SHELL_BELL` 순서 교환. 문자열 ID 변화 없음 |

## 제외한 변경

- `battle_message.c`의 단순 한글 번역, 조사·띄어쓰기·줄바꿈·토큰 교체처럼 **같은 코드 경로가 같은 ID를 계속 출력**하는 변경.
- 1.17.0 이식 또는 PR #9777에서 새로 생겼으나, 사용자 요청으로 출력 경로를 바꾸지 않은 문자열·번역.
- 문자열이 실제로 출력되는지 확인만 하고 코드·스크립트를 바꾸지 않은 조사 기록.

## 검증 상태

각 항목은 당시 관련 오브젝트 또는 HNS 전체 빌드로 확인했으며, 다수는 실제 게임 화면 재현이 아직 남아 있다. 개별 빌드 결과·미검증 시나리오는 `STATUS.md`와 `SESSION_LOG.md`의 원 작업 기록을 따른다.
