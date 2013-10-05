#ifndef VERTEX_HPP
#define VERTEX_HPP
#include <list>
#include "Edge.hpp"
#include <vector>
#include <string>

/**
 * @author Kevin Yang
 * This class defines the basic information stored for each vertex.
 */
class Vertex {
  friend class Graph;

  private:
    
    // The name of this Vertex.
    std::string name;

    // The adjacency list for this Vertex.
    std::list<Edge*> adj;

    // int index of this Vertex.
    int index;

    // Variables for use by algorithms
    double dist;
    int prev;
    int scratch;
    bool done;

    Vertex(std::string nm, int i) : name(nm), index(i) { }

    Vertex(Vertex* v) {
      name = v->name;
      index = v->index;
      dist = v->dist;
      prev = v->prev;
      scratch = v->scratch;
      done = v->done;
    }

    // Destructor: iterate through adjacency list, deleting edges.
    ~Vertex() {
      std::list<Edge*>::iterator it = adj.begin();
      while(it != adj.end()) {
	delete *it;
	++it;
      }
    }

};

#endif // VERTEX_HPP
