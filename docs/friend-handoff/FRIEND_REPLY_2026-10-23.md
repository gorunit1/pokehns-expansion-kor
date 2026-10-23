# 친구 답 2026-10-23: BGM 추가 불일치 7곳 수정·4곳 유지, 금빛 로켓단 점거 음악을 라디오타워만으로

사용자가 2026-10-23에 전달한 원문이다. 2026-10-22 BGM 반영 문자(`results/bgm-hgss-2026-10-22.md` 8·9절)에 대한 답이다. 묶음 25 문자(재확인 79)와 재확인 70·71·72, 실기 결과는 다루지 않는다. 첨부 파일은 없다.

---

HnS BGM 수정 결과 확인했어. 추가 발견된 불일치와 금빛시티 로켓단 점거 음악도 HGSS 원작 기준으로 통일하기로 결정했어.

앞서 완료한 BGM 변경은 유지하고, 아래 사항들을 추가로 수정해 줘.

### 1. 추가 발견된 장소별 BGM 불일치 수정

다음 장소는 HGSS 원작에서 사용하는 BGM으로 변경해 줘.

| HnS 맵 | 현재 BGM | 변경할 BGM |
|---|---|---|
| `GoldenrodCity_BikeShop_hns` | `MUS_HG_POKE_MART` | `MUS_HG_GOLDENROD` |
| `GoldenrodCity_DepartmentStoreBasement_hns` | `MUS_HG_POKE_MART` | `MUS_HG_GOLDENROD` |
| `GoldenrodCity_DepartmentStoreElevator_hns` | `MUS_HG_GOLDENROD` | `MUS_HG_POKE_MART` |
| `ViridianCity_House1_hns` | `MUS_HG_PALLET` | `MUS_HG_PEWTER` |
| `ViridianCity_House2_hns` | `MUS_HG_PALLET` | `MUS_HG_PEWTER` |
| `WhirlIslands_LugiaChamber_hns` | `MUS_HG_UNION_CAVE` | `MUS_NONE` |
| `PokemonLeague_HallOfFame_hns` | `MUS_HG_HALL_OF_FAME` | `MUS_HG_POKEMON_LEAGUE` |

상록시티에서 `MUS_HG_PEWTER`를 사용하는 것은 HnS의 기존 도시 BGM 대응 관계에 따른 것이므로 그대로 적용해 줘.

소용돌이섬 루기아 방은 HGSS 원작처럼 기본 BGM을 무음으로 변경하되, 루기아 등장 연출 및 전투 음악은 유지해 줘.

전당등록 방도 HGSS 원작의 `SEQ_GS_CHAMPROAD`에 맞춰 기본 BGM을 `MUS_HG_POKEMON_LEAGUE`로 변경해 줘. 다만 `src/hall_of_fame.c`에서 실제 전당등록 화면에 재생하는 `MUS_HG_HALL_OF_FAME`은 그대로 유지해 줘.

### 2. 금빛시티 로켓단 점거 BGM — HGSS 원작 방식 적용

현재 HnS는 로켓단 점거 중 금빛시티 시내와 지하통로, 스위치방, 창고 등에서도 `MUS_HG_ROCKET_TAKEOVER`가 재생되도록 처리하고 있어.

HGSS 원작의 `pret/pokeheartgold/src/sys_flags.c`에 있는 `sMusicOverrideMap`을 보면 점거 음악 `SEQ_GS_SENKYO`는 라디오타워 1~5층, 전망대, 엘리베이터에만 적용돼.

**HnS도 HGSS 원작과 동일하게 라디오타워에서만 점거 BGM이 재생되도록 변경해 줘.**

`src/overworld.c`의 `IsRocketTakeover()`에서 금빛시티 시내와 지하 관련 맵들을 점거 음악 대상에서 제외하고, 라디오타워에 해당하는 맵들만 남겨 줘.

| 장소 | 로켓단 점거 중 BGM |
|---|---|
| 금빛시티 시내 | `MUS_HG_GOLDENROD` |
| 지하통로 입구 | 해당 맵의 HGSS 기본 BGM |
| 지하통로 | `MUS_HG_GOLDENROD` |
| 지하 스위치방 | `MUS_HG_UNION_CAVE` |
| 지하창고 | `MUS_HG_UNION_CAVE` |
| 라디오타워 | `MUS_HG_ROCKET_TAKEOVER` |

라디오타워 전망대와 엘리베이터에 대응하는 HnS 맵이 있다면 해당 장소도 원작처럼 처리해 줘.

점거 상태가 아닐 때는 라디오타워도 해당 맵의 기본 BGM을 사용해야 해.

이전 커밋 `fc5ff3fc`에서 수정한 목적지 기준 음악 판정 오류는 그대로 유지하고, 점거 음악의 적용 범위만 HGSS에 맞춰 수정해 줘.

### 3. 변경하지 않을 장소

다음 장소는 HnS의 현재 BGM을 그대로 유지해 줘.

- **노랑시티 격투도장 본관** `SaffronCity_FightingDojo_hns` — `MUS_HG_GYM`
- **노랑시티 격투도장 VIP룸** `SaffronCity_FightingDojoVIP_hns` — `MUS_HG_DANCE_THEATER`
- **성도 사파리존 내부** — `MUS_HG_SAFARI_ZONE`
- **달맞이산 광장** `MtMoon_Outside_hns` — `MUS_HG_MT_MOON_SQUARE`
- **달맞이산 광장 상점** `MtMoon_Shop_hns` — `MUS_HG_MT_MOON_SQUARE`

노랑시티 격투도장은 HnS에서 노력치 훈련 시설로 확장됐으므로 체육관 BGM을 유지하기로 했어. VIP룸의 체육관 관장 재대결 기능과 음악도 변경하지 말아 줘.

성도 사파리존 역시 현재 사파리존 전용 BGM을 유지하고 `MUS_HG_ROUTE34`로 변경하지 말아 줘.

DexNav는 현재 `DEXNAV_ENABLED FALSE`이므로 이번 작업에서는 수정하지 않아도 돼.

### 4. 기존 BGM 수정 사항 유지

앞서 완료한 다음 변경들은 그대로 보존해 줘.

- 관동 포켓몬센터 11곳 → `MUS_HG_POKE_CENTER`
- 관동 체육관 8곳 → `MUS_HG_GYM`
- 연분홍시티 사파리존 내부 4곳 → `MUS_HG_SAFARI_ZONE`
- 28번도로·28번도로 집·은빛산 외부 → `MUS_HG_POKEMON_LEAGUE`
- 방울탑 옥상 낮·밤 → 은빛산 정상과 동일한 바람소리
- 방울소리좁은길 → `MUS_NONE`
- 벼랑끝 게이트 → `MUS_HG_ROUTE34`
- 매몰탑 → `MUS_HG_UNION_CAVE`
- 알프의 유적 퍼즐방·문자방 → `MUS_HG_UNION_CAVE`
- 금빛시티 지하 스위치방·창고의 기본 BGM → `MUS_HG_UNION_CAVE`

특히 28번도로 ↔ 은빛산 외부, 28번도로 ↔ 집처럼 같은 BGM을 사용하는 장소 사이의 연속 재생 처리는 유지해 줘.

### 5. 작업 범위 및 결과 보고

기존 이벤트 진행, 전설의 포켓몬 등장 연출, 전투 음악 및 전당등록 음악에 영향을 주지 않는 범위에서 수정해 줘.

변경이 끝나면 다음 사항을 정리해서 알려줘.

1. 수정한 파일 및 맵 목록
2. 변경 전후 BGM ID
3. `IsRocketTakeover()`의 최종 적용 대상
4. 맵 간 BGM 전환 및 연속성 정적 확인 결과
5. 기존 이벤트 음악에 영향을 줄 가능성이 있는 사항

**이번에는 코드 수정과 정적 확인까지만 진행해 줘. 빌드와 mGBA 실기 검사는 내가 직접 할게.**
