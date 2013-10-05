#ifndef DISJSETS_HPP
#define DISJSETS_HPP
#include <vector>

/**
 * @author Kevin Yang
 * A very compact and elegant implementation of disjoint subsets suitable for
 * union-by-height and path-compression find can be done by using arrays.
 */

class DisjSets {

  private:

    /** Using the array representation for disjoint subsets */
    std::vector<int> array;

  public:

    /** Construct the disjoint sets object, given the initial number of 
     *  disjoint sets.
     */
    DisjSets(int numElements) {
      array = std::vector<int>(numElements);
      for(int i = 0; i < array.size(); i++) array[i] = -1;
    }

    /** Perform a find with path compression
     *  @param x the label of the element being search for.
     *  @return the label of the set containing x.
     */
    int find(int x);

    /** Union two disjoint sets using the height heuristic.
     *  For simplicity, we assume root1 and root2 are distinct and represent
     *  set labels.
     *  @param root1 the root of set 1.
     *  @param root2 the root of set 2.
     *  @return the root of the union.
     */
    int unionByHeight(int root1, int root2);

};

#endif // DISJSETS_HPP
