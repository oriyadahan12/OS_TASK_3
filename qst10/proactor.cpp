#include "proactor.hpp"

void proactor::startProactor(int server_fd, proactorFunc threadFunc) {
    server_thread = thread([server_fd, threadFunc, this]() {
        // accept clients and create a thread for each client
        for (;;) {
            struct sockaddr_in client_addr;
            socklen_t client_addr_len = sizeof(client_addr);

            int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
            if (client_fd < 0) {
                perror("accept");
                break;
            }
            cout << "New connection from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << endl;
            std::thread client_thread(threadFunc, client_fd, std::ref(this->mtx));
            client_thread.detach();
        }
    });

}

void proactor::stopProactor() {
    if (server_thread.joinable()) {
        server_thread.join();
    }
}

mutex &proactor::get_mtx() {
    return mtx;
}