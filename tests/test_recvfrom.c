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
    const char *message = "Hello from recvfrom!";

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8081);
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
    char buffer[100];
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8081);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, &len);
    buffer[len] = '\0';  // Null-terminate the received data
    printf("Received message: %s\n", buffer);

    pthread_join(sender, NULL);
    close(sockfd);

    return 0;
}
