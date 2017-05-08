#include "rdt.hpp"
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

/* Initialize a socket for data communication. */
int rdt_socket(int address_family, int type, int protocol)
{
    return socket(address_family, type, protocol);
}

/* Binds a port for data communication. */
int rdt_bind(int socket_descriptor, const struct sockaddr *local_address, socklen_t address_length)
{
    // Set port that the server will use in its packet.
    //struct sockaddr_in *bind_addr = (struct sockaddr_in *) &local_address;
    //server_port = bind_addr->sin_port;
    return bind(socket_descriptor, local_address, address_length);
}


/*char *combinePackets(packet_t pkt_list[], size_t length)
{
    char *combinedPackets = (char *)malloc(length);
    // Need to use c++
    return combinedPackets;
}*/

/* Data is received from a client and a packet is extracted and sent to the application layer */
int rdt_recv(int socket_descriptor, void *buffer, int buffer_length, int flags, struct sockaddr *from_address, socklen_t *address_length)
{
    // 2.0
    /* As buffer comes in packets will need to be put together
     * before sending it to the application layer. Hence, this
     * will need to combine the packets from the same source
     * prior to fully receiving it.
     */
    /*int num_packet, i, p_i;
    size_t packet_sz, dataLength;
    char *combinedPackets[50];*/

    // udt_rcv()
    return recvfrom(socket_descriptor, buffer, buffer_length, flags, from_address, address_length);

    /*num_packet = buffer_length / sizeof(packet_t);
    packet_sz = sizeof(packet_t);

    pkt_hldr_t pkts[100]; // max pkt that it can hold (buffer).

    for(i = 0; i < num_packet; i++)
    {
        // set up packet struct for organization from buffer
        // Extraction is based on struct packet memory portions
        packet_t pkt;
        memcpy(&pkt.id, &buffer[i*packet_sz], sizeof(uint16_t));
        memcpy(&pkt.srcPort, &buffer[i*packet_sz], sizeof(uint16_t));
        memcpy(&pkt.dstPort , &buffer[i*packet_sz*sizeof(uint16_t)], sizeof(uint16_t));
        memcpy(&pkt.cksum, &buffer[i*packet_sz*sizeof(uint16_t)*2], sizeof(uint16_t));
        memcpy(&pkt.len, &buffer[i*packet_sz*sizeof(uint16_t)*3], sizeof(uint16_t));
        memcpy(&pkt.ackno, &buffer[i*packet_sz*sizeof(uint16_t)*4], sizeof(uint32_t));
        memcpy(&pkt.seqno, &buffer[i*packet_sz*sizeof(uint16_t)*4*sizeof(uint32_t)], sizeof(uint32_t));

        dataLength = abs(pkt.len - sizeof(pkt.srcPort) + sizeof(pkt.srcPort)
                             + sizeof(pkt.srcPort) + sizeof(pkt.srcPort)
                             + sizeof(pkt.srcPort) + sizeof(pkt.srcPort));

        memcpy(pkt.data, &buffer[(i*packet_sz*sizeof(uint16_t)*4*sizeof(uint32_t)) + dataLength], dataLength);

        // Set packet to packet list to correct packet map or new map if it does not exist.
        for(p_i = 0; p_i < sizeof(pkts); p_i++)
        {
            if(pkts[i].id = pkt.id)
            {
                // This should be a Queue from c++
                // For now using an arbitrary value.
                pkts[i].pkt_list[0] = pkt;
                break;
            }
            if(sizeof(pkts) - p_i)
            {
               pkts[i + 1].id = pkt.id;
               pkts[i + 1].full_data_sz = pkt.full_data_sz;
               pkts[i + 1].pkt_list[0] = pkt;
            }
        }
    }

    // Use packet identifier to determine when to put the packets together
    // Combine at correct order to complete packet list
    for(i = 0; i < sizeof(pkts); i++)
    {
        if(pkts[i].full_data_sz == sizeof(pkts[i].pkt_list))
        {
            combinedPackets[i] = combinePackets(pkts[i].pkt_list, sizeof(pkts[i].pkt_list));
        }
    }

    int errorCd;

    for(i = 0; i < sizeof(combinedPackets); i++)
    {
        errorCd = recvfrom(socket_descriptor, combinedPackets[i], sizeof(combinedPackets[i]), flags, from_address, address_length);
        if(errorCd < 0) {
            break;
        }
    }

    // rdt_rcv()
    return errorCd;*/
}

/* Data is received from the application layer and packets are created and sent to the source */
int rdt_sendto(int socket_descriptor, void *buffer, int buffer_length, int flags, struct sockaddr *destination_address, socklen_t address_length)
{
   /* int num_pkts = buffer_length / PACKET_SIZE;
    int i, errorCd;
    size_t dataLength;

    packet_t pkts[num_pkts]; // max pkt that it can hold (packets).
    // make_pkts();
    for(i = 0; i < num_pkts; i++)
    {
        packet_t pkt;
        memcpy(&pkt.id, &buffer[i*PACKET_SIZE], sizeof(uint16_t));
        memcpy(&pkt.srcPort, &buffer[i*PACKET_SIZE], sizeof(uint16_t));
        memcpy(&pkt.dstPort , &buffer[i*PACKET_SIZE*sizeof(uint16_t)], sizeof(uint16_t));
        memcpy(&pkt.cksum, &buffer[i*PACKET_SIZE*sizeof(uint16_t)*2], sizeof(uint16_t));
        memcpy(&pkt.len, &buffer[i*PACKET_SIZE*sizeof(uint16_t)*3], sizeof(uint16_t));
        memcpy(&pkt.ackno, &buffer[i*PACKET_SIZE*sizeof(uint16_t)*4], sizeof(uint32_t));
        memcpy(&pkt.seqno, &buffer[i*PACKET_SIZE*sizeof(uint16_t)*4*sizeof(uint32_t)], sizeof(uint32_t));

        dataLength = abs(pkt.len - sizeof(pkt.srcPort) + sizeof(pkt.srcPort)
                             + sizeof(pkt.srcPort) + sizeof(pkt.srcPort)
                             + sizeof(pkt.srcPort) + sizeof(pkt.srcPort));

        memcpy(pkt.data, &buffer[(i*PACKET_SIZE*sizeof(uint16_t)*4*sizeof(uint32_t)) + dataLength], dataLength);

        pkts[i] = pkt;
    }

    for(i = 0; i < num_pkts; i++)
    {
        errorCd = sendto(socket_descriptor, (const void *)&pkts[i], sizeof(pkts[i]), flags, destination_address, address_length);
        if(errorCd < 0) {
            break;
        }
    }

    // udt_send()
    return errorCd;*/
    return sendto(socket_descriptor, buffer, buffer_length, flags, destination_address, address_length);
}

/* Closes the communication socket */
int rdt_close(int fildes)
{
    return close(fildes);
}

