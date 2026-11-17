BattleFrontier_Lounge5_MapScripts::
	.byte 0

BattleFrontier_Lounge5_EventScript_NatureGirl::
	lock
	faceplayer
	msgbox BattleFrontier_Lounge5_Text_NatureGirlGreeting, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_Lounge5_EventScript_NatureGirlNoneShown
	special ChoosePartyMon
	lock
	faceplayer
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, BattleFrontier_Lounge5_EventScript_NatureGirlNoneShown
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, BattleFrontier_Lounge5_EventScript_NatureGirlEgg
	special ShowNatureGirlMessage
	waitmessage
	waitbuttonpress
	release
	end

BattleFrontier_Lounge5_EventScript_NatureGirlEgg::
	msgbox BattleFrontier_Lounge5_Text_NatureGirlEgg, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge5_EventScript_NatureGirlNoneShown::
	msgbox BattleFrontier_Lounge5_Text_NatureGirlNoneShown, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge5_EventScript_Gentleman::
	msgbox BattleFrontier_Lounge5_Text_LadyClaimsSheUnderstandsPokemon, MSGBOX_NPC
	end

BattleFrontier_Lounge5_EventScript_BlackBelt::
	msgbox BattleFrontier_Lounge5_Text_GirlSayingSomethingProfound, MSGBOX_NPC
	end

BattleFrontier_Lounge5_EventScript_LittleBoy::
	msgbox BattleFrontier_Lounge5_Text_GirlPlaysAtRedHouseALot, MSGBOX_NPC
	end

BattleFrontier_Lounge5_Text_NatureGirlGreeting::
	.string "Ehehe!\n"
	.string "I can tell what POKéMON are thinking!\p"
	.string "Please!\n"
	.string "Can I see your POKéMON?$"

BattleFrontier_Lounge5_Text_NatureGirlNoneShown::
	.string "Boo!\n"
	.string "Cheapie!$"

BattleFrontier_Lounge5_Text_NatureGirlHardy::
	.string "Hmhm…\p"
	.string "This one says it likes to battle!\n"
	.string "It will battle even if it has a lot\l"
	.string "of ouchies!$"

BattleFrontier_Lounge5_Text_NatureGirlLonely::
	.string "Hmhm…\p"
	.string "This one says it likes to be sneaky!\n"
	.string "But if it gets enough ouchies,\l"
	.string "it will hit back!$"

BattleFrontier_Lounge5_Text_NatureGirlBrave::
	.string "Hmhm…\p"
	.string "This one says it likes to battle!\n"
	.string "But if it gets enough ouchies,\l"
	.string "it will worry about itself!$"

BattleFrontier_Lounge5_Text_NatureGirlAdamant::
	.string "Hmhm…\p"
	.string "This one says it likes to battle!\n"
	.string "It will battle even if it has a lot\l"
	.string "of ouchies!$"

BattleFrontier_Lounge5_Text_NatureGirlNaughty::
	.string "Hmhm…\p"
	.string "This one says it looks after itself!\n"
	.string "But if it gets enough ouchies,\l"
	.string "it will hit back!$"

BattleFrontier_Lounge5_Text_NatureGirlBold::
	.string "Hmhm…\p"
	.string "This one says it likes to be sneaky!\n"
	.string "But if it gets enough ouchies,\l"
	.string "it will worry about itself!$"

BattleFrontier_Lounge5_Text_NatureGirlDocileNaiveQuietQuirky::
	.string "Hmhm…\p"
	.string "This one says it likes to battle!\n"
	.string "It will battle even if it has a lot\l"
	.string "of ouchies!$"

BattleFrontier_Lounge5_Text_NatureGirlRelaxed::
	.string "Hmhm…\p"
	.string "This one says it likes to be sneaky!\n"
	.string "But if it gets enough ouchies,\l"
	.string "it will hit back!$"

BattleFrontier_Lounge5_Text_NatureGirlImpish::
	.string "Hmhm…\p"
	.string "This one says it likes to battle!\n"
	.string "But if it gets enough ouchies,\l"
	.string "it will worry about itself!$"

BattleFrontier_Lounge5_Text_NatureGirlLax::
	.string "Hmhm…\p"
	.string "This one says it likes to be sneaky!\n"
	.string "It says it likes to be sneaky even\l"
	.string "if it has a lot of ouchies!$"

BattleFrontier_Lounge5_Text_NatureGirlTimid::
	.string "Hmhm…\p"
	.string "This one says it likes to battle!\n"
	.string "But if it gets enough ouchies,\l"
	.string "it will turn sneaky!$"

BattleFrontier_Lounge5_Text_NatureGirlHasty::
	.string "Hmhm…\p"
	.string "This one says it likes to battle!\n"
	.string "It will battle even if it has a lot\l"
	.string "of ouchies!$"

BattleFrontier_Lounge5_Text_NatureGirlSerious::
	.string "Hmhm…\p"
	.string "This one says it likes to be sneaky!\n"
	.string "It says it likes to be sneaky even\l"
	.string "if it has a lot of ouchies!$"

BattleFrontier_Lounge5_Text_NatureGirlJolly::
	.string "Hmhm…\p"
	.string "This one says it likes to be sneaky!\n"
	.string "But if it gets enough ouchies,\l"
	.string "it will worry about itself!$"

BattleFrontier_Lounge5_Text_NatureGirlModest::
	.string "Hmhm…\p"
	.string "This one says it looks after itself!\n"
	.string "It says it worries about itself whether\l"
	.string "or not it has a lot of ouchies!$"

BattleFrontier_Lounge5_Text_NatureGirlMild::
	.string "Hmhm…\p"
	.string "This one says it looks after itself!\n"
	.string "But if it gets enough ouchies,\l"
	.string "it will turn sneaky!$"

BattleFrontier_Lounge5_Text_NatureGirlBashful::
	.string "Hmhm…\p"
	.string "This one says it looks after itself!\n"
	.string "It says it worries about itself even\l"
	.string "if it has a lot of ouchies!$"

BattleFrontier_Lounge5_Text_NatureGirlRash::
	.string "Hmhm…\p"
	.string "This one says it likes to be sneaky!\n"
	.string "It says it likes to be sneaky even\l"
	.string "if it has a lot of ouchies!$"

BattleFrontier_Lounge5_Text_NatureGirlCalm::
	.string "Hmhm…\p"
	.string "This one says it looks after itself!\n"
	.string "It says it worries about itself even\l"
	.string "if it has a lot of ouchies!$"

BattleFrontier_Lounge5_Text_NatureGirlGentle::
	.string "Hmhm…\p"
	.string "This one says it looks after itself!\n"
	.string "But if it gets enough ouchies,\l"
	.string "it will hit back!$"

BattleFrontier_Lounge5_Text_NatureGirlSassy::
	.string "Hmhm…\p"
	.string "This one says it likes to battle!\n"
	.string "But if it gets enough ouchies,\l"
	.string "it will turn sneaky!$"

BattleFrontier_Lounge5_Text_NatureGirlCareful::
	.string "Hmhm…\p"
	.string "This one says it looks after itself!\n"
	.string "But if it gets enough ouchies,\l"
	.string "it will turn sneaky!$"

BattleFrontier_Lounge5_Text_NatureGirlEgg::
	.string "That's silly! An EGG is asleep!\n"
	.string "I can't talk to it!$"

BattleFrontier_Lounge5_Text_LadyClaimsSheUnderstandsPokemon::
	.string "How charming!\n"
	.string "That little lady claims she can\l"
	.string "understand POKéMON!$"

BattleFrontier_Lounge5_Text_GirlSayingSomethingProfound::
	.string "I have this feeling that the little girl\n"
	.string "is saying something profound.$"

BattleFrontier_Lounge5_Text_GirlPlaysAtRedHouseALot::
	.string "I know something!\p"
	.string "That little girl plays at the red house\n"
	.string "a lot!$"


@ ---- Battle Royale: Pokemon shop ----
@ VAR_0x8005 = stock list (0 standard, 1 rare), VAR_0x8004 = chosen item.
@ Stock, levels and prices are in src/battle_royale.c (sShopMons).
BattleRoyale_PokemonShop_EventScript_Seller::
	lock
	faceplayer
	setvar VAR_0x8005, 0
	msgbox BattleRoyale_PokemonShop_Text_Welcome, MSGBOX_DEFAULT
	goto BattleRoyale_PokemonShop_EventScript_ShowMenu

BattleRoyale_PokemonShop_EventScript_RareSeller::
	lock
	faceplayer
	goto_if_lt VAR_BR_TIERS_CLEARED, BR_RARE_SHOP_TIERS, BattleRoyale_PokemonShop_EventScript_RareLocked
	setvar VAR_0x8005, 1
	msgbox BattleRoyale_PokemonShop_Text_RareWelcome, MSGBOX_DEFAULT
	goto BattleRoyale_PokemonShop_EventScript_ShowMenu

BattleRoyale_PokemonShop_EventScript_RareLocked::
	msgbox BattleRoyale_PokemonShop_Text_RareLocked, MSGBOX_DEFAULT
	release
	end

BattleRoyale_PokemonShop_EventScript_ShowMenu::
	closemessage
	showmoneybox 0, 0
	call_if_eq VAR_0x8005, 0, BattleRoyale_PokemonShop_EventScript_StandardMenu
	call_if_eq VAR_0x8005, 1, BattleRoyale_PokemonShop_EventScript_RareMenu
	goto_if_ge VAR_RESULT, BR_SHOP_MONS_PER_LIST, BattleRoyale_PokemonShop_EventScript_Exit
	copyvar VAR_0x8004, VAR_RESULT
	special BattleRoyale_BufferShopOffer
	msgbox BattleRoyale_PokemonShop_Text_ConfirmBuy, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleRoyale_PokemonShop_EventScript_Exit
	special BattleRoyale_BuyShopMon
	goto_if_eq VAR_RESULT, BR_SHOP_NO_MONEY, BattleRoyale_PokemonShop_EventScript_NotEnoughMoney
	goto_if_eq VAR_RESULT, BR_SHOP_NO_ROOM, BattleRoyale_PokemonShop_EventScript_NoRoom
	copyvar VAR_TEMP_2, VAR_RESULT
	goto BattleRoyale_PokemonShop_EventScript_Bought

BattleRoyale_PokemonShop_EventScript_StandardMenu::
	multichoice 17, 0, MULTI_BR_POKEMON_SHOP, FALSE
	return

BattleRoyale_PokemonShop_EventScript_RareMenu::
	multichoice 17, 0, MULTI_BR_POKEMON_SHOP_RARE, FALSE
	return

BattleRoyale_PokemonShop_EventScript_Exit::
	hidemoneybox
	msgbox BattleRoyale_PokemonShop_Text_ComeAgain, MSGBOX_DEFAULT
	release
	end

BattleRoyale_PokemonShop_EventScript_Bought::
	updatemoneybox
	bufferspeciesname STR_VAR_1, VAR_TEMP_TRANSFERRED_SPECIES
	playfanfare MUS_OBTAIN_ITEM
	message BattleRoyale_PokemonShop_Text_Bought
	waitmessage
	waitfanfare
	hidemoneybox
	bufferspeciesname STR_VAR_1, VAR_TEMP_TRANSFERRED_SPECIES
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleRoyale_PokemonShop_EventScript_AfterNickname
	goto_if_eq VAR_TEMP_2, BR_SHOP_TO_PC, BattleRoyale_PokemonShop_EventScript_NicknameBoxMon
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto BattleRoyale_PokemonShop_EventScript_AfterNickname

BattleRoyale_PokemonShop_EventScript_NicknameBoxMon::
	call Common_EventScript_NameReceivedBoxMon
	goto BattleRoyale_PokemonShop_EventScript_AfterNickname

BattleRoyale_PokemonShop_EventScript_AfterNickname::
	call_if_eq VAR_TEMP_2, BR_SHOP_TO_PC, Common_EventScript_TransferredToPC
	msgbox BattleRoyale_PokemonShop_Text_TakeCare, MSGBOX_DEFAULT
	release
	end

BattleRoyale_PokemonShop_EventScript_NotEnoughMoney::
	hidemoneybox
	msgbox BattleRoyale_PokemonShop_Text_NotEnoughMoney, MSGBOX_DEFAULT
	release
	end

BattleRoyale_PokemonShop_EventScript_NoRoom::
	hidemoneybox
	msgbox BattleRoyale_PokemonShop_Text_NoRoom, MSGBOX_DEFAULT
	release
	end

BattleRoyale_PokemonShop_Text_Welcome:
	.string "Welcome to the POKéMON SHOP!\n"
	.string "My POKéMON grow as you clear TIERS.$"

BattleRoyale_PokemonShop_Text_RareWelcome:
	.string "Looking for something special?\n"
	.string "I deal in rare POKéMON.$"

BattleRoyale_PokemonShop_Text_RareLocked:
	.string "I deal in rare POKéMON, but only\n"
	.string "with proven TRAINERS.\p"
	.string "Come back when you've cleared\n"
	.string "TIER 2.$"

BattleRoyale_PokemonShop_Text_ConfirmBuy:
	.string "{STR_VAR_1}, level {STR_VAR_2},\n"
	.string "costs ¥{STR_VAR_3}. Buy it?$"

BattleRoyale_PokemonShop_Text_Bought:
	.string "{PLAYER} bought {STR_VAR_1}!$"

BattleRoyale_PokemonShop_Text_TakeCare:
	.string "Take good care of it!$"

BattleRoyale_PokemonShop_Text_ComeAgain:
	.string "Come back any time!$"

BattleRoyale_PokemonShop_Text_NotEnoughMoney:
	.string "Sorry, you don't have enough money\n"
	.string "for that one.$"

BattleRoyale_PokemonShop_Text_NoRoom:
	.string "Your party and your PC are both full.\n"
	.string "There's no room for another POKéMON.$"
