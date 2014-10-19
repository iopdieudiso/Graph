// @file graphm.cpp
// @author Muhammed Saleh
// @author Alejandro Alfaro

#ifndef GRAPHM_CPP
#define GRAPHM_CPP

#include "graphm.h"

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

// ---------------------------------------------------------------------------
// Constructor 
// Default constructor for GraphM class initializes all dist, path, and
// values in the adjacency array to MAX_VALUE. Also marks all nodes unvisited
// and size to 0.
GraphM::GraphM() {
	size = 0;
	for (int i = 1; i <= MAXNODES; i++) {    // for each node in the graph
		for (int j = 1; j <= MAXNODES; j++) { // for each adjacent node
			T[i][j].visited = false;           // mark as unvisited
			T[i][j].dist = MAX_VALUE;
			T[i][j].path = MAX_VALUE;	// set all distances to MAX_VALUE
			
			C[i][j] = MAX_VALUE;
		}
	}
}

// ---------------------------------------------------------------------------
// buildGraph
// Takes in an ifstream& and builds a graph based on the data found in that
// file.
// Pre: the file is properly formatted starting with an int size > 0 followed
//		by size lines containing data about each node, followed by edges,
//		one per line in the form "int startNode int endNode int label"
void GraphM::buildGraph(ifstream& input) {
	bool stop = false;				// end of graph when startNode is 0 
	while (!input.eof() && !stop) {
		
		input >> size;              // set size to first value in file
		string s;					    // skip to next line
		getline(input, s);
		
		// for the next size number of lines
		for (int i = 1; i <= size; i++) { 
			data[i].setData(input); // set data to the data on each line
		}
		
		int startNode = 0;
		int endNode = 0;
		int label = 0;
		
		while (!input.eof() && !stop) {
			input >> startNode;
			input >> endNode;		   // take next 3 ints on line
			input >> label;
			getline(input, s);		// skip to next line
			
			if (startNode == 0) {   // end of graph 
				stop = true;
			}
			
			if (!stop) {            // not yet end of graph
				insertEdge(startNode, endNode, label);	// insert
			}
		}
	}
}

// ---------------------------------------------------------------------------
// insertEdge
// Takes in a startNode int an endNode int and a label int and inserts an 
// edge into the graph from the startNode to the endNode. Returns whether 
// or not the edge was inserted.
bool GraphM::insertEdge(const int startNode, 
							   const int endNode, const int label) {
	// if startNode and endNode are valid
	if (startNode >= 1 && endNode >= 1 && label >= 0 &&
		startNode <= size && endNode <= size &&
		label <= MAX_VALUE && startNode != endNode) {

		// insert label
		C[startNode][endNode] = label;
		return true; // successful insert
	}
	return false; // invalid startNode or endNode
}

// ---------------------------------------------------------------------------
// removeEdge
// Takes in a startNode int and an endNode int and removes the corresponding
// edge from the graph. Returns whether or not the edge was removed.
bool GraphM::removeEdge(const int startNode, const int endNode) {
	// if startNode and endNode are valid
	if (startNode >= 1 && endNode >= 1 &&
	    startNode <= size && endNode <= size &&
		 startNode != endNode) {
		
		// reset value at startNode endNode
		C[startNode][endNode] = MAX_VALUE;
		return true; // successful removal
	}
	return false; // invalid startNode or endNode
}

// ---------------------------------------------------------------------------
// findShortestPath
// Uses Dijkstra's algorithm to find the shortest path between each
// node in the graph and every other node.
void GraphM::findShortestPath() {
	for (int source = 1; source <= size; source++) { // for each noce
		T[source][source].dist = 0;				// set source dist to 0
		int v = 1;								
		for (int i = 1; i <= size; i++) { // for each other node
			v = getSmallest(T[i]);	       // find v
			T[i][v].visited = true;        // mark v as visited
			
			for (int w = 1; w <= size; w++) { // for each adjacent node
				if (!T[i][w].visited) {        // if unvisited
					// check which distance is smaller
					if (T[i][w].dist > (T[i][v].dist + C[v][w])) {
						// going through v is smaller than current
						T[i][w].dist = T[i][v].dist + C[v][w];
						T[i][w].path = v; // add v to the path
					}
				}
			}
		}
	}
}

// ---------------------------------------------------------------------------
// getSmallest
// Helper method for findShortestPath returns the index of the smallest value
// in the passed in array.
int GraphM::getSmallest(TableType data[]) {
	int min = size;
	int shortestDist = MAX_VALUE; // starting lowest dist is infinite
	for (int i = 1; i <= size; i++) { // for each element in array
		// if smaller and unvisited
		if (data[i].dist < shortestDist && !data[i].visited) {
			min = i;				           // save index
			shortestDist = data[i].dist; // mark as shortest dist
		}
	}
	return min; // shortest dist and is unmarked
}

// ---------------------------------------------------------------------------
// displayAll
// Displays all nodes in the graph as well as the edges starting at 
// each node
void GraphM::displayAll() const {
	cout << "Description From node To node Dijkstra's Path" << endl;
	for (int i = 1; i <= size; i++) {     // for each node
		cout << data[i] << endl;          // print description
		for (int j = 1; j <= size; j++) { // for each other node
			if (i != j) {                 // if they arent the same node
				displayHelper(i, j);
			}
		}
		cout << endl;
	}
}

// ---------------------------------------------------------------------------
// display
// Takes in a startNode and an endNode and displays a single edge between 
// those two nodes if the passed in values are valid. Also displays the
// discriptions of the nodes in the path.
void GraphM::display(int startNode, int endNode) const {
	// checks for valid node values and that a path exists
	if (startNode <= size && endNode <= size && 
	    startNode >= 1 && endNode >= 1 && 
		 startNode != endNode) {
		
		displayHelper(startNode, endNode);
		if (T[startNode][endNode].dist != MAX_VALUE) {
			printDescriptions(startNode, endNode); // display labels
		   cout << data[endNode] << endl;         // print last node
		}
	}
}

// ---------------------------------------------------------------------------
// displayHelper
// Takes in a startNode and and endNode and displays a single edge between 
// those two nodes if the passed in values are valid.
void GraphM::displayHelper(int startNode, int endNode) const {
	// passed in startNode and endNode are valid
	if (startNode <= size && endNode <= size &&
		 startNode >= 1 && endNode >= 1 && startNode != endNode) {
		cout << " " << startNode << " " << endNode << " "; // print edge
		int length = T[startNode][endNode].dist;
		if (length == MAX_VALUE) { // if length is infinite
			cout << "----" << endl;
		} else {
			cout << length << " ";
			int current = endNode;
			printPath(startNode, current); // print path backwards
			cout << endNode << endl;       // print last node

			current = endNode;
		}
	}
}

// ---------------------------------------------------------------------------
// printPath
// Helper method for display recursively prints the shortest path backwards.
void GraphM::printPath(int startNode, int current) const {
	if (current != startNode) {
		current = T[startNode][current].path;
		printPath(startNode, current); // recursively print path
		cout << current << " ";        // print current node
	}
}

// ---------------------------------------------------------------------------
// printDescriptions
// Helper method for display recursively prints the descriptions of
// the nodes in the shortest path backwards.
void GraphM::printDescriptions(int startNode, int current) const {
	if (current != startNode) {
		current = T[startNode][current].path;  // get next node
		printDescriptions(startNode, current); // recurse
		cout << data[current] << endl; // print current node's description
	}
}

#endif
