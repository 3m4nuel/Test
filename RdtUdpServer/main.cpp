#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "rdt.hpp"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3) {
       fprintf(stderr, "%s <ip-address> <port>\n", argv[0]);
       return -1;
    }

    /* Create the socket */
    int sockfd = rdt_socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
       fprintf(stderr, "%s : cannot create socket, errno = %d, (%s) \n", argv[0],
               errno, strerror(errno));
       return -1;
    }

    struct sockaddr_in saddr;
    memset((void *)&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(argv[1]);
    saddr.sin_port = htons(atoi(argv[2]));

    /* bind service to a port */
    if (rdt_bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr)) == -1) {
        fprintf(stderr, "%s: unable to bind to port '%s', errno = %d (%s) \n", argv[0], argv[2], errno, strerror(errno));
        rdt_close(sockfd);
        return -1;
    }

    char sumData[500];
    int slen;
    struct sockaddr_in caddr;
    while (1) {
        bzero(sumData, 500);
        /* Waiting for a join request from a client */
        slen = sizeof(caddr);
        if (rdt_recv(sockfd, sumData, 500, 0, (struct sockaddr *)&caddr, &slen) == -1) {
            fprintf(stderr, "%s: error on recvfrom, errno = %d (%s) \n", argv[0], errno, strerror(errno));
            continue;
        }

        cout << "Application Layer receives: " << sumData << "\n";
        cout << "\n";

        /* Send response to client that message was received. */
       /* string testSend("Server received msg.");
        if (rdt_sendto(sockfd, (char *)testSend.c_str(), testSend.length(), 0, (struct sockaddr *)&caddr, slen) == -1) {
            fprintf(stderr, "Error sending to client, errno = %d (%s) \n",
            errno, strerror(errno));
        }*/
    }

    return 0;
}
