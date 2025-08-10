#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include "UnionFind.hpp"
#include "Compare.hpp"



// using Vertex = unsigned long;
using Weight = unsigned long;
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
    public:
        Graph() {}
        Graph(int size) : union_set(UnionFind(size)), adj(size) {}
        Graph(std::vector<std::tuple<Vertex, Vertex, Weight>> input_data) : union_set(UnionFind(input_data.size())) {
            for (auto x : input_data) {
                Edge e = Edge(std::get<0>(x), std::get<1>(x), std::get<2>(x));
                this->add_element(e);
                union_set.union_operation(e.get_left().val, e.get_right().val);
                adj[e.get_left().val].push_back(e);
                adj[e.get_right().val].push_back(e);
            }
        }
        Graph(std::vector<Edge> input_data) : union_set(UnionFind(input_data.size())) {
            // for (int i = 0; i < input_data.size(); i++) {
            //     adj.r
            // }
            adj.resize(input_data.size()*2);
            for (Edge e : input_data) {
                this->add_element(e);
                union_set.union_operation(e.get_left().val, e.get_right().val);
                adj[e.get_left().val].push_back(e);
                adj[e.get_right().val].push_back(e);
            }
        }

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

        void reweight(Edge& edge, Vertex subject, Vertex end) {
            std::cout << "Potential: " << potential(edge.get_other(subject), end) << ". Potential: " << potential(subject, end) << std::endl;
            edge.set_weight(edge.get_weight() + potential(edge.get_other(subject), end) - potential(subject, end));
        }
        
        std::tuple<std::vector<double>, std::vector<int>> Dijkstra(Vertex v, Vertex end) {

            // TODO: Construct lots of examples to test this heuristic. Add a visualization. Keep testing, compare to Dijkstra's normal.
            // TODO: Make it make decisions sequentially in the visualization (add a wait time after each move).
            // Once done, study and add the neural network.
            // Try to make it live and changing.
            // If possible, try to integrate this into osm stuff.


            double inf = 1.0/ 0.0;  // Set this to infinity.
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
                                std::cout << "Past weight: " << new_edge.get_weight() << std::endl;
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
                                std::cout << "Past weight: " << new_edge.get_weight() << std::endl;
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
                if (pq.empty()) { return std::tuple(dist, prev); }
                Edge best = pq.top(); pq.pop();
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
                // And pop the rest of the edges out
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


