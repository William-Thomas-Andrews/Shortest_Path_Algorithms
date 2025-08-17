#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <filesystem>
#include <format>

#include "Graph.hpp"


// The input set for a Graphic Matroid

Graph::Graph() {}
Graph::Graph(int size) : union_set(UnionFind(size)), adj(size) {}
Graph::Graph(std::string file_path) {
    std::cout << file_path << std::endl;
    folly::dynamic data = parse_json(file_path);
    Vertex v1, v2;
    Weight weight;
    adj.resize(data["edges"].size() * 2);
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
        total_vertices.push_back(v1); total_vertices.push_back(v2);
        std::cout << v1.val << " " << v2.val << std::endl; 
    }

    // GenerateGraph();
}

folly::dynamic Graph::parse_json(std::string file_path) {
    std::cout << std::filesystem::current_path() << std::endl;
    std::string output = "";
    std::ifstream file(file_path);
    std::string line;
    while (std::getline(file, line)) { output += line; }
    return folly::parseJson(output);
}

std::tuple<Vertex, Vertex> Graph::get_random_vertex_pair() {
    srand(time(nullptr));
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

std::vector<Edge> Graph::get_solution_edges() { return solution_edges; }

void Graph::clear_solution() { solution_edges.clear(); }

void Graph::add_element(Edge e) {
    edges.push_back(e);
    union_set.union_operation(e.get_left().val, e.get_right().val);
    adj[e.get_left().val].push_back(e);
    adj[e.get_right().val].push_back(e);
}

void Graph::show_solution(const std::vector<int>& prev, int begin, int end) {
    std::ofstream graph_viz_file;
    std::string line, str, color, v1_label, v2_label, v1_shade_str, v2_shade_str;
    graph_viz_file.open("../img_gen/g.gv");
    graph_viz_file << "digraph G {\n\tgraph [pad=\"0.212,0.055\" bgcolor=lightgray]\n\tnode [style=filled]\n\tsplines=true" << std::endl ;
    Vertex v1, v2;
    for (Edge edge : edges) {
        v1 = edge.get_left(); v2 = edge.get_right();
        v1_label = std::to_string(v1.val), v2_label = std::to_string(v2.val);
        if (v1.val == begin) v1_label = "Cur.";   if (v2.val == begin) v2_label = "Cur.";
        else if (v1.val == end) v1_label = "End";  else if (v2.val == end) v2_label = "End";
        color = "black";
        if (is_solution_edge(edge)) { color = "red"; }
        v1_shade_str = "", v2_shade_str ="";
        if (v1.val != begin and v1.val != end and color == "red")  { v1_shade_str = "fillcolor=\"#6f6f6fff\""; }
        if (v2.val != begin and v2.val != end and color == "red")  { v2_shade_str = "fillcolor=\"#6f6f6fff\""; }
        graph_viz_file << "\t" << v1.val << std::format(" [label=\"{}\", {}{}{}", (v1_label), (v1.val == begin ? "fillcolor=\"#7b9aa7ff\"" : ""), (v1.val == end ? "fillcolor=\"#ae9b0bff\"" : ""), v1_shade_str) << std::format("pos=\"{},{}!\"]", v1.x, v1.y) << std::endl;
        graph_viz_file << "\t" << v2.val << std::format(" [label=\"{}\", {}{}{}", (v2_label), (v2.val == begin ? "fillcolor=\"#7b9aa7ff\"" : ""), (v2.val == end ? "fillcolor=\"#ae9b0bff\"" : ""), v2_shade_str) << std::format("pos=\"{},{}!\"]", v2.x, v2.y) << std::endl;
        graph_viz_file << "\t" << v1.val << " -> " << v2.val << std::format(" [color=\"{}\"", color) << std::format(" label=\"{}\"]", std::to_string(edge.get_weight())) << std::endl;
    }
    std::cout << solution_edges.size();
    graph_viz_file << "}";
    graph_viz_file.close();

    solution_edges.clear();

    system("neato -n2 -Tpng ../img_gen/g.gv -o ../img_gen/file1.png ; open ../img_gen/file1.png");
}

void Graph::show_solution(const std::vector<int>& prev, int begin, int end, int iteration) {
    system(std::format("rm ../img_gen/g{}.gv ; rm ../img_gen/file{}.png", iteration, iteration).c_str());
    std::ofstream graph_viz_file;
    std::string line, str, color, v1_label, v2_label, v1_shade_str, v2_shade_str;
    graph_viz_file.open(std::format("../img_gen/g{}.gv", iteration));
    graph_viz_file << "digraph G {\n\tgraph [pad=\"0.212,0.055\" bgcolor=lightgray]\n\tnode [style=filled]\n\tsplines=true" << std::endl ;
    Vertex v1, v2;
    for (Edge edge : edges) {
        v1 = edge.get_left(); v2 = edge.get_right();
        v1_label = std::to_string(v1.val), v2_label = std::to_string(v2.val);
        if (v1.val == begin) v1_label = "Cur.";   if (v2.val == begin) v2_label = "Cur.";
        else if (v1.val == end) v1_label = "End";  else if (v2.val == end) v2_label = "End";
        color = "black";
        if (is_solution_edge(edge)) { color = "red"; }
        v1_shade_str = "", v2_shade_str ="";
        if (v1.val != begin and v1.val != end and color == "red")  { v1_shade_str = "fillcolor=\"#6f6f6fff\""; }
        if (v2.val != begin and v2.val != end and color == "red")  { v2_shade_str = "fillcolor=\"#6f6f6fff\""; }
        graph_viz_file << "\t" << v1.val << std::format(" [label=\"{}\", {}{}{}", (v1_label), (v1.val == begin ? "fillcolor=\"#7b9aa7ff\"" : ""), (v1.val == end ? "fillcolor=\"#ae9b0bff\"" : ""), v1_shade_str) << std::format("pos=\"{},{}!\"]", v1.x, v1.y) << std::endl;
        graph_viz_file << "\t" << v2.val << std::format(" [label=\"{}\", {}{}{}", (v2_label), (v2.val == begin ? "fillcolor=\"#7b9aa7ff\"" : ""), (v2.val == end ? "fillcolor=\"#ae9b0bff\"" : ""), v2_shade_str) << std::format("pos=\"{},{}!\"]", v2.x, v2.y) << std::endl;
        graph_viz_file << "\t" << v1.val << " -> " << v2.val << std::format(" [color=\"{}\"", color) << std::format(" label=\"{}\"]", std::to_string(edge.get_weight())) << std::endl;
    }
    std::cout << solution_edges.size();
    graph_viz_file << "}";
    graph_viz_file.close();

    solution_edges.clear();

    // system(std::format("killall Preview ; neato -n2 -Tpng ../img_gen/g{}.gv -o ../img_gen/file{}.png ; open ../img_gen/file{}.png", iteration, iteration, iteration).c_str());
    system(std::format(" neato -n2 -Tpng ../img_gen/g{}.gv -o ../img_gen/file{}.png ; open ../img_gen/file{}.png", iteration, iteration, iteration).c_str());
}

void Graph::write_solution(const std::vector<int>& prev, int begin, int end) {
    std::ofstream graph_viz_file;
    std::string line, str, color, v1_label, v2_label, v1_shade_str, v2_shade_str;
    graph_viz_file.open("../img_gen/g.gv");
    graph_viz_file << "digraph G {\n\tgraph [pad=\"0.212,0.055\" bgcolor=lightgray]\n\tnode [style=filled]\n\tsplines=true" << std::endl ;
    Vertex v1, v2;
    for (Edge edge : edges) {
        v1 = edge.get_left(); v2 = edge.get_right();
        v1_label = std::to_string(v1.val), v2_label = std::to_string(v2.val);
        if (v1.val == begin) v1_label = "Cur.";   if (v2.val == begin) v2_label = "Cur.";
        else if (v1.val == end) v1_label = "End";  else if (v2.val == end) v2_label = "End";
        color = "black";
        if (is_solution_edge(edge)) { color = "red"; }
        v1_shade_str = "", v2_shade_str ="";
        if (v1.val != begin and v1.val != end and color == "red")  { v1_shade_str = "fillcolor=\"#6f6f6fff\""; }
        if (v2.val != begin and v2.val != end and color == "red")  { v2_shade_str = "fillcolor=\"#6f6f6fff\""; }
        graph_viz_file << "\t" << v1.val << std::format(" [label=\"{}\", {}{}{}", (v1_label), (v1.val == begin ? "fillcolor=\"#7b9aa7ff\"" : ""), (v1.val == end ? "fillcolor=\"#ae9b0bff\"" : ""), v1_shade_str) << std::format("pos=\"{},{}!\"]", v1.x, v1.y) << std::endl;
        graph_viz_file << "\t" << v2.val << std::format(" [label=\"{}\", {}{}{}", (v2_label), (v2.val == begin ? "fillcolor=\"#7b9aa7ff\"" : ""), (v2.val == end ? "fillcolor=\"#ae9b0bff\"" : ""), v2_shade_str) << std::format("pos=\"{},{}!\"]", v2.x, v2.y) << std::endl;
        graph_viz_file << "\t" << v1.val << " -> " << v2.val << std::format(" [color=\"{}\"", color) << std::format(" label=\"{}\"]", std::to_string(edge.get_weight())) << std::endl;
    }
    std::cout << solution_edges.size();
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
        v1 = edge.get_left(); v2 = edge.get_right();
        v1_label = std::to_string(v1.val), v2_label = std::to_string(v2.val);
        if (v1.val == begin) v1_label = "Cur.";   if (v2.val == begin) v2_label = "Cur.";
        else if (v1.val == end) v1_label = "End";  else if (v2.val == end) v2_label = "End";
        color = "black";
        if (is_solution_edge(edge)) { color = "red"; }
        v1_shade_str = "", v2_shade_str ="";
        if (v1.val != begin and v1.val != end and color == "red")  { v1_shade_str = "fillcolor=\"#6f6f6fff\""; }
        if (v2.val != begin and v2.val != end and color == "red")  { v2_shade_str = "fillcolor=\"#6f6f6fff\""; }
        graph_viz_file << "\t" << v1.val << std::format(" [label=\"{}\", {}{}{}", (v1_label), (v1.val == begin ? "fillcolor=\"#7b9aa7ff\"" : ""), (v1.val == end ? "fillcolor=\"#ae9b0bff\"" : ""), v1_shade_str) << std::format("pos=\"{},{}!\"]", v1.x, v1.y) << std::endl;
        graph_viz_file << "\t" << v2.val << std::format(" [label=\"{}\", {}{}{}", (v2_label), (v2.val == begin ? "fillcolor=\"#7b9aa7ff\"" : ""), (v2.val == end ? "fillcolor=\"#ae9b0bff\"" : ""), v2_shade_str) << std::format("pos=\"{},{}!\"]", v2.x, v2.y) << std::endl;
        graph_viz_file << "\t" << v1.val << " -> " << v2.val << std::format(" [color=\"{}\"", color) << std::format(" label=\"{}\"]", std::to_string(edge.get_weight())) << std::endl;
    }
    std::cout << solution_edges.size();
    graph_viz_file << "}";
    graph_viz_file.close();

    system(" neato -n2 -Tpng ../img_gen/g.gv -o ../img_gen/file.png");
} 

bool Graph::is_solution_edge(Edge edge) {
    for (Edge e : solution_edges) {
        if (e == edge) { return true; }
    }
    std::cout << edge << std::endl;
    return false;
}

Edge Graph::find_edge(Vertex v1, Vertex v2) {
    for (Edge& e : adj[v1.val]) {
        if (e.get_left() == v2 or e.get_right() == v2) {
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
    for (Vertex v : total_vertices) {
        if (v.val == index) {
            return v;
        }
    }
    return Vertex();
}

signed long Graph::potential(Vertex start, Vertex end) {
    return std::sqrt( std::sqrt( std::pow((start.x - end.x), 2) + std::pow((start.y - end.y), 2) ) );
}

void Graph::reweight(Edge& edge, Vertex leading_vertex, Vertex end) {
    int heuristic = potential(leading_vertex, end);
    std::cout << "-Potential: " << potential(edge.get_other(leading_vertex), end) << ". Potential: " << potential(leading_vertex, end) << ". Diff: " << heuristic << std::endl;
    edge.set_weight(edge.get_weight() + heuristic);
}

std::tuple<std::vector<double>, std::vector<int>> Graph::A_Star(Vertex v, Vertex end) {


    double inf = 1.0/ 0.0; 
    std::vector<double> dist(adj.size(), inf);
    std::vector<int> prev(adj.size(), -1);

    // Contains previous wedges and their weights for adding to 'count' variable // Actually not needed if you use dist and prev
    // std::vector<std::tuple<Weighted_Edge, double>> prev_edges(G.V(), std::tuple<Weighted_Edge, double>());

    // Update with starting vertex
    dist[v.val] = 0; // update dist
    prev[v.val] = v.val; // update prev

    // The minimum priority queue pq stores the edges by edge weight.
    auto compare = [](Edge e, Edge f) {return (e.get_weight() > f.get_weight());};
    std::priority_queue<Edge, std::vector<Edge>, decltype(compare)> pq(compare);

    // A union set for partitioning
    UnionFind union_set = UnionFind(adj.size());

    // A set of used vertices
    std::vector<Vertex> pivot_vertices = {v};
    // TODO UnionFind pivot_union = UnionFind(adj.size());

    // Iterate through number of 'turns'
    for (int i = 0; i < adj.size()-1; i++) {
        std::cout << "Da size: " << adj.size() - 1 << std::endl;
        // Iterate through pivot (used) vertices
        for (Vertex pivot_vertex : pivot_vertices) {
            // Iterate through options per pivot vertex
            for (auto edge : adj[pivot_vertex.val]) {
                // If this edge is not directed towards the pivot and adding the pivot vertex would not form a cycle (if the edge would not connect the same partition)
                if (edge.get_right() != pivot_vertex and union_set.find_operation(pivot_vertex.val) != union_set.find_operation(edge.get_other(pivot_vertex).val)) {
                    // If the distance can be improved
                    if (dist[edge.get_other(pivot_vertex).val] > (dist[pivot_vertex.val] + edge.get_weight())) {
                        // Then update the dist vector with new dist
                        dist[edge.get_other(pivot_vertex).val] = (dist[pivot_vertex.val] + edge.get_weight());
                        Edge new_edge = Edge(pivot_vertex, edge.get_other(pivot_vertex), dist[edge.get_other(pivot_vertex).val]);
                        std::cout << new_edge << "  ~Past weight: " << new_edge.get_weight() << std::endl;
                        if (in(new_edge.get_left().val, pivot_vertices)) {
                            reweight(new_edge, new_edge.get_right(), end);
                        }
                        else {
                            reweight(new_edge, new_edge.get_left(), end);
                        }
                        std::cout << "New weight: " << new_edge.get_weight() << std::endl;
                        // Then push it to the queue
                        pq.push(new_edge);
                    }
                    // If it cannot be improved
                    else {
                        // Update dist vector with old dist + weight
                        Edge new_edge = Edge(pivot_vertex, edge.get_other(pivot_vertex), edge.get_weight()+dist[pivot_vertex.val]);
                        std::cout << new_edge << "  ~Past weight: " << new_edge.get_weight() << std::endl;
                        if (in(new_edge.get_left().val, pivot_vertices)) {
                            reweight(new_edge, new_edge.get_right(), end);
                        }
                        else {
                            reweight(new_edge, new_edge.get_left(), end);
                        }
                        std::cout << "New weight: " << new_edge.get_weight() << std::endl;
                        // Then push it to the queue
                        pq.push(new_edge);
                    }
                }
            }
        }
        // Take top value of queue, then that is the turn, so update prev and add the vertex that has not been used to the used pivot_vertices
        if (pq.empty()) { 
            // show_solution();
            std::cout << "Adj size: " << adj.size() << std::endl;
            return std::tuple(dist, prev); 
        }
        Edge best = pq.top(); pq.pop();
        solution_edges.push_back(best);
        std::cout << "=======We chose the best: " << best << std::endl;
        Vertex first_vertex, second_vertex, old_vertex, new_vertex;
        first_vertex = best.get_left(); second_vertex = best.get_right();
        bool in_first = in(first_vertex.val, pivot_vertices);
        bool in_second = in(second_vertex.val, pivot_vertices);
        // If connecting two disjoint trees in an edge with two used vertices
        if (in_first and in_second) {
            // Decide which edge comes first in prev
            if (union_set.find_operation(first_vertex.val) == v.val) {
                old_vertex = first_vertex;
                new_vertex = second_vertex;
            }
            else if (union_set.find_operation(second_vertex.val) == v.val) {
                old_vertex = second_vertex;
                new_vertex = first_vertex;
            }
        }
        // If our used vertex is the first vertex in the best edge
        else if (in_first and !in_second) {
            old_vertex = first_vertex;
            new_vertex = second_vertex;
        }
        else if (!in_first and in_second) {
            old_vertex = second_vertex;
            new_vertex = first_vertex;
        }

        prev[new_vertex.val] = old_vertex.val;

        // // Update prev
        // if (in(best.either(), pivot_vertices)) {
        //     old_vertex = best.either();
        //     new_vertex = best.other(best.either());
        //     prev[new_vertex] = old_vertex;
        // }
        // else if (in(best.other(best.either()), pivot_vertices)) {
        //     old_vertex =  best.other(best.either());
        //     new_vertex = best.either();
        //     prev[new_vertex] = old_vertex;
        // }

        // Add to the used vertices
        pivot_vertices.push_back(new_vertex);
        // Union the rest
        union_set.union_operation(best.get_left().val, best.get_right().val);
        // If vertex found:
        if (end.val == new_vertex.val) {
            std::cout << "We found edge number " << end.val << " with " << new_vertex.val << " !! :O" << std::endl;
            break;
        }
        // And if vertex not found, pop the rest of the edges out
        while (!pq.empty()) {
            pq.pop();
            if (pq.size() > adj.size()) return std::tuple(dist, prev);
        }
        // and repeat~
    }
    return std::tuple(dist, prev);
}
        

std::ostream& operator<<(std::ostream& os, Graph& G) {
    os << G.get_string();
    return os;
}