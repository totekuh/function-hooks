#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>


void *ipv4_server_thread(void *arg) {
    int sockfd, newsockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8081);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(sockfd, 5);

    clilen = sizeof(cliaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
    close(newsockfd);
    close(sockfd);

    return NULL;
}


void *uds_server_thread(void *arg) {
    int sockfd, newsockfd;
    struct sockaddr_un servaddr, cliaddr;
    socklen_t clilen;

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sun_family = AF_UNIX;
    strcpy(servaddr.sun_path, "/tmp/test.sock");

    unlink("/tmp/test.sock");  // Remove any existing socket file
    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(sockfd, 5);

    clilen = sizeof(cliaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
    close(newsockfd);
    close(sockfd);

    return NULL;
}

int main() {
    pthread_t ipv4_server, uds_server;
    pthread_create(&ipv4_server, NULL, ipv4_server_thread, NULL);
    pthread_create(&uds_server, NULL, uds_server_thread, NULL);

    sleep(1);  // Wait a bit to ensure the servers are ready

    // IPv4 client
    int ipv4_sockfd;
    struct sockaddr_in ipv4_servaddr;

    ipv4_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&ipv4_servaddr, 0, sizeof(ipv4_servaddr));
    ipv4_servaddr.sin_family = AF_INET;
    ipv4_servaddr.sin_port = htons(8081);
    ipv4_servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    connect(ipv4_sockfd, (struct sockaddr *)&ipv4_servaddr, sizeof(ipv4_servaddr));
    close(ipv4_sockfd);

    // UDS client
    int uds_sockfd;
    struct sockaddr_un uds_servaddr;

    uds_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    memset(&uds_servaddr, 0, sizeof(uds_servaddr));
    uds_servaddr.sun_family = AF_UNIX;
    strcpy(uds_servaddr.sun_path, "/tmp/test.sock");

    connect(uds_sockfd, (struct sockaddr *)&uds_servaddr, sizeof(uds_servaddr));
    close(uds_sockfd);

    pthread_join(ipv4_server, NULL);
    pthread_join(uds_server, NULL);

    return 0;
}
