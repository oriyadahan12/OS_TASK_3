libreactor.a: proactor.o
	ar rcs libProactor.a proactor.o
	ranlib libProactor.a

proactor.o: proactor.cpp proactor.hpp
	g++ -std=c++2a -c proactor.cpp

clean:
	rm -f libProactor.a proactor.o

.PHONY: all clean