#include <iostream>
#include <queue>
#include <stdlib.h>

#include "packet.hpp"
#include "debugmsg.hpp"
#include "testcases.hpp"

using namespace std;

/* Test 1: Packet sent successfully */
void success_pktinorder()
{
    cout << "Running test case: SUCCESS_PKTINORDER\n\n";
}

/* Test 2: Packet sent out of order successfully */
void success_pktoutoforder(queue<DATA_PKT> pkts)
{
    if(pkts.size() < 3)
    {
        cout << "Test case requires 3 or more pkts\n";
        exit(1);
    }

    cout << "Running test case: SUCCESS_PKTOUTOFORDER\n";
    cout << "Packets are being reordered...";
    DATA_PKT pkt1 = pkts.front();
    cout << "Packet Seqno. " << pkt1.seqno << " pop.";
    pkts.pop();
    DATA_PKT pkt2 = pkts.front();
    cout << "Packet Seqno. " << pkt2.seqno << " pop.";
    pkts.pop();
    DATA_PKT pkt3 = pkts.front();
    cout << "Packet Seqno. " << pkt3.seqno << " pop.";
    pkts.pop();

    cout << "Packet Seqno. " << pkt2.seqno << " push.";
    pkts.push(pkt2);
    cout << "Packet Seqno. " << pkt3.seqno << " push.";
    pkts.push(pkt3);
    cout << "Packet Seqno. " << pkt1.seqno << " push.";
    pkts.push(pkt1);
}

/* Test 3: Corrupted packet received at receiver */
void error_corruptdata(queue<DATA_PKT> pkts)
{
    cout << "Running test case: ERROR_CORRUPTDATA\n";
    DATA_PKT corruptPkt = pkts.front();
    pkts.pop();
    corruptPkt.cksum = 0xff;
    cout << "Data has been corrupted.\n\n";
    displayPktInfo(corruptPkt);
    pkts.push(corruptPkt);
}

/* Test 4: Packet never received from receiver */
void error_losspkttoreceiver(queue<DATA_PKT> pkts)
{
    cout << "Running test case: ERROR_LOSSPKTTORECEIVER\n";
    cout << "Data has been corrupted.\n";
}

/* Test 5: ACK never received from sender */
void error_lossackpkttosender(ACK_PKT pkt)
{
    cout << "Running test case: ERROR_LOSSACKPKTTOSENDER\n";
    cout << "Transport layer will not process ACK and expect a timeout.";
    displayAckPktInfo(pkt);
}

/* Validate a correct test case has been inputted. */
bool isValidTestCase(char *testCase)
{
    if(!strcmp(testCase, "SUCCESS_PKTINORDER")
        && !strcmp(testCase, "SUCCESS_PKTOUTOFORDER")
        && !strcmp(testCase, "ERROR_CORRUPTACK")
        && !strcmp(testCase, "ERROR_CORRUPTDATA")
        && !strcmp(testCase, "ERROR_LOSSPKTTORECEIVER")
        && !strcmp(testCase, "ERROR_LOSSACKPKTTOSENDER"))
        return false;

    return true;
}
