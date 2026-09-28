# 작업 회신 — 2026-09-29 (1.17.0 full-sync 본격 port 시작 인수인계)

## 1. 목적

친구가 전달한 pokeemerald-expansion 1.17.0 full-sync 0~1단계를 로컬 한글화 작업과 통합하고, 이후 선행 이식과 정책 결정을 반영한 현재 상태를 넘긴다. 다음 담당자는 이 문서와 기존 계획을 읽은 뒤 517개 실제 port를 시작하면 된다. **517개 본격 port는 아직 시작하지 않았다.**

세부 PR 판정과 이식 순서는 이 문서가 아니라 다음 두 파일이 source of truth다.

- [전체 동기화 계획](results/pokeemerald-expansion-1.17.0-full-sync-plan.md)
- [port 순서표](results/1.17.0-sync-plan/port_sequence.tsv)

## 2. 현재 Git/브랜치 상태

- 저장소: `gorunit1/pokehns-expansion-kor`
- 작업 브랜치: `pokehns-expansion-kor`
- 이 문서 작성 전 기준 HEAD: `c339f2b7493297555fc76567b3a65a4b9db40894`
- 이 문서 작성 전 상태: working tree clean, `origin/pokehns-expansion-kor`보다 7커밋 앞섬
- 친구 작업과 로컬 후속 작업의 실제 merge-base: `dc48c4728e2b8b237d617e39933968c0fcff0871`
- 친구 작업 당시 tip: `9c59ad80bd4f0b5bcf2f60537124c75d61498949`
- 통합 merge commit: `4a7f529ceef5ea5190e8b71381d18b58f99698f8`
  - 부모 1: `e518e3fbc9a141824ad9bf33bc1de3d7225d46e1` (로컬 보존 작업)
  - 부모 2: `9c59ad80bd4f0b5bcf2f60537124c75d61498949` (친구 작업)

작업 시작 시 `git status --short --branch`가 clean인지 확인하고 `git pull --ff-only origin pokehns-expansion-kor`를 실행한다. dirty 상태면 기존 변경을 덮지 말고 중단한다.

## 3. 친구 작업 수신 후 통합한 내용

친구가 넘긴 다음 작업을 로컬 변경과 의미 단위로 병합했다.

- 2026-09-26 선별 버그 수정 35개
- 1.17.0 changelog PR 694개 전체 조사와 잔여분 재분류
- full-sync 0~1단계
- `Makefile`의 LTO=0 section GC용 `-ffunction-sections -fdata-sections`
- 인접 트레이너 방향 고정 후속 수정
- 전체 계획, 그룹별 계획, `port_sequence.tsv`

통합 과정에서 충돌한 `docs/localization/STATUS.md`와 `SESSION_LOG.md`는 양쪽 기존 기록을 모두 보존해 병합했다. 코드·그래픽 충돌은 없었다. 원래 친구 작업의 상세는 [2026-09-28 회신](HANDBACK_2026-09-28.md)에 있다.

## 4. 이후 로컬 작업

`origin/pokehns-expansion-kor` 기준 로컬 전용 커밋은 다음 7개였다. 추측이 아니라 실제 `git log origin/pokehns-expansion-kor..HEAD`와 각 commit diff를 기준으로 한 목록이다.

| 커밋 | 내용 |
|---|---|
| `e518e3fbc9` | 통합 전 로컬 변경 보존: Pokegear/Pokenav 그래픽, 주인공 이름 3글자 제한, `all_learnables.json`, NPC 외 텍스트 조사와 관련 상태 기록 |
| `4a7f529cee` | 친구 full-sync 0~1단계와 위 로컬 작업 통합 |
| `01913c9476` | 한국어 박스 이름 입력을 8글자/16바이트 저장 공간으로 확장하고 라이벌 이름 제한을 3글자로 복원 |
| `c327319589` | Pokegear 헤더 조각 패킹·정렬 보정 및 Gen4 체력박스의 메가 아이콘/레벨 100 겹침 수정 |
| `8280caf163` | #10429 ZWS를 HNS 한글 인코딩에 맞춰 선행 이식 |
| `b63ee21152` | #8943 recorded battle 형식을 A안으로 확정 |
| `c339f2b749` | #10268과 #9819를 A안으로 확정 |

`e518e3fbc9`에는 `docs/localization/NON_NPC_TEXT_AUDIT.md`와 번역된 Pokegear/Pokenav PNG·BIN도 포함된다. 이 파일들과 HNS 전용 동작을 upstream 버전으로 일괄 덮어쓰지 않는다.

## 5. #10429 HNS 선행 이식

`#10429 Add Zero-Width-Space and fix some line break handling`은 `8280caf16314c52039b44bd0b67ee97eebbdb53c`에서 선행 이식했다.

- upstream `CHAR_ZWS = 0x3A`는 HNS 2바이트 한글의 선두·후속 바이트와 충돌하므로 사용하지 않았다.
- HNS는 `0x42`를 **비일본어 문자열에서 독립된 단일 바이트일 때만** ZWS로 사용한다.
- 문자열 순회를 논리 글자 단위로 보강해 한글 후속 바이트 `0x42`, `0x3A`, `0xAE`가 ZWS 또는 하이픈으로 오인되지 않게 했다.
- 일본어 모드의 `ぢ = 0x42`는 기존 글리프로 유지한다.
- 기존 문자열 리소스 재인코딩은 0건임을 확인했다.
- 충돌 후보 한글을 정적 검사했고, 사용자 mGBA 확인에서 최소 `겠`은 정상 표시됐다.
- #10301은 아직 미이식이다. 아이템 설명에서 실제 ZWS를 생성하는 최종 경로는 #10301 이식 후 실기 검증해야 한다.

`port_sequence.tsv`에서 #10429(현재 seq 291)에 도달해도 다시 이식하지 않는다. 현재 코드를 검증하고 `already applied / HNS-adapted`로 처리한다. #10301(현재 seq 289)을 이식할 때 현재 `0x42` 구현과 연결하고 다음을 확인한다.

- 하이픈 뒤 불필요한 공백이 표시되지 않는지
- 좁은 폭에서는 ZWS 위치에서 줄바꿈되는지
- 긴 한글 아이템 설명의 글자 중간이 분리되지 않는지

## 6. 확정된 외부결정

세부 근거와 다른 선택지는 [full-sync plan 5절](results/pokeemerald-expansion-1.17.0-full-sync-plan.md#5-외부결정-6건-결정-완료)을 따른다.

- **#8943 — A안** (`b63ee2115262db27496782f154448f23fdbf4440`)
  - upstream 1.17.0 `RecordedBattleSave` 구조를 사용한다.
  - 기존 HNS 구형 녹화 배틀의 호환은 보장하지 않으며 무효화를 허용한다.
  - 일반 게임 세이브 호환성은 유지한다.
  - 구형 녹화 형식 compatibility layer는 만들지 않는다.
  - upstream 정합성과 후속 유지보수를 우선한다.
- **#10144 — C안:** upstream 판정·처리 순서와 버그 수정은 이식하되 HNS의 한국어 효과 없음 메시지 정책과 문구를 유지한다.
- **#10151 — B안 / `GEN_9`:** HNS 현재 동작과 밸런스를 유지한다.
- **#10454 — B안 / `GEN_8`:** HNS 현재 날씨 동작을 유지한다.
- **#10461:** HNS의 `<=` 경계를 유지하고 나머지 구조 개선만 필요한 범위에서 이식한다.
- **#9920:** 기존 `saveVersionMagic`을 이동하지 않고 `SaveBlock3` 끝에 `u32 dailySeed`를 추가한다.

## 7. 확정된 6절 세부 결정

- **#10268 — A안** (`c339f2b7493297555fc76567b3a65a4b9db40894`)
  - 열매 발동 시 upstream 아이템 팝업을 사용한다.
  - 실제 코드는 `port_sequence.tsv`의 해당 순서에서 이식한다.
- **#9819 — A안** (같은 결정 커밋)
  - upstream #9819를 그대로 이식한다.
  - 현재 `IndigoPlateau_hns`와 `Route23_hns`에 `MB_ROCK_CLIMB` 실제 배치는 0개라 현재 플레이 동선은 바뀌지 않는다.
  - `OW_ROCK_CLIMB_FIELD_MOVE = FALSE`의 의미와 상호작용을 일치시키고 후속 #10548의 구조를 따른다.

나머지 6절 항목은 [full-sync plan 6절](results/pokeemerald-expansion-1.17.0-full-sync-plan.md#6-기본-처리-규칙이-필요한-세부-항목)의 HNS 유지·적응 방침을 그대로 source of truth로 삼는다.

## 8. 빌드/실기 상태

- #10429 적용 직후 clean build 기록: 종료 코드 0, ROM 32,724,180 B(97.53%), EWRAM 249,112 B(95.03%), IWRAM 25,644 B(78.26%). 적용 직전보다 ROM +528 B이고 ZWS 관련 신규 warning은 0건이었다.
- 이 handoff 작성 뒤 최종 HEAD에서 `rm -rf build/hns && make hns -j8` clean build를 다시 수행했다. 종료 코드 0, ROM 32,724,180 B(97.53%), EWRAM 249,112 B(95.03%), IWRAM 25,644 B(78.26%), ROM SHA-1 `2897f6a3f501cbe9cd7393feb37c89364919223e`다.
- warning diagnostic은 164건이며 기존 `-Woverride-init`·미사용 함수/변수 계열이다. #10429 관련 파일(`charmap.txt`, `characters.h`, `line_break.c`, `text.c`)의 신규 warning은 0건이다.
- #10429 한글 회귀 실기 확인: 사용자 확인으로 `겠` 정상 표시.
- 친구가 요청했던 기본 실기 항목은 사용자가 대체로 확인했지만, 항목별 완전 통과 기록은 없다. 전부 통과했다고 간주하지 않는다.
- 기존 회귀 체크리스트는 [HANDBACK_2026-09-28.md의 실기 확인 필요](HANDBACK_2026-09-28.md#실기-확인-필요-아직-안-함)와 [HANDBACK_2026-09-26.md의 실기 확인 항목](HANDBACK_2026-09-26.md#실기-확인이-필요한-항목)을 계속 사용한다. 관련 코드를 다시 건드리면 재검증한다.
- 로컬 후속 작업 중 Pokegear 헤더와 메가 아이콘/레벨 100 수정은 빌드·정적 검증을 통과했지만 각 최신 수정본의 mGBA 최종 확인 기록은 완전하지 않다.

## 9. 517개 port 현재 위치

- changelog 1.15.2~1.17.0 upstream PR 694개 조사 완료
- 2026-09-26 현존 버그 수정 35개 선별 이식 완료
- 남은 594개 재분류 완료
- 이식 대상 517개 + 테스트 러너 복구 1개 확정
- 동등 56개
- 무관 15개
- 외부결정 6개(현재 모두 결정 완료)
- dependency 기준 `port_sequence.tsv` 작성 완료
- 일부 선행 변경과 #10429 HNS 맞춤 이식 완료
- **517개 본격 대규모 port는 시작 전**

다음 시작점은 [port_sequence.tsv](results/1.17.0-sync-plan/port_sequence.tsv)의 seq 1 `U-testrunner-fix`다. 단, 각 항목을 적용하기 전에 current history와 코드를 확인해 이미 적용된 PR 또는 HNS 선행 이식인지 판정한다.

## 10. 앞으로의 port 원칙

1. 시작 전 `git pull --ff-only origin pokehns-expansion-kor`를 실행한다.
2. working tree가 clean하지 않으면 기존 변경을 덮지 말고 중단한다.
3. `port_sequence.tsv` 순서를 기본으로 한다.
4. 이미 적용된 PR은 history/current code를 검증한 뒤 skip/equivalent로 기록한다.
5. #10429의 현재 HNS 맞춤 구현을 보존한다.
6. 외부결정과 6절 항목은 full-sync plan의 확정안을 따른다.
7. HNS의 한글 문자열, `STRINGID`, `{B_...}` 포맷, 메시지 출력 정책, HNS config, 한국어 인코딩을 upstream으로 일괄 덮어쓰지 않는다.
8. 무작정 cherry-pick하지 않고 현재 HNS diff를 읽어 의미 단위로 port한다.
9. 작은 dependency unit 단위로 적용한다.
10. 각 unit 후 최소 `git diff --check`, HNS build, 관련 자동 테스트를 수행한다.
11. 큰 battle/refactor 단위 뒤에는 mGBA 체크리스트를 작성한다.
12. 관련 없는 formatting을 하지 않는다.
13. 새로운 HNS 정책 결정이 필요하면 임의 결정하지 말고 handoff에 기록해 사람에게 질문한다.
14. upstream `PokemonHnS-Development`에는 push하거나 PR을 만들지 않는다.
15. 작업과 push 대상은 `gorunit1/pokehns-expansion-kor`의 `pokehns-expansion-kor` 브랜치다.

## 11. 읽어야 할 기존 문서

1. 이 문서
2. [2026-09-26 작업 회신](HANDBACK_2026-09-26.md)
3. [2026-09-28 full-sync 0~1단계 회신](HANDBACK_2026-09-28.md)
4. [전체 동기화 계획](results/pokeemerald-expansion-1.17.0-full-sync-plan.md)
5. [port 순서표](results/1.17.0-sync-plan/port_sequence.tsv)
6. 작업할 PR의 [그룹별 계획 디렉터리](results/1.17.0-sync-plan/)
7. 필요할 때 [STATUS](../localization/STATUS.md)와 [SESSION_LOG](../localization/SESSION_LOG.md)

## 12. Claude 실행 지시문 위치

친구가 Claude Code/Claude에 바로 전달할 실행 지시문은 [CLAUDE_FULL_SYNC_PORT_PROMPT.md](CLAUDE_FULL_SYNC_PORT_PROMPT.md)에 있다.

## 13. 금지사항

- 이미 적용된 PR 또는 #10429 재이식
- HNS 한글 문자열·인코딩·메시지 정책 일괄 덮어쓰기
- `reset --hard`, `clean`, rebase, force push
- dirty working tree의 제3자 변경 삭제
- 결정되지 않은 HNS 정책 임의 확정
- upstream `PokemonHnS-Development` push/PR
- dependency를 무시한 517개 일괄 cherry-pick
