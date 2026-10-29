# full-sync 묶음 71 — seq 505 (#10184 안 쓰는 타일셋 그림 지우기)

- 요청: 사용자 "full-sync seq 502 #10116부터 port_sequence 순서대로 이어서 (목표 1.17.1)"(2026-10-29).
- upstream: 읽기 전용 사본, #10184 `53f5218dfd`(1.17.0, 1파일 +47/−57). 같은 단위 `U-incgfx-9881`.
- 혼자 진행한 근거: 안 쓰는 데이터 정리(문장·동작 0). 판정 기준 = 심볼·크기 같음, 차이는 배치뿐(충족).

## 커밋

| seq | PR | 커밋 | 내용 | ROM(B) |
|---|---|---|---|---|
| 505 | #10184 | `1b640f80` | `src/data/tilesets/graphics.h` — 안 쓰는 비밀기지 압축 타일 6개·`gTilesetTiles_UnknownSecretBase` 삭제, 비밀기지 타일셋을 파일 앞으로, 호연 블록을 `#if !IS_FRLG`로 감쌈(HnS는 `#endif`로 닫음) | 32,977,012(같음) |

이식 전(`faf5cf81`, 게임 코드 = `dda68a3f`) SHA1 `ed4348cc…` → 이식 후 SHA1 **`fdbe7fd767b0b9b9eb564af5a9a4962e72a1958c`**(크기 같음, 배치만 다름).

## 내용과 HnS 판단

- **지워지는 심볼 7개**(`gTilesetTiles_SecretBase{BrownCave,Tree,Shrub,BlueCave,YellowCave,RedCave}Compressed`, `gTilesetTiles_UnknownSecretBase`)는 HnS 저장소 어디서도 안 씀(검색 0). 이식 전 ELF에도 없음(링크에서 이미 빠짐).
- **구조 충돌 1곳:** upstream은 호연 타일셋을 `#if !IS_FRLG … #else <FRLG> #endif`로 나눔. HnS는 같은 자리에 HnS가 더한 `gTilesetTiles_Lab_Frlg`(조건 없음)와 `#if IS_FRLG <FRLG> #elif IS_HNS <HnS 타일셋> #endif`가 있어 그대로 받으면 `#else` 뒤에 `#elif`가 됨. → 호연 블록을 `#endif // !IS_FRLG`로 닫고 Lab_Frlg·FRLG·HnS 블록은 이식 전 조건 그대로. HnS 빌드(IS_HNS, !IS_FRLG)에 들어가는 타일셋 집합은 이식 전과 같음.
- 이 블록 밖은 1.17.1 파일과 같음(1.17.1에서 이 파일은 #10184 뒤로 안 바뀜).
- **ROM 비교:** SHA1은 다르지만 새 도구 `dev_scripts/hns_verify/port/relocmp.py`로 이식 전(`dda68a3f` 클론 빌드)·후 ELF를 심볼별로 비교 — ROM 심볼 96,782개 이름·크기·종류 모두 같고, 옮겨진 것은 타일셋 데이터 24개(비밀기지·컨테스트·포켓몬센터 등 파일 안 순서)와 링커 중계 스텁 39개뿐. 내용이 다른 심볼은 모두 같은 대상(심볼+오프셋)을 가리키는 BL·포인터 차이(타일셋 구조체 13개의 그림 포인터, 모래기둥 팔레트 포인터 등)이고, 남는 40개는 중계 스텁 35개와 그걸 부르는 ARM 함수 5개(`__udivsi3`·`__divsi3`·`__umodsi3`·`__modsi3`·`RandomRollDmg`)뿐. seq 494와 같은 꼴.
- 세이브: 영향 없음.

## 검증

- **빌드:** `make hns -j8` 종료 0. 저장소 전체 재빌드(`build/hns` 지우고): 종료 0, SHA1 `fdbe7fd7…`(첫 빌드와 같음), 새 컴파일 경고 0(`warncheck.sh`). 비교 도구도 이식 전 클론(`faf5cf81`) 빌드와 다시 돌려 같은 결과(심볼 96,782, 빠짐·크기 바뀜 0, 옮김 63, 설명 안 되는 40 = 스텁·ARM).
- **전체 테스트**(`GITHUB_ACTION=1 make check BUILD=hns -j8`, 코드 = `1b640f80`): 목록이 `test-baseline-seq504.txt`와 **바이트 같음**(PASS 3,008, 새 PASS 0, 사라진 PASS 0).
- **한글 테스트:** 새로 쓰지 않음(안 쓰는 데이터 삭제·배치 변경).
- **세이브 정적 비교**(이전 = 클론 `faf5cf81` 빌드): **PASS(FAIL 0, WARN 0)**.
- **한글 회귀**(`ALLOW_REPO=1 kortests/run.sh <저장소> kor-s505 8`): **555/652, `판정: PASS (기대와 같음)`, 요약 바이트 같음**.
- **ROM**(커밋 `1b640f80`): SHA1 `fdbe7fd767b0b9b9eb564af5a9a4962e72a1958c`, 사용 32,977,012 B(묶음 70과 같음), 파일 33,554,432 B, 클라우드 컨테이너 `make hns -j8`, arm-none-eabi-gcc 13.2.1.
- **실기: 새 후보 없음**(내용 같음, 배치만). 묶음 70 도감 후보는 이 ROM으로 봐도 됨.

## 다음

- seq 506 #10172(도감 공통 함수·변수를 따로 파일로 — 재확인 110 HnS 함수 위치 다시 맞춤), 507 #10334(컨테스트 값 정리). 목표 1.17.1.
