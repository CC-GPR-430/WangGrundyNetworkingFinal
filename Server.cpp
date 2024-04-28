#include "Server.h"

void Server::RunServer() {

	//Begin listening on the server socket
	listen_sock->Listen();

	//Accept connections from clients
	while (true)
	{
		/*while (!isBothPlayersConnected) {

		}*/
		// a single client, so can call send(), recv(), and even close()
		std::cout << "Waiting for connection...\n";
		Socket conn_sock = listen_sock->Accept();

		// Talk to the client until they hang up.
		while (HandleConnection(conn_sock))
		{
			std::cout << "Accepted connection.\n";
		}

		std::cout << "Other end hung up.\n";
	}
}

bool Server::HandleConnection(Socket& conn_sock)
{
	char buffer[4096];
	int nbytes_recvd = conn_sock.Recv(buffer, sizeof(buffer));

	if (nbytes_recvd == -1) //error check
	{
		perror("recv()");
		std::cout << "Big error occured\n";
		return false;
	}

	// Next, check if client hung up connection.
	if (nbytes_recvd == 0) // recv() returns 0 iff this is the case.
	{
		return false; //quit and start new connection
	}

	// If we made it here, nbyte_recvd > 0.
	std::string msg_str(buffer, nbytes_recvd);
	std::cout << "Received message '" << msg_str << "'\n";

	ReadData(conn_sock);

	// Successful back-and-forth -- ask caller to keep looping.
	return true;
}

void Server::ReadData(Socket& conn_sock) {
	string txt = "we see you, from server\n";
	conn_sock.Send(txt.data(), txt.size());
	//send something back
}