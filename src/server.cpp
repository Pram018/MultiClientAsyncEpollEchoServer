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



int main() {

	// Creating a socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1)
	{
		std::cerr << "Socket failed";
		exit(EXIT_FAILURE);
	}
	
	// Making it  a non blocking
	int flags = fcntl(sock,F_GETFL, 0);
	if(flags == -1) 
	{
		std::cerr << "fcntl(F_GETFL) failed";
		exit(EXIT_FAILURE);
	}
	
	// Declaring the port
	const int PORT = 54000;

	// bind it to a port
	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(54000);
	address.sin_addr.s_addr = INADDR_ANY;

	if(bind(sock, (struct sockaddr*)&aaddress, size0f(address)) == -1) 
	{
		std::cerr << "bind failed";
		exit(EXIT_FAILURE);
	}

	// mark the socket for listening in
	if(listen(sock, SOMAXCONN) == 1)
	{
		std::cerr << "Can't listen";
		exit(EXIT_FAILURE);
	}

	// Creating a epool instance
	int epoll_fd = epoll_create1(0);
	if(epoll_fd == -1) 
	{
		throw std::runtime_error("Failed to connect epoll instance");
	}

	// Adding the listening sockets to epoll
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = sock;

	if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &ev) == -1)
	{
		throw std::runtime_error("failes to add sock to epoll");
	}





















