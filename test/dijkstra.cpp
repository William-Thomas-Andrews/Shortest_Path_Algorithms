#include <format>
#include "TestRunner.hpp"
#include "DeltaField.hpp"


int main() {
    std::string file_path = "../src/data/data_set/data_london.txt";
    Graph G = Graph(file_path);

    DeltaField DF(G);
    DF.random_dijkstra_activation();
    system("mv ../img_gen/file0.png ../static");
    std::cout << "hereee \n\n\n" << std::endl;
}