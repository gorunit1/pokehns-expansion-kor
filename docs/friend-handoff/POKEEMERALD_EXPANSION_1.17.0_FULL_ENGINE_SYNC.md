# pokeemerald-expansion 1.17.0 전체 엔진 동기화 작업 지시서

## 작업 목표

최종 목표는 **HNS를 `pokeemerald-expansion` `expansion/1.17.0`과 엔진 동작 기준으로 동기화**하는 것이다. HNS는 단순한 upstream 포크가 아니라 **HNS 한글패치 + 최신화된 엔진**이어야 한다.

2026-09-26의 묶음 A는 694개 upstream PR을 조사한 뒤 충돌 위험이 낮은 35개만 선별 이식한 첫 단계다. 이 작업은 그 지점에서 멈추지 않는다. 현재 브랜치·기준 태그·실제 코드 차이를 다시 대조하여 남은 engine-relevant upstream PR과 필요한 의존 변경을 모두 이식한다.

이 목표는 upstream 전체 merge 또는 파일 덮어쓰기를 뜻하지 않는다. HNS의 기능·한글화·사용자 지정 배틀 메시지 동작을 보존한 **기능 단위 적응 이식**으로 1.17.0의 엔진 동작을 완성하는 작업이다.

## 반드시 먼저 읽을 문서

1. [`AGENTS.md`](../../AGENTS.md)
2. [`CURRENT_HNS_HANDOFF.md`](CURRENT_HNS_HANDOFF.md)
3. [`POKEEMERALD_EXPANSION_1.17.0_UPDATE.md`](POKEEMERALD_EXPANSION_1.17.0_UPDATE.md) — 기존 보존 규칙과 결과 양식
4. [`HANDBACK_2026-09-26.md`](HANDBACK_2026-09-26.md) — 묶음 A의 실제 이식 범위·보류 사항
5. [`results/pokeemerald-expansion-1.17.0-update-report.md`](results/pokeemerald-expansion-1.17.0-update-report.md) 및 `results/1.17.0-inventory/`
6. [`docs/localization/STATUS.md`](../localization/STATUS.md), [`docs/localization/SESSION_LOG.md`](../localization/SESSION_LOG.md)
7. [`docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`](../localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md)

문서의 과거 판정은 출발점일 뿐이다. 최종 판정은 반드시 현재 HNS 코드와 `expansion/1.17.0` 태그를 직접 비교해 내린다.

## 절대 보존할 것

- 현재 한글 문장 본문, 줄바꿈, 제어 코드, `{B_...}` 지정 코드, 조사 토큰을 임의로 바꾸지 않는다.
- `src/battle_message.c` 및 연결된 배틀 메시지 최신화 로직(특성 팝업 순서, 상태별 실패·회복 문구, 도주/텔레포트, 장벽 제거 등)을 upstream 파일로 덮어쓰지 않는다.
- HNS 전용 종·폼·메가진화·그래픽·맵·이벤트·charmap·한글 폰트·저장 호환 처리·설정 기본값을 임의로 삭제하거나 upstream 값으로 바꾸지 않는다.
- upstream에서 새 문자열이 필요할 때만 새 ID/문자열을 추가한다. 기존 한글 문자열의 영문화·재번역·문장 수정은 금지한다. 공식 한국어 근거가 없는 신규 문장은 임의 창작하지 않고 미결로 기록한다.
- `git reset --hard`, 일괄 `checkout`, `clean`, 전체 merge/rebase, 파일 통째 복사는 금지한다. 기존 변경이 있는 작업 트리를 자신의 변경으로 취급하거나 되돌리지 않는다.

## 완료 기준

다음 조건을 모두 만족해야 “전체 엔진 동기화”가 완료다.

- [ ] 1.15.2 개발 계열 이후부터 `expansion/1.17.0`까지의 모든 upstream PR/커밋을 현재 코드 기준으로 다시 대조했다.
- [ ] 각 항목이 `적용`, `현재 HNS에 동등하게 존재`, `HNS에 기능상 무관`, `명시적 외부 결정 필요` 중 하나로 확정되어 있다. 단순히 `나중에 가능`, `선행 필요`, `충돌`로 방치하지 않는다.
- [ ] engine-relevant 항목은 의존성을 포함해 모두 기능 단위로 이식했다. 대형 리팩터가 필요하면 그 리팩터와 후속 PR까지 같은 계획으로 완료한다.
- [ ] HNS 고유 구현과 충돌한 항목은 현재 동작을 보존하면서 upstream의 버그 수정/기능 목적을 만족하도록 적응했다.
- [ ] 기존 한글화와 배틀 메시지 출력 최신화가 보존됐음을 diff와 실제 경로로 확인했다.
- [ ] 각 이식 단위의 정적 검토, `git diff --check`, 영향 오브젝트 빌드, `make hns -j8` 결과가 기록됐다.
- [ ] 주요 배틀·AI·필드·저장 기능은 새 ROM과 **게임 내 저장**으로 실제 검증됐다. savestate로 ROM 간 검증하지 않는다.
- [ ] `STATUS.md`, `SESSION_LOG.md`, `docs/friend-handoff/results/`의 인벤토리·결과·미결 항목이 최신 상태다.

`명시적 외부 결정 필요`는 HNS의 사용자 지정 동작을 바꾸는 선택이 정말 필요한 경우에만 허용한다. 이 경우에도 정확한 upstream PR, 파일/함수, HNS 동작, 두 선택지와 영향을 기록하여 작업자에게 넘긴다. 단순한 코드 충돌은 중단 사유가 아니라, HNS 구조에 맞춰 해결할 과제다.

## 작업 순서

### 0. 시작 상태와 묶음 A 재검증

1. 현재 브랜치·커밋·`git status --short`·upstream 태그·merge-base를 기록한다.
2. 2026-09-26 묶음 A 35건이 현재 브랜치에 실제로 존재하는지 확인한다.
3. `results/1.17.0-inventory/*.tsv`의 후행 탭을 제거하여 전체 변경 범위의 `git diff --check`가 통과하게 한다. 이는 문서 형식 정리이며 판정 데이터는 바꾸지 않는다.
4. 깨끗한 worktree에서 `make hns -j8`을 실행하여 기준 ROM, EWRAM/IWRAM/ROM 사용량, SHA1을 기록한다.

### 1. 전체 인벤토리 재확정

1. 1.15.2~1.17.0의 694개 목록과 실제 commit range를 현재 HNS에 대조한다.
2. 묶음 A 이후 상태를 반영해 각 항목의 현재 상태·의존 PR·충돌 파일·테스트 경로를 갱신한다.
3. 이미 동등한 수정이 HNS에 있으면 해당 코드 위치와 upstream 목적을 기록한다. PR 번호만 보고 “이미 적용”으로 추정하지 않는다.
4. 문서·CI·지원 플랫폼 전용 변경은 엔진 동작에 무관한 근거를 남겨 `무관`으로 확정할 수 있다. 단, 빌드·저장·그래픽 변환·툴체인에 영향을 주는 변경은 무관으로 처리하지 않는다.

### 2. 의존성 순서로 기능 단위 이식

1. 기반 리팩터·공통 API·데이터 구조를 먼저 이식하고, 그 위의 버그 수정·기능을 이어서 이식한다.
2. PR 하나가 대형이면 관련 PR 묶음을 하나의 검증 단위로 삼되, 커밋·문서에서는 원래 PR별 반영 여부를 추적한다.
3. HNS와 upstream이 같은 파일을 바꾸었으면 파일 전체를 교체하지 말고 함수·필드·스크립트 명령 단위로 목적을 대조한다.
4. 새 upstream API/상수/구조체를 도입할 때는 모든 호출자, 초기화, 저장 구조 영향, ASM 매크로, 그래픽·데이터 생성 규칙까지 함께 이식한다.
5. 저장 데이터 구조 또는 ROM 레이아웃 영향이 있으면 기존 게임 내 저장 호환성·새 저장 모두를 확인하고, 복구/마이그레이션이 필요하면 별도 단위로 구현한다.

### 3. 한글화·배틀 메시지 겹침 처리

upstream 변경이 배틀 메시지·상태·특성·아이템·기술 스크립트와 겹치면 다음 순서를 지킨다.

1. upstream 변경의 실제 게임 효과와 출력 조건을 먼저 확인한다.
2. HNS의 현재 메시지 ID·특성 팝업·플레이스홀더·조사 토큰을 보존한 채 같은 엔진 효과만 이식한다.
3. upstream이 새 메시지를 요구할 때 기존 문구를 교체하지 말고 새 경로/ID를 추가한다. 공식 한국어 원문이 없으면 이식은 코드까지 진행하되 문구는 미결 목록에 남긴다.
4. `battle_message.c`와 배틀 스크립트는 hunk별 diff와 실제 호출 경로를 함께 기록한다.

### 4. 검증

각 단위에서 다음을 수행한다.

1. `git diff --check`
2. 영향받은 C/ASM/데이터 오브젝트 빌드
3. HNS 전체 빌드: `GITHUB_ACTION=1 make --jobserver-style=pipe hns -j8`
4. ROM/EWRAM/IWRAM 한계와 새 경고를 기준 빌드와 비교
5. 해당 기능의 자동 테스트가 HNS에서 가능하면 실행하고, 불가능하면 이유를 기록
6. 실제 ROM에서 최소 재현을 실행하고 코드 검증·빌드 검증·실기 검증을 분리 기록

우선 실기 검증 목록:

- 전자부유·레이저포커스·바톤터치·중력·떨어뜨리기
- 메가진화·테라스탈·울트라버스트 뒤 하양허브·편승·흉내허브·탈출팩
- 미래예지의 열매·저주받은바디 반응, 만능우산과 충전 기술, 절대영도, 코트체인지
- 더블배틀 AI·더블 재대결·트레이너 발견/접근·동반 포켓몬·멀티배틀 뒷모습
- PC·파티·박스·새 저장과 기존 게임 내 저장의 진입 및 저장

## 결과 기록과 Git 규칙

- `docs/friend-handoff/results/`에 PR별 또는 의존성 묶음별 결과를 남긴다. 변경 파일, 목적, HNS 적응 내용, 보존한 한글화, 검증, 남은 위험을 기록한다.
- 작업 단위마다 `docs/localization/STATUS.md`와 `SESSION_LOG.md`를 갱신한다.
- 논리 단위로 커밋한다. `git add .`를 쓰지 말고 실제 변경 파일을 명시적으로 추가한다.
- 원본 `PokemonHnS-Development`에는 push·Pull Request·merge를 만들지 않는다. Fork `gorunit1/pokehns-expansion-kor`의 지정 브랜치에 push하는 것은 작업 의뢰자가 명시적으로 허용한 경우에만 한다.

## 결과 보고 양식

```markdown
## 동기화 단위: <upstream PR/커밋 또는 의존성 묶음>

- 현재 판정: 적용 / HNS 동등 구현 / 무관 / 외부 결정 필요
- upstream 근거:
- 해결한 의존성:
- 수정 파일:
- HNS 적응과 보존한 한글화/배틀 메시지 동작:
- 저장·ROM·그래픽 영향:
- 검증:
  - 코드 대조:
  - `git diff --check`:
  - 영향 오브젝트:
  - `make hns -j8`:
  - 자동 테스트:
  - 실제 ROM:
- 다음 단위 또는 외부 결정 필요 사항:
```
