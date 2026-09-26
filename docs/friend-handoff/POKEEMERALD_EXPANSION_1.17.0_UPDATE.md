# pokeemerald-expansion 1.17.0 안전 업데이트 작업 지시서

## 목표

현재 HNS는 `pokeemerald-expansion` 1.15.2 개발 사이클(직전 정식 릴리스 1.15.1)을 기반으로 한다. 공식 `expansion/1.17.0` 태그까지 추가된 upstream 변경을 HNS에 적용한다.

이 작업은 upstream 내용을 그대로 덮어쓰는 병합이 아니다. HNS 고유 기능, 한글화, 배틀 메시지 최신화 변경을 보존하면서 upstream의 미반영 기능·수정만 기능 단위로 이식하는 업데이트다.

## 반드시 먼저 읽을 문서

- [`AGENTS.md`](../../AGENTS.md) — 저장소 작업 원칙과 HNS 빌드·인수인계 규칙
- [`docs/localization/STATUS.md`](../localization/STATUS.md) — 현재 기반 버전, 이미 선별 이식한 1.17.0 항목, 미검증 사항
- [`docs/localization/SESSION_LOG.md`](../localization/SESSION_LOG.md) — 2026-09-15~17의 1.17.0 이식·충돌·회귀 복구 이력
- [`docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`](../localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md) — 사용자 요청으로 변경된 배틀 메시지 출력 동작 목록
- [`docs/localization/BATTLE_MESSAGE_KR_COMPARE.md`](../localization/BATTLE_MESSAGE_KR_COMPARE.md) — 배틀 메시지 한글화·지정 코드 대조 기록

## 기준 소스

- HNS 작업 대상: 현재 `pokehns-expansion` 작업 트리
- upstream: [`rh-hideout/pokeemerald-expansion`](https://github.com/rh-hideout/pokeemerald-expansion)
- 목표 태그: [`expansion/1.17.0`](https://github.com/rh-hideout/pokeemerald-expansion/releases/tag/expansion/1.17.0), 태그 커밋 `e8bd1cd`
- 현재 HNS 기반 확인: `include/constants/expansion.h` 및 Git 계보. 현재 문서상 기반은 1.15.2 개발 사이클이며, 1.15.1 이후의 모든 upstream 변경이 자동으로 미적용 상태라는 뜻은 아니다.

upstream 대조는 별도 임시 clone 또는 읽기 전용 remote에서 수행한다. HNS 작업 트리에 upstream 전체를 무차별 merge, rebase, checkout 또는 덮어쓰지 않는다.

## 절대 보존할 사항

### HNS·한글화·자산

- 현재 HNS에 추가된 파일과 수정 사항을 임의로 삭제·되돌림·교체하지 않는다. HNS와 upstream이 다르면 HNS 내용을 우선한다.
- 현재 한글화된 문장 본문, 줄바꿈, 제어 코드, `{B_...}` 지정 코드, 조사 토큰을 임의로 수정하지 않는다.
- upstream에서 새 문자열·기능이 필요해 새 텍스트를 추가해야 할 때만 새 ID와 문자열을 추가한다. 기존 한글 문자열을 영어나 upstream 문장으로 교체하지 않는다.
- 공식 한국어 원문이 없어 새 문장을 신뢰성 있게 추가할 수 없으면 임의 번역하지 말고 미결 항목으로 기록한다.
- 한글 폰트·charmap·조사 처리·HNS 전용 맵/이벤트/그래픽/종 데이터/메가진화 자산을 upstream 파일로 교체하지 않는다.

### 배틀 메시지와 최신화 코드

아래 파일과 연결된 테스트·헤더는 특히 높은 충돌 위험 영역이다. upstream 전체 파일로 교체하지 않고, 필요한 hunk를 현재 HNS 구현과 대조해 기능 단위로만 반영한다.

- `src/battle_message.c`
- `data/battle_scripts_1.s`, `data/battle_scripts_2.s`
- `src/battle_util.c`, `src/battle_script_commands.c`, `src/battle_hold_effects.c`, `src/battle_end_turn.c`
- `include/battle*.h`, `include/constants/battle_string_ids.h`, `asm/macros/battle_script.inc`
- `test/battle/**`
- `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`

특히 기술·특성·도구 효과에 따라 표시되는 메시지, 특성 팝업 순서, 상태별 메시지 선택, 도주/텔레포트, 장벽 제거, 상태 회복의 현재 HNS 동작은 보존한다. upstream 변경이 같은 코드 영역을 건드리면 먼저 충돌 보고를 작성하고, 사용자의 명시적 결정 전에는 해당 hunk를 바꾸지 않는다.

## 이미 반영했거나 선별 검토된 1.17.0 범위

`STATUS.md`와 `SESSION_LOG.md`에 따라 다음은 이미 일부 또는 전부 선별 이식·검토된 이력이 있다. 다시 일괄 적용하지 말고 현재 코드와 upstream 태그를 직접 비교해 실제 미반영 부분만 판정한다.

- PR #9878 — 알 생성·부화 및 `givemon` IsEgg
- PR #10058 — `GEN_CHAMPIONS` 및 기술 데이터
- PR #10151, #10257 — Champions 배틀 메커니즘
- PR #10025 — 상대 HP 백분율 표시
- PR #10121 — 디버그 메뉴
- PR #10416 — 메가솔 특성 팝업
- PR #10426 — 흡수 리팩터링
- PR #10561 — 메가찌르호크 아이콘
- PR #10601 — 포켓덱스 플러스 진화 텍스트 정렬
- PR #10603 — Z-A 메가 몸색
- 1.17.0 릴리스의 Items, Battle AI, Fixed 항목 일부

이 목록은 “완전 동일”을 보장하지 않는다. 문서가 불충분한 경우 각 PR/커밋과 현재 HNS 코드를 비교해 적용·부분 적용·미적용을 판정한다.

## 작업 절차

1. **사전 상태 기록**
   - 현재 브랜치, `git status --short`, `include/constants/expansion.h` 버전을 기록한다.
   - 기존 작업 트리가 더럽다면 그 변경을 자신의 변경으로 취급하거나 되돌리지 않는다.
   - 시작 전 상태를 이 문서와 별도 결과 문서에 기록한다.

2. **upstream 변경 인벤토리 작성**
   - 1.15.1 이후부터 `expansion/1.17.0`까지의 upstream 릴리스·PR·커밋을 기능 영역별로 분류한다.
   - 각 항목을 `이미 적용`, `부분 적용`, `미적용·안전 이식 가능`, `충돌`, `HNS와 무관`으로 표시한다.
   - 이미 반영된 항목을 다시 가져오지 않는다.

3. **작은 단위로 이식**
   - 독립적인 upstream 기능 또는 수정 하나씩만 적용한다.
   - 파일 전체 복사, 대규모 자동 치환, upstream 브랜치의 일괄 merge/rebase는 금지한다.
   - 변경 전후 diff에서 기존 한글 문자열과 HNS 전용 코드를 보존했는지 확인한다.
   - 새 API·상수·데이터를 추가할 때는 그 기능이 실제로 호출되는 경로와 필요한 모든 의존성을 함께 확인한다.

4. **충돌 처리**
   - 충돌 또는 의미가 불명확한 차이가 나오면 즉시 해당 hunk 적용을 중단한다.
   - 다음을 결과 문서에 남긴 뒤 사용자에게 보고한다.
     - upstream PR/커밋과 파일·함수·라인
     - HNS의 현재 동작과 보존해야 하는 이유
     - upstream 변경의 목적
     - 가능한 선택지와 예상 영향
   - 사용자 지시 없이 HNS 쪽을 upstream으로 교체하거나 임의 절충안을 만들지 않는다.

5. **검증**
   - 변경 단위마다 `git diff --check`를 실행한다.
   - 영향받은 C·ASM·테스트 오브젝트를 먼저 빌드한다.
   - 단위가 끝나면 HNS 전체 빌드 `make hns -j8`을 실행한다. 기본 `make` 성공은 HNS 검증이 아니다.
   - 코드 대조, 빌드 성공, 자동 테스트, 실제 HNS 화면/플레이 검증을 구분해 기록한다.
   - ROM/EWRAM/IWRAM 한계와 32MiB 제약을 확인한다.

6. **문서화와 커밋**
   - 각 기능 단위 완료 또는 보류 때 `docs/localization/STATUS.md`와 `docs/localization/SESSION_LOG.md`를 갱신한다.
   - 아래 결과 문서를 유지한다. `docs/friend-handoff/results/pokeemerald-expansion-1.17.0-update-report.md`
   - 사용자가 명시하지 않는 한 원격 push, Pull Request 생성, 원본 저장소 merge는 하지 않는다.

## 금지 사항

- `git reset --hard`, 일괄 `git checkout`, `git clean`, upstream 전체 merge/rebase
- HNS·한글화·배틀 메시지 최신화 코드의 임의 삭제 또는 upstream 내용으로 덮어쓰기
- 기존 한글 문자열의 임의 수정·영문화·재번역
- 충돌 hunk의 독단적 해결
- 빌드 산출물 또는 대량 생성 파일을 변경 이유 확인 없이 추가·삭제

## 완료 조건

- [x] 1.15.1 이후~1.17.0 upstream 변경의 인벤토리가 작성되었다.
- [x] 각 항목의 적용/부분 적용/미적용/충돌 상태와 근거가 기록되었다.
- [ ] 안전한 미적용 항목만 기능 단위로 이식되었다.
- [ ] HNS·한글화·배틀 메시지 최신화 변경이 보존되었다.
- [ ] 충돌은 해결하지 않은 채 정확한 위치와 선택지를 사용자에게 보고했다.
- [ ] 영향 범위 테스트와 `make hns -j8` 결과가 기록되었다.
- [ ] `STATUS.md`, `SESSION_LOG.md`, 결과 문서가 최신 상태다.

## 결과 보고 양식

```markdown
## 적용 단위: <upstream PR/커밋 또는 기능명>

- 판정: 적용 / 부분 적용 / 보류(충돌) / 이미 적용 / 무관
- upstream 근거: <PR, 커밋, 태그>
- 수정 파일:
- 보존한 HNS·한글화 차이:
- 충돌 여부와 상세:
- 검증:
  - 코드 대조:
  - 대상 빌드:
  - `make hns -j8`:
  - 자동/실기 테스트:
- 남은 문제 및 사용자 결정 필요 사항:
```
