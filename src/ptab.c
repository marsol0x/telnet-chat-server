#include <stdlib.h>
#include <string.h>

#include "ptab.h"

ptab * ptab_init()
{
    ptab *pt = (ptab *) malloc(sizeof(ptab));
    pt->data = NULL;
    for (int i = 0; i < PTAB_MAX_PREFIX; i++)
        pt->prefix[i] = NULL;
    return pt;
}

ptab * ptab_add(ptab *pt, char *prefix, void *data)
{
    ptab **p = &pt;
    for (size_t i = 0; i < strlen(prefix); i++) {
        p = &((*p)->prefix[prefix[i] - 'a']);
        if (*p == NULL) {
            *p = ptab_init();
        }
    }

    (*p)->data = data;

    return *p;
}

void * ptab_get(ptab *pt, char *prefix)
{
    ptab *p = pt;
    for (size_t i = 0; i < strlen(prefix); i++) {
        int index = prefix[i] - 'a';
        if (p->prefix[index] == NULL) {
            return NULL;
        }
        p = p->prefix[index];
    }

    if (p == NULL) return NULL;
    return p->data;
}
