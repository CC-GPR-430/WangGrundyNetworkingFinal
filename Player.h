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

    char msgBuffer[4096];

public:

    void Init()
    {
        //create address struct
        address = new Address(HOST, PORT);

        //create a TCP socket for the server to use
        sendSock = new Socket(Socket::Family::INET, Socket::Type::STREAM);

        //bind address to socket
        sendSock->Connect(*address);

        //sendSock->SetNonBlockingMode(true);

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
        char buffer[4096];
        size_t nbytes_recved = sendSock->Recv(buffer, sizeof(buffer));

        if (nbytes_recved == -1 || nbytes_recved == 0) {
            return;
        }

        std::cout << "recieve message here???\n";
        std::string msgRecieved(buffer, nbytes_recved);
        std::cout << msgRecieved << std::endl;
    }
};