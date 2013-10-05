#include "Graph.hpp"
#include <iostream> // XXX

/**
 * @author Kevin Yang
 * Implementation class for the Graph instance.
 */

/** Add a vertex with the given name to the Graph. Return false if Vertex 
 *  already exists, else true.
 */
bool Graph::addVertex(std::string const & vertexName) {

  // Check if Vertex with this name already exists.
  int index = getVertexIndex(vertexName);
  if(index >= 0) return false; // don't add duplicates

  // Next available index.
  index = vertexVec.size();

  // Create a new Vertex, add pointer to it at end of vertexVec.
  Vertex *pointer = new Vertex(vertexName, index);
  vertexVec.push_back(pointer); 

  // Associate the vertex name and index in the vertexMap.
  vertexMap[vertexName] = index;
  helperMap[index] = vertexName;

  return true;
}

/** Add the edge (src, dest, cost) to the graph. Return true if edge is 
 *  successfully added, else false.
 */
bool Graph::addEdge(std::string src, std::string dest, double cost = 1, 
                    double time = 1) {

  // Ensure src and dest vertices already exist.
  int srci = getVertexIndex(src);
  int desti = getVertexIndex(dest);
  if(srci < 0 || desti < 0) return false;

  // Get a reference to src Vertex's adjacency list.
  std::list<Edge*> & adj = vertexVec[srci]->adj;
  std::list<Edge*> & ret = vertexVec[desti]->adj;

  // Should iterate through it, make sure an edge to dest is not already there;
  // if it is, return false.
  std::list<Edge*>::iterator it = adj.begin();
  for(; it != adj.end(); it++) if((*it)->dest == desti) return false;

  // Add new Edge object to src's adjacency list. Note that network links are
  // bidirectional, the input graph is undirected: if there's a link from 
  // machine A to machine B, that means there's necessarily also a link from B
  // to A.
  adj.push_back(new Edge(srci, desti, cost, time));
  ret.push_back(new Edge(desti, srci, cost, time));

  return true;
}

/** Return the index of the named Vertex in this Graph, or -1 if there's no
 *  Vertex with that name.
 */
int Graph::getVertexIndex(std::string vname) {
  
  // the find() member function of std::map takes a key, and returns an 
  // iterator to the std::pair with the key and its mapped value, if the key
  // exists in the map; or the end(), if not
  std::map<std::string, int>::iterator it = vertexMap.find(vname);
  if(it == vertexMap.end()) return -1;
  return it->second;

}

/** Build a minimal spanning tree using Kruskal's algorithm.
 */
void Graph::buildMST(Graph g) {

  // Create a forest of one-node trees, one for each vertex of g.
  std::vector<Vertex*> forest;
  std::vector<Vertex*>::iterator vit = g.vertexVec.begin();
  for(; vit != g.vertexVec.end(); vit++) forest.push_back(new Vertex(*vit));

  // Create a priority queue containing all the edges in g, ordered by edge 
  // weight.
  std::priority_queue<Edge> pq;
  for(int i = 0; i < forest.size(); i++) {
    Vertex* it = g.vertexVec[i];
    std::list<Edge*>::iterator start = it->adj.begin();
    std::list<Edge*>::iterator end = it->adj.end();
    for(; start != end; start++) pq.push(*(*start));
  }

  DisjSets disjointSet(pq.size());
  int edgesAdded = 0;
  std::string src, des;
  cost = 0.0;

  // While fewer than forest.size() - 1 edges have been added to the forest:
  while(edgesAdded < forest.size() - 1) {

    // Delete the smallest-weighted edge, (srcVertex, destVertex) from the 
    // priority queue.
    Edge edge = pq.top();
    pq.pop();
    int srcVertex = disjointSet.find(edge.src);
    int destVertex = disjointSet.find(edge.dest);

    // If srcVertex and destVertex already belong to the same tree in the 
    // forest, don't do anything. Adding this edge will create a cycle.
    if(srcVertex == destVertex) continue;

    // Else, srcVertex and destVertex are in different trees. Join those 
    // vertices with that edge (this joins their trees, reducing the number
    // trees in the forest by 1. Then continue.
    else {
      disjointSet.unionByHeight(srcVertex, destVertex);
      cost = cost + edge.cost;
      src = g.helperMap[srcVertex];
      des = g.helperMap[destVertex];
      addVertex(src);
      addVertex(des);
      addEdge(src, des, edge.cost, edge.time);
      addEdge(des, src, edge.cost, edge.time);
      edgesAdded++;
    }

  }

}

/** Use Dijkstra's algorithm to find the minimum time path between each pair of
 *  vertices.
 *  @param startNode the "root" or the starting point of the graph.
 *  @return the minimum time to get from one vertex to another.
 */
int Graph::Dijkstra(int startNode) {
  int total = 0;
  Vertex* pointer;

  // Initialize the vertex vector for the graph. Set all "dist" fields to
  // INFINITY, and all "done" fields to false. Locate the start vertex s. Set
  // its "dist" field to 0, and its "prev" field to -1. Put the pair 
  // (pointer,0) on the priority queue.
  for(int i = 0; i < vertexVec.size(); i++) {
    pointer = vertexVec[i];
    pointer->dist = 99999999;
    pointer->prev = -1;
    pointer->done = false;
  }
  pointer = vertexVec[startNode];
  pointer->dist = 0;
  std::priority_queue<Compare*, std::vector<Compare*>, ComparePtr> pq;
  Compare* compObj = new Compare(pointer, (int)pointer->dist);
  pq.push(compObj);

  // Is the priority queue empty? Done!
  while(!pq.empty()) {

    // Remove from the priority queue the pair (v,cost) with the smallest cost.
    Compare* comp = pq.top();
    pq.pop();

    // Is the "done" field of the vertex v marked true? If yes then continue
    // the loop.
    if(comp->v->done == true) continue;

    // Mark the "done" field of vertex v true. The shortest path from s to v 
    // is now known, and the "cost" and "prev" fields in v are correct.
    comp->v->done = true;

    // Traverse v's adjacency list. For each vertex w adjacent to v:
    std::list<Edge*>::iterator it = comp->v->adj.begin();
    for(; it != comp->v->adj.end(); ++it) {

      // Compute the path cost c from s to w going through v (this is just the
      // sum of the true cost from s to v, which is now known, plus the cost of
      // the edge from v to w).
      Vertex* w = vertexVec[(*it)->dest];

      // If c is less than the best cost from s to w known so far (this value
      // is stored in the "dist" field of w), we have found a better path to w.
      // Update the "dist" field in w with this value, and insert the pair 
      // (w,c) into the priority queue.
      if(w->dist > (comp->v->dist + (*it)->time)) {
	w->dist = (int)(comp->v->dist + (*it)->time);
	w->prev = comp->v->index;
	Compare* comp2 = new Compare(w, (int)w->dist);
	pq.push(comp2);
      }

    }
    total = total + (int)comp->v->dist;
  }

  return total;
}

/** Apply Dijkstra's algorithm repeatedly on the calling object to find the 
 *  minimum time path between each pair of vertices.
 */
int Graph::DijkstraAlg() {
  int total = 0;
  for(int i = 0; i < vertexVec.size(); i++) total += Dijkstra(i);
  return total;
}
