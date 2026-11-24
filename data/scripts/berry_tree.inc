#if IS_HNS

BerryTreeScript::
	special ObjectEventInteractionGetBerryTreeData
	switch VAR_0x8004
	case BERRY_STAGE_SPARKLING, BerryTree_EventScript_Hns_NotRipe
	case BERRY_STAGE_NO_BERRY, BerryTree_EventScript_Hns_NotRipe
	case BERRY_STAGE_PLANTED, BerryTree_EventScript_Hns_NotRipe
	case BERRY_STAGE_SPROUTED, BerryTree_EventScript_Hns_NotRipe
	case BERRY_STAGE_TALLER, BerryTree_EventScript_Hns_NotRipe
	case BERRY_STAGE_FLOWERING, BerryTree_EventScript_Hns_NotRipe
	case BERRY_STAGE_BERRIES, BerryTree_EventScript_Hns_FullyGrown
	end

BerryTree_EventScript_Hns_NotRipe::
	lock
	message BerryTree_Text_Hns_NotRipe
	waitmessage
	waitbuttonpress
	release
	end

BerryTree_EventScript_Hns_FullyGrown::
	buffernumberstring STR_VAR_2, VAR_0x8006
	lock
	faceplayer
	goto BerryTree_EventScript_Hns_PickBerry

BerryTree_EventScript_Hns_PickBerry::
	special ObjectEventInteractionPickBerryTree
	goto_if_eq VAR_0x8004, 0, BerryTree_EventScript_Hns_BagFull
	special IncrementDailyPickedBerries
	special ObjectEventInteractionRemoveBerryTree
	message BerryTree_Text_Hns_Picked
	playfanfare MUS_OBTAIN_BERRY
	special ObjectEventInteractionPlantBerryTree
	incrementgamestat GAME_STAT_PLANTED_BERRIES
	special IncrementDailyPlantedBerries
	waitmessage
	waitfanfare
	release
	end

BerryTree_EventScript_Hns_BagFull::
	message BerryTree_Text_Hns_BagFull
	waitmessage
	waitbuttonpress
	release
	end

BerryTree_EventScript_Hns_Sprout::
	message BerryTree_Text_Hns_Sprouted
	waitmessage
	callnative Berry_Ready
	waitbuttonpress
	release
	end

BerryTree_EventScript_ItemUsePlantBerry::
	end

BerryTree_EventScript_ItemUseWailmerPail::
	end

BerryTree_Text_Hns_NotRipe:
	.string "나무열매가 열린 나무다!\n"
	.string "열매는 아직 안 익은 듯하다.$"

BerryTree_Text_Hns_Picked:
	.string "{PLAYER}{K_EUNNEUN}\n"
	.string "{STR_VAR_1}{K_EULREUL} {STR_VAR_2}개 땄다!$"

BerryTree_Text_Hns_BagFull:
	.string "가방이 가득 찼다.\p"
	.string "{STR_VAR_1}{K_EULREUL} 포기했다.$"

BerryTree_Text_Hns_Sprouted:
	.string "나무열매가 갑자기 싹을 틔웠다!$"

#else

BerryTreeScript::
	special ObjectEventInteractionGetBerryTreeData
	switch VAR_0x8004
	case BERRY_STAGE_SPARKLING, BerryTree_EventScript_Sparkling
	case BERRY_STAGE_NO_BERRY, BerryTree_EventScript_CheckSoil
	case BERRY_STAGE_PLANTED, BerryTree_EventScript_CheckBerryStage1
	case BERRY_STAGE_SPROUTED, BerryTree_EventScript_CheckBerryStage2
	case BERRY_STAGE_TALLER, BerryTree_EventScript_CheckBerryStage3
	case BERRY_STAGE_TRUNK, BerryTree_EventScript_CheckBerryStageTrunk
	case BERRY_STAGE_BUDDING, BerryTree_EventScript_CheckBerryStageBudding
	case BERRY_STAGE_FLOWERING, BerryTree_EventScript_CheckBerryStage4
	case BERRY_STAGE_BERRIES, BerryTree_EventScript_CheckBerryFullyGrown
	end

BerryTree_EventScript_Sparkling::
	lockall
	message BerryTree_Text_ExclamationPoint
	waitmessage
	waitbuttonpress
	releaseall
	end

BerryTree_EventScript_CheckSoil::
	lock
	faceplayer
	specialvar VAR_RESULT, PlayerHasBerries
	goto_if_eq VAR_RESULT, TRUE, BerryTree_EventScript_WantToPlant
	specialvar VAR_RESULT, PlayerHasMulch
	goto_if_eq VAR_RESULT, TRUE, BerryTree_EventScript_WantToMulch
	message BerryTree_Text_ItsSoftLoamySoil
	waitmessage
	waitbuttonpress
	release
	end

BerryTree_EventScript_WantToPlant::
	specialvar VAR_RESULT, PlayerHasMulch
	goto_if_eq VAR_RESULT, TRUE, BerryTree_EventScript_WantToPlantMulch
	msgbox BerryTree_Text_WantToPlant, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, BerryTree_EventScript_ChooseBerryToPlant
	goto_if_eq VAR_RESULT, NO, BerryTree_EventScript_CancelPlanting
	end

BerryTree_EventScript_WantToMulch::
.if OW_BERRY_MULCH_USAGE == TRUE
	msgbox BerryTree_Text_WantToMulch, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, BerryTree_EventScript_ChooseMulchToUse
	goto_if_eq VAR_RESULT, NO, BerryTree_EventScript_CancelPlanting
.endif
	end

BerryTree_EventScript_WantToPlantMulch::
.if OW_BERRY_MULCH_USAGE == TRUE
	message BerryTree_Text_ItsSoftLoamySoil
	waitmessage
	multichoice 0, 0, MULTI_BERRY_PLOT, FALSE
	switch VAR_RESULT
	case 0, BerryTree_EventScript_ChooseMulchToUse
	case 1, BerryTree_EventScript_ChooseBerryToPlant
	case 2, BerryTree_EventScript_CancelPlanting
	case MULTI_B_PRESSED, BerryTree_EventScript_CancelPlanting

BerryTree_EventScript_ChooseMulchToUse::
	fadescreen FADE_TO_BLACK
	closemessage
	special Bag_ChooseMulch
	waitstate
	goto_if_eq VAR_ITEM_ID, 0, BerryTree_EventScript_CancelPlanting
	removeitem VAR_ITEM_ID
	call BerryTree_EventScript_UseMulch
.endif

BerryTree_EventScript_ChooseBerryToPlant::
	fadescreen FADE_TO_BLACK
	closemessage
	special Bag_ChooseBerry
	goto_if_eq VAR_ITEM_ID, 0, BerryTree_EventScript_CancelPlanting
	removeitem VAR_ITEM_ID
	call BerryTree_EventScript_PlantBerry

BerryTree_EventScript_CancelPlanting::
	release
	end

BerryTree_EventScript_CheckBerryStage1::
	lockall
	special ObjectEventInteractionGetBerryCountString
	message BerryTree_Text_BerryGrowthStage1
	waitmessage
	waitbuttonpress
	goto BerryTree_EventScript_WantToWater

BerryTree_EventScript_CheckBerryStage2::
	lockall
	special ObjectEventInteractionGetBerryName
	message BerryTree_Text_BerryGrowthStage2
	waitmessage
	waitbuttonpress
	goto BerryTree_EventScript_WantToWater

BerryTree_EventScript_CheckBerryStage3::
	lockall
	special ObjectEventInteractionGetBerryName
	message BerryTree_Text_BerryGrowthStage3
	waitmessage
	waitbuttonpress
	goto BerryTree_EventScript_WantToWater

BerryTree_EventScript_CheckBerryStageTrunk::
	lockall
	special ObjectEventInteractionGetBerryName
	message BerryTree_Text_BerryGrowthStageTrunk
	waitmessage
	waitbuttonpress
	goto BerryTree_EventScript_WantToWater

BerryTree_EventScript_CheckBerryStageBudding::
	lockall
	special ObjectEventInteractionGetBerryName
	message BerryTree_Text_BerryGrowthStageBudding
	waitmessage
	waitbuttonpress
	goto BerryTree_EventScript_WantToWater

BerryTree_EventScript_CheckBerryStage4::
	call BerryTree_EventScript_GetCareAdverb
	lockall
	special ObjectEventInteractionGetBerryName
	message BerryTree_Text_BerryGrowthStage4
	waitmessage
	waitbuttonpress
	goto BerryTree_EventScript_WantToWater

@ VAR_0x8005 here is the number of times watered
@ Buffered by ObjectEventInteractionGetBerryTreeData
BerryTree_EventScript_GetCareAdverb::
	goto_if_eq VAR_0x8005, 0, BerryTree_EventScript_SetAdverbPoor
	goto_if_eq VAR_0x8005, 4, BerryTree_EventScript_SetAdverbGreat
	bufferstring STR_VAR_2, BerryTree_Text_CareAdverbGood
	return

BerryTree_EventScript_SetAdverbGreat::
	bufferstring STR_VAR_2, BerryTree_Text_CareAdverbGreat
	return

BerryTree_EventScript_SetAdverbPoor::
	bufferstring STR_VAR_2, BerryTree_Text_CareAdverbPoor
	return

@ VAR_0x8006 here is the number of berries
BerryTree_EventScript_CheckBerryFullyGrown::
	buffernumberstring STR_VAR_2, VAR_0x8006
	lock
	faceplayer
	special ObjectEventInteractionGetBerryCountString
.if OW_BERRY_MUTATIONS == TRUE
	goto_if_eq VAR_RESULT, TRUE, BerryTree_EventScript_CheckBerryFullyGrown_Mutation
.endif
	msgbox BerryTree_Text_WantToPick, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, BerryTree_EventScript_PickBerry
	goto_if_eq VAR_RESULT, NO, BerryTree_EventScript_CancelPickingBerry

.set BERRY_NORMAL_BAG_FULL, 0
.set BERRY_NORMAL_SPACE_IN_BAG, 1
.set BERRY_MUTATION_BAG_FULL, 2
.set BERRY_MUTATION_SPACE_IN_BAG, 3

BerryTree_EventScript_PickBerry::
	special ObjectEventInteractionPickBerryTree
	goto_if_eq VAR_0x8004, BERRY_NORMAL_BAG_FULL, BerryTree_EventScript_BerryPocketFull
.if OW_BERRY_MUTATIONS == TRUE
	goto_if_eq VAR_0x8004, BERRY_MUTATION_BAG_FULL, BerryTree_EventScript_BerryPocketFull_Mutation
	goto_if_eq VAR_0x8004, BERRY_MUTATION_SPACE_IN_BAG, BerryTree_EventScript_PickBerry_Mutation
.endif
	special IncrementDailyPickedBerries
	special ObjectEventInteractionRemoveBerryTree
	message BerryTree_Text_PickedTheBerry
	delay 10
	showberrydescription
	playfanfare MUS_OBTAIN_BERRY
	waitmessage
	waitfanfare
	waitbuttonpress
	message BerryTree_Text_PutAwayBerry
	waitmessage
	waitbuttonpress
	hideitemdescription
	release
	end

BerryTree_EventScript_BerryPocketFull::
	message BerryTree_Text_BerryPocketFull
	waitmessage
	waitbuttonpress
	release
	end

BerryTree_EventScript_CancelPickingBerry::
	message BerryTree_Text_BerryLeftUnpicked
	waitmessage
	waitbuttonpress
	release
	end

BerryTree_EventScript_ItemUsePlantBerry::
	lockall
	special ObjectEventInteractionGetBerryTreeData
	call BerryTree_EventScript_PlantBerry
	releaseall
	end

BerryTree_EventScript_WantToWater::
.if OW_BERRY_PESTS == TRUE
	call BerryTree_EventScript_CheckForPests
.endif
.if OW_BERRY_WEEDS == TRUE
	call BerryTree_EventScript_CheckForWeed
.endif
	checkitem ITEM_WAILMER_PAIL
	goto_if_eq VAR_RESULT, FALSE, BerryTree_EventScript_DontWater
	specialvar VAR_RESULT, CanWaterBerryPlot
	goto_if_eq VAR_RESULT, FALSE, BerryTree_EventScript_DontWater
	special ObjectEventInteractionGetBerryName
	msgbox BerryTree_Text_WantToWater, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, BerryTree_EventScript_WaterBerry
	goto_if_eq VAR_RESULT, NO, BerryTree_EventScript_DontWater

BerryTree_EventScript_DontWater::
	releaseall
	end

BerryTree_EventScript_ItemUseWailmerPail::
	special ObjectEventInteractionGetBerryTreeData
	lockall
BerryTree_EventScript_WaterBerry::
	special ObjectEventInteractionGetBerryName
	message BerryTree_Text_WateredTheBerry
	waitmessage
	special ObjectEventInteractionWaterBerryTree
	special DoWateringBerryTreeAnim
	message BerryTree_Text_PlantIsDelighted
	waitmessage
	waitbuttonpress
	releaseall
	end

BerryTree_EventScript_PlantBerry::
	special ObjectEventInteractionPlantBerryTree
	incrementgamestat GAME_STAT_PLANTED_BERRIES
	special IncrementDailyPlantedBerries
	special ObjectEventInteractionGetBerryCountString
	message BerryTree_Text_PlantedOneBerry
	waitmessage
	waitbuttonpress
	return

BerryTree_Text_ItsSoftLoamySoil:
	.string "푹신푹신한 흙이다.$"

BerryTree_Text_WantToPlant:
	.string "푹신푹신한 흙이다.\n"
	.string "나무열매를 심겠습니까?$"

BerryTree_Text_PlantedOneBerry:
	.string "{PLAYER}{K_EUNNEUN} 푹신푹신한 흙에\n"
	.string "{STR_VAR_1}{K_EULREUL} 하나 심었다!$"

BerryTree_Text_BerryGrowthStage1:
.if OW_BERRY_SIX_STAGES == TRUE
	.string "{STR_VAR_1}{K_EULREUL} 심었던 장소다!$"
.else
	.string "{STR_VAR_1}{K_EULREUL} 심었던 장소다!$"
.endif

BerryTree_Text_BerryGrowthStage2:
.if OW_BERRY_SIX_STAGES == TRUE
	.string "{STR_VAR_1}의 싹이 나왔다!$"
.else
	.string "{STR_VAR_1}의 싹이 나왔다!$"
.endif

BerryTree_Text_BerryGrowthStage3:
.if OW_BERRY_SIX_STAGES == TRUE
	.string "{STR_VAR_1}의 새싹이\n"
	.string "잘 자라고 있다!$"
.else
	.string "{STR_VAR_1}의 줄기가\n"
	.string "커다랗게 자랐다!$"
.endif

BerryTree_Text_BerryGrowthStageTrunk:
	.string "{STR_VAR_1}의 줄기가\n"
	.string "커다랗게 자랐다!$"

BerryTree_Text_BerryGrowthStageBudding:
	.string "{STR_VAR_1}의 꽃봉오리가 맺혔다!$"

BerryTree_Text_BerryGrowthStage4:
.if OW_BERRY_SIX_STAGES == TRUE
	.string "{STR_VAR_1}의\n"
	.string "꽃이 피어 있다!$"
.else
	.string "{STR_VAR_1}의 꽃이\n"
	.string "{STR_VAR_2} 피어 있다!$"
.endif

BerryTree_Text_CareAdverbGreat:
	.string "아주 예쁘게$"

BerryTree_Text_CareAdverbPoor:
	.string "귀엽게$"

BerryTree_Text_CareAdverbGood:
	.string "예쁘게$"

BerryTree_Text_WantToPick:
	.string "{STR_VAR_1}{K_IGA} {STR_VAR_2}개 열려 있다!\p"
	.string "{STR_VAR_1}{K_EULREUL} 따겠습니까?$"

BerryTree_Text_PickedTheBerry:
	.string "{PLAYER}{K_EUNNEUN}\n"
	.string "{STR_VAR_1}{K_EULREUL} {STR_VAR_2}개 땄다!$"

BerryTree_Text_PutAwayBerry:
	.string "{PLAYER}{K_EUNNEUN} {STR_VAR_1}{K_EULREUL}\n"
	.string "가방에 넣었다!\p"
	.string "푹신푹신한 흙으로 돌아왔다.$"

BerryTree_Text_BerryPocketFull:
	.string "가방이 가득 찼다.\p"
	.string "{STR_VAR_1}{K_EULREUL} 포기했다.$"

BerryTree_Text_BerryLeftUnpicked:
	.string "{PLAYER}{K_EUNNEUN} {STR_VAR_1}{K_EULREUL}\n"
	.string "따지 않고 그대로 놔두었다.$"

BerryTree_Text_WantToWater:
	.string "{STR_VAR_1}에 고래왕자물뿌리개로\n"
	.string "물을 주겠습니까?$"

BerryTree_Text_WateredTheBerry:
	.string "{PLAYER}{K_EUNNEUN} {STR_VAR_1}에\n"
	.string "물을 줬다!$"

BerryTree_Text_PlantIsDelighted:
	.string "식물이 기뻐하는 것 같다.$"

BerryTree_Text_ExclamationPoint:
	.string "!$"

.if OW_BERRY_MUTATIONS == TRUE
BerryTree_EventScript_CheckBerryFullyGrown_Mutation:
	msgbox BerryTree_Text_WantToPick_Mutation, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, BerryTree_EventScript_PickBerry
	goto_if_eq VAR_RESULT, NO, BerryTree_EventScript_CancelPickingBerry_Mutation

BerryTree_EventScript_CancelPickingBerry_Mutation::
	message BerryTree_Text_BerryLeftUnpicked_Mutation
	waitmessage
	waitbuttonpress
	release
	end

BerryTree_EventScript_BerryPocketFull_Mutation::
	message BerryTree_Text_BerryPocketFull_Mutation
	waitmessage
	waitbuttonpress
	release
	end

BerryTree_EventScript_PickBerry_Mutation::
	special IncrementDailyPickedBerries
	special ObjectEventInteractionRemoveBerryTree
	message BerryTree_Text_PickedTheBerry_Mutation
	playfanfare MUS_OBTAIN_BERRY
	waitmessage
	waitfanfare
	waitbuttonpress
	message BerryTree_Text_PutAwayBerry_Mutation
	waitmessage
	waitbuttonpress
	release
	end

BerryTree_Text_WantToPick_Mutation:
	.string "You found {STR_VAR_2} {STR_VAR_1}\n"
	.string "and 1 {STR_VAR_3}!\p"
	.string "Do you want to pick them?$"

BerryTree_Text_BerryLeftUnpicked_Mutation:
	.string "{PLAYER} left the {STR_VAR_1}\n"
	.string "and the {STR_VAR_3} unpicked.$"

BerryTree_Text_BerryPocketFull_Mutation:
	.string "The BAG's BERRIES POCKET is full.\p"
	.string "The {STR_VAR_1} and the\n"
	.string "{STR_VAR_3} couldn't be taken.$"

BerryTree_Text_PickedTheBerry_Mutation:
	.string "{PLAYER} picked the {STR_VAR_2} {STR_VAR_1}\n"
	.string "and the {STR_VAR_3}.$"

BerryTree_Text_PutAwayBerry_Mutation:
	.string "{PLAYER} put away the {STR_VAR_1}\n"
	.string "and the {STR_VAR_3} in the BAG's\l"
	.string "BERRIES POCKET.\p"
	.string "The soil returned to its soft and\n"
	.string "loamy state.$"
.endif

.if OW_BERRY_MULCH_USAGE == TRUE
BerryTree_EventScript_UseMulch::
	special ObjectEventInteractionApplyMulch
	message BerryTree_Text_ScatteredMulch
	waitmessage
	waitbuttonpress
	return

BerryTree_Text_WantToMulch:
	.string "푹신푹신한 흙이다.\n"
	.string "비료를 뿌리겠습니까?$"

BerryTree_Text_ScatteredMulch:
	.string "{PLAYER}{K_EUNNEUN} 푹신푹신한 흙에\n"
	.string "{STR_VAR_1}{K_EULREUL} 뿌렸다!$"
.endif

.if OW_BERRY_WEEDS == TRUE
BerryTree_EventScript_CheckForWeed::
	specialvar VAR_RESULT, ObjectEventInteractionBerryHasWeed
	call_if_eq VAR_RESULT, TRUE, BerryTree_EventScript_WeedIsGrowing
	return

BerryTree_EventScript_WeedIsGrowing::
	msgbox BerryTree_Text_WeedIsGrowing, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, BerryTree_EventScript_PullOutWeed
	return

BerryTree_EventScript_PullOutWeed::
	special ObjectEventInteractionPullBerryWeed
	message BerryTree_Text_PulledOutTheWeed
	waitmessage
	waitbuttonpress
	return

BerryTree_Text_WeedIsGrowing:
	.string "잡초가 자라고 있다.\n"
	.string "뽑겠습니까?$"

BerryTree_Text_PulledOutTheWeed:
	.string "{PLAYER}{K_EUNNEUN} 잡초를 뽑았다!$"
.endif

.if OW_BERRY_PESTS == TRUE
BerryTree_EventScript_CheckForPests::
	specialvar VAR_RESULT, ObjectEventInteractionBerryHasPests
	call_if_eq VAR_RESULT, TRUE, BerryTree_EventScript_EncounterPests
	return

BerryTree_EventScript_EncounterPests::
	message BerryTree_Text_APokemonAppeared
	waitmessage
	waitbuttonpress
	dowildbattle
	return

BerryTree_Text_APokemonAppeared:
	.string "야생 포켓몬이 나타났다!$"
.endif

#endif
