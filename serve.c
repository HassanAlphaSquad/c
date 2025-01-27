#include <stdio.h>
#include <unistd.h>     // For close
#include <sys/types.h>  // For socket
#include <sys/socket.h> // For socket
#include <string.h>     // For memset
#include <stdlib.h>     // For sizeof
#include <netinet/in.h> // For INADDR_ANY
#include <dirent.h>
#include <sys/stat.h>

#define MAX_MESSAGE_LENGTH 1024
#define PORT 8080

int search_file(const char *filename)
{
    struct dirent *entry;
    DIR *dp = opendir("."); // Open the current directory

    if (dp == NULL)
    {
        perror("Unable to open directory");
        return 0; // Return 0 if directory can't be opened
    }

    int found = 0;
    while ((entry = readdir(dp)) != NULL)
    {
        if (strcmp(entry->d_name, filename) == 0)
        {
            found = 1; // File found
            break;     // Exit the loop as the file is found
        }
    }

    closedir(dp); // Always close the directory

    if (!found)
    {
        printf("File '%s' not found in the current directory.\n", filename);
    }

    return found; // Return 1 if file is found, otherwise 0
}

typedef enum
{
    Unknown,
    HTML,
    CSS,
    JS,
    PNG,
    COUNT
} Extension;

const char *RESPONSE_HEADERS[COUNT] = {
    "HTTP/1.1 415 Unsupported Media Type\r\n\r\n",
    "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n",
    "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\n\r\n",
    "HTTP/1.1 200 OK\r\nContent-Type: application/x-javascript\r\n\r\n",
    "HTTP/1.1 200 OK\r\nContent-Type: image/png\r\n\r\n",
};

const char *EXTENTSIONS[COUNT] = {
    "unknown",
    ".html",
    ".css",
    ".js",
    ".png"};

Extension get_extension(const char *src)
{
    Extension e = 1;
    printf("SOURCE -> %s", src);
    while (e != COUNT)
    {
        if (strcmp(src, EXTENTSIONS[e]) == 0)
        {
            return e;
        }
    }
    return Unknown;
}

int main()
{
    int sock;
    int acceptingSocket;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    int n;
    char message[MAX_MESSAGE_LENGTH];
    int clientAddressLength;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Error opening socket");
        exit(1);
    }

    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(PORT);

    if (bind(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("Error binding socket");
        exit(1);
    }

    listen(sock, 5);
    printf("Server listening on port %d...\n", PORT);

    while (1)
    {
        printf("***** Waiting for new client connection *****\n");
        clientAddressLength = sizeof(clientAddress);
        acceptingSocket = accept(sock, (struct sockaddr *)&clientAddress, &clientAddressLength);
        if (acceptingSocket < 0)
        {
            perror("Error accepting connection");
            continue;
        }

        // pid_t pid = fork();
        // if (pid < 0)
        // {
        //     perror("Error forking");
        //     close(acceptingSocket);
        //     continue;
        // }

        if (1)
        { // Child process
            // close(sock); // Child doesn't need the listening socket

            n = recv(acceptingSocket, message, MAX_MESSAGE_LENGTH, 0);
            if (n < 0)
            {
                perror("Error receiving data");
                close(acceptingSocket);
                exit(1);
            }

            char method[10], path[80], protocol[10], version[10];
            sscanf(message, "%s /%s %[^/]/%s", method, path, protocol, version);
            printf("\n");

            if (strncmp(method, "GET", 3) == 0)
            {
                if (search_file(path))
                {
                    FILE *file = fopen(path, "rb");
                    if (file == NULL)
                    {
                        perror("Error opening file");
                    }
                    else
                    {
                        const char *header;
                        const char *extension = strrchr(path, '.');
                        if (extension != NULL)
                        {

                            header = RESPONSE_HEADERS[get_extension(extension)];
                            send(acceptingSocket, header, strlen(header), 0);

                            char buffer[1024];
                            size_t bytes;
                            while ((bytes = fread(buffer, 1, sizeof(buffer), file)) > 0)
                            {
                                send(acceptingSocket, buffer, bytes, 0);
                            }
                        }
                        fclose(file);
                    }
                }
                else
                {
                    const char *errorResponse = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n"
                                                "<html><body><h1>404 Not Found</h1><p>The requested file was not found.</p></body></html>";
                    send(acceptingSocket, errorResponse, strlen(errorResponse), 0);
                }
            }

            message[n] = '\0'; // Null terminate the message
            send(acceptingSocket, message, n, 0);

            printf("Received and sent back: %s\n", message);
            close(acceptingSocket);
            exit(0); // Exit the child process after handling the client
        }
        else
        {                           // Parent process
            close(acceptingSocket); // Parent doesn't need the accepted socket
        }
    }

    close(sock); // Close the listening socket (never reached in this case)
}
