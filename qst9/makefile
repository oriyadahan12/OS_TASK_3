.PHONY: all run clean

all: q9

run: q9
	./q9

q9: q9.o kosaraju.o libreactor.a
	g++ -o q9 q9.o kosaraju.o -L. -lProactor -lpthread

q9.o: q9.cpp 
	g++ -c q9.cpp

Kosaraju.o: kosaraju.cpp
	g++ -c kosaraju.cpp

libreactor.a: proactor.o
	ar rcs libProactor.a proactor.o
	ranlib libProactor.a

Proactor.o: proactor.cpp proactor.hpp
	g++ -c proactor.cpp	

clean:
	rm -f q9 q9.o kosaraju.o libProactor.a proactor.o
