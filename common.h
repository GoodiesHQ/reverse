#ifndef REVERSE_COMMON_H
#define REVERSE_COMMON_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


#ifdef DEBUG
#define debugf(fmt, ...) fprintf(stderr, "%s:%d:%s(): %s" fmt, __FILE__, __LINE__, __func__, __VA_ARGS__)
#else
#define debugf(...)
#endif

#define logf(fmt, ...) fprintf(stdout, "[reverse] " fmt, __VA_ARGS__)

#define CLIENT_STR "client"
#define SERVER_STR "server"

#define MIN(a, b) (a) < (b) ? (a) : (b)
#define MAX(a, b) (a) > (b) ? (a) : (b)

/*
 * Determines the mode in which the process runs
 */
enum {
    REVERSE_MODE_CLIENT,
    REVERSE_MODE_SERVER,
    REVERSE_MODE_INVALID = -1,
};

/*
 * Tasks used by client/server
 */
typedef struct
{
    struct addrinfo hints;      // address information about the forwarding socket
    union {
        struct in_addr ipv4;    // if the AF is IPv4, use this.
        struct in6_addr ipv6;   // if the AF is IPv6, use this.
    };
    uint16_t port;              // for servers, determines the listening port.
    int proto;                  // the protocol used (TCP/UDP only)
    int sock;                   // the forwarding socket object
} reverse_task;

#endif