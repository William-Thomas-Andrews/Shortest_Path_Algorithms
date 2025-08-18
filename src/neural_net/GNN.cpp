#include "GNN.hpp"

GNN::GNN() {}

GNN::GNN(Graph G, double learning_rate) : alpha(learning_rate) {

}


double sig_op(double op) {
    return 1 / (1 + exp(-op));
}

Matrix GNN::sigmoid(Matrix& M) {
    Matrix return_matrix = Matrix(M.get_rows(), M.get_cols());
    for (int i = 0; i < M.get_rows(); i++) {
        for (int j = 0; j < M.get_cols(); j++) {
            return_matrix(i, j) = sig_op(M(i, j));
        }
    }
    return return_matrix;
}

Matrix GNN::sigmoid_prime(Matrix& M) {
    Matrix return_matrix = Matrix(M.get_rows(), M.get_cols());
    for (int i = 0; i < M.get_rows(); i++) {
        for (int j = 0; j < M.get_cols(); j++) {
            return_matrix(i, j) = sig_op(M(i, j)) * (1 - sig_op(M(i, j)));
        }
    }
    return return_matrix;
}

Matrix GNN::softmax(Matrix& M) {
    Matrix return_matrix = Matrix(M.get_rows(), M.get_cols());
    for (int i = 0; i < return_matrix.get_rows(); i++) {
        for (int j = 0; j < return_matrix.get_cols(); j++) {
            return_matrix(i, j) = exp(return_matrix(i, j));
        }
    }
    double sum = return_matrix.sum_elements();
    return_matrix = return_matrix / sum;
    return return_matrix;
}

void GNN::forward_prop() {
    // Matrix A_0 = X;
    // Matrix Z_1 = dot(W_1, A_0) + B_1;
    // Matrix A_1 = sigmoid(Z_1);
    // Matrix Z_2 = dot(W_2, A_1) + B_2;
    // Matrix A_2 = softmax(Z_2);
}

void GNN::back_prop() {
    // // Step 1
    // Matrix dz_2 = A_2 - Y;
    // Matrix dw_2 = (1/m) * dot(dz_2, A_2.Transpose);
    // Matrix db_2 = (1/m) * my_sum(dz_2);
    
    // // Step 2
    // Matrix dz_1 = dot(dot(W_2.Transpose, dz_2), sigmoid_prime(Z_1));
    // Matrix dw_1 = (1/m) * dot(dz_1, X.Transpose);
    // Matrix db_1 = (1/m) * my_sum(dz_1);
}

void GNN::update_params() {
    // W_1 = W_1 - alpha * dw_1;
    // B_1 = B_1 - alpha * db_1;
    // W_2 = W_2 - alpha * dw_2;
    // B_2 = B_2 - alpha * db_2;
}

void GNN::train() { // Try with single instance input for each iteration

    // Loop begins, for n iterations:

        // Input vertices from Graph G into the input_vertices matrix (perhaps just two input vertices, the beginning and the end?)

        // Weight and bias matrix multiplication from each perceptron and activation function, then feeds to the ouptut stage.

        // Calculate the Dijkstra actual path and put in a cost matrix by entering in the weights

        // Calculate result with cost function matrix by subtracting from the weight matrix chosen by the model

        // Perform the rest of back prop according with the sheet I wrote

        // Update the parameters

        // Repeat loop ~

    // Return;

    std::vector<double> vec({0, 1, 2, 3, 4, 5});
    std::vector<double> vec0({0, 10, 20, 30, 40, 50});
    std::vector<double> vec2({100, 80, 60, 40, 20, 0});

    Matrix A(vec, 2, 3);

    Matrix A0(vec0, 2, 3);

    Matrix B(vec2, 3, 2);

    Matrix C = A + A0;

    std::cout << C  << std::endl;
}

// Possibly change return type later
std::vector<Edge> GNN::predict(Vertex start, Vertex end) { 

    // Perform forward prop from above

    // Return prediction vector (or other way of showing the path taken);
    return std::vector<Edge>();
}