#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"
#include "sock.h"
#include "user.h"

#include "globals.h"

int parse(int sock, char *msg)
{
    user *user;
    user = getuserbysock(users, sock);
    char buffer[BUFFER_LEN];
    char *tok;

    // Users are created on sock accept, if it doesn't exist throw error
    if (user == NULL) {
        perror("Error locating user for current sock.");
        return -1;
    }

    // If the username is non-existance, assume msg is the username
    // accept() asks for a username
    if (user->name == NULL) {
        int len = strlen(buffer);
        user->name = (char *) malloc(sizeof(buffer));
        strncpy(user->name, buffer, len);

        // Replace the first instance of a character in TELNET_EOL with the
        // null-terminator, we only need the first one since all subsequent
        // will be auto-nulled by the placement.
        tok = strtok(user->name, TELNET_EOL);
        tok = '\0';
    }
}
