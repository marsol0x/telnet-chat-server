#ifndef PTAB_H
#define PTAB_H

#include "user.h"

typedef struct ptab {
    void (* command)(t_user *u, char *data);
    struct ptab * prefix[26];
} ptab;

ptab * ptab_init(ptab *pt, char *p);
int ptab_add(ptab *pt, char *prefix, void (* command)(t_user *u, char *data));

#endif // PTAB_H
