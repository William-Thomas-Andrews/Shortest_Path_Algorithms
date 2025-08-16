#include <thread>   
#include <chrono>   

#include "DeltaField.hpp"
// #include "Graph.hpp"
// #include "utils.cpp"
#include "../test/utils.cpp"


DeltaField::DeltaField(Graph Graph_Input) : G(Graph_Input) {

}

int DeltaField::activate_A_star() {

    int total = 0;

    Vertex begin, end;
    std::string file_path;
    std::tuple<Vertex, Vertex> vertex_pair; 
    std::tuple<std::vector<double>, std::vector<int>> tup;
    std::vector<double> dist;
    std::vector<int> prev;
    std::vector<int> path;


    vertex_pair = G.get_random_vertex_pair();
    begin = std::get<0>(vertex_pair);
    end = std::get<1>(vertex_pair);


    while (begin != end) {
        std::cout << "Start: " << begin.val << ", and end at: " << end.val << std::endl;
        tup = G.A_Star(begin, end);
        dist = std::get<0>(tup);
        prev = std::get<1>(tup);
        path = get_and_print_path(prev, begin.val, end.val);
        G.show_solution(prev, begin.val, end.val); 
        begin = G.get_vertex(path[1]);  // Take a step forward
        std::cout << "Step to: " << path[1] << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    // return
    // TODO: CREATE totals, and multiple frame iterations - auto, not manual
    // TODO: Add more to the map. Then add random effects to some roads to throw off the algorithm.

    std::cout << "Done with the Delta Field!" << std::endl;

    G.show_solution(prev, begin.val, end.val);

}

int DeltaField::activate_neural_network() {

}