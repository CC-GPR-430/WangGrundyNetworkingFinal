#pragma once
#include "CONSTS.h"

#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
#include <stdlib.h>

#include "socklib.h"
#include "defer.h"

#include "Data.h"

using namespace std;

struct Position {
    float x;
    float y;
};

class Player{

private:
    Address* address;
    const char* const HOST = "127.0.0.1";
    const int PORT = 12345;
    Socket* sendSock;
    std::string userMessage;
    std::string userInput;

    char msgBuffer[4096];
    
public:
    float changeX, changeY, changeZ;
    Position position;
    float moveSpeed;

    Player() {
        moveSpeed = 2;

        position.x = 200;
        position.y = 200;
    }

    void Init()
    {
        //create address struct
        address = new Address(HOST, PORT);

        //create a TCP socket for the server to use
        sendSock = new Socket(Socket::Family::INET, Socket::Type::STREAM);

        //bind address to socket
        sendSock->Connect(*address);

        sendSock->SetNonBlockingMode(true);

        SendMessage("ready");
    }
   
    void SendMessage(std::string messageInput) {

        //Send a properly - formatted sort string to the server.
        size_t nbytes_sent = sendSock->Send(messageInput.data(), messageInput.size());

        //Receive the server's response.
        char buffer[4096];
        size_t nbytes_recved = sendSock->Recv(buffer, sizeof(buffer));

        if (nbytes_recved == -1 || nbytes_recved == 0) {
            return;
        }

        std::string msgRecieved(buffer, nbytes_recved);
        std::cout << msgRecieved << std::endl;
    }

    void Update() {

        TrySend();
        
        TryRecv();
    }

    void TryRecv() {
        //wait a little
        float wait_time = consts::INITIAL_TIMEOUT;
        //sendSock->SetTimeout(consts::INITIAL_TIMEOUT);

        char buffer[4096];
        size_t nbytes_recved = sendSock->Recv(buffer, sizeof(buffer));

        if (nbytes_recved == -1 || nbytes_recved == 0) {

            /* if (sendSock->GetLastError() == Socket::Error::SOCKLIB_ETIMEDOUT)
            {
                std::cout << "Timed out. Maybe retrying...\n";
                sendSock->SetTimeout(consts::INITIAL_TIMEOUT);
            }*/
            return;
        }

        std::string msgRecieved(buffer, nbytes_recved);
        std::cout << msgRecieved << std::endl;
    }

    void TrySend() {
        string msg = "From Player\n";
        //size_t nbytes_recved = sendSock->Send(msg.data(), msg.size());
        char buffer[4092];
        SerializeGameObjectAsBytes(this, buffer, sizeof(buffer));
        
        sendSock->Send(buffer, msg.size());
    }

    //////////////
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
        size_t bytes_read = 0;

        bytes_read += copy_from_buffer(&buffer[bytes_read], &go->position.x, buffer_size - bytes_read);
        bytes_read += copy_from_buffer(&buffer[bytes_read], &go->position.y, buffer_size - bytes_read);

        return bytes_read;
    }
};