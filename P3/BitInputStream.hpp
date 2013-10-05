#ifndef BITINPUTSTREAM_HPP
#define BITINPUTSTREAM_HPP

#include <iostream>

using namespace std;

class BitInputStream {

/**
 * Author: Kevin Yang
 * Login name: kjy002
 *
 * Description: All disk I/O operations deal with a byte as the smallest unit
 * of storage. But in this assignment, it would be convenient to have an API
 * to the filesystem that permits writing and reading one bit at the time. 
 * BitInputStream allows us to read one bit at a time.
 */

private:

  /* One byte buffer of bits */
  char buf;

  /* How many bits have been read from buf */
  int nbits;

  /* The input stream to use */
  istream & in;

public:

  /** Initialize a BitInputStream that will use the given istream for input.
   */
  BitInputStream(istream & is) : in(is) {
    buf = 0;
    nbits = 8;
  }

  /** Fill the buffer from the input */
  void fill();

  /** Read the next bit from the bit buffer.
   *  Fill the buffer from the input stream first if needed.
   *  Return 1 if the bit read is 1;
   *  Return 0 if the bit read is 0.
   */
  int readBit();

  /** Read the ints from the header */
  int readInt();

  /** Read the chars from the header */
  int readChar();

};

#endif // BITINPUTSTREAM_HPP
