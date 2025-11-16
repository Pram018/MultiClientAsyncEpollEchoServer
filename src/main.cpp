#include "ThreadPool.h"
#include "server.h"
#include <thread>   //std::thread::hardware_concurrency

int main() {
    const int port = 8080;

    size_t numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4;

    ThreadPool pool(numThreads);
    Server server(port, pool);

    server.start();  // starting accepting connections
    return 0;
}
