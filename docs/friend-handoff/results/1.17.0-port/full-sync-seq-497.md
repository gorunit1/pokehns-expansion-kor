# full-sync 묶음 63 — seq 497 (#9924 공통 애니메이션 함수 추가)

- 요청: 사용자 "ㄱㄱ 497 시작해 혼자해도 되는거라면"(2026-10-28).
- upstream: 읽기 전용 사본, #9924 `1a35ede7d5`(1.17.0, 2파일, +332/−712, XL). 단위 `U-animcommon-9924`(앞 seq 495·496 적용됨).
- 혼자 진행한 근거: 애니메이션 정리(새 문장 0). HnS 결정(seq 428 회복 반짝임)은 유지하고, 동작이 같은지는 애니메이션 라벨별 비교로 확인. 바뀌는 것은 upstream PR 자체의 작은 애니 수정 3개뿐.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 497 | #9924 | `9d35c02a` | 반복되는 애니 묶음을 매크로·공용 서브루틴으로(`create_powder_particles`·`create_particle_ring`·`create_flying_sand_crescents`·`create_overheat_flames`·`create_syrup_bomb`, `CreateElectricPuffs`·`CreateThunderWaves`·`CreateThunderboltOrb` 등 서브루틴 14개), HnS: `create_electric_puff_sprite` 매크로를 앞당김 | 32,960,420 |

이식 전(`12b75148`, 게임 코드 = `e254fc86`) 32,962,756 B → **32,960,420 B(−2,336)** — 같은 묶음을 서브루틴 하나로.

## 내용과 HnS 판단

- **적용 방법:** 3-way 병합(`git merge-file`). 매크로 파일은 충돌 없음, `data/battle_anim_scripts.s` 충돌 8곳(diff3로는 10곳):
  - **진흙쌓기(Shore Up):** HnS는 seq 428(#10416, 플랜 A — 회복 반짝임은 공통 회복 스크립트에서 한 번)에서 `call HealingEffect`를 지움. upstream은 그 줄을 남긴 채 모래 초승달을 매크로로 → 매크로는 받고 `call HealingEffect`는 계속 뺌. `HealingEffect`·`HealingEffect2` 호출 수가 이식 전 HnS와 같음(20·3).
  - **전기 연기 7곳(합체번개·충전·볼트태클·기가볼트해저드·라이트닝서프라이드):** HnS는 `createsprite gElectricPuffSpriteTemplate …` 원래 형태, upstream 이전은 `create_electric_puff_sprite` 매크로 — upstream 쪽(매크로·`call CreateElectricPuffs`)을 받음. 이 매크로는 pret#2290에서 온 것(expansion에선 순서표 뒤쪽 pret 합치기로 들어옴)이라 HnS에 없어 첫 빌드가 어셈블 오류 → **매크로 정의 3줄을 앞당겨 넣음**(`@ HnS:` 주석, 펼치면 원래 `createsprite`와 같음). 뒤에 pret 합치기(seq 500 등)를 받을 때 중복 정의가 되므로 그때 정리(재확인 106).
- **동작 확인 — 애니메이션 라벨별 비교(새 도구 `dev_scripts/hns_verify/animcmp/run.sh`):** ROM 크기가 바뀌어 바이트 비교를 못 해서, 이식 전·후 소스 모두 서브루틴 `call`(대소문자 무관)을 본문으로 펼친 뒤 각 트리의 매크로·어셈블러로 어셈블하고 라벨마다 바이트와 재배치(ARM REL 더할 값을 라벨+오프셋으로)를 비교:
  - **1,827개 같음, 4개 다름** — 모두 upstream PR이 직접 바꾼 것:
    - 불대문자(`gBattleAnimMove_FireBlast`·`FireBlastCross`): `delay 3`을 서브루틴 안으로 옮기며 마지막 십자 뒤에도 3프레임(바로 뒤 `waitforvisualfinish`).
    - 매직파우더(`gBattleAnimMove_MagicPowder`): 독가루·매직파우더 가루 스프라이트를 번갈아 쓰던 것이 매직파우더 스프라이트만(위치·속도·지연 같음).
    - 테라버스트 전기(`TeraBlastElectric`): 같은 자리에 두 번 만들던 번개 구슬 스프라이트가 한 번.
  - 이식 전에만 있는 라벨 4개(`DischargeElectrify` 등)는 upstream이 펼쳐 지운 서브루틴, 새 라벨 14개는 새 공용 서브루틴.
- 세이브: 영향 없음.

## 검증

- **빌드:** `make hns -j8` 종료 0(매크로 앞당김 뒤), 전체 재빌드(`build/hns` 지우고) 로그로 새 컴파일 경고 0(`warncheck.sh`).
- **애니메이션 라벨별 비교:** 위(1,827 같음 / 4 = upstream 수정).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `9d35c02a`): 목록이 `test-baseline-seq492.txt`와 **바이트 같음**(INVALID 24).
- **한글 테스트:** 새로 쓰지 않음(애니 스프라이트·지연은 배틀 테스트 러너가 검사하지 않음) — 라벨별 비교로 대신.
- **세이브 정적 비교**(이전 = 클론 `12b75148` 빌드, SHA1 `67a944b0…` = 묶음 61·62 기록과 같음): **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s497 8`): **555/652, `판정: PASS (기대와 같음)`, 요약 바이트 같음**.
- **ROM**(커밋 `9d35c02a`): SHA1 `f4e7a1d67f4a5a95cfce2dd0b776538fe9c625ef`, 사용 32,960,420 B(파일 33,554,432 B), 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 하지 않음(친구).** 후보 — 불대문자·매직파우더·테라버스트(전기) 애니메이션(위 3개), 전기 기술(충전·볼트태클·합체번개)·진흙쌓기·오버히트·시럽봄 애니메이션이 이식 전과 같은지.

## 다음

- seq 498 #10374(기술 애니메이션 정리). 목표 1.17.1.
