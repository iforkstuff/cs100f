#ifndef COMPARE_HPP_
#define COMPARE_HPP_
#include "Vertex.hpp"

class Compare {
  public:
    Vertex* v;
    int dist;
    Compare(Vertex* v, int dist) : v(v), dist(dist) {}

    bool operator<(Compare const & other) const;
};

#endif // COMPARE_HPP_

