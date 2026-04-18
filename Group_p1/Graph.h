#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <string>
#include "Vertex.h"
#include "Edge.h"
#include "MinHeap.h"
using namespace std;

template <typename T>
class Graph {
    public:
        Graph() : cost_graph_data(nullptr) {}
        //insert vertex and edge with weights of distance and cost 
        void insert_vertex(const Vertex<T>& ver); 
        void add_edge(const Vertex<T>& ver1, const Vertex<T>& ver2, int weight);
        //Dijkstra Shortest Path returns weight
        int dijkstra_shortest_path(const Vertex<T>& src, const Vertex<T>& dest);
        //shortest path to airports in some state that hops to diff airports 
        void short_paths_state(const Vertex<T>& src, const string& state);
        //short path with stops 
        void short_path_stops(const Vertex<T>& src, const Vertex<T>& dest, int stops);
        //Disp connections 
        void disp_connections_sort(); 
        //build undirected graph weighted by cost
        void cost_graph();
        //prim's minimum span tree
        void prim_mst(); 
        //kruskal's minimum span tree
        void kruskal_mst(); 
        //Breadth first search
        void BFS(Vertex<T>& ver); 
        //Depth first search 
        void DFS(Vertex<T>& ver); 
        //read from csv file 
        Graph(const string& filename);
    private:
        vector<Vertex<T>> vertices;
        vector<vector<Edge>> edges;
        void clean_visited();
        void DFS_helper(Vertex<T>& ver);
        int get_vertex_index(const Vertex<T>& ver);
        vector<string> vertex_cities;
        //Undirected Graph G_u 
        Graph<T>* cost_graph_data; 
        //helper functions for kruskal 
        int find_set(vector<int>& parent, int i); 
        void union_set(vector<int>& parent, int i, int j); 
};

#endif
