#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <tuple>

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

int main() {
    // === Test Case 1: Simple direct connection ===
    {
        std::cout << "=== Test 1: Simple direct connection ===\n";
        Vertex v0(0, 1, 2); Vertex v1(1, 3, 4);
        std::vector<Edge> edges = {
            Edge(v0, v1, 1)
        };
        Graph G(edges);

        auto [dist, prev] = G.Dijkstra(v0, v1);
        assert(dist[1] == 1);
        assert(prev[1] == 0);

        print_path(prev, 0, 1);
    }

    // === Test Case 2: Multiple paths, shortest chosen ===
    {
        std::cout << "=== Test 2: Multiple paths ===\n";
        Vertex v0(0, 1, 2); Vertex v1(1, 3, 4); Vertex v2(2, 5, 6); Vertex v3(3, 4, 6);
        std::vector<Edge> edges = {
            Edge(v0, v1, 10),
            Edge(v0, v2, 2),
            Edge(v2, v3, 2),
            Edge(v3, v1, 2) // total 6 vs direct 10
        };
        Graph G(edges);

        auto [dist, prev] = G.Dijkstra(v0, v1);
        print_path(prev, 0, 1);
        assert(dist[1] == 6);
        assert(prev[1] == 3);
    }

    // Input data test
    Graph Gi = Graph("../src/data_1.txt");
    std::cout << Gi << std::endl;


    // // === Test Case 3: Unreachable node ===
    // {
    //     std::cout << "=== Test 3: Unreachable node ===\n";
    //     std::vector<Edge> edges = {
    //         Edge(0, 1, 5),
    //         Edge(2, 3, 7)
    //     };
    //     Graph G(edges);

    //     auto [dist, prev] = G.Dijkstra(0, 3);
    //     // dist for unreachable node should be some sentinel (like INF)
    //     constexpr double INF = 1e9;
    //     assert(dist[3] >= INF || dist[3] < 0);  // depends on your INF implementation
    //     assert(prev[3] == -1);

    //     std::cout << "Distance to 3: " << dist[3] << std::endl;
    //     print_path(prev, 0, 3);
    // }

    // // === Test Case 4: Cycle in graph ===
    // {
    //     std::cout << "=== Test 4: Cycle ===\n";
    //     std::vector<Edge> edges = {
    //         Edge(0, 1, 1),
    //         Edge(1, 2, 1),
    //         Edge(2, 0, 1),
    //         Edge(2, 3, 2)
    //     };
    //     Graph G2(edges);

    //     auto [dist, prev] = G2.Dijkstra(0, 3);
    //     print_path(prev, 0, 3);
    //     assert(dist[3] == 3); // path 0->2->3 cost 3

    // }

    // // === Test Case 5: Larger graph ===
    // {
    //     std::cout << "=== Test 5: Larger graph ===\n";
    //     std::vector<Edge> edges = {
    //         Edge(0, 1, 4), Edge(0, 2, 3), Edge(1, 3, 2), Edge(2, 3, 5),
    //         Edge(3, 4, 1), Edge(4, 5, 7), Edge(5, 6, 2), Edge(6, 7, 1),
    //         Edge(2, 6, 12), Edge(7, 8, 4), Edge(8, 9, 3), Edge(9, 10, 6),
    //         Edge(10, 11, 1), Edge(11, 12, 2), Edge(12, 13, 3), Edge(13, 14, 5),
    //         Edge(14, 0, 10), Edge(1, 8, 9), Edge(5, 13, 8)
    //     };
    //     Graph G(edges);

    //     auto [dist, prev] = G.Dijkstra(0, 13);
    //     std::cout << "Distance to 13: " << dist[13] << std::endl;
    //     print_path(prev, 0, 13);

    //     // for (Vertex x : prev) {
    //     //     std::cout << x << std::endl;
    //     // }

    //     // You can add asserts based on expected distance if known
    // }

    std::cout << "All tests done." << std::endl;
    return 0;
}
