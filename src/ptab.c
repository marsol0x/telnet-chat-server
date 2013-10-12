#include <stdlib.h>
#include <string.h>

#include "ptab.h"
#include "user.h"

ptab * ptab_init(ptab *pt, char *p)
{
    // Find where this prefix should go
    ptab *t = pt;
    for (size_t i = 0; i < strlen(p); i++) {
        ptab *tmp = t->prefix['a' - p[i]];
        if (tmp == NULL) {
            tmp = (ptab *) malloc(sizeof(ptab));
        } 
        t = tmp;
    }

    return t;
}

int ptab_add(ptab *pt, char *prefix, void (* command)(t_user *u, char *data))
{
    return 0;
}
