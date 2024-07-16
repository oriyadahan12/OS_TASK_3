#include "graph.hpp"
#include <sstream>
#include <algorithm>

Graph::Graph(int V) : V(V) {
    adj.resize(V);
}

void Graph::addEdge(int v, int w) {
    adj[v].push_back(w);
}

void Graph::removeEdge(int v, int w) {
    adj[v].remove(w);
}

void Graph::reset(int V) {
    this->V = V;
    adj.clear();
    adj.resize(V);
}

std::string Graph::getGraphInfo() {
    std::ostringstream info;
    info << "Graph created with " << V << " vertices.\nEdges:\n";
    for (int i = 0; i < V; ++i) {
        for (int neighbor : adj[i]) {
            info << i + 1 << " -> " << neighbor + 1 << "\n"; // 1-based indexing
        }
    }
    return info.str();
}

std::string Graph::getSCCsAsString() {
    string SCCs = kosaraju(V, adj);
    return SCCs;
}
