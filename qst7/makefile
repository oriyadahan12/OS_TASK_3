.PHONY: all run clean

all: q7

run: q7
	./q7

q7: q7.o kosaraju.o
	g++ -o q7 q7.o kosaraju.o -lpthread  # Add -lpthread here

q7.o: q7.cpp 
	g++ -c q7.cpp

kosaraju.o: kosaraju.cpp
	g++ -c kosaraju.cpp

clean:
	rm -f q7 q7.o kosaraju.o
