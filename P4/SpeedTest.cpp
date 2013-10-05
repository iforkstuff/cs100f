#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "MyBogglePlayer.h"
using std::vector;using std::string;
int main(void){
    MyBogglePlayer me;
    vector<string> u,v,w;
    int n;
    if(!me.GetCustomBoard(&v,&n)){std::cerr<<"very bad!"<<std::endl;
            return 1;}
    if(!me.SetBoard(v,n)){std::cerr<<"very bad 2!"<<std::endl;
            return 2;}
    std::ifstream in("boglex.txt");
    char line[80];
    while(in.good()) {
        in.getline(line, 80);
        string ln(line);
        if(ln.length() > 0)w.push_back(ln);
    }
    me.BuildLexicon(w);
    if(me.GetAllValidWords(2,&u))std::cout<<u.size()<<std::endl;
    else std::cerr<<"bad!"<<std::endl;
    return 0;
}
