#include <thread>   
#include <chrono>   

#include "DeltaField.hpp"
// #include "Utils.cpp" 


DeltaField::DeltaField(Graph Graph_Input) : G(Graph_Input) {}

int DeltaField::activate_A_Star() {

    int total = 0;
    int total2 = 0;

    Vertex begin, end, step;
    std::string file_path;
    std::tuple<Vertex, Vertex> vertex_pair; 
    std::tuple<std::vector<double>, std::vector<int>, int> tup;
    std::vector<double> dist;
    std::vector<int> prev;
    std::vector<Vertex> path;



    // vertex_pair = G.get_random_vertex_pair();
    // begin = std::get<0>(vertex_pair);
    // end = std::get<1>(vertex_pair);

    begin = G.get_vertex(45);
    end = G.get_vertex(1);

    int i = 0;

    system("killall Preview ; rm -rf ../img_gen/* "); // The Preview process is the default pdf/image viewer app for macOS

    // Time function1
    // int sum = 0;
    // int index = 0;
    // auto start1 = std::chrono::high_resolution_clock::now();
    // while (begin != end) {
    //     path.push_back(begin);
    //     std::cout << "Start: " << begin.val << ", and end at: " << end.val << std::endl;
    //     G.Seriel_A_Star(begin, end);
    //     // path = get_and_print_path(prev, begin.val, end.val);
    //     for (auto z : G.get_solution_edges()) {
    //         path.push_back(z.get_destination());
    //     }
    //     G.show_solution(begin.val, end.val, i); 
    //     step = G.get_vertex(path[1].val);
    //     total += G.find_edge(begin, step).get_weight();
    //     begin = step;  // Take a step forward
    //     std::cout << "Step to: " << step.val << std::endl;
    //     // std::this_thread::sleep_for(std::chrono::seconds(0.1));
    //     i++;
    //     index++;
    //     if (index > 15) {return total;}
    //     path.clear();
    // }
    // auto end1 = std::chrono::high_resolution_clock::now();
    // auto duration1 = duration_cast<std::chrono::microseconds>(end1 - start1).count();
    // std::cout << "Sum1: " << total << std::endl;
    // G.show_solution(begin.val, end.val, i);


    // Vertex begin2, end2, step2;
    // std::string file_path2;
    // // std::tuple<Vertex, Vertex> vertex_pair2; 
    // std::tuple<std::vector<double>, std::vector<int>, int> tup2;
    // std::vector<double> dist2;
    // std::vector<int> prev2;
    // std::vector<Vertex> path2;

    // specified_standard_activation(24, 44);

    // random_multithreaded_activation();
    specified_multithreaded_activation(begin, end);

    // std::cout << "Seriel took " << duration1 << " microseconds\n";

    std::cout << "Done with the Delta Field!" << std::endl;

    // G.show_solution(begin2.val, end2.val, i);

    return total;
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