#pragma once

#include <iostream>
#include <fstream>

#define MAX_HOP_COUNT	16

typedef unsigned int UINT_32;
typedef unsigned short UINT_16;
typedef unsigned char UINT_8;

using namespace std;

class Packet
{
public:
	Packet(UINT_32 srcAddr, UINT_32 dstAddr, UINT_32 sN)
		: srcAddr(srcAddr), dstAddr(dstAddr), seqNo(sN)
	{
		hop_count = 0;
	}
	Packet() {}

	UINT_32 getSrcAddr() { return srcAddr; }
	UINT_32 getDstAddr() { return dstAddr; }
	UINT_16 getProtocol() { return protocol; }
	UINT_32 getSeqNo() { return seqNo; }
	UINT_32 getHopCount() { return hop_count; }
	UINT_32 getRouteNodes(int h) { return route_nodes[h]; }

	void setSrcAddr(UINT_32 sA) { srcAddr = sA; }
	void setDstAddr(UINT_32 dA) { dstAddr = dA; }
	void setProtocol(UINT_16 proto) { protocol = proto; }
	void setSeqNo(UINT_32 sN) { seqNo = sN; }
	void setHopCount(UINT_32 hc) { hop_count = hc; }
	void setRouteNodes(int h, int rn) { route_nodes[h] = rn; }
	void pushRouteNode(int rn) { route_nodes[hop_count++] = rn; }

	ostream& printSaddrSeqNo(ostream *pFout);
	ostream& printPacket(ostream *pFout);

private:
	UINT_32 srcAddr;
	UINT_32 dstAddr;
	UINT_16 protocol;
	UINT_32 seqNo;
	UINT_32 payloadLength;
	int hop_count;
	int route_nodes[MAX_HOP_COUNT];
};
