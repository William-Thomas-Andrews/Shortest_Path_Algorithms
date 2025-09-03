#pragma once

#include "Graph.hpp"

class DeltaField {
    private:
        Graph G;
    public:
        DeltaField(Graph Graph_Input);
        long long specified_dijkstra_activation(Vertex start, Vertex end);
        long long specified_dijkstra_activation(int start_val, int end_val);
        long long specified_standard_activation(Vertex start, Vertex end);
        long long specified_standard_activation(int start_val, int end_val);
        long long specified_multithreaded_activation(Vertex start_val, Vertex end_val);
        long long specified_multithreaded_activation(int start_val, int end_val);
        long long random_dijkstra_activation();
        long long random_standard_activation();
        long long random_multithreaded_activation();
};