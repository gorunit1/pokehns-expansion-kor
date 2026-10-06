# HnS 심향(Ethan)·금선(Lyra) 플레이어 그래픽 이식 작업 지시서

> 상태: **적용 완료(2026-10-06), 실기 확인 대기.** 결과: [`results/ethan-lyra-player-graphics-2026-10-06.md`](results/ethan-lyra-player-graphics-2026-10-06.md). 친구가 2026-10-06에 보낸 지시서 원문을 옮겼다([`FRIEND_REPLY_2026-10-06.md`](FRIEND_REPLY_2026-10-06.md) 3절).
>
> **사용자 추가 지시(2026-10-06): 남자 주인공(심향)의 배틀 앞모습은 바꾸지 않는다.** 아래 1-1의 12번(`ethan_front_pic.png` → `gold_hns.png`)과 2절의 "배틀 앞모습 팔레트"(`graphics/trainers/palettes/gold_hns.pal`), 10절의 Ethan 줄은 적용하지 않는다. 금선(Kris) 앞모습은 지시서대로 바꾼다.
>
> push: 사용자가 실기 확인을 위해 커밋을 단계별로 나눠 push하기로 정했다(2026-10-06). pull·rebase·reset은 하지 않았다.

## 0. 작업 기준

대상 저장소:

`gorunit1/pokehns-expansion-kor`

대상 브랜치:

`pokehns-expansion-kor`

확인한 현재 HEAD:

`4501e7cc47a4c992966c2daa13d14cff62f0a788`

그래픽/코드 원본:

`RafaPierangeli/pokemonemeraldrp`

브랜치:

`RHH-Expansion-Costume-Slawter`

확인한 기준 HEAD:

`bcc37fa723df74c68cf19f108195d0afb5550d5f`

이번 작업의 기본 대응은 다음과 같다.

- Ethan(심향) → 기존 HnS Gold 자리
- Lyra(금선) → 기존 HnS Kris 자리

새 플레이어 슬롯이나 Costume 시스템을 이식하는 작업이 아니다.
현재 Gold/Kris 슬롯과 파일명을 그대로 유지하면서 그래픽을 교체한다.

또한 이 작업에서는 기존 사용자 로컬 변경과 친구 작업을 임의로 섞지 않는다. `pull`, `push`, `rebase`, `reset`, `restore`, force push 등을 새로 지시하거나 수행하지 말고, 현재 작업 환경에서 수정·빌드·검증까지만 진행한다.

---

# 1. 가져올 PNG 26개

## 1-1. Ethan → Gold

| # | Pierangeli 원본 | HnS 목적지 |
|---|---|---|
| 1 | `graphics/object_events/pics/people/ethan/walking.png` | `graphics/object_events/pics/people/gold/walking_hns.png` |
| 2 | `graphics/object_events/pics/people/ethan/running.png` | `graphics/object_events/pics/people/gold/running_hns.png` |
| 3 | `graphics/object_events/pics/people/ethan/mach_bike.png` | `graphics/object_events/pics/people/gold/mach_bike_hns.png` |
| 4 | `graphics/object_events/pics/people/ethan/acro_bike.png` | `graphics/object_events/pics/people/gold/acro_bike_hns.png` |
| 5 | `graphics/object_events/pics/people/ethan/surfing.png` | `graphics/object_events/pics/people/gold/surfing_hns.png` |
| 6 | `graphics/object_events/pics/people/ethan/underwater.png` | `graphics/object_events/pics/people/gold/underwater_hns.png` |
| 7 | `graphics/object_events/pics/people/ethan/fishing.png` | `graphics/object_events/pics/people/gold/fishing_hns.png` |
| 8 | `graphics/object_events/pics/people/ethan/field_move.png` | `graphics/object_events/pics/people/gold/field_move_hns.png` |
| 9 | `graphics/object_events/pics/people/ethan/watering.png` | `graphics/object_events/pics/people/gold/watering_hns.png` |
| 10 | `graphics/object_events/pics/people/ethan/decorating.png` | `graphics/object_events/pics/people/gold/decorating_hns.png` |
| 11 | `graphics/pokenav/region_map/ethan_icon.png` | `graphics/pokenav/region_map/gold_icon.png` |
| 12 | `graphics/trainers/front_pics/ethan_front_pic.png` | `graphics/trainers/front_pics/gold_hns.png` |
| 13 | `graphics/trainers/back_pics/ethan_back_pic.png` | `graphics/trainers/back_pics/gold_hns.png` |

Ethan 오버월드는 현재 Gold와 규격이 맞으므로 **PNG 및 팔레트 교체만 하고 Gold 애니메이션 코드는 유지**한다.

---

## 1-2. Lyra → Kris

| # | Pierangeli 원본 | HnS 목적지 |
|---|---|---|
| 1 | `graphics/object_events/pics/people/lyra/walking.png` | `graphics/object_events/pics/people/kris/walking_hns.png` |
| 2 | `graphics/object_events/pics/people/lyra/running.png` | `graphics/object_events/pics/people/kris/running_hns.png` |
| 3 | `graphics/object_events/pics/people/lyra/mach_bike.png` | `graphics/object_events/pics/people/kris/mach_bike_hns.png` |
| 4 | `graphics/object_events/pics/people/lyra/acro_bike.png` | `graphics/object_events/pics/people/kris/acro_bike_hns.png` |
| 5 | `graphics/object_events/pics/people/lyra/surfing.png` | `graphics/object_events/pics/people/kris/surfing_hns.png` |
| 6 | `graphics/object_events/pics/people/lyra/underwater.png` | `graphics/object_events/pics/people/kris/underwater_hns.png` |
| 7 | `graphics/object_events/pics/people/lyra/fishing.png` | `graphics/object_events/pics/people/kris/fishing_hns.png` |
| 8 | `graphics/object_events/pics/people/lyra/field_move.png` | `graphics/object_events/pics/people/kris/field_move_hns.png` |
| 9 | `graphics/object_events/pics/people/lyra/watering.png` | `graphics/object_events/pics/people/kris/watering_hns.png` |
| 10 | `graphics/object_events/pics/people/lyra/decorating.png` | `graphics/object_events/pics/people/kris/decorating_hns.png` |
| 11 | `graphics/pokenav/region_map/lyra_icon.png` | `graphics/pokenav/region_map/kris_icon.png` |
| 12 | `graphics/trainers/front_pics/lyra_front_pic.png` | `graphics/trainers/front_pics/kris_hns.png` |
| 13 | `graphics/trainers/back_pics/lyra_back_pic.png` | `graphics/trainers/back_pics/kris_hns.png` |

**Lyra PNG는 절대로 기존 Kris 폭에 맞춰 잘라내지 말 것.**

Lyra는 좌우 비대칭 디자인이라 동쪽 전용 프레임이 추가되어 있다. Pierangeli 원본의 전체 폭을 그대로 보존하고 아래 asymmetric 코드까지 함께 이식한다.

---

# 2. 팔레트 교체

PNG만 교체하지 말고 다음 8개 팔레트도 교체한다.

## Ethan / Gold

### 오버월드 일반 팔레트

Ethan `walking.png`의 16색 indexed palette를 그대로 추출하여:

`graphics/object_events/palettes/gold_hns.pal`

에 넣는다.

walking/running/bike/surfing/fishing/watering 등 Ethan 일반 그래픽은 같은 팔레트 인덱스 체계를 사용하므로 이것을 Gold 기본 팔레트로 사용한다.

### 오버월드 물 반사 팔레트

목적지:

`graphics/object_events/palettes/gold_reflection_hns.pal`

주의: Pierangeli에는 Ethan 전용 reflection `.pal`이 존재하지 않는다.

따라서 기존 Gold reflection 팔레트를 그대로 남겨두면 안 된다. Ethan의 새 normal palette와 색 인덱스가 맞지 않을 수 있다.

Ethan normal palette의 **16개 인덱스 순서는 절대로 바꾸지 않은 채**, 현재 HnS의 물 반사 팔레트 스타일에 맞춘 물빛/밝기 보정판을 만들어 위 파일에 넣는다.

즉:

`Ethan normal index 0 → Ethan reflection index 0`

`Ethan normal index 1 → Ethan reflection index 1`

...

형태로 인덱스 대응을 반드시 유지한다.

기존 `gold_reflection_hns.pal`은 색감의 참고 자료로만 사용하고, 기존 Gold 색을 그대로 복사하지 않는다.

### 배틀 앞모습 팔레트

`ethan_front_pic.png`의 indexed palette

→

`graphics/trainers/palettes/gold_hns.pal`

### 배틀 뒷모습 팔레트

`ethan_back_pic.png`의 indexed palette

→

`graphics/trainers/back_pics/gold_hns.pal`

---

## Lyra / Kris

동일한 방식으로:

`lyra/walking.png` palette
→ `graphics/object_events/palettes/kris_hns.pal`

새 Lyra reflection palette
→ `graphics/object_events/palettes/kris_reflection_hns.pal`

`lyra_front_pic.png` palette
→ `graphics/trainers/palettes/kris_hns.pal`

`lyra_back_pic.png` palette
→ `graphics/trainers/back_pics/kris_hns.pal`

---

## underwater 팔레트

Pierangeli Ethan/Lyra의 underwater 그래픽이 사용하는 팔레트와 현재 HnS의 공용 underwater 팔레트가 일치하므로, 우선:

`OBJ_EVENT_PAL_TAG_PLAYER_UNDERWATER`

및 공용 underwater palette는 변경하지 않는다.

실기에서 색이 깨지는 경우에만 다시 조사한다.

---

# 3. 파일명을 유지하므로 수정할 필요가 없는 부분

현재 HnS의:

`src/data/object_events/object_event_graphics.h`

에는 이미 Gold/Kris의 PNG와 `.pal` 경로가 모두 등록되어 있다.

목적지 파일명을 그대로 유지하기 때문에 **이 파일의 경로나 심볼명은 변경하지 않는다.**

마찬가지로 `src/event_object_movement.c`에도 Gold/Kris normal/reflection palette tag가 이미 등록되어 있다.

따라서 새 palette tag를 만들지 말고 기존:

- `OBJ_EVENT_PAL_TAG_GOLD_HNS`
- `OBJ_EVENT_PAL_TAG_GOLD_REFLECTION_HNS`
- `OBJ_EVENT_PAL_TAG_KRIS_HNS`
- `OBJ_EVENT_PAL_TAG_KRIS_REFLECTION_HNS`

를 그대로 사용한다.

---

# 4. Lyra asymmetric 애니메이션 이식

수정 대상:

`src/data/object_events/object_event_anims.h`

현재 HnS에는 `sAnimTable_Following_Asym`이라는 별개의 비대칭 팔로워 구조가 있지만, 이것은 Lyra 플레이어용 테이블과 다르다.

**기존 `Following_Asym`을 수정하거나 대체하지 말 것.**

Pierangeli에서 Lyra가 사용하는 다음 7개 플레이어용 테이블을 별도로 이식한다.

```text
sAnimTable_WalkRun_Asymmetric
sAnimTable_MachBike_Asymmetric
sAnimTable_AcroBike_Asymmetric
sAnimTable_Surfing_Asymmetric
sAnimTable_Underwater_Asymmetric
sAnimTable_Watering_Asymmetric
sAnimTable_Fishing_Asymmetric
```

그리고 이 테이블들이 참조하는 `_Asymmetric` helper animation들도 함께 가져온다.

주요 그룹은 다음과 같다.

### Normal / Run

```text
sAnim_FaceEast_Asymmetric
sAnim_GoEast_Asymmetric
sAnim_GoFastEast_Asymmetric
sAnim_GoFasterEast_Asymmetric
sAnim_GoFastestEast_Asymmetric

sAnim_RunSouth_Asymmetric
sAnim_RunNorth_Asymmetric
sAnim_RunWest_Asymmetric
sAnim_RunEast_Asymmetric
```

walking에 동쪽 프레임 3개가 추가되기 때문에 running 영역의 시작 인덱스도 밀린다. 그래서 East뿐 아니라 Run South/North/West도 Pierangeli의 asymmetric 버전을 사용해야 한다.

### Mach Bike

```text
sAnim_FaceEast_MachBike_Asymmetric
sAnim_GoEast_MachBike_Asymmetric
sAnim_GoFastEast_MachBike_Asymmetric
sAnim_GoFasterEast_MachBike_Asymmetric
sAnim_GoFastestEast_MachBike_Asymmetric
```

### Acro Bike

```text
sAnim_FaceWest_AcroBike_Asymmetric
sAnim_GoEast_AcroBike_Asymmetric
sAnim_GoFastEast_AcroBike_Asymmetric
sAnim_GoFasterEast_AcroBike_Asymmetric
sAnim_GoFastestEast_AcroBike_Asymmetric

sAnim_BunnyHopBackWheelEast_Asymmetric
sAnim_BunnyHopFrontWheelEast_Asymmetric
sAnim_StandingWheelieBackWheelEast_Asymmetric
sAnim_StandingWheelieFrontWheelEast_Asymmetric
sAnim_MovingWheelieEast_Asymmetric
```

### Surf

```text
sAnim_GetOnOffSurfBlobEast_Asymmetric
sAnim_FaceEast_Surfing_Asymmetric
sAnim_GoEast_Surfing_Asymmetric
sAnim_GoFastEast_Surfing_Asymmetric
sAnim_GoFasterEast_Surfing_Asymmetric
sAnim_GoFastestEast_Surfing_Asymmetric
```

### Underwater

```text
sAnim_FaceEast_Underwater_Asymmetric
sAnim_GoEast_Underwater_Asymmetric
sAnim_GoFastEast_Underwater_Asymmetric
sAnim_GoFasterEast_Underwater_Asymmetric
sAnim_GoFastestEast_Underwater_Asymmetric
```

### Watering

```text
sAnim_FaceEast_Watering_Asymmetric
sAnim_GoEast_Watering_Asymmetric
sAnim_GoFastEast_Watering_Asymmetric
sAnim_GoFasterEast_Watering_Asymmetric
sAnim_GoFastestEast_Watering_Asymmetric
```

### Fishing

```text
sAnim_TakeOutRodEast_Asymmetric
sAnim_PutAwayRodEast_Asymmetric
sAnim_HookedPokemonEast_Asymmetric
```

Pierangeli 쪽에는 총 43개의 위 helper가 있고 현재 HnS HEAD에는 이 exact `_Asymmetric` helper들이 없다.

---

# 5. 1.17.0에 맞춘 추가 수정: SPIN

여기서는 Pierangeli 코드를 단순 복붙하면 안 된다.

현재 HnS 1.17.0의:

`sAnimTable_BrendanMayNormal`

에는 Pierangeli의 구버전 테이블에는 없던:

```text
ANIM_SPIN_SOUTH
ANIM_SPIN_NORTH
ANIM_SPIN_WEST
ANIM_SPIN_EAST
```

가 존재한다.

Lyra에서 기존 `sAnim_Spin*`을 그대로 사용하면 East를 West의 좌우반전으로 표시하게 되므로 Lyra의 비대칭 디자인이 깨진다.

따라서 Lyra 전용 spin animation 4개를 추가한다.

프레임 순서는 다음 논리로 만들면 된다.

```text
Spin South: 0 → 9 → 1 → 2
Spin North: 1 → 2 → 0 → 9
Spin West : 9 → 1 → 2 → 0
Spin East : 2 → 0 → 9 → 1
```

여기서 `9`가 Lyra walking의 동쪽 정면 전용 프레임이다.

기존 `sAnim_Spin*`과 동일한 duration/loop 방식으로 만들고:

```text
sAnim_SpinSouth_Asymmetric
sAnim_SpinNorth_Asymmetric
sAnim_SpinWest_Asymmetric
sAnim_SpinEast_Asymmetric
```

를 정의한 뒤 `sAnimTable_WalkRun_Asymmetric`에:

```text
[ANIM_SPIN_SOUTH]
[ANIM_SPIN_NORTH]
[ANIM_SPIN_WEST]
[ANIM_SPIN_EAST]
```

도 반드시 등록한다.

즉 **Pierangeli의 구버전 asymmetric 테이블을 그대로 가져오는 것이 아니라 현재 HnS의 `sAnimTable_BrendanMayNormal` 기능을 모두 보존한 asymmetric 버전**을 만드는 것이 목표다.

---

# 6. StepAnimTable 등록

동일한 `src/data/object_events/object_event_anims.h`의:

`sStepAnimTables[]`

에도 다음 7개를 추가한다.

```text
sAnimTable_WalkRun_Asymmetric
sAnimTable_MachBike_Asymmetric
sAnimTable_AcroBike_Asymmetric
sAnimTable_Surfing_Asymmetric
sAnimTable_Underwater_Asymmetric
sAnimTable_Watering_Asymmetric
sAnimTable_Fishing_Asymmetric
```

각 항목의:

```c
.animPos = {1, 3, 0, 2},
```

는 Pierangeli와 동일하게 사용한다.

기존 HnS의 `Following` / `Following_Asym` / `BrendanMayNormal` 등록은 삭제하지 않는다.

---

# 7. Lyra pic table 수정

수정 대상:

`src/data/object_events/object_event_pic_tables.h`

여기서 중요한 점은 현재 HnS의:

```c
#define overworld_ascending_frames(ptr, width, height) \
    {.data = (u8 *)ptr, .size = ..., .relativeFrames = TRUE}
```

구조다.

즉 relative frame 방식인 테이블은 PNG가 길어져도 프레임을 하나씩 새로 등록할 필요가 없다.

따라서 다음 Kris 테이블은 **현재 형태를 그대로 유지해도 된다.**

```text
sPicTable_KrisNormal_hns
sPicTable_KrisMachBike_hns
sPicTable_KrisAcroBike_hns
sPicTable_KrisFishing_hns
```

Lyra PNG 전체를 넣고 animation frame 번호만 asymmetric 버전으로 맞추면 추가 프레임에 접근할 수 있다.

반대로 Surfing / Underwater / Watering은 현재 explicit mapping 방식이므로 수정해야 한다.

---

## 7-1. `sPicTable_KrisSurfing_hns`

현재 12-entry 구조를 Lyra 방식의 15-entry 구조로 확장한다.

raw PNG frame 순서는:

```text
0, 2, 4,
0, 0,
2, 2,
4, 4,
1, 3, 5,
6, 6, 7
```

즉 현재 12개 엔트리 뒤에 추가:

```c
overworld_frame(gObjectEventPic_KrisSurfing_hns, 4, 4, 6),
overworld_frame(gObjectEventPic_KrisSurfing_hns, 4, 4, 6),
overworld_frame(gObjectEventPic_KrisSurfing_hns, 4, 4, 7),
```

한다.

동쪽용 table position은 12~14가 된다.

---

## 7-2. `sPicTable_KrisUnderwater_hns`

현재:

```text
0,1,2,0,0,1,1,2,2
```

뒤에 Lyra 동쪽 프레임:

```text
3
```

을 추가한다.

최종:

```text
0,1,2,0,0,1,1,2,2,3
```

table position 9가 Lyra East 전용이다.

---

## 7-3. `sPicTable_KrisWatering_hns`

현재 9개 엔트리:

```text
0,2,4,1,1,3,3,5,5
```

뒤에:

```text
6,7
```

을 추가한다.

최종:

```text
0,2,4,1,1,3,3,5,5,6,7
```

table position 9~10이 Lyra East 전용이다.

---

# 8. Kris GraphicsInfo를 Lyra용 asymmetric table로 변경

수정 대상:

`src/data/object_events/object_event_graphics_info.h`

Gold 쪽은 손대지 않는다.

Kris 쪽에서 `.anims`만 다음과 같이 바꾼다.

| GraphicsInfo | 현재 | 변경 |
|---|---|---|
| `gObjectEventGraphicsInfo_KrisNormal_hns` | `sAnimTable_BrendanMayNormal` | `sAnimTable_WalkRun_Asymmetric` |
| `gObjectEventGraphicsInfo_KrisMachBike_hns` | `sAnimTable_Standard` | `sAnimTable_MachBike_Asymmetric` |
| `gObjectEventGraphicsInfo_KrisAcroBike_hns` | `sAnimTable_AcroBike` | `sAnimTable_AcroBike_Asymmetric` |
| `gObjectEventGraphicsInfo_KrisSurfing_hns` | `sAnimTable_Surfing` | `sAnimTable_Surfing_Asymmetric` |
| `gObjectEventGraphicsInfo_KrisUnderwater_hns` | `sAnimTable_Standard` | `sAnimTable_Underwater_Asymmetric` |
| `gObjectEventGraphicsInfo_KrisFishing_hns` | `sAnimTable_Fishing` | `sAnimTable_Fishing_Asymmetric` |
| `gObjectEventGraphicsInfo_KrisWatering_hns` | `sAnimTable_Standard` | `sAnimTable_Watering_Asymmetric` |

다음 두 개는 변경하지 않는다.

```text
KrisFieldMove_hns → sAnimTable_FieldMove
KrisDecorating_hns → sAnimTable_Inanimate
```

`.size`, `.width`, `.height`, `.paletteSlot`, `.oam`, `.subspriteTables` 등의 나머지 설정도 특별한 문제가 없는 한 그대로 유지한다.

특히 기존 KrisUnderwater의 다른 HnS 전용 설정도 이번 그래픽 작업과 무관하게 임의 수정하지 않는다.

---

# 9. 배틀 뒷모습 5프레임 대응

수정 대상:

`src/data/graphics/trainers.h`

현재 HnS:

```c
[TRAINER_PIC_GOLD_HNS]
{
    .frontPic = TRAINER_FRONT_PIC(...),
    .backPic = TRAINER_BACK_PIC(4, gTrainerBackPic_GoldHns,
                                gTrainerBackPicPalette_GoldHns,
                                sBackAnims_Hoenn),
}
```

및 Kris가 동일하게 `4 + sBackAnims_Hoenn` 구조다.

여기서 매우 중요한 점:

**`TRAINER_BACK_PIC` 첫 번째 인자는 프레임 수가 아니라 `yOffset`이다.**

현재 매크로 정의도:

```text
TRAINER_BACK_PIC(yOffset, sprite, pal, anim)
```

이다.

따라서 단순히 "5프레임이니까 5"라고 바꾸는 것이 아니다.

Pierangeli의 실제 Ethan/Lyra 설정을 확인하면:

```text
Ethan y_offset = 5
Lyra  y_offset = 5
```

이고, 두 캐릭터의 투척 animation은:

```text
frame 1 → 2 → 3 → 4 → 0
```

이다.

현재 HnS의 `sAnimCmd_Kanto`가 정확히:

```text
1 → 2 → 3 → 4 → 0
```

을 사용한다.

따라서 최종 변경은 다음이 맞다.

### Gold / Ethan

```c
.backPic = TRAINER_BACK_PIC(
    5,
    gTrainerBackPic_GoldHns,
    gTrainerBackPicPalette_GoldHns,
    sBackAnims_Kanto
),
```

### Kris / Lyra

```c
.backPic = TRAINER_BACK_PIC(
    5,
    gTrainerBackPic_KrisHns,
    gTrainerBackPicPalette_KrisHns,
    sBackAnims_Kanto
),
```

즉 결과만 보면:

```text
4 → 5
sBackAnims_Hoenn → sBackAnims_Kanto
```

이 맞지만,

- `4 → 5`는 **프레임 수 변경이 아니라 Pierangeli 원본의 Y 오프셋 적용**
- `sBackAnims_Kanto`가 실제 **5번째 frame index 4를 사용하는 투척 animation**

이라는 이유로 바꾸는 것이다.

새 별도 Ethan/Lyra back animation을 만들 필요는 없다.

---

# 10. 배틀 앞모습

다음 두 PNG만 교체한다.

```text
ethan_front_pic.png → gold_hns.png
lyra_front_pic.png  → kris_hns.png
```

현재 HnS의:

```text
TRAINER_PIC_GOLD_HNS
TRAINER_PIC_KRIS_HNS
```

상수 및:

```text
gTrainerFrontPic_GoldHns
gTrainerFrontPic_KrisHns
```

심볼은 그대로 둔다.

64×64 규격이 같으므로 별도 front-pic 코드 변경은 필요 없다.

팔레트만 반드시 함께 교체한다.

---

# 11. 지역맵 아이콘

```text
ethan_icon.png → gold_icon.png
lyra_icon.png  → kris_icon.png
```

둘 다 기존 목적지와 16×16 규격이 같고 HnS가 이미 성별에 따라 Gold/Kris 아이콘을 선택한다.

따라서 코드 수정 없이 PNG 교체만 한다.

---

# 12. 권장 작업 순서

문제가 생겼을 때 원인을 쉽게 찾을 수 있도록 한 번에 전부 섞지 말고 다음 순서로 진행한다.

### 1단계 — Ethan

- Ethan → Gold 13 PNG 교체
- Gold normal/front/back palette 교체
- Gold reflection palette 제작
- Gold backPic을 `yOffset 5 + sBackAnims_Kanto`로 변경
- 빌드

Ethan이 정상인 것을 먼저 확인한다.

### 2단계 — Lyra 그래픽

- Lyra → Kris 13 PNG 교체
- Kris normal/front/back palette 교체
- Kris reflection palette 제작

아직 asymmetric 코드를 완성하지 않은 상태에서 Lyra 화면이 깨져도 정상적인 중간 상태다.

### 3단계 — Lyra asymmetric

- `_Asymmetric` helper 정의 추가
- 7개 asymmetric table 추가
- HnS 1.17용 Spin 4종 추가
- `sStepAnimTables[]` 등록
- Surf/Underwater/Watering pic table 확장
- Kris GraphicsInfo 7개 `.anims` 변경
- 빌드

### 4단계 — Lyra battle back

- Kris backPic을 `yOffset 5 + sBackAnims_Kanto`로 변경
- 빌드

### 5단계 — 최종 실기 검사

전체 캐릭터 동작을 확인한다.

---

# 13. 필수 실기 체크리스트

## Ethan / Gold

- 걷기: 아래 / 위 / 왼쪽 / 오른쪽
- 달리기: 4방향
- 제자리 회전/회전 animation이 사용되는 이벤트
- 마하자전거 4방향
- 아크로자전거
- bunny hop
- 앞바퀴 들기
- 뒷바퀴 들기
- 이동 wheelie
- 파도타기 4방향
- 파도타기 탑승/하차
- 다이빙 4방향
- 낚시 4방향
- 물주기 4방향
- 필드기술
- 꾸미기
- 물에 비친 모습
- 지역맵 아이콘
- 배틀 앞모습
- 배틀 인트로 뒷모습
- 몬스터볼 투척 전 프레임
- 몬스터볼 투척 animation 전체
- 배틀 중 point animation이 있다면 그것도 확인
- 팔레트 깨짐/색 인덱스 이상 없음

## Lyra / Kris

위 항목 전부에 더해 특히 다음을 집중해서 확인한다.

- 서쪽을 보고 있을 때 정상
- 동쪽을 보고 있을 때 **서쪽 모습의 단순 좌우반전이 아님**
- East walking 전용 프레임 정상
- East running 전용 프레임 정상
- East Mach Bike 정상
- East Acro Bike 모든 동작 정상
- East Surf 정상
- East Underwater 정상
- East Fishing 정상
- East Watering 정상
- Spin 도중 동쪽 프레임이 올바른 Lyra 모습인지 확인

Lyra에서 머리 장식이나 가방 등의 위치가 동쪽 방향에서 반대로 뒤집혀 보이면 asymmetric 이식이 덜 된 것이다.

---

# 14. 빌드 오류가 발생했을 때 먼저 볼 곳

### `_Asymmetric` symbol undefined

`object_event_anims.h`에서 해당 helper 또는 table을 빠뜨렸는지 확인한다.

### animation은 빌드되지만 동쪽에서 잘못된 프레임이 나옴

`object_event_pic_tables.h`의 Surf / Underwater / Watering 추가 엔트리와 animation frame index를 확인한다.

### Lyra running이 엉뚱한 walking 프레임을 읽음

walking에 East용 3개가 추가되면서 running section이 뒤로 밀린 것을 반영했는지 확인한다.

Pierangeli의 `sAnim_RunSouth/North/West/East_Asymmetric`를 사용해야 한다.

### 배틀 투척 중 마지막 프레임이 깨짐

PNG가 실제 64×320인지 확인하고 `sBackAnims_Kanto`가 사용되고 있는지 확인한다.

### 배틀 뒷모습 높이가 이상함

`TRAINER_BACK_PIC` 첫 숫자가 프레임 수가 아니라 Y 오프셋임을 다시 확인한다.

Ethan/Lyra 원본 기준은 `5`다.

### 물 반사만 색이 이상함

normal palette가 아니라:

```text
gold_reflection_hns.pal
kris_reflection_hns.pal
```

의 인덱스 대응을 확인한다.

---

# 15. 이번 작업에서 가능하면 건드리지 않을 파일

특별한 컴파일 문제나 구조상 이유가 없다면 다음은 수정하지 않는다.

```text
src/data/object_events/object_event_graphics.h
src/event_object_movement.c
```

기존 Gold/Kris asset path와 palette tag를 그대로 재사용하기 때문이다.

이번 작업의 핵심 C/H 수정 파일은 원칙적으로 다음 4개다.

```text
src/data/object_events/object_event_anims.h
src/data/object_events/object_event_pic_tables.h
src/data/object_events/object_event_graphics_info.h
src/data/graphics/trainers.h
```

그리고 그래픽 26개 + palette 8개가 변경된다.

---

# 16. 크레딧

이번 에셋 출처 기록에는 최소한 다음 이름을 남겨 둔다.

```text
Ethan player sprites:
POKABBIE, RichardPT, robloxmaster376

Lyra player sprites:
RichardPT, robloxmaster376

pokeemerald-expansion adaptation / additional sprite work:
Rafa Pierangeli

Costume asset lineage:
Slawter666
```

Costume 시스템 자체는 이번에 가져오지 않지만 출처 계보 보존 차원에서 Slawter666도 기록해 두는 것을 권장한다.

---

# 17. 작업 완료 후 보고해 줬으면 하는 내용

적용이 끝나면 다음을 알려줘.

1. 실제 수정한 파일 목록
2. 각 파일에서 무엇을 바꿨는지
3. 빌드 성공 여부
4. 새 warning 발생 여부
5. Ethan 실기 검사 결과
6. Lyra 실기 검사 결과
7. 특히 Lyra East 방향에서 문제가 있었는지
8. 배틀 뒷모습 투척 animation이 5프레임 모두 정상인지
9. reflection palette가 정상인지
10. 작업 중 이 지시서와 현재 1.17.0 구조가 달라 추가 대응한 부분이 있었는지

중요: 현재 HnS 구조를 기준으로 필요한 부분만 이식하고, Pierangeli의 Costume Menu 시스템이나 별도 플레이어 슬롯 시스템 전체를 가져오지는 말아줘.
