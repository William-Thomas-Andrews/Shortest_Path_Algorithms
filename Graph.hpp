#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include "UnionFind.hpp"
#include "Compare.hpp"

void print_path(const std::vector<Vertex> &path) {
    std::cout << "Path: ";
    for (Vertex v : path) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}

using Vertex = unsigned long;
using Weight = unsigned long;

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
            std::string str = "(" + std::to_string(v) + " - " + std::to_string(u) + ")" + "(" + std::to_string(weight) + ")";
            return str;
        }

        const std::string get_string() const {
            std::string str = "(" + std::to_string(v) + " - " + std::to_string(u) + ")" + "(" + std::to_string(weight) + ")";
            return str;
        }

        Vertex get_left() const { return v; }
        Vertex get_right() const { return u; }
        Weight get_weight() { return weight; }
        Vertex get_other(Vertex v) const { 
            if (this->get_left() == v) { return this->get_right(); }
            if (this->get_right() == v) { return this->get_left(); }
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
            if (v == e2.get_left() and u == e2.get_right() and weight == e2.get_weight()) return true;
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
                union_set.union_operation(e.get_left(), e.get_right());
                adj[e.get_left()].push_back(e);
                adj[e.get_right()].push_back(e);
            }
        }
        Graph(std::vector<Edge> input_data) : union_set(UnionFind(input_data.size())) {
            // for (int i = 0; i < input_data.size(); i++) {
            //     adj.r
            // }
            adj.resize(input_data.size()*2);
            for (Edge e : input_data) {
                this->add_element(e);
                union_set.union_operation(e.get_left(), e.get_right());
                adj[e.get_left()].push_back(e);
                adj[e.get_right()].push_back(e);
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
            union_set.union_operation(e.get_left(), e.get_right());
            adj[e.get_left()].push_back(e);
            adj[e.get_right()].push_back(e);
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

        bool in (Vertex v, std::vector<Vertex> vertices) {
            for (Vertex x : vertices) {
                if (v == x) { return true; }
            }
            return false;
        }

        
        std::tuple<std::vector<double>, std::vector<Vertex>> Dijkstra(Vertex v, Vertex end) {
            // Basically without heuristics just push things onto a heap and perform on each turn the lowest one, popping each time and adding neighbors to the heap.
            // std::vector<Vertex> path(1, v);
            // UnionFind union_collection;
            // std::priority_queue<Edge, std::vector<Edge>, Compare<Edge>> pq;
            // int counter = edges.size();
            // while (true) {
            //     print_path(path);
            //     for (Vertex u : path) {
            //         for (Edge& e : adj[u]) {
            //             if (union_collection.find_operation(e.get_left()) != union_collection.find_operation(e.get_right())) {
            //                 // std::cout << union_collection.find_operation(e.get_left()) << std::endl;
            //                 std::cout << e.get_left() << std::endl;
            //                 std::cout << e.get_right() << std::endl;
            //                 pq.push(e);
            //             }
            //         }
            //     }
            //     std::cout << pq.top() << std::endl;

            //     // Now we make a decision and repeat
            //     if (union_collection.is_empty()) {
            //         union_collection.union_operation(v, pq.top().get_other(v));
            //         path.push_back(pq.top().get_other(v));
            //         pq.pop();
            //         continue;
            //     }
            //     Vertex left = pq.top().get_left();
            //     Vertex right = pq.top().get_right();
            //     // If there is a cycle or a used edge
            //     // while (union_collection.find_operation(left) == union_collection.find_operation(v) and union_collection.find_operation(right) == union_collection.find_operation(v)) {
            //     //     pq.pop();
            //     //     left = pq.top().get_left();
            //     //     right = pq.top().get_right();
            //     // }
            //     // If the left vertex is already in the used vertices
            //     if (union_collection.find_operation(left) == union_collection.find_operation(v)) { // the old (used) vertex
            //         path.push_back(right); // the new vertex
            //         if (right == end) { // Then we have found the end point and we return the best path we found
            //             // path.push_back(end);
            //             return path;
            //         }
            //         std::cout << "here" << std::endl;
            //         union_collection.union_operation(right, v);
            //     }
            //     // If the right vertex is already in the used vertices
            //     else if (union_collection.find_operation(right) == union_collection.find_operation(v)) { // the old (used) vertex
            //         path.push_back(left); // the new vertex
            //         if (left == end) { // Then we have found the end point and we return the best path we found
            //             // path.push_back(end);
            //             return path; 
            //         }
            //         union_collection.union_operation(left, v);

            //     }
            //     pq.pop();
            //     counter--;
            //     if (counter < 0) {
            //         std::cout << "fail" << std::endl;
            //         return path;
            //     }
            // }
            // return path;
            // Once done, add lots of heuristics and test. Then add a fancy visual and keep testing.
            // Once done, study and add the neural network.
            // If possible, try to integrate this into osm stuff.


            double inf = 1.0/ 0.0;  // Set this to infinity.
            std::vector<double> dist(adj.size(), inf);
            std::vector<Vertex> prev(adj.size(), -1);

            // Contains previous wedges and their weights for adding to 'count' variable // Actually not needed if you use dist and prev
            // std::vector<std::tuple<Weighted_Edge, double>> prev_edges(G.V(), std::tuple<Weighted_Edge, double>());

            // Update with starting vertex
            dist[v] = 0; // update dist
            prev[v] = v; // update prev

            // The minimum priority queue pq stores the edges by edge weight.
            auto compare = [](Edge e, Edge f) {return (e.get_weight() > f.get_weight());};
            std::priority_queue<Edge, std::vector<Edge>, decltype(compare)> pq(compare);

            // A union set for partitioning
            UnionFind union_set = UnionFind(adj.size());

            // A set of used vertices
            std::vector<Vertex> pivot_vertices = {v};
            // TODO UnionFind pivot_union = UnionFind(adj.size());

            std::cout << adj.size() << "he" << std::endl;
            // Iterate through number of 'turns'
            for (int i = 0; i < adj.size()-1; i++) {
                // Iterate through pivot (used) vertices
                for (Vertex pivot_vertex : pivot_vertices) {
                    // Iterate through options per pivot vertex
                    for (auto edge : adj[pivot_vertex]) {
                        // If adding the pivot vertex would not form a cycle (if the edge would not connect the same partition)
                        if (union_set.find_operation(pivot_vertex) != union_set.find_operation(edge.get_other(pivot_vertex))) {
                            // If the distance can be improved
                            if (dist[edge.get_other(pivot_vertex)] > (dist[pivot_vertex] + edge.get_weight())) {
                                // Then update the dist vector with new dist
                                dist[edge.get_other(pivot_vertex)] = (dist[pivot_vertex] + edge.get_weight());
                                Edge new_edge = Edge(pivot_vertex, edge.get_other(pivot_vertex), dist[edge.get_other(pivot_vertex)]);
                                // Then push it to the queue
                                pq.push(new_edge);

                            }
                            // If it cannot be improved
                            else {
                                // Update dist vector with old dist + weight
                                Edge new_edge = Edge(pivot_vertex, edge.get_other(pivot_vertex), edge.get_weight()+dist[pivot_vertex]);
                                // Then push it to the queue
                                pq.push(new_edge);
                            }
                        }
                    }
                }
                // Take top value of queue, then that is the turn, so update prev and add the vertex that has not been used to the used pivot_vertices
                if (pq.empty()) { return std::tuple(dist, prev); }
                Edge best = pq.top(); pq.pop();
                Vertex first_vertex, second_vertex, old_vertex, new_vertex;
                first_vertex = best.get_left(); second_vertex = best.get_right();
                bool in_first = in(first_vertex, pivot_vertices);
                bool in_second = in(second_vertex, pivot_vertices);
                // If connecting two disjoint trees in an edge with two used vertices
                if (in_first and in_second) {
                    // Decide which edge comes first in prev
                    if (union_set.find_operation(first_vertex) == v) {
                        old_vertex = first_vertex;
                        new_vertex = second_vertex;
                    }
                    else if (union_set.find_operation(second_vertex) == v) {
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

                prev[new_vertex] = old_vertex;

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
                union_set.union_operation(best.get_left(), best.get_right());
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


