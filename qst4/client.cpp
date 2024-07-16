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

using namespace std;

void error(const char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr,"Usage: %s <hostname> <port>\n", argv[0]);
        exit(0);
    }

    int portno = atoi(argv[2]);
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    while (true) {
        printf("Enter command: ");
        bzero(buffer, 256);
        fgets(buffer, 255, stdin);

        // Remove trailing newline from fgets
        buffer[strcspn(buffer, "\n")] = 0;

        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0)
            error("ERROR writing to socket");

        // Check if the command is "Newgraph"
        stringstream ss(buffer);
        string command;
        ss >> command;
        if (command == "Newgraph") {
            int n, m;
            ss >> n >> m;
            for (int i = 0; i < m; ++i) {
                printf("Enter command: ");
                bzero(buffer, 256);
                fgets(buffer, 255, stdin);
                buffer[strcspn(buffer, "\n")] = 0;

                n = write(sockfd, buffer, strlen(buffer));
                if (n < 0)
                    error("ERROR writing to socket");
            }
        }

        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0)
            error("ERROR reading from socket");

        printf("Server response: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
