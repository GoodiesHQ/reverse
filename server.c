#include "server.h"
#include "buffer.h"

static int servers[4];
static int *clients = NULL;
static size_t servers_cnt = 0;
static size_t clients_cnt = 0;

static int sock_tcp4, sock_udp4, sock_tcp6, sock_udp6;

static const char *proto_str(int af, int proto)
{
    switch(af)
    {
    case AF_INET:
        switch(proto)
        {
        case IPPROTO_TCP: return "TVPv4";
        case IPPROTO_UDP: return "UDPv4";
        default: return "???";
        }
    case AF_INET6:
        switch(proto)
        {
        case IPPROTO_TCP: return "TVPv6";
        case IPPROTO_UDP: return "UDPv6";
        default: return "???";
        }
    default: return "???";
    }
}

static int create_server(uint16_t port, int af, int type, int proto)
{
    int sock = -1, optval = 1;
    struct sockaddr_in saddr4;
    struct sockaddr_in6 saddr6;
    struct sockaddr *saddr = NULL;
    size_t saddr_size = 0;

    sock = socket(af, type, proto);
    if(sock < 0)
    {
        warnf("Could not create socket %s\n", proto_str(af, proto));
        return -1;
    }

    switch(af)
    {
    case AF_INET:
        memset(&saddr4, 0, sizeof(saddr4));
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
        set_blocking_mode(sock, false);
        saddr4.sin_family = AF_INET;
        saddr4.sin_port = htons(port);
        saddr = (struct sockaddr*)&saddr4;
        saddr_size = sizeof(saddr4);
        break;

    case AF_INET6:
        memset(&saddr6, 0, sizeof(saddr6));
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
        setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, &optval, sizeof(optval));
        set_blocking_mode(sock, false);
        saddr6.sin6_family = AF_INET6;
        saddr6.sin6_port = htons(port);
        saddr = (struct sockaddr*)&saddr6;
        saddr_size = sizeof(saddr6);
        break;

    default:
        close(sock);
        return -1;
    }


    errno = 0;
    if(bind(sock, saddr, (socklen_t)saddr_size) < 0)
    {
        close(sock);
        warnf("Could not bind socket %s\n", proto_str(af, proto));
        return -1;
    }

    if(proto == IPPROTO_TCP)
    {
        errno = 0;
        if(listen(sock, 10) < 0)
        {
            close(sock);
            warnf("Could not listen on socket %s: %s\n", proto_str(af, proto), strerror(errno));
            return -1;
        }
    }

    logf("Started %s\n", proto_str(af, proto));
    return sock;
}

int start_server(args_server *args)
{
    logf("Running on localhost:%" PRIu16 "\n", args->port);

    memset(servers, 0, sizeof(servers));
    servers_cnt = 0;

    free(clients);
    clients = malloc(sizeof(int) * 1024);
    clients_cnt = 0;

    const int optval = 1;

    servers_cnt += ((servers[servers_cnt] = (sock_tcp4 = create_server(args->port, AF_INET, SOCK_STREAM, IPPROTO_TCP))) != -1);
    servers_cnt += ((servers[servers_cnt] = (sock_tcp6 = create_server(args->port, AF_INET6, SOCK_STREAM, IPPROTO_TCP))) != -1);
    servers_cnt += ((servers[servers_cnt] = (sock_udp4 = create_server(args->port, AF_INET, SOCK_DGRAM, IPPROTO_UDP))) != -1);
    servers_cnt += ((servers[servers_cnt] = (sock_udp6 = create_server(args->port, AF_INET6, SOCK_DGRAM, IPPROTO_UDP))) != -1);

    logf("Started %zu/4 servers.\n", servers_cnt);

    while(true)
    {
        /*
         * server polling for inbound connections from a reverse client
         */
    }

    return 0;
}
