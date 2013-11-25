// Commands are defined here, but registered at command.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sock.h"
#include "command.h"
#include "cmds.h"
#include "user.h"

#include "globals.h"

COMMAND(emote)
{
    if (args == NULL) {
        senderror(u, "What are you emoting?");
        return;
    }

    char output[BUFFER_LEN];
    sprintf(output, "%s %s", u->name, args);
    sendtoall(output);
}

COMMAND(nick)
{
    if (args == NULL) {
        senderror(u, "Need a new nickname to change.");
        return;
    }

    // No whitespace
    if (strstr(args, "\t ") != NULL) {
        senderror(u, "Nickname cannot contain whitespace");
        return;
    }

    setusername(u, args);
    sendtouser(u, "Nickname changed");
}
