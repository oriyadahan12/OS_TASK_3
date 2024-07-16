CXX = g++
CXXFLAGS = -std=c++11 -Wall

TARGETS = server client
SRCS = server.cpp client.cpp reactor.cpp graph.cpp kosaraju.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGETS)

server: server.o reactor.o graph.o kosaraju.o
	$(CXX) $(CXXFLAGS) -o server server.o reactor.o graph.o kosaraju.o

client: client.o reactor.o graph.o kosaraju.o
	$(CXX) $(CXXFLAGS) -o client client.o reactor.o graph.o kosaraju.o

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) $(OBJS)
