#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <filesystem>
#include <fmt/format.h>
#include <thread>
#include <tuple>
#include <atomic>

#include "Graph.hpp"


// The input set for a Graphic Matroid

Graph::Graph() {}
Graph::Graph(int size) : union_set(UnionFind(size)), adj(size) {}
Graph::Graph(std::string file_path) {
    std::cout << "Parsing from: " << file_path << std::endl;
    system("pwd");
    folly::dynamic data = parse_json(file_path);
    Vertex v1, v2;
    Weight weight;
        adj.resize(data["vertices"].size());
    adjacency_matrix = Matrix(data["vertices"].size(), data["vertices"].size());
    for (auto edge : data["edges"]) {
        v1 = Vertex(
            data["vertices"][edge["from"].asInt()]["id"].asInt(), 
            data["vertices"][edge["from"].asInt()]["x"].asInt(), 
            data["vertices"][edge["from"].asInt()]["y"].asInt() 
        );
        if (!in(v1.val, stored_vertices)) { stored_vertices.push_back(v1); }
        v2 = Vertex(
            data["vertices"][edge["to"].asInt()]["id"].asInt(), 
            data["vertices"][edge["to"].asInt()]["x"].asInt(), 
            data["vertices"][edge["to"].asInt()]["y"].asInt()
        );
        if (!in(v2.val, stored_vertices)) { stored_vertices.push_back(v2); }
        
        weight = edge["weight"].asInt();
        this->add_element(Edge(v1, v2, weight));
        adjacency_matrix(v1.val, v2.val) = weight;
        ordered_vertices.push_back(v1); ordered_vertices.push_back(v2);
    }
    // GenerateGraph();
    // std::cout << &ordered_vertices << std::endl; // Prints address for reference (if needed)
}

Graph& Graph::operator=(const Graph& other) {
    if (this != &other) {
        
        edges = other.edges;
        union_set = other.union_set;
        adj = other.adj;
        stored_vertices = other.stored_vertices;
        union_vertices = other.union_vertices;
        solution_edges = other.solution_edges;
        ordered_vertices = other.ordered_vertices;
        adjacency_matrix = other.adjacency_matrix; 
    }
    return *this;
}

Vertex Graph::operator[](int vertex_index) {
    for (Vertex v : ordered_vertices) { if (vertex_index == v.val) { return v; } }
    return Vertex();
}

folly::dynamic Graph::parse_json(std::string file_path) {
    // std::cout << "Parsing from: " << std::filesystem::current_path() << std::endl;
    std::string output = "";
    std::ifstream file(file_path);
    std::string line;
    while (std::getline(file, line)) { output += line; }
    return folly::parseJson(output);
}

Matrix& Graph::get_adjacency_matrix() {
    return adjacency_matrix;
}

std::tuple<Vertex, Vertex> Graph::get_random_vertex_pair() {
    srand(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    if (stored_vertices.size() < 2) {
        throw std::runtime_error("Not enough vertices to form a pair.");
    }
    Vertex v1 = stored_vertices[rand() % stored_vertices.size()];
    Vertex v2 = stored_vertices[rand() % stored_vertices.size()];
    int count = 0;
    while (v1 == v2) { 
        v2 = stored_vertices[rand() % union_set.get_size()]; 
        if (++count > 1000) throw std::runtime_error("Error in processing random vertices from the stored_vertices vector."); 
    }
    // std::cout << rand() % stored_vertices.size() << std::endl;
    return {v1, v2};
}

std::vector<Edge>& Graph::get_solution_edges() { return solution_edges; }

std::vector<Edge>& Graph::get_thread_solution_edges() { return path_edges; }

void Graph::clear_solution() { solution_edges.clear(); }

void Graph::add_element(Edge e) {
    edges.push_back(e);
    union_set.union_operation(e.get_source().val, e.get_destination().val);
    adj[e.get_source().val].push_back(e);
    adj[e.get_destination().val].push_back(e);
}

void Graph::show_solution(int begin, int end, int iteration) {
    // system(fmt::format("rm ../img_gen/g{}.gv ; rm ../static/display{}.png", iteration, iteration).c_str());
    std::ofstream graph_viz_file;
    std::string line, str, color, v1_label, v2_label, v1_shade_str, v2_shade_str;
    graph_viz_file.open(fmt::format("../img_gen/g{}.gv", iteration));
    graph_viz_file << "digraph G {\n\tgraph [pad=\"0.212,0.055\" bgcolor=lightgray]\n\tnode [style=filled]\n\tsplines=true" << std::endl ;
    Vertex v1, v2;
    for (Edge edge : edges) {
        v1 = edge.get_source(); v2 = edge.get_destination();
        v1_label = std::to_string(v1.val), v2_label = std::to_string(v2.val);
        if (v1.val == begin) v1_label = "Cur.";   if (v2.val == begin) v2_label = "Cur.";
        else if (v1.val == end) v1_label = "End";  else if (v2.val == end) v2_label = "End";
        color = "black";
        if (is_solution_edge(edge)) { color = "red"; }
        if (is_thread_solution_edge(edge)) { color = "blue"; }
        v1_shade_str = "", v2_shade_str ="";
        if (v1.val != begin and v1.val != end and color == "red")  { v1_shade_str = "fillcolor=\"#6f6f6fff\""; }
        if (v2.val != begin and v2.val != end and color == "red")  { v2_shade_str = "fillcolor=\"#6f6f6fff\""; }
        graph_viz_file << "\t" << v1.val << fmt::format(" [label=\"{}\", {}{}{}", (v1_label), (v1.val == begin ? "fillcolor=\"#7b9aa7ff\"" : ""), (v1.val == end ? "fillcolor=\"#ae9b0bff\"" : ""), v1_shade_str) << fmt::format("pos=\"{},{}!\"]", v1.x, v1.y) << std::endl;
        graph_viz_file << "\t" << v2.val << fmt::format(" [label=\"{}\", {}{}{}", (v2_label), (v2.val == begin ? "fillcolor=\"#7b9aa7ff\"" : ""), (v2.val == end ? "fillcolor=\"#ae9b0bff\"" : ""), v2_shade_str) << fmt::format("pos=\"{},{}!\"]", v2.x, v2.y) << std::endl;
        graph_viz_file << "\t" << v1.val << " -> " << v2.val << fmt::format(" [color=\"{}\"", color) << fmt::format(" label=\"{}\"]", std::to_string(edge.get_weight())) << std::endl;
    }
    // std::cout << solution_edges.size();
    graph_viz_file << "}";
    graph_viz_file.close();

    // solution_edges.clear();
    // thread_solution_edges.clear();

    // system(fmt::format("killall Preview ; neato -n2 -Tpng ../img_gen/g{}.gv -o ../img_gen/file{}.png ; open ../img_gen/file{}.png", iteration, iteration, iteration).c_str());
    // system(fmt::format(" neato -n2 -Tpng ../img_gen/g{}.gv -o ../img_gen/file{}.png ; open ../img_gen/file{}.png", iteration, iteration, iteration).c_str());
    system(fmt::format(" neato -n2 -Tpng ../img_gen/g{}.gv -o ../static/display{}.png ", iteration, iteration).c_str());
}

void Graph::write_solution(const std::vector<int>& prev, int begin, int end) {
    std::ofstream graph_viz_file;
    std::string line, str, color, v1_label, v2_label, v1_shade_str, v2_shade_str;
    graph_viz_file.open("../img_gen/g.gv");
    graph_viz_file << "digraph G {\n\tgraph [pad=\"0.212,0.055\" bgcolor=lightgray]\n\tnode [style=filled]\n\tsplines=true" << std::endl ;
    Vertex v1, v2;
    for (Edge edge : edges) {
        v1 = edge.get_source(); v2 = edge.get_destination();
        v1_label = std::to_string(v1.val), v2_label = std::to_string(v2.val);
        if (v1.val == begin) v1_label = "Cur.";   if (v2.val == begin) v2_label = "Cur.";
        else if (v1.val == end) v1_label = "End";  else if (v2.val == end) v2_label = "End";
        color = "black";
        if (is_solution_edge(edge)) { color = "red"; }
        v1_shade_str = "", v2_shade_str ="";
        if (v1.val != begin and v1.val != end and color == "red")  { v1_shade_str = "fillcolor=\"#6f6f6fff\""; }
        if (v2.val != begin and v2.val != end and color == "red")  { v2_shade_str = "fillcolor=\"#6f6f6fff\""; }
        graph_viz_file << "\t" << v1.val << fmt::format(" [label=\"{}\", {}{}{}", (v1_label), (v1.val == begin ? "fillcolor=\"#7b9aa7ff\"" : ""), (v1.val == end ? "fillcolor=\"#ae9b0bff\"" : ""), v1_shade_str) << fmt::format("pos=\"{},{}!\"]", v1.x, v1.y) << std::endl;
        graph_viz_file << "\t" << v2.val << fmt::format(" [label=\"{}\", {}{}{}", (v2_label), (v2.val == begin ? "fillcolor=\"#7b9aa7ff\"" : ""), (v2.val == end ? "fillcolor=\"#ae9b0bff\"" : ""), v2_shade_str) << fmt::format("pos=\"{},{}!\"]", v2.x, v2.y) << std::endl;
        graph_viz_file << "\t" << v1.val << " -> " << v2.val << fmt::format(" [color=\"{}\"", color) << fmt::format(" label=\"{}\"]", std::to_string(edge.get_weight())) << std::endl;
    }
    // std::cout << solution_edges.size();
    graph_viz_file << "}";
    graph_viz_file.close();
}

void Graph::plot_path(const std::vector<int>& prev, int begin, int end) {
    std::ofstream graph_viz_file;
    std::string line, str, color, v1_label, v2_label, v1_shade_str, v2_shade_str;
    graph_viz_file.open("../img_gen/g.gv");
    graph_viz_file << "digraph G {\n\tgraph [pad=\"0.212,0.055\" bgcolor=lightgray]\n\tnode [style=filled]\n\tsplines=true" << std::endl ;
    Vertex v1, v2;
    for (Edge edge : edges) {
        v1 = edge.get_source(); v2 = edge.get_destination();
        v1_label = std::to_string(v1.val), v2_label = std::to_string(v2.val);
        if (v1.val == begin) v1_label = "Cur.";   if (v2.val == begin) v2_label = "Cur.";
        else if (v1.val == end) v1_label = "End";  else if (v2.val == end) v2_label = "End";
        color = "black";
        if (is_solution_edge(edge)) { color = "red"; }
        v1_shade_str = "", v2_shade_str ="";
        if (v1.val != begin and v1.val != end and color == "red")  { v1_shade_str = "fillcolor=\"#6f6f6fff\""; }
        if (v2.val != begin and v2.val != end and color == "red")  { v2_shade_str = "fillcolor=\"#6f6f6fff\""; }
        graph_viz_file << "\t" << v1.val << fmt::format(" [label=\"{}\", {}{}{}", (v1_label), (v1.val == begin ? "fillcolor=\"#7b9aa7ff\"" : ""), (v1.val == end ? "fillcolor=\"#ae9b0bff\"" : ""), v1_shade_str) << fmt::format("pos=\"{},{}!\"]", v1.x, v1.y) << std::endl;
        graph_viz_file << "\t" << v2.val << fmt::format(" [label=\"{}\", {}{}{}", (v2_label), (v2.val == begin ? "fillcolor=\"#7b9aa7ff\"" : ""), (v2.val == end ? "fillcolor=\"#ae9b0bff\"" : ""), v2_shade_str) << fmt::format("pos=\"{},{}!\"]", v2.x, v2.y) << std::endl;
        graph_viz_file << "\t" << v1.val << " -> " << v2.val << fmt::format(" [color=\"{}\"", color) << fmt::format(" label=\"{}\"]", std::to_string(edge.get_weight())) << std::endl;
    }
    // std::cout << solution_edges.size();
    graph_viz_file << "}";
    graph_viz_file.close();

    system(" neato -n2 -Tpng ../img_gen/g.gv -o ../img_gen/file.png");
} 

bool Graph::is_solution_edge(Edge edge) {
    for (Edge e : solution_edges) {
        if (e == edge) { return true; }
    }
    // std::cout << edge << std::endl;
    return false;
}

bool Graph::is_thread_solution_edge(Edge edge) {
    for (Edge e : path_edges) {
        if (e == edge) { return true; }
    }
    // std::cout << edge << std::endl;
    return false;
}

Edge Graph::find_edge(Vertex v1, Vertex v2) {
    for (Edge& e : adj[v1.val]) {
        if (e.get_source() == v1 and e.get_destination() == v2) {
            return e;
        }
    }
    return Edge();
}

Edge Graph::find_edge(int v1, int v2) {
    for (Edge& e : adj[v1]) {
        if (e.get_source().val == v1 and e.get_destination().val == v2) {
            return e;
        }
    }
    return Edge();
}

std::vector<Edge>& Graph::get_data() {
    return edges;
}

std::vector<std::vector<Edge>>& Graph::get_adj() {
    return adj;
}

std::string Graph::get_string() {
    std::string str = "";
    for (auto edge : edges) {
        str += edge.get_string() + " ";
    }
    str += "\n";
    return str;
}

bool Graph::in (int val, std::vector<Vertex> vertices) {
    for (Vertex x : vertices) {
        if (val == x.val) { return true; }
    }
    return false;
}

Vertex Graph::get_vertex(int index) {
    for (Vertex v : ordered_vertices) {
        if (v.val == index) {
            return v;
        }
    }
    return Vertex();
}


const std::vector<Vertex>& Graph::get_vertices() {
    return ordered_vertices;
}

signed long Graph::potential(Vertex start, Vertex end) {
    return std::sqrt( std::pow((start.x - end.x), 2) + std::pow((start.y - end.y), 2) );
}

void Graph::heuristic_reweight(Edge& edge, Vertex leading_vertex, Vertex end) {
    int heuristic = potential(leading_vertex, end);
    // std::cout << "-Potential: " << potential(edge.get_other(leading_vertex), end) << ". Potential: " << potential(leading_vertex, end) << ". Diff: " << heuristic << std::endl;
    edge.set_weight(edge.get_weight() + heuristic);
}

void Graph::reweight(Edge& edge, Vertex leading_vertex, Vertex end) {
    edge.set_weight(edge.get_weight());
}



void Graph::Serial_A_Star(Vertex start, Vertex end) {

    solution_edges.clear();
    thread_solution_edges.clear();
    path_edges.clear();
    
    int sum = 0;
    double inf = 1.0 / 0.0; 
    std::vector<double> dist(adj.size(), inf);
    std::vector<int> prev(adj.size(), -1);

    // Update with starting vertex
    dist[start.val] = 0; // update dist
    prev[start.val] = start.val; // update prev

    // The minimum priority queue pq stores the edges by edge weight.
    auto compare = [](Edge e, Edge f) {return (e.get_weight() > f.get_weight());};
    std::priority_queue<Edge, std::vector<Edge>, decltype(compare)> pq(compare);

    std::vector<int> visited(adj.size(), 0); // A visited set for partitioning. Entries are 0 for not visited, and 1 for visited.
    visited[start.val] = 1;

    std::vector<Vertex> pivot_vertices = {start}; // A set of used vertices

    for (int i = 0; i < adj.size()-1; i++) { // Iterate through number of 'turns'
        for (Vertex pivot_vertex : pivot_vertices) { // Iterate through pivot (used) vertices
            for (auto edge : adj[pivot_vertex.val]) { // Iterate through options per pivot vertex
                if (edge.get_destination() != pivot_vertex and visited[edge.get_destination().val] == 0) { // If this edge is not directed towards the pivot and the new vertex has not already been visited
                    if (dist[edge.get_other(pivot_vertex).val] > (dist[pivot_vertex.val] + edge.get_weight())) { // If the distance can be improved
                        dist[edge.get_other(pivot_vertex).val] = (dist[pivot_vertex.val] + edge.get_weight()); // Then update the dist vector with new dist
                    }
                    Edge new_edge = Edge(edge.get_source(), edge.get_destination(), edge.get_weight()+dist[pivot_vertex.val]); // Update dist vector with old dist + weight
                    heuristic_reweight(new_edge, new_edge.get_destination(), end);
                    pq.push(new_edge); // Then push it to the queue
                }
            }
        }

        if (pq.empty()) { 
            return; 
        }

        
        Edge best = pq.top(); pq.pop(); // Take top value of queue, then that is the turn, so update prev and add the vertex that has not been used to the used pivot_vertices
        solution_edges.push_back(best);
        // std::cout << "=======We chose the best: " << best << std::endl;
        prev[best.get_destination().val] = best.get_source().val;
        visited[best.get_destination().val] = 1;
        sum += best.get_weight();

        pivot_vertices.push_back(best.get_destination()); // Add to the used vertices

        
        if (visited[end.val] == visited[best.get_destination().val]) {  // If vertex found:
            std::cout << "We found edge number " << end.val << " with " << best.get_destination().val << " !!" << std::endl;
            break;
        }

        while (!pq.empty()) { // And if vertex not found, pop the rest of the edges out
            pq.pop();
            if (pq.size() > adj.size()) return;
        }

        // and repeat~
    }
    
    return;
}


void Graph::Serial_Dijkstra(Vertex start, Vertex end) {

    solution_edges.clear();
    thread_solution_edges.clear();
    path_edges.clear();
    
    int sum = 0;
    double inf = 1.0 / 0.0; 
    std::vector<double> dist(adj.size(), inf);
    std::vector<int> prev(adj.size(), -1);

    // Update with starting vertex
    dist[start.val] = 0; // update dist
    prev[start.val] = start.val; // update prev

    // The minimum priority queue pq stores the edges by edge weight.
    auto compare = [](Edge e, Edge f) {return (e.get_weight() > f.get_weight());};
    std::priority_queue<Edge, std::vector<Edge>, decltype(compare)> pq(compare);

    std::vector<int> visited(adj.size(), 0); // A visited set for partitioning. Entries are 0 for not visited, and 1 for visited.
    visited[start.val] = 1;

    std::vector<Vertex> pivot_vertices = {start}; // A set of used vertices

    for (int i = 0; i < adj.size()-1; i++) { // Iterate through number of 'turns'
        for (Vertex pivot_vertex : pivot_vertices) { // Iterate through pivot (used) vertices
            for (auto edge : adj[pivot_vertex.val]) { // Iterate through options per pivot vertex
                if (edge.get_destination() != pivot_vertex and visited[edge.get_destination().val] == 0) { // If this edge is not directed towards the pivot and the new vertex has not already been visited
                    if (dist[edge.get_other(pivot_vertex).val] > (dist[pivot_vertex.val] + edge.get_weight())) { // If the distance can be improved
                        dist[edge.get_other(pivot_vertex).val] = (dist[pivot_vertex.val] + edge.get_weight()); // Then update the dist vector with new dist
                    }
                    Edge new_edge = Edge(edge.get_source(), edge.get_destination(), edge.get_weight()+dist[pivot_vertex.val]); // Update dist vector with old dist + weight
                    // heuristic_reweight(new_edge, new_edge.get_destination(), end);
                    pq.push(new_edge); // Then push it to the queue
                }
            }
        }

        if (pq.empty()) { 
            return; 
        }

        
        Edge best = pq.top(); pq.pop(); // Take top value of queue, then that is the turn, so update prev and add the vertex that has not been used to the used pivot_vertices
        solution_edges.push_back(best);
        // std::cout << "=======We chose the best: " << best << std::endl;
        prev[best.get_destination().val] = best.get_source().val;
        visited[best.get_destination().val] = 1;
        sum += best.get_weight();

        pivot_vertices.push_back(best.get_destination()); // Add to the used vertices

        
        if (visited[end.val] == visited[best.get_destination().val]) {  // If vertex found:
            std::cout << "We found edge number " << end.val << " with " << best.get_destination().val << " !!" << std::endl;
            break;
        }

        while (!pq.empty()) { // And if vertex not found, pop the rest of the edges out
            pq.pop();
            if (pq.size() > adj.size()) return;
        }

        // and repeat~
    }
    
    return;
}


void Graph::Parallel_A_Star(Vertex start, Vertex end) {

    solution_edges.clear();
    thread_solution_edges.clear();
    path_edges.clear();

    // If the start is right next to the end, then quickly get it and return
    // if (find_edge(start, end)) {
    //     path_edges.push_back()
    // }

    int sum = 0;
    double inf = 1.0/ 0.0; 
    std::vector<int> prev(adj.size(), -1);
    std::vector<double> dist(adj.size(), inf);
    std::vector<double> thread_dist(adj.size(), inf);
    prev[start.val] = start.val;
    dist[start.val] = 0;
    thread_dist[end.val] = 0;

    std::vector<Vertex> thread_forward(adj.size());

    // The minimum priority queue pq stores the edges by edge weight.
    auto compare = [](Edge e, Edge f) {return (e.get_weight() > f.get_weight());};
    std::priority_queue<Edge, std::vector<Edge>, decltype(compare)> pq(compare);
    std::priority_queue<Edge, std::vector<Edge>, decltype(compare)> final_pq(compare);

    // A visited set for partitioning. Entries are 0 for not visited, and 1 for visited.
    std::vector<std::atomic<int>> visited(adj.size());
    for (auto& x : visited) { x.store(0); }
    visited[start.val] = 1;
    visited[end.val] = 2;

    // A set of used vertices
    std::vector<Vertex> pivot_vertices = {start};
    std::vector<Vertex> thread_pivot_vertices = {end};

    int connections = 0;

    double min_thread_dist = inf;


    // Begin thread: Start auxilary search (a search from the endpoint) -------------------------------------------------
    std::thread t1( [&]() { 
        
        // The minimum priority queue pq stores the edges by edge weight.
        auto thread_compare = [](Edge e, Edge f) {return (e.get_weight() > f.get_weight());};
        std::priority_queue<Edge, std::vector<Edge>, decltype(thread_compare)> thread_pq(thread_compare);

        for (int i = 0; i < adj.size()-1; i++) {  // Iterate through number of 'turns'
            for (Vertex thread_pivot_vertex : thread_pivot_vertices) { // Iterate through pivot (used) vertices
                for (auto edge : adj[thread_pivot_vertex.val]) { // Iterate through options per pivot vertex
                    if (edge.get_source() != thread_pivot_vertex and visited[edge.get_source().val] != 2) { // If this edge is not directed towards the pivot and the new vertex has not already been visited by this thread
                        if (thread_dist[edge.get_other(thread_pivot_vertex).val] > (thread_dist[thread_pivot_vertex.val] + edge.get_weight())) { // If the distance can be improved
                            thread_dist[edge.get_other(thread_pivot_vertex).val] = (thread_dist[thread_pivot_vertex.val] + edge.get_weight()); // Then update the dist vector with new dist
                            if (thread_dist[edge.get_other(thread_pivot_vertex).val] < min_thread_dist) {
                                min_thread_dist = thread_dist[edge.get_other(thread_pivot_vertex).val];
                            }
                        }
                        Edge new_edge = Edge(edge.get_source(), edge.get_destination(), edge.get_weight()+thread_dist[thread_pivot_vertex.val]); // Update dist vector with old dist + weight
                        thread_pq.push(new_edge); // Then push it to the queue
                    }
                }
            }

            if (thread_pq.empty()) { 
                std::cout << "Adj size: " << adj.size() << std::endl;
                return; 
            }

            // Take top value of queue, then that is the turn, so update prev and add the vertex that has not been used to the used pivot_vertices
            Edge best = thread_pq.top(); thread_pq.pop();
            // std::cout << "=======Thread chose the best: \n\n" << best << std::endl;
            // prev[best.get_destination().val] = best.get_source().val;
            thread_forward[best.get_source().val] = best.get_destination();
            thread_solution_edges.push_back(best);

            if (visited[best.get_source().val].load() == 1) { 
                connections++;
               if (connections > adj.size() / 20) return;
            }

            visited[best.get_source().val] = 2;
            
            thread_pivot_vertices.push_back(best.get_source()); // Add to the used vertices

            
            if (visited[best.get_source().val] == 1) {  // If vertex found:
                prev[best.get_destination().val] = best.get_source().val; // Algorithm is complete
                return;
            }

            
            while (!thread_pq.empty()) { // And if vertex not found, pop the rest of the edges out
                thread_pq.pop();
                if (thread_pq.size() > adj.size()) return;
            }

            // and repeat~
        }
        return;
    } );
    // End thread ----------------------------------------------------------------------------------------------
    

    for (int i = 0; i < adj.size()-1; i++) {  // Iterate through number of 'turns'
        for (Vertex pivot_vertex : pivot_vertices) { // Iterate through pivot (used) vertices
            for (auto edge : adj[pivot_vertex.val]) { // Iterate through options per pivot vertex
                if (edge.get_destination() != pivot_vertex and visited[edge.get_destination().val] != 1) { // If this edge is not directed towards the pivot and the new vertex has not already been visited
                    if (dist[edge.get_other(pivot_vertex).val] > (dist[pivot_vertex.val] + edge.get_weight())) { // If the distance can be improved
                        dist[edge.get_other(pivot_vertex).val] = (dist[pivot_vertex.val] + edge.get_weight()); // Then update the dist vector with new dist
                    }
                    Edge new_edge = Edge(edge.get_source(), edge.get_destination(), edge.get_weight()+dist[pivot_vertex.val]); // Update dist vector with old dist + weight
                    heuristic_reweight(new_edge, new_edge.get_destination(), end);
                    pq.push(new_edge); // Then push it to the queue
                }
            }
        }

        if (pq.empty()) { 
            t1.join(); return; 
        }

        
        Edge best = pq.top(); pq.pop(); // Take top value of queue, then that is the turn, so update prev and add the vertex that has not been used to the used pivot_vertices
        solution_edges.push_back(best);
        // std::cout << "=======Main chose the best: \n\n" << best << std::endl;
        prev[best.get_destination().val] = best.get_source().val;

        if (visited[best.get_destination().val].load() == 2) { // If vertex is found
            t1.join();
            for (auto e : solution_edges) {
                if (visited[e.get_destination().val].load() == 2) {
                    Edge temp_edge = Edge(e.get_source(), e.get_destination(), dist[e.get_source().val] + thread_dist[e.get_destination().val]); //e.get_weight() + thread_dist[e.get_destination().val]);
                    final_pq.push(temp_edge);
                }
            }

            Edge connector_edge = final_pq.top(); final_pq.pop();
            // Append prev 
            for (int at = connector_edge.get_source().val; at != start.val && at != -1; at = prev[at]) {
                path_edges.push_back(find_edge(prev[at], at));
            }
            std::reverse(path_edges.begin(), path_edges.end());
            path_edges.push_back(connector_edge);
            // Append forward
            for (int at = connector_edge.get_destination().val; at != end.val && at != -1; at = thread_forward[at].val) {
                path_edges.push_back(find_edge(at, thread_forward[at].val));
            }
            // std::cout << "min_thread_dist: " << min_thread_dist << std::endl;
            for (auto& x : visited) {
                // std::cout << "Visited: " << x << std::endl;
            }
            return; // Then we have connected the two partitions
        }

        visited[best.get_destination().val] = 1;
        sum += best.get_weight();
        
        pivot_vertices.push_back(best.get_destination()); // Add to the used vertices

        while (!pq.empty()) { // And if vertex not found, pop the rest of the edges out
            pq.pop();
            if (pq.size() > adj.size()) { 
                t1.join(); return; 
            } 
        }

        // and repeat~
    }
    t1.join();
    return;
}






std::ostream& operator<<(std::ostream& os, Graph& G) {
    os << G.get_string();
    return os;
}