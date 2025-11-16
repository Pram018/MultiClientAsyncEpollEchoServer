#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

Server::Server(int port, ThreadPool& pool)
    : port_(port), threadPool_(pool) {}

void Server::start() {
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ == -1) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Allow quick restart without "Address already in use"
    int opt = 1;
    setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);

    if (bind(server_fd_, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd_, SOMAXCONN) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << port_ << std::endl;

    while (true) {
        socklen_t addrlen = sizeof(address);
        int client_fd = accept(server_fd_, (struct sockaddr*)&address, &addrlen);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        // Hand off client work to the thread pool
        threadPool_.enqueue([this, client_fd]() {
            handleClient(client_fd);
        });
    }
}

void Server::handleClient(int client_fd) {
    char buffer[1024] = {0};
    int bytes_read = read(client_fd, buffer, sizeof(buffer));
    if (bytes_read <= 0) {
        close(client_fd);
        return;
    }

    std::cout << "Received:\n" << buffer << std::endl;

    const char* response =
        "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    send(client_fd, response, strlen(response), 0);

    close(client_fd);
}
