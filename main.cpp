#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <deque>

#include <socklib.h>
#include <defer.h>

#include <SDL.h>
#include <SDL_ttf.h>
#include "GameObject.h"
#include "Player.h"
#include "Data.h"

SDL_Rect rect;
SDL_Renderer* renderer;
bool bothPlayersIn = false;

void StartRender();

void fatal(bool check, const char* msg) {
    if (check) {
        perror(msg);
        exit(1);
    }
}

class MessageLog : public std::deque<std::string>
{
public:
    MessageLog(size_t maxRows = 20) :
        std::deque<std::string>(maxRows, "")
    {}

    void AddMessage(const std::string& message)
    {
        pop_front();
        push_back(message);
    }
};

void RenderText(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, const std::string& text, int x, int y)
{
    // Draw to surface and create a texture
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Setup the rect for the texture
    SDL_Rect dstRect;
    dstRect.x = x;
    dstRect.y = y;
    SDL_QueryTexture(texture, nullptr, nullptr, &dstRect.w, &dstRect.h);

    // Draw the texture
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

    // Destroy the surface/texture
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

// Used for rendering messages
enum MessageSource {
    USER,
    SERVER
};

// Not for student use. Adds a decoded character to the typing queue.
void pushch(char c);

// Gets the next character from the typing queue
// or returns EOF.
int getch();

bool is_backspace(char c) { return c == '\b'; }
bool is_newline(char c) { return c == '\n' || c == '\r'; }

// Adds a message to the log to be rendered.
void append_message(const std::string& msg, MessageSource src);

static MessageLog messageLog(8);

class MessageHolder {
private:
    Address* address;
    const char* const HOST = "68.183.63.165";
    const int PORT = 24925;
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

    // Do all the work for a given frame
    void Update(float dt)
    {
        //add all inputs if there is something
        char tempChar = getch();

        if (tempChar == EOF) {
            return;
        }

        //collect all inputs
        while (tempChar != EOF) {
            userInput += tempChar;
            tempChar = getch();
        }

        char c = userInput[0];
        int index = 0;

        while (c != '\0') {
            //check for spaces
            if (is_backspace(c)) {
                //check if this is last character
                if (index == 0) {
                    //first char in our message, don't delete anything
                    //delete itself
                    userInput.erase(index, 1);
                    c = userInput[index];
                    continue;
                    //std::cout << "after: \"" << userInput << "\"" << std::endl;
                }
                else if (userInput.find(c) == std::string::npos) {
                    std::cout << "char doesn't exist, something went wrong - wang\n";
                }
                else {
                    //remove last character

                    //remove this char
                    //remove last char
                    //starts at index - 1, removes last char and the backspace.
                    userInput.erase(index - 1, 2);
                    index -= 2;
                    //std::cout << "after: \"" << userInput << "\"" << std::endl;
                }

            }

            if (is_newline(c)) {

                userMessage.clear();

                //copy all data up to index - 1 into user message
                for (int i = 0; i < index; i++) {
                    userMessage += userInput[i];
                }
                std::cout << "sent message: " << userMessage << std::endl;

                append_message(userMessage, USER);
                size_t nbytes_sent = (*sock).Send(userMessage.data(), userMessage.size());
                userInput.clear();

                char buffer[4096];
                size_t nbytes_recved = (*sock).Recv(buffer, sizeof(buffer));

                //Create a std::string from the server's response and return it.
                std::string msgRecieved(buffer, nbytes_recved);
                append_message(msgRecieved, SERVER);
                userMessage.clear();

                break;
            }

            index++;
            c = userInput[index];

            //update message holder
            userMessage.clear();

            //copy all data up to index - 1 into user message
            for (int i = 0; i < index; i++) {
                userMessage += userInput[i];
            }

            std::cout << "current message: " << userMessage << std::endl;
        }
    }
};

int RunProgram();
int RunTests();

int main(int argc, char* argv[]) {
    SockLibInit();
    atexit(SockLibShutdown);

    if (argc > 1) {
        //return RunTests();
    }

    SDL_Init(SDL_INIT_EVERYTHING);
    if (TTF_Init() == -1) {
        std::cerr << "TTF failed to initialize\n";
    }

    TTF_Font* font = TTF_OpenFont("Carlito-Regular.ttf", 18);
    SDL_Window* window;

    fatal(SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer) != 0, "CreateWindow");

    Uint64 last_frame = SDL_GetTicks64();
    float target_dt = 1 / 60.f;

    MessageHolder messageHolder;
    messageHolder.Init();

    puts("Initialized.");

    bool quit = false;
    while (!quit) {
        Uint64 frame_start = SDL_GetTicks64();
        float dt = (frame_start - last_frame) / 1000.f;
        if (dt < target_dt)
            continue;

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                }

                unsigned char key = e.key.keysym.sym;
                // Printable characters
                if (e.key.keysym.mod & KMOD_SHIFT) {
                    // capitalization
                    if (key >= 97 && key <= 122) key -= 0x20;
                    // symbols don't seem to have a nice mapping -- this only
                    // works with QWERTY keyboards, though :/
                    if (key == '1') key = '!';
                    if (key == '2') key = '@';
                    if (key == '3') key = '#';
                    if (key == '4') key = '$';
                    if (key == '5') key = '%';
                    if (key == '6') key = '^';
                    if (key == '7') key = '&';
                    if (key == '8') key = '*';
                    if (key == '9') key = '(';
                    if (key == '0') key = ')';
                    if (key == '`') key = '~';
                    if (key == '-') key = '_';
                    if (key == '=') key = '+';
                    if (key == '/') key = '?';
                    if (key == ';') key = ':';
                }
                if (key == '\b' || key == '\r' || key == '\n' || (key >= 32 && key <= 127))
                    pushch(key);
            }
            else if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        messageHolder.Update(dt);

        SDL_SetRenderDrawColor(renderer, 0, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_Color fg = { 0, 0, 0, SDL_ALPHA_OPAQUE };
        RenderText(renderer, font, fg, messageHolder.GetCurrentMessage(), 10, 480 - 26);

        //Render function here /////////////////////////////////////////
        StartRender();

        int penY = 0;
        for (const std::string& message : messageLog) {
            RenderText(renderer, font, fg, message.c_str(), 0, penY);
            penY += 18;
        }

        SDL_RenderPresent(renderer);

        last_frame = frame_start;
    }

    SDL_Quit();

    return 0;
}

int RunTests() {
#define MSG messageHolder.GetCurrentMessage()
    MessageHolder messageHolder;
    float dt = 1 / 60.f;
    messageHolder.Init();

    pushch('a');
    pushch('b');
    pushch('c');
    if (!MSG.empty()) {
        puts("FAIL: Characters prematurely added to messageHolder.");
        return 1;
    }

    messageHolder.Update(dt);
    if (MSG != "abc") {
        printf("FAIL: messageHolder contains %s but should have %s.\n", MSG.c_str(), "abc");
        return 1;
    }

    messageHolder.Update(dt);
    if (MSG != "abc") {
        printf("FAIL: Two consecutive calls to Update() changed message from \"abc\" to "
            "\"%s\". Message should stay the same if no new inputs occurred.\n",
            MSG.c_str());
        return 1;
    }

    pushch('\r');
    messageHolder.Update(dt);
    if (!MSG.empty()) {
        puts("FAIL: 'enter' key did not clear message buffer");
        return 1;
    }

    // Wait a little bit for the message to get back to us
    SDL_Delay(100);
    messageHolder.Update(dt);

    int msgCount = 0;
    for (const std::string& str : messageLog) {
        if (!str.empty()) msgCount++;
    }

    if (msgCount != 2) {
        printf("FAIL: %d messages in message queue, but should be 2\n", msgCount);
        return 1;
    }

    puts("All tests passed!");
    return 0;
#undef MSG
}

static std::deque<char> typing_queue;

void pushch(char c) {
    typing_queue.push_back(c);
}

int getch() {
    if (typing_queue.empty())
        return EOF;

    int c = typing_queue.front();
    typing_queue.pop_front();
    return c;
}

void append_message(const std::string& msg, MessageSource src) {
    std::string prefix(src == USER ? "User: " : "Server: ");
    messageLog.AddMessage(prefix + msg);
}

void StartRender() {
    SDL_Rect rect;

    rect.x = 0;
    rect.y = 0;
    rect.w = 500;
    rect.h = 500;

    SDL_SetRenderDrawColor(renderer, 0, 1, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rect);
}
