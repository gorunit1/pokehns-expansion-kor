.set PLAYER_STARTER_SPECIES,  VAR_TEMP_2

NewBarkTown_Lab_hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, NewBarkTown_Lab_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, NewBarkTown_Lab_OnFrame
	.byte 0

NewBarkTown_Lab_OnTransition::
	goto_if_eq VAR_NEWBARKTOWN_LABSTATE, 6, NewBarkTown_Lab_EventScript_ElmAtComputer
	goto_if_eq VAR_NEWBARKTOWN_LABSTATE, 2, NewBarkTown_Lab_EventScript_ElmAtComputer
	goto_if_eq VAR_NEWBARKTOWN_LABSTATE, 3, NewBarkTown_Lab_EventScript_ElmAtComputer
	end
	
NewBarkTown_Lab_OnFrame::
	map_script_2 VAR_NEWBARKTOWN_LABSTATE, 4, NewBarkTown_Lab_EventScript_PoliceEpisode
	.2byte 0

NewBarkTown_Lab_EventScript_ElmAtComputer::
	setobjectxyperm LOCALID_ELM, 3, 2
	setobjectmovementtype LOCALID_ELM, MOVEMENT_TYPE_FACE_UP
	end

NewBarkTown_Lab_EventScript_TriggerStartLabLeft::	
	lock
	applymovement OBJ_EVENT_ID_PLAYER, NewBarkTown_Lab_Movement_WalkRight
	waitmovement 0
	goto NewBarkTown_Lab_EventScript_TriggerStartLab
	end

NewBarkTown_Lab_EventScript_TriggerStartLabRight::
	lock
	applymovement OBJ_EVENT_ID_PLAYER, NewBarkTown_Lab_Movement_WalkLeft
	waitmovement 0
	goto NewBarkTown_Lab_EventScript_TriggerStartLab
	end

NewBarkTown_Lab_EventScript_TriggerStartLab::
	lock
	applymovement OBJ_EVENT_ID_PLAYER, NewBarkTown_LabMovement_WalkToElm
	waitmovement 0
	goto NewBarkTown_Lab_EventScript_ElmIntro
	end

NewBarkTown_EventScript_Aide1::
	lock
	faceplayer
	msgbox NewBarkTown_Lab_Text_AideAlwaysBusy, MSGBOX_DEFAULT
	closemessage
	release
	end

NewBarkTown_EventScript_Aide2::
	lock
	faceplayer
	goto_if_eq VAR_NEWBARKTOWN_LABSTATE, 5, NewBarkTown_EventScript_Aide2Testimony
	msgbox NewBarkTown_Lab_Text_AideSave, MSGBOX_DEFAULT
	closemessage
	release
	end

NewBarkTown_EventScript_Aide2Testimony::
	msgbox NewBarkTown_Lab_Text_AideTheftTestimony, MSGBOX_DEFAULT
	closemessage
	release
	end

NewBarkTown_Lab_EventScript_Elm::
	lock
	faceplayer
	goto_if_set FLAG_HIDE_NEWBARKTOWN_LAB_AIDE, NewBarkTown_Lab_EventScript_ElmAideIsWaiting
	goto_if_eq VAR_NEWBARKTOWN_LABSTATE, 7, NewBarkTown_Lab_EventScript_ElmMasterBall
	goto_if_eq VAR_NEWBARKTOWN_LABSTATE, 8, NewBarkTown_Lab_EventScript_ElmKimono
	goto_if_eq VAR_NEWBARKTOWN_LABSTATE, 9, NewBarkTown_Lab_EventScript_ElmLeague
	goto_if_eq VAR_NEWBARKTOWN_LABSTATE, 10, NewBarkTown_Lab_EventScript_ElmSSTicket

	goto_if_unset FLAG_RECEIVED_TOGEPI_EGG, NewBarkTown_Lab_EventScript_Elm_AfterEggCheck
	goto_if_set FLAG_SHOWN_ELM_TOGEPI, NewBarkTown_Lab_EventScript_Elm_CallMe
	call_if_unset FLAG_SHOWN_ELM_TOGEPI, NewBarkTown_Lab_EventScript_Elm_Check_Togepi
	@ HGSS: "has that EGG changed any?" only while the Egg from the aide is still unhatched
	callnative HnsCheckPlayerHasTogepiEgg
	goto_if_eq VAR_RESULT, TRUE, NewBarkTown_Lab_EventScript_Elm_Check_WhereIsEgg
	@ HGSS: hatched, but none of the Togepi line is in the party
	goto NewBarkTown_Lab_EventScript_Elm_ThoughtEggHatched
NewBarkTown_Lab_EventScript_Elm_AfterEggCheck:
	goto_if_eq VAR_NEWBARKTOWN_LABSTATE, 5, NewBarkTown_Lab_EventScript_ElmState5

	goto_if_ge VAR_NEWBARKTOWN_LABSTATE, 6, NewBarkTown_Lab_EventScript_ElmState5
	goto_if_eq VAR_NEWBARKTOWN_LABSTATE, 2, CopNewBarkTown_Lab_EventScript_ElmState2
	goto_if_eq VAR_NEWBARKTOWN_LABSTATE, 3, CopNewBarkTown_Lab_EventScript_ElmState2
	goto_if_set FLAG_SYS_POKEMON_GET, NewBarkTown_Lab_EventScript_MrPokemon
	goto_if_eq VAR_NEWBARKTOWN_LABSTATE, 1, NewBarkTown_Lab_EventScript_ElmChoosePokemon
	end

NewBarkTown_Lab_EventScript_ElmAideIsWaiting::
	msgbox NewBarkTown_Lab_Text_ElmAideIsWaiting, MSGBOX_DEFAULT
	closemessage
	release
	end

NewBarkTown_Lab_Text_ElmAideIsWaiting:
	.string "앗 {PLAYER}\n"
	.string "내 조수 못 만났느냐?\p"
	.string "너에게 알을 건네주기 위해\n"
	.string "도라지시티의 포켓몬센터에\l"
	.string "갔을 텐데...$"

NewBarkTown_Lab_EventScript_ElmIntro::
	msgbox NewBarkTown_Lab_Text_ElmIntro, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, NewBarkTown_Lab_EventScript_ElmAccepted
	goto_if_eq VAR_RESULT, NO, NewBarkTown_Lab_EventScript_ElmRefused
	end

NewBarkTown_Lab_EventScript_ElmRefused::
	msgbox NewBarkTown_Lab_Text_ElmRefused, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, NewBarkTown_Lab_EventScript_ElmAccepted
	goto_if_eq VAR_RESULT, NO, NewBarkTown_Lab_EventScript_ElmRefused
	end

NewBarkTown_Lab_EventScript_ElmAccepted::
	msgbox NewBarkTown_Lab_Text_ElmAccepted, MSGBOX_DEFAULT
	msgbox NewBarkTown_Lab_Text_ElmResearchAmbitions, MSGBOX_DEFAULT
	delay 30
	playse SE_PC_LOGIN
	waitse
	playse SE_PIN
	applymovement LOCALID_ELM, Common_Movement_QuestionMark
	delay 55
	waitmovement LOCALID_ELM
	applymovement LOCALID_ELM, NewBarkTown_Lab_Movement_CheckEmail
	applymovement OBJ_EVENT_ID_PLAYER, NewBarkTown_Lab_Movement_Look_Left
	waitmovement 0
	waitmovement 0
	msgbox NewBarkTown_Lab_Text_ElmGotAnEmail, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_ELM, NewBarkTown_Lab_Movement_ReturnFromEmail
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, NewBarkTown_Lab_Movement_Look_Up
	waitmovement 0
	delay 30
	msgbox NewBarkTown_Lab_Text_ElmMissionFromMrPokemon, MSGBOX_DEFAULT
	goto NewBarkTown_Lab_EventScript_ElmChoosePokemon
	end

NewBarkTown_Lab_EventScript_ElmChoosePokemon::
	msgbox NewBarkTown_Lab_Text_ElmChooseAPokemon, MSGBOX_DEFAULT
	closemessage
	setvar VAR_NEWBARKTOWN_LABSTATE, 1
	release
	end

NewBarkTown_Lab_EventScript_TriggerReturnPlayer::
	lock
	msgbox NewBarkTown_Lab_Text_ElmWhereGoing, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, NewBarkTown_Lab_Movement_ReturnPlayer
	waitmovement 0
	release
	end

NewBarkTown_Lab_EventScript_ShouldntTouch::
	msgbox NewBarkTown_Lab_Text_ShouldntTouch, MSGBOX_DEFAULT
	closemessage
	release
	end


NewBarkTown_Lab_EventScript_ChikoritaBall::
	lock
	goto_if_set FLAG_SYS_POKEMON_GET, NewBarkTown_Lab_EventScript_ShouldntTouch
	goto_if_eq VAR_NEWBARKTOWN_LABSTATE, 1, NewBarkTown_Lab_EventScript_ChikoritaBall_Choice
	msgbox NewBarkTown_Lab_Text_ShouldntTouch, MSGBOX_DEFAULT
	release
	closemessage
	end

NewBarkTown_Lab_EventScript_ChikoritaBall_Choice::
	@callnative DisableStaticRandomizer
	setvar VAR_STARTER_MON, 0
	setvar PLAYER_STARTER_SPECIES, SPECIES_CHIKORITA
	showmonpic PLAYER_STARTER_SPECIES, 10, 3
	playmoncry PLAYER_STARTER_SPECIES, CRY_MODE_NORMAL
	bufferspeciesname STR_VAR_1, PLAYER_STARTER_SPECIES
	buffermoncategory STR_VAR_2, PLAYER_STARTER_SPECIES
	msgbox NewBarkTown_Lab_Text_ChooseChikorita, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, NewBarkTown_Lab_EventScript_ChoseStarter
	goto_if_eq VAR_RESULT, NO, NewBarkTown_Lab_EventScript_DeclinedStarter
	end

NewBarkTown_Lab_EventScript_CyndaquilBall::
	lock
	goto_if_set FLAG_SYS_POKEMON_GET, NewBarkTown_Lab_EventScript_ShouldntTouch
	goto_if_eq VAR_NEWBARKTOWN_LABSTATE, 1, NewBarkTown_Lab_EventScript_CyndaquilBall_Choice
	msgbox NewBarkTown_Lab_Text_ShouldntTouch, MSGBOX_DEFAULT
	closemessage
	release
	end

NewBarkTown_Lab_EventScript_CyndaquilBall_Choice::
	@callnative DisableStaticRandomizer
	setvar VAR_STARTER_MON, 1
	setvar PLAYER_STARTER_SPECIES, SPECIES_CYNDAQUIL
	showmonpic PLAYER_STARTER_SPECIES, 10, 3
	playmoncry PLAYER_STARTER_SPECIES, CRY_MODE_NORMAL
	bufferspeciesname STR_VAR_1, PLAYER_STARTER_SPECIES
	buffermoncategory STR_VAR_2, PLAYER_STARTER_SPECIES
	msgbox NewBarkTown_Lab_Text_ChooseCyndaquil, MSGBOX_YESNO	
	goto_if_eq VAR_RESULT, YES, NewBarkTown_Lab_EventScript_ChoseStarter
	goto_if_eq VAR_RESULT, NO, NewBarkTown_Lab_EventScript_DeclinedStarter
	end

NewBarkTown_Lab_EventScript_TotodileBall::
	lock
	goto_if_set FLAG_SYS_POKEMON_GET, NewBarkTown_Lab_EventScript_ShouldntTouch
	goto_if_eq VAR_NEWBARKTOWN_LABSTATE, 1, NewBarkTown_Lab_EventScript_TotodileBall_Choice
	msgbox NewBarkTown_Lab_Text_ShouldntTouch, MSGBOX_DEFAULT
	release
	closemessage
	end

NewBarkTown_Lab_EventScript_TotodileBall_Choice::
	@callnative DisableStaticRandomizer
	setvar VAR_STARTER_MON, 2
	setvar PLAYER_STARTER_SPECIES, SPECIES_TOTODILE
	showmonpic PLAYER_STARTER_SPECIES, 10, 3
	playmoncry PLAYER_STARTER_SPECIES, CRY_MODE_NORMAL
	bufferspeciesname STR_VAR_1, PLAYER_STARTER_SPECIES
	buffermoncategory STR_VAR_2, PLAYER_STARTER_SPECIES
	msgbox NewBarkTown_Lab_Text_ChooseTotodile, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, NewBarkTown_Lab_EventScript_ChoseStarter
	goto_if_eq VAR_RESULT, NO, NewBarkTown_Lab_EventScript_DeclinedStarter
	end

NewBarkTown_Lab_EventScript_DeclinedStarter::
	hidemonpic
	msgbox NewBarkTown_Lab_Text_DidntChooseStarter, MSGBOX_DEFAULT
	closemessage
	@callnative EnableStaticRandomizer
	release
	end

NewBarkTown_Lab_EventScript_ChoseStarter::
	@callnative EnableStaticRandomizer
	hidemonpic
	removeobject VAR_LAST_TALKED
	msgbox NewBarkTown_Lab_Text_ChoseStarter, MSGBOX_DEFAULT
	call_if_eq VAR_STARTER_MON, 0, NewBarkTown_Lab_EventScript_GiveStarter_Chikorita
	call_if_eq VAR_STARTER_MON, 1, NewBarkTown_Lab_EventScript_GiveStarter_Cyndaquil
	call_if_eq VAR_STARTER_MON, 2, NewBarkTown_Lab_EventScript_GiveStarter_Totodile
	setflag FLAG_SYS_POKEMON_GET
	playfanfare MUS_OBTAIN_ITEM
	bufferspeciesname STR_VAR_1, PLAYER_STARTER_SPECIES
	message NewBarkTown_Lab_Text_ReceivedStarter
	waitfanfare
	specialvar VAR_RESULT, IsNuzlockeNicknamingActive
	goto_if_eq VAR_RESULT, TRUE, NewBarkTown_Lab_EventScript_ChoseStarter_NicknameStarter_Nuzlocke
	msgbox NewBarkTown_Lab_Text_Nickname, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, NewBarkTown_Lab_EventScript_NicknameStarter
	callnative UpdateFollowingPokemon
	release
	end

NewBarkTown_Lab_EventScript_GiveStarter_Chikorita::
	call_if_unset FLAG_SHINY_STARTER_1, NewBarkTown_Lab_EventScript_GiveStarter_NotShiny
	call_if_set FLAG_SHINY_STARTER_1, NewBarkTown_Lab_EventScript_GiveStarter_Shiny
	return

NewBarkTown_Lab_EventScript_GiveStarter_Cyndaquil::
	call_if_unset FLAG_SHINY_STARTER_2, NewBarkTown_Lab_EventScript_GiveStarter_NotShiny
	call_if_set FLAG_SHINY_STARTER_2, NewBarkTown_Lab_EventScript_GiveStarter_Shiny
	return

NewBarkTown_Lab_EventScript_GiveStarter_Totodile::
	call_if_unset FLAG_SHINY_STARTER_3, NewBarkTown_Lab_EventScript_GiveStarter_NotShiny
	call_if_set FLAG_SHINY_STARTER_3, NewBarkTown_Lab_EventScript_GiveStarter_Shiny
	return

NewBarkTown_Lab_EventScript_GiveStarter_NotShiny::
	givemon PLAYER_STARTER_SPECIES, 5, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	return

NewBarkTown_Lab_EventScript_GiveStarter_Shiny::
	givemon PLAYER_STARTER_SPECIES, 5, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_ALWAYS	
	return

NewBarkTown_Lab_EventScript_NicknameStarter::
	setvar VAR_0x8004, 0
	call Common_EventScript_NameReceivedPartyMon
	release
	end

NewBarkTown_Lab_EventScript_ChoseStarter_NicknameStarter_Nuzlocke::
	message NewBarkTown_Lab_Text_Nickname
	waitmessage
	setvar VAR_0x8004, 0
	call Common_EventScript_NameReceivedPartyMon
	release
	end

NewBarkTown_Lab_EventScript_MrPokemon::
	setvar VAR_NEWBARKTOWN_LABSTATE, 2
	setvar VAR_NEWBARK_TOWN_STATE, 3
	msgbox NewBarkTown_Lab_Text_ElmDirections1_Register, MSGBOX_DEFAULT
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox NewBarkTown_Lab_Text_RegisteredElm, MSGBOX_DEFAULT
	waitfanfare
	setflag FLAG_ENABLE_PROF_BIRCH_MATCH_CALL
	msgbox NewBarkTown_Lab_Text_ElmDirections2, MSGBOX_DEFAULT
	closemessage
	goto_if_eq VAR_FACING, DIR_EAST, NewBarkTown_Lab_EventScript_MrPokemonMovementSafe
	applymovement LOCALID_ELM, NewBarkTown_Lab_Movement_CheckEmailSlow
	waitmovement 0
	clearflag FLAG_RECEIVED_FIRST_BALLS
	clearflag FLAG_RECEIVED_FIRST_POTION
	setvar VAR_NEWBARK_TOWN_STATE, 3
	setflag FLAG_ENABLE_CONDITION
	release
	end

NewBarkTown_Lab_EventScript_MrPokemonMovementSafe::
	applymovement LOCALID_ELM, NewBarkTown_Lab_Movement_GotoComputerSafe
	waitmovement 0
	clearflag FLAG_RECEIVED_FIRST_BALLS
	clearflag FLAG_RECEIVED_FIRST_POTION
	setvar VAR_NEWBARK_TOWN_STATE, 3
	release
	end

CopNewBarkTown_Lab_EventScript_ElmState2::
	msgbox NewBarkTown_Lab_Text_ElmDirections1, MSGBOX_DEFAULT
	msgbox NewBarkTown_Lab_Text_ElmDirections2, MSGBOX_DEFAULT
	applymovement LOCALID_ELM, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	closemessage
	release
	end

NewBarkTown_Lab_EventScript_HealingMachine1::
	lock
	goto_if_set FLAG_SYS_POKEMON_GET, NewBarkTown_Lab_EventScript_HealingMachine2
	msgbox LittlerootTown_ProfessorBirchsLab_Text_SeriousLookingMachine, MSGBOX_SIGN
	closemessage
	release
	end

NewBarkTown_Lab_EventScript_HealingMachine2::
	call Common_EventScript_OutOfCenterPartyHeal
	release
	end



NewBarkTown_Lab_EventScript_TriggerAideGivePotion::
	lock
	applymovement LOCALID_AIDE1, NewBarkTown_Lab_Movement_Aide1WalkToPlayer
	waitmovement 0
	turnobject OBJ_EVENT_ID_PLAYER, DIR_WEST
	msgbox NewBarkTown_Lab_Text_AideGivePotion, MSGBOX_DEFAULT
	closemessage
	giveitem ITEM_POTION
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_FIRST_POTION
	setvar VAR_NEWBARKTOWN_LABSTATE, 3
	applymovement LOCALID_AIDE1, NewBarkTown_Lab_Movement_Aide1LeavePlayer
	waitmovement 0
	turnobject OBJ_EVENT_ID_PLAYER, DIR_SOUTH
	release
	end

NewBarkTown_Lab_EventScript_HideCyndaquil::
	setflag FLAG_HIDE_CYNDAQUILBALL
	return
NewBarkTown_Lab_EventScript_HideTotodile::
	setflag FLAG_HIDE_TOTODILEBALL
	return
NewBarkTown_Lab_EventScript_HideChikorita::
	setflag FLAG_HIDE_CHIKORITABALL
	return

NewBarkTown_Lab_EventScript_PoliceEpisode::
	lock
	setmetatile 7, 1, METATILE_R26_21_Broken_Window, TRUE
	call_if_eq VAR_STARTER_MON, 0 NewBarkTown_Lab_EventScript_HideCyndaquil
	call_if_eq VAR_STARTER_MON, 1 NewBarkTown_Lab_EventScript_HideTotodile
	call_if_eq VAR_STARTER_MON, 2 NewBarkTown_Lab_EventScript_HideChikorita
	applymovement OBJ_EVENT_ID_PLAYER, NeBarkTownLab_Movement_PlayerWalkToCop
	waitmovement 0
	setflag FLAG_HIDE_LAB_POLICEMAN
	turnobject LOCALID_COP DIR_WEST
	msgbox NewBarkTown_Lab_Text_Officer1, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, NewBarkTown_Lab_EventScript_PoliceYes
	goto_if_eq VAR_RESULT, NO, NewBarkTown_Lab_EventScript_PoliceNo
	end

NewBarkTown_Lab_EventScript_PoliceNo::
	msgbox NewBarkTown_Lab_Text_OfficerNo, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, NewBarkTown_Lab_EventScript_PoliceYes
	goto_if_eq VAR_RESULT, NO, NewBarkTown_Lab_EventScript_PoliceNo
	end

NewBarkTown_Lab_EventScript_PoliceYes::
	closemessage
	special NameRival
	msgbox NewBarkTown_Lab_Text_Officer2_1, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, FALSE, NewBarkTown_Lab_EventScript_PoliceYes
	msgbox NewBarkTown_Lab_Text_Officer2, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_COP, NewBarkTown_Lab_Movement_CopLeaves
	waitmovement 0
	removeobject LOCALID_COP

	@ setobjectxyperm LOCALID_COP, 25, 4
	applymovement OBJ_EVENT_ID_PLAYER, NewBarkTown_Lab_Movement_PlayerStepsToElm
	waitmovement 0
	msgbox NewBarkTown_Lab_Text_ElmAfterTheft1, MSGBOX_DEFAULT
	playfanfare MUS_LEVEL_UP
	msgbox NewBarkTown_Lab_Text_ElmAfterTheft2, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	applymovement LOCALID_ELM, NewBarkTown_Lab_Movement_ElmStepBack
	waitmovement 0
	msgbox NewBarkTown_Lab_Text_ElmAfterTheft3, MSGBOX_DEFAULT
	delay 20
	closemessage
	applymovement LOCALID_ELM, NewBarkTown_Lab_Movement_ElmStepForward
	waitmovement 0
	msgbox NewBarkTown_Lab_Text_ElmAfterTheft4, MSGBOX_DEFAULT
	closemessage
	setvar VAR_NEWBARKTOWN_LABSTATE, 5
	setvar VAR_NEWBARK_TOWN_STATE, 4
	setflag FLAG_HIDE_MOMS_FRIEND
	clearflag FLAG_HIDE_MOMS_FRIEND2
	setflag FLAG_ADVENTURE_STARTED
	setflag FLAG_START_NUZLOCKE
	setflag FLAG_EXP_SHARE_ENABLED
	removeitem ITEM_MYSTERY_EGG
	release
	end

NewBarkTown_Lab_EventScript_ElmState5::
	lock
	faceplayer
	msgbox NewBarkTown_Lab_Text_ElmStudyingEgg, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_ELM Common_Movement_FaceOriginalDirection
	release
	end
	
NewBarkTown_Lab_EventScript_ElmMasterBall::
	lock
	faceplayer
	msgbox NewBarkTown_Lab_Text_ElmGiveMasterBall1, MSGBOX_DEFAULT
	giveitem ITEM_MASTER_BALL
	msgbox NewBarkTown_Lab_Text_ElmGiveMasterBall2, MSGBOX_DEFAULT
	closemessage
	delay 20
	playse SE_PIN
	applymovement LOCALID_ELM, Common_Movement_ExclamationMark
	delay 55
	waitmovement LOCALID_ELM
	msgbox NewBarkTown_Lab_Text_KimonoGirls, MSGBOX_DEFAULT
	closemessage
	setvar VAR_NEWBARKTOWN_LABSTATE, 8
	setvar VAR_ECRUTEAK_CITY_THEATER, 4
	setflag FLAG_HIDE_ECRUTEAK_SILVER
	setflag FLAG_HIDE_ECRUTEAK_CITY_THEATER_NPCS
	clearflag FLAG_HIDE_ECRUTEAK_CITY_THEATER_KIMONOS
	release
	end

NewBarkTown_Lab_EventScript_ElmKimono::
	lock
	faceplayer
	msgbox NewBarkTown_Lab_Text_KimonoWaiting, MSGBOX_DEFAULT
	closemessage
	release
	end

NewBarkTown_Lab_EventScript_ElmLeague::
	lock
	faceplayer
	msgbox NewBarkTown_Lab_Text_League, MSGBOX_DEFAULT
	closemessage
	release
	end

NewBarkTown_Lab_EventScript_ElmSSTicket::
	msgbox NewBarkTown_Lab_Text_ElmGiveTicket1, MSGBOX_DEFAULT
	giveitem ITEM_SS_TICKET
	msgbox NewBarkTown_Lab_Text_ElmGiveTicket2, MSGBOX_DEFAULT
	closemessage
	clearflag FLAG_HIDE_SSAQUA_1F_GRANDPA
	setvar VAR_NEWBARKTOWN_LABSTATE, 11
	clearflag FLAG_HIDE_OLIVINE_PORT_OAK
	release
	end

NewBarkTown_Lab_EventScript_TriggerAideGiveBalls::
	lock
	applymovement LOCALID_AIDE1, NewBarkTown_Lab_Movement_Aide1WalkToPlayer
	waitmovement 0
	turnobject OBJ_EVENT_ID_PLAYER, DIR_WEST
	msgbox NewBarkTown_Lab_Text_AideGiveBalls, MSGBOX_DEFAULT
	closemessage
	giveitem ITEM_POKE_BALL, 5
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_FIRST_BALLS
	setvar VAR_NEWBARKTOWN_LABSTATE, 6
	applymovement LOCALID_AIDE1, NewBarkTown_Lab_Movement_Aide1LeavePlayer
	waitmovement 0
	turnobject OBJ_EVENT_ID_PLAYER, DIR_SOUTH
	release
	end

NewBarkTown_Lab_EventScript_Elm_Check_WhereIsEgg::
	msgbox NewBarkTown_Lab_Text_ElmWaitingEggHatch, MSGBOX_DEFAULT
	closemessage
	release
	end

NewBarkTown_Lab_EventScript_Elm_ThoughtEggHatched::
	msgbox NewBarkTown_Lab_Text_ElmThoughtEggHatched, MSGBOX_DEFAULT
	closemessage
	release
	end

NewBarkTown_Lab_EventScript_Elm_Check_Togepi::
	checkspecies SPECIES_TOGEPI
	goto_if_eq VAR_RESULT, TRUE, NewBarkTown_Lab_EventScript_Elm_Togepi_Seen
	checkspecies SPECIES_TOGETIC
	goto_if_eq VAR_RESULT, TRUE, NewBarkTown_Lab_EventScript_Elm_Togepi_Seen
	checkspecies SPECIES_TOGEKISS
	goto_if_eq VAR_RESULT, TRUE, NewBarkTown_Lab_EventScript_Elm_Togepi_Seen
	return

NewBarkTown_Lab_EventScript_Elm_Togepi_Seen::
	msgbox NewBarkTown_Lab_Text_ShowTogepi1, MSGBOX_DEFAULT
	waitmessage
	msgbox NewBarkTown_Lab_Text_ShowTogepi2, MSGBOX_DEFAULT
	waitmessage
	msgbox NewBarkTown_Lab_Text_ShowTogepi3, MSGBOX_DEFAULT
	waitmessage
	msgbox NewBarkTown_Lab_Text_ElmGiveEverstone1, MSGBOX_DEFAULT
	waitmessage
	giveitem ITEM_EVERSTONE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox NewBarkTown_Lab_Text_ElmGiveEverstone2, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_SHOWN_ELM_TOGEPI
	release
	end

NewBarkTown_Lab_EventScript_Elm_CallMe::
	msgbox NewBarkTown_Lab_Text_Elm_CallMe, MSGBOX_DEFAULT
	closemessage
	release
	end

NewBarkTown_Lab_Movement_WalkRight:
	walk_right
	step_end

NewBarkTown_Lab_Movement_WalkLeft:
	walk_left
	step_end

NewBarkTown_LabMovement_WalkToElm:
	walk_up
	walk_up
	walk_up
	step_end

NewBarkTown_Lab_Movement_ReturnPlayer:
	walk_up
	step_end

NewBarkTown_Lab_Movement_CheckEmail:
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_up
	step_end

NewBarkTown_Lab_Movement_CheckEmailSlow:
	walk_left
	walk_left
	walk_left
	walk_up
	step_end

NewBarkTown_Lab_Movement_GotoComputerSafe:
	walk_up
	walk_left
	walk_left
	walk_left
	face_up
	step_end

NewBarkTown_Lab_Movement_ReturnFromEmail:
	walk_fast_down
	walk_fast_right
	walk_fast_right
	walk_fast_right
	face_down
	step_end

NewBarkTown_Lab_Movement_Look_Left:
	face_left
	step_end

NewBarkTown_Lab_Movement_Look_Up:
	face_up
	step_end


NewBarkTown_Lab_Movement_Aide1WalkToPlayer:
	walk_down
	walk_down
	walk_right
	walk_right
	step_end

NewBarkTown_Lab_Movement_Aide1LeavePlayer:
	walk_left
	walk_left
	walk_up
	walk_up
	face_down
	step_end

NeBarkTownLab_Movement_PlayerWalkToCop:
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_left
	walk_fast_up
	face_right
	step_end

NewBarkTown_Lab_Movement_CopLeaves:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

NewBarkTown_Lab_Movement_PlayerStepsToElm:
	walk_right
	face_up
	step_end

NewBarkTown_Lab_Movement_ElmStepBack:
	lock_facing_direction
	walk_fast_up
	unlock_facing_direction
	step_end

NewBarkTown_Lab_Movement_ElmStepForward:
	walk_fast_down
	step_end

NewBarkTown_Lab_Text_ShouldntTouch:
	.string "...만지지는 말자!$"

NewBarkTown_Lab_Text_ElmIntro:
	.string "공박사: 앗 {PLAYER}{K_AYA}\n"
	.string "오기를 기다리고 있었단다!\p"
	.string "오늘 너를 부른 것은\n"
	.string "부탁할 일이 있어서란다!\p"
	.string "지금 포켓몬에 대해\n"
	.string "새로운 연구를 하고 있는데,\l"
	.string "{PLAYER}도 도와줬으면 좋겠구나!\p"
	.string "사실은...\p"
	.string "학회에서 발표할 자료를\n"
	.string "작성하고 있는데,\l"
	.string "아직 모르는 게 있어서 말이야...\p"
	.string "그래서!\p"
	.string "내가 얼마 전에 잡은 포켓몬을\n"
	.string "네가 키워 줬으면 좋겠구나!$"

NewBarkTown_Lab_Text_ElmAccepted:
	.string "{PLAYER}{K_AYA} 고맙다!\n"
	.string "정말 큰 도움이 되겠구나!$"

NewBarkTown_Lab_Text_ElmRefused:
	.string "그런 말 하지 말고!\n"
	.string "부탁한다!$"

NewBarkTown_Lab_Text_ElmResearchAmbitions:
	.string "이 연구를 발표하면\n"
	.string "포켓몬의 수수께끼를 푸는 데\l"
	.string "한 걸음 더 다가갈 게 틀림없단다!\p"
	.string "기대해 주렴!$"

NewBarkTown_Lab_Text_ElmGotAnEmail:
	.string "아 메일이다!\p"
	.string "흠흠\n"
	.string "호- 그렇구나…$"

NewBarkTown_Lab_Text_ElmMissionFromMrPokemon:
	.string "공박사: 저기 내가 아는 사람 중에\n"
	.string "포켓몬 할아버지라고\p"
	.string "희귀한 것을 찾아와서\n"
	.string "대발견! 이라고 큰 소동을 벌이는\l"
	.string "할아버지가 있지.\p"
	.string "그런데 지금\n"
	.string "“이번에야말로 진짜일세!”\l"
	.string "라는 메일이 왔다.\p"
	.string "아마 또\n"
	.string "포켓몬알이라고 생각하지만\p"
	.string "나도 조수도\n"
	.string "지금 바쁘니까...\p"
	.string "아!\p"
	.string "그렇지!\p"
	.string "네가 대신 가주면\n"
	.string "딱 좋겠다!$"

NewBarkTown_Lab_Text_ElmChooseAPokemon:
	.string "네가 키워 줬으면 하는 포켓몬이\n"
	.string "이 몬스터볼 안에 있단다!\p"
	.string "그 포켓몬의 첫 번째 파트너는\n"
	.string "{PLAYER}{K_IGA} 되는 거란다!\p"
	.string "자, 골라 보렴!$"

NewBarkTown_Lab_Text_ElmWhereGoing:
	.string "공박사: 잠깐!\n"
	.string "어디 가느냐?$"

NewBarkTown_Lab_Text_ChooseCyndaquil:
	.string "공박사: {STR_VAR_2}포켓몬\n"
	.string "{STR_VAR_1}{K_EU}로 하겠니?$"

NewBarkTown_Lab_Text_ChooseTotodile:
	.string "공박사: {STR_VAR_2}포켓몬\n"
	.string "{STR_VAR_1}{K_EU}로 하겠니?$"

NewBarkTown_Lab_Text_ChooseChikorita:
	.string "공박사: {STR_VAR_2}포켓몬\n"
	.string "{STR_VAR_1}{K_EU}로 하겠니?$"

NewBarkTown_Lab_Text_DidntChooseStarter:
	.string "공박사: 신중하게 골라 보렴.\p"
	.string "소중한 파트너가 될 테니까!$"

NewBarkTown_Lab_Text_ChoseStarter:
	.string "공박사: 나도 그 포켓몬은\n"
	.string "좋은 포켓몬이라고 생각한다!$"

NewBarkTown_Lab_Text_ReceivedStarter:
	.string "{PLAYER}{K_EUNNEUN} 공박사에게\n"
	.string "{STR_VAR_1}{K_EULREUL} 받았다!$"

NewBarkTown_Lab_Text_Nickname:
	.string "{STR_VAR_1}에게\n"
	.string "별명을 붙이겠습니까?$"

NewBarkTown_Lab_Text_ElmDirections1:
	.string "공박사: 포켓몬 할아버지는\n"
	.string "여러 장소에 가서\l"
	.string "희귀한 것을 발견해 온단다.\p"
	.string "무궁시티를 빠져나가서\n"
	.string "조금 북쪽에 살고 있지.\p"
	.string "혹시 모르니 내\n"
	.string "전화번호를 알려주지!\p"
	.string "무슨 일이 있으면\n"
	.string "연락하거라!$"

NewBarkTown_Lab_Text_ElmDirections1_Register:
	.string "공박사: 포켓몬 할아버지는\n"
	.string "여러 장소에 가서\l"
	.string "희귀한 것을 발견해 온단다.\p"
	.string "무궁시티를 빠져나가서\n"
	.string "조금 북쪽에 살고 있지.\p"
	.string "혹시 모르니 내\n"
	.string "전화번호를 알려주지!\p"
	.string "무슨 일이 있으면\n"
	.string "연락하거라!$"

NewBarkTown_Lab_Text_RegisteredElm:
	.string "{PLAYER}{K_EUNNEUN} 포켓기어에\n"
	.string "공박사를 등록했다!$"


NewBarkTown_Lab_Text_ElmLetYourMonBattle:
	.string "If a wild POKéMON appears,\n"
	.string "let your POKéMON battle it!$"

NewBarkTown_Lab_Text_ElmDirections2:
	.string "포켓몬을 가지고 있으면\n"
	.string "야생의 포켓몬이 나와도\l"
	.string "싸우게 하면 되는 거다!\p"
	.string "포켓몬이 상처입었을 때는\n"
	.string "저기 있는 머신으로\l"
	.string "회복시키면 된다.\p"
	.string "그럼\n"
	.string "{PLAYER}{K_AYA} 잘 부탁한다!$"

NewBarkTown_Lab_Text_GotElmsNumber:
	.string "{PLAYER} got ELM's phone\n"
	.string "number.$"

NewBarkTown_Lab_Text_ElmDescribesMrPokemon:
	.string "MR. POKéMON goes everywhere and\n"
	.string "finds rarities.\p"
	.string "Too bad they're just rare and\n"
	.string "not very useful…$"

NewBarkTown_Lab_Text_ElmPokeBall:
	.string "It contains a POKéMON caught\n"
	.string "by PROF.ELM.$"

@THEFT================================================================================

NewBarkTown_Lab_Text_ElmAfterTheft1:
	.string "공박사: {PLAYER}{K_AYA}...\n"
	.string "큰일이 있었다.\p"
	.string "아, 맞아!\n"
	.string "포켓몬 할아버지의\l"
	.string "대발견이라는 게 뭐였느냐?$"

NewBarkTown_Lab_Text_ElmAfterTheft2:
	.string "{PLAYER}{K_EUNNEUN} 공박사에게\n"
	.string "이상한알을 전해줬다!$"

NewBarkTown_Lab_Text_ElmAfterTheft3:
	.string "공박사: 응? 뭐라고?\n"
	.string "이건 알...이지?\p"
	.string "확실히 이건 본 적이 없는\n"
	.string "종류인 것 같은데...\p"
	.string "이제 와서 포켓몬알을 보고\n"
	.string "놀라다니 포켓몬 할아버지도\l"
	.string "여전하구나...\p"
	.string "그런데... 모처럼 받은 알이고\n"
	.string "혹시 뭔가 비밀이\l"
	.string "있을지도 모른다.\p"
	.string "좋아!\n"
	.string "당분간 내가 맡아두고\l"
	.string "조사해 보겠다-$"

NewBarkTown_Lab_Text_ElmAfterTheft4:
	.string "공박사: 뭐? 오박사님께\n"
	.string "포켓몬 도감을 받았다고!?\p"
	.string "{PLAYER}{K_AYA} 정말이냐!?\n"
	.string "그거 대단한 일이란다!\p"
	.string "뭐니 뭐니 해도 오박사님은\n"
	.string "트레이너의 재능을 꿰뚫어보는\l"
	.string "힘이 있는 분이니까.\p"
	.string "호오 그렇구나...\n"
	.string "나도 {PLAYER}{K_IGA}\l"
	.string "보통 사람이 아니라고 생각했지만\l"
	.string "이건 일이 재미있게\l"
	.string "되었구나!\p"
	.string "너에게 준 포켓몬도\n"
	.string "완전히 친해진 듯하고...\p"
	.string "좋아!\p"
	.string "이대로 각지의 포켓몬체육관에\n"
	.string "도전해보는 게 어떻겠느냐?\p"
	.string "그리고...\n"
	.string "전부를 이겨나갈 수 있다면\l"
	.string "그 앞에 기다리고 있는 것은...\l"
	.string "포켓몬리그의 챔피언!\p"
	.string "...일 수도 있겠구나!\p"
	.string "챔피언을 목표로 하는 것은\n"
	.string "간단한 일이 아니지만\p"
	.string "우선 여기서 제일 가까운\n"
	.string "도라지시티의 체육관이라면\p"
	.string "너 정도의 힘이 있으면\n"
	.string "어떻게든 될 거다.\p"
	.string "...{PLAYER}{K_AYA}\n"
	.string "각지의 포켓몬체육관을\l"
	.string "하나씩 찾아 가는 것은\l"
	.string "긴 여행이 될 것으로 생각한다.\p"
	.string "출발하기 전에\n"
	.string "어머님께 얘기해 두어라.$"

NewBarkTown_Lab_Text_ElmStudyingEgg:
	.string "공박사: 열심히 하거라!\n"
	.string "나도 알에 대해 아는 대로\l"
	.string "연락하겠다!$"

NewBarkTown_Lab_Text_ElmAideHasEgg:
	.string "ELM: {PLAYER}? Didn't you meet\n"
	.string "my assistant?\p"
	.string "He should have met you with\n"
	.string "the EGG at VIOLET CITY's\p"
	.string "POKéMON CENTER.\p"
	.string "You must have just missed him.\n"
	.string "Try to catch him there.$"

NewBarkTown_Lab_Text_ElmWaitingEggHatch:
	.string "공박사: 어이 {PLAYER}{K_AYA}\n"
	.string "건강해 보이는구나!\p"
	.string "그런데...\n"
	.string "알의 상태는 어떤가?\p"
	.string "어디에 갈 때도\n"
	.string "잘 데리고 다니고 있느냐?\p"
	.string "시간과 애정을 쏟아\n"
	.string "알을 대하면\l"
	.string "반드시 건강한 포켓몬이\l"
	.string "태어날 거다!$"

NewBarkTown_Lab_Text_ElmThoughtEggHatched:
	.string "{PLAYER}? I thought the EGG\n"
	.string "hatched.\p"
	.string "Where is the POKéMON?$"

NewBarkTown_Lab_Text_ShowTogepi1:
	.string "공박사: 어이 {PLAYER}{K_AYA}\n"
	.string "건강해 보이는구나!$"

NewBarkTown_Lab_Text_ShowTogepi2:
	.string "엥?\n"
	.string "그 포켓몬은...!$"

NewBarkTown_Lab_Text_ShowTogepi3:
	.string "흐-음 이런 포켓몬은\n"
	.string "내가 아는 한 성도에서는\l"
	.string "발견한 적이 없다...\p"
	.string "과연 포켓몬 할아버지가\n"
	.string "전화로 말한 대로구나.\l"
	.string "이 정도라면 전설의 포켓...\p"
	.string "아 아니 아니 아무것도 아니다!\p"
	.string "어쨌든 앞으로도\n"
	.string "{PLAYER}{K_EULREUL} 중심으로\l"
	.string "대단한 일이 생기는 것은\l"
	.string "틀림없을 것 같구나!$"

NewBarkTown_Lab_Text_ElmGiveEverstone1:
	.string "좋-아! 이렇게\n"
	.string "두근두근하게 해주는 너에게\l"
	.string "선물을 주겠다!\l"
	.string "사양 말고 가지고 가거라!$"

NewBarkTown_Lab_Text_ElmGiveEverstone2:
	.string "그것은 변함없는돌!\p"
	.string "어떤 포켓몬은 일정 레벨에\n"
	.string "도달하면 진화한단다.\p"
	.string "하지만 변함없는돌을\n"
	.string "지니게 하면 진화를\l"
	.string "막을 수 있지.\p"
	.string "진화시키고 싶지 않은\n"
	.string "포켓몬이 있다면\l"
	.string "사용해 보거라!$"

NewBarkTown_Lab_Text_ElmCallYou:
	.string "ELM: {PLAYER}, I'll call you if\n"
	.string "anything comes up.$"

NewBarkTown_Lab_Text_AideAfterTheft:
	.string "…sigh… That stolen POKéMON.\p"
	.string "I wonder how it's doing.\p"
	.string "They say a POKéMON raised by a\n"
	.string "bad person turns bad itself.$"

NewBarkTown_Lab_Text_ElmGiveMasterBall1:
	.string "공박사: 여어 {PLAYER}!\p"
	.string "네 덕분에 내 연구도\n"
	.string "순조롭게 진행되고 있단다!\p"
	.string "감사의 표시로\n"
	.string "이걸 받아 주거라!$"

NewBarkTown_Lab_Text_ElmGiveMasterBall2:
	.string "그것은 마스터볼!\p"
	.string "어떤 포켓몬이든\n"
	.string "반드시 잡을 수 있는\l"
	.string "궁극의 몬스터볼이다!\p"
	.string "포켓몬 연구의 성과를\n"
	.string "인정받은 사람에게만\l"
	.string "주어지는 볼이지.\p"
	.string "나보다 {PLAYER}{K_IGA}\n"
	.string "훨씬 잘 쓸 수 있을 거다!$"

NewBarkTown_Lab_Text_KimonoGirls:
	.string "그런데...\n"
	.string "아름다운 전통무용수들이\l"
	.string "너를 찾으러 왔었단다.\p"
	.string "인주시티의 예술극장에서\n"
	.string "널 기다린다고 하더구나.\p"
	.string "그 알 때문에 여러 가지\n"
	.string "일에 휘말려서\l"
	.string "힘들었을 테지만...\p"
	.string "끝까지 해냈으면 좋겠구나!$"

NewBarkTown_Lab_Text_KimonoWaiting:
	.string "공박사: 전통무용수들이\n"
	.string "인주시티의 예술극장에서\l"
	.string "널 기다리고 있단다!$"

NewBarkTown_Lab_Text_League:
	.string "공박사: 후후...\n"
	.string "수고했다!\p"
	.string "그 희귀한 알 때문에\n"
	.string "큰일을 겪게 되었구나.\p"
	.string "전통무용수들이\n"
	.string "나에 대해 뭐라고 했느냐?\p"
	.string "하지만 나는 그 일과\n"
	.string "아무 관계도 없다.\p"
	.string "모든 것은 네 스스로\n"
	.string "결정하고 해낸 일이다.\p"
	.string "여기까지 해낸 너라면\n"
	.string "포켓몬리그에도\l"
	.string "도전할 수 있을 거다!\p"
	.string "자! 석영고원으로 가거라!\p"
	.string "가는 길은 알고 있느냐?\n"
	.string "27번도로를 지나서\l"
	.string "26번도로로 들어가거라.\p"
	.string "거기서 곧장\n"
	.string "북쪽으로 나아가거라!$"


NewBarkTown_Lab_Text_ElmGiveTicket1:
	.string "공박사: 여어 {PLAYER}!\n"
	.string "기다리고 있었다!\p"
	.string "널 부른 것은\n"
	.string "주고 싶은 게 있어서다!\p"
	.string "자! 승선티켓이다!\p"
	.string "이제 관동지방에서도\n"
	.string "포켓몬을 잡을 수 있겠구나!$"

NewBarkTown_Lab_Text_ElmGiveTicket2:
	.string "쾌속선은 담청시티의\n"
	.string "항구에서 출발한단다.\p"
	.string "하지만 {PLAYER}{K_EUNNEUN}\n"
	.string "이미 알고 있겠구나.\p"
	.string "포켓몬과 함께\n"
	.string "여러 곳을 돌아다녔으니\l"
	.string "말이다.\p"
	.string "관동에 가면\n"
	.string "오박사님께 안부 전해줘!$"

NewBarkTown_Lab_Text_LabMonEgg:
	.string "It's the POKéMON EGG being\n"
	.string "studied by PROF.ELM.$"

NewBarkTown_Lab_Text_AideGivePotion:
	.string "{PLAYER}님!\n"
	.string "심부름하실 때 필요할 테니\l"
	.string "이걸 드리지요!$"

NewBarkTown_Lab_Text_AideAlwaysBusy:
	.string "연구소에는 저희 셋뿐이라\n"
	.string "늘 바쁘답니다.$"

NewBarkTown_Lab_Text_AideSave:
	.string "리포트를 자주 작성해\n"
	.string "모험을 기록해 두세요!\p"
	.string "START 버튼을 누른 다음\n"
	.string "리포트를 기록하면 됩니다!$"

NewBarkTown_Lab_Text_AideTheftTestimony:
	.string "밖에서 큰 소리가 났어요...\p"
	.string "확인하러 나간 사이에\n"
	.string "누군가 포켓몬을 훔쳤어요.\p"
	.string "어떻게 그런 짓을 할 수 있죠!?\p"
	.string "...하아...\n"
	.string "도난당한 포켓몬은\l"
	.string "지금쯤 어떻게 지낼까요...\p"
	.string "나쁜 사람이 키우면\n"
	.string "포켓몬도 나빠진다던데...$"

NewBarkTown_Lab_Text_AideGiveBalls:
	.string "{PLAYER}님!\p"
	.string "포켓몬 도감을 채우는 데\n"
	.string "이걸 사용해 주세요!$"

NewBarkTown_Lab_Text_AideExplainBalls:
	.string "To add to your POKéDEX, you\n"
	.string "have to catch POKéMON.\p"
	.string "Throw POKé BALLS at wild\n"
	.string "POKéMON to get them.$"

NewBarkTown_Lab_Text_Officer1:
	.string "이 연구소에서 포켓몬이\n"
	.string "도난당했다고 하더군…\p"
	.string "방금 공박사님께\n"
	.string "사건 이야기를 듣고 있었지.\p"
	.string "범인은 새빨간 머리의\n"
	.string "남자아이라 하더군…\p"
	.string "뭐라고?\p"
	.string "자네는 그런 트레이너와\n"
	.string "싸웠다는 겐가?\p"
	.string "그런데 그 소년의\n"
	.string "이름은 알고 있나?$"

NewBarkTown_Lab_Text_Officer2_1:
	.string "알겠네!\n"
	.string "{RIVAL}{K_I}라고 하는군?$"

NewBarkTown_Lab_Text_Officer2:
	.string "협력해 줘서 고맙네!$"

NewBarkTown_Lab_Text_OfficerNo:
	.string "그 소년의 이름을\n"
	.string "알고 있지 않나?$"

NewBarkTown_Lab_Text_Elm_CallMe:
	.string "공박사: 아, {PLAYER}!\p"
	.string "포켓몬 도감은\n"
	.string "어디까지 채웠느냐?\p"
	.string "지금은 바쁘지만\n"
	.string "나중에 전화해 주면\l"
	.string "얼마나 진행됐는지 살펴보겠다!$"
