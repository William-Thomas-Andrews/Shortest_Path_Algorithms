#include <iostream>
#include <format>
#include <fstream>
#include <string>
#include <random>
#include "data_generator.hpp"

void data_gen(int i) {
    std::ofstream file;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist_max_i(10,40);
    std::uniform_int_distribution<std::mt19937::result_type> dist_coord(0,1000);
    std::uniform_int_distribution<std::mt19937::result_type> dist_from_to;
    std::uniform_int_distribution<std::mt19937::result_type> dist_weight(1,100);
    int max, j;
    
    std::cout << i << std::endl;
    file.open(std::format("data_{}.txt", i));
    file << "{\n";
    file << "\t\"vertices\": [\n";
    max = dist_max_i(rng);
    dist_from_to = std::uniform_int_distribution<std::mt19937::result_type>(0, max);
    for (j = 0; j < max; j++) {
        file << "\t\t{\"id\":" << j << ",\"x\":" << std::to_string(dist_coord(rng)) << ",\"y\":" << std::to_string(dist_coord(rng)) << "},\n";
    }
    file << "\t\t{\"id\":" << j << ",\"x\":" << std::to_string(dist_coord(rng)) << ",\"y\":" << std::to_string(dist_coord(rng)) << "}\n";
    file << "\t],\n";
    file << "\t\"edges\": [\n";
    for (int k = 0; k < max*2; k++) {
        file << "\t\t{\"from\":" << std::to_string(dist_from_to(rng)) << ",\"to\":" << dist_from_to(rng) << ",\"weight\":" << std::to_string(dist_weight(rng)) << "},\n";
    }
    file << "\t\t{\"from\":" << std::to_string(dist_from_to(rng)) << ",\"to\":" << std::to_string(dist_from_to(rng)) << ",\"weight\":" << std::to_string(dist_weight(rng)) << "}\n";
    file << "\t]\n";
    file << "}";
}

int main() {
    for (int i = 0; i < 9; i++) {
        data_gen(i);
    }
    return 0;
}