# A simple makefile for CSE 100 P1

#use g++ for everything
CC= g++  	

# include debugging symbols in object files,
# and enable all warnings
CXXFLAGS= -std=c++11 -g -Wall

#include debugging symbols in executable
LDFLAGS= -g	

rst: test_RST.o countint.o
	g++ -o rst test_RST.o countint.o
bench: benchtree.o countint.o
	g++ -o bench benchtree.o countint.o

benchtree.o: BST.hpp RST.hpp countint.o

test_RST.o: BST.hpp BSTNode.hpp BSTIterator.hpp countint.o

countint.o: countint.hpp

clean: 
	rm -f *.o rst *core
