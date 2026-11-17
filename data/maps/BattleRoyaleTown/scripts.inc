BattleRoyaleTown_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, BattleRoyaleTown_OnFrame
	.byte 0

BattleRoyaleTown_OnFrame:
	map_script_2 VAR_BR_GAME_OVER, 1, BattleRoyaleTown_EventScript_GameOver
	map_script_2 VAR_LITTLEROOT_INTRO_STATE, 0, BattleRoyaleTown_EventScript_Welcome
	.2byte 0

BattleRoyaleTown_EventScript_Welcome::
	lockall
	playfanfare MUS_OBTAIN_ITEM
	message BattleRoyaleTown_Text_Welcome
	waitmessage
	waitfanfare
	waitbuttonpress
	setvar VAR_LITTLEROOT_INTRO_STATE, 1
	releaseall
	end

@ Blacked out in the grass with no money (see BattleRoyale_OnWhiteOut)
BattleRoyaleTown_EventScript_GameOver::
	lockall
	msgbox BattleRoyale_Text_GameOver, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	special BattleRoyale_GameOver
	end

BattleRoyaleTown_EventScript_TowerSign::
	msgbox BattleRoyaleTown_Text_TowerSign, MSGBOX_SIGN
	end

BattleRoyaleTown_Text_Welcome:
	.string "Welcome to {JPN}バトル·ロワイアル{ENG},\n"
	.string "{PLAYER}!\p"
	.string "You received three POKéMON!\n"
	.string "Win TOWER battles to earn money.$"

BattleRoyaleTown_Text_TowerSign:
	.string "BATTLE TOWER\n"
	.string "Opening soon!$"
