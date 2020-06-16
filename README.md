# TCP_Connection
A simple client-server application with I/O multiplexing.  
This is just an example of C socket programming.  

First you have to start the server with either `make debug_server` or `make run_server`.  
The server will be binded to the loopback address 127.0.0.1 and port 50000.  
Then you can start the clients with either `make debug_client` or `make run_client`.  
To use the debug mode you need to have valgrind previously installed.  

# Server interaction
To keep it simple, the only server command is `exit`, which closes the server and all the  
connected clients. Please note that the server might go into a `TIME_WAIT` state if a client  
closes after the server, in which case you have to wait a few seconds before attempting to  
restart the server. You will observe this behaviour upon receiving the `Address is already in use`  
message.  

# Client interaction
The command to close a client is `disconnect`.  
In order to send a message to the server, you have to issue the `cmd` command. Then, you enter the  
message you wish to send to the server.