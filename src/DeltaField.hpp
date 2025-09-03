#pragma once

#include "Graph.hpp"

class DeltaField {
    private:
        Graph G;
    public:
        DeltaField(Graph Graph_Input);
        void specified_dijkstra_activation(Vertex start, Vertex end);
        void specified_dijkstra_activation(int start_val, int end_val);
        void specified_standard_activation(Vertex start, Vertex end);
        void specified_standard_activation(int start_val, int end_val);
        void specified_multithreaded_activation(Vertex start_val, Vertex end_val);
        void specified_multithreaded_activation(int start_val, int end_val);
        void random_dijkstra_activation();
        void random_standard_activation();
        void random_multithreaded_activation();
};