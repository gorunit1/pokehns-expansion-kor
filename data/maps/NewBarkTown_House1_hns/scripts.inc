NewBarkTown_House1_hns_MapScripts::
	.byte 0

NewBarkTown_House1_EventScript_Mother::
	lock
	faceplayer
	msgbox NewBarkTown_House1_Text_Neighbor, MSGBOX_DEFAULT
	closemessage
	release
	end

NewBarkTown_House1_EventScript_Daughter::
	lock
	faceplayer
	msgbox NewBarkTown_House1_Text_NeighborsDaughter, MSGBOX_DEFAULT
	closemessage
	release
	end

NewBarkTown_House1_Text_NeighborsDaughter:
	.string "피카츄는 이미 진화한\p"
    .string "포켓몬이다!\p"
    .string "...라는 것을\n"
    .string "최초에 발견한 건\l"
    .string "공박사님이야.\p"
    .string "나 깜짝 놀랐어!\p"
    .string "역시 포켓몬 진화\n"
    .string "연구로 유명한 박사님.\p"
    .string "하아... 나도 그런\n"
    .string "대단한 연구자가 되고 싶어.$"


NewBarkTown_House1_Text_Neighbor:
    .string "우리 딸이 말이야,\n"
    .string "공박사님의 조수가 되고 싶다고\l"
    .string "고집을 부리는 거 있지!\p"
    .string "정말 포켓몬을 좋아하나 봐!\p"
	.string "뭐, 나도 좋아하긴 하지만 말이야...$"

