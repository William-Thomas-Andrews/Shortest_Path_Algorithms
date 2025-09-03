#include "Utils.hpp"
#include <atomic>

std::vector<int> get_and_print_path(const std::vector<int>& prev, int start, int end) {
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

std::vector<int> get_and_print_path(const std::vector<std::atomic<int>>& prev, int start, int end) {
    std::vector<int> path;
    for (int at = end; at != start && at != -1; at = prev[at]) {
        std::cout << "Prev: ";

        path.push_back(at);
    }
    if (prev[end] == -1 && end != start) {
        std::cout << "No path found from " << start << " to " << end << std::endl;
        return path;
    }
    path.push_back(std::atomic<int>(start));
    std::reverse(path.begin(), path.end());

    std::cout << "Path: ";
    for (int v : path) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
    return path;
}