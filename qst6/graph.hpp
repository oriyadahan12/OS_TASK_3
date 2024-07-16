#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "kosaraju.hpp"
#include <vector>
#include <list>
#include <string>

class Graph {
private:
    int V; // Number of vertices
    std::vector<std::list<int>> adj; // Adjacency list

public:
    Graph(int V); // Constructor
    void addEdge(int v, int w); // Add edge to graph
    void removeEdge(int v, int w); // Remove edge from graph
    void reset(int V); // Reset the graph
    std::string getGraphInfo(); // Get information about the graph
    std::string getSCCsAsString(); // Get strongly connected components as a string
};

#endif // GRAPH_HPP
