#include <thread>   
#include <chrono>   

#include "DeltaField.hpp"


DeltaField::DeltaField(Graph Graph_Input) : G(Graph_Input) {}

void DeltaField::activate_A_Star() {

    // Vertex begin = G.get_vertex(45);
    // Vertex end = G.get_vertex(1);

    int i = 0;

    system("killall Preview ; rm -rf ../img_gen/* "); // The Preview process is the default pdf/image viewer app for macOS

    // specified_multithreaded_activation(begin, end);
    random_multithreaded_activation();

    // TODO: Test the parallel with the A* without weights (dijkstra basically)
    // TODO: Test larger test cases
    // TODO: Factor in the matrix class
    // TODO: Test code a lot
    // TODO: Make code cleaner and run smoother
    // TODO: Make accessible to other OS by hosting the images on a website or doing a popup window
    // TODO: Containerize it to the best practice standards
    // TODO: Do write up

    std::cout << "Done with the Delta Field!" << std::endl;
    return;
}

void DeltaField::specified_standard_activation(Vertex start, Vertex end) {
    std::vector<Vertex> path;
    Vertex step;
    int iteration = 0;
    int i = 1;
    auto start_time = std::chrono::high_resolution_clock::now();
    G.Seriel_A_Star(start, end);
    path.push_back(start);
    for (auto edge : G.get_solution_edges()) {
        path.push_back(edge.get_destination());
    }
    while (start != end) {
        std::cout << "Start: " << start.val << ", and end at: " << end.val << std::endl;
        G.show_solution(start.val, end.val, iteration); 
        step = G.get_vertex(path[i].val);
        i++;
        // total2 += G.find_edge(begin2, step2).get_weight();
        start = step;  // Take a step forward
        std::cout << " Step to: " << step.val << std::endl;
        // std::this_thread::sleep_for(std::chrono::seconds(0)); // optional for delay in image popup time
        iteration++;
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    std::cout << "This took " << duration << " microseconds\n";
}

void DeltaField::specified_standard_activation(int start_val, int end_val) {
    Vertex start = G.get_vertex(start_val);
    Vertex end = G.get_vertex(end_val);
    std::vector<Vertex> path;
    Vertex step;
    int iteration = 0;
    int i = 1;
    auto start_time = std::chrono::high_resolution_clock::now();
    G.Seriel_A_Star(start, end);
    path.push_back(start);
    for (auto edge : G.get_solution_edges()) {
        path.push_back(edge.get_destination());
    }
    while (start != end) {
        std::cout << "Start: " << start.val << ", and end at: " << end.val << std::endl;
        G.show_solution(start.val, end.val, iteration); 
        step = G.get_vertex(path[i].val);
        i++;
        // total2 += G.find_edge(begin2, step2).get_weight();
        start = step;  // Take a step forward
        std::cout << " Step to: " << step.val << std::endl;
        // std::this_thread::sleep_for(std::chrono::seconds(0)); // optional for delay in image popup time
        iteration++;
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    std::cout << "This took " << duration << " microseconds\n";
}

void DeltaField::specified_multithreaded_activation(Vertex start, Vertex end) {
    std::vector<Vertex> path;
    Vertex step;
    int iteration = 0;
    int i = 1;
    auto start_time = std::chrono::high_resolution_clock::now();
    G.Parallel_A_Star(start, end);
    path.push_back(start);
    for (auto edge : G.get_thread_solution_edges()) {
        path.push_back(edge.get_destination());
        std::cout << edge.get_destination().val << "->";
    }
    std::cout << std::endl;
    while (start != end) {
        std::cout << "Start: " << start.val << ", and end at: " << end.val << std::endl;
        G.show_solution(start.val, end.val, iteration); 
        step = G.get_vertex(path[i].val);
        i++;
        // total2 += G.find_edge(begin2, step2).get_weight();
        start = step;  // Take a step forward
        std::cout << " Step to: " << step.val << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // optional for delay in image popup time
        iteration++;
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    std::cout << "This took " << duration << " microseconds\n";
}

void DeltaField::specified_multithreaded_activation(int start_val, int end_val) {
    Vertex start = G.get_vertex(start_val);
    Vertex end = G.get_vertex(end_val);
    std::vector<Vertex> path;
    Vertex step;
    int iteration = 0;
    int i = 1;
    auto start_time = std::chrono::high_resolution_clock::now();
    G.Parallel_A_Star(start, end);
    path.push_back(start);
    for (auto edge : G.get_thread_solution_edges()) {
        path.push_back(edge.get_destination());
        std::cout << edge.get_destination().val << "->";
    }
    std::cout << std::endl;
    while (start != end) {
        std::cout << "Start: " << start.val << ", and end at: " << end.val << std::endl;
        G.show_solution(start.val, end.val, iteration); 
        step = G.get_vertex(path[i].val);
        i++;
        // total2 += G.find_edge(begin2, step2).get_weight();
        start = step;  // Take a step forward
        std::cout << " Step to: " << step.val << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // optional for delay in image popup time
        iteration++;
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    std::cout << "This took " << duration << " microseconds\n";
}

void DeltaField::random_standard_activation() {
    std::tuple<Vertex, Vertex> vertex_pair(G.get_random_vertex_pair());
    Vertex start = std::get<0>(vertex_pair);
    Vertex end = std::get<1>(vertex_pair);
    std::vector<Vertex> path;
    Vertex step;
    int iteration = 0;
    int i = 1;
    auto start_time = std::chrono::high_resolution_clock::now();
    G.Seriel_A_Star(start, end);
    path.push_back(start);
    for (auto edge : G.get_solution_edges()) {
        path.push_back(edge.get_destination());
    }
    while (start != end) {
        std::cout << "Start: " << start.val << ", and end at: " << end.val << std::endl;
        G.show_solution(start.val, end.val, iteration); 
        step = G.get_vertex(path[i].val);
        i++;
        // total2 += G.find_edge(begin2, step2).get_weight();
        start = step;  // Take a step forward
        std::cout << " Step to: " << step.val << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // optional for delay in image popup time
        iteration++;
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    std::cout << "This took " << duration << " microseconds\n";
}

void DeltaField::random_multithreaded_activation() {
    std::tuple<Vertex, Vertex> vertex_pair(G.get_random_vertex_pair());
    Vertex start = std::get<0>(vertex_pair);
    Vertex end = std::get<1>(vertex_pair);
    std::vector<Vertex> path;
    Vertex step;
    int iteration = 0;
    int i = 1;
    auto start_time = std::chrono::high_resolution_clock::now();
    G.Parallel_A_Star(start, end);
    path.push_back(start);
    for (auto edge : G.get_thread_solution_edges()) {
        path.push_back(edge.get_destination());
        std::cout << edge.get_destination().val << std::endl;
    }
    while (start != end) {
        std::cout << "Start: " << start.val << ", and end at: " << end.val << std::endl;
        G.show_solution(start.val, end.val, iteration); 
        step = G.get_vertex(path[i].val);
        i++;
        // total2 += G.find_edge(begin2, step2).get_weight();
        start = step;  // Take a step forward
        std::cout << " Step to: " << step.val << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // optional for delay in image popup time
        iteration++;
    }
    G.show_solution(start.val, end.val, iteration); 
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    std::cout << "This took " << duration << " microseconds\n";
}