#include <stdlib.h>

#include "user.h"

t_user * newuser(t_userlist *ul)
{
    t_user *u;
    t_userlist *p = ul;
    while (p->next != NULL) {
        p = p->next; // Go to end of userlist to append new user
    }
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

void deluser(t_userlist **ul, t_user *u)
{
    t_userlist *next, *p, *prev;
    for (p = *ul; p->user != u; p = p->next) {
        prev = p; // Find the user, but also keep track of previous user
    }
    if (p == *ul) {
        if (p->next != NULL) {
            *ul = p->next;
            free(p);
        } else {
            p->user = NULL;
        }
    } else {
        next = p->next;
        prev->next = next;
        free(p);
    }
    free(u);
}

t_user * getuserbysock(t_userlist *ul, int sock)
{
    t_userlist *p = ul;
    do {
        if ((p->user != NULL) && (p->user->sock == sock)) {
            return p->user;
        }
        p = p->next;
    } while (p != NULL);

    // If we got here, we didn't find the user
    return NULL;
}

