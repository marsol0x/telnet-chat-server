#include <stdio.h>
#include <stdlib.h>

#include "sock.h"

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage:\n\t%s <port>\n", argv[0]);
        return -1;
    }

    if (createserver(argv[1]) == -1) {
        return -1;
    }

    for (;;) {
        pollconns();
    }

    return 0;
}
