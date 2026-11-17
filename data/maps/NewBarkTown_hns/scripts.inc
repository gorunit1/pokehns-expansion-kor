NewBarkTown_hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, NewBarkTown_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, NewBarkTown_OnFrame
	map_script MAP_SCRIPT_ON_RESUME, SetTimeEncounters
	.byte 0

NewBarkTown_OnFrame::
	map_script_2 VAR_NEWBARK_TOWN_STATE, 6, NewBarkTown_EventScript_ElmCallAfterLeague
	.2byte 0

NewBarkTown_EventScript_ElmCallAfterLeague::
	lock
	setspeaker NewBarkTown_Text_ElmName
	pokenavcall BlackthornCity_Text_ElmCall
	waitmessage
	delay 30
	setvar VAR_NEWBARK_TOWN_STATE, 7
	setvar VAR_NEWBARKTOWN_LABSTATE, 10
	release
	end

NewBarkTown_Text_ElmName:
	.string "PROF.ELM$"

NewBarkTown_OnTransition::
	goto_if_eq VAR_NEWBARK_TOWN_STATE, 2, NewBarkTown_Set_TalkToElm
	goto_if_eq VAR_NEWBARK_TOWN_STATE, 4, NewBarkTown_Set_TalkToMom
	end

NewBarkTown_Set_TalkToElm::
	clearflag FLAG_HIDE_SILVER_NEWBARKTOWN
	setobjectxyperm LOCALID_LASS, 0, 11
	callnative SetTimeBasedEncounters
	end

NewBarkTown_Set_TalkToMom::
	setobjectxyperm LOCALID_LASS, 0, 11
	callnative SetTimeBasedEncounters
	end

NewBarkTown_EventScript_LabSign::
	msgbox NewBarkTown_Text_LabSign, MSGBOX_SIGN
	end

NewBarkTown_EventScript_TownSign::
	msgbox NewBarkTown_Text_Sign, MSGBOX_SIGN
	closemessage
	end


NewBarkTown_EventScript_Lass::
	lock
	faceplayer
	goto_if_eq VAR_NEWBARK_TOWN_STATE, 2, NewBarkTown_EventScript_LassState2
	goto_if_eq VAR_NEWBARK_TOWN_STATE, 3, NewBarkTown_EventScript_LassState3
	goto_if_eq VAR_NEWBARK_TOWN_STATE, 4, NewBarkTown_EventScript_LassState4
	msgbox NewBarkTown_Text_LassState5, MSGBOX_DEFAULT
	closemessage
	release
	end
	
NewBarkTown_EventScript_LassState2::
	playse SE_PIN
	applymovement LOCALID_LASS, Common_Movement_ExclamationMark
	delay 55
	waitmovement LOCALID_LASS
	faceplayer
	waitmovement 0
	msgbox NewBarkTown_Text_LassState2, MSGBOX_DEFAULT
	closemessage
	release
	end

NewBarkTown_EventScript_LassState3::
	msgbox NewBarkTown_Text_LassState3, MSGBOX_DEFAULT
	closemessage
	release
	end

NewBarkTown_EventScript_LassState4::
	playse SE_PIN
	applymovement LOCALID_LASS, Common_Movement_ExclamationMark
	delay 55
	waitmovement LOCALID_LASS
	faceplayer
	waitmovement 0
	msgbox NewBarkTown_Text_LassState4, MSGBOX_DEFAULT
	closemessage
	release
	end

NewBarkTown_EventScript_Wooper::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_WOOPER, CRY_MODE_NORMAL
	msgbox NewBarkTown_Text_Wooper, MSGBOX_DEFAULT
	waitmoncry
	release
	end

NewBarkTown_EventScript_LeaveTrigger::
	lock
	call_if_eq VAR_NEWBARK_TOWN_STATE, 2, NewBarkTown_EventScript_LeaveTriggersState2
	call_if_eq VAR_NEWBARK_TOWN_STATE, 4, NewBarkTown_EventScript_LeaveTriggersState4
	applymovement OBJ_EVENT_ID_PLAYER, OldaleTown_Movement_PlayerStepBack
	waitmovement 0
	release
	end

NewBarkTown_EventScript_LeaveTriggersState2::
	playse SE_PIN
	applymovement LOCALID_LASS, Common_Movement_ExclamationMark
	delay 55
	waitmovement LOCALID_LASS
	applymovement LOCALID_LASS, NewBarkTown_Movement_FaceDown
	waitmovement 0
	msgbox NewBarkTown_Text_LassState2, MSGBOX_DEFAULT
	closemessage
	return

NewBarkTown_EventScript_LeaveTriggersState4::
	playse SE_PIN
	applymovement LOCALID_LASS, Common_Movement_ExclamationMark
	delay 55
	waitmovement LOCALID_LASS
	applymovement LOCALID_LASS, NewBarkTown_Movement_FaceDown
	waitmovement 0
	msgbox NewBarkTown_Text_LassState4, MSGBOX_DEFAULT
	closemessage
	return 



NewBarkTown_EventScript_FatMan::
	lock
	faceplayer
	goto_if_eq VAR_NEWBARK_TOWN_STATE, 2, NewBarkTown_EventScript_FatManState2
	msgbox NewBarkTown_Text_FatManState3, MSGBOX_DEFAULT
	closemessage
	release
	end

NewBarkTown_EventScript_FatManState2::
	msgbox NewBarkTown_Text_FatManState2, MSGBOX_DEFAULT
	closemessage
	release
	end

NewBarkTown_EventScript_Silver::
	lock
	msgbox NewBarkTown_Text_Silver1, MSGBOX_DEFAULT
	delay 60
	playse SE_PIN
	applymovement LOCALID_SILVER, Common_Movement_ExclamationMark
	faceplayer
	waitmovement 0
	msgbox NewBarkTown_Text_Silver2, MSGBOX_DEFAULT
	playse SE_BANG
	applymovement LOCALID_SILVER, NewBarkTown_Movement_SilverPushesPlayer
	applymovement OBJ_EVENT_ID_PLAYER, NewBarkTown_Movement_PlayerPushedBySilver
	waitmovement 0
	applymovement LOCALID_SILVER, Common_Movement_FaceOriginalDirection
	waitmovement 0
	waitmovement 0
	release
	end


NewBarkTown_EventScript_Door::
	msgbox NewBarkTown_Text_Door, MSGBOX_SIGN
	end

NewBarkTown_EventScript_TestMan1::
	givebp 4
	msgbox BattleFrontier_Text_ObtainedXBattlePoints, MSGBOX_GETPOINTS
	end

NewBarkTown_EventScript_TestMan2::
	setvar VAR_NEWBARK_TOWN_STATE, 4
	setflag FLAG_HIDE_MOMS_FRIEND
	setflag FLAG_ADVENTURE_STARTED
	setflag FLAG_START_NUZLOCKE
	setflag FLAG_HIDE_SILVER_NEWBARKTOWN
	end

NewBarkTown_EventScript_TestMan3::
	setvar VAR_NEWBARK_TOWN_STATE, 5
	
	clearflag FLAG_COMPLETED_AERODACTYL_PUZZLE
	clearflag FLAG_COMPLETED_HOOH_PUZZLE
	clearflag FLAG_COMPLETED_KABUTO_PUZZLE
	clearflag FLAG_COMPLETED_OMANYTE_PUZZLE
	end

NewBarkTown_Movement_SilverPushesPlayer:
	walk_fast_left
	walk_fast_left
	walk_fast_down
	face_down
	lock_facing_direction
	unlock_facing_direction
	walk_fast_up
	walk_fast_right
	walk_fast_right
	step_end


NewBarkTown_Movement_PlayerPushedBySilver:
	walk_fast_left
	face_up
	walk_fast_down
	face_up
	lock_facing_direction
	jump_2_down
	unlock_facing_direction
	face_down
	step_end

	


NewBarkTown_Movement_FaceDown:
	face_down
	step_end

NewBarkTown_Text_LabSign:
	.string "공박사 포켓몬 연구소$"

NewBarkTown_Text_Door:
	.string "문이 잠겨 있다.$"

NewBarkTown_Text_Wooper:
	.string "우파: 웅팟!$"

NewBarkTown_Text_FatManState2:
	.string "네가 가지고 있는 포켓기어\n"
	.string "무척 멋있구나!\p"
	.string "혹시... 최신형?$"

NewBarkTown_Text_FatManState3:
	.string "여! {PLAYER}\p"
	.string "공박사님이 새로운\n"
	.string "포켓몬을 발견했대.$"

NewBarkTown_Text_LassState2:
	.string "이런! 포켓몬도 없이\n"
	.string "도로에 나가다니 위험해!\p"
	.string "근처 마을까지는\n"
	.string "야생의 포켓몬이 튀어나오는\l"
	.string "풀숲뿐이니까!$"

NewBarkTown_Text_LassState3:
	.string "어머! 귀여운 포켓몬!\n"
	.string "나도 갖고 싶어.$"

NewBarkTown_Text_LassState4:
	.string "어머 {PLAYER}\n"
	.string "또 외출하니?\p"
	.string "외출할 때는 엄마한테\n"
	.string "꼭 말해야지!$"

NewBarkTown_Text_LassState5:
	.string "포켓기어가 있으면 언제라도\n"
	.string "엄마한테 건강한 목소리를\l"
	.string "들려줄 수 있어.$"

NewBarkTown_Text_Silver1:
	.string "...... ...... ......\p"
	.string "여기가 유명한\n"
	.string "공박사 포켓몬 연구소...$"

NewBarkTown_Text_Silver2:
	.string "...뭐야\n"
	.string "사람을 빤히 쳐다보지 말라고.$"

NewBarkTown_Text_Sign:
	.string "여기는 연두마을\n"
	.string "시작을 알리는 바람의 마을$"

NewBarkTown_EventScript_PlayersHouseMailbox::
	msgbox NewBarkTown_Text_PlayersHouse, MSGBOX_SIGN
	end

NewBarkTown_EventScript_ElmsHouseMailbox::
	msgbox NewBarkTown_Text_ElmsHouse, MSGBOX_SIGN
	end

NewBarkTown_Text_PlayersHouse:
	.string "여기는 {PLAYER}의 집$"

NewBarkTown_Text_ElmsHouse:
	.string "여기는 공박사의 집$"

