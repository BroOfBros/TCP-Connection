#pragma once

#include <stdint.h>

constexpr int buffLen = 1024;

class Client {
	public:
		Client();
		~Client();

		void connectTo(const char *ip, uint16_t port);

	private:
		int sockFd;
		bool connected;

		void getLn(char *buffer);
};