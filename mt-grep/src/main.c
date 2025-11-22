#include <stdio.h>
#include <stdlib.h>
#include "fsearch.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: %s <dir> <pattern>\n", argv[0]);
        return 1;
    }
    search_in_dir(argv[1], argv[2]);
    return 0;
}

