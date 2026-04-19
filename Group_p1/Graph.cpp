#include "Graph.h"
#include "MinHeap.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
#define INT_MAX 1000000

/*
Description: Inserts a new vertex into the graph if it does not exist. 
Parameter: ver - the vertex to insert 
Return:  Nothing 
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
Return: nothing 
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
void Graph<T>::BFS(Vertex<T>& ver, bool print) {
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
        if(print) cout << vertices[i].getData() << " "; 

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
Return: nothing 
*/
template <typename T> 
void Graph<T>::DFS_helper(Vertex<T>& ver, bool print) {
    int i = get_vertex_index(ver); 
    if(i == -1) return;
    vertices[i].setVisited(true);
    if(print) cout << vertices[i].getData() << " ";
    for(int j = 0; j < edges[i].size(); j++) {
        int adjacent_ver = edges[i][j].dest;
        if(!vertices[adjacent_ver].getVisited()) {
            DFS_helper(vertices[adjacent_ver], print);
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
Return: nothing
*/
template<typename T>
Graph<T>::Graph(const string& filename):cost_graph_data(nullptr) {
    ifstream file(filename);//opens the file 
    string line; //check if file opened 
    if(!file.is_open()) {
        throw string("Could not open file\n");
    }
    getline(file, line); //skip the first line in airports.csv

    while(getline(file, line)) { //read each line of the file 
        if(line.empty()) continue;
        //store 6 fields like "Origin_airport,Destination_airport,
        //Origin_city,Destination_city,Distance,Cost"
        string fields[6]; 
        size_t start = 0;
        //using a for loop to handle the quoted fields which contain commas
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

        //Stores city & state (Tampa, FL)
        string label1 = fields[0] + " (" + fields[2] + ")";
        string label2 = fields[1] + " (" + fields[3] + ")";
        //Create vertices for origin and destination airports
        Vertex<T> v1(fields[0]);
        Vertex<T> v2(fields[1]);
        //using stoi to convert string to int "distance and cost"
        int distance = stoi(fields[4]);
        int cost = stoi(fields[5]);
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
        if(i1 != -1 && i2 != -1) { //if both exist then add to graph 
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
Description: Find all shortest paths between the given origin airport and all the airports in the 
destination state. DFS is first used to identify which airports in the target state are reachable 
from the source. Dijkstra then computes the shortest path and cost to each of those airports.
The algorithm outputs all paths and their lengths, or an appropriate message if no paths exist.
Parameter: src - source vertex, state - destination state abbreviation (e.g. "FL")
Return: nothing
*/
template<typename T>
void Graph<T>::short_path_state(const Vertex<T>& src, const string& state) {
    int i_src = get_vertex_index(src);
    if(i_src == -1) {
        cout << "Origin airport not found." << endl;
        return;
    }
    //DFS marks all verticies from source
    clean_visited();
    DFS_helper(vertices[i_src], false);

    //all reachable airports located in the target state
    vector<int> state_airports;
    for(int i = 0; i < vertices.size(); i++) {
        if(vertices[i].getVisited() && vertex_cities[i].find(state) != string::npos) {
            state_airports.push_back(i);
        }
    }
    clean_visited();
    if(state_airports.empty()) {
        cout << "No paths found to " << state << " state airports." << endl;
        return;
    }

    // Step 3: Dijkstra from source to compute shortest distance and cost to all vertices
    vector<int> distances(vertices.size(), INT_MAX);
    vector<int> parent(vertices.size(), -1);
    vector<int> total_costs(vertices.size(), 0);
    distances[i_src] = 0;

    MinHeap<Edge> heap;
    int cur_ver = i_src;
    int visited_count = 0;

    while(visited_count < vertices.size()) {
        int u = cur_ver;
        for(int j = 0; j < edges[u].size(); j++) {
            int v = edges[u][j].dest;
            if(!vertices[v].getVisited()) {
                heap.insert(edges[u][j]);
                int new_dist = distances[u] + edges[u][j].weight;
                if(new_dist < distances[v]) {
                    distances[v] = new_dist;
                    parent[v] = u;
                    total_costs[v] = total_costs[u] + edges[u][j].cost;
                }
            }
        }
        vertices[u].setVisited(true);
        visited_count++;
        bool found_next = false;
        while(!heap.isEmpty()) {
            Edge e = heap.delete_min();
            if(!vertices[e.dest].getVisited()) {
                cur_ver = e.dest;
                found_next = true;
                break;
            }
        }
        if(!found_next) break;
    }
    clean_visited();

    // Step 4: Output shortest path for each DFS-discovered state airport
    bool found = false;
    for(int i = 0; i < state_airports.size(); i++) {
        int idx = state_airports[i];
        if(distances[idx] != INT_MAX) {
            found = true;
            vector<T> path;
            for(int cur = idx; cur != -1; cur = parent[cur])
                path.push_back(vertices[cur].getData());
            cout << "Shortest route to " << vertices[idx].getData()
                 << " (" << vertex_cities[idx] << "): ";
            for(int p = path.size() - 1; p >= 0; p--)
                cout << path[p] << (p == 0 ? "" : " -> ");
            cout << ". The length is " << distances[idx]
                 << ". The cost is " << total_costs[idx] << "." << endl;
        }
    }
    if(!found)
        cout << "No paths found to " << state << " state airports." << endl;
}


/*
Description: Find the shortest path between the given origin airport and destination airport with
exactly a given number of stops. BFS is used with (vertex, edge_count) pairs in the queue to 
explore paths level by level, where each level represents one additional flight hop. This enforces
the exact stop count while tracking the shortest weighted distance. Outputs the path and cost, 
or an appropriate message if no path exists.
Parameter: src - source vertex, dest - destination vertex, stops - number of intermediate stops
Return: nothing
*/
template<typename T>
void Graph<T>::short_path_stops(const Vertex<T>& src, const Vertex<T>& dest, int stops) {
    int i_src = get_vertex_index(src);
    int i_dest = get_vertex_index(dest);

    if(i_src == -1 || i_dest == -1) {
        cout << "Shortest route from " << src.getData() << " to " << dest.getData()
             << " with " << stops << " stops: None" << endl;
        return;
    }

    int edges_needed = stops + 1; // n stops = n+1 edges
    int n = vertices.size();

    // dist[v][e] = shortest distance to reach vertex v using exactly e edges
    vector<vector<int>> dist(n, vector<int>(edges_needed + 1, INT_MAX));
    // parent[v][e] = predecessor of v when reached with exactly e edges
    vector<vector<int>> parent(n, vector<int>(edges_needed + 1, -1));
    dist[i_src][0] = 0;

    // BFS queue stores (vertex_index, edges_used) pairs
    // vector with head pointer simulates queue without STL queue
    vector<pair<int,int>> bfs_queue;
    bfs_queue.push_back({i_src, 0});
    int head = 0;

    // BFS: each level of the queue represents one additional hop
    while(head < bfs_queue.size()) {
        int u = bfs_queue[head].first;  // current vertex
        int e = bfs_queue[head].second; // edges used to reach u
        head++;

        if(e >= edges_needed) continue; // stop limit reached, do not expand further

        for(int j = 0; j < edges[u].size(); j++) {
            int v = edges[u][j].dest;
            int w = edges[u][j].weight;
            // If path through u to v with e+1 edges is shorter, update and enqueue
            if(dist[u][e] != INT_MAX && dist[u][e] + w < dist[v][e + 1]) {
                dist[v][e + 1] = dist[u][e] + w;
                parent[v][e + 1] = u;
                bfs_queue.push_back({v, e + 1});
            }
        }
    }

    cout << "Shortest route from " << src.getData() << " to " << dest.getData()
         << " with " << stops << " stops: ";

    if(dist[i_dest][edges_needed] == INT_MAX) {
        cout << "None" << endl;
    } else {
        // Reconstruct path by tracing parent array from dest back to src
        vector<T> path;
        int cur = i_dest;
        for(int step = edges_needed; step >= 0; step--) {
            path.push_back(vertices[cur].getData());
            if(step > 0) cur = parent[cur][step];
        }
        for(int i = path.size() - 1; i >= 0; i--)
            cout << path[i] << (i == 0 ? "" : " -> ");

        // Calculate total cost by retracing edges along reconstructed path
        int total_cost = 0;
        int temp_v = i_dest;
        for(int step = edges_needed; step > 0; step--) {
            int par = parent[temp_v][step];
            for(int j = 0; j < edges[par].size(); j++) {
                if(edges[par][j].dest == temp_v) {
                    total_cost += edges[par][j].cost;
                    break;
                }
            }
            temp_v = par;
        }
        cout << ". The length is " << dist[i_dest][edges_needed]
             << ". The cost is " << total_cost << "." << endl;
    }
}
/*
Description: Count and display total direct flight connections to each airport. You should consider both
outbound and inbound flights. For instance, if you can directly fly to Tampa airport only from
Miami, Orlando, and Atlanta, the inbound count for Tampa airport would be 3. If you can directly
fly from Tampa airport only to New York, the outbound count for Tampa airport is 1. The total
number of direct flight connections for Tampa airport is 4. The list of airports should be sorted
based on the total direct flight connections count, starting with the airports having the highest
number of direct flight connections.
Parameter: none
Return: nothing 
*/
template <typename T>
void Graph<T>::disp_connections_sort() {
    vector<int> inbound(vertices.size(), 0); //count inbound flights for each vertex 
    vector<int> outbound(vertices.size(), 0); //count outbound flights for each vertex 
    //traverse through edges to count inbound and outbound
    for(int u = 0; u < edges.size(); u++) {
        outbound[u] = edges[u].size();
        for(int j = 0; j < edges[u].size(); j++) {
            inbound[edges[u][j].dest]++;
        }
    }
    MinHeap<Edge> heap;//using minheap to sort airports by connections 
    for(int i = 0; i < vertices.size(); i++) { //insert into heap 
        int total = inbound[i] + outbound[i];
        Edge e(0, i, -total, 0);// store total in weight
        heap.insert(e);
    }
    cout << "Airports sorted by total direct flight connections:" << endl;
    while(!heap.isEmpty()) {//output airports for heap 
        Edge e = heap.delete_min();
        int index = e.dest; //get vertex index from edge
        int total = -e.weight; // total connections from weight 
        cout << vertices[index].getData() << ": " << total << endl;
    }
}

/*
Description: Create an undirected graph G_u from the original directed graph G using the following rules:
a. For each pair of vertices u and v, if there is only one directed edge(either (u,v) or (v,u))
between them, you keep that single edge with its corresponding cost as an undirected
weighted edge. You can ignore the distance on that edge.
b. For each pair of vertices u and v, if there are two directed edges (u,v) and (v, u) between
them, you keep the one with the minimum cost value as an undirected weighted edge. You
can ignore the distance on that edge.
Parameter: nothing
Return: nothing 
*/
template <typename T>
void Graph<T>::cost_graph() {
    if(cost_graph_data != nullptr) { //if graph exits then delete needed for Prim and Kruskal MST 
        delete cost_graph_data; 
    }
    cost_graph_data = new Graph<T>(); //new graph for Prim and kruskal 
    const int n = vertices.size(); 
    cost_graph_data->vertices = vertices; 
    cost_graph_data->edges.assign(n, vector<Edge>()); //edges for new graph 
    vector<vector<int>> best_cost(n, vector<int>(n, -1)); //best cost between verties 
    for(int u = 0; u < n; u++) { //traverse through vertices and edges to find best cost 
        for(int j = 0; j < edges[u].size(); j++) { 
            int v = edges[u][j].dest; 
            int a = min(u,v); 
            int b = max(u,v);
            int cost = edges[u][j].cost; //cost of edge 
            if(best_cost[a][b] == -1 || cost < best_cost[a][b]) {
                best_cost[a][b] = cost;
            }
        }
    }
    for (int u = 0; u < n; u++) {
        for(int v = u; v < n; v++) {
            if(best_cost[u][v] != -1) {//if edge between u and v then add to new graph weight 
                cost_graph_data->add_edge(cost_graph_data->vertices[u], cost_graph_data->vertices[v], best_cost[u][v]);
            }
        }
    }
}

/*
Description: Generate a Minimal Spanning Tree utilizing Prim’s algorithm on G_u that you created in the
previous step. The algorithm will output both the content of the constructed MST and its total cost.
In this step, for each edge you need to consider the cost as weight to minimize the total cost. In the
event of a disconnected graph, the algorithm will appropriately notify that an MST cannot be
formed. Note: A connected graph is defined as one where there exists a path between every pair of
vertices.
Parameter: nothing
Return: nothing
*/
template <typename T>
void Graph<T>::prim_mst() {
    if(cost_graph_data == nullptr) {
        cost_graph();
    }
    const int n = cost_graph_data->vertices.size();
    if(n == 0) return;

    vector<int> min_cost(n, INT_MAX);
    vector<int> parent(n, -1);
    vector<bool> in_mst(n, false);
    int total_cost = 0;
    int visited_count = 0;

    for (int i = 0; i < n; i++) {
        if (!in_mst[i]) {
            MinHeap<Edge> heap;
            min_cost[i] = 0;
            heap.insert(Edge(i, i, 0, 0));

            while (!heap.isEmpty()) {
                Edge current = heap.delete_min();
                int u = current.dest;
                if (in_mst[u]) continue;
                in_mst[u] = true;
                visited_count++;
                total_cost += current.weight;

                for (int j = 0; j < cost_graph_data->edges[u].size(); j++) {
                    Edge& edge = cost_graph_data->edges[u][j];
                    int v = edge.dest;
                    int weight = edge.weight;

                    if (!in_mst[v] && weight < min_cost[v]) {
                        min_cost[v] = weight;
                        parent[v] = u;
                        heap.insert(Edge(u, v, weight, 0));
                    }
                }
            }
        }
    }
    cout << "Prim MST:" << endl;
    for (int v = 0; v < n; v++) {
        if (parent[v] != -1 && parent[v] != v) {
            cout << cost_graph_data->vertices[parent[v]].getData()
                 <<" - "<< cost_graph_data->vertices[v].getData()
                 <<": "<< min_cost[v] << endl;
        }
    }
    cout <<"Total cost: "<< total_cost<<endl;
}
/*
Description:Generate a Minimal Spanning Tree using Kruskal’s algorithm on G_u that you created in the
previous step. The algorithm will output both the content of the constructed MST and its total cost.
In this step, for each edge you need to consider the cost as weight to minimize the total cost. If the
graph is disconnected the algorithm should provide minimum spanning forest consisting of a
minimum spanning tree for each connected component
Parameter: nothing 
Return: nothing 
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
Description: Helper function for Kruskal's algorithm to find the representative of the set that element i belongs to.
Parameter: parent - vector representing the parent of each element in the disjoint set, i - the element to find the set for
Return: The representative of the set that element i belongs to
*/
template<typename T>
int Graph<T>::find_set(vector<int>& parent, int i) {
    if(parent[i] != i) {
        parent[i] = find_set(parent, parent[i]);
    }
    return parent[i];
}
/*
Description: Helper function for Kruskal's algorithm to unite two sets.
Parameter: parent - vector representing the parent of each element in the disjoint set, i - the first element, j - the second element
Return: nothing
*/
template<typename T>
void Graph<T>::union_set(vector<int>& parent, int i, int j) {
    parent[find_set(parent, i)] = find_set(parent, j);
}