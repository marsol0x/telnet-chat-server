#ifndef SOCK_H
#define SOCK_H

int createserver(char *port);
void pollconns();
int acceptnewconn();
int readall(int sock);
void sendtoall(int sock, char *buffer, int size);
int sendall(int sock, char *buffer, int size);

#endif // SOCK_H
