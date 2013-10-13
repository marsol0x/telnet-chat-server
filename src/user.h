#ifndef USER_H
#define USER_H

typedef struct user {
    int sock; // User's socket
    char *name; // User's name
} user;

typedef struct userlist {
    user *user;
    struct userlist *next;
} userlist;

user * newuser(userlist *ul);
void deluser(userlist **ul, user *u);
user * getuserbysock(userlist *ul, int sock);

#endif // USER_H
