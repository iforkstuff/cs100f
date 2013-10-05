#include "HCTree.hpp"
#include <stack>

/** 
 * Author: Kevin Yang
 * Login name: kjy002
 * Description: HCTree.cpp implements every method specified in the header file
 * HCTree.hpp
 */

HCTree::~HCTree() {
  makeEmpty(root);
  root = 0;
}

/** Use the Huffman algorithm to build a Huffman coding trie.
 *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is 
 *  the frequency of occurrence of byte i in the message.
 *  POSTCONDITION:  root points to the root of the trie,
 *  and leaves[i] points to the leaf node containing byte i.
 */
void HCTree::build(const vector<int>& freqs) {

  priority_queue<HCNode*,vector<HCNode*>,HCNodePtrComp> pq;

  // Get the ASCII characters and store them into leaves and pq.
  for(int i = 0; i < 256; ++i) {
    if(freqs[i] != 0) {
      HCNode *pointer = new HCNode(freqs[i],(unsigned char)i,0,0,0);
      leaves[i] = pointer;
      pq.push(leaves[i]);
    }
  }

  while(pq.size() > 1) {

    // Pick the two elements that have the highest priority in pq.
    HCNode *copy1 = pq.top();
    pq.pop();
    HCNode *copy2 = pq.top();
    pq.pop();

    // Compute the frequency of the two HCNodes.
    int frequency = copy1->count + copy2->count;
    int memory = ((int)(copy1->symbol + copy2->symbol))%256;

    // Create an HCNode that has the total frequency of the two HCNodes, and 
    // make it to be their parent node.
    HCNode *pointer = new HCNode(frequency,(byte)memory,copy1,copy2,0);
    if((copy1->count) < (copy2->count)) {
      pointer->c0 = copy1;
      pointer->c1 = copy2;
    }
    else {
      pointer->c0 = copy2;
      pointer->c1 = copy1;
    }
    copy1->p = pointer;
    copy2->p = pointer;

    // Push the parent node to pq for the next priority comparison
    pq.push(pointer);

  }

  // If pq only has one element then HCTree should also has one element, and
  // must have no child, and no parent.
  if(pq.size() == 1) {
    root = pq.top();
    pq.pop();
  }

}

/** Write to the given BitOutputStream
 *  the sequence of bits coding the given symbol.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
void HCTree::encode(byte symbol, BitOutputStream& out) const {
  HCNode *pointer = leaves[symbol];
  std::stack<int> stk;
  while(pointer->p != 0) {
    if(pointer == pointer->p->c0) stk.push(0);
    else stk.push(1);
    pointer = pointer->p;
  }
  while(stk.size() != 0) {
    out.writeBit(stk.top());
    stk.pop();
  }
}

/** Return symbol coded in the next sequence of bits from the stream.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
byte HCTree::decode(BitInputStream& in) const {
  HCNode *pointer = root;
  while((pointer->c0 != 0) && (pointer->c1 != 0)) {
    if(in.readBit() == 0) pointer = pointer->c0;
    else pointer = pointer->c1;
  }
  return (byte)(pointer->symbol);
}

/** A helper method to destruct the tree and kill every node */
void HCTree::makeEmpty(HCNode *&h) {
  if(h != 0) {
    makeEmpty(h->c0);
    makeEmpty(h->c1);
    delete h;
  }
  h = 0;
}



