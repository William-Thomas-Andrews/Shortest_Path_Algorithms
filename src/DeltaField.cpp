#include <thread>   
#include <chrono>   

#include "DeltaField.hpp"


DeltaField::DeltaField(Graph Graph_Input) : G(Graph_Input) {}

void DeltaField::specified_dijkstra_activation(Vertex start, Vertex end) {
    int img_index = 0;
    auto start_time = std::chrono::high_resolution_clock::now();
    G.Serial_Dijkstra(start, end);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    std::cout << "This took " << duration << " microseconds\n";
    G.show_solution(start.val, end.val, img_index); 
}

void DeltaField::specified_dijkstra_activation(int start_val, int end_val) {
    Vertex start = G.get_vertex(start_val);
    Vertex end = G.get_vertex(end_val);
    int img_index = 0;
    auto start_time = std::chrono::high_resolution_clock::now();
    G.Serial_Dijkstra(start, end);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    std::cout << "This took " << duration << " microseconds\n";
    G.show_solution(start.val, end.val, img_index); 
}

void DeltaField::specified_standard_activation(Vertex start, Vertex end) {
    int img_index = 0;
    auto start_time = std::chrono::high_resolution_clock::now();
    G.Serial_A_Star(start, end);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    std::cout << "This took " << duration << " microseconds\n";
    G.show_solution(start.val, end.val, img_index); 
}

void DeltaField::specified_standard_activation(int start_val, int end_val) {
    Vertex start = G.get_vertex(start_val);
    Vertex end = G.get_vertex(end_val);
    int img_index = 0;
    auto start_time = std::chrono::high_resolution_clock::now();
    G.Serial_A_Star(start, end);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    std::cout << "This took " << duration << " microseconds\n";
    G.show_solution(start.val, end.val, img_index); 
    
}

void DeltaField::specified_multithreaded_activation(Vertex start, Vertex end) {
    int img_index = 0;
    auto start_time = std::chrono::high_resolution_clock::now();
    G.Parallel_A_Star(start, end);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    std::cout << "This took " << duration << " microseconds\n";
    G.show_solution(start.val, end.val, img_index); 
}

void DeltaField::specified_multithreaded_activation(int start_val, int end_val) {
    Vertex start = G.get_vertex(start_val);
    Vertex end = G.get_vertex(end_val);
    int img_index = 0;
    auto start_time = std::chrono::high_resolution_clock::now();
    G.Parallel_A_Star(start, end);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    std::cout << "This took " << duration << " microseconds\n";
    G.show_solution(start.val, end.val, img_index); 
}

void DeltaField::random_dijkstra_activation() {
    std::tuple<Vertex, Vertex> vertex_pair(G.get_random_vertex_pair());
    Vertex start = std::get<0>(vertex_pair);
    Vertex end = std::get<1>(vertex_pair);
    int img_index = 0;
    auto start_time = std::chrono::high_resolution_clock::now();
    G.Serial_Dijkstra(start, end);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    std::cout << "This took " << duration << " microseconds\n";
    G.show_solution(start.val, end.val, img_index); 
}

void DeltaField::random_standard_activation() {
    std::tuple<Vertex, Vertex> vertex_pair(G.get_random_vertex_pair());
    Vertex start = std::get<0>(vertex_pair);
    Vertex end = std::get<1>(vertex_pair);
    int img_index = 0;
    auto start_time = std::chrono::high_resolution_clock::now();
    G.Serial_A_Star(start, end);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    std::cout << "This took " << duration << " microseconds\n";
    G.show_solution(start.val, end.val, img_index); 
}

void DeltaField::random_multithreaded_activation() {
    std::tuple<Vertex, Vertex> vertex_pair(G.get_random_vertex_pair());
    Vertex start = std::get<0>(vertex_pair);
    Vertex end = std::get<1>(vertex_pair);
    Vertex step;
    int img_index = 0;
    auto start_time = std::chrono::high_resolution_clock::now();
    G.Parallel_A_Star(start, end);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    std::cout << "This took " << duration << " microseconds\n";
    G.show_solution(start.val, end.val, img_index); 
}