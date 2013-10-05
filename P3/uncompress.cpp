#include <fstream>
#include "HCTree.hpp"

using namespace std;

/**
 * Author: Kevin Yang
 * Login name: kjy002
 * Description: When run, this program will read the contents of the file named
 * by its first command line argument, which should be a file that has been 
 * created by the compress program. It will use the contents of that file to 
 * reconstruct the original, uncompressed version, which is written to a file 
 * named by the second command line argument.
 */

int main(int argc, char* argv[]) {
  ifstream in;
  ofstream out;

  // If the user inputs invalid command, exit the program immediately.
  if(argc != 3) {
    cout << "Invalid command!" << endl;
    return -1;
  }

  // Open the input for reading. Exit the program if we fail to do so.
  in.open(argv[1],ios::binary);
  if(!in) {
    cout << "Failed to open " << argv[1] << ". Program will now exit" << endl;
    return -1;
  }

  byte ch = 0;
  int actual = 0;
  int sum = 0;

  // Read the file header at the beginning of the input file, and reconstruct
  // the Huffman coding tree.
  BitInputStream input(in);
  vector<int> freqs = vector<int>(256,0);
  int distinct_symbols = input.readChar();
  while(distinct_symbols-- != -1) {
    ch = input.readChar();
    actual = input.readInt();
    freqs[ch] = actual;
    sum += actual;
  }
  HCTree *tree = new HCTree();
  tree->build(freqs); 

  // Open the output file for writing.
  out.open(argv[2],ios::binary);

  // Using the Huffman coding tree, decode the bits from the input file into 
  // the appropriate sequence of bytes, writing them to the output file.
  for(int i = 0; i < sum; i++) out.put((byte)tree->decode(input));

  // Close the input and the output file.
  in.close();
  out.close();

  delete tree;
  tree = 0;

  return 0;
}

