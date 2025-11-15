#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <cstdio>

#define PORT 8080

int main() 
{
	int server_fd, client_fd;

	struct sockaddr_in address;
	int addtlen = sizeof(address);
	char buffer[1024] = {0};

	// creating a socket 
	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if(server_fd == -1) {
		perror("Socket failed");
		exit(EXIT_FAILURE);
	}
	
	// binding the socket
	if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	// listening
	if(listen(server_fd, SOMAXCONN) < 0) {
		fprintf(stderr, "listeining failed: %s \n", strerror(err))
		exit(EXIT_FAILURE);
	}


	// accepting the incoming requests
	while(true) {
		client_fd = accept();


	






	


