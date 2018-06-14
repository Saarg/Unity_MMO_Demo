#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include <chrono>
#include <algorithm>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include "player.hpp"
#include "server.hpp"

class Game {
    public:
        Game(int serverId, std::vector<int>& clients, std::map<int, Server>& servers, std::map<int, Player>& players);

        void Run();

        void Spawn(int prefabId, int owner, bool hasAuthority = false);
        void SpawnPlayer(int client);
        void Despawn(int objectId);
        void DespawnPlayer(int clientSocket);

        void RegisterServer(int id, int serverSocket);

    private:
        void Loop();

        std::thread* loop;

        int serverId;

        float x = 0;
        float y = 0;

        float size_x = 10;
        float size_y = 10;
        float size_z = 10;

        std::vector<int>& clients;
        std::map<int, Server>& servers;
        std::map<int, Player>& players;

        int lastValidId = 10000;
        std::map<int, int> objects; // objectId, ownerId TODO: networkedObject (json data?)
};

#endif