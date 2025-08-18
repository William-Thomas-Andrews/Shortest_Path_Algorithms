#pragma once
#include <random>

using Weight = signed long;
using Coordinate = signed long;

signed long max_val = 10; // Coordinate max value
signed long min_val = -10; // Coordinate min value

struct Vertex {
    int val;
    int x = rand() % (max_val - min_val + 1) + min_val; // x-coordinate
    int y = rand() % (max_val - min_val + 1) + min_val; // y-coordinate
    bool operator==(const Vertex& other) const {
        return val == other.val;
    }
    bool operator!=(const Vertex& other) const {
        return val != other.val;
    }
    bool operator>(const Vertex& other) const {
        return val > other.val;
    }
    bool operator<(const Vertex& other) const {
        return val < other.val;
    }
    Vertex() {}
    Vertex(int v0, int x0, int y0) : val(v0), x(x0), y(y0) {}
};