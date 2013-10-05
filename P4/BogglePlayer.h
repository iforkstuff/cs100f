#ifndef BOGGLE_PLAYER_H
#define BOGGLE_PLAYER_H

#include <vector>
#include <string>
using std::vector;
using std::string;

// This is an interface for a Boggle-playing entity. It contains only pure
// virtual methods. Any class that derives from this interface must implement
// all the pure virtual methods.

class BogglePlayer {
public:

    // Given a vector of strings, construct the lexicon to use in the game.
    // The vector contains the words which will appear in the lexicon.
    // The words must consist of lowercase letters a-z only.
    // wordList:    a vector of strings containing the words to use.
    virtual void BuildLexicon(const vector<string>& word_list) = 0;

    // Set the board for the game to show die faces with those strings on them.
    // Each string in the board may be upper or lower case, and may contain one
    // or more letters; for purposes of finding words on the board, they should
    // all be considered lowercase.
    // new_board:   A one-dimensional vector of std::strings for the board.
    //              Elements [0...width - 1] should contain the top row of the
    //              board; elements [width...(2 * width) - 1] should contain the
    //              second row, and so on. In general, the element at (x, y)
    //              should be found at (x + y * width).
    virtual bool SetBoard(const vector<string>& new_board, int width) = 0;

    // Given a minimum word length, populate a vector of strings with all words
    // of at least that length on the board that are also in the lexicon.
    // minimum_word_length: the minimum word length to use.
    // words:       a vector that will contain all valid words.
    // returns:     false if either BuildLexicon() or SetBoard() have not yet
    //              been called; true otherwise.
    virtual bool GetAllValidWords(int minimum_word_length,
                                  vector<string>* words) = 0;

    // Check if a given word is in the lexicon.
    // word_to_check: the word to check.
    // returns:     true if the word is in the lexicon, false if it is not, or
    //              if BuildLexicon has not yet been called.
    virtual bool IsInLexicon(const string& word_to_check) = 0;

    // Given a word, populate a List of Integers specifying the location
    // of dice on the board that can be used to form the word. For details
    // of restrictions that a path of locations must meet, and how
    // locations are coded as integers, see the assignment README.
    // wordToCheck: a string specifying the word to find.
    // locations:   a vector<int> specifying locations of dice on the board
    //              that spell the word.
    // returns:     true if the word is on the board, false if it is not.
    virtual bool IsOnBoard(const string& word_to_check,
                           vector<int>* locations) = 0;

    // Populates, in a form suitable as an argument to SetBoard(), a
    // specification of a Boggle board. Mainly useful as a way to provide a
    // specific, non-random board for testing.
    // new_board:   a vector of strings to populate with the new board.
    // width:       a pointer to an integer to populate with the board's width.
    // returns:     false if any of the pointers were NULL; true otherwise.
    virtual bool GetCustomBoard(vector<string>* new_board, int* width) = 0;
};

#endif
