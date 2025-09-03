#include <fmt/format.h>
#include "TestRunner.hpp"
#include "DeltaField.hpp"


int main() {
    std::string file_path = "../src/data/data_set/data_london.txt";
    Graph G = Graph(file_path);

    DeltaField DF(G);
    DF.random_standard_activation();
}