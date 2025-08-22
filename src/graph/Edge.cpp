#include "Edge.hpp"


Edge::Edge() {}
Edge::Edge(Vertex v1, Vertex v2, Weight w) {
    v = v1;
    u = v2;
    weight = w;
}

std::string Edge::get_string() {
    std::string str = "(" + std::to_string(v.val) + " - " + std::to_string(u.val) + ")" + "(" + std::to_string(weight) + ")";
    return str;
}

Vertex Edge::get_source() { return v; }
Vertex Edge::get_destination() { return u; }
Weight Edge::get_weight() { return weight; }
Vertex Edge::get_other(Vertex v) { 
    if (this->get_source().val == v.val) { return this->get_destination(); }
    if (this->get_destination().val == v.val) { return this->get_source(); }
    else { return Vertex(); }
}
void Edge::set_weight(Weight w) { weight = w; }

// Comparison operator <
bool Edge::operator<(Edge& e2) {
    return (weight < e2.get_weight());
}
// Comparison operator >
bool Edge::operator>(Edge& e2) {
    return (weight > e2.get_weight());
}
// Comparison operator ==
bool Edge::operator==(Edge& e2) {
    if (v.val == e2.get_source().val and u.val == e2.get_destination().val) return true;
    return false;
}
// Comparison operator !=
bool Edge::operator!=(Edge& e2) {
    return (weight != e2.get_weight());
}

std::ostream& operator<<(std::ostream& os, Edge& e) {
    os << e.get_string();
    return os;
}