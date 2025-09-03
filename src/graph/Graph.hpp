#pragma once


#include "folly/dynamic.h"
#include "folly/json.h"
#include "UnionFind.hpp"
#include "Edge.hpp"
#include "Matrix.hpp"


// The input set for a Graphic Matroid
class Graph {
    private:
        std::vector<Edge> edges;
        UnionFind union_set;
        std::vector<std::vector<Edge>> adj;
        std::vector<Vertex> stored_vertices;
        UnionFind union_vertices;
        std::vector<Edge> solution_edges;
        std::vector<Edge> thread_solution_edges;
        std::vector<Edge> path_edges;
        std::vector<Vertex> ordered_vertices;
        Matrix adjacency_matrix;

    public:
        Graph();
        Graph(int size);
        Graph(std::string file_path);

        // void operator=(const Graph& other);
        Graph& operator=(const Graph& other);

        Vertex operator[](int vertex_index);

        Matrix& get_adjacency_matrix();

        folly::dynamic parse_json(std::string file_path);

        std::tuple<Vertex, Vertex> get_random_vertex_pair();

        std::vector<Edge>& get_solution_edges();

        std::vector<Edge>& get_thread_solution_edges();

        void clear_solution();

        void add_element(Edge e);

        void show_solution(int begin, int end, int iteration);

        void write_solution(const std::vector<int>& prev, int begin, int end);

        void plot_path(const std::vector<int>& prev, int begin, int end);

        bool is_solution_edge(Edge edge);

        bool is_thread_solution_edge(Edge edge);

        Edge find_edge(Vertex v1, Vertex v2);

        Edge find_edge(int v1, int v2);

        std::vector<Edge>& get_data();

        std::vector<std::vector<Edge>>& get_adj();

        std::string get_string();

        bool in (int val, std::vector<Vertex> vertices);

        Vertex get_vertex(int index);

        const std::vector<Vertex>& get_vertices();

        signed long potential(Vertex start, Vertex end);

        void heuristic_reweight(Edge& edge, Vertex leading_vertex, Vertex end);

        void reweight(Edge& edge, Vertex leading_vertex, Vertex end);
        
        void Serial_A_Star(Vertex start, Vertex end);

        void Serial_Dijkstra(Vertex start, Vertex end);

        void Parallel_A_Star(Vertex start, Vertex end);

        // void Auxiliary_Dijkstra(Vertex v, Vertex end, std::vector<std::atomic<int>>& prev, std::vector<std::atomic<int>>& visited);
        
        friend std::ostream& operator<<(std::ostream& os, Graph& G);
};

