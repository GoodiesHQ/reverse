#ifndef REVERSE_SERVER_H
#define REVERSE_SERVER_H

#include "reverse.h"

typedef struct {
    uint16_t port;
} args_server;

int start_server(args_server *args);

#endif
