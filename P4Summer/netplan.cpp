#include <iostream>
#include <fstream>
#include "Graph.hpp"
#include "DisjSets.hpp"
#include <stdlib.h>

using namespace std;

/**
 * @author Kevin Yang
 * Driver for network infrastructure planning tool.
 */

int main(int argc, char** argv) {

  std::ifstream infile;

  // Exit the program if the user inputs an invalid command. Otherwise, open 
  // the file requested.
  if(argc != 2) {
    std::cerr << "Invalid command!" << std::endl;
    return -1;
  }

  // Exit the program if an error is encountered while opening the requested 
  // file.
  if(!infile) {
    std::cerr << "Failed to open " << argv[1] << ". Program will now exit."
      << std::endl;
    return -1;
  }

  // Create a graph, initially empty. 
  Graph g;
  std::string input;
  double totalCost = 0.0;

  // Open the input file for reading. We assume that the input file is in the
  // format specified in the README.
  infile.open(argv[1]);

  // Read lines from the input file and add appropriate vertices and edges in
  // the associated graph g.
  while(getline(infile, input)) {

    std::string word[4];

    // Read the lines from the input.
    int wordCount = 0;
    for(int i = 0; i < input.length(); i++) {
      if(input[i] == ' ') continue;
      word[wordCount] += input[i];
      if(input[i+1] == ' ') wordCount++;
    }

    // Add appropriate vertices and edges in the associate graph g.
    input = word[0];
    g.addVertex(input);
    g.addVertex(word[1]);
    g.addEdge(word[0], word[1], atof(word[2].c_str()), atof(word[3].c_str()));

    // Compute the totalCost 
    totalCost += atof(word[2].c_str());

  }

  // Close the program if the loop stops for some bad reason.
  if(!infile.eof()) {
    std::cerr << "There was a problem. Sorry!" << std::endl;
    return -1;
  }

  // Close the input file and output the total cost of edges in g.
  infile.close();
  std::cout << totalCost << std::endl;

  // Find a minimum cost spanning tree of graph g, say mst. Output the total
  // cost of edges in mst, and the difference between the total cost of edges
  // in g and in mst. Note that the difference will never be negative.
  Graph mst;
  mst.buildMST(g);
  std::cout << mst.cost << std::endl;
  if(totalCost > mst.cost) std::cout << totalCost - mst.cost << std::endl;
  else std::cout << mst.cost - totalCost << std::endl;

  // Apply Dijkstra's algorithm repeatedly in both mst and g to find the 
  // minimum time path between each pair of vertices. Output the total time for
  // all such pairs for g and mst.
  int totalTransit = g.DijkstraAlg();
  std::cout << totalTransit << std::endl;
  int mstTransit = mst.DijkstraAlg();
  std::cout << mstTransit << std::endl;

  // Also output the difference between them.
  int difference = 0;
  if(mstTransit > totalTransit) difference = mstTransit - totalTransit;
  else difference = totalTransit - mstTransit;
  std::cout << difference << std::endl;

  return 0;
}
