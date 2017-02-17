#pragma once

#include <queue>
#include <windows.h>
#include "Packet.h"

using namespace std;

class DataLink
{
private:
	CRITICAL_SECTION queue_mutex;
	queue<Packet> pktQ;
public:
	DataLink(void) {
		InitializeCriticalSection(&queue_mutex);
	}
	~DataLink(void) {}

	void enqueue(Packet pkt) {
		EnterCriticalSection(&queue_mutex);
		pktQ.push(pkt);
		LeaveCriticalSection(&queue_mutex);
	}

	void dequeue() {
		EnterCriticalSection(&queue_mutex);

		//queue가 비어(empty)있으면 아무런 기능을 수행하지 않는다.
		if (pktQ.empty()) {
			LeaveCriticalSection(&queue_mutex);
			return;
		}
		else {
			pktQ.pop();
		}
		LeaveCriticalSection(&queue_mutex);
	}

	void front(Packet* pPkt) {
		EnterCriticalSection(&queue_mutex);

		//queue가 비어(empty)있으면 pPkt에 NULL을 저장한다.
		if (pktQ.empty()) {
			pPkt = NULL;
			LeaveCriticalSection(&queue_mutex);
			return;
		}
		*pPkt = pktQ.front();
		LeaveCriticalSection(&queue_mutex);
	}

	bool empty() {
		int size = pktQ.size();
		if (size == 0)
			return true;
		else
			return false;
	}
};
