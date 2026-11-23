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
	.string "{PLAYER}!\p"
	.string "What are you doing here?\p"
	.string "My aide is still waiting for you\n"
	.string "In VIOLET CITY!$"

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
	.string "ELM: {PLAYER}! There you are!\p"
	.string "I needed to ask you a favor.\p"
	.string "I'm conducting new POKéMON\n"
	.string "research right now. I was\l"
	.string "wondering if you could help\l"
	.string "me with it, {PLAYER}.\p"
	.string "You see…\p"
	.string "I'm writing a paper that I\n"
	.string "want to present at a conference.\p"
	.string "But there are some things I\n"
	.string "don't quite understand yet.\p"
	.string "So!\p"
	.string "I'd like you to raise a\n"
	.string "POKéMON that I recently caught.$"

NewBarkTown_Lab_Text_ElmAccepted:
	.string "Thanks, {PLAYER}!\n"
	.string "You're a great help!$"

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
	.string "Hey, listen.\p"
	.string "I have an acquaintance called\n"
	.string "MR. POKéMON.\p"
	.string "He keeps finding weird things and\n"
	.string "raving about his discoveries.\p"
	.string "Anyway, I just got an e-mail\n"
	.string "from him saying that this\l"
	.string "time it's real.\p"
	.string "It is intriguing, but we're\n"
	.string "busy with our POKéMON research...\p"
	.string "Wait!\n"
	.string "I know!\p"
	.string "{PLAYER}, can you go in our\n"
	.string "place?$"

NewBarkTown_Lab_Text_ElmChooseAPokemon:
	.string "I want you to raise one of\n"
	.string "the POKéMON contained in these BALLS.\p"
	.string "You'll be that POKéMON's\n"
	.string "first partner, {PLAYER}!\p"
	.string "Go on. Pick one!$"

NewBarkTown_Lab_Text_ElmWhereGoing:
	.string "공박사: 잠깐!\n"
	.string "어디 가느냐?$"

NewBarkTown_Lab_Text_ChooseCyndaquil:
	.string "ELM: You'll take {STR_VAR_1},\n"
	.string "the {STR_VAR_2} POKéMON?$"

NewBarkTown_Lab_Text_ChooseTotodile:
	.string "ELM: Do you want {STR_VAR_1},\n"
	.string "the {STR_VAR_2} POKéMON?$"

NewBarkTown_Lab_Text_ChooseChikorita:
	.string "ELM: So, you like {STR_VAR_1},\n"
	.string "the {STR_VAR_2} POKéMON?$"

NewBarkTown_Lab_Text_DidntChooseStarter:
	.string "공박사: 신중하게 골라 보렴.\p"
	.string "소중한 파트너가 될 테니까!$"

NewBarkTown_Lab_Text_ChoseStarter:
	.string "공박사: 나도 그 포켓몬은\n"
	.string "좋은 포켓몬이라고 생각한다!$"

NewBarkTown_Lab_Text_ReceivedStarter:
	.string "{PLAYER} received {STR_VAR_1}!$"

NewBarkTown_Lab_Text_Nickname:
	.string "Would you like to give your\n"
	.string "{STR_VAR_1} a nickname?$"

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
	.string "Registered PROF.ELM\n"
	.string "in the POKéGEAR.$"


NewBarkTown_Lab_Text_ElmLetYourMonBattle:
	.string "If a wild POKéMON appears,\n"
	.string "let your POKéMON battle it!$"

NewBarkTown_Lab_Text_ElmDirections2:
	.string "If a wild POKéMON appears, let\n"
	.string "your POKéMON battle it!\p"
	.string "If your POKéMON is hurt, you\n"
	.string "should heal it with the machine\l"
	.string "to my right, or at the POKéCENTER.\p"
	.string "I'm counting on you!$"

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
	.string "ELM: {PLAYER}, this is terrible…\p"
	.string "Oh, yes, what was MR. POKéMON's\n"
	.string "big discovery?$"

NewBarkTown_Lab_Text_ElmAfterTheft2:
	.string "{PLAYER} handed the MYSTERY EGG\n"
	.string "to PROF.ELM.$"

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
	.string "But… Is it a POKéMON EGG?\p"
	.string "If it is, it is a great\n"
	.string "discovery!\p"
	.string "ELM: What?!?\p"
	.string "PROF.OAK gave you a POKéDEX?\p"
	.string "{PLAYER}, is that true?\n"
	.string "Th-that's incredible!\p"
	.string "He is superb at seeing the\n"
	.string "potential of people as trainers.\p"
	.string "Wow, {PLAYER}. You may have what\n"
	.string "it takes to become the CHAMPION.\p"
	.string "You seem to be getting on\n"
	.string "great with POKéMON too.\p"
	.string "You should take the POKéMON\n"
	.string "GYM challenge.\p"
	.string "The closest GYM would be the\n"
	.string "one in VIOLET CITY.\p"
	.string "…{PLAYER}. The road to the\n"
	.string "championship will be a long one.\p"
	.string "Before you leave, make sure\n"
	.string "that you talk to your mom.$"

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
	.string "ELM: Hey, has that EGG changed\n"
	.string "any?$"

NewBarkTown_Lab_Text_ElmThoughtEggHatched:
	.string "{PLAYER}? I thought the EGG\n"
	.string "hatched.\p"
	.string "Where is the POKéMON?$"

NewBarkTown_Lab_Text_ShowTogepi1:
	.string "ELM: {PLAYER}, you look great!$"

NewBarkTown_Lab_Text_ShowTogepi2:
	.string "엥?\n"
	.string "그 포켓몬은...!$"

NewBarkTown_Lab_Text_ShowTogepi3:
	.string "The EGG hatched! So, POKéMON\n"
	.string "are born from EGGS…\p"
	.string "No, perhaps not all POKéMON\n"
	.string "are.\p"
	.string "Wow, there's still a lot of\n"
	.string "research to be done.$"

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
	.string "ELM: Hi, {PLAYER}!\p"
	.string "Thanks to you, my research is\n"
	.string "going great!\p"
	.string "Take this as a token of my\n"
	.string "appreciation.$"

NewBarkTown_Lab_Text_ElmGiveMasterBall2:
	.string "The MASTER BALL is the best!\p"
	.string "It's the ultimate BALL! It'll\n"
	.string "catch any POKéMON without fail.\p"
	.string "It's given only to recognized\n"
	.string "POKéMON researchers.\p"
	.string "I think you can make much better\n"
	.string "use of it than I can, {PLAYER}!$"

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
	.string "ELM: {PLAYER}! There you are!\p"
	.string "I called because I have\n"
	.string "something for you.\p"
	.string "See? It's an S.S.TICKET.\p"
	.string "Now you can catch POKéMON in\n"
	.string "KANTO.$"

NewBarkTown_Lab_Text_ElmGiveTicket2:
	.string "The ship departs from OLIVINE\n"
	.string "CITY.\p"
	.string "But you knew that already,\n"
	.string "{PLAYER}.\p"
	.string "After all, you've traveled all\n"
	.string "over with your POKéMON.\p"
	.string "Give my regards to PROF.OAK in\n"
	.string "KANTO!$"

NewBarkTown_Lab_Text_LabMonEgg:
	.string "It's the POKéMON EGG being\n"
	.string "studied by PROF.ELM.$"

NewBarkTown_Lab_Text_AideGivePotion:
	.string "{PLAYER}, I want you to have\n"
	.string "this for your errand.$"

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
	.string "{PLAYER}!\p"
	.string "Use these on your POKéDEX\n"
	.string "quest!$"

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
	.string "OK! So {RIVAL} was his name?$"

NewBarkTown_Lab_Text_Officer2:
	.string "협력해 줘서 고맙네!$"

NewBarkTown_Lab_Text_OfficerNo:
	.string "그 소년의 이름을\n"
	.string "알고 있지 않나?$"

NewBarkTown_Lab_Text_Elm_CallMe:
	.string "Ah, {PLAYER}!\p"
	.string "How's your POKéDEX coming along?\n"
	.string "I'm busy now, but call me sometime\l"
	.string "and I can check your progress!$"
