#ifndef GLOBALS_H
#define GLOBALS_H

#include "user.h"

#define BUFFER_LEN 1024
#define TELNET_EOL "\n\r"

extern userlist *users; // Initialized in sock.c

#endif // GLOBALS_H
