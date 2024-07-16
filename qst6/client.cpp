#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

int main() {
    int sockfd;
    struct sockaddr_in serv_addr;
    char buffer[256];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(9035);

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        close(sockfd);
        exit(1);
    }

    while (true) {
        cout << "Enter command: ";
        bzero(buffer, sizeof(buffer)); // Use sizeof for safety
        cin.getline(buffer, sizeof(buffer) - 1); // Leave space for null terminator
        cout << buffer << endl;

        int n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) {
            perror("ERROR writing to socket");
            close(sockfd);
            exit(1);
        }
        // cout << n << endl;

        // Read server response after command
        bzero(buffer, sizeof(buffer));
        n = read(sockfd, buffer, sizeof(buffer) - 1); // Leave space for null terminator
        if (n < 0) {
            perror("ERROR reading from socket");
            close(sockfd);
            exit(1);
        }
        
        buffer[n] = '\0'; // Null-terminate the received string
        cout << "Server response: " << buffer << endl;

        // Handle edge input if the command was "Newgraph"
        if (strncmp(buffer, "New graph created", 17) == 0) {
            cout << "Enter graph edges:" << endl;
            int edges;
            sscanf(buffer + 17, "%d", &edges); // Get the number of edges from response
            edges--;
            for (int i = 0; i < edges; ++i) {
                cout << "Enter edge (u v): ";
                bzero(buffer, sizeof(buffer)); // Clear buffer
                cin.getline(buffer, sizeof(buffer) - 1); // Get edge input
                n = write(sockfd, buffer, strlen(buffer)); // Send edge to server
                if (n < 0) {
                    perror("ERROR writing edge to socket");
                    close(sockfd);
                    exit(1);
                }

                // Read server response for edge addition
                bzero(buffer, sizeof(buffer));
                n = read(sockfd, buffer, sizeof(buffer) - 1); // Leave space for null terminator
                if (n < 0) {
                    perror("ERROR reading from socket");
                    close(sockfd);
                    exit(1);
                }
                
                buffer[n] = '\0'; // Null-terminate the received string
                cout << "Server response: " << buffer << endl;
            }
        }

        if (strcmp(buffer, "exit\n") == 0) {
            break; // Exit the loop on "exit" command
        }
    }

    close(sockfd);
    return 0;
}
