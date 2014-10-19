// @file graphm.h
// @author Muhammed Saleh
// @author Alejandro Alfaro

#ifndef GRAPHM_H
#define GRAPHM_H

#include "nodedata.h"
#include <climits>

//---------------------------------------------------------------------------
// GraphM class implements a directed graph of nodes with marked edges.
//		Allows functionality for constructing and destructing graphs, 
//		building new graphs from data files, inserting edges into the graph,
//		removing edges from the graph, finding the shortest path from one
//    graph node to another, displaying all edges in the graph, and
//    displaying a single edge in the graph.
//
// Implementation and assumptions:
//	 -- Uses Dijkstra's algorithm to find shortest path
//  -- Graph is stored in an adjacency matrix 
//	 -- First node is stored at index 1 in the array
//  -- Negative node values or node values greater than size are ignored
//  -- No more than 100 nodes
//	 -- Edge length can be no greater than (2^15 - 1) / 2
//  -- Data files must be properly formatted with an int size > 0 followed
//		 by size lines containing data about each node, followed by edges,
//		 one per line in the form "int startNode int endNode"

const int MAXNODES = 100;			// maxiumum number of nodes
// maximum edge length allows for the addition of two edge lengths
// represents infinity
const int MAX_VALUE =  INT_MAX / 2;

class GraphM {
	
public:
	GraphM();							       // constructor
	void buildGraph(ifstream& input);	 // builds graph from data file
	bool insertEdge(const int startNode, // inserts a new edge
					    const int endNode, const int label);
	bool removeEdge(const int startNode, const int endNode);
	void findShortestPath();	// find shortest path between all nodes
	void displayAll() const;	// display all nodes and paths
	void display(int startNode, int endNode) const; // display one path
	
private:
	struct TableType {
		bool visited; // whether node has been visited
		int dist;	  // currently known shortest distance from source
		int path;     // previous node in path of min dist
	};
	
	NodeData data[MAXNODES + 1];       // data for graph nodes information
	int C[MAXNODES + 1][MAXNODES + 1]; // adjacency matrix
	int size;                          // number of nodes in the graph
	// stores visited, distance, path
	TableType T[MAXNODES + 1][MAXNODES + 1]; 

	// helper methods
	int getSmallest(TableType data[]);	// find smallest value in array
	void displayHelper(int startNode, int endNode) const; // display one path
	void printPath(int startNode, int current) const; // print path
	void printDescriptions(int startNode, int current) const;
};

#endif
