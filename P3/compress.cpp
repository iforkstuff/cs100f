#include <fstream>
#include "HCTree.hpp"

using namespace std;

/**
 * Author: Kevin Yang
 * Login name: kjy002
 * Description: When run, this program will read the contents of the file named
 * by its first command line argument, construct a Huffman code for the 
 * contents of that file, and use that code to construct a compressed version 
 * which is written to a file named by the second command line argument. The 
 * input file can contain any data (not just ASCII characters) so it should be
 * treated as a binary file. The input file can be up to 100 megabytes in size
 * (so the same byte value may occur up to 100 million times in the file). 
 */

int main(int argc, char* argv[]) {
  ifstream in;
  ofstream out;

  // Program exits if the user inputs invalid command.
  if(argc != 3) {
    cout << "Invalid command!" << endl;
    return -1;
  }

  // Open the input file for reading. Terminates the program if opening fails.
  in.open(argv[1],ios::binary);
  if(!in) {
    cout << "Failed to open " << argv[1] << ". Program will now exit" << endl;
    return -1;
  }

  byte ch;
  vector<int> freqs = vector<int>(256,0);

  // Read bytes from the file, counting the number of occurences of each byte
  // value; then close the file
  while(1) {
    ch = in.get();
    if(!in.good()) break;
    freqs[ch]++;
  }

  // Exit the program if the loop stops for some bad reason. Or close the file.
  if(!in.eof()) {
    cerr << "There was a problem, sorry." << endl;
    return -1;
  }
  in.close();

  // Use these byte counts to construct a Huffman coding tree.
  HCTree *tree = new HCTree();
  tree->build(freqs);

  // Open the output file for writing.
  out.open(argv[2],ios::binary);

  // Write enough information (a "file header") to the output file to enable 
  // the coding tree to be reconstructed when the file is read by your 
  // uncompress program.
  int distinct_symbols = -1;
  BitOutputStream output(out);
  for(int i = 0; i < 256; i++) if(freqs[i] != 0) distinct_symbols++;
  output.writeChar(distinct_symbols);
  for(int i = 0; i < 256; i++) {
    if(freqs[i] != 0) {
      output.writeChar((byte)i);
      output.writeInt((int)freqs[i]);
    }
  }

  // Open the input file for reading, again.
  in.open(argv[1],ios::binary);

  // Using the Huffman coding tree, translate each byte from the input file 
  // into its code, and append these codes as a sequence of bits to the output 
  // file, after the header.
  while(1) {
    ch = in.get();
    if(!in.good()) break;
    tree->encode(ch,output);
  }
  output.flush();

  // Close the program if the loop stops for some bad reason.
  if(!in.eof()) {
    cerr << "There was a problem, sorry!" << endl;
    return -1;
  }

  // Close the input and output files
  in.close();
  out.close();

  delete tree;
  tree = 0;

  return 0;
}

