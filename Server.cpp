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

				//accept any incoming messages
				Socket s = listen_sock->Accept();
				cout << "Accepted connection\n";

				while (PlayerLobby(s)) {
					/*std::string response = "SERVER HAS FUCKING FINALLY FOUND YOU!\n";
					std::cout << "server sending connection message to player\n";
					s.Send(response.data(), response.size());*/
				}

				/*while (true) {
					std::cout << "sending message to player 1\n";
					string tempS = "You are connected to WANG'S SERVER\n";
					s.Send(tempS.data(), tempS.size());
				}*/

				//player1 = new Socket();
				playerCount++;

				
			}
			else {
				player2 = new Socket(listen_sock->Accept());
				playerCount++;
			}

			if (playerCount == 2) {
				std::cout << "both players connected\n";
				break;
			}
			std::cout << "player joined \n";
		}
		
		//send a message to let the player know they are connected

		//THIS MESSAGE RETURNED AINT WORKINGGG
		//THIS MESSAGE RETURNED AINT WORKINGGG
		//THIS MESSAGE RETURNED AINT WORKINGGG
		//THIS MESSAGE RETURNED AINT WORKINGGG
		//THIS MESSAGE RETURNED AINT WORKINGGG
		//THIS MESSAGE RETURNED AINT WORKINGGG
		//THIS MESSAGE RETURNED AINT WORKINGGG

		/*while (true) {
			string tempS = "You are connected to WANG'S SERVER\n";
			player1->Send(tempS.data(), tempS.size());
			tempS = "LIAM IS A BITCH\n";
			player1->Send(tempS.data(), tempS.size());
		}*/

		// Talk to the client until they hang up.
		bool isActiveConnection = true;
		while (true)
		{
			/*isActiveConnection = HandleConnection(*player1);

			if (!isActiveConnection) {
				break;
			}

			isActiveConnection = HandleConnection(*player2);

			if (!isActiveConnection) {
				break;
			}*/
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

	std::string response = "SERVER HAS FUCKING FINALLY FOUND YOU!\n";

	/*while (true) {
		std::cout << "server sending connection message to player\n";
		conn_sock.Send(response.data(), response.size());
	}*/

	//EVEN WHEN THIS IS IN A WHILE LOOp, IT ONLY SENDS 1, IDK WHY?
	//EVEN WHEN THIS IS IN A WHILE LOOp, IT ONLY SENDS 1, IDK WHY?
	//EVEN WHEN THIS IS IN A WHILE LOOp, IT ONLY SENDS 1, IDK WHY?
	//EVEN WHEN THIS IS IN A WHILE LOOp, IT ONLY SENDS 1, IDK WHY?
	//EVEN WHEN THIS IS IN A WHILE LOOp, IT ONLY SENDS 1, IDK WHY?
	//EVEN WHEN THIS IS IN A WHILE LOOp, IT ONLY SENDS 1, IDK WHY?
	//EVEN WHEN THIS IS IN A WHILE LOOp, IT ONLY SENDS 1, IDK WHY?
	//EVEN WHEN THIS IS IN A WHILE LOOp, IT ONLY SENDS 1, IDK WHY?
	//EVEN WHEN THIS IS IN A WHILE LOOp, IT ONLY SENDS 1, IDK WHY?

	std::cout << "server sending connection message to player\n";
	conn_sock.Send(response.data(), response.size());
	
	/*std::cout << "server sending connection message to player\n";
	listen_sock->SendAll(response.data(), response.size());*/

	/*if (msg_str == "ready") {
		std::cout << "server sending connection message to player\n";
		conn_sock.Send(response.data(), response.size());
		std::cout << "nothing wrong with send\n";
	}*/

	// Successful back-and-forth -- ask caller to keep looping.
	return true;
}