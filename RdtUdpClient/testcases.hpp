#ifndef TESTCASES_HPP_INCLUDED
#define TESTCASES_HPP_INCLUDED

#include <queue>

#include "packet.hpp"
#include "debugmsg.hpp"

using namespace std;

/* Test 1: Packet sent successfully */
void success_pktinorder();

/* Test 2: Packet sent out of order successfully */
void success_pktoutoforder(queue<DATA_PKT> pkts);

/* Test 3: Corrupted ACK received at sender */
void error_corruptack(ACK_PKT *ackpkt);

/* Test 4: Corrupted packet received at receiver */
void error_corruptdata(queue<DATA_PKT> pkts);

/* Test 5: Packet never received from receiver */
void error_losspkttoreceiver(queue<DATA_PKT> pkts);

/* Test 6: ACK never received from sender */
void error_lossackpkttosender(ACK_PKT pkt);

/* Validate a correct test case has been inputted. */
bool isValidTestCase(char *testCase);

#endif // TESTCASES_HPP_INCLUDED
