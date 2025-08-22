#pragma once

#include "Graph.cpp"
// #include "Matrix.cpp"

class GNN {
    private:
        Graph G;
        Matrix W_1, W_2, B_1, B_2, X, Y, A_0, A_1, A_2, Z_1, Z_2, dz_2, dz_1, dw_2, dw_1, db_2, db_1; 
        double alpha;
        int m = 1;
        double xavier_upper_bound, xavier_lower_bound;
    
    public:

        GNN();
        GNN(Graph G, double learning_rate);
        
        Matrix sigmoid(Matrix& M);
        Matrix sigmoid_prime(Matrix& M);
        Matrix softmax(Matrix& M);
        Matrix ReLU(Matrix& M);
        Matrix square(Matrix& M);
        Matrix log(Matrix& M);
        Matrix col_summation(Matrix& M);

        void mask_incorrect_entries(Matrix& M);
        void xavier_initialization();
        void forward_propagation(Vertex start, Vertex end);
        void training_propagation();
        void backward_propagation(Vertex start, Vertex end);
        void update_params();
        void update_Y(Vertex start, Vertex end);
        void check_valid_edges();
        void check_cycles(Vertex start, Vertex end);

        void train(int iterations);
        std::vector<Edge> predict(Vertex start, Vertex end);

        void print_forward_prop();
        void print_backward_prop();
        void print_params();

};