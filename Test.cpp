#include <iostream>
#include <cassert>
#include "Graph.hpp"

int main() {
    Graph G = Graph(10);
    std::cout << G << std::endl;

    G.add_element(Edge(1, 2, 3));

    std::cout << G << std::endl;

    // std::cout << G.get_adj()[0][0] << std::endl;


    return 0;
}