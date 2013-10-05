#include "DisjSets.hpp"

/**
 * @author Kevin Yang
 * Implementation class for the DisjSet interface.
 */

/** Perform a find with path compression
 *  @param x the label of the element being search for.
 *  @return the label of the set containing x.
 */
int DisjSets::find(int x) {
  if(x < 0 || x >= array.size()) return -1;
  if(array[x] < 0) return x;
  else return find(array[x]);
}

/** Union two disjoint sets using the height heuristic.
 *  For simplicity, we assume root1 and root2 are distinct and represent
 *  set labels.
 *  @param root1 the root of set 1.
 *  @param root2 the root of set 2.
 *  @return the root of the union.
 */
int DisjSets::unionByHeight(int root1, int root2) {

  // root2 is higher.
  if(array[root2] < array[root1]) {
    
    // Make root2 a new root.
    array[root1] = root2;

    return root2;
  }

  else {

    // Tie-breaking. Decrease height of root1 if they're equal.
    if(array[root1] == array[root2]) array[root1]--;

    // Make root1 a new root.
    array[root2] = root1;

    return root1;
  }
}
