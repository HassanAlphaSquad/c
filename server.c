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
#define PORT 8000

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

// array of pointers
const char *RESPONSE_HEADERS[COUNT] = {

    "HTTP/1.1 415 Unsupported Media Type\r\n\r\n",
    "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n",
    "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\n\r\n",
    "HTTP/1.1 200 OK\r\nContent-Type: application/javascript\r\n\r\n",
    "HTTP/1.1 200 OK\r\nContent-Type: image/png\r\n\r\n",
};

// array of pointers
const char *EXTENSIONS[COUNT] = {
    "unknown",
    ".html",
    ".css",
    ".js",
    ".png"};

Extension get_extension(const char *src)
{
    for (Extension e = 1; e < COUNT; e++) // Start from 1 to skip "unknown"
    {
        if (strcmp(src, EXTENSIONS[e]) == 0)
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

    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("Error setting socket options");
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

        n = recv(acceptingSocket, message, MAX_MESSAGE_LENGTH, 0);
        if (n < 0)
        {
            perror("Error receiving data");
            close(acceptingSocket);
            continue;
        }

        char method[10], path[80], protocol[10];
        // sscanf(message, "%s /%s %[^/]/%s", method, path, protocol, version); // used previously, it was giving path name with / but that path w/o / was causing issue with open default html
        sscanf(message, "%s %s %s", method, path, protocol);
        printf("Received request: METHOD->%s, PATH->%s, PROTOCOL->%s\n", method, path, protocol);

        if (strncmp(method, "GET", 3) == 0)
        {
            // if (strcmp(path, "") == 0 || strcmp(path, "/") == 0) // Check if path is empty or root path "/"
            // {
            //     // Dynamically list all HTML files in the current directory, excluding default.html
            //     DIR *dir = opendir("."); // Open the current directory
            //     if (dir == NULL)
            //     {
            //         perror("Unable to open directory");
            //         const char *errorResponse = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\n\r\n"
            //                                     "<html><body><h1>500 Internal Server Error</h1><p>Could not open directory.</p></body></html>";
            //         send(acceptingSocket, errorResponse, strlen(errorResponse), 0);
            //     }
            //     else
            //     {
            //         char buffer[1024];
            //         char html_response[4096]; // Adjust buffer size as needed
            //         strcpy(html_response, "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Homepage</title><style>h1{text-align:center; padding-top:30px;} .main{border:2px solid black; width:70%; margin:auto; margin-top:5%; padding:10px;} .links{font-style:italic;}</style></head><body><h1>Welcome to Homepage</h1><div class='main'><p class='heading'>Index</p><ol class='links'>");

            //         struct dirent *entry;
            //         while ((entry = readdir(dir)) != NULL)
            //         {
            //             // Filter for only .html files and exclude default.html
            //             if (strstr(entry->d_name, ".html") != NULL && strcmp(entry->d_name, "default.html") != 0)
            //             {
            //                 snprintf(buffer, sizeof(buffer), "<li><a href='%s' target='_blank'>%s</a></li>", entry->d_name, entry->d_name); // Ensure links point to correct location

            //                 strcat(html_response, buffer);
            //             }
            //         }

            //         strcat(html_response, "</ol></div></body></html>");
            //         closedir(dir); // Close the directory

            //         // Send the response header
            //         const char *header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
            //         send(acceptingSocket, header, strlen(header), 0);
            //         // Send the dynamically generated HTML content
            //         send(acceptingSocket, html_response, strlen(html_response), 0);
            //     }
            // }

            if (strcmp(path, "") == 0 || strcmp(path, "/") == 0) // Check if path is empty or root path "/"
            {
                // Open the directory to list HTML files
                DIR *dir = opendir(".");
                if (dir == NULL)
                {
                    perror("Unable to open directory");
                    const char *errorResponse = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\n\r\n"
                                                "<html><body><h1>500 Internal Server Error</h1><p>Could not open directory.</p></body></html>";
                    send(acceptingSocket, errorResponse, strlen(errorResponse), 0);
                }
                else
                {
                    FILE *file = fopen("default.html", "r"); // Open default.html for reading
                    if (file == NULL)
                    {
                        perror("Error opening default.html");
                        const char *errorResponse = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\n\r\n"
                                                    "<html><body><h1>500 Internal Server Error</h1><p>Could not open default.html.</p></body></html>";
                        send(acceptingSocket, errorResponse, strlen(errorResponse), 0);
                    }
                    else
                    {
                        // Read the contents of default.html
                        char html_response[4096]; // Buffer for HTML content
                        int bytesRead = fread(html_response, 1, sizeof(html_response), file);
                        html_response[bytesRead] = '\0'; // Null-terminate the string
                        fclose(file);                    // Close the file after reading

                        // Look for the placeholder where the links should be inserted
                        char *placeholder = strstr(html_response, "<!-- LINKS_PLACEHOLDER -->"); // <!-- LINKS_PLACEHOLDER --> will be searched in html_response  #strstr searches for 2nd string in 1st string
                        if (placeholder != NULL)
                        {
                            // Generate the links dynamically
                            char links_section[1024] = "<ul class='links'>";

                            struct dirent *entry;
                            while ((entry = readdir(dir)) != NULL)
                            {
                                // Filter for only .html files and exclude default.html
                                if (strstr(entry->d_name, ".html") != NULL && strcmp(entry->d_name, "default.html") != 0)
                                {
                                    snprintf(links_section + strlen(links_section), sizeof(links_section) - strlen(links_section),
                                             "<li><a href='%s' target='_blank'>%s</a></li>", entry->d_name, entry->d_name);
                                }
                            }
                            strcat(links_section, "</ul>");

                            // Replace the placeholder with the dynamically generated links
                            char buffer[4096];
                            snprintf(buffer, sizeof(buffer), "%.*s%s%s", (int)(placeholder - html_response), html_response, links_section, placeholder + strlen("<!-- LINKS_PLACEHOLDER -->"));
                            strcpy(html_response, buffer);
                        }

                        closedir(dir); // Close the directory

                        // Send the response header
                        const char *header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
                        // header = RESPONSE_HEADERS[get_extension(extension)];
                        send(acceptingSocket, header, strlen(header), 0);

                        // Send the dynamically modified HTML content
                        send(acceptingSocket, html_response, strlen(html_response), 0);
                    }
                }
            }

            else if (search_file(path + 1)) // Remove leading slash for file search
            {
                FILE *file = fopen(path + 1, "rb"); // Open file after removing leading slash
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
                                            "<html><body><title>Page not found</title><h1>404 Not Found</h1><p>The requested file was not found.</p></body></html>";
                send(acceptingSocket, errorResponse, strlen(errorResponse), 0);
            }
        }

        printf("Processed request: %s\n", message);
        close(acceptingSocket);
    }

    close(sock); // Close the server socket after the loop
    return 0;
}
