#include <iostream>
#include <format>

#include "test_runner.hpp"
#include "Graph.hpp"



void print_path(const std::vector<int>& prev, int start, int end) {
    std::vector<int> path;
    for (int at = end; at != start && at != -1; at = prev[at]) {
        path.push_back(at);
    }
    if (prev[end] == -1 && end != start) {
        std::cout << "No path found from " << start << " to " << end << std::endl;
        return;
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());

    std::cout << "Path: ";
    for (int v : path) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}

void run_all_tests() {
    // Input data test
    std::string str = "../src/data_2.txt";
    Graph Gi = Graph(str);
    std::cout << Gi << std::endl;
    Vertex v0(0, -1, -7); Vertex v3(3, 4, 1);
    auto [dist, prev] = Gi.Dijkstra(v0, v3);
    print_path(prev, 0, 3);

    // Graph G;
    // std::string file_path;
    // for (int i = 0; i < 11; i++) {
    //     file_path = std::format("../src/data_{}.txt", i);
    //     G = Graph(file_path);
    //     Ver
    //     auto [dist, prev] = G.Dijkstra( ,);
    //     print_path(prev, );
    // }

    std::cout << "All tests done!." << std::endl;
}