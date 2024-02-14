/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include <stdlib.h>
#include "ChallengeModes.h"

// Add player scripts
class WorldMission : public PlayerScript
{
public:
    WorldMission() : PlayerScript("GlobalMission") { }

    bool GivePlayerQuest(uint32 questId, Player* player)
    {
        Quest const* quest = sObjectMgr->GetQuestTemplate(questId);
        if (quest)
        {
            if (player->CanTakeQuest(quest, false)) return false;
            player->AddQuest(quest, nullptr);
            player->SendQuestUpdate(questId);
            return true;
        }
        return false;
    }

    std::string MissionIndex = sConfigMgr->GetOption<std::string>("zhCN.GlobalMission", "1");
    std::string IsHardCoreStr = sConfigMgr->GetOption<std::string>("zhCN.IsHardCoreRequired", "1");
    std::string GetWorldMissionDialague = sConfigMgr->GetOption<std::string>("zhCN.MissionDialague", "世界任务已接取");

    void OnLogin(Player* player) override
    {
        //if (sConfigMgr->GetOption<bool>("MyModule.Enable", false))
        //{
        //    ChatHandler(player->GetSession()).PSendSysMessage(HELLO_WORLD);
        //}
        if (IsHardCoreStr == "1")
        {
            if (!sChallengeModes->challengeEnabledForPlayer(SETTING_GOLDEN_HARDCORE, player)) return;
            int MissionNum = atoi(MissionIndex.c_str());
            GivePlayerQuest(MissionNum, player);
        }
        else
        {
            int MissionNum = atoi(MissionIndex.c_str());
            GivePlayerQuest(MissionNum, player);
        }
        ChatHandler(player->GetSession()).PSendSysMessage(GetWorldMissionDialague.c_str());
    }
};

// Add all scripts in one
void AddWorldMissionScripts()
{
    new WorldMission();
}
