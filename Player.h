#pragma once

using namespace std;

class Player {

private:
    Address* address;
    const char* const HOST = "127.0.0.1";
    const int PORT = 12345;
    Socket* sendSock;
    std::string userMessage;
    std::string userInput;

public:

    void Init()
    {
        //create address struct
        address = new Address(HOST, PORT);

        //create a TCP socket for the server to use
        sendSock = new Socket(Socket::Family::INET, Socket::Type::STREAM);

        //bind address to socket
        (*sendSock).Connect(*address);
    }
   
    void SendMessage(std::string messageInput) {

        //Send a properly - formatted sort string to the server.
        size_t nbytes_sent = (*sendSock).Send(messageInput.data(), messageInput.size());

        //Receive the server's response.
        char buffer[4096];
        size_t nbytes_recved = (*sendSock).Recv(buffer, sizeof(buffer));

        //Create a std::string from the server's response and return it.
        std::string msgRecieved(buffer, nbytes_recved);
        std::cout << msgRecieved << std::endl;
    }
};