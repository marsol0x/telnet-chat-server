#include <stdlib.h>

#include "command.h"
#include "cmds.h"
#include "ptab.h"

ptab *commands;

void commands_init()
{
    commands = ptab_init();

    command *emote_cmd = malloc(sizeof(command));
    emote_cmd->cmd = emote;
    ptab_add(commands, "emote", emote_cmd);
    command *nick_cmd = malloc(sizeof(command));
    nick_cmd->cmd = nick;
    ptab_add(commands, "nick", nick_cmd);
}
