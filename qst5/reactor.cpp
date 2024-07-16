#include "reactor.hpp"
#include <iostream>

using namespace std;
Reactor::Reactor() : running(false) {}
Reactor::~Reactor() {
    // Stop the reactor
    stopReactor();
}

int Reactor::addFdToReactor(int fd, reactorFunc func) {
    // Add the file descriptor and function to the reactor
    this->fdFunctions[fd] = func;
    return 0;
}

int Reactor::removeFdFromReactor(int fd) {
    // Remove the file descriptor from the reactor
    this->fdFunctions.erase(fd);
    return 0;
}
int Reactor::stopReactor() {
    this->running = false;
    return 0;
}
void Reactor::run() {
    // Implementation of the event loop that waits for events on the registered fds
    // and calls the appropriate callback functions.
    while (true) {
        // This is a simplified example using select.
        // In a real implementation, you might use select, poll, or epoll.
        fd_set read_fds;
        FD_ZERO(&read_fds);
        int max_fd = 0;

        for (const auto &entry : fdFunctions) {
            int fd = entry.first;
            FD_SET(fd, &read_fds);
            if (fd > max_fd) {
                max_fd = fd;
            }
        }

        int activity = select(max_fd + 1, &read_fds, nullptr, nullptr, nullptr);

        if (activity < 0 && errno != EINTR) {
            perror("select");
            break;
        }

        for(int i = 0; i <= max_fd; i++) {
            if (FD_ISSET(i, &read_fds)) {
                reactorFunc func = fdFunctions[i];
                func(i);  // The callback function is called here with i as the argument
            }
        }
    }
}