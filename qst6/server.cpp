#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <atomic>
#include "reactor.hpp" // Include your reactor header
#include "graph.hpp"   // Include your graph header

using namespace std;

// Global variable for the graph
Graph* graph = nullptr; // Declare graph here

void* handleClient(int client_socket) {
    cout << "entered handleClient" << endl;
    char buffer[256];
    int n;

    while (true) {
        bzero(buffer, sizeof(buffer));
        n = read(client_socket, buffer, sizeof(buffer) - 1);
        if (n <= 0) {
            if (n < 0) perror("ERROR reading from socket");
            else cout << "Client closed connection" << endl;
            break;
        }

        cout << "Received from client: " << buffer << endl;

        // Parse command
        stringstream ss(buffer);
        string command;
        ss >> command;

        if (command == "Newgraph") {
            int v, e;
            ss >> v >> e;
            graph = new Graph(v); // Initialize the graph
            cout << "Creating graph with " << v << " vertices and " << e << " edges" << endl;

            char message[18] = "New graph created";
            n = write(client_socket, message, strlen(message)); // TODO check if this is server or client
            if (n < 0) {
                perror("ERROR writing to socket");
                close(client_socket);
                exit(1);
            }

            // Read edges
            for (int i = 0; i < e; ++i) {
                bzero(buffer, sizeof(buffer)); // Clear the buffer
                n = read(client_socket, buffer, sizeof(buffer) - 1);
                if (n <= 0) {
                    if (n < 0) perror("ERROR reading from socket");
                    else cout << "Client closed connection during edge reading" << endl;
                    break;
                }
                buffer[n] = '\0'; // Null-terminate the buffer
                cout << "Received edge from client: " << buffer << endl;

                char message[26] = "Received edge from client";
                n = write(client_socket, message, strlen(message)); // TODO check if this is server or client
                if (n < 0) {
                    perror("ERROR writing to socket");
                    close(client_socket);
                    exit(1);
                }

                stringstream edgeStream(buffer);
                int u, w;
                edgeStream >> u >> w;
                graph->addEdge(u - 1, w - 1); // Adjust for 0-based indexing
            }
            // string response = "New graph created with " + to_string(v) + " vertices and " + to_string(e) + " edges\n";
            // send(client_socket, response.c_str(), response.size(), 0);
        } else if (command == "Kosaraju") {
            if (graph != nullptr) {
                string sccResult = graph->getSCCsAsString();
                cout << "Kosaraju: " << sccResult << endl;
                send(client_socket, sccResult.c_str(), sccResult.size(), 0);
            } else {
                string errorMsg = "No graph created. Please create a graph first.\n";
                send(client_socket, errorMsg.c_str(), errorMsg.size(), 0);
            }
        } else if (command.substr(0, 7) == "Newedge") {
            int u, w;
            ss >> u >> w;
            if (graph != nullptr) {
                graph->addEdge(u - 1, w - 1); // Adjust for 0-based indexing
                string response = "Edge added: " + to_string(u) + " -> " + to_string(w) + "\n";
                send(client_socket, response.c_str(), response.size(), 0);
            } else {
                string errorMsg = "No graph created. Please create a graph first.\n";
                send(client_socket, errorMsg.c_str(), errorMsg.size(), 0);
            }
        } else if (command.substr(0, 11) == "Removeedge") {
            int u, w;
            ss >> u >> w;
            if (graph != nullptr) {
                graph->removeEdge(u - 1, w - 1); // Adjust for 0-based indexing
                string response = "Edge removed: " + to_string(u) + " -> " + to_string(w) + "\n";
                send(client_socket, response.c_str(), response.size(), 0);
            } else {
                string errorMsg = "No graph created. Please create a graph first.\n";
                send(client_socket, errorMsg.c_str(), errorMsg.size(), 0);
            }
        } else {
            string errorMsg = "Invalid command\n";
            send(client_socket, errorMsg.c_str(), errorMsg.size(), 0);
        }
    }
    close(client_socket);
    return nullptr;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <port>" << endl;
        exit(1);
    }

    int portno = atoi(argv[1]);
    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    Reactor* reactor = reinterpret_cast<Reactor*>(startReactor());

    cout << "Server is listening on port " << portno << endl;

    while (true) {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("ERROR on accept");
            continue; // Continue accepting other clients
        }

        cout << "Accepted a new client connection" << endl;

        reactor->addFdToReactor(newsockfd, handleClient); // Register the client socket
        reactor->run();
    }

    close(sockfd);
    return 0;
}
