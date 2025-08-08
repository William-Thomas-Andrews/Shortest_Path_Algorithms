#include <iostream>
#include <vector>
#include <algorithm>
#include "UnionFind.hpp"


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

        Vertex get_left() { return v; }
        Vertex get_right() { return u; }
        Weight get_weight() { return weight; }

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
        Graph(std::vector<std::tuple<Vertex, Vertex, Weight>> input_data) : union_set(UnionFind(input_data.size())), adj(input_data.size(), std::vector<Edge>(input_data.size()*2)) {
            for (auto x : input_data) {
                Edge e = Edge(std::get<0>(x), std::get<1>(x), std::get<2>(x));
                this->add_element(e);
                union_set.union_operation(e.get_left(), e.get_right());
                adj[e.get_left()].push_back(e);
                adj[e.get_right()].push_back(e);
            }
        }
        Graph(std::vector<Edge> input_data) : union_set(UnionFind(input_data.size())), adj(input_data.size(), std::vector<Edge>(input_data.size()*2)) {
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

        
        std::vector<Edge> A_Star() {
            // Basically without heuristics just push things onto a heap and perform on each turn the lowest one, popping each time and adding neighbors to the heap.
        }
        
        
        friend std::ostream& operator<<(std::ostream& os, Graph& G);
};

std::ostream& operator<<(std::ostream& os, Graph& G) {
    os << G.get_string();
    return os;
}


