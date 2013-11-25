#ifndef USER_H
#define USER_H

#define USERNAMEMAX 50

typedef struct user {
    int sock; // User's socket
    char name[USERNAMEMAX]; // User's name
} user;

typedef struct userlist {
    user *user;
    struct userlist *next;
} userlist;

user * newuser(userlist *ul);
void deluser(userlist **ul, user *u);
user * getuserbysock(userlist *ul, int sock);
void setusername(user *u, char *name);

#endif // USER_H
