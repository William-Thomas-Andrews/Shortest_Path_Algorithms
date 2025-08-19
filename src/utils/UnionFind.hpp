#pragma once


class UnionFind {
    private:
        std::vector<int> union_data;
        bool empty;
    public:
        UnionFind();
        UnionFind(int size);

        void operator=(const UnionFind& other);
        
        void union_operation(int v, int u);

        int find_operation(int v);

        void print_data();

        std::string get_string();

        int get_size();

        bool is_empty();

        friend std::ostream& operator<<(std::ostream& os, UnionFind union_set);
};

std::ostream& operator<<(std::ostream& os, UnionFind union_set);