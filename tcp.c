#include <stdio.h>
#include <unistd.h>     // For close
#include <sys/types.h>  // For socket
#include <sys/socket.h> // For socket
#include <string.h>     // For memset
#include <stdlib.h>     // For sizeof
#include <netinet/in.h> // For INADDR_ANY
#include <dirent.h>
#include <sys/stat.h>

#define PORT 8000
#define MAX_MESSAGE_LENGTH 2000

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

int search_file1(const char *filename)
{
    struct dirent *entry;
    DIR *dp = opendir(".");

    if (dp == NULL)
    {
        perror("Unable to open directory");
        exit(EXIT_FAILURE);
    }

    // Skip the leading '/' in the filename if present
    // const char *actual_filename = (filename[0] == '/') ? filename + 1 : filename; // if there is / on 1st index it will start search from 2nd index

    int found = 0;
    while ((entry = readdir(dp)) != NULL)
    {
        if (strcmp(entry->d_name, filename) == 0)
        {
            printf("File '%s' found in the current directory.\n", filename);
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("File '%s' not found in the current directory.\n", filename);
    }

    closedir(dp);
}

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

            while (fork() != 10)
            {
                n = recv(acceptingSocket, message, MAX_MESSAGE_LENGTH, 0); // it uses acceptingSocket to receive message of MAX_MESSAGE_LENGTH (0 means no flag/behaviour with msg)

                char method[10], path[80], protocol[10], version[10];

                sscanf(message, "%s /%s %[^/]/%s", method, path, protocol, version); // this will assign values to variable from message array

                // printf("METHOD: \'%s\', URL: \'%s\', PROTOCOL: \'%s\', VERSION: \'%s\'", method, path, protocol, version); // this will print information about request
                printf("\n");

                if (strncmp(method, "GET", 3) == 0)
                {
                    if (search_file(path))
                    {
                        FILE *file = fopen(path, "rb"); // Open file in binary mode to handle both text and image files
                        if (file == NULL)
                        {
                            perror("Error opening file");
                        }
                        else
                        {
                            // Get the file extension
                            const char *extension = strrchr(path, '.'); // Find the last dot in the file name

                            const char *header;

                            if (extension != NULL)
                            {
                                if (strcmp(extension, ".html") == 0)
                                {
                                    // Send HTTP response header for HTML files
                                    header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
                                }
                                else if (strcmp(extension, ".png") == 0)
                                {
                                    // Send HTTP response header for PNG files
                                    header = "HTTP/1.1 200 OK\r\nContent-Type: image/png\r\n\r\n";
                                }
                                else
                                {
                                    // Send Unsupported Media Type header if the file type is unsupported
                                    header = "HTTP/1.1 415 Unsupported Media Type\r\n\r\n";
                                }

                                // Send the HTTP response header
                                send(acceptingSocket, header, strlen(header), 0);

                                // Read the file and send its content to the browser
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
                        // If file not found, send 404 error page in HTML format
                        const char *errorResponse = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n"
                                                    "<html><body><h1>404 Not Found</h1><p>The requested file was not found.</p></body></html>";
                        send(acceptingSocket, errorResponse, strlen(errorResponse), 0);
                    }
                }

                message[n] = '\0'; // adding null character so program clears the space characters from last input
                send(acceptingSocket, message, n, 0);
                // printf("Received and sent back:(%d) %s\n ", getpid(), message);
                printf("Received and sent back:%s\n ", message);
                exit(1);
            }
        }

        close(sock);
    }
}
