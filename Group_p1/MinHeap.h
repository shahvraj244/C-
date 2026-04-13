#ifndef MINHEAP_H
#define MINHEAP_H

#include <vector>
using namespace std;

// MinHeap always keeps the SMALLEST element at the top
// We use this in Dijkstra and Prim to always pick the shortest/cheapest edge next
template <typename T>
class MinHeap {
public:
    vector<T> data; // stores all elements in heap order

    // Check if heap has no elements
    bool isEmpty() {
        return data.size() == 0;
    }

    // Add a new element to the heap
    void insert(T val);

    // Remove and return the smallest element
    T delete_min();

private:
    // Move element at index i UP until heap order is restored
    void percolate_up(int i);

    // Move element at index i DOWN until heap order is restored
    void percolate_down(int i);

    // Swap two elements
    void swap_elements(int i, int j) {
        T temp = data[i];
        data[i] = data[j];
        data[j] = temp;
    }
};

#endif