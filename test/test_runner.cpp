#include <format>
#include "test_runner.hpp"
#include "Graph.hpp"
#include "utils.cpp"
// #include "ShowSolutionGraph.cpp"


void run_all_tests() {
    Graph G;
    Vertex begin, end;
    std::string file_path;
    std::tuple<Vertex, Vertex> vertex_pair; 
    std::tuple<std::vector<double>, std::vector<int>>tup;
    std::vector<double> dist = std::get<0>(tup);
    std::vector<int> prev = std::get<1>(tup);
    for (int i = 9; i < 10; i++) {
        file_path = std::format("../src/data/data_set/data_{}.txt", i);
        G = Graph(file_path);
        std::cout << G << std::endl;
        vertex_pair = G.get_random_vertex_pair();
        begin = std::get<0>(vertex_pair);
        end = std::get<1>(vertex_pair);
        std::cout << "Start: " << begin.val << ", and end at: " << end.val << std::endl;
        auto [dist, prev] = G.Dijkstra(begin, end);
        print_path(prev, begin.val, end.val);
        G.show_solution(prev, begin.val, end.val);
    }
    
    std::cout << "All tests done!." << std::endl;
}