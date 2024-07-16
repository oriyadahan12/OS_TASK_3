#include <iostream>
#include <vector>
#include <stack>
#include <chrono> 

using namespace std;
using namespace std::chrono;

void dfs(int v, vector<bool>& visited, vector<vector<bool>>& adj, stack<int>& Stack) {
    visited[v] = true;
    for (size_t u = 1; u < adj.size(); ++u) {
        if (adj[v][u] && !visited[u]) {
            dfs(u, visited, adj, Stack);
        }
    }
    Stack.push(v);
}

void dfs_reverse(int v, vector<bool>& visited, vector<vector<bool>>& adj_rev, vector<int>& scc) {
    visited[v] = true;
    scc.push_back(v);
    for (size_t u = 1; u < adj_rev.size(); ++u) {
        if (adj_rev[v][u] && !visited[u]) {
            dfs_reverse(u, visited, adj_rev, scc);
        }
    }
}

void kosaraju(int n, vector<vector<bool>>& adj, vector<vector<bool>>& adj_rev) {
    vector<bool> visited(n + 1, false);
    stack<int> Stack;

    auto start = high_resolution_clock::now();

    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) {
            dfs(i, visited, adj, Stack);
        }
    }

    visited.assign(n + 1, false);

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

            auto end = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(end - start);
            cout << "Execution time: " << duration.count() << " microseconds" << endl;
        }
    }
}

int main() {
    int n, m;
    cin >> n >> m;

    vector<vector<bool>> adj(n + 1, vector<bool>(n + 1, false));
    vector<vector<bool>> adj_rev(n + 1, vector<bool>(n + 1, false));

    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u][v] = true;
        adj_rev[v][u] = true;
    }

    kosaraju(n, adj, adj_rev);

    return 0;
}
