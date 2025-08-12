// #include <iostream>
#include <format>

#include "test_runner.hpp"
#include "Graph.hpp"
#include "utils.hpp"



void run_all_tests() {
    // // Input data test
    // std::string str = "../src/data/data_2.txt";
    // Graph Gi = Graph(str);
    // std::cout << Gi << std::endl;
    // Vertex v0(0, -1, -7); Vertex v3(3, 4, 1);
    // auto [dist, prev] = Gi.Dijkstra(v0, v3);
    // // print_path(prev, 0, 3); // TODO: Fix linking error

    Graph G;
    Vertex begin, end;
    std::string file_path;
    std::tuple<Vertex, Vertex> vertex_pair; 
    int cou = 10;
    for (int i = 0; i < 2; i++) {
        file_path = std::format("../src/data/data_{}.txt", i);
        G = Graph(file_path);
        std::cout << G << std::endl;
        vertex_pair = G.get_random_vertex_pair();
        begin = std::get<0>(vertex_pair);
        end = std::get<1>(vertex_pair);
        std::cout << "Start: " << begin.val << ", and end at: " << end.val << std::endl;
        auto [dist, prev] = G.Dijkstra(begin, end);
        // print_path(prev, begin.val, end.val); // TOD: Fix linking error
    }

    std::cout << "All tests done!." << std::endl;
}