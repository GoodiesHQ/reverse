#include "reverse.h"
#include "client.h"
#include "server.h"

bool set_blocking_mode(int sock, bool is_blocking)
{
    bool ret = true;

#ifdef WIN32
    unsigned long non_blocking = is_blocking ? 0 : 1;
    ret = NO_ERROR == ioctlsocket(sock, FIONBIO, &non_blocking);
#else
    const int flags = fcntl(sock, F_GETFL, 0);

    if (
            ((flags & O_NONBLOCK) && !is_blocking) ||
            (!(flags & O_NONBLOCK) && is_blocking)) {
        return ret;
    }

    ret = 0 == fcntl(sock, F_SETFL, is_blocking ? flags ^ O_NONBLOCK : flags | O_NONBLOCK);
#endif

    return ret;
}

void *reverse_parse_args(char **opts, size_t opts_cnt, int mode)
{
    if(mode == REVERSE_MODE_CLIENT)
    {
        args_client *args;
        size_t i, j, len;
        char **targets = malloc(sizeof(char*) * opts_cnt);

        memset(targets, 0, sizeof(char*) * opts_cnt);

        for(i = 0; i < opts_cnt; ++i)
        {
            char *target = targets[i];
            len = strlen(target);
            char *target_end = &target[len - 1];

            while(*target_end != ':' && target_end != target)
            {
                --target_end;
            }
        }
    }

    if(mode == REVERSE_MODE_SERVER)
    {
        args_server *args;
        const char *port = opts[0];

        if((args = malloc(sizeof(args_server))) == NULL)
        {
            debugf("%s\n", "could not allocate memory.");
            return NULL;
        }
        memset(args, 0, sizeof(args_server));

        if(port == NULL || opts_cnt == 0)
        {
            args->port = REVERSE_DEFAULT_PORT;
        }
        else
        {
            errno = 0;
            uintmax_t value = strtoumax(port, NULL, 10);
            if(errno || value == 0 || value > 65535)
            {
                logf("'%s' is an invalid port\n", port);
                free(args);
                return NULL;
            }
            args->port = (uint16_t)value;
        }
        return args;
    }
    return NULL;
}
