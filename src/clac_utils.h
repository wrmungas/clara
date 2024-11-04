#ifndef CLAC_UTILS
#define CLAC_UTILS

#include <stdio.h>
#include <stdlib.h>
#include "clac_string.h"
#include "clac_types.h"

void fatal(string message) {
    fprintf(stderr, "%*s\n", message.len, message.chars);
    exit(1);
}

void error(string message) {
    fprintf(stderr, "%*s\n", message.len, message.chars);
}

void message(string message) {
    fprintf(stdout, "%*s\n", message.len, message.chars);
}

#endif