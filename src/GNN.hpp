#pragma once

#include "Graph.cpp"

class GNN {
    private:
        Graph G;
        int* input_layer; // Move to using  my matrix class with arrays
        int* perceptron_layer; // ~
        int* output_layer; // ~
    
    public:

        GNN();
        GNN(Graph G);

        // add some intermediate functions and possibly classes; don't just do a huge function

        void train();
        std::vector<Edge> predict(Vertex start, Vertex end);

};