#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

#include <string_view>
#include <string>
#include <format>
#include <vector>
#include <set>
#include <map>
#include <thread>

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



std::set<int> connectionSockets;


void Loop(int listenSocket);
int globalListenSocket = -1;

int main(int argc, char**argv) {
	printf("\n\n\n");
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
	
	globalListenSocket = s;
	atexit([]()->void{close(globalListenSocket);printf("At exit\n");fflush(stdout);});

	if(bind(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
		tcperror("Bind()");
		exit(3);
	}
	
	std::thread t = std::thread([]()->void{
			char buf[128];
			scanf("%10s", buf);
			printf("Graceful exit with keyboard input.\n");
			exit(0);
		});
	
	// set non blocking
	int flags = fcntl(s, F_GETFL, 0);
	if(flags == -1) {
		tcperror("Cannot cannot get flags");
	}
	if(fcntl(s, F_SETFL, flags | O_NONBLOCK) != 0) {
		tcperror("Cannot set non blockin");
	}
	
	// Listen for connections. Specify the backlog as 1.
	if(listen(s, 1) != 0) {
		tcperror("Listen()");
		exit(4);
	}
	
	while(true) {
		Loop(s);
	}
	
	for(int socket : connectionSockets) {
		close(socket);
	}
	close(s);
	
	printf("Server ended successfully\n");
	return 0;
}

struct SendData {
	std::string response;
	size_t sendBytes;
};
std::map<int, SendData> sendData;
std::map<int, std::string> receivedData;

void Loop(int listenSocket) {
	fd_set fdRead, fdWrite, fdExcept;
	int maxSD = 0;
	
	FD_ZERO(&fdRead);
	FD_ZERO(&fdWrite);
	FD_ZERO(&fdExcept);
	FD_ZERO(&fdRead);
	maxSD = listenSocket;
	FD_SET(listenSocket, &fdRead);
	FD_SET(listenSocket, &fdWrite);
	FD_SET(listenSocket, &fdExcept);
	for(int socket : connectionSockets) {
		FD_SET(socket, &fdRead);
		FD_SET(socket, &fdWrite);
		FD_SET(socket, &fdExcept);
		maxSD = std::max(maxSD, socket);
	}
	struct timeval timeout;
	timeout.tv_sec = 2;
	timeout.tv_usec = 1000000/20;
	const int selectedCount = select(maxSD+1, &fdRead, &fdWrite, &fdExcept, &timeout);
	if(selectedCount < 0) {
		return;
	}
	
	const size_t MAX_BUFFER = 1024;
	char buffer[MAX_BUFFER];
	int handled = 0;
	for(int socket : std::vector<int>(connectionSockets.begin(), connectionSockets.end())) {
		if(FD_ISSET(socket, &fdRead)) {
			printf("set read %i\n", socket);
			++handled;
			ssize_t bytes = recv(socket, buffer, MAX_BUFFER, 0);
			if(bytes < 0) {
				printf("Receive error.\n");
			} else {
				receivedData[socket].append(std::string_view(buffer, bytes));
				std::string& str = receivedData[socket];
				printf("\n\n\n`%s`\n\n\n\n", str.c_str());
				printf("%i %i\n\n\n", (int)str[str.size()-2], (int)str[str.size()-1]);
				printf("%i %i\n\n\n", (int)'\n', (int)'\r');
			}
		}
		if(FD_ISSET(socket, &fdWrite)) {
			printf("set write %i\n", socket);
			++handled;
			auto& data = sendData[socket];
			if(data.sendBytes < data.response.size()) {
			} else {
				close(socket);
				receivedData.erase(socket);
				sendData.erase(socket);
				connectionSockets.erase(socket);
				continue;
			}
			const char* str = data.response.c_str() + data.sendBytes;
			const size_t bytesToSend = data.response.size() - data.sendBytes;
			ssize_t bytes = send(socket, str, bytesToSend, 0);
			if(bytes < 0) {
				printf("Receive error.\n");
			} else if(bytes > 0) {
				data.sendBytes += bytes;
			} else {
				printf("Send 0 bytes.\n");
			}
		}
		if(FD_ISSET(socket, &fdExcept)) {
			printf("Don't know what it does. Trying to disconnect. %i\n", socket);
			++handled;
			close(socket);
			receivedData.erase(socket);
			sendData.erase(socket);
			connectionSockets.erase(socket);
		}
	}
	if(FD_ISSET(listenSocket, &fdRead)) {
		printf("Set listen socket read\n");
		++handled;
		struct sockaddr_in client;
		int namelen = sizeof(client);
		int socket = accept(listenSocket, (struct sockaddr*)&client, (socklen_t*)&namelen);
		if(socket == -1) {
			printf("not accepted\n");
			if(errno == EWOULDBLOCK) {
				printf("    would block\n");
			} else {
				tcperror("Accept()");
				exit(5);
			}
		} else {
		printf("    accepted: %i\n", socket);
			connectionSockets.insert(socket);
			
			sendData[socket].response = BuildDefaultResponse();
			sendData[socket].sendBytes = 0;
		}
	}
	printf("\nhandled %i / %i, open connections: %i\n", handled, selectedCount, connectionSockets.size());
}


