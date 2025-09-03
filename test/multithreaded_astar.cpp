#include <fmt/format.h>
#include "TestRunner.hpp"
#include "DeltaField.hpp"


int main() {
    std::string file_path = "../src/data/data_set/data_london.txt";
    Graph G = Graph(file_path);

    DeltaField DF(G);
    DF.random_multithreaded_activation();

    // Enable for time tests
    // long long count = 0;
    // for (int i = 0; i < 1000; i++) {
    //     count += DF.random_multithreaded_activation();;
    // }
    // std::cout << "Multithreaded count: " << count << std::endl;
}