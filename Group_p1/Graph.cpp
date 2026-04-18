#include "Graph.h"
#include "MinHeap.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
//#include <algorithm>

using namespace std;

#define INT_MAX 1000000

/*
Description: Inserts a new vertex into the graph if it does not exist. 
Parameter: ver - the vertex to insert 
Return: void - Nothing 
*/
template <typename T>
void Graph<T>::insert_vertex(const Vertex<T>& ver) {
    if(get_vertex_index(ver) == -1) {
        vertices.push_back(ver); 
        vector<Edge> tmp; 
        edges.push_back(tmp); 
    }
}
/*
Description: Searches for a vertex in the graph and returns its index 
Parameter: ver - vertex to search for 
Return: index of vertex else -1 
*/
template <typename T> 
int Graph<T>::get_vertex_index(const Vertex<T>& ver) {
    for(int i = 0; i < vertices.size(); i++) {
        if(vertices[i].getData() == ver.getData()) {
            return i; 
        }
    }
    return -1; 
}
/*
Description: Add a weighed edge between two vertices
Parameter: ver1- source vertex, ver2 - destination vertex, weight - edge weight
Return: void - nothing 
*/
template <typename T> 
void Graph<T>::add_edge(const Vertex<T>& ver1, const Vertex<T>& ver2, int weight) {
    int i1 = get_vertex_index(ver1);
    int i2 = get_vertex_index(ver2);
    if(i1 == -1 || i2 == -1) {
        throw string("Add edge: incorrect vertices\n"); 
    }
    Edge v(i1, i2, weight);
    Edge v2(i2, i1, weight);
    edges[i1].push_back(v);
    if(i1 != i2) {
        edges[i2].push_back(v2); 
    }
}
/*
Description: Perform Depth First Search starting from a given vertex
Parameter: ver - starting vertex for DFS
Return: void - nothing 
*/
template <typename T> 
void Graph<T>::DFS(Vertex<T>& ver) {
    clean_visited();
    DFS_helper(ver);
    clean_visited();
}
/*
Description: Perform Breadth first search starting from a given vertex 
Parameter: ver - starting vertex for BFS
Return: void - nothing 
*/
template <typename T> 
void Graph<T>::BFS(Vertex<T>& ver) {
    clean_visited();
    int start_index = get_vertex_index(ver); 
    if(start_index == -1) {
        throw string("BFS: incorrect vertex\n"); 
    }
    vector<int> q; 
    int head = 0; 

    q.push_back(start_index);
    vertices[start_index].setVisited(true);

    while(head < q.size()) {
        int i = q[head++]; 
        cout<<vertices[i].getData() << " "; 

        for(int j = 0; j < edges[i].size(); j++) {
            int adjacent_ver = edges[i][j].dest;
            if(vertices[adjacent_ver].getVisited() == false) {
                vertices[adjacent_ver].setVisited(true); 
                q.push_back(adjacent_ver); 
            }
        }
    }
    clean_visited();
}
/*
Description: Helper function for DFS that performs the recursive calls to visit adjacent vertices
Parameter: ver - the vertex to start DFS from 
Return: void - nothing 
*/
template <typename T> 
void Graph<T>::DFS_helper(Vertex<T>& ver) {
    int i = get_vertex_index(ver); 
    if(i == -1) 
        return;
    vertices[i].setVisited(true);
    cout << vertices[i].getData() << " ";
    for(int j = 0; j < edges[i].size(); j++) {
        int adjacent_ver = edges[i][j].dest;
        if(vertices[adjacent_ver].getVisited() == false) {
            DFS_helper(vertices[adjacent_ver]); 
        }
    }
}
/*
Description: Resets the visited status of all vertices in the graph to false. 
This is a helper function used before and after performing graph traversals 
like DFS and BFS to ensure that the visited status is accurate for each traversal.
Parameter: none
Return: void - nothing 
*/
template <typename T>
void Graph<T>::clean_visited() {
    for(int i = 0; i < vertices.size(); i++) {
        vertices[i].setVisited(false); 
    }
}

/*
Description: Read the information from the dataset (a csv file) and create a weighted directed graph G. 
Consider two weights for each edge which are Distance and Cost. 
Parameter: filename - the name of the CSV file to read
Return: void - nothing
*/
template<typename T>
Graph<T>::Graph(const string& filename) : cost_graph_data(nullptr) {
    ifstream file(filename);
    string line;

    if(!file.is_open()) {
        throw string("Could not open file\n");
    }
    getline(file, line); // skip header

    while(getline(file, line)) {
        if(line.empty()) continue;

        string fields[6];
        size_t start = 0;

        for(int i = 0; i < 6; i++) {
            if(start < line.length() && line[start] == '"') {
                start++;
                size_t end = line.find('"', start);
                fields[i] = line.substr(start, end - start);
                start = line.find(',', end) + 1;
            } else {
                size_t next = line.find(',', start);
                fields[i] = line.substr(start, next - start);
                start = next + 1;
            }
        }

        // Store "CODE (City, ST)" so state is searchable
        string label1 = fields[0] + " (" + fields[2] + ")";
        string label2 = fields[1] + " (" + fields[3] + ")";

        // After parsing fields...
        Vertex<T> v1(fields[0]); // "ATL" — unchanged
        Vertex<T> v2(fields[1]); // "TPA" — unchanged

        int distance = stoi(fields[4]);
        int cost     = stoi(fields[5]);

        // Insert vertices and track cities
        if(get_vertex_index(v1) == -1) {
            insert_vertex(v1);
            vertex_cities.push_back(fields[2]); // "Atlanta, GA"
        }
        if(get_vertex_index(v2) == -1) {
            insert_vertex(v2);
            vertex_cities.push_back(fields[3]); // "Tampa, FL"
        }

        int i1 = get_vertex_index(v1);
        int i2 = get_vertex_index(v2);
        if(i1 != -1 && i2 != -1) {
            edges[i1].push_back(Edge(i1, i2, distance, cost));
            if(i1 != i2)
                edges[i2].push_back(Edge(i2, i1, distance, cost));
        }
    }
    file.close();
}

/*
Description: Find the shortest path between the given origin airport and destination airport. The algorithm
should output both the path and the length of the path. The algorithm should provide the
appropriate message if such path doesn't exist.
Parameter: src - source vertex, dest - destination vertex
Return: Shortest distance to destination
*/
template<typename T>
int Graph<T>::dijkstra_shortest_path(const Vertex<T>& src, const Vertex<T>& dest) {
    int i_src = get_vertex_index(src); //get index of source
    int i_dest = get_vertex_index(dest); //get index of dest

    if (i_src == -1 || i_dest == -1) { //check if vertices exist
        throw string("Shortest path: incorrect vertices\n");
    }

    clean_visited(); //reset visited status of vertices before running Dijkstra
    vector<int> distances(vertices.size(), INT_MAX); //distance as infinity
    vector<int> parent(vertices.size(), -1); //parent array to reconstruct path 
    vector<int> total_costs(vertices.size(), 0); //total cost to reach each vertex 
    distances[i_src] = 0;

    MinHeap<Edge> heap; //using min heap to store edges for shortest distance 
    int cur_ver = i_src; //current vertex index
    int visited_count = 0;//count of visited vertices
    while (visited_count < vertices.size()) {//unvisited vertices
        int u = cur_ver;
        for (int j = 0; j < edges[u].size(); j++) {//iterate through adjacent edges of cur_vertex
            int v = edges[u][j].dest;
            //if vertex not visited, add edge to heap and check 
            //if path through u is shorter than current known path to v
            if (!vertices[v].getVisited()) {
                heap.insert(edges[u][j]); 
                int new_dist = distances[u] + edges[u][j].weight;
                //if new path is shorter, update distance, parent, and total cost to reach v
                if (new_dist < distances[v]) {
                    distances[v] = new_dist;
                    parent[v] = u;
                    total_costs[v] = total_costs[u] + edges[u][j].cost;
                }
            }
        }
        vertices[u].setVisited(true); //mark current vertex as visited 
        visited_count++; //increment visited count 
        // get next vertex from heap
        while (!heap.isEmpty()) {
            Edge e = heap.delete_min();
            if (!vertices[e.dest].getVisited()) {
                cur_ver = e.dest;
                break;
            }
        }
        // if heap is empty → no more reachable nodes
        if (heap.isEmpty()) break;
    }
    //Output design for shortest path and distance to destination
    cout << "Shortest route from " << src.getData() << " to " << dest.getData() << ": ";
    if (distances[i_dest] == INT_MAX) {
        cout << "None" << endl;
    } else {
        vector<T> path; //reconstruct path from dest to source using parent arr
        for (int cur = i_dest; cur != -1; cur = parent[cur]) {
            path.push_back(vertices[cur].getData());
        }
        //source to dest path 
        for (int i = path.size() - 1; i >= 0; i--) {
            cout << path[i] << (i == 0 ? "" : " -> ");
        }
        //from document sample output section
        cout << ". The length is " << distances[i_dest]
             << ". The cost is " << total_costs[i_dest] << "." << endl;
    }
    clean_visited();//reset visited status after running Dijkstra
    return distances[i_dest];//return shortest distance to destination
}


/*
Description: Find all shortest paths between the given origin airport and all the airports in the destination state.
The algorithm should output all the paths and their lengths. The algorithm should provide the
appropriate message if no such paths exist
Parameter: src - source vertex, state - dest state to search for airports 
Return: void - nothing
*/
template<typename T>
void Graph<T>::short_paths_state(const Vertex<T>& src, const string& state) {
    int i_src = get_vertex_index(src);
    if (i_src == -1) {
        cout << "Origin airport not found." << endl;
        return;
    }

    clean_visited();
    vector<int> distances(vertices.size(), INT_MAX);
    vector<int> parent(vertices.size(), -1);
    vector<int> total_costs(vertices.size(), 0);
    distances[i_src] = 0;

    MinHeap<Edge> heap;
    int cur_ver = i_src;
    int visited_count = 0;

    while (visited_count < vertices.size()) {
        int u = cur_ver;
        for (int j = 0; j < edges[u].size(); j++) {
            int v = edges[u][j].dest;
            if (!vertices[v].getVisited()) {
                heap.insert(edges[u][j]);
                int new_dist = distances[u] + edges[u][j].weight;
                if (new_dist < distances[v]) {
                    distances[v] = new_dist;
                    parent[v] = u;
                    total_costs[v] = total_costs[u] + edges[u][j].cost;
                }
            }
        }
        vertices[u].setVisited(true);
        visited_count++;
        bool found_next = false;
        while (!heap.isEmpty()) {
            Edge e = heap.delete_min();
            if (!vertices[e.dest].getVisited()) {
                cur_ver = e.dest;
                found_next = true;
                break;
            }
        }
        if (!found_next) break;
    }
    bool found = false;
    for (int i = 0; i < vertices.size(); i++) {
        if (vertex_cities[i].find(state) != string::npos) {
            if (distances[i] != INT_MAX) {
                found = true;
                vector<T> path;
                for (int cur = i; cur != -1; cur = parent[cur])
                    path.push_back(vertices[cur].getData());

                cout << "Shortest route to " << vertices[i].getData()
                     << " (" << vertex_cities[i] << "): ";
                for (int p = path.size() - 1; p >= 0; p--)
                    cout << path[p] << (p == 0 ? "" : " -> ");
                cout << ". The length is " << distances[i]
                     << ". The cost is " << total_costs[i] << "." << endl;
            }
        }
    }
    if (!found)
        cout << "No paths found to " << state << " state airports." << endl;

    clean_visited();
}
/*
Description:Find the shortest path between the given origin airport and destination airport with a given
number of stops. The algorithm should provide the appropriate message if such path doesn’t exist.
Parameter: src - source vertex, dest - destination vertex, stops - number of stops allowed in path
Return: void - nothing
*/
template<typename T>
void Graph<T>::short_path_stops(const Vertex<T>& src, const Vertex<T>& dest, int stops) {
    int i_src = get_vertex_index(src); //source vertex 
    int i_dest = get_vertex_index(dest); //destination vertex 
    if(i_src == -1 || i_dest == -1) {//cehck for a vetex if it exists in graph 
        cout<<"Shortest route from "<<src.getData()<<" to "<<dest.getData()<<" with "<<stops<<" stops: None"<<endl;
        return;
    }
    int edges_needed = stops + 1; // n stops = n + 1 edges
    vector<vector<int>> dist(vertices.size(), vector<int>(edges_needed + 1, INT_MAX)); //shortest distance to v using edges
    vector<vector<int>> parent(vertices.size(), vector<int>(edges_needed + 1, -1)); //reconsturct path using parent arr 
    dist[i_src][0] = 0;
    //find shortest path for each edge count edges
    for(int e = 1; e <= edges_needed; e++) { //edge count 
        for(int u = 0; u < vertices.size(); u++) { //traverse through vertices to find shortest path to vertex edge 
            if(dist[u][e-1] == INT_MAX) continue; //if no path to u with e-1 edges then skip 
            for(int j = 0; j < edges[u].size(); j++) { //traverse through adjacent edges of u to find path 
                int v = edges[u][j].dest; //destination vertex of edge 
                int weight = edges[u][j].weight; //weight of edge
                if(dist[u][e-1] != INT_MAX && dist[u][e-1] + weight < dist[v][e]) {
                    dist[v][e] = dist[u][e-1] + weight;
                    parent[v][e] = u;
                }
            }
        }
    }
    //output design for shortest path with stops 
    cout<<"Shortest route from "<<src.getData()<<" to "<<dest.getData()<<" with "<<stops<<" stops: ";
    if(dist[i_dest][edges_needed] == INT_MAX) { //if no path found the none as output 
        cout<<"None"<<endl;
    } else { 
        vector<T> path;
        int cur = i_dest, e = edges_needed; 
        while(cur != -1 && e >= 0) {//reconstruct path from dest to source using parent arr
            path.push_back(vertices[cur].getData());//add current vertex to path 
            cur = parent[cur][e];
            e--;
        }
        for(int i = path.size() - 1; i >= 0; i--) { //output path from source to dest 
            cout << path[i] << (i == 0 ? "" : " -> ");
        }
        int total_cost = 0; //calc total cost of path by traversing through parrent arr and sum cost of edges 
        int temp_v = i_dest; 
        for(int e = edges_needed; e > 0; e--) { //find parent vertex and add cost 
            int par = parent[temp_v][e];
            for(int j = 0; j < edges[par].size(); j++) {
                if(edges[par][j].dest == temp_v) {
                    total_cost += edges[par][j].cost;
                    break;
                }
            }
            temp_v = par;
        }
        cout<<". The lenght is "<<dist[i_dest][edges_needed]<<". The cost is "<<total_cost<<"."<<endl;
    }
}

/*
5. Count and display total direct flight connections to each airport. You should consider both
outbound and inbound flights. For instance, if you can directly fly to Tampa airport only from
Miami, Orlando, and Atlanta, the inbound count for Tampa airport would be 3. If you can directly
fly from Tampa airport only to New York, the outbound count for Tampa airport is 1. The total
number of direct flight connections for Tampa airport is 4. The list of airports should be sorted
based on the total direct flight connections count, starting with the airports having the highest
number of direct flight connections.
*/

/*
Description:
Parameter:
Return:
*/
/*template <typename T>
void Graph<T>::disp_connections_sort() {
    vector<int> inbound(vertices.size(), 0);
    vector<int> outbound(vertices.size(), 0);

    for(int u = 0; u < edges.size(); u++) {
        outbound[u] = edges[u].size();
        for(int j = 0; j < edges[u].size(); j++) {
            inbound[edges[u][j].dest]++;
        }
    }

    vector<pair<int, int>> connection_counts;
    connection_counts.reserve(vertices.size());
    for(int i = 0; i < vertices.size(); i++) {
        connection_counts.push_back(make_pair(inbound[i] + outbound[i], i));
    }

    sort(connection_counts.begin(), connection_counts.end(),
        [](const pair<int, int>& a, const pair<int, int>& b) {
            if(a.first != b.first) {
                return a.first > b.first;
            }
            return a.second < b.second;
        });

    cout << "Airports sorted by total direct flight connections:" << endl;
    for(int i = 0; i < connection_counts.size(); i++) {
        int vertex_index = connection_counts[i].second;
        cout << vertices[vertex_index].getData() << ": " << connection_counts[i].first << endl;
    }
}*/

template <typename T>
void Graph<T>::disp_connections_sort() {

    vector<int> inbound(vertices.size(), 0);
    vector<int> outbound(vertices.size(), 0);

    // Count inbound & outbound
    for(int u = 0; u < edges.size(); u++) {
        outbound[u] = edges[u].size();
        for(int j = 0; j < edges[u].size(); j++) {
            inbound[edges[u][j].dest]++;
        }
    }

    // Use MinHeap of Edge
    MinHeap<Edge> heap;

    // Insert into heap
    for(int i = 0; i < vertices.size(); i++) {
        int total = inbound[i] + outbound[i];

        // store total in weight (NEGATIVE for descending)
        Edge e(0, i, -total, 0);
        heap.insert(e);
    }

    cout << "Airports sorted by total direct flight connections:" << endl;

    // Extract in sorted order
    while(!heap.isEmpty()) {
        Edge e = heap.delete_min();

        int index = e.dest;
        int total = -e.weight; // convert back

        cout << vertices[index].getData() << ": " << total << endl;
    }
}
/*
6. Create an undirected graph G_u from the original directed graph G using the following rules:
a. For each pair of vertices u and v, if there is only one directed edge(either (u,v) or (v,u))
between them, you keep that single edge with its corresponding cost as an undirected
weighted edge. You can ignore the distance on that edge.
b. For each pair of vertices u and v, if there are two directed edges (u,v) and (v, u) between
them, you keep the one with the minimum cost value as an undirected weighted edge. You
can ignore the distance on that edge.
*/
/*
Description:
Parameter:
Return:
*/
template <typename T>
void Graph<T>::cost_graph() {
    if(cost_graph_data != nullptr) {
        delete cost_graph_data;
    }
    cost_graph_data = new Graph<T>();
    const int n = vertices.size();
    cost_graph_data->vertices = vertices;
    cost_graph_data->edges.assign(n, vector<Edge>());
    vector<vector<int>> best_cost(n, vector<int>(n, -1)); 
    for(int u = 0; u < n; u++) {
        for(int j = 0; j < edges[u].size(); j++) {
            int v = edges[u][j].dest;
            int a = min(u,v);
            int b = max(u,v);
            int cost = edges[u][j].cost;
            if(best_cost[a][b] == -1 || cost < best_cost[a][b]) {
                best_cost[a][b] = cost;
            }
        }
    }
    for (int u = 0; u < n; u++) {
        for(int v = u; v < n; v++) {
            if(best_cost[u][v] != -1) {
                cost_graph_data->add_edge(cost_graph_data->vertices[u], cost_graph_data->vertices[v], best_cost[u][v]);
            }
        }
    }
}

/*
7.Generate a Minimal Spanning Tree utilizing Prim’s algorithm on G_u that you created in the
previous step. The algorithm will output both the content of the constructed MST and its total cost.
In this step, for each edge you need to consider the cost as weight to minimize the total cost. In the
event of a disconnected graph, the algorithm will appropriately notify that an MST cannot be
formed. Note: A connected graph is defined as one where there exists a path between every pair of
vertices.
*/
/*
Description:
Parameter:
Return:
*/

template <typename T>
void Graph<T>::prim_mst() {

    if(cost_graph_data == nullptr) {
        cost_graph();
    }
    Graph<T> g = *cost_graph_data;  // get undirected graph

    int n = g.vertices.size();
    if(n == 0) {
        cout << "MST cannot be formed." << endl;
        return;
    }

    vector<bool> inMST(n, false);
    MinHeap<Edge> heap;

    int total_cost = 0;
    int edges_used = 0;

    // Start from vertex 0
    inMST[0] = true;

    // Push all edges from vertex 0
    for(int i = 0; i < g.edges[0].size(); i++) {
        Edge e = g.edges[0][i];
        heap.insert(e);
    }

    cout << "Prim MST:" << endl;

    while(!heap.isEmpty() && edges_used < n - 1) {

        Edge e = heap.delete_min();

        int u = e.src;
        int v = e.dest;

        // Skip if both already in MST
        if(inMST[u] && inMST[v]) {
            continue;
        }

        int new_vertex;
        if(inMST[u]) {
            new_vertex = v;
        } else {
            new_vertex = u;
        }

        cout << g.vertices[u].getData()
             << " - "
             << g.vertices[v].getData()
             << ": "
             << e.weight << endl;

        total_cost += e.weight;
        edges_used++;

        inMST[new_vertex] = true;

        // Add edges from new vertex
        for(int i = 0; i < g.edges[new_vertex].size(); i++) {
            Edge next = g.edges[new_vertex][i];
            if(!inMST[next.dest]) {
                heap.insert(next);
            }
        }
    }

    if(edges_used != n - 1) {
        cout << "MST cannot be formed." << endl;
        return;
    }

    cout << "Total cost: " << total_cost << endl;
}
/*
Description:Generate a Minimal Spanning Tree using Kruskal’s algorithm on G_u that you created in the
previous step. The algorithm will output both the content of the constructed MST and its total cost.
In this step, for each edge you need to consider the cost as weight to minimize the total cost. If the
graph is disconnected the algorithm should provide minimum spanning forest consisting of a
minimum spanning tree for each connected component
Parameter:
Return:
*/

template<typename T>
void Graph<T>::kruskal_mst() {

    if(cost_graph_data == nullptr) {
        cost_graph();
    }

    const int n = cost_graph_data->vertices.size();
    if(n == 0) return;

    MinHeap<Edge> heap;

    // Insert all edges (only one direction)
    for(int u = 0; u < n; u++) {
        for(int j = 0; j < cost_graph_data->edges[u].size(); j++) {
            Edge e = cost_graph_data->edges[u][j];

            if(e.src < e.dest) {
                heap.insert(e);
            }
        }
    }

    vector<int> parent(n);
    for(int i = 0; i < n; i++) {
        parent[i] = i;
    }

    vector<Edge> mst_edges;
    int total_cost = 0;

    while(!heap.isEmpty() && mst_edges.size() < n - 1) {

        Edge current_edge = heap.delete_min();

        int root_u = find_set(parent, current_edge.src);
        int root_v = find_set(parent, current_edge.dest);

        if(root_u != root_v) {
            union_set(parent, root_u, root_v);

            mst_edges.push_back(current_edge);
            total_cost += current_edge.weight;
        }
    }

    cout << "Kruskal MST:" << endl;

    for(int i = 0; i < mst_edges.size(); i++) {
        cout << cost_graph_data->vertices[mst_edges[i].src].getData()
             << " - "
             << cost_graph_data->vertices[mst_edges[i].dest].getData()
             << ": "
             << mst_edges[i].weight << endl;
    }

    cout << "Total cost: " << total_cost << endl;
}

/*
Description:
Parameter:
Return:
*/
template<typename T>
int Graph<T>::find_set(vector<int>& parent, int i) {
    if(parent[i] != i) {
        parent[i] = find_set(parent, parent[i]);
    }
    return parent[i];
}
/*
Description:
Parameter:
Return:
*/
template<typename T>
void Graph<T>::union_set(vector<int>& parent, int i, int j) {
    parent[find_set(parent, i)] = find_set(parent, j);
}
