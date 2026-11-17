.set LOCALID_BR_OPPONENT, 1
.set LOCALID_BR_ATTENDANT, 2

BattleRoyale_TowerBattleRoom_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, BattleRoyale_TowerBattleRoom_OnFrame
	.byte 0

BattleRoyale_TowerBattleRoom_OnFrame:
	map_script_2 VAR_TEMP_0, 0, BattleRoyale_TowerBattleRoom_EventScript_EnterRoom
	.2byte 0

BattleRoyale_TowerBattleRoom_EventScript_EnterRoom::
	lockall
	setvar VAR_TEMP_0, 1
	applymovement LOCALID_PLAYER, BattleFrontier_BattleTowerBattleRoom_Movement_PlayerEnter
	waitmovement 0
BattleRoyale_TowerBattleRoom_EventScript_NextOpponent::
	special BattleRoyale_SetUpOpponent
	addobject LOCALID_BR_OPPONENT
	applymovement LOCALID_BR_OPPONENT, BattleFrontier_BattleTowerBattleRoom_Movement_OpponentEnter
	waitmovement 0
	msgbox gStringVar4, MSGBOX_DEFAULT
	closemessage
	special BattleRoyale_StartTowerBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, BattleRoyale_TowerBattleRoom_EventScript_Won
	goto BattleRoyale_TowerBattleRoom_EventScript_Lost

BattleRoyale_TowerBattleRoom_EventScript_Won::
	applymovement LOCALID_BR_OPPONENT, BattleFrontier_BattleTowerBattleRoom_Movement_OpponentExit
	waitmovement 0
	removeobject LOCALID_BR_OPPONENT
	applymovement LOCALID_BR_ATTENDANT, BattleFrontier_BattleTowerBattleRoom_Movement_AttendantApproachPlayer
	waitmovement 0
	applymovement LOCALID_PLAYER, BattleFrontier_BattleTowerBattleRoom_Movement_PlayerFaceAttendant
	waitmovement 0
	special BattleRoyale_OnWin
	goto_if_eq VAR_RESULT, TRUE, BattleRoyale_TowerBattleRoom_EventScript_TierCleared
	msgbox BattleRoyale_TowerBattleRoom_Text_HealMons, MSGBOX_DEFAULT
	playfanfare MUS_HEAL
	waitfanfare
	special HealPlayerParty
BattleRoyale_TowerBattleRoom_EventScript_AskNext::
	message BattleRoyale_TowerBattleRoom_Text_ReadyForNext
	waitmessage
	multichoice 18, 6, MULTI_BR_BETWEEN_BATTLES, TRUE
	switch VAR_RESULT
	case 1, BattleRoyale_TowerBattleRoom_EventScript_SaveAndQuit
	case 2, BattleRoyale_TowerBattleRoom_EventScript_AskRetire
BattleRoyale_TowerBattleRoom_EventScript_Continue::
	closemessage
	applymovement LOCALID_PLAYER, BattleFrontier_BattleTowerBattleRoom_Movement_PlayerFaceBattle
	waitmovement 0
	applymovement LOCALID_BR_ATTENDANT, BattleFrontier_BattleTowerBattleRoom_Movement_AttendantReturnToPos
	waitmovement 0
	goto BattleRoyale_TowerBattleRoom_EventScript_NextOpponent

BattleRoyale_TowerBattleRoom_EventScript_AskRetire::
	msgbox BattleRoyale_TowerBattleRoom_Text_ReallyRetire, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleRoyale_TowerBattleRoom_EventScript_AskNext
	special BattleRoyale_Retire
	msgbox BattleRoyale_TowerBattleRoom_Text_Retired, MSGBOX_DEFAULT
	goto BattleRoyale_TowerBattleRoom_EventScript_WarpToLobby

BattleRoyale_TowerBattleRoom_EventScript_SaveAndQuit::
	msgbox BattleRoyale_TowerBattleRoom_Text_PauseRun, MSGBOX_DEFAULT
	setvar VAR_BR_PAUSED, 1
	goto BattleRoyale_TowerBattleRoom_EventScript_WarpToLobby

BattleRoyale_TowerBattleRoom_EventScript_TierCleared::
	playfanfare MUS_OBTAIN_BADGE
	message BattleRoyale_TowerBattleRoom_Text_TierCleared
	waitmessage
	waitfanfare
	waitbuttonpress
	special HealPlayerParty
	goto BattleRoyale_TowerBattleRoom_EventScript_WarpToLobby

BattleRoyale_TowerBattleRoom_EventScript_Lost::
	special HealPlayerParty
	special BattleRoyale_OnLoss
	applymovement LOCALID_BR_ATTENDANT, BattleFrontier_BattleTowerBattleRoom_Movement_AttendantApproachPlayer
	waitmovement 0
	applymovement LOCALID_PLAYER, BattleFrontier_BattleTowerBattleRoom_Movement_PlayerFaceAttendant
	waitmovement 0
	msgbox BattleRoyale_TowerBattleRoom_Text_Lost, MSGBOX_DEFAULT
	goto BattleRoyale_TowerBattleRoom_EventScript_WarpToLobby

BattleRoyale_TowerBattleRoom_EventScript_WarpToLobby::
	closemessage
	warp MAP_BATTLE_ROYALE_TOWER_LOBBY, 14, 6
	waitstate
	end

BattleRoyale_TowerBattleRoom_Text_HealMons:
	.string "Well done! I'll restore your POKéMON\n"
	.string "to full health.$"

BattleRoyale_TowerBattleRoom_Text_ReadyForNext:
	.string "Battle no. {STR_VAR_1} is next.\n"
	.string "What would you like to do?$"

BattleRoyale_TowerBattleRoom_Text_PauseRun:
	.string "I'll hold your place in this TIER.\n"
	.string "Let's go back to the lobby.$"

BattleRoyale_TowerBattleRoom_Text_ReallyRetire:
	.string "Do you want to stop here?\n"
	.string "You keep the prize money you've won,\l"
	.string "but this TIER run will end.$"

BattleRoyale_TowerBattleRoom_Text_Retired:
	.string "Thank you for playing.\n"
	.string "Please come again!$"

BattleRoyale_TowerBattleRoom_Text_TierCleared:
	.string "Congratulations!\n"
	.string "You cleared TIER {STR_VAR_2}!\p"
	.string "{PLAYER} received a bonus of\n"
	.string "¥{STR_VAR_1}!$"

BattleRoyale_TowerBattleRoom_Text_Lost:
	.string "That was a tough one.\n"
	.string "Your POKéMON have been healed.\p"
	.string "{PLAYER} received ¥{STR_VAR_1}\n"
	.string "for taking part.\p"
	.string "Please try again any time!$"

BattleRoyale_Text_Intro0::
	.string "I've been waiting for a challenger!\n"
	.string "Let's go!$"

BattleRoyale_Text_Intro1::
	.string "I trained all week for this.\n"
	.string "Don't hold back!$"

BattleRoyale_Text_Intro2::
	.string "You look strong…\n"
	.string "But my POKéMON are stronger!$"

BattleRoyale_Text_Intro3::
	.string "The prize money is going to be mine!$"

BattleRoyale_Text_Intro4::
	.string "A new face? I'll show you how\n"
	.string "things work in the TOWER!$"

BattleRoyale_Text_Intro5::
	.string "My heart is pounding…\n"
	.string "Here I come!$"

BattleRoyale_Text_Defeat0::
	.string "Wow, you're good!$"

BattleRoyale_Text_Defeat1::
	.string "All that training…$"

BattleRoyale_Text_Defeat2::
	.string "I guess you were stronger after all.$"

BattleRoyale_Text_Defeat3::
	.string "There goes my prize money…$"

BattleRoyale_Text_Defeat4::
	.string "So that's how it is!$"

BattleRoyale_Text_Defeat5::
	.string "That was over so fast!$"
