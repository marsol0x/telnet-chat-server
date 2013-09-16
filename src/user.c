#include <stdlib.h>

#include "user.h"

t_user * newuser(t_userlist *ul)
{
    t_userlist *p;
    t_user *u;
    for (p = ul; (p->next != NULL) && (p->user != NULL); p = p->next);
        // Go to end of userlist to append new user
    u = (t_user *) malloc(sizeof(t_user));
    u->name = NULL;

    if (p->user == NULL) {
        p->user = u;
    } else {
        p->next = (t_userlist *) malloc(sizeof(t_userlist));
        p->next->next = NULL;
        p->next->user = u;
    }
    return u;
}

void deluser(t_userlist *ul, t_user *u)
{
    t_userlist *next, *p;
    for (p = ul; p->next->user != u; p = p->next);
        // Go to where the user is located
    next = p->next->next;
    p->next = next;
    free(u);
}

t_user * getuserbysock(t_userlist *ul, int sock)
{
    t_userlist *p;
    for (p = ul;; p++) {
        if ((p->user != NULL) && (p->user->sock == sock)) {
            return p->user;
        }
        if (p->next == NULL) {
            return NULL;
        }
    }

    // If we got here, we didn't find the user
    return NULL;
}
