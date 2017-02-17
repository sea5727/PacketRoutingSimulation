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

		//queue�� ���(empty)������ �ƹ��� ����� �������� �ʴ´�.
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

		//queue�� ���(empty)������ pPkt�� NULL�� �����Ѵ�.
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
