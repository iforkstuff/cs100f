#ifndef MY_BOGGLE_PLAYER_H
#define MY_BOGGLE_PLAYER_H

#include "BogglePlayer.h"
#include "MyBoggleUtil.h"
#include <vector>
#include <string>

using namespace std;

/**
 * MyBogglePlayer Interface.
 * P4 CSE 100 2010
 * Author: Qihua Wu--cs100fbz & 
 *         Kevin Yang--kjy002
 * (copyright) 2010
 */


/** Interface for a Boggle-playing entity. It only contains virtual methods. */
class MyBogglePlayer : public BogglePlayer {
	
 private:	
  // A ternary trie for constructing the Lexicon.
  LEXTrie *myTrie;
	
  // For board construction.
  BoardStructure *myBoard;
	
  // Determine whether the Lexicon and the board have been built properly.
  bool LexBuilt, BoardBuilt;
	
 public:	
	/** Public constructor for the Boggle game. */
  MyBogglePlayer() {
    myTrie = new LEXTrie();
    myBoard = new BoardStructure();
    BoardBuilt = LexBuilt = false;
  }
	
	/** Destructor for the Boggle game. */
  ~MyBogglePlayer() {
    delete myTrie;
    myTrie = 0;
    delete myBoard;
    myBoard = 0;
  }
	
	
  /** Given a vector of strings, construct the lexicon to use in the game.
   *  The vector contains the words which will appear in the lexicon.
   *  The words must consist of lowercase letters a-z only.
   *  wordList:    a vector of strings containing the words to use.
   *  POST condition: 	LEXTrie* myTrie is the trie which contains all the word
   *  in the dictionary.
   */
  virtual void BuildLexicon(const vector<string>& word_list);
	
  /** Set the board for the game to show die faces with those strings on them.
   *  Each string in the board may be upper or lower case, and may contain one
   *  or more letters; for purposes of finding words on the board, they should
   *  all be considered lowercase.
   *
   *  new_board:   A one-dimensional vector of std::strings for the board.
   *               Elements [0...width - 1] should contain the top row of the
   *               board; elements [width...(2 * width) - 1] should contain the
   *               second row, and so on. In general, the element at (x, y)
   *               should be found at (x + y * width).
   */
  virtual bool SetBoard(const vector<string>& new_board, int width);
	
  /** Given a minimum word length, populate a vector of strings with all words
   *  of at least that length on the board that are also in the lexicon.
   *  minimum_word_length: the minimum word length to use.
   *
   *  words:       a vector that will contain all valid words.
   *  returns:     false if either BuildLexicon() or SetBoard() have not yet
   *               been called; true otherwise.
   */
  virtual bool GetAllValidWords(int minimum_word_length, vector<string>* words);
	
  /** Check if a given word is in the lexicon.
   *  word_to_check: the word to check.
   *
   *  returns:     true if the word is in the lexicon, false if it is not, or
   *               if BuildLexicon has not yet been called.
   */
  virtual bool IsInLexicon(const string& word_to_check);
	
  /** Given a word, populate a List of Integers specifying the location
   *  of dice on the board that can be used to form the word.
   *  wordToCheck: a string specifying the word to find.
   *  locations:   a vector<int> specifying locations of dice on the board
   *               that spell the word.
   *  returns:     true if the word is on the board, false if it is not.
   */
  virtual bool IsOnBoard(const string& word_to_check, vector<int>* locations);
	
  /** Populates, in a form suitable as an argument to SetBoard(), a
   *  specification of a Boggle board. Mainly useful as a way to provide a
   *  specific, non-random board for testing.
   *
   *  new_board:   a vector of strings to populate with the new board.
   *  width:       a pointer to an integer to populate with the board's width.
   *  returns:     false if any of the pointers were NULL; true otherwise.
   */
  virtual bool GetCustomBoard(vector<string>* new_board, int* width);
	
};

#endif // MYBOGGLEPLAYER_H
