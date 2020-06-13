#include "Client.hpp"

#include <arpa/inet.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

Client::Client() {
	// Create socket
	sockFd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockFd < 0) {
		perror("Could not create server socket");
	}
}

Client::~Client() {
	close(sockFd);
}

void Client::getLn(char *buffer) {
	memset(buffer, 0, buffLen);
	fgets(buffer, buffLen, stdin);
	buffer[strlen(buffer) - 1] = '\0';
}

void Client::connectTo(const char *ip, uint16_t port) {
	struct sockaddr_in servAddr;

	// Connect to server
	if(port < 1024 || port > 65535) {
		fprintf(stderr, "The port should be in the (1024, 65535) interval\n");
		return;
	}

	if(inet_aton(ip, &servAddr.sin_addr) == 0) {
		perror("Invalid server ip address");
		return;
	}

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(port);

	if(connect(sockFd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr))) {
		perror("Could not connect to server");
		return;
	}

	// Create file descr sets
	fd_set readFds;
	fd_set tmpFds;

	FD_ZERO(&readFds);
	FD_ZERO(&tmpFds);

	int fdMax;

	// Add STDIN to set
	FD_SET(STDIN_FILENO, &readFds);

	// Add sockFd to set
	FD_SET(sockFd, &readFds);
	fdMax = sockFd;

	// Client specific vars
	char *buffer = new char[buffLen];

	// Run client
	connected = true;

	while(connected) {
		tmpFds = readFds;
		select(fdMax + 1, &tmpFds, NULL, NULL, NULL);

		for(int i = 0; i <= fdMax; i++) {
			if(FD_ISSET(i, &tmpFds) == 0) {
				continue;
			}

			// Accept data from STDIN
			if(i == STDIN_FILENO) {
				getLn(buffer);

				if(strcmp(buffer, "disconnect") == 0) {
					printf("Disconnecting from server...\n");
					connected = false;
					break;
				}

				continue;
			}

			// Receive messages from server
		}
	}

	// Free allocated memory
	delete[] buffer;
}