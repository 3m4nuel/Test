#ifndef PACKET_H_INCLUDED
#define PACKET_H_INCLUDED

#include <stdint.h>
#include <queue>

#define DATA_HEAD_SZ 96
#define ACK_HEAD_SZ 64
#define DATA_BUFF 5

using namespace std;

struct DATA_PKT {
    uint16_t cksum;
    uint16_t hlen; // header length
    uint16_t dlen; // data length
    uint16_t numSeg; // Number of segments
    uint32_t seqno;
    char data[DATA_BUFF];
    bool operator<(const DATA_PKT& rhs) const
    {
        return seqno < rhs.seqno;
    }

    bool operator==(const DATA_PKT& pkt) const
    {
        return seqno == pkt.seqno;
    }
};

struct ACK_PKT {
    uint16_t cksum;
    uint16_t hlen;  // header length
    uint32_t ackno;
};

queue<DATA_PKT> make_pkts(char *buffer, int buffer_length);

ACK_PKT make_ackpkt(uint32_t ackno, char *buffer);

bool isSeqExist(deque<DATA_PKT> recv_pkts, DATA_PKT pkt);

#endif
