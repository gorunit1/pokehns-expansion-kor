# #7305(seq 107) 열매 번호·세이브 호환 확인 도구

seq 107(#7305 gBerries 리팩터) 이식 뒤, 기존 세이브의 열매 나무(`SaveBlock1.berryTrees[].berry`)가 같은 열매로 읽히는지 다시 확인하는 도구다. 결과와 판단 근거는 [`../full-sync-seq-107-107.md`](../full-sync-seq-107-107.md)에 있다.

- 원본: 이식 전 사전 분석 에이전트(A1)가 세션 스크래치에서 만든 스크립트. 저장소로 옮기면서 두 가지를 고쳤다.
  - `newgame_trees.sh`: `cpp | awk`에서 awk가 먼저 `exit`하면 cpp가 EPIPE로 실패해 무작위로 FAIL이 났다. awk가 입력을 끝까지 읽게 했다. 스크래치 경로 대신 스크립트 위치를 쓴다.
  - `verify.sh`: 안내 문구만 고쳤다.
- 모든 스크립트는 저장소를 읽기만 한다. 중간 파일은 `$TMPDIR`(없으면 `/tmp`)에 만들고 지운다.
- 필요: `/opt/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin`(다르면 `TC=<bin 경로>`. `layout.sh`가 이 값으로 `READELF`도 정한다), 호스트 `gcc`, `python3`, `readelf`.

## 1. 데이터 수준 검증 (4항목)

```bash
PATH=/opt/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin:$PATH make hns -j8
B=docs/friend-handoff/results/1.17.0-port/berry-7305
bash $B/verify.sh "$PWD" "$PWD/pokehns.elf" <출력 폴더>
```

| 항목 | 내용 | PASS 조건 |
|---|---|---|
| (a) | 아이템 ↔ 열매 번호(68종), 경계 상수(`NUM_BERRIES` 등) | `pre/berry_ids.tsv`와 같음(`BERRY_INVALID` 줄 제외) |
| (b) | 세이브 구조체 레이아웃(DWARF) | `pre/save_layout.txt`와 같음 |
| (c) | 새 게임 초기 나무 118그루 (treeId, 열매 번호, stage) | `pre/newgame_trees.tsv`와 같고 ELF 바이트와 일치 |
| (d) | 열매 번호로 찾는 ROM 표(열매 정보·자연의은혜·크래시·열매 그림·나무 그림/팔레트) | `pre/berry_rom_tables.tsv`와 같음 |

- 예상된 차이(PASS에 포함): (a) 무효 저장값(0, 69~127)을 이전에는 버치열매(CHERI)로, 이후에는 `ITEM_NONE`으로 해석한다(60줄). (d) E-Reader 의문열매의 자연의은혜 타입·위력이 이전에는 표 범위 밖 읽기(`OOB`), 이후에는 0이다(2줄). 정상 플레이에서는 생기지 않는 값이다.
- `pre/`는 이식 직전(HEAD `84fd460dc0`, `pokehns.elf` SHA1 `da206fba…`) 값이다. `post/berry_ids.tsv`는 이식 직후 (a) 결과다. 이식 후 (b) 결과는 `pre/save_layout.txt`와 같다.
- `berry_ids.sh`는 `include/constants/berries.h`가 없으면 이식 전 규칙(`ITEM_TO_BERRY`)으로 계산한다.

## 2. 기존 세이브 파일로 확인

```bash
python3 $B/sav_berry_trees.py <hns.sav> $B/pre/save_layout.txt $B/pre/berry_ids.tsv  > trees.pre
python3 $B/sav_berry_trees.py <hns.sav> $B/pre/save_layout.txt $B/post/berry_ids.tsv > trees.post
diff trees.pre trees.post   # 0줄이어야 한다(무효 저장값이 있는 나무만 예외)
```

실기용 사본 만들기(원본은 바꾸지 않고 섹터 checksum을 다시 계산한다). 나무 ID 40은 섹터 경계에 걸쳐 거부된다.

```bash
# 90=31번도로(CHERI_1), 91=도라지시티(CHERI_2), 92=37번도로(CHESTO_1), 93=42번도로(CHESTO_2), 94=33번도로(PECHA_1), 95=고동마을(PECHA_2)
# 36=카리열매 37=오카열매 52=바리비열매 53=로셀열매 54=치리열매 61=의문열매 65=애터열매, 단계 5=열매, 수확 3개
python3 $B/sav_set_tree.py <hns.sav> <out.sav> $B/pre/save_layout.txt 90=36:5:3 91=37:5:3 92=53:5:3 93=52:5:3 94=65:5:3 95=54:5:3
```

`<out.sav>`를 이식 전 ROM과 이식 후 ROM에 각각 넣고 나무 그림·팔레트, "○○열매가 N개" 문구, 수확 아이템, 가방 열매 번호, 열매 태그 화면이 같은지 본다.

## 3. 블렌더 NPC 열매 표

```bash
bash $B/blender/run.sh "$PWD" blender.tsv && cmp blender.tsv $B/blender/blender_pre.tsv
```

- `src/berry_blender.c`에서 `SetOpponentsBerryData`와 표를 잘라 호스트에서 컴파일한다. 플레이어가 넣은 열매마다 NPC 1~3명이 고르는 열매를 적는다(432행).
- `blender_pre.tsv`는 이식 전 동작이다. HnS는 `SetOpponentsBerryData`의 `// HnS:` 줄로 이 동작을 유지한다. `upstream_unfixed.txt`는 upstream 코드를 그대로 두었을 때의 차이(버치·유루·복슝·복분·배리열매를 넣은 30행)다.
- HnS에서는 `FLAG_HIDE_LILYCOVE_CONTEST_HALL_BLEND_MASTER`가 0이라 `FlagGet`이 FALSE다. 그래서 `flagHidden=0` 행이 실제 동작이다.
- 이후 이 함수를 바꾸는 PR(예: seq 381 #10181)을 이식할 때 다시 돌려 같은지 본다.
