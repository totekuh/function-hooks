#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

void *sender_thread(void *arg) {
    sleep(1);  // Wait a bit to ensure the receiver is ready

    int sockfd;
    struct sockaddr_in servaddr;
    const char *message = "Hello, world!";

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    close(sockfd);

    return NULL;
}

int main() {
    pthread_t sender;
    pthread_create(&sender, NULL, sender_thread, NULL);

    int sockfd;
    struct sockaddr_in servaddr;
    struct msghdr msg;
    struct iovec iov[1];
    char buffer[100];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    iov[0].iov_base = buffer;
    iov[0].iov_len = sizeof(buffer);
    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

    recvmsg(sockfd, &msg, 0);
    buffer[iov[0].iov_len] = '\0';  // Null-terminate the received data
    printf("Received message: %s\n", buffer);

    pthread_join(sender, NULL);
    close(sockfd);

    return 0;
}
