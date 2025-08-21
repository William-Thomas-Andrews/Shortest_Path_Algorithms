#include "GNN.hpp"
#include "Utils.cpp"

GNN::GNN() {}


GNN::GNN(Graph input_graph, double learning_rate) : G(input_graph), alpha(learning_rate) {

    int num_inputs = 2; // 2
    int num_outputs = 50; // 50
    int num_perceptrons = 4; // 4
    int batch_size = m; // 1

    // Generate input X (for now only one training input instance)
    X = Matrix(std::vector<double>{0, 9}, num_inputs, batch_size);

    // Generate output Y
    Y = Matrix(num_outputs, batch_size);

    xavier_initialization();

    // Generate weights
    W_1 = Matrix(num_perceptrons, num_inputs, xavier_lower_bound, xavier_upper_bound);
    W_2 = Matrix(num_outputs, num_perceptrons, xavier_lower_bound, xavier_upper_bound);

    // Generate biases
    B_1 = Matrix(num_perceptrons, batch_size);
    B_2 = Matrix(num_outputs, batch_size);

    // Generate main matrices
    A_0 = X;
    A_1 = Matrix(num_perceptrons, batch_size);
    A_2 = Matrix(num_outputs, batch_size);

    // Generate weighted and biased sum matrices
    Z_1 = Matrix(num_perceptrons, batch_size);
    Z_2 = Matrix(num_outputs, batch_size);

    // Generate partial derivative matrices
    dz_2 = Matrix(num_outputs, batch_size);
    dz_1 = Matrix(num_perceptrons, batch_size);
    dw_2 = Matrix(num_outputs, num_perceptrons);
    dw_1 = Matrix(num_perceptrons, num_inputs);
    db_2 = Matrix(num_perceptrons, batch_size);
    db_1 = Matrix(num_perceptrons, batch_size);
}

void GNN::xavier_initialization() {
    xavier_upper_bound = std::sqrt( (6.0) / (X.get_size() + Y.get_size()) );
    xavier_lower_bound = (-xavier_upper_bound);
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
            return_matrix(i, j) = exp(M(i, j));
        }
    }
    double sum = return_matrix.sum_elements();
    return_matrix = return_matrix / sum;
    return return_matrix;
}

Matrix GNN::square(Matrix& M) {
    Matrix return_matrix = Matrix(M.get_rows(), M.get_cols());
    for (int i = 0; i < M.get_rows(); i++) {
        for (int j = 0; j < M.get_cols(); j++) {
            return_matrix(i, j) = M(i,j) * M(i, j);
        }
    }
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

void GNN::forward_propagation(Vertex start, Vertex end) {
    // Setting the two inputs for X
    X(0,0) = start.val;
    X(1,0) = end.val;
    A_0 = X;
    Z_1 = dot(W_1, A_0) + B_1;
    A_1 = sigmoid(Z_1);
    Z_2 = dot(W_2, A_1) + B_2;
    // A_2 = softmax(Z_2);
    A_2 = Z_2;
    print_forward_prop();
}

void GNN::training_forward_propagation() {
    std::tuple<Vertex, Vertex> vertex_pair = G.get_random_vertex_pair();
    Vertex start = std::get<0>(vertex_pair);
    Vertex end = std::get<1>(vertex_pair);
    forward_propagation(start, end);
    update_Y(start, end);
}

void GNN::backward_propagation() {
    // Step 1
    dz_2 = A_2 - Y;
    dw_2 = dot(dz_2, A_1.Transpose()) * (1/m);
    db_2 = col_summation(dz_2) * (1/m);
    
    // Step 2
    dz_1 = dot(W_2.Transpose(), dz_2) * sigmoid_prime(Z_1);
    dw_1 =  dot(dz_1, X.Transpose()) * ((double) (1/m));
    db_1 = col_summation(dz_1) * ((double)(1/m));

    print_backward_prop();
}

void GNN::update_params() {
    print_params();
    W_1 = W_1 - alpha * dw_1;
    B_1 = B_1 - alpha * db_1;
    W_2 = W_2 - alpha * dw_2;
    B_2 = B_2 - alpha * db_2;
    print_params();
}

void GNN::update_Y(Vertex start, Vertex end) {
    Y.clear();
    solution.clear();
    std::tuple<std::vector<double>, std::vector<int>> tup = G.A_Star(start, end);
    std::vector<double> dist = std::get<0>(tup);
    std::vector<int> prev = std::get<1>(tup);
    std::vector<int> path = get_and_print_path(prev, start.val, end.val);
    for (int at = end.val; at != start.val && at != -1; at = prev[at]) { // change into a function soon
        for (Edge e : G.get_adj()[at]) {
            if (e.get_left().val == prev[at]) {
                solution.push_back(e);
            }
        }
    }
    std::reverse(solution.begin(), solution.end());
    for (int i = 0; i < solution.size(); i++) { // updating Y
        Y(i, 0) = solution[i].get_weight();
    }
}

void GNN::train(int iterations) { 
    for (int i = 0; i < iterations; i++) {
        training_forward_propagation();
        backward_propagation();
        update_params();
    }
}

std::vector<Edge> GNN::predict(Vertex start, Vertex end) { 
    forward_propagation(start, end);
    std::cout << "Prediction, A_2: \n" << A_2 << std::endl;
    return std::vector<Edge>();  // Return prediction vector
}

void GNN::print_forward_prop(){
    std::cout << "Forward prop:\n" << A_0 << std::endl;
    std::cout << W_1 << std::endl;
    std::cout << "Z_1:\n" << Z_1 << std::endl;
    std::cout << "A_1:\n" << A_1 << std::endl;
    std::cout << "Z_2:\n" << Z_2 << std::endl;
    std::cout << "A_2:\n" << A_2 << std::endl << std::endl;
}

void GNN::print_backward_prop() {
    std::cout << "Backward prop:\n" << A_0 << std::endl;
    std::cout << "Y:\n" << Y << std::endl;
    std::cout << "dz_2:\n" << dz_2 << std::endl;
    std::cout << "dw_2:\n" << dw_2 << std::endl;
    std::cout << "db_2:\n" << db_2 << std::endl;
    std::cout << "dz_1:\n" << dz_1 << std::endl;
    std::cout << "dw_1:\n" << dw_1 << std::endl;
    std::cout << "db_2:\n" << db_2 << std::endl<< std::endl;
}

void GNN::print_params() {
    std::cout << "Update params.\nW_1:\n" << W_1 << std::endl;
    std::cout << "B_2:\n" << B_1 << std::endl;
    std::cout << "W_2:\n" << W_2 << std::endl;
    std::cout << "B_2:\n" << B_2 << std::endl << std::endl;
}