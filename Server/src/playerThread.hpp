#ifndef PLAYERTHREAD_HPP
#define PLAYERTHREAD_HPP

#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include <chrono>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include "game.hpp"
#include "player.hpp"

class PlayerThread {
    public:
        PlayerThread(int client, Player& player, Game& game);
        ~PlayerThread();

        int GetSocket() { return client; }

        void Run();

        bool running = false;
    private:
        void Loop();

        std::thread* loop;

        int client;
        Player& player;

        Game& game;
};

#endif