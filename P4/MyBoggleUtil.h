#ifndef MY_BOGGLE_UTIL_H
#define MY_BOGGLE_UTIL_H

#include <vector>
#include <string>

/**
 * MyBoggleUtil Interface.
 * P4 CSE 100 2010
 * Author: Qihua Wu--cs100fbz & 
 *         Kevin Yang--kjy002
 * (copyright) 2010
 */

using namespace std;

/** Class that defines how each node is structured in a ternary trie */
class LEXNode{
 public:
	
  char letter;
  bool isInValidList;	
  // Flag... True indicates that a string ends at this node is a valid word.
  bool isWord; 
	
  // Left, right, and middle child pointers.
  LEXNode * left;
  LEXNode * right;
  LEXNode * middle;

	
  // Public constructor for every node in the ternary trie.
 LEXNode(char letter, bool isInValidList = false,
	 bool isWord = false,
	 LEXNode * left = 0,
	 LEXNode * right = 0,
	 LEXNode * middle = 0)
   :letter(letter),isInValidList(isInValidList), isWord(isWord),
    left(left), right(right), middle(middle) {}


};



/** Class that defines a ternary trie */
class LEXTrie{
 private:	
  /** Helper function: Add the letter one by one */
  LEXNode* addLetter(char ch, bool flag, LEXNode* node);

  void makeEmpty(LEXNode* currRoot);
	
 public:
	
  // Root of the trie. Root == 0 means the tree is empty.
  LEXNode* root;
  int minWordLength; // Minimum length of an English word
	
  /** Public constructor for the ternary trie */
 LEXTrie() : root(0) {
    root = new LEXNode(0,false, false , 0, 0, 0);
    minWordLength = 0;
  }
	
  /** Destructor for the ternary trie */
  ~LEXTrie(){
    makeEmpty(root);
    delete root;
    root = 0;
  }
	
  /** Add one word into the lexicon */
  void addWord(string word);

  /** helper function for the destructor*/
  void makeEmpty();
	
  /** check if a word is in the lexicon */
  bool IfHaveWord(string word);

  int isPrefix(string word);

  void reset(LEXNode* currRoot);
};




class BoardStructure{
 public:
  vector<string> board;
  string currWord;
  int minWordLength;
  vector<bool> isVisited;
  int width;
  void explore(int boardIndex,vector<string>* words , LEXTrie* trie);
  // Total number of the dices on the Boggle board.
  int totalSize;
	
  /** Public constructor for the Boggle board. */
  BoardStructure() {
    currWord = "";
    width = 0;
    totalSize = 0;
    minWordLength = 0;
  }	
  /** A helper function for setting up the board. First, we check that all the 
   *  preconditions are satisfied. Once we checked them, we can start building
   *  the board. While we're constructing the boggle board, we check that every
   *  element in new_board is A-Z or a-z. 
   */
  bool buildBoard(const vector<string>& new_board,int width);
	
	
  bool ifHaveWord(const string& word_to_check, int boardIndex, 
		  vector<int>* locations, int wordIndex);
};

#endif // MYBOGGLEUTIL_H
