#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <list>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "kosaraju.hpp"

#define PORT "9034"
using namespace std;

vector<list<int>> sharedGraph;
mutex mtx;

string graph_user_commands(string input_user) {
    string ans;
    string command_of_user;
    istringstream iss(input_user);
    iss >> command_of_user;

    int n, m;

    if (command_of_user.empty()) {
        ans += "No command received.\n";
    } else if (command_of_user == "Newgraph") {
        iss >> n >> m;
        if (n <= 0 || m < 0) {
            ans += "Invalid graph parameters. \n";
        } else {
            sharedGraph = Newgraph(iss, n, m);
            ans += "Graph created:\n";
            for (int i = 0; i < n; i++) {
                ans += to_string(i + 1) + ": ";
                for (int neighbor : sharedGraph[i]) {
                    ans += to_string(neighbor + 1) + " ";
                }
                ans += "\n";
            }
        }
    } else if (command_of_user == "Kosaraju") {
        if (!sharedGraph.empty()) {
            ans += kosaraju(sharedGraph.size(), sharedGraph);
        } else {
            ans += "No graph found for Kosaraju.\n";
        }
    } else if (command_of_user == "Newedge") {
        int u, v;
        iss >> u >> v;
        if (!sharedGraph.empty()) {
            addEdge(u, v, sharedGraph);
            ans += "Edge added from " + to_string(u) + " to " + to_string(v) + "\n";
        } else {
            ans += "No graph found for adding edge.\n";
        }
    } else if (command_of_user == "Removeedge") {
        int u, v;
        iss >> u >> v;
        if (!sharedGraph.empty()) {
            removeEdge(u, v, sharedGraph);
            ans += "Edge removed from " + to_string(u) + " to " + to_string(v) + "\n";
        } else {
            ans += "No graph found for removing edge.\n";
        }
    } else {
        ans += "Unknown command.\n";
    }
    return ans;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main() {
    int listener;
    struct sockaddr_storage clientAddress;
    socklen_t addrlen;
    char remoteIP[INET6_ADDRSTRLEN];
    int yes = 1;
    int rv;
    struct addrinfo hints, *ai, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        cerr << "selectserver: " << gai_strerror(rv) << endl;
        exit(1);
    }

    for (p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) {
            continue;
        }

        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    if (p == NULL) {
        cerr << "selectserver: failed to bind" << endl;
        exit(2);
    }

    freeaddrinfo(ai);

    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(3);
    }

    thread server = thread([&]() {
        while (true) {
            addrlen = sizeof(clientAddress);
            int newfd = accept(listener, (struct sockaddr *)&clientAddress, &addrlen);

            if (newfd == -1) {
                perror("accept");
                continue;
            }

            cout << "selectserver: new connection from "
                 << inet_ntop(clientAddress.ss_family,
                              get_in_addr((struct sockaddr *)&clientAddress),
                              remoteIP, INET6_ADDRSTRLEN)
                 << " on socket " << newfd << endl;

            thread t = thread([&](int clientSocket) {
                char buf[256];
                int nbytes;
                while ((nbytes = recv(clientSocket, buf, sizeof(buf), 0)) > 0) {
                    buf[nbytes] = '\0';
                    string input_user(buf);

                    mtx.lock();
                    string response = graph_user_commands(input_user);
                    mtx.unlock();

                    if (send(clientSocket, response.c_str(), response.length(), 0) == -1) {
                        perror("send");
                    }
                }
                if (nbytes == 0) {
                    cout << "selectserver: socket " << clientSocket << " hung up" << endl;
                } else {
                    perror("recv");
                }
                close(clientSocket);
            }, newfd);
            t.detach();
        }
    });

    server.join();
    return 0;
}
