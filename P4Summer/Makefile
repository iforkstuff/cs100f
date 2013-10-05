# A simple makefile for CSE 100 P4

CC=g++
CXXFLAGS=-g
LDFLAGS=-g

all: clean netplan 

netplan: DisjSets.o Graph.o Edge.o Vertex.o Compare.o

Graph.o: Graph.hpp Edge.o Vertex.o Compare.o

DisjSets.o: DisjSets.hpp

Vertex.o: Vertex.hpp Edge.o

Edge.o: Edge.hpp

Compare.o: Compare.hpp Vertex.o

clean:
	rm -f netplan *.o core*
