#include <format>
#include "test_runner.hpp"
// #include "Graph.cpp"
// #include "utils.cpp"
#include "DeltaField.cpp"


void run_all_tests() {
    Graph G;
    Vertex begin, end;
    std::string file_path;
    std::tuple<Vertex, Vertex> vertex_pair; 
    std::tuple<std::vector<double>, std::vector<int>> tup;
    // for (int i = 8; i < 9; i++) {
    //     file_path = std::format("../src/data/data_set/data_{}.txt", i);
    //     G = Graph(file_path);
    //     std::cout << G << std::endl;
    //     vertex_pair = G.get_random_vertex_pair();
    //     begin = std::get<0>(vertex_pair);
    //     end = std::get<1>(vertex_pair);
    //     std::cout << "Start: " << begin.val << ", and end at: " << end.val << std::endl;
    //     auto [dist, prev] = G.Dijkstra(begin, end);
    //     print_path(prev, begin.val, end.val);
    //     G.show_solution(prev, begin.val, end.val);
    // }
    file_path = "../src/data/data_set/data_london.txt";
    G = Graph(file_path);
    std::cout << G << std::endl;

    // vertex_pair = G.get_random_vertex_pair();
    // begin = std::get<0>(vertex_pair);
    // end = std::get<1>(vertex_pair);
    // std::cout << "Start: " << begin.val << ", and end at: " << end.val << std::endl;
    // tup = G.A_Star(begin, end);
    // std::vector<double> dist = std::get<0>(tup);
    // std::vector<int> prev = std::get<1>(tup);
    // std::vector<int> path = get_and_print_path(prev, begin.val, end.val);
    // G.show_solution(prev, begin.val, end.val);

    // std::cout << "All tests done!." << std::endl;
    // for (auto x : path) {
    //     std::cout << x << std::endl;
    // }

    DeltaField DF(G);

    DF.activate_A_star();
}