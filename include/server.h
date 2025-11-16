#pragma once

#include "ThreadPool.h"

class Server {
public:
    Server(int port, ThreadPool& pool);
    void start();

private:
    int port_;
    int server_fd_;
    ThreadPool& threadPool_;

    void handleClient(int client_fd);
};
