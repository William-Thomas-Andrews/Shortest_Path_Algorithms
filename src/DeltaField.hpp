#pragma once

// #include "DeltaField.hpp"
#include "Graph.cpp"

class DeltaField {
    private:
        Graph G;

    public:
        DeltaField(Graph Graph_Input);

        int activate_A_Star();

        int activate_neural_network();
};