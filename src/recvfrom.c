#define _GNU_SOURCE // Enable GNU extensions
#include <dlfcn.h>  // Must come after _GNU_SOURCE
#include "common.h"
#include <sys/socket.h>

static ssize_t (*original_recvfrom)(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen) = NULL;

ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen) {
    if (!original_recvfrom) {
        original_recvfrom = dlsym(RTLD_NEXT, "recvfrom");
    }

    printf("[ :::::::::::::: Start of recvfrom Hook :::::::::::::: ]\n");

    ssize_t ret = original_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);

    // Log the hook
    printf("HOOK: recvfrom hooked!\n");
    print_call_stack();

    // Log the data received
    if (ret > 0) {
        printf("HOOK: Received data (recvfrom) in HEX: ");
        print_hex_data(buf, ret);
        printf("HOOK: Received data (recvfrom) in ASCII: ");
        print_ascii_data(buf, ret);
    }

    printf("[ :::::::::::::: End of recvfrom Hook :::::::::::::: ]\n");

    return ret;
}


