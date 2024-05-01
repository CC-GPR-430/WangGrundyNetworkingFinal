#pragma once
#include <socklib.h>
#include <iostream>
#include <vector>

using namespace std;

class Server {
public:
	void Init() {
		//create a TCP socket for the server to use
		listen_sock = new Socket(Socket::Family::INET, Socket::Type::STREAM);

		//bind this socket to the provided host and port
		address = new Address(HOST, PORT);
		(*listen_sock).Bind(*address);
	}

	~Server() {
		delete address;
		delete listen_sock;
	}

	void RunServer();

private:
	const char* HOST = "127.0.0.1";
	const int PORT = 12345;
	Address* address = nullptr;
	Socket* listen_sock = nullptr;
	bool isBothPlayersConnected = false;

	//vector<Socket*> connSocks;

	bool HandleConnection(Socket& conn_sock);
	void ReadData(Socket& conn_sock);
	bool PlayerLobby(Socket& conn_sock);
};