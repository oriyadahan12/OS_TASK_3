#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

class Graph {
    int V;
    vector<list<int>> adj;
    vector<list<int>> adjRev;

    void DFS1(int v, vector<bool> &visited, stack<int> &Stack);
    void DFS2(int v, vector<bool> &visited, vector<int> &component);

public:
    Graph(int V);
    void addEdge(int v, int w);
    void removeEdge(int v, int w);
    void findSCCs();
    void reset(int V);
};

Graph::Graph(int V) {
    this->V = V;
    adj.resize(V);
    adjRev.resize(V);
}

void Graph::reset(int V) {
    this->V = V;
    adj.clear();
    adjRev.clear();
    adj.resize(V);
    adjRev.resize(V);
}

void Graph::addEdge(int v, int w) {
    adj[v].push_back(w);
    adjRev[w].push_back(v);
}

void Graph::removeEdge(int v, int w) {
    adj[v].remove(w);
    adjRev[w].remove(v);
}

void Graph::DFS1(int v, vector<bool> &visited, stack<int> &Stack) {
    visited[v] = true;
    for (int i : adj[v])
        if (!visited[i])
            DFS1(i, visited, Stack);
    Stack.push(v);
}

void Graph::DFS2(int v, vector<bool> &visited, vector<int> &component) {
    visited[v] = true;
    component.push_back(v);
    for (int i : adjRev[v])
        if (!visited[i])
            DFS2(i, visited, component);
}

void Graph::findSCCs() {
    stack<int> Stack;
    vector<bool> visited(V, false);

    // Step 1: Fill vertices in stack according to their finishing times
    for (int i = 0; i < V; i++)
        if (!visited[i])
            DFS1(i, visited, Stack);

    // Step 2: Create a reversed graph
    fill(visited.begin(), visited.end(), false);

    // Step 3: Process all vertices in order defined by Stack
    while (!Stack.empty()) {
        int v = Stack.top();
        Stack.pop();
        if (!visited[v]) {
            vector<int> component;
            DFS2(v, visited, component);
            for (int v : component)
                cout << v + 1 << " "; // Adjusting for 1-based indexing
            cout << endl;
        }
    }
}

int main() {
    Graph g(0);
    string line;
    while (getline(cin, line)) {
        stringstream ss(line);
        string command;
        ss >> command;

        if (command == "Newgraph") {
            int n, m;
            ss >> n >> m;
            g.reset(n);
            for (int i = 0; i < m; ++i) {
                getline(cin, line);
                stringstream edgeStream(line);
                int u, v;
                edgeStream >> u >> v;
                g.addEdge(u - 1, v - 1);
            }
        } else if (command == "Kosaraju") {
            g.findSCCs();
        } else if (command == "Newedge") {
            int i, j;
            ss >> i >> j;
            g.addEdge(i - 1, j - 1);
        } else if (command == "Removeedge") {
            int i, j;
            ss >> i >> j;
            g.removeEdge(i - 1, j - 1);
        }
    }
    return 0;
}
