#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <fcntl.h>
#include <stdexcept>

#define MAX_EVENTS 64
#define PORT 54000
#define BUFFER_SIZE 1024

int main() {
    // Creating a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Socket failed: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Set socket to non-blocking
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags == -1) {
        std::cerr << "fcntl(F_GETFL) failed: " << strerror(errno) << std::endl;
        close(sock);
        exit(EXIT_FAILURE);
    }

    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1) {
        std::cerr << "fcntl(F_SETFL) failed: " << strerror(errno) << std::endl;
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Prepare socket address
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(sock, (struct sockaddr*)&address, sizeof(address)) == -1) {
        std::cerr << "Bind failed: " << strerror(errno) << std::endl;
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(sock, SOMAXCONN) == -1) {
        std::cerr << "Listen failed: " << strerror(errno) << std::endl;
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Create epoll instance
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << "Epoll create failed: " << strerror(errno) << std::endl;
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Prepare epoll event
    struct epoll_event ev, events[MAX_EVENTS];
    ev.events = EPOLLIN | EPOLLET;  // Edge-triggered read events
    ev.data.fd = sock;

    // Add listening socket to epoll
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &ev) == -1) {
        std::cerr << "Epoll control failed: " << strerror(errno) << std::endl;
        close(sock);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];

    // Main event loop
    while (true) {
        int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (num_events == -1) {
            std::cerr << "Epoll wait failed: " << strerror(errno) << std::endl;
            break;
        }

        for (int i = 0; i < num_events; i++) {
            // Listening socket: accept connections
            if (events[i].data.fd == sock) {
                while (true) {
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);

                    int client_fd = accept(sock, (struct sockaddr*)&client_addr, &client_len);
                    if (client_fd == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                            break;
                        std::cerr << "Accept failed: " << strerror(errno) << std::endl;
                        continue;
                    }

                    // Make client non-blocking
                    int cflags = fcntl(client_fd, F_GETFL, 0);
                    fcntl(client_fd, F_SETFL, cflags | O_NONBLOCK);

                    ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
                    ev.data.fd = client_fd;

                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
                        std::cerr << "Failed to add client to epoll: " << strerror(errno) << std::endl;
                        close(client_fd);
                        continue;
                    }

                    char ip_str[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &client_addr.sin_addr, ip_str, sizeof(ip_str));

                    std::cout << "New connection from " 
                              << ip_str << ":" << ntohs(client_addr.sin_port) << std::endl;
                }
            } 
            else {
                int client_fd = events[i].data.fd;
                bool client_alive = true;

                // Read all available data
                while (true) {
                    ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

                    if (bytes_read == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                            break;  // ET mode: all data consumed
                        std::cerr << "Receive failed: " << strerror(errno) << std::endl;
                        client_alive = false;
                        break;
                    }

                    if (bytes_read == 0) {
                        std::cout << "Client disconnected" << std::endl;
                        client_alive = false;
                        break;
                    }

                    buffer[bytes_read] = '\0';
                    std::cout << "Received: " << buffer << std::endl;

                    send(client_fd, buffer, bytes_read, 0);
                }

                if (!client_alive) {
                    close(client_fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, nullptr);
                    continue;
                }

                // Rearm EPOLLONESHOT
                ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
                ev.data.fd = client_fd;
                epoll_ctl(epoll_fd, EPOLL_CTL_MOD, client_fd, &ev);
            }
        }
    }

    close(sock);
    close(epoll_fd);

    return 0;
}

