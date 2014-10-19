// @file graphl.h
// @author Muhammed Saleh
// @author Alejandro Alfaro

#ifndef GRAPHL_H
#define GRAPHL_H

#include "nodedata.h"

//---------------------------------------------------------------------------
// GraphL class implements a directed graph of nodes with unmarked edges.
//		Allows functionality for constructing and destructing graphs, 
//		building new graphs from data files, inserting edges into the graph
//		displaying the edges in the graph, and performing a depth first
//      traversal of the graph.
//
// Implementation and assumptions:
//   -- Graph is stored in an adjacency list consisting of a matrix of 
//      linked lists
//	 -- First node is stored at index 1 in the array
//   -- Negative node values or node values greater than size are ignored
//   -- No more than 100 nodes
//   -- Data files must be properly formatted with an int size > 0 followed
//		by size lines containing data about each node, followed by edges,
//		one per line in the form "int startNode int endNode"

const int MAXNODESL = 100; // maximum number of nodes

class GraphL {
	
public:
	GraphL();							// constructor
	~GraphL();							// destructor
	void buildGraph(ifstream& input);	// builds graph from data file
	void displayGraph() const;			// displays contents of graph
	void depthFirstSearch();			// performs depth first traversal
	
private:
	struct EdgeNode; // forward reference for the compiler
	struct GraphNode { 
		EdgeNode* edgeHead; // head of the list of edges
		NodeData data;		// description of node
		bool visited;
	};
	
	struct EdgeNode {
		int adjGraphNode;   // subscript of the adjacent graph node
		EdgeNode* nextEdge;
	};
	
	GraphNode* nodes[MAXNODESL + 1]; // array of nodes
	int size;						 // number of nodes in graph
	
	// helper methods
	bool insertEdge(const int startNode, const int endNode);
	void dfs(int v);				// recursive deapth first search
	void markUnvisited();			// marks all nodes unvisited
};

#endif
