#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include <chrono>

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include "game.hpp"

class Login {
    public:
        Login(int const port, Game& game);

        void Run();
        void Loop();

    private:
        int sockfd;
        struct sockaddr_in address;

        std::thread* loop;

        Game& game;
};

#endif