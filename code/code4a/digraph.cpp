/*********************************************
 * file:	~\code4a\digraph.cpp              *
 * remark: implementation of directed graphs  *
 **********************************************/

#include <iostream>
#include <algorithm>
#include <limits>  //std::numeric_limits
#include <vector>
#include <cassert>
#include <queue> //queues
#include <format>

#include "digraph.h"

 // Note: graph vertices are numbered from 1 -- i.e. there is no vertex zero

 // -- CONSTRUCTORS

Digraph::Digraph(int n)
	: table(n + 1)  // slot zero not used
	, size{ n }       // number of verices
	, n_edges{ 0 }
	, dist(n + 1)
	, path(n + 1)
	, done(n + 1) {
	assert(n >= 1);
	// Note: graph vertices are numbered from 1 -- i.e. there is no vertex zero
}

// Create a digraph with n vertices and the edges in V
Digraph::Digraph(const std::vector<Edge>& V, int n) : Digraph{ n } {
	for (auto e : V) {
		insertEdge(e);
	}
}

// -- MEMBER FUNCTIONS

// insert directed edge e = (u, v, w)
// update weight w if edge (u, v) is present
void Digraph::insertEdge(const Edge& e) {
	assert(e.from >= 1 && e.from <= size);
	assert(e.to >= 1 && e.to <= size);

	// Check if edge e already exists
	if (auto it = std::find_if(begin(table[e.from]), end(table[e.from]),
		[e](const Edge& ed) { return e.links_same_nodes(ed); });
		it == end(table[e.from])) {
		table[e.from].push_back(e);  // insert new edge e
		++n_edges;
	}
	else {
		it->weight = e.weight;  // update the weight
	}
}

// remove directed edge e
void Digraph::removeEdge(const Edge& e) {
	assert(e.from >= 1 && e.from <= size);
	assert(e.to >= 1 && e.to <= size);

	auto it = std::find_if(begin(table[e.from]), end(table[e.from]),
		[e](const Edge& ed) { return e.links_same_nodes(ed); });

	assert(it != end(table[e.from]));
	table[e.from].erase(it);
	--n_edges;
}

// construct unweighted single source shortest path-tree for start vertex s
void Digraph::uwsssp(int s) const {
	assert(s >= 1 && s <= size);

	// *** TODO ***

	//dist vector should be initialized with std::numeric_limits<int>::max() and all paths are zero initially
	for (std::size_t i = 0; i <= size; i++) {
		dist[i] = std::numeric_limits<int>::max();
		path[i] = 0;
	}

	//the start vertex s has distance 0 
	dist[s] = 0;

	//create a queue for the unvisited verteces 
	std::queue<int> vertex_queue;

	//load the start vertex onto the queue
	vertex_queue.push(s);

	//use breadth-first search to find adjacent unvisited vertices 
	while (!vertex_queue.empty()) {

		//dequeue front vertex u since it has now been visited, by placing ourselves at the front and pop
		int u = vertex_queue.front();
		vertex_queue.pop();

		//go to all adjacent vertices of visited u, using the adjacency list of the digraph 
		for (const Edge& e : table[u]) {

			//get the next adjacent vertex of u, which is v, and v is wherever vertex e is pointing to
			int v = e.to;

			//check if distance from u to v via e has been updated, if not, we update it
			if (dist[v] == std::numeric_limits<int>::max()) {

				//distances incremented by one for each level away from the start vertex we are, since all edges have 
				//weight 1 in an unweighted graph
				dist[v] = dist[u] + 1;
				//path of vertex v is u, since edges are unweighted
				path[v] = u;

				//add=enqueue the neighbours of v
				vertex_queue.push(v);
			}
		}
	}
}


// construct positive weighted single source shortest path-tree for start vertex s
// Dijktra’s algorithm
void Digraph::pwsssp(int s) const {
	assert(s >= 1 && s <= size);

	// *** TODO ***

	//dist vector should be initialized with std::numeric_limits<int>::max(), paths and marked zero respectively
	//false initially
	for (std::size_t i = 1; i <= size; i++) {
		dist[i] = std::numeric_limits<int>::max();
		path[i] = 0;
		done[i] = false;
	}

	//distance to starting vertex s is zero 
	dist[s] = 0;

	//go through all vertecies until they have all been marked as visited
	for (std::size_t i = 1; i <= size; ++i) {

		//find unvisited vertex with smallest distance
		int min = std::numeric_limits<int>::max();
		//vertex with smallest distance marked as -1, this will be s at first, so s is marked visited in this loop
		int u = -1;

		//go through all vertecies to find the the one with the smallest distance 
		for (std::size_t v = 1; v <= size; ++v) {

			//if v hasn't been visited, and distance of v is smaller than current min distance, update the smallest
			//distance
			if (!done[v] && dist[v] < min) {
				min = dist[v];
				u = v; //u no longer the smallest
			}
		}

		//if no appropriate smallest vertex is found, return 
		if (u == -1) {
			break;
		}

		//mark u as visited
		done[u] = true;

		//update the distances in the adjacency list connected to u 
		for (const Edge& e : table[u]) {

			//next vertex whaever vertex e points to
			int v = e.to;

			//current weight the weight of e
			int current_weight = e.weight;

			//update paths whenever less costy weighted paths have been found
			if (dist[u] + current_weight < dist[v]) {
				dist[v] = dist[u] + current_weight;
				path[v] = u;
			}
		}
	}
}

// print graph
void Digraph::printGraph() const {
	std::cout << std::format("{:-<66}\n", '-');
	std::cout << "Vertex  adjacency lists\n";
	std::cout << std::format("{:-<66}\n", '-');

	for (int v = 1; v <= size; ++v) {
		std::cout << std::format("{:4} : ", v);
		for (auto const& e : table[v]) {
			std::cout << std::format("({:2}, {:2}) ", e.to, e.weight);
		}
		std::cout << "\n";
	}
	std::cout << std::format("{:-<66}\n", '-');
}

// print shortest path tree for s
void Digraph::printTree() const {
	std::cout << std::format("{:-<22}\n", '-');
	// std::cout << "----------------------\n";
	std::cout << "vertex    dist    path\n";
	std::cout << std::format("{:-<22}\n", '-');
	// std::cout << "----------------------\n";

	for (int v = 1; v <= size; ++v) {
		std::cout << std::format("{:4} : {:6} {:6}\n", v,
			((dist[v] == std::numeric_limits<int>::max()) ? -1 : dist[v]),
			path[v]);
	}
	std::cout << std::format("{:-<22}\n", '-');
	// std::cout << "----------------------\n";
}


//recursive private function for path printing 
void Digraph::print_shortest_path(int v) const {

	//path of last vertex will be zero, so just print the last vertex v and return = base case
	if (path[v] == 0) {
		std::cout << " " << v;
		return;
	}

	//move through all vertices v on shortest path tree and print v to cout 
	print_shortest_path(path[v]);
	std::cout << " " << v;
}

// print shortest path from s to t and the corresponding path length
// Hint: consider using recursion
void Digraph::printPath(int t) const {
	assert(t >= 1 && t <= size);

	// *** TODO ***

	//path vertices should be printed, and the path length in parenthesis in the end and path length = |E| = |V|-1
	//use the recursive function above for printing vertex t
	print_shortest_path(path[t]);
	//in the end, print the path length to t
	std::cout << " " << t << " (" << dist[t] << ")" << std::endl;
}