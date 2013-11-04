#include "command.h"
#include "cmds.h"
#include "ptab.h"

ptab *commands;

void commands_init()
{
    commands = ptab_init();

    command emote_cmd = {emote};
    ptab_add(commands, "emote", &emote_cmd);
    command nick_cmd = {nick};
    ptab_add(commands, "nick", &nick_cmd);
}
