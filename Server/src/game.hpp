#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include <algorithm>

#include <chrono>
#include <ctime>
#include <ratio>
#include <chrono>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include "player.hpp"
#include "server.hpp"
#include "NetworkMessages/networkMessage.hpp"

class Game {
    public:
        Game(int serverId, std::vector<int>& clients, std::map<int, Server*>& servers, std::map<int, Player>& players);

        void Run();

        void Spawn(int prefabId, int owner, bool hasAuthority = false);
        void SpawnPlayer(int client);
        void Despawn(int objectId);
        void DespawnPlayer(int clientSocket);

        void SendMsgTo(NetworkMessage* msg, int targetId);
        void SendMsgTo(NetworkMessage* msg, Player& target);
        void SendMsgToAllInterested(NetworkMessage* msg, Player* p1);
        void SendMsgToAllNotInterested(NetworkMessage* msg, Player* p1);        
        void SendMsgToAll(NetworkMessage* msg, Player* p1 = NULL, bool interestedOnly = true);

        void RegisterServer(int id, int serverSocket);

    private:
        void Loop();

        std::thread* loop;

        Server server;

        std::vector<int>& clients;
        std::map<int, Server*>& servers;
        std::map<int, Player>& players;

        int lastValidId = 10000;
        std::map<int, int> objects; // objectId, ownerId TODO: networkedObject (json data?)
};

#endif