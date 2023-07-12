const struct QuestNpc sExtraNpc_Test[] = {
    {.localId = 0, .map = MAP_PETALBURG_CITY},
    {.localId = 1, .map = MAP_PETALBURG_CITY},
    {.localId = 2, .map = MAP_PETALBURG_CITY},
    {.localId = 3, .map = MAP_PETALBURG_CITY},
    {.localId = 4, .map = MAP_PETALBURG_CITY},
    {.localId = 5, .map = MAP_PETALBURG_CITY},
    {.localId = 6, .map = MAP_PETALBURG_CITY},
    {.localId = 7, .map = MAP_PETALBURG_CITY},
    {.localId = 8, .map = MAP_PETALBURG_CITY},
    {.localId = 9, .map = MAP_PETALBURG_CITY},
    {.localId = 10, .map = MAP_PETALBURG_CITY},
    {.localId = 11, .map = MAP_PETALBURG_CITY},
    {.localId = 12, .map = MAP_PETALBURG_CITY},
    {.localId = 13, .map = MAP_PETALBURG_CITY},
    {.localId = 14, .map = MAP_PETALBURG_CITY},
    {.localId = 15, .map = MAP_PETALBURG_CITY},
    {.localId = 16, .map = MAP_PETALBURG_CITY},
    {.localId = 17, .map = MAP_PETALBURG_CITY},
    {.localId = 18, .map = MAP_PETALBURG_CITY},
    {.localId = 19, .map = MAP_PETALBURG_CITY},
    {.localId = 20, .map = MAP_PETALBURG_CITY},
    {.localId = 21, .map = MAP_PETALBURG_CITY},
    {.localId = 22, .map = MAP_PETALBURG_CITY},
    {.localId = 23, .map = MAP_PETALBURG_CITY},
    {.localId = 24, .map = MAP_PETALBURG_CITY},
    {.localId = 25, .map = MAP_PETALBURG_CITY},
    {.localId = NULL_NPC, .map = NULL_MAP},
};

const struct Quest sQuestList[QUEST_COUNT] = 
{
    [QUEST_1] = {
        .type = QUEST_PRIMARY,
        .name = _("Test Quest"),
        .description = _("This is a test quest.\n"
                        "You can have up to 24 + 1\n"
                        "characters per line.1234"),
        .location = _("Ciudad Petalia FF"),
        .advisedLevel = 5,
        .difficulty = 1,
        .npc = {.localId = 1, .map = MAP_OLDALE_TOWN},
        .extraNpcs = sExtraNpc_Test,
        .rewardMoney = 0,
        .rewardItems = {
            {.id = ITEM_POTION, .amount = 1},
            {.id = ITEM_PREMIER_BALL, .amount = 5}
        }         
    },
    [QUEST_2] = {
        .type = QUEST_PRIMARY,
        .name = _("Test Quest 2"),
        .description = _("This is a test quest 2."),
        .location = _("Ciudad Petalia FF"),
        .advisedLevel = 10,
        .difficulty = 2,
        .npc = {.localId = 2, .map = MAP_PETALBURG_CITY},
        .extraNpcs = NULL,
        .rewardMoney = 10,
        .rewardItems = {
            {.id = ITEM_SUPER_POTION, .amount = 2},
            {.id = ITEM_GREAT_BALL, .amount = 5},
        }
    },
    [QUEST_3] = {
        .type = QUEST_PRIMARY,
        .name = _("Test Quest 3"),
        .description = _("This is a test quest 3."),
        .location = _("Ciudad Petalia FF"),
        .advisedLevel = 15,
        .difficulty = 3,
        .npc = {.localId = 3, .map = MAP_PETALBURG_CITY},
        .extraNpcs = NULL,
        .rewardMoney = 100,
        .rewardItems = {
            {.id = ITEM_HYPER_POTION, .amount = 3},
            {.id = ITEM_PREMIER_BALL, .amount = 5},
        }
    },
    [QUEST_4] = {
        .type = QUEST_SECONDARY,
        .name = _("Test Quest 4"),
        .description = _("This is a test quest 4."),
        .location = _("Ciudad Petalia FF"),
        .advisedLevel = 20,
        .difficulty = 4,
        .npc = {.localId = 4, .map = MAP_PETALBURG_CITY},
        .extraNpcs = NULL,
        .rewardMoney = 1000,
        .rewardItems = { 
            {.id = ITEM_POTION, .amount = 4},
            {.id = ITEM_ULTRA_BALL, .amount = 5},
        }
    },
    [QUEST_5] = {
        .type = QUEST_SECONDARY,
        .name = _("Test Quest 5"),
        .description = _("This is a test quest 5."),
        .location = _("Ciudad Petalia FF"),
        .advisedLevel = 25,
        .difficulty = 5,
        .npc = {.localId = 5, .map = MAP_PETALBURG_CITY},
        .extraNpcs = NULL,
        .rewardMoney = 10000,
        .rewardItems = { 
            {.id = ITEM_NONE, .amount = 5},
            {.id = ITEM_GREAT_BALL, .amount = 0},
        }
    },
    [QUEST_6] = {
        .type = QUEST_SECONDARY,
        .name = _("Test Quest 6"),
        .description = _("This is a test quest 6."),
        .location = _("Ciudad Petalia FF"),
        .advisedLevel = 30,
        .difficulty = 1,
        .npc = {.localId = 6, .map = MAP_PETALBURG_CITY},
        .extraNpcs = NULL,
        .rewardMoney = 100000,
        .rewardItems = {
            {.id = ITEM_HYPER_POTION, .amount = 6},
            {.id = ITEM_LUXURY_BALL, .amount = 5},
        }
    },
    [QUEST_7] = {
        .type = QUEST_SECONDARY,
        .name = _("Test Quest 7"),
        .description = _("This is a test quest 7."),
        .location = _("Ciudad Férrica F"),
        .advisedLevel = 35,
        .difficulty = 2,
        .npc = {.localId = 1, .map = MAP_RUSTBORO_CITY},
        .extraNpcs = NULL,
        .rewardMoney = 1000000,
        .rewardItems = { 
            {.id = ITEM_POTION, .amount = 7},
            {.id = ITEM_PREMIER_BALL, .amount = 5},
        }
    },
    [QUEST_8] = {
        .type = QUEST_SECONDARY,
        .name = _("Test Quest 8"),
        .description = _("This is a test quest 8."),
        .location = _("Ciudad Férrica F"),
        .advisedLevel = 40,
        .difficulty = 3,
        .npc = {.localId = 2, .map = MAP_RUSTBORO_CITY},
        .extraNpcs = NULL,
        .rewardMoney = 9878,
        .rewardItems = { 
            {.id = ITEM_SUPER_POTION, .amount = 8},
            {.id = ITEM_NEST_BALL, .amount = 5},
        }
    },
    [QUEST_9] = {
        .type = QUEST_SECONDARY,
        .name = _("Test Quest 9"),
        .description = _("This is a test quest 9."),
        .location = _("Ciudad Férrica F"),
        .advisedLevel = 45,
        .difficulty = 4,
        .npc = {.localId = 3, .map = MAP_RUSTBORO_CITY},
        .extraNpcs = NULL,
        .rewardMoney = 27369,
        .rewardItems = {
            {.id = ITEM_HYPER_POTION, .amount = 9},
            {.id = ITEM_PREMIER_BALL, .amount = 5},
        }
    },
    [QUEST_10] = {
        .type = QUEST_SECONDARY,
        .name = _("Test Quest 10"),
        .description = _("characters per line.1234\n"
                    "characters per line.1234\n"
                    "characters per line.1234"),
        .location = _("Ciudad Férrica F"),
        .advisedLevel = 50,
        .difficulty = 5,
        .npc = {.localId = 4, .map = MAP_RUSTBORO_CITY},
        .extraNpcs = NULL,
        .rewardMoney = 1000000,
        .rewardItems = {
            {.id = ITEM_POTION, .amount = 10},
            {.id = ITEM_PREMIER_BALL, .amount = 5},
        }
    },
    [QUEST_11] = {
        .type = QUEST_SECONDARY,
        .name = _("Test Quest"),
        .description = _("This is a test quest.\n"
                        "You can have up to 24 + 1\n"
                        "characters per line.1234"),
        .location = _("Ciudad Petalia FF"),
        .advisedLevel = 5,
        .difficulty = 1,
        .npc = {.localId = 0, .map = MAP_PETALBURG_CITY},
        .extraNpcs = NULL,
        .rewardMoney = 0,
        .rewardItems = {
            {.id = ITEM_POTION, .amount = 1},
            {.id = ITEM_PREMIER_BALL, .amount = 5}
        }         
    },
    [QUEST_12] = {
        .type = QUEST_SECONDARY,
        .name = _("Test Quest 2"),
        .description = _("This is a test quest 2."),
        .location = _("Ciudad Petalia FF"),
        .advisedLevel = 10,
        .difficulty = 2,
        .npc = {.localId = 1, .map = MAP_PETALBURG_CITY},
        .extraNpcs = NULL,
        .rewardMoney = 10,
        .rewardItems = {
            {.id = ITEM_SUPER_POTION, .amount = 2},
            {.id = ITEM_GREAT_BALL, .amount = 5},
        }
    },
    [QUEST_13] = {
        .type = QUEST_SECONDARY,
        .name = _("Test Quest 3"),
        .description = _("This is a test quest 3."),
        .location = _("Ciudad Petalia FF"),
        .advisedLevel = 15,
        .difficulty = 3,
        .npc = {.localId = 2, .map = MAP_PETALBURG_CITY},
        .extraNpcs = NULL,
        .rewardMoney = 100,
        .rewardItems = {
            {.id = ITEM_HYPER_POTION, .amount = 3},
            {.id = ITEM_PREMIER_BALL, .amount = 5},
        }
    },
    [QUEST_14] = {
        .type = QUEST_SECONDARY,
        .name = _("Test Quest 4"),
        .description = _("This is a test quest 4."),
        .location = _("Ciudad Petalia FF"),
        .advisedLevel = 20,
        .difficulty = 4,
        .npc = {.localId = 3, .map = MAP_PETALBURG_CITY},
        .extraNpcs = NULL,
        .rewardMoney = 1000,
        .rewardItems = { 
            {.id = ITEM_POTION, .amount = 4},
            {.id = ITEM_ULTRA_BALL, .amount = 5},
        }
    },
    [QUEST_15] = {
        .type = QUEST_SECONDARY,
        .name = _("Test Quest 5"),
        .description = _("This is a test quest 5."),
        .location = _("Ciudad Petalia FF"),
        .advisedLevel = 25,
        .difficulty = 5,
        .npc = {.localId = 4, .map = MAP_PETALBURG_CITY},
        .extraNpcs = NULL,
        .rewardMoney = 10000,
        .rewardItems = { 
            {.id = ITEM_NONE, .amount = 5},
            {.id = ITEM_GREAT_BALL, .amount = 0},
        }
    },
    [QUEST_16] = {
        .type = QUEST_SECONDARY,
        .name = _("Test Quest 6"),
        .description = _("This is a test quest 6."),
        .location = _("Ciudad Petalia FF"),
        .advisedLevel = 30,
        .difficulty = 1,
        .npc = {.localId = 5, .map = MAP_PETALBURG_CITY},
        .extraNpcs = NULL,
        .rewardMoney = 100000,
        .rewardItems = {
            {.id = ITEM_HYPER_POTION, .amount = 6},
            {.id = ITEM_LUXURY_BALL, .amount = 5},
        }
    },
    [QUEST_17] = {
        .type = QUEST_SECONDARY,
        .name = _("Test Quest 7"),
        .description = _("This is a test quest 7."),
        .location = _("Ciudad Férrica F"),
        .advisedLevel = 35,
        .difficulty = 2,
        .npc = {.localId = 1, .map = MAP_RUSTBORO_CITY},
        .extraNpcs = NULL,
        .rewardMoney = 1000000,
        .rewardItems = { 
            {.id = ITEM_POTION, .amount = 7},
            {.id = ITEM_PREMIER_BALL, .amount = 5},
        }
    },
    [QUEST_18] = {
        .type = QUEST_SECONDARY,
        .name = _("Test Quest 8"),
        .description = _("This is a test quest 8."),
        .location = _("Ciudad Férrica F"),
        .advisedLevel = 40,
        .difficulty = 3,
        .npc = {.localId = 2, .map = MAP_RUSTBORO_CITY},
        .extraNpcs = NULL,
        .rewardMoney = 9878,
        .rewardItems = { 
            {.id = ITEM_SUPER_POTION, .amount = 8},
            {.id = ITEM_NEST_BALL, .amount = 5},
        }
    },
    [QUEST_19] = {
        .type = QUEST_SECONDARY,
        .name = _("Test Quest 9"),
        .description = _("This is a test quest 9."),
        .location = _("Ciudad Férrica F"),
        .advisedLevel = 45,
        .difficulty = 4,
        .npc = {.localId = 3, .map = MAP_RUSTBORO_CITY},
        .extraNpcs = NULL,
        .rewardMoney = 27369,
        .rewardItems = {
            {.id = ITEM_HYPER_POTION, .amount = 9},
            {.id = ITEM_PREMIER_BALL, .amount = 5},
        }
    },
    [QUEST_20] = {
        .type = QUEST_SECONDARY,
        .name = _("Test Quest 10"),
        .description = _("characters per line.1234\n"
                    "characters per line.1234\n"
                    "characters per line.1234"),
        .location = _("Ciudad Férrica F"),
        .advisedLevel = 50,
        .difficulty = 5,
        .npc = {.localId = 4, .map = MAP_RUSTBORO_CITY},
        .extraNpcs = NULL,
        .rewardMoney = 1000000,
        .rewardItems = {
            {.id = ITEM_POTION, .amount = 10},
            {.id = ITEM_PREMIER_BALL, .amount = 5},
        }
    },
};
