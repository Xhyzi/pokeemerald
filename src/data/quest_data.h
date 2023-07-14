const struct QuestNpc sExtraNpc_Test[] = {
    {.localId = 2, .map = MAP_OLDALE_TOWN},
    {.localId = 4, .map = MAP_OLDALE_TOWN},
    {.localId = NULL_NPC, .map = NULL_MAP},
};

const struct QuestSecondaryMarker sSecondaryMarker_Test[] = 
{
    {.localId = 3, .map = MAP_OLDALE_TOWN, .flags = QFLAG_0, .iconType = ICON_ARROW},
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
        .secondaryMarkers = sSecondaryMarker_Test,
        .rewardMoney = 0,
        .rewardItems = {
            {.id = ITEM_POTION, .amount = 1},
            {.id = ITEM_PREMIER_BALL, .amount = 5}
        },
        .isQuestReadyCB = QuestCB_DummyTest,
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
        .secondaryMarkers = NULL,
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
