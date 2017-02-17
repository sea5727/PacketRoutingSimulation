#include <iostream>
#include <fstream>
#include <iomanip>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <cstdlib>
#include <string>
#include "Packet.h"
#include "DataLink.h"
#include "Graph.h"
#include "BreadthFirstSearch.h"

#define NUM_NODES 20
#define NUM_EDGES 70

typedef Graph::Vertex Vertex;
typedef Graph::Edge Edge;
typedef std::list<Graph::Vertex> VtxList;
typedef std::list<Graph::Edge> EdgeList;
typedef std::list<Graph::Vertex>::iterator VtxItor;
typedef std::list<Graph::Edge>::iterator EdgeItor;

using namespace std;
#define MAX_QUEUE_SIZE 10

enum ROLE { PACKET_ROUTER, PACKET_GEN, PACKET_RECV, LINK_TX, LINK_RX };
DWORD WINAPI Thread_PacketRouter(LPVOID pParam);

typedef struct ThreadParam
{
	int id;
	CRITICAL_SECTION *pCS;
	DataLink ***pppDL;
	BreadthFirstSearch* pNetTopology;
	int num_queue;
	int role; //
	UINT_32 addr;
	int max_queue;
	int* pNum_total_received;
	int *pTotal_packet_TX;
	ostream *pFout;
	ThreadParam() {}
} ThreadParam;

void main()
{
	int total_packet_TX = 0;
	int num_nodes, numDataLinks;
	static int num_total_received = 0; // num of packets received by all routers

	Vertex v[NUM_NODES] = {
		Vertex("Seattle", 0, UNEXPLORED),
		Vertex("SanFrancisco", 1, UNEXPLORED),
		Vertex("LosAngels", 2, UNEXPLORED),
		Vertex("Salt Lake City", 3, UNEXPLORED),
		Vertex("Rapid City", 4, UNEXPLORED),
		Vertex("Denver", 5, UNEXPLORED),
		Vertex("Phoenix", 6, UNEXPLORED),
		Vertex("Minneapolis", 7, UNEXPLORED),
		Vertex("Dallas", 8, UNEXPLORED),
		Vertex("Houston", 9, UNEXPLORED),
		Vertex("New Orleans", 10, UNEXPLORED),
		Vertex("Memphis", 11, UNEXPLORED),
		Vertex("St.Louis", 12, UNEXPLORED),
		Vertex("Chicago", 13, UNEXPLORED),
		Vertex("Detroit", 14, UNEXPLORED),
		Vertex("Miami", 15, UNEXPLORED),
		Vertex("Atlanta", 16, UNEXPLORED),
		Vertex("Washington D.C.", 17, UNEXPLORED),
		Vertex("New York", 18, UNEXPLORED),
		Vertex("Boston", 19, UNEXPLORED)
	};

	Graph::Edge edges[NUM_EDGES] = {
		Edge(v[0], v[1], 820), Edge(v[1], v[0], 820),
		Edge(v[0], v[3], 828), Edge(v[3], v[0], 828),
		Edge(v[0], v[4], 1144), Edge(v[4], v[0], 1144),
		Edge(v[1], v[2], 380), Edge(v[2], v[1], 380),
		Edge(v[1], v[3], 745), Edge(v[3], v[1], 745),
		Edge(v[2], v[3], 688), Edge(v[3], v[2], 688),
		Edge(v[2], v[6], 381), Edge(v[6], v[2], 381),
		Edge(v[3], v[4], 657), Edge(v[4], v[3], 657),
		Edge(v[3], v[5], 521), Edge(v[5], v[3], 521),
		Edge(v[4], v[5], 389), Edge(v[4], v[5], 389),
		Edge(v[4], v[7], 611), Edge(v[7], v[4], 611),
		Edge(v[5], v[6], 816), Edge(v[6], v[5], 816),
		Edge(v[5], v[7], 920), Edge(v[7], v[5], 920),
		Edge(v[5], v[8], 780), Edge(v[8], v[5], 780),
		Edge(v[5], v[12], 861), Edge(v[12], v[5], 861),
		Edge(v[6], v[8], 1067), Edge(v[8], v[6], 1067),
		Edge(v[7], v[13], 409), Edge(v[13], v[7], 409),
		Edge(v[8], v[9], 246), Edge(v[9], v[8], 246),
		Edge(v[8], v[11], 454), Edge(v[11], v[8], 454),
		Edge(v[9], v[10], 352), Edge(v[10], v[9], 352),
		Edge(v[10], v[11], 393), Edge(v[11], v[10], 393),
		Edge(v[10], v[15], 861), Edge(v[15], v[10], 861),
		Edge(v[10], v[16], 473), Edge(v[16], v[10], 473),
		Edge(v[11], v[12], 285), Edge(v[12], v[11], 285),
		Edge(v[11], v[16], 394), Edge(v[16], v[11], 394),
		Edge(v[12], v[13], 297), Edge(v[13], v[12], 297),
		Edge(v[12], v[17], 845), Edge(v[17], v[12], 845),
		Edge(v[13], v[14], 286), Edge(v[14], v[13], 286),
		Edge(v[14], v[17], 534), Edge(v[17], v[14], 534),
		Edge(v[14], v[18], 640), Edge(v[18], v[14], 640),
		Edge(v[14], v[19], 834), Edge(v[19], v[14], 834),
		Edge(v[15], v[16], 661), Edge(v[16], v[15], 661),
		Edge(v[16], v[17], 632), Edge(v[17], v[16], 632),
		Edge(v[17], v[18], 237), Edge(v[18], v[17], 237),
		Edge(v[18], v[19], 211), Edge(v[19], v[18], 211)
	};

	Graph simpleGraph(NUM_NODES);
	cout << "Inserting vertices .." << endl;
	for (int i = 0; i<NUM_NODES; i++) {
		simpleGraph.insertVertex(v[i]);
	}

	VtxList vtxLst;
	simpleGraph.vertices(vtxLst);
	int count = 0;
	cout << "Inserted vertices: ";
	for (VtxItor vItor = vtxLst.begin(); vItor != vtxLst.end(); ++vItor) {
		cout << *vItor << ", ";
	}
	cout << endl;

	cout << "Inserting edges .." << endl;
	for (int i = 0; i<NUM_EDGES; i++)
	{
		simpleGraph.insertEdge(edges[i]);
	}

	cout << "Inserted edges: " << endl;
	count = 0;
	EdgeList egLst;
	simpleGraph.edges(egLst);
	for (EdgeItor p = egLst.begin(); p != egLst.end(); ++p)
	{
		count++;
		cout << *p << ", ";
		if (count % 5 == 0)
			cout << endl;
	}
	cout << endl;

	cout << "Print out Graph based on Adjacency List .." << endl;
	simpleGraph.printGraph();
	num_nodes = simpleGraph.getNumVertices();
	numDataLinks = egLst.size();
	/* ======================================= ===*/
	BreadthFirstSearch networkTopology(simpleGraph);
	DataLink ***pppDL;
	int **ppDistMtrx;
	ThreadParam *pThrParam, threadParam;
	CRITICAL_SECTION crit;
	HANDLE hThreadPktRouter[NUM_NODES];

	networkTopology.initialize();
	networkTopology.initDistMtrx();

	pppDL = new DataLink**[num_nodes];
	for (int i = 0; i<num_nodes; i++) {
		pppDL[i] = new DataLink*[num_nodes];
	}

	ppDistMtrx = networkTopology.getppDistMtrx();
	for (int i = 0; i<num_nodes; i++) {
		for (int j = 0; j<num_nodes; j++)
		{
			if (i == j) {
				pppDL[i][j] = new DataLink();
			}
			else if (ppDistMtrx[i][j] == PLUS_INF) {
				pppDL[i][j] = NULL;
			}
			else {
				pppDL[i][j] = new DataLink();
			}
		}
	}

	InitializeCriticalSection(&crit);
	EnterCriticalSection(&crit);
	cout << "==== Generating " << num_nodes << " threads ... ==== " << endl;
	for (int node_id = 0; node_id<num_nodes; node_id++)
	{
		pThrParam = new ThreadParam;
		pThrParam->id = node_id;
		pThrParam->addr = (UINT_32)node_id;
		pThrParam->pNetTopology = new BreadthFirstSearch(simpleGraph);
		pThrParam->role = PACKET_ROUTER;
		pThrParam->pCS = &crit;
		pThrParam->pppDL = pppDL;
		pThrParam->max_queue = MAX_QUEUE_SIZE;
		pThrParam->pFout = &cout;
		pThrParam->pNum_total_received = &num_total_received;
		hThreadPktRouter[node_id] =
			CreateThread(NULL, 0, Thread_PacketRouter, pThrParam, 0, NULL);
	}
	LeaveCriticalSection(&crit);
	EnterCriticalSection(&crit);

	cout << "\n====== main() routing is waiting threads to terminate ... " << endl;
	LeaveCriticalSection(&crit);
	DWORD nExitCode = NULL;
	for (int i = 0; i<num_nodes; i++)
	{
		WaitForSingleObject(hThreadPktRouter[i], INFINITE);
		GetExitCodeThread(hThreadPktRouter[i], &nExitCode);
		TerminateThread(hThreadPktRouter[i], nExitCode);
		CloseHandle(hThreadPktRouter[i]);
	}
	DeleteCriticalSection(&crit);
}

DWORD WINAPI Thread_PacketRouter(LPVOID pParam)
{
	ThreadParam *pThrParam;
	BreadthFirstSearch* pNetTopology;
	DataLink ***pppDL; // pppDL[u][v] indicates the address of DataLink[u][v]
	Vertex *pVrtx, *pMyVrtx;
	int myNetAddr;
	Packet *pPkt = NULL, newPkt;
	DataLink* pDL;
	UINT_32 srcAddr;
	ostream *pFout;

	int next_node, dst;
	int *pParent;
	int *pForwardTable;
	int num_nodes;
	int num_packets_generated = 0;
	int *pTotal_received;

	pThrParam = (ThreadParam *)pParam;
	myNetAddr = pThrParam->addr;
	pNetTopology = pThrParam->pNetTopology;
	pFout = pThrParam->pFout;
	pppDL = pThrParam->pppDL;
	pTotal_received = pThrParam->pNum_total_received;

	EnterCriticalSection(pThrParam->pCS);
	cout << " ******* Thread_PacketRouter [" << myNetAddr << "] *******" << endl;
	LeaveCriticalSection(pThrParam->pCS);

	srand(time(0) + pThrParam->addr);
	pNetTopology->initialize();
	pNetTopology->initDistMtrx();
	pVrtx = pNetTopology->getGraph().getpVrtxArray();
	pMyVrtx = &(pVrtx[myNetAddr]);
	pNetTopology->ShortestPathsTree(*pMyVrtx);
	pParent = pNetTopology->getpParent();
	num_nodes = pNetTopology->getGraph().getNumVertices();
	// initialize packet forwarding table

	EnterCriticalSection(pThrParam->pCS);
	pForwardTable = new int[num_nodes];
	for (dst = 0; dst<num_nodes; dst++)
	{
		int nxt;
		nxt = dst;
		while (pParent[nxt] != myNetAddr) {
			nxt = pParent[nxt];
		}
		if (pParent[nxt] == myNetAddr) {
			pForwardTable[dst] = nxt;
		}
		else {
			cout << "Error in packet forwarding : destination (" << dst;
			cout << ") is not reachable from this node (" << myNetAddr << "!!" << endl;
		}
	} // end for
	LeaveCriticalSection(pThrParam->pCS);

	// packet generations as source node
	for (int i = 0; i < num_nodes; i++)
	{
		dst = i % num_nodes;
		pPkt = new Packet(myNetAddr, dst, i);
		pPkt->pushRouteNode(myNetAddr); // record route
		next_node = pForwardTable[dst];

		EnterCriticalSection(pThrParam->pCS);
		DataLink* pDL = pThrParam->pppDL[myNetAddr][next_node];
		if (pDL != NULL)
			pDL->enqueue(*pPkt);
		else
			cout << " Error : pppDL[][] is NULL" << endl;

		*pFout << "Router[" << myNetAddr << "]: (";
		*pFout << num_packets_generated << ")-th packet (";
		*pFout << pPkt->getSrcAddr() << " -> " << pPkt->getDstAddr();
		*pFout << ", seqNo: " << pPkt->getSeqNo() << ")\n";
		LeaveCriticalSection(pThrParam->pCS);

		num_packets_generated++;
		Sleep(1);
	} // end for( ; ; ) for generation of packets

	EnterCriticalSection(pThrParam->pCS);
	cout << "=== Router[" << myNetAddr << "] generated total ";
	cout << num_packets_generated << " packets " << endl;
	LeaveCriticalSection(pThrParam->pCS);
	// packet forwarding as transit node, and packet processing as destination node

	int source;
	int total_processed = 0;
	int received_by_this_node = 0;
	int hop_count;

	while ((*pTotal_received < (num_nodes * num_nodes)) || (received_by_this_node < num_nodes))
	{
		DataLink* _pDL;
		for (int i = 0; i<num_nodes; i++) {
			EnterCriticalSection(pThrParam->pCS);
			_pDL = pppDL[i][myNetAddr];
			if (_pDL != NULL && !(_pDL->empty())) {
				_pDL->front(pPkt);
				_pDL->dequeue();

				pPkt->pushRouteNode(myNetAddr);
				next_node = pForwardTable[pPkt->getDstAddr()];

				if ((pPkt->getDstAddr()) != myNetAddr) {
					_pDL = pppDL[myNetAddr][next_node];
					_pDL->enqueue(*pPkt);
					*pFout << "  Router[" << myNetAddr << "] forwarded a packet from ["
						<< pPkt->getSrcAddr() << "] to [" << pPkt->getDstAddr() << "];";
					*pFout << "  this hop (" << myNetAddr << "), next hop (" << next_node << ")" << endl;
				}
				else {
					(*pTotal_received)++;
					received_by_this_node++;

					*pFout << "  Router[" << myNetAddr << "] received a packet from ["
						<< pPkt->getSrcAddr() << "] to [" << pPkt->getDstAddr() << "];";
					*pFout << " route : ";
					int count = pPkt->getHopCount();
					for (int j = 0; j<count; j++) {
						if (j != (count - 1))
							*pFout << pPkt->getRouteNodes(j) << " -> ";
						else
							*pFout << pPkt->getRouteNodes(j);
					}
					*pFout << endl;
					*pFout << "  *** Router[" << myNetAddr << "] currently received "
						<< received_by_this_node << " packets" << endl;
					*pFout << "Routers currently received total " << *pTotal_received << " packets" << endl;
				}
			}
			LeaveCriticalSection(pThrParam->pCS);
			Sleep(1);
		}
	}
	EnterCriticalSection(pThrParam->pCS);
	*pFout << ">>> Router[" << myNetAddr << "] received total ";
	*pFout << received_by_this_node << " packets ... terminating Thread...\n";
	LeaveCriticalSection(pThrParam->pCS);
	return 0;
}
