#ifndef PTAB_H
#define PTAB_H

#define PTAB_MAX_PREFIX 26

typedef struct ptab_t {
    void *data;
    struct ptab_t * prefix[PTAB_MAX_PREFIX];
} ptab;

ptab * ptab_init();
ptab * ptab_add(ptab *pt, char *prefix, void *data);
void * ptab_get(ptab *pt, char *prefix);

#endif // PTAB_H
