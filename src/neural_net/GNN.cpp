#include "GNN.hpp"
#include "Utils.cpp"

GNN::GNN() {}

// s = 2
// y = 100
// num of perceptrons = 100

GNN::GNN(Graph input_graph, double learning_rate) : G(input_graph), alpha(learning_rate) {

    // std::cout << " THIS IS ALPHA\n\n " << alpha<< std::endl;

    // int s = G.get_vertices().size();
    int s = 2;
    int y = 50;
    xavier_initialization();

    // Generate weights
    W_1 = Matrix(4, s, xavier_lower_bound, xavier_upper_bound);
    W_2 = Matrix(y, 4, xavier_lower_bound, xavier_upper_bound);

    // Generate biases
    B_1 = Matrix(4, m);
    B_2 = Matrix(y, m);

    // Generate input X (for now only one training input instance)
    // perhaps only two vertices, start and finish? start with all for now
    // std::cout << "Vertices size " << G.get_vertices().size() << std::endl;
    // X = Matrix(std::vector<Vertex>{G[0], G[10]}, s, m);
    X = Matrix(std::vector<double>{0, 9}, s, m);
    // std::cout << "X is:\n" << X << std::endl;

    // Generate output Y
    Y = Matrix(y, m);
    // A_0 = Matrix(10, 10);
    A_0 = X;
    A_1 = Matrix(4, m);
    A_2 = Matrix(y, m);
    Z_1 = Matrix(4, m);
    Z_2 = Matrix(y, m);
    dz_2 = Matrix(y, m);
    dz_1 = Matrix(4, m);
    dw_2 = Matrix(y, 4);
    dw_1 = Matrix(4, s);
    db_2 = Matrix(4, m);
    db_1 = Matrix(4, m);
    // alpha = 1;

    

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
    X(0,0) = start.val;
    X(1,0) = end.val;
    A_0 = X;
    Z_1 = dot(W_1, A_0) + B_1;
    // std::cout << B_1 << std::endl;
    A_1 = sigmoid(Z_1);
    // std::cout << "what the sigma?\n\n" << A_1 << std::endl;
    Z_2 = dot(W_2, A_1) + B_2;
    // A_2 = softmax(Z_2);
    A_2 = Z_2;
    // std::cout << "what the softmax?\n\n" << A_2 << std::endl;

    std::cout << "Forward prop:\n" << A_0 << std::endl;
    std::cout << "Z_1:\n" << Z_1 << std::endl;
    std::cout << "A_1:\n" << A_1 << std::endl;
    std::cout << "Z_2:\n" << Z_2 << std::endl;
    std::cout << "A_2:\n" << A_2 << std::endl << std::endl;
}

void GNN::backward_propagation() {
    // Step 1
    dz_2 = A_2 - Y;
    dw_2 = dot(dz_2, A_1.Transpose()) * (1/m);
    db_2 = col_summation(dz_2) * (1/m);
    
    // Step 2
    dz_1 = dot(W_2.Transpose(), dz_2) * sigmoid_prime(Z_1);
    // Matrix T = A_1.Transpose();
    Matrix T;
    T = dz_1;
    // std::cout << "Back prop\n " << T << " " << dz_2.get_cols() << std::endl;
    dw_1 =  dot(dz_1, X.Transpose()) * ((double) (1/m));
    db_1 = col_summation(dz_1) * ((double)(1/m)); // no sum for average because we are only doing one training instance so far
    std::cout << "Backward prop:\n" << A_0 << std::endl;
    std::cout << "Y:\n" << Y << std::endl;
    std::cout << "dz_2:\n" << dz_2 << std::endl;
    std::cout << "dw_2:\n" << dw_2 << std::endl;
    std::cout << "db_2:\n" << db_2 << std::endl;
    std::cout << "dz_1:\n" << dz_1 << std::endl;
    std::cout << "dw_1:\n" << dw_1 << std::endl;
    std::cout << "db_2:\n" << db_2 << std::endl<< std::endl;
}

void GNN::update_params() {
        std::cout << " THIS IS ALPHA\n\n " << alpha << std::endl;
    
    alpha = 0.01;
    std::cout << "Update params before. W_1:\n" << W_1 << std::endl;
    std::cout << "B_2:\n" << B_1 << std::endl;
    std::cout << "W_2:\n" << W_2 << std::endl;
    std::cout << "B_2:\n" << B_2 << std::endl;
    // std::cout << "dz_1:\n" << dz_1 << std::endl;
    // std::cout << "dw_1:\n" << dw_1 << std::endl << std::endl;
    W_1 = W_1 - alpha * dw_1;
    B_1 = B_1 - alpha * db_1;
    W_2 = W_2 - alpha * dw_2;
    B_2 = B_2 - alpha * db_2;
    std::cout << "Update params after. W_1:\n" << W_1 << std::endl;
    std::cout << "B_2:\n" << B_1 << std::endl;
    std::cout << "W_2:\n" << W_2 << std::endl;
    std::cout << "B_2:\n" << B_2 << std::endl << std::endl;
    Matrix ex = alpha * db_2;
    std::cout << alpha << std::endl;
    // std::cout << "dz_1:\n" << dz_1 << std::endl;
    // std::cout << "dw_1:\n" << dw_1 << std::endl << std::endl;
}

void GNN::train() { // Try with single instance input for each iteration
    
    std::vector<double> dist;
    std::vector<int> prev;
    std::vector<int> path;
    std::vector<Edge> solution;
    Vertex start, end;
    std::tuple<Vertex, Vertex> vertex_pair;
    std::tuple<std::vector<double>, std::vector<int>> tup;
    for (int iteration = 0; iteration < 20000; iteration++) {
        
        vertex_pair = G.get_random_vertex_pair();
        start = std::get<0>(vertex_pair);
        end = std::get<1>(vertex_pair);
        forward_propagation(start, end);
        // start = G[16];
        // end = G[27];
        Y.clear();
        tup = G.A_Star(start, end);
        dist = std::get<0>(tup);
        prev = std::get<1>(tup);
        path = get_and_print_path(prev, start.val, end.val);
        for (int at = end.val; at != start.val && at != -1; at = prev[at]) { // change into a function soon
            for (Edge e : G.get_adj()[at]) {
                if (e.get_left().val == prev[at]) {
                    solution.push_back(e);
                }
            }
        }
        std::reverse(solution.begin(), solution.end());
        
        std::cout << solution.size() << std::endl;
        for (int i = 0; i < solution.size(); i++) { // updating Y
            Y(i, 0) = solution[i].get_weight();
        }

        backward_propagation();

        update_params();

        solution.clear();
    }

    Vertex v1 = G.get_vertex(0);
    Vertex v2 = G.get_vertex(10);

    forward_propagation(v1, v2);

    std::cout << "Prediction, A_2: \n" << A_2 << std::endl;

    // Matrix zzz = A_2;//-square(Y);

    // std::cout << "Prediction: \n" << zzz << std::endl;


    // Return;

    // std::vector<double> vec({0, 1, 2, 3, 4, 5});
    // std::vector<double> vec0({0, 10, 20, 30, 40, 50});
    // std::vector<double> vec2({100, 80, 60, 40, 20, 0});

    // Matrix A(vec, 2, 3);

    // Matrix A0(vec0, 2, 3);

    // Matrix B(vec2, 3, 2);

    // std::cout << B << std::endl;

    // Matrix C = col_summation(B);

    // Matrix C = dot(A, B);

    // std::cout << C  << std::endl;

    // Matrix rand = Matrix(3, 3, -1, 1);
    // std::cout << rand << std::endl;

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