#include "RST.hpp"
#include "countint.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <math.h>
#include <cstring>
#include <cstdlib>

using namespace std;

/**
 * Author: Kevin Yang
 * Login name: kjy002
 * Description: This program measures the performance of successful finds for 
 * the data structures, bst, rst, set, when the elements are sorted and 
 * shuffled. The program then prints out the total avg comparisons and sdev
 */

int main(int argc, char *argv[]) {
  
  int N = 0;
  int R = 0;

  int first = 0;
  int second = 0;

  double avgcomps = 0;
  double total = 0;
  double sum = 0;
  double square = 0;
  double sdev = 0;
  double squares = 0;

  // Check the user type everything in the correct format, before the benchmark
  if (argc == 5) {
    if (!strcmp(argv[1],"bst")) first = 1;
    if (!strcmp(argv[1],"rst")) first = 2;
    if (!strcmp(argv[1],"set")) first = 3;
    if (!strcmp(argv[2],"sorted")) second = 1;
    if (!strcmp(argv[2],"shuffled")) second = 2;
    if (argv[3] != 0) N = atoi(argv[3]);
    if (argv[4] != 0) R = atoi(argv[4]);
  }

  // If the format is incorrect, then terminate the program.
  if ((first == 0) || (second == 0) || (N == 0) || (R == 0)) {
    cout << "usage..." << endl;
    return -1;
  }

  cout << "# Benchmarking average number of comparisons for successful find" 
	<< endl;
  cout << "# Data structure: " << argv[1] << endl;
  cout << "# Data: " << argv[2] << endl;
  cout << "# N is a powers of 2, minus 1, from 1 to " << N << endl;
  cout << "# Averaging over " << R << " for each N" << endl;
  cout << "#" << endl;
  cout << "# N\tavgcomps\tsdev" << endl;

  vector<countint> v;

  vector<countint>::iterator vit;
  vector<countint>::iterator ven;

  // Begin the benchmark. The first loop is for all j in 1,2,... N = 2^j - 1
  for (int i = 1; i <= N; i+=i+1) {
    srand(time(0));

    // For R in 1,2,..., number of runs
    for (int j = 0; j < R; ++j) {

      // Every run should use new data structures.
      BST<countint> bst = BST<countint>();
      RST<countint> rst = RST<countint>();
      set<countint> s = set<countint>();
      v.clear();

      // Perform the insert of vector
      for (int k = 0; k < i; ++k) v.push_back(k);

      // Shuffle the elements if the user input "shuffle" in argv[2]
      if (second == 2) random_shuffle(v.begin(),v.end());

      vit = v.begin();
      ven = v.end();

      // Insert all elements in bst, rst, or set
      for (vit = v.begin(); vit != ven; ++vit) {
        if (first == 1) bst.insert(*vit);
        if (first == 2) rst.insert(*vit);
        if (first == 3) s.insert(*vit);
      }

      countint::clearcount();

      // Now perform find, and record average # comparisons per successful find
      for (vit = v.begin(); vit != ven; ++vit) {
        if (first == 1) bst.find(*vit);
        if (first == 2) rst.find(*vit);
        if (first == 3) s.find(*vit);
      }
      avgcomps = countint::getcount()/((double)i);
      sum += avgcomps;
      square += avgcomps*avgcomps;

    }
 
    // Record the datas, and print out the total, and the standard deviation
    total = sum/R;
    squares = square/R;
    sdev = sqrt(fabs(squares - (total*total)));
    cout << i << "\t" << total << "\t" << sdev << endl;

    // Also erase the sum and the square
    sum = 0;
    square = 0;

  }

  return 0;
}
