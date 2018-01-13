#ifndef REVERSE_CLIENT_H
#define REVERSE_CLIENT_H

#include "reverse.h"

typedef struct {
    char **targets;
    size_t targets_cnt;
} args_client;

int start_client(args_client *args);

#endif
