#include <fmt/format.h>
#include "TestRunner.hpp"
#include "DeltaField.hpp"


void run_all_tests() {
    std::string file_path = "../src/data/data_set/data_london.txt";
    Graph G = Graph(file_path);

    DeltaField DF(G);
    DF.specified_multithreaded_activation(28, 44);
    DF.specified_standard_activation(28, 44);
}