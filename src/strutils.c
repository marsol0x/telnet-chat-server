#include <string.h>
#include "strutils.h"

char * strnchr(char *s, int c, int n)
{
    while(--n) {
        if (*s == c) {
            return s;
        } else {
            s++;
        }
    }

    return NULL;
}
