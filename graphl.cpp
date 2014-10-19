// @file graphl.cpp
// @author Muhammed Saleh
// @author Alejandro Alfaro

#ifndef GRAPHL_CPP
#define GRAPHL_CPP

#include "graphl.h"

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

// ---------------------------------------------------------------------------
// Constructor 
// Default constructor for GraphL class initializes all nodes to NULL and
// size to 0.
GraphL::GraphL() {
	size = 0;
	// initializes the nodes array
	for (int i = 1; i <= MAXNODESL; i++) {
		nodes[i] = NULL;
	}
}

// ---------------------------------------------------------------------------
// Destructor
// Destructor for GraphL class
GraphL::~GraphL() {
	for (int i = 1; i <= size; i++) {			// for each GraphNode
		EdgeNode* current = nodes[i]->edgeHead;
		while (current != NULL) {				// for each EdgeNode
			EdgeNode* temp = current->nextEdge;	// save next edge
			delete current;						// clear current memory
			current = temp;						// advance	
		}
		delete nodes[i];						// clear GraphNode memory
		nodes[i] = NULL;
	}
}

// ---------------------------------------------------------------------------
// buildGraph
// Takes in an ifstream& and builds a graph based on the data found in that
// file.
// Pre: the file is properly formatted starting with an int size > 0 followed
//		by size lines containing data about each node, followed by edges,
//		one per line in the form "int startNode int endNode"
void GraphL::buildGraph(ifstream& input) {
	bool stop = false;				// end of graph when startNode is 0 
	while (!input.eof() && !stop) {
		
		input >> size;				// read first int to size
		string s;					
		getline(input, s);			// skip to next line
		
		// read the next size number of lines
		for (int i = 1; i <= size; i++) {
			nodes[i] = new GraphNode;		
			nodes[i]->data.setData(input); // initialize new node
			nodes[i]->edgeHead = NULL;
		}
		
		int startNode = 0;
		int endNode = 0;
		
		while (!input.eof() && !stop) {
			input >> startNode;			// read first int on line
			input >> endNode;			// read second int on line
			getline(input, s);			// skip to next line
			
			if (startNode == 0) {		// end of graph
				stop = true;
			}
			
			if (!stop) {                // not the end of graph
				insertEdge(startNode, endNode); // insert edge
			}
		}
	}
}

// ---------------------------------------------------------------------------
// insertEdge
// Takes in a startNode int and an endNode int and inserts an edge into the
// graph from the startNode to the endNode. Returns whether or not the
// edge was inserted.
bool GraphL::insertEdge(const int startNode, const int endNode) {
	// check to see that startNode and endNode are valid
	if (startNode <= size && endNode <= size &&
	startNode >= 1 && endNode >= 1 && startNode != endNode) {
		// insert edge at beginning of list
		EdgeNode* temp = nodes[startNode]->edgeHead; // save fist node
		EdgeNode* newEdge = new EdgeNode;			 // make new node
		newEdge->adjGraphNode = endNode;	
		newEdge->nextEdge = temp;		  // new node points to first node
		nodes[startNode]->edgeHead = newEdge;	// front to new node
		
		return true; // successful insert
	}
	return false; // failed insert, invalid startNode or endNode
}

// ---------------------------------------------------------------------------
// displayGraph
// Displays the discription for each node in the graph as well as each edge
// that starts at that node.
void GraphL::displayGraph() const {
	cout << "Graph:" << endl;
	for (int i = 1; i <= size; i++) { // for each GraphNode
		cout << "Node " << i << " " << nodes[i]->data << endl; // print data
		EdgeNode* current = nodes[i]->edgeHead;
		while (current != NULL) {	  // for each edgeNode
			// print edge
			cout << " edge " << i << " " << current->adjGraphNode << endl;
			current = current->nextEdge; // progress
		}
	}
	cout << endl;
}

// ---------------------------------------------------------------------------
// depthFirstSearch
// Performs a depth traversal of the graph and prints the ordering.
void GraphL::depthFirstSearch() {
	cout << "Depth-first ordering:";
	markUnvisited();					// mark all v unvisited
	for (int v = 1; v <= size; v++) {   // for each v
		if (!nodes[v]->visited) {       // if it is unvisited
			dfs(v);						// traverse graph
		}
	}
	cout << endl;
}

// ---------------------------------------------------------------------------
// dfs
// Recursive helper method for depthFirstSearch
void GraphL::dfs(int v) {
	nodes[v]->visited = true;                    // mark as visited
	cout << " " << v;                                   // print node
	EdgeNode* current = nodes[v]->edgeHead; 
	while (current != NULL) {                    // for each edgeNode
		if (!nodes[current->adjGraphNode]->visited) { // if unvisited
			dfs(current->adjGraphNode);          // traverse that node
		}
		current = current->nextEdge;             // progress
	}
}

// ---------------------------------------------------------------------------
// markUnvisited
// Private helper method sets visited to false for all nodes in the graph
void GraphL::markUnvisited() {
	for (int i = 1; i <= size; i++) {  // for each graphNode
		nodes[i]->visited = false;     // mark unvisited
	}
}

#endif
