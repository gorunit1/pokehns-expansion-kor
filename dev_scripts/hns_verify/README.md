# HnS 검증 도구 (`dev_scripts/hns_verify/`)

full-sync 묶음과 HnS 수정 때 쓰는 **세이브 호환·한글 배틀 출력 회귀 검증 도구**다. 데스크탑과 노트북이 같은 도구·같은 기준으로 돌리도록 저장소에 넣었다(재확인 17, 친구 승인 2026-10-07: "빌드·테스트·ROM에는 포함되지 않도록").

## 0. 빌드·테스트·ROM에 들어가지 않는다

- `Makefile`은 `src/`·`test/` 등 정해진 폴더만 모은다(`TEST_SRCS_IN := $(wildcard test/*.c test/*/*.c test/*/*/*.c)`). `make_tools.mk`는 포함 목록이고, CI(`.github/workflows/build.yml`)도 이 폴더를 보지 않는다. → `make hns`·`make check`·CI 결과가 바뀌지 않는다(추가 커밋에서 실측: ROM SHA1 그대로, `make check`의 테스트 소스 958개 그대로).
- 한글 회귀 테스트 `.c`는 `test/` 밖(`kortests/sets/`)에 있다. **`test/` 아래로 옮기지 않는다** — 옮기면 `make check` 전체 목록과 CI에 들어간다. 도구가 실행할 때만 대상 트리의 `test/battle/`(세이브 왕복은 `test/`)에 복사하고, 실행 뒤(중간에 끊겨도) 지운다.
- 출력은 저장소 밖 `$HNS_VERIFY_OUT`(기본 `$HOME/hns-sync-work/verify-runs`)에만 쓴다. 출력 폴더가 저장소 작업 트리 안(`build/` 밖)이면 도구가 거부한다. 파이썬 도구는 `__pycache__`를 만들지 않는다.
- 추적 파일(`Makefile`, `.gitignore`, `.gitattributes`, `test/**`, CI)은 고치지 않았다. 세이브 이미지는 `.gitignore`의 `*.sa*`를 피하고 `.gitattributes`의 `*.bin binary`를 받도록 `*-flash.bin` 이름이다.

## 1. 구성

| 경로 | 내용 |
|---|---|
| `common.sh`, `hnsverify_paths.py` | 경로 규칙(환경 변수·기본값), 실제 체크아웃 판정(`.git`), 출력 폴더 검사, `DEVKITARM` → `PATH` |
| `kortests/run.sh` | 한글 배틀 출력 회귀 607개 실행·요약·기대 비교 |
| `kortests/compare.py` | 요약 두 개 비교(테스트별 상태 + 합계, `-j`와 무관) |
| `kortests/sets/*.c` (29) | 테스트(이름이 모두 `HNS`로 시작). 옛 328개 세트 22파일 + `zz_hns9730_k1~k5`(235) + `zz_hnsx1_kor.c`(21) + `zz_hns9918_pledge.c`(23) |
| `kortests/expected/summary.txt`, `summary.meta` | 현재 HEAD 기대 요약(517/607)과 그 설명 |
| `kortests/tools/` | `trace.patch`(사본 전용 이벤트 출력), `trace_decode.py`, `tracecmp.py`, `tracediff.py`, `showtrace.sh`, `gen_scene.py`(trace로 새 테스트 SCENE 채우기) |
| `save/save_compat.py`, `save/dwarf_layout.py` | 세이브 정적 비교(구조체 레이아웃·섹터 배치·세이브 경로 기계어·RAM 변수) |
| `save/savetest/` | 세이브 왕복(`savetest.py`, `run_all.sh`, 테스트 `zz_hns8943_savecompat.c`, 기준 `baseline/`) |
| `mkcopy.sh` | 스크래치 사본 만들기(`--allow-dirty`, `--patch`, `--trace`) |
| `warncheck.sh`, `expected/warn-base.txt` | 새 컴파일 경고 확인(기준 42줄) |
| `testlist.sh` | 전체 테스트 로그 → 비교용 목록, 사라진 PASS 확인 |

## 2. 환경 변수와 노트북 설정

| 변수 | 기본값 | 뜻 |
|---|---|---|
| `HNS_REPO` | 이 폴더가 든 git 체크아웃 최상위 | 대상 저장소(`save_compat.py`의 `--src`/`--post-src` 기본값, `mkcopy.sh --from` 기본값) |
| `HNS_WORK` | `$HOME/hns-sync-work` | 스크래치 작업 폴더 |
| `HNS_VERIFY_OUT` | `$HNS_WORK/verify-runs` | 출력: `kortests/`, `savetest/`, `save/`, `testlist/` |
| `HNS_JOBS` | 한글 회귀 8, 세이브 왕복 `nproc` | `make -j` 기본값(각 도구 인자가 우선) |
| `DEVKITARM` | 없음 | 있으면 `$DEVKITARM/bin`을 `PATH` 앞에(Makefile과 같은 규칙). 없고 `PATH`에 `arm-none-eabi-gcc`도 없으면 `/opt/arm-gnu-toolchain-*/bin`을 찾아 붙인다 |
| `ARM_GCC`, `ARM_OBJDUMP`, `READELF` | `arm-none-eabi-*` | `save_compat.py`의 개별 도구 덮어쓰기 |
| `ALLOW_REPO=1` | 없음 | 실제 체크아웃(`.git` 있는 트리)에서 테스트 파일을 복사·실행·삭제하도록 허용 |
| `HNS_KOR_EXPECT` | `kortests/expected/summary.txt` | 한글 회귀 비교 기준 바꾸기(예: 이식 전 실행 요약) |

노트북(ARM 공식 툴체인이 `PATH` 밖, 8코어·7 GB, 홈 `/home/jinmo`):

```sh
export DEVKITARM=/opt/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi
R=$HOME/pokehns-expansion-kor; V=$R/dev_scripts/hns_verify; O=$HOME/hns-sync-work/verify-runs
```

`-j8`보다 높이지 않는다(hydra 메모리). 필요한 것: `python3`(표준 라이브러리만), `git`, `make check BUILD=hns`가 되는 환경, 사본을 만들 때만 `rsync`. 사본 없이도 `ALLOW_REPO=1`로 한글 회귀·세이브 왕복을 저장소에서 바로 돌릴 수 있다(trace만 사본 필요).

## 3. 도구별 사용법과 기대 출력

아래 수치는 HEAD `9b22b2c3dc`(코드 = `820148e779`), 데스크탑(Ubuntu `gcc-arm-none-eabi` 13.2.1) 실측이다.

### 3.1 한글 배틀 출력 회귀 (`kortests/run.sh`)

```sh
ALLOW_REPO=1 $V/kortests/run.sh $R <label> 8          # 저장소에서(복사 → 실행 → 삭제)
$V/kortests/run.sh <사본> <label> 8                    # mkcopy.sh 사본에서
```

- 기대: `- Tests PASSED: 517`, `- Tests TOTAL: 607`, 세트별 PASS(`HNS9730 199/235`, `HNSX1 21/21`, `HNS9918 23/23` 등, `kortests/expected/summary.meta`), 끝에 `status lines that differ from expected: 0`, `whole summary file byte-identical to expected: yes`, `판정: PASS (기대와 같음)`. 종료 코드 0(다르면 1, 빌드 오류 2). `make check exit 2`는 정상이다(기대 FAIL·INVALID 90개).
- 출력: `$HNS_VERIFY_OUT/kortests/<label>.log`, `<label>-summary.txt`.
- 판정은 테스트별 상태와 합계로 한다. 요약 끝의 `  - test/…` 줄은 hydra가 실패 앞 50개만 runner 순서로 적는 것이라 `-j`가 다르면 달라진다 → 기대 요약과 바이트까지 맞추려면 `-j8`.
- 저장소 테스트가 없는 문장을 지키는 테스트(리뷰 R3): `HNS9730 K1-51`(충전), `K2-16`(위협 vs 주눅구슬), `K3-25`(심술꾸러기 주눅구슬, 받아들인 FAIL), `HNSX1` 21개.
- 세트 하나만: `… run.sh $R <label> 8 HNS9730`(기대 비교는 하지 않고 PASS 수와 non-PASS 목록만).
- 실제 저장소에 `trace.patch`(표식 `-TRACE`)가 있으면 거부한다. 같은 이름의 다른 파일이 `test/battle/`에 있거나 추적 파일이면 덮어쓰지 않고 멈춘다.

**trace(배틀 이벤트 전부 해독, 사본 전용)**

```sh
$V/mkcopy.sh $O/copy-pre --trace                   # 사본 + trace.patch (.git 있는 트리·git 작업 트리 안에는 만들지 않는다)
$V/kortests/run.sh $O/copy-pre pre 8                # → $O/kortests/pre-trace.txt (607 블록). 요약은 trace 없이와 같다
python3 $V/kortests/tools/tracecmp.py $O/kortests/pre-trace.txt $O/kortests/post-trace.txt   # 테스트별 SAME/DIFF
python3 $V/kortests/tools/tracediff.py <a-trace> <b-trace> 'HNS9730 K4-06'                   # 한 테스트 diff
$V/kortests/tools/showtrace.sh <trace> "K1-03"                                                # 한 테스트 이벤트
```

`trace.patch`는 `test/test_runner_battle.c`·`src/battle_interface.c`를 고친다. 이식으로 그 줄이 움직이면 묶음 시작 때 `git apply --check -p1 $V/kortests/tools/trace.patch`(저장소에서 확인만)로 보고, 깨지면 위치만 다시 맞춘다.

### 3.2 세이브 왕복 (`save/savetest/run_all.sh`)

이식 전(#8943 전) ROM이 만든 세이브 2개를 현재 빌드의 실제 `LoadGameSave`로 읽어 이식 전 결과와 비교한다.

```sh
ALLOW_REPO=1 $V/save/savetest/run_all.sh $R <label> 8      # 저장소에서(test/에 넣고 실행 뒤 지움)
$V/mkcopy.sh $O/copy && $V/save/savetest/run_all.sh $O/copy <label> 8   # 사본에서(커밋 전 변경이면 --allow-dirty)
```

- 기대(두 이미지 각각): `HNS8943 SAVE LOAD/MAKE/NEWGAME/RESAVE: PASS`, `[PASS] LOAD 테스트가 읽은 이미지 = pre-make.sav@2cde99625f97`(newgame은 `pre-newgame.sav@d0ddbf3dd753`), `[PASS] make.sav 일반 세이브 섹터(0~30) 바이트 동일`, `[PASS] newgame.sav … 동일 (기준 expect-newgame-flash.bin)`, `[PASS] load.txt 불러오기 결과 95줄 동일`, `[INFO] 녹화 기록 유효: 1 → 0`(make; newgame은 `0 → 0`), `판정: PASS`, 마지막 `run_all: PASS`. 종료 코드 0.
- 매 실행마다 테스트 오브젝트를 지워 다시 컴파일하고, LOAD 테스트가 출력한 이미지 이름@SHA1이 이번 이미지와 다르면 FAIL로 처리한다(옛 도구에서 이미지를 바꾼 뒤 다시 빌드되지 않아 앞 이미지를 읽은 적이 있다, 재확인 17).
- 출력: `$HNS_VERIFY_OUT/savetest/<label>-{make,newgame}/`(`make.sav`, `newgame.sav`, `load.txt`, `summary.txt`, `make.log`).
- 개별 명령: `python3 $V/save/savetest/savetest.py run|compare|reparse|image …`(머리 주석).

### 3.3 세이브 정적 비교 (`save/save_compat.py`)

**이식 전 사실은 같은 기계에서 묶음 직전에 `collect`로 만든다.** 기계어(`objdump`)·RAM(`.map`) 사실은 툴체인마다 다를 수 있어 다른 기계의 사실 폴더나 옛 `verify/pre`를 기준으로 쓰지 않는다(그래서 기준 사실은 커밋하지 않았고, 저장소판 `run`은 `--pre`가 필수다).

```sh
# 이식 전: 깨끗한 HEAD에서 make hns -j8 뒤
python3 $V/save/save_compat.py collect --src $R --elf $R/pokehns.elf --label pre-<seq> --out $O/save/pre-<seq>
# 이식 후: make hns -j8 뒤
python3 $V/save/save_compat.py run --pre $O/save/pre-<seq> [--out $O/save/post-<seq>]
```

- 기대(같은 커밋끼리 = 도구 확인): `판정: PASS (FAIL 0, WARN 0)`. 종료 코드 0 = FAIL 0(WARN은 사람이 읽고 설명), 1 = FAIL, 2 = 도구 오류.
- 이식 뒤 WARN은 보고서가 가리키는 차이 파일(`code_*.diff`, `ram_symbols.diff.tsv`)을 읽고 결과 문서에 설명한다. 예: seq 181(`chunk-181/main/savecompat-pre181` 대 현재 HEAD)은 `PASS (FAIL 0, WARN 3)` — `BoxMonRestorePP` 정규화 코드 2줄, 검토 묶음 차이 1, `gSpecialStatuses` +272 B(세이브 무관).
- `collect --rev <커밋>`은 그 커밋의 `include/`를 풀어 헤더 사실만 만든다(ELF는 `--elf`로 따로). `compare <pre> <post> --out …`은 사실 폴더 둘을 비교한다.
- 옛 기본 `run`(옛 `verify/pre`·`base/` 자동 생성)과 `selftest`는 저장소판에 없다(2단계).

### 3.4 새 경고 (`warncheck.sh`)

```sh
make hns -j8 > build/port.log 2>&1 ; $V/warncheck.sh build/port.log
```

기대: `new warnings: 0`(종료 코드 0). 증분 빌드 로그에는 다시 컴파일한 파일의 경고만 나온다. 허용 도구 경고 `libpng warning: bKGD: invalid index`는 따로 허용한다.

### 3.5 전체 테스트 목록 (`testlist.sh`)

```sh
GITHUB_ACTION=1 make check BUILD=hns -j8 > build/port-check.log 2>&1
$V/testlist.sh build/port-check.log docs/friend-handoff/results/1.17.0-port/test-baseline-<직전>.txt
```

- 기대(기준 목록은 가장 최근 `test-baseline-*.txt` — 2026-10-07 밤 묶음 7 뒤 `test-baseline-seq187.txt`): 목록이 기준과 바이트 동일하거나 차이가 그 묶음의 예측과 같고, `list is byte-identical to …`, `lost PASS 0`. 목록은 `$HNS_VERIFY_OUT/testlist/<로그 이름>.txt`.
- `docs/friend-handoff/results/1.17.0-port/PORT_INSTRUCTIONS.md` "테스트" 절의 `LC_ALL=C` + `grep -a` 명령과 같다. 실제 저장소에 `test/battle/zz_*`·`test/zz_*`나 trace 표식이 남아 있으면 경고한다.

## 4. 한 묶음 절차(예)

```sh
# 이식 전(깨끗한 HEAD)
make hns -j8 > build/port-base.log 2>&1
python3 $V/save/save_compat.py collect --src $R --elf $R/pokehns.elf --label pre-<seq> --out $O/save/pre-<seq>
ALLOW_REPO=1 $V/kortests/run.sh $R pre-<seq> 8              # 판정: PASS(기대 요약과 같음)
# 이식 후
GITHUB_ACTION=1 make hns -j8 > build/port.log 2>&1 ; $V/warncheck.sh build/port.log
python3 $V/save/save_compat.py run --pre $O/save/pre-<seq>
ALLOW_REPO=1 $V/save/savetest/run_all.sh $R post-<seq> 8
ALLOW_REPO=1 $V/kortests/run.sh $R post-<seq> 8             # 다르면 DIFF 줄을 보고 의도한 변화인지 판단
python3 $V/kortests/compare.py $O/kortests/post-<seq>-summary.txt $O/kortests/pre-<seq>-summary.txt   # (선택) 이식 전 실행과 직접 비교
GITHUB_ACTION=1 make check BUILD=hns -j8 > build/port-check.log 2>&1
$V/testlist.sh build/port-check.log docs/friend-handoff/results/1.17.0-port/test-baseline-<직전>.txt
```

이식 전·후에는 **같은 도구 체크아웃**(같은 `kortests/sets/`)을 쓴다. 요약 줄에 `test/battle/<파일>.c:<줄>`이 들어가기 때문이다.

## 5. 기준 갱신

| 기준 | 언제 | 방법 |
|---|---|---|
| `kortests/expected/summary.txt` | 묶음·HnS 수정이 한글 출력을 **의도해서** 바꿨을 때, 또는 `sets/`의 테스트를 고치거나 더했을 때(줄 번호가 요약에 들어감) | 바뀐 테스트마다 DIFF 이유를 결과 문서에 적은 뒤, 그 커밋에서 `ALLOW_REPO=1 kortests/run.sh $R new 8`을 돌려 `$O/kortests/new-summary.txt`를 `expected/summary.txt`로 복사하고 `summary.meta`(커밋·날짜·세트별 수)를 고친다. 코드·세트·기대 요약을 **같은 커밋**에 넣는다. `-j8`로 만든다 |
| `kortests/sets/*.c` 새 세트 | 새 묶음용 한글 테스트를 계속 둘 때 | 이름을 `HNS<번호>`로 시작(저장소 테스트와 겹치지 않음)하고 `sets/`에 `zz_hns<번호>_*.c`로 둔다. 위처럼 기대 요약 갱신 |
| `save/savetest/baseline/` | 세이브 형식을 일부러 바꾸는 확정 결정이 들어갈 때만 | 입력 세이브 이미지(`pre-*-flash.bin`)는 이식 전(#8943 전) ROM이 만든 것이라 다시 만들 수 없으니 바꾸지 않는다. 불러오기 결과 `pre-load-*/load.txt`는 확정된 형식 변화가 생기면 그 줄만 고치고 여기에 적는다. **2026-10-07 seq 186.5 #9920(확정 결정 A, SaveBlock3 끝 `u32 dailySeed`, 52 → 56 B):** 두 `load.txt`의 `DUMP STATE` sb3 해시 `3b703d18` → `919cf698`(이식 전 세이브의 앞 52 B는 같고 새 4 B는 0 — 이미지에서 직접 계산해 확인). 다른 값은 그대로 |
| `save/savetest/baseline/expect-newgame-flash.bin` | 새 게임 초기화(`NewGameInitData`)가 만드는 **값**을 일부러 바꾸는 이식이 들어갈 때(형식 변화가 아님) | NEWGAME 출력(`newgame.sav`) 비교 기준. 없으면 옛 방식대로 `pre-newgame-flash.bin`과 비교한다. 그 커밋에서 `run_all.sh`를 돌려 `$O/savetest/<label>-newgame/newgame.sav`를 이 이름으로 복사하고(두 이미지 실행의 `newgame.sav`는 같다), 바뀐 섹터·필드를 여기에 적는다. `pre-newgame-flash.bin`(LOAD 입력)은 바꾸지 않는다. **seq 202 #10050(새 게임에서 `UpdateDailySeed()`):** 섹터 15(ID 0) `SaveBlock3.dailySeed` 0 → `0x45b982b2`, `SaveBlock2.playerApprentice.id` 2 → 1, 섹터 19(ID 4) `SaveBlock1.lilycoveLady`(퀴즈 문제·정답·상품) — 새 게임 난수열이 한 칸 밀린 값 변화. 트레이너 ID·복권 번호·`load.txt`는 그대로. SHA1 `8d5a0726fe409905f9b7cc08b2cb8b1c47454993` |
| `expected/warn-base.txt` | 의도한 경고 변화(새 경고를 받아들이거나 없어짐)가 있을 때 | 전체 빌드 로그에서 `LC_ALL=C grep -a 'warning:' log \| LC_ALL=C sed -E 's/:[0-9]+:[0-9]+: /: /' \| LC_ALL=C sort -u`로 다시 만들어 같은 커밋에 |
| 세이브 정적 비교 이식 전 사실 | 묶음마다 | `collect`로 새로 만든다. 커밋하지 않는다 |
| 전체 테스트 기준 목록 | 묶음마다 | 지금처럼 `docs/friend-handoff/results/1.17.0-port/test-baseline-<seq>.txt`(도구 폴더가 아님) |

## 6. 하지 않는 것

- 한글 회귀·세이브 테스트 `.c`를 `test/`에 커밋하지 않는다. `git add`는 경로를 적어서 한다(`git add .` 금지) — 중간에 끊긴 실행이 `test/battle/zz_*`를 남겼을 수 있다(`testlist.sh`가 경고).
- `trace.patch`를 실제 저장소에 적용하지 않는다(`mkcopy.sh --trace`만, 사본만). `Makefile`·CI를 이 도구 때문에 고치지 않는다.
- trace·로그·실행 요약, 세이브 정적 비교 사실, ELF·ROM은 커밋하지 않는다(`$HNS_VERIFY_OUT`).
- 테스트 `.c` 머리 주석의 "never committed/커밋하지 않는다", 옛 경로 문구는 옛 기록이다. 고치면 요약 줄 번호가 바뀌므로 기대 요약 갱신과 함께 한다(2단계). 기준 `baseline/pre-load-*/summary.txt`의 `tree …` 줄도 옛 기록이다(비교에 쓰지 않음).

## 7. 옛 스크래치 경로 → 새 경로

| 옛(데스크탑 `~/hns-sync-work/…`) | 새 |
|---|---|
| `chunk-1385/verify/save_compat.py`, `dwarf_layout.py` | `save/` (경로·`--pre` 필수·출력 위치만 바뀜) |
| `chunk-1385/verify/savetest/{savetest.py,run_all.sh,zz_hns8943_savecompat.c}` | `save/savetest/` (+ 저장소 모드·다시 컴파일·이미지 확인·정리) |
| `chunk-1385/verify/savetest/baseline/pre-{make,newgame}.sav` | `save/savetest/baseline/pre-{make,newgame}-flash.bin` (바이트 동일, `pre-load-*/` 안 중복 `.sav` 4개는 뺌) |
| `chunk-1385/verify/savetest/mkcopy_worktree.sh`, `chunk-132/D-tests/mkcopy.sh` | `mkcopy.sh` (`--allow-dirty`) |
| `hnsfix-1007b/kortests-final/{set328/*.c, zz_hns9730_k*.c, zz_hnsx1_kor.c}` | `kortests/sets/` (바이트 동일) |
| `hnsfix-1007b/kortests-final/run.sh`, `tools/cmp_expected.py` | `kortests/run.sh`, `kortests/compare.py` |
| `hnsfix-1007b/kortests-final/tools/*` | `kortests/tools/` (바이트 동일) |
| `hnsfix-1007b/kortests-final/runs/finalx5-summary.txt` | `kortests/expected/summary.txt` |
| `chunk-ahead-130-167/warn-base.txt` | `expected/warn-base.txt` |
| 출력 `chunk-1385/tmp-D/{runs,savetest-runs}`, `kortests-final/runs/` | `$HNS_VERIFY_OUT/{save,savetest,kortests}` |

넣지 않은 것: `save_compat_selftest.py`(지금 형태로는 통과하지 않음, 2단계), `verify/pre/`, `verify/expected/report-abc.txt`(#8943 리허설 기록), `kortests-final/expected-changes.tsv`(seq 181 분석 기록), 실행 trace·로그.

## 8. 알려진 한계

- 노트북 `objdump`(ARM 공식 13.2.Rel1)에서 세이브 정적 비교의 기계어 정규화가 같은 커밋끼리 `FAIL 0, WARN 0`인지는 아직 노트북에서 확인하지 않았다. 다르면 정규화를 고치기 전까지 정적 비교는 데스크탑에서 한다.
- 한글 회귀 기대 요약은 데스크탑에서 만들었다. 테스트 결과는 툴체인과 무관할 것으로 보지만(노트북 전체 테스트 목록이 데스크탑과 바이트 동일했다) 노트북 첫 실행으로 확인한다.
- 한글 세트는 평소 빌드에서 컴파일되지 않으므로 API가 바뀌면 다음 실행 때 알게 된다(그 묶음 커밋에서 고친다).
