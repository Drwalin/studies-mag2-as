#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <format>

#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <vector>

// const std::string headerResponse =
// R"(HTTP/1.1 200 OK
// Date: Mon, 27 Jul 2009 12:28:53 GMT
// Server: Apache/2.2.14 (Win32)
// Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT
// Content-Length: 88
// Content-Type: text/html
// Connection: Closed
// 
// )";
const char* headerResponse =
R"(HTTP/1.1 200 OK
Content-Length: {}
Content-Type: text/html
Connection: Closed

)";

std::string BuildResponse(std::string text) {
	std::string fmt = headerResponse;
	return std::format(R"(HTTP/1.1 200 OK
Content-Length: {}
Content-Type: text/html
Connection: Closed

)", text.length()) + text;
}

std::string BuildDefaultResponse() {
	return BuildResponse(R"(<!DOCTYPE html>
<html>
<head>
<title>Page Title</title>
</head>
<body>

<h1>This is Hello World!</h1>
<p>This is a paragraph.</p>

</body>
</html>)");
}






void tcperror(const char*txt) {
	fprintf(stderr, "Err: %s\n", txt);
	fflush(stderr);
	exit(77);
}



std::vector<int> connectionSockets;

int main(int argc, char**argv) {
	unsigned short port = 8080;/* port server binds to                */
	char buf[12];              /* buffer for sending & receiving data */
	struct sockaddr_in client; /* client address information          */
	struct sockaddr_in server; /* server address information          */
	int s;                     /* socket for accepting connections    */
	int ns;                    /* socket connected to client          */
	int namelen;               /* length of client name               */

	// Get a socket for accepting connections.
	if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		tcperror("Socket()");
		exit(2);
	}

	// Bind the socket to the server address.
	server.sin_family = AF_INET;
	server.sin_port   = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;

	if(bind(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
		tcperror("Bind()");
		exit(3);
	}
	
// 	// set non blocking
// 	int flags = fcntl(s, F_GETFL, 0);
// 	if(flags == -1) {
// 		tcperror("Cannot cannot get flags");
// 	}
// 	if(fcntl(s, F_SETFL, flags | O_NONBLOCK) != 0) {
// 		tcperror("Cannot set non blockin");
// 	}
	
	// Listen for connections. Specify the backlog as 1.
	if(listen(s, 1) != 0) {
		tcperror("Listen()");
		exit(4);
	}

	// Accept a connection.
	namelen = sizeof(client);
	if((ns = accept(s, (struct sockaddr *)&client, (socklen_t*)&namelen)) == -1) {
		tcperror("Accept()");
		exit(5);
	}

// 	// Receive the message on the newly connected socket.
// 	if(recv(ns, buf, sizeof(buf), 0) == -1) {
// 		tcperror("Recv()");
// 		exit(6);
// 	}

	// Send the message back to the client.
	std::string response = BuildDefaultResponse();
	if(send(ns, response.c_str(), response.size(), 0) < 0) {
		tcperror("Send()");
		exit(7);
	}

	close(ns);
	close(s);

	printf("Server ended successfully\n");
	exit(0);
}

void Loop(int listenSocket) {
	
	
	
}







