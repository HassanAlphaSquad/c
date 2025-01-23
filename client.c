#include <stdio.h>
#include <sys/types.h>  //socket
#include <unistd.h>     // For close
#include <sys/socket.h> //socket
#include <string.h>     //memset
#include <stdlib.h>     //sizeof
#include <netinet/in.h> //INADDR_ANY
#include <arpa/inet.h>  // For inet_addr and other network functions

#define PORT 8000
#define SERVER_IP "127.0.0.1"
#define MAX_MESSAGE_LENGTH 2000

int main()
{
    int sock;

    struct sockaddr_in serverAddress; // client will connect on this

    int n;
    char message1[MAX_MESSAGE_LENGTH];
    char message2[MAX_MESSAGE_LENGTH];

    sock = socket(AF_INET, SOCK_STREAM, 0); // create socket

    // initialize the socket addresses
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddress.sin_port = htons(PORT);

    connect(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress)); // client's connection to server

    // send to sever and receive back message from server

    while (1)
    {
        printf("\nEnter message to send to server:\n");
        fgets(message1, MAX_MESSAGE_LENGTH, stdin);

        n = strlen(message1) + 1;
        send(sock, message1, n, 0);
        n = recv(sock, message2, MAX_MESSAGE_LENGTH, 0);

        printf("Server is replying: %s\n", message2);
    }

    return 0;
}