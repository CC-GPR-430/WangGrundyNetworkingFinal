class Player {
private:
    Address* address;
    const char* const HOST = "127.0.0.1";
    const int PORT = 12345;
    Socket* sock;
    std::string userMessage;
    std::string userInput;

public:

    // Do whatever setup is necessary
    void Init()
    {
        //create address struct
        address = new Address(HOST, PORT);

        //create a TCP socket for the server to use
        sock = new Socket(Socket::Family::INET, Socket::Type::STREAM);

        //bind address to socket
        (*sock).Connect(*address);
    }

    // Used for rendering
    std::string GetCurrentMessage()
    {
        return userMessage;
    }

    //// Do all the work for a given frame
    //void Update(float dt)
    //{
    //    //add all inputs if there is something
    //    char tempChar = getch();

    //    if (tempChar == EOF) {
    //        return;
    //    }

    //    //collect all inputs
    //    while (tempChar != EOF) {
    //        userInput += tempChar;
    //        tempChar = getch();
    //    }

    //    char c = userInput[0];
    //    int index = 0;

    //    while (c != '\0') {
    //        //check for spaces
    //        if (is_backspace(c)) {
    //            //check if this is last character
    //            if (index == 0) {
    //                //first char in our message, don't delete anything
    //                //delete itself
    //                userInput.erase(index, 1);
    //                c = userInput[index];
    //                continue;
    //                //std::cout << "after: \"" << userInput << "\"" << std::endl;
    //            }
    //            else if (userInput.find(c) == std::string::npos) {
    //                std::cout << "char doesn't exist, something went wrong - wang\n";
    //            }
    //            else {
    //                //remove last character

    //                //remove this char
    //                //remove last char
    //                //starts at index - 1, removes last char and the backspace.
    //                userInput.erase(index - 1, 2);
    //                index -= 2;
    //                //std::cout << "after: \"" << userInput << "\"" << std::endl;
    //            }

    //        }

    //        if (is_newline(c)) {

    //            userMessage.clear();

    //            //copy all data up to index - 1 into user message
    //            for (int i = 0; i < index; i++) {
    //                userMessage += userInput[i];
    //            }
    //            std::cout << "sent message: " << userMessage << std::endl;

    //            append_message(userMessage, USER);
    //            size_t nbytes_sent = (*sock).Send(userMessage.data(), userMessage.size());
    //            userInput.clear();

    //            char buffer[4096];
    //            size_t nbytes_recved = (*sock).Recv(buffer, sizeof(buffer));

    //            //Create a std::string from the server's response and return it.
    //            std::string msgRecieved(buffer, nbytes_recved);
    //            append_message(msgRecieved, SERVER);
    //            userMessage.clear();

    //            break;
    //        }

    //        index++;
    //        c = userInput[index];

    //        //update message holder
    //        userMessage.clear();

    //        //copy all data up to index - 1 into user message
    //        for (int i = 0; i < index; i++) {
    //            userMessage += userInput[i];
    //        }

    //        std::cout << "current message: " << userMessage << std::endl;
    //    }
    //}
};