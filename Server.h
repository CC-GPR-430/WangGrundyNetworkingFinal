#pragma once
#include <socklib.h>
#include <iostream>
#include <vector>
#include "CONSTS.h"
#include "Data.h"
#include "Player.h"

using namespace std;

class Server {
public:
	void Init() {
		//create a TCP socket for the server to use
		listen_sock = new Socket(Socket::Family::INET, Socket::Type::STREAM);

		//bind this socket to the provided host and port
		address = new Address(HOST, PORT);
		listen_sock->Bind(*address);
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

	Player serverPlayer;

	void ReadData(Socket& conn_sock);
	bool PlayerLobby(Socket& conn_sock);
	bool TryRecv(Socket& conn_sock);
	bool TrySend(Socket& conn_sock);
    bool IDCHECK(Socket& conn_sock);

	//

    template <typename T>
    size_t copy_to_buffer(char* buffer, T* object, size_t buffer_size)
    {
        if (sizeof(T) > buffer_size) return 0;
        char* object_as_bytes = (char*)object;
        for (int i = 0; i < sizeof(T); i++) {
            buffer[i] = object_as_bytes[i];
        }
        return sizeof(T);
    }

    template <typename T>
    size_t copy_from_buffer(const char* buffer, T* object, size_t buffer_size)
    {
        if (sizeof(T) > buffer_size) return 0;
        char* object_as_bytes = (char*)object;
        for (int i = 0; i < sizeof(T); i++) {
            object_as_bytes[i] = buffer[i];
        }
        return sizeof(T);
    }

    size_t copy_to_buffer(char* buffer, int* object, size_t buffer_size)
    {
        if (buffer_size < sizeof(int)) return 0;
        char* object_as_bytes = (char*)object;
        for (int i = 0; i < sizeof(int); i++) {
            buffer[i] = object_as_bytes[i];
        }
        return sizeof(int);
    }

    // Second option: Serialize as binary
    size_t SerializeGameObjectAsBytes(const Player* go, char* buffer, size_t buffer_size)
    {
        size_t bytes_written = 0;
        bytes_written += copy_to_buffer(&buffer[bytes_written], &go->position.x, buffer_size - bytes_written);
        bytes_written += copy_to_buffer(&buffer[bytes_written], &go->position.y, buffer_size - bytes_written);
        return bytes_written;
    }

    size_t DeserializeGameObjectAsBytes(Player* go, char* buffer, size_t buffer_size)
    {
        int id = -1;

        //check ID
        copy_from_buffer(&buffer[0], &id, buffer_size - sizeof(int));
        
        if (isIDValid(id)) {
            std::cout << "ID WAS VALID [ " << id << " ]\n";
        }
        else {
            std::cout << "ID WAS INVALID [ " << id << " ]\n";
            return -1;
        }

        size_t bytes_read = 0;

        bytes_read += copy_from_buffer(&buffer[bytes_read], &go->position.x, buffer_size - bytes_read);
        bytes_read += copy_from_buffer(&buffer[bytes_read], &go->position.y, buffer_size - bytes_read);

        return bytes_read;
    }

    bool isIDValid(int id) {

        //ID stayed invalid
        if (id == -1) {
            return false;
        }

        //id is just wrong
        if (id != 0 && id !=1 && id != 2) {
            return false;
        }

        return true;
    }

    //// Second option: Serialize as binary
    //size_t SerializeGameObjectAsBytes(const Player* go, char* buffer, size_t buffer_size)
    //{
    //    size_t bytes_written = 0;
    //    bytes_written += copy_to_buffer(&buffer[bytes_written], &go->position.x, buffer_size - bytes_written);
    //    bytes_written += copy_to_buffer(&buffer[bytes_written], &go->position.y, buffer_size - bytes_written);
    //    return bytes_written;
    //}

    //size_t DeserializeGameObjectAsBytes(Player* go, char* buffer, size_t buffer_size)
    //{
    //    size_t bytes_read = 0;

    //    bytes_read += copy_from_buffer(&buffer[bytes_read], &go->position.x, buffer_size - bytes_read);
    //    bytes_read += copy_from_buffer(&buffer[bytes_read], &go->position.y, buffer_size - bytes_read);

    //    return bytes_read;
    //}
};