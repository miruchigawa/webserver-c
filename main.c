#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define ADDRESS INADDR_ANY
#define PORT 8080
#define MAX_BUFFER 1024

void handle_connection(int client_fd) {
    FILE *file = fopen("index.html", "r");
    char html[MAX_BUFFER];
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }
    
    size_t html_len = fread(html, 1, MAX_BUFFER, file);
    fclose(file);
    
    char header[100];
    sprintf(header, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %zu\r\n\r\n", html_len);
    send(client_fd, header, strlen(header) +1, 0);
    send(client_fd, html, strlen(html) +1, 0);
}

int main() {
    int socket_fd;
    struct sockaddr_in socket_addr;
    
    // Creating Socket Connection
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Failed create socket");
        exit(EXIT_FAILURE);
    }
    
    // Setup where socket listening ex server address and port
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_addr.s_addr = ADDRESS;
    socket_addr.sin_port = htons(PORT);
    
    // Bind sockwt to server socket
    if ((bind(socket_fd, (struct sockaddr*)&socket_addr, sizeof(socket_addr))) < 0){
        perror("Failed to bind socket");
        exit(EXIT_FAILURE);
    }
    
    // Listen connection
    if ((listen(socket_fd, 10)) < 0) {
        perror("Failed to listen socket");
        exit(EXIT_FAILURE);
    }
    
    puts("Server listening on port 8080");
    
    for (;;) {
        int client_fd;
        struct sockaddr_in client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        
        // Accept client connection
        if ((client_fd = accept(socket_fd, (struct sockaddr*)&client_addr, &client_addr_size)) < 0) {
            perror("Failed to accept connection");
            continue;
        }
        
        puts("Get new connection");
        // Send response
        handle_connection(client_fd);
    }
    
    return 0;
}