#ifndef SERVER_INCLUDE
#define SERVER_INCLUDE

#include <iostream>
#include <map>
#include <thread>

#include "player.hpp"

class Game;

class Server {
    public:
        Server(int id = -1, int socket = 0, Game* game = nullptr);
        ~Server();

        void Update();

        inline int GetSocket() { return socket; }

        void MsgReception();

        void SpawnFor(Player& p);

        void AddPlayer(int index, bool withMsg = true);
        void RemovePlayer(int index, bool withMsg = true);
        inline bool ContainsPlayer(int index) { return players->count(index) != 0; }

    private:
        int id;
        int socket;
        Game* game;

        float x = 0;
        float z = 0;

        float size_x = 10;
        float size_y = 10;
        float size_z = 10;

        std::map<int, Player>* players;

        std::thread* msgReceptionThread;

    public:
        inline int Id() { return id; }

        inline float X() { return x; }
        inline float Z() { return z; }

        inline float Size_x() { return size_x; }
        inline float Size_y() { return size_y; }
        inline float Size_z() { return size_z; }  
        inline void SetPlayers(std::map<int, Player>* p) { delete players; players = p; }
};

#endif