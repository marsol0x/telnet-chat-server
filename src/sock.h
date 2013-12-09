#ifndef SOCK_H
#define SOCK_H

#include "user.h"

int createserver(char *port);
void pollconns();
int acceptnewconn();
int readall(int sock);
void sendtoall(char *buffer);
void sendtouser(user *u, char *msg);
void senderror(user *u, char *msg);

#endif // SOCK_H
