#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "ChatCommandHelpers.h"
#include "InstanceSaveMgr.h"
#include "WorldSession.h"

using namespace Acore::ChatCommands;

class DailyReset : public PlayerScript
{
public:
    DailyReset() : PlayerScript("DailyReset") { }

    void OnPlayerLogin(Player* player) override
    {
        ChatHandler(player->GetSession()).PSendSysMessage("Type .daily reset to reset your daily limits.");
    }
};

class DailyResetCommandScript : public CommandScript
{
public:
    DailyResetCommandScript() : CommandScript("DailyResetCommandScript") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable gcCommandTable =
        {
            { "reset", HandleResetDailyCommand, SEC_PLAYER, Console::Yes },
        };
        static ChatCommandTable HandleGlobalChatCommandTable =
        {
            { "daily", gcCommandTable }
        };
        return HandleGlobalChatCommandTable;
    }

    static bool HandleResetDailyCommand(ChatHandler* handler)
    {
        Player* player = handler->GetSession()->GetPlayer();
        if (!player)
            return false;

        player->ResetDailyQuestStatus();
        player->ResetInstances(player->GetGUID() , INSTANCE_RESET_ALL, false);
        player->ResetInstances(player->GetGUID() , INSTANCE_RESET_ALL, true);
        handler->PSendSysMessage("Your daily limits have been reset.");
        return true;
    }
};

void AddDailyResetScripts()
{
    new DailyReset();
    new DailyResetCommandScript();
}
