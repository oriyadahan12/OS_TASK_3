#include "kosaraju.hpp"
#include <vector>

// Helper function for DFS
void DFS(int v, vector<list<int>>& adj, vector<bool>& visited, stack<int>& Stack) {
     if (v < 0 || v >= adj.size()) {
       // cout << "Invalid vertex index: " << v << endl; // Debug: Check for invalid index
        return;
    }
  //  cout << "Visiting vertex " << v << endl; // Debug: Start of DFS for vertex v
    visited[v] = true;
    for (int neighbor : adj[v]) {
        if (!visited[neighbor]) {
            //cout << "Going to visit neighbor " << neighbor << " of vertex " << v << endl; // Debug: Before visiting a neighbor
            DFS(neighbor, adj, visited, Stack);
        }
    }
    Stack.push(v);
 //   cout << "Pushing vertex " << v << " to Stack after visiting all neighbors" << endl; // Debug: After visiting all neighbors
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
  //  cout << "Getting transpose of the graph" << endl; // Debug: Before getting transpose
    vector<list<int>> transpose = getTranspose(adj);

    // Mark all the vertices as not visited (for second DFS)
    fill(visited.begin(), visited.end(), false);

    vector<vector<int>> sccs;
    while (!Stack.empty()) {
        int v = Stack.top();
        Stack.pop();
        if (!visited[v]) {
            vector<int> scc;
         //   cout << "Performing DFSUtil for vertex " << v << endl; // Debug: Before DFSUtil
            DFSUtil(v, transpose, visited, scc);
            sccs.push_back(scc);
         //   cout << "Found SCC with " << scc.size() << " vertices." << endl; // Debug: After finding an SCC
        }
    }
   // cout << "Finished findSCC, found " << sccs.size() << " strongly connected components." << endl; // Debug: End of function
    return sccs;
}

vector<list<int>> Newgraph(istringstream  &iss, int n, int m) {  // function to create a new graph and get m edge inputs
    // cout << "Enter input: ";

    vector<list<int>> vecList(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
      //  cout << "Enter edge " << i + 1 << ": ";
        iss >> u >> v;
        vecList[u - 1].push_back(v - 1);
    }
   //cout << "Graph created" << endl;

    return vecList;
}
pair<string, vector<vector<int>>> kosaraju(int n, vector<list<int>>& vecList) {
    cout << "Starting kosaraju function with n = " << n << endl;

    string scc_str;
    vector<vector<int>> ans = findSCC(n, vecList);

    for (int i = 0; i < ans.size(); i++) {
        for (int j = 0; j < ans[i].size(); j++) {
            scc_str += to_string(ans[i][j] + 1) + " ";
        }
        scc_str += "\n";
    }

    return make_pair(scc_str, ans);
}
void addEdge(int u, int v, vector<list<int>>& vecList) {
    vecList[u - 1].push_back(v - 1);
    cout << "added edge from " << u << " to " << v << endl;
}

void removeEdge(int u, int v, vector<list<int>>& vecList) {
    u = u - 1;
    v = v - 1;
    // For simplicity, assume the edge exists and remove it
    for (auto it = vecList[u].begin(); it != vecList[u].end(); ++it) {
        if (*it == v) {
            vecList[u].erase(it);
            cout << "removed edge from " << u + 1 << " to " << v + 1 << endl;
            return;
        }
    }
}