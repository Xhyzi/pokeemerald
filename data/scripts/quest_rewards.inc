.set QUESTID, VAR_0x8000
.set ITEMID_1, VAR_0x8001
.set AMOUNT_1, VAR_0x8002
.set ITEMID_2, VAR_0x8003
.set AMOUNT_2, VAR_0x8004
.set MONEY, VAR_0x8005

Std_GiveQuestRewards::
    callnative Quest_BufferQuestRewards

EventScript_TryGiveReward_1:
    goto_if_eq ITEMID_1, ITEM_NONE, EventScript_TryGiveReward_2
    goto_if_eq AMOUNT_1, 0, EventScript_TryGiveReward_2
    giveitem ITEMID_1, AMOUNT_1

EventScript_TryGiveReward_2: 
    goto_if_eq ITEMID_2, ITEM_NONE, EventScript_TryGiveRewardMoney
    goto_if_eq AMOUNT_2, 0, EventScript_TryGiveRewardMoney
    giveitem ITEMID_2, AMOUNT_2

EventScript_TryGiveRewardMoney:
    goto_if_eq MONEY, 0, EventScript_FinishReward
    addmoney MONEY
    buffernumberstring STR_VAR_1, MONEY
    msgbox Text_PlayerRecievedMoney, MSGBOX_DEFAULT
    closemessage

EventScript_FinishReward:
    end

Text_PlayerRecievedMoney:
    .string "{PLAYER} recibió {STR_VAR_1} ¥.$"
