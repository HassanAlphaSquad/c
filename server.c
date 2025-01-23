#include <stdio.h>      // For printf
#include <unistd.h>     // For close
#include <sys/types.h>  // For socket
#include <sys/socket.h> // For socket
#include <string.h>     // For memset
#include <stdlib.h>     // For sizeof
#include <netinet/in.h> // For INADDR_ANY

#define PORT 8000
#define MAXSZ 100

int main()
{
    int sockfd;    // To create socket
    int newsockfd; // To accept connection

    struct sockaddr_in serverAddress; // Server receives on this address
    struct sockaddr_in clientAddress; // Server sends to client on this address

    int n;
    char msg[MAXSZ];
    int clientAddressLength;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // if (sockfd < 0)
    // {
    //     perror("Socket creation failed");
    //     exit(1);
    // }

    // Initialize the socket addresses
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(PORT);

    // Bind the socket with the server address and port

    bind(sockfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    // if (bind(sockfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    // {
    //     perror("Bind failed");
    //     close(sockfd);
    //     exit(1);
    // }

    // Listen for connections from clients
    // if (listen(sockfd, 5) < 0)
    // {
    //     perror("Listen failed");
    //     close(sockfd);
    //     exit(1);
    // }

    listen(sockfd, 5);
    // Accept and handle client connections
    while (1)
    {
        printf("\n***** Server waiting for new client connection *****\n");
        clientAddressLength = sizeof(clientAddress);
        newsockfd = accept(sockfd, (struct sockaddr *)&clientAddress, &clientAddressLength);
        // if (newsockfd < 0)
        // {
        //     perror("Accept failed");
        //     continue;
        // }

        // Receive and echo messages from the client
        while (1)
        {
            n = recv(newsockfd, msg, MAXSZ, 0);
            if (n <= 0)
            {
                printf("Client disconnected\n");
                close(newsockfd);
                break;
            }
            msg[n] = '\0'; // Null-terminate the received message
            send(newsockfd, msg, n, 0);

            printf("Received and sent: %s\n", msg);
        }
    }

    // Close the main socket (never reached in this implementation)
    close(sockfd);

    return 0;
}
