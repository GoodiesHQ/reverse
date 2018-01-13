#include "server.h"
#include "buffer.h"
#include <unistd.h>

static int *clients = NULL;
static size_t clients_cnt = 0;

static int sock_tcp4, sock_udp4, sock_tcp6, sock_udp6;

int start_server(args_server *args)
{
    logf("Running on localhost:%" PRIu16 "\n", args->port);

    const int optval = 1;
    struct sockaddr_in saddr_tcp4, saddr_udp4;
    struct sockaddr_in6 saddr_tcp6, saddr_udp6;

    memset(&saddr_tcp4, 0, sizeof(saddr_tcp4));
    memset(&saddr_udp4, 0, sizeof(saddr_udp4));
    memset(&saddr_tcp6, 0, sizeof(saddr_tcp6));
    memset(&saddr_udp6, 0, sizeof(saddr_udp6));

    sock_tcp4 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sock_udp4 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sock_tcp6 = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    sock_udp6 = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);

    if( sock_tcp4 == -1 ||
        sock_udp4 == -1 ||
        sock_tcp6 == -1 ||
        sock_udp6 == -1 )
    {
        logf("%s\n", "sockets could not be created.");
        return 1;
    }

    // TODO: error checking for all of these
    setsockopt(sock_tcp4, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    setsockopt(sock_udp4, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    setsockopt(sock_tcp6, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    setsockopt(sock_udp6, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    // explicitly use IPv6 for highest amount of portability (and annoyance)
    setsockopt(sock_tcp6, IPPROTO_IPV6, IPV6_V6ONLY, &optval, sizeof(optval));
    setsockopt(sock_udp6, IPPROTO_IPV6, IPV6_V6ONLY, &optval, sizeof(optval));

    set_blocking_mode(sock_tcp4, false);
    set_blocking_mode(sock_udp4, false);
    set_blocking_mode(sock_tcp6, false);
    set_blocking_mode(sock_udp6, false);

    saddr_tcp4.sin_family = AF_INET;
    saddr_udp4.sin_family = AF_INET;
    saddr_tcp6.sin6_family = AF_INET6;
    saddr_udp6.sin6_family = AF_INET6;

    saddr_tcp4.sin_port = htons(args->port);
    saddr_udp4.sin_port = htons(args->port);
    saddr_tcp6.sin6_port = htons(args->port);
    saddr_udp6.sin6_port = htons(args->port);

    if(bind(sock_tcp4, (struct sockaddr*)&saddr_tcp4, sizeof(saddr_tcp4)) < 0)
    {
        warnf("%s\n", "Could not listen on TVPv4");
        close(sock_tcp4);
        sock_tcp4 = -1;
    }
    listen(sock_tcp4, 10);

    if(bind(sock_udp4, (struct sockaddr*)&saddr_udp4, sizeof(saddr_udp4)) < 0)
    {
        warnf("%s\n", "Could not listen on UDPv4");
        close(sock_udp4);
        sock_udp4 = -1;
    }
    listen(sock_udp4, 10);

    if(bind(sock_tcp6, (struct sockaddr*)&saddr_tcp6, sizeof(saddr_tcp6)) < 0)
    {
        warnf("%s\n", "Could not listen on TCPv6");
        close(sock_tcp6);
        sock_tcp6 = -1;
    }
    listen(sock_tcp6, 10);

    if(bind(sock_udp6, (struct sockaddr*)&saddr_udp6, sizeof(saddr_udp6)) < 0)
    {
        warnf("%s\n", "Could not listen on UDPv6");
        close(sock_udp6);
        sock_udp6 = -1;
    }
    listen(sock_udp6, 10);

    sleep(1000);

    return 0;
}
