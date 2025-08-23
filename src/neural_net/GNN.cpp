#include "GNN.hpp"
#include "Utils.cpp"


GNN::GNN() {}

GNN::GNN(Graph input_graph, double learning_rate) : G(input_graph), alpha(learning_rate) {

    int num_inputs = 2025; // 1
    int num_outputs = G.get_adj().size() * G.get_adj().size(); // 2025
    int num_perceptrons = 10; // 10
    // int batch_size = m; // 1
    int output_size = 1; // 1
    std::cout << "hegiirge" << std::endl;

    // Generate input X (for now only one training input instance)
    X = Matrix(num_inputs, 1);
    // Change X to being dimensions: features x num_outputs=45, then the other matrix logic will work and the output will be 45x45

    // Generate output Y
    Y = Matrix(num_outputs, output_size);

    // xavier_initialization();

    // Generate weights
    xavier_upper_bound = sqrt(6.0 / W_1.get_cols() + W_1.get_rows());
    xavier_lower_bound = -xavier_upper_bound;
    W_1 = Matrix(num_perceptrons, num_inputs, xavier_lower_bound, xavier_upper_bound);
    xavier_upper_bound = sqrt(6.0 / W_2.get_cols() + W_2.get_rows());
    xavier_lower_bound = -xavier_upper_bound;
    W_2 = Matrix(num_outputs, num_perceptrons, xavier_lower_bound, xavier_upper_bound);
    
    // Generate biases
    B_1 = Matrix(num_perceptrons, 1);
    B_2 = Matrix(num_outputs, 1);

    // Generate main matrices
    A_0 = X;
    A_1 = Matrix(num_perceptrons, 1);
    A_2 = Matrix(num_outputs, 1);

    // Generate weighted and biased sum matrices
    Z_1 = Matrix(num_perceptrons, 1);
    Z_2 = Matrix(num_outputs, 1);
    
    // Generate partial derivative matrices
    dz_2 = Matrix(num_outputs, 1);
    dz_1 = Matrix(num_perceptrons, 1);
    dw_2 = Matrix(num_outputs, num_perceptrons);
    dw_1 = Matrix(num_perceptrons, num_inputs);
    db_2 = Matrix(num_perceptrons, 1);
    db_1 = Matrix(num_perceptrons, 1);
}

void GNN::xavier_initialization() {
    xavier_upper_bound = std::sqrt( (6.0) / (W_1.get_rows() + Y.get_size()) );
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
        double max = -100000;
        for (int p = 0; p < return_matrix.get_cols(); p++) {
            if (M(i, p) > max) { max = M(i, p); }
        }
        for (int j = 0; j < return_matrix.get_cols(); j++) {
            return_matrix(i, j) = exp(M(i, j) - max);
        }
        double sum = return_matrix.sum_row(i);
        for (int k = 0; k < return_matrix.get_cols(); k++) {
            return_matrix(i, k) /= sum;
        }
    }
    return return_matrix;
}

Matrix GNN::ReLU(Matrix& M) {
    Matrix return_matrix = Matrix(M.get_rows(), M.get_cols());
    for (int i = 0; i < return_matrix.get_rows(); i++) {
        for (int j = 0; j < return_matrix.get_cols(); j++) {
            if (M(i, j) > 0) {
                return_matrix(i, j) = M(i, j);
            }
            else {
                return_matrix(i, j) = 0;
            }
        }
    }
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

Matrix GNN::log(Matrix& M) {
    Matrix return_matrix = Matrix(M.get_rows(), M.get_cols());
    for (int i = 0; i < M.get_rows(); i++) {
        for (int j = 0; j < M.get_cols(); j++) {
            return_matrix(i, j) = std::log(M(i,j));
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

void GNN::mask_incorrect_entries(Matrix& M) {
    int n = G.get_adj().size();
    for (int i = 0; i < M.get_rows(); i++) {
        int row = i / n;
        int col = i % n;
        if (M(i,0) > 0 && G.get_adjacency_matrix()(row, col) <= 0) {
            M(i, 0) = -100000;
        }
    }
}

double dist(Vertex v1, Vertex v2) {
    return std::sqrt(std::pow(v2.x - v1.x, 2) + std::pow(v2.y - v1.y, 2));
}

void GNN::forward_propagation(Vertex start, Vertex end) {
    // Setting the two inputs for X
    X(start.val*45 + end.val, 0) = dist(start, end);//1.0;

    // X(end.val, 0) = 1.0; 
    A_0 = X; //
    
    Z_1 = dot(W_1, A_0) + B_1;//dot(B_1, Matrix(1, 1, A_0.get_cols()));
    A_1 = sigmoid(Z_1);
    Z_2 = dot(W_2, A_1) + B_2;//dot(B_2, Matrix(1, 1, A_1.get_cols()));
    mask_incorrect_entries(Z_2);
    A_2 = sigmoid(Z_2); 
    print_forward_prop();
}

void GNN::training_propagation() {
    // std::cout << "heyy!!" << std::endl;
    std::tuple<Vertex, Vertex> vertex_pair = G.get_random_vertex_pair();
    Vertex start = std::get<0>(vertex_pair);
    Vertex end = std::get<1>(vertex_pair);
    // Vertex start = G.get_vertex(3);
    // Vertex end = G.get_vertex(24);
    forward_propagation(start, end);
    update_Y(start, end);
    backward_propagation(start, end);
}


void GNN::backward_propagation(Vertex start, Vertex end) {

    // check_valid_edges();
    // check_cycles(start, end);
    Matrix W = Matrix(1.0, 2025, 1);

    for (int j = 0; j < 45; j++) {
        W(start.val*45 + j, 0) = 50.0;
    }

    for (int r = 0; r < 45; r++) {
        for (int c = 0; c < 45; c++) {
            W(r*45 + c, 0) += dist(G.get_vertex(r), end) - dist(G.get_vertex(c), end); // Euclidean distance value
        }
    }
    
    // Step 1
    dz_2 = (A_2 - Y) * W;
    // dz_2 = (-1) * (Y.sum_elements() * log(A_2));
    dw_2 = dot(dz_2, A_1.Transpose()) * (1/m);
    // db_2 = col_summation(dz_2) * (1/m);
    // db_2 = dz_2 * (1/m);
    db_2 = dz_2;
    // replace^

    // Step 2
    dz_1 = dot(W_2.Transpose(), dz_2) * sigmoid_prime(Z_1);
    dw_1 =  dot(dz_1, X.Transpose()) * ((double) (1/m));
    // db_1 = col_summation(dz_1) * ((double)(1/m));
    // db_1 = dz_1 * (1/m);
    db_1 = dz_1;
    //replace^

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
    std::tuple<std::vector<double>, std::vector<int>, int> tup = G.Seriel_A_Star(start, end);
    std::vector<double> dist = std::get<0>(tup);
    std::vector<int> prev = std::get<1>(tup);
    std::vector<int> path = get_and_print_path(prev, start.val, end.val);
    // update Y to be the adjacency matrix 
    // for (int i = 0; i < path.size()-1; i++) {
    //     Y(path[i], path[i+1]) = 1;
    // }
    for (int i = 0; i < path.size()-1; i++) {
        int row_index = path[i];
        int col_index = path[i+1];
        int index = (row_index * 45) + col_index;
        Y(index, 0) = 1.0;
        // std::cout << index << std::endl;
    }

}

void GNN::check_valid_edges() { // Operate on A_2, iterate through, check if adj matrix has a representation. if not, penalize that jump entry

}

void GNN::check_cycles(Vertex start, Vertex end) { // Iterate through the path. If path does not start at the start, penalize the whole thing (or just the start) 
    // // if does not end at the end, then penalize the whole thing (or just the end), if creates a cycle, penalize the cycle. Once path is finished
    // // or it hits a cycle, penalize anything not in the main path with big points.

    // UnionFind union_set = UnionFind(A_2.get_rows());
    // if (A_2(start.val, 0) < 0.5) { // If path produced by network does not start with the starting point
    //     // A_2(start.val, 0) = 1 / A_2(start.val, 0); // penalize by a lot
    //     A_2(start.val, 0) += 10;
    //     return;
    // }

    
    // // Scanning path until end for cycles
    // double v1 = start.val;
    // // std::cout << "huya " << A_2(v1, 0) << std::endl;
    // // std::cout << union_set.find_operation(A_2(v1, 0)) << std::endl;
    // std::vector<int> local_path;
    // while (union_set.find_operation(A_2(v1, 0)) != -1 and union_set.find_operation(v1) != union_set.find_operation(A_2(v1, 0))) {
    //     // std::cout << "rindex: " << union_set.find_operation(v1) << std::endl;
    //     union_set.union_operation(v1, A_2(v1, 0));
    //     local_path.push_back(A_2(v1, 0)); // for offsetting later when penalizing outside entries
    //     v1 = A_2(v1, 0);
    // }


    // // If cycle found
    // if (union_set.find_operation(v1) == union_set.find_operation(A_2(v1, 0))) {
    //     A_2(A_2(v1, 0), 0) += 10; // penalize
    //     A_2(v1, 0) += 10; // penalize
    // }

    // // Penalize entries outside this path
    // for (int i = 0; i < A_2.get_rows(); i++) {
    //     for (auto x : local_path) {
    //         if (A_2(i, 0) == x) {
    //             A_2(i, 0) -= 10;
    //         }

    //     }
    // }
    // for (int i = 0; i < A_2.get_rows(); i++) {
    //     A_2(i, 0) += 10;
    // }
}

void GNN::train(int iterations) { 
    for (int i = 0; i < iterations; i++) {
        training_propagation();
        update_params();
    }
}

std::vector<Edge> GNN::predict(Vertex start, Vertex end) { 
    forward_propagation(start, end);
    // std::cout << "Prediction, A_2: \n" << A_2 << std::endl;
    // std::cout << A_2(start.val, 0) << " and " << A_2(end.val, 0) << std::endl;
    for (int i = 0; i < A_2.get_rows(); i++) {
        if (A_2(i, 0) >= 0.001) {
            std::cout << A_2(i, 0) << " with index: " << i << std::endl;
        }
    }
    return std::vector<Edge>();  // Return prediction vector
}


void GNN::print_forward_prop(){
    // std::cout << "Forward prop:\n" << A_0 << std::endl;
    // std::cout << W_1 << std::endl;
    // std::cout << "Z_1:\n" << Z_1 << std::endl;
    // std::cout << "A_1:\n" << A_1 << std::endl;
    // std::cout << "Z_2:\n" << Z_2 << std::endl;
    // std::cout << "A_2:\n" << A_2 << std::endl << std::endl;
}

void GNN::print_backward_prop() {
    // std::cout << "Backward prop:\n" << A_0 << std::endl;
    // std::cout << "Y:\n" << Y << std::endl;
    // std::cout << "dz_2:\n" << dz_2 << std::endl;
    // std::cout << "dw_2:\n" << dw_2 << std::endl;
    // std::cout << "db_2:\n" << db_2 << std::endl;
    // std::cout << "dz_1:\n" << dz_1 << std::endl;
    // std::cout << "dw_1:\n" << dw_1 << std::endl;
    // std::cout << "db_2:\n" << db_2 << std::endl<< std::endl;
}

void GNN::print_params() {
    // std::cout << "Update params.\nW_1:\n" << W_1 << std::endl;
    // std::cout << "B_2:\n" << B_1 << std::endl;
    // std::cout << "W_2:\n" << W_2 << std::endl;
    // std::cout << "B_2:\n" << B_2 << std::endl << std::endl;
}