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
    } else {
        printf("%s\n", msg);
        sendtoall(user->name, msg, strlen(msg));
    }

    return 0;
}
