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

    begin = G.get_vertex(23);
    end = G.get_vertex(43);

    int i = 0;

    system("killall Preview ; rm -rf ../img_gen/* "); // The Preview process is the default pdf/image viewer app for macOS

    // Time function1
    int sum = 0;
    int index = 0;
    auto start1 = std::chrono::high_resolution_clock::now();
    while (begin != end) {
        path.push_back(begin);
        std::cout << "Start: " << begin.val << ", and end at: " << end.val << std::endl;
        G.Seriel_A_Star(begin, end);
        // path = get_and_print_path(prev, begin.val, end.val);
        for (auto z : G.get_solution_edges()) {
            path.push_back(z.get_destination());
        }
        G.show_solution(begin.val, end.val, i); 
        step = G.get_vertex(path[1].val);
        total += G.find_edge(begin, step).get_weight();
        begin = step;  // Take a step forward
        std::cout << "Step to: " << step.val << std::endl;
        // std::this_thread::sleep_for(std::chrono::seconds(0.1));
        i++;
        index++;
        if (index > 15) {return total;}
        path.clear();
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = duration_cast<std::chrono::microseconds>(end1 - start1).count();
    std::cout << "Sum1: " << total << std::endl;
    G.show_solution(begin.val, end.val, i);


    Vertex begin2, end2, step2;
    std::string file_path2;
    // std::tuple<Vertex, Vertex> vertex_pair2; 
    std::tuple<std::vector<double>, std::vector<int>, int> tup2;
    std::vector<double> dist2;
    std::vector<int> prev2;
    std::vector<Vertex> path2;

    // Time function2
    // begin2 = std::get<0>(vertex_pair);
    // end2 = std::get<1>(vertex_pair);
    begin2 = G.get_vertex(23);
    end2 = G.get_vertex(43);
    index = 0;
    auto start_2 = std::chrono::high_resolution_clock::now();
    while (begin2 != end2) {
        std::cout << "Start: " << begin2.val << ", and end at: " << end2.val << std::endl;
        G.Parallel_A_Star(begin2, end2);
        path2.push_back(begin2);
        for (auto z : G.get_solution_edges()) {
            path2.push_back(z.get_destination());
        }
        for (auto z1 : G.get_thread_solution_edges()) {
            // if (z1.get)
            path2.push_back(z1.get_destination());
        }
        std::cout << "two " << std::endl;
        G.show_solution(begin2.val, end2.val, i); 
        step2 = G.get_vertex(path2[1].val);
        total2 += G.find_edge(begin2, step2).get_weight();
        begin2 = step2;  // Take a step forward
        std::cout << " Step to: " << step2.val << std::endl;
        // std::this_thread::sleep_for(std::chrono::seconds(0));
        i++;
        index++;
        if (index > 15) {return total;}
        path2.clear();
    }
    auto end_2 = std::chrono::high_resolution_clock::now();
    auto duration_2 = duration_cast<std::chrono::microseconds>(end_2 - start_2).count();

    std::cout << "Seriel took " << duration1 << " microseconds\n";
    std::cout << "Parallel took " << duration_2 << " microseconds\n";

    
    

    std::cout << "Done with the Delta Field!" << std::endl;

    G.show_solution(begin2.val, end2.val, i);
    std::cout << "hereeeeeee" << std::endl;

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