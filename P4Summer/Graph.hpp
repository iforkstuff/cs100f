#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <map>
#include <vector>
#include "Vertex.hpp"
#include "Edge.hpp"
#include <list>
#include <queue>
#include "DisjSets.hpp"
#include <float.h>
#include "Compare.hpp"

/**
 * @author Kevin Yang
 * A class instances of which represents graphs.
 */

class ComparePtr {
  public:
    bool operator()(Compare* & lhs, Compare* & rhs) const {
      return *lhs < *rhs;
    }
};

class Graph {

  private: 

    // Maps vertex name to number
    std::map<std::string, int> vertexMap;

    // Map number to vertex name
    std::map<int, std::string> helperMap;

    // Vector of Vertexes in Graph
    std::vector<Vertex*> vertexVec;

  public:

    double cost;

    /** Constructor for the Graph instance.
     */
    Graph() {}
    
    /** Destructor: iterate through vertex vector, deleting vertexes.
     */
    ~Graph() {
    }

    /** Add a vertex with the given name to the Graph. Return false if Vertex
     *  already exists, else true.
     */
    bool addVertex(std::string const & vertexName);

    /** Add the edge (src, dest, cost) to the graph.
     *  Return true if edge is successfully added, else false.
     */
    bool addEdge(std::string src, std::string dest, double cost, double time);

    /** Return the index of the named Vertex in this Graph, or -1 if there's no
     *  Vertex with that name.
     */
    int getVertexIndex(std::string vname);

    /** Build a minimal spanning tree using Kruskal's algorithm.
     */
    void buildMST(Graph g);

    /** Using Dijkstra's algorithm to find the shortest graph of the calling 
     *  object, assuming the graph has been built correctly.
     */
    int Dijkstra(int starterNode);

    int DijkstraAlg();

};

#endif // GRAPH_HPP
