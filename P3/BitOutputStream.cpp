#include "BitOutputStream.hpp"

/** 
 * Author: Kevin Yang
 * Login name: kjy002
 * Description: BitOutputStream.cpp implements all the methods in the header
 * file BitOutputStream.hpp.
 */

/** Send the buffer to the output, and clear it.
 */
void BitOutputStream::flush() {
  out.put(buf);
  out.flush();
  buf = nbits = 0;
}

/** Write the least significant bit of the argument to the bit buffer, and 
 *  increment the bit buffer index. But flush the buffer first, if it's
 *  full.
 */
void BitOutputStream::writeBit(int i) {

  // Flush the bit buffer if full.
  if(nbits == 8) flush();

  // Write the least significant bit of i into the buffer at the current index.
  buf |= (i << (7 - nbits));

  // Increment the index.
  ++nbits;

}

/** Write the integer to the output file.
 */
void BitOutputStream::writeInt(int j) {
  for(int i = 31; i >= 0; i--) writeBit((j >> i) & 1);
}

/** Write the character to the output file. 
 */
void BitOutputStream::writeChar(char ch) {
  for(int i = 7; i >= 0; i--) writeBit((ch >> i) & 1);
}
