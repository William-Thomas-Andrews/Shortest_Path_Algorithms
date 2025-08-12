#pragma once


class UnionFind {
    private:
        std::vector<int> union_data;
        bool empty;
    public:
        UnionFind() : empty(true) {}
        UnionFind(int size) : union_data(size, -1), empty(false ? size <= 0 : true) {}
        
        void union_operation(int v, int u) {
            if (empty) {
                empty = false;
            }
            if (v >= union_data.size()) {
                while (union_data.size() <= v) {
                    union_data.push_back(-1);
                }
            }
            if (u >= union_data.size()) {
                while (union_data.size() <= u) {
                    union_data.push_back(-1);
                }
            }
            int v_index = find_operation(v);
            int u_index = find_operation(u);
            if (v_index == u_index) return;
            if (union_data[v_index] < union_data[u_index]) {
                union_data[v_index] += union_data[u_index];
                union_data[u_index] = v_index;
            }
            if (union_data[v_index] >= union_data[u_index]) {
                union_data[u_index] += union_data[v_index];
                union_data[v_index] = u_index;
            }
        }

        int find_operation(int v) {
            if (v >= union_data.size()) {
                while (union_data.size() <= v) {
                    union_data.push_back(-1);
                }
            }
            if (union_data[v] < 0) {
                return v;
            }
            else {
                return union_data[v] = find_operation(union_data[v]);
            }
        }

        void print_data() {
            for (int i = 0; i < union_data.size(); i++) {
                std::cout << "(" << i << ", " << union_data[i] << ") ";
            }
            std::cout << std::endl;
        }

        std::string get_string() {
            std::string str = "";
            std::cout << union_data.size() << std::endl;
            for (int i = 0; i < union_data.size(); i++) {
                str += "(" + std::to_string(i) + ", " + std::to_string(union_data[i]) + ") ";
            }
            return str;
        }

        int get_size() {
            return union_data.size();
        }

        bool is_empty() {
            return empty;
        }

        friend std::ostream& operator<<(std::ostream& os, UnionFind union_set);
};

std::ostream& operator<<(std::ostream& os, UnionFind union_set) {
    os << union_set.get_string();
    return os;
}