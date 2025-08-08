using Vertex = unsigned long;

class UnionFind {
    private:
        std::vector<int> union_data;
    public:
        UnionFind() {}
        UnionFind(int size) : union_data(size, -1) {}
        
        void union_operation(Vertex v, Vertex u) {
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
            Vertex v_index = find_operation(v);
            Vertex u_index = find_operation(u);
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

        Vertex find_operation(Vertex v) {
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

        friend std::ostream& operator<<(std::ostream& os, UnionFind union_set);
};

std::ostream& operator<<(std::ostream& os, UnionFind union_set) {
    os << union_set.get_string();
    return os;
}