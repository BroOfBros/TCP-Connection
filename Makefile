# Variables
CPPC = g++
CFLAGS = -Wall
DFLAGS = -Wall -g3
VFLAGS = --tool=memcheck --leak-check=full --track-origins=yes --gen-suppressions=yes

# Server variables
SERVER_SRC = $(shell find src/server/ -name '*.cpp')
SERVER_BIN = server

# General rules
all: build_server

build_server: $(SERVER_SRC) $(LOGGER_SRC)
	$(CPPC) -o $(SERVER_BIN) $(CFLAGS) $^

# Custom rules
debug_server: $(SERVER_SRC)
	$(CPPC) -o $(SERVER_BIN) $(DFLAGS) $^
	valgrind $(VFLAGS) ./$(SERVER_BIN)

clean:
	rm -rf $(SERVER_BIN)