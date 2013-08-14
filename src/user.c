#include <stdlib.h>

#include "user.h"

t_user * newuser(t_userlist *ul)
{
    t_userlist *p;
    t_user *u;
    for (p = ul; p->next != NULL; p = p->next);
        // Go to end of userlist to append new user
    u = (t_user *) malloc(sizeof(t_user));
    p->user = u; 
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
