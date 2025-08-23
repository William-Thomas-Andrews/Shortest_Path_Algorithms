#include <thread>   
#include <chrono>   

#include "DeltaField.hpp"
// #include "Utils.cpp" 


DeltaField::DeltaField(Graph Graph_Input) : G(Graph_Input) {}

int DeltaField::activate_A_Star() {

    int total = 0;

    Vertex begin, end, step;
    std::string file_path;
    std::tuple<Vertex, Vertex> vertex_pair; 
    std::tuple<std::vector<double>, std::vector<int>, int> tup;
    std::vector<double> dist;
    std::vector<int> prev;
    std::vector<int> path;

    vertex_pair = G.get_random_vertex_pair();
    begin = std::get<0>(vertex_pair);
    end = std::get<1>(vertex_pair);

    int i = 0;

    system("killall Preview ; rm -rf ../img_gen/* "); // The Preview process is the default pdf/image viewer app for macOS

    // // Time function1
    int sum = 0;
    auto start1 = std::chrono::high_resolution_clock::now();
    while (begin != end) {
        std::cout << "Start: " << begin.val << ", and end at: " << end.val << std::endl;
        tup = G.Seriel_A_Star(begin, end);
        dist = std::get<0>(tup);
        prev = std::get<1>(tup);
        sum += std::get<2>(tup);
        path = get_and_print_path(prev, begin.val, end.val);
        G.show_solution(prev, begin.val, end.val, i); 
        step = G.get_vertex(path[1]);
        total += G.find_edge(begin, step).get_weight();
        begin = step;  // Take a step forward
        std::cout << "Step to: " << step.val << std::endl;
        // std::this_thread::sleep_for(std::chrono::seconds(0.1));
        i++;
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = duration_cast<std::chrono::microseconds>(end1 - start1).count();
    std::cout << "Sum1: " << total << std::endl;

    // Time function2
    begin = std::get<0>(vertex_pair);
    end = std::get<1>(vertex_pair);
    auto start2 = std::chrono::high_resolution_clock::now();
    while (begin != end) {
        std::cout << "Start: " << begin.val << ", and end at: " << end.val << std::endl;
        tup = G.Parallel_A_Star(begin, end);
        dist = std::get<0>(tup);
        prev = std::get<1>(tup);
        std::cout << "one" << std::endl;
        path = get_and_print_path(prev, begin.val, end.val);
        std::cout << "two " << std::endl;
        G.show_solution(prev, begin.val, end.val, i); 
        step = G.get_vertex(path[1]);
        total += G.find_edge(begin, step).get_weight();
        begin = step;  // Take a step forward
        std::cout << "Step to: " << step.val << std::endl;
        // std::this_thread::sleep_for(std::chrono::seconds(0));
        i++;
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    auto duration2 = duration_cast<std::chrono::microseconds>(end2 - start2).count();

    std::cout << "Seriel took " << duration1 << " microseconds\n";
    std::cout << "Parallel took " << duration2 << " microseconds\n";

    
    

    std::cout << "Done with the Delta Field!" << std::endl;

    G.show_solution(prev, begin.val, end.val, i);

    return total;

}

int DeltaField::activate_neural_network() {
    // std::cout << G << std::endl;
    Neural_Network = GNN(G, 0.01);
    Neural_Network.train(1000);
    Vertex v1 = G.get_vertex(1);
    Vertex v2 = G.get_vertex(10);
    Neural_Network.predict(v1, v2);
    std::cout << "\nsepararse \n\n" << std::endl;
    v1 = G.get_vertex(3);
    v2 = G.get_vertex(24);
    Neural_Network.predict(v1, v2);
    std::cout << "wii made it " << std::endl;
    return 0;
}