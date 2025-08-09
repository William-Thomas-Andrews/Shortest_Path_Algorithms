#include <iostream>
#include <cassert>
#include <vector>
#include <tuple>
#include "Graph.hpp"

void print_path(const std::vector<Vertex>& prev, Vertex start, Vertex end) {
    std::vector<Vertex> path;
    for (Vertex at = end; at != start && at != -1; at = prev[at]) {
        path.push_back(at);
    }
    if (prev[end] == -1 && end != start) {
        std::cout << "No path found from " << start << " to " << end << std::endl;
        return;
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());

    std::cout << "Path: ";
    for (Vertex v : path) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}

int main() {
    // === Test Case 1: Simple direct connection ===
    {
        std::cout << "=== Test 1: Simple direct connection ===\n";
        std::vector<Edge> edges = {
            Edge(0, 1, 1)
        };
        Graph G(edges);

        auto [dist, prev] = G.Dijkstra(0, 1);
        assert(dist[1] == 1);
        assert(prev[1] == 0);

        print_path(prev, 0, 1);
    }

    // === Test Case 2: Multiple paths, shortest chosen ===
    {
        std::cout << "=== Test 2: Multiple paths ===\n";
        std::vector<Edge> edges = {
            Edge(0, 1, 10),
            Edge(0, 2, 2),
            Edge(2, 3, 2),
            Edge(3, 1, 2) // total 6 vs direct 10
        };
        Graph G(edges);

        auto [dist, prev] = G.Dijkstra(0, 1);
        print_path(prev, 0, 1);
        assert(dist[1] == 6);
        assert(prev[1] == 3);
    }

    // === Test Case 3: Unreachable node ===
    {
        std::cout << "=== Test 3: Unreachable node ===\n";
        std::vector<Edge> edges = {
            Edge(0, 1, 5),
            Edge(2, 3, 7)
        };
        Graph G(edges);

        auto [dist, prev] = G.Dijkstra(0, 3);
        // dist for unreachable node should be some sentinel (like INF)
        constexpr double INF = 1e9;
        assert(dist[3] >= INF || dist[3] < 0);  // depends on your INF implementation
        assert(prev[3] == -1);

        std::cout << "Distance to 3: " << dist[3] << std::endl;
        print_path(prev, 0, 3);
    }

    // === Test Case 4: Cycle in graph ===
    {
        std::cout << "=== Test 4: Cycle ===\n";
        std::vector<Edge> edges = {
            Edge(0, 1, 1),
            Edge(1, 2, 1),
            Edge(2, 0, 1),
            Edge(2, 3, 2)
        };
        Graph G2(edges);

        auto [dist, prev] = G2.Dijkstra(0, 3);
        print_path(prev, 0, 3);
        assert(dist[3] == 3); // path 0->2->3 cost 3

    }

    // === Test Case 5: Larger graph ===
    {
        std::cout << "=== Test 5: Larger graph ===\n";
        std::vector<Edge> edges = {
            Edge(0, 1, 4), Edge(0, 2, 3), Edge(1, 3, 2), Edge(2, 3, 5),
            Edge(3, 4, 1), Edge(4, 5, 7), Edge(5, 6, 2), Edge(6, 7, 1),
            Edge(2, 6, 12), Edge(7, 8, 4), Edge(8, 9, 3), Edge(9, 10, 6),
            Edge(10, 11, 1), Edge(11, 12, 2), Edge(12, 13, 3), Edge(13, 14, 5),
            Edge(14, 0, 10), Edge(1, 8, 9), Edge(5, 13, 8)
        };
        Graph G(edges);

        auto [dist, prev] = G.Dijkstra(0, 14);
        std::cout << "Distance to 14: " << dist[14] << std::endl;
        print_path(prev, 0, 14);

        // You can add asserts based on expected distance if known
    }

    std::cout << "All tests done." << std::endl;
    return 0;
}
