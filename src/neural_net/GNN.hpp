#pragma once

#include "Graph.cpp"
#include "Matrix.cpp"

class GNN {
    private:
        Graph G;
        Matrix W_1, W_2, B_1, B_2, X, Y, A_0, A_1, A_2, Z_1, Z_2, dz_2, dz_1, dw_2, dw_1, db_2, db_1; // Input Layer X, and Cost Layer Y
        double alpha;
        int m = 1;
    
    public:

        GNN();
        GNN(Graph G, double learning_rate);
        void operator=(const GNN& other);
        Matrix sigmoid(Matrix& M);
        Matrix sigmoid_prime(Matrix& M);
        Matrix softmax(Matrix& M);
        Matrix col_summation(Matrix& M);
        void forward_propagation();
        void back_propagation();
        void update_params();
        void train();
        std::vector<Edge> predict(Vertex start, Vertex end);

};