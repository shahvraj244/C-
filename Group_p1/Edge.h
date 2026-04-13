#ifndef EDGE_H
#define EDGE_H

// An Edge connects two vertices (by their index numbers)
// It stores the distance (miles) and cost (dollars) of the flight
class Edge {
public:
    int src;      // index of the starting airport
    int dest;     // index of the ending airport
    int distance; // flight distance in miles
    int cost;     // flight cost in dollars

    // Default constructor - sets everything to 0
    Edge() {
        src = 0;
        dest = 0;
        distance = 0;
        cost = 0;
    }

    // Constructor with values
    Edge(int s, int d, int dist, int c) {
        src = s;
        dest = d;
        distance = dist;
        cost = c;
    }

    // This lets MinHeap compare edges by distance (for Dijkstra and Prim)
    bool operator<(const Edge& other) const {
    return distance < other.distance;
}
};

#endif