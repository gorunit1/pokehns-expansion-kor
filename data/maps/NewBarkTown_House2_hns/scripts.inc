NewBarkTown_House2_hns_MapScripts::
	.byte 0


NewBarkTown_House2_EventScript_ElmsWife::
	lock
	faceplayer
	msgbox NewBarkTown_House2_Text_ElmsWife, MSGBOX_DEFAULT
	closemessage
	release
	end

NewBarkTown_House2_EventScript_ElmsSon::
	lock
	faceplayer
	msgbox NewBarkTown_House2_Text_ElmsSon, MSGBOX_DEFAULT
	closemessage
	release
	end

NewBarkTown_House2_Text_ElmsWife:
	.string "어머 {PLAYER}{K_AYA}!\p"
	.string "우리 남편\n"
	.string "뭔가 바쁜 것 같은데\l"
	.string "무리하고 있지 않니?\p"
	.string "포켓몬 연구를 하게 되면\n"
	.string "열중해서 밥도 안 먹거든.$"

NewBarkTown_House2_Text_ElmsSon:
	.string "난 어른이 되면\n"
	.string "아버지를 도울 거야.\p"
	.string "그래서 굉장한\n"
	.string "포켓몬 박사가 될 거야!$"
