#pragma once

#include <list>
#include <vector>
#include <iostream>
#include <iomanip>
#include <limits>
#include <string>

#define PLUS_INF	INT_MAX/2

enum VertexStatus { UNEXPLORED, VISITED, VRTX_NOT_FOUND };
enum EdgeStatus { DISCOVERY, BACK, CROSS, EDGE_UNEXPLORED, EDGE_VISITED, EDGE_NOT_FOUND };

using namespace std;

class Graph
{
public:
	class Vertex;
	class Edge;
public:
	class Vertex
	{
		friend ostream& operator<<(ostream& fout, Vertex v) {
			fout << v.getID();
			return fout;
		}
	public:
		Vertex() : name(), ID(-1) {}
		Vertex(string n, int id, VertexStatus vs) : name(n), ID(id), vtxStatus(vs) {}
		Vertex(char *pN, int id, VertexStatus vs) : name(string(pN)), ID(id), vtxStatus(vs) {}
		Vertex(int id) : ID(id) {}

		string getName() { return name; }
		int getID() { return ID; }

		void setName(string c_name) { name = c_name; }
		void setID(int id) { ID = id; }

		bool operator==(Vertex v) {
			return ((ID == v.getID()) && (name == v.getName()));
		}
		bool operator!=(Vertex v) {
			return ((ID != v.getID()) || (name != v.getName()));
		}

		void setVtxStatus(VertexStatus vs) { vtxStatus = vs; }
		VertexStatus getVtxStatus() { return vtxStatus; }
	private:
		string name;
		int ID;
		VertexStatus vtxStatus;
	};
public:
	typedef std::list<Graph::Vertex> VtxList;
	typedef std::list<Graph::Edge> EdgeList;
public:
	class Edge
	{
		friend ostream& operator<<(ostream& fout, Edge& e) {
			fout << "Edge(" << setw(2) << e.getVertex_1() << ", " << setw(2) << e.getVertex_2();
			fout << ", d(" << setw(4) << e.getDistance() << "))";
			return fout;
		}
	public:
		Edge() : pVrtx_1(NULL), pVrtx_2(NULL), distance(PLUS_INF) {}
		Edge(Vertex v1, Vertex v2, int d)
			: vrtx_1(v1), vrtx_2(v2), distance(d), pVrtx_1(NULL), pVrtx_2(NULL), edgeStatus(EDGE_UNEXPLORED) {}

		void endVertices(VtxList& vtxList) {
			vtxList.push_back(*pVrtx_1);
			vtxList.push_back(*pVrtx_2);
		}

		Vertex opposite(Vertex v) {
			if (v == *pVrtx_1)
				return *pVrtx_2;
			else if (v == *pVrtx_2)
				return *pVrtx_1;
			else {
				cout << "Error in opposite()" << endl;
				return Vertex(NULL);
			}
		}

		Vertex getVertex_1() { return vrtx_1; }
		Vertex getVertex_2() { return vrtx_2; }
		Vertex* getpVrtx_1() { return pVrtx_1; }
		Vertex* getpVrtx_2() { return pVrtx_2; }
		int getDistance() { return distance; }

		void setpVrtx_1(Vertex* pV) { pVrtx_1 = pV; }
		void setpVrtx_2(Vertex* pV) { pVrtx_2 = pV; }
		void setDistance(int d) { distance = d; }

		bool operator==(Edge e) {
			return ((*pVrtx_1 == e.getVertex_1()) && (*pVrtx_2 == e.getVertex_2()));
		}
		bool operator!=(Edge e) {
			return ((*pVrtx_1 != e.getVertex_1()) || (*pVrtx_2 != e.getVertex_2()));
		}

		void setEdgeStatus(EdgeStatus es) { edgeStatus = es; }
		EdgeStatus getEdgeStatus() { return edgeStatus; }
	private:
		Vertex vrtx_1;
		Vertex vrtx_2;
		Vertex* pVrtx_1;
		Vertex* pVrtx_2;
		int distance;
		EdgeStatus edgeStatus;
	};
public:
	typedef std::list<Vertex>::iterator VtxItor;
	typedef std::list<Edge>::iterator EdgeItor;

	Graph() : pVrtxArray(NULL), pAdjLstArray(NULL) {}
	Graph(int num_nodes) : pVrtxArray(NULL), pAdjLstArray(NULL)
	{
		typedef Edge* EdgePtr;

		num_vertices = num_nodes;
		pVrtxArray = new Graph::Vertex[num_vertices];
		for (int i = 0; i<num_nodes; i++)
			pVrtxArray[i] = NULL;

		pAdjLstArray = new EdgeList[num_vertices];
		for (int i = 0; i<num_vertices; i++)
			pAdjLstArray[i].clear();
	}

	void vertices(VtxList& vtxLst);
	void edges(EdgeList&);

	bool isAdjacentTo(Vertex v, Vertex w);

	void insertVertex(Vertex& v);
	void insertEdge(Edge& e);

	void eraseEdge(Edge e);
	void eraseVertex(Vertex v);

	void incidentEdges(Vertex v, EdgeList& edges);

	int getNumVertices() { return num_vertices; }
	Vertex* getpVrtxArray() { return pVrtxArray; }
	EdgeList* getpAdjLstArray() { return pAdjLstArray; }

	void printGraph();
private:
	Vertex* pVrtxArray;
	EdgeList* pAdjLstArray;
	int num_vertices;
};
