#define _GNU_SOURCE // Enable GNU extensions
#include "common.h"
#include <stdio.h>
#include <dlfcn.h>
#include <sys/socket.h>

static ssize_t (*original_recvmsg)(int sockfd, struct msghdr *msg, int flags) = NULL;

ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags) {
    if (!original_recvmsg) {
        original_recvmsg = dlsym(RTLD_NEXT, "recvmsg");
    }

    printf("[ :::::::::::::: Start of recvmsg Hook :::::::::::::: ]\n");

    ssize_t ret = original_recvmsg(sockfd, msg, flags);

    // Log the hook
    printf("HOOK: recvmsg hooked!\n");
    print_call_stack();

    // Log the data received
    if (ret > 0 && msg->msg_iov && msg->msg_iov->iov_base) {
        printf("HOOK: Received data (recvmsg) in HEX: ");
        print_hex_data(msg->msg_iov->iov_base, ret);
        printf("HOOK: Received data (recvmsg) in ASCII: ");
        print_ascii_data(msg->msg_iov->iov_base, ret);
    }

    printf("[ :::::::::::::: End of recvmsg Hook :::::::::::::: ]\n");

    return ret;
}
