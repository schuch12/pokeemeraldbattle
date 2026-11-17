BattleFrontier_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, BattleFrontier_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

BattleFrontier_PokemonCenter_1F_OnTransition:
	setrespawn HEAL_LOCATION_BATTLE_FRONTIER_OUTSIDE_EAST
	end

BattleFrontier_PokemonCenter_1F_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_FRONTIER_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

BattleFrontier_PokemonCenter_1F_EventScript_SchoolKid::
	msgbox BattleFrontier_PokemonCenter_1F_Text_NeverSeenPokemon, MSGBOX_NPC
	end

BattleFrontier_PokemonCenter_1F_EventScript_Man::
	msgbox BattleFrontier_PokemonCenter_1F_Text_NextStopBattleArena, MSGBOX_NPC
	end

BattleFrontier_PokemonCenter_1F_EventScript_Picnicker::
	msgbox BattleFrontier_PokemonCenter_1F_Text_GoingThroughEveryChallenge, MSGBOX_NPC
	end

BattleFrontier_PokemonCenter_1F_EventScript_Skitty::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_SKITTY, CRY_MODE_NORMAL
	msgbox BattleFrontier_PokemonCenter_1F_Text_Skitty, MSGBOX_DEFAULT
	waitmoncry
	msgbox BattleRoyale_Cheats_Text_Intro, MSGBOX_DEFAULT
	goto BattleRoyale_Cheats_EventScript_Menu

@ ---- Battle Royale: cheat menu ----
@ Switches are flags in include/constants/battle_royale.h; menu text is
@ MultichoiceList_BRCheats in src/data/script_menu.h.
BattleRoyale_Cheats_EventScript_Menu::
	closemessage
	multichoice 0, 0, MULTI_BR_CHEATS, FALSE
	switch VAR_RESULT
	case 0, BattleRoyale_Cheats_EventScript_NoCap
	case 1, BattleRoyale_Cheats_EventScript_AllTiers
	case 2, BattleRoyale_Cheats_EventScript_FastExp
	case 3, BattleRoyale_Cheats_EventScript_Money
	case 4, BattleRoyale_Cheats_EventScript_Candy
	case 5, BattleRoyale_Cheats_EventScript_Heal
	release
	end

BattleRoyale_Cheats_EventScript_NoCap::
	goto_if_set FLAG_BR_CHEAT_NO_LEVEL_CAP, BattleRoyale_Cheats_EventScript_NoCapOff
	setflag FLAG_BR_CHEAT_NO_LEVEL_CAP
	msgbox BattleRoyale_Cheats_Text_NoCapOn, MSGBOX_DEFAULT
	goto BattleRoyale_Cheats_EventScript_Menu

BattleRoyale_Cheats_EventScript_NoCapOff::
	clearflag FLAG_BR_CHEAT_NO_LEVEL_CAP
	msgbox BattleRoyale_Cheats_Text_NoCapOff, MSGBOX_DEFAULT
	goto BattleRoyale_Cheats_EventScript_Menu

BattleRoyale_Cheats_EventScript_AllTiers::
	goto_if_set FLAG_BR_CHEAT_ALL_TIERS, BattleRoyale_Cheats_EventScript_AllTiersOff
	setflag FLAG_BR_CHEAT_ALL_TIERS
	msgbox BattleRoyale_Cheats_Text_AllTiersOn, MSGBOX_DEFAULT
	goto BattleRoyale_Cheats_EventScript_Menu

BattleRoyale_Cheats_EventScript_AllTiersOff::
	clearflag FLAG_BR_CHEAT_ALL_TIERS
	msgbox BattleRoyale_Cheats_Text_AllTiersOff, MSGBOX_DEFAULT
	goto BattleRoyale_Cheats_EventScript_Menu

BattleRoyale_Cheats_EventScript_FastExp::
	goto_if_set FLAG_BR_CHEAT_FAST_EXP, BattleRoyale_Cheats_EventScript_FastExpOff
	setflag FLAG_BR_CHEAT_FAST_EXP
	msgbox BattleRoyale_Cheats_Text_FastExpOn, MSGBOX_DEFAULT
	goto BattleRoyale_Cheats_EventScript_Menu

BattleRoyale_Cheats_EventScript_FastExpOff::
	clearflag FLAG_BR_CHEAT_FAST_EXP
	msgbox BattleRoyale_Cheats_Text_FastExpOff, MSGBOX_DEFAULT
	goto BattleRoyale_Cheats_EventScript_Menu

BattleRoyale_Cheats_EventScript_Money::
	addmoney 100000
	playse SE_SHOP
	msgbox BattleRoyale_Cheats_Text_Money, MSGBOX_DEFAULT
	goto BattleRoyale_Cheats_EventScript_Menu

BattleRoyale_Cheats_EventScript_Candy::
	giveitem ITEM_RARE_CANDY, 10
	goto BattleRoyale_Cheats_EventScript_Menu

BattleRoyale_Cheats_EventScript_Heal::
	special HealPlayerParty
	playfanfare MUS_HEAL
	waitfanfare
	msgbox BattleRoyale_Cheats_Text_Heal, MSGBOX_DEFAULT
	goto BattleRoyale_Cheats_EventScript_Menu

BattleRoyale_Cheats_Text_Intro:
	.string "…SKITTY seems to know a few secrets.\n"
	.string "(CHEAT MENU)$"

BattleRoyale_Cheats_Text_NoCapOn:
	.string "Level limit is now OFF.\n"
	.string "Full EXP. at any level.$"

BattleRoyale_Cheats_Text_NoCapOff:
	.string "Level limit is back ON.$"

BattleRoyale_Cheats_Text_AllTiersOn:
	.string "All TIERS are now open.$"

BattleRoyale_Cheats_Text_AllTiersOff:
	.string "TIERS must be cleared in order again.$"

BattleRoyale_Cheats_Text_FastExpOn:
	.string "EXP. ×4 is now ON.$"

BattleRoyale_Cheats_Text_FastExpOff:
	.string "EXP. ×4 is now OFF.$"

BattleRoyale_Cheats_Text_Money:
	.string "{PLAYER} received ¥100000!$"

BattleRoyale_Cheats_Text_Heal:
	.string "Your POKéMON were healed.$"

BattleFrontier_PokemonCenter_1F_Text_NeverSeenPokemon:
	.string "So, today's lesson is…\p"
	.string "You face each other until there's\n"
	.string "only one left.\p"
	.string "Nothing is against the rules.$"

BattleFrontier_PokemonCenter_1F_Text_NextStopBattleArena:
	.string "There's a way out of this game.\n"
	.string "Face yourselves together, here, now.\p"
	.string "If you can't do that, then don't\n"
	.string "trust anyone… Just run.$"

BattleFrontier_PokemonCenter_1F_Text_GoingThroughEveryChallenge:
	.string "If I survive, can I go home?$"

BattleFrontier_PokemonCenter_1F_Text_Skitty:
	.string "SKITTY: Mya myaaah!$"

