#include "GNN.hpp"

GNN::GNN() {}

GNN::GNN(Graph input_graph, double learning_rate) : G(input_graph), alpha(learning_rate) {

    // Generate weights
    W_1 = Matrix(10, 230);
    W_2 = Matrix(10, 10);

    // Generate biases
    B_1 = Matrix(10, 1);
    B_2 = Matrix(10, 1);

    // Generate input X (for now only one training input instance)
    // perhaps only two vertices, start and finish? start with all for now
    // std::cout << "Vertices size " << G.get_vertices().size() << std::endl;
    X = Matrix(G.get_vertices(), G.get_vertices().size(), 1);
    // std::cout << "X is:\n" << X << std::endl;

    // Generate output Y
    Y = Matrix(G.get_vertices(), G.get_vertices().size(), 1); // just keep at the same thing for now

    A_0 = Matrix(10, 10);
    A_1 = Matrix(10, 10);
    A_2 = Matrix(10, 10);
    Z_1 = Matrix(10, 10);
    Z_2 = Matrix(10, 10);
    dz_2 = Matrix(10, 10);
    dz_1 = Matrix(10, 10);
    dw_2 = Matrix(10, 10);
    dw_1 = Matrix(10, 10);
    db_2 = Matrix(10, 10);
    db_1 = Matrix(10, 10);
    alpha = 0.01;

}

void GNN::operator=(const GNN& other) { // Copy assignment
    G = other.G;
    W_1 = other.W_1;
    W_2 = other.W_2;
    B_1 = other.B_1;
    B_2 = other.B_2;
    X = other.X;
    Y = other.Y;
    // A_0 = other.A_0;
    // A_1 = other.A_1;
    // A_2 = other.A_2;
    // Z_1 = other.Z_1;
    // Z_2 = other.Z_2;
    // dz_2 = other.dz_2;
    // dz_1 = other.dz_1;
    // dw_2 = other.dw_2;
    // dw_1 = other.dw_1;
    // db_2 = other.db_2;
    // db_1 = other.db_1;
    // alpha = other.alpha;
    // m = other.m;
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

Matrix GNN::col_summation(Matrix& M) {
    double sum = 0;
    Matrix return_matrix = Matrix(M.get_rows(), 1);
    for (int i = 0; i < M.get_rows(); i++) {
        for (int j = 0; j < M.get_cols(); j++) {
            sum += M(i, j);
        }
        return_matrix(i, 0) += sum;
        sum = 0;
    }
    return return_matrix;
}

void GNN::forward_propagation() {
    A_0 = X;
    Z_1 = dot(W_1, A_0) + B_1;
    A_1 = sigmoid(Z_1);
    Z_2 = dot(W_2, A_1) + B_2;
    A_2 = softmax(Z_2);
}

void GNN::back_propagation() {
    // Step 1
    dz_2 = A_2 - Y;
    dw_2 = dot(dz_2, A_2.Transpose()) * (1/m);
    db_2 = col_summation(dz_2) * (1/m); // no sum for average because we are only doing one training instance so far
    
    // Step 2
    dz_1 = dot(dot(W_2.Transpose(), dz_2), sigmoid_prime(Z_1));
    dw_1 =  dot(dz_1, X.Transpose()) * ((double) (1/m));
    db_1 = col_summation(dz_1) * ((double)(1/m)); // no sum for average because we are only doing one training instance so far
}

void GNN::update_params() {
    W_1 = W_1 - alpha * dw_1;
    B_1 = B_1 - alpha * db_1;
    W_2 = W_2 - alpha * dw_2;
    B_2 = B_2 - alpha * db_2;
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

    // Matrix A0(vec0, 2, 3);

    Matrix B(vec2, 3, 2);

    std::cout << B << std::endl;

    // Matrix C = dot(A, B);

    // std::cout << C  << std::endl;

    // Matrix D = 1 / C ;
    
    // std::cout << D << std::endl;


    // forward_propagation();
}

// Possibly change return type later
std::vector<Edge> GNN::predict(Vertex start, Vertex end) { 

    // Perform forward prop from above

    // Return prediction vector (or other way of showing the path taken);
    return std::vector<Edge>();
}