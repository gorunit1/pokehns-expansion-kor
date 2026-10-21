# HnS 타이틀 화면 v3 — 검은 윤곽선 복원 + 새 로고 + GBA 팔레트 최적화

기준: 원격 `gorunit1/pokehns-expansion-kor` / 브랜치 `pokehns-expansion-kor` 타이틀 코드. 사용자 업로드 `HNS6(1).png`(154×52) 및 `하트앤소울.png`(105×22).

## 이번 v3 변경

1. `HNS6(1).png` 새 로고에서 기존 대비 변경된 35픽셀을 반영. 로고 좌표/크기는 불변.
2. 기존 변환의 `alpha == 255` 비교 오류를 `alpha >= 128`로 고쳐 **253~254 투명도의 검은 윤곽선 30픽셀 복구**. 미세한 반투명 잡티(알파 1~60)는 버림.
3. BG2/OBJ 전용 팔레트를 **실제 GBA RGB555 → 8비트 화면 변환에 가장 가까운 색**으로 반올림하여 PNG/팔레트에 반영. 이것이 8비트 원본 색상을 물리적으로 동일하게 만들지는 못한다.
4. `rayquaza.png`, `rayquaza.bin`의 **배경 그래픽/타일 배치는 그대로 유지**. 다만 새 빌드에서 OBJ/BG 팔레트 공간 확장 때문에 원래 필요했던 타일맵 팔레트 뱅크 14→15는 그대로 유지한다.
5. v2 스크립트에서 HnS 조건 없이 변경했던 `VERSION_BANNER_LEFT_X`와 `RIGHT_X`, 배경/OBJ 팔레트 크기를 이제 `#if IS_HNS`로 분기해 **다른 에메랄드 빌드는 원래 동작**하도록 수정.
6. HnS 배너 애니메이션 타깃 Y=61, 시작 Y=2, 좌측 OBJ 중심 X=165/우측 X=229는 유지. 로고 BG2 초기 오프셋 X=-29,Y=-32, 최종 Y=0 및 반짝임 Y=68은 그대로 두어 애니메이션 경로를 유지.

## 최종 자산 규격

- `graphics/title_screen/hns/pokemon_logo.png`: 256×64, 인덱스 P. 로고 좌상단 (14,10) (BG2의 화면 X 보정 +29 → 화면상 X=43,Y=10).
- `graphics/title_screen/hns/pokemon_logo.pal`: 240색 JASC-PAL (BG2 팔레트 0~239).
- `graphics/title_screen/hns/emerald_version.png`: 64×64 인덱스 P (64×32 OBJ 두 개, 위/아래로 저장), 144색 팔레트 사용. 화면상 합성 좌상단 X=133,Y=45. 하트앤소울 원본의 비투명 2,139픽셀 마스크 유지.
- HnS 배경 BG0 타일맵의 기존 Bank 14→15 이동은 RGB 팔레트가 충돌하지 않도록 필요한 v2 변경 사항이며 순수 배경 타일의 그래픽/좌표는 바꾸지 않는다.

## 이미 v2 적용된 `pokehns-test`에 v3 적용하기 (권장)

1. 이전 실행 중인 mGBA를 종료한다.
2. 다운로드한 v3 ZIP을 다운로드 폴더에 압축 해제한다.
3. WSL에서:
```
cd /mnt/c/Users/tjkel/Downloads/HnS_title_final_v3
python3 apply_to_test_clone.py /mnt/c/Users/tjkel/Desktop/decomps/pokehns-test
```
4. `DRY-RUN` 통과 후 실제 적용:
```
python3 apply_to_test_clone.py /mnt/c/Users/tjkel/Desktop/decomps/pokehns-test --apply
```
5. `pokehns-test`에서 `make hns -j4` 실행 후 mGBA로 타이틀 최종 정지화면, 초기 반짝임/로고 이동, 하트앤소울 내려오는 애니메이션, PRESS START 동작 확인.
6. 수정 전 버전 파일은 `_hns_title_screen_v3_backup`에 보관. 이전 v2 `_hns_title_screen_backup`은 보존. 메인 로컬과 원격에는 적용되지 않으며 Git 명령은 사용하지 않는다.

## 친구 저장소에 반영하는 경우 (테스트 PASS 이후에만)

아래 내용을 HnS 조건부로 포팅. 테스트 클론에서 검증된 상태만 친구가 원격 브랜치에 커밋한다.

- `src/title_screen.c`: HnS 일 때만 배너 X 중심값 165/229, HnS 배너 Y goal 61, BG 팔레트 16×16, OBJ 팔레트 9×16. 비-HnS 좌표 98/162, 배경 팔레트 15×16, OBJ 팔레트 1×16 유지.
- `graphics_file_rules.mk`: HnS `pokemon_logo.gbapal` 생성 `-num_colors 240`, HnS `emerald_version.gbapal` `-num_colors 144`.
- `graphics/title_screen/hns/rayquaza.bin`: 사용 중인 32×20 타일 팔레트 bank 14 → 15; 아래 12행은 0 패딩 유지. 각 타일 번호와 상하좌우 플립은 보존.
- `graphics/title_screen/hns/{pokemon_logo.png,pokemon_logo.pal,emerald_version.png}`: 제공 파일 교체.
- 반짝임 `StartPokemonLogoShine`, `SpriteCB_PokemonLogoShine`, BG2 타이틀 이동 오프셋, Press Start/저작권 표기, 배경 소스는 수정하지 말 것.

## 5비트 색상 제한과 정확도

GBA 팔레트 RGB555는 채널마다 32단계이므로 8비트 원본의 모든 RGB 값을 표현할 수 없다. 특히 목표 이미지가 `(248,248,248)`을 사용해도 에뮬레이터에서 사용 가능한 가장 가까운 값은 일반 RGB555 확장 출력 기준 `(247,247,247)` 정도로, 원본과 **한 단계 차이**가 남는다. 옛 팔레트의 `(248,248,248)`을 그대로 5비트로 내리면 31이 되어 `(255,255,255)`로 나올 수 있다. 이번엔 최대한 가까운 쪽으로 색을 재매핑했다.

이는 **색의 물리적 표현 한계**이지 로고 윤곽선이 빠지거나 좌표가 틀어지는 문제의 핑계가 아니다. PNG가 index 0 투명/불투명 픽셀 보존, 타일 정렬, 팔레트 뱅크, 레이어 순서를 만족하는지는 따로 검사한다. `preview_v3_simulated_not_emulator.png`는 기존 HnS mGBA 스크린샷에 예상 색을 합성한 **시뮬레이션**이며 실제 ROM 캡처로 검증이 필요하다.
