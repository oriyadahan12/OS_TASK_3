
#include <iostream>
#include <vector>
#include <stack>
#include <chrono> 

using namespace std;
using namespace std::chrono;

// Function to perform DFS and fill finishing times in stack
void dfs(int v, vector<bool>& visited, vector<vector<int>>& adj, stack<int>& Stack) {
    visited[v] = true;
    for (int u : adj[v]) {
        if (!visited[u]) {
            dfs(u, visited, adj, Stack);
        }
    }
    Stack.push(v);
}

// Function to perform DFS on the transposed graph
void dfs_reverse(int v, vector<bool>& visited, vector<vector<int>>& adj_rev, vector<int>& scc) {
    visited[v] = true;
    scc.push_back(v); // Store vertices in the same SCC
    for (int u : adj_rev[v]) {
        if (!visited[u]) {
            dfs_reverse(u, visited, adj_rev, scc);
        }
    }
}

// Function to find and print all strongly connected components
void kosaraju(int n, vector<vector<int>>& adj, vector<vector<int>>& adj_rev) {
    vector<bool> visited(n + 1, false);
    stack<int> Stack;

    // Time measurement variables
    auto start = high_resolution_clock::now();

    // Step 1: Fill vertices in stack according to their finishing times
    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) {
            dfs(i, visited, adj, Stack);
        }
    }

    // Step 2: Reverse the graph
    visited.assign(n + 1, false);

    // Step 3: Process vertices in order defined by Stack
    while (!Stack.empty()) {
        int v = Stack.top();
        Stack.pop();
        if (!visited[v]) {
            vector<int> scc;
            dfs_reverse(v, visited, adj_rev, scc);
            
            for (int u : scc) {
                cout << u << " ";
            }
            cout << endl;
            // Measure execution time
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(end - start);
            cout << "Execution time: " << duration.count() << " microseconds" << endl;
        }
    }
}

int main() {
    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj(n + 1);
    vector<vector<int>> adj_rev(n + 1);

    // Read the graph edges
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj_rev[v].push_back(u); // Store the reverse edges for the transposed graph
    }

    // Execute Kosaraju's algorithm
    kosaraju(n, adj, adj_rev);

    return 0;
}
