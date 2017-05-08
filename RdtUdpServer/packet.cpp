#include <math.h>
#include <string.h>

#include "packet.hpp"

queue<DATA_PKT> make_pkts(char *buffer, int buffer_length)
{
    queue<DATA_PKT> pkts;

    int current_buff_length = buffer_length;
    int num_pkts = (buffer_length - DATA_BUFF) <= 0 ? 1 : ceil((double)buffer_length / (double)DATA_BUFF);
    int seq_num = 1;
    int data_len;

    do
    {
        data_len = current_buff_length > DATA_BUFF ? DATA_BUFF : current_buff_length;

        DATA_PKT pkt;
        pkt.cksum = 0xff;
        pkt.hlen = DATA_HEAD_SZ;
        pkt.dlen = data_len;
        pkt.numSeg = num_pkts;
        pkt.seqno = seq_num;

        if(seq_num < num_pkts) {
            strncpy(pkt.data, buffer, data_len);
            pkt.data[data_len] = '\0';
            buffer += data_len;
        } else {
            strcpy(pkt.data, buffer);
        }

        pkts.push(pkt);

        current_buff_length = current_buff_length - DATA_BUFF;
        seq_num++;
    }
    while(seq_num <= num_pkts);

    return pkts;
}

ACK_PKT make_ackpkt(uint32_t ackno, char *buffer)
{
    ACK_PKT pkt;
    pkt.cksum = 0xff;
    pkt.hlen = ACK_HEAD_SZ;
    pkt.ackno = ackno;

    return pkt;
}

bool isSeqExist(deque<DATA_PKT> recv_pkts, DATA_PKT pkt) {
    while(!recv_pkts.empty()) {
        if(recv_pkts.front().seqno == pkt.seqno) {
            return true;
        }
        recv_pkts.pop_front();
    }
    return false;
}

