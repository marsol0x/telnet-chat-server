#include <stdio.h>
#include <stdlib.h>
#include "sock.h"

int main(int argc, char **argv)
{
    if (createserver(argv[1]) == -1) {
        return -1;
    }
    
    for (;;) {
        pollconns();
    }

    return 0;
}
