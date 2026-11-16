gText_PkmnTransferredSomeonesPC::
	.string "{STR_VAR_2}{K_EUNNEUN} 누군가의 PC\n"
	.string "박스 “{STR_VAR_1}”에\l"
	.string "전송되었다!$"

gText_PkmnTransferredLanettesPC::
	.string "{STR_VAR_2}{K_EUNNEUN}\n"
#if IS_FRLG || IS_HNS
	.string "이수재의 PC\n"
#else
	.string "유미의 PC\n"
#endif
	.string "박스 “{STR_VAR_1}”에\l"
	.string "전송되었다!$"

gText_PkmnTransferredSomeonesPCBoxFull::
	.string "누군가의 PC\n"
	.string "박스 “{STR_VAR_3}”{K_EUNNEUN}\l"
	.string "가득 찼다!\p"
	.string "{STR_VAR_2}{K_EUNNEUN}\n"
	.string "박스 “{STR_VAR_1}”에\l"
	.string "전송되었다!$"

gText_PkmnTransferredLanettesPCBoxFull::
#if IS_FRLG || IS_HNS
	.string "이수재의 PC\n"
#else
	.string "유미의 PC\n"
#endif
	.string "박스 “{STR_VAR_3}”{K_EUNNEUN}\l"
	.string "가득 찼다!\p"
	.string "{STR_VAR_2}{K_EUNNEUN}\n"
	.string "박스 “{STR_VAR_1}”에\l"
	.string "전송되었다!$"

gText_PkmnSentToPCAfterCatch::
	.string "{STR_VAR_2}{K_EUNNEUN}\n"
	.string "{B_PC_CREATOR_NAME} PC로 전송되었다!\p"
	.string "박스 “{STR_VAR_1}”에 들어갔다!$"

gText_PlayerObtainedTheMon::
	.string "{PLAYER}{K_EUNNEUN}\n"
	.string "{STR_VAR_1}{K_EULREUL} 손에 넣었다!$"

gText_NoMoreRoomForPokemon::
	.string "포켓몬을 잡을 수 없습니다!\p"
	.string "박스가 가득 차서\n"
	.string "전송할 수 없습니다!$"

gText_NicknameThisPokemon::
	.string "잡은 {STR_VAR_1}에게\n"
	.string "닉네임을 붙이겠습니까?$"
