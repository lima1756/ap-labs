#include <stdio.h>
#include "strlib.h"

int main(int argc, char **argv) {
    int size = mystrlen(argv[1]);
    char *addition = mystradd(argv[1], argv[2]);
    int find = mystrfind(addition, argv[3]);

    printf("Initial Lenght\t: %i\nNew String\t: %s\nSubString was found\t: %s\n",
        size, addition, find?"yes":"no"
    );
    return 0;
}
