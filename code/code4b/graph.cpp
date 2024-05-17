/*********************************************
 * file:	~\code4b\graph.h                  *
 * remark:implementation of undirected graphs *
 **********************************************/

#include <iostream>
#include <algorithm>
#include <format>
#include <cassert>     // assert
#include <limits>      // std::numeric_limits
#include <algorithm>   // std::make_heap(), std::pop_heap(), std::push_heap()
#include <queue>       //priority_queue

#include "graph.h"
#include "dsets.h"

// Note: graph vertices are numbered from 1 -- i.e. there is no vertex zero

// -- CONSTRUCTORS

// Create a graph with n vertices and no vertices
Graph::Graph(int n) : table(n + 1), size{n}, n_edges{0} {
    assert(n >= 1);
}

Graph::Graph(const std::vector<Edge> &V, int n) : Graph{n} {
    for (auto e : V) {
        insertEdge(e);
    }
}

// -- MEMBER FUNCTIONS

// insert undirected edge e
// update weight if edge e is present
void Graph::insertEdge(const Edge &e) {
    assert(e.from >= 1 && e.from <= size);
    assert(e.to >= 1 && e.to <= size);

    auto edge_insertion = [&T = this->table, &n = this->n_edges](const Edge &e1) {
        if (auto it = std::find_if(begin(T[e1.from]), end(T[e1.from]),
                                   [e1](const Edge &ed) { return e1.links_same_nodes(ed); });
            it == end(T[e1.from])) {
            T[e1.from].push_back(e1);  // insert new edge e1
            ++n;                       // increment the counter of edges
        } else {
            it->weight = e1.weight;  // update the weight
        }
    };

    edge_insertion(e);
    edge_insertion(e.reverse());
}

// remove undirected edge e
void Graph::removeEdge(const Edge &e) {
    assert(e.from >= 1 && e.from <= size);
    assert(e.to >= 1 && e.to <= size);

    auto edgeRemoval = [&T = this->table, &n = this->n_edges](const Edge &e1) {
        auto it = std::find_if(begin(T[e1.from]), end(T[e1.from]),
                               [e1](const Edge &ed) { return e1.links_same_nodes(ed); });

        assert(it != end(T[e1.from]));
        T[e1.from].erase(it);  // remove edge e1
        --n;                   // decrement the counter of edges
    };

    edgeRemoval(e);
    edgeRemoval(e.reverse());
}

// Prim's minimum spanning tree algorithm
void Graph::mstPrim() const {
    std::vector<int> dist(size + 1, std::numeric_limits<int>::max());
    std::vector<int> path(size + 1, 0);
    std::vector<bool> done(size + 1, false);

    // *** TODO ***

    //starting vertex, has distance 0 to itself 
    int start_vertex = 1; 
    dist[start_vertex] = 0; 
    //mark sart vertex as visited 
    done[start_vertex] = true; 
    //start at start vertex
    int v = start_vertex; 

    //keep track of the summed egde weights
    int weights_sum = 0; 

    //iterate all vertecies in the graph until all have been inserted into the MST and marked as visited 
    while (true) {
        for (auto& e : table[v]) {

            //vertex u whatever vertex edge e goes to
            int u = e.to;

            //if edges e to unvisited u have smaller weight, update the distance and path of u to that of v 
            //and the weight of e 
            if (!done[u] && dist[u] > e.weight) {

                dist[u] = e.weight; 
                path[u] = v; 
            }
        }

        //initialize the smallest distance  
        int min = std::numeric_limits<int>::max(); 

        //for unvisited adjacent vertices, find the smallest distance
        for (int i = 1; i <= size; i++) {

            //if vertex isn't visited and the distance to it is smaller than current min, update current min 
            if (!done[i] && dist[i] < min) {

                //update smallest distance to that of i and assign vertex i to v -> stepping forward 
                min = dist[i]; 
                v = i; 
            }
        }

        //no univisted vertices with finite distance found, we are done -> break the algorithm
        if (min == std::numeric_limits<int>::max()) {
            break; 
        }

        //update v as visited 
        done[v] = true;
        //add the distance of v to the weight sum
        weights_sum += min;
        //print edge as: from_vertex, to_vertex, edge_weight
        std::cout << "(" << path[v] << ", " << v << ", " << dist[v] << ")" << std::endl;
    }

    //print the total weight sum
    std::cout << "Total weight = " << weights_sum << std::endl; 
}



// Kruskal's minimum spanning tree algorithm
void Graph::mstKruskal() const {
    // *** TODO ***

    //create and reserve a vector for the edges of the graph, |E| = size-1 = |V|-1
    std::vector<Edge> edges; 
    edges.reserve(size - 1); 

    //use the disjoint sets to partition the graph, size=|V|=number of vertices on the graph
    DSets D(size); 

    //keep track of the total weight sum 
    int weights_sum = 0; 

    //read edges from table and add them to the edge vector 
    for (int v = 1; v <= size; v++) {
        for (auto& e : table[v])

            if (e.from < e.to) {
                edges.push_back(e); 
            }
    }

    //make a minHeap out of the edge vector, greater to sort in reverse since heap is maxHeap by default in STL 
    //and we want a minHeap for the edges 
    std::make_heap(edges.begin(), edges.end(), std::greater<Edge>()); 

    //iterate the edges on the queue until queue is emptied = edge vector is empty 
    while (!edges.empty()) {

        //retrieve the topmost edge = smallest edge weight, then remove it from the queue and edge vector 
        Edge current_edge = edges.front(); 
        std::pop_heap(edges.begin(), edges.end(), std::greater<Edge>()); 
        edges.pop_back(); 

        //find the vertices on both sides of the current edge 
        int from_vertices = D.find(current_edge.from); 
        int to_vertices = D.find(current_edge.to);

        //test that edges don't make a cycle
        if (from_vertices != to_vertices) {

            //join from DSets adds egde between to- and from vertices to MST by union
            D.join(from_vertices, to_vertices); 
            //add the current edge weight to the total weight sum 
            weights_sum += current_edge.weight; 

            //print the edges -> printing and format from Edge class
            std::cout << current_edge << std::endl; 
        }
    }

    //print total edge sum 
    std::cout << "Total weight = " << weights_sum << std::endl;
}


// print graph
void Graph::printGraph() const {
    std::cout << std::format("{:-<66}\n", '-');
    std::cout << "Vertex  adjacency lists\n";
    std::cout << std::format("{:-<66}\n", '-');

    for (int v = 1; v <= size; v++) {
        std::cout << std::format("{:4} : ", v);
        for (auto const &e : table[v]) {
            std::cout << std::format("({:2}, {:2}) ", e.to, e.weight);
        }
        std::cout << "\n";
    }
    std::cout << std::format("{:-<66}\n", '-');
}
