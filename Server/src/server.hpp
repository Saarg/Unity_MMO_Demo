#ifndef SERVER_INCLUDE
#define SERVER_INCLUDE

#include <map>
#include "player.hpp"

class Server {
    public:
        Server(int id = -1, int socket = 0);
        ~Server();

    private:
        int id;
        int socket;

        float x = 0;
        float y = 0;

        float size_x = 10;
        float size_y = 10;
        float size_z = 10;

        std::map<int, Player> players;
};

#endif