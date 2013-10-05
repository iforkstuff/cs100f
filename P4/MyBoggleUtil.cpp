#include "MyBoggleUtil.h"
#include "MyBogglePlayer.h"
#include <iostream>


/**
 * MyBoggleUtil Class.
 * P4 CSE 100 2010
 * Author: Qihua Wu--cs100fbz & 
 *         Kevin Yang--kjy002
 * (copyright) 2010
 */

using namespace std;


void LEXTrie::makeEmpty(LEXNode* currRoot){
  if(currRoot != 0){
    makeEmpty(currRoot->left);
    makeEmpty(currRoot->middle);
    makeEmpty(currRoot->right);
    delete currRoot;
    currRoot = 0;
  }
}



void LEXTrie::reset(LEXNode* currRoot){
  if(currRoot != 0){
    reset(currRoot->left);
    reset(currRoot->middle);
    reset(currRoot->right);
    currRoot->isInValidList = false;
  }
}


int LEXTrie::isPrefix(string word){
  LEXNode* currNode = root;
  int length =  word.length();	
	
  // Start from the first letter of the string.
  for (int i = 0; i < length ; i++) {
    // Looking for the letter at the i th position of the string. If the letter
    // is larger than the letter at the current node we're comparing, then we
    // know that the letter should be in the right subtrie. Else left.
    // Nevertheless, if we somehow point at an empty node, then we know that
    // the word we're looking for doesn't exist, so return false.
    while (currNode->letter != word[i]){
      if(word[i] > currNode->letter){
	currNode = currNode->right;
	if (currNode == 0) return 0;	
      }
      else {
	currNode = currNode->left;
	if (currNode == 0) return 0;
      }
    }
    // Letter at the ith position found. So let's continue looking for the i+1
    // letter in the trie.
    if (i < length-1) {
      currNode = currNode->middle;
      if (currNode == 0) return 0;			
    }
		
    // At the last index of the string, and also just found the last letter in
    // the trie. If the current node we're pointing at contains the "true"
    // flag, then the words exists, so return true. Else false.
    else {
      if(currNode->isWord){
	if(currNode->isInValidList == false){
	  currNode->isInValidList = true;
	  return 2;
	}
        else return 1;
      }
      else return 1;
    }
  }
}
  


/** Check if a word is in the lexicon */
bool LEXTrie::IfHaveWord(string word){
	
  // For each time, we start by searching from the root of the trie.
  LEXNode* currNode = root;
	
  int length =  word.length();
	
  // Start from the first letter of the string.
  for (int i = 0; i < length ; i++) {
		
    // Looking for the letter at the i th position of the string. If the letter
    // is larger than the letter at the current node we're comparing, then we
    // know that the letter should be in the right subtrie. Else left.
    // Nevertheless, if we somehow point at an empty node, then we know that
    // the word we're looking for doesn't exist, so return false.
    while (currNode->letter != word[i]){
      if(word[i] > currNode->letter){
	currNode = currNode->right;
	if (currNode == 0) return false;
      }
      else{
	currNode = currNode->left;
	if (currNode == 0) return false;
      }
    }
    // Letter at the ith position found. So let's continue looking for the i+1
    // letter in the trie.
    if (i < length-1){
      currNode = currNode->middle;
      if (currNode == 0) return false;			
    }	
		
    // At the last index of the string, and also just found the last letter in
    // the trie. If the current node we're pointing at contains the "true"
    // flag, then the words exists, so return true. Else false.
    else {
      if (currNode->isWord) return true;
      else return false;			
    }
  }	
}

/** Add one word into the lexicon. */
void LEXTrie::addWord(string word){	
  // For each time, if we want to insert a new word, we start by comparing 
  // the first letter of the new word with the root.	
  LEXNode* currNode = root; 
  int length = word.length();	
  // Add the word letter by letter.	
  for (int i = 0;i < length;i++) {
    // Letter that's not the last index of the word shouldn't have the true    
    // flag because in most cases "they're not genuine English words."	
    if (i < length-1 ) currNode = addLetter(word[i], false, currNode);
    else currNode = addLetter(word[i], true, currNode);
  }
}

/** Add one letter into the lexicon, or the ternary trie */
LEXNode* LEXTrie::addLetter(char ch, bool flag, LEXNode* node){
	
  // End of recursion. If the node doesn't contain any letter, then we can add  
  // the letter to it, and set its flag.
  if (node->letter == 0) {		
    node->letter = ch;		
    node->isWord = flag;	
    if(flag == false) node->middle = new LEXNode(0,false,false,0,0,0);
    return node->middle;
  }
	
	
  // Recursive case. If the letter we want to add is smaller than the letter in	
  // the current node we're at, then the letter we want to add should be in the	
  // left subtrie. Else it should be in the right subtrie. Note that once we
  // reach the leaves, we add a new child to it (for the new letter), and move  
  // to the end of recursion. 	
  else if (ch < node->letter) {
    if (!node->left) {
      node->left = new LEXNode(0,false,false,0,0,0);
      return addLetter(ch,flag,node->left);
    }
    else return addLetter(ch,flag,node->left);		
  }
  else if (ch > node->letter){	
    if (!node->right) {			
      node->right = new LEXNode(0,false,false,0,0,0);		
      return addLetter(ch,flag,node->right);		
    }
    else return addLetter(ch,flag,node->right);
  }
	
  // If the flag is true, then we have an English word. Moreover, if the middle	
  // child is empty, then this will be the node we add letter to it. Else	
  // return the middle node.
  else {
    if (flag) node->isWord = true;
    if (!node->middle) { 
      node->middle = new LEXNode(0,false,false,0,0,0);	
      return node->middle;
    }
    else return node->middle;
  }	
}

bool BoardStructure::ifHaveWord(const string& word_to_check, int boardIndex, vector<int>* locations, int wordIndex){
	
  // Negative boardIndex doesn't exist, and boardIndex >= totalSize
  // Moreover, according to the rules of Boggle game, if a die is used once, 
  // you can't use it anymore.
  if (boardIndex < 0 ) return false;
  if (boardIndex >= totalSize) return false;
  if (isVisited[boardIndex]) return false;
	
  // the Length of the string appeared on the die namely "board[boardIndex]"
  int lengthOfTheNodeLetters = board[boardIndex].length();	
  for (int i = 0; i < lengthOfTheNodeLetters; i++, wordIndex++) {
    // the word_to_check has end but the string in the board node doesn't end, return false
    if (wordIndex >= word_to_check.length()) {
      return false;
    }
		
    // The letter the gamer types doesn't equal to the letter appeared on the
    // die board[boardIndex].
    if(word_to_check[wordIndex] != board[boardIndex][i]) {
      return false;
    }
  }
	
  // So we have checked the die, so you can't visit this die in this interation anymore
  isVisited[boardIndex] = true;
	
	
  // if it can reaches here, means the board node with boardIndex might be part of the word
  locations->push_back(boardIndex);

  if(wordIndex == word_to_check.length()) {
    return true;
  }
	
  // Checking the boards around board[boardIndex]. If everything around it is
  // false, then this means that the word the gamer types in still doesn't 
  // exist, so return false.
	
  if ((boardIndex%width) == 0) {
    if (ifHaveWord(word_to_check, boardIndex-width,locations,wordIndex)) return true;
    if (ifHaveWord(word_to_check, boardIndex-width+1,locations,wordIndex)) return true;		
    if (ifHaveWord(word_to_check, boardIndex+1,locations,wordIndex)) return true;
    if (ifHaveWord(word_to_check, boardIndex+width,locations,wordIndex)) return true;
    if (ifHaveWord(word_to_check, boardIndex+width+1,locations,wordIndex)) return true;
  }	
  else if ((boardIndex%width) == (width - 1)) {
    if (ifHaveWord(word_to_check, boardIndex-1-width,locations,wordIndex)) return true;
    if (ifHaveWord(word_to_check, boardIndex-width,locations,wordIndex)) return true;
    if (ifHaveWord(word_to_check, boardIndex-1,locations,wordIndex)) return true;
    if (ifHaveWord(word_to_check, boardIndex+width,locations,wordIndex)) return true;
    if (ifHaveWord(word_to_check, boardIndex+width-1,locations,wordIndex)) return true;
  }
  else {
    if (ifHaveWord(word_to_check, boardIndex-1-width,locations,wordIndex)) return true;
    if (ifHaveWord(word_to_check, boardIndex-width,locations,wordIndex)) return true;
    if (ifHaveWord(word_to_check, boardIndex-width+1,locations,wordIndex)) return true;
    if (ifHaveWord(word_to_check, boardIndex-1,locations,wordIndex)) return true;
    if (ifHaveWord(word_to_check, boardIndex+1,locations,wordIndex)) return true;
    if (ifHaveWord(word_to_check, boardIndex+width,locations,wordIndex)) return true;
    if (ifHaveWord(word_to_check, boardIndex+width+1,locations,wordIndex)) return true;
    if (ifHaveWord(word_to_check, boardIndex+width-1,locations,wordIndex)) return true;
  }
  isVisited[boardIndex] = false;
  locations->pop_back();
  return false;
}


/** A helper function for setting up the board. First, we check that all the 
*  preconditions are satisfied. Once we checked them, we can start building
*  the board. While we're constructing the boggle board, we check that every
*  element in new_board is A-Z or a-z. 
*/
bool BoardStructure::buildBoard(const vector<string>& new_board,int width){
	
  // The totalSize == 0, smaller than the width, it's not a board.	
  totalSize = new_board.size();
  //	this->width = width;
  if (totalSize == 0) return false;
  if (totalSize < width) return false;
  if (totalSize%width != 0) return false;
	
  // Starting building the board.	
  board = vector<string>(totalSize,"");	
  isVisited = vector<bool>(totalSize,false);	
  for (int i = 0;i < totalSize; i++) {		
    for (int j = 0;j < new_board[i].length(); j++) {			
      if(new_board[i][j] >= 'A'&& new_board[i][j] <= 'Z') {		
	board[i] += (new_board[i][j]+32);
      }
      else if (new_board[i][j] >= 'a'&&new_board[i][j] <= 'z') {
	board[i] += new_board[i][j];
      }
      else return false;
    }
  }
  return true;
}




void BoardStructure::explore(int boardIndex,vector<string>* words, LEXTrie* trie){
  if(boardIndex < 0){
    return;
  }
  if(boardIndex >=totalSize){
    return;
  }
  if(isVisited[boardIndex]){
    return;
  }
  currWord += board[boardIndex];
  int size = currWord.length();
  int allOrPart = trie->isPrefix(currWord);
  if(allOrPart != 0){
    isVisited[boardIndex] = true;
    if((size >= minWordLength) && (allOrPart == 2)){
      words->push_back(currWord);
    }
    if(boardIndex%width == 0){
      explore(boardIndex-width,words,trie);
      explore(boardIndex-width+1,words,trie);
      explore(boardIndex+1,words,trie);
      explore(boardIndex+width,words,trie);
      explore(boardIndex+width+1,words,trie);
    }
    else if(boardIndex%width == width-1){
      explore(boardIndex-width-1,words,trie);
      explore(boardIndex-width,words,trie);
      explore(boardIndex-1,words,trie);
      explore(boardIndex+width-1,words,trie);
      explore(boardIndex+width,words,trie);
    }
    else{
      explore(boardIndex-width-1,words,trie);
      explore(boardIndex-width,words,trie);
      explore(boardIndex-width+1,words,trie);
      explore(boardIndex-1,words,trie);
      explore(boardIndex+1,words,trie);
      explore(boardIndex+width-1,words,trie);
      explore(boardIndex+width,words,trie);
      explore(boardIndex+width+1,words,trie);
    }
    currWord.resize(size-board[boardIndex].length());
    isVisited[boardIndex] = false;
    return;
  }else {
    currWord.resize(size-board[boardIndex].length());
    return;
  }	
}
