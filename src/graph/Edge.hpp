#pragma once

#include "Vertex.hpp"

// Corresponds to the Edge type for Graphic Matroids
class Edge {
    private:
        Vertex v;
        Vertex u;
        Weight weight;

    public:
        Edge();
        Edge(Vertex v1, Vertex v2, Weight w);

        std::string get_string();
        
        Vertex get_source();
        Vertex get_destination();
        Weight get_weight();
        Vertex get_other(Vertex v);
        void set_weight(Weight w);
        
        // Comparison operators
        bool operator<(Edge& e2);
        bool operator>(Edge& e2);
        bool operator==(Edge& e2);
        bool operator!=(Edge& e2);

        friend std::ostream& operator<<(std::ostream& os, Edge& e);
        friend std::ostream& operator<<(std::ostream& os, const Edge& e);
};