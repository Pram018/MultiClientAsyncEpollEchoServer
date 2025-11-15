#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#define PORT 8080

int main() 
{
	int server_fd, client_fd;

	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);
	char buffer[1024] = {0};

	// creating a socket 
	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if(server_fd == -1) {
		perror("Socket failed");
		exit(EXIT_FAILURE);
	}
	
	//setup address
	address.sin_family =AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// binding the socket
	if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	// listening
	if(listen(server_fd, SOMAXCONN) < 0) {
		perror("listen failed");
		exit(EXIT_FAILURE);
	}

	std::cout << "Server listening on port " << PORT << std::endl;


	// accepting the incoming requests
	while(true) {
		client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen);
		if(client_fd < 0) {
			perror("Accept failed");
			continue;
		}

	//reading the data
	int bytes_read = read(client_fd, buffer, sizeof(buffer));
	std::cout << "Recieved:\n" << buffer << std::endl;

	// Send response
    	const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
	send(client_fd, response, strlen(response), 0);

	// Close connection
	close(client_fd);
	}
	return 0;
}

	






	


