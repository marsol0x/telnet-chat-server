#ifndef GLOBALS_H
#define GLOBALS_H

#include "user.h"
#include "ptab.h"

#define BUFFER_LEN 1024
#define TELNET_EOL "\n\r"
#define LEADER "> "
#define CMD_LEADER '/'

extern userlist *users; // Initialized in sock.c
extern ptab *commands; // Initialized in command.c

#endif // GLOBALS_H
