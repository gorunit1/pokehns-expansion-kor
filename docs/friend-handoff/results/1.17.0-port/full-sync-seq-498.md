# full-sync 묶음 64 — seq 498 (#10374 기술 애니메이션 정리)

- 요청: 사용자 "ㄱㄱ 498 시작해 혼자해도 되는거라면"(2026-10-28).
- upstream: 읽기 전용 사본, #10374 `58b806c6eb`(1.17.0, 5파일, +146/−198). 같은 단위(`U-10374`)에 다른 seq 없음.
- 혼자 진행한 근거: 애니메이션 정리(새 문장 0). upstream 결함 1개(파도 팔레트)는 이식 전 동작을 지키는 HnS 보정(재확인 107). 독압정 그래픽은 HnS가 이미 켠 설정을 upstream이 독압정에도 적용한 것 — 실기 후보로 남김.

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 498 | #10374 | `7c19730f` | 배경 해제 서브루틴 `UnsetPsychicBg`·`UnsetHighSpeedBg`·`UnsetSkyBg`(본문 같음)를 `UnsetBackground` 하나로, 직접 쓴 해제 4줄도 그 호출로, `createleechseedsprite` → `create_leech_seed_sprite`, 파도 팔레트 선택을 표로(`ANIM_SURF_PAL_SURF_ORIGINAL`/`_NEW`), 독압정 그래픽이 `B_NEW_SPIKES_PARTICLE`를 따름, HnS 보정 1 | 32,960,292 |

이식 전(`5dd0b6c7`, 게임 코드 = `9d35c02a`) 32,960,420 B → **32,960,292 B(−128)**.

## 내용과 HnS 판단

- **적용 방법:** 파일별 3-way 병합(`git merge-file`). 충돌 2곳 — 미래예지 애니메이션: HnS는 seq 482(#10362) 형태(`choosetwoturnanim FutureSightSetUp FutureSightHit`, 착탄 애니는 기술 애니 안)이고 upstream은 그 이전 형태(`gBattleAnimGeneral_FutureSightHit` 등)를 고침 → HnS 형태 유지, 그 안의 `call UnsetPsychicBg` 2곳을 새 이름 `UnsetBackground`로(남은 옛 이름 0).
- **HnS 보정(upstream 결함, 재확인 107):** upstream은 `ANIM_SURF_PAL_SURF`를 `#if B_NEW_SURF_PARTICLE_PALETTE`로 `_NEW`/`_ORIGINAL` 중에 고르게 바꿨는데, 애니메이션 스크립트를 전처리할 때 `TRUE`가 정의되지 않아(`gba/defines.h`를 안 거침) 항상 `_ORIGINAL`(0)이 됨 — 실제 빌드 전처리 결과로 확인(`create_surf_wave palette=0`). 이식 전 C 코드는 0을 받으면 설정을 보고 새 팔레트를 골랐으므로, 그대로면 HnS(설정 TRUE)의 파도타기·파도 계열 애니가 원래 팔레트로 바뀜. 1.17.1·master 같은 코드. → `AnimTask_CreateSurfWave`에서 `_ORIGINAL`이고 설정이 TRUE면 `_NEW`(이전 `switch`와 같은 결과, `// HnS:` 주석).
- **바뀌는 화면(upstream 의도):** 독압정 스프라이트(`ANIM_TAG_TOXIC_SPIKES`)가 `B_NEW_SPIKES_PARTICLE == TRUE`면 새 압정 그래픽(`gBattleAnimSpriteGfx_NewSpikes`, 팔레트는 독압정 그대로). HnS 설정은 TRUE(압정뿌리기는 이미 새 그래픽) → 독압정도 같은 새 그래픽. 문장 변화 없음.
- 세이브: 영향 없음.

## 검증

- **빌드:** `make hns -j8` 종료 0, 전체 재빌드(`build/hns` 지우고) 로그로 새 컴파일 경고 0(`warncheck.sh`).
- **애니메이션 라벨별 비교**(`dev_scripts/hns_verify/animcmp/run.sh`, 이전 = 클론 `5dd0b6c7`): **1,842개 모두 같음, 다른 것 0** — 바뀐 라벨은 합쳐진 해제 서브루틴(옛 3개 → `UnsetBackground`)뿐. ELF 심볼 비교에서 뿔드릴·메가혼·코스믹파워·테라버스트(땅) 등이 2바이트씩 줄어든 것은 직접 쓴 해제 4줄을 `call UnsetBackground`로 바꾼 것(펼치면 같음). `AnimTask_CreateSurfWave` 코드 크기 변화·`gBattleAnimBgPalette_Surf`(32 B, 표에서 참조) 추가는 C 쪽 변경.
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `7c19730f`): 목록이 `test-baseline-seq492.txt`와 **바이트 같음**(INVALID 24).
- **한글 테스트:** 새로 쓰지 않음(애니 그래픽·팔레트는 배틀 테스트 러너가 검사하지 않음) — 라벨별 비교·전처리 결과 확인으로 대신.
- **세이브 정적 비교**(이전 = 클론 `5dd0b6c7` 빌드, SHA1 `f4e7a1d6…` = 묶음 63 기록과 같음): **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s498 8`): **555/652, `판정: PASS (기대와 같음)`, 요약 바이트 같음**.
- **ROM**(커밋 `7c19730f`): SHA1 `88b276f679710b6fd931db0f3d9e1c46d1163df5`, 사용 32,960,292 B(파일 33,554,432 B), 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 하지 않음(친구).** 후보 — ① 독압정 애니메이션이 새 압정 그래픽(압정뿌리기와 같은 모양, 독 색)인지 — 원치 않으면 되돌릴 수 있음, ② 파도타기·탁류·오물웨이브 파도 색이 이식 전과 같은지(파도타기는 새 팔레트), ③ 미래예지·코스믹파워 등 배경을 바꿨다 되돌리는 애니메이션.

## 다음

- seq 499 #10401(나뭇잎·시드플레어 `target_both` FALSE). 목표 1.17.1.
