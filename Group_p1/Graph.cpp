#include "Graph.h"
#include "MinHeap.h"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

const int INF = 100000;

template <typename T>
void Graph<T>::insert_vertex(const Vertex<T>& ver) {
    if(get_vertex_index(ver) == -1) {
        vertices.push_back(ver); 
        vector<Edge> tmp; 
        edges.push_back(tmp); 
    }
}

template <typename T> 
int Graph<T>::get_vertex_index(const Vertex<T>& ver) {
    for(int i = 0; i < vertices.size(); i++) {
        if(vertices[i].data == ver.data) {
            return i; 
        }
    }
    return -1; 
}

template <typename T> 
void Graph<T>::add_edge(const Vertex<T>& ver1, const Vertex<T>& ver2, int distance, int cost) {
    int i1 = get_vertex_index(ver1);
    int i2 = get_vertex_index(ver2);
    if(i1 == -1 || i2 == -1) {
        throw string("Add edge: incorrect vertices\n"); 
    }
    Edge v(i1, i2, distance, cost);
    Edge v2(i2, i1, distance, cost);
    edges[i1].push_back(v);
    if(i1 != i2) {
        edges[i2].push_back(v2); 
    }
}

template <typename T> 
void Graph<T>::DFS(Vertex<T>& ver) {
    clean_visited();
    DFS_helper(ver);
    clean_visited();
}

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
    vertices[start_index].visited = true;

    while(head < q.size()) {
        int i = q[head++]; 
        cout << vertices[i].data << " "; 

        for(int j = 0; j < edges[i].size(); j++) {
            int adjacent_ver = edges[i][j].dest;
            if(vertices[adjacent_ver].visited == false) {
                vertices[adjacent_ver].visited = true; 
                q.push_back(adjacent_ver); 
            }
        }
    }
    clean_visited();
}

template <typename T> 
void Graph<T>::DFS_helper(Vertex<T>& ver) {
    int i = get_vertex_index(ver); 
    if(i == -1) 
        return;
    vertices[i].visited = true;
    cout << vertices[i].data << " ";
    for(int j = 0; j < edges[i].size(); j++) {
        int adjacent_ver = edges[i][j].dest;
        if(vertices[adjacent_ver].visited == false) {
            DFS_helper(vertices[adjacent_ver]); 
        }
    }
}

template <typename T>
void Graph<T>::clean_visited() {
    for(int i = 0; i < vertices.size(); i++) {
        vertices[i].visited = false;
    }
}

/*
1. Read the information from the dataset (a csv file) and create a weighted directed graph G.
*/
template <typename T> 
Graph<T>::Graph(const string& filename) {
    ifstream file(filename);
    string line; 

    getline(file, line); 

    while(getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string origin_airport, dest_airport, origin_city, dest_city, distance_str, cost_str;

        getline(ss, origin_airport, ',');
        getline(ss, dest_airport, ',');

        auto get_csv_field = [](stringstream& stream, string& field) {
            if (stream.peek() == '"') {
                stream.get();
                getline(stream, field, '"');
                if (stream.peek() == ',') stream.get();
            } else {
                getline(stream, field, ',');
            }
        };

        get_csv_field(ss, origin_city);
        get_csv_field(ss, dest_city);

        getline(ss, distance_str, ',');
        getline(ss, cost_str, ',');

        auto manual_clean_num = [](string& s) {
            string cleaned = "";
            for (int i = 0; i < s.length(); i++) {
                if (s[i] >= '0' && s[i] <= '9') cleaned += s[i];
            }
            s = cleaned;
        };

        manual_clean_num(distance_str);
        manual_clean_num(cost_str);

        if (distance_str.empty() || cost_str.empty()) continue;

        int distance = stoi(distance_str);
        int cost = stoi(cost_str);

        Vertex<T> v1(origin_airport);
        v1.city = origin_city;
        Vertex<T> v2(dest_airport);
        v2.city = dest_city;

        if(get_vertex_index(v1) == -1) insert_vertex(v1);
        if(get_vertex_index(v2) == -1) insert_vertex(v2);
        
        add_edge(v1, v2, distance, cost);
    }
    file.close(); 
}

/*
2. Find the shortest path between the given origin airport and destination airport.
   Uses distance as the weight. Outputs path, length, and cost.
*/
template<typename T>
int Graph<T>::dijkstra_shortest_path(const Vertex<T>& src, const Vertex<T>& dest) {
    int i_src = get_vertex_index(src);
    int i_dest = get_vertex_index(dest);
    if (i_src == -1 || i_dest == -1) {
        cout << "Shortest route from " << src.data << " to " << dest.data << ": None" << endl;
        return INF;
    }

    clean_visited();
    vector<int> distances(vertices.size(), INF);
    vector<int> costs(vertices.size(), INF);
    vector<int> parent(vertices.size(), -1);
    
    distances[i_src] = 0;
    costs[i_src] = 0;

    // MinHeap ordered by distance: store distance in the 'distance' field of Edge
    MinHeap<Edge> heap;
    heap.insert(Edge(i_src, i_src, 0, 0));

    while (!heap.isEmpty()) {
        Edge e = heap.delete_min();
        int u = e.dest;
        if (vertices[u].visited) continue;
        vertices[u].visited = true;
        if (u == i_dest) break;

        for (int j = 0; j < edges[u].size(); j++) {
            int v = edges[u][j].dest;
            int weight = edges[u][j].distance;
            int c = edges[u][j].cost;
            
            if (!vertices[v].visited && (distances[u] + weight < distances[v])) {
                distances[v] = distances[u] + weight;
                costs[v] = costs[u] + c;
                parent[v] = u;
                // Store cumulative distance in 'distance' field so heap sorts by it
                heap.insert(Edge(u, v, distances[v], 0));
            }
        }
    }
    clean_visited();

    if (distances[i_dest] == INF) {
        cout << "Shortest route from " << src.data << " to " << dest.data << ": None" << endl;
    } else {
        string path = "";
        for (int curr = i_dest; curr != -1; curr = parent[curr]) {
            path = vertices[curr].data + (path == "" ? "" : " -> " + path);
        }
        cout << "Shortest route from " << src.data << " to " << dest.data 
             << ": " << path 
             << ". The length is " << distances[i_dest] 
             << ". The cost is " << costs[i_dest] << "." << endl;
    }
    return distances[i_dest];
}

/*
3. Find all shortest paths from src to all airports in the given state.
   Outputs path, length, and cost in a table.
*/
template <typename T>
void Graph<T>::short_paths_state(const Vertex<T>& src, const string& state) {
    int n = vertices.size();

    vector<int> dist(n, INF);
    vector<int> cost(n, INF);
    vector<int> parent(n, -1);

    int src_index = get_vertex_index(src);
    if (src_index == -1) {
        cout << "Source airport not found." << endl;
        return;
    }
    dist[src_index] = 0;
    cost[src_index] = 0;

    // Heap ordered by distance
    MinHeap<Edge> heap;
    heap.insert(Edge(src_index, src_index, 0, 0));

    vector<bool> visited(n, false);

    while (!heap.isEmpty()) {
        Edge current = heap.delete_min();
        int u = current.dest;
        if (visited[u]) continue;
        visited[u] = true;

        for (int i = 0; i < edges[u].size(); i++) {
            int v = edges[u][i].dest;
            int weight = edges[u][i].distance;
            int c = edges[u][i].cost;

            if (!visited[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                cost[v] = cost[u] + c;
                parent[v] = u;
                heap.insert(Edge(u, v, dist[v], 0));
            }
        }
    }

    bool found = false;
    for (int i = 0; i < n; i++) {
        if (vertices[i].city.find(state) != string::npos && dist[i] != INF) {
            found = true;

            // Reconstruct path string
            vector<int> path;
            int temp = i;
            while (temp != -1) { path.push_back(temp); temp = parent[temp]; }

            string pathStr = "";
            for (int j = path.size() - 1; j >= 0; j--) {
                pathStr += vertices[path[j]].data;
                if (j != 0) pathStr += "->";
            }

            // Print row: pad path to 25 chars, then length, then cost
            cout << pathStr;
            int pad = 25 - (int)pathStr.size();
            if (pad < 1) pad = 1;
            cout << string(pad, ' ') << dist[i] << "       " << cost[i] << "\n";
        }
    }
    if (!found) cout << "No airports found in state: " << state << endl;
}

/*
4. Find the shortest path from src to dest with exactly (stops-1) intermediate airports.
   'stops' here = number of edges in the path.
*/
template <typename T>
void Graph<T>::short_path_stops(const Vertex<T>& src, const Vertex<T>& dest, int stops) {
    int n = vertices.size();
    int src_index = get_vertex_index(src);
    int dest_index = get_vertex_index(dest);

    if (src_index == -1 || dest_index == -1) {
        cout << "Shortest route from " << src.data << " to " << dest.data
             << " with " << stops - 1 << " stops: None" << endl;
        return;
    }

    // dist[v][s] = shortest distance reaching v in exactly s edges
    vector<vector<int>> dist(n, vector<int>(stops + 1, INF));
    // parent[v][s] = which vertex came before v at step s
    vector<vector<int>> parent(n, vector<int>(stops + 1, -1));

    dist[src_index][0] = 0;

    for (int s = 0; s < stops; s++) {
        for (int u = 0; u < n; u++) {
            if (dist[u][s] == INF) continue;
            for (int j = 0; j < edges[u].size(); j++) {
                int v = edges[u][j].dest;
                int w = edges[u][j].distance;

                // Prevent revisiting src to avoid cycles back to start
                if (v == src_index) continue;

                if (dist[u][s] + w < dist[v][s + 1]) {
                    dist[v][s + 1] = dist[u][s] + w;
                    parent[v][s + 1] = u;
                }
            }
        }
    }

    if (dist[dest_index][stops] == INF) {
        cout << "Shortest route from " << src.data << " to " << dest.data
             << " with " << stops - 1 << " stops: None" << endl;
    } else {
        vector<int> path;
        int cur = dest_index;
        for (int s = stops; s >= 0; s--) {
            path.push_back(cur);
            if (s > 0) cur = parent[cur][s];
        }
        cout << "Shortest route from " << src.data << " to " << dest.data
             << " with " << stops - 1 << " stops: ";
        for (int i = path.size() - 1; i >= 0; i--) {
            cout << vertices[path[i]].data;
            if (i != 0) cout << " -> ";
        }
        cout << ". The length is " << dist[dest_index][stops] << "." << endl;
    }
}

/*
5. Count and display total direct flight connections (inbound + outbound) per airport,
   sorted descending. 'limit' controls how many rows to print (-1 = all).
*/
template <typename T>
void Graph<T>::disp_connections_sort(int limit) {
    int n = vertices.size();
    vector<int> total(n, 0);

    // Outbound count
    for (int i = 0; i < n; i++)
        total[i] += edges[i].size();

    // Inbound count
    for (int i = 0; i < n; i++)
        for (int j = 0; j < edges[i].size(); j++)
            total[edges[i][j].dest]++;

    // Sort an index array (don't mutate the graph)
    vector<int> idx(n);
    for (int i = 0; i < n; i++) idx[i] = i;

    for (int i = 0; i < n - 1; i++) {
        int max_pos = i;
        for (int j = i + 1; j < n; j++)
            if (total[idx[j]] > total[idx[max_pos]]) max_pos = j;
        int tmp = idx[i]; idx[i] = idx[max_pos]; idx[max_pos] = tmp;
    }

    int print_count = (limit == -1) ? n : limit;
    for (int i = 0; i < print_count && i < n; i++) {
        cout << vertices[idx[i]].data << "    " << total[idx[i]] << "\n";
    }
}

/*
6. Build undirected cost graph G_u from directed graph G.
*/
template <typename T> 
Graph<T> Graph<T>::cost_graph() {
    Graph<T> G_u;

    for (int i = 0; i < vertices.size(); i++) {
        G_u.insert_vertex(vertices[i]);
    }

    for(int i = 0; i < edges.size(); i++) {
        for(int j = 0; j < edges[i].size(); j++) {
            int src = edges[i][j].src;
            int dest = edges[i][j].dest;
            int cost = edges[i][j].cost; 

            if(src < dest) {
                bool reverse_edge_exists = false; 
                int reverse_cost = INF;

                for(int k = 0; k < edges[dest].size(); k++) {
                    if(edges[dest][k].dest == src) {
                        reverse_edge_exists = true; 
                        if (edges[dest][k].cost < reverse_cost)
                            reverse_cost = edges[dest][k].cost;
                    }
                }
                int min_cost = reverse_edge_exists ? (cost < reverse_cost ? cost : reverse_cost) : cost;
                G_u.add_edge(vertices[src], vertices[dest], min_cost, min_cost);
            }
        }
    }

    return G_u;
}

/*
7. Prim's MST on G_u. Uses cost as weight.
   'limit' controls how many edges to print (-1 = all).
*/
template <typename T>
void Graph<T>::prim_mst(int limit) {
    int n = vertices.size();

    vector<bool> visited(n, false);

    // Heap must order by COST for Prim's — insert Edge with cost in 'distance' field
    MinHeap<Edge> heap;

    int start = 0;
    visited[start] = true;

    for (int i = 0; i < edges[start].size(); i++) {
        Edge e = edges[start][i];
        // Put cost into distance field so MinHeap (ordered by distance) picks cheapest
        heap.insert(Edge(e.src, e.dest, e.cost, e.cost));
    }

    int total_cost = 0;
    int edge_count = 0;
    int printed = 0;

    while (!heap.isEmpty() && edge_count < n - 1) {
        Edge e = heap.delete_min();
        int v = e.dest;

        if (visited[v]) continue;
        visited[v] = true;

        if (limit == -1 || printed < limit) {
            cout << vertices[e.src].data << " - "
                 << vertices[e.dest].data
                 << "          " << e.cost << "\n";
            printed++;
        }

        total_cost += e.cost;
        edge_count++;

        for (int i = 0; i < edges[v].size(); i++) {
            if (!visited[edges[v][i].dest]) {
                Edge next = edges[v][i];
                heap.insert(Edge(next.src, next.dest, next.cost, next.cost));
            }
        }
    }

    if (edge_count != n - 1) {
        cout << "Graph is disconnected. MST cannot be formed.\n";
    } else {
        cout << "Total Cost of MST: " << total_cost << "\n";
    }
}

/*
8. Kruskal's MST on G_u. Handles disconnected graphs with spanning forest.
*/
template <typename T>
int Graph<T>::find_set(vector<int>& parent, int i) {
    if (parent[i] != i) {
        parent[i] = find_set(parent, parent[i]);
    }
    return parent[i];
}

template <typename T>
void Graph<T>::union_set(vector<int>& parent, int i, int j) {
    int root_i = find_set(parent, i);
    int root_j = find_set(parent, j);
    parent[root_i] = root_j;
}

template <typename T>
void Graph<T>::kruskal_mst() {
    int n = vertices.size();

    vector<Edge> all_edges;

    // Collect unique edges (src < dest to avoid duplicates in undirected graph)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < edges[i].size(); j++) {
            if (edges[i][j].src < edges[i][j].dest) {
                all_edges.push_back(edges[i][j]);
            }
        }
    }

    // Bubble sort by cost
    for (int i = 0; i < (int)all_edges.size() - 1; i++) {
        for (int j = 0; j < (int)all_edges.size() - i - 1; j++) {
            if (all_edges[j + 1].cost < all_edges[j].cost) {
                Edge temp = all_edges[j];
                all_edges[j] = all_edges[j + 1];
                all_edges[j + 1] = temp;
            }
        }
    }

    vector<int> parent(n);
    for (int i = 0; i < n; i++) parent[i] = i;

    int total_cost = 0;

    for (int i = 0; i < (int)all_edges.size(); i++) {
        int u = all_edges[i].src;
        int v = all_edges[i].dest;

        if (find_set(parent, u) != find_set(parent, v)) {
            union_set(parent, u, v);
            cout << vertices[u].data << " - "
                 << vertices[v].data
                 << "          " << all_edges[i].cost << "\n";
            total_cost += all_edges[i].cost;
        }
    }

    cout << "Total Cost of MST: " << total_cost << "\n";
}