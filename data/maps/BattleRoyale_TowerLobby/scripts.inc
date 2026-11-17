BattleRoyale_TowerLobby_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, BattleRoyale_TowerLobby_OnFrame
	.byte 0

BattleRoyale_TowerLobby_OnFrame:
	map_script_2 VAR_BR_PAUSED, 1, BattleRoyale_TowerLobby_EventScript_OfferSave
	.2byte 0

@ Runs when the player comes back from the battle room after choosing SAVE AND QUIT
BattleRoyale_TowerLobby_EventScript_OfferSave::
	lockall
	setvar VAR_BR_PAUSED, 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox BattleRoyale_TowerLobby_Text_RunPaused, MSGBOX_DEFAULT
	call Common_EventScript_SaveGame
	msgbox BattleRoyale_TowerLobby_Text_TalkToContinue, MSGBOX_DEFAULT
	releaseall
	end

BattleRoyale_TowerLobby_EventScript_Attendant::
	lock
	faceplayer
	goto_if_ne VAR_BR_TIER, 0, BattleRoyale_TowerLobby_EventScript_RunInProgress
	message BattleRoyale_TowerLobby_Text_Welcome
	waitmessage
	setvar VAR_0x8004, SCROLL_MULTI_BR_TIERS
	special ShowScrollableMultichoice
	waitstate
	goto_if_ge VAR_RESULT, BR_NUM_TIERS, BattleRoyale_TowerLobby_EventScript_Cancel
	copyvar VAR_TEMP_1, VAR_RESULT
	addvar VAR_TEMP_1, 1
	@ VAR_TEMP_1 = chosen tier number
	special BattleRoyale_GetHighestUnlockedTier
	goto_if_gt VAR_TEMP_1, VAR_RESULT, BattleRoyale_TowerLobby_EventScript_TierLocked
	copyvar VAR_0x8004, VAR_TEMP_1
	special BattleRoyale_CheckPartyAllowed
	goto_if_eq VAR_RESULT, FALSE, BattleRoyale_TowerLobby_EventScript_BannedMon
	special BattleRoyale_BufferTierInfo
	call_if_eq VAR_RESULT, FALSE, BattleRoyale_TowerLobby_EventScript_ShowTierInfo
	call_if_eq VAR_RESULT, TRUE, BattleRoyale_TowerLobby_EventScript_ShowTierInfoFee
	msgbox BattleRoyale_TowerLobby_Text_EnterTier, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleRoyale_TowerLobby_EventScript_Cancel
	special HealPlayerParty
	copyvar VAR_0x8004, VAR_TEMP_1
	special BattleRoyale_StartRun
	goto BattleRoyale_TowerLobby_EventScript_EnterBattleRoom

BattleRoyale_TowerLobby_EventScript_ShowTierInfo::
	msgbox BattleRoyale_TowerLobby_Text_TierInfo, MSGBOX_DEFAULT
	return

BattleRoyale_TowerLobby_EventScript_ShowTierInfoFee::
	msgbox BattleRoyale_TowerLobby_Text_TierInfoFee, MSGBOX_DEFAULT
	return

BattleRoyale_TowerLobby_EventScript_TierLocked::
	msgbox BattleRoyale_TowerLobby_Text_TierLocked, MSGBOX_DEFAULT
	release
	end

BattleRoyale_TowerLobby_EventScript_BannedMon::
	msgbox BattleRoyale_TowerLobby_Text_BannedMon, MSGBOX_DEFAULT
	release
	end

BattleRoyale_TowerLobby_EventScript_RunInProgress::
	special BattleRoyale_BufferRunInfo
	msgbox BattleRoyale_TowerLobby_Text_ContinueRun, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, BattleRoyale_TowerLobby_EventScript_ResumeRun
	msgbox BattleRoyale_TowerLobby_Text_AbandonRun, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleRoyale_TowerLobby_EventScript_Cancel
	special BattleRoyale_Retire
	msgbox BattleRoyale_TowerLobby_Text_RunAbandoned, MSGBOX_DEFAULT
	release
	end

BattleRoyale_TowerLobby_EventScript_ResumeRun::
	special BattleRoyale_BufferRunInfo
	special BattleRoyale_CheckPartyAllowed
	goto_if_eq VAR_RESULT, FALSE, BattleRoyale_TowerLobby_EventScript_BannedMon
	special HealPlayerParty
	goto BattleRoyale_TowerLobby_EventScript_EnterBattleRoom

BattleRoyale_TowerLobby_EventScript_EnterBattleRoom::
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

BattleRoyale_TowerLobby_Text_TierInfo:
	.string "{STR_VAR_1}:\n"
	.string "Five battles in a row against\l"
	.string "POKéMON up to level {STR_VAR_2}.\p"
	.string "Your POKéMON are healed between\n"
	.string "battles.$"

BattleRoyale_TowerLobby_Text_TierInfoFee:
	.string "{STR_VAR_1}:\n"
	.string "Five battles in a row against\l"
	.string "POKéMON up to level {STR_VAR_2}.\p"
	.string "Your POKéMON are healed between\n"
	.string "battles.\p"
	.string "Be warned: if you lose, you must pay\n"
	.string "a fee of ¥{STR_VAR_3}.$"

BattleRoyale_TowerLobby_Text_EnterTier:
	.string "Would you like to enter?$"

BattleRoyale_TowerLobby_Text_TierLocked:
	.string "I'm sorry. You must clear the TIERS\n"
	.string "in order. That one isn't open yet.$"

BattleRoyale_TowerLobby_Text_BannedMon:
	.string "I'm sorry. {STR_VAR_1} is too rare and\n"
	.string "powerful for this TIER.\p"
	.string "Legendary POKéMON may only enter\n"
	.string "the CHAMPION TIER.$"

BattleRoyale_TowerLobby_Text_RunPaused:
	.string "Your TIER run is on hold.\n"
	.string "You can save and stop here.$"

BattleRoyale_TowerLobby_Text_TalkToContinue:
	.string "Talk to me when you want to\n"
	.string "carry on with your run.$"

BattleRoyale_TowerLobby_Text_ContinueRun:
	.string "Welcome back! Your run is on hold:\n"
	.string "{STR_VAR_1}, battle no. {STR_VAR_2}.\p"
	.string "Would you like to continue it?$"

BattleRoyale_TowerLobby_Text_AbandonRun:
	.string "Do you want to give up this run?\n"
	.string "You keep the prize money you've won.$"

BattleRoyale_TowerLobby_Text_RunAbandoned:
	.string "The run has ended.\n"
	.string "You may enter a TIER again any time.$"

BattleRoyale_TowerLobby_Text_RightThisWay:
	.string "Good luck! Right this way.$"

BattleRoyale_TowerLobby_Text_ComeBack:
	.string "Come back when you're ready.$"

BattleRoyale_TowerLobby_Text_Rules:
	.string "BATTLE TOWER RULES\p"
	.string "Win five battles in a row to clear\n"
	.string "a TIER. Every win pays prize money,\l"
	.string "and clearing a TIER pays a bonus.\p"
	.string "There are eight TIERS, and then the\n"
	.string "CHAMPION TIER. Clear them in order.\p"
	.string "If you lose, the run ends but you\n"
	.string "keep your POKéMON. From TIER 3 up,\l"
	.string "losing also costs a fee.\p"
	.string "If your whole team faints and you\n"
	.string "have no money at all, it's GAME OVER.\p"
	.string "Each TIER has a level limit. POKéMON\n"
	.string "at the limit gain very little EXP.,\l"
	.string "and traded POKéMON above it may\l"
	.string "disobey. Clear the TIER to raise it.\p"
	.string "Legendary POKéMON may only enter\n"
	.string "the CHAMPION TIER.$"

BattleRoyale_Text_TierName::
	.string "TIER $"

BattleRoyale_Text_ChampionTierName::
	.string "CHAMPION TIER$"
