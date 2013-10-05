#ifndef EDGE_HPP
#define EDGE_HPP

/**
 * @author Kevin Yang
 * This class defines the information in each item in a Vertex's adjacency 
 * list.
 */
class Edge {

  friend class Graph;
  friend class Vertex;

  private:
    
    // Source vertex of edge can be implicit.
    int src;

    // Index of destination vertex of this edge.
    int dest;

    // Cost (weight) of this edge
    double cost;

    // Time of this edge
    double time;

    /** Constructor of the Edge instance
     */
    Edge(int s, int d, double c = 1.0, double t = 1.0) {
      src = s;
      dest = d;
      cost = c;
      time = t;
    }

  public:

    /** Overloading the < operator for priority queue comparison.
     */
    bool operator<(const Edge &other) const;

}; 
#endif // EDGE_HPP
