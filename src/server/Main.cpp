#include "Server.hpp"

#include <stdlib.h>

int main(int argc, char const *argv[]) {
	Server *server = new Server("127.0.0.1", 50000);
	server->start();

	// Free allocated memory
	delete server;
	return EXIT_SUCCESS;
}