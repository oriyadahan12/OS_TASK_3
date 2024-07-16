#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
#include <list>
#include <stack>
#include <algorithm>

using namespace std;

// Graph class definition
class Graph {
    int V;
    vector<list<int>> adj;
    vector<list<int>> adjRev;

    void DFS1(int v, vector<bool> &visited, stack<int> &Stack);
    void DFS2(int v, vector<bool> &visited, vector<int> &component);

public:
    Graph(int V);
    void reset(int V);
    void addEdge(int v, int w);
    void removeEdge(int v, int w);
    void findSCCs();
    string getSCCsAsString();
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
        }
    }
}

string Graph::getSCCsAsString() {
    vector<vector<int>> sccs;
    vector<bool> visited(V, false);
    stack<int> Stack;

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
            sccs.push_back(component);
        }
    }

    // Format SCCs as string
    string result;
    for (const auto &scc : sccs) {
        for (int v : scc) {
            result += to_string(v + 1) + " "; // Adjust for 1-based indexing
        }
        result += "\n";
    }
    return result;
}

// Server handling functions
void error(const char *msg) {
    perror(msg);
    exit(1);
}

void handleClientCommands(int sockfd, Graph &graph) {
    char buffer[256];
    int n;

    while (true) {
        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0) error("ERROR reading from socket");
        if (n == 0) break; // Client closed connection

        // Parse command
        stringstream ss(buffer);
        string command;
        ss >> command;

        if (command == "Newgraph") {
            int n, m;
            ss >> n >> m;
            graph.reset(n);
            for (int i = 0; i < m; ++i) {
                bzero(buffer, 256);
                n = read(sockfd, buffer, 255);
                if (n < 0) error("ERROR reading from socket");
                stringstream edgeStream(buffer);
                int u, v;
                edgeStream >> u >> v;
                graph.addEdge(u - 1, v - 1);
            }
            const char *successMsg = "Graph created successfully\n";
            n = write(sockfd, successMsg, strlen(successMsg));
            if (n < 0) error("ERROR writing to socket");
        } else if (command == "Kosaraju") {
            string sccResult = graph.getSCCsAsString();
            n = write(sockfd, sccResult.c_str(), sccResult.length());
            if (n < 0) error("ERROR writing to socket");
        } else if (command == "Newedge") {
            int i, j;
            ss >> i >> j;
            graph.addEdge(i - 1, j - 1);
            const char *successMsg = "Edge added successfully\n";
            n = write(sockfd, successMsg, strlen(successMsg));
            if (n < 0) error("ERROR writing to socket");
        } else if (command == "Removeedge") {
            int i, j;
            ss >> i >> j;
            graph.removeEdge(i - 1, j - 1);
            const char *successMsg = "Edge removed successfully\n";
            n = write(sockfd, successMsg, strlen(successMsg));
            if (n < 0) error("ERROR writing to socket");
        } else {
            // Invalid command handling
            const char *errorMsg = "Invalid command\n";
            n = write(sockfd, errorMsg, strlen(errorMsg));
            if (n < 0) error("ERROR writing to socket");
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <port>" << endl;
        exit(1);
    }

    int portno = atoi(argv[1]);
    int sockfd, newsockfd;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    Graph graph(0); // Initialize an empty graph

    while (true) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
             error("ERROR on accept");

        pid_t pid = fork();
        if (pid < 0) {
            error("ERROR on fork");
        }
        if (pid == 0) {
            close(sockfd);
            handleClientCommands(newsockfd, graph); // Pass graph object to handler
            exit(0);
        } else {
            close(newsockfd);
        }
    }

    close(sockfd);
    return 0;
}
