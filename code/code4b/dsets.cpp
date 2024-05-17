/*********************************************
 * file:	~\code4b\dsets.cpp                *
 * remark: implementation of disjoint sets    *
 **********************************************/

#include <iostream>
#include <format>
#include <cassert>

#include "dsets.h"

// -- CONSTRUCTORS

DSets::DSets(int theSize) : V(theSize + 1) {  // slot zero is not used
    assert(theSize > 0);
    init();
}

// -- MEMBER FUNCTIONS

// create initial sets
void DSets::init() {
    for (auto i = 1; i < std::ssize(V); ++i) {
        V[i] = -1;  // every disjoint set has one node -- singleton
    }
}

// join sets named r and s where r != s
// i.e. join trees with roots r and s
void DSets::join(int r, int s) {
    
    assert(r != s);
    assert(r >= 1 && r <= std::ssize(V) - 1);
    assert(s >= 1 && s <= std::ssize(V) - 1);
    assert(V[r] < 0);
    assert(V[s] < 0);

    //// simple union
    //V[r] = s;



    // *** TODO ***
    
    // Do Union by Size - join smaller subtree with larger subtree, based on number of vertices in each subtree 
    //if subset r < subset s, join r with s and add number of vertices of r to that of s 
    if (V[r] < V[s]) {
        //update size of r with that of s
        V[r] += V[s];   
        //point r to s
        V[s] = r;
    }
    //if s is smaller or r=s, join s to r 
    else {
        //update size of s with that of r 
        V[s] += V[r]; 
        //point s to r 
        V[r] = s; 
    }
}

// return name of current set for x
// i.e. return root of tree for x
int DSets::find(int x) {
    assert(x >= 1 && x <= std::ssize(V) - 1);

    //// simple find
    //if (V[x] < 0) {
    //    return x;
    //} else {
    //    return find(V[x]);
    //}

     // *** TODO ***
    // find with path compression

    //path compression = every vertex on path from x to root has its parent redirected to the root 
    //check if x is already the root of the subset = base case (negative values indicate x is root)
    if (V[x] < 0) {
        //x found to be the root, x root returned 
        return x; 
    }
    //continue searching for the subset root
    else {
        //move up the tree and update each subtree parent py pointing it to the root 
        return V[x] = find(V[x]); 
    }
}

// just in case ...
void DSets::print() const {
    std::cout << "\n";

    for (auto i = 1; i < std::ssize(V); i++) {
        std::cout << std::format("{:4}", i);
    }
    std::cout << "\n";

    for (auto u : V) {
        std::cout << std::format("{:4}", u);
    }
    std::cout << "\n";
}
