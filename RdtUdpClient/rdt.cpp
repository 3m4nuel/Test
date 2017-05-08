#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <arpa/inet.h>
#include <queue>
#include <algorithm>

#include "rdt.hpp"
#include "packet.hpp"
#include "debugmsg.hpp"

using namespace std;

/* Initialize a socket for data communication. */
int rdt_socket(int address_family, int type, int protocol)
{
    return socket(address_family, type, protocol);
}

/* Binds a port for data communication. */
int rdt_bind(int socket_descriptor, const struct sockaddr *local_address, socklen_t address_length)
{
    return bind(socket_descriptor, local_address, address_length);
}

/* Data is received from a client and a packet is extracted and sent to the application layer */
int rdt_recv(int socket_descriptor, char *buffer, int buffer_length, int flags, struct sockaddr *from_address, int *address_length)
{
    deque<DATA_PKT> recv_pkts;
    int statusCd;
    DATA_PKT recv_pkt;

    // udt_rcv()
    do
    {
        if (( statusCd = recvfrom(socket_descriptor, (void *)&recv_pkt, sizeof(recv_pkt), flags, from_address, (socklen_t *)address_length)) == -1) {
            fprintf(stderr, "Error on transport layer recvfrom, errno = %d (%s) \n", errno, strerror(errno));
            return statusCd;
        }

        displayRcvMsg(recv_pkt.cksum, recv_pkt.hlen, recv_pkt.dlen, recv_pkt.numSeg, recv_pkt.seqno, recv_pkt.data);

        // Only process pkt if it is the next pkt seq no.
        // Time out here, need to get next seq pkt.
        if(!isSeqExist(recv_pkts, recv_pkt)) {
            recv_pkts.push_front(recv_pkt);

            // Send acknowledgement
            ACK_PKT ackPkt = make_ackpkt(recv_pkt.seqno, buffer);

            displaySendAckMsg(ackPkt.cksum, ackPkt.hlen, ackPkt.ackno);

            //((sockaddr_in *)from_address)->sin_addr.s_addr = inet_addr("localhost");
            //((sockaddr_in *)from_address)->sin_port = htons(atoi("8080"));

            int sendStatus;
            if((sendStatus = sendto(socket_descriptor, (void *)&ackPkt, sizeof(ackPkt), flags, from_address, (socklen_t)address_length)) == -1) {
                fprintf(stderr, "Error on transport layer recvfrom while sending ACK through sendto, errno = %d (%s) \n", errno, strerror(errno));
                return sendStatus;
            }
        }
    }
    while(recv_pkts.size() < recv_pkt.numSeg);

    sort(recv_pkts.begin(), recv_pkts.end());

    string combined_pkts = "";
    while(!recv_pkts.empty())
    {
        recv_pkt = recv_pkts.front();
        combined_pkts = combined_pkts + recv_pkt.data;
        recv_pkts.pop_front();
    }

    strcpy(buffer, combined_pkts.data());

    return 0;
}

/* Data is received from the application layer and packets are created and sent to the source */
int rdt_sendto(int socket_descriptor, char *buffer, int buffer_length, int flags, struct sockaddr *destination_address, int address_length)
{
    queue<DATA_PKT> pkts = make_pkts(buffer, buffer_length);
    DATA_PKT sending_pkt;
    int num_sending_pkts = pkts.size();

    while(!pkts.empty())
    {
        sending_pkt = pkts.front();
        pkts.pop();

        displaySendMsg(sending_pkt.cksum, sending_pkt.hlen, sending_pkt.dlen, sending_pkt.numSeg, sending_pkt.seqno, sending_pkt.data);

        int sendStatus = sendto(socket_descriptor, (void *)&sending_pkt, (sending_pkt.hlen + sending_pkt.dlen), flags, destination_address, address_length);

        if(sendStatus == -1)
            return sendStatus;
    }

    int statusCd;
    ACK_PKT ackPkt;

    while(num_sending_pkts > 0)
    {
        // receive acknowledgement
        if (( statusCd = recvfrom(socket_descriptor, (void *)&ackPkt, sizeof(ackPkt), flags, (struct sockaddr *)&destination_address, (socklen_t *)&address_length)) == -1) {
                fprintf(stderr, "Error on transport layer recvfrom, errno = %d (%s) \n", errno, strerror(errno));
            return statusCd;
        }

        displayRcvAckMsg(ackPkt.cksum, ackPkt.hlen, ackPkt.ackno);

        --num_sending_pkts;
    }

    return 0;
}

/* Closes the communication socket */
int rdt_close(int fildes)
{
    return close(fildes);
}
