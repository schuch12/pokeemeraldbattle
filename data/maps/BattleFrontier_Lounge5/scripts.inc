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
BattleRoyale_PokemonShop_EventScript_Seller::
	lock
	faceplayer
	msgbox BattleRoyale_PokemonShop_Text_Welcome, MSGBOX_DEFAULT
	closemessage
	showmoneybox 0, 0
	multichoice 12, 0, MULTI_BR_POKEMON_SHOP, FALSE
	switch VAR_RESULT
	case 0, BattleRoyale_PokemonShop_EventScript_Buy0
	case 1, BattleRoyale_PokemonShop_EventScript_Buy1
	case 2, BattleRoyale_PokemonShop_EventScript_Buy2
	case 3, BattleRoyale_PokemonShop_EventScript_Buy3
	case 4, BattleRoyale_PokemonShop_EventScript_Buy4
	case 5, BattleRoyale_PokemonShop_EventScript_Buy5
	hidemoneybox
	msgbox BattleRoyale_PokemonShop_Text_ComeAgain, MSGBOX_DEFAULT
	release
	end

BattleRoyale_PokemonShop_EventScript_Buy0::
	checkmoney 1000
	goto_if_eq VAR_RESULT, FALSE, BattleRoyale_PokemonShop_EventScript_NotEnoughMoney
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_ZIGZAGOON
	givemon SPECIES_ZIGZAGOON, 5
	goto_if_eq VAR_RESULT, MON_CANT_GIVE, BattleRoyale_PokemonShop_EventScript_NoRoom
	copyvar VAR_TEMP_2, VAR_RESULT
	removemoney 1000
	goto BattleRoyale_PokemonShop_EventScript_Bought

BattleRoyale_PokemonShop_EventScript_Buy1::
	checkmoney 1500
	goto_if_eq VAR_RESULT, FALSE, BattleRoyale_PokemonShop_EventScript_NotEnoughMoney
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_WINGULL
	givemon SPECIES_WINGULL, 5
	goto_if_eq VAR_RESULT, MON_CANT_GIVE, BattleRoyale_PokemonShop_EventScript_NoRoom
	copyvar VAR_TEMP_2, VAR_RESULT
	removemoney 1500
	goto BattleRoyale_PokemonShop_EventScript_Bought

BattleRoyale_PokemonShop_EventScript_Buy2::
	checkmoney 2000
	goto_if_eq VAR_RESULT, FALSE, BattleRoyale_PokemonShop_EventScript_NotEnoughMoney
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_MAKUHITA
	givemon SPECIES_MAKUHITA, 5
	goto_if_eq VAR_RESULT, MON_CANT_GIVE, BattleRoyale_PokemonShop_EventScript_NoRoom
	copyvar VAR_TEMP_2, VAR_RESULT
	removemoney 2000
	goto BattleRoyale_PokemonShop_EventScript_Bought

BattleRoyale_PokemonShop_EventScript_Buy3::
	checkmoney 2000
	goto_if_eq VAR_RESULT, FALSE, BattleRoyale_PokemonShop_EventScript_NotEnoughMoney
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_GEODUDE
	givemon SPECIES_GEODUDE, 5
	goto_if_eq VAR_RESULT, MON_CANT_GIVE, BattleRoyale_PokemonShop_EventScript_NoRoom
	copyvar VAR_TEMP_2, VAR_RESULT
	removemoney 2000
	goto BattleRoyale_PokemonShop_EventScript_Bought

BattleRoyale_PokemonShop_EventScript_Buy4::
	checkmoney 3000
	goto_if_eq VAR_RESULT, FALSE, BattleRoyale_PokemonShop_EventScript_NotEnoughMoney
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_GASTLY
	givemon SPECIES_GASTLY, 5
	goto_if_eq VAR_RESULT, MON_CANT_GIVE, BattleRoyale_PokemonShop_EventScript_NoRoom
	copyvar VAR_TEMP_2, VAR_RESULT
	removemoney 3000
	goto BattleRoyale_PokemonShop_EventScript_Bought

BattleRoyale_PokemonShop_EventScript_Buy5::
	checkmoney 3500
	goto_if_eq VAR_RESULT, FALSE, BattleRoyale_PokemonShop_EventScript_NotEnoughMoney
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_TRAPINCH
	givemon SPECIES_TRAPINCH, 5
	goto_if_eq VAR_RESULT, MON_CANT_GIVE, BattleRoyale_PokemonShop_EventScript_NoRoom
	copyvar VAR_TEMP_2, VAR_RESULT
	removemoney 3500
	goto BattleRoyale_PokemonShop_EventScript_Bought

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
	goto_if_eq VAR_TEMP_2, MON_GIVEN_TO_PC, BattleRoyale_PokemonShop_EventScript_NicknameBoxMon
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto BattleRoyale_PokemonShop_EventScript_AfterNickname

BattleRoyale_PokemonShop_EventScript_NicknameBoxMon::
	call Common_EventScript_NameReceivedBoxMon
	goto BattleRoyale_PokemonShop_EventScript_AfterNickname

BattleRoyale_PokemonShop_EventScript_AfterNickname::
	call_if_eq VAR_TEMP_2, MON_GIVEN_TO_PC, Common_EventScript_TransferredToPC
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
	.string "Every POKéMON here is level 5.$"

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
