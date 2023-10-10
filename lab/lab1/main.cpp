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

int messageNumber = 0;
std::string BuildDefaultResponse() {
	messageNumber++;
	return BuildResponse(
			std::format(R"(<!DOCTYPE html>
<html>
<head>
<title>Page Title</title>
</head>
<body>

<h1>This is Hello World!</h1>
<p>This is a paragraph.</p>
{}

<br/>
<p>Lorem Ipsum</p>
[32] Sed ut perspiciatis, unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam eaque ipsa, quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt, explicabo. Nemo enim ipsam voluptatem, quia voluptas sit, aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos, qui ratione voluptatem sequi nesciunt, neque porro quisquam est, qui dolorem ipsum, quia dolor sit amet consectetur adipisci[ng] velit, sed quia non numquam [do] eius modi tempora inci[di]dunt, ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum[d] exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? [D]Quis autem vel eum i[r]ure reprehenderit, qui in ea voluptate velit esse, quam nihil molestiae consequatur, vel illum, qui dolorem eum fugiat, quo voluptas nulla pariatur?

[33] At vero eos et accusamus et iusto odio dignissimos ducimus, qui blanditiis praesentium voluptatum deleniti atque corrupti, quos dolores et quas molestias excepturi sint, obcaecati cupiditate non provident, similique sunt in culpa, qui officia deserunt mollitia animi, id est laborum et dolorum fuga. Et harum quidem reru[d]um facilis est e[r]t expedita distinctio. Nam libero tempore, cum soluta nobis est eligendi optio, cumque nihil impedit, quo minus id, quod maxime placeat facere possimus, omnis voluptas assumenda est, omnis dolor repellend[a]us. Temporibus autem quibusdam et aut officiis debitis aut rerum necessitatibus saepe eveniet, ut et voluptates repudiandae sint et molestiae non recusandae. Itaque earum rerum hic tenetur a sapiente delectus, ut aut reiciendis voluptatibus maiores alias consequatur aut perferendis doloribus asperiores repellat.

[32] But I must explain to you how all this mistaken idea of reprobating pleasure and extolling pain arose. To do so, I will give you a complete account of the system, and expound the actual teachings of the great explorer of the truth, the master-builder of human happiness. No one rejects, dislikes or avoids pleasure itself, because it is pleasure, but because those who do not know how to pursue pleasure rationally encounter consequences that are extremely painful. Nor again is there anyone who loves or pursues or desires to obtain pain of itself, because it is pain, but occasionally circumstances occur in which toil and pain can procure him some great pleasure. To take a trivial example, which of us ever undertakes laborious physical exercise, except to obtain some advantage from it? But who has any right to find fault with a man who chooses to enjoy a pleasure that has no annoying consequences, or one who avoids a pain that produces no resultant pleasure?

[33] On the other hand, we denounce with righteous indignation and dislike men who are so beguiled and demoralized by the charms of pleasure of the moment, so blinded by desire, that they cannot foresee the pain and trouble that are bound to ensue; and equal blame belongs to those who fail in their duty through weakness of will, which is the same as saying through shrinking from toil and pain. These cases are perfectly simple and easy to distinguish. In a free hour, when our power of choice is untrammeled and when nothing prevents our being able to do what we like best, every pleasure is to be welcomed and every pain avoided. But in certain circumstances and owing to the claims of duty or the obligations of business it will frequently occur that pleasures have to be repudiated and annoyances accepted. The wise man therefore always holds in these matters to this principle of selection: he rejects pleasures to secure other greater pleasures, or else he endures pains to avoid worse pains.
[32] Sed ut perspiciatis, unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam eaque ipsa, quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt, explicabo. Nemo enim ipsam voluptatem, quia voluptas sit, aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos, qui ratione voluptatem sequi nesciunt, neque porro quisquam est, qui dolorem ipsum, quia dolor sit amet consectetur adipisci[ng] velit, sed quia non numquam [do] eius modi tempora inci[di]dunt, ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum[d] exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? [D]Quis autem vel eum i[r]ure reprehenderit, qui in ea voluptate velit esse, quam nihil molestiae consequatur, vel illum, qui dolorem eum fugiat, quo voluptas nulla pariatur?

[33] At vero eos et accusamus et iusto odio dignissimos ducimus, qui blanditiis praesentium voluptatum deleniti atque corrupti, quos dolores et quas molestias excepturi sint, obcaecati cupiditate non provident, similique sunt in culpa, qui officia deserunt mollitia animi, id est laborum et dolorum fuga. Et harum quidem reru[d]um facilis est e[r]t expedita distinctio. Nam libero tempore, cum soluta nobis est eligendi optio, cumque nihil impedit, quo minus id, quod maxime placeat facere possimus, omnis voluptas assumenda est, omnis dolor repellend[a]us. Temporibus autem quibusdam et aut officiis debitis aut rerum necessitatibus saepe eveniet, ut et voluptates repudiandae sint et molestiae non recusandae. Itaque earum rerum hic tenetur a sapiente delectus, ut aut reiciendis voluptatibus maiores alias consequatur aut perferendis doloribus asperiores repellat.

[32] But I must explain to you how all this mistaken idea of reprobating pleasure and extolling pain arose. To do so, I will give you a complete account of the system, and expound the actual teachings of the great explorer of the truth, the master-builder of human happiness. No one rejects, dislikes or avoids pleasure itself, because it is pleasure, but because those who do not know how to pursue pleasure rationally encounter consequences that are extremely painful. Nor again is there anyone who loves or pursues or desires to obtain pain of itself, because it is pain, but occasionally circumstances occur in which toil and pain can procure him some great pleasure. To take a trivial example, which of us ever undertakes laborious physical exercise, except to obtain some advantage from it? But who has any right to find fault with a man who chooses to enjoy a pleasure that has no annoying consequences, or one who avoids a pain that produces no resultant pleasure?

[33] On the other hand, we denounce with righteous indignation and dislike men who are so beguiled and demoralized by the charms of pleasure of the moment, so blinded by desire, that they cannot foresee the pain and trouble that are bound to ensue; and equal blame belongs to those who fail in their duty through weakness of will, which is the same as saying through shrinking from toil and pain. These cases are perfectly simple and easy to distinguish. In a free hour, when our power of choice is untrammeled and when nothing prevents our being able to do what we like best, every pleasure is to be welcomed and every pain avoided. But in certain circumstances and owing to the claims of duty or the obligations of business it will frequently occur that pleasures have to be repudiated and annoyances accepted. The wise man therefore always holds in these matters to this principle of selection: he rejects pleasures to secure other greater pleasures, or else he endures pains to avoid worse pains.
[32] Sed ut perspiciatis, unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam eaque ipsa, quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt, explicabo. Nemo enim ipsam voluptatem, quia voluptas sit, aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos, qui ratione voluptatem sequi nesciunt, neque porro quisquam est, qui dolorem ipsum, quia dolor sit amet consectetur adipisci[ng] velit, sed quia non numquam [do] eius modi tempora inci[di]dunt, ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum[d] exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? [D]Quis autem vel eum i[r]ure reprehenderit, qui in ea voluptate velit esse, quam nihil molestiae consequatur, vel illum, qui dolorem eum fugiat, quo voluptas nulla pariatur?

[33] At vero eos et accusamus et iusto odio dignissimos ducimus, qui blanditiis praesentium voluptatum deleniti atque corrupti, quos dolores et quas molestias excepturi sint, obcaecati cupiditate non provident, similique sunt in culpa, qui officia deserunt mollitia animi, id est laborum et dolorum fuga. Et harum quidem reru[d]um facilis est e[r]t expedita distinctio. Nam libero tempore, cum soluta nobis est eligendi optio, cumque nihil impedit, quo minus id, quod maxime placeat facere possimus, omnis voluptas assumenda est, omnis dolor repellend[a]us. Temporibus autem quibusdam et aut officiis debitis aut rerum necessitatibus saepe eveniet, ut et voluptates repudiandae sint et molestiae non recusandae. Itaque earum rerum hic tenetur a sapiente delectus, ut aut reiciendis voluptatibus maiores alias consequatur aut perferendis doloribus asperiores repellat.

[32] But I must explain to you how all this mistaken idea of reprobating pleasure and extolling pain arose. To do so, I will give you a complete account of the system, and expound the actual teachings of the great explorer of the truth, the master-builder of human happiness. No one rejects, dislikes or avoids pleasure itself, because it is pleasure, but because those who do not know how to pursue pleasure rationally encounter consequences that are extremely painful. Nor again is there anyone who loves or pursues or desires to obtain pain of itself, because it is pain, but occasionally circumstances occur in which toil and pain can procure him some great pleasure. To take a trivial example, which of us ever undertakes laborious physical exercise, except to obtain some advantage from it? But who has any right to find fault with a man who chooses to enjoy a pleasure that has no annoying consequences, or one who avoids a pain that produces no resultant pleasure?

[33] On the other hand, we denounce with righteous indignation and dislike men who are so beguiled and demoralized by the charms of pleasure of the moment, so blinded by desire, that they cannot foresee the pain and trouble that are bound to ensue; and equal blame belongs to those who fail in their duty through weakness of will, which is the same as saying through shrinking from toil and pain. These cases are perfectly simple and easy to distinguish. In a free hour, when our power of choice is untrammeled and when nothing prevents our being able to do what we like best, every pleasure is to be welcomed and every pain avoided. But in certain circumstances and owing to the claims of duty or the obligations of business it will frequently occur that pleasures have to be repudiated and annoyances accepted. The wise man therefore always holds in these matters to this principle of selection: he rejects pleasures to secure other greater pleasures, or else he endures pains to avoid worse pains.
[32] Sed ut perspiciatis, unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam eaque ipsa, quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt, explicabo. Nemo enim ipsam voluptatem, quia voluptas sit, aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos, qui ratione voluptatem sequi nesciunt, neque porro quisquam est, qui dolorem ipsum, quia dolor sit amet consectetur adipisci[ng] velit, sed quia non numquam [do] eius modi tempora inci[di]dunt, ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum[d] exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? [D]Quis autem vel eum i[r]ure reprehenderit, qui in ea voluptate velit esse, quam nihil molestiae consequatur, vel illum, qui dolorem eum fugiat, quo voluptas nulla pariatur?

[33] At vero eos et accusamus et iusto odio dignissimos ducimus, qui blanditiis praesentium voluptatum deleniti atque corrupti, quos dolores et quas molestias excepturi sint, obcaecati cupiditate non provident, similique sunt in culpa, qui officia deserunt mollitia animi, id est laborum et dolorum fuga. Et harum quidem reru[d]um facilis est e[r]t expedita distinctio. Nam libero tempore, cum soluta nobis est eligendi optio, cumque nihil impedit, quo minus id, quod maxime placeat facere possimus, omnis voluptas assumenda est, omnis dolor repellend[a]us. Temporibus autem quibusdam et aut officiis debitis aut rerum necessitatibus saepe eveniet, ut et voluptates repudiandae sint et molestiae non recusandae. Itaque earum rerum hic tenetur a sapiente delectus, ut aut reiciendis voluptatibus maiores alias consequatur aut perferendis doloribus asperiores repellat.

[32] But I must explain to you how all this mistaken idea of reprobating pleasure and extolling pain arose. To do so, I will give you a complete account of the system, and expound the actual teachings of the great explorer of the truth, the master-builder of human happiness. No one rejects, dislikes or avoids pleasure itself, because it is pleasure, but because those who do not know how to pursue pleasure rationally encounter consequences that are extremely painful. Nor again is there anyone who loves or pursues or desires to obtain pain of itself, because it is pain, but occasionally circumstances occur in which toil and pain can procure him some great pleasure. To take a trivial example, which of us ever undertakes laborious physical exercise, except to obtain some advantage from it? But who has any right to find fault with a man who chooses to enjoy a pleasure that has no annoying consequences, or one who avoids a pain that produces no resultant pleasure?

[33] On the other hand, we denounce with righteous indignation and dislike men who are so beguiled and demoralized by the charms of pleasure of the moment, so blinded by desire, that they cannot foresee the pain and trouble that are bound to ensue; and equal blame belongs to those who fail in their duty through weakness of will, which is the same as saying through shrinking from toil and pain. These cases are perfectly simple and easy to distinguish. In a free hour, when our power of choice is untrammeled and when nothing prevents our being able to do what we like best, every pleasure is to be welcomed and every pain avoided. But in certain circumstances and owing to the claims of duty or the obligations of business it will frequently occur that pleasures have to be repudiated and annoyances accepted. The wise man therefore always holds in these matters to this principle of selection: he rejects pleasures to secure other greater pleasures, or else he endures pains to avoid worse pains.

</body>
</html>)", messageNumber));
}







void tcperror(const char*txt) {
	fprintf(stderr, "Err: %s\n", txt);
	fflush(stderr);
	exit(77);
}



std::set<int> connectionSockets;


void Loop(int listenSocket);
int globalListenSocket = -1;

volatile bool flagExit = false;

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

	if(bind(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
		tcperror("Bind()");
		exit(3);
	}
	
	std::thread t = std::thread([]()->void{
			char buf[128];
			scanf("%10s", buf);
			printf("Graceful exit with keyboard input.\n");
			flagExit = true;
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
	if(listen(s, 64) != 0) {
		tcperror("Listen()");
		exit(4);
	}
	
	uint64_t iterationsCount = 0;
	while(!flagExit) {
		iterationsCount++;
		printf("Iteration: %lu\n", iterationsCount);
		fflush(stdout);
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
	uint64_t lastSent;
};
std::map<int, SendData> sendData;
std::map<int, std::string> receivedData;

void Loop(int listenSocket) {
	fd_set fdRead, fdWrite, fdExcept;
	int maxSD = 0;
	
	FD_ZERO(&fdRead);
	FD_ZERO(&fdWrite);
	FD_ZERO(&fdExcept);
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
// 	timeout.tv_sec = 2;
	timeout.tv_sec = 0;
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
				close(socket);
				receivedData.erase(socket);
				sendData.erase(socket);
				connectionSockets.erase(socket);
				continue;
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
			if(data.lastSent + CLOCKS_PER_SEC / 100 + 1 < clock()) {
				const char* str = data.response.c_str() + data.sendBytes;
				const size_t bytesToSend = std::min<size_t>(data.response.size() - data.sendBytes, 1);
				ssize_t bytes = send(socket, str, bytesToSend, MSG_NOSIGNAL);
				if(bytes < 0) {
					printf("Receive error, Closing socket %i\n", socket);
					close(socket);
					receivedData.erase(socket);
					sendData.erase(socket);
					connectionSockets.erase(socket);
					continue;
				} else if(bytes > 0) {
					data.sendBytes += bytes;
				} else {
					printf("Send 0 bytes.\n");
				}
				data.lastSent = clock()+1;
			}
		}
		if(FD_ISSET(socket, &fdExcept)) {
			printf("Don't know what it does. Trying to disconnect. %i\n", socket);
			++handled;
			close(socket);
			receivedData.erase(socket);
			sendData.erase(socket);
			connectionSockets.erase(socket);
			continue;
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
			sendData[socket].lastSent = clock();
		}
	}
	printf("\nhandled %i / %i, open connections: %i\n", handled, selectedCount, connectionSockets.size());
}


