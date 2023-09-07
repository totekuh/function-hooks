#define _GNU_SOURCE
#include <dlfcn.h>
#include "common.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <errno.h>


static int (*original_connect)(int sockfd, const struct sockaddr *addr, socklen_t addrlen) = NULL;

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    if (!original_connect) {
        original_connect = dlsym(RTLD_NEXT, "connect");
    }

    printf("[ :::::::::::::: Start of connect Hook :::::::::::::: ]\n");

    // Log the hook
    printf("HOOK: connect hooked!\n");

    // Log the address and port being connected to based on the socket family
    if (addr->sa_family == AF_INET) {
        struct sockaddr_in *addr_in = (struct sockaddr_in *)addr;
        printf("HOOK: Connecting to IPv4 address: %s, Port: %d\n", inet_ntoa(addr_in->sin_addr), ntohs(addr_in->sin_port));
    } else if (addr->sa_family == AF_INET6) {
        struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)addr;
        char ipstr[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &addr_in6->sin6_addr, ipstr, sizeof(ipstr));
        printf("HOOK: Connecting to IPv6 address: %s, Port: %d\n", ipstr, ntohs(addr_in6->sin6_port));
    } else if (addr->sa_family == AF_UNIX) {
        struct sockaddr_un *addr_un = (struct sockaddr_un *)addr;
        printf("HOOK: Connecting to UNIX socket: %s\n", addr_un->sun_path);
    } else {
        printf("HOOK: Connecting to unknown socket family: %d\n", addr->sa_family);
    }

    int ret = original_connect(sockfd, addr, addrlen);

    // Log the result
    if (ret == 0) {
        printf("HOOK: Connection successful.\n");
    } else {
        // Check the errno value
        if (errno == EINPROGRESS) {
            printf("HOOK: Connection in progress (non-blocking).\n");
        } else {
            printf("HOOK: Connection failed, errno: %d.\n", errno);
        }
    }

    printf("[ :::::::::::::: End of connect Hook :::::::::::::: ]\n");

    return ret;
}
