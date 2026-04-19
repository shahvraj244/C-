#include <iostream>
#include <string>
#include "Graph.h"
#include "Vertex.h"
#include "Graph.cpp"
#include "MinHeap.cpp"

using namespace std;

void disp() {
    cout<<"\n----Airport System-----\n";
    cout<<"1. Dijkstra Shortest Path\n";
    cout<<"2. Shortest Paths to State\n";
    cout<<"3. Shortest Path with Stops\n";
    cout<<"4. Display all Connections\n";
    cout<<"5. Prim's Minimum Spanning Tree\n";
    cout<<"6. Kruskal's Minimum Spanning Tree\n";
    cout<<"0. Exit\n";
    cout<<"Enter choice: ";
}
int main() {
    Graph<string> airportGraph("airports.csv");
    int choice = -1;
    string src, dest, state;
    int stops; 

    cout<<"--- Graph-Based Airport Connectivity System Loaded ---\n";
    while(choice != 0) {
        disp();
        cin>>choice;

        switch(choice) {
            case 1:
                cout << "Enter Source Airport (e.g., IAD): ";
                cin >> src;
                cout << "Enter Destination Airport (e.g., MIA): ";
                cin >> dest;
                airportGraph.dijkstra_shortest_path(Vertex<string>(src), Vertex<string>(dest));
                break;
            case 2:
                cout << "Enter Source Airport (e.g., IAD): ";
                cin >> src;
                cout << "Enter State Abbreviation (e.g., FL): ";
                cin >> state;
                airportGraph.short_paths_state(Vertex<string>(src), state);
                break;
            case 3:
                cout << "Enter Source Airport (e.g., ATL): ";
                cin >> src;
                cout << "Enter Destination Airport (e.g., TPA): ";
                cin >> dest;
                cout << "Enter Max Stops (e.g., 2): ";
                cin >> stops;
                airportGraph.short_path_stops(Vertex<string>(src), Vertex<string>(dest), stops);
                break;
            case 4:
                airportGraph.disp_connections_sort();
                break;
            case 5:
                airportGraph.cost_graph();
                airportGraph.prim_mst();
                break;
            case 6:
                airportGraph.kruskal_mst();
                break;
            default:
                cout<<"Invalid choice!\n";
        }
    }
    return 0; 
}



/*
//Test main function
int main() {
    cout << "--- Graph-Based Airport Connectivity and Flight Route Optimization System ---" << endl;
    
    //Call airports.csv file
    Graph<string> airportGraph("airports.csv");
    cout << endl;

    //Vertex airports test
    Vertex<string> vATL("ATL");
    Vertex<string> vIAD("IAD");
    Vertex<string> vMIA("MIA");
    Vertex<string> vORD("ORD");
    Vertex<string> vPIT("PIT");
    Vertex<string> vACT("ACT");
    Vertex<string> vTPA("TPA");
    Vertex<string> vMCO("MCO");
    Vertex<string> vJFK("JFK");

    //Task 2: Dijkstra Shortest Path
    cout << "--- Task 2: Dijkstra Shortest Path ---" << endl;
    airportGraph.dijkstra_shortest_path(vIAD, vMIA);

    cout << "Shortest route from PIT to ACT: ";
    airportGraph.dijkstra_shortest_path(vPIT, vACT);
    cout << endl;

    //Task 3: Shortest Paths to State
    cout << "--- Task 3: Shortest Paths to State ---" << endl;
    cout << "Shortest paths from ATL to FL state airports are:" << endl;
    airportGraph.short_paths_state(vATL, "FL");
    cout << endl;

    //Task 4: Shortest Path with Stops
    cout << "--- Task 4: Shortest Path with Stops ---" << endl;
    airportGraph.short_path_stops(vIAD, vMIA, 3);

    airportGraph.short_path_stops(vPIT, vACT, 2);
    cout << endl;

    //Task 5: Direct Flight Connection
    cout << "--- Task 5: Direct Flight Connection ---" << endl;
    airportGraph.disp_connections_sort();
    cout << endl;

    //Task 6: Prim's Minimum Spanning Tree
    airportGraph.cost_graph(); 
    cout << "--- Task 6: Prim's Minimum Spanning Tree ---" << endl;
    airportGraph.prim_mst();
    cout << endl;

    //Task 7: Kruskal's Minimum Spanning Tree
    cout << "--- Task 7: Kruskal's Minimum Spanning Tree ---" << endl;
    airportGraph.kruskal_mst();
    cout << endl;

    return 0;
}*/