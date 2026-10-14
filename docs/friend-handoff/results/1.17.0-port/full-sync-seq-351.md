# full-sync 묶음 22 — seq 351 #9927 Mass outbreak (+ #10089·#10142 앞당김)

- 시작 HEAD `2d1e601c`(코드 = 묶음 21 끝 `49f2c5e5`, ROM SHA1 `6377af69…`). 작업: 클라우드 세션, 2026-10-14.
- 방식: XL·세이브 영향 Y라 단독 묶음으로 했다(seq 311 선례).
  - 사전 분석은 1갈래로 했다. 사본에서 거부 hunk 9개를 손으로 맞추고 빌드·전체 테스트·한글 회귀·세이브 정적 비교·출현 측정까지 돌렸다. 산출물은 세션 임시 `b23/`, 보고서는 `b23/seq351-9927.md`다.
  - 사용자 지시: "혼자 할 수 있는 것이라면 ㄱㄱ".
- 의존 PR 6개는 모두 이식돼 있다: #9713 `d057cee5`, #8943 `022e6668`, #8434 `9d0e2f51`, #9890 `5b878273`, #9920 `e3637df4`, #10051 `660496e0`.
- 실기(mGBA)는 아직 안 했다(친구가 함).

## 커밋

| seq | PR | 커밋 | 판정·HnS 적응 | ROM(B) |
|---|---|---|---|---:|
| 351 | #9927 Mass outbreak | `7c17b9f1` | 18파일(upstream 목록과 같음), 거부 hunk 9개 손 맞춤(아래). **HnS 적응:** TV 대량발생 쇼의 `outbreakIndex`를 `u8`로 둠(아래 "세이브"). 새 파일 5개의 파일 끝 빈 줄은 지웠다(seq 346 선례) | 32,787,012 |
| (353) | #10089 Fix debug trainer and flags vars submenu | `443a264d` | **앞당김**(353 → 351 바로 뒤), upstream 그대로. #9927이 만든 회귀 2개를 고침: 디버그 메인 메뉴 "Trainers…"·"Flags & Vars…"가 빈 하위 메뉴를 엶, Pokérus 균주 설정이 파티 번호를 덮어씀(`tStrain`과 밀린 `tPartyId`가 같은 `data[6]`) | 32,787,012 |
| (364) | #10142 Fix Debug Menu's `Fly to map` not working immediately | `4ae4d7e1` | **앞당김**(364 → 351 바로 뒤), upstream 그대로. #9927 뒤 `Debug_DestroyMenu_Full`이 `!= 0` 검사로 없는 창 0xFF를 지우던 것(`gWindows` 범위 밖)을 `WINDOW_NONE`/`SPRITE_NONE`으로 고침 | 32,787,012 |

- ROM 32,782,772 → **32,787,012 B(+4,240)**, EWRAM 250,408 B·IWRAM 25,516 B 그대로다.
- 커밋마다 `git diff --check` 통과, `make hns -j8` 종료 0, 새 경고 0.
- 최종 ROM SHA1 `008d5029…`는 사전 분석 사본(같은 코드를 한 패치로 넣고 테스트한 것)과 같다.
- 사전 분석은 #10089·#10142 9줄을 351에 합치자고 했다. 메인은 PR별로 커밋을 나눠 seq 351 바로 뒤에 앞당겨 넣었다. 결과 코드는 같다. 순서표 353·364행에는 "앞당김 적용"을 표시했다(그 순서에서 다시 넣지 않음).
- **커밋 메시지 정정:** `7c17b9f1` 메시지의 "only the Emerald Mauville script sets FLAG_SYS_TV_START"는 틀렸다. HnS 빌드는 `include/constants/flags_hns.h`가 `FLAG_SYS_TV_START`를 **`0`으로 정의**해서 `FlagGet`이 늘 FALSE다(사전 분석이 맞았고, 메인이 `flags.h` 값을 보고 잘못 고쳐 씀). 그 뒤 커밋이 쌓여 메시지는 그대로 두고 여기 정정한다.

### 손 맞춤 거부 hunk 9개

- `src/debug.c` 5:
  - 앞쪽 선언
  - 워프 맵 그룹·맵·워프 선택 3곳: `tOrigin == 0`이면 **HnS B 버튼 복귀 도우미**를 유지하고, `== 1`이면 upstream 대량발생 메뉴로 복귀한다. 맵·워프 선택의 B는 HnS가 단계마다 완전히 되돌리는 방식을 유지했다(upstream은 `func`를 안 바꾸는 결함).
  - `tCurrentSong` → `data[6]`
- `src/tv.c` 2: 앞쪽 선언, `TryPutSmartShopperOnAir`. HnS `GetTVMetatileId`, HnS 프런티어 상점 조건(`MAP_BATTLE_FRONTIER_MART_HNS`)은 그대로 뒀다.
- `src/wild_encounter_ow.c` 1: `level` 인자만 추가했다. OWE는 꺼져 있다(재확인 24 — 이동 타입 번호 안 건드림).
- `docs/SUMMARY.md` 1.
- 그대로 남은 HnS 디버그 항목: 트레이너 1 = 1부터, Fake RTC, Pokérus 균주, GBS 소리 메뉴, B 버튼 복귀.

## 세이브

- **SaveBlock1·2·3·PokemonStorage:** 크기 같음, 섹터 배치·`sGFRomHeader`·세이브 경로 코드(엄격 54·검토 18)·RAM 변수 같음. SaveBlock1의 `outbreak*` 필드 배치도 그대로다(의미만 "종 ≠ 없음" → "남은 날 > 0"이 활성 — HnS 세이브는 이 값이 늘 0이라 전후 모두 비활성).
- **TV 대량발생 쇼(`tvShows[].massOutbreak`):**
  - upstream은 새 필드를 `enum MassOutbreakIndex`로 두었다. 이 enum은 Makefile 플래그(`-fshort-enums` 없음)에서 4바이트다(사전 분석이 작은 TU로 `sizeof` 확인). 그래서 쇼 안 0x10부터 필드 9개가 밀리고, 옛 세이브의 쇼가 큰 인덱스로 읽힌다. upstream 자기 주석(`/*0x0E*/`, `/*0x0F*/ u8 unused2`)은 1바이트를 전제한다. upstream 1.17.1·master에도 같은 결함이 있다.
  - **HnS는 `u8`(값 = enum + 1, 0 = 옛 쇼)로 두어 바이트 배치가 그대로다**(`// HnS:` 주석, 재확인 68). 친구의 "세이브 호환성 보호" 원칙에 따른 것이고, 친구에게는 알림만 한다.
- **정적 비교**(`save_compat.py run`, 이식 전 사실 = `49f2c5e5` 빌드 `post-350`): **판정 FAIL 2(SaveBlock1, ASLR 래퍼) — 이름 변화뿐**. 차이 전부:

  ```
  -  96208    16 tvShows[].massOutbreak.unused2 u16
  +  96208     8 tvShows[].massOutbreak.outbreakIndex u8
  +  96216     8 tvShows[].massOutbreak.unused2 u8
  -  96272    16 tvShows[].massOutbreak.daysLeft u16
  +  96272    16 tvShows[].massOutbreak.daysBeforeOutbreak u16
  ```

  - 비트 위치·폭이 같은 칸을 나누고 이름을 바꾼 것뿐이다. 도구에 이름 변경 허용 규칙이 없어 FAIL로 찍힌다.
  - 그 밖 구조체·섹터·세이브 코드·RAM은 모두 PASS다.
  - 다음 묶음의 이식 전 사실은 `post-351`(이 빌드)이고, 다음 비교부터는 같은 이름이다.
- **기존 HnS 세이브(코드 해석):**
  - HnS는 전당 등록 뒤 야생전 끝마다 1/200로 대량발생 쇼를 만들 수 있다. TV가 방송하지 않아 슬롯에 남는다.
  - 그런 세이브를 새 ROM으로 불러오면, 옛 쇼의 0x0E(이전 `unused2`, 늘 0)가 `outbreakIndex` 0으로 읽혀 upstream "legacy" 경로를 탄다. 0x16 `daysLeft`는 같은 뜻의 `daysBeforeOutbreak`다. 완전 호환이다.
  - 세이브 왕복 도구(`savetest/run_all.sh`)는 돌리지 않았다.

## HnS 일반 진행 동작

- **변화 없음**(코드 해석 + 측정).
  - 대량발생은 TV 방송으로만 시작한다. HnS는 `FLAG_SYS_TV_START`·`FLAG_SYS_TV_WATCH`·`FLAG_SYS_TV_HOME`이 `0`(`flags_hns.h`)이다. 그래서 집 TV는 늘 `gText_MomOrDadMightLikeThisProgram`에서 끝난다.
  - 새 스크립트 명령(`startoutbreak` 등)은 HnS 맵 스크립트가 쓰지 않는다.
  - 설정값으로 켜고 끄는 기능이 아니다.
  - 새로 보이는 것은 디버그 메뉴 "Encounters… → Mass outbreak…"뿐이다.
- **HnS 고유 대량발생(스웜) 구현은 없다**(라디오·포켓기어·매치콜). 맵 대사 2곳(분노의호수·인주시 센터)은 HGSS 이야기 문장일 뿐 코드 연동이 없다. upstream은 Emerald 대량발생 코드를 `mass_outbreak.c`로 옮긴 것이라, HnS 기능과 겹치거나 충돌하지 않는다.
- **야생 출현:** 대량발생이 비활성이면 `DoMassOutbreakEncounterTest`가 난수 없이 FALSE다(디스어셈블 확인). 사전 분석 사본의 임시 테스트를 이식 전후에 같이 돌렸다.
  - 전 `gWildMonHeaders` 땅 표 × 시간대에서 924마리를 생성했고, 종·레벨·성격값·IV·끝 난수 상태 해시가 `1122699047`로 같다.
  - 비활성 상태에서 1000회 호출하면 TRUE 0번, 난수 상태 불변이다.
- **유일한 실제 차이(코드 해석):** 야생전 끝 TV 코드의 `!rbernoulli(1, 1)`(항상 참, `Random()` 1회)이 사라져 난수 소비가 1회 준다. 하루 지날 때 `TryPutWorldOfMastersOnAir`도 1회 준다. HnS는 VBlank마다 난수가 돌아 관찰되는 결과 차이는 없다.
- **dailySeed**(재확인 31)는 건드리지 않는다. HnS `UpdateDailySeed()`는 그대로 `gSaveBlock3Ptr->dailySeed`다.

## 새 텍스트

- 새 배틀 문장 0, 기존 문자열 수정 0.
- 배틀 외 새 텍스트는 디버그 메뉴 영문 14개뿐이고 영어 그대로다(정책 5절 5번): `Encounters…`, `Mass outbreak…`, `Set Static Outbreak`, `Clear Active Outbreak`, `Species: {STR_VAR_1}`, `Map: {STR_VAR_1}`, `Level: {STR_VAR_1}`, `Moves`, `Probability: {STR_VAR_1}`, `Days Left: {STR_VAR_1}`, `Set Dynamic Outbreak`, `ID: {STR_VAR_1}⏎{STR_VAR_2}…`, `Probability:{CLEAR_TO 90}…`, `Days Left:{CLEAR_TO 90}…`.
- 튜토리얼 문서 1개(영문).

## 친구에게 알릴 것 (결정 필요 없음)

1. 세이브: TV 대량발생 쇼 필드를 upstream `enum` 대신 `u8`로 두었다(세이브 호환 원칙, 재확인 68). 정적 비교 FAIL 2는 이름 변화뿐이다.
2. HnS 기능 활성화 없음: 대량발생은 일반 진행에서 계속 안 일어나고, 디버그 메뉴만 생긴다. HGSS식 스웜을 쓰려면 별도 요청.
3. #10089·#10142 앞당김(디버그 메뉴 회귀 — 친구가 디버그로 실기하므로).
4. 보고만 하는 upstream 결함 3개(HnS 진행에서는 닿지 않음):
   - `enum` 배치
   - TV 방송 `GetMapName` 셋째 인자에 맵 그룹을 넘김
   - upstream 워프 맵 선택의 B 버튼(HnS 복귀 코드 유지로 해당 없음)

## 검증

- **빌드:** 위 표.
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 `4ae4d7e1`): PASS 2,684 → **2,691**(목록 기준) / TOTAL 5,614 → 5,621. `test-baseline-seq350.txt` 대비 **새 PASS 7(`test/mass_outbreak.c` 7개), 사라진 PASS 0**, 그 밖 줄 변화 없음. 사전 분석 사본(-j6)에서 보였던 `Battle strings fit…` PASS는 실제 저장소 -j8에서는 기준과 같은 FAIL(샤드 배치 영향). 목록 [`test-baseline-seq351.txt`](test-baseline-seq351.txt)(**다음 비교 기준**).
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-b23 8`, 코드 `4ae4d7e1`): **532/622**, 기대 요약과 바이트 같음.
- **세이브 정적 비교:** 위 "세이브".
- **최종 ROM(실기용):** 코드 `4ae4d7e1`, 클라우드 `make hns -j8`(`arm-none-eabi-gcc 13.2.1`) → 사용 32,787,012 B, SHA1 `008d5029554bcae3c4f943edf585bbec2a8a9e1a`.

## 실기(mGBA) 후보

1. 디버그 메뉴(R+START) 메인:
   - "Trainers…"·"Flags & Vars…"가 정상 하위 메뉴를 여는지
   - "Encounters… → Mass outbreak…" 진입·나가기
2. 디버그 Utilities → 날기·워프:
   - 바로 동작하는지
   - 워프 맵 그룹·맵·워프 선택에서 B가 한 단계씩 돌아가는지
3. 디버그 Party → Pokérus 균주·일수, 친밀도 설정.
4. HnS 집 TV 문장이 이식 전과 같은지.
5. 전당 등록 뒤 세이브를 새 ROM으로 불러와 다음을 확인:
   - 야생 출현
   - 저장·다시 불러오기
6. (선택) 디버그로 HnS 도로에 동적 대량발생 → 출현·기술 확인 → "Clear Active Outbreak".

## 재확인·뒤 PR 메모

- 68(새): `outbreakIndex` `u8`. 뒤 PR이 이 필드·사용처를 바꾸면 맞출 것.
- 353·364: 앞당김 적용(다시 넣지 않음).
- 31·24: 영향 없음.
