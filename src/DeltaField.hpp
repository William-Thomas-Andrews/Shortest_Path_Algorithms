#pragma once

#include "Graph.hpp"

class DeltaField {
    private:
        Graph G;

    public:
        DeltaField(Graph Graph_Input);

        void activate_A_Star();

        void specified_standard_activation(Vertex start, Vertex end);
        void specified_standard_activation(int start_val, int end_val);
        void specified_multithreaded_activation(Vertex start_val, Vertex end_val);
        void specified_multithreaded_activation(int start_val, int end_val);
        void random_standard_activation();
        void random_multithreaded_activation();

        // int activate_neural_network();
};