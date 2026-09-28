# 새 세션용 프롬프트 모음 (클라우드 / 노트북 공통)

1.17.0 full-sync 이식을 **새 Claude 세션**에서 이어갈 때 쓴다. 위에서부터 순서대로 하나씩 붙여 넣는다.
진행 상황의 기준은 항상 저장소 안의 문서다: `docs/localization/STATUS.md` 맨 위, `docs/friend-handoff/results/1.17.0-port/`.

> 주의: 데스크탑, 노트북, 클라우드 가운데 **한 곳에서만** 작업한다. 친구도 같은 브랜치에 push하므로 새 세션은 항상 pull부터 한다.

---

## 0-A. 환경 준비 — 클라우드 세션(claude.ai/code)

GitHub 저장소 `gorunit1/pokehns-expansion-kor`를 연결해 세션을 연 뒤 붙여 넣는다.

```text
이 저장소는 Pokémon HnS 한글화 프로젝트(GBA, pokeemerald-expansion 기반)야. 오늘은 환경 준비만 해줘. 코드는 수정하지 마.

1. 작업 브랜치를 `pokehns-expansion-kor`로 전환하고 `git status --short --branch`를 보여줘.
2. GBA 빌드 도구를 설치해줘.
   sudo apt-get update && sudo apt-get install -y build-essential binutils-arm-none-eabi gcc-arm-none-eabi libnewlib-arm-none-eabi libpng-dev python3
   설치된 `arm-none-eabi-gcc --version`을 알려줘. 기준 환경은 13.2.1이야.
3. 저장소 옆 폴더에 upstream을 clone해줘.
   git clone https://github.com/rh-hideout/pokeemerald-expansion.git ../pokeemerald-expansion-upstream
   `git -C ../pokeemerald-expansion-upstream tag -l 'expansion/1.1[5-7]*'`에 `expansion/1.17.0`이 있는지 확인해.
4. `make hns -j$(nproc)`로 빌드하고 종료 코드와 `Memory region`(ROM/EWRAM/IWRAM)을 알려줘.
5. push 권한을 확인해줘. `git push --dry-run origin HEAD:pokehns-expansion-kor`을 실행해 이 세션이 **`pokehns-expansion-kor` 브랜치에 직접 push할 수 있는지** 보고해. 새 브랜치나 PR로만 된다면 그렇다고 알려줘.
   upstream `PokemonHnS-Development`에는 절대 push하거나 PR을 만들지 마.
6. CPU 코어 수, 메모리, 세션 시간 제한 같은 환경 제약을 알 수 있으면 알려줘.
```

**판단:** 4번 빌드와 5번 직접 push가 **둘 다 되면** 클라우드로 계속 간다. 하나라도 안 되면 0-B(노트북)로 간다.

---

## 0-B. 환경 준비 — 노트북 WSL

아래 명령은 **사람이 직접** 실행한다. sudo 비밀번호가 필요하기 때문이다.

PowerShell(관리자)에서 실행한다. 이미 WSL Ubuntu가 있으면 이 단계는 건너뛴다.
```powershell
wsl --install -d Ubuntu-24.04
```

Ubuntu 터미널에서 실행한다.
```bash
sudo apt update && sudo apt install -y build-essential git binutils-arm-none-eabi gcc-arm-none-eabi libnewlib-arm-none-eabi libpng-dev python3
```
```bash
git config --global user.name "jinmo" && git config --global user.email "hajinmo0725@gmail.com"
```
노트북에 Git for Windows가 있으면 GitHub 로그인을 연결한다.
```bash
git config --global credential.helper "/mnt/c/Program\ Files/Git/mingw64/bin/git-credential-manager.exe"
```
```bash
git clone https://github.com/gorunit1/pokehns-expansion-kor.git ~/pokehns-expansion-kor && cd ~/pokehns-expansion-kor && git switch pokehns-expansion-kor
```
```bash
git clone https://github.com/rh-hideout/pokeemerald-expansion.git ~/pokeemerald-expansion-upstream
```
```bash
cd ~/pokehns-expansion-kor && make hns -j8
```

그다음 Claude 앱의 Code 탭에서 WSL `~/pokehns-expansion-kor` 폴더를 열고 새 세션을 시작한다.

---

## 1. 상태 파악 (코드 수정 없음)

```text
HnS 한글화 저장소에서 pokeemerald-expansion 1.17.0 full-sync 이식을 이어서 할 거야. 이번 단계는 상태 파악만 해줘. 파일은 수정하지 마.

1. `git status --short --branch` 결과가 깨끗하면 `git pull --ff-only origin pokehns-expansion-kor`을 실행해. 깨끗하지 않으면 멈추고 파일 목록을 보여줘.
2. 아래 순서로 읽어.
   - AGENTS.md
   - docs/localization/STATUS.md 맨 위 항목들
   - docs/friend-handoff/CLAUDE_FULL_SYNC_PORT_PROMPT.md
   - docs/friend-handoff/results/1.17.0-port/PORT_INSTRUCTIONS.md
   - docs/friend-handoff/results/1.17.0-port/의 가장 최근 결과 문서
3. 아래를 짧게 보고해줘.
   - 마지막으로 끝난 seq와 다음 시작 seq (port_sequence.tsv 기준)
   - 최근 빌드의 ROM/EWRAM/IWRAM
   - 테스트(`make check BUILD=hns`)를 쓸 수 있는 상태인지
   - 친구가 새로 push한 커밋이 있으면 그 내용
   - 사람 결정이 필요한 미결 사항
```

---

## 2. 이식 재개

```text
docs/friend-handoff/results/1.17.0-port/PORT_INSTRUCTIONS.md와 CLAUDE_FULL_SYNC_PORT_PROMPT.md 규칙대로 full-sync 이식을 이어서 진행해줘.

- STATUS.md에 적힌 "다음 시작 seq"부터 port_sequence.tsv 순서대로 진행해.
- 진행 방식:
  - 구간 단위로 에이전트에게 맡겨. 크기 가중치 S=1, M=3, L=8, XL=25로 합이 40~60 정도인 구간이 적당해. XL 하나는 단독 구간으로 해.
  - 구간은 한 번에 하나씩, 순서대로 진행해. 같은 브랜치를 동시에 고치는 병렬 작업은 하지 마.
  - PR마다 `git diff --check`, `make hns` 빌드, 커밋을 해. 결과는 `results/1.17.0-port/full-sync-seq-FROM-TO.md`에 기록해.
  - 테스트는 PORT_INSTRUCTIONS의 "테스트" 절대로 **이식 전후 비교**로 판단해. 큰 단위가 끝나면 전체 테스트를 돌려 최신 `test-baseline-seq*.txt`와 비교하고, 새 기준 목록을 저장해.
- 구간이 끝날 때마다 네가 직접 확인해.
  - 소스 diff에서 한글이 들어간 줄이 바뀌었는지 검사
  - 전체 빌드와 ROM 수치
  - STATUS.md·SESSION_LOG.md 갱신(다음 시작 seq 포함)
  - push (`pokehns-expansion-kor` 브랜치만)
- 큰 리팩터(#9655, #8943, #9730, #9939, 캔슬러 체인)가 끝나면 친구용 mGBA 확인 목록을 results/1.17.0-port/에 문서로 남기고 알려줘.
- 중단 조건(PORT_INSTRUCTIONS의 목록)에 걸리면 멈추고 나한테 물어봐. 그 외에는 계속 진행해.
- 구간 하나가 끝날 때마다 진행 상황을 짧게 알려줘.
```

---

## 3. 세션이 끊겼다가 다시 시작할 때

```text
이전 세션이 이식 도중 끊겼어. 이어서 하기 전에 상태부터 정리해줘.
1. `git status --short --branch`와 `git log --oneline -15`를 보여줘.
2. 커밋되지 않은 변경이 있으면 어느 seq·PR의 작업인지 결과 문서와 대조해서 알려줘. 지우거나 되돌리지 말고, 이어서 완성할지 버릴지 나한테 먼저 물어봐.
3. 결과 문서의 마지막 기록과 커밋이 서로 맞는지 확인해줘. 커밋은 있는데 기록이 없는 것, 기록은 있는데 커밋이 없는 것을 찾아줘.
4. 정리가 끝나면 2번 프롬프트 방식으로 다음 seq부터 재개해.
```

---

## 4. 작업을 멈추기 전 (세션 종료·기기 전환 전)

```text
오늘 작업은 여기까지 할게. 마무리해줘.
1. 지금 돌고 있는 구간이 있으면 진행 중인 PR까지 끝내고 멈춰. 반쯤 된 PR은 커밋하지 말고 상태만 기록해.
2. 결과 문서, STATUS.md, SESSION_LOG.md에 아래를 적어.
   - 마지막으로 끝난 seq와 다음 시작 seq
   - 최근 ROM 수치
   - 미결 사항
   - 친구가 mGBA로 확인할 항목
3. docs/friend-handoff/에 오늘 날짜로 짧은 작업 회신(HANDBACK_YYYY-MM-DD.md)을 쓰고, README.md의 "작업 회신"에 링크를 추가해.
4. `git diff --check`를 통과시키고 커밋한 뒤 `pokehns-expansion-kor`에 push해. push 전에 fetch해서 친구 커밋이 있으면 merge로 합쳐(rebase 금지).
5. push한 커밋 범위와, 친구에게 전할 한두 줄 요약을 알려줘.
```
