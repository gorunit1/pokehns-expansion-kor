# 친구 요청 2026-10-14(2): 웅 머그샷 — 로컬 실기 확인한 표시 범위 수정 적용

사용자가 2026-10-14 디스코드에서 전달한 원문이다. 대화에 첨부된 실기 화면 `pokehns-12.png`(배틀 전환 머그샷 — 웅 머리와 몸이 띠 아래까지 이어진 화면)는 사용자가 대화로 보여 줬고 저장소에는 넣지 않았다. 반영 결과는 [`results/1.17.0-port/friend-reply-2026-10-14.md`](results/1.17.0-port/friend-reply-2026-10-14.md).

---

# HnS 웅 머그샷 — 로컬 실기 확인한 표시 범위 수정 적용 요청

웅 머그샷을 로컬 테스트 ROM에서 수정해 확인했어. **첨부한 화면에서 머리 위치가 기존 HnS와 비슷해지고, 몸도 머그샷 띠 아래까지 이어져 의도대로 표시되는 것을 확인했어. 이 방식으로 적용해 줘.**

## 1. 기존 −8px 이동을 표시 범위 변경으로 교체

기존 `4b885582`의 `y2 -= 8` 방식은 그림뿐 아니라 표시 영역의 아래 경계도 위로 옮겨서, 몸이 띠 하단보다 먼저 끝나는 문제가 있었어.

이번에는 머그샷 표시 영역을 기존 위치에 유지하고, **웅 원본 그림의 8~39행을 읽도록 시작 위치만 변경**했어. `src/battle_transition.c`의 `Mugshots_CreateTrainerPics()` 마지막에 있는 웅 A/B의 `y2 -= 8` 처리 두 개를 아래 코드로 교체해 줘.

```c
// HnS: Brock's mugshot uses source rows 8..39, keeping the banner bounds fixed.
if (trainerAPicId == TRAINER_PIC_LEADER_BROCK_HNS)
{
    opponentSpriteA->sheetTileStart += 8;
    SetSpriteSheetFrameTileNum(opponentSpriteA);
}

if (opponentSpriteB != NULL
    && trainerBPicId == TRAINER_PIC_LEADER_BROCK_HNS)
{
    opponentSpriteB->sheetTileStart += 8;
    SetSpriteSheetFrameTileNum(opponentSpriteB);
}
```

기존 `y2 -= 8`은 제거하고 위 처리로 교체해 줘. 두 방식을 함께 적용하지 말아 줘.

## 2. 읽는 위치와 보존 범위

`sheetTileStart += 8`은 가로64px 그림의 한 타일 행을 건너뛰어 원본8px 아래부터 읽는 설정이야. 화면에서−8px 이동하는 것과 다른 처리이고, 기존2배 확대에서는 원본8px가 화면16px에 해당해.

이 시작 위치를 `SetSpriteSheetFrameTileNum()`으로 실제 표시에도 반영해 줘. `oam.tileNum`만 바꾸면 애니메이션 처리에서 되돌아갈 수 있으므로, 로컬에서 확인한 이 조합으로 적용해 줘.

다음 항목은 유지해 줘.

- 64×32 표시 크기·2배 확대·기본 좌표
- 슬라이드의 위치 판정·타이밍·표시/숨김 처리
- 웅의 원본PNG·팔레트와 일반 배틀 위치·그림
- 다른 트레이너와 파트너의 머그샷, 실버 멀티의 목호·이향 간격 보정

추가 그림이나 그림ID 없이 웅의 머그샷 스프라이트가 읽는 위치만 변경해 줘.

## 3. 로컬 검증 ROM과 실기 결과

이번 테스트는 아래 기준HEAD에 로컬 수정을 추가한 ROM이야. 미커밋 수정이므로 HEAD만으로 이번 ROM을 재현할 수 없다는 점도 기록해 줘.

- 기준HEAD: `267b8ec66778e06ac2411b5ef11a471499d67cd4`
- 로컬 변경: 위 웅 머그샷 A/B 처리와 설명 주석
- 빌드: `make hns -j16`, GCC 14.2.1, 종료0
- ROM SHA1: `6bb6acc048ae98e0290e0c5d74744b83c806023c`
- 실기 확인 화면: `pokehns-12.png`

이 화면의 머리 위치와 몸의 아래 경계는 사용자 실기PASS로 기록할게. 다만 이 한 장으로 일반·OBC 양쪽 이벤트나 슬라이드 전체·일반 배틀·다른 트레이너까지PASS로 확대한 것은 아니므로 확인 범위는 구분해 줘.

## 4. 적용 후 검증과 보고

이 변경을 별도 커밋으로 적용하고, 변경diff가 지정한 머그샷 처리에 한정되는지 확인해 줘. 빌드·기존 자동 테스트·한글 회귀·세이브 구조 비교를 진행하고 새 경고나 실패 여부를 보고해 줘.

일반 웅과OBC 이후 웅이 모두 이 그림ID를 사용하는 것, 상대B 분기도 같은 처리인 것, 참조 타일이 기존 시트 할당 범위 안인 것도 확인해 줘.

적용 커밋·새 ROM의 기준 커밋/SHA1·포함 변경·검증 결과를 알려 줘. 내 로컬 실기PASS와 네 빌드/자동 테스트 결과는 구분해서 보존해 줘. 통합 후ROM에서는 변경 포함과 동등성을 확인하고 실제 영향이 있는 미확인 조건을 추가 검사할게.

기존 한국어 텍스트·메가 특성·HnS 고유 기능·세이브 호환성 보호와 full-sync 순서는 그대로 유지해 줘.
