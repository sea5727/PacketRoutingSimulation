#include <iostream>
#include <iomanip>
#include <list>
#include <algorithm>
#include "Graph.h"
#include "BreadthFirstSearch.h"

using namespace std;

typedef Graph::Vertex Vertex;
typedef Graph::Edge Edge;
typedef std::list<Graph::Vertex> VertexList;
typedef std::list<Graph::Vertex>::iterator VertexItor;
typedef std::list<Graph::Edge> EdgeList;
typedef std::list<Graph::Edge>::iterator EdgeItor;

void BreadthFirstSearch::initialize()
{
	VertexList verts;
	graph.vertices(verts);
	for (VertexItor pv = verts.begin(); pv != verts.end(); ++pv)
		unvisit(*pv);

	EdgeList edges;
	graph.edges(edges);
	for (EdgeItor pe = edges.begin(); pe != edges.end(); ++pe)
		unvisit(*pe);

	done = false;
}

void BreadthFirstSearch::initDistMtrx()
{
	int** ppDistMtrx;
	int num_nodes;
	Vertex* pVrtxArray;
	EdgeList* pAdjLstArray;
	int curVrtx_ID, vrtxID;

	num_nodes = graph.getNumVertices();
	pVrtxArray = graph.getpVrtxArray();
	pAdjLstArray = graph.getpAdjLstArray();
	ppDistMtrx = getppDistMtrx();

	for (int i = 0; i<num_nodes; i++) {
		curVrtx_ID = pVrtxArray[i].getID();
		EdgeItor pe = pAdjLstArray[curVrtx_ID].begin();
		while (pe != pAdjLstArray[curVrtx_ID].end()) {
			vrtxID = ((*pe).getVertex_2()).getID();
			ppDistMtrx[curVrtx_ID][vrtxID] = (*pe).getDistance();
			pe++;
		}
		ppDistMtrx[curVrtx_ID][curVrtx_ID] = 0;
	}
}

void BreadthFirstSearch::printDistMtrx()
{
	int** ppDistMtrx;
	int num_nodes;
	int dist;

	num_nodes = graph.getNumVertices();
	ppDistMtrx = getppDistMtrx();

	cout << "   |";
	for (int i = 0; i<num_nodes; i++) {
		cout << setw(4) << i;
	}
	cout << endl;
	cout << "----+";
	for (int i = 0; i<num_nodes; i++) {
		cout << "-----";
	}
	cout << endl;
	for (int i = 0; i<num_nodes; i++) {
		cout << setw(2) << i << " |";
		for (int j = 0; j<num_nodes; j++) {
			dist = ppDistMtrx[i][j];
			if (dist == PLUS_INF)
				cout << setw(4) << "+oo";
			else
				cout << setw(4) << dist;
		}
		cout << endl;
	}
	cout << endl;
}

enum BFS_PROCESS_STATUS { NOT_SELECTED, SELECTED };
void BreadthFirstSearch::bfsTraversal(Vertex& s, Vertex& target, VertexList& path)
{
	int** ppDistMtrx;
	int* pLeastCost;
	int num_nodes, num_selected;
	int* pPrev;
	int minID, minCost;
	BFS_PROCESS_STATUS* pBFS_Process_Stat;
	Vertex* pVrtxArray;
	Vertex vrtx, v;
	Edge e;
	int start_vrtxid, target_vrtxid, vrtxID;
	EdgeList* pAdjLstArray;

	pVrtxArray = graph.getpVrtxArray();
	pAdjLstArray = graph.getpAdjLstArray();
	start_vrtxid = start.getID();
	target_vrtxid = target.getID();
	num_nodes = graph.getNumVertices();
	ppDistMtrx = getppDistMtrx();

	pLeastCost = new int[num_nodes];
	pPrev = new int[num_nodes];
	pBFS_Process_Stat = new BFS_PROCESS_STATUS[num_nodes];

	// initialize L(n) = w(start, n);
	for (int i = 0; i< num_nodes; i++) {
		pLeastCost[i] = ppDistMtrx[start_vrtxid][i];
		pPrev[i] = start_vrtxid;
		pBFS_Process_Stat[i] = NOT_SELECTED;
	}

	pBFS_Process_Stat[start_vrtxid] = SELECTED;
	num_selected = 1;
	int round = 0;

	while (num_selected < num_nodes) {
		round++;
		cout << "round [" << setw(2) << round << "] ";
		// find current node with LeastCost
		minID = -1;
		minCost = PLUS_INF;
		for (int i = 0; i<num_nodes; i++) {
			if ((pLeastCost[i] < minCost) && (pBFS_Process_Stat[i] != SELECTED)) {
				minID = i;
				minCost = pLeastCost[i];
			}
		}
		if (minID == -1) {
			cout << "Error in FindShortestPath() -- target is not connected to the start !!" << endl;
			break;
		}
		else {
			pBFS_Process_Stat[minID] = SELECTED;
			num_selected++;
			if (minID == target_vrtxid) {
				cout << "reached to the target node !!" << endl;
				cout << "Least Cost = " << minCost << endl;
				vrtxID = minID;
				do {
					vrtx = pVrtxArray[vrtxID];
					path.push_front(vrtx);
					vrtxID = pPrev[vrtxID];
				} while (vrtxID != start_vrtxid);
				vrtx = pVrtxArray[vrtxID];
				path.push_front(vrtx); // start node
				break;
			}
		}

		int pLS, ppDistMtrx_i;
		for (int i = 0; i<num_nodes; i++)
		{
			pLS = pLeastCost[i];
			ppDistMtrx_i = ppDistMtrx[minID][i];
			if ((pBFS_Process_Stat[i] != SELECTED) && (pLeastCost[i] >(pLeastCost[minID] + ppDistMtrx[minID][i])))
			{
				pPrev[i] = minID;
				pLeastCost[i] = pLeastCost[minID] + ppDistMtrx[minID][i];
			}
		}
		// print out the pLeastCost[] for debugging
		for (int i = 0; i<num_nodes; i++)
		{
			if (pLeastCost[i] == PLUS_INF)
				cout << setw(2) << i << " " << setw(4) << "+oo" << ", ";
			else
				cout << setw(2) << i << " " << setw(4) << pLeastCost[i] << ", ";
		}
		cout << endl;
	} // end while()
}

void BreadthFirstSearch::dijkstraBFS(Vertex& s, int* pParent)
{
	int** ppDistMtrx;
	int* pLeastCost;
	int num_nodes, num_selected;
	int minID, minCost;
	BFS_PROCESS_STATUS* pBFS_Process_Stat;
	Vertex* pVrtxArray;
	Vertex vrtx, v;
	Edge e;
	int start_vrtxid;
	EdgeList* pAdjLstArray;
	VertexList path;
	path.clear();

	pVrtxArray = graph.getpVrtxArray();
	pAdjLstArray = graph.getpAdjLstArray();
	start_vrtxid = s.getID();
	num_nodes = graph.getNumVertices();
	ppDistMtrx = getppDistMtrx();

	pLeastCost = new int[num_nodes];
	//pParent = new int[num_nodes];
	pBFS_Process_Stat = new BFS_PROCESS_STATUS[num_nodes];

	// initialize L(n) = w(start, n);
	for (int i = 0; i< num_nodes; i++) {
		pLeastCost[i] = ppDistMtrx[start_vrtxid][i];
		pParent[i] = start_vrtxid;
		pBFS_Process_Stat[i] = NOT_SELECTED;
	}

	pBFS_Process_Stat[start_vrtxid] = SELECTED;
	num_selected = 1;
	int round = 0;

	while (num_selected < num_nodes) {
		round++;
		//cout << "round [" << setw(2) << round << "] ";
		// find current node with LeastCost
		minID = -1;
		minCost = PLUS_INF;
		for (int i = 0; i<num_nodes; i++) {
			if ((pLeastCost[i] < minCost) && (pBFS_Process_Stat[i] != SELECTED)) {
				minID = i;
				minCost = pLeastCost[i];
			}
		}
		if (minID == -1) {
			cout << "Error in FindShortestPath() -- target is not connected to the start !!" << endl;
			break;
		}
		else {
			pBFS_Process_Stat[minID] = SELECTED;
			num_selected++;
		}

		int pLS, ppDistMtrx_i;
		for (int i = 0; i<num_nodes; i++)
		{
			pLS = pLeastCost[i];
			ppDistMtrx_i = ppDistMtrx[minID][i];
			if ((pBFS_Process_Stat[i] != SELECTED) && (pLeastCost[i] >(pLeastCost[minID] + ppDistMtrx[minID][i])))
			{
				pParent[i] = minID;
				pLeastCost[i] = pLeastCost[minID] + ppDistMtrx[minID][i];
			}
		}
		// print out the pLeastCost[] for debugging
		/*
		for (int i=0; i<num_nodes; i++)
		{
		if(pLeastCost[i] == PLUS_INF)
		cout << setw(2) << i << " " << setw(4) << "+oo" << ", ";
		else
		cout << setw(2) << i << " " << setw(4) << pLeastCost[i] << ", ";
		}
		cout << endl;
		*/
	} // end while()

	  /*
	  cout << endl;
	  //Print route
	  int count;
	  int index;
	  int* route = new int[num_nodes];
	  for(int i=0; i<num_nodes; i++) {
	  cout<< "shortest path from " << s.getID() << " to " << setw(2) << i << " :";

	  count = 0;
	  for(int n=0; n<num_nodes; n++)
	  route[n] = 0;
	  index = i;

	  while(index != s.getID()) {
	  route[count] = pParent[index];
	  index = pParent[index];
	  count++;
	  }

	  for(int j=count-1; j>-1; j--) {
	  if(route[j] == PLUS_INF)
	  cout << " " << " +oo" << setw(3) << "->";
	  else
	  cout << " " << route[j] << setw(3) << "->";
	  }
	  cout << " " << i << endl;
	  }
	  delete []route;
	  */
}

void BreadthFirstSearch::findShortestPath(Vertex &s, Vertex &target, VertexList& path)
{
	initialize();
	path.clear();
	start = s;
	initDistMtrx();
	printDistMtrx();
	bfsTraversal(start, target, path);
}

void BreadthFirstSearch::ShortestPathsTree(Vertex &s)
{
	initialize();
	start = s;
	initDistMtrx();
	//printDistMtrx();
	dijkstraBFS(start, pParent);
}

void BreadthFirstSearch::visit(Vertex& v)
{
	Graph::Vertex* pVtx;
	int vtx_ID = v.getID();
	int numNodes = getGraph().getNumVertices();
	if (vtx_ID >= 0 && vtx_ID < numNodes) {
		pVtx = getGraph().getpVrtxArray();
		pVtx[vtx_ID].setVtxStatus(VISITED);
	}
}

void BreadthFirstSearch::visit(Edge& e)
{
	EdgeItor eItor;
	Graph::EdgeList* pEdgeLst;
	Vertex vtx_1, vtx_2;
	int vtx_1_ID;

	int numNodes = getGraph().getNumVertices();
	vtx_1 = e.getVertex_1();
	vtx_1_ID = vtx_1.getID();

	if (vtx_1_ID >= 0 && vtx_1_ID < numNodes) {
		pEdgeLst = getGraph().getpAdjLstArray();
		eItor = find(pEdgeLst[vtx_1_ID].begin(), pEdgeLst[vtx_1_ID].end(), e);
		if (eItor == pEdgeLst[vtx_1_ID].end())
			cout << "Edge (" << e << " not found from the graph !!" << endl;
		else
			(*eItor).setEdgeStatus(EDGE_VISITED);
	}
}

void BreadthFirstSearch::unvisit(Vertex& v)
{
	int vtx_ID = v.getID();
	int numNodes = getGraph().getNumVertices();
	Vertex* pVtx = getGraph().getpVrtxArray();
	if (vtx_ID >= 0 && vtx_ID < numNodes)
		pVtx[vtx_ID].setVtxStatus(UNEXPLORED);
}

void BreadthFirstSearch::unvisit(Edge& e)
{
	EdgeItor eItor;
	Vertex vtx_1, vtx_2;
	int vtx_1_ID;
	int numNodes = getGraph().getNumVertices();
	EdgeList* pAdjLstArray;
	vtx_1 = e.getVertex_1();
	vtx_1_ID = vtx_1.getID();
	if (vtx_1_ID >= 0 && vtx_1_ID < numNodes) {
		pAdjLstArray = getGraph().getpAdjLstArray();
		eItor = find(pAdjLstArray[vtx_1_ID].begin(), pAdjLstArray[vtx_1_ID].end(), e);
		if (eItor == pAdjLstArray[vtx_1_ID].end())
			cout << "Edge (" << e << " not found from the graph !!" << endl;
		else
			(*eItor).setEdgeStatus(EDGE_UNEXPLORED);
	}
}

bool BreadthFirstSearch::isVisited(Vertex& v)
{
	Vertex* pVtx;
	int vtx_ID = v.getID();
	int numNodes = getGraph().getNumVertices();
	Vertex* pVrtxArray;
	if (vtx_ID >= 0 && vtx_ID < numNodes) {
		pVrtxArray = getGraph().getpVrtxArray();
		pVtx = &(pVrtxArray[vtx_ID]);
		return (pVtx->getVtxStatus() == VISITED);
	}
	else {
		return false;
	}
}

bool BreadthFirstSearch::isVisited(Edge& e)
{
	EdgeItor eItor;
	Vertex vtx_1, vtx_2;
	int vtx_1_ID;
	int numNodes = getGraph().getNumVertices();
	EdgeList* pAdjLstArray;
	EdgeStatus eStat;
	vtx_1 = e.getVertex_1();
	vtx_1_ID = vtx_1.getID();
	if (vtx_1_ID >= 0 && vtx_1_ID < numNodes) {
		pAdjLstArray = getGraph().getpAdjLstArray();
		eItor = find(pAdjLstArray[vtx_1_ID].begin(), pAdjLstArray[vtx_1_ID].end(), e);
		if (eItor == pAdjLstArray[vtx_1_ID].end()) {
			cout << "Edge (" << e << " not found from the graph !!" << endl;
			return false;
		}
		else {
			eStat = (*eItor).getEdgeStatus();
			if ((eStat == EDGE_VISITED) || (eStat == DISCOVERY) || (eStat == BACK))
				return true;
			else
				return false;
		}
	}
	return false;
}

void BreadthFirstSearch::setEdgeStatus(Edge& e, EdgeStatus es)
{
	EdgeItor eItor;
	int vtx1id = e.getVertex_1().getID();
	int vtx2id = e.getVertex_1().getID();
	EdgeList* pAdjLstArray;
	pAdjLstArray = getGraph().getpAdjLstArray();
	eItor = pAdjLstArray[vtx1id].begin();
	while (eItor != pAdjLstArray[vtx1id].end()) {
		if (*eItor == e)
			break;
		else
			eItor++;
	}
	if (eItor != pAdjLstArray[vtx1id].end())
		(*eItor).setEdgeStatus(es);
	else
		cout << "Edge (" << e << ") was not found from AdjacencyList" << endl;
}

EdgeStatus BreadthFirstSearch::getEdgeStatus(Edge& e)
{
	EdgeItor eItor;
	int vtx1id = e.getVertex_1().getID();
	int vtx2id = e.getVertex_1().getID();
	EdgeList* pAdjLstArray;
	pAdjLstArray = getGraph().getpAdjLstArray();
	eItor = pAdjLstArray[vtx1id].begin();
	while (eItor != pAdjLstArray[vtx1id].end())
	{
		if (*eItor == e)
			break;
	}
	if (eItor != pAdjLstArray[vtx1id].end())
		return (*eItor).getEdgeStatus();
	else {
		cout << "Edge (" << e << ") was not found from AdjacencyList" << endl;
		return EDGE_NOT_FOUND;
	}
}
