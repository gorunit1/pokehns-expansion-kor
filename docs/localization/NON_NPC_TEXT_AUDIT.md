# NPC 대화 외 잔여 텍스트 인벤토리

## 2026-09-27 조사 범위와 판정 기준

이 문서는 NPC와 직접 대화할 때 나오는 맵 스크립트 대사를 제외하고, 플레이어가 메뉴·시스템·배틀·도감·시설에서 볼 수 있는 영문을 다음 작업 단위로 나누기 위한 인벤토리다. 소스는 실제 현재 `pokehns-expansion-kor` 작업 트리를 기준으로 확인했다.

- C/H의 `_(...)`, `COMPOUND_STRING(...)`, 어셈블리 `.string`을 검색하고, 호출부 또는 화면 용도를 확인했다.
- 검색 수는 줄/초기화 항목 수다. 여러 줄 문자열은 하나의 문장일 수 있으므로, 번역 물량의 정확한 문장 수로 해석하지 않는다.
- 영문 약어·버튼명·제어 토큰(`{A_BUTTON}`, `{CLEAR_TO ...}` 등)과 디버그 전용 표기는 자동 번역 대상으로 넣지 않았다.
- 대규모 서사성 텍스트(TV·라디오·뉴스 등)는 NPC 대화는 아니지만, 우선 UI를 마친 뒤 별도 범위로 정한다. 인게임 트레이너 대사·맵 NPC 대사·Match Call 통화 본문은 이 문서의 범위에서 제외한다.

## 바로 나눠 작업할 우선순위

| 우선 | 화면/기능 | 실제 잔여 위치 | 근거와 주의 |
| --- | --- | --- | --- |
| P0 | PC·가방·필드 도구·파티 | `src/player_pc.c:171-194,230-232`, `src/item_menu.c:297,305-318,2845-2852`, `src/item_use.c:85,90-97`, `src/party_menu.c:7005`, `src/script_menu.c:1323`, `src/strings.c`의 공통 라벨 | 항상 접근 가능한 기본 UI다. PC 보관 시스템 자체는 현재 대부분 한글이며 이 파일의 확인된 잔여 메뉴명은 `MENU_ETCETERA = "etc."`뿐이다. 조사·이름 치환 수정 전의 PC 영문 목록은 현재 소스 상태와 다르므로 과거 기록으로만 취급한다. |
| P0 | 저장·획득·PC 전송 등 시스템 이벤트 | `data/text/{pc,pc_transfer,save,obtain_item,surf,check_furniture,record_mix,mart_clerk,abnormal_weather}.inc` | 이 9개 파일에 영문 `.string` 행이 112개 남아 있다. 저장 덮어쓰기 경고, 획득, 상자 전송은 NPC 대사가 아니라 시스템 메시지다. `\n`, `\p`, `{STR_VAR_n}`을 보존하고 창 너비를 함께 확인한다. |
| P0 | HGSS 도감·도감 서식·DexNav | `src/pokedex_plus_hgss.c:136-231,6733-...`, `src/pokedex_area_screen.c:660-699`, `src/dexnav.c:185-194` | 능력치 약어, 포획률·친밀도·성장·알그룹, 진화/폼 조건, 시간대·서식·검색 안내가 영문이다. 특히 `pokedex_plus_hgss.c:6733` 이후는 조건에 따라 문장을 조합하므로 먼저 토큰과 폭을 표로 대조해야 한다. |
| P0 | 포케기어(포케내비) | `src/pokenav_main_menu.c:95-115`, `src/pokenav_menu_handler_gfx.c:337-362`, `src/pokenav_match_call_gfx.c:124-130,207-209`, `src/pokenav_list.c:104-106`, `src/pokenav_match_call_list.c:39` | 메뉴 설명, 버튼 안내, 통화 목록 라벨/상태가 영문이다. Match Call의 **통화 본문**은 NPC 대사라 제외하지만, 이 UI 라벨은 포함한다. |
| P0 | 포케기어 픽셀 라벨 | `graphics/pokenav/hns/options/{hoenn_map,match_call,radio}.png`, `graphics/pokenav/hns/left_headers/{hoenn_map,match_call}.png`, `graphics/pokenav/options/*.png`, `graphics/pokenav/left_headers/*.png`, `graphics/pokenav/region_map/city_zoom_text.png` | 시각 확인으로 HNS 옵션에 `MAP`·`CALL`·`RAD`, HNS 통화 헤더에 `CALL`, 공용 그래픽에 `CONDITION`·`SWITCH OFF`·`MAIN MENU`·`CITY ZOOM` 등이 남아 있음을 확인했다. `src/graphics.c:1989-2044`가 HNS 옵션 시트와 공용 헤더/도시 확대 그래픽을 ROM에 포함한다. 원본 PNG를 고친 뒤 파생 `.4bpp`/`.smol`을 재생성한다. |
| P0 | 배틀에서 즉시 보이는 안내 | `src/battle_controller_player.c:1767-1769,2037`, `src/battle_z_move.c:97-105` | 기술 정보의 `CAT/PWR/ACC`, 동료 행동 안내, Z기술 추가 효과 문구가 영문이다. 전투 창 폭과 기존 제어 코드를 유지한다. |
| P1 | 배틀 메시지 잔여 | `src/battle_message.c` 및 `docs/localization/BATTLE_MESSAGE_KR_COMPARE.md` | 실제 현행 호출 경로가 있는 대표 후보는 멸망의바디 3턴 문구(`:720`), 테라스탈 후속 문구(`:862`), FRLG 포켓몬피리(`:870-872`), Sleep Clause(`:881`), FRLG 유령/사파리(`:895-903`)다. 눈·안개·강철서지 제거와 Symbiosis는 현재 한글임을 재확인했다. Dynamax는 현재 기본 설정에서 비활성이고, FRLG/Safari는 별도 모드이므로 P0 기본 UI 뒤에 처리한다. |
| P1 | 아이템·기술·특성 설명 | `src/data/items.h`, `src/data/moves_info.h`, `src/data/abilities.h`, `src/data/types_info.h` | 아이템 설명 필드 920개, 기술 설명 필드 947개가 있으며 공유 설명 정의도 각각 35/25개다. 특성 설명 321개와 타입 일반명 21개도 영문이다. 이름은 이미 한글인 경우가 많으므로 이름을 다시 바꾸지 않고 설명만 독립 작업으로 다룬다. |
| P1 | 열매·굿즈·콘테스트 표기 | `src/berry.c`, `src/berry_tag_screen.c`, `src/pokeblock.c`, `src/data/decoration/{header,description}.h`, `src/contest.c`, `src/contest_painting.c`, `src/data/{contest_moves,contest_text_tables}.h` | 열매 설명 204개(두 줄씩), 굿즈명 121개, 열매 태그/포켓블록/콘테스트 설명·랭크가 남아 있다. 굿즈명과 짧은 랭크명은 저장된 데이터가 아니라 표시 문자열이지만, 메뉴 폭 검증이 필요하다. |
| P1 | 지역 지도·시설명 | `src/landmark.c:20-60`, `src/map_name_popup.c:508-515`, `src/data/script_menu.h` | 호연 랜드마크 42개, 배틀피라미드 층명, FRLG/세비·게임코너·교환 메뉴가 영문이다. 지역명 팝업의 `region_map_sections.json` 한글화와 별개의 경로다. |
| P1 | 교환·유니언룸·미스터리 이벤트 | `src/data/{trade,union_room,help_window}.h`, `src/mystery_event_msg.c` | 교환 확인/메뉴, 통신 방 대기·선택·시설명, 도움말 헤더, 희귀 단어/전송 완료 메시지가 영문이다. 교환 데이터의 OT명·닉네임은 표시 UI가 아니라 원본 데이터 호환성도 있으므로 별도 결정 없이 일괄 번역하지 않는다. |
| P2 | 이지챗 | `src/data/easy_chat/*.h` | 화면에서 고르는 그룹명과 단어 1,030개가 영문이다. 포켓몬/기술 그룹은 다른 한글 이름 테이블을 참조해 이 검색에는 잡히지 않는다. 단어의 조합 가능성·글자 폭·기존 저장 단어 ID를 보존하는 전용 작업으로 나눈다. |
| P2 | 배틀프런티어·미니게임 | `src/battle_dome.c`, `src/berry_blender.c`, `src/roulette.c`, `src/slot_machine.c`, `src/voltorb_flip.c`, `src/sliding_puzzle.c` | 배틀돔의 팀 평가·대진·결과(114개 검색 행), 블렌더 결과, 룰렛/슬롯, 볼트체인지/퍼즐 버튼 안내가 영문이다. `frontier_util.c`의 짧은 트레이너 발화는 NPC성 대사이므로 제외한다. |

## 직접 그래픽 스프라이트 수정 분류

이 절은 C/어셈블리 문자열이 아니라 PNG의 픽셀 자체가 글자인 항목만 따로 분류한다. `직접 수정`은 원본 PNG와 대응 타일을 편집해야 한다는 뜻이며, 생성물 `.4bpp`·`.smol`을 직접 고치는 뜻은 아니다. 크기·팔레트·타일 순서와 기존 tilemap을 보존한 뒤 GBA 그래픽과 압축 산출물을 재생성해야 한다.

| 분류 | 우선 | 현재 상태와 원본 | 화면상 잔여/판정 | 근거와 작업 경계 |
| --- | --- | --- | --- | --- |
| 직접 수정 | P0 | `graphics/pokedex/hgss/tileset_{interface_hns,menu_list,menu1,menu2,menu3,menu_search}.png` | `SELECT`, `SEARCH`, `START`, `SEEN`, `OWN`, `JOHTO`, `NATIONAL`, `MODE`, `ORDER`, `NAME`, `NUMBER`, `TYPE`, `CANCEL` 등 도감 고정 라벨이 영문 픽셀이다. | `POKEDEX_PLUS_HGSS=TRUE`이며 `src/pokedex_plus_hgss.c:244-257`이 이 타일셋을 로드한다. HNS에는 `interface_hns`가, 나머지 5개는 공용으로 실제 사용된다. 런타임 도감 문구 번역과 같은 P0 묶음으로 처리하되, 타일 이동 없이 글자만 교체한다. |
| 직접 수정 | P0 | `graphics/pokedex/area_unknown.png` | `AREA UNKNOWN`이 영문 픽셀이다. | HGSS 도감의 서식 화면도 `DisplayPokedexAreaScreen()`을 호출한다(`src/pokedex_plus_hgss.c:4144`). 해당 종의 서식이 없을 때 스프라이트를 만든다(`src/pokedex_area_screen.c:1012-1045`). |
| 직접 수정 | P0 | `graphics/pokenav/hns/options/{hoenn_map,match_call,radio}.png`, `graphics/pokenav/{options,left_headers}/*.png`, `graphics/pokenav/hns/left_headers/{hoenn_map,match_call}.png`, `graphics/pokenav/{condition/cancel,region_map/city_zoom_text}.png` | `MAP`/`CALL`/`RAD`, `CONDITION`, `SWITCH OFF`, `MAIN MENU`, `RIBBONS`, `SEARCH`, `PARTY`, `CANCEL`, 도시 확대의 `POKéMON CENTER`/`MART` 등이 고정 영문 픽셀이다. | HNS 옵션 시트는 `src/graphics.c:1989-1991`, 좌측 헤더는 `:2013-2032`, 도시 확대 글자는 `:2044`에서 ROM에 들어간다. 옵션 PNG 하나는 32×64 원본을 네 개의 32×16 스프라이트 조각으로 세로 저장한 것이며, 런타임이 가로 128×16 라벨로 재배치한다. `options.bin`은 이 라벨 배치용 파일이 아니다. |
| 직접 수정 | P0 | `graphics/naming_screen/rwindow.png`, `graphics/naming_screen/roptions.png` | 작명 화면의 `BACK`, `OK`, `UPPER`, `lower`, `others`가 픽셀 글자다. | `src/graphics.c:2133-2136`과 `src/naming_screen.c:3077-3083`이 `rwindow`/`roptions`의 오프셋을 직접 스프라이트로 사용한다. 개별 `back_button.png`·`ok_button.png`가 아니라 이 두 실제 로드 원본을 수정 대상으로 삼는다. |
| 직접 수정, 표기 정책 확인 | P1 | `graphics/title_screen/hns/press_start.png` | 현재 타이틀의 `PRESS START`가 영문 픽셀이다. | HNS 타이틀에 실제 포함되는 원본이다. 포켓몬/하트골드 로고는 브랜드 표기로 유지하고, 시작 안내만 한글화할지 결정한 뒤 작업한다. |
| 직접 수정 | P1 | `graphics/union_room_chat/r_button_labels.png` | 유니언룸 채팅의 등록/버튼 라벨이 영문 픽셀이다. | `FREE_UNION_ROOM_CHAT=FALSE`이고 `src/union_room_chat.c:758-765`가 실제 스프라이트 시트로 로드한다. 통신 기능의 런타임 문자열 작업과 같은 묶음으로 처리한다. |
| 이미 한글화됨 — 수정 불필요 | — | `graphics/battle_interface/{move_info_window_l,move_info_window_r}.png`, `graphics/battle_interface/gen4/{move_info_window_l,move_info_window_r}.png` | `기술 / 정보` 표기가 이미 한글이다. | 현재 `B_MOVE_DESCRIPTION_BUTTON=L_BUTTON`이며 L=A 설정일 때 R 변형도 사용한다(`src/battle_interface.c:3184-3216`). Gen4 UI 선택 시 Gen4 L/R도 사용하므로 네 파일을 모두 확인했다. |
| 이미 한글화됨 — 수정 불필요 | — | `graphics/interface/menu_info.png`, `graphics/pokemon_storage/menu.png`, `graphics/battle_frontier/tourney_buttons.png` | 메뉴 정보, PC 저장 시스템, 배틀돔 버튼의 글자는 한글이다. | 각각 `src/graphics.c:1961-1964,2117,1410`의 실제 포함 경로까지 대조했다. `tourney_info_card.png`, 가방/상점 배경은 확인한 범위에 번역 대상 영문 글자가 없다. |
| 의도적으로 유지 | — | `graphics/easy_chat/start_select_buttons.png` | `START`/`SELECT` 버튼명만 있는 그래픽이다. | 이지챗에서 실제 로드되지만(`src/easy_chat.c:709-712,889-895`), 본 문서의 버튼명·약어 유지 원칙에 따라 현재는 번역하지 않는다. |
| 조건부/미사용 — 지금 수정하지 않음 | — | `graphics/battle_interface/move_info_window_start.png`, `graphics/pokedex/hgss/*_DECA*.png`, `graphics/dexnav/{captured_all,no_data}.png` | START 변형에는 `START/MOVE INFO`가 남아 있으나 현재 L 설정에서는 로드되지 않는다. DECA 타일셋은 `HGSS_DECAPPED=FALSE`다. DexNav 두 PNG는 이름과 달리 문자 없이 아이콘/표식만 있으며 `DEXNAV_ENABLED=FALSE`다. | 설정을 바꾸거나 DexNav를 활성화할 때 다시 확인한다. `move_info_window_start`을 한글화하더라도 `B_MOVE_DESCRIPTION_BUTTON`을 START로 바꾸지 않는 한 현재 ROM에는 영향이 없다. |

2026-09-28 후속 상태: 공용 `graphics/pokenav/options/*.png` 13개가 한글 수정본으로 다시 추가됐고, HNS 라디오 옵션·라디오 UI·도시 확대 PNG도 수정·재변환됐다. 큰 헤더는 타일 띠를 단순 이동하지 않고, 사용자가 다시 편집한 Aseprite의 동일 폭 위 2px·가운데 8px·아래 2px를 합친 51×12 완성 그림으로 만든 뒤 7×3 타일로 다시 패킹했다. 선택의 원본 좌표는 위 `(16,6)`, 가운데 `(64,0)`, 아래 `(128,0)`이며 앞쪽 공백도 보존한다. 최종 타일맵에서 추출한 51×12 픽셀은 이 세 선택 영역의 결합 결과와 612/612 픽셀 일치한다. 기본/HNS 옵션 합성물과 헤더 변환 및 새 ROM 링크를 마쳤으므로 이 표의 포케기어 **픽셀 라벨 묶음은 코드·그래픽 기준 완료**다. 실제 mGBA 화면 검증과 별도 런타임 영문 문자열 번역은 남아 있다.

### 스프라이트 작업의 검증 단위

1. 원본 PNG에서 4bpp 팔레트·가로세로·타일 순서를 보존한다. 특히 도감과 포케기어는 타일 ID를 tilemap이 참조하므로 한글 글자를 넣기 위해 타일을 삽입·삭제하지 않는다.
2. `make hns -j8`로 `.4bpp`와 `.smol` 생성물 및 HNS ROM을 다시 만들고, `git diff --check`를 실행한다. HNS 포케기어에는 단일 `options.png`가 없지만, `graphics_file_rules.mk`의 전용 규칙이 개별 14개 라벨의 `.4bpp`를 현재 고정 순서(지도, 컨디션, 통화, 리본, 전원, 파티, 검색, 쿨, 뷰티, 큐트, 스마트, 터프, 취소, 라디오)로 자동 연결한다. 단, 원본 PNG가 삭제된 채 예전 `.4bpp`만 남아 있으면 증분 빌드는 낡은 결과를 재사용할 수 있고 깨끗한 빌드는 재현되지 않으므로, 각 원본과 생성물의 존재·시각을 함께 확인한다.
3. 실제 ROM에서 도감 목록/검색/서식, 포케기어 각 메뉴와 도시 확대, 작명 화면, 유니언룸을 열어 팔레트 깨짐·타일 어긋남·잘림을 확인한다. 빌드 성공은 이 시각 검증을 대체하지 않는다.

## 서사성·보조 범위

아래는 NPC 대화는 아니지만 한 번에 번역하면 검증량이 큰 콘텐츠다. UI와 시스템 메시지를 마친 뒤, 한 파일군씩 사용자 승인 범위를 정해 진행한다.

| 파일군 | 영문 `.string` 행 | 내용 |
| --- | ---: | --- |
| `data/text/tv.inc` | 2,188 | TV 프로그램·보도·인터뷰 |
| `data/text/fame_checker_frlg.inc` | 819 | Fame Checker 설명/기록 |
| `data/text/berries.inc` | 153 | 열매 관련 이벤트/안내 |
| `data/text/pokemon_news.inc` | 141 | 뉴스 |
| `data/text/{event_ticket_1,event_ticket_2,questionnaire,lottery_corner,shoal_cave}.inc` | 195 | 이벤트 티켓·설문·복권·조수동굴 안내 |

`data/text/trainers*.inc`, `data/text/match_call*.inc`, `data/maps/**/scripts.inc`, 트레이너/배틀텐트/견습생의 대사 데이터는 이번 요청의 NPC 대화 제외 원칙에 따라 인벤토리에 넣지 않았다. 라디오 방송 대본(`src/data/text/radio_strings.h`)도 방송 화자 대사로 같은 기준에서 보류한다.

## 의도적으로 유지하거나 별도 판단할 항목

- `START`, `SELECT`, `AM/PM`, `HP/PP/BP`, `Lv.`, `No.`와 영문 알파벳 키보드는 UI 규격·입력 호환 표기다. 필요하면 한국판 표기와 화면 폭을 대조해 결정하되 일괄 치환하지 않는다.
- `src/strings.c`의 `Sold Out`, `KNOCKOUT`, `MIXED`, `OK!`, `EXCELLENT`, `Are these three POKéMON OK?`는 실제 사용 여부를 호출부별로 확인한 뒤 P0/P1에 편입한다. `NO WEATHER` 등 `// Unused` 디버그 표기는 기본 번역 대상에서 제외한다.
- 배틀 기술 정보 스프라이트 L/R 및 Gen4 L/R은 모두 이미 `기술 / 정보`로 확인됐다. START 변형은 현재 설정에서 사용하지 않는 조건부 자산이므로 위 분류를 따른다.
- 원본 텍스트의 `\n`, `\p`, `{STR_VAR_n}`, `{DYNAMIC n}`, `{B_...}` 및 조사 placeholder는 번역 중 그대로 유지한다. 포켓몬 이름·기술명·도감 분류는 이미 정한 공식 한글 표기를 재변경하지 않는다.

## 다음 작업 권장 순서

1. `src/player_pc.c`·`src/item_menu.c`·`src/item_use.c`와 9개 시스템 `.inc`를 한 화면 단위로 번역한다.
2. HGSS 도감 UI와 DexNav를 조건문/가변 문자열 표와 함께 번역한다.
3. 포케기어의 런타임 텍스트와 확인된 PNG 라벨을 묶어 고치고 HNS ROM에서 메뉴를 확인한다.
4. 이후 아이템·기술·특성 설명, 이지챗, 시설/미니게임, 서사성 데이터 순으로 분리한다.

각 코드·텍스트·PNG 작업 뒤에는 `git diff --check`와 `make hns -j8`을 실행하고, 메뉴 폭·줄바꿈·제어 토큰은 실제 ROM 화면에서 검증한다.
