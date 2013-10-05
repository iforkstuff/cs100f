#include "BogglePlayer.h"
#include "MyBogglePlayer.h"
#include <iostream>
#include <vector>
#include <string>

int main (int argc, char* argv[]) {

  BogglePlayer * p = new MyBogglePlayer();
  vector<string> lex;
  lex.push_back("a");
  vector<string> board;
  board.push_back("a");
  vector<string> words;
  string wordA("a");
  string wordB("b");
  vector<int> locations;

  p->BuildLexicon(lex);

  if(!p->SetBoard(board,1)) {
    std::cerr << "Apparent problem with SetBoard." << std::endl;
    return -1;
  }

  if(p->IsInLexicon(wordB)) {
    std::cerr << "Apparent problem with IsInLexicon." << std::endl;
    return -1;
  }
  if(!p->IsInLexicon(wordA)) {
    std::cerr << "Apparent problem with IsInLexicon." << std::endl;
    return -1;
  }
  if(p->IsOnBoard(wordB,&locations)) {
    std::cerr << "Apparent problem with IsOnBoard." << std::endl;
    return -1;
  }
  if(!p->IsOnBoard(wordA,&locations)) {
    std::cerr << "Apparent problem with IsOnBoard." << std::endl;
    return -1;
  }
  p->GetAllValidWords(0,&words);
  if(words.size() != 1 || words[0].compare("a") != 0) {
    std::cerr << "Apparent problem with GetAllValidWords." << std::endl;
    return -1;
  }

  return 0;

}
