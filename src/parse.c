#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"
#include "sock.h"
#include "user.h"
#include "command.h"
#include "ptab.h"

#include "globals.h"

int parse(int sock, char *msg)
{
    user *user;
    user = getuserbysock(users, sock);

    // Users are created on sock accept, if it doesn't exist throw error
    if (user == NULL) {
        fprintf(stderr, "Error locating user for current socket: %d\n", sock);
        // TODO: Should I close the socket?
        return -1;
    }

    // If the username is non-existant, assume msg is the username
    // accept() asks for a username
    if (user->name == NULL) {
        int len = strlen(msg);
        user->name = (char *) malloc(sizeof(msg));
        strncpy(user->name, msg, len);

        printf("New user: %s\n", user->name);

        return 0;
    }

    // Check for command
    if (msg[0] == CMD_LEADER) {
        char *cmd_name = NULL;
        char *args = NULL;
        command *cmd;
        int size;
        char *end;

        if (strlen(msg) == 1) {
            senderror(user, "Invalid command");
            return -1;
        }
        end = strchr((msg + 1), ' ');
        // If end is NULL, we have a one-word command, no args
        if (end == NULL) {
            size = strlen(msg) - 1;
        } else {
            size = end - (msg + 1);
            args = end + 1;
        }

        cmd_name = (char *) malloc(size);
        strncpy(cmd_name, (msg + 1), size);
        cmd = (command *) ptab_get(commands, cmd_name);
        if (cmd == NULL) {
            free(cmd_name);
            senderror(user, "No such command");
            return -1;
        }

        cmd->cmd(user, args);
        free(cmd_name);
        return 0;
    }

    // Else, just send to all as normal
    char output[BUFFER_LEN];
    sprintf(output, "%s%s%s", user->name, LEADER, msg);
    sendtoall(output);

    return 0;
}
