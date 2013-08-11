#ifndef SOCK_H
#define SOCK_H

int createserver(char *port);
void pollconns();
int acceptnewconn();
int readall(int sock);

#endif // SOCK_H
