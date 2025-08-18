#include "Utils.hpp"

std::vector<int> get_and_print_path(const std::vector<int>& prev, int start, int end) {
    // std::cout << "Prev: ";
    // for (int x : prev) {
    //     std::cout << x << std::endl;
    // }
    std::vector<int> path;
    for (int at = end; at != start && at != -1; at = prev[at]) {
        path.push_back(at);
    }
    if (prev[end] == -1 && end != start) {
        std::cout << "No path found from " << start << " to " << end << std::endl;
        return path;
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());

    std::cout << "Path: ";
    for (int v : path) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
    return path;
}
