#include <string.h>

using namespace std;

void displayAppLayerMsg(uint16_t cksum, uint16_t hlen, uint16_t dlen, uint16_t numSeg, uint32_t seqno, char *data)
{
    cout << "Receiving... \n";
    cout << "cksum: " << cksum << "\n";
    cout << "hlen: " << hlen << "\n";
    cout << "dlen: " << dlen << "\n";
    cout << "numSeg: " << numSeg << "\n";
    cout << "seqno: " << seqno << "\n";
    cout << "data: " << data << "\n";
    cout << "\n";
}

void displayRcvMsg(uint16_t cksum, uint16_t hlen, uint16_t dlen, uint16_t numSeg, uint32_t seqno, char *data)
{
    cout << "Receiving... \n";
    cout << "cksum: " << cksum << "\n";
    cout << "hlen: " << hlen << "\n";
    cout << "dlen: " << dlen << "\n";
    cout << "numSeg: " << numSeg << "\n";
    cout << "seqno: " << seqno << "\n";
    cout << "data: " << data << "\n";
    cout << "\n";
}

void displaySendMsg(uint16_t cksum, uint16_t hlen, uint16_t dlen, uint16_t numSeg, uint32_t seqno, char *data)
{
    cout << "Sending... \n";
    cout << "cksum: " << cksum << "\n";
    cout << "hlen: " << hlen << "\n";
    cout << "dlen: " << dlen << "\n";
    cout << "numSeg: " << numSeg << "\n";
    cout << "seqno: " << seqno << "\n";
    cout << "data: " << data << "\n";
    cout << "\n";
}

void displaySendAckMsg(uint16_t cksum, uint16_t hlen, uint32_t ackno)
{
    cout << "Sending ACK...\n";
    cout << "cksum: " << cksum << "\n";
    cout << "len: " << hlen << "\n";
    cout << "ackno: " << ackno << "\n";
    cout << "\n";
}

void displayRcvAckMsg(uint16_t cksum, uint16_t hlen, uint32_t ackno)
{
    cout << "Receiving ACK... \n";
    cout << "cksum: " << cksum << "\n";
    cout << "len: " << hlen << "\n";
    cout << "ackno: " << ackno << "\n";
    cout << "\n";
}
