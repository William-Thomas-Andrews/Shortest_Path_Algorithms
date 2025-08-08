#include <iostream>
#include <cassert>
#include "Graph.hpp"

int main() {
    Graph G = Graph(10);
    std::cout << G << std::endl;

    G.add_element(Edge(1, 4, 3));
    G.add_element(Edge(4, 2, 5));
    G.add_element(Edge(5, 2, 5));
    G.add_element(Edge(1, 5, 5));


    std::cout << G << std::endl;

    // std::cout << G.get_adj()[0][0] << std::endl;

    std::vector<Vertex> path = G.A_Star(1, 2);

    for (Vertex x : path) {
        std::cout << x << std::endl;
    }

    return 0;
}