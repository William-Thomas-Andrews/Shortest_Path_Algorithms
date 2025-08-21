#include <thread>   
#include <chrono>   

#include "DeltaField.hpp"
// #include "Utils.cpp" 


DeltaField::DeltaField(Graph Graph_Input) : G(Graph_Input) {}

int DeltaField::activate_A_Star() {

    int total = 0;

    Vertex begin, end, step;
    std::string file_path;
    std::tuple<Vertex, Vertex> vertex_pair; 
    std::tuple<std::vector<double>, std::vector<int>> tup;
    std::vector<double> dist;
    std::vector<int> prev;
    std::vector<int> path;

    vertex_pair = G.get_random_vertex_pair();
    begin = std::get<0>(vertex_pair);
    end = std::get<1>(vertex_pair);

    int i = 0;

    system("killall Preview ; rm -rf ../img_gen/* "); // The Preview process is the default pdf/image viewer app for macOS

    while (begin != end) {
        std::cout << "Start: " << begin.val << ", and end at: " << end.val << std::endl;
        tup = G.A_Star(begin, end);
        dist = std::get<0>(tup);
        prev = std::get<1>(tup);
        path = get_and_print_path(prev, begin.val, end.val);
        G.show_solution(prev, begin.val, end.val, i); 
        step = G.get_vertex(path[1]);
        total += G.find_edge(begin, step).get_weight();
        begin = step;  // Take a step forward
        std::cout << "Step to: " << step.val << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        i++;
    }
    
    

    std::cout << "Done with the Delta Field!" << std::endl;

    G.show_solution(prev, begin.val, end.val, i);

    return total;

}

int DeltaField::activate_neural_network() {
    // std::cout << G << std::endl;
    Neural_Network = GNN(G, 0.1);
    Neural_Network.train(2000);
    Vertex v1 = G.get_vertex(0);
    Vertex v2 = G.get_vertex(10);
    Neural_Network.predict(v1, v2);
    return 0;
}