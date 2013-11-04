// Commands are defined here, but registered at command.c
#include <stdio.h>

#include "command.h"
#include "cmds.h"
#include "user.h"

COMMAND(emote)
{
    printf("emote\n");
}

COMMAND(nick)
{
    printf("nick\n");
}
