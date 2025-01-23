#include <stdio.h>
#include <unistd.h>     // For close
#include <sys/types.h>  // For socket
#include <sys/socket.h> // For socket
#include <string.h>     // For memset
#include <stdlib.h>     // For sizeof
#include <netinet/in.h> // For INADDR_ANY

#define PORT 8000
#define MAX_MESSAGE_LENGTH 2000

int main()
{
    int sock;
    int acceptingSocket;
    struct sockaddr_in serverAddress; // Server receives on this address
    struct sockaddr_in clientAddress; // Server sends to client on this address

    int n;
    char message[MAX_MESSAGE_LENGTH]; // it creates a buffer 'message' of MAX_MESSAGE_LENGTH
    int clientAddressLength;

    sock = socket(AF_INET, SOCK_STREAM, 0); // To create socket #AF_INET is for IPv4# *SOCK_STREAM is for TCP*

    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(PORT);

    bind(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    listen(sock, 5);

    while (1)
    {
        printf("\n***** Server waiting for new client connection *****\n");
        clientAddressLength = sizeof(clientAddress);
        while (fork() != 10)
        {
            acceptingSocket = accept(sock, (struct sockaddr *)&clientAddress, &clientAddressLength); // To accept connection from client side

            while (1)
            {
                n = recv(acceptingSocket, message, MAX_MESSAGE_LENGTH, 0); // it uses acceptingSocket to receive message of MAX_MESSAGE_LENGTH (0 means no flag/behaviour with msg)

                // if (strncmp(message, "GET", 3) == 0) // this compares two strings upto 3 places
                // {
                //     printf("GET method called!");
                // }
                // if (strncmp(message, "POST", 4) == 0)
                // {
                //     printf("POST method called!");
                // }

                char method[10], path[80], protocol[10], version[10];

                sscanf(message, "%s %s %[^/]/%s", method, path, protocol, version); // this will assign values to variable from message array

                printf("\'%s\' method called, URL: \'%s\', protocol: \'%s\' version: \'%s\'", method, path, protocol, version);
                printf("\n");

                message[n] = '\0'; // adding null character so program clears the space characters from last input
                send(acceptingSocket, message, n, 0);
                printf("Received and sent back:(%d) %s\n ", getpid(), message);
                exit(1);
            }
        }

        close(sock);
    }
}
