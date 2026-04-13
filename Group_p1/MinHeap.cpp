#include "MinHeap.h"
#include <iostream>
using namespace std;

// ── insert ────────────────────────────────────────────────────────────────────
// Add val to the end of the array, then bubble it UP to the right spot
template <typename T>
void MinHeap<T>::insert(T val) {
    data.push_back(val);          // add to the end
    percolate_up(data.size() - 1); // bubble it up
}

// ── delete_min ────────────────────────────────────────────────────────────────
// The minimum is always at index 0
// Swap it with the last element, remove last, then push new root DOWN
template <typename T>
T MinHeap<T>::delete_min() {
    T min_val = data[0];                    // save the minimum to return
    data[0] = data[data.size() - 1];        // move last element to the top
    data.pop_back();                        // remove the last element
    if (!data.empty()) {
        percolate_down(0);                  // push the new root down
    }
    return min_val;
}

// ── percolate_up ──────────────────────────────────────────────────────────────
// Element at index i is too small — swap it with its parent until it's in place
template <typename T>
void MinHeap<T>::percolate_up(int i) {
    // Keep going while we are not at the root (index 0)
    while (i > 0) {
        int parent = (i - 1) / 2; // parent is always at (i-1)/2
        if (data[i] < data[parent]) {
            swap_elements(i, parent); // swap with parent if we are smaller
            i = parent;               // move up to where parent was
        } else {
            break; // we are in the right place — stop
        }
    }
}

// ── percolate_down ────────────────────────────────────────────────────────────
// Element at index i is too big — swap it with its smallest child until in place
template <typename T>
void MinHeap<T>::percolate_down(int i) {
    int n = data.size();

    while (true) {
        int left  = 2 * i + 1; // left child index
        int right = 2 * i + 2; // right child index
        int smallest = i;       // assume current is smallest

        // Is left child smaller than current?
        if (left < n && data[left] < data[smallest]) {
            smallest = left;
        }
        // Is right child smaller than current smallest?
        if (right < n && data[right] < data[smallest]) {
            smallest = right;
        }

        if (smallest != i) {
            swap_elements(i, smallest); // swap with the smallest child
            i = smallest;               // continue from new position
        } else {
            break; // we are in the right place — stop
        }
    }
}