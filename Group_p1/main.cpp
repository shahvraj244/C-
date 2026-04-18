#include <iostream>
#include <string>
#include "Graph.h"
#include "Vertex.h"
#include "Graph.cpp"
#include "MinHeap.cpp"

using namespace std;

int main() {
    try {
        cout << "--- Graph-Based Airport Connectivity and Flight Route Optimization System ---" << endl;
        Graph<string> airportGraph("airports.csv");
        cout<<endl;
        
        Vertex<string> vATL("ATL"); 
        Vertex<string> vIAD("IAD"); 
        Vertex<string> vMIA("MIA"); 
        Vertex<string> vORD("ORD"); 
        Vertex<string> vPIT("PIT"); 
        Vertex<string> vACT("ACT"); 
        Vertex<string> vTPA("TPA");
        Vertex<string> vMCO("MCO");
        Vertex<string> vJFK("JFK");

        cout << "--- Task 2: Dijkstra Shortest Path ---" << endl;
        cout << "Shortest route from IAD to MIA: ";
        airportGraph.dijkstra_shortest_path(vIAD, vMIA);

        cout << "Shortest route from PIT to ACT: ";
        airportGraph.dijkstra_shortest_path(vPIT, vACT);
        cout << endl;

        cout << "--- Task 3: Shortest Paths to State ---" << endl;
        cout << "Shortest paths from ATL to FL state airports are:" << endl;
        airportGraph.short_paths_state(vATL, "FL");
        cout << endl;

        cout << "--- Task 4: Shortest Path with Stops ---" << endl;

        cout << "Shortest route from IAD to MI A with 3 stops: ";
        airportGraph.short_path_stops(vIAD, vMIA, 3);

        cout << "Shortest route from PIT to ACT with 2 stops: ";
        airportGraph.short_path_stops(vPIT, vACT, 2);

        cout << endl;

        cout << "--- Task 5: Direct Flight Connection ---" << endl;
        airportGraph.disp_connections_sort();
        cout << endl;
        
        airportGraph.cost_graph(); //define the cost_graph function 
        cout << "--- Task 6: Prim's Minimum Spanning Tree ---" << endl;
        
        airportGraph.prim_mst();
        cout << endl;
        
        cout << "--- Task 7: Kruskal's Minimum Spanning Tree ---" << endl;
        airportGraph.kruskal_mst();
        cout << endl;
    } catch (const string& e) {
        cout << "Error: " << e << endl;
    } catch (const exception& e) {
        cout << "Standard Exception: " << e.what() << endl;
    }

    return 0;
}