# 심향(Ethan)·금선(Lyra) 플레이어 그래픽 이식 결과 — 2026-10-06

지시서: [`../ETHAN_LYRA_PLAYER_GRAPHICS.md`](../ETHAN_LYRA_PLAYER_GRAPHICS.md)(친구, 2026-10-06). 사용자 추가 지시: **심향 배틀 앞모습은 바꾸지 않는다.** 실기 확인을 위해 커밋을 나눠 push했다(사용자 승인, 지시서 0절의 "push 하지 말 것"은 친구 로컬과 섞지 말라는 뜻으로 보고 pull·rebase·reset은 하지 않았다).

- 원본: `RafaPierangeli/pokemonemeraldrp` `RHH-Expansion-Costume-Slawter` `bcc37fa723df74c68cf19f108195d0afb5550d5f`(지시서 기준과 같음을 확인). 저장소 밖 스크래치 `/home/jinmo/hns-sync-work/sprites-ethan-lyra/`에 필요한 파일만 받았다.
- 시작 HEAD: `357eb06659`. 작업 컴퓨터: 노트북(WSL, ARM 공식 툴체인).

## 1·2. 커밋과 수정 파일

| 단계 | 커밋 | 파일 | 내용 |
|---|---|---|---|
| 1 심향 → Gold | `fe939d6456` | `graphics/object_events/pics/people/gold/*_hns.png` 10개, `graphics/pokenav/region_map/gold_icon.png`, `graphics/trainers/back_pics/gold_hns.png`, 팔레트 `graphics/object_events/palettes/gold_hns.pal`·`gold_reflection_hns.pal`, `graphics/trainers/back_pics/gold_hns.pal`, `src/data/graphics/trainers.h`, `CREDITS.md` | PNG 12개 교체(**앞모습 제외**), 팔레트 3개, Gold 뒷모습 `TRAINER_BACK_PIC(5, …, sBackAnims_Kanto)`(`// HnS:`), 출처 기록 |
| 2 금선 → Kris 그림 | `eb46975ff9` | `graphics/object_events/pics/people/kris/*_hns.png` 10개, `kris_icon.png`, `graphics/trainers/front_pics/kris_hns.png`, `graphics/trainers/back_pics/kris_hns.png`, 팔레트 `kris_hns.pal`·`kris_reflection_hns.pal`, `graphics/trainers/palettes/kris_hns.pal`, `graphics/trainers/back_pics/kris_hns.pal`, `CREDITS.md` | PNG 13개 교체(원본 전체 폭, 잘라내지 않음), 팔레트 4개 |
| 3 금선 비대칭 | `f24bfd60de` | `src/data/object_events/object_event_anims.h`, `object_event_pic_tables.h`, `object_event_graphics_info.h` | 원본 `_Asymmetric` 도우미 43개 + 회전 4개, 표 7개, `sStepAnimTables` 7개 등록, Kris 파도타기·다이빙·물주기 그림 표 확장(3·1·2개), Kris 그래픽 정보 7개 `.anims` 교체 |
| 4 금선 뒷모습 | `1a17daf00f` | `src/data/graphics/trainers.h` | Kris 뒷모습 `TRAINER_BACK_PIC(5, …, sBackAnims_Kanto)`(`// HnS:`) |

바꾸지 않은 것(지시서대로): `src/data/object_events/object_event_graphics.h`, `src/event_object_movement.c`(경로·팔레트 태그 그대로), Gold 애니메이션 코드, `sAnimTable_Following_Asym`, 공용 다이빙 팔레트(`player_underwater.pal`, 원본 다이빙 그림 팔레트와 16색이 모두 같음을 확인), KrisFieldMove(`sAnimTable_FieldMove`)·KrisDecorating(`sAnimTable_Inanimate`), 그래픽 정보의 `.size`·`.oam` 등 나머지 필드. **심향 앞모습 `graphics/trainers/front_pics/gold_hns.png`와 `graphics/trainers/palettes/gold_hns.pal`도 그대로다.**

### 그림 규격 (원본 → HnS 목적지)

- 심향 10장은 기존 Gold와 크기가 모두 같다(걷기 144×32 등).
- 금선은 동쪽 전용 프레임만큼 넓다.

| 그림 | 기존 Kris | 금선 | 늘어난 프레임 |
|---|---|---|---|
| 걷기·달리기(16×32 프레임) | 144 | 192 | +3 |
| 마하자전거(32×32) | 288 | 384 | +3 |
| 아크로자전거 | 864 | 1024 | +5 |
| 파도타기 | 192 | 256 | +2 |
| 낚시 | 384 | 512 | +4 |
| 물주기 | 192 | 256 | +2 |
| 다이빙·필드기술·꾸미기 | 같음 | 같음 | 다이빙은 원래 4프레임 중 4번째가 동쪽 |

- 배틀 뒷모습은 두 사람 모두 64×320(5프레임), 앞모습 64×64, 지역맵 아이콘 16×16이다.
- 금선 오버월드 그림 9장(다이빙 제외)은 `walking.png`와 같은 16색 팔레트를 쓴다(심향도 같음).

## 3·4. 빌드와 경고

| 단계 | 종료 코드 | ROM | EWRAM / IWRAM |
|---|---|---:|---|
| 이식 전(`357eb06659`) | 0 | 32,738,292 B | 250,132 / 25,516 B |
| 1 | 0 | 32,740,340 B(+2,048, 뒷모습 4→5프레임) | 같음 |
| 2 | 0 | 32,752,020 B(+11,680, 금선 동쪽 프레임·앞모습) | 같음 |
| 3 | 0 | 32,753,620 B(+1,600, 애니메이션 표) | 같음 |
| 4 | 0 | 32,753,620 B(0) | 같음 |

- 최종 SHA1(노트북 툴체인): `60542673d56a8cc54db98c844b3dfe4f99b4cf6e`
- **새 경고 0.** 원본 PNG의 `bKGD` 청크가 16색 밖 인덱스(115, 255, 112)를 가리켜 `libpng warning: bKGD: invalid index`가 7개 새로 났다. 복사한 PNG에서 이 청크만 지웠고, 나머지 청크(IHDR·PLTE·IDAT 등)는 원본과 바이트 단위로 같음을 확인했다. 남은 `region_map.c` override-init 경고 122줄은 이식 전과 같다.
- 전체 테스트: 아래 "전체 테스트".

## 5~8. 실기 확인

**하지 않았다(화면 확인 불가). 친구 실기 확인이 필요하다.** 대신 코드·데이터로 다음을 확인했다.

- **금선 동쪽 프레임(7):** Kris 그래픽 정보 7개의 애니메이션 표가 참조하는 모든 프레임 번호가 실제 그림 프레임 수 안에 있다. 동쪽 애니메이션 가운데 좌우 반전(`hFlip`)을 쓰는 것은 0개다.

  | 그래픽 정보 | 표 | 그림 프레임 | 최대 참조 |
  |---|---|---:|---:|
  | KrisNormal | `sAnimTable_WalkRun_Asymmetric` | 24 | 23 |
  | KrisMachBike | `sAnimTable_MachBike_Asymmetric` | 12 | 11 |
  | KrisAcroBike | `sAnimTable_AcroBike_Asymmetric` | 32 | 31 |
  | KrisSurfing | `sAnimTable_Surfing_Asymmetric` | 15 | 14 |
  | KrisUnderwater | `sAnimTable_Underwater_Asymmetric` | 10 | 9 |
  | KrisFishing | `sAnimTable_Fishing_Asymmetric` | 16 | 15 |
  | KrisWatering | `sAnimTable_Watering_Asymmetric` | 11 | 10 |

- **표 항목 보존:** 새 비대칭 표 7개의 항목 수와 이름이 원래 쓰던 표(`BrendanMayNormal` 28, `Standard` 20, `AcroBike` 40, `Surfing` 24, `Fishing` 12)와 같다. 빠진 항목(호출 시 멈춤)이 없다. `WalkRun_Asymmetric`에는 1.17.0의 `ANIM_SPIN_*` 4개가 들어 있다.
- **회전:** 지시서 순서대로 남 0→9→1→2, 북 1→2→0→9, 서 9→1→2→0, 동 2→0→9→1. 기존 `sAnim_Spin*`과 같은 2프레임 간격, `ANIMCMD_LOOP(1)`, `ANIMCMD_END`.
- **배틀 뒷모습(8):** 그림이 64×320(5프레임)이고 `sBackAnims_Kanto`가 대기 0번, 투척 1→2→3→4→0을 쓴다. 이전 4프레임 그림은 대기가 3번(`sAnim_GeneralFrame3`)이었다.

## 9. 물 반사 팔레트

- **1.17.0에서는 물 반사 색을 `*_reflection_hns.pal`에서 읽지 않는다.** 반사 스프라이트를 만들 때 본 팔레트에 연못 필터 `ApplyPondFilter()`(0번 투명색은 그대로, 나머지는 5비트 파란색 +10, 최대 31)나 얼음 필터를 씌워 그 자리에서 만든다(`src/field_effect_helpers.c` `LoadObjectRegularReflectionPalette`). 플레이어 반사 팔레트를 읽던 `LoadPlayerObjectReflectionPalette()`는 호출하는 곳이 없다. 그래서 게임에 보이는 반사는 새 본 팔레트에서 자동으로 맞는다.
- 그래도 지시서대로 파일을 새로 만들었다. 새 본 팔레트에 같은 연못 필터를 씌운 값이라 16개 인덱스가 1:1로 대응하고, 게임이 그리는 반사 색과도 같다.
- 참고: 기존 HnS `gold_reflection_hns.pal`은 본 팔레트와 인덱스가 맞지 않았다(7번 거의 검정 → 반사 자홍색, 8번 자홍색 → 반사 갈색 등). 엔진이 읽지 않아 화면에는 영향이 없었다.

## 10. 지시서와 1.17.0 구조가 달라 추가로 대응한 것

1. 물 반사 팔레트: 위 9절. 엔진이 쓰지 않으므로 같은 필터로 만든 값을 넣었다.
2. `bKGD` 청크 제거: 새 경고를 없애기 위해 메타데이터만 지웠다(픽셀·팔레트 그대로).
3. 회전 4개: 지시서 5절대로 새로 만들었다(원본 저장소에는 없음).
4. 심향 앞모습 유지: 사용자 추가 지시. 지시서 12절 1단계의 "front palette 교체"도 하지 않았다.
5. HnS Kris 걷기·달리기 그림은 두 장을 하나로 이은 오름차순 프레임(`overworld_ascending_frames`)이다. 금선 걷기가 12프레임이므로 달리기가 12번부터 시작해 원본의 `sAnim_Run*_Asymmetric` 번호(12~23)와 맞는다.

## 전체 테스트

- `1a17daf00f`에서 `make check BUILD=hns -j6`(노트북 7분 59초, 로그 `build/port-check-sprites1006.log`): PASSED 2,419 / FAILED 2,244 / KNOWN_FAILING 9 / TOTAL 5,322, assertion·Killed 0.
- 표준 추출 목록 5,253줄이 `test-baseline-seq174.txt`와 **바이트 단위로 같다**(그래픽 변경은 테스트 결과에 영향 없음).

## 실기 체크리스트 (친구용)

지시서 13절을 그대로 쓴다. 특히 다음을 먼저 봐 줘.

1. 금선 동쪽: 걷기·달리기·마하·아크로(모든 묘기)·파도타기(타기·내리기 포함)·다이빙·낚시·물주기에서 서쪽 모습의 좌우 반전이 아닌지
2. 금선 회전(회전 동작이 나오는 이벤트)에서 동쪽 프레임
3. 두 사람의 배틀 인트로 뒷모습 높이, 몬스터볼 투척 5프레임 전체
4. 물에 비친 모습 색, 지역맵 아이콘, 금선 배틀 앞모습
5. 심향 배틀 앞모습이 **이전과 같은지**(바꾸지 않았다)
6. 녹화 배틀·통신·사파리처럼 플레이어 뒷모습이 나오는 다른 장면
