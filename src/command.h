#ifndef COMMAND_H
#define COMMAND_H

#include "user.h"

#define COMMAND(command_name) void command_name(user *u, char *args)

typedef struct command_t {
    void (* cmd)(user *u, char *args);
} command;

void commands_init();

#endif // COMMAND_H
