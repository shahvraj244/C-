#ifndef VERTEX_H
#define VERTEX_H

#include <string>
using namespace std;

// A Vertex represents one airport in our graph
// It stores the airport code (like "ATL") and city name (like "Atlanta, GA")
template <typename T>
class Vertex {
public:
    T data;           // airport code e.g. "ATL"
    string city;      // full city name e.g. "Atlanta, GA"
    bool visited;     // used by DFS and BFS to track if we visited this airport

    // Default constructor
    Vertex() {
        visited = false;
    }

    // Constructor with just the airport code
    Vertex(T d) {
        data = d;
        city = "";
        visited = false;
    }

    // Constructor with airport code AND city name
    Vertex(T d, string c) {
        data = d;
        city = c;
        visited = false;
    }
};

#endif