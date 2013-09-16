#ifndef USER_H
#define USER_H

typedef struct user {
    int sock; // User's socket
    char *name; // User's name
} t_user;

typedef struct userlist {
    t_user *user;
    struct userlist *next;
} t_userlist;

t_user * newuser(t_userlist *ul);
void deluser(t_userlist *ul, t_user *u);
t_user * getuserbysock(t_userlist *ul, int sock);

#endif // USER_H
