#include "kosaraju.hpp"
#include <iostream>

// Helper function for DFS
void DFS(int v, vector<list<int>>& adj, vector<bool>& visited, stack<int>& Stack) {
    if (v < 0 || v >= adj.size()) {
        return;
    }
    visited[v] = true;
    for (int neighbor : adj[v]) {
        if (!visited[neighbor]) {
            DFS(neighbor, adj, visited, Stack);
        }
    }
    Stack.push(v);
}

// Function to get the transpose of the graph
vector<list<int>> getTranspose(vector<list<int>>& adj) {
    vector<list<int>> transpose(adj.size());
    for (int v = 0; v < adj.size(); v++) {
        for (int neighbor : adj[v]) {
            transpose[neighbor].push_back(v);
        }
    }
    return transpose;
}

// Helper function for DFS on transposed graph
void DFSUtil(int v, vector<list<int>>& adj, vector<bool>& visited, vector<int>& scc) {
    visited[v] = true;
    scc.push_back(v);
    for (int neighbor : adj[v]) {
        if (!visited[neighbor]) {
            DFSUtil(neighbor, adj, visited, scc);
        }
    }
}

// Function to find strongly connected components using Kosaraju's algorithm
vector<vector<int>> findSCC(int n, vector<list<int>>& adj) {
    stack<int> Stack;
    vector<bool> visited(n, false);

    // Fill vertices in stack according to their finishing times
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            DFS(i, adj, visited, Stack);
        }
    }

    // Get the transpose of the graph
    vector<list<int>> transpose = getTranspose(adj);

    // Mark all the vertices as not visited (for second DFS)
    fill(visited.begin(), visited.end(), false);

    vector<vector<int>> sccs;
    while (!Stack.empty()) {
        int v = Stack.top();
        Stack.pop();
        if (!visited[v]) {
            vector<int> scc;
            DFSUtil(v, transpose, visited, scc);
            sccs.push_back(scc);
        }
    }
    return sccs;
}

vector<list<int>> Newgraph(istringstream &iss, int n, int m) {
    vector<list<int>> vecList(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        if (!(iss >> u >> v) || u <= 0 || v <= 0 || u > n || v > n) {
            cerr << "Invalid edge input. Edges must be between 1 and " << n << ".\n";
            continue;
        }
        vecList[u - 1].push_back(v - 1);
    }
    return vecList;
}

string kosaraju(int n, vector<list<int>>& vecList) {
    string scc_str;
    vector<vector<int>> ans = findSCC(n, vecList);

    for (int i = 0; i < ans.size(); i++) {
        for (int j = 0; j < ans[i].size(); j++) {
            scc_str += to_string(ans[i][j] + 1) + " ";
        }
        scc_str += "\n";
    }

    return scc_str;
}

void addEdge(int u, int v, vector<list<int>>& vecList) {
    vecList[u - 1].push_back(v - 1);
}

void removeEdge(int u, int v, vector<list<int>>& vecList) {
    u = u - 1;
    v = v - 1;
    for (auto it = vecList[u].begin(); it != vecList[u].end(); ++it) {
        if (*it == v) {
            vecList[u].erase(it);
            return;
        }
    }
}
