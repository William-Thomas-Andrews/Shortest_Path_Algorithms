#include <iomanip>


template <typename ELEMENT>
class MinCompare {
    public:
        bool operator()(ELEMENT a, ELEMENT b) {
            return (a > b);
        }
};

template <typename ELEMENT>
class MaxCompare {
    public:
        bool operator()(ELEMENT a, ELEMENT b) {
            return (a < b);
        }
};

template <typename ELEMENT>
struct Compare {
    bool operator()(ELEMENT edge1, ELEMENT edge2) {
        return (edge1.get_weight() < edge2.get_weight());
    }
};