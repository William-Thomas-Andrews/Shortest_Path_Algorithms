#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <filesystem>

#include "folly/dynamic.h"
#include "folly/json.h"
#include "UnionFind.hpp"





// using Vertex = unsigned long;
using Weight = signed long;
using Coordinate = signed long;
signed long max_val = 10; // Coordinate max value
signed long min_val = -10; // Coordinate min value

struct Vertex {
    int val;
    int x = rand() % (max_val - min_val + 1) + min_val; // x-coordinate
    int y = rand() % (max_val - min_val + 1) + min_val; // y-coordinate
    bool operator==(const Vertex& other) const {
        return val == other.val;
    }
    bool operator!=(const Vertex& other) const {
        return val != other.val;
    }
    bool operator>(const Vertex& other) const {
        return val > other.val;
    }
    bool operator<(const Vertex& other) const {
        return val < other.val;
    }
    Vertex() {}
    Vertex(int v0, int x0, int y0) : val(v0), x(x0), y(y0) {}
};

// void print_path(const std::vector<Vertex> &path) {
//     std::cout << "Path: ";
//     for (Vertex v : path) {
//         std::cout << v.val << " ";
//     }
//     std::cout << std::endl;
// }

// Corresponds to the Edge type for Graphic Matroids
class Edge {
    private:
        Vertex v;
        Vertex u;
        Weight weight;

    public:
        Edge() {}
        Edge(Vertex v1, Vertex v2, Weight w) : v(v1 > v2 ? v1 : v2), u(v1 > v2 ? v2 : v1), weight(w) {}

        std::string get_string() {
            std::string str = "(" + std::to_string(v.val) + " - " + std::to_string(u.val) + ")" + "(" + std::to_string(weight) + ")";
            return str;
        }
        const std::string get_string() const {
            std::string str = "(" + std::to_string(v.val) + " - " + std::to_string(u.val) + ")" + "(" + std::to_string(weight) + ")";
            return str;
        }
        
        Vertex get_left() const { return v; }
        Vertex get_right() const { return u; }
        Weight get_weight() { return weight; }
        Vertex get_other(Vertex v) const { 
            if (this->get_left().val == v.val) { return this->get_right(); }
            if (this->get_right().val == v.val) { return this->get_left(); }
        }
        void set_weight(Weight w) { weight = w; }
        
        // Comparison operator <
        bool operator<(Edge& e2) {
            return (weight < e2.get_weight());
        }
        // Comparison operator >
        bool operator>(Edge& e2) {
            return (weight > e2.get_weight());
        }
        // Comparison operator ==
        bool operator==(Edge& e2) {
            if (v.val == e2.get_left().val and u.val == e2.get_right().val and weight == e2.get_weight()) return true;
            return false;
        }
        // Comparison operator !=
        bool operator!=(Edge& e2) {
            return (weight != e2.get_weight());
        }

        friend std::ostream& operator<<(std::ostream& os, Edge& e);
        friend std::ostream& operator<<(std::ostream& os, const Edge& e);
};

std::ostream& operator<<(std::ostream& os, Edge& e) {
    os << e.get_string();
    return os;
}
std::ostream& operator<<(std::ostream& os, const Edge& e) {
    os << e.get_string();
    return os;
}

// The input set for a Graphic Matroid
class Graph {
    private:
        std::vector<Edge> edges;
        UnionFind union_set;
        std::vector<std::vector<Edge>> adj;
        std::vector<Vertex> stored_vertices;
        UnionFind union_vertices;
        std::vector<Edge> solution_edges;

    public:
        Graph() {}
        Graph(int size) : union_set(UnionFind(size)), adj(size) {}
        // Graph(std::vector<std::tuple<Vertex, Vertex, Weight>> input_data) : union_set(UnionFind(input_data.size())) {
        //     for (auto x : input_data) {
        //         Edge e = Edge(std::get<0>(x), std::get<1>(x), std::get<2>(x));
        //         this->add_element(e);
        //     }
        // }
        // Graph(std::vector<Edge> input_data) : union_set(UnionFind(input_data.size())) {
        //     adj.resize(input_data.size()*2);
        //     for (Edge e : input_data) {
        //         this->add_element(e);
        //     }
        // }
        Graph(std::string file_path) {
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
            }
            // GenerateGraph();
        }

        folly::dynamic parse_json(std::string file_path) {
            std::cout << std::filesystem::current_path() << std::endl;
            std::string output = "";
            std::ifstream file(file_path);
            std::string line;
            while (std::getline(file, line)) { output += line; }
            return folly::parseJson(output);
        }

        std::tuple<Vertex, Vertex> get_random_vertex_pair() {
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

        std::vector<Edge> get_solution_edges() { return solution_edges; }

        void clear_solution() { solution_edges.clear(); }

        // // Matroid functions begin --------------------------------------------------------------------------------------------------
        // void min_sort() {
        //     std::sort(edges.begin(), edges.end(), MinCompare<Edge>{});
        // }

        // void max_sort() {
        //     std::sort(edges.begin(), edges.end(), MaxCompare<Edge>{});
        // }

        // bool not_empty() {
        //     return (!edges.empty());
        // }

        // Edge top() {
        //     if (edges.empty()) { throw std::runtime_error("Cannot get first element of an empty graph"); }
        //     else {
        //         return edges[edges.size()-1];
        //     }
        // }

        // // If adding Edge e does not create a cycle then it will return true
        // bool is_independent(Edge& e) {
        //     // If both sides of the edge are in the same partition, 
        //     // then it creates a cycle and we return false because adding 'e' is not valid if we want to keep the graph acyclic.
        //     // Otherwise return true because both parititions are disjoint
        //     return (!(union_set.find_operation(e.get_left()) == union_set.find_operation(e.get_right())));
        // }   

        void add_element(Edge e) {
            edges.push_back(e);
            union_set.union_operation(e.get_left().val, e.get_right().val);
            adj[e.get_left().val].push_back(e);
            adj[e.get_right().val].push_back(e);
        }

        void show_solution(std::vector<int> prev, int begin, int end) {
            for (auto edge : solution_edges) {
                // place_edge();
            }
            std::ofstream graph_viz_file;
            std::string line, str;
            graph_viz_file.open("../src/g.gv");
            graph_viz_file << "graph G {\n\tgraph [pad=\"0.212,0.055\" bgcolor=lightgray]" << std::endl;
            Vertex v1, v2;
            for (Edge edge : solution_edges) {
                v1 = edge.get_left(); v2 = edge.get_right();
                graph_viz_file << "\t" << v1.val << " [fillcolor=\"#d62728\" " << std::format("pos=\"{},{}!\"]", v1.x, v1.y) << std::endl;
                graph_viz_file << "\t" << v2.val << " [fillcolor=\"#d62728\" " << std::format("pos=\"{},{}!\"]", v2.x, v2.y) << std::endl;
                graph_viz_file << "\t" << v1.val << " -- " << v2.val << std::endl;
            }
            graph_viz_file << "}";
        }

        // void pop() {
        //     edges.pop_back();
        // }
        // // Matroid functions end ---------------------------------------------------------------------------------------------------------


        std::vector<Edge>& get_data() {
            return edges;
        }

        std::vector<std::vector<Edge>>& get_adj() {
            return adj;
        }

        std::string get_string() {
            std::string str = "";
            for (auto edge : edges) {
                str += edge.get_string() + " ";
            }
            str += "\n";
            return str;
        }

        bool in (int val, std::vector<Vertex> vertices) {
            for (Vertex x : vertices) {
                if (val == x.val) { return true; }
            }
            return false;
        }

        signed long potential(Vertex start, Vertex end) {
            return std::sqrt( std::pow((start.x - end.x), 2) + std::pow((start.y - end.y), 2) );
        }

        void reweight(Edge& edge, Vertex leading_vertex, Vertex end) {
            int heuristic = potential(leading_vertex, end);
            std::cout << "-Potential: " << potential(edge.get_other(leading_vertex), end) << ". Potential: " << potential(leading_vertex, end) << ". Diff: " << heuristic << std::endl;
            edge.set_weight(edge.get_weight() + heuristic);
        }
        
        std::tuple<std::vector<double>, std::vector<int>> Dijkstra(Vertex v, Vertex end) {


            // TODO: Construct lots of examples to test this heuristic and new data loading format. Add a visualization. Keep testing, compare to Dijkstra's normal.
            // TODO: Separate the Vertex and Edge class files into more files.
            // TODO: Make it make decisions sequentially in the visualization (add a wait time after each move).
            // Once done, study and add the neural network.
            // Try to make it live and changing.
            // If possible, try to integrate this into osm stuff.


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
                // Iterate through pivot (used) vertices
                for (Vertex pivot_vertex : pivot_vertices) {
                    // Iterate through options per pivot vertex
                    for (auto edge : adj[pivot_vertex.val]) {
                        // If adding the pivot vertex would not form a cycle (if the edge would not connect the same partition)
                        if (union_set.find_operation(pivot_vertex.val) != union_set.find_operation(edge.get_other(pivot_vertex).val)) {
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
                    if (pq.size() > 10) return std::tuple(dist, prev);
                }
                // and repeat~
            }
            return std::tuple(dist, prev);
        }
        
    
        friend std::ostream& operator<<(std::ostream& os, Graph& G);
};

std::ostream& operator<<(std::ostream& os, Graph& G) {
    os << G.get_string();
    return os;
}


