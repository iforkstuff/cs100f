#ifndef BITOUTPUTSTREAM_HPP
#define BITOUTPUTSTREAM_HPP

#include <iostream>

using namespace std;

class BitOutputStream {

/**
 * Author: Kevin Yang
 * Login name: kjy002
 *
 * Description: All disk I/O operations deal with a byte as the smallest unit
 * of storage. But in this assignment, it would be convenient to have an API
 * to the filesystem that permits writing and reading one bit at the time. 
 * BitOutputStream allows us to write one bit at a time.
 */

private:

  /* one bye buffer of bits */
  char buf;

  /* how many bits have been written to buf */
  int nbits;

  /* The output stream to use */
  ostream & out;

public:

  /** Initialize a BitOutputStream that will use the given ostream for output.
   */
  BitOutputStream(ostream & os) : out(os) {
    buf = nbits = 0;
  }

  /** Send the buffer to the output, and clear it.
   */
  void flush();

  /** Write the least significant bit of the argument to the bit buffer, and 
   *  increment the bit buffer index. But flush the buffer first, if it's
   *  full.
   */
  void writeBit(int i);

  /** Write the Integer into the output file. 
   */
  void writeInt(int i);

  /** Write the character into the output file. 
   */
  void writeChar(char ch);

};

#endif // BITOUTPUTSTREAM_HPP
