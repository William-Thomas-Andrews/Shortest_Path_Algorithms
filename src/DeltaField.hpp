#pragma once

#include "GNN.cpp"

class DeltaField {
    private:
        Graph G;
        GNN Neural_Network;

    public:
        DeltaField(Graph Graph_Input);

        int activate_A_Star();

        int activate_neural_network();
};