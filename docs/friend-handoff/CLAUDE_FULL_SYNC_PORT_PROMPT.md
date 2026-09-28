# Claude 실행 지시문 — pokeemerald-expansion 1.17.0 full-sync 실제 port

아래 지시를 따라 `gorunit1/pokehns-expansion-kor` 저장소의 `pokehns-expansion-kor` 브랜치에서 pokeemerald-expansion 1.17.0 full-sync 실제 port를 시작해라. 0~1단계 조사와 이식 계획은 끝났고, 517개 본격 port는 아직 시작 전이다.

## 1. 시작

1. 현재 브랜치와 작업 트리를 확인한다.

   ```bash
   git status --short --branch
   ```

2. 브랜치가 `pokehns-expansion-kor`인지 확인한다. working tree가 clean하지 않으면 기존 변경을 수정·삭제·stash하지 말고 파일 목록을 보고한 뒤 중단한다.
3. clean일 때만 다음을 실행한다.

   ```bash
   git pull --ff-only origin pokehns-expansion-kor
   ```

4. pull 후 아래 필수 문서를 읽고 현재 Git history와 실제 코드를 함께 확인한다. 문서만 믿고 이미 적용된 코드를 재적용하지 않는다.

## 2. 필수 읽기 문서

1. `docs/friend-handoff/HANDBACK_2026-09-29_FULL_SYNC_PORT_START.md`
2. `docs/friend-handoff/HANDBACK_2026-09-26.md`
3. `docs/friend-handoff/HANDBACK_2026-09-28.md`
4. `docs/friend-handoff/results/pokeemerald-expansion-1.17.0-full-sync-plan.md`
5. `docs/friend-handoff/results/1.17.0-sync-plan/port_sequence.tsv`
6. 현재 작업할 PR이 속한 `docs/friend-handoff/results/1.17.0-sync-plan/*_plan.md`와 대응 TSV
7. 필요한 경우에만 `docs/localization/STATUS.md`, `docs/localization/SESSION_LOG.md`, `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`

full-sync 전체 판정·정책은 `pokeemerald-expansion-1.17.0-full-sync-plan.md`, 정확한 적용 순서는 `port_sequence.tsv`, PR별 적응 근거는 해당 group plan을 source of truth로 사용한다.

## 3. 실제 port 방법

1. `port_sequence.tsv`의 seq 1 `U-testrunner-fix`부터 순서대로 진행한다.
2. 각 행을 시작하기 전에 PR 번호·upstream commit·현재 history·현재 코드를 대조한다.
3. 이미 적용됐거나 동등 구현이 있으면 재적용하지 말고 `already applied`, `HNS-adapted`, 또는 `equivalent` 근거를 기록한다.
4. 같은 dependency unit은 작은 검증 가능한 단위로 묶되, `deps`와 upstream 순서를 지킨다.
5. upstream commit의 의도는 유지하되 HNS의 기존 한글화와 커스텀 동작을 보존한다.
6. 단순 cherry-pick 후 `ours`/`theirs`로 파일 전체를 고르지 않는다. upstream diff와 HNS diff를 읽고 의미 단위로 이식한다.
7. 다음을 upstream 버전으로 무조건 덮어쓰지 않는다.
   - 한글 문자열과 한국어 인코딩
   - `STRINGID` 매핑
   - `{B_...}` 배틀 메시지 포맷과 조사 처리
   - HNS 배틀 메시지 출력 정책
   - HNS-specific config와 현재 세대 판정
   - Pokegear/Pokenav, 작명, storage 등 HNS 로컬 변경
8. 각 unit 후 최소한 다음을 수행한다.

   ```bash
   git diff --check
   make hns -j8
   ```

   관련 자동 테스트가 있으면 메시지 문자열 단정과 로직 단정을 구분해 실행한다. HNS 테스트 러너 복구 후에는 `make check BUILD=hns`를 사용하되, 영문 `MESSAGE` 기대값 때문에 생기는 실패를 로직 실패와 혼동하지 않는다.
9. ROM/EWRAM/IWRAM 변화, warning, 테스트 결과, HNS 적응 내용을 unit별 결과 문서에 기록한다.
10. 큰 battle/refactor 묶음(#9655, #8943, #9730, #9939, canceler chain 등)이 끝날 때마다 mGBA 실기 체크리스트를 작성한다.
11. 독립적인 작은 unit마다 검증 가능한 커밋을 만들고, 큰 묶음이 끝나면 handoff와 진행 상태를 갱신한다. 관련 없는 formatting은 하지 않는다.
12. 사소한 conflict는 아래 확정 결정과 group plan을 기준으로 해결하고 계속 진행한다. 중단 조건에 해당할 때만 사람에게 질문한다.

## 4. 확정 결정

- **#8943 — A안:** upstream 1.17.0 recorded battle 구조 사용. 기존 HNS 구형 녹화 배틀 호환·변환 계층은 만들지 않고 기존 기록 무효화를 허용한다. 일반 게임 세이브 호환성은 유지한다.
- **#10144 — C안:** upstream 판정·처리 순서와 버그 수정은 이식하되 HNS의 한국어 효과 없음 메시지 정책과 문구를 유지한다.
- **#10151 — B안 / `GEN_9`:** HNS 현재 동작과 밸런스를 유지한다.
- **#10454 — B안 / `GEN_8`:** HNS 현재 날씨 동작을 유지한다.
- **#10461:** HNS의 `<=` 경계를 유지하고 다른 upstream 구조 개선만 이식한다.
- **#9920:** 기존 save version 구조를 건드리지 않고 `SaveBlock3` 끝에 `u32 dailySeed`를 추가한다.
- **#10268 — A안:** 열매 발동 시 upstream 아이템 팝업을 사용한다.
- **#9819 — A안:** upstream #9819를 그대로 이식한다. 현재 관련 HNS 맵의 `MB_ROCK_CLIMB` 배치는 0개다.
- **#10429:** `8280caf163`에서 **already applied / HNS-adapted**. upstream `0x3A`를 다시 넣지 말고 현재 비일본어 독립 ZWS `0x42`와 한글 인식형 순회를 보존한다.

다른 세부 항목은 `pokeemerald-expansion-1.17.0-full-sync-plan.md` 6절을 그대로 따른다. 문서에 없는 새로운 HNS 정책은 임의로 만들지 않는다.

## 5. #10429 / #10301 특별 주의

`port_sequence.tsv`에서 #10301은 현재 seq 289, #10429는 seq 291이다.

- #10301을 이식할 때 `ReformatItemDescription` 수정과 현재 #10429 HNS ZWS 구현을 통합한다.
- #10429는 다시 cherry-pick하거나 upstream `CHAR_ZWS = 0x3A`를 적용하지 않는다.
- HNS의 `CHAR_ZWS = 0x42`는 비일본어 문자열에서 독립 바이트일 때만 ZWS다.
- 한글 후속 `0x42`, `0x3A`, `0xAE`와 일본어 `ぢ = 0x42`를 기존 글자로 보존한다.
- #10301 이식 후 build와 mGBA 체크리스트에 반드시 다음을 포함한다.
  1. 하이픈 뒤 공백 없이 이어서 표시되는 아이템 설명
  2. 좁은 폭에서 ZWS 위치 줄바꿈
  3. 긴 한글 아이템 설명
  4. 한글 2바이트 중간 분리·깨짐 없음
  5. 일반 영문·한글 문자열 폭과 중앙 정렬 회귀 없음

## 6. 진행 기록과 push 범위

- 작업 브랜치와 push 대상은 `gorunit1/pokehns-expansion-kor`의 `pokehns-expansion-kor`뿐이다.
- upstream `PokemonHnS-Development`에는 push하거나 PR을 만들지 않는다.
- 기존 문서를 통째로 덮어쓰지 말고 unit 결과와 다음 시작점을 의미 단위로 갱신한다.
- `git reset --hard`, `git clean`, rebase, force push, 기존 로컬 변경 삭제를 하지 않는다.

## 7. 자동 진행 중단 조건

다음 경우에만 안전한 읽기 전용 확인을 끝낸 뒤 작업을 중단하고 친구에게 보고한다.

- 기존 문서 범위를 넘어서는 새로운 HNS 정책 결정이 필요함
- 일반 게임 세이브 호환성 영향이 확정 결정 범위를 넘어감
- 한글 인코딩·문자열·조사·메시지를 깨뜨릴 가능성이 있음
- 예상하지 못한 대규모 코드 충돌로 양쪽 의도를 안전하게 보존하기 어려움
- build/test 실패 원인을 기존 계획 안에서 안전하게 해결할 수 없음
- working tree에서 제3자의 미보존 변경을 발견함

그 외에는 사소한 충돌마다 승인을 기다리지 말고 확정된 plan과 HNS 보존 원칙을 적용해 계속 진행한다.

## 8. 첫 실행 목표

1. seq 1 테스트 러너 include 순서 복구를 현재 코드에 맞게 적용·검증한다.
2. 이어지는 작은 dependency unit부터 `port_sequence.tsv` 순서대로 실제 port를 시작한다.
3. 이미 적용된 항목을 만나면 재적용하지 않고 근거와 함께 skip한다.
4. 각 unit의 변경·검증·ROM 수치·남은 위험을 기록하고 커밋한다.
