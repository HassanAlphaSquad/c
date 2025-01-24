#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_MESSAGE_LENGTH 1024
#define PORT 8000

// Function to search for a file in the current directory
int search_file(const char *filename)
{
    struct dirent *entry;
    DIR *dp = opendir(".");

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
            break;
        }
    }

    closedir(dp); // Always close the directory

    if (!found)
    {
        printf("File '%s' not found in the current directory.\n", filename);
    }

    return found; // Return 1 if file is found, otherwise 0
}

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    char message[MAX_MESSAGE_LENGTH];
    socklen_t client_address_length;

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("Error opening socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Error binding socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0)
    {
        perror("Error listening on socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1)
    {
        printf("***** Waiting for new client connection *****\n");

        // Accept a new client connection
        client_address_length = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_length);
        if (client_socket < 0)
        {
            perror("Error accepting connection");
            continue;
        }

        // Fork a new process to handle the client
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("Error forking");
            close(client_socket);
            continue;
        }

        if (pid == 0)
        {                         // Child process
            close(server_socket); // Child doesn't need the listening socket

            // Receive data from the client
            ssize_t n = recv(client_socket, message, MAX_MESSAGE_LENGTH, 0);
            if (n < 0)
            {
                perror("Error receiving data");
                close(client_socket);
                exit(EXIT_FAILURE);
            }

            message[n] = '\0'; // Null terminate the received message

            // Parse the HTTP request
            char method[10], path[80], protocol[10], version[10];
            sscanf(message, "%s /%s %[^/]/%s", method, path, protocol, version);

            // Handle GET request
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

                        // Determine content type
                        if (extension != NULL)
                        {
                            if (strcmp(extension, ".html") == 0)
                            {
                                header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
                            }
                            else if (strcmp(extension, ".png") == 0)
                            {
                                header = "HTTP/1.1 200 OK\r\nContent-Type: image/png\r\n\r\n";
                            }
                            else if (strcmp(extension, ".css") == 0)
                            {
                                header = "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\n\r\n";
                            }
                            else
                            {
                                header = "HTTP/1.1 415 Unsupported Media Type\r\n\r\n";
                            }

                            send(client_socket, header, strlen(header), 0);

                            // Send file content
                            char buffer[1024];
                            size_t bytes;
                            while ((bytes = fread(buffer, 1, sizeof(buffer), file)) > 0)
                            {
                                send(client_socket, buffer, bytes, 0);
                            }
                        }

                        fclose(file);
                    }
                }
                else
                {
                    // Send 404 Not Found response
                    const char *error_response =
                        "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n"
                        "<html><body><h1>404 Not Found</h1><p>The requested file was not found.</p></body></html>";
                    send(client_socket, error_response, strlen(error_response), 0);
                }
            }

            printf("Received and processed request: %s\n", message);

            close(client_socket);
            exit(EXIT_SUCCESS); // Exit child process
        }
        else
        {                         // Parent process
            close(client_socket); // Parent doesn't need the accepted socket
        }
    }

    close(server_socket); // Close the listening socket (never reached)
    return 0;
}