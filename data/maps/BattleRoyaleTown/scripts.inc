BattleRoyaleTown_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, BattleRoyaleTown_OnFrame
	.byte 0

BattleRoyaleTown_OnFrame:
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
