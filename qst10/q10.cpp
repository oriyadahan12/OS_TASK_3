/*
** selectserver.cpp -- a cheezy multiperson chat server
*/

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <list>
#include <mutex>
#include <sstream>  // Include this for istringstream
#include <string>
#include <thread>
#include <vector>

#include "kosaraju.hpp"
#include "proactor.hpp"

#define PORT "9034"  // port we're listening on
using namespace std;
bool gthen50 = false;
condition_variable gt_than_50;
vector<list<int>> sharedGraph;  // Shared graph for all clients

void print_status(mutex &mtx) {
    // wait until the more_than_50_flag is true, and then each time the kosaraju function is called we will print the status
    unique_lock<mutex> lk(mtx);//, unique_lock<mutex> lk(mtx); 
    gt_than_50.wait(lk, [] { return gthen50; });
    while (true) {
        // check if the flag is still true
        if (gthen50) {
            cout << "\033[1;32m" << "At Least 50% of the graph belongs to the same scc\n\n"
                 << "\033[0m";
        } else {
            // print in red
            cout << "\033[1;31m" << "At Least 50% of the graph no longer belongs to the same SCC\n\n"
                 << "\033[0m";
        }

        // wait until the next notification
        gt_than_50.wait(lk);
    }
}
void is_gt_50_kosaraju(int n) {
    vector<vector<int>> sccs = kosaraju(sharedGraph.size(), sharedGraph).second;
    for (int i = 0; i < sccs.size(); i++) {
        if (sccs[i].size() > n / 2) {
            gthen50 = true;
            gt_than_50.notify_one();
            break;
        }
    }
}
void is_gt_50_remove(int n) {
    if (gthen50 == true) {  // if before the removal of the edge the graph was greater than 50% of the same scc
        gthen50 = false;
        vector<vector<int>> sccs = kosaraju(sharedGraph.size(), sharedGraph).second;
        for (int i = 0; i < sccs.size(); i++) {
            if (sccs[i].size() > n / 2) {
                gthen50 = true;
            }
        }
        if (gthen50 == false) {
            gt_than_50.notify_one();
        }
    }
}

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
            ans += kosaraju(sharedGraph.size(), sharedGraph).first;
        } else {
            ans += "No graph found for Kosaraju.\n";
        }
        is_gt_50_kosaraju(n);
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
            is_gt_50_remove(n);
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
    proactor proactor;
    int listener = -1;                      // listening socket descriptor
    struct sockaddr_storage clientAddress;  // client address
    socklen_t addrlen;
    char remoteIP[INET6_ADDRSTRLEN];
    int yes = 1;  // for setsockopt() SO_REUSEADDR, below
    int rv;
    struct addrinfo hints, *ai, *p;

    // get us a socket and bind it
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

        // lose the pesky "address already in use" error message
        if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }
        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    // if we got here, it means we didn't get bound
    if (p == NULL) {
        cerr << "selectserver: failed to bind" << endl;
        exit(2);
    }

    freeaddrinfo(ai); 

    // listen
    if (listen(listener, 10) == -1) {
        perror("listen");
        close(listener);
        exit(3);
    }
    thread component_status(print_status, ref(proactor.get_mtx()));
    component_status.detach();
    // Start the proactor with a lambda function
    proactor.startProactor(listener, [](int newfd, mutex &mtx) -> void * {
        char buf[2048];
        int nbytes;
        while (true) {
            nbytes = recv(newfd, buf, sizeof(buf), 0);
            if (nbytes <= 0) {
                if (nbytes == 0) {
                    cout << "selectserver: socket " << newfd << " hung up" << endl;
                } else {
                    perror("recv");
                }
                close(newfd);
                break;
            } else {
                buf[nbytes] = '\0';
                string client_input = string(buf);
                mtx.lock();
                cout << "mutex locked by thread: " << newfd << endl;
                string ans = graph_user_commands(client_input);
                // sleep(5);
                mtx.unlock();
                cout << "mutex unlocked by thread: " << newfd << endl;
                cout << "Response to client: " << ans << endl;
                if (send(newfd, ans.c_str(), ans.size(), 0) == -1) {
                    perror("send");
                }
            }
        }
        return nullptr;
    });

    // Stop the proactor
    proactor.stopProactor();

    return 0;
}