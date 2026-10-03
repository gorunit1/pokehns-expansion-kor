# #9475 트레이너 그림 데이터 비교 도구

seq 128 #9475(Refactor/trainer pic info) 이식 전후에 **모든 트레이너가 실제로 쓰는 그림 데이터**가 같은지 ELF에서 직접 대조한 도구와 결과다. 결과 문서: [`../full-sync-seq-128-128.md`](../full-sync-seq-128-128.md) "그림 데이터 비교".

## 파일

| 파일 | 내용 |
|---|---|
| `trainer_pic_verify.py` | 도구(파이썬 표준 라이브러리 + `arm-none-eabi-gcc`/`objcopy`). `run`·`dump`·`compare`·`src-check`·`make-map`·`selftest` |
| `old_dump.tsv.gz` | 이식 전 ELF(코드 `fdc110d528`, ROM SHA1 `817f500d…`, 데스크탑 툴체인)의 트레이너별 그림 튜플 2,591행 |
| `new_dump.tsv.gz` | 이식 후 ELF(`06c6bac8c2`, ROM SHA1 `4b3b96bf…`)의 같은 덤프 |
| `pic_name_map.tsv` | 옛 enum 이름 → 새 enum 이름 224행(upstream 대응표 + HnS 규칙: `FRONT_X_HNS` → `X_HNS`, 뒷모습 GOLD·KRIS·SILVER는 같은 이름, `BACK_LANCE_HNS` → `CHAMPION_LANCE_HNS`) |
| `player_cases.tsv` | 플레이어·고정 뒷모습 경우 16개의 옛 ID·새 ID·기대(same/changed). 옛 구조 ↔ 새 구조 비교(`run`)용 |
| `player_cases_new.tsv` | 같은 16개를 옛·새 칸 모두 새 이름으로 채운 사본. 새 구조끼리 비교(`compare --identity`)용 |
| `report.txt` | 2026-10-03 실행 결과(`RESULT: 차이 0 (OK)`) |

비교 값은 심볼 주소·태그·enum 숫자가 아니라 **내용**이다. 앞모습은 압축 그림 바이트 해시·팔레트 해시·머그샷 좌표·회전·애니메이션 명령 열, 뒷모습은 좌표(size·y_offset)·프레임 전체 해시·팔레트·애니메이션이다. 구조체 오프셋과 enum 값은 HnS Makefile 플래그로 헤더를 ARM 컴파일해서 얻는다(게임 ELF에 DWARF가 없음).

## 다시 돌리기

도구는 기본 경로를 저장소 밖 작업 폴더(`/home/hjm0725/hns-sync-work/chunk-128/`)로 잡는다. 다른 컴퓨터나 나중에는 인자로 경로를 준다. 저장소에는 아무것도 쓰지 않는다.

```bash
mkdir -p ~/tp-verify && cp docs/friend-handoff/results/1.17.0-port/trainerpic-9475/* ~/tp-verify/ && gunzip -f ~/tp-verify/*.gz
make hns -j8
python3 ~/tp-verify/trainer_pic_verify.py --repo "$PWD" --work ~/tp-verify/work dump --elf pokehns.elf --layout new --src "$PWD" -o ~/tp-verify/now_dump.tsv
python3 ~/tp-verify/trainer_pic_verify.py --repo "$PWD" --work ~/tp-verify/work --player ~/tp-verify/player_cases_new.tsv compare --identity ~/tp-verify/new_dump.tsv ~/tp-verify/now_dump.tsv
```

- 새 구조끼리(이식 후 덤프 ↔ 지금 덤프) 비교할 때는 `compare --identity`와 `--player player_cases_new.tsv`를 쓴다(`player_cases.tsv`를 주면 옛 이름을 찾느라 [3]에 MISSING 16건이 나온다). 2026-10-03에 같은 ELF로 확인한 기대 출력은 `RESULT: 차이 0 (OK)`다. INCGFX 전환(seq 500 #9881) 등 트레이너 그림 INCBIN·표를 건드리는 PR 뒤에 돌려 그림 데이터가 그대로인지 본다. 의도한 그림 추가·변경이 있으면 그 항목만 차이로 나와야 한다.
- 옛 구조 ↔ 새 구조 비교(`run`)는 이식 전 ELF가 필요하다. 이식 전 ELF는 저장소에 없고, 그 덤프가 `old_dump.tsv.gz`다(`run --old-dump`로 준다).
- 녹화 배틀 재생의 플레이어 뒷모습을 Gold/Kris로 바꾸면(결과 문서 "친구에게 물을 것" 1) `player_cases.tsv`의 `RECORDED_LINK_DRAW_MALE/FEMALE` 두 행을 `new_id=TRAINER_PIC_GOLD_HNS/KRIS_HNS`, `expect=changed`로 고친다.
- 도구는 데이터만 본다. 코드가 어느 ID를 고르는지, 태그·팔레트 슬롯, 위치 계산은 범위 밖이라 실기 확인으로 본다.
