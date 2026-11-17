BattleRoyale_TowerLobby_MapScripts::
	.byte 0

BattleRoyale_TowerLobby_EventScript_Attendant::
	lock
	faceplayer
	msgbox BattleRoyale_TowerLobby_Text_Welcome, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleRoyale_TowerLobby_EventScript_Cancel
	special HealPlayerParty
	setvar VAR_0x8004, 1
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
	.string "Welcome to the BATTLE TOWER!\p"
	.string "TIER 1 is five battles in a row.\n"
	.string "Your POKéMON are healed between\l"
	.string "battles.\p"
	.string "Would you like to take on TIER 1?$"

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
	.string "keep your POKéMON and your winnings.$"
