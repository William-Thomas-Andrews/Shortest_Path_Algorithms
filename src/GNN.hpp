#pragma once

#include "Graph.cpp"
#include "Matrix.cpp"

class GNN {
    private:
        Graph G;
        Matrix input_layer; // Move to using  my matrix class with arrays
        Matrix perceptron_layer; // ~
        Matrix output_layer; // ~
        Matrix X, Y; // Input Layer X, and Cost Layer Y
        double alpha;
    
    public:

        GNN();
        GNN(Graph G, double learning_rate);

        // add some intermediate functions and possibly classes; don't just do a huge function
        Matrix sigmoid(Matrix& M);
        Matrix sigmoid_prime(Matrix& M);
        Matrix softmax(Matrix& M);
        void forward_prop();
        void back_prop();
        void update_params();
        void train();
        std::vector<Edge> predict(Vertex start, Vertex end);

};