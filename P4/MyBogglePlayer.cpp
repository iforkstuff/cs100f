#include "MyBogglePlayer.h"
#include "MyBoggleUtil.h"
#include <iostream>


/**
 * MyBogglePlayer Class.
 * P4 CSE 100 2010
 * Author: Qihua Wu--cs100fbz & 
 *         Kevin Yang--kjy002
 * (copyright) 2010
 */


/** Given a minimum word length, populate a vector of strings with all words
 *  of a t least that length on the board that are also in the lexicon
 *  minimum_word_length: the minimum word length to use.
 *  words:       a vector that will contain all valid words.
 *  returns:     false if either BuildLexicon() or SetBoard() have not yet
 *  been called; true otherwise.
 */
bool MyBogglePlayer::GetAllValidWords(int minimum_word_length, 
                                      vector<string>* words) {
  if ((!BoardBuilt) || (!LexBuilt)) {
	  return false;
  }
  myBoard->minWordLength = minimum_word_length;

  for (int i = 0; i < myBoard->totalSize; i++) {
    myBoard->isVisited = vector<bool>(myBoard->totalSize,false);
    myBoard->currWord = "";
    myBoard->explore(i,words,myTrie);
  }
  myTrie->reset(myTrie->root);
  if(words->size() == 0) return false;
  else return true;
}

bool MyBogglePlayer::GetCustomBoard(vector<string>* new_board, int* width) {

  new_board->clear();

  const char* strs[] = { 
    "c","f","a","j","g","h","f","d","b","f","d","j","i","c","g","f","c","a","j","d","b","g","e",
    "g","j","e","h","a","h","i","i","g","a","g","f","b","f","j","b","b","a","e","a","h","e","g",
    "b","e","f","g","b","c","b","j","d","i","a","f","j","d","d","e","f","f","d","i","c","e","j",
    "f","j","a","f","a","b","d","a","f","i","f","e","a","g","b","g","i","h","d","c","d","i","c",
    "c","g","h","a","c","c","b","e","a","f","c","j","e","b","a","b","g","a","c","d","j","g","j",
    "f","d","j","f","g","h","c","j","h","b","j","c","i","b","b","f","e","h","d","h","d","h","h",
    "i","d","b","b","d","b","a","g","i","e","j","a","i","i","i","j","e","a","f","a","d","e","e",
    "d","d","c","g","e","c","g","e","i","c","b","d","d","i","g","h","d","b","c","f","h","d","i",
    "g","e","h","d","g","j","c","g","a","i","a","c","g","b","e","h","a","d","f","j","h","j","a",
    "b","c","e","j","d","a","g","a","j","f","g","a","i","a","d","g","f","h","g","e","g","b","i",
    "g","c","d","c","b","j","f","b","j","b","b","f","g","a","g","j","e","c","c","e","a","d","a",
    "e","c","d","g","i","h","c","b","h","a","c","b","h","h","f","d","b","i","b","h","a","c","g",
    "c","f","c","a","j","i","i","a","i","d","c","h","h","b","f","a","f","a","h","b","e","f","d",
    "a","i","c","c","a","g","g","f","j","c","f","i","e","i","e","b","d","b","b","f","f","d","f",
    "d","i","j","b","i","i","e","c","e","b","g","f","e","h","c","h","e","j","e","b","d","b","i",
    "c","b","j","i","e","c","g","b","a","d","a","i","e","h","j","i","g","e","a","c","a","e","j",
    "g","d","i","j","i","b","g","a","f","j","d","d","c","i","c","b","j","f","b","g","e","g","h",
    "d","c","g","h","g","e","i","j","j","a","f","d","c","e","h","j","c","d","j","e","d","g","e",
    "d","d","a","g","c","a","g","b","d","a","a","d","i","a","d","a","e","a","d","i","b","g","f",
    "d","d","g","g","a","c","e","i","d","c","g","d","f","g","e","b","a","f","h","a","i","b","i"
  };
  const int width1 = 23;
  const int height = 20;
 
  std::vector<string> board;
  for(int i = 0; i < width1*height; ++i) new_board->push_back(string(strs[i]));
  *width = width1;
}

/** Given a word, populate a List of Integers specifying the location
*  of dice on the board that can be used to form the word. For details
*  of restrictions that a path of locations must meet, and how
*  locations are coded as integers, see the assignment README.
*  wordToCheck: a string specifying the word to find.
*  locations:   a vector<int> specifying locations of dice on the board
*               that spell the word.
*  returns:     true if the word is on the board, false if it is not.
*/
bool MyBogglePlayer::IsOnBoard(const string& word_to_check,
                               vector<int>* locations) {
  myBoard->isVisited = vector<bool>(myBoard->totalSize, false);
  if (!BoardBuilt) return false;
  for (int i = 0;i < myBoard->totalSize; i++) {
    myBoard->isVisited = vector<bool>(myBoard->totalSize, false);
    if (myBoard->ifHaveWord(word_to_check, i, locations, 0)) return true;
  }
  return false;
}

void MyBogglePlayer::BuildLexicon(const vector<string> & word_list) {
	
  // Get word one by one, and put them into the data structure.
  for(int i = 0; i < word_list.size(); i++) myTrie->addWord(word_list[i]);
	
  // Lexicon is now built. 
  LexBuilt = true;
	
}

/** Check if a given word is in the lexicon.
*  word_to_check: the word to check.
*  returns: true if the word is in the lexicon, false if it is not, or
*  if BuildLexicon has not yet been called.
*/
bool MyBogglePlayer::IsInLexicon(const string& word_to_check) {
  if(!LexBuilt) return false;
  return myTrie->IfHaveWord(word_to_check);
}

/** Set the board for the game to show die faces with those strings on them.
*  Each string in the board may be upper or lower case, and may contain one
*  or more letters; for purposes of finding words on the board, they should
*  all be considered lowercase.
*  new_board:   A one-dimensional vector of std::strings for the board.
*               Elements [0...width - 1] should contain the top row of the
*               board; elements [width...(2 * width) - 1] should contain the
*               second row, and so on. In general, the element at (x, y)
*               should be found at (x + y * width).
*/
bool MyBogglePlayer::SetBoard(const vector<string>& new_board, int width) {
	
  // If the board is built successfully, then we return true. Else false.
  myBoard->width = width;
  if(myBoard->buildBoard(new_board, width) == true){
    BoardBuilt = true;
    return true;
  }
  else return false;
}

