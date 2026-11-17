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
	release
	end

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

