#ifndef EDGE_H
#define EDGE_H

class Edge {
public:
    Edge(int src = 0, int dest = 0, int weight = 0, int cost = 0) : 
        src(src), dest(dest), weight(weight), cost(cost) {}

    bool operator<(const Edge& other) const {//compares edges based on weight 
        if(this->weight < other.weight) {
            return true;
        }
        return false;
    }
  
    int src; //source vertex index
    int dest; //destination vertex index
    int weight;//weight of the edge, represents cost 
    int cost; //cost of the edge, represents distance
};

#endif