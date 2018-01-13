#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"
#include "server.h"

int usage(const char *prog)
{
    fprintf(stderr, "Usage:\n\t%s client HOST1:PORT1:[tcp/udp] HOST2:PORT2:[tcp/udp]...\n\t%s server <port>", prog, prog);
    return 1;
}

int main(int argc, char **argv) {
    int mode = REVERSE_MODE_INVALID;

    size_t modelen, maxmodelen = MAX(sizeof(SERVER_STR), sizeof(CLIENT_STR));

    if(argc < 2)
    {
        return usage(argv[0]);
    }

    modelen = strlen(argv[1]);

    if(modelen >= maxmodelen)
    {
        fprintf(stderr, "Invalid\n");
        return usage(argv[0]);
    }

    if(strncmp(CLIENT_STR, argv[1], modelen) == 0)
    {
        if(argc < 3)
        {
            return usage(argv[0]);
        }
        logf("%s\n", "Running in Client mode.");
        mode = REVERSE_MODE_CLIENT;
    }

    if(strncmp(SERVER_STR, argv[1], modelen) == 0)
    {
        logf("%s\n", "Running in Server mode.");
        mode = REVERSE_MODE_SERVER;
    }

    if(mode == REVERSE_MODE_INVALID)
    {
        return usage(argv[0]);
    }

    void *args = reverse_parse_args(argv + 2, (size_t)argc - 2, mode);

    if(args == NULL)
    {
        return usage(argv[0]);
    }

    return mode == REVERSE_MODE_CLIENT ? start_client(args) : start_server(args);
}