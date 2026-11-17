BattleRoyale_TowerLobby_MapScripts::
	.byte 0

BattleRoyale_TowerLobby_EventScript_Attendant::
	lock
	faceplayer
	message BattleRoyale_TowerLobby_Text_Welcome
	waitmessage
	multichoice 20, 4, MULTI_BR_TIERS, FALSE
	switch VAR_RESULT
	case 0, BattleRoyale_TowerLobby_EventScript_Tier1
	case 1, BattleRoyale_TowerLobby_EventScript_Tier2
	goto BattleRoyale_TowerLobby_EventScript_Cancel

BattleRoyale_TowerLobby_EventScript_Tier1::
	setvar VAR_0x8004, 1
	msgbox BattleRoyale_TowerLobby_Text_Tier1Info, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleRoyale_TowerLobby_EventScript_Cancel
	goto BattleRoyale_TowerLobby_EventScript_StartRun

BattleRoyale_TowerLobby_EventScript_Tier2::
	goto_if_lt VAR_BR_TIERS_CLEARED, 1, BattleRoyale_TowerLobby_EventScript_Tier2Locked
	setvar VAR_0x8004, 2
	msgbox BattleRoyale_TowerLobby_Text_Tier2Info, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleRoyale_TowerLobby_EventScript_Cancel
	goto BattleRoyale_TowerLobby_EventScript_StartRun

BattleRoyale_TowerLobby_EventScript_Tier2Locked::
	msgbox BattleRoyale_TowerLobby_Text_Tier2Locked, MSGBOX_DEFAULT
	release
	end

BattleRoyale_TowerLobby_EventScript_StartRun::
	special HealPlayerParty
	special BattleRoyale_StartRun
	msgbox BattleRoyale_TowerLobby_Text_RightThisWay, MSGBOX_DEFAULT
	closemessage
	warp MAP_BATTLE_ROYALE_TOWER_BATTLE_ROOM, 4, 8
	waitstate
	end

BattleRoyale_TowerLobby_EventScript_Cancel::
	msgbox BattleRoyale_TowerLobby_Text_ComeBack, MSGBOX_DEFAULT
	release
	end

BattleRoyale_TowerLobby_EventScript_RulesBoard::
	msgbox BattleRoyale_TowerLobby_Text_Rules, MSGBOX_SIGN
	end

BattleRoyale_TowerLobby_Text_Welcome:
	.string "Welcome to the BATTLE TOWER!\n"
	.string "Which TIER would you like to enter?$"

BattleRoyale_TowerLobby_Text_Tier1Info:
	.string "TIER 1 is five battles in a row\n"
	.string "against POKéMON up to level 8.\p"
	.string "Your POKéMON are healed between\n"
	.string "battles. Enter TIER 1?$"

BattleRoyale_TowerLobby_Text_Tier2Info:
	.string "TIER 2 is five battles in a row\n"
	.string "against POKéMON up to level 23.\p"
	.string "Your POKéMON are healed between\n"
	.string "battles. Enter TIER 2?$"

BattleRoyale_TowerLobby_Text_Tier2Locked:
	.string "I'm sorry. You must clear TIER 1\n"
	.string "before you can enter TIER 2.$"

BattleRoyale_TowerLobby_Text_RightThisWay:
	.string "Good luck! Right this way.$"

BattleRoyale_TowerLobby_Text_ComeBack:
	.string "Come back when you're ready.$"

BattleRoyale_TowerLobby_Text_Rules:
	.string "BATTLE TOWER RULES\p"
	.string "Win five battles in a row to clear\n"
	.string "a TIER. Every win pays prize money,\l"
	.string "and clearing a TIER pays a bonus.\p"
	.string "If you lose, the run ends, but you\n"
	.string "keep your POKéMON and your winnings.\p"
	.string "POKéMON at level 15 or higher gain\n"
	.string "very little EXP. until you clear\l"
	.string "TIER 1. After that, the limit is\l"
	.string "level 25.$"
