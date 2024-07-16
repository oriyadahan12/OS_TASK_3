#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <algorithm>
#include <chrono>
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
    void findSCCs();
};

Graph::Graph(int V) {
    this->V = V;
    adj.resize(V);
    adjRev.resize(V);
}

void Graph::addEdge(int v, int w) {
    adj[v].push_back(w);
    adjRev[w].push_back(v);
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
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < V; i++)
        if (!visited[i])
            DFS1(i, visited, Stack);
    auto end = chrono::high_resolution_clock::now();
    cout << "Time taken for step 1 (list): "
         << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microseconds" << endl;

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
    int V, E;
    cin >> V >> E;
    Graph g(V);
    for (int i = 0; i < E; i++) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u - 1, v - 1);
    }
    g.findSCCs();
    return 0;
}
