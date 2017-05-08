#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include "rdt.hpp"

using namespace std;


int main(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <ip_address> <port> <msg>\n", argv[0]);
        return -1;
    }

    /* Open a UCP based socket connection */
    int sockfd;
    sockfd = rdt_socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "Error opening socket, errno = %d (%s) \n",
                errno, strerror(errno));
        return -1;
    }

    int broadcastEnable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) == -1) {
        fprintf(stderr, "Error sending socket to broadcast mode, errno = %d (%s) \n", errno, strerror(errno));
        rdt_close(sockfd);
        return -1;
    }

    char* addr = argv[1];
    char* port = argv[2];
    struct sockaddr_in saddr;
    memset((void *)&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(addr);
    saddr.sin_port = htons(atoi(port));

    char *msg = argv[3];
    char sumData[500];
    bzero(sumData, 500);

    if (rdt_sendto(sockfd, msg, strlen(msg), 0,(struct sockaddr *) &saddr, sizeof(saddr)) == -1) {
        fprintf(stderr, "Error sending udp pkt to server, errno = %d (%s) \n", errno, strerror(errno));
        rdt_close(sockfd);
        return -1;
    }

    /* Wait for response from server (get time) */
    /*int clen;
    if (rdt_recv(sockfd, sumData, 500, 0, (struct sockaddr *)&saddr, &clen) == -1) {
        fprintf(stderr, "Error receiving udp pkt from server, errno = %d (%s) \n", errno, strerror(errno));
        close(sockfd);
        return -1;
    }*/

   // cout << "Application Layer receives: " << sumData << "\n";
   // cout << "\n";

    rdt_close(sockfd);

    return 0;
}
