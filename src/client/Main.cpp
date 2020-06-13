#include "Client.hpp"

#include <stdlib.h>

int main(int argc, char const *argv[]) {
	Client *client = new Client();
	client->connectTo("127.0.0.1", 50000);

	// Free allocated memory
	delete client;
	return EXIT_SUCCESS;
}