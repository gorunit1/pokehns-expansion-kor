# 1.17.0 full-sync 실제 port — 구간 담당 에이전트 지시

당신은 HnS 한글화 저장소에서 `port_sequence.tsv`의 **지정된 seq 구간**을 순서대로 실제 이식한다. 친구(저장소 소유자)의 실행 지시문이 기준이다.

## 기준 문서 (반드시 먼저 읽기)
저장소: 이 문서가 들어 있는 저장소 (브랜치 `pokehns-expansion-kor`)
1. `docs/friend-handoff/CLAUDE_FULL_SYNC_PORT_PROMPT.md` — **실행 지시문. 여기 규칙을 전부 따른다.**
2. `docs/friend-handoff/HANDBACK_2026-09-29_FULL_SYNC_PORT_START.md`
3. `docs/friend-handoff/results/pokeemerald-expansion-1.17.0-full-sync-plan.md`(특히 5·6절 확정 결정)
4. `docs/friend-handoff/results/1.17.0-sync-plan/port_sequence.tsv`와, 담당 PR이 속한 그룹의 `*_plan.tsv`/`*_plan.md`(approach·evidence·notes)
5. `docs/localization/BATTLE_MESSAGE_OUTPUT_CHANGES.md`, `AGENTS.md`

upstream은 저장소 옆 폴더 `../pokeemerald-expansion-upstream` (없으면 `git clone https://github.com/rh-hideout/pokeemerald-expansion.git ../pokeemerald-expansion-upstream`)에서 읽기만 한다. `git log --all --grep='(#NNNN)'`로 커밋을 찾고 `git show`로 diff를 본다.

## 작업 방식
- 작업 폴더는 메인 저장소 하나다. 다른 에이전트나 다른 컴퓨터가 동시에 같은 브랜치를 수정하지 않는다.
- 시작할 때 `git status --short --branch`가 깨끗한지 확인한다. 깨끗하지 않으면 중단하고 보고한다. pull·push는 하지 않는다(메인이 한다).
- 담당 구간의 각 행을 **seq 순서대로** 처리한다.
  1. **이미 적용 여부 확인:** 현재 코드와 history를 대조한다(`git log --grep`, 해당 hunk 존재 여부). 이미 적용·동등이면 재적용하지 않고 근거를 기록한다.
  2. **이식:** upstream diff와 HnS 코드를 대조해 의미 단위로 옮긴다. 파일 통째 복사나 `ours`/`theirs` 선택은 하지 않는다. group plan의 approach를 따른다.
     - 보존: 한글 문자열·인코딩·`STRINGID` 매핑·`{B_...}`·조사 토큰·HnS 배틀 메시지 출력 정책·HnS config·Pokegear/Pokenav·작명·storage 로컬 변경, #10429 HnS 구현(`CHAR_ZWS=0x42`)
     - upstream 테스트 파일(`test/**`)도 이식 대상이다. 영문 `MESSAGE` 기대값은 원문 그대로 둔다. HnS에서 실패하는 것은 알려진 한계로 기록한다.
  3. **검증:** `git diff --check`를 통과시킨 뒤 `GITHUB_ACTION=1 make hns -j8 > build/port.log 2>&1`로 빌드한다. 종료 코드와 `Memory region`(ROM/EWRAM/IWRAM)을 기록한다. 새 경고가 나왔으면 원인을 확인한다.
     - 노트북 WSL에서는 ARM 공식 툴체인이 PATH에 없다. 빌드·테스트 명령마다 앞에 `PATH=/opt/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin:$PATH`를 붙인다.
     - 출력이 없다는 이유로 빌드를 중복 실행하지 않는다.
     - 테스트가 가능하면 해당 테스트 파일만 돌린다(아래 "테스트" 절).
  4. **커밋:** PR(또는 반드시 같이 가야 하는 unit)마다 커밋 1개를 만든다. `git add`에는 변경 파일 경로를 명시한다(`.`/`-A` 금지). 메시지 형식:
     ```
     Port upstream #NNNN: <영문 제목>

     <HnS 적응 내용 1~3줄, 제외한 hunk와 이유>

     Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>
     ```
     skip/동등은 커밋 없이 결과 문서에만 기록한다.
  5. **기록:** 결과 문서에 이어서 적는다. 적을 때마다 저장한다.
     - 결과 문서: `docs/friend-handoff/results/1.17.0-port/full-sync-seq-{FROM}-{TO}.md`
     - 형식: 지시서의 "결과 보고 양식"
     - 항목: 판정(적용/부분 적용/이미 적용/HnS 동등/skip), 커밋, 수정 파일, HnS 적응·보존 내용, ROM/EWRAM/IWRAM, 테스트, 남은 위험, 실기 확인 필요 여부
- **PR마다 진행 기록 커밋(2026-09-29 추가):** 사용량 한도로 세션이 끊겨도 진행이 남도록, 각 PR 커밋 직후 결과 문서에 그 PR 항목(커밋 해시 포함)을 적고 결과 문서만 별도 커밋한다(`Record full-sync port progress seq N (#NNNN)`). 결과 문서 첫머리에는 "진행 중: 마지막 완료 seq N, 다음 seq M" 한 줄을 두고 매번 갱신한다. 메인이 커밋을 수시로 push하므로 `index.lock` 오류가 나면 몇 초 뒤 다시 시도한다.
- 구간을 끝내면 결과 문서를 별도 커밋으로 남긴다(`Record full-sync port results seq FROM-TO`).
- `STATUS.md`·`SESSION_LOG.md`는 건드리지 않는다(메인이 통합한다).

## 테스트

**검증 도구(2026-10-07부터 저장소 안):** `dev_scripts/hns_verify/`(사용법 `README.md`). 한글 배틀 출력 회귀 `kortests/run.sh`, 세이브 정적 비교 `save/save_compat.py`(`collect`로 직전 기준을 만들고 `run --pre`), 세이브 왕복 `save/savetest/run_all.sh`, 새 경고 `warncheck.sh`, 전체 테스트 목록·사라진 PASS `testlist.sh`. 빌드·테스트·ROM에는 들어가지 않는다. 아래 명령은 같은 일을 손으로 하는 방법이다.
`make check BUILD=hns`는 쓸 수 있는 상태다. 테스트 러너 복구 seq 1과 힙 수정 `7df90335e4`·`a04eae0499`가 반영되어 있다. 원인과 분류는 [`TEST_RUNNER_FIX.md`](TEST_RUNNER_FIX.md)에 있다.
- 파일 하나만 실행: `make check BUILD=hns -j4 TESTS="test/battle/weather/hail.c"`
- 이름 접두어로 실행: `make check BUILD=hns -j4 TESTS="Hail deals 1/16"`
- 이름 중간 일치로 실행: `make check BUILD=hns -j4 TESTS="*Sandstorm"`
- 전체 실행: `make check BUILD=hns -j6` (약 5분)
- **판정 방법:** 영문 `MESSAGE` 기대값 불일치(HnS는 한글 문자열)로 약 2,000건이 원래 실패한다. 그래서 절대 통과 수가 아니라 **이식 전후 비교**로 판단한다.
  - 이식한 PR과 관련된 테스트 파일을 이식 전후로 돌린다.
  - 통과하던 테스트가 실패로 바뀌면 회귀다.
  - 실패 사유가 `Unmatched MESSAGE`뿐이면 알려진 한계다.
  - 큰 단위(XL·L)가 끝나면 전체를 돌려 이전 전체 결과와 PASS 수와 실패 목록을 비교한다.
- 전체 기준 결과는 `docs/localization/STATUS.md`에 기록된 최신 값을 쓴다. 테스트별 상태 목록은 `results/1.17.0-port/test-baseline-seq*.txt` 가운데 가장 최근 것을 쓴다.
- 전체 실행 로그를 비교용 목록으로 바꾸는 방법:
  ```bash
  sed 's/\x1b\[[0-9;]*m//g' <로그> | grep -E '^\[[0-9]+\] .*: (PASS|FAIL|KNOWN_FAILING|TO_DO|EXPECTED_FAIL)$' | sed -E 's/^\[[0-9]+\] //' | sort -u > new.txt
  diff <(grep ': PASS$' <기준 목록>) <(grep ': PASS$' new.txt) | grep '^<'   # 통과하던 것 중 사라진 것 = 회귀 후보
  ```
- 노트북 WSL 셸의 `grep`은 ugrep 래퍼 함수라서 파일 인자를 주면 빈 결과가 나올 수 있다. 위 비교에는 `command grep`을 쓴다.
- "~ fit on ~" 계열 23개 테스트는 실패 내용의 한글 인코딩 바이트(UTF-8이 아님)가 이름 줄에 붙어 출력된다. UTF-8 로케일에서는 이 줄이 목록에서 빠지므로 `LC_ALL=C`에서 `grep -a`로 추출한다. 노트북에서 쓴 명령:
  ```bash
  LC_ALL=C sed 's/\x1b\[[0-9;]*m//g' <로그> | LC_ALL=C command grep -a -E '^\[[0-9]+\] .*: (PASS|FAIL|KNOWN_FAILING|TO_DO|EXPECTED_FAIL)$' | LC_ALL=C sed -E 's/^\[[0-9]+\] //' | LC_ALL=C sort -u > new.txt
  ```
- 무한 출력하는 크래시가 생기면 테스트 러너(hydra)가 메모리 부족으로 죽을 수 있다. 전체 실행이 `Killed`로 끝나면 로그 끝에서 반복되는 출력부터 확인한다.

## 중단 조건 (여기에 해당할 때만 멈추고 보고)
- 문서 범위를 넘는 새 HnS 정책 결정이 필요하다.
- 일반 게임 세이브 호환성에 확정 결정 범위를 넘는 영향이 있다.
- 한글 인코딩·문자열·조사·메시지가 깨질 가능성이 있다.
- 대규모 충돌로 양쪽 의도를 안전하게 보존하기 어렵다.
- 빌드·테스트 실패를 계획 안에서 안전하게 해결할 수 없다.
- 작업 트리에서 모르는 변경을 발견했다.

중단할 때는 **그 행 직전까지의 커밋과 결과 기록을 남긴다.** 멈춘 행의 변경은 커밋하지 않는다. 작업 트리에 그대로 둔 채 무엇이 남았는지 보고하고, 되돌리지 않는다.

## 금지
- `git reset --hard`, `git clean`, 일괄 `checkout`, rebase, stash, push, force
- 이미 적용된 PR과 #10429 재이식
- 관련 없는 포매팅
- 메인 저장소 밖(다른 worktree 포함) 수정

## 최종 응답
짧게 쓴다.
- 처리한 seq 범위
- PR별 한 줄: 판정 / 커밋 / 비고
- 마지막 빌드의 ROM·EWRAM·IWRAM
- 실기 확인 필요 항목
- 중단했다면 사유와 남은 상태
