#include <format>
#include "TestRunner.hpp"
#include "DeltaField.cpp"


void run_all_tests() {

    // TODO: Simplify Everything and make it cleaner and more efficient. Simplify the mess in the Graph and DeltaField classes lol.
    // TODO: Change Neural Network from learning weights to learning which edges to flip on or off (use softmax) and number the edges of course
    // in order to do this. Factor in weights somehow either individually or penalize with total weight. We are not trying to have it guess 
    // weight, but we want it to guess what edge to use, penalizing bad edge choices that result in higher weight.
    // TODO: Scale up to multiple training examples (batch size).
    // TODO UnionFind pivot_union = UnionFind(adj.size());


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
    // std::cout << G << std::endl;

    // vertex_pair = G.get_random_vertex_pair();
    // begin = std::get<0>(vertex_pair);
    // end = std::get<1>(vertex_pair);
    // std::cout << "Start: " << begin.val << ", and end at: " << end.val << std::endl;
    // tup = G.A_Star(begin, end);
    // std::vector<double> dist = std::get<0>(tup);
    // std::vector<int> prev = std::get<1>(tup);
    // std::vector<int> path = get_and_print_path(prev, begin.val, end.val);
    // G.plot_path(prev, begin.val, end.val);

    // std::cout << "All tests done!." << std::endl;
    // for (auto x : path) {
    //     std::cout << x << std::endl;
    // }

    DeltaField DF(G);

    // std::cout << DF.activate_A_Star() << std::endl;

    DF.activate_neural_network();

}