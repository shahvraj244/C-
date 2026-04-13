#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <string>
#include "Vertex.h"
#include "Edge.h"
using namespace std;

template <typename T>
class Graph {
    public:
        Graph() {}
        Graph(const string& filename);

        void insert_vertex(const Vertex<T>& ver); 
        void add_edge(const Vertex<T>& ver1, const Vertex<T>& ver2, int distance, int cost);

        int dijkstra_shortest_path(const Vertex<T>& src, const Vertex<T>& dest);
        void short_paths_state(const Vertex<T>& src, const string& state);
        void short_path_stops(const Vertex<T>& src, const Vertex<T>& dest, int stops);
        void disp_connections_sort(int limit = -1); 
        Graph<T> cost_graph();      // returns undirected cost graph
        void prim_mst(int limit = -1); 
        void kruskal_mst(); 
        void print() const;         // const matches .cpp
        void BFS(Vertex<T>& ver); 
        void DFS(Vertex<T>& ver); 
    
    private:
        vector<Vertex<T>> vertices;
        vector<vector<Edge>> edges;

        void clean_visited();
        void DFS_helper(Vertex<T>& ver);
        int get_vertex_index(const Vertex<T>& ver);
        int find_set(vector<int>& parent, int i); 
        void union_set(vector<int>& parent, int i, int j); 
};

#endif