.PHONY: all run clean

all: q10

run: q10
	./q10

q10: q10.o kosaraju.o libProactor.a
	g++ -o q10 q10.o kosaraju.o -L. -lProactor -pthread

q10.o: q10.cpp
	g++ -c q10.cpp

kosaraju.o: kosaraju.cpp
	g++ -c kosaraju.cpp

libProactor.a: proactor.o
	ar rcs libProactor.a proactor.o
	ranlib libProactor.a

proactor.o: proactor.cpp proactor.hpp
	g++ -c proactor.cpp	

clean:
	rm -f q10 q10.o kosaraju.o libProactor.a proactor.o
