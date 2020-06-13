#include "Server.hpp"

#include <arpa/inet.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

Server::Server(const char *ip, uint16_t port) {
	memset(&servAddr, 0, sizeof(struct sockaddr_in));
	running = false;

	// Create socket
	sockFd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockFd < 0) {
		perror("Could not create server socket");
	} else {
		// Bind address to socket
		if(port > 1023 && port < 65535) {
			if(inet_aton(ip, &servAddr.sin_addr)) {
				servAddr.sin_family = AF_INET;
				servAddr.sin_port = htons(port);

				if(bind(sockFd, (struct sockaddr *) &servAddr, sizeof(struct sockaddr)) < 0) {
					perror("Could not bind address to server socket");
				}
			} else {
				perror("Invalid server ip address");
			}
		}
	}
}

Server::~Server() {
	close(sockFd);
}

void Server::getLn(char *buffer) {
	memset(buffer, 0, buffLen);
	fgets(buffer, buffLen, stdin);
	buffer[strlen(buffer) - 1] = '\0';
}

void Server::start() {
	// Listen for new connections
	listen(sockFd, backlog);

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

	// Server specific vars
	char *buffer = new char[buffLen];

	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen;

	// Start server
	running = true;

	while(running) {
		tmpFds = readFds;
		select(fdMax + 1, &tmpFds, NULL, NULL, NULL);

		for(int i = 0; i <= fdMax; i++) {
			if(FD_ISSET(i, &tmpFds) == 0) {
				continue;
			}

			// Accept data from STDIN
			if(i == STDIN_FILENO) {
				getLn(buffer);

				if(strcmp(buffer, "exit") == 0) {
					printf("Closing server and its connections...\n");
					running = false;
					break;
				}

				continue;
			}

			// Accept new TCP connections
			if(i == sockFd) {
				clientAddrLen = sizeof(struct sockaddr_in);
				memset(&clientAddr, 0, clientAddrLen);

				int client = accept(sockFd, (struct sockaddr *)&clientAddr, &clientAddrLen);

				if(client < 0) {
					perror("Could not accept a new TCP client");
					continue;
				}

				// Add the new client to set
				FD_SET(client, &readFds);
				fdMax = fdMax < client ? client : fdMax;

				printf("A new client has connected from: %s:%d\n",
					   inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

				continue;
			}

			// Receive messages from connected clients
		}
	}

	// Free allocated memory
	delete[] buffer;
}