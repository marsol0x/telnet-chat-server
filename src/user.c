#include <stdlib.h>

#include "user.h"

user * newuser(userlist *ul)
{
    user *u;
    userlist *p = ul;
    while (p->next != NULL) {
        p = p->next; // Go to end of userlist to append new user
    }
    u = (user *) malloc(sizeof(user));
    u->name = NULL;

    if (p->user == NULL) {
        p->user = u;
    } else {
        p->next = (userlist *) malloc(sizeof(userlist));
        p->next->next = NULL;
        p->next->user = u;
    }
    return u;
}

void deluser(userlist **ul, user *u)
{
    userlist *next, *p, *prev;
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

user * getuserbysock(userlist *ul, int sock)
{
    userlist *p = ul;
    do {
        if ((p->user != NULL) && (p->user->sock == sock)) {
            return p->user;
        }
        p = p->next;
    } while (p != NULL);

    // If we got here, we didn't find the user
    return NULL;
}

