#pragma once

#include <netinet/in.h>
#include <stdint.h>

constexpr int backlog = 5;
constexpr int buffLen = 1024;

class Server {
	public:
		Server(const char *ip, uint16_t port);
		~Server();

		void start();

	private:
		int sockFd;
		struct sockaddr_in servAddr;
		bool running;

		void getLn(char *buffer);
};