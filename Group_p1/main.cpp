#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Graph.h"
#include "Graph.cpp"
#include "MinHeap.h"
#include "MinHeap.cpp"

using namespace std;

int main() {
    Graph<string> G("airports.csv");

    // --- Task 2 ---
    cout << "Task 2:\n";
    G.dijkstra_shortest_path(Vertex<string>("IAD"), Vertex<string>("MIA"));
    G.dijkstra_shortest_path(Vertex<string>("PIT"), Vertex<string>("ACT"));
    cout << "\n";

    // --- Task 3 ---
    cout << "Task 3:\n";
    cout << "Shortest paths from ATL to FL state airports are:\n";
    cout << "Path             Length    Cost\n";
    G.short_paths_state(Vertex<string>("ATL"), "FL");
    cout << "\n";

    // --- Task 4 ---
    cout << "Task 4:\n";
    G.short_path_stops(Vertex<string>("IAD"), Vertex<string>("MIA"), 4); // 4 edges = 3 stops
    G.short_path_stops(Vertex<string>("PIT"), Vertex<string>("ACT"), 3); // 3 edges = 2 stops
    cout << "\n";

    // --- Task 5: only print top 3 ---
    cout << "Task 5:\n";
    cout << "Airport        Connections\n";
    G.disp_connections_sort(3); // pass limit
    cout << "\n";

    // --- Tasks 7 & 8: only print first 3 MST edges ---
    // Task 7 header:
    cout << "Minimal Spanning Tree:\n";
    cout << "Edge           Weight\n";
    Graph<string> G_u = G.cost_graph();
    G_u.prim_mst(3); // pass limit
    cout << "\n";

    return 0;
}