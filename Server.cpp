#include "Server.h"

void Server::RunServer() {

	//Begin listening on the server socket
	listen_sock->Listen();

	//Accept connections from clients
	while (true)
	{
		Socket* player1 = nullptr;
		Socket* player2 = nullptr;

		int playerCount = 0;

		while (true) {
			//keep accepting connections
			std::cout << "Waiting for connection...\n";

			if (playerCount == 0) {
				player1 = new Socket(listen_sock->Accept()); //hangs to wait for p1
				cout << "Accepted connection\n";

				playerCount++;

				std::string asd = "HELLO PLAYER 1\n";
				player1->Send(asd.data(), asd.size());
			}
			else {
				player2 = new Socket(listen_sock->Accept()); //hangs to wait for p2
				cout << "Accepted connection\n";

				playerCount++;

				std::string asd = "HELLO PLAYER 2\n";
				player2->Send(asd.data(), asd.size());
			}

			if (playerCount == 2) {
				std::cout << "both players connected\n";
				break;
			}
			std::cout << "player joined \n";
		}
		
		//send a message to let the player know they are connected
		string tempS = "You are connected to WANG'S SERVER\n";
		player1->Send(tempS.data(), tempS.size());
		player2->Send(tempS.data(), tempS.size());

		// Talk to the client until they hang up.
		bool isActiveConnection = true;
		while (true)
		{
			isActiveConnection = TryRecv(*player1);

			if (!isActiveConnection) {
				break;
			}

			isActiveConnection = TryRecv(*player2);

			if (!isActiveConnection) {
				break;
			}

			isActiveConnection = TrySend(*player1);
			isActiveConnection = TrySend(*player2);
		}

		std::cout << "Other end hung up.\n";
	}
}

void Server::ReadData(Socket& conn_sock) {
	//string txt = "we see you, from server\n";
	//conn_sock.Send(txt.data(), txt.size());
	//send something back
}

bool Server::PlayerLobby(Socket& conn_sock)
{
	char buffer[4096];
	int nbytes_recvd = conn_sock.Recv(buffer, sizeof(buffer));

	if (nbytes_recvd == -1 || nbytes_recvd == 0)
	{
		return false;
	}

	// If we made it here, nbyte_recvd > 0.
	std::string msg_str(buffer, nbytes_recvd);
	std::cout << "Received message '" << msg_str << "'\n";

	std::string response = "SERVER HAS NOTICED YOU!\n";

	float wait_time = consts::INITIAL_TIMEOUT;


	for (int i = 0; i < 10; i++) {
		std::cout << "sending message to client\n";
		conn_sock.Send(response.data(), response.size());
	}
	
	if (msg_str == "ready") {
		std::cout << "server sending connection message to player\n";
		conn_sock.Send(response.data(), response.size());
		std::cout << "nothing wrong with send\n";
	}

	// Successful back-and-forth -- ask caller to keep looping.
	return true;
}

bool Server::TryRecv(Socket& conn_sock)
{
	char buffer[4096];
	int nbytes_recvd = conn_sock.Recv(buffer, sizeof(buffer));

	if (nbytes_recvd == -1 || nbytes_recvd == 0) //error check
	{
		return false;
	}

	DeserializeGameObjectAsBytes(&serverPlayer ,buffer, sizeof(buffer));

	std::cout << "Player X = '" << serverPlayer.position.x << "'\n";

	ReadData(conn_sock);

	// Successful back-and-forth -- ask caller to keep looping.
	return true;
}

bool Server::TrySend(Socket& conn_sock)
{
	string str = "Try Send TEST!!!\n";
	conn_sock.Send(str.data(), str.size());

	return true; // continue
}

//bool Server::TryRecv(Socket& conn_sock)
//{
//	char buffer[4096];
//	int nbytes_recvd = conn_sock.Recv(buffer, sizeof(buffer));
//
//	if (nbytes_recvd == -1 || nbytes_recvd == 0) //error check
//	{
//		return false;
//	}
//
//	// If we made it here, nbyte_recvd > 0.
//	std::string msg_str(buffer, nbytes_recvd);
//	std::cout << "Received message '" << msg_str << "'\n";
//
//	ReadData(conn_sock);
//
//	// Successful back-and-forth -- ask caller to keep looping.
//	return true;
//}