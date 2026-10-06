# HnS 수정 — 2026-10-06 친구 답 반영 (문자열 4줄, 프런티어 로비 직원 번호)

친구 답장 [`FRIEND_REPLY_2026-10-06.md`](../../FRIEND_REPLY_2026-10-06.md)의 질문 2·4(재확인 10d·10e), `MENU_READ` 추가 요청, 배틀타워 질문 A·B를 HnS 수정 커밋 2개로 넣었다. upstream 이식과 섞지 않은 별도 커밋이다. 질문 1·3·5(재확인 3b·3c·10f)는 엔진 수정이라 다음 작업으로 남겼다(아래 "남은 것").

시작 HEAD: `4501e7cc47`(seq 175 완료). 작업 컴퓨터: 노트북(WSL, ARM 공식 툴체인 `/opt/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi`).

## 요약

| 항목 | 커밋 | 파일 | 내용 |
|---|---|---|---|
| 10d 파티 메뉴 메일 | `0276c7310f` | `src/data/party_menu.h` | `MENU_MAIL` `apdlf` → `메일` |
| `MENU_READ` | `0276c7310f` | `src/data/party_menu.h` | `메일을 읽는` → `메일을 읽는다` |
| 10e 4인 통신 멀티 오른쪽 상대 | `0276c7310f` | `src/battle_message.c` | `sText_LinkTrainer2SentOutPkmn2` `내보냈다!!` → `내보냈다!` |
| 질문 B 예/아니오 | `0276c7310f` | `src/data/script_menu.h` | `MultichoiceList_YesNo`(`MULTI_YESNO`)의 `"아니"` → `gText_No`(`아니오`) |
| 질문 A 배틀타워 직원 | `9126e5f8df` | `data/maps/BattleFrontier_BattleTowerLobby_hns/{map.json,scripts.inc}`, `data/maps/BattleFrontier_BattleDomeLobby_hns/{map.json,scripts.inc}` | HnS 로비 오브젝트에 `*_HNS` 번호 이름을 붙이고 HnS 스크립트가 그 이름을 쓰게 함 |

- 빌드(`9126e5f8df`): 종료 코드 0, **ROM 32,738,292 B(97.57%, −16 B) / EWRAM 250,132 B / IWRAM 25,516 B**. 고친 파일에서 나온 경고 0(전체 다시 빌드 경고 168줄은 수정하지 않은 파일의 기존 경고와 알려진 `libpng warning: bKGD`).
- 한글 소스 줄 변경: 위 문자열 4줄(새 문장 없음). 세이브 영향 없음(오브젝트 순서·내용은 그대로이고 `local_id`는 번호 이름만 만든다).
- 전체 테스트: 아래 "전체 테스트".

## 문자열 4줄 (`0276c7310f`)

- 네 줄 모두 2026-09-26 HNS 작업 트리 업로드(`1821fd6749`) 때부터 있던 값이다.
- `메일을 읽는다` 폭: `FONT_NORMAL` 한글 8 px × 6 + 공백 3 px = 51 px, 커서 자리 8 px를 더해 59 px. 메일 메뉴 창(`sMailReadTakeWindowTemplate`, 폭 8타일 = 64 px) 안에 들어가므로 창은 바꾸지 않았다.
- `아니`: 창이 잘린 것이 아니라 `MULTI_YESNO` 목록 문자열 자체가 `"아니"`였다. HnS의 다른 예/아니오 창과 같은 `gText_No`(`아니오`)를 쓴다. `MULTI_YESNO`는 스크립트 60곳에서 쓰이고, 창 폭은 문자열 폭으로 계산되며 화면 밖으로 나가면 왼쪽으로 당겨진다(`ScriptMenu_AdjustLeftCoordFromWidth`). 오른쪽 끝 가까이(왼쪽 좌표 23 이상)에서 띄우는 곳은 없다.

## 배틀타워·배틀돔 로비 직원 (`9126e5f8df`)

- 원인: 오브젝트 번호 상수 `LOCALID_*`는 `map.json`의 `local_id`로 자동 생성된다(`include/constants/map_event_ids.h`). HnS 로비 `map.json`에는 `local_id`가 없어서 HnS 스크립트가 Emerald 로비의 번호를 그대로 썼다. 그런데 HnS 로비는 Emerald 로비에서 오브젝트가 빠져 번호가 당겨져 있다.
  - 배틀타워 로비: Emerald는 싱글 1 / 기자 5 / 제자 6 / 더블 7 / 멀티 8 / 통신 멀티 9. HnS는 싱글 1 / 더블 6 / 멀티 7 / 통신 멀티 8(스크립트 없음)이다. 엘리베이터로 데려갈 때(`BattleFrontier_BattleTowerLobby_EventScript_SetAttendantTalkedTo_hns`) `VAR_LAST_TALKED`를 Emerald 번호로 다시 정하므로
    - **멀티:** 18번 칸의 통신 멀티 직원이 걷고, 플레이어는 멈춰 있는 멀티 직원을 뚫고 올라갔다(친구가 본 장면).
    - **더블:** 멀티 직원이 대신 걸었다.
    - **통신 멀티:** 9번 오브젝트가 없어 아무도 걷지 않았다.
    - 싱글만 맞았다.
  - 배틀돔 로비: Emerald는 싱글 1 / 더블 6. HnS는 Maniac이 빠져 더블이 5번이다. 더블 신청 때 직원이 움직이지 않았다.
- 수정: HnS 로비 오브젝트에 `LOCALID_TOWER_ATTENDANT_{SINGLES,DOUBLES,MULTIS,LINK_MULTIS}_HNS`(1·6·7·8), `LOCALID_DOME_ATTENDANT_{SINGLES,DOUBLES}_HNS`(1·5)를 붙이고 HnS 스크립트 8줄을 바꿨다. 빌드 뒤 생성된 값이 위와 같은지 확인했다.
- 원래 동작: Emerald 원본 로비는 번호가 맞아 담당 직원이 플레이어 앞에서 엘리베이터(문)까지 같이 걷는다. 이번 수정으로 HnS도 그 동작이 된다.
- 같은 문제를 다른 맵에서도 찾았다: HnS 맵 스크립트가 다른 맵의 `LOCALID_*`를 빌려 쓰는 89곳을 모두 대조했다. 위치·역할이 어긋난 곳은 위 두 로비뿐이고, 나머지 프런티어 맵은 오브젝트 순서가 Emerald와 같다.
  - 별건(프런티어 밖, 이번에 안 고침): `TinTower_RoofDay_hns`가 `WhirlIslands_LugiaChamber_hns`의 `LOCALID_KIMONO_1/4/MID`(3·5·4번)를 빌려 쓴다. 두 맵에서 3번·5번 기모노 소녀의 이름(스크립트)이 서로 다르다(TinTower 3번 = 미키, Lugia 3번 = 나오코). 의도대로 움직이는지 실기 확인이 필요하다(재확인 목록 25).

## 전체 테스트

- 명령: `PATH=/opt/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin:$PATH GITHUB_ACTION=1 make check BUILD=hns -j6`(`9126e5f8df`, 노트북 13분 10초, 로그 `build/port-check-hnsfix1006.log`)
- 결과: PASSED 2,419 / FAILED 2,244 / KNOWN_FAILING 9 / TOTAL 5,322, assertion·Killed 0. 표준 추출 목록 5,253줄이 `test-baseline-seq174.txt`와 **바이트 단위로 같다**(사라진 PASS 0). 기준 목록은 그대로 `test-baseline-seq174.txt`를 쓴다.
- 노트북과 데스크탑의 테스트 목록이 같다는 것도 이번에 다시 확인됐다(데스크탑 seq 174·175 기준과 동일).

## 실기 확인 (친구용)

1. 배틀타워 로비: 싱글·더블·멀티 신청 뒤 그 담당 직원이 플레이어와 같이 엘리베이터로 걷는지, 다른 직원을 통과하지 않는지
2. 배틀돔 로비: 싱글·더블 신청 뒤 담당 직원이 같이 걷는지
3. 배틀타워 멀티 파트너 방 등 예/아니오 창에 `아니오`가 다 보이는지(다른 예/아니오 창도 같은 목록을 쓴다)
4. 파티 메뉴: 메일을 지닌 포켓몬 → `메일` 항목, 그 안의 `메일을 읽는다`·`가져온다`·`그만둔다`가 창 안에 들어가는지
5. 4인 통신 멀티 오른쪽 상대 교체 문장 끝이 `!` 하나인지(통신 환경이 있을 때)

## 남은 것

- 질문 1(재확인 3b) 탈출버튼·탈출팩으로 들어온 춤추기, 질문 3(3c) 불복종 자해 HP, 질문 5(10f) 유니온룸·배틀타워 통신 멀티 교체 문장 직업+이름 형식: 엔진·배틀 메시지 수정이다. 데스크탑의 한글 회귀(328개)로 함께 확인하는 것을 권장한다.
- 그래픽 작업(심향·금선): [`../../ETHAN_LYRA_PLAYER_GRAPHICS.md`](../../ETHAN_LYRA_PLAYER_GRAPHICS.md), 시작 전 push 여부 확인.
