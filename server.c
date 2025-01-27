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

                        // Array of extensions you want to support (without dots)
                        const char *extensions[] = {"html", "txt", "css", "png", "py", "php"};
                        size_t num_extensions = sizeof(extensions) / sizeof(extensions[0]);

                        // Look for the placeholder where the links should be inserted
                        char *placeholder = strstr(html_response, "<!-- LINKS_PLACEHOLDER -->"); // <!-- LINKS_PLACEHOLDER --> will be searched in html_response
                        if (placeholder != NULL)
                        {
                            // Generate the links dynamically based on the file types
                            char links_section[1024] = "<div class='file-types'>";

                            // Create a structure to hold categorized files
                            char categorized_files[256][256] = {0};

                            struct dirent *entry; // this structure is used for directory listing/exploring
                            while ((entry = readdir(dir)) != NULL)
                            {
                                // Loop through all the supported extensions
                                for (size_t i = 0; i < num_extensions; ++i)
                                {
                                    // Check if the file matches the current extension and exclude default.html
                                    const char *ext = strrchr(entry->d_name, '.'); // Find the last dot in the filename
                                    if (ext != NULL)
                                    {
                                        ext++;                                                                             // Skip the dot
                                        if (strcmp(ext, extensions[i]) == 0 && strcmp(entry->d_name, "default.html") != 0) // strcmp-compares strings
                                        {
                                            // Add the file to the appropriate category
                                            snprintf(categorized_files[i] + strlen(categorized_files[i]), sizeof(categorized_files[i]) - strlen(categorized_files[i]),
                                                     "<li> <a href='%s' target='_blank'>%s</a></li>", entry->d_name, entry->d_name);
                                            break; // Exit loop once matching extension is found
                                        }
                                    }
                                }
                            }

                            // Add the categorized files to the HTML
                            for (size_t i = 0; i < num_extensions; ++i)
                            {
                                if (strlen(categorized_files[i]) > 0)
                                {
                                    snprintf(links_section + strlen(links_section), sizeof(links_section) - strlen(links_section),
                                             "<div class='file-category'><br><strong>%s:</strong><br>%s</div>", extensions[i], categorized_files[i]);
                                }
                            }

                            strcat(links_section, "</div>");

                            // Replace the placeholder with the dynamically generated links
                            char buffer[4096];
                            snprintf(buffer, sizeof(buffer), "%.*s%s%s", (int)(placeholder - html_response), html_response, links_section, placeholder + strlen("<!-- LINKS_PLACEHOLDER -->")); // snprintf is used to combine multiple parts of data to form a string
                            strcpy(html_response, buffer);                                                                                                                                      // strcpy-copies data from 2nd parameter to 1st
                        }

                        closedir(dir); // Close the directory

                        // Send the response header
                        const char *header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
                        send(acceptingSocket, header, strlen(header), 0);

                        // Send the dynamically modified HTML content
                        send(acceptingSocket, html_response, strlen(html_response), 0);
                    }
                }
            }

            else if (strstr(path, ".php")) // Handle PHP CGI script
            {
                // Construct the command to execute the PHP script
                char command[256];
                snprintf(command, sizeof(command), "php -d display_errors=1 -d error_reporting=E_ALL %s", path + 1);

                // Execute the script using popen and capture the output
                FILE *fp = popen(command, "r"); // popen- opens the pipe for process (pipe is a unidirectional way of communication)
                if (fp == NULL)
                {
                    perror("Error executing CGI script");
                    const char *errorResponse = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\n\r\n"
                                                "<html><body><h1>500 Internal Server Error</h1><p>Could not execute script.</p></body></html>";
                    send(acceptingSocket, errorResponse, strlen(errorResponse), 0);
                }
                else
                {
                    const char *header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
                    send(acceptingSocket, header, strlen(header), 0);

                    char buffer[1024];
                    size_t bytes;
                    while ((bytes = fread(buffer, 1, sizeof(buffer), fp)) > 0)
                    {
                        send(acceptingSocket, buffer, bytes, 0);
                    }
                    pclose(fp); // Close the process
                }
            }

            //            else if (strstr(path, ".py") || strstr(path, ".php")) // Handle CGI scripts
            // {
            //     // Construct the command to execute the script
            //     char command[256];
            //     if (strstr(path, ".py"))
            //     {
            //         snprintf(command, sizeof(command), "python3 %s", path + 1);
            //     }
            //     else if (strstr(path, ".php"))
            //     {
            //         // Add flags to PHP command to ensure error reporting
            //         snprintf(command, sizeof(command), "php -d display_errors=1 -d error_reporting=E_ALL %s", path + 1);
            //     }

            //     // Execute the script using popen and capture the output
            //     FILE *fp = popen(command, "r");
            //     if (fp == NULL)
            //     {
            //         perror("Error executing CGI script");
            //         const char *errorResponse = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\n\r\n"
            //                                     "<html><body><h1>500 Internal Server Error</h1><p>Could not execute script.</p></body></html>";
            //         send(acceptingSocket, errorResponse, strlen(errorResponse), 0);
            //     }
            //     else
            //     {
            //         const char *header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
            //         send(acceptingSocket, header, strlen(header), 0);

            //         char buffer[1024];
            //         size_t bytes;
            //         while ((bytes = fread(buffer, 1, sizeof(buffer), fp)) > 0)
            //         {
            //             send(acceptingSocket, buffer, bytes, 0);
            //         }
            //         pclose(fp); // Close the process
            //     }
            // }

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
