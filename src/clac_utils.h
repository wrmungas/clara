#ifndef CLAC_UTILS
#define CLAC_UTILS

#include <stdio.h>
#include <stdlib.h>
#include "clac_string.h"
#include "clac_types.h"

void init_all();
void free_all(bool info);

void fatal(char *src) {
    fprintf(stderr, "%s\n", src);
    free_all(false);
    exit(1);
}

void error(string src) {
    fprintf(stderr, "%*s\n", src.len, src.chars);
}

void message(string src) {
    fprintf(stdout, "%*s\n", src.len, src.chars);
}

#endif