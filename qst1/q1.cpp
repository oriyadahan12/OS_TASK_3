#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

class Graph {
    int V; // Number of vertices
    vector<vector<int>> adj; // Adjacency list
    vector<vector<int>> adjRev; // Reversed adjacency list

    // DFS utility function
    void DFS1(int v, vector<bool> &visited, stack<int> &Stack);
    void DFS2(int v, vector<bool> &visited, vector<int> &component);

public:
    Graph(int V); // Constructor
    void addEdge(int v, int w); // Add an edge to the graph
    void findSCCs(); // Function to find and print all SCCs
};

Graph::Graph(int V) {
    this->V = V;
    adj.resize(V);
    adjRev.resize(V);
}

void Graph::addEdge(int v, int w) {
    adj[v].push_back(w);
    adjRev[w].push_back(v); // Reverse the edge
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

            // Print the current strongly connected component
            for (int v : component)
                cout << v << " ";
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
        g.addEdge(u - 1, v - 1); // Adjusting for 0-based indexing
    }

    g.findSCCs();

    return 0;
}
