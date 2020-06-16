# Variables
CPPC = g++
CFLAGS = -Wall
DFLAGS = -Wall -g3
VFLAGS = --tool=memcheck --leak-check=full --track-origins=yes --gen-suppressions=yes

# Server variables
SERVER_SRC = $(shell find src/server/ -name '*.cpp')
SERVER_BIN = server

# Client variables
CLIENT_SRC = $(shell find src/client/ -name '*.cpp')
CLIENT_BIN = client

# General rules
all: build_server build_client

build_server: $(SERVER_SRC)
	$(CPPC) -o $(SERVER_BIN) $(CFLAGS) $^

build_client: $(CLIENT_SRC)
	$(CPPC) -o $(CLIENT_BIN) $(CFLAGS) $^

run_server: build_server
	./$(SERVER_BIN)

run_client: build_client
	./$(CLIENT_BIN)

clean:
	rm -rf $(SERVER_BIN) $(CLIENT_BIN)

# Custom rules
debug_server: $(SERVER_SRC)
	$(CPPC) -o $(SERVER_BIN) $(DFLAGS) $^
	valgrind $(VFLAGS) ./$(SERVER_BIN)

debug_client: $(CLIENT_SRC)
	$(CPPC) -o $(CLIENT_BIN) $(DFLAGS) $^
	valgrind $(VFLAGS) ./$(CLIENT_BIN)