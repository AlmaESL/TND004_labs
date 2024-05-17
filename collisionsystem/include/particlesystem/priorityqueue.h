#pragma once

#include <iostream>
#include <vector>
#include <cassert>

#include <algorithm>

#define TEST_PRIORITY_QUEUE

/**
 * A heap based priority queue where the root is the smallest element -- min heap
 */
template <class Comparable>
class PriorityQueue {
public:
    /**
     * Constructor to create a queue with the given capacity
     */
    explicit PriorityQueue(int initCapacity = 100) : orderOK{true} {
        pq.reserve(initCapacity + 1);
        makeEmpty();
        assert(isEmpty());
    }

    /**
     * Constructor to initialize a priority queue based on a given vector V
     * Assumes V[0] is not used
     */
    explicit PriorityQueue(const std::vector<Comparable>& V) : pq{V} {
        heapify();
#ifdef TEST_PRIORITY_QUEUE
        assert(isMinHeap());
#endif
    }

    /**
     * Make the queue empty
     */
    void makeEmpty() {
        pq.clear();
        pq.push_back(Comparable{});
    }

    /**
     * Check is the queue is empty
     * Return true if the queue is empty, false otherwise
     */
    bool isEmpty() const {
        return pq.size() == 1;  // slot zero is not used
    }

    /**
     * Get the size of the queue, i.e. number of elements in the queue
     */
    size_t size() const { return pq.size() - 1; }

    /**
     * Get the smallest element in the queue
     */
    Comparable findMin() {
        assert(isEmpty() == false);
        return pq[1];
    }

    /**
     * Remove and return the smallest element in the queue
     */
    Comparable deleteMin();

    /**
     * Add a new element x to the queue
     */
    void insert(const Comparable& x);

    /**
     * Insert element x in the end of the queue, without preserving the heap property
     */
    void toss(const Comparable& x);

private:
    std::vector<Comparable> pq;  // slot with index 0 not used
    bool orderOK;  // flag to keep internal track of when the heap is ordered / not ordered

    // Auxiliary member functions

    /**
     * Restore the heap-ordering property
     */
    void heapify();

    void percolateDown(size_t i);

    /**
     * Test whether pq is a min heap
     */
    bool isMinHeap() const {
        // TO BE IMPLEMENTED

        // if (isEmpty()) {
        //     return false;
        // }

        // for (std::size_t i = 1; i < pq.size()/2; i++) {

        //    if ((2 * i < pq.size()-1) && (pq[2 * i] <= pq[i])) {
        //        return false;
        //    }
        //    if ((((2 * i) + 1) < pq.size()-1) && (pq[(2 * i) + 1] <= pq[i])) {
        //        return false;
        //    }
        //}

        // return true;

        bool isMin = true;
        size_t i = 1;  // Start from the root

        int leftChild;
        int rightChild;

        while (i <= pq.size()) {  // Only need to check up to the last parent node

            leftChild = 2 * i;       // Left child index
            rightChild = 2 * i + 1;  // Right child index

            // Check if current node is greater than its left child
            if (leftChild < pq.size() && pq[i] > pq[leftChild]) {
                isMin = false;
                break;
            }
            // Check if current node is greater than its right child
            if (rightChild < pq.size() && pq[i] > pq[rightChild]) {
                isMin = false;
                break;
            }
            i++;
        }

        return isMin;
    }
};

template <class Comparable>
void PriorityQueue<Comparable>::percolateDown(size_t i) {
    Comparable temp = pq[i];  // temp is value at position i in pq
    auto c = 2 * i;           // left child

    while (c < pq.size()) {  // test
        if (c < pq.size() - 1) {
            if (pq[c + 1] < pq[c])  // largest child?
                c++;
        }
        // percolate down
        if (pq[c] < temp) {
            pq[i] = pq[c];
            i = c;
            c = 2 * i;
        } else {
            break;
        }
    }
    pq[i] = temp;
}

/**
 * Restore the heap property
 */
template <class Comparable>
void PriorityQueue<Comparable>::heapify() {
    assert(pq.size() > 1);  // slot zero is not used

    for (size_t i = (pq.size() - 1) / 2; i >= 1; --i) {
        percolateDown(i);
    }
    orderOK = true;
}

/**
 * Remove and return the smallest element in the queue
 */
template <class Comparable>
Comparable PriorityQueue<Comparable>::deleteMin() {
    assert(!isEmpty());

    if (!orderOK) {
        heapify();
    }

    Comparable x = pq[1];
    Comparable y = pq[pq.size() - 1];

    pq[1] = y;  // set last element in the heap as the new root
    percolateDown(1);
    pq.pop_back();

#ifdef TEST_PRIORITY_QUEUE
    assert(isMinHeap());
#endif
    return x;
}

/**
 * Insert element x on the last slot, without preserving the heap property
 */
template <class Comparable>
void PriorityQueue<Comparable>::toss(const Comparable& x) {
    orderOK = false;
    pq.push_back(x);
}

/**
 * Add a new element x to the queue
 */
template <class Comparable>
void PriorityQueue<Comparable>::insert(const Comparable& x) {
    // TO BE IMPLEMENTED
   // toss(x);  // delete this line 

    //insert x at the end of the vector
    pq.push_back(x); 

    //create a hole for x at the end of the vector
    int empty_slot = pq.size()-1;

    //move x to root
    pq[0] = std::move(x); 

    //move hole upwards while x < parent -> parent = i/2 for any index i 
    for (; x < pq[empty_slot / 2]; empty_slot /= 2) { // division by 2 is cheap -> bit shifts 

        //swap positions with parent while x is smaller than current parent node 
        pq[empty_slot] = std::move(pq[empty_slot/2]); 
    }

    //insert x at the empty slot, remember that copy of x was assigned to the root pq[0]
    pq[empty_slot] = std::move(pq[0]); 


#ifdef TEST_PRIORITY_QUEUE  // do not delete
    /*if (!orderOK) {
        heapify();
    }*/
    assert(isMinHeap());
#endif
}
