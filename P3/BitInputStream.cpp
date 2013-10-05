#include "BitInputStream.hpp"

/**
 * Author: Kevin Yang
 * Login name: kjy002
 *
 * Description: BitInputStream.cpp implements every method in the header file
 * BitInputStream.hpp.
 */

/** Fill the buffer from the input */
void BitInputStream::fill() {
  buf = in.get();
  nbits = 0;
}

/** Read the next bit from the bit buffer.
 *  Fill the buffer from the input stream first if needed.
 *  Return 1 if the bit read is 1;
 *  Return 0 if the bit read is 0.
 */
int BitInputStream::readBit() {

  // If all bits in the buffer are read, fill it buffer first
  if (nbits == 8) fill();

  // Get the bit at the approriate location in the bit buffer, and return the 
  // approriate int. Also, increment the index.
  ++nbits;
  return ((buf >> (8-nbits)) & 1);

}

/** Read the ints from the header */
int BitInputStream::readInt() {
  int sum = 0;
  for(int i = 31; i >= 0; i--) sum = (sum*2) + readBit();
  return sum;
}

/** Read the chars from the header */
int BitInputStream::readChar() {
  int sum = 0;
  for(int i = 7; i>=0; i--) sum = (sum*2) + readBit();
  return sum;
}
