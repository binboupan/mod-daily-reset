#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "ChatCommandHelpers.h"
#include "InstanceSaveMgr.h"
#include "WorldSession.h"

using namespace Acore::ChatCommands;


// Add player scripts
class DailyReset : public PlayerScript
{
public:
    DailyReset() : PlayerScript("DailyReset") { }

    void OnPlayerLogin(Player* player) override
    {
            ChatHandler(player->GetSession()).PSendSysMessage("Type .daily reset to reset your daily dungeon limits.");
    }
};

// Chat command script for .reset daily
class DailyResetCommandScript : public CommandScript
{
public:
    DailyResetCommandScript() : CommandScript("DailyResetCommandScript") { }

  ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable gcCommandTable =
        {
            { "reset",  HandleResetDailyCommand,  SEC_PLAYER, Console::Yes },
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

    // Reset daily dungeon limits
    player->ResetDailyQuestStatus();
    
    // Send confirmation message
    handler->PSendSysMessage("Your daily dungeon limits have been reset.");
    return true;
    }
};

// Add all scripts in one
void AddDailyResetScripts()
{
    new DailyReset();
    new DailyResetCommandScript();
}